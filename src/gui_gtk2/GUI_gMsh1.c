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
Select file for open, write controlfile for plugin gMsh1.
All testoutput to stdout must be preceded with "##"
-----------------------------------------------------
TODO:

-----------------------------------------------------
Modifications:
2020-01-10 Created. RF.

=============================================================================
Using:  see INF_GUI_dlg1   (../inf/../inf/GUI.c)


=============================================================================
// ReBuild:
. ../options.sh && make -f GUI_dlg1.mak


// ReBuild and test:
./GUI_gMsh1.sh

// debug:
gdb /home/fwork/devel/bin/gcad3d/Linux_x86_64/GUI_gMsh1_gtk2
run


../APP/gMsh1.c                     plugin
../gui_gtk2/GUI_gMsh1.c            GUI-exe
../myGIS1/gMsh__.c                 mesh-exe


Startparameters:
1. par = filename info-file;

info-file:    1. line = filename controlfile; Input;
              2. line = user-selection (mesh|edit|cancel ..)


*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>              // for ...

#include <gtk/gtk.h>

#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_txTab.h"              // TxtTab
#include "../ut/deb_prt.h"             // printd
#include "../ut/ut_os.h"               //


// #include "../gui/gui__.h"


// GLOBAL:
// static char *sGui = "gtk2";
int        nArg, upState=0, iEnt=0, lEnt=64;
GtkWidget  *wEnt;


GtkWidget    *UI_win=NULL;
GtkWidget    *we_clf, *wl_err, *wr_gc, *wr_dxf;
GtkWidget    *wl_pt, *wl_bl, *wl_ib, *wl_ob;
GtkWidget    *wc_fac, *wc_pt, *wc_bl, *wc_bnd;
GtkWidget    *wc_fCen, *wc_blIn, *wc_bndIn;

char         **paArg;
char         clfNam[400],       // commandfilename
             fnTmp[400],        // filename info-file
             fnOut[400],        // aux
             fnIb[8000];        // inner-boundaries


UtxTab_NEW (txTab1);                // stringtable

  char *AP_bin_dir;         // dir binaries





//================================================================
// prototypes:
  int GUI_dlg1_cancel (void *parent, void *data);



//================================================================
// dummy-funcs:
DEB_dump_nobj__   ()  {};
DEB_dump_ox_s_    ()  {};




//================================================================
//================================================================
//================================================================
  char* AP_get_bas_dir () {
//================================================================
/// AP_get_bas_dir           returns path of ?


  char     *p1;

#if defined _MSC_VER || __MINGW64__
  p1 = getenv("APPDATA");
#else
  p1 = getenv("HOME");
#endif

  return p1;
  
} 



//================================================================
//================================================================
//================================================================
// ../ut/ut_os__.c


//================================================================
  int OS_ck_DirAbs (char *fNam) {
//================================================================
// check if string is absolute or relative Filname
// Returncodes:
//   0  = yes, absolut
//   1  = no, relativ ..

// see also OS_dirAbs_fNam

// Tests:
// /dir/fn          abs
// ./fn             rel
// ../dir/fn        rel
// dir/fn           rel
// C:\xx            abs-MS
// \xx\yy           abs-MS


  UTX_pos_skipLeadBlk (fNam);

  if(fNam[0] == '/') return 0;


#if defined _MSC_VER || __MINGW64__
  if(fNam[0] == '\\') return 0;
  if(fNam[1] == ':') return 0;
#endif


  return 1;

}


// //================================================================
//   char* OS_get_tmp_dir() {
// //================================================================
// // OS_get_tmp_dir                get directory for temporary files
// // - must end with "/" or (MS-Win) "\"
// 
// 
// 
// #ifdef _MSC_VER
//   static  char os_tmp_dir[256] = "\0\0", *p1;
// 
// 
//   if(!os_tmp_dir[0]) {
//     p1 = getenv ("TEMP");           // %TEMP%
//     strcpy(os_tmp_dir, p1);
//     UTX_add_fnam_del (os_tmp_dir);
// 
//       // TESTBLOCK
//       // printf(" OS_get_tmp_dir |%s|\n",os_tmp_dir);
//       // END TESTBLOCK
//   }
// 
//   return os_tmp_dir;
// 
// #else
//   static  char u_tmp_dir[] = "/tmp/";
// 
//   return u_tmp_dir;
// #endif
// 
// }



//================================================================
//================================================================
//================================================================
// ../ut/ut_txt.c


//================================================================
  int UTX_add_fnam_del (char *cbuf) {
//================================================================
/// \code
/// UTX_add_fnam_del       add closing "/" or "\\" to string (filename-delimiter)
/// see also UTX_endAddChar
/// \endcode


  char *ps;

  // wenn cbuf kein closing "/" hat, eins zufuegen.
  ps = &cbuf[strlen(cbuf)-1];

#if defined _MSC_VER || __MINGW64__
  // MS: CR-LF !
  if(*ps == '\r') --ps;
#endif


  if(*ps != fnam_del) strcat(cbuf, fnam_del_s);


  return 0;

}


