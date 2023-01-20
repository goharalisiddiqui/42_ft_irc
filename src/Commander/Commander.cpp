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

        std::string command = parser.getInput().getCommand();
        std::cout << command << std::endl;
        if (command == "USER")
        {
            if (caller.isauthentic())
            {
                caller.respond(":AFGchat 462 NOTICE Auth :You may not reregister\n");
                return ;
            }
            user_info = parser.getUserInfo(input);
            if (user_info.size() == 4)
                this->commandUSER(clients, caller, user_info.at(0), user_info.at(1), user_info.at(2), user_info.at(3));
            else
              caller.respond(":AFGchat 461 NOTICE Auth :Unable to authenticate user\n");

        }
        else if (command == "NICK")
        {
            user_nick = parser.getUserNick(input);
            if (user_nick != "")
                this->commandNICK(clients, caller, user_nick);
            else
              caller.respond(":AFGchat 432 NOTICE Auth :Unable to authenticate nick\n");
        }
        else if (command == "PASS")
        {
            if (caller.isauthentic())
            {
                caller.respond(":AFGchat 462 NOTICE Auth :You may not reregister\n");
                return ;
            }
            std::string user_pass = parser.parseToken(" ", 1);
            // printf("Input=%s\n", input.c_str());
            // printf("ENTERED pass=%s\n", user_pass.c_str());

            this->commandPASS(caller, user_pass);
        }
        if (!caller.isauthentic())
            return;
        if (command == "PING")
        {
            caller.respond(":AFGchat PONG :AFGchat\n");
        }
        if (command == "TOPIC")
        {
            std::string                 new_topic = parser.getInput().getMessage();
            std::vector<std::string>    channel_name = parser.getInput().getTargets();

            this->commandTOPIC(caller, channel_name, new_topic);
        }
        else if (command == "PRIVMSG")
        {
            //if(*(parser.parseToken(" ", 1).begin()) == '#')
            std::vector<std::string> targets;
            targets = parser.getInput().getTargets();
            if (targets.size() == 0)
            {
                caller.respond(":AFGchat 411 NOTICE Auth :No recipient given (PRIVMSG)\n");
                return ;
            }
            for (int i = 0; i < targets.size(); i++)
            {
                if(targets[i][0] == '#')
                    this->commandChannelMessage(clients, caller, targets[i], parser.parseMessage());
                else
                    this->commandPRIVMSG(clients, caller, targets[i], parser.parseMessage());
            }
        }
        else if (command == "JOIN")
        {
            std::vector<std::string> delims;
            delims.push_back(" ");
            delims.push_back(",");
            std::vector<std::string> channel_name = parser.parseListToken(delims, 1);

            for(std::vector<std::string>::iterator it = channel_name.begin(); it != channel_name.end(); ++it)
            {
                this->commandJOIN(caller, *it);
            }
        }
        else if (command == "MODE")
        {
            std::string channel_name = parser.parseToken(" ", 1);
            std::string modes = parser.parseToken(" ", 2);
            std::string user = parser.parseToken(" ", 3);;

            this->commandMODE(caller, channel_name, modes, user);
        }
        else if (command == "INVITE")
        {
            std::string nick = parser.parseToken(" ", 1);
            std::string channel_name = parser.parseToken(" ", 2);

            this->commandINVITE(clients, caller, channel_name, nick);
        }
        else if (command == "KICK")
        {
            std::vector<std::string> delims;
            delims.push_back(" ");
            delims.push_back(",");
            std::vector<std::string> channel_names = parser.parseListToken(delims, 1);
            std::vector<std::string> user_names = parser.parseListToken(delims, 2);
            std::string comment = parser.parseToken(" ", 3);
            if (comment == "")
                comment = caller.get_nick(); //no :!
            if (channel_names.at(0) == "" || user_names.at(0) == "")
                caller.respond(":" SERVER_NAME " " ERR_NEEDMOREPARAMS + command + " :Not enough parameters" + MSG_END_SEQ);
            else
                commandKICK(caller, user_names, channel_names, comment);
        }
        else if (command == "PART")
        {
            std::vector<std::string> channel_names;
            channel_names = parser.getInput().getTargets();

            this->commandPART(caller, channel_names);
        }
        else if (command == "QUIT")
        {
            this->commandQUIT(caller);
        }
        else if (command == "LIST")
        {
            caller.respond(":AFGchat 321 " + caller.get_nick() + " Channel :Users Name (START of LIST)\n");
            for (std::list<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
            {
                caller.respond(":AFGchat 322 " + caller.get_nick() + " " + it->getName() + " " + std::to_string(it->getUsers().size()) + " :[" + it->getModes() + "] " + it->getTopic() + "\n");
            }
            caller.respond(":AFGchat 323 " + caller.get_nick() + " :END of LIST\n");
        }
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

    // only for Debugging
    void Commander::printChannels()
    {
        std::set<Client*> chan_users;
        std::cout << "START printing channels" << std::endl;
        for(std::list<Channel>::iterator it = this->channels.begin(); it != this->channels.end(); ++it)
        {
            std::cout << "Channel = " << it->getName() << " of size = " << it->getUsers().size() << " :" << std::endl;
            chan_users = it->getUsers();
            for(std::set<Client *>::const_iterator it2 = chan_users.begin(); it2 != chan_users.end(); ++it2)
            {
                std::cout << (*it2)->get_nick() << std::endl;
            }
            std::cout << std::endl;
        }
        std::cout << "END printing channels" << std::endl;
    }
}
