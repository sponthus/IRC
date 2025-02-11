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

// Variable format :
// :<prefix> <code> <content> :<suffix>
// <code> <content> :<suffix>
// <code> <content>
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

//////////////////////// RESPONSES MESSAGES /////////////////////////

// 001 Welcome
// Response to a USER when logging in the server
// ":<server> 001 <nick> :Welcome to the IRC Network, <nick>!<user>@<host>"
std::string Builder::Welcome(const std::string& nick, const std::string&user)
{
	return create()
		.setPrefix(SERVER)
		.setCode("001")
		.setContent(nick)
		.setSuffix("Welcome to the IRC Network, " + nick + "!" + user + "@" + HOST)
		.build()
		.toString();
}

std::string Builder::Ping()
{
	return create()
		.setPrefix(SERVER)
		.setCode("PING")
		.build()
		.toString();
}

std::string Builder::PrivMsg(const std::string& sender, const std::string& target, const std::string& message)
{
	return create()
		.setPrefix(sender)
		.setCode("PRIVMSG")
		.setContent(target)
		.setSuffix(message)
		.build()
		.toString();
}

// User + every user sharing a canal with him see the change
// :<oldNick>!<user>@<host> NICK <newNick>
std::string Builder::Nick(const std::string& oldNick, const std::string& user, const std::string& newNick)
{
	return create()
		.setPrefix(oldNick + "!" + user + "@" + HOST)
		.setCode("NICK")
		.setContent(newNick)
		.build()
		.toString();
}


///////////////////////// ERROR MESSAGES /////////////////////////

// 263 RPL_TRYAGAIN
// "<command> :Please wait a while and try again."

// 324 RPL_CHANNELMODEIS
// "<channel> <mode> <mode params>"

// 331 RPL_NOTOPIC
// ":<server> 331 <nick> #<channel> :No topic is set"

// 332 RPL_TOPIC
// Answer to setting a topic : ":<nick> TOPIC #<channel> :<topic>"
// Answer to viewing a topic . ":<server> 332 <nick> #<channel> :<topic>"

// 341 RPL_INVITING
// "#<channel> <nick>"

// 401 ERR_NOSUCHNICK 
// ":<server> 401 <targetNick> <invalidNick> :No such nick/channel" 
std::string Builder::ErrNoSuchNick(const std::string& targetNick, const std::string& invalidNick)
{
	return create()
		.setPrefix(SERVER)
		.setCode("401")
		.setContent(targetNick + " " + invalidNick)
		.setSuffix("No such nick/channel")
		.build()
		.toString();
}

// 403 ERR_NOSUCHCHANNEL
// :<server> 403 <requestingNick> <channel> :No such channel
std::string Builder::ErrNoSuchChannel(const std::string& requestingNick, const std::string& channel)
{
	return create()
		.setPrefix(SERVER)
		.setCode("403")
		.setContent(requestingNick + " " + channel)
		.setSuffix("No such channel")
		.build()
		.toString();
}

// 404 ERR_CANNOTSENDTOCHAN ? TODO : check if needed
// ":<server> 404 <requestingNick> <channel> :Cannot send to channel"
std::string Builder::ErrCannotSendToChan(const std::string& requestingNick, const std::string& channel)
{
	return create()
		.setPrefix(SERVER)
		.setCode("404")
		.setContent(requestingNick + " " + channel)
		.setSuffix("Cannot send to channel")
		.build()
		.toString();
}

// 407 ERR_TOOMANYTARGETS
// :<server> 407 <requestingNick> <targets> :Too many recipients
// TODO : Function to get list of targets in 1 string with "," separator
std::string Builder::ErrTooManyTargets(const std::string& requestingNick, const std::string& targets)
{
	return create()
		.setPrefix(SERVER)
		.setCode("407")
		.setContent(requestingNick + " " + targets)
		.setSuffix("Too many recipients")
		.build()
		.toString();
}

// 411 ERR_NORECIPIENT
// ":<server> 411 <requesting_nick> :No recipient given (<command>)"
std::string Builder::ErrNoRecipient(const std::string& requestingNick, const std::string& command)
{
	return create()
		.setPrefix(SERVER)
		.setCode("411")
		.setContent(requestingNick)
		.setSuffix("No recipient given (" + command + ")")
		.build()
		.toString();
}

