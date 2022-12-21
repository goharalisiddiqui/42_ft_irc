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
        if (command == "TOPIC")
        {
            std::string                 new_topic = parser.getInput().getMessage();
            std::vector<std::string>    channel_name = parser.getInput().getTargets();

            this->commandTOPIC(caller, channel_name, new_topic);
        }
        else if (command == "PRIVMSG")
        {
            //if(*(parser.parseToken(" ", 1).begin()) == '#')
            if(parser.parseToken(" ", 1)[0] == '#')
                this->commandChannelMessage(clients, caller, parser.parseToken(" ", 1), parser.parseToken(":", 1));
            else
            this->commandPRIVMSG(clients, caller, parser.parseToken(" ", 1), parser.parseToken(":", 1));
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
        
    }
//    void Commander::commandChannelMessage(std::list<Client> &clients, Client &caller, Channel &channel, std::string msg)
    void Commander::commandChannelMessage(std::list<Client> &clients, Client &caller, std::string channel, std::string msg)
    {
         //std::cout << "Channel" << std::endl;
        for(std::list<Channel>::const_iterator it = this->channels.begin(); it != this->channels.end(); ++it)
        {
            //std::cout << it->getName() << " listitem | channel: "<< channel << "!" <<std::endl;
            if (it->getName() == channel) //if channel looking for is channel in lst
            {
                if(it->hasUser(caller) == false) //geht das?
                    std::cout << "no user\n"; //return;
                std::set<Client*> users = it->getUsers();
                for(std::set<Client*>::const_iterator jt = users.begin(); jt != users.end(); ++jt)
                {
                    //std::cout << (*jt)->get_nick() << "=Nick now| NICK caller" << caller.get_nick() << std::endl;
                    if ((*jt)->get_nick() == caller.get_nick()) // dont send message to yourself
                        continue;
                    (*jt)->respond(":" + caller.get_nick() + "!" + caller.get_username() + ("@"));
                    (*jt)->respond(caller.get_hostname() + " PRIVMSG " + channel + " :" + msg + "\n");
                }
                return;
            }
        }
        //channel not found
        caller.respond(":AFGchat 401 ");
        caller.respond(caller.get_nick());
        caller.respond((" ") + channel + (" :No such channel!\n"));
    }

    void Commander::commandPRIVMSG(std::list<Client> &clients, Client &caller, std::string othername, std::string msg)
    {
        for(std::list<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
        {
            //std::cout << it->get_nick() << std::endl;
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
            //caller.respond("Wrong Channel name\n");
            caller.respond(":AFGchat 403 " + caller.get_nick() + " " + channelName + " :Invalid channel!\n");
            return;
        }
        if (!channelExists(channelName))
            channels.push_back(Channel(channelName));
        addUserToChannel(caller, channelName);
        printChannels();
        return;
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
    /* work in progress: pseudo code since channel class is not fully done yet */
    void    Commander::commandTOPIC(Client &caller, std::vector<std::string> channel_name, std::string new_topic)
    {
        if (channel_name.size() != 1)
        {
            std::cerr << "Error: one and only one channel." << std::endl;
            return ;
        }
        std::list<Channel>::iterator it;
        for(it = this->channels.begin(); it != this->channels.end(); ++it)
        {
            if (it->getName() == channel_name.at(0)) //if channel looking for is channel in lst
                break ;
        }
        if (new_topic != "")
        {
            if (it->isTopicOpOnly() == true)
            {
                if (it->isOperator(caller) == false)
                {
                    std::cerr << "Can't change topic: channel is in -t mode: only operators can change the topic" << std::endl;
                    return ;
                }
            }
            it->setTopic(new_topic);
        }
        else if (this->channelExists(channel_name.at(0)))
            caller.respond(":AFGchat NOTICE " + it->getTopic() + "\n"); // correct format for weechat?
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
            {
                if (it->isInvited(user) || !it->isInviteOnly())
                {
                    it->addUser(user);
                    user.respond(":");
                    user.respond(user.get_nick());
                    user.respond("!");
                    user.respond(user.get_username());
                    user.respond("@");
                    user.respond(user.get_hostname());
                    user.respond(" JOIN :");
                    user.respond(channelName);
                    user.respond("\n");
                    user.respond(":AFGchat 353 " + user.get_nick() + " = " + channelName + " :Joined!\n");
                }
                else
                {
                    user.respond(":AFGchat 401 ");
                    user.respond(user.get_nick());
                    user.respond(" :Uninvited users connot join invite-only channels\n");
                }
                return;
            }
        }
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
