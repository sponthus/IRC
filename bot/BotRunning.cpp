/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotRunning.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:20:23 by sponthus          #+#    #+#             */
/*   Updated: 2025/05/07 17:52:47 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void	Bot::run()
{
	int ret = poll(&_pfd, 1, 1000);
	if (isShutdown())
	{
		if (ret > 0)
			sendData(QUIT);
		return ;
	}
	if (ret == -1)
	{
		setShutdown(true);
		throw std::runtime_error("poll error");
	}
	else if (ret > 0)
	{
		std::string data = recieveData(_message);
		if (isShutdown())
			return;
		if (messageIsFull(&data))
		{
			std::vector<std::string> messages = splitMessages(data);
			for (size_t i = 0; i < messages.size() && !isShutdown(); ++i)
			{
				WriteMessage(false, BLUE, ">> " + messages[i]);
				handleMessage(messages[i]);
			}
		}
	}
	if (_ready && _nbPlayers != 0)
	{
		quizz();
	}
	else if (!_ready && _timer.getElapsedSeconds() >= TIME_TO_DIE)
	{
		setShutdown(true);
		throw std::runtime_error("Unable to connect to the server");
	}
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
	std::string sender = msg.substr(prefix + 1, end_prefix - prefix - 1);
	return (sender);
}

size_t	Bot::getQuestionId()
{
	pthread_mutex_lock(&_questionsMutex);
	size_t max = _questions[_actualTheme]->getNbQuestions();
	pthread_mutex_unlock(&_questionsMutex);
	size_t nb = 0;
	if (max > 0)
		nb = (rand() % max) + 1;
	return (nb);
}

void	Bot::askQuestion()
{
	pthread_mutex_lock(&_questionsMutex);
	sendToChan(ASK + _actualTheme + " - " + _questions[_actualTheme]->getQuestion(_actualId - 1));
	pthread_mutex_unlock(&_questionsMutex);
}

void	Bot::nextQuestion()
{
	_timer.stopTimer();
	pthread_mutex_lock(&_questionsMutex);
	if (_questions.size() > 1)
	{
		if (rand() % 2 == 0) //  1/10 chances to change the theme
		{
			usleep(1000000);
			std::string newTheme = _actualTheme;
			while (_questions.size() > 1 && newTheme == _actualTheme) // Don't take the same theme
			{
				std::map<std::string, Questions*>::iterator it = _questions.begin();
				std::advance(it, rand() % _questions.size());
				newTheme = it->first;
			}
			_actualTheme = newTheme;
			sendToChan("Change of theme, " + _actualTheme);
		}
	}
	int	nbQuestions = _questions[_actualTheme]->getNbQuestions();
	pthread_mutex_unlock(&_questionsMutex);
	int	oldId = _actualId;
	while (nbQuestions > 1 && _actualId == oldId)
	{
		_actualId = getQuestionId();
	}
	_actualId *= -1;
	_timer.resetTimer();
	_timer.startTimer();
}

void	Bot::winner(std::string winner)
{
	pthread_mutex_lock(&_questionsMutex);
	sendToChan(WINMSG + winner + ANSWER + _questions[_actualTheme]->getFirstAnswer(_actualId - 1));
	pthread_mutex_unlock(&_questionsMutex);
	nextQuestion();
}

// Handle initial server answers : 001 or error of log
// Then create his own channel
// And send questions to every new person in the channel, and with a timer
void	Bot::handleMessage(std::string msg)
{	
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
			WriteMessage(false, GREEN, _nbPlayers, " player(s) available to play ...");
			return ;
		}
		if (msg.find("JOIN") != std::string::npos)
		{
			_nbPlayers += 1;
			WriteMessage(false, GREEN, _nbPlayers, " player(s) available to play ...");
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
	pthread_mutex_lock(&_questionsMutex);
	std::vector<std::string>	answers = _questions[_actualTheme]->getAnswers(_actualId - 1);
	pthread_mutex_unlock(&_questionsMutex);
	
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
	pthread_mutex_lock(&_questionsMutex);
	if (_nbPlayers == 0 || _questions.size() == 0) // No players or no questions known = No launch
	{
		pthread_mutex_unlock(&_questionsMutex);
		usleep(1000);
		return ;
	}
	pthread_mutex_unlock(&_questionsMutex);
	if (_actualId == 0) // Beginning
	{
		if (_actualTheme.size() == 0)
		{
			pthread_mutex_lock(&_questionsMutex);
			if (_questions.size() == 0)
			{
				pthread_mutex_unlock(&_questionsMutex);
				return ;
			}
			std::map<std::string, Questions *>::iterator it = _questions.begin();
			std::advance(it, rand() % _questions.size());
			_actualTheme = it->first;
			int nb = _questions[_actualTheme]->getNbQuestions();
			pthread_mutex_unlock(&_questionsMutex);
			std::ostringstream oss;
			oss << "Let's play! Actual theme is " << _actualTheme << ", " << nb << " questions available";
			sendToChan(oss.str());
		}
		_actualId = getQuestionId();
		askQuestion();
	}
	else
	{
		if (_actualId > 0 && _timer.getElapsedSeconds() >= TIME_TO_ANSWER)
		{
			pthread_mutex_lock(&_questionsMutex);
			sendToChan(LATE + std::string(ANSWER) + _questions[_actualTheme]->getFirstAnswer(_actualId - 1));
			pthread_mutex_unlock(&_questionsMutex);
			nextQuestion();
		}
		if (_actualId < 0 && _timer.getElapsedSeconds() >= TIME_TO_WAIT)
		{
			_actualId *= -1;
			askQuestion();
		}
	}
}

void	Bot::handleMessageConnexion(std::string msg)
{
	if (msg.find(SERVER) == std::string::npos || msg.size() < std::string(SERVER).size() + 4)
		return ;
	std::string code = msg.substr(std::string(SERVER).size() + 1, 3);
	if (!isCode(code))
		return ;
	int codeNb = atoi(code.c_str());
	switch (codeNb)
	{
		case 001 :
		{
			WriteMessage(false, GREEN, NICK + std::string(" is connected !"));
			std::string cmd = "JOIN " + std::string(CHANNEL);
			sendData(cmd);
			break ;
		}
		case 353 :
		{
			size_t sep = msg.find_last_of(":");
			std::string players = msg.substr(sep, msg.size() - sep - 1);
			int count = 0;
			for (size_t i = 0; (i = players.find(' ', i)) != std::string::npos; i++) {
				count++;
			}
			_nbPlayers = count;
			WriteMessage(false, GREEN, _nbPlayers, std::string(" player(s) available to play ..."));
			_ready = true;
			_timer.resetTimer();
			pthread_mutex_lock(&_questionsMutex);
			if (_questions.size() == 0)
				WriteMessage(false, GREEN, "Warning: I know no questions ...");
			pthread_mutex_unlock(&_questionsMutex);
			break ;
		};
		case 433 :
		{
			WriteMessage(true, RED, ERROR + std::string(ERR_ALREADY_CONN));
			setShutdown(true);
			break ;
		}
		case 464 :
			WriteMessage(true, RED, ERROR + std::string(ERR_CHANN_PW));
			setShutdown(true);
			break ;
		case 471 :
		{
			WriteMessage(true, RED, ERROR + std::string(ERR_CHANN) + CHANNEL);
			setShutdown(true);
			break ;
		}
		case 473 :
		{
			WriteMessage(true, RED, ERROR + std::string(ERR_CHANN) + CHANNEL);
			setShutdown(true);
			break ;
		}
		case 475 :
		{
			WriteMessage(true, RED, ERROR + std::string(ERR_CHANN) + CHANNEL);
			setShutdown(true);
			break ;
		}
		case 476 :
		{
			WriteMessage(true, RED, ERROR + std::string(ERR_CHANN) + CHANNEL);
			setShutdown(true);
			break ;
		}
	}
}