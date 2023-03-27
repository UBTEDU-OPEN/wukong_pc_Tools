#include "base32.h"
#include "type.h"
#include "stdbool.h"
#ifdef __MACH__
#include <stdint.h>
#endif
const char base32_alphabet[] = "0123456789abcdefghjkmnpqrstvwxyz";
const char base32_Alphabet[] = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";

#define NIL    -3
#define SP    -2

static unsigned char detab[128] = {
    NIL,-1, -1, -1, -1, -1, -1, -1, -1, SP, SP, SP, SP, SP, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    SP, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
    -1, 10, 11, 12, 13, 14, 15, 16, 17, -1, 18, 19, -1, 20, 21, -1,
    22, 23, 24, 25, 26, -1, 27, 28, 29, 30, 31, -1, -1, -1, -1, -1,
    -1, 10, 11, 12, 13, 14, 15, 16, 17, -1, 18, 19, -1, 20, 21, -1,
    22, 23, 24, 25, 26, -1, 27, 28, 29, 30, 31, -1, -1, -1, -1, -1,
};

static size_t _encode(const char *alphabet, char *out, const void *in, size_t len)
{
    const unsigned char *s = (const unsigned char *)in;
    char *d = out;
    int c0, c1, c2, c3, c4;

    for (; len >= 5; len -= 5)
    {
        c0 = s[0];
        c1 = s[1];
        c2 = s[2];
        c3 = s[3];
        c4 = s[4];
        d[0] = alphabet[c0 >> 3];
        d[1] = alphabet[(c0 << 2 | c1 >> 6) & 0x1F];
        d[2] = alphabet[(c1 >> 1) & 0x1F];
        d[3] = alphabet[(c1 << 4 | c2 >> 4) & 0x1F];
        d[4] = alphabet[(c2 << 1 | c3 >> 7) & 0x1F];
        d[5] = alphabet[(c3 >> 2) & 0x1F];
        d[6] = alphabet[(c3 << 3 | c4 >> 5) & 0x1F];
        d[7] = alphabet[c4 & 0x1F];
        s += 5;
        d += 8;
    }

    c1 = c2 = c3 = c4 = 0;
    switch (len)
    {
    case 4:
        c3 = s[3];
        d[6] = alphabet[(c3 << 3 | c4 >> 5) & 0x1F];
        d[5] = alphabet[(c3 >> 2) & 0x1F];
    case 3:
        c2 = s[2];
        d[4] = alphabet[(c2 << 1 | c3 >> 7) & 0x1F];
    case 2:
        c1 = s[1];
        d[3] = alphabet[(c1 >> 4 | c2 >> 4) & 0x1F];
        d[2] = alphabet[(c1 >> 1) & 0x1F];
    case 1:
        c0 = s[0];
        d[1] = alphabet[(c0 << 2 | c1 >> 6) & 0x1F];
        d[0] = alphabet[c0 >> 3];
    }

    d += BASE32_LEN(len);
    *d = 0;
    return d - out;
}

size_t base32_encode(char *out, const void *in, size_t len)
{
    return _encode(base32_alphabet, out, in, len);
}

size_t base32_Encode(char *out, const void *in, size_t len)
{
    return _encode(base32_Alphabet, out, in, len);
}

size_t base32_decode(void *out, const char *in, size_t len)
{
    const unsigned char *s = (const unsigned char *)in, *end = (const unsigned char *)-1;
    char *d = (char *)out;
    bool find_end = true;
    int c, x, r, r2, n;

    if (len >= 0)
    {
        end = s + len;
        find_end = false;
    }

    for (r2 = 0, r = 0, n = 0; s < end && (c = *s++) != 0; )
    {
        x = (c < 128) ? detab[c] : -1;
        if (x < 0)
        {
            if (x == NIL && find_end)
                break;

            return -(s - (const unsigned char *)in);
        }

        switch (n)
        {
        case 0:
            ++n;
            r = x << 3;
            break;
        case 1:
            ++n;
            *d++ = r + (x >> 2);
            r = x << 6;
            break;
        case 2:
            ++n;
            r2 = r;
            r = x << 1;
            break;
        case 3:
            ++n;
            *d++ = r2 + r + (x >> 4);
            r = x << 4;
            break;
        case 4:
            ++n;
            *d++ = r + (x >> 1);
            r = x << 7;
            break;
        case 5:
            ++n;
            r2 = r;
            r = x << 2;
            break;
        case 6:
            ++n;
            *d++ = r2 + r + (x >> 3);
            r = x << 5;
            break;
        case 7:
            n = 0;
            *d++ = r + x;
        }
    }

    if ((s < end && c != 0) 
        || (n == 1 || n == 3 || n == 6)
        || (n && (r & 0xff)))
    {
        return -(s - (const unsigned char *)in);
    }

    return d - (char *)out;
}

static char _luhn_char(const char *alphabet, const char *base32str, size_t len)
{
    const unsigned char *s = (const unsigned char *)base32str, *end = (const unsigned char *)-1;
    bool find_end = true;
    int c, x, n;
    int flip = 0;
    unsigned long sum[2] = {0,0};

    if (len >= 0)
    {
        end = s + len;
        find_end = false;
    }

    for (; s < end && (c = *s++) != 0; )
    {
        x = (c < 128) ? detab[c] : -1;
        if (x < 0)
        {
            if (x == NIL && find_end)
                break;

            return 0;
        }

        sum[flip] += (x*2/32) + (x*2%32);
        flip = !flip;
        sum[flip] += x;
    }

    flip = !flip;
    n = (sum[flip] % 32);
    return alphabet[n ? 32 - n : 0];
}

char base32_luhn_char(const char *base32str, size_t len)
{
    return _luhn_char(base32_alphabet, base32str, len);
}

char base32_luhn_Char(const char *base32str, size_t len)
{
    return _luhn_char(base32_Alphabet, base32str, len);
}

bool base32_luhn_check(const char *base32_with_luhn, size_t len)
{
    const unsigned char *s = (const unsigned char *)base32_with_luhn, *end = (const unsigned char *)-1;
    bool find_end = true;
    int c, x, r, r2, n;
    int flip = 0;
    unsigned long sum[2] = {0,0};

    if (len >= 0)
    {
        end = s + len;
        find_end = false;
    }

    if (len == 1)
        return false;

    for (r2 = 0, r = 0, n = 0; s < end && (c = *s++) != 0; )
    {
        x = (c < 128) ? detab[c] : -1;
        if (x < 0)
        {
            if (x == NIL && find_end)
                break;

            return false;
        }

        sum[flip] += (x*2/32) + (x*2%32);
        flip = !flip;
        sum[flip] += x;
    }

    return (sum[flip] % 32) == 0;
}

#ifdef TEST_BASE32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int len;
    char buf[256];
    char abc[] = 
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        ;
    int abc_len = strlen(abc);

    len = base32_encode(buf, abc, abc_len);
    //printf("%d:\n%s\n", len, buf);

    //printf("%c\n", base32_luhn_char(buf, len));
    
    buf[len++] = base32_luhn_char(buf, len);
    buf[len] = 0;

    //printf("%d\n", base32_luhn_check(buf, len));
    buf[--len] = 0;

    memset(abc, 0, abc_len);
    len = base32_decode(abc, buf, -1);
    //printf("%d:\n%s\n", len, abc);

    return 0;
}

#endif

