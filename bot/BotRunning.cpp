/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotRunning.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:20:23 by sponthus          #+#    #+#             */
/*   Updated: 2025/04/18 16:59:18 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

extern bool	g_shutdown;

void	Bot::run()
{
	int ret = poll(&_pfd, 1, 1000);
	if (g_shutdown == true)
		return ;
	if (ret == -1)
	{
		g_shutdown = true;
		throw std::runtime_error("poll error");
	}
	else if (ret > 0)
	{
		std::string msg = recieveData(_message);
		if (g_shutdown)
			return;
		if (messageIsFull(&msg))
		{
			std::cout << BLUE << ">>" << msg << RESET;
			handleMessage(msg);
		}
	}
	if (_ready && _nbPlayers != 0)
		quizz();
}

void	Bot::log()
{
	std::string cmd = "PASS " + this->_pw + "\r\n" \
		+ "NICK " + NICK + "\r\n" \
		+ "USER " + USER + " 0 * IRCbot";

	sendData(cmd);
}

std::string Bot::whoSentThis(std::string msg)
{
	size_t prefix = msg.find_first_of(":");
	size_t end_prefix = msg.find_first_of("!");
	std::string sender = msg.substr(prefix + 1, end_prefix - prefix - 2);
	return (sender);
}

size_t	Bot::getQuestionId()
{
	size_t max = _questions[_actualTheme]->getNbQuestions();
	size_t nb = 0;
	if (max > 0)
		nb = (rand() % max) + 1;
	return (nb);
}

void	Bot::askQuestion()
{
	sendToChan(ASK + _actualTheme + " - " + _questions[_actualTheme]->getQuestion(_actualId - 1));
}

void	Bot::nextQuestion()
{
	_timer.stopTimer();
	_actualId = getQuestionId();
	_actualId *= -1;
	_timer.resetTimer();
	_timer.startTimer();
}

void	Bot::winner(std::string winner)
{
	sendToChan(WINMSG + winner + ANSWER + _questions[_actualTheme]->getFirstAnswer(_actualId - 1));
	nextQuestion();
}

void	Bot::handleMessage(std::string msg)
{	
	// Handle answers : 001 or error of log
	// Then create his own channel
	// And send questions to every new person in the channel, and with a timer
	// Counts score ?
	
	if (_ready == false)
		handleMessageConnexion(msg);
	else
	{
		if (msg.find("QUIT") != std::string::npos || msg.find("PART") != std::string::npos)
		{
			if (_nbPlayers >= 1)
				_nbPlayers -= 1;
			if (_nbPlayers == 0)
			{
				_actualId = 0;
				_timer.resetTimer();
			}
			std::cout << GREEN << _nbPlayers << " player(s) available to play ..." << RESET << std::endl;
			return ;
		}
		if (msg.find("JOIN") != std::string::npos)
		{
			_nbPlayers += 1;
			std::cout << GREEN << _nbPlayers << " player(s) available to play ..." << RESET << std::endl;
			return ;
		}
		if (_actualId > 0) // A question was asked
		{
			if (isAnswerToActualQuestion(msg))
			{
				std::string sender = whoSentThis(msg);
				winner(sender);
			}
		}
	}
	if (msg.find("PING") == 0) {
		std::string pong = "PONG" + msg.substr(4);
		send(this->_socket, pong.c_str(), pong.length(), 0);
	}
}

bool Bot::isAnswerToActualQuestion(std::string msg)
{
	std::vector<std::string>	answers = _questions[_actualTheme]->getAnswers(_actualId - 1);
	std::string	upperMsg = msg;
	std::transform(upperMsg.begin(), upperMsg.end(), upperMsg.begin(), ::toupper);
	std::vector<std::string>::const_iterator it;
	
	for (it = answers.begin(); it != answers.end(); it++)
	{
		if (upperMsg.find(*it) != std::string::npos)
		{
			return (true);
		}
	}
	return (false);
}

void	Bot::quizz()
{
	if (_nbPlayers == 0)
	{
		usleep(1000);
		return ;
	}
	else if (_actualId == 0) // Beginning
	{
		if (_actualTheme.size() == 0)
		{
			_actualTheme = "Capitals"; // TODO theme aleatoire
			std::cout << GREEN << "I have " << _questions[_actualTheme]->getNbQuestions() << " questions" << RESET << std::endl;
		}
		_actualId = getQuestionId();
		askQuestion();
	}
	else
	{
		if (_actualId > 0 && _timer.getElapsedSeconds() >= 20.0)
		{
			sendToChan(LATE + std::string(ANSWER) + _questions[_actualTheme]->getFirstAnswer(_actualId - 1));
			nextQuestion();
		}
		if (_actualId < 0 && _timer.getElapsedSeconds() >= 3.0)
		{
			_actualId *= -1;
			askQuestion();
		}
	}
}

void	Bot::handleMessageConnexion(std::string msg)
{
	if (msg.find(SERVER + std::string("464")) != std::string::npos) {
		std::cout << RED << ERROR << NICK << " has the wrong password for the server !" << std::endl;
		g_shutdown = true;
		return ;
	}
	else if (msg.find(SERVER + std::string("433")) != std::string::npos) {
		std::cout << RED << ERROR << NICK << " is already connected !" << std::endl;
		g_shutdown = true;
		return ;
	}
	else if (msg.find(SERVER + std::string("001")) != std::string::npos) {
		std::cout << GREEN << NICK << " is connected !" << std::endl;
		std::string cmd = "JOIN " + std::string(CHANNEL);
		sendData(cmd);
		return ;
	}
	else if (msg.find(SERVER + std::string("476")) != std::string::npos \
		|| msg.find(SERVER + std::string("475")) != std::string::npos \
		|| msg.find(SERVER + std::string("473")) != std::string::npos \
		|| msg.find(SERVER + std::string("471")) != std::string::npos) {
		std::cout << RED << ERROR << "The channel is invalid (probably already occupied): " << CHANNEL << std::endl;
		g_shutdown = true;
		return ;
	}
	else if (msg.find(SERVER + std::string("353")) != std::string::npos) {
		size_t sep = msg.find_last_of(":");
		std::string players = msg.substr(sep, msg.size() - sep - 1);
		int count = 0;
		for (size_t i = 0; (i = players.find(' ', i)) != std::string::npos; i++) {
			count++;
		}
		_nbPlayers = count - 1;
		std::cout << GREEN << _nbPlayers << " player(s) available to play ..." << RESET << std::endl;
		parseQuestions("capitals.txt");
		_ready = true;
		return ;
	}
}