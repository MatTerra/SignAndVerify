#include <iostream>
#include "str_utils.h"
#include "rsa.h"
#include "oaep.h"

// If parameter is not true, test fails
// This check function would be provided by the test framework
#define IS_TRUE(x)                                                                          \
    {                                                                                       \
        if (!x)                                                                             \
            std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl;       \
        else                                                                                \
            std::cout << __FUNCTION__ << " at line " << __LINE__ << " passed" << std::endl; \
    }

#define IS_EQUAL(x, y)                                                                      \
    {                                                                                       \
        if (x != y)                                                                         \
        {                                                                                   \
            std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl;       \
            std::cout << "Expected result to be: " << y << std::endl;                       \
            std::cout << "Got: " << x << std::endl;                                         \
        }                                                                                   \
        else                                                                                \
        {                                                                                   \
            std::cout << __FUNCTION__ << " at line " << __LINE__ << " passed" << std::endl; \
        }                                                                                   \
    }

#define IS_STR_EQUAL(x, y)                                                                  \
    {                                                                                       \
        if (x != y)                                                                         \
        {                                                                                   \
            std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl;       \
            std::cout << "Expected result to be: <" << y << ">" << std::endl;               \
            std::cout << "Got: <" << x << ">" << std::endl;                                 \
        }                                                                                   \
        else                                                                                \
        {                                                                                   \
            std::cout << __FUNCTION__ << " at line " << __LINE__ << " passed" << std::endl; \
        }                                                                                   \
    }

// Test for function1()
// You would need to write these even when using a framework
void test_replaceSubString()
{
    IS_STR_EQUAL(replaceSubString("abcdef", "abc", "def"), "defdef");
    IS_STR_EQUAL(replaceSubString("abcdef", "abc", ""), "def");
    IS_STR_EQUAL(replaceSubString("abcdef", "abc", "defdef"), "defdefdef");
}

void test_addPadding()
{
    OAEP oaep(80, 0, 0);
    IS_STR_EQUAL(oaep.addPadding("000"), "0000000000");
}

void test_toBinary()
{
    IS_STR_EQUAL(toBinary("a"), "01100001");
}

void test_fromBinary()
{
    IS_STR_EQUAL(fromBinary("0110000101100001"), "aa");
    IS_STR_EQUAL(fromBinary("01100001011000100110001100110000001100000011000000110000001100000011000000110000"), "abc0000000")
}

void test_RSA()
{
    RSA rsa(80);
    IS_STR_EQUAL(rsa.decrypt(rsa.encrypt("abc")), "abc0000000");

    OAEP oaep(1024, 0, 0);
    RSA rsa1024(1024);
    std::string input = "abcdefghijklmnopqrstuvxywzABCDEFGHIJKLMNOPQRSTUVXYWZ";
    IS_STR_EQUAL(rsa1024.decrypt(rsa1024.encrypt(input)), oaep.addPadding(input));
}

int main(void)
{
    // Call all tests. Using a test framework would simplify this.
    test_replaceSubString();
    test_addPadding();
    test_toBinary();
    test_fromBinary();
    test_RSA();
}