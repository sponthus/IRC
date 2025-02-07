/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 16:01:59 by sponthus          #+#    #+#             */
/*   Updated: 2025/02/07 16:07:27 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Builder.hpp"

Message Builder::_message;

Builder::Builder()
{}

Builder& Builder::setPrefix(const std::string& prefix)
{
	_message.setPrefix(prefix);
	return getInstance();
}

Builder& Builder::setCode(const std::string& code)
{
	_message.setCode(code);
	return getInstance();
}

Builder& Builder::setContent(const std::string& content)
{
	_message.setContent(content);
	return getInstance();
}

Builder& Builder::setSuffix(const std::string& suffix)
{
	_message.setSuffix(suffix);
	return getInstance();
}

Builder& Builder::create()
{
	_message.clear();
	return getInstance();
}

Builder& Builder::getInstance()
{
	static Builder instance;
	return instance;
}

Message Builder::build()
{
	Message result = _message;
	_message.clear();
	return result;
}

// 401 ERR_NOSUCHNICK 
// ":<server> <targetNick> <invalidNick> :No such nick/channel" 
std::string Builder::buildErrorNoSuchNick(const std::string& targetNick,
											const std::string& invalidNick)
{
	return create()
		.setPrefix(SERVER)
		.setCode("401")
		.setContent(targetNick + " " + invalidNick)
		.setSuffix("No such nick/channel")
		.build()
		.toString();
}

// 433 ERR_NICKNAMEINUSE
// ":<server> <clientID> <existingNick> :Nickname is already in use"
// <clientID> is often "*" in this case (undefined)
std::string Builder::buildNickInUse(const std::string& clientID, const std::string& existingNick)
{
	return create()
		.setPrefix(SERVER)
		.setCode("433")
		.setContent(clientID + " " + existingNick)
		.setSuffix("Nickname is already in use")
		.build()
		.toString();
}

std::string Builder::buildPing()
{
	return create()
		.setPrefix(SERVER)
		.setCode("PING")
		.build()
		.toString();
}

std::string Builder::buildPrivMsg(const std::string& sender, const std::string& target, const std::string& message)
{
	return create()
		.setPrefix(sender)
		.setCode("PRIVMSG")
		.setContent(target)
		.setSuffix(message)
		.build()
		.toString();
}