#include <cassert>
#include <cstring>

#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/evp.h>

#include <utils/CCipherBase.h>

namespace utils
{
    const char *CCipherBase::s_base64                   = "base64";
    const char *CCipherBase::s_dsa                      = "dsaEncryption";
    const char *CCipherBase::s_pkcs7_data               = "pkcs7-data";
    const char *CCipherBase::s_md5                      = "md5";
    const char *CCipherBase::s_des_ede3_cbc             = "des-ede3-cbc";
    //const char *CCipherBase::s_algorithm              = "algorithm";
    const char *CCipherBase::s_des_ecb                  = "des-ecb";
    const char *CCipherBase::s_des_cbc                  = "des-cbc";
    const char *CCipherBase::s_des_ofb64                = "des-ofb";
    const char *CCipherBase::s_des_cfb64                = "des-cfb";
    const char *CCipherBase::s_des_ede_ecb              = "des-ede";
    const char *CCipherBase::s_des_ede3_ecb             = "des-ede3";
    const char *CCipherBase::s_des_ede_cbc              = "des-ede-cbc";
    const char *CCipherBase::s_des_ede_cfb64            = "des-ede-cfb";
    const char *CCipherBase::s_des_ede3_cfb64           = "des-ede3-cfb";
    const char *CCipherBase::s_des_ede_ofb64            = "des-ede-ofb";
    const char *CCipherBase::s_des_ede3_ofb64           = "des-ede3-ofb";
    const char *CCipherBase::s_desx_cbc                 = "desx-cbc";
    const char *CCipherBase::s_sha                      = "sha";
    const char *CCipherBase::s_rsa                      = "rsa";
    const char *CCipherBase::s_aes_128_ecb              = "aes-128-ecb";
    const char *CCipherBase::s_aes_128_cbc              = "aes-128-cbc";
    const char *CCipherBase::s_aes_128_ofb128           = "aes-128-ofb";
    const char *CCipherBase::s_aes_128_cfb128           = "aes-128-cfb";
    const char *CCipherBase::s_aes_192_ecb              = "aes-192-ecb";
    const char *CCipherBase::s_aes_192_cbc              = "aes-192-cbc";
    const char *CCipherBase::s_aes_192_ofb128           = "aes-192-ofb";
    const char *CCipherBase::s_aes_192_cfb128           = "aes-192-cfb";
    const char *CCipherBase::s_aes_256_ecb              = "aes-256-ecb";
    const char *CCipherBase::s_aes_256_cbc              = "aes-256-cbc";
    const char *CCipherBase::s_aes_256_ofb128           = "aes-256-ofb";
    const char *CCipherBase::s_aes_256_cfb128           = "aes-256-cfb";
    const char *CCipherBase::s_aes_128_cfb1             = "aes-128-cfb1";
    const char *CCipherBase::s_aes_192_cfb1             = "aes-192-cfb1";
    const char *CCipherBase::s_aes_256_cfb1             = "aes-256-cfb1";
    const char *CCipherBase::s_aes_128_cfb8             = "aes-128-cfb8";
    const char *CCipherBase::s_aes_192_cfb8             = "aes-192-cfb8";
    const char *CCipherBase::s_aes_256_cfb8             = "aes-256-cfb8";
    const char *CCipherBase::s_des_cfb1                 = "des-cfb1";
    const char *CCipherBase::s_des_cfb8                 = "des-cfb8";
    const char *CCipherBase::s_des_ede3_cfb1            = "des-ede3-cfb1";
    const char *CCipherBase::s_des_ede3_cfb8            = "des-ede3-cfb8";
    const char *CCipherBase::s_sha256                   = "sha256";
    const char *CCipherBase::s_sha384                   = "sha384";
    const char *CCipherBase::s_sha512                   = "sha512";
    const char *CCipherBase::s_sha224                   = "sha224";



    CCipherBase::CCipherBase():
    m_cipher(NULL),
    m_ivn(0),
    m_bkn(0),
    m_kyn(0),
    m_isPadding(true)
    {

    }

    CCipherBase::~CCipherBase()
    {
        //Todo openssl不需要m_cipher的资源 EVP_cleanup();
    }

