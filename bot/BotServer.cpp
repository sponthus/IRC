/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:21:21 by sponthus          #+#    #+#             */
/*   Updated: 2025/04/22 14:59:46 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

// Not usable
Bot::Bot()
{
}

Bot::Bot(const int port, const char *serverIp, std::string pw) : _pw(pw), _message(""), _ready(false), _nbPlayers(0), _actualId(0), _actualTheme("")
{
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket < 0)
		throw std::runtime_error("socket creation failed");

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET, serverIp, &server_addr.sin_addr); // convert IP addresses from text to binary form

	if (connect(this->_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		std::cerr << RED << ERROR << "Is the server online at " << serverIp << "::" << port << "?" << RESET << std::endl; 
        close(this->_socket);
		throw std::runtime_error("connection failed");
    }

	_pfd.fd = this->_socket;
	_pfd.events = POLLIN;
	_pfd.revents = 0;

	_timer.stopTimer();
	_timer.resetTimer();

	srand(time(NULL));
}

Bot::~Bot()
{
	shutdown(this->_socket, SHUT_RDWR);
	close(this->_socket);

	for (std::map<std::string, Questions*>::iterator it = _questions.begin(); it != _questions.end(); ++it)
    {
        if (it->second)
			delete it->second;
    }
	_questions.clear();
}

void	Bot::sendData(std::string message) const
{
	message += "\r\n";
	std::cout << MAGENTA << "<<" << message << RESET;
	send(this->_socket, message.c_str(), message.length(), 0);
}

void	Bot::sendToChan(std::string message) const
{
	std::string result = CHANMSG + message;
	result += "\r\n";
	std::cout << MAGENTA << "<<" << result << RESET;
	send(this->_socket, result.c_str(), result.length(), 0);
}

std::string	Bot::recieveData(std::string message)
{
	char buffer[BUFF_SIZE];
	memset(buffer, 0, sizeof(buffer));
	int size = recv(this->_socket, buffer, sizeof(buffer) - 1, 0);
	if (size <= 0) {
		std::cout << RED << NICK << " has been disconnected by the server" << RESET << std::endl;
		g_shutdown = true;
		return ("");
	}
	else
	{
		buffer[size] = '\0';
		if (size > 0)
			message += buffer;
		if (size + 1 == BUFF_SIZE)
			message += this->recieveData(message);
	}
	return (message);
}

bool	Bot::messageIsFull(std::string *message)
{
	if (message->size() == 0)
		return (false);
	_message += *message;
	size_t pos = _message.find_last_of("\r\n");
	if (pos == std::string::npos) // Data does not contain \r\n
		return (false);
	if (pos != _message.size() - 1) // Data doesn't end with /r/n but contains it
	{
		*message = _message.substr(0, pos);
		std::string keep = _message.substr(pos + 2, _message.size() - pos - 2);
		_message = keep;
	}
	else // Data ends with /r/n
	{
		*message = _message;
		this->_message = std::string("");
	}
	return (true);
}

void	Bot::parseQuestions(std::string filename)
{
	try
	{
		Questions *qa = new Questions(filename);
		std::string theme = qa->getTheme();
		if (_questions.find(theme) != _questions.end())
		{
			throw (std::invalid_argument(NICK + std::string(" already knows questions about ") + theme));
		}
		_questions[theme] = qa;
		std::cout << GREEN << "Learned questions about " << qa->getTheme() << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << RED << ERROR << e.what() << RESET << std::endl;
	}
}