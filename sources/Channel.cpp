/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 11:25:35 by sponthus          #+#    #+#             */
/*   Updated: 2025/03/31 11:53:37 by sponthus         ###   ########.fr       */
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
			return ;
		}
	}
}

const std::vector<Client *> Channel::getClients() const
{
	return (this->_Clients);
}

void	Channel::joinChannel(Server *server,Client *client, std::string *PW = NULL)
{
	if (isClient(client))
		return; // Client is already registered in channel, send a response
	if (hasPW())
	{
		if (!PW || (PW && *PW != getPW()))
		{
			server->SendToClient(client, Builder::BadChannelKey(client->getNick(), this->getName()));
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
	server->SendToGroup(this->_Clients, Builder::RplJoin(client->getNick(), client->getUser(), this->getName()));
	if (this->_topic.size() > 0)
		server->SendToClient(client, Builder::RplTopic(getName(), this->_topic));
	// server->SendToClient(client, Builder::RplNamReply(getName(), this->_Clients));
	return;
}

void	Channel::leaveChannel(Client *client)
{
	if (isOP(client))
		removeOP(client);
	std::cout << "PART" << std::endl;
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
		return ;
	std::vector<Client *>::iterator it;
	for (it = _ops.begin(); it != _ops.end(); it++)
	{
		if ((*it)->getNick() == client->getNick())
		{
			_ops.erase(it);
			return ;
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
	this->_server->SendToClient(invited, Builder::RplInviting(this->_name, client->getNick(), invited->getNick()));
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
	if (this->hasPW())
	{	
		this->_server->SendToClient(client, Builder::ErrKeySet(client->getNick(), this->getName()));
		return ;
	}
	this->_server->SendToClient(client, "add PW\n");
	this->_PW = PW;
	this->_HasPW = true;
}

void	Channel::deletePW(Client *client)
{
	this->_server->SendToClient(client, "delete PW\n");
	this->_PW.clear(); // = "";
	this->_HasPW = false;
}

void	Channel::setInviteOnly(Client *client, char Flag)
{
	if (Flag == '+')
	{
		this->_server->SendToClient(client, "add invite only\n");
		this->_InviteOnly = true;
	}
	else if (Flag == '-')
	{
		this->_server->SendToClient(client, "remove invite only\n");
		this->_InviteOnly = false;
	}
}

bool	Channel::isTopicRestrict() const
{
	return (this->_TopicRestrict);
}

const std::string&	Channel::getTopic() const
{
	return (this->_topic);
}

void	Channel::setTopic(std::string &topic)
{
	if (isTopicRestrict())
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

void	Channel::setUserLimit(Client *client, int limit, char Flag)
{
	if (limit <= 0)
		return ; // Invalid value
	if (Flag == '+')
	{
		this->_server->SendToClient(client, "add user limit\n");
		this->_HasUserLimit = true;
		this->_UserLimit = limit;
	}
	else if (Flag == '-')
	{
		this->_server->SendToClient(client, "remove user limit\n");
		this->_HasUserLimit = false;
		this->_UserLimit = 0;
	}
}

void	Channel::setTopicRestriction(Client *client, char Flag)
{
	if (Flag == '-')
	{
		this->_server->SendToClient(client, "remove Topic restriction\n");
		this->_TopicRestrict = false;
	}
	else if (Flag == '+')
	{
		this->_server->SendToClient(client, "add Topic restriction\n");
		this->_TopicRestrict = true;
	}
}
void	Channel::SendToAll(std::string message) const
{
	_server->SendToGroup(this->_Clients, message);
}
