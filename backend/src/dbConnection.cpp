#include "dbConnection.h"
#include <iostream>

DbConnection::DbConnection(const std::string& connInfo) {
    conn = PQconnectdb(connInfo.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection error: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        exit(1);
    }
    std::cout << "Database connection established successfully." << std::endl;
}

DbConnection::~DbConnection() {
    if (conn != nullptr) {
        PQfinish(conn);
    }
}

PGresult* DbConnection::executeQuery(const std::string& query) {
    PGresult* res = PQexec(conn, query.c_str());
    if (!res || (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK)) {
        std::cerr << "Query error: " << PQerrorMessage(conn) << std::endl;
        if (res) PQclear(res);
        return nullptr;
    }
    return res;
}

PGresult* DbConnection::createAccount(const std::string& holder, const std::string& cpf, const std::string& hashedPassword, double initialBalance) {
    const char* paramValues[4] = {holder.c_str(), cpf.c_str(), hashedPassword.c_str(), std::to_string(initialBalance).c_str()};
    const char* query = "INSERT INTO accounts (holder, cpf, password, balance) VALUES ($1, $2, $3, $4::numeric) RETURNING id;";

    PGresult* res = PQexecParams(conn, query, 4, nullptr, paramValues, nullptr, nullptr, 0);

    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Error creating account: " << PQerrorMessage(conn) << std::endl;
        if (res) PQclear(res);
        return nullptr;
    }

    return res;
}


PGresult* DbConnection::findAccountByCpf(const std::string& cpf) {
    const char* paramValues[1] = {cpf.c_str()};
    const char* query = "SELECT * FROM accounts WHERE cpf = $1;";
    PGresult* res = PQexecParams(conn, query, 1, nullptr, paramValues, nullptr, nullptr, 0);

    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Error finding account by CPF: " << PQerrorMessage(conn) << std::endl;
        if (res) PQclear(res);
        return nullptr;
    }

    return res;
}

PGresult* DbConnection::insertTransaction(const std::string& type, double amount, int accountId) {
    const char* paramValues[3] = {type.c_str(), std::to_string(amount).c_str(), std::to_string(accountId).c_str()};
    const char* query = "INSERT INTO transactions (type, amount, account_id) VALUES ($1, $2::numeric, $3::int) RETURNING id;";
    PGresult* res = PQexecParams(conn, query, 3, nullptr, paramValues, nullptr, nullptr, 0);

    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Error inserting transaction: " << PQerrorMessage(conn) << std::endl;
        if (res) PQclear(res);
        return nullptr;
    }

    std::cout << "Transaction inserted successfully." << std::endl;
    return res;
}

PGresult* DbConnection::getTransactions(int accountId) {
    const char* paramValues[1] = {std::to_string(accountId).c_str()};
    const char* query = "SELECT * FROM transactions WHERE account_id = $1::int ORDER BY date DESC;";
    PGresult* res = PQexecParams(conn, query, 1, nullptr, paramValues, nullptr, nullptr, 0);

    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Error retrieving transactions: " << PQerrorMessage(conn) << std::endl;
        if (res) PQclear(res);
        return nullptr;
    }

    return res;
}
