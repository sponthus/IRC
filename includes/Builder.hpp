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
#include "Message.hpp"

# define SERVER "irc.server.42"
# define HOST "localhost"

class Message;

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
		// Cmd
		static std::string Ping();
		static std::string PrivMsg(const std::string& sender, const std::string& senderUser, const std::string& target, const std::string& message);
		static std::string Nick(const std::string& oldNick, const std::string& user, const std::string& newNick);
		static std::string Join(const std::string& requestingNick, const std::string& requestingUser, const std::string& channel);
		static std::string Topic(const std::string& requestingNick, const std::string& requestingUser, const std::string& channel, const std::string& topic);
		static std::string Mode(const std::string& requestingNick, const std::string& requestingUser, const std::string& target, const std::string& modes, const std::string& parameters);
		static std::string Part(const std::string& requestingNick, const std::string& requestingUser, const std::string& channel, const std::string& message);
		static std::string Quit(const std::string& requestingNick, const std::string& requestingUser, const std::string& message);
		static std::string Kick(const std::string& requestingNick, const std::string& requestingUser, const std::string& channel, const std::string& targetNick, const std::string& reason);
		
		// Responses
		static std::string RplWelcome(const std::string& nick, const std::string&user);
		static std::string RplTryAgain(const std::string& requestingNick, const std::string& command);
		static std::string RplChannelModeIs(const std::string& requestingNick, const std::string& channel, const std::string& modes, const std::string& parameters);
		static std::string RplNoTopic(const std::string& requestingNick, const std::string& channel);
		static std::string RplTopic(const std::string& requestingNick, const std::string& channel, const std::string& topic);
		static std::string RplTopicWhoTime(const std::string& requestingNick, const std::string& channel, const std::string& topicSetter, const std::string& timestamp);
		static std::string RplInviting(const std::string& requestingNick, const std::string& channel, const std::string& targetNick);
		static std::string RplNamReply(const std::string& requestingNick, const std::string& channel, const std::string& nickList);
		static std::string RplEndOfNames(const std::string& requestingNick, const std::string& channel);
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
		static std::string ErrNotRegistered(const std::string& requestingNick);
		static std::string ErrNeedMoreParams(const std::string& requestingNick, const std::string& command);
		static std::string ErrPasswdMismatch(const std::string& requestingNick);
		static std::string ErrKeySet(const std::string& requestingNick, const std::string& channel);
		static std::string ErrChannelIsFull(const std::string& requestingNick, const std::string& channel);
		static std::string ErrUnknownMode(const std::string& requestingNick, const std::string& mode, const std::string& channel);
		static std::string ErrInviteOnlyChan(const std::string& requestingNick, const std::string& channel);
		static std::string ErrBadChannelKey(const std::string& requestingNick, const std::string& channel);
		static std::string ErrNoPrivileges(const std::string& requestingNick);
		static std::string ErrChanOPrivsNeeded(const std::string& requestingNick, const std::string& channel);
		static std::string ErrUModeUnknownFlag(const std::string& requestingNick);

};

#endif