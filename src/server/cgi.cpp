#include "cgiHandler.hpp"
#include "Utils.hpp"
#include "Log.hpp"
#include "StatusCodes.hpp"
#include <fstream>
#include <cstring>

#define WRITE_BUFFER	4096

volatile sig_atomic_t	g_signal = 0;

CgiHandler::CgiHandler(const Request &req, Response &res, const CGI &config,  const Connection &conn):
	_body(req.Get_body()), _config(config), _request(req), _response(res)
{
	this->_env_arr = NULL;
	this->initEnv(conn);
}

CgiHandler::CgiHandler(CgiHandler const &src): _request(src._request), _response(src._response)
{
	*this = src;
}

CgiHandler::~CgiHandler(void)
{
	size_t	n = 0;
	while (_env_arr && _env_arr[n])
		delete [] _env_arr[n++];
	delete [] _env_arr;
}

CgiHandler	&CgiHandler::operator=(CgiHandler const &src) 
{
	if (this != &src)
	{
		this->_body = src._body;
		this->_env = src._env;
		this->_config = src._config;
	}
	return (*this);
}

const	std::map<std::string, std::string>	&CgiHandler::getEnv( void ) const
{
	return (this->_env);
}

char	normalizeChar(char c)
{
	return (std::isalnum(c) ? static_cast<char>(std::toupper(c)) : '_');
}

void	CgiHandler::EnvtoArray( void ) 
{
	const std::deque<std::string> &uf = this->_request.Get_unknown_fields();

	this->_env_arr = new char*[this->_env.size() + uf.size() + 1]();
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator i = this->_env.begin(); i != this->_env.end(); ++i) 
	{
		this->_env_arr[j] = new char[i->first.length() + i->second.length() + 2];

		std::strcpy(this->_env_arr[j], i->first.c_str());
		this->_env_arr[j][i->first.length()] = '=';
		std::strcpy(&this->_env_arr[j][i->first.length() + 1], i->second.c_str());
		++j;
	}
	size_t	start;
	size_t	end;
	bool	leading_space;
	for (std::deque<std::string>::const_iterator i = uf.begin(); i != uf.end(); ++i)
	{
		start = i->find(':') + 1;
		end = i->length() - 2;		// \r\n
		end -= i->at(end) == ' ';	// Trailing optional space
		leading_space = i->at(start) == ' ';
		
		this->_env_arr[j] = new char[end + 6 - leading_space];
		std::strcpy(this->_env_arr[j], "HTTP_");

		for (size_t k = 0; k < start - 1; ++k)
			this->_env_arr[j][5 + k] = normalizeChar(i->at(k));

		this->_env_arr[j][start + 4] = '=';
		std::strncpy(&this->_env_arr[j][start + 5],
			i->c_str() + start + leading_space, end - start - leading_space);
		this->_env_arr[j][end + 5 - leading_space] = '\0';
		++j;
	}
	this->_env_arr[j] = NULL;
}

std::string get_method(t_http_method method)
{
	if (method == HttpGet)
		return ("GET");
	else if (method == HttpPost)
		return ("POST");
	else if (method == HttpDelete)
		return ("DELETE");
	else if (method == HttpHead)
		return ("HEAD");
	return ("UNKNOWN");
}

