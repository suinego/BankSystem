#include "DepositAccount.hpp"

DepositAccount::DepositAccount(double balance, time_t time)
    : Account(balance), time_(time) {}


DepositAccount::DepositAccount(int64_t id, double balance, time_t time) : Account(id, balance), time_(time) {}

void DepositAccount::Withdraw(double amount) {
  if (std::time(nullptr) < time_) {
    std::cout << "Please try again after " << time_ << '\n';
  } else {
    Account::balance_ -= amount;
  }
}

void DepositAccount::Deposit(double amount) {
  if (std::time(nullptr) < time_) {
    std::cout << "Please try again after " << time_ << '\n';
  } else {
    Account::balance_ += amount;
  }
}
