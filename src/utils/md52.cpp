#include <stdio.h>
#include <string>
#include <iostream>

#include "crypto_helpers.h" 

int main()
{
    std::cout << modsecurity::utils::crypto::md5("a") << std::endl;
    return 0;
}


