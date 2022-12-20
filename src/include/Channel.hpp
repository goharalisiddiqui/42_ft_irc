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




    // Get the name of the channel
    std::string getName() const;

    // Add a user to the channel
    void addUser(Client& user);

    // Remove a user from the channel
    void removeUser(Client& user);

    // Check if a user is in the channel
    bool hasUser(const Client& user) const;

    // Get a list of all the users in the channel
    std::set<Client*> getUsers() const;

    // Invite a user to the channel
    void inviteUser(const Client& inviter, Client& invitee);

    // Add an operator to the channel
    void addOperator(Client& user);

    // Remove an operator from the channel
    void removeOperator(Client& user);

    // Check if a user is an operator in the channel
    bool isOperator(Client& user) const;

    void makeInviteOnly(void);
    void removeInviteOnly(void);

  private:
    std::string name;
    std::set<Client*> users;
    std::set<Client*> operators;
    std::set<Client*> invited_users;
    bool inviteOnly;
  };
}

#endif