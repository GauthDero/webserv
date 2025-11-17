/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Response.hpp									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: dzapata <dzapata@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/07/23 17:44:31 by gdero			 #+#	#+#			 */
/*   Updated: 2025/10/20 18:59:39 by dzapata		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <stdlib.h>
# include <cstring>
# include <iostream>
# include <fstream>
# include <fcntl.h>
# include <unistd.h>
# include <errno.h>
# include <sys/stat.h>
# include <sstream>
# include <dirent.h>

# include "Request.hpp"
# include "StatusCodes.hpp"
# include "server.hpp"

class Response
{
	private:

		const	Server			&server;

		const	std::string		&http_version;
		const	std::string		&request_path;
		const	char			*boundary;

		const	t_http_method	method;

		const	Request			&request;

		std::string	path;
		std::string	alias;

		std::string	status_code;
		std::string	location;
		std::string	web_server;
		std::string	content_type;
		std::string	content_length;
		std::string	connection;
		
		std::string	body;
		std::string	allow;

	public:
		Response(const Request& request_);
		~Response( void );

		//Getters
				t_http_method	Get_method( void )			const;
		const	std::string		&Get_location( void )		const;
		const	std::string		&Get_content_type( void )	const;
		const	std::string		&Get_content_length( void )	const;
		const	std::string		&Get_allow( void )			const;
		const	std::string		&Get_connection( void )		const;
		const	std::string		&Get_web_server( void )		const;
		const	std::string		&Get_protocol( void )		const;
		const	std::string		&Get_status_code( void )	const;
		const	std::string		&Get_body( void )			const;
		const	std::string		&Get_path( void )			const;
		const	Server			&Get_server( void )			const;

		//Setters
		void		Set_path(const std::string &path_);
		void		Set_alias(const std::string &alias_);
		void		Set_status_code(const std::string &errorcode);
		void		Set_body(const std::string &body_);
		void		Set_close_connection( void );
		void		Set_content_length( void );
		void		Set_content_type(const char *type);

		// Appends
		void		Append_body(const char *buffer, size_t length);

		//export function
		void		Initiate_strings( void );
		std::string	Generate_filename(const PropertiesSet& ps);
		std::string	Generate_response( void );
		void		json_message(t_code status_code_);
		void		manage_request(const PropertiesSet& ps, const Connection &conn, t_http_method methods);
		void		Manage_cgi_response(const PropertiesSet &ps, t_code res);
		int			Manage_cgi_header(const std::string &header, size_t &pos);
		void		redirection(const Returnable *ret);
		void		html_error(const PropertiesSet &ps, t_code status_code_);
		void		html_error(const PropertiesSet &ps);
		bool		is_directory( void );

		t_code	cgi(const CGI &config, const Connection &conn);

		const CGI	&Get_CGI(const PropertiesSet &ps);

		//methods
		bool		try_cgi(const PropertiesSet &ps, const Connection &conn);
		void		switch_methods(const Connection &conn);
		void		Http_get_method(const PropertiesSet &ps, t_http_method methods);
		void		Http_post_method(const PropertiesSet &ps);
		void		Http_put_method(const PropertiesSet &ps);
		void		Http_delete_method(const PropertiesSet &ps);
		void		post_multipart( void );
		void		put_multipart( void );
		void		read_file(int fd);
		void		generate_autoindex(const PropertiesSet &ps, t_http_method methods);
		void		set_path(const Location *location, const PropertiesSet& ps);
};

#endif
