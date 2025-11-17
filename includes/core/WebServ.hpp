/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 21:36:32 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/06 02:28:34 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "CommonTypes.hpp"
# include "Http.hpp"

class WebServ
{
	private:
	Http	_httpBlock;

	public:
	~WebServ( void );
	WebServ( void );
	WebServ(Http httpBlock);
	WebServ(const WebServ &webserv);

	WebServ	&operator=(const WebServ &webserv);

	const	Http	&getHttpBlock( void ) const;

			void	setHttpBlock(const Http &httpBlock);
};