    void CCipherBase::printBufHex( unsigned char *buf, int len )
    {
        int i;

        for( i = 0; i < len; i++ )
        {
            printf( "%02x", buf[i] );
        }
    }

    bool CCipherBase::init(const char* cipherName)
    {
        bool ret = false;
        OpenSSL_add_all_ciphers();
        OpenSSL_add_all_algorithms();
        OpenSSL_add_all_digests();

        /*SSL_load_error_strings();
        ERR_load_crypto_strings();*/
        if (!strcmp(cipherName, s_rsa))
        {
            m_ivn = 0;
            m_kyn = 0;
            m_bkn = 0;
            return true;
        }
        
        if (!strcmp(cipherName, s_base64))
        {
            m_ivn = 0;
            m_kyn = 0;
            m_bkn = 0;
            return true;
        }
        
        m_cipher = (EVP_CIPHER*)EVP_get_cipherbyname(cipherName);

        if (m_cipher)
        {
            m_ivn = EVP_CIPHER_iv_length(m_cipher);
            m_kyn = EVP_CIPHER_key_length(m_cipher);
            m_bkn = EVP_CIPHER_block_size(m_cipher);

            ret = true;
        }
        return ret;
    }

    int CCipherBase::getBlockLen() const
    {
        return m_bkn;
    }

    int CCipherBase::getIvLen() const
    {
        return m_ivn;
    }

    int CCipherBase::getKeyLen() const
    {
        return m_kyn;
    }

    int CCipherBase::genKeyFromBytes(const unsigned char *salt,
                        const char *pw, int count,
                        unsigned char *key, unsigned char *iv)
    {
        /*
        * 将密码与之前设定的 iv(initial vector) 转换为加密时所需的 key。
        *
        * 其中 "salt" 即为 UNIX 系统中 /etc/passwd 中密码栏位所使用的
        * salt，至少必须是八个 bytes。
        *
        * 而参数中的那个 1 表示在 BytesToKey 以 hashing 产生 key 的过程中，
        * 重覆几次 hashing 的动作，愈多次愈花时间，但相对地也愈安全。
        *
        * EVP_md5() 表示过程中所使用的 hash 演算法。
        *
        * EVP_idea_ecb() 表示产生的 key 是给哪一种 cipher 用的。
        */
        if(!m_cipher)
            return BASE_CIPHER_WITHOUT_CIPHER;
        int keysize = EVP_BytesToKey(m_cipher, EVP_md5(), salt, (unsigned char *)pw, strlen(pw),
            count, key, iv);
        assert(keysize == m_kyn);
        return BASE_CIPHER_SUCCESS;
    }


