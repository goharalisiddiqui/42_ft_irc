#include "../include/ErrorHandler.hpp"

namespace AFG
{

    ErrorHandler::ErrorHandler()
    {

    }

    ErrorHandler::~ErrorHandler()
    {

    }

    // ErrorHandler::ErrorHandler(const ErrorHandler &src)
    // {

    // }


    // ErrorHandler &ErrorHandler::operator=(const ErrorHandler &src)
    // {

    // }

    void ErrorHandler::handle(const std::exception &e)
    {
        try 
        {
            throw;
        }
        catch( ServerSocket::CannotBindSocket &e) 
        {
            this->bindExceptioHandler();
        }
        catch(std::exception &e) {
            std::cout << "An exception was thrown which is not handled by the ErrorHandler yet the error message is:" << e.what() << std::endl;
            std::cout << "Continuing..." << e.what() << std::endl;
        }


    }

    void ErrorHandler::bindExceptioHandler(void)
    {
        int wait;

        wait = 5;
        std::cout << "Cannot bind socket, this is probably because it needs time to be freed" << std::endl;
        std::cout << "Will wait " << wait << " seconds before continuing" << std::flush;
        while(wait > 0)
        {
            sleep(1);
            std::cout << "." << std::flush;
            wait--;
        }
        std::cout << std::endl;

    }

}