#pragma once
#include <string>

class Transaction {
private:
    int id;
    std::string type;  // Deposit, Withdraw, Transfer
    double amount;
    std::string date;
    int accountId;

public:
    Transaction(int id, const std::string& type, double amount, const std::string& date, int accountId);

    int getId() const;
    std::string getType() const;
    double getAmount() const;
    std::string getDate() const;
    int getAccountId() const;
};
