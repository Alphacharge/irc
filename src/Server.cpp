/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 14:45:04 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/02 14:23:26 by rbetz            ###   ########.fr       */
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

void	Server::addClient(void) {
	Client		newClient;
	socklen_t	clientAddressLen = sizeof(newClient.getClientAddress());

	newClient.setClientSocket(accept(this->_serverSocket, (struct sockaddr*)&newClient.getClientAddress(), &clientAddressLen));
	if (newClient.getClientSocket() < 0) {throw AcceptException();}

	this->_clientVector.push_back(newClient);
	std::cout << "New client connected : " << inet_ntoa(newClient.getClientAddress().sin_addr) << ":" << ntohs(newClient.getClientAddress().sin_port) << std::endl;
	newClient.setClientPollfd_fd(newClient.getClientSocket());
	newClient.setClientPollfd_events(POLLIN);
	this->_fds.push_back(newClient.getClientPollfd());

	//might have to use fcntl on mac? not sure if needed
	// int	flags = fcntl(newClient.getClientSocket(), F_GETFL, 0);

	// if (fcntl(newClient.getClientSocket(), F_SETFL, flags | O_NONBLOCK) == -1)
	// 	std::cerr << "Setting socket to nonblock failed\n";
}

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

void	Server::parseClientInput(std::string const& message, std::vector<t_ircMessage>& clientInput) {
	size_t			pos = 0;
	bool			goOn = true;
	t_ircMessage	clientCommand;

	while (goOn) {
		if (!message.empty()) {
			size_t splitEnd = message.find('\n', pos);
			std::string inputSplit = message.substr(pos, splitEnd - pos);
			pos = splitEnd + 1;
			if (parseSplit(inputSplit, clientCommand.prefix, clientCommand.command, clientCommand.parameters)) {
				clientInput.push_back(clientCommand);
			}
			if (pos >= message.size()) {goOn = false;}
		}
	}
}

void	Server::handleClient(char* buffer, int const& clientFd) {
	std::vector<t_ircMessage>	clientInput;

	std::cout << "Incoming from client : " << buffer;
	parseClientInput(buffer, clientInput);

	for (std::vector<t_ircMessage>::iterator it = clientInput.begin(); it != clientInput.end(); ++it) {
		std::cout << "-----------\n";
		std::cout << "Prefix     : " << it->prefix << std::endl;
		std::cout << "Command    : " << it->command << std::endl;
		std::cout << "Parameters : " << it->parameters << std::endl;

		//if CAP LS is received, the server will suspend registration until CAP END is received
		if (it->command == "CAP")
			send(clientFd, RPL_CAP.c_str(), RPL_CAP.size(), 0);
		if (it->command == "PING")
			send(clientFd, PONG(it->parameters).c_str(), PONG(it->parameters).size(), 0);
		if (it->command == "NICK") {
			//save client name
			send(clientFd, RPL_WELCOME(it->parameters).c_str(), sizeof(RPL_WELCOME(it->parameters).c_str()), 0);
		}
	}
}

/*Starts up the server and waits in poll until an event is registered.*/
void	Server::serverStart(void) {
	char	buffer[1024];
	int		ret;

	try {
		serverSetup();
		while (true)
		{
			ret = poll(this->_fds.data(), this->_fds.size(), -1);
			if (ret == -1)
				throw PollException();

			//check server
			if (this->_fds[0].revents & POLLIN)
				addClient();

			//check clients
			for (size_t i = 1; i < this->_fds.size(); ++i) {
				if ((this->_fds[i].revents & POLLIN)) {
					bzero(buffer, sizeof(buffer));

					ret = recv(this->_fds[i].fd, buffer, sizeof(buffer), 0);
					if(ret > 0)
						handleClient(buffer, this->_fds[i].fd);
					else if (this->_fds[i].revents & (POLLERR | POLLHUP) || ret <= 0)
					{
						std::cout << "Client disconnected" << std::endl;
						close(this->_fds[i].fd);
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
