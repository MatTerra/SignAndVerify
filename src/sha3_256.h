#ifndef SHA3_256_H__
#define SHA3_256_H__

#include <openssl/evp.h>
#include <openssl/sha.h>

#include "sha3_256.h"
#include "str_utils.h"

std::string sha3_256(const std::string);

#endif