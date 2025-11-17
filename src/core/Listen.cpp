/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:33:17 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/06 03:07:12 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Listen.hpp"

Listen::Listen( void ): _ip(DEFAULT_IP), _port(DEFAULT_PORT) {}

Listen::Listen(t_port port): _ip(DEFAULT_IP), _port(port) {}

Listen::Listen(uint32_t ip, t_port port): _ip(ip), _port(port) {}

uint32_t	Listen::getIP( void ) const
{
	return (this->_ip);
}

t_port	Listen::getPort( void ) const
{
	return (this->_port);
}

Listen &Listen::operator=(const Listen &listen)
{
	if (this != &listen)
	{
		this->_ip = listen._ip;
		this->_port = listen._port;
	}
	return (*this);
}

bool Listen::operator==(const Listen &listen)
{
	return (this->_ip == listen._ip && this->_port == listen._port);
}

void Listen::setIP(uint32_t ip)
{
	this->_ip = ip;
}

void Listen::setPort(t_port port)
{
	this->_port = port;
}

std::ostream &operator<<(std::ostream &os, const Listen listen)
{
	os << listen.getIP() << ":" << listen.getPort();
	return (os);
}
