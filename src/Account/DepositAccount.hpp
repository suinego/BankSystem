#include <ctime>

#include "Account.hpp"

class DepositAccount : public Account {
 private:
  time_t time_;

 public:
  DepositAccount(double balance, time_t time);
  DepositAccount(int64_t id, double balance, time_t time);
  void Withdraw(double amount) final;
  void Deposit(double amount) final;
};