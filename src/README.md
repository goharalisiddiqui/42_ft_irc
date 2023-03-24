# ft_irc

ft_irc is an IRC server developed in C++98 that can handle multiple clients simultaneously and never hangs. The server is capable of authenticating users, setting nicknames and usernames, joining channels, sending and receiving private messages, and forwarding messages from one client to every other client that joined the channel.
## Requirements

To run the ft_irc server, you need to have the following:

- A C++98 compiler
- TCP/IP (v4 or v6) connectivity
- A client that can connect to the server via TCP/IP

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
- Nickname and username setting
- Joining channels
- Sending and receiving private messages
- Forwarding messages from one client to every other client that joined the channel
- Operators and regular users
- Commands that are specific to operators
- Handle file transfer
- A bot

## Limitations

The following limitations apply to the ft_irc server:

- The server cannot handle server-to-server communication.
- The server does not have a built-in client, so you need to use a separate IRC client to connect to the server.
- Forking is not allowed. All I/O operations must be non-blocking.
- Only 1 poll() (or equivalent) can be used for handling all these operations (read, write, but also listen, and so forth).

## Authors

ft_irc was developed by [Arno](https://github.com/AF200), [Gohar](https://github.com/goharalisiddiqui), and [Fredi](https://github.com/Fredi-B).
