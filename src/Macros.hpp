/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 13:57:29 by lsordo            #+#    #+#             */
/*   Updated: 2023/08/02 16:28:01 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# define VERBOSE	3 //0 zero output, 1 errors, 2 warnings, 3 debug

// Client status codes
# define DISCONNECTED	0
# define CONNECTED		1
# define CAP			2
# define AUTHENTICATED	3
# define NICKGIVEN		4
# define USERGIVEN		5
# define REGISTERED		6
