/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:16:01 by sponthus          #+#    #+#             */
/*   Updated: 2025/01/29 13:06:57 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>
#include <unistd.h>

class Client {
	public:
		Client();
		~Client();

		int			getFD() const;
		std::string	getAddress() const;
		std::string	getNick() const;
		std::string	getUser() const;
		bool		isRegistered() const;

		void		setFD(int fd);
		void		setAddress(std::string address);
		void		setNick(std::string nickname);
		void		setUser(std::string username);

	private:
		int			_fd;
		std::string	_address;
		std::string	_nick;
		std::string	_user;
		bool		_registered;
};

#endif