#include "rsa_.h"
#include <fstream>

const unsigned int RSA_::getSize()
{
    return this->size;
}

RSA_::RSA_(unsigned int size, std::string keyFile /* = "key" */) : size(size)
{
    if (size < 1024)
    {
        throw std::runtime_error("The key length must be at least 1024 bits!");
    }
    mpz_init(this->d);
    mpz_init(this->n);
    mpz_init_set_ui(this->e, 65537);
    TRACE("Starting key generation")
    this->generateKeyPair();
    this->writeKeysToFile(keyFile);
}

RSA_::RSA_(std::string keyFile) : size(RSA_::getKeySize(keyFile))
{

    std::string pubKeyFile = keyFile + ".pub";
    std::ifstream pubKey(pubKeyFile);
    std::string file_line;
    std::getline(pubKey, file_line);
    mpz_init_set_str(this->n, file_line.c_str(), 10);
    mpz_init_set_ui(this->e, 65537);

    try
    {
        std::ifstream privKey(keyFile);
        if (!privKey)
        {
            throw std::runtime_error("No private key!");
        }
        std::getline(privKey, file_line);
        mpz_t priv_n;
        mpz_init_set_str(priv_n, file_line.c_str(), 10);
        if (mpz_cmp(this->n, priv_n) != 0)
        {
            std::cerr << "Public and private key n don't match!";
            exit(EXIT_FAILURE);
        }
        std::getline(privKey, file_line);
        mpz_init_set_str(this->d, file_line.c_str(), 10);
    }
    catch (const std::exception &e)
    {
        std::cout << "No private key available. Only verify and encrypt possible." << std::endl;
        mpz_init_set_ui(this->d, 0);
    }
}

void RSA_::writeKeysToFile(std::string file)
{
    char n[this->size];
    mpz_get_str(n, 10, this->n);
    char d[mpz_sizeinbase(this->d, 10)];
    mpz_get_str(d, 10, this->d);
    char e[mpz_sizeinbase(this->e, 10)];
    mpz_get_str(e, 10, this->e);
    TRACE("Loaded keys to char variables");
    std::string pubKeyFile = file + ".pub";
    std::ofstream pubKey(pubKeyFile);
    TRACE("Opened pubkey file");
    if (pubKey)
    {
        pubKey << n << std::endl
               << e;
    }
    else
    {
        std::cerr << "Couldn't open public key file!";
    }
    pubKey.close();
    TRACE("Saved pubkey");
    if (std::string(d) == std::string("0"))
    {
        std::cout << "No private key available, won't save private key file." << std::endl;
        return;
    }
    std::ofstream privKey(file);
    TRACE("Opened privkey file");
    if (privKey)
    {
        privKey << n << std::endl
                << d;
    }
    else
    {
        std::cerr << "Couldn't open private key file!";
    }
    TRACE("Saved privkey")
}

unsigned int RSA_::getKeySize(std::string file)
{
    std::string pubKeyFile = file + ".pub";
    std::ifstream pubKey(pubKeyFile);
    std::string n_str;
    std::getline(pubKey, n_str);
    mpz_t n;
    mpz_init_set_str(n, n_str.c_str(), 10);
    return mpz_sizeinbase(n, 2);
}

void RSA_::generateKeyPair()
{
    // Temporary values for key generation computations
    mpz_t temp;
    mpz_t temp2;
    mpz_t p;
    mpz_t q;
    mpz_t tot;
    mpz_init(temp);
    mpz_init(temp2);
    mpz_init(p);
    mpz_init(q);
    mpz_init(tot);
    TRACE("Started mpz pointers");
    TRACE("Will generate keys with size ");
    TRACE(this->size);

    char rand_in[this->size];     // Declare input for urandom data
    size_t bits = this->size / 2; // Declare size of data to read from urandom

    TRACE("Opening /dev/urandom");
    std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary); //Open stream
    if (urandom)
    {
        TRACE("Started reading from urandom");
        urandom.read(rand_in, bits);
        mpz_import(p, bits / 8, 1, 1, 1, 0, rand_in);
        mpz_nextprime(p, p);
        urandom.read(rand_in, bits);
        mpz_import(q, bits / 8, 1, 1, 1, 0, rand_in);
        mpz_nextprime(q, q);
        bits = this->size / 2 - 2;
        TRACE("Adjusting key size.");
        while (mpz_sizeinbase(n, 2) < this->size)
        {
            TRACE(".");
            urandom.read(rand_in, bits);
            mpz_import(temp, bits / 8, 1, 1, 1, 0, rand_in);
            mpz_add(p, p, temp);
            mpz_nextprime(p, p);
            mpz_mul(n, p, q);
            if (mpz_sizeinbase(n, 2) == this->size)
                break;
            urandom.read(rand_in, bits);
            mpz_import(temp, bits / 8, 1, 1, 1, 0, rand_in);
            mpz_add(q, q, temp);
            mpz_nextprime(q, q);
            mpz_mul(n, p, q);
        }
    }
    else
    {
        std::cerr << "Can't read from /dev/urandom to generate keys!";
        exit(EX_NOINPUT);
    }
    urandom.close();

    mpz_nextprime(q, q);
    mpz_mul(this->n, p, q);

    mpz_sub_ui(temp, p, 1);
    mpz_sub_ui(temp2, q, 1);
    mpz_mul(tot, temp, temp2);

    mpz_set_si(temp, -1);
    mpz_powm(this->d, this->e, temp, tot);

    TRACE(mpz_sizeinbase(n, 2));
    TRACE("\n");
}

std::string RSA_::encrypt(std::string plaintext)
{
    TRACE("Encrypting " + plaintext + "\n");
    if ((this->size / 8) < plaintext.size())
    {
        throw inputTooLargeException("Plaintext must be smaller than the modulus n!");
    }
    TRACE("Padding plaintext\n");
    OAEP oaep(this->size, this->size - 512);
    char toEncrypt[this->size + 1];
    oaep.addPadding(plaintext, toEncrypt);
    TRACE("Padded plaintext is " << toEncrypt << ", with size " << sizeof(toEncrypt) << "\n");
    TRACE("Plaintext in binary is " << toBinary(toEncrypt).c_str() << ", with size " << toBinary(toEncrypt).size() << "\n");
    mpz_t plaintext_integer;
    mpz_t ciphertext_integer;
    mpz_init(plaintext_integer);
    mpz_init(ciphertext_integer);
    mpz_set_str(plaintext_integer, toBinary(toEncrypt).c_str(), 2);
    mpz_powm(ciphertext_integer, plaintext_integer, this->e, this->n);
    TRACE("Power done\n");
    char ciphertext_binary[this->size];
    mpz_get_str(ciphertext_binary, 2, ciphertext_integer);
    TRACE("Got ciphertext from mpz\n");

    std::string ciphertext(ciphertext_binary);
    TRACE("Ciphertext is " << ciphertext << ", with size " << ciphertext.size() << "\n");
    while (ciphertext.size() < this->size)
    {
        ciphertext = ciphertext.insert(0, "0");
    }
    TRACE("Ciphertext is " << ciphertext << ", with size " << ciphertext.size() << "\n");

    return ciphertext;
}

std::string RSA_::decrypt(std::string ciphertext)
{
    if (this->size < ciphertext.size())
    {
        throw inputTooLargeException("Ciphertext must be of the same size as the modulus n!");
    }
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
    char output[this->size];
    fromBinary(output, plaintext);
    OAEP oaep(this->size, this->size - 512);
    std::string result = oaep.removePadding(output);
    return result;
}
