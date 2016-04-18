#include <stdio.h>
#include <string>
#include <iostream>

#include "mbedtls/md5.h"
#include "mbedtls/sha1.h"
#include "hexify.h"
#include "crypto_helpers.h"

std::string modsecurity::utils::crypto::md5( std::string input )
{
    const char unsigned * y = (unsigned char *)input.c_str();
    unsigned char md5sum[16];
    mbedtls_md5_context md5_ctx;
    modsecurity::utils::crypto::mbedtls_md5_init(&md5_ctx);
    modsecurity::utils::crypto::mbedtls_md5_starts(&md5_ctx);
    modsecurity::utils::crypto::mbedtls_md5_update(&md5_ctx,y,input.length());
    modsecurity::utils::crypto::mbedtls_md5_finish(&md5_ctx,md5sum);
    unsigned char output[32];
    modsecurity::utils::hexify(output,md5sum, 16);
    std::string myString(reinterpret_cast<char const*>(output), 31);
    return myString;
}

std::string modsecurity::utils::crypto::md5_raw( std::string input )
{
    const char unsigned * y = (unsigned char *)input.c_str();
    unsigned char md5sum[16];
    mbedtls_md5_context md5_ctx;
    modsecurity::utils::crypto::mbedtls_md5_init(&md5_ctx);
    modsecurity::utils::crypto::mbedtls_md5_starts(&md5_ctx);
    modsecurity::utils::crypto::mbedtls_md5_update(&md5_ctx,y,input.length());
    modsecurity::utils::crypto::mbedtls_md5_finish(&md5_ctx,md5sum);
    std::string retval( reinterpret_cast<char const*>(md5sum), 16 ) ;
    return retval;
}

std::string modsecurity::utils::crypto::sha1( std::string input )
{
    const char unsigned * y = (unsigned char *)input.c_str();
    unsigned char sha1sum[20];
    mbedtls_sha1_context sha1_ctx;
    modsecurity::utils::crypto::mbedtls_sha1_init(&sha1_ctx);
    modsecurity::utils::crypto::mbedtls_sha1_starts(&sha1_ctx);
    modsecurity::utils::crypto::mbedtls_sha1_update(&sha1_ctx,y,input.length());
    modsecurity::utils::crypto::mbedtls_sha1_finish(&sha1_ctx,sha1sum);
    unsigned char output[41];
    modsecurity::utils::hexify(output,sha1sum, 20);
    std::string myString(reinterpret_cast<char const*>(output), 40);
    return myString;
}

std::string modsecurity::utils::crypto::sha1_raw( std::string input )
{
    const char unsigned * y = (unsigned char *)input.c_str();
    unsigned char sha1sum[20];
    mbedtls_sha1_context sha1_ctx;
    modsecurity::utils::crypto::mbedtls_sha1_init(&sha1_ctx);
    modsecurity::utils::crypto::mbedtls_sha1_starts(&sha1_ctx);
    modsecurity::utils::crypto::mbedtls_sha1_update(&sha1_ctx,y,input.length());
    modsecurity::utils::crypto::mbedtls_sha1_finish(&sha1_ctx,sha1sum);
    std::string retval( reinterpret_cast<char const*>(sha1sum), 20 ) ;
    return retval;

}


