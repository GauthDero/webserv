/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 19:28:13 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/06 02:31:54 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include "Verify.hpp"

class Token
{
	private:
	std::string	_str;
	DataType	_type;

	public:
	Token(const std::string &str);
	Token(const std::string &str, DataType type);
	Token(const Token &token);
	virtual ~Token( void );

	Token	&operator=(const Token &token);

	const	std::string	&getString( void )	const;
			DataType	getType( void )		const;

			void		setType(DataType type);
			void		setString(const std::string &str);
};
