/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:16:01 by sponthus          #+#    #+#             */
/*   Updated: 2025/01/28 15:26:56 by sponthus         ###   ########.fr       */
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

		void		setFD(int fd);
		void		setAddress(std::string address);
		int			getFD() const;
		std::string	getAddress() const;

	private:
		int			_fd;
		std::string	_address;
};

#endif