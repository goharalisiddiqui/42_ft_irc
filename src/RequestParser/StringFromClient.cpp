#include "../include/StringFromClient.hpp"

namespace AFG
{
    StringFromClient::StringFromClient() {}

    StringFromClient::~StringFromClient() {}

    StringFromClient::StringFromClient(const StringFromClient &src)
    {
        *this = src;
    }

    StringFromClient &StringFromClient::operator=(const StringFromClient &src)
    {
        if (this != &src)
        {
            this->command = src.getCommand();
            this->targets = src.getTargets();
            this->message = src.getMessage();
        }
        return (*this);
    }

/*---------------- getter ----------------*/

    std::string StringFromClient::getCommand(void) const
    {
        return (this->command);
    }
    std::vector<std::string> StringFromClient::getTargets(void) const
    {
        return (this->targets);
    }

    std::string StringFromClient::getMessage(void) const
    {
        return (this->message);
    }

/*---------------- setter ----------------*/

   void StringFromClient::setCommand(std::string _command)
    {
        this->command = _command;
    }
   void StringFromClient::setTargets(std::vector<std::string> _targets)
    {
        this->targets = _targets;
    }

   void StringFromClient::setMessage(std::string _message)
    {
        this->message = _message;
    }
}
