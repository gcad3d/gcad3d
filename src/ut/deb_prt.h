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


// activate printd  (display all printd-output)
//#define printd printf

// disable printd  (do not display printd-output)
#define printd if(0) printf


// #ifdef DEB
// // following line prints into file <tmp>/debug.dat
// #define printd if(DEB_prt_stat) DEB_prt_print
// // following line prints to console (must have leading "##"
// // #define printd printf
// #else
// #define printd if(0) printf
// #endif



// eof
