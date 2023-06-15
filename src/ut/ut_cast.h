/*!
\file ../ut/ut_cast.h

                            Linux     MS-Win        MSYS2
                      byte   bit-gcc   bit-MS-cl     bit-gcc
32-bit: sizeof(int)    4       32        32          32
        sizeof(long)   4       32        32          32
        sizeof(float)  4       32        32          32
        sizeof(double) 8       64        64          64
        sizeof(void*)  4       32        32          32

64-bit: sizeof(int)    4       32        32          32
        sizeof(long)   4|8     64        32          32                <<<<<<<<<<
     sizeof(longlong)  8       64        64          64
        sizeof(float)  4       32        32          32
        sizeof(double) 8       64        64          64
        sizeof(void*)  8       64        64          64

MSVC: compilerswitch can set size of long to 64-bit; default is 32.




see also ../ut/ut_types.h
see also /ucrt64/include/basetsd.h

*/

// long from pointer                long = LONG__PTR (ptr);
// MINGW: LONG__PTR in /ucrt64/include/basetsd.h
long LONG__PTR (void *);
 #define LONG__PTR(vp1) (long)((long long)vp1)
// #define LONG__PTR(vp1) (intptr_t)vp1

// integer from pointer             int = INT__PTR (ptr);
// MINGW: INT__PTR in /ucrt64/include/basetsd.h
int INT__PTR (void *);
#define INT__PTR(vp1) (int)((long long)vp1)
// #define INT__PTR(vp1) (intptr_t)vp1

// text from pointer
char* TXT_PTR (void *);
#define TXT_PTR(vp1) (char*)(vp1)


// pointer from long                ptr = PTR_LONG (long);
void* PTR_LONG (long);
#define PTR_LONG(l1) (void*)(long long)l1
// #define PTR_LONG(l1) (void*)(long)l1

// pointer from integer             ptr = PTR_INT (int|long)
// #ifdef _LP64
// 64-bit
void* PTR_INT (int);
#define PTR_INT(i1) (void*)((long long)i1)
// #define PTR_INT(i1) (void*)((long)i1)

// #else
// // 32-bit
// #define PTR_INT(i1) (void*)((int)i1)
// #endif


// pointer from character-array     ptr = PTR_CHRA (*txt);
void* PTR_CHRA (char*);
#define PTR_CHRA(txt) (void*)txt


// EOF
