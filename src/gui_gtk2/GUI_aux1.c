/*
 *
 * Copyright (C) 2020 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
-----------------------------------------------------
Select file for open.
Returns selected file via stdout.
All testoutput to stdout must be precede with "##"
-----------------------------------------------------
TODO:

-----------------------------------------------------
Modifications:
2020-01-10 Created. RF.

=============================================================================
Using:  see INF_GUI_dlg1   (../inf/../inf/GUI.c)


=============================================================================
// ReBuild:
. ../options.sh && make -f GUI_dlg1.mak


// ReBuild and test:
./GUI_test_dlg1.sh


cat /tmp/debug.dat

*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>              // for ...

#include <gtk/gtk.h>

#include "../ut/ut_umem.h"          // Memspc
#include "../ut/ut_txTab.h"         // UtxTab_NEW
// #include "../ut/deb_prt.h"          // printd


// activate printd
// #define printd printf
// disable printd
#define printd if(0) printf


/// FilenamedelimiterChar
#if defined _MSC_VER || __MINGW64__
#define  fnam_del '\\'
#define  fnam_del_s "\\"
#else
#define  fnam_del '/'
#define  fnam_del_s "/"
#endif



//----------------------------------------------------------------
// prototypes
  void UTX_CleanCR (char* string);
  int OS_osVar_eval__ (char *fno, char *fni, int fnoSiz);
  int GUI_dlg1_cancel (void *parent, void *data);
  char** UTX_wTab_file (char *memspc, int memSiz, char *fnam);
  char** OS_wTab_file (char **memspc, char *fnam);
  char * OS_get_tmp_dir();

//----------------------------------------------------------------


//================================================================
  int GUI_get_version (char *SW, int *vMaj, int *vMin) {
//================================================================
/// \code
/// GUI_get_version     returns guiSW, SW-mayor-version, SW-minor-version
/// Output:
///   SW          "gtk"
///   vMaj        2 or 3
/// \endcode


  strcpy(SW, "gtk");
  *vMaj = GTK_MAJOR_VERSION;
  *vMin = GTK_MINOR_VERSION;

    // printf(" ex-GUI_get_version %s %d %d\n",SW,*vMaj,*vMin);


  return 0;

}


//================================================================
  char** OS_wTab_file (char **memspc, char *fnam) {
//================================================================
// OS_wTab_file         get file into table of words
// - must free memspc!

  long       fs;
  char       **wTab;
  // Memspc     strSpc;
  // UtxTab_NEW (txTab1);

  
  // get size of fnam
  fs = 512 + (OS_FilSiz (fnam) * 2);

  // get memspc
  *memspc = malloc (fs);


	// read file into memspc
  return UTX_wTab_file (*memspc, fs, fnam);

}


//================================================================
int DEB_dump_obj__ (int typ, void *data, char *txt, ...) { return 0; }
void TX_Error (char* txt, ...) { printf("%s\n",txt); }
void TX_Print (char* txt, ...) { printf("%s\n",txt); }
// see also ../ut/ut_TX.c


//================================================================
  int MSG_err_1 (char *key, char *fmt, ...) {
  printf("%s\n",key);
  return 0;
}


//================================================================
  int MSG_get_1 (char *msg, int msgSiz, char *key, char *fmt, ...) {
  printf("%s\n",key); 
  return 0;
}


//================================================================
// ../ut/ut_txt.c\
//================================================================
//
//=========================================================================
  char** UTX_wTab_file (char *memspc, int memSiz, char *fnam) {
//=========================================================================
// make table of words from file. End of List is NULL.
// Input:
//   memspc, memSiz      where to save file and table. No reallocate !

  int        i1, iNr;
  char       **wTab;
  Memspc     strSpc;
  UtxTab_NEW (txTab1);


  // printf("UTX_wTab_file siz=%d |%s|\n",memSiz,fnam);

  // read file, add each line to memspc.
  UME_init (&strSpc, memspc, memSiz);
  UtxTab_init_Memspc (&txTab1, &strSpc);

  // add file to stringTab
  i1 = UtxTab_add_file (&txTab1, fnam);
  if(i1 < 1) return NULL;
    // UtxTab_dump (&txTab1);

  // get next free address of txTab1
  wTab = (char**)UtxTab_pos_free(&txTab1);

  // need i1 bytes
  iNr = UtxTab_nr(&txTab1);
  i1 = sizeof(void*) * (iNr + 1);
    // printf(" i1=%d\n",i1);

  // check if enough free space
  if(i1 >= UtxTab_bytes_free(&txTab1)) {
    printf("***** Error UTX_wTab_file EOM \n");
    return NULL;
  }

  // loop tru wTab; add pointer to each line of file -> optLst.
  for(i1=0; i1<iNr; ++i1) wTab[i1] = UtxTab__(i1, &txTab1);
  wTab[iNr] = NULL;

    // TESTBLOCK
//     // UTX_wTab_dump (wTab);
//     UtxTab_dump (&txTab1, "ex-UTX_wTab_file");
//     printf(" ex-UTX_wTab_file iNr=%d\n",iNr);
    // END TESTBLOCK
   
  return wTab;

}


///===========================================================
  void UTX_CleanCR (char* string) {
///===========================================================
/// UTX_CleanCR                  Delete Blanks, CR's u. LF's am Ende.
///   Ersetzung von Blanks, Carriage Returns und Linefeeds durch
///   Nullzeichen von hinten nach vorne bis zum ersten von diesen
///   verschiedenen Zeichen.
  

  int  ilen;
  char *tpos; 
    
  ilen = strlen (string);
    
  if(ilen < 1) goto L_exit;
    
  tpos = &string[ilen];
  --tpos;
  
  
  while ((*tpos  == ' ')  ||
         (*tpos  == '\t') ||          /* tab */
         (*tpos  == '\n') ||          /* newline */
         (*tpos  == '\r'))   {        /* CR */

    *tpos    = '\0';
    --tpos;
    if(tpos < string) break;
  }

  L_exit:
  // printf("ex UTX_CleanCR |%s|\n", string);

  return;
}


