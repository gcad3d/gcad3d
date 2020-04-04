// xa_msg.                                   2009-12-11  RF
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
- make MSG_msgLst_keyLst
  make a list of messages from a list of keys 
    for use by GUI_OptMenChg GUI_OptMen

      // get tooltiptext
      if(strLst[i1]) {
        tipLst[i1] = MSG_get_str (strLst[i1]);
        // copy string -> tempspace ....
        strcpy (tipLst[i1] .....  ???
      } else {
        tipLst[i1] = NULL;
      }

-----------------------------------------------------
Modifications:
2009-11-20 neu erstellt. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void MSG(){}
#endif
/*!
\file  ../xa/xa_msg.c
\brief functions for output of messages in different languages 
\code
=====================================================
List_functions_start:

-------------- ERROR-MESSAGES ----------------------------------
MSG_ERR__       errormessage (key, additional_text)         see INF_DEB_MSG_ERR
MSG_ERR_out     error/warning/info
MSG_ERR_sev     get severity (1|2|3) from errorcode (< 0)

-------------- SYSTEM-MESSAGES ----------------------------------
APP_MSG_pri_0   print message with 0 parameter (TX_Print)
APP_MSG_pri_1   print formatted message with 1 parameter (TX_Print)
APP_MSG_err_0   error (TX_Error)
APP_MSG_err_1   print errormessage with 1 parameter, formatted ..
APP_MSG_Tip     add tooltip to last created GUI-widget
APP_MSG_get_0   get application-message with 0 parameters
APP_MSG_get_1   get msg with parameter from messagefile for ctive application
APP_MSG_read    read msg from active application-messagefile
APP_MSG_close

MSG_const__     get const-message from nr
MSG_const_init  load file xa/msg_const_de.txt -> MSG_tab

MSG_Tip         add tooltip to last created GUI-widget
MSG_pri_0       print message with 0 parameter (TX_Print)
MSG_pri_1       print formatted message with 1 parameter (TX_Print)
MSG_get_0       return message 
MSG_get_1       get formatted message with 1 parameter
MSG_err_0       error (TX_Error)
MSG_err_1       print errormessage with 1 parameter, formatted ..
MSG_get_wTab    get a table of words from integers
MSG_get_str     get message from string (get key from string; 0 parameters)
MSG_Init        open / reopen

MSG_get__
MSG_read
MSG_fread       read message <key> in (open) file <fpIn>

List_functions_end:
=====================================================




Defaultlanguage is "_en"  english
                    _de   german
                    _es   spanish
                    _fr   french
                    _it   italian


//================================================================
  Application-Messages:
//================================================================
Each application (plugin) can have its own messagefile:
{docDir}/msg/{appnam}_{language}.txt
File ~/gCAD3D/xa/app1_en.txt   would be used by plugin
~/gCAD3D/binLinux32/plugins/app1.so
Opening & closing this files is done automatic.

Fileformat of the messagefile:
{keyCode} {message}
  Messagelinelength may not exceed 255 chars
  message can have a parameter (" %% " will be replaced by the parameter).
    can have continuation-lines (last char \, first char of next line \)

Functions APP_MSG_*

#include "../xa/xa_msg.h"              // Messages





//================================================================
  Standard-Messages
//================================================================
Get messages, identified by a integer; ../xa/xa_msg.h eg func_not_impl

Examples:
  MSG_ERR__ (func_not_impl, "E001 - %d",4);

  



//================================================================
  Messages for the menu's:
//================================================================
Get messages, identified by a keyword

Files:   ../../doc/msg/msg_en.txt ../../doc/msg/msg_de.txt ..

Format:  keyword=message

Tools for manipulating the language-files: see INF_MSG_new

Functions:

  // message without parameters:
  MSG_pri_0("PEDmod1");                       // TX_Print message
  MSG_Tip ("MMclear");                        // add Tooltip to GTK-widget
  GUI_Label_1(box0, MSG_get_0("MD_On"), 0);    // get message for label

  // message with 1 parameter:
  MSG_err_1 ("FileOp",fnam);                  // FileOpenError
  MSG_get_1 (s2, 256, "SEL_sDir", "%s", s1);  // s1 replaces %% in message





//================================================================
  Constant-Messages
//================================================================
Often used, short messages; identified by integers (../xa/xa_msg.h)

Messagecodes:
  ../xa/xa_msg.h

Messagefiles:
{baseDir}/gCAD3D/xa/msg_const_{language}.txt
  eg msg_const_de.txt  msg_const_en.txt
  ../../doc/msg/msg_const_de.txt

Messagecodes: enum in file
{baseDir}/gCAD3D/xa/xa_msg.h

Constant-Messages cannot have parameters.

Examples:
  GUI_Tip        (MSG_const__(MSG_upd));
  strcpy(s1, MSG_const__(MSG_open));   // "Öffnen");



\endcode *//*

Examples:



//================================================================
  Tools for manipulating the language-files: see INF_MSG_new
//================================================================

\endcode *//*----------------------------------------


Hauptmenütexte bleiben englisch, aber länderspezifische Tooltips.


*/


#ifdef _MSC_VER
#include "MS_Def0.h"
#endif



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // ...


#include "../ut/ut_geo.h"
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_txTab.h"            // TxtTab
#include "../ut/ut_os.h"               // OS_
#include "../db/ut_DB.h"               // DB_mdlNam_iBas
#include "../xa/xa_msg.h"              // MSG_read
#include "../xa/xa.h"                  // APP_act_nam AP_lang
#include "../ut/ut_memTab.h"           // MemTab
#include "../xa/xa_mem.h"              // memspc501


//========= Extern Var: =====================



//========= Lokale Var: =====================
static char*  MSG_tab=NULL;
static FILE   *APP_MSG_fp=NULL;
static FILE   *MSG_fp=NULL;

#define MSG_bSiz 512
static char   MSG_buf[MSG_bSiz];



//================================================================
// ERROR-MESSAGES 
//================================================================

char *MSG_ERR_txt[]={
  "*** INF:",      // iSev = 0
  "*** WNG:",      // iSev = 1
  "*** ERROR:"     // iSev = 2
  "*** BREAK:"     // iSev = 3
};


/// messages for MSG_ERR_out MSG_ERR__
char *MSG_ERR_tab[]={
  "internal error - exit",                ///< 3 99 internal Error,
  "memtab out of memory error",           ///< 2 98 error, exit right now
  "not implemented yet error - exit",     ///< 2 97 error, exit right now
  "memspace already in use",              ///< 2 96 ERR_MEMSPC_IN_USE
  "not implemented yet error - info",     ///< 2 95 info only - TODO
  "function not implemented",             ///< 1 94 func_not_impl,
  "subModel undefined",                   ///< 1 93 subModel_undefined,
  "DB-object undefined",                  ///< 1 92 db_obj_undefined,
  "file open",                            ///< 1 91 file_open,
  "testExit",                             ///< 1 90 testExit,
  "OBSOLETE",                             ///< 0 89 
  "unsupported",                          ///< 0 88 
  "canceled by user",                     ///< 0 87 ERR_USER_ABORT
  "uu"                                    ///< -
};




//========= Prototypen: =====================


//================================================================
  int APP_MSG_close () {
//================================================================

  // printf("APP_MSG_close \n");

  if(APP_MSG_fp) {
    fclose (APP_MSG_fp);
    APP_MSG_fp = NULL;
  }

  return 0;

}


//================================================================
  char* APP_MSG_read (char *sbuf, int bufSiz, char *key) {
//================================================================
// read msg from ../doc/msg/<applicationName>_<actlang>.txt -> sbuf
// eg ../doc/msg/msg_de.txt
// see also MSG_read MSG_Init


  // printf("APP_MSG_read |%s| siz=%d\n",key,bufSiz);


  // if not yet open: get aplicationName, open its message-file
  if(!APP_MSG_fp) {
    sprintf(MSG_buf, "%smsg%c%s_%s.txt", OS_get_doc_dir(), fnam_del,
            APP_act_nam, AP_lang);
      // printf(" msgfile = |%s|\n",MSG_buf);


    if((APP_MSG_fp=fopen(MSG_buf,"r")) == NULL) {
      // TX_Print("***** MESSAGEFILE %s DOES NOT EXIST *****",MSG_buf);
      printf("***** MESSAGEFILE %s DOES NOT EXIST *****",MSG_buf);
      sprintf(MSG_buf, "%smsg%cmsg_en.txt",OS_get_doc_dir(),fnam_del);
      if((APP_MSG_fp=fopen(MSG_buf,"r")) == NULL) return NULL;
    }

  }

  return MSG_fread (sbuf, bufSiz, key, APP_MSG_fp);

}


//================================================================
  char* MSG_fread (char *sbuf, int bufSiz, char *key, FILE *fpIn) {
//================================================================
/// \code
/// read message "<key> in (open) file <fpIn>
/// Output:
///   sbuf
///   retCod   NULL key not found
///            else position of text without key in sbuf
/// Howto create new message: see INF_MSG_new
/// \endcode
 
  int    iloop, ilen;
  char   *cp1, *cp2, *cp3, sKey[128];


  // printf("MSG_fread |%s| %d len1=%ld\n",key,bufSiz,strlen(key));

  //----------------------------------------------------------------
  if(!fpIn) {printf("MSG_fread E000\n"); return NULL;}    // 2011-03-16

  ilen = strlen(key);
  if(ilen > 80) {printf("MSG_fread E001\n"); return NULL;}

  iloop = 0;
  strcpy(sKey, key);
  strcat(sKey, "=");
  ++ilen;
    // printf(" _fread-sKey |%s| %d\n",sKey,ilen);



  // loop tru file ..
  L_nxt:
    // read next line > sbuf
    if(fgets (sbuf, bufSiz, fpIn) == NULL) {++iloop; goto L_restart;}
      // printf(" _fread-L_nxt: |%s|\n",sbuf);

    // skip different key
    // if(strncmp(sbuf, key, ilen)) goto L_nxt;
    if(strncmp(sKey, sbuf, ilen)) goto L_nxt;

    // if(sbuf[ilen] != ' ') goto L_nxt;
    // if((sbuf[ilen] != ' ')&&(sbuf[ilen] != '=')) goto L_nxt;  // 2015-11-14

    // found key ..
    cp1 = &sbuf[ilen];        // skip the key
    // UTX_pos_skipLeadBlk (cp1);
      // printf("   MSG_read 1 |%s|\n",sbuf);

    // remove the \n
    cp2 = UTX_CleanCR (cp1);

    L_br_nxt:
      cp2 = strstr (cp1, "<br>");
      if(cp2) {
        *cp2 = ' '; ++cp2; *cp2 = '\n'; ++cp2; *cp2 = ' '; ++cp2; *cp2 = ' ';
        goto L_br_nxt;
      }

/*
    cp2 = UTX_CleanCR (cp1);            // remove the \n

    // check for continuation-line (old version: txt\\n + \nxtln\\n ...)
    L_cont:
      --cp2;
      if(*cp2 == '\\') {
        *cp2 = '\n';
        ++cp2;
        ilen = fgetc (fpIn);
        if(ilen != '\\') goto L_err1;
        ilen = UTX_Siz (bufSiz, sbuf, cp2);   // bufSiz - (cp2 - sbuf);
          // printf(" ilen=%d\n",ilen);
          // printf("|%s|\n",sbuf);
        if(fgets (cp2, ilen, fpIn) == NULL) goto L_err1;
        cp2 = UTX_CleanCR (cp2);
        goto L_cont;
      }
*/
        // printf("ex MSG_read 1 |%s|\n",cp1);

    return cp1;

  L_restart:
    if(iloop < 2) {
      rewind (fpIn);
      goto L_nxt;
    }


  L_err1:
    printf("****** ERROR MSG_fread %s\n", key);
    TX_Print("****** ERROR MSG_fread %s",key);
      // sprintf(sbuf, "echo %s >> t1", key);
      // OS_system (sbuf);
      // TX_Error("TEST");  // TEST ONLY; use in debug-mode
    return NULL;

}


//================================================================
  char* APP_MSG_get_0 (char *key) {
//================================================================
/// get application-message with 0 parameters.


  char    *cp1;

  cp1 = APP_MSG_read (MSG_buf, MSG_bSiz, key);
  return cp1;

}


//================================================================
  int APP_MSG_pri_0 (char *key) {
//================================================================
/// print message with 0 parameters.

  char    *cp1;

  cp1 = APP_MSG_read (MSG_buf, MSG_bSiz, key);
  if(cp1 == NULL) return -1;

  TX_Write (cp1);

  return 0;

}


//================================================================
  char* APP_MSG_get_1 (char *key, char *fmt, ...) {
//================================================================
/// \code
/// get message with 1 parameter, formatted ..
/// get msg from messagefile for the active application
/// \endcode

// MSG_pri_1 ("TST0", "%d %.2f",5,12.3489);

  int     irc;
  va_list va;


  va_start(va, fmt);

  irc = MSG_get__ (MSG_buf, MSG_bSiz, key, APP_MSG_fp, fmt, &va);

  va_end (va);

  if(irc == 0) return NULL;

  return MSG_buf;

}


//================================================================
  int APP_MSG_pri_1 (char *key, char *fmt, ...) {
//================================================================
/// \code
/// print message with 1 parameter, formatted ..
/// get msg from messagefile for the active application
/// \endcode

// MSG_pri_1 ("TST0", "%d %.2f",5,12.3489);

  int     irc;
  va_list va;


  va_start(va, fmt);

  irc = MSG_get__ (MSG_buf, MSG_bSiz, key, APP_MSG_fp, fmt, &va);

  va_end (va);

  if(irc == 0) TX_Print (MSG_buf);

  return 0;

}


//================================================================
  int APP_MSG_err_0 (char *key, ...) {
//================================================================
// print errormessage with 0 parameters.

  char    *cp1;


  printf("MSG_err_0 |%s|\n",key);

  cp1 = APP_MSG_read (MSG_buf, MSG_bSiz, key);
  if(cp1 == NULL) return -1;
    // printf(" cp1=|%s|\n",cp1);

  TX_Error (cp1);

  return 0;

}


//================================================================
  int APP_MSG_err_1 (char *key, char *fmt, ...) {
//================================================================
// print errormessage with 1 parameter, formatted ..
// get msg from ../msg/<appNam>_<lng>.txt

// APP_MSG_pri_1 ("TST0", "%d %.2f",5,12.3489);

  int     irc;
  va_list va;


  printf("APP_MSG_err_1 |%s|\n",key);

  va_start(va, fmt);
  irc = MSG_get__ (MSG_buf, MSG_bSiz, key, APP_MSG_fp, fmt, &va);
  va_end (va);

  if(irc == 0) TX_Error (MSG_buf);

  return 0;

}


//================================================================
  int APP_MSG_Tip (char *key) {
//================================================================
/// APP_MSG_Tip         add tooltip to last created GUI-widget

  char    *cp1;

  // printf("MSG_Tip |%s|\n",key);


  cp1 = APP_MSG_read (MSG_buf, MSG_bSiz, key);
  if(cp1 == NULL) return -1;
    // printf(" cp1=|%s|\n",cp1);
    
  GUI_Tip (cp1);

  return 0;
    
}   



//================================================================
  int MSG_Init (char *lang) {
//================================================================
// open file msg/msg_<sLang>.txt on lun MSG_fp

  char     fnam[200];

  // printf("MSG_Init |%s|\n",lang);
  //printf("  bas_dir  = |%s|\n",OS_get_bas_dir());
  //printf("  fnam_del = |%c|\n",fnam_del);


  if(MSG_fp) fclose (MSG_fp);

  sprintf(fnam, "%smsg%cmsg_%s.txt",OS_get_doc_dir(),fnam_del,lang);
    // printf("MSG_Init msgfile = |%s|\n",fnam);


  if((MSG_fp=fopen(fnam,"r")) == NULL) {
    // TX_Print("***** MESSAGEFILE %s DOES NOT EXIST *****",fnam);
    printf("***** MESSAGEFILE %s DOES NOT EXIST *****",fnam);
    sprintf(fnam, "%smsg%cmsg_en.txt",OS_get_doc_dir(),fnam_del);
    if((MSG_fp=fopen(fnam,"r")) == NULL) return -1;
  }


  return 0;

}

 

//================================================================
  int MSG_Tip (char *key) {
//================================================================
/// \code
/// MSG_Tip         add tooltip to last created GUI-widget
/// msg out of files  msg_de.txt  msg_en.txt ..
/// \endcode

// how to add new message: see INF_MSG_new

  char    *cp1;

  // printf("MSG_Tip |%s|\n",key);
    
    
  cp1 = MSG_read (MSG_buf, MSG_bSiz, key);
  if(cp1 == NULL) return -1;
    // printf(" cp1=|%s|\n",cp1);

  GUI_Tip (cp1);

  return 0;

}


//================================================================
  int MSG_pri_0 (char *key) {
//================================================================
/// print message (TX_Write) with 0 parameters.
// see INF_MSG_new

// MSG_pri_0 ("PED_msg1");
 
  // int    iloop, ilen;
  char    *cp1;


  // printf("MSG_pri_0 |%s|\n",key);
  // TEST
  // if(!strcmp(key, "PEDe2")) AP_debug__();


  cp1 = MSG_read (MSG_buf, MSG_bSiz, key);
  if(cp1 == NULL) return -1;
    // printf(" MSG_pri_0-cp1 = |%s|\n",cp1);

  // TX_Print (cp1);
  TX_Write (cp1);

  return 0;

}


//================================================================
  char* MSG_get_0 (char *key) {
//================================================================
// return message 
// see also MSG_get_str

  return MSG_read (MSG_buf, MSG_bSiz, key);

}


//================================================================
  int MSG_get_1 (char *msg, int msgSiz, char *key, char *fmt, ...) {
//================================================================
// copy formatted message -> msg

  int     irc;
  va_list va;


  // printf("MSG_get_1 |%s|%s|\n",key,fmt);

  va_start(va, fmt);

  irc = MSG_get__ (msg, msgSiz, key, MSG_fp, fmt, &va);

  va_end (va);

  return irc;

}


//==========================================================================
  int MSG_get__ (char *msg, int msgSiz, char *key, FILE *fpIn,
                 char *fmt, va_list *va) {
//==========================================================================
// get message & substitute " %% " by a string in va
// Input:
//   msgSiz    size of msg
//   key 
// Output:
//   msg       formatted message 

  int     ii;
  long    l1;
  char    sbuf2[200], *cp1, *cp2;


  // printf("MSG_get__ key=|%s| msgSiz=%d\n",key,msgSiz);


  vsprintf(sbuf2, fmt, *va);
    // printf("  sbuf2=|%s|\n",sbuf2);


  // get message out of messagefile
  cp1 = MSG_fread (msg, msgSiz, key, fpIn);    // 2011-02-12
  if(cp1 == NULL) return -1;
    // printf(" cp1=|%s|\n",cp1);


  // remove the key
  MEM_del_str (msg, cp1 - msg);
    // printf(" del_str |%s|\n",msg);


  // search for " %% "
  cp1 = strstr (msg, " {0}");       // was %%
  if(cp1 == NULL) {
    TX_Print("****** Parametererror 1 MSG_get__ %s",key);
    goto L_wri;
  }

  // test if %% is at end of line
  cp2 = cp1 + 4;          // was 3
  if(*cp2 == '\0') {
    ii = 4;
  } else if(*cp2 == ' ') {
    ii = 5;
  } else {
    TX_Print("****** Parametererror 2 MSG_get__ %s",key);
    goto L_wri;
  }

  // substitute string
  // l1 = sizeof(msg) - (cp2 - msg);
  l1 = msgSiz - (cp1 - msg);
  MEM_chg_str (cp1, l1, ii, sbuf2);

  // out ..
  L_wri:

    // printf("ex MSG_get__ |%s|\n",msg);

  return 0;

}


//================================================================
  int MSG_err_1 (char *key, char *fmt, ...) {
//================================================================
// print errormessage with 1 parameter, formatted ..
// get msg from ../msg/msg_de.txt

// MSG_pri_1 ("TST0", "%d %.2f",5,12.3489);

  int     irc;
  va_list va;


  // printf("MSG_err_1 |%s|\n",key);

  va_start(va, fmt);
  irc = MSG_get__ (MSG_buf, MSG_bSiz, key, MSG_fp, fmt, &va);
  va_end (va);

  if(irc == 0) TX_Error (MSG_buf);

  return 0;

}


//================================================================
  int MSG_pri_1 (char *key, char *fmt, ...) {
//================================================================
// print message with 1 parameter, formatted ..
// get msg from ../msg/msg_de.txt



  int     irc;
  va_list va;
    

  va_start(va, fmt);

  irc = MSG_get__ (MSG_buf, MSG_bSiz, key, MSG_fp, fmt, &va);
    // printf("MSG_pri_1 |%s|\n",MSG_buf);

  va_end (va);

  if(irc == 0) TX_Print (MSG_buf);

  return 0;

}


//================================================================
  int MSG_err_0 (char *key, ...) {
//================================================================
// print errormessage with 0 parameters.

  char    *cp1;


  printf("MSG_err_0 |%s|\n",key);

  cp1 = MSG_read (MSG_buf, MSG_bSiz, key);
  if(cp1 == NULL) return -1;
    // printf(" cp1=|%s|\n",cp1);

  TX_Error (cp1); 

  return 0;

}


//================================================================
  char* MSG_read (char *sbuf, int bufSiz, char *key) {
//================================================================
// read msg from ../msg/msg_de.txt -> sbuf
  
// how to add new message: see INF_MSG_new


  int    iloop, ilen;
  char   *p1, *cp1, *cp2, *cp3;


  // printf("MSG_read |%s| siz=%d\n",key,bufSiz);

  // if not yet open:
  if(!MSG_fp) {strcpy(sbuf,key); return 0;}


  iloop = 0;
  ilen = strlen(key);
  if(ilen > 60) {printf("MSG_read E001\n"); return NULL;}


  return MSG_fread (sbuf, bufSiz, key, MSG_fp);

}


//================================================================
  int MSG_const_init (char *sLang) {
//================================================================
// load file msg/msg_const_<sLang>.txt -> MSG_tab
  
  int    i1, siz1, siz_, ls;
  long   l1;
  char   s1[256], *cp1, *cp2, *spc1;
  FILE   *fpi;
  
  printf("MSG_const_init |%s|\n",sLang);
  //printf("  bas_dir  = |%s|\n",OS_get_bas_dir());
  //printf("  fnam_del = |%c|\n",fnam_del);

  // (re)open messagefile msg/msg_const_de.txt
  sprintf(s1, "%smsg%cmsg_const_%s.txt",OS_get_doc_dir(),fnam_del,sLang);
    // printf(" constmsgfile = |%s|\n",s1);


  l1 = OS_FilSiz (s1);
  if(l1 < 2) {
    TX_Error("cannot access %s",s1);
    printf("**** MSG_const_init E001 %s\n",s1);
    return -1;
  }
    // printf(" fsiz=%ld\n",l1);
  

  siz1 = l1 + 256;
  spc1 = MEM_alloc_tmp (siz1);
  siz_ = 0;

  if((fpi = fopen (s1, "r")) == NULL) {
    // TX_Print("***** MESSAGEFILE %s DOES NOT EXIST *****",s1);
    printf("***** MESSAGEFILE %s DOES NOT EXIST *****",s1);
    sprintf(s1, "%smsg%cmsg_const_en.txt",OS_get_doc_dir(),fnam_del);
    if((fpi=fopen(s1,"r")) == NULL) return -1;
  }


  // first use memspcX; later (if size is known) malloc.   
  while (!feof (fpi)) {
    if(fgets (s1, 256, fpi) == NULL) break;
    // skip comments
    if(s1[0] == '#') continue;
    // remove key
    cp1 = strchr(s1, '=');
    if(!cp1) {
      printf("**** ERROR const-message \"%s\"\n",s1);
      continue;
    }
    ++cp1;  // skip '='
    // replace '\n' by '\0'
    cp2 = strchr(cp1, '\n');
    if(cp2) *cp2 = '\0';
      // printf("_const_init |%s|\n",cp1);
    // add cp1-cp2 -> spc1
    ls = cp2 - cp1 + 1;       //printf(" ls=%d\n",ls);
    strncpy (&spc1[siz_], cp1, ls);
    siz_ += ls;
    if(siz_ >= siz1) {
      printf("**** ERROR const-message EOM-1");
      break;
    }
  }
  fclose (fpi);

  // copy -> MSG_tab
  MSG_tab = (char*) realloc ((void*)MSG_tab, siz_ + 8);
  if(MSG_tab == NULL) {TX_Error("MSG_const_init EOM-2"); return -1;}
  memcpy(MSG_tab, spc1, siz_);


/*
  fread (MSG_tab, l1, 1, fpi);
  fclose (fpi);
  // replace all '\n' by '\0'
  cp1 = MSG_tab;
  for(;;) {
    cp2 = strchr(cp1, '\n');
    if(cp2 == NULL) break;
    *cp2 = '\0';
    cp1 = cp2 + 1;
  }
*/

    // printf("ex-MSG_const_init \n");
    // DEB_mcheck__ ();

  return 0;

}


//================================================================
  char* MSG_const__ (int iMsg) {
//================================================================
/// \code
/// MSG_const__     get const-message from nr
///
/// message-constants: see file msg_const_de.txt
/// p1 = MSG_const__ (MSG_ok);         // 0=first message = yes
///   iMsg - eg MSG_ok - see xa_msg.h
/// Add new constants:
///   add ID in xa_msg.h;
///   add msg in msg_const_de.txt (and all others, msg_const_en.txt ..)
/// \endcode

  int    i1;
  char   *cp1, *cp2;


  // printf("MSG_const__ %d\n",iMsg);

  // if(!MSG_tab) {TX_Error("MSG_const__ E000"); return NULL;}
  if(!MSG_tab) {printf("MSG_const__ E000-%d\n",iMsg); return NULL;}

  i1  = -1;
  cp1 = MSG_tab;

  L_nxt:
    if(*cp1 != '#') ++i1;
    cp2 = strchr(cp1, '\0');
    if(cp2 == NULL) return NULL;

    if(i1 == iMsg) {
        // printf("ex MSG_const__ |%s|\n",cp1);
      return cp1;
    }

    cp1 = cp2 + 1;
    goto L_nxt;

    // printf("ex MSG_const__ |NULL|\n");
  return NULL;

}


//================================================================
  char* MSG_get_str (char *keyStr) {
//================================================================
/// \code
/// MSG_get_str     get message from string (get key from string; 0 parameters)
/// max returned size = 256 chars
/// see also MSG_get_0
/// \endcode

  char   *p1, key[64];

  // printf("MSG_get_str |%s|\n",keyStr);

  if(strlen(keyStr) > 60) return NULL;

  strcpy(key, keyStr);
  UTX_CleanAN (key);  // change blanks in key into '_'
    // printf("MSG_get_str |%s|\n",key);

  return MSG_read (MSG_buf, MSG_bSiz, key);

}


//================================================================
  int MSG_get_wTab (TxtTab *txTab, int tabSiz) {
//================================================================
/// get a table of words from integers

  int    i1;
  char   *ps;

  // printf("MSG_get_wTab %d\n",tabSiz);


  for(i1=0; i1<tabSiz; ++i1) {
    // get string and add it to stringTable
    ps = MSG_const__ (i1);
      // printf(" |%s|\n",ps);
    
    if(ps) UtxTab_add (txTab, ps);      // add word
  }


  // TEST:
  // for(i1=0;i1<txTab->iNr; ++i1)
     // printf(" txTab[%d]=|%s|\n",i1,UtxTab__(i1,txTab));

  return 0;

}


//=======================================================================
  int MSG_ERR_out (int msgTyp, const char *fnc, int iErr, char *txt, ...) {
//=======================================================================
/// \code
/// MSG_ERR_out     error/warning/info; use with MSG_ERR__
///   msgTypr       MSG_ERR_typ_ERR|MSG_ERR_typ_WNG|MSG_ERR_typ_INF
///   iErr          errorcode; see ERR_codes
///
/// output:       TX_Error ..
///   retCod      errorcode = iErr (< 0)
///
/// see also MSG_ERR__ MSG_ERR_sev
/// \endcode

  va_list va;
  int     irc = 0, iSev, iKey;
  char    s1[400], s2[256];


  printf("MSG_ERR_out %d |%s| %d\n",msgTyp,fnc,iErr);
  printf("  ERR_internal=%d\n",ERR_internal);


  va_start (va, txt);
  vsprintf (s1, txt, va);
  va_end (va);
  iKey = iErr - ERR_internal;   // ERR_internal = first element in enum-list
    printf(" s1=|%s| iKey=%d\n",s1,iKey);



  // TX_Print ("%s %s(): %s %s", MSG_ERR_txt[MSG_ERR_typ_ERR],
                     // fnc,
                     // MSG_ERR_tab[ikey],
                     // s1);

  iSev = MSG_ERR_sev (iErr);
    // printf(" ERR_out-iSev=%d\n",iSev);

  // s2 = "[- SM <subModelname> ]- Line <lineNr>"
  if(AP_get_modact_ind() >= 0) {
    // subModel is active ..
    sprintf(s2, "- SM %s - Line %d ",DB_mdlNam_iBas(AP_get_modact_ind()),
            ED_get_lnr_SM());
  } else {
    // mainModel is active ..
    sprintf(s2, "- Line %d ",APT_get_line_act());
  }

  TX_Print ("%s %s in %s() %s - %s",
             MSG_ERR_txt[iSev], MSG_ERR_tab[iKey], fnc, s2, s1);

  if(iSev > 1) AP_errStat_set (1);

  return iErr;

}



// EOF
