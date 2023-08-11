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
		sendMessage(client, ERR_NOTREGISTERED(client.getNick()));
		return;
	}
	if (params.parameters.empty()) {
		sendMessage(client, ERR_NEEDMOREPARAMS(client.getNick() + " " + params.command));
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
			std::map<std::string, Client> invites = it_chan->getInviteList();
			std::map<std::string, Client>::iterator clientinvited = invites.find(client.getNick());
			if (it_chan->getName() == *it_join && it_chan->isMember(client.getNick()))
				return ;
			if (it_chan->getName() == *it_join && it_chan->isInviteOnly() == true && clientinvited == invites.end()) {
				sendMessage(client, ERR_INVITEONLYCHAN(*it_join));
				return ;
			}
			//Channellimit reached
			if (it_chan->getName() == *it_join && it_chan->getLimit() > 0 && it_chan->getAmountOfAll() >= it_chan->getLimit()) {
				sendMessage(client, ERR_CHANNELISFULL(*it_join));
				return ;
			}
			//possible problem if no password exist in the command, also valid iterator of password is not secured
			if (it_chan->getName() == *it_join && ((it_joinpw == tojoinpw.end() && it_chan->getPassword() != "") || (it_joinpw != tojoinpw.end() && it_chan->getPassword() != *it_joinpw))) {
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
			if (it_joinpw != tojoinpw.end())
				newCH.setPassword(*it_joinpw);
			this->_channel_list.push_back(newCH);
			broadcastMessage(newCH.getOperators(), client, newCH.getName(), "JOIN", "");
			sendMessage(client, USERLIST(inet_ntoa(this->_serverAddress.sin_addr), client, *it_join, newCH.genUserlist()));
			sendMessage(client, USERLISTEND(inet_ntoa(this->_serverAddress.sin_addr), client, *it_join));
		} else {
			it_chan->setUser(client);
			// if ( it_chan->isInviteOnly())
				it_chan->removeInvite(client);
			broadcastMessage(it_chan->getAllMember(), client, it_chan->getName(), "JOIN", "");
			if (it_chan->getTopic().empty())
				sendMessage(client, RPL_NOTOPIC(client.getNick(), it_chan->getName()));
			else
				sendMessage(client, RPL_TOPIC(client.getNick(), it_chan->getName(), it_chan->getTopic()));
			sendMessage(client, USERLIST(inet_ntoa(this->_serverAddress.sin_addr), client, it_chan->getName(), it_chan->genUserlist()));
			sendMessage(client, USERLISTEND(inet_ntoa(this->_serverAddress.sin_addr), client, it_chan->getName()));
		}
		it_join++;
		it_joinpw++;
	}
	if (VERBOSE >= 2)
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
		sendMessage(client, ERR_ALREADYREGISTERED(client.getNick()));
	else if (params.parameters.empty())
		sendMessage(client, ERR_NEEDMOREPARAMS(client.getNick() + " " + params.command));
	else if (this->_serverPassword == params.parameters)
	{
		client.setStatus(AUTHENTICATED);
		if (VERBOSE >= 2)
			std::cout << ORANGE << "DEBUG: Password accepted.\n" << WHITE;
	}
	else
		sendMessage(client, ERR_PASSWDMISMATCH);
}

