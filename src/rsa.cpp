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

    TRACE(key);
    return key.c_str();
}

RSA::RSA(unsigned int size) : size(size)
{
    this->generateKeyPair();
}

void RSA::generateKeyPair()
{

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
    mpz_t temp2;
    char rand_in[this->size];

    mpz_init(this->p);
    mpz_init(this->q);
    mpz_init(this->d);
    mpz_init(this->n);
    mpz_init(this->e);
    mpz_set_ui(this->e, 65537);
    mpz_init(this->tot);
    mpz_init(temp);
    mpz_init(temp2);

    size_t bits = this->size / 2;                                           //Declare size of data
    std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary); //Open stream
    if (urandom)
    {
        urandom.read(rand_in, bits);
        mpz_import(this->p, bits / 8, 1, 1, 1, 0, rand_in);
        mpz_nextprime(this->p, this->p);
        urandom.read(rand_in, bits);
        mpz_import(this->q, bits / 8, 1, 1, 1, 0, rand_in);
        mpz_nextprime(this->q, this->q);
        bits = this->size / 2 - 2;
        while (mpz_sizeinbase(n, 2) < this->size)
        {
            urandom.read(rand_in, bits);
            mpz_import(temp, bits / 8, 1, 1, 1, 0, rand_in);
            mpz_add(this->p, this->p, temp);
            mpz_nextprime(this->p, this->p);
            mpz_mul(n, this->p, this->q);
            if (mpz_sizeinbase(n, 2) == this->size)
                break;
            urandom.read(rand_in, bits);
            mpz_import(temp, bits / 8, 1, 1, 1, 0, rand_in);
            mpz_add(this->q, this->q, temp);
            mpz_nextprime(this->q, this->q);
            mpz_mul(n, this->p, this->q);
        }
    }
    mpz_nextprime(this->q, this->q);
    mpz_mul(this->n, this->p, this->q);

    mpz_sub_ui(temp, this->p, 1);
    mpz_sub_ui(temp2, this->q, 1);
    mpz_mul(this->tot, temp, temp2);

    mpz_set_si(temp, -1);
    mpz_powm(this->d, this->e, temp, this->tot);

    TRACE(mpz_sizeinbase(n, 2));
    TRACE("\n");
}

std::string RSA::encrypt(std::string plaintext)
{
    TRACE("Encrypting " + plaintext + "\n");
    if ((this->size / 8) < plaintext.size())
    {
        throw inputTooLargeException("Plaintext must be smaller than the modulus n!");
    }
    TRACE("Padding plaintext\n");
    OAEP oaep(this->size, 90, 90);
    std::string toEncrypt = oaep.addPadding(plaintext);
    TRACE("Padded plaintext is " + toEncrypt + "\n");
    TRACE("Plaintext in binary is " + toBinary(toEncrypt) + "\n");
    mpz_t plaintext_;
    mpz_init(plaintext_);
    mpz_set_str(plaintext_, toBinary(toEncrypt).c_str(), 2);
    mpz_powm(plaintext_, plaintext_, this->e, this->n);
    TRACE("Power done\n");
    char ciphertext_[this->size];
    mpz_get_str(ciphertext_, 2, plaintext_);
    TRACE("Got ciphertext from mpz\n");
    std::string ciphertext(ciphertext_);
    TRACE("Ciphertext is " + ciphertext + "\n");
    return ciphertext;
}

std::string RSA::decrypt(std::string ciphertext)
{
    TRACE("Decrypting " + ciphertext + "\n");
    mpz_t ciphertext_;
    mpz_init_set_str(ciphertext_, ciphertext.c_str(), 2);
    TRACE("Imported number to mpz\n");
    mpz_powm(ciphertext_, ciphertext_, this->d, this->n);
    TRACE("Power done\n");
    char plaintext_[this->size];
    mpz_get_str(plaintext_, 2, ciphertext_);
    TRACE("Got plaintext from mpz\n");
    std::string plaintext(plaintext_);
    if (plaintext.length() < this->size)
    {
        std::string missing_zeros(this->size - plaintext.length(), '0');
        plaintext = missing_zeros + plaintext;
    }
    TRACE("Plaintext is " + plaintext + "\n");
    return fromBinary(plaintext);
}
