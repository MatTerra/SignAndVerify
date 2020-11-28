#include "oaep.h"

std::string toBinary(std::string input)
{
    std::string output = "";
    for (char &_char : input)
    {
        output += std::bitset<8>(_char).to_string();
    }
    return output;
}

OAEP::OAEP(int n, int k1) : n(n), k0(160), k1(k1)
{
    if (n % 8 != 0)
        throw std::runtime_error("The modulus must be a multiple of 8");
    if (n < 320)
        throw std::runtime_error("The modulus must be at least 328 bits");
    if (n < k0 + k1 - 8)
        throw std::runtime_error("The modulus must be greater than k0+k1");
    if (k1 % 8 != 0)
        throw std::runtime_error("K1 must be a multiple of 8");
}

std::string OAEP::addPadding(std::string message)
{

    int message_length = message.length(); // message.length = n - k0 - k1

    if (message_length != (n - k0 - k1) / 8)
    {
        throw std::runtime_error("Message must be n-k0-k1 bits");
    }

    char msg[(n - k0) / 8 + 1]; // Array for padded message

    std::strcpy(msg, message.c_str());

    // First step: pad message with k1 zeros
    for (int i = message_length; i < message_length + (k1 / 8) + 1; i++)
    {
        msg[i] = '\0';
    }

    // Second step: generate random number with k0 bits
    char r[(k0 / 8) + 1];

    std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary); //Open stream
    if (urandom)
    {
        urandom.read(r, k0 / 8);
        urandom.close();
    }
    r[k0 / 8] = '\0';

    // Third step: MGF1 - SHA256 to expand r to n-k0 bits
    char expanded_r[(n - k0) / 8 + 1];
    strncpy(expanded_r, r, sizeof(r));

    // This seed could be random to ensure non-deterministic output, but the input is random
    const unsigned char seed[2] = "\0";
    int result = PKCS1_MGF1(reinterpret_cast<unsigned char *>(expanded_r), (n - k0) / 8, seed, 1, EVP_sha256());
    if (result)
    {
        std::cerr << "Error on padding " << result << std::endl;
    }
    expanded_r[(n - k0) / 8] = '\0';

    char out[n / 8 + 1];
    // Fourth step: XOR between r and msg
    unsigned char x[(n - k0) / 8 + 1];

    for (int i = 0; i < (n - k0) / 8; i++)
    {
        x[i] = msg[i] ^ expanded_r[i];
        out[i] = msg[i] ^ expanded_r[i];
    }
    x[(n - k0) / 8] = '\0';

    int offset = (n - k0) / 8;

    // Fifth step: Reduce x to k0 bits, which is 20 bytes or 160 bits
    unsigned char reduced_x[(k0 / 8) + 1];
    SHA1(x, (k0 / 8) + 1, reduced_x);

    reduced_x[(k0 / 8)] = '\0';

    // Sixth step: XOR between r and reduced x
    char y[k0 / 8 + 1];

    for (int i = 0; i < k0 / 8; i++)
    {
        y[i] = r[i] ^ reduced_x[i];
        out[offset + i] = y[i];
    }

    out[n / 8] = '\0';

    std::string output(out);

    return output;
}

std::string OAEP::removePadding(std::string encoded)
{
    char encode_[n / 8 + 1];
    strncpy(encode_, encoded.c_str(), n / 8);
    encode_[n / 8] = '\0';

    // First step: Split encoded in X and Y (x + y = encoded)
    unsigned char encoded_x[(n - k0) / 8 + 1];
    unsigned char encoded_y[k0 / 8 + 1];

    strncpy((char *)encoded_x, encode_, ((n - k0) / 8));
    encoded_x[(n - k0) / 8] = '\0';

    strncpy((char *)encoded_y, &encode_[(n - k0) / 8], k0 / 8);
    encoded_y[k0 / 8] = '\0';

    // Second step: XOR between Y and reduced X
    unsigned char reduced_x[(k0 / 8) + 1];
    SHA1(encoded_x, (k0 / 8) + 1, reduced_x);

    reduced_x[k0 / 8] = '\0';

    char r[k0 / 8 + 1];

    for (int i = 0; i < k0 / 8; i++)
        r[i] = encoded_y[i] ^ reduced_x[i];

    r[k0 / 8] = '\0';

    // Third step: XOR between X and expanded r
    char expanded_r[(n - k0) / 8 + 1];
    strncpy(expanded_r, r, sizeof(r));

    // This seed could be random to ensure non-deterministic output, but the input is random
    const unsigned char seed[2] = "\0";
    int result = PKCS1_MGF1(reinterpret_cast<unsigned char *>(expanded_r), (n - k0) / 8, seed, 1, EVP_sha256());
    if (result)
    {
        std::cerr << "Error on padding " << result << std::endl;
    }
    expanded_r[(n - k0) / 8] = '\0';

    char padded_message[(n - k0) / 8];

    for (int i = 0; i < k0 / 8; i++)
        padded_message[i] = encoded_x[i] ^ expanded_r[i];

    std::string output(padded_message);
    return output;
}