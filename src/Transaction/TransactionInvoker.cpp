#include "TransactionInvoker.hpp"

void TransactionInvoker::AddTransaction(
    std::shared_ptr<Transaction> transaction) {
  transactions_.insert(transaction);
}

void TransactionInvoker::ExecuteTransactions() {
  for (auto transaction : transactions_) {
    transaction->ExecuteTransaction();
  }
  transactions_.clear();
}

void TransactionInvoker::CancelTransaction(int64_t id_source, int64_t id_target, int64_t amount) {
  size_t num_transactions = transactions_.size();
  for (auto transaction : transactions_) {
    if (transaction->source->GetId() == id_source &&
        transaction->target->GetId() == id_target &&
        transaction->amount_money == amount) {
      transactions_.erase(transaction);
    }
  }
  if (transactions_.size() == num_transactions) {
    std::cout << "Transaction not found" << '\n';
    return;
  }
}