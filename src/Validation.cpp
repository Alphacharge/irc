/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validation.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 08:37:46 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/04 09:14:52 by rbetz            ###   ########.fr       */
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
void	splitString(std::list<std::string>& parametersList, std::string const& parameters) {
	std::istringstream	iss(parameters);
	std::string			token;

	while(getline(iss, token, ' ')) {parametersList.push_back(token);}
}
