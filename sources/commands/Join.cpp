/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandJoin.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:34:36 by endoliam          #+#    #+#             */
/*   Updated: 2025/05/13 10:41:26 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::join(std::vector<std::string> *arg)
{
	std::cout << "join function called " << std::endl;
	PrintArg(*arg);
	if (!CheckArgAndRegister(this->_client, this->_server, *arg, "JOIN"))
		return ;
	std::map<std::string, std::string> JoinnedChan;
	setMapJoin(&JoinnedChan, arg);
	for (std::map<std::string, std::string>::iterator it = JoinnedChan.begin(); it != JoinnedChan.end(); it++)
	{
		if (CheckMaskChan(this->_client, this->_server, (std::string *)&it->first))
		{
			if (this->_server->getChannel(it->first) == NULL)
			{
				this->_server->initChannel(it->first);
				this->_server->getChannel(it->first)->addOP(this->_client);
			}
			this->_server->getChannel(it->first)->joinChannel(this->_server, this->_client, &(it->second));
		}
	}
}

/*								JOIN UTILS								*/

std::vector<std::string>::iterator	FindLastChannel(std::vector<std::string>* arg)
{
	std::vector<std::string>::iterator lastChan = arg->end();
	for (std::vector<std::string>::iterator it = arg->begin(); it != arg->end(); it++)
	{
		if (it->find("#", 0) == 0 || it->find("&", 0) == 0)
			lastChan = it;
	}
	return (lastChan);
}

void	setMapJoin(std::map<std::string, std::string> *JoinnedChan, std::vector<std::string> *arg)
{
	std::vector<std::string>::iterator lastChan = FindLastChannel(arg);
	std::vector<std::string>::iterator it = arg->begin();
	if (lastChan != arg->end())
		lastChan++;
	std::vector<std::string>::iterator key = lastChan;
	while (++it != arg->end() && it != lastChan)
	{
		if (key != arg->end())
		{	
			(*JoinnedChan)[*it] = *key;
			key++;
		}
		else
			(*JoinnedChan)[*it] = "";
	}
	return ;
}

