/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 22:29:21 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/15 05:16:36 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"
#include "server.hpp"
#include "Colors.hpp"

#include <arpa/inet.h>

void	printTabs(std::ostream &out, unsigned int amount)
{
	for (unsigned int i = 0; i < amount; i++)
		out << "\t";
}

void	printCGI(std::ostream &out, const CGI &cgi, unsigned int level)
{
	const char	*boolean[] = {"no", "yes"};

	printTabs(out, level);
	out << "Pass: " << cgi.getPass() << std::endl;
	printTabs(out, level);
	out << "UseShebang: " << boolean[cgi.getUseShebang()] << std::endl;
	printTabs(out, level);
	out << "ExecutionTimeout: " << cgi.getExecutionTimeout() << std::endl;
}

void	printPropertiesSet(std::ostream &out, const PropertiesSet &ps, unsigned int level)
{
	const char	*boolean[] = {"no", "yes"};
	const char	*path_type[] = {"None", "Root", "Alias"};

	printTabs(out, level);
	out << "Path: " << ps.getPath() << std::endl;

	printTabs(out, level);
	out << "PathType: ";
	if (ps.getPathType() < 3)
		out << path_type[ps.getPathType()] << std::endl;
	else
		out << ps.getPathType() << std::endl;

	printTabs(out, level);
	out << "DefaultType: " << ps.getDefaultType() << std::endl;
	printTabs(out, level);
	out << "AutoIndex: " << (boolean[ps.getAutoIndex()]) << std::endl;
	printTabs(out, level);
	out << "ClientMaxBodySize: " << ps.getClientMaxBodySize() << std::endl;
	printTabs(out, level);
	out << "Indexes: " << ps.getIndexes().size() << std::endl;
	for (std::vector<std::string>::const_iterator it = ps.getIndexes().begin();
		it != ps.getIndexes().end(); ++it)
	{
		printTabs(out, level + 1);
		out << (*it) << std::endl;
	}
	printTabs(out, level);
	out << "Error Pages: " << ps.getErrorPages().size() << std::endl;
	for (std::map<t_code, std::string>::const_iterator	it = ps.getErrorPages().begin();
		it != ps.getErrorPages().end(); ++it)
	{
		printTabs(out, level + 1);
		out << it->first << " -> " << it->second << std::endl;
	}
	printTabs(out, level);
	out << "CGI: " << ps.getCGIConfig().size() << std::endl;
	for (std::map< std::string, CGI>::const_iterator	it = ps.getCGIConfig().begin();
		it != ps.getCGIConfig().end(); ++it)
	{
		printTabs(out, level + 1);
		out << "Extension: " << it->first << std::endl;
		printCGI(out, it->second, level + 2);
	}
	printTypes(out, ps.getTypes(), level);
}

void	printReturnable(std::ostream &out, const Returnable &ps, unsigned int level)
{
	const char	*boolean[] = {"no", "yes"};

	printTabs(out, level);
	out << "Has Return: " << (boolean[ps.hasReturn()]) << std::endl;
	if (ps.hasReturn())
	{
		printTabs(out, level + 1);
		out << "Status Code: " << ps.getReturn().getStatusCode() << std::endl;
		printTabs(out, level + 1);
		out << "Content: " << ps.getReturn().getString() << std::endl;
	}
}

int	amountAllowedMethods(const Location &location)
{
	return (location.allowsGet()
		+ location.allowsPost()
		+ location.allowsDelete()
		+ location.allowsHead());
}

void	printLocation(std::ostream &out, const Location &location, unsigned int level)
{
	printTabs(out, level);
	out << "Location" << std::endl;
	printTabs(out, level);
	out << "Path: " << location.getPath() << std::endl;
	printTabs(out, level);
	out << "Allowed Methods: " << amountAllowedMethods(location) << std::endl;
	if (location.allowsGet())
	{
		printTabs(out, level + 1);
		out << "Get" << std::endl;
	}
	if (location.allowsPost())
	{
		printTabs(out, level + 1);
		out << "Post" << std::endl;
	}
	if (location.allowsDelete())
	{
		printTabs(out, level + 1);
		out << "Delete" << std::endl;
	}
	if (location.allowsHead())
	{
		printTabs(out, level + 1);
		out << "Head" << std::endl;
	}
	printPropertiesSet(out, location, level);
	printReturnable(out, location, level);
}

