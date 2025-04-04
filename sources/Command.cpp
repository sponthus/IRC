/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:34:36 by endoliam          #+#    #+#             */
/*   Updated: 2025/04/04 15:43:01 by sponthus         ###   ########.fr       */
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

Command::Command(Command &rhs)
{
	*this = rhs;
	return ;
}

Command::~Command()
{
	return ;
}

Command	&Command::operator=(Command &rhs)
{
	return (rhs);
}

/*						debug									*/
void PrintArg(std::vector<std::string> arg)
{
	for (std::vector<std::string>::iterator it = arg.begin(); it != arg.end(); it++)
		std::cout << "arg function = " << *it << std::endl;
	return ;
}
/*							setter							*/

void	Command::SetInputCmd(std::vector<std::string> VectorMsg)
{
	for (std::vector<std::string>::iterator it = VectorMsg.begin(); it != VectorMsg.end(); it++)
	{
		if (it != VectorMsg.end() && IsCmd(*it))
		{
			std::vector<std::string> _M;
			_M.push_back(*it);
			it++;
			while (it != VectorMsg.end() && !IsCmd(*it))
			{
				_M.push_back(*it);
				it++;
			}
			this->input.push_back(_M);
			it--;
		}
	}
}

/*							members functions							*/

void	Command::Kick(std::vector<std::string> *arg)
{
	std::cout << "kick function called " << std::endl;
	PrintArg(*arg);
	if (!parsingCmd(this->_client, this->_server, *arg, "KICK"))
		return ;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	it->erase(0, 1);
	Channel *Channel = this->_client->getChannel(*it);
	it++;
	if (!ThereIsArg(this->_client, this->_server, it, *arg, "TOPIC") 
		|| !IsClientOnChannel(this->_client, this->_server, Channel, *it)
		|| !CheckIsOp(this->_client, this->_server, Channel))
		return ;
	const Client *TargetUser = this->_server->getClientByNick(*it);
	Channel->leaveChannel((Client *)TargetUser);
	((Client *)TargetUser)->removeChannel(Channel);
	it++;
	if (it != arg->end())
	{
		this->_server->SendToClient(TargetUser, Builder::Kick(this->_client->getNick(), TargetUser->getNick(), Channel->getName(), &(*it)));
		Channel->SendToAll(Builder::Kick(this->_client->getNick(), TargetUser->getNick(), Channel->getName(), &(*it)));
	}
	else
	{
		this->_server->SendToClient(TargetUser, Builder::Kick(this->_client->getNick(), TargetUser->getNick(), Channel->getName(), NULL));
		Channel->SendToAll(Builder::Kick(this->_client->getNick(), TargetUser->getNick(), Channel->getName(), NULL));
	}
}

void	Command::Invite(std::vector<std::string> *arg)
{
	std::cout << "invite function called " << std::endl;
	PrintArg(*arg);
	if (!CheckArgAndRegister(this->_client, this->_server, *arg, "INVITE"))
		return ;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	if (!ThereIsArg(this->_client, this->_server, it, *arg, "INVITE") || !IsOnServer(this->_client, this->_server, *it))
		return ;
	const Client *TargetUser = this->_server->getClientByNick(*it);
	it++;
	if (!ThereIsArg(this->_client, this->_server, it, *arg, "INVITE") || !CheckMaskChan(this->_client, this->_server, &(*it)))
		return ;
	Channel *Channel = this->_client->getChannel(*it);
	if (!Channel)
	{
		this->_server->initChannel(*it);
		Channel = this->_client->getChannel(*it);
		Channel->addOP(this->_client);
		Channel->joinChannel(this->_server, this->_client, NULL);
	}
	Channel->invite(this->_client, (Client *)TargetUser);
}

