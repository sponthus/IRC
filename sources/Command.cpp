/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:34:36 by endoliam          #+#    #+#             */
/*   Updated: 2025/02/06 13:38:18 by endoliam         ###   ########lyon.fr   */
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

int		FindWichNext(std::string msg, size_t &pos)
{
	size_t pos1 = msg.find("/kick", pos);
	size_t pos2 = msg.find("/invite", pos);
	size_t pos3 = msg.find("/topic", pos);
	size_t pos4 = msg.find("/mode", pos);
	if (pos1 == msg.npos && pos2 == msg.npos && pos3 == msg.npos && pos4 == msg.npos)
	{
		pos = msg.npos;
		return (-1);
	}
	else if (pos1 <= pos2 && pos1 <= pos3 && pos1 <= pos4)
	{
		pos = pos1;
		return (1);
	}
	else if (pos2 <= pos1 && pos2 <= pos3 && pos2 <= pos4)
	{
		pos = pos2;
		return (2);
	}
	else if (pos3 <= pos1 && pos3 <= pos2 && pos3 <= pos4)
	{
		pos = pos3;
		return (3);
	}
	else if (pos4 <= pos1 && pos4 <= pos2 && pos4 <= pos3)
	{
		pos = pos4;
		return (4);
	}
	return (0);
}


std::list<std::string>		SetListCMd(std::string cmd, size_t &pos, std::string msg)	
{
	std::list<std::string>	lst;

	lst.push_back(cmd);
	size_t	posendl	= msg.find("\n", pos);
	std::cout << "posendl = " << posendl << std::endl;
	std::cout << "cmd = " << cmd << std::endl;
	while (msg[pos] && pos + 1 <= posendl)
	{
		while(msg[pos] == ' ')
			pos++;
		if (msg[pos])
		{
			std::string arg(msg, pos, (msg.find(" ", pos) - pos));
			std::cout << "arg = " << arg << std::endl;
			lst.push_back(arg);
			pos += arg.size();
		}
	}
	return (lst);
}

Command::Command(std::string msg)
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
	*this = rhs;
	return (*this);
}


/*			members functions				*/
// void	Command::Kick(std::string channel, std::list<std::string> users)
// {
// }
void	Command::Kick()
{	
}

// void	Command::Invite(std::string pseudo, std::string channel)
// {
// }
void	Command::Invite()
{
}

// void	Command::Topic(std::string channel, std::string subject)
// {
// }
void	Command::Topic()
{
}

// void	Command::Mode(std::string ar)
// {
// }

void	Command::Mode()
{
}