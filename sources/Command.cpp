/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:34:36 by endoliam          #+#    #+#             */
/*   Updated: 2025/03/28 14:12:43 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/*								functions								*/

Command::Command()
{
	return ;
}
bool	IsCmd(std::string input)
{
	if (input == "KICK" || input == "INVITE" || input == "TOPIC" || input == "MODE"
		|| input == "JOIN" || input == "NICK" || input == "PASS" || input == "USER"
		|| input == "PRIVMSG" || input == "QUIT" || input == "PART")
		return (true);
	return (false);
}
Command::Command(Server *server, Client *client, std::string msg) : _server(server), _client(client)
{
	std::stringstream			ss;
	std::vector<std::string> vectormsg;
	std::vector<std::string> VectorMsg;
	ss << msg;
	for (size_t i = 0; i < msg.size(); i++)
	{
		std::string ToPushed;
		ss >> ToPushed;
		if (ToPushed[0] == ':')
		{
			std::string _ToPushed = ToPushed;
			while (i < msg.size() || ToPushed[ToPushed.size() - 1] == '\n')
			{
				ss >> ToPushed;
				_ToPushed += " " + ToPushed;
				i += ToPushed.size();
			}
			ToPushed.clear();
			ToPushed = _ToPushed;
		}
		if (!ToPushed.empty())
			VectorMsg.push_back(ToPushed);
		if (ToPushed.find('\n', 0) != std::string::npos)
			ToPushed.erase(ToPushed.find('\n', 0), ToPushed.find('\n', 0) + 1);
		i += ToPushed.size();
		ToPushed.clear();
	}
	for (std::vector<std::string>::iterator it = VectorMsg.begin(); it != VectorMsg.end(); it++)
	{
		if (it != VectorMsg.end() && IsCmd(*it))
		{
			this->_server->SendToClient(this->_client, "--------------------CMD--------------------\n");
			std::vector<std::string> _M;
			_M.push_back(*it);
			this->_server->SendToClient(this->_client, "_M = " + *it + "\n");
			it++;
			while (it != VectorMsg.end() && !IsCmd(*it))
			{
				_M.push_back(*it);
				this->_server->SendToClient(this->_client, "_M = " + *it + "\n");
				it++;
			}
			this->input.push_back(_M);
			this->_server->SendToClient(this->_client, "--------------------CMD--------------------\n");
			it--;
		}
	}
	return ;
}

Command::Command(Command &rhs)
{
	// if (*this != rhs)
	*this = rhs;
	return ;
}

Command::~Command()
{
	return ;
}
Command	&Command::operator=(Command &rhs)
{
	(void)rhs; // added because op calls himself if *this = rhs
	return (*this);
}

void PrintArg(std::vector<std::string> arg)
{
	for (std::vector<std::string>::iterator it = arg.begin(); it != arg.end(); it++)
		std::cout << "arg function = " << *it << std::endl;
	return ;
}

/*								members functions								*/

