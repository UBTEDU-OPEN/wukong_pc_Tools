/**
* @file CCipherBase.h
* @brief 加密算法
* @version 1.0
* @date 2010-05-21
*/

#ifndef _SERVER2_CRYPT_BASECIPHER_H_
#define _SERVER2_CRYPT_BASECIPHER_H_

#include <openssl/evp.h>
#include <openssl/pem.h>
#include "utilsdef.h"

//Todo
//现在实现了encrypt系列函数和base64系列
//没有实现的接口包括digist加密，比如MD5,如果使用md5来init，会返回false

namespace utils
{
    /**
    * @class CCipherBase
    * @brief 包装openssl的接口, 可以设置不同的加密方式，对称加密，公钥加密，digist加密
    *        openssl使用汇编实现可以提供较高的加密解密速度
    * @warning 具备一定的线程安全性，同样加密算法的工作者线程保证线程安全，其他情况调用者来保证线程安全性
    * @warning 全部函数异常安全
    */
    class UTILS_EXPORTS CCipherBase
    {
    public:
        //通用的标志
        //ecb——电子密码(Electronic Code Book)加密方式
        //cbc 加密块链(Cipher Block Chaining)加密方式
        //cfb 64位加密反馈(Cipher Feedbac)）加密方式
        //ofb 64位输出反馈(Output Feedback)加密方式
        //ede 该加密算法采用了加密、解密、加密的方式，第一个密钥和最后一个密钥是相同的
        //ede3该加密算法采用了加密、解密、加密的方式，但是三个密钥都不相同

        //支持的对称加密算法
        //NULL算法
        //    说明：该算法不作任何事情，也就是没有进行加密处理
        //DES算法
        //    说明：分别是CBC方式、ECB方式、CFB方式以及OFB方式的DES算法
        //使用两个密钥的3DES算法
        //    说明：分别是CBC方式、ECB方式、CFB方式以及OFB方式的3DES算法，算法的第一个密钥和最后一个密钥相同，事实上就只需要两个密钥
        //使用三个密钥的3DES算法
        //    说明：分别是CBC方式、ECB方式、CFB方式以及OFB方式的3DES算法，算法的三个密钥都不相同
        //DESX算法
        //    说明：CBC方式DESX算法
        //RC4算法
        //    RC4流加密算法。该算法的密钥长度可以改变，缺省是128位。
        //40位RC4算法
        //    说明：密钥长度40位的RC4流加密算法。
        //IDEA算法
        //    说明：分别是CBC方式、ECB方式、CFB方式以及OFB方式的IDEA算法。
        //RC2算法
        //    说明：分别是CBC方式、ECB方式、CFB方式以及OFB方式的RC2算法，该算法的密钥长度是可变的，可以通过设置有效密钥长度或有效密钥位来设置参数来改变。缺省的是128位。
        //定长的两种RC2算法
        //    说明：分别是40位和64位CBC模式的RC2算法。
        //Blowfish算法
        //    说明：分别是CBC方式、ECB方式、CFB方式以及OFB方式的Blowfish算法，该算法的密钥长度是可变的
        //CAST算法
        //    说明：分别是CBC方式、ECB方式、CFB方式以及OFB方式的CAST算法，该算法的密钥长度是可变的
        //RC5算法
        //    分别是CBC方式、ECB方式、CFB方式以及OFB方式的RC5算法，该算法的密钥长度可以根据参数“number of rounds”（算法中一个数据块被加密的次数）来设置，缺省的是128位密钥，加密次数为12次。目前来说，由于RC5算法本身实现代码的限制，加密次数只能设置为8、12或16。
        //128位AES算法
        //    说明：分别是CBC方式、ECB方式、CFB方式以及OFB方式的128位AES算法
        //192位AES算法
        //    说明：分别是CBC方式、ECB方式、CFB方式以及OFB方式的192位AES算法
        //256位AES算法
        //    分别是CBC方式、ECB方式、CFB方式以及OFB方式的256位AES算法
        static const char *s_base64;
        static const char *s_dsa;
        static const char *s_pkcs7_data;
        static const char *s_md5;
        static const char *s_des_ede3_cbc;
        //static const char *s_algorithm;
        static const char *s_des_ecb;
        static const char *s_des_cbc;
        static const char *s_des_ofb64;
        static const char *s_des_cfb64;
        static const char *s_des_ede_ecb;
        static const char *s_des_ede3_ecb;
        static const char *s_des_ede_cbc;
        static const char *s_des_ede_cfb64;
        static const char *s_des_ede3_cfb64;
        static const char *s_des_ede_ofb64;
        static const char *s_des_ede3_ofb64;
        static const char *s_desx_cbc;
        static const char *s_sha;
        static const char *s_rsa;
        static const char *s_aes_128_ecb;
        static const char *s_aes_128_cbc;
        static const char *s_aes_128_ofb128;
        static const char *s_aes_128_cfb128;
        static const char *s_aes_192_ecb;
        static const char *s_aes_192_cbc;
        static const char *s_aes_192_ofb128;
        static const char *s_aes_192_cfb128;
        static const char *s_aes_256_ecb;
        static const char *s_aes_256_cbc;
        static const char *s_aes_256_ofb128;
        static const char *s_aes_256_cfb128;
        static const char *s_aes_128_cfb1;
        static const char *s_aes_192_cfb1;
        static const char *s_aes_256_cfb1;
        static const char *s_aes_128_cfb8;
        static const char *s_aes_192_cfb8;
        static const char *s_aes_256_cfb8;
        static const char *s_des_cfb1;
        static const char *s_des_cfb8;
        static const char *s_des_ede3_cfb1;
        static const char *s_des_ede3_cfb8;
        static const char *s_sha256;
        static const char *s_sha384;
        static const char *s_sha512;
        static const char *s_sha224;
        


