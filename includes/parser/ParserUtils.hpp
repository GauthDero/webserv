/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 23:36:38 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/07 18:05:22 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Token.hpp"
#include "SingleInstruction.hpp"
#include "BlockInstruction.hpp"

#include <deque>
#include <map>
#include <stack>

bool	isQuotes(char c);
bool	isTokenizableChar(char c);

void	Blocks(std::vector<BlockInstruction> &defaultBlock);
void	Singles(std::vector<SingleInstruction> &defaultSingle);
void	tokenizeLine(std::deque<Token> &list, std::string &line);

void	verifyInstruction(std::deque<Token> &tokens,
	std::map<std::string, AInstruction *> &defInstructions,
	std::deque<Token>::iterator	&it,
	std::stack<BlockInstruction *> &context);

void	verifySingle(std::map<std::string, AInstruction *>::iterator &instruction,
	std::deque<Token> &tokens,
	std::deque<Token>::iterator	&it,
	std::stack<BlockInstruction *> &context);

void	verifyBlock(std::map<std::string, AInstruction *>::iterator &instruction,
	std::deque<Token> &tokens,
	std::deque<Token>::iterator	&it,
	std::stack<BlockInstruction *> &context);

void	verifyMimeTypeExtensions(std::deque<Token> &tokens, std::deque<Token>::iterator &it);
