# 42 irc Project
This project is a selfcoded irc server (Internet Relay Chat) from [fkernbac](https://github.com/fkernbac), [lsordo](https://github.com/sc00bid00) and [rbetz](https://github.com/Alphacharge).
It's coded in C++ STD 98 and the reference client is KVIrc 5.0.0.

## Installation
This project should compile under Mac OS and Unix:
```
git clone https://github.com/Alphacharge/irc.git
cd irc
make
```

## Description
The server knows following commands:
- PASS
- USER
- NICK
- JOIN
- PRIVMSG
- KICK
- INVITE
- TOPIC
- MODE
  + +/- i Invite only
  + +/- t restrict Topic
  + +/- k Channel password
  + +/- o Operator privilege
  + +/- l User limit

## Usage
```
./ircserv <port> <password>
```

Example with nc:
```
nc localhost <port>
PASS <password>
USER <username> <hostname> <servername> <realname>
NICK <nickname>
JOIN #testchannel [password]
MODE #testchannel +i
TOPIC #testchannel [new topic]
PRIVMSG otheruser I invite you to testchannel
INVITE otheruser #testchannel
MODE #testchannel +t
PRIVMSG #testchannel I invited otheruser to our channel
KICK #testchannel otheruser [reason]
```
