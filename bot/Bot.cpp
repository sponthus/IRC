/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:20:23 by sponthus          #+#    #+#             */
/*   Updated: 2025/04/17 17:16:19 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

extern bool	g_shutdown;

// Not usable
Bot::Bot()
{
}

Bot::Bot(const int port, const char *serverIp, std::string pw) : _pw(pw)
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

void	Bot::run()
{
	char buffer[512];
	memset(buffer, 0, sizeof(buffer));
	int bytes_received = recv(this->_socket, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received <= 0) {
		std::cout << RED << "You have been disconnected by the server" << RESET << std::endl;
		g_shutdown = true;
		return ;
	}

	std::string msg(buffer);
	std::cout << msg;

	// Handle answers : 001 or error of log
	// Then create his own channel
	// And send questions to every new person in the channel, and with a timer
	// Counts score ?
	
	if (msg.find("PING") == 0) {
		std::string pong = "PONG" + msg.substr(4);
		send(this->_socket, pong.c_str(), pong.length(), 0);
	}
}
