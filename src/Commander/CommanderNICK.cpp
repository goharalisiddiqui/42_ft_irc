#include "../include/Commander.hpp"

namespace AFG
{
    void Commander::commandNICK(std::list<Client> &clients, Client &caller, std::string nick)
    {
        if (this->nickTaken(nick, clients))
        {
            caller.respond(":AFGchat 433 NOTICE Auth :Nick already taken. please use another\n");
            return;
        }
        if (caller.isauthentic())
        {   
            caller.respond(":");
            caller.respond(caller.get_nick());
            caller.respond("!");
            caller.respond(caller.get_username());
            caller.respond("@");
            caller.respond(caller.get_hostname());
            caller.respond(" NICK ");
            caller.respond(nick);
            caller.respond("\n");
        }
        caller.set_nick(nick);
        caller.authenticate();
    }
}
