#pragma once

#include <memory>

#include "../Account/Account.hpp"

class Transaction {
 public:
  double amount_money;
  std::shared_ptr<Account> source;
  std::shared_ptr<Account> target;

  Transaction(std::shared_ptr<Account>& other_source,
              std::shared_ptr<Account>& other_target, double amount);

  virtual void ExecuteTransaction() = 0;
};