/*need to know parsing to fill env with */
// CONTENT_LENGTH		123									Length (in bytes) of the request body — only for POST or PUT methods.
// CONTENT_TYPE			application/x-www-form-urlencoded	From the HTTP header Content-Type. Needed if request has a body.
// GATEWAY_INTERFACE	CGI/1.1								Must always be this exact string.
// PATH_INFO			/extra/stuff						Additional path information following the CGI script’s path (optional).
// PATH_TRANSLATED		/var/www/html/extra/stuff			Physical path corresponding to PATH_INFO. Usually root + PATH_INFO.
// REMOTE_ADDR			127.0.0.1							IP address of the client (optional but recommended).
// REQUEST_METHOD		GET / POST / DELETE					HTTP method of the request.
// SCRIPT_NAME			/cgi-bin/script.py					The URL path to the CGI script.
// SERVER_NAME			localhost							The server hostname (from Host header or config).
// SERVER_PORT			8080								TCP port the server is listening on.
// SERVER_PROTOCOL		HTTP/1.1							Protocol version used in the request.
// SERVER_SOFTWARE		webserv/1.0							Your server name and version (you can invent this).
void		CgiHandler::initEnv(const Connection &conn)
{
	this->_env["SERVER_PORT"]		= toString(ntohs(conn._server._info._data.sin_port));
	this->_env["REMOTE_ADDR"]		= inet_ntoa(conn._client._data.sin_addr); //ip address of the client making the request
	this->_env["REMOTE_PORT"]		= toString(ntohs(conn._client._data.sin_port)); // same for port
	this->_env["GATEWAY_INTERFACE"]	= "CGI/1.1";
	this->_env["SCRIPT_FILENAME"]	= this->_request.Get_path();
	this->_env["SERVER_SOFTWARE"]	= "webserv/1.0";
	this->_env["PATH_INFO"]			= this->_request.Get_info_path().empty() ? "/" : this->_request.Get_info_path();

	this->_env["PATH_TRANSLATED"].assign(this->_response.Get_path(), 0,
		this->_response.Get_path().find(this->_request.Get_path())).append(this->_env["PATH_INFO"]);

	std::string str(20, '\0');

	str.resize(0);
	Append_size_t(str, this->_request.Get_body().length());

	this->_env["CONTENT_LENGTH"]	= str;
	this->_env["CONTENT_TYPE"]		= this->_request.Get_content_type();
	this->_env["SERVER_NAME"]		= this->_request.Get_host();
	this->_env["QUERY_STRING"]		= this->_request.Get_query_string();
	this->_env["SERVER_PROTOCOL"]	= this->_request.Get_protocol();
	this->_env["REQUEST_METHOD"]	= get_method(this->_request.Get_method());

	if (!this->_request.Get_referer().empty())
		this->_env["HTTP_REFERER"]			= this->_request.Get_referer();
	if (!this->_request.Get_user_agent().empty())
		this->_env["HTTP_USER_AGENT"]		= this->_request.Get_user_agent();
	if (!this->_request.Get_accept().empty())
		this->_env["HTTP_ACCEPT"]			= this->_request.Get_accept();
	if (!this->_request.Get_accept_language().empty())
		this->_env["HTTP_ACCEPT_LANGUAGE"]	= this->_request.Get_accept_language();
	if (!this->_request.Get_accept_encoding().empty())
		this->_env["HTTP_ACCEPT_ENCODING"]	= this->_request.Get_accept_encoding();
}

t_code CgiHandler::find_executabe(std::string &executable, const std::string &scriptName)
{
	if (this->_config.getUseShebang())
	{
		logMessage("CGI: Trying to use shebang...", INFO);
		if (access(scriptName.c_str(), F_OK | R_OK | X_OK) != 0)
		{
			logMessage(systemError("CGI: Checking script"), ERROR);
			return (Get_status_errno());
		}

		std::ifstream	file(scriptName.c_str());
		if (!file)
			return (INTERNAL_SERVER_CODE);

		std::string		line;
		std::getline(file, line);

		if (file.bad())
		{
			logMessage("CGI: Script read failed", ERROR);
			return (INTERNAL_SERVER_CODE);
		}
		size_t shebang = line.find("#!");
		if (shebang == 0)
		{
			logMessage("CGI: Shebang found", SUCCESS);
			size_t first_ws = line.find(' ');
			size_t second_ws = (first_ws != std::string::npos) ? line.find(' ', first_ws + 1) : std::string::npos;
			executable.assign(line, 2, second_ws != std::string::npos ? second_ws : line.length());
			return (0);
		}
		else
			logMessage("CGI: Shebang not found", WARNING);
	}
	logMessage("CGI: Using default executable...", INFO);
	executable.assign(this->_config.getPass());

	if (access(executable.c_str(), F_OK | X_OK) != 0)
	{
		logMessage(systemError("CGI: Checking executable"), ERROR);
		return (Get_status_errno());
	}
	return (0);
}

