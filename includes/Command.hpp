/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:25:17 by endoliam          #+#    #+#             */
/*   Updated: 2025/03/17 15:51:14 by endoliam         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <iostream>
#include <list>
#include <vector>
#include <list>

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Builder.hpp"

class Client;
class Server;

class Command
{
	private:
		/*															*/
		Command();
		Command(Command &rhs);

		/*			private members functions						*/

		Server *_server;
		Client *_client; // Client who sent cmd, necessary to exec
		// Channel	*_channel;

	public:
		/*			members functions called by the server			*/
		void	Kick(std::list<std::string> *arg);
		void	Invite(std::list<std::string> *arg);
		void	Topic(std::list<std::string> *arg);
		void	Mode(std::list<std::string> *arg);
		void	join(std::list<std::string> *arg);
		void	nick(std::list<std::string> *arg);
		void	pass(std::list<std::string> *arg);
		void	user(std::list<std::string> *arg);
		void	privmsg(std::list<std::string> *arg);
		void	quit(std::list<std::string> *arg);
		void	part(std::list<std::string> *arg);
	
		Command(Server *server, Client *client, std::string msg);
		~Command();
		Command	&operator=(Command &rhs);
		std::vector<std::list<std::string> >		input;

};

#endif