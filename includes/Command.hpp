/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:25:17 by endoliam          #+#    #+#             */
/*   Updated: 2025/02/06 17:17:27 by sponthus         ###   ########.fr       */
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
		Command(); // default constructor
		Command(Command &rhs); // copy constructor 

		/*			private members functions						*/

		void	Kick(std::string channel, std::list<std::string> user);
		void	Invite(std::string pseudo, std::string channel);
		void	Topic(std::string channel, std::string *subject);
		void	Mode(std::string ar1);
		void	Join(std::string channel, std::string *key);

		Server *_server;
		Client *_client; // Client who sent cmd, necessary to exec

	public:
		/*			members functions called by the server			*/
		void	Kick(void);
		void	Invite(void);
		void	Topic(void);
		void	Mode(void);
		Command(Server *server, Client *client, std::string msg); // parametric constructor
		~Command(); // default destructor
		Command	&operator=(Command &rhs); // operator = 
		std::vector<std::list<std::string> >		input;


};

#endif