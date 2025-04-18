/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 14:51:46 by sponthus          #+#    #+#             */
/*   Updated: 2025/04/18 16:28:39 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

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
	const char *serverIp = "127.0.0.1";
	const int port = std::atoi(argv[1]);
	try
	{
		std::cout << BOLDYELLOW << " ---------- Bot trying to connect ---------- " << RESET << std::endl;
		Bot	bot(port, serverIp, argv[2]);
		std::cout << BOLDYELLOW << " --------- Bot connected to server --------- " << RESET << std::endl;
		bot.log();
		signal(SIGINT, sig);
		signal(SIGQUIT, sig);
		while (!g_shutdown) {
			bot.run();
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << RED << ERROR << e.what() << RESET << std::endl;
	}
	std::cout << BOLDYELLOW << " ------------- Bot closed ------------- " << RESET << std::endl;
}