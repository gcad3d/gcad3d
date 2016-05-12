// Export / Print port.                                    RF 2016-03-09
/*
 *
 * Copyright (C) 2016 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
2016-03-09 New. RF.

-----------------------------------------------------
*/
/*!
\file  ../xa/print__.c
\brief write / print PDF EPS SVP
\code
=====================================================
List_functions_start:

PRI__        MAIN ENTRY

List_functions_end:
=====================================================

\endcode *//*----------------------------------------

make -f xa_print__.mak

...................................
GUI via Notebook-pages;

Print | pdf-File | ps - File | HPGL - File | Preview

Wie ein Image machen ?
export as jpg < bmp ?


...................................
parameters for PDF:
- preview / command               xdg-open | evince | xpdf | firefox file://<fn>
OR print to file /outfilename
OR print direct / printcommand         lpr -PMFC7360N <fn> 
     use "evince -w <fn>

Papersize   A4 | A3

Landscape or not


...................................
Test ob ps2pdf installiert:
OS_ckSW_ps2pdf
  which ps2pdf

export to ps,  dann:
ps2pdf -sPAPERSIZE=a3 <fn>.ps <fn>.pdf

PRI_UI__   ??



-------------------------------------------------------------------
*/



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include <errno.h>
// #include <dlfcn.h>           // Unix: dlopen


#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
__declspec(dllexport) int PRI__ (void**);
#define extern __declspec(dllimport)
#endif


#define   TRUE 1
#define   FALSE 0



#include "../ut/ut_geo.h"
#include "../ut/ut_os.h"       // OS_get_tmp_dir

#include "../gui/gui__.h"


// Externals aus ../xa/xa.c:
extern char WC_modnam[128];
extern char AP_printer[80];       // Printer



// Local vars:
static MemObj win0 = GUI_OBJ_NEW;
static void  *dll0;

// Prototypes:
int win1__ (MemObj *mo, void **data);



//===========================================================================
  int PRI__ (void **dBlock)  {
//===========================================================================
/// Export/Print

  // int    i1;
  // char   *fNam;
  // char   fNam[256], *fNam;


  // fNam = (char*)dBlock[0];
  // dll0 = dBlock[1];
  // printf("PRI__ |%s|\n", fNam);


  PRI_UI__ (NULL, GUI_SETDAT_EI(TYP_EventPress, UI_FuncInit));

  return 0;

}



