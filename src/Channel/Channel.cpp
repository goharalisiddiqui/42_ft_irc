#include "../include/Channel.hpp"

Channel::Channel(const std::string& name) : name(name) {}

std::string Channel::getName() const
{
  return name;
}

void Channel::addUser(Client& user)
{
  if (invited_users.count(&user) > 0 || !this->inviteOnly)
  {
    users.insert(&user);
    invited_users.erase(&user);
  }
  // Add other checks or error handling here as needed
}

void Channel::removeUser(const Client& user)
{
  users.erase(std::remove(users.begin(), users.end(), &user), users.end());
}

bool Channel::hasUser(const Client& user) const
{
  return (std::find(users.begin(), users.end(), user) != users.end());
}

std::set<Client*> Channel::getUsers(void) const
{
  return this->users;
}

void Channel::inviteUser(const Client& inviter, Client& invitee)
{
    invited_users.insert(&invitee);
}

void Channel::addOperator(Client& user)
{
  operators.insert(&user);
}

void Channel::removeOperator(Client& user)
{
  operators.erase(&user);
}

bool Channel::isOperator(Client& user) const
{
  return (operators.count(&user) > 0);
}
