/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sponthus <sponthus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:07:27 by sponthus          #+#    #+#             */
/*   Updated: 2025/01/23 16:19:04 by sponthus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

class Server {
	public: 
		Server();
		Server(const Server &src);
		~Server();

		Server &operator=(Server const &rhs);

	private :
		int			port;
		std::string pw;
		
}

#endif