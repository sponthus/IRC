/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 11:25:35 by sponthus          #+#    #+#             */
/*   Updated: 2025/03/21 15:53:21 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// Impossible to instantiate without a name
Channel::Channel()
{}

Channel::~Channel()
{
}

Channel::Channel(Server *server, std::string name) : _server(server), _name(name), _topic(""), _PW(""), \
	 _UserLimit(0), _InviteOnly(false), _HasPW(false), _HasUserLimit(false), \
	 _TopicRestrict(false)
{
}

const std::string&	Channel::getName() const
{
	return (this->_name);
}

bool	Channel::isClient(Client *client) const
{
	std::vector<Client *>::const_iterator it;
	for (it = _Clients.begin(); it != _Clients.end(); it++)
	{
		if ((*it)->getNick() == client->getNick())
			return (true);
	}
	return (false);
}

void	Channel::removeClient(Client *client)
{
	std::vector<Client *>::iterator it;
	for (it = _Clients.begin(); it != _Clients.end(); it++)
	{
		if ((*it)->getNick() == client->getNick())
		{
			_Clients.erase(it);
		}
	}
}

const std::vector<Client *> Channel::getClients() const
{
	return (this->_Clients);
}

void	Channel::joinChannel(Client *client, std::string *PW = NULL)
{
	if (isClient(client))
		return ; // Client is already registered in channel, send a response
	if (hasPW())
	{
		if (!PW || (PW && *PW != getPW()))
		{
			std::cerr << "ERR_BADCHANNELKEY" << std::endl; // To client
			return ; // Wrong PW
		}
	}
	if (hasUserLimit() && getUserLimit() == getUserNb())
	{
		std::cerr << "ERR_CHANNELISFULL" << std::endl; // To client
		return ; // Too many users connected
	}
	if (isInviteOnly() && !isInvited(client))
	{
		std::cerr << "ERR_INVITEONLYCHAN" << std::endl; // To client
		return ; // Channel is invite only you should be invited
	}
	this->_Clients.push_back(client);
	std::cout << "JOIN" << std::endl; // To channel
	if (this->_topic.size() > 0)
		std::cout << "RPL_TOPIC" << std::endl; // To client
	std::cout << "RPL_NAMREPLY" << std::endl; // To client
}

void	Channel::leaveChannel(Client *client)
{
	if (isOP(client))
		removeOP(client);
	std::cout << "PART" << std::endl; // To channel
	removeClient(client);
}

bool	Channel::isOP(Client *client) const
{
	std::vector<Client *>::const_iterator it;
	for (it = _ops.begin(); it != _ops.end(); it++)
	{
		if ((*it)->getNick() == client->getNick())
			return (true);
	}
	return (false);
}

void	Channel::addOP(Client *client)
{
	if (isOP(client))
		return ; // Is already OP
	this->_ops.push_back(client);
}

void	Channel::removeOP(Client *client)
{
	if (!isOP(client))
		return ; // Is not an OP
	std::vector<Client *>::iterator it;
	for (it = _Clients.begin(); it != _Clients.end(); it++)
	{
		if ((*it)->getNick() == client->getNick())
		{
			_Clients.erase(it);
		}
	}
}

bool	Channel::isInviteOnly() const
{
	return (this->_InviteOnly);
}

bool	Channel::isInvited(Client *client) const
{
	std::vector<Client *>::const_iterator it;
	for (it = _InvitedClients.begin(); it != _InvitedClients.end(); it++)
	{
		if ((*it)->getNick() == client->getNick())
			return (true);
	}
	return (false);
}

void	Channel::invite(Client *client, Client *invited)
{
	if (isInviteOnly() && !isOP(client))
	{
		std::cerr << "ERR_CHANOPRIVSNEEDED" << std::endl; // To client
	}
	if (!isInvited(client))
		_InvitedClients.push_back(invited);
	std::cout << "RPL_INVITING" << std::endl; // To client
	std::cout << "INVITE" << std::endl; // To invited
}

void	Channel::setInviteOnly(Client *client)
{
	if (!isOP(client))
		return ; // Is not an OP
	this->_InviteOnly = true;
}

void	Channel::deleteInviteOnly(Client *client)
{
	if (!isOP(client))
		return ; // Is not an OP
	this->_InviteOnly = false;
	_InvitedClients.clear();
}


const std::string&	Channel::getPW() const
{
	return (this->_PW);
}

bool	Channel::hasPW() const
{
	return (this->_HasPW);
}

void	Channel::setPW(Client *client, std::string &PW)
{
	if (!isOP(client))
		return ; // Is not an OP
	this->_PW = PW;
}

void	Channel::deletePW(Client *client)
{
	if (!isOP(client))
		return ; // You have no right to do this you are not OP
	if (!hasPW())
		return ; // Has no PW
	this->_PW = "";
	this->_HasPW = false;
}

bool	Channel::isTopicRestrict() const
{
	return (this->_TopicRestrict);
}

const std::string&	Channel::getTopic() const
{
	return (this->_topic);
}

void	Channel::setTopic(Client *client, std::string &topic)
{
	if (isTopicRestrict() && !isOP(client))
	{
		std::cerr << "ERR_CHANOPRIVSNEEDED" << std::endl;
		return ;
	}
	this->_topic = topic;
	std::cout << "TOPIC" << std::endl;
}

bool	Channel::hasUserLimit() const
{
	return (this->_HasUserLimit);
}

const int&	Channel::getUserLimit() const
{
	return (this->_UserLimit);
}

int	Channel::getUserNb() const
{
	return (this->_Clients.size());
}

void	Channel::setUserLimit(Client *client, int limit)
{
	if (!isOP(client))
		return ; // Is not an OP
	if (limit <= 0)
		return ; // Invalid value
	if (!hasUserLimit())
		this->_HasUserLimit = true;
	this->_UserLimit = limit;
}

void	Channel::deleteUserLimit(Client *client)
{
	if (!isOP(client))
		return ; // Is not an OP
	this->_UserLimit = 0;
	if (hasUserLimit())
		this->_HasUserLimit = false;
}

void	Channel::SendToAll(std::string message) const
{
	_server->SendToGroup(this->_Clients, message);
}
