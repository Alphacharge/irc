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
	this->_commandMap["PRIVMSG"] = &Server::privmsg;
	this->_commandMap["SHUTDOWN"] = &Server::shutdown;
	this->_commandMap["MODE"] = &Server::mode;
	this->_commandMap["INVITE"] = &Server::invite;
	this->_commandMap["KICK"] = &Server::kick;
}

Server::Server(Server const &src) {
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
	this->_commandMap.clear();
}

Server &Server::operator=(Server const &rhs) {
	if (VERBOSE >= 3)
		std::cout << BLUE << "Server assignment operator called" << WHITE << std::endl;
	if (this != &rhs)
	{
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
char const *Server::SocketException::what() const throw() { return ("Error creating socket."); }

char const *Server::BindException::what() const throw() { return ("Error binding socket."); }

char const *Server::ListenException::what() const throw() { return ("Error listening on socket."); }

char const *Server::AcceptException::what() const throw() { return ("Error accepting connection."); }

char const *Server::PollException::what() const throw() { return ("Error in poll."); }

char const *Server::FdException::what() const throw() { return ("Error with the connection."); }

/* === FUNCTIONS === */
void	Server::serverSetup(void)
{
	try
	{
		this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (this->_serverSocket < 0)
		{
			throw Server::SocketException();
		}
		this->_optval = 1;
		if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &this->_optval, sizeof(this->_optval)) < 0)
		{
			throw SocketException();
		}
		this->_serverAddress.sin_family = AF_INET;
		this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
		this->_serverAddress.sin_port = htons(this->_serverPort);
		if (bind(this->_serverSocket, (struct sockaddr *)&this->_serverAddress, sizeof(this->_serverAddress)) < 0)
		{
			throw Server::BindException();
		}
		if (listen(this->_serverSocket, 5) < 0)
		{
			throw Server::ListenException();
		}
		this->_serverPollfd.fd = this->_serverSocket;
		this->_serverPollfd.events = POLLIN;
		this->_fds.push_back(this->_serverPollfd);

		//setting socket to non-blocking as required in subject
		// fcntl(_serverSocket, F_SETFL, O_NONBLOCK);

		std::cout << "Server startup completed. Listening on port " << this->_serverPort << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

/*Starts up the server and waits in poll until an event is registered.*/
void	Server::serverStart(void)
{
	char buffer[1024];
	int ret;

	try
	{
		serverSetup();
		while (true)
		{
			ret = poll(this->_fds.data(), this->_fds.size(), -1);
			if (ret == -1)
				throw PollException();

			// check server
			if (this->_fds[0].revents & POLLIN)
				addClient();

			// check clients
			for (size_t i = 1; i < this->_fds.size(); ++i)
			{
				std::vector<t_ircMessage> commands; // contains all valid client messages
				std::vector<Client>::iterator clientIterator = this->_clientVector.begin() + i - 1;

				if ((this->_fds[i].revents & POLLIN))
				{
					bzero(buffer, sizeof(buffer));

					recv(this->_fds[i].fd, buffer, sizeof(buffer), 0);

					if (ret > 0 && handleClient(buffer, clientIterator, commands))
					{
						for (std::vector<t_ircMessage>::iterator it = commands.begin(); it != commands.end(); ++it)
						{
							std::cout << "------------\n";
							std::cout << "Status     : " << clientIterator->getStatus() << std::endl;
							std::cout << "Prefix     : " << it->prefix << std::endl;
							std::cout << "Command    : " << it->command << std::endl;
							std::cout << "Parameters : " << it->parameters << std::endl;

							std::map<std::string, void (Server::*)(Client &, t_ircMessage &)>::iterator function = this->_commandMap.find(it->command);
							if (function != _commandMap.end())
								(this->*(function->second))(*clientIterator, *it);
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
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

std::vector<Client>::iterator	Server::getClient(std::string& nick) {
	std::vector<Client>::iterator	it = this->_clientVector.begin();
	while (it->getNick() != nick)
		it++;
	return it;
	}

void	Server::broadcastMessage(std::map<std::string, Client> map, std::string message){
	for (std::map<std::string, Client>::iterator it = map.begin(); it != map.end(); it++)
			sendMessage(it->second, message);
}

/* === COMMANDS === */

void	Server::join(Client &client, t_ircMessage& params){
	if (client.getStatus() < REGISTERED) {
		sendMessage(client, ERR_NOTREGISTERED);
		return;
	}
	if (params.parameters.empty()) {
		sendMessage(client, ERR_NEEDMOREPARAMS(params.command));
		return ;
	}
	if (params.parametersList.size() == 1)
		params.parametersList.push_back("");
	std::list<std::string> tojoin = splitString(params.parametersList.front(), ',');
	std::list<std::string> tojoinpw = splitString(params.parametersList.back(), ',');
	std::list<std::string>::iterator it_join = tojoin.begin();
	std::list<std::string>::iterator it_joinpw = tojoinpw.begin();
	std::cout << RED << "Channel:|" << *it_join << "|PW:|" << *it_join << "|" << WHITE << std::endl;
	while (it_join != tojoin.end()) {
		//--->We found on official Server no Character that triggers this error
		if (!isValidChannelName(*it_join)) {
			sendMessage(client, ERR_BADCHANMASK(*it_join));
			return ;
		}
		std::list<Channel>::iterator it_chan = this->_channel_list.begin();
		while (it_chan != this->_channel_list.end()) {
			if (VERBOSE >= 3)
				std::cout << CYAN << client.getNick() << " tries to join " << *it_join << ". Testing: " << it_chan->getName() << WHITE << std::endl;
			// Missing check if client is not invited
			if (it_chan->getName() == *it_join && it_chan->getInvite() == true) {
				sendMessage(client, ERR_INVITEONLYCHAN(*it_join));
				return ;
			}
			//Channellimit reached
			if (it_chan->getName() == *it_join && it_chan->getAmountOfAll() == it_chan->getLimit()) {
				sendMessage(client, ERR_CHANNELISFULL(*it_join));
				return ;
			}
			//possible problem if no password exist in the command, also valid iterator of password is not secured
			if (it_chan->getName() == *it_join && it_joinpw != tojoinpw.end() && it_chan->getPassword() != *it_joinpw) {
				sendMessage(client, ERR_BADCHANNELKEY(*it_join));
				return ;
			}
			if (it_chan->getName() == *it_join && it_chan->isBanned(client)) {
				sendMessage(client, ERR_BANNEDFROMCHAN(*it_join));
				return ;
			}
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
		if (it_chan == this->_channel_list.end())
		{
			Channel newCH(*it_join);
			newCH.setOperator(client);
				// std::cout << RED << *it_joinpw << WHITE << std::endl;
			if (it_joinpw != tojoinpw.end())
				newCH.setPassword(*it_joinpw);
			this->_channel_list.push_back(newCH);
			broadcastMessage(newCH.getOperators(), client, newCH.getName(), "JOIN", "");
			sendMessage(client, USERLIST(inet_ntoa(this->_serverAddress.sin_addr), client, *it_join, newCH.genUserlist()));
			sendMessage(client, USERLISTEND(inet_ntoa(this->_serverAddress.sin_addr), client, *it_join));
		} else {
			it_chan->setUser(client);
			broadcastMessage(it_chan->getAllMember(), client, it_chan->getName(), "JOIN", "");
			sendMessage(client, USERLIST(inet_ntoa(this->_serverAddress.sin_addr), client, it_chan->getName(), it_chan->genUserlist()));
			sendMessage(client, USERLISTEND(inet_ntoa(this->_serverAddress.sin_addr), client, it_chan->getName()));
		}
		it_join++;
		it_joinpw++;
	}
	printAllChannels();
}

void	Server::cap(Client &client, t_ircMessage &params)
{
	if (params.parameters == "LS")
	{
		client.setStatus(CAP);
		sendMessage(client, RPL_CAP);
		if (VERBOSE >= 3)
			std::cout << ORANGE << "DEBUG: sent RPL_CAP" << WHITE << std::endl;
	}
	if (params.parameters == "END")
		client.setStatus(CONNECTED);
}

void	Server::pong(Client &client, t_ircMessage &params)
{
	sendMessage(client, PONG(params.parameters));
}

void	Server::pass(Client &client, t_ircMessage &params)
{
	if (client.getStatus() >= AUTHENTICATED)
		return;

	if (params.parameters.empty())
		sendMessage(client, ERR_NEEDMOREPARAMS(params.parameters));
	else if (client.getStatus() >= AUTHENTICATED)
		sendMessage(client, ERR_ALREADYREGISTERED);
	else if (_serverPassword == params.parameters)
	{
		client.setStatus(AUTHENTICATED);
		if (VERBOSE >= 3)
			std::cout << ORANGE << "DEBUG: Password accepted.\n"
					  << WHITE;
	}
	else
		sendMessage(client, ERR_PASSWDMISMATCH);
}

void	Server::nick(Client &client, t_ircMessage &params)
{
	// check if a password was supplied
	if (client.getStatus() < AUTHENTICATED) {
		sendMessage(client, ERR_NOTREGISTERED);
		return;
	}

	// check if there is a name
	if (params.parametersList.size() == 0)
		sendMessage(client, ERR_NONICKNAMEGIVEN);

	// MISSING: check character validity ERR_ERRONEUSNICKNAME

	//check uniqueness
	for (std::vector<Client>::iterator it = _clientVector.begin(); it != _clientVector.end(); it++) {
		if (VERBOSE >= 3)
			std::cout << ORANGE << "DEBUG: comparing " << params.parameters << " to " << it->getNick() << WHITE << std::endl;
		if (it->getStatus() >= AUTHENTICATED && it->getNick() == params.parameters) {
			sendMessage(client, ERR_NICKNAMEINUSE(params.parameters));
			return;
		}
	}

	// set nickname
	std::string	oldNick = client.getNick();
	client.setNick(params.parametersList.front());
	if (VERBOSE >= 3)
		std::cout << ORANGE << "DEBUG: set nick " << client.getNick() << WHITE << std::endl;

	// check registration status
	switch (client.getStatus())
	{
	case AUTHENTICATED:
		client.setStatus(NICKGIVEN);
		break;
	case USERGIVEN:
		client.setStatus(REGISTERED);
		sendMessage(client, RPL_WELCOME(client));
		break;
	case REGISTERED:
		sendMessage(client, NICK(oldNick, client));
		break;
	default:
		break;
	}
}

void	Server::user(Client& client, t_ircMessage& params) {
	//check if a password was supplied
	if (client.getStatus() < AUTHENTICATED) {
		sendMessage(client, ERR_NOTREGISTERED);
		return;
	}

	// set username
	if (client.getStatus() == REGISTERED)
		sendMessage(client, ERR_ALREADYREGISTERED);
	else if (params.parametersList.size() == 0)
		client.setUsername("unknown");
	else
		client.setUsername(params.parametersList.front());
	if (VERBOSE >= 3)
		std::cout << ORANGE << "DEBUG: set username " << client.getUsername() << WHITE << std::endl;

	// check registration status
	switch (client.getStatus())
	{
	case AUTHENTICATED:
		client.setStatus(USERGIVEN);
		break;
	case NICKGIVEN:
		client.setStatus(REGISTERED);
		sendMessage(client, RPL_WELCOME(client));
		break;
	default:
		break;
	}
}

void	Server::quit(Client &client, t_ircMessage &params)
{
	(void)params;
	sendMessage(client, ERROR((std::string) "Connection closed."));
	client.setStatus(DISCONNECTED);

	//broadcast to all relevant channels
	for (std::list<Channel>::iterator it = _channel_list.begin(); it != _channel_list.end(); it++) {
		if (it->isMember(client))
			broadcastMessage(it->getAllMember(), ":" + client.getNick() + "@irc42" + params.parameters + "; User left the irc network.");
	}
}

void	Server::privmsg(Client &client, t_ircMessage &params)
{
	if (client.getStatus() < REGISTERED) {
		sendMessage(client, ERR_NOTREGISTERED);
		return;
	}
	std::string	textToBeSent;
	size_t	spacePos = params.parameters.find(" ");
	if (spacePos != std::string::npos)
		textToBeSent = params.parameters.substr(spacePos + 1, params.parameters.size());
	if (spacePos == std::string::npos || textToBeSent.empty()) {
		sendMessage(client, ERR_NOTEXTTOSEND(client.getNick()));
		return;
		}
	std::list<std::string>	targetList = splitString(params.parameters.substr(0, spacePos), ',');
	for (std::list<std::string>::iterator itTarget = targetList.begin(); itTarget != targetList.end(); ++itTarget) {
		if (isValidChannelName(*itTarget)) {
			std::list<Channel>::iterator itChannel = this->_channel_list.begin();
			while (itChannel != this->_channel_list.end()) {
				if (itChannel->getName() == *itTarget) {
					broadcastMessage(itChannel->getAllMember(), client, itChannel->getName(), "PRIVMSG", textToBeSent);
					break;
				}
				itChannel++;
			}
			if (itChannel == this->_channel_list.end()) {
				sendMessage(client, ERR_NOSUCHCHANNEL(*itTarget));
				return;
			}
		}
		else {
			std::vector<Client>::iterator itClient = this->_clientVector.begin();
			while(itClient != this->_clientVector.end()) {
				if(itClient->getNick() == *itTarget) {
					std::map<std::string, Client> target;
					target[itClient->getNick()] =  *itClient;
					broadcastMessage(target, client, "", "PRIVMSG", textToBeSent);
					break;
				}
				itClient++;
			}
			if (itClient == this->_clientVector.end()) {
				sendMessage(client, ERR_NOSUCHNICK(*itTarget));
				return;
			}
		}
	}
}

void	Server::invite(Client& client, t_ircMessage& params) {
	// too few parameters
	if (params.parametersList.size() < 2)
		return (sendMessage(client, ERR_NEEDMOREPARAMS(params.parameters)));
	std::list<std::string>::iterator itParams = params.parametersList.begin();
	std::list<Channel>::iterator itChannel = this->_channel_list.begin();
	std::vector<Client>::iterator itClient = this->_clientVector.begin();
	while (itClient != this->_clientVector.end()) {
		if (*itParams == itClient->getNick())
			break;
		itClient++;
	}
	// invited nickname does not exist
	if (itClient == this->_clientVector.end())
		return(sendMessage(client, ERR_NOSUCHNICK(*itParams)));
	itParams++;
	// channel name is not valid
	if(!isValidChannelName(*itParams))
		return (sendMessage(client, ERR_NOSUCHCHANNEL(*itParams)));
	while(itChannel != this->_channel_list.end()) {
		if (*itParams == itChannel->getName())
			break;
		itChannel++;
	}
	// channel does not exist
	if (itChannel == this->_channel_list.end())
		return (sendMessage(client, ERR_NOSUCHCHANNEL(*itParams)));
	// channel is invite only and issuer is not operator
	if (itChannel->getInvite() && !itChannel->isOperator(client))
		return (sendMessage(client, ERR_CHANOPRIVSNEEDED(client)));
	// channel is not invit0e only and issuer is not a channel opertator and is not a channel member
	else if(!itChannel->getInvite() && !itChannel->isOperator(client) && !itChannel->isMember(client.getNick()))
		return (sendMessage(client, ERR_NOTONCHANNEL(client.getNick())));
	// invited nickname is already on channel
	if (itChannel->isMember((*itClient).getNick()))
		return (sendMessage(*itClient, ERR_USERONCHANNEL((*itClient).getNick())));
	std::string	textToBeSent = " :" + itChannel->getName();
	sendMessage(*itClient, GENMESSAGE(client, inet_ntoa(client.getClientAddress().sin_addr), itClient->getNick(), "INVITE", textToBeSent));
	sendMessage(client, RPL_INVITING(client.getNick(), itClient->getNick(), itChannel->getName()));
}

//Debug function
void	Server::shutdown(Client &client, t_ircMessage &params)
{
	(void)params;
	(void)client;
	std::cout << MAGENTA << "i will shutdown" << WHITE << std::endl;
	std::vector<Client>::iterator it = this->_clientVector.begin();
	while (it != this->_clientVector.end()) {
		sendMessage(*it, "Server is shutting down. Bye\n");
		it++;
	}
	exit(0);
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

void	Server::mode(Client& client, t_ircMessage& params) {
	std::list<std::string>::iterator	parameter = params.parametersList.begin();
	std::string							channelname = params.parametersList.front();

	//check if a channel mode is given
	if (params.parametersList.size() < 2)
		return;
	//check if channel exists
	if (isValidChannelName(channelname) == false) {
		sendMessage(client, ERR_BADCHANMASK(channelname));
		return;
	}

	std::list<Channel>::iterator channel = _channel_list.begin();
	while (channel->getName() != channelname)
		if (channel++ == _channel_list.end()) {
			sendMessage(client, ERR_NOSUCHCHANNEL(channelname));
			return;
		}

	//check if client is channel operator
	if (channel->isOperator(client) == false) {
		sendMessage(client, ERR_CHANOPRIVSNEEDED(client));
		return;
	}

	std::string	mode = *++parameter;
	//MISSING: check if there is no sign
	bool		add = true;

	for (std::string::iterator s = mode.begin(); s != mode.end(); s++) {
		if (*s == '+')
			add = true;
		else if (*s == '-')
			add = false;
		else if (*s == 'o')
		//MISSING: what happens if mode doesn't exist?
			modeO(client, *channel, add, *++parameter);
		// further modes follow here tbd
	}
}

void	Server::modeO(Client& client, Channel& channel, bool add, std::string& target) {
	// search target client
	std::vector<Client>::iterator	it = _clientVector.begin();
	while (it->getNick() != target)
		if (it++ == _clientVector.end()) {
			sendMessage(client, ERR_NOSUCHNICK(target));
			return;
		}

	if (add == true)
		channel.setOperator(*it);
	else
		channel.removeOperator(*it);
}
