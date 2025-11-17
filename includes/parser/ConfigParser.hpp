/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:54:51 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/15 05:13:43 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <list>
#include <deque>
#include <map>
#include <vector>

#include "AInstruction.hpp"
#include "SingleInstruction.hpp"
#include "BlockInstruction.hpp"
#include "Token.hpp"
#include "WebServ.hpp"

#define	N_SINGLE_INSTRUCTIONS 		14
#define	N_BLOCK_INSTRUCTIONS 		5
#define	MAX_ARGS					32

// Blocks
#define	INS_SERVER					"server"
#define	INS_HTTP					"http"
#define	INS_LOCATION				"location"
#define	INS_CGI						"cgi"
#define	INS_TYPES					"types"

// PropertiesSet
#define	INS_ROOT					"root"
#define	INS_ERROR_PAGE				"error_page"
#define INS_CLIENT_MAX_BODY_SIZE	"client_max_body_size"
#define	INS_ALIAS					"alias"
#define	INS_INDEX					"index"
#define	INS_AUTOINDEX				"autoindex"
#define	INS_DEFAULT_TYPE			"default_type"

// CGI
#define	INS_PASS					"pass"
#define	INS_USE_SHEBANG				"use_shebang"
#define INS_EXECUTION_TIMEOUT		"execution_timeout"

// Others
#define	INS_RETURN					"return"
#define	INS_LISTEN					"listen"
#define	INS_LIMIT_EXCEPT			"limit_except"

class ConfigParser
{
	private:
	std::deque<Token>	tokens;
	std::map<std::string, AInstruction *>	_defaultInstructions;
	std::vector<SingleInstruction>			_defaultSingle;
	std::vector<BlockInstruction>			_defaultBlock;

	ConfigParser ( void );

	void createInstructions( void );

	public:
	~ConfigParser( void );
	ConfigParser(std::istream &input);

	void	verifyTokens( void );
	WebServ	parseTokens( void );
	
	static std::deque<Token>			tokenize(std::istream &input);

	static void	verifyTokens(std::deque<Token> &tokens,
		std::map<std::string, AInstruction *> &defInstructions);

	static	WebServ	parseTokens(std::deque<Token> &tokens);

	std::map<std::string, AInstruction *>	&getDefaultInstructions( void );
	std::deque<Token>						&getTokens( void );
};
