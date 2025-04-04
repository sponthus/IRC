/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:07:08 by sponthus          #+#    #+#             */
/*   Updated: 2025/04/04 15:23:51 by sponthus         ###   ########.fr       */
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
		std::cerr << RED << e.what() << std::endl;
	}
	std::cout << BOLDYELLOW" ---------- Server closed ---------- " << RESET << std::endl;
}
