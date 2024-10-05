#include "ClientBuilder.hpp"

#include <iostream>
#include <string>

std::shared_ptr<Client> ClientBuilder::InputClientInformation(std::string first_name, std::string last_name, std::string address, std::string passport) {
  first_name_ = first_name;
  last_name_ = last_name;
  address_ = address;
  passport_ = passport;
  if (first_name_ == "Admin" && last_name_ == "Adminovich") {
    return std::make_shared<Admin>();
  } else if (passport_.empty() && address_.empty()) {
    return std::make_shared<Client>(first_name_, last_name_);
  } else {
    return std::make_shared<Client>(first_name_, last_name_, address_,
                                    passport_);
  }
}