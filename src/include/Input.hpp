#ifndef INPUT_HPP
#define INPUT_HPP

#pragma once

#include "libs.hpp"

namespace AFG
{
    class Input
    {
        public:
            Input();
            ~Input();

            Input(const Input &src);

            Input &operator=(const Input &src);

            std::string                         getCommand(void) const;
            std::vector<std::string>			getTargets(void) const;
            std::string                         getMessage(void) const;

            void    setCommand(std::string _command);
            void    setTargets(std::vector<std::string> _targets);
            void    setMessage(std::string _message);

        private:

            std::string                         command;
            std::vector<std::string>            targets;
            std::string                         message;

        public:
            class FlawedMessage: public ::std::exception
            {
                public:
                    virtual const char* what() const throw()
                    {
                        return "Received flawed message from Responder";
                    }
            };

    };
}

#endif
