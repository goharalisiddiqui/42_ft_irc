#include <string>
#include <vector>
#include <set>
#include <algorithm>

class Channel
{
public:
  // Constructor to create a new channel with a given name
  Channel(const std::string& name);

  // Get the name of the channel
  std::string getName() const;

  // Add a user to the channel
  void addUser(const std::string& user);

  // Remove a user from the channel
  void removeUser(const std::string& user);

  // Check if a user is in the channel
  bool hasUser(const std::string& user) const;

  // Get a list of all the users in the channel
  std::vector<std::string> getUsers() const;

  // Invite a user to the channel
  void inviteUser(const std::string& inviter, const std::string& invitee);

  // Add an operator to the channel
  void addOperator(const std::string& user);

  // Remove an operator from the channel
  void removeOperator(const std::string& user);

  // Check if a user is an operator in the channel
  bool isOperator(const std::string& user) const;

private:
  std::string name;
  std::vector<std::string> users;
  std::set<std::string> invitedUsers;
  std::set<std::string> operators;
};
