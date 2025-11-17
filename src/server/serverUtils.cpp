/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 01:49:28 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/15 04:01:36 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

char	g_errorMessage[ERROR_BUFFER];

bool hasLostConnection(const uint32_t event)
{
	return (event & EPOLLRDHUP);
}

bool isClientReadable(const uint32_t event)
{
	return (event & EPOLLIN);
}

bool isClientWritable(const uint32_t event)
{
	return (event & EPOLLOUT);
}

bool	clientListenR(const int clientfd, const int epfd)
{
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
	ev.data.fd = clientfd;
	return (epoll_ctl(epfd, EPOLL_CTL_MOD, clientfd, &ev) == -1);
}

bool	clientListenRW(const int clientfd, const int epfd)
{
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET | EPOLLOUT | EPOLLRDHUP;
	ev.data.fd = clientfd;
	return (epoll_ctl(epfd, EPOLL_CTL_MOD, clientfd, &ev) == -1);
}

bool isNonBlockingBehaviour( void )
{
	return (errno == EAGAIN || errno == EWOULDBLOCK);
}

void tryClose(const int fd)
{
	if (fd > -1 && close(fd) == -1)
		throw FatalException(systemError("Close"));
}

char	*systemError(const char *message)
{
	return (stringError(message, strerror(errno)));
}

char	*stringError(const char *arg, const char *arg2)
{
	snprintf(g_errorMessage, ERROR_BUFFER, "%s: %s", arg, arg2);
	return (g_errorMessage);
}

char	*stringError(const char *arg, const char *arg2, const char *arg3)
{
	snprintf(g_errorMessage, ERROR_BUFFER, "%s: %s: %s", arg, arg2, arg3);
	return (g_errorMessage);
}

char	*stringError(const char *arg, const char *arg2, const char *arg3, const char *arg4)
{
	snprintf(g_errorMessage, ERROR_BUFFER, "%s: %s: %s: %s", arg, arg2, arg3, arg4);
	return (g_errorMessage);
}

bool set_nonblocking(const int fd)
{
	return (fcntl(fd, F_SETFL, O_CLOEXEC | O_NONBLOCK) == -1);
}

size_t	getMaxListeners(const std::vector<Server> &servers)
{
	size_t	maxListeners = 0;
	for (size_t i = 0; i < servers.size(); i++)
		maxListeners += servers.at(i).getListeners().size();
	return (maxListeners);
}

sockaddr_in	getSocketIn(const Listen &listener)
{
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(listener.getIP());
	server_addr.sin_port = htons(listener.getPort());
	return (server_addr);
}

epoll_event getEPollEvent(const int fd)
{
	epoll_event event = epoll_event();
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = fd;
	return (event);
}

bool is_chunked_transfer(const std::string &headers) 
{
    return headers.find("Transfer-Encoding: chunked") != std::string::npos
        || headers.find("transfer-encoding: chunked") != std::string::npos;
}
