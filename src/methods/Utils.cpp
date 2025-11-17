/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 00:28:48 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/06 02:11:21 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include <cstdio>
#include <cctype>

#define SIZE_T_BUFFER	40

void	Append_size_t(std::string &str, size_t size)
{
	char buffer[SIZE_T_BUFFER];
	sprintf(buffer, "%lu", (unsigned long) size);
	str.append(buffer);
}

bool	Compare_insensitive(const char *s1, const char *s2, size_t n)
{
	size_t	i = 0;

	while (i < n && s1[i] && s2[i] && std::tolower(s1[i]) == std::tolower(s2[i]))
		++i;
	return (i == n);
}