    int CCipherBase::doEncrypt(const unsigned char* key, int kn,
                          const unsigned char* iv, int ivn,
                          const unsigned char* plaintext, int pn,
                          unsigned char* ciphertext, int& cn)
    {
        if((!m_cipher) || (pn <= 0))
            return (BASE_CIPHER_WITHOUT_CIPHER);

        int /*i,*/ol,tmp;
        ol = 0;
        EVP_CIPHER_CTX ctx;
        if(kn != m_kyn)
        {
            return (BASE_CIPHER_INITPARAM_ERROR);
        }
        EVP_CIPHER_CTX_init(&ctx);
        const int maxoutl = pn + m_bkn;
        unsigned char* out = (unsigned char*)malloc(maxoutl);
        if (!out)
        {
            EVP_CIPHER_CTX_cleanup(&ctx);
            fprintf(stderr, "Bad alloc heap memory [%d]\n", maxoutl);
            return (BASE_CIPHER_LACK_OF_MEMORY);
        }
        memset((void *)out, 0, maxoutl);
        if(!EVP_EncryptInit_ex(&ctx, m_cipher, NULL, key, iv))
        {
            free(out);
            EVP_CIPHER_CTX_cleanup(&ctx);
            return (BASE_CIPHER_CRYPT_INIT_ERROR);
        }
        //EVP_CIPHER_CTX_set_padding(&ctx, 1);
        /*for (i = 0;  i < pn / 100;  i++) {
            if (!EVP_EncryptUpdate(&ctx, &out[ol], &tmp, &plaintext[ol], 100))
            {
                free(out);
                EVP_CIPHER_CTX_cleanup(&ctx);
                return (BASE_CIPHER_UPDATE_ERROR);
            }
            ol += tmp;
        }
        if (pn % 100) {
            if (!EVP_EncryptUpdate(&ctx, &out[ol], &tmp, &plaintext[ol], pn % 100))
            {
                free(out);
                EVP_CIPHER_CTX_cleanup(&ctx);
                return (BASE_CIPHER_UPDATE_ERROR);
            }
            ol += tmp;
        }*/
        if (!EVP_EncryptUpdate(&ctx, &out[ol], &tmp, plaintext, pn))
        {
            free(out);
            EVP_CIPHER_CTX_cleanup(&ctx);
            return (BASE_CIPHER_UPDATE_ERROR);
        }
        ol += tmp;

        //只有开启了padding的时候有效
        if (!EVP_EncryptFinal_ex(&ctx, &out[ol], &tmp))
        {
            free(out);
            EVP_CIPHER_CTX_cleanup(&ctx);
            return (BASE_CIPHER_FINAL_ERROR);
        }
        ol += tmp;
        if(cn < ol)
        {
            free(out);
            EVP_CIPHER_CTX_cleanup(&ctx);
            return (BASE_CIPHER_UNSUFFICIENT_BUFLEN);
        }
        else
        {
            memcpy(ciphertext, (const void *)out, ol);
            free(out);
            cn = ol;
        }
        EVP_CIPHER_CTX_cleanup(&ctx);
        return (BASE_CIPHER_SUCCESS);
    }


    int CCipherBase::doEncrypt(const char* password,
                  const unsigned char* plaintext, int pn,
                  unsigned char *ciphertext, int& cn)
    {
        unsigned char *key = (unsigned char *)malloc(m_kyn);
        if(!key)
            return BASE_CIPHER_LACK_OF_MEMORY;
        unsigned char *iv = (unsigned char *)malloc(m_ivn);
        if(!iv)
            return BASE_CIPHER_LACK_OF_MEMORY;
        int ret = genKeyFromBytes(NULL, password, 1, key, iv);
        if(!ret)
        {
            ret = doEncrypt(key, m_kyn,
                            iv, m_ivn,
                                plaintext, pn,
                                ciphertext, cn);
            free(key);
            return ret;
        }
        else
        {
            free(key);
            return ret;
        }
    }

    int CCipherBase::doDecrypt(const unsigned char* key, int kn,
                          const unsigned char* iv, int ivn,
                          unsigned char* plaintext, int& pn,
                          const unsigned char* ciphertext, int cn)
    {
        if((!m_cipher) || (cn <= 0))
            return (BASE_CIPHER_WITHOUT_CIPHER);

        int outl, outtaill;

        EVP_CIPHER_CTX ctx;
        if(kn != m_kyn)
        {
            return (BASE_CIPHER_INITPARAM_ERROR);
        }
        EVP_CIPHER_CTX_init(&ctx);
        const int maxoutl = cn + m_bkn;
        unsigned char* out = (unsigned char*)malloc(maxoutl);
        if (!out)
        {
            EVP_CIPHER_CTX_cleanup(&ctx);
            fprintf(stderr, "Bad alloc heap memory [%d]\n", maxoutl);
            return (BASE_CIPHER_LACK_OF_MEMORY);
        }
        memset((void *)out, 0, maxoutl);
        if(!EVP_DecryptInit_ex(&ctx, m_cipher, NULL, key, iv))
        {
            EVP_CIPHER_CTX_cleanup(&ctx);
            free(out);
            return (BASE_CIPHER_CRYPT_INIT_ERROR);
        }

        if(!EVP_DecryptUpdate(&ctx, &out[0], &outl, ciphertext, cn))
        {
            EVP_CIPHER_CTX_cleanup(&ctx);
            free(out);
            return (BASE_CIPHER_UPDATE_ERROR);
        }

        if(!EVP_DecryptFinal_ex(&ctx, &out[outl], &outtaill))
        {
            EVP_CIPHER_CTX_cleanup(&ctx);
            free(out);
            return (BASE_CIPHER_FINAL_ERROR);
        }

        int outlen = outtaill + outl;
        if(pn < outlen)
        {
            EVP_CIPHER_CTX_cleanup(&ctx);
            free(out);
            return (BASE_CIPHER_UNSUFFICIENT_BUFLEN);
        }
        else
        {
            memcpy(plaintext, (const void *)&out[0], outlen);
            free(out);
            pn = outlen;
        }
        EVP_CIPHER_CTX_cleanup(&ctx);
        return (BASE_CIPHER_SUCCESS);
    }

