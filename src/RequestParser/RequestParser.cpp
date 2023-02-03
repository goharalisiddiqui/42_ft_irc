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
        if (_command != "USER" && _command != "NICK")
        {
            _delimiter.push_back(" ");
            _delimiter.push_back(",");
            _targets = this->parseListToken(_delimiter, 1);
            _message = this->parseMessage();

            this->input.setTargets(_targets);
            this->input.setMessage(_message);
        }
        this->input.setCommand(_command);
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
            this->input = src.getInput();
        }
        return (*this);
    }

/*---------------- getter ----------------*/

    std::string RequestParser::getClientRequest(void) const
    {
        return (this->client_request);
    }

    Input   RequestParser::getInput(void) const
    {
        return (this->input);
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
        if (!_client_request.empty())
            tokens.push_back(_client_request);
        return (tokens);
    }

    /* splits client request in tokens with _delimiter. returns the token at position _token_pos. */
    std::string RequestParser::parseToken(std::string _delimiter, size_t _token_pos)
    {
        std::vector<std::string>    request_split_in_tokens;

        request_split_in_tokens = this->afgSplit(this->client_request, _delimiter);
        /* <= because _token_pos starts with index 0. */
        if (request_split_in_tokens.size() <= _token_pos)
            return (""); // throw error instead?
        return (request_split_in_tokens.at(_token_pos));
    }

    /* splits client request in tokens with _delimiter.at(0). returns the list of token splitted with _delimiter.at(1).
    e.g.: PRIVMSG user1,user2 :msg 
    1) gets split with space as the delimiter
    2) token specified through _token_pos (here "user1,user2") gets split with comma as the delimiter
    3) returns vector filled with the users (targets) */
    std::vector<std::string> RequestParser::parseListToken(std::vector<std::string> _delimiter, size_t _token_pos)
    {
        std::vector<std::string>    request_split_in_tokens;
        std::vector<std::string>    token_list;

        request_split_in_tokens = this->afgSplit(this->client_request, _delimiter.at(0));
        /* <= because _token_pos starts with index 0. */
        if (request_split_in_tokens.size() <= _token_pos)
            return (std::vector<std::string>()); // throw error instead?
        token_list = this->afgSplit(request_split_in_tokens.at(_token_pos), _delimiter.at(1));
        return (token_list);
    }

    /* gets user info in one vector: 0) <username>, 1) <hostname>, 2) <servername>, 3) <realname> 
    (realname is prefixed by a colon because it can have spaces in it) */
    std::vector<std::string>    RequestParser::getUserInfo(std::string input)
    {
        std::vector<std::string>    user_info;
        std::vector<std::string>    splitted_input;

        splitted_input = this->afgSplit(input, " ");
        if (splitted_input.size() != 5)
        {
            std::cerr << "Error. Unable to authenticate user. USERNAME, HOSTNAME, SERVERNAME, REALNAME" << std::endl;
            return (std::vector<std::string>()); // or throw error 
        }
        for (int i = 1; i <= 3; i++)
        {
            if (splitted_input.at(i).find_first_of(":") != splitted_input.at(i).npos)
            {
                std::cerr << "Error. Unable to authenticate user. USERNAME, HOSTNAME, SERVERNAME" << std::endl;
                return (std::vector<std::string>()); // or throw error 
            }
            user_info.push_back(splitted_input.at(i));
        }
        splitted_input = this->afgSplit(input, ":");
        if (splitted_input.size() != 2)
        {
            std::cerr << "Error. Unable to authenticate user. REAL NAME" << std::endl;
            return (std::vector<std::string>()); // or throw error 
        }
        user_info.push_back(splitted_input.at(1));
        return (user_info);
    }

    /* parses the user nick.
    could probably be done by using parseToken(), but now the fct is already written ^^ */
    std::string RequestParser::getUserNick(std::string input)
    {
        std::string                 user_nick = "";
        std::vector<std::string>    splitted_input;

        splitted_input = this->afgSplit(input, " ");
        if (splitted_input.size() != 2)
        {
            std::cerr << "Error. Unable to authenticate user. too many params" << std::endl;
            return (user_nick); // or throw error 
        }
        user_nick = splitted_input.at(1);
        /* "nickname having a maximum length of nine (9) characters" see RFC1459  */
        if (user_nick.size() > 9 || user_nick.find_first_of("!@#$%+-:,") != user_nick.npos)
        {
            std::cerr << "Error. Unable to authenticate user. nickname too long || invalid character(s)" << std::endl;
            return (std::string()); // or throw error 
        }
        return (user_nick);
    }

    std::string RequestParser::parseMessage(void)
    {
        std::string _message = "";

        if (this->client_request.find(':') != this->client_request.npos)
            _message = this->client_request.substr(this->client_request.find(':') + 1, this->client_request.npos);
        return (_message);
    }
}
