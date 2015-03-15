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
  ..

-----------------------------------------------------
Modifications:
  ..

-----------------------------------------------------
*/
/*!
\file  ../APP/DemoPlugin_Resolv.c
\brief resolve selected objects
\code
=====================================================
List_functions_start:
..
List_functions_end:
=====================================================

\endcode *//*----------------------------------------


make -f DemoPlugin_Resolv.mak

*/

#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_geo.h"              // Point ...

#include "../gui/gui__.h"

#include "../gr/ut_UI.h"               // UI_Func...
#include "../gr/ut_DL.h"               // DB_GetDLatt
#include "../db/ut_DB.h"               // DB_GetObjGX
// #include "../xa/xa_ui.h"               // UID_..




// prototypes:
  int gcad_key_CB (int key);
  int gcad_sel_CB (int src, long dl_ind);


#ifdef _MSC_VER
// die folgenden 2 Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif





//=========================================================
  int gcad_key_CB (int key) {
//=========================================================
// userKeyin callback
// dzt nicht wirklich benutzt ...


  printf("gcad_key_CB %d\n",key);


  switch (key) {

    case GUI_KeyEsc:
      gCad_fini ();
      break;

    case 'q':
      gCad_fini ();
      break;

  }

  return 0;

}


//=========================================================
  int gcad_sel_CB (int src, long dl_ind) {
//=========================================================
// userSelection-callback
// dzt nicht wirklich benutzt ...

  int      apt_typ;
  short    model_nr;
  long     apt_ind;
  Point    pt1;
  DL_Att   dla;

  printf("gcad_sel_CB source=%d ind=%ld\n",src,dl_ind);


  if(src == GUI_MouseR) {gCad_fini(); return 0;}
  if(src != GUI_MouseL) return 0;      // skip all but left mousebutt.

  if(dl_ind < 0) {
    // indicate position
    sele_get_selPos (&pt1);
    TX_Print(" indicate pos %f %f %f\n",pt1.x,pt1.y,pt1.z);
    return 0;
  }



  dla = DL_GetAtt(dl_ind);               // get DL-record
  apt_typ  = dla.typ;
  apt_ind  = dla.ind;
  model_nr = dla.modInd;    // -1=active Model, 0-n=Submodel
    printf(" typ=%d ind=%ld mnr=%d\n",apt_typ,apt_ind,model_nr);


  // if object is not in active model: cannot get it from DB.
  if(model_nr >= 0) {
    TX_Print(" obj not in active model ...");
    return 0;
  }

  TX_Print("resolv %d %d - see Textwindow",apt_typ,apt_ind);

  res_obj (apt_typ, apt_ind);


  return 0;

}


//=========================================================
  int gCad_fini () {
//=========================================================

  TX_Print("exiting DemoPlugin_Resolv. Bye.");

  DL_hili_off (-1L);         //  -1 = unhilite all
  DL_Redraw ();               // Redraw DispList

  AP_User_reset ();           // close application

  return 0;
}





//=========================================================
  int gCad_main () {
//=========================================================

  TX_Print(">>>>>>>>> gCad_main aus DemoPlugin_Resolv <<<<<<<<<<");

  // resolv DispList
  // res_DL__ ();

  // Resolv Database
  // res_DB__ ();


  // attach KeyIn
  AP_UserKeyIn_get (gcad_key_CB);

  // attach grafic selections
  AP_UserSelection_get (gcad_sel_CB);

  TX_Print(" select obj to resolv or key q to quit");


  return 0;

}


//=========================================================
  int res_DL__ () {
//=========================================================
// resolve (Cad-)DispList


  int       i1, i2, apt_typ, tra_ind, model_nr;
  long      l1, apt_ind, dlNr;
  ObjGX     ox1;
  Point2    *pt21;
  Point     *pt1, pt3;
  Plane     *pl1;
  CurvBSpl  *bs1;
  DL_Att    dla;
  ModelBas  *mdr;



  dlNr = GL_Get_DLind();               // get nr of dispListObjects


  for(l1=0; l1<dlNr; ++l1) {

    dla = DL_GetAtt(l1);                  // get alle DL-Infos
    apt_typ  = dla.typ;
    apt_ind  = dla.ind;
    model_nr = dla.modInd;    // -1=active Model, 0-n=Submodel


    // if object is not in active model: cannot get it from DB.
    if(model_nr >= 0) {
      mdr = DB_get_ModBas (model_nr);
      printf("Obj %3ld typ =%3d ind =%3ld mod=%s\n",l1,
        apt_typ,apt_ind,mdr->mnam);
      continue;
    }


    res_obj (apt_typ, apt_ind);


    if(dla.hili == ON) { printf ("..... this obj is hilite\n"); }
    // dim, disp, pick, ...
  }

  return 0;
}


