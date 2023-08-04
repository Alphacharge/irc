/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 14:39:17 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/04 07:18:00 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <vector>
# include <map>
# include <list>
# include <cstring>
# include <poll.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <unistd.h>
# include <string>
# include <string.h>
# include <sstream>
# include <algorithm>
# include "Client.hpp"
# include "Channel.hpp"
# include "Macros.hpp"
# include "Messages.hpp"
# include "Colors.hpp"
# include "Validation.hpp"

typedef struct s_irc {
	std::string	prefix;
	std::string	command;
	std::string	parameters;
}		t_ircMessage;

class Client;
class Channel;


class	Server {
	private:
		//Server
		int					_serverPort;
		std::string			_serverPassword;
		int					_serverSocket;
		int					_optval;
		struct sockaddr_in	_serverAddress;
		std::vector<pollfd>	_fds;
		pollfd				_serverPollfd;
		//Client
		std::vector<Client>	_clientVector;
		//Channels
		std::list<Channel>	_channel_list;
		//Functions
		std::map<std::string, void (Server::*)(Client&, std::string&)>	_commandMap;
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
		bool		inputParse(std::string const&, t_ircMessage&);
		void		handleClient(char*, std::vector<Client>::iterator&, std::vector<t_ircMessage>&);

		//Commands
		void		join(Client &client, std::string& channel);
		void	cap(Client& client, std::string& params);
		void	pong(Client &client, std::string& params);
		void	nick(Client &client, std::string& params);
		void	user(Client &client, std::string& params);

		//Helper functions
		void		sendMessage(Client&, std::string);
};
