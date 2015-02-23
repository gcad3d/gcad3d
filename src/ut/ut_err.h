// see ERR_RESET

#include <signal.h>
#include <setjmp.h>

// #ifdef _MSC_VER
// #define ERR_SET1() 0
// #else
#define ERR_SET1() setjmp(err_buf); ERR_init ()
// #endif

extern jmp_buf       err_buf;


// EOF
