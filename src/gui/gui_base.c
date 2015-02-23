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
improve opts for GUI_box__ GUI_entry__ ..
noFrame,hSiz15,hSizE,vSiz..,left, ..
hSizE   horiz. expandable, default = do not expand (size fixed) ..
delimitercharacter blank or comma

-----------------------------------------------------
Modifications:
2012-09-04 First production version; RF.
2012-04-13 First version; extracted from Gtk-toolbox. RF.

-----------------------------------------------------
*/
#ifdef globTag
void GUI(){}
#endif
/*!
\file ../gui/gui_base.c
\brief GraficalUserInterface internal functions, common for gtk2,gtk3
\code

#include "../gui/gui__.h"              // includefile for all GUI-functions

------ Links to Gtk ---------------
../gui_gtk3/gtk_base.c              Window Box
../gui_gtk3/gtk_label.c             Label
../gui_gtk3/gtk_button.c            Button (with image) Checkbox
../gui_gtk3/gtk_menu.c              Menu
../gui_gtk3/gtk_entry.c             Entry
../gui_gtk3/gtk_image.c             Image ScreenCast
../gui_gtk3/gtk_dlg_std.c           Messagebox Dialogbox Slider Colorselect
../gui_gtk3/gtk_dlg_files.c         File open / save
../gui_gtk3/gtk_msgWin.c            Messagewindow
../gui_gtk3/gtk_lists.c             List
../gui_gtk3/gtk_container.c         Frame Toolbar Notebook Paned
../gui_gtk3/gtk_multichoice.c       Radiobuttons Popup-menu Option-menu
../gui_gtk3/gtk_tree_it.c           Treeview with icon + text
../gui_gtk3/gtk_ed1.c               TextEditor
../gui_gtk3/gtk_opengl.c            OpenGL-window
../gui_gtk3/gtk_gl_x11.c            Gtk-binding OpenGL

 
------ macros for callback-data ---------
GUI_DATA_EVENT           *(int*)data[0]        callbackEventType
GUI_DATA_I1              *(int*)data[1]
GUI_DATA_D1              *(double*)data[1]
GUI_DATA_S1              (char*)data[1]
GUI_DATA_PTR_I1          (int*)(data[1])       a pointer to integer 1

GUI_OBJ_TYP(*mo)         returns widgetType  
GUI_OBJ_SRC(*mo)         returns gui-widget-nr
GUI_obj_cmp(*mo,*mo)     compare 2 MemObj's

GUI_OBJ_IS_VALID         test if GUI-object is valid; 1 is OK; 0 is invalid obj.
GUI_OBJ_INVALID          set MemObj invalid
GUI_OBJ_NEW              initialize new MemObj
GUI_SETDAT_E()           create GUI_DATA-block  ../gui_gtk/gtk_base.c  
GUI_SETDAT_EI()          create GUI_DATA-block  ../gui_gtk/gtk_base.c  
GUI_SETDAT_ES()          create GUI_DATA-block  ../gui_gtk/gtk_base.c  

------ constants ---------
../gui/gui_types.h   events, devices, keys, funcs, data-macros
../gui/gui_base.h    objects; from ../gui/gui__.h


=====================================================
List_functions_start:

GUI_obj_init             get a new memBlockId <UI_umbId>
GUI_obj_save             save UI_tmpSpc -> memoryblock <UI_umbId>
GUI_obj_clear            clear the local memSpc UI_tmpSpc
GUI_obj_spc              get address & memspc for new widget (in UI_tmpSpc)
GUI_obj_tmp              get temporary object; only one possible.
GUI_obj_pos              get widget-data-position from MemObj
GUI_obj_src              get widget-nr from MemObj
GUI_obj_cmp              compare 2 MemObj's                         INLINE
GUI_obj_free             free the memBlock

GUI_obj_parentBox        get widget-type & data data from MemObj; check for Box.
GUI_obj_typ              get widget-type & widget-data from MemObj
GUI_obj__                get temporary block UI_tmpSpc
GUI_obj_destroy          free memBlock from MemObj
GUI_obj_dump_mo          dump MemObj
GUI_obj_dump_o

GUI_opts_get1            get 1-char-parameter from opts-string

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

UI_tmpSpc is a permanent active memoryspace.
All widget-objects for the active window are stored in UI_tmpSpc.
If a new window is started before the active window is destroyed, then
  UI_tmpSpc is saved in a memoryblock (by malloc new space, copy to new space).
If a saved window gets destroyed, its memoryblock is freed.


UI_umbId       memBlockNr; index into umbTab

UI_srcId       nr of records in memBlock; is stored in umbTab[].recNr

umbId          memBlockName; is stored in umbTab[].umbId

see ../ut/ut_umb.c



=====================================================
compile mit gcc:
cc -c -O0 -g3 -Wall gui_base.c

Windows:
cl -c gui_base.c




test offline:
cc gui_base.c ut_umb.c ut_umem.c ut_mainut.c -DOFFLINE_GUI1 -lm&&./a.out
ctags -f gui_base.tag gui_base.c

*/


