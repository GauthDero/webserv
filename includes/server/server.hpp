/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 03:16:01 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/15 00:01:11 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <cerrno>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <map>
#include <string.h>
#include <sstream>

#include "FdGuard.hpp"
#include "WebServ.hpp"

#define BUFFER_SIZE		4096
#define ERROR_BUFFER	512

extern char	g_errorMessage[ERROR_BUFFER];

struct Connection_info
{
	int			_fd;
	sockaddr_in	_data;

	Connection_info(int fd, const sockaddr_in &data): _fd(fd), _data(data) {}
};

struct Connection_serv
{
	Connection_info	_info;
	const	Server&	_serv;

	Connection_serv(const Connection_info &info, const Server &serv):
		_info(info), _serv(serv) {}
};

struct Connection 
{
			Connection_info	_client;
	const	Connection_serv	&_server;
	std::string				read_buffer;
	std::string				response;
	size_t					total_sent;
	ssize_t					sent;
	bool					request_read;

	Connection(const Connection_info &client, const Connection_serv &server):
		_client(client), _server(server)
	{
		request_read = false;
		total_sent = 0;
		sent = 0;
	}
};

// Utils

bool		hasLostConnection(const uint32_t event);
bool		isClientReadable(const uint32_t event);
bool		isClientWritable(const uint32_t event);
bool		clientListenR(const int clientfd, const int epfd);
bool		clientListenRW(const int clientfd, const int epfd);
bool		isNonBlockingBehaviour( void );
void		tryClose(const int fd);
bool		set_nonblocking(const int fd);
size_t		getMaxListeners(const std::vector<Server> &servers);
sockaddr_in	getSocketIn(const Listen &listener);
epoll_event	getEPollEvent(const int fd);
char		*systemError(const char *message);
char		*stringError(const char *arg, const char *arg2);
char		*stringError(const char *arg, const char *arg2, const char *arg3);
char		*stringError(const char *arg, const char *arg2, const char *arg3, const char *arg4);
bool 		is_chunked_transfer(const std::string &headers);

bool		run_server(const WebServ &wb);
std::string	methods(const std::string &input, const Connection &conn);

#include "Utils.tpp"

#endif
