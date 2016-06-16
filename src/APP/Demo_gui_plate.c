/* example dialog.
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
\file  ../APP/DemoKondensatorplatte.c
\brief dialog demo
\code
=====================================================
List_functions_start:
..
List_functions_end:
=====================================================

\endcode *//*----------------------------------------


make -f DemoKondensatorplatte.mak

*/

#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef _MSC_VER
// die folgenden 2 Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_os.h"               // OS_ ..

#include "../gui/gui__.h"

#include "../ut/func_types.h"               // UI_Func...
#include "../gr/ut_DL.h"               // DB_GetDLatt
#include "../gr/ut_GL.h"               // GL_GetCurpos0
#include "../db/ut_DB.h"               // DB_GetObjGX
#include "../ci/NC_Main.h"             // WC_ask_ModSiz

#include "../xa/xa_mem.h"                    // gen.purp.memblocks
#include "../xa/xa_ui.h"                     // UID_..





// ext aus xa.c:
extern  char      WC_modnam[128];    // der Modelname
extern  char      AP_dir_open[128];  // Verzeichnis f Open, ..




  char memspc1[50000];
  char memspc2[50000];

  static MemObj win0;





//=========================================================
  int dia_KeyIn (int key) {
//=========================================================
// userKeyin callback


  printf("dia_KeyIn %d\n",key);

  switch (key) {

    case GUI_KeyEsc:   // see ../gui/gui_types.h
      gCad_fini ();
      break;

    case 'q':
      gCad_fini ();
      break;

  }

  return 0;

}


//=========================================================
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  TX_Print("DemoKondensatorplatte: bye ..");

  GUI_Win_kill (&win0);

  AP_User_reset ();      // close application

  return 0;

}


//=========================================================
  int gCad_main () {
//=========================================================
// init userfunction

  printf("DemoKondensatorplatte starting\n");


  TX_Print("--->>>>>> gCad_main aus DemoKondensatorplatte <<<<<<<---\n");
  TX_Print("select Menu;  Esc = reset, q = exit");

  // init diawin
  // dia_win_main (NULL, (void*)UI_FuncInit);
  dia_win_main (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));

  // attach KeyIn
  AP_UserKeyIn_get (dia_KeyIn);


  return 0;
}


//=========================================================
  int dia_win_main (MemObj *mo, void **data) {
//=========================================================


  static MemObj wEnt_r1, wEnt_r2;

  int           i1;
  double        d1, d2;
  char          *p1, *p2;
  char          cbuf[256];
  MemObj        box0, wact;



  i1 = GUI_DATA_I1;
  printf("dia_win_main %d\n",i1);


  switch (i1) {


    //---------------------------------------------------------
    case UI_FuncInit:
      printf("dia init\n");

      win0 = GUI_Win__ ("Kondensatorplatte",dia_win_main,"");

      box0 = GUI_box_v (&win0, "");

      wEnt_r1 = GUI_entry__ (&box0, "   Hauptradius","100",NULL,NULL,"");
      wEnt_r2 = GUI_entry__ (&box0, "   Rohrradius ","12",NULL,NULL,"");
      GUI_button__(&box0,"als Flaeche",  dia_win_main, &GUI_FuncUCB1,"");
      GUI_button__(&box0,"als Zeichnung",dia_win_main, &GUI_FuncUCB2,"");
      GUI_button__(&box0,"Export Iges",  dia_win_main, &GUI_FuncUCB3,"");
      GUI_button__(&box0,"Exit ",        dia_win_main, &GUI_FuncExit,"");

      GUI_Win_up (NULL, &win0, 0);  // always on top
      GUI_Win_go (&win0);

      break;




    //---------------------------------------------------------
    case UI_FuncUCB1:
    case UI_FuncUCB2:
      printf("UI_FuncUCB1/2\n");         // export as surface
      p1 = GUI_entry_get (&wEnt_r1);
      d1 = strtod(p1, &p2);
      p1 = GUI_entry_get (&wEnt_r2);
      d2 = strtod(p1, &p2);
      printf(" d1=%f d2=%f\n",d1,d2);
      uf1 (i1, d1, d2);
      break;


    //---------------------------------------------------------
    case UI_FuncUCB3:
      printf("UI_FuncUCB3 - Export Iges\n");
      // sprintf(cbuf, "%s.igs",OS_get_bas_dir(),WC_modnam);
      sprintf(cbuf, "%s%s.igs",AP_dir_open,WC_modnam);
      TX_Print("export -> file %s",cbuf);
      AP_ExportIges__ (cbuf);
      break;


    //---------------------------------------------------------
    case UI_FuncExit:
      gCad_fini ();
      return 0;


  }

  return 0;

}


