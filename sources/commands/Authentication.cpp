#include "Command.hpp"

void	Command::nick(std::vector<std::string> *arg)
{
	if (!IsPassGiven(this->_client, this->_server))
		return ;
	if (arg->size() != 1)
	{
		std::vector<std::string>::iterator it = arg->begin();
		it++;
		if (!CheckNickInUse(this->_client, this->_server, *it) || !CheckErroneusNickName(this->_client, this->_server, *it))
			return ;
		if (this->_client->getNick().empty() && this->_client->isRegistered())
			this->_server->SendToClient(this->_client, Builder::RplWelcome(*it, this->_client->getUser()));
		else if (!this->_client->getNick().empty())
		{
			this->_server->SendToClient(this->_client, Builder::Nick(this->_client->getNick(), this->_client->getUser(), *it));
			if (!this->_client->getNick().empty())
				this->_server->EraseClientByNick(this->_client->getNick());
		}
		this->_client->setNick(*it);
		this->_server->SetClientByNick(*it, this->_client);
	}
	else
		this->_server->SendToClient(this->_client, Builder::ErrNoNickGiven(this->_client->getNick()));
}

void	Command::pass(std::vector<std::string> *arg)
{
	if (!IsAlreadyRegistered(this->_client, this->_server))
		return ;
	std::vector<std::string>::iterator it = arg->begin();
	it++;
	if	(!ThereIsArg(this->_client, this->_server, it, *arg, "PASS"))
		return ;
	std::cout << "Server mdp = /" << this->_server->getPW() << "/ client sent /" << *it << "/" << std::endl;
	if	(this->_server->getPW() == *it)
		this->_client->PassUSer();
	else
	{
		this->_server->SendToClient(this->_client, Builder::ErrPasswdMismatch(this->_client->getNick()));
		std::cout << "Server closed client " << this->_client->getFD() << " connexion" << std::endl;
		this->_server->clearClient(this->_client->getFD()); // Disconnects the client
	}
}

void	Command::user(std::vector<std::string> *arg)
{
	if (!IsAlreadyRegistered(this->_client, this->_server))
		return ;
	if (!IsPassGiven(this->_client, this->_server))
		return ;
	std::vector<std::string>::iterator i = arg->begin();
	if (++i == arg->end())
	{
		SetVoidUser(this->_client, this->_server);
		return ;
	}
	this->_client->setUser(*i);
	if (++i == arg->end())
	{
		SetVoidUser(this->_client, this->_server);
		return ;
	}
	this->_client->setHostname(*i);
	if (++i != arg->end())
	{
		SetVoidUser(this->_client, this->_server);
		return ;
	}
	this->_client->setServerName(*i);
	if (++i == arg->end())
	{
		SetVoidUser(this->_client, this->_server);
		return ;
	}
	if (i->find(":", 0) == 0)
		i->erase(0, 1);
	this->_client->setRealname(*i);
	this->_client->registerUser();
	if(!this->_client->getNick().empty())
		this->_server->SendToClient(this->_client, Builder::RplWelcome(this->_client->getNick(), this->_client->getUser()));	
}

/*								COMMON UTILS							*/

bool	IsPassGiven(Client *client, Server *server)
{
	if (!client->isPass())
	{
		server->SendToClient(client, Builder::ErrPasswdMismatch(client->getNick()));
		return (false);
	}
	return (true);
}

bool	IsAlreadyRegistered(Client *client, Server *server)
{
	if (client->isRegistered())
	{
		server->SendToClient(client, Builder::ErrAlreadyRegisted(client->getUser()));
		return (false);
	}
	return (true);
}

/*								USER UTILS								*/

void	SetVoidUser(Client *client, Server *server)
{
	if (client->getUser().empty())
		client->setUser("");
	if (client->getHostname().empty())
		client->setHostname("");
	if (client->getServerName().empty())
		client->setServerName("");
	if (client->getRealName().empty())
		client->setRealname("");
	client->registerUser();
	if(!client->getNick().empty())
		server->SendToClient(client, Builder::RplWelcome(client->getNick(), client->getUser()));
	return ;
}

/*								NICK UTILS								*/

bool	CheckNickInUse(Client *client, Server *server, std::string GivenNick)
{
	 if (server->FindClientByNick(GivenNick))
	 {
		server->SendToClient(client,  Builder::ErrNickInUse(client->getAddress(), GivenNick));
		return (false);
	 }
	 return (true);
}

bool	CheckErroneusNickName(Client *client, Server *server, std::string GivenNick)
{
	if (GivenNick.find('@', 0) != std::string::npos || GivenNick.find('#', 0) != std::string::npos || GivenNick.find('&', 0) != std::string::npos)
	{
		server->SendToClient(client, Builder::ErrErroneusNickname(client->getNick(), GivenNick));
		return (false);
	}
	return (true);
}
