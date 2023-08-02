/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Colors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 08:37:46 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/02 06:55:59 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef COLORS_HPP
# define COLORS_HPP

# define RED		"\x01\033[1;31m\x02"
# define YELLOW		"\x01\033[1;33m\x02"
# define DGREEN		"\x01\033[1;32m\x02"
# define BLUE		"\x01\033[1;34m\x02"
# define WHITE		"\x01\033[0m\x02"
# define MAGENTA	"\x01\033[1;38;2;255;0;255m\x02"
# define ORANGE		"\x01\033[1;38;2;255;165;0m\x02"
# define CYAN		"\x01\033[1;38;2;0;255;255m\x02"
# define PURPLE		"\x01\033[1;38;2;128;0;128m\x02"
# define LGREEN		"\x01\033[1;38;2;0;255;0m\x02"

# define VERBOSE	0 //0 zero output, 1 errors, 2 warnings, 3 debug
#endif

/*---------------	Constructors	---------------*/
/*---------------	Destructor		---------------*/
/*---------------	Operators		---------------*/
/*---------------	Getters/Setters	---------------*/
/*---------------	Methods			---------------*/
/*---------------	Variables		---------------*/
/*---------------	Exceptions		---------------*/

/*
if (VERBOSE)
		std::cout << DGREEN << "xx default constructor called" << WHITE << std::endl;
if (VERBOSE)
		std::cout << YELLOW << "xx copy constructor called" << WHITE << std::endl;
if (VERBOSE)
		std::cout << RED << "xx destructor called" << WHITE << std::endl;
if (VERBOSE)
		std::cout << BLUE << "xx assignment operator called" << WHITE << std::endl;
*/
