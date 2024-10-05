#include "Account.hpp"

#include <random>

Account::Account(double money) : id_(CreateId()), balance_(money) {}

Account::Account(int64_t id, double amount) : id_(id), balance_(amount) {}

int64_t Account::CreateId() {
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<int64_t> dis(10000000, 99999999);
  int64_t id = dis(gen);
  return id;
}

int64_t Account::GetId() const { return id_; }

double Account::GetBalance() { return balance_; }