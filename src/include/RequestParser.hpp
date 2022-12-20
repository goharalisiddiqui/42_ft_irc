#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#pragma once

#include "libs.hpp"
#include "StringFromClient.hpp"

namespace AFG
{
    //class   HttpRequest;

    class   RequestParser
    {
        public:
            RequestParser();
            ~RequestParser();

            RequestParser(std::string _message);
            RequestParser(const RequestParser &src);

            RequestParser &operator=(const RequestParser &src);

            std::string                 getClientRequest(void) const;
            StringFromClient            getStringFromClient(void) const;
            std::vector<std::string>    getUserInfo(std::string input);
            std::string                 getUserNick(std::string input);

        private:
            std::string                 client_request;
            std::vector<std::string>    client_request_split_in_lines;
            StringFromClient            string_from_client;

            std::vector<std::string>    afgSplit(std::string _message, std::string _delimiter);
            std::string                 parseToken(std::string _delimiter, int _token_pos);
            std::vector<std::string>    parseListToken(std::vector<std::string> _delimiter, int _token_pos);
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
