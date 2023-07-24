//            xa_aux.c
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
2002-02-21 fread .. == 0 statt NULL. RF.
2001-06-17 Port. -> Linux. RF. Extrah. aus MainFrm.cpp.

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_aux.c
\brief  import / export aux-funcs
\code
=====================================================
List_functions_start:

AP_ImportWRL_ckTyp           check VRML-1 or VRML-2
AP_Import_obj_CB             Callback des Iges-Import
AP_ImportXML

AP_ImportLwo
lwo_cb_savePoint
lwo_cb_saveFace

AP_Import3ds
ds3_cb_savePoint
ds3_cb_saveFace

// AP_ExportDxf
AP_ExportIges__
AP_ExportIges_Model

AP_appr                      Approximieren;   Curve -> Circ

List_functions_end:
=====================================================
// AP_ImportDxf

\endcode *//*----------------------------------------


Interaktive Funktionen - nicht freigegeben.



-------------------------------------------------------
Compile:
cc -c `gtk-config --cflags` xa_aux.c
cl -c /I ..\include xa_aux.c

*/


#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>                        /* printf ..   */
#include <ctype.h>                        /* isdigit ... */
#include <stdlib.h>                       /* atoi ..     */
#include <string.h>                       /* strcmp ..   */
#include <math.h>                         /* sin, cos .. */



#include "../ut/ut_geo.h"                 // Typ_PT
#include "../ut/ut_txt.h"                 // UTX_add_fl_u2
#include "../ut/ut_TX.h"                  // TX_Print
#include "../ut/ut_txTab.h"               // UtxTab
#include "../ut/ut_os.h"                  // AP_get_bas_dir
#include "../ut/ut_txfil.h"               // UTF_FilBufSiz
#include "../ut/func_types.h"                  // SYM_..
#include "../ut/ut_memTab.h"           // MemTab

#include "../gui/gui_types.h"      // UI_Func..

#include "../db/ut_DB.h"                  // APT_PT_SIZ

#include "../gr/ut_DL.h"                  // DB_GetDLatt

#include "../xa/xa.h"                     // mem_cbuf1_SIZ AP_mod_fnam
#include "../xa/xa_uid.h"                 // UID_but_END
#include "../xa/xa_mem.h"                 // memspc51, mem_cbuf1
#include "../xa/xa_aux.h"


// aus xa.h:



// Unix:
#define FILETRENNCHAR  '/'





//============================================================
// external Vars:

// aus ut_cvApp.c:
// extern double    cvApp_tol;        // Abweichung von Originalkurve
// extern double    cvApp_lmin;       // Approximation: MindestLineLen.


// aus xa.c

extern double     AP_txsiz;       // Notes-Defaultsize
extern double     AP_txdimsiz;    // Dimensions-Text-size


extern double APT_ModSiz;




//============================================================
// local Vars:

static Vector imp_off;
static int    imp_file;  // 0=Ausgabe ins mem, 1=Ausgabe ins File.
static FILE   *imp_lun = NULL;


static long   *imp_itab;
static long   imp_itab_siz;

       long   impTabSiz, impNr, impInd;
       ImpObj *impTab;


static double imp_scale;


static int   arNr;



//================================================================
  int AP_ImportWRL_ckTyp (char *fNam) {
//================================================================
/// returns  Mtyp_WRL or Mtyp_WRL2 or -1 (other format) -2(file-open-error)

// #VRML V1.0 ascii
// #VRML V2.0 utf8

  FILE   *fpi;
  char   s1[16], *cp1;

  // printf("AP_ImportWRL_ckTyp |%s|\n",fNam);

  // get first 10 chars of file


  // find "V1."  else it is a V2 ..
  if ((fpi = fopen (fNam, "rb")) == NULL) {
    printf ("*** Error open |%s|\n", fNam);
    return -2;
  }

  fread (s1, 12, 1, fpi);

  fclose(fpi);

  cp1 = strstr(s1, "V1.");
  if(cp1) return Mtyp_WRL;

  cp1 = strstr(s1, "V2.");
  if(cp1) return Mtyp_WRL2;

  return -1;

}


