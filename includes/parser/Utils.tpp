/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 03:11:24 by dzapata           #+#    #+#             */
/*   Updated: 2025/10/02 22:25:52 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <vector>
#include <iostream>

// Two passes are made for not couting repeated elements and avoiding unnecessary allocations
template<typename T>
void	addUniqueElements(std::vector<T> &dst, const std::vector<T> &src)
{
	size_t count = 0;
	for (typename std::vector<T>::const_iterator it = src.begin(); it != src.end(); ++it)
	{
		if (std::find(dst.begin(), dst.end(), *it) == dst.end())
			++count;
	}
	dst.reserve(dst.size() + count);
	for (typename std::vector<T>::const_iterator it = src.begin(); it != src.end(); ++it)
	{
		if (std::find(dst.begin(), dst.end(), *it) == dst.end())
			dst.push_back(*it);
	}
}
