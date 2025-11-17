/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FatalException.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 23:47:57 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/07 18:02:19 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class FatalException: public std::exception
{
	private:
	const	char*	_message;

	public:
	FatalException(char *message);
	FatalException(const char *message);
	FatalException( void );

	const char* what() const throw();
};
