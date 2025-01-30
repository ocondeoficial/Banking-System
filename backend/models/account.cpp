#include "account.h"
#include <iostream>

Account::Account(int id, const std::string& holder, const std::string& cpf, double initialBalance)
    : id(id), holder(holder), cpf(cpf), balance(initialBalance) {}

int Account::getId() const {
    return id;
}

std::string Account::getHolder() const {
    return holder;
}

std::string Account::getCpf() const {
    return cpf;
}

double Account::getBalance() const {
    return balance;
}

