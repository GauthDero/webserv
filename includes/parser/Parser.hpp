/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 05:04:15 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/07 18:05:25 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Http.hpp"
#include "Token.hpp"

#include <deque>
#include <stdint.h>

// Blocks
Http						parseHttp(std::deque<Token>::iterator &it);
Server						parseServer(const Server &server, std::deque<Token>::iterator &it);
Location					parseLocation(const Location &parent, std::deque<Token>::iterator &it);
CGI							parseCGI(std::deque<Token>::iterator &it);
Types						parseTypes(std::deque<Token>::iterator &it);

//Instructions
Listen						parseListen(const std::deque<Token>::iterator &it);
Return						parseReturn(std::deque<Token>::iterator &it);
std::vector<Listen>			parseListeners(std::deque<Token>::iterator &it);
std::vector<std::string>	parseStringList(std::deque<Token>::iterator &it);
t_http_method				parseHttpMethodList(std::deque<Token>::iterator &it);

bool	parsePropertiesSet(PropertiesSet &ps, std::deque<Token>::iterator &it);
bool	parseReturnable(Returnable &ret, std::deque<Token>::iterator &it);
void	parseErrorPages(PropertiesSet &ps, std::deque<Token>::iterator &it);
void	parseCGIconfig(PropertiesSet &ps, std::deque<Token>::iterator &it);
void	parseMimeTypes(Types &types, std::deque<Token>::iterator &it);

//DataTypes
t_int						parseInteger(const std::deque<Token>::iterator &it);
t_uint						parseUInteger(const std::deque<Token>::iterator &it);
bool						parseBoolean(const std::deque<Token>::iterator &it);
std::string					parseString(const std::deque<Token>::iterator &it);
uint32_t					parseIP(const std::deque<Token>::iterator &it);
t_port						parsePort(const std::deque<Token>::iterator &it);
t_time						parseTime(const std::deque<Token>::iterator &it);
Listen						parseIPPort(const std::deque<Token>::iterator &it);
t_uint						parseFileSize(const std::deque<Token>::iterator &it);
t_http_method				parseHttpMethod(const std::deque<Token>::iterator &it);
