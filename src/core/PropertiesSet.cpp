/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PropertiesSet.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 19:14:37 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/11 19:23:54 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PropertiesSet.hpp"
#include <algorithm>

PropertiesSet::~PropertiesSet( void ) {}

PropertiesSet::PropertiesSet( void )
{
	this->_clientMaxBodySize = DEF_CLIENTMAXBODYSIZE;
	this->_autoIndex = DEF_AUTOINDEX;
	this->_defaultType = DEF_DEFAULTTYPE;
	this->_path_type = Root;
}

PropertiesSet::PropertiesSet(const PropertiesSet &cp)
{
	*this = cp;
}

void PropertiesSet::setIndexes(const std::vector<std::string> &index)
{
	this->_index = index;
}

void PropertiesSet::addIndex(const std::string &str)
{
	if (std::find(this->_index.begin(), this->_index.end(), str) == this->_index.end())
		this->_index.push_back(str);
}

void	PropertiesSet::setPath(const std::string &str)
{
	this->_path = str;
}

void	PropertiesSet::setDefaultType(const std::string &str)
{
	this->_defaultType = str;
}

void	PropertiesSet::setClientMaxBodySize(t_uint size)
{
	this->_clientMaxBodySize = size;
}

void	PropertiesSet::setAutoIndex(bool autoIndex)
{
	this->_autoIndex = autoIndex;
}

void	PropertiesSet::addErrorPage(t_code error, const std::string &page)
{
	this->_errorPages[error] = page;
}

void	PropertiesSet::setErrorPages(const	std::map<t_code, std::string> &errorPages)
{
	this->_errorPages = errorPages;
}

void	PropertiesSet::setTypes(const Types& types)
{
	this->_mimeTypes = types;
}

void PropertiesSet::setCGIConfig(const	std::map<std::string, CGI> &cgiConfig)
{
	this->_cgiConfig = cgiConfig;
}

void PropertiesSet::setPathType(const t_path_type type)
{
	this->_path_type = type;
}

t_path_type	PropertiesSet::getPathType( void ) const
{
	return (this->_path_type);
}

const std::map<std::string, CGI> &PropertiesSet::getCGIConfig( void ) const
{
	return (this->_cgiConfig);
}

const std::vector<std::string> &PropertiesSet::getIndexes( void ) const
{
	return (this->_index);
}
	
const std::string &PropertiesSet::getPath( void ) const
{
	return (this->_path);
}

const std::string &PropertiesSet::getDefaultType( void ) const
{
	return (this->_defaultType);
}

const Types	&PropertiesSet::getTypes( void ) const
{
	return (_mimeTypes);
}

t_uint PropertiesSet::getClientMaxBodySize( void ) const
{
	return (this->_clientMaxBodySize);
}

bool PropertiesSet::getAutoIndex( void ) const
{
	return (this->_autoIndex);
}

const std::map<t_code, std::string> &PropertiesSet::getErrorPages( void ) const
{
	return (this->_errorPages);
}

void PropertiesSet::addCGI(const CGI &cgi, const std::string &extension)
{
	this->_cgiConfig[extension] = cgi;
}

bool PropertiesSet::hasErrorPage(t_code error) const
{
	std::map<t_code, std::string>::const_iterator it = this->_errorPages.find(error);
	return (it != this->_errorPages.end());
}

bool PropertiesSet::hasCGI(const std::string &extension) const
{
	std::map<std::string, CGI>::const_iterator it = this->_cgiConfig.find(extension);
	return (it != this->_cgiConfig.end());
}

PropertiesSet	&PropertiesSet::operator=(const PropertiesSet &cp)
{
	if (this != &cp)
	{
		this->_index				= cp._index;
		this->_path					= cp._path;
		this->_errorPages			= cp._errorPages;
		this->_autoIndex			= cp._autoIndex;
		this->_clientMaxBodySize	= cp._clientMaxBodySize;
		this->_defaultType			= cp._defaultType;
		this->_mimeTypes			= cp._mimeTypes;
		this->_cgiConfig			= cp._cgiConfig;
		this->_path_type			= cp._path_type;
	}
	return (*this);
}
