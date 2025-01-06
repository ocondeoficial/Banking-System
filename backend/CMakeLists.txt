cmake_minimum_required(VERSION 3.10)

project(BankingSystem)

# Include directories
include_directories(include controllers services models third_party /usr/include/postgresql)

# Add executable
add_executable(BankingSystem
    routes/main.cpp
    src/dbConnection.cpp
    controllers/accountController.cpp
    services/accountService.cpp
    models/account.cpp
    models/transaction.cpp
)

# Link libraries
target_link_libraries(BankingSystem pq pthread)
