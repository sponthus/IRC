/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:49:23 by sponthus          #+#    #+#             */
/*   Updated: 2025/04/18 15:58:04 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Timer.hpp"

Timer::Timer() : _isRunning(false)
{

}

Timer::~Timer()
{

}


void Timer::startTimer()
{
	_isRunning = true;
	gettimeofday(&_startTime, 0);
}

void Timer::stopTimer()
{
	_isRunning = false;
	gettimeofday(&_endTime, 0);
}

void Timer::getCurrentTime(struct timeval& result) const
{
	gettimeofday(&result, 0);
}

double Timer::getElapsedSeconds() const
{
	if (_isRunning)
	{
		struct timeval currentTime;
		getCurrentTime(currentTime);
		double seconds = currentTime.tv_sec - _startTime.tv_sec;
		double microseconds = currentTime.tv_usec - _startTime.tv_usec;
		double time = seconds + microseconds / 1000000.0;
		return (time);
	}

	double seconds = _endTime.tv_sec - _startTime.tv_sec;
	double microseconds = _endTime.tv_usec - _startTime.tv_usec;
	return (seconds + microseconds / 1000000.0);
}

void	Timer::resetTimer()
{
	_startTime.tv_sec = 0;
	_startTime.tv_usec = 0;
	_endTime.tv_sec = 0;
	_endTime.tv_usec = 0;
	_isRunning = false;
}

std::string	Timer::getTimestamp(void)
{
	std::time_t	timestamp = std::time(NULL);
	std::tm *now = std::localtime(&timestamp);
	char buff[24];
	
	std::strftime(buff, sizeof(buff), "%Y/%m/%d at %Hh%Mm%Ss", now);
	std::string time(buff);
	return time;
}