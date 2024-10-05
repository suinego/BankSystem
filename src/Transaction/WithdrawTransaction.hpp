#include <iostream>

#include "Transaction.hpp"

class WithdrawTransaction : public Transaction {
 public:
  WithdrawTransaction(std::shared_ptr<Account>& other_source,
                      std::shared_ptr<Account>& other_target, double amount);
  void ExecuteTransaction() final;
};