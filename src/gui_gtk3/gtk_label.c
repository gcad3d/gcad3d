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
void GUI_LABEL(){}
#endif
/*!
\file ../gui_gtk3/gtk_label.c
\brief backgroundtext
\code

=====================================================
List_functions_start:

GUI_label__            BackgroundText (Label) with align
GUI_label_mod          modify label
GUI_label_styl         modify colour of label-text;

GUI_label_htm__        static text with html-attributes
GUI_label_htm_mod      modify text with html-attributes

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


======================================================================
*/




// #include "../xa/MS_Def0.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include "../gui/gui_types.h"                   // UI_Func
#include "../ut/ut_umem.h"                     // Memspc
#include "../gui/gui_base.h"                  // GUI_obj_*

#include "../gui_gtk3/gtk_base.h"
#include "../gui_gtk3/gtk_label.h"


// gtk_core.c:
extern GtkWidget *UI_act_wi;
extern int       UI_fontsizX, UI_fontsizY;
extern GdkRGBA   *UI_stylTab[3];



//=====================================================================
  int GUI_label_styl (MemObj *o_par,  int iCol) {
//=====================================================================
/// modify label-colour

// see GUI_button_styl


  int       pTyp;
  Obj_Label *g_lbl;

  GUI_obj_typ (&pTyp, (void**)&g_lbl, o_par);
  if(!pTyp) return (-1);


  if(iCol < 1) {
    gtk_widget_override_color (g_lbl->label, 0, NULL);

  } else {
    gtk_widget_override_color (g_lbl->label, 0, UI_stylTab[iCol]);
  }


  return 0;

}


//=====================================================================
  int GUI_label_mod (MemObj *o_par, char* txt) {
//=====================================================================
/// modify label

  int       pTyp;
  Obj_Label *g_lbl;

  GUI_obj_typ (&pTyp, (void**)&g_lbl, o_par);
  if(!pTyp) return (-1);

  gtk_label_set_text (GTK_LABEL(g_lbl->label), txt);

  return 0;

}


//===============================================================================
  MemObj GUI_label__ (MemObj *o_par, char* ltxt, char *opts) {
//===============================================================================
/// \code
/// display static text  (Label) with align
/// Input:
///   o_par    must be Obj_Box
///   ltxt     text to display
///   opts     options; (alignment)
///            first character 'l'=left aligned; 'r'=right aligned; def.=center.
///            Example: "l" or ""
/// 
/// Text with attributes: see GUI_HtmLabel
/// more than 1 line: use \n
/// update text with GUI_label_mod
/// 
/// \endcode


  int        i1, pTyp;
  char       c1;
  void       *w_par;
  Obj_Label  *gLb;
  GtkWidget  *w1;


  // printf("GUI_label__ |%s|%s|\n",ltxt,opts);

  // get gtk-widget for parentBox from MemObj

  w_par = GUI_obj_parent__ (&pTyp, o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  GUI_obj_spc ((void**)&gLb, sizeof(Obj_Label));
  if(!gLb) return (UME_obj_invalid_set (-1));


  //----------------------------------------------------------------
  w1 = gtk_label_new (ltxt);

  // align
  c1 = GUI_opts_get1 (&opts, "clr");  // 1=default
  if(c1 == 'l')
    gtk_misc_set_alignment (GTK_MISC (w1), 0.f, 0.5f); // left
  if(c1 == 'r')
    gtk_misc_set_alignment (GTK_MISC (w1), 1.f, 0.5f);  // right


  GUI_w_pack1 (pTyp, w_par, w1, opts);


  //----------------------------------------------------------------
  gLb->gio_typ  = TYP_GUI_Label;
  gLb->label    = w1;

  UI_act_wi = w1;

  // return ((void*)gLb);
  return (((Obj_Unknown*)gLb)->mem_obj);

}


//=====================================================================
  int GUI_label_htm_mod (MemObj *o_par, char* txt) {
//=====================================================================
/// \code
/// modify htm-label
/// see GUI_label_htm__
/// \endcode

  int       pTyp;
  Obj_Label *go;

  GUI_obj_typ (&pTyp, (void**)&go, o_par);
  if(!pTyp) return (-1);

  gtk_label_set_markup (GTK_LABEL(go->label), txt);

  return 0;

}


//=====================================================================
  MemObj GUI_label_htm__ (MemObj *o_par, char* ltxt, char *opts) {
//=====================================================================
/// \code
/// GUI_HtmLabel           static text with html-attributes
///
/// Input:
///   o_par      parentBox
///   ltxt     text to display
///   opts     options; (alignment)
///            first character 'l'=left aligned; 'r'=right aligned; def.=center.
///            Example: "l" or ""
///
/// Examples:
/// GUI_label_htm__ (&box1, "<b>Text:</b>", "");
/// GUI_label_htm__ (&box1,"<span weight=\"bold\">Text:</span>","l");
/// GUI_label_htm__ (&box1,"<span size=\"x-small\">Text:</span>","r");
/// w = GUI_label_htm__ (&box1,"<span fgcolor=\"#00ffff\">Text:</span>","");
/// GUI_label_htm_mod (&w,"<span fgcolor=\"#ff0000\" weight=\"bold\">xy</span>");
///   bgcolor ..
/// see "Pango Text Attribute Markup Language"
/// "<b>text<span foreground=\"blue">text</span></b>"
/// \endcode

  GtkWidget  *w1;
  int        pTyp;
  char       c1;
  void       *w_par;
  Obj_Label  *go;


  // printf("GUI_label_htm__ |%s|%s|\n",ltxt,opts);


  // get gtk-widget for parentBox from MemObj
  w_par = GUI_obj_parent__ (&pTyp, o_par);
  if(!w_par) return (UME_obj_invalid_set (-3));


  GUI_obj_spc ((void**)&go, sizeof(Obj_Label));
  if(!go) return (UME_obj_invalid_set (-1));



  w1 = gtk_label_new (NULL);
  gtk_label_set_markup(GTK_LABEL(w1), ltxt);


  // align
  c1 = GUI_opts_get1 (&opts, "clr");  // 1=default
  if(c1 == 'l')
    gtk_misc_set_alignment (GTK_MISC (w1), 0.f, 0.5f); // left
  if(c1 == 'r')
    gtk_misc_set_alignment (GTK_MISC (w1), 1.f, 0.5f);  // right


  GUI_w_pack1 (pTyp, w_par, w1, opts);



  go->gio_typ  = TYP_GUI_Label;
  go->label    = w1;

  UI_act_wi = w1;

  // return ((void*)gLb);
  return (((Obj_Unknown*)go)->mem_obj);

}


// EOF
