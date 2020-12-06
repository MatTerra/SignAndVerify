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
#include "str_utils.h"

class OAEP
{
public:
  OAEP(int n, int k1);
  OAEP(int n, int k0, int k1);
  int addPadding(std::string, char *);
  std::string removePadding(char *);

private:
  const int n, k0, k1;
};
#endif
