/*

 * gtk_dlg_files.c
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
void GUI_DIALOG(){}
#endif
/*!
\file ../gui_gtk2/gtk_dlg_files.c
\brief File-open/save
\code

=====================================================
List_functions_start:

GUI_file_open__    open file, waiting.
GUI_file_save__    save file, waiting.

GUI_file_cb_dirsym1 INTERNAL
GUI_file_cb_filt    INTERNAL

List_functions_end:
=====================================================

needs gtk_entry


\endcode *//*----------------------------------------

gcc `pkg-config --cflags gtk+-3.0` gtk_dialog.c


======================================================================
*/


// // ex ../ut/ut_geo.h:
// #define IMIN(x,y)  (((x)<(y))?(x):(y))



// #include "../xa/MS_Def0.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_umem.h"

#include <gtk/gtk.h>
#include "../gui/gui_types.h"           // TYP_Event..
#include "../ut/ut_cast.h"             // INT_PTR

#include "../gui_gtk2/gtk_base.h"
#include "../gui_gtk2/gtk_entry.h"
#include "../gui_gtk2/gtk_dlg_files.h"


static  GtkWidget *UI_FileWin=NULL;



// ex gtk_core:
// extern int UI_WinposX, UI_WinposY;
extern int       UI_fontsizX, UI_fontsizY;
extern GtkWidget *UI_MainWin;

static  void         *UI_File_p1;



int GUI_file_cb_dirsym1 (void *parent, void *data);
int GUI_file_cb_dirsym2 (void *parent, void **data);
int GUI_file_cb_filt (void *parent, void *event, void *data);



//================================================================
GtkWidget* GUI_file_get () { return UI_FileWin; }
// returns file-window-widget 


