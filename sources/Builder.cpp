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

Builder &Builder::setPrefix(const std::string &Prefix)
{
	_message.setPrefix(Prefix);
	return getInstance();
}

Builder &Builder::setCode(const std::string &Code)
{
	_message.setCode(Code);
	return getInstance();
}

Builder &Builder::setContent(const std::string &Content)
{
	_message.setContent(Content);
	return getInstance();
}

Builder &Builder::setSuffix(const std::string &Suffix)
{
	_message.setSuffix(Suffix);
	return getInstance();
}

Builder &Builder::create()
{
	_message.clear();
	return getInstance();
}

Builder &Builder::getInstance()
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

//////////////////////// COMMAND RESPONSES /////////////////////////


// User + every user sharing a canal with him see the change
// :<oldNick>!<user>@<host> NICK <newNick>
std::string Builder::Nick(const std::string &OldNick, const std::string &Username, \
	const std::string &NewNick)
{
	return create()
		.setPrefix(OldNick + "!" + Username + "@" + HOST)
		.setCode("NICK")
		.setContent(NewNick)
		.build()
		.toString();
}

// Uses either a recipient or a channel
std::string Builder::PrivMsg(Client *Sender, const std::string &Msg, \
	const std::string *Channel, const Client *Recipient)
{
	std::string Destination = "";
	if (Channel)
	{
		Destination += "#";
		Destination += *Channel;
	}
	else if (Recipient)
		Destination = Recipient->getNick();
	return create()
		.setPrefix(Sender->getNick() + "!" + Sender->getUser() + "@" + SERVER)
		.setCode("PRIVMSG")
		.setContent(Destination)
		.setSuffix(Msg)
		.build()
		.toString();
}

std::string Builder::Join(const std::string &Nickname, const std::string &Username, const std::string &Channel)
{
	return create()
		.setPrefix(Nickname + "!" + Username + "@" + HOST)
		.setCode("JOIN")
		.setContent("#" + Channel)
		.build()
		.toString();
}

std::string Builder::Part(const std::string &Nickname, const std::string &Username, \
	const std::string &Channel, const std::string *Msg)
{
	std::string Reason = "";
	if (Msg)
		Reason = *Msg;
	return create()
		.setPrefix(Nickname + "!" + Username + "@" + HOST)
		.setCode("PART")
		.setContent("#" + Channel)
		.setSuffix(Reason)
		.build()
		.toString();
}

std::string Builder::Kick(const std::string &KickerNick, const std::string &KickedNick, \
	const std::string &Channel, const std::string *Msg)
{
	std::string Reason = "";
	if (Msg)
		Reason = *Msg;
	return create()
		.setPrefix(KickerNick)
		.setCode("KICK")
		.setContent("#" + Channel + " " + KickedNick)
		.setSuffix(Reason)
		.build()
		.toString();
}

std::string Builder::Invite(Client *Inviter, const std::string &InvitedNick, \
	const std::string &Channel)
{
	return create()
		.setPrefix(Inviter->getNick() + "!" + Inviter->getUser() + "@" + SERVER)
		.setCode("INVITE")
		.setContent(InvitedNick + " #" + Channel)
		.build()
		.toString();
}

std::string Builder::Mode(const std::string &Nickname, const std::string &Channel, \
	const std::string &RecapModes, const std::string &RecapOptions)
{
	std::string Changes;
	Changes += RecapModes;
	if (!RecapOptions.empty())
	{
		Changes += " ";
		Changes += RecapOptions;
	}
	return create()
		.setPrefix(Nickname)
		.setCode("MODE")
		.setContent("#" + Channel + " " + Changes)
		.build()
		.toString();
}

// :<nickname>!<user>@<host> QUIT :<message>
std::string Builder::Quit(const std::string &Nickname, const std::string &Username, \
	const std::string &Msg)
{
	return create()
		.setPrefix(Nickname + "!" + Username + "@" + SERVER)
		.setCode("QUIT")
		.setSuffix(Msg)
		.build()
		.toString();
}

//////////////////////// RESPONSES MESSAGES /////////////////////////

// 001 RPL_WELCOME
// Response to a USER when logging in the server
// ":<server> 001 <nick> :Welcome to the IRC Network, <nick>!<user>@<host>"
std::string Builder::RplWelcome(const std::string &Nickname, const std::string &Username)
{
	return create()
		.setPrefix(SERVER)
		.setCode("001")
		.setContent(Nickname)
		.setSuffix("Welcome to the IRC Network, " + Nickname + "! " + Username + "@" + HOST)
		.build()
		.toString();
}

