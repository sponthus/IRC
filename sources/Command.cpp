/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:34:36 by endoliam          #+#    #+#             */
/*   Updated: 2025/02/04 13:47:59 by endoliam         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/*						functions 							*/

Command::Command()
{
	return ;
}

Command::Command(std::string msg)
{
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