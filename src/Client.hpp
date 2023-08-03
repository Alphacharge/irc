/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 09:38:50 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/03 15:56:39 by lsordo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <iostream>
# include <netinet/in.h>
# include <poll.h>
# include "Macros.hpp"
# include "Colors.hpp"

class Client {
	private:
		int					_clientSocket;
		struct sockaddr_in	_clientAddress;
		pollfd				_clientPollfd;
		std::string			_nick;
		std::string			_username;
		int					_status;	//string? int? makro?
		std::string			_buffer;

	public:
		Client(void);
		Client(Client const&);
		~Client(void);
		Client&	operator=(Client const&);

		void	setClientSocket(int const&);
		void	setClientAddress(struct sockaddr_in const&);
		void	setClientPollfd_fd(int const&);
		void	setClientPollfd_events(short const&);
		void	setNick(std::string);
		void	setUsername(std::string);
		void	setStatus(int);
		void	appendBuffer(std::string const&);

		int&				getClientSocket(void);
		struct sockaddr_in&	getClientAddress(void);
		pollfd&				getClientPollfd(void);

		std::string&		getNick(void);
		std::string&		getUsername(void);
		int					getStatus(void);
		std::string&		getBuffer(void);
};
