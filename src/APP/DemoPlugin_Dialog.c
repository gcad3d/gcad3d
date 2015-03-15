// example dialog.
/*
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
\file  ../APP/DemoPlugin_Dialog.c
\brief example dialog 
\code
=====================================================
List_functions_start:
..
List_functions_end:
=====================================================

\endcode *//*----------------------------------------


make -f DemoPlugin_Dialog.mak

*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
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

#include "../gr/ut_UI.h"               // UI_Func...
#include "../gr/ut_DL.h"               // DB_GetDLatt
#include "../gr/ut_GL.h"               // GL_GetCurpos0
#include "../db/ut_DB.h"               // DB_GetObjGX
#include "../xa/xa_ui.h"               // UI_men__..


#define   TRUE 1
#define   FALSE 0

#define CBUFSIZ 1024



  static Point   pt1, pt_tab[5];        // pointbuffer
  static Line    ln1;
  static Vector  vc_tab[3];
  static int     dia_nr;                 // nr of points
  static char    cBuf[CBUFSIZ];


  static MemObj  win0;
  static MemObj  dia_ideg, dia_lb_func;

  static long    ID_tab[3];
  static long    dl_start;



// prototypes:
  int dia_CB_1 (MemObj *mo, void **data);
  int dia_cre_Poly (void *parent, void *data);









//=========================================================
  int dia_sele2 (int src, long dl_ind) {
//=========================================================
// userselection-callback  turn-arrow
// (user selects arrow)

  int    i1, irc, ideg;
  long   l1;
  ObjGX  ox1;
  DL_Att att1;



  printf("dia_sele2 source=%d ind=%ld nr=%d\n",src,dl_ind,dia_nr);


  if(src != GUI_MouseL) goto L_err_1; // skip all but left mousebutt.

  // get DL-record
  att1 = DL_GetAtt(dl_ind);

  if(att1.typ != Typ_SymV) goto L_err_1;     // skip all but symbols


  for(i1=0; i1<3; ++i1) {
    if(ID_tab[i1] == dl_ind) {         // dynam. symbols report DL-Ind.
      printf(" arr %d sel\n",i1);
      goto L_1;
    }
  }
  goto L_err_1;



  //-----------------------------------------------------
  L_1:
  // delete selected obj
  GL_Del0 (ID_tab[i1]);

  // delete displist from dl_start -> end (points, line from last run)
  if(dl_start != 0) GL_Delete (dl_start+1);

  // redraw inverted vector
  UT3D_vc_invert (&vc_tab[i1], &vc_tab[i1]);   // invert vector
  APT_disp_SymV3 (SYM_ARROW, 0, &pt_tab[i1], &vc_tab[i1], 10.);
  ID_tab[i1] = GL_GetActInd();

  // save DL-posi (to delete all foll. objs)
  dl_start = GL_GetActInd(); // keep last obj

  // 2 point & 1 line
  UT3D_pt_traptvclen (&ln1.p1, &pt_tab[i1], &vc_tab[i1], -20.);
  l1 = -1;  // do not store obj in DB
  GR_CrePoint (&l1, 0, &ln1.p1);
  UT3D_pt_traptvclen (&ln1.p2, &pt_tab[i1], &vc_tab[i1], -100.);
  l1 = -1;  // do not store obj in DB
  GR_CrePoint (&l1, 0, &ln1.p2);
  l1 = -1;  // do not store obj in DB
  GR_CreLine (&l1, 1, &ln1);

  DL_Redraw (); // Redraw DispList

  return 0;



  //-----------------------------------------------------
  L_err_1:
  TX_Print (" select arrow !!");
  return 0;

}


//=========================================================
  int dia_sele1 (int src, long dl_ind) {
//=========================================================
// userselection-callback points->curve
// (user selects or indicates points)

  int      irc, ideg;
  long     l1, dbi;
  char     *p1;
  DL_Att   att1;


  printf("dia_sele1 source=%d ind=%ld nr=%d\n",src,dl_ind,dia_nr);



  // skip right & middle-mousebutton
  if(src != GUI_MouseL) {
    printf("right mousebutton not used\n");
    return 0;
  }


  // indicate position
  if(dl_ind < 0) {
    pt1 = GL_GetViewPos ();      // get cursorposition


  // select Point
  } else {
    // get DL-record
    att1 = DL_GetAtt(dl_ind);

    if(att1.typ == Typ_PT) {
      pt1 = DB_GetPoint (att1.ind);
      // UT3D_stru_dump (Typ_PT, &pt1, "sel.P%d",att1.ind);

    } else {
      TX_Error (" select point or indicate position");
      return 0;
    }
  }


  if(dia_nr == 0) dia_cre_init ();


  printf("Pos=%f,%f,%f\n",pt1.x,pt1.y,pt1.z);
  pt_tab[dia_nr] = pt1;


  // display small triangle
  APT_disp_SymB (SYM_TRI_S,  0, &pt1);  // cr dynamic symbol


  // done for first 4 points
  if(dia_nr < 4) {
    ++dia_nr;
    DL_Redraw (); // Redraw DispList
    return 0;
  }


  // // dim follow objects (and do not export these obj)
  // ED_add_Line ("DIM");

  // create polygon
  irc = dia_cre_obj (Typ_CVPOL, Typ_PT, 5, (void*)&pt_tab);
  if(irc < 0) {
    TX_Print("_Dialag Error 001");
    return -1;
  }


  // get degree
  // p1 = gtk_entry_get_text (GTK_ENTRY(dia_ideg));
  p1 = GUI_entry_get (&dia_ideg);
  ideg = atoi(p1);

  // // reset dim (graf att 1 = default)
  // ED_add_Line ("G1");

  // polygon -> B-spline
  sprintf(cBuf,"S%d=BSP,S%d,%d",irc+1,irc,ideg);
  UTF_add1_line  (cBuf);

  // add objects from buffer -> CAD & update display
  dia_cre_update ();


  // reset
  dia_nr = 0;


  return 0;


}


//=========================================================
  int dia_KeyIn (int key) {
//=========================================================
// userKeyin callback;    exit on 'q' or Esc-key.


  printf("dia_KeyIn %d\n",key);

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
  int gCad_fini () {
//=========================================================
// dll being unloaded - reset Input, kill all open windows !!!

  // kill window
  GUI_Win_kill (&win0);

  AP_User_reset ();      // close application

  return 0;

}


//=========================================================
  int gCad_main () {
//=========================================================
// init userfunction


  TX_Print("--->>>>>> gCad_main aus DemoPlugin_Dialog <<<<<<<---\n");
  TX_Print("select Menu;  Esc = reset, q = exit");

  // attach KeyIn - connect KeyIn -> func dia_KeyIn
  AP_UserKeyIn_get (dia_KeyIn);

  // init diawin (create and display userpanel)
  dia_win_main (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));

  // init object-create
  dia_cre_init ();

  return 0;
}





//=========================================================
  int dia_win_main (MemObj *mo, void **data) {
//=========================================================
// UI_FuncInit:  create and display userpanel
// UI_FuncKill:  kill userpanel


  static int  icb=UI_FuncExit;

  int         i1;
  MemObj      box0, wact;



  i1 = GUI_DATA_I1;
  printf("dia_win_main %d\n",i1);


  switch (i1) {

    //---------------------------------------------------------
    case UI_FuncInit:
      printf("dia init\n");

      win0 = GUI_Win__ ("Demodialog",dia_win_main,"");

      box0 = GUI_box_v (&win0, "");    // create vertical box

      // textlabel
      GUI_label__ (&box0, "active function is:", "l");
      // textlabel
      dia_lb_func = GUI_label__ (&box0, "points->curve ", "l");


      // create buttons (parentwidget, textlabel, callback, CB-data, border)
      // box0 = GUI_Hbox (box0, 0);
      GUI_button__ (&box0, " points->curve ", dia_CB_1, (void*)"p2c", "");
      dia_ideg = GUI_entry__(&box0,  "degree","2", NULL, NULL, "");
      GUI_button__ (&box0, " turn vec ", dia_CB_1, (void*)"turn", "");
      GUI_button__ (&box0, " polygon ", dia_cre_Poly, NULL, "a,a");
      GUI_button__ (&box0, " Export ", dia_CB_1, (void*)"Export", "");
      GUI_button__ (&box0, " Clear ",  dia_CB_1, (void*)"Clear", "");
      GUI_button__ (&box0, " Exit ",   dia_win_main, (void*)&GUI_FuncExit, "");


      // simulate buttonpress = init function points->curve
      // dia_CB_1 (NULL, (void*)"p2c");

      GUI_Win_up (NULL, &win0, 0);  // always on top
      GUI_Win_go (&win0);

      // inactivate box degree
      GUI_set_enable (&dia_ideg, FALSE);

      break;





    //---------------------------------------------------------
    case UI_FuncExit:
      gCad_fini ();

  }

  return 0;

}


//=========================================================
  int dia_cre_Poly (void *parent, void *data) {
//=========================================================
// create polygon


  int    i1, i2;
static  Point  pta[32];
static  Plane  pln;

  printf("Cre_Poly\n");

  // clean undo-tables
  UNDO_clear ();


  // polygonpoints
  for(i1=0; i1<32; ++i1) {
    pta[i1].x = i1 - 16.;
    pta[i1].y = (pta[i1].x * pta[i1].x)  *  0.1;
    pta[i1].z = i1 - 16;
    // printf(" p%d = %f,%f,%f\n",i1,pta[i1].x,pta[i1].y,pta[i1].z);
  }

  // create
  dia_cre_obj (Typ_CVPOL, Typ_PT, 32, (void*)&pta);

  // update display
  dia_cre_update ();

  TX_Print (" curve created ..");

  return 0;

}


//=========================================================
  int dia_CB_1 (MemObj *mo, void **data) {
//=========================================================
// dialog-callbacks

  char      *cp1;


  cp1 = GUI_DATA_S1;
  printf("dia_CB_1 |%s|\n",cp1);


  //-----------------------------------------------------
  if(!strcmp(cp1, "Clear")) {

    // clear model
    UI_men__ ("new");



  //-----------------------------------------------------
  } else if(!strcmp(cp1, "Export")) {

    sprintf(cBuf, "%sdialog.ige",OS_get_tmp_dir());
    AP_ExportIges__ (cBuf);



  //-----------------------------------------------------
  } else if(!strcmp(cp1, "p2c")) {

    // replace info
    GUI_label_mod (&dia_lb_func, "points -> curve");

    // reactivate box degree
    GUI_set_enable (&dia_ideg, TRUE);

    // imply END-Button (Redraw)
    UI_but_END ();

    // attach grafic selections
    AP_UserSelection_get (dia_sele1);

    dia_nr = 0;                 // nr of points

    TX_Print(" --- select/indicate  5 points");




  //-----------------------------------------------------
  } else if(!strcmp(cp1, "turn")) {

    // replace info
    GUI_label_mod (&dia_lb_func, "turn arrows");

    // inactivate box degree
    GUI_set_enable (&dia_ideg, FALSE);


    // create 3 pt's & 1 ln
    // use only once (fixed objNam)
    ED_add_Line ("P10=P(100,0)");
    ED_add_Line ("P11=P(120,0)");
    ED_add_Line ("P12=P(140,0)");
    ED_add_Line ("L10=P10,P12");


    // display vectors temporary (not stored in DB)
    UT3D_pt_3db (&pt_tab[0], 100., 0., 0.);
    UT3D_pt_3db (&pt_tab[1], 120., 0., 0.);
    UT3D_pt_3db (&pt_tab[2], 140., 0., 0.);

    UT3D_vc_3db (&vc_tab[0], 0., 1., 1.);
    UT3D_vc_3db (&vc_tab[1], 0., 1., 1.);
    UT3D_vc_3db (&vc_tab[2], 0., 1., 1.);

    APT_disp_SymV3 (SYM_ARROW, 9, &pt_tab[0], &vc_tab[0], 10.);
    ID_tab[0] = GL_GetActInd();  // save dispListIndex
    APT_disp_SymV3 (SYM_ARROW, 9, &pt_tab[1], &vc_tab[1], 10.);
    ID_tab[1] = GL_GetActInd();
    APT_disp_SymV3 (SYM_ARROW, 9, &pt_tab[2], &vc_tab[2], 10.);
    ID_tab[2] = GL_GetActInd();
    printf(" arrID's=%ld,%ld,%ld\n",ID_tab[0],ID_tab[1],ID_tab[2]);

    // Redraw DispList
    DL_Redraw ();
    TX_Print(" --- select arrow to invert");

    // attach grafic selections -> func dia_sele2
    AP_UserSelection_get (dia_sele2);

    dl_start = 0;

  //-----------------------------------------------------
  }

  dia_nr = 0;

  return 0;

}


//=========================================================
  int dia_cre_init () {
//=========================================================

  printf("dia_cre_init \n");


  // clear auxBuffer UTF_FilBuf1
  UTF_clear1 ();

  // Init objNrs
  AP_obj_2_txt (NULL, 0L, NULL, 0L);

  return 0;

}


//=========================================================
  int dia_cre_obj (int typ, int form, int siz, void *obj) {
//=========================================================
// create objCode and add code to auxBuffer

  int    irc;
  long   ind;
  ObjGX  ox1;


  printf("dia_cre_obj typ=%d form=%d siz=%d\n",typ,form,siz);
  // printf(" CBUFSIZ=%ld\n",CBUFSIZ);



  // prepare data
  ox1.typ  = typ;     // die Bedeutung der Daten im Feld <data>
  ox1.form = form;    // die Struktur der Daten im Feld <data>
  ox1.siz  = siz;     // Anzahl der Daten; wenn > 1: ein Array im Feld <data>
  ox1.data = obj;     //


  // change Object -> Text, add obj to auxBuffer
  irc = AP_obj_2_txt (cBuf, CBUFSIZ, &ox1, -1L);
  if(irc < 0) return irc;

  // query typ and index of created object
  AP_obj_2_txt_query (&typ, &ind);
    printf(" ex _query: typ=%d dbi=%ld\n",typ,ind);


  return ind;

}


//=========================================================
  int dia_cre_update () {
//=========================================================


  printf("dia_cre_update \n");


  // output auxBuffer UTF_FilBuf1 -> CAD
  UTF_insert1 (-1L);

  // update display (work new created objects)
  APED_update__ (-1L);

  return 0;

}


//======================== EOF ======================
