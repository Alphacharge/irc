/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_getline.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsordo <lsordo@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 07:45:57 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/03 12:35:49 by lsordo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sstream>
#include <iostream>

int main(void) {
	// char const* buffer = "Lorem Ipsum is simply dummy text \nof the printing and typesetting industry. \nLorem Ipsum has been the industry's standard dummy text\
	//  ever since the 1500s, when an unknown printer took a galley of type\n and scrambled it to make a type specimen book. \nIt has survived not only five \
	//  centuries, but also the leap into electronic \ntypesetting, remaining essentially unchanged. It was popularised \nin the 1960s with the release of Letraset \
	//  sheets containing Lorem Ipsum passages, \nand more recently with desktop publishing software like Aldus \nPageMaker including versions of Lorem Ipsum.";
	// std::istringstream	iss(buffer);
	std::string line;
	while(std::getline(std::cin, line)) {
		if(std::cin.eof()) {
			std::cout << std::endl << "got a : ^D" << std::endl;
			std::cin.clear();
		}
		std::cout << line << std::endl;}
	std::cout << "exited loop" << std::endl;
	return 0;
}
