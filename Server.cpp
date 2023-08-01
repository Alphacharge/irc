/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 14:45:04 by lsordo            #+#    #+#             */
/*   Updated: 2023/07/31 19:51:26 by lsordo           ###   ########.fr       */
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
		this->_serverPort = rhs._serverPort;
		this->_serverPassword = rhs._serverPassword;
		this->_serverSocket = rhs._serverSocket;
		this->_optval = rhs._optval;
		this->_clientSocket = rhs._clientSocket;
		this->_serverAddress = rhs._serverAddress;
		this->_clientAddress = rhs._clientAddress;
		this->_fds = rhs._fds;
		this->_serverPollfd = rhs._serverPollfd;
		this->_clientPollfd = rhs._clientPollfd;
	}
	return *this;
}

/* === EXCEPTIONS === */
char const* Server::SocketException::what() const throw() { return ("Error creating socket.");}

char const* Server::BindException::what() const throw() { return ("Error binding socket.");}

char const* Server::ListenException::what() const throw() { return ("Error listening on socket.");}

char const* Server::AcceptException::what() const throw() { return ("Error accepting connection.");}

char const* Server::PollException::what() const throw() { return ("Error in poll.");}

/* === FUNCTIONS === */
void	Server::serverSetup(void) {

	try {
		this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (this->_serverSocket < 0) {throw	Server::SocketException();}
		this->_optval = 1;
		if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &this->_optval, sizeof(this->_optval))< 0) {throw SocketException();}
		this->_serverAddress.sin_family = AF_INET;
		this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
		this->_serverAddress.sin_port = htons(this->_serverPort);
		if (bind(this->_serverSocket, (struct sockaddr*)&this->_serverAddress, sizeof(this->_serverAddress)) < 0) {throw Server::BindException();}
		if (listen(this->_serverSocket, 5) < 0) {throw Server::ListenException();}
		std::cout << "Server started and listening on port " << this->_serverPort << std::endl;
		this->_serverPollfd.fd = this->_serverSocket;
		this->_serverPollfd.events = POLLIN;
		this->_fds.push_back(this->_serverPollfd);
		while(true) {
			int numReady = poll(&this->_fds[0], this->_fds.size(), -1);
			if (numReady < 0) {throw PollException();}
			if (this->_fds[0].revents & POLLIN) {
				socklen_t	clientAddressLen = sizeof(this->_clientAddress);
				this->_clientSocket = accept(this->_serverSocket, (struct sockaddr*)&this->_clientAddress, &clientAddressLen); // implicit casting sucks
				if (this->_clientSocket < 0) {throw AcceptException();}
				std::cout << "New client connected : " << inet_ntoa(this->_clientAddress.sin_addr) << ":" << ntohs(this->_clientAddress.sin_port) << std::endl;
				this->_clientPollfd.fd = this->_clientSocket;
				this->_clientPollfd.events = POLLIN;
				this->_fds.push_back(this->_clientPollfd);
			}
		}
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}