    int CCipherBase::doDecrypt(const char* password,
                  unsigned char* plaintext, int& pn,
                  const unsigned char* ciphertext, int cn)
    {
        unsigned char *key = (unsigned char *)malloc(m_kyn);
        if(!key)
            return BASE_CIPHER_LACK_OF_MEMORY;
        unsigned char *iv = (unsigned char *)malloc(m_ivn);
        if(!iv)
            return BASE_CIPHER_LACK_OF_MEMORY;
        int ret = genKeyFromBytes(NULL, password, 1, key, iv);
        if(!ret)
        {
            ret = doDecrypt(key, m_kyn,
                iv, m_ivn,
                plaintext, pn,
                ciphertext, cn);
            free(key);
            return ret;
        }
        else
        {
            free(key);
            return ret;
        }
    }

    int CCipherBase::doBase64(const unsigned char* plaintext, int pn,
                 unsigned char* ciphertext, int& cn)
    {
        EVP_ENCODE_CTX    ctx;
        EVP_EncodeInit(&ctx);

        int olen = 2 * pn;
        int tmp, ol;
        ol = 0;
        unsigned char* obuf = (unsigned char *)malloc(olen);
        if(!obuf)
        {
            fprintf(stderr, "Bad alloc heap memory [%d]\n", olen);
            return BASE_CIPHER_LACK_OF_MEMORY;
        }
        EVP_EncodeUpdate(&ctx, &obuf[ol], &tmp, plaintext, pn);
        ol += tmp;
        EVP_EncodeFinal(&ctx, &obuf[ol], &tmp);
        ol += tmp;
        if ( cn < ol )
        {
            free(obuf);
            return BASE_CIPHER_UNSUFFICIENT_BUFLEN;
        }
        else
        {
            memcpy(ciphertext, obuf, ol);
            free(obuf);
            cn = ol;
        }
        return BASE_CIPHER_SUCCESS;
    }

    int CCipherBase::undoBase64(unsigned char* plaintext, int& pn,
                   const unsigned char* ciphertext, int cn)
    {
        EVP_ENCODE_CTX    ctx;
        EVP_DecodeInit(&ctx);

        int olen = 2 * cn;
        int tmp, ol;
        ol = 0;
        unsigned char* obuf = (unsigned char *)malloc(olen);
        if(!obuf)
        {
            fprintf(stderr, "Bad alloc heap memory [%d]\n", olen);
            return BASE_CIPHER_LACK_OF_MEMORY;
        }
        int ret = EVP_DecodeUpdate(&ctx, &obuf[ol], &tmp, ciphertext, cn);
        if(ret < 0)
        {
            free(obuf);
            return BASE_CIPHER_UPDATE_ERROR;
        }
        ol += tmp;
        if(!EVP_DecodeFinal(&ctx, &obuf[ol], &tmp))
        {
            free(obuf);
            return BASE_CIPHER_FINAL_ERROR;
        }
        ol += tmp;
        if ( pn < ol )
        {
            free(obuf);
            return BASE_CIPHER_UNSUFFICIENT_BUFLEN;
        }
        else
        {
            memcpy(plaintext, obuf, ol);
            free(obuf);
            pn = ol;
        }
        return BASE_CIPHER_SUCCESS;
    }

