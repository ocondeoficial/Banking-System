# Banking System

## Overview
The Banking System prototype is a foundational project designed to simulate essential banking operations. It focuses on modularity and simplicity while maintaining core functionalities like account management, basic transaction handling, and secure data storage. This prototype is built using **C++** for backend logic and **Angular** for a responsive frontend.

---

## Key Features

1. **Account Management**
   - Create, edit, and delete bank accounts.
   - View basic account information, including balance and account holder.

2. **Banking Operations**
   - Perform basic deposits, withdrawals, and transfers between accounts.
   - Keep a simple log of transaction history.

3. **Basic Security**
   - Simulated user authentication.
   - Simple encryption for sensitive data.

4. **Reports (Basic Analytics)**
   - Provide straightforward summaries of account and transaction activities.

---

## Technologies Used

- **Backend**:
  - Language: **C++**
  - Database: **PostgreSQL**
  - API Library: **cpp-httplib**

- **Frontend**:
  - Framework: **Angular**
  - UI Components: **Angular Material**
  - Styling: **SCSS**

---

## Technical Highlights

### Backend
- **Modularity**:
  - Divided into simple classes for `Bank`, `Account`, and `Transaction` management.
- **Validation**:
  - Implements basic input checks, like valid CPF and balance sufficiency.
- **Database**:
  - Uses PostgreSQL for storing accounts and transactions with basic SQL scripts.

### Frontend
- **Angular-Based UI**:
  - Provides a basic interface for interacting with the backend.
- **Routing**:
  - Includes basic navigation for managing accounts, performing transactions, and viewing reports.
- **Responsive Design**:
  - Optimized for both desktop and mobile browsers.

---

## System Flow

1. **Authentication**:
   - Simulated login process for users.
2. **Account Management**:
   - Create and view account details.
3. **Transactions**:
   - Perform deposits, withdrawals, and transfers.
4. **Basic Reports**:
   - View a log of recent transactions.

---

## Development Workflow

### Backend
- **Data Handling**:
  - Basic CRUD operations for accounts and transactions.
- **Validation**:
  - Simplified checks for user input and account operations.
- **Database**:
  - Minimalistic schema with two tables: `accounts` and `transactions`.

### Frontend
- **Simple UI**:
  - Minimalistic forms and tables for data interaction.
- **State Management**:
  - Basic Angular services to handle backend communication.

---

## Future Improvements
While this prototype is a basic implementation, potential improvements include:
- Enhanced user authentication (e.g., OAuth 2.0).
- Advanced validation and error handling.
- Expanded reporting with visual charts.
- Support for role-based access.

---

## Setup Instructions

### Backend
1. Install C++ and PostgreSQL.
2. Set up the database using the provided initialization script.
3. Compile and run the backend.

### Frontend
1. Install Angular CLI.
2. Set up the frontend dependencies (`npm install`).
3. Run the frontend using `ng serve`.

---

This project serves as a prototype for understanding and demonstrating basic banking functionalities, providing a foundation for further development and refinement.