// 002 RPL_YOURHOST
// ":server 002 <nick> :Your host is <server_hostname>, running version <version>"
std::string Builder::RplYourHost(const std::string &Nickname)
{
	return create()
		.setPrefix(SERVER)
		.setCode("002")
		.setContent(Nickname)
		.setSuffix("Your host is " + std::string(HOST) + ", running version " + VERSION)
		.build()
		.toString();
}

///////////////////////// ERROR MESSAGES /////////////////////////

// 315 RPL_ENDOFWHO
// ":server 315 <client> <channel> :End of WHO list"
/* The RPL_WHOREPLY and RPL_ENDOFWHO pair are used
to answer a WHO message.  The RPL_WHOREPLY is only
sent if there is an appropriate match to the WHO
query.  If there is a list of parameters supplied
with a WHO message, a RPL_ENDOFWHO MUST be sent
after processing each list item with <name> being
the item.*/
std::string Builder::RplEndOfWho(const std::string &RequestingNick, const std::string &Channel)
{
	return create()
		.setPrefix(SERVER)
		.setCode("315")
		.setContent(RequestingNick + " #" + Channel)
		.setSuffix("End of WHO list")
		.build()
		.toString();
	}
	
// 324 RPL_CHANNELMODEIS
//     "<canal> <mode> <paramètres de mode >" 
std::string Builder::RplChannelModeIs(Channel *Channel, const std::string &RequestingNick)
{
	std::string Mode = "";
	std::string ModeArg = "";
	if (Channel->isInviteOnly())
		Mode += "i";
	if (Channel->isTopicRestrict())
		Mode += "t";
	if (Channel->hasPW())
	{
		Mode += "k";
		ModeArg += Channel->getPW();
	}
	if (Channel->hasUserLimit())
	{
		Mode += "l";
		if (!ModeArg.empty())
			ModeArg += " ";
		std::stringstream ss;
		ss << Channel->getUserLimit();
		ModeArg += ss.str();
	}
	if (!Mode.empty())
		Mode.insert(0, "+");

	std::string Result = "";
	if (!Mode.empty())
		Result += Mode;
	if (!ModeArg.empty())
	{
		if (!Result.empty())
			Result += " ";
		Result += ModeArg;
	}
	return create()
		.setPrefix(SERVER)
		.setCode("324 " + RequestingNick)
		.setContent("#" + Channel->getName() + " " + Result)
		.build()
		.toString();
}

// 331 RPL_NOTOPIC
// ":<server> 331 <nick> #<channel> :No topic is set"
std::string Builder::RplNoTopic(const std::string &RequestingNick, const std::string &Channel)
{
	return create()
		.setPrefix(SERVER)
		.setCode("331")
		.setContent(RequestingNick + " #" + Channel)
		.setSuffix("No topic is set")
		.build()
		.toString();
}

// 332 RPL_TOPIC
// Answer to viewing a topic . ":<server> 332 <nick> #<channel> :<topic>"
std::string Builder::RplTopic(const std::string &RequestingNick, const std::string &Channel, const std::string &Topic)
{
	return create()
		.setPrefix(SERVER)
		.setCode("332")
		.setContent(RequestingNick + " #" + Channel)
		.setSuffix(Topic)
		.build()
		.toString();
}

// 341 RPL_INVITING
// "#<channel> <nick>"
std::string Builder::RplInviting(const std::string &Channel, const std::string &InviterNick, const std::string &InvitedNick)
{
	return create()
		.setPrefix(SERVER)
		.setCode("341")
		.setContent(InviterNick + " " + InvitedNick + " " + "#" + Channel)
		.build()
		.toString();
}

