# **Banking System**

The **Banking System** is a simplified banking system project that allows you to create accounts, make deposits, withdrawals, transfers and consult financial transactions.

---

## **Summary**
- [Back-End](#back-end)
- [Front-End](#front-end)
- [Contributions](#contributions)

---

## **Back-End**

### **Technologies Used**
- **C++** (compiled with g++)
- **PostgreSQL** (Database)
- **cpp-httplib** (HTTP Server)
- **json.hpp** (JSON Manipulation)
- **OpenSSL** (Password Hashing with SHA256)
- **Docker** (For development environment and database)

### **Installation and Configuration**

#### **Prerequisites**
- **Docker** and **Docker Compose** installed
- **g++** (if you want to run without Docker)
- **PostgreSQL** (if you want to run without Docker)

#### **Steps to run the backend**
1. **Clone the repository**
```sh
git clone https://github.com/yLukas077/Banking-System.git
cd banking-system
```

2. **Upload the containers with Docker Compose**
```sh
docker-compose up --build -d
```

3. **The API will be available at:**
```
http://localhost:8080
```

### **API Routes**

#### **Authentication and Account**
- **Create account:** `POST /account/create`
- **Login:** `POST /account/login`
- **Search for account by CPF:** `GET /account/find?cpf={cpf}`

#### **Operations Banking**
- **Deposit:** `POST /account/deposit`
- **Withdrawal:** `POST /account/withdraw`
- **Transfer:** `POST /account/transfer`

#### **Query**
- **Query transactions by CPF:** `GET /transactions?cpf={cpf}`
- **Query all transactions:** `GET /transactions/all`

---

## **Front-End**

### **Technologies Used**
-**HTML5**
-**CSS**
-**JavaScript**
---

## **Contributions**
Contributions are welcome! If you want to help with the development of the project, follow these steps:
1. Fork the repository. 

2. Create a **branch** with the new feature or fix:
```sh
git checkout -b my-change
```
3. **Commit your changes:**
```sh
git commit -m "Change description"
```
4. **Push to your fork:**
```sh
git push origin my-change
```
5. **Create a Pull Request** in the main repository.

---
