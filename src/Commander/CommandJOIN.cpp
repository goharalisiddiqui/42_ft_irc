#include "../include/Commander.hpp"
#include "../include/RequestParser.hpp"


namespace AFG
{
    void cJOIN_reponseInvalidChannel(Client &caller, std::string &channelname)  // When channel name is invalid
    {
        caller.respond(":" SERVER_NAME " " ERR_NOSUCHCHANNEL " " + caller.get_nick() + " " +  channelname + " :Invalid channel!" MSG_END_SEQ);
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
            caller.respond(":" SERVER_NAME " " RPL_NOTOPIC " " + caller.get_nick() + " " + channel.getName() + " :" + channel.getTopic() + MSG_END_SEQ);
    }

    void cJOIN_reponseSendNewcomerNamelist(Client &caller, Channel &channel) // Notify channel of the joined user
    {
        
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

        if ((channelName.at(0) != '#') || (channelName.find_first_of(":") != channelName.npos))
        {
            cJOIN_reponseInvalidChannel(caller, channelName);   
            return;
        }
        ch = cJOIN_getChannel(this->channels, channelName);
        if (!ch)
        {
            channels.push_back(Channel(channelName));
            ch = &(channels.back());
        }
        if (ch->isInviteOnly() && !ch->isInvited(caller))
        {
            cJOIN_reponseNotInvited(caller, *ch);
            return;
        }
        ch->addUser(caller);

        cJOIN_reponseNotifyChannelOfJoin(caller, *ch);
        cJOIN_reponseSendNewcomerTopic(caller, *ch);
        cJOIN_reponseSendNewcomerNamelist(caller, *ch);
        // printChannels();
    }



}



/*




JOIN #channel
            │21:09:39 --> gnirc | :momo!~momo@localhost JOIN :#channel
            │21:09:39 --> gnirc | :irc.example.net 353 momo = #channel :momo @gohar
            │21:09:39 --> gnirc | :irc.example.net 366 momo #channel :End of NAMES list
            │21:09:41 <-- gnirc | MODE #channel
            │21:09:41 --> gnirc | :irc.example.net 324 momo #channel +
            │21:09:41 --> gnirc | :irc.example.net 329 momo #channel 1671825638
            │21:10:05 <-- gnirc | PING 127.0.0.1
            │21:10:05 --> gnirc | :irc.example.net PONG irc.example.net :127.0.0.1
*/