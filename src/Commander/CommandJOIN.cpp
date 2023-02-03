#include "../include/Commander.hpp"
#include "../include/RequestParser.hpp"


namespace AFG
{
    void cJOIN_reponseInvalidChannel(Client &caller, std::string &channelname)  // When channel name is invalid
    {
        caller.respond(":" SERVER_NAME " " ERR_NOSUCHCHANNEL " " + caller.get_nick() + " " +  channelname + " :Invalid channel!" MSG_END_SEQ);
    }

    void cJOIN_reponseChannelsFull(Client &caller, std::string &channelname)  // When channels list is full
    {
        caller.respond(":" SERVER_NAME " " ERR_NOSUCHCHANNEL " " + caller.get_nick() + " " +  channelname + " :Channel list is full!" MSG_END_SEQ);
    }

    void cJOIN_reponseNotInvited(Client &caller, Channel &channel) // When user not invited to a invite-only channel
    {
        caller.respond(":" SERVER_NAME " " ERR_INVITEONLYCHAN " " + caller.get_nick() + " " + channel.getName() + " :Cannot join channel (+i)" + MSG_END_SEQ);
    }

    void cJOIN_reponseNotifyChannelOfJoin(Client &caller, Channel &channel) // Notify channel of the joined user
    {
        channel.announce(":" + caller.get_nick() + "!" + caller.get_username() + "@" + caller.get_hostname() + " JOIN :" + channel.getName() + MSG_END_SEQ);
    }

    void cJOIN_reponseSendNewcomerTopic(Client &caller, Channel &channel) // Notify channel of the joined user
    {
        if (channel.getTopic() == "")
            caller.respond(":" SERVER_NAME " " RPL_NOTOPIC " " + caller.get_nick() + " " + channel.getName() + " :No topic is set" + MSG_END_SEQ);
        else
            caller.respond(":" SERVER_NAME " " RPL_TOPIC " " + caller.get_nick() + " " + channel.getName() + " :" + channel.getTopic() + MSG_END_SEQ);
    }

    void cJOIN_reponseSendNewcomerNamelist(Client &caller, Channel &channel) // Notify channel of the joined user
    {
        std::string prefix = ":" SERVER_NAME " " RPL_NAMREPLY " " + caller.get_nick() + " = " + channel.getName() + " :";
        int length  = prefix.length();
        std::set<Client *> users = channel.getUsers();
        std::string reply = prefix;

        for(std::set<Client *>::const_iterator it = users.begin(); it != users.end(); ++it)
        {
            if (reply.length() + 1 + (*it)->get_nick().length() > 510)
            {
                caller.respond(reply + MSG_END_SEQ);
                reply = prefix;
            }
            if (reply != prefix)
                reply.append(" ");
            if (channel.isOperator(*(*it)))
                reply.append("@");
            reply.append((*it)->get_nick());
        }
        if (reply != prefix)
            caller.respond(reply + MSG_END_SEQ);

        caller.respond(":" SERVER_NAME " " RPL_ENDOFNAMES " " + caller.get_nick() + " " + channel.getName() + " :End of NAMES list" MSG_END_SEQ);

    }




    Channel *cJOIN_getChannel(std::list<Channel> &channels, std::string &channelname)
    {
        for(std::list<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
        {
            if (it->getName() == channelname)
                return &(*it);
        }  
        return nullptr;
    }

    void Commander::commandJOIN(Client &caller, std::string &channelName)
    {
        Channel *ch;

        if ((channelName.size() < 2) || (channelName.at(0) != '#') \
            || (channelName.find_first_of(":") != channelName.npos))
        {
            cJOIN_reponseInvalidChannel(caller, channelName);   
            return;
        }
        ch = cJOIN_getChannel(this->channels, channelName);
        if (!ch)
        {
            if (this->channels.size() == MAX_CHANNELS)
            {
                cJOIN_reponseChannelsFull(caller, channelName);
                return;
            }
            else
            {
                channels.push_back(Channel(channelName));
                ch = &(channels.back());
                ch->addOperator(caller);
            }
        }
        if (ch->hasUser(caller)) // When caller is already in the channel
            return;
        if (ch->isInviteOnly() && !ch->isInvited(caller))
        {
            cJOIN_reponseNotInvited(caller, *ch);
            return;
        }
        ch->addUser(caller);

        cJOIN_reponseNotifyChannelOfJoin(caller, *ch);
        cJOIN_reponseSendNewcomerTopic(caller, *ch);
        cJOIN_reponseSendNewcomerNamelist(caller, *ch);
    }



}
