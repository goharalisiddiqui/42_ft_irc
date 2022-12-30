#include "../include/Commander.hpp"

namespace AFG
{
    /* changes channel modes, gives/removes operator privileges.
    before that: checks if channel exists, if user is operator, if modes exist */
    void Commander::commandMODE(Client &caller, std::string channel_name, std::string modes, std::string user)
    {
        if (this->channelExists(channel_name) == false)
        {
            caller.respond(":AFGchat 403 NOTICE " + channel_name + " :" + "No such channel\n"); // correct format for weechat?
            return ;
        }
        std::list<Channel>::iterator it;
        for(it = this->channels.begin(); it != this->channels.end(); ++it)
        {
            if (it->getName() == channel_name) //if channel looking for is channel in lst
                break ;
        }
        if (it->isOperator(caller) == false)
        {
            caller.respond(":AFGchat 482 NOTICE " + channel_name + " :You're not channel operator\n"); // correct format for weechat?
            return ;
        }
        if ((modes.at(0) != '-' && modes.at(0) != '+') \
            || modes.find_first_not_of("iot", 1) != modes.npos)
        {
            caller.respond(":AFGchat 472 NOTICE " + modes + " :is unknown mode char to me\n"); // correct format for weechat?
            return ;
        }
        // to do: inform client about changes
        if (modes.find_first_of("i") != modes.npos)
        {
            if (modes.at(0) == '+')
			{
                caller.respond(":AFGchat 324 NOTICE " + channel_name + " :invite-only ON\n"); // correct format for weechat?
                it->makeInviteOnly();
			}
            else
			{
                caller.respond(":AFGchat 324 NOTICE " + channel_name + " :invite-only OFF\n"); // correct format for weechat?
                it->removeInviteOnly();
			}
        }
        if (modes.find_first_of("t") != modes.npos)
        {
            if (modes.at(0) == '+')
			{
                caller.respond(":AFGchat 324 NOTICE " + channel_name + " :topic operater only ON\n"); // correct format for weechat?
                it->makeTopicOpOnly();
			}
            else
			{
                caller.respond(":AFGchat 324 NOTICE " + channel_name + " :topic operater only OFF\n"); // correct format for weechat?
                it->removeTopicOpOnly();
			}
        }
        if (modes.find_first_of("o") != modes.npos)
        {
            if (user == "")
            {
                caller.respond(":AFGchat 431 NOTICE " + user + " :No nickname given\n"); // correct format for weechat?
                return ;
            }
            else
            {
                std::set<Client*>::const_iterator it_client;
                for(it_client = it->getUsers().begin(); it_client != it->getUsers().end(); ++it_client)
                {
                    if ((*it_client)->get_nick() == user) //if user looking for is user in channel
                        break ;
                }
                std::cout << (*it_client)->get_nick() << std::endl;

                if (it->hasUser(*(*it_client)) == false)
                {
                    caller.respond(":AFGchat 401 NOTICE " + user + " :No such nick\n"); // correct format for weechat?
                    return ;
                }
                if (modes.at(0) == '+')
				{
	                caller.respond(":AFGchat 324 NOTICE " + channel_name + " :" + (*it_client)->get_nick() + " operator privileges ON\n"); // correct format for weechat?
                    it->addOperator(*(*it_client));
				}
                else
				{
	                caller.respond(":AFGchat 324 NOTICE " + channel_name + " :" + (*it_client)->get_nick() + " operator privileges OFF\n"); // correct format for weechat?
                    it->removeOperator(*(*it_client));
				}
            }
        }
    }
}