// 412 ERR_NOTEXTTOSEND
// ":<server> 412 <requesting_nick> :No text to send"
std::string Builder::ErrNoTextToSend(const std::string& requestingNick)
{
	return create()
		.setPrefix(SERVER)
		.setCode("412")
		.setContent(requestingNick)
		.setSuffix("No text to send")
		.build()
		.toString();
}

// 421 ERR_UNKNOWNCOMMAND
// ":<server> 421 <requestingNick> <command> :Unknown command"
std::string Builder::ErrUnknownCommand(const std::string& requestingNick, const std::string& command)
{
	return create()
		.setPrefix(SERVER)
		.setCode("421")
		.setContent(requestingNick + " " + command)
		.setSuffix("Unknown command")
		.build()
		.toString();
}

// 431 ERR_NONICKNAMEGIVEN
// :<server> 431 <requestingNick> :No nickname given
std::string Builder::ErrNoNickGiven(const std::string& requestingNick)
{
	return create()
		.setPrefix(SERVER)
		.setCode("431")
		.setContent(requestingNick)
		.setSuffix("No nickname given")
		.build()
		.toString();
}

// 432 ERR_ERRONEUSNICKNAME
// ":<server> 432 <requestingNick> <nick> :Erroneous nickname"
std::string Builder::ErrOneUsNick(const std::string& requestingNick, const std::string& nick)
{
	return create()
		.setPrefix(SERVER)
		.setCode("432")
		.setContent(requestingNick + " " + nick)
		.setSuffix("Erroneous nickname")
		.build()
		.toString();
}

// 433 ERR_NICKNAMEINUSE
// ":<server> <clientID> <existingNick> :Nickname is already in use"
// <clientID> is often "*" in this case (undefined)
std::string Builder::ErrNickInUse(const std::string& clientID, const std::string& existingNick)
{
	return create()
		.setPrefix(SERVER)
		.setCode("433")
		.setContent(clientID + " " + existingNick)
		.setSuffix("Nickname is already in use")
		.build()
		.toString();
}

// 436 ERR_NICKCOLLISION
// ":<server> 436 <user> <collisionNick> :Nickname collision KILL"
std::string Builder::ErrNickCollision(const std::string& collisionNick, const std::string& user = "*")
{
	return create()
		.setPrefix(SERVER)
		.setCode("436")
		.setContent(user + " " + collisionNick)
		.setSuffix("Nickname collision KILL")
		.build()
		.toString();
}

// 441 ERR_USERNOTINCHANNEL
// ":<server> 441 <requestingNick> <targetNick> #<channel> :They aren't on that channel"
std::string Builder::ErrUserNotInChannel(const std::string& requestingNick, const std::string& targetNick, const std::string& channel)
{
	return create()
		.setPrefix(SERVER)
		.setCode("441")
		.setContent(requestingNick + " " + targetNick + " " + channel)
		.setSuffix("They aren't on that channel")
		.build()
		.toString();
}

// 442 ERR_NOTONCHANNEL
// ":<server> 442 <requestingNick> #<channel> :You're not on that channel"
std::string Builder::ErrNotOnChannel(const std::string& requestingNick, const std::string& channel)
{
	return create()
		.setPrefix(SERVER)
		.setCode("442")
		.setContent(requestingNick + " " + channel)
		.setSuffix("You're not on that channel")
		.build()
		.toString();
}

// 443 ERR_USERONCHANNEL
// ":<server> 443 <requestingNick> <targetNick> #<channel> :is already on channel"
std::string Builder::ErrUserOnChannel(const std::string& requestingNick, const std::string& targetNick, const std::string& channel)
{
	return create()
		.setPrefix(SERVER)
		.setCode("443")
		.setContent(requestingNick + " " + targetNick + " " + channel)
		.setSuffix("is already on channel")
		.build()
		.toString();
}

// 451 ERR_NOTREGISTERED
// :<server> 451 <requestingNick> :You have not registered
std::string Builder::ErrNotRegistered(const std::string& requestingNick)
{
	return create()
		.setPrefix(SERVER)
		.setCode("451")
		.setContent(requestingNick)
		.setSuffix("You have not registered")
		.build()
		.toString();
}

