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
            std::string user_pass = parser.parseToken(" ", 1);
            this->commandPASS(caller, user_pass);
        }
        if (!caller.isauthentic())
            return;
        if (command == "PING")
        {
            caller.respond(":AFGchat PONG :AFGchat\n");
        }
        else if (command == "PRIVMSG")
        {
            this->commandPRIVMSG(clients, caller, parser.parseToken(" ", 1), parser.parseToken(":", 1));
        }
    }
    
    void Commander::commandPRIVMSG(std::list<Client> &clients, Client &caller, std::string othername, std::string msg)
    {
        for(std::list<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
        {
            std::cout << it->get_nick() << std::endl;
            if (it->get_nick() == othername)
            {
                if (caller.get_nick() == othername)
                {
                  caller.respond(":AFGchat 401 ");
                  caller.respond(caller.get_nick());
                  caller.respond((" ") + othername + (" :Cannot write to yourself!\n"));
                    return;
                }
                it->respond(":" + caller.get_nick() + "!" + caller.get_username() + ("@"));
                it->respond(caller.get_hostname() + " PRIVMSG " + othername + " :" + msg + "\n");
                return;
            }
        }
            //else if nick/othername does not exist
            caller.respond(":AFGchat 401 ");
            caller.respond(caller.get_nick());
            caller.respond((" ") + othername + (" :No such nick!\n"));
    }

    void Commander::commandUSER(std::list<Client> &clients, Client &caller, std::string username, std::string hostname, std::string servername, std::string realname)
    {
        if (this->usernameTaken(username, clients))
        {
            caller.respond(":AFGchat NOTICE Auth :Username already taken. please use another\n");
            return;
        }
        caller.set_username(username);
        caller.set_hostname(hostname);
        caller.set_servername(servername);
        caller.set_realname(realname);
        caller.respond(":AFGchat NOTICE Auth :Username identified.\n");

        caller.authenticate();
    }

    void Commander::commandNICK(std::list<Client> &clients, Client &caller, std::string nick)
    {
        if (this->nickTaken(nick, clients))
        {
            caller.respond(":AFGchat NOTICE Auth :Nick already taken. please use another\n");
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


    void Commander::commandJOIN(Client &caller, std::string &channelName)
    {
        if (channelName.at(0) != '#')
        {
            caller.respond("Wrong Channel name");
            return;
        }
        if (!channelExists(channelName))
            channels.push_back(Channel(channelName));
        addUserToChannel(caller, channelName);



    }





    void Commander::commandPASS(Client &caller, std::string pass)
    {
        printf("Correct pass=%s\n", this->pass.c_str());
        // printf("ENTERED pass=%s\n", pass.c_str());
        if (pass != this->pass)
        {
            caller.respond(":AFGchat NOTICE Auth :Wrong password!\n");
            return;
        }
        caller.set_passed();
        caller.respond(":AFGchat NOTICE Auth :Password accepted.\n");

        caller.authenticate();
    }

    bool Commander::usernameTaken(std::string username, std::list<Client> &clients) const
    {
        for(std::list<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
        {
            if (it->get_username() == username)
                return true;
        }  
        return false;
    }

    bool Commander::nickTaken(std::string nick, std::list<Client> &clients) const
    {
        for(std::list<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
        {
            if (it->get_nick() == nick)
                return true;
        }  
        return false;
    }

    bool Commander::channelExists(std::string channelName) const
    {
        for(std::list<Channel>::const_iterator it = this->channels.begin(); it != this->channels.end(); ++it)
        {
            if (it->getName() == channelName)
                return true;
        }  
        return false;
    }

    void Commander::addUserToChannel(Client &user, std::string &channelName)
    {
        for(std::list<Channel>::iterator it = this->channels.begin(); it != this->channels.end(); ++it)
        {
            if (it->getName() == channelName)
                it->addUser(user);
        }
    }


}
