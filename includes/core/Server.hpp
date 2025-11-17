/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 21:42:57 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/06 02:29:20 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <map>

#include "PropertiesSet.hpp"
#include "CommonTypes.hpp"
#include "Location.hpp"
#include "Listen.hpp"
#include "Returnable.hpp"

class Server: public PropertiesSet, public Returnable
{
	private:
	std::vector<Location>	_locations;
	std::vector<Listen>		_listen;

	public:
	~Server( void );
	Server( void );
	Server(const PropertiesSet &ps);
	Server(const Server & server);

	Server	&operator=(const Server &server);

	const	std::vector<Listen>			&getListeners( void )	const;
	const	std::vector<Location>		&getLocations( void )	const;

			void						setListeners(const std::vector<Listen> &listen);
			void						setLocations(const std::vector<Location> &locations);

			void						addLocation(const Location &location);
			void						addListener(const Listen &listener);
			void						addListeners(const std::vector<Listen> &listeners);
};
