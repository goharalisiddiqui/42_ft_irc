#include "../include/Commander.hpp"
#include "../include/RequestParser.hpp"


namespace AFG
{
    void cKICK_reponseChannelNotPresent(Client &caller, std::string &channelname)  // When channel does not exist
    {
        caller.respond(":" SERVER_NAME " " ERR_NOSUCHCHANNEL " " + caller.get_nick() + " " +  channelname + " :No such channel" MSG_END_SEQ);
    }

    void cKICK_reponseUserNotPresent(Client &caller, std::string &channelname, std::string &username) // When user not in channel
    {
        caller.respond(":" SERVER_NAME " " ERR_NOSUCHNICK " " + caller.get_nick() + " " + username + " :No such nick" MSG_END_SEQ);
    }

    void cKICK_reponseCallerNotPresent(Client &caller, std::string &channelname) // When caller not in channel
    {
        caller.respond(":" SERVER_NAME " " ERR_NOTONCHANNEL " " + caller.get_nick() + " " + channelname + " :You're not on that channel" MSG_END_SEQ);
    }

    void cKICK_reponseNotOperator(Client &caller, std::string &channelname) // When not operator of the channel 
    {
        caller.respond(":" SERVER_NAME " " ERR_CHANOPRIVSNEEDED " " + caller.get_nick() + " " + channelname + " :You're not channel operator" MSG_END_SEQ);
    }

    void cKICK_reponseNotifyChannelOfRemoval(Client &user, Client &caller, Channel &channel, std::string &comment) // Notify channel of the removal of the user from the channel
    {

        caller.respond(":" + caller.get_nick() + "!" + caller.get_username() + "@" + caller.get_hostname() + " KICK " + channel.getName() + " " + user.get_nick() + " :" + comment + MSG_END_SEQ);
    }

    Channel *cKICK_getChannel(std::list<Channel> &channels, std::string const &channelname)
    {
        for (std::list<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
        {
            if (it->getName() == channelname)
                return &(*it);
        }
        return nullptr;
    }

    Channel *cKICK_getChannelUser(std::list<Channel> &channels, std::string const &channelname)
    {
        for (std::list<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
        {
            if (it->getName() == channelname)
                return &(*it);
        }
        return nullptr;
    }

    void Commander::commandKICK(Client &caller, std::vector<std::string> &usernames, std::vector<std::string> &channelnames, std::string &comment)
    {
        Channel *ch;
        Client *us;

        for (std::vector<std::string>::iterator cit = channelnames.begin(); cit != channelnames.end(); ++cit)
        {
            ch = cKICK_getChannel(this->channels, *cit);
            if (!ch) // When channel does not exist
            {
                cKICK_reponseChannelNotPresent(caller, *cit);
                continue;
            }
            if (!ch->hasUser(caller)) // When not operator of the channel
            {
                cKICK_reponseCallerNotPresent(caller, *cit);
                continue;
            }
            if (!ch->isOperator(caller)) // When not operator of the channel
            {
                cKICK_reponseNotOperator(caller, *cit);
                continue;
            }
            for(std::vector<std::string>::iterator uit = usernames.begin(); uit != usernames.end(); ++uit)
            {
                us = ch->getUser(*uit);
                if (!us) // When user not in channel
                {
                    cKICK_reponseUserNotPresent(caller, *cit, *uit);
                    continue;
                }
                ch->removeUser(*us);
                cKICK_reponseNotifyChannelOfRemoval(*us, caller, *ch, comment); // Notify caller of the removal of the user from the channel
            }
            
        }
    }



}
