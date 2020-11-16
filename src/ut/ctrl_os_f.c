/*  ../ut/ctrl_os_f.c                                  2013-01-11      RF
RemoteControl for Linux & Windows with files.

*/


#ifdef _MSC_VER
// #define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>                     // isdigit
#include <stdarg.h>                    // va_list

// #include <io.h>                    // va_list

#include <fcntl.h>                    // va_list
#include <sys/types.h>                    // va_list
#include <sys/stat.h>                    // va_list

#include "../ut/ut_os.h"                 // OS_get_bas_dir
#include "../ut/ctrl_os.h"               // OS_CTL_read__

#include "../ut/ut_txTab.h"              // TxtTab


//______________________________
// EXTERNALS:
// aus xa.c:
extern TxtTab    AP_TxTab1;




//______________________________
// LOCAL VARS:
static char      fnSwap[256];

#define SIZ_sBuf 1024
static char      sBuf[SIZ_sBuf];

static char *pnin = NULL, *pnout = NULL;       // pipenames





//================================================================
  int OS_CTL_exit () {
//================================================================

  return 0;

}


//================================================================
  int OS_CTL_read_init (char *fn) {
//================================================================
// init 

  char        fnin[256];


  printf("OS_CTL_read_init |%s|\n",fn);

  UtxTab_add (&AP_TxTab1, fn);
  UtxTab_query (&pnin, &AP_TxTab1);

  remove (pnin);    // delete inputfile

  // set name of workingfile
  sprintf(fnSwap, "%sCTRLfin",OS_get_tmp_dir());
    printf(" CTRL_CB__ fnSwap=|%s|\n",fnSwap);


  return 0;

}


//================================================================
  int OS_CTL_read_reset () {
//================================================================


  int    cNr;
  char   s1[32];


  printf("OS_CTL_read_reset \n");

  // if(pinLun == - 1) return -1;

  remove (pnin);

  return 0;

}


//================================================================
  char* OS_CTL_read__ (int *sSiz) {
//================================================================
// Output: 
//   sSiz     size of data
// RetCod: NULL = no input exists; else pointer to data.
//            dataspace may be modified.


static FILE   *fpi = NULL;

  int    i1;
  char   *p1, *p2;
  
  

  // test if file not yet finished ..
  if(fpi) goto L_read_nxt;



  // test if file exists
  L_start:

#ifdef _MSC_VER
// MS:
    i1 = GetFileAttributes (pnin); // -1=file not found; 16=file, 32=directory
    if(i1 < 0) return NULL;
#else
// Linux:
    i1 = access (pnin, R_OK);  // 0=file_exists
    if(i1) return NULL;
#endif

      // printf(" OS_CTL_read__ L1\n");

    // delete last workFile
    remove (fnSwap);

    // rename file
    rename (pnin, fnSwap);


    // work file
    fpi = fopen (fnSwap, "r");


  L_read_nxt:
    p1 = sBuf;

      // printf(" OS_CTL_read__ L_read_nxt\n");
    // fgets needs eof or '\n' to stop ..
    p2 = fgets (p1, SIZ_sBuf, fpi);
    if(!p2) goto L_eof;

    // remove '\n' 
    i1 = strlen(p1);
    if(p1[i1 - 1] == '\n') {
      --i1;
      p1[i1] = '\0';
    }
      // printf("%d |%s|\n",i1,p1);

    *sSiz = i1;

    return p1;


  L_eof:
      // printf(" OS_CTL_read__ L_eof\n");
    fclose (fpi);
    fpi = NULL;
    goto L_start;

}


//================================================================
  int OS_CTL_write_init (char *fn) {
//================================================================
// init 

  printf("OS_CTL_write_init |%s|\n",fn);


  UtxTab_add (&AP_TxTab1, fn);
  UtxTab_query (&pnout, &AP_TxTab1);


  remove (pnout);    // delete File (sonst get das rename ned ..)

  return 0;

}


//================================================================
  int OS_CTL_write__ (char *sBufOut, int sSiz) {
//================================================================
// send line to remote partner

  FILE *fpo;

  printf("OS_CTL_write__ |%s| %d\n",sBufOut, sSiz);

  // // append
  // fpo = fopen (pnout, "a");

  // always create new file
  fpo = fopen (pnout, "w");
  fwrite (sBufOut, sSiz, 1, fpo);
  fclose (fpo);

    // printf(" wr ..\n");

  return 0;

}


//====================== EOF ===========================
