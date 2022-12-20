#include "../include/GateKeeper.hpp"


namespace AFG
{
    GateKeeper::GateKeeper() : sock(), conmax(10), fredi("")
    {
        this->sock.socklisten();
        this->fdmax = this->sock.get_socket();
        setnonblock(this->sock.get_socket());

    }

    GateKeeper::GateKeeper(int domain, int type, int protocol, int ip, int port, int backlog, int nmax, std::string pass) : sock(domain, type, protocol), conmax(nmax), fredi(pass)
    {
        this->sock.socklisten((struct in_addr){(in_addr_t)ip}, port, backlog);
        this->fdmax = sock.get_socket();
        setnonblock(this->sock.get_socket());
    }

    GateKeeper::GateKeeper(in_port_t port, std::string pass) : sock(), conmax(10), fredi(pass)
    {
        this->sock.socklisten((struct in_addr){(in_addr_t)INADDR_ANY}, port, 10);
        this->fdmax = sock.get_socket();
        setnonblock(this->sock.get_socket());
    }

    GateKeeper::~GateKeeper()
    {

    }

    ///////////// Getters /////////////////////

    std::list<Client> &GateKeeper::get_clients(void)
    {
        return (this->clients);
    }

    ///////////// FUNCTIONS /////////////////////
    
    fd_set GateKeeper::build_selist()
    {
        fd_set res;

        FD_ZERO(&res);

        // printf("Clients size=%lu\n", this->Clients.size());
        // printf("Adding %d to sellist.\n", this->sock.get_socket());
        FD_SET(this->sock.get_socket(), &res);
        for(std::list<Client>::iterator it=this->clients.begin(); it != this->clients.end(); ++it)
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
		    add_client();
            return_val--;
        }
        for(std::list<Client>::iterator it=this->clients.begin(); it != this->clients.end(); ++it)
        {
            if (FD_ISSET(it->get_fd(), &selist))
                it->activate();
            else
                it->deactivate();
        }
    }

    void GateKeeper::add_client()
    {
        struct sockaddr_in addr = sock.getaddr();
        socklen_t addr_len = (socklen_t)sizeof(addr);

        int newfd = accept(this->sock.get_socket(),(struct sockaddr *)&addr, &addr_len);
        //TODO Use sockaccept from ServerSocket instead
        if (newfd < 0)
            throw ServerSocket::SocketCannotAccept();
        setnonblock(newfd);
        // printf("New Client fd=%d\n", newfd);
        if (this->clients.size() == this->conmax)
        {
            // printf("Refusing Client of %d\n", newfd);
            refuse_client(newfd);
            return;
        }
        Client newcon(newfd, addr.sin_addr);
        this->clients.push_back(newcon);
        if (newfd > this->fdmax)
            this->fdmax = newfd;
        newcon.respond(":AFGchat NOTICE Auth :To enter you must first give three commands: PASS, USER and NICK\n");
    }

    void GateKeeper::refuse_client(int fd)
    {
        std::string refuse_msg("Server is busy\n");
        write(fd, refuse_msg.c_str(), refuse_msg.size());
        close(fd);
    }

    void GateKeeper::remove_client(int fd)
    {
        for(std::list<Client>::iterator it=this->clients.begin(); it != this->clients.end(); ++it)
        {
            if (it->get_fd() == fd)
            {
                close(fd);
                this->clients.erase(it);
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
        for(std::list<Client>::iterator it=this->clients.begin(); it != this->clients.end(); ++it)
        {
            if (it->isgarbage())
			{
                this->clients.erase(it);
				it=this->clients.begin();
			}
            //TODO Update the fdmax
        }
    }


    // void GateKeeper::announce(std::string const &msg) const
    // {
    //     for(std::list<AFG::Client>::const_iterator it = this->clients.begin(); it != this->clients.end(); ++it)
    //     {
    //         if (it->isauthentic())
    //             it->respond(msg);
    //     }
    // }

    void GateKeeper::spreadmsgfrom(Client *speaker) const
    {
            return; //not needed anymore and conflicts with PRIVMSG 
        for(std::list<AFG::Client>::const_iterator it = this->clients.begin(); it != this->clients.end(); ++it)
        {
            if (it->isauthentic() && &(*it) != speaker)
            {
                it->respond(speaker->get_username());
                it->respond(": ");
                it->respond(speaker->get_message());
            }
        }

    }
    void GateKeeper::serve(void)
    {
        for(std::list<AFG::Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
        {
            if (it->isactive())
            {
                it->hear();
                // printf("Printing message of length %ld\n", it->get_message().size());
            }
            if (it->ismessagecomplete())
            {
                it->trimMessage();
                this->fredi.process(this->clients, *it);
                if (it->isauthentic())
                    this->spreadmsgfrom(&(*it));
                it->clearmessage();
                it->deactivate();
            }
            

        }   
    }



}
