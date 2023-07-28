/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   myServer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 13:41:15 by lsordo            #+#    #+#             */
/*   Updated: 2023/07/28 14:18:16 by lsordo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <stdio.h>
#include <iostream>

int main(int argc, char** argv) {
	static_cast<void>(argc);
	static_cast<void>(argv);

	int	server_fd =  socket(AF_INET, SOCK_STREAM, 0);
	int	opt = 1;
	if (server_fd < 0) {
		std::cerr << strerror(errno) << std::endl; // todo exception instead
		exit(EXIT_FAILURE);
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr <<strerror(errno) << std::endl; // todo exception instead
		exit(EXIT_FAILURE);
	}

	return 0;
}

