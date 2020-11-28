#include "oaep.h"

OAEP::OAEP(int n, int k0, int k1) : n(n), k0(k0), k1(k1) {

}

std::string OAEP::addPadding(std::string message) {

    int message_length = message.length(); // message.length = n - k0 - k1

    char msg[n-k0+1]; // Array for padded message

    std::strcpy(msg, message.c_str());

    // First step: pad message with k1 zeros
    for (int i = message_length; i < message_length + k1; i++) { 
        msg[i] = '\0';
    }

    // std::cout << msg << " " << sizeof(msg) << std::endl;

    // Second step: generate random number with k0 bits
    char r[k0+1]; 

    std::ifstream urandom ("/dev/urandom", std::ios::in | std::ios::binary); //Open stream
    if (urandom) {
        urandom.read(r, k0);
        urandom.close();
    }

    // Third step: SHA1 to expand r to n-k0 bits 
    char expanded_r[n-k0+1];

    SHA1(reinterpret_cast<unsigned char*>(r), n-k0, reinterpret_cast<unsigned char*>(expanded_r));

    // Fourth step: XOR between r and msg
    char x[n-k0+1];

    for (int i = 0; i < n-k0; i++)
        x[i] = msg[i] ^ expanded_r[i];

    // Fifth step: Reduce x to k0 bits
    char reduced_x[k0+1];

    SHA1(reinterpret_cast<unsigned char*>(x), k0, reinterpret_cast<unsigned char*>(reduced_x));

    // Sixth step: XOR between r and reduced x
    char y[k0+1];

    for (int i = 0; i < k0; i++)
        y[i] = r[i] ^ reduced_x[i];

    // Seventh step: append X to Y
    std::string output = "";
    output += x;
    output += y;

    return output;
}

std::string OAEP::removePadding(std::string encoded) {

    // First step: Split encoded in X and Y (x + y = encoded)
    char encoded_x[n-k0+1];
    char encoded_y[k0+1];

    strncpy(encoded_x, encoded.c_str(), n-k0);
    strncpy(encoded_y, &encoded.c_str()[n-k0], k0);

    // Second step: XOR between Y and reduced X
    char reduced_x[k0+1];

    SHA1(reinterpret_cast<unsigned char*>(encoded_x), k0, reinterpret_cast<unsigned char*>(reduced_x));

    char r[k0+1];

    for (int i = 0; i < k0; i++)
        r[i] = encoded_y[i] ^ reduced_x[i];

    // Third step: XOR between X and expanded r
    char expanded_r[n-k0+1];

    SHA1(reinterpret_cast<unsigned char*>(r), n-k0, reinterpret_cast<unsigned char*>(expanded_r));

    char padded_message[n-k0+1];

    for (int i = 0; i < k0; i++)
        padded_message[i] = encoded_x[i] ^ expanded_r[i];

    std::string output = "";
    output += padded_message;
    return output;
}