/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 08:37:46 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/11 17:59:46 by lsordo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef MESSAGES_HPP
# define MESSAGES_HPP

# define RPL_WELCOME(nick)												(std::string(":irc42 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!\n"))
# define RPL_NOTOPIC(nickName, channelName)								(std::string(":irc42 331 " + nickName + " " + channelName + " :No topic is set"))
# define RPL_TOPIC(nickName, channelName, topic)						(std::string(":irc42 332 " + nickName + " " + channelName + " " + topic))
# define RPL_TOPICWHOTIME(nickName, channelName, setbyNickname, setat)	(std::string(":irc42 333 " + nickName + " " + channelName + " " + setbyNickname + " " + setat))
# define RPL_INVITING(issuerName, guestName, channelName)				(std::string(":irc42 341 " + issuerName + " " + guestName + " " + channelName))
# define RPL_CAP														(std::string("CAP * LS :\n"))

# define ERR_NOSUCHNICK(nick, target)					(std::string (":irc42 401 " + nick + " " + target + " :No such nick/channel"))
# define ERR_NOSUCHCHANNEL(nick, channel)				(std::string(":irc42 403 " + nick+ " " + channel + " :No such channel"))
# define ERR_CANNOTSENDTOCHAN(nick, channel)			(std::string(":irc42 404  " + nick + " " + channel + " :Cannot send to channel"))
# define ERR_NOTEXTTOSEND(nick)							(std::string(":irc42 412 " + nick + " :No text to send"))
# define ERR_NONICKNAMEGIVEN(nick)						(std::string(":irc42 431 " + nick + " :No nickname given"))
# define ERR_NICKNAMEINUSE(nick)						(std::string(":irc42 433 " + nick + " :Nickname is already in use\n"))
# define ERR_USERNOTINCHANNEL(nick, target, channel)	(std::string("irc42 441 " + nick + " " + target + " "  + channel + " :They aren't on that channel"))
# define ERR_NOTONCHANNEL(nick, channel)				(std::string("irc42 442 " + nick + " " + channel + " :You're not on that channel"))
# define ERR_USERONCHANNEL(nick, channel)				(std::string("irc42 443 " + nick + " " + channel +  " :is already on channel"))
# define ERR_NOTREGISTERED(nick)						(std::string(":irc42 451 " + nick + " :You have not registered"))
# define ERR_NEEDMOREPARAMS(nick, cmd)					(std::string(":irc42 461 " + nick + " " + cmd + " :Not enough parameters"))
# define ERR_ALREADYREGISTERED(nick)					(std::string(":irc42 462 " + nick + " :Already registered"))
# define ERR_PASSWDMISMATCH(nick)						(std::string(":irc42 464 " + nick + " :Password required"))
# define ERR_KEYSET(nick, channel)						(std::string(":irc42 467 " + nick + " " + channel + " :Channel key already set"))
# define ERR_CHANNELISFULL(nick, channel)				(std::string(":irc42 471 " + nick + " " + channel + " :Cannot join channel (+l)"))
# define ERR_UNKNOWNMODE(nick, mode)					(std::string(":irc42 472 " + nick + " " + mode + " :is unknown mode char to me"))
# define ERR_INVITEONLYCHAN(nick, channel)				(std::string(":irc42 473 " + nick + " " + channel + " :Cannot join channel (+i)"))
# define ERR_BANNEDFROMCHAN(nick, channel)				(std::string(":irc42 474 " + nick + " " + channel + " :Cannot join channel (+b)"))
# define ERR_BADCHANNELKEY(nick, channel)				(std::string(":irc42 475 " + nick + " " + channel + " :Cannot join channel (+k)"))
# define ERR_BADCHANMASK(nick, channel)					(std::string(":irc42 476 " + nick + " " + channel + " :Bad Channel Mask"))
# define ERR_CHANOPRIVSNEEDED(nick, channel)			(std::string(":irc42 482 " + nick + " " + channel + " :You're not channel operator"))

# define ERROR(message)	(std::string("ERROR :Closing Link: " + message))
# define PONG(params)	(std::string(":irc42 PONG irc42 " + params))

# define GENUSER(client, address)	(std::string(client.getNick() + "!~" + client.getUsername() + "@" + address))

# define GENMESSAGE(client, address, target, type, textToBeSent)	(std::string(":" + client.getNick() + "!~" + client.getUsername() + "@" + address + " " + type + " " + target + " " + textToBeSent + "\n"))
# define GENUSERMESSAGE(client, address, type, textToBeSent)		(std::string(":" + client.getNick() + "!~" + client.getUsername() + "@" + address + " " + type + " " + textToBeSent + "\n"))

# define USERLIST(host, client, channel, clients)	(std::string(std::string(":") + host + " 353 " + client.getNick() + " = " + channel + " :" + clients + "\n"))
# define USERLISTEND(host, client, channel)			(std::string(std::string(":") + host + " 366 " + client.getNick() + " " + channel + " :End of /NAMES list.\n"))

#endif
