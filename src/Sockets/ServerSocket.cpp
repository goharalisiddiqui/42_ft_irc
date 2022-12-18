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

    int ServerSocket::sockaccept()
    {
        int return_val;
        socklen_t addrlen = sizeof(this->addr);

        if (this->status < AFG_SOCK_LISTENING)
            throw ServerSocket::SocketCannotAccept();

        // printf("socket=%d, ip-%d, port=%d\n", get_socket(), addr.sin_addr, addr.sin_port);
        // printf("Starting accept now\n");
        return_val = accept(this->get_socket(), (sockaddr *)&(this->addr), &addrlen);
        // printf("Accepted now\n");

        if (return_val < 0)
            throw ServerSocket::SocketCannotAccept();
        
        return return_val;
    }

    // int ServerSocket::sockaccept(std::string &message)
    // {
    //     int return_val;
    //     int flag = 1;
    //     char readchar;
    //     std::string msg;

    //     return_val = sockaccept();
    //     fcntl(return_val, F_SETFL, O_NONBLOCK);
    //     while(flag > 0)
    //     {
    //         usleep(100);
    //         flag = read(return_val, &readchar, 1);
    //         if (flag > 0)
    //             message += readchar;
    //         printf("flag=%d, char=%d\n",flag, readchar);
    //     }

        

    //     return return_val;
    // }


} // namespace AFG