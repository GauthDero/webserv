/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FatalException.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 00:10:41 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/06 02:47:54 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FatalException.hpp"

FatalException::FatalException(const char *message): _message(message) {}

FatalException::FatalException(char *message): _message(message) {}

FatalException::FatalException( void ): _message("Fatal error") {}

const char	*FatalException::what() const throw()
{
	return (this->_message);
}
