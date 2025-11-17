/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AInstruction.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 22:40:27 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/06 02:30:18 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AInstruction.hpp"

AInstruction::AInstruction (const int minArgs, const int maxArgs, const int argsType):
	_minArgs(minArgs), _maxArgs(maxArgs), _argsType(argsType), _type(ITUnknown) {}

AInstruction::~AInstruction( void ) {}

AInstruction::AInstruction(const AInstruction & ainst):
	_minArgs(ainst._minArgs), _maxArgs(ainst._maxArgs), _argsType(ainst._argsType), _type(ainst._type) {}

int AInstruction::getMinArgs ( void ) const
{
	return (_minArgs);
}

int AInstruction::getMaxArgs ( void ) const
{
	return (_maxArgs);
}

int AInstruction::getArgsType ( void ) const
{
	return (_argsType);
}

InstructionType AInstruction::getType ( void ) const
{
	return (_type);
}
