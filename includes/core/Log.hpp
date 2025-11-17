/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 22:31:33 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/12 02:45:33 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "WebServ.hpp"

#include <iostream>
#include <netinet/in.h>

typedef enum e_log_mode	
{
	MESSAGE,
	INFO,
	SUCCESS,
	WARNING,
	ERROR,
	FATALERROR
}	t_log_mode;

void	printWebServConfig(std::ostream &out, const WebServ &ws);
void	printHTTP(std::ostream &out, const Http &http, unsigned int level);
void	printServer(std::ostream &out, const Server &server, unsigned int level);
void	printPropertiesSet(std::ostream &out, const PropertiesSet &ps, unsigned int level);
void	printReturnable(std::ostream &out, const Returnable &ps, unsigned int level);
void	printLocation(std::ostream &out, const Location &location, unsigned int level);
void	printTypes(std::ostream &out, const Types &types, unsigned int level);
void	printCGI(std::ostream &out, const CGI &cgi, unsigned int level);

void	printConnection(std::ostream &out, sockaddr_in &client_addr);
void	printLimit(std::ostream &out, const std::string &str, const char *module, size_t limit);

void	logMessage(std::ostream &out, const char *message, t_log_mode mode);
void	logMessage(const char *message, t_log_mode mode);
void	logMessage(const std::string &message, t_log_mode mode);
