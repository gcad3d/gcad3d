// windows-testprog.
// nmake -f tst_c0.nmak "OS=MS32"
// ..\binMS32\tst_c0.exe



#ifdef _MSC_VER
// #define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <Psapi.h>
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



HANDLE   hw1;

OVERLAPPED ow1;


//============================
  int main (int paranz, char *params[]) {
//============================

  // MemObj   m1;
  char     s1[256], c1, *p1, **pp1;
  char     fNam[256];
  int      i1, irc;
  long     l1, fSiz;
  double   d1, d2;
  char     *ta[]={"Jaenner","Februar","Maerz",NULL};  // Pointerarray
  char     cBuf[256];
  HANDLE   hc;

  // SECURITY_ATTRIBUTES lsa;


  hc = GetCurrentProcess();
  // i1 = GetFinalPathNameByHandle (hc, s1, 256, VOLUME_NAME_NT); //FILE_NAME_NORMALIZED);
  // GetProcessImageFileName (hc, s1, 256);
  // printf(" |%s|\n",s1);

  GetModuleFileName (0, s1, 256);
  printf(" |%s|\n",s1);


  // GetFullPathName (s1, fNam



  strcpy(cBuf, "pip1");


  OS_CTL_write_init (cBuf);

  for (;;)    {
    Sleep (2000);
    OS_CTL_write__ (cBuf);
  }

  return 0;

}


//================================================================
  int OS_CTL_write_init (char *fn) {
//================================================================
// init 

  remove (fn);    // delete File (sonst get das rename ned ..)

  // rename (fnOld, fnNew);

  return 0;

}


//================================================================
  int OS_CTL_write__ (char *fn) {
//================================================================


  FILE *fpo;

  fpo = fopen (fn, "a");
  fwrite ("abc\n", 4, 1, fpo);
  fwrite ("def\n", 4, 1, fpo);
  fclose (fpo);

    printf(" wr ..\n");

  return 0;

}


//====================== EOF ===========================
