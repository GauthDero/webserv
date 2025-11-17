/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:33:26 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/15 04:33:52 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "FatalException.hpp"
#include "cgiHandler.hpp"
#include "server.hpp"
#include "Colors.hpp"
#include "Log.hpp"
#include "Utils.hpp"

#define DATE_BUFFER				60
#define FILE_BUFFER 			4096
#define	DEF_NAME_START			"upload_"
#define	SUFFIX_LENGTH			16
#define	GENERATE_NAME_RETRIES	5
#define CHARSET_LENGTH			62

// Header fields ===============================================================

#define	CONTENT_TYPE_STR		"Content-Type:"
#define	CONTENT_LENGTH_STR		"Content-Length:"
#define	CONNECTION_STR			"Connection:"
#define	ALLOW_STR				"Allow:"
#define	LOCATION_STR			"Location:"
#define	SERVER_STR				"Server:"
#define	DATE_STR				"Date:"

#define AUTOINDEX_SCRIPT \
\
"<script>\n\
	function deleteFile(path) {\n\
		if (!confirm('Are you sure you want to delete \"' + path + '\"?')) return;\n\
		fetch(path, {\n\
			method: 'DELETE',\n\
			headers: { 'Content-Type': 'application/x-www-form-urlencoded' },\n\
			body: new URLSearchParams({ filename: path })\n\
		})\n\
		.then(async response => {\n\
			if (response.ok) {\n\
				location.reload();\n\
			} else {\n\
				const html = await response.text();\n\
				document.open();\n\
				document.write(html);\n\
				document.close();\n\
			}\n\
		})\n\
		.catch(error => {\n\
			console.error('Delete error:', error);\n\
			document.body.innerHTML = '<h1>Unexpected error occurred</h1>"\
			"<p>Unable to process your request.</p>';\n\
		});\n\
	}\n\
</script>\n"

// Constructor =================================================================

Response::Response(const Request &request_) :
	server(request_.Get_server()),
	http_version(request_.Get_protocol()),
	request_path(request_.Get_path()),
	boundary(std::strstr(request_.Get_content_type().c_str(), "boundary=")),
	method(request_.Get_method()),
	request(request_)
{
	Response::Initiate_strings();
}

// Destructor ==================================================================

Response::~Response( void ) {}

void	Response::Initiate_strings( void )
{
	this->web_server.assign("webserv");
	this->connection.assign("keep-alive");
}

// Getters =====================================================================

const std::string &Response::Get_web_server( void ) const
{
	return (this->web_server);
}

const std::string &Response::Get_protocol( void ) const
{
	return (this->http_version);
}

const std::string &Response::Get_allow( void ) const
{
	return (this->allow);
}

const std::string &Response::Get_connection( void ) const
{
	return (this->connection);
}

const std::string &Response::Get_status_code( void ) const
{
	return (this->status_code);
}

const std::string &Response::Get_content_type( void ) const
{
	return (this->content_type);
}

const std::string &Response::Get_content_length( void ) const
{
	return (this->content_length);
}

const std::string &Response::Get_body( void ) const
{
	return (this->body);
}

t_http_method Response::Get_method( void ) const
{
	return (this->method);
}

const std::string &Response::Get_location( void ) const
{
	return (this->location);
}

const std::string &Response::Get_path( void ) const
{
	return (this->path);
}

const Server &Response::Get_server( void ) const
{
	return (this->server);
}

// Setters =====================================================================

void	Response::Set_path(const std::string &path_)
{
	this->path = path_;
}

void	Response::Set_alias(const std::string &alias_)
{
	this->alias = alias_;
}

void	Response::Set_body(const std::string &body_)
{
	this->body.assign(body_);
}

void	Response::Set_status_code(const std::string &errorcode_)
{
	this->status_code = errorcode_;
}

void	Response::Set_close_connection( void )
{
	this->connection.assign("close");
}

void	Response::Set_content_type(const char *type)
{
	this->content_type.assign(type);
}

void	Response::Set_content_length( void )
{
	this->content_length.resize(0);
	Append_size_t(this->content_length, this->body.size());
}

// Append functions ============================================================

