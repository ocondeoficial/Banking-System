#pragma once
#include <string>
#include "../models/account.h"
#include "../include/dbConnection.h"

class AccountService {
private:
    DbConnection dbConnection;

public:
    AccountService();

    bool createAccount(const std::string& holder, const std::string& cpf, const std::string& hashedPassword, double initialBalance);
    Account* findAccountByCpf(const std::string& cpf);
    bool validateLogin(const std::string& cpf, const std::string& password);
    bool deposit(const std::string& cpf, double amount);
    bool withdraw(const std::string& cpf, double amount);
    bool transfer(const std::string& fromCpf, const std::string& toCpf, double amount);
};