//================================================================
  int UTX_fdir_s (char *fdir, char *cbuf) {
//================================================================
// UTX_fdir_s        get fileDirectory from string
// last char ist immer der FilenamedelimiterChar !
// alles vor dem letzten FilenamedelimiterChar fnam_del ist fileDir;
// Wenn erster Char ist fnam_del: absolutes fileDir; else relativ.
// relativ: das pwd (os_bas_dir) vorne weg ...

  int    i1;
  char   *p1, *p2;


  // printf("UTX_fdir_s |%s|\n",cbuf);

  // skip 1. char if it is |"|
  if(*cbuf == '"') ++cbuf;


  // absoluter oder relativer Filename ?
  if(cbuf[0] == '\\') goto L_abs;     // zB "\\ooserv\...."
  if(cbuf[0] == '/')  goto L_abs;     // zB "/xx/yy/fn"

#if defined _MSC_VER || __MINGW64__
  if(cbuf[1] == ':')  goto L_abs;
#endif

  L_rel: // relatives verzeichnis
    strcpy (fdir, AP_get_bas_dir ());
    p1 = cbuf;
    if((*p1 == '.')&&(*(p1+1) == fnam_del)) p1 += 2;  // skip "./"
    strcat (fdir, p1);
    goto L_cut;


  L_abs: // absolut (kompletter) filename
    strcpy (fdir, cbuf);


  L_cut:     // remove Filename
  // printf("      fnam tot=|%s|\n",fdir);

  p1 = strrchr(fdir, '/');  // find last FilenamedelimiterChar
  p2 = strrchr(fdir, '\\');  // find last FilenamedelimiterChar

  if(p1 == NULL) p1 = fdir;
  else ++p1;         // FilenamedelimiterChar soll last char bleiben

  if(p2 == NULL) p2 = fdir;
  else ++p2;         // FilenamedelimiterChar soll last char bleiben

  if(p2 > p1) p1 = p2;

  *p1 = '\0';     // cut off filename

  // printf("ex UTX_fdir_s 0 |%s|%s|\n",fdir,cbuf);

  return 0;

  L_err:
  // printf("ex UTX_fdir_s -1 |%s|\n",cbuf);
  return -1;
}


//================================================================
  int UTX_fnam_rem_dirLast (char *sDir) {
//================================================================
// UTX_fnam_rem_dirLast        remove last dir 
// Example: "/actDir1/actDir2/" -> "/actDir1/"


  int    irc, sl;
  char   *pfn;

  // printf("UTX_fnam_rem_dirLast |%s|\n",sDir);

  // remove last directory-delimiter
  sl = strlen(sDir);
  --sl;
  if(sl < 1) {irc = -1; goto L_exit;}
  if(sDir[sl] == fnam_del) {
    sDir[sl] = '\0';
    --sl;
  }

  // pfn = find last filename-delimiter
  // must check for '/' AND '\' (in MS '/' can come from out of source)
#if defined _MSC_VER || __MINGW64__
  pfn = UTX_find_strrchrn(sDir, "/\\");
#else
  pfn = strrchr(sDir, fnam_del);
#endif

  if(pfn) {++pfn; *pfn = '\0';}

  irc = 0;

  L_exit:
    // printf("ex-UTX_fnam_rem_dirLast %d |%s|\n",irc,sDir);
  return irc;

}


//================================================================
  int UTX_ffNam_fNam (char *ffNam, char *fnIn, char *actDir) {
//================================================================
// UTX_ffNam_fNam       get full-filename from (unknown) filename
// Input:
//   actDir     pwd; used for relative path; must have closing '/'
// Output:
//   ffNam      size must be >= sMaxLen = 400
// Test with: 
// UTX_ffNam_fNam (s2, "../../../xy/dev.dat","/actDir1/actDir2/");
// see also MDLFN_ffNam_fNam

#define sMaxLen 400

  int    irc, i1, ls;

  printf("## UTX_ffNam_fNam |%s|%s|\n",fnIn,actDir);

  // check for absolute path
  if(!OS_ck_DirAbs(fnIn)) {strcpy(ffNam, fnIn); goto L_exit;}


  //----------------------------------------------------------------
  // relative path -> full-filename
  ls = strlen(actDir);
  if(ls >= sMaxLen) goto L_err;
  strcpy (ffNam, actDir);
  // clear follwing blanks
  L_remBlk:
  i1=strlen(ffNam); if(ffNam[i1] == ' ') {ffNam[i1] = '\0'; goto L_remBlk;}
    printf("## UTX_ffNam_fNam ffNam |%s|\n",ffNam);

  if(fnIn[0] != '.') {
    // eg ("act.dat", "/actDir/"); add actDir+fnIn
    if(ls + strlen(fnIn) >= sMaxLen) goto L_err;
    strcat(ffNam, fnIn);
    goto L_exit;
  }

  if(fnIn[1] != '.') {
    // eg ("./dev.dat", "/actDir/"); remove "./", add actDir+fnIn
    if(ls + strlen(fnIn) - 2 >= sMaxLen) goto L_err;
    strcat(ffNam, &fnIn[2]);
    goto L_exit;
  }

  // eg ("../xy/dev.dat","/actDir1/actDir2/"); for each "../" remove last dir of actDir
  L_nxtRem:
      // printf(" L_nxtRem: |%s|%s|\n",ffNam,fnIn);
    irc = UTX_fnam_rem_dirLast (ffNam);  // skip last part of dir
    if(irc < 0) {
        // printf(" L_nxtRem_-1: |%s|%s|\n",ffNam,fnIn);
      strcpy(ffNam, fnIn);
      goto L_exit;
    }
    fnIn += 3;  // skip "../"
    if(!strncmp(fnIn, "../",3)) goto L_nxtRem;
    if(strlen(fnIn) + strlen(ffNam) >= sMaxLen) goto L_err;
    strcat(ffNam, fnIn);

  //----------------------------------------------------------------
  L_exit:
    // printf(" ex-MDLFN_ffNam_fNam |%s|\n\n",ffNam);
  return 0;


  L_err:
    TX_Error ("UTX_ffNam_fNam - string too long");
    return -1;

}



//================================================================
//================================================================
//================================================================
// ../gui_gtk2/gtk_base.c
 

//================================================================
//================================================================
//================================================================
 

//================================================================
  int gmsh1_error (char *eTxt) {
//================================================================

  printf("## gmsh1_error |%s|\n",eTxt);

  gtk_label_set_text ((GtkLabel*)wl_err, eTxt);

  return -1;

}


