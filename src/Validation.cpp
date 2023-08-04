/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validation.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 08:37:46 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/04 11:06:27 by rbetz            ###   ########.fr       */
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

/* split strings on spaces*/
std::list<std::string>	splitString(std::string const&parameters, char const& delimiter) {
	std::list<std::string>	parametersList;
	std::istringstream		iss(parameters);
	std::string				token;

	while(getline(iss, token, delimiter)) {parametersList.push_back(token);}
	return parametersList;
}
