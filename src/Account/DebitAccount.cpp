#include "DebitAccount.hpp"

void DebitAccount::Withdraw(double amount) {
  if (amount > Account::balance_) {
    std::cout << "Insufficient funds";
  } else {
    Account::balance_ -= amount;
  }
}
void DebitAccount::Deposit(double amount) { Account::balance_ += amount; }

DebitAccount::DebitAccount(double balance) : Account(balance) {}

DebitAccount::DebitAccount(int64_t id, double balance) : Account(id, balance) {}