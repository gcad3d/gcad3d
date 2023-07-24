/* gcad3d-plugin export iges.                     3.Version 2023-07-01
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
Color - D2/2  17-24   - negative index of 314-def ?
  // 314,<red>,<green>,<blue>[,<colorNam>];
  // color = double 0-100; colorNam eg "5HGRAY1"
  ????

Solids: does not work on cascade ...
  resolv to surfaces .. 

text: .. todo ..   does not work on cascade ...

dimensions: .. todo ..


-----------------------------------------------------
Modifications:
2000-04-28  new from V2 of 2001-06-17
-----------------------------------------------------
\file  ../exp/ige_w.c
\brief write iges files 

=====================================================
List_functions_start:

IGE_w__          main entry
IGE_w_model      export subModel
IGE_w_init       write H-block
IGE_w_rT         join files, write T-Block
IGE_w_ogx__      UU
IGE_w_obj__      export CAD-obj
IGE_w_obj_dist   create entity-data and export
IGE_w_PT
IGE_w_LN
IGE_w_CI
IGE_w_ELL
IGE_w_PLG
IGE_w_CLOT
IGE_w_P_POL2     UU
IGE_w_BSPL
IGE_w_PSPL       UU
IGE_w_NTE        TODO
IGE_w_CCV
IGE_w_PLG        polygon Typ_CVPOL CurvPoly > Ent 106/12
IGE_w_SRV        revolved-surface iges-entity 120
IGE_w_SRU        RuledSurface 118
IGE_w_SRC        ruled-surface; Tabulated-Cylinder-Entity (curve/vector) 122
IGE_w_SBS        Rational B-Spline Surface 128
IGE_w_SUTP       trimmed-perforated-surf - 144
IGE_w_PRI        solid prism 164    UU
IGE_w_BND        boundary IG_EntBND 142
IGE_w_MD         SubfigDef. 308
IGE_w_MR         SubfigInst. 408
IGE_w_106        106 Copious Data
IGE_w_124        124 trMat

IGE_w_obj_init   init D- and P-lines
IGE_w_P_pt1
IGE_w_P_vc1
IGE_w_P_db1
IGE_w_P_i1
IGE_w_P_in       add n ints to P-line
IGE_w_P_txt
IGE_w_obj_out__  write P-lines and D-lines
IGE_w_obj_outD
IGE_w_obj_outP
IGE_w_eTab_add   store cadTyp and dbi of exported obj in expTab
IGE_w_eTab_find    find cad-obj (typ,dbi) in exported-obj-list expTab
IGE_w_eTab_clr

IGE_w_dump_eTab
IGE_w_dump_cadObj

List_functions_end:
=====================================================


//----------------------------------------------------------------
Every entity has 2 D-lines and 1 P-line-block.

 D-line1 and D-line-2: 9 Integers/Chars with 8 Chars; 2 lines per obj
       0       1       2       3       4       5       6       7       8D     9
                                                ...matID        01234567
    1-8     9-16   17-24   25-32   33-40   41-48   49-56   57-64   65-72  74-80



D-line1: ---------------
0   1-8  Entity-type. Eg 100=Circ; Same as D2/0 and P-first word.
1   9-16 lineNr of P-line (col. 74-80 of P-line)
7  57-64 0 or TrMat (eg for CI=100 the ? of the corresponding trMat(124))
8  65-72 4 status a 2 digits;
   65,66: 00=Visible, 01=Blanked(hidden)
   67,68: dependency; 00=not-dep; 01=is-child=dep; 02=can-exist; 03=dep;
   69,70: 00=geom; 01=annotation; 02=definition;
   71,72: Hierarchie; 00=TopDown
-     73 "D"
9  74-80 Dline1-Nr; first is 1;


D-line2: ---------------
0   1-8  Entity-type.
1   9-16 Weight in mm/1000; 0|200|1000
2  17-24 color;0=None;1=Black,2=red,3=green,4=blue;5=yellow,6=magenta,7=cyan,8=white;
         neg:Pointer -> 314
3  25-32 nr of P-lines;
4  33-40 FormNr = entity-subTyp;
7  57-64 cadObjID
8  65-72 "Subscr"; - 0 
-     73 "D"
9  74-80 Dlinei2-Nr; first is 2;


P-line-block: ---------------
1-64  integers/doubles; separator is ',' end is ';' First is Entity-type.
  65  space
66-72 LineNr of corresponding D-line1
   73 'P'
74-80 PlineNr; first is 1;


//----------------------------------------------------------------
Entities:
IG_EntCCV 102 composite-curve               IGE_w_CCV
IG_EntTRA 124 trMat                         IGE_w_124
IG_EntPLN 108 Plane (also 190)              IGE_w_PLN

.....................................
IG_EntPT  116 Point                         IGE_w_PT
IG_EntLN  110 Line                          IGE_w_LN
IG_EntCI  100 Circ                          IGE_w_CI
IG_EntELL 104 Ellipse                       IGE_w_ELL
IG_EntPOL 106 polygon                       IGE_w_PLG
IG_EntBND 142 CurveOnParametricSurface      IGE_w_BND
          
.....................................
IG_EntPLN 108 Plane                         IGE_w_P_PLN
IG_EntSRU 118 RuledSurface (2 curves)       IGE_w_SRU
IG_EntSRV 120 Revolved Surf                 IGE_w_SRV IGE_w_P_SRV
IG_EntSRC 122 RuledSurface (circ + Vector)  IGE_w_SRC
IG_EntSBS 128 Rational B-Spline Surface     
IG_EntTS  143 Trimmed Surface, no holes     
IG_EntSTP 144 TrimmedPerforatedSurf (TPS)   IGE_w_SUTP


//----------------------------------------------------------------
Iges-doku /mnt/serv2/devel/cadfiles/igs/doc/version6.pdf
- D-parDescr: S.52
- IGE_w_PLG 106 Polygon    S.405
- IGE_w_MD  308 SubfigDef. S.405
- IGE_w_MR  408 SubfigInst. S.585
  -         210 General label
- IGE_w_NTE 212 General note S.284
            124 Transformation Matrix S.151
            164  Solid of Linear Extrusion S.232
            128 Plane S.121
  IGE_w_SRU 118 ruled-surf; S.139
            143 IG_EntTS; S.207  
Solids:
block    150
cylinder 154
cone     156
sphere   158 S.
torus    160
revSol   162

//----------------------------------------------------------------

*/


// definition "export"
#include "../xa/export.h"


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"
#include "../ut/ut_cast.h"             // INT__PTR
#include "../ut/ut_ox_base.h"             // OGX_SET_INDEX
#include "../ut/ut_txt.h"
#include "../ut/ut_TX.h"
#include "../ut/ut_memTab.h"           // MemTab
#include "../ut/ut_bspl.h"
// #include "../ut/ut_crv.h"

#include "../db/ut_DB.h"                  // DB_GetCurv

#include "../gr/ut_DL.h"                  // DL_IS_HIDDEN
#include "../gr/vf.h"                     // GL_vf1_CS

#include "../xa/xa_mem.h"                 // memspc101
#include "../xa/xa_sele.h"                // Typ_go*
#include "../xa/mdl__.h"                  // SIZMFTot
#include "../xa/ap_dir.h"                 // AP_get_tmp_dir
#include "../xa/xa_msg.h"              // MSG_*



typedef_MemTab(int);
typedef_MemTab(Point);


//----------------------------------------------------------------

#define IG_EntPT  116
#define IG_EntLN  110
#define IG_EntCCV 102
#define IG_EntPOL 106
#define IG_EntPLN 108
#define IG_EntELL 104
#define IG_EntPS  112  // TODO
#define IG_EntSRU 118  // -
#define IG_EntSRC 122  // curve + vector IGE_w_SRC
#define IG_EntSRV 120  // revolved; axis + curve .. IGE_w_SRV
#define IG_EntTRA 124  // transformation
#define IG_EntBND 142  // use Typ_goGeo1 / Boundary = CurveOnParametricSurface
#define IG_EntTS  143  // bounded surface, no holes
#define IG_EntSTP 144  // bounded surface, with holes
#define IG_EntBS  126  // Rational B-Spline Curve
#define IG_EntSBS 128  // Rational B-Spline Surf
#define IG_EntCI  100
#define IG_EntTX  212
#define IG_EntEXT 164  // Solid of Linear Extrusion / PRISM
#define IG_EntMR  408
#define IG_EntMD  308

//#define IG_EntBnd 141 // boundary (no holes)
//#define IG_EntCOF 130 // Offset-Curve; TODO
//#define IG_EntSOF 140 // Offset-Surface; TODO


static int  oTotNr=0, oExpNr=0;
static int  IG_D_line_nr, IG_S_line_nr, IG_G_line_nr, IG_P_line_nr;


// typ   DB-typ
// ind   DB-index
// iEnt  iges-entity-nr
// clo   for 106 PLG: 0=closed-curve, 1=open-curve; -1=undefined
typedef struct {int ind; int typ, iEnt; char clo,i2,i3,i4;}            ExpObj;

const ExpObj ExpObj_NUL = { 0,0,0,  -1,0,0,0};


// typ   cad-typ; eh Typ_LN
// oNr   nr of subObjs; for CCV, SUR 
// ent   iges-entity; eg IG_EntLN = 110
// plNr  P-block-lineNr
// trNr  transformation
// ipa   aux-data; surfaces: [0]=supportSurface;
// hid   hidden obj; 0=visible; 1=hidden
// clo   curves - closed; 0=yes, 1=not_closed; -1=undefined;
typedef struct {void *obj; long dbi; int typ, oNr, ent, plNr, trNr, form, ipa[2];
                char hid, clo; }                                      cadObj__;

const cadObj__ cadObj_NUL =
               {     NULL,      0L,      0,   0,    0,   0,    0,    0,   {0,0},
                     0, -1 };


typedef struct {int modNr; int modEnt;}                               ModPos;


static FILE   *fp_o1, *fp_o2;

#define SIZ_expTab 100000
static ExpObj *expTab = NULL; // list of already exported entities
static int    expTabSiz, expNr;//, expInd;

static int    IG_modNr;
static ModPos *IG_modTab = NULL;


static int    IG_Da1[9];
static int    IG_Da2[9];

#define SIZ_lnP 16000
static char   IGlnP[SIZ_lnP];
static int    len_lnP;

static MemTab(int) iaEnt = _MEMTAB_NUL;

// get inde into D-block of last created expTab-record
int IGE_w_getLastPtr;
#define IGE_w_getLastPtr  expNr * 2 - 1



//================================================================
  int IGE_w__ (char* fnam) {
//================================================================
// IGE_w__                   mainEntry export iges-file
//   sequence of submodels must be ordered (definition before use)
// Output:
//   retCode   <0 is Error; >0 is minor errors;


  int      i1, ipos, modNr, iMod, exp_errNr;
  FILE     *fpi;
  char     cbuf[256], safPrim[SIZMFTot], smNam[SIZMFTot], s1[320], *p1;



  printf("======================================================== \n");
  printf("IGE_w__ |%s|\n",fnam);

    // TESTBLOCK
    // LOG_A_test ();
    // return 0;
    // END TESTBLOCK


  LOG_A_init ("export_ige");          // init logFile

//   // store active DB
//   DB_save__ ("");

  if ((fp_o1 = fopen (fnam, "w+")) == NULL) {
    TX_Error ("Open file %s",fnam);
    return -1;
  }


  // einen Tempfilename für die Hilfsdatei
  // sprintf(cbuf,"%s\\tmp\\IGES.tmp",os_get_bas_dir ());
  sprintf(cbuf,"%sIGES.tmp",AP_get_tmp_dir ());
  if ((fp_o2 = fopen (cbuf, "w+")) == NULL) {
    TX_Error ("Open file %s",cbuf);
    fclose(fp_o1);
    return -2;
  }
    // printf("cbuf=|%s|\n",cbuf);


  IGE_w_init ();

  IGE_w_obj_init ();

  MemTab_ini__ (&iaEnt, sizeof(int), Typ_Int4, 4000);

  IGE_w_eTab_clr ();

  IGE_w_124 (UT3D_MAT_4x3); // basePlane; iEnt=1;

  //==================================================================

  // get list of submodels in loadersequence in file ../tmp/Mod.lst
  modNr = MDL_ldm_f ();
  if(modNr < 0) goto L_exit;    // err
  if(modNr < 1) goto L_main;    // go if no subModels exist

  // get safe name of primary model
  p1 = MDL_mNam_prim ();
  strcpy(safPrim, p1);
  UTX_safeName (safPrim, 1);

  // get spc for ModPos
  IG_modTab = malloc (modNr * sizeof(ModPos));

  // save primary model
  DB_save__  (safPrim);

//   DL_Init ();     // kill / Init DL

  // open list of models
  sprintf(cbuf,"%sMod.lst",AP_get_tmp_dir());
  if((fpi=fopen(cbuf,"r")) == NULL) {
    TX_Print("AP_ExportIges__ E002 %s",cbuf);
    return -3;
  }

  sprintf(cbuf,"%sModel_",AP_get_tmp_dir());
  ipos = strlen(cbuf);

  while (!feof (fpi)) {
    if (fgets (smNam, SIZMFTot, fpi) == NULL) break;
    UTX_CleanCR (smNam);
    UTX_safeName (smNam, 1);
      printf(" _ExportIges__-nxt-model |%s|\n",smNam);
    // fprintf(fp_o1,"nxt model |%s|\n",cbuf);

    // load subModel 
    DB_load__ (smNam);

    // work submodel
    iMod = MDL_imb_mNam(smNam, 1);  // get ModelNr from Modelname
    if(iMod < 0) {
      TX_Error("AP_ExportIges__: E004 %s %d",smNam,iMod);
      goto L_err;
    }

    // export objs of model
    i1 = IGE_w_model (iMod,smNam);

    // add model to IG_modTab
    IG_modTab[IG_modNr].modNr  = iMod;
    IG_modTab[IG_modNr].modEnt = i1;
    ++IG_modNr;

    IGE_w_eTab_clr ();

  }
  fclose(fpi);


  // restore primary model
  DB_load__ (safPrim);



  //----------------------------------------------------------------
  // work mainModel
  L_main:
    printf(" _ExportIges__-main..\n");

  // DL_disp_def (ON); // ab nun wieder alles anzeigen

  // sprintf(cbuf,"%stmp%cModel_",AP_get_bas_dir(),fnam_del);
  // printf("nxt model |%s|\n",cbuf);
  // fprintf(fp_o1,"nxt model main\n",cbuf);

  // DB_Init (1);
  // GR_Init1 (); // DispList loeschen
  // ED_Init ();
  // if(ED_work_file (cbuf) < 0) goto L_err3;
  // ED_lnr_reset ();
  // ED_Run ();

  // ED_Reset ();  // ED_lnr_act = 0; 2004-02

  // ED_work_END (0);


  // work mainmodel
  i1 = IGE_w_model (-1, "-main-");



  //==================================================================
  // join files, write T-line
  IGE_w_rT ();


  // remove auxFile
  L_exit:
  fclose(fp_o1);
  fclose(fp_o2);
  sprintf(cbuf,"%sIGES.tmp",AP_get_tmp_dir ());
  OS_file_delete (cbuf);


  //sprintf(auxFilNam,"notepad %s",fnam);
  //system(auxFilNam);


  // FERTIG:
  // DL_disp_def (ON); // ab nun wieder alles anzeigen


  // Statistik ausgeben
  TX_Print ("IGES-Export into file %s",fnam);
  TX_Print(" - logfile %sexport_ige.log",AP_get_tmp_dir());
  sprintf (s1,"objects exported:  %d  of  %d",oExpNr,oTotNr);
  TX_Print (s1);
  LOG_A__ (ERR_INF, s1);


  //----------------------------------------------------------------
  // free, exit 
  L_fini:

    if(expTab) free (expTab);

    MemTab_free (&iaEnt);

    LOG_A_exit (&exp_errNr);           // close logfile

    if(IG_modTab) free (IG_modTab);

    return exp_errNr;


  //================================================================
  L_err:
  fclose(fpi);
  fclose(fp_o1);
  fclose(fp_o2);
  goto L_fini;

}


