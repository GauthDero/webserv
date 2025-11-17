/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AInstruction.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 20:25:39 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/06 02:30:18 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

enum InstructionType
{
	ITUnknown,
	ITSingle,
	ITBlock
};

class AInstruction
{
	protected:
	int				_minArgs;
	int				_maxArgs;
	int				_argsType;
	InstructionType	_type;

	public:
	AInstruction(const int minArgs, const int maxArgs, const int argsType);
	AInstruction(const AInstruction & ainst);
	virtual ~AInstruction( void );

	int					getMinArgs( void )	const;
	int					getMaxArgs( void )	const;
	int					getArgsType( void )	const;
	InstructionType		getType( void )		const;
};