//===========================================================================
  int AP_Import_obj_CB (ObjGX *ox1) {
//===========================================================================
// Callback des Iges-Import;
// mit Open next Model
// mit obj ausgeben 

  int   i1, i2, irc;
  long  *indTab;
  char  cbuf[256];

  // printf("AP_Import_obj_CB %d\n",ox1->typ);



  // Typ_Done: skip this obj ..
  if(ox1->typ == Typ_Done) {
    return 0;



  //-----------------------------------------------------------
  // Typ_SubModel = Start new Model; data = Submodelname
  } else if(ox1->typ == Typ_SubModel) {

    printf("Start new Model |%s|\n",(char*)ox1->data);

    // open File  *imp_lun = NULL;
    if(imp_lun != NULL) {
      fprintf(imp_lun, "# import end\n");
      fclose (imp_lun);
      imp_lun = NULL;
    }


    // Mainmodel hat Modelname "" !
    if(strlen(ox1->data) < 1) {
      imp_file = 0;     // Mainmodel: Ausgabe ins mem

    } else {
      sprintf(mem_cbuf1,"# Import Submodel %s",(char*)ox1->data);
      UTF_add1_line (mem_cbuf1);

      sprintf(cbuf,"%sModel_%s",AP_get_tmp_dir(),(char*)ox1->data);
      printf("Submodel |%s|\n",cbuf);
      if((imp_lun=fopen(cbuf,"w")) == NULL) {
        TX_Print("Mod_chg__ E001 %s",cbuf);
        return -1;
      }
      fprintf(imp_lun, "# %s\n",OS_date1());
      imp_file = 1;     // Ausgabe ins File

      // den Namen in die ModelnameTable speichern
      // get ModelNr from Modelname - BasModelNr merken
      impTab[impInd].ind = DB_StoreModBas (1, ox1->data); // 1=internalModel

    }

    AP_obj_2_txt (NULL, 0L, ox1, 0L);  // reset Startindizes

    return 0;





  //-----------------------------------------------------------
  // CCV nicht via AP_obj_2_txt in Text umwandeln sondern sofort hier.
  } else if(ox1->typ == Typ_CVTRM) {
    // printf("CCV ex Igs %d\n",ox1->siz); 

    // nur objHeader holen ("S#=CCV")
    AP_obj_2_txt (mem_cbuf1, mem_cbuf1_SIZ, ox1, -1L);

    // add members ..
    indTab = ox1->data;
    for(i1=0; i1<ox1->siz; ++i1) {
      i2 = indTab[i1];
      // printf("  CCV[%d]=%d typ=%d ind=%d\n",i1,i2,impTab[i2].typ,impTab[i2].ind);
      if(impTab[i2].typ == Typ_Error) {
        TX_Print("IGES-ERR 001 - Obj %d",(i2*2)+1);
        return -1;
      }
      APED_oid_dbo__(cbuf, impTab[i2].typ,impTab[i2].ind);
      strcat(mem_cbuf1, " ");
      strcat(mem_cbuf1, cbuf);
    }

    // typ und DB-index of generated object eintragen !
    AP_obj_2_txt_query (&impTab[impInd].typ, &impTab[impInd].ind);
    // printf("  out %d: typ=%d ind=%d\n",impInd,
           // impTab[impInd].typ,impTab[impInd].ind);




  //-----------------------------------------------------------
  } else {
    // Objekt in Text umwandeln
    // irc = AP_obj_2_apt (mem_cbuf1, mem_cbuf1_SIZ, &elT, NULL);
    irc = AP_obj_2_txt (mem_cbuf1, mem_cbuf1_SIZ, ox1, -1L);
    if(irc < 0) return irc;


    // typ und DB-index of generated object eintragen !
    AP_obj_2_txt_query (&impTab[impInd].typ, &impTab[impInd].ind);
    // printf("  out %d: typ=%d ind=%d\n",impInd,
           // impTab[impInd].typ,impTab[impInd].ind);





  //-----------------------------------------------------------
  }

  // ObjektZeile raus;
  if(imp_file == 0) {  // Ausgabe ins mem
    UTF_add1_line  (mem_cbuf1);

  } else {  // Ausgabe ins File
    fprintf(imp_lun, "%s\n",mem_cbuf1);
  }

    // Next_Obj:;
    // ++impNr;
    // if(impNr >= impTabSiz) break;


  return 0;

}



//===========================================================================
  // void AP_ImportIges (char *off, int mode3d, char* fnam)  {
//===========================================================================
// mode3d: ON  = 0 = Ja,   als 2D-Mode behandeln;
// mode3d: OFF = 1 = Nein, als 3D-Mode behandeln;
// fnam = kompletter Igesfilename

	
/*
  int     i1, irc;
  double  min_dist;
  char    *p1, *p2;


  int     doOff, impID;
  int     AP_ind_p=20, AP_ind_c=20, AP_ind_l=20, AP_ind_s=20;

  double  d1, a_x, a_y, e_x, e_y, c_x, c_y;
  long    pt_ind, ln_ind, ci_ind, cv_ind;
  // Point2  pt21, pt22;
  Point   pt1;
  // ObjG    elAct, elT;
  ObjGX   *ox1, *ox2;
  // Vector  vcOff;
  // Point   ptOff;
  Plane   plOff;
  Mat_4x3 trOff;


  FILE    *fp1 = NULL, *fp2 = NULL, *fp3 = NULL;
  FILE    *ige_auxFilLun;
*/





  // //----------------------------------------------------------
  // printf("AP_ImportIges |%s|%s| 3D=%d\n",off,fnam,mode3d);

  // TX_Print("****  Bitte warten bis zur Meldung Einlesen fertig");

  // // AP_ImportIg1 (off, mode3d, fnam);
  // AP_Import__ ("xa_stp_r", "STP_r__", fnam);



/*

  // die ID-Tabelle -> memspc52
  // impTab = (ImpObj*)memspc52;
  // impTabSiz = sizeof(memspc52) / sizeof(ImpObj);
  impTab = (ImpObj*) malloc (20000);
  impTabSiz = 20000 / sizeof(ImpObj);
  impNr = 0;



  ox1 = (ObjGX*)memspc51;



  // Init Objektindexe
  AP_obj_2_txt (NULL, 0, ox1, 0);




  // pt -> Refsys
  UT3D_pl_XYZ (&plOff);
  UT3D_pt_txt (&plOff.po, off);
  TX_Print(" Origin: %f, %f, %f",plOff.po.x, plOff.po.y, plOff.po.z);
  if(UT3D_comp2pt(&plOff.po, &UT3D_PT_NUL, UT_TOL_min1)) {
    doOff = OFF;
  } else {
    doOff = ON;
    UT3D_m3_loadpl (trOff, &plOff);
  }



  // check Lic
  // Iges-Import frei: kein LicCheck.
  //if(ED_ckl() != 0) goto Fertig_2;




  // Open for read
  if((fp1=fopen(fnam,"r")) == NULL) {
    TX_Print("****** OPEN ERROR FILE %s **********\n",fnam);
    goto L_exit;
  } else {
    fp2 = fopen(fnam,"r");
    fp3 = fopen(fnam,"r");
  }




  // Textbuffer 1 loeschen
  UTF_clear1();


  sprintf(mem_cbuf1,"# IGES-Import %s",fnam);
  UTF_add1_line (mem_cbuf1);


  // 3D-einschalten
  // if(mode3d != ON) {
    // UTF_add1_line ("MODE 3D");
  // }



  // Init IGES-lesen
  i1 = IGE_r_init (AP_Import_obj_CB);
  //if(i1 != 0)  return;

  // alle Obj -> impTab laden
  if(IGE_r_sf_load (fp1, fp2, fp3) < 0) goto L_exit;

  // resolve ..
  IGE_r_sf_find (fp1, fp2, fp3);
  goto FERTIG;
  // exit(0);
*/