    int CCipherBase::getPKEY(t_rsaContext* ctx, EVP_PKEY** lpPKEY)
    {
        //The exponent is an odd number, typically 3, 17 or 65537.
        RSA* rsa=RSA_generate_key(1024,//公钥模长
            RSA_F4,                        //typically 3, 17 or 65537第三个费尔玛数作为公钥中的e
            NULL,                   //素数产生状态的回调函数
            NULL                   //传给回调函数的参数
            );
        if(NULL==rsa)
        {
            //生成RSA密钥对失败
            return BASE_CIPHER_KEYPAIR_ERROR;
        }
        //隐藏RSA密钥抵御定时攻击
        RSA_blinding_on(rsa,NULL);
        
        int pubKeyLen = BN_num_bytes(rsa->n);
        int priKeyLen = BN_num_bytes(rsa->d);
        //int expLen = BN_num_bytes(rsa->e);
        
        if ((ctx->pubKeyLen < pubKeyLen) || (ctx->priKeyLen < priKeyLen) /*|| (ctx.exponentLen < expLen)*/)
        {
            RSA_free(rsa);
            return BASE_CIPHER_UNSUFFICIENT_BUFLEN;
        }
        unsigned char* pubKey = (unsigned char*)malloc(pubKeyLen);
        unsigned char* priKey = (unsigned char*)malloc(priKeyLen);
        if ((NULL == pubKey) || (NULL == priKey))
        {
            RSA_free(rsa);
            return BASE_CIPHER_LACK_OF_MEMORY;
        }
        
        //BN_bn2bin( rsa->e, ctx.exponent );

        BN_bn2bin( rsa->n, pubKey ); // 保存公钥
        BN_bn2bin( rsa->d, priKey ); // 保存私钥

    //#ifdef _DEBUG
    //    printf("\n===================================\nPublicKey:\n");
    //    printBufHex(pubKey, pubKeyLen);
    //    printf("\n===================================\nPrivateKey:\n");
    //    printBufHex(priKey, priKeyLen);
    //    printf("\n===================================\n");
    //#endif

        ctx->priKeyLen = priKeyLen;
        ctx->pubKeyLen = pubKeyLen;
        memcpy(ctx->pubKey, pubKey, pubKeyLen);
        memcpy(ctx->priKey, priKey, priKeyLen);
        free(pubKey);
        free(priKey);
        //将rsa对象赋给EVP_PKEY结构
        *lpPKEY =EVP_PKEY_new();
        if(NULL==*lpPKEY)
        {
            RSA_free(rsa);
            return BASE_CIPHER_LACK_OF_MEMORY;
        }
        EVP_PKEY_assign_RSA(*lpPKEY, rsa);
        //因为PKEY和RSA可以同时释放，所以不在这个api中free
        return BASE_CIPHER_SUCCESS;
    }

    int CCipherBase::getPKEY(t_rsaContext ctx, EVP_PKEY** lpPKEY)
    {
        RSA* rsa = RSA_new();
        if(NULL==rsa)
        {
            //生成RSA密钥对失败
            return BASE_CIPHER_KEYPAIR_ERROR;
        }
        //隐藏RSA密钥抵御定时攻击
        RSA_blinding_on(rsa,NULL);

        const char* exp = "\x01\x00\x01";
        rsa->n = BN_bin2bn( ctx.pubKey, ctx.pubKeyLen, rsa->n );
        rsa->e = BN_bin2bn( (const unsigned char*)exp, 3, rsa->e );
        rsa->d = BN_bin2bn( ctx.priKey, ctx.priKeyLen, rsa->d );
        //将rsa对象赋给EVP_PKEY结构
        *lpPKEY =EVP_PKEY_new();
        if(NULL==*lpPKEY)
        {
            RSA_free(rsa);
            return BASE_CIPHER_LACK_OF_MEMORY;
        }
        EVP_PKEY_assign_RSA(*lpPKEY, rsa);
        //因为PKEY和RSA可以同时释放，所以不在这个api中free
        return BASE_CIPHER_SUCCESS;
    }

