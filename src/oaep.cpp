#include "oaep.h"


OAEP::OAEP(int n, int k0, int k1) {
    OAEP::n = n;
    OAEP::k0 = k0;
    OAEP::k1 = k1;
}

std::string OAEP::addPadding(std::string message) {
    message.insert(message.length(), k1, '\0'); // First step: pad message with k1 zeros

    unsigned long long int r = 0; // Second step: generate random number with k0 bits

    std::ifstream urandom ("/dev/urandom", std::ios::in | std::ios::binary); //Open stream
    if (urandom) {
        urandom.read(reinterpret_cast<char*>(&r), k0);
        // if (urandom) { //Check if stream is ok, read succeeded
        //     std::cout << "Read random value: " << r << std::endl;
        // }
        // else { //Read failed
        //     std::cerr << "Failed to read from /dev/urandom" << std::endl;
        // }
        urandom.close();
    }

    // Third step: SHA1 
    std::cout << "toviva" << std::endl;
    unsigned char expanded_r[512];


    SHA1(reinterpret_cast<unsigned char*>(&r), n-k0, expanded_r);

    std::cout << expanded_r << std::endl;


    return "oi";
}