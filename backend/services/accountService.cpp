#include "accountService.h"
#include <iostream>
#include "../services/hashUtils.h"

AccountService::AccountService() : dbConnection(std::getenv("DB_CONN_INFO")) {
    if (!std::getenv("DB_CONN_INFO")) {
        std::cerr << "Error: DB_CONN_INFO environment variable is not set." << std::endl;
        exit(1);
    }
}


bool AccountService::createAccount(const std::string& holder, const std::string& cpf, const std::string& hashedPassword, double initialBalance) {
    PGresult* result = dbConnection.createAccount(holder, cpf, hashedPassword, initialBalance);
    if (result != nullptr) {
        std::cout << "Account created successfully: " << holder << " (CPF: " << cpf << ")" << std::endl;
        PQclear(result);
        return true;
    }
    return false;
}

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


Account* AccountService::findAccountByCpf(const std::string& cpf) {
    PGresult* result = dbConnection.findAccountByCpf(cpf);
    if (result != nullptr && PQntuples(result) > 0) {
        int id = std::stoi(PQgetvalue(result, 0, 0));
        std::string holder = PQgetvalue(result, 0, 1);
        double balance = std::stod(PQgetvalue(result, 0, 2));
        PQclear(result);
        return new Account(id, holder, cpf, balance);
    }
    PQclear(result);
    return nullptr;
}

bool AccountService::transfer(const std::string& fromCpf, const std::string& toCpf, double amount) {
    Account* fromAccount = findAccountByCpf(fromCpf);
    Account* toAccount = findAccountByCpf(toCpf);

    if (!fromAccount || !toAccount) {
        std::cerr << "One or both accounts not found!" << std::endl;
        return false;
    }

    if (fromAccount->getBalance() < amount) {
        std::cerr << "Insufficient balance for transfer!" << std::endl;
        return false;
    }

    if (fromAccount->withdraw(amount) && toAccount->deposit(amount)) {
        dbConnection.insertTransaction("Transfer Out", amount, fromAccount->getId());
        dbConnection.insertTransaction("Transfer In", amount, toAccount->getId());
        std::cout << "Transfer of $" << amount << " from CPF: " << fromCpf << " to CPF: " << toCpf << " was successful!" << std::endl;
        return true;
    }

    std::cerr << "Transfer failed due to an unexpected error!" << std::endl;
    return false;
}


bool AccountService::deposit(const std::string& cpf, double amount) {
    Account* account = findAccountByCpf(cpf);
    if (account) {
        if (account->deposit(amount)) {
            dbConnection.insertTransaction("Deposit", amount, account->getId());
            std::cout << "Deposit of $" << amount << " to account with CPF: " << cpf << " was successful!" << std::endl;
            return true;
        } else {
            std::cerr << "Invalid deposit amount: " << amount << " for CPF: " << cpf << std::endl;
            return false;
        }
    }
    std::cerr << "Account with CPF: " << cpf << " not found!" << std::endl;
    return false;
}

bool AccountService::withdraw(const std::string& cpf, double amount) {
    Account* account = findAccountByCpf(cpf);
    if (account) {
        if (account->withdraw(amount)) {
            dbConnection.insertTransaction("Withdraw", amount, account->getId());
            std::cout << "Withdrawal of $" << amount << " from account with CPF: " << cpf << " was successful!" << std::endl;
            return true;
        } else {
            std::cerr << "Insufficient balance or invalid withdrawal amount for CPF: " << cpf << std::endl;
            return false;
        }
    }
    std::cerr << "Account with CPF: " << cpf << " not found!" << std::endl;
    return false;
}
