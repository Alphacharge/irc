/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:33:15 by rbetz             #+#    #+#             */
/*   Updated: 2023/07/25 14:21:25 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int main(int argc, char** argv)
{
	if (argc < 2)
		throw TooFewArgsException();
	else if (argc > 2)
		throw TooManyArgsException();
	else
		return 1;
	return 0;
}
