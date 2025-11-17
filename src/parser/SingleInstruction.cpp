/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SingleInstruction.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:43:09 by dzapata           #+#    #+#             */
/*   Updated: 2025/11/06 02:30:18 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SingleInstruction.hpp"

SingleInstruction::SingleInstruction(const int minArgs, const int maxArgs, const int argsType):
	AInstruction(minArgs, maxArgs, argsType)
{
	this->_type = ITSingle;
}

SingleInstruction::SingleInstruction(const int args, const int argsType):
	AInstruction(args, args, argsType)
{
	this->_type = ITSingle;
}

/*std::vector<std::string>	&SingleInstruction::getKeywords(void)
{
	return (keywords);
}*/
