/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 08:37:46 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/03 12:05:02 by rbetz            ###   ########.fr       */
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
void	Channel::print(void)
{
	std::cout << MAGENTA << "*--------------------CHANNEL--------------------*" << std::endl;
	std::cout << "|name:\t\t|" << WHITE << this->_name <<  MAGENTA <<"|" << std::endl;
	std::cout << "|topic:\t\t|" << WHITE << this->_topic <<  MAGENTA <<"|" << std::endl;
	std::cout << "|password:\t|" << WHITE << this->_password <<  MAGENTA <<"|" << std::endl;
	std::cout << "|modes:\t\t|" << WHITE;
	std::set<t_chmode>::const_iterator it = this->_mode.begin();
	while (it != this->_mode.end())
	{
		std::cout << *it << ",";
		it++;
	}
	std::cout <<  MAGENTA <<"|" << std::endl;
	std::cout << "|users:\t\t|" << WHITE;
	std::map<std::string, Client>::iterator it2 = this->_users.begin();
	while (it2 != this->_users.end())
	{
		std::cout << it2->second.getName() << ",";
		it2++;
	}
	std::cout <<  MAGENTA <<"|" << std::endl;
	std::cout << "|operators\t|" << WHITE;
	std::map<std::string, Client>::iterator it3 = this->_operators.begin();
	while (it3 != this->_operators.end())
	{
		std::cout << it3->second.getName() << ",";
		it3++;
	}
	std::cout <<  MAGENTA <<"|" << std::endl;
	std::cout << "*-----------------------------------------------*" << WHITE << std::endl;
}

std::string	Channel::getName(void) const{
	return this->_name;
}

void		Channel::setOperator(Client &client) {
	this->_operators[client.getNick()] = client;
}

void		Channel::setUser(Client &client) {
	this->_users[client.getNick()] = client;
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
