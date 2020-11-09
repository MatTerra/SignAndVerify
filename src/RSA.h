#ifndef RSA_H
#define RSA_H

#include <strings>

class RSA{
	
	public:
		RSA(std::string, std::string);
		std::string encrypt(std::string);
		std::string decrypt(std::string); 		
	private:
		std::string publicKey;
		std::string privateKey;
};

#endif
