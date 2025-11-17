#include "server.hpp"
#include "Log.hpp"
#include "Colors.hpp"
#include <algorithm>
#include <iostream>
#include <limits>

void setSockets(const FdGuard &epoll_fd, const std::vector<Server> &servers,
	std::map<int, Connection_serv> &serverSockets)
{
	std::vector<Listen>::const_iterator	it;
	sockaddr_in	server_addr;
	epoll_event	event;
	FdGuard		sockfd;
	int			opt;

	for (size_t i = 0; i < servers.size(); ++i)
	{
		for (it = servers[i].getListeners().begin(); it != servers[i].getListeners().end(); ++it)
		{
			sockfd.setFd(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0));

			if (sockfd.getFd() == -1) 
				throw std::runtime_error(systemError("Server: Failed to create socket"));

			opt = 1;

			if (setsockopt(sockfd.getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
				throw std::runtime_error(systemError("Server: setsockopt failed"));

			server_addr = getSocketIn(*it);
			printConnection(std::cerr, server_addr);

			if (bind(sockfd.getFd(), (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
				throw std::runtime_error(systemError("Server: Failed to bind"));

			if (listen(sockfd.getFd(), SOMAXCONN) < 0) 
				throw std::runtime_error(systemError("Server: Failed to listen"));

			event = getEPollEvent(sockfd.getFd());

			if (epoll_ctl(epoll_fd.getFd(), EPOLL_CTL_ADD, sockfd.getFd(), &event) == -1) 
				throw std::runtime_error(systemError("Server: epoll_ctl failed"));

			serverSockets.insert(std::make_pair(sockfd.getFd(), Connection_serv(Connection_info(sockfd.getFd(), server_addr), servers[i])));
		}
	}
	sockfd.setFd(-1);
}

void addClientSocketToEPoll(std::map<int, Connection> &connections, const Connection_serv& serv, const FdGuard &epoll_fd)
{
	sockaddr_in	client_addr;
	socklen_t	client_len;
	epoll_event	ev;
	bool		clean;
	int			client_fd;

	while (true) 
	{
		clean = false;
		client_addr = sockaddr_in();
		client_len = sizeof(client_addr);
		client_fd = accept(serv._info._fd, (struct sockaddr*)&client_addr, &client_len);

		if (client_fd == -1)
		{
			if (isNonBlockingBehaviour())
				break ;
			logMessage(systemError("Server: Accept error"), ERROR);
			continue ;
		}

		if (set_nonblocking(client_fd))
		{
			logMessage(systemError("Server: Setting non-blocking"), ERROR);
			tryClose(client_fd);
			continue ;
		}
		
		printConnection(std::cerr, client_addr);

		ev = epoll_event();
		ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
		ev.data.fd = client_fd;

		if (epoll_ctl(epoll_fd.getFd(), EPOLL_CTL_ADD, client_fd, &ev) == -1) 
		{
			logMessage(systemError("Server: Failed to add client to epoll"), ERROR);
			tryClose(client_fd);
			continue ;
		}

		try
		{
			connections.insert(std::make_pair(client_fd, Connection(Connection_info(client_fd, client_addr), serv)));
		}
		catch (std::exception &e)
		{
			logMessage(systemError("Server: Failed assign a connection to the client"), ERROR);
			clean = true;
		}
		if (clean)
		{
			if ((epoll_ctl(epoll_fd.getFd(), EPOLL_CTL_DEL, client_fd, &ev) == -1))
				throw std::runtime_error(systemError("Server: Failed to clean client"));
			tryClose(client_fd);
		}
	}
}

size_t	getContentLenght(const std::string &headers)
{
	size_t	pos = headers.find("Content-Length:");
			
	if (pos != std::string::npos) 
		return (std::strtoul(headers.c_str() + pos + 15, NULL, 10));
	return (0);
}

bool manageRequest(std::map<int, Connection>::iterator &conn,
	const std::map<int, Connection_serv> &serverSockets)
{
	size_t	header_end;
	size_t	total_length;
	size_t	end_pos;
	std::map<int, Connection_serv>::const_iterator it(serverSockets.find(conn->second._server._info._fd));

	it = (it == serverSockets.end() ? serverSockets.begin() : it);

	header_end = conn->second.read_buffer.find("\r\n\r\n");
	if (header_end == std::string::npos)
		return (false);
	std::string headers = conn->second.read_buffer.substr(0, header_end + 4);

	if (is_chunked_transfer(headers))
	{
		end_pos = conn->second.read_buffer.find("0\r\n\r\n");
		if (end_pos == std::string::npos)
			return (false);
		header_end = end_pos + 1;
	}

	total_length = header_end + 4 + getContentLenght(headers);

	if (conn->second.read_buffer.size() < total_length) /*check if request is complete*/
		return (false);

	std::string request = conn->second.read_buffer.substr(0, total_length);
	conn->second.read_buffer.erase(0, total_length);
	conn->second.response = methods(request, conn->second);
	return (true);
}

void closeClientConnection(std::map<int, Connection> &connections, std::map<int,Connection>::iterator &conn)
{
	logMessage("Server: Closing connection with client...", INFO);
	tryClose(connections, conn->first);
	logMessage("Server: Connection closed", SUCCESS);
}

void resetClient(std::map<int,Connection>::iterator &conn)
{
	conn->second.total_sent = 0;
	conn->second.sent = 0;
	conn->second.request_read = false;
}

void sendResponse(std::map<int, Connection> &connections,
	std::map<int,Connection>::iterator &conn, int epfd)
{
	while (conn->second.total_sent < conn->second.response.size())
	{
		conn->second.sent = send(conn->first, conn->second.response.c_str() + conn->second.total_sent,
			conn->second.response.size() - conn->second.total_sent, 0);
		if (conn->second.sent < 0)
		{
			if (!isNonBlockingBehaviour())
			{
				logMessage(systemError("Server: Sending data to client"), ERROR);
				closeClientConnection(connections, conn);
				return ;
			}
			break ;
		}
		conn->second.total_sent += static_cast<size_t>(conn->second.sent);
	}

	if (conn->second.total_sent != conn->second.response.size())
		return ;
	else if (conn->second.response.find("Connection: close") != std::string::npos)
		closeClientConnection(connections, conn);
	else if (clientListenR(conn->first, epfd))
	{
		logMessage(systemError("Server: Could not set client to listen to read in epoll"), ERROR);
		closeClientConnection(connections, conn);
	}
	else
	{
		logMessage("Server: Response sent to the client", SUCCESS);
		resetClient(conn);
	}
}

void readClientRequest(const std::map<int, Connection_serv> &serverSockets,
	std::map<int, Connection> &connections, std::map<int, Connection>::iterator &it, const int epfd)
{
	char	buffer[BUFFER_SIZE];
	ssize_t	bytes_read;

	try
	{
		while ((bytes_read = read(it->first, buffer, BUFFER_SIZE)) > 0) 
			it->second.read_buffer.append(buffer, static_cast<size_t>(bytes_read));

		if (bytes_read == 0) // Error / Client closed connection
			throw std::runtime_error(systemError("Server: Reading client's request"));

		logMessage("Server: Request has been read", SUCCESS);
		if (manageRequest(it, serverSockets))
		{
			if (clientListenRW(it->first, epfd))
			{
				logMessage(systemError("Server: Could not set client to listen to write in epoll"), ERROR);
				closeClientConnection(connections, it);
			}
			else
				it->second.request_read = true;
		}
	}
	catch(const FatalException& e)
	{
		throw;
	}
	catch(const std::exception& e)
	{
		logMessage(e.what(), ERROR);
		logMessage("Server: Closing connection with client...", INFO);
		tryClose(connections, it->first);
		logMessage("Server: Connection closed", SUCCESS);
	}
}

bool run_server(const WebServ &wb)
{
	FdGuard epoll_fd(epoll_create1(0));

	if (epoll_fd.getFd() == -1) 
		throw std::runtime_error(systemError("Server: Failed to create epoll"));

	std::map<int, Connection_serv>				serverSockets;
	std::map<int, Connection>					connections;
	std::vector<epoll_event>					events;
	std::map<int, Connection_serv>::iterator	it;
	const std::vector<Server>					&serv = wb.getHttpBlock().getServers();
	size_t										n_listeners = getMaxListeners(serv);
	size_t										i;
	int											num_events = 0;
	int											maxListeners;

	if ((static_cast<size_t>(std::numeric_limits<int>::max())) < n_listeners)
		throw FatalException("Server: Max amount of listeners exceeded");

	maxListeners = static_cast<int>(n_listeners);

	events.reserve(n_listeners);

	logMessage("Server: Creating server sockets...", INFO);
	setSockets(epoll_fd, serv, serverSockets);
	logMessage("Server: Server sockets created", SUCCESS);
	try
	{
		while (num_events != -1) // No return unless critical errors
		{
			num_events = epoll_wait(epoll_fd.getFd(), events.data(), maxListeners, -1);
			for (i = 0; num_events > 0 && i < static_cast<size_t>(num_events); ++i)
			{
				it = serverSockets.find(events[i].data.fd);
				if (it != serverSockets.end()) // If it's a server's socket ==> create a new fd and add it to the epoll
				{
					logMessage("Server: Creating clients sockets...", INFO);
					addClientSocketToEPoll(connections, it->second, epoll_fd);
					logMessage("Server: Clients sockets created", SUCCESS);
					continue ;
				}
				else if (hasLostConnection(events[i].events)) // Fd lost connection
				{
					logMessage("Server: Client disconnected", WARNING);
					tryClose(connections, events[i].data.fd);
					continue ;
				}

				std::map<int, Connection>::iterator conn = connections.find(events[i].data.fd);

				if (isClientReadable(events[i].events) && !conn->second.request_read) // Client fd is readable
				{
					logMessage("Server: Reading client's request...", INFO);
					readClientRequest(serverSockets, connections, conn, epoll_fd.getFd());
				}
				else if (isClientWritable(events[i].events))
				{
					logMessage("Server: Sending response to client...", INFO);
					sendResponse(connections, conn, epoll_fd.getFd());
				}
			}
		}
	}
	catch (std::exception &e) // catch exception for guarantee the close of the sockets
	{
		logMessage(e.what(), FATALERROR);
		logMessage("Server: shutting down to preserve system integrity", INFO);
	}

	closeFD(connections);
	closeFD(serverSockets);
	
	return (num_events == -1);
}
