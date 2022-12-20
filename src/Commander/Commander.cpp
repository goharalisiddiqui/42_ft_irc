#include "../include/Commander.hpp"
#include "../include/RequestParser.hpp"


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
        else if (command == "NICK")
        {
            user_nick = parser.getUserNick(input);
            if (user_nick != "")
                this->commandNICK(clients, caller, user_nick);
        }
        else if (command == "PASS")
        {
            pass = parser.getUserNick(input);
            this->commandPASS(caller, pass);
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
