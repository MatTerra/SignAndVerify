#include <iostream>
#include "str_utils.h"
#include "rsa.h"

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
        if (x.compare(y) != 0)                                                              \
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

// Test for function1()
// You would need to write these even when using a framework
void test_replaceSubString()
{
    IS_STR_EQUAL(replaceSubString("abcdef", "abc", "def"), "defdef");
    IS_STR_EQUAL(replaceSubString("abcdef", "abc", ""), "def");
    IS_STR_EQUAL(replaceSubString("abcdef", "abc", "defdef"), "defdefdef");
}

int main(void)
{
    // Call all tests. Using a test framework would simplify this.
    test_replaceSubString();
}