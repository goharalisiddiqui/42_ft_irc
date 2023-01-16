#include "../include/Commander.hpp"

namespace AFG
{
    void Commander::commandQUIT(Client &caller)
    {
        std::list<AFG::Channel>::iterator it;
        for (it = this->channels.begin(); it != this->channels.end(); it++)
        {
            if (it->hasUser(caller))
            {
                it->removeUser(caller);
/*                 if (it->isOperator(caller))
                    it->removeOperator(caller); */
            }
        }
    }
}
