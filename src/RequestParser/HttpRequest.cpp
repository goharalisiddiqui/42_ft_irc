#include "../include/HttpRequest.hpp"

namespace AFG
{
    HttpRequest::HttpRequest() {}

    HttpRequest::~HttpRequest() {}

    HttpRequest::HttpRequest(const HttpRequest &src)
    {
        *this = src;
    }

    HttpRequest &HttpRequest::operator=(const HttpRequest &src)
    {
        if (this != &src)
        {
            this->path = src.getPath();
            this->method = src.getMethod();
            this->http_version = src.getHttp_version();
            this->fields = src.getFieldsVal();
        }
        return (*this);
    }

/*---------------- getter ----------------*/

    std::string HttpRequest::getMethod(void) const
    {
        return (this->method);
    }
    std::string HttpRequest::getPath(void) const
    {
        return (this->path);
    }

    std::string HttpRequest::getHttp_version(void) const
    {
        return (this->http_version);
    }

    std::map<std::string, std::string>  &HttpRequest::getFieldsRef(void)
    {
        return (this->fields);
    }

    std::map<std::string, std::string>  HttpRequest::getFieldsVal(void) const
    {
        return (this->fields);
    }

/*---------------- setter ----------------*/

   void HttpRequest::setMethod(std::string _method)
    {
        this->method = _method;
    }
   void HttpRequest::setPath(std::string _path)
    {
        this->path = _path;
    }

   void HttpRequest::setHttp_version(std::string _http_version)
    {
        this->http_version = _http_version;
    }
}
