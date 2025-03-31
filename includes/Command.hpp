/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:25:17 by endoliam          #+#    #+#             */
/*   Updated: 2025/03/31 11:44:33 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <iostream>
#include <vector>
#include <vector>
#include <vector>
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

		/*							private atributes							*/

		Server *_server;
		Client *_client;
		
		/*							private functions							*/
		void	SetInputCmd(std::vector<std::string> VectorMsg);

	public:
		/*							members functions called by the servers							*/
		void	Kick(std::vector<std::string> *arg);
		void	Invite(std::vector<std::string> *arg);
		void	Topic(std::vector<std::string> *arg);
		void	Mode(std::vector<std::string> *arg);
		void	join(std::vector<std::string> *arg);
		void	nick(std::vector<std::string> *arg);
		void	pass(std::vector<std::string> *arg);
		void	user(std::vector<std::string> *arg);
		void	privmsg(std::vector<std::string> *arg);
		void	quit(std::vector<std::string> *arg);
		void	part(std::vector<std::string> *arg);
		void	Who(std::vector<std::string> *arg);
	
		Command(Server *server, Client *client, std::string msg);
		~Command();
		Command	&operator=(Command &rhs);
		
		/*							public atributes							*/
		std::vector<std::vector<std::string> >		input;
};

/*							Parsing Command							*/
bool							CheckNickInUse(Client *client, Server *server, std::string GivenNick);
bool							parsingCmd(Client *client, Server *server, std::vector<std::string> arg, std::string cmdName);
bool							ThereIsArg(Client *client, Server *server, std::vector<std::string>::iterator it, std::vector<std::string> &arg, std::string cmdName);
bool							IsAlreadyRegistered(Client *client, Server *server);
bool							IsPassGiven(Client *client, Server *server);
bool							IsClientOnChannel(Client *client, Server *server, Channel *Channel, std::string TargetClient);
bool							CheckChannelArg(Client *client, Server *server, std::string ChannelName);
bool							CheckArgAndRegister(Client *client, Server *server, std::vector<std::string> arg, std::string cmdName);
bool							CheckMaskChan(Client *client, Server *server,std::string *ChannelName);
bool							isValidFlag(Client *client, Server *server, char Flag);
bool							IsOnServer(Client *client, Server *server, std::string TargetClient);
bool							CheckIsOp(Client *client, Server *server, Channel *channel);
bool							CheckChanOnServer(Client *client, Server *server, std::string ChannelName);

/*							Command Utils							*/

void							addmod(Client *client, Server *server, Channel *Channel, std::map<char, std::string *>::iterator it);
void							removemod(Client *client, Server *server, Channel *Channel, std::map<char, std::string *>::iterator it);
std::map<char, std::string *>	SetMapMods(std::string mod, std::vector<std::string> *arg, char Flag);

void							setMapJoin(std::map<std::string, std::string> *JoinnedChan, std::vector<std::string> *arg);

void							SetVectorMsg(std::vector<std::string> *VectorMsg, std::string msg);
std::string						JoinMsg(std::string ToPushed,std::stringstream *ss);
bool							IsCmd(std::string input);

#endif