#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>                     // memcpy

#include "../ut/ut_umem.h"              // Memspc
#include "../ut/ut_umb.h"               // UMB
#include "../gui/gui_types.h"       // TYP_GUI_Win

// #define extern          // damit wird "extern" im Includefile geloescht !
#include "../gui/gui_base.h"
// #undef extern           // reset extern ..

// ../ut/ut_geo.h
#define Typ_Error          0  ///<     U0 = undefined


#define TMPSPC_SIZ   15000
#define TMPSPC_INC   15000


//----------------------------------------------------------------
// was ../gtk/ut_gtk.c: 
int       UI_fontsizX, UI_fontsizY;
// was ../xa/xa_ui.c
void      *UI_MainWin;
void      *UI_act_wi;


//----------------------------------------------------------------
// local:
static Memspc UI_tmpSpc=UME_NEW;
static void   *UI_tmpPos = NULL;
static int    UI_umbId;                // userMemoryBlockNr;
static int    UI_srcId;                // widget-Nr;




// defines are in ../gui/gui_types.h
// these are extern in ../gui/gui__.h
int GUI_FuncInit = UI_FuncInit;
int GUI_FuncUCB1 = UI_FuncUCB1;
int GUI_FuncUCB2 = UI_FuncUCB2;
int GUI_FuncUCB3 = UI_FuncUCB3;
int GUI_FuncUCB4 = UI_FuncUCB4;
int GUI_FuncUCB5 = UI_FuncUCB5;
int GUI_FuncUCB6 = UI_FuncUCB6;
int GUI_FuncUCB7 = UI_FuncUCB7;
int GUI_FuncUCB8 = UI_FuncUCB8;
int GUI_FuncUCB9 = UI_FuncUCB9;
int GUI_FuncUCB10 = UI_FuncUCB10;
int GUI_FuncUCB11 = UI_FuncUCB11;
int GUI_FuncUCB12 = UI_FuncUCB12;
int GUI_FuncUCB13 = UI_FuncUCB13;
int GUI_FuncWork = UI_FuncWork;
int GUI_FuncExit = UI_FuncExit;
int GUI_FuncKill = UI_FuncKill;





// protos:
char*   GUI_Win_tit     (void *gtkWin);




//================================================================
  Memspc* GUI_obj__ () {
//================================================================
/// GUI_obj__              get temporary block UI_tmpSpc

  return (&UI_tmpSpc);

}


//================================================================
  void GUI_obj_destroy (void *wo) {
//================================================================
/// free memBlock from MemObj

  MemObj  *mo;


  mo = &((Obj_Unknown*)wo)->mem_obj;

  // printf("GUI_obj_destroy %d\n",mo->mbID);


  // is winMem == tmpSpc (local):
  if(mo->mbID == UI_umbId) {
    // yes: clear temSpc
    GUI_obj_clear ();
  }

  GUI_obj_free (mo);   // free memBlock / set memBlock-stat = 0

}


//================================================================
  void* GUI_obj_parent__ (int *pTyp, MemObj *mObj) {
//================================================================
/// get widget-type & data from MemObj; check for Box.
/// Input:
///   mObj          parentObj
/// Output:
///   pTyp          type of parent; eg TYP_GUI_BoxH
///   RetCod        the gtk-widget of the parent


  void  *g_par, *w_par;


  // get widget-type & widget-data from MemObj
  GUI_obj_typ (pTyp, (void**)&g_par, mObj);
  if(!pTyp) return (NULL);


  if((*pTyp == TYP_GUI_BoxH) ||
     (*pTyp == TYP_GUI_BoxV) ||
     (*pTyp == TYP_GUI_Win))    {
    w_par = ((Obj_Box*)g_par)->box;
      // printf(" w_par = %p\n",w_par);

  } else {
    printf("***** GUI_obj_parent__ parent must be box, not %d\n",*pTyp);
    return NULL;
  }

  return (w_par);

}


