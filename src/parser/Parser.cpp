/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 05:04:00 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/15 05:15:22 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "Parser.hpp"
#include "ParserUtils.hpp"
#include "Log.hpp"
#include "server.hpp"

#include <iostream>
#include <cstdlib>

// Note: unless there is a discrepancy between the verifier and the expected model,
// the 'invalid instruction found' exceptions should never been throwed

// Blocks ======================================================================

Http	parseHttp(std::deque<Token>::iterator &it)
{
	Http	http;

	++it; // {
	while (it->getString() != "}")
	{
		if (it->getType() == Separator)
			++it;
		else if (it->getType() == Instruction)
		{
			if (parsePropertiesSet(http, it))
				continue ;
			else if (it->getString() == INS_SERVER)
			{
				Server serv(parseServer(Server(static_cast<PropertiesSet>(http)), ++it));
				if (serv.getListeners().size() == 0)
					serv.addListener(Listen());
				http.addServer(serv);
			}
			else
				throw std::runtime_error(
					stringError("Config file: http context: invalid instruction", it->getString().c_str()));
		}
		else
			throw std::runtime_error(
				stringError("Config file: http context: invalid token", it->getString().c_str()));
	}
	if (http.getServers().size() == 0)
	{
		Server serv;
		serv.addListener(Listen());
		http.addServer(Server());
	}
	++it; // }
	return (http);
}

Server	parseServer(const Server& parent, std::deque<Token>::iterator &it)
{
	Server	server(parent);

	++it; // {
	while (it->getString() != "}")
	{
		if (it->getType() == Separator)
			++it;
		else if (it->getType() == Instruction)
		{
			if (parsePropertiesSet(server, it) || parseReturnable(server, it))
				continue ;
			else if (it->getString() == INS_LOCATION)
				server.addLocation(parseLocation(Location(server), ++it));
			else if (it->getString() == INS_LISTEN)
				server.addListeners(parseListeners(++it));
			else
				throw std::runtime_error(
					stringError("Config file: Server context: invalid instruction found", it->getString().c_str()));
		}
		else
			throw std::runtime_error(
				stringError("Config file: Server context: invalid token found", it->getString().c_str()));
	}
	++it; // }
	return (server);
}

Location	parseLocation(const Location &parent, std::deque<Token>::iterator &it)
{
	Location	location(parent);

	location.setPath(parseString(it));
	++it; // Path
	++it; // {
	while (it->getString() != "}")
	{
		if (it->getType() == Separator)
			++it;
		else if (it->getType() == Instruction)
		{
			if (parsePropertiesSet(location, it) || parseReturnable(location, it))
				continue ;
			else if (it->getString() == INS_LIMIT_EXCEPT)
				location.setAllowedMethods(parseHttpMethodList(++it));
			else
				throw std::runtime_error(
					stringError("Config file: Location context: invalid instruction found", it->getString().c_str()));
		}
		else
			throw std::runtime_error(
				stringError("Config file: Location context: invalid token found", it->getString().c_str()));
	}
	++it; // }
	return (location);
}

Types	parseTypes(std::deque<Token>::iterator &it)
{
	Types	types;

	++it; // {
	while (it->getString() != "}")
	{
		if (it->getType() == Separator)
			++it;
		else if (it->getType() == MimeType)
			parseMimeTypes(types, it);
	}
	++it; // }
	return (types);
}

CGI	parseCGI(std::deque<Token>::iterator &it)
{
	CGI	cgi;

	++it; // {
	while (it->getString() != "}")
	{
		if (it->getType() == Separator)
			++it;
		else if (it->getType() == Instruction)
		{
			if (it->getString() == INS_PASS)
				cgi.setPass(parseString(++it));
			else if (it->getString() == INS_USE_SHEBANG)
				cgi.setUseShebang(parseBoolean(++it));
			else if (it->getString() == INS_EXECUTION_TIMEOUT)
				cgi.setExecutionTimeout(parseTime(++it));
			else
				throw std::runtime_error(
					stringError("Config file: CGI context: invalid instruction found", it->getString().c_str()));
			++it;
		}
		else
			throw std::runtime_error(
				stringError("Config file: CGI context: invalid token found", it->getString().c_str()));
	}
	++it; // }
	return (cgi);
}

