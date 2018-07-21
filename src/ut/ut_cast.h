/*!
\file ../ut/ut_cast.h
\ingroup grp_ut_aux


                            Linux     MS-Win
                            gcc:      MS-cl:
32-bit: sizeof(int)    4       32        32
        sizeof(long)   4       32        32
        sizeof(void*)  4       32        32

64-bit: sizeof(int)    4       32        32
        sizeof(long)   4|8     64        32 | 64  (Compilerswitch) set to 64 !!!
        sizeof(void*)  8       64        64


see also ../ut/ut_types.h

*/

/// long from pointer                long = LONG_PTR (ptr);
#define LONG_PTR(vp1) (long)vp1

/// integer from pointer             int = INT_PTR (ptr);
#define INT_PTR(vp1) (int)((long)vp1)

/// pointer from integer             ptr = PTR_INT (int|long)
#ifdef _LP64
// 64-bit
#define PTR_INT(i1) (void*)((long)i1)

#else
// 32-bit
#define PTR_INT(i1) (void*)((int)i1)
#endif


/// pointer from long                ptr = PTR_LONG (long);
#define PTR_LONG(l1) (void*)l1

/// pointer from character-array     ptr = PTR_CHRA (*txt);
#define PTR_CHRA(txt) (void*)txt

/// text from pointer
#define TXT_PTR(vp1) (char*)(vp1)


// EOF
