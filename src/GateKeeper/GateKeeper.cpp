#include "../include/GateKeeper.hpp"
#include "../include/ErrorHandler.hpp"


namespace AFG
{
    GateKeeper::GateKeeper() : sock(), conmax(MAX_CLIENTS), fredi("")
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

    GateKeeper::GateKeeper(in_port_t port, std::string pass) : sock(), conmax(MAX_CLIENTS), fredi(pass)
    {
        this->sock.socklisten((struct in_addr){(in_addr_t)INADDR_ANY}, port, 10);
        this->fdmax = sock.get_socket();
        setnonblock(this->sock.get_socket());
    }

    GateKeeper::~GateKeeper()
    {

    }

    GateKeeper &GateKeeper::operator=(const GateKeeper &src)
    {
        this->fdmax = src.fdmax;
        this->conmax = src.conmax;
        this->clients = src.clients;
        this->sock = src.sock;
        this->fredi = src.fredi;

        return (*this);
    }


    ///////////// Getters /////////////////////

    std::list<Client> &GateKeeper::get_clients(void)
    {
        return (this->clients);
    }

    ServerSocket &GateKeeper::get_sock(void)
    {
        return this->sock;
    }

    ///////////// FUNCTIONS /////////////////////
    
    fd_set GateKeeper::build_selist()
    {
        fd_set res;

        FD_ZERO(&res);

        FD_SET(this->sock.get_socket(), &res);
        for(std::list<Client>::iterator it=this->clients.begin(); it != this->clients.end(); ++it)
        {
            if (!it->ismessagecomplete())
                FD_SET(it->get_fd(), &res);
        }
        return res;
    }


    void GateKeeper::watchover()
    {
        struct timeval timeout;
        int return_val = 0;

        garbage_collector();

        fd_set selist;

        while (return_val == 0)
        {
            selist = build_selist();
            timeout.tv_sec = 1;
            timeout.tv_usec = 10;
		    return_val = select(this->fdmax + 1, &selist, (fd_set *) 0, (fd_set *) 0, &timeout);
		    if (return_val < 0)
                throw GateKeeper::CannotSelect();
        }
        if (FD_ISSET(this->sock.get_socket(),&selist))
        {
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
        struct sockaddr_in addr;
        socklen_t addr_len = (socklen_t)sizeof(addr);

        int newfd = this->sock.sockaccept(addr, addr_len);
        setnonblock(newfd);
        if (this->clients.size() == this->conmax)
        {
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
        std::string refuse_msg(":" SERVER_NAME " " ERR_NOSUCHCHANNEL " "  SERVER_NAME  " :Server is busy." MSG_END_SEQ);
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
        this->fdmax = this->sock.get_socket();
        for(std::list<Client>::iterator it=this->clients.begin(); it != this->clients.end(); ++it)
        {
            if (it->isgarbage())
			{
                fredi.cQUIT_purgeClient(*it);
                it = this->clients.erase(it);
			}
            if (it->get_fd() > this->fdmax)
            {
                this->fdmax = it->get_fd();
            }
        }
    }


    void GateKeeper::serve(void)
    {
        for(std::list<Client>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
        {
            if (it->isactive())
            {
                it->hear();
            }
            if (it->ismessagecomplete())
            {
                it->trimMessage();
                this->fredi.process(this->clients, *it);
                it->clearmessage();
                it->deactivate();
            }
        }   
    }
}
