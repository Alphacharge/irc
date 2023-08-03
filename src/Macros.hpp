/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 13:57:29 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/02 14:28:36 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# define VERBOSE	0 //0 zero output, 1 errors, 2 warnings, 3 debug

# define ERR_EMPTY_STACK		"Stack is empty !"

// Client status codes
# define CONNECTED		1
# define CAP			2
# define AUTHENTICATED	3
# define NICK			4
# define USER			5
# define REGISTERED		6
