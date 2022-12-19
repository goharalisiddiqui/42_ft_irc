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
    }


    Client &Client::operator=(const Client &src)
    {
        this->fd = src.get_fd();
        this->message = src.get_message();
        this->active= src.isactive();
        this->garbage = src.isgarbage();
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

    void Client::activate(void)
    {
        this->active = true;
    }



    ///////////// SETTERS /////////////////////


    void Client::authenticate(void)
    {
        bool success = this->read_username();
        if (success)
        {

            this->authentic = true;
        }
        else
        {
            this->respond("Invalid username");
            this->set_garbage();
        }
    }

    bool Client::read_username(void)
    {
        this->username = this->message.substr(0,this->message.length() - 1);
        return true;
    }

    void Client::deactivate(void)
    {
        this->active = false;
    }

    void Client::set_garbage(void)
    {
        deactivate();
        this->authentic = false;
        this->message.clear();
        this->garbage = true;
    }





    ///////////// FUNCTIONS /////////////////////


    void Client::hear(void)
    {
        int flag = 1;
        char readchar = '\0';

        while(flag > 0)
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
}