void	Response::Append_body(const char *buffer, size_t length)
{
	this->body.append(buffer, length);
}

// Local functions =============================================================

const char	*Find_mime_type(const PropertiesSet &ps, const std::string &path)
{
	size_t	pos = path.rfind(".");

	if (pos != std::string::npos)
	{
		const std::map<std::string, std::string> &types = ps.getTypes().getMimeTypes();
		std::map<std::string, std::string>::const_iterator it = types.begin();

		while (it != types.end() && path.compare(pos + 1, std::string::npos, it->first) != 0)
			++it;

		if (it != types.end())
			return (it->second.c_str());
	}
	return (ps.getDefaultType().c_str());
}

const std::string	*Find_extension(const PropertiesSet &ps, const std::string &mime_type)
{
	size_t				count = 0;
	const std::string	*extension;
	for (std::map<std::string, std::string>::const_iterator it = ps.getTypes().getMimeTypes().begin();
		it != ps.getTypes().getMimeTypes().end() && count < 2; ++it)
	{
		if (it->second == mime_type)
		{
			count++;
			extension = &it->first;
		}
	}
	return (count == 1 ? extension : NULL); // If none is found or there are more than 1, return NULL
}

// Does not counts the length of the date
size_t	calculateResponseSize(const Response &res)
{
	static	size_t	fields_length[] = {
		std::strlen(CONTENT_TYPE_STR),
		std::strlen(CONTENT_LENGTH_STR),
		std::strlen(CONNECTION_STR),
		std::strlen(ALLOW_STR),
		std::strlen(LOCATION_STR),
		std::strlen(SERVER_STR),
		std::strlen(DATE_STR),
	};

	size_t	size =	19 											// 5 whitespace + 7 \r\n
		+ res.Get_protocol().length()							// Http/1.1
		+ res.Get_status_code().length()						// Status code
		+ fields_length[0] + res.Get_content_type().length()	// text/application/image....
		+ fields_length[1] + res.Get_content_length().length()
		+ fields_length[2] + res.Get_connection().length()		// Keep-alive/close
		+ fields_length[5] + res.Get_web_server().length()		// Program name
		+ fields_length[6];										// Date (Only field string)

	if (!res.Get_allow().empty())
		size += fields_length[3] + res.Get_allow().length() + 3; // Whitespace + \r\n
	if (!res.Get_location().empty())
		size += fields_length[4] + res.Get_location().length() + 3; // Whitespace + \r\n
	if (res.Get_method() != HttpHead)
		size += res.Get_body().length();
	return (size);
}

std::string	Get_default_error_page(t_uint status_code)
{
	switch (status_code)
	{
		case (BAD_REQUEST_CODE):			return (DEFAULT_ERROR_PAGE("400", "Bad request"));
		case (FORBIDDEN_CODE):				return (DEFAULT_ERROR_PAGE("403", "Forbidden"));
		case (NOT_FOUND_CODE):				return (DEFAULT_ERROR_PAGE("404", "Page not found"));
		case (METHOD_NOT_ALLOWED_CODE):		return (DEFAULT_ERROR_PAGE("405", "Method not allowed"));
		case (REQUEST_TIMEOUT_CODE):		return (DEFAULT_ERROR_PAGE("408", "Request timeout"));
		case (CONFLICT_CODE):				return (DEFAULT_ERROR_PAGE("409", "Conflict"));
		case (LENGTH_REQUIRED_CODE):		return (DEFAULT_ERROR_PAGE("411", "Length Required"));
		case (PAYLOAD_TOO_LARGE_CODE):		return (DEFAULT_ERROR_PAGE("413", "Payload too large"));
		case (URI_TOO_LONG_CODE):			return (DEFAULT_ERROR_PAGE("414", "URI Too Long"));
		case (UNSUPPORTED_MEDIA_TYPE_CODE):	return (DEFAULT_ERROR_PAGE("415", "Unsupported media type"));
		case (NOT_IMPLEMENTED_CODE):		return (DEFAULT_ERROR_PAGE("501", "Not Implemented"));
		case (BAD_GATEWAY_CODE):			return (DEFAULT_ERROR_PAGE("502", "Bad Gateway"));
		case (SERVICE_UNAVAILABLE_CODE):	return (DEFAULT_ERROR_PAGE("503", "Service Unavailable"));
		case (GATEWAY_TIMEOUT_CODE):		return (DEFAULT_ERROR_PAGE("504", "Gateway Timeout"));
		case (VERSION_NOT_SUPPORTED_CODE):	return (DEFAULT_ERROR_PAGE("505", "Version not supported"));
		case (INSUFFICIENT_STORAGE_CODE):	return (DEFAULT_ERROR_PAGE("507", "Insufficient Storage"));
		case (LOOP_DETECTED_CODE):			return (DEFAULT_ERROR_PAGE("508", "Loop Detected"));
		default:							return (DEFAULT_ERROR_PAGE("500", "Internal Server Error"));
	}
}

