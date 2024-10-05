#pragma once

#include <algorithm>
#include <iomanip>
#include <vector>

#include "Account/CreditAccount.hpp"
#include "Account/DebitAccount.hpp"
#include "Account/DepositAccount.hpp"
#include "Client/ClientBuilder.hpp"
#include "Transaction/DepositTransaction.hpp"
#include "Transaction/TransactionInvoker.hpp"
#include "Transaction/WithdrawTransaction.hpp"
#include "sqlite3.h"

class ProccessSQL {
 public:
  ProccessSQL();

  ~ProccessSQL();

  void Registration(std::string first_name, std::string last_name, std::string address, std::string passport);

  void Authorization(std::string first_name, std::string last_name);

  bool AddClient(const Client& client);

  bool ClientExists(const std::string& first_name, const std::string& last_name);

  std::shared_ptr<Client> GetClient(const std::string& first_name, const std::string& last_name);

  double GetBalance(int64_t id);

  bool IsAvailable() const;

  bool MakeTransaction(int64_t source_id, int64_t target_id, double amount);

  bool MakeTopUp(int64_t id, double amount);

  void CreateAccount(std::string type_of_account, double money, time_t time);

  void ProsecutingAccount();

  void GetAccounts(const Client& client);

  void UpdateAccountBalance();

  std::set<std::shared_ptr<Account>> GetAccountsSet();

  std::shared_ptr<Account> FindBalance(const int64_t& id);

  void CancelTransaction(int64_t source_id, int64_t target_id, double amount);

  void ExecuteTransactions();

  void BanClient(std::string first_name, std::string last_name);

  void BanAccount(int64_t id);

  std::string GetFirstName();

  std::string GetLastName();

  std::string GetAddress();

  std::string GetPassport();

  void SetAddressAndPassport(std::string address, std::string passport);

  bool IsAdmin();
   private:
  std::shared_ptr<Client> client_;
  std::set<std::shared_ptr<Account>> accounts_;
  TransactionInvoker transaction_invoker_;
  sqlite3* db;
};