void	printTypes(std::ostream &out, const Types &types, unsigned int level)
{
	printTabs(out, level);
	out << "Mime Types: " << types.getMimeTypes().size() << std::endl;
	for (std::map<std::string, std::string>::const_iterator	it = types.getMimeTypes().begin();
		it != types.getMimeTypes().end(); ++it)
	{
		printTabs(out, level + 1);
		out << it->first << " -> " << it->second << std::endl;
	}
}

void	printServer(std::ostream &out, const Server &server, unsigned int level)
{
	printTabs(out, level);
	out << "Server" << std::endl;
	printPropertiesSet(out, server, level + 1);
	printReturnable(out, server, level + 1);

	printTabs(out, level + 1);
	out << "Locations: " << server.getLocations().size() << std::endl;
	std::vector<Location>::const_iterator	it = server.getLocations().begin();
	for (size_t i = 0; i < server.getLocations().size(); ++i)
		printLocation(out, *it++, level + 2);

	printTabs(out, level + 1);
	out << "Listeners: " << server.getListeners().size() << std::endl;
	std::vector<Listen>::const_iterator	it2 = server.getListeners().begin();
	in_addr ip;
	for (size_t i = 0; i < server.getListeners().size(); i++)
	{
		printTabs(out, level + 2);
		ip.s_addr = htonl(it2->getIP());
		out << inet_ntoa(ip);
		out << " -> " << it2->getPort() << std::endl;
		++it2;
	}
}

void	printHTTP(std::ostream &out, const Http &http, unsigned int level)
{
	printTabs(out, level);
	out << "Http" << std::endl;
	printPropertiesSet(out, http, level + 1);
	std::vector<Server>::const_iterator	it = http.getServers().begin();
	for (size_t i = 0; i < http.getServers().size(); i++, ++it)
		printServer(out, (*it), level + 1);
}

void	printWebServConfig(std::ostream &out, const WebServ &ws)
{
	out << "Webserv" << std::endl;
	printHTTP(out, ws.getHttpBlock(), 1);
}

void	printConnection(std::ostream &out, sockaddr_in &client_addr)
{
	out << GREEN << "New connection from: "
		<< inet_ntoa(client_addr.sin_addr) << ":"
		<< ntohs(client_addr.sin_port) << RESET << std::endl;
}

void	printLimit(std::ostream &out, const std::string &str, const char *module, size_t limit)
{
	if (str.length() < limit)
		out << str << std::endl;
	else
	{
		size_t	separator = str.find("\r\n\r\n");
		if (separator > limit)
		{
			separator = str.find("\n\n");
			if (separator > limit)
			{
				logMessage(stringError(module, "Header separator not found"), WARNING);
				return ;
			}
			separator += 2;
		}
		else
			separator += 4;

		for (size_t i = 0; i < separator; i++)
			out.put(str.c_str()[i]);
		logMessage(stringError(module, "Content is too large to be printed"), WARNING);
	}
}

void	logMessage(std::ostream &out, const char *message, t_log_mode mode)
{
	const	char *color;
	const	char *start;

	switch (mode)
	{
		case INFO:			color = CYAN;		start = "[INFO]      ";	break;
		case SUCCESS:		color = GREEN;		start = "[SUCCESS]   ";	break;
		case WARNING:		color = YELLOW;		start = "[WARNING]   ";	break;
		case ERROR:			color = RED;		start = "[ERROR]     ";	break;
		case FATALERROR:	color = MAGENTA;	start = "[FATALERROR]";	break;
		default:			color = WHITE;		start = "[MESSAGE]   ";
	}
	out << color << start << " " << message << RESET << std::endl;
}

void	logMessage(const char *message, t_log_mode mode)
{
	logMessage(std::cerr, message, mode);
}

void	logMessage(const std::string &message, t_log_mode mode)
{
	logMessage(std::cerr, message.c_str(), mode);
}
