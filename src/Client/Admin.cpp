#include "Admin.hpp"

Admin::Admin() : Client("ADMIN", "ADMINOVICH", "0", "0") {}

void Admin::BanClient(const std::string& first_name,
                      const std::string& last_name, sqlite3* db) {
  DeleteClientFromDB(first_name, last_name, db);
}

void Admin::PrintHistory(const int64_t& id, sqlite3* db) {
  char* sql = sqlite3_mprintf(
      "SELECT source_id, target_id, amount, timestamp FROM transactions WHERE "
      "source_id = %lld OR target_id = %lld;",
      id, id);
  sqlite3_stmt* stmt;
  int64_t error = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
  if (error != SQLITE_OK) {
    std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  std::cout << "Transaction history for account with ID: " << id << std::endl;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    int64_t source_id = sqlite3_column_int(stmt, 0);
    int64_t target_id = sqlite3_column_int(stmt, 1);
    double amount = sqlite3_column_double(stmt, 2);
    time_t timestamp = sqlite3_column_int(stmt, 3);
    std::cout << "Source ID: " << source_id << ", Target ID: " << target_id
              << ", Amount: " << amount << ", Timestamp: " << timestamp
              << std::endl;
  }
  sqlite3_finalize(stmt);
}

void Admin::BanAccount(const int64_t& id, sqlite3* db) {
  DeleteAccountFromDB(id, db);
}

bool Admin::DeleteAccountFromDB(const int64_t& id, sqlite3* db) {
  char* sql = sqlite3_mprintf("DELETE FROM accounts WHERE id = %lld;", id);
  int64_t error = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
  sqlite3_free(sql);
  return (error == SQLITE_OK);
}

bool Admin::DeleteClientFromDB(std::string first_name, std::string last_name,
                               sqlite3* db) {
  char* sql = sqlite3_mprintf(
      "DELETE FROM clients WHERE first_name = '%q' AND last_name = '%q';",
      first_name.c_str(), last_name.c_str());
  int64_t error = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
  sqlite3_free(sql);
  return (error == SQLITE_OK);
}
