#include "RSA.h"

RSA::RSA(int size){
    this->generateKeyPair(size);
}

RSA::RSA(std::string publicKey, std::string privateKey){
    mpz_init(this->publicKey);
    mpz_init(this->privateKey);
    mpz_set_str(this->publicKey, publicKey.c_str(), 10);
    mpz_set_str(this->privateKey, privateKey.c_str(), 10);
}

void RSA::generateKeyPair(int size){
    mpz_init(this->publicKey);
    mpz_init(this->privateKey);
    unsigned long long p;
    unsigned long long q;
}

std::string RSA::encrypt(std::string plaintext){
    return "ciphertext";
}

std::string RSA::decrypt(std::string ciphertext){
    return "plaintext";
}
