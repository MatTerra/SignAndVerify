#include <string>

#include "oaep.h"

int main()
{
    OAEP meubb(1, 7, 11); 
    std::cout << meubb.addPadding("abcdefgh") << std::endl;
    // RSA my_rsa(1024);
    // std::cout << SHA3::calculate_file_hash("my_file");
    return 0;
}
