#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#pragma once

#include "libs.hpp"
#include "Input.hpp"

namespace AFG
{
    class   RequestParser
    {
        public:
            RequestParser();
            ~RequestParser();

            RequestParser(std::string _message);
            RequestParser(const RequestParser &src);

            RequestParser &operator=(const RequestParser &src);

            std::string                 getClientRequest(void) const;
            Input            getInput(void) const;
            std::vector<std::string>    getUserInfo(std::string input);
            std::string                 getUserNick(std::string input);
            std::string                 parseToken(std::string _delimiter, size_t _token_pos);
            std::vector<std::string>    parseListToken(std::vector<std::string> _delimiter, size_t _token_pos);
            std::string                 parseMessage(void);

        private:
            std::string                 client_request;
            std::vector<std::string>    client_request_split_in_lines;
            Input                       input;

            std::vector<std::string>    afgSplit(std::string _message, std::string _delimiter);
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
