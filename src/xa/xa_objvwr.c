// objvwr.c   
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
2009-11-20 neu erstellt. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void OVR(){}
#endif
/*!
\file  ../xa/xa_objvwr.c
\brief ObjectViewer  (function analyze) 
\code
=====================================================
List_functions_start:

OVR_add__      add obj into ObjViewerWindow
OVR__          create ObjViewerWindow & populate
OVR_Win__      create ObjViewerWindow
OVR_add_chd    add childs to ObjViewerWindow

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_txTab.h"            // TxtTab
#include "../ut/ut_memTab.h"           // MemTab
#include "../ut/ut_cast.h"             // INT_PTR

#include "../gui/gui__.h"              // Gtk3

#include "../ut/func_types.h"               // Typ_Att_hili
#include "../db/ut_DB.h"               // DB_


#include "../xa/xa_brw.h"              // ICO_PT,




// LOCAL:
// static GIO_WinTree winOvr;
static MemObj winOvr;


#define tmpSiz 10000       // size in chars for strings



  int OVR_CB_key (void *parent, void *event);




//================================================================
  int OVR_add_chd (TxtTab *sTab, TreeNode *ndPar, int iPar) {
//================================================================
/// search & add next child to ndPar
// add all childs of (ParentRecordNr==iPar) to ndPar


  int          i1, ip, ic;
  char         *p1;
  // GtkTreeIter  ndChd;
  TreeNode     ndChd;


  // printf("OVR_add_chd %d\n",iPar);


  for(i1=0; i1<sTab->iNr; ++i1) {
    p1 = UtxTab__(i1, sTab);
    sscanf(p1, "%4d", &ip);          // get ParentRecordNr from first 4 chars
      // printf(" _chd[%d] ip=%d |%s|\n",i1,ip,p1);
    if(ip != iPar) continue;

    sscanf(&p1[4], "%2d", &ic);      // get iconNr from chars 4-5
      // printf(" [%d]= %d %d |%s|\n",i1,ip,ic,p1);
    // GUI_TreeIT_row_add (&ndChd, winOvr, ndPar, ic, &p1[6], 1);
    GUI_tree1_row_add (&ndChd, &winOvr, ndPar, ic, &p1[6], 1);

    // recurse !
    OVR_add_chd (sTab, &ndChd, i1);
  }


  return 0;

}


//===============================================================
  int OVR__ (int typ, void *data, int oNr, char *txt) {
//===============================================================
// create & populate objViewer
//   txt    obj.description (should be objName)


  char         *s1;
  UtxTab_NEW   (txTab1);


  // printf("OVR__ typ=%d txt=|%s|\n",typ,txt);
  if(oNr == 1)
    DEB_dump_obj__ (typ, data, txt);



  // create window
  OVR_Win__ (NULL, GUI_SETDAT_EI(TYP_EventPress,UI_FuncInit));


  // get space for strings
  s1 = (char*) MEM_alloc_tmp (tmpSiz);
  UtxTab_init_spc (&txTab1, s1, tmpSiz);

  
  // create strings of infos about struct in txTab1
  DEB_dump_ntxt (&txTab1, typ, data, oNr, txt, -1, 0);
    // UtxTab_dump (&txTab1);


  // add all childs
  OVR_add_chd (&txTab1, NULL, -1);

  // GUI_Tree_expand_all (winOvr);
  GUI_tree1_expand__ (&winOvr, NULL);

  

    // printf("ex OVR__ \n");

  return 0;

}


//===============================================================
  int OVR_Win__ (MemObj *mo, void **data) {
//===============================================================
// objViewer
// see ../gtk/tst_tree_it.c

  static MemObj win0=GUI_OBJ_NEW;

  int       i1, mode;
  MemObj    box0, box1, wTmp;


  i1 = GUI_DATA_I1;

  // printf("OVR_Win__ %d\n",i1);


  switch (i1) {

    //---------------------------------------------------------
    case UI_FuncInit:
      if(GUI_OBJ_IS_VALID(&win0)) return 0;
      // if(win0 != NULL) {           // Win schon vorhanden ?
        // // gtk_widget_destroy (win0);
        // gdk_window_raise (((GtkWidget*)win0)->window);
        // return 1;
      // }

      // win0 = GUI_Window ("ObjectViewer", OVR_Win__, (void*)UI_FuncKill, 0);
      win0 = GUI_Win__ ("ObjectViewer", OVR_Win__, "-500,-500");

      box0 = GUI_box_v (&win0, "");

      // GUI_Butt (box0, "Clear", OVR_Win__, (void*)UI_FuncUCB1, -1);
      GUI_button__ (&box0, "Clear", OVR_Win__, &GUI_FuncUCB1, "");

      // box1 = GUI_box_h (&box0, "");

      // init tree
      // winOvr = GUI_TreeIT__ (box1, NULL, -300, -600);
      winOvr = GUI_tree1__ (&box0, NULL, "e,e");


      //========================================================
      // Display MainWin und enter the GTK main processing loop
      // gtk_widget_set_usize (win0, 500, 500);
      // gtk_signal_connect (GTK_OBJECT(win0),"key_press_event",  // GTK2
                           // GTK_SIGNAL_FUNC(OVR_CB_key), NULL);
      // gtk_widget_show (win0);
      GUI_Win_go (&win0);
      break;


    //---------------------------------------------------------
    case UI_FuncUCB1:          // clear
      GUI_tree1_clear (&winOvr);
      break;


    //---------------------------------------------------------
    case UI_FuncExit:  // 102
      win0 = GUI_OBJ_INVALID();
      // send destroy-signal
      // if(win0) gtk_signal_emit_by_name (GTK_OBJECT(win0),"destroy");
      // return 0;


    //---------------------------------------------------------
    case UI_FuncKill:
      // printf("OVR_Win__ exit\n");
      break;
  }

  return 0;

}

/*
//================================================================
  int OVR_CB_key (void *parent, void *event) {
//================================================================
// callback from keys in SurfacColorWindow

  switch (((GdkEventKey*)event)->keyval) {

    case 'q':
    case 's':
    case GDK_Escape:
      // OVR_Win__ (NULL, (void*)UI_FuncExit);  CRASH ! 2011-08-10
      OVR_Win__ (NULL, (void*)UI_FuncKill);
      break;
  }

  return TRUE;  // prevent the default handler from being run

}
*/

//================================================================
  int OVR_add__ (int typ, long dbi) {
//================================================================
// inspect obj

  int       irc, oNr;
  char      oNam[32];
  void      *vp1;


  printf("OVR_add__ %d %ld\n",typ,dbi);



  // dli = DL_dli__dbo (typ, dbi, -1L);
  APED_oid_dbo__ (oNam, typ, dbi);
    // printf(" oNam=|%s|\n",oNam);


  // get objStruct from dbo
  // irc = UTO_get_dbo (&typ, &vp1, &oNr, typ, dbi);
  irc = UTO_objDat_dbo (&vp1, &oNr, &typ, dbi);
  if(irc < 0) return -1;


  // objViewer
  OVR__ (typ, vp1, oNr, oNam);


  // _DEB_dump_obj__ (typ, vp1, oNam);

  return 0;

}


// EOF
