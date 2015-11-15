//  ../xa/xa_edi__.c                        Reiter 2012-07-30
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
2012-07-30 ex ../xa/xa_ed.c            RF

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_edi__.c
\brief  Neutral EditorFunctions (not Gtk-specific).
\code
=====================================================
List_functions_start:

ED_CB__             editor-callback

ED_load__           mem -> editor
ED_unload__         editor -> memory
ED_save_file        save editor -> file

EDI_set_lnr         set ED_lnr_act, goto Line, display lNr
ED_goto__           goto curPos in editor
EDI_goto_lnr        goto Line

ED_sysed__          use system-editor for active line

List_functions_end:
=====================================================
See also APED_ (APED_dec_defLn ..)

\endcode *//*----------------------------------------


 Neutral EditorFunctions (not Gtk-specific).
../xa/xa_edi__.c            this source
../xa/xa_ed.c
../xa/xa_cad_ed.c
../xa/xa_ed_mem.c
AP_SRC_*
AP_src*
AP_lNr_*
AP_APT_*


*/




#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>                    // isdigit


#include "../ut/ut_umem.h"                // Memspc MemObj UME_*
#include "../ut/ut_cast.h"                // INT_PTR
#include "../ut/ut_os.h"          // OS_get_tmp_dir

#include "../gui/gui__.h"         // GUI_ UI_Func..

#include "../xa/xa_ui.h"          //  UI_men__
#include "../xa/xa_uid.h"         //  UI_MODE_MAN
#include "../xa/xa_mem.h"         //  mem_cbuf1
#include "../xa/xa_app.h"         // PRC_IS_ACTIVE
#include "../xa/xa.h"             // APP_act_nam

 





//===========================================================================
// Externals:

// aus xa_ui.c:
extern MemObj    winED;
extern MemObj    winGR;
extern int       UI_InpMode;
extern int       UI_EditMode;
extern int       xa_fl_TxMem;

// aus ut_txfil.c:
extern char       *UTF_FilBuf0;
extern long       UTF_FilBuf0Siz;
extern long       UTF_FilBuf0Len;  // die aktuelle Filesize

// from ../xa/xa_ed.c:
extern int ED_lnr_act;

// from ../xa/xa.c
// extern long       AP_ED_lNr;
extern int        AP_src;                // AP_SRC_MEM od AP_SRC_EDI
//int       AP_ED_cPos;
//char      AP_ED_oNam;

// from ../ci/NC_Main.c
extern int       APT_dispPL;
// replace ED_lnr_act mit AP_ED_lNr


// from ../xa/xa_ed.c
extern int       APT_mac_fil; // 1=InputFromMemory; 0=InputFromFile.


//===========================================================================
// Locals:



//================================================================
  int ED_sysed__ (int mode) {
//================================================================
/// \code
/// ED_sysed__          use system-editor for active line
/// mode    1   edit single line (lnNr <ED_lnr_act>)
/// \endcode

// see also AP_APT_sysed AP_src_mod_ed

  int    s1Siz;
  char   *s1, fnam[256];
  long   lNr;


  printf("ED_sysed__ %d\n",mode);

  s1 = mem_cbuf1;
  s1Siz = mem_cbuf1_SIZ;
  lNr = ED_lnr_act;
  sprintf(fnam, "%sselection.txt", OS_get_tmp_dir());

  // get line into buffer
  s1Siz = GUI_edi_RdLn (s1, s1Siz, lNr, &winED);
  if(s1Siz < 1) goto L_err1;


  // Zeile in ein File rausschreiben    see also UTF_wrf_line
  UTX_wrf_str (fnam, s1);
  TX_Print("Edit line %d spawned ..",lNr);


  // modify File m. Editor
  APP_edit (fnam);
  TX_Print("Edit done ..");


  // load modified textline
  s1Siz = OS_FilSiz (fnam);
  if(s1Siz > mem_cbuf1_SIZ) goto L_err1;
  UTX_str_file (s1, &s1Siz, fnam);


  // modify line in gcad-editor
  GUI_edi_mod_ln (&winED, lNr, s1);


  return 0;

  L_err1:
    TX_Print(".. line empty ..");
    return -1;

}

 
//================================================================
  int EDI_set_lnr (long lNr) {
//================================================================
/// EDI_set_lnr         set ED_lnr_act, goto Line, display lNr

// was GUI_Ed_setCnxtL

  // goto <lNr> in editor; display <lNr>
  EDI_goto_lnr (lNr);

  // set ED_lnr_act = lNr
  ED_set_lnr_act (lNr);

  return 0;

}


//================================================================
  int EDI_goto_lnr (long lNr) {
//================================================================
/// EDI_goto_lnr        goto Line, display lNr

  long   i1;

  GUI_edi_setLnr (&winED, lNr);

  i1 = lNr + 1;
  UI_AP (UI_FuncSet, UID_ouf_lNr, (void*)i1);
  

  return 0;

}
 

