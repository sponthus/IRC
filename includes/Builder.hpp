/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 14:22:06 by sponthus          #+#    #+#             */
/*   Updated: 2025/02/07 16:07:40 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_BUILDER_HPP
# define MESSAGE_BUILDER_HPP

#include <string>
#include "Client.hpp"
#include "Message.hpp"

# define SERVER "irc.server.42"
# define HOST "localhost"
# define VERSION "42.0"

class Message;
class Client;
class Channel;

class Builder
{
	private:
		static Message _message;

		Builder();
		static Builder &setPrefix(const std::string &Prefix);
		static Builder &setCode(const std::string &Code);
		static Builder &setContent(const std::string &Content);
		static Builder &setSuffix(const std::string &Suffix);
		static Builder &create();
		static Builder &getInstance();
		Message build();

	public:
		// Commands
		static std::string Nick(const std::string &OldNick, const std::string &Username, const std::string &NewNick);
		static std::string PrivMsg(Client *Sender, const std::string &Msg, const std::string *ChannelName, const Client *Recipient);
		static std::string Join(const std::string &Nickname, const std::string &Username, const std::string &Channel);
		static std::string Part(const std::string &Nickname, const std::string &Username, const std::string &Channel, const std::string *Msg);
		static std::string Kick(const std::string &KickerNick, const std::string &KickedNick, const std::string &Channel, const std::string *Msg);
		static std::string Invite(Client *Inviter, const std::string &InvitedNick, const std::string &Channel);
		static std::string Mode(const std::string &Nickname, const std::string &Channel, const std::string &RecapModes, const std::string &RecapOptions);
		static std::string Quit(const std::string &Nick, const std::string &Username, const std::string &Msg);

		// Responses
		/* 001*/ static std::string RplWelcome(const std::string &Nickname, const std::string &Username);
		/* 002*/ static std::string RplYourHost(const std::string &Nickname);
		/* 315*/ static std::string RplEndOfWho(const std::string &RequestingNick, const std::string &Channel);
		/* 324*/ static std::string RplChannelModeIs(Channel *Channel,  const std::string &RequestingNick);
		/* 331*/ static std::string RplNoTopic(const std::string &RequestingNick, const std::string &Channel);
		/* 332*/ static std::string RplTopic(const std::string &RequestingNick, const std::string &Channel, const std::string &Topic);
		/* 341*/ static std::string RplInviting(const std::string &Channel, const std::string &InviterNick, const std::string &InvitedNick);
		/* 352*/ static std::string RplWhoReply(Channel *Channel, Client *RequestingClient, Client *RequestedClient);
		/* 353*/ static std::string RplNamReply(Channel *Channel, const std::string &RequestingNick);

		// Errors
		/* 401*/ static std::string ErrNoSuchNick(const std::string &TargetNick, const std::string &InvalidNick);
		/* 403*/ static std::string ErrNoSuchChannel(const std::string &RequestingNick, const std::string &Channel);
		/* 407*/ static std::string ErrTooManyTargets(const std::string &RequestingNick, const std::string &Targets);
		/* 411*/ static std::string ErrNoRecipient(const std::string &RequestingNick, const std::string &Command);
		/* 412*/ static std::string ErrNoTextToSend(const std::string &RequestingNick);
		/* 421*/ static std::string ErrUnknownCommand(const std::string &RequestingNick, const std::string &Command);
		/* 431*/ static std::string ErrNoNickGiven(const std::string &RequestingNick);
		/* 432*/ static std::string ErrErroneusNickname(const std::string &RequestingNick, const std::string &RequestedNick);
		/* 433*/ static std::string ErrNickInUse(const std::string &ClientID, const std::string &ExistingNick);
		/* 441*/ static std::string ErrUserNotInChannel(const std::string &RequestingNick, const std::string &TargetNick, const std::string &Channel);
		/* 442*/ static std::string ErrNotOnChannel(const std::string &RequestingNick, const std::string &Channel);
		/* 443*/ static std::string ErrUserOnChannel(const std::string &RequestingNick, const std::string &TargetNick, const std::string &Channel);
		/* 451*/ static std::string ErrNotRegistered();
		/* 462*/ static std::string ErrAlreadyRegisted(const std::string &Nickname);
		/* 461*/ static std::string ErrNeedMoreParams(const std::string &RequestingNick, const std::string &Command);
		/* 464*/ static std::string ErrPasswdMismatch(const std::string &RequestingNick);
		/* 467*/ static std::string ErrKeySet(const std::string &RequestingNick, const std::string &Channel);
		/* 471*/ static std::string ErrChannelIsFull(const std::string &RequestingNick, const std::string &Channel);
		/* 472*/ static std::string ErrUnknownMode(const std::string &RequestingNick, const std::string &Mode, const std::string &Channel);
		/* 473*/ static std::string ErrInviteOnlyChan(const std::string &RequestingNick, const std::string &Channel);
		/* 475*/ static std::string BadChannelKey(const std::string &RequestingNick, const std::string &Channel);
		/* 476*/ static std::string BadChannelMask(const std::string &Channel);
		/* 481*/ static std::string ErrNoPrivileges(const std::string &RequestingNick);
		/* 482*/ static std::string ErrChanOPrivsNeeded(const std::string &RequestingNick, const std::string &Channel);
		/* 501*/ static std::string ErrUModeUnknownFlag(const std::string &RequestingNick);
};

#endif