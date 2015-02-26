// windows-testprog.
// nmake -f tst_c1.nmak "OS=MS32"
// ..\binMS32\tst_c1.exe




#ifdef _MSC_VER
// #define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <winspool.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>                     // isdigit
#include <stdarg.h>                    // va_list

#include <io.h>                    // va_list
#include <fcntl.h>                    // va_list
#include <sys/types.h>                    // va_list
#include <sys/stat.h>                    // va_list


  char     fwn[256];

#define SIZ_sBuf 1024
char     sBuf[SIZ_sBuf];


  char* OS_CTL_read__ (int *sSiz, char *fn);



//============================
  int main (int paranz, char *params[]) {
//============================

  // MemObj   m1;
  char     s1[256], c1, *p1, **pp1;
  char     fn[256];
  int      i1, irc;
  long     l1, fSiz;
  double   d1, d2;
  char     *ta[]={"Jaenner","Februar","Maerz",NULL};  // Pointerarray
  FILE     *fpi;

  SECURITY_ATTRIBUTES lsa;


  printf("==== t0-main 2012-07-25 ====\n");



  // Read output from the child process, and write to parent's STDOUT.
  strcpy(fn, "pip1");


  OS_CTL_read_init (fn);


  for(;;) {

    // get next line
    p1 = OS_CTL_read__ (&i1, fn);
    if(!p1) {
      Sleep (500);
      continue;
    }
    
  }


  return 0;

}


//================================================================
  int OS_CTL_read_init (char *fn) {
//================================================================
// init 

  remove (fn);    // delete inputfile

  // set name of workingfile
  strcpy(fwn, "wpip1");

  // rename (fnOld, fnNew);

  return 0;

}



//================================================================
  char* OS_CTL_read__ (int *sSiz, char *fn) {
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
    i1 = GetFileAttributes (fn); // -1=file not found; 16=file, 32=directory
    if(i1 < 0) return NULL;

    p1 = sBuf;

    // delete last workFile
    remove (fwn);

    // rename file
    rename (fn, fwn);


    // work file
    fpi = fopen (fwn, "r");


  L_read_nxt:
    // fgets needs eof or '\n' to stop ..
    p2 = fgets (p1, SIZ_sBuf, fpi);
    if(!p2) goto L_eof;

    // remove '\n' 
    i1 = strlen(p1);
    if(p1[i1 - 1] == '\n') {
      --i1;
      p1[i1] = '\0';
    }
      printf("%d |%s|\n",i1,p1);

    *sSiz = i1;

    return p1;


  L_eof:
    fclose (fpi);
    fpi = NULL;
    goto L_start;

}



//====================== EOF ===========================
