/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 17:17:51 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/15 05:14:25 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>

#include "CommonTypes.hpp"

#define	DEF_USESHEBANG		true
#define	DEF_TIMEOUT			10

class CGI
{
	private:
	std::string					_pass;
	t_time						_executionTimeout;
	bool						_useShebang;

	public:
	CGI( void );
	CGI(const CGI &cgi);
	~CGI( void );

	CGI		&operator=(const CGI &cgi);

	void	setPass(const std::string &pass);
	void	setUseShebang(const bool useShebang);
	void	setExecutionTimeout(t_time timeout);

	const	std::string	&getPass( void )				const;

	t_time				getExecutionTimeout( void )		const;
	bool				getUseShebang( void )			const;
};
