/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Return.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 04:59:47 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/10 00:59:15 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include "CommonTypes.hpp"

class Return
{
	private:
	std::string	_str;
	t_code		_code;

	public:
	Return( void );
	Return(const t_code code);
	Return(const t_code code, const std::string &str);
	Return(const Return &ret);
	~Return( void );

	Return	&operator=(const Return& ret);

	const	std::string	&getString( void )		const;
			t_code		getStatusCode( void )	const;

			void		setString(const std::string &str);
			void		setStatusCode(const t_code code);
};
