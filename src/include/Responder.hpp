#ifndef RESPONDER_HPP
#define RESPONDER_HPP

#pragma once

#include "libs.hpp"
#include "RequestParser.hpp"

namespace AFG
{
    class RequestParser;

    class Responder
    {
        public:
            Responder();
            ~Responder();

            Responder(std::string _message);
            Responder(const Responder &src);

            Responder &operator=(const Responder &src);

            RequestParser   getRequestParser(void) const;
            std::string     getMessage(void) const;

        private:
            std::string     message;
            RequestParser   request_parser;

        public:
            class FlawedMessage: public ::std::exception
            {
                public:
                    virtual const char* what() const throw()
                    {
                        return "Received flawed message from Connection";
                    }
            };

    };
}

#endif