/*
  //---------------------------------------------------------
  for(i1=0; i1<impTabSiz; ++i1) {


    // das naechste Obj holen; -1=Grober Fehler; -2=Obj-defekt,weiter.
    // irc = IGE_r_rec (&elAct, fp1, fp2, fp3);
    irc = IGE_r_rec (&ox1, &impID, fp1, fp2, fp3);
    impTab[impNr].ID = impID;
    // hat Buffer in memspc54; ox1 ev in memspc55!
    if(irc < -1) continue;      // Error
    if(irc < 0) goto FERTIG;


    // printf("===== ige next: ===== %d %d %d\n",i1,ox1->typ,impTab[i1].ID);


    // //TX_Print(" > %d %d",bufSiz,strlen(mem_cbuf1));
    // if(bufSiz > UTF_FilBufSiz) {
      // TX_Error("Eingabedatei Überlauf");
      // goto Fertig_1;
    // }




    // CCV muss gesondert behandelt werden:
    // ox1 hat Tabelle der Iges-ID's; in impTab sind die zugehoerigen
    // gCad-typen und Indexe.
    if(ox1->typ == Typ_CVTRM) {
      irc = AP_iges_ccv2txt (mem_cbuf1, mem_cbuf1_SIZ, ox1);
      if(irc == -1) continue;      // Error
      if(irc == -2) goto FERTIG;   // grober Fehler
      goto L_obj_out;
    }


    // Objekt ox1 translieren
    // UT3D_obj_vctra (&ox1, &elAct, &vcOff);
    // ox2 = *ox1;
    if(doOff == ON) {
      ox2 = (ObjGX*)memspc51;
      UTO_ox_tra (&ox2, ox1, trOff);
      ox1 = (ObjGX*)memspc51;
    }


    // Objekt in Text umwandeln
    // irc = AP_obj_2_apt (mem_cbuf1, mem_cbuf1_SIZ, &elT, NULL);
    irc = AP_obj_2_txt (mem_cbuf1, mem_cbuf1_SIZ, ox1, -1);
    if(irc == -1) continue;      // Error
    if(irc == -2) goto FERTIG;   // grober Fehler


    // query typ und DB-index of generated object
    AP_obj_2_txt_query (&impTab[impNr].typ, &impTab[impNr].ind);
    // printf("  %d  typ=%d ind=%d ID=%d\n",impNr,
           // impTab[impNr].typ,impTab[impNr].ind,impTab[impNr].ID);


    // Zeile raus;
    L_obj_out:
    UTF_add1_line  (mem_cbuf1);

    Next_Obj:;
    ++impNr;
    if(impNr >= impTabSiz) break;
  }

  TX_Error("AP_ImportIges E001");
*/

/*
  //---------------------------------------------------------
  FERTIG:
  // 3D ausschalten
  // UTF_add1_line ("MODE 2D");

  if(imp_lun != NULL) {
    fprintf(imp_lun, "# import end\n");
    fclose (imp_lun);
  }


  sprintf(mem_cbuf1,"# End IGES-Import");
  UTF_add1_line (mem_cbuf1);




  // gesamte Datei schreiben
  Fertig_1:

  TX_Print("****  Einlesen fertig");
  fclose(fp1);
  fclose(fp2);
  fclose(fp3);


  // Textbuffer 1 in die Hauptdatei (nur memory!) einfuegen
  UTF_insert1(-1L);


  L_exit:
  free (impTab);
*/

  // return;



// }

/* 
//================================================================
  int AP_ImportXML (char* fnam) {
//================================================================
 
  int  irc;
  long is;
  char *p1;


  printf("AP_ImportXML |%s|\n",fnam);

  is = DB_dbo_get_last (Typ_SUR);   // A
  ++is;

  // LandXML-File einlesen; ../exp/lxml.c
  irc = lxml_read (fnam, AP_modact_nam, is);


  // Textbuffer 1 loeschen
  UTF_clear1();

  sprintf(mem_cbuf1, "A%ld=PTAB \"%s\"",is,AP_mod_fnam);
  UTF_add1_line (mem_cbuf1);
  ++is;
  sprintf(mem_cbuf1, "A%ld=MSH A%ld",is,is - 1L);
  UTF_add1_line (mem_cbuf1);

  // Textbuffer 1 in die Hauptdatei einfuegen
  UTF_insert1(-1L);


  return 0;

}
*/


//===========================================================================
void AP_ImportLwo(int mode, char *off, double scale, char* fnam) {
//===========================================================================

  int     i1;
  ObjGX   elAct;

  printf("AP_ImportLwo %d |%s|%s|\n",mode,off,fnam);


  imp_scale = scale;


  // store Offset
  UT3D_vc_txt (&imp_off, off);


  // Init Objektindexe
  DB_Init (1);
  AP_obj_2_txt (NULL, 0L, NULL, 0L);


  // Init Speicherbereich fuer Pointindextabelle
  imp_itab = (void*) memspc51;
  imp_itab_siz = sizeof(memspc51) / sizeof(long);
  // printf("imp_itab_siz=%d\n",imp_itab_siz);


  // Textbuffer 1 loeschen
  UTF_clear1();


  // work; 2 callbacks:  lwo_cb_savePoint u. lwo_cb_saveFace
  arNr = 20;    // init surface-startindex (A20)
  i1 = lwo_read (fnam);
  if (i1 < 0) {
    TX_Error("Can't read LWO-File |%s|\n", fnam);
    return;
  }


  // Textbuffer 1 in die Hauptdatei einfuegen
  UTF_insert1(-1L);


  return;

}


