/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FdGuard.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 19:35:17 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/06 02:47:25 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FdGuard.hpp"

FdGuard::FdGuard(int fd): _fd(fd) {}

FdGuard::FdGuard( void ): _fd(-1) {}

FdGuard::FdGuard(const FdGuard& fd)
{
	*this = fd;
}

FdGuard::~FdGuard( void )
{
	if (this->_fd != -1)
		close(this->_fd);
}

int FdGuard::getFd() const
{
	return (_fd);
}

void FdGuard::setFd(int fd)
{
	this->_fd = fd;
}

FdGuard &FdGuard::operator=(const FdGuard &fd)
{
	if (this != &fd)
	{
		this->_fd = fd._fd;
	}
	return (*this);
}

bool FdGuard::operator==(const FdGuard &fd) const
{
	return (this->_fd == fd._fd);
}

bool FdGuard::operator==(int fd) const
{
	return (this->_fd == fd);
}

int FdGuard::release()
{
	int temp = this->_fd;
	_fd = -1;
	return (temp);
}
