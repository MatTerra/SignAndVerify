#include <string>
#include <getopt.h>
#include <sysexits.h>
#include <fstream>
#include <sstream>

#include "rsa_.h"
#include "str_utils.h"
#include "base64.h"
#include "sha3_256.h"

enum Command
{
    NOCOMMAND = 0,
    SIGN = 1,
    VERIFY = 2,
    ENCRYPT = 3,
    DECRYPT = 4,
    GENERATE_KEYS = 5
};

int sign(std::string inFile, RSA_ *rsa, bool base64out)
{
    std::string outFile = inFile + ".sig";
    std::ifstream input(inFile);
    std::ofstream output(outFile);
    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string content = buffer.str();
    std::string hash = sha3_256(content);
    std::string signature = rsa->sign(hash);
    if (base64out)
    {
        char encBits[rsa->getSize()];
        fromBinary(encBits, signature);
        signature = base64_encode(encBits, rsa->getSize() / 8);
    }
    output << signature;
    input.close();
    output.close();
    return EXIT_SUCCESS;
}

int verify(std::string inFile, RSA_ *rsa, bool base64in)
{
    std::ifstream input(inFile);
    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string content = buffer.str();
    std::string hash = sha3_256(content);

    std::ifstream sigFile(inFile + ".sig");
    std::string toVerify;
    std::getline(sigFile, toVerify);

    TRACE("Read file");
    if (base64in)
    {
        std::string decoded = base64_decode(toVerify.c_str());
        toVerify = toBinary(decoded.substr(0, decoded.size() - 1));
    }

    if (toVerify.size() != rsa->getSize())
    {
        std::cerr << "Wrong signature size!";
        exit(EX_USAGE);
    }

    bool verified = rsa->verify(toVerify, hash);

    std::cout << verified << std::endl;

    input.close();
    sigFile.close();
}

int encrypt(std::string inFile, std::string outFile, RSA_ *rsa, bool base64out)
{

    if (outFile == "")
        outFile = inFile + ".enc";
    std::ifstream input(inFile);
    std::ofstream output(outFile);
    TRACE("Chose encrypt, reading file");
    std::string toEncrypt;
    std::getline(input, toEncrypt);
    if (toEncrypt.size() > rsa->getSize() / 8)
    {
        std::cerr << "File to long to encrypt using this key!";
        exit(EX_USAGE);
    }
    TRACE("Read file");
    std::string enc = rsa->encrypt(toEncrypt);
    std::cout << enc << std::endl;
    if (base64out)
    {
        char encBits[rsa->getSize()];
        fromBinary(encBits, enc);
        enc = base64_encode(encBits, rsa->getSize() / 8);
    }
    output << enc;
    input.close();
    output.close();
    return EXIT_SUCCESS;
}

int decrypt(std::string inFile, std::string outFile, RSA_ *rsa, bool base64in)
{

    if (outFile == "")
        outFile = inFile + ".dec";
    std::ifstream input(inFile);
    std::ofstream output(outFile);
    TRACE("Chose encrypt, reading file");
    std::string toDecrypt;
    std::getline(input, toDecrypt);

    TRACE("Read file");
    if (base64in)
    {
        std::string decoded = base64_decode(toDecrypt.c_str());
        toDecrypt = toBinary(decoded.substr(0, decoded.size() - 1));
    }
    std::cout << toDecrypt << std::endl;
    // toDecrypt.size();
    std::cout << "RSA key length is " << rsa->getSize() << std::endl;
    if (toDecrypt.size() != rsa->getSize())
    {
        std::cerr << "Wrong file size!";
        exit(EX_USAGE);
    }

    std::string dec = rsa->decrypt(toDecrypt);

    std::cout << dec << std::endl;

    output << dec;
    input.close();
    output.close();
}

int main(int argc, char **argv)
{
    unsigned int keyLength = -1;
    std::string keyFile = "key";
    bool generateKeys = false;
    int command = 0;
    std::string file = "";
    std::string output = "";
    bool base64out = false;

    int c;
    int digit_optind = 0;
    while (1)
    {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"bits", required_argument, 0, 'b'},
            {"key-file", required_argument, 0, 'k'},
            {"sign", required_argument, 0, 's'},
            {"verify", required_argument, 0, 'v'},
            {"encrypt", required_argument, 0, 'e'},
            {"decrypt", required_argument, 0, 'd'},
            {"output", required_argument, 0, 'o'},
            {"base64", no_argument, 0, 0},
            {0, 0, 0, 0}};

        c = getopt_long(argc, argv, "b:k:s:v:e:d:o:",
                        long_options, &option_index);
        if (c == -1)
            break;

        switch (c)
        {
        // Output-related arguments
        case 'o':
            // Output filename specified
            output = optarg;
            break;

        case 0:
            // Output should be base64
            if (long_options[option_index].name == "base64")
            {
                base64out = true;
            }
            break;

        // Key-related arguments
        case 'k':
            // Key file specified
            keyFile = optarg;
            break;

        case 'b':
            // Key length specified, will generate keys
            if (keyFile != "")
                keyLength = atoi(optarg);

            generateKeys = true;
            if (command == Command::NOCOMMAND)
                command = Command::GENERATE_KEYS;
            break;

        // Operation-related arguments
        case 's':
            // Will sign specified file
            if (command != Command::NOCOMMAND)
            {
                std::cout << "Two commands specified! Only one of -s, -v, -e or -d may be used at once.";
                exit(EX_USAGE);
            }
            command = Command::SIGN;
            file = optarg;
            break;
        case 'v':
            // Will verify signature in file
            if (command != Command::NOCOMMAND)
            {
                std::cout << "Two commands specified! Only one of -s, -v, -e or -d may be used at once.";
                exit(EX_USAGE);
            }
            command = Command::VERIFY;
            file = optarg;
            break;
        case 'e':
            // Will encrypt file
            if (command != Command::NOCOMMAND)
            {
                std::cout << "Two commands specified! Only one of -s, -v, -e or -d may be used at once.";
                exit(EX_USAGE);
            }
            command = Command::ENCRYPT;
            file = optarg;
            break;
        case 'd':
            // Will decrypt file
            if (command != Command::NOCOMMAND)
            {
                std::cout << "Two commands specified! Only one of -s, -v, -e or -d may be used at once.";
                exit(EX_USAGE);
            }
            command = Command::DECRYPT;
            file = optarg;
            break;

        case '?':
            exit(EX_USAGE);

        default:
            printf("?? getopt returned character code 0%o ??\n", c);
        }
    }
    TRACE("Done reading options");
    RSA_ *my_rsa;

    if (generateKeys && command != Command::NOCOMMAND)
    {
        if (keyLength == -1)
        {
            std::cout << "No key length specified, using 1024!";
            keyLength = 1024;
        }
        TRACE("Creating keys");
        my_rsa = new RSA_(keyLength, keyFile);
    }
    else
    {
        TRACE("Reading keys");
        my_rsa = new RSA_(keyFile);
    }

    TRACE("RSA loaded, starting command");

    switch (command)
    {
    case Command::SIGN:
        return sign(file, my_rsa, base64out);
    case Command::VERIFY:
        return verify(file, my_rsa, base64out);
    case Command::ENCRYPT:
        return encrypt(file, output, my_rsa, base64out);
    case Command::DECRYPT:
        return decrypt(file, output, my_rsa, base64out);
    case Command::GENERATE_KEYS:
        std::cout << "Keys generated and saved to " << keyFile << std::endl;
        break;

    default:
        std::cerr
            << "No command informed! Nothing done!";
        exit(EX_USAGE);
        break;
    }

    // std::cout << SHA3::calculate_file_hash("my_file");
    return 0;
}