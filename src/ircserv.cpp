/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:33:15 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/10 11:24:38 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <stdlib.h>

int	ft_strisdigit(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	main(int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "Usage : ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	if (!ft_strisdigit(argv[1])) {
		std::cerr << "No valid Port!" << std::endl;
		return 1;
	}
	int port = atoi(argv[1]);
	if (port < 6665 || port > 6669) {
		std::cerr << "No valid Port! Use a Port between 6665 and 6669" << std::endl;
		return 1;
	}
	std::string	password = static_cast<std::string>(argv[2]);
	Server	ircServer(port, password);
	ircServer.serverStart();
	return 0;
}
