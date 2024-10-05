#include "WithdrawTransaction.hpp"

void WithdrawTransaction::ExecuteTransaction() {
  source->Withdraw(amount_money);
  target->Deposit(amount_money);
}

WithdrawTransaction::WithdrawTransaction(std::shared_ptr<Account>& other_source,
                                         std::shared_ptr<Account>& other_target,
                                         double amount)
    : Transaction(other_source, other_target, amount) {}