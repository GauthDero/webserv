/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCodes.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 19:15:27 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/11 22:19:40 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CommonTypes.hpp"
#include <string>

#define	OK_CODE						200
#define	CREATED_CODE				201
#define	NO_CONTENT_CODE				204

#define	MOVED_PERMANENTLY_CODE		301
#define	FOUND_CODE					302

#define	BAD_REQUEST_CODE			400
#define	FORBIDDEN_CODE				403
#define	NOT_FOUND_CODE				404
#define	METHOD_NOT_ALLOWED_CODE		405
#define	REQUEST_TIMEOUT_CODE		408
#define	CONFLICT_CODE				409
#define	LENGTH_REQUIRED_CODE		411
#define	PAYLOAD_TOO_LARGE_CODE		413
#define	URI_TOO_LONG_CODE			414
#define	UNSUPPORTED_MEDIA_TYPE_CODE	415

#define	INTERNAL_SERVER_CODE		500
#define	NOT_IMPLEMENTED_CODE		501
#define	BAD_GATEWAY_CODE			502
#define	SERVICE_UNAVAILABLE_CODE	503
#define	GATEWAY_TIMEOUT_CODE		504
#define	VERSION_NOT_SUPPORTED_CODE	505
#define	INSUFFICIENT_STORAGE_CODE	507
#define	LOOP_DETECTED_CODE			508

#define	DEFAULT_ERROR_PAGE(code, message) \
\
"<!DOCTYPE html>\n\
<html>\n\
	<head>\n\
		<title>Error " code "</title>\n\
	</head>\n\
	<body>\n\
		<hl1>" code " Error</hl1>\n\
		<p>" message ".</p>\n\
	</body>\n\
</html>"

const std::string	Get_status_header(t_code status_code);
t_code				Get_status_errno( void );