//================================================================
  char* gmsh1_load_setenv (FILE *fpi, char *s1) {
//================================================================
// load all comment setenv logfile outfile records;
// s1 = size 256
 
  int      irc;
  char     *p1, *p2;

  // printf("## gmsh1_load_setenv |%s|\n",s1);

  if(!txTab1.iNr) UtxTab_init__ (&txTab1);            // init (malloc ..)



  L_nxt_ln:
    p1 = fgets(s1, 250, fpi);
    if(p1 == NULL) {
      TX_Print ("****** READ ERROR gmsh1_load_setenv");
      p1 = NULL;
      goto L_exit;
    }

    UTX_CleanCR (p1);
    UTX_pos_skipLeadBlk (p1);        // find 1. nonblank char
    if(!strlen(p1)) goto L_nxt_ln; // skip empty line

    if((*p1 == '#')                    ||
       (!strncmp(p1, "setenv ", 7))    ||
       (!strncmp(p1, "logfile ", 8))   ||
       (!strncmp(p1, "outfile ", 8))) {
        // printf("## gmsh1_load_setenv add |%s|\n",s1);
      UtxTab_add (&txTab1, s1);

//       if     (!strcmp(p1, "outfile")) {
//         // get filetype; dxf or gcad or dxf
//         p2 = strrchr(p1, '.');
//         if((p2)&&(strlen(p2) == 4))
//           gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wr_dxf), TRUE);
//       }

      goto L_nxt_ln;
    }



  L_exit:
    // printf("## gmsh1_load_setenv exit |%s|\n",s1);
  return p1;

}


//================================================================
  int gmsh1_loadfile () {
//================================================================
// load content of selected controlfile
// read file clfNam, set checkboxes and labels.

  int   irc;
  char  *p1, *p2, *p3, s1[256], s2[400];
  FILE  *fpi;


  printf("## gmsh1_loadfile |%s|\n",clfNam);

  UtxTab_init__ (&txTab1);  // reset
  fnIb[0] = '\0';

  // clear errorLabel
  gmsh1_error ("-");

  // open commandfile
  if((fpi=fopen(clfNam,"r")) == NULL) {
    gmsh1_error ("****** OPEN ERROR commandfile **********");
    TX_Print ("****** OPEN ERROR commandfile **********");
    return irc;
  }


  // clear all filenames
  gtk_button_set_label ((GtkButton*)wl_pt, "-");
  gtk_button_set_label ((GtkButton*)wl_bl, "-");
  gtk_button_set_label ((GtkButton*)wl_ib, "-");
  gtk_button_set_label ((GtkButton*)wl_ob, "-");

  // clear all checkboxes ..
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wr_dxf), FALSE);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wc_fac), TRUE);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wc_pt), FALSE);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wc_bl), FALSE);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wc_blIn), FALSE);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wc_bnd), FALSE);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wc_bndIn), FALSE);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wc_fCen), FALSE);


  // load all comment setenv logfile outfile records;
  p1 = gmsh1_load_setenv (fpi, s1);
  if(!p1) return -2;
  // returns with first record loaded
  goto L_sep;


  //----------------------------------------------------------------
  L_nxt_ln:
    p1 = fgets(s1, 250, fpi);
    if(p1 == NULL) { irc = 0; goto L_done; }

    UTX_pos_skipLeadBlk (p1);        // find 1. nonblank char
    if(*p1 == '#') goto L_nxt_ln;    // skip comment
    UTX_CleanCR (p1);
    if(!strlen(p1)) goto L_nxt_ln; // skip empty line

    L_sep:
    // separate all words
    p1 = strtok (p1, " ");
      // printf(" p1 = |%s|\n",p1);
    p2 = strtok (NULL, " ");
    if(p2) p3 = strtok (NULL, " ");

      // printf("## gmsh1_loadfile sep |%s|%s|\n",p1,p2);

  if     (!strcmp(p1, "outfmt")) {
    if(!strcmp(p2, "dxf")) 
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wr_dxf), TRUE);
  }
  else if(!strcmp(p1, "ms_pt"))
    gtk_button_set_label ((GtkButton*)wl_pt, p2);

  else if(!strcmp(p1, "ms_bl"))
    gtk_button_set_label ((GtkButton*)wl_bl, p2);

  else if(!strcmp(p1, "ms_ib")) {if(p2){strcat(fnIb, p2); strcat(fnIb, "\n");}}

  else if(!strcmp(p1, "ms_ob"))
    gtk_button_set_label ((GtkButton*)wl_ob, p2);
  // else if(!strcmp(p1, "pt_def_id"))   irc = pt_def_id (p2);
  // else if(!strcmp(p1, "ms_def_id"))   irc = ms_def_id (p2);
  // else if(!strcmp(p1, "ms_write"))    goto L_nxt_ln;   // ignore; do at end ..

  // else if(!strcmp(p1, "gc_view"))     irc = disp__ (p2);
  else if(!strcmp(p1, "no_disp_fac"))    
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wc_fac), FALSE);
  else if(!strcmp(p1, "disp_pts"))    
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wc_pt), TRUE);
  else if(!strcmp(p1, "disp_bl"))     
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wc_bl), TRUE);
  else if(!strcmp(p1, "disp_blIn"))   
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wc_blIn), TRUE);
  else if(!strcmp(p1, "disp_bd"))     
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wc_bnd), TRUE);
  else if(!strcmp(p1, "disp_bdIn"))   
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wc_bndIn), TRUE);
  else if(!strcmp(p1, "disp_fce"))    
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wc_fCen), TRUE);
  // else if(!strcmp(p1, "disp_mdl"))    disp_mdl   = 1;

  // undocumented testfunctions:
  // else if(!strcmp(p1, "dump_fp1"))    irc = dump_fp1 (p2);
  // else if(!strcmp(p1, "test_ex"))     irc = -99;
  else {
    sprintf(s2, "COMMAND %s",p1);
    TX_Print("***** Error %s",s2);
  }

  goto L_nxt_ln;


  //----------------------------------------------------------------
  L_done:
  fclose (fpi);

  if(fnIb[0]) {
    // remove last \n
    p1 = strrchr(fnIb, '\n');
    if(p1) *p1 = '\0';
      // printf(" loadfile-ib=|%s|\n",fnIb);
    gtk_button_set_label ((GtkButton*)wl_ib, fnIb);
  }


  return 0;

}