//============================================================
int lwo_cb_savePoint (int ip, float *pt1) {
//============================================================
// callback von lwo_read

  int     irc, i1;
  long    ind;
  Point   p1;
  ObjGX   elAct, elT;

  // printf("lwo_cb_savePoint %d %f,%f,%f\n",ip,pt1[0],pt1[1],pt1[2]);

  p1.x = pt1[0] * imp_scale;
  p1.y = pt1[1] * imp_scale;
  p1.z = pt1[2] * imp_scale;
  
  elAct.typ  = Typ_PT;
  elAct.form = Typ_PT;
  elAct.siz  = 1;
  elAct.data = &p1;


  // Objekt translieren
  // UT3D_obj_vctra (&elT, &elAct, &imp_off);


  // Objekt in Text umwandeln
  irc = AP_obj_2_txt (mem_cbuf1, mem_cbuf1_SIZ, &elAct, -1L);
  if(irc < 0) return irc;

  if(ip >= imp_itab_siz) return -2;

  AP_obj_2_txt_query (&i1, &ind);
  imp_itab[ip] = ind;  // save original indexNr

  // // Zeile -> Textbuffer 1 raus
  // UTF_add1_line  (mem_cbuf1);


  return 0;
}


//============================================================
int lwo_cb_saveFace (int vNr, int *vTab) {
//============================================================


  int   i1, ind;
  char  auxBuf[32];


  // printf("lwo_cb_saveFace A%d nNr=%d  ",arNr,vNr);

  sprintf(mem_cbuf1, "A%d=RCIR",arNr);
  ++arNr;


  // for(i1=0; i1<vNr; ++i1) {
  for(i1=vNr-1; i1>=0; --i1) {
    // printf("%4d(%d)  ",vTab[i1],imp_itab[vTab[i1]]);
    ind = imp_itab[vTab[i1]];
    sprintf(auxBuf, ",P%d",ind);
    strcat(mem_cbuf1, auxBuf);
  }
  // strcat(mem_cbuf1, "\n");
    // printf("\n");

  // printf(mem_cbuf1);
  UTF_add1_line  (mem_cbuf1);

  return 0;
}


//===========================================================================
void AP_Import3ds(int mode, char *off, double scale, char* fnam) {
//===========================================================================

  int     i1;
  ObjGX   elAct;

  printf("AP_Import3ds %d |%s|%s|\n",mode,off,fnam);

  imp_scale = scale;


  // store Offset
  UT3D_vc_txt (&imp_off, off);


  // Init Objektindexe
  DB_Init (1);
  AP_obj_2_txt (NULL, 0L, &elAct, 0L);


  // Init Speicherbereich fuer Pointindextabelle
  imp_itab = (void*) memspc51;
  imp_itab_siz = sizeof(memspc51) / sizeof(long);
  // printf("imp_itab_siz=%d\n",imp_itab_siz);


  // Textbuffer 1 loeschen
  UTF_clear1();


  // work
  arNr = 20;     // init surface-startindex (A20)
  i1 = ds3_read (fnam);
  if (i1 < 0) {
    TX_Error("Err reading 3DS-file %s\n", fnam);
    return;
  }

  // Textbuffer 1 in die Hauptdatei einfuegen
  UTF_insert1(-1L);

  return;

}



//============================================================
int ds3_cb_savePoint (int ip, float *pt1) {
//============================================================

  lwo_cb_savePoint (ip, pt1);

  return 0;
}


//============================================================
int ds3_cb_saveFace (int vNr, int *vTab) {
//============================================================

  lwo_cb_saveFace (vNr, vTab);

  return 0;
}


