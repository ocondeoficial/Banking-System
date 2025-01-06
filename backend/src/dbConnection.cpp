#include "dbConnection.h"
#include <iostream>

DbConnection::DbConnection(const std::string& connInfo) {
    conn = PQconnectdb(connInfo.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection error: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        exit(1);
    }
}

DbConnection::~DbConnection() {
    if (conn != nullptr) {
        PQfinish(conn);
    }
}

PGresult* DbConnection::executeQuery(const std::string& query) {
    PGresult* res = PQexec(conn, query.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Query error: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        return nullptr;
    }
    return res;
}

PGresult* DbConnection::insertTransaction(const std::string& type, double amount, int accountId) {
    std::string query = "INSERT INTO transactions (type, amount, account_id) VALUES ('" + type + "', " + std::to_string(amount) + ", " + std::to_string(accountId) + ") RETURNING id;";
    return executeQuery(query);
}

PGresult* DbConnection::getTransactions(int accountId) {
    std::string query = "SELECT * FROM transactions WHERE account_id = " + std::to_string(accountId) + " ORDER BY date DESC;";
    return executeQuery(query);
}

PGresult* DbConnection::createAccount(const std::string& holder, const std::string& cpf, double initialBalance) {
    std::string query = "INSERT INTO accounts (holder, cpf, balance) VALUES ('" + holder + "', '" + cpf + "', " + std::to_string(initialBalance) + ") RETURNING id;";
    return executeQuery(query);
}

PGresult* DbConnection::findAccountByCpf(const std::string& cpf) {
    std::string query = "SELECT * FROM accounts WHERE cpf = '" + cpf + "';";
    return executeQuery(query);
}
