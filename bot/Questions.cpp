/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Questions.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:08:51 by sponthus          #+#    #+#             */
/*   Updated: 2025/05/07 14:03:25 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Questions.hpp"

Questions::Questions(std::string filename) : _theme("")
{;
	parseFile(filename);
}

Questions::~Questions()
{

}

void	Questions::parseLine(std::string line)
{
	std::string					question, allAnswers;
	std::vector<std::string>	answers;
	std::string					answer;

	size_t	pipe = line.find_first_of("|");
	if (pipe == std::string::npos || pipe == line.size())
	{
		throw std::invalid_argument(std::string("Question should follow [question]|[answerA]/[answerB] format : ") + line);
	}
	question = line.substr(0, pipe);
	allAnswers = line.substr(pipe + 1, line.size() - pipe - 1);
	size_t sep = allAnswers.find_first_of("/");
	if (sep == std::string::npos)
	{
		answer = allAnswers;
		std::transform(answer.begin(), answer.end(), answer.begin(), ::toupper);
		answers.push_back(answer);
	}
	else
	{
		answer = allAnswers.substr(0, sep);
		std::transform(answer.begin(), answer.end(), answer.begin(), ::toupper);
		answers.push_back(answer);
		allAnswers = allAnswers.substr(sep + 1, allAnswers.size() - sep - 1);
		while (sep != std::string::npos)
		{
			sep = allAnswers.find_first_of("/");
			answer = allAnswers.substr(0, sep);
			std::transform(answer.begin(), answer.end(), answer.begin(), ::toupper);
			answers.push_back(answer);
			allAnswers = allAnswers.substr(sep + 1, allAnswers.size() - sep - 1);
		}
	}
	_qa[question] = answers;
}

void	Questions::parseFile(std::string filename)
{
	if (filename.empty())
		throw std::invalid_argument("filename for questions should not be empty");

	std::ifstream ifs(filename.c_str());
	if (!(ifs.good()))
		throw std::invalid_argument("could not open questions file : " + filename);
	
	std::string line;
	while (getline(ifs, line, '\n'))
	{
		if (_theme.empty())
		{
			size_t	pos = line.find("theme=");
			if (pos == std::string::npos || pos + 6 == line.size())
				throw std::invalid_argument(filename + " has no theme at first line (expected: theme=[theme])");
			else
				_theme = line.substr(pos + 6, line.size() - pos - 1);
		}
		else
			parseLine(line);
	}
}

std::string	Questions::getTheme() const
{
	return (this->_theme);
}

int	Questions::getNbQuestions() const
{
	return (_qa.size());
}

std::string	Questions::getQuestion(size_t id) const
{
	if (id >= _qa.size()) {
        WriteMessage(true, RED, "Invalid ID");
        return ("");
    }
	std::map<std::string, std::vector<std::string> >::const_iterator it = _qa.begin();
	std::advance(it, id);
	return (it->first);
}

std::vector<std::string>	Questions::getAnswers(size_t id) const
{
	if (id >= _qa.size()) {
        WriteMessage(true, RED, "Invalid ID");
        return (std::vector<std::string>());
    }
	std::map<std::string, std::vector<std::string> >::const_iterator it = _qa.begin();
	std::advance(it, id);
	return (it->second);
}

std::string	Questions::getFirstAnswer(size_t id) const
{
	if (id >= _qa.size()) {
        WriteMessage(true, RED, "Invalid ID");
        return ("");
    }
	std::map<std::string, std::vector<std::string> >::const_iterator it = _qa.begin();
	std::advance(it, id);
	return (it->second[0]);
}