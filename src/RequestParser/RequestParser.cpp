#include "../include/RequestParser.hpp"

namespace AFG
{
    RequestParser::RequestParser()
    {
    }

    RequestParser::~RequestParser()
    {
    }

    RequestParser::RequestParser(std::string _client_request)
    {
        std::string _command;
        std::vector<std::string> _targets;
        std::vector<std::string> _delimiter;
        std::string _message;


        this->client_request = _client_request;

        _command = this->parseToken(" ", 0);

        _delimiter.push_back(" ");
        _delimiter.push_back(",");
        _targets = this->parseListToken(_delimiter, 1);
        _message = this->parseToken(":", 1);

        this->string_from_client.setCommand(_command);
        this->string_from_client.setTargets(_targets);
        this->string_from_client.setMessage(_message);

    }

    RequestParser::RequestParser(const RequestParser &src)
    {
        *this = src;
    }

    RequestParser &RequestParser::operator=(const RequestParser &src)
    {
        if (this != &src)
        {
            this->client_request = src.getClientRequest();
            this->string_from_client = src.getStringFromClient();
        }
        return (*this);
    }

/*---------------- getter ----------------*/

    std::string RequestParser::getClientRequest(void) const
    {
        return (this->client_request);
    }

    StringFromClient   RequestParser::getStringFromClient(void) const
    {
        return (this->string_from_client);
    }

/* ------------------ functions --------------- */

     /* gets client_request as parameter and splits it in single tokens stored in a vector */
    std::vector<std::string>    RequestParser::afgSplit(std::string _client_request, std::string _delimiter)
    {
        size_t                      pos = 0;
        std::vector<std::string>    tokens;

        while ((pos = _client_request.find(_delimiter)) != std::string::npos)
        {
            tokens.push_back(_client_request.substr(0, pos));
            _client_request.erase(0, pos + _delimiter.length());
        }
        if (!tokens.empty())
            tokens.push_back(_client_request);
        return (tokens);
    }

    /* splits client request in tokens with _delimiter. returns the token at position _token_pos. */
    std::string RequestParser::parseToken(std::string _delimiter, int _token_pos)
    {
        size_t                      pos = 0;
        std::vector<std::string>    request_split_in_tokens;

        request_split_in_tokens = this->afgSplit(this->client_request, _delimiter);
        /* <= because _token_pos starts with index 0. */
        if (request_split_in_tokens.size() <= _token_pos)
            return (""); // throw error instead?
        return (request_split_in_tokens.at(_token_pos));
    }

    /* splits client request in tokens with _delimiter.at(0). returns the list of token splitted with _delimiter.at(1). */
    std::vector<std::string> RequestParser::parseListToken(std::vector<std::string> _delimiter, int _token_pos)
    {
        size_t                      pos = 0;
        std::vector<std::string>    request_split_in_tokens;
        std::vector<std::string>    token_list;

        request_split_in_tokens = this->afgSplit(this->client_request, _delimiter.at(0));
        /* <= because _token_pos starts with index 0. */
        if (request_split_in_tokens.size() <= _token_pos)
            return (std::vector<std::string>()); // throw error instead?
        token_list = this->afgSplit(request_split_in_tokens.at(_token_pos), _delimiter.at(1));
        return (token_list);
    }
}

//to do: zum laufen bringen (gohars version)
// to do: parseTargets() schreiben