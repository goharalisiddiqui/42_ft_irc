#include "../include/Input.hpp"

namespace AFG
{
    Input::Input() {}

    Input::~Input() {}

    Input::Input(const Input &src)
    {
        *this = src;
    }

    Input &Input::operator=(const Input &src)
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

    std::string Input::getCommand(void) const
    {
        return (this->command);
    }
    std::vector<std::string> Input::getTargets(void) const
    {
        return (this->targets);
    }

    std::string Input::getMessage(void) const
    {
        return (this->message);
    }

/*---------------- setter ----------------*/

   void Input::setCommand(std::string _command)
    {
        this->command = _command;
    }
   void Input::setTargets(std::vector<std::string> _targets)
    {
        this->targets = _targets;
    }

   void Input::setMessage(std::string _message)
    {
        this->message = _message;
    }
}