//================================================================
  int EDI_CB__ (MemObj *mo, void **data) {
//================================================================
/// ED_CB__             editor-callback of GUI_edi__

  int     i1;
  long    l1;
  char    *p1;


  // printf("EDI_CB__ ev=%d\n",GUI_DATA_EVENT);
  // printf("  EDI_CB__: ED_lnr_act=%d\n",ED_lnr_act);


  //----------------------------------------------------------------
  if(GUI_DATA_EVENT == TYP_EventEnter) {    // 400
    // caused by mouseclick into editor ..
    l1 = GUI_DATA_L2;
      // printf(" enter line %ld\n",l1);
    // ED_newPos ED_update
    ED_update (0L);  // editor -> memory if Filesize has changed ..
    // von der zuletzt bearbeiteten Zeile bis zu lNr anzeigen/abarbeiten
    ED_work_CurSet (l1);
    //
    GUI_edi_Focus (&winED);

    return 0;

    

  //----------------------------------------------------------------
  } else if (GUI_DATA_EVENT == TYP_EventPress) {   // 402
    // key-press

/* kommt nicht !
    if(GUI_DATA_I1 == GUI_KeyReturn) {
      // UI_key_return ();
      l1 = ED_get_lnr_act();
      --l1;
      ED_lnr_act = l1;
      // l1 -= 2;
      // ED_work_CurSet (l1);
      // UI_EdKeyCR (1);
      goto AllDone;
    }
*/

    // test if it is a special-char
    if(isascii(GUI_DATA_I1)) goto AllDone;
    i1 = UI_key_view__ (GUI_DATA_EVENT, GUI_DATA_I1);
    if(i1) goto AllDone;


    if(GUI_DATA_I1 == GUI_KeyEsc) {
      UI_key_escape ();


    } else if(GUI_DATA_I1 == GUI_KeyF1) {   // HELP-key
      // test if process is active
      if(PRC_IS_ACTIVE) {
        APP_Help (APP_act_proc, "");
      } else {
        APP_Help ("gCAD3D_startMAN", "");
      }


    } else if(GUI_DATA_I1 == GUI_KeyF3) {
      UI_men__ ("Edit");
      ED_update (0L);


    } else if(GUI_DATA_I1 == GUI_KeyF4) {
      AP_APT_clean ();
    }

    goto AllDone;





  //----------------------------------------------------------------
  } else {                       // 403 = TYP_EventRelease = key-release
      // printf(" key=%c (%x) mod=%d\n",GUI_DATA_I1,GUI_DATA_I1,GUI_DATA_I2);
    
    // skip all not printable keys
    if(!isascii(GUI_DATA_I1)) {

      // necessary; keyDown is not activated (in last line) ?
      if((GUI_DATA_I1 == GUI_KeyDel)    ||
         (GUI_DATA_I1 == GUI_KeyReturn))     {
          // printf(" release-CR..\n");
        // bei CR am end of file kommt kein TYP_EventEnter new-Line !!!!!
        // UI_EdKeyCR (); geht nicht - liefert zusätzliches CR !

        ED_update (0L);
        l1 = GUI_edi_getLnr (mo);
        if(ED_lnr_act >= l1) ED_lnr_act = l1 - 1;
        // l1 = ED_lnr_act; --ED_lnr_act;
          // printf(" ED_lnr_act=%d l1=%ld\n",ED_lnr_act,l1);
        UI_AP (UI_FuncSet, UID_ouf_lNr, (void*)l1);
        GL_temp_delete ();    // remove circle
        WC_set_obj_stat (0);  // 0=perm
        ED_work_CurSet (l1);
        l1 = ED_get_lnr_act() - 1;  // get lNr AP_ED_lNr
        // printf(" lastLn %d |%s|\n",l1,mem_cbuf1); // filled by ED_Run
        if(GUI_DATA_I1 == GUI_KeyReturn) {  // no undo for delete -operations
          UNDO_grp_add (l1, 0);             // add new codeline to undo-list
          UNDO_upd__ ();
        }


      } else {
        // modify view (move/pan/zoom)
        UI_key_view__ (GUI_DATA_EVENT, GUI_DATA_I1);
      }
      goto AllDone;
    }


    // Shift + Alt
    if((GUI_DATA_I2 & GUI_Modif_shift) && (GUI_DATA_I2 & GUI_Modif_alt)) {
      UI_key_spcShAlt (GUI_DATA_I1);
      goto AllDone;
    }


    // check modifier Ctrl
    if(GUI_DATA_I2 & GUI_Modif_ctrl) {
      // test if group-creation is active 
      // KeyStatCtrl = OFF;
      UI_key_spcCtrl (GUI_DATA_I1);
      goto AllDone;
    }


    // check modifier ALT
    if(GUI_DATA_I2 & GUI_Modif_alt) {
        // printf(" ALT IS ON\n");  

      i1 = toupper(GUI_DATA_I1);
      p1 = strchr("PLCSABDTVRINM",i1);

      if(p1) {
        i1 = AP_typ_typChar (i1);
          // printf(" i1=%d |%c|\n",i1,i1);
        // i1 = UI_creObjHdr (NULL, PTR_INT(i1));
        l1 = AP_cre_defHdr (i1, 20);
        // if(i1 > 0) goto AllDone;
  
      } else {
        if(i1 == 'F') {ED_add_Def ("FROM "); goto AllDone;}
        if(i1 == 'W') {ED_add_Def ("WORK "); goto AllDone;}
      }
      goto Finish;
    }
  }


  //----------------------------------------------------------------
  Finish:
  AllDone:
  return 0;

}