//================================================================
  void* GUI_obj_parentBox (void *o_par) {
//================================================================
/// get widget-type & data from MemObj; check for Box.


  int   pTyp;
  void  *g_par, *w_par;


  // get widget-type & widget-data from MemObj
  GUI_obj_typ (&pTyp, (void**)&g_par, o_par);
  if(!pTyp) return (NULL);


  if((pTyp == TYP_GUI_BoxH) ||
     (pTyp == TYP_GUI_BoxV) ||
     (pTyp == TYP_GUI_Win))    {
    w_par = ((Obj_Box*)g_par)->box;
      // printf(" w_par = %p\n",w_par);

  } else {
    printf("***** GUI_obj_parentBox parent must be box, not %d\n",pTyp);
    return NULL;
  }

  return (w_par);

}


//================================================================
  void GUI_obj_typ (int *pTyp, void **g_par, MemObj *o_par) {
//================================================================
/// get widget-type & widget-data from MemObj


  // get widget-data from MemObj
  *g_par = GUI_obj_pos (o_par);
  if(!g_par) {
    *pTyp = Typ_Error;
    printf("***** GUI_obj_typ E001\n");
    return;
  }

  // get widget-type
  *pTyp = ((Obj_Unknown*)*g_par)->gio_typ;

  // printf("ex GUI_obj_typ %d\n",*pTyp);


}


//================================================================
  int GUI_obj_reload (MemObj *o_par) {
//================================================================
/// \code
/// reload the block <o_par->mbID> into UI_tmpSpc
/// init local memSpc UI_tmpSpc
/// \endcode


  int   irc=0, mbId;
  void  *vpn, *oPos;


  // printf("GUI_obj_reload UI_umbId=%d mbID=%d\n",UI_umbId,o_par->mbID);

  // do nothing if window is already active
  if(o_par->mbID == UI_umbId) return 0;



  // test if UI_tmpSpc ( = local space for widgets is empty;
  //   eg after delete last window ..
  if(UME_get_start(&UI_tmpSpc) == UME_get_next(&UI_tmpSpc)) {
      // printf(" winMem is empty\n");
    goto L_reload;
  }
    // printf(" winMem is NOT empty\n");


  // save UI_tmpSpc -> memoryblock <UI_umbId>
  irc = UME_obj_save (UI_umbId, &UI_tmpSpc, UI_srcId);
  if(irc < 0) return irc;


  //----------------------------------------------------------------
  L_reload:

  // reload
  irc = UMB_reload (&UI_srcId,
                    UI_tmpSpc.start, &UI_tmpSpc.next, UI_tmpSpc.end,
                    o_par->mbID);
  if(irc < 0) return -1;

  UI_umbId = o_par->mbID;

  // free old spc
  UMB_free (UI_umbId);


  return UI_umbId;      // returns Id

}


//================================================================
  int GUI_obj_init () {
//================================================================
/// \code
/// get a new memBlockId <UI_umbId>
/// init local memSpc UI_tmpSpc
/// \endcode


  int   irc=0;
  void  *oPos;


  // printf("GUI_obj_init IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\n");

  // printf(" TTTTTTTTTTTTTTTTTTTTTTTTT GUI_FuncExit = %d\n",GUI_FuncExit);



  // get tempSpace ..
  if(!UI_tmpPos) {
    // init local UI_tmpSpc
    UME_malloc (&UI_tmpSpc, TMPSPC_SIZ, TMPSPC_INC);
    UI_tmpPos = UME_get_start (&UI_tmpSpc);
      // printf(" UI_tmpPos = %p\n",UI_tmpPos);
      // printf(" pStart=%p pEnd=%p\n",UI_tmpSpc.start,UI_tmpSpc.end);
    goto L_getNew;
  }


  // test if UI_tmpSpc ( = local space for widgets is empty)
  if(UME_get_start(&UI_tmpSpc) == UME_get_next(&UI_tmpSpc)) {
      // printf(" winMem is empty\n");
    // must increment the blockId for this reUse
    irc = UME_reuse (UI_umbId);
    goto L_exit;
  }

    // printf(" winMem is NOT empty\n");

  // save UI_tmpSpc -> memoryblock <UI_umbId>
  irc = UME_obj_save (UI_umbId, &UI_tmpSpc, UI_srcId);
  if(irc < 0) return irc;

  // clear UI_tmpSpc
  GUI_obj_clear ();


  L_getNew:
  // get a new memBlock-index and Id
  UI_umbId = UMB_reserve__ (&irc);


  L_exit:
    // reset widget-nr
    UI_srcId = 0;

    // first object in memSpc is UI_umbId - a uniq blockNr 
    UME_obj_reserve (&oPos,  &UI_tmpSpc, sizeof(int), UI_umbId);
    memcpy (oPos, &irc, sizeof(int));

      // printf("ex GUI_obj_init ind=%d id=%d\n",UI_umbId,irc);

    return irc;      // returns Id

}


