#pragma once
#include <string>
#include <libpq-fe.h>

class DbConnection {
public:
    DbConnection(const std::string& connInfo);
    ~DbConnection();

    PGresult* executeQuery(const std::string& query);
    PGresult* insertTransaction(const std::string& type, double amount, int accountId);
    PGresult* getTransactions(int accountId);
    PGresult* createAccount(const std::string& holder, const std::string& cpf, double initialBalance);
    PGresult* findAccountByCpf(const std::string& cpf);

private:
    PGconn* conn;
};
