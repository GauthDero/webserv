/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:32:55 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/15 04:30:43 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::~Location( void ) {}

void	initLocation(Location &loc)
{
	loc.setAllowedMethods(static_cast<t_http_method>(HttpGet | HttpPost | HttpDelete | HttpHead));
	loc.setHasReturn(false);
}

Location::Location( void )
{
	initLocation(*this);
}

Location::Location(std::string &path): _path(path)
{
	initLocation(*this);
}

Location::Location(const PropertiesSet &ps): PropertiesSet(ps), Returnable()
{
	initLocation(*this);
}

Location::Location(const Location & location):
	PropertiesSet(location), Returnable(location)
{
	this->_path = location._path;
	this->_allowedMethods = location._allowedMethods;
}

Location &Location::operator=(const Location &location)
{
	if (this != &location)
	{
		PropertiesSet::operator=(location);
		this->_path = location._path;
		this->_allowedMethods = location._allowedMethods;
	}
	return (*this);
}

std::string	Location::getHttpMethods( void ) const
{
	std::string methods(static_cast<size_t>(
		this->allowsGet()		* 5 +
		this->allowsPost()		* 6 +
		this->allowsDelete()	* 8 +
		this->allowsHead()		* 6), '\0'
	);

	methods.resize(0);
	if (this->allowsGet())
		methods.append("GET, ");
	if (this->allowsPost())
		methods.append("POST, ");
	if (this->allowsDelete())
		methods.append("DELETE, ");
	if (this->allowsHead())
		methods.append("HEAD, ");
	if (methods.length() > 0)
		methods.erase(methods.length() - 2, 2);
	return (methods);
}

const std::string &Location::getPath( void ) const
{
	return (this->_path);
}

void Location::setPath(const std::string &str)
{
	this->_path = str;
}

void Location::setAllowedMethods(const t_http_method method)
{
	this->_allowedMethods = method;
}

void Location::addAllowedMethod(const t_http_method method)
{
	this->_allowedMethods = static_cast<t_http_method>(this->_allowedMethods | method);
}

const t_http_method &Location::getAllowedMethods( void ) const
{
	return (this->_allowedMethods);
}

bool Location::allowsGet( void ) const
{
	return (this->_allowedMethods & HttpGet);
}

bool Location::allowsPost( void ) const
{
	return (this->_allowedMethods & HttpPost);
}

bool Location::allowsDelete( void ) const
{
	return (this->_allowedMethods & HttpDelete);
}

bool Location::allowsHead( void ) const
{
	return (this->_allowedMethods & HttpHead);
}
