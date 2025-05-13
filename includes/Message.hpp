/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 14:41:27 by sponthus          #+#    #+#             */
/*   Updated: 2025/05/13 16:03:47 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <string>

class Message {

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