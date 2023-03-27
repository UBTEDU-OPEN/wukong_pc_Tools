#ifndef BASE_CIPHER_H_
#define BASE_CIPHER_H_

#include "utilsdef.h"
#include <stdint.h>
#include "type.h"
namespace utils
{
    enum CIPHER_STATUS
    {
        CIPHER_STATUS_OK,
        CIPHER_STATUS_BUFFER_ERROR,
        CIPHER_STATUS_UNKNOWN_ERROR
    };

    class UTILS_EXPORTS BaseCipher
    {
    public:
        virtual int32_t encrypt(const unsigned char* raw, const size_t& rawLen, unsigned char* dest, size_t* destLen) = 0;
        virtual int32_t decrypt(const unsigned char* raw, const size_t& rawLen, unsigned char* dest, size_t* destLen) = 0;
    };

}

#endif
