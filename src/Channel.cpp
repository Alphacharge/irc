/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 08:37:46 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/11 08:09:35 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/*---------------	Exceptions		---------------*/

/*---------------	Constructors	---------------*/
Channel::Channel(void) {
	if (VERBOSE >= 3)
		std::cout << DGREEN << "Channel default constructor called" << WHITE << std::endl;
}

Channel::Channel(std::string &name) {
	if (VERBOSE >= 3)
		std::cout << DGREEN << "Channel argument constructor called" << WHITE << std::endl;
	this->_name = name;
	this->_inviteonly = false;
	this->_restrictTopic = false;
	this->_limit = -1;
	this->_topic = "";
	this->_topicSetat = "";
	this->_topicSetby = "";
	this->_password = "";
	this->_restrictTopic = false;
}

Channel::Channel(Channel const& other) {
	if (VERBOSE >= 3)
		std::cout << YELLOW << "Channel copy constructor called" << WHITE << std::endl;
	*this = other;
}

/*---------------	Destructor		---------------*/
Channel::~Channel(void) {
	if (VERBOSE >= 3)
		std::cout << RED << "Channel destructor called" << WHITE << std::endl;
	this->_mode.clear();
	this->_banns.clear();
	this->_users.clear();
	this->_operators.clear();
}

/*---------------	Operators		---------------*/
Channel& Channel::operator=(Channel const& other) {
	if (VERBOSE >= 3)
		std::cout << BLUE << "Channel assignment operator called" << WHITE << std::endl;
	if (this != &other)
	{
		this->_name				= other._name;
		this->_topic			= other._topic;
		this->_restrictTopic	= other._restrictTopic;
		this->_topicSetat		= other._topicSetat;
		this->_topicSetby		= other._topicSetby;
		this->_password			= other._password;
		this->_inviteonly		= other._inviteonly;
		this->_limit			= other._limit;
		this->_mode				= other._mode;
		this->_banns			= other._banns;
		this->_users			= other._users;
		this->_operators 		= other._operators;
	}
	return *this;
}

/*---------------	Methods			---------------*/
void	Channel::print(void) {
	std::cout << MAGENTA << "*--------------------CHANNEL--------------------*" << std::endl;
	std::cout << "|name:\t\t|" << WHITE << this->_name <<  MAGENTA <<"|" << std::endl;
	std::cout << "|topic:\t\t|" << WHITE << this->_topic <<  MAGENTA <<"|" << std::endl;
	std::cout << "|password:\t|" << WHITE << this->_password <<  MAGENTA <<"|" << std::endl;
	std::cout << "|inviteonly:\t|" << WHITE << this->_inviteonly <<  MAGENTA <<"|" << std::endl;
	std::cout << "|topic restricted:\t|" << WHITE << this->_restrictTopic << MAGENTA << "|" <<std::endl;
	std::cout << "|limit:\t|" << WHITE << this->_limit <<  MAGENTA <<"|" << std::endl;
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
		std::cout << it2->second.getNick() << ",";
		it2++;
	}
	std::cout <<  MAGENTA <<"|" << std::endl;
	std::cout << "|operators\t|" << WHITE;
	std::map<std::string, Client>::iterator it3 = this->_operators.begin();
	while (it3 != this->_operators.end())
	{
		std::cout << it3->second.getNick() << ",";
		it3++;
	}
	std::cout <<  MAGENTA <<"|" << std::endl;
	std::cout << "*-----------------------------------------------*" << WHITE << std::endl;
}

std::string	Channel::getName(void) const {
	return this->_name;
}

std::string	Channel::getPassword(void) const {
	return this->_password;
}

void	Channel::setPassword(std::string pw) {
	this->_password = pw;
}

bool	Channel::isInviteOnly(void) const {
	return this->_inviteonly;
}

void	Channel::setInviteOnly(bool yesOrNo) {
	_inviteonly = yesOrNo;
}

void	Channel::removeInvite(Client& client) {
	this->_inviteList.erase(client.getNick());
}
void	Channel::setRestrictTopic(bool yesOrNo) {
	_restrictTopic = yesOrNo;
}