//================================================================
  int GUI_file_open__ (char *filNam, int fSiz, char *dirNam, int dSiz,
                     char *filterO,
                     void *dirLst, char *title, char *filterI) {
//================================================================
/// \code
/// GUI_File_open          open file, waiting.
/// Input:
///   fSiz       max size of filNam in bytes
///   dSiz       max size of dirNam in bytes
///   dirNam     directory or "." (active directory)
///   dirLst     NULL or filename with "symbol directory"-lines (Button DIR-SYM)
///   filterI    NULL or filtertext; eg "*.c";
/// Output:
///   filNam
///   dirNam
///   filterO    modified filtertext or NULL
///   retCode    0=OK, -1=Cancel, -2=fSiz/dSiz too small
///
/// Example:
///  char   s1[204], s2[204];    int  irc;
///  strcpy(s2, ".");
///  irc = GUI_file_open__ (s1, 200, s2, 200, NULL, NULL, "open", "*.c");
///  if(irc) return irc;
///    printf(" open |%s|%s|\n",s1,s2);
///
/// TODO: case-insensitivity of filter ..
/// \endcode
 
  int           iRes, i1, ysiz;
  char          *p1;
  GtkWidget     *box1, *w1, *w2, *bt1;
  GtkFileFilter *wf;


  printf("GUI_file_open__ |%s|%s|\n",dirNam,filterI);

  UI_File_p1 = filterO;

  UI_FileWin = gtk_file_chooser_dialog_new (title,
            GTK_WINDOW (UI_MainWin),       // stay on top of this or NULL
            GTK_FILE_CHOOSER_ACTION_OPEN,
            GTK_STOCK_CANCEL,               // begin buttons ..
            GTK_RESPONSE_CANCEL,
            GTK_STOCK_OPEN,
            GTK_RESPONSE_ACCEPT,
            NULL);   // NULL = end buttons


  //----------------------------------------------------------------
  // preset directory
  if(!strcmp(dirNam, ".")) {
    OS_get_curDir (dirNam, dSiz);
  }
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (UI_FileWin), dirNam);


  //----------------------------------------------------------------
  // extra-widgets
    box1 = gtk_hbox_new (FALSE, 0);
    ysiz = UI_fontsizY * 2;

    gtk_widget_show (box1);


    w1 = gtk_label_new ("Filter:");
    gtk_container_add (GTK_CONTAINER (box1), w1);
    gtk_widget_show (w1);

    w2 = gtk_entry_new ();
    gtk_widget_set_size_request (w2, 40, ysiz); // DOES NOT WORK FOR GTK+3.2.0
    gtk_container_add (GTK_CONTAINER (box1), w2);
    gtk_entry_set_text ((GtkEntry*)w2, filterI);
    gtk_widget_show (w2);


    gtk_widget_set_events (GTK_WIDGET(w2),
                           GDK_FOCUS_CHANGE_MASK|
                           GDK_BUTTON_RELEASE_MASK);
    g_signal_connect (G_OBJECT (w2),
                        "key-release-event",
                        G_CALLBACK (GUI_file_cb_filt), UI_FileWin);


    if(dirLst) {
      bt1 = gtk_button_new_with_label ("DIR-SYM");
      gtk_container_add (GTK_CONTAINER (box1), bt1);
      g_signal_connect (bt1, "clicked",
                        G_CALLBACK(GUI_file_cb_dirsym1), dirLst);
      gtk_widget_show (bt1);
    }


    gtk_file_chooser_set_extra_widget (GTK_FILE_CHOOSER(UI_FileWin), box1);


  //----------------------------------------------------------------
  // preset filter
    if(filterI) {
      GtkFileFilter *wf = NULL;
      printf(" preset |%s|\n",filterI);
      gtk_entry_set_text (GTK_ENTRY(w2), filterI);
      wf = gtk_file_filter_new ();
      gtk_file_filter_add_pattern (wf, filterI);
      gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (UI_FileWin), wf);
    }


  //----------------------------------------------------------------
  // wait, modal:
  // start waiting; does not return until user clicks button.
  iRes = gtk_dialog_run (GTK_DIALOG (UI_FileWin));    // wait (modal) !
    // printf(" iRes=%d\n",iRes);
  // GTK_RESPONSE_ACCEPT
  // GTK_RESPONSE_OK GTK_RESPONSE_YES GTK_RESPONSE_CANCEL ..


  if (iRes == GTK_RESPONSE_ACCEPT) {
    char *filename;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (UI_FileWin));
      // printf(" GTK_RESPONSE_ACCEPT |%s|\n",filename);

#ifdef _MSC_VER
    p1 = strrchr(filename, '\\');
#else
    p1 = strrchr(filename, '/');
#endif
    i1 = p1 - filename;
    if(i1 >= dSiz) {iRes = -2; goto L_exit; }
      // printf(" i1=%d\n",i1);
    strncpy(dirNam, filename, i1);
    dirNam[i1] = '\0';
    ++p1;
    if(strlen(p1) >= fSiz) {iRes = -2; goto L_exit; }
    strcpy(filNam, p1);

    g_free (filename);
    iRes = 0;

  } else {
    iRes = -1;
  }


  L_exit:
  gtk_widget_destroy (UI_FileWin);
  UI_FileWin = NULL;                 // 2013-05-13
  if(dirLst) GUI_list1_dlg_del ();   // remove dir-list


  return iRes;

}


