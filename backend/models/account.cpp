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

bool Account::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        return true; // Depósito bem-sucedido
    }
    return false; // Valor inválido para depósito
}

bool Account::withdraw(double amount) {
    if (balance >= amount && amount > 0) {
        balance -= amount;
        return true; // Saque bem-sucedido
    }
    return false; // Saldo insuficiente ou valor inválido
}
