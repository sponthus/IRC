/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:54:35 by sponthus          #+#    #+#             */
/*   Updated: 2025/02/06 14:11:37 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMER_HPP
# define TIMER_HPP

#include <sys/time.h>
#include <iostream>
#include <string>

class Timer {

public:
	~Timer();
	Timer();

	void start();
	void stop();
	void reset();
	double	getElapsedMicroseconds() const;
	
private:
	struct timeval startTime, endTime;
	bool isRunning;

	double	getElapsedMilliseconds() const;
	double	getElapsedSeconds() const;
	void	getCurrentTime(struct timeval& result) const;
};

#endif