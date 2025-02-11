/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:56:15 by sponthus          #+#    #+#             */
/*   Updated: 2025/02/11 11:07:38 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Timer.hpp"

Timer::Timer() : isRunning(false)
{
	startTime.tv_sec = 0;
	startTime.tv_usec = 0;
	endTime.tv_sec = 0;
	endTime.tv_usec = 0;
}

Timer::~Timer() {}

void Timer::start()
{
	isRunning = true;
	gettimeofday(&startTime, 0);
}

void Timer::stop()
{
	isRunning = false;
	gettimeofday(&endTime, 0);
}

void Timer::getCurrentTime(struct timeval& result) const
{
	gettimeofday(&result, 0);
}

double	Timer::getUnixTimeSeconds() const
{
	struct timeval currentTime;
	getCurrentTime(currentTime);
	double seconds = currentTime.tv_sec;
	return seconds;
}

double Timer::getElapsedMicroseconds() const
{
	if (isRunning)
	{
		struct timeval currentTime;
		getCurrentTime(currentTime);
		double seconds = currentTime.tv_sec - startTime.tv_sec;
		double microseconds = currentTime.tv_usec - startTime.tv_usec;
		return (seconds * 1000000.0 + microseconds);
	}
	
	// Calcul de la différence de temps
	double seconds = endTime.tv_sec - startTime.tv_sec;
	double microseconds = endTime.tv_usec - startTime.tv_usec;
	
	// Conversion en microsecondes
	return seconds * 1000000 + microseconds;
}

double Timer::getElapsedMilliseconds() const
{
	return getElapsedMicroseconds() / 1000.0;
}

double Timer::getElapsedSeconds() const
{
	return getElapsedMicroseconds() / 1000000.0;
}

void Timer::reset()
{
	isRunning = false;
}