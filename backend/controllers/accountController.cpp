#include "accountController.h"

#include <iostream>
#include "../third_party/json.hpp"
#include "../services/hashUtils.h"

using json = nlohmann::json;

AccountController::AccountController() {}

// Configuração das rotas da API
void AccountController::setupRoutes(httplib::Server& server) {
    
    server.set_pre_routing_handler([](const httplib::Request &req, httplib::Response &res) {
        
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");

        
        if (req.method == "OPTIONS") {
            res.status = 200;
            return httplib::Server::HandlerResponse::Handled;
        }

        return httplib::Server::HandlerResponse::Unhandled;
    });
    
    // Criar conta
    server.Post("/account/create", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);
            std::string holder = body["holder"];
            std::string cpf = body["cpf"];
            std::string password = body["password"];
            double initialBalance = body["initial_balance"];

            std::string hashedPassword = hashPassword(password);
            bool success = accountService.createAccount(holder, cpf, hashedPassword, initialBalance);

            json response = {{"message", success ? "Account created successfully!" : "Error creating account. CPF may already exist."}};
            res.status = success ? 200 : 400;
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception&) {
            res.status = 400;
            res.set_content(json{{"error", "Invalid request format"}}.dump(), "application/json");
        }
    });

    // Login de conta
    server.Post("/account/login", [&](const httplib::Request& req, httplib::Response& res) {
        auto body = json::parse(req.body);
        std::string cpf = body["cpf"];
        std::string password = body["password"];

        if (accountService.validateLogin(cpf, password)) {
            Account* account = accountService.findAccountByCpf(cpf);

            json response = {
                {"success", true},
                {"holder", account->getHolder()},
                {"balance", account->getBalance()},
                {"cpf", account->getCpf()}
            };
            res.set_content(response.dump(), "application/json");
        } else {
            res.status = 401;
            json response = {{"success", false}, {"message", "CPF ou senha inválidos."}};
            res.set_content(response.dump(), "application/json");
        }
    });

    // Buscar conta por CPF
    server.Get("/account/find", [&](const httplib::Request& req, httplib::Response& res) {
        std::string cpf = req.get_param_value("cpf");
        Account* account = accountService.findAccountByCpf(cpf);

        if (account) {
            json response = {
                {"id", account->getId()},
                {"holder", account->getHolder()},
                {"cpf", account->getCpf()},
                {"balance", account->getBalance()}
            };
            res.set_content(response.dump(), "application/json");
        } else {
            res.status = 404;
            res.set_content(json{{"error", "Account not found"}}.dump(), "application/json");
        }
    });

    // Depositar dinheiro
    server.Post("/account/deposit", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);
            std::string cpf = body["cpf"];
            double amount = body["amount"];

            bool success = accountService.deposit(cpf, amount);
            json response = {{"message", success ? "Deposit successful!" : "Account not found!"}};
            res.status = success ? 200 : 404;
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception&) {
            res.status = 400;
            res.set_content(json{{"error", "Invalid request format"}}.dump(), "application/json");
        }
    });

    // Sacar dinheiro
    server.Post("/account/withdraw", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);
            std::string cpf = body["cpf"];
            double amount = body["amount"];

            bool success = accountService.withdraw(cpf, amount);
            json response = {{"message", success ? "Withdrawal successful!" : "Withdrawal failed! Check balance or CPF."}};
            res.status = success ? 200 : 400;
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception&) {
            res.status = 400;
            res.set_content(json{{"error", "Invalid request format"}}.dump(), "application/json");
        }
    });

    // Transferência entre contas
    server.Post("/account/transfer", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);
            std::string fromCpf = body["from_cpf"];
            std::string toCpf = body["to_cpf"];
            double amount = body["amount"];

            if (amount <= 0) {
                res.status = 400;
                res.set_content(json{{"error", "Invalid transfer amount. Must be greater than 0."}}.dump(), "application/json");
                return;
            }

            bool success = accountService.transfer(fromCpf, toCpf, amount);
            json response = {{"message", success ? "Transfer successful!" : "Transfer failed! Check balance, CPF, or other details."}};
            res.status = success ? 200 : 400;
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception&) {
            res.status = 400;
            res.set_content(json{{"error", "Invalid request format"}}.dump(), "application/json");
        }
    });

    // Buscar transações por CPF
    server.Get("/account/transactions", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string cpf = req.get_param_value("cpf");
            json transactions = accountService.getTransactionsByCpf(cpf);

            if (transactions.empty()) {
                res.status = 404;
                res.set_content(json{{"error", "No transactions found"}}.dump(), "application/json");
            } else {
                res.set_content(transactions.dump(), "application/json");
            }
        } catch (const std::exception&) {
            res.status = 500;
            res.set_content(json{{"error", "Internal server error"}}.dump(), "application/json");
        }
    });

    // Buscar todas as transações
    server.Get("/account/transactions/all", [&](const httplib::Request& req, httplib::Response& res) {
        json transactions = accountService.getAllTransactions();
        if (transactions.empty()) {
            res.status = 404;
            res.set_content(json{{"error", "No transactions found"}}.dump(), "application/json");
        } else {
            res.set_content(transactions.dump(), "application/json");
        }
    });
}