void	Generate_error_page(Response &res, t_code status_code)
{
	logMessage("Response: Generating default error page...", INFO);
	res.Set_body(Get_default_error_page(status_code));
	res.Set_content_type("text/html");
}

void	serve_file(Response &res, const PropertiesSet &ps, const std::string &path)
{
	logMessage("Response: Serving file...", INFO);

	int fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
		return (res.html_error(ps, Get_status_errno()));
	res.Set_status_code(Get_status_header(OK_CODE));
	res.Set_content_type(Find_mime_type(ps, path.c_str()));
	try
	{
		res.read_file(fd);
	}
	catch (const FatalException &e)
	{
		throw;
	}
	catch (const std::exception &e)
	{
		tryClose(fd);
		throw ;
	}
	logMessage("Response: File served", SUCCESS);
}

const Location *getLocationBlock(const std::vector<Location> &locations, const std::string &path)
{
	const Location *loc = NULL;
	size_t			compare_length;
	size_t			max_compared = 0;
	std::vector<Location>::const_iterator it = locations.begin();

	while (it != locations.end())
	{
		compare_length = std::min(it->getPath().length(), path.length());
		if (max_compared < compare_length && it->getPath().compare(0, compare_length, path, 0, compare_length) == 0)
		{
			max_compared = compare_length;
			loc = &(*it);
		}
		++it;
	}
	return (loc);
}

tm	Get_uc_time( void )
{
	errno = 0;
	time_t	now = time(NULL);

	if (errno)
		throw FatalException(systemError("Response: Date"));

	struct	tm*	gmt = gmtime(&now);

	if (!gmt)
		throw FatalException(systemError("Response: Date"));

	return (*gmt);
}

std::string	Generate_date( void )
{
	logMessage("Response: Generating date...", INFO);

	char		buffer[DATE_BUFFER];
	struct tm	gmt = Get_uc_time();

	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &gmt);

	logMessage("Response: Date generated", SUCCESS);
	return (buffer);
}

// Other methods ===============================================================

