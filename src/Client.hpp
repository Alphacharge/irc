/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 09:38:50 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/02 14:39:48 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <iostream>
# include <netinet/in.h>
# include <poll.h>
# include "Macros.hpp"

class Client {
	private:
		int					_clientSocket;
		struct sockaddr_in	_clientAddress;
		pollfd				_clientPollfd;
		std::string			_name;

	public:
		Client(void);
		Client(Client const&);
		~Client(void);
		Client&	operator=(Client const&);

		void	setClientSocket(int const&);
		void	setClientAddress(struct sockaddr_in const&);
		void	setClientPollfd_fd(int const&);
		void	setClientPollfd_events(short const&);

		int&				getClientSocket(void);
		struct sockaddr_in&	getClientAddress(void);
		pollfd&				getClientPollfd(void);
		std::string			getName(void);
};
