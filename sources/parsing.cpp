/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 10:42:14 by sponthus          #+#    #+#             */
/*   Updated: 2025/03/26 14:39:42 by endoliam         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "messages.hpp"
#include "Server.hpp"

#include <cstdlib>
// No space, no ' or "
bool	isValidPW(std::string arg)
{
	std::string::const_iterator it = arg.begin();
	
	while (it != arg.end())
	{
		if (*it == ' ' || *it == 34 || *it == 39)
			return false;
		it++;
	}
	return true;
}

// Positive port between 1024 and 65535
bool	isValidPort(std::string arg)
{
	int	port;
	std::string::const_iterator it = arg.begin();

	while (it != arg.end())
	{
		if (std::isdigit((unsigned char)(*it)) == 0)
		{
			std::cout << ERROR << ERR_PORT_NUM << std::endl;
			return false;
		}
		it++;
	}
	port = std::atoi(arg.c_str());
	if (port < 1024 || port > 65535)
	{
		std::cout << ERROR << ERR_PORT_VAL << std::endl;
		return false;
	}
	return true;
}

bool	ThereIsArg(Client *client, Server *server, std::list<std::string>::iterator it, std::list<std::string> &arg, std::string cmdName)
{
	if (it == arg.end())
	{
		server->SendToClient(client, Builder::ErrNeedMoreParams(client->getNick(), cmdName) + "\n");
		return (false);
	}
	return (true);
}

bool	IsClientInChannel(Client *client, Server *server, Channel *Channel, std::string TargetClient)
{
	const Client *TargetUser = server->getClientByNick(TargetClient);
	if (!TargetUser)
	{
		server->SendToClient(client, Builder::ErrNoSuchNick(client->getNick(), TargetClient) + "\n");
		return (false);
	}
	else if (!Channel->isClient((Client *)TargetUser))
	{
		server->SendToClient(client, Builder::ErrNotOnChannel(TargetUser->getNick(), TargetClient) +  "\n");
		return (false);
	}
	return (true);
}
bool	CheckMaskChan(Client *client, Server *server,std::string ChannelName)
{
	if (ChannelName.find("#", 0) == 0 || ChannelName.find("&", 0) ==0)
		return (true);
	server->SendToClient(client, Builder::BadChannelMask(ChannelName) + "\n");
	return (false);
}
bool	CheckChannelArg(Client *client, Server *server, std::string ChannelName)
{
	if (!CheckMaskChan(client, server, ChannelName))
		return (false);
	if (!server->getChannel(ChannelName))
	{
		server->SendToClient(client, Builder::ErrNoSuchChannel(client->getNick(), ChannelName)  + "\n");
		return (false);
	}
	Channel *channel = client->getChannel(ChannelName);
	if (!IsClientInChannel(client, server, channel, client->getNick()))
		return (false);
	if (!channel->isOP(client))
	{
		server->SendToClient(client, Builder::ErrChanOPrivsNeeded(client->getNick(), channel->getName()) + "\n"); // is not the op
		return (false);
	}
	return (true);
}

bool	CheckArgAndRegister(Client *client, Server *server, std::list<std::string> arg, std::string cmdName)
{
	if (!client->isRegistered() && client->getNick().empty())
	{
		server->SendToClient(client, Builder::ErrNotRegistered() +"\n");
		return (false);
	}
	if	(arg.size() == 1)
	{
		server->SendToClient(client, Builder::ErrNeedMoreParams(client->getNick(), cmdName) + "\n");
		return (false);
	}
	return (true);
}

bool	parsingCmd(Client *client, Server *server, std::list<std::string> arg, std::string cmdName)
{
	if (!CheckArgAndRegister(client, server, arg, cmdName))
		return (false);
	std::list<std::string>::iterator it = arg.begin();
	it++;
	if (!ThereIsArg(client, server, it, arg, cmdName))
		return (false);
	return (CheckChannelArg(client, server, *it));
}
bool	isValidFlag(Client *client, Server *server, char Flag)
{
	if (Flag == '+' || Flag == '-')
		return (true);
	server->SendToClient(client, Builder::ErrUModeUnknownFlag(client->getNick()) + "\n");
	return (false);
}