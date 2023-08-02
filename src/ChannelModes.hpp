/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelModes.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 08:37:46 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/02 09:14:19 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CHANNELMODES_HPP
# define CHANNELMODES_HPP

enum class ChannelMode {
	Secret = 's',		// +s: Channel is secret, not listed in /list command
	InviteOnly = 'i',	// +i: Channel is invite-only
	Moderated = 'm',	// +m: Channel is moderated, only ops can send messages
	NoOutsideMsg = 'n',	// +n: Only channel members can send messages
	TopicLock = 't',	// +t: Only ops can change the topic
	Private = 'p',		// +p: Private channel, invisible to non-members
	ModeratedExt = 'M',	// +M: Moderated, only registered members can send messages
	InviteExt = 'I',	// +I: Invite-only, only registered members can join
	SecretExt = 'S',	// +S: Secret, only registered members can find the channel
};
#endif
