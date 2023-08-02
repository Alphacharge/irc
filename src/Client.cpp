/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 10:01:01 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/02 18:26:27 by lsordo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/* === CANONICAL IMPLEMENTATION === */

Client::Client(void) {
	if (VERBOSE)
		std::cout << "Client default constructor called" << std::endl;
}

Client::Client(Client const& src) {
	if (VERBOSE)
		std::cout << "Client copy constructor called" << std::endl;
	*this = src;
}

Client::~Client(void) {
	if (VERBOSE)
		std::cout << "Client destructor called" << std::endl;
}

Client&	Client::operator=(Client const& rhs) {
	if (VERBOSE)
		std::cout << "Client operator= called" << std::endl;
	if (this != &rhs) {
		this->_clientAddress = rhs._clientAddress;
		this->_clientPollfd = rhs._clientPollfd;
		this->_clientSocket = rhs._clientSocket;
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
}

int&				Client::getClientSocket(void) {
	return this->_clientSocket;
}

struct sockaddr_in&	Client::getClientAddress(void) {
	return this->_clientAddress;
}

pollfd&				Client::getClientPollfd(void) {
	return this->_clientPollfd;
}

std::string	Client::getName(void) {
	return this->_name;
}

std::string&	Client::getBuffer(void) {
	return this->_buffer;
}

void	Client::appendBuffer(std::string const& str) {
	this->_buffer.append(str);
}

