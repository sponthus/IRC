/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:07:08 by sponthus          #+#    #+#             */
/*   Updated: 2025/01/30 18:22:12 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "messages.hpp"

bool	g_shutdown = false;

void	sig(int signal)
{
	(void)signal;
	g_shutdown = true;
}

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << ERROR << ERR_NB_ARG << std::endl;
		return 1;
	}
	if (!isValidPort(argv[1]) || !isValidPW(argv[2]))
		return 1;
	try
	{
		Server	server((std::atoi(argv[1])), argv[2]);
		std::cout << " ---------- Server opened ---------- " << std::endl;
		std::cout << "    >> Waiting for connections <<    " << std::endl;
		// catch signaux qui turn on g_shutdown
		signal(SIGINT, sig);
		signal(SIGQUIT, sig);
		while (!g_shutdown)
			server.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	std::cout << " ---------- Server closed ---------- " << std::endl;
}
