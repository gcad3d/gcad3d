//  franz.reiter@cadcam.co.at
//
// see also ../ut/ut_cast.h
// # if __WORDSIZE == 64


#include <stdint.h>              // ISO-C99 uint32_t ..


#ifdef _MSC_VER
// Mircosoft - MS32
#define INT_8     INT8           // char 
#define INT_16    signed short   // signed short INT16
#define INT_32    INT32          // int  INT BOOL LONG32
#define INT_64    INT64          // long long
#define UINT_8    unsigned char  // unsigned char  BYTE UINT8
#define UINT_16   UINT16         // unsigned short WORD
#define UINT_32   uint32_t       // unsigned int DWORD UINT ULONG32
#define UINT_64   uint64_t       // unsigned long long
#else
// GCC - Linux 32,64
#define INT_8     char           // char  __s8
#define INT_16    short          // signed short __s16
#define INT_32    int32_t        // int
#define INT_64    int64_t        // __s64 long long
#define UINT_8    unsigned char  // unsigned char __u8
#define UINT_16   uint16_t       // __u16  unsigned short
#define UINT_32   uint32_t       // __u32  unsigned int
#define UINT_64   uint64_t       // __u64  unsigned long long
#endif


#define INT_8_MAX            127    // max val signed char
#define INT_16_MAX         32767    // max val signed short
#define INT_24_MAX       8388608    // ?
#define INT_32_MAX    4294967295    // ?

#define UINT_8_MAX           255
#define UINT_16_MAX        65535
#define UINT_24_MAX     16777216

#define FLT_32_MAX      9999999999999.999
//                      1234567890123
#define FLT_32_MIN1     0.000001

#define FLT_64_MAX      99999999999999999999999999999999999999
#define FLT_64_MIN1     0.0000000000000001


//----------------------------------------------------------------
#define MEMSPCTYP_NONE         0    // no memspace
#define MEMSPCTYP_MALLOC__     1    // malloc-type=can-reallocate,must-free
#define MEMSPCTYP_MALLOC_FIX   2    // malloc-type=can-reallocate,must-NOT-free;
#define MEMSPCTYP_FIX__        4    // fixed-CANNOT-reallocate;must-NOT-free
#define MEMSPCTYP_STACK        5    // stack,CANNOT-reallocate,must-NOT-free

// EOF