void	Channel::setOperatorStatus(Client &client) {
	this->_operators[client.getNick()] = client;
	this->_users.erase(client.getNick());
}

void	Channel::removeOperator(Client& client) {
	this->_operators.erase(client.getNick());
}

void	Channel::setOperator(Client& client) {
	this->_operators[client.getNick()] = client;
}

void	Channel::removeOperatorStatus(Client& client) {
	this->removeOperator(client);
	this->_users[client.getNick()] = client;
}

bool	Channel::isOperator(Client& client)
{
	std::map<std::string, Client>::iterator found = this->_operators.find(client.getNick());
	if (found == this->_operators.end())
		return (false);
	return (true);
}

std::map<std::string, Client>	Channel::getOperators(void) {
	return this->_operators;
}

void	Channel::setUser(Client &client) {
	this->_users[client.getNick()] = client;
}

std::map<std::string, Client>	Channel::getUsers(void) {
	return this->_users;
}

bool	Channel::isUser(Client& client)
{
	std::map<std::string, Client>::iterator found = this->_users.find(client.getNick());
	if (found == this->_users.end())
		return (false);
	return (true);
}

void	Channel::removeUser(Client& client) {
	this->_users.erase(client.getNick());
}

std::string	Channel::getTopic(void) {
	return this->_topic;
}

void		Channel::setTopic(std::string topic) {
	this->_topic = topic;
}

std::string		Channel::getTopicSetat(void) {
	return this->_topicSetat;
}

void			Channel::setTopicSetat(void) {
	this->_topicSetat = currentTimeString();
}

std::string		Channel::getTopicSetby(void) {
	return this->_topicSetby;
}

void			Channel::setTopicSetby(std::string nickName) {
	this->_topicSetby = nickName;
}

void	Channel::clearTopic(void) {
	this->_topic.clear();
	this->_topicSetat.clear();
	this->_topicSetby.clear();
	this->_topicSetby.clear();
}

std::map<std::string, Client>	Channel::getAllMember(void) {
	std::map<std::string, Client>			copy = this->_users;
	std::map<std::string, Client>::iterator	it = this->_operators.begin();
	while (it != this->_operators.end())
	{
		copy[it->first] = it->second;
		it++;
	}

	return copy;
}

int	Channel::getAmountOfAll(void) {
	return (this->_users.size() + this->_operators.size());
}

int	Channel::getLimit(void) {
	return this->_limit;
}

void	Channel::setLimit(int limit) {
	if (limit > 0)
		_limit = limit;
}

void	Channel::setLimit(std::string& limit) {
	std::istringstream	iss(limit);
	int					integer;

	if (iss >> integer && integer > 0)
		_limit = integer;
}

void	Channel::removeLimit() {
	_limit = -1;
}

bool	Channel::isMember(std::string& nick) {
	if (this->_users.find(nick) != this->_users.end() || this->_operators.find(nick) != this->_operators.end())
		return (true);
	return (false);
}

bool	Channel::isTopicRestricted(void) {
	return this->_restrictTopic;
}

void	Channel::bann(Client &client) {
	if (!isBanned(client))
		this->_banns.insert(&client);
}

bool	Channel::isBanned(Client &client) {
	std::set<Client*>::iterator it = this->_banns.find(&client);
	if (it == this->_banns.end())
		return false;
	return true;
}

std::string	Channel::genUserlist(void) {
	std::string userlist;
	std::map<std::string, Client>::iterator it = this->_users.begin();
	if (it != this->_users.end()) {
		userlist += it->second.getNick();
		it++;
	}
	while (it != this->_users.end())
	{
		userlist += " " + it->second.getNick();
		it++;
	}
	it = this->_operators.begin();
	if (it != this->_operators.end() && userlist.empty()){
		userlist += "@" + it->second.getNick();
		it++;
	}
	while (it != this->_operators.end())
	{
		userlist += " @" + it->second.getNick();
		it++;
	}
	return userlist;
}

std::map<std::string, Client>	Channel::getInviteList(void) {
	return this->_inviteList;
}

void	Channel::setInviteList(Client& client) {
	this->_inviteList[client.getNick()] = client;
}