//=============================================================================
  int IGE_w_model (int iMod, char *mdlNam) {
//=============================================================================
// IGE_w_model               export all objs in dispList
// Input:
//   iMod              -1=main;
// -  single obj out
// Output:
//   oTotNr   (global)  nr of processed cad-objs
//   oExpNr   (global)  nr of exported cad-objs


  int         irc, iEntID;     
  long        dlNr, l1, l2;
  DL_Att      *dlTab, *dla;
  cadObj__    cadObj1;


  // printf("IGE_w_model %d\n",iMod);

  dlNr = DL_get__ (&dlTab);  // get DL


  for(l1=0; l1<dlNr; ++l1) {
      // DL_DumpObj__ (l1, "IGE_w_model-dlo");

    dla = &dlTab[l1];

    if(dla->modInd != iMod) continue;    // skip if not in active model
    if(dla->dim    == 0) continue;       // skip dimmed objects
    if(dla->unvis  == 1) continue;       // skip obj with no graph. representation

    ++oTotNr;

    cadObj1     = cadObj_NUL;
    cadObj1.typ = dla->typ;
    cadObj1.dbi = dla->ind;
    cadObj1.hid = DL_IS_HIDDEN(dlTab[l1]);      // 0=visible; 1=hidden;
      // printf(" cadObj1.hid=%d\n",cadObj1.hid);

    // tra = dla->irs;


    // export CAD-obj
    iEntID = IGE_w_obj__ (&cadObj1);
    if(iEntID < 0) goto NextRec;

    // add entID to table iaEnt
    MemTab_add (&iaEnt, &l2, &iEntID, 1, 0);

    ++oExpNr;

    NextRec:;
  }

  //============================================================================
  // export iaEnt -> SubfigDef 308 - not for main
  if(strcmp(mdlNam,"-main-")) irc = IGE_w_MD (mdlNam);
  else                        irc = -1;

    // printf(" ex-IGE_w_model %d\n",irc);

  return irc;

}


//================================================================
  int IGE_w_init () {
//================================================================
// IGE_w_init     write H-block

  int i1;


  // printf("IGE_w_init \n");


  /* --------------------------------------------------------------------- */
  /* die ersten 68 Char's */
  fprintf(fp_o1,"%s",
   "START RECORD GO HERE                                                ");
  /*           1234567890123456789012   12345678    1234567890123456789    */
  /*1234567890123456789012345678901234567890123456789012345678901234567890 */
  /*         1         2         3         4         5         6           */

   /* die fehlenden 12 */
  fprintf(fp_o1,"%s",
   "    S      1\n");
  /*1234567890123456789012345678901234567890123456789012345678901234567890 */
  /*         1         2         3         4         5         6           */

/*
,,,8Hext1.IGS,24HCAD-CAM Services Reiter ,3H1.0,                        G 1
32,38,16,38,16,,1.,2,                                                   G 2
2HMM,450,4.5,                                                           G 3
15H20000329.171500,0.001,500.,,,11,0,                                   G 4
15H20000329.171500,;                                                    G 5
*/

  /* die ersten 72 Char's */
  /*   4=Filename, 5=SystemID, 6=InterfaceVersion               */
  fprintf(fp_o1,"%s",
   ",,,8HFILE.IGS,23Hgcad3d   www.gcad3d.org,3H1.0,                         ");
  /*123456789012345678901234567890123456789012345678901234567890123456789012 */
  /*         1         2         3         4         5         6         7 */
  /* die fehlenden 8 */
  fprintf(fp_o1,"%s",
   "G      1\n");
  /*1234567890123456789012345678901234567890123456789012345678901234567890 */
  /*         1         2         3         4         5         6           */


  /* die ersten 72 Char's */
  /* 7=BitsOfInt,  ...      13=ModelSpaceScale 14=UnitFlag(2=mm) */
  fprintf(fp_o1,"%s",
   "32,38,16,38,16,,1.,2,                                                   ");
  /*123456789012345678901234567890123456789012345678901234567890123456789012 */
  /*         1         2         3         4         5         6         7 */
  /* die fehlenden 8 */
  fprintf(fp_o1,"%s",
   "G      2\n");
  /*1234567890123456789012345678901234567890123456789012345678901234567890 */
  /*         1         2         3         4         5         6           */



  /* die ersten 72 Char's */
  /* 15=Units, 16=Max.LineWeight, 17=Max.LineWidth */
  fprintf(fp_o1,"%s",
   "2HMM,450,4.5,                                                           ");
  /*123456789012345678901234567890123456789012345678901234567890123456789012 */
  /*         1         2         3         4         5         6         7 */
  /* die fehlenden 8 */
  fprintf(fp_o1,"%s",
   "G      3\n");
  /*1234567890123456789012345678901234567890123456789012345678901234567890 */
  /*         1         2         3         4         5         6           */


  /* die ersten 72 Char's */
  /* 18=GenerationDate, 19=Min.Resol, 20=Max.CoordVal, 23=IgesVersionNumber */
  fprintf(fp_o1,"%s",
   "15H20000329.171500,0.001,500.,,,6,0,                                    ");
  /*123456789012345678901234567890123456789012345678901234567890123456789012 */
  /*         1         2         3         4         5         6         7 */
  /* die fehlenden 8 */
  fprintf(fp_o1,"%s",
   "G      4\n");
  /*1234567890123456789012345678901234567890123456789012345678901234567890 */
  /*         1         2         3         4         5         6           */




  /* die ersten 72 Char's */
  /* 25=Date?? */
  fprintf(fp_o1,"%s",
   "15H20000329.171500,;                                                    ");
  /*123456789012345678901234567890123456789012345678901234567890123456789012 */
  /*         1         2         3         4         5         6         7 */
  /* die fehlenden 8 */
  fprintf(fp_o1,"%s",
   "G      5\n");
  /*1234567890123456789012345678901234567890123456789012345678901234567890 */
  /*         1         2         3         4         5         6           */


  //-----------------------------------------------------------------------
  IG_S_line_nr = 1;
  IG_G_line_nr = 5;

  IG_D_line_nr = 0;
  IG_P_line_nr = 0;

  // IG_D_line_mod = 0; // end of last submodel


//   for (i1=0; i1 < 9; ++i1) {
//     IG_Pa1[i1]=0;
//     IG_Pa2[i1]=0;
//   }

  expTab = (void*)malloc(SIZ_expTab);
  expTabSiz = SIZ_expTab / sizeof(ExpObj);
  expNr  = 0;
    // printf(" expTabSiz=%d\n",expTabSiz);

  IG_modNr = 0;
  // IG_TrInd = 0;

  return 1;

}


//================================================================
  int IGE_w_rT () {
//================================================================
// join files, write T-Block


  int   i1;
  char  s1[88];

  // printf("IGE_w_rT.................\n");


  /* ---------------------------------------------------------- */
  // nun Datei 2 hinter Datei 1 anfuegen
  rewind (fp_o2);
  while(fgets (s1, 84, fp_o2) != NULL) {
    fprintf(fp_o1, "%s",s1);
  }


  //----------------------------------------------------------------------
   /* Schlusszeile */
  strcpy(&s1[0],"S");
  sprintf(&s1[1],"%7d",IG_S_line_nr);

  strcpy(&s1[8],"G");
  sprintf(&s1[9],"%7d",IG_G_line_nr);

  strcpy(&s1[16],"D");
  sprintf(&s1[17],"%7d",IG_D_line_nr);

  strcpy(&s1[24],"P");
  sprintf(&s1[25],"%7d",IG_P_line_nr);

   /* Von 32 bis 72 blanks */
  for (i1=32; i1<72; i1++)   { strcpy(&s1[i1]," "); }

  strcpy(&s1[72],"T");

   /* Von 74-80 die Anz. der Zeilen (1) */
  sprintf(&s1[73],"%7d",1);

  fprintf(fp_o1,"%s\n",s1);

  return 1;

}


//================================================================
  // int IGE_err (int irc,
//================================================================

 
//================================================================
  int IGE_w_ogx__ (ObjGX *ox1) {
//================================================================
 
  int       irc;
  cadObj__  cadObj2 = cadObj_NUL;


   // DEB_dump_ox_s_ (ox1, "\n============= IGE_w_SRC-cv");

  if(ox1->form == Typ_Index) {
    OGX_GET_INDEX (&cadObj2.typ, &cadObj2.dbi, ox1);

    // export curve
    irc = IGE_w_obj__ (&cadObj2);
    if(irc < 0) {irc = -1; goto L_err;}

  } else { irc = -2; goto L_err;}


  return irc;

  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_ogx__ E%d %d %ld",irc,cadObj2.typ, cadObj2.dbi);
    return -1;

}


//================================================================
  int IGE_w_obj__ (cadObj__ *cadObj1) {
//================================================================
// IGE_w_obj__             export CAD-obj
// Input:
//   cadObj1->typ    cadTyp; eg typ_LN
//   cadObj1->dbi    DB-index;


  int      irc, cadTyp, igInd;
  long     dbi;
  ObjGX    ox1;


  // printf("==================IGE_w_obj__============================= \n");
  // IGE_w_dump_cadObj(cadObj1,"IGE_w_obj__-in");

  if(!cadObj1->dbi) {
    if(cadObj1->obj) goto L_exp;
    else { irc = -1; goto L_err; }
  }

  // get CAD-obj
  ox1 = DB_GetObjGX (cadObj1->typ, cadObj1->dbi);
  if(ox1.typ == Typ_Error) { irc = -2; goto L_err; }
    // DEB_dump_obj__ (Typ_ObjGX, &ox1, "IGE_w_obj__-ox1-1");


  // resolv form=Index
  if(ox1.form == Typ_Index) {
    // get typ and dbi
    OGX_GET_INDEX (&cadTyp, &dbi, &ox1);
      // printf(" IGE_w_obj__-indexed typ=%d dbi=%ld\n",cadTyp,cadObj1->dbi);


    // get ox from indexed obj
    ox1 = DB_GetObjGX (cadTyp, dbi);
    if(ox1.typ == Typ_Error) { irc = -3; goto L_err;}
      // DEB_dump_obj__ (Typ_ObjGX, &ox1, "IGE_w_obj__-ox1-2");
     
    cadObj1->typ = cadTyp;
    cadObj1->dbi = dbi;

  }


    // TESTBLOCK
    // DEB_dump_obj__ (ox1.form, ox1.data, "IGE_w_obj__-ox1-3");
    // printf("IGE_w_obj__-ox-typ=%d form=%d siz=%d\n",ox1.typ,ox1.form,ox1.siz);
    // END TESTBLOCK
 
  cadObj1->typ = ox1.typ;
  cadObj1->oNr = ox1.siz;   // nr of subObjs - only for CCV, SUR
  cadObj1->obj = ox1.data;


    // TESTBLOCK
    // DEB_dump_ox_s_ (&ox1, "IGE_w_obj__-ox1");
    // END TESTBLOCK

  // create and export obj
  L_exp:
    return IGE_w_obj_dist (cadObj1);


  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_obj__ E%d %d %ld",irc,cadObj1->typ, cadObj1->dbi);
    return -1;
 
}


