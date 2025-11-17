/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 01:05:12 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/15 04:32:00 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

#include "StatusCodes.hpp"
#include "Log.hpp"
#include "Colors.hpp"
#include "Utils.hpp"
#include <cstdlib>
#include <cstring>
#include <sstream>

#define MAX_ACCEPTED_FIELDS	13

// Static ======================================================================

const char* Request::accepted_fields[] = {
	"Host:", "Content-Length:", "Date:", "User-Agent:",	// Must not be duplicated
	"Connection:", "Referer:", "Accept:", "Accept-Language:",
	"Accept-Encoding:", "Upgrade-Insecure-Requests:", "Content-Type:",
	"Transfer-Encoding:", "Expect:"
};

// Constructor =================================================================

Request::Request(const Server &serv) : server(serv)
{
	// Fragile but works
	this->elements[0] = &this->host;
	this->elements[1] = &this->content_length;
	this->elements[2] = &this->date;
	this->elements[3] = &this->user_agent;
	this->elements[4] = &this->connection;
	this->elements[5] = &this->referer;
	this->elements[6] = &this->accept;
	this->elements[7] = &this->accept_language;
	this->elements[8] = &this->accept_encoding;
	this->elements[9] = &this->upgrade_insecure_requests;
	this->elements[10] = &this->content_type;
	this->elements[11] = &this->transfer_encoding;
	this->elements[12] = &this->expect;
	this->method = HttpUnknown;
}

Request::~Request() {}

// Getters =====================================================================

const std::string &Request::Get_protocol( void ) const
{
	return (this->http_version);
}

const std::string &Request::Get_host( void ) const
{
	return (this->host);
}

const std::string &Request::Get_date( void ) const
{
	return (this->date);
}

const std::string &Request::Get_user_agent( void ) const
{
	return (this->user_agent);
}

t_http_method Request::Get_method( void ) const
{
	return (this->method);
}

const std::string &Request::Get_path( void ) const
{
	return (this->path);
}

const std::string &Request::Get_body( void ) const
{
	return (this->body);
}

const std::string &Request::Get_content_type( void ) const
{
	return (this->content_type);
}

const std::string &Request::Get_query_string ( void ) const
{
	return (this->query_string);
}

const std::string &Request::Get_info_path ( void ) const
{
	return (this->info_path);
}

const Server &Request::Get_server( void ) const
{
	return (this->server);
}

const std::string &Request::Get_accept( void ) const
{
	return (this->accept);
}

const std::string &Request::Get_accept_language( void ) const
{
	return (this->accept_language);
}

const std::string &Request::Get_accept_encoding( void ) const
{
	return (this->accept_encoding);
}

const std::string &Request::Get_referer( void ) const
{
	return (this->referer);
}

const std::deque<std::string> &Request::Get_unknown_fields( void ) const
{
	return (this->unknown_fields);
}

void Request::Set_protocol(const std::string &http_version_)
{
	this->http_version = http_version_;
}

// Other Functions =============================================================

bool	Request::enough_info() const
{
	return (!(this->http_version.empty() || this->method == HttpUnknown ||
		this->path.empty() || this->host.empty() ||
		(!this->content_length.empty() && !this->transfer_encoding.empty()) ||
		((this->method == HttpPost) && this->content_type.empty())));
}

void	Request::Set_cgi_paths( void )
{
	size_t	info_path_pos;
	size_t	pos = this->path.find("?");
	if (pos != std::string::npos)
	{
		this->query_string.assign(this->path, pos + 1);
		this->path.resize(pos);
	}
	pos = this->path.find(".");
	info_path_pos = this->path.find('/', pos);
	if (pos != std::string::npos && info_path_pos != std::string::npos)
	{
		info_path_pos = this->path.find('/', pos);
		if (info_path_pos != std::string::npos)
		{
			this->info_path.assign(this->path, info_path_pos);
			this->path.erase(info_path_pos);
		}
	}
}

