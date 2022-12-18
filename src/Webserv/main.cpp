#include "../include/All.hpp"


int AFGwebserver(void)
{
    AFG::GateKeeper heimdal;
    AFG::ErrorHandler harvey;
    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 27\r\nConnection: close\r\n\r\nGreetings! fron AFG webserv";
    while (true)
    {
        try 
        {
            std::cout << "Server started" << std::flush;
            heimdal.watchover();
            for(std::list<AFG::Connection>::iterator it=heimdal.get_connections().begin(); it != heimdal.get_connections().end(); ++it)
            {

                if (it->isactive())
                {
                    
                    it->hear();
                    // printf("Printing message of length %ld\n", it->get_message().size());
                }
                if (it->ismessagecomplete())
                {
                    AFG::Responder      responder(it->get_message());
                
                
                    // std::cout << std::endl << it->get_message() << std::endl;
                    std::cout << std::endl << responder.getRequestParser().getHttpRequest().getHttp_version() << std::endl;
                    std::cout << std::endl << responder.getRequestParser().getHttpRequest().getMethod() << std::endl;
                    std::cout << std::endl << responder.getRequestParser().getHttpRequest().getPath() << std::endl;

                    it->respond(response);
                    close(it->get_fd());
                    it->set_garbage();
                }
            }   
        }
        catch(std::exception &e)
        {
            harvey.handle(e);
        }
        catch(...)
        {
            std::cout << "Something went WROOOONG!" << std::endl;
        }
    }
}


int main()
{
    
    

    AFGwebserver();


    return (0);
}