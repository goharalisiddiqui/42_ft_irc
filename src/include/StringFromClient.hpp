#ifndef STRINGFROMCLIENT_HPP
#define STRINGFROMCLIENT_HPP

#pragma once

#include "libs.hpp"

namespace AFG
{
    class StringFromClient
    {
        public:
            StringFromClient();
            ~StringFromClient();

            StringFromClient(const StringFromClient &src);

            StringFromClient &operator=(const StringFromClient &src);

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