//================================================================
  int gmsh1_writefile () {
//================================================================
// write active state of menu into file

  int   irc, i1;
  char  *p1, *p2, fn[400], s1[425];
  FILE  *fpo;
  // char  *p1, *p2, *p3, s1[256], fn[400];

  printf("## gmsh1_writefile\n");


    // get filename
    p1 = gtk_entry_get_text ((GtkEntry*)we_clf);
    strcpy(fn, p1);
    if(strlen(fn) < 3) {
      gmsh1_error ("ERROR - no commandfilename");
      irc = -1;
      goto L_exit;
    }

    // make absolute filename from unknown filename
    UTX_ffNam_fNam (clfNam, fn, p1);

    sprintf(s1, "%s_old",clfNam);
    // remove (clfNam);
    // OS_file_rename (clfNam, s1);
    rename (clfNam, s1);

    if((fpo=fopen(clfNam, "w")) == NULL) {
      sprintf(s1, "****** OPEN ERROR FILE %s \n",clfNam);
      gmsh1_error(s1);
      irc = -1;
      goto L_exit;
    }


    //----------------------------------------------------------------
    // write setenv-records
    // printf("## L_mesh-txTab1.iNr = %d\n",txTab1.iNr);
    if(txTab1.iNr) {
      for(i1=0; i1 < txTab1.iNr; ++i1) {
        // printf("## L_mesh-txTab1 %d %d\n",i1,UtxTab__(i1,&txTab1));
        fprintf(fpo,"%s\n",UtxTab__(i1,&txTab1));
      }
    }


    i1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wc_fac));
    if(!i1) fprintf(fpo,"no_disp_fac\n");

    i1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wc_pt));
    if(i1) fprintf(fpo,"disp_pts\n");

    i1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wc_bl));
    if(i1) fprintf(fpo,"disp_bl\n");

    i1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wc_bnd));
    if(i1) fprintf(fpo,"disp_bd\n");

    i1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wc_fCen));
    if(i1) fprintf(fpo,"disp_fce\n");

    i1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wc_blIn));
    if(i1) fprintf(fpo,"disp_blIn\n");

    i1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wc_bndIn));
    if(i1) fprintf(fpo,"disp_bdIn\n");


    //----------------------------------------------------------------
    // i1 = GTK_TOGGLE_BUTTON (wr_gc)->active;
    i1 = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(wr_gc));
//     if(i1) fprintf(fpo,"outfile %sactMdl.gcad\n",OS_get_tmp_dir());
//     else   fprintf(fpo,"outfile %sactMdl.dxf\n",OS_get_tmp_dir());
    if(i1) fprintf(fpo,"outfmt gcad\n");
    else   fprintf(fpo,"outfmt dxf\n");

    //----------------------------------------------------------------
    p1 = gtk_button_get_label ((GtkButton*)wl_pt);
    if(strlen(p1) > 3) fprintf(fpo,"ms_pt %s\n",p1);

    p1 = gtk_button_get_label ((GtkButton*)wl_bl);
    if(strlen(p1) > 3) fprintf(fpo,"ms_bl %s\n",p1);

    // ib
    p1 = gtk_button_get_label ((GtkButton*)wl_ib);
    if(strlen(p1) > 3) {
      strcpy(fnIb, p1);
      p1 = fnIb;
        // printf(" wl_ib=|%s|\n",p1);
      L_nxt_ib:
        p2 = strchr (p1, '\n');
        if(p2) {
          *p2 = '\0';
          fprintf(fpo,"ms_ib %s\n",p1);
          p1 = p2;
          ++p1;
          goto L_nxt_ib;
        }
        fprintf(fpo,"ms_ib %s\n",p1);
    }

    p1 = gtk_button_get_label ((GtkButton*)wl_ob);
    if(strlen(p1) > 3) fprintf(fpo,"ms_ob %s\n",p1);



    fprintf(fpo,"# eof gMsh1 cmd %s\n",clfNam);
    fclose (fpo);

    irc = 0;

  L_exit:
    return irc;

}


//================================================================
 int GUI_cb_clf_sel (void *parent, void *data) {
//================================================================
//  callback double-click on controlfile
  
  char    *filename;
  
  printf("## GUI_cb_clf_sel \n");
  
  // GtkFileChooser *chooser = GTK_FILE_CHOOSER(wfl1);
  filename = gtk_file_chooser_get_filename (parent);
  printf("##  fn |%s|\n",filename);
  
  if(strlen(filename) < sizeof(fnOut)) strcpy(fnOut,filename);
  g_free (filename);

  
  //----------------------------------------------------------------
  gtk_widget_destroy (parent);
  
  // exit; return parameters
  // return GUI_file_exit (fnOut);
  return 0;

}
  

//================================================================
 int GUI_cb_edi__ (void *parent, void *data) {
//================================================================
//  callback edit controlfile <clfNam>

  printf("## GUI_cb_edi__ %s\n",clfNam);

  // write active state of menu into file
  gmsh1_writefile ();

  OS_edit__ (clfNam); 

  // reRead
  gmsh1_loadfile ();

  // update GUI
  GUI_update__ ();

  return 0;

}


