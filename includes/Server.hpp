/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:07:27 by sponthus          #+#    #+#             */
/*   Updated: 2025/02/11 10:13:51 by endoliam         ###   ########lyon.fr   */
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
#include "Command.hpp"
#include "Channel.hpp"

#include "Builder.hpp"

# define BUFF_SIZE 512

class Command;
class Channel;
class Client;

class Server {
	public: 
		Server(int port, std::string pw);
		~Server();

		int			getPort() const;
		const std::string	getPW() const;

		void	initClient(int fd, struct sockaddr_in ClientAddress);
		void	init();
		void	SetCmdMap();
		void	initChannel(Client *client, std::string name);
		void	run();

		std::string	recieveData(int fd, std::string msg);
		void		connectClient();
		void		clearClient(int fd);

		void	sendData(int fd, std::string response) const;
		void	SendToGroup(const std::vector<Client *> clients, const std::string message) const;
		// TODO function to send to all channels, excluding double response for users in 2 channels
		void	SendToNick(const Client *sender, const std::string nick, const std::string message) const;
		void	SendToClient(const Client *client, const std::string message) const;
		
		bool	isChannel(std::string name);
		Channel*	getChannel(std::string name);

	private :
		Server();
		void	initSocket();
		void	initPoll(int fd);

		int			_port;
		std::string _pw;
		int			_socketFD;

		std::map<std::string, void(Command::*)(std::list<std::string> *arg)>	CmdMap;
	

		std::vector<struct pollfd>			_fds;
		std::vector<Client *>				_Clients;
		std::map<std::string, Client *>		_ClientsByNick; // Add nickname validation
		std::map<int, Client *>				_ClientsByFD;
		std::map<std::string, Channel *>	_ChannelsByName;
};

bool	isValidPW(std::string arg);
bool	isValidPort(std::string arg);


#endif