void	terminate_child(pid_t pid)
{
	logMessage("CGI: Stopping child process", INFO);
	if (kill(pid, SIGTERM) && errno != ESRCH)
	{
		logMessage(systemError("CGI: Child could not be stopped"), ERROR);
		logMessage("CGI: Forcing child process to stop", INFO);
		if (kill(pid, SIGKILL) && errno != ESRCH)
			throw FatalException(systemError("CGI: child process could not be stopped"));
	}
}

void	get_signal(int sig)
{
	g_signal = sig;
}

int	try_wait(pid_t pid, int &exit_code)
{
	errno = 0;
	int	wait = waitpid(pid, &exit_code, 0);

	alarm(0);
	signal(SIGALRM, SIG_DFL);

	if (wait < 0 && errno != EINTR)
		throw FatalException(systemError("CGI: Waitpid"));
	if (wait > 0 && WIFEXITED(exit_code))
	{
		logMessage("CGI: child process terminated gracefully", SUCCESS);
		exit_code = WEXITSTATUS(exit_code);
		return (0);
	}
	if (wait == 0 || errno == EINTR)
	{
		logMessage("CGI: script still running", ERROR);
		terminate_child(pid);
		logMessage("CGI: Child process terminated", SUCCESS);
		wait = waitpid(pid, &exit_code, 0);
		if (wait < 0)
			throw FatalException(systemError("CGI: Waitpid after kill"));
		if (g_signal == SIGALRM)
			return (2);
	}
	return (1);
}

bool	pass_file(const Request & res)
{
	return (res.Get_method() == HttpGet || res.Get_method() == HttpHead);
}

void	clean(int pipeIn[2], int pipeOut[2])
{
	tryClose(pipeIn[0]);
	tryClose(pipeIn[1]);
	tryClose(pipeOut[0]);
	tryClose(pipeOut[1]);
}

void	CgiHandler::child_process(int pipeIn[2], int pipeOut[2],
	const std::string &scriptName, const std::string &executable)
{
	try
	{
		if ((close(pipeIn[1]) == -1) | (close(pipeOut[0]) == -1))
		{
			close(pipeIn[0]);
			close(pipeOut[1]);
			throw FatalException("CGI: child process: close");
		}

		if (dup2(pipeIn[0], STDIN_FILENO) < 0
			|| dup2(pipeOut[1], STDOUT_FILENO) < 0)
			{
				close(pipeIn[0]);
				close(pipeOut[1]);
				throw FatalException("CGI: Dup in child process");
			}

		if ((close(pipeIn[0]) == -1) | (close(pipeOut[1]) == -1))
			throw FatalException("CGI: child process: close");

		char *const args[] = {
			(char *) executable.c_str(), 
			(char *) scriptName.c_str(),
			pass_file(this->_request) ? (char *) this->_env.find("PATH_TRANSLATED")->second.c_str() : NULL, // include file for GET request
			NULL
		};

		execve(executable.c_str(), args, this->_env_arr);
		logMessage("CGI: child: execution failed", ERROR);
	}
	catch(const FatalException& e)
	{
		logMessage(e.what(), FATALERROR);
	}
	logMessage("CGI: child: terminate", INFO);
	std::exit(EXIT_FAILURE);
}

