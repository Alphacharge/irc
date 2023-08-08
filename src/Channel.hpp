/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 08:37:46 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/08 15:45:45 by rbetz            ###   ########.fr       */
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

	std::string	getName(void) const;
	std::string	getPassword(void) const;
	void		setPassword(std::string pw);
	bool		isInviteOnly(void) const;
	void		setInviteOnly(bool);
	void		setRestrictTopic(bool);
	void		setLimit(int);
	void		setLimit(std::string&);
	int			getLimit(void);

	void							setOperator(Client &client);
	std::map<std::string, Client>	getOperators(void);
	void							removeOperator(Client& client);

	void							setOperatorStatus(Client &client);
	void							removeOperatorStatus(Client& client);

	bool							isOperator(Client& client);

	void							setUser(Client &client);
	std::map<std::string, Client>	getUsers(void);
	
	void							setInviteList(Client& client);
	std::map<std::string, Client>	getInviteList(void);

	bool							isUser(Client& client);
	void							removeUser(Client& client);

	std::map<std::string, Client>	getAllMember(void);
	std::string						genUserlist(void);

	bool	isMember(std::string& nick);
	void	setUser(Client &client);
	int								getAmountOfAll(void);
	void							bann(Client &client);
	bool							isBanned(Client &client);
};
#endif
