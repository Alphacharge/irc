/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   myIrcParser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 10:33:17 by lsordo            #+#    #+#             */
/*   Updated: 2023/07/31 10:38:38 by lsordo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

bool parseIRCMessage(const std::string& message, std::string& prefix, std::string& command, std::string& params) {
	size_t pos = 0;

	// Check for the optional prefix (starts with ':')
	if (message[pos] == ':') {
		pos++;
		size_t prefixEnd = message.find(' ', pos);
		if (prefixEnd == std::string::npos) {
			// Invalid message, missing command and params
			return false;
		}
		prefix = message.substr(pos, prefixEnd - pos);
		pos = prefixEnd + 1;
	}

	// Find the command (ends with ' ')
	size_t commandEnd = message.find(' ', pos);
	if (commandEnd == std::string::npos) {
		// Invalid message, missing params
		return false;
	}
	command = message.substr(pos, commandEnd - pos);
	pos = commandEnd + 1;

	// Get the params
	params = message.substr(pos);

	return true;
}

int main() {
	std::string message = ":nick!user@host PRIVMSG #channel :Hello, IRC!";
	std::string prefix, command, params;

	if (parseIRCMessage(message, prefix, command, params)) {
		std::cout << "Prefix: " << prefix << std::endl;
		std::cout << "Command: " << command << std::endl;
		std::cout << "Params: " << params << std::endl;
	} else {
		std::cout << "Invalid IRC message." << std::endl;
	}

	return 0;
}
