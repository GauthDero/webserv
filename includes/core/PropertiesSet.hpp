/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PropertiesSet.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:09:50 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/11 19:05:22 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <vector>
#include <string>
#include "CommonTypes.hpp"
#include "CGI.hpp"
#include "Types.hpp"

#define	DEF_CLIENTMAXBODYSIZE	1000000 // 1MB
#define	DEF_AUTOINDEX			false
#define DEF_DEFAULTTYPE			"text/plain"

// Set of properties shared among most blocks

typedef enum e_path_type
{
	None,
	Root,
	Alias
}	t_path_type;

class PropertiesSet
{
	protected:
	Types							_mimeTypes;
	std::vector<std::string>		_index;
	std::map<std::string, CGI>		_cgiConfig;
	std::map<t_code, std::string>	_errorPages;
	std::string						_path;
	std::string						_defaultType;						
	t_uint							_clientMaxBodySize;
	t_path_type						_path_type;
	bool							_autoIndex;

	public:

	virtual ~PropertiesSet( void );
	PropertiesSet( void );
	PropertiesSet(const PropertiesSet &cp);

	PropertiesSet	&operator=(const PropertiesSet &cp);

	void	setPath(const std::string &str);
	void	setPathType(const t_path_type type);
	void	setClientMaxBodySize(const t_uint size);
	void	setAutoIndex(const bool autoIndex);
	void	setDefaultType(const std::string &str);
	void	setTypes(const Types& types);

	void	setIndexes(const std::vector<std::string> &index);
	void	setCGIConfig(const	std::map<std::string, CGI> &cgiConfig);
	void	setErrorPages(const	std::map<t_code, std::string> &errorPages);

	void	addIndex(const std::string &str);
	void	addErrorPage(t_code error, const std::string &page);
	void	addCGI(const CGI &cgi, const std::string &page);
	
	const	std::vector<std::string>		&getIndexes( void )						const;
	const	std::string						&getPath( void )						const;
	const	std::string						&getDefaultType( void )					const;
	const	std::map<t_code, std::string>	&getErrorPages( void )					const;
	const	std::map<std::string, CGI>		&getCGIConfig( void )					const;
	const	Types							&getTypes( void )						const;
	
			t_uint							getClientMaxBodySize( void )			const;
			t_path_type						getPathType( void )						const;
			bool							getAutoIndex( void )					const;

			bool							hasErrorPage(t_code error)				const;
			bool							hasCGI(const std::string &extension)	const;
};
