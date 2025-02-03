/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:07:27 by sponthus          #+#    #+#             */
/*   Updated: 2025/02/03 10:51:40 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <cstring> // strerror
#include <iostream>
#include <vector>
#include <cstdlib> // atoi()
#include <sys/socket.h> // socket(), address
#include <netinet/in.h> // IP specialties 
#include <unistd.h> // fcntl
#include <fcntl.h>
#include <poll.h>
#include <netinet/in.h> // inet_ntoa
#include <arpa/inet.h> // inet_ntoa
#include <cerrno> 
#include <map>

#include "Client.hpp"
#include "Channel.hpp"

# define BUFF_SIZE 512

class Server {
	public: 
		Server(int port, std::string pw);
		~Server();

		int			getPort() const;
		const std::string	getPW() const;

		void	initClient(int fd, struct sockaddr_in ClientAddress);
		void	initChannel(Client *client, std::string name);
		void	run();

		std::string	recieveData(int fd, std::string msg);
		void	connectClient();
		void	clearClient(int fd);

		void	sendData(int fd, std::string response) const;

	private :
		Server();
		void	initSocket();
		void	initPoll(int fd);

		int			_port;
		std::string _pw;
		int			_socketFD;

		std::vector<struct pollfd>			_fds;
		std::vector<Client *>				_Clients;
		std::map<std::string, Client *>		_ClientsByNick; // Add nickname validation
		std::map<int, Client *>				_ClientsByFD;
		std::map<std::string, Channel *>	_ChannelsByName;
};

bool	isValidPW(std::string arg);
bool	isValidPort(std::string arg);

#endif