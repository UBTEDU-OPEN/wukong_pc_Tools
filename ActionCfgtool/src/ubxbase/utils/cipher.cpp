
#include "Cipher.h"
#include "base64.h"
#include "openssl/sha.h"
#include "openssl/md5.h"
#include "openssl/des.h"

namespace utils
{

    AESCipher::AESCipher()
    {

    }

    AESCipher::~AESCipher()
    {
        EVP_CIPHER_CTX_cleanup(&m_ectx);
        EVP_CIPHER_CTX_cleanup(&m_dctx);
    }

    bool AESCipher::init(unsigned char *key_data, int key_data_len)
    {
        int i, nrounds = 5;
        unsigned char key[32], iv[32];

        /*
        * Gen key & IV for AES 256 CBC mode. A SHA1 digest is used to hash the supplied key material.
        * nrounds is the number of times the we hash the material. More rounds are more secure but
        * slower.
        */
        i = EVP_BytesToKey(EVP_aes_256_cfb(), EVP_sha1(), NULL, key_data, key_data_len, nrounds, key, iv);
        if (i != 32) {
            //printf("Key size is %d bits - should be 256 bits/n", i);
            return false;
        }

        EVP_CIPHER_CTX_init(&m_ectx);
        EVP_EncryptInit_ex(&m_ectx, EVP_aes_256_cfb(), NULL, key, iv);

        EVP_CIPHER_CTX_init(&m_dctx);
        EVP_DecryptInit_ex(&m_dctx, EVP_aes_256_cfb(), NULL, key, iv);
        return true;
    }

       bool AESCipher::init2(unsigned char *key_data, int key_data_len)
    {
        int i, nrounds = 1;
        unsigned char key[32], iv[32];

        /*
        * Gen key & IV for AES 256 CBC mode. A SHA1 digest is used to hash the supplied key material.
        * nrounds is the number of times the we hash the material. More rounds are more secure but
        * slower.
        */
        i = EVP_BytesToKey(EVP_aes_256_cfb(), EVP_md5(), NULL, key_data, key_data_len, nrounds, key, iv);
        if (i != 32) {
            //printf("Key size is %d bits - should be 256 bits/n", i);
            return false;
        }

        EVP_CIPHER_CTX_init(&m_ectx);
        EVP_EncryptInit_ex(&m_ectx, EVP_aes_256_cfb(), NULL, key, iv);

        EVP_CIPHER_CTX_init(&m_dctx);
        EVP_DecryptInit_ex(&m_dctx, EVP_aes_256_cfb(), NULL, key, iv);
        return true;
    }

    int32_t AESCipher::encrypt(const unsigned char* raw, const size_t& rawLen, unsigned char* dest, size_t* destLen) 
    {

        /* max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes */
        int c_len = rawLen % AES_BLOCK_SIZE == 0 ? rawLen : (rawLen / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE; ;
        if (*destLen < (size_t)c_len)
        {
            return CIPHER_STATUS_BUFFER_ERROR;
        }
        
        int f_len = 0;
        if (!EVP_EncryptInit_ex(&m_ectx, NULL, NULL, NULL, NULL))
        {
            return CIPHER_STATUS_UNKNOWN_ERROR;
        }
        

        if (!EVP_EncryptUpdate(&m_ectx, dest, &c_len, raw, rawLen))
        {
            return CIPHER_STATUS_UNKNOWN_ERROR;
        }

        if (!EVP_EncryptFinal_ex(&m_ectx, dest+c_len, &f_len))
        {
            return CIPHER_STATUS_UNKNOWN_ERROR;
        }

        *destLen = c_len + f_len;
        return CIPHER_STATUS_OK;
    }

    int32_t AESCipher::decrypt(const unsigned char* raw, const size_t& rawLen, unsigned char* dest, size_t* destLen)
    {
        if (*destLen < rawLen)
        {
            return CIPHER_STATUS_BUFFER_ERROR;
        }

        int ret = 0;

        int p_len = rawLen;
        int f_len = 0;
        
        if (!EVP_DecryptInit_ex(&m_dctx, NULL, NULL, NULL, NULL))
        {
            return CIPHER_STATUS_UNKNOWN_ERROR;
        }

        if (!EVP_DecryptUpdate(&m_dctx, dest, &p_len, raw, rawLen))
        {
            return CIPHER_STATUS_UNKNOWN_ERROR;
        } 

        if (!EVP_DecryptFinal_ex(&m_dctx, dest+p_len, &f_len))
        {
            return CIPHER_STATUS_UNKNOWN_ERROR;
        }

        *destLen = p_len + f_len;
        
        return CIPHER_STATUS_OK;
    }

    std::shared_ptr<BaseCipher> createCipher(const char* key, const size_t& times)
    {
        unsigned char * md = MD5((unsigned char*)key, strlen(key), NULL);
        size_t realTime = times % 5;
        for (size_t i = 0; i < realTime; ++i)
        {
            unsigned char * raw = md;
            md = SHA1(raw, strlen((char *)raw), NULL);
        }
        std::shared_ptr<AESCipher> cipher(new AESCipher());
        cipher->init((unsigned char*)key, strlen(key));
        return cipher;
    }

    string encryptPassword( const string& strUser, const string& strPassword )
    {
        unsigned char keys[8] = {'r', 'z', 'r', 'k', '.', 'k', 'e', 'y'};
        for(size_t i=0; i<strUser.length() && i<sizeof(keys); i++)
        {
            keys[i] = keys[i] + strUser.at(i);
        }

        utils::AESCipher* pAesCipher = new AESCipher();
        pAesCipher->init2(keys, sizeof(keys));
        unsigned char out[1024] = {0};
        size_t outlen = sizeof(out);
        pAesCipher->encrypt((const unsigned char *)strPassword.c_str(), strPassword.length(), out, &outlen);
        if(outlen != strPassword.length())
        {
            delete pAesCipher;
            return strPassword;
        }
        delete pAesCipher;

        base64_t b64;
        base64_init(&b64, (const char*)&url_base64.alphabet[0]);
        char buf[512] = {0};
        int buflen = base64_encode(&b64, buf, out, outlen, BASE64_IGNORE_SPACE);

        string strResult = string(&buf[0], buflen);
        return strResult;
    }

    string decryptPassword( const string& strUser, const string& strPassword )
    {
        base64_t b64;
        base64_init(&b64, (const char*)&url_base64.alphabet[0]);
        char buf[512] = {0};
        int buflen = base64_decode(&b64, buf, strPassword.c_str(), strPassword.length(), BASE64_IGNORE_SPACE);

        unsigned char keys[8] = {'r', 'z', 'r', 'k', '.', 'k', 'e', 'y'};
        for(size_t i=0; i<strUser.length() && i<sizeof(keys); i++)
        {
            keys[i] = keys[i] + strUser.at(i);
        }
        utils::AESCipher* pAesCipher = new AESCipher();
        pAesCipher->init2(keys, sizeof(keys));
        unsigned char out[512] = {0};
        size_t outlen = sizeof(out);
        pAesCipher->decrypt((const unsigned char *)&buf[0], buflen, out, &outlen);
        if(outlen != buflen)
        {
            delete pAesCipher;
            return strPassword;
        }
        delete pAesCipher;
        string strResult = string((char *)out, outlen);
        return strResult;
    }

}