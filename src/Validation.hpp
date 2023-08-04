/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validation.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 14:39:17 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/04 07:05:51 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <cstring>
# include <map>
# include "Colors.hpp"

bool	isValidChannelName(std::string const &name);
bool	containsPasswords(std::string const &input);
bool	containsMultipleChannels(std::string const &input);
std::map<std::string, std::string>	joinSplitInput(std::string const &input);
