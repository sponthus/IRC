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
		// Responses
		static std::string Welcome(const std::string& nick, const std::string&user);
		static std::string Ping();
		static std::string PrivMsg(const std::string& sender, const std::string& target, const std::string& message);
		static std::string Nick(const std::string& oldNick, const std::string& user, const std::string& newNick);
		static std::string RplJoin(const std::string& nick, const std::string& user, const std::string& channel);
		static std::string RplEndOfWho(const std::string& RequestingNick, const std::string& Channel);
		static std::string RplNoTopic(const std::string& Channel);
		static std::string RplTopic(const std::string &Channel, const std::string &Topic);
		static std::string RplInviting(const std::string &channel, const std::string &inviterNick, const std::string &invitedNick);
		static std::string RplInviting();
		static std::string RplPrivMsg(std::string client, std::string msg);
		static std::string RplNamReply(Channel *Channel, std::string requestingNick);
		static std::string RplWhoReply(Channel *Channel, Client *RequestingClient, Client *Client);
		static std::string RplLeave(std::string client, std::string ChanName);
		static std::string RplLeaveChan(std::string client, std::string ChanName);
		static std::string RplKicked(std::string client, std::string ChanName, std::string *msg);
		static std::string RplKick(std::string client, std::string clientKicked, std::string ChanName, std::string *msg);
		static std::string RplQuit(std::string nick, std::string user, std::string msg);
		// RPL_NAMREPLY
		// Errors
		static std::string ErrNoSuchNick(const std::string& targetNick, const std::string& invalidNick);
		static std::string ErrNoSuchChannel(const std::string& requestingNick, const std::string& channel);
		static std::string ErrCannotSendToChan(const std::string& requestingNick, const std::string& channel);
		static std::string ErrTooManyTargets(const std::string& requestingNick, const std::string& targets);
		static std::string ErrNoRecipient(const std::string& requestingNick, const std::string& command);
		static std::string ErrNoTextToSend(const std::string& requestingNick);
		static std::string ErrUnknownCommand(const std::string& requestingNick, const std::string& command);
		static std::string ErrNoNickGiven(const std::string& requestingNick);
		static std::string ErrOneUsNick(const std::string& requestingNick, const std::string& nick);
		static std::string ErrNickInUse(const std::string& clientID, const std::string& existingNick);
		static std::string ErrNickCollision(const std::string& collisionNick, const std::string& user);
		static std::string ErrUserNotInChannel(const std::string& requestingNick, const std::string& targetNick, const std::string& channel);
		static std::string ErrNotOnChannel(const std::string& requestingNick, const std::string& channel);
		static std::string ErrUserOnChannel(const std::string& requestingNick, const std::string& targetNick, const std::string& channel);
		static std::string ErrNotRegistered();
		static std::string ErrAlreadyRegisted(const std::string& nick);
		static std::string ErrNeedMoreParams(const std::string& requestingNick, const std::string& command);
		static std::string ErrPasswdMismatch(const std::string& requestingNick);
		static std::string ErrKeySet(const std::string& requestingNick, const std::string& channel);
		static std::string ErrChannelIsFull(const std::string& requestingNick, const std::string& channel);
		static std::string ErrUnknownMode(const std::string& requestingNick, const std::string& mode, const std::string& channel);
		static std::string ErrInviteOnlyChan(const std::string& requestingNick, const std::string& channel);
		static std::string BadChannelKey(const std::string& requestingNick, const std::string& channel);
		static std::string BadChannelMask(const std::string& channel);
		static std::string ErrNoPrivileges(const std::string& requestingNick);
		static std::string ErrChanOPrivsNeeded(const std::string& requestingNick, const std::string& channel);
		static std::string ErrUModeUnknownFlag(const std::string& requestingNick);
		static std::string ErrUModeUnknownMod(const char &requestingmode);
};

#endif