/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:25:17 by endoliam          #+#    #+#             */
/*   Updated: 2025/05/13 16:20:19 by sponthus         ###   ########.fr       */
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
		Command();

		/*							private atributes							*/

		Server *_server;
		Client *_client;
		
		/*							private functions							*/
		void	SetInputCmd(std::vector<std::string> VectorMsg);
		
		/* 		Util JOIN 		*/
		void	LeaveAllChannels();

	public:
		/*							members functions called by the server		*/

		// Authentication
		void	pass(std::vector<std::string> *arg);
		void	user(std::vector<std::string> *arg);
		void	nick(std::vector<std::string> *arg);

		void	Topic(std::vector<std::string> *arg);
		void	Mode(std::vector<std::string> *arg);
		void	join(std::vector<std::string> *arg);
		void	privmsg(std::vector<std::string> *arg);
		void	quit(std::vector<std::string> *arg);
		void	part(std::vector<std::string> *arg);
		void	Who(std::vector<std::string> *arg);
		void	Kick(std::vector<std::string> *arg);
		void	Invite(std::vector<std::string> *arg);

	
		Command(Server *server, Client *client, std::string msg);
		~Command();
		
		/*							public atributes							*/
		std::vector<std::vector<std::string> >		input;
};

/*									Authentication utils						*/

bool	IsPassGiven(Client *client, Server *server);
bool	IsAlreadyRegistered(Client *client, Server *server);

/*									User utils									*/

void	SetVoidUser(Client *client, Server *server);

/*									Nick utils									*/

bool	CheckNickInUse(Client *client, Server *server, std::string GivenNick);
bool	CheckErroneusNickName(Client *client, Server *server, std::string GivenNick);

/*									Join utils									*/

std::vector<std::string>::iterator	FindLastChannel(std::vector<std::string>* arg);
void								setMapJoin(std::map<std::string, std::string> *JoinnedChan, std::vector<std::string> *arg);

/*									Mode utils									*/

std::map<char, std::string *>	SetMapMods(std::string mod, std::vector<std::string> *arg, char Flag);
bool							isModWhitOption(char c);
void							SetModeInChan(Client *client, Server *server, Channel *Channel, std::map<char, std::string *> Mods, char Flag);
bool							removemod(Client *client, Server *server, Channel *Channel, std::map<char, std::string *>::iterator it);
bool							addmod(Client *client, Server *server, Channel *Channel, std::map<char, std::string *>::iterator it);
void							SetRecapOptions(std::string arg, std::string *recapOptions, char c);
bool							isValidFlag(Client *client, Server *server, char Flag);

/*									Constructor utils							*/

bool		IsCmd(std::string input);
std::string	JoinMsg(std::string ToPushed,std::stringstream *ss);
void		SetVectorMsg(std::vector<std::string> *VectorMsg, std::string msg);

/*									Common utils/checks							*/

bool	ThereIsArg(Client *client, Server *server, std::vector<std::string>::iterator it, std::vector<std::string> &arg, std::string cmdName);
bool	IsOnServer(Client *client, Server *server, std::string TargetClient);
bool	IsClientOnChannel(Client *client, Server *server, std::string ChannelName, std::string TargetClient);
bool	CheckMaskChan(Client *client, Server *server,std::string *ChannelName);
bool	CheckIsOp(Client *client, Server *server, Channel *channel);
bool	CheckChanOnServer(Client *client, Server *server, std::string ChannelName);
bool	CheckChannelArg(Client *client, Server *server, std::string ChannelName);
bool	CheckArgAndRegister(Client *client, Server *server, std::vector<std::string> arg, std::string cmdName);
bool	parsingCmd(Client *client, Server *server, std::vector<std::string> arg, std::string cmdName);

#endif