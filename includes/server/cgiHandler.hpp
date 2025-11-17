/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:08:06 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/13 21:57:49 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "WebServ.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "server.hpp"
#include "CGI.hpp"
#include <wait.h>
#include <sstream>

class CgiHandler 
{
	public:
		CgiHandler(const Request &req, Response &res, const CGI &config, const Connection& conn); // sets up env according to the request
		CgiHandler(CgiHandler const &src);
		virtual ~CgiHandler(void);
		CgiHandler   	&operator=(CgiHandler const &src);
		t_code			executeCgi(const std::string &scriptName);	// executes cgi and returns a status code and a body in case of success
		const std::map<std::string, std::string>	&getEnv( void ) const;

	private:
		CgiHandler(void);
		void								initEnv(const Connection &conn);
		void								EnvtoArray( void );
		t_code 								find_executabe(std::string &executable, const std::string &scriptName);
		void								child_process(int pipeIn[2], int pipeOut[2],
												const std::string &scriptName, const std::string &executable);
		std::map<std::string, std::string>	_env;
		std::string							_body;
		CGI									_config;
		char								**_env_arr;
		const Request&						_request;
		Response&							_response;
};
