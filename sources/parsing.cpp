/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 10:42:14 by sponthus          #+#    #+#             */
/*   Updated: 2025/01/29 14:15:26 by endoliam         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "messages.hpp"
#include <cstdlib>

// No space, no ' or "
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
bool	isValidPort(std::string arg)
{
	int	port;
	std::string::const_iterator it = arg.begin();

	while (it != arg.end())
	{
		if (std::isdigit((unsigned char)(*it)) == 0)
		{
			std::cout << ERROR << ERR_PORT_NUM << std::endl;
			return false;
		}
		it++;
	}
	port = std::atoi(arg.c_str());
	if (port < 1024 || port > 65535)
	{
		std::cout << ERROR << ERR_PORT_VAL << std::endl;
		return false;
	}
	return true;
}