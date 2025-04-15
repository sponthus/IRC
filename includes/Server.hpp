/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:07:27 by sponthus          #+#    #+#             */
/*   Updated: 2025/04/15 16:13:31 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <cstdlib> // atoi()
#include <signal.h>
#include <sys/socket.h> // socket(), address
#include <netinet/in.h> // IP specialties 
#include <unistd.h> // fcntl
#include <fcntl.h>
#include <poll.h>
#include <netinet/in.h> // inet_ntoa
#include <arpa/inet.h> // inet_ntoa
#include <cerrno> // recv errno
#include <cstring> // strerror

#include "Client.hpp"
#include "Command.hpp"
#include "Channel.hpp"
#include "Builder.hpp"

# define BUFF_SIZE 512
# define MAX_PORT 65535

class Command;
class Channel;
class Client;

class Server {
	public: 
		Server(int port, std::string pw);
		~Server();

		int					getPort() const;
		const std::string	getPW() const;
		const Client *		getClientByNick(std::string nick) const;

		void	initClient(int fd, struct sockaddr_in ClientAddress);
		void	init();
		void	initChannel(std::string name);
		void	run();
		
		void	SetClientByNick(std::string nick, Client *client);
		void	EraseClientByNick(std::string nick);
		bool	FindClientByNick(std::string nick);
		void	SetCmdMap();

		std::string	recieveData(int fd, std::string msg);
		bool	messageIsFull(Client *cl, std::string *message);
		void	handleData(Client *cl, std::string message);
		void		connectClient();
		void		clearClient(int fd);

		void	sendData(int fd, std::string response) const;
		void	SendToGroup(const std::vector<Client *> clients, const std::string message) const;
		void	SendToAllChannels(const Client *sender, const std::string message);
		void	SendToNick(const Client *sender, const std::string nick, const std::string message) const;
		void	SendToClient(const Client *client, const std::string message) const;

		bool		isChannel(std::string name);
		Channel*	getChannel(std::string name);

	private :
		Server();
		void	initSocket();
		void	initPoll(int fd);

		int			_port;
		std::string _pw;
		int			_socketFD;

		std::map<std::string, void(Command::*)(std::vector<std::string> *arg)>	CmdMap;
	

		std::vector<struct pollfd>			_fds;
		std::vector<Client *>				_Clients;
		std::map<std::string, Client *>		_ClientsByNick;
		std::map<int, Client *>				_ClientsByFD;
		std::map<std::string, Channel *>	_ChannelsByName;
};

bool	isValidPW(std::string arg);
bool	isValidPort(std::string arg);

# define ERROR "Error :"
# define ERR_NB_ARG "Wrong number of arguments, expected 2 : <port> <password>"
# define ERR_PORT_NUM "Port value should be numeric"
# define ERR_PORT_VAL "Valid port should be between 1024 and 65535"
# define ERR_PW "Password should not contain spaces, \' or \""

#endif