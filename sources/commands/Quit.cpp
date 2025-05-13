#include "Command.hpp"

void	Command::quit(std::vector<std::string> *arg)
{
	if (!this->_client->isRegistered() || this->_client->getNick().empty())
	{
		this->_server->SendToClient(this->_client, Builder::ErrNotRegistered());
		return;
	}
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	if (it != arg->end())
		this->_server->SendToAllChannels(this->_client, Builder::Quit(this->_client->getNick(), this->_client->getUser(), *it));
	else
		this->_server->SendToAllChannels(this->_client, Builder::Quit(this->_client->getNick(), this->_client->getUser(), "leaving"));
	this->_server->clearClient(this->_client->getFD());
}
