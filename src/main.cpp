#include <string>

#include "oaep.h"

int main()
{
    OAEP meubb(64, 12, 16); 
    std::cout << meubb.addPadding("ja te disseram o que atum e? e peixe") << std::endl;

    // RSA my_rsa(1024);
    // std::cout << SHA3::calculate_file_hash("my_file");
    return 0;
}