/*!
\file ../ut/ut_cast.h
\ingroup grp_ut_aux


32-bit: sizeof(int)  = 32
        sizeof(long) = 32
        sizeof(void*)= 32

64-bit: sizeof(int)  = 32
        sizeof(long) = 64
        sizeof(void*)= 64

see also ../ut/ut_types.h

*/

/// long from pointer                long = LONG_PTR (ptr);
#define LONG_PTR(vp1) (long)vp1

/// integer from pointer             int = INT_PTR (ptr);
#define INT_PTR(vp1) (int)((long)vp1)

/// pointer from integer             ptr = PTR_INT (int);
#ifdef _LP64
// #define INT_PTR(vp1) (int)((long)vp1)
// #define LONG_PTR(vp1) (long)vp1
#define PTR_INT(i1) (void*)((long)i1)
#else
// #define INT_PTR(vp1) (int)((int)vp1)
// #define LONG_PTR(vp1) (long)vp1
#define PTR_INT(i1) (void*)((int)i1)
#endif


/// pointer from long                ptr = PTR_LONG (long);
#define PTR_LONG(l1) (void*)l1

/// pointer from character-array     ptr = PTR_CHRA (*txt);
#define PTR_CHRA(txt) (void*)txt

/// text from pointer
#define TXT_PTR(vp1) (char*)(vp1)


// EOF
