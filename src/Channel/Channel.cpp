#include "../include/Channel.hpp"



namespace AFG
{
  Channel::Channel(const std::string& name) : name(name) {}

  Channel::~Channel(){}

  Channel::Channel(const Channel &src)
  {

  }
  Channel &Channel::operator=(const Channel &src)
  {

  }

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

  void Channel::removeUser(Client& user)
  {
    users.erase(&user);
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


  bool Channel::isOperator(Client& user) const
  {
    return (operators.count(&user) > 0);
  }

  void Channel::makeInviteOnly(void)
  {
    this->inviteOnly = true;
  }

  void Channel::removeInviteOnly(void)
  {
    this->inviteOnly = false;
  }

  bool  Channel::isInviteOnly(void) const
  {
    return (this->inviteOnly);
  }

  void Channel::makeTopicOpOnly(void)
  {
    this->topicOpOnly = true;
  }

  void Channel::removeTopicOpOnly(void)
  {
    this->topicOpOnly = false;
  }

  bool  Channel::isTopicOpOnly(void) const
  {
    return (this->topicOpOnly);
  }

  void Channel::makeInviteOnly(void)
  {
    this->inviteOnly = true;
  }

  void Channel::removeInviteOnly(void)
  {
    this->inviteOnly = false;
  }

  bool  Channel::isInviteOnly(void) const
  {
    return (this->inviteOnly);
  }

  void Channel::makeTopicOpOnly(void)
  {
    this->topicOpOnly = true;
  }

  void Channel::removeTopicOpOnly(void)
  {
    this->topicOpOnly = false;
  }

  bool  Channel::isTopicOpOnly(void) const
  {
    return (this->topicOpOnly);
  }
}