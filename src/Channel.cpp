/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 08:37:46 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/02 14:04:49 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/*---------------	Exceptions		---------------*/

/*---------------	Constructors	---------------*/
Channel::Channel(void)
{
	if (VERBOSE >= 3)
		std::cout << DGREEN << "Channel default constructor called" << WHITE << std::endl;
}

Channel::Channel(std::string &name)
{
	if (VERBOSE >= 3)
		std::cout << DGREEN << "Channel argument constructor called" << WHITE << std::endl;
	this->_name = name;
}

Channel::Channel(Channel const& other)
{
	if (VERBOSE >= 3)
		std::cout << YELLOW << "Channel copy constructor called" << WHITE << std::endl;
	*this = other;
}

/*---------------	Destructor		---------------*/
Channel::~Channel(void)
{
	if (VERBOSE >= 3)
		std::cout << RED << "Channel destructor called" << WHITE << std::endl;
	this->_mode.clear();
	this->_users.clear();
	this->_operators.clear();
}

/*---------------	Operators		---------------*/
Channel& Channel::operator=(Channel const& other)
{
	if (VERBOSE >= 3)
		std::cout << BLUE << "Channel assignment operator called" << WHITE << std::endl;
	if (this != &other)
	{
		this->_name		= other._name;
		this->_topic	= other._topic;
		this->_password	= other._password;
		this->_mode		= other._mode;
		this->_users	= other._users;
		this->_operators = other._operators;
	}
	return *this;
}

/*---------------	Methods			---------------*/
void	print(void)
{
	std::cout << "*--------------------CHANNEL--------------------*" << std::endl;
	std::cout << "|name:\t\t|" << this->_name << "|" << std::endl;
	std::cout << "|topic:\t\t|" << this->_topic << "|" << std::endl;
	std::cout << "|password:\t|" << this->_password << "|" << std::endl;
	std::cout << "|modes:\t\t|";
	set::const_iterator it = this->_mode.begin();
	while (it != this->_mode.end())
	{
		std::cout << *it << ",";
		it++;
	}
	std::cout << "|" << std::endl;
	std::cout << "|users:\t\t|";
	map::const_iterator it = this->_users.begin();
	while (it != this->_users.end())
	{
		std::cout << *it->second << ",";
		it++;
	}
	std::cout << "|" << std::endl;
	std::cout << "|operators\t|";
	map::const_iterator it = this->_operators.begin();
	while (it != this->_operators.end())
	{
		std::cout << *it->second << ",";
		it++;
	}
	std::cout << "|" << std::endl;
	std::cout << "*-----------------------------------------------*" << std::endl;
}

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