//================================================================
 int GUI_cb_clf__ (void *parent, void *data) {
//================================================================
//  callback select controlfile

  GtkWidget       *wfc;
  gint            res;
  GtkFileFilter   *wfi1 = NULL;
  char            sDir[400];


  printf("## GUI_cb_clf__ \n");

  fnOut[0] = '\0';
  // strcpy(wTyp, (char*)data);
  
  // get filename
  wfc = gtk_file_chooser_dialog_new ("select controlfile",
                                    (GtkWindow*)UI_win,         // parent_window
                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                    NULL,
                                    NULL);

  // copy directory, filename
  // gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (wfc), clfNam);
  strcpy(sDir, clfNam);
  UTX_fnam_rem_dirLast (sDir);
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (wfc), sDir);

  // set filter
  wfi1 = gtk_file_filter_new ();
  gtk_file_filter_add_pattern (wfi1, "*.gmsh1");
  gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (wfc), wfi1);


  // connect Enter / double-click
  g_signal_connect (G_OBJECT (wfc),
                    "file-activated", G_CALLBACK (GUI_cb_clf_sel), NULL);

  // wait for user-select; -1 = double-click or CR; -4 = Cancel
  res = gtk_dialog_run (GTK_DIALOG (wfc));
    printf("## GUI_cb_clf__-sel %d\n",res);

  // Cancel - delete last
  if((res == 0) || (res == -4)) {
    strcpy(fnOut, "-");
    goto L_exit;
  }
  
  // gtk_label_set_text ((GtkLabel*)w1, fnOut);
  if(fnOut[0]) {
      printf("## GUI_cb_clf__-fnOut |%s|\n",fnOut);
    strcpy(clfNam, fnOut);
    // display selected file
    gtk_entry_set_text ((GtkEntry*)we_clf, clfNam);
    // load content of selected controlfile
    gmsh1_loadfile ();
  }


  L_exit:
    return 0;

}


//================================================================
 int GUI_cb_bt_sel (void *parent, void *data) {
//================================================================
//  callback double-click on file ..

  char    *filename;

  printf("## GUI_cb_bt_sel \n");

  // GtkFileChooser *chooser = GTK_FILE_CHOOSER(wfl1);
  filename = gtk_file_chooser_get_filename (parent);
  printf("## fn |%s|\n",filename);

  if(strlen(filename) < sizeof(fnOut)) strcpy(fnOut,filename);
  g_free (filename);


  //----------------------------------------------------------------
  gtk_widget_destroy (parent);

  // exit; return parameters
  // return GUI_file_exit (fnOut);
  return 0;

}


//================================================================
  int GUI_cb_bt__ (void *parent, void *data) {
//================================================================
// callback button

static GtkWidget  *w1 = NULL;
static char       wTyp[8] = "";
       GtkWidget  *wfc;
  int             irc, i1;
  gint            res;
  char            *p1, *p2, fDir[440];
  FILE            *fpo;


  printf("## GUI_cb_bt__ |%s|\n",(char*)data);

  if     (!strcmp("canc", (char*)data)) goto L_canc;
  else if(!strcmp("mesh", (char*)data)) goto L_mesh;
  else if(!strcmp("view", (char*)data)) goto L_view;
  else if(!strcmp("help", (char*)data)) goto L_help;
  else if(!strcmp("undo", (char*)data)) {
       if(!strcmp("w_ib", wTyp))        goto L_undo_ib;
       else                             goto L_undo__;
  }


  fnOut[0] = '\0';
  strcpy(wTyp, (char*)data);

  // get fDir = directory of controlfile
  irc = UTX_fdir_s (fDir, clfNam);
    printf("## GUI_cb_bt__-fDir |%s|\n",fDir);
  

  // get filename
  wfc = gtk_file_chooser_dialog_new ("select / clear file",
                                     (GtkWindow*)UI_win,         // parent_window
                                     GTK_FILE_CHOOSER_ACTION_OPEN,
                                     ("_Clear"),  1,
                                     NULL);
  // copy directory, filename
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (wfc), fDir);

//   gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (wfl1), fNam);
//   // gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (wfl1), sDir);
//   // gtk_file_chooser_set_current_folder_uri  (GTK_FILE_CHOOSER (wfl1), sDir);
// see ../gui_gtk3/GUI_file.c  file_chooser

  // connect Enter / double-click
  g_signal_connect (G_OBJECT (wfc),
                    "file-activated", G_CALLBACK (GUI_cb_bt_sel), NULL);


  // wait for user-select; -1 = double-click or CR; -4 = Cancel
  res = gtk_dialog_run (GTK_DIALOG (wfc));
    printf("## GUI_cb_bt__-sel %d\n",res);


  if     (!strcmp("w_pt", (char*)data)) w1 = wl_pt;
  else if(!strcmp("w_bl", (char*)data)) w1 = wl_bl;
  else if(!strcmp("w_ib", (char*)data)) {w1 = wl_ib; goto L_IB;}
  else if(!strcmp("w_ob", (char*)data)) w1 = wl_ob;

  // Cancel - delete last
  if(res == 1) strcpy(fnOut, "-");

  // gtk_label_set_text ((GtkLabel*)w1, fnOut);
  if(fnOut[0]) gtk_button_set_label ((GtkButton*)w1, fnOut);

  if(wfc) {gtk_widget_destroy (wfc); wfc = NULL;}

  return 0;


  //----------------------------------------------------------------
  L_IB:
    // add selection to list of inner-bounds
    printf("## GUI_cb_bt__-Ib:\n");
    if(res == 1) goto L_undo_ib;
    i1 = strlen(fnIb);
    if(i1 > 1) strcat(fnIb,"\n");
    else       fnIb[0] = '\0';
    strcat(fnIb, fnOut);
    goto L_set_ib;


  //----------------------------------------------------------------
  L_undo__:
    // undo (clear) last selection
    if(w1) gtk_button_set_label ((GtkButton*)w1, "-");
    return 0;


  //----------------------------------------------------------------
  L_undo_ib:
    // printd("## GUI_cb_bt__-undo_ib |%s|\n",fnIb); makes printf ! ?
    // undo (clear) last ib
    p1 = strrchr(fnIb, '\n');
    if(p1) *p1 = '\0';
    else strcpy(fnIb, "-");


  //----------------------------------------------------------------
  L_set_ib:
    if(w1) {
      w1 = wl_ib;
        // printd("## GUI_cb_bt__-L_set_ib: |%s|\n",fnIb); makes printf ! ?
      gtk_button_set_label ((GtkButton*)w1, fnIb);
    }
    if(wfc) {gtk_widget_destroy (wfc); wfc = NULL;}
    return 0;


  //================================================================
  L_help:
   return GUI_dlg1_exit ("__viewHelp__");


  //================================================================
  L_view:
   return GUI_dlg1_exit ("__viewLog__");


  //================================================================
  L_mesh:
    // write file clfNam
    gmsh1_writefile ();

