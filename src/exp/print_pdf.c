// PDF export.                                    RF 2016-03-09
/*
 *
 * Copyright (C) 2016 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
2016-03-09 New. RF.

-----------------------------------------------------
*/
/*!
\file  ../exp/print_pdf.c
\brief write PDF
\code
=====================================================
List_functions_start:

PRI_PDF__    MAIN ENTRY

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

make -f xa_print_pdf.mak

...................................
GUI via Notebook-pages;

Print | pdf-File | ps - File | HPGL - File | Preview

Wie ein Image machen ?
export as jpg < bmp ?


...................................
parameters for PDF:
- preview / command                    evince | xpdf | firefox file://<fn>
OR print to file /outfilename
OR print direct / printcommand         lpr -PMFC7360N <fn> 
     use "evince -w <fn>

Papersize   A4 | A3

Landscape or not


...................................
Test ob ps2pdf installiert:
OS_ckSW_ps2pdf
  which ps2pdf

export to ps,  dann:
ps2pdf -sPAPERSIZE=a3 <fn>.ps <fn>.pdf

UI_WinPrint1   ??



-------------------------------------------------------------------
*/



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
__declspec(dllexport) int PRI_PDF__ (char*);
#define extern __declspec(dllimport)
#endif


#include "../ut/ut_geo.h"

#include "../db/ut_DB.h"                  // DB_GetObjGX

#include "../xa/xa_mem.h"                 // memspc501
#include "../xa/xa.h"                     // AP_STAT




//===========================================================================
  int PRI_PDF__ (char* fnam)  {
//===========================================================================
/// export as PDF
/// Input:
///   AP_stat.subtyp  0 = DXF-R12
///                   1 = DXF-2000

  int    i1;

  printf("PRI_PDF__ |%s|\n", fnam);

  i1 = OS_ck_SW_is_installed ("ps2pdf");
  if(i1) {
    TX_Print("**** ERROR: ps2pdf is not installed. ****");
    // not installed; ERRMSG.
    return -1;
  }


  return 0;

}

// EOF
