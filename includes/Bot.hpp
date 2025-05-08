/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 14:58:43 by sponthus          #+#    #+#             */
/*   Updated: 2025/05/08 13:11:03 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

#include <iostream>
#include <string>
#include <map>
#include <signal.h>
#include <vector>
#include <poll.h>
#include <sys/socket.h> // socket(), address
#include <netinet/in.h> // IP specialties 
#include <netdb.h>
#include <arpa/inet.h> // inet_pton
#include <cstdlib> // atoi()
#include <cstring> // memset
#include <fcntl.h>
#include <unistd.h> // usleep
#include <algorithm> // toupper
#include <iomanip> // getline
#include <cstdlib>
#include <cstdio>
#include <pthread.h> // Thread
#include <sstream>

#include "Colors.hpp"
#include "Timer.hpp"
#include "Questions.hpp"

# define NICK "QuestBot"
# define USER "QuestBot"
# define CHANNEL "#Quizz"
# define SERVER "irc.server.42 "

// Messages
# define CHANMSG "PRIVMSG #Quizz :"
# define WINMSG "Congrats, you won "
# define ASK "-> Question about "
# define LATE "It's too late, maybe next time !"
# define ANSWER ", the answer was "
# define SERVER_IP "127.0.0.1"
# define QUIT "QUIT :bye bye!"

// Error messages
# define ERROR "Error :"
# define ERR_NB_ARG "Wrong number of arguments, expected 2 :<port> <password>"
# define ERR_PORT_NUM "Port value should be numeric"
# define ERR_PORT_VAL "Valid port should be between 1024 and 65535"
# define ERR_PW "Password should not contain spaces, \' or \""
# define ERR_CHANN " The channel is invalid (probably already occupied) "
# define ERR_CHANN_PW " Wrong password for the server !"
# define ERR_ALREADY_CONN " A bot is already connected !"

// Variables
# define BUFF_SIZE 512
# define MAX_PORT 65535
# define TIME_TO_ANSWER 20.0
# define TIME_TO_WAIT 3.0
# define TIME_TO_DIE 30.0

extern bool g_shutdown;

class Bot {
	public:
		Bot(const int port, const char *serverIp, std::string pw);
		~Bot();

		int		getPort() const;
		void	log();
		bool	CheckInput(std::string input);
		void	parseQuestions(std::string file);
		void	handleInput(std::string input);
		void	run();
		void	quizz();
		void	handleMessageConnexion(std::string msg);
		void	handleMessage(std::string message);
		bool	messageIsFull(std::string *message);
		std::vector<std::string>	splitMessages(const std::string& raw);
		std::string recieveData(std::string message);
		void	sendToChan(std::string message) const;
		void	sendData(std::string message) const;
		bool	isAnswerToActualQuestion(std::string msg);
		std::string whoSentThis(std::string message);
		void	winner(std::string winner);
		void	nextQuestion();
		void	askQuestion();
		size_t	getQuestionId();

	private:
		Bot(); // Not usable

		std::string		_pw;
		int				_socket;
		struct pollfd 	_pfd;
		std::string		_message;
		bool			_ready;
		std::map<std::string, Questions *>	_questions;
		Timer			_timer;
		int				_nbPlayers;
		int				_actualId;
		std::string		_actualTheme;
		pthread_mutex_t	_questionsMutex;
};


bool	isValidPW(std::string arg);
bool	isValidPort(std::string arg);
bool	isCode(std::string arg);
void	setShutdown(bool value);
bool	isShutdown();
void	WriteMessage(bool error, std::string color, std::string message);
void	WriteMessage(bool error, std::string color, int value, std::string message);

# endif