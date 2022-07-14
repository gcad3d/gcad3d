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

#include "../ut/ut_os.h"          // OS_get_tmp_dir
// #include "../ut/deb_prt.h"          // printd


// activate printd
// #define printd printf
// disable printd
#define printd if(0) printf



/// FilenamedelimiterChar
#ifdef _MSC_VER
#define  fnam_del '\\'
#define  fnam_del_s "\\"
#else
#define  fnam_del '/'
#define  fnam_del_s "/"
#endif


// GLOBAL:
#ifdef _MSC_VER
static char *sGui = "gtk2_MS";      
#else
static char *sGui = "gtk2";      
#endif

int  nArg, fnID;
char **paArg;
char fnOut[512];
char title[512];
char *sDir, *fnSymDir, *sFilter, *sTit;

GtkWidget *wfl1;


  void UTX_CleanCR (char* string);
  int OS_filnam_eval (char *fno, char *fni, int fnoSiz);
  int UTX_fnam1__ (char* sDir, char* sNam, char* sIn);
  void UTX_CleanCR (char* string);
  

//================================================================
void TX_Error (char* txt, ...) { printf("%s\n",txt); }
// see also ../ut/ut_TX.c


//================================================================
void TX_Print (char* txt, ...) { printf("%s\n",txt); }
// see also ../ut/ut_TX.c


//================================================================
  int MSG_err_1 (char *key, char *fmt, ...) { printf("%s\n",key); }


//================================================================
  int MSG_get_1 (char *msg, int msgSiz, char *key, char *fmt, ...) {

  printf("%s\n",key); 

}


//=======================================================================
  int UTX_fgetLine (char *cbuf, int sizBuf, char *filNam, int lNr) {
//=======================================================================
/// \code
/// UTX_fgetLine           read line nr. <lNr> out of file <filNam>
///             first line has lineNr 1
/// sizBuf      size of cbuf
/// rc -1   File does not exist
/// rc -2   Line does not exist
/// rc  0   OK
/// \endcode


  int    i1;
  FILE   *fpi;

  // printf("UTX_fgetLine |%s| %d\n",filNam,lNr);


  if ((fpi = fopen (filNam, "r")) == NULL) {
    return -1;
  }

  i1=0;
  while (!feof (fpi)) {
    ++i1;
    if (fgets (cbuf, sizBuf, fpi) == NULL) goto Fertig;
    // printf(" ..--in |%s|\n",cbuf);
    if(i1 != lNr) continue;
    fclose(fpi);
    UTX_CleanCR (cbuf);
    // printf("ex UTX_fgetLine %d |%s|\n",lNr,cbuf);
    return 0;
  }
  Fertig:
  fclose(fpi);
  return -2;
}


///===========================================================
  void UTX_CleanCR (char* string) {
///===========================================================
/// UTX_CleanCR                  Delete Blanks, CR's u. LF's am Ende.
///   Ersetzung von Blanks, Carriage Returns und Linefeeds durch
///   Nullzeichen von hinten nach vorne bis zum ersten von diesen
///   verschiedenen Zeichen.
  

  int  ilen;
  char *tpos; 
    
  ilen = strlen (string);
    
  if(ilen < 1) goto L_exit;
    
  tpos = &string[ilen];
  --tpos;
  
  
  while ((*tpos  == ' ')  ||
         (*tpos  == '\t') ||          /* tab */
         (*tpos  == '\n') ||          /* newline */
         (*tpos  == '\r'))   {        /* CR */

    *tpos    = '\0';
    --tpos;
    if(tpos < string) break;
  }

  L_exit:
  // printf("ex UTX_CleanCR |%s|\n", string);

  return;
}


//================================================================
  char *UTX_find_strrchrn (char *cbuf, char *str) {
//================================================================
/// \code
/// UTX_find_strrchrn        find last occurence of one of the chars of str2
/// returns NULL or the position of the last char in cbuf also found in str. 
///  (see strpbrk = find first)
/// NULL: nicht enthalten
/// \endcode

  int    ii;
  char   *p1, *p2;

  // printf("UTX_find_strrchrn |%s|%s| \n",cbuf,str);


  ii = 0;
  p1 = cbuf;

  while(str[ii]) {
    p2 = strrchr (p1, str[ii]);
    if(p2) {
      // found
      p1 = ++p2;  // start here
    }
    ++ii;
  }

  if(p1) --p1;

  // printf("ex-UTX_find_strrchrn |%s| \n",p1);

  return p1;

}


