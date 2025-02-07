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
		// Errors
		static std::string buildErrorNoSuchNick(const std::string& targetNick, const std::string& invalidNick);
		static std::string buildNickInUse(const std::string& clientID, const std::string& existingNick);
		
		// Responses
		static std::string buildPing();
		static std::string buildPrivMsg(const std::string& sender, const std::string& target, const std::string& message);
};

#endif