/*
    printd("## mesh %s\n",clfNam);

    // get filename
    p1 = gtk_entry_get_text ((GtkEntry*)we_clf);
    strcpy(fn, p1);
    if(strlen(fn) < 3) return gmsh1_error ("ERROR - no commandfilename");


    // make absolute filename from unknown filename
    UTX_ffNam_fNam (clfNam, fn, paArg[1]);

    remove (clfNam);

    if((fpo=fopen(clfNam, "w")) == NULL) {
      sprintf(fn, "****** OPEN ERROR FILE %s \n",clfNam);
      gmsh1_error(fn);
      return -1;
    }




    //----------------------------------------------------------------
    // write setenv-records
    // printd("## L_mesh-txTab1.iNr = %d\n",txTab1.iNr);
    if(txTab1.iNr) {
      for(i1=0; i1 < txTab1.iNr; ++i1) {
        // printd("## L_mesh-txTab1 %d %d\n",i1,UtxTab__(i1,&txTab1));
        fprintf(fpo,"%s\n",UtxTab__(i1,&txTab1));
      }
    }


    i1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wc_fac));
    if(!i1) fprintf(fpo,"no_disp_fac\n");

    i1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wc_pt));
    if(i1) fprintf(fpo,"disp_pts\n");

    i1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wc_bl));
    if(i1) fprintf(fpo,"disp_bl\n");

    i1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wc_bnd));
    if(i1) fprintf(fpo,"disp_bd\n");

    i1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wc_fCen));
    if(i1) fprintf(fpo,"disp_fce\n");

    i1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wc_blIn));
    if(i1) fprintf(fpo,"disp_blIn\n");

    i1 = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wc_bndIn));
    if(i1) fprintf(fpo,"disp_bdIn\n");


    //----------------------------------------------------------------
    // i1 = GTK_TOGGLE_BUTTON (wr_gc)->active;
    i1 = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(wr_gc));
    if(i1) fprintf(fpo,"outfmt gcad\n");
    else   fprintf(fpo,"outfmt dxf\n");


    //----------------------------------------------------------------
    p1 = gtk_button_get_label ((GtkButton*)wl_pt);
    if(strlen(p1) > 3) fprintf(fpo,"ms_pt %s\n",p1);

    p1 = gtk_button_get_label ((GtkButton*)wl_bl);
    if(strlen(p1) > 3) fprintf(fpo,"ms_bl %s\n",p1);

    // ib
    p1 = gtk_button_get_label ((GtkButton*)wl_ib);
    if(strlen(p1) > 3) {
      strcpy(fnIb, p1);
      p1 = fnIb;
        // printf(" wl_ib=|%s|\n",p1);
      L_nxt_ib:
        p2 = strchr (p1, '\n');
        if(p2) {
          *p2 = '\0';
          fprintf(fpo,"ms_ib %s\n",p1);
          p1 = p2;
          ++p1;
          goto L_nxt_ib;
        }
        fprintf(fpo,"ms_ib %s\n",p1);
    }

    p1 = gtk_button_get_label ((GtkButton*)wl_ob);
    if(strlen(p1) > 3) fprintf(fpo,"ms_ob %s\n",p1);



    fprintf(fpo,"# eof gMsh1 cmd %s\n",clfNam);
    fclose (fpo);
*/

    return GUI_dlg1_exit (clfNam);



  //================================================================
  L_canc:
    GUI_dlg1_cancel (NULL, NULL);
    return 0;
}


