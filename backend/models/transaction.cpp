#include "transaction.h"

Transaction::Transaction(int id, const std::string& type, double amount, const std::string& date, int accountId)
    : id(id), type(type), amount(amount), date(date), accountId(accountId) {}

int Transaction::getId() const {
    return id;
}

std::string Transaction::getType() const {
    return type;
}

double Transaction::getAmount() const {
    return amount;
}

std::string Transaction::getDate() const {
    return date;
}

int Transaction::getAccountId() const {
    return accountId;
}
