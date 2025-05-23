/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:00:24 by sponthus          #+#    #+#             */
/*   Updated: 2025/05/16 13:28:29 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"
#include "Colors.hpp"

extern bool	g_shutdown;

// Private
Server::Server()
{
}

Server::Server(int port, std::string pw) : _port(port), _pw(pw), _socketFD(-1)
{
	SetCmdMap();
	initSocket();
	initPoll(this->_socketFD);
}

Server::~Server()
{
	if (this->_socketFD != -1)
	{
		shutdown(this->_socketFD, SHUT_RDWR);
		close(this->_socketFD);
	}
	for (std::vector<Client*>::iterator it = _Clients.begin(); it != _Clients.end(); ++it)
	{
		if (*it)
			delete *it;
	}
	_Clients.clear();
	_ClientsByNick.clear();
	for (std::vector<struct pollfd>::iterator it = _fds.begin(); it != _fds.end(); it++)
	{
		close((*it).fd);
	}
	for (std::map<std::string, Channel*>::iterator it = _ChannelsByName.begin(); it != _ChannelsByName.end(); ++it)
    {
        if (it->second)
			delete it->second;
    }
	_ChannelsByName.clear();
}

void	Server::SetCmdMap()
{
	this->CmdMap["KICK"] = &Command::Kick;
	this->CmdMap["INVITE"] = &Command::Invite;
	this->CmdMap["TOPIC"] = &Command::Topic;
	this->CmdMap["MODE"] = &Command::Mode;
	this->CmdMap["JOIN"] = &Command::join;
	this->CmdMap["NICK"] = &Command::nick;
	this->CmdMap["PASS"] = &Command::pass;
	this->CmdMap["USER"] = &Command::user;
	this->CmdMap["PRIVMSG"] = &Command::privmsg;
	this->CmdMap["QUIT"] = &Command::quit;
	this->CmdMap["PART"] = &Command::part;
	this->CmdMap["WHO"] = &Command::Who;
	return ;
}

void	Server::initSocket()
{
	this->_socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socketFD <= 0)
	{
		throw std::runtime_error("socket creation failed");
	}

	int	val = 1;
	if (setsockopt(this->_socketFD, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1) // Parameters to allow immediate reuse of the port without waiting time
	{
		throw std::runtime_error("setsockopt failed on socket");
	}
	
	struct sockaddr_in	address;
	address.sin_addr.s_addr = INADDR_ANY; // Accepts connexions from any local IP (0.0.0.0)
	address.sin_family = AF_INET; // IPV4
	address.sin_port = htons(this->_port); // Links port and network (converts byte orders)
	if (fcntl(this->_socketFD, F_SETFL, O_NONBLOCK) == -1) // Configuration of socket in non-blocking, can treat info while listening
	{
		throw std::runtime_error("fcntl failed on socket");
	}

	if (bind(this->_socketFD, (struct sockaddr *)&address, sizeof(address)) == -1) // Affects the address to the socket
	{
		throw std::runtime_error("bind failed on socket");
	}

	if (listen(this->_socketFD, SOMAXCONN) == -1) // Now is listening, with max connexions = SOMAXCONN (the limit)
	{
		throw std::runtime_error("listen failed on socket");
	}
}

void	Server::initPoll(int fd)
{
	struct pollfd poll;
	poll.fd = fd;
	poll.events = POLLIN;
	poll.revents = 0;
	this->_fds.push_back(poll);
}

// fd from the client who sent a msg
// Reccursively recieves data if buffer is not big enouth
std::string	Server::recieveData(int fd, std::string msg)
{
	ssize_t	size = 1;
	char	buffer[BUFF_SIZE];
	memset(buffer, 0, sizeof(buffer));
	size = recv(fd, buffer, sizeof(buffer) - 1, 0);
	std::string str = msg;
	if (size < 0)
	{
		std::cout << RED << "recv() error: " << strerror(errno) << RESET << std::endl;
		clearClient(fd);
	}
	else if (size == 0)
	{
		std::cout << CYAN << "Client closed connection properly (fd " << fd << ")" << RESET << std::endl;
		clearClient(fd);
	}
	else
	{
		buffer[size] = '\0';
		str += buffer;
		if (size + 1 == BUFF_SIZE)
			str = recieveData(fd, str);
	}
	return (str);
}

void	Server::clearClient(int fd)
{
	close(fd);
	this->_ClientsByFD[fd]->leaveChannels();
	for (size_t i = 0; i < this->_fds.size(); i++)
	{
		if (this->_fds[i].fd == fd)
		{
			this->_fds.erase(this->_fds.begin() + i);
			break;
		}
	}
	for (std::vector<Client*>::iterator it = this->_Clients.begin(); it != this->_Clients.end(); ++it)
	{
		if ((*it)->getFD() == fd) {
			if (!(*it)->getNick().empty()) {
				this->_ClientsByNick.erase((*it)->getNick());
			}
			this->_ClientsByFD.erase(fd);
			delete *it;
			this->_Clients.erase(it);
			break;
		}
	}
}

void	Server::initClient(int fd, struct sockaddr_in ClientAddress)
{
	Client *client = new Client();
	client->setFD(fd);
	client->setAddress(inet_ntoa(ClientAddress.sin_addr));
	this->_Clients.push_back(client);
	this->_ClientsByFD[fd] = client;
}

void	Server::connectClient()
{
	std::cout << CYAN << "Accepting client" << RESET << std::endl;
	struct sockaddr_in	ClientAddress;
	socklen_t			LenAddress = sizeof(ClientAddress);

	int	fd = accept(this->_socketFD, (sockaddr *)&ClientAddress, &LenAddress);
	if (fd == -1)
	{
		std::cerr << RED << "Accept failed" << RESET << std::endl;
		close(fd);
		return ;
	}
	std::cout << CYAN << "New connection accepted on fd " << fd << RESET << std::endl;
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << RED << "fcntl failed on new client" << RESET << std::endl;
		close(fd);
		return ;
	}
	try
	{
		initClient(fd, ClientAddress);
	    initPoll(fd);
        std::cout << GREEN << "Client " << fd << " connected successfully with IP " 
                  << inet_ntoa(ClientAddress.sin_addr) << RESET << std::endl;
    }
    catch (const std::exception& e)
	{
        std::cerr << RED << "Error setting up client: " << e.what() << RESET << std::endl;
        close(fd);
        return;
    }
}

void	Server::sendData(int fd, std::string response) const
{
	if (send(fd, response.c_str(), response.size(), 0) == -1)
	{
		std::cerr << RED "send failed on response : " << response << RESET << std::endl;
	}
	std::cout << CYAN << "Sent data to " << fd << ": " << response << RESET;
}

void	Server::handleData(Client *cl, std::string message)
{
	Command	cmd(this, cl, message);
	for (std::vector<std::vector<std::string> >::iterator i = cmd.input.begin(); i != cmd.input.end() ; i++)
	{
		for (std::map<std::string, void(Command::*)(std::vector<std::string> *arg)>::iterator it = this->CmdMap.begin(); it != this->CmdMap.end(); it++)
		{
			if (!i->empty() && it->first == (*i->begin()))
			{
				(cmd.*(it->second))(&(*i));
			}
		}
	}
}

// Check if \r\n is recieved
// then stores the full message in *message
// and the remaining data in cl->_Message
bool	Server::messageIsFull(Client *cl, std::string *message)
{
	if (message->size() == 0)
		return (false);
	cl->addMessage(*message);
	size_t pos = cl->getMessage().find_last_of("\r\n");
	if (pos == std::string::npos) // Data does not contain \r\n, everything is stored
	{
		return (false);
	}
	if (pos != cl->getMessage().size() - 1) // Data doesn't end with /r/n but contains it, store msg + rest
	{
		*message = cl->getMessage().substr(0, pos);
		std::string keep = cl->getMessage().substr(pos + 2, cl->getMessage().size() - pos - 2);
		cl->setMessage(keep);
	}
	else // Data ends with /r/n -> Store everything in *message
	{
		*message = cl->getMessage();
		cl->clearMessage();
	}
	return (true);
}

// If multiple messages with \r\n recieved in 1 row, splits them
std::vector<std::string> Server::splitMessages(const std::string& raw)
{
	std::vector<std::string> messages;
	size_t start = 0, end;

	while ((end = raw.find("\r\n", start)) != std::string::npos)
	{
		end += 2;
		messages.push_back(raw.substr(start, end - start));
		start = end;
	}
	return messages;
}

void	Server::printMessage(Client *cl, std::string message, size_t i)
{
	if (cl && cl->getNick().empty())
		std::cout << MAGENTA << this->_fds[i].fd;
	else if (cl)
		std::cout << MAGENTA << cl->getNick();
	std::cout << MAGENTA << " sent: " << message << RESET;
	if (message.empty() || *(message.end() - 1) != '\n')
		std::cout << std::endl;
}

void	Server::run()
{
	if (poll(&(this->_fds[0]), this->_fds.size(), -1) == -1 && g_shutdown == false)
		throw(std::runtime_error("poll failed"));
	for (size_t i = 0; i < this->_fds.size(); i++)
	{
		if (this->_fds[i].revents != 0)
		{
			if (this->_fds[i].fd == this->_socketFD)
				connectClient();
			else if (this->_fds[i].revents & POLLNVAL)
			{
				std::cerr << RED << "POLLNVAL detected for fd " << this->_fds[i].fd << RESET << std::endl;
				clearClient(this->_fds[i].fd);
			}
			else if (this->_fds[i].revents & POLLIN)
			{
				std::string message = recieveData(this->_fds[i].fd, "");
				Client* cl = this->_ClientsByFD[this->_fds[i].fd];
				printMessage(cl, message, i);
				if (messageIsFull(cl, &message))
				{
					std::vector<std::string> messages = splitMessages(message);
					for (size_t i = 0; i < messages.size(); ++i)
					{
						if (!messages[i].empty() && messages[i].size() > 2)
							std::cout << MAGENTA << " - " << messages[i] << RESET;
						handleData(cl, messages[i]);
					}
				}
			}
			else if (this->_fds[i].revents & (POLLHUP | POLLERR))
			{
				std::cerr << RED << "POLLHUP or POLLERR detected for fd " << this->_fds[i].fd << RESET << std::endl;
				clearClient(this->_fds[i].fd);
			}
			this->_fds[i].revents = 0;
		}
	}
}

int	Server::getPort() const
{
	return this->_port;
}

const std::string	Server::getPW() const
{
	return this->_pw;
}

const Client *		Server::getClientByNick(std::string nick) const
{
	const std::map<std::string, Client *>::const_iterator it = this->_ClientsByNick.find(nick);
	if (it != this->_ClientsByNick.end())
		return (it->second);
	return (NULL);
}

void	Server::SetClientByNick(std::string nick, Client *client)
{
	this->_ClientsByNick[nick] = client;
}

void	Server::EraseClientByNick(std::string nick)
{
	std::map<std::string, Client *>::iterator it = this->_ClientsByNick.find(nick);
	this->_ClientsByNick.erase(it);
}

bool	Server::FindClientByNick(std::string nick)
{
	if (this->_ClientsByNick.find(nick) != this->_ClientsByNick.end())
		return (true);
	return (false);
}

void	Server::initChannel(std::string name)
{
	Channel *channel = new Channel(this, name);
	this->_ChannelsByName[name] = channel;
}

bool	Server::isChannel(std::string name)
{
	return _ChannelsByName.count(name) > 0;
}

Channel*	Server::getChannel(std::string name)
{
	if (isChannel(name))
		return (_ChannelsByName[name]);
	else
		return (NULL);
}

void	Server::SendToGroup(const std::vector<Client *> clients, const std::string message) const
{
	if (clients.size() == 0)
		return ;
	for (std::vector<Client *>::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		sendData((*it)->getFD(), message);
	}
}

void	Server::SendToNick(const Client *sender, const std::string nick, const std::string message) const
{
    std::map<std::string, Client *>::const_iterator it = _ClientsByNick.find(nick);
    if (it != _ClientsByNick.end())
	{
		sendData(it->second->getFD(), message);
    }
	else
	{
		SendToClient(sender, Builder::ErrNoSuchNick(sender->getNick(), nick));
	}
}

// Avoids doubles when user present on different channels
void	Server::SendToAllChannels(const Client *sender, const std::string message)
{
	std::vector<Client *>	list;
	
	std::vector <Channel *> channels = sender->getChannels();
	if (channels.size() == 0)
		return ;

	for (std::vector<Channel *>::const_iterator it = channels.begin(); it != channels.end(); it++)
	{
		std::vector<Client *> cli = (*it)->getClients();
		if (cli.size() == 0)
			continue ;
		for (std::vector<Client *>::const_iterator cl = cli.begin(); cl != cli.end(); cl++)
		{
			if (sender != *cl && std::find(list.begin(), list.end(), *cl) == list.end())
				list.push_back(*cl);
		}
	}
	SendToGroup(list, message);
}

void	Server::SendToClient(const Client *client, const std::string message) const
{
	sendData(client->getFD(), message);
}
