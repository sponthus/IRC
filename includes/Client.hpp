/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:16:01 by sponthus          #+#    #+#             */
/*   Updated: 2025/03/24 11:28:41 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>
#include <unistd.h>

#include <Channel.hpp>

class Channel;

class Client {
	public:
		Client();
		~Client();

		int					getFD() const;
		const std::string	getAddress() const;
		const std::string	getNick() const;
		const std::string	getUser() const;
		// const std::string	getFullName() const;
		// const std::string	getHostname() const;
		// const std::string	getServerName() const;
		bool				isRegistered() const;
		Channel				*getChannel(std::string name);
		const std::vector<Channel *>		getChannels() const;

		void		setFD(int fd);
		void		setAddress(std::string address);
		void		setNick(std::string nickname);
		void		setUser(std::string username);
		void		setFullName(std::string fullname);
		void		setHostname(std::string hostname);
		void		setServerName(std::string servername);
		void		registerUser();
		void		addChannel(Channel *_Channel);

		void		leaveChannels();

	private:
		int						_fd;
		std::string				_address;
		std::string				_nick;
		std::string				_user;
		std::string				_servername;
		std::string				_hostname;
		std::string				_fullname;
		bool					_registered;
		std::vector<Channel *>	_Channels;
};

#endif