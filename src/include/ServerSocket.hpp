#ifndef SERVERSOCKET_HPP
#define SERVERSOCKET_HPP

#pragma once

#include "libs.hpp"
#include "SimpleSocket.hpp"

// MACRO DEFINITIONS
#define AFG_SOCK_BOUND 2
#define AFG_SOCK_LISTENING 3


namespace AFG
{
    class ServerSocket : public SimpleSocket
    {
        public:
            ServerSocket();
            ~ServerSocket();
            ServerSocket(const ServerSocket &src);

            ServerSocket &operator=(const ServerSocket &src);

            ServerSocket(int domain, int type, int protocol);
            ServerSocket(struct s_sockparams par);

            struct sockaddr_in getaddr(void) const;

            void socklisten();
            void socklisten(struct in_addr ip, in_port_t port, int queue_len);

            int sockaccept();
            int sockaccept(struct sockaddr_in &raddr, socklen_t &socklen);


        private:
            struct sockaddr_in addr;
            const static struct sockaddr_in default_binding;

        public:

            class CannotBindSocket: public ::std::exception
            {
                public:
                    virtual const char* what() const throw()
                    {
                        return "Cannot bind socket";
                    }
            };

            class SocketCannotListen: public ::std::exception
            {
                public:
                    virtual const char* what() const throw()
                    {
                        return "Socket is unable to listen";
                    }
            };

            class SocketCannotAccept: public ::std::exception
            {
                public:
                    virtual const char* what() const throw()
                    {
                        return "Socket is unable to listen";
                    }
            };

    };

}

#include "ErrorHandler.hpp"
#endif