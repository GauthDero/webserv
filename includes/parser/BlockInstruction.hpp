/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlockInstruction.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:42:08 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/07 18:03:55 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "AInstruction.hpp"
#include <vector>
#include <string>

typedef enum e_blk_flags
{
	BLKDefault			= 0,
	BLKMainContext		= 1,
	BLKAcceptMimeTypes	= 1 << 1
}	t_blk_flags;

class BlockInstruction: public AInstruction
{
	protected:
	t_blk_flags					_flags;
	std::vector<std::string>	_allowedInstructions;

	private:
	// Emulate constructor delegation
	void	initBlockInstruction(const char *allowed [], unsigned int n_allowed);
	
	public:

	BlockInstruction(const char *allowed [], unsigned int n_allowed, t_blk_flags flags,
		const int amountArgs, const int argsType);
	BlockInstruction(const char *allowed [], unsigned int n_allowed, t_blk_flags flags,
		const int min, const int max ,const int argsType);
	BlockInstruction(const char *allowed [], unsigned int n_allowed, t_blk_flags flags);
	BlockInstruction(const char *allowed [], unsigned int n_allowed);
	
	std::vector<std::string>	&getAllowedInstructions( void );
	bool						isAllowedInMainContext( void )					const;
	bool						AcceptMimeTypes( void )							const;
	bool						isAllowedInstruction(const std::string &str)	const;
};
