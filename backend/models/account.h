#include "account.h"

Account::Account(int id, const std::string& holder, const std::string& cpf, double balance)
    : id(id), holder(holder), cpf(cpf), balance(balance) {}

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
