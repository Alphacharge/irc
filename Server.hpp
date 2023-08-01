/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 14:39:17 by lsordo            #+#    #+#             */
/*   Updated: 2023/07/31 19:46:12 by lsordo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Macros.hpp"

class	Server {
	private:
		int					_serverPort;
		std::string			_serverPassword;
		int					_serverSocket;
		int					_optval;
		int					_clientSocket;
		struct sockaddr_in	_serverAddress;
		struct sockaddr_in	_clientAddress;
		std::vector<pollfd>	_fds;
		pollfd				_serverPollfd;
		pollfd				_clientPollfd;
		Server(void);

	public:
		Server(int, std::string);
		Server(Server const&);
		~Server(void);
		Server&	operator=(Server const&);

		class SocketException : public std::exception {
			public:
				virtual char const* what() const throw();
		};
		class BindException : public std::exception {
			public:
				virtual char const* what() const throw();
		};
		class ListenException : public std::exception {
			public:
				virtual char const* what() const throw();
		};
		class AcceptException : public std::exception {
			public:
				virtual char const* what() const throw();
		};
		class PollException : public std::exception {
			public:
				virtual char const* what() const throw();
		};

		void		serverSetup(void);
};
