/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:25:17 by endoliam          #+#    #+#             */
/*   Updated: 2025/02/10 13:39:19 by endoliam         ###   ########lyon.fr   */
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

class Server;

class Command
{
	private:
		/*															*/
		Command();
		Command(Command &rhs);

		/*			private members functions						*/

		void	Kick(std::string channel, std::list<std::string> user);
		void	Invite(std::string pseudo, std::string channel);
		void	Topic(std::string channel, std::string subject);
		void	Mode(std::string ar1);

		Server *_server;
		Client *_client; // Client who sent cmd, necessary to exec

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