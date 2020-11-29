#include <iostream>
#include "str_utils.h"
#include "rsa_.h"
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
            std::cout << "Expected result to be: \t<" << y << ">" << std::endl;             \
            std::cout << "Got: \t\t\t<" << x << ">" << std::endl;                           \
        }                                                                                   \
        else                                                                                \
        {                                                                                   \
            std::cout << __FUNCTION__ << " at line " << __LINE__ << " passed" << std::endl; \
        }                                                                                   \
    }

#define IS_STR_NOT_EQUAL(x, y)                                                              \
    {                                                                                       \
        if (x == y)                                                                         \
        {                                                                                   \
            std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl;       \
            std::cout << "Expected result to be: \t<" << y << ">" << std::endl;             \
            std::cout << "Got: \t\t\t<" << x << ">" << std::endl;                           \
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
    OAEP oaep1(512, 200);
    std::string input1("0123456");
    char result1[513];
    oaep1.addPadding(input1, result1);
    IS_STR_EQUAL(oaep1.removePadding(result1), input1);

    OAEP oaep2(768, 416);
    std::string input2("012345678901");
    char result2[769];
    oaep2.addPadding(input2, result2);
    IS_STR_EQUAL(oaep2.removePadding(result2), input2);

    // std::string input2();
    // IS_STR_EQUAL(oaep2.removePadding(oaep2.addPadding(input2)), input2);

    OAEP oaep3(896, 400);
    char result3[897];
    std::string input3(30, 'a');
    oaep3.addPadding(input3, result3);
    IS_STR_EQUAL(oaep3.removePadding(result3), input3);

    OAEP oaep4(1024, 512);
    char result4[1025];
    std::string input4(32, 'a');
    oaep4.addPadding(input4, result4);
    IS_STR_EQUAL(oaep4.removePadding(result4), input4);
}

void test_toBinary()
{
    IS_STR_EQUAL(toBinary("a"), "01100001");
    IS_STR_EQUAL(toBinary("aa"), "0110000101100001");
}

void test_fromBinary()
{
    char output[2];
    fromBinary(output, "0110000101100001");
    IS_STR_EQUAL(std::string(output), "aa");
    char output2[10];
    fromBinary(output2, "01100001011000100110001100110000001100000011000000110000001100000011000000110000");
    IS_STR_EQUAL(std::string(output2), "abc0000000")
}

void test_base64()
{
    const char *input = "abc";
    IS_STR_EQUAL(base64_decode(base64_encode(input, 3).c_str()), input);
    std::string input2(input);
    IS_STR_EQUAL(base64_decode(base64_encode(input2.c_str(), 3).c_str()), input);
}

void test_RSA()
{
    RSA_ rsa(1024);
    std::string input(32, 'a');
    std::string enc = rsa.encrypt(input);
    std::string dec = rsa.decrypt(enc);
    IS_STR_EQUAL(dec, input);
    RSA_ rsaLoad("key");
    IS_STR_EQUAL(rsaLoad.decrypt(enc), input);
    IS_STR_NOT_EQUAL(rsaLoad.encrypt(input), enc);

    RSA_ rsa1024(1536);
    std::string enc2 = rsa1024.encrypt(input);
    IS_STR_EQUAL(rsa1024.decrypt(enc2), input);

    RSA_ rsaLoad2("key");
    IS_STR_EQUAL(rsaLoad2.decrypt(enc2), input);
    IS_STR_NOT_EQUAL(rsaLoad2.encrypt(input), enc2);
}

void test_base64_and_rsa()
{
    // Create RSA_
    RSA_ rsa(80);
    // Encrypt
    std::string enc = rsa.encrypt("abc");
    // std::cout << enc.size();
    // Change to byte Array
    char encNotBin[rsa.getSize()];
    fromBinary(encNotBin, enc);
    // Encode in base64
    std::string enc64 = base64_encode(encNotBin, rsa.getSize() / 8);
    // Compare base64 decoded to binary
    std::string encFrom64 = base64_decode(enc64.c_str());
    IS_STR_EQUAL(encFrom64, encNotBin);
    // Convert to binary
    std::string encInBin = toBinary(encNotBin);
    IS_STR_EQUAL(encInBin, enc);
    IS_STR_EQUAL(rsa.decrypt(encInBin), "abc0000000");
}

int main(void)
{
    // Call all tests. Using a test framework would simplify this.
    // test_replaceSubString();
    test_addPadding();
    // test_toBinary();
    // test_fromBinary();
    test_RSA();
    // test_base64();
    // test_base64_and_rsa();
}