//================================================================
  int GUI_obj_save () {
//================================================================
/// \code
/// save UI_tmpSpc -> memoryblock <UI_umbId>
///   & clear UI_tmpSpc
///   & get new memBlockId
/// \endcode

  int   irc;


  // printf("GUI_obj_save %d\n",UI_umbId);

  // save UI_tmpSpc -> memoryblock <UI_umbId>
  irc = UME_obj_save (UI_umbId, &UI_tmpSpc, UI_srcId);
  if(irc < 0) return irc;

  // clear UI_tmpSpc
  GUI_obj_clear ();

  // get new memBlockId <UI_umbId>
  GUI_obj_init ();

  return 0;

}


//================================================================
  void GUI_obj_clear () {
//================================================================
/// clear the local memSpc UI_tmpSpc

  // printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC \n");
  // printf("GUI_obj_clear \n");


  UME_set_free (-1, &UI_tmpSpc);

}


//================================================================
  MemObj GUI_obj_tmp (void **oPos, int siz) {
//================================================================
/// get temporary object; only one possible.

  MemObj    mo;


  mo.mbID = UI_umbId;
  // mo.ioff = UI_tmpSpc.next - UI_tmpSpc.start;
  mo.ioff = (char*)UI_tmpSpc.next - (char*)UI_tmpSpc.start;

  // if(mo.ioff < 4) {
  if(UME_obj_IS_VALID(&mo) < 0) {
    // 0 = invalid adress !
    // UI_tmpSpc.next = UI_tmpSpc.start + 4;
    UI_tmpSpc.next = (char*)UI_tmpSpc.start + 4;
    mo.ioff = 4;
  }

  *oPos = UI_tmpSpc.next;

  // copy MemObj into reserved space 
  ((Obj_Unknown*)*oPos)->mem_obj = mo;


  return (mo);

}


//================================================================
  MemObj GUI_obj_spc (void **oPos, int siz) {
//================================================================
/// \code
/// get address & memspc for new widget (in UI_tmpSpc)
/// Input:
///   siz        size for new widget
/// Output:
///   *oPos      free space for the new object (in UI_tmpSpc)
///   RetCod     memblockId (reserved) and dataOffset
/// \endcode

  MemObj    mo;


  // printf("GUI_obj_spc siz=%d UI_umbId=%d\n",siz,UI_umbId);

  // get <siz> bytes in UI_tmpSpc and set memBlock-Id & memBlock-offset
  mo = UME_obj_reserve (oPos,  &UI_tmpSpc, siz, UI_umbId);
  // if(mo.mbID < 0) return (mo);
  if(UME_obj_IS_VALID(&mo) < 0) return (mo);    // 2012-07-19

  // copy MemObj into reserved space 
  ((Obj_Unknown*)*oPos)->mem_obj = mo;

  // save gui-sourcename gio_src
  ((Obj_Unknown*)*oPos)->gio_src = UI_srcId;

  // increment gui-sourcename
  ++UI_srcId;

  return (mo);
}


//================================================================
  void GUI_obj_dump__ () {
//================================================================

  int   ioff;
  void  *pos;


  ioff = 0;
  pos  = UI_tmpPos;


  printf("-------------- GUI_obj_dump__ --------- UI_umbId=%d UI_tmpPos=%p\n",
     UI_umbId, UI_tmpPos);

}


//================================================================
  void GUI_obj_dump_mo (MemObj *mo) {
//================================================================
/// dump MemObj

  printf("-------------- GUI_obj_dump_mo -------------- mbID=%d ioff=%d\n",
     mo->mbID, mo->ioff);

}



//================================================================
  void GUI_obj_dump_o (void *go) {
//================================================================
/// dump gui-object

  MemObj  *mo;
  void    *wi;
  char    *cp1;


  if(!go) {
    printf("-------------- GUI_obj_dump_o -------------- INVALID OBJ\n");
    return;
  }

  mo = &((Obj_Unknown*)go)->mem_obj;

  printf("-------------- GUI_obj_dump_o -------------- mbID=%d ioff=%d\n",
      mo->mbID, mo->ioff);
    printf(" typ=%d stat=%d\n",
      ((Obj_Unknown*)go)->gio_typ,
      ((Obj_Unknown*)go)->gio_src);

    // printf(" data=|%s|\n",go->data);
    // printf(" func=|%p|\n",go->uFunc);


  if(((Obj_Unknown*)go)->gio_typ == TYP_GUI_Win) {
      printf(" TYP_GUI_Win:\n");
    wi = ((Obj_Win*)go)->win;
    cp1 = GUI_Win_tit (wi);
      printf("  winTit |%s|\n",cp1);
  }


}


