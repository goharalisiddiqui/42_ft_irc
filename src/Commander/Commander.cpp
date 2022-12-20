#include "../include/Commander.hpp"


namespace AFG
{
    Commander::Commander()
    {
    }
    Commander::Commander(std::string const &pass): pass(pass)
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
        else if (input == "USER user2 host server :real\n")
        {
            this->commandUSER(clients, caller, "user2","host","server","real");
        }
        else if (input == "NICK nick\n")
        {
            this->commandNICK(clients, caller, "nick");
        }
        else if (input == "PASS pass\n")
        {
            this->commandPASS(caller, "pass");
        }
        else if (!caller.isauthentic())
            caller.respond("Command not known.\n");

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
        caller.respond("Username identified.\n");

        caller.authenticate();
    }

    void Commander::commandNICK(std::list<Client> &clients, Client &caller, std::string nick)
    {
        caller.set_nick(nick);
        caller.respond("Nick set.\n");
        
        caller.authenticate();
    }

    void Commander::commandPASS(Client &caller, std::string pass)
    {
        printf("Correct pass=%s\n", this->pass.c_str());
        if (pass != this->pass)
        {
            caller.respond("Wrong password!\n");
            return;
        }
        caller.set_passed();
        caller.respond("Password accepted.\n");

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
