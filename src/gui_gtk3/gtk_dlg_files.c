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
void GUI_DIALOG(){}
#endif
/*!
\file ../gui_gtk3/gtk_dlg_files.c
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

#include "../gui_gtk3/gtk_base.h"
#include "../gui_gtk3/gtk_entry.h"
#include "../gui_gtk3/gtk_dlg_files.h"


static  GtkWidget *UI_FileWin=NULL;



// ex gtk_core:
// extern int UI_WinposX, UI_WinposY;
extern int       UI_fontsizX, UI_fontsizY;
extern GtkWidget *UI_MainWin;


typedef struct {char *fNam, *dNam, *lNam, *filtI, *filtO, *title;
                int fSiz, dSiz, stat;}                             GUI_FILE_DLG;
static  GUI_FILE_DLG GUI_dlg1;



int GUI_file_cb_dirsym1 (void *parent, void *data);
int GUI_file_cb_dirsym2 (void *parent, void **data);
int GUI_file_cb_filt (void *parent, void *event, void *data);
int GUI_file_open_1 ();





//================================================================
void* GUI_file_get () { return UI_FileWin; }
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
 

  int irc;


  printf("GUI_file_open__ |%s|%s|\n",dirNam,filterI);

  GUI_dlg1.fNam  = filNam;
  GUI_dlg1.fSiz  = fSiz;
  GUI_dlg1.dNam  = dirNam;
  GUI_dlg1.dSiz  = dSiz;
  GUI_dlg1.lNam  = dirLst;
  GUI_dlg1.filtI = filterI;
  GUI_dlg1.filtO = filterO;
  GUI_dlg1.title = title;


  L_start:
  GUI_dlg1.stat = 0;
  irc = GUI_file_open_1 ();
    printf(" open_1 irc = %d stat=%d\n",irc,GUI_dlg1.stat);
  if(!GUI_dlg1.stat) return irc;

  // wait until DIR-SYM-List is closed ..
  L_noAmoi:
  GUI_update__ ();  // bring up DIR-SYM-List
  if(GUI_dlg1.stat < 0) return -1; // cancel DirList
  if(GUI_dlg1.stat) {
    usleep (10000);
    goto L_noAmoi;
  }

    printf("exit open__\n");
  goto L_start;

}


//================================================================
  int GUI_file_open_1 () {
//================================================================



  int           iRes, i1, ysiz;
  char          *p1;
  GtkWidget     *box1, *w1, *w2, *bt1;
  GtkFileFilter *wf;

  printf("GUI_file_open_1 |%s|\n",GUI_dlg1.dNam);


  UI_FileWin = gtk_file_chooser_dialog_new (GUI_dlg1.title,
            GTK_WINDOW (UI_MainWin),       // parent; stay on top of this or NULL
            GTK_FILE_CHOOSER_ACTION_OPEN,
            "_Cancel", GTK_RESPONSE_CANCEL,
            "_Open",   GTK_RESPONSE_ACCEPT,
            NULL);   // NULL = end buttons

  //----------------------------------------------------------------
  // preset directory
  if(!strcmp(GUI_dlg1.dNam, ".")) {
    OS_get_curDir (GUI_dlg1.dNam, GUI_dlg1.dSiz);
  }
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (UI_FileWin),
                                       GUI_dlg1.dNam);


  //----------------------------------------------------------------
  // extra-widgets
    box1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    ysiz = 0;

    gtk_widget_show (box1);


    w1 = gtk_label_new ("Filter:");
    gtk_container_add (GTK_CONTAINER (box1), w1);
    gtk_widget_show (w1);

    w2 = gtk_entry_new ();
    gtk_widget_set_size_request (w2, 40, ysiz); // DOES NOT WORK FOR GTK+3.2.0
    gtk_container_add (GTK_CONTAINER (box1), w2);
    gtk_entry_set_text ((GtkEntry*)w2, GUI_dlg1.filtI);
    gtk_widget_show (w2);


    gtk_widget_set_events (GTK_WIDGET(w2),
                           GDK_FOCUS_CHANGE_MASK|
                           GDK_BUTTON_RELEASE_MASK);
    g_signal_connect (G_OBJECT (w2),
                        "key-release-event",
                        G_CALLBACK (GUI_file_cb_filt), UI_FileWin);


    if(GUI_dlg1.lNam) {
      bt1 = gtk_button_new_with_label ("DIR-SYM");
      gtk_container_add (GTK_CONTAINER (box1), bt1);
      g_signal_connect (bt1, "clicked",
                        G_CALLBACK(GUI_file_cb_dirsym1), GUI_dlg1.lNam);
      gtk_widget_show (bt1);
    }


    gtk_file_chooser_set_extra_widget (GTK_FILE_CHOOSER(UI_FileWin), box1);


  //----------------------------------------------------------------
  // preset filter
    if(GUI_dlg1.filtI) {
      GtkFileFilter *wf = NULL;
      printf(" preset |%s|\n",GUI_dlg1.filtI);
      gtk_entry_set_text (GTK_ENTRY(w2), GUI_dlg1.filtI);
      wf = gtk_file_filter_new ();
      gtk_file_filter_add_pattern (wf, GUI_dlg1.filtI);
      gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (UI_FileWin), wf);
    }


  //----------------------------------------------------------------
  // wait, modal:
  // start waiting; does not return until user clicks button.
  iRes = gtk_dialog_run (GTK_DIALOG (UI_FileWin));    // wait (modal) !
    printf(" iRes=%d\n",iRes);


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
    if(i1 >= GUI_dlg1.dSiz) {iRes = -2; goto L_exit; }
      // printf(" i1=%d\n",i1);
    strncpy(GUI_dlg1.dNam, filename, i1);
    GUI_dlg1.dNam[i1] = '\0';
    ++p1;
    if(strlen(p1) >= GUI_dlg1.fSiz) {iRes = -2; goto L_exit; }
    strcpy(GUI_dlg1.fNam, p1);

    g_free (filename);
    iRes = 0;

  } else {
    iRes = -1;
  }

  L_exit:
  if(UI_FileWin) {    // not yet killed from DIR-SYM
    gtk_widget_destroy (UI_FileWin);
    UI_FileWin = NULL;
  }

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
  


  int irc;


  printf("GUI_file_save__ |%s|%s|%s|\n",filNam,dirNam,filter);


  GUI_dlg1.fNam  = filNam;
  GUI_dlg1.fSiz  = fSiz;
  GUI_dlg1.dNam  = dirNam;
  GUI_dlg1.dSiz  = dSiz;
  GUI_dlg1.lNam  = dirLst;
  GUI_dlg1.filtI = filter;
  GUI_dlg1.title = title;


  L_start:
  GUI_dlg1.stat = 0;
  irc = GUI_file_save_1 ();
    printf(" save_1 irc = %d stat=%d\n",irc,GUI_dlg1.stat);
  if(!GUI_dlg1.stat) return irc;

  // wait until DIR-SYM-List is closed ..
  L_noAmoi:
  GUI_update__ ();  // bring up DIR-SYM-List
  if(GUI_dlg1.stat < 0) return -1; // cancel DirList
  if(GUI_dlg1.stat) {
    usleep (10000);
    goto L_noAmoi;
  }

    printf("exit save__\n");
  goto L_start;

}


//================================================================
  int GUI_file_save_1 () {
//================================================================


  int           iRes, i1, ysiz;
  char          *p1;
  GtkWidget     *box1, *w1, *w2, *bt1;
  GtkFileFilter *wf;
            
            
  printf("GUI_file_save_1\n");

            
  UI_FileWin = gtk_file_chooser_dialog_new (GUI_dlg1.title,
            GTK_WINDOW (UI_MainWin),       // stay on top of this or NULL
            GTK_FILE_CHOOSER_ACTION_SAVE,
            "_Cancel", GTK_RESPONSE_CANCEL,
            "_Save",   GTK_RESPONSE_ACCEPT,
            NULL);   // NULL = end buttons


  // confirm overwrite
  gtk_file_chooser_set_do_overwrite_confirmation (
    GTK_FILE_CHOOSER (UI_FileWin), TRUE);


  //----------------------------------------------------------------
  // preset directory
  if(!strcmp(GUI_dlg1.dNam, ".")) {
    getcwd (GUI_dlg1.dNam, GUI_dlg1.dSiz);
  }
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (UI_FileWin),
                                       GUI_dlg1.dNam);

  gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (UI_FileWin),
                                    GUI_dlg1.fNam);



  //----------------------------------------------------------------
  // extra-widgets
    box1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    ysiz = 0;

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

    if(GUI_dlg1.lNam) {
      bt1 = gtk_button_new_with_label ("DIR-SYM");
      gtk_container_add (GTK_CONTAINER (box1), bt1);
      g_signal_connect (bt1, "clicked",
                        G_CALLBACK(GUI_file_cb_dirsym1), GUI_dlg1.lNam);
      gtk_widget_show (bt1);
    }


    gtk_file_chooser_set_extra_widget (GTK_FILE_CHOOSER(UI_FileWin), box1);


  //----------------------------------------------------------------
  // preset filter
    if(GUI_dlg1.filtI) {
      GtkFileFilter *wf = NULL;
      // printf(" preset |%s|\n",GUI_dlg1.filtI);
      gtk_entry_set_text (GTK_ENTRY(w2), GUI_dlg1.filtI);
      wf = gtk_file_filter_new ();
      gtk_file_filter_add_pattern (wf, GUI_dlg1.filtI);
      gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (UI_FileWin), wf);
    }


  //----------------------------------------------------------------
  // wait, modal:
  // start waiting; does not return until user clicks button.
  iRes = gtk_dialog_run (GTK_DIALOG (UI_FileWin));    // wait (modal) !
    printf(" iRes=%d\n",iRes);  // -6=cancel, -3=ACCEPT


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
    if(i1 >= GUI_dlg1.dSiz) {iRes = -2; goto L_exit; }
      // printf(" i1=%d\n",i1);
    strncpy(GUI_dlg1.dNam, filename, i1);
    GUI_dlg1.dNam[i1] = '\0';
    ++p1;
    if(strlen(p1) >= GUI_dlg1.fSiz) {iRes = -2; goto L_exit; }
    strcpy(GUI_dlg1.fNam, p1);

    g_free (filename);
    iRes = 0;

  } else {
    iRes = -1;
  }


  L_exit:
  if(UI_FileWin) {    // not yet killed from DIR-SYM
    gtk_widget_destroy (UI_FileWin);
    UI_FileWin = NULL;                 // 2013-05-13
  }

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


  printf("GUI_file_cb_filt |%s|\n",txt);

  i1 = strlen(txt);
  if(strlen(txt) < 1) txt = all;
  if(i1 < 80) {
    if(GUI_dlg1.filtO) strcpy (GUI_dlg1.filtO, txt);
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

  int       i1;
  char      *titLst[]={"Symbol", "Directory"};
  GtkWidget *w1;


  printf("GUI_file_cb_dirsym1 \n");

  GUI_dlg1.stat = 1;

  // gtk_widget_set_sensitive (parent, 0); // block parent
  w1 = gtk_widget_get_toplevel (UI_FileWin);
  gtk_widget_destroy (w1);
  UI_FileWin = NULL;
    printf(" after kill FileSel-Win\n");



  // show list of directories, get one
  i1 = GUI_list1_dlg__ (NULL, "select directory ..", data, "2",
                        titLst, GUI_file_cb_dirsym2, "80,20");
    printf("after GUI_list1_dlg__ \n");


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

  // char  wTit[80];


  printf("GUI_file_cb_dirsym2 %d\n",GUI_DATA_EVENT);
// return FALSE;

  if(GUI_DATA_EVENT == TYP_EventExit) {
    // close win ("X") pressed
    GUI_dlg1.stat = -1;
    return FALSE;
  }



  if(data) {
    // normal selection; 
      printf("GUI_File_cb_dirsym2 |%s|%s|\n",GUI_DATA_S3,GUI_DATA_S4);

    // copy symbolic name -> title (size of title misssing)
    // strcpy (GUI_dlg1.title, GUI_DATA_S3);

    // copy dir -> dNam
    strcpy (GUI_dlg1.dNam, GUI_DATA_S4);
      printf(" GUI_dlg1.dNam=|%s|\n",GUI_dlg1.dNam);


    
    // delete this ListWindow
    GUI_list1_dlg_del ();
    GUI_dlg1.stat = 0;


  } else {
    printf("GUI_file_cb_dirsym2 list cancelled\n");
  }

    printf("exit GUI_File_cb_dirsym2 \n");

  return TRUE;
  // return FALSE;

}


// EOF