    int CCipherBase::doRsa(const unsigned char* plaintext, int pn,
              unsigned char* ciphertext, int& cn,
              EVP_PKEY* pKey)
    {
        if (pn > 86)
            return BASE_CIPHER_NEED_BLOCKED;    
        if (NULL == pKey)
            return BASE_CIPHER_KEYPAIR_ERROR;
    #if OPENSSL_VERSION_NUMBER>=0x01000000L
        EVP_PKEY_CTX *ctx;
        ctx = EVP_PKEY_CTX_new(pKey, NULL);
        if (!ctx)
            return BASE_CIPHER_EVP_CTX_ERROR;
        if (EVP_PKEY_encrypt_init(ctx) <= 0)
            return BASE_CIPHER_EVP_CTX_ERROR;

        //RSA_PKCS1_PADDING for PKCS#1 padding, 
        //RSA_SSLV23_PADDING for SSLv23 padding, 
        //RSA_NO_PADDING for no padding, 
        //RSA_PKCS1_OAEP_PADDING for OAEP padding (encrypt and decrypt only), 
        //RSA_X931_PADDING for X9.31 padding (signature operations only),
        //RSA_PKCS1_PSS_PADDING (sign and verify only).
        if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0)
            return BASE_CIPHER_EVP_CTX_ERROR;

        /* Determine buffer length */
        size_t outlen = 0;
        int ret = 1;
        if ((ret = EVP_PKEY_encrypt(ctx, NULL, &outlen, plaintext, pn)) <= 0)
        {
            /*char errorInfo[1024] = {0};
            ERR_error_string_n(ret, errorInfo, 1023);
            printf("%s\n", errorInfo);*/
            if(ret == -2)
                return BASE_CIPHER_UNSUPPORT_OPERATION;
            else
                return BASE_CIPHER_UPDATE_ERROR; /* Error */
        }
        if ((size_t)cn < outlen)
            return BASE_CIPHER_UNSUFFICIENT_BUFLEN;
        if ((ret = EVP_PKEY_encrypt(ctx, ciphertext, &outlen, plaintext, pn)) <= 0)
        {
            /*char errorInfo[1024] = {0};
            ERR_error_string_n(ret, errorInfo, 1023);
            printf("%s\n", errorInfo);*/
            if(ret == -2)
                return BASE_CIPHER_UNSUPPORT_OPERATION;
            else
                return BASE_CIPHER_UPDATE_ERROR; /* Error */
        }
        cn = outlen;
        EVP_PKEY_CTX_free(ctx);
        //释放EVP_PKEY对象，其关联的RSA对象也被同时释放。
        EVP_PKEY_free(pKey);
        pKey = NULL;
        return BASE_CIPHER_SUCCESS;
    #else
        int ret = BASE_CIPHER_SUCCESS;
        int outlen = 0 ;
        int keylen = EVP_PKEY_size(pKey);
        int pos = 0 ;
        do{
          if(pos+keylen>cn){
	    ret = BASE_CIPHER_UNSUFFICIENT_BUFLEN;
	    break;
          }
          outlen = RSA_public_encrypt(pn-pos,plaintext+pos,ciphertext+pos,pKey->pkey.rsa,RSA_PKCS1_PADDING);
          if(outlen<0){
	    ret = BASE_CIPHER_UPDATE_ERROR;
	    break;
          }
          pos += outlen;
         
        }while(pos<pn);

