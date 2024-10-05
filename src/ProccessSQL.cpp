#include "ProccessSQL.hpp"

ProccessSQL::ProccessSQL() {
  int64_t error = sqlite3_open("src/bank.db", &db);
  if (error) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << '\n';
    sqlite3_close(db);
    exit(1);
  }

  const char sql_clients[] =
      "CREATE TABLE IF NOT EXISTS clients ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
      "first_name TEXT NOT NULL,"
      "last_name TEXT NOT NULL,"
      "address TEXT NOT NULL,"
      "passport TEXT NOT NULL"
      ");";

  error = sqlite3_exec(db, sql_clients, nullptr, nullptr, nullptr);
  if (error != SQLITE_OK) {
    std::cerr << "SQL error: " << sqlite3_errmsg(db) << '\n';
    sqlite3_close(db);
    exit(1);
  }

  const char sql_accounts[] =
      "CREATE TABLE IF NOT EXISTS accounts ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
      "type TEXT NOT NULL,"
      "first_name TEXT NOT NULL,"
      "last_name TEXT NOT NULL,"
      "balance REAL NOT NULL,"
      "date DATA NOT NULL"
      ");";

  error = sqlite3_exec(db, sql_accounts, nullptr, nullptr, nullptr);
  if (error != SQLITE_OK) {
    std::cerr << "SQL error: " << sqlite3_errmsg(db) << '\n';
    sqlite3_close(db);
    exit(1);
  }
}

ProccessSQL::~ProccessSQL() { sqlite3_close(db); }

void ProccessSQL::Registration(std::string first_name, std::string last_name,
                               std::string address, std::string passport) {
  ClientBuilder builder;
  accounts_.clear();
  client_ =
      builder.InputClientInformation(first_name, last_name, address, passport);
  if (!ClientExists(client_->GetFirstName(), client_->GetLastName())) {
    AddClient(*client_);
  }
}

void ProccessSQL::Authorization(std::string first_name, std::string last_name) {
  if (first_name == "ADMIN" && last_name == "ADMINOVICH") {
    client_ = std::make_shared<Admin>();
    GetAccounts(*client_);
  } else {
    client_ = GetClient(first_name, last_name);
    if (client_ != std::make_shared<Client>()) {
      GetAccounts(*client_);
    }
  }
}

bool ProccessSQL::AddClient(const Client& client) {
  char* sql = sqlite3_mprintf(
      "INSERT INTO clients (first_name, last_name, address, passport) "
      "VALUES ('%q', '%q', '%q', '%q');",
      client.GetFirstName().c_str(), client.GetLastName().c_str(),
      client.GetAddress().c_str(), client.GetPassport().c_str());
  int64_t error = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
  sqlite3_free(sql);
  if (error != SQLITE_OK) {
    std::cerr << "SQL error: " << sqlite3_errmsg(db) << '\n';
    return false;
  }
  return true;
}

bool ProccessSQL::ClientExists(const std::string& first_name,
                               const std::string& last_name) {
  std::string query = "SELECT COUNT(*) FROM clients WHERE first_name='" +
                      first_name + "' AND last_name='" + last_name + "';";

  sqlite3_stmt* stmt;
  int64_t error = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

  if (error != SQLITE_OK) {
    std::cerr << "Error while preparing request: " << sqlite3_errmsg(db)
              << std::endl;
    return false;
  }

  error = sqlite3_step(stmt);
  if (error != SQLITE_ROW) {
    std::cerr << "Error while preparing request: " << sqlite3_errmsg(db)
              << std::endl;
    sqlite3_finalize(stmt);
    return false;
  }
  int64_t count = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);
  return count > 0;
}

bool ProccessSQL::IsAvailable() const { return (client_ == nullptr); }

std::shared_ptr<Client> ProccessSQL::GetClient(const std::string& first_name,
                                               const std::string& last_name) {
  std::string query =
      "SELECT id, first_name, last_name FROM clients WHERE first_name = '" +
      first_name + "' AND last_name = '" + last_name + "';";
  sqlite3_stmt* stmt;
  int64_t error = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
  if (error != SQLITE_OK) {
    std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    return nullptr;
  }
  error = sqlite3_step(stmt);
  if (error == SQLITE_ROW) {
    std::string first_name =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    std::string last_name =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    sqlite3_finalize(stmt);
    return std::make_shared<Client>(first_name, last_name, "", "");
  }
  sqlite3_finalize(stmt);
  return std::make_shared<Client>();
}