//================================================================
  int GUI_gMsh1__ () {
//================================================================

  int          irc;
  GtkWidget    *box0, *box1, *box2, *w1, *w2;


  UI_win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position (GTK_WINDOW(UI_win), GTK_WIN_POS_MOUSE);
  gtk_window_set_title (GTK_WINDOW (UI_win), " gMsh1 ");
  g_signal_connect (UI_win, "destroy",
                    G_CALLBACK(GUI_dlg1_cancel), NULL);


  box0 = gtk_vbox_new (FALSE, 0);   // gtk2; TRUE=all objs same space
  gtk_container_add (GTK_CONTAINER (UI_win), box0);


  //================================================================
  box1 = gtk_hbox_new (TRUE, 0);
  w1 = gtk_label_new ("Controlfile:");
  gtk_misc_set_alignment (GTK_MISC (w1), 0.f, 0.5f);
  gtk_container_add (GTK_CONTAINER (box1), w1);

  w1 = gtk_button_new_with_label (" select ");
  g_signal_connect (w1, "clicked", G_CALLBACK(GUI_cb_clf__), NULL);
  gtk_container_add (GTK_CONTAINER (box1), w1);
  gtk_widget_set_tooltip_text (w1, "select controlfile");

  w2 = gtk_button_new_with_label (" edit ");
  g_signal_connect (w2, "clicked", G_CALLBACK(GUI_cb_edi__), NULL);
  gtk_container_add (GTK_CONTAINER (box1), w2);
  gtk_widget_set_tooltip_text (w2, "edit controlfile");

  gtk_container_add (GTK_CONTAINER (box0), box1);

  we_clf = gtk_entry_new ();
  gtk_entry_set_text (GTK_ENTRY(we_clf), clfNam); // preset Text
  gtk_container_add (GTK_CONTAINER (box0), we_clf);
  gtk_widget_set_tooltip_text (we_clf, "key new controlfilename");

  w1 = gtk_hseparator_new ();
  gtk_container_add (GTK_CONTAINER (box0), w1);


  //================================================================
  box1 = gtk_frame_new ("filename points:");

  wl_pt = gtk_button_new_with_label (" - ");
  g_signal_connect (wl_pt, "clicked", G_CALLBACK(GUI_cb_bt__), "w_pt");
  gtk_container_add (GTK_CONTAINER (box1), wl_pt);
  gtk_box_set_spacing (GTK_BOX(box0), 10);   // vertical space
  gtk_container_add (GTK_CONTAINER (box0), box1);


  //================================================================
  box1 = gtk_frame_new ("filename breaklines:");

  wl_bl = gtk_button_new_with_label (" - ");
  g_signal_connect (wl_bl, "clicked", G_CALLBACK(GUI_cb_bt__), "w_bl");
  gtk_container_add (GTK_CONTAINER (box1), wl_bl);
  gtk_box_set_spacing (GTK_BOX(box0), 10);   // vertical space
  gtk_container_add (GTK_CONTAINER (box0), box1);


  //================================================================
  box1 = gtk_frame_new ("filenames inner-boundaries:");

  wl_ib = gtk_button_new_with_label (" - ");
  g_signal_connect (wl_ib, "clicked", G_CALLBACK(GUI_cb_bt__), "w_ib");
  gtk_container_add (GTK_CONTAINER (box1), wl_ib);
  gtk_box_set_spacing (GTK_BOX(box0), 10);   // vertical space
  gtk_container_add (GTK_CONTAINER (box0), box1);


  //================================================================
  box1 = gtk_frame_new ("filename outer-boundary:");

  wl_ob = gtk_button_new_with_label (" - ");
  g_signal_connect (wl_ob, "clicked", G_CALLBACK(GUI_cb_bt__), "w_ob");
  gtk_container_add (GTK_CONTAINER (box1), wl_ob);
  gtk_box_set_spacing (GTK_BOX(box0), 10);   // vertical space
  gtk_container_add (GTK_CONTAINER (box0), box1);

  w1 = gtk_hseparator_new ();
  gtk_container_add (GTK_CONTAINER (box0), w1);


  //================================================================
  // CHECKBOXES
  w1 = gtk_label_new ("Display:");
  gtk_misc_set_alignment (GTK_MISC (w1), 0.f, 0.5f);
  gtk_container_add (GTK_CONTAINER (box0), w1);

  box1 = gtk_hbox_new (TRUE, 0);   // gtk2; TRUE=all objs same space

  wc_fac = gtk_check_button_new_with_label ("Fac");
  // g_signal_connect (wc_fac, "clicked", G_CALLBACK(GUI_cb_cb), "fac");
  gtk_container_add (GTK_CONTAINER (box1), wc_fac);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wc_fac), TRUE);
  gtk_widget_set_tooltip_text (wc_fac, "display faces");

  wc_pt = gtk_check_button_new_with_label ("PT");
  // g_signal_connect (wc_pt, "clicked", G_CALLBACK(GUI_cb_cb), "PT");
  gtk_container_add (GTK_CONTAINER (box1), wc_pt);
  gtk_widget_set_tooltip_text (wc_pt, "display points");

  wc_bl = gtk_check_button_new_with_label ("BL");
  // g_signal_connect (wc_bl, "clicked", G_CALLBACK(GUI_cb_cb), "BL");
  gtk_container_add (GTK_CONTAINER (box1), wc_bl);
  gtk_widget_set_tooltip_text (wc_bl, "display 3D-breaklines");

  wc_bnd = gtk_check_button_new_with_label ("Bnd");
  // g_signal_connect (wc_ib, "clicked", G_CALLBACK(GUI_cb_cb), "IB");
  gtk_container_add (GTK_CONTAINER (box1), wc_bnd);
  gtk_widget_set_tooltip_text (wc_bnd, "display 3D-inner- and outer-boundaries");

  wc_fCen = gtk_check_button_new_with_label ("fCen");
  // g_signal_connect (wc_fCen, "clicked", G_CALLBACK(GUI_cb_cb), "fCen");
  gtk_container_add (GTK_CONTAINER (box1), wc_fCen);
  gtk_widget_set_tooltip_text (wc_fCen, "display centerpoints of faces");

  wc_blIn = gtk_check_button_new_with_label ("blIn");
  // g_signal_connect (wc_blIn, "clicked", G_CALLBACK(GUI_cb_cb), "blIn");
  gtk_container_add (GTK_CONTAINER (box1), wc_blIn);
  gtk_widget_set_tooltip_text (wc_blIn, "display breaklines - input");

  wc_bndIn = gtk_check_button_new_with_label ("bndIn");
  // g_signal_connect (wc_bndIn, "clicked", G_CALLBACK(GUI_cb_cb), "bndIn");
  gtk_container_add (GTK_CONTAINER (box1), wc_bndIn);
  gtk_widget_set_tooltip_text (wc_bndIn, "display inner- and outer-boundaries - input");

  gtk_container_add (GTK_CONTAINER (box0), box1);
  // gtk_container_set_border_width (GTK_CONTAINER (box0), 10);

  w1 = gtk_hseparator_new ();
  gtk_container_add (GTK_CONTAINER (box0), w1);



  //================================================================
  // outputformat:
  box1 = gtk_hbox_new (TRUE, 0);   // gtk2; TRUE=all objs same space

  w1 = gtk_label_new ("Outputformat:");
  gtk_misc_set_alignment (GTK_MISC (w1), 0.f, 0.5f);
  gtk_container_add (GTK_CONTAINER (box1), w1);

  wr_gc = gtk_radio_button_new_with_label (NULL, "gcad");
  gtk_container_add (GTK_CONTAINER (box1), wr_gc);

  wr_dxf = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(wr_gc),"dxf");
  gtk_container_add (GTK_CONTAINER (box1), wr_dxf);

  gtk_container_add (GTK_CONTAINER (box0), box1);

  w1 = gtk_hseparator_new ();
  gtk_container_add (GTK_CONTAINER (box0), w1);



  //================================================================
  // BUTTONS
  box1 = gtk_hbox_new (TRUE, 0);   // gtk2; TRUE=all objs same space

  w1 = gtk_button_new_with_label ("Undo");
  g_signal_connect (w1, "clicked", G_CALLBACK(GUI_cb_bt__), "undo");
  gtk_container_add (GTK_CONTAINER (box1), w1);
  gtk_widget_set_tooltip_text (w1, "clear last selected file");

  w1 = gtk_button_new_with_label ("Cancel");
  g_signal_connect (w1, "clicked", G_CALLBACK(GUI_cb_bt__), "canc");
  gtk_container_add (GTK_CONTAINER (box1), w1);

  w1 = gtk_button_new_with_label ("Help");
  g_signal_connect (w1, "clicked", G_CALLBACK(GUI_cb_bt__), "help");
  gtk_container_add (GTK_CONTAINER (box1), w1);

  w1 = gtk_button_new_with_label ("ViewLog");
  g_signal_connect (w1, "clicked", G_CALLBACK(GUI_cb_bt__), "view");
  gtk_container_add (GTK_CONTAINER (box1), w1);
  gtk_widget_set_tooltip_text (w1, "view logFile of last mesh");

  w1 = gtk_button_new_with_label ("Mesh");
  g_signal_connect (w1, "clicked", G_CALLBACK(GUI_cb_bt__), "mesh");
  gtk_container_add (GTK_CONTAINER (box1), w1);
  gtk_widget_set_tooltip_text (w1, "mesh and create modelfile");

  gtk_container_add (GTK_CONTAINER (box0), box1);


  //================================================================
  // ERRORINFO
  wl_err = gtk_label_new (" - ");
  gtk_misc_set_alignment (GTK_MISC (wl_err), 0.f, 0.5f);
  gtk_container_add (GTK_CONTAINER (box0), wl_err);


  //----------------------------------------------------------------
  gtk_widget_show_all (box1);
  gtk_widget_show_all (box0);
  gtk_window_set_position (GTK_WINDOW(UI_win), GTK_WIN_POS_CENTER_ALWAYS);
  //  GTK_WIN_POS_MOUSE not ok
  gtk_widget_show (UI_win);


  //----------------------------------------------------------------
  // load file clfNam
  gmsh1_loadfile ();

  GUI_update__ ();

  upState = 1;

  gtk_main ();

  return 0;

}

 
//================================================================
  int GUI_dlg1_exit (char *sOut) {
//================================================================
// write info-file (<tmp>/gMsh1.txt) end exit.
// 1.Line = clfNam = filename commandfile
// 2.Line = output sOut

  FILE     *fpo;

  printf("## exit-GUI_dlg1_exit |%s|\n",sOut);

  if((fpo=fopen(fnTmp, "w")) == NULL) {
    printf("## ERROR GUI_dlg1 write file %s\n",fnTmp);
    exit(1);
  }
  fprintf(fpo, "%s\n", clfNam);
  fprintf(fpo, "%s\n", sOut);
  fclose(fpo);

  // printf("%s\n", sOut);  // to provide to caller via stdout
  // fflush(stdout);

  // gtk_widget_hide (UI_list1_win);
  // gtk_signal_emit_by_name (GTK_OBJECT(UI_list1_win),"destroy");
  // gtk_widget_destroy (UI_list1_win);

  exit(0);

}


