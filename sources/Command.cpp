/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:34:36 by endoliam          #+#    #+#             */
/*   Updated: 2025/03/21 20:36:15 by endoliam         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include <algorithm>
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
// bool single_digit (const size_t& value) { return (value == std::string::npos); }

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
/*			members functions				*/
void	Command::Kick(std::list<std::string> *arg)
{
	if (this->_client->isRegistered() && !this->_client->getNick().empty())
	{
	}
	else
		this->_server->SendToClient(this->_client, Builder::ErrNotRegistered() +"\n");
	std::cout << "Kick function called " << std::endl;
	PrintArg(*arg);
}
void	Command::Invite(std::list<std::string> *arg)
{
	if (this->_client->isRegistered() && !this->_client->getNick().empty())
	{
	}
	else
		this->_server->SendToClient(this->_client, Builder::ErrNotRegistered() +"\n");
	std::cout << "Invite function called " << std::endl;
	PrintArg(*arg);
}
void	Command::Topic(std::list<std::string> *arg)
{
	std::cout << "Topic function called " << std::endl;
	if (this->_client->isRegistered() && !this->_client->getNick().empty())
	{
		if (arg->size() != 1)
		{
			std::string	ChannelName = find_Channel(*arg);
			if ( _client->getChannel(ChannelName))
			{
				if (arg->size() == 2 &&  _client->getChannel(ChannelName)->getTopic().empty())
					this->_server->SendToClient(this->_client, Builder::RplNoTopic(ChannelName) + "\n"); 
				std::list<std::string>::iterator it = arg->begin();
				for (size_t i = 0; i < 2; i++)
					it++;
				if (it != arg->end())
				{
					if (it->find(":", 0) == 0)
						it->replace(0, 1, "");
					_client->getChannel(ChannelName)->setTopic(_client, *it);
				}
				if (!_client->getChannel(ChannelName)->getTopic().empty())
					this->_server->SendToClient(this->_client, Builder::RplTopic(ChannelName,  _client->getChannel(ChannelName)->getTopic()) + "\n");		
			}
			else
				this->_server->SendToClient(this->_client, Builder::ErrNotOnChannel(_client->getNick(), ChannelName) + "\n");
		}
		else
			this->_server->SendToClient(this->_client, Builder::ErrNeedMoreParams(_client->getNick(), "TOPIC") + "\n"); 
	}
	else
		this->_server->SendToClient(this->_client, Builder::ErrNotRegistered() +"\n");
	// ERR_CHANOPRIVSNEEDED
}

