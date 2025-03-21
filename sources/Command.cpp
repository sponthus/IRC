/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:34:36 by endoliam          #+#    #+#             */
/*   Updated: 2025/02/12 10:15:29 by sponthus         ###   ########.fr       */
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
	{
		std::cout << "min = " << min << " pos = " << pos << std::endl;
		min = pos;
	}
	return ;
}
bool single_digit (const size_t& value) { return (value == std::string::npos); }

int		FindWichNext(std::string msg, size_t &pos)
{
	std::map<int, size_t> cmd;
	cmd[1] = msg.find("/kick", pos);
	cmd[2] = msg.find("/invite", pos);
	cmd[3] = msg.find("/topic", pos);
	cmd[4] = msg.find("/mode", pos);
	cmd[5] = msg.find("/join", pos);
	cmd[6] = msg.find("/nick", pos);
	cmd[7] = msg.find("/pass", pos);
	cmd[8] = msg.find("/user", pos);
	cmd[9] = msg.find("/privmsg", pos);
	cmd[10] = msg.find("/quit", pos);
	cmd[11] = msg.find("/part", pos);

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
			std::cout << "min = " << first_min << std::endl; // don't work as espect
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
			if (msg[pos] == ':' || msg[pos] == ';')
			{
				posendwrd = posendl;
				if (msg.find(";", pos + 1) < posendl && msg.find(";", pos + 1) != std::string::npos)
				{
					std::cout << "add ; endl " << std::endl;
					posendwrd = msg.find(";", pos + 1);
				}
				if (msg.find(":", pos + 1) < posendwrd && msg.find(":", pos + 1) != std::string::npos)
				{
					std::cout << "add : endl " << std::endl;
					posendwrd = msg.find(":", pos + 1);
				}
			}
			std::string arg(msg, pos, (posendwrd - pos));
			lst.push_back(arg);
			pos += arg.size();
			// std::cout << "arg = " << arg << std::endl;
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
				pos += 5;
				this->input.push_back(SetListCMd("KICK", pos, msg));
				break ;
			case 2 :
				pos += 7;
				this->input.push_back(SetListCMd("INVITE", pos, msg));
				break ;
			case 3 :
				pos += 6;
				this->input.push_back(SetListCMd("TOPIC", pos, msg));
				break ;
			case 4 :
				pos += 5;
				this->input.push_back(SetListCMd("MODE", pos, msg));
				break ;
			case 5 :
				pos += 5;
				this->input.push_back(SetListCMd("JOIN", pos, msg));
				break ;
			case 6 :
				pos += 5;
				this->input.push_back(SetListCMd("NICK", pos, msg));
				break ;
			case 7 :
				pos += 5;
				this->input.push_back(SetListCMd("PASS", pos, msg));
				break ;
			case 8 :
				pos += 5;
				this->input.push_back(SetListCMd("USER", pos, msg));
				break ;
			case 9 :
				pos += 8;
				this->input.push_back(SetListCMd("PRIVMSG", pos, msg));
				break ;
			case 10 :
				pos += 5;
				this->input.push_back(SetListCMd("QUIT", pos, msg));
				break ;
			case 11 :
				pos += 5;
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
	{
		std::cout << "arg function = " << *it << std::endl;
	}
	return ;
}
/*			members functions				*/
void	Command::Kick(std::list<std::string> *arg)
{
	std::cout << "Kick function called " << std::endl;
	PrintArg(*arg);
}
void	Command::Invite(std::list<std::string> *arg)
{
	std::cout << "Invite function called " << std::endl;
	PrintArg(*arg);
}
void	Command::Topic(std::list<std::string> *arg)
{
	std::cout << "Topic function called " << std::endl;
	PrintArg(*arg);
}
void	Command::Mode(std::list<std::string> *arg)
{
	std::cout << "Mode function called " << std::endl;
	PrintArg(*arg);
}
void	Command::join(std::list<std::string> *arg)
{
	std::cout << "join function called " << std::endl;
	PrintArg(*arg);
}
void	Command::nick(std::list<std::string> *arg)
{
	std::cout << "nick function called " << std::endl;
	PrintArg(*arg);
}

void	Command::pass(std::list<std::string> *arg)
{
	std::cout << "pass function called " << std::endl;
	PrintArg(*arg);
}

void	Command::user(std::list<std::string> *arg)
{
	std::cout << "user function called " << std::endl;
	PrintArg(*arg);
}

void	Command::privmsg(std::list<std::string> *arg)
{
	std::cout << "privmsg function called " << std::endl;
	PrintArg(*arg);
}

void	Command::quit(std::list<std::string> *arg)
{
	std::cout << "quit function called " << std::endl;
	PrintArg(*arg);
	
}
void	Command::part(std::list<std::string> *arg)
{
	std::cout << "part function called " << std::endl;
		PrintArg(*arg);
}