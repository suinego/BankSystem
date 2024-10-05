#include "Client.hpp"
#include "sqlite3.h"

class Admin : public Client {
 public:
  Admin();

  void BanClient(const std::string& first_name, const std::string& last_name,
                 sqlite3* db);

  void PrintHistory(const int64_t& id, sqlite3* db);

  void BanAccount(const int64_t& id, sqlite3* db);

 private:
  bool DeleteAccountFromDB(const int64_t& id, sqlite3* db);

  bool DeleteClientFromDB(std::string first_name, std::string last_name,
                          sqlite3* db);
};