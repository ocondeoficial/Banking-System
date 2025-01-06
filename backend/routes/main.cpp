#include "../controllers/accountController.h"
#include <httplib.h>

int main() {
    httplib::Server server;

    AccountController accountController;
    accountController.setupRoutes(server);

    std::cout << "Server running at http://localhost:8080" << std::endl;
    server.listen("0.0.0.0", 8080);

    return 0;
}
