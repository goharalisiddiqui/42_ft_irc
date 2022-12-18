#include "../include/Connection.hpp"

namespace AFG
{
    Connection::Connection()
    {
        throw Connection::EmptyConnection();
    }

    Connection::~Connection()
    {
        // close(fd);
    }

    Connection::Connection(int fid)
    {
        this->active = false;
        this->garbage = false;
        this->fd = fid;
    }

    int Connection::get_fd(void) const
    {
        return this->fd;
    }

    std::string Connection::get_message(void) const
    {
        return this->message;
    }

    void Connection::set_garbage(void)
    {
        deactivate();
        this->message.clear();
        this->garbage = true;
    }

    void Connection::clearmessage()
    {
        this->message.clear();
    }



    Connection::Connection(const Connection &src)
    {
        this->fd = src.get_fd();
        this->message = src.get_message();
        this->active = src.isactive();
        this->garbage = src.isgarbage();
    }


    Connection &Connection::operator=(const Connection &src)
    {
        this->fd = src.get_fd();
        this->message = src.get_message();
        this->active= src.isactive();
        this->garbage = src.isgarbage();
        return (*this);

    }

    void Connection::hear(void)
    {
        int flag = 1;
        char readchar;

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
            //     throw Connection::CannotReadfromSocket();
            // }
            if (flag == 0)
            {
                this->set_garbage();
                // throw Connection::CannotReadfromClosedSocket();
            }
            if (flag > 0)
                this->message += readchar;
            // this->done_read = ismessagecomplete();
        }
    }

    bool Connection::ismessagecomplete(void)
    {
        int i = 0;

        for (std::string::reverse_iterator rit=this->message.rbegin(); rit != this->message.rend(); ++rit)
        {
            if (*rit != 10 && *rit != 13)
            {
                // printf("prev char=%d\n", *(rit+1));
                // printf("Message from %d of length %ld is NOT complete\n", this->fd, this->message.size());
                return false;
            }
            if (++i == 4)
                break;
        }
        if (i == 4)
            return true;
        return false;
    }

    bool Connection::isactive() const
    {
        return (this->active);
    }
    bool Connection::isgarbage() const
    {
        return (this->garbage);
    }

    void Connection::activate(void)
    {
        this->active = true;
    }
    void Connection::deactivate(void)
    {
        this->active = false;
    }



    void Connection::respond(std::string response)
    {
        write(this->fd, response.c_str(), response.size());
    }

}