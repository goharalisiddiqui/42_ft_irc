#include "../include/Channel.hpp"

Channel::Channel(const std::string& name) : name(name) {}

std::string Channel::getName() const
{
  return name;
}

void Channel::addUser(const std::string& user)
{
  if (invitedUsers.count(user) > 0)
  {
    users.push_back(user);
    invitedUsers.erase(user);
  }
  // Add other checks or error handling here as needed
}

void Channel::removeUser(const std::string& user)
{
  users.erase(std::remove(users.begin(), users.end(), user), users.end());
}

bool Channel::hasUser(const std::string& user) const
{
  return (std::find(users.begin(), users.end(), user) != users.end());
}

std::vector<std::string> Channel::getUsers() const
{
  return users;
}

void Channel::inviteUser(const std::string& inviter, const std::string& invitee)
{
  if (hasUser(inviter))
  {
    invitedUsers.insert(invitee);
  }
}

void Channel::addOperator(const std::string& user)
{
  operators.insert(user);
}

void Channel::removeOperator(const std::string& user)
{
  operators.erase(user);
}

bool Channel::isOperator(const std::string& user) const
{
  return (operators.count(user) > 0);
}
