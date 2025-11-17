/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:32:11 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/06 02:28:32 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

WebServ::WebServ( void ) {}

WebServ::~WebServ( void ) {}

WebServ::WebServ(Http httpBlock): _httpBlock(httpBlock) {}

WebServ::WebServ(const WebServ &webserv)
{
	*this = webserv;
}

WebServ	&WebServ::operator=(const WebServ &webserv)
{
	if (this != &webserv)
	{
		this->_httpBlock = webserv._httpBlock;
	}
	return (*this);
}

const Http &WebServ::getHttpBlock( void ) const
{
	return (this->_httpBlock);
}

void WebServ::setHttpBlock(const Http &httpBlock)
{
	this->_httpBlock = httpBlock;
}
