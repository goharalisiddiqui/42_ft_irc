#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#pragma once

#include "libs.hpp"
#include "Client.hpp"


namespace AFG
{


class Client;

class Channel
{
  public:
    // Constructor to create a new channel with a given name
    Channel(const std::string& name);
    ~Channel();

    Channel(const Channel &src);
    Channel &operator=(const Channel &src);



    /////////////// GETTERS ///////////////////
    // Get the name of the channel
    std::string getName() const;

    // Get a list of all the users in the channel
    std::set<Client*> getUsers() const;
    std::set<Client*> getOperators() const;
    std::set<Client*> getInvitedUsers() const;


    // Add a user to the channel
    void addUser(Client& user);

    // Remove a user from the channel
    void removeUser(Client& user);

    // Check if a user is in the channel
    bool hasUser(const Client& user) const;


    // Invite a user to the channel
    void inviteUser(const Client& inviter, Client& invitee);

    // Add an operator to the channel
    void addOperator(Client& user);

    // Remove an operator from the channel
    void removeOperator(Client& user);

    // Check if a user is an operator in the channel
    bool isOperator(Client& user) const;

    void spreadmsgfrom(Client &speaker, std::string &msg) const;
    
  // Make channel invite only
  void makeInviteOnly(void);

  // Make channel open for all
  void removeInviteOnly(void);

  // Check if channel is invite only
  bool isInviteOnly(void) const;

  // Make topic of channel changeable only by operators
  void makeTopicOpOnly(void);

  // Make topic of channel changeable by all
  void removeTopicOpOnly(void);

  // Check if channel is changeable only by operators
  bool isTopicOpOnly(void) const;

  // Check if channel is invite only
  bool isInviteOnly(void) const;

  // Make topic of channel changeable only by operators
  void makeTopicOpOnly(void);

  // Make topic of channel changeable by all
  void removeTopicOpOnly(void);

  // Check if channel is changeable only by operators
  bool isTopicOpOnly(void) const;

private:
  std::string name;
  std::set<Client*> users;
  std::set<Client*> operators;
  std::set<Client*> invited_users;
  bool inviteOnly;
  bool topicOpOnly;
};

#endif