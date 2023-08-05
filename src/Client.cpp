/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 10:01:01 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/05 20:31:17 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/* === CANONICAL IMPLEMENTATION === */

Client::Client(void) {
	if (VERBOSE >= 3)
		std::cout << DGREEN << "Client default constructor called" << WHITE << std::endl;
	_status = CONNECTED;
}

Client::Client(Client const& src) {
	if (VERBOSE >= 3)
		std::cout << YELLOW << "Client copy constructor called" << WHITE << std::endl;
	*this = src;
}

Client::~Client(void) {
	if (VERBOSE >= 3)
		std::cout << RED << "Client destructor called" << WHITE << std::endl;
}

Client&	Client::operator=(Client const& rhs) {
	if (VERBOSE >= 3)
		std::cout << BLUE << "Client assignment operator called" << WHITE << std::endl;
	if (this != &rhs) {
		this->_clientSocket = rhs._clientSocket;
		this->_clientAddress = rhs._clientAddress;
		this->_clientPollfd = rhs._clientPollfd;
		this->_nick = rhs._nick;
		this->_username = rhs._username;
		this->_status = rhs._status;
		this->_buffer = rhs._buffer;
	}
	return *this;
}

/* === GETTERS - SETTERS === */
void Client::setClientSocket(int const& socket) {
	this->_clientSocket = socket;
}

void Client::setClientAddress(struct sockaddr_in const& address) {
	this->_clientAddress = address;
}

void Client::setClientPollfd_fd(int const& fd) {
	this->_clientPollfd.fd = fd;
}

void Client::setClientPollfd_events(short const& event) {
	this->_clientPollfd.events = event;
	this->_clientPollfd.revents = 0;
}

void	Client::setNick(std::string nick) {
	_nick = nick;
}

void	Client::setUsername(std::string username) {
	_username = username;
}

void	Client::setStatus(int status)
{
	_status = status;
}

int&	Client::getClientSocket(void) {
	return this->_clientSocket;
}

struct sockaddr_in&	Client::getClientAddress(void) {
	return this->_clientAddress;
}

pollfd&	Client::getClientPollfd(void) {
	return this->_clientPollfd;
}

std::string&	Client::getNick(void) {
	return this->_nick;
}

std::string&	Client::getUsername(void) {
	return (_username);
}

std::string&	Client::getBuffer(void) {
	return this->_buffer;
}

void	Client::appendBuffer(std::string const& str) {
	this->_buffer.append(str);
}

int	Client::getStatus(void) {
	return (_status);
}

void	Client::print(void) {
	std::cout << LGREEN << "-----Printing Client:-----" << WHITE << std::endl;
	std::cout << LGREEN << "Name:\t|" << this->_username << WHITE << std::endl;
	std::cout << LGREEN << "Nick:\t|" << this->_nick << WHITE << std::endl;
	std::cout << LGREEN << "Status:\t|" << this->_status << WHITE << std::endl;
	std::cout << LGREEN << "Buffer:\t|" << this->_buffer << WHITE << std::endl;
}
