/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:34:36 by endoliam          #+#    #+#             */
/*   Updated: 2025/05/13 16:42:31 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/*							functions							*/

Command::Command()
{
	return ;
}

Command::Command(Server *server, Client *client, std::string msg) : _server(server), _client(client)
{
	std::vector<std::string> 	VectorMsg;
	SetVectorMsg(&VectorMsg, msg);
	SetInputCmd(VectorMsg);
	return ;
}

Command::~Command()
{
	return ;
}

/*							setter									*/

void	Command::SetInputCmd(std::vector<std::string> VectorMsg)
{
	for (std::vector<std::string>::iterator it = VectorMsg.begin(); it != VectorMsg.end(); it++)
	{
		if (it != VectorMsg.end() && IsCmd(*it))
		{
			std::vector<std::string> _M;
			_M.push_back(*it);
			it++;
			while (it != VectorMsg.end())
			{
				_M.push_back(*it);
				it++;
			}
			this->input.push_back(_M);
			it--;
		}
	}
}

/*						CONSTRUCTOR UTILS								*/

bool	IsCmd(std::string input)
{
	if (input == "KICK" || input == "INVITE" || input == "TOPIC" || input == "MODE"
		|| input == "JOIN" || input == "NICK" || input == "PASS" || input == "USER"
		|| input == "PRIVMSG" || input == "QUIT" || input == "PART" || input == "WHO")
		return (true);
	return (false);
}

std::string	JoinMsg(std::string ToPushed,std::stringstream *ss)
{
	std::string _ToPushed = ToPushed;
	*ss >> ToPushed;
	while (!ss->eof() && !IsCmd(ToPushed))
	{
		_ToPushed += " " + ToPushed;
		*ss >> ToPushed;
	}
	ToPushed.clear();
	ToPushed = _ToPushed;
	return (ToPushed);
}

void	SetVectorMsg(std::vector<std::string> *VectorMsg, std::string msg)
{
	std::stringstream			ss;
	ss << msg;
	while (!ss.eof())
	{
		std::string ToPushed;
		ss >> ToPushed;
		if (ToPushed[0] == ':' || ToPushed[0] == ';')
			ToPushed = JoinMsg(ToPushed, &ss);
		if (!ToPushed.empty())
			VectorMsg->push_back(ToPushed);
		ToPushed.clear();
	}
}

/*									COMMAND COMMON UTILS						*/


bool	ThereIsArg(Client *client, Server *server, std::vector<std::string>::iterator it, std::vector<std::string> &arg, std::string cmdName)
{
	if (it == arg.end())
	{
		server->SendToClient(client, Builder::ErrNeedMoreParams(client->getNick(), cmdName));
		return (false);
	}
	return (true);
}

bool	IsOnServer(Client *client, Server *server, std::string TargetClient)
{
	if (client->getNick() == TargetClient)
		return (true);
	const Client *TargetUser = server->getClientByNick(TargetClient);
	if (!TargetUser)
	{
		server->SendToClient(client, Builder::ErrNoSuchNick(client->getNick(), TargetClient));
		return (false);
	}
	return (true);
}

bool	IsClientOnChannel(Client *client, Server *server, std::string ChannelName, std::string TargetClient)
{
	if (!IsOnServer(client, server, TargetClient))
		return (false);
	Channel *channel = client->getChannel(ChannelName);
	const Client *TargetUser = server->getClientByNick(TargetClient);
	if (!channel || !channel->isClient((Client *)TargetUser))
	{
		if (TargetClient == client->getNick())
			server->SendToClient(client, Builder::ErrNotOnChannel(TargetClient, ChannelName));
		else
			server->SendToClient(client, Builder::ErrUserNotInChannel(client->getNick(), TargetClient, ChannelName));
		return (false);
	}
	return (true);
}

bool	CheckMaskChan(Client *client, Server *server, std::string *ChannelName)
{
	if (ChannelName->find("#", 0) == 0 || ChannelName->find("&", 0) ==0)
	{
		ChannelName->erase(0, 1);
		if (ChannelName->empty())
		{	
			server->SendToClient(client, Builder::ErrBadChannelMask(*ChannelName));
			return (false);
		}
		return (true);
	}
	server->SendToClient(client, Builder::ErrBadChannelMask(*ChannelName));
	return (false);
}

bool	CheckIsOp(Client *client, Server *server, Channel *channel)
{
	if (!channel->isOP(client))
	{
		server->SendToClient(client, Builder::ErrChanOPrivsNeeded(client->getNick(), channel->getName())); // is not the op
		return (false);
	}
	return (true);
}

bool	CheckChanOnServer(Client *client, Server *server, std::string ChannelName)
{
	if (!server->getChannel(ChannelName))
	{
		server->SendToClient(client, Builder::ErrNoSuchChannel(client->getNick(), ChannelName) );
		return (false);
	}
	return (true);
}

bool	CheckChannelArg(Client *client, Server *server, std::string ChannelName)
{
	if (!CheckMaskChan(client, server, &ChannelName))
		return (false);
	if (!CheckChanOnServer(client, server, ChannelName))
		return (false);
	if (!IsClientOnChannel(client, server, ChannelName, client->getNick()))
		return (false);
	return (true);
}

bool	CheckArgAndRegister(Client *client, Server *server, std::vector<std::string> arg, std::string cmdName)
{
	if (!client->isRegistered() || client->getNick().empty())
	{
		server->SendToClient(client, Builder::ErrNotRegistered());
		return (false);
	}
	if	(arg.size() == 1)
	{
		server->SendToClient(client, Builder::ErrNeedMoreParams(client->getNick(), cmdName));
		return (false);
	}
	return (true);
}

bool	parsingCmd(Client *client, Server *server, std::vector<std::string> arg, std::string cmdName)
{
	if (!CheckArgAndRegister(client, server, arg, cmdName))
		return (false);
	std::vector<std::string>::iterator it = arg.begin();
	it++;
	if (!ThereIsArg(client, server, it, arg, cmdName))
		return (false);
	return (CheckChannelArg(client, server, *it));
}
