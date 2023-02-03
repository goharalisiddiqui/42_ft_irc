#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#pragma once

#include "libs.hpp"

#define ERR_WAIT 5

namespace AFG
{

    class ErrorHandler
    {
    public:
        ErrorHandler();
        ~ErrorHandler();

        void handle(const std::exception &e);



    private:

        void bindExceptionHandler(void);
        void selectExceptionHandler(void);
        void waiter(int wait);
        


    };

}



#endif