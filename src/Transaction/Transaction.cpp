#include "Transaction.hpp"

Transaction::Transaction(std::shared_ptr<Account>& other_source,
                         std::shared_ptr<Account>& other_target, double amount)
    : source(std::move(other_source)),
      target(std::move(other_target)),
      amount_money(amount) {}