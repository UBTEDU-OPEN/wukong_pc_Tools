#ifndef _BASE64_H_
#define _BASE64_H_
#include "utilsdef.h"
#include <string>
namespace utils {

#define BASE64_LEN(n)    (((n) * 4 + 2) / 3)

    typedef struct base64_t base64_t;

    struct base64_t {
        unsigned char alphabet[66];
        unsigned char detab[256];
    };

UTILS_EXPORTS void base64Encode(const std::string& input, std::string& output);
UTILS_EXPORTS void base64Decode(const std::string& input, std::string& output);

}
#endif

