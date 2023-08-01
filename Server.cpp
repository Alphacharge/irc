/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 14:45:04 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/01 16:49:37 by lsordo           ###   ########.fr       */
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
		this->_serverAddress = rhs._serverAddress;
		this->_fds = rhs._fds;
		this->_serverPollfd = rhs._serverPollfd;
	}
	return *this;
}

/* === EXCEPTIONS === */
char const* Server::SocketException::what() const throw() { return ("Error creating socket.");}

char const* Server::BindException::what() const throw() { return ("Error binding socket.");}

char const* Server::ListenException::what() const throw() { return ("Error listening on socket.");}

char const* Server::AcceptException::what() const throw() { return ("Error accepting connection.");}

char const* Server::PollException::what() const throw() { return ("Error in poll.");}

char const* Server::FdException::what() const throw() { return ("Error with the connection.");}

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
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

void	Server::serverPoll(void) {
	int numReady = poll(&this->_fds[0], this->_fds.size(), -1);
	if (numReady < 0) {throw PollException();}
	if (this->_fds[0].revents & POLLIN) {
	Client		newClient;
	socklen_t	clientAddressLen = sizeof(newClient.getClientAddress());
	newClient.setClientSocket(accept(this->_serverSocket, (struct sockaddr*)&newClient.getClientAddress(), &clientAddressLen));
	if (newClient.getClientSocket() < 0) {throw AcceptException();}
	this->_clientVector.push_back(newClient);
	std::cout << "New client connected : " << inet_ntoa(newClient.getClientAddress().sin_addr) << ":" << ntohs(newClient.getClientAddress().sin_port) << std::endl;
	newClient.setClientPollfd_fd(newClient.getClientSocket());
	newClient.setClientPollfd_events(POLLIN);
	this->_fds.push_back(newClient.getClientPollfd());
	}
}

void	Server::handleClientMessage(char* buffer) {
	std::cout << "Incoming from client : " << buffer;
}

void	Server::serverStart(void) {
	try {
		serverSetup();
		while(true) {
			serverPoll();
			char	buffer[1024];
			for (size_t i = 1; i < this->_fds.size(); ++i) {
				if ((this->_fds[i].revents & POLLIN) && this->_fds[i].fd != this->_fds[0].fd) {
					bzero(buffer, sizeof(buffer));
					size_t ret = recv(this->_fds[i].fd, buffer, sizeof(buffer), 0);
					if(ret > 0)
						handleClientMessage(buffer);
					else if (this->_fds[i].revents & (POLLERR | POLLHUP) || ret <= 0)
					{
						std::cout << "Client disconnected" << std::endl;
						this->_fds.erase(this->_fds.begin() + i);
						this->_clientVector.erase(this->_clientVector.begin() + i - 1);
						--i;
					}
				}
			}
		}
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}