//================================================================
  int UTX_fnam1__ (char* sDir, char* sNam, char* sIn) {
//================================================================
// UTX_fnam1__        separate/copy directory,fileName of full filename
// see  UTX_fnam__
// Output:
//   sDir       directory     size must be 256; including closing '/'
//   sNam       filename[.typ]     size must be 128

  int    sdl, snl;
  char   *pfn;

  sDir[0] ='\0';


  // printf("----------------------------------- \n");
  // printf("UTX_fnam1__ |%s|\n",sIn);

  // pfn = find last filename-delimiter
  // must check for '/' AND '\' (in MS '/' can come from out of source)
#ifdef _MSC_VER
  pfn = UTX_find_strrchrn(sIn, "/\\");
#else
  pfn = strrchr(sIn, fnam_del);
#endif
    printd(" fnam1__-pfn|%s|\n",pfn);


  // test if length of sDir > 256
  if((pfn - sIn) >= 256) return -1;


  if(!pfn) {
    // no directory;
    pfn = sIn;

  } else {
    // pfn = pos. of last '/'
    sdl =  pfn - sIn + 1;
    strncpy(sDir, sIn, sdl);
    sDir[sdl] = '\0';
    ++pfn;  // skip deli
  }

  // copy the filname
  snl = strlen(pfn);

  // test if length of sNam > 128
  if(snl >= 128) return -2;
  strcpy(sNam, pfn);

    // printf("ex-UTX_fnam1__ |%s|%s|\n",sDir,sNam);

  return 0;

}



//================================================================
  int GUI_update__ () {
//================================================================
/// update all windows

// Achtung: löscht events !

  printd("GUI_update__ \n");


  // Display zwischendurch updaten
  while (gtk_events_pending()) {
    gtk_main_iteration();
  }

  return 0;

}


