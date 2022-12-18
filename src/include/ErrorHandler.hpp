#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#pragma once

#include "libs.hpp"
#include "All.hpp"

namespace AFG
{

    class ErrorHandler
    {
    public:
        ErrorHandler();
        ~ErrorHandler();
        // ErrorHandler(const ErrorHandler &src);

        // ErrorHandler &operator=(const ErrorHandler &src);

        void handle(std::exception &e);



    private:

        void bindExceptioHandler(void);
        


    };

}


#endif