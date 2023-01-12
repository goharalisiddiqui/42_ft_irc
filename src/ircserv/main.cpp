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
    std::string pass(argv[2]);
    AFG::GateKeeper heimdal(port, pass);
    while (true)
    {
        // printf("status=%d\n", heimdal.get_sock().get_status());
        if (heimdal.get_sock().get_status() == AFG_SOCK_LISTENING)
            break;
        heimdal = AFG::GateKeeper(port,pass);
    }
    std::cout << "Server started" << std::endl;
    AFG::ErrorHandler elliot;
    while (true)
    {
        try
        {
            heimdal.watchover();
            heimdal.serve();
        }
        catch(const std::exception& e)
        {
            elliot.handle(e);
        }
    }

    exit(0);

    return (0);
}