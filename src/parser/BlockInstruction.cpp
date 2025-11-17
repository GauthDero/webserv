/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlockInstruction.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 22:42:28 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/06 02:30:43 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BlockInstruction.hpp"
#include <algorithm>

void	BlockInstruction::initBlockInstruction(const char *allowed [], unsigned int n_allowed)
{
	this->_type = ITBlock;
	this->_allowedInstructions.reserve(n_allowed);
	for (unsigned int i = 0; i < n_allowed; i++)
		this->_allowedInstructions.push_back(allowed[i]);
}

BlockInstruction::BlockInstruction(const char *allowed [], unsigned int n_allowed,
	t_blk_flags flags, const int minArgs, const int argsType):
	AInstruction(minArgs, minArgs, argsType),
	_flags(flags)
{
	initBlockInstruction(allowed, n_allowed);
}

BlockInstruction::BlockInstruction(const char *allowed [], unsigned int n_allowed,
	t_blk_flags flags, const int min, const int max, const int argsType):
	AInstruction(min, max, argsType),
	_flags(flags)
{
	initBlockInstruction(allowed, n_allowed);
}

BlockInstruction::BlockInstruction(const char *allowed [], unsigned int n_allowed,
	t_blk_flags flags): AInstruction(0, 0, 0), _flags(flags)
{
	initBlockInstruction(allowed, n_allowed);
}

BlockInstruction::BlockInstruction(const char *allowed [], unsigned int n_allowed):
	AInstruction(0, 0, 0), _flags(BLKDefault)
{
	initBlockInstruction(allowed, n_allowed);
}

std::vector<std::string> &BlockInstruction::getAllowedInstructions( void )
{
	return (this->_allowedInstructions);
}

bool BlockInstruction::isAllowedInMainContext( void )	const
{
	return (this->_flags & BLKMainContext);
}

bool BlockInstruction::AcceptMimeTypes( void )	const
{
	return (this->_flags & BLKAcceptMimeTypes);
}

bool BlockInstruction::isAllowedInstruction(const std::string &str) const
{
	return (std::find(this->_allowedInstructions.begin(),
		this->_allowedInstructions.end(), str) != this->_allowedInstructions.end());
}
