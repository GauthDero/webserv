/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 16:51:34 by gdero             #+#    #+#             */
/*   Updated: 2025/11/14 20:03:41 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"
#include "Colors.hpp"
#include "Utils.hpp"
#include "Log.hpp"
#include "FatalException.hpp"
#include "server.hpp"

std::string	methods(const std::string &input, const Connection & conn)
{
	std::cout << BLUE << "Request" << RESET << std::endl;
	printLimit(std::cout, input, "Server", 100000);

	logMessage("Server: Parsing Request...", INFO);

	Request	request(conn._server._serv);
	t_code	error_code = request.get_info(input);

	if (error_code)
	{
		logMessage("Error while parsing the request", ERROR);
		request.Set_protocol("HTTP/1.1"); // Make sure it has the right protocol
	}
	else
		logMessage("Server: Request parsed", SUCCESS);

	Response response(request);

	logMessage("Server: Executing Request...", INFO);

	if (error_code)
		response.html_error(response.Get_server(), error_code);
	else
		response.switch_methods(conn);

	logMessage("Server: Generating Response...", INFO);

	std::string	http_response(response.Generate_response());

	logMessage("Server: Response generated", SUCCESS);

	std::cout << BLUE << "Response" << RESET << std::endl;
	printLimit(std::cout, http_response, "Server", 100000);
	return (http_response);
}

t_code	Get_status_errno( void )
{
	switch (errno)
	{
		case (ENOTDIR):
			return (BAD_REQUEST_CODE);
		case (ENOENT):
			return (NOT_FOUND_CODE);
		case (ENAMETOOLONG):
			return (URI_TOO_LONG_CODE);
		case (EACCES):
			return (FORBIDDEN_CODE);
		case (ELOOP):
			return (LOOP_DETECTED_CODE);
		case (ETIMEDOUT):
			return (GATEWAY_TIMEOUT_CODE);
		case (ENOTEMPTY):
			// -fallthrough
		case (EEXIST):
			return (CONFLICT_CODE);
		case (EMFILE):
			// -fallthrough
		case (EAGAIN):
			return (SERVICE_UNAVAILABLE_CODE);
		default:
			return (INTERNAL_SERVER_CODE);
	}
}

// Only the status code is needed. The message is optional
const std::string	Get_status_header(t_code status_code)
{
	switch(status_code)
	{
		case (OK_CODE):						return ("200 OK");
		case (CREATED_CODE):				return ("201 Created");
		case (NO_CONTENT_CODE):				return ("204 No Content");
		case (MOVED_PERMANENTLY_CODE):		return ("301 Moved Permanently");
		case (FOUND_CODE):					return ("302 Found");
		case (BAD_REQUEST_CODE):			return ("400 Bad Request");
		case (FORBIDDEN_CODE):				return ("403 Forbidden");
		case (NOT_FOUND_CODE):				return ("404 Not Found");
		case (METHOD_NOT_ALLOWED_CODE):		return ("405 Method Not Allowed");
		case (REQUEST_TIMEOUT_CODE):		return ("408 Request Timeout");
		case (CONFLICT_CODE):				return ("409 Conflict");
		case (LENGTH_REQUIRED_CODE):		return ("411 Length Required");
		case (PAYLOAD_TOO_LARGE_CODE):		return ("413 Payload Too Large");
		case (URI_TOO_LONG_CODE):			return ("414 URI Too Long");
		case (UNSUPPORTED_MEDIA_TYPE_CODE):	return ("415 Unsupported Media Type");
		case (INTERNAL_SERVER_CODE):		return ("500 Internal Server Error");
		case (NOT_IMPLEMENTED_CODE):		return ("501 Not Implemented");
		case (BAD_GATEWAY_CODE):			return ("502 Bad Gateway");
		case (SERVICE_UNAVAILABLE_CODE):	return ("503 Service Unavailable");
		case (GATEWAY_TIMEOUT_CODE):		return ("504 Gateway Timeout");
		case (VERSION_NOT_SUPPORTED_CODE):	return ("505 HTTP Version Not Supported");
		case (INSUFFICIENT_STORAGE_CODE):	return ("507 Insufficient Storage");
		case (LOOP_DETECTED_CODE):			return ("508 Loop Detected");
		default: // Hope never have to use it
		{
			std::string str;
			Append_size_t(str, status_code);
			return (str);
		}
	}
}