double ProccessSQL::GetBalance(int64_t id) {
  std::string query =
      "SELECT balance FROM accounts WHERE id = " + std::to_string(id) + ";";

  sqlite3_stmt* stmt;
  int64_t error = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

  if (error != SQLITE_OK) {
    std::cerr << "Error while preparing request: " << sqlite3_errmsg(db)
              << '\n';
    return -1;
  }

  error = sqlite3_step(stmt);
  if (error != SQLITE_ROW) {
    std::cerr << "Error while executing request: " << sqlite3_errmsg(db)
              << '\n';
    sqlite3_finalize(stmt);
    return -1;
  }

  double balance = sqlite3_column_double(stmt, 0);

  sqlite3_finalize(stmt);

  return balance;
}

bool ProccessSQL::MakeTransaction(int64_t source_id, int64_t target_id,
                                  double amount) {
  ProsecutingAccount();
  std::shared_ptr<Account> source_account = FindBalance(source_id);
  std::shared_ptr<Account> target_account = FindBalance(target_id);
  if (source_account && target_account) {
    std::shared_ptr<Transaction> transaction =
        std::make_shared<WithdrawTransaction>(source_account, target_account,
                                              amount);
    transaction_invoker_.AddTransaction(transaction);
    return true;
  }
  return false;
}

bool ProccessSQL::MakeTopUp(int64_t id, double amount) {
  ProsecutingAccount();
  std::shared_ptr<Account> account = FindBalance(id);
  if (account) {
    std::shared_ptr<Transaction> transaction =
        std::make_shared<DepositTransaction>(account, amount);
    transaction_invoker_.AddTransaction(transaction);
    return true;
  }
  return false;
}

std::shared_ptr<Account> ProccessSQL::FindBalance(const int64_t& id) {
  double balance = GetBalance(id);

  auto it = std::find_if(
      accounts_.begin(), accounts_.end(),
      [&](const std::shared_ptr<Account>& acc) { return acc->GetId() == id; });
  if (it != accounts_.end()) {
    return *it;
  } else {
    std::cout << "Account with ID " << id << " not found." << '\n';
    return nullptr;
  }
}

void ProccessSQL::CreateAccount(std::string type_of_account, double money,
                                time_t time) {
  int64_t id_account;
  if (type_of_account == "Credit") {
    std::shared_ptr<Account> account_ptr =
        std::make_shared<CreditAccount>(money);
    accounts_.insert(account_ptr);
    id_account = account_ptr->GetId();
  } else if (type_of_account == "Debit") {
    std::shared_ptr<Account> account_ptr =
        std::make_shared<DebitAccount>(money);
    accounts_.insert(account_ptr);
    id_account = account_ptr->GetId();
  } else if (type_of_account == "Deposit") {
    std::shared_ptr<Account> account_ptr =
        std::make_shared<DepositAccount>(money, time);
    accounts_.insert(account_ptr);
    id_account = account_ptr->GetId();
  }
  std::string query =
      "INSERT INTO accounts (id, type, first_name, last_name, balance, date) "
      "VALUES ('" +
      std::to_string(id_account) + "', '" + type_of_account + "', '" +
      GetFirstName() + "',  '" + GetLastName() + "',  '" +
      std::to_string(money) + "', '" + std::to_string(time) + "');";
  int64_t error = sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);
  if (error != SQLITE_OK) {
    std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
  }
}

