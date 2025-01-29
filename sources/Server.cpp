/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:00:24 by sponthus          #+#    #+#             */
/*   Updated: 2025/01/29 11:31:07 by sponthus         ###   ########.fr       */
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
	this->_socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socketFD <= 0)
	{
		throw std::runtime_error("socket creation failed");
	}

	int	val = 1;
	// IPPROTO_IP for IP protocols or SOL_SOCKET for general configuration ?
	if (setsockopt(this->_socketFD, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1) // Parameters to allow immediate reuse of the port without waiting time
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
	poll.events = POLLIN;
	poll.revents = 0;
	this->_fds.push_back(poll);
}

void	Server::recieveData(int fd) // fd from the client that sent a msg
{
	ssize_t	size = 1;
	char	buffer[1024];
	for (size_t i = 0; i < 1024 - 1; i++)
		buffer[i] = 0;

	size = recv(fd, buffer, sizeof(buffer) - 1, 0);
	std::cout << "recv() returned " << size << " for fd " << fd << std::endl;
	if (size < 0)
	{
		if (errno == EWOULDBLOCK || errno == EAGAIN)
        {
            std::cout << "Would block, trying again later" << std::endl;
            return;
        }
        std::cout << "recv() error: " << strerror(errno) << std::endl;
        clearClient(fd);
        return;
	}
	else if (size == 0)
	{
		std::cout << "Client closed connection properly" << std::endl;
        clearClient(fd);
        return;
	}
	else
	{
		buffer[size] = '\0';
		std::cout << fd << " sent : //" << buffer << "//" << std::endl;
	}
}

void	Server::clearClient(int fd)
{
	close(fd);
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
	std::cout << "New connection accepted on fd " << fd << std::endl;
	if (fcntl(fd, F_GETFL) == -1) 
    {
        std::cerr << "Failed to get socket flags: " << strerror(errno) << std::endl;
        return;
    }
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "fcntl failed on new client" << std::endl;
		return ; // Throw error ?
	}

	try
	{
        Client client;
        client.setFD(fd);
        client.setAddress(inet_ntoa(ClientAddress.sin_addr));
        this->_clients.push_back(client);

        struct pollfd poll;
        poll.fd = fd;
        poll.events = POLLIN;
        poll.revents = 0;
        this->_fds.push_back(poll);

        std::cout << "Client " << fd << " connected successfully with IP " 
                  << inet_ntoa(ClientAddress.sin_addr) << std::endl;
    }
    catch (const std::exception& e)
	{
        std::cerr << "Error setting up client: " << e.what() << std::endl;
        close(fd);
        return;
    }
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
		for (size_t i = 0; i < this->_fds.size(); i++)
		{
			if (this->_fds[i].fd != this->_socketFD)  // Ne pas tester le socket serveur
			{
				if (fcntl(this->_fds[i].fd, F_GETFL) == -1)
				{
					std::cout << "fd " << this->_fds[i].fd << " became invalid: " << strerror(errno) << std::endl;
				}
			}
			if (this->_fds[i].revents != 0)
			{
				std::cout << "Event detected on fd " << this->_fds[i].fd << ": revent = " << this->_fds[i].revents << std::endl;
				
				if (this->_fds[i].fd == this->_socketFD)
				{
					acceptClient();
				}
				else if (this->_fds[i].revents & POLLNVAL)
				{
					std::cout << "POLLNVAL detected for fd " << this->_fds[i].fd << std::endl;
					clearClient(this->_fds[i].fd);
				}
				else if (this->_fds[i].revents & POLLIN)
				{
					recieveData(this->_fds[i].fd);
				}
				else if (this->_fds[i].revents & (POLLHUP | POLLERR))
				{
					std::cout << "POLLHUP or POLLERR detected for fd " << this->_fds[i].fd << std::endl;
					clearClient(this->_fds[i].fd);
				}
				this->_fds[i].revents = 0;
			}
		}
	}
}
