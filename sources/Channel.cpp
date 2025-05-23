/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 11:25:35 by sponthus          #+#    #+#             */
/*   Updated: 2025/05/13 16:42:05 by sponthus         ###   ########.fr       */
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
	_ops.clear();
	_Clients.clear();
	_InvitedClients.clear();
}

/* 								JOIN/LEAVE								*/

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
			return ;
		}
	}
}

const std::vector<Client *> &Channel::getClients() const
{
	return (this->_Clients);
}

void	Channel::joinChannel(Server *server,Client *client, std::string *PW = NULL)
{
	if (isClient(client))
		return; // Client is already registered in channel, no response
	if (hasPW())
	{
		if (!PW || (PW && *PW != getPW()))
		{
			server->SendToClient(client, Builder::ErrBadChannelKey(client->getNick(), this->getName()));
			return; // Wrong PW
		}
	}
	if (hasUserLimit() && getUserLimit() == getUserNb())
	{
		server->SendToClient(client, Builder::ErrChannelIsFull(client->getNick(), this->getName()));
		return; // Too many users connected
	}
	if (isInviteOnly() && !isInvited(client))
	{
		server->SendToClient(client, Builder::ErrInviteOnlyChan(client->getNick(), this->getName()));
		return; // Channel is invite only you should be invited
	}
	this->_Clients.push_back(client);
	client->addChannel(server->getChannel(this->getName()));
	this->SendToAll(Builder::Join(client->getNick(), client->getUser(), this->getName()));
	if (!this->_topic.empty())
		server->SendToClient(client, Builder::RplTopic(client->getNick(), getName(), this->_topic));
	else
		server->SendToClient(client, Builder::RplNoTopic(client->getNick(), getName()));
	server->SendToClient(client, Builder::RplNamReply(this, client->getNick()));
	return;
}

void	Channel::leaveChannel(Client *client)
{
	if (isOP(client))
		removeOP(client);
	removeClient(client);
}

/* 								OP								*/

bool	Channel::isOP(const Client *client) const
{
	std::vector<Client *>::const_iterator it;
	if (_ops.size() == 0 || !client)
		return (false);
	for (it = _ops.begin(); it != _ops.end(); it++)
	{
		if ((*it)->getNick() == client->getNick())
			return (true);
	}
	return (false);
}

bool	Channel::addOP(Client *client)
{
	if (isOP(client))
		return (false); // Is already OP
	this->_ops.push_back(client);
	return (true);
}

bool	Channel::removeOP(Client *client)
{
	if (!isOP(client))
		return (false);
	std::vector<Client *>::iterator it;
	for (it = _ops.begin(); it != _ops.end(); it++)
	{
		if ((*it)->getNick() == client->getNick())
		{
			_ops.erase(it);
			return (true);
		}
	}
	return (false);
}

/* 								INVITES								*/

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
	if (!this->isClient(client))
	{
		this->_server->SendToClient(client, Builder::ErrNotOnChannel(client->getNick(), this->getName()));
		return ;
	}
	if (isInviteOnly() && !isOP(client))
	{
		this->_server->SendToClient(client, Builder::ErrChanOPrivsNeeded(client->getNick(), this->getName()));
		return ;
	}
	if (this->isClient(invited))
	{
		this->_server->SendToClient(client, Builder::ErrUserOnChannel(client->getNick(), invited->getNick(), this->getName()));
		return ;
	}
	if (!isInvited(client))
		_InvitedClients.push_back(invited);
	this->_server->SendToClient(client, Builder::RplInviting(this->_name, client->getNick(), invited->getNick()));
	this->_server->SendToClient(invited, Builder::Invite(client, invited->getNick(), this->_name));
}

/* 								GETTER/SETTER								*/

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
	if (!isClient(client))
		return;
	if (this->hasPW())
	{	
		this->_server->SendToClient(client, Builder::ErrKeySet(client->getNick(), this->getName()));
		return ;
	}
	this->_PW = PW;
	this->_HasPW = true;
}

void	Channel::deletePW(Client *client)
{
	if (!isClient(client))
		return;
	this->_PW.clear();
	this->_HasPW = false;
}

void	Channel::setInviteOnly(Client *client, char Flag)
{
	if (!isClient(client))
		return;
	if (Flag == '+')
	{
		this->_InviteOnly = true;
	}
	else if (Flag == '-')
		this->_InviteOnly = false;
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
		this->_server->SendToClient(client, Builder::ErrChanOPrivsNeeded(client->getNick(), this->_name));
		return ;
	}
	this->_topic = topic;
	SendToAll(Builder::RplTopic(client->getNick(), this->getName(), this->getTopic()));
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

void	Channel::setUserLimit(int limit, char Flag)
{
	if (limit < 0)
		return ; // Invalid value
	if (limit == 0 || Flag == '-')
	{
		this->_HasUserLimit = false;
		this->_UserLimit = 0;
	}
	else if (Flag == '+')
	{
		this->_HasUserLimit = true;
		this->_UserLimit = limit;
	}
}

const std::string&	Channel::getName() const
{
	return (this->_name);
}

void	Channel::setTopicRestriction(Client *client, char Flag)
{
	if (!isClient(client))
		return;
	if (Flag == '-')
		this->_TopicRestrict = false;
	else if (Flag == '+')
		this->_TopicRestrict = true;
}

/* 								SEND MESSAGES								*/

void	Channel::SendToAll(std::string message) const
{
	_server->SendToGroup(this->_Clients, message);
}

void	Channel::SendToAllBut(Client *client, std::string message) const
{
	std::vector<Client *>	list;
	
	if (this->_Clients.size() == 0)
		return ;
	for (std::vector<Client *>::const_iterator it = this->_Clients.begin(); it != this->_Clients.end(); it++)
	{
		if (client != *it)
			list.push_back(*it);
	}
	this->_server->SendToGroup(list, message);
}