// Instructions ================================================================

std::vector<Listen> parseListeners(std::deque<Token>::iterator &it)
{
	std::vector<Listen> listen;
	size_t				count = 0;
	for	(std::deque<Token>::iterator i = it; (*i).getType() != Separator; ++i)
		++count;
	listen.reserve(count);
	while (it->getType() != Separator)
		listen.push_back(parseListen(it++));
	return (listen);
}

Listen	parseListen(const std::deque<Token>::iterator &it)
{
	if (it->getType() == IPv4)
	{
		Listen listen;
		listen.setIP(parseIP(it));
		return (listen);
	}
	else if (it->getType() == Port)
		return (Listen(parsePort(it)));
	else if (it->getType() == IPPort)
		return (parseIPPort(it));
	else
		throw std::runtime_error(stringError("Config file: Listen: unknown type", it->getString().c_str()));
}

bool	parsePropertiesSet(PropertiesSet &ps, std::deque<Token>::iterator &it)
{
	if (it->getString() == INS_INDEX)
	{
		ps.setIndexes(parseStringList(++it));
		return (true);
	}
	else if (it->getString() == INS_ROOT)
	{
		ps.setPath(parseString(++it));
		ps.setPathType(Root);
		++it; // Argument
		return (true);
	}
	else if (it->getString() == INS_ALIAS)
	{
		ps.setPath(parseString(++it));
		ps.setPathType(Alias);
		++it; //Argument
		return (true);
	}
	else if (it->getString() == INS_DEFAULT_TYPE)
	{
		ps.setDefaultType(parseString(++it));
		++it; //Argument
		return (true);
	}
	else if (it->getString() == INS_AUTOINDEX)
	{
		ps.setAutoIndex(parseBoolean(++it));
		++it; // Argument
		return (true);
	}
	else if (it->getString() == INS_CLIENT_MAX_BODY_SIZE)
	{
		ps.setClientMaxBodySize(parseFileSize(++it));
		++it; // Argument
		return (true);
	}
	else if (it->getString() == INS_ERROR_PAGE)
	{
		parseErrorPages(ps, it);
		return (true);
	}
	else if (it->getString() == INS_TYPES)
	{
		++it; // Instruction
		ps.setTypes(parseTypes(it));
		return (true);
	}
	else if (it->getString() == INS_CGI)
	{
		parseCGIconfig(ps, it);
		return (true);
	}
	return (false);
}

bool	parseReturnable(Returnable &ret, std::deque<Token>::iterator &it)
{
	if (it->getString() == INS_RETURN)
	{
		ret.setHasReturn(true);
		ret.setReturn(parseReturn(++it));
		return (true);
	}
	return (false);
}

void	parseCGIconfig(PropertiesSet &ps, std::deque<Token>::iterator &it)
{
	std::deque<Token>::iterator temp = ++it;
	while (it->getString() != "{")
		++it;
	CGI	cgi(parseCGI(it));
	while (temp->getString() != "{")
	{
		ps.addCGI(cgi, temp->getString());
		++temp; // Extensions
	}
}

void	parseErrorPages(PropertiesSet &ps, std::deque<Token>::iterator &it)
{
	std::deque<Token>::iterator temp = ++it;
	while (temp->getType() != Text)
		++temp;
	std::string path(parseString(temp));
	while (it->getType() != Text)
	{
		ps.addErrorPage(static_cast<t_code>(parseUInteger(it)), path);
		++it; // Error codes
	}
	++it; // Text
}

void	parseMimeTypes(Types &types, std::deque<Token>::iterator &it)
{
	const std::string &type = it->getString();
	++it; // Type/Subtype
	while (it->getType() == Text)
	{
		types.addMimeType(type, parseString(it));
		++it; // Extension
	}
}

