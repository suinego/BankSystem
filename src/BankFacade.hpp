#include "ProccessSQL.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <vector>

class BankFacade {
public:
    BankFacade() = default;

    ~BankFacade() = default;

    void Registration(ProccessSQL& sql_handler, std::string first_name, std::string last_name, std::string address, std::string passport);

    void Authorization(ProccessSQL& sql_handler, std::string first_name, std::string last_name);

    double Balance(ProccessSQL& sql_handler, int64_t id);

    bool TopUp(ProccessSQL& sql_handler, int64_t id, double amount);

    bool Transaction(ProccessSQL& sql_handler, int64_t source_id, int64_t target_id, double amount);

    void CreateAccount(ProccessSQL& sql_handler, std::string type_of_account, double money, time_t time = 0);

    void CancelTransaction(ProccessSQL& sql_handler, int64_t source_id, int64_t target_id, double amount);

    void Exit(ProccessSQL& sql_handler);

    bool IsAdmin(ProccessSQL& sql_handler);

};