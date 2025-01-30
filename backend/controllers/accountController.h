#pragma once
#include "../services/accountService.h"
#include <httplib.h>

// Controlador responsável por gerenciar as rotas relacionadas às contas
class AccountController {
private:
    AccountService accountService; // Serviço de contas para operações no banco de dados

public:
    AccountController(); // Construtor

    void setupRoutes(httplib::Server& server); // Configura todas as rotas da API
};