//================================================================
  int IGE_w_obj_dist (cadObj__ *cadObj1) {
//================================================================
// create entity-data and export
// cadObj1->obj has binary obj of type cadObj1->typ
// Input:
//   cadObj1->typ    cadTyp; eg typ_LN
//   cadObj1->dbi    DB-index;
//   cadObj1->obj    must have type cadObj1->typ


  int   irc, cadTyp, iEnt;


  // printf("\n\n–––––––––––––––––––––––––––––––––––––––––––––––––––––– \n");
  // IGE_w_dump_cadObj (cadObj1, "IGE_w_obj_dist-in");

  cadTyp = cadObj1->typ;

  if     (cadTyp == Typ_PT)     irc = IGE_w_PT   (cadObj1);
  // curves
  else if(cadTyp == Typ_LN)     irc = IGE_w_LN   (cadObj1);
  else if(cadTyp == Typ_CI)     irc = IGE_w_CI   (cadObj1);
  else if(cadTyp == Typ_CVPOL)  irc = IGE_w_PLG  (cadObj1);
  else if(cadTyp == Typ_CVELL)  irc = IGE_w_ELL  (cadObj1);
  else if(cadTyp == Typ_CVBSP)  irc = IGE_w_BSPL (cadObj1);
  else if(cadTyp == Typ_CVPSP3) irc = IGE_w_PSPL (cadObj1);
  else if(cadTyp == Typ_CVCLOT) irc = IGE_w_CLOT (cadObj1);
  else if(cadTyp == Typ_CVTRM)  irc = IGE_w_CCV  (cadObj1);
  // support-surfaces
  else if(cadTyp == Typ_PLN)    irc = IGE_w_PLN  (cadObj1);
  else if(cadTyp == Typ_SURRU)  irc = IGE_w_SRU  (cadObj1);
  else if(cadTyp == Typ_SURRV)  irc = IGE_w_SRV  (cadObj1);
  else if(cadTyp == Typ_SURBSP) irc = IGE_w_SBS  (cadObj1);
  else if(cadTyp == Typ_SURCIR) irc = IGE_w_SCI  (cadObj1);
  // trimmed-surf
  else if(cadTyp == Typ_SUTP)   irc = IGE_w_SUTP (cadObj1);
  // solids
  else if(cadTyp == Typ_PRI)    irc = IGE_w_PRI  (cadObj1);
  // notes
  else if(cadTyp == Typ_ATXT)   irc = IGE_w_NTE  (cadObj1);
  else if(cadTyp == Typ_GTXT)   irc = IGE_w_NTE  (cadObj1);
  // subModels
  else if(cadTyp == Typ_Model)  irc = IGE_w_MR   (cadObj1);

// TODO:
// Typ_CVELL

  else {irc = -19; goto L_err;} // cad-type unsupported

  if(irc < 0) return irc;
  if(irc > 0) { iEnt = irc; goto L_exit; }  // already exported


  // write D- and P-lines
  iEnt = IGE_w_obj_out__ (cadObj1);

  L_exit:
      // printf("ex-IGE_w_obj_dist %d\n",iEnt);
    return iEnt;



  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_obj_dist E%d %d %ld",irc,cadObj1->typ, cadObj1->dbi);
    return -1;
 
}


//===================================================================
  int IGE_w_PT (cadObj__ *cadObj1) {
//===================================================================

  int    iEnt, iTbe;
  Point  *pt1;

  // printf("IGE_w_PT\n");

  // test if obj already defined
  iEnt = IGE_w_eTab_find (&iTbe, cadObj1->typ, cadObj1->dbi);
  if(iEnt > 0) return iEnt;


  pt1 = (Point*)cadObj1->obj;
    // DEB_dump_obj__ (Typ_PT, pt1, "IGE_w_P_PT");

  cadObj1->ent = IG_EntPT;

  // add Point-Coords
  IGE_w_P_pt1 (pt1);

  // add (3.) int - 0
  IGE_w_P_i1 (0);

  return 0;

}


//===================================================================
  int IGE_w_LN (cadObj__ *cadObj1) {
//===================================================================

  int    iEnt, iTbe;
  Line   *ln1;

  // printf("IGE_w_LN\n");

  // test if obj already defined
  iEnt = IGE_w_eTab_find (&iTbe, cadObj1->typ, cadObj1->dbi);
  if(iEnt > 0) return iEnt;


  ln1 = (Line*)cadObj1->obj;
    // DEB_dump_obj__ (Typ_LN, ln1, "IGE_w_LN");

  cadObj1->ent = IG_EntLN;

  IGE_w_P_pt1 (&ln1->p1);
  IGE_w_P_pt1 (&ln1->p2);

  return 0;

}


//===================================================================
  int IGE_w_CI (cadObj__ *cadObj1) {
//===================================================================
// IGE_w_CI                    export circle
//  100, mz, mx,my, ax,ay, ex,ey;
// was IGE_w_CI IGE_w_mat

  int         irc, iEnt, trNr, iTbe, iClo;
  char        s1[256];
  double      d0 = 0.;
  Circ        *ci1;
  Circ2       ci2;
  Mat_4x3     m1;


  // IGE_w_dump_cadObj (cadObj1, "IGE_w_CI");

  // test if obj already defined
  iEnt = IGE_w_eTab_find (&iTbe, cadObj1->typ, cadObj1->dbi);
  if(iEnt > 0) {
    // already stored; OK if not to be closed
    if(cadObj1->clo != 0) goto L_exit;
    // OK if closed
    if(expTab[iTbe].clo == 0) goto L_exit;
  }

  ci1 = (Circ*)cadObj1->obj;
    // DEB_dump_obj__ (Typ_CI, ci1, "IGE_w_CI-ci1");

  // test if plg is closed; 
  iClo = UT3D_ck_ci360 (ci1); // 1=no; 0=yes=360Deg;

  // check if curve must be closed (BND)
  if(cadObj1->clo == 0) {
    // curve must be closed !
    if(iClo) {
      // relimited circle not valid for boundary; create CC ..
      sprintf(s1, "relimited circle C%ld not valid for boundary; create CCV ..",
              cadObj1->dbi);
      LOG_A__ (ERR_ERR,"***** IGE_w_CI %s",s1);
      return -1;
    }
  }

  // set closed for expTab
  cadObj1->clo = iClo;



  // check if circ is 2D-circ; is in baseplane (pc.z=0; vz=0,0,1)
  if(UT3D_ci_ck_2D(ci1)) {
    // yes; 2D-circle;
    UT2D_ci_ci3 (&ci2, ci1);
    trNr = 1;   // basePlane

  } else {
    // is NOT 2D-Circ; get trMat, 2D-start- and endPoint
    UT2D_ciTra_ci3 (m1, &ci2, ci1);
      // DEB_dump_obj__ (Typ_CI2, &ci2, "ex-UT2D_ciTra ci3");

    // write TrMat
    trNr = IGE_w_124 (m1);
  }

    // DEB_dump_obj__ (Typ_CI2, &ci2, "IGE_w_CI-ci2");
    // printf(" IGE_w_CI-trNr=%d\n",trNr);



  // circle not 360-deg and clockwise: swap start/endpoint
  if(iClo == 1) {
    // relimited arc; CW -> CCW
    if(ci1->ango < 0.) {
      MEM_swap__ (&ci2.p1, &ci2.p2, sizeof(Point2));
    }
    cadObj1->clo = 1; // not closed
  } else {
    cadObj1->clo = 0; // closed
  }


  //----------------------------------------------------------------
  cadObj1->ent = IG_EntCI;
  cadObj1->trNr = trNr;

 
  // 100, mz, mx,my,  ax,ay, ex,ey, 0,0;
  IGE_w_P_db1 (&d0);

  IGE_w_P_db1 (&ci2.pc.x);
  IGE_w_P_db1 (&ci2.pc.y);

  IGE_w_P_db1 (&ci2.p1.x);
  IGE_w_P_db1 (&ci2.p1.y);

  IGE_w_P_db1 (&ci2.p2.x);
  IGE_w_P_db1 (&ci2.p2.y);

  iEnt = 0;  // not yet written out 

  L_exit:
      // printf(" ex-IGE_w_CI %d\n",iEnt);

    return iEnt;

}


//===================================================================
  int IGE_w_ELL (cadObj__ *cadObj1) {
//===================================================================
// ellipse Typ_CVELL CurvElli > IG_EntELL 104
// 104 Conic Arc; ellipse|hyperbola|parabola
// majorAxis must be parallel world-x-axis, minorAxis parallel world-y-axis;
// 104, a,b,c,d,e,f, pc.z, p1.x,p1.y,  p2.x,p2.y;
// D2/4 (33-40)=Form: 1=ellipse; 2=hyperbola; 3=parabola;


  int          iEnt, iTbe;
  double       d0 = 0.;
  CurvElli     *el1;
  CurvEll2C    el2c;      // CurvEll2C
  Mat_4x3      m1;
  polcoeff_d5  ec;


  // printf("IGE_w_ELL\n");

  // test if obj already defined
  iEnt = IGE_w_eTab_find (&iTbe, cadObj1->typ, cadObj1->dbi);
  if(iEnt > 0) return iEnt;

  // get curve
  el1 = (CurvElli*) cadObj1->obj;
    // DEB_dump_obj__ (Typ_CVELL, el1, "IGE_w_ELL");

  cadObj1->ent = IG_EntELL;

  // get trMat
  UT2D_elTra_el3 (m1, &el2c, el1);

  // clockwise: swap start/endpoint
  if(el2c.srot) {
    MEM_swap__ (&el2c.p1, &el2c.p2, sizeof(Point2));
  }

  cadObj1->trNr = IGE_w_124 (m1);


  // get coeffs a-f
  UT3D_elcoe_el (&ec, el1);


  IGE_w_P_db1 (&ec.a);                // coeff
  IGE_w_P_db1 (&ec.b);                // coeff
  IGE_w_P_db1 (&ec.c);                // coeff
  IGE_w_P_db1 (&ec.d);                // coeff
  IGE_w_P_db1 (&ec.e);                // coeff
  IGE_w_P_db1 (&ec.f);                // coeff
  IGE_w_P_db1 (&d0);                  // Z-Val - transformed to 0.;
  IGE_w_P_db1 (&el2c.p1.x);           // pa
  IGE_w_P_db1 (&el2c.p1.y);           // pa
  IGE_w_P_db1 (&el2c.p2.x);           // pe
  IGE_w_P_db1 (&el2c.p2.y);           // pe

  return 0;

}


//================================================================
  int UT3D_pta_ck_clo (int ptNr, Point *pta) {
//================================================================
// UT3D_pta_ck_clo           check if polygon is closed
// returns    0=closed-curve, 1=open-curve


  if(UT3D_comp2pt (&pta[0], &pta[ptNr-1], UT_TOL_cv)) return 0;
  return 1;

}


//===================================================================
  int IGE_w_PLG (cadObj__ *cadObj1) {
//===================================================================
// IGE_w_PLG                polygon Typ_CVPOL CurvPoly > Ent 106/12
// 106/12 =  CopiousData/Form-3D-Polygon
// 106,2,<ptNr>,x,y,z[,x,y,z..];

  int        irc, ptNr, iEnt, iClo, iTbe;
  Point      *pta;
  CurvPoly   *plg1;


  // printf("IGE_w_PLG typ=%d dbi=%ld clo=%d\n",cadObj1->typ,cadObj1->dbi,cadObj1->clo);
  // IGE_w_dump_eTab ("w_PLG-in");


  // test if obj already defined
  iEnt = IGE_w_eTab_find (&iTbe, cadObj1->typ, cadObj1->dbi);
    // printf(" w_PLG-iEnt=%d iTbe=%d clo=%d\n",iEnt,iTbe,expTab[iTbe].clo);

  if(iEnt > 0) {
    // already stored; OK if not to be closed
    if(cadObj1->clo != 0) goto L_exit;
    // OK if closed
    if(expTab[iTbe].clo == 0) goto L_exit;
  }

  plg1 = (CurvPoly*) cadObj1->obj;
    // DEB_dump_obj__ (Typ_CVPOL, plg1, "IGE_w_PLG");

  cadObj1->ent = IG_EntPOL;

  // PolygonCurve -> relimited Polygon
  pta  = (Point*)memspc101;
  ptNr = sizeof(memspc101) / sizeof(Point);
  irc = UT3D_pta_plg (&ptNr, pta, plg1);
  if(irc < 0) goto L_err;

  // test if plg is closed; set cadObj1->clo
  iClo = UT3D_pta_ck_clo (ptNr, pta);
    // printf(" w_PLG-iClo=%d\n",iClo);

  // test if plg is to be closed; ck cadObj1->clo
  if(cadObj1->clo == 0) {
    // curve must be closed !
    if(iClo) {
      // add point ..
      pta[ptNr] = pta[0];
      ++ptNr;
    }
    cadObj1->dbi = 0; // anonymize obj;
  }
  // set closed for expTab
  cadObj1->clo = iClo;


  IGE_w_106 (ptNr, pta);
  cadObj1->form = 2;       // data-triples


  L_exit:
    return 0;


  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_PLG E%d %d %ld",irc,cadObj1->typ, cadObj1->dbi);
    return -1;

}


//===================================================================
  int IGE_w_CLOT (cadObj__ *cadObj1) {
//===================================================================
// clotoid Typ_CVCLOT CurvPoly > Ent 106/12 CopiousData/Form-3D-Polygon
// 106,2,<ptNr>,x,y,z[,x,y,z..];

  int        irc, ptNr, iEnt, iTbe;
  Point      *pta;
  CurvClot   *cv1;


  // printf("IGE_w_CLOT  %d %ld\n",cadObj1->typ, cadObj1->dbi);

  // test if obj already defined
  iEnt = IGE_w_eTab_find (&iTbe, cadObj1->typ, cadObj1->dbi);
  if(iEnt > 0) return iEnt;

  cv1 = (CurvClot*) cadObj1->obj;
    // DEB_dump_obj__ (Typ_CVCLOT, cv1, "IGE_w_CLOT");

  cadObj1->ent = IG_EntPOL;

  // PolygonCurve -> relimited Polygon
  pta  = (Point*)memspc101;
  ptNr = sizeof(memspc101) / sizeof(Point);
  irc = UT3D_npt_clot (pta, &ptNr, cv1, UT_DISP_cv);
  if(irc < 0) goto L_err;

  IGE_w_106 (ptNr, pta);
  cadObj1->form = 2;       // data-triples

  return 0;


  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_CLOT E%d %d %ld",irc,cadObj1->typ, cadObj1->dbi);
    return -1;

}


//================================================================
  int IGE_w_106 (int ptNr, Point *pta) {
//================================================================
// 106 Copious Data
// D2/4 Form must be 2
 

  int     i1;

  IGE_w_P_i1 (2);       // 1=2D-points, 2=3D-Points (3 doubles per data)
  IGE_w_P_i1 (ptNr);    // Nr of points

  // add points
  for(i1=0; i1<ptNr; ++i1) IGE_w_P_pt1 (&pta[i1]);


  return 0;

}


