#pragma once
#include <string>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

inline std::string bytesToHexString(const unsigned char* bytes, size_t length) {
    std::stringstream ss;
    for (size_t i = 0; i < length; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)bytes[i];
    }
    return ss.str();
}

inline std::string hashPassword(const std::string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(password.c_str()), password.size(), hash);
    return bytesToHexString(hash, SHA256_DIGEST_LENGTH);
}

inline bool validatePassword(const std::string& password, const std::string& storedHash) {
    return hashPassword(password) == storedHash;
}
