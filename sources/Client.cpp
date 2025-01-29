/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:18:32 by sponthus          #+#    #+#             */
/*   Updated: 2025/01/29 13:07:10 by sponthus         ###   ########.fr       */
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

void		Client::setFD(int fd)
{
	this->_fd = fd;
}

void		Client::setAddress(std::string address)
{
	this->_address = address;
}

int			Client::getFD() const
{
	return (this->_fd);
}

std::string	Client::getAddress() const
{
	return (this->_address);
}

std::string	Client::getNick() const
{
	return (this->_nick);
}

std::string	Client::getUser() const
{
	return (this->_user);
}

bool		Client::isRegistered() const
{
	return (this->_registered);
}

void		Client::setNick(std::string nickname)
{
	this->_nick = nickname;
}

void		Client::setUser(std::string username)
{
	this->_user = username;
}
