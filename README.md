# IRC Project README
## Overview
Welcome to the IRC (Internet Relay Chat) project, a collaborative real-time messaging platform designed to facilitate communication among users in a distributed network. This project aims to provide a comprehensive understanding of networking concepts and protocols while implementing a functional IRC server and client.
It's coded in C++ STD 98 and the reference client is KVIrc 5.0.0.

## Features
- Real-time Chat: Engage in instant messaging with users connected to the IRC server.
- Multiple Channels: Create and join different channels for organized discussions.
- User Authentication: Securely log in to the IRC server with a unique identifier.
- Private Messaging: Send private messages to individual users for more personal conversations.
- Command Handling: Implement various IRC commands for operations like joining channels, changing nicknames, and more.

## Installation
This project should compile under Mac OS and Unix:
```
git clone https://github.com/Alphacharge/irc.git
cd irc
make
```

## IRC Commands
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

## Server Usage
```
./ircserv <port> <password>
```

## Client Usage
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

## Contributors
- [fkernbac](https://github.com/fkernbac)
- [lsordo](https://github.com/sc00bid00)
- [rbetz](https://github.com/Alphacharge)

## License
This project is licensed under the GPL-3.0 License - see the LICENSE file for details.

## Acknowledgments
- Thank you to the 42 school for providing the opportunity to work on this project.
- Inspiration from various UNIX shells and open-source projects.

Feel free to contribute, report issues, or suggest improvements! Happy coding!
