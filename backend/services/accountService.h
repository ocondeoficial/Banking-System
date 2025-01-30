#pragma once
#include <string>
#include "../models/account.h"
#include "../include/dbConnection.h"
#include "../third_party/json.hpp"

// Define um alias para facilitar o uso da biblioteca JSON
using json = nlohmann::json;

// Classe responsável pelos serviços relacionados às contas bancárias
class AccountService {
private:
    DbConnection dbConnection; // Instância da conexão com o banco de dados

public:
    // Construtor
    AccountService();

    // Cria uma nova conta bancária
    bool createAccount(const std::string& holder, const std::string& cpf, const std::string& hashedPassword, double initialBalance);

    // Busca uma conta bancária pelo CPF
    Account* findAccountByCpf(const std::string& cpf);

    // Valida o login verificando CPF e senha
    bool validateLogin(const std::string& cpf, const std::string& password);

    // Realiza um depósito na conta do usuário
    bool deposit(const std::string& cpf, double amount);

    // Realiza um saque na conta do usuário
    bool withdraw(const std::string& cpf, double amount);

    // Realiza uma transferência entre duas contas
    bool transfer(const std::string& fromCpf, const std::string& toCpf, double amount);

    // Retorna todas as transações de um usuário pelo CPF
    json getTransactionsByCpf(const std::string& cpf);

    // Retorna todas as transações do sistema
    json getAllTransactions();

    // Formata a data para um formato mais legível
    std::string formatDate(const std::string& rawDate);
};
