/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:07:08 by sponthus          #+#    #+#             */
/*   Updated: 2025/01/29 14:15:12 by endoliam         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "messages.hpp"

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << ERROR << ERR_NB_ARG << std::endl;
		return 1;
	}
	if (!isValidPort(argv[1]) || !isValidPW(argv[2]))
		return 1;
	Server	server((std::atoi(argv[1])), argv[2]);
	std::cout << " ---------- Server opened ---------- " << std::endl;
	try
	{
		// catch signaux
		server.init();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	std::cout << " ---------- Server closed ---------- " << std::endl;
}
