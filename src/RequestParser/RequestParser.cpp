#include "../include/RequestParser.hpp"

namespace AFG
{
    RequestParser::RequestParser()
    {
    }

    RequestParser::~RequestParser()
    {
    }

    RequestParser::RequestParser(std::string _message)
    {
        std::string _method;
        std::string _path;
        std::string _http_version;
        int first_line = 0;

        this->message = _message;
        this->message_split_in_lines = this->afgSplit(_message, "\n");

        _method = this->parseToken(" ", first_line, 0);
        _path = this->parseToken(" ", first_line, 1);
        _http_version = this->parseToken(" ", first_line, 2);

        this->http_request.setMethod(_method);
        this->http_request.setPath(_path);
        this->http_request.setHttp_version(_http_version);
        this->fillFields();

    }

    RequestParser::RequestParser(const RequestParser &src)
    {
        *this = src;
    }

    RequestParser &RequestParser::operator=(const RequestParser &src)
    {
        if (this != &src)
        {
            this->message = src.getMessage();
            this->http_request = src.getHttpRequest();
        }
        return (*this);
    }

/*---------------- getter ----------------*/

    std::string RequestParser::getMessage(void) const
    {
        return (this->message);
    }

    HttpRequest   RequestParser::getHttpRequest(void) const
    {
        return (this->http_request);
    }

/* ------------------ functions --------------- */

    /* gets message as parameter and splits it in single lines stored in a vector */
    std::vector<std::string>    RequestParser::afgSplit(std::string _message, std::string _delimiter)
    {
        size_t                      pos = 0;
        std::vector<std::string>    lines;

        while ((pos = _message.find(_delimiter)) != std::string::npos)
        {
            lines.push_back(_message.substr(0, pos));
            _message.erase(0, pos + _delimiter.length());
        }
        if (!lines.empty())
            lines.push_back(_message);
        return (lines);
    }

    /* splits message in lines. then, splits _line_nbr line with " " as delimiter. returns the token at position _token_pos. */
    std::string RequestParser::parseToken(std::string _delimiter, int _line_nbr, int _token_pos)
    {
        size_t                      pos = 0;
        std::vector<std::string>    line_split_in_token;

        line_split_in_token = this->afgSplit(this->message_split_in_lines.at(_line_nbr), _delimiter);
        /* <= because _token_pos starts with index 0. */
        if (line_split_in_token.size() <= _token_pos || this->message_split_in_lines.size() <= _line_nbr)
            return (""); // throw error instead?
        return (line_split_in_token.at(_token_pos));
    }

    /* loops through the lines of the message and parses the fields in the fields map */
    void    RequestParser::fillFields()
    {
        size_t                      nbr_of_lines;
        size_t                      i;

        //  todo: if there is a body size will differ
        nbr_of_lines = this->message_split_in_lines.size();
        for (i = 1; i < nbr_of_lines; i++)
        {
            this->http_request.getFieldsRef()[parseToken(": ", i, 0)] = parseToken(": ", i, 1);
            // ^^^^^^^^^^^^^test print
            // std::cout << std::endl << this->http_request.getFieldsRef()[parseToken(": ", i, 0)] << std::endl;
        }

        
    }

}
