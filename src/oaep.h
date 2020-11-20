#ifndef OAEP_H
#define OAEP_H

#include <string>

class OAEP
{
public:
   OAEP(int n, int k0, int k1) : n(n), k0(k0), k1(k1){};
   std::string addPadding(std::string);

private:
   int n, k0, k1;
};

#endif
