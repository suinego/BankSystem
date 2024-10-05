#include "Account.hpp"

class DebitAccount : public Account {
 public:
  DebitAccount();
  DebitAccount(double balance);
  DebitAccount(int64_t id, double balance);
  void Withdraw(double amount) final;
  void Deposit(double amount) final;
};