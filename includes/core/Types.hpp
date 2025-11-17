/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Types.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 03:33:17 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/06 02:28:20 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>

class Types
{
	private:
	std::map<std::string, std::string>	_types;

	public:
	Types( void );
	Types(const Types &types);
	~Types( void );

	Types	&operator=(const Types &types);

	void	addMimeType(const std::string &type, const std::string &extension);
	void	setMimeTypes(const	std::map<std::string, std::string> &types);

	const	std::map<std::string, std::string>	&getMimeTypes( void )						const;
			bool								hasMimeType(const std::string &extension)	const;
};
