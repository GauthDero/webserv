/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Verify.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:37:53 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/15 04:35:29 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Verify.hpp"
#include "CommonTypes.hpp"

#include <iostream>
#include <limits>
#include <cstdlib>

Verify::Verify ( void ) {}

Verify::Verify (const Verify &v)
{
	(void) v;
}

Verify::~Verify ( void ) {}

bool	compareCStrings(t_string_parse &str, const char *str2, bool end)
{
	if (!strncmp(&str.ptr[str.pos], str2, strlen(str2) + end))
	{
		str.pos += strlen(str2);
		return (true);
	}
	return (false);
}

bool	Verify::verifyText(t_string_parse &str)
{
	while (std::isprint(str.ptr[str.pos]))
		++str.pos;
	return (!str.ptr[str.pos]);
}

bool	Verify::verifyIPv4(t_string_parse &str, bool end)
{
	bool	isValid = true;
	int		i = 0;
	size_t	oldPos = str.pos;

	while (i < 4 && isValid)
	{
		isValid = verifyUInteger<unsigned char>(str, std::numeric_limits<unsigned char>::max(), false)
			&& (i == 3 || str.ptr[str.pos] == '.');
		str.pos += (i < 3);
		++i;
	}
	if (isValid && (!end || !str.ptr[str.pos]))
		return (true);
	str.pos = oldPos;
	return (false);
}

bool	Verify::verifyIPPort(t_string_parse &str, bool end)
{
	size_t	oldPos = str.pos;

	if (verifyIPv4(str, false) && str.ptr[str.pos] == ':')
	{
		++str.pos;
		if (verifyUInteger<t_port>(str, std::numeric_limits<t_port>::max(), end))
			return (true);
	}
	str.pos = oldPos;
	return (false);
}

bool	Verify::verifyHTTPCode(t_string_parse &str, bool end)
{
	if (std::strlen(str.ptr) != 3 || !verifyUInteger<t_code>(str, 599, end))
		return (false);
	size_t	oldPos = str.pos;
	if (std::strtoul(str.ptr, NULL, 10) > 99)
		return (true);
	str.pos = oldPos;
	return (false);
}

bool	Verify::verifyIPMask(t_string_parse &str, bool end)
{
	size_t	oldPos = str.pos;

	if (verifyIPv4(str, false) && str.ptr[str.pos] == '/')
	{
		++str.pos;
		if (verifyUInteger<unsigned char>(str, 32, end))
		return (true);
	}
	str.pos = oldPos;
	return (false);
}

bool	Verify::verifyFileSize(t_string_parse &str)
{
	size_t	oldPos = str.pos;

	if (!verifyUInteger<t_uint>(str, std::numeric_limits<t_uint>::max(), false))
		return (false);
	else if (compareCStrings(str, "", true))
		return (true);	// No unity specified
	t_uint size = strtoul(&str.ptr[oldPos], NULL, 10);
	if (compareCStrings(str, KB_ID, true))
		return (!__builtin_mul_overflow(size, KB_SIZE, &size));
	else if (compareCStrings(str, MB_ID, true))
		return (!__builtin_mul_overflow(size, MB_SIZE, &size));
	else if (compareCStrings(str, GB_ID, true))
		return (!__builtin_mul_overflow(size, GB_SIZE, &size));
	str.pos = oldPos;
	return (false); //Unknown type
}

bool	Verify::verifyHTTPMethod(t_string_parse &str, bool end)
{
	return (compareCStrings(str, HTTP_GET, end)
			|| compareCStrings(str, HTTP_POST, end)
			|| compareCStrings(str, HTTP_DELETE, end)
			|| compareCStrings(str, HTTP_HEAD, end));
}

bool	Verify::verifyBool(t_string_parse &str, bool end)
{
	return (compareCStrings(str, BOOL_TRUE, end) || compareCStrings(str, BOOL_FALSE, end));
}

bool	Verify::verifyMimeType(t_string_parse &str)
{
	return (Verify::verifyMimeType(&str.ptr[str.pos]));
}

bool	Verify::verifyMimeType(const char *str)
{
	const char *slash = strchr(str, '/');
	return (slash && slash != str && *(slash + 1) && !strchr(slash + 1, '/')); // Regex: .+/.+
}

DataType	Verify::verifyString(const std::string &str, const int acceptedTypes)
{
	t_string_parse	sp;

	sp.ptr = str.c_str();
	sp.pos = 0;
	if (acceptedTypes & FileSize && Verify::verifyFileSize(sp))
		return (FileSize);
	else if (acceptedTypes & Integer	&& Verify::verifyInteger<t_int>(sp, std::numeric_limits<t_int>::max(), true))
		return (Integer);
	else if (acceptedTypes & Port		&& Verify::verifyUInteger<t_port>(sp, std::numeric_limits<t_port>::max() , true))
		return (Port);
	else if (acceptedTypes & UInteger	&& Verify::verifyUInteger<t_uint>(sp, std::numeric_limits<t_uint>::max(), true))
		return (UInteger);
	else if (acceptedTypes & Time		&& Verify::verifyUInteger<t_time>(sp, std::numeric_limits<t_time>::max(), true))
		return (Time);
	else if (acceptedTypes & HTTPCode	&& Verify::verifyHTTPCode(sp, true))
		return (HTTPCode);
	else if (acceptedTypes & IPv4		&& Verify::verifyIPv4(sp, true))
		return (IPv4);
	else if (acceptedTypes & IPPort		&& Verify::verifyIPPort(sp, true))
		return (IPPort);
	else if (acceptedTypes & IPMask		&& Verify::verifyIPMask(sp, true))
		return (IPMask);
	else if (acceptedTypes & Bool		&& Verify::verifyBool(sp, true))
		return (Bool);
	else if (acceptedTypes & HTTPMethod	&& Verify::verifyHTTPMethod(sp, true))
		return (HTTPMethod);
	else if (acceptedTypes & MimeType	&& Verify::verifyMimeType(sp))
		return (MimeType);
	else if (acceptedTypes & Text		&& Verify::verifyText(sp))
		return (Text);
	return (Unknown);
}
