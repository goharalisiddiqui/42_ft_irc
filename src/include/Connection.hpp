#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#pragma once

#include "libs.hpp"

namespace AFG
{
    class Connection
    {
        public:
            Connection();
            ~Connection();

            Connection(int fid);
            Connection(const Connection &src);

            Connection &operator=(const Connection &src);

            int get_fd(void) const;
            std::string get_message(void) const;

            void set_garbage(void);


            void activate(void);
            void deactivate(void);



            void hear(void);
            void respond(std::string response);

            void clearmessage();

            bool ismessagecomplete(void);
            bool isactive(void) const;
            bool isgarbage(void) const;
            

        private:
            int fd;
            std::string message;
            bool active;
            bool garbage;

        

        public:
            class EmptyConnection: public ::std::exception
            {
                public:
                    virtual const char* what() const throw()
                    {
                        return "Attempt to make an empty connection";
                    }
            };

            class CannotReadfromSocket: public ::std::exception
            {
                public:
                    virtual const char* what() const throw()
                    {
                        return "Attempt to read from a socket failed.";
                    }
            };

            class CannotReadfromClosedSocket: public ::std::exception
            {
                public:
                    virtual const char* what() const throw()
                    {
                        return "Attempt to read from a socket failed.";
                    }
            };

    };
}

#endif