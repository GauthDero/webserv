/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:32:27 by dzapata           #+#    #+#             */
/*   Updated: 2025/10/03 00:17:33 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

Http::~Http( void ) {}

Http::Http( void ): PropertiesSet() {}

Http::Http(const PropertiesSet &ps): PropertiesSet(ps) {}

Http::Http(const Http & http): PropertiesSet(http)
{
	this->servers = http.servers;
}

Http	&Http::operator=(const Http& http)
{
	if (this != &http)
	{
		PropertiesSet::operator=(http);
		this->servers = http.servers;
	}
	return (*this);
}

void	Http::addServer(const Server &serv)
{
	this->servers.push_back(serv);
}

const std::vector<Server> &Http::getServers( void ) const
{
	return (this->servers);
}