//================================================================
  int ED_goto__ (long curPos) {
//================================================================
/// ED_goto__           goto curPos
/// curPos -1L = goto EOF

// was UI_AP (UI_FuncSet, UID_Edit_Cpos);


  // printf("UI_AP UID_Edit_Cpos: CurPos -> %ld\n",l1);

  GUI_edi_setCpos (&winED, curPos);
  // AP_search_init ();

  return 0;

}


//================================================================
  int ED_save_file (char *fNam) {
//================================================================
/// ED_save_file        save editor -> file

  int     i1;
  long    l1, l2;
  char    *p1;


  // printf("ED_save_file |%s|\n",fNam);


  l1 = GUI_edi_getEof (&winED);
    // printf(" l1=%ld\n",l1);
  l2 = l1 + 100 + l1/4;  // for UTF8-characters


  // get tempspace
  p1 = UME_alloc_tmp (l2);


  // get editor -> tempspace
  i1 = GUI_edi_Read (p1, &l2, 0L, -1L, &winED);
  if(i1 < 0) {TX_Error("ED_save_file E001"); return -1;}

    // printf(" len-l2=%ld\n",l2);
  // necessary on MS:
  p1[l2] = '\0';  // 2013-04-11

  // add '\n' as last char
  UTX_endAddChar (p1, '\n');

  // save tempspace -> file
  UTX_wrf_str (fNam, p1);

    // printf(" fsiz=%ld\n",OS_FilSiz(fNam));

  return 0;

}

 
//================================================================
  int ED_unload__ () {
//================================================================
/// ED_unload__         editor -> memory

// was UI_AP (UI_FuncGet,  UID_WinEdit, NULL);


  int   i1, i2;
  char  *p1, s1[256];


  // printf("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU \n");
  // printf("ED_unload__ \n");


  // addOn-prog active ?
  if(PRC_IS_ACTIVE) {
      // printf(" _unload__ - write ed -> file\n");
    // save active process: copy editor -> file
    sprintf(s1, "%s%s", OS_get_tmp_dir(), APP_act_nam);
    ED_save_file (s1);

    goto L_exit;
  }


  //----------------------------------------------------------------
  // mormal: copy editor -> memory
  // Editfenster(gesamten APT-Text) > Memory kopieren
  // printf("Text->Mem modif=%d\n",xa_fl_TxMem);
  if(xa_fl_TxMem != 0) {      //Edittext - memory:  needUpdate
      // printf(" _unload__ - get ed -> mem\n");
    UTF_load_ (NULL, NULL);   // clear mem

    L_Get_WinEdit_1:
    UTF_FilBuf0Len = UTF_FilBuf0Siz;
    i1 = GUI_edi_Read (UTF_FilBuf0, &UTF_FilBuf0Len, 0L, -1L, &winED);
    if(i1 < 0) {
      i2 = UTF_alloc__ (UTF_FilBuf0Len);
      if(i2 < 0) { TX_Error("out of mem .."); return -1; }
      goto L_Get_WinEdit_1;
    }



   // cut at last \n (remove last unfinished line)  2012-11-18
   p1 = strrchr(UTF_FilBuf0, '\n');
   if(p1) {
       // printf(" bufLen=%ld %d\n",UTF_FilBuf0Len,strlen(UTF_FilBuf0));
       // printf(" buf=|%s|\n",UTF_FilBuf0);
     ++p1;
     *p1 = '\0';
     UTF_FilBuf0Len = p1 - UTF_FilBuf0;
   }

     // printf(" bufLen=%ld %d\n",UTF_FilBuf0Len,strlen(UTF_FilBuf0));
     // printf(" buf=|%s|\n",UTF_FilBuf0);
  }


  L_exit:
  xa_fl_TxMem = 0;

  return 0;

}

//================================================================
  int ED_load__ () {
//================================================================
/// ED_load__           mem -> editor



  // printf("ED_load__ %d\n",UI_InpMode);
  // printf(" UTF_FilBuf0Len=%ld\n",UTF_FilBuf0Len);
  // printf(" FilBuf0: %c %c %c %c\n",UTF_FilBuf0[0],UTF_FilBuf0[1],
                                   // UTF_FilBuf0[2],UTF_FilBuf0[3]);


  if(UI_InpMode != UI_MODE_MAN) return 0;


  // copy UTF_FilBuf0 -> Editor
  GUI_edi_Write (&winED, UTF_FilBuf0, &UTF_FilBuf0Len, UTF_FilBuf0Siz);
  // UI_Ed_fsiz = UTF_FilBuf0Len;
  ED_Reset ();  // ED_lnr_act = 0;

  return 0;

}


// EOF