/*
//===========================================================================
void AP_ImportDxf(int mode, char *off, char* fnam) {
//===========================================================================
/// load DXF-file -> memory
/// All coordinates are translated !

// TrVec: vc1 (from $EXTMIN bis $EXTMAX)

// Memory-Usage:
// memspc51    ox1
// memspc55    namTab
// memspc101   translated objData
// memspc102   ox2
// memspc501   objData
// mem_cbuf1   act codeline

  int      i1, ism, irc, doOff, oSiz, iaErr[3], iaImp[8], iaAux[8];
  double   d1, d2, d3, min_dist, dTab[10];

  char     *pf, cbuf[256];
  Point    pt1;
  Vector   vc1;

  FILE     *fp = NULL, *fp1 = NULL;
  FILE     *dxf_auxFilLun;

  Memspc   wrkSpc, wrk1Spc;
  ObjGX    *ox1, *ox2;
  Plane    plOff;
  Mat_4x3  trOff;
  ModelRef *mr;

  UtxTab_NEW (namTab);

  for(i1=0; i1<3; ++i1) iaErr[i1] = 0;
  for(i1=0; i1<8; ++i1) iaImp[i1] = 0;


  //-----------------------------------------------------

  // printf("AP_ImportDxf %d |%s|%s|\n",mode,off,fnam);
  printf("AP_ImportDxf |%s|%s|\n",off,fnam);


  Mod_kill__ ();   // alle tmp/Model_* loeschen


  // Init Objektindexe
  ox1 = (ObjGX*)memspc51;
  AP_obj_2_txt (NULL, 0, NULL, 0);



  // pt -> Refsys
  UT3D_pl_XYZ (&plOff);              // load DefaultRefsys

  UT3D_pt_txt (&plOff.po, off);      // set RefsysOrigin = inputPoint

  // TX_Print(" Origin: %f, %f, %f",plOff.po.x, plOff.po.y, plOff.po.z);
  if(UT3D_comp2pt(&plOff.po, &UT3D_PT_NUL, UT_TOL_min1)) {
    doOff = OFF;
  } else {
    doOff = ON;
    UT3D_m3_loadpl (trOff, &plOff);
  }



  // check Lic
  // Testversion: Kein Check on ReadDXF.
  //if(ED_ckl() != 0) goto FERTIG;



  // Open for read
  if((fp=fopen(fnam,"r")) == NULL) {
    TX_Print("****** OPEN ERROR INPUT-FILE %s **********\n",fnam);
    return;
  } else {
    fp1=fopen(fnam,"r");
  }


  // Pfad weg.
  pf = strrchr(fnam, FILETRENNCHAR);
  if(pf) {
    ++pf; // letzten \ weg
  } else {
   pf=fnam;
  }
  

  // Textbuffer 1 loeschen
  UTF_clear1();

  // init StringList for subModelnames
  UtxTab_init_spc (&namTab, memspc55, sizeof(memspc55));

  sprintf(mem_cbuf1,"# DXF-Import %s",pf);
  UTF_add1_line (mem_cbuf1);

  // Notes- und Dimensions-Text-Defaultsize setzen
  for(i1=0; i1<10; ++i1) dTab[i1] = 0;

  i1 = dxfr_init (dTab, fp, fp1);
  if(i1 != 0) {irc = -2; goto Fertig_1;}

  if(dTab[0] > 0.01) AP_txsiz    = dTab[0];
  if(dTab[1] > 0.01) AP_txdimsiz = dTab[1];

  GR_InitGFPar (AP_txdimsiz);


  // die DXF-Input-Boxgrenzen sind dTab[2-4] / dTab[5-7] ($EXTMIN bis $EXTMAX)
  // Abstaende vom 0-Punkt erechnen
  d1 = UT3D_len_2pt ((Point*)&dTab[2], (Point*)&UT3D_PT_NUL);
  d2 = UT3D_len_2pt ((Point*)&dTab[5], (Point*)&UT3D_PT_NUL);
  // printf(" d1=%f d2=%f d3=%f\n",d1,d2,APT_ModSiz);

  // d1 = DMIN (fabs(d1), fabs(d2));
  if((fabs(d1) > APT_ModSiz)||(fabs(d2) > APT_ModSiz)) {
    UT3D_pt_mid2pt (&pt1, (Point*)&dTab[2], (Point*)&dTab[5]);
    DEB_dump_obj__ (Typ_PT, &pt1, "imp-center");


    if(doOff == OFF) {
      doOff = ON;
      TX_Print("Model out of ModelSize - translated");

      // UT3D_pl_XYZ (&plOff);
      // // UT3D_pt_txt (&plOff.po, off);
      // plOff.po.x = -pt1.x;
      // plOff.po.y = -pt1.y;
      // plOff.po.z = -pt1.z;
      // TX_Print(" Origin: %f, %f, %f",plOff.po.x, plOff.po.y, plOff.po.z);
      // UT3D_m3_loadpl (trOff, &plOff);

      UT3D_vc_pt (&vc1, &pt1);
      UT3D_vc_invert (&vc1, &vc1);
      DEB_dump_obj__ (Typ_VC, &vc1, "transl.Vec:");
      UTRA_def__ (1, Typ_VC, &vc1);
    }

    // $EXTMIN 1.000000E+20         $EXTMAX -1.000000E+20
    if(UT3D_compvc0(&vc1, 1) == 1) {
      doOff = OFF;
    }
  }


  // doOff = OFF;  // TEST ONLY !


  //---------------------------------------------------------
  for(i1=0; i1<250000; ++i1) {


    // TX_Print("===== next: =====\n");

    UME_init (&wrkSpc, memspc501, sizeof(memspc501));

    // get next record (object) from dxf-file;
    irc = dxfr_rec__ (&ox1, fp, fp1, &wrkSpc);
    if(irc > 1) goto FERTIG;   // 2 = FERTIG
    if(irc > 0) continue;   // minor error
    if(irc < 0) goto FERTIG;   // -1: unrecov. Err
    if(ox1->typ == Typ_Error) continue;   // minor error


    // subModelCalls: save subModelname in StringList namTab
    if(ox1->typ == Typ_Model) {
      mr = ox1->data;
        // printf(" >>>>>>>>>>>>>>>>>> Model |%s|\n",mr->mnam);
      UtxTab_add_uniq__ (&namTab, mr->mnam);
    }



    // Objekt translieren
    if(doOff == ON) {
      // ox2 = (ObjGX*)memspc52;
      // irc = UTO_ox_tra (&ox2, ox1, trOff);
      UME_init (&wrk1Spc, memspc101, sizeof(memspc101));
      ox2 = (ObjGX*)memspc102;
      oSiz = sizeof(memspc102);
      irc = UTRA_app_obj (ox2, &oSiz, ox1, wrk1Spc);
      if(irc < 0) continue;      // Error
      ox1 = (ObjGX*)memspc102;
    }


    // Objekt in Text umwandeln
    irc = AP_obj_2_txt (mem_cbuf1, mem_cbuf1_SIZ, ox1, -1);
    if(irc == -1) {iaErr[0] += 1; continue;}      // Error
    if(irc == -2) goto FERTIG;   // grober Fehler


    Next_Obj:;

  }










  //---------------------------------------------------------
  FERTIG:
  sprintf(mem_cbuf1,"### End DXF-Import");
  UTF_add1_line (mem_cbuf1);



  Fertig_1:


  // query DB-indices
  AP_stru_2_txt (NULL, 0, iaImp, 1);
    // printf(" iaImp %d %d %d %d\n",iaImp[0],iaImp[1],iaImp[2],iaImp[3]);


  // write code -> file
  if(irc > -2) {
    // Textbuffer 1 in die Hauptdatei einfuegen
    // UTF_insert1(-1L);

    sprintf(cbuf, "%s%cModel_",AP_get_tmp_dir(),fnam_del);
    UTF_file_Buf1__ (cbuf);

  }



  //================================================================
  // output subModels
  ism = -1;

  L_nxt_sm:
  ism += 1;
  if(ism >= namTab.iNr) goto L_exit;

  pf = UtxTab__ (ism, &namTab);     // get string (subModelName)
    // printf(" nxt sm %d |%s|\n",ism,pf);

  // reset DB
  WC_Init_all (0);
  GA_hide__ (-1, 0, 0);               // reset HideList
  AP_obj_2_txt (NULL, 0, NULL, 0);    // reset objIndices

  // init dxf (find start of block
  dxfr_block_find (fp, fp1, pf);

  // start 
  UTF_clear1();  // Textbuffer 1 loeschen

  sprintf(mem_cbuf1,"# DXF-Import BLOCK %s",pf);
  UTF_add1_line (mem_cbuf1);


  // loop tru block
  for(i1=0; i1<250000; ++i1) {
    UME_init (&wrkSpc, memspc501, sizeof(memspc501));
    irc = dxfr_rec__ (&ox1, fp, fp1, &wrkSpc);
    if(irc > 1) goto L_sm_done;           // 2 = FERTIG
    if(irc > 0) continue;
    if(ox1->typ == Typ_Error) continue;   // minor error

    // subModelCalls: save subModelname in StringList namTab
    if(ox1->typ == Typ_Model) {
      mr = ox1->data;
      UtxTab_add (&namTab, mr->mnam);
        printf(" >>>>>>>>>>>>>>>>>> Model |%s|\n",mr->mnam);
    }

    // Objekt in Text umwandeln
    irc = AP_obj_2_txt (mem_cbuf1, mem_cbuf1_SIZ, ox1, -1);
    if(irc == -1) {iaErr[0] += 1; continue;}      // Error
    if(irc == -2) goto L_sm_done;         // grober Fehler

  }



  // write code -> file
  L_sm_done:

  // write code -> file
  if(irc > -2) {
    // Textbuffer 1 in die Hauptdatei einfuegen
    // UTF_insert1(-1L);

    sprintf(cbuf, "%s%cModel_%s",AP_get_tmp_dir(),fnam_del,pf);
    UTF_file_Buf1__ (cbuf);

  }

  // query DB-indices
  AP_stru_2_txt (NULL, 0, iaAux, 1);
  for(i1=0; i1<8; ++i1) iaImp[i1] += iaAux[i1];


  // UtxTab_nr    // get nr of strings UtxTab_nr
  goto L_nxt_sm;



  //================================================================
  L_exit:

  if(fp)fclose(fp);
  if(fp1)fclose(fp1);

    printf("exit AP_ImportDxf \n");

  // report unsupported objects
  dxf_log();

  // report Errors
  if(iaErr[0] > 0)
    TX_Print("*** %d objects with errors (not imported)",iaErr[0]);


  // report nr of imported objects
  sprintf(cbuf,
    "imported: %d points, %d lines, %d circles, %d curves, %d surfaces, %d notes, %d subModels",
               iaImp[6],  iaImp[5], iaImp[4],   iaImp[0],  iaImp[1],   iaImp[3],  iaImp[7]);
  TX_Print("%s",cbuf);

  return;


}

*/

