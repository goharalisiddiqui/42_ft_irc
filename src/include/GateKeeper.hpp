#ifndef GATEKEEPER_HPP
#define GATEKEEPER_HPP

#pragma once

#include "libs.hpp"
#include "Client.hpp"
#include "ServerSocket.hpp"
#include "Commander.hpp"

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
            GateKeeper(in_port_t port);


            std::list<Client> &get_clients(void);

            void watchover();
            void announce(std::string const &msg) const;
            void spreadmsgfrom(Client *speaker) const;

            void serve(void);

        private:
            int fdmax;
            int conmax;
            std::list<Client> clients;
            ServerSocket sock;
            Commander fredi;

            void refuse_client(int fd);
            void add_client();
            void remove_client(int fd);

            void setnonblock(int fd);

            fd_set build_selist(void);
            void garbage_collector(void);

            bool nottaken(std::string username) const;



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