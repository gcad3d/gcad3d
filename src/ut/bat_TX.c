//   TX_Print - functions without GUI.                          RF.
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
\file  ../ut/ut_TX.c
\brief TX_Print - functions without GUI 
\code
GUI-Version: see ../xa/ut_ui_TX.c
=====================================================
List_functions_start:

TX_Print
TX_Error
TX_Write

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


Compile gcc:
cc -c ut_TX.c

*/


#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>


#define ON                 0
#define OFF                1




#include "../ut/ut_txt.h"              // UTX_CleanCR
#include "../xa/xa_msg.h"              // MSG_ERR_typ_..

#include "../ut/ut_TX.h"


/*  alle Ausgaben EIN */
int   TX_mode = ON;







///==========================================================
  void TX_Print (char* txt, ...) {
///==========================================================
/// \code
/// formatted write into messagewindow.
/// Example:
///   TX_Print (".. distance is %lf", d1);
/// \endcode


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
/// formatted errortext into messagewindow.
/// Example:
///   TX_Print (".. distance must not exceed %lf", d1);


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
  strcpy(TX_buf1, "*** ERROR:  ");
                // o123456789012
  vsprintf(&TX_buf1[12],txt,va);
  // UTX_CleanCR (TX_buf1);
  va_end(va);

  if(LOG_A_ck()) LOG_A__ (MSG_ERR_typ_ERR, TX_buf1);
  else           TX_Write (TX_buf1);

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




/*============ EOF ======================*/
