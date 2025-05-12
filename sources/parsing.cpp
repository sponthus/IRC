/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 10:42:14 by sponthus          #+#    #+#             */
/*   Updated: 2025/05/12 17:58:14 by endoliam         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Colors.hpp"

// No space, no ' or "
bool	isValidPW(std::string arg)
{
	std::string::const_iterator it = arg.begin();
	
	if (arg.size() < 1)
		return false;
	while (it != arg.end())
	{
		if (*it == ' ' || *it == 34 || *it == 39)
			return false;
		it++;
	}
	return true;
}

// Positive port between 1024 and 65535
// To avoid port already used and not usable
bool	isValidPort(std::string arg)
{
	int	port;
	std::string::const_iterator it = arg.begin();

	while (it != arg.end())
	{
		if (std::isdigit((unsigned char)(*it)) == 0)
		{
			std::cout << RED << ERROR << ERR_PORT_NUM << RESET << std::endl;
			return false;
		}
		it++;
	}
	port = std::atoi(arg.c_str());
	if (port < 1024 || port > MAX_PORT)
	{
		std::cout << RED << ERROR << ERR_PORT_VAL << RESET << std::endl;
		return false;
	}
	return true;
}

bool	CheckNickInUse(Client *client, Server *server, std::string GivenNick)
{
	 if (server->FindClientByNick(GivenNick))
	 {
		server->SendToClient(client,  Builder::ErrNickInUse(client->getAddress(), GivenNick));
		return (false);
	 }
	 return (true);
}

bool	ThereIsArg(Client *client, Server *server, std::vector<std::string>::iterator it, std::vector<std::string> &arg, std::string cmdName)
{
	if (it == arg.end())
	{
		server->SendToClient(client, Builder::ErrNeedMoreParams(client->getNick(), cmdName));
		return (false);
	}
	return (true);
}

bool	IsPassGiven(Client *client, Server *server)
{
	if (!client->isPass())
	{
		server->SendToClient(client, Builder::ErrPasswdMismatch(client->getNick()));
		return (false);
	}
	return (true);
}

bool	IsAlreadyRegistered(Client *client, Server *server)
{
	if (client->isRegistered())
	{
		server->SendToClient(client, Builder::ErrAlreadyRegisted(client->getUser()));
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

// bool	IsClientOnChannel(Client *client, Server *server, Channel *channel, std::string TargetClient)
// {
// 	if (!IsOnServer(client, server, TargetClient))
// 		return (false);
// 	const Client *TargetUser = server->getClientByNick(TargetClient);
// 	if (!channel || !channel->isClient((Client *)TargetUser))
// 	{
// 		server->SendToClient(client, Builder::ErrNotOnChannel(TargetClient, ""));
// 		return (false);
// 	}
// 	return (true);
// }

bool	IsClientOnChannel(Client *client, Server *server, std::string ChannelName, std::string TargetClient)
{
	if (!IsOnServer(client, server, TargetClient))
		return (false);
	Channel *channel = client->getChannel(ChannelName);
	const Client *TargetUser = server->getClientByNick(TargetClient);
	if (!channel || !channel->isClient((Client *)TargetUser))
	{
		server->SendToClient(client, Builder::ErrNotOnChannel(TargetClient, ChannelName));
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
			server->SendToClient(client, Builder::BadChannelMask(*ChannelName));
			return (false);
		}
		return (true);
	}
	server->SendToClient(client, Builder::BadChannelMask(*ChannelName));
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

bool	isValidFlag(Client *client, Server *server, char Flag)
{
	if (Flag == '+' || Flag == '-')
		return (true);
	server->SendToClient(client, Builder::ErrUModeUnknownFlag(client->getNick()));
	return (false);
}