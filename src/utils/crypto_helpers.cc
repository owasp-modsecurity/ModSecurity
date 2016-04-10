#include <stdio.h>
#include <string>
#include <iostream>

#include "mbedtls/md5.h"
#include "hexify.h"
#include "crypto_helpers.h"

std::string modsecurity::utils::crypto::md5( std::string input )
{
    const char unsigned * y = (unsigned char *)input.c_str();
    unsigned char md5sum[16];
    modsecurity::utils::crypto::mbedtls_md5_context md5_ctx;
    modsecurity::utils::crypto::mbedtls_md5_init(&md5_ctx);
    modsecurity::utils::crypto::mbedtls_md5_starts(&md5_ctx);
    modsecurity::utils::crypto::mbedtls_md5_update(&md5_ctx,y,input.length());
    modsecurity::utils::crypto::mbedtls_md5_finish(&md5_ctx,md5sum);
    unsigned char output[32];
    modsecurity::utils::hexify(output,md5sum, 16);
    std::string myString(reinterpret_cast<char const*>(output), 32);
    return myString;
}

