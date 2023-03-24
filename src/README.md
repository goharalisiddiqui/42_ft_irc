# ft_irc

ft_irc is an IRC server developed in C++98 that can handle multiple clients simultaneously and never hangs. The server is capable of authenticating users, setting nicknames and usernames, sending and receiving private messages, joining channels and forwarding messages from one client to every other client that joined the channel.

## Requirements

To run the ft_irc server, you need to have the following:

- A C++98 compiler
- TCP/IP (v4 or v6) connectivity
- A client that can connect to the server via TCP/IP or Netcat

## Usage

To run the server, use the following command:

```
  ./ircserv <port> <password>
```

Where:

- `<port>`: The port number on which your IRC server will be listening to for incoming IRC connections.
- `<password>`: The connection password needed by any IRC client that tries to connect to your server.

## Features

The ft_irc server has the following features:

- Authentication
- Set nickname and username
- Sending and receiving private messages
- Join channels
- Forwarding messages from one client to every other client that joined the channel
- Commands that are specific to channel operators
- Handle file transfer
- A bot

## Commands

- /nick - Set nickname
- /user - Set username
- /join - Join channels
- /quit - Leave channels
- /list - list channels
- /privmsg
  - Sending and receiving private messages
  - Forwarding messages from one client to every other client that joined the channel
  - Chatting with chatbot  
### Channel operator commands:
- /invite - Invite a client to an invite-only channel
- /kick - Eject a client from the channel
- /topic - Change the channel topic in a mode +t channel
- /mode - Change the channel's mode
    - -o - give/take channel operator privileges
    - -i - invite-only
    - -t - topic settable by channel operator
---
## Limitations

The following limitations apply to the ft_irc server:

- The server cannot handle server-to-server communication.
- The server does not have a built-in client, so you need to use a separate IRC client to connect to the server.
  - The ft_irc server was developed for WeeChat.

---

### Restrictions

This 42 project is written in C++98 and in accordance to restrictions from the project subject.

Sample restrictions:
- Forking is not allowed. All I/O operations must be non-blocking.
- Only 1 poll() (or equivalent) can be used for handling all these operations (read, write, but also listen, and so forth).
---
## Authors

ft_irc was developed by [Arno](https://github.com/AF200), [Gohar](https://github.com/goharalisiddiqui), and [Fredi](https://github.com/Fredi-B).
