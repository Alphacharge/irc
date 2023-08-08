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

void	Server::sendMessage(Client &client, std::string message) {
	size_t bytesSent = 0;

	if (!message.empty() && message[message.size() - 1] != '\n')
		message += '\n';
	// while (bytesSent < message.length())
	// We should found the reason why we should need it
	// ==62812== Process terminating with default action of signal 13 (SIGPIPE)
	// ==62812==    at 0x4B9B859: send (send.c:28)
	// ==62812==    by 0x10D83C: Server::sendMessage(Client&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (Server.cpp:287)
	// ==62812==    by 0x10EC21: Server::join(Client&, s_irc&) (Server.cpp:379)
	// ==62812==    by 0x10D4F2: Server::serverStart() (Server.cpp:256)
	// ==62812==    by 0x10B707: main (ircserv.cpp:24)
		bytesSent += send(client.getClientPollfd().fd, message.c_str(), message.length(), 0);
}

void	Server::broadcastMessage(std::map<std::string, Client> map, Client& client, std::string channelName, std::string type, std::string textToBeSent) {
	std::map<std::string, Client>::iterator it = map.begin();
	while (it != map.end()) {
		if(it->second.getNick() != client.getNick() || type != "PRIVMSG")
			sendMessage(it->second, GENMESSAGE(client, inet_ntoa(client.getClientAddress().sin_addr), channelName, type, textToBeSent));
		it++;
	}
}

void	Server::broadcastMessage(std::map<std::string, Client> map, std::string message) {
	for (std::map<std::string, Client>::iterator it = map.begin(); it != map.end(); it++)
			sendMessage(it->second, message);
}

//Debug function
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
