#include "Command.hpp"

void	Command::privmsg(std::vector<std::string> *arg)
{
	std::cout << "privmsg function called " << std::endl;
	PrintArg(*arg);
	if (!CheckArgAndRegister(this->_client, this->_server, *arg, "PRIVMSG"))
		return ;
	bool    noText = false;
	std::vector<std::string>::iterator msg = arg->begin();
	for (size_t i = 0; i < arg->size(); i++)
	{
		if (!(*msg).empty() && (*msg)[0] == ':')
		{
			if ((*msg).size() == 2)
				noText = true;
			if (i == 1)
			{
				this->_server->SendToClient(this->_client, Builder::ErrNoRecipient(this->_client->getNick(), "PRIVMSG"));
				return ;
			}
			(*msg).erase(0, 1);
			break ;
		}
		msg++;
	}
	if (msg == arg->end())
		noText = true;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	while (it != msg)
	{
		if (((*it)[0] == '#' || (*it)[0] == '&'))
		{
			if (CheckChannelArg(this->_client, this->_server, *it))
			{
				it->erase(0,1);
				Channel *Channel = this->_server->getChannel(*it);
				if (noText)
					Channel->SendToAll(Builder::ErrNoTextToSend(*it));
				else
					Channel->SendToAllBut(this->_client, Builder::PrivMsg(this->_client, *msg, &(Channel->getName()), NULL));
			}
		}
		else if (IsOnServer(this->_client, this->_server, *it))
		{
			const Client *TargetUser = this->_server->getClientByNick(*it);
			if (noText)
			{
				this->_server->SendToClient(this->_client, Builder::ErrNoTextToSend(*it));
				this->_server->SendToClient(TargetUser, Builder::ErrNoTextToSend(this->_client->getNick()));
			}
			else
				this->_server->SendToClient(TargetUser, Builder::PrivMsg(this->_client, *msg, NULL, TargetUser));
		}
		it++;
	}
}