void	Command::Kick(std::vector<std::string> *arg)
{
	std::cout << "kick function called " << std::endl;
	PrintArg(*arg);
	if (!parsingCmd(this->_client, this->_server, *arg, "KICK"))
		return ;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	Channel *Channel = this->_client->getChannel(*it);
	it++;
	if (!ThereIsArg(this->_client, this->_server, it, *arg, "TOPIC") || !IsClientOnChannel(this->_client, this->_server, Channel, *it))
		return ;
	const Client *TargetUser = this->_server->getClientByNick(*it);
	Channel->leaveChannel((Client *)TargetUser);
	this->_client->removeChannel(Channel);
	it++;
	if (it != arg->end())
	{
		this->_server->SendToClient(TargetUser, Builder::RplKicked(this->_client->getNick(), Channel->getName(), &(*it)) + "\n");
		Channel->SendToAll(Builder::RplKick(this->_client->getNick(), TargetUser->getNick(), Channel->getName(), &(*it)) + "\n");
	}
	else
	{
		this->_server->SendToClient(TargetUser, Builder::RplKicked(this->_client->getNick(), Channel->getName(), NULL) + "\n");
		Channel->SendToAll(Builder::RplKick(this->_client->getNick(), TargetUser->getNick(), Channel->getName(), NULL) + "\n");
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
	if (!ThereIsArg(this->_client, this->_server, it, *arg, "TOPIC") || !IsOnServer(this->_client, this->_server, *it))
		return ;
	const Client *TargetUser = this->_server->getClientByNick(*it);
	it++;
	if (!ThereIsArg(this->_client, this->_server, it, *arg, "TOPIC") || !CheckMaskChan(this->_client, this->_server, &(*it)))
		return ;
	Channel *Channel = this->_client->getChannel(*it);
	if (!Channel)
	{
		this->_server->initChannel(*it);
		Channel = this->_client->getChannel(*it);
		Channel->addOP(this->_client);
		Channel->joinChannel(this->_server, this->_client, NULL);
	}
	if (CheckIsOp(this->_client, this->_server, Channel))
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
	Channel *Channel = this->_client->getChannel(*it);
	it++;
	if (!ThereIsArg(this->_client, this->_server, it, *arg, "TOPIC"))
		return ;
	if (it == arg->end() && Channel->getTopic().empty())
		this->_server->SendToClient(this->_client, Builder::RplNoTopic(Channel->getName()) + "\n"); 
	if (it != arg->end())
	{
		if (it->find(":", 0) == 0)
			it->replace(0, 1, "");
		Channel->setTopic(*it);
	}
	if (!Channel->getTopic().empty())
		this->_server->SendToClient(this->_client, Builder::RplTopic(Channel->getName(), Channel->getTopic()) + "\n");		
}

void	Command::Mode(std::vector<std::string> *arg)
{
	std::cout << "Mode function called " << std::endl;
	PrintArg(*arg);
	if (!parsingCmd(this->_client, this->_server, *arg, "MODE"))
		return ;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	Channel *Channel = this->_client->getChannel(*it);
	it++;
	if (!ThereIsArg(this->_client, this->_server, it, *arg, "MODE"))
		return ;
	char Flag = (*it)[0];
	if (!isValidFlag(this->_client, this->_server, Flag))
		return ;
	std::map<char, std::string *> Mods = SetMapMods(*it, arg, Flag);
	for (std::map<char, std::string *>::iterator it = Mods.begin(); it != Mods.end(); it++)
	{
		if (Flag == '+')
			addmod(this->_client, this->_server, Channel, it);
		else
			removemod(this->_client, this->_server, Channel, it);
	}
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
	// if (!IsPassGiven(this->_client, this->_server))
	// 	return ;
	if (arg->size() != 1)
	{
		std::vector<std::string>::iterator it = arg->begin();
		it++;
		if (!CheckNickInUse(this->_client, this->_server, *it))
			return ;
		if (this->_client->getNick().empty() && this->_client->isRegistered())
		{
			this->_server->SendToClient(this->_client, "----------------you've been successfully registered----------------\n");
			this->_server->SendToClient(this->_client, Builder::Welcome(*it, this->_client->getUser()) + "\n");
		}
		else
		{
			this->_server->SendToClient(this->_client, Builder::Nick(this->_client->getNick(), this->_client->getUser(), *it) + "\n");
			if (!this->_client->getNick().empty())
				this->_server->EraseClientByNick(this->_client->getNick());
		}
		this->_client->setNick(*it);
		this->_server->SetClientByNick(*it, this->_client);
	}
	else
		this->_server->SendToClient(this->_client, Builder::ErrNoNickGiven(this->_client->getNick()) + "\n");
	//ERR_ERRONEUSNICKNAME
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
		this->_server->SendToClient(this->_client, Builder::ErrPasswdMisMatch() + "\n");
}

void	Command::user(std::vector<std::string> *arg)
{
	std::cout << "user function called " << std::endl;
	PrintArg(*arg);
	// if (!IsAlreadyRegistered(this->_client, this->_server) || !IsPassGiven(this->_client, this->_server))
	// 	return ;
	if (!IsAlreadyRegistered(this->_client, this->_server))
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
		this->_client->setFullName(*i);
		this->_client->registerUser();
		if(!this->_client->getNick().empty())
		{
			this->_server->SendToClient(this->_client, "-----------you've been successfully registered-----------\n");
			this->_server->SendToClient(this->_client, Builder::Welcome(this->_client->getNick(), this->_client->getUser()) + "\n");
		}
	}
	else
		this->_server->SendToClient(this->_client, Builder::ErrNeedMoreParams(this->_client->getNick(), "USER") + "\n");	
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
		if (IsOnServer(this->_client, this->_server, *it))
		{
			const Client *TargetUser = this->_server->getClientByNick(*it);
			this->_server->SendToClient(TargetUser, Builder::RplPrivMsg(this->_client->getNick(), *msg)+ "\n");
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
	this->_server->SendToAllChannels(this->_client, Builder::RplQuit(this->_client->getNick(), this->_client->getUser(), *it));
	this->_server->clearClient(this->_client->getFD());
}
void	Command::part(std::vector<std::string> *arg)
{
	std::cout << "part function called " << std::endl;
	PrintArg(*arg);
	if (!CheckArgAndRegister(this->_client, this->_server, *arg, "JOIN"))
		return ;
	std::cout << "part function called " << std::endl;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	while (it != arg->end())
	{
		if (CheckMaskChan(this->_client, this->_server, &(*it)) && CheckChanOnServer(this->_client, this->_server, *it))
		{
			Channel *ChanToQuit = this->_server->getChannel(*it);
			if (IsClientOnChannel(this->_client, this->_server, ChanToQuit, this->_client->getNick()))
			{
				this->_server->SendToClient(this->_client, Builder::RplLeave(this->_client->getNick(), ChanToQuit->getName()) + "\n");
				ChanToQuit->leaveChannel(this->_client);
				this->_client->removeChannel(ChanToQuit);
				ChanToQuit->SendToAll(Builder::RplLeaveChan(this->_client->getNick(), ChanToQuit->getName()) + "\n");
			}
		}
		it++;
	}
}
