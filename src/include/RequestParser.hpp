#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#pragma once

#include "libs.hpp"
#include "HttpRequest.hpp"

namespace AFG
{
    class   HttpRequest;

    class   RequestParser
    {
        public:
            RequestParser();
            ~RequestParser();

            RequestParser(std::string _message);
            RequestParser(const RequestParser &src);

            RequestParser &operator=(const RequestParser &src);

            std::string                 getMessage(void) const;
            HttpRequest                 getHttpRequest(void) const;

        private:
            std::string                 message;
            std::vector<std::string>    message_split_in_lines;
            HttpRequest                 http_request;

            std::vector<std::string>    afgSplit(std::string _message, std::string _delimiter);
            std::string                 parseToken(std::string _delimiter, int _line_nbr, int _token_pos);
            void                        fillFields();

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
