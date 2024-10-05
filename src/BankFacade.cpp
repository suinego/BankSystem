#include "BankFacade.hpp"

void BankFacade::Registration(ProccessSQL& sql_handler, std::string first_name, std::string last_name, std::string address, std::string passport) {
    sql_handler.Registration(first_name, last_name, address, passport);
}

void BankFacade::Authorization(ProccessSQL& sql_handler, std::string first_name, std::string last_name) {
    sql_handler.Authorization(first_name, last_name);
}

double BankFacade::Balance(ProccessSQL& sql_handler, int64_t id) {
    return sql_handler.GetBalance(id);
}

bool BankFacade::TopUp(ProccessSQL& sql_handler, int64_t id, double amount) {
  return sql_handler.MakeTopUp(id, amount);
}

bool BankFacade::Transaction(ProccessSQL& sql_handler, int64_t source_id, int64_t target_id, double amount) {
  return sql_handler.MakeTransaction(source_id, target_id, amount);
}

void BankFacade::CreateAccount(ProccessSQL& sql_handler, std::string type_of_account, double money, time_t time) {
  sql_handler.CreateAccount(type_of_account, money, time);
}

void BankFacade::CancelTransaction(ProccessSQL& sql_handler,int64_t source_id, int64_t target_id, double amount) {
      sql_handler.CancelTransaction(source_id, target_id, amount);
}

void BankFacade::Exit(ProccessSQL& sql_handler) {
    sql_handler.ExecuteTransactions();
    sql_handler.UpdateAccountBalance();
}

bool BankFacade::IsAdmin(ProccessSQL& sql_handler) {
    return sql_handler.IsAdmin();
}
