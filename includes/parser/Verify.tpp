/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Verify.tpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 02:22:53 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/06 02:34:40 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Verify.hpp"

template <typename T>
bool	Verify::verifyInteger(t_string_parse &str, T max, bool end)
{
	T		num		= 0;
	bool	sign	= (str.ptr[str.pos] == '+' || str.ptr[str.pos] == '-');
	size_t	i		= str.pos + sign;
	bool 	hasDigits;

	while (num > -1 && std::isdigit(str.ptr[i]))
	{
		num = num * 10 + str.ptr[i] - '0';
		++i; 
	}
	hasDigits = i - str.pos - sign > 0;
	if (num > -1 && hasDigits && num <= max && (!end || !str.ptr[i]))
	{
		str.pos = i;
		return (true);
	}
	return (false);
}

template <typename T>
bool	Verify::verifyUInteger(t_string_parse &str, T max, bool end)
{
	T		num			= 0;
	bool	sign		= str.ptr[str.pos] == '+';
	bool	overflow	= false;
	size_t	i			= str.pos + sign;
	bool	hasDigits;

	while (!overflow && std::isdigit(str.ptr[i]))
	{
		overflow = __builtin_mul_overflow(num, 10, &num)
			|| __builtin_add_overflow(num, str.ptr[i] - '0', &num);
		++i;
	}
	hasDigits = i - str.pos - sign > 0;
	if (!overflow && hasDigits && num <= max && (!end || !str.ptr[i]))
	{
		str.pos = i;
		return (true);
	}
	return (false);
}