// 352 RPL_WHOREPLY
//  ":<server> 352 <client> <channel> <username> <host> <server> <nick> <flags> :<hopcount> <realname>"
// 1 answer per present person, this creates every response
// hopcount = nb of intermediate servers between client and nick (0)
// flags = * if OP
// client = nick from person who sent WHO
// channel = asked channel
// server = Server sending the answer, then server the client is connected to
// Realname = not parsed in our server
/* The RPL_WHOREPLY and RPL_ENDOFWHO pair are used
to answer a WHO message.  The RPL_WHOREPLY is only
sent if there is an appropriate match to the WHO
query.  If there is a list of parameters supplied
with a WHO message, a RPL_ENDOFWHO MUST be sent
after processing each list item with <name> being
the item.*/
std::string Builder::RplWhoReply(Channel *Channel, Client *RequestingClient, Client *RequestedClient)
{
	std::string Prefix = RequestingClient->getNick() \
		+ " #" + Channel->getName() + " " \
		+ RequestingClient->getUser() + " " \
		+ HOST + " " + SERVER;

	std::string NickFlags = RequestedClient->getNick() + " H";
	if (Channel->isOP(RequestedClient))
		NickFlags += "@";
	std::string Result = create() \
		.setPrefix(SERVER) \
		.setCode("352") \
		.setContent(Prefix + " " + NickFlags) \
		.setSuffix("1") \
		.build() \
		.toString();
	return Result;
}

// 353 RPL_NAMREPLY
//     ":server 353 <nickname> = <channel> : [[@|+]<pseudo> [[@|+]<pseudo> [...]]]" 
std::string Builder::RplNamReply(Channel *Channel, const std::string &RequestingNick)
{
	std::string Names;
	for (std::vector<Client *>::const_iterator i = Channel->getClients().begin(); \
		i != Channel->getClients().end(); i++)
	{
		if (Channel->isOP(*i))
			Names += "@";
		Names += (*i)->getNick();
		Names += " ";
	}
	return create()
		.setPrefix(SERVER)
		.setCode("353")
		.setContent(RequestingNick + " = #" + Channel->getName())
		.setSuffix(Names)
		.build()
		.toString();
}

// 401 ERR_NOSUCHNICK 
// ":<server> 401 <targetNick> <invalidNick> :No such nick/channel" 
std::string Builder::ErrNoSuchNick(const std::string &TargetNick, const std::string &InvalidNick)
{
	return create()
		.setPrefix(SERVER)
		.setCode("401")
		.setContent(TargetNick + " " + InvalidNick)
		.setSuffix("No such nick")
		.build()
		.toString();
}

// 403 ERR_NOSUCHCHANNEL
// :<server> 403 <requestingNick> <channel> :No such channel
std::string Builder::ErrNoSuchChannel(const std::string &RequestingNick, const std::string &Channel)
{
	return create()
		.setPrefix(SERVER)
		.setCode("403")
		.setContent(RequestingNick + " #" + Channel)
		.setSuffix("No such channel")
		.build()
		.toString();
}

// 407 ERR_TOOMANYTARGETS
// :<server> 407 <requestingNick> <targets> :Too many recipients
// TODO : Function to get list of targets in 1 string with "," separator
std::string Builder::ErrTooManyTargets(const std::string &RequestingNick, const std::string &Targets)
{
	return create()
		.setPrefix(SERVER)
		.setCode("407")
		.setContent(RequestingNick + " " + Targets)
		.setSuffix("Too many recipients")
		.build()
		.toString();
}

// 411 ERR_NORECIPIENT
// ":<server> 411 <requesting_nick> :No recipient given (<command>)"
std::string Builder::ErrNoRecipient(const std::string &RequestingNick, const std::string &Command)
{
	return create()
		.setPrefix(SERVER)
		.setCode("411")
		.setContent(RequestingNick)
		.setSuffix("No recipient given (" + Command + ")")
		.build()
		.toString();
}

// 412 ERR_NOTEXTTOSEND
// ":<server> 412 <requesting_nick> :No text to send"
std::string Builder::ErrNoTextToSend(const std::string &RequestingNick)
{
	return create()
		.setPrefix(SERVER)
		.setCode("412")
		.setContent(RequestingNick)
		.setSuffix("No text to send")
		.build()
		.toString();
}

// 421 ERR_UNKNOWNCOMMAND
// ":<server> 421 <requestingNick> <command> :Unknown command"
std::string Builder::ErrUnknownCommand(const std::string &RequestingNick, const std::string &Command)
{
	return create()
		.setPrefix(SERVER)
		.setCode("421")
		.setContent(RequestingNick + " " + Command)
		.setSuffix("Unknown command")
		.build()
		.toString();
}