//=========================================================
  int uf1 (int mode, double r1, double r2) {
//=========================================================
// export as surface

#define maxNr     10000

  int     i1, typ, iNr;
  long    ind, indTab[maxNr];
  char    cBuf[256];
  double  d1, dx, dy, lMax, lInc;
  Point   pt1, pt2;


  printf("uf1 %d %f %f\n",mode,r1,r2);


  UI_men__ ("new");          // init mainmodel


  // compute and set new Modelsize (if 
  // make bigger Modelsize (reduce nr of points for display)
  d1 = r1 * 8.;
  d1 = UTP_db_rnd5 (d1);     // round
  if(WC_ask_ModSiz() < d1) NC_setModSiz (d1);
  printf(" Modsiz %f %f\n",d1,WC_ask_ModSiz());


  AP_obj_2_txt (NULL, 0, NULL, 0L); // Init Objektindexe

  UTF_clear1();                    // clear auxBuf

  // comment
  UTF_add1_line ("# Demo_gui_plate - start");
  sprintf(cBuf,"#  R1=%f R2=%f %s",r1,r2,OS_date1());
  UTF_add1_line (cBuf);

  iNr = 0;
  
  pt1.x = 0;
  pt1.y = 0;
  pt1.z = 0;

  i1 = cre_cir (&pt1, r1, indTab, &iNr);
  if(i1 < 0) {
    TX_Print("too much for 1 surf; divide ..");
    return -1;
  }

  // goto L_fertig;

  lMax = r1 - r2*2.5;    // max Abst vom cen
  lInc = r2 * 4.;

  for(dy=-r1; dy<r1; dy+=lInc) {     // vert.loop
    for(dx=-r1; dx<r1; dx+=lInc) {     // horiz.loop
      pt2.x = dx; pt2.y=dy; pt2.z=0.;
      // Abst vom cen max lMax
      if(UT3D_len_2pt(&pt1,&pt2) > lMax) continue;
      cre_cir (&pt2, r2, indTab, &iNr);
    }
  }

  // generate Surf
  if(mode == UI_FuncUCB1) {
    printf("Flaeche ..\n");
    cre_sur (iNr, indTab);
  } else {
    printf("Zeichnung ..\n");
    cre_dim (iNr, r1, r2);
  }


  L_fertig:
  UTF_add1_line ("# Demo_gui_plate - end");     // add -> Aux.Buffer
  UTF_insert1 (-1L);                            // Add Aux.Buffer -> MainBuffer
  UI_but_END ();                                // rework whole

  TX_Print(" %d Kreise generiert",iNr);

  return 0;

}


//=========================================================
  int cre_sur (int iNr, long *indTab) {
//=========================================================
// generate Surf

  int     i1;
  char    *cBuf;


  cBuf = memspc1;  // use a 50K-general purp. memblock

  // strcpy(cBuf, "FSUB");
  cBuf[0]='\0';

  for(i1=0; i1<iNr; ++i1) AP_obj_add_obj (cBuf, Typ_CI, indTab[i1]);
  // printf("cre_sur |%s|\n",cBuf);

  cre_obj (Typ_SUR, Typ_Txt, strlen(cBuf)+1, (void*)cBuf);

  return 0;

}


