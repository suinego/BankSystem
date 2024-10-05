#include <iostream>

#include "Transaction.hpp"

class DepositTransaction : public Transaction {
 public:
  DepositTransaction(std::shared_ptr<Account>& account, double amount);
  void ExecuteTransaction() final;
};