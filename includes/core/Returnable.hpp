/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Returnable.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 23:58:12 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/07 18:04:23 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Return.hpp"

class Returnable
{
	private:
	Return	_ret;
	bool	_hasRet;

	public:
	Returnable( void );
	Returnable(const Returnable &ret);
	~Returnable( void );

	Returnable	&operator=(const Returnable &ret);

	const	Return		&getReturn( void )	const;
			bool		hasReturn( void )	const;

			void		setReturn(const Return &ret);
			void		setHasReturn(const bool hasRet);
};
