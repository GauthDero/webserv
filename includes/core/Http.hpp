/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 21:37:04 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/07 18:04:42 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>
#include <map>

#include "CommonTypes.hpp"
#include "PropertiesSet.hpp"
#include "Server.hpp"

class Http: public PropertiesSet
{
	private:
	
	std::vector<Server>	servers;

	public:
	~Http( void );
	Http( void );
	Http(const PropertiesSet &ps);
	Http(const Http & http);

	Http	&operator=(const Http& http);

	void	addServer(const Server &serv);

	const	std::vector<Server>	&getServers( void )	const;
};
