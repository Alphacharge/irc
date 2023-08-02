/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:33:15 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/02 14:21:14 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <stdlib.h>

int	main(int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "Usage : ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	int port = atoi(argv[1]);
	std::string	password = static_cast<std::string>(argv[2]);
	Server	ircServer(port, password);
	ircServer.serverStart();
	return 0;
}
