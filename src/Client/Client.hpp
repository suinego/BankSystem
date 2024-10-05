#pragma once

#include <iostream>
#include <set>
#include <string>

class Client {
 public:
  Client();
  Client(std::string first_name, std::string last_name);
  Client(std::string first_name, std::string last_name, std::string address,
         std::string passport);
  Client operator=(const Client& other);

  bool operator==(const Client& other) const;

  std::string GetFirstName() const;

  std::string GetLastName() const;

  std::string GetAddress() const;

  std::string GetPassport() const;

 private:
  std::string first_name_;
  std::string last_name_;
  std::string address_;
  std::string passport_;
};