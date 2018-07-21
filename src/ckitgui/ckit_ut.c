/*
 *
 * Copyright (C) 2015 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
TODO:

-----------------------------------------------------
Modifications:
2012-02-01 extracted.  RF.


-----------------------------------------------------
*//*!
\brief utility-functions
\file ../ut/ckit_ut.c
\brief utility-functions for Gui-Toolbox only
\code

=====================================================
List_functions_start:

UTX_CleanCR
UTX_endDelChar
UTX_cnr_chr
UTX_fgetLine           read line nr. <lNr> out of file <filNam>
UTX_wrf_str            write string -> file
UTX_tmp_file           get file into temp. memspc
UTX_str_file           read file -> string
UTX_ck_num_i           test if word is numeric (int)

UTI_round_i2b          round integer to byte (back: UTI_round_b2i)
UTI_round_b2i          make integer from byte (back from UTI_round_i2b)

CSV_read__             read/decode next line in csv-file

TX_Print
TX_Error
TX_Write

MSG_get_1              DUMMY
MSG_err_1              DUMMY
MSG_pri_1              DUMMY
MSG_get_str            DUMMY
gCad_fini              DUMMY
AP_tutStat_get         DUMMY

List_functions_end:
=====================================================


cc -c gui_gtk_ut.c


\endcode *//*----------------------------------------


_______________________________________________________________________________
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <stdarg.h>          // ...)
#include <sys/stat.h>        // stat


#include "../ckitgui/ckit_ut.h"        // UTX_tmp_file
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_umem.h"        // UME_alloc_tmp




#define ON                 0
#define OFF                1


/*  alle Ausgaben EIN */
int   TX_mode = ON;


/*
typedef struct {
  int  nrVal;      // nr of values per line
  char **pTab;     // table of <nrVal> pointers; strings; for out and in.
  char delVal;     // value-delimiter-char; comma or semicolon
  } CSV_struct;
*/




//======================================================================
  void UTX_str_file (char* txbuf, long *fSiz, char *fnam) {
//======================================================================
/// UTX_str_file           read file -> string
/// Used by UTX_tmp_file()
/// fSiz must have filesize; see OS_FilSiz.

  FILE  *fpi;


  if ((fpi = fopen (fnam, "r")) == NULL) {
    TX_Error ("UTX_str_file: Error open %s",fnam);
    *fSiz = -2;
    return;
  }

  fread (txbuf, *fSiz, 1, fpi);

  txbuf[*fSiz] = '\0';
  if(txbuf[*fSiz - 1] == 10) {    // LF
    *fSiz -= 1;
    txbuf[*fSiz] = '\0';
  }

  fclose(fpi);

  return;

}


//================================================================
  int UTX_ck_num_i (char *s1) {
//================================================================
/// \code
/// UTX_ck_num_i          test if word is numeric (int)
/// RetCod:  -1   s1 does NOT start with an integer
///          else nr of characters (including sign)
/// \endcode

  int     iNr=0;

  while (*s1  == ' ') { ++s1; ++iNr; }


  if((*s1 == '-')||(*s1 == '+')) {
    ++s1;
    ++iNr;
  }

  while(isdigit(*s1)) {
    ++s1;
    ++iNr;
  }

  if(iNr < 1) return -1;

  return iNr;

}


