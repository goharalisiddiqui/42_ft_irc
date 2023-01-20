#include "../include/Commander.hpp"

namespace AFG
{

    void cQUIT_reponseNotifyChannelOfQuit(Client &caller, Channel &channel) // Notify channel of the quited user
    {
        channel.announce(":" + caller.get_nick() + "!" + caller.get_username() + "@" + caller.get_hostname() + " QUIT :" + " " + MSG_END_SEQ);
    }

    void Commander::commandQUIT(Client &caller)
    {
        std::list<AFG::Channel>::iterator it;
        for (it = this->channels.begin(); it != this->channels.end(); it++)
        {
            if (it->hasUser(caller))
            {
                it->removeUser(caller);
                if (it->isOperator(caller))
                    it->removeOperator(caller);
                cQUIT_reponseNotifyChannelOfQuit(caller, *it);
                break;
            }
        }
        close(caller.get_fd());
        caller.set_garbage();
    }
}