// 431 ERR_NONICKNAMEGIVEN
// :<server> 431 <requestingNick> :No nickname given
std::string Builder::ErrNoNickGiven(const std::string &RequestingNick)
{
	return create()
		.setPrefix(SERVER)
		.setCode("431")
		.setContent(RequestingNick)
		.setSuffix("No nickname given")
		.build()
		.toString();
}

// 432 ERR_ErrErroneusNicknameNAME
// ":<server> 432 <requestingNick> <nick> :Erroneous nickname"
// Returned after receiving a NICK message which contains characters which do not fall in the defined set. 
std::string Builder::ErrErroneusNickname(const std::string &RequestingNick, const std::string &RequestedNick)
{
	return create()
		.setPrefix(SERVER)
		.setCode("432")
		.setContent(RequestingNick + " " + RequestedNick)
		.setSuffix("Erroneous nickname")
		.build()
		.toString();
}

// 433 ERR_NICKNAMEINUSE
// ":<server> <clientID> <existingNick> :Nickname is already in use"
// <clientID> is often "*" in this case (undefined)
std::string Builder::ErrNickInUse(const std::string &ClientID, const std::string &ExistingNick)
{
	return create()
		.setPrefix(SERVER)
		.setCode("433")
		.setContent(ClientID + " " + ExistingNick)
		.setSuffix("Nickname is already in use")
		.build()
		.toString();
}

// TODO = Not used
// 436 ERR_NICKCOLLISION
// ":<server> 436 <user> <collisionNick> :Nickname collision KILL"
std::string Builder::ErrNickCollision(const std::string &CollisionNick, const std::string &Username = "*")
{
	return create()
		.setPrefix(SERVER)
		.setCode("436")
		.setContent(Username + " " + CollisionNick)
		.setSuffix("Nickname collision KILL")
		.build()
		.toString();
}

// 441 ERR_USERNOTINCHANNEL
// ":<server> 441 <requestingNick> <targetNick> #<channel> :They aren't on that channel"
std::string Builder::ErrUserNotInChannel(const std::string &RequestingNick, const std::string &TargetNick, const std::string &Channel)
{
	return create()
		.setPrefix(SERVER)
		.setCode("441")
		.setContent(RequestingNick + " " + TargetNick + " " + Channel)
		.setSuffix("They aren't on that channel")
		.build()
		.toString();
}

// 442 ERR_NOTONCHANNEL
// ":<server> 442 <requestingNick> #<channel> :You're not on that channel"
std::string Builder::ErrNotOnChannel(const std::string &RequestingNick, const std::string &Channel)
{
	return create()
		.setPrefix(SERVER)
		.setCode("442")
		.setContent(RequestingNick + " " + Channel)
		.setSuffix("You're not on that channel")
		.build()
		.toString();
}

// 443 ERR_USERONCHANNEL
// ":<server> 443 <requestingNick> <targetNick> #<channel> :is already on channel"
std::string Builder::ErrUserOnChannel(const std::string &RequestingNick, const std::string &TargetNick, const std::string &Channel)
{
	return create()
		.setPrefix(SERVER)
		.setCode("443")
		.setContent(RequestingNick + " " + TargetNick + " #" + Channel)
		.setSuffix("is already on channel")
		.build()
		.toString();
}

// 451 ERR_NOTREGISTERED
// :<server> 451 <requestingNick> :You have not registered
std::string Builder::ErrNotRegistered()
{
	return create()
		.setPrefix(SERVER)
		.setCode("451")
		.setContent(":")
		.setSuffix("You have not registered")
		.build()
		.toString();
}

// 461 ERR_NEEDMOREPARAMS
// ":<server> 461 <requestingNick> <command> :Not enough parameters"
std::string Builder::ErrNeedMoreParams(const std::string &RequestingNick, const std::string &Command)
{
	std::string Nick;
	if (RequestingNick.empty())
		Nick = "*";
	else
		Nick = RequestingNick;
	return create()
		.setPrefix(SERVER)
		.setCode("461")
		.setContent(Nick + " " + Command)
		.setSuffix("Not enough parameters")
		.build()
		.toString();
}

// 462 ERR_ALREADYREGISTRED
// :<server> 451 <requestingUSER> :":You may not reregister
std::string Builder::ErrAlreadyRegisted(const std::string &Nickname)
{
	return create()
		.setPrefix(SERVER)
		.setCode("462")
		.setContent(Nickname)
		.setSuffix("You may not reregister")
		.build()
		.toString();
}

