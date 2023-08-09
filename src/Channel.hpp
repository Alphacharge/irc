/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 08:37:46 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/09 06:57:10 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <iostream>
# include <sstream>
# include <map>
# include <set>
# include "Colors.hpp"
# include "Client.hpp"
# include "ChannelModes.hpp"

class Client;

class Channel {
	private:
	Channel(void);
	/*---------------	Variables		---------------*/
	std::string						_name; //begin with & or #, max 200 chars, not contain any spaces (' '), a control G (^G or ASCII 7), or a comma (',' which is used as a list item)
	std::string						_topic;
	std::string						_password;
	bool							_inviteonly;
	bool							_restrictTopic;
	int								_limit;
	std::set<t_chmode>				_mode;
	std::set<Client*>				_banns;
	std::map<std::string, Client>	_users;
	std::map<std::string, Client>	_operators;
	std::map<std::string, Client>	_inviteList;

	public:
	/*---------------	Exceptions		---------------*/
	/*---------------	Constructors	---------------*/
	Channel(std::string &name);
	Channel(Channel const& other);

	/*---------------	Destructor		---------------*/
	~Channel(void);

	/*---------------	Operators		---------------*/
	Channel&	operator=(Channel const& other);

	/*---------------	Methods			---------------*/
	void							print(void);

	std::string						getName(void) const;
	std::string						getPassword(void) const;
	int								getLimit(void);
	std::map<std::string, Client>	getInviteList(void);
	int								getAmountOfAll(void);
	
	void							setPassword(std::string pw);
	void							setLimit(int);
	void							setLimit(std::string&);
	void							setInviteOnly(bool);
	void							removeInvite(Client& client);
	void							setRestrictTopic(bool);
	void							setInviteList(Client& client);
	
	std::map<std::string, Client>	getOperators(void);
	void							setOperator(Client &client);
	void							removeOperator(Client& client);
	void							setOperatorStatus(Client &client);
	void							removeOperatorStatus(Client& client);

	std::map<std::string, Client>	getUsers(void);
	void							setUser(Client &client);
	void							removeUser(Client& client);

	bool							isOperator(Client& client);
	bool							isInviteOnly(void) const;
	bool							isUser(Client& client);
	bool							isMember(std::string& nick);

	std::map<std::string, Client>	getAllMember(void);
	std::string						genUserlist(void);

	void							bann(Client &client);
	bool							isBanned(Client &client);
};
#endif
