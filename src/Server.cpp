/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 14:45:04 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/03 15:44:34 by lsordo           ###   ########.fr       */
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

/*
bool	Server::parseSplit(std::string const& message, std::string& prefix, std::string& command, std::string& parameters) {
	size_t	pos = 0;

	if(!message.empty() && message[pos] == ':') {
		pos++;
		size_t	prefixEnd = message.find(' ', pos);
		if (prefixEnd == std::string::npos) { return false;}
		prefix = message.substr(pos, prefixEnd - pos);
		pos = prefixEnd + 1;
	}
	size_t	commandEnd = message.find(' ', pos);
	if (commandEnd == std::string::npos) { return false;}
	command = message.substr(pos, commandEnd - pos);
	pos = commandEnd + 1;
	parameters = message.substr(pos);
	return true;
}
*/

bool	Server::inputParse(std::string const& message, t_ircMessage& clientCommand) {
	size_t	pos = 0;
	if(!message.empty() && message[pos] == ':') {
		pos++;
		size_t	prefixEnd = message.find(' ', pos);
		if (prefixEnd == std::string::npos) { return false;}
		clientCommand.prefix = message.substr(pos, prefixEnd - pos);
		pos = prefixEnd + 1;
	}
	size_t	commandEnd = message.find(' ', pos);
	if (commandEnd == std::string::npos) { return false;}
	clientCommand.command = message.substr(pos, commandEnd - pos);
	pos = commandEnd + 1;
	clientCommand.parameters = message.substr(pos, message.size() - 2);
	/* trim final CR-LF */
	if (!clientCommand.parameters.empty())
		clientCommand.parameters.substr(0,clientCommand.parameters.size()-2);
	else
		clientCommand.command.substr(0, clientCommand.command.size() - 2);
	return true;
}

void	Server::handleClient(char* buffer, std::vector<Client>::iterator& clientIterator, std::vector<t_ircMessage>& commands) {

	std::istringstream			iss(buffer);
	std::string					tmpBuffer;
	t_ircMessage				clientCommand;

	tmpBuffer.clear();
	while (getline(iss, tmpBuffer)) {
		clientIterator->appendBuffer(tmpBuffer);
			if (inputParse(clientIterator->getBuffer(), clientCommand)) {
				commands.push_back(clientCommand);
				tmpBuffer.clear();
				clientIterator->getBuffer().clear();
			}
	}
}

void	Server::serverStart(void) {
	try {
		serverSetup();
		while(true) {
			serverPoll();
			for (size_t i = 1; i < this->_fds.size(); ++i) {
				char	buffer[1024];
				std::vector<t_ircMessage>	commands; // contains all valid client messages
				std::vector<Client>::iterator	clientIterator = this->_clientVector.begin() + i - 1;
				if ((this->_fds[i].revents & POLLIN)) {
					bzero(buffer, sizeof(buffer));
					size_t ret = recv(this->_fds[i].fd, buffer, sizeof(buffer), 0);
					if(ret > 0) {
						handleClient(buffer, clientIterator, commands);
						/* simulates commands implementation */
						for (std::vector<t_ircMessage>::iterator it = commands.begin(); it != commands.end(); ++it) {
							std::cout << "Prefix     : " << it->prefix << std::endl;
							std::cout << "Command    : " << it->command << std::endl;
							std::cout << "Parameters : " << it->parameters << std::endl;
						}
						commands.clear();
					}
					else if (this->_fds[i].revents & (POLLERR | POLLHUP) || ret < 0)
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
