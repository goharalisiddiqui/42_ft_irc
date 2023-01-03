#include "../include/Commander.hpp"

namespace AFG
{
    void Commander::commandJOIN(Client &caller, std::string &channelName)
    {
        if ((channelName.at(0) != '#') || (channelName.find_first_of(":") != channelName.npos))
        {
            //caller.respond("Wrong Channel name\n");
            caller.respond(":AFGchat 403 " + caller.get_nick() + " " + channelName + " :Invalid channel!\n");
            return;
        }
        if (!channelExists(channelName))
        {
            channels.push_back(Channel(channelName));
            channels.back().addOperator(caller);
        }
        addUserToChannel(caller, channelName);
        printChannels();
        return;
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
                    user.respond(":AFGchat 473 ");
                    user.respond(user.get_nick());
                    user.respond(" :Uninvited users connot join invite-only channels\n");
                }
                return;
            }
        }
    }
}