//===================================================================
  int IGE_w_P_POL2 (cadObj__ *cadObj1) {
//===================================================================
// 2D-polygon
/*
  int        ipt, ipol, ptNr;
  // Point2Tab  *cv1;
  Point2     *ptTab;


  printf("IGE_w_P_POL *** form=%d\n",IG_Pa2[4]);


  // cv1   = el->data;
  ipol  = el->siz;
  ptTab = el->data;


  ipt  = 0;



  IGE_w_P_i1 (1);       // 1=2D-points, 2=3D-Points
  IGE_w_P_i1 (ipol);    // Nr of points
  IGE_w_P_db1 (0.);       // Z-Val (nur f. 2D-points)


  L_next:
  IGE_w_P_db1 (ptTab[ipt].x);
  IGE_w_P_db1 (ptTab[ipt].y);
  ++ipt;
  if(ipt < ipol) goto L_next;


  L_fertig:
*/
  return 0;

}


//===================================================================
  int IGE_w_BSPL (cadObj__ *cadObj1) {
//===================================================================
// B-spline Typ_CVBSP CurvBSpl > ?
// D2/4 (33-40)=form 0-5; 0=unspec; 1=line, 20circ ..
// P-Block:
// 126,<ptNr-1>,<deg>,<props>,<knots>,<weigths>,<points>,<V0>,<V1>,<nVC>
// prop1: 0 = nonplanar, 1 = planar
// prop2: 0 = open curve, 1 = closed curve
// prop3: 0 = rational, 1 = polynomial
// prop4: 0 = nonperiodic, 1 = periodic
//
// TODO: set planar,closed,periodic ?


  int      i1, i2, iEnt, iTbe;
  double   d1;
  CurvBSpl *cv1;


  // printf("IGE_w_BSPL %d %ld\n",cadObj1->typ, cadObj1->dbi);

  // test if obj already defined
  iEnt = IGE_w_eTab_find (&iTbe, cadObj1->typ, cadObj1->dbi);
  if(iEnt > 0) return iEnt;

  cv1 = (CurvBSpl*) cadObj1->obj;
    // DEB_dump_obj__ (Typ_CVBSP, cv1, "IGE_w_PLG");

  cadObj1->ent = IG_EntBS;


  IGE_w_P_i1 (cv1->ptNr - 1); // I Anzahl points
  IGE_w_P_i1 (cv1->deg);      // I Degree
  IGE_w_P_i1 (0);     // I nonplanar
  IGE_w_P_i1 (0);     // I open curve
  IGE_w_P_i1 (1);     // I polynomial
  IGE_w_P_i1 (0);     // I non-periodic

  // Knotvektor
  i2 = cv1->ptNr + cv1->deg + 1;
  for(i1=0; i1<i2; ++i1) {
    IGE_w_P_db1 (&cv1->kvTab[i1]);     // I non-periodic
  }

  // weigths
  i2 = cv1->ptNr;
  for(i1=0; i1<i2; ++i1) IGE_w_P_i1 (1);

  // controlpoints
  i2 = cv1->ptNr;
  for(i1=0; i1<i2; ++i1) IGE_w_P_pt1 (&cv1->cpTab[i1]);

  // start/end-u-val
  IGE_w_P_db1 (&cv1->v0);

  // d1 = cv1->kvTab[cv1->ptNr + cv1->deg];
  IGE_w_P_db1 (&cv1->v1);

//   // normalvektor (if planar)
//   d1 = 0.;
//   IGE_w_P_db1 (&d1);
//   IGE_w_P_db1 (&d1);
//   IGE_w_P_db1 (&d1);

  return 0;

}


//===================================================================
  int IGE_w_PSPL (ObjGX *el) {
//===================================================================
// CurvPsp3 Param.Spline
// was IGE_w_P_PS
/*
  int         i1, iNr;
  polynom_d3  *polTab;
  // CurvPol3    *cvpsp3;


  // printf("IGE_w_P_PS\n");


  IGE_w_P_i1 (3); // Sourcetyp = 3
  IGE_w_P_i1 (1); // Continuity = 1
  IGE_w_P_i1 (3); // non-Planar = 3


  // cvpsp3 = el->data;
  polTab = el->data;

  // iNr = cvpsp3->polNr;
  iNr = el->siz;
  IGE_w_P_i1 (iNr-1); // Anz.Segments


  // Die Segmentoffsets raus
  for(i1=0; i1<iNr; ++i1) {
    // printf("segOff %d %f\n",i1,polTab[i1].u);
    IGE_w_P_db1 (polTab[i1].u);
  }


  for(i1=0; i1<iNr; ++i1) {
    // printf("seg %d\n",i1);
    // die X-Koeffizienten
    IGE_w_P_db1 (polTab[i1].x.a);
    IGE_w_P_db1 (polTab[i1].x.b);
    IGE_w_P_db1 (polTab[i1].x.c);
    IGE_w_P_db1 (polTab[i1].x.d);
    // die Y-Koeffizienten
    IGE_w_P_db1 (polTab[i1].y.a);
    IGE_w_P_db1 (polTab[i1].y.b);
    IGE_w_P_db1 (polTab[i1].y.c);
    IGE_w_P_db1 (polTab[i1].y.d);
    // die Z-Koeffizienten
    IGE_w_P_db1 (polTab[i1].z.a);
    IGE_w_P_db1 (polTab[i1].z.b);
    IGE_w_P_db1 (polTab[i1].z.c);
    IGE_w_P_db1 (polTab[i1].z.d);
  }
*/
  return 0;

}


//===================================================================
  int IGE_w_NTE (cadObj__ *cadObj1) {
//===================================================================
// IGE_w_NTE              alfaText, grafText
// was IGE_w_P_TX


//   int        i1;
//   double     d1, d2;

  int        irc, ils;
  double     dir, d1, d2;
  AText      *txa;
  GText      *txg;
  Point      *pos;
  char       *stxt;


  // printf("=============================================== \n");
  // IGE_w_dump_cadObj(cadObj1,"IGE_w_NTE-in");


  if(cadObj1->typ == Typ_ATXT) {
    // alfa-text always same size, always in viewplane; no rotAngle, scale;
    txa = (AText*) cadObj1->obj;
          // DEB_dump_obj__ (cadObj1->typ, txa, "IGE_w_NTE-txa");
    stxt = txa->txt;
    ils = strlen(stxt);
    if(ils < 1) {irc = -1; goto L_err;}
    pos = &txa->p1;
    dir = 0.;
    // d1=TextBoxWidth, d2=TextBoxHeight
    // geht nicht wirklich;
    // d1 = i1 * 3 / GL_get_Scale();
    // d2 = 3 / GL_get_Scale();
    d1 = ils * 3;
    d2 = 3;



  } else if(cadObj1->typ == Typ_GTXT) {
    // grafic-text; 
    txg = (GText*) cadObj1->obj;
          // DEB_dump_obj__ (cadObj1->typ, txg, "IGE_w_NTE-txg");
    stxt = txg->txt;
    ils = strlen(stxt);
    if(ils < 1) {irc = -2; goto L_err;}
    pos = &txg->pt;
    dir = UT_RADIANS(txg->dir);
    // d1=TextBoxWidth, d2=TextBoxHeight
    d1 = ils * (GL_vf1_CW + GL_vf1_CS);
    d2 = GL_vf1_CW + GL_vf1_CS;   // Höhe = Breite + space

    d1 *= txg->size;
    d2 *= txg->size;



  } else {irc = -3; goto L_err;}

    // printf(" d1=%f, d2=%f dir=%f\n",d1,d2,dir);


  IGE_w_P_i1 (1);                      // Nr of strings = 1
  IGE_w_P_i1 (ils);                    // Nr of chars
  IGE_w_P_db1 (&d1);                   // TextBoxBreite
  IGE_w_P_db1 (&d2);                   // TextBoxLaenge
  IGE_w_P_i1 (1);                      // FontCode = 1
  // IGE_w_P_db1 ((double*)&RAD_90);               // SlantAngle = Pi/2
  d1 = RAD_90;
  IGE_w_P_db1 (&d1);                // SlantAngle = Pi/2
  IGE_w_P_db1 (&dir);               // Angle
  IGE_w_P_i1 (0);                   // no Mirror = 0
  IGE_w_P_i1 (0);                   // horiz. = 0
  IGE_w_P_pt1 (&pos);               // Pos
  IGE_w_P_txt (stxt);               // Text


  cadObj1->ent = IG_EntTX;  // 212


  return 0;


  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_NTE E%d %d %ld",irc,cadObj1->typ, cadObj1->dbi);
    return -1;

}



//===================================================================
  int IGE_w_CCV (cadObj__ *cadObj1) {
//===================================================================
// Write 102 = composite-curve.
// iges does not have trimmed-curves; get standard-obj from trimmed-curve and export.
// 102,<nr-of-curves>,<pointers-to-curves>

  int       irc, i1, ii, cvNr, *subObjTab, iEnt, iTbe;
  ObjGX     ox1, *oxa;
  CurvCCV   *cvTab, *cv1;
  cadObj__  cadObj2;  // = cadObj_NUL;
  char      cvo[OBJ_SIZ_MAX];


  // printf("--------------------------------------- \n");
  // IGE_w_dump_cadObj (cadObj1, "IGE_w_CCV-in");

  // test if obj already defined
  iEnt = IGE_w_eTab_find (&iTbe, cadObj1->typ, cadObj1->dbi);
  if(iEnt > 0) return iEnt;

  // get spc for subObjTab
  subObjTab = (int*) MEM_alloc_tmp (cadObj1->oNr * sizeof(int));

  cvNr = cadObj1->oNr;
  cvTab = cadObj1->obj; 

  // loop tru all trimmed-curves
  for(i1=0; i1<cvNr; ++i1) {

    cadObj2 = cadObj_NUL;       // clear trMat !

    cv1 = &cvTab[i1];
      // DEB_dump_obj__ (Typ_CVTRM, cv1, "cv1[%d] ",i1);

    // get standard-obj from trimmed-curve
    irc = UTO_cv_cvtrm (&cadObj2.typ, cvo, NULL, cv1);
    if(irc < 0) { irc = -2; goto L_err;}

    cadObj2.obj = (void*)cvo;
      // DEB_dump_obj__ (cadObj2.typ, cvo, "cvo[%d]",i1);

    // export curve[i1]
    irc = IGE_w_obj_dist (&cadObj2);
    if(irc < 0) {irc = -3; goto L_err;}
    subObjTab[i1] = irc;
  }

  //----------------------------------------------------------------
  cadObj1->ent = IG_EntCCV;   // 102

  // out nr of members
  IGE_w_P_i1 (cvNr);

  // out members
  IGE_w_P_in (subObjTab, cvNr);


    // TESTBLOCK
    // IGE_w_dump_eTab ("ex-IGE_w_CCV");
    // END TESTBLOCK
 

  return 0;


  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_CCV E%d %d %ld",irc,cadObj1->typ, cadObj1->dbi);
    return -1;

}


//===================================================================
  int IGE_w_PLN (cadObj__ *cadObj1) {
//===================================================================
// Plane 108 IG_EntPLN
// 108, A, B, C, D, zero, <pos-symbol>,<size-symbol>;
// A * x + B * y + C * z = D

  int          i1, iEnt, iTbe;
  double       d1, d2;
  Plane        *pl1;
  polcoeff_d3  co3;


  // printf("IGE_w_PLN\n");

  // test if obj already defined
  iEnt = IGE_w_eTab_find (&iTbe, cadObj1->typ, cadObj1->dbi);
  if(iEnt > 0) return iEnt;

  pl1 = (Plane*)cadObj1->obj;

  cadObj1->ent = IG_EntPLN;


  UT3D_plcoe_pl (&co3, pl1);

  IGE_w_P_db1 (&co3.a);                // coeff
  IGE_w_P_db1 (&co3.b);                // coeff
  IGE_w_P_db1 (&co3.c);                // coeff
  IGE_w_P_db1 (&co3.d);                // coeff
  d1 = 0.;
  IGE_w_P_db1 (&d1);                  // Pointer to boundary
  IGE_w_P_pt1 (&pl1->po);             // posi of symbol
  d1 = 1.;
  IGE_w_P_db1 (&d1);                  // size of symbol

  return 0;

}


/* UU
//================================================================
  int IGE_w_SUR (cadObj__ *cadObj1) {
//================================================================
// Typ_SURRV  IGE_w_SRV    IG_EntSRV
// Typ_SURRU  IGE_w_SRU    IG_EntSRU
// Typ_SUTP   IGE_w_SUTP   IG_EntSTP
// Typ_SURBSP IGE_w_SBS    IG_EntSTP
//            IGE_w_SURBS      
//                         IG_EntSRC   contour + vector ?? 

  int       irc, i1, cvNr;
  ObjGX     ox1, *oxa;


  IGE_w_dump_cadObj (cadObj1, "IGE_w_SUR-in");

  // SUR is ObjGX with many objs;
//   ox1 = DB_GetObjGX (cadObj1->typ, cadObj1->dbi);
//   if(ox1.typ == Typ_Error) { irc = -1; goto L_err;}
//     printf("  SUR-typ=%d form=%d\n",ox1.typ,ox1.form);
//     DEB_dump_ox_s_ (&cadObj1->obj, "IGE_w_SUR");

  
  oxa = cadObj1->obj;
  for(i1=0; i1<cadObj1->oNr; ++i1) {
    DEB_dump_obj__ (Typ_ObjGX, &oxa[i1], "oxa[%d]",i1);
  }

  





  return 0;

  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_SUR E%d %d %ld \n",irc,cadObj1->typ, cadObj1->dbi);
    return -1;

}
*/


