/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 14:39:17 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/02 15:45:54 by lsordo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <vector>
# include <cstring>
# include <poll.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <unistd.h>
# include <string>
# include <string.h>

# include "Client.hpp"
# include "Macros.hpp"

# define RPL_CAP				(std::string("CAP * LS :\n"))
# define PONG(params)			(std::string(":irc42 PONG irc42 " + params + "\n"))
# define RPL_WELCOME(params)	(std::string(":irc42 001 " + params + " :Welcome to the Internet Relay Network!\n"))

# define ERR_NONICKNAMEGIVEN		(std::string(":irc42 431 :No nickname given!\n"))
// # define ERR_NICKNAMEINUSE(params)	(std::string(":irc42 433 " + params + " :Nickname is already in use\n"))
// # define ERR_NEEDMOREPARAMS(params)	(std::string(":irc42 461 " + params + " :Not enough parameters\n"))
# define ERR_ALREADYREGISTERED		(std::string(":irc42 462 :Already registered\n"))

typedef struct s_irc {
	std::string	prefix;
	std::string	command;
	std::string	parameters;
}		t_ircMessage;

class	Server {
	private:
		int					_serverPort;
		std::string			_serverPassword;
		int					_serverSocket;
		int					_optval;
		struct sockaddr_in	_serverAddress;
		std::vector<pollfd>	_fds;
		pollfd				_serverPollfd;
		std::vector<Client>	_clientVector;
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
		class FdException : public std::exception {
			public:
				virtual char const* what() const throw();
		};

		void		serverStart(void);
		void		serverSetup(void);
		void		addClient(void);
		bool		parseSplit(std::string const&, std::string&, std::string&, std::string&);
		void		parseClientInput(std::string const&, std::vector<t_ircMessage>&);
		void		handleClient(char*, Client& client);

		void		sendMessage(Client& client, std::string message);
};
