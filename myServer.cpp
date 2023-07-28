/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   myServer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 13:41:15 by lsordo            #+#    #+#             */
/*   Updated: 2023/07/28 14:30:34 by lsordo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <netinet/in.h>

#define PORT	8080;

int main(int argc, char** argv) {
	static_cast<void>(argc);
	static_cast<void>(argv);

	// socket creation
	int	server_fd =  socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		std::cerr << strerror(errno) << std::endl; // todo exception instead
		exit(EXIT_FAILURE);
	}

	//socket configuration
	int		opt = 1;
	struct sockaddr_in	address;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr <<strerror(errno) << std::endl; // todo exception instead
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET; // should not be necessary given socket's creation
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	return 0;
}

