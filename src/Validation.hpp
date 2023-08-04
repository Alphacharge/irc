/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validation.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 14:39:17 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/04 10:32:12 by lsordo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <cstring>
# include <map>
# include <list>
# include <sstream>
# include "Colors.hpp"

bool	isValidChannelName(std::string const &name);
bool	containsPasswords(std::string const &input);
bool	containsMultipleChannels(std::string const &input);
std::map<std::string, std::string>	joinSplitInput(std::string const &input);
void	splitString(std::list<std::string>&, std::string const&);
