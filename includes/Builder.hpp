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
# define HOST "localhost" // TODO : what about host in IRC ?
# define VERSION "42.0"

class Message;
class Client;
class Channel;

class Builder
{
	private:
		static Message _message;

		Builder();
		static Builder& setPrefix(const std::string& prefix);
		static Builder& setCode(const std::string& code);
		static Builder& setContent(const std::string& content);
		static Builder& setSuffix(const std::string& suffix);
		static Builder& create();
		static Builder& getInstance();
		Message build();

	public:
		// Commands
		static std::string Join(const std::string& nick, const std::string& user, const std::string& channel);
		static std::string PrivMsg(Client *sender, std::string msg, const std::string *ChannelName, const Client *recipient);
		static std::string Part(std::string nick,std::string user, std::string ChanName, std::string *msg);
		static std::string PartChan(std::string client, std::string ChanName);
		static std::string Kick(std::string client, std::string clientKicked, std::string ChanName, std::string *msg);
		static std::string Quit(std::string nick, std::string user, std::string msg);
		static std::string Ping();
		static std::string Nick(const std::string& oldNick, const std::string& user, const std::string& newNick);
		static std::string Invite(Client *inviter, std::string invitedNick, std::string channel);
		
		// Responses
		/* 001*/ static std::string RplWelcome(const std::string& nick, const std::string&user);
		/* 002*/ static std::string RplYourHost(const std::string& nick);
		/* 315*/ static std::string RplEndOfWho(const std::string& RequestingNick, const std::string& Channel);
		/* 324*/ static std::string RplChannelModeIs(Channel *Channel, std::string ClientName);
		/* 331*/ static std::string RplNoTopic(const std::string& requestingNick, const std::string& Channel);
		/* 332*/ static std::string RplTopic(const std::string &client, const std::string &Channel, const std::string &Topic);
		/* 341*/ static std::string RplInviting(const std::string &channel, const std::string &inviterNick, const std::string &invitedNick);
		/* 352*/ static std::string RplWhoReply(Channel *Channel, Client *RequestingClient, Client *Client);
		/* 353*/ static std::string RplNamReply(Channel *Channel, std::string requestingNick);

		// Errors
		/* 401*/ static std::string ErrNoSuchNick(const std::string& targetNick, const std::string& invalidNick);
		/* 403*/ static std::string ErrNoSuchChannel(const std::string& requestingNick, const std::string& channel);
		/* 404*/ static std::string ErrCannotSendToChan(const std::string& requestingNick, const std::string& channel);
		/* 407*/ static std::string ErrTooManyTargets(const std::string& requestingNick, const std::string& targets);
		/* 411*/ static std::string ErrNoRecipient(const std::string& requestingNick, const std::string& command);
		/* 412*/ static std::string ErrNoTextToSend(const std::string& requestingNick);
		/* 421*/ static std::string ErrUnknownCommand(const std::string& requestingNick, const std::string& command);
		/* 431*/ static std::string ErrNoNickGiven(const std::string& requestingNick);
		/* 432*/ static std::string ErrOneUsNick(const std::string& requestingNick, const std::string& nick);
		/* 433*/ static std::string ErrNickInUse(const std::string& clientID, const std::string& existingNick);
		/* 436*/ static std::string ErrNickCollision(const std::string& collisionNick, const std::string& user);
		/* 441*/ static std::string ErrUserNotInChannel(const std::string& requestingNick, const std::string& targetNick, const std::string& channel);
		/* 442*/ static std::string ErrNotOnChannel(const std::string& requestingNick, const std::string& channel);
		/* 443*/ static std::string ErrUserOnChannel(const std::string& requestingNick, const std::string& targetNick, const std::string& channel);
		/* 451*/ static std::string ErrNotRegistered();
		/* 462*/ static std::string ErrAlreadyRegisted(const std::string& nick);
		/* 461*/ static std::string ErrNeedMoreParams(const std::string& requestingNick, const std::string& command);
		/* 464*/ static std::string ErrPasswdMismatch(const std::string& requestingNick);
		/* 467*/ static std::string ErrKeySet(const std::string& requestingNick, const std::string& channel);
		/* 471*/ static std::string ErrChannelIsFull(const std::string& requestingNick, const std::string& channel);
		/* 472*/ static std::string ErrUModeUnknownMod(const char &requestingmode);
		/* 472*/ static std::string ErrUnknownMode(const std::string& requestingNick, const std::string& mode, const std::string& channel);
		/* 473*/ static std::string ErrInviteOnlyChan(const std::string& requestingNick, const std::string& channel);
		/* 475*/ static std::string BadChannelKey(const std::string& requestingNick, const std::string& channel);
		/* 476*/ static std::string BadChannelMask(const std::string& channel);
		/* 481*/ static std::string ErrNoPrivileges(const std::string& requestingNick);
		/* 482*/ static std::string ErrChanOPrivsNeeded(const std::string& requestingNick, const std::string& channel);
		/* 501*/ static std::string ErrUModeUnknownFlag(const std::string& requestingNick);
};

#endif