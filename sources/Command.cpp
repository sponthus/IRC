/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:34:36 by endoliam          #+#    #+#             */
/*   Updated: 2025/03/28 14:08:53 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/*								functions								*/

Command::Command()
{
	return ;
}

size_t	FindPosCmd(std::string msg, std::string tf, size_t pos)
{
	return (msg.find(tf, pos));
}
void		FindMindCmd(size_t &min, size_t pos ,std::string msg)
{
	if (min < pos && pos != msg.npos)
		min = pos;
	return ;
}

int		FindWichNext(std::string msg, size_t &pos)
{
	std::map<int, size_t> cmd;
	cmd[1] = msg.find("KICK", pos);
	cmd[2] = msg.find("INVITE", pos);
	cmd[3] = msg.find("TOPIC", pos);
	cmd[4] = msg.find("MODE", pos);
	cmd[5] = msg.find("JOIN", pos);
	cmd[6] = msg.find("NICK", pos);
	cmd[7] = msg.find("PASS", pos);
	cmd[8] = msg.find("USER", pos);
	cmd[9] = msg.find("PRIVMSG", pos);
	cmd[10] = msg.find("QUIT", pos);
	cmd[11] = msg.find("PART", pos);

	size_t second_min = std::string::npos;
	int		first_min = 0;
	for (std::map<int, size_t>::iterator it = cmd.begin(); it != cmd.end(); it++)
	{
		if (it->second != std::string::npos && second_min && second_min > it->second)
		{
			first_min = it->first;
			second_min = it->second;
		}
		if (it->second != std::string::npos && !second_min)
		{
			first_min = it->first;
			second_min = it->second;
		}
	}
	if (second_min)
		pos = second_min;
	if (first_min)
		return (first_min);
	else
		return (-1);
}

std::list<std::string>		SetListCMd(std::string cmd, size_t &pos, std::string msg)	
{
	std::list<std::string>	lst;

	lst.push_back(cmd);
	size_t	posendl	= msg.find("\n", pos);
	while (msg[pos] && pos + 1 <= posendl)
	{
		while(msg[pos] == ' ')
			pos++;
		if (msg[pos])
		{
			size_t	posendwrd = msg.find(" ", pos);
			if (posendwrd == std::string::npos)
				posendwrd = posendl;
			if (msg[pos] == ':' || msg[pos] == ';')
			{
				posendwrd = posendl;
				if (msg.find(";", pos + 1) < posendl && msg.find(";", pos + 1) != std::string::npos)
				{
					// std::cout << "add ; endl " << std::endl;
					posendwrd = msg.find(";", pos + 1);
				}
				if (msg.find(":", pos + 1) < posendwrd && msg.find(":", pos + 1) != std::string::npos)
				{
					// std::cout << "add : endl " << std::endl;
					posendwrd = msg.find(":", pos + 1);
				}
			}
			if (pos != posendwrd)
			{
				std::string arg(msg, pos, (posendwrd - pos));
				lst.push_back(arg);
				pos += arg.size();
			}
			// std::cout << "size arg = " << arg.size() <<std::endl;
		}
	}
	return (lst);
}

