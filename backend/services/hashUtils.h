#pragma once
#include <string>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

// Converte um array de bytes em uma string hexadecimal
inline std::string bytesToHexString(const unsigned char* bytes, size_t length) {
    std::stringstream ss;
    for (size_t i = 0; i < length; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[i]);
    }
    return ss.str();
}

// Gera um hash SHA-256 para a senha fornecida
inline std::string hashPassword(const std::string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(password.c_str()), password.size(), hash);
    return bytesToHexString(hash, SHA256_DIGEST_LENGTH);
}

// Compara a senha fornecida com o hash armazenado para validar a autenticação
inline bool validatePassword(const std::string& password, const std::string& storedHash) {
    return hashPassword(password) == storedHash;
}
