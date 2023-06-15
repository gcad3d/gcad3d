/*
 *
 * Copyright (C) 2020 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
Select file for open.
Returns selected file via stdout.
All testoutput to stdout must be precede with "##"
-----------------------------------------------------
TODO:

-----------------------------------------------------
Modifications:
2020-01-10 Created. RF.

-----------------------------------------------------

. ../options.sh && make -f GUI_file.mak

/home/fwork/devel/bin/gcad3d/Linux_x86_64/GUI_file_gtk2 open /mnt/serv2/devel/cadfiles/gcad/ /mnt/serv2/devel/gcad3d/gCAD3D/cfg/dir.lst '*' 'Öffnen'

/home/fwork/devel/bin/gcad3d/Linux_x86_64/GUI_file_gtk2 save /mnt/serv2/devel/cadfiles/gcad/xx.y /mnt/serv2/devel/gcad3d/gCAD3D/cfg/dir.lst '*' 'Speichern'

cat /tmp/debug.dat

gcc `pkg-config --cflags gtk+-3.0` ../gui/GUI_file_open.c `pkg-config --libs gtk+-3.0` && ./a.out


*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>              // for ...

#include <gtk/gtk.h>

#include "../ut/deb_prt.h"          // printd


/// FilenamedelimiterChar
#if defined _MSC_VER || __MINGW64__
#define  fnam_del '\\'
#define  fnam_del_s "\\"
#else
#define  fnam_del '/'
#define  fnam_del_s "/"
#endif


//----------------------------------------------------------------
// prototypes
  void UTX_CleanCR (char* string);
  int OS_osVar_eval__ (char *fno, char *fni, int fnoSiz);
  int UTX_fnam1__ (char* par_dir, char* sNam, char* sIn);
  void UTX_CleanCR (char* string);
  char** UTX_wTab_file (char *memspc, int memSiz, char *fnam);
  char** OS_wTab_file (char **memspc, char *fnam);
  char* OS_get_tmp_dir();

	
//----------------------------------------------------------------
extern int DEB_prt_stat;    // ../ut/deb_prt.c


//----------------------------------------------------------------
// GLOBAL:
#ifdef _MSC_VER
  static char *sGui = "gtk2_MS";      
#else
  // MSYS
  static char *sGui = "gtk2";      
#endif

  int  nArg, fnID;
  char **parLst, *wTabSpc, *fnIn;
  char fnOut[400];
  char title[512];
  char sbuf1[512];
  
  char *par_dir;
  char *par_sym;
  char *par_flt;
  char *par_tit;

  GtkWidget *wfl1;

  char *AP_bin_dir;         // dir binaries




//================================================================
  int GUI_file_symdir__ (char *par_dir, int sSiz) {
//================================================================
// get symbolic-directory

  int    irc, il;
  char   s2[2048], s3[512], fnTmp[400], *binDir, *p1;
  FILE   *fpo, *fpi;


  printf("GUI_file_symdir__ |%s| %d\n",par_dir,sSiz);

  // get binDir
  binDir = getenv("gcad_dir_bin");
    printf(" GUI_file_symdir__-binDir |%s|\n",binDir);

  // get fnExe
  sprintf(fnTmp, "%s/GUI_dlg1_%s",binDir,sGui);
 
  // call exe, get output into par_dir
  irc = OS_exe_file__ (par_dir, sizeof(par_dir), fnTmp,
          "list1",
          par_sym,
          "- select symbolic directory -", 
          "x40,y20",
	  NULL);

  // output par_dir is a symbolic directory; get full path
  // get path for symbol s1  -> s3
  if((fpi=fopen(par_sym,"r")) == NULL) {
    printf("***** symdir__ - Error Open E002 %s\n",par_sym);
    return -2;
  }

  il = strlen(par_dir);
  while(fgets(s2, sizeof(s2), fpi) != NULL) {
    if(s2[0] == '#') continue;
    if(!strncmp(par_dir,s2,il)) {
      p1 = strchr(s2, ' ');
      if(!p1) {printf("***** symdir__ - Error E003 %s\n",par_dir); return -3;}
      while(*p1 == ' ') ++p1;    // skip leading blanks
      UTX_CleanCR (p1); // remove follow. CR
        // printf("##  symdir__-2 |%s|\n",p1);
      if(strlen(p1) < sizeof(s3)) {
        strcpy (s3, p1);
          printd("##  symdir__-2 |%s|\n",s3);
        break;
      } else {
        printf("***** symdir__ - Error Open E004\n");
        fclose(fpi);
        return -4;
      }
    }
  }

  fclose(fpi);
    printf("## GUI_file_symdir__-s3 |%s|\n",s3);



  //----------------------------------------------------------------
  // expand shell variables in filenames eg "${DIR_DEV}cadfiles/dxf/"
  irc = OS_osVar_eval__ (par_dir, s3, sSiz);
     printf("##  symdir__-3 |%s|\n",par_dir);



  //----------------------------------------------------------------
  // test if directory exists
  if((irc) || (!OS_checkFilExist(par_dir,1))) {
    // directory par_dir does not exist;
      printd("##  dir__ |%s| does not exist\n",par_dir);

    sprintf(s2,"*** ERROR - Directory %s does not exist",par_dir);

    // call exe, get output into par_dir
    irc = OS_exe_file__ (par_dir, sizeof(par_dir), fnTmp,
            s2,
	    NULL);

    par_dir[0] = '\0';
    return -5;
  }

    printd("##  ex-symdir__ |%s|\n",par_dir);


  return 0;

}


//================================================================
 int GUI_file_cb_open (void *parent, void *data) {
//================================================================
//  callback double-click on file ..
    
  char    *filename;
  
  printd("## GUI_file_cb_open \n");
  
  // GtkFileChooser *chooser = GTK_FILE_CHOOSER(wfl1);
  filename = gtk_file_chooser_get_filename (parent);
  printd("##  fn |%s|\n",filename);
  
  if(strlen(filename) < sizeof(sbuf1)) strcpy(sbuf1,filename);
  g_free (filename);


  //----------------------------------------------------------------
  gtk_widget_destroy (wfl1);
  
  // exit; return parameters
  return GUI_file_exit (sbuf1);
    
}


//================================================================
 int GUI_file_cb_filt (void *parent, void *data) {
//================================================================
/// INTERNAL set filtertext for OPEN-Dialog

  int    i1;
  char   *txt, *all={"*"};
  GtkFileFilter *wfi1 = NULL;


  
  txt = (char*) gtk_entry_get_text (GTK_ENTRY(parent));


  printd("## GUI_file_cb_filt |%s|\n",par_flt);

  i1 = strlen(txt);
  if(strlen(txt) < 1) txt = all;

  // set filter
  wfi1 = gtk_file_filter_new ();
  gtk_file_filter_add_pattern (wfi1, txt);
  gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (wfl1), wfi1);

  return 0;

}


//================================================================
  int GUI_save__ () {
//================================================================

// file:///usr/share/gtk-doc/html/gtk3/GtkFileChooserDialog.html

  int       irc;
  char      s1Dir[400], *filename;
  GtkWidget *wb1, *we1, *wl1;
  char fDir[256], fNam[128];
  gint res;


  printd("## GUI_save__\n");

  // prepare filename, title
  if(nArg < 5)  return GUI_file_err1 ();
  par_dir = parLst[1];
  par_sym = parLst[2];
  par_flt = parLst[3];
  par_tit = parLst[4];


  // separate par_dir, sNam
  UTX_fnam1__ (fDir, fNam, par_dir);
    printd("## _save__ fDir |%s| fNam |%s|  par_dir |%s|\n",fDir,fNam,par_dir);

  // prepare title
  sprintf(title, "%s %s",par_tit,par_dir);


  //----------------------------------------------------------------
  wfl1 = gtk_file_chooser_dialog_new (title,
                                      NULL,                // parent_window
                                      GTK_FILE_CHOOSER_ACTION_SAVE,
                                      ("SYM-DIR"),  2,
                                      ("_Cancel"),  0,
                                      ("_Save"),    1,
                                      NULL);

  // confirm overwrite
  gtk_file_chooser_set_do_overwrite_confirmation (
    GTK_FILE_CHOOSER (wfl1), TRUE);


  // copy directory, filename
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (wfl1), fDir);
  gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (wfl1), fNam);
  // gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (wfl1), par_dir);
  // gtk_file_chooser_set_current_folder_uri  (GTK_FILE_CHOOSER (wfl1), par_dir);


  // add filter- entry
  wb1 = gtk_hbox_new (FALSE, 0);    // gtk2 !
  // wb1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);   // gtk3 !

  we1 = gtk_entry_new ();
  gtk_container_add (GTK_CONTAINER (wb1), we1);
  g_signal_connect (G_OBJECT (we1),
                    "activate", G_CALLBACK (GUI_file_cb_filt), NULL);

  // if(par_flt) load into we1
  if(strcmp(par_flt, "NONE"))
    gtk_entry_set_text (GTK_ENTRY(we1), par_flt);

  wl1 = gtk_label_new ("  Filter");
  gtk_container_add (GTK_CONTAINER (wb1), wl1);

  // set filter as extra-widget
  gtk_widget_show (we1);
  gtk_widget_show (wl1);
  gtk_file_chooser_set_extra_widget (GTK_FILE_CHOOSER (wfl1), wb1);

  if(strcmp(par_flt, "NONE"))
    GUI_file_cb_filt (we1, NULL);



  // wait for user-select
  L_wait:
  res = gtk_dialog_run (GTK_DIALOG (wfl1));
    printd(" f-dialog_run %d\n",res);

  if (res == 0) {
    // cancel
    return (GUI_file_exit (""));

  } else if (res == 1) {
    // Save ..
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(wfl1);
    filename = gtk_file_chooser_get_filename (chooser);
    if(!filename) goto L_wait;
    if(strlen(filename) < sizeof(sbuf1)) strcpy(sbuf1,filename);
    g_free (filename);


  } else if(res == 2) {
    // change directory
    // gtk_widget_hide (wfl1);   // must hide; else zentity unter this window
    gtk_widget_set_sensitive (wfl1, FALSE);
    GUI_update__ ();

    // display list for directory-select
    irc = GUI_file_symdir__ (s1Dir, sizeof(s1Dir));
    if(irc < 0) {
      // error or cancel in symdir__
        printd("## **** error symdir__ %d |%s|\n",irc,s1Dir);
      s1Dir[0] = '\0';
    }

    if(strlen(s1Dir) > 1) {
      // update title
      sprintf(title, "%s %s", par_tit, s1Dir);
      gtk_window_set_title (GTK_WINDOW (wfl1), title);
      // add filter or "*"
      // if(strcmp(par_flt, "NONE")) strcat (s1Dir, par_flt);
      // strcat (s1Dir, "*"); - makes MSYS-gtk2 Error
      gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER(wfl1), s1Dir);
    }


    // gtk_widget_show (wfl1);
    gtk_widget_set_sensitive (wfl1, TRUE);

    goto L_wait;
  }


  //----------------------------------------------------------------
  gtk_widget_destroy (wfl1);

  // exit; return parameters
  return GUI_file_exit (sbuf1);

}


//================================================================
  int GUI_open__ () {
//================================================================

// file:///usr/share/gtk-doc/html/gtk3/GtkFileChooserDialog.html

  int       irc;
  char      s1Dir[400], *filename, s2[256];
  GtkWidget *wb1, *we1, *wl1;
  gint res;


  printf("## GUI_open__\n");

  // prepare filename, title
  if(nArg < 5)  return GUI_file_err1 ();
  par_dir = parLst[1];    // dir to open
  par_sym = parLst[2];    // sym-file
  par_flt = parLst[3];
  par_tit = parLst[4];


  //----------------------------------------------------------------
  wfl1 = gtk_file_chooser_dialog_new ("OPEN File",
                                        NULL,                // parent_window
                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                        ("SYM-DIR"),  2,
                                        ("_Cancel"),  0,
                                        ("_Open"),    1,
                                        NULL);

  // connect Enter / double-click
  g_signal_connect (G_OBJECT (wfl1),
                    "file-activated", G_CALLBACK (GUI_file_cb_open), NULL);

  // add filter- entry
  wb1 = gtk_hbox_new (FALSE, 0);    // gtk2 !
  // wb1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);    // gtk3 !

  we1 = gtk_entry_new ();
  gtk_container_add (GTK_CONTAINER (wb1), we1);
  g_signal_connect (G_OBJECT (we1),
                    "activate", G_CALLBACK (GUI_file_cb_filt), NULL);

  strcpy(s1Dir, par_dir);
  strcat(s1Dir, "*");
  gtk_file_chooser_set_filename (GTK_FILE_CHOOSER(wfl1), s1Dir);
  // gtk_file_chooser_set_current_folder (..);

  // if(par_flt) load into we1
  if(strcmp(par_flt, "NONE"))
    gtk_entry_set_text (GTK_ENTRY(we1), par_flt);

  wl1 = gtk_label_new ("  Filter");
  gtk_container_add (GTK_CONTAINER (wb1), wl1);


  gtk_widget_show (we1);
  gtk_widget_show (wl1);

  gtk_file_chooser_set_extra_widget (GTK_FILE_CHOOSER (wfl1), wb1);

  if(strcmp(par_flt, "NONE"))
    GUI_file_cb_filt (we1, NULL);

  // gtk_window_set_transient_for (GTK_WINDOW(wfl1),  NULL);
  // gtk_window_set_modal (GTK_WINDOW (wfl1), TRUE);


  // wait for user-select
  L_wait:
  res = gtk_dialog_run (GTK_DIALOG (wfl1));
    printf(" f-dialog_run %d\n",res);


  if (res == 0) {
    // cancel
    return (GUI_file_exit (""));


  } else if (res == 1) {
    // Open ..
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(wfl1);
    filename = gtk_file_chooser_get_filename (chooser);
    if(!filename) goto L_wait;
    if(strlen(filename) < sizeof(sbuf1)) {
        printf("## chooser-get |%s|\n",filename);
      strcpy(sbuf1,filename);
    }
    g_free (filename);


  } else if(res == 2) {
    // change directory
    // gtk_widget_hide (wfl1);  // must hide; else zentity unter this window
    gtk_widget_set_sensitive (wfl1, FALSE);
    GUI_update__ ();

    // display list for directory-select
    irc = GUI_file_symdir__ (s1Dir, sizeof(s1Dir));
    if(irc < 0) {
      // error or cancel in symdir__
        printf("## **** error symdir__ %d |%s|\n",irc,s1Dir);
      s1Dir[0] = '\0';
    }


    if(strlen(s1Dir) > 1) {
      // add filter or "*"
      // if(strcmp(par_flt, "NONE")) strcat (s1Dir, par_flt);
      strcat (s1Dir, "*");
      gtk_file_chooser_set_filename (GTK_FILE_CHOOSER(wfl1), s1Dir);
    }


    // gtk_widget_show (wfl1);
    gtk_widget_set_sensitive (wfl1, TRUE);

    goto L_wait;
  }



  //----------------------------------------------------------------
  gtk_widget_destroy (wfl1);

  // exit; return parameters
  return GUI_file_exit (sbuf1);

}


//================================================================
  int main (int argc, char *argv[]) {
//================================================================
// Input:
//   [1]   start-directory for file-selection; -> par_dir, sNam
//   [2]   filename of symbolic-directories    -> par_sym
//   [3]   filterText  (eg "*")                -> par_flt
//   [4]   window-title                        -> par_tit
// Output:
//   stdout     full selected filename; empty for Cancel;

  int        irc,i1;


  printf("***** start exe GUI_file V2.0 %d\n",argc);

  if(argc < 2) return (GUI_file_err1());


  for(i1=0; i1<argc; ++i1) printf("## GUI_file argv[%d]=|%s|\n",i1,argv[i1]);
  printf(" DEB_prt_stat = %d\n",DEB_prt_stat);


  gtk_disable_setlocale ();  // sonst Beistrich statt Decimalpunkt !! (LC_ALL)

  i1 = 0;
  gtk_init (&i1, NULL);

  fnIn = argv[1];

  // get filename fnOut from fnIn
  irc = GUI_file_out_in (fnOut, fnIn);

  // get parLst = content of file fnIn
  parLst = OS_wTab_file (&wTabSpc, fnIn);

  nArg = 0;
  while(parLst[nArg]) {
    printf("## %d = %s\n",nArg,parLst[nArg]);
    ++nArg;
  }

 
  if(!strcmp(parLst[0],"open"))  return GUI_open__ ();
  if(!strcmp(parLst[0],"save"))  return GUI_save__ ();

  gtk_main ();


  L_inpErr:
    printf("***** error input parameter %d .. \n",i1);
    return -1;


  return 0;
}


//================================================================
  int GUI_file_cancel (void *parent, void *data) {
//================================================================
/// \code
/// INTERNAL
/// list-window exiting; report to user ..
/// \endcode


  printd("## GUI_file_cancel\n");

  return (GUI_file_exit (""));

}


//================================================================
  int GUI_file_exit (char *sOut) {
//================================================================
// exit - provide selection to caller via stdout

  FILE    *fpo;


  printf("GUI_file_exit out  |%s|\n",sOut);
  printf("GUI_file_exit fn = |%s|\n",fnOut);


  if((fpo=fopen(fnOut,"w")) == NULL) {
    printf("***** Error GUI_file GUI_file_exit Open %s\n",fnOut);
    exit(1);
  }

  fprintf(fpo, "%s\n",sOut); 
  fclose(fpo);

  // gtk_widget_destroy (wfl1);

  // remove inputFile
  OS_file_delete (fnIn);

  exit(0);

}


//================================================================
  int GUI_file_err1 () {
//================================================================

  int   i1;

  printf("ERROR GUI_file error parameters\n");
  for(i1=0; i1<nArg; ++i1) printf("## GUI_file argv[%d]=|%s|\n",i1,parLst[i1]);
  exit(1);

}

 
// EOF
