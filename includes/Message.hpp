/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 14:41:27 by sponthus          #+#    #+#             */
/*   Updated: 2025/02/07 16:51:50 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <string>
// #include "Builder.hpp" // necessary ?

class Message {

	// friend class Builder; // Necessary ?

	private:
		std::string _prefix;
		std::string _code;
		std::string _content;
		std::string _suffix;

	public:
		Message();
		std::string	toString() const;
		void	setPrefix(const std::string& content);
		void	setCode(const std::string& content);
		void	setContent(const std::string& content);
		void	setSuffix(const std::string& content);
		void	clear();
};

#endif