int	Request::analyse_first_line(const std::string &line)
{
	size_t	path_beginning;
	size_t	path_length;
	size_t	http_version_pos;

	if (line.compare(0, 4, "GET ") == 0)
		this->method = HttpGet;
	else if (line.compare(0, 5, "POST ") == 0)
		this->method = HttpPost;
	else if (line.compare(0, 7, "DELETE ") == 0)
		this->method = HttpDelete;
	else if (line.compare(0, 5, "HEAD ") == 0)
		this->method = HttpHead;
	else
	{
		logMessage("Request: Bad method", ERROR);
		return (2);
	}

	path_beginning = line.find_first_of(" ") + 1;
	if (path_beginning == std::string::npos || line[path_beginning] != '/')
		return (1);
	path_length = path_beginning;
	while (line[path_length] && line[path_length] != ' ')
		path_length++;
	if (line[path_length] != ' ') // If the string ends before encounting the ' '
		return (1);
	path_length -= path_beginning;
	this->path.assign(line, path_beginning, path_length);	

	http_version_pos = path_length + path_beginning + 1;
	if (line.compare(http_version_pos, 5, "HTTP/") != 0		// HTTP/
		|| !std::isdigit(line[http_version_pos + 5])		// DIGIT
		|| line[http_version_pos + 6] != '.'				// .
		|| !std::isdigit(line[http_version_pos + 7])		// DIGIT
		|| line.compare(http_version_pos + 8, 2, "\r\n"))	// \r\n
		return (1);

	this->http_version.assign(line, http_version_pos, 8);	// Excludes "\r\n"
	Request::Set_cgi_paths();
	return (0);
}

bool	Request::analyse_line(const std::string &line)
{
	int		index = 0;
	size_t	field_pos;
	size_t	end_pos;

	while (index < MAX_ACCEPTED_FIELDS
		&& !Compare_insensitive(accepted_fields[index], line.c_str(), std::strlen(accepted_fields[index])))
		++index;

	if (index == MAX_ACCEPTED_FIELDS)
	{
		logMessage("Request: Unknown field found", WARNING);
		if (line.find(':') == std::string::npos)
		{
			logMessage("Request: field has no ':'", ERROR);
			return (false);
		}
		this->unknown_fields.push_back(line);
		return (true); // Field not found, and therefore ignored
	}
	else if (index < 4 && !this->elements[index]->empty())
	{
		logMessage("Request: Duplicated field not allowed", ERROR);
		return (false); // Duplicated fields not allowed
	}

	field_pos = line.find(":") + 1;

	field_pos += (line[field_pos] == ' '); // Skip optional leading space

	end_pos = line.length() - 2 - (*(line.end() - 3) == ' ');

	this->elements[index]->assign(
		line.begin() + static_cast<std::ptrdiff_t>(field_pos),	// Start of the value
		line.begin() + static_cast<std::ptrdiff_t>(end_pos)		// End of string
	);
	return (true);
}

bool	Request::unchunk_body(const std::string &input, size_t index)
{
	size_t	end = input.rfind("0\r\n\r\n");

	if (end == std::string::npos)	// No end found
	{
		logMessage("Request: POST: Chunked: Not end found", ERROR);
		return (false);
	}
	if (end + 5 != input.size())	// There is content after the end
	{
		logMessage("Request: POST: Chunked: Unexpected content after the end", ERROR);
		return (false);
	}

	size_t	chunk_size = 0;
	size_t	separator;
	char	*err;

	while (index < end)
	{
		separator = input.find("\r\n", index);

		chunk_size = std::strtoul(input.c_str() + index, &err, 16);

		index += static_cast<size_t>(err - index - input.c_str()) + 2;

		if (index != separator + 2)	// invalid characters
		{
			logMessage("Request: POST: Chunked: Invalid character found", ERROR);
			return (false);
		}
		if (errno == ERANGE) // Overflow
		{
			logMessage("Request: POST: Chunked: Overflow", ERROR);
			return (false);
		}
		if (input.length() - index <= chunk_size) // Remaining string is smaller than expected
		{
			logMessage("Request: POST: Chunked: Remaining string is smaller than expected", ERROR);
			return (false);
		}
		if (input.compare(index + chunk_size, 2, "\r\n") != 0)	// Chunk content does not ends with \r\n
		{
			logMessage("Request: POST: Chunked: Chunk content does not ends with the expected separator", ERROR);
			return (false);
		}

		this->body.append(input, index, chunk_size);
		index += chunk_size + 2;
	}
	return (true);
}