// 461 ERR_NEEDMOREPARAMS
// ":<server> 461 <requestingNick> <command> :Not enough parameters"
std::string Builder::ErrNeedMoreParams(const std::string& requestingNick, const std::string& command)
{
	return create()
		.setPrefix(SERVER)
		.setCode("461")
		.setContent(requestingNick + " " + command)
		.setSuffix("Not enough parameters")
		.build()
		.toString();
}

// 464 ERR_PASSWDMISMATCH
// ":<server> 464 <requestingNick> :Password incorrect"
std::string Builder::PasswdMismatch(const std::string& requestingNick)
{
	return create()
		.setPrefix(SERVER)
		.setCode("464")
		.setContent(requestingNick)
		.setSuffix("Password incorrect")
		.build()
		.toString();
}

// 467 ERR_KEYSET
// ":<server> 467 <requestingNick> #<channel> :Channel key already set"
std::string Builder::ErrKeySet(const std::string& requestingNick, const std::string& channel)
{
	return create()
		.setPrefix(SERVER)
		.setCode("467")
		.setContent(requestingNick + " " + channel)
		.setSuffix("Channel key already set")
		.build()
		.toString();
}

// 471 ERR_CHANNELISFULL
// ":<server> 471 <requestingNick> #<channel> :Cannot join channel (+l)"
std::string Builder::ErrChannelIsFull(const std::string& requestingNick, const std::string& channel)
{
	return create()
			.setPrefix(SERVER)
			.setCode("471")
			.setContent(requestingNick + " " + channel)
			.setSuffix("Cannot join channel (+l)")
			.build()
			.toString();
}

// 472 ERR_UNKNOWNMODE
// ":<server> 472 <requestingNick> <mode> :is unknown mode char to me for #<channel>"
std::string Builder::ErrUnknownMode(const std::string& requestingNick, const std::string& mode, const std::string& channel)
{
	return create()
			.setPrefix(SERVER)
			.setCode("472")
			.setContent(requestingNick + " " + mode)
			.setSuffix("is unknown mode char to me for " + channel)
			.build()
			.toString();
}

// 473 ERR_INVITEONLYCHAN
// ":<server> 473 <requestingNick> #<channel> :Cannot join channel (+i)"
std::string Builder::ErrInviteOnlyChan(const std::string& requestingNick, const std::string& channel)
{
	return create()
			.setPrefix(SERVER)
			.setCode("473")
			.setContent(requestingNick + " " + channel)
			.setSuffix("Cannot join channel (+i)")
			.build()
			.toString();
}

// 475 ERR_BADCHANNELKEY
// ":<server> 475 <requestingNick> #<channel> :Cannot join channel (+k)"
std::string Builder::BadChannelKey(const std::string& requestingNick, const std::string& channel)
{
	return create()
			.setPrefix(SERVER)
			.setCode("475")
			.setContent(requestingNick + " " + channel)
			.setSuffix("Cannot join channel (+k)")
			.build()
			.toString();
}

// 481 ERR_NOPRIVILEGES
// ":<server> 481 <requestingNick> :Permission Denied- You're not an IRC operator"
std::string Builder::ErrNoPrivileges(const std::string& requestingNick)
{
	return create()
			.setPrefix(SERVER)
			.setCode("481")
			.setContent(requestingNick)
			.setSuffix("Permission Denied- You're not an IRC operator")
			.build()
			.toString();
}

// 482 ERR_CHANOPRIVSNEEDED
// ":<server> 482 <requestingNick> #<channel> :You're not channel operator"
std::string Builder::ErrChanOPrivsNeeded(const std::string& requestingNick, const std::string& channel)
{
	return create()
		.setPrefix(SERVER)
		.setCode("482")
		.setContent(requestingNick + " " + channel)
		.setSuffix("You're not channel operator")
		.build()
		.toString();
}

// 501 ERR_UMODEUNKNOWNFLAG
// ":<server> 501 <requesting_nick> :Unknown MODE flag"
std::string Builder::ErrUModeUnknownFlag(const std::string& requestingNick)
{
	return create()
		.setPrefix(SERVER)
		.setCode("501")
		.setContent(requestingNick)
		.setSuffix("Unknown MODE flag")
		.build()
		.toString();
}