std::string	Response::Generate_filename(const PropertiesSet &ps)
{
	logMessage("Response: Generating filename...", INFO);

	char		buffer[DATE_BUFFER];
	struct tm	gmt = Get_uc_time();

	const std::string *extension = Find_extension(ps, this->request.Get_content_type());
	std::string filename(std::strlen(DEF_NAME_START) + SUFFIX_LENGTH + 15
		+ (extension ? extension->length() : 0), '\0');

	filename.assign(DEF_NAME_START);

	snprintf(buffer, DATE_BUFFER, "%04d%02d%02d_%02d%02d%02d",
			gmt.tm_year + 1900,
			gmt.tm_mon + 1,
			gmt.tm_mday,
			gmt.tm_hour,
			gmt.tm_min,
			gmt.tm_sec);

	filename.append(buffer);

	static const char charset[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789";

	for (size_t i = 0; i < SUFFIX_LENGTH; ++i)
		filename += charset[rand() % CHARSET_LENGTH];

	if (extension)
		filename.append(*extension);

	logMessage("Response: Filename generated", SUCCESS);
	return (filename);
}

std::string	Response::Generate_response( void )
{
	size_t		response_size = calculateResponseSize(*this); // Without the date
	std::string	date(Generate_date());
	std::string response(response_size + date.length(), '\0');

	response.resize(0);
	response.append(this->Get_protocol()).append(" ").append(this->status_code).append("\r\n");

	response.append(DATE_STR).append(" ").append(date).append("\r\n");
	response.append(SERVER_STR).append(" ").append(this->web_server).append("\r\n");

	if (!this->allow.empty())
		response.append(ALLOW_STR).append(" ").append(this->allow).append("\r\n");

	response.append(CONNECTION_STR).append(" ").append(this->connection).append("\r\n");

	if (!this->location.empty())
		response.append(LOCATION_STR).append(" ").append(this->location).append("\r\n");

	response.append(CONTENT_TYPE_STR).append(" ").append(this->content_type).append("\r\n");
	response.append(CONTENT_LENGTH_STR).append(" ").append(this->content_length).append("\r\n");

	response.append("\r\n");
	
	if (this->Get_method() != HttpHead)
		response.append(this->Get_body());

	return (response);
}

void	Response::read_file(int fd)
{
	ssize_t	stream;
	char	buffer[FILE_BUFFER];

	logMessage("Response: Reading file...", INFO);
	while ((stream = read(fd, buffer, FILE_BUFFER)) > 0)
		this->body.append(buffer, static_cast<size_t>(stream));
	if (stream == -1)
		throw std::runtime_error(systemError("Response: Error reading file"));
	tryClose(fd);
	Set_content_length();
	logMessage("Response: File read", SUCCESS);
}

void	Response::html_error(const PropertiesSet &ps)
{
	html_error(ps, static_cast<t_code>(strtoul(Get_status_code().c_str(), NULL, 10)));
}

void	Response::html_error(const PropertiesSet &ps, t_code status_code_)
{
	this->Set_close_connection(); //closera d'office si erreur -> a voir s'il faut
	int		fd = -1;
	bool	generate_default = false;
	const std::map<t_code, std::string> &pages = this->server.getErrorPages();
	std::map<t_code, std::string>::const_iterator it = pages.find(status_code_);

	logMessage("Response: Generating error page...", INFO);
	try
	{
		if (it != pages.end())
		{
			fd = open(it->second.c_str(), O_RDONLY);
			if (fd < 0)
				throw std::runtime_error(systemError("Response: Could not open error page"));
			Set_content_type(Find_mime_type(ps, it->second));
			read_file(fd);
		}
		else
		{
			std::cerr << YELLOW << "No error page assigned to the status code " << status_code_ << RESET << std::endl;
			generate_default = true;
		}
	}
	catch(const FatalException& e)
	{
		throw;
	}
	catch(const std::exception& e)
	{
		logMessage(e.what(), ERROR);
		tryClose(fd);
		generate_default = true;
	}
	try
	{
		if (generate_default)
			Generate_error_page(*this, status_code_);
		this->Set_status_code(Get_status_header(status_code_));
		Set_content_length();
		logMessage("Response: Error page generated", SUCCESS);
	}
	catch (const std::exception &e)
	{
		logMessage(stringError("Response: Could not generate error page", e.what()), ERROR);
	}
}

void	Response::generate_autoindex(const PropertiesSet &ps, t_http_method methods)
{
	struct dirent*	entry;
	std::string		name;
	char			*linkname;
	DIR*			dir = opendir(this->path.c_str());

	logMessage("Response: Generating autoindex...", INFO);
	if (!dir)
	{
		logMessage(systemError("Response: Opening directory"), ERROR);
		return (Response::html_error(ps, Get_status_errno()));
	}

	this->body.reserve(2 * this->request_path.length()
		+ 128 + (methods & HttpDelete ? std::strlen(AUTOINDEX_SCRIPT) : 0));

	this->body.assign("<!DOCTYPE html><html><head><title>Index of ").append(this->request_path);
	this->body.append("</title></head><body>\n<h1>Index of ").append(this->request_path).append("\n<pre><hr><table>");

	errno = 0;
	while ((entry = readdir(dir)) && errno == 0)
	{
		linkname = entry->d_name;
		if (!std::strncmp(linkname, ".", 2)) 
			continue;
		name.resize(0);
		name.append(this->request_path).append(*this->request_path.rbegin() != '/' ? "/" : "").append(linkname);
	
		this->body.reserve(this->body.length() + 34 + name.length() + std::strlen(linkname)	// List string
			+ (methods & HttpDelete ? (57 + name.length()): 0));							// Delete button

		this->body.append("<tr><td><a href=\"");
		this->body.append(name).append("\">").append(linkname).append("</a></td>");
		if (methods & HttpDelete && std::strncmp(linkname, "..", 3))
			this->body.append("<td><button onclick=\"deleteFile('").append(name).append("')\">Delete</button></td>");
		this->body.append("</tr>\n");
	}
	
	bool err = (errno != 0);
	if (closedir(dir) == -1 && errno != EBADF)
		throw FatalException(systemError("Could not close directory"));
	else if (err)
		return (Response::html_error(ps, Get_status_errno()));

	this->body.append("</table><hr></pre>");

	if (methods & HttpDelete)
		this->body.append(AUTOINDEX_SCRIPT);

	this->body.append("</body></html>\n");
	
	Set_content_type("text/html");
	Set_content_length();
	this->status_code.assign(Get_status_header(OK_CODE));
	logMessage("Response: Autoindex generated", SUCCESS);
}

void	Response::Http_get_method(const PropertiesSet &ps, t_http_method method_)
{
	if (access(this->path.c_str(), R_OK) != 0)
	{
		logMessage(systemError("Response: Checking access to path"), ERROR);
		return (Response::html_error(ps, Get_status_errno()));
	}
	else if (Response::is_directory())
	{
		for (std::vector<std::string>::const_iterator it = ps.getIndexes().begin();
			it != ps.getIndexes().end(); ++it)
		{
			std::string path_(this->path.length() + it->length() + 1, '\0');

			path_.assign(this->path).append("/").append(*it);
			if (access(path_.c_str(), F_OK | R_OK) != 0)
			{
				if (errno != ENOENT)
					return (Response::html_error(ps, Get_status_errno()));
				logMessage(stringError("Response: Index not found", it->c_str()), WARNING);
			}
			else
			{
				logMessage("Response: Index found!", SUCCESS);
				return (serve_file(*this, ps, path_));
			}
		}
		if (ps.getAutoIndex())
			return (Response::generate_autoindex(ps, method_));
		Response::html_error(ps, NOT_FOUND_CODE);
	}
	else
		serve_file(*this, ps, this->path);
}

void	Response::json_message(t_code status_code_)
{
	const	char		*success[] = {"false", "true"};
	const	std::string message(Get_status_header(status_code_));
			size_t		space = message.find(' ');

	if (space == std::string::npos)
		space = message.length();

	logMessage("Response: Generating JSON response...", INFO);
	this->body.reserve(45 + message.length());
	this->body.resize(0);
	this->body.append("{\"success\": ");
	this->body.append(success[status_code_ < 400]);
	this->body.append(", \"status\": ");
	this->body.append(message, 0, space);
	this->body.append(", \"message\": \"");
	this->body.append(message, space + (space != message.length()));
	this->body.append("\"}");

	Set_content_length();
	if (status_code_ == CREATED_CODE)
		this->location = this->request_path;

	this->status_code.assign(Get_status_header(status_code_));
	Set_content_type("application/json");
}

// Manage and upload all the files in the multipart request.
// if anyone fails, returns error
void	Response::post_multipart( void )
{
	logMessage("Response: POST: Managing multipart request...", INFO);
	std::string filename;
	std::string path_;
	size_t		boundary_len = std::strlen(this->boundary + 9);
	size_t		index = 0;
	size_t		limit;
	size_t		position;
	size_t		position_end;
	ssize_t		chars;
	t_code		code = CREATED_CODE;
	int			fd;
	
	for (limit = this->request.Get_body().find(this->boundary + 9, index, boundary_len);
		limit != std::string::npos && this->request.Get_body().compare(limit + boundary_len, 2, "--") != 0;)
	{
		position = this->request.Get_body().find("filename=", limit + boundary_len) + 10;
		position_end = this->request.Get_body().find('"', position);
		filename.assign(this->request.Get_body(), position, position_end - position);

		logMessage(stringError("Response: Uploading file", filename.c_str()), INFO);

		path_.reserve(this->path.length() + 1 + filename.length());
		path_.assign(this->path).append("/").append(filename);
		index = this->request.Get_body().find("\r\n\r\n", limit) + 4;
		limit = this->request.Get_body().find(this->boundary + 9, index, boundary_len);
		fd = open(path_.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0755);

		if (fd < 0) // already exist or error
		{
			logMessage(stringError("Response: POST", filename.c_str(), strerror(errno)), ERROR);
			code = errno == EAGAIN ? CONFLICT_CODE : Get_status_errno();
			continue ;
		}

		chars = write(fd, this->request.Get_body().c_str() + index, limit - index - 4); // \r\n--

		if (chars < 0 || static_cast<size_t>(chars) != limit - index - 4) // Partially or not written at all
		{
			logMessage(systemError("Response: Could not upload the requested file"), ERROR);
			if (std::remove(path_.c_str()) != 0)
				throw FatalException(systemError("Response: File could not be deleted"));
			tryClose(fd);
			code = Get_status_errno();
			continue ;
		}

		logMessage("Response: File uploaded", SUCCESS);
		tryClose(fd);
	}
	json_message(code);
}

void	Response::Http_post_method(const PropertiesSet& ps)
{
	logMessage("Response: POST: Executing static request...", INFO);

	if (access(this->path.c_str(), F_OK | W_OK) != 0) // Files can be placed in the directory
	{
		logMessage(systemError("Response: File cannot be uploaded to the specified directory"), ERROR);
		return (Response::html_error(ps, Get_status_errno()));
	}

	if (Compare_insensitive(this->request.Get_content_type().c_str(), "multipart/", 10))
		return (Response::post_multipart());

	logMessage("Response: Managing single file...", INFO);

	std::string	filename(Generate_filename(ps));
	std::string	path_(this->path.length() + filename.length() + 1, '\0');
	size_t		tries;
	ssize_t		chars;
	int			fd;

	path_.resize(0);
	path_.assign(this->path).append("/").append(filename);

	errno = 0;
	fd = open(path_.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0755);

	for (tries = 0; tries < GENERATE_NAME_RETRIES && fd < 0 && errno == EEXIST; ++tries) // Retry in case the file already exist
	{
		filename.assign(Generate_filename(ps));
		path_.resize(this->path.length() + 1);
		path_.append(filename);
		fd = open(path_.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0755);
	}

	if (fd < 0 && errno != EEXIST)
		throw std::runtime_error("Response: POST: Generating file name");
	else if (tries == GENERATE_NAME_RETRIES)
	{
		logMessage("Response: POST: Max tries for generate file name exausted", ERROR);
		return (html_error(ps, INTERNAL_SERVER_CODE));
	}

	chars = write(fd, this->request.Get_body().c_str(), this->request.Get_body().length());

	if (chars < 0 || static_cast<size_t>(chars) != this->request.Get_body().length()) // Partially or not written at all
	{
		logMessage(systemError("Response: Could not upload the requested file"), ERROR);
		if (std::remove(path_.c_str()) != 0)
			throw FatalException(systemError("Response: File could not be deleted"));
		tryClose(fd);
		return (Response::html_error(ps, Get_status_errno()));
	}

	logMessage("Response: File uploaded", SUCCESS);
	tryClose(fd);
	json_message(CREATED_CODE);
}

void	Response::manage_request(const PropertiesSet& ps, const Connection &conn, t_http_method methods)
{
	if (this->request.Get_body().length() > ps.getClientMaxBodySize())
	{
		logMessage("Request: POST/PUT: Body exceeds the maximun length", ERROR);
		return (Response::html_error(ps, PAYLOAD_TOO_LARGE_CODE));
	}

	if (try_cgi(ps, conn)) // Verifies if the request may be satisfied with the CGI
		return ;

	if (this->Get_method() == HttpGet && methods & HttpGet)
	{
		logMessage("Response: Executing GET...", INFO);
		Response::Http_get_method(ps, methods);
	}
	else if (this->Get_method() == HttpHead && methods & HttpHead)
	{
		logMessage("Response: Executing HEAD...", INFO);
		Response::Http_get_method(ps, methods);
	}
	else if (this->Get_method() == HttpPost && methods & HttpPost)
		Response::Http_post_method(ps);
	//else if (this->Get_method() == HttpPut && methods & HttpPut)
	//	Response::Http_put_method(ps);
	else if (this->Get_method() == HttpDelete && methods & HttpDelete)
		Response::Http_delete_method(ps);
	else
		Response::html_error(ps, METHOD_NOT_ALLOWED_CODE);
}

// may check the path from the body instead of the URI
void	Response::Http_delete_method(const PropertiesSet &ps)
{
	logMessage("Response: Executing DELETE...", INFO);

	if (access(this->path.c_str(), F_OK) != 0)
	{
		logMessage(systemError("Response: Checking requested file"), ERROR);
		return (Response::html_error(ps, Get_status_errno()));
	}

	std::string directory(this->path, 0, this->path.rfind('/'));

	if (access(directory.c_str(), W_OK | X_OK) != 0)
	{
		logMessage(systemError("Response: Checking directory"), ERROR);
		return (Response::html_error(ps, Get_status_errno()));
	}
	if (std::remove(this->path.c_str()) == -1)
	{
		logMessage(systemError("Response: Could not remove file"), ERROR);
		return (Response::html_error(ps, Get_status_errno()));
	}

	logMessage("Response: File removed", SUCCESS);
	this->status_code.assign(Get_status_header(NO_CONTENT_CODE));
}

void	Response::redirection(const Returnable *ret)
{
	logMessage("Response: Redirecting...", INFO);
	this->Set_status_code(Get_status_header(ret->getReturn().getStatusCode()));
	if (ret->getReturn().getStatusCode() > 299
		&& ret->getReturn().getStatusCode() < 400)
	{
		this->location = ret->getReturn().getString();
		Set_body("");
	}
	else
		Set_body(ret->getReturn().getString());
	Set_content_type("text/html");
	Set_content_length();
}

void	Response::set_path(const Location *location_conf, const PropertiesSet& ps)
{
	size_t resource_pos = location_conf && ps.getPathType() == Alias ?
		this->request_path.find('/', location_conf->getPath().length() - (*(this->request_path.end() - 1) == '/')) : 0;

	resource_pos = resource_pos == std::string::npos ? this->request_path.length() : resource_pos;

	this->path.reserve(ps.getPath().length() + this->request_path.length() - resource_pos);
	this->path.assign(ps.getPath()).append(this->request_path, resource_pos);
}

void	Response::switch_methods(const Connection &conn)
{
	const	Location		*location_conf;
	const	PropertiesSet	*ps;
	const	Returnable		*ret;

	location_conf = getLocationBlock(this->server.getLocations(), this->request_path);

	if (location_conf)
	{
		logMessage("Response: Found location for the requested URI", SUCCESS);
		ps = location_conf;
		ret = location_conf;
	}
	else
	{
		logMessage("Response: No location matches the requested URI", WARNING);
		ps = &this->server;
		ret = &this->server;
	}

	t_http_method methods = (location_conf ? location_conf->getAllowedMethods() :
		static_cast<t_http_method>(HttpGet | HttpPost | HttpDelete | HttpHead));

	try
	{
		this->allow = (location_conf ? location_conf->getHttpMethods() : "GET, POST, DELETE, HEAD");
		if (ret->hasReturn())
			redirection(ret);
		else
		{
			set_path(location_conf, *ps);
			manage_request(*ps, conn, methods);
		}
	}
	catch (const FatalException &e)
	{
		throw;
	}
	catch (const std::exception &e)
	{
		logMessage(e.what(), ERROR);
		Response::html_error(*ps, Get_status_errno());
	}
}

bool	Response::is_directory( void )
{
	struct stat info;

	if (stat(this->path.c_str(), &info) != 0)
	{
		if (errno == ENOTDIR)
			return (false);
		throw std::runtime_error(systemError("Response: Could not retrieve path properties"));
	}
	return (S_ISDIR(info.st_mode));
}
