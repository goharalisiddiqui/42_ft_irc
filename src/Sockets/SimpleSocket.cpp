#include "../include/SimpleSocket.hpp"
#include "../include/ErrorHandler.hpp"


namespace AFG
{

    /// CONSTRUCTORS

    const struct s_sockparams SimpleSocket::default_params = {AF_INET, SOCK_STREAM, 0};

    SimpleSocket::SimpleSocket()
    {
        
        this->status = AFG_SOCK_EMPTY;
        this->params = default_params;
        create_socket(this->params);
        
    }

    SimpleSocket::SimpleSocket(int domain, int type, int protocol)
    {
        this->status = AFG_SOCK_EMPTY;
        this->params = (struct s_sockparams){domain, type, protocol};
        create_socket(this->params);
    }

    SimpleSocket::SimpleSocket(struct s_sockparams par)
    {
        this->status = AFG_SOCK_EMPTY;
        this->params = par;
        create_socket(this->params);
    }

    SimpleSocket::SimpleSocket(const SimpleSocket &src)
    {
        this->params = src.get_params();
    }
    

    /// DESTRUCTOR

    SimpleSocket::~SimpleSocket()
    {
        close(sock);
    }


    /// OPERATOR OVERLOADS

    SimpleSocket &SimpleSocket::operator=(SimpleSocket const &src)
    {
        this->params = src.get_params();
        this->sock = src.sock;
        this->status = src.sock;

        return *this;
    }

    /// GETTERS

    struct s_sockparams SimpleSocket::get_params() const
    {
        return this->params;
    }

    int SimpleSocket::get_status() const
    {
        return this->status;
    }

    int SimpleSocket::get_socket() const
    {
        if (status >= AFG_SOCK_CREATED)
            return this->sock;
        else
        {
            throw SimpleSocket::EmptySocket();
        }

    }

    /// MEMBER FUNCTIONS

    void SimpleSocket::create_socket(struct s_sockparams par)
    {
        ErrorHandler elliot;
        int return_val;
        try
        {

            if (this->status >= AFG_SOCK_CREATED)
                return;

            return_val = socket(par.domain, par.type, par.protocol);
            if (return_val < 0)
                throw SimpleSocket::CannotCreateSocket();
            
            this->status = AFG_SOCK_CREATED;
            this->sock = return_val;
            return_val = setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, &this->reuse_addr, sizeof(this->reuse_addr));
            if (return_val < 0)
                throw SimpleSocket::CannotReuseSocket();
        }
        catch(const std::exception& e)
        {
            elliot.handle(e);
        }
    }

}