        //支持的公钥加密算法
        //支持的摘要算法


        typedef enum{
            BASE_CIPHER_SIGN_VERIFY_ERROR   = -14,  /*签名或是验证过程出错<*/
            BASE_CIPHER_NEED_BLOCKED        = -13, /*需要分成几块加密*/
            BASE_CIPHER_UNSUPPORT_OPERATION = -12, /*不支持的操作*/
            BASE_CIPHER_KEYPAIR_ERROR       = -11, /*生成密钥对失败*/
            BASE_CIPHER_INITPARAM_ERROR     = -10, /*加密解密的初始化参数不合理，可能是加密块的大小和key的长度不等<*/
            BASE_CIPHER_EVP_CTX_ERROR       = -9,  /*加密的EVP上下文错误<*/
            BASE_CIPHER_EVP_PAD_ERROR       = -8,  /*块加密的字节填充错误，可能是需要PSK算法填充却没有填充之类<*/
            BASE_CIPHER_LACK_OF_MEMORY      = -7,  /*内存大小不够<*/
            BASE_CIPHER_UPDATE_ERROR        = -6,  /*加密或是解密过程出错<*/
            BASE_CIPHER_FINAL_ERROR         = -5,  /*加密或是解密最后一段时候出错<*/
            BASE_CIPHER_WITHOUT_CIPHER      = -4,  /*没有提供密码本<*/
            BASE_CIPHER_WITHOUT_IV          = -3,  /*没有提供初次加密的初始化向量<*/
            BASE_CIPHER_CRYPT_INIT_ERROR    = -2,  /*加密解密engine初始化失败<*/
            BASE_CIPHER_UNSUFFICIENT_BUFLEN = -1,  /*输出buf分派的内存大小不够*/
            BASE_CIPHER_SUCCESS             = 0,   /*成功<*/
        }BASE_CIPHER_ERR;

        typedef struct RSAContext 
        {
            unsigned char* pubKey;
            int               pubKeyLen;
            unsigned char* priKey;
            int               priKeyLen;
            /*unsigned char* exponent;
            int               exponentLen;*/
        }t_rsaContext;
        
    public:
        CCipherBase();
        ~CCipherBase();

        /**
        * @fn init
        * @brief 初始化
        * @param cipherName:密码算法
        *
        * @return 成功等于true，否则false
        */
        bool init(const char* cipherName);