//===================================================================
  int IGE_w_SRV (cadObj__ *cadObj1) {
//===================================================================
// IGE_w_SRV             revolved-surface iges-entity 120
// 120,<centerLine>,<curve>,<startAngle>,<endAngle>;


  int       irc, i1, typ, eln, ecv;
  long      dbi;
  double    a1, a2;
  Line      lnc;
  Plane     *pl1;
  ObjGX     oxCen, oxCrv;
  SurRev    *srv;
  CurvCCV   tcv;
  char      cvo[OBJ_SIZ_MAX];
  cadObj__  cadObjLn = cadObj_NUL;
  cadObj__  cadObjCv = cadObj_NUL;


  srv = (SurRev*) cadObj1->obj;

  // printf("\n\nSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS\n");
  // DEB_dump_obj__(Typ_SURRV, srv, "IGE_w_SRV-in");

  // get CAD-obj centerline
  typ = srv->typCen;
  dbi = srv->indCen;
  oxCen = DB_GetObjGX (typ, dbi);
  if(oxCen.typ == Typ_Error) { irc = -1; goto L_err;}
    // DEB_dump_ox_s_ (&oxCen, "IGE_w_SRV-cen");


  //----------------------------------------------------------------
  // get line from centerline oxCen
  if(srv->typCen == Typ_PLN) {
    // get plane
    pl1 = (Plane*)oxCen.data;
    // line from origin & vector of plane
    UT3D_ln_ptvc (&lnc, &pl1->po, &pl1->vz);
      // DEB_dump_obj__ (Typ_LN, &lnc, "IGE_w_SRV-lnc");
    // export line
    cadObjLn.typ = Typ_LN;
    cadObjLn.obj = &lnc;
    // create entity from typ,data
    eln = IGE_w_obj_dist (&cadObjLn);
    if(eln < 0) {irc = -1; goto L_err;}

  } else { irc = -2; goto L_err;}


  //----------------------------------------------------------------
  // export curve

  // trim curve
  if((srv->v0 == 0.)&&(srv->v1 == 1.)) {
    // get / export curve
    cadObjCv.typ = srv->typCov;
    cadObjCv.dbi = srv->indCov;

  } else {
    // trim curve
    CVTRM__dbo (&tcv, srv->typCov, srv->indCov);
    tcv.v0 = srv->v0;
    tcv.v1 = srv->v1;
      // DEB_dump_obj__ (Typ_CVTRM, &tcv, "IGE_w_SRV-tcv");
    // make standard-curve from trimmed-curve
    irc = UTO_cv_cvtrm (&cadObjCv.typ, cvo, NULL, &tcv);
    if(irc < 0) { irc = -4; goto L_err;}
    cadObjCv.obj = (void*)cvo;
      // DEB_dump_obj__ (cadObjCv.typ, cadObjCv.obj, "cvo");
  }

  // export curve
  ecv = IGE_w_obj__ (&cadObjCv);
  // ecv = IGE_w_obj_dist (&cadObjCv);
  if(ecv < 0) {irc = -5; goto L_err;}


  //----------------------------------------------------------------
  cadObj1->ent = IG_EntSRV;   // 120

  // write centerLine
  IGE_w_P_i1 (eln);

  // write curve
  IGE_w_P_i1 (ecv);

  // write angles
  a1 = srv->ang1;
  a2 = srv->ang2;
  if(srv->dir) { // change angles
    MEM_swap_2db (&a1, &a2);
  }
  IGE_w_P_db1 (&a1);
  IGE_w_P_db1 (&a2);


  return 0;

  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_SRV E%d %d %ld",irc,cadObj1->typ, cadObj1->dbi);
    return -1;

}


//===================================================================
  int IGE_w_SRU (cadObj__ *cadObj1) {
//===================================================================
// IGE_w_SRU                               118 RuledSurface
//     Pointer 1. curve
//     Pointer 2. curve
//     (int)   direction; 0=first to first, 1=first to last
//     (int)   development flag: 1=developable, 0=possibly not

  int       irc, i1, ecv1, ecv2, iDir, zDir;
  ObjGX     *oTab, *ox1;
  cadObj__  cadObj2 = cadObj_NUL;
  cadObj__  cadObj3 = cadObj_NUL;


  oTab = cadObj1->obj;

  // printf("\n===============IGE_w_SRU================================ \n");
  // IGE_w_dump_cadObj(cadObj1,"IGE_w_SRU-in");
  // DEB_dump_obj__(Typ_ObjGX,cadObj1->obj,"IGE_w_SRU-in");
  // for(i1=0;i1<cadObj1->oNr;++i1) DEB_dump_obj__(Typ_ObjGX,&oTab[i1],"oxa[%d]",i1);


  // curve + vector: goto IGE_w_SRC
  if(oTab[1].typ == Typ_VC) return IGE_w_SRC (cadObj1);


  //----------------------------------------------------------------
  // export curve1
  ox1 = &oTab[0];
  if(ox1->form == Typ_Index) {
    OGX_GET_INDEX (&cadObj2.typ, &cadObj2.dbi, ox1);

    // export curve
    ecv1 = IGE_w_obj__ (&cadObj2);
    // ecv1 = IGE_w_obj_dist (&cadObj2);
    if(ecv1 < 0) {irc = -1; goto L_err;}

  } else { irc = -2; goto L_err;}


  //----------------------------------------------------------------
  // export curve2
  ox1 = &oTab[1];
  if(ox1->form == Typ_Index) {
    OGX_GET_INDEX (&cadObj3.typ, &cadObj3.dbi, ox1);

    // export curve
    ecv2 = IGE_w_obj__ (&cadObj3);
    // ecv2 = IGE_w_obj_dist (&cadObj3);
    if(ecv2 < 0) {irc = -2; goto L_err;}

  } else { irc = -2; goto L_err;}


  //----------------------------------------------------------------
  cadObj1->ent = IG_EntSRU;   // 118

  // write curve1, curve2
  IGE_w_P_i1 (ecv1);
  IGE_w_P_i1 (ecv2);


  // need obj; if curves store earlier; get obj
  if(!cadObj2.obj) IGE_w_eTab_obj (&cadObj2);
  if(!cadObj3.obj) IGE_w_eTab_obj (&cadObj3);


  // (int) i1 = direction; 0=first to first, 1=first to last
  iDir = 0;
  if(oTab[0].dir != oTab[1].dir) iDir = 1;
  
  if((oTab[0].typ == Typ_CI) && (oTab[1].typ == Typ_CI))      {
      // DEB_dump_obj__ (Typ_CI, cadObj2.obj, "IGE_w_SRU-ci1");
      // DEB_dump_obj__ (Typ_CI, cadObj3.obj, "IGE_w_SRU-ci2");

    // both circs are CCW; (one or both are changed from CW -> CCW)
    if(oTab[0].dir != oTab[1].dir) {
      // dir shall be changed; test if circs have different dir;
      // - if yes (1): ignore - already done (both circs CCW)
      i1 = UT3D_ci2_ck_dir ((Circ*)cadObj2.obj, (Circ*)cadObj3.obj);
      if(i1) iDir = 0; // different circDirs - ignore
      else   iDir = 1; // same circDirs - change dir
    }

    // z-vectors of circs can have oppos. direction;
    // test if z-vectors are parallel or opposite
    zDir = UT3D_sid_2vc (&((Circ*)cadObj2.obj)->vz, &((Circ*)cadObj3.obj)->vz);
    if(!zDir) {irc = -3; goto L_err;}
      // printf(" IGE_w_SRU-zDir=%d\n",zDir);
    // different dir: change direction
    if(zDir < 0) ICHG01(iDir);
  }
    // printf(" IGE_w_SRU-iDir=%d\n",iDir);

  IGE_w_P_i1 (iDir);

  // (int)   development flag: 1=developable, 0=possibly not
  IGE_w_P_i1 (0);

    // TESTBLOCK
    // IGE_w_dump_cadObj(cadObj1,"ex-IGE_w_SRU");
    // END TESTBLOCK
  
  return 0;


  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_SRU E%d %d %ld",irc,cadObj1->typ, cadObj1->dbi);
    return -1;
}


//===================================================================
  int IGE_w_SRC (cadObj__ *cadObj1) {
//===================================================================
// 122 ruled-surface; Tabulated-Cylinder-Entity (curve/vector)
// 122,<curve>,<endpoint of vector above startpoint of curve>

//   int    irc, typ, rNr, dNr;
//   long   icv, ivc;
//   Vector *vc1;
//   ObjGX  *objTab, *obj1, *obj2;
// 
// 
//   objTab = el->data;
//   obj1   = &objTab[0];
//   obj2   = &objTab[1];

  int       irc, i1, typ, ecv, pTyp;
  long      dbi;
  Vector    *vc1;
  Point     pt1, pt2;
  ObjGX     *oTab, *obj1;
  cadObj__  cadObj2 = cadObj_NUL;

  oTab = cadObj1->obj;


  // printf("-------------------------------------------------------- \n");
  // printf("IGE_w_SRC-in typ=%d siz=%d\n",cadObj1->typ,cadObj1->oNr);
  // for(i1=0;i1<cadObj1->oNr;++i1) DEB_dump_obj__(Typ_ObjGX,&oTab[i1],"oxa[%d]",i1);


  //----------------------------------------------------------------
  // get curve
  obj1 = &oTab[0];
    // DEB_dump_ox_s_ (obj1, "IGE_w_SRC-cv");

  if(obj1->form == Typ_Index) {
    OGX_GET_INDEX (&cadObj2.typ, &cadObj2.dbi, obj1);

    // export curve
    ecv = IGE_w_obj__ (&cadObj2);
    if(ecv < 0) {irc = -4; goto L_err;}

  } else { irc = -1; goto L_err;}


  //----------------------------------------------------------------
  // get vc1 = vector 
  obj1 = &oTab[1];
    // DEB_dump_ox_s_ (obj1, "IGE_w_SRC-vc");

  if(obj1->form == Typ_Index) {
    OGX_GET_INDEX (&typ, &dbi, obj1);

    if(typ == Typ_VC) {
      vc1 = DB_get_VC (dbi);
        // DEB_dump_obj__ (Typ_VC, vc1, "IGE_w_SRC-VC");
    } else { irc = -2; goto L_err;}

  } else { irc = -3; goto L_err;}


  //----------------------------------------------------------------
  // get pt1 = startPoint of curve
  pTyp = Ptyp_start;  // LN,PLG,BSPL, ..

  if(cadObj2.typ == Typ_CI) {
    if(((Circ*)cadObj2.obj)->rad < 0.) {
      // circ cw; but changed to CCW
      pTyp = Ptyp_end;
    }
  }

  // get point pt1 = startPt curve
  irc = UT3D_ptvcpar_std_dbo (&pt1, NULL, NULL, pTyp, cadObj2.typ, cadObj2.dbi);
  if(irc < 0) {irc = -4; goto L_err;}
    // DEB_dump_obj__ (Typ_PT, &pt1, "IGE_w_SRC-PT");

  // get pt2 = <endpoint of vector above startpoint of curve>
  // pt2 = pt1 + vc1;
  UT3D_pt_traptvc (&pt2, &pt1, vc1);


  //----------------------------------------------------------------
  cadObj1->ent = IG_EntSRC;   // 122

  // write curve
  IGE_w_P_i1 (ecv);

  // write endPt of vec
  IGE_w_P_pt1 (&pt2);

  return 0;


  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_SRC E%d %d %ld",irc,cadObj1->typ, cadObj1->dbi);
    return -1;

}


//===================================================================
  int IGE_w_SCI (cadObj__ *cadObj1) {
//===================================================================
// IGE_w_SCI              export Typ_SURCIR tesselated-fan as ??
// 118 RuledSurface
//     Pointer 1. curve
//     Pointer 2. curve
//     (int)   direction; 0=first to first, 1=first to last
//     (int)   development flag: 1=developable, 0=possibly not


  int       irc, i1, ecv, epl, ebnd;
  Point     pta[4];
  ObjGX     *oTab, *obj1, ox1;
  cadObj__  cadObj2 = cadObj_NUL;
  cadObj__  cadObj3 = cadObj_NUL;
  CurvPoly  plg1;
  Memspc    memSeg1;
  char      tmpSpc[1024];
  Plane     pln1;


  oTab = cadObj1->obj;


  // printf("-------------------------------------------------------- \n");
  // DEB_dump_obj__(Typ_ObjGX, cadObj1->obj, "IGE_w_SCI-in");


  //----------------------------------------------------------------
  // get points
  for(i1=0; i1<3; ++i1) {
    obj1 = &oTab[i1];
      // DEB_dump_ox_s_ (obj1, "IGE_w_SCI-%d",i1);

    if(obj1->form == Typ_Index) {
      OGX_GET_INDEX (&cadObj2.typ, &cadObj2.dbi, obj1);

      // get data 
      ox1 = DB_GetObjGX (cadObj2.typ, cadObj2.dbi);
      if(ox1.typ == Typ_Error) { irc = -1; goto L_err;}
      pta[i1] = *(Point*)ox1.data;
  
    } else { irc = -2; goto L_err;}
  }

  pta[3] = pta[0];

  //----------------------------------------------------------------
  // export polygon
  cadObj1->ent = IG_EntPOL;
  cadObj1->form = 2;       // data-triples

  IGE_w_106 (4, pta);      // add points


  return 0;


  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_SCI E%d %d %ld",irc,cadObj1->typ, cadObj1->dbi);
    return -1;

}