void	Server::nick(Client &client, t_ircMessage &params) {
	// check if a password was supplied
	if (client.getStatus() < AUTHENTICATED) {
		sendMessage(client, ERR_NOTREGISTERED(client.getNick()));
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

	if (VERBOSE >= 2)
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
	if (client.getStatus() < AUTHENTICATED)
		return sendMessage(client, ERR_NOTREGISTERED(client.getNick()));
	// set username
	if (client.getStatus() >= REGISTERED)
		sendMessage(client, ERR_ALREADYREGISTERED(client.getNick()));
	else if (params.parametersList.size() == 0)
		client.setUsername("unknown");
	else
		client.setUsername(params.parametersList.front());
	if (VERBOSE >= 2)
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
	sendMessage(client, ERROR(params.parameters));
	client.setStatus(DISCONNECTED);
}

void	Server::shutdown(Client &client, t_ircMessage &params) {
	(void)params;
	(void)client;
	if (VERBOSE >= 1)
		std::cout << PURPLE << "SERVER SHUTTING DOWN in 5s..." << WHITE << std::endl;
	std::vector<Client>::iterator it = this->_clientVector.begin();
	while (it != this->_clientVector.end()) {
		sendMessage(*it, "Server is shutting down. Bye\n");
		it++;
	}
	for (short i = 4; i > 0; --i) {
		sleep(1);
		if (VERBOSE >= 1)
			std::cout << PURPLE << i << "s..." << WHITE << std::endl;
	}
	this->_run = false;
}

void	Server::privmsg(Client &client, t_ircMessage &params) {
	if (client.getStatus() < REGISTERED) {
		sendMessage(client, ERR_NOTREGISTERED(client.getNick()));
		return;
	}
	std::string	textToBeSent;
	size_t	spacePos = params.parameters.find(" ");
	// no text to be sent
	if (spacePos != std::string::npos)
		textToBeSent = params.parameters.substr(spacePos + 1, params.parameters.size());
	if (spacePos == std::string::npos || textToBeSent.empty()) {return(sendMessage(client, ERR_NOTEXTTOSEND(client.getNick())));}
	std::list<std::string>	targetList = splitString(params.parameters.substr(0, spacePos), ',');
	for (std::list<std::string>::iterator itTarget = targetList.begin(); itTarget != targetList.end(); ++itTarget) {
		// no valid channel name
		if (isValidChannelName(*itTarget)) {
			std::list<Channel>::iterator itChannel = this->_channel_list.begin();
			while (itChannel != this->_channel_list.end()) {
				if (itChannel->getName() == *itTarget) {
					if (!itChannel->isMember(client.getNick())) { return(sendMessage(client, ERR_CANNOTSENDTOCHAN(client.getNick(),itChannel->getName())));}
					broadcastMessage(itChannel->getAllMember(), client, itChannel->getName(), "PRIVMSG", textToBeSent);
					break;
				}
				itChannel++;
			}
			// no existing channel name
			if (itChannel == this->_channel_list.end()) {return (sendMessage(client, ERR_NOSUCHCHANNEL(*itTarget)));}
		}
		else {
			// send message to user
			std::vector<Client>::iterator itClient = this->_clientVector.begin();
			while(itClient != this->_clientVector.end()) {
				if(itClient->getNick() == *itTarget) {
					std::map<std::string, Client> target;
					target[itClient->getNick()] =  *itClient;
					broadcastMessage(target, client, "PRIVMSG", textToBeSent);
					break;
				}
				itClient++;
			}
			// nickname not found
			if (itClient == this->_clientVector.end()) {return(sendMessage(client, ERR_NOSUCHNICK(*itTarget)));}
		}
	}
}

void	Server::mode(Client& client, t_ircMessage& params) {
	if (client.getStatus() < REGISTERED)
		return sendMessage(client, ERR_NOTREGISTERED(client.getNick()));

	//check if a channel mode is given
	if (params.parametersVector.size() < 1)
		return sendMessage(client, ERR_NEEDMOREPARAMS(client.getNick() + " " + params.command));
	else if (params.parametersVector.size() == 1)
	//MISSING: add MODE #channel command here if you like
		return;

	std::string	channelname = params.parametersVector[0];
	//search channel
	if (isValidChannelName(channelname) == false)
		return sendMessage(client, ERR_BADCHANMASK(client.getNick() + " " + channelname));

	std::list<Channel>::iterator channel = this->_channel_list.begin();
	while (channel->getName() != channelname)
		if (++channel == this->_channel_list.end())
			return sendMessage(client, ERR_NOSUCHCHANNEL(client.getNick() + " " + channelname));

	//check if client is channel operator
	if (channel->isOperator(client) == false)
		return sendMessage(client, ERR_CHANOPRIVSNEEDED(client));

	//check number of parameters
	if (enoughModeParameters(params) == false)
		return sendMessage(client, ERR_NEEDMOREPARAMS(client.getNick() + " " + params.command));

	if (VERBOSE >= 2)
		channel->print();

	std::string	mode = params.parametersVector[1];
	bool		add = true;
	size_t		parameter = 2;
	//MISSING: check if there is no sign

	for (std::string::iterator s = mode.begin(); s != mode.end(); s++) {
		switch (*s) {
			case '+':
				add = true;
				break;
			case '-':
				add = false;
				break;
			case 'i':
				channel->setInviteOnly(add);
				break;
			case 't':
				channel->setRestrictTopic(add);
				break;
			case 'k':
				modeK(client, *channel, add, params.parametersVector[parameter++]);
				break;
			case 'o':
				modeO(client, *channel, add, params.parametersVector[parameter++]);
				break;
			case 'l':
				if (add == true)
					channel->setLimit(params.parametersVector[parameter++]);
				else
					channel->removeLimit();
				break;
			default:
				sendMessage(client, ERR_UNKNOWNMODE(client.getNick() + " " + mode, channel->getName()));
				return;
		}
	}
}

void	Server::modeO(Client& client, Channel& channel, bool add, std::string& target) {
	std::vector<Client>::iterator	it = getClient(target);
	if (it == this->_clientVector.end()) {
			sendMessage(client, ERR_NOSUCHNICK(client.getNick() + " " + target));
			return;
	}

	if (add == true)
		channel.setOperatorStatus(*it);
	else
		channel.removeOperatorStatus(*it);
}

void	Server::modeK(Client& client, Channel& channel, bool add, std::string& password) {
	if (add == true) {
		if (channel.getPassword() != "")
			sendMessage(client, ERR_KEYSET);
		else
			channel.setPassword(password);
	} else {
		if (channel.getPassword() != password)
			sendMessage(client, ERR_PASSWDMISMATCH);
		else
			channel.setPassword("");
	}
}

bool	Server::enoughModeParameters(t_ircMessage& params) {
	size_t		parameter = 2;
	std::string	mode = params.parametersVector[1];
	bool		add = true;

	for (std::string::iterator	c = mode.begin(); c != mode.end(); c++) {
		if (VERBOSE >= 2)
			if (parameter < params.parametersVector.size())
				std::cout << "DEBUG: add " << add << " mode " << *c << " parameter " << params.parametersVector[parameter] << std::endl;
		switch (*c)
		{
			case '+':
				add = true;
				break;
			case '-':
				add = false;
				break;
			case 'l':
				if (add == false)
					continue;
				//fallthrough
			case 'k':
			case 'o':
				if (parameter >= params.parametersVector.size())
					return (false);
				parameter++;
				break;
			default:
				break;
		}
	}
	return (true);
}

void	Server::kick(Client &client, t_ircMessage& params) {
	if (client.getStatus() < REGISTERED) {
		sendMessage(client, ERR_NOTREGISTERED(client.getNick()));
		return;
	}
	if (params.parameters.size() < 2) {
		sendMessage(client, ERR_NEEDMOREPARAMS(client.getNick() + " " + params.command));
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
			//client is no Member of the Channel
			if (it_chan->getName() == *it_to_kick_from && !it_chan->isMember(client.getNick())) {
				sendMessage(client, ERR_NOTONCHANNEL(it_chan->getName()));
				return ;
			}
			//Kicking Member is no Operator
			if (it_chan->getName() == *it_to_kick_from && !it_chan->isOperator(client)) {
				sendMessage(client, ERR_CHANOPRIVSNEEDED(client));
				return ;
			}
			//To be kicked User is no Member of the Channel
			if (it_chan->getName() == *it_to_kick_from && !it_chan->isMember(*it_to_kick_users)) {
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
			broadcastMessage(it_chan->getAllMember(), client, it_chan->getName(), "KICK", textToBeSent);
			if (it_chan->isOperator(*it_client))
				it_chan->removeOperator(*it_client);
			if (it_chan->isUser(*it_client))
				it_chan->removeUser(*it_client);
		}
		it_to_kick_from++;
		it_to_kick_users++;
	}
	if (VERBOSE >= 2)
		printAllChannels();
}

void	Server::invite(Client& client, t_ircMessage& params) {
	if (client.getStatus() < REGISTERED) {
		sendMessage(client, ERR_NOTREGISTERED(client.getNick()));
		return;
	}
	// too few parameters
	if (params.parametersList.size() < 2)
		return (sendMessage(client, ERR_NEEDMOREPARAMS(client.getNick() + " " + params.command)));

	// search invited user
	std::list<Channel>::iterator		itChannel = this->_channel_list.begin();
	std::vector<Client>::iterator		itClient = _clientVector.begin();
	std::string							channelName = params.parametersVector[1];
	std::string							clientName = params.parametersVector[0];
	for (; itClient != _clientVector.end(); itClient++) {
		if (itClient->getNick() == clientName) {
			break;
		}
	}
	// invited nickname does not exist
	if (itClient == this->_clientVector.end())
		return(sendMessage(client, ERR_NOSUCHNICK(clientName)));

	// channel name is not valid
	if(!isValidChannelName(channelName))
		return (sendMessage(client, ERR_NOSUCHCHANNEL(channelName)));
	//search channel
	for (; itChannel != _channel_list.end(); itChannel++)
		if (itChannel->getName() == channelName)
			break;
	// channel does not exist
	if (itChannel == _channel_list.end())
		return (sendMessage(client, ERR_NOSUCHCHANNEL(channelName)));

	// channel is invite only and issuer is not operator
	if (itChannel->isInviteOnly() && !itChannel->isOperator(client))
		return (sendMessage(client, ERR_CHANOPRIVSNEEDED(client)));
	// channel is not invite only and issuer is not a channel opertator and is not a channel member
	else if(!itChannel->isInviteOnly() && !itChannel->isMember(client.getNick()))
		return (sendMessage(client, ERR_NOTONCHANNEL(client.getNick())));
	// invited nickname is already on channel
	if (itChannel->isMember(clientName))
		return (sendMessage(client, ERR_USERONCHANNEL(clientName)));

	itChannel->setInviteList(*itClient);

	std::string	textToBeSent = ":" + channelName;
	sendMessage(*itClient, GENMESSAGE(client, inet_ntoa(client.getClientAddress().sin_addr), itClient->getNick(), "INVITE", textToBeSent));
	sendMessage(client, RPL_INVITING(client.getNick(), itClient->getNick(), itChannel->getName()));
}

void	Server::topic(Client& client, t_ircMessage& params) {
	if (client.getStatus() < REGISTERED)
		return sendMessage(client, ERR_NOTREGISTERED(client.getNick()));

	// need more parameters
	if (params.parametersList.empty()) {return (sendMessage(client, ERR_NEEDMOREPARAMS(client.getNick() + " " + params.command)));}
	std::list<std::string>::iterator itParams = params.parametersList.begin();
	// no such channel
	std::list<Channel>::iterator itChannel = this->getChannel(*itParams);
	if (!isValidChannelName(*itParams) || itChannel == this->_channel_list.end())
		return(sendMessage(client, ERR_NOSUCHCHANNEL(*itParams)));
	//not on channel
	if (!itChannel->isMember(client.getNick()))
		return (sendMessage(client, ERR_NOTONCHANNEL(client.getNick())));
	// no operator with protected channel
	if (itChannel->isTopicRestricted() && !itChannel->isOperator(client) && params.parametersList.size() > 1)
		return (sendMessage(client, ERR_CHANOPRIVSNEEDED(client)));
	// no topic argument passed
	if (params.parametersList.size() == 1) {
		// no previously set topic to return
		if(itChannel->getTopic().empty()) {return(sendMessage(client, RPL_NOTOPIC(client.getNick(), itChannel->getName())));}
		// return previously set topic
		else {
			sendMessage(client, RPL_TOPIC(client.getNick(), itChannel->getName(), itChannel->getTopic()));
			sendMessage(client, RPL_TOPICWHOTIME(client.getNick(), itChannel->getName(), itChannel->getTopicSetby(), itChannel->getTopicSetat()));
		}
	}
	// clear topic
	else if (params.parametersList.size() >= 2 && *(++itParams) == ":") {
		itChannel->clearTopic();
		std::map<std::string, Client> members;
		std::map<std::string, Client>::iterator it = members.begin();
		for (; it != members.end(); ++it) {
			sendMessage(it->second, RPL_TOPIC(it->first, itChannel->getName(), itChannel->getTopic()));
		}
	}
	// set new topic
	else if (params.parametersList.size() >=2) {
		size_t pos = 0;
		std::string	topic;
		pos = params.parameters.find(' ', pos);
		if (pos != std::string::npos)
			topic = params.parameters.substr(pos + 1, params.parameters.size());
		itChannel->setTopic(topic);
		itChannel->setTopicSetat();
		itChannel->setTopicSetby(client.getNick());
		std::map<std::string, Client> members = itChannel->getAllMember();
		std::map<std::string, Client>::iterator it = members.begin();
		for (; it != members.end(); ++it) {
			sendMessage(it->second, RPL_TOPIC(it->first, itChannel->getName(), itChannel->getTopic()));
			sendMessage(it->second, RPL_TOPICWHOTIME(it->first, itChannel->getName(), itChannel->getTopicSetby(), itChannel->getTopicSetat()));
		}
	}
}
