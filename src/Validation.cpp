/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validation.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 08:37:46 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/03 16:17:00 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Validation.hpp"

/*Channel name can't contain more than 200chars, spaces, commas, ^D or miss a & or # at the beginning*/
bool	isValidChannelName(std::string const &name)
{
	if (name.empty())
		return false;
	if (name.length() > 200)
		return false;
	if (name.find(' ', 0) != name.npos)
		return false;
	if (name.find(',', 0) != name.npos)
		return false;
	if (name.find('\a', 0) != name.npos)
		return false;
	if (name.length() > 0 && (name[0] == '&' || name[0] == '#'))
		return false;
	return false;
}

bool	containsPasswords(std::string const &input)
{
	if (input.find(' ') == input.npos)
		return false;
	return true;
}

bool	containsMultipleChannels(std::string const &input)
{
	if (input.find(',') == input.npos)
		return false;
	return true;
}

std::map<std::string, std::string>	joinSplitInput(std::string const &input)
{
	std::map<std::string, std::string>	map;
	std::string	channels = input;
	size_t		pos = input.find(' ');
	if (pos != 0)
	{
		channels = input.substr(0, pos);
		std::string passwords = input.substr(pos + 1, input.length() - pos);
	}
	while (containsMultipleChannels(channels))
	{
		size_t		lim = channels.find(',');
		size_t		limpw = passwords.find(',');
		map[channels.substr(0, lim)] = 
	}
}