#include "../include/Client.hpp"

namespace AFG
{
    Client::Client()
    {
        throw Client::EmptyClient();
    }

    Client::~Client()
    {
    }

    Client::Client(int fid, in_addr ip)
    {
        this->authentic = false;
        this->active = false;
        this->passed = false;
        this->garbage = false;
        this->fd = fid;
        this->ip = ip;
    }


    Client::Client(const Client &src)
    {
        this->authentic = src.isauthentic();
        this->fd = src.get_fd();
        this->message = src.get_message();
        this->active = src.isactive();
        this->passed = src.ispassed();
        this->garbage = src.isgarbage();
        this->username = src.get_username();
        this->servername = src.get_servername();
        this->hostname = src.get_hostname();
        this->realname = src.get_realname();
        this->nick = src.get_nick();
        this->ip = src.get_ip();


    }


    Client &Client::operator=(const Client &src)
    {
        this->authentic = src.isauthentic();
        this->fd = src.get_fd();
        this->message = src.get_message();
        this->active= src.isactive();
        this->passed = src.ispassed();
        this->garbage = src.isgarbage();
        this->username = src.get_username();
        this->servername = src.get_servername();
        this->hostname = src.get_hostname();
        this->realname = src.get_realname();
        this->nick = src.get_nick();
        this->ip = src.get_ip();

        return (*this);

    }

    ///////////// GETTERS /////////////////////

    int Client::get_fd(void) const
    {
        return this->fd;
    }

    std::string Client::get_message(void) const
    {
        return this->message;
    }

    std::string Client::get_username(void) const
    {
        return this->username;
    }

    std::string Client::get_hostname(void) const
    {
        return this->hostname;
    }

    std::string Client::get_realname(void) const
    {
        return this->realname;
    }

    std::string Client::get_servername(void) const
    {
        return this->servername;
    }

    std::string Client::get_nick(void) const
    {
        return this->nick;
    }

    struct in_addr Client::get_ip(void) const
    {
        return this->ip;
    }

    bool Client::isactive() const
    {
        return (this->active);
    }

    bool Client::ispassed() const
    {
        return (this->passed);
    }

    bool Client::isauthentic() const
    {
        return (this->authentic);
    }

    bool Client::isgarbage() const
    {
        return (this->garbage);
    }



    ///////////// SETTERS /////////////////////

    void Client::set_username(std::string const &str)
    {
        this->username = str;
    }

    void Client::set_hostname(std::string const &str)
    {
        this->hostname = str;
    }

    void Client::set_realname(std::string const &str)
    {
        this->realname = str;
    }

    void Client::set_servername(std::string const &str)
    {
        this->servername = str;
    }

    void Client::set_nick(std::string const &str)
    {
        this->nick = str;
    }

    void Client::set_garbage(void)
    {
        deactivate();
        this->authentic = false;
        this->message.clear();
        this->garbage = true;
    }

    void Client::set_passed(bool state)
    {
        this->passed = state;
    }

    void Client::activate(void)
    {
        this->active = true;
    }

    void Client::authenticate(void)
    {
        if (this->username.length() == 0)
            return;
        if (this->hostname.length() == 0)
            return;
        if (this->realname.length() == 0)
            return;
        if (this->servername.length() == 0)
            return;
        if (this->nick.length() == 0)
           return;
        if (!(this->ispassed()))
            return;
        std::cout << "User " << this->username << " authenticated!" << std::endl;
        this->authentic = true;
        this->respond(":AFGchat 001 " + this->nick);
        this->respond(" :Welcome to AFGchat 4.0. You are now authenticated!\n");
        this->respond(":AFGchat 002 " + this->nick + " :Your host is ");
        this->respond(this->hostname + " and your user@host is: ");
        this->respond(this->username + "@" + this->servername );
        this->respond(". You are known as: " + this->realname);
        this->respond(". Nice to meet your nick: " + this->nick + "!\n");
    }

    void Client::deactivate(void)
    {
        this->active = false;
    }

    




    ///////////// FUNCTIONS /////////////////////


    void Client::hear(void)
    {
        int flag = 1;
        char readchar = '\0';

        while(flag > 0 && !this->ismessagecomplete())
        {
            flag = read(this->fd, &readchar, 1);
            if (flag == 0)
                this->set_garbage();
            if (flag > 0)
                this->message += readchar;
        }
    }

    void Client::respond(std::string response) const
    {
        write(this->fd, response.c_str(), response.size());
    }

    void Client::clearmessage()
    {
        this->message.clear();
    }

    bool Client::ismessagecomplete(void)
    {
        int msglen = sizeof(MSG_END_SEQ) - 1;
        if (this->message.length() >= msglen
            && this->message.substr(this->message.length() - msglen, msglen) == MSG_END_SEQ)
            return true;
        return false;
    }

    void Client::trimMessage(void)
    {
        if (this->ismessagecomplete())
        {
            this->message = this->message.substr(0, this->message.length() - sizeof(MSG_END_SEQ) + 1);
        }
    }
}