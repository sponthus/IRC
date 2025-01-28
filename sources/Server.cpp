/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:00:24 by sponthus          #+#    #+#             */
/*   Updated: 2025/01/28 17:17:48 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Private
Server::Server()
{}

Server::Server(int port, std::string pw) : _port(port), _pw(pw), _socketFD(-1), _sig(false)
{

}

Server::Server(const Server &src) : _port(src.getPort()), _pw(src.getPW()), _socketFD(-1), _sig(false)
{
}

Server::~Server()
{

}

int	Server::getPort() const
{
	return this->_port;
}

std::string	Server::getPW() const
{
	return this->_pw;
}

void	Server::initSocket()
{
	this->_socketFD = socket(PF_INET, SOCK_STREAM, 0);
	if (this->_socketFD == -1)
	{
		throw std::runtime_error("socket creation failed");
	}

	int	val = 1;
	// IPPROTO_IP for IP protocols or SOL_SOCKET for general configuration ?
	if (setsockopt(this->_socketFD, IPPROTO_IP, SO_REUSEADDR, &val, sizeof(val)) == -1) // Parameters to allow immediate reuse of the port without waiting time
	{
		throw std::runtime_error("setsockopt failed on socket");
	}
	
	struct sockaddr_in	address;
	address.sin_addr.s_addr = INADDR_ANY; // Accepts connexions on every network interface
	address.sin_family = AF_INET; // IPV4
	address.sin_port = htons(this->_port); // Links port and network (converts byte orders)
	if (fcntl(this->_socketFD, F_SETFL, O_NONBLOCK) == -1) // Configuration in non-blocking, can treat info while listening
	{
		throw std::runtime_error("fcntl failed on socket");
	}

	if (bind(this->_socketFD, (struct sockaddr *)&address, sizeof(address)) == -1) // Affects the address to the socket
	{
		throw std::runtime_error("bind failed on socket");
	}

	if (listen(this->_socketFD, SOMAXCONN) == -1) // Defines max connexions, limit is SOMAXCONN
	{
		throw std::runtime_error("listen failed on socket");
	}
}

void	Server::initPoll()
{
	struct pollfd poll;
	poll.fd = this->_socketFD;
	poll.events = POLLIN | POLLHUP | POLLERR;
	poll.revents = 0;
	this->_fds.push_back(poll);
}

void	Server::recieveData(int fd) // fd from the client that sent a msg
{
	std::cout << "Recieving data from " << fd << std::endl;
	char	buffer[1024];
	for (size_t i = 0; i < 1024 - 1; i++)
		buffer[i] = 0;

	ssize_t	size = recv(fd, buffer, sizeof(buffer) - 1, 0);
	
	if (size <= 0)
	{
		std::cout << "Client disconnected, fd = " << fd << std::endl;
		clearClient(fd);
		close(fd);
	}
	else
	{
		buffer[size] = '\0';
		std::cout << fd << " sent : //" << buffer << "//" << std::endl;
		// Process data sent
	}
}

void	Server::clearClient(int fd)
{
	for (size_t i = 0; i < this->_fds.size(); i++)
	{
		if (this->_fds[i].fd == fd)
		{
			this->_fds.erase(this->_fds.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].getFD() == fd)
		{
			this->_clients.erase(this->_clients.begin() + i);
			break;
		}
	}

}

void	Server::acceptClient()
{
	std::cout << "Accepting client" << std::endl;
	struct sockaddr_in	ClientAddress;
	socklen_t			LenAddress = sizeof(ClientAddress);

	int	fd = accept(this->_socketFD, (sockaddr *)&ClientAddress, &LenAddress);
	if (fd == -1)
	{
		std::cerr << "Accept failed" << std::endl;
		return ; // Throw error ?
	}

	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "fcntl failed on new client" << std::endl;
		return ; // Throw error ?
	}

	struct pollfd poll;
	poll.fd = fd;
	poll.events = POLLIN | POLLHUP | POLLERR;
	poll.revents = 0;
	this->_fds.push_back(poll);

	Client	client;
	client.setFD(fd);
	client.setAddress(inet_ntoa(ClientAddress.sin_addr)); // Converts IP address to a string
	this->_clients.push_back(client);

	std::cout << "Client " << fd << " connected" << std::endl;
}

void	Server::init()
{
	initSocket();
	initPoll();

	std::cout << " >> Waiting for connections << " << std::endl;
	while (this->_sig == false)
	{
		if (poll(&(this->_fds[0]), this->_fds.size(), -1) == -1) // & sig == false ?, waits for messages
			throw(std::runtime_error("poll failed"));
		std::cout << "something detected" << std::endl;
		for (size_t i = 0; i < this->_fds.size(); i++)
		{
			if (this->_fds[i].revents != 0)
			{
				std::cout << "found it comes from " << this->_fds[i].fd << std::endl;
				if (this->_fds[i].fd == this->_socketFD)
					acceptClient();
				else if (this->_fds[i].revents & POLLIN)
					recieveData(this->_fds[i].fd);
				else if (this->_fds[i].revents & (POLLHUP | POLLERR))
				{
					std::cout << "Client " << this->_fds[i].fd << " disconnected or error" << std::endl;
					clearClient(this->_fds[i].fd);
				}
				this->_fds[i].revents = 0;
			}
		}
	}
}