void	Command::Topic(std::vector<std::string> *arg)
{
	std::cout << "Topic function called " << std::endl;
	PrintArg(*arg);
	if (!parsingCmd(this->_client, this->_server, *arg, "TOPIC"))
		return ;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	it->erase(0, 1);
	Channel *Channel = this->_client->getChannel(*it);
	it++;
	if (it == arg->end() && Channel->getTopic().empty())
	{
		this->_server->SendToClient(this->_client, Builder::RplNoTopic(this->_client->getNick(), Channel->getName()));
		return ;
	}
	else if (it != arg->end())
	{
		if (it->find(":", 0) == 0)
			it->replace(0, 1, "");
		Channel->setTopic(this->_client, *it); 
	}
	this->_server->SendToClient(this->_client, Builder::RplTopic(this->_client->getNick(), Channel->getName(), Channel->getTopic()));
}

void	Command::Mode(std::vector<std::string> *arg)
{
	std::cout << "Mode function called " << std::endl;
	PrintArg(*arg);
	if (!parsingCmd(this->_client, this->_server, *arg, "MODE"))
		return ;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	it->erase(0, 1);
	Channel *Channel = this->_client->getChannel(*it);
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
	SetModeInChan(this->_client, this->_server, Channel, Mods, Flag);
}

void	Command::join(std::vector<std::string> *arg)
{
	std::cout << "join function called " << std::endl;
	PrintArg(*arg);
	if (!CheckArgAndRegister(this->_client, this->_server, *arg, "JOIN"))
		return ;
	std::map<std::string, std::string> JoinnedChan;
	setMapJoin(&JoinnedChan, arg);
	for (std::map<std::string, std::string>::iterator it = JoinnedChan.begin(); it != JoinnedChan.end(); it++)
	{
		if (CheckMaskChan(this->_client, this->_server, (std::string *)&it->first))
		{
			if (this->_server->getChannel(it->first) == NULL)
			{
				this->_server->initChannel(it->first);
				this->_server->getChannel(it->first)->addOP(this->_client);
			}
			this->_server->getChannel(it->first)->joinChannel(this->_server, this->_client, &(it->second));
		}
	}
}

void	Command::nick(std::vector<std::string> *arg)
{
	std::cout << "nick function called " << std::endl;
	PrintArg(*arg);
	if (!IsPassGiven(this->_client, this->_server))
		return ;
	if (arg->size() != 1)
	{
		std::vector<std::string>::iterator it = arg->begin();
		it++;
		if (!CheckNickInUse(this->_client, this->_server, *it))
			return ;
		if (this->_client->getNick().empty() && this->_client->isRegistered())
			this->_server->SendToClient(this->_client, Builder::RplWelcome(*it, this->_client->getUser()));
		else
		{
			this->_server->SendToClient(this->_client, Builder::Nick(this->_client->getNick(), this->_client->getUser(), *it));
			if (!this->_client->getNick().empty())
				this->_server->EraseClientByNick(this->_client->getNick());
		}
		this->_client->setNick(*it);
		this->_server->SetClientByNick(*it, this->_client);
	}
	else
		this->_server->SendToClient(this->_client, Builder::ErrNoNickGiven(this->_client->getNick()));
}

void	Command::pass(std::vector<std::string> *arg)
{
	std::cout << "pass function called " << std::endl;
	PrintArg(*arg);
	if (!IsAlreadyRegistered(this->_client, this->_server))
		return ;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	if	(!ThereIsArg(this->_client, this->_server, it, *arg, "PASS"))
		return ;
	std::cout << "Server mdp = /" << this->_server->getPW() << "/ client sent /" << *it << "/" << std::endl;
	if	(this->_server->getPW() == *it)
		this->_client->PassUSer();
	else
	{
		this->_server->SendToClient(this->_client, Builder::ErrPasswdMismatch(this->_client->getNick()));
		this->_server->clearClient(this->_client->getFD()); // Disconnects the client 
	}
}

