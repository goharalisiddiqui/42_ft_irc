#include "../include/GateKeeper.hpp"


namespace AFG
{
    GateKeeper::GateKeeper() : sock(), conmax(10)
    {
        this->sock.socklisten();
        this->fdmax = this->sock.get_socket();
        setnonblock(this->sock.get_socket());

    }

    GateKeeper::GateKeeper(int domain, int type, int protocol, int ip, int port, int backlog, int nmax) : sock(domain, type, protocol), conmax(nmax)
    {
        this->sock.socklisten((struct in_addr){(in_addr_t)ip}, port, backlog);
        this->fdmax = sock.get_socket();
        setnonblock(this->sock.get_socket());
    }

    GateKeeper::~GateKeeper()
    {

    }

    std::list<Connection> &GateKeeper::get_connections(void)
    {
        return (this->connections);
    }
    
    fd_set GateKeeper::build_selist()
    {
        fd_set res;

        FD_ZERO(&res);

        // printf("connections size=%lu\n", this->connections.size());
        // printf("Adding %d to sellist.\n", this->sock.get_socket());
        FD_SET(this->sock.get_socket(), &res);
        for(std::list<Connection>::iterator it=this->connections.begin(); it != this->connections.end(); ++it)
        {
            // printf("Adding %d to sellist.\n", it->get_fd());
            if (!it->ismessagecomplete())
                FD_SET(it->get_fd(), &res);
        }
        return res;
    }


    void GateKeeper::watchover()
    {
        struct timeval timeout;
        int return_val = 0;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

        garbage_collector();


        fd_set selist = build_selist();

        // printf("Server Socket status=%d\n", this->sock.get_status());
        // printf("Max file descriptor=%d\n", this->fdmax);
        // printf("Is Server Socket in the watch list=%d\n", FD_ISSET(this->sock.get_socket(), &selist));
        // printf("Server Socket params:\nDomain=%d, type=%d, protocol=%d, ip=%d, port=%d\n", this->sock.get_params().domain, this->sock.get_params().type, this->sock.get_params().protocol, (int)this->sock.getaddr().sin_addr.s_addr, ntohs(this->sock.getaddr().sin_port));            
        while (return_val == 0)
        {
            selist = build_selist();
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;
		    return_val = select(this->fdmax + 1, &selist, (fd_set *) 0, (fd_set *) 0, &timeout);
		    if (return_val < 0)
            {
                // perror("select:");
                throw GateKeeper::CannotSelect();

            }
		    if (return_val == 0) {
			    printf(".");
			    fflush(stdout);
		    }
        }
        // printf("HERE1\n");
        if (FD_ISSET(this->sock.get_socket(),&selist))
        {
            // printf("HERE2\n");
		    addconnection();
            return_val--;
        }
        for(std::list<Connection>::iterator it=this->connections.begin(); it != this->connections.end(); ++it)
        {
            if (FD_ISSET(it->get_fd(), &selist))
                it->activate();
            else
                it->deactivate();
        }
    }

    void GateKeeper::addconnection()
    {
        struct sockaddr_in addr = sock.getaddr();
        socklen_t addr_len = (socklen_t)sizeof(addr);

        int newfd = accept(this->sock.get_socket(),(struct sockaddr *)&addr, &addr_len);
        //TODO Use sockaccept from ServerSocket instead
        if (newfd < 0)
            throw ServerSocket::SocketCannotAccept();
        setnonblock(newfd);
        // printf("New connection fd=%d\n", newfd);
        if (this->connections.size() == this->conmax)
        {
            // printf("Refusing connection of %d\n", newfd);
            refuse_connection(newfd);
            return;
        }
        Connection newcon(newfd);
        this->connections.push_back(newcon);
        if (newfd > this->fdmax)
            this->fdmax = newfd;
    }

    void GateKeeper::refuse_connection(int fd)
    {
        std::string refuse_msg("Server is busy\n");
        write(fd, refuse_msg.c_str(), refuse_msg.size());
        close(fd);
    }

    void GateKeeper::removeconnection(int fd)
    {
        for(std::list<Connection>::iterator it=this->connections.begin(); it != this->connections.end(); ++it)
        {
            if (it->get_fd() == fd)
            {
                close(fd);
                this->connections.erase(it);
                break;
            }
        }

    }


    void GateKeeper::setnonblock(int fd)
    {
        fcntl(fd, F_SETFL, O_NONBLOCK);
    }

    void GateKeeper::garbage_collector(void)
    {
        for(std::list<Connection>::iterator it=this->connections.begin(); it != this->connections.end(); ++it)
        {
            if (it->isgarbage())
			{
                this->connections.erase(it);
				it=this->connections.begin();
			}
        }
    }



}
