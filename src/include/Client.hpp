#ifndef CLIENT_HPP
#define CLIENT_HPP

#pragma once

#include "libs.hpp"

namespace AFG
{
    class Client
    {
        public:
            Client();
            ~Client();

            Client(int fid, in_addr ip);
            Client(const Client &src);

            Client &operator=(const Client &src);
            
            // Getters
            int get_fd(void) const;
            std::string get_message(void) const;
            std::string get_username(void) const;
            std::string get_hostname(void) const;
            std::string get_realname(void) const;
            std::string get_servername(void) const;
            std::string get_nick(void) const;
            struct in_addr get_ip(void) const;
            bool isactive(void) const;
            bool ispassed(void) const;
            bool isauthentic(void) const;
            bool isgarbage(void) const;

            // Setter
            void set_username(std::string const &str);
            void set_hostname(std::string const &str);
            void set_realname(std::string const &str);
            void set_servername(std::string const &str);
            void set_nick(std::string const &str);
            void set_garbage(void);
            void set_passed(bool state);
            void activate(void);
            void deactivate(void);
            void authenticate(void);


            // Functions
            void hear(void);
            void respond(std::string response) const;
            void clearmessage();
            bool ismessagecomplete(void);
            void trimMessage(void);

            
            

        private:
            int fd;
            std::string message;
            bool active;
            bool authentic;
            bool passed;
            bool garbage;
            std::string username;
            std::string nick;
            struct in_addr ip;
            std::string servername;
            std::string hostname;
            std::string realname;


            bool read_username(void);

        public:
            class EmptyClient: public ::std::exception
            {
                public:
                    virtual const char* what() const throw()
                    {
                        return "Attempt to make an empty Client";
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