/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_cgi.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 07:56:11 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/15 05:11:39 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Log.hpp"
#include "Colors.hpp"
#include "Utils.hpp"
#include "cgiHandler.hpp"

#define CGI_HEADER_FIELDS	4

int	Response::Manage_cgi_header(const std::string &header, size_t &pos)
{
	static const char *fields[] =
	{
		"Status:",
		"Content-Type:", // mandatory
		"Content-Length:",
		"Location:"
	};

	size_t		i = 0;
	size_t		start;
	size_t		end;
	bool		is_standard_separator;
	std::string	*field;

	while (i < CGI_HEADER_FIELDS
		&& !Compare_insensitive(fields[i], header.c_str() + pos, start = std::strlen(fields[i])))
		++i;

	end = header.find('\n', pos);
	if (end == std::string::npos)
	{
		logMessage("Response: CGI Header: No field separator found", ERROR);
		return (2); // Error
	}

	is_standard_separator = header[end - 1] == '\r';
	end -= is_standard_separator; // Adjust in case is a \r\n
	end -= header[end - 1] == ' '; // Ignore optional Trailing space

	if (end == pos + start) // Empty field
	{
		logMessage("Response: CGI Header: Field with empty value", ERROR);
		return (2);
	}

	start += pos; // move to the start of the value
	start += header[start] == ' '; // Skip optional Leading space

	pos = end + 1 + is_standard_separator; //Setting the pos in the place of the next field.

	if (i == CGI_HEADER_FIELDS)
	{
		logMessage("Response: CGI Header: Unknown field found", WARNING);
		return (1); // Ignore field
	}

	switch (i) // Fragile and ugly af but works, i guess...
	{
		case 0:		field = &this->status_code;		break ;
		case 1:		field = &this->content_type;	break ;
		case 2:		field = &this->content_length;	break ;
		default:	field = &this->location;
	}

	if (!field->empty())
	{
		logMessage("Response: CGI Header: Duplicated field found", ERROR);
		return (2);
	}
	field->assign(header, start, end - start);
	return (0);
}

void	Response::Manage_cgi_response(const PropertiesSet &ps, t_code res)
{
	logMessage("Response: Managing CGI response...", INFO);

	if (res != OK_CODE)
	{
		logMessage("Response: Bad CGI execution", ERROR);
		return (Response::html_error(ps, res));
	}

	std::cout << BLUE << "CGI Response" << RESET << std::endl;
	printLimit(std::cout, this->body, "Response: CGI", 100000);

	size_t	pos = 0;

	while (pos < this->body.length() && this->body.compare(pos, 1,"\n") != 0
		&& this->body.compare(pos, 2,"\r\n") != 0)
	{
		if (Manage_cgi_header(this->body, pos) == 2)
			return (Response::html_error(ps, INTERNAL_SERVER_CODE));
	}

	if (this->content_type.empty())
	{
		logMessage("Response: CGI: Content-Type not found", ERROR);
		return (Response::html_error(ps, INTERNAL_SERVER_CODE));
	}

	if (pos == this->body.length())
	{
		logMessage("Response: CGI: no valid separator found", ERROR);
		return (Response::html_error(ps, INTERNAL_SERVER_CODE));	
	}

	pos += this->body[pos] == '\r';	// Skip \r if present
	++pos;								// Skip \n

	if (!this->content_length.empty())// Validate the content length
	{
		char	*err;
		size_t	length = std::strtoul(this->content_length.c_str(), &err, 10);

		if (err[0] || length != this->body.length() - pos)
		{
			logMessage("Response: CGI: Invalid Content-Length", ERROR);
			return (Response::html_error(ps, INTERNAL_SERVER_CODE));
		}
	}

	this->body.erase(0, pos);

	if (this->content_length.empty())
		Set_content_length();
	if (this->status_code.empty())
		this->status_code = Get_status_header(res);
}

bool	Response::try_cgi(const PropertiesSet& ps, const Connection &conn)
{
	CGI 	cgi_conf;
	bool	needs_cgi = false;

	try
	{
		logMessage("Response: Searching CGI configuration...", INFO);
		cgi_conf = Get_CGI(ps);
		needs_cgi = true;
		logMessage("Response: CGI configuration found", SUCCESS);
	}
	catch(const std::exception& e)
	{
		logMessage(e.what(), WARNING);
	}
	
	if (needs_cgi)
	{
		try
		{
			CgiHandler cgi(this->request, *this, cgi_conf, conn);
			Manage_cgi_response(ps, cgi.executeCgi(this->path));
		}
		catch (const FatalException &e)
		{
			throw;
		}
		catch (const std::exception &e)
		{
			logMessage(e.what(), ERROR);
			Response::html_error(ps, INTERNAL_SERVER_CODE);
		}
	}
	return (needs_cgi);
}

const CGI	&Response::Get_CGI(const PropertiesSet &ps)
{
	size_t	start = this->request_path.rfind('.');

	if (start == std::string::npos) // No extension found
		throw std::runtime_error("Response: No extension found");
	std::map<std::string, CGI>::const_iterator it = ps.getCGIConfig().begin();
	++start;
	while (it != ps.getCGIConfig().end()
		&& it->first.compare(0, it->first.length(), this->request_path, start, it->first.length()) != 0)
		++it;
	if (it == ps.getCGIConfig().end()) // No CGI config for the extension
		throw std::runtime_error(stringError("Response", "No CGI config found"));
	return (it->second);
}
