/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FdGuard.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 19:34:23 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/06 02:47:25 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <iostream>
# include <unistd.h>

class FdGuard 
{
	private:
	int	_fd;

	public:
	FdGuard(int fd);
	FdGuard(const FdGuard& fd);
	FdGuard( void );
	~FdGuard( void );

	int		getFd()		const;

	void	setFd(int fd);

	FdGuard	&operator=(const FdGuard &fd);

	bool	operator==(const FdGuard &fd)	const;
	bool	operator==(int fd)				const;

	int		release( void );
};