        EVP_PKEY_free(pKey);
        return ret;
    #endif
    }

    void CCipherBase::freePKEY(EVP_PKEY* lpPKEY)
    {
        EVP_PKEY_free(lpPKEY);
    }

    int CCipherBase::undoRsa(unsigned char* plaintext, int& pn,
                const unsigned char* ciphertext, int cn,
                EVP_PKEY* pKey, bool freeKey)
    {
    #if OPENSSL_VERSION_NUMBER > 0x01000000L
        if (NULL == pKey)
            return BASE_CIPHER_KEYPAIR_ERROR;

        EVP_PKEY_CTX *ctx;
        ctx = EVP_PKEY_CTX_new(pKey, NULL);
        if (!ctx)
            return BASE_CIPHER_EVP_CTX_ERROR;
        if (EVP_PKEY_decrypt_init(ctx) <= 0)
            return BASE_CIPHER_EVP_CTX_ERROR;
        //RSA_PKCS1_PADDING for PKCS#1 padding, 
        //RSA_SSLV23_PADDING for SSLv23 padding, 
        //RSA_NO_PADDING for no padding, 
        //RSA_PKCS1_OAEP_PADDING for OAEP padding (encrypt and decrypt only), 
        //RSA_X931_PADDING for X9.31 padding (signature operations only),
        //RSA_PKCS1_PSS_PADDING (sign and verify only).
        if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0)
            return BASE_CIPHER_EVP_CTX_ERROR;
        /* Determine buffer length */
        size_t outlen = 0;
        if (EVP_PKEY_decrypt(ctx, NULL, &outlen, ciphertext, cn) <= 0)
            return BASE_CIPHER_UPDATE_ERROR; /* Error */
        if ((size_t)pn < outlen)
            return BASE_CIPHER_UNSUFFICIENT_BUFLEN;
        pn = outlen;
        int ret = EVP_PKEY_decrypt(ctx, plaintext, &outlen, ciphertext, cn);
        if (ret <= 0)
            return BASE_CIPHER_UPDATE_ERROR;
        pn = outlen;
        EVP_PKEY_CTX_free(ctx);
        //释放EVP_PKEY对象，其关联的RSA对象也被同时释放。
        if (freeKey)
        {
            EVP_PKEY_free(pKey);
            pKey = NULL;
        }
        return BASE_CIPHER_SUCCESS;
    #else
        int ret = BASE_CIPHER_SUCCESS;
        int outlen =0;
        int pos = 0 ; 
        int keylen = EVP_PKEY_size(pKey);
        do{
          if(pos+keylen>pn){
	    ret = BASE_CIPHER_UNSUFFICIENT_BUFLEN;
	    break;
          }
          outlen = RSA_public_decrypt(cn-pos,ciphertext+pos,plaintext+pos,pKey->pkey.rsa,RSA_PKCS1_OAEP_PADDING);
          if(outlen<0){
	    ret =BASE_CIPHER_UPDATE_ERROR;
	    break;
          }
          pos += outlen;
        }while(pos<cn);
        EVP_PKEY_free(pKey);
        return ret;
    #endif
    }

    int CCipherBase::sign(const unsigned char* text, unsigned int textLen,
        unsigned char *signValue, unsigned int& signLen,
        EVP_PKEY *pKey)
    {  
        if (NULL == pKey)
        {
            return BASE_CIPHER_KEYPAIR_ERROR;
        }

        EVP_MD_CTX mdctx; //摘要算法上下文变量
        EVP_MD_CTX_init(&mdctx); //初始化摘要上下文

        if (!EVP_SignInit_ex(&mdctx, EVP_md5(), NULL)) //签名初始化，设置摘要算法
        {
            EVP_PKEY_free(pKey);
            return BASE_CIPHER_SIGN_VERIFY_ERROR; /* Error */
        }

        if (!EVP_SignUpdate(&mdctx, text, textLen)) //计算签名（摘要）Update
        {
            EVP_PKEY_free(pKey);
            return BASE_CIPHER_SIGN_VERIFY_ERROR; /* Error */
        }

        if (!EVP_SignFinal(&mdctx, signValue, &signLen, pKey))  //签名输出
        {
            EVP_PKEY_free(pKey);
            EVP_MD_CTX_cleanup(&mdctx);
            return BASE_CIPHER_SIGN_VERIFY_ERROR; /* Error */
        }

        EVP_PKEY_free(pKey);
        EVP_MD_CTX_cleanup(&mdctx);
        return BASE_CIPHER_SUCCESS;
    }

    int CCipherBase::verify(const unsigned char *text, unsigned int textLen,
        const unsigned char *signValue, unsigned int signLen,
        EVP_PKEY *pKey)
    {
        ERR_load_EVP_strings();
        EVP_MD_CTX mdctx; //摘要算法上下文变量
        EVP_MD_CTX_init(&mdctx); //初始化摘要上下文

        if(!EVP_VerifyInit_ex(&mdctx, EVP_md5(), NULL)) //验证初始化，设置摘要算法，一定要和签名一致
        {
            EVP_PKEY_free(pKey);
            return BASE_CIPHER_SIGN_VERIFY_ERROR; /* Error */
        }

        if(!EVP_VerifyUpdate(&mdctx, text, textLen)) //验证签名（摘要）Update
        {
            EVP_PKEY_free(pKey);
            return BASE_CIPHER_SIGN_VERIFY_ERROR; /* Error */
        }

        if(!EVP_VerifyFinal(&mdctx, signValue, signLen, pKey))
        {
            EVP_PKEY_free(pKey);
            EVP_MD_CTX_cleanup(&mdctx);
            return BASE_CIPHER_SIGN_VERIFY_ERROR; /* Error */
        }

        //释放内存
        EVP_PKEY_free(pKey);
        EVP_MD_CTX_cleanup(&mdctx);
        return BASE_CIPHER_SUCCESS;
    }

    int CCipherBase::getPKEY(RSA* rsa, EVP_PKEY** lpPKEY)
    {
        if (NULL == rsa)
        {
            //密钥为空
            return BASE_CIPHER_SIGN_VERIFY_ERROR;
        }

        //将rsa对象赋给EVP_PKEY结构
        *lpPKEY = EVP_PKEY_new();
        if (NULL == *lpPKEY)
        {
            RSA_free(rsa);
            return BASE_CIPHER_LACK_OF_MEMORY;
        }

        // 被这个接口坑了好久，终于处理好了
        EVP_PKEY_set1_RSA(*lpPKEY, rsa);
        return BASE_CIPHER_SUCCESS;
    }

    RSA *CCipherBase::getPubKeyFromString( const char *pubKey )
    {
        BIO *pubBIO = BIO_new(BIO_s_mem());
        BIO_puts(pubBIO, pubKey);
        if (NULL == pubBIO)
        {
            return NULL;
        }

        RSA *rsa = PEM_read_bio_RSAPublicKey(pubBIO, NULL, NULL, NULL);

        if (NULL != rsa)
        {
            RSA_blinding_on(rsa,NULL);
        }
        return rsa;
    }

    RSA *CCipherBase::getPriKeyFromString( const char *priKey )
    {
        BIO *priBIO = BIO_new(BIO_s_mem());
        BIO_puts(priBIO, priKey);
        if (NULL == priBIO)
        {
            return NULL;
        }

        RSA *rsa = PEM_read_bio_RSAPrivateKey(priBIO, NULL, NULL, NULL);

        if (NULL != rsa)
        {
            RSA_blinding_on(rsa,NULL);
        }
        return rsa;
    }

    RSA *CCipherBase::getPubKeyFromFile( const char *pubKeyPath )
    {
        BIO *pubBIO = BIO_new(BIO_s_file());
        BIO_read_filename(pubBIO, pubKeyPath);
        if (NULL == pubBIO)
        {
            return NULL;
        }

        RSA *rsa = PEM_read_bio_RSAPublicKey(pubBIO, NULL, NULL, NULL);

        if (NULL != rsa)
        {
            RSA_blinding_on(rsa,NULL);
        }
        return rsa;
    }

    RSA *CCipherBase::getPriKeyFromFile( const char *priKeyPath )
    {
        BIO *priBIO = BIO_new(BIO_s_file());
        BIO_read_filename(priBIO, priKeyPath);
        if (NULL == priBIO)
        {
            return NULL;
        }

        RSA *rsa = PEM_read_bio_RSAPrivateKey(priBIO, NULL, NULL, NULL);

        if (NULL != rsa)
        {
            RSA_blinding_on(rsa,NULL);
        }
        return rsa;
    }

    Base64::Base64()
    {
    }

    Base64::~Base64()
    {
    }

    bool Base64::init()
    {
        if(!m_cryptModule.init(CCipherBase::s_base64))
            return false;
        return true;
    }

    int Base64::doBase64(const unsigned char *plaintext, int pn, unsigned char *ciphertext, int &cn)
    {
        return m_cryptModule.doBase64(plaintext, pn, ciphertext, cn);
    }

    int Base64::undoBase64(unsigned char *plaintext, int &pn, const unsigned char *ciphertext, int cn)
    {
        return m_cryptModule.undoBase64(plaintext, pn, ciphertext, cn);
    }

}