void	Command::Mode(std::list<std::string> *arg)
{
	std::cout << "Mode function called " << std::endl;
	if (this->_client->isRegistered() && !this->_client->getNick().empty())
	{
		if (arg->size() != 1)
		{
			std::list<std::string>::iterator it = arg->begin();
			it++;
			if (it->find("#", 0) == 0 || it->find("&", 0) == 0 || this->_server->getChannel(*it))
			{
				if (this->_client->getChannel(*it))
				{
					Channel *channel = this->_client->getChannel(*it);
					if (channel->isOP(this->_client))
					{
						it++;
						if (*it == "-i")
						{
							channel->setInviteOnly(this->_client);
							this->_server->SendToClient(this->_client, "add invite only\n");
						}	
						else if(*it == "-t")
						{
						}
						else if(*it == "-k")
						{
							if (!channel->hasPW())
							{
								it++;
								if (it != arg->end())
									channel->setPW(this->_client, *it);
								this->_server->SendToClient(this->_client, "add PW\n");
							}
							else
							{
								channel->deletePW(this->_client);
								this->_server->SendToClient(this->_client, "delete PW\n");
							}
						}
						else if(*it == "-o")
						{
						}
						else if(*it == "-l")
						{
						}
						else
							this->_server->SendToClient(this->_client, Builder::ErrUModeUnknownFlag(this->_client->getNick()) + "\n");
					}
					else
						this->_server->SendToClient(this->_client, Builder::ErrNoPrivileges(this->_client->getNick()) + "\n"); // is not the op
				}
				else
					this->_server->SendToClient(this->_client, Builder::ErrNotOnChannel(this->_client->getNick(), *it) +  "\n");
			}
			else
				this->_server->SendToClient(this->_client, Builder::ErrNoSuchChannel(this->_client->getNick(), *it)  + "\n");
		}
		else
			this->_server->SendToClient(this->_client, Builder::ErrNeedMoreParams(this->_client->getNick(), "MODE"));
	}
	else
		this->_server->SendToClient(this->_client, Builder::ErrNotRegistered() +"\n");
	PrintArg(*arg);
	// — i : Définir/supprimer le canal sur invitation uniquement
	// — t : Définir/supprimer les restrictions de la commande TOPIC pour les opé-
	// rateurs de canaux
	// — k : Définir/supprimer la clé du canal (mot de passe)
	// — o : Donner/retirer le privilège de l’opérateur de canal
	// — l : Définir/supprimer la limite d’utilisateurs pour le canal
	// RPL_CHANNELMODEIS
    // ERR_CHANOPRIVSNEEDED
    // ERR_KEYSET
    // RPL_BANLIST                     RPL_ENDOFBANLIST
    // ERR_UNKNOWNMODE
    // ERR_USERSDONTMATCH              RPL_UMODEIS
    // ERR_UMODEUNKNOWNFLAG
}
std::list<std::string>::iterator	FindLastChannel(std::list<std::string>* arg)
{
	std::list<std::string>::iterator lastChan = arg->end();
	for (std::list<std::string>::iterator it = arg->begin(); it != arg->end(); it++)
	{
		if (it->find("#", 0) == 0 || it->find("&", 0) == 0)
			lastChan = it;
	}
	if (lastChan != arg->end())
		std::cout << "last chan = " << *lastChan << std::endl;
	if (lastChan == arg->end())
		return (arg->end());
	return (lastChan);
}

/*				parse argument and split into 2 list channels and keys			*/
bool	Command::SetCmdJoin(std::list<std::string> &Channels, std::list<std::string> &keys, std::list<std::string> *arg)
{
	std::list<std::string>::iterator lastChan = FindLastChannel(arg);
		if (lastChan == arg->end())
		{
			for (std::list<std::string>::iterator it = arg->begin(); it != arg->end(); ++it)
			{
				if (*it != "JOIN")
					this->_server->SendToClient(this->_client, Builder::BadChannelMask(*it) + "\n");
			}
			return (false);
		}
		bool	isLastChan = false;
		for (std::list<std::string>::iterator it = arg->begin(); it != arg->end(); ++it)
		{
			if (*it == "JOIN")
				it++;
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
	if (this->_client->isRegistered() && !this->_client->getNick().empty())
	{
		if (arg->size() != 1)
		{
			std::list<std::string> Channels;
			std::list<std::string> keys;
			if (this->SetCmdJoin(Channels, keys, arg) == true)
			{
				std::list<std::string>::iterator key = keys.begin();
				for (std::list<std::string>::iterator it = Channels.begin(); it != Channels.end(); it++)
				{
					if (it->find("#", 0) == 0 || it->find("&", 0) == 0)
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
					else
						this->_server->SendToClient(this->_client, Builder::BadChannelMask(*it) + "\n");
				}
			}
		}
		else
			this->_server->SendToClient(this->_client, Builder::ErrNeedMoreParams(this->_client->getNick(), "JOIN") +"\n");
	}
	else
		this->_server->SendToClient(this->_client, Builder::ErrNotRegistered() +"\n");
	// ERR_BANNEDFROMCHAN
	// ERR_TOOMANYCHANNELS
}

void	Command::nick(std::list<std::string> *arg)
{
	std::cout << "nick function called " << std::endl;
	if (this->_client->isRegistered())
	{
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
	}
	else
		this->_server->SendToClient(this->_client, Builder::ErrNotRegistered() +"\n");
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
