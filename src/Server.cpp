/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 14:45:04 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/04 13:04:54 by rbetz            ###   ########.fr       */
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
	this->_commandMap["PASS"] = &Server::pass;
	this->_commandMap["NICK"] = &Server::nick;
	this->_commandMap["USER"] = &Server::user;
	this->_commandMap["QUIT"] = &Server::quit;
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
	this->_channel_list.clear();
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
		this->_clientVector = rhs._clientVector;
		this->_channel_list = rhs._channel_list;
		this->_commandMap = rhs._commandMap;
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

		//setting socket to non-blocking as required in subject
		fcntl(_serverSocket, F_SETFL, O_NONBLOCK);

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

	//setting socket to non-blocking as required in subject
	fcntl(newClient.getClientSocket(), F_SETFL, O_NONBLOCK);
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
	/* remove initial ':' if present, e.g. KVirc PASS :pw */
	if (!clientCommand.parameters.empty() && clientCommand.parameters.size() > 1 && clientCommand.parameters[0] == ':')
		clientCommand.parameters = clientCommand.parameters.substr(1,clientCommand.parameters.size());
	/* split parameters string on space, into list of strings */
	if (!clientCommand.parameters.empty())
		clientCommand.parametersList = splitString(clientCommand.parameters, ' ');

	// /* === START debug parametersList === */
	// if (VERBOSE >= 3)
	// {
	// 	std::cout << "DEBUG test parametersList : " << std::endl;
	// 	for (std::list<std::string>::iterator it = clientCommand.parametersList.begin(); it != clientCommand.parametersList.end(); ++it) {std::cout << *it << std::endl;}
	// }
	// /* === END   debug parametersList === */

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
							std::cout << "------------\n";
							std::cout << "Status     : " << clientIterator->getStatus() << std::endl;
							std::cout << "Prefix     : " << it->prefix << std::endl;
							std::cout << "Command    : " << it->command << std::endl;
							std::cout << "Parameters : " << it->parameters << std::endl;
							std::map<std::string, void (Server::*)(Client&, t_ircMessage&)>::iterator	function = this->_commandMap.find(it->command);
							if (function != _commandMap.end())
								(this->*(function->second))(*clientIterator, *it);
							else if (VERBOSE >= 3)
								std::cout << "Command not found.\n";
						}
						commands.clear();
					}
					if (!this->_clientVector[i - 1].getStatus() || this->_fds[i].revents & (POLLERR | POLLHUP) || ret < 0)
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

void	Server::broadcastMessage(std::map<std::string, Client> map, std::string channel, std::string message){
	std::map<std::string, Client>::iterator it = map.begin();
	while (it != map.end()) {
		if (message == "JOIN")
			sendMessage(it->second, JOIN(it->second, inet_ntoa(it->second.getClientAddress().sin_addr), channel));
		it++;
	}
}

/* === COMMANDS === */