void	Command::user(std::vector<std::string> *arg)
{
	std::cout << "user function called " << std::endl;
	PrintArg(*arg);
	if (!IsAlreadyRegistered(this->_client, this->_server))
		return ;
	if (!IsPassGiven(this->_client, this->_server))
		return ;
	if (arg->size() >= 5)
	{
		std::vector<std::string>::iterator i = arg->begin();
		i++;
		this->_client->setUser(*i);
		i++;
		this->_client->setHostname(*i);
		i++;
		this->_client->setServerName(*i);
		i++;
		if (i->find(":", 0) == 0)
			i->erase(0, 1);
		this->_client->setFullName(*i);
		this->_client->registerUser();
		if(!this->_client->getNick().empty())
			this->_server->SendToClient(this->_client, Builder::RplWelcome(this->_client->getNick(), this->_client->getUser()));
	}
	else
		this->_server->SendToClient(this->_client, Builder::ErrNeedMoreParams(this->_client->getNick(), "USER"));	
}

void	Command::privmsg(std::vector<std::string> *arg)
{
	std::cout << "privmsg function called " << std::endl;
	PrintArg(*arg);
	if (!CheckArgAndRegister(this->_client, this->_server, *arg, "JOIN"))
		return ;
	std::vector<std::string>::iterator msg = arg->begin();
	for (size_t i = 0; i < arg->size() - 1; i++)
		msg++;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	while (it != msg)
	{
		if (((*it)[0] == '#' || (*it)[0] == '&') && CheckChannelArg(this->_client, this->_server, *it))
		{
			it->erase(0,1);
			Channel *Channel = this->_server->getChannel(*it);
			Channel->SendToAllBut(this->_client, Builder::PrivMsg(this->_client, *msg, &(Channel->getName()), NULL));
		}
		else if (IsOnServer(this->_client, this->_server, *it))
		{
			const Client *TargetUser = this->_server->getClientByNick(*it);
			this->_server->SendToClient(TargetUser, Builder::PrivMsg(this->_client, *msg, NULL, TargetUser));
		}
		it++;
	}
}

void	Command::quit(std::vector<std::string> *arg)
{
	std::cout << "quit function called " << std::endl;
	PrintArg(*arg);
	if (!CheckArgAndRegister(this->_client, this->_server, *arg, "JOIN"))
		return ;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	this->_server->SendToAllChannels(this->_client, Builder::Quit(this->_client->getNick(), this->_client->getUser(), *it));
	this->_server->clearClient(this->_client->getFD());
}

void	Command::part(std::vector<std::string> *arg)
{
	std::cout << "part function called " << std::endl;
	PrintArg(*arg);
	if (!CheckArgAndRegister(this->_client, this->_server, *arg, "JOIN"))
		return ;
	std::vector<std::string>::iterator msg = arg->begin();
	for (size_t i = 0; i < arg->size() - 1; i++)
		msg++;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	while (it != msg)
	{
		if (!CheckChannelArg(this->_client, this->_server, *it))
			return ;
		it->erase(0,1);
		Channel *ChanToQuit = this->_server->getChannel(*it);
		this->_server->SendToClient(this->_client, Builder::Part(this->_client->getNick(), this->_client->getUser(), ChanToQuit->getName(), &(*msg)));
		ChanToQuit->leaveChannel(this->_client);
		this->_client->removeChannel(ChanToQuit);
		ChanToQuit->SendToAll(Builder::Part(this->_client->getNick(), this->_client->getUser(), ChanToQuit->getName(), &(*msg)));
		it++;
	}
}

void	Command::Who(std::vector<std::string> *arg)
{
	std::cout << "WHO function called " << std::endl;
	PrintArg(*arg);
	if (!parsingCmd(this->_client, this->_server, *arg, "WHO"))
		return ;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	while (it != arg->end())
	{
		if (CheckMaskChan(this->_client, this->_server, &(*it)))
		{
			Channel *ChanToWho = this->_server->getChannel(*it);
			if (IsClientOnChannel(this->_client, this->_server, ChanToWho, this->_client->getNick()))
			{
				for (std::vector<Client *>::const_iterator i = ChanToWho->getClients().begin(); i != ChanToWho->getClients().end(); i++)
					this->_server->SendToClient(this->_client, Builder::RplWhoReply(ChanToWho, this->_client, (*i))); // 352 RPL_WHOREPLY
			}
			this->_server->SendToClient(this->_client, Builder::RplEndOfWho(this->_client->getNick(), ChanToWho->getName()));
		}
		it++;
	}
}
