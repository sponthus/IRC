/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:18:32 by sponthus          #+#    #+#             */
/*   Updated: 2025/03/24 11:30:39 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <iostream> // Debug msg

Client::Client() : _fd(-1), _address(""), _registered(false)
{}

Client::~Client()
{
	std::cout << "Destructor called for fd " << this->_fd << std::endl;
}

void	Client::setFD(int fd)
{
	this->_fd = fd;
}

void	Client::setAddress(std::string address)
{
	this->_address = address;
}

int		Client::getFD() const
{
	return (this->_fd);
}

const std::string	Client::getAddress() const
{
	return (this->_address);
}

const std::string	Client::getNick() const
{
	return (this->_nick);
}

const std::string	Client::getUser() const
{
	return (this->_user);
}

bool	Client::isRegistered() const
{
	return (this->_registered);
}

Channel*		Client::getChannel(std::string name)
{
	std::vector<Channel *>::iterator	it;
	for (it = _Channels.begin(); it != _Channels.end(); it++)
	{
		if ((*it)->getName() == name)
			return (*it);
	}
	return (NULL);
}

const std::vector<Channel *>		Client::getChannels() const
{
	return (this->_Channels);
}

void	Client::setNick(std::string nickname)
{
	this->_nick = nickname;
}

void	Client::setUser(std::string username)
{
	this->_user = username;
}

void		Client::setFullName(std::string fullname)
{
	this->_fullname = fullname;
}

void		Client::setHostname(std::string hostname)
{
	this->_hostname = hostname;
}

void		Client::setServerName(std::string servername)
{
	this->_servername = servername;
}

void		Client::registerUser()
{
	this->_registered = true;
}
void		Client::addChannel(Channel *_Channel)
{
	this->_Channels.push_back(_Channel);
}
void	Client::leaveChannels()
{
	std::vector<Channel *>::iterator	it;
	for (it = _Channels.begin(); it != _Channels.end(); it++)
	{
		(*it)->leaveChannel(this);
	}
}
