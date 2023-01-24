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

            GateKeeper &operator=(const GateKeeper &src);

            GateKeeper(int domain, int type, int protocol, int ip, int port, int backlog, int nmax, std::string pass);
            GateKeeper(in_port_t port, std::string pass);


            std::list<Client> &get_clients(void);
            ServerSocket &get_sock(void);

            void watchover();

            void serve(void);

            //temp
            void printClients(void);

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
            void garbage_collector(void);

            fd_set build_selist(void);




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