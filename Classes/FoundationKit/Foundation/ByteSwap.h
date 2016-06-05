#ifndef LOSEMYMIND_BYTESWAP_H
#define LOSEMYMIND_BYTESWAP_H


#pragma once
#include "FoundationKit/Base/Types.h"
NS_FK_BEGIN

//=============== Endian ====================
//Other impl see:https://sourceforge.net/p/predef/wiki/Endianness/
//if (ENDIANNESS_ORDER == ENDIANNESS_LITTLE_ENDIAN)
//    std::cout << "Is little system" << std::endl;
//else if (ENDIANNESS_ORDER == ENDIANNESS_BIG_ENDIAN)
//    std::cout << "Is big system" << std::endl;
//else if (ENDIANNESS_ORDER == ENDIANNESS_PDP_ENDIAN)
//    std::cout << "Is pop system" << std::endl;
//else
//    std::cout << "Is unknow system" << std::endl;

const int ENDIANNESS_ORDER = 0xFFFFFFFF & 1;
const int ENDIANNESS_LITTLE_ENDIAN = 0x00000001;
const int ENDIANNESS_BIG_ENDIAN = 0x01000000;
const int ENDIANNESS_PDP_ENDIAN = 0x00010000;
const int ENDIANNESS_UNKNOWN_ENDIAN = 0xFFFFFFFF;


static char *ENDIAN_SWAP128(char *val)
{
    static char swp[16];
    swp[0] = val[15];
    swp[1] = val[14];
    swp[2] = val[13];
    swp[3] = val[12];
    swp[4] = val[11];
    swp[5] = val[10];
    swp[6] = val[9];
    swp[7] = val[8];
    swp[8] = val[7];
    swp[9] = val[6];
    swp[10] = val[5];
    swp[11] = val[4];
    swp[12] = val[3];
    swp[13] = val[2];
    swp[14] = val[1];
    swp[15] = val[0];
    return swp;
}

/// when define returns true it means that our architecture uses big endian
#define HOST_IS_BIG_ENDIAN (bool)(*(unsigned short *)"\0\xff" < 0x100) 
#define ENDIAN_SWAP16(i)  ((i & 0x00ff) << 8 | (i &0xff00) >> 8)  
#define ENDIAN_SWAP32(i)  ((i & 0x000000ff) << 24 | (i & 0x0000ff00) << 8 | (i & 0x00ff0000) >> 8 | (i & 0xff000000) >> 24)
#define ENDIAN_SWAP64(val)((((val) >> 56) & 0x00000000000000FF) | (((val) >> 40) & 0x000000000000FF00) |(((val) >> 24) & 0x0000000000FF0000) | (((val) >> 8) & 0x00000000FF000000) |(((val) << 8) & 0x000000FF00000000) | (((val) << 24) & 0x0000FF0000000000) |(((val) << 40) & 0x00FF000000000000) | (((val) << 56) & 0xFF00000000000000))
#define SWAP_INT16_LITTLE_TO_HOST(i) ((HOST_IS_BIG_ENDIAN == true)? ENDIAN_SWAP16(i) : (i) )
#define SWAP_INT32_LITTLE_TO_HOST(i) ((HOST_IS_BIG_ENDIAN == true)? ENDIAN_SWAP32(i) : (i) )
#define SWAP_INT64_LITTLE_TO_HOST(i) ((HOST_IS_BIG_ENDIAN == true)? ENDIAN_SWAP64(i) : (i) )
#define SWAP_INT16_BIG_TO_HOST(i)    ((HOST_IS_BIG_ENDIAN == true)? (i):  ENDIAN_SWAP16(i) )
#define SWAP_INT32_BIG_TO_HOST(i)    ((HOST_IS_BIG_ENDIAN == true)? (i):  ENDIAN_SWAP32(i) )
#define SWAP_INT64_BIG_TO_HOST(i)    ((HOST_IS_BIG_ENDIAN == true)? (i):  ENDIAN_SWAP64(i) )

//=============== Endian End =================

NS_FK_END

#endif // LOSEMYMIND_BYTESWAP_H


