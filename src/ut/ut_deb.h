// ../ut/ut_deb.h
// defined in ../ut/ut_dump.c
// see INF_debug


//----------------------------------------------------------------
// MS-VS-2010:
#ifndef __func__ 
#define __func__ __FUNCTION__
#endif


// DEB_goto_L_exit         goto Label L_exit - display exit-functionName   INLINE
#define DEB_goto_L_exit() {\
 printf("\n***** DEB_goto_L_exit in %s lNr %d *****\n\n",__func__,__LINE__);\
 goto L_exit;\
}

// DEB_exit                exit program - display exit-functionName             INLINE
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


// set DEB_STAT = 0 for supress test-output in functions with: if(!DEB_STAT) return 0;
// set DEB_STAT = 1 for continue - print test-data
extern int       DEB_STAT;


// eof
