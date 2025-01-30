/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 11:25:35 by sponthus          #+#    #+#             */
/*   Updated: 2025/01/30 17:37:57 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// Impossible to instantiate without a name
Channel::Channel()
{}

Channel::~Channel()
{
}

Channel::Channel(std::string name) : _name(name), _topic(""), _PW(""), \
	 _UserLimit(0), _InviteOnly(false), _HasPW(false), _HasUserLimit(false), \
	 _TopicRestrict(false)
{
}

// Get channel settings
const std::string&	Channel::getTopic() const
{
	return (this->_topic);
}

const std::string&	Channel::getName() const
{
	return (this->_name);
}

const std::string&	Channel::getPW() const
{
	return (this->_PW);
}

const int&	Channel::getUserLimit() const
{
	return (this->_UserLimit);
}

int	Channel::getUserNb() const
{
	return (this->_Clients.size());
}

bool	Channel::hasPW() const
{
	return (this->_HasPW);
}

bool	Channel::isClient(Client &client) const
{
	std::vector<Client>::const_iterator it;
	for (it = _Clients.begin(); it != _Clients.end(); it++)
	{
		if (it->getNick() == client.getNick())
			return (true);
	}
	return (false);
}

bool	Channel::isOP(Client &client) const
{
	std::vector<Client>::const_iterator it;
	for (it = _ops.begin(); it != _ops.end(); it++)
	{
		if (it->getNick() == client.getNick())
			return (true);
	}
	return (false);
}

bool	Channel::isInviteOnly() const
{
	return (this->_InviteOnly);
}

bool	Channel::hasUserLimit() const
{
	return (this->_HasUserLimit);
}

bool	Channel::isTopicRestrict() const
{
	return (this->_TopicRestrict);
}

// Channel management
void	Channel::joinChannel(Client &client)
{
	if (isClient(client))
		return ; // Client is already registered in channel, send a response
	if (hasPW())
		return ; // Channel is PW protected, send a response
	if (hasUserLimit() && getUserLimit() == getUserNb())
		return ; // Too many users connected
	if (isInviteOnly())
		return ; // Channel is invite only you should be invited
	this->_Clients.push_back(client);
}

void	Channel::joinChannel(Client &client, std::string &PW)
{
	if (isClient(client))
		return ; // Client is already registered in channel, send a response
	if (hasPW() && PW != getPW())
		return ; // Wrong PW
	if (hasUserLimit() && getUserLimit() == getUserNb())
		return ; // Too many users connected
	if (isInviteOnly())
		return ; // Channel is invite only you should be invited
	this->_Clients.push_back(client);
}

void	Channel::addOP(Client &client)
{
	if (isOP(client))
		return ; // Is already OP
	this->_ops.push_back(client);
}

void	Channel::removeOP(Client &client)
{
	if (!isOP(client))
		return ; // Is not an OP
	std::vector<Client>::iterator it;
	for (it = _Clients.begin(); it != _Clients.end(); it++)
	{
		if (it->getNick() == client.getNick())
		{
			_Clients.erase(it);
		}
	}
}

// Modify channel settings
void	Channel::setTopic(Client &client, std::string &topic)
{
	if (isTopicRestrict() && !isOP(client))
		return ; // You have no right to do this
	this->_topic = topic;
}

void	Channel::setPW(Client &client, std::string &PW)
{
	if (!isOP(client))
		return ; // Is not an OP
	this->_PW = PW;
}

void	Channel::setUserLimit(Client &client, int limit)
{
	if (!isOP(client))
		return ; // Is not an OP
	if (limit <= 0)
		return ; // Invalid value
	if (!hasUserLimit())
		this->_HasUserLimit = true;
	this->_UserLimit = limit;
}

void	Channel::deleteUserLimit(Client &client)
{
	if (!isOP(client))
		return ; // Is not an OP
	this->_UserLimit = 0;
	if (hasUserLimit())
		this->_HasUserLimit = false;
}

void	Channel::deletePW(Client &client)
{
	if (!isOP(client))
		return ; // You have no right to do this you are not OP
	if (!hasPW())
		return ; // Has no PW
	this->_PW = "";
	this->_HasPW = false;
}
