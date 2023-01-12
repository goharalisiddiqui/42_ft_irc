#include "../include/Commander.hpp"

namespace AFG
{
    void Commander::commandPASS(Client &caller, std::string pass)
    {
        // printf("Correct pass=%s\n", this->pass.c_str());
        // printf("ENTERED pass=%s\n", pass.c_str());
        if (pass != this->pass)
        {
            caller.respond(":AFGchat 464 NOTICE Auth :Wrong password!\n");
            return;
        }
        caller.set_passed();
        caller.respond(":AFGchat NOTICE Auth :Password accepted.\n");

        caller.authenticate();
    }
}