//================================================================
  int GUI_file_save__ (char *filNam, int fSiz, char *dirNam, int dSiz,
                     void *dirLst,
                     char *title, char *filter) {
//================================================================
/// \code
/// GUI_File_save          save file
/// Input:      
///   filNam     file to save
///   fSiz       size of filNam in chars
///   dirNam     directory or "." (active directory)
///   dSiz       size of dirNam in chars
///   dirLst     NULL or filename with "symbol directory"-lines (Button DIR-SYM)
///   filter     NULL or filtertext; eg "*.c"
/// Output:
///   filNam
///   dirNam
///   retCode    0=OK, -1=Cancel, -2=fSiz/dSiz too small, -3=file does not exist
///
/// TODO: case-insensitivity of filter ..
/// \endcode
  
  int           iRes, i1, ysiz;
  char          *p1;
  GtkWidget     *box1, *w1, *w2, *bt1;
  GtkFileFilter *wf;
            
            
  printf("GUI_file_save__ |%s|%s|%s|\n",filNam,dirNam,filter);

/*
  {
    char s1[1024];
#ifdef _MSC_VER
    sprintf(s1, "%s\\%s",dirNam,filNam);
#else
    sprintf(s1, "%s/%s",dirNam,filNam);
#endif
    if(!OS_checkFilExist (s1, 0)) return -3;
  }
*/     
            
  UI_FileWin = gtk_file_chooser_dialog_new (title,
            GTK_WINDOW (UI_MainWin),       // stay on top of this or NULL
            GTK_FILE_CHOOSER_ACTION_SAVE,
            GTK_STOCK_CANCEL,               // begin buttons ..
            GTK_RESPONSE_CANCEL,
            GTK_STOCK_SAVE,
            GTK_RESPONSE_ACCEPT,
            NULL);   // NULL = end buttons


  // confirm overwrite
  gtk_file_chooser_set_do_overwrite_confirmation (
    GTK_FILE_CHOOSER (UI_FileWin), TRUE);


  //----------------------------------------------------------------
  // preset directory
  if(!strcmp(dirNam, ".")) {
    getcwd (dirNam, dSiz);
  }
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (UI_FileWin), dirNam);

  gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (UI_FileWin), filNam);



  //----------------------------------------------------------------
  // extra-widgets
    box1 = gtk_hbox_new (FALSE, 0);
    ysiz = UI_fontsizY * 2;

    gtk_widget_show (box1);

    w1 = gtk_label_new ("Filter:");
    gtk_container_add (GTK_CONTAINER (box1), w1);
    gtk_widget_show (w1);

    w2 = gtk_entry_new ();
    gtk_widget_set_size_request (w2, 40, ysiz); // DOES NOT WORK FOR GTK+3.2.0
    gtk_container_add (GTK_CONTAINER (box1), w2);
    gtk_widget_show (w2);

    gtk_widget_set_events (GTK_WIDGET(w1),
                           GDK_FOCUS_CHANGE_MASK|
                           GDK_BUTTON_RELEASE_MASK);
    g_signal_connect (G_OBJECT (w2),
                        "key-release-event",
                        G_CALLBACK (GUI_file_cb_filt), UI_FileWin);

    if(dirLst) {
      bt1 = gtk_button_new_with_label ("DIR-SYM");
      gtk_container_add (GTK_CONTAINER (box1), bt1);
      g_signal_connect (bt1, "clicked",
                        G_CALLBACK(GUI_file_cb_dirsym1), dirLst);
      gtk_widget_show (bt1);
    }


    gtk_file_chooser_set_extra_widget (GTK_FILE_CHOOSER(UI_FileWin), box1);


  //----------------------------------------------------------------
  // preset filter
    if(filter) {
      GtkFileFilter *wf = NULL;
      // printf(" preset |%s|\n",filter);
      gtk_entry_set_text (GTK_ENTRY(w2), filter);
      wf = gtk_file_filter_new ();
      gtk_file_filter_add_pattern (wf, filter);
      gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (UI_FileWin), wf);
    }


  //----------------------------------------------------------------
  // wait, modal:
  // start waiting; does not return until user clicks button.
  iRes = gtk_dialog_run (GTK_DIALOG (UI_FileWin));    // wait (modal) !
    // printf(" iRes=%d\n",iRes);  // -6=cancel, -3=ACCEPT
  // GTK_RESPONSE_ACCEPT
  // GTK_RESPONSE_OK GTK_RESPONSE_YES GTK_RESPONSE_CANCEL ..


  if (iRes == GTK_RESPONSE_ACCEPT) {
    char *filename;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (UI_FileWin));
      // printf(" GTK_RESPONSE_ACCEPT |%s|\n",filename);

