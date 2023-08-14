/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Colors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 08:37:46 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/14 14:04:55 by rbetz            ###   ########.fr       */
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

# define VERBOSE	1 //3 Constructors, 2 Infos from Commands, 1 Input from Client, 0 Zero

#endif