Command::Command(Server *server, Client *client, std::string msg) : _server(server), _client(client)
{
	std::list<size_t>	cmdpos;
	size_t			 	pos = 0;
	
	while (pos <= msg.npos && pos <= msg.size())
	{
		int s = FindWichNext(msg, pos);
		switch(s)
		{
			case -1:
				return ;
			case 1 :
				pos += 4;
				this->input.push_back(SetListCMd("KICK", pos, msg));
				break ;
			case 2 :
				pos += 6;
				this->input.push_back(SetListCMd("INVITE", pos, msg));
				break ;
			case 3 :
				pos += 5;
				this->input.push_back(SetListCMd("TOPIC", pos, msg));
				break ;
			case 4 :
				pos += 4;
				this->input.push_back(SetListCMd("MODE", pos, msg));
				break ;
			case 5 :
				pos += 4;
				this->input.push_back(SetListCMd("JOIN", pos, msg));
				break ;
			case 6 :
				pos += 4;
				this->input.push_back(SetListCMd("NICK", pos, msg));
				break ;
			case 7 :
				pos += 4;
				this->input.push_back(SetListCMd("PASS", pos, msg));
				break ;
			case 8 :
				pos += 4;
				this->input.push_back(SetListCMd("USER", pos, msg));
				break ;
			case 9 :
				pos += 7;
				this->input.push_back(SetListCMd("PRIVMSG", pos, msg));
				break ;
			case 10 :
				pos += 4;
				this->input.push_back(SetListCMd("QUIT", pos, msg));
				break ;
			case 11 :
				pos += 4;
				this->input.push_back(SetListCMd("PART", pos, msg));
				break ;
			default :
				std::cout << "default called" << std::endl;
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

void PrintArg(std::list<std::string> arg)
{
	for (std::list<std::string>::iterator it = arg.begin(); it != arg.end(); it++)
		std::cout << "arg function = " << *it << std::endl;
	return ;
}

/*								members functions								*/

void	Command::Kick(std::list<std::string> *arg)
{
	if (!parsingCmd(this->_client, this->_server, *arg, "KICK"))
		return ;
	std::list<std::string>::iterator it = arg->begin();
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

void	Command::Invite(std::list<std::string> *arg)
{
	if (!CheckArgAndRegister(this->_client, this->_server, *arg, "INVITE"))
		return ;
	std::list<std::string>::iterator it = arg->begin();
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

void	Command::Topic(std::list<std::string> *arg)
{
	std::cout << "Topic function called " << std::endl;
	if (!parsingCmd(this->_client, this->_server, *arg, "TOPIC"))
		return ;
	std::list<std::string>::iterator it = arg->begin();
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

void	Command::Mode(std::list<std::string> *arg)
{
	std::cout << "Mode function called " << std::endl;
	if (!parsingCmd(this->_client, this->_server, *arg, "MODE"))
		return ;
	std::list<std::string>::iterator it = arg->begin();
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

void	Command::join(std::list<std::string> *arg)
{
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

void	Command::nick(std::list<std::string> *arg)
{
	std::cout << "nick function called " << std::endl;
	if (!IsPassGiven(this->_client, this->_server))
		return ;
	if (arg->size() != 1)
	{
		std::list<std::string>::iterator it = arg->begin();
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

void	Command::pass(std::list<std::string> *arg)
{
	// PrintArg(*arg);
	std::cout << "pass function called " << std::endl;
	if (!IsAlreadyRegistered(this->_client, this->_server))
		return ;
	std::list<std::string>::iterator it = arg->begin();
	it++;
	if	(!ThereIsArg(this->_client, this->_server, it, *arg, "PASS"))
		return ;
	std::cout << "Server mdp = /" << this->_server->getPW() << "/ client sent /" << *it << "/" << std::endl;
	if	(this->_server->getPW() == *it)
		this->_client->PassUSer();
	else
		this->_server->SendToClient(this->_client, Builder::ErrPasswdMisMatch() + "\n");
}

void	Command::user(std::list<std::string> *arg)
{
	std::cout << "user function called " << std::endl;
	if (!IsAlreadyRegistered(this->_client, this->_server))
		return ;
	if (arg->size() >= 5)
	{
		std::list<std::string>::iterator i = arg->begin();
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

void	Command::privmsg(std::list<std::string> *arg)
{
	if (!CheckArgAndRegister(this->_client, this->_server, *arg, "JOIN"))
		return ;
	std::list<std::string>::iterator msg = arg->begin();
	for (size_t i = 0; i < arg->size() - 1; i++)
		msg++;
	std::list<std::string>::iterator it = arg->begin();
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

void	Command::quit(std::list<std::string> *arg)
{
	std::cout << "quit function called " << std::endl;
	PrintArg(*arg);
	if (!CheckArgAndRegister(this->_client, this->_server, *arg, "JOIN"))
		return ;
	std::list<std::string>::iterator it = arg->begin();
	it++;
	this->_server->SendToAllChannels(this->_client, Builder::RplQuit(this->_client->getNick(), this->_client->getUser(), *it));
	this->_server->clearClient(this->_client->getFD());
}
void	Command::part(std::list<std::string> *arg)
{
	// if (!CheckArgAndRegister(this->_client, this->_server, *arg, "JOIN"))
	// 	return ;
	std::cout << "part function called " << std::endl;
	std::list<std::string>::iterator it = arg->begin();
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
