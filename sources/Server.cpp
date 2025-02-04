/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:00:24 by sponthus          #+#    #+#             */
/*   Updated: 2025/02/04 13:41:54 by endoliam         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"

// Private
Server::Server()
{
}

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

void	Server::SetCmdMap()
{
	this->CmdMap["KICK"] = &Command::Kick;
	this->CmdMap["INVITE"] = &Command::Invite;
	this->CmdMap["TOPIC"] = &Command::Topic;
	this->CmdMap["MODE"] = &Command::Mode;
	return ;
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

void	Server::initPoll(int fd)
{
	struct pollfd poll;
	poll.fd = fd;
	poll.events = POLLIN;
	poll.revents = 0;
	this->_fds.push_back(poll);
}

std::string	Server::recieveData(int fd, std::string msg) // fd from the client that sent a msg
{
	ssize_t	size = 1;
	char	buffer[BUFF_SIZE];
	for (size_t i = 0; i < BUFF_SIZE - 1; i++) // memeset
		buffer[i] = 0;

	size = recv(fd, buffer, sizeof(buffer) - 1, 0);
	std::string str = msg;
	if (size < 0)
	{
		if (errno == EWOULDBLOCK || errno == EAGAIN)
        {
            std::cout << "Would block, trying again later" << std::endl;
        }
        std::cout << "recv() error: " << strerror(errno) << std::endl;
        clearClient(fd);
	}
	else if (size == 0)
	{
		std::cout << "Client closed connection properly" << std::endl;
        clearClient(fd);
	}
	else
	{
		buffer[size] = '\0';
		std::cout << size << std::endl;
		str += buffer;
		if (size + 1 == BUFF_SIZE)
			str = recieveData(fd, str);
	}
	//parsing commande
	return (str);
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

void	Server::initClient(int fd, struct sockaddr_in ClientAddress)
{
	Client client;
	client.setFD(fd);
	client.setAddress(inet_ntoa(ClientAddress.sin_addr));
	this->_clients.push_back(client);
}

void	Server::connectClient()
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
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "fcntl failed on new client" << std::endl;
		return ; // Throw error ?
	}

	try
	{
		initClient(fd, ClientAddress);
        initPoll(fd);

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

void	Server::sendData(int fd, std::string response) const // A surcharger avec tout un channel au lien de 1 FD
{
	if (send(fd, response.c_str(), sizeof(response.c_str()), 0) == -1)
	{
		std::cerr << "send failed on response : " << response << std::endl;
	}
}

void	Server::init()
{
	initSocket();
	initPoll(this->_socketFD);

	std::cout << " >> Waiting for connections << " << std::endl;
	while (this->_sig == false)
	{
		if (poll(&(this->_fds[0]), this->_fds.size(), -1) == -1) // & sig == false ?, waits for messages
			throw(std::runtime_error("poll failed"));
		for (size_t i = 0; i < this->_fds.size(); i++)
		{
			if (this->_fds[i].revents != 0)
			{
				std::cout << "Event detected on fd " << this->_fds[i].fd << ": revent = " << this->_fds[i].revents << std::endl;
				
				if (this->_fds[i].fd == this->_socketFD)
				{
					connectClient();
				}
				else if (this->_fds[i].revents & POLLNVAL)
				{
					std::cout << "POLLNVAL detected for fd " << this->_fds[i].fd << std::endl;
					clearClient(this->_fds[i].fd);
				}
				else if (this->_fds[i].revents & POLLIN)
				{
					std::string message = recieveData(this->_fds[i].fd, "");
					std::cout << this->_fds[i].fd << " sent : //" << message << "//" << std::endl;

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