//==================================================================
  int CSV_read__ (CSV_struct *fTyp, char *sb, int sSiz, FILE *fpi) {
//==================================================================
// read/decode next line in csv-file
// first line: get nr of values, value-delimiter-char and column-titles.
// Input:
//   fTyp->delVal set to blank (' ') for first call.
//   fTyp->nrVal  the maximum nr of values per line (size of pTab !)
//   fTyp->pTab   no input, but must have size fTyp->nrVal !
//   sSiz         size of stringbuffer sb
//   fpi          FILE* of inputfile; must be open.
// Output:
//   fTyp->delVal the value-delimiter-char
//   fTyp->nrVal  the nr of values per line
//   fTyp->pTab   the values (pointers into sb)
//   sb           stringbuffer
// RetCodes:
//   0            OK
//   -1           EOF; pTab and sb invalid.
//   -2           cannot find value-delimiter-char in first line
//   -3           size of pTab too small


  int   ii;
  char  *p1, *p2, *p3;
  int   vMax;


  // printf("CSV_read__ |%c| %d\n",fTyp->delVal,fTyp->nrVal);


  // read next line
  if (fgets (sb, sSiz, fpi) == NULL) {
    return -1;
  }
  // remove following CR,LF
  L_clean:
  ii = strlen(sb);
  --ii;
  if((sb[ii] == '\n')||(sb[ii] == '\r')) {
    sb[ii] = '\0';
    goto L_clean;
  }



  vMax = fTyp->nrVal;
    // printf(" vMax=%d\n",vMax);


  // read first line of file
  if(fTyp->delVal != ' ') goto L_readl;


  // decode value-delimiter-char & nr-of-values
  // is 1. char '"' ? 
  if(sb[0] != '"') goto L_noString;


  //----------------------------------------------------------------
  // decode value-delimiter-char
  p1 = strchr(&sb[1], '"');
  if(!p1) return -2;
  ++p1;
  fTyp->delVal = *p1;


  //----------------------------------------------------------------
  // first title has not "; look for , or ;
  L_noString:
    p1 = strpbrk (sb, ",;");
    if(!p1) return -2;
    fTyp->delVal = *p1;
      printf(" found delVal=|%c|\n",fTyp->delVal);



  //----------------------------------------------------------------
  // decode normal line ..
  L_readl:
  p1 = sb;
  ii = 0;

  L_nxtVal:
    printf(" nxt |%s|\n",p1);
  // find next delimiter
  p2 = strchr(p1, fTyp->delVal);
  if(p2) {
    p3 = p2 + 1;
  } else {
    p2 = strchr(p1, '\0');
    p3 = '\0';
      // printf(" end:%d %d\n",p2-p1,strlen(p1));
  }

  if(ii >= vMax) return -3;


  // remove "-s
  if(*p1 == '"') {
    ++p1;
    --p2;
  }
  *p2 = '\0';
    printf(" ii=%d p1=|%s|\n",ii,p1);
  fTyp->pTab[ii] = p1;
  p1 = p3;
  ++ii;
  if(p1) goto L_nxtVal;


  L_done:
  fTyp->nrVal = ii;
    // printf(" done-%d\n",ii);


    printf("ex CSV_read__ |%c| %d\n",fTyp->delVal,fTyp->nrVal);
    for(ii=0; ii<fTyp->nrVal; ++ii) {
      printf(" %d |%s|\n",ii,fTyp->pTab[ii]);
    }


  return 0;

}


//================================================================
  int UTX_wrf_str (char *fnam, char *txt) {
//================================================================
/// \code
/// UTX_wrf_str            write string -> file
/// see UTF_wrf_line
/// \endcode

  FILE     *fp1;

  // printf("UTX_wrf_str |%s|\n",fnam);


  // write Line -> File
  if((fp1=fopen(fnam,"w")) == NULL) {
    TX_Print("UTX_wrf_str E001 |%s|",fnam);
    return -1;
  }
  // fwrite(txt, strlen(txt)+1, 1, fp1);
  fprintf(fp1, "%s",txt);                   // 2012-07-14 \n removed
  fclose(fp1);

  return 0;

}


//================================================================
  int MSG_Tip (char *key) {
//================================================================
/// \code
/// dummyfunction for MSG_Tip
/// MSG_Tip         add tooltip to last created GUI-widget
/// msg out of files  msg_de.txt  msg_en.txt ..
/// \endcode

  // cp1 = MSG_read (MSG_buf, MSG_bSiz, key);
  // if(cp1 == NULL) return -1;
    // // printf(" cp1=|%s|\n",cp1);
  // GUI_Tip (cp1);

  GUI_Tip (key);

  return 0;

}



//================================================================
  int UTI_round_i2b (int i1) {
//================================================================
/// \code
/// UTI_round_i2b             round integer to byte (back: UTI_round_b2i)
///   Purpose: save Integers in a single byte.
///     UTI_round_i2b(10000)  -> 14
///     UTI_round_b2i(14)     -> 16384
/// \endcode

//  ii = 10000;
//  printf(" ii=%d, i2b=%d; b2i=%d\n",ii,
//           UTI_round_i2b(ii),UTI_round_b2i(UTI_round_i2b(ii)));


  int   ii=0;

  while(i1) {i1 /= 2; ++ii;}

  return ii;

}


//================================================================
  int UTI_round_b2i (int i1) {
//================================================================
/// UTI_round_b2i         make integer from byte (back from UTI_round_i2b)

  int ii=1;

  while(i1) {--i1; ii *=2;}

  return ii;

}


//================================================================
  int UTX_cnr_chr (char *txt, char c1) {
//================================================================
/// returns nr of char c1 in string txt


  int   iNr;
  char  *p1;

  iNr = 0;

  p1 = txt;

  while (*p1) {
    if(*p1 == c1) ++iNr;
    ++p1;
  }

  return iNr;

}


