/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 00:03:49 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/07 18:03:33 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CommonTypes.hpp"

#include <iostream>
#include <stdint.h>

# define	DEFAULT_IP		0
# define	DEFAULT_PORT	8080

class Listen
{
	private:
	uint32_t	_ip;
	t_port		_port;

	public:
	Listen( void );
	Listen(t_port port);
	Listen(uint32_t ip, t_port port);

	Listen		&operator=(const Listen &listen);

	bool		operator==(const Listen &listen);

	uint32_t	getIP( void )	const;
	t_port		getPort( void )	const;

	void		setIP(uint32_t ip);
	void		setPort(t_port port);
};

std::ostream &operator<<(std::ostream &os, const Listen listen);
