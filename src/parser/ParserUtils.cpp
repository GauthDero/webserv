/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 23:37:07 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/15 05:16:05 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "ParserUtils.hpp"
#include "FatalException.hpp"
#include "BlockInstruction.hpp"
#include "server.hpp"

#include <iostream>

#define	COMMENT_CHAR	'#'
#define MAX_CHILDREN	12

bool	isQuotes(char c)
{
	return (c == '\'' || c == '\"');
}

bool	isTokenizableChar(char c)
{
	return (c == ';' || c == '{' || c == '}' || isQuotes(c));
}

/*
	1. Skip white spaces
	2. if its the end of the line, end.
	3. verify if has quotes.
	3.1 if has quotes, moves the pointer to the next char
	4. Pass through the string as long as the following rules are satisfied:
		4.1 It is amongs the limit of the string
		4.2 If has quotes and the char is not the same as the quote
		4.3 If has not quotes and the char is neither a whitespace nor a tokenizable char -> {};"'
	5. if has quotes and they are not closed or the next char is not a whitespace,
		or has no quotes but it finds a quote chat, it is an error
	6. if the string is a single quoted tokenizable char and is not a quote or ends in whitespace, adjust the pointer for not including it
	7. Substring for pushing to the list
	8. Repeat from 1 until the end of the line
*/

void	tokenizeLine(std::deque<Token> &list, std::string &line)
{
	char	quotes;
	size_t 	start;
	size_t	limit = line.length();

	for (size_t i = 0; i < limit; i++)
	{
		while (i < limit && std::isspace(line[i]))
			i++;
		if (i == limit || line[i] == COMMENT_CHAR)
			break ;
		quotes = isQuotes(line[i]) ? line[i] : '\0';
		start = i;
		i += quotes != 0;
		while (i < limit && ((quotes && line[i] != quotes)
			|| (!quotes && (!std::isspace(line[i])
			&& !isTokenizableChar(line[i]) && line[i] != COMMENT_CHAR))))
			i++;
		if ((quotes && (i == limit || quotes != line[i]
			|| (i + 1 < limit && (!std::isspace(line[i + 1]))
				&& !isQuotes(line[i + 1]) && !isTokenizableChar(line[i + 1]))))
			|| (!quotes && (i < limit && isQuotes(line[i]))))
			throw std::invalid_argument(stringError("Config file","malformed string", line.c_str()));
		// Not include undesirable characters
		i -= i < limit && !isQuotes(line[i]) && (isTokenizableChar(line[i]) || std::isspace(line[i])) && i - start > 0;
		list.push_back(line.substr(start, i - start + 1));
	}
}

void	Blocks(std::vector<BlockInstruction> &defaultBlock)
{
	const char		*fields[MAX_CHILDREN];
	unsigned int	n_fields = 0;

	// PropertiesSet
	fields[n_fields] = INS_INDEX;
	fields[++n_fields] = INS_AUTOINDEX;
	fields[++n_fields] = INS_TYPES;
	fields[++n_fields] = INS_ROOT;
	fields[++n_fields] = INS_ALIAS;
	fields[++n_fields] = INS_ERROR_PAGE;
	fields[++n_fields] = INS_CLIENT_MAX_BODY_SIZE;
	fields[++n_fields] = INS_DEFAULT_TYPE;
	fields[++n_fields] = INS_CGI;

	// http
	fields[++n_fields] = INS_SERVER;
	defaultBlock.push_back(BlockInstruction(fields, n_fields + 1, BLKMainContext));
	n_fields -= 1;

	// server
	fields[++n_fields] = INS_LOCATION;
	fields[++n_fields] = INS_LISTEN;
	fields[++n_fields] = INS_RETURN;
	defaultBlock.push_back(BlockInstruction(fields, n_fields + 1, BLKDefault));
	n_fields -= 3;

	// location
	fields[++n_fields] = INS_RETURN;
	fields[++n_fields] = INS_LIMIT_EXCEPT;
	defaultBlock.push_back(BlockInstruction(fields, n_fields + 1, BLKDefault, 1, Text));
	n_fields = 0;

	// cgi
	fields[n_fields] = INS_PASS;
	fields[++n_fields] = INS_USE_SHEBANG;
	fields[++n_fields] = INS_EXECUTION_TIMEOUT;
	defaultBlock.push_back(BlockInstruction(fields, n_fields + 1, BLKDefault, 1, MAX_ARGS, Text));

	//Types
	defaultBlock.push_back(BlockInstruction(NULL, 0, BLKAcceptMimeTypes));
}

void	Singles(std::vector<SingleInstruction> &defaultSingle)
{
	defaultSingle.push_back(SingleInstruction(1, MAX_ARGS, IPv4 | Port | IPPort));	// listen
	defaultSingle.push_back(SingleInstruction(1, Text));							// root
	defaultSingle.push_back(SingleInstruction(1, Text));							// alias
	defaultSingle.push_back(SingleInstruction(1, MAX_ARGS, Text));					// index
	defaultSingle.push_back(SingleInstruction(2, MAX_ARGS, Text | HTTPCode));		// error_page
	defaultSingle.push_back(SingleInstruction(1, Bool));							// autoindex
	defaultSingle.push_back(SingleInstruction(1, FileSize));						// client_max_body_size
	defaultSingle.push_back(SingleInstruction(1, 2, HTTPCode | Text));				// return
	defaultSingle.push_back(SingleInstruction(1, MimeType));						// default_type
	defaultSingle.push_back(SingleInstruction(1, MAX_ARGS, HTTPMethod));			// limit_except
	defaultSingle.push_back(SingleInstruction(1, Text));							// pass
	defaultSingle.push_back(SingleInstruction(1, Bool));							// use_shebang
	defaultSingle.push_back(SingleInstruction(1, Time));							// execution_time
}