/* sru from line + pt; OK, but not supported by cascade;
//===================================================================
  int IGE_w_SCI (cadObj__ *cadObj1) {
//===================================================================
// IGE_w_SCI              export Typ_SURCIR tesselated-fan as ??
// 118 RuledSurface
//     Pointer 1. curve
//     Pointer 2. curve
//     (int)   direction; 0=first to first, 1=first to last
//     (int)   development flag: 1=developable, 0=possibly not


  int       irc, i1, ept, ecv;
  Line      ln1;
  ObjGX     *oTab, *obj1, ox1;
  cadObj__  cadObj2 = cadObj_NUL;


  oTab = cadObj1->obj;
  

  printf("-------------------------------------------------------- \n");
  DEB_dump_obj__(Typ_ObjGX, cadObj1->obj, "IGE_w_SCI-in");


  //----------------------------------------------------------------
  // export point 0
  obj1 = &oTab[0];
    DEB_dump_ox_s_ (obj1, "IGE_w_SCI-0");

  if(obj1->form == Typ_Index) {
    OGX_GET_INDEX (&cadObj2.typ, &cadObj2.dbi, obj1);

    // export point 0
    ept = IGE_w_obj__ (&cadObj2);
    if(ept < 0) {irc = -1; goto L_err;}
      printf(" w_SCI-1 %d %d\n",i1,ept);

  } else { irc = -2; goto L_err;}


  //----------------------------------------------------------------
  // get point 1 and 2 out as line 110
  obj1 = &oTab[1];
    DEB_dump_ox_s_ (obj1, "IGE_w_SCI-1");
    
  if(obj1->form == Typ_Index) { 
    OGX_GET_INDEX (&cadObj2.typ, &cadObj2.dbi, obj1);
      
    // get data 
    ox1 = DB_GetObjGX (cadObj2.typ, cadObj2.dbi);
    if(ox1.typ == Typ_Error) { irc = -3; goto L_err;}
    ln1.p1 = *(Point*)ox1.data;

  } else { irc = -4; goto L_err;}


  obj1 = &oTab[2];
    DEB_dump_ox_s_ (obj1, "IGE_w_SCI-2");

  if(obj1->form == Typ_Index) {  
    OGX_GET_INDEX (&cadObj2.typ, &cadObj2.dbi, obj1);
      
    // get data 
    ox1 = DB_GetObjGX (cadObj2.typ, cadObj2.dbi);
    if(ox1.typ == Typ_Error) { irc = -5; goto L_err;}
    ln1.p2 = *(Point*)ox1.data;

  } else { irc = -6; goto L_err;}
    DEB_dump_obj__(Typ_LN, &ln1, "IGE_w_SCI-ln1");


  //----------------------------------------------------------------
  // export line
  cadObj2.typ = Typ_LN;
  cadObj2.dbi = 0;
  cadObj2.obj = &ln1;
  ecv = IGE_w_obj_dist (&cadObj2);
  if(ecv < 0) {irc = -7; goto L_err;}


  //----------------------------------------------------------------
  // export surf line + point
  cadObj1->ent = IG_EntSRU;   // 118

  IGE_w_P_i1 (ecv);
  IGE_w_P_i1 (ept);
  IGE_w_P_i1 (0);
  IGE_w_P_i1 (0);


  return 0;

  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_SCI E%d %d %ld \n",irc,cadObj1->typ, cadObj1->dbi);
    return -1;


}
*/
/*
//===================================================================
  int IGE_w_SCI (cadObj__ *cadObj1) {
//===================================================================
// IGE_w_SCI              export Typ_SURCIR tesselated-fan as ??
// 118 RuledSurface
//     Pointer 1. curve
//     Pointer 2. curve
//     (int)   direction; 0=first to first, 1=first to last
//     (int)   development flag: 1=developable, 0=possibly not


  int       irc, i1, ept[3], ecv, epl, ebnd;
  // Line      ln1;
  Point     pta[3];
  ObjGX     *oTab, *obj1, ox1;
  cadObj__  cadObj2 = cadObj_NUL;
  cadObj__  cadObj3 = cadObj_NUL;
  CurvPoly  plg1;
  Memspc    memSeg1;
  char      tmpSpc[1024];
  Plane     pln1;


  oTab = cadObj1->obj;


  printf("-------------------------------------------------------- \n");
  DEB_dump_obj__(Typ_ObjGX, cadObj1->obj, "IGE_w_SCI-in");


  //----------------------------------------------------------------
  // export points
  for(i1=0; i1<3; ++i1) {
    obj1 = &oTab[i1];
      DEB_dump_ox_s_ (obj1, "IGE_w_SCI-%d",i1);

    if(obj1->form == Typ_Index) {
      OGX_GET_INDEX (&cadObj2.typ, &cadObj2.dbi, obj1);

      // get data 
      ox1 = DB_GetObjGX (cadObj2.typ, cadObj2.dbi);
      if(ox1.typ == Typ_Error) { irc = -1; goto L_err;}
      pta[i1] = *(Point*)ox1.data;
  
    } else { irc = -2; goto L_err;}
  }


  //----------------------------------------------------------------
  // out ecv = polygon from points 106
  UME_init (&memSeg1, tmpSpc, sizeof(tmpSpc));
  irc = UT3D_plg_pta (&plg1, &pta, 3, &memSeg1);
  if(irc < 0) {irc = -3; goto L_err;}

  cadObj2.typ = Typ_CVPOL;
  cadObj2.dbi = 0;
  cadObj2.obj = &plg1;

  ecv = IGE_w_obj_dist (&cadObj2);
  if(ecv < 0) {irc = -3; goto L_err;}
    printf(" IGE_w_SCI-ecv=%d\n",ecv);



  //----------------------------------------------------------------
  // out epl = supporting-plane
  // test if all points are in baseplane
  i1 = UT3D_pl_ck_bas (pta, 3, &UT_TOL_pt);  // 1=yes-in-basePlane; 0=not-in-baseplane;
  if(i1) epl = 1;
  else {
    // get plane
    UT3D_pl_3pt (&pln1, &pta[0], &pta[1], &pta[2]);
      DEB_dump_obj__ (Typ_PLN, &pln1, "IGE_w_SCI-pln1\n");
    cadObj2.typ = Typ_PLN;
    cadObj2.dbi = 0;
    cadObj2.obj = &pln1;
    epl = IGE_w_obj_dist (&cadObj2);
    if(epl < 0) {irc = -3; goto L_err;}
  }
    printf(" IGE_w_SCI-epl=%d\n",epl);


  //----------------------------------------------------------------
  // out 143 = Trimmed-Surface
  cadObj1->ent = IG_EntTS;   // 143


//....................................
//   // export bnd as 142=CurveOnParametricSurface
//   cadObj2.ent = IG_EntBND;   // 142
// 
//   cadObj2.typ = Typ_CVPOL;
//   cadObj2.dbi = 0;
//   cadObj2.obj = &plg1;
// 
//   // write 142 - D- and P-lines
//   ebnd = IGE_w_BND (&cadObj2);
//   if(ebnd < 0) {irc = -2; goto L_err;}
// 
// 
//     // printf("ex-IGE_w_BND %d\n",iEnt);
//   return iEnt;


  return 0;

  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_SCI E%d %d %ld \n",irc,cadObj1->typ, cadObj1->dbi);
    return -1;


}
*/


//===================================================================
  int IGE_w_SBS (cadObj__ *cadObj1) {
//===================================================================
// 128 Rational B-Spline Surface
//     K1 (int) upper index of sum
//     K2 (int) upper index of sum
//     M1 (int) degree of basic functions
//     M2 (int) degree of basic functions
//     Prop1 (int) 0=not closed in 1.dir, 1=closed
//     Prop2 (int) 0=not closed in 2.dir, 1=closed
//     Prop3 (int) 0=rational, 1=polynomial
//     Prop4 (int) 0=nonperodic in 1.dir, 1=periodic.
//     Prop5 (int) 0=nonperodic in 2.dir, 1=periodic.
//     T[]   (floats) 1. KnotSequence Anzahl=K-M+1+2*M
//     T[]   (floats) 2. KnotSequence Anzahl=K-M+1+2*M
//     W[]   (floats) Weigths, Anzahl=K
//     X,Y,Z (3 floats) CtrlPoints, Anzahl=K
//     V(0)  (float) 1. StartParameterWert
//     V(1)  (float) 1. EndParameterWert
//     V(0)  (float) 2. StartParameterWert
//     V(1)  (float) 2. EndParameterWert
//
//
// cpTab[ptUNr*ptVNr]    u0v0,u1v0..uUv0;u1v0,u1v1..uUv1;..;uUv0,uUv1..uUvV;
// kvTabU[ptUNr+degU+1]
// kvTabV[ptVNr+degV+1]


  int       i1, i2, i3;
  double    *dTab;
  Point     *pTab;
  SurBSpl   *sbs;


  sbs = (SurBSpl*) cadObj1->obj;

  // printf("-------------------------------------------------------- \n");
  // DEB_dump_obj__(Typ_SURBSP, sbs, "IGE_w_SBS-in");

  // write sumIndex's
  IGE_w_P_i1 (sbs->ptUNr - 1);
  IGE_w_P_i1 (sbs->ptVNr - 1);

  // write degrees
  IGE_w_P_i1 (sbs->degU, 0.);
  IGE_w_P_i1 (sbs->degV, 0.);

  IGE_w_P_i1 (0); // 0=not closed   TODO !
  IGE_w_P_i1 (0); // 0=not closed   TODO !
  IGE_w_P_i1 (1); // 1=polynom.
  IGE_w_P_i1 (0); // 0=nonperodic
  IGE_w_P_i1 (0); // 0=nonperodic

  // write U-knots
  i1 = sbs->ptUNr + sbs->degU + 1;
  dTab = sbs->kvTabU;
  for(i2=0; i2<i1; ++i2) {
    IGE_w_P_db1 (&dTab[i2]);
  }

  // write V-knots
  i1 = sbs->ptVNr + sbs->degV + 1;
  dTab = sbs->kvTabV;
  for(i2=0; i2<i1; ++i2) {
    IGE_w_P_db1 (&dTab[i2]);
  }

  // write weigths
  for(i1=0; i1<sbs->ptVNr; ++i1) {
    for(i2=0; i2<sbs->ptUNr; ++i2) {
      IGE_w_P_i1 (1);
    }
  }

  // wr CtrlPoints
  pTab = sbs->cpTab;
  i3 = 0;
  for(i1=0; i1<sbs->ptVNr; ++i1) {
    for(i2=0; i2<sbs->ptUNr; ++i2) {
      IGE_w_P_pt1 (&pTab[i3]);
      ++i3;
    }
  }

  // wr U-start/endVal
  dTab = sbs->kvTabU;
  IGE_w_P_db1 (dTab[0]);
  i1 = sbs->ptUNr + sbs->degU;
  IGE_w_P_db1 (&dTab[i1]);

  // wr V-start/endVal
  dTab = sbs->kvTabV;
  IGE_w_P_db1 (dTab[0]);
  i1 = sbs->ptVNr + sbs->degV;
  IGE_w_P_db1 (&dTab[i1]);


  cadObj1->ent = IG_EntSBS;   // 128

  return 0;

}


//===================================================================
  int IGE_w_SUTP (cadObj__ *cadObj1) {
//===================================================================
// IGE_w_SUTP       export trimmed-perforated-surf - 144
//
// 144 = trimmed-perforated-surf
// 144,<supportSurf>,1,<nrInnBnds>,<oBnd>[,<iBnds..>]

  int       irc, i1, iSu, *subObjTab, cNr;
  ObjGX     *oTab, *oss, *bnd;
  cadObj__  cadObj2 = cadObj_NUL;


  oTab = cadObj1->obj; 

  // printf("\n\n-------------------------------------------------------- \n");
  // printf("IGE_w_SUTP-in typ=%d siz=%d\n",cadObj1->typ,cadObj1->oNr);
  // IGE_w_dump_eTab ("IGE_w_SUTP-in");
  // for(i1=0;i1<cadObj1->oNr;++i1) DEB_dump_obj__(Typ_ObjGX,&oTab[i1],"oxa[%d]",i1);


  // cNr = nr boundaries
  cNr  = cadObj1->oNr - 1;  // oss,outerBnd[,holes..]


  // get spc for subObjTab
  subObjTab = (int*) MEM_alloc_tmp ((cadObj1->oNr + 2) * sizeof(int));


  //----------------------------------------------------------------
  // get ObjGX oss = support-surface
  oss = &oTab[0];
    // DEB_dump_ox_s_ (oss, "IGE_w_SUTP-ss");

  // get typ and dbi from ObjGX
  if(oss->form == Typ_Index) {
    OGX_GET_INDEX (&cadObj2.typ, &cadObj2.dbi, oss); 
  } else { irc = -1; goto L_err;}

  // planar-surface: can be Typ_SUTP; get 


  // get cadObj2.obj = dataStruct for ObjGX oss
  UTO_objDat_ox (&cadObj2.obj, &cadObj2.oNr, oss);


  // export oss
  irc = IGE_w_obj__ (&cadObj2);
  if(irc < 0) {irc = -2; goto L_err;}
  subObjTab[0] = irc;


  //----------------------------------------------------------------
  // export boundaries; out curveObjects and boundary IG_EntBND (group of curves)

  for(i1=0; i1<cNr; ++i1) {
    bnd = &oTab[i1 + 1];
      // DEB_dump_ox_s_ (bnd, "SUTP-bnd-nxt");

    // get typ and dbi from ObjGX
    if(bnd->form == Typ_Index) {
      OGX_GET_INDEX (&cadObj2.typ, &cadObj2.dbi, bnd);
    } else { irc = -3; goto L_err;}

    // export obj
    cadObj2.ipa[0] = subObjTab[0];   // oss
    irc = IGE_w_BND (&cadObj2);
    if(irc < 0) {irc = -3; goto L_err;}
    subObjTab[2 + i1] = irc;    // IG_D_line_nr - 1;
  }


  //----------------------------------------------------------------
  // write 144 = TrimmedPerforatedSurf

  cadObj1->ent = IG_EntSTP;

  // set trimmed-flag; 
  if(cadObj2.typ == Typ_CI) subObjTab[1] = 0;   // untrimmed
  else                      subObjTab[1] = 1;   // trimmed


  IGE_w_P_i1 (subObjTab[0]);      // Pointer to supportSurface
  IGE_w_P_i1 (subObjTab[1]);      // TrimmedFlag; 0=untrimmed, 1=trimmed
  IGE_w_P_i1 (cNr-1);             // nr of innerBoundaries
  IGE_w_P_i1 (subObjTab[2]);      // Pointer to outerBoundary
  for(i1=1; i1<cNr; ++i1)
    IGE_w_P_i1 (subObjTab[i1+2]); // Pointer to innerBoundaries


  // IGE_w_obj__,IGE_w_obj_dist exports the top-level-obj

  return 0;


  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_SUTP E%d %d %ld",irc,cadObj1->typ, cadObj1->dbi);
    return -1;

}



