/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:07:27 by sponthus          #+#    #+#             */
/*   Updated: 2025/01/29 13:17:55 by sponthus         ###   ########.fr       */
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

#include "Client.hpp"

# define BUFF_SIZE 512

class Server {
	public: 
		Server(int port, std::string pw);
		Server(const Server &src);
		~Server();

		int			getPort() const;
		std::string	getPW() const;

		void	initSocket();
		void	initPoll(int fd);
		void	initClient(int fd, struct sockaddr_in ClientAddress);
		void	init();

		void	recieveData(int fd);
		void	connectClient();
		void	clearClient(int fd);

	private :
		Server();
		int			_port;
		std::string _pw;
		int			_socketFD;
		bool		_sig;

		std::vector<struct pollfd>	_fds;
		std::vector<Client>			_clients;
		
};

bool	isValidPW(std::string arg);
bool	isValidPort(std::string arg);

#endif