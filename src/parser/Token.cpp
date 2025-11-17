/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 19:33:44 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/06 02:31:54 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"

Token::Token(const std::string &str): _str(str), _type(Text) {}

Token::Token(const std::string &str, DataType type): _str(str), _type(type) {}

Token::Token(const Token &token): _str(token._str), _type(token._type) {}

Token::~Token( void ) {}

Token	&Token::operator=(const Token &token)
{
	if (this != &token)
	{
		this->_str = token._str;
		this->_type = token._type;
	}
	return (*this);
}

const std::string &Token::getString( void ) const
{
	return (this->_str);
}

DataType Token::getType( void ) const
{
	return (this->_type);
}

void Token::setType(DataType type)
{
	this->_type = type;
}

void Token::setString(const std::string &str)
{
	this->_str = str;
}