// 464 ERR_PASSWDMISMATCH
// ":<server> 464 <requestingNick> :Password incorrect"
std::string Builder::ErrPasswdMismatch(const std::string &RequestingNick)
{
	std::string Nick;
	if (RequestingNick.empty())
		Nick = "*";
	else
		Nick = RequestingNick;
	return create()
		.setPrefix(SERVER)
		.setCode("464")
		.setContent(Nick)
		.setSuffix("Password incorrect")
		.build()
		.toString();
}

// 467 ERR_KEYSET
// ":<server> 467 <requestingNick> #<channel> :Channel key already set"
std::string Builder::ErrKeySet(const std::string &RequestingNick, const std::string &Channel)
{
	return create()
		.setPrefix(SERVER)
		.setCode("467")
		.setContent(RequestingNick + " #" + Channel)
		.setSuffix("Channel key already set")
		.build()
		.toString();
}

// 471 ERR_CHANNELISFULL
// ":<server> 471 <requestingNick> #<channel> :Cannot join channel (+l)"
std::string Builder::ErrChannelIsFull(const std::string &RequestingNick, const std::string &Channel)
{
	return create()
			.setPrefix(SERVER)
			.setCode("471")
			.setContent(RequestingNick + " #" + Channel)
			.setSuffix("Cannot join channel (+l)")
			.build()
			.toString();
}

// 472 ERR_UNKNOWNMODE
// ":<server> 472 <requestingNick> <mode> :is unknown mode char to me for #<channel>"
std::string Builder::ErrUnknownMode(const std::string &RequestingNick, const std::string &Mode, const std::string &Channel)
{
	return create()
			.setPrefix(SERVER)
			.setCode("472")
			.setContent(RequestingNick + " " + Mode)
			.setSuffix("is unknown mode char to me for #" + Channel)
			.build()
			.toString();
}

// 473 ERR_INVITEONLYCHAN
// ":<server> 473 <requestingNick> #<channel> :Cannot join channel (+i)"
std::string Builder::ErrInviteOnlyChan(const std::string &RequestingNick, const std::string &Channel)
{
	return create()
			.setPrefix(SERVER)
			.setCode("473")
			.setContent(RequestingNick + " #" + Channel)
			.setSuffix("Cannot join channel (+i)")
			.build()
			.toString();
}

// 475 ERR_BADCHANNELKEY
// ":<server> 475 <requestingNick> #<channel> :Cannot join channel (+k)"
std::string Builder::BadChannelKey(const std::string &RequestingNick, const std::string &Channel)
{
	return create()
			.setPrefix(SERVER)
			.setCode("475")
			.setContent(RequestingNick + " #" + Channel)
			.setSuffix("Cannot join channel (+k)")
			.build()
			.toString();
}

// 476 ERR_BADCHANMASK
//  "<channel> :Bad Channel Mask"
std::string Builder::BadChannelMask(const std::string &Channel)
{
	return create()
			.setPrefix(SERVER)
			.setCode("476")
			.setContent(Channel)
			.setSuffix("Bad Channel Mask")
			.build()
			.toString();
}

// 481 ERR_NOPRIVILEGES
// ":<server> 481 <requestingNick> :Permission Denied- You're not an IRC operator"
std::string Builder::ErrNoPrivileges(const std::string &RequestingNick)
{
	return create()
			.setPrefix(SERVER)
			.setCode("481")
			.setContent(RequestingNick)
			.setSuffix("Permission Denied- You're not an IRC operator")
			.build()
			.toString();
}

// 482 ERR_CHANOPRIVSNEEDED
// ":<server> 482 <requestingNick> #<channel> :You're not channel operator"
std::string Builder::ErrChanOPrivsNeeded(const std::string &RequestingNick, const std::string &Channel)
{
	return create()
		.setPrefix(SERVER)
		.setCode("482")
		.setContent(RequestingNick + " #" + Channel)
		.setSuffix("You're not channel operator")
		.build()
		.toString();
}

// 501 ERR_UMODEUNKNOWNFLAG
// ":<server> 501 <requesting_nick> :Unknown MODE flag"
std::string Builder::ErrUModeUnknownFlag(const std::string &RequestingNick)
{
	return create()
		.setPrefix(SERVER)
		.setCode("501")
		.setContent(RequestingNick)
		.setSuffix("Unknown MODE flag")
		.build()
		.toString();
}
