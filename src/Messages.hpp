/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbetz <rbetz@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 08:37:46 by rbetz             #+#    #+#             */
/*   Updated: 2023/08/02 16:20:34 by rbetz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef MESSAGES_HPP
# define MESSAGES_HPP

# define RPL_WELCOME(client, serverAddress) (":Welcome to the Internet Relay Network " + client.getNick() + "!" + client.getName() + "@" + serverAddress.sin_addr.s_addr)
// # define RPL_YOURHOST() ()
// # define RPL_CREATED() ()
// # define RPL_MYINFO() ()
// # define RPL_MYINFO() ()
// # define RPL_BOUNCE() ()
// # define RPL_ISUPPORT() ()
// # define RPL_MAP() ()
// # define RPL_MAPEND() ()
// # define RPL_SNOMASK() ()
// # define RPL_STATMEMTOT() ()
// # define RPL_BOUNCE() ()
// # define RPL_STATMEM() ()
// # define RPL_YOURCOOKIE() ()
// # define RPL_MAP() ()
// # define RPL_MAPMORE() ()
// # define RPL_MAPEND() ()
// # define RPL_YOURID() ()
// # define RPL_SAVENICK() ()
// # define RPL_ATTEMPTINGJUNC() ()
// # define RPL_ATTEMPTINGREROUTE() ()
// # define RPL_TRACELINK() ()
// # define RPL_TRACECONNECTING() ()
// # define RPL_TRACEHANDSHAKE() ()
// # define RPL_TRACEUNKNOWN() ()
// # define RPL_TRACEOPERATOR() ()
// # define RPL_TRACEUSER() ()
// # define RPL_TRACESERVER() ()
// # define RPL_TRACESERVICE() ()
// # define RPL_TRACENEWTYPE() ()
// # define RPL_TRACECLASS() ()
// # define RPL_TRACERECONNECT() ()
// # define RPL_STATS() ()
// # define RPL_STATSLINKINFO() ()
// # define RPL_STATSCOMMANDS() ()
// # define RPL_STATSCLINE() ()
// # define RPL_STATSNLINE() ()
// # define RPL_STATSILINE() ()
// # define RPL_STATSKLINE() ()
// # define RPL_STATSQLINE() ()
// # define RPL_STATSPLINE() ()
// # define RPL_STATSYLINE() ()
// # define RPL_ENDOFSTATS() ()
// # define RPL_STATSPLINE() ()
// # define RPL_STATSBLINE() ()
// # define RPL_UMODEIS() ()
// # define RPL_MODLIST() ()
// # define RPL_SQLINE_NICK() ()
// # define RPL_STATSBLINE() ()
// # define RPL_STATSELINE() ()
// # define RPL_STATSGLINE() ()
// # define RPL_STATSFLINE() ()
// # define RPL_STATSTLINE() ()
// # define RPL_STATSDLINE() ()
// # define RPL_STATSZLINE() ()
// # define RPL_STATSELINE() ()
// # define RPL_STATSCOUNT() ()
// # define RPL_STATSNLINE() ()
// # define RPL_STATSGLINE() ()
// # define RPL_STATSVLINE() ()
// # define RPL_STATSQLINE() ()
// # define RPL_SERVICEINFO() ()
// # define RPL_ENDOFSERVICES() ()
// # define RPL_RULES() ()
// # define RPL_SERVICE() ()
// # define RPL_SERVLIST() ()
// # define RPL_SERVLISTEND() ()
// # define RPL_STATSVERBOSE() ()
// # define RPL_STATSENGINE() ()
// # define RPL_STATSFLINE() ()
// # define RPL_STATSIAUTH() ()
// # define RPL_STATSVLINE() ()
// # define RPL_STATSXLINE() ()
// # define RPL_STATSLLINE() ()
// # define RPL_STATSUPTIME() ()
// # define RPL_STATSOLINE() ()
// # define RPL_STATSHLINE() ()
// # define RPL_STATSSLINE() ()
// # define RPL_STATSPING() ()
// # define RPL_STATSTLINE() ()
// # define RPL_STATSULINE() ()
// # define RPL_STATSBLINE() ()
// # define RPL_STATSXLINE() ()
// # define RPL_STATSGLINE() ()
// # define RPL_STATSULINE() ()
// # define RPL_STATSDEFINE() ()
// # define RPL_STATSULINE() ()
// # define RPL_STATSDEBUG() ()
// # define RPL_STATSDLINE() ()
// # define RPL_STATSCONN() ()
// # define RPL_LUSERCLIENT() ()
// # define RPL_LUSEROP() ()
// # define RPL_LUSERUNKNOWN() ()
// # define RPL_LUSERCHANNELS() ()
// # define RPL_LUSERME() ()
// # define RPL_ADMINME() ()
// # define RPL_ADMINLOC1() ()
// # define RPL_ADMINLOC2() ()
// # define RPL_ADMINEMAIL() ()
// # define RPL_TRACELOG() ()
// # define RPL_TRACEPING() ()
// # define RPL_TRACEEND() ()
// # define RPL_TRYAGAIN() ()
// # define RPL_LOCALUSERS() ()
// # define RPL_GLOBALUSERS() ()
// # define RPL_START_NETSTAT() ()
// # define RPL_NETSTAT() ()
// # define RPL_END_NETSTAT() ()
// # define RPL_PRIVS() ()
// # define RPL_SILELIST() ()
// # define RPL_ENDOFSILELIST() ()
// # define RPL_NOTIFY() ()
// # define RPL_ENDNOTIFY() ()
// # define RPL_STATSDELTA() ()
// # define RPL_STATSDLINE() ()
// # define RPL_VCHANEXIST() ()
// # define RPL_VCHANLIST() ()
// # define RPL_VCHANHELP() ()
// # define RPL_GLIST() ()
// # define RPL_ENDOFGLIST() ()
// # define RPL_ACCEPTLIST() ()
// # define RPL_ENDOFACCEPT() ()
// # define RPL_JUPELIST() ()
// # define RPL_ALIST() ()
// # define RPL_ENDOFJUPELIST() ()
// # define RPL_ENDOFALIST() ()
// # define RPL_FEATURE() ()
// # define RPL_GLIST_HASH() ()
// # define RPL_CHANINFO_HANDLE() ()
// # define RPL_NEWHOSTIS() ()
// # define RPL_CHANINFO_USERS() ()
// # define RPL_CHKHEAD() ()
// # define RPL_CHANINFO_CHOPS() ()
// # define RPL_CHANUSER() ()
// # define RPL_CHANINFO_VOICES() ()
// # define RPL_PATCHHEAD() ()
// # define RPL_CHANINFO_AWAY() ()
// # define RPL_PATCHCON() ()
// # define RPL_CHANINFO_OPERS() ()
// # define RPL_HELPHDR() ()
// # define RPL_DATASTR() ()
// # define RPL_CHANINFO_BANNED() ()
// # define RPL_HELPOP() ()
// # define RPL_ENDOFCHECK() ()
// # define RPL_CHANINFO_BANS() ()
// # define RPL_HELPTLR() ()
// # define RPL_CHANINFO_INVITE() ()
// # define RPL_HELPHLP() ()
// # define RPL_CHANINFO_INVITES() ()
// # define RPL_HELPFWD() ()
// # define RPL_CHANINFO_KICK() ()
// # define RPL_HELPIGN() ()
// # define RPL_CHANINFO_KICKS() ()
// # define RPL_END_CHANINFO() ()
// # define RPL_NONE() ()
// # define RPL_AWAY() ()
// # define RPL_AWAY() ()
// # define RPL_USERHOST() ()
// # define RPL_ISON() ()
// # define RPL_TEXT() ()
// # define RPL_UNAWAY() ()
// # define RPL_NOWAWAY() ()
// # define RPL_USERIP() ()
// # define RPL_WHOISREGNICK() ()
// # define RPL_SUSERHOST() ()
// # define RPL_NOTIFYACTION() ()
// # define RPL_WHOISADMIN() ()
// # define RPL_RULESSTART() ()
// # define RPL_NICKTRACE() ()
// # define RPL_WHOISSADMIN() ()
// # define RPL_ENDOFRULES() ()
// # define RPL_WHOISHELPER() ()
// # define RPL_WHOISSVCMSG() ()
// # define RPL_WHOISHELPOP() ()
// # define RPL_WHOISSERVICE() ()
// # define RPL_WHOISUSER() ()
// # define RPL_WHOISSERVER() ()
// # define RPL_WHOISOPERATOR() ()
// # define RPL_WHOWASUSER() ()
// # define RPL_ENDOFWHO() ()
// # define RPL_WHOISCHANOP() ()
// # define RPL_WHOISIDLE() ()
// # define RPL_ENDOFWHOIS() ()
// # define RPL_WHOISCHANNELS() ()
// # define RPL_WHOISVIRT() ()
// # define RPL_WHOIS_HIDDEN() ()
// # define RPL_WHOISSPECIAL() ()
// # define RPL_LISTSTART() ()
// # define RPL_LIST() ()
// # define RPL_LISTEND() ()
// # define RPL_CHANNELMODEIS() ()
// # define RPL_UNIQOPIS() ()
// # define RPL_CHANNELPASSIS() ()
// # define RPL_NOCHANPASS() ()
// # define RPL_CHPASSUNKNOWN() ()
// # define RPL_CHANNEL_URL() ()
// # define RPL_CREATIONTIME() ()
// # define RPL_WHOWAS_TIME() ()
// # define RPL_WHOISACCOUNT() ()
// # define RPL_NOTOPIC() ()
// # define RPL_TOPIC() ()
// # define RPL_TOPICWHOTIME() ()
// # define RPL_LISTUSAGE() ()
// # define RPL_COMMANDSYNTAX() ()
// # define RPL_LISTSYNTAX() ()
// # define RPL_WHOISBOT() ()
// # define RPL_CHANPASSOK() ()
// # define RPL_WHOISACTUALLY() ()
// # define RPL_BADCHANPASS() ()
// # define RPL_USERIP() ()
// # define RPL_INVITING() ()
// # define RPL_SUMMONING() ()
// # define RPL_INVITED() ()
// # define RPL_INVITELIST() ()
// # define RPL_ENDOFINVITELIST() ()
// # define RPL_EXCEPTLIST() ()
// # define RPL_ENDOFEXCEPTLIST() ()
// # define RPL_VERSION() ()
// # define RPL_WHOREPLY() ()
// # define RPL_NAMREPLY() ()
// # define RPL_WHOSPCRPL() ()
// # define RPL_NAMREPLY_() ()
// # define RPL_MAP() ()
// # define RPL_MAPMORE() ()
// # define RPL_MAPEND() ()
// # define RPL_KILLDONE() ()
// # define RPL_CLOSING() ()
// # define RPL_CLOSEEND() ()
// # define RPL_LINKS() ()
// # define RPL_ENDOFLINKS() ()
// # define RPL_ENDOFNAMES() ()
// # define RPL_BANLIST() ()
// # define RPL_ENDOFBANLIST() ()
// # define RPL_ENDOFWHOWAS() ()
// # define RPL_INFO() ()
// # define RPL_MOTD() ()
// # define RPL_INFOSTART() ()
// # define RPL_ENDOFINFO() ()
// # define RPL_MOTDSTART() ()
// # define RPL_ENDOFMOTD() ()
// # define RPL_KICKEXPIRED() ()
// # define RPL_SPAM() ()
// # define RPL_BANEXPIRED() ()
// # define RPL_WHOISHOST() ()
// # define RPL_MOTD() ()
// # define RPL_KICKLINKED() ()
// # define RPL_WHOISMODES() ()
// # define RPL_BANLINKED() ()
// # define RPL_YOURHELPER() ()
// # define RPL_YOUREOPER() ()
// # define RPL_REHASHING() ()
// # define RPL_YOURESERVICE() ()
// # define RPL_MYPORTIS() ()
// # define RPL_NOTOPERANYMORE() ()
// # define RPL_QLIST() ()
// # define RPL_IRCOPS() ()
// # define RPL_ENDOFQLIST() ()
// # define RPL_ENDOFIRCOPS() ()
// # define RPL_ALIST() ()
// # define RPL_ENDOFALIST() ()
// # define RPL_TIME() ()
// # define RPL_TIME() ()
// # define RPL_TIME() ()
// # define RPL_TIME() ()
// # define RPL_USERSSTART() ()
// # define RPL_USERS() ()
// # define RPL_ENDOFUSERS() ()
// # define RPL_NOUSERS() ()
// # define RPL_HOSTHIDDEN() ()
// # define ERR_UNKNOWNERROR() ()
// # define ERR_NOSUCHNICK() ()
// # define ERR_NOSUCHSERVER() ()
// # define ERR_NOSUCHCHANNEL() ()
// # define ERR_CANNOTSENDTOCHAN() ()
// # define ERR_TOOMANYCHANNELS() ()
// # define ERR_WASNOSUCHNICK() ()
// # define ERR_TOOMANYTARGETS() ()
// # define ERR_NOSUCHSERVICE() ()
// # define ERR_NOCOLORSONCHAN() ()
// # define ERR_NOORIGIN() ()
// # define ERR_NORECIPIENT() ()
// # define ERR_NOTEXTTOSEND() ()
// # define ERR_NOTOPLEVEL() ()
// # define ERR_WILDTOPLEVEL() ()
// # define ERR_BADMASK() ()
// # define ERR_TOOMANYMATCHES() ()
// # define ERR_QUERYTOOLONG() ()
// # define ERR_LENGTHTRUNCATED() ()
// # define ERR_UNKNOWNCOMMAND() ()
// # define ERR_NOMOTD() ()
// # define ERR_NOADMININFO() ()
// # define ERR_FILEERROR() ()
// # define ERR_NOOPERMOTD() ()
// # define ERR_TOOMANYAWAY() ()
// # define ERR_EVENTNICKCHANGE() ()
// # define ERR_NONICKNAMEGIVEN() ()
// # define ERR_ERRONEUSNICKNAME() ()
// # define ERR_NICKNAMEINUSE() ()
// # define ERR_SERVICENAMEINUSE() ()
// # define ERR_NORULES() ()
// # define ERR_SERVICECONFUSED() ()
// # define ERR_BANONCHAN() ()
// # define ERR_NICKCOLLISION() ()
// # define ERR_UNAVAILRESOURCE() ()
// # define ERR_BANNICKCHANGE() ()
// # define ERR_NICKTOOFAST() ()
// # define ERR_DEAD() ()
// # define ERR_TARGETTOOFAST() ()
// # define ERR_SERVICESDOWN() ()
// # define ERR_USERNOTINCHANNEL() ()
// # define ERR_NOTONCHANNEL() ()
// # define ERR_USERONCHANNEL() ()
// # define ERR_NOLOGIN() ()
// # define ERR_SUMMONDISABLED() ()
// # define ERR_USERSDISABLED() ()
// # define ERR_NONICKCHANGE() ()
// # define ERR_NOTIMPLEMENTED() ()
// # define ERR_NOTREGISTERED() ()
// # define ERR_IDCOLLISION() ()
// # define ERR_NICKLOST() ()
// # define ERR_HOSTILENAME() ()
// # define ERR_ACCEPTFULL() ()
// # define ERR_ACCEPTEXIST() ()
// # define ERR_ACCEPTNOT() ()
// # define ERR_NOHIDING() ()
// # define ERR_NOTFORHALFOPS() ()
// # define ERR_NEEDMOREPARAMS() ()
// # define ERR_ALREADYREGISTERED() ()
// # define ERR_NOPERMFORHOST() ()
// # define ERR_PASSWDMISMATCH() ()
// # define ERR_YOUREBANNEDCREEP() ()
// # define ERR_YOUWILLBEBANNED() ()
// # define ERR_KEYSET() ()
// # define ERR_INVALIDUSERNAME() ()
// # define ERR_ONLYSERVERSCANCHANGE() ()
// # define ERR_LINKSET() ()
// # define ERR_LINKCHANNEL() ()
// # define ERR_KICKEDFROMCHAN() ()
// # define ERR_CHANNELISFULL() ()
// # define ERR_UNKNOWNMODE() ()
// # define ERR_INVITEONLYCHAN() ()
// # define ERR_BANNEDFROMCHAN() ()
// # define ERR_BADCHANNELKEY() ()
// # define ERR_BADCHANMASK() ()
// # define ERR_NOCHANMODES() ()
// # define ERR_NEEDREGGEDNICK() ()
// # define ERR_BANLISTFULL() ()
// # define ERR_BADCHANNAME() ()
// # define ERR_LINKFAIL() ()
// # define ERR_NOULINE() ()
// # define ERR_CANNOTKNOCK() ()
// # define ERR_NOPRIVILEGES() ()
// # define ERR_CHANOPRIVSNEEDED() ()
// # define ERR_CANTKILLSERVER() ()
// # define ERR_RESTRICTED() ()
// # define ERR_ISCHANSERVICE() ()
// # define ERR_DESYNC() ()
// # define ERR_ATTACKDENY() ()
// # define ERR_UNIQOPRIVSNEEDED() ()
// # define ERR_KILLDENY() ()
// # define ERR_CANTKICKADMIN() ()
// # define ERR_ISREALSERVICE() ()
// # define ERR_NONONREG() ()
// # define ERR_HTMDISABLED() ()
// # define ERR_ACCOUNTONLY() ()
// # define ERR_CHANTOORECENT() ()
// # define ERR_MSGSERVICES() ()
// # define ERR_TSLESSCHAN() ()
// # define ERR_VOICENEEDED() ()
// # define ERR_SECUREONLYCHAN() ()
// # define ERR_NOOPERHOST() ()
// # define ERR_NOSERVICEHOST() ()
// # define ERR_NOFEATURE() ()
// # define ERR_BADFEATURE() ()
// # define ERR_BADLOGTYPE() ()
// # define ERR_BADLOGSYS() ()
// # define ERR_BADLOGVALUE() ()
// # define ERR_ISOPERLCHAN() ()
// # define ERR_CHANOWNPRIVNEEDED() ()
// # define ERR_UMODEUNKNOWNFLAG() ()
// # define ERR_USERSDONTMATCH() ()
// # define ERR_GHOSTEDCLIENT() ()
// # define ERR_VWORLDWARN() ()
// # define ERR_USERNOTONSERV() ()
// # define ERR_SILELISTFULL() ()
// # define ERR_TOOMANYWATCH() ()
// # define ERR_BADPING() ()
// # define ERR_INVALID_ERROR() ()
// # define ERR_TOOMANYDCC() ()
// # define ERR_BADEXPIRE() ()
// # define ERR_DONTCHEAT() ()
// # define ERR_DISABLED() ()
// # define ERR_NOINVITE() ()
// # define ERR_LONGMASK() ()
// # define ERR_ADMONLY() ()
// # define ERR_TOOMANYUSERS() ()
// # define ERR_OPERONLY() ()
// # define ERR_MASKTOOWIDE() ()
// # define ERR_WHOTRUNC() ()
// # define ERR_LISTSYNTAX() ()
// # define ERR_WHOSYNTAX() ()
// # define ERR_WHOLIMEXCEED() ()
// # define ERR_QUARANTINED() ()
// # define ERR_OPERSPVERIFY() ()
// # define ERR_REMOTEPFX() ()
// # define ERR_PFXUNROUTABLE() ()
// # define ERR_BADHOSTMASK() ()
// # define ERR_HOSTUNAVAIL() ()
// # define ERR_USINGSLINE() ()
// # define ERR_STATSSLINE() ()
// # define RPL_LOGON() ()
// # define RPL_LOGOFF() ()
// # define RPL_WATCHOFF() ()
// # define RPL_WATCHSTAT() ()
// # define RPL_NOWON() ()
// # define RPL_NOWOFF() ()
// # define RPL_WATCHLIST() ()
// # define RPL_ENDOFWATCHLIST() ()
// # define RPL_WATCHCLEAR() ()
// # define RPL_MAPMORE() ()
// # define RPL_ISOPER() ()
// # define RPL_ISLOCOP() ()
// # define RPL_ISNOTOPER() ()
// # define RPL_ENDOFISOPER() ()
// # define RPL_MAPMORE() ()
// # define RPL_WHOISMODES() ()
// # define RPL_WHOISHOST() ()
// # define RPL_DCCSTATUS() ()
// # define RPL_WHOISBOT() ()
// # define RPL_DCCLIST() ()
// # define RPL_ENDOFDCCLIST() ()
// # define RPL_WHOWASHOST() ()
// # define RPL_DCCINFO() ()
// # define RPL_RULESSTART() ()
// # define RPL_RULES() ()
// # define RPL_ENDOFRULES() ()
// # define RPL_MAPMORE() ()
// # define RPL_OMOTDSTART() ()
// # define RPL_OMOTD() ()
// # define RPL_ENDOFO() ()
// # define RPL_SETTINGS() ()
// # define RPL_ENDOFSETTINGS() ()
// # define RPL_DUMPING() ()
// # define RPL_DUMPRPL() ()
// # define RPL_EODUMP() ()
// # define RPL_TRACEROUTE_HOP() ()
// # define RPL_TRACEROUTE_START() ()
// # define RPL_MODECHANGEWARN() ()
// # define RPL_CHANREDIR() ()
// # define RPL_SERVMODEIS() ()
// # define RPL_OTHERUMODEIS() ()
// # define RPL_ENDOF_GENERIC() ()
// # define RPL_WHOWASDETAILS() ()
// # define RPL_WHOISSECURE() ()
// # define RPL_UNKNOWNMODES() ()
// # define RPL_CANNOTSETMODES() ()
// # define RPL_LUSERSTAFF() ()
// # define RPL_TIMEONSERVERIS() ()
// # define RPL_NETWORKS() ()
// # define RPL_YOURLANGUAGEIS() ()
// # define RPL_LANGUAGE() ()
// # define RPL_WHOISSTAFF() ()
// # define RPL_WHOISLANGUAGE() ()
// # define RPL_MODLIST() ()
// # define RPL_ENDOFMODLIST() ()
// # define RPL_HELPSTART() ()
// # define RPL_HELPTXT() ()
// # define RPL_ENDOFHELP() ()
// # define RPL_ETRACEFULL() ()
// # define RPL_ETRACE() ()
// # define RPL_KNOCK() ()
// # define RPL_KNOCKDLVR() ()
// # define ERR_TOOMANYKNOCK() ()
// # define ERR_CHANOPEN() ()
// # define ERR_KNOCKONCHAN() ()
// # define ERR_KNOCKDISABLED() ()
// # define RPL_TARGUMODEG() ()
// # define RPL_TARGNOTIFY() ()
// # define RPL_UMODEGMSG() ()
// # define RPL_OMOTDSTART() ()
// # define RPL_OMOTD() ()
// # define RPL_ENDOFOMOTD() ()
// # define ERR_NOPRIVS() ()
// # define RPL_TESTMARK() ()
// # define RPL_TESTLINE() ()
// # define RPL_NOTESTLINE() ()
// # define RPL_XINFO() ()
// # define RPL_XINFOSTART() ()
// # define RPL_XINFOEND() ()
// # define ERR_CANNOTDOCOMMAND() ()
// # define ERR_CANNOTCHANGEUMODE() ()
// # define ERR_CANNOTCHANGECHANMODE() ()
// # define ERR_CANNOTCHANGESERVERMODE() ()
// # define ERR_CANNOTSENDTONICK() ()
// # define ERR_UNKNOWNSERVERMODE() ()
// # define ERR_SERVERMODELOCK() ()
// # define ERR_BADCHARENCODING() ()
// # define ERR_TOOMANYLANGUAGES() ()
// # define ERR_NOLANGUAGE() ()
// # define ERR_TEXTTOOSHORT() ()
// # define ERR_NUMERIC_ERR() ()
#endif
