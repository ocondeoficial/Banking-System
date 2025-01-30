#pragma once
#include <string>
#include <libpq-fe.h>

class DbConnection {
public:
    DbConnection(const std::string& connInfo);
    ~DbConnection();

    PGresult* executeQuery(const std::string& query);
    PGresult* insertTransaction(const std::string& type, double amount, int accountId, const std::string& fromCpf = "", const std::string& toCpf = "");
    PGresult* getTransactions(int accountId);
    PGresult* createAccount(const std::string& holder, const std::string& cpf, const std::string& hashedPassword, double initialBalance);
    PGresult* findAccountByCpf(const std::string& cpf);
    PGresult* updateBalance(const std::string& cpf, double newBalance);
    PGresult* getAllTransactions();


private:
    PGconn* conn;
};
