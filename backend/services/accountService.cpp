#include "accountService.h"
#include <iostream>
#include "../services/hashUtils.h"
#include <vector>
#include "../models/transaction.h"
#include <iomanip>
#include <sstream>

// ------------------ MÉTODO PARA FORMATAÇÃO DE DATA ------------------
std::string AccountService::formatDate(const std::string& rawDate) {
    struct tm tm = {};
    std::istringstream ss(rawDate);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    std::ostringstream formattedDate;
    formattedDate << std::put_time(&tm, "%d/%m/%Y %H:%M:%S");
    return formattedDate.str();
}

// ------------------ CONSTRUTOR DA CLASSE ------------------
AccountService::AccountService() : dbConnection(std::getenv("DB_CONN_INFO")) {
    if (!std::getenv("DB_CONN_INFO")) {
        std::cerr << "Error: DB_CONN_INFO environment variable is not set." << std::endl;
        exit(1);
    }
}

// ------------------ CRIAÇÃO DE CONTA ------------------
bool AccountService::createAccount(const std::string& holder, const std::string& cpf, const std::string& hashedPassword, double initialBalance) {
    PGresult* result = dbConnection.createAccount(holder, cpf, hashedPassword, initialBalance);
    if (result) {
        std::cout << "Account created successfully: " << holder << " (CPF: " << cpf << ")" << std::endl;
        PQclear(result);
        return true;
    }
    return false;
}

// ------------------ VALIDAÇÃO DE LOGIN ------------------
bool AccountService::validateLogin(const std::string& cpf, const std::string& password) {
    PGresult* result = dbConnection.findAccountByCpf(cpf);
    if (result && PQntuples(result) > 0) {
        std::string storedHash = PQgetvalue(result, 0, 3);
        PQclear(result);
        return validatePassword(password, storedHash);
    }
    PQclear(result);
    return false;
}

// ------------------ BUSCAR CONTA PELO CPF ------------------
Account* AccountService::findAccountByCpf(const std::string& cpf) {
    PGresult* result = dbConnection.findAccountByCpf(cpf);
    if (result && PQntuples(result) > 0) {
        int id = std::stoi(PQgetvalue(result, 0, 0));  
        std::string holder = PQgetvalue(result, 0, 1); 
        std::string cpfStored = PQgetvalue(result, 0, 2);  
        std::string passwordHash = PQgetvalue(result, 0, 3);
        double balance = std::stod(PQgetvalue(result, 0, 4));

        PQclear(result);
        return new Account(id, holder, cpfStored, balance);
    }
    PQclear(result);
    return nullptr;
}

// ------------------ TRANSFERÊNCIA ENTRE CONTAS ------------------
bool AccountService::transfer(const std::string& fromCpf, const std::string& toCpf, double amount) {
    Account* fromAccount = findAccountByCpf(fromCpf);
    Account* toAccount = findAccountByCpf(toCpf);

    if (!fromAccount || !toAccount || fromAccount->getBalance() < amount) {
        std::cerr << "Transfer failed: Invalid accounts or insufficient balance." << std::endl;
        return false;
    }

    PGresult* resBegin = dbConnection.executeQuery("BEGIN;");
    if (!resBegin) {
        std::cerr << "Error starting transaction!" << std::endl;
        return false;
    }
    PQclear(resBegin);

    PGresult* resFrom = dbConnection.updateBalance(fromCpf, fromAccount->getBalance() - amount);
    PGresult* resTo = dbConnection.updateBalance(toCpf, toAccount->getBalance() + amount);

    if (!resFrom || !resTo) {
        std::cerr << "Error updating balances! Rolling back transaction." << std::endl;
        dbConnection.executeQuery("ROLLBACK;");
        return false;
    }

    PQclear(resFrom);
    PQclear(resTo);

    PGresult* resOut = dbConnection.insertTransaction("Transfer Out", amount, fromAccount->getId(), fromCpf, toCpf);
    PGresult* resIn = dbConnection.insertTransaction("Transfer In", amount, toAccount->getId(), fromCpf, toCpf);

    if (!resOut || !resIn) {
        std::cerr << "Error inserting transactions! Rolling back transaction." << std::endl;
        dbConnection.executeQuery("ROLLBACK;");
        return false;
    }

    PQclear(resOut);
    PQclear(resIn);

    PGresult* resCommit = dbConnection.executeQuery("COMMIT;");
    if (!resCommit) {
        std::cerr << "Error committing transaction!" << std::endl;
        return false;
    }
    PQclear(resCommit);

    std::cout << "Transfer of $" << amount << " from " << fromCpf << " to " << toCpf << " was successful!" << std::endl;
    return true;
}

// ------------------ DEPÓSITO ------------------
bool AccountService::deposit(const std::string& cpf, double amount) {
    Account* account = findAccountByCpf(cpf);
    if (!account || amount <= 0) {
        std::cerr << "Invalid deposit operation." << std::endl;
        return false;
    }

    PGresult* res = dbConnection.updateBalance(cpf, account->getBalance() + amount);
    if (!res) {
        return false;
    }
    PQclear(res);

    dbConnection.insertTransaction("Deposit", amount, account->getId());
    std::cout << "Deposit of $" << amount << " to CPF: " << cpf << " was successful!" << std::endl;
    return true;
}

// ------------------ SAQUE ------------------
bool AccountService::withdraw(const std::string& cpf, double amount) {
    Account* account = findAccountByCpf(cpf);
    if (!account || amount <= 0 || account->getBalance() < amount) {
        std::cerr << "Invalid withdrawal operation." << std::endl;
        return false;
    }

    PGresult* res = dbConnection.updateBalance(cpf, account->getBalance() - amount);
    if (!res) {
        return false;
    }
    PQclear(res);

    dbConnection.insertTransaction("Withdraw", amount, account->getId());
    std::cout << "Withdrawal of $" << amount << " from CPF: " << cpf << " was successful!" << std::endl;
    return true;
}

// ------------------ BUSCAR TRANSAÇÕES PELO CPF ------------------
json AccountService::getTransactionsByCpf(const std::string& cpf) {
    Account* account = findAccountByCpf(cpf);
    if (!account) {
        return json::array();
    }

    PGresult* result = dbConnection.getTransactions(account->getId());
    if (!result) {
        return json::array();
    }

    json transactions = json::array();
    for (int i = 0; i < PQntuples(result); i++) {
        json transaction = {
            {"id", std::stoi(PQgetvalue(result, i, 0))},
            {"type", PQgetvalue(result, i, 1)},
            {"amount", std::stod(PQgetvalue(result, i, 2))},
            {"date", formatDate(PQgetvalue(result, i, 3))},
            {"account_id", std::stoi(PQgetvalue(result, i, 4))}
        };

        if (transaction["type"] == "Transfer Out" || transaction["type"] == "Transfer In") {
            transaction["from_cpf"] = PQgetvalue(result, i, 5);
            transaction["to_cpf"] = PQgetvalue(result, i, 6);
        }

        transactions.push_back(transaction);
    }

    PQclear(result);
    return transactions;
}

// ------------------ BUSCAR TODAS AS TRANSAÇÕES ------------------
json AccountService::getAllTransactions() {
    PGresult* result = dbConnection.getAllTransactions();
    if (!result) {
        return json::array();
    }

    json transactions = getTransactionsByCpf("");
    PQclear(result);
    return transactions;
}
