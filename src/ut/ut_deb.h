// ../ut/ut_deb.h


// set DEB_STAT = 0 for supress debug-output;
// set DEB_STAT = 1 for activate debug-output;
extern int       DEB_STAT;


//----------------------------------------------------------------
// MS-VS-2010:
#ifndef __func__ 
#define __func__ __FUNCTION__
#endif

#define DEB_exit() {\
 printf("***** DEB_exit in %s lNr %d *****\n",__func__,__LINE__);\
 exit(-1);\
}

// conditional stop; use eg with: if(x==y) DEB_stop ();
// gdb-initfile must contain: "break DEB_halt"
#define DEB_stop() {\
 printf("***** DEB_stop in %s lNr %d *****\n",__func__,__LINE__);\
 DEB_halt();\
}


// eof
