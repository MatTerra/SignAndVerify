#include "sha3_256.h"

std::string sha3_256(const std::string input)
{
    uint32_t digest_length = SHA256_DIGEST_LENGTH;
    const EVP_MD *algorithm = EVP_sha3_256();
    uint8_t *digest = static_cast<uint8_t *>(OPENSSL_malloc(digest_length + 1));
    EVP_MD_CTX *context = EVP_MD_CTX_new();
    EVP_DigestInit_ex(context, algorithm, nullptr);
    EVP_DigestUpdate(context, input.c_str(), input.size());
    EVP_DigestFinal_ex(context, digest, &digest_length);
    EVP_MD_CTX_destroy(context);
    digest[digest_length] = '\0';
    std::string output = reinterpret_cast<char *>(digest);
    while (output.size() < 32)
    {
        output += '\0' + std::string((char *)digest + (output.size() + 1));
    }
    OPENSSL_free(digest);
    return output;
}