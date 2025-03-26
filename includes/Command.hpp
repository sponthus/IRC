/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:25:17 by endoliam          #+#    #+#             */
/*   Updated: 2025/03/26 15:24:34 by endoliam         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <iostream>
#include <list>
#include <vector>
#include <list>
#include <sstream>
#include <algorithm>

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Builder.hpp"

class Client;
class Server;
class Channel;

class Command
{
	private:
		/*															*/
		Command();
		Command(Command &rhs);

		/*			private members functions						*/

		Server *_server;
		Client *_client; // Client who sent cmd, necessary to exec

	public:
		/*			members functions called by the server			*/
		void	Kick(std::list<std::string> *arg);
		void	Invite(std::list<std::string> *arg);
		void	Topic(std::list<std::string> *arg);
		void	Mode(std::list<std::string> *arg);
		void	join(std::list<std::string> *arg);
		void	nick(std::list<std::string> *arg);
		void	pass(std::list<std::string> *arg);
		void	user(std::list<std::string> *arg);
		void	privmsg(std::list<std::string> *arg);
		void	quit(std::list<std::string> *arg);
		void	part(std::list<std::string> *arg);
	
		Command(Server *server, Client *client, std::string msg);
		~Command();
		Command	&operator=(Command &rhs);
		std::vector<std::list<std::string> >		input;

};

/*					Parsing Command							*/
bool	parsingCmd(Client *client, Server *server, std::list<std::string> arg, std::string cmdName);
bool	ThereIsArg(Client *client, Server *server, std::list<std::string>::iterator it, std::list<std::string> &arg, std::string cmdName);
bool	IsClientInChannel(Client *client, Server *server, Channel *Channel, std::string TargetClient);
bool	CheckChannelArg(Client *client, Server *server, std::string ChannelName);
bool	CheckArgAndRegister(Client *client, Server *server, std::list<std::string> arg, std::string cmdName);
bool	CheckMaskChan(Client *client, Server *server,std::string ChannelName);
bool	isValidFlag(Client *client, Server *server, char Flag);


/*					Commande Utils							*/
void							addmod(Client *client, Server *server, Channel *Channel, std::map<char, std::string *>::iterator it);
void							removemod(Client *client, Server *server, Channel *Channel, std::map<char, std::string *>::iterator it);
std::map<char, std::string *>	SetMapMods(std::string mod, std::list<std::string> *arg, char Flag);

void							setMapJoin(std::map<std::string, std::string> *JoinnedChan, std::list<std::string> *arg);
#endif