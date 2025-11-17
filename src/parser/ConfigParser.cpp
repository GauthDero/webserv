/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 22:49:35 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/15 05:15:41 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "BlockInstruction.hpp"
#include "SingleInstruction.hpp"
#include "Verify.hpp"
#include "ParserUtils.hpp"
#include "Parser.hpp"
#include "server.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <stack>

ConfigParser::ConfigParser ( void ) {}

ConfigParser::~ConfigParser ( void ) {}

ConfigParser::ConfigParser(std::istream &input)
{
	this->tokens = tokenize(input);
	this->_defaultSingle.reserve(N_SINGLE_INSTRUCTIONS);
	this->_defaultBlock.reserve(N_BLOCK_INSTRUCTIONS);
	createInstructions();
}

std::deque<Token>	&ConfigParser::getTokens( void )
{
	return (this->tokens);
}

std::map<std::string, AInstruction *>	&ConfigParser::getDefaultInstructions( void )
{
	return (this->_defaultInstructions);
}

WebServ	ConfigParser::parseTokens( void )
{
	return (parseTokens(this->tokens));
}

std::deque<Token>	ConfigParser::tokenize(std::istream &input)
{
	std::deque<Token>	tokens;
	std::string			line;

	if (!input)
		throw std::ios_base::failure("Input stream is not open");
	while (std::getline(input, line))
		tokenizeLine(tokens, line);
	if (input.bad())
		throw std::ios_base::failure(systemError("Config file: Reading input"));
	else if (input.fail() && !input.eof())
		throw std::ios_base::failure(systemError("Config file: Parsing error"));
	return (tokens);
}

void	ConfigParser::createInstructions( void )
{
	Blocks(_defaultBlock);
	Singles(_defaultSingle);

	_defaultInstructions[INS_HTTP]					= &_defaultBlock[0];
	_defaultInstructions[INS_SERVER]				= &_defaultBlock[1];
	_defaultInstructions[INS_LOCATION]				= &_defaultBlock[2];
	_defaultInstructions[INS_CGI]					= &_defaultBlock[3];
	_defaultInstructions[INS_TYPES]					= &_defaultBlock[4];

	_defaultInstructions[INS_LISTEN]				= &_defaultSingle[0];
	_defaultInstructions[INS_ROOT]					= &_defaultSingle[1];
	_defaultInstructions[INS_ALIAS]					= &_defaultSingle[2];
	_defaultInstructions[INS_INDEX]					= &_defaultSingle[3];
	_defaultInstructions[INS_ERROR_PAGE]			= &_defaultSingle[4];
	_defaultInstructions[INS_AUTOINDEX]				= &_defaultSingle[5];
	_defaultInstructions[INS_CLIENT_MAX_BODY_SIZE]	= &_defaultSingle[6];
	_defaultInstructions[INS_RETURN]				= &_defaultSingle[7];
	_defaultInstructions[INS_DEFAULT_TYPE]			= &_defaultSingle[8];
	_defaultInstructions[INS_LIMIT_EXCEPT]			= &_defaultSingle[9];
	_defaultInstructions[INS_PASS]					= &_defaultSingle[10];
	_defaultInstructions[INS_USE_SHEBANG]			= &_defaultSingle[11];
	_defaultInstructions[INS_EXECUTION_TIMEOUT]		= &_defaultSingle[12];
}

void	ConfigParser::verifyTokens( void )
{
	verifyTokens(this->tokens, this->_defaultInstructions);
}

void	ConfigParser::verifyTokens(std::deque<Token> &tokens,
	std::map<std::string, AInstruction *> &defInstructions)
{
	std::deque<Token>::iterator	it = tokens.begin();
	std::stack<BlockInstruction *> context;
	t_path_type	path = None;

	while (it != tokens.end())
	{
		if (it->getString() == INS_ROOT)
		{
			if (path == Alias)
				throw std::runtime_error("Config file: 'root' instruction is incompatible with 'alias' instruction");
			path = Root;
		}
		else if (it->getString() == INS_ALIAS)
		{
			if (path == Root)
				throw std::runtime_error("Config file: 'root' instruction is incompatible with 'alias' instruction");
			path = Alias;
		}
		if (it->getString() == "}")
		{
			if (context.size() == 0)
				throw std::runtime_error("Config file: Unexpected '}'");
			context.pop();
			path = None;
			++it;
		}
		else if (it->getString() == ";")
		{
			it->setType(Separator);
			++it;
		}
		else
			verifyInstruction(tokens, defInstructions, it, context);
	}
	if (context.size())
		throw std::runtime_error("Config file: Unclosed block instruction");
}

WebServ	ConfigParser::parseTokens(std::deque<Token> &tokens)
{
	std::deque<Token>::iterator	it = tokens.begin();
	WebServ						ws;
	bool						foundHttp = false;

	while (it != tokens.end())
	{
		if (it->getString() == INS_HTTP)
		{
			if (foundHttp)
				throw std::runtime_error("Config file: Duplicated http block");
			foundHttp = true;
			ws.setHttpBlock(parseHttp(++it));
		}
		else
			throw std::runtime_error(
				stringError("Config file: Main context: invalid instruction", it->getString().c_str()));
	}
	if (!foundHttp)
		throw std::runtime_error("Config file: Http block not found");
	return (ws);
}
