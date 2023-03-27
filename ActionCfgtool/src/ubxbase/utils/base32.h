#ifndef BASE32_H_
#define BASE32_H_ 1

#include <stdlib.h>
#include "stdbool.h"
#include "utilsdef.h"

#ifdef __cplusplus
extern "C" {
#endif
#define BASE32_LEN(n)    (((n) * 8 + 4) / 5)
/*
 * Not used alphabetic characters:
 *    ILOU
 */

/*  0123456789abcdefghjkmnpqrstvwxyz  */
extern const char base32_alphabet[];

/*  0123456789ABCDEFGHJKMNPQRSTVWXYZ  */
extern const char base32_Alphabet[];
/* Return the number of characters placed in out. 
 */
UTILS_EXPORTS size_t base32_encode(char *out, const void *in, size_t len);
UTILS_EXPORTS size_t base32_Encode(char *out, const void *in, size_t len);

 
/* Return the number of characters placed in out. 
 * On error, return a negative number, the absolute value of the number
 * equals to the consumed size of the input string.
 * If ~len~ is negative, length of ~in~ is taken as strlen(~in~);
 */
UTILS_EXPORTS size_t base32_decode(void *out, const char *in, size_t len);



UTILS_EXPORTS char base32_luhn_char(const char *base32str, size_t len);
UTILS_EXPORTS char base32_luhn_Char(const char *base32str, size_t len);


//bool base32_luhn_check(const char *base32_with_luhn, size_t len);



#ifdef __cplusplus
}
#endif

#endif