//=========================================================
  int cre_dim (int iNr, double r1, double r2) {
//=========================================================
// generate Surf

  int     i1;
  char    *cBuf, auxBuf[256];
  Point   pt1;


  cBuf = memspc1;  // use a 50K-general purp. memblock
  
  strcpy(cBuf, "DIM");


  pt1.x = -r1; pt1.y= 0; pt1.z = 0.;
  AP_obj_add_pt (cBuf, &pt1);

  pt1.x = r1; pt1.y= 0.; pt1.z = 0.;
  AP_obj_add_pt (cBuf, &pt1);

  pt1.x = 0.; pt1.y= -r1 - 12.; pt1.z = 0.;
  AP_obj_add_pt (cBuf, &pt1);

  AP_obj_add_func1 (cBuf, 'A', 0.);

  cre_obj (Typ_GTXT, Typ_Txt, strlen(cBuf)+1, (void*)cBuf);


  //----------------
  cBuf[0] = '\0';
  pt1.x = 0.; pt1.y= -r1 - 16.; pt1.z = 0.;
  AP_obj_add_pt (cBuf, &pt1);     // Textposi
  AP_obj_add_val (cBuf, 1.);      // Textsize
  sprintf(auxBuf, " \"Dmr Aussen = %f\"",r1*2.);
  strcat(cBuf, auxBuf);           // Text
  cre_obj (Typ_GTXT, Typ_Txt, strlen(cBuf)+1, (void*)cBuf);


  cBuf[0] = '\0';
  pt1.x = 0.; pt1.y= -r1 - 20.; pt1.z = 0.;
  AP_obj_add_pt (cBuf, &pt1);     // Textposi
  AP_obj_add_val (cBuf, 1.);      // Textsize
  sprintf(auxBuf, " \"Dmr Bohrg. = %f\"",r2*2.);
  strcat(cBuf, auxBuf);           // Text
  cre_obj (Typ_GTXT, Typ_Txt, strlen(cBuf)+1, (void*)cBuf);


  cBuf[0] = '\0';
  pt1.x = 0.; pt1.y= -r1 - 24.; pt1.z = 0.;
  AP_obj_add_pt (cBuf, &pt1);     // Textposi
  AP_obj_add_val (cBuf, 1.);      // Textsize
  sprintf(auxBuf, " \"Anzahl B.  = %d\"",iNr-1);
  strcat(cBuf, auxBuf);           // Text
  cre_obj (Typ_GTXT, Typ_Txt, strlen(cBuf)+1, (void*)cBuf);


  return 0;

}



//=========================================================
  int cre_cir (Point *pc, double rd1, long *objInd, int *iNr) {
//=========================================================

  int     typ;
  Circ    ci1;


  // printf("cre_cir %d\n",*iNr);


  // Circ from center, axis, radius
  // UT3D_ci_ptvcr (&ci1, pc, &UT3D_VECTOR_Z, rd1);


  ci1.pc = *pc;   // centerpt
  ci1.p1 = *pc;
  ci1.p1.x += rd1;
  ci1.p2 = ci1.p1;
  ci1.vz = UT3D_VECTOR_Z;    // 0 0 1
  ci1.rad = rd1;           // pos = CCW; neg = CW
  ci1.ango = RAD_360;


  cre_obj (Typ_CI, Typ_CI, 1, (void*)&ci1);

  // get and save index of last object
  AP_obj_2_txt_query (&typ, &objInd[*iNr]);

  if(*iNr < maxNr-1) ++(*iNr);
  else return -1;

  return 0;

}


//=========================================================
  int cre_obj (int typ, int form, int siz, void *obj) {
//=========================================================

  int     irc;
  long    bufSiz;
  ObjGX   ox1;
  char    *cBuf;


  cBuf = memspc2;  // use a 50K-general purp. memblock
  bufSiz = sizeof(memspc2);
  cBuf[0] = '\0';


  // printf("cre_obj typ=%d form=%d siz=%d\n",typ,form,siz);


  ox1.typ  = typ;     // die Bedeutung der Daten im Feld <data>
  ox1.form = form;    // die Struktur der Daten im Feld <data>
  ox1.siz  = siz;     // Anzahl der Daten; wenn > 1: ein Array im Feld <data>
  ox1.data = obj;     //


  // change Object -> Text, save
  irc = AP_obj_2_txt (cBuf, bufSiz, &ox1, -1L);
  // if(irc < 0) return irc;

  // printf("cBuf=|%s|\n",cBuf);

  // // add line
  // UTF_add1_line (cBuf);


  return irc;

}


//======================== EOF ======================
