/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Questions.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:03:19 by sponthus          #+#    #+#             */
/*   Updated: 2025/05/07 13:55:17 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUESTIONS_HPP
# define QUESTIONS_HPP

#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>

#include "Colors.hpp"

class Questions {
	public:
		Questions(std::string filename);
		~Questions();

		std::string	getTheme() const;
		void		parseLine(std::string line);
		void		parseFile(std::string filename);
		int			getNbQuestions() const;
		std::string					getQuestion(size_t id) const;
		std::vector<std::string>	getAnswers(size_t id) const;
		std::string	getFirstAnswer(size_t id) const;

	private:
		std::map<std::string, std::vector<std::string> >	_qa;
		std::string							_theme;
};

void	WriteMessage(bool error, std::string color, std::string message);
void	WriteMessage(bool error, std::string color, int value, std::string message);

# endif