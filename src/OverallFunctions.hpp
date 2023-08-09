/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OverallFunctions.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 14:39:17 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/09 17:04:28 by lsordo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <cstring>
# include <map>
# include <list>
# include <sstream>
# include <ctime>
# include <sstream>
# include "Colors.hpp"

bool	isValidChannelName(std::string const &name);
std::list<std::string>	splitString(std::string const& stringToSplit, char const& delimiter);
std::string				currentTimeString(void);
