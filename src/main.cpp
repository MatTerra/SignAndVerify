#include "SHA3.h"
#include <iostream>

int main(){
	std::cout << SHA3::calculate_file_hash("my_file");
	return 0;
}
