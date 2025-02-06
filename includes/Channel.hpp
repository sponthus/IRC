/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 11:25:53 by sponthus          #+#    #+#             */
/*   Updated: 2025/02/06 15:27:48 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>
#include <iostream> // remove after adding link to sending msg

#include "Client.hpp"

class Client;

class Channel {
	public:
		Channel(std::string name);
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
		bool	hasUserLimit() const;
		bool	isTopicRestrict() const;

		// Channel management
		void	joinChannel(Client *client);
		void	joinChannel(Client *client, std::string &PW);
		void	leaveChannel(Client *client);
		void	addOP(Client *client);
		void	removeOP(Client *client);
		void	removeClient(Client *client);

		// Modify channel settings
		void	setTopic(Client *client, std::string &topic);
		void	setPW(Client *client, std::string &PW);
		void	setUserLimit(Client *client, int limit);
		void	deleteUserLimit(Client *client);
		void	deletePW(Client *client);

	private:
		Channel();
		std::string 			_name;
		std::string 			_topic;
		std::string				_PW;
		std::vector<Client *>	_Clients;
		std::vector<Client *>	_ops;
		int						_UserLimit;

		// Channel options : set or not.
		bool				_InviteOnly;
		bool				_HasPW;
		bool				_HasUserLimit;
		bool				_TopicRestrict;

};

#endif