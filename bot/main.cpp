/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 14:51:46 by sponthus          #+#    #+#             */
/*   Updated: 2025/05/08 13:41:17 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

bool	g_shutdown = false;
pthread_mutex_t	shutdown_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t	write_mutex = PTHREAD_MUTEX_INITIALIZER;

void	WriteMessage(bool error, std::string color, std::string message)
{
	if (message.size() >= 2 && message[message.size() - 2] == '\r' && message[message.size() - 1] == '\n')
		message.erase(message.size() - 2);
	pthread_mutex_lock(&write_mutex);
	if (error)
		std::cerr << color << message << RESET << std::endl;
	else
		std::cout << color << message << RESET << std::endl;
	pthread_mutex_unlock(&write_mutex);
}

void	WriteMessage(bool error, std::string color, int value, std::string message)
{
	if (message.size() >= 2 && message[message.size() - 2] == '\r' && message[message.size() - 1] == '\n')
		message.erase(message.size() - 2);
	pthread_mutex_lock(&write_mutex);
	if (error)
		std::cerr << color << value << message << RESET << std::endl;
	else
		std::cout << color << value << message << RESET << std::endl;
	pthread_mutex_unlock(&write_mutex);
}

void	sig(int signal)
{
	(void)signal;
	std::cout << std::endl;
	setShutdown(true);
}

void	setShutdown(bool value)
{
	pthread_mutex_lock(&shutdown_mutex);
	g_shutdown = value;
	pthread_mutex_unlock(&shutdown_mutex);
}

bool	isShutdown()
{
	pthread_mutex_lock(&shutdown_mutex);
	bool value = g_shutdown;
	pthread_mutex_unlock(&shutdown_mutex);
	return value;
}

void*	inputListener(void* arg)
{	
	Bot* bot = static_cast<Bot*>(arg);
	fd_set fds; // Surveillance structure of a fd set
	
	struct timeval timeout;
	timeout.tv_sec = 1; // Wait 1s
	timeout.tv_usec = 0;
	
	while (!isShutdown())
	{
		FD_ZERO(&fds); // Zero initialization of fds
		FD_SET(STDIN_FILENO, &fds); // Put stdin in fds
		int ret = select(STDIN_FILENO + 1, &fds, NULL, NULL, &timeout); // options = max fd observed, fd_set - Monitors STDIN status
		if (ret > 0 && FD_ISSET(STDIN_FILENO, &fds)) // STDIN is ready
		{
			std::string input;
			if (!std::getline(std::cin, input)) // EOF detected
			{
				setShutdown(true);
				break;
			}
			if (!input.empty() && bot->CheckInput(input))
				bot->handleInput(input);
		}
		else if (ret < 0) // STDIN has a problem
		{
			setShutdown(true);
			break;
		}
	}
	return NULL;
}

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << RED << ERROR << ERR_NB_ARG << RESET << std::endl;
		return 1;
	}
	if (!isValidPort(argv[1]) || !isValidPW(argv[2]))
		return 1;

	const char *serverIp = SERVER_IP;
	const int port = std::atoi(argv[1]);
	pthread_t inputThread;
	bool inputThreadStarted = false;

	try
	{
		std::cout << BOLDYELLOW << " ---------- Bot trying to connect ---------- " << RESET << std::endl;
		Bot	bot(port, serverIp, argv[2]);
		std::cout << BOLDYELLOW << " --------- Bot connected to server --------- " << RESET << std::endl;
		bot.log();

		signal(SIGINT, sig);
		signal(SIGQUIT, sig);
		
		if (pthread_create(&inputThread, NULL, inputListener, &bot) != 0)
			throw std::runtime_error("Error with thread creation");
		inputThreadStarted = true;

		while (!isShutdown())
		{
			bot.run();
		}

		if (inputThreadStarted)
			pthread_join(inputThread, NULL);
	}
	catch (const std::exception &e) {
		if (inputThreadStarted)
			pthread_join(inputThread, NULL);
		WriteMessage(true, RED, ERROR + std::string(" ") + e.what());
	}
	std::cout << BOLDYELLOW << " ------------- Bot closed ------------- " << RESET << std::endl;
}