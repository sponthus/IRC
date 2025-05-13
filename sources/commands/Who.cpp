
#include "Command.hpp"

void	Command::Who(std::vector<std::string> *arg)
{
	if (!parsingCmd(this->_client, this->_server, *arg, "WHO"))
		return ;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	while (it != arg->end())
	{
		if (CheckMaskChan(this->_client, this->_server, &(*it)))
		{
			Channel *ChanToWho = this->_server->getChannel(*it);
			std::string ChanToWhoName = *it;
			if (IsClientOnChannel(this->_client, this->_server, ChanToWhoName, this->_client->getNick()))
			{
				for (std::vector<Client *>::const_iterator i = ChanToWho->getClients().begin(); i != ChanToWho->getClients().end(); i++)
					this->_server->SendToClient(this->_client, Builder::RplWhoReply(ChanToWho, this->_client, (*i))); // 352 RPL_WHOREPLY
			}
			this->_server->SendToClient(this->_client, Builder::RplEndOfWho(this->_client->getNick(), ChanToWho->getName()));
		}
		it++;
	}
}