        /**
        * @fn doEncrypt
        * @brief 对称加密算法的通用接口
        * @param key:秘钥
        * @param kn:秘钥长度
        * @param iv:初始向量
        * @param in:初始向量长度
        * @param plaintext:原始文本
        * @param pn:原始文本长度
        * @param ciphertext:密码文本
        * @param cn:密码文本长度
        *
        * @return 成功等于0，否则小于0
        */
        virtual int doEncrypt(const unsigned char* key, int kn,
            const unsigned char* iv, int ivn,
            const unsigned char* plaintext, int pn,
            unsigned char* ciphertext, int& cn);

        int doEncrypt(const char* password,
            const unsigned char* plaintext, int pn,
            unsigned char *ciphertext, int& cn);
        /**
        * @fn doDecrypt
        * @brief 对称解密算法的通用接口
        *
        * @param key: 秘钥
        * @param kn: 秘钥长度
        * @param iv: 初始向量
        * @param in: 初始向量长度
        * @param plaintext: 原始文本
        * @param pn: 原始文本长度
        * @param ciphertext: 密码文本
        * @param cn: 密码文本长度
        *
        * @return 成功等于0，否则小于0
        */
        virtual int doDecrypt(const unsigned char* key, int kn,
            const unsigned char* iv, int ivn,
            unsigned char* plaintext, int& pn,
            const unsigned char* ciphertext, int cn);

        int doDecrypt(const char* password,
            unsigned char* plaintext, int& pn,
            const unsigned char* ciphertext, int cn);
        

        /**
        * @fn genKeyFromBytes
        * @brief 从输入密码产生了密钥key和初始化向量iv
        *        算法:D_i = HASH^count(D_(i-1) || data || salt)
        *
        * @param salt: 至少8 byte buffer or NULL
        * @param pw: 用户输入的密码, c－style string
        * @param count:重复hash的次数，重复次数越多越安全
        * @param key: 生成的key
        * @param iv: 生成的iv
        * @warn: key和iv的长度必须等于getKeyLen和getIvLen返回的长度
        * @return BASE_CIPHER_ERR
        */
        int genKeyFromBytes(const unsigned char *salt,
            const char *pw, int count,
            unsigned char *key, unsigned char *iv);

        /**
        * @fn getKeyLen
        * @brief 获得某一个密码本的默认key的长度
        *
        * @return key的长度
        */
        int getKeyLen() const;

        /**
        * @fn getIvLen
        * @brief 获得某一个密码本的默认Iv的长度
        *
        * @return iv的长度
        */
        int getIvLen() const;

        /**
        * @fn getBlockLen
        * @brief 获得某一个密码本的默认块大小
        *
        * @return 块大小
        */
        int getBlockLen() const;

        /**
        * @fn doBase64， undoBase64
        * @brief 对数据进行base64加解密，不需要进行init就可以使用
        *
        * @param plaintext: 原始内容
        * @param pn: (传入传出)参数，表示原始内容长度
        * @param ciphertext: 加密内容
        * @param cn: (传入传出)参数，表示加密内容长度
        * @return BASE_CIPHER_ERR
        */
        int doBase64(const unsigned char* plaintext, int pn,
                     unsigned char* ciphertext, int& cn);

        int undoBase64(unsigned char* plaintext, int& pn,
                     const unsigned char* ciphertext, int cn);

        /**
         * @fn getPKEY
         * @brief 获得公钥
         *
         * @param [in, out] ctx: 表示公钥，私钥，公钥长度，私钥长度
         * @param [out] lpPKEY: 表示公钥的数据结构， doRsa等算法中需要
         *
         */
        int getPKEY(t_rsaContext* ctx, EVP_PKEY** lpPKEY);

        /**
        * @fn getPKEY
        * @brief 获得公钥
        *
        * @param [in] ctx: 表示公钥，私钥，公钥长度，私钥长度
        * @param [out] lpPKEY: 表示公钥的数据结构， doRsa等算法中需要
        *
        */
        int getPKEY(t_rsaContext ctx, EVP_PKEY** lpPKEY);

        /**
        * @fn freePKEY
        * @brief 释放公钥
        *
        * @param [in] lpPKEY: 表示公钥
        *
        */
        void freePKEY(EVP_PKEY* lpPKEY);

