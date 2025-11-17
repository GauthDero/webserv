/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonTypes.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 02:42:59 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/15 04:30:12 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

typedef int64_t			t_int;
typedef uint64_t		t_uint;
typedef	uint16_t		t_code;
typedef unsigned int	t_time;
typedef uint16_t		t_port;

typedef enum e_http_method
{
	HttpUnknown	= 0,
	HttpGet		= 1,
	HttpPost	= 1 << 1,
	HttpDelete	= 1 << 2,
	HttpHead	= 1 << 3
}	t_http_method;
