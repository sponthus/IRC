/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:50:08 by sponthus          #+#    #+#             */
/*   Updated: 2025/04/18 15:10:28 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMER_HPP
# define TIMER_HPP

#include <sys/time.h>
#include <string>
#include <ctime>

class Timer {
	public: 
		Timer();
		~Timer();

		void	startTimer();
		void	stopTimer();
		void	resetTimer();
		double	getElapsedSeconds() const;
		void	getCurrentTime(struct timeval& result) const;
		std::string	getTimestamp(void);

	private:
		bool	_isRunning;
		struct timeval		_startTime, _endTime;
};

#endif