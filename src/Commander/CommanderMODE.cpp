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
        std::list<Channel>::iterator it_channel;
        for(it_channel = this->channels.begin(); it_channel != this->channels.end(); ++it_channel)
        {
            if (it_channel->getName() == channel_name) //if channel looking for is channel in lst
                break ;
        }
        if (modes == "")
        {
            caller.respond(":AFGchat 324 " + caller.get_nick() + " " + channel_name + " " + it_channel->getModes() + "\n");
            return;
        }
        if (it_channel->isOperator(caller) == false)
        {
            caller.respond(":AFGchat 482 NOTICE " + channel_name + " :You're not channel operator\n"); // correct format for weechat?
            return ;
        }
        if ((modes.at(0) != '-' && modes.at(0) != '+') \
            || modes.find_first_not_of("iot", 1) != modes.npos
            || modes.find_first_of("iot", 1) == modes.npos)
        {
            caller.respond(":AFGchat 472 NOTICE " + modes + " :is unknown mode char to me\n"); // correct format for weechat?
            return ;
        }
        if (modes.find_first_of("i") != modes.npos)
        {
            if (modes.at(0) == '+')
			{
                caller.respond(":AFGchat 324 NOTICE " + channel_name + " :invite-only ON\n"); // correct format for weechat?
                it_channel->makeInviteOnly();
			}
            else
			{
                caller.respond(":AFGchat 324 NOTICE " + channel_name + " :invite-only OFF\n"); // correct format for weechat?
                it_channel->removeInviteOnly();
			}
        }
        if (modes.find_first_of("t") != modes.npos)
        {
            if (modes.at(0) == '+')
			{
                caller.respond(":AFGchat 324 NOTICE " + channel_name + " :topic operater only ON\n"); // correct format for weechat?
                it_channel->makeTopicOpOnly();
			}
            else
			{
                caller.respond(":AFGchat 324 NOTICE " + channel_name + " :topic operater only OFF\n"); // correct format for weechat?
                it_channel->removeTopicOpOnly();
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
                /* get list of users in specified channel and loop through it
                   if specified user is in channel add/remove operator-rights */
				std::set<Client*> const user_set = it_channel->getUsers();
                std::set<Client*>::const_iterator it_client;
                for(it_client = user_set.begin(); it_client != user_set.end(); ++it_client)
                {
                    if ((*it_client)->get_nick() == user)
					{
                	    std::cout << (*it_client)->get_nick() << std::endl;
                	    if (modes.at(0) == '+')
				        {
	                        caller.respond(":AFGchat 324 NOTICE " + channel_name + " :" + (*it_client)->get_nick() + " operator privileges ON\n"); // correct format for weechat?
                            it_channel->addOperator(*(*it_client));
				        }
                	    else
				        {
                            if (it_channel->getOperators().size() > 1)
                            {
                                it_channel->removeOperator(*(*it_client));
	                            caller.respond(":AFGchat 324 NOTICE " + channel_name + " :" + (*it_client)->get_nick() + " operator privileges OFF\n"); // correct format for weechat?
                            }
                            else
	                            caller.respond(":AFGchat 324 NOTICE " + channel_name + " :" + " only one operator on channel. " + (*it_client)->get_nick() + " operator privileges UNCHANGED.\n"); // correct format for weechat?
				        }
                        return ;
					}
                }
                caller.respond(":AFGchat 401 NOTICE " + user + " :No such nick\n"); // correct format for weechat?
            }
        }
    }
}
