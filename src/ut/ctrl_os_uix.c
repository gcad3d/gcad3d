// ../ut/ctrl_os_aix.c                   2013-01-11              RF
/*
 *
 * Copyright (C) 2015 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
  ..

-----------------------------------------------------
Modifications:
  ..

-----------------------------------------------------
*/
/*!
\file  ../ut/ctrl_os_aix.c
\brief remote control - opsys-specific Unix
\code
=====================================================
List_functions_start:

OS_CTL_write_init
OS_CTL_write__          send line to remote
OS_CTL_read_init
OS_CTL_read_reset
OS_CTL_read__           get next line (terminated with '\n') from inputpipe
OS_CTL_exit

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>                      // write read

#include "../ut/ut_os.h"                 // AP_get_bas_dir
#include "../ut/ctrl_os.h"               // OS_CTL_read__
#include "../ut/ut_txTab.h"              // TxtTab
#include "../ut/ut_memTab.h"           // MemTab

#include "../xa/xa_mem.h"                // IE_buf mem_cbuf1_SIZ memspc201



//______________________________
// EXTERNALS:
// aus xa.c:
extern TxtTab    AP_TxTab1;




//______________________________
// LOCAL VARS:

static char *pnin = NULL, *pnout = NULL;  // pipenames

static int   pinLun = -1;                // input-pipe; always open
static int   poutLun = -1;               // output-pipe; always open

static FILE  *finLun = NULL;             // swapfile for large input



//================================================================
  int OS_CTL_exit () {
//================================================================
 
  // close CTRLfin
  if(finLun) fclose (finLun);

  // close pnin 
  if(pinLun != -1) close (pinLun);

  // close pnout
  if(poutLun != -1) close (poutLun);

  return 0;

}


//================================================================
  int OS_CTL_write_init (char *fn) {
//================================================================

  int         irc;
  struct stat att1;


  UtxTab_add (&AP_TxTab1, fn);
  UtxTab_query (&pnout, &AP_TxTab1);

#ifdef __MINGW64__
  remove (pnout);    // delete File (sonst get das rename ned ..)

#else
  // Linux
  // delete pipe if it exists
  remove (fn);     // unix can have pipes with old content.

  // create output-pipe
  if(stat(pnout, &att1) == -1) {
    // create named-pipe. See also mknod.
    // O_RDWR=read and write
    irc = mkfifo (pnout, O_RDWR|S_IWUSR|S_IWGRP|S_IRUSR|S_IRGRP);
    if(irc == -1) return -2;
  }


  // open output-pipe (keep it open permanently)
  poutLun = open (pnout, O_RDWR|O_NONBLOCK); 
  if(poutLun == - 1) return -1;

#endif

  return 0;

}
 

//================================================================
  int OS_CTL_write__ (char *sBufOut, int sSiz) {
//================================================================
// send line to remote partner

  write (poutLun, sBufOut, sSiz);

  // printf("OS_CTL_write__ |%s|\n",sBufOut);

  return 0;
}


//================================================================
  int OS_CTL_read_init (char *fn) {
//================================================================
// retCode: -1   error open pipe <tmpDir>CTRLpin

  int         irc;
  char        fnin[256];
  struct stat att1;


  UtxTab_add (&AP_TxTab1, fn);
  UtxTab_query (&pnin, &AP_TxTab1);

  // printf("OS_CTL_read_init |%s|\n",fn);


#ifdef __MINGW64__
  remove (pnin);    // delete File (sonst get das rename ned ..)
  goto L_exit;

#else
  // Linux
  // create input-pipe
  if(stat(pnin, &att1) == -1) {
    // create named-pipe. See also mknod.
    // O_RDWR=read and write
    irc = mkfifo (pnin, O_RDWR|S_IWUSR|S_IWGRP|S_IRUSR|S_IRGRP);
    if(irc == -1) return -3;
  }
#endif

  // fix tempfilename
  sprintf(fnin, "%sCTRLfin",AP_get_tmp_dir());
    // printf(" CTRL_CB__ fnin=|%s|\n",fnin);

  // open input-file (keep it open permanently)
  // finLun = open (fnin, O_RDWR|O_NONBLOCK);
  finLun = fopen (fnin, "w+");   // write & read
  if(!finLun) return -2;

  // open output-pipe (keep it open permanently)
#ifdef __MINGW64__
  pinLun = open (pnin, O_RDWR);
#else
  pinLun = open (pnin, O_RDWR|O_NONBLOCK);
#endif
  if(pinLun == - 1) return -1;

  L_exit:

  return 0;

}


//================================================================
  int OS_CTL_read_reset () {
//================================================================
 

  int    cNr;
  char   s1[32];

  printf("OS_CTL_read_reset \n");

  if(pinLun == - 1) return -1;

  L_nxt:
  cNr = read (pinLun, s1, 30);
  if(cNr < 1) return 0;             // empty: return -1;
  goto L_nxt;

}


//================================================================
  char* OS_CTL_read__ (int *outSiz) {
//================================================================
// get next line (terminated with '\n') from inputpipe.
// Output:
//   sSiz     size of data
//   RetCode  NULL = no input exists or input not yet complete.
//            else pointer to data. Dataspace may be modified.


#define SIZ_sBuf 256
  static char sBuf[SIZ_sBuf];
  static int  cNr=0;      // nr of chars in buffer sBuf
  static char *cPos=NULL; // startPos of unused input in sBuf; NULL=none.
  static char *cEnd=NULL;
  static int  finSiz = 0;

  char    *po, *cNxt;
  int     sl, irc;


  // printf("CTRL_read__\n");

  if(pinLun == -1) return NULL;


  // check if complete buffer processed;
  if(cPos) goto L_nxt;


  // complete buffer processed; start new read
  // read from pipe
  L_read:
  cNr = read (pinLun, sBuf, SIZ_sBuf);
  if(cNr < 1) return NULL;             // empty

  // delimit the input
  cEnd = &sBuf[cNr];
  *cEnd = '\0';
  --cEnd;
    // printf(" read pin %d |%s|\n",cNr,sBuf);


  cPos = sBuf;

    // // testDisplay
    // for(sl=0; sl<cNr; ++sl) printf(" [%d] = %d\n",sl,sBuf[sl]);


  //----------------------------------------------------------------
  // find next '\n'; change it into \0 and return its position
  L_nxt:
    // printf(" cPos=|%s|\n",cPos);
// TODO: CTRL_read__: skip \n inside "textStrings"
  cNxt = strchr (cPos, '\n');
  if(!cNxt) goto L_input_broken;


  *cNxt = '\0';
  sl = cNxt - cPos;  // skip the \n
  ++cNxt;
  if(cNxt >= cEnd) cNxt = NULL;
    // printf("  CTL_read__cNxt=|%s| sl=%d finSiz=%d\n",cNxt,sl,finSiz);

  // get outspc
  po = IE_buf;      // size mem_cbuf1_SIZ!

  if(finSiz + sl > mem_cbuf1_SIZ) {
    TX_Error("OS_CTL_read__ overflow");
    finSiz = 0;
    cPos = NULL;
    return NULL;
  }

  // copy swapfile
  if(finSiz) {
    fflush (finLun);
    rewind (finLun);
    fread (po, finSiz, 1, finLun);
    rewind (finLun);
  }

  // copy output
  strncpy (&po[finSiz], cPos, sl + 1);

  finSiz = 0;
  cPos = cNxt;

    // printf("ex OS_CTL_read__ |%s|\n",po);

  return po;



  //----------------------------------------------------------------
  // no '\n' found in input.
  // add text into swapfile.
  L_input_broken:
    sl = cEnd - cPos + 1;
      // printf("  CTRL_read__ sl=%d\n",sl);

  // cPos ist the start of the line; sl is the lenght.
  fwrite (cPos, sl, 1, finLun);
  finSiz += sl;

  cPos = NULL;

  goto L_read;


}


// EOF
