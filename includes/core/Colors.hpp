/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Colors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 20:16:55 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/07 18:02:29 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifdef PRINT_COLORS
	#define	RESET	"\033[0m"
	#define BLACK	"\033[30m"
	#define RED		"\033[31m"
	#define GREEN	"\033[32m"
	#define YELLOW	"\033[33m"
	#define	BLUE	"\033[34m"
	#define MAGENTA	"\033[35m"
	#define CYAN	"\033[36m"
	#define WHITE	"\033[37m"
#else
	#define	RESET	""
	#define BLACK	""
	#define RED		""
	#define GREEN	""
	#define YELLOW	""
	#define	BLUE	""
	#define MAGENTA	""
	#define CYAN	""
	#define WHITE	""
#endif