//=======================================================================
  int UTX_fgetLine (char *cbuf, int sizBuf, char *filNam, int lNr) {
//=======================================================================
/// \code
/// UTX_fgetLine           read line nr. <lNr> out of file <filNam>
///             first line has lineNr 1
/// sizBuf      size of cbuf
/// rc -1   File does not exist
/// rc -2   Line does not exist
/// rc  0   OK
/// \endcode


  int    i1;
  FILE   *fpi;

  // printf("UTX_fgetLine |%s| %d\n",filNam,lNr);


  if ((fpi = fopen (filNam, "r")) == NULL) {
    return -1;
  }

  i1=0;
  while (!feof (fpi)) {
    ++i1;
    if (fgets (cbuf, sizBuf, fpi) == NULL) goto Fertig;
    // printf(" ..--in |%s|\n",cbuf);
    if(i1 != lNr) continue;
    fclose(fpi);
    UTX_CleanCR (cbuf);
      // printf("ex UTX_fgetLine %d |%s|\n",lNr,cbuf);
    return 0;
  }
  Fertig:
  fclose(fpi);
  return -2;
}



//================================================================
  char *UTX_find_strrchrn (char *cbuf, char *str) {
//================================================================
/// \code
/// UTX_find_strrchrn        find last occurence of one of the chars of str2
/// returns NULL or the position of the last char in cbuf also found in str.
///  (see strpbrk = find first)
/// NULL: nicht enthalten
/// \endcode

  int    ii;
  char   *p1, *p2;

  // printf("UTX_find_strrchrn |%s|%s| \n",cbuf,str);


  ii = 0;
  p1 = cbuf;

  while(str[ii]) {
    p2 = strrchr (p1, str[ii]);
    if(p2) {
      // found
      p1 = ++p2;  // start here
    }
    ++ii;
  }

  if(p1) --p1;

    // printf("ex-UTX_find_strrchrn |%s| \n",p1);

  return p1;

}


//================================================================
  int UTX_fnam1__ (char* par_dir, char* sNam, char* sIn) {
//================================================================
// UTX_fnam1__        separate/copy directory,fileName of full filename
// see  UTX_fnam__
// Output:
//   par_dir       directory     size must be 256; including closing '/'
//   sNam       filename[.typ]     size must be 128

  int    sdl, snl;
  char   *pfn;

  par_dir[0] ='\0';


  // printf("----------------------------------- \n");
  // printf("UTX_fnam1__ |%s|\n",sIn);

  // pfn = find last filename-delimiter
  // must check for '/' AND '\' (in MS '/' can come from out of source)
#if defined _MSC_VER || __MINGW64__
  pfn = UTX_find_strrchrn(sIn, "/\\");
#else
  pfn = strrchr(sIn, fnam_del);
#endif
    // printd(" fnam1__-pfn|%s|\n",pfn);


  // test if length of par_dir > 256
  if((pfn - sIn) >= 256) return -1;


  if(!pfn) {
    // no directory;
    pfn = sIn;

  } else {
    // pfn = pos. of last '/'
    sdl =  pfn - sIn + 1;
    strncpy(par_dir, sIn, sdl);
    par_dir[sdl] = '\0';
    ++pfn;  // skip deli
  }

  // copy the filname
  snl = strlen(pfn);

  // test if length of sNam > 128
  if(snl >= 128) return -2;
  strcpy(sNam, pfn);

    // printf("ex-UTX_fnam1__ |%s|%s|\n",par_dir,sNam);

  return 0;

}



//================================================================
// ../ut/ut_uti.c
//================================================================

//================================================================
  int UTI_round_i2b (int ii) {
//================================================================
/// \code
/// UTI_round_i2b             round integer to byte (back: UTI_round_b2i)
///  0,1 -> 0; 2 -> 1; 3,4 -> 2; 5-8 -> 3; 9-16 -> 4; 17-32 -> 5; ..
///   Purpose: save Integers in a single byte, rounding up.
///     UTI_round_i2b(20)    -> 5;     // all values from 17 to 32 give 5;
///     UTI_round_b2i(5)     -> 32
/// \endcode

//  ii = 10000;
//  printf(" ii=%d, i2b=%d; b2i=%d\n",ii,
//           UTI_round_i2b(ii),UTI_round_b2i(UTI_round_i2b(ii)));


  int   ib = -1;

  if(ii < 2) return 0;

  ii *= 2;
  --ii;

  while(ii) {ii /= 2; ++ib;}

  return ib;

}


//================================================================
  int UTI_round_b2i (int i1) {
//================================================================
/// \code
/// UTI_round_b2i         make integer from byte (back from UTI_round_i2b)
///   returns (2 to the power of i1);
/// 0 -> 0; 1 -> 2; 2 -> 4; 3 -> 8; 4 -> 16; 5 -> 32; .. 8=256; .. 10=1024 ..
/// \endcode

  int ii=1;

  if(i1) {
    while(i1) {--i1; ii *= 2;}
  } else return i1;

  return ii;

}


//================================================================
// ../gui_gtk2/gtk_base.c
//================================================================

//================================================================
  int GUI_update__ () {
//================================================================
/// update all windows

// Achtung: löscht events !


  // Display zwischendurch updaten
  while (gtk_events_pending()) {
    gtk_main_iteration();
  }

  return 0;

}


// eof
