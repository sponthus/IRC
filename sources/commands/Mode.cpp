/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:34:36 by endoliam          #+#    #+#             */
/*   Updated: 2025/05/13 16:09:56 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::Mode(std::vector<std::string> *arg)
{
	if (!parsingCmd(this->_client, this->_server, *arg, "MODE"))
		return ;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	it->erase(0, 1);
	Channel *Channel = this->_client->getChannel(*it);
	std::string ChannelName = *it;
	it++;
	if (it == arg->end())
	{
		this->_server->SendToClient(this->_client, Builder::RplChannelModeIs(Channel, this->_client->getNick()));
		return ;
	}
	else if (!CheckIsOp(this->_client, this->_server, Channel))
		return ;	
	char Flag = (*it)[0];
	if (!isValidFlag(this->_client, this->_server, Flag))
		return ;
	std::map<char, std::string *> Mods = SetMapMods(*it, arg, Flag);
	if (!Channel)
		this->_server->SendToClient(this->_client, Builder::ErrNotOnChannel(this->_client->getNick(), ChannelName));
	SetModeInChan(this->_client, this->_server, Channel, Mods, Flag);
}

/*								MODE UTILS								*/

// Creates a map : std::map<char, std::string *>
std::map<char, std::string *>	SetMapMods(std::string mod, std::vector<std::string> *arg, char Flag)
{
	std::map<char, std::string *> Mods;
	std::vector<std::string>::iterator it = arg->begin();
	for (size_t i = 0; i < 3; i++)
		it++;
	for (size_t i = 1; i < mod.size(); i++)
	{
		if	(it != arg->end() && mod[i])
		{
			if ((Flag == '+' && mod[i] == 'l' )|| (mod[i] == 'k') || mod[i] == 'o')
			{
				Mods[mod[i]] = &(*it);
				it++;
			}
			else
				Mods[mod[i]] = NULL;
		}
		else if (mod[i])
			Mods[mod[i]] = NULL;
	}
	return (Mods);
}

bool	isModWhitOption(char c)
{
	std::string mods = "okl";
	if (mods.find(c) != std::string::npos)
		return (true);
	return (false);
}

void	SetModeInChan(Client *client, Server *server, Channel *Channel, std::map<char, std::string *> Mods, char Flag)
{
	std::string recapModes(1, Flag);
	std::string recapOptions;
	for (std::map<char, std::string *>::iterator it = Mods.begin(); it != Mods.end(); it++)
	{
		if (Flag == '+')
		{
			if (addmod(client, server, Channel, it))
			{
				recapModes += it->first;
				if (isModWhitOption(it->first))
					SetRecapOptions(*it->second, &recapOptions, it->first);
			}
		}
		else
		{
			if (removemod(client, server, Channel, it))
			{
				recapModes += it->first;
				if (it->first == 'o' || it->first == 'k') // Only case of options with -
					SetRecapOptions(*it->second, &recapOptions, it->first);
			}
		}
	}
	if (recapModes.size() != 1)
		Channel->SendToAll(Builder::Mode(client->getNick(), Channel->getName(), recapModes, recapOptions));
}

bool	removemod(Client *client, Server *server, Channel *Channel, std::map<char, std::string *>::iterator it)
{
	if (it->first == 'i')
		Channel->setInviteOnly(client, '-');
	else if (it->first == 't')
		Channel->setTopicRestriction(client, '-');
	else if (it->first == 'k')
	{
		if (!it->second)
		{
			server->SendToClient(client, Builder::ErrNeedMoreParams(client->getNick(), "MODE -k"));
			return (false);
		}
		if (*it->second != Channel->getPW())
		{
			server->SendToClient(client, Builder::ErrPasswdMismatch(client->getNick()));
			return (false);
		}
		Channel->deletePW(client);
	}
	else if (it->first == 'o')
	{
		if (it->second)
		{
			const Client *TargetUser = NULL;
			if (!IsClientOnChannel(client, server, Channel->getName(), *it->second))
				return (false);
			TargetUser = server->getClientByNick(*it->second);
			return (Channel->removeOP((Client *)TargetUser));
		}
		else
		{
			server->SendToClient(client, Builder::ErrNeedMoreParams(client->getNick(), std::string("MODE -o").append(&it->first)));
			return (false);
		}
	}
	else if (it->first == 'l')
		Channel->setUserLimit(0, '-');
	else
	{
		server->SendToClient(client, Builder::ErrUnknownMode(client->getNick(), "" + it->first, Channel->getName()));
		// server->SendToClient(client, Builder::ErrUModeUnknownMod(it->first));
		return (false);
	}
	return (true);
}

bool	addmod(Client *client, Server *server, Channel *Channel, std::map<char, std::string *>::iterator it)
{
	if (it->first == 'i')
		Channel->setInviteOnly(client, '+');
	else if (it->first == 't')
		Channel->setTopicRestriction(client, '+');
	else if (it->first == 'k')
	{
		if (it->second)
			Channel->setPW(client, *it->second);
	}
	else if (it->first == 'o')
	{
		if (it->second)
		{
			const Client *TargetUser = NULL;
			if (!IsClientOnChannel(client, server, Channel->getName(), *it->second))
				return (false);
			TargetUser = server->getClientByNick(*it->second);
			return (Channel->addOP((Client *)TargetUser));
		}
	}
	else if (it->first == 'l')
	{
		if (it->second)
		{
			int limit = 0;
			std::stringstream ss;
			ss << *it->second;
			ss >> limit;
			Channel->setUserLimit(limit, '+');
		}	
	}
	else
	{
		// server->SendToClient(client, Builder::ErrUModeUnknownMod(it->first));
		server->SendToClient(client, Builder::ErrUnknownMode(client->getNick(), "" + it->first, Channel->getName()));
		return (false);
	}
	if (isModWhitOption(it->first) && !it->second)
	{
		server->SendToClient(client, Builder::ErrNeedMoreParams(client->getNick(), std::string("MODE +") + (it->first)));
		return (false);
	}
	return (true);
}

void	SetRecapOptions(std::string arg, std::string *recapOptions, char c)
{
	if (c == 'k')
	{
		for	(size_t i = 0; i < arg.size(); i++)
			*recapOptions += "*";
	}
	else
		*recapOptions += arg;
	*recapOptions += " ";
}

bool	isValidFlag(Client *client, Server *server, char Flag)
{
	if (Flag == '+' || Flag == '-')
		return (true);
	server->SendToClient(client, Builder::ErrUModeUnknownFlag(client->getNick()));
	return (false);
}
