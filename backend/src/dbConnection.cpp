#include "dbConnection.h"
#include <iostream>

// Construtor
DbConnection::DbConnection(const std::string& connInfo) {
    conn = PQconnectdb(connInfo.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection error: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        exit(1);
    }
    std::cout << "Database connection established successfully." << std::endl;
}

// Destrutor
DbConnection::~DbConnection() {
    if (conn) {
        PQfinish(conn);
    }
}

// Executar consultas genéricas
PGresult* DbConnection::executeQuery(const std::string& query) {
    PGresult* res = PQexec(conn, query.c_str());
    if (!res || (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK)) {
        std::cerr << "Query error: " << PQerrorMessage(conn) << std::endl;
        if (res) PQclear(res);
        return nullptr;
    }
    return res;
}

// Criar uma conta
PGresult* DbConnection::createAccount(const std::string& holder, const std::string& cpf, const std::string& hashedPassword, double initialBalance) {
    const char* paramValues[4] = {holder.c_str(), cpf.c_str(), hashedPassword.c_str(), std::to_string(initialBalance).c_str()};
    const char* query = 
        "INSERT INTO accounts (holder, cpf, password, balance) "
        "VALUES ($1, $2, $3, $4::numeric) RETURNING id";

    PGresult* res = PQexecParams(conn, query, 4, nullptr, paramValues, nullptr, nullptr, 0);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Error creating account: " << PQerrorMessage(conn) << std::endl;
        if (res) PQclear(res);
        return nullptr;
    }
    return res;
}

// Buscar conta por CPF
PGresult* DbConnection::findAccountByCpf(const std::string& cpf) {
    const char* paramValues[1] = {cpf.c_str()};
    const char* query = 
        "SELECT * FROM accounts WHERE cpf = $1";

    PGresult* res = PQexecParams(conn, query, 1, nullptr, paramValues, nullptr, nullptr, 0);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Error finding account by CPF: " << PQerrorMessage(conn) << std::endl;
        if (res) PQclear(res);
        return nullptr;
    }
    return res;
}

// Atualizar saldo da conta
PGresult* DbConnection::updateBalance(const std::string& cpf, double newBalance) {
    const char* paramValues[2] = {std::to_string(newBalance).c_str(), cpf.c_str()};
    const char* query = 
        "UPDATE accounts SET balance = $1::numeric WHERE cpf = $2 RETURNING balance";

    PGresult* res = PQexecParams(conn, query, 2, nullptr, paramValues, nullptr, nullptr, 0);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Error updating balance: " << PQerrorMessage(conn) << std::endl;
        if (res) PQclear(res);
        return nullptr;
    }
    return res;
}

// Inserir transação
PGresult* DbConnection::insertTransaction(const std::string& type, double amount, int accountId, const std::string& fromCpf, const std::string& toCpf) {
    const char* paramValues[5] = {
        type.c_str(), 
        std::to_string(amount).c_str(), 
        std::to_string(accountId).c_str(), 
        fromCpf.empty() ? nullptr : fromCpf.c_str(), 
        toCpf.empty() ? nullptr : toCpf.c_str()
    };
    const int paramFormats[5] = {0, 0, 0, 0, 0};

    const char* query = 
        "INSERT INTO transactions (type, amount, account_id, from_cpf, to_cpf) "
        "VALUES ($1, $2::numeric, $3::int, $4, $5) RETURNING id";

    PGresult* res = PQexecParams(conn, query, 5, nullptr, paramValues, nullptr, paramFormats, 0);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Error inserting transaction: " << PQerrorMessage(conn) << std::endl;
        if (res) PQclear(res);
        return nullptr;
    }
    std::cout << "Transaction inserted successfully." << std::endl;
    return res;
}

// Buscar transações por conta
PGresult* DbConnection::getTransactions(int accountId) {
    const char* paramValues[1] = {std::to_string(accountId).c_str()};
    const char* query = 
        "SELECT transactions.id, transactions.type, transactions.amount, transactions.date, transactions.account_id, "
        "COALESCE(from_acc.cpf, transactions.from_cpf, '') AS from_cpf, "
        "COALESCE(to_acc.cpf, transactions.to_cpf, '') AS to_cpf "
        "FROM transactions "
        "LEFT JOIN accounts from_acc ON transactions.from_cpf = from_acc.cpf "
        "LEFT JOIN accounts to_acc ON transactions.to_cpf = to_acc.cpf "
        "WHERE transactions.account_id = $1::int OR transactions.from_cpf IS NOT NULL "
        "ORDER BY transactions.date DESC";

    PGresult* res = PQexecParams(conn, query, 1, nullptr, paramValues, nullptr, nullptr, 0);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Error retrieving transactions: " << PQerrorMessage(conn) << std::endl;
        if (res) PQclear(res);
        return nullptr;
    }
    return res;
}

// Buscar todas as transações
PGresult* DbConnection::getAllTransactions() {
    const char* query = 
        "SELECT transactions.id, transactions.type, transactions.amount, transactions.date, transactions.account_id, "
        "COALESCE(from_acc.cpf, transactions.from_cpf, '') AS from_cpf, "
        "COALESCE(to_acc.cpf, transactions.to_cpf, '') AS to_cpf "
        "FROM transactions "
        "LEFT JOIN accounts from_acc ON transactions.from_cpf = from_acc.cpf "
        "LEFT JOIN accounts to_acc ON transactions.to_cpf = to_acc.cpf "
        "ORDER BY transactions.date DESC";

    PGresult* res = PQexec(conn, query);
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Error retrieving all transactions: " << PQerrorMessage(conn) << std::endl;
        if (res) PQclear(res);
        return nullptr;
    }
    return res;
}