t_code	Request::verify_multipart( void ) const
{
	return (0);
}

t_code	Request::post_method(const std::string &input, size_t index)
{
	logMessage("Request: Managing POST...", INFO);
	if ((this->content_length.empty() && this->transfer_encoding.empty()))
	{
		logMessage("Request: POST: Missing length in non-chunked request", ERROR);
		return (LENGTH_REQUIRED_CODE);
	}

	bool multipart = Compare_insensitive(this->content_type.c_str(), "multipart/", 10);
	if (multipart)
	{
		size_t	boundary = this->content_type.find("boundary=");
		if (boundary == std::string::npos || !this->content_type[boundary + 9])
		{
			logMessage("Request: POST: Missing boundary for multipart request", ERROR);
			return (BAD_REQUEST_CODE);
		}
	}

	if (Compare_insensitive(this->transfer_encoding.c_str(), "chunked", 7))
	{
		logMessage("Request: POST: Unchunking body...", INFO);
		if (!Request::unchunk_body(input, index))
		{
			logMessage("Request: POST: Chunked: Malformed body", ERROR);
			return (BAD_REQUEST_CODE);
		}
		logMessage("Request: POST: Body unchunked", SUCCESS);
	}
	else
	{
		logMessage("Request: POST: Extracting body...", INFO);
		this->body.append(input, index);
		char *err;
		size_t length = strtoul(this->content_length.c_str(), &err, 10);

		if (this->content_length.length() == 0 || err[0] != '\0')
		{
			logMessage("Request: POST: Bad Content-Length", ERROR);
			return (BAD_REQUEST_CODE);
		}

		if (length != this->body.size())
		{
			logMessage("Request: POST: Content-Length mismatch", ERROR);
			std::cerr << RED << "Expected: " << length << RESET << std::endl;
			std::cerr << RED << "Sent:" << this->body.length() << RESET << std::endl; 

			if (length < this->body.size())
				return (BAD_REQUEST_CODE);
			return (REQUEST_TIMEOUT_CODE);
		}
	}
	if (multipart)
		return (verify_multipart());
	return (0);
}

t_code	Request::get_info(const std::string &input)
{
	size_t	index = input.find("\r\n");
	size_t	start;
	bool	end_request = false;
	int		err;

	if (index == std::string::npos)
		return (BAD_REQUEST_CODE);
	
	index += 2;
	std::string	line(input, 0, index);

	logMessage("Request: Analysing first line...", INFO);
	err = Request::analyse_first_line(line);

	switch (err)
	{
		case 1:
			logMessage("Request: Malformed header", ERROR);
			return (BAD_REQUEST_CODE);
		case 2:
			logMessage("Request: Method not implemented", ERROR);
			return (NOT_IMPLEMENTED_CODE);
	}

	logMessage("Request: Analysing header...", INFO);
	while (!end_request)
	{
		start = index;
		index = input.find("\r\n", index);
		if (index == std::string::npos)
			break ; // Bad request
		index += 2; // Move to next line
		if (index - start == 2)
			end_request = true;
		else
		{
			line.assign(input, start, index - start);
			if (!Request::analyse_line(line))
				return (BAD_REQUEST_CODE);
		}
	}
	logMessage("Request: Header parsed", SUCCESS);
	if (!end_request)
	{
		logMessage("Request: Malformed header", ERROR);
		return (BAD_REQUEST_CODE);
	}
	if (!Request::enough_info())
	{
		logMessage("Request: Header has not enough information", ERROR);
		return (BAD_REQUEST_CODE);
	}
	else if (this->http_version.compare("HTTP/1.1") != 0)
	{
		logMessage("Request: Unsupported HTTP version", ERROR);
		return (VERSION_NOT_SUPPORTED_CODE);
	}
	if (this->method == HttpPost)
		return (Request::post_method(input, index));
	return (0);
}
