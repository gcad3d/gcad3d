//  xa/ut_ui_TX.c 
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
2001-06-06 GUI_Tx_wri statt GUI_Tx_add.

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_ui_TX.c
\brief TX_Print .. for messageWindow 
\code
=====================================================
List_functions_start:

TX_file_Print    print file into messageWindow
TX_Print         print formatted into messageWindow
TX_Error         print formatted into messageWindow & set error
TX_Write         print textstring into messageWindow

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

Ausgabe nach GUI_Tx_wri (mit GUI);
die Funktionen ohne GUI sind in ut/ut_TX.c


*/


#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>


#define ON                 0
#define OFF                1




#include "../ut/ut_txt.h"        /* f.UTX_CleanCR */
#include "../ut/ut_os.h"        // OS_beep
#include "../ut/ut_TX.h"               // TX_Print

#include "../xa/xa_msg.h"       // MSG_Error
#include "../xa/xa.h"           // AP_STAT printd






/*  alle Ausgaben EIN */
static int   TX_mode = ON;


static char   TX_buf1[1024];




// Protos:
 int AP_errStat_set (int);




//================================================================
  int TX_file_Print (char *fNam) {
//================================================================
/// TX_file_Print    print file into messageWindow

  FILE  *fpi;


  if ((fpi = fopen (fNam, "r")) == NULL) {
    TX_Error ("TX_file_Print: Error open %s",fNam);
    return -1;
  }
  
  while (!feof (fpi)) {
    if (fgets (TX_buf1, 1020, fpi) == NULL) break;
    TX_Print (TX_buf1);
  }

  fclose(fpi);

  return 0;

}
 

//==========================================================
  void TX_Print (char* txt, ...) {
//==========================================================
/// \code
/// display message
/// 0-10 parameters; max. 250 characters
/// Example:
///   TX_Print (".. distance is %lf", d1);
/// \endcode

// OFFEN: Fehler bei:
// TX_Print ("define 50% or 80%, then select..);  

  va_list va;


  if(TX_mode == OFF) {
    return;
  }



  /* wenn UI-Aisgabefenster noch nicht up, exit. */
  /* if(GR_initialized != 1) { */
    /* return; */
  /* } */


  va_start (va, txt);
  vsprintf(TX_buf1, txt, va);
  va_end(va);

  // sprintf(TX_buf1,txt,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11);
  UTX_CleanCR (TX_buf1);

  if(strlen(TX_buf1) > 255) {
    TX_buf1[256] ='\0';
  }

  TX_Write (TX_buf1);


}





//==========================================================
  void TX_Error (char* txt, ...) {
//==========================================================
/// \code
/// display message and raise error
/// 0-10 parameters; max. 250 characters
/// Example:
///   TX_Print (".. distance must not exceed %lf", d1);
///
/// Set/reset Errors: see AP_errStat_set ()
/// \endcode



  char    *p1;
  va_list va;


  // printf("TX_Error |%d| stat=%d\n",txt,AP_stat.sysStat);

  va_start(va,txt);

  // strcpy(TX_buf1, "*** Fehler: ");
                   // o123456789012
  // vsprintf(&TX_buf1[12],txt,va);

  sprintf(TX_buf1, "*** %s: ", MSG_const__(MSG_Error));
  p1 = strchr(TX_buf1, '\0');
  vsprintf(p1, txt, va);

  va_end(va);


  UTX_CleanCR (TX_buf1);

  TX_Write (TX_buf1);
    printf(" %s\n",TX_buf1);   // 2013-09-18


  AP_errStat_set (1);

  OS_beep ();

}

//==========================================================
  void TX_Write (char* txt) {
//==========================================================
///  display message


  if(TX_mode == OFF) {
    return;
  }


  // printf("TX_Write %s\n",txt);

  // if gui is not up yet:
  if(AP_stat.sysStat < 1) {
    // GUI_MsgBox (txt);
    printf("*********** %s\n",txt);


  } else {
    UI_winTX_prt (txt);
  }

#ifdef DEB
  if(AP_stat.debStat) {
    // printf("DEB: |%s|\n", txt);
    fprintf(AP_deb_fp, "%s\n", txt);
    return;
  }
#endif

}





/*============ EOF ======================*/
