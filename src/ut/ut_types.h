//  franz.reiter@cadcam.co.at
//
// see also ../ut/ut_cast.h
// # if __WORDSIZE == 64


#ifndef _MSC_VER
#include <stdint.h>              // ISO-C99 uint32_t - not yet VS-2008
#endif


#ifdef _MSC_VER
// Mircosoft - MS32
#define INT_8     INT8           // char 
#define INT_16    signed short   // signed short INT16
#define INT_32    INT32          // int  INT BOOL LONG32
#define INT_64    INT64          // long long
#define UINT_8    unsigned char  // unsigned char  BYTE UINT8
#define UINT_16   unsigned short //UINT16         // unsigned short WORD
// #define UINT_32   uint32_t       // unsigned int DWORD UINT ULONG32
// #define UINT_64   uint64_t       // unsigned long long
#define UINT_32   unsigned int      // DWORD UINT ULONG32
#define UINT_64   unsigned long long
#else
// GCC - Linux 32,64
#define INT_8     char           // char  __s8
#define INT_16    short          // signed short __s16
#define INT_32    int32_t        // int
#define INT_64    int64_t        // __s64 long long
#define UINT_8    unsigned char  // unsigned char __u8
#define UINT_16   unsigned short // __u16  uint16_t unsigned short
#define UINT_32   uint32_t       // __u32  unsigned int
#define UINT_64   uint64_t       // __u64  unsigned long long
#endif


#define INT_8_MAX            127    // max val signed char
#define INT_16_MAX         32767    // max val signed short
#define INT_24_MAX       8388608    // 8 MB ?
#define INT_32_MAX    4294967295    // 4 GB ?

#define UINT_8_MAX           255
#define UINT_16_MAX        65535
#define UINT_24_MAX     16777216

#define FLT_32_MAX      9999999999999.999
//                      1234567890123
#define FLT_32_MIN1     0.000001

#define FLT_64_MAX      99999999999999999999999999999999999999
#define FLT_64_MIN1     0.0000000000000001


//----------------------------------------------------------------
#define MEMTYP_NONE         0  // no memspace; undefined/empty
  // realloc - YES,   malloc - NO,   free - NO

#define MEMTYP_ALLOC__      1  // malloc;
  // realloc - YES,   malloc - NO,   free - YES;

#define MEMTYP_ALLOC_PROT   5  // malloc, but do not free
  // realloc - YES,   malloc - NO,   free - NO

#define MEMTYP_ALLOC_EXPND  6  // static, malloced memspc, expandable
  // realloc - NO,    malloc - YES,  free - NO (yes after malloc)
  // if too small: malloc, copy, do not free source, set MEMTYP_ALLOC__

#define MEMTYP_STACK__     12  // stack; temp.memspace; no expand.
  // realloc - NO,    malloc - NO,   free - NO.

#define MEMTYP_STACK_EXPND 14  // stack, temp.memspace, expandable
  // realloc - NO,    malloc - YES,  free - NO (yes after malloc)
  // if too small: malloc, copy, do not free source, set MEMTYP_ALLOC__


// bits for MEMTYP
//   1   do realloc if too small
//   2   do malloc if too small, copy, do not free source.
//   4   do NOT free this memspace


// EOF
