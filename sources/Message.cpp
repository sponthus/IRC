/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 16:42:57 by sponthus          #+#    #+#             */
/*   Updated: 2025/02/07 16:46:10 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

Message::Message() {}

// Variable format :
// :<prefix> <code> <content> :<suffix>
// <code> <content> :<suffix>
// <code> <content>
std::string Message::toString() const
{
	std::string result;
	if (!_prefix.empty()) {
		result += ":" + _prefix + " ";
	}
	result += _code + " " + _content;
	if (!_suffix.empty()) {
		result += " :" + _suffix;
	}
	return result;
}

void	Message::setPrefix(const std::string& content)
{
	_prefix = content;
}

void	Message::setCode(const std::string& content)
{
	_code = content;
}

void	Message::setContent(const std::string& content)
{
	_content = content;
}

void	Message::setSuffix(const std::string& content)
{
	_suffix = content;
}

void Message::clear()
{
	_prefix.clear();
	_code.clear();
	_content.clear();
	_suffix.clear();
}