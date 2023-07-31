/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   myServer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 13:41:15 by lsordo            #+#    #+#             */
/*   Updated: 2023/07/31 09:18:19 by lsordo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

#define PORT	8080

int main(int argc, char** argv) {
	static_cast<void>(argc);
	static_cast<void>(argv);

	// socket creation
	int	server_fd =  socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		std::cerr << strerror(errno) << std::endl; // todo exception instead
		exit(EXIT_FAILURE);
	}

	// socket configuration
	int		opt = 1;
	struct sockaddr_in	address;
	int					addrlen = sizeof(address);
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr << strerror(errno) << std::endl; // todo exception instead of exit
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET; // should not be necessary given socket's creation
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// socket binding
	if(bind(server_fd, (struct sockaddr*)(&address), addrlen) < 0) {
		std::cerr << strerror(errno) <<std::endl; // todo exception istead of exit
		exit(EXIT_FAILURE);
	}

	// server listening
	if (listen(server_fd, 3) < 0) {
		std::cerr << strerror(errno) <<std::endl; // todo exception istead of exit
		exit(EXIT_FAILURE);
	}

	// accepting client
	int	new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
	if (new_socket < 0) {
		std::cerr << strerror(errno) <<std::endl; // todo exception istead of exit
		exit(EXIT_FAILURE);
	}

	// read and display incoming
	char	buffer[1024] = {0};
	int		valread = read(new_socket, buffer, 1024);
	std::cout << buffer << std::endl;

	// reply to client
	const char* hello = "Hello from server !";
	if (send(new_socket, hello, strlen(hello), 0) < 0) {
		std::cerr << strerror(errno) <<std::endl; // todo exception istead of exit
		exit(EXIT_FAILURE);
	};
	std::cout << "Hello message sent." << std::endl;

	// close client connection
	close(new_socket);

	// shutdown server socket
	shutdown(server_fd, SHUT_RDWR);

	return 0;
}

