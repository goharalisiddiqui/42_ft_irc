#include "../include/Commander.hpp"
#include "../include/RequestParser.hpp"


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
        AFG::RequestParser  parser(input);

        std::cout << parser.getStringFromClient().getCommand() << std::endl;



        if (input == "USER user host server :real")
        {
            this->commandUSER(clients, caller, "user","host","server","real");
        }
        
    }

    void Commander::commandUSER(std::list<Client> &clients, Client &caller, std::string username, std::string hostname, std::string servername, std::string realname)
    {
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


}
