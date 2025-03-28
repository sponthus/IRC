/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:28:35 by endoliam          #+#    #+#             */
/*   Updated: 2025/03/28 15:17:37 by endoliam         ###   ########lyon.fr   */
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
			server->SendToClient(client, Builder::ErrNeedMoreParams(client->getNick(), "MODE " + it->first) + "\n");
	}
	else if (it->first == 'l')
		Channel->setUserLimit(client, 0, '-');
	else
		server->SendToClient(client, Builder::ErrUModeUnknownMod(it->first) + "\n");
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
		server->SendToClient(client, Builder::ErrUModeUnknownMod(it->first) + "\n");
	if ((it->first == 'o' && !it->second) || (it->first == 'k' && !it->second) || (it->first == 'k' && !it->second))
		server->SendToClient(client, Builder::ErrNeedMoreParams(client->getNick(), "MODE " + it->first) + "\n");
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

/*								CONSTRUCTOR UTILS								*/

bool	IsCmd(std::string input)
{
	if (input == "KICK" || input == "INVITE" || input == "TOPIC" || input == "MODE"
		|| input == "JOIN" || input == "NICK" || input == "PASS" || input == "USER"
		|| input == "PRIVMSG" || input == "QUIT" || input == "PART")
		return (true);
	return (false);
}

std::string	JoinMsg(std::string ToPushed,std::stringstream *ss)
{
	ToPushed.erase(0, 1);
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