#pragma once
#include <string>

class Account {
private:
    int id;
    std::string holder;
    std::string cpf;
    double balance;

public:
    Account(int id, const std::string& holder, const std::string& cpf, double initialBalance);

    int getId() const;
    std::string getHolder() const;
    std::string getCpf() const;
    double getBalance() const;

    bool deposit(double amount);
    bool withdraw(double amount);
};
