#ifndef COMMANDER_HPP
#define COMMANDER_HPP

#pragma once

#include "libs.hpp"
#include "Client.hpp"
#include "ServerSocket.hpp"

namespace AFG
{
    class Commander
    {
        public:
            Commander();
            ~Commander();
            // Commander(const Commander &src);

            // Commander &operator=(const Commander &src);
            void process(std::list<Client> &clients, Client &caller);
 

        private:
            void commandUSER(std::list<Client> &clients, Client &caller, std::string username, std::string hostname, std::string servername, std::string realname);
            void commandNICK(std::list<Client> &clients, Client &caller, std::string nick);




        public:



    };
}

#endif