#include "../include/All.hpp"


in_port_t readport(std::string const &portstr)
{
    in_port_t port;

    std::istringstream(portstr) >> port;

    return port;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Need port and password" << std::endl;
        exit(EXIT_FAILURE);
    }
    in_port_t port = readport(std::string(argv[1]));
    // std::cout << "port:" << port << std::endl;
    AFG::GateKeeper heimdal(port);
    std::cout << "Server started" << std::flush;
    while (true)
    {
        heimdal.watchover();
        heimdal.serve();
    }


    return (0);
}