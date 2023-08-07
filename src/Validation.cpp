/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validation.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 08:37:46 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/07 10:27:51 by lsordo           ###   ########.fr       */
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

/* splits the stringToBeSPlit on delimiter and returns a list of splits  */
std::list<std::string>	splitString(std::string const&stringToBeSplit, char const& delimiter) {
	std::list<std::string>	listOfSplits;
	std::istringstream		iss(stringToBeSplit);
	std::string				token;

	while(getline(iss, token, delimiter)) {listOfSplits.push_back(token);}
	return listOfSplits;
}
