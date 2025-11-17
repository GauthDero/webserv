/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 21:42:39 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/15 04:30:03 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include <vector>

#include "CommonTypes.hpp"
#include "PropertiesSet.hpp"
#include "Returnable.hpp"

class Location: public PropertiesSet, public Returnable
{
	private:
	t_http_method	_allowedMethods;
	std::string		_path;

	public:
	~Location( void );
	Location( void );
	Location(const PropertiesSet &ps);
	Location(std::string &path);
	Location(const Location & location);

	Location	&operator=(const Location &location);

	const	std::string		&getPath( void )			const;
	const	t_http_method	&getAllowedMethods( void )	const;

			void		setPath(const std::string &str);
			void		setAllowedMethods(const t_http_method	allowedMethods);
			void		addAllowedMethod(const t_http_method method);

			std::string	getHttpMethods( void )	const;

			bool		allowsGet( void )		const;
			bool		allowsPost( void )		const;
			bool		allowsDelete( void )	const;
			bool		allowsHead( void )		const;
};
