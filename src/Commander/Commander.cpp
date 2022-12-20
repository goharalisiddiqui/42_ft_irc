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

        /* parse user info */
        AFG::RequestParser          parser(input);
        std::vector<std::string>    user_info;
        std::string                 user_nick;

        std::string command = parser.getStringFromClient().getCommand();
        std::cout << command << std::endl;
        if (command == "USER")
        {
            user_info = parser.getUserInfo(input);
            if (user_info.size() == 4)
                this->commandUSER(clients, caller, user_info.at(0), user_info.at(1), user_info.at(2), user_info.at(3));
        }
        if (command == "NICK")
        {
            user_nick = parser.getUserNick(input);
            if (user_nick != "")
                this->commandNICK(clients, caller, user_nick);
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
