#include "../include/Commander.hpp"

namespace AFG
{
    /* does two things: either - if a topic is given - setting a new topic or sending the current topic */
    void    Commander::commandTOPIC(Client &caller, std::vector<std::string> channel_name, std::string new_topic)
    {
        if (channel_name.size() != 1)
        {
            caller.respond(":AFGchat 407 NOTICE :TOPIC needs one and only one channel.\n"); // correct format for weechat?
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
                    caller.respond(":AFGchat 482 NOTICE :channel is in -t mode. only operators can change the topic\n"); // correct format for weechat?
                    return ;
                }
            }
            it->setTopic(new_topic);
        }
        else if (this->channelExists(channel_name.at(0)))
            caller.respond(":AFGchat 332 NOTICE " + channel_name.at(0) + " :" + it->getTopic() + "\n"); // correct format for weechat?
    }
}