//===================================================================
  int IGE_w_PRI (cadObj__ *cadObj1) {
//===================================================================
// IGE_w_PRI       export solid prism 164
//

//   int       irc, i1, iSu, *subObjTab, cNr;
//   ObjGX     *oTab, *oss, *bnd;
//   cadObj__  cadObj2 = cadObj_NUL;

  int       irc, i1, iSsu, iBnd, typ;
  long      dbi;
  double    dLen;
  Vector    vc1, *vcz;
  Plane     pln1;
  ObjGX     *oTab, *ox1;
  cadObj__  cadObj2 = cadObj_NUL;
  cadObj__  cadObj3 = cadObj_NUL;


 // IGE_w_dump_cadObj (cadObj1, "SSSSSSSSSSSSSSSSSSSSSSSSSS IGE_w_PRI");
// // no solids supported by cascade; UT3D_pl_dbo unfinished (PRCV_get_dbo__ E1-20-20)
return -1;


  oTab = cadObj1->obj;
  // for(i1=0;i1<cadObj1->oNr;++i1) DEB_dump_obj__(Typ_ObjGX,&oTab[i1],"oxa[%d]",i1);
  // baseContour +  thickness or vector


  //----------------------------------------------------------------
  // get baseContour
  ox1 = &oTab[0];

  // get typ and dbi from ObjGX
  if(ox1->form == Typ_Index) {
    OGX_GET_INDEX (&cadObj3.typ, &cadObj3.dbi, ox1);

    // get plane of baseContour = support-surface iSsu
    irc = UT3D_pl_dbo (&pln1, cadObj3.typ, cadObj3.dbi);
    if(irc < 0) { irc = -1; goto L_err;}

  } else { irc = -2; goto L_err;}


  //----------------------------------------------------------------
  // export plane
  cadObj2.typ = Typ_PLN;
  cadObj2.obj = &pln1;
  iSsu = IGE_w_obj_dist (&cadObj2);
  if(iSsu < 0) {irc = -3; goto L_err;}
    // DEB_dump_obj__ (Typ_PLN, &pln1, " pln1");


  //----------------------------------------------------------------
  // export baseContour
  cadObj3.ipa[0] = iSsu;              // support-surface
  iBnd = IGE_w_BND (&cadObj3);
  if(iBnd < 0) {irc = -4; goto L_err;}


  //----------------------------------------------------------------
  // get Vector and length
  ox1 = &oTab[1];

  if(ox1->form == Typ_Index) { 
    OGX_GET_INDEX (&typ, &dbi, ox1);
  } else { irc = -5; goto L_err;}
    // printf(" PRI-thick %d %ld\n",typ,dbi);


  if(typ == Typ_VAR) {
    dLen = DB_GetVar (dbi);  //  printf(" dLen=%f\n",dLen);
    vcz = &pln1.vz;

  } else if(typ == Typ_VC) {
    vc1 = DB_GetVector (dbi);
    dLen = UT3D_len_vc (&vc1);
    UT3D_vc_setLength (&vc1, &vc1, 1.);        // normalize
    vcz = &vc1;

  } else { irc = -6; goto L_err; }


  //----------------------------------------------------------------
  // 164 - Solid of Linear Extrusion
  // 164,<curve>,<length>,<vector>

  cadObj1->ent = IG_EntEXT;

  IGE_w_P_i1 (iBnd);
  IGE_w_P_db1 (&dLen);
  IGE_w_P_vc1 (vcz);

  return 0;


  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_PRI E%d %d %ld",irc,cadObj1->typ, cadObj1->dbi);
    return -1;

}


//===================================================================
  int IGE_w_BND (cadObj__ *cadObj1) {
//===================================================================
// IGE_w_BND                             boundary IG_EntBND 142
// - bnd must be closed; can be CI, polygon, CCV;
// 142 = boundary
// 142,0,<supportSurf>,0,<CCV>,2

  int       irc, i1, ii, ipccv, iEnt, iTbe;
  cadObj__  cadObj2 = cadObj_NUL;
  ObjGX     ox1;

  

  // IGE_w_dump_cadObj (cadObj1, "BBBBBBBBBBBBBBBBBBBBBBBBBB IGE_w_BND");
  // IGE_w_dump_eTab ("IGE_w_BND-in");

  // get CAD-obj
  ox1 = DB_GetObjGX (cadObj1->typ, cadObj1->dbi);
  if(ox1.typ == Typ_Error) { irc = -1; goto L_err;}
    // DEB_dump_ox_s_ (&ox1, "IGE_w_BND-bnd");

  cadObj2.obj = ox1.data;
  cadObj2.oNr = ox1.siz;
    

  //----------------------------------------------------------------
  // export all curves of boundary;
  // test if already exported
  ii = IGE_w_eTab_find (&iTbe, ox1.typ, cadObj1->dbi);
  if(ii > 0) {
    ipccv = ii;
    // test if ipccv is closed curve; yes: goto L_142
    if(expTab[iTbe].clo == 0) goto L_142;
    // if(IGE_w_eTab_ck_clo(&itb, ipccv) == 0) goto L_142;
  }

  // set boundary to be closed curve
  cadObj2.clo = 0;

  // write boundary; get its entID as ipccv
  cadObj2.typ = ox1.typ;
  cadObj2.dbi = cadObj1->dbi;
  ipccv = IGE_w_obj_dist (&cadObj2);
  if(ipccv < 0) {irc = -2; goto L_err;}
    // printf(" IGE_w_BND-ipccv=%d\n",ipccv);


  //----------------------------------------------------------------
  L_142:
    // printf("IGE_w_BND-L_142: ipss=%d ipccv=%d\n",cadObj1->ipa[0],ipccv);


  // export bnd as 142=CurveOnParametricSurface
  cadObj1->ent = IG_EntBND;   // 142

  IGE_w_P_i1 (0);               // CreationFlag: int 0=unspec.
  IGE_w_P_i1 (cadObj1->ipa[0]); // support-surface (108|..)
  IGE_w_P_i1 (0);               // 2D-representation = 0 (no)
  IGE_w_P_i1 (ipccv);           // Ptr 3D-Curve Boundary (CI,CCV ..)
  IGE_w_P_i1 (2);               // display:  2=3D

  // write 142 - D- and P-lines
  iEnt = IGE_w_obj_out__ (cadObj1);

    // printf("ex-IGE_w_BND %d\n",iEnt);

  return iEnt;


  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_BND E%d %d %ld",irc,cadObj1->typ, cadObj1->dbi);
    return -1;

  L_not_closed:
    LOG_A__ (ERR_ERR,"***** IGE_w_BND E%d %d %ld",99,cadObj1->typ, cadObj1->dbi);
    return -1;

}


//===================================================================
  int IGE_w_MD (char *mdlNam) {
//===================================================================
// 308. (SubfigDef.)
// int   Depth of subfigure (indicating the amount of nesting); 0 = none
// strg  Subfigure name  eg "8HTRIANGLE"
// int   entNr
// int[] entID's

  int       i1, iNr, *ia;
  cadObj__  cadObj1 = cadObj_NUL;


  // printf("IGE_w_MD |%s|\n",mdlNam);

  // Depth 0
  IGE_w_P_i1 (0);

  // Name
  IGE_w_P_txt (mdlNam);

  // number of entID's
  iNr = MEMTAB_IND (&iaEnt);
  IGE_w_P_i1 (iNr);

  // entID's
  ia = MEMTAB_DAT (&iaEnt);
  for(i1=0; i1<iNr; ++i1) IGE_w_P_i1 (ia[i1]);


  //----------------------------------------------------------------
  cadObj1.ent = IG_EntMD;   // 308

  // write D- and P-lines
  IGE_w_obj_outP (&cadObj1);
  IGE_w_obj_outD (&cadObj1);

  // init D- and P-lines
  IGE_w_obj_init ();


  //----------------------------------------------------------------
  // clear iaEnt
  MEMTAB_CLEAR (&iaEnt);

  return IG_D_line_nr - 1;

}


//===================================================================
  int IGE_w_MR (cadObj__ *cadObj1) {
//===================================================================
// IGE_w_MR               Typ_Model ditto 408. (SubfigInst.)
// int   SubfigDef (308-entID)
// dbl   translVec x,y,z (0,0,0)
// dbl   scale



  int        irc, i1, iEntMdl = -1;
  double     d1;
  Plane      pl1;
  ModelRef   *mr1;
  Mat_4x3    m1;


  // printf("=============================================== \n");
  // IGE_w_dump_cadObj(cadObj1,"IGE_w_MR-in");


  // get subModel-definition
  mr1 = (ModelRef*)cadObj1->obj;
    // DEB_dump_obj__ (Typ_Model, mr1, "IGE_w_P_MR");


  //----------------------------------------------------------------
  // add TrMat

  // get Plane from ci3-center, ci3-axis, startpoint of circ = x-axis
  UT3D_pl_pto_vcz_vcx (&pl1, &mr1->po, &mr1->vz, &mr1->vx);

  // get Matrix from Plane erstellen
  UT3D_m3_loadpl (m1, &pl1);

  // write TrMat
  cadObj1->trNr = IGE_w_124 (m1);


  //----------------------------------------------------------------
  cadObj1->ent = IG_EntMR;  // 408

 
  // find modelEntity in IG_modTab
  for(i1=0; i1<IG_modNr; ++i1) {
    if(IG_modTab[i1].modNr == mr1->modNr) { iEntMdl = IG_modTab[i1].modEnt; break;}
  }
  if(iEntMdl < 0) {irc = -1; goto L_err;}

  // add entID of SubfigDef
  IGE_w_P_i1 (iEntMdl);

  // Position 0,0,0  (via Trafo(124) setzen!)
  d1 = 0.;
  IGE_w_P_db1 (&d1);
  IGE_w_P_db1 (&d1);
  IGE_w_P_db1 (&d1);

  // Scale
  d1 = mr1->scl;
  if(fabs(d1) < 0.00001) d1 = 1.;
  IGE_w_P_db1 (&d1);

  return 0;


  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_MR E%d %d %ld",irc,cadObj1->typ, cadObj1->dbi);
    return -1;


}


//================================================================
  int IGE_w_124 (Mat_4x3 m1) {
//================================================================
// returns D-lineNr of 124

  int       iEnt;
  cadObj__  cadObj1 = cadObj_NUL;

  cadObj1.ent = IG_EntTRA;   // 124

  // printf("IGE_w_124  ent=%d\n",cadObj1.ent);


  IGE_w_P_db1 (&m1[0][0]);
  IGE_w_P_db1 (&m1[0][1]);
  IGE_w_P_db1 (&m1[0][2]);
  IGE_w_P_db1 (&m1[0][3]);

  IGE_w_P_db1 (&m1[1][0]);
  IGE_w_P_db1 (&m1[1][1]);
  IGE_w_P_db1 (&m1[1][2]);
  IGE_w_P_db1 (&m1[1][3]);

  IGE_w_P_db1 (&m1[2][0]);
  IGE_w_P_db1 (&m1[2][1]);
  IGE_w_P_db1 (&m1[2][2]);
  IGE_w_P_db1 (&m1[2][3]);

  iEnt = IGE_w_obj_out__ (&cadObj1);

  return iEnt;

}


/* UNUSED (integreate into IGE_w_BND)
//===================================================================
  int IGE_w_142 (ObjGX *el) {
//===================================================================
// 142 CurveOnParametricSurface
//     (int) CreationFlag 0=unspec;1=proj.Curv->Surf;2=Inters.Surf/Surf;
//             3=IsoparametricCurve.
//     Pointer to supportSurf (Surf on which curv lies); eg 108 od 128
//     Ptr to 2D-Ableitung der Curve; dzt immer 0.
//     Ptr to 3D-Curve Boundary (eg CCV - 102) or 100-Circ!
//     (int)   Darstellung:0=unspec;1=2D;2=3D;3:2D od 3D; dzt immer 3.
/
  int    *iTab;

  // printf("IGE_w_P_142\n");

// write 142

  int    iTab[2];
  ObjGX  ox1;

  iTab[0] = ibs; // Pointer to Stuetzflaeche
  iTab[1] = ica; // Pointer to 3D-Curve

  ox1.typ   = Typ_goGeo1;
  ox1.form  = Typ_Index;
  ox1.siz   = 2;
// TODO: use Typ_Int8
  ox1.data  = (void*)&iTab;
  IGE_w_obj (&ox1, 0, 0);

//----------------------------------------------------------------
 
  iTab = el->data;

  IGE_w_P_i1 (0);       // CreationFlag: 0=unspec.
  IGE_w_P_i1 (iTab[0]); // Pointer to Stuetzflaeche
  IGE_w_P_i1 (0);       // Ptr auf die 2D-Ableitung = 0.
  IGE_w_P_i1 (iTab[1]); // Ptr auf die 3D-Curve Boundary
  IGE_w_P_i1 (2);       // Darstellung:  2=3D
/
  return 0;

}



//===================================================================
  int IGE_w_144 (ObjGX *el) {
//===================================================================
// 144 Trimmed Parametric Surface
//  0  Ptr auf die Stuetzflaeche; zB auf 102 od 128
//  1  TrimmedFlag; 0=untrimmed, 1=trimmed
//  2  IBN = Anzahl Inseln innerhalb der Boundary
//  3  Ptr auf die Boundary; (-> 142)
//     Ptr[IBN] auf die Inseln ..
/
  int    i1, *iTab, cNr;


  // [0] = supportSurface;
  // [1] = outer boundary
  // [2] = first hole ..
  iTab = &IG_subObjTab[0];

  printf("IGE_w_P_144\n");
  for(i1=0;i1<IG_subObjNr;++i1) printf(" iTab[%d] = %d\n",i1,iTab[i1]);


  // cNr = nr holes
  cNr  = IG_subObjNr - 3;


  IGE_w_P_i1 (iTab[0]);      // Pointer to supportSurface
  IGE_w_P_i1 (iTab[1]);      // TrimmedFlag; 0=untrimmed, 1=trimmed
  IGE_w_P_i1 (cNr);          // nr of innerBoundaries
  IGE_w_P_i1 (iTab[2]);      // Pointer to outerBoundary
  for(i1=0; i1<cNr; ++i1)
    IGE_w_P_i1 (iTab[i1+3]); // Pointer to innerBoundaries
/
  return 0;

}
*/


//================================================================
//================================================================
//================================================================

 
 
//================================================================
  int IGE_w_obj_init () {
//================================================================
// init D- and P-lines

  int    i1;

  // init D
  for(i1=1; i1<9; ++i1) {
    IG_Da1[i1] = 0;
    IG_Da2[i1] = 0;
  }

  // init P-line
  strcpy(IGlnP, "...,");
  len_lnP = 4;

  return 0;

}