//=====================================================================
  int PRI_UI__ (MemObj *mo, void **data) {
//=====================================================================
// Print-Panel

  int              i1, irot;
  char             *txoff, *txscl, *txcmd1, *txcmd2, *txcmd3, *txgray;
  char             cbuf1[256], fNam[128];
  MemObj           box0, wtmp1, wtmp2, wtmp3, wtmp4;

  static int       mode;
  static char      fTyp[8], pgTyp[4];
  static MemObj    w_rot, w_off, w_scl, w_gray,
                   w_cmd1, w_cmd2, w_cmd3, wb_form, wb_view,
                   w_func1, w_func2, w_func3,
                   w_a4, w_a3;

  void            GL_Print_Redraw ();


  printf("PRI_UI__ \n"); fflush(stdout);

  i1 = GUI_DATA_I1;


  printf("PRI_UI__ %d\n",i1); fflush(stdout);



  switch (i1) {


    //---------------------------------------------------------
    case UI_FuncInit:

      if(GUI_OBJ_IS_VALID(&win0)) {           // Win schon vorhanden ?
        // gtk_widget_destroy (win0);
        return -1;
      }

      // create white background in grafic-window
      GL_Redra__ (1);


      win0 = GUI_Win__ ("Export/Print",PRI_UI__, "");

      box0 = GUI_box_v (&win0, "");

      wtmp3 = GUI_box_h (&box0, "");
        GUI_radiobutt__(&wtmp3,"PDF  ", 0,PRI_UI__, &GUI_FuncUCB6, "");
        GUI_radiobutt__(&wtmp3,"PS ",   1,PRI_UI__, &GUI_FuncUCB5, "");
        GUI_radiobutt__(&wtmp3,"PCL5 ", 1,PRI_UI__, &GUI_FuncUCB1, "");
        GUI_radiobutt__(&wtmp3,"HPGL ", 1,PRI_UI__, &GUI_FuncUCB2, "");
        GUI_radiobutt__(&wtmp3,"SVG  ", 1,PRI_UI__, &GUI_FuncUCB7, "");
      mode = 0;  // 0=PDF
      strcpy(fTyp, "pdf");


      wb_form = GUI_box_h (&box0, "");
        GUI_radiobutt__(&wb_form, "A4 ", 0,PRI_UI__, &GUI_FuncUCB3, "");
        GUI_radiobutt__(&wb_form, "A3 ", 1,PRI_UI__, &GUI_FuncUCB4, "");
      strcpy(pgTyp, "A4");


// ACHTUNG: GUI_frame__ geht nach einem GUI_ckbutt__ nicht !!!!!!!!!!!!!!!
      // wtmp1 = GUI_frame__ (box0, NULL, 2);
      GUI_sep_h (&box0, 2);

      // wtmp2 = GUI_box_v (wtmp1, 0);
      wtmp2 = GUI_box_h (&box0, "");

      wtmp3 = GUI_box_v (&wtmp2, "");
        wtmp4 = GUI_box_h (&wtmp3, "");
        w_func1=GUI_radiobutt__(&wtmp4, "Preview       ", 0,NULL, NULL, "");
        wtmp4 = GUI_box_h (&wtmp3, "");
        w_func2=GUI_radiobutt__(&wtmp4, "print to file ", 1,NULL, NULL, "");
        wtmp4 = GUI_box_h (&wtmp3, "");
        w_func3=GUI_radiobutt__(&wtmp4, "print direct  ", 1,NULL, NULL, "");

      wtmp3 = GUI_box_v (&wtmp2, "");
        wtmp4 = GUI_box_h (&wtmp3, "");
        // OS_get_vwr_ps: get ps-viewer (gv|evince)
        w_cmd1=GUI_entry__(&wtmp4, NULL, OS_get_vwr_ps(), NULL, NULL, "10");

      wtmp4 = GUI_box_h (&wtmp3, "");
        // filename w.o. filetyp
        sprintf(cbuf1, "%sprint",OS_get_tmp_dir());
        w_cmd2=GUI_entry__(&wtmp4, NULL, cbuf1,   NULL,NULL, "");

      // printer | printcommand
      wtmp4 = GUI_box_h (&wtmp3, "");
#ifdef _MSC_VER
        w_cmd3=GUI_entry__(&wtmp4, NULL, OS_get_printer(),  NULL,NULL, "");
#else
        sprintf(AP_printer, "lpr -P%s",OS_get_printer()); //2016-03-31 -l removed
        w_cmd3=GUI_entry__(&wtmp4, NULL, AP_printer,        NULL,NULL, "");
#endif

      GUI_sep_h (&box0, 2);

      w_rot = GUI_ckbutt__ (&box0, "Landscape (rotate 90 deg)",
                          TRUE, NULL, NULL, "");

      wtmp1 = GUI_box_h (&box0, "");
      w_off = GUI_entry__(&wtmp1, "Offset", "0,0", NULL,NULL, "10");
      w_scl = GUI_entry__(&wtmp1, " Scale ", "1",  NULL,NULL, "10");

      // wtmp1 = GUI_box_h (box0, 0);
      // w_gray=GUI_Entry(wtmp1, "Graufaktor", "2", NULL, -50);

      wtmp1 = GUI_box_h (&box0, "");
      GUI_button__ (&wtmp1, "OK",   PRI_UI__, &GUI_FuncWork, "e");
      GUI_button__ (&wtmp1, "Exit", PRI_UI__, &GUI_FuncExit, "e");



      GUI_Win_up (NULL, &win0, 0);  // always on top
      GUI_Win_go (&win0);
      break;




    //---------------------------------------------------------
    case UI_FuncUCB6:   // PDF
      mode = 0;
      strcpy(fTyp, "pdf");
      GUI_set_enable (&wb_form, TRUE);  // A4/A3 ein
      GUI_set_enable (&w_func1, TRUE);  // Preview ein
      GUI_set_enable (&w_cmd1, TRUE);   // PreviewCmd ein
    case UI_FuncUCB5:    // PS
      mode = 1;
      strcpy(fTyp, "eps");
      GUI_set_enable (&wb_form, TRUE);  // A4/A3 ein
      GUI_set_enable (&w_func1, TRUE);  // Preview ein
      GUI_set_enable (&w_cmd1, TRUE);   // PreviewCmd ein
      break;
    case UI_FuncUCB1:   // PCL5
      mode = 2;
      strcpy(fTyp, "pcl");
      GUI_set_enable (&wb_form, TRUE);  // A4/A3 ein
      GUI_set_enable (&w_func1, FALSE); // Preview aus
      GUI_set_enable (&w_cmd1, FALSE);  // PreviewCmd aus
      break;
    case UI_FuncUCB2:   // HPGL
      mode = 3;
      strcpy(fTyp, "hpgl");
      GUI_set_enable (&wb_form, FALSE); // A4/A3 aus
      GUI_set_enable (&w_func1, FALSE); // Preview aus
      GUI_set_enable (&w_cmd1, FALSE);  // PreviewCmd aus
      break;
    case UI_FuncUCB7:   // SVG
      mode = 4;
      strcpy(fTyp, "svg");
      GUI_set_enable (&wb_form, TRUE);  // A4/A3 ein
      GUI_set_enable (&w_func1, TRUE);  // Preview ein
      GUI_set_enable (&w_cmd1, TRUE);   // PreviewCmd ein
      break;
    case UI_FuncUCB3:   // A4
      strcpy(pgTyp, "A4");
      break;
    case UI_FuncUCB4:   // A3
      strcpy(pgTyp, "A3");
      break;


    //---------------------------------------------------------
    case UI_FuncWork:
        // printf("Print work\n");

      // drehen -
      // irot = GTK_TOGGLE_BUTTON (w_rot)->active;
      irot = GUI_ckbutt_get (&w_rot);
        printf("  rot=%d\n",irot);

      // cmd's, Offset, Scale
      txcmd1 = GUI_entry_get (&w_cmd1);
      txcmd2 = GUI_entry_get (&w_cmd2);  // filename printfile
      txcmd3 = GUI_entry_get (&w_cmd3);  // printer | printcommand
      txoff  = GUI_entry_get (&w_off);
      txscl  = GUI_entry_get (&w_scl);
      // txgray = gtk_entry_get_text ((GtkEntry*) (w_gray));
      strcpy(AP_printer, GUI_entry_get (&w_cmd3));

      // Hautpfunktion ausfuehren
      // auslesen, Zwischendatei <tempDir>/print.tmp generieren
      // if(AP_print__() < 0) break;

      // set outfilename - without \"
      // sprintf(fNam,"\"%sprint.%s\"", OS_get_tmp_dir(), fTyp);
      sprintf(fNam,"%sprint.%s", OS_get_tmp_dir(), fTyp);
        printf(" fNam=|%s|\n",fNam);


      if(mode == 0) {    // PDF:
        // create <tempDir>/print.eps
        gl2ps_print3 (fNam, WC_modnam, "gCAD3D", GL_Print_Redraw);
        // AP_print_pdf (irot, pgTyp, txoff, txscl, "2");


      } else if(mode == 1) {   // PS: 
        //           rot, off, Scale, gray)
        // create <tempDir>/print.eps
        gl2ps_print3 (fNam, WC_modnam, "gCAD3D", GL_Print_Redraw);
        // AP_print_psv2 (irot, txoff, txscl, "2");


      } else if(mode == 2) {   // PCL5
        // create <tempDir>/print.pcl 
        // ptyp "A4" od "A5"
        // rot=0=normal,1=90Grad_drehen.
        AP_print_gl1 (2, pgTyp, irot, txoff, txscl);


      } else if(mode == 3) {   // HPGL
        // create <tempDir>/print.hpgl
        AP_print_gl1 (1, pgTyp, irot, txoff, txscl);


      } else if(mode == 4) {   // SVG
        // create <tempDir>/print.hpgl
        gl2ps_print3 (fNam, WC_modnam, "gCAD3D", GL_Print_Redraw);

      }



      //..............................................
      // preView PDF|PS
      // if(GTK_TOGGLE_BUTTON (w_func1)->active) {
      if(GUI_radiobutt_get (&w_func1)) {
        // AP_Print0 (1,txcmd1,irot,txoff,txscl);
        if((mode == 2)||(mode == 3)) {
          TX_Print ("***** cannot view PCL5 / HPGL");
          return -1;
        }
#ifdef _MSC_VER
        // MS: GSview or .. ??; start direkt with "<fNam.ps>"
        // sprintf(cbuf1,"move \"%sprint.dat\" \"%sprint.eps\"",
                // OS_get_tmp_dir(),OS_get_tmp_dir());
          // printf("system %s\n",cbuf1);
        // system(cbuf1);
        sprintf(cbuf1,"%s", fNam);

#else
        // Linux: system "evince <fNam.eps>"
        sprintf(cbuf1,"%s %s &", txcmd1, fNam);
#endif
          printf("system %s\n",cbuf1);
        system(cbuf1);


      //..............................................
      // copy -> file
      // } else if(GTK_TOGGLE_BUTTON (w_func2)->active) {
      } else if(GUI_radiobutt_get (&w_func2)) {
        // AP_Print0 (2,txcmd2,irot,txoff,txscl);
        // get outfilename from txcmd2 = w_cmd2;

#ifdef _MSC_VER
        sprintf(cbuf1,"copy/Y %s \"%s.%s\"", fNam, txcmd2, fTyp);
#else
        sprintf(cbuf1,"cp -f %s %s.%s", fNam, txcmd2, fTyp);
#endif
          printf("%s\n",cbuf1);
        TX_Print (cbuf1);
        system (cbuf1);


      //..............................................
      // print direct
      // } else if(GTK_TOGGLE_BUTTON (w_func3)->active) {
      } else if(GUI_radiobutt_get (&w_func3)) {
        // AP_Print0 (3,txcmd3,irot,txoff,txscl);
#ifdef _MSC_VER
        // MS: gtk: GUI_printer__ - no gtk_print_unix_dialog..
        // (gtk_print_job_new,gtk_print_job_set_source,gtk_print_job_send)
        // .NET: MS_print.vb - LocalPrintServer - no supported ..
        // direkt with copy - does not work (eps not recognized ..)
        // copy "C:\temp\print.eps" "\\FWORK\Brother MFC-7360N Printer"
        // sprintf(cbuf1,"copy/b %s %s", fNam, txcmd3);  // OS_get_printer not OK
        sprintf(cbuf1,"%s", fNam);
#else
        // Linux: "lpr -PMFC7360N <fNam.eps>"
        sprintf(cbuf1,"%s %s &", txcmd3, fNam);
#endif
          printf("%s\n",cbuf1);
        TX_Print (cbuf1);
        system (cbuf1);

      }


      break;



    //---------------------------------------------------------
    case UI_FuncExit:  // 102
    case UI_FuncKill:
      // EXIT
      if(GUI_OBJ_IS_VALID(&win0)) {  
        GUI_Win_kill (&win0);
        win0 = GUI_OBJ_INVALID();
      }

  }

  return 0;

}


// EOF