//================================================================
  int GUI_file_symdir__ (char *sDir, int sSiz) {
//================================================================
// get symbolic-directory

  int    irc, il;
  char   s2[2048], s3[512], fnTmp[400], *binDir, *p1;
  FILE   *fpi;

  // get binDir
  binDir = getenv("gcad_dir_bin");
    printd(" GUI_file_symdir__-binDir |%s|\n",binDir);

  // filename outputFile
  sprintf (fnTmp, "%stemp_%d",OS_get_tmp_dir(),rand());

  // call GUI_dlg1_gtk2 list1
  // <binDir>/GUI_dlg1_gtk2 list1 <symListfile> title
#ifdef _MSC_VER
  sprintf(s2,
"CMD /C \"\"%sGUI_dlg1_%s\" list1 \"%s\" \"symbolic directory\" \"x40,y20\" \"%s\"\"",
          binDir,     sGui,       fnSymDir,                                  fnTmp);
#else
  sprintf(s2,"%sGUI_dlg1_%s list1 %s \"symbolic directory\" \"x40,y20\" %s",
          binDir,       sGui,  fnSymDir,                              fnTmp);
#endif
    printd(" GUI_file_symdir__ |%s|\n",s2);

  // irc = OS_sys1 (sDir, sSiz, s2);
  irc = OS_system (s2);
  if(irc) {printf("***** symdir__ - Error OS_sys1 %d\n",irc); return -1;}
  UTX_CleanCR (sDir);
    printd("## GUI_file_symdir__-in %d |%s|\n",irc,sDir);


  // read file <tmp>/tmp_<iRnd>, delete file, return filecontent:
  irc = UTX_fgetLine (sDir, sSiz, fnTmp, 1);
  OS_file_delete (fnTmp);


  //----------------------------------------------------------------
  // get path for symbol s1  -> s3
  if((fpi=fopen(fnSymDir,"r")) == NULL) {
    printf("***** symdir__ - Error Open E002 %s\n",fnSymDir);
    return -2;
  }

  il = strlen(sDir);
  while(fgets(s2, sizeof(s2), fpi) != NULL) {
    if(s2[0] == '#') continue;
    if(!strncmp(sDir,s2,il)) {
      p1 = strchr(s2, ' ');
      if(!p1) {printf("***** symdir__ - Error E003 %s\n",sDir); return -3;}
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


  //----------------------------------------------------------------
  // expand shell variables in filenames eg "${DIR_DEV}cadfiles/dxf/"
  irc = OS_filnam_eval (sDir, s3, sSiz);
     printd("##  symdir__-3 |%s|\n",sDir);



  //----------------------------------------------------------------
  // test if directory exists
  if((irc) || (!OS_checkFilExist(sDir,1))) {
    // directory sDir does not exist;
      printd("##  dir__ |%s| does not exist\n",sDir);


  // start GUI
#ifdef _MSC_VER
    sprintf(s2,"CMD /C \"%sGUI_dlg1_%s info \"ERROR - Directory %s does not exist\"\"",
            binDir, sGui, sDir);
#else
    sprintf(s2,"%sGUI_dlg1_%s info \"ERROR - Directory %s does not exist\"",
            binDir, sGui, sDir);
#endif
      printd("##  symdir__-4 |%s|\n",s2);

    // OS_sys1 (sDir, sSiz, s2);
    OS_system (s2);
    sDir[0] = '\0';
    return -5;
  }

    printd("##  ex-symdir__ |%s|\n",sDir);


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
  
  if(strlen(filename) < sizeof(fnOut)) strcpy(fnOut,filename);
  g_free (filename);


  //----------------------------------------------------------------
  gtk_widget_destroy (wfl1);
  
  // exit; return parameters
  return GUI_file_exit (fnOut);
    
}


//================================================================
 int GUI_file_cb_filt (void *parent, void *data) {
//================================================================
/// INTERNAL set filtertext for OPEN-Dialog

  int    i1;
  char   *txt, *all={"*"};
  GtkFileFilter *wfi1 = NULL;


  
  txt = (char*) gtk_entry_get_text (GTK_ENTRY(parent));


  printd("## GUI_file_cb_filt |%s|\n",sFilter);

  i1 = strlen(txt);
  if(strlen(txt) < 1) txt = all;

  // set filter
  wfi1 = gtk_file_filter_new ();
  gtk_file_filter_add_pattern (wfi1, txt);
  gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (wfl1), wfi1);

  return 0;

}


//================================================================
  int GUI_file_save__ () {
//================================================================

// file:///usr/share/gtk-doc/html/gtk3/GtkFileChooserDialog.html

  int       irc;
  char      s1Dir[256], *filename;
  GtkWidget *wb1, *we1, *wl1;
  char fDir[256], fNam[128];
  gint res;


  printd("## GUI_file_save__\n");

  // prepare filename, title
  if(nArg < 7)  return GUI_file_err1 ();
  sDir     = paArg[2];
  fnSymDir = paArg[3];
  sFilter  = paArg[4];
  sTit     = paArg[5];


  // separate sDir, sNam
  UTX_fnam1__ (fDir, fNam, sDir);
    printd("## _save__ fDir |%s| fNam |%s|  sDir |%s|\n",fDir,fNam,sDir);

  fnOut[0] = '\0';

  // prepare title
  sprintf(title, "%s %s",sTit,sDir);


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
  // gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (wfl1), sDir);
  // gtk_file_chooser_set_current_folder_uri  (GTK_FILE_CHOOSER (wfl1), sDir);


  // add filter- entry
  wb1 = gtk_hbox_new (FALSE, 0);    // gtk2 !
  // wb1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);   // gtk3 !

  we1 = gtk_entry_new ();
  gtk_container_add (GTK_CONTAINER (wb1), we1);
  g_signal_connect (G_OBJECT (we1),
                    "activate", G_CALLBACK (GUI_file_cb_filt), NULL);

  // if(sFilter) load into we1
  if(strcmp(sFilter, "NONE"))
    gtk_entry_set_text (GTK_ENTRY(we1), sFilter);

  wl1 = gtk_label_new ("  Filter");
  gtk_container_add (GTK_CONTAINER (wb1), wl1);

  // set filter as extra-widget
  gtk_widget_show (we1);
  gtk_widget_show (wl1);
  gtk_file_chooser_set_extra_widget (GTK_FILE_CHOOSER (wfl1), wb1);

  if(strcmp(sFilter, "NONE"))
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
    if(strlen(filename) < sizeof(fnOut)) strcpy(fnOut,filename);
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
      sprintf(title, "%s %s", sTit, s1Dir);
      gtk_window_set_title (GTK_WINDOW (wfl1), title);
      // add filter or "*"
      // if(strcmp(sFilter, "NONE")) strcat (s1Dir, sFilter);
      strcat (s1Dir, "*");
      gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER(wfl1), s1Dir);
    }


    // gtk_widget_show (wfl1);
    gtk_widget_set_sensitive (wfl1, TRUE);

    goto L_wait;
  }


  //----------------------------------------------------------------
  gtk_widget_destroy (wfl1);

  // exit; return parameters
  return GUI_file_exit (fnOut);

}