/*
//=============================================================================
  void AP_ExportDxf (char* fnam, int dxfVersion) {
//=============================================================================
//   dxfVersion    0=DxfR12  1=Dxf2000

  int      i1, irc, tra_ind, tra_act, apt_typ, anz_obj=0, anz_sur=0;
  long     l1, apt_ind;
  long     gr_ind;
  // Point   pt1;
  // Line    ln1;
  // Circ    ci1;
  ObjGX    ox1, *op1;
  Plane    pl1;
  Mat_4x3  m1;
  DL_Att   dla;
  FILE     *fp;


  printf("AP_ExportDxf %d |%s|\n",dxfVersion,fnam);



  // // ev. vorhandene Dateiendung abschneiden.
  // int i1 = docTit.Find('.', 0);
  // if(i1 < 0) {
    // docTit += ".dxf";
  // } else {
    // docTit = docTit.Left(i1) + ".dxf";
  // }



  if ((fp = fopen (fnam, "w+")) == NULL) {
    TX_Error ("open file %s",fnam);
    return;
  }


  // check Lic
  // if(ED_ckl() != 0) goto FERTIG;


   // Fuer die Testversion: auskommentieren, goto FERTIG
  dxfw_init (fp, dxfVersion);
  // goto FERTIG;



  //============================================================================
  // Alle GR_Obj raus

  gr_ind = GL_Get_DLind();

  for(l1=0; l1<gr_ind; ++l1) {

    // printf(" expdxf %d\n",l1);


    // // ein transformiertes Objekt holen
    // irc = AP_GetObj (&o1, l1);
    // // apt_typ = DL_dbTyp__dli(l1);
    // // apt_ind = DL_get_dbi(l1);
    // // tra_ind = DL_GetTrInd(l1);
    // // i1 = DB_Get_GR_Obj (&o1, apt_typ, apt_ind, tra_ind);
    // if(irc < 0) continue;  // skip unknown types


    // apt_typ = DL_dbTyp__dli(l1);
    // apt_ind = DL_get_dbi(l1);
    // tra_ind = DL_GetTrInd(l1);


    // dla = DL_GetAtt(l1);                      // get alle DL-Infos
    DL_dlRec__dli (&dla, l1);


    // if(dla.dim    != OFF) continue;           // skip dimmed objects
    if(dla.disp   == OFF) continue;           // skip hidden obj's

    apt_typ = dla.typ;
    apt_ind = dla.ind;
    // tra_ind = dla.refInd;

    // printf(" typ=%d ind=%ld tra=%d\n",apt_typ,apt_ind,tra_ind);


    // skip surfs
    if(apt_typ == Typ_SUR) { anz_sur += 1; continue; }   // erst em Ende ..


    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;  // skip unknown types



    // if(tra_ind > 0) {
// 
      // if(tra_act != tra_ind) {
         // DB_GetRef (&pl1, tra_ind);
         // UT3D_m3_loadpl (m1, &pl1);
         // tra_act = tra_ind;
      // }
// 
      // op1 = (ObjGX*)memspc54;
      // UTO_ox_tra (&op1, &ox1, m1);
      // tra_ind = 0;
// 
    // } else {

      op1 = &ox1;
    // }

    // DEB_dump_ox_0 (op1, 1);

    irc = dxfw_rec (op1, fp); // Element ausgeben
    // printf(" ex dxfw_rec %d\n",irc);
    if(irc >= 0) ++anz_obj;

  }



  //----------------------------------------------------------------
  // dxfw_end wird nun vom TSU_exp__ (Surfaces ausgeben) erledigt
  // TSU_exp__ folgt nach AP_ExportDxf
  // dxfw_end (fp);


  fclose (fp);


  //sprintf(mem_cbuf1,"notepad %s",fnam);
  //system(mem_cbuf1);

  if(DB_get_ModBasNr() > 0) {
    TX_Error("- DXF cannot yet export subModels ..");
  }


  if(anz_obj < 1) {
    TX_Print("  objects exported:  0");
    return;
  }

  // Statistik ausgeben
  // FERTIG:
  TX_Print("DXF-Export into file %s",fnam);
  TX_Print("   objects exported:  %d",anz_obj);

  return;

}


*/


