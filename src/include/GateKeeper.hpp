#ifndef GATEKEEPER_HPP
#define GATEKEEPER_HPP

#pragma once

#include "libs.hpp"
#include "Connection.hpp"
#include "ServerSocket.hpp"

namespace AFG
{
    class GateKeeper
    {
        public:
            GateKeeper();
            ~GateKeeper();
            // GateKeeper(const GateKeeper &src);

            // GateKeeper &operator=(const GateKeeper &src);

            GateKeeper(int domain, int type, int protocol, int ip, int port, int backlog, int nmax);

            std::list<Connection> &get_connections(void);

            void watchover();

        private:
            int fdmax;
            int conmax;
            std::list<Connection> connections;
            std::list<Connection> active_connections;
            ServerSocket sock;

            void refuse_connection(int fd);
            void addconnection();
            void removeconnection(int fd);

            void setnonblock(int fd);

            fd_set build_selist(void);
            void garbage_collector(void);


        public:
            class CannotSelect: public ::std::exception
            {
                public:
                    virtual const char* what() const throw()
                    {
                        return "Select() failed";
                    }
            };



    };
}

#endif