//================================================================
  int GUI_file_open__ () {
//================================================================

// file:///usr/share/gtk-doc/html/gtk3/GtkFileChooserDialog.html

  int       irc;
  char      s1Dir[256], *filename, s2[256];
  GtkWidget *wb1, *we1, *wl1;
  gint res;


  printd("## GUI_file_open__\n");

  // prepare filename, title
  if(nArg < 7)  return GUI_file_err1 ();
  sDir     = paArg[2];
  fnSymDir = paArg[3];
  sFilter  = paArg[4];
  sTit     = paArg[5];


  //----------------------------------------------------------------
  fnOut[0] = '\0';
 
  wfl1 = gtk_file_chooser_dialog_new ("Open File",
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

  strcpy(s1Dir, sDir);
  strcat(s1Dir, "*");
  gtk_file_chooser_set_filename (GTK_FILE_CHOOSER(wfl1), s1Dir);
  // gtk_file_chooser_set_current_folder (..);

  // if(sFilter) load into we1
  if(strcmp(sFilter, "NONE"))
    gtk_entry_set_text (GTK_ENTRY(we1), sFilter);

  wl1 = gtk_label_new ("  Filter");
  gtk_container_add (GTK_CONTAINER (wb1), wl1);


  gtk_widget_show (we1);
  gtk_widget_show (wl1);

  gtk_file_chooser_set_extra_widget (GTK_FILE_CHOOSER (wfl1), wb1);

  if(strcmp(sFilter, "NONE"))
    GUI_file_cb_filt (we1, NULL);

  // gtk_window_set_transient_for (GTK_WINDOW(wfl1),  NULL);
  // gtk_window_set_modal (GTK_WINDOW (wfl1), TRUE);


  // wait for user-select
  L_wait:
  res = gtk_dialog_run (GTK_DIALOG (wfl1));
    printd(" f-dialog_run %d\n",res);


  if (res == 0) {
    // cancel
    return (GUI_file_exit (""));


  } else if (res == 1) {
    // Open ..
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(wfl1);
    filename = gtk_file_chooser_get_filename (chooser);
    if(!filename) goto L_wait;
    if(strlen(filename) < sizeof(fnOut)) {
        printd("## chooser-get |%s|\n",filename);
      strcpy(fnOut,filename);
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
        printd("## **** error symdir__ %d |%s|\n",irc,s1Dir);
      s1Dir[0] = '\0';
    }


    if(strlen(s1Dir) > 1) {
      // add filter or "*"
      // if(strcmp(sFilter, "NONE")) strcat (s1Dir, sFilter);
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
  return GUI_file_exit (fnOut);

}


//================================================================
  int main (int argc, char *argv[]) {
//================================================================
// Input:
//   argv[i1]   start-directory for file-selection; -> sDir, sNam
//   argv[i2]   filename of symbolic-directories    -> fnSymDir
//   argv[i3]   filterText  (eg "*")                -> sFilter
//   argv[i4]   window-title                        -> sTit
// Output:
//   stdout     full selected filename; empty for Cancel;


  int        i1;

  paArg = argv;
  nArg = argc;

  if(nArg < 3) return (GUI_file_err1());


  printd("***** start exe GUI_file V1.0 ..\n");
  for(i1=0; i1<argc; ++i1) printd("## GUI_file argv[%d]=|%s|\n",i1,argv[i1]);


  gtk_disable_setlocale ();  // sonst Beistrich statt Decimalpunkt !! (LC_ALL)

  i1 = 0;
  gtk_init (&i1, NULL);

  if(!strcmp(argv[1],"open"))  return GUI_file_open__ ();
  if(!strcmp(argv[1],"save"))  return GUI_file_save__ ();

  gtk_main ();

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

//   printf("%s\n", sOut);  // to provide to caller via stdout
//   fflush(stdout);

  FILE    *fpo;

  printd("exit-GUI_file_exit |%s|\n",sOut);


  if((fpo=fopen(paArg[6],"w")) == NULL) {
    printf("***** symdir__ - Error GUI_file GUI_file_exit Open %s\n",paArg[6]);
    exit(1);
  }

  fprintf(fpo, "%s\n",sOut); 
  fclose(fpo);

  // gtk_widget_destroy (wfl1);

  exit(0);

}


//================================================================
  int GUI_file_err1 () {
//================================================================

  int   i1;

  printf("ERROR GUI_file error parameters\n");
  for(i1=0; i1<nArg; ++i1) printf("## GUI_file argv[%d]=|%s|\n",i1,paArg[i1]);

  exit(1);

}

 
// EOF
