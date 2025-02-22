CREATE TABLE accounts (
    id SERIAL PRIMARY KEY,
    holder VARCHAR(100) NOT NULL,
    cpf CHAR(11) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    balance NUMERIC(12, 2) DEFAULT 0.00 NOT NULL
);

CREATE TABLE transactions (
    id SERIAL PRIMARY KEY,
    type VARCHAR(20) NOT NULL,
    amount NUMERIC(12, 2) NOT NULL,
    date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    from_cpf CHAR(11),
    to_cpf CHAR(11),
    account_id INT REFERENCES accounts(id)
);
