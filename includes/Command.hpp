/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:25:17 by endoliam          #+#    #+#             */
/*   Updated: 2025/01/30 16:43:08 by endoliam         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <iostream>
#include <list>

class Command
{
	private:
				/*			*/
		Command();
		~Command();
		/*			private members functions				*/
		void	Kick(std::string channel, std::list<std::string> users);
		void	Invite(std::string pseudo, std::string channel);
		void	Topic(std::string channel, std::string subject);
		void	Mode(std::string ar1);
				
	public:
		/*			members functions called by the server			*/
		void	Kick(void);
		void	Invite(void);
		void	Topic(void);
		void	Mode(void);
};


#endif