//=========================================================
  int res_DB__ () {
//=========================================================
// resolve DataBase-objects

  int    i1;
  long   dbInd, dlInd, iMax;
  DL_Att dla;
  ObjGX  *o1;

  printf("res_DB__\n");


  // display stat's
  // DB_Stat ();

  // Dump DispList
  // DL_DumpObjTab ();


  // resolv all defined surfaces
  // Typ Typ_SUR        DB_GetSur
  iMax = DB_dbo_get_free (Typ_SUR);
  printf(" iMax=%ld\n",iMax);

  for(dbInd = 1; dbInd<iMax; ++dbInd) {

    o1 = DB_GetSur (dbInd, 0);

    // does Surf exist ? (if not, typ == Typ_Error)
    if(o1->typ == Typ_Error) continue;     // skip, if surf not defined

    // get DispListIndex of object ...
    dlInd = DL_find_obj (o1->typ, dbInd, -1);
    if(dlInd < 0) continue;                // no DispListRecord ..

    // get DispListRecord of object ...
    dla = DL_GetAtt(dlInd);

    printf(" dbI=%ld dlI=%ld typ=%d disp=%d\n",dbInd,dlInd,o1->typ,dla.disp);

  }




  return 0;

}

//================================================================
  int res_obj (int apt_typ, long apt_ind) {
//================================================================


  int       i1, sTyp, cNr;
  ObjGX     ox1, *op1, *oTab;
  Point     *pt1, pt3;
  Plane     *pl1;
  CurvBSpl  *bs1;
  // SurBSpl   *sbs1;
  void       *sSur;


  printf("res_obj %d %ld\n",apt_typ,apt_ind);

  
  // irc = UTO_get_DB (&oxo, &iNr, &apt_typ, apt_ind);        // get DB-record
  // if(irc < 0) return irc;

 

  // object is in active model; get objdata from DB
  ox1 = DB_GetObjGX (apt_typ, apt_ind);  // get obj from DB
  if(ox1.typ == Typ_Error) {
     printf(" obj not ok ...");
     return 0;
  }



  printf("Obj %ld typ =%3d form =%3d siz =%3d\n",apt_ind,
         ox1.typ,ox1.form,ox1.siz);



  // Typ_? see gCAD3D_prog.txt/Datentypen or ../ut/ut_geo.h
  if(ox1.form == Typ_PT) {
    pt1 = ox1.data;
    printf("  Point %ld = %f,%f,%f\n",apt_ind,pt1->x,pt1->y,pt1->z);


  //----------------------------------------------------------------
  } else if(ox1.form == Typ_PLN) {
    pl1 = ox1.data;
    printf("  Plane %ld vz=%f,%f,%f\n",apt_ind,
            pl1->vz.dx,pl1->vz.dy,pl1->vz.dz);



  //----------------------------------------------------------------
  } else if(ox1.form == Typ_CVBSP) {
    bs1 = ox1.data;
      printf("  B-Spline %ld: ptNr=%d deg=%d v0=%.3f,v1=%.3f\n",apt_ind,
              bs1->ptNr, bs1->deg, bs1->v0,bs1->v1);
      UT3D_pt_evalparCv(&pt3, bs1, bs1->v0);
      printf("  Startpt. = %f,%f,%f\n",pt3.x,pt3.y,pt3.z);
      UT3D_pt_evalparCv(&pt3, bs1, bs1->v1);
      printf("  Endpt. = %f,%f,%f\n",pt3.x,pt3.y,pt3.z);
      UT3D_stru_dump (Typ_CVBSP, bs1, "B-Spline-Curve:\n");



  //----------------------------------------------------------------
  } else if(ox1.form == Typ_SURBSP) {   // B_Spline_Surf
    UT3D_stru_dump (ox1.form, ox1.data, "B-Spline-surf:\n");



  //----------------------------------------------------------------
  } else if(ox1.form == Typ_ObjGX) {   // complexObj ...



    if(ox1.typ == Typ_SUR) {
      op1 = DB_GetSur (apt_ind, 1);
        UT3D_stru_dump (Typ_ObjGX, op1, "");
      // op1->typ: Typ_SUR Typ_SURRU Typ_SURRV Typ_SURCIR Typ_SURSTRIP
      //           Typ_SURBSP Typ_SURRBSP Typ_SURHAT


      i1 = SUR_ck_typ (&sTyp, &sSur, &cNr, &ox1);
      if(i1 < 0) return i1;
        printf(" surfTyp=%d contours=%d\n",sTyp,cNr);


      if(op1->typ == Typ_SUR) {  // Flaeche m Supportflaeche, getrimmt, gelocht
        // 1. obj: supportsurface
        // 2. obj: outline
        // 3-n. obj: domains (holes)
        oTab = op1->data;
        UT3D_stru_dump (Typ_ObjGX, &oTab[0], "supportsurface\n");       
        if(op1->siz > 1) 
        UT3D_stru_dump (Typ_ObjGX, &oTab[1], "outline\n");       
        if(op1->siz > 2) 
        UT3D_stru_dump (Typ_ObjGX, &oTab[2], "1.domain\n");       
      



      } else {
        printf(" surface %d not yet resolved ...\n",ox1.typ);
      }

    } else {
      printf(" complexObj %d not yet resolved ...\n",ox1.typ);
    }


  //----------------------------------------------------------------
  } else {
    UT3D_stru_dump (ox1.form, ox1.data, "..\n");
  }


  return 0;

}


//======================== EOF ======================
