#ifndef RSA_H
#define RSA_H

#include <string>
#include <gmp.h>
#include <fstream>
#include <iostream>
#include <random>

#include "isaac.cpp"
#include "str_utils.h"
#include "base64.h"
#include "rsa.h"

#define PUBLIC 0
#define PRIVATE 1
#define RSA_PUBLIC_BEGIN_HEADER "-----BEGIN PUBLIC KEY-----"
#define RSA_PUBLIC_END_HEADER "-----END PUBLIC KEY-----"

class fileNotFoundException : std::runtime_error
{
public:
    fileNotFoundException(std::string what) : std::runtime_error(what)
    {
    }
};

class RSA
{
public:
    RSA(const char *publicKey, const char *privateKey);
    RSA(int);
    std::string encrypt(std::string);
    std::string decrypt(std::string);

    static const char *readKey(int type, const char *file);

private:
    mpz_t n;
    mpz_t p;
    mpz_t q;
    mpz_t publicKey;
    mpz_t privateKey;
    void generateKeyPair(int);
};

#endif
