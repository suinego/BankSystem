#include "Account.hpp"

class CreditAccount : public Account {
 private:
  double penalty;

 public:
  CreditAccount(double amount);
  CreditAccount(int64_t id, double amount);
  void Withdraw(double amount) final;
  void Deposit(double amount) final;
};