void ProccessSQL::ProsecutingAccount() {
  if (client_->GetAddress() == "" || client_->GetPassport() == "") {
    std::cout << "Please provide more details" << '\n';
    if (client_->GetAddress() == "" && client_->GetPassport() != "") {
      std::cout << "Enter address:" << '\n';
      std::string address;
      std::cin >> address;
      client_ = std::make_shared<Client>(client_->GetFirstName(),
                                         client_->GetLastName(), address,
                                         client_->GetPassport());
    } else if (client_->GetPassport() == "" && client_->GetAddress() != "") {
      std::cout << "Enter the passport information" << '\n';
      std::string passport;
      std::cin >> passport;
      client_ = std::make_shared<Client>(client_->GetFirstName(),
                                         client_->GetLastName(),
                                         client_->GetAddress(), passport);
    } else if (client_->GetAddress() == "" && client_->GetPassport() == "") {
      std::cout
          << "Give me your passport and address information (he-he-he +credit)"
          << '\n';
      std::string address;
      std::string passport;
      std::cout << "Please, enter the passport information" << '\n';
      std::cin >> passport;
      std::cout << "Please, enter the address information" << '\n';
      std::cin >> address;
      client_ = std::make_shared<Client>(
          client_->GetFirstName(), client_->GetLastName(), address, passport);
    }
  }
}

void ProccessSQL::GetAccounts(const Client& client) {
  accounts_.clear();
  std::string query =
      "SELECT id, balance, type, date FROM accounts WHERE first_name = '" +
      client.GetFirstName() + "' AND last_name = '" + client.GetLastName() +
      "';";
  sqlite3_stmt* stmt;
  int64_t error = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
  if (error != SQLITE_OK) {
    std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
  }
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    int64_t id = sqlite3_column_int(stmt, 0);
    double balance = sqlite3_column_double(stmt, 1);
    const unsigned char* type_str = sqlite3_column_text(stmt, 2);
    std::string type = reinterpret_cast<const char*>(type_str);
    if (type == "Credit") {
      std::shared_ptr<Account> account_ptr =
          std::make_shared<CreditAccount>(id, balance);
      accounts_.insert(account_ptr);
    } else if (type == "Debit") {
      std::shared_ptr<Account> account_ptr =
          std::make_shared<DebitAccount>(id, balance);
      accounts_.insert(account_ptr);
    } else if (type == "Deposit") {
      time_t time = sqlite3_column_int(stmt, 3);
      std::shared_ptr<Account> account_ptr =
          std::make_shared<DepositAccount>(id, balance, time);

      accounts_.insert(account_ptr);
    }
  }
  sqlite3_finalize(stmt);
}

void ProccessSQL::UpdateAccountBalance() {
  for (const auto& account : accounts_) {
    int64_t id = account->GetId();
    double balance = account->GetBalance();
    std::string type;
    if (dynamic_cast<CreditAccount*>(account.get())) {
      type = "Credit";
    } else if (dynamic_cast<DebitAccount*>(account.get())) {
      type = "Debit";
    } else if (dynamic_cast<DepositAccount*>(account.get())) {
      type = "Deposit";
    }

    char* sql = sqlite3_mprintf(
        "UPDATE accounts SET balance = %f WHERE id = %lld;", balance, id);

    int64_t error = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
    if (error != SQLITE_OK) {
      std::cerr << "SQL error: " << sqlite3_errmsg(db) << '\n';
    }
    sqlite3_free(sql);
  }
}

void ProccessSQL::CancelTransaction(int64_t source_id, int64_t target_id,
                                    double amount) {
  transaction_invoker_.CancelTransaction(source_id, target_id, amount);
}

void ProccessSQL::ExecuteTransactions() {
  transaction_invoker_.ExecuteTransactions();
}

bool ProccessSQL::IsAdmin() {
  return client_->GetFirstName() == "Admin" &&
         client_->GetLastName() == "Adminovich";
}

void ProccessSQL::BanClient(std::string first_name, std::string last_name) {
  static_cast<Admin*>(client_.get())->BanClient(first_name, last_name, db);
}

void ProccessSQL::BanAccount(int64_t id) {
  static_cast<Admin*>(client_.get())->BanAccount(id, db);
}

std::set<std::shared_ptr<Account>> ProccessSQL::GetAccountsSet() {
  return accounts_;
}

std::string ProccessSQL::GetFirstName() { return client_->GetFirstName(); }

std::string ProccessSQL::GetLastName() { return client_->GetLastName(); }

std::string ProccessSQL::GetAddress() { return client_->GetAddress(); }

std::string ProccessSQL::GetPassport() { return client_->GetPassport(); }

void ProccessSQL::SetAddressAndPassport(std::string address,
                                        std::string passport) {
  client_ = std::make_shared<Client>(client_->GetFirstName(),
                                     client_->GetLastName(), address, passport);
}
