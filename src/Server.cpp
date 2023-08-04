/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 14:45:04 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/04 07:38:21 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* === CANONICAL IMPLEMENTATION === */
Server::Server(void) {
	if (VERBOSE >= 3)
		std::cout << DGREEN << "Server default constructor called" << WHITE << std::endl;
}

Server::Server(int port, std::string password) : _serverPort(port), _serverPassword(password) {
	if (VERBOSE >= 3)
		std::cout << DGREEN << "Server parametric constructor called" << WHITE << std::endl;
	this->_commandMap["JOIN"] = &Server::join;
	this->_commandMap["CAP"] = &Server::cap;
	this->_commandMap["PING"] = &Server::pong;
	this->_commandMap["NICK"] = &Server::nick;
	this->_commandMap["USER"] = &Server::user;
}

Server::Server(Server const& src) {
	if (VERBOSE >= 3)
		std::cout << YELLOW << "Server copy constructor called" << WHITE << std::endl;
	*this = src;
}

Server::~Server(void) {
	if (VERBOSE >= 3)
		std::cout << RED << "Server destructor called" << WHITE << std::endl;
	this->_fds.clear();
	this->_clientVector.clear();
}

Server&	Server::operator=(Server const& rhs) {
	if (VERBOSE >= 3)
		std::cout << BLUE << "Server assignment operator called" << WHITE << std::endl;
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
		this->_serverPollfd.fd = this->_serverSocket;
		this->_serverPollfd.events = POLLIN;
		this->_fds.push_back(this->_serverPollfd);
		std::cout << "Server startup completed. Listening on port " << this->_serverPort << std::endl;
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

	std::cout << "New client connected : " << inet_ntoa(newClient.getClientAddress().sin_addr) << ":" << ntohs(newClient.getClientAddress().sin_port) << std::endl;
	newClient.setClientPollfd_fd(newClient.getClientSocket());
	newClient.setClientPollfd_events(POLLIN);
	newClient.setStatus(CONNECTED);
	this->_clientVector.push_back(newClient);
	this->_fds.push_back(newClient.getClientPollfd());
}

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
	if (!clientCommand.parameters.empty()) {
		if (clientCommand.parameters[clientCommand.parameters.size() - 1] == '\r')
			clientCommand.parameters = clientCommand.parameters.substr(0,clientCommand.parameters.size() - 1);
	}
	else {
		if (clientCommand.command[clientCommand.command.size() - 1] == '\r')
			clientCommand.command = clientCommand.command.substr(0,clientCommand.command.size() - 1);
	}
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
		if (!iss.eof() && clientIterator->getBuffer().find(0, clientIterator->getBuffer().length(), '\n')) {
			clientIterator->getBuffer().clear();
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
				std::vector<t_ircMessage>		commands; // contains all valid client messages
				std::vector<Client>::iterator	clientIterator = this->_clientVector.begin() + i - 1;

				if ((this->_fds[i].revents & POLLIN)) {
					bzero(buffer, sizeof(buffer));

					recv(this->_fds[i].fd, buffer, sizeof(buffer), 0);

					if(ret > 0) {
						handleClient(buffer, clientIterator, commands);
						/* simulates commands implementation */
						for (std::vector<t_ircMessage>::iterator it = commands.begin(); it != commands.end(); ++it) {
							std::cout << "Prefix     : " << it->prefix << std::endl;
							std::cout << "Command    : " << it->command << std::endl;
							std::cout << "Parameters : " << it->parameters << std::endl;

							std::map<std::string, void (Server::*)(Client&, std::string&)>::iterator	function = this->_commandMap.find(it->command);
							if (function != _commandMap.end())
							{
std::cout << "DEBUG: found function\n";
								(this->*(function->second))(*clientIterator, it->parameters);
							}
						}
						commands.clear();
					}
					if (this->_fds[i].revents & (POLLERR | POLLHUP) || ret < 0)
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

void	Server::sendMessage(Client& client, std::string message)
{
	size_t	bytesSent = 0;

	//loop might not be needed?
	// while (bytesSent < message.length())
	// {
		bytesSent += send(client.getClientPollfd().fd, message.c_str(), message.length(), 0);
	// }
}

/* === COMMANDS === */

void	Server::join(Client &client, std::string& channel){
	std::map<std::string, std::string> input = joinSplitInput(channel);
	std::map<std::string, std::string>::iterator it2 = input.begin();
	while (it2 != input.end())
	{
		std::cout << CYAN << "Channel:|" << it2->first << "|\tPassword:|" << it2->second << "|" << WHITE << std::endl;
		it2++;
	}
	std::list<Channel>::iterator it = this->_channel_list.begin();
	while (it != this->_channel_list.end() && it->getName() != channel)
		it++;
	if (it == this->_channel_list.end())
	{
		Channel newCH(channel);
		newCH.setOperator(client);
		this->_channel_list.push_back(newCH);
	} else {
		it->setUser(client);
	}
}

void	Server::cap(Client& client, std::string& params){
	if (params == "LS") {
		client.setStatus(CAP);
		sendMessage(client, RPL_CAP);
std::cout << "DEBUG: sent RPL_CAP";
	}
	if (params == "END")
		client.setStatus(CONNECTED);
}

void	Server::pong(Client &client, std::string& params){
	(void)params;
	sendMessage(client, PONG(params));
}

void	Server::nick(Client &client, std::string& params){
	if (params.empty())
		sendMessage(client, ERR_NONICKNAMEGIVEN);

	client.setNick(params);
	client.setStatus(NICK);
std::cout << "DEBUG: set nick '" << client.getNick() << "'\n";

	if (client.getStatus() == USER)
	{
		client.setStatus(REGISTERED);
		sendMessage(client, RPL_WELCOME(client));
	}
}

void	Server::user(Client &client, std::string& params){
	size_t	length = params.find(" ");

	if (client.getStatus() == REGISTERED)
		sendMessage(client, ERR_ALREADYREGISTERED);

	if (length == 0)
		client.setUsername("unknown");
	else
		client.setUsername(params.substr(0, length));
std::cout << "DEBUG: set username '" << client.getUsername() << "'\n";

	if (client.getStatus() == NICK)
	{
		client.setStatus(REGISTERED);
		sendMessage(client, RPL_WELCOME(client));
	}
	else
		client.setStatus(USER);
}
