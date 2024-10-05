#include "CreditAccount.hpp"

CreditAccount::CreditAccount(double amount)
    : penalty(0.005 * balance_), Account(amount) {}


CreditAccount::CreditAccount(int64_t id, double amount)
    : penalty(0.005 * balance_), Account(id, amount) {}

void CreditAccount::Withdraw(double amount) {
  if (Account::balance_ - amount < 0.0) {
    Account::balance_ -= amount;
  } else {
    Account::balance_ -= (amount + penalty);
  }
}

void CreditAccount::Deposit(double amount) { balance_ += amount; }