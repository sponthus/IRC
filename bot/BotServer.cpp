/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:21:21 by sponthus          #+#    #+#             */
/*   Updated: 2025/05/08 13:23:15 by sponthus         ###   ########.fr       */
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
	_timer.startTimer();

	srand(time(NULL));

	_questionsMutex = PTHREAD_MUTEX_INITIALIZER;
}

Bot::~Bot()
{
	shutdown(this->_socket, SHUT_RDWR);
	close(this->_socket);

	pthread_mutex_lock(&_questionsMutex);
	for (std::map<std::string, Questions*>::iterator it = _questions.begin(); it != _questions.end(); ++it)
    {
        if (it->second)
			delete it->second;
    }
	_questions.clear();
	pthread_mutex_unlock(&_questionsMutex);
}

bool	Bot::CheckInput(std::string input)
{
	if (input.empty())
		return (false);
	if (input.rfind("LEARN ", 0) != 0)
		return (false);
	return (true);
}

void	Bot::handleInput(std::string input)
{
	if (input.length() < 7)
	{
		WriteMessage(true, RED, "No infile given, use: LEARN [infile]");
		return ;
	}
	std::string infile = input.substr(6, input.length() - 6);
	parseQuestions(infile);
}

void	Bot::sendData(std::string message) const
{
	message += "\r\n";
	WriteMessage(false, MAGENTA, "<< " + message);
	send(this->_socket, message.c_str(), message.length(), 0);
}

void	Bot::sendToChan(std::string message) const
{
	std::string result = CHANMSG + message;
	result += "\r\n";
	WriteMessage(false, MAGENTA, "<< " + result);
	send(this->_socket, result.c_str(), result.length(), 0);
}

std::string	Bot::recieveData(std::string message)
{
	char buffer[BUFF_SIZE];
	memset(buffer, 0, sizeof(buffer));
	int size = recv(this->_socket, buffer, sizeof(buffer) - 1, 0);
	if (size <= 0) {
		WriteMessage(false, RED, NICK + std::string(" has been disconnected by the server"));
		setShutdown(true);
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

std::vector<std::string> Bot::splitMessages(const std::string& raw)
{
	std::vector<std::string> messages;
	size_t start = 0, end;

	while ((end = raw.find("\r\n", start)) != std::string::npos)
	{
		messages.push_back(raw.substr(start, end - start));
		start = end + 2;
	}
	return messages;
}

void	Bot::parseQuestions(std::string filename)
{
	try
	{
		Questions *qa = new Questions(filename);
		std::string theme = qa->getTheme();
		pthread_mutex_lock(&_questionsMutex);
		if (_questions.find(theme) != _questions.end())
		{
			delete qa;
			pthread_mutex_unlock(&_questionsMutex);
			throw (std::invalid_argument(NICK + std::string(" already knows questions about ") + theme));
		}
		_questions[theme] = qa;
		pthread_mutex_unlock(&_questionsMutex);
		WriteMessage(false, GREEN, "Learned questions about " +  qa->getTheme());
	}
	catch (const std::exception &e)
	{
		WriteMessage(true, RED, ERROR + std::string(e.what()));
	}
}