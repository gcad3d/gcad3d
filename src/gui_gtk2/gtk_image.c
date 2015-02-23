/*
 *
 * Copyright (C) 2012 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
2012-02-01 extracted from ut_gtk.c.  RF.

-----------------------------------------------------
*/
#ifdef globTag
void GUI_IMAGE(){}
#endif
/*!
\file ../gui_gtk2/gtk_image.c
\brief image, button with image, tutorial
\code

=====================================================
List_functions_start:

GUI_img__               backgroundimage
GUI_Ico_init            init and load all icons (used with GUI_TreeIT__)
GUI_TUT_m__             ScreenCast-mousebuttons    NOT IMPLEM.; ONLY GTK3
GUI_TUT_t__             ScreenCast-Text            NOT IMPLEM.; ONLY GTK3

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


see tst_gtk_image.c

======================================================================
*/



// #include "../xa/MS_Def0.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <gtk/gtk.h>

#include "../ut/ut_umem.h"
#include "../ut/ut_os.h"               // OS_ ..

#include "../gui/gui_types.h"
#include "../gui/gui_base.h"

#include "../gui_gtk2/gtk_image.h"


// local vars:
static int       icoNr;
       GdkPixbuf **IcoTab;



// gtk_core.c:
extern GtkWidget *UI_act_wi;



//====================================================================
  MemObj GUI_img__ (MemObj *o_par, char* filnam, char *opts) {
//====================================================================
/// \code
/// display Pixmap  (.xpm or .jpg or .png)
/// Input:
///   o_par      parentBox
///   fNam       full image-filename
///   opts       options; (HorSiz,VertSiz)
///              HorSiz,VertSiz:  size in characters; default is automatic size.
///                              'e' = expand widget; default is fixed size.
///                               negative values: size in pixels.
///              Examples: "" or "10" or "10e,e"
///                "10e,e" horiz. size 10 chars, hor. and vert. expandable.
/// \endcode

  int          pTyp;
  void         *w_par;
  Obj_Unknown  *go;


  printf("GUI_img__ |%s|\n",filnam);


  if(OS_checkFilExist(filnam, 0) == 0) {
    // TX_Error (" Imagefile %s does not exist",fNam);
    MSG_err_1 ("NOEX_fil", "%s", filnam);
    return (UME_obj_invalid_set (-2));
  }


  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parent__ (&pTyp, o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  // get address & memspc
  GUI_obj_spc ((void**)&go, sizeof(Obj_Unknown));
  if(!go) return (UME_obj_invalid_set (-1));


  // image direct into containeryy
  // get jpg|png|xpm
  UI_act_wi = gtk_image_new_from_file (filnam);

  GUI_w_pack1 (pTyp, w_par, UI_act_wi, opts);


  go->gio_typ  = TYP_GUI_Unknown;
  go->widget   = UI_act_wi;

  return (((Obj_Unknown*)go)->mem_obj);

}


//================================================================
  int GUI_Ico_init (char *icoNam[]) {
//================================================================
/// \code
/// GUI_Ico_init         create and load all icons
/// iconfiles must be in directory <base>/icons/
/// Example:
/// char *icoNam[]={"t2.png","t3.png",NULL};
/// \endcode

// was Brw_Init, GUI_TreeIT__
 
  int  i1;
  char cbuf[256];
  void **vp1;

  GError            *error = NULL;

  printf("GUI_Ico_init \n");


  // count nr of icons
  for(i1=0; i1<1000; ++i1) {
    if(icoNam[i1]) continue;
    icoNr = i1;
    IcoTab = (GdkPixbuf**) malloc (sizeof(void*) * icoNr);
    break;
  }


  // init icons
  i1 = 0;
  vp1 = (void**)icoNam;
  while (*vp1) {
    sprintf(cbuf, "%s%s",OS_get_ico_dir(),(char*)*vp1);
      // printf(" icoNam[%d]=|%s|\n",i1,cbuf);
    if(OS_checkFilExist(cbuf, 0) == 0) {
      // TX_Error (" Imagefile %s does not exist",cbuf);
      MSG_err_1 ("NOEX_fil", "%s", cbuf);
    } else {
      IcoTab[i1] = gdk_pixbuf_new_from_file(cbuf, &error);
      if (error) {
        TX_Error ("Could not load icon: %s\n", error->message);
        g_error_free(error);
        error = NULL;
      }
    }
    ++vp1;
    ++i1;
    if(i1 > icoNr) {printf("*** GUI_Ico_init - too much icons\n"); break;}
  }

  return 0;

}


//================================================================
  int GUI_TUT_m__ (int ii) {
//================================================================
/// \code
/// GTK2: NOT IMPLEMENTED; ONLY GTK3.
/// display image "mousebutton" without decoration for 0.8 secs
/// Input: ii=mousebutton; 1|2|3
/// \endcode

  TX_Print ("ScreenCast-Support only Gtk3");
  return 0;

}


//================================================================
  int GUI_TUT_t__ (char *ltxt) {
//================================================================
/// \code
/// GTK2: NOT IMPLEMENTED; ONLY GTK3.
/// display text without decoration for 0.8 secs
/// \endcode

  TX_Print ("ScreenCast-Support only Gtk3");
  return 0;

}


// EOF
