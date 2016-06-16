//  franz.reiter@cadcam.co.at
//
// see also ../ut/ut_cast.h


#include <stdint.h>              // ISO-C99 uint32_t ..


#ifdef _MSC_VER
// Mircosoft - MS32
#define INT_8     INT8           // char 
#define INT_16    INT16          // signed short 
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


#define INT_8_MAX            255
#define INT_16_MAX         65535
#define INT_24_MAX      16777215
#define INT_32_MAX    4294967295

#define FLT_32_MAX 9999999999999.999


// EOF
