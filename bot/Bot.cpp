/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:20:23 by sponthus          #+#    #+#             */
/*   Updated: 2025/04/17 18:48:58 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

extern bool	g_shutdown;

// Not usable
Bot::Bot()
{
}

Bot::Bot(const int port, const char *serverIp, std::string pw) : _pw(pw), _message(""), _ready(false)
{
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket < 0)
		throw std::runtime_error("socket creation failed");

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET, serverIp, &server_addr.sin_addr); // convert IP addresses from text to binary form

	if (connect(this->_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		std::cerr << RED << ERROR << "Is the server online at " << serverIp << ":" << port << "?" << RESET << std::endl; 
        close(this->_socket);
		throw std::runtime_error("connection failed");
    }
		
}

Bot::~Bot()
{
	shutdown(this->_socket, SHUT_RDWR);
	close(this->_socket);
}

void	Bot::log()
{
	std::string cmd = "PASS " + this->_pw + "\r\n" \
		+ "NICK " + NICK + "\r\n" \
		+ "USER " + USER + " 0 * IRCbot\r\n";

	send(this->_socket, cmd.c_str(), cmd.length(), 0);
}

std::string	Bot::recieveData(std::string message)
{
	char buffer[BUFF_SIZE];
	memset(buffer, 0, sizeof(buffer));
	int size = recv(this->_socket, buffer, sizeof(buffer) - 1, 0);
	if (size <= 0) {
		std::cout << RED << "You have been disconnected by the server" << RESET << std::endl;
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

void	Bot::run()
{
	std::string msg = recieveData(_message);
	if (messageIsFull(&msg) && !g_shutdown)
	{
		handleMessage(msg);
	}
	if (_ready)
		quizz();
}

void	Bot::handleMessage(std::string msg)
{
	std::cout << BLUE << msg << RESET;
	std::cout << SERVER_PREFIX + std::string("476") << std::endl;
	// Handle answers : 001 or error of log
	// Then create his own channel
	// And send questions to every new person in the channel, and with a timer
	// Counts score ?

	if (_ready == false)
	{
		if (msg.find(SERVER_PREFIX + std::string("464")) == 0) {
			std::cout << RED << ERROR << "Bot has the wrong password !" << std::endl;
			g_shutdown = true;
			return ;
		}
		else if (msg.find(SERVER_PREFIX + std::string("433")) == 0) {
			std::cout << RED << ERROR << "Bot is already connected !" << std::endl;
			g_shutdown = true;
			return ;
		}
		else if (msg.find(SERVER_PREFIX + std::string("001")) == 0) {
			std::cout << GREEN << "I am connected !" << std::endl;
			std::string cmd = "JOIN " + std::string(CHANNEL);
			send(this->_socket, cmd.c_str(), cmd.length(), 0);
			return ;
		}
		else if (msg.find(SERVER_PREFIX + std::string("476")) \
		|| msg.find(SERVER_PREFIX + std::string("475")) \
		|| msg.find(SERVER_PREFIX + std::string("473")) \
		|| msg.find(SERVER_PREFIX + std::string("471"))) // TODO = Rentre toujours ici ?
		{
			std::cout << RED << ERROR << "The channel is invalid (probably already occupied): " << CHANNEL << std::endl;
			g_shutdown = true;
			return ;
		}
		else if (msg.find(SERVER_PREFIX + std::string("353 ") + std::string(NICK) + " = " + std::string(CHANNEL) + " :@" + std::string(NICK)))
		{
			_ready = true;
			return ;
		}
	}

	else if (msg.find("PING") == 0) {
		std::string pong = "PONG" + msg.substr(4);
		send(this->_socket, pong.c_str(), pong.length(), 0);
	}
}

void	Bot::quizz()
{
	std::cout << "I am quizzing" << std::endl;
}