#ifdef _MSC_VER
    p1 = strrchr(filename, '\\');
#else
    p1 = strrchr(filename, '/');
#endif
    i1 = p1 - filename;
    if(i1 >= dSiz) {iRes = -2; goto L_exit; }
      // printf(" i1=%d\n",i1);
    strncpy(dirNam, filename, i1);
    dirNam[i1] = '\0';
    ++p1;
    if(strlen(p1) >= fSiz) {iRes = -2; goto L_exit; }
    strcpy(filNam, p1);

    g_free (filename);
    iRes = 0;

  } else {
    iRes = -1;
  }


  L_exit:
  gtk_widget_destroy (UI_FileWin);
  UI_FileWin = NULL;                 // 2013-05-13
  if(dirLst) GUI_list1_dlg_del ();   // remove dir-list


  return iRes;

}


//================================================================
 int GUI_file_cb_filt (void *parent, void *event, void *data) {
//================================================================
/// INTERNAL set filtertext for OPEN-Dialog

  int    i1;
  char                 *txt, *all={"*"};
  static GtkFileFilter *wf = NULL;


  txt = (char*) gtk_entry_get_text (GTK_ENTRY(parent));


  // printf("GUI_file_cb_filt |%s|\n",txt);

  i1 = strlen(txt);
  if(strlen(txt) < 1) txt = all;
  if(i1 < 80) {
    if(UI_File_p1) strcpy (UI_File_p1, txt);
  }

  wf = gtk_file_filter_new ();
  gtk_file_filter_add_pattern (wf, txt);
  gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (data), wf);


  return 1;

}


//================================================================
  int GUI_file_cb_dirsym1 (void *parent, void *data) {
//================================================================
/// INTERNAL
/// button DIR-SYM pressed; display content of userfile (symbol,directory)
///   data    filename with "symbol directory"-lines

  char         *titLst[]={"Symbol", "Directory"};
  GtkWidget    *w1;


  // printf("GUI_file_cb_dirsym1 \n");


  // show list of directories, get one
  // GUI_List_dlg__ (UI_FileWin, "select directory ..", data, "2",
                  // titLst, GUI_file_cb_dirsym2, 80, 20);
  // w1 = gtk_widget_get_toplevel (parent);                       // 2013-05-13
  // GUI_list1_dlg__ (w1, "select directory ..", data, "2",
  GUI_list1_dlg__ (NULL, "select directory ..", data, "2",
                  titLst, GUI_file_cb_dirsym2, "80,20");

  return 0;
}


//================================================================
  int GUI_file_cb_dirsym2 (void *parent, void **data) {
//================================================================
/// \code
/// INTERNAL
/// user selection in directory-list;
/// display content of selected dir. in fileChooser ..
/// \endcode

  char  wTit[80];


  // printf("GUI_file_cb_dirsym2 %d\n",GUI_DATA_EVENT);


  if(data) {
    // normal selection
    // printf("GUI_File_cb_dirsym2 |%s|%s|\n",(char*)data[4],(char*)data[5]);

    if(GUI_DATA_EVENT == TYP_EventExit) return FALSE;


    gtk_window_set_modal (GTK_WINDOW (UI_FileWin), TRUE);
    gtk_window_set_transient_for (GTK_WINDOW(UI_FileWin), GTK_WINDOW(UI_MainWin));


    // set new directory
    sprintf (wTit, " sym.dir %s", GUI_DATA_S3);
    gtk_window_set_title (GTK_WINDOW (UI_FileWin), wTit);


    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (UI_FileWin),
                                         GUI_DATA_S4);

    
    // delete ListWindow
    GUI_list1_dlg_del ();



  } else {
    printf("GUI_file_cb_dirsym2 list cancelled\n");
  }

  return FALSE;

}


// EOF