//===========================================================
  char* UTX_CleanCR (char* string) {
//===========================================================
/// \code
/// UTX_CleanCR              Delete Blanks, CR's and LF's at end of string
/// returns positon of stringterminator \0
///  
/// see also UTX_del_follBl UTX_CleanSC
/// \endcode


  int  ilen;
  char *tpos;
    
  ilen = strlen (string);
    
  tpos = &string[ilen];
    
  if(ilen < 1) goto L_exit;

  --tpos;


  while ((*tpos  == ' ')  ||
         (*tpos  == '\t') ||          /* tab */
         (*tpos  == '\n') ||          /* newline */
         (*tpos  == '\r'))   {        /* CR */

    *tpos    = '\0';
    if(tpos <= string) goto L_exit;
    --tpos;
  }

  ++tpos;


  L_exit:
  // printf("ex UTX_CleanCR |%s|\n", string);

  return tpos;
}


//=============================================================
  int UTX_endDelChar (char* txtbuf, char chr) {
//=============================================================
/// UTX_endDelChar           if last char of string == chr: remove it.
                                                              
  
  if (txtbuf[strlen(txtbuf)-1] == chr) {
    txtbuf[strlen(txtbuf)-1] = '\0';
  }

  /* printf("UTX_endDelChar |%s| %c\n", txtbuf,chr); */

  return 1;
  
} 
  

//================================================================
// dummyfunctions:

int MSG_get_1 (char *msg, int msgSiz, char *key, char *fmt, ...) {
  printf("MSG_get_1: %s\n",msg);
  return 0;
}


int MSG_err_1 (char *key, char *fmt, ...) {
  char    TX_buf1[1024];
  va_list va;

  printf("MSG_err_1: %s\n",key);
  va_start(va,fmt);
  strcpy(TX_buf1, "*** MSG_err_1: ");
                // o123456789012345
  vsprintf(&TX_buf1[16],fmt,va);
  va_end(va);
  TX_Write (TX_buf1);

  return 0;
}


int MSG_pri_1 (char *key, char *fmt, ...) {
  printf("MSG_pri_1: %s\n",key);
  return 0;
}

char* MSG_get_str (char *key) {
  return key;
}


int gCad_fini () {
  return 0;
}


int AP_tutStat_get () {
  return 0;
}




///==========================================================
  void TX_Print (char* txt, ...) {
///==========================================================
///   formatiertes Rausschreiben; wie bei printf; mehr als 4 Parm's Problem?



  va_list va;
  char    TX_buf1[1024];


  if(TX_mode == OFF) {
    return;
  }


  va_start(va,txt);

  vsprintf(TX_buf1,txt,va);

  va_end(va);

  if(strlen(TX_buf1) > 100) {
    TX_buf1[100] ='\0';
  }

  TX_Write (TX_buf1);


}





///==========================================================
  void TX_Error (char* txt, ...) {
///==========================================================


  char   TX_buf1[1024];


  va_list va;
  // void    *p1,*p2,*p3,*p4,*p5,*p6,*p7,*p8,*p9,*p10,*p11;


  va_start(va,txt);

  // p1  = va_arg(va,void*);
  // p2  = va_arg(va,void*);
  // p3  = va_arg(va,void*);
  // p4  = va_arg(va,void*);
  // p5  = va_arg(va,void*);
  // p6  = va_arg(va,void*);
  // p7  = va_arg(va,void*);
  // p8  = va_arg(va,void*);
  // p9  = va_arg(va,void*);
  // p10 = va_arg(va,void*);
  // p11 = va_arg(va,void*);


  // sprintf(TX_buf1,txt,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11);
  strcpy(TX_buf1, "*** Fehler: ");
                // o123456789012
  vsprintf(&TX_buf1[12],txt,va);
  // UTX_CleanCR (TX_buf1);
  va_end(va);

  TX_Write (TX_buf1);

  /* os_beep (); */


}

///==========================================================
  void TX_Write (char* txt) {
///==========================================================
///  direkt rausschreiben.


  printf("%s\n",txt);

  if(TX_mode == OFF) {
    return;
  }



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


/*
//================================================================
  int UTX_tmp_file (char *fBuf, long *fSiz, char *fNam) {
//================================================================

  *fSiz = OS_FilSiz (fNam);\
  fBuf = UME_alloc_tmp (*fSiz);\
  UTX_str_file (fBuf, fSiz, fNam);\

  return 0;

}
*/

// EOF
