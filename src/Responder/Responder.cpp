#include "../include/Responder.hpp"

namespace AFG
{
    Responder::Responder()
    {
        throw Responder::FlawedMessage();
    }

    Responder::~Responder()
    {
    }

    /* construct tmp with message to fill it. copy filled tmp*/
    Responder::Responder(std::string _message)
    {
        RequestParser   tmp(_message);

        this->request_parser = tmp;
        this->message = _message;
    }

    Responder::Responder(const Responder &src)
    {
        this->message = src.getMessage();
    }

    Responder &Responder::operator=(const Responder &src)
    {
        this->message = src.getMessage();
        return (*this);
    }

    RequestParser   Responder::getRequestParser(void) const
    {
        return (this->request_parser);
    }

    std::string Responder::getMessage(void) const
    {
        return (this->message);
    }

}
