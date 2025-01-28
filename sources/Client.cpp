/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:18:32 by sponthus          #+#    #+#             */
/*   Updated: 2025/01/28 15:20:37 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : _fd(-1), _address("")
{}

Client::~Client()
{
	if (this->_fd != -1)
		close(this->_fd);
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
