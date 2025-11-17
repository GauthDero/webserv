/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Types.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 04:14:10 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/06 02:28:20 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Types.hpp"

Types::Types( void ) {}

Types::Types(const Types &types)
{
	*this = types;
}

Types::~Types( void ) {}

Types	&Types::operator=(const Types &types)
{
	if (this != &types)
		this->_types = types._types;
	return (*this);
}

void Types::addMimeType(const std::string &type, const std::string &extension)
{
	this->_types[extension] = type;
}

void Types::setMimeTypes(const std::map<std::string, std::string> &types)
{
	this->_types = types;
}

const std::map<std::string, std::string> &Types::getMimeTypes( void ) const
{
	return (this->_types);
}

bool Types::hasMimeType(const std::string &extension) const
{
	std::map<std::string, std::string>::const_iterator it = this->_types.find(extension);
	return (it != this->_types.end());
}
