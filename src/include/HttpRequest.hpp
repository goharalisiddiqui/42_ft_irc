#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#pragma once

#include "libs.hpp"

namespace AFG
{
    class HttpRequest
    {
        public:
            HttpRequest();
            ~HttpRequest();

            HttpRequest(const HttpRequest &src);

            HttpRequest &operator=(const HttpRequest &src);

            std::string                         getPath(void) const;
            std::string                         getMethod(void) const;
            std::string                         getHttp_version(void) const;
            std::map<std::string, std::string>  &getFieldsRef(void);
            std::map<std::string, std::string>  getFieldsVal(void) const;

            void    setMethod(std::string _method);
            void    setPath(std::string _path);
            void    setHttp_version(std::string _http_request);

        private:

            std::string                         method;
            std::string                         path;
            std::string                         http_version;
            std::map<std::string, std::string>  fields;

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
