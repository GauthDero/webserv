/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Return.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 05:14:23 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/10 00:59:39 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Return.hpp"

Return::Return( void ): _str(""), _code(0) {}

Return::Return(const t_code code): _str(""), _code(code) {}

Return::Return(const t_code code, const std::string &str): _str(str), _code(code) {}

Return::Return(const Return &ret)
{
	*this = ret;
}

Return::~Return( void ) {}

Return &Return::operator=(const Return& ret)
{
	if (this != &ret)
	{
		this->_code = ret._code;
		this->_str = ret._str;
	}
	return (*this);
}

const std::string &Return::getString( void ) const
{
	return (this->_str);
}

t_code Return::getStatusCode( void ) const
{
	return (this->_code);
}

void Return::setString(const std::string &str)
{
	this->_str = str;
}

void Return::setStatusCode(const t_code code)
{
	this->_code = code;
}
