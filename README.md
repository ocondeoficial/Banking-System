# **Banking System**

O **Banking System** é um projeto de sistema bancário simplificado que permite a criação de contas, depósitos, saques, transferências e consulta de transações financeiras.

---

## **Sumário**
- [Back-End](#back-end)
- [Front-End](#front-end)
- [Contribuições](#contribuições)
- [Licença](#licença)

---

## **Back-End**

### **Tecnologias Utilizadas**
- **C++** (compilado com g++)
- **PostgreSQL** (Banco de dados)
- **cpp-httplib** (Servidor HTTP)
- **json.hpp** (Manipulação de JSON)
- **OpenSSL** (Hash de senhas com SHA256)
- **Docker** (Para ambiente de desenvolvimento e banco de dados)

### **Instalação e Configuração**

#### **Pré-requisitos**
- **Docker** e **Docker Compose** instalados
- **g++** (caso deseje rodar sem Docker)
- **PostgreSQL** (caso deseje rodar sem Docker)

#### **Passos para rodar o backend**
1. **Clone o repositório**  
   ```sh
   git clone https://github.com/yLukas077/Banking-System.git
   cd banking-system
   ```

2. **Suba os containers com Docker Compose**  
   ```sh
   docker-compose up --build
   ```

3. **A API estará disponível em:**  
   ```
   http://localhost:8080
   ```

### **Rotas da API**

#### **Autenticação e Conta**
- **Criar conta:** `POST /account/create`
- **Login:** `POST /account/login`
- **Buscar conta por CPF:** `GET /account/find?cpf={cpf}`

#### **Operações Bancárias**
- **Depósito:** `POST /account/deposit`
- **Saque:** `POST /account/withdraw`
- **Transferência:** `POST /account/transfer`

#### **Consultas**
- **Consultar transações por CPF:** `GET /transactions?cpf={cpf}`
- **Consultar todas as transações:** `GET /transactions/all`

---

## **Front-End**

### **Tecnologias Utilizadas**


---

## **Contribuições**
Contribuições são bem-vindas! Se deseja ajudar no desenvolvimento do projeto, siga os passos:
1. Faça um **fork** do repositório.
2. Crie uma **branch** com a nova funcionalidade ou correção:  
   ```sh
   git checkout -b minha-mudanca
   ```
3. **Commit suas alterações:**  
   ```sh
   git commit -m "Descrição da mudança"
   ```
4. **Envie para seu fork:**  
   ```sh
   git push origin minha-mudanca
   ```
5. **Crie um Pull Request** no repositório principal.

---
