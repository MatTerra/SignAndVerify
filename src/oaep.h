#ifndef OAEP_H
#define OAEP_H

#include <string>
#include <iostream>
#include <random>
#include <fstream>
#include <openssl/sha.h>


class OAEP {
    public:
       OAEP(int n, int k0, int k1);
       std::string addPadding(std::string);
    private:
       int n, k0, k1;
};

#endif