//================================================================
  int IGE_w_P_pt1 (Point *pt1) {
//================================================================

  if(!pt1) {LOG_A__ (ERR_ERR,"***** IGE_w_P_pt1 NULL"); return -1;};

  IGE_w_P_db1 (&pt1->x);
  IGE_w_P_db1 (&pt1->y);
  IGE_w_P_db1 (&pt1->z);

  return 0;

}


//================================================================
  int IGE_w_P_vc1 (Vector *vc1) {
//================================================================

  if(!vc1) {LOG_A__ (ERR_ERR,"***** IGE_w_P_vc1 NULL"); return -1;};

  IGE_w_P_db1 (&vc1->dx);
  IGE_w_P_db1 (&vc1->dy);
  IGE_w_P_db1 (&vc1->dz);
  
  return 0;

} 


//================================================================
  int IGE_w_P_db1 (double *db1) {
//================================================================
// add 1 doubles to P-line
// was IGE_w_rPP

  int    sl;
  char   *p1;


  if(!db1) {LOG_A__ (ERR_ERR,"***** IGE_w_P_db1 NULL"); return -1;};

  if((len_lnP + 100) > SIZ_lnP) goto L_err;
 
  p1 = &IGlnP[len_lnP];

  UTX_add_fl_f  (p1, *db1, 10);
  UTX_del_foll0 (p1);
  len_lnP += strlen(p1);
  strcpy(&IGlnP[len_lnP], ",");
  len_lnP += 1;

    // printf(" ex-IGE_w_P_db1 %d |%s|\n",len_lnP,IGlnP);

  return 0;


  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_P_db1 E001n");
    return -1;

}


//================================================================
  int IGE_w_P_i1 (int ii) {
//================================================================
// add int to P-line
// was IGE_w_rP1

  int    sl;
  char   *p1;


  if((len_lnP + 100) > SIZ_lnP) goto L_err;

  p1 = &IGlnP[len_lnP];

  sprintf(p1, "%d,",ii);
  len_lnP += strlen(p1);

    // printf(" ex-IGE_w_P_i1 %d |%s|\n",len_lnP,IGlnP);

  return 0;


  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_P_i1 E001");
    return -1;

}


//================================================================
  int IGE_w_P_in (int *iTab, int iNr) {
//================================================================
// add int to P-line
// was IGE_w_rP1 

  int    irc, i1;


  for(i1=0; i1<iNr; ++i1) {
      // printf("IGE_w_P_in [%d] = %d\n",i1,iTab[i1]);

    irc = IGE_w_P_i1 (iTab[i1]);
    if(irc < 0) goto L_err;
  }

  return 0;


  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_P_in E001");
    return -1;
    
}


//===================================================================
  int IGE_w_P_txt (char *txt) {
//===================================================================
// IGE_w_P_txt          exp. text (modelnames ..
// <charNr>'H',<txt>
// eg "5Habcde"

  int   ltx;
  char  *p1;


  // printf("IGE_w_P_txt |%s|\n",txt);

  ltx = strlen(txt);

  p1 = &IGlnP[len_lnP];

  sprintf(p1, "%dH%s,",ltx,txt);

  len_lnP += strlen(p1);

  return 0;

}


//================================================================
  int IGE_w_obj_out__ (cadObj__ *cadObj1) {
//================================================================
// write P-lines and D-block of ent 
// Input:
//   cadObj1.ent     entID
//   IGlnP           P-block-text
 
  // IGE_w_dump_cadObj (cadObj1, "IGE_w_obj_out__-in");


  // write D- and P-lines
  IGE_w_obj_outP (cadObj1);
  IGE_w_obj_outD (cadObj1);

  // add obj to expTab
  IGE_w_eTab_add (cadObj1);

  // init D- and P-lines
  IGE_w_obj_init ();

  return IG_D_line_nr - 1;

}


//================================================================
  int IGE_w_obj_outD (cadObj__ *cadObj1) {
//================================================================
// write D- and P-lines
// Input:
//   cadObj1.ent     entID
//   cadObj1.plNr    P-block-lineNr (IGE_w_obj_outP sets it)
//   cadObj1.trNr
// was IGE_w_D_Ln IGE_w_rP1
//     0       1       2       3       4       5       6       7       8D     9
//  1-8     9-16   17-24   25-32   33-40   41-48   49-56   57-64   65-72  74-80

static char *sBlk = "        ";
                //  12345678
  char   sln[256], s1[80];


  // printf("IGE_w_obj_out \n");


  //----------------------------------------------------------------
  // write D-line1

  // 0 = entity; eg IG_EntLN = 110
  IG_Da1[0] = cadObj1->ent;

  // 1 = P-line-nr (P74-80)
  IG_Da1[1] = IG_P_line_nr - cadObj1->plNr + 1;

  // 3 = FontPattern; 1=solid; TODO !
  IG_Da1[3] = 1;  // can be 0 for 102

  // 6 = TraMat; the D-line1-nr of TraMat 124 (for the entity to be transformed)
  IG_Da1[6] = cadObj1->trNr;

  // 8 = status      12345678
  strcpy (s1, "00000000");     // def.
	//  stat[01] = hidden-flag: 00=visible; 01=hidden;
	if(cadObj1->hid) s1[1] = '1';

	// 9
  ++IG_D_line_nr;

  //              0  1  2  3  4  5  6  7 8   9
  sprintf(sln, "%8d%8d%8d%8d%8d%8d%8d%8d%sD%7d",
          IG_Da1[0],IG_Da1[1],IG_Da1[2],IG_Da1[3],IG_Da1[4],
          IG_Da1[5],IG_Da1[6],IG_Da1[7],s1,       IG_D_line_nr);
        // 5         6         7         8         9

  fprintf(fp_o1,"%s\n",sln);

    
  //----------------------------------------------------------------
  // write D-line2
  IG_Da2[0] = cadObj1->ent;
  // 0 = entity; eg IG_EntLN = 110

  // 3 = nr of P-lines
  IG_Da2[3] = cadObj1->plNr;

  // 4 = form
  IG_Da2[4] = cadObj1->form;

  // 7 = cad-objID    57-64
  APED_oid_dbo__ (s1, cadObj1->typ, cadObj1->dbi);
  strcat(s1, "      ");
  s1[8] = '\0';

  // 8 - "0" (subscr) ? necess. for CASCADE ..

  // 9 = IG_D_line_nr
  ++IG_D_line_nr;

  //              0  1  2  3  4 5 6 7  8   9
  sprintf(sln, "%8d%8d%8d%8d%8d%s%s%s%8dD%7d",
          IG_Da2[0],IG_Da2[1],IG_Da2[2],IG_Da2[3],IG_Da2[4],
          sBlk,     sBlk,     s1,       IG_Da2[8],IG_D_line_nr);
        // 5         6         7         8         9

  fprintf(fp_o1,"%s\n",sln);


  return 0;

}


//================================================================
  int IGE_w_obj_outP (cadObj__ *cadObj1) {
//================================================================
// write P-lines
// Input:
//   cadObj1.ent     entID
//   IGlnP           P-block-text

static char *lnp =
"                                                                        P      1";

  int    irc, i1, i2, ie, ll, iPos;
  char   sln[88], s1[80], *p1, *p2;


    // printf("IGE_w_obj_out-P %d len_lnP=%d\n",cadObj1->ent,len_lnP);


  cadObj1->plNr = IG_P_line_nr;

  // replace first 3 chars with cadObj1->ent;
  sprintf(s1, "%3d", cadObj1->ent);
  strncpy(IGlnP, s1, 3);


  i1 = 0;
  ie = len_lnP - 1;
  p1 = &IGlnP[0];


  //----------------------------------------------------------------
  // fill textLines (1-64); 65 is blank
  L_nxt__:
    i2 = i1 + 64;
    if(i2 > len_lnP) i2 = len_lnP;
      // printf("IGE_w_obj_out-P1 i1=%d i2=%d\n",i1,i2);
    L_nxt1:
      --i2;
      if(i2 <= i1) {irc = 1; goto L_err;}
      if(IGlnP[i2] != ',') goto L_nxt1;

    ll = i2 - i1 + 1;
    // copy out ll chars starting at p1
    // write p1 to p1[i2]
      // printf("IGE_w_obj_out-P2 i1=%d i2=%d ll=%d |%c|\n",i1,i2,ll,p1[i2]);
    strcpy(sln, lnp);
    strncpy(sln, p1, ll);

    i1 = i2 + 1;
    if(i1 >= ie) {
      // replace last ',' by ';'
      sln[ll-1] = ';';
    }

    // set D-lineNr (a MUST for cascade. no ObjID possible)
    sprintf(s1, "%8d", IG_D_line_nr + 1);
    strncpy(&sln[64], s1, 8);

    // set P-linNr
    ++IG_P_line_nr;
    sprintf(&sln[73],"%7d", IG_P_line_nr);

      // printf("%s\n",sln);
    fprintf(fp_o2,"%s\n",sln);

    p1 = &IGlnP[i1];
    if(i1 < ie) goto L_nxt__;
    //----------------------------------------------------------------
 



  // set P-block-lineNr for Dline2/3
  cadObj1->plNr = IG_P_line_nr - cadObj1->plNr;

  return 0;


  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_obj_out E%d",irc);
    return -1;

}


//=======================================================================
   IGE_w_eTab_obj (cadObj__ *cadObj1) {
//=======================================================================
// IGE_w_eTab_obj            get obj of ialready processed cadObj__

  int     irc;  //, i1, iTbe;
  ObjGX   ox1;


  IGE_w_dump_cadObj (cadObj1, "IGE_w_eTab_obj-in");

//   // get expTab-index
//   irc = IGE_w_eTab_find (&iTbe, cadObj1->typ, cadObj1->dbi) {
//   if(irc < 0) {irc = -1; goto L_err; }

  // get CAD-obj
  ox1 = DB_GetObjGX (cadObj1->typ, cadObj1->dbi);
  if(ox1.typ == Typ_Error) { irc = -2; goto L_err; }
    DEB_dump_obj__ (Typ_ObjGX, &ox1, "IGE_w_obj__-ox1-1");

  cadObj1->obj = ox1.data;

  return 0;


  L_err:
    LOG_A__ (ERR_ERR,"***** IGE_w_eTab_obj E%d",irc);
    return -1;

}


//=======================================================================
  int IGE_w_eTab_add (cadObj__ *cadObj1) {
//=======================================================================
// store cadTyp and dbi of exported obj in expTab

  int     i1;

  // printf("IGE_w_eTab_add %d\n",expNr);

  // do not add obj if(db == 0); // CCV-objs; cannot be stored as obj;
  if(!cadObj1->dbi) return 0; 


  // die zugehoerige gCad-typ- und Index speichern
  if(expNr >= expTabSiz) {
    TX_Error("IGE_w_eTab_add E001-overflow");
    return -1;
  }

  expTab[expNr] = ExpObj_NUL;
  expTab[expNr].typ  = AP_typDB_typ(cadObj1->typ);
  expTab[expNr].ind  = cadObj1->dbi;
  expTab[expNr].iEnt = IG_D_line_nr - 1;
  expTab[expNr].clo  = cadObj1->clo;
  ++expNr;


  return 0;

}


// //================================================================
//   int IGE_w_eTab_ck_clo (int *itb, int iEnt) {
// //================================================================
// // IGE_w_eTab_ck_clo                test if iEnt is closed curve
// // returns  clo - 0=closed-curve, 1=open-curve; -1=undefined;
// 
// 
//   int    i1;
// 
//   for(i1=0; i1<expNr; ++i1) {
//     if(expTab[i1].iEnt != iEnt) continue;
//     *itb = i1;
//     return expTab[i1].clo;
//   }
// 
//   return -1;
// 
// }


//=======================================================================
  int IGE_w_eTab_find (int *iTbe, int typ, long ind) {
//=======================================================================
// find cad-obj (typ,dbi) in exported-obj-list expTab
// test if already exported
// returns -1 - not found or >= 1 for found entID

  int  iEnt = -1, btyp;
  long i1, dNr;

  // printf("IGE_w_eTab_find %d %ld\n",typ,ind);

  btyp = AP_typDB_typ (typ);

  *iTbe = 0;

  for(i1=0; i1<expNr; ++i1) {
    // printf(" ...%d %d %d\n",i1,expTab[i1].typ,expTab[i1].ind);
    if(expTab[i1].typ != btyp) continue;
    if(expTab[i1].ind != ind) continue;

    iEnt = expTab[i1].iEnt;
    *iTbe = i1;
    break;
  }

    // TESTBLOCK
    // iEnt = -1;
    // END TESTBLOCK
 
    // TESTBLOCK
    // IGE_w_dump_eTab();  // Testdisplay ausgegebene Object-ID's ..
    // printf(" ex-IGE_w_eTab_find iEnt=%d typ=%d ind=%ld\n",iEnt,typ,ind);
    // END TESTBLOCK

  return iEnt;

}


//================================================================
  int IGE_w_eTab_clr () {
//================================================================

  expNr = 0;

  return 0;

}


//=======================================================================
  int IGE_w_dump_eTab (char *inf) {
//=======================================================================

  int     i1;

  printf("========== %s IGE_w_dump_eTab %d\n",inf,expNr);

  for(i1=0; i1<expNr; ++i1) {
    printf("%d typ=%d dbi%d ent=%d clo=%d\n",i1,
           expTab[i1].typ,expTab[i1].ind,expTab[i1].iEnt,expTab[i1].clo);
  }

  return 0;

}


//================================================================
  int IGE_w_dump_cadObj (cadObj__ *cadObj1, char *inf) {
//================================================================

  printf("  %s IGE_w_dump_cadObj ------------------\n",inf);

  printf(" ent=%d typ=%d dbi=%ld oNr=%d hid=%d clo=%d ipa=%d,%d\n",
         cadObj1->ent,
         cadObj1->typ, cadObj1->dbi, cadObj1->oNr, cadObj1->hid, cadObj1->clo,
         cadObj1->ipa[0], cadObj1->ipa[1]);

  return 0;

}

//================================================================
// EOF
