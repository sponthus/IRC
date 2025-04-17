/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 14:58:43 by sponthus          #+#    #+#             */
/*   Updated: 2025/04/17 17:14:46 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

#include <iostream>
#include <string>
#include <map>
#include <signal.h>
#include <vector>
#include <poll.h>
#include <sys/socket.h> // socket(), address
#include <netinet/in.h> // IP specialties 
#include <netdb.h>
#include <arpa/inet.h> // inet_pton
#include <cstdlib> // atoi()
#include <cstring> // memset

#include "Colors.hpp"

# define NICK "QuestBot"
# define USER "QuestBot"

class Bot {
	public:
		Bot(const int port, const char *serverIp, std::string pw);
		~Bot();

		int					getPort() const;

		void	log();
		void	run();

	private:
		Bot(); // Not usable

		std::string		_pw;
		int				_socket;
		std::map<std::string, std::vector<std::string> >	_qa;
};

# define ERROR "Error :"
# define ERR_NB_ARG "Wrong number of arguments, expected 2 :<port> <password>"
# define ERR_PORT_NUM "Port value should be numeric"
# define ERR_PORT_VAL "Valid port should be between 1024 and 65535"
# define ERR_PW "Password should not contain spaces, \' or \""

# define BUFF_SIZE 512
# define MAX_PORT 65535

bool	isValidPW(std::string arg);
bool	isValidPort(std::string arg);

# endif