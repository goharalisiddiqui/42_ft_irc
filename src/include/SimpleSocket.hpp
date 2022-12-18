#ifndef SIMPLESOCKET_HPP
#define SIMPLESOCKET_HPP

#pragma once

#include "libs.hpp"

// MACRO DEFINITIONS

#define AFG_SOCK_EMPTY 0
#define AFG_SOCK_CREATED 1



namespace AFG
{
    struct s_sockparams{
        int domain;
        int type;
        int protocol;

    };

    class SimpleSocket
    {
        public:
            SimpleSocket();
            ~SimpleSocket();
            SimpleSocket(const SimpleSocket &src);

            SimpleSocket &operator=(SimpleSocket const &src);




            SimpleSocket(int domain, int type, int protocol);
            SimpleSocket(struct s_sockparams);
            

            // Setter and Getters
            struct s_sockparams get_params() const;
            int get_socket() const;
            int get_status() const;


        private:
            int sock;
            int reuse_addr;
            struct s_sockparams params;
            const static struct s_sockparams default_params;

            void create_socket(struct s_sockparams par);
        
        protected:
            int status;

            
        
        public:

            class EmptySocket: public ::std::exception
            {
                public:
                    virtual const char* what() const throw()
                    {
                        return "A program tried to get access to an empty socket";
                    }
            };

            class CannotCreateSocket: public ::std::exception
            {
                public:
                    virtual const char* what() const throw()
                    {
                        return "Failed to create socket";
                    }
            };

            class CannotReuseSocket: public ::std::exception
            {
                public:
                    virtual const char* what() const throw()
                    {
                        return "Failed to create socket";
                    }
            };
    };

}

#endif