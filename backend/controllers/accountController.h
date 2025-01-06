#pragma once
#include "../services/accountService.h"
#include <httplib.h>

class AccountController {
private:
    AccountService accountService;

public:
    AccountController();

    void setupRoutes(httplib::Server& server);
};
