/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 17:44:31 by gdero             #+#    #+#             */
/*   Updated: 2025/11/15 03:44:05 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <iostream>
# include <sys/socket.h>
# include <fcntl.h>
# include <unistd.h>
# include <errno.h>
# include <vector>
# include <deque>
# include "Server.hpp"

# define MAX_FIELDS	13

class Request
{
	private:
		const   Server  &server;

		static	const	char	*accepted_fields[];

		std::string	*elements[15];

		std::deque<std::string>	unknown_fields;

		//obligatoire
		t_http_method	method;

		std::string		http_version;
		std::string		path;
		std::string		host;

		//optionnel
		std::string	content_length;
		std::string	date;
		std::string	user_agent;
		std::string	connection;
		std::string	referer; //(historique)
		std::string	accept;
		std::string	accept_language;
		std::string	accept_encoding;
		std::string	query_string;
		std::string	info_path;
		std::string	upgrade_insecure_requests; //redirection httpS

		//body
		std::string	content_type;
		std::string	transfer_encoding;
		std::string	expect;

		//authentification
		//std::string	authorization;
		//std::string	cookie;

		//body
		std::string	body;

		public:
		Request(const Server &serv);
		~Request();

		//Getters
		const	Server			&Get_server( void )				const;
		const	std::string		&Get_protocol( void )			const;
		const 	std::string		&Get_path( void )				const;
		const	std::string		&Get_body( void )				const;
		const	std::string		&Get_content_type( void )		const;
		const	std::string		&Get_query_string( void )		const;
		const	std::string		&Get_info_path ( void )			const;
		const	std::string		&Get_host( void )				const;
		const	std::string		&Get_date( void )				const;
		const	std::string		&Get_user_agent( void )			const;
		const	std::string		&Get_referer( void )			const;
		const	std::string		&Get_accept( void )				const;
		const	std::string		&Get_accept_language( void )	const;
		const	std::string		&Get_accept_encoding( void )	const;

		const	std::deque<std::string>		&Get_unknown_fields( void )	const;

				t_http_method	Get_method( void )			const;

		// Setters
		void		Set_protocol(const std::string &http_version_);
		void		Set_cgi_paths( void );
	
		t_code		get_info(const std::string &input);
		bool		enough_info() const;
		int			analyse_first_line(const std::string &line); 
		bool		analyse_line(const std::string &line);
		bool		unchunk_body(const std::string &input, size_t index);

		t_code		post_method(const std::string &input, size_t index);
		t_code		verify_multipart( void ) const;
};

#endif
