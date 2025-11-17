/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SingleInstruction.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzapata <dzapata@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:34:36 by dzapata           #+#    #+#             */
/*   Updated: 2025/09/30 23:44:32 by dzapata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "AInstruction.hpp"
#include <vector>
#include <string>

class SingleInstruction: public AInstruction
{
	//protected:
	//std::vector<std::string> keywords;

	public:
	SingleInstruction(const int minArgs, const int maxArgs, const int argsType);
	SingleInstruction(const int args, const int argsType);

	//std::vector<std::string>	&getKeywords( void );
};