//=================================================================
  int AP_appr (char *obj, char *ref, double tol, double lmin,int mode) {
//=================================================================
// Approximieren;   Curve -> Circ
//   mode     1=anzeigen und Hiliten; 2=abspeichern
// - Curve -> ptTab
// - ptTab transformieren
// - Approximieren
// - Kreise/Lines generieren
// - Curves loeschen


  int      iCrv, iTra, i1, irc, pTab1Nr, p2TabNr, o2TabNr, ptMax;
  long     l1;
  void     *pa, *pe;
  double   tol_cv, tol_ln;
  Point2   *p2Tab;
  Point    pt1, *pTab1;
  Plane    pl1;
  Mat_4x3  m1;
  ObjG2    *o2Tab;
  ObjGX    o1, *cv1;
  // FILE    *auxFilLun;


  printf("AP_appr obj=|%s| ref=|%s| tol=%f lmin=%f mode=%d\n",
                        obj,ref,tol,lmin,mode);





  iCrv = atoi(obj);

  iTra = atoi(ref);
  // if(iTra != 0) TX_Error("Tranformation dzt nicht implementiert\n");




  cv1 = DB_GetCurv ((long)iCrv);



  //--------------------------------------------------------------
  if(cv1->typ == Typ_CVPOL2) {
    p2Tab = cv1->data;
    p2TabNr = cv1->siz;
    goto L_appr_start;



  //--------------------------------------------------------------
  // Typ_CVPOL -> Typ_CVPOL2 umwandeln .................
  } else if(cv1->typ == Typ_CVPOL) {
    pTab1 = cv1->data;
    p2Tab = (Point2*)memspc51;
    p2TabNr = sizeof(memspc51) / sizeof(Point2);
    if(p2TabNr <= cv1->siz) {
      TX_Error("Buffer zu klein");
      return -1;
    }
    // Test: pTab1 -> p2Tab umkopieren.
    for(i1=0; i1<cv1->siz; ++i1) {
      p2Tab[i1].x = pTab1[i1].x;
      p2Tab[i1].y = pTab1[i1].y;
    }
    p2TabNr = cv1->siz;
    goto L_appr_start;






  //--------------------------------------------------------------
  // Typ_CVELL                            siehe APT_DrawCurv
  } else if(cv1->typ == Typ_CVELL) {
    pTab1 = (Point*)memspc53;
    ptMax = sizeof(memspc53) / sizeof(Point);
    // ELL -> Polygon: Tol tol od UT_TOL_pt ?
    // irc = UT3D_cv_ell (pTab1,&pTab1Nr,cv1->data,ptMax,UT_TOL_pt);
    i1 = UT3D_ptNr_ell (cv1->data, UT_TOL_pt);
    if(i1 > ptMax) goto L_EOM;
    UT3D_npt_ell (&pTab1Nr, pTab1, cv1->data);

    goto L_translate;



  //--------------------------------------------------------------
  // Typ_CVBSP                            siehe APT_DrawCurv
  } else if(cv1->typ == Typ_CVBSP) {
    pTab1 = (Point*)memspc53;
    ptMax = sizeof(memspc53) / sizeof(Point);
    // BSP -> Polygon: Tol tol od UT_TOL_pt ?
    irc = bspl_pol_bsp (&pTab1Nr,pTab1,cv1->data,ptMax,UT_TOL_pt);

    goto L_translate;



  //--------------------------------------------------------------
  }


  TX_Error("unsupported: typ=%d form=%d",cv1->typ,cv1->form);
  return -1;






  //==========================================================
  L_translate:
  // Input hier: (Point)pTab1[pTab1Nr]

/*
    // translate
    DB_GetRef(&pl1, m1, iTra);
    for(i1=0; i1<pTab1Nr; ++i1) {
      UT3D_pt_tra_pt_m3 (&pTab1[i1], m1, &pTab1[i1]);
    }
*/

    // -> p2Tab umkopieren
    p2Tab = (Point2*)memspc51;
    p2TabNr = sizeof(memspc51) / sizeof(Point2);
    for(i1=0; i1<pTab1Nr; ++i1) {
      p2Tab[i1] = UT2D_pt_pt3 (&pTab1[i1]);
    }
    p2TabNr = pTab1Nr;
    goto L_appr_start;




  //==========================================================
  // - Approximieren
  // Input hier: (Point2)p2Tab[p2TabNr]
  L_appr_start:

  printf("L_appr_start %d\n",p2TabNr);


  // UT_TOL_cv u UT_TOL_ln setzen und nach cvApp_app
  // wieder zuruecksetzen !!!
  tol_cv = UT_TOL_cv;
  tol_ln = UT_TOL_ln;
  UT_TOL_cv = tol;
  UT_TOL_ln = lmin;



  // fuer die Ergebnisobjekte (Circ's)
  o2Tab = (void*) memspc53;
  o2TabNr = sizeof(memspc53) / sizeof(ObjG2);


  // Polygon (p2Tab[pTab1Nr] in Line/Circ umwandeln (-> o2Tab[oTabNr])
  cvApp_app (o2Tab, &o2TabNr, p2Tab, p2TabNr);
  printf("ex cvApp_appr_s o2TabNr=%d\n",o2TabNr);


  // reset
  UT_TOL_cv = tol_cv;
  UT_TOL_ln = tol_ln;


  // nur zum Test: das Testpolygon in eine Datei raus
  cvApp_test_wr (p2Tab, p2TabNr);
  // testen mit main aus ut_cvApp.c


  // imply END-Button (Redraw)  (vorherige testausgaben loeschen)
  // UI_AP (UI_FuncSet, UID_but_END, NULL);
  ED_work_END (0);



  // printf("IG_oTabNr=%d\n",IG_oTabNr);
  if(o2TabNr < 1) return -2;




  //---------------------------------------------------------------
  // die Objekte ausgeben 



  //...................................................
  if(mode == 1) {                 // test - nur Hilite 


    // GLB_DrawInit ();
    TX_Print(" %d objects ..",o2TabNr);


  //...................................................
  } else if(mode == 2) {         // save

    // Textbuffer 1 loeschen
    UTF_clear1();

    // Init Objektindexe
    AP_obj_2_txt (NULL, 0L, &o1, 0L);

  }



  //------------------------------------------------------
  for(i1=0; i1<o2TabNr; ++i1) {

    //...................................................
    if(mode == 1) {           // test - anzeigen

        printf(" [%d] typ=%d\n",i1,o2Tab[i1].typ);
      APT_disp_ac (&o2Tab[i1], Typ_Att_hili1, 0.0);



    //...................................................
    } else if(mode == 2) {    // abspeichern

      // cvApp_appr_s liefert 2D-Objekte; wieder -> 3D-Objekte umwandeln.
      // o1 = UT3D_obj_obj2 (&o2Tab[i1]);
      if((o2Tab[i1].typ == Typ_CI)||(o2Tab[i1].typ == Typ_CI2)) {
        o1.typ  = Typ_CI;
        o1.form = Typ_CI2;
      } else {
        o1.typ  = Typ_LN;
        o1.form = Typ_LN2;
      }
      o1.siz  = 1;
      o1.data = memspc55;
      UT2D_void_obj2 (memspc55, &l1, &o2Tab[i1]);


      // Objekt in Text umwandeln
      irc = AP_obj_2_txt (mem_cbuf1, mem_cbuf1_SIZ, &o1, -1L);
      if(irc == -1) continue;      // Error
      if(irc == -2) goto FERTIG;   // grober Fehler
      // UTF_add1_line  (mem_cbuf1);    // abspeichern
    }


  }


  //--------------------------------------------------------------
  FERTIG:

    //...................................................
  if(mode == 1) {            // test
    GLB_DrawInit ();
    GL_Redraw ();
    GLB_DrawExit ();


    //...................................................
  } else if(mode == 2) {     // save

    // Textbuffer 1 in die Hauptdatei einfuegen
    UTF_insert1(-1L);

    // die Hauptdatei raus ins Editfenster
    ED_load__ ();

/*
    // Curve iCrv loeschen
    sprintf(mem_cbuf1, "S%d=",iCrv);
    i1 = AP_search_def (mem_cbuf1);
    printf("del iCrv=%d lNr=%d\n",iCrv,i1);
    AP_obj_del2 (i1, "_", 1);
*/

    // hide Curve iCrv
    l1 = DL_dli__dbo (Typ_CV, iCrv, -1L);
    GA_view__ (l1, 1, 0, 0L);
    // DL_hide__ (l1, OFF);
    // GA_hide__ (3, l1, 0);
    // DL_Redraw ();


  }


  return 0;

  L_EOM:
    TX_Error("AP_appr EOM"); return -1;

}


//========================= EOF =================================
