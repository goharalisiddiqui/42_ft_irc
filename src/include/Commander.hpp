#ifndef COMMANDER_HPP
#define COMMANDER_HPP

#pragma once

#include "libs.hpp"
#include "Client.hpp"
#include "ServerSocket.hpp"
#include "Channel.hpp"

#define MAX_MSG_LENGTH 512
#define MAX_CHANNELS 10

#define ERR_NOSUCHNICK "401"
#define ERR_NOSUCHCHANNEL "403"
#define ERR_NOTONCHANNEL "422"
#define ERR_NEEDMOREPARAMS "461"
#define ERR_INVITEONLYCHAN "473" 
#define ERR_CHANOPRIVSNEEDED "482"
#define ERR_NOSUCHSERVER "402"


#define RPL_NOTOPIC "331"
#define RPL_TOPIC "332"
#define RPL_NAMREPLY "353"
#define RPL_ENDOFNAMES "366"


class Channel;

namespace AFG
{
    class Commander
    {
        public:
            Commander(std::string const &pass);
            ~Commander();
            // Commander(const Commander &src);

            Commander &operator=(const Commander &src);
            void process(std::list<Client> &clients, Client &caller);
 
            void cQUIT_purgeClient(Client &caller);

        private:
            Commander();
            std::string pass;
            std::list<Channel> channels;

            void commandChannelMessage(std::list<Client> &clients, Client &caller, std::string channel, std::string msg);
            void commandPRIVMSG(std::list<Client> &clients, Client &caller, std::string othername, std::string msg, std::string command);
            void commandUSER(std::list<Client> &clients, Client &caller, std::string username, std::string hostname, std::string servername, std::string realname);
            void commandNICK(std::list<Client> &clients, Client &caller, std::string nick);
            void commandPASS(Client &caller, std::string pass);
            void commandTOPIC(Client &caller, std::vector<std::string> channel_name, std::string new_topic);
            void commandJOIN(Client &caller, std::string &channelName);
            void commandMODE(Client &caller, std::string channel_name, std::string modes, std::string user);
            void commandINVITE(std::list<Client> &clients, Client &caller, std::string channel_name, std::string nick);
            void commandKICK(Client &caller, std::vector<std::string> &usernames, std::vector<std::string> &channelnames, std::string &comment);
            void commandPART(Client &caller, std::vector<std::string> &channel_names);
            void commandQUIT(Client &caller);



            bool usernameTaken(std::string username, std::list<Client> &clients) const;
            bool nickTaken(std::string nick, std::list<Client> &clients) const;
            bool channelExists(std::string channelName) const;
        public:



    };
}

#endif