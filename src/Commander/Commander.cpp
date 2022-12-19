#include "../include/Commander.hpp"


namespace AFG
{
    Commander::Commander()
    {
    }
    Commander::~Commander()
    {
    }
    void Commander::process(std::list<Client> &clients, Client &caller)
    {
        std::string input = caller.get_message();


        if (input == "USER user1 host server :real\n")
        {
            this->commandUSER(clients, caller, "user1","host","server","real");
        }
        if (input == "USER user2 host server :real\n")
        {
            this->commandUSER(clients, caller, "user2","host","server","real");
        }
        if (input == "NICK nick\n")
        {
            this->commandNICK(clients, caller, "nick");
        }
    }

    void Commander::commandUSER(std::list<Client> &clients, Client &caller, std::string username, std::string hostname, std::string servername, std::string realname)
    {
        if (this->usernameTaken(username, clients))
        {
            caller.respond("Username already taken. please use another\n");
            return;
        }
        caller.set_username(username);
        caller.set_hostname(hostname);
        caller.set_servername(servername);
        caller.set_realname(realname);
        if (caller.get_nick().length() != 0)
            caller.authenticate();
    }

    void Commander::commandNICK(std::list<Client> &clients, Client &caller, std::string nick)
    {
        caller.set_nick(nick);
        if (caller.get_username().length() != 0
            && caller.get_hostname().length() != 0
            && caller.get_servername().length() != 0
            && caller.get_realname().length() != 0)
            caller.authenticate();
    }

    bool Commander::usernameTaken(std::string username, std::list<Client> &clients) const
    {
        for(std::list<AFG::Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
        {
            if (it->get_username() == username)//.substr(0,username.length() - 1))
                return true;
        }  
        return false;
    }


}
