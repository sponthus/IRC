/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 11:25:53 by sponthus          #+#    #+#             */
/*   Updated: 2025/02/07 11:36:48 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>
#include <iostream> // remove after adding link to sending msg

#include "Client.hpp"
#include "Server.hpp"

class Client;
class Server;

class Channel {
	public:
		Channel(Server *server, std::string name);
		~Channel();

		// Get channel settings
		const std::string	&getTopic() const;
		const std::string	&getName() const;
		const std::string	&getPW() const;
		const int			&getUserLimit() const;
		int		getUserNb() const;
		bool	hasPW() const;
		bool	isOP(Client *client) const;
		bool	isClient(Client *client) const;
		bool	isInviteOnly() const;
		bool	isInvited(Client *client) const;
		bool	hasUserLimit() const;
		bool	isTopicRestrict() const;

		// Channel actions
		void	joinChannel(Client *client, std::string *PW);
		void	leaveChannel(Client *client);
		void	addOP(Client *client);
		void	removeOP(Client *client);
		void	invite(Client *client, Client *invited);
		void	SendToAll(std::string message) const;

		// Modify channel settings
		void	setTopic(Client *client, std::string &topic);
		void	setPW(Client *client, std::string &PW);
		void	setUserLimit(Client *client, int limit);
		void	deleteUserLimit(Client *client);
		void	deletePW(Client *client);
		void	setInviteOnly(Client *client);
		void	deleteInviteOnly(Client *client);

	private:
		Channel();

		Server *				_server;
		std::string 			_name;
		std::string 			_topic;
		std::string				_PW;
		std::vector<Client *>	_Clients;
		std::vector<Client *>	_ops;
		std::vector<Client *>	_InvitedClients;
		int						_UserLimit;

		// Channel options : set or not.
		bool				_InviteOnly;
		bool				_HasPW;
		bool				_HasUserLimit;
		bool				_TopicRestrict;

		// Functions used by class only, no interest from outside
		void	removeClient(Client *client);
};

#endif