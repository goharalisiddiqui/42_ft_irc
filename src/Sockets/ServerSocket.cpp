#include "../include/ServerSocket.hpp"
namespace AFG
{

    // const struct sockaddr_in ServerSocket::default_binding = {(__uint8_t)0, AF_INET, htons(80), INADDR_ANY};

    ServerSocket::ServerSocket() : SimpleSocket::SimpleSocket()
    {


    }

    ServerSocket::~ServerSocket()
    {

    }

    ServerSocket::ServerSocket(ServerSocket const &src) : SimpleSocket::SimpleSocket(src)
    {

    }

    ServerSocket::ServerSocket(int domain, int type, int protocol) : SimpleSocket::SimpleSocket(domain, type, protocol)
    {

    }
    
    ServerSocket::ServerSocket(struct s_sockparams par) : SimpleSocket::SimpleSocket(par)
    {

    }

    struct sockaddr_in ServerSocket::getaddr(void) const
    {
        return this->addr;
    }

    void ServerSocket::socklisten()
    {
        this->socklisten((struct in_addr){(in_addr_t)INADDR_ANY}, 8080, 10);
    }

    void ServerSocket::socklisten(struct in_addr ip, in_port_t port, int queue_len)
    {
        int return_val;
        ErrorHandler elliot;

        try
        {
            if (this->status < AFG_SOCK_CREATED)
                throw SimpleSocket::EmptySocket();
            if (this->status >= AFG_SOCK_LISTENING)
                return;

            memset((char *)&(this->addr), 0, sizeof(this->addr));
            this->addr.sin_addr = ip;
            //TODO maybe have to use htonl on ip also
            this->addr.sin_family = get_params().domain;
            this->addr.sin_port = htons(port);

            // printf("Binding now\n");
            return_val = bind(this->get_socket(), (const sockaddr *)&(this->addr), sizeof(this->addr));

            if (return_val == 0)
                this->status = AFG_SOCK_BOUND;
            else
                throw ServerSocket::CannotBindSocket();

            // printf("Starting listen now\n");
            return_val = listen(get_socket(), queue_len);

            if (return_val == 0)
                this->status = AFG_SOCK_LISTENING;
            else
                throw ServerSocket::SocketCannotListen();
        }
        catch(const std::exception& e)
        {
            elliot.handle(e);
        }
    }

    int ServerSocket::sockaccept()
    {
        int return_val;
        struct sockaddr_in raddr;
        socklen_t addrlen;
        return_val = this->sockaccept(raddr, addrlen);
        return return_val;
    }

    int ServerSocket::sockaccept(struct sockaddr_in &raddr, socklen_t &socklen)
    {
        int return_val;
        ErrorHandler elliot;

        try
        {
            if (this->status < AFG_SOCK_LISTENING)
                throw ServerSocket::SocketCannotAccept();

            return_val = accept(this->get_socket(), (struct sockaddr *)&raddr, &socklen);
            if (return_val < 0)
                throw ServerSocket::SocketCannotAccept();
        }
        catch(const std::exception& e)
        {
            elliot.handle(e);
        }
        return return_val;

    }


} // namespace AFG