bool	verifyErrorPage(std::deque<Token>::iterator	&it)
{
	while (it->getType() == HTTPCode)
		++it;
	return (it->getType() != Text || (++it)->getType() != Separator);
}

bool	verifyReturn(std::deque<Token>::iterator	&it)
{
	if (it->getType() != HTTPCode)
		return (true);
	if (it->getString().length() == 3 && it->getString().at(0) == '3') // 3XX status codes
		return ((++it)->getType() != Text || (++it)->getType() != Separator);
	if ((++it)->getType() == Separator)
		return (false);
	return (it->getType()!= Text || (++it)->getType() != Separator);
}

void	verifyInstruction(std::deque<Token> &tokens, std::map<std::string, AInstruction *> &defInstructions,
	std::deque<Token>::iterator	&it, std::stack<BlockInstruction *> &context)
{
	std::map<std::string, AInstruction *>::iterator instruction = defInstructions.find(it->getString());
	if (instruction == defInstructions.end())
	{
		if (!context.top()->AcceptMimeTypes() || !Verify::verifyMimeType(it->getString().c_str()))
			throw std::invalid_argument(stringError("Config file", it->getString().c_str(), "invalid instruction"));
		it->setType(MimeType);
		verifyMimeTypeExtensions(tokens, it);
	}
	else
	{
		it->setType(Instruction);
		if ((instruction->second)->getType() == ITSingle)
			verifySingle(instruction, tokens, it, context);
		else if ((instruction->second)->getType() == ITBlock)
			verifyBlock(instruction, tokens, it, context);
		else
			throw std::runtime_error(stringError("Config file", it->getString().c_str(), "unknown type instruction")); // Should Never happen
	}
}

void	verifySingle(std::map<std::string, AInstruction *>::iterator &instruction,
	std::deque<Token> &tokens, std::deque<Token>::iterator	&it,
	std::stack<BlockInstruction *> &context)
{
	int							i = 0;
	bool						errorPage;
	bool						ret;
	std::deque<Token>::iterator	start;
	SingleInstruction			*single;
	const char					*name_instruction = it->getString().c_str();

	if (!context.size() || !context.top()->isAllowedInstruction(instruction->first))
		throw std::runtime_error(
			stringError("Config file", name_instruction ,"not allowed in this context"));

	single = dynamic_cast<SingleInstruction *>(instruction->second);
	if (!single)
		throw FatalException("Cast failed");

	errorPage = it->getString() == INS_ERROR_PAGE;
	ret = it->getString() == INS_RETURN;
	++it; // Instruction
	start = it;
	while (it != tokens.end() && it->getString() != ";" && i < single->getMaxArgs())
	{
		it->setType(Verify::verifyString(it->getString(), single->getArgsType()));
		if (it->getType() == Unknown)
			throw std::runtime_error(stringError("Config file", name_instruction, "bad argument"));
		++it;
		++i;
	}

	if (i < single->getMinArgs())
		throw std::runtime_error(stringError("Config file", name_instruction, "not enough arguments"));
	if (it != tokens.end() && i == single->getMaxArgs() && it->getString() != ";")
		throw std::runtime_error(stringError("Config file", name_instruction, "too many arguments"));
	if (it == tokens.end() || it->getString() != ";")
		throw std::runtime_error(stringError("Config file", name_instruction, "missing ';'"));
	
	it->setType(Separator);

	if ((errorPage && verifyErrorPage(start))
		|| (ret && verifyReturn(start)))
		throw std::runtime_error(stringError("Config file", name_instruction, "bad format"));
}

void	verifyBlock(std::map<std::string, AInstruction *>::iterator &instruction,
	std::deque<Token> &tokens,
	std::deque<Token>::iterator	&it,
	std::stack<BlockInstruction *> &context)
{
	BlockInstruction	*block = dynamic_cast<BlockInstruction *>(instruction->second);
	int					i = 0;
	const char			*name_instruction = it->getString().c_str();

	if (!block)
		throw FatalException("Cast failed");
	else if (!context.size() && !block->isAllowedInMainContext())
		throw std::runtime_error(
			stringError("Config file", name_instruction, "not allowed in main context"));
	else if (context.size() && !context.top()->isAllowedInstruction(instruction->first))
		throw std::runtime_error(
			stringError("Config file", name_instruction ,"not allowed in this context"));
	context.push(block);
	++it;
	while (it != tokens.end() && it->getString() != "{" && i < block->getMaxArgs())
	{
		it->setType(Verify::verifyString(it->getString(), block->getArgsType()));
		if (it->getType() == Unknown)
			throw std::runtime_error(stringError("Config file", name_instruction, "bad argument", it->getString().c_str()));
		++it;
		++i;
	}
	if (i < block->getMinArgs())
		throw std::runtime_error(stringError("Config file",name_instruction, "not enough arguments"));
	if (it == tokens.end() || it->getString() != "{")
		throw std::runtime_error(stringError("Config file", name_instruction, "missing '{'"));
	++it;
}

void	verifyMimeTypeExtensions(std::deque<Token> &tokens, std::deque<Token>::iterator &it)
{
	const char	*mime_type = it->getString().c_str();
	++it; // Mime Type
	size_t i = 0;
	while (it != tokens.end() && it->getString() != ";")
	{
		it->setType(Verify::verifyString(it->getString(), Text));
		if (it->getType() == Unknown)
			throw std::runtime_error(
				stringError("Config file", mime_type, it->getString().c_str(), "invalid extension"));
		++it;
		++i;
	}
	if (i == 0)
		throw std::runtime_error(stringError("Config file", mime_type, "mime-type has no extensions"));
}
