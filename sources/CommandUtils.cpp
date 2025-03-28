/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:28:35 by endoliam          #+#    #+#             */
/*   Updated: 2025/03/28 15:03:23 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Command.hpp"

/*								MODE UTILS								*/
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
			if ((Flag == '+' && mod[i] == 'l' )|| (Flag == '+' && mod[i] == 'k') || mod[i] == 'o')
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

void	removemod(Client *client, Server *server, Channel *Channel, std::map<char, std::string *>::iterator it)
{
	if (it->first == 'i')
		Channel->setInviteOnly(client, '-');
	else if (it->first == 't')
		Channel->setTopicRestriction(client, '-');
	else if (it->first == 'k')
		Channel->deletePW(client);
	else if (it->first == 'o')
	{
		if (it->second)
		{
			const Client *TargetUser = NULL;
			if (IsClientOnChannel(client, server, Channel, *it->second))
			{
				TargetUser = server->getClientByNick(*it->second);
				Channel->removeOP((Client *)TargetUser);
			}	
		}
		else
			server->SendToClient(client, Builder::ErrNeedMoreParams(client->getNick(), "MODE " + it->first));
	}
	else if (it->first == 'l')
		Channel->setUserLimit(client, 0, '-');
	else
		server->SendToClient(client, Builder::ErrUModeUnknownMod(it->first));
}

void	addmod(Client *client, Server *server, Channel *Channel, std::map<char, std::string *>::iterator it)
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
			if (IsClientOnChannel(client, server, Channel, *it->second))
			{
				TargetUser = server->getClientByNick(*it->second);
				Channel->addOP((Client *)TargetUser);
			}
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
			Channel->setUserLimit(client, limit, '+');
		}	
	}
	else
		server->SendToClient(client, Builder::ErrUModeUnknownMod(it->first));
	if ((it->first == 'o' && !it->second) || (it->first == 'k' && !it->second) || (it->first == 'k' && !it->second))
		server->SendToClient(client, Builder::ErrNeedMoreParams(client->getNick(), "MODE " + it->first));
}

/*								JOIN UTILS								*/

std::vector<std::string>::iterator	FindLastChannel(std::vector<std::string>* arg)
{
	std::vector<std::string>::iterator lastChan = arg->end();
	for (std::vector<std::string>::iterator it = arg->begin(); it != arg->end(); it++)
	{
		if (it->find("#", 0) == 0 || it->find("&", 0) == 0)
			lastChan = it;
	}
	return (lastChan);
}

void	setMapJoin(std::map<std::string, std::string> *JoinnedChan, std::vector<std::string> *arg)
{
	std::vector<std::string>::iterator lastChan = FindLastChannel(arg);
	std::vector<std::string>::iterator it = arg->begin();
	if (lastChan != arg->end())
		lastChan++;
	while (++it != arg->end() || lastChan != it)
	{
		if (lastChan != arg->end())
			(*JoinnedChan)[*it] = *lastChan;
		else
			(*JoinnedChan)[*it] = "";
	}
	return ;
}

/*								KICK UTILS								*/

