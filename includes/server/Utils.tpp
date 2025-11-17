/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdero <gdero@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 20:21:29 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/01 15:37:19 by gdero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <unistd.h>
#include <string>
#include <sstream>

#include "FatalException.hpp"

template<typename T>
void	closeFD(const std::map<int, T> &map)
{
	for (typename std::map<int, T>::const_iterator it = map.begin(); it != map.end(); ++it)
		close(it->first);
}

template<typename T>
void	tryClose(std::map<int, T> &map, int fd)
{
	if (fd > -1 && close(fd) == -1)
	{
		if (errno != EINTR)
			map.erase(fd);
		throw FatalException(systemError("Fatal error: close"));
	}
	map.erase(fd);
}

template<typename T>
std::string	toString(const T & arg)
{
	std::stringstream ss;
	ss << arg;
	if (ss.fail())
		throw std::runtime_error(systemError("To string error"));
	return (ss.str());
}

template<typename T>
void	destroyMatrix(T **arr)
{
	for (size_t i = 0; arr && arr[i]; i++)
		delete[] arr[i];
	delete[] arr;
}
