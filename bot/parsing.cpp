/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:02:13 by sponthus          #+#    #+#             */
/*   Updated: 2025/05/07 14:02:34 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Colors.hpp"

bool	isValidPW(std::string arg)
{
	std::string::const_iterator it = arg.begin();
	
	while (it != arg.end())
	{
		if (*it == ' ' || *it == 34 || *it == 39)
			return false;
		it++;
	}
	return true;
}

// Positive port between 1024 and 65535
// To avoid port already used and not usable
bool	isValidPort(std::string arg)
{
	int	port;
	std::string::const_iterator it = arg.begin();

	while (it != arg.end())
	{
		if (std::isdigit((unsigned char)(*it)) == 0)
		{
			std::cout << RED << ERROR << ERR_PORT_NUM << RESET << std::endl;
			return false;
		}
		it++;
	}
	port = std::atoi(arg.c_str());
	if (port < 1024 || port > MAX_PORT)
	{
		std::cout << RED << ERROR << ERR_PORT_VAL << RESET << std::endl;
		return false;
	}
	return true;
}

bool	isCode(std::string arg)
{
	if (arg.size() != 3)
		return false;
	
	std::string::const_iterator it = arg.begin();

	while (it != arg.end())
	{
		if (std::isdigit((unsigned char)(*it)) == 0)
			return false;
		it++;
	}
	return true;
}