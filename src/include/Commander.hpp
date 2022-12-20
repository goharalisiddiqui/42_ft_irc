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
            Commander(std::string const &pass);
            ~Commander();
            // Commander(const Commander &src);

            // Commander &operator=(const Commander &src);
            void process(std::list<Client> &clients, Client &caller);
 

        private:
            Commander();
            std::string pass;

            void commandPRIVMSG(std::list<Client> &clients, Client &caller, std::string othername, std::string msg);
            void commandUSER(std::list<Client> &clients, Client &caller, std::string username, std::string hostname, std::string servername, std::string realname);
            void commandNICK(std::list<Client> &clients, Client &caller, std::string nick);
            void commandPASS(Client &caller, std::string pass);



            bool usernameTaken(std::string username, std::list<Client> &clients) const;
            bool nickTaken(std::string nick, std::list<Client> &clients) const;
        public:



    };
}

#endif