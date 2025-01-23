/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:07:08 by sponthus          #+#    #+#             */
/*   Updated: 2025/01/23 16:21:38 by sponthus         ###   ########.fr       */
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
	if (!isValid(argv[1]) && !isValid(argv[2]))
		return 1;
	Server	server(atoi(argv[1]), argv[2]);
}