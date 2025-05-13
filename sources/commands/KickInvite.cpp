#include "Command.hpp"

void	Command::Kick(std::vector<std::string> *arg)
{
	if (!parsingCmd(this->_client, this->_server, *arg, "KICK"))
		return ;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	it->erase(0, 1);
	Channel *Channel = this->_client->getChannel(*it);
	std::string ChannelName = *it;
	it++;
	if (!ThereIsArg(this->_client, this->_server, it, *arg, "KICK") 
		|| !IsClientOnChannel(this->_client, this->_server, ChannelName, *it)
		|| !CheckIsOp(this->_client, this->_server, Channel))
		return ;
	const Client *TargetUser = this->_server->getClientByNick(*it);
	Channel->leaveChannel((Client *)TargetUser);
	((Client *)TargetUser)->removeChannel(Channel);
	it++;
	if (it != arg->end())
	{
		this->_server->SendToClient(TargetUser, Builder::Kick(this->_client->getNick(), TargetUser->getNick(), Channel->getName(), &(*it)));
		Channel->SendToAll(Builder::Kick(this->_client->getNick(), TargetUser->getNick(), Channel->getName(), &(*it)));
	}
	else
	{
		this->_server->SendToClient(TargetUser, Builder::Kick(this->_client->getNick(), TargetUser->getNick(), Channel->getName(), NULL));
		Channel->SendToAll(Builder::Kick(this->_client->getNick(), TargetUser->getNick(), Channel->getName(), NULL));
	}
}

void	Command::Invite(std::vector<std::string> *arg)
{
	if (!CheckArgAndRegister(this->_client, this->_server, *arg, "INVITE"))
		return ;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	if (!ThereIsArg(this->_client, this->_server, it, *arg, "INVITE") || !IsOnServer(this->_client, this->_server, *it))
		return ;
	const Client *TargetUser = this->_server->getClientByNick(*it);
	it++;
	if (!ThereIsArg(this->_client, this->_server, it, *arg, "INVITE") || !CheckMaskChan(this->_client, this->_server, &(*it)))
		return ;
	Channel *Channel = this->_server->getChannel(*it);
	if (!Channel)
	{
		this->_server->initChannel(*it);
		Channel = this->_server->getChannel(*it);
		Channel->addOP(this->_client);
		Channel->joinChannel(this->_server, this->_client, NULL);
	}
	Channel->invite(this->_client, (Client *)TargetUser);
}
