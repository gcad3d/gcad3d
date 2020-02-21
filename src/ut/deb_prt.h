/* ../ut/deb_prt.h



*/

// prereq: 
// #include <stdarg.h>              // for ...
// extern char DEB_prt_fp;



void DEB_prt_print (char* format, ...);
void DEB_prt_init (int mode);

// extern FILE *DEB_prt_fp;
extern int DEB_prt_stat;


//================================================================
// debug-print (see DEB_prt_init())

#ifdef DEB
#define printd if(DEB_prt_stat) DEB_prt_print
#else
#define printd if(0) printf
#endif



// eof
