#ifndef RSA_H
#define RSA_H

#include <string>
#include <gmp.h>
// #include "isaac.cpp"
#include "base64.cpp"

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

std::string readKey(int type, std::string file)
{
    std::string line;
    std::string key;

    std::ifstream keyFile(file.c_str());
    if (keyFile.is_open())
    {
        while (std::getline(keyFile, line))
        {
            key.append(line);
        }
    }
    else
    {
        throw fileNotFoundException(file);
    }
    if (type == PUBLIC)
    {
        key = replaceSubString(key, RSA_PUBLIC_BEGIN_HEADER, "");
        key = replaceSubString(key, RSA_PUBLIC_END_HEADER, "");
        key = base64_decode(key);
    }

    return key;
}

class RSA
{
public:
    RSA(std::string publicKey, std::string privateKey);
    RSA(int);
    std::string encrypt(std::string);
    std::string decrypt(std::string);
    mpz_t publicKey;

private:
    mpz_t privateKey;
    void generateKeyPair(int);
};

#endif
