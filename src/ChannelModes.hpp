/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelModes.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 08:37:46 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/02 14:33:31 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CHANNELMODES_HPP
# define CHANNELMODES_HPP

typedef enum e_chmode {
	Secret,			// +s: Channel is secret, not listed in /list command
	InviteOnly,		// +i: Channel is invite-only
	Moderated,		// +m: Channel is moderated, only ops can send messages
	NoOutsideMsg,	// +n: Only channel members can send messages
	TopicLock,		// +t: Only ops can change the topic
	Private,		// +p: Private channel, invisible to non-members
	ModeratedExt,	// +M: Moderated, only registered members can send messages
	InviteExt,		// +I: Invite-only, only registered members can join
	SecretExt		// +S: Secret, only registered members can find the channel
}			t_chmode;
#endif
