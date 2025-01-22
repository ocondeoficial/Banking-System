#include "accountController.h"
#include "../third_party/json.hpp"
#include "../services/hashUtils.h"


using json = nlohmann::json;

AccountController::AccountController() {}

void AccountController::setupRoutes(httplib::Server& server) {
    server.Post("/account/create", [&](const httplib::Request& req, httplib::Response& res) {
        auto body = json::parse(req.body);
        std::string holder = body["holder"];
        std::string cpf = body["cpf"];
        std::string password = body["password"];
        double initialBalance = body["initial_balance"];

        std::string hashedPassword = hashPassword(password);

        if (accountService.createAccount(holder, cpf, hashedPassword, initialBalance)) {
            res.set_content("Account created successfully!", "text/plain");
        } else {
            res.status = 400;
            res.set_content("Error creating account. CPF may already exist.", "text/plain");
        }
    });

    server.Post("/account/login", [&](const httplib::Request& req, httplib::Response& res) {
        auto body = json::parse(req.body);
        std::string cpf = body["cpf"];
        std::string password = body["password"];

        if (accountService.validateLogin(cpf, password)) {
            res.set_content("Login successful!", "text/plain");
        } else {
            res.status = 401;
            res.set_content("Invalid CPF or password.", "text/plain");
        }
    });

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
            res.set_content("Account not found", "text/plain");
        }
    });

    server.Post("/account/deposit", [&](const httplib::Request& req, httplib::Response& res) {
        auto body = json::parse(req.body);
        std::string cpf = body["cpf"];
        double amount = body["amount"];

        if (accountService.deposit(cpf, amount)) {
            res.set_content("Deposit successful!", "text/plain");
        } else {
            res.status = 404;
            res.set_content("Account not found!", "text/plain");
        }
    });

    server.Post("/account/withdraw", [&](const httplib::Request& req, httplib::Response& res) {
        auto body = json::parse(req.body);
        std::string cpf = body["cpf"];
        double amount = body["amount"];

        if (accountService.withdraw(cpf, amount)) {
            res.set_content("Withdrawal successful!", "text/plain");
        } else {
            res.status = 400;
            res.set_content("Withdrawal failed! Check balance or CPF.", "text/plain");
        }
    });

    server.Post("/account/transfer", [&](const httplib::Request& req, httplib::Response& res) {
        auto body = json::parse(req.body);
        std::string fromCpf = body["from_cpf"];
        std::string toCpf = body["to_cpf"];
        double amount = body["amount"];

        if (amount <= 0) {
            res.status = 400;
            res.set_content("Invalid transfer amount. Must be greater than 0.", "text/plain");
            return;
        }

        if (accountService.transfer(fromCpf, toCpf, amount)) {
            res.set_content("Transfer successful!", "text/plain");
        } else {
            res.status = 400;
            res.set_content("Transfer failed! Check balance, CPF, or other details.", "text/plain");
        }
    });
}
