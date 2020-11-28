#include <string>

#include "oaep.h"

int main()
{
    OAEP meubb(64, 12, 16);
    std::string padded = meubb.addPadding("ja te disseram o que atum e? e peixe");
    std::cout << padded << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << meubb.removePadding(padded) << std::endl;
    // RSA my_rsa(1024);
    // std::cout << SHA3::calculate_file_hash("my_file");
    return 0;
}