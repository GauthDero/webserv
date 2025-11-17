/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:32:42 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/10 00:59:50 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Utils.tpp"

Server::~Server( void ) {}

Server::Server( void ) {}

Server::Server(const PropertiesSet &ps): PropertiesSet(ps), Returnable() {}

Server::Server(const Server & server):
	PropertiesSet(server), Returnable(server)
{
	this->_listen = server._listen;
	this->_locations = server._locations;
}

Server &Server::operator=(const Server &server)
{
	if (this != &server)
	{
		PropertiesSet::operator=(server);
		this->_locations = server._locations;
		this->_listen = server._listen;
	}
	return (*this);
}

const std::vector<Listen> &Server::getListeners( void ) const
{
	return (this->_listen);
}

const std::vector<Location> &Server::getLocations( void ) const
{
	return (this->_locations);
}

void Server::setLocations(const std::vector<Location> &locations)
{
	this->_locations = locations;
}

void Server::setListeners(const std::vector<Listen> &listen)
{
	this->_listen = listen;
}

void Server::addLocation(const Location &location)
{
	std::vector<Location>::iterator it = this->_locations.begin();
	while (it != this->_locations.end() && it->getPath() != location.getPath())
		++it;
	if (it == this->_locations.end())
		this->_locations.push_back(location);
	else
		*it = location;
}

void Server::addListener(const Listen &listener)
{
	if (std::find(this->_listen.begin(), this->_listen.end(), listener) == this->_listen.end())
		this->_listen.push_back(listener);
}

void Server::addListeners(const std::vector<Listen> &listeners)
{
	addUniqueElements(this->_listen, listeners);
}
