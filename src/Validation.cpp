/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validation.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 08:37:46 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/04 07:30:37 by rbetz            ###   ########.fr       */
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
	std::string	passwords;
	size_t		pos = input.find(' ');
	std::cout << LGREEN << input << WHITE << std::endl;
	if (pos != input.npos)
	{
		channels = input.substr(0, pos);
		passwords = input.substr(pos + 1, input.length() - pos);
		std::cout << RED << channels << WHITE << std::endl;
		std::cout << YELLOW << passwords << WHITE << std::endl;
	}
	while (containsMultipleChannels(channels))
	{
		std::cout << PURPLE << channels << WHITE << std::endl;
		size_t		lim = channels.find(',');
		size_t		limpw = passwords.find(',');
		map[channels.substr(0, lim)] = passwords.substr(0, limpw);
		channels = channels[lim];
		passwords = passwords[limpw];
		std::cout << DGREEN << channels << WHITE << std::endl;
		std::cout << BLUE << channels << WHITE << std::endl;
	}
	return map;
}