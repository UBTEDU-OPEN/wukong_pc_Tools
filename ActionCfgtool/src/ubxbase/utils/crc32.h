#ifndef CRC32_H
#define CRC32_H

#include "utilsdef.h"
#define CRC32_INIT_VALUE 0xFFFFFFFF

UTILS_EXPORTS unsigned int crc32_standard(unsigned char * buffer, unsigned int size);
UTILS_EXPORTS unsigned int crc32_custom(unsigned int crc, unsigned char *buffer, unsigned int size);

#endif
