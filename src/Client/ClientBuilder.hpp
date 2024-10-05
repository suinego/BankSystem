#include <memory>
#include <string>

#include "Admin.hpp"
#include "Client.hpp"

class ClientBuilder {
 public:
  std::shared_ptr<Client> InputClientInformation(std::string first_name, std::string last_name, std::string address, std::string passport);
  void CreateAccount();

 private:
  std::string first_name_;
  std::string last_name_;
  std::string address_;
  std::string passport_;
};