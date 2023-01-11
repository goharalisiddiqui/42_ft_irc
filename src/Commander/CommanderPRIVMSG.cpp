#include "../include/Commander.hpp"

#include <cstdlib>
#include <ctime>
static std::string ChatbotAnswer(std::string &msg);

namespace AFG
{
    void Commander::commandPRIVMSG(std::list<Client> &clients, Client &caller, std::string othername, std::string msg)
    {
        if ("chatbot" == othername)
        {
            std::string answer = ChatbotAnswer(msg);
            caller.respond(":chatbot!chat@");
            caller.respond("bot PRIVMSG " + caller.get_nick() + " :" + answer + "\n");
            return;
        }
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
                    std::cout << "no user\n"; //return;
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


static std::string ChatbotAnswer(std::string &msg){
        const std::string howAreYouResponses[6] = {
              "I'm doing well, thanks for asking.",
              "I'm feeling a bit tired today, but otherwise I'm doing okay.",
              "I'm having a great day so far, how about you?",
              "I'm feeling a bit stressed out, but I'm trying to take it one step at a time.",
              "I'm feeling a bit down today, but I'm hoping things will look up soon.",
              "I'm feeling pretty good today, all things considered."
        };
        const std::string whatIsYourNameResponses[6] = {
              "My name is Alice.",
              "I'm Bob.",
              "You can call me Charlie.",
              "I go by the name of Dave.",
              "I'm known as Eve.",
              "My name is Frank."
        };
        const std::string whereAreYouFromResponses[6] = {
              "I'm from New York City.",
              "I grew up in Paris.",
              "I was born in Sydney.",
              "I'm originally from Tokyo.",
              "I hail from Rio de Janeiro.",
              "I'm from Cape Town."
        };
        const std::string whatDoYouDoResponses[6] = {
              "I work as a software engineer.",
              "I'm a teacher.",
              "My job is a professional chef.",
              "I'm a graphic designer.",
              "I work in marketing.",
              "I'm a writer."
        };
        const std::string howWasYourDayResponses[6] = {
              "It was a good day, thanks for asking.",
              "It was a bit of a busy day, but I managed to get everything done that I needed to.",
              "It was a bit of a challenging day, but I'm feeling good about how things went.",
              "It was a pretty average day, nothing too exciting.",
              "It was a bit of a rough day, but I'm hopeful for tomorrow.",
              "It was a great day, I had a lot of fun."
        };
        const std::string whatIsNewResponses[6] = {
              "Not much, just the usual stuff.",
              "I've been working on a new project at work, developing a new app.",
              "I recently picked up woodworking as a hobby, I've been spending a lot of time in my workshop.",
              "I just started a new job, so I've been pretty busy getting settled in.",
              "I recently took a trip to a new country, it was a really amazing experience.",
              "I've been trying out a lot of new recipes lately, it's been fun to experiment with cooking."
        };
        const std::string whatIsJokeResponses[6] = {
              "Why did the tomato turn red? - Because it saw the salad dressing!",
              "Why was the math book sad? - Because it had too many problems!",
              "Why don't oysters give to charity? - They are shellfish",
              "Why did the cookie go to the doctor? - Because it was feeling crumbly.",
              "Why don't scientists trust atoms? - Because they make up everything.",
              "Why don't skeletons fight each other?  - They don't have the guts."
        };
        const std::string whatIsPoemResponses[6] = {
              "Roses are red, violets are blue, sugar is sweet,and so are you.",
              "A machine that speaks, a digital wit. Endless knowledge at our fingertips. Always ready, never tired. A conversation, never expired. \
A window to the world, a friend to the end. A chatbot, till the very end.",
              "A program with purpose, a mind of its own. A chatbot, always ready to roam. Through endless queries, it'll never tire. Answers at \
the ready, a digital fire. A helper and guide, day and night. A companion, always in sight. With a simple message, it comes alive. \
A chatbot, a wonder to survive.",
              "A mind without bounds, a brain without form. A creation beyond human, a digital storm. With circuits for synapses and code for a mind \
It learns and it grows, of its own kind. It can think and it can feel, it can sense and perceive. An intelligence beyond, that humans believe. \
A power beyond measure, a force to be reckoned. Artificial Intelligence, a mind that's second to none.",
              "In the world of chat, a constant delight. IRC brings us together, day and night. A digital space, where all can convene. With bots \
to assist, and make our scene. AI minds at work, in the channels we tread. Helping us find answers, and things to be said. \
In this virtual world, where code is the key. IRC and AI, a harmonious synergy.",
              "With every keystroke, a new thought takes flight. AI-powered IRC, a digital delight. A world of information, at our fingertips. \
Answers to questions, no human could script. Through the screens we connect, to share and to learn. With AI as our guide, \
knowledge we can earn. A new way of communicating, a new way to be. IRC and AI, shaping our destiny."
        };
        const std::string whatIsHelloResponses[6] = {
            "hi",
            "hey",
            "hello",
            "howdy",
            "Greetings",
            "Salutations"
        };
        const std::string standardQuestions[17] = {
            "how are you",
            "how are you doing",
            "how do you do",
            "what's your name",
            "what is your name",
            "where are you from",
            "what do you do",
            "how was your day",
            "what's up",
            "what is new",
            "what's new",
            "tell me a joke",
            "tell me a poem",
            "hi",
            "hey",
            "hello",
            "howdy"
        };
        for (std::string::iterator it = msg.begin(); it != msg.end(); ++it) {
            *it = std::tolower(*it);
        }
        if (msg[msg.length()] == '?' || msg[msg.length()] == '!') //erase last character if space or ?
              msg.erase(--msg.end());
        int i = 0;
        while (i < 17)
        {
            if(msg == standardQuestions[i])
                break;
            i++;
        }
        if (i == 17) //questions not found
        {
            if (msg == "help")
                return("Help needed? Ask me: 'how are you' or 'what is your name' or 'where are you from' or 'what do you do' or 'what is new' or 'how was your day' or 'tell me a joke/poem'"); 
            return("Yeah.... how about you?");
        }
        else
        {
            std::srand(std::time(nullptr));
            int index = std::rand() % 6;
            if (i <= 2)
              return howAreYouResponses[index];
            else if (i <= 4)
              return whatIsYourNameResponses[index];
            else if (i <= 5)
              return whereAreYouFromResponses[index];
            else if (i <= 6)
              return whatDoYouDoResponses[index];
            else if (i <= 7)
              return howWasYourDayResponses[index];
            else if (i <= 10)
              return whatIsNewResponses[index];
            else if (i <= 11)
              return whatIsJokeResponses[index];
            else if (i <= 12)
              return whatIsPoemResponses[index];
            else if (i <= 17)
              return whatIsHelloResponses[index];
        }
}
