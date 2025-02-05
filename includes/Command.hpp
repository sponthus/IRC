/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:25:17 by endoliam          #+#    #+#             */
/*   Updated: 2025/02/05 12:10:41 by endoliam         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <iostream>
#include <list>
#include <vector>
#include <array>

class Command
{
	private:
		/*															*/
		Command(); // default constructor
		Command(std::string msg); // parametric constructor
		Command(Command &rhs); // copy constructor 
		~Command(); // default destructor
		Command	&operator=(Command &rhs); // operator = 

		/*			private members functions						*/

		void	Kick(std::string channel, std::list<std::string> users);
		void	Invite(std::string pseudo, std::string channel);
		void	Topic(std::string channel, std::string subject);
		void	Mode(std::string ar1);
		
		std::vector<std::array<std::string, 2>>		input;
	public:
		/*			members functions called by the server			*/
		void	Kick(void);
		void	Invite(void);
		void	Topic(void);
		void	Mode(void);
};

#endif