        /**
        * @fn doRsa
        * @brief 做RSA加密，采用的是RSA_PKCS1_OAEP_PADDING填充方式， 加密用的exp是 0x10001
        *
        * @param [in] plaintext: 表示原文
        * @param [in] pn: 表示原文长度
        * @param [in，out] ciphertext: 表示密文
        * @param [in，out] cn: 表示密文长度
        * @param [in] pKey: 表示公钥
        *
        * @warn: 如果rsa加密失败，需要自己保证pKey被free
        *
        */
        int doRsa(const unsigned char* plaintext, int pn,
            unsigned char* ciphertext, int& cn,
            EVP_PKEY* pKey);

        /**
        * @fn undoRsa
        * @brief 做RSA解密，采用的是RSA_PKCS1_OAEP_PADDING填充方式， 加密用的exp是 0x10001
        *
        * @param [in，out] plaintext: 表示原文
        * @param [in，out] pn: 表示原文长度
        * @param [in] ciphertext: 表示密文
        * @param [in] cn: 表示密文长度
        * @param [in] pKey: 表示公钥
        *
        * @warn: 如果rsa加密失败，需要自己保证pKey被free
        *
        *
        */
        int undoRsa(unsigned char* plaintext, int& pn,
            const unsigned char* ciphertext, int cn,
            EVP_PKEY* pKey, bool freeKey = true);

        /**
        * @fn sign
        * @brief 做RSA签名
        *
        * @author wangjia
        *
        * @param [in] text: 表示原文
        * @param [in] textLen: 表示原文长度
        * @param [in，out] signValue: 表示签名
        * @param [in，out] signLen: 表示签名长度
        * @param [in] pKey: 表示公钥
        *
        */
        int sign(const unsigned char* text, unsigned int textLen,
            unsigned char *signValue, unsigned int& signLen,
            EVP_PKEY *pKey);

        /**
        * @fn sign
        * @brief 做RSA校验
        *
        * @author wangjia
        *
        * @param [in] text: 表示原文
        * @param [in] textLen: 表示原文长度
        * @param [in] signValue: 表示签名
        * @param [in] signLen: 表示签名长度
        * @param [in] pKey: 表示公钥
        *
        */
        int verify(const unsigned char *text, unsigned int textLen,
            const unsigned char *signValue, unsigned int signLen,
            EVP_PKEY *pKey);

        /**
        * @fn getPKEY
        * @brief 获得公钥（释放EVP_PKEY时，rsa不会被释放，由调用者负责释放）
        *
        * @author wangjia
        *
        * @param [in] rsa: rsa
        * @param [out] lpPKEY: 表示公钥的数据结构， doRsa等算法中需要
        *
        */
        int getPKEY(RSA* rsa, EVP_PKEY** lpPKEY);

        /**
        * 加载公私钥并隐藏 wangjia
        *
        */
        RSA *getPubKeyFromString(const char *pubKey);
        RSA *getPriKeyFromString(const char *priKey);

        RSA *getPubKeyFromFile(const char *pubKeyPath);
        RSA *getPriKeyFromFile(const char *priKeyPath);

        static void printBufHex( unsigned char *buf, int len );
    private:
        EVP_CIPHER *         m_cipher;
        int                  m_ivn;
        int                  m_bkn;
        int                     m_kyn;
        bool                 m_isPadding;
    };

    struct Aes256_Trait
    {
        const char* getName()
        {
            return CCipherBase::s_aes_256_cbc;
        }
    };


    struct Des_Trait
    {
        const char* getName()
        {
            return CCipherBase::s_des_cbc;
        }
    };

    struct Rsa_Trait
    {
        const char* getName()
        {
            return CCipherBase::s_rsa;
        }
    };

    struct Base64_Trait
    {
        const char* getName()
        {
            return CCipherBase::s_base64;
        }
    };


    class Base64
    {
    public:
        Base64();
        ~Base64();
        /**
        * 初始化签名算法
        * 
        * @return true 表示初始化成功
        */
        bool init();
        int doBase64(const unsigned char* plaintext, int pn,
                     unsigned char* ciphertext, int& cn);

        int undoBase64(unsigned char* plaintext, int& pn,
                     const unsigned char* ciphertext, int cn);
    private:
        CCipherBase m_cryptModule;
    };
}
#endif //_SERVER2_CRYPT_BASECIPHER_H_