std::vector<std::string> parseStringList(std::deque<Token>::iterator &it)
{
	size_t	i = 0;
	std::deque<Token>::iterator	temp = it;
	std::vector<std::string>	list;
	while (temp->getType() != Separator)
	{
		i += !temp->getString().empty();
		++temp;
	}
	list.reserve(i);
	for (size_t j = 0; j < i; j++, ++it)
	{
		if (it->getString().empty())
			continue ;
		list.push_back(parseString(it));
	}
	return (list);
}

t_http_method parseHttpMethodList(std::deque<Token>::iterator &it)
{
	t_http_method	methods = HttpUnknown;

	while (it->getType() != Separator)
	{
		methods = static_cast<t_http_method>(methods | parseHttpMethod(it));
		++it;
	}
	return (methods);
}

// Data Types ==================================================================

t_int parseInteger(const std::deque<Token>::iterator &it)
{
	t_int	number = strtol(it->getString().c_str(), NULL, 10);
	return (number);
}

t_uint parseUInteger(const std::deque<Token>::iterator &it)
{
	t_uint	number = strtoul(it->getString().c_str(), NULL, 10);
	return (number);
}

t_uint parseFileSize(const std::deque<Token>::iterator &it)
{
	char	*unity;
	t_uint	number = strtoul(it->getString().c_str(), &unity, 10);

	if (!strncmp(unity, KB_ID, strlen(KB_ID) + 1))
		number *= KB_SIZE;
	else if (!strncmp(unity, MB_ID, strlen(MB_ID) + 1))
		number *= MB_SIZE;
	else if (!strncmp(unity, GB_ID, strlen(GB_ID) + 1))
		number *= GB_SIZE;
	return (number);
}

std::string	parseString(const std::deque<Token>::iterator &it)
{
	bool hasQuotes = isQuotes(it->getString().at(0));
	return (std::string(it->getString(), hasQuotes,
		it->getString().length() - (hasQuotes * 2))); // supprime Quotes and Null character
}

bool parseBoolean(const std::deque<Token>::iterator &it)
{
	return (it->getString() == BOOL_TRUE);
}

uint32_t parseIP(const std::deque<Token>::iterator &it)
{
	const char	*str = it->getString().c_str();
	uint32_t	ip = 0;
	for (size_t i = 0; i < 4; i++)
	{
		ip = static_cast<uint32_t>((ip << 8) | strtoul(str, NULL, 10));
		str = strchr(str, '.') + 1;
	}
	return (ip);
}

t_port parsePort(const std::deque<Token>::iterator &it)
{
	return (static_cast<t_port>(strtoul(it->getString().c_str(), NULL, 10)));
}

t_time parseTime(const std::deque<Token>::iterator &it)
{
	return (static_cast<t_time>(strtoul(it->getString().c_str(), NULL, 10)));
}

Listen	parseIPPort(const std::deque<Token>::iterator &it)
{
	uint32_t	ip = parseIP(it);
	t_port		port = static_cast<t_port>(strtoul(strchr(it->getString().c_str(), ':') + 1, NULL, 10));
	return (Listen(ip, port));
}

Return	parseReturn(std::deque<Token>::iterator &it)
{
	Return ret(static_cast<t_code>(parseUInteger(it)));

	if ((++it)->getType() != Separator)
	{
		ret.setString(parseString(it));
		++it;
	}
	return (ret);
}

t_http_method	parseHttpMethod(const std::deque<Token>::iterator &it)
{
	if (it->getString() == HTTP_GET)
		return (HttpGet);
	else if (it->getString() == HTTP_POST)
		return (HttpPost);
	else if (it->getString() == HTTP_DELETE)
		return (HttpDelete);
	else if (it->getString() == HTTP_HEAD)
		return (HttpHead);
	return (HttpUnknown);
}
