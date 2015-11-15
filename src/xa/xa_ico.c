// aux-functions for icons                                 2009-11-06
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
2009-11-06 Created. RF.

-----------------------------------------------------
*/
#ifdef globTag
void Ico(){}
#endif
/*!
\file  ../xa/xa_ico.c
\brief aux-functions for icons  
\code
=====================================================
List_functions_start:

Ico_init         load all icons
Ico_icoNr_typ    get iconNumber from obj-typ

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



*/



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"
#include "../xa/xa_ico.h"              // ICO_PT

#include "../gui/gui__.h"              // Gtk3




// extern vars:


// local vars:


// prototypes:



//================================================================
  int Ico_init () {
//================================================================
/// Ico_init     create all icons

// types: see ../xa/xa_ico.h


  char *icoTab[]={
    "value.png",  "vector.png", "point.png",  "line.png",   "circle.png",  // 0-4
    "curve.png",  "plane.png",  "surface.png","body.png",   "image.png",   // 5-9
    "part.png",   "natMdl.png", "extMdl.png", "libPart.png","binDat.png",  // 10-
    "link.png",   "process.png", NULL};                                    // 15-

  printf("Ico_init \n");

  GUI_Ico_init (icoTab);

  return 0;
}


//================================================================
  int Ico_icoNr_typ (int typ) {
//================================================================
/// \code
/// find typeRow for type.
/// use with ICO_x in ../xa/xa_ico.h
/// \endcode

 int       ii;

  switch (typ) {

    case Typ_VAR:
      ii = ICO_Var; // 0;
      break;

    case Typ_VC:
      ii = ICO_VC; // 1;
      break;

    case Typ_PT:
      ii = ICO_PT; // 2;
      break;

    case Typ_LN:
      ii = ICO_LN;  // 3;
      break;

    case Typ_CI:
      ii = ICO_CI; // 4;
      break;

    case Typ_CV:
    case Typ_CVLNA:
    case Typ_CVPOL:
    case Typ_CVPOL2:
    case Typ_CVBSP:
    case Typ_CVRBSP:
    case Typ_CVELL:
    case Typ_CVCLOT:
    case Typ_CVCCV:
      ii = ICO_CV; // 5;
      break;

    case Typ_PLN:
      ii = ICO_PLN; // 6;
      break;

    case Typ_SUR:
      ii = ICO_SUR; // 7;
      break;

    case Typ_SOL:
      ii = ICO_SOL; // 8;
      break;

    case Typ_Note:
    case Typ_ATXT:
    case Typ_GTXT:
    case Typ_Dimen:
    case Typ_Dim3:
    case Typ_Tag:
      ii = ICO_Img; // 9;
      break;

    case Typ_Model:
      ii = ICO_refM; // 10;        // "part.png"
      break;

    case Typ_SubModel:
      ii = ICO_natM; // 11;        // "natMdl.png"
      break;

    case Typ_Mock:
      ii = ICO_extM; // 12;        // "extMdl.png"
      break;

      // Typ_CtlgPart
      // ii = ICO_libM; // 13;        // "libPart.png"

    case Typ_Process:
      ii = ICO_proc; // 16;        // "process.png"
      break;


    default:
      // TX_Print("Ico_icoNr_typ E001 %d",typ);
      printf("Ico_icoNr_typ E001 %d\n",typ);
      return -1;
  }


    // printf("ex Ico_icoNr_typ %d %d\n",ii,typ);

  return ii;

}


// EOF
