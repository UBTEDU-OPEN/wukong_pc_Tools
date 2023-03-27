#ifndef CIPHER_H_
#define CIPHER_H_

#include "utilsdef.h"
#include "BaseCipher.h"
#include "openssl/aes.h"
#include "openssl/evp.h"
#include "openssl/ossl_typ.h"
#include <memory>
#include <string>
using std::string;
namespace utils
{
    class UTILS_EXPORTS AESCipher : public BaseCipher
    {
    public:
        AESCipher();
        ~AESCipher();

        bool init(unsigned char *key_data, int key_data_len);
        bool init2(unsigned char *key_data, int key_data_len);

        virtual int32_t encrypt(const unsigned char* raw, const size_t& rawLen, unsigned char* dest, size_t* destLen) ;

        virtual int32_t decrypt(const unsigned char* raw, const size_t& rawLen, unsigned char* dest, size_t* destLen) ;

    private:
        EVP_CIPHER_CTX m_ectx;
        EVP_CIPHER_CTX m_dctx;
    };

    UTILS_EXPORTS std::shared_ptr<BaseCipher> createCipher(const char* key, const size_t& times);

    UTILS_EXPORTS string encryptPassword(const string &strUser, const string &strPassword);
    UTILS_EXPORTS string decryptPassword(const string &strUser, const string &strPassword);
}

#endif
