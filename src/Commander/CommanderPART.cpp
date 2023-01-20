#include "../include/Commander.hpp"

namespace AFG
{
    void cPART_reponseNotifyChannelOfPart(Client &caller, Channel &channel) // Notify channel of the quited user
    {
        channel.announce(":" + caller.get_nick() + "!" + caller.get_username() + "@" + caller.get_hostname() + " PART " + channel.getName() + " : " + MSG_END_SEQ);
    }

    void Commander::commandPART(Client &caller, std::vector<std::string> &channel_names)
    {
        if (channel_names.size() < 1)
        {
            caller.respond(":AFGchat 461 NOTICE Auth :PART :Not enough parameters\n");
            return ;
        }
        for (int i = 0; i < channel_names.size(); i++)
        {
            if(!this->channelExists(channel_names[i]))
                caller.respond(":AFGchat 461 NOTICE Auth : " + channel_names[i] + " :No such channel\n");
            else
            {
                std::list<AFG::Channel>::iterator it;
                for (it = this->channels.begin(); it != this->channels.end(); it++)
                {
                    if (it->getName() == channel_names[i])
                    {
                        if (it->hasUser(caller))
                        {
                            cPART_reponseNotifyChannelOfPart(caller, *it);
                            it->removeUser(caller);
                        }
                        else
                            caller.respond(":AFGchat 442 NOTICE Auth : " + channel_names[i] + " :You're not on that channel\n");
                    }
                }
            }
        }
    }
}