//================================================================
  int GUI_dlg1_cancel (void *parent, void *data) {
//================================================================
/// \code
/// INTERNAL
/// list-window exiting; report to user ..
/// \endcode


  printf("## GUI_dlg1_cancel\n");

  return (GUI_dlg1_exit (""));

}


//================================================================
  int GUI_dlg1_err1 () {
//================================================================
 
  int   i1;

  printf("## ERROR GUI_dlg1 error parameters\n");
  printf("- par[1]  - filename info-file)\n");
  for(i1=0; i1<nArg; ++i1) printf("## GUI_file argv[%d]=|%s|\n",i1,paArg[i1]);
  
  exit(1);

}


//================================================================
  int main (int argc, char *argv[]) {
//================================================================
// Input:
//   argv[1]    startDirectory
//   argv       max 2 parameters
// Output:
//   stdout     full selected filename; empty for Cancel;


  int      irc, i1;


  paArg = argv;
  nArg = argc;

  if(argc < 2) return GUI_dlg1_err1 ();

  // set AP_bin_dir = dir binaries
  OS_bin_dir_set (argv);


  strcpy(fnTmp, paArg[1]);


  // get clfNam = filename command-file from 1. line of info-file
  irc = UTX_fgetLine (clfNam, 400, fnTmp, 1);
  if(irc < 0) {
    printf("## ERROR GUI_dlg1 info-file %s\n",clfNam);
    exit(1);
  }
    printf("## clfNam = |%s|\n",clfNam);

//   // sprintf(clfNam, "%sunknown.gmsh1",paArg[1]);
//   sprintf(clfNam, "%s%s",paArg[1],paArg[2]);


  fnIb[0] = '\0';

  gtk_disable_setlocale ();  // sonst Beistrich statt Decimalpunkt !! (LC_ALL)

  i1 = 0;
  gtk_init (&i1, NULL);

  return GUI_gMsh1__ ();

}

 
// EOF
