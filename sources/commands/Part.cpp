/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandPart.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:34:36 by endoliam          #+#    #+#             */
/*   Updated: 2025/05/13 10:40:58 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::part(std::vector<std::string> *arg)
{
	std::cout << "part function called " << std::endl;
	PrintArg(*arg);
	if (!CheckArgAndRegister(this->_client, this->_server, *arg, "PART"))
		return ;
	std::string _msg = "leaving";
	std::vector<std::string>::iterator msg = arg->begin();
	for (size_t i = 0; i < arg->size() - 1; i++)
		msg++;
	if (msg->find(":", 0) != 0)
		msg = arg->end();
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	while (it != msg)
	{
		if (!CheckChannelArg(this->_client, this->_server, *it))
			return ;
		it->erase(0,1);
		Channel *ChanToQuit = this->_server->getChannel(*it);
		if (msg != arg->end())
			this->_server->SendToClient(this->_client, Builder::Part(this->_client->getNick(), this->_client->getUser(), ChanToQuit->getName(), &(*msg)));
		else
			this->_server->SendToClient(this->_client, Builder::Part(this->_client->getNick(), this->_client->getUser(), ChanToQuit->getName(), &(_msg)));
		ChanToQuit->leaveChannel(this->_client);
		this->_client->removeChannel(ChanToQuit);
		if (msg != arg->end())
			ChanToQuit->SendToAll(Builder::Part(this->_client->getNick(), this->_client->getUser(), ChanToQuit->getName(), &(*msg)));
		else
			ChanToQuit->SendToAll(Builder::Part(this->_client->getNick(), this->_client->getUser(), ChanToQuit->getName(),  &(_msg)));
		it++;
	}
}

