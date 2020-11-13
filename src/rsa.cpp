#include "rsa.h"
#include <fstream>

const char *RSA::readKey(int type, const char *file)
{
    std::string line;
    std::string key;

    std::ifstream keyFile(file);
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
        key = replaceSubString(key, "\n", "");
        key = base64_decode(key);
    }

    std::cout << key;
    return key.c_str();
}

RSA::RSA(int size)
{
    this->generateKeyPair(size);
}

// RSA::RSA(std::string publicKey, std::string privateKey)
// {
//     mpz_init(this->publicKey);
//     mpz_init(this->privateKey);
//     mpz_set_str(this->publicKey, publicKey.c_str(), 10);
//     mpz_set_str(this->privateKey, privateKey.c_str(), 10);
// }

RSA::RSA(const char *publicKey, const char *privateKey)
{
    mpz_init(this->publicKey);
    mpz_init(this->privateKey);
    // mpz_import(this->privateKey, privateKey)
}

void RSA::generateKeyPair(int size)
{
    unsigned int e = 65537;
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_int_distribution<unsigned long> distrib(0, UINT64_MAX);
    // std::srand(distrib(gen));
    // std::string file = "../public.pem";
    // const char *publicKey = "as";
    // int a = distrib(gen), b = distrib(gen), c = distrib(gen);
    // QTIsaac<16, ISAAC_INT> qtIsaac(a, b, c);

    // mpz_t p;
    // mpz_t q;
    // mpz_t n;
    mpz_t temp;
    char rand_in[1024];

    mpz_init(this->publicKey);
    mpz_init(this->privateKey);
    mpz_init(this->p);
    mpz_init(this->q);
    mpz_init(n);
    mpz_init(temp);
    size_t bits = 512;                                                      //Declare size of data
    std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary); //Open stream
    if (urandom)
    {
        urandom.read(rand_in, bits);
        mpz_import(this->p, bits / 8, 1, 1, 1, 0, rand_in);
        mpz_nextprime(this->p, this->p);
        urandom.read(rand_in, bits);
        mpz_import(this->q, bits / 8, 1, 1, 1, 0, rand_in);
        mpz_nextprime(this->q, this->q);
        bits = 510;
        while (mpz_sizeinbase(n, 2) < 1024)
        {
            urandom.read(rand_in, bits);
            mpz_import(temp, bits / 8, 1, 1, 1, 0, rand_in);
            mpz_add(this->p, this->p, temp);
            mpz_nextprime(this->p, this->p);
            mpz_mul(n, this->p, this->q);
            if (mpz_sizeinbase(n, 2) == 1024)
                break;
            urandom.read(rand_in, bits);
            mpz_import(temp, bits / 8, 1, 1, 1, 0, rand_in);
            mpz_add(this->q, this->q, temp);
            mpz_nextprime(this->q, this->q);
            mpz_mul(n, this->p, this->q);
        }
    }
    mpz_nextprime(this->q, this->q);
    mpz_mul(n, this->p, this->q);

    std::cout << mpz_sizeinbase(n, 2) << std::endl;
    std::cout << mpz_out_str(stdout, 10, n) << std::endl;
    std::cout << mpz_out_str(stdout, 10, this->p) << std::endl;
    std::cout << mpz_out_str(stdout, 10, this->q) << std::endl;
}

std::string RSA::encrypt(std::string plaintext)
{
    return "ciphertext";
}

std::string RSA::decrypt(std::string ciphertext)
{
    return "plaintext";
}
