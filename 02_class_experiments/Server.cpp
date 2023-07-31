/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 14:45:04 by lsordo            #+#    #+#             */
/*   Updated: 2023/07/31 16:10:49 by lsordo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* === CANONICAL IMPLEMENTATION === */
Server::Server(void) {
	if (VERBOSE)
		std::cout << "Server default constructor called" << std::endl;
}

Server::Server(int port, std::string password) : _serverPort(port), _serverPassword(password) {
	if (VERBOSE)
		std::cout << "Server parametric constructor called" << std::endl;
}

Server::Server(Server const& src) {
	if (VERBOSE)
		std::cout << "Server copy constructor called" << std::endl;
	*this = src;
}

Server::~Server(void) {
	if (VERBOSE)
		std::cout << "Server destructor called" << std::endl;
}

Server&	Server::operator=(Server const& rhs) {
	if (VERBOSE)
		std::cout << "Server operator= called" << std::endl;
	if (this != &rhs) {
		this->_serverPassword = rhs._serverPassword;
		this->_serverPort = rhs._serverPort;
	}
	return *this;
}

/* === GETTERS === */
std::string	Server::getPassword(void) const {
	return this->_serverPassword;
}

int	Server::getPort(void) const {
	return this->_serverPort;
}

/* === EXCEPTIONS === */
char const* Server::SocketException::what() const throw() { return ("Error creating socket.");}

char const* Server::BindException::what() const throw() { return ("Error binding socket.");}

char const* Server::ListenException::what() const throw() { return ("Error listening on socket.");}

char const* Server::PollException::what() const throw() { return ("Error in poll.");}

/* === FUNCTIONS === */
void	Server::serverSetup(void) {

	struct sockaddr_in	serverAddress;
	int					serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	try {
		if (serverSocket < 0)
			throw	Server::SocketException();
		bzero(&serverAddress, sizeof(serverAddress));
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(this->_serverPort);
		serverAddress.sin_addr.s_addr = INADDR_ANY;
		if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress) < 0))
			throw	Server::BindException();
		if (listen(serverSocket, 5) < 0)
			throw	Server::ListenException();
		std::cout << "Server started and listening on port " << this->_serverPort << std::endl;
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}
