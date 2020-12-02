#ifndef RSA__H
#define RSA__H

#include <string>
#include <gmp.h>
#include <fstream>
#include <iostream>
#include <random>
#include <sysexits.h>

#include "isaac.cpp"
#include "str_utils.h"
#include "base64.h"
#include "rsa_.h"
#include "oaep.h"

#define PUBLIC 0
#define PRIVATE 1
// From linux/kconfig.h
/*
* Helper macros to use CONFIG_ options in C/CPP expressions. Note that
* these only work with boolean and tristate options.
*/
/*
* Getting something that works in C and CPP for an arg that may or may
* not be defined is tricky.  Here, if we have "#define CONFIG_BOOGER 1"
* we match on the placeholder define, insert the "0," for arg1 and generate
* the triplet (0, 1, 0).  Then the last step cherry picks the 2nd arg (a one).
* When CONFIG_BOOGER is not defined, we generate a (... 1, 0) pair, and when
* the last step cherry picks the 2nd arg, we get a zero.
*/
#define __ARG_PLACEHOLDER_1 0,
#define __take_second_arg(__ignored, val, ...) val
#define __is_defined(x) ___is_defined(x)
#define ___is_defined(val) ____is_defined(__ARG_PLACEHOLDER_##val)
#define ____is_defined(arg1_or_junk) __take_second_arg(arg1_or_junk 1, 0)
#define DEBUG 0
#define TRACE(y)                     \
    if (__is_defined(DEBUG))         \
    {                                \
        std::cout << y << std::endl; \
    }

class fileNotFoundException : std::runtime_error
{
public:
    fileNotFoundException(std::string what) : std::runtime_error(what)
    {
    }
};
class inputTooLargeException : std::runtime_error
{
public:
    inputTooLargeException(std::string what) : std::runtime_error(what)
    {
    }
};

class RSA_
{
public:
    RSA_(unsigned int, std::string = "key");
    RSA_(std::string);
    std::string encrypt(std::string);
    std::string decrypt(std::string);
    static unsigned int getKeySize(std::string);
    void writeKeysToFile(std::string);
    const unsigned int getSize();

private:
    mpz_t n;
    mpz_t d;
    mpz_t e;
    const unsigned int size;
    void generateKeyPair();
};

#endif
