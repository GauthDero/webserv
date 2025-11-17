/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 00:30:33 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/07 18:05:10 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "Log.hpp"
#include "server.hpp"
#include "Colors.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>

WebServ	init(const char *configFile)
{
	std::ifstream	iss(configFile);
	ConfigParser	cp(iss);
	iss.clear();
	iss.close();
	if (iss.fail())
		throw FatalException(systemError("Fatal error"));
	cp.verifyTokens();
	return (cp.parseTokens());
}

void	end_program(int sig)
{
	(void) sig;
	throw FatalException("Program terminated due to a signal");
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		logMessage("Configuration file is missing", ERROR);
		return (EXIT_FAILURE);
	}
	try
	{
		time_t seed = time(NULL);
		if (errno)
			throw FatalException(systemError("Main: getting current time"));
		srand(static_cast<unsigned int>(seed));
		signal(SIGTERM, end_program);
		signal(SIGINT, end_program);
		WebServ	ws(init(argv[1]));
		printWebServConfig(std::cout, ws);
		run_server(ws);
		logMessage("Webserv terminated", INFO);
	}
	catch (const FatalException &e)
	{
		logMessage(e.what(), FATALERROR);
		return (EXIT_FAILURE);
	}
	catch (const std::exception &e)
	{
		logMessage(e.what(), ERROR);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
