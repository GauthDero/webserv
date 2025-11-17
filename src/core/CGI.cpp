/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 17:31:43 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/15 05:14:33 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI( void ):  _executionTimeout(DEF_TIMEOUT), _useShebang(DEF_USESHEBANG) {}

CGI::CGI(const CGI &cgi)
{
	*this = cgi;
}

CGI::~CGI( void ) {}

CGI &CGI::operator=(const CGI &cgi)
{
	if (this != &cgi)
	{
		this->_pass = cgi._pass;
		this->_useShebang = cgi._useShebang;
		this->_executionTimeout = cgi._executionTimeout;
	}
	return (*this);
}

void	CGI::setPass(const std::string &pass)
{
	this->_pass = pass;
}

void	CGI::setUseShebang(const bool useShebang)
{
	this->_useShebang = useShebang;
}

void	CGI::setExecutionTimeout(t_time timeout)
{
	this->_executionTimeout = timeout;
}

t_time	CGI::getExecutionTimeout( void ) const
{
	return (this->_executionTimeout);
}

const std::string &CGI::getPass( void ) const
{
	return (this->_pass);
}

bool CGI::getUseShebang( void ) const
{
	return (this->_useShebang);
}
