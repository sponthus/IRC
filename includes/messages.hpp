/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endoliam <endoliam@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:10:45 by sponthus          #+#    #+#             */
/*   Updated: 2025/03/17 15:28:00 by endoliam         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGES_HPP
# define MESSAGES_HPP

#include <iostream>
#include <signal.h>

# define ERROR "Error :"
# define ERR_NB_ARG "Wrong number of arguments, expected 2 : <port> <password>"
# define ERR_PORT_NUM "Port value should be numeric"
# define ERR_PORT_VAL "Valid port should be between 1024 and 65535"
# define ERR_PW "Password should not contain spaces, \' or \""

// # define RPL_NOTOPIC ""
// # define RPL_TOPIC " : no topic is set"
// # define ERR_NEEDMOREPARAMS " :Not enough parameters"
// # define ERR_NOTONCHANNEL " :You're not on that channel"
// # define ERR_CHANOPRIVSNEEDED " :You're not channel operator"

#endif