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

/* === COMMANDS === */

void	Server::join(Client &client, t_ircMessage& params) {
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

void	Server::cap(Client &client, t_ircMessage &params) {
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

void	Server::pong(Client &client, t_ircMessage &params) {
	sendMessage(client, PONG(params.parameters));
}

void	Server::pass(Client &client, t_ircMessage &params) {
	if (client.getStatus() >= AUTHENTICATED)
		return;
	if (params.parameters.empty())
		sendMessage(client, ERR_NEEDMOREPARAMS(params.parameters));
	else if (client.getStatus() >= AUTHENTICATED)
		sendMessage(client, ERR_ALREADYREGISTERED);
	else if (this->_serverPassword == params.parameters)
	{
		client.setStatus(AUTHENTICATED);
		if (VERBOSE >= 3)
			std::cout << ORANGE << "DEBUG: Password accepted.\n" << WHITE;
	}
	else
		sendMessage(client, ERR_PASSWDMISMATCH);
}

void	Server::nick(Client &client, t_ircMessage &params) {
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
	for (std::vector<Client>::iterator it = this->_clientVector.begin(); it != this->_clientVector.end(); it++) {
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

void	Server::quit(Client &client, t_ircMessage &params) {
	(void)params;
	sendMessage(client, ERROR((std::string) "Connection closed."));
	client.setStatus(DISCONNECTED);
	//broadcast to all relevant channels
	for (std::list<Channel>::iterator it = _channel_list.begin(); it != _channel_list.end(); it++) {
		if (it->isMember(client.getNick()))
			broadcastMessage(it->getAllMember(), ":" + client.getNick() + "@irc42" + params.parameters + "; User left the irc network.");
	}
}

void	Server::shutdown(Client &client, t_ircMessage &params) {
	(void)params;
	(void)client;
	std::cout << PURPLE << "i will shutdown" << WHITE << std::endl;
	std::vector<Client>::iterator it = this->_clientVector.begin();
	while (it != this->_clientVector.end()) {
		sendMessage(*it, "Server is shutting down. Bye\n");
		it++;
	}
	exit(0);
}

void	Server::privmsg(Client &client, t_ircMessage &params) {
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
		return;}
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
			}
		} else {
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
			if (itClient == this->_clientVector.end())
				sendMessage(client, ERR_NOSUCHNICK(*itTarget));
		}
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
	std::list<Channel>::iterator channel = this->_channel_list.begin();
	while (channel->getName() != channelname)
		if (channel++ == this->_channel_list.end()) {
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
	// std::vector<Client>::iterator	it = this->_clientVector.begin();
	// while (it->getNick() != target)
	// 	if (it++ == this->_clientVector.end()) {
	// 		sendMessage(client, ERR_NOSUCHNICK(target));
	// 		return;
	// 	}
	std::vector<Client>::iterator	it = getClient(target);
	if (it == this->_clientVector.end()) {
			sendMessage(client, ERR_NOSUCHNICK(target));
			return;
	}

	if (add == true)
		channel.setOperator(*it);
	else
		channel.removeOperatorStatus(*it);
}

void	Server::kick(Client &client, t_ircMessage& params) {
	if (client.getStatus() < REGISTERED) {
		sendMessage(client, ERR_NOTREGISTERED);
		return;
	}
	if (params.parameters.size() < 2) {
		sendMessage(client, ERR_NEEDMOREPARAMS(params.command));
		return ;
	}
	std::list<std::string> to_kick_from = splitString(params.parametersList.front(), ',');
	std::list<std::string> to_kick_users = splitString(params.parametersList.back(), ',');
	std::list<std::string>::iterator it_to_kick_from = to_kick_from.begin();
	std::list<std::string>::iterator it_to_kick_users = to_kick_users.begin();
	while (it_to_kick_from != to_kick_from.end()) {
		//--->We found on official Server no Character that triggers this error
		if (!isValidChannelName(*it_to_kick_from)) {
			sendMessage(client, ERR_BADCHANMASK(*it_to_kick_from));
			return ;
		}
		std::list<Channel>::iterator it_chan = this->_channel_list.begin();
		while (it_chan != this->_channel_list.end()) {
			if (VERBOSE >= 3)
				std::cout << CYAN << client.getNick() << " tries to join " << *it_to_kick_from << ". Testing: " << it_chan->getName() << WHITE << std::endl;
			//client is no Member of the Channel
			if (it_chan->getName() == *it_to_kick_from && it_chan->isMember(client.getNick())) {
				sendMessage(client, ERR_NOTONCHANNEL(it_chan->getName()));
				return ;
			}
			//Kicking Member is no Operator
			if (it_chan->getName() == *it_to_kick_from && it_chan->isOperator(client)) {
				sendMessage(client, ERR_CHANOPRIVSNEEDED(client));
				return ;
			}
			//To be kicked User is no Member of the Channel
			if (it_chan->getName() == *it_to_kick_from && it_chan->isMember(*it_to_kick_users)) {
				sendMessage(client, ERR_USERNOTINCHANNEL(*it_to_kick_users, it_chan));
				return ;
			}
			if (*it_to_kick_from != it_chan->getName())
				it_chan++;
			else
				break;
		}
		std::string	textToBeSent;
		//extract message
		if (params.parameters.size() > 2) {
			size_t	spacePos = params.parameters.find(" ", params.parameters.find(" ") + 1);
			if (spacePos != std::string::npos)
				textToBeSent = params.parameters.substr(spacePos + 1, params.parameters.size());
		}
		if (it_chan == this->_channel_list.end()) {
			sendMessage(client, ERR_NOSUCHCHANNEL(*it_to_kick_from));
			return ;
		} else {
			std::vector<Client>::iterator it_client = getClient(*it_to_kick_users);
			if (it_chan->isOperator(*it_client))
				it_chan->removeOperator(*it_client);
			if (it_chan->isUser(*it_client))
				it_chan->removeUser(*it_client);
			// sendMessage(client, USERLIST(inet_ntoa(this->_serverAddress.sin_addr), client, it_chan->getName(), it_chan->genUserlist()));
			// broadcastMessage(it_chan->getAllMember(), client, it_chan->getName(), "JOIN", "");
		}
		it_to_kick_from++;
		it_to_kick_users++;
	}
	printAllChannels();
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
