/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:34:36 by endoliam          #+#    #+#             */
/*   Updated: 2025/02/06 11:29:07 by sponthus         ###   ########.fr       */
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
}

std::string		FindArgs(int s, std::string msg, size_t pos)
{
	std::string args(msg, pos, pos - msg.find("\r\n", pos));
	return (args);
}
Command::Command(std::string msg)
{
	std::list<size_t>	cmdpos;
	size_t			 	pos = 0;
	
	while (pos != msg.npos)
	{
		int s = FindWichNext(msg, pos);
		switch(s)
		{
			case -1:
				return ;
			case 1 :
				this->input.push_back({"KICK", FindArgs(s, msg, pos + 5)});
				break ;
			case 2 :
				this->input.push_back({"INVITE", FindArgs(s, msg, pos + 7)});
				break ;
			case 3 :
				this->input.push_back({"TOPIC", FindArgs(s, msg, pos + 6)});
				break ;
			case 4 :
				this->input.push_back({"MODE", FindArgs(s, msg, pos + 5)});
				break ;
		}
	}
	return ;
}
Command::Command(Command &rhs)
{
	return ;
}

Command::~Command()
{
	return ;
}
Command	&Command::operator=(Command &rhs)
{
	return (*this);
}


/*			members functions				*/
void	Command::Kick(std::string channel, std::list<std::string> users)
{
}
void	Command::Kick()
{	
}

void	Command::Invite(std::string pseudo, std::string channel)
{
}
void	Command::Invite()
{
}

void	Command::Topic(std::string channel, std::string subject)
{
}
void	Command::Topic()
{
}

void	Command::Mode(std::string ar)
{
}

void	Command::Mode()
{
}