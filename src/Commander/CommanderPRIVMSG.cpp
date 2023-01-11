#include "../include/Commander.hpp"

namespace AFG
{
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
                {
                    std::cout << "no user\n"; //return;
                    return;
                }
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
        caller.respond(":AFGchat 403 ");
        caller.respond(caller.get_nick());
        caller.respond((" ") + channel + (" :No such channel!\n"));
    }
}