t_code		CgiHandler::executeCgi(const std::string &scriptName) 
{	
	pid_t		pid;
	int			pipeIn[2] = {-1, -1};
	int			pipeOut[2] = {-1, -1};
	std::string	executable;
	t_code		status = find_executabe(executable, scriptName);

	if (status)
		return (status);
	
	try 
	{
		this->EnvtoArray();
	}
	catch (const std::bad_alloc &e) 
	{
		logMessage(e.what(), ERROR);
		return (INTERNAL_SERVER_CODE);
	}

	if (pipe(pipeIn) == -1 || pipe(pipeOut) == -1) 
	{
		logMessage(systemError("CGI: Pipe creation"), ERROR);
		clean(pipeIn, pipeOut);
		return (INTERNAL_SERVER_CODE);
	}

	if (set_nonblocking(pipeOut[0]) || set_nonblocking(pipeIn[1]))
	{
		logMessage(systemError("CGI: Setting pipe flags"), ERROR);
		clean(pipeIn, pipeOut);
		return (INTERNAL_SERVER_CODE);
	}

	pid = fork();

	if (pid == -1) 
	{
		logMessage(systemError("CGI: Fork"), ERROR);
		clean(pipeIn, pipeOut);
		return (INTERNAL_SERVER_CODE);
	}
	else if (pid == 0) // Child
		child_process(pipeIn, pipeOut, scriptName, executable);

	logMessage("CGI: Executing child...", INFO);

	struct sigaction sa;
	sa.sa_handler = get_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGALRM, &sa, NULL);

	alarm(this->_config.getExecutionTimeout());

	if ((close(pipeIn[0]) == -1) | (close(pipeOut[1]) == -1))
	{
		close(pipeOut[0]);
		close(pipeIn[1]);
		if (kill(pid, SIGTERM) && errno != ESRCH)
			kill(pid, SIGKILL);
		throw FatalException(systemError("CGI: Closing pipes"));
	}

	char	buffer[WRITE_BUFFER];
	size_t	totalWritten = 0;
	ssize_t	bytesRead = 1;
	ssize_t	bytesWritten = 1;
	fd_set	writefds;
	fd_set	readfds;
	int		biggestPipe = std::max(pipeIn[1], pipeOut[0]);
	int		select_ret;
	struct timeval tv;
	tv.tv_sec = this->_config.getExecutionTimeout();
	tv.tv_usec = 0;
	errno = 0;

	while (bytesWritten > 0 || bytesRead > 0)
	{
		if (bytesWritten > 0)
		{
			FD_ZERO(&writefds);
			FD_SET(pipeIn[1], &writefds);
		}
		if (bytesRead > 0)
		{
			FD_ZERO(&readfds);
			FD_SET(pipeOut[0], &readfds);
		}
		select_ret = select(biggestPipe + 1, bytesRead > 0 ? &readfds : NULL, bytesWritten > 0 ?  &writefds : NULL, NULL, &tv);
		if (select_ret < 0)
		{
			logMessage(systemError("CGI: Select"), ERROR);
			break ;
		}
		else if (select_ret == 0)
		{
			logMessage("CGI: Timeout for operating over the pipes", ERROR);
			break ;
		}
		if (FD_ISSET(pipeIn[1], &writefds) && bytesWritten)
		{
			bytesWritten = write(pipeIn[1], this->_request.Get_body().c_str() + totalWritten,
				this->_request.Get_body().length() - totalWritten);
			if (bytesWritten < 0)
				break ;
			totalWritten += static_cast<size_t>(bytesWritten);
			if (bytesWritten == 0)
			{
				if (close(pipeIn[1]) == -1)
				{
					close(pipeOut[0]);
					if (kill(pid, SIGTERM) && errno != ESRCH)
						kill(pid, SIGKILL);
					throw FatalException(systemError("CGI: Closing read pipe"));
				}
			}
		}
		if (FD_ISSET(pipeOut[0], &readfds) && bytesRead)
		{
			bytesRead = read(pipeOut[0], buffer, WRITE_BUFFER - 1);
			if (bytesRead < 0)
				break ;
			this->_response.Append_body(buffer, static_cast<size_t>(bytesRead));
			if (bytesRead == 0)
			{
				if (close(pipeOut[0]) == -1)
				{
					close(pipeIn[1]);
					if (kill(pid, SIGTERM) && errno != ESRCH)
						kill(pid, SIGKILL);
					throw FatalException(systemError("CGI: Closing write pipe"));
				}
			}
		}
	}

	if (errno == EINTR)
		terminate_child(pid);

	int	exit_code;
	int wait = try_wait(pid, exit_code);

	if (bytesWritten < 0 || totalWritten != this->_request.Get_body().length()
		|| bytesRead < 0)
		return (INTERNAL_SERVER_CODE);
	if (wait == 2)
		return (GATEWAY_TIMEOUT_CODE);
	else if (wait == 1 || exit_code != 0)
		return (INTERNAL_SERVER_CODE);
	return (OK_CODE);
}
