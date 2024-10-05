#include "DepositTransaction.hpp"

void DepositTransaction::ExecuteTransaction() {
  source->Deposit(amount_money);
  std::cout << "Depositing " << amount_money << " to account "
            << target->GetId() << '\n';
}

DepositTransaction::DepositTransaction(std::shared_ptr<Account>& account,
                                       double amount)
    : Transaction(account, account, amount) {}