void	Server::join(Client &client, t_ircMessage& params){
	//---> that does not work @ fkernbac
	// if (client.getStatus() != REGISTERED)
	// 	return;
// std::cout << "1\n";
	if (params.parameters.empty()) {
		sendMessage(client, ERR_NEEDMOREPARAMS(params.command));
		return ;
	}
std::cout << "2\n";
	if (params.parametersList.size() == 1)
		params.parametersList.push_back("");
	std::list<std::string> tojoin = splitString(params.parametersList.front(), ',');
	std::list<std::string> tojoinpw = splitString(params.parametersList.back(), ',');
	std::list<std::string>::iterator it_join = tojoin.begin();
	std::list<std::string>::iterator it_joinpw = tojoinpw.begin();
	std::cout << RED << "Channel:|" << *it_join << "|PW:|" << *it_join << "|" << WHITE << std::endl;
	while (it_join != tojoin.end()) {
		//--->We found on official Server no Character that triggers this error
		// if (!isValidChannelName(*it_join)) {
		// 	sendMessage(client, ERR_BADCHANMASK(*it_join));
		// 	return ;
		// }
		std::list<Channel>::iterator it_chan = this->_channel_list.begin();
std::cout << "7\n";
		while (it_chan != this->_channel_list.end()) {
			if (VERBOSE >= 3)
				std::cout << CYAN << client.getNick() << " tries to join " << *it_join << ". Testing: " << it_chan->getName() << WHITE << std::endl;
// std::cout << "8\n";
			if (it_chan->getName() == *it_join && it_chan->getInvite() == true) {
				sendMessage(client, ERR_INVITEONLYCHAN(*it_join));
				return ;
			}
// std::cout << "9\n";
			if (it_chan->getName() == *it_join && it_chan->getAmountOfAll() == it_chan->getLimit()) {
				sendMessage(client, ERR_CHANNELISFULL(*it_join));
				return ;
			}
			//possible proble if no password exist in the command, also valid iterator of password is not secured
			// if (it_chan->getName() == *it_join && it_chan->getPassword() != *it_joinpw) {
			// 	sendMessage(client, ERR_BADCHANNELKEY(*it_join));
			// 	return ;
			// }
// std::cout << "10\n";
			if (it_chan->getName() == *it_join && it_chan->isBanned(client)) {
				sendMessage(client, ERR_BANNEDFROMCHAN(*it_join));
				return ;
			}
// std::cout << "11\n";
			//--->We don't have a Channellimit, if we want just create this variables and the getter
			// if (it_chan->getName() == *it_join && this->_channelLimit == client->getAmountOfChannels()) {
			// 	sendMessage(client, ERR_TOOMANYCHANNELS(*it_join));
			// 	return ;
			// }
			//--->We don't support shortnames for channel names
			//ERR_TOOMANYTARGETS
			//	Returned to a client which is attempting to JOIN a safe
			//	channel using the shortname when there are more than one
			//	such channel.
			//--->We don't support delay mechanism
			//ERR_UNAVAILRESOURCE
			//	Returned by a server to a user trying to join a channel
			//	currently blocked by the channel delay mechanism.
			if (*it_join != it_chan->getName())
				it_chan++;
			else
				break;
		}
		//--->Not clear in which case we send Nosuchchannel, because we create
		// if (it_chan == this->_channel_list.end()) {
		// 	sendMessage(client, ERR_NOSUCHCHANNEL(*it_join));
		// 	return ;
		// }
// std::cout << "12\n";
		if (it_chan == this->_channel_list.end())
		{
			Channel newCH(*it_join);
// std::cout << "13\n";
			newCH.setOperator(client);
			newCH.setPassword(*it_joinpw);
// std::cout << "14\n";
			this->_channel_list.push_back(newCH);
// std::cout << "15\n";
			broadcastMessage(newCH.getOperators(), newCH.getName(), "JOIN");
// std::cout << "16\n";
			sendMessage(client, USERLIST(inet_ntoa(this->_serverAddress.sin_addr), client, *it_join, newCH.genUserlist()));
// std::cout << "17\n";
			sendMessage(client, USERLISTEND(inet_ntoa(this->_serverAddress.sin_addr), client, *it_join));
			// newCH.print();
		} else {
// std::cout << "18\n";
			it_chan->setUser(client);
// std::cout << "19\n";
			// here a problem, the biradcast should contain the nick of the joining client, not the iterator one
			broadcastMessage(it_chan->getUsers(), it_chan->getName(), "JOIN");
// std::cout << "20\n";
			broadcastMessage(it_chan->getOperators(), it_chan->getName(), "JOIN");
// std::cout << "21\n";
			sendMessage(client, USERLIST(inet_ntoa(this->_serverAddress.sin_addr), client, it_chan->getName(), it_chan->genUserlist()));
// std::cout << "22\n";
			sendMessage(client, USERLISTEND(inet_ntoa(this->_serverAddress.sin_addr), client, it_chan->getName()));
		}
// std::cout << "23\n";
		it_join++;
		it_joinpw++;
	}
	printAllChannels();
}

void	Server::cap(Client& client, t_ircMessage& params) {
	if (params.parameters == "LS") {
		client.setStatus(CAP);
		sendMessage(client, RPL_CAP);
		if (VERBOSE >= 3)
			std::cout << ORANGE << "DEBUG: sent RPL_CAP" << WHITE << std::endl;
	}
	if (params.parameters == "END")
		client.setStatus(CONNECTED);
}

