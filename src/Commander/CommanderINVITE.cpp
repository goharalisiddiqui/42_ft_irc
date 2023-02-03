#include "../include/Commander.hpp"

namespace AFG
{
    void Commander::commandINVITE(std::list<Client> &clients, Client &caller, std::string channel_name, std::string nick)
    {
        if (nickTaken(nick, clients) == false)
        {
            caller.respond(":AFGchat 401 NOTICE " + nick + " :No such nick\n"); // correct format for weechat?
            return ;
        }
        if (channel_name == "")
        {
            caller.respond(":AFGchat 461 NOTICE INVITE :Not enough parameters\n"); // correct format for weechat?
            return ;
        }
        if (channelExists(channel_name) == false)
        {
            caller.respond(":AFGchat 341 NOTICE " + channel_name + " " + nick + " :got invited\n"); // correct format for weechat?
            return ;
        }
        std::list<Channel>::iterator it;
        for(it = this->channels.begin(); it != this->channels.end(); ++it)
        {
            if (it->getName() == channel_name) //if channel looking for is in lst
                break ;
        }
        if (it->isInviteOnly() == true)
        {
            if (it->isOperator(caller) == false)
            {
                caller.respond(":AFGchat 482 NOTICE " + channel_name + " :You're not channel operator\n"); // correct format for weechat?
                return ;
            }
        }
        std::list<Client>::iterator it_client;
        for(it_client = clients.begin(); it_client != clients.end(); ++it_client)
        {
            if (it_client->get_nick() == nick)
            {
                it_client->respond(":" + caller.get_nick() + "!" + caller.get_username() + ("@"));
                it_client->respond(caller.get_hostname() + " INVITE " + nick + " :" + channel_name + "\n");
                caller.respond(":AFGchat 341 " + caller.get_nick() + " " + nick + " " + channel_name + "\n");
                it->inviteUser(caller, *it_client);
            }
        }
    }
}
