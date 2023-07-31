/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:02:58 by lsordo            #+#    #+#             */
/*   Updated: 2023/07/31 18:33:53 by lsordo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	main(int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "Usage : ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	int port = atoi(argv[1]);
	std::string	password = static_cast<std::string>(argv[2]);
	Server	ircServer(port, password);
	ircServer.serverSetup();
	return 0;
}
