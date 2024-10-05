#include <iostream>
#include <string>
#include <vector>

#pragma once

class Account {
 public:
  Account() = default;

  Account(double money);

  Account(int64_t id, double amount);

  int64_t CreateId();

  int64_t GetId() const;
  double GetBalance();
  virtual void Withdraw(double amount) = 0;
  virtual void Deposit(double amount) = 0;

 protected:
  int64_t id_;
  double balance_;
};