#include "../include/Client.hpp"

namespace AFG
{
    Client::Client()
    {
        throw Client::EmptyClient();
    }

    Client::~Client()
    {
        // close(fd);
    }

    Client::Client(int fid, in_addr ip)
    {
        this->active = false;
        this->garbage = false;
        this->fd = fid;
        this->ip = ip;
    }


    Client::Client(const Client &src)
    {
        this->fd = src.get_fd();
        this->message = src.get_message();
        this->active = src.isactive();
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
        this->fd = src.get_fd();
        this->message = src.get_message();
        this->active= src.isactive();
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

    void Client::activate(void)
    {
        this->active = true;
    }

    void Client::authenticate(void)
    {
        bool success = this->read_username();
        if (success)
        {
            std::cout << "User " << this->username << " authenticated!" << std::endl;
            this->authentic = true;
        }
        else
        {
            this->respond("Invalid username");
            this->set_garbage();
        }
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

        while(flag > 0 && readchar != '\n')
        {
            // usleep(1000);
            flag = read(this->fd, &readchar, 1);
            // if (flag < 0)
            // {
            //     // if (errno == EWOULDBLOCK)
            //     //     printf("read() would block\n");
            //     // perror("Reading:");
            //     // printf("current received message: %s\n", this->message.c_str());
            //     throw Client::CannotReadfromSocket();
            // }
            if (flag == 0)
            {
                this->set_garbage();
                // throw Client::CannotReadfromClosedSocket();
            }
            if (flag > 0)
                this->message += readchar;
            // this->done_read = ismessagecomplete();
        }
    }

    void Client::respond(std::string response) const
    {
        write(this->fd, response.c_str(), response.size());
    }

    void Client::clearmessage()
    {
        this->message.clear();
        // std::cout << "msg cleared" << std::endl;
    }

    bool Client::ismessagecomplete(void)
    {
        if (this->message.back() == '\n')
            return true;
        return false;
    }

    bool Client::read_username(void)
    {
        this->username = this->message.substr(0,this->message.length() - 1);
        return true;
    }
}