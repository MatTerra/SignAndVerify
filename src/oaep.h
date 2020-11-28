#ifndef OAEP_H
#define OAEP_H

#include <string>
#include <iostream>
#include <random>
#include <fstream>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <bits/stdc++.h>
#include <openssl/evp.h>

class OAEP
{
public:
  OAEP(int n, int k1);
  std::string addPadding(std::string);
  std::string removePadding(std::string);

private:
  const int n, k0, k1;
}
#endif
