#include <memory>
#include <set>
#include <unordered_set>

#include "Transaction.hpp"

class TransactionInvoker {
 private:
  std::set<std::shared_ptr<Transaction>> transactions_;

 public:
  void AddTransaction(std::shared_ptr<Transaction> transaction);

  void ExecuteTransactions();

  void CancelTransaction(int64_t id_source, int64_t id_target, int64_t amount);
};