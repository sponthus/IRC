/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:07:08 by sponthus          #+#    #+#             */
/*   Updated: 2025/05/13 11:05:39 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Colors.hpp"

bool	g_shutdown = false;

void	sig(int signal)
{
	(void)signal;
	std::cout << std::endl;
	g_shutdown = true;
}

// No space, no ' or ", no whitespaces, no non-printable char
bool	isValidPW(std::string arg)
{
	std::string::const_iterator it = arg.begin();
	
	if (arg.size() < 1)
		return false;
	while (it != arg.end())
	{
		if (*it == ' ' || *it == 34 || *it == 39)
			return false;
		else if (*it < 33 || *it > 126) // Non printable char
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

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << RED << ERROR << ERR_NB_ARG << RESET << std::endl;
		return 1;
	}
	if (!isValidPort(argv[1]) || !isValidPW(argv[2]))
		return 1;
	try
	{
		Server	server((std::atoi(argv[1])), argv[2]);
		std::cout << BOLDYELLOW << " ---------- Server opened ---------- " << RESET << std::endl;
		std::cout << BOLDYELLOW << "    >> Waiting for connections <<    " << RESET << std::endl;
		signal(SIGINT, sig);
		signal(SIGQUIT, sig);
		while (!g_shutdown)
			server.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << RED << ERROR << e.what() << RESET << std::endl;
	}
	std::cout << BOLDYELLOW" ---------- Server closed ---------- " << RESET << std::endl;
}
