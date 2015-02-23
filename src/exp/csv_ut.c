//
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
-) tool for changing the decimal-delimiter-char "," into "." for csv-input-files

-----------------------------------------------------
Modifications:
2012-07-20 New. RF.

-----------------------------------------------------
*/
#ifdef globTag
void CSV(){}
#endif
/*!
\file  ../exp/csv_ut.c
\brief write, read csv-files (comm-separated-values, "flat-files")
\code
=====================================================
List_functions_start:

CSV_read__           read/decode next line in csv-file
CSV_write__          write line into csv-file

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


CSV-fileformat:
First line: titleheaders; all following line values.
value-delimiter-char for writing: CSV_delVal (comma)
  Reading can handle commas(,) and semicolons(;).
  Decimal-delimietr-character is ".";



\endcode *//*----------------------------------------


cc ../exp/csv_ut.c -DOFFLINE&&./a.out



======================================================================
*/



// #include "../xa/MS_Def0.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>                    // va_list va_arg


#include "../exp/csv_ut.h"     // struct CSV_struct



int TX_Error(char* txt, ...);




//################################################################
//################################################################
#ifdef OFFLINE
// cc ../exp/csv_ut.c -DOFFLINE&&./a.out
// cc csv_ut.c ut_TX.o ut_txt.o ut_mem.o ut_os_aix.o -DOFFLINE&&./a.out


  int main () {

  int            irc, i1;
  CSV_struct     fTyp;
  char           fNam[128];
  char           *tiTab[] = {"Title 1","Title 2","Info"};
  char           *pTab[3], s1[512], s2[80], s3[80];
  FILE           *fp1;


  printf("csv_ut test\n");

  strcpy(fNam, "../gui/list3.csv");

  fTyp.delVal = ',';
  fTyp.nrVal  = 3;
  fTyp.pTab   = tiTab;
  

  if((fp1=fopen(fNam,"w")) == NULL) {
    TX_Error("error open file %s",fNam);
    return -1;
  }

  // write title-line
  CSV_write__ (fp1, &fTyp);

  // write value-lines
  fTyp.pTab   = pTab;
  pTab[0] = s1;
  pTab[1] = s2;
  pTab[2] = s3;

  strcpy(s1, "col. 1");
  strcpy(s2, "col. 2");
  strcpy(s3, "col. 3");

  // CSV_write_v
  CSV_write__ (fp1, &fTyp);
  CSV_write__ (fp1, &fTyp);

  fclose(fp1);



  //----------------------------------------------------------------
  // read csv-file

  if((fp1=fopen(fNam,"r")) == NULL) {
    TX_Error("error open file %s",fNam);
    return -1;
  }

  fTyp.delVal = ' ';   // init for first call
  fTyp.pTab   = pTab;
  fTyp.nrVal  = 3;     // size of pTab

  // get nr of values, value-delimiter-char.
  L_nxt:
    irc = CSV_read__ (&fTyp, s1, 512, fp1);
    if(!irc) {
      printf(" \n");
      goto L_nxt;
    }


  fclose(fp1);


  return 0;

}

int TX_Error(char* txt, ...) {
    va_list va;
    va_start(va,txt);
    printf("*** Fehler: ");
    vprintf(txt, va);
    printf("\n");
  return 0;

}
#endif
//################################################################
//################################################################


//================================================================
  int CSV_write__ (FILE *fpo, CSV_struct *fTyp) {
//================================================================
// write line into csv-file.

  int  i1, sl;
  char s1[1024];


  // printf("CSV_write__ |%c| %d\n",fTyp->delVal,fTyp->nrVal);


  sprintf(s1, "\"%s\"",fTyp->pTab[0]);
  sl = strlen(s1);

  for(i1=1; i1<fTyp->nrVal; ++i1) {
    if(sl >= 1000) {                         // max size of line ..
      TX_Error("CSV_write__ E002 - buffersize");
      return -1;
    }

    sprintf(&s1[sl], "%c\"%s\"",fTyp->delVal,fTyp->pTab[i1]);
    sl += strlen(&s1[sl]);
      // printf(" sl=%d  |%s|\n",sl,s1);

  }

  fprintf(fpo,"%s\n",s1);

    printf("CSV_write__ |%s|\n",s1);

  return 0;

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
      // printf(" found delVal=|%c|\n",fTyp->delVal);



  //----------------------------------------------------------------
  // decode normal line ..
  L_readl:
  p1 = sb;
  ii = 0;

  L_nxtVal:
    // printf(" nxt |%s|\n",p1);
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
    // printf(" p1=|%s|\n",p1);
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

// EOF