//================================================================
  int GUI_obj_src (MemObj *mo) {
//================================================================
/// GUI_obj_src              get widget-nr from MemObj


  Obj_Unknown *go;

  go = GUI_obj_pos (mo);
  if(!go) return 0;

  return (go->gio_src);

}


//================================================================
  void* GUI_obj_pos (MemObj *mo) {
//================================================================
/// \code
/// get widget-data-position from MemObj
/// MemObj contains window-nr and offset in the window-memoryblock.
/// widget-data contains at least Obj_Unknown
///
/// Example:
/// Obj_Unknown *go;
/// go = GUI_obj_pos (mo);
/// printf(" typ=%d src=%d\n", go->gio_typ, go->gio_src);
/// \endcode


  // printf("GUI_obj_pos UI_umbId=%d\n",UI_umbId);

  if(!mo) {
    printf("***** GUI_obj_pos - MemObj = NULL\n");
    return NULL;
  }

    // printf("  mbID=%d ioff=%d\n", mo->mbID, mo->ioff);

  // if(mo->ioff == 0) {
  if(UME_obj_IS_VALID(mo) < 0) {          // 2012-07-19
    printf("***** GUI_obj_pos - invalid MemObj\n");
    return NULL;
  }

  // get baseAdress of memSpc for Window <winID>
  if(UI_umbId == mo->mbID) {
      // printf(" _obj_pos local\n");
    return ((char*)UI_tmpPos + mo->ioff);

  } else {
      // printf(" _obj_pos extern\n");
    return (UME_obj_get (mo));
  }

}


//================================================================
  char GUI_opts_get1 (char **s1, char *opts) {
//================================================================
/// \code
/// GUI_opts_get1        get 1-char-parameter from opts-string
/// Input:
///   s1       the opts-string; paramters, delimited by ','.
///   opts     the characters of the parameter
/// Output:
///   s1       the next parameter (delimiter ',' is skipped)
///   RetCod   the paramter (one character of opts)
///            or opts[0] (the first character is the default).
/// \endcode


  char  *p1, *p2;


  p1 = *s1;

  p2 = strpbrk (p1, opts);
  // if(!p2) return (opts[0]);
  if(!p2) return ('\0');

  p1 = p2 + 1;

  if(*p1 == ',') ++p1;   // skip delimiter

  *s1 = p1;

  return (*p2);

}


//================================================================
  int GUI_obj_free (MemObj *mo) {
//================================================================
/// free the memBlock

  // printf("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF\n");
  // printf("GUI_obj_free %d\n",mo->mbID);

  return UMB_free (mo->mbID);

}




//========================================================
//========================================================
#ifdef OFFLINE_GUI1


typedef struct {double d1, d2; } myObj1;


//============================
  int main (int paranz, char *params[]) {
//============================

  int     mbId1, mbId2, ii1;
  myObj1  *o1, *o2;
  char    s1[256], *p1;
  Memspc  mSpc;
  MemObj  mo1, mo2;
  long    mSiz;
  void    *mPos;



  printf("OFFLINE ut_umb \n");


  // get memBlockId
  GUI_obj_init ();
  UMB_dump__ ();


  // get space for obj in memblock
  mo1 = GUI_obj_spc ((void*)&o1, sizeof(myObj1));


  // fill obj
  o1->d1 = 12.34;
  o1->d2 = 56.78;


  // get obj (yet out of tempSpc)
  o2 = GUI_obj_pos (&mo1);
  if(!o2) return 0;
  printf(" o2.d1=%lf\n",o2->d1);



  // get another memBlockId
  mbId2 = UMB_reserve__ (&ii1);
  UMB_dump__ ();


  // save tempSpace persistant -> memBlock & clear the tempSpace
  // OR GUI_obj_clear
  GUI_obj_save ();
  // GUI_obj_clear ();
  UMB_dump__ ();


  // get obj (now from memBlock)
  o2 = GUI_obj_pos (&mo1);
  if(!go) return 0;
  printf(" o2.d1=%lf\n",o2->d1);


  // clear the memBlock
  GUI_obj_free (&mo1);


  return 0;

}

#endif


// EOF
