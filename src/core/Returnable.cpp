/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Returnable.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 21:49:35 by dzapata           #+#    #+#             */
/*   Updated: 2025/10/20 18:54:41 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Returnable.hpp"
#include <algorithm>

Returnable::~Returnable( void ) {}

Returnable::Returnable( void )
{
	this->_hasRet = false;
}

Returnable::Returnable(const Returnable &res)
{
	*this = res;
}

Returnable &Returnable::operator=(const Returnable &res)
{
	if (this != &res)
	{
		this->_hasRet = res._hasRet;
		this->_ret = res._ret;
	}
	return (*this);
}

const Return &Returnable::getReturn( void ) const
{
	return (this->_ret);
}
bool Returnable::hasReturn( void ) const
{
	return (this->_hasRet);
}

void Returnable::setReturn(const Return &ret)
{
	this->_ret = ret;
}

void Returnable::setHasReturn(const bool hasRet)
{
	this->_hasRet = hasRet;
}
