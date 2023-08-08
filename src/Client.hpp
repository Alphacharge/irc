/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 09:38:50 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/08 06:17:04 by rbetz            ###   ########.fr       */
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
		int					_status;
		std::string			_buffer;

	public:
		Client(void);
		Client(Client const& src);
		~Client(void);
		Client&	operator=(Client const& rhs);

		void	setClientSocket(int const& socket);
		void	setClientAddress(struct sockaddr_in const& address);
		void	setClientPollfd_fd(int const&);
		void	setClientPollfd_events(short const&);
		void	setNick(std::string nick);
		void	setUsername(std::string username);
		void	setStatus(int status);
		void	appendBuffer(std::string const& str);
		void	print(void);

		int&				getClientSocket(void);
		struct sockaddr_in&	getClientAddress(void);
		pollfd&				getClientPollfd(void);

		std::string&		getNick(void);
		std::string&		getUsername(void);
		int					getStatus(void);
		std::string&		getBuffer(void);
};