void	Server::pong(Client &client, t_ircMessage& params) {
	sendMessage(client, PONG(params.parameters));
}

void	Server::pass(Client &client, t_ircMessage& params) {
	if (VERBOSE >= 3)
		std::cout  << ORANGE << "DEBUG: server password " << _serverPassword << " | " << params.parameters.substr(1) << "\n" << WHITE;

	if (params.parameters.empty())
		sendMessage(client, ERR_NEEDMOREPARAMS(params.parameters));
	else if (client.getStatus() >= AUTHENTICATED)
		sendMessage(client, ERR_ALREADYREGISTERED);
	else if (_serverPassword == params.parameters)
	{
		client.setStatus(AUTHENTICATED);
		if (VERBOSE >= 3)
			std::cout << ORANGE << "DEBUG: Password accepted.\n" << WHITE;
	}
	else
		sendMessage(client, ERR_PASSWDMISMATCH);
}

void	Server::nick(Client &client, t_ircMessage& params) {
	std::string	oldNick = client.getNick();
	std::cout << "DEBUG: oldNICK " << oldNick << "\n";

	//check if a password was supplied
	if (client.getStatus() < AUTHENTICATED)
		return;

	//check if there is a name
	if (params.parametersList.size() == 0)
		sendMessage(client, ERR_NONICKNAMEGIVEN);

	//MISSING: check character validity ERR_ERRONEUSNICKNAME

	//check uniqueness
	for (std::vector<Client>::iterator it = _clientVector.begin(); it != _clientVector.end(); it++) {
		if (VERBOSE >= 3)
			std::cout << ORANGE << "DEBUG: comparing " << params.parameters << " to " << it->getNick() << WHITE << std::endl;
		if (it->getStatus() >= AUTHENTICATED && it->getNick() == params.parameters) {
			sendMessage(client, ERR_NICKNAMEINUSE(params.parameters));
			return;
		}
	}

	//set nickname
	client.setNick(params.parametersList.front());
	client.setStatus(NICKGIVEN);
	if (VERBOSE >= 3)
		std::cout << ORANGE << "DEBUG: set nick " << client.getNick() << WHITE << std::endl;

	//check registration status
	if (client.getStatus() == USERGIVEN)
	{
		client.setStatus(REGISTERED);
		sendMessage(client, RPL_WELCOME(client));
	}
	else if (client.getStatus() == REGISTERED)
		sendMessage(client, NICK(oldNick, client));
}

void	Server::user(Client& client, t_ircMessage& params) {
	//check if a password was supplied
	if (client.getStatus() >= AUTHENTICATED)
		return;

	//set username
	if (client.getStatus() == REGISTERED)
		sendMessage(client, ERR_ALREADYREGISTERED);
	else if (params.parametersList.size() == 0)
		client.setUsername("unknown");
	else
		client.setUsername(params.parametersList.front());
	if (VERBOSE >= 3)
		std::cout << ORANGE << "DEBUG: set username " << client.getUsername() << WHITE << std::endl;

	//check registration status
	if (client.getStatus() == NICKGIVEN)
	{
		client.setStatus(REGISTERED);
		sendMessage(client, RPL_WELCOME(client));
	}
	else
		client.setStatus(USERGIVEN);
}

void	Server::quit(Client& client, t_ircMessage& params) {
	(void)params;
	sendMessage(client, ERROR((std::string)"Connection closed."));
	client.setStatus(DISCONNECTED);
	//MISSING: message to all other clients
	// broadcastMessage(channel.getUsers(), channel.getName(), "x left the channel");
}

void	Server::printAllClients(void) {
	std::vector<Client>::iterator it = this->_clientVector.begin();
	while (it != this->_clientVector.end())
	{
		it->print();
		it++;
	}
}

void	Server::printAllChannels(void) {
	std::list<Channel>::iterator it = this->_channel_list.begin();
	while (it != this->_channel_list.end())
	{
		it->print();
		it++;
	}
}
