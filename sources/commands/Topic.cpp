#include "Command.hpp"

void	Command::Topic(std::vector<std::string> *arg)
{
	if (!parsingCmd(this->_client, this->_server, *arg, "TOPIC"))
		return ;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	it->erase(0, 1);
	Channel *Channel = this->_client->getChannel(*it);
	it++;
	if (it == arg->end() && Channel->getTopic().empty())
	{
		this->_server->SendToClient(this->_client, Builder::RplNoTopic(this->_client->getNick(), Channel->getName()));
		return ;
	}
	else if (it != arg->end())
	{
		if (it->find(":", 0) == 0)
			it->replace(0, 1, "");
		if (Channel->isTopicRestrict() && !CheckIsOp(this->_client, this->_server, Channel))
			return ;
		Channel->setTopic(this->_client, *it); 
	}
	else
		this->_server->SendToClient(this->_client, Builder::RplTopic(this->_client->getNick(), Channel->getName(), Channel->getTopic()));
}
