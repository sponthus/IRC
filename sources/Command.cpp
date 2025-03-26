/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:34:36 by endoliam          #+#    #+#             */
/*   Updated: 2025/03/26 14:52:46 by endoliam         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/*						functions 							*/

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

/*			members functions				*/

void	Command::Kick(std::list<std::string> *arg)
{
	if (!parsingCmd(this->_client, this->_server, *arg, "KICK"))
		return ;
	std::cout << "Kick function called " << std::endl;
	PrintArg(*arg);
}
void	Command::Invite(std::list<std::string> *arg)
{
	if (!parsingCmd(this->_client, this->_server, *arg, "INVITE"))
		return ;
	std::cout << "Invite function called " << std::endl;
	PrintArg(*arg);
}
std::string		find_Channel(std::list<std::string> arg)
{
	for (std::list<std::string>::iterator i = arg.begin(); i != arg.end(); i++)
	{
		std::string	channel = *i;
		if (channel.find("#", 0) == 0 || channel.find("&", 0) == 0 )
			return (channel);
	}
	return (NULL);
}

void	Command::Topic(std::list<std::string> *arg)
{
	std::cout << "Topic function called " << std::endl;
	if (!parsingCmd(this->_client, this->_server, *arg, "TOPIC"))
		return ;
	std::list<std::string>::iterator it = arg->begin();
	it++;
	std::string	ChannelName = *it;
	it++;
	if (!ThereIsArg(this->_client, this->_server, it, *arg, "TOPIC"))
		return ;
	if (it == arg->end() && _client->getChannel(ChannelName)->getTopic().empty())
		this->_server->SendToClient(this->_client, Builder::RplNoTopic(ChannelName) + "\n"); 
	if (it != arg->end())
	{
		if (it->find(":", 0) == 0)
			it->replace(0, 1, "");
		_client->getChannel(ChannelName)->setTopic(*it);
	}
	if (!_client->getChannel(ChannelName)->getTopic().empty())
		this->_server->SendToClient(this->_client, Builder::RplTopic(ChannelName,  _client->getChannel(ChannelName)->getTopic()) + "\n");		
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

std::list<std::string>::iterator	FindLastChannel(std::list<std::string>* arg)
{
	std::list<std::string>::iterator lastChan = arg->end();
	for (std::list<std::string>::iterator it = arg->begin(); it != arg->end(); it++)
	{
		if (it->find("#", 0) == 0 || it->find("&", 0) == 0)
			lastChan = it;
	}
	return (lastChan);
}

/*				parse argument and split into 2 list channels and keys			*/
bool	Command::SetCmdJoin(std::list<std::string> &Channels, std::list<std::string> &keys, std::list<std::string> *arg)
{
	std::list<std::string>::iterator lastChan = FindLastChannel(arg);
	std::list<std::string>::iterator it = arg->begin();
	/*				block					*/
	if (lastChan == arg->end())
	{
		while (++it != arg->end())
				this->_server->SendToClient(this->_client, Builder::BadChannelMask(*it) + "\n");
		return (false);
	}
	/*				block					*/
	bool	isLastChan = false;
	it = arg->begin();
	while (++it != arg->end())
	{
		if (!isLastChan)
			Channels.push_back(*it);
		else
			keys.push_back(*it);
		if (it == lastChan)
			isLastChan = true;
	}
	return (true);
}

void	Command::join(std::list<std::string> *arg)
{
	if (!CheckArgAndRegister(this->_client, this->_server, *arg, "JOIN"))
		return ;
	std::list<std::string> Channels;
	std::list<std::string> keys;
	if (!this->SetCmdJoin(Channels, keys, arg))
		return ;
	std::list<std::string>::iterator key = keys.begin();
	for (std::list<std::string>::iterator it = Channels.begin(); it != Channels.end(); it++)
	{
		if (CheckMaskChan(this->_client, this->_server, *it))
		{
			if (this->_server->getChannel(*it) == NULL)
			{
				this->_server->initChannel(*it);
				this->_server->getChannel(*it)->addOP(this->_client);
			}
			if (key != keys.end() && !key->empty())
			{
				this->_server->getChannel(*it)->joinChannel(this->_server, this->_client, &(*key));
				key++;
			}
			else
				this->_server->getChannel(*it)->joinChannel(this->_server, this->_client, NULL);
		}	
	}
}

void	Command::nick(std::list<std::string> *arg)
{
	std::cout << "nick function called " << std::endl;
	if (arg->size() != 1)
	{
		std::list<std::string>::iterator i = arg->begin();
		i++;
		if (!this->_server->FindClientByNick(*i))
		{
			if (this->_client->getNick().empty())
				this->_server->SendToClient(this->_client, Builder::Welcome(*i, this->_client->getUser()) + "\n");
			else
			{
				this->_server->SendToClient(this->_client, Builder::Nick(this->_client->getNick(), this->_client->getUser(), *i) + "\n");
				this->_server->EraseClientByNick(this->_client->getNick());
			}
			this->_client->setNick(*i);
			this->_server->SetClientByNick(*i, this->_client);
		}
		else
			this->_server->SendToClient(this->_client,  Builder::ErrNickInUse(this->_client->getAddress(), *i) + "\n");
	}
	else
		this->_server->SendToClient(this->_client, Builder::ErrNoNickGiven(this->_client->getNick()) + "\n");
	//ERR_ERRONEUSNICKNAME
}

void	Command::pass(std::list<std::string> *arg)
{
	if (this->_client->isRegistered() && !this->_client->getNick().empty())
	{
	}
	else
		this->_server->SendToClient(this->_client, Builder::ErrNotRegistered() +"\n");
	std::cout << "pass function called " << std::endl;
	PrintArg(*arg);
}

void	Command::user(std::list<std::string> *arg)
{
	std::cout << "user function called " << std::endl;
	if (!this->_client->isRegistered())
	{
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
			this->_server->SendToClient(this->_client, "-------you've been successfully registered-------\n");
		}
		else
			this->_server->SendToClient(this->_client, Builder::ErrNeedMoreParams(this->_client->getNick(), "USER") + "\n");
	}
	else
		this->_server->SendToClient(this->_client, Builder::ErrAlreadyRegisted(this->_client->getUser()) + "\n");
}

void	Command::privmsg(std::list<std::string> *arg)
{
	if (this->_client->isRegistered() && !this->_client->getNick().empty())
	{
	}
	else
		this->_server->SendToClient(this->_client, Builder::ErrNotRegistered() +"\n");
	std::cout << "privmsg function called " << std::endl;
	PrintArg(*arg);
}

void	Command::quit(std::list<std::string> *arg)
{
	if (this->_client->isRegistered() && !this->_client->getNick().empty())
	{
	}
	else
		this->_server->SendToClient(this->_client, Builder::ErrNotRegistered() +"\n");
	std::cout << "quit function called " << std::endl;
	PrintArg(*arg);
	
}
void	Command::part(std::list<std::string> *arg)
{
	if (this->_client->isRegistered() && !this->_client->getNick().empty())
	{
	}
	else
		this->_server->SendToClient(this->_client, Builder::ErrNotRegistered() +"\n");
	std::cout << "part function called " << std::endl;
		PrintArg(*arg);
}
