#include "../include/ErrorHandler.hpp"
#include "../include/ServerSocket.hpp"
#include "../include/SimpleSocket.hpp"
#include "../include/GateKeeper.hpp"

namespace AFG
{

    ErrorHandler::ErrorHandler()
    {

    }

    ErrorHandler::~ErrorHandler()
    {

    }


    void ErrorHandler::waiter(int wait)
    {
        while(wait > 0)
        {
            sleep(1);
            std::cout << "." << std::flush;
            wait--;
        }
        std::cout << std::endl;
    }

    void ErrorHandler::handle(const std::exception &e)
    {
        e.what();
        try 
        {
            throw;
        }
        catch( ServerSocket::CannotBindSocket &e) 
        {
            this->bindExceptionHandler();
        }
        catch( GateKeeper::CannotSelect &e) 
        {
            this->selectExceptionHandler();
        }
        catch(std::exception &e) {
            std::cout << "An exception was thrown which is not handled by the ErrorHandler yet, the error message is: " << e.what() << std::endl;
            std::cout << "Continuing in " << ERR_WAIT << "seconds";
        }


    }

    void ErrorHandler::bindExceptionHandler(void)
    {
        std::cout << "Cannot bind socket, this is probably because it needs time to be freed" << std::endl;
        std::cout << "Will wait " << ERR_WAIT << " seconds before reattempting" << std::flush;
        this->waiter(ERR_WAIT);
    }

    void ErrorHandler::selectExceptionHandler(void)
    {
        std::cout << "Error while trying to select a client to serve, this should not happen" << std::endl;
        std::cout << "Will wait " << ERR_WAIT << " seconds before reattempting" << std::flush;
        this->waiter(ERR_WAIT);
    }

}