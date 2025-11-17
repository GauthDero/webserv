/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Verify.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:38:08 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/15 04:35:04 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <cstring>

#define BOOL_TRUE	"on"
#define BOOL_FALSE	"off"

#define HTTP_GET	"GET"
#define HTTP_POST	"POST"
#define HTTP_DELETE	"DELETE"
#define HTTP_HEAD	"HEAD"

// Size unities. case sensitive
#define KB_ID		"k"
#define MB_ID		"M"
#define GB_ID		"G"

#define KB_SIZE		1000
#define MB_SIZE		1000000
#define GB_SIZE		1000000000

enum DataType
{
	Unknown		= 0,
	Instruction	= 1,
	Separator	= 1 << 1,	// ;
	Integer		= 1 << 2,
	UInteger	= 1 << 3,
	Bool		= 1 << 4,
	Text		= 1 << 5,
	IPv4		= 1 << 6,	// 0.0.0.0
	Port		= 1 << 7,	// uint16
	IPPort		= 1 << 8,	// IP:Port
	IPMask		= 1 << 9,	// Access, Deny instructions. Not Implemented
	HTTPMethod	= 1 << 10,	// GET, POST, DELETE
	FileSize	= 1 << 11,	// k, M, G
	MimeType	= 1 << 12,	// type/subtype
	Time		= 1 << 13,	// uint
	HTTPCode	= 1 << 14	// uint16	(100 - 599)
};

struct	t_string_parse
{
	const char	*ptr;
	size_t		pos;
};

class Verify
{
	private:
	Verify ( void );
	Verify (const Verify &v);
	~Verify ( void );

	public:
	template<typename T>
	static bool		verifyInteger(t_string_parse &str, T max, bool end);

	template<typename T>
	static bool		verifyUInteger(t_string_parse &str, T max, bool end);

	static bool		verifyText(t_string_parse &str);
	static bool		verifyBool(t_string_parse &str, bool end);
	static bool		verifyIPv4(t_string_parse &str, bool end);
	static bool		verifyIPPort(t_string_parse &str, bool end);
	static bool 	verifyIPMask(t_string_parse &str, bool end);
	static bool 	verifyHTTPMethod(t_string_parse &str, bool end);
	static bool 	verifyHTTPCode(t_string_parse &str, bool end);
	static bool		verifyFileSize(t_string_parse &str);
	static bool		verifyMimeType(t_string_parse &str);

	static bool		verifyMimeType(const char *str);
	static DataType	verifyString(const std::string &str, const int acceptedTypes);
};

#include "Verify.tpp"
