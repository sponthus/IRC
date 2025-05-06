/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:16:01 by sponthus          #+#    #+#             */
/*   Updated: 2025/05/06 14:28:46 by endoliam         ###   ########lyon.fr   */
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
		const std::string	getHostname() const;
		const std::string	getServerName() const;
		const std::string	getRealName() const;
		bool				isRegistered() const;
		bool				isPass() const;
		Channel				*getChannel(std::string name);
		const std::vector<Channel *>		getChannels() const;

		std::string			getMessage() const;
		void			addMessage(std::string message);
		void			setMessage(std::string message);
		void			clearMessage();

		void		setFD(int fd);
		void		setAddress(std::string address);
		void		setNick(std::string nickname);
		void		setUser(std::string username);
		void		setRealname(std::string Realname);
		void		setHostname(std::string hostname);
		void		setServerName(std::string servername);
		void		registerUser();
		void		PassUSer();
		void		addChannel(Channel *_Channel);
		void		removeChannel(Channel *_Channel);

		void		leaveChannels();

	private:
		int						_fd;
		std::string				_address;
		std::string				_nick;
		std::string				_user;
		std::string				_servername;
		std::string				_hostname;
		std::string				_realname;
		bool					_registered;
		bool					_Pass;
		std::vector<Channel *>	_Channels;
		std::string				_Message;
};

#endif