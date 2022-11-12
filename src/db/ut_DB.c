//     APT-Datbase.
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
- DB_StoreXXX test if already defined ((!(DB_isFree_XXX)
  if yes: ERROR. Before set-free when going back in code (loop tru all
   DL-records beeing deleted).  ED_work_CurSet, ED_work_END, ..

- Relocate DB_CSEG vor ReAllocate (DB_allocCDAT)
  (see tess_reloc_obj tess_reloc_f_)


-----------------------------------------------------
Modifications:
2009-09-05  DB_StoreCvCCV new; DB_store_obj: support for ObjGX added. RF.
2005-09-21  dynam. Planes neu. RF.
2005-09-19  DB_DefRef neu. RF.
2003-12-29  rewrite m. realloc. RF.
2002-07-20  DB_StoreSur. RF.
2002-05-20  DB_StoreVector: DX DY DZ zu. RF.
2002-05-20  DB_StoreCurv von 2D-Curve -> 3D-Curve. RF.
2001-10-26  DB_FindVector neu.
2001-10-19  DB_QueryNxtFree: Refsys zu.
2001-05-04  DB_Lay* -> ut_DL; DB_Delete deaktiviert; bei calls von DB_Init u
            DB_Get_GR_Obj Aenderungen siehe unten.
2000-10-16  Die nun neu in ut_DL befindl. Routinen ausgelagert.

-----------------------------------------------------
*/
#ifdef globTag
void DB(){}
#endif
/*!
\file  ../db/ut_DB.c
\brief database
\code
=====================================================
List_functions_start:

DB_fix_tol
DB_Stat            display statistic
DB_dump__          dump complete DB
DB_dump_dyn__      dump all dynamic objects of type<typ>
DB_dump_f          dump all objects of type<typ> into open file
DB_dump_Activ      dump all activities
DB_dump_dbo        DEB_dump_dbo   dump DB-obj

DB_save__          gesamte DB -> Datei raus
DB_load__          gesamte DB aus Datei einlesen
DB_dyn__           get/(re)set dynamic space for <typ>

DB_GetVar          get var ((double)"V")
DB_get_Var         get *double ((double)"V")
DB_get_PT          get *Point
DB_GetPoint        get Point
DB_get_LN          get *Line
DB_GetLine         get Line
DB_get_VC          get *Vector
DB_GetVector       get Vector
DB_get_CI          get *Circ
DB_GetCirc         get Circ
DB_get_PLN         get *Plane
DB_GetRef          get Plane (RefSys)
DB_DefRef          das Defaultrefsys (RX/RY/RZ) liefern
DB_get_typ_cv      get form of Curve
DB_get_typ_note    get form of note/text/tag/image
DB_get_CV          get form and struct of Curve ((ObjGX*)"S")
DB_GetCurv         get *Curve ((ObjGX*)"S",cv_tab/cv_dyn)
DB_GetGTxt         get *Text ((ObjGX*)"N")
DB_get_Text        get text only (GText - stored with "N#="txt")
DB_GetSur          get Surf ((ObjGX*)"A")
DB_GetSol          get ? ((ObjGX*),so_tab)
DB_GetTra          get Transformation ((ObjGX*)"T")
DB_GetTool         get Tool ((BTool*),"?",tl_tab)
DB_get_Activ

DB_GetObjGX        get obj  (ObjGX)
UTO__dbo           get data-struct from typ, DB-index

DB_store_ox       call DB_StoreXX with ObjGX
DB_store_obj      call DB_StoreXX with structTyp,struct
DB_StoreVar
DB_StoreVector
DB_StorePoint
DB_StoreDynPoint   store/overwrite dynamic point
DB_StoreLine
DB_StoreCirc
DB_StoreCvEll
DB_StoreCvPlg
DB_StoreCvPl2
DB_StoreClot
DB_StoreCvBsp
DB_StoreObjGX
DB_StoreCvCCV
DB_StoreCurv
DB_StoreDim_
DB_StoreATxt
DB_StoreGTxt
DB_StoreImg        store BMP-Bitmap (AText)
// DB_StoreTex        store Texture
DB_StoreRef        store Plane "R" (RefSys)
DB_StoreSur        su_tab
DB_StoreSol        so_tab
DB_StoreTra        tra_tab
DB_StoreTool       tl_tab
DB_StoreActiv      save Activity
DB_StoreJointNr    report last used jointNr
DB_StoreDummy

DB_StoreModel      store subModel-Reference (Ditto); mdr_tab
DB_get_ModRef      get referenceModel from Index
DB_dump_ModRef     dump refModels
DB_get_mdr         returns referenceModels
DB_del_ModRef      delete refModel and all following
DB_del_MR__        delete refModel
DB_del_Mod         delete all refModels and all basicModels

DB_StoreModBas     store basicModel; mdb_dyn
DB_get_mdb         returns basicModels
DB_get_ModBas      get subModel from Index
DB_get_iModBas_iModRef  get basic-ModelNr from the ReferenceModelNr
DB_get_ModBasNr    get nr of defined subModels
DB_del_ModBas      delete baseModel and all following
DB_clear_ModBas    clear (reset) single basicModel
DB_del_mdb_syFn    delete baseModel from symbolic-name
DB_mdlNam_iBas     get the name of a basic-model from its basicModelNr
DB_mdlNam_iRef     get Modelname from DB-index of the Modelreference
DB_mdlTyp_iBas     get Modeltyp from its basicModelNr
DB_mdlTyp_iRef     get Modeltyp from DB-index of the Modelreference
DB_dump_ModBas     dump baseModels

DB_Store_hdr_cv    Curve-Header speichern
DB_Store_hdr_su    Surf.-Header speichern
DB_Store_hdr_nt    Note-Header speichern

DB_Filter__        get list of filtered DB-objects
DB_FindVector
DB_QueryPoint      search Point in DB
DB_QueryLine
DB_QueryCirc
DB_QueryCurv
DB_QueryActiv
DB_GetConnObj
DB_GetObjTyp2Pt

DB_CkRef           check if Plane[Ind] is defined.
DB_CheckInd        unused
DB_Get_GR_Obj      read &transform obj ???
DB_GetDynInd       get next dyn. Index
DB_get_ObjNr       get nr of defined objects of given type
DB_QueryDef        check if obj is defined in DB;

DB_dbo_get_free    get next free index
DB_QueryNxtFree    get next free index from typ,startIndex
DB_QueryNxtUsed    get next used index
DB_QueryPrvUsed    get previous used index
DB_Query__         get dataTable, lastIndex, tabSiz from typ

DB_isFree_Var
DB_isFree_PT
DB_isFree_LN
DB_isFree_VC
DB_isFree_CI
DB_isFree_PLN
DB_isFree_CV
DB_isFree_GTxt
DB_isFree_Sur
DB_isFree_Sol
DB_isFree_Tra
DB_isFree_Act
DB_isFree_ModRef

DB_Error
DB_Delete          delete obj in DB (set Free)
DB_setFree_Var     kill DB-obj; other objects DB_setFree_..

DB_set_state       save/restore all saved DB-Pointers (kill newer objects)
DB_CSEG__          reset DB_CSEG after use for temporary objects.
DB_CSEG_ck         testen, ob noch mind 25 K in CDAT frei sind;
DB_cPos
DB_cSav
DB_cGet
DB_Query_siz

DB_Init
DB_allocCDAT
DB_allocVAR
DB_allocDynVAR
DB_allocPoint
DB_allocDynPoint
DB_allocVector
DB_allocDynVector
DB_allocLine
DB_allocDynLine
DB_allocCirc
DB_allocDynCirc
DB_allocCurve
DB_allocDynCurve
DB_allocNote
DB_allocDynNote
DB_allocRef
DB_allocModRef
DB_allocModNam
DB_allocModBas
DB_allocTool
DB_allocTra
DB_allocSur
DB_allocSol

List_functions_end:
=====================================================
UNUSED:
DB_allocModNod
DB_StoreModNod     Model-Node (basicModelIndex+parentbasicModelIndex+DB-index)
DB_get_ModNod
DB_dump_ModNod
DB_ModBas_seqMax   get highest seqNr
DB_list_ModBas     Modelnames in korrekter Reihenfolge -> Datei
DB_ModNod_ckLoop_  check for call itsef
DB_ModNod_ckLoop1


\endcode *//*----------------------------------------
see also UTO_obj_dbo



Hier sind die Speicher für die Punkte, Lines, Arcs ...
Man kann die Koord. Speichern (DB_StoreVar) und abholen (DB_GetPoint).

Es gibt Speicher für die APT-Variablen (von Index 0 bis APT_VR_SIZ,
 einen Index auf das naechste freie Obj gibt es nicht !

DB-obj is DB-typ (typ) and DB-index (dbi)
  typ: eg Typ_PT - see INF_obj_types
  dbi: index in DB for given typ;    index 0 is not valid;
       permanent objects have positive index 
       dynamic objects have negative index 

-------------------------------
In Version vor 2001-05-04 erforderliche Korrekturen:

 nach Aufruf DB_Init auch
  // Displist Init
  DL_Init ();

  DL_InitAttTab ();

  // Auch die DispList loeschen
  GR_Init1 ();

 ----------------
 DB_Get_GR_Obj-Interf geaendert:
  int      tra_ind, apt_typ;
  long     apt_ind;

  apt_typ = DL_dbTyp__dli(gr_ind);
  apt_ind = DL_get_dbi(gr_ind);
  tra_ind = DL_GetTrInd(gr_ind);

  .. DB_Get_GR_Obj (..o, apt_typ, apt_ind, tra_ind);

-------------------------------
nach DB_Delete erforderl:
    GL_Delete (ind);





--------------------------------------
Memory-probleme:


beim realloc von
DB_allocModNam                    DB_MNM_INC
DB_allocCDAT                      DB_CINC
 muss man von vorn beginnen !!!!!



..........................   typ       ptr
DB_allocCurve  S   cv_tab    ObjGX     data
DB_allocNote   N   tx_tab    ObjGX     data
DB_allocSur    A   su_tab    ObjGX     data
DB_allocSol    B   so_tab    ObjGX     data
  data zeigt auf die Curve-struct, meist im CDAT.
  Kein Problem, solang CDAT steht !



..........................   typ       ptr
DB_allocModRef     mdr_tab   ModelRef  mnam  APT_MR_INC
DB_allocModBas     mdb_dyn   ModelBas  mnam  DYN_MB_INC
  // mnam zeigt auf Modelname; ev in mdb_nam ?

// DB_allocModNam     mdb_nam   char            DB_MNM_INC
//   Modelnames ?; wechseln die posi -
//   muessten beim Vater korrigiert werden !!!
//   also alle mnam von mdr_tab u mdb_dyn korrigieren ?
//   Es stimmen nun aber auch alle Kopien non mnam nicht mehr !





======================================================================
*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>                         /* fabs  ...   */
#include <stdio.h>                        // FILE ..
#include <stdlib.h>
#include <string.h>                       /* strcmp ..   */



#include "../ut/ut_geo.h"              //
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_obj.h"              // UTO_obj_save
#include "../ut/ut_txt.h"              //
#include "../ut/ut_TX.h"
#include "../ut/ut_ox_base.h"          // OGX_SET_INDEX
#include "../ut/ut_os.h"               // OS_ ..
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../ut/ut_gtypes.h"           // AP_src_typ__
#include "../ut/ut_deb.h"              // DEB_*

#include "../gr/ut_gr.h"               //
#include "../gr/ut_DL.h"




#include "../db/ut_DB.h"



//============ Externe Var: =======================================
// EXTERNALS:
// from ../xa/xa.c:

extern double     AP_txsiz;             ///< Notes-Defaultsize
extern double     AP_txdimsiz;          ///< Dimensions-Text-size
extern int        AP_txNkNr;            ///< Nachkommastellen
extern ColRGB     AP_defcol;

extern Point AP_box_pm1, AP_box_pm2;  // box around mainmodel (without subModels)

extern int       WC_sur_ind;            ///< Index auf die ActiveConstrPlane
extern Plane     WC_sur_act;            ///< die aktive Plane
extern Mat_4x3   WC_sur_mat;            ///< TrMat of ActiveConstrPlane
extern Mat_4x3   WC_sur_imat;           ///< inverse TrMat of ActiveConstrPlane


// from ../xa/xa_ga.c:
extern ObjAtt  *GA_ObjTab;        // die PermanentAttributeTable
extern int     GA_recNr;          // die aktuelle Anzahl von Records

// ex ../xa/xa_ed.c
extern int       ED_lnr_act;

// from ../gr/ut_gtx.c:
extern double GR_tx_scale;

// from ../ci/NC_Main.c:
extern double APT_ModSiz;
// extern double APT_ln_len;

// ../ut/ut_txfil.c
extern char       *UTF_FilBuf0;         // memspc
extern long       UTF_FilBuf0Len;       // size of UTF_FilBuf0 (memspc)


//============ Lokale Var: =======================================

typedef struct {int typ; void *data;}       DB_obj;       


#define APT_VR_INC       200
#define APT_PT_INC       5000
#define APT_VC_INC       2000
#define APT_LN_INC       5000
#define APT_CI_INC       2000
#define APT_PL_INC       500
#define APT_MR_INC       200
#define APT_TL_INC       200
#define APT_TR_INC       200
#define APT_CV_INC       2000
#define APT_TX_INC       2000
#define APT_SU_INC       2000
#define APT_SO_INC       2000
#define APT_AC_INC       50
#define DB_CINC          2000000    ///< memsiz for DB_CDAT (curves .. 2000000
// #define DB_MNM_INC       5000       ///< memsiz for mdb_nam (DB_allocModNam)


long APT_VR_SIZ, APT_VR_IND=0;   ///< APT_.._IND: last occupied Index
long APT_PT_SIZ, APT_PT_IND=0;
long APT_VC_SIZ, APT_VC_IND=0;
long APT_LN_SIZ, APT_LN_IND=0;
long APT_CI_SIZ, APT_CI_IND=0;
long APT_PL_SIZ, APT_PL_IND=0;
long APT_MR_SIZ, APT_MR_IND=-1;
// long APT_TL_SIZ, APT_TL_IND=0;
long APT_TR_SIZ, APT_TR_IND=0;
long APT_CV_SIZ, APT_CV_IND=0;
long APT_TX_SIZ, APT_TX_IND=0;
long APT_SU_SIZ, APT_SU_IND=0;
long APT_SO_SIZ, APT_SO_IND=0;
long APT_AC_SIZ, APT_AC_IND=0;
long DB_MNM_SIZ, APT_MN_IND=0;
long DB_CSIZ;


#define DYN_VR_INC       100
#define DYN_PT_INC       5000
#define DYN_VC_INC       1000
#define DYN_LN_INC       1000
#define DYN_CI_INC       1000
#define DYN_PL_INC       500
#define DYN_CV_INC       1000
#define DYN_SU_INC       1000
#define DYN_TX_INC       1000
#define DYN_MB_INC       100


static long DYN_VR_SIZ=0, DYN_VR_IND=0;
static long DYN_PT_SIZ=0, DYN_PT_IND=0;
static long DYN_VC_SIZ=0, DYN_VC_IND=0;
static long DYN_LN_SIZ=0, DYN_LN_IND=0;
static long DYN_CI_SIZ=0, DYN_CI_IND=0;
static long DYN_PL_SIZ=0, DYN_PL_IND=0;
static long DYN_CV_SIZ=0, DYN_CV_IND=0;
static long DYN_SU_SIZ=0, DYN_SU_IND=0;
static long DYN_TX_SIZ=0, DYN_TX_IND=0;
static long DYN_MB_SIZ=0, DYN_MB_IND=0;
// static long               DB_MNM_IND=0;
static int                DB_JNT_IND=0;



static double     *vr_tab = NULL;
static double     *vr_dyn = NULL;

static Point      *pt_tab = NULL;
static Point      *pt_dyn = NULL;

static Vector     *vc_tab = NULL;
static Vector     *vc_dyn = NULL;

static Line       *ln_tab = NULL;
static Line       *ln_dyn = NULL;

static Circ       *ci_tab = NULL;
static Circ       *ci_dyn = NULL;

static ObjGX      *tx_tab  = NULL;
static ObjGX      *tx_dyn  = NULL;

static Plane      *pln_tab = NULL;
static Plane      *pln_dyn = NULL;

static ModelRef   *mdr_tab = NULL;
static ModelBas   *mdb_dyn = NULL;

static ObjGX      *cv_tab  = NULL;
static ObjGX      *cv_dyn  = NULL;

static ObjGX      *su_tab  = NULL;
static ObjGX      *su_dyn  = NULL;

// static char       *mdb_nam = NULL;
static Activity   *ac_tab  = NULL;
static ObjGX      *tra_tab = NULL;
static ObjGX      *so_tab  = NULL;

static Memspc     DB_CSEG;
static char       *DB_CDAT = NULL;


// static ModelNode  *mNod = NULL;
// static int        MNOD_SIZ = 0, MNOD_IND = 0;
// #define MNOD_INC             100


static double     DB_sel_tol = 2.5;
static double     DB_sel_tol_fakt = 2.5;


static Point      DB_pt0;


static ModelBas _MODELBAS_NUL = {NULL, {0., 0., 0.}, {FLT_32_MAX, 0., 0.},
             -1L, -1,
             0, -1};
// {FLT_32_MAX, 0., 0.} is empty point.


//================================================================
  int DB_test__ () {
//================================================================

  DEB_dump_obj__ (Typ_VC, &vc_dyn[1], "vc_dyn[1]:");

  return 0;

}


/***********************************************************************/
  void   DB_fix_tol     (double scale ) {
/***********************************************************************/


  DB_sel_tol = DB_sel_tol_fakt / scale;

  //TX_Print("DB_fix_tol %f %f",DB_sel_tol,scale);

}


//================================================================
  int DB_dump_stat () {
//================================================================

  printf("DB_dump_stat ===========================================\n");

  printf("  Var's  : %ld\n",APT_VR_SIZ);
  printf("  Points : %ld\n",APT_PT_SIZ);
  printf("  Vec's  : %ld\n",APT_VC_SIZ);
  printf("  Lines  : %ld\n",APT_LN_SIZ);
  printf("  Circ's : %ld\n",APT_CI_SIZ);
  printf("  Curves : %ld\n",APT_CV_SIZ);
  printf("  Planes : %ld\n",APT_PL_SIZ);
  printf("  Surf's : %ld\n",APT_SU_SIZ);
  printf("  Activ's: %ld\n",APT_AC_SIZ);

  printf("  dyn-var's  : %ld / %ld\n",DYN_VR_IND,DYN_VR_SIZ);
  printf("  dyn-points : %ld / %ld\n",DYN_PT_IND,DYN_PT_SIZ);
  printf("  dyn-lines  : %ld / %ld\n",DYN_LN_IND,DYN_LN_SIZ);
  printf("  dyn-circ's : %ld / %ld\n",DYN_CI_IND,DYN_CI_SIZ);
  printf("  dyn-curves : %ld / %ld\n",DYN_CV_IND,DYN_CV_SIZ);
  printf("  dyn-Planes : %ld / %ld\n",DYN_PL_IND,DYN_PL_SIZ);

  printf("  Bas-Models : %ld / %ld\n",DYN_MB_IND,DYN_MB_SIZ);
  printf("  Ref-Models : %ld\n",APT_MR_SIZ);

  return 0;

}

 
//********************************************************************
  void   DB_Stat     ( ) {
//********************************************************************


  long l1, l2;



  TX_Print("DB_Stat:");

  TX_Print("  dyn-Punkte : %d / %d",DYN_PT_IND,DYN_PT_SIZ);
  TX_Print("  dyn-Linien : %d / %d",DYN_LN_IND,DYN_LN_SIZ);
  TX_Print("  dyn-Kreise : %d / %d",DYN_CI_IND,DYN_CI_SIZ);
  TX_Print("  dyn-Planes : %d / %d",DYN_PL_IND,DYN_PL_SIZ);

  TX_Print("  Bas-Models : %d / %d",DYN_MB_IND,DYN_MB_SIZ);
  TX_Print("  Ref-Models : %d",APT_MR_SIZ);


  //TX_Print("  APT-Punkte : %d",apt_pt_siz);
  //l1 = DB_QueryNxtFree (Typ_PT, WC_ind_p+1) - 1;
  // l2=0;
  // for(l1=0; l1<apt_pt_siz; ++l1) {
    // if(pt_tab[l1].x != UT_VAL_MAX) ++l2;
  // }
  // TX_Print("  APT-Punkte : %d von %d",l2,apt_pt_siz);
  TX_Print("  APT-Punkte : %d",APT_PT_SIZ);


  //TX_Print("  APT-Linien : %d",apt_ln_siz);
  //l1 = DB_QueryNxtFree (Typ_LN, WC_ind_l+1) - 1;
  // l2=0;
  // for(l1=0; l1<apt_ln_siz; ++l1) {
    // if(ln_tab[l1].p1.x != UT_VAL_MAX) ++l2;
  // }
  TX_Print("  APT-Linien : %d",APT_LN_SIZ);



  //TX_Print("  APT-Kreise : %d",apt_ci_siz);
  //l1 = DB_QueryNxtFree (Typ_CI, WC_ind_c+1) - 1;
  // l2=0;
  // for(l1=0; l1<apt_ci_siz; ++l1) {
    // if(ci_tab[l1].p1.x != UT_VAL_MAX) ++l2;
  // }
  TX_Print("  APT-Kreise : %d",APT_CI_SIZ);
  TX_Print("  APT-Planes : %d",APT_PL_SIZ);
  TX_Print("  APT-Surf.s : %d",APT_SU_SIZ);


  // ED_query_fsiz (&l1, &l2);
  // TX_Print(" Dateigrösse : %d von %d Byte",l1,l2);


}


//================================================================
  int DB_size_set (long *lTab) {
//================================================================

  int   i1;
  long  iAll, sizTot;


  printf("DB_size_set %ld %ld %ld %ld %ld %ld %ld %ld\n",
         lTab[0],lTab[1],lTab[2],lTab[3],
         lTab[4],lTab[5],lTab[6],lTab[7]);


  if(lTab[0] > APT_CV_SIZ) DB_allocCurve  (lTab[0]);
  if(lTab[1] > APT_SU_SIZ) DB_allocSur    (lTab[1]);
  if(lTab[2] > APT_SO_SIZ) DB_allocSol    (lTab[2]);
  if(lTab[3] > APT_TX_SIZ) DB_allocNote   (lTab[3]);
  if(lTab[4] > APT_CI_SIZ) DB_allocCirc   (lTab[4]);
  if(lTab[5] > APT_LN_SIZ) DB_allocLine   (lTab[5]);
  if(lTab[6] > APT_PT_SIZ) DB_allocPoint  (lTab[6]);
  if(lTab[7] > APT_MR_SIZ) DB_allocModRef (lTab[7]);


  // iall = nr of total objects
  sizTot = 0;
  iAll = 0;
  for(i1=0; i1<8; ++i1) iAll += lTab[i1];
  DL_alloc__ (iAll);
//   GL_alloc__ (iAll);



  // Anzahl DynCurves = (AnzahlCurves / 2)  + AnzahlSurfs
  sizTot = (lTab[0] / 2)  + lTab[1];
  if(sizTot > DYN_CV_SIZ) DB_allocDynCurve (sizTot);


  // Berechnung Memspc fuer CDAT:
  // ein K pro Curve,  0.1 K pro surf, sol, txt.
  sizTot = (lTab[0] *  800) +            // APT_CV_SIZ     def 7000
           (lTab[1] *  124) +            // APT_SU_SIZ
           (lTab[2] *   64) +            // APT_SO_SIZ
           (lTab[3] *   64);             // APT_TX_SIZ


  //TEST ONLY:
  // sizTot += (iAll * 100);
    // printf(" sizTot=%ld iAll=%ld\n",sizTot,iAll);

  DB_allocCDAT (sizTot);



  // printf(" APT_CV_SIZ=%d\n",APT_CV_SIZ);
  // printf(" APT_SU_SIZ=%d\n",APT_SU_SIZ);
  // printf(" APT_SO_SIZ=%d\n",APT_SO_SIZ);
  // printf(" APT_TX_SIZ=%d\n",APT_TX_SIZ);

  // printf(" DB_CSIZ=%d\n",DB_CSIZ);



  return 0;

}
 

//================================================================
   void DB_Init (int mode) {
//================================================================
/// mode=0: init all (new)
/// mode=1: init DB and dyn-objects; not baseModels
/// mode=2: init dyn-objects only


  long i1;


  // printf ("-----------------------DB_Init %d -----------\n",mode);

  if(mode == 0) {
    DYN_MB_IND = 0;          // clear all baseModels
    MDL_reset (0);            // reset loadmap and mdl_tab
    // APT_MR_IND=-1;
    goto L_DB;
  }


  if(mode == 1) {
    MDL_reset (1);            // reset loadmap and mdl_tab
    goto L_DB;
  }

  if(mode == 2) goto L_dyn;



  //----------------------------------------------------------------
  // reset DB and dyn-objects
  L_DB:

    DB_allocCDAT (0);
    DB_allocVAR  (0);
    DB_allocPoint (0);
    DB_allocVector (0);
    DB_allocLine (0); 
    DB_allocCirc (0);
    DB_allocRef (0);
    DB_allocModRef (0);         // clear all refModels
    // DB_allocTool (0);
    DB_allocTra  (0);
    DB_allocCurve (0);
    DB_allocNote (0);
    DB_allocSur (0);
    DB_allocSol (0);
    DB_allocAct (0);
  

  //----------------------------------------------------------------
  // reset dyn-objects
  L_dyn:

  DB_allocDynVAR (0);
  DB_allocDynPoint (0);
  DB_allocDynVector (0);
  DB_allocDynLine (0);
  DB_allocDynCirc (0);
  DB_allocDynPlane (0);
  DB_allocDynCurve (0);
  DB_allocDynSur   (0);
  DB_allocDynNote (0);


  // store Defaultvektoren
  vc_dyn[-DB_VCX_IND] = UT3D_VECTOR_X;
  vc_dyn[-DB_VCY_IND] = UT3D_VECTOR_Y;
  vc_dyn[-DB_VCZ_IND] = UT3D_VECTOR_Z;
  vc_dyn[-DB_VCIX_IND] = UT3D_VECTOR_IX;
  vc_dyn[-DB_VCIY_IND] = UT3D_VECTOR_IY;
  vc_dyn[-DB_VCIZ_IND] = UT3D_VECTOR_IZ;
  DYN_VC_IND = 6;
    // printf(" DB_Init DYN_VC_IND=%ld\n",DYN_VC_IND);
    // DEB_dump_obj__ (Typ_VC, &vc_dyn[1], "vc_dyn[1]:");
  


  // create DefaultPlanes --> pln_dyn
  DB_DefRef (0L);
  // printf(" nach DB_DefRef\n");



  // mode=1: alles ausser basicModels initialisieren
  if(mode > 0) return;

  DB_JNT_IND = 0;   // reset last used jointNr



  vr_tab[0] = 0.; // f. UI_disp_vec2


  // reset den Name-Buffer in der DB
  // DB_StoreModBas (0, NULL);


  // dyn_vr_ind = apt_vr_siz-1;
  // dyn_vc_ind = apt_vc_siz-1;
  // dyn_ln_ind = apt_ln_siz-1;
  // dyn_ci_ind = apt_ci_siz-1;
  // dyn_cv_ind = apt_cv_siz-1;
  // dyn_tx_ind = apt_tx_siz-1;
 


  DB_pt0.x = 0.0;
  DB_pt0.y = 0.0;

  // DB_vc0.dx = 0.0;
  // DB_vc0.dy = 0.0;


  // str_tab[0] = '\0';
  // str_ind    = 0;


/*
  // Displist Init
  DL_Init ();


  DL_InitAttTab ();


  // Auch die DispList löschen
  GR_Init1 ();
*/

  // printf("ex DB_Init\n");


}


//=======================================================================
  int DB_save__ (char *mNam) {
//=======================================================================
/// gesamte DB -> Datei raus
// die Felder muessen schon die erfolderliche Groesse haben; sonst waere
// nach dem einlesen von _SIZ noch ein alloc erforderlich !
// Do not save mdb_dyn,mdb_nam


  int    i1;
  long   fSiz;
  char   fnam[256], s1[128];
  FILE   *fp1;



  // printf("------------------ DB_save__ |%s| ------------------------\n",mNam);
  // printf(" _save__-GA_recNr=%d\n",GA_recNr);



  strcpy(s1, mNam);
  // change all '/' of mNam into '_' - else no correct filename possible
  UTX_safeName (s1, 2);

  sprintf(fnam, "%sDB__%s.dat",AP_get_tmp_dir(),s1);
    // printf("DB_save__ |%s|\n",fnam);


  if((fp1=fopen(fnam,"wb")) == NULL) {
    TX_Print("DB_save__ E001-%s\n",fnam);
    return -1;
  }


  fwrite(&APT_ModSiz, sizeof(double), 1, fp1);
  fwrite(&UT_TOL_pt, sizeof(double), 1, fp1);
  fwrite(&UT_TOL_ln, sizeof(double), 1, fp1);
  fwrite(&UT_TOL_cv, sizeof(double), 1, fp1);
  fwrite(&UT_DISP_cv, sizeof(double), 1, fp1);
  fwrite(&AP_txdimsiz, sizeof(double), 1, fp1);
  fwrite(&AP_txsiz, sizeof(double), 1, fp1);
  fwrite(&GR_tx_scale, sizeof(double), 1, fp1);
  fwrite(&AP_txNkNr, sizeof(int), 1, fp1);
  fwrite(&AP_defcol, sizeof(ColRGB), 1, fp1);
  fwrite(&ED_lnr_act, sizeof(int), 1, fp1);

  fwrite(&DB_JNT_IND, sizeof(int), 1, fp1);    // 2017-04-15

  fwrite(&WC_sur_ind, sizeof(int), 1, fp1);
  fwrite(&WC_sur_act, sizeof(Plane), 1, fp1);
  fwrite(&WC_sur_mat, sizeof(Mat_4x3), 1, fp1);
  fwrite(&WC_sur_imat, sizeof(Mat_4x3), 1, fp1);

  i1 = NC_getGrafAtt ();
  fwrite(&i1, sizeof(int), 1, fp1);


  // printf(" DB_sav: %d %d %d %d %d %d\n",APT_VR_IND,APT_PT_IND,
                   // APT_VC_IND,APT_LN_IND,APT_CI_IND,APT_PL_IND);

  fwrite(&APT_VR_IND, sizeof(long), 1, fp1);
  if(APT_VR_IND > 0)
  fwrite(vr_tab, sizeof(double), APT_VR_IND+1, fp1);

  fwrite(&APT_PT_IND, sizeof(long), 1, fp1);
  if(APT_PT_IND > 0)
  fwrite(pt_tab, sizeof(Point), APT_PT_IND+1, fp1);

  fwrite(&APT_VC_IND, sizeof(long), 1, fp1);
  if(APT_VC_IND > 0)
  fwrite(vc_tab, sizeof(Vector), APT_VC_IND+1, fp1);

    // printf(" APT_LN_IND=%ld APT_LN_SIZ=%ld\n",APT_LN_IND,APT_LN_SIZ);
  fwrite(&APT_LN_IND, sizeof(long), 1, fp1);
  if(APT_LN_IND > 0)
  fwrite(ln_tab, sizeof(Line), APT_LN_IND+1, fp1);

  fwrite(&APT_CI_IND, sizeof(long), 1, fp1);
  if(APT_CI_IND > 0)
  fwrite(ci_tab, sizeof(Circ), APT_CI_IND+1, fp1);

  fwrite(&APT_PL_IND, sizeof(long), 1, fp1);
  if(APT_PL_IND > 0)
  fwrite(pln_tab, sizeof(Plane), APT_PL_IND+1, fp1);

  fwrite(&APT_MR_IND, sizeof(long), 1, fp1);
  if(APT_MR_IND > 0)
  fwrite(mdr_tab, sizeof(ModelRef), APT_MR_IND+1, fp1);

  fwrite(&APT_AC_IND, sizeof(long), 1, fp1);
  if(APT_AC_IND > 0)
  fwrite(ac_tab, sizeof(Activity), APT_AC_IND+1, fp1);

  fwrite(&APT_TR_IND, sizeof(long), 1, fp1);
  if(APT_TR_IND > 0)
  fwrite(tra_tab, sizeof(ObjGX), APT_TR_IND+1, fp1);

  fwrite(&APT_CV_IND, sizeof(long), 1, fp1);
  if(APT_CV_IND > 0)
  fwrite(cv_tab, sizeof(ObjGX), APT_CV_IND+1, fp1);

  fwrite(&APT_TX_IND, sizeof(long), 1, fp1);
  if(APT_TX_IND > 0)
  fwrite(tx_tab, sizeof(ObjGX), APT_TX_IND+1, fp1);

  fwrite(&APT_SU_IND, sizeof(long), 1, fp1);
  if(APT_SU_IND > 0)
  fwrite(su_tab, sizeof(ObjGX), APT_SU_IND+1, fp1);

  fwrite(&APT_SO_IND, sizeof(long), 1, fp1);
  if(APT_SO_IND > 0)
  fwrite(so_tab, sizeof(ObjGX), APT_SO_IND+1, fp1);


  DB_Query_siz (&fSiz);
  fwrite(&fSiz, sizeof(long), 1, fp1);
  fwrite(DB_CDAT, fSiz, 1, fp1);

  fwrite(&DB_CSEG, sizeof(Memspc), 1, fp1);



  fwrite(&DYN_VR_IND, sizeof(long), 1, fp1);
  if(DYN_VR_IND > 0)
  fwrite(vr_dyn, sizeof(double), DYN_VR_IND+1, fp1);

  fwrite(&DYN_PT_IND, sizeof(long), 1, fp1);
  if(DYN_PT_IND > 0)
  fwrite(pt_dyn, sizeof(Point), DYN_PT_IND+1, fp1);

    // printf(" DB_save__ DYN_VC_IND=%ld\n",DYN_VC_IND);
  fwrite(&DYN_VC_IND, sizeof(long), 1, fp1);
  if(DYN_VC_IND > 0)
  fwrite(vc_dyn, sizeof(Vector), DYN_VC_IND+1, fp1);

  fwrite(&DYN_LN_IND, sizeof(long), 1, fp1);
  if(DYN_LN_IND > 0)
  fwrite(ln_dyn, sizeof(Line), DYN_LN_IND+1, fp1);

  fwrite(&DYN_CI_IND, sizeof(long), 1, fp1);
  if(DYN_CI_IND > 0)
  fwrite(ci_dyn, sizeof(Circ), DYN_CI_IND+1, fp1);

  fwrite(&DYN_PL_IND, sizeof(long), 1, fp1);
  if(DYN_PL_IND > 0)
  fwrite(pln_dyn, sizeof(Plane), DYN_PL_IND+1, fp1);

  fwrite(&DYN_CV_IND, sizeof(long), 1, fp1);
  if(DYN_CV_IND > 0)
  fwrite(cv_dyn, sizeof(ObjGX), DYN_CV_IND+1, fp1);

  fwrite(&DYN_SU_IND, sizeof(long), 1, fp1);
  if(DYN_SU_IND > 0)
  fwrite(su_dyn, sizeof(ObjGX), DYN_SU_IND+1, fp1);

  fwrite(&DYN_TX_IND, sizeof(long), 1, fp1);
  if(DYN_TX_IND > 0)
  fwrite(tx_dyn, sizeof(ObjGX), DYN_TX_IND+1, fp1);

  fwrite(&GA_recNr, sizeof(int),  1, fp1);
  if(GA_ObjTab > 0)
  fwrite(GA_ObjTab, sizeof(ObjAtt), GA_recNr, fp1);

  // save UTF_FilBuf0 (UTF_save__)
  fwrite(&UTF_FilBuf0Len, sizeof(long),  1, fp1);
  if(UTF_FilBuf0Len > 0)
  fwrite(UTF_FilBuf0, UTF_FilBuf0Len, 1, fp1);

  // save AP_box_pm1,2 AP_stat-bits mdl_modified and mdl_box_valid
  AP_stat_file (1, fp1);

  // OPAR_file (1, fp1);    // write ParentTable

  fclose(fp1);


  //----------------------------------------------------------------
    // printf("ex DB_save__ %f\n",AP_txdimsiz);

  return 0;

}


//=======================================================================
  int DB_load__ (char *mNam) {
//=======================================================================
// DB_load__                  load DB for model mNam
// Input:
//   mNam          modelname - safe

  int  i1;
  long fSiz, l1;
  char fnam[256], s1[128];
  FILE *fp1;


  strcpy(s1, mNam);
  // change all '/' of mNam into '_' - else no correct filename possible
  UTX_safeName (s1, 2);


  sprintf(fnam, "%sDB__%s.dat",AP_get_tmp_dir(),mNam);

  // printf("----------------- DB_load__ |%s|\n",fnam);


  if((fp1=fopen(fnam,"rb")) == NULL) {
    TX_Print("DB_load__ E001 |%s|\n",fnam);
    return -1;
  }

  fread(&APT_ModSiz, sizeof(double), 1, fp1);
  fread(&UT_TOL_pt, sizeof(double), 1, fp1);
  fread(&UT_TOL_ln, sizeof(double), 1, fp1);
  fread(&UT_TOL_cv, sizeof(double), 1, fp1);
  fread(&UT_DISP_cv, sizeof(double), 1, fp1);
  fread(&AP_txdimsiz, sizeof(double), 1, fp1);
  fread(&AP_txsiz, sizeof(double), 1, fp1);
  fread(&GR_tx_scale, sizeof(double), 1, fp1);
  fread(&AP_txNkNr, sizeof(int), 1, fp1);
  fread(&AP_defcol, sizeof(ColRGB), 1, fp1);
  fread(&ED_lnr_act, sizeof(int), 1, fp1);

  fread(&DB_JNT_IND, sizeof(int), 1, fp1);

  // fread(&i1, sizeof(int), 1, fp1);
    // printf(" DB_load__ lnr=%d\n",i1);
  // ED_set_lnr_act (i1);
  // AP_lNr_set (i1);

  fread(&WC_sur_ind, sizeof(int), 1, fp1);
  fread(&WC_sur_act, sizeof(Plane), 1, fp1);
  fread(&WC_sur_mat, sizeof(Mat_4x3), 1, fp1);
  fread(&WC_sur_imat, sizeof(Mat_4x3), 1, fp1);
  // GL_SetConstrPln (0);
  

  fread(&i1, sizeof(int), 1, fp1);
  NC_setGrafAtt (i1);    // set GR_Att_act


  fread(&APT_VR_IND, sizeof(long), 1, fp1);
  if(APT_VR_IND > 0)
  fread(vr_tab, sizeof(double), APT_VR_IND+1, fp1);
  for(l1=APT_VR_IND+1; l1<APT_VR_SIZ; ++l1) DB_setFree_Var(l1);

  fread(&APT_PT_IND, sizeof(long), 1, fp1);
  if(APT_PT_IND > 0)
  fread(pt_tab, sizeof(Point), APT_PT_IND+1, fp1);
  for(l1=APT_PT_IND+1; l1<APT_PT_SIZ; ++l1) DB_setFree_PT(l1);

  fread(&APT_VC_IND, sizeof(long), 1, fp1);
  if(APT_VC_IND > 0)
  fread(vc_tab, sizeof(Vector), APT_VC_IND+1, fp1);
  for(l1=APT_VC_IND+1; l1<APT_VC_SIZ; ++l1) DB_setFree_VC(l1);

  fread(&APT_LN_IND, sizeof(long), 1, fp1);
  if(APT_LN_IND > 0)
  fread(ln_tab, sizeof(Line), APT_LN_IND+1, fp1);
  for(l1=APT_LN_IND+1; l1<APT_LN_SIZ; ++l1) DB_setFree_LN(l1);
    // printf(" APT_LN_IND=%ld APT_LN_SIZ=%ld\n",APT_LN_IND,APT_LN_SIZ);

  fread(&APT_CI_IND, sizeof(long), 1, fp1);
  if(APT_CI_IND > 0)
  fread(ci_tab, sizeof(Circ), APT_CI_IND+1, fp1);
  for(l1=APT_CI_IND+1; l1<APT_CI_SIZ; ++l1) DB_setFree_CI(l1);

  fread(&APT_PL_IND, sizeof(long), 1, fp1);
  if(APT_PL_IND > 0)
  fread(pln_tab, sizeof(Plane), APT_PL_IND+1, fp1);
  for(l1=APT_PL_IND+1; l1<APT_PL_SIZ; ++l1) DB_setFree_PLN(l1);


  fread(&APT_MR_IND, sizeof(long), 1, fp1);
  if(APT_MR_IND > 0)
  fread(mdr_tab, sizeof(ModelRef), APT_MR_IND+1, fp1);
  for(l1=APT_MR_IND+1; l1<APT_MR_SIZ; ++l1) DB_setFree_MR(l1);

  fread(&APT_AC_IND, sizeof(long), 1, fp1);
  if(APT_AC_IND > 0)
  fread(ac_tab, sizeof(Activity), APT_AC_IND+1, fp1);
  for(l1=APT_AC_IND+1; l1<APT_AC_SIZ; ++l1) DB_setFree_Act(l1);

  fread(&APT_TR_IND, sizeof(long), 1, fp1);
  if(APT_TR_IND > 0)
  fread(tra_tab, sizeof(ObjGX), APT_TR_IND+1, fp1);
  for(l1=APT_TR_IND+1; l1<APT_TR_SIZ; ++l1) DB_setFree_TR(l1);

  fread(&APT_CV_IND, sizeof(long), 1, fp1);
  if(APT_CV_IND > 0)
  fread(cv_tab, sizeof(ObjGX), APT_CV_IND+1, fp1);
  for(l1=APT_CV_IND+1; l1<APT_CV_SIZ; ++l1) DB_setFree_CV(l1);

  fread(&APT_TX_IND, sizeof(long), 1, fp1);
  if(APT_TX_IND > 0)
  fread(tx_tab, sizeof(ObjGX), APT_TX_IND+1, fp1);
  for(l1=APT_TX_IND+1; l1<APT_TX_SIZ; ++l1) DB_setFree_TX(l1);

  fread(&APT_SU_IND, sizeof(long), 1, fp1);
  if(APT_SU_IND > 0)
  fread(su_tab, sizeof(ObjGX), APT_SU_IND+1, fp1);
  for(l1=APT_SU_IND+1; l1<APT_SU_SIZ; ++l1) DB_setFree_SU(l1);

  fread(&APT_SO_IND, sizeof(long), 1, fp1);
  if(APT_SO_IND > 0)
  fread(so_tab, sizeof(ObjGX), APT_SO_IND+1, fp1);
  for(l1=APT_SO_IND+1; l1<APT_SO_SIZ; ++l1) DB_setFree_SO(l1);




  fread(&fSiz, sizeof(long), 1, fp1);
  fread(DB_CDAT, fSiz, 1, fp1);

  fread(&DB_CSEG, sizeof(Memspc), 1, fp1);


  fread(&DYN_VR_IND, sizeof(long), 1, fp1);
  if(DYN_VR_IND > 0)
  fread(vr_dyn, sizeof(double), DYN_VR_IND+1, fp1);
  for(i1=DYN_VR_IND+1; i1<DYN_VR_SIZ; ++i1) vr_dyn[i1] = UT_VAL_MAX;

  fread(&DYN_PT_IND, sizeof(long), 1, fp1);
  if(DYN_PT_IND > 0)
  fread(pt_dyn, sizeof(Point), DYN_PT_IND+1, fp1);
  for(i1=DYN_PT_IND+1; i1<DYN_PT_SIZ; ++i1) pt_dyn[i1].x = UT_VAL_MAX;

  fread(&DYN_VC_IND, sizeof(long), 1, fp1);
  if(DYN_VC_IND > 0)
  fread(vc_dyn, sizeof(Vector), DYN_VC_IND+1, fp1);
  // 1-6: standardVectors !
  for(i1=DYN_VC_IND+1; i1<DYN_VC_SIZ; ++i1) vc_dyn[i1].dx = UT_VAL_MAX;

  fread(&DYN_LN_IND, sizeof(long), 1, fp1);
  if(DYN_LN_IND > 0)
  fread(ln_dyn, sizeof(Line), DYN_LN_IND+1, fp1);
  for(i1=DYN_LN_IND+1; i1<DYN_LN_SIZ; ++i1) ln_dyn[i1].p1.x = UT_VAL_MAX;

  fread(&DYN_CI_IND, sizeof(long), 1, fp1);
  if(DYN_CI_IND > 0)
  fread(ci_dyn, sizeof(Circ), DYN_CI_IND+1, fp1);
  for(i1=DYN_CI_IND+1; i1<DYN_CI_SIZ; ++i1) ci_dyn[i1].p1.x = UT_VAL_MAX;

  fread(&DYN_PL_IND, sizeof(long), 1, fp1);
  if(DYN_PL_IND > 0)
  fread(pln_dyn, sizeof(Plane), DYN_PL_IND+1, fp1);
  // 1-3: standardPlanes !
  for(i1=DYN_PL_IND+1; i1<DYN_PL_SIZ; ++i1) pln_dyn[i1].p = UT_VAL_MAX;

  fread(&DYN_CV_IND, sizeof(long), 1, fp1);
  if(DYN_CV_IND > 0)
  fread(cv_dyn, sizeof(ObjGX), DYN_CV_IND+1, fp1);
  for(i1=DYN_CV_IND+1; i1<DYN_CV_SIZ; ++i1) cv_dyn[i1].typ = Typ_Error;

  fread(&DYN_SU_IND, sizeof(long), 1, fp1);
  if(DYN_SU_IND > 0)
  fread(su_dyn, sizeof(ObjGX), DYN_SU_IND+1, fp1);
  for(i1=DYN_SU_IND+1; i1<DYN_SU_SIZ; ++i1) su_dyn[i1].typ = Typ_Error;

  fread(&DYN_TX_IND, sizeof(long), 1, fp1);
  if(DYN_TX_IND > 0)
  fread(tx_dyn, sizeof(ObjGX), DYN_TX_IND+1, fp1);
  for(i1=DYN_TX_IND+1; i1<DYN_TX_SIZ; ++i1) tx_dyn[i1].typ = Typ_Error;


  fread(&GA_recNr, sizeof(int),  1, fp1);
  if(GA_ObjTab > 0)
  fread(GA_ObjTab, sizeof(ObjAtt), GA_recNr, fp1);

  // save UTF_FilBuf0 (UTF_save__)
  fread(&UTF_FilBuf0Len, sizeof(long),  1, fp1);
  if(UTF_FilBuf0Len > 0)
  fread(UTF_FilBuf0, UTF_FilBuf0Len, 1, fp1);

  // read AP_box_pm1,2 AP_stat-bits mdl_modified and mdl_box_valid
  AP_stat_file (2, fp1);

  // OPAR_file (2, fp1);     // read ParentTable

  fclose(fp1);

  // printf("ex DB_load__ %f\n",AP_txdimsiz);

  //================================================================
  // restore dynamic-data

  // reLoad defCol into GL-List
  GL_init_defCol (&AP_defcol);

  // GR_tx_ardx setzen ..
  GR_InitGFPar (AP_txdimsiz);


    // printf("ex-DB_load__ GA_recNr=%d\n",GA_recNr);
    // printf("ex DB_load__ %d %d %d %d %d %d\n",APT_VR_IND,APT_PT_IND,
                     // APT_VC_IND,APT_LN_IND,APT_CI_IND,APT_PL_IND);
    // printf("             %d %d %d %d %d %d\n",APT_MR_IND,APT_AC_IND,
                     // APT_TL_IND,APT_TR_IND,APT_CV_IND,APT_TX_IND,
                     // APT_SU_IND,APT_SO_IND);

  return 0;

}


/*
//=======================================================================
  int DB_GetObjX (ObjX *ox, int typ, long Ind) {
//=======================================================================
// den Pointer zum Object typ/apt_ind holen



  switch (typ) {


    case Typ_PT:
      ox->pt1 = &pt_tab[Ind];
      break;


    case Typ_LN:
      ox->ln1 = &ln_tab[Ind];
      break;


    case Typ_CI:
    case Typ_CI:
      ox->ci1 = &ci_tab[Ind];
      break;


    default:
      TX_Error("DB_GetObjUX E001");
  }


  return 0;

}
*/

//=======================================================================
  Point* DB_get_PT (long Ind) {
//=======================================================================
/// \code
/// returns pointer to point
/// copy PT mit DB_GetPoint
/// \endcode

  // printf("DB_get_PT %ld\n",Ind);

  if(Ind < 0) {
    Ind = -Ind;
    if(Ind >= DYN_PT_SIZ) Ind = DYN_PT_SIZ - 1;
    if(DB_isFree_PT(&pt_dyn[Ind])) {
    // if(pt_dyn[Ind].x == UT_VAL_MAX) {
      TX_Error (" Dyn.Point %d undefined",Ind);
    }
    return &pt_dyn[Ind];

  } else {
    if(Ind >= APT_PT_SIZ) Ind = APT_PT_SIZ - 1;

    if(DB_isFree_PT(&pt_tab[Ind])) {
    // if(pt_tab[Ind].x == UT_VAL_MAX) {
      TX_Error (" Point %d undefined",Ind);
    }

    return &pt_tab[Ind];
  }
}


//=======================================================================
  Line* DB_get_LN (long Ind) {
//=======================================================================
/// copy: DB_GetLine

  if(Ind < 0) {
    Ind = -Ind;
    if(Ind >= DYN_LN_SIZ) Ind = DYN_LN_SIZ - 1;
    if(DB_isFree_LN(&ln_dyn[Ind])) {
    // if(ln_dyn[Ind].p1.x == UT_VAL_MAX) {
      TX_Error (" Dyn.Line %d undefined",Ind);
    }
    return &ln_dyn[Ind];

  } else {
    if(Ind >= APT_LN_SIZ) Ind = APT_LN_SIZ - 1;

    if(DB_isFree_LN(&ln_tab[Ind])) {
    // if(ln_tab[Ind].p1.x == UT_VAL_MAX) {
      TX_Error (" Line %d undefined",Ind);
    }

    return &ln_tab[Ind];
  }

}

//=======================================================================
  Vector* DB_get_VC (long Ind) {
//=======================================================================
/// copy: DB_GetVector

  if(Ind < 0) {
    Ind = -Ind;
    if(Ind >= DYN_VC_SIZ) Ind = DYN_VC_SIZ - 1;
    if(DB_isFree_VC(&vc_dyn[Ind])) {
    // if(vc_dyn[Ind].dx == UT_VAL_MAX) {
      TX_Error (" Dyn.Vector %d undefined",Ind);
    }
    return &vc_dyn[Ind];

  } else {
    if(Ind >= APT_VC_SIZ) Ind = APT_VC_SIZ - 1;
    if(DB_isFree_VC(&vc_tab[Ind])) {
    // if(vc_tab[Ind].dx == UT_VAL_MAX) {
      TX_Error (" Vector %d undefined",Ind);
    }
    return &vc_tab[Ind];
  }

}


//=======================================================================
  Circ* DB_get_CI (long Ind) {
//=======================================================================
/// copy: DB_GetCirc

  // printf("DB_get_CI %d %d %d\n",Ind,DYN_CI_SIZ,APT_CI_SIZ);


  if(Ind < 0) {
    Ind = -Ind;
    if(Ind >= DYN_CI_SIZ) Ind = DYN_CI_SIZ - 1;
    if(DB_isFree_CI(&ci_dyn[Ind])) {
    // if(ci_dyn[Ind].p1.x == UT_VAL_MAX) {
      TX_Error (" Dyn.Circ %d undefined",Ind);
    }
    return &ci_dyn[Ind];

  } else {
    if(Ind >= APT_CI_SIZ) Ind = APT_CI_SIZ - 1;

    if(DB_isFree_CI(&ci_tab[Ind])) {
    // if(ci_tab[Ind].p1.x == UT_VAL_MAX) {
      TX_Error (" Circ %d undefined",Ind);
    }

    return &ci_tab[Ind];
  }

}


//=======================================================================
  Plane* DB_get_PLN (long Ind) {
//=======================================================================
/// \code
/// see also DB_GetRef DB_StoreRef
/// es gibt keine dyn planes !
/// see DB_GetRef
/// \endcode


  // printf("DB_get_PLN %ld\n",Ind);

  if(Ind < 0) {
    Ind = -Ind;
    if(Ind >= DYN_PL_SIZ) Ind = DYN_PL_SIZ - 1;
    if(DB_isFree_PLN(&pln_dyn[Ind])) {
      TX_Error (" Dyn.Plane %d undefined",Ind);
    }
      // DEB_dump_obj__(Typ_PLN, &pln_dyn[Ind], "ex DB_get_PLN:");
    return &pln_dyn[Ind];
  }


  if(Ind >= APT_PL_SIZ) Ind = APT_PL_SIZ - 1;

  if(DB_isFree_PLN(&pln_tab[Ind])) {
    TX_Error (" Dyn.Plane %d undefined",Ind);
  }

  // DEB_dump_obj__(Typ_PLN, &pln_tab[Ind], "ex DB_get_PLN:");
  return &pln_tab[Ind];

}


//================================================================
  int DB_get_typ_note (long Ind) {
//================================================================
// DB_get_typ_note      get form of Text/Note/Tag/Image

  int   typ;


  if(Ind < 0) {
    typ = tx_dyn[Ind].form;

  } else {
    typ = tx_tab[Ind].form;

  }

  return typ;

}


//================================================================
  int DB_get_typ_cv (long Ind) {
//================================================================
// DB_get_typ_cv      get form of Curve

  int   typ;


  if(Ind < 0) {
    typ = cv_dyn[Ind].form;

  } else {
    typ = cv_tab[Ind].form;

  }

  return typ;

}


//=======================================================================
  void* DB_get_CV (int *ityp, long Ind) {
//=======================================================================
/// \code
/// returns form and struct of Curve BUT NOT ITS PRIMARY OBJECT !
/// Out:
///   ityp   <-- oGX[Ind].form      (od Typ_CVTRM bei Typ_ObjGX)
///   retVal <-- oGX[Ind].data      (also die struct !!!
///
/// see also DB_GetCurv (returns the primary obj (ObjGX))
/// \endcode

  // printf("DB_get_CV %d\n",Ind);


  if(Ind < 0) {
    Ind = -Ind;
    if(Ind >= DYN_CV_SIZ) Ind = DYN_CV_SIZ - 1;
    if(cv_dyn[Ind].form == Typ_ObjGX) {
      *ityp = Typ_CVTRM;
    } else {
      *ityp = cv_dyn[Ind].form;
    }
      // DEB_dump_obj__ (*ityp, cv_dyn[Ind].data, "get_CV:");
    return cv_dyn[Ind].data;

  } else {
    if(Ind >= APT_CV_SIZ) Ind = APT_CV_SIZ - 1;
    if(cv_tab[Ind].form == Typ_ObjGX) {
      *ityp = Typ_CVTRM;
    } else {
      *ityp = cv_tab[Ind].form;
    }
      // DEB_dump_obj__ (*ityp, cv_tab[Ind].data, "get_CV:");
    return cv_tab[Ind].data;
  }

}


//=======================================================================
  ObjGX DB_GetObjGX (int typ, long apt_ind) {
//=======================================================================
/// \code
///   Returns a ObjGX of any object stored in DB;
///   Does NOT resolve objRefs (Typ_Index).
/// DB_GetObjGX    get complexObj from DB-Obj Keep Refs.   Give ObjGX.
///
/// Liefert einen ObjGX-Record von jedem DB-Obj.
/// Error:      o.typ == Typ_Error;
///
/// get datastruct of DB-obj: see UTO__dbo or UTO_objDat_ox
/// \endcode


  static ObjGX ox1;

  ObjGX *oxp;


  // printf("DB_GetObjGX typ=%d ind=%ld\n", typ, apt_ind);


/*
  if(DB_CheckInd(typ, apt_ind) < 0) {
    TX_Error("DB_GetObjGX Index %d fehlerhaft (Typ %d)",apt_ind,typ);
    ox1.typ = Typ_Error;
    return ox1;
  }
*/


  ox1.typ   = typ;
  ox1.form  = typ;
  ox1.siz   = 1;



  switch (typ) {

      case Typ_VAR:
        if(apt_ind >= APT_VR_SIZ) goto L_Error_9;
        ox1.data = DB_get_Var (apt_ind);
        if(DB_isFree_Var(((double*)ox1.data))) goto L_Error_9;
        // if(*((double*)ox1.data) == UT_VAL_MAX) goto L_Error_9;
        break;


      case Typ_VC:
        if(apt_ind >= APT_VC_SIZ) goto L_Error_9;
        ox1.data = DB_get_VC (apt_ind);
        if(DB_isFree_VC(((Vector*)ox1.data))) goto L_Error_9;
        // if(((Vector*)ox1.data)->dx == UT_VAL_MAX) goto L_Error_9;
        break;


      case Typ_PT:
        ox1.data = DB_get_PT (apt_ind);
        if(DB_isFree_PT(((Point*)ox1.data))) goto L_Error_9;
        // if(((Point*)ox1.data)->x == UT_VAL_MAX) goto L_Error_9;
        break;


      case Typ_LN:
        ox1.data = DB_get_LN (apt_ind);
        if(DB_isFree_LN(((Line*)ox1.data))) goto L_Error_9;
        // if(((Line*)ox1.data)->p1.x == UT_VAL_MAX) goto L_Error_9;
        break;


      case Typ_CI:
        ox1.data = DB_get_CI (apt_ind);
        if(DB_isFree_CI(((Circ*)ox1.data))) goto L_Error_9;
        // if(((Circ*)ox1.data)->p1.x == UT_VAL_MAX) goto L_Error_9;
        break;


      case Typ_CV:
      case Typ_CVLNA:
      case Typ_CVPSP3:
      case Typ_CVPOL:
      case Typ_CVPOL2:
      case Typ_CVELL:
      case Typ_CVBSP:
      case Typ_CVRBSP:
      case Typ_CVTRM:
      // case Typ_CVTRM2:
        oxp = DB_GetCurv (apt_ind);
          // printf("cv.typ=%d form=%d siz=%d\n",oxp->typ,oxp->form,oxp->siz);
        if(DB_isFree_CV((oxp))) goto L_Error_9;
        // copy obj
        ox1 = *oxp;
          // printf("GetObjGX ox1:typ=%d form=%d siz=%d\n",
                 // ox1.typ,ox1.form,ox1.siz);
        break;


      case Typ_PLN:
        ox1.data = DB_get_PLN (apt_ind);
        if(DB_isFree_PLN(((Plane*)ox1.data))) goto L_Error_9;
        // if(((Plane*)ox1.data)->p == UT_VAL_MAX) goto L_Error_9;
        break;


      case Typ_Note:
      case Typ_ATXT:
      case Typ_GTXT:
      case Typ_Dimen:
      case Typ_Dim3:
      case Typ_Tag:
        oxp = DB_GetGTxt (apt_ind);
        // if(apt_ind >= APT_TX_SIZ) goto L_Error_9;
        // if(apt_ind >= 0) ox1 = tx_tab[apt_ind];
        // else             ox1 = tx_dyn[-apt_ind];
        if(DB_isFree_GTxt(oxp)) goto L_Error_9;
        ox1 = *oxp;
        break;



      case Typ_SUR:
      case Typ_SURCIR:
      case Typ_SURSTRIP:
      case Typ_SURBSP:    // Typ_SURSUP
      case Typ_SURRU:     // Typ_SURSUP
      case Typ_SURRV:     // Typ_SURSUP
      case Typ_SURSWP:    // Typ_SURSUP
      case Typ_SURSUP:
        oxp = DB_GetSur (apt_ind, 0);
        if(DB_isFree_Sur(oxp)) goto L_Error_9;
        // if(oxp->typ == Typ_Error) goto L_Error_9;
        ox1 = *oxp; // copy obj

        // if(apt_ind >= APT_SU_SIZ) goto L_Error_9; // skip dynam. text
        // ox1 = su_tab[apt_ind];
        // if(su_tab[apt_ind].typ == Typ_Error) goto L_Error_9;
        break;



      case Typ_Model:
      case Typ_Mock:
        if(apt_ind >= APT_MR_SIZ) goto L_Error_9; // skip dynam. text
        ox1.data = &mdr_tab[apt_ind];
        if(DB_isFree_ModRef((&mdr_tab[apt_ind]))) goto L_Error_9;
        // if(mdr_tab[apt_ind].po.x == UT_VAL_MAX) goto L_Error_9;
        break;



      case Typ_SubModel:
        if(apt_ind >= DYN_MB_IND) goto L_Error_9;
        ox1.data = &mdb_dyn[apt_ind];
        break;



      case Typ_Tra:
        oxp = DB_GetTra (apt_ind);
        if(oxp) ox1 = *oxp; // copy obj
        break;



      case Typ_SOL:
        oxp = DB_GetSol (apt_ind);
        if(DB_isFree_Sol(oxp)) goto L_Error_9;
        // if(oxp->typ == Typ_Error) goto L_Error_9;
        ox1 = *oxp; // copy obj
        break;



      default:
        goto L_Error_9;
    }


    // DEB_dump_obj__(Typ_ObjGX, &ox1, "ex DB_GetObjGX:\n");
    // DEB_dump_obj__(ox1.typ, ox1.data, "ex DB_GetObjGX:\n");

  return ox1;



  //----------------------------------------------------------------
  L_Error_9:
        TX_Print("**** DB_GetObjGX E1 Typ=%d Ind=%d",typ,apt_ind);
        ox1.typ = Typ_Error;
        return ox1;




}


//================================================================
  int UTO__dbo (void **pDat, int *oNr, int dbTyp, long dbInd) {
//================================================================
/// \code
/// UTO__dbo          get data-struct from typ, DB-index
///   Returns a pointer to the data-struct.
///   Referenced objects (Typ_Index) are resolved.
/// Surfaces/Solids: retCod = typ_ObjGX; *pDat = the address of the primary obj.
/// 
/// Output:
///   pDat        pointer to dataStruct of DB-Obj dbTyp/dbInd
///   oNr         nr of structs of type <Retcode>
///   Retcode     type of datastruct pDat
///               Typ_Error  (0)   obj unused
///               -1               UTO_objDat_ox - error
/// 
/// see also UTO_obj_dbo UTO_objDat_dbS UTO_objDat_ox DB_GetObjGX
/// \endcode

// OFFEN: replace UTO_obj_dbo with UTO__dbo


  ObjGX  ox1;


  // printf("UTO__dbo typ=%d dbi=%ld\n",dbTyp,dbInd);


  // Surfaces: return adress of surface directly (ox1 is not static !)
  if(dbTyp == Typ_SUR) {
    *pDat = DB_GetSur (dbInd, 0);
    *oNr = 1;
    // if(((ObjGX*)*pDat)->typ == Typ_Error) return Typ_Error;
    if(DB_isFree_Sur((ObjGX*)*pDat)) return Typ_Error;
    return Typ_ObjGX;
  }

  // Surfaces: return adress of surface directly (ox1 is not static !)
  if(dbTyp == Typ_SOL)   {
    *pDat = DB_GetSol (dbInd);
    *oNr = 1;
    // if(((ObjGX*)*pDat)->typ == Typ_Error) return Typ_Error;
    if(DB_isFree_Sol((ObjGX*)*pDat)) return Typ_Error;
    return Typ_ObjGX;
  }


  if(dbTyp == Typ_Tra)   {
    *pDat = DB_GetTra (dbInd);
    *oNr = 1;
    if(*pDat == NULL) return Typ_Error;
    return Typ_ObjGX;
  }




  // get ObjGX of a object stored in DB
  ox1 = DB_GetObjGX (dbTyp, dbInd);
  if(!ox1.typ) return -1;

  // get data-struct from ObjGX
  return UTO_objDat_ox (pDat,  oNr, &ox1);

}


/*
//================================================================
  int DB_GetObjG2 (ObjG2* o1, int typ, long ind) {
//================================================================


  if(DB_CheckInd(typ, ind) < 0) goto Error1; 

  switch (typ) {

    case Typ_PT2:
      *o1 = UT2D_obj_pt3 (&pt_tab[ind]);
      break;


    case Typ_LN2:
      *o1 = UT2D_obj_ln3 (&ln_tab[ind]);
      break;


    case Typ_CI2:
    case Typ_CI2:
      *o1 = UT2D_obj_ci3 (&ci_tab[ind]);
      break;


    case Typ_CV2:
      // *o1 = UT2D_obj_cv3 (&cv_tab[ind]);
      TX_Error("DB_GetObjG2 E001\n");
      break;


    default:
      TX_Error("DB_GetObjG2 Typ=%d Ind=%d",typ,ind);
      return -1;
  }

  return 0;



  Error1:
  TX_Error("DB_GetObjG2 Index %d fehlerhaft (Typ %d)",ind,typ);
  return -1;
}
*/

/*
//================================================================
  int DB_CheckInd (int typ, long Ind) {
//================================================================
// 1=Index OK, -1=fehlerhaft.


  switch (typ) {

    case Typ_PT2:
      if((Ind < 0)||(Ind > APT_PT_SIZ)) goto Error1;
      break;


    case Typ_LN2:
      if((Ind < 0)||(Ind > APT_LN_SIZ)) goto Error1;
      break;


    case Typ_CI2:
    case Typ_CI2:
      if((Ind < 0)||(Ind > APT_CI_SIZ)) goto Error1;
      break;


    case Typ_CV2:
      if((Ind < 0)||(Ind > APT_CV_SIZ)) goto Error1;
      break;


    default:
      TX_Error("DB_CheckInd Typerror - Typ=%d Ind=%d",typ,Ind);
      return -1;
  }

  return 1;



  Error1:
  TX_Error("DB_CheckInd Index %d fehlerhaft (Typ %d)",Ind,typ);
  return -1;
}
*/


/*
//================================================================
  int DB_Get_GR_Obj (ObjG* o1, int apt_typ, long apt_ind, int tra_ind) {
//================================================================

// Ein Objekt lesen und transformieren
// irc 1 = OK; -1=Fehler



  int      typ1;
  Point    pt1;
  Line     ln1;
  Circ     ci1;
  // ObjG        cv1;
  ObjGX       *ox1;

  //Mat_4x3* m1;


  printf("DB_Get_GR_Obj typ=%d ind=%d tra=%d\n",apt_typ,apt_ind,tra_ind);

  if(apt_ind < 0) return -1;


    switch (apt_typ) {


      case Typ_PT:
        // if(apt_ind >= dyn_pt_max) return -1; // skip dynam. obj
        pt1 = DB_GetPoint (apt_ind);
        if(tra_ind != 0) {
          UT3D_pt_tra_pt_m3 (&pt1, mat_tab[tra_ind], &pt1);
        }
        *o1  = UT3D_obj_pt (&pt1);
        break;


      case Typ_LN:
        // if(apt_ind >= dyn_ln_max) return -1; // skip dynam. obj
        ln1 = DB_GetLine (apt_ind);
        if(tra_ind != 0) {
          UT3D_pt_tra_pt_m3 (&ln1.p1, mat_tab[tra_ind], &ln1.p1);
          UT3D_pt_tra_pt_m3 (&ln1.p2, mat_tab[tra_ind], &ln1.p2);
        }
        *o1  = UT3D_obj_ln (&ln1);
        break;


      case Typ_CI:
      case Typ_CI:
        // if(apt_ind >= dyn_ci_max) return -1; // skip dynam. obj
        ci1 = DB_GetCirc (apt_ind);
        if(tra_ind != 0) {
          UT3D_pt_tra_pt_m3 (&ci1.p1, mat_tab[tra_ind], &ci1.p1);
          UT3D_pt_tra_pt_m3 (&ci1.p2, mat_tab[tra_ind], &ci1.p2);
          UT3D_pt_tra_pt_m3 (&ci1.pc, mat_tab[tra_ind], &ci1.pc);
          ci1.vz.dx  = mat_tab[tra_ind][0][2];
          ci1.vz.dy  = mat_tab[tra_ind][1][2];
          ci1.vz.dz  = mat_tab[tra_ind][2][2];
        }
        *o1  = UT3D_obj_ci (&ci1);
        break;


      case Typ_CV:
      case Typ_CVTRM:
      case Typ_CVELL:
      case Typ_CVBSP:
        // if(apt_ind >= dyn_cv_max) return -1; // skip dynam. obj
        ox1 = DB_GetCurv (apt_ind);
        o1->typ  = ox1->typ;
        o1->data = ox1->data;
        return 1;



      case Typ_PLN:
        // if(apt_ind >= dyn_su_max) return -1; // skip dynam. obj
        return 1;



      case Typ_SUR:
        // if(apt_ind >= dyn_su_max) return -1; // skip dynam. obj
        return 1;



      case Typ_SOL:                  // ??????
        // if(apt_ind >= dyn_su_max) return -1; // skip dynam. obj
        return 1;



      case Typ_Model:
        o1->typ  = Typ_Model;
        o1->data = (void*)DB_get_ModRef(apt_ind);
        return 1;



      case Typ_ATXT:
      case Typ_GTXT:
      case Typ_Dimen:
        if(apt_ind >= APT_TX_SIZ) return -1; // skip dynam. text
        ox1 = DB_GetGTxt (apt_ind);
        o1->typ  = ox1->typ;
        o1->data = ox1->data;
        return 1;



      default:
        TX_Error("DB_Get_GR_Obj Typ=%d Ind=%d",apt_typ,apt_ind);
        return -1;

    }

  return 1;

}
*/


//=======================================================================
long DB_GetDynInd (int typ) {
//=======================================================================
/// \code
/// returns next free dynamic object-Index for typ (negative)
/// \endcode



  // printf("DB_GetDynInd %d\n",typ);


  if(typ == Typ_VAR) {
    ++DYN_VR_IND;
    if(DYN_VR_IND >= DYN_VR_SIZ) {
      if(DB_allocDynVector (1) < 0) {
        --DYN_VR_IND;
        return 0;
        // TX_Error ("******** Dyn-VR-DB voll *********");
      }
    }
    // printf("ex DB_GetDynInd VR %d\n",-DYN_VR_IND);
    return -DYN_VR_IND;





  } else if(typ == Typ_VC) {
    ++DYN_VC_IND;
    if(DYN_VC_IND >= DYN_VC_SIZ) {
      if(DB_allocDynVector (1) < 0) {
        --DYN_VC_IND;
        return 0;
        // TX_Error ("******** Dyn-VC-DB voll *********");
      }
    }
      // printf("ex DB_GetDynInd -DYN_VC_IND=%ld\n",-DYN_VC_IND);
    return -DYN_VC_IND;




  } else if(typ == Typ_PT) {
    ++DYN_PT_IND;
    if(DYN_PT_IND >= DYN_PT_SIZ) {
      if(DB_allocDynPoint (1) < 0) {
        --DYN_PT_IND;
        return 0;
        // TX_Error ("******** Dyn-pt-DB voll *********");
      }
    }
    // printf("ex DB_GetDynInd PT %d\n",-DYN_PT_IND);
    return -DYN_PT_IND;




  } else if (typ == Typ_LN) {
    ++DYN_LN_IND;
    if(DYN_LN_IND >= DYN_LN_SIZ) {
      if(DB_allocDynLine (1) < 0) {
        --DYN_LN_IND;
        return 0;
        // TX_Error ("******** Dyn-LN-DB voll *********");
      }
    }
    // printf("ex DB_GetDynInd LN %d\n",-DYN_LN_IND);
    return -DYN_LN_IND;




  } else if (typ == Typ_CI) {
    ++DYN_CI_IND;
    if(DYN_CI_IND >= DYN_CI_SIZ) {
      if(DB_allocDynCirc (1) < 0) {
        --DYN_CI_IND;
        return 0;
        // TX_Error ("******** Dyn-CI-DB voll *********");
      }
    }
    // printf("ex DB_GetDynInd CI %d\n",-DYN_CI_IND);
    return -DYN_CI_IND;



  } else if (typ == Typ_PLN) {
    ++DYN_PL_IND;
    if(DYN_PL_IND >= DYN_PL_SIZ) {
      if(DB_allocDynPlane (1) < 0) {
        --DYN_PL_IND;
        return 0;
        // TX_Error ("******** Dyn-PL-DB voll *********");
      }
    }
    // printf("ex DB_GetDynInd PL %d\n",-DYN_PL_IND);
    return -DYN_PL_IND;




  } else if (typ == Typ_CV) {
    ++DYN_CV_IND;
    if(DYN_CV_IND >= DYN_CV_SIZ) {
      if(DB_allocDynCurve(1) < 0) {
        --DYN_CV_IND;
        return 0;
        // TX_Error ("******** Dyn-CV-DB voll *********");
      }
    }
    // printf("ex DB_GetDynInd CV %d\n",-DYN_CV_IND);
    return -DYN_CV_IND;



  } else if (typ == Typ_SUR) {
    ++DYN_SU_IND;
    if(DYN_SU_IND >= DYN_SU_SIZ) {
      if(DB_allocDynSur(1) < 0) {
        --DYN_SU_IND;
        return 0;
        // TX_Error ("******** Dyn-SU-DB voll *********");
      }
    }
      printf("ex DB_GetDynInd SU %ld\n",-DYN_SU_IND);
    return -DYN_SU_IND;




  } else if (typ == Typ_GTXT) {
    ++DYN_TX_IND;
    if(DYN_TX_IND >= DYN_TX_SIZ) {
      if(DB_allocDynNote(1) < 0) {
        --DYN_TX_IND;
        return 0;
        // TX_Error ("******** Dyn-TX-DB voll *********");
      }
    }
    // printf("ex DB_GetDynInd TX %d\n",-DYN_TX_IND);
    return -DYN_TX_IND;


/*
 } else if (typ == Typ_SUR) {
    if(dyn_cv_ind < dyn_cv_max) {
      ++dyn_cv_ind;
    } else {
      TX_Error ("******** Dyn-cv-DB voll *********");
    }
    //TX_Print("DB_GetDynInd CV %d\n",dyn_cv_ind);
    return dyn_cv_ind;
*/





  }




  TX_Error(" 1 DB_GetDynInd");
  return 0;
}



/*
// ***********************************************************************
  int DB_StoreAtt (long Ind, GR_Att* att1) {
// ***********************************************************************

  //TX_Print("DB_StoreAtt %d = %d %d %d",Ind,att1->col,att1->ltyp,att1->lthick);




  // Alle dürfen nur zwischen 0 und 15 sein!
  int i1=att1->col;
  int i2=att1->ltyp;
  int i3=att1->lthick;
  if((i1<0)||(i1>15)) {
    TX_Print(" Parameter Farbe nicht 0 bis 15");
  } else   if((i2<0)||(i1>15)) {
    TX_Print(" Parameter Linientyp nicht 0 bis 15");
  } else   if((i3<0)||(i1>15)) {
    TX_Print(" Parameter Liniendicke nicht 0 bis 15");
  }





  if((Ind < 0)||(Ind >= GR_ATT_TAB_SIZ)) {
    TX_Error("- Zugriff auf Attribut %d ****",Ind);
    return -1;
  }


  GR_AttTab[Ind]        = *att1;
  GR_AttTab[Ind].used   = ON;


  // das Attr nun in der DL definieren
  GL_InitNewAtt (0, Ind);




  return 0;

}
*/



/*
//==============================================================
  int DB_StoreTxt (long Ind, char* txt) {
//==============================================================

  TX_Print("DB_StoreTxt %d %s",Ind,txt);

  strcpy (txt_tab[Ind], txt);

  return 0;

}
*/



//==============================================================
  ObjGX* DB_GetGTxt (long Ind) {
//==============================================================
/// get Note (GText, AText, Dimen, tag, Image ..)

  ObjGX       *tx_out;


  // printf("DB_GetGTxt %ld\n",Ind);
  // printf("  typ=%d form=%d\n",tx_tab[Ind].typ,tx_tab[Ind].form);


  if(Ind < 0) {
    if (Ind <= -DYN_TX_SIZ) {
      TX_Error("- Zugriff auf dyn. Note %d ****",Ind);
      return &tx_tab[0];
    }
    tx_out = &tx_dyn[-Ind];

  } else {
    if (Ind >= APT_TX_SIZ) {
      TX_Error("- Zugriff auf Note %d ****",Ind);
      return &tx_tab[0];
    }
    tx_out = &tx_tab[Ind];
  }

  // if(tx_out->typ == Typ_Error) {
  if(DB_isFree_GTxt (tx_out)) {
    TX_Error (" Note %d undefined",Ind);
    tx_out = &tx_tab[0];
  }


/*
  // nur Test:
  DEB_dump_obj__(Typ_ObjGX, tx_out, "ex DB_GetGTxt");
  // DEB_dump_obj__(tx_out->form, tx_out->data, "ex DB_GetGTxt");
  // DEB_dump_ox_0 (tx_out, "ex DB_GetGTxt");
  // gt1 = tx_tab[Ind].data;
  // printf(" DB_GetGTxt %ld pt=%f,%f\n",Ind,tx_out->pt.x,tx_out->pt.y);
  // printf(" GTxt |%s|\n",tx_out->txt);
  {  
    // AText *atx; atx=tx_out->data; printf(" txPtr=%p\n",atx->txt);
    if(tx_out->form == Typ_GTXT) {
      GText     *gtx1;
      gtx1 = tx_out->data;
      printf(" GText %p |%s|\n",gtx1->txt,gtx1->txt);
      DEB_dump_obj__(Typ_GTXT, gtx1, "ex DB_GetGTxt:");
    }
  }
*/


  return tx_out;

}


//================================================================
  char *DB_get_Text (long Ind) {
//================================================================
// DB_get_Text              get text only (GText - stored with "N#="txt")
 
  ObjGX     *ox1;
  
  // printf(" DB_get_Text %ld\n",Ind);
  
  ox1 = (ObjGX*)DB_GetGTxt (Ind);
  if(ox1->form != Typ_GTXT) {
    TX_Error ("DB_get_Text %ld E001",Ind);
    return NULL;
  }

    // TESTBLOCK
    // DEB_dump_obj__(Typ_ObjGX, ox1, "ex-DB_get_Text %ld",Ind);
    // printf("ex-DB_get_Text %ld txt=|%s|\n",Ind,((GText*)ox1->data)->txt);
    // END TESTBLOCK
  
  return ((GText*)ox1->data)->txt;
  
} 
  

/*
//==============================================================
long DB_StoreTex (long Ind, TexBas *oi) {
//==============================================================

  int       i1;
  long      dbi;
  ObjGX     *txo;
  TexBas    *texNew;


  printf("DB_StoreTex %d |%s|\n",Ind,oi->fnam);


  dbi = DB_Store_hdr_nt (&txo, Ind);


  texNew = DB_cPos ();
  DB_cSav(sizeof(TexBas), (void*)oi);



  // den Text selbst merken
  if(oi->fnam != NULL) {

    // new Pos of Text
    texNew->fnam = DB_cPos ();

    // den Text selbst merken
    i1=strlen(oi->fnam)+1;
    DB_cSav(i1, oi->fnam);
  }


  txo->typ   = Typ_TEXB;
  txo->form  = Typ_TEXB;
  txo->siz   = 1;
  txo->data  = texNew; // replace


  return dbi;

}
*/

//==============================================================
long DB_StoreImg (long Ind, AText *oi) {
//==============================================================

  int       i1;
  long      dbi;
  ObjGX     *txo;
  AText     *gtNew;


  // printf("DB_StoreImg %d |%s|\n",Ind,oi->txt);


  dbi = DB_Store_hdr_nt (&txo, Ind);


  gtNew = DB_cPos ();
  DB_cSav(sizeof(AText), (void*)oi);



  // den Text selbst merken
  if(oi->txt != NULL) {

    // new Pos of Text
    gtNew->txt = DB_cPos ();

    // den Text selbst merken
    i1=strlen(oi->txt)+1;
    DB_cSav(i1, oi->txt);
  }


  txo->typ   = Typ_Tag;
  txo->form  = Typ_ATXT;
  txo->siz   = 1;
  txo->data  = gtNew; // replace


  return dbi;

}


//==============================================================
long DB_StoreDim_ (long Ind, Dimen *dim1) {
//==============================================================

  int  i1;
  long dbi;
  void      *cPos;
  ObjGX     *txo;
  Dimen     *gtNew;


  // printf("DB_StoreDim_ %d |%s|\n",Ind,dim1->txt);

  // store -> tx_dyn | tx_tab
  dbi = DB_Store_hdr_nt (&txo, Ind);
    // printf(" dbi=%d\n",dbi);



  gtNew = DB_cPos ();
  cPos = DB_cSav(sizeof(Dimen), (void*)dim1);
  if(!cPos) {printf("DB_StoreDim_ E001\n"); return -1;}



  // den Text selbst merken
  if(dim1->txt != NULL) {

    // new Pos of Text
    gtNew->txt = DB_cPos ();

    // den Text selbst merken
    i1=strlen(dim1->txt)+1;
    DB_cSav(i1, dim1->txt);
  }



  txo->typ   = Typ_Note;
  txo->form  = Typ_Dimen;
  txo->siz   = 1;
  txo->data  = gtNew;


  return dbi;

}


//==============================================================
long DB_StoreDim3 (long Ind, Dim3 *dim3) {
//==============================================================

  int  i1;
  long dbi;
  void      *cPos;
  ObjGX     *txo;
  Dim3      *dNew;


  // printf("DB_StoreDim3 Ind=%d |%s|\n",Ind,dim3->txt);


  dbi = DB_Store_hdr_nt (&txo, Ind);


  dNew = DB_cPos ();
  DB_cSav(sizeof(Dim3), (void*)dim3);


  // den Text selbst merken
  if(dim3->txt != NULL) {

    // new Pos of Text
    dNew->txt = DB_cPos ();

    // den Text selbst merken
    i1=strlen(dim3->txt)+1;
    DB_cSav(i1, dim3->txt);
  }


  txo->typ   = Typ_Note;
  txo->form  = Typ_Dim3;
  txo->siz   = 1;
  txo->data  = dNew;


  return dbi;

}


//==============================================================
long DB_StoreATxt (long Ind, AText *atx1) {
//==============================================================

  int       i1;
  long      dbi;
  void      *cPos;
  ObjGX     *txo;
  AText     *gtNew;

  // printf("DB_StoreATxt %d |%s|\n",Ind,atx1->txt);


  dbi = DB_Store_hdr_nt (&txo, Ind);


  // save AText-struct
  gtNew = DB_cPos ();
  DB_cSav(sizeof(AText), (void*)atx1);


  if(atx1->txt) {
    // replace new Pos of Text
    gtNew->txt = DB_cPos ();
    // save Text
    i1=strlen(atx1->txt)+1;
    DB_cSav(i1, atx1->txt);
      // printf(" _StoreATxt %d |%s| %p\n",i1,gtNew->txt,gtNew->txt);
  }

  // fix ObjGX-struct
  txo->typ   = Typ_Note;
  txo->form  = Typ_ATXT;
  txo->siz   = 1;
  txo->data  = gtNew;

  // DEB_dump_obj__ (Typ_ATXT, gtNew, "ex DB_StoreATxt");


  return dbi;

}


//==============================================================
long DB_StoreGTxt (long Ind, GText *gtx1) {
//==============================================================

  int       i1;
  long      dbi;
  void      *cPos;
  ObjGX     *txo;
  GText     *gtNew;


  // printf("DB_StoreGTxt %d |%s|\n",Ind,gtx1->txt);


  dbi = DB_Store_hdr_nt (&txo, Ind);


  gtNew = DB_cPos ();
  DB_cSav(sizeof(GText), (void*)gtx1);


  // new Pos of Text
  gtNew->txt = DB_cPos ();


  // den Text selbst merken
  i1=strlen(gtx1->txt)+1;
  DB_cSav(i1, gtx1->txt);


  txo->typ   = Typ_Note;
  txo->form  = Typ_GTXT;
  txo->siz   = 1;
  txo->data  = gtNew;

  
    // DEB_dump_obj__(Typ_GTXT, gtNew, "ex DB_StoreGTxt:");
    // printf(" txt=%p\n",gtNew->txt);


  return dbi;

}


/*
//==============================================================
  int DB_StoreTxt (char **pos, char *text) {
//==============================================================


  int i1;


  // TX_Print("DB_StoreTxt |%s|",text);



  i1=strlen(text)+1;


  if((str_ind + i1) > APT_CH_SIZ) {
    TX_Error("DB_StoreTxt: overflow ");
    *pos = NULL;
    return -1;
  }


  *pos = &str_tab[str_ind];


  memcpy(&str_tab[str_ind], text, i1);


  str_ind += i1;


  printf("ex DB_StoreTxt pos=%d i1=%d\n",str_ind,i1);

  return 0;

}
*/


//==============================================================
  long DB_StoreVar (long Ind, double d1) {
//==============================================================


  long dbi;


  // printf("DB_StoreVar %ld %lf\n",Ind,d1);


  // Dynam. Objekt erzeugen -
  if(Ind < 0) {
    dbi = DB_GetDynInd(Typ_VAR);
    vr_dyn[-dbi] = d1;


  } else {

    if(Ind > APT_VR_IND) {
      APT_VR_IND = Ind;

      if(Ind >= APT_VR_SIZ) {    // realloc
        if(DB_allocVAR (Ind) < 0) return 0;
      }
    }

    dbi = Ind;
    vr_tab[dbi] = d1;
  }




  // var_tab[dbi] = DB_cPos ();
  // DB_cSav(sizeof(double), (void*)&d1);



  return dbi;

}


//================================================================
  double* DB_get_Var (long Ind) {
//================================================================

  if(Ind < 0) {
    Ind = -Ind;
    if(Ind >= DYN_VR_SIZ) Ind = DYN_VR_SIZ - 1;
    return &vr_dyn[Ind];
  } else {
    if(Ind >= APT_VR_SIZ) Ind = APT_VR_SIZ - 1;
    return &vr_tab[Ind];
  }

}


//==============================================================
  double DB_GetVar (long Ind) {
//==============================================================

  double *d1;


  // if((Ind < 0)||(Ind >= dyn_vr_max)) {
  if(Ind < 0) {
    if (Ind <= -DYN_VR_SIZ) {
      TX_Error("- Zugriff auf Variable %d ****",Ind);
      return 0.;
    }
    d1 = &vr_dyn[-Ind];



  } else {
    if (Ind >= APT_VR_SIZ) {
      TX_Error("- Zugriff auf Var %d ****",Ind);
      return 0.;
    }
    d1 = &vr_tab[Ind];
  }



  // if(*d1 == UT_VAL_MAX) {
  if(DB_isFree_Var(d1)) {
      // printf(" V %d = %f\n",Ind,*d1);
    TX_Error (" Variable %d undefined",Ind);
    return 0.;
  }

  // printf("ex DB_GetVar %d %f\n",Ind,*d1);
  return *d1;

}


//=======================================================================
  int DB_CkRef (long Ind) {
//=======================================================================
/// \code
/// check if Plane[Ind] is defined.
/// RC=0; jes is defined.
/// RC=-1: no not defined.
/// RC=-2: no Ind out of range
/// \endcode

  int irc;

  if(Ind >= APT_PL_SIZ) return -2;


  // if(pln_tab[Ind].p == UT_VAL_MAX) {
  if(DB_isFree_PLN(&pln_tab[Ind])) {
    irc = -1;
  } else {
    irc = 0;
  }
   
  // printf("ex DB_CkRef %ld %d %f\n",Ind,irc,pln_tab[Ind].p);

  return irc;

}


//***********************************************************************
  long DB_StoreRef (long Ind, Plane* pl1) {
//***********************************************************************


  // DEB_dump_obj__(Typ_PLN, pl1, "DB_StoreRef RefSys %d\n",Ind);


  if(Ind < 0) {
    Ind = DB_GetDynInd(Typ_PLN);
    pln_dyn[-Ind] = *pl1;
    // DEB_dump_obj__(Typ_PLN, &pln_dyn[-Ind], "ex DB_StoreRef RefSys %d\n",Ind);


  //---------------- normal ---------------------------
  } else {


    if(Ind > APT_PL_IND) {
      APT_PL_IND = Ind;

      if(Ind >= APT_PL_SIZ) {    // realloc
        if(DB_allocRef (Ind) < 0) return 0;
      }
    }

    pln_tab[Ind] = *pl1;
    // DEB_dump_obj__(Typ_PLN, &pln_tab[Ind], "ex DB_StoreRef RefSys %d\n",Ind);

  }


  return Ind;

}


//================================================================
  Plane* DB_DefRef (long Ind) {
//================================================================
/// \code
/// create defaultPlanes in DB
/// Input:
///   Ind   0  create all defaultPlanes
///        <0  return Plane
/// \endcode

  int    i1;

  // printf("DB_DefRef %d\n",Ind);


  if(Ind < 0) {
    return &pln_dyn[-Ind];
  }

  // init planes ..
  i1 = -DB_PLX_IND;     // RX
      pln_dyn[i1].vx = UT3D_VECTOR_Y;
      pln_dyn[i1].vy = UT3D_VECTOR_Z;
      pln_dyn[i1].vz = UT3D_VECTOR_X;
      UT3D_pl_ptpl (&pln_dyn[i1], (Point*)&UT3D_PT_NUL);

  i1 = -DB_PLY_IND;     // RY
      pln_dyn[i1].vx = UT3D_VECTOR_IX;
      pln_dyn[i1].vy = UT3D_VECTOR_Z;
      pln_dyn[i1].vz = UT3D_VECTOR_Y;
      UT3D_pl_ptpl (&pln_dyn[i1], (Point*)&UT3D_PT_NUL);

  i1 = -DB_PLZ_IND;     // RZ = XYZ
      pln_dyn[i1].vx = UT3D_VECTOR_X;
      pln_dyn[i1].vy = UT3D_VECTOR_Y;
      pln_dyn[i1].vz = UT3D_VECTOR_Z;
      UT3D_pl_ptpl (&pln_dyn[i1], (Point*)&UT3D_PT_NUL);



  i1 = -DB_PLIX_IND;    // RIX
      pln_dyn[i1].vx = UT3D_VECTOR_Y;
      pln_dyn[i1].vy = UT3D_VECTOR_IZ;
      pln_dyn[i1].vz = UT3D_VECTOR_IX;
      UT3D_pl_ptpl (&pln_dyn[i1], (Point*)&UT3D_PT_NUL);

  i1 = -DB_PLIY_IND;    // RIY
      pln_dyn[i1].vx = UT3D_VECTOR_X;  // 2019-06-11  was IX
      pln_dyn[i1].vy = UT3D_VECTOR_Z;  // 2019-06-11  was IZ
      pln_dyn[i1].vz = UT3D_VECTOR_IY;
      UT3D_pl_ptpl (&pln_dyn[i1], (Point*)&UT3D_PT_NUL);

  i1 = -DB_PLIZ_IND;    // RIZ
      pln_dyn[i1].vx = UT3D_VECTOR_X;
      pln_dyn[i1].vy = UT3D_VECTOR_IY;
      pln_dyn[i1].vz = UT3D_VECTOR_IZ;
      UT3D_pl_ptpl (&pln_dyn[i1], (Point*)&UT3D_PT_NUL);

  DYN_PL_IND = 6;
 
  return NULL;

}

 
//================================================================
  void DB_GetRef (Plane* pl1, long Ind) {
//================================================================
// DB_GetRef             get a copy of a plane
// see DB_get_PLN
// see defaultPlanes DB_DefRef DB_PLX_IND - DB_PLIZ_IND

  // printf("DB_GetRef %d\n",Ind);

  Plane  *plx;


  if(Ind < 0) {
    Ind = -Ind;
    if(Ind >= DYN_PL_SIZ) Ind = DYN_PL_SIZ - 1;
    *pl1 = pln_dyn[Ind];
    return;

  } 

  if(Ind >= APT_PL_SIZ) Ind = APT_PL_SIZ - 1;
  if(DB_isFree_PLN(&pln_tab[Ind])) {
  // if(pln_tab[Ind].p == UT_VAL_MAX) {
    TX_Error (" Plane %d undefined",Ind);
    return;
  }

  *pl1 = pln_tab[Ind];

  return;

}


/*
//======================================================================
  long DB_GetPtInd (long Ind) {
//======================================================================

  return ptiTab[Ind];

}

//======================================================================
  void DB_StorePtInd (long ptInd) {
//======================================================================

  // printf("DB_StorePtInd %d %d\n",ptInd,ptiInd);

  if((ptiInd < 0)||(ptiInd >= APT_PTI_SIZ)) {
    TX_Error("DB_StorePtInd E001\n");
    return;
  }

  ptiTab[ptiInd] = ptInd;
  ++ptiInd;

  return;

}
*/


//======================================================================
  ObjGX* DB_GetSol (long Ind) {
//======================================================================

  ObjGX       *so_out;

  // printf("DB_GetSol %d\n",Ind);


  if(Ind < 0) {
    // if (Ind >= DYN_SO_SIZ) {
      TX_Error("- Zugriff auf dyn. Solid %d ****",Ind);
      // return &cv_tab[0];
    // }
    // cv_out = &cv_dyn[-Ind];

  } else {
    if (Ind >= APT_SO_SIZ) {
      TX_Error("- Zugriff auf Solid %d ****",Ind);
      goto L_err;
    }
    so_out = &so_tab[Ind];
  }

  // if(so_out->typ == Typ_Error) {
  if(DB_isFree_Sol(so_out)) {
    TX_Error (" Solid %d undefined",Ind);
    goto L_err;
  }


  //---- Anf Testausg:
  // oiTab = so_tab[Ind].oiTab;
  // printf("  typ=%d ind=%d\n",oiTab[0].typ,oiTab[0].ind);
  // printf("  typ=%d ind=%d\n",oiTab[1].typ,oiTab[1].ind);
  //---- End Testausg:


  return so_out;


  L_err:
    so_out = &so_tab[APT_SO_SIZ-1];
    so_out->typ = Typ_Error;
    return so_out;


}


//======================================================================
  int DB_StoreSol (long *dbi, ObjGX *bd1) {
//======================================================================
// store solid in DB.
// types: Typ_SPH,Typ_CON,Typ_TOR,Typ_SOL
//    Typ_SOL,TypObjGX = PRISM: 2 ox (baseCurve & value (thick) or vector)


  int       irc;
  long      oSiz, Ind;
  void      *cPos;
  ObjGX     *oxo;


  // printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx \n");
  // printf("DB_StoreSol dbi=%ld typ=%d form=%d\n",*dbi,bd1->typ,bd1->form);
  // printf(" APT_SO_IND=%ld APT_SO_SIZ=%ld\n",APT_SO_IND,APT_SO_SIZ);
  // DEB_dump_ox_s_ (bd1, "DB_StoreSol-in\n");
  // DEB_dump_ox_0 (bd1, "DB_StoreSol-in\n");
  


  //----------------------------------------------------------------
  // save parentRecord & data
  irc = DB_store_obj (&cPos, bd1->typ, bd1->form, bd1->data, bd1->siz, dbi);
    // printf(" irc ex _store_stru %d\n",irc);
  if(irc < 0) return -1;



  //---- Anf Testausg:
  // oxo = DB_GetSol (*dbi);
  // DEB_dump_ox_s_ (oxo, "DB_StoreSol-out\n");
  // DEB_dump_ox_0 (oxo, "DB_StoreSol-out\n");
  // printf("xxxxxxxxxxxxxxxxxxx ex DB_StoreSol %ld\n",*dbi);
  // exit(0);
  //---- End Testausg:


  return 0;

}


//======================================================================
  int DB_StoreDummy (int typ, long Ind) {
//======================================================================
/// occupy

  ObjGX   ox1;

  // printf("DB_StoreDummy %d %ld\n",typ,Ind);


  // das SUR-Obj in der DB besetzen, damit es nicht mehr als frei gemeldet wird
  ox1.typ = Typ_ObjGX;


  if(typ == Typ_SUR) {
    DB_StoreSur (&Ind, &ox1);

  } else {
    TX_Error ("DB_StoreDummy E001_%d",typ);
    return -1;
  }


  return 0;

}


//================================================================
  long DB_StoreSurBsp (long Ind, SurBSpl *si) {
//================================================================
/// save RevSur s1 --> su_tab[Ind], Daten nach DB_CSEG.

  long     i1;
  ObjGX    *so;
  void     *cPos1, *cPos2, *cPos3, *cPos4;

  // printf("DB_StoreSurBsp %d\n",Ind);
  // DEB_dump_obj__ (Typ_SURRV, si, "DB_StoreSurBsp %d\n",Ind);

  Ind = DB_Store_hdr_su (&so, Ind);

  // Startpos im mem 
  cPos1 = DB_cPos ();

  // die cpTab der SurBSpl speichern
  i1 = sizeof(Point) * si->ptUNr * si->ptVNr;
  cPos2 = DB_cSav(i1, (void*)si->cpTab);

  // die kvTabU der SurBSpl speichern
  i1 = sizeof(double) * (si->ptUNr + si->degU + 1);
  cPos3 = DB_cSav(i1, (void*)si->kvTabU);

  // die kvTabV der SurBSpl speichern
  i1 = sizeof(double) * (si->ptVNr + si->degV + 1);
  cPos4 = DB_cSav(i1, (void*)si->kvTabV);

  // SurBSpl speichern
  DB_cSav(sizeof(SurBSpl), (void*)si);


  so->typ    = Typ_SURBSP;
  so->form   = Typ_SURBSP;
  so->siz    = 1;
  so->data   = cPos4;      // Startpt der Area

  // pointer ersetzen
  ((SurBSpl*)so->data)->cpTab  = cPos1;
  ((SurBSpl*)so->data)->kvTabU = cPos2;
  ((SurBSpl*)so->data)->kvTabV = cPos3;

  return Ind;

}

//================================================================
  long DB_StoreSurStripe (long Ind, SurStripe *si) {
//================================================================
/// save Rat.BspSur s1 --> su_tab[Ind], Daten nach DB_CSEG.

  long     i1;
  ObjGX    *so;
  void     *cPos1, *cPos2;


  // printf("DB_StoreSurStripe %ld\n",Ind);
  // DEB_dump_obj__ (Typ_SURSTRIP, si, "DB_StoreSurStripe %ld\n",Ind);
    
  // get parent-record so
  Ind = DB_Store_hdr_su (&so, Ind);

  // Startpos im mem
  cPos1 = DB_cPos ();

  // save all points
  i1 = sizeof(ObjGX) * si->ptUNr * si->ptVNr;
  cPos2 = DB_cSav (i1, (void*)si->pTab);

  // save sur
  DB_cSav (sizeof(SurStripe), (void*)si);

  // create parent-record so
  so->typ    = Typ_SURSTRIP;
  so->form   = Typ_SURSTRIP;
  so->siz    = 1;
  so->data   = cPos2;      // Startpt of surf

  // replace pointers
  ((SurStripe*)so->data)->pTab  = cPos1;

  return Ind;

}


//================================================================
  long DB_StoreSurRBsp (long Ind, SurRBSpl *si) {
//================================================================
/// save Rat.BspSur s1 --> su_tab[Ind], Daten nach DB_CSEG.

  long     i1;
  ObjGX    *so;
  void     *cPos1, *cPos2, *cPos3, *cPos4, *cPos5;

  // printf("DB_StoreSurRBsp %d\n",Ind);
  // DEB_dump_obj__ (Typ_SURRBSP, si, "DB_StoreSurBsp %d\n",Ind);


  Ind = DB_Store_hdr_su (&so, Ind);

  // Startpos im mem
  cPos1 = DB_cPos ();

  // die cpTab der SurBSpl speichern
  i1 = sizeof(Point) * si->ptUNr * si->ptVNr;
  cPos2 = DB_cSav(i1, (void*)si->cpTab);

  // die kvTabU der SurBSpl speichern
  i1 = sizeof(double) * (si->ptUNr + si->degU + 1);
  cPos3 = DB_cSav(i1, (void*)si->kvTabU);

  // die kvTabV der SurBSpl speichern
  i1 = sizeof(double) * (si->ptVNr + si->degV + 1);
  cPos4 = DB_cSav(i1, (void*)si->kvTabV);

  // sie wTab speichern
  i1 = sizeof(double) * si->ptUNr * si->ptVNr;
  cPos5 = DB_cSav(i1, (void*)si->wTab);

  // SurBSpl speichern
  DB_cSav(sizeof(SurRBSpl), (void*)si);


  so->typ    = Typ_SURRBSP;
  so->form   = Typ_SURRBSP;
  so->siz    = 1;
  so->data   = cPos5;      // Startpt der Area

  // pointer ersetzen
  ((SurRBSpl*)so->data)->cpTab  = cPos1;
  ((SurRBSpl*)so->data)->kvTabU = cPos2;
  ((SurRBSpl*)so->data)->kvTabV = cPos3;
  ((SurRBSpl*)so->data)->wTab   = cPos4;

  return Ind;

}


//================================================================
  long DB_StoreSurRV (long Ind, SurRev *si) {
//================================================================
/// save RevSur s1 --> su_tab[Ind], Daten nach DB_CSEG.

  ObjGX    *so;

  // printf("DB_StoreSurRV %d\n",Ind);
  // DEB_dump_obj__ (Typ_SURRV, si, "DB_StoreSurRV %d\n",Ind);

  Ind = DB_Store_hdr_su (&so, Ind);

  so->typ    = Typ_SURRV;
  so->form   = Typ_SURRV;
  so->siz    = 1;
  so->data   = DB_cPos ();

  // save struct
  DB_cSav(sizeof(SurRev), (void*)si);

  return Ind;

}


//======================================================================
  int DB_StoreSur (long *IndIn, ObjGX *ox1) {
//======================================================================
//   IndIn     >0  store with this objID;
//             -1  store dynamic, return next free dynamic plane-index
//
// store as complex-obj

  int       irc, sSiz;
  long      Ind, i1, dbi, dSiz, l1;
  void      *cPos1, *cPos2, *cPos3, *cPos4, *cPos5;
  ObjGX     *oxo;
  SurStripe *sus1;
  SurBSpl   *sub1;


  // printf("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS \n");
  // printf("DB_StoreSur %ld typ=%d form=%d\n",*IndIn,ox1->typ,ox1->form);


  Ind = *IndIn;

  if(ox1->siz < 1) {          // Typ_SURPTAB/Typ_NULL(A=PTAB)
    *IndIn = DB_Store_hdr_su (&oxo, Ind); // oxo zeigt nun in die su_tab
    goto L_Dummy;         // A=PTAB
  }


  //----------------------------------------------------------------
  // new 2014-01-25
  // TODO; use it for Typ_SURRV(SurRev) Typ_SURHAT(SurHat)
  //         Typ_SURBSP(SurBSpl) *kvTabU, *kvTabV *cpTab ???????
  if(ox1->typ == Typ_SURRV)    { sSiz = sizeof (SurSwp); goto L_sav1; }
  if(ox1->typ == Typ_SURSWP)   { sSiz = sizeof (SurSwp); goto L_sav1; }



  //----------------------------------------------------------------
  // old
  if((ox1->typ == Typ_SUR)      ||
     (ox1->typ == Typ_SUTP)     ||
     (ox1->typ == Typ_SURSUP)   ||
     (ox1->typ == Typ_SURRU)    ||
     (ox1->typ == Typ_SURBSP)   ||
     (ox1->typ == Typ_SURRBSP)  ||
     (ox1->typ == Typ_SURCIR)   ||
     (ox1->typ == Typ_SURSTRIP) ||
     (ox1->typ == Typ_SURPMSH)  ||
     (ox1->typ == Typ_SURPTAB)  ||
     (ox1->typ == Typ_SURHAT))     {

    // get IndIn = index into isu_dyn|su_tab;
    // get oxo = addr of su_tab[Ind]
    *IndIn = DB_Store_hdr_su (&oxo, Ind);

    return DB_store_ox (oxo, ox1, IndIn); // store surf -> DB

/*  //----------------------------------------------------------------
    cPos1 = DB_cPos ();
    *oxo = *ox1;    // copy ParentObj
    oxo->data = cPos1;
    l1 = Ind;
    irc = DB_store_obj (&cPos1, Typ_ObjGX, ox1->data, ox1->siz, &l1);
*/
      // DEB_dump_ox_s_ (oxo, "DB_StoreSur-out\n");
      // DEB_dump_ox_0 (oxo, "DB_StoreSur-out\n");
      // exit(0);
  }


  *IndIn = DB_Store_hdr_su (&oxo, Ind);
  // oxo zeigt nun in die su_tab.


  if(ox1->typ == Typ_SURCIR)   goto L_SurCir;
  if(ox1->typ == Typ_SURSTRIP) goto L_SurStripe;
  if(ox1->typ == Typ_SURHAT)   goto L_SurHat;

  TX_Error("DB_StoreSur E002 %ld %d",Ind,ox1->typ);
  return -1;




  //---------------------- save Dummy ----------------------------
  // nur den Platz besetzen (zB fuer WCUT-Objekte)
  L_Dummy:
  *oxo        = *ox1;
  oxo->data   = NULL;

  goto L_fertig;






  //================================================================
  //---------------------- save hatch  ----------------------------
  L_SurHat:


  oxo->typ   = ox1->typ;
  oxo->form  = ox1->form;
  oxo->siz   = ox1->siz;
  oxo->data  = DB_cPos ();

  cPos1 = DB_cSav (sizeof(SurHat), ox1->data);

  goto L_fertig;





  //---------------------- save SurCir  --------------------------------
  L_SurCir:
  // suc1 = ox1->data;

  if(ox1->form != Typ_Index) {
    TX_Error("DB_StoreSur E003");
    return -1;
  }

  // Startpos im mem merken
  cPos1 = DB_cPos ();
  // die ptiTab der SurCir speichern
  cPos2 = DB_cSav((sizeof(long) * ox1->siz), ox1->data);


  *oxo        = *ox1;
  oxo->data   = cPos1;      // Startpt der SurCir


  goto L_fertig;




  //---------------------- save SurStripe  --------------------------------
  L_SurStripe:
/*
  sus1 = ox1->data;

  // Startpos im mem merken
  cPos1 = DB_cPos ();
  // die ptiTab der SurStripe speichern
  i1 = sizeof(long) * (sus1->ptUNr * sus1->ptVNr);
  cPos2 = DB_cSav(i1, (void*)sus1->ptiTab);
  // ptiTab-Adresse in der SurStripe ersetzen
  sus1->ptiTab = cPos1;
  // SurStripe speichern
  cPos3 = DB_cSav(sizeof(SurStripe), (void*)sus1);

  *oxo        = *ox1;
  oxo->data   = cPos2;      // Startpt der SurStripe

  goto L_fertig;
*/


  //----------------------------------------------------------------
  // save struct; struct has no extra-data
  L_sav1:

    // get IndIn = index into su_dyn|su_tab;
    // get oxo = addr of su_tab[Ind]
    *IndIn = DB_Store_hdr_su (&oxo, Ind);

    // write su_dyn|su_tab-record
    OGX_SET_OBJ (oxo, ox1->typ, ox1->form, 1, DB_cPos());

    // save struct
    cPos1 = DB_cSav (sSiz, ox1->data);
    if(!cPos1) return -1;
    goto L_fertig;



  //----------------------------------------------------------------
  L_fertig:
    // printf("ex DB_StoreSur %ld\n",*IndIn);
  return 0;

}



//======================================================================
  ObjGX* DB_GetSur (long Ind, int mode) {
//======================================================================
/// \code
/// mode=0: silent; do not display Errors
/// mode=1: display Errors
/// Error (surf not defined): (ObjGX*)->typ == Typ_Error) !
/// \endcode


  // int  i1;
  // long *itab;
  ObjGX       *su_out;

  // printf("DB_GetSur %d\n",Ind);


  if(Ind < 0) {
    Ind = -Ind;
    if (Ind >= DYN_SU_SIZ) {
      if(mode > 0) TX_Error("- Zugriff auf dyn. Surf %d ****",Ind);
      return &su_tab[0];
    }
    su_out = &su_dyn[Ind];

  } else {
    if (Ind >= APT_SU_SIZ) {
      if(mode > 0) TX_Error("- Zugriff auf Surf %d ****",Ind);
      return &su_tab[0];
    }
    su_out = &su_tab[Ind];
    // printf(" GetSur-typ%d form%d siz%d\n",
                 // su_out->typ,su_out->form,su_out->siz);
  }

  // if(su_out->typ == Typ_Error) {
  if(DB_isFree_Sur (su_out)) {
    if(mode > 0) TX_Error (" Surf %d undefined",Ind);
    su_out = &su_tab[0];
  }

  
/*
  //---- Anf Testausg:
  printf("  ptNr=%d\n",su_tab[Ind].ptNr);
  itab = su_tab[Ind].ptiTab;
  for(i1=0; i1<su_tab[Ind].ptNr; ++i1) {
    printf("  %d %d\n",i1,itab[i1]);
  }
  //---- End Testausg:
*/

  return su_out;

}


//======================================================================
  int DB_StoreModel (long Ind, ObjGX *md1) {
//======================================================================
// DB_StoreModel                          store refModel (Typ_Model)
// no dynam. models

  // printf("DB_StoreModel %ld %d %d\n",Ind,md1->typ,md1->form);
  // DEB_dump_obj__ (Typ_Model, md1->data, " ModelRef: ");


  if(Ind < 0) {
    TX_Error("DB_StoreModel E001 %d",Ind);
    return -1;


  //---------------- normal ---------------------------
  } else {

    if(Ind > APT_MR_IND) {
      APT_MR_IND = Ind;

      if(Ind >= APT_MR_SIZ) {    // realloc
        if(DB_allocModRef (Ind) < 0) return 0;
      }
    }

    mdr_tab[Ind] = *(ModelRef*)md1->data;

  }

  return 0;

}


//================================================================
  int DB_del_MR__ (long dbi) {
//================================================================
// DB_del_MR__                     delete refModel
// Input:
//   dbi      DB-index refModel to clear; -1 = clear all refModels


  if(dbi < 0L) {
    return DB_del_ModRef (0L);
  }


  DB_setFree_MR (dbi);

  return 0;

}

 
//================================================================
  int DB_del_ModRef (int iMdr) {
//================================================================
// DB_del_ModRef      delete refModel and all following
// Input:
// Output:
//   retCode      0=OK, -1=err.
// was ..


  int   i1;

  // printf("DB_del_ModRef %d - %ld\n",iMdr,APT_MR_IND);

  for(i1=iMdr; i1<= APT_MR_IND; ++i1) DB_setFree_MR (i1);
  APT_MR_IND = iMdr;

  return 0;

}


//================================================================
int DB_del_Mod__ () {
//================================================================
// DB_del_Mod         delete all refModels and all basicModels

  long   i1;

  // printf(">>>>>>>>>>>>>>> DB_del_Mod__ %ld %ld \n",APT_MR_IND,APT_MR_IND);

  // delete all refModels
  for(i1=0; i1<= APT_MR_IND; ++i1) DB_setFree_MR (i1);
  APT_MR_IND = 0;


  // delete all refModels
  DB_StoreModBas (0, NULL);  // kill (reset) BasicModels

  return 0;

}


//=========================================================================
  int Mod_mdr__bmi_pln (ModelRef *mdr, int bmi, Plane *pl1, double scale) {
//=========================================================================
// Mod_mdr__bmi_pln    set modelRef from basicModelNr and refSys
// Input:
//   bmi     basicModelNr
//   scale
//   pl1     origin po, vx, vz used


  mdr->modNr = bmi;            // basicModelNr
  mdr->scl   = scale;
  mdr->po    = pl1->po;
  mdr->vx    = pl1->vx;
  mdr->vz    = pl1->vz;

  return 0;

}


//======================================================================
  ModelRef* DB_get_ModRef (long Ind) {
//======================================================================
/// DB_get_ModRef      get Ditto from Index
// Input:
//   Ind       the DB-index of the submodel (eg 20 for subModel M20)


  // printf("DB_get_ModRef %ld from %ld\n",Ind,APT_MR_SIZ);


  if((Ind < 0)||(Ind >= APT_MR_SIZ)) {
    TX_Error("DB_get_ModRef E001 %d",Ind);
    return &mdr_tab[0];
  }


  // if(mdr_tab[Ind].po.x == UT_VAL_MAX) {
   if(DB_isFree_ModRef (&mdr_tab[Ind])) {
    TX_Error (" ModRef %d undefined",Ind);
    return &mdr_tab[0];
  }

    // DEB_dump_obj__ (Typ_Model, &mdr_tab[Ind], "mr:");

  return &mdr_tab[Ind];

}


//======================================================================
  int DB_dump_Activ () {
//======================================================================
/// DB_dump_Activ    dump all activities
  int i1;

  printf(" DB_dump_Activ\n");

  for(i1=0; i1<APT_AC_SIZ; ++i1) {
    // if(ac_tab[i1].typ == Typ_Error) continue;
    if(DB_isFree_Act (&ac_tab[i1])) continue;
    DEB_dump_obj__ (Typ_Activ, &ac_tab[i1], " ac_tab[%d]",i1);
  }

  return 0;

}


//================================================================
  int DB_dump_f (FILE *fpo, int typ) {
//================================================================
/// \code
/// dump all objects of type<typ> into open file
/// see also DEB_dump_dbo
/// \endcode


  int    i1;
  long   l1, ii;
  double *d1;
  char   cbuf[64];
  Vector *vc1;
  ObjGX  *ox1;


  if(typ == Typ_VAR) {
    for(ii=1; ii<=APT_VR_IND; ++ii) {
      // if(vr_tab[ii] == UT_VAL_MAX) continue;
      if(DB_isFree_Var (&vr_tab[ii])) continue;
      fprintf(fpo," V[%ld] = %9.3f\n",ii,vr_tab[ii]);
    }


  } else if(typ == Typ_VC) {
    for(ii=1; ii<=APT_VC_IND; ++ii) {
      // if(vc_tab[ii].dx == UT_VAL_MAX) continue;
      if(DB_isFree_VC (&vc_tab[ii])) continue;
      vc1 = &vc_tab[ii];
      fprintf(fpo," D[%ld] = %9.3f,%9.3f,%9.3f\n",ii,vc1->dx,vc1->dy,vc1->dz);
    }


  } else if(typ == Typ_PT) {
    for(ii=1; ii<=APT_PT_IND; ++ii) {
      // if(vc_tab[ii].dx == UT_VAL_MAX) continue;
      if(DB_isFree_PT (&pt_tab[ii])) continue;
      fprintf(fpo," P[%ld] = %9.3f,%9.3f,%9.3f\n",ii,
              pt_tab[ii].x,pt_tab[ii].y,pt_tab[ii].z);
    }



  } else if(typ == Typ_Tra) {
    for(ii=1; ii<=APT_TR_IND; ++ii) {
      // if(tra_tab[ii].typ == Typ_Error) continue;
      if(DB_isFree_Tra (&tra_tab[ii])) continue;
      ox1 = &tra_tab[ii];
      if(ox1->form == Typ_VC) {
        vc1 = (Vector*)ox1->data;
        fprintf(fpo," T[%ld] = Vec. %9.3f,%9.3f,%9.3f\n",ii,
                vc1->dx,vc1->dy,vc1->dz);
      } else if(ox1->form == Typ_TraRot) {
        vc1 = &((TraRot*)ox1->data)->vz;
        d1 =  &((TraRot*)ox1->data)->angr;
        fprintf(fpo," T[%ld] = Axis %9.3f,%9.3f,%9.3f   Ang. = %f\n",ii,
                vc1->dx,vc1->dy,vc1->dz,*d1);
      }
    }


  } else if(typ == Typ_Activ) {
    for(ii=1; ii<=APT_AC_IND; ++ii) {
      // if(ac_tab[ii].typ == Typ_Error) continue;
      if(DB_isFree_Act (&ac_tab[ii])) continue;
      i1 = ac_tab[ii].typ;
      l1 = ac_tab[ii].ind;
      APED_oid_dbo__ (cbuf, i1, l1);
      fprintf(fpo," I[%ld] on %s = |%s|\n",ii,cbuf,ac_tab[ii].data);
    }



  } else if(typ == Typ_TEXR) {
    Tex_dump__ (fpo);



  } else if(typ == Typ_Model) {



  } else fprintf(fpo, "Error E001 %d\n",typ);


  return 0;

}

 
//================================================================
  long DB_dyn__ (int mode, int typ, long ind) {
//================================================================
/// \code
/// DB_dyn__           get/(re)set dynamic space for <typ>
/// Input:
///   mode      0=save DYN_<typ>_SIZ here and return pointer
///             1=(re)set from local stored
///             2=(re)set pointer to <ind>  (not for Typ_goGeom)
///   typ       Typ_VC|Typ_PT|Typ_LN|Typ_CI|Typ_goGeom
///   ind       only for mode=2
/// \endcode


  static long ptd=6, pti=0, ptl=0, pta=0, ptp=3;


  // printf("DB_dyn__ %d %d %ld \n",mode,typ,ind);
  // printf(" %ld %ld %ld\n",pti,ptl,pta);
  // printf(" %ld %ld %ld\n",DYN_PT_IND,DYN_LN_IND,DYN_CI_IND);



  //----------------------------------------------------------------
  if(mode == 0) {              // save pointer in pti; return pointer
    switch (typ) {
      case Typ_VC:
        ptd = DYN_VC_IND;
        return ptd;

      case Typ_PT:
        pti = DYN_PT_IND;
          // printf(" _dyn__-store-DYN_PT_IND=%ld\n",pti);
        return pti;

      case Typ_LN:
        ptl = DYN_LN_IND;
        return ptl;

      case Typ_CI:
        pta = DYN_CI_IND;
        return pta;

      case Typ_PLN:
        ptp = DYN_PL_IND;
        return ptp;

      default:
        ptd = DYN_VC_IND;
        pti = DYN_PT_IND;
        ptl = DYN_LN_IND;
        pta = DYN_CI_IND;
        ptp = DYN_PL_IND;
    }
    return 0L;

  //----------------------------------------------------------------
  } else if(mode == 1) {     // 1=(re)set from local stored
    if(pti < 0) {TX_Error("DB_dyn__ E001\n"); return -1L; }
    switch (typ) {
      case Typ_VC:
        DYN_VC_IND = ptd;
        return DYN_VC_IND;
      case Typ_PT:
        DYN_PT_IND = pti;
          printf(" _dyn__-reset-DYN_PT_IND=%ld\n",pti);
        return DYN_PT_IND;
      case Typ_LN:
        DYN_LN_IND = ptl;
        return DYN_LN_IND;
      case Typ_CI:
        DYN_CI_IND = pta;
        return DYN_CI_IND;
      case Typ_PLN:
        DYN_PL_IND = ptp;
        return DYN_PL_IND;
      default:
        DYN_VC_IND = ptd;
        DYN_PT_IND = pti;
        DYN_LN_IND = ptl;
        DYN_CI_IND = pta;
        DYN_PL_IND = ptp;
        return 0L;
    }


  //----------------------------------------------------------------
  } else {      // 2=(re)set pointer to <ind>
    switch (typ) {
      case Typ_VC:
        DYN_VC_IND = ind;
        return ind;
      case Typ_PT:
        DYN_PT_IND = ind;
        return ind;
      case Typ_LN:
        DYN_LN_IND = ind;
        return ind;
      case Typ_CI:
        DYN_CI_IND = ind;
        return ind;
      case Typ_PLN:
        DYN_PL_IND = ind;
        return ind;
      default:
        TX_Error("DB_dyn__ E001 %d\n",typ);
        return -1L;
    }
  }



  return DYN_PT_IND;

}


//================================================================
  int DB_dump_dyn__ (int typ) {
//================================================================
// DB_dump_dyn__      dump dynamic
// typ yet unused


  printf("Points: %ld used from %ld\n",DYN_PT_IND,DYN_PT_SIZ);

  return 0;

}


//======================================================================
  int DB_dump__ () {
//======================================================================
/// dump complete DB



  int     i1, typ, form;
  char    *p1;

  printf("DB_dump__\n");



  printf(" %ld points:\n",APT_PT_SIZ);
  for(i1=1; i1<APT_PT_SIZ; ++i1) {
    // if(pt_tab[i1].x == UT_VAL_MAX) continue;
    if(DB_isFree_PT (&pt_tab[i1])) continue;
    DEB_dump_obj__(Typ_PT, &pt_tab[i1], "  %d ",i1);
  }


  printf(" %ld lines:\n",APT_LN_SIZ);
  for(i1=1; i1<APT_PT_SIZ; ++i1) {
    // if(ln_tab[i1].p1.x == UT_VAL_MAX) continue;
    if(DB_isFree_LN (&ln_tab[i1])) continue;
    DEB_dump_obj__(Typ_LN, &ln_tab[i1], "  %d ",i1);
  }


  printf(" %ld circs:\n",APT_CI_SIZ);
  for(i1=1; i1<APT_CI_SIZ; ++i1) {
    // if(ci_tab[i1].p1.x == UT_VAL_MAX) continue;
    if(DB_isFree_CI (&ci_tab[i1])) continue;
    DEB_dump_obj__(Typ_CI, &ci_tab[i1], "  %d ",i1);
  }


  printf(" %ld curves\n",APT_CV_SIZ);
  for(i1=1; i1<APT_CV_SIZ; ++i1) {
    // if(cv_tab[i1].typ == Typ_Error) continue;
    if(DB_isFree_CV (&cv_tab[i1])) continue;
    // typ = cv_tab[i1].form;
    typ  = cv_tab[i1].typ;
    form = cv_tab[i1].form;
    p1 = AP_src_typ__(typ);
    if(form != Typ_ObjGX) {
      DEB_dump_obj__ (form, cv_tab[i1].data, "  %d %s",i1,p1);
    } else {  // CCV
      printf("  %d %s ===========================\n",i1,p1);
      DEB_dump_ox_1 (&cv_tab[i1], "");
    }
  }


  printf(" %ld surfaces\n",APT_SU_SIZ);
  for(i1=1; i1<APT_SU_SIZ; ++i1) {
    // if(su_tab[i1].typ == Typ_Error) continue;
    if(DB_isFree_Sur (&su_tab[i1])) continue;
    typ  = su_tab[i1].typ;
    form = su_tab[i1].form;
    p1 = AP_src_typ__(typ);
    if(form != Typ_ObjGX) {
      DEB_dump_obj__ (form, su_tab[i1].data, "  %d %s",i1,p1);
    } else {  // CCV
      printf("  %d %s ===========================\n",i1,p1);
      DEB_dump_ox_1 (&su_tab[i1], "");
    }
  }


  printf(" %ld ModelReferences:\n",APT_MR_SIZ);
  for(i1=0; i1<APT_MR_SIZ; ++i1) {
    // if(mdr_tab[i1].po.x == UT_VAL_MAX) continue;
    if(DB_isFree_ModRef (&mdr_tab[i1])) continue;
    DEB_dump_obj__ (Typ_Model, &mdr_tab[i1], "  %3d ",i1);
  }

  
  DB_dump_ModBas ("");



  return 0;

}


// //================================================================
//   int DB_dump_ModNam () {
// //================================================================
// // mdb_nam keeps the names of active (sub)model and its subModels;
// // not of all models.
// 
//   int   i1, i2, ii;
// 
//   printf("--------------- DB_dump_ModNam DB_MNM_IND = %ld\n",DB_MNM_IND);
// 
//   ii = 0;
//   i2 = 0;
// 
//   L_nxt:
//     i1 = strlen(&mdb_nam[i2]);
//     printf("%5d |%s|\n",ii, &mdb_nam[i2]);
//     i2 += i1 + 1;
//     ++ii;
//     if(i2 < DB_MNM_IND) goto L_nxt;
// 
// 
//   return 0;
// 
// }

 
//================================================================
  int DB_dump_ModBas (char *txt) {
//================================================================
/// dump basic-models mdb_dyn
 
  int  i1;

  printf("%s - %ld basic models:\n",txt,DYN_MB_IND);
  for(i1=0; i1<DYN_MB_IND; ++i1) {
    DEB_dump_obj__ (Typ_SubModel, &mdb_dyn[i1], "  %d ",i1);
  }

  DEB_dump_obj__ (Typ_Txt, "\nMainmodel:", "");
  DEB_dump_obj__ (Typ_PT, &AP_box_pm1, " pb1 ");
  DEB_dump_obj__ (Typ_PT, &AP_box_pm2, " pb2 ");

  return 0;

}


//================================================================
  int DB_dump_ModRef (char *txt) {
//================================================================

  int  i1;

  printf("%s - %ld reference models\n",txt,APT_MR_IND);
  // for(i1=0; i1<APT_MR_IND; ++i1) {
  for(i1=0; i1<=APT_MR_IND; ++i1) {
    // if(mdr_tab[i1].po.x == UT_VAL_MAX) continue;
    if(DB_isFree_ModRef (&mdr_tab[i1])) continue;
    DEB_dump_obj__ (Typ_Model, &mdr_tab[i1], "  M%d ",i1);
  }

  return 0;

}


//================================================================
  char* DB_mdlNam_iBas (int bmNr) {
//================================================================
/// DB_mdlNam_iBas    get the name of a basic-model from its basicModelNr
/// Input:
///   bmNr     nr of its basic-model

  // printf("DB_mdlNam_iBas %d DYN_MB_IND=%ld \n",bmNr,DYN_MB_IND);

  if(bmNr >= DYN_MB_IND) {
    TX_Print("DB_mdlNam_iBas E1-%d",bmNr);
    return NULL;

  } else if(bmNr < 0) {
    // mainModel active
    return "-";
  }

  return mdb_dyn[bmNr].mnam;

}


//================================================================
  int DB_mdlNam_iRef (char **mdlNam, long dbi) {
//================================================================
/// get Modelname from DB-index of the Modelreference
/// returns a pointer to the modelname ..


  ModelRef  *mra;


  if(APT_MR_IND < dbi) return -1;

  // check if free or used
  mra = mdr_tab;
  if(DB_isFree_ModRef (&mra[dbi])) return -1;

    // DEB_dump_obj__ (Typ_Model, &mra[i1], "  M%d ",i1);
  *mdlNam = DB_mdlNam_iBas (mra[dbi].modNr);
    // printf("M%d = %s\n",dbi,*mdlNam);

  return 0;

}

//================================================================
  int DB_mdlTyp_iBas (int *mdlTyp, int mbNr) {
//================================================================
// DB_mdlTyp_iBas     get Modeltyp from its basicModelNr
// RetCod:   -3 = error
//           else see MBTYP_INTERN ..


  ModelRef    *mra;
  ModelBas    *mb;


  if((mbNr < 0)||(mbNr >= DYN_MB_IND)) return -3;


  *mdlTyp = mdb_dyn[mbNr].typ;

    printf("ex DB_mdlTyp_iBas typ=%d mbNr=%d\n",*mdlTyp,mbNr);

  return 0;

}


//================================================================
  int DB_mdlTyp_iRef (int *mdlTyp, long dbi) {
//================================================================
// DB_mdlTyp_iRef     get Modeltyp from DB-index of the Modelreference


  ModelRef    *mra;
  ModelBas    *mb;


  if(APT_MR_IND < dbi) return -1;

  // check if free or used
  mra = mdr_tab;
  if(DB_isFree_ModRef (&mra[dbi])) return -1;

    // DEB_dump_obj__ (Typ_Model, &mra[i1], "  M%d ",i1);
  mb = DB_get_ModBas (mra[dbi].modNr);

  *mdlTyp = mb->typ;
    // printf("M%d = %s\n",dbi,*mdlNam);

  return 0;

}


/* UU
//================================================================
  int DB_ModBas_seqMax () {
//================================================================
/// DB_ModBas_seqMax   get highest seqNr

  int   i1, actSeq;

  actSeq = 0;

  for(i1=0; i1<DYN_MB_IND; ++i1) {
    if(mdb_dyn[i1].seqNr > actSeq) actSeq = mdb_dyn[i1].seqNr;
  }

  // printf("ex DB_ModBas_seqMax %d\n",actSeq);
  return actSeq;

}


// UU
//======================================================================
  int DB_list_ModBas () {
//======================================================================
// in BasicModels gibts Reihenfolgenummer seqNr.
// Diese in korrekter Reihenfolge -> Datei <tmpdir>/Mod.lst ausgeben.
// irc: Anzahl Models; (<0: Error).
// used by AP_ExportIges__

  int      i1, i2;
  char     cbuf[256];
  FILE     *fp1;

  printf("DB_list_ModBas   %ld\n",DYN_MB_IND);


  sprintf(cbuf,"%sMod.lst",AP_get_tmp_dir());
  if((fp1=fopen(cbuf,"w+")) == NULL) {
    TX_Print("DB_list_ModBas E001 %s",cbuf);
    return -1;
  }

  i1 = -1;

  L_next:
  ++i1;

    for(i2=0; i2<DYN_MB_IND; ++i2) {
      if(mdb_dyn[i2].seqNr != i1) continue;
        printf("  %d seq=%d %s\n",i2,mdb_dyn[i2].seqNr,mdb_dyn[i2].mnam);
      fprintf(fp1, "%s\n",mdb_dyn[i2].mnam);
      goto L_next;
    }



  fclose(fp1);

  return i1;

}
*/


//================================================================
  int DB_del_mdb_syFn  (char *mbNam) {
//================================================================
// DB_del_mdb_syFn        delete baseModel             
// Input:
//   mbNam    symbolic-name

  int      bmi, ii;

  printf("DB_del_mdb_syFn  |%s|\n",mbNam);

  // get index of baseModel mbNam
  bmi = MDL_imb_mNam (mbNam, 0);
  if(bmi < 0) {TX_Print("***** DB_del_mdb_syFn I001 %s",mbNam);return -1;}
    

  // copy last mdb over mdb to delete; --mdbNr
  ii = DYN_MB_IND - 1;
    printf(" del_mdb_syFn- bmi=%d ii=%d\n",bmi,ii);
  if(bmi < ii) mdb_dyn[bmi] = mdb_dyn[ii];
  DYN_MB_IND -= 1;

  return 0;

}


//================================================================
  int DB_clear_ModBas (long ind) {
//================================================================
// DB_clear_ModBas                 clear (reset) single basicModel
 
  printf("DB_clear_ModBas %ld\n",ind);

  mdb_dyn[ind] = _MODELBAS_NUL;

  return 0;

}


//======================================================================
  int DB_del_ModBas (long ind) {
//======================================================================
// DB_del_ModBas             delete baseModel and all following baseModels
// Input:
//   ind          >=0 delete baseModel and all following baseModels
//                -1  clear all baseModels

  int     i1;
  long    dlBis;
  

  // printf("XXXXXXXXXXXXXXXXXXXXXXX DB_del_ModBas %ld\n",ind);

  if(ind < 0L) {DYN_MB_IND = 0; return 0;}

  for(i1=DYN_MB_IND-1; i1>=0; --i1) {
    // printf("  %d DLind=%d DLsiz=%d\n",i1,mdb_dyn[i1].DLind,mdb_dyn[i1].DLsiz);
    dlBis = mdb_dyn[i1].DLind + mdb_dyn[i1].DLsiz;
    if(ind < dlBis) {
        // printf(".... delete BaseModel %d IND=%d\n",i1,DYN_MB_IND);
      mdb_dyn[i1].mnam  = NULL;
      mdb_dyn[i1].DLind = -1;
      mdb_dyn[i1].DLsiz = -1;
      mdb_dyn[i1].typ   = 0;
      if(i1+1 == DYN_MB_IND)  // last BasModel !!
        --DYN_MB_IND;
    }
  }

  return 0;

}

/*
//======================================================================
  int DB_allocModNod () {
//======================================================================

  MNOD_SIZ += MNOD_INC;

  mNod = (ModelNode*)realloc(mNod, MNOD_SIZ*sizeof(ModelNode));

  if(mNod == NULL) {
    TX_Error ("******** out of memory - DB_allocModNod *********");
    return -1;
  }

  return 0;

}


//======================================================================
  int DB_StoreModNod (int mod, int par, long ind) {
//======================================================================
// DB_StoreModNod              store basicModelNode (hierarchy)
// mNod is a list of all submodels of the active-model. Permanent.
// mNod keeps the sequence of loading basicModels - highest <par> first.
// mNod goes down from the primaryModel (not mainModel)
//   mod    der basicModelIndex
//   par    the nodeIndex of the parent of the model. -1=topLevel.
//   ind    the refModelIndex (= DB-index) in the baseModelfile
// Get the modelfileName from mdb_dyn[mNod[<index>].mod]


  int recNr;

  // printf("DB_StoreModNod %d %d %d\n",mod,par,ind);


  // reset
  if(ind < 0) {
    MNOD_IND = 0;
    return 0;
  }


  if(MNOD_IND >= MNOD_SIZ) {
    if(DB_allocModNod () < 0) return -1;
  }

  // create a new mdb_dyn-Record.
  mNod[MNOD_IND].mod = mod;
  mNod[MNOD_IND].par = par;
  mNod[MNOD_IND].ind = ind;

  recNr = MNOD_IND;
  ++MNOD_IND;

  return recNr;

}


//================================================================
  ModelNode* DB_get_ModNod (int ind) {
//================================================================

  return &mNod[ind];

}


//================================================================
  int DB_dump_ModNod () {
//================================================================

  int       i1;
  ModelBas  *mb1;

  printf(" DB_dump_ModNod %d Nodes\n",MNOD_IND);

  for(i1=0; i1<MNOD_IND; ++i1) {
    mb1 = &mdb_dyn[mNod[i1].mod];   // basicModel
    printf(" %4d mod=%3d par=%2d ind=%3d  %s\n",i1,
             mNod[i1].mod,mNod[i1].par,mNod[i1].ind,mb1->mnam);
  }

  return 0;

}


//================================================================
  int DB_ModNod_find (int imb) {
//================================================================
// parentnode for basicModel imb suchen..
// hier fehlt wahrscheinl noch ein Input !!! um richtigen node zu finden ..
// zum eindeutigen Finden braucht man zusaetzlich WAS ? 
// sucht dzt nur ersten Node mit (mod == imb) !

  int   i1;

  for(i1=0; i1<MNOD_IND; ++i1) {
    if(mNod[i1].mod == imb) {
      printf("ex DB_ModNod_find %d %d\n",i1,imb);
      return i1;
    }
  }

  TX_Error("DB_ModNod_find E001 %d",imb);
  return -1;

}

// TODO: use in MDL ..
//================================================================
  int DB_ModNod_ckLoop1 (int ibm, int errbm) {
//================================================================
// ipar = par(actNode)
// Loop ueber alle Nodes {
//   if(ipar < 0) skip this node;
//   if(bm==ipar) {
//     if(ipar==iMod) Error;
//     recurse mit bm = ipar
//   }
// }
// Retcod -1   == OK;
// Retcod >= 0 == Error !


  int   i1, i2;

  for(i1=0; i1<MNOD_IND; ++i1) {
    if(mNod[i1].mod != ibm) continue;

    if(mNod[i1].par == errbm) goto L_err;

    // recurse;
    i2 = DB_ModNod_ckLoop1 (mNod[i1].par, errbm);
    if(i2 >= 0) return i2;
  }

  return -1;


  L_err:
    // printf("ex DB_ModNod_ckLoop1 -1 %d %d %d\n",i1,ibm,errbm);
    return i1;
}
 

//================================================================
  int DB_ModNod_ckLoop_ (int ii) {
//================================================================
/// \code
/// den aktuellen Zweig scannen, ob das Model schon vorkommt (Loop !)
/// parNr ist der NODE-Index des subModel, in dem mNam gerufen wird.
/// \endcode


  int       i1;
  ModelBas  *mb1, *mb2;
  // ModelRef  *mr;
 
  // printf("DB_ModNod_ckLoop_ node=%d\n",ii);
  // DB_dump_ModBas();
  // DB_dump_ModNod();


  // test if not yet deleted   2010-11-11
  mb1 = &mdb_dyn[ii];     // DB_get_ModBas (ii);
  if(mb1->mnam == NULL) return 0;  // skip this test; model deleted ..



  if(mNod[ii].par == mNod[ii].mod) {i1 = ii; goto L_Err;}

  i1 = DB_ModNod_ckLoop1 (mNod[ii].par, mNod[ii].mod);
  if(i1 >= 0) goto L_Err;

  return 0;


  L_Err:
    mb2 = &mdb_dyn[i1];     // DB_get_ModBas (ii);
    TX_Print("- in subModel %s:",mb1->mnam);
    TX_Error("- SubModel %s calls itself; Mod_get_namStore-E001",mb2->mnam);
    return -1;

}
*/

//======================================================================
  int DB_StoreModBas (int mdlTyp, char *newNam) {
//======================================================================
// DB_StoreModBas          store new basicModel
// check if SubModel exists. If not: create a new mdb_dyn-Record.
// Add libpath, Filename oder name of internal Model to ModNamTab.
// Add name also to mdb_dyn (if not yet present).
//
// Input:
//  newNam     modelName; must be static (pointer stored in mdl_names)
//             NULL=delete all basicModels and modelNames 
//  mdlTyp     eg MBTYP_EXTERN
// Output:
//  RC = ModelNumber; -1 = Error.
//
// BasicModels are stored in mdb_dyn; modelType (mode), 
//   first index in DL and nr of dl-records.
// // Modelnames are stored in mdb_nam



  int  modNr, iLen;
  char *cp1;


  // printf("DB_StoreModBas mdlTyp=%d |%s| DYN_MB_IND=%ld\n",
         // mdlTyp,newNam,DYN_MB_IND);


  //----------------------------------------------------------------
  if(newNam == NULL) {   // INIT
    // delete all base- and refModels see DB_del_Mod__
    DYN_MB_IND = 0;        // basicModels
//     DB_MNM_IND = 0;        // modelNames
    return 0;
  }


  //----------------------------------------------------------------
  // existiert name schon: - return.
  modNr = MDL_imb_mNam (newNam, 0);
  if(modNr >= 0) goto L_fertig;


  // Model newNam existiert noch nicht; neu anlegen.
  if(DYN_MB_IND >= DYN_MB_SIZ) {
    if(DB_allocModBas (DYN_MB_IND) < 0) {modNr= -1; goto L_fertig;}
  }


  //----------------------------------------------------------------
// TODO: use new nameBuffer MDLMP - mdNames; 
// remove DB_MNM_IND mdb_nam DB_MNM_SIZ DB_allocModNam

  // add newname into mdb_nam
  iLen = strlen(newNam);

//   if((DB_MNM_IND+iLen) >= DB_MNM_SIZ) {
//     if(DB_allocModNam (1) < 0) return -1;
//   }

  // create a new mdb_dyn-Record.
  modNr = DYN_MB_IND;
  ++DYN_MB_IND;


//   // save startpos of newNam
//   // mdb_dyn[modNr].mnam = &mdb_nam[DB_MNM_IND];
//   cp1 = &mdb_nam[DB_MNM_IND];
// 
//   // add newNam to textbuffer
//   // DB_cSav (strlen(newNam)+1, newNam);
//   strncpy(&mdb_nam[DB_MNM_IND], newNam, iLen);
//   DB_MNM_IND += iLen;
//   mdb_nam[DB_MNM_IND] = '\0';
//   ++DB_MNM_IND;
// 
//   newNam = cp1;


  //----------------------------------------------------------------
  // set BasModel-record
  mdb_dyn[modNr]        = _MODELBAS_NUL;
  mdb_dyn[modNr].typ    = mdlTyp;
  mdb_dyn[modNr].mnam   = newNam;


  L_fertig:

    // if(modNr >= 0)DEB_dump_obj__(Typ_SubModel,&mdb_dyn[modNr],"ex-DB_StoreModBas");
    // printf("ex-DB_StoreModBas modNr=%d\n",modNr);

  return modNr;

}


//======================================================================
  ModelBas* DB_get_ModBas (int Ind) {
//======================================================================
// DB_get_ModBas              get the basicModel with index <Ind>
// returns NULL for error (basic model does not exist)


  // printf("DB_get_ModBas %d von %ld\n",Ind,DYN_MB_IND);

  if((Ind < 0)||(Ind >= DYN_MB_IND)) {
    // TX_Error("DB_get_ModBas E001 %d",Ind);
    return NULL;
  }

    // DEB_dump_obj__ (Typ_SubModel, &mdb_dyn[Ind], "mb:");

  return &mdb_dyn[Ind];

}


//================================================================
  int DB_get_mdr (int *mdrNr, ModelRef **mdra) {
//================================================================
// DB_get_mdr             returns referenceModels
// Output:
//   mdrNr   last occupied ModelRef-index
//   mdra    table of referenceModels

  *mdrNr = APT_MR_IND;
  *mdra = mdr_tab;

  return 0;

}


//================================================================
  int DB_get_mdb (int *mdbNr, ModelBas **mdba) {
//================================================================
// DB_get_mdb         returns basicModels
// Output:
//   mdbNr   nr of  basicModels
//   mdba    table of basicModels

  *mdbNr = DYN_MB_IND;
  *mdba = mdb_dyn;

  return 0;

}


//================================================================
  int DB_get_ModBasNr () {
//================================================================
// get nr of basic models (next free index)

  return DYN_MB_IND;

}


//====================================================================
  int DB_get_iModBas_iModRef (long iModRef) {
//====================================================================
/// \code
/// get basic-ModelNr from the ReferenceModelNr
/// RC >= 0: ModelNr; Model is already loaded.
/// RC = -1: Model not yet loaded ..
/// \endcode

  // printf("DB_get_iModBas_iModRef %d %d\n",iModRef,APT_MR_IND);

  if(APT_MR_IND < iModRef) return -1;

  return mdr_tab[iModRef].modNr;

}


/* replaced by MDL_imb_mNam
//====================================================================
  int DB_mbi_mRefID (char *modNam) {
//====================================================================
/// \code
/// get basic-ModelNr from ModelReferenceID (eg "M20")
///   Modelname eg "M20"
/// RC >= 0: ModelNr; Model is already loaded.
/// RC = -1: Model not yet loaded ..
/// \endcode


  int   i1, typ;
  long  ind;

  // AP_dump_statPg ("DB_mbi_mRefID: ");


  // printf("DB_mbi_mRefID |%s| %ld\n",modNam,DYN_MB_IND);
  // DB_dump_ModBas();
  

  // check if modNam is ModelID ("M20");
  if(modNam[0] == 'M') {
    // test modNam, return dbi
    i1 = APED_dbo_oid (&typ, &ind, modNam);
    if(i1 != 0) goto L_decode_1;

      // give back <mmodelRef>.modNr
      // printf(" typ=%d ind=%d\n",typ,ind);
      // printf("ex DB_mbi_mRefID %d |%s|\n",mdr_tab[ind].modNr,modNam);
    return mdr_tab[ind].modNr;
  }



  L_decode_1:
  for(i1=0; i1<DYN_MB_IND; ++i1) {
      // printf(" test %d |%s|\n",i1,mdb_dyn[i1].mnam);
    if(mdb_dyn[i1].mnam == NULL) continue;      // kann NULL sein !
    if(strcmp(modNam, mdb_dyn[i1].mnam)) continue;
      // printf("ex DB_mbi_mRefID %d |%s|\n",i1,modNam);
    return i1;

  }

    printf("ex-DB_mbi_mRefID -1 |%s|\n",modNam);

  return -1;

}
*/

//================================================================
long DB_FindVector (Vector* vc1) {
//================================================================

  long i1;

  // printf("DB_FindVector\n");

  for(i1=0; i1<=APT_VC_SIZ; ++i1) {
    // if(vc_tab[i1].dx == UT_VAL_MAX) continue;    // Luecke.
    if(DB_isFree_VC (&vc_tab[i1])) continue;
    if((UTP_comp2db (vc1->dx, vc_tab[i1].dx, 0.1)) &&
       (UTP_comp2db (vc1->dy, vc_tab[i1].dy, 0.1)) &&
       (UTP_comp2db (vc1->dz, vc_tab[i1].dz, 0.1))) {
      // printf(" found %d\n",i1);
      return i1;
    }

  }

  return -1;

}


//================================================================
long DB_StoreVector (long Ind, Vector* vc1) {
//================================================================

  long dbi;


  // DEB_dump_obj__(Typ_VC, vc1, "DB_StoreVector %ld  ",Ind);
  // DB_test__();



  // Dynam. Objekt erzeugen -
  if(Ind < 0) {
    dbi = DB_GetDynInd(Typ_VC);
    vc_dyn[-dbi] = *vc1;


  //---------------- normal ---------------------------
  } else {

    if(Ind > APT_VC_IND) {
      APT_VC_IND = Ind;

      if(Ind >= APT_VC_SIZ) {    // realloc
        if(DB_allocVector (Ind) < 0) return 0;
      }
    }

    dbi = Ind;
    vc_tab[dbi] = *vc1;
  }




  // printf("ex DB_StoreVector %ld > %d %f,%f,%f\n",Ind,dbi,
          // vc1->dx,vc1->dy,vc1->dz);

  return dbi;

}







//====================================================================
  int DB_allocVAR (long Ind) {
//====================================================================
// realloc vr_tab


  long i1, newSiz;


  if((Ind == 0)&&(vr_tab != NULL)) {  // reInit
    newSiz = APT_VR_SIZ;
    APT_VR_SIZ = 0;
    APT_VR_IND = 0;
    goto L_init;
  }

  newSiz = APT_VR_SIZ + APT_VR_INC;

  // printf("::::DB_allocVAR %d %d %d %d\n",Ind,APT_VR_SIZ,newSiz,APT_VR_INC);

  while (Ind >= newSiz) newSiz += APT_VR_INC;
  // printf("  %d %d %d\n",Ind,APT_VR_SIZ,newSiz);

  vr_tab = (double*)realloc(vr_tab, newSiz*sizeof(double));

  if(vr_tab == NULL) {
    TX_Error ("******** out of memory - DB_allocVAR *********");
    return -1;
  }

  L_init:
  for(i1=APT_VR_SIZ; i1<newSiz; ++i1) DB_setFree_Var(i1);
  APT_VR_SIZ = newSiz;

  // printf("ex DB_allocVAR %d %d %d\n",Ind,APT_VR_SIZ,newSiz);

  return 0;

}


//====================================================================
  int DB_allocPoint (long Ind) {
//====================================================================
// realloc pt_tab


  long i1, newSiz;


  if((Ind == 0)&&(pt_tab != NULL)) {  // reInit
    newSiz = APT_PT_SIZ;
    APT_PT_SIZ = 0;
    APT_PT_IND = 0;
    goto L_init;
  }

  newSiz = APT_PT_SIZ + APT_PT_INC;

  // printf("::::DB_allocPoint %d %d %d %d\n",Ind,APT_PT_SIZ,newSiz,APT_PT_INC);

  while (Ind >= newSiz) newSiz += APT_PT_INC;
  // printf("  %d %d %d\n",Ind,APT_PT_SIZ,newSiz);
  
  pt_tab = (Point*)realloc(pt_tab, newSiz*sizeof(Point));

  if(pt_tab == NULL) {
    TX_Error ("******** out of memory - DB_allocPoint *********");
    return -1;
  }

  L_init:
  for(i1=APT_PT_SIZ; i1<newSiz; ++i1) DB_setFree_PT (i1);
  APT_PT_SIZ = newSiz;

  // printf("ex DB_allocPoint %d %d %d\n",Ind,APT_PT_SIZ,newSiz);

  return 0;

}


//====================================================================
  int DB_allocVector (long Ind) {
//====================================================================
// realloc vc_tab


  long i1, newSiz;


  if((Ind == 0)&&(vc_tab != NULL)) {   // reInit
    newSiz = APT_VC_SIZ;
    APT_VC_SIZ = 0;
    APT_VC_IND = 0;
    goto L_init;
  }

  newSiz = APT_VC_SIZ + APT_VC_INC;

  // printf("::::DB_allocVector %d %d %d %d\n",Ind,APT_VC_SIZ,newSiz,APT_VC_INC);

  while (Ind >= newSiz) newSiz += APT_VC_INC;
  // printf("  %d %d %d\n",Ind,APT_VC_SIZ,newSiz);

  vc_tab = (Vector*)realloc(vc_tab, newSiz*sizeof(Vector));

  if(vc_tab == NULL) {
    TX_Error ("******** out of memory - DB_allocVector *********");
    return -1;
  }

  L_init:
  for(i1=APT_VC_SIZ; i1<newSiz; ++i1) DB_setFree_VC (i1);
  APT_VC_SIZ = newSiz;

  // printf("ex DB_allocVector %d %d %d\n",Ind,APT_VC_SIZ,newSiz);

  return 0;

}


//====================================================================
  int DB_allocLine (long Ind) {
//====================================================================
// realloc ln_tab


  long i1, newSiz;


  if((Ind == 0)&&(ln_tab != NULL)) {   // reInit
    newSiz = APT_LN_SIZ;
    APT_LN_SIZ = 0;
    APT_LN_IND = 0;
    goto L_init;
  }

  newSiz = APT_LN_SIZ + APT_LN_INC;

  while (Ind >= newSiz) newSiz += APT_LN_INC;
  // printf("  %d %d %d\n",Ind,APT_LN_SIZ,newSiz);

  printf("::::DB_allocLine %ld %ld %ld %d\n",Ind,APT_LN_SIZ,newSiz,APT_LN_INC);

  ln_tab = (Line*)realloc(ln_tab, newSiz*sizeof(Line));

  if(ln_tab == NULL) {
    TX_Error ("******** out of memory - DB_allocLine *********");
    return -1;
  }

  L_init:
  for(i1=APT_LN_SIZ; i1<newSiz; ++i1) DB_setFree_LN (i1);
  APT_LN_SIZ = newSiz;

  // printf("ex DB_allocLine %d %d %d\n",Ind,APT_LN_SIZ,newSiz);

  return 0;

}


//====================================================================
  int DB_allocCirc (long Ind) {
//====================================================================
// realloc ci_tab


  long   i1, newSiz;


  // printf("DB_allocCirc %ld\n",Ind);


  if((Ind == 0)&&(ci_tab != NULL)) {    // reInit
    newSiz = APT_CI_SIZ;
    APT_CI_SIZ = 0;
    APT_CI_IND = 0;
    goto L_init;
  }

  newSiz = APT_CI_SIZ + APT_CI_INC;

  // printf("::::DB_allocCirc %d %d %d %d\n",Ind,APT_CI_SIZ,newSiz,APT_CI_INC);

  while (Ind >= newSiz) newSiz += APT_CI_INC;
  // printf("  %d %d %d\n",Ind,APT_CI_SIZ,newSiz);

  // printf(" ci_tab1=%p siz=%d\n",ci_tab,newSiz*sizeof(Circ));
  ci_tab = (Circ*)realloc((void*)ci_tab, newSiz*sizeof(Circ));
  // printf(" ci_tab2=%p\n",ci_tab);

  if(ci_tab == NULL) {
    TX_Error ("******** out of memory - DB_allocCirc *********");
    return -1;
  }

  L_init:
  for(i1=APT_CI_SIZ; i1<newSiz; ++i1) DB_setFree_CI (i1);
  APT_CI_SIZ = newSiz;

    // printf("ex DB_allocCirc %d %d %d\n",Ind,APT_CI_SIZ,newSiz);

  return 0;

}


//====================================================================
  int DB_allocRef (long Ind) {
//====================================================================
// realloc pln_tab


  long   i1, newSiz;


  if((Ind == 0)&&(pln_tab != NULL)) {    // reInit
    newSiz = APT_PL_SIZ;
    APT_PL_SIZ = 0;
    APT_PL_IND = 0;
    goto L_init;
  }

  newSiz = APT_PL_SIZ + APT_PL_INC;

  // printf("::::DB_allocRef %d %d %d %d\n",Ind,APT_PL_SIZ,newSiz,APT_PL_INC);

  while (Ind >= newSiz) newSiz += APT_PL_INC;
  // printf("  %d %d %d\n",Ind,APT_PL_SIZ,newSiz);

  pln_tab = (Plane*)realloc((void*)pln_tab, newSiz*sizeof(Plane));
  // mat_tab = (Mat_4x3*)realloc((void*)mat_tab, newSiz*sizeof(Mat_4x3));

  // if((pln_tab == NULL)||(mat_tab == NULL)) {
  if(pln_tab == NULL) {
    TX_Error ("******** out of memory - DB_allocRef *********");
    return -1;
  }

  L_init:
  for(i1=APT_PL_SIZ; i1<newSiz; ++i1) DB_setFree_PLN (i1);
  APT_PL_SIZ = newSiz;

  // printf("ex DB_allocRef %d %d %d\n",Ind,APT_PL_SIZ,newSiz);

  return 0;

}


//====================================================================
  int DB_allocModRef (long Ind) {
//====================================================================
// realloc ?


  long   i1, newSiz;


  if((Ind == 0)&&(mdr_tab != NULL)) {
    // reInit - clear all refMdls
    newSiz = APT_MR_SIZ;
    APT_MR_SIZ = 0;
    APT_MR_IND = 0;
    goto L_init;
  }

  newSiz = APT_MR_SIZ + APT_MR_INC;

  while (Ind >= newSiz) newSiz += APT_MR_INC;
  // printf("  %d %d %d\n",Ind,APT_MR_SIZ,newSiz);

  mdr_tab = (ModelRef*)realloc((void*)mdr_tab, newSiz*sizeof(ModelRef));

  printf("::::DB_allocModRef %ld %ld %ld %d\n",Ind,APT_MR_SIZ,newSiz,APT_MR_INC);

  if(mdr_tab == NULL) {
    TX_Error ("******** out of memory - DB_allocModRef *********");
    return -1;
  }

  L_init:
  // init for DB_isFree_ModRef (set po.x == UT_VAL_MAX)
  for(i1=APT_MR_SIZ; i1<newSiz; ++i1) DB_setFree_MR (i1);

  // init error
  if(APT_MR_SIZ == 0) mdr_tab[0].modNr = -2;

  APT_MR_SIZ = newSiz;


  // printf("ex DB_allocModRef %d %d %d\n",Ind,APT_MR_SIZ,newSiz);

  return 0;

}


// //====================================================================
//   int DB_allocModNam (long Ind) {
// //====================================================================
// // realloc space fuer BasModelnames; wird nicht gesichert !
// 
//   long   i1, newSiz;
// 
// 
//   // Reset mdb_nam
//   if((Ind == 0)&&(mdb_nam != NULL)) {
//     // printf("DB_allocModNam reset\n");
//     DB_MNM_IND = 0;
//     return 0;
//   }
// 
// 
//   newSiz = DB_MNM_SIZ + DB_MNM_INC;
// 
//   // printf("::::DB_allocModNam %d\n",newSiz);
// 
//   mdb_nam = (char*)realloc(mdb_nam, newSiz);
// 
//   if(mdb_nam == NULL) {
//     TX_Error ("******** out of memory - DB_allocModNam *********");
//     return -1;
//   }
//   DB_MNM_SIZ = newSiz;
// 
// 
//   return 0;
// 
// }


//====================================================================
  int DB_allocModBas (long Ind) {
//====================================================================
// realloc mdb_dyn


  long   i1, newSiz;


  if((Ind == 0)&&(mdb_dyn != NULL)) {
    // printf("DB_allocModBas reset\n");
    DYN_MB_IND = 0;
    return 0;
  }

  newSiz = DYN_MB_SIZ + DYN_MB_INC;

  // printf("::::DB_allocModBas %d %d %d %d\n",Ind,DYN_MB_SIZ,newSiz,DYN_MB_INC);

  mdb_dyn = (ModelBas*)realloc((void*)mdb_dyn, newSiz*sizeof(ModelBas));

  if(mdb_dyn == NULL) {
    TX_Error ("******** out of memory - DB_allocModBas *********");
    return -1;
  }
  DYN_MB_SIZ = newSiz;

  // printf("ex DB_allocModBas %d %d %d\n",Ind,DYN_MB_SIZ,newSiz);

  return 0;

}

/*
//====================================================================
  int DB_allocTool (long Ind) {
//====================================================================
// realloc ?


  long   i1, newSiz;


  if((Ind == 0)&&(tl_tab != NULL)) {    // reInit
    newSiz = APT_TL_SIZ;
    APT_TL_SIZ = 0;
    APT_TL_IND = 0;
    goto L_init;
  }

  newSiz = APT_TL_SIZ + APT_TL_INC;

  // printf("::::DB_allocTool %d %d %d %d\n",Ind,APT_TL_SIZ,newSiz,APT_TL_INC);

  while (Ind >= newSiz) newSiz += APT_TL_INC;
  // printf("  %d %d %d\n",Ind,APT_TL_SIZ,newSiz);

  tl_tab = (BTool*)realloc((void*)tl_tab, newSiz*sizeof(BTool));

  if(tl_tab == NULL) {
    TX_Error ("******** out of memory - DB_allocTool *********");
    return -1;
  }

  L_init:
  for(i1=APT_TL_SIZ; i1<newSiz; ++i1) {
    tl_tab[i1].dmr = UT_VAL_MAX;
  }

  APT_TL_SIZ = newSiz;

  // printf("ex DB_allocTool %d %d %d\n",Ind,APT_TL_SIZ,newSiz);

  return 0;

}
*/

//====================================================================
  int DB_allocTra (long Ind) {
//====================================================================
// realloc ?


  long   i1, newSiz;


  if((Ind == 0)&&(tra_tab != NULL)) {  // reInit
    newSiz = APT_TR_SIZ;
    APT_TR_SIZ = 0;
    APT_TR_IND = 0;
    goto L_init;
  }

  newSiz = APT_TR_SIZ + APT_TR_INC;

  // printf("::::DB_allocTra %d %d %d %d\n",Ind,APT_TR_SIZ,newSiz,APT_TR_INC);

  while (Ind >= newSiz) newSiz += APT_TR_INC;
  // printf("  %d %d %d\n",Ind,APT_TR_SIZ,newSiz);

  tra_tab = (ObjGX*)realloc((void*)tra_tab, newSiz*sizeof(ObjGX));

  if(tra_tab == NULL) {
    TX_Error ("******** out of memory - DB_allocTra *********");
    return -1;
  }

  L_init:
  for(i1=APT_TR_SIZ; i1<newSiz; ++i1) DB_setFree_TR (i1);
  APT_TR_SIZ = newSiz;

  // printf("ex DB_allocTra %d %d %d\n",Ind,APT_TR_SIZ,newSiz);

  return 0;

}


//====================================================================
  int DB_allocCurve (long Ind) {
//====================================================================
// realloc cv_tab


  long   i1, newSiz;


  if((Ind == 0)&&(cv_tab != NULL)) {     // reInit
    newSiz = APT_CV_SIZ;
    APT_CV_SIZ = 0;
    APT_CV_IND = 0;
    goto L_init;
  }

  newSiz = APT_CV_SIZ + APT_CV_INC;

  // printf("::::DB_allocCurve %d %d %d %d\n",Ind,APT_CV_SIZ,newSiz,APT_CV_INC);

  while (Ind >= newSiz) newSiz += APT_CV_INC;
  // printf("  %d %d %d\n",Ind,APT_CV_SIZ,newSiz);

  cv_tab = (ObjGX*)realloc((void*)cv_tab, newSiz*sizeof(ObjGX));

  if(cv_tab == NULL) {
    TX_Error ("******** out of memory - DB_allocCurve *********");
    return -1;
  }

  L_init:
  for(i1=APT_CV_SIZ; i1<newSiz; ++i1) DB_setFree_CV (i1);
  APT_CV_SIZ = newSiz;

  // printf("ex DB_allocCurve %d %d %d\n",Ind,APT_CV_SIZ,newSiz);

  return 0;

}


//====================================================================
  int DB_allocNote (long Ind) {
//====================================================================
// realloc tx_tab


  long   i1, newSiz;


  if((Ind == 0)&&(tx_tab != NULL)) {     // reInit
    newSiz = APT_TX_SIZ;
    APT_TX_SIZ = 0;
    APT_TX_IND = 0;
    goto L_init;
  }

  newSiz = APT_TX_SIZ + APT_TX_INC;

  // printf("::::DB_allocNote %d %d %d %d\n",Ind,APT_TX_SIZ,newSiz,APT_TX_INC);

  while (Ind >= newSiz) newSiz += APT_TX_INC;
  // printf("  %d %d %d\n",Ind,APT_TX_SIZ,newSiz);

  tx_tab = (ObjGX*)realloc((void*)tx_tab, newSiz*sizeof(ObjGX));

  if(tx_tab == NULL) {
    TX_Error ("******** out of memory - DB_allocNote *********");
    return -1;
  }

  L_init:
  for(i1=APT_TX_SIZ; i1<newSiz; ++i1) DB_setFree_TX (i1);
  APT_TX_SIZ = newSiz;

  // printf("ex DB_allocNote %d %d %d\n",Ind,APT_TX_SIZ,newSiz);

  return 0;

}


//====================================================================
  int DB_allocSur (long Ind) {
//====================================================================
// realloc su_tab


  long   i1, newSiz;


  // printf("DB_allocSur %ld\n",Ind);



  if((Ind == 0)&&(su_tab != NULL)) {     // reInit
    newSiz = APT_SU_SIZ;
    APT_SU_SIZ = 0;
    APT_SU_IND = 0;
    goto L_init;
  }

  newSiz = APT_SU_SIZ + APT_SU_INC;

  // printf("::::DB_allocSur %d %d %d %d\n",Ind,APT_SU_SIZ,newSiz,APT_SU_INC);

  while (Ind >= newSiz) newSiz += APT_SU_INC;
  // printf("  %d %d %d\n",Ind,APT_SU_SIZ,newSiz);

  su_tab = (ObjGX*)realloc((void*)su_tab, newSiz*sizeof(ObjGX));

  if(su_tab == NULL) {
    TX_Error ("******** out of memory - DB_allocSur *********");
    return -1;
  }

  L_init:
  for(i1=APT_SU_SIZ; i1<newSiz; ++i1) DB_setFree_SU (i1);

  APT_SU_SIZ = newSiz;

    // printf("ex DB_allocSur %d %d %d\n",Ind,APT_SU_SIZ,newSiz);

  return 0;

}


//====================================================================
  int DB_allocSol (long Ind) {
//====================================================================
// realloc so_tab


  long   i1, newSiz;


  if((Ind == 0)&&(so_tab != NULL)) {   // reInit
    newSiz = APT_SO_SIZ;
    APT_SO_SIZ = 0;
    APT_SO_IND = 0;
    goto L_init;
  }

  newSiz = APT_SO_SIZ + APT_SO_INC;

  // printf("::::DB_allocSol %d %d %d %d\n",Ind,APT_SO_SIZ,newSiz,APT_SO_INC);

  while (Ind >= newSiz) newSiz += APT_SO_INC;
  // printf("  %d %d %d\n",Ind,APT_SO_SIZ,newSiz);

  so_tab = (ObjGX*)realloc((void*)so_tab, newSiz*sizeof(ObjGX));

  if(so_tab == NULL) {
    TX_Error ("******** out of memory - DB_allocSol *********");
    return -1;
  }

  L_init:
  for(i1=APT_SO_SIZ; i1<newSiz; ++i1) DB_setFree_SO (i1);

  APT_SO_SIZ = newSiz;

  // printf("ex DB_allocSol %d %d %d\n",Ind,APT_SO_SIZ,newSiz);

  return 0;

}


//====================================================================
  int DB_allocAct (long Ind) {
//====================================================================

  long   i1, newSiz;


  if((Ind == 0)&&(ac_tab != NULL)) {    // reInit
    newSiz = APT_AC_SIZ;
    APT_AC_SIZ = 0;
    APT_AC_IND = 0;
    goto L_init;
  }

  newSiz = APT_AC_SIZ + APT_AC_INC;

  // printf("::::DB_allocAct %d %d %d %d\n",Ind,APT_AC_SIZ,newSiz,APT_AC_INC);

  while (Ind >= newSiz) newSiz += APT_AC_INC;
  // printf("  %d %d %d\n",Ind,APT_AC_SIZ,newSiz);

  // printf(" ac_tab1=%p siz=%d\n",ac_tab,newSiz*sizeof(Activity));
  ac_tab = (Activity*)realloc((void*)ac_tab, newSiz*sizeof(Activity));
  // printf(" ac_tab2=%p\n",ac_tab);

  if(ac_tab == NULL) {
    TX_Error ("******** out of memory - DB_allocAct *********");
    return -1;
  }

  L_init:
  for(i1=APT_AC_SIZ; i1<newSiz; ++i1) DB_setFree_Act (i1),
  APT_AC_SIZ = newSiz;

  // printf("ex DB_allocAct %d %d %d\n",Ind,APT_AC_SIZ,newSiz);

  return 0;

}


//====================================================================
  int DB_allocDynVAR (long Ind) {
//====================================================================
// realloc pt_dyn


  long i1, newSiz;

  if((Ind == 0)&&(vr_dyn != NULL)) {
    DYN_VR_IND = 0;
    return 0;
  }

  // printf("::::DB_allocDynVAR %d\n",DYN_VR_SIZ);

  newSiz = DYN_VR_SIZ + DYN_VR_INC;

  vr_dyn = (double*)realloc(vr_dyn, newSiz*sizeof(double));

  if(vr_dyn == NULL) {
    TX_Error ("******** out of memory - DB_allocDynVAR *********");
    return -1;
  }

  for(i1=DYN_VR_SIZ; i1<newSiz; ++i1) {
    vr_dyn[i1] = UT_VAL_MAX;
  }
  DYN_VR_SIZ = newSiz;

  return 0;

}


//====================================================================
  int DB_allocDynPoint (long Ind) {
//====================================================================
// realloc pt_dyn


  long i1, newSiz;

  if((Ind == 0)&&(pt_dyn != NULL)) {
    DYN_PT_IND = 0;
    return 0;
  }

  // printf("::::DB_allocDynPoint %d\n",DYN_PT_SIZ);

  newSiz = DYN_PT_SIZ + DYN_PT_INC;

  pt_dyn = (Point*)realloc(pt_dyn, newSiz*sizeof(Point));

  if(pt_dyn == NULL) {
    TX_Error ("******** out of memory - DB_allocDynPoint *********");
    return -1;
  }

  for(i1=DYN_PT_SIZ; i1<newSiz; ++i1) {
    pt_dyn[i1].x = UT_VAL_MAX;
  }
  DYN_PT_SIZ = newSiz;

  return 0;

}


//====================================================================
  int DB_allocDynVector (long Ind) {
//====================================================================
// realloc vc_dyn


  long i1, newSiz;

  // printf("DB_allocDynVector %ld\n",Ind);


  if((Ind == 0)&&(vc_dyn != NULL)) {
    DYN_VC_IND = 6;  // vec 0-5 sind DX,Dy,DZ .... !!!!
    return 0;
  }

  // printf("::::DB_allocDynVector %d\n",DYN_VC_SIZ);

  newSiz = DYN_VC_SIZ + DYN_VC_INC;

  vc_dyn = (Vector*)realloc(vc_dyn, newSiz*sizeof(Vector));

  if(vc_dyn == NULL) {
    TX_Error ("******** out of memory - DB_allocDynVector *********");
    return -1;
  }

  for(i1=DYN_VC_SIZ; i1<newSiz; ++i1) {
    vc_dyn[i1].dx = UT_VAL_MAX;
  }
  DYN_VC_SIZ = newSiz;

  return 0;

}


//====================================================================
  int DB_allocDynLine (long Ind) {
//====================================================================
// realloc ln_dyn


  long i1, newSiz;


  if((Ind == 0)&&(ln_dyn != NULL)) {
    DYN_LN_IND = 0;
    return 0;
  }

  // printf("::::DB_allocDynLine %d\n",DYN_LN_SIZ);

  newSiz = DYN_LN_SIZ + DYN_LN_INC;

  ln_dyn = (Line*)realloc(ln_dyn, newSiz*sizeof(Line));

  if(ln_dyn == NULL) {
    TX_Error ("******** out of memory - DB_allocDynLine *********");
    return -1;
  }

  for(i1=DYN_LN_SIZ; i1<newSiz; ++i1) {
    ln_dyn[i1].p1.x = UT_VAL_MAX;
  }
  DYN_LN_SIZ = newSiz;

  return 0;

}


//====================================================================
  int DB_allocDynCirc (long Ind) {
//====================================================================
// realloc ci_dyn


  long i1, newSiz;

  if((Ind == 0)&&(ci_dyn != NULL)) {
    DYN_CI_IND = 0;
    return 0;
  }

  // printf("::::DB_allocDynCirc %d\n",DYN_CI_SIZ);

  newSiz = DYN_CI_SIZ + DYN_CI_INC;

  ci_dyn = (Circ*)realloc(ci_dyn, newSiz*sizeof(Circ));

  if(ci_dyn == NULL) {
    TX_Error ("******** out of memory - DB_allocDynCirc *********");
    return -1;
  }

  for(i1=DYN_CI_SIZ; i1<newSiz; ++i1) {
    ci_dyn[i1].p1.x = UT_VAL_MAX;
  }
  DYN_CI_SIZ = newSiz;

  return 0;

}


//====================================================================
  int DB_allocDynPlane (long Ind) {
//====================================================================
// realloc ci_dyn


  long i1, newSiz;

  if((Ind == 0)&&(pln_dyn != NULL)) {
    DYN_PL_IND = 0;
    return 0;
  }

  // printf("::::DB_allocDynPlane %d\n",DYN_PL_SIZ);

  newSiz = DYN_PL_SIZ + DYN_PL_INC;

  pln_dyn = (Plane*)realloc(pln_dyn, newSiz*sizeof(Plane));

  if(pln_dyn == NULL) {
    TX_Error ("******** out of memory - DB_allocDynPlane *********");
    return -1;
  }

  for(i1=DYN_PL_SIZ; i1<newSiz; ++i1) {
    pln_dyn[i1].p = UT_VAL_MAX;
  }
  DYN_PL_SIZ = newSiz;

  return 0;

}


//====================================================================
  int DB_allocDynCurve (long Ind) {
//====================================================================
// realloc cv_dyn


  long i1, newSiz;

  if((Ind == 0)&&(cv_dyn != NULL)) {
    DYN_CV_IND = 0;
    return 0;
  }

  newSiz = DYN_CV_SIZ + DYN_CV_INC;
  while (Ind >= newSiz) newSiz += DYN_CV_INC;


  // printf("::::DB_allocDynCurve %d %d\n",DYN_CV_SIZ,newSiz);


  cv_dyn = (ObjGX*)realloc(cv_dyn, newSiz*sizeof(ObjGX));

  if(cv_dyn == NULL) {
    TX_Error ("******** out of memory - DB_allocDynCurve *********");
    return -1;
  }

  for(i1=DYN_CV_SIZ; i1<newSiz; ++i1) {
    cv_dyn[i1].typ = Typ_Error;
  }
  DYN_CV_SIZ = newSiz;

  return 0;

}


//====================================================================
  int DB_allocDynSur (long Ind) {
//====================================================================
// realloc cv_dyn


  long i1, newSiz;

  if((Ind == 0)&&(su_dyn != NULL)) {
    DYN_SU_IND = 0;
    return 0;
  }

  // printf("::::DB_allocDynSur %d\n",DYN_SU_SIZ);

  newSiz = DYN_SU_SIZ + DYN_SU_INC;

  su_dyn = (ObjGX*)realloc(su_dyn, newSiz*sizeof(ObjGX));

  if(su_dyn == NULL) {
    TX_Error ("******** out of memory - DB_allocDynSur *********");
    return -1;
  }

  for(i1=DYN_SU_SIZ; i1<newSiz; ++i1) {
    su_dyn[i1].typ = Typ_Error;
  }
  DYN_SU_SIZ = newSiz;

  return 0;

}


//====================================================================
  int DB_allocDynNote (long Ind) {
//====================================================================
// realloc tx_dyn


  long i1, newSiz;

  if((Ind == 0)&&(tx_dyn != NULL)) {
    DYN_TX_IND = 0;
    return 0;
  }

  // printf("::::DB_allocDynNote %d\n",DYN_TX_SIZ);

  newSiz = DYN_TX_SIZ + DYN_TX_INC;

  tx_dyn = (ObjGX*)realloc(tx_dyn, newSiz*sizeof(ObjGX));

  if(tx_dyn == NULL) {
    TX_Error ("******** out of memory - DB_allocDynNote *********");
    return -1;
  }

  for(i1=DYN_TX_SIZ; i1<newSiz; ++i1) {
    tx_dyn[i1].typ = Typ_Error;
  }
  DYN_TX_SIZ = newSiz;

  return 0;

}

 
//================================================================
  int DB_set_state (int mode) {
//================================================================
/// \code
/// DB_set_state     save/restore all saved DB-Pointers (kill newer objects)
/// mode=0     save all pointers
/// mode=1     restore all pointers (kill newer objects)
/// \endcode

// see DB_load__ (DB_setFree_..)

  static long vr,vc,pt,ln,ci,cv,su,so,pl,tr,tx,mr,ac;
  long   l1;


  // printf("SSSSSSSSSSSSSSSSSSSSSSS DB_set_state %d\n",mode);
  // printf("  set_state pt=%ld vc=%ld\n",pt,vc);


  if(mode == 0) {              // save
    vr = APT_VR_IND;
    vc = APT_VC_IND;
    pt = APT_PT_IND;
    ln = APT_LN_IND;
    ci = APT_CI_IND;
    cv = APT_CV_IND;
    su = APT_SU_IND;
    so = APT_SO_IND;
    pl = APT_PL_IND;
    tr = APT_TR_IND;
    tx = APT_TX_IND;
    mr = APT_MR_IND;
    ac = APT_AC_IND;


  //----------------------------------------------------------------
  } else {                     // 1=restore
    if(APT_VR_IND > vr) {
        // printf(" restore old-vr=%ld over %ld\n",vr,APT_VR_IND);
      for(l1=vr+1; l1<=APT_VR_IND; ++l1) DB_setFree_Var(l1);
      APT_VR_IND = vr;
    }


    if(APT_VC_IND > vc) {
      for(l1=vc+1; l1<=APT_VC_IND; ++l1) DB_setFree_VC(l1);
      APT_VC_IND = vc;
    }


    if(APT_PT_IND > pt) {
      for(l1=pt+1; l1<=APT_PT_IND; ++l1) DB_setFree_PT(l1);
      APT_PT_IND = pt;
    }


    if(APT_LN_IND > ln) {
      for(l1=ln+1; l1<=APT_LN_IND; ++l1) DB_setFree_LN(l1);
      APT_LN_IND = ln;
    }


    if(APT_CI_IND > ci) {   // Typ_CI
      for(l1=ci+1; l1<=APT_CI_IND; ++l1) DB_setFree_CI(l1);
      APT_CI_IND = ci;
    }


    if(APT_CV_IND > cv) {
      for(l1=cv+1; l1<=APT_CV_IND; ++l1) DB_setFree_CV(l1);
      APT_CV_IND = cv;
    }


    if(APT_SU_IND > su) {
      for(l1=su+1; l1<=APT_SU_IND; ++l1) DB_setFree_SU(l1);
      APT_SU_IND = su;
    }


    if(APT_SO_IND > so) {
      for(l1=so+1; l1<=APT_SO_IND; ++l1) DB_setFree_SO(l1);
      APT_SO_IND = so;
    }


    if(APT_PL_IND > pl) {
      for(l1=pl+1; l1<=APT_PL_IND; ++l1) DB_setFree_PLN(l1);
      APT_PL_IND = pl;
    }


    if(APT_TR_IND > tr) {
      for(l1=tr+1; l1<=APT_TR_IND; ++l1) DB_setFree_TR(l1);
      APT_TR_IND = tr;
    }


    if(APT_TX_IND > tx) {
      for(l1=tx+1; l1<=APT_TX_IND; ++l1) DB_setFree_TX(l1);
      APT_TX_IND = tx;
    }


    if(APT_MR_IND > mr) {
      for(l1=mr+1; l1<=APT_MR_IND; ++l1) DB_setFree_MR(l1);
      APT_MR_IND = mr;
    }


    if(APT_AC_IND > ac) {
      for(l1=ac+1; l1<=APT_AC_IND; ++l1) DB_setFree_Act(l1);
      APT_AC_IND = ac;
    }


  }

    // printf("ex-set_state pt=%ld vc=%ld\n",pt,vc);

  return 0;

}


//================================================================
  int DB_CSEG__ (int mode) {
//================================================================
// save / reset DB_CSEG
// Input:
//   mode      0=save 
//             1=(re)set from local stored
//
// see also DB_dyn__


// sollte eigentlich bei einem go_back_to_previous_sourceline
// zurückgesetzt werden !
// Dazu muesste man zu jeder sourceline die CSEG-Pos speichern !
// In NC gibts doch was ähnliches .. ?


  static unsigned long actCseg = 0;
  

  // printf("DB_CSEG__ %d %ld act=%ld\n",mode,actCseg,
          // DB_CSEG.next - DB_CSEG.start);


  if(mode == 1) {
    // reset !
    // DB_CSEG.next = (char*)DB_CSEG.start + actCseg;
    DB_CSEG.next = (void*)((unsigned long)DB_CSEG.start + actCseg);

  } else {
    // actCseg = (char*)DB_CSEG.next - DB_CSEG.start;
    actCseg = (unsigned long)DB_CSEG.next - (unsigned long)DB_CSEG.start;
  }

  return 0;

}

/*
//================================================================
  int DB_CSEG__ (int iTmp) {
//================================================================
/// \code
/// reset DB_CSEG after use for temporary objects.
/// itmp      0=permanent, 1=temporary (workmode)
/// see also DB_dyn__
/// \endcode

// ACHTUNG: funktioniert nur wenn keine dependig objects nachfolgen;
// also S1=.. dann A1=S1 ..  GEHT NED !

// sollte eigentlich bei einem go_back_to_previous_sourceline
// zurückgesetzt werden !
// Dazu muesste man zu jeder sourceline die CSEG-Pos speichern !
// In NC gibts doch was ähnliches .. ?


  static int actTmp = 0;
  static unsigned long actCseg = 0;
  

  if(actTmp != 0) {
    // reset !
      // printf(" reset DB_CSEG to %ld\n",actCseg);
    DB_CSEG.next = DB_CSEG.start + actCseg;
  }

  actTmp = iTmp;
  actCseg = DB_CSEG.next - DB_CSEG.start;

  return 0;

}
*/

//====================================================================
  int DB_CSEG_ck () {
//====================================================================
/// \code
/// testen, ob noch mind 25 K in CDAT frei sind;
/// wenn nein: realloc UND stop !
/// RC =  0: genug Platz vorhanden ...
/// RC = -1: realloc done, restart.
/// \endcode

  unsigned long  freeSpc;

  freeSpc = (char*)DB_CSEG.end - (char*)DB_CSEG.next;


  // TEST ONLY
  { static unsigned long oldSpc;
    if(oldSpc != freeSpc)
      // printf(" now DB_CSEG-space %ld\n",freeSpc);
    oldSpc = freeSpc;
  }
  // TEST ONLY



  if(freeSpc > 25000) return 0;

    printf(" DB_CSEG too small ..\n");

  DB_allocCDAT (1);

  return -1;

}


//====================================================================
  int DB_allocCDAT (long Ind) {
//====================================================================
/// \code
/// realloc DB_CDAT
/// Ind = 0    Init / reInit mit Mind.Siz = DB_CINC
/// Ind = 1    increase um DB_CINC
/// Ind > 1    make Mind.Siz = Ind
/// \endcode


  long i1, newSiz, oldSiz;


  // printf("DB_allocCDAT %d %d\n",Ind,DB_CSIZ);

  //----------------------------------------------------------------
  if(Ind == 0) {                 // Init mit Mind.Siz = DB_CINC
    if(DB_CDAT == NULL) {
      DB_CSEG.start = NULL;
      DB_CSEG.next  = NULL;
      DB_CSEG.end   = NULL;
      DB_CSIZ = 0;
      oldSiz  = 0;
      newSiz  = DB_CINC;

    }  else {                               // reInit
      DB_CSEG.start = DB_CDAT;
      DB_CSEG.next  = DB_CDAT;
      DB_CSEG.end   = DB_CDAT + DB_CSIZ;
      goto L_exit;
    }

  //----------------------------------------------------------------
  } else if(Ind == 1) {          // increase um DB_CINC
    oldSiz = (long)((char*)DB_CSEG.next - (char*)DB_CSEG.start);
    newSiz = DB_CSIZ + DB_CINC;


  //----------------------------------------------------------------
  } else {                       // make Mind.Siz = Ind
    if(DB_CSIZ > Ind) goto L_exit;

    oldSiz = (long)((char*)DB_CSEG.next - (char*)DB_CSEG.start);
    newSiz = DB_CSIZ;
    while (Ind >= newSiz) newSiz += DB_CINC;

  }


    printf("::::DB_allocCDAT %ld %ld\n",DB_CSIZ,newSiz);
    // AP_dump_statPg ("DB_allocCDAT: ");


  DB_CDAT = (char*)realloc(DB_CDAT, newSiz);

  if(DB_CDAT == NULL) {
    printf ("******** out of memory - DB_allocCDAT *********\n");
    TX_Error ("******** out of memory - DB_allocCDAT *********");
    return -1;
  }

  // UME_init (&DB_CSEG, DB_CDAT, DB_CSIZ)  // korrigieren
  DB_CSEG.start = DB_CDAT;
  DB_CSEG.next  = DB_CDAT + oldSiz;
  DB_CSEG.end   = DB_CDAT + newSiz;

  DB_CSIZ = newSiz;

  // Init OK.
  if(Ind == 0) goto L_exit;

  // nach realloc muss man neu beginnen !
  TX_Print ("realloc DB_allocCDAT - please wait ..");
  // exit(0);  // TEST ONLY
  AP_errStat_set (2);

  L_exit:
    // printf("ex DB_allocCDAT\n");
  return 0;

}


//================================================================
  long DB_StoreDynPoint (long Ind, Point* pt1) {
//================================================================
// DB_StoreDynPoint               store/overwrite dynamic point
// Input:
//   Ind          0 = create new point;  else overwrite point
// Output:
//   retCode      returns the DB-index as negative nr


  long dbi;


  if(!Ind) {
    // 0 = create new point
    dbi = DB_GetDynInd (Typ_PT);  // returns new neg. index


  } else {
    // overwrite point
    // make index negative
    if(Ind > 0) dbi = -Ind;
    else        dbi = Ind;
  }


  pt_dyn[-dbi] = *pt1;


  return dbi;

}

 
//=============================================================
  long DB_StorePoint (long Ind, Point* pt1) {
//=============================================================
// DB_StorePoint            store permanent point
// Input:
//   Ind          >0=permanent; -1=dynamic-permanent;
//   retCode      0=Err; DB-index; negativ for dynamic-permanent

  long dbi;


  // DEB_dump_obj__(Typ_PT, pt1, "DB_StorePoint %d:",Ind);


  //---------------- Dynam. ---------------------------
  if(Ind < 0) {

    dbi = DB_GetDynInd (Typ_PT);
      // printf(" dynInd=%ld\n",dbi);
    pt_dyn[-dbi] = *pt1;

  
  //---------------- normal ---------------------------
  } else {
    
    if(Ind > APT_PT_IND) {
      APT_PT_IND = Ind;
      if(Ind >= APT_PT_SIZ) DB_allocPoint (Ind);
    }
      
    dbi = Ind;
    pt_tab[dbi] = *pt1;
  }

    // printf("ex DB_StorePoint %ld %ld %f,%f,%f\n",Ind,dbi,pt1->x,pt1->y,pt1->z);

  return dbi;

}



//================================================================
Vector DB_GetVector (long Ind) {
//================================================================
/// DB_get_VC

  Vector vc1;

  // printf("DB_GetVector %d %d %d\n",Ind,DYN_VC_SIZ,APT_VC_SIZ);
  // DEB_dump_obj__ (Typ_VC, &vc_dyn[1], "vc_dyn[1]:");


  if(Ind < 0) {
    if (Ind <= -DYN_VC_SIZ) {
      TX_Error("- Zugriff auf dyn. Vec. %d ****",Ind);
      return UT3D_VECTOR_Z;
    }
    vc1 = vc_dyn[-Ind];
    goto L_exit;
  }


  if((Ind >= APT_VC_SIZ) || (DB_isFree_VC (&vc_tab[Ind])))  {
      // (vc_tab[Ind].dx == UT_VAL_MAX)) {
      if(Ind != 0) TX_Error (" Vektor %d undefined",Ind);
      return UT3D_VECTOR_Z;
  }


  vc1 = vc_tab[Ind];

  L_exit:

    // printf("ex DB_GetVector %ld=%f,%f,%f\n",Ind,vc1.dx,vc1.dy,vc1.dz);

  return vc1;

}



//================================================================
Point DB_GetPoint (long Ind) {
//================================================================
/// \code
/// returns point.
/// get pointer with DB_get_PT
/// \endcode

  Point pt1;


  if(Ind < 0) {
    if (Ind <= -DYN_PT_SIZ) {
      TX_Error("- Zugriff auf dyn. Punkt %d ****",Ind);
      return pt_tab[0];
    }
    pt1 = pt_dyn[-Ind];


  } else if((Ind >= APT_PT_SIZ) || (DB_isFree_PT (&pt_tab[Ind]))) {
            // (pt_tab[Ind].x == UT_VAL_MAX)) {
    if(Ind != 0) TX_Error (" Punkt %d undefined",Ind);
    pt1 = DB_pt0;


  } else {
    pt1 = pt_tab[Ind];

  }

  // printf("DB_GetPoint %d=%f,%f,%f\n",Ind,pt1.x,pt1.y,pt1.z);
  // DEB_dump_obj__ (Typ_PT, &pt1, "ex DB_GetPoint %d",Ind);
  

  return pt1;

}



//================================================================
long DB_StoreLine (long Ind, Line* ln1) {
//================================================================
/// \code
/// create DB-record for line
/// Input:
///   Ind       dbi; -1   get dynamic-obj-index; 
///                  >=0  overwrite existing DB-record
/// Output:
///   RetCod     dbi  (negative for dynamic obj)
/// \endcode

// Achtung: wenn TypInd<0, dann muss ein dynam. Objekt noch
//  erzeugt werden !

  long dbi;


  // DEB_dump_obj__ (Typ_LN, ln1, "DB_StoreLine %d",Ind);
  // printf(" APT_LN_SIZ=%d\n",APT_LN_SIZ);


  // Dynam. Objekt erzeugen -
  if(Ind < 0) {
    dbi = DB_GetDynInd(Typ_LN);  // get dynamic DB-index (negative)
    ln_dyn[-dbi] = *ln1;


  //---------------- normal ---------------------------
  } else {

    if(Ind > APT_LN_IND) {
      APT_LN_IND = Ind;

      if(Ind >= APT_LN_SIZ) {    // realloc
        if(DB_allocLine (Ind) < 0) return 0;
      }
    }
    dbi = Ind;
    // if(!(DB_isFree_LN(&ln_tab[dbi])))
       // TX_Print("**** DB: overwrite Line %ld",dbi); // problem with MODVAL
    ln_tab[dbi] = *ln1;
  }


  //TX_Print("DB_StoreLine %ld %ld\n",Ind,dbi);
  //TX_Print("    %f %f %f %f\n",ln1->p1.x,ln1->p1.y,ln1->p2.x,ln1->p2.y);


  return dbi;

}



//********************************************************************
Line DB_GetLine (long Ind) {
//********************************************************************
// DB_get_LN

  Line ln1;


  if(Ind < 0) {
    if (Ind <= -DYN_LN_SIZ) {
      TX_Error("- Zugriff auf dyn. Line %d ****",Ind);
      return ln_tab[0];
    }
    ln1 = ln_dyn[-Ind];


  } else if((Ind >= APT_LN_SIZ) || (DB_isFree_LN (&ln_tab[Ind]))) {
            // (ln_tab[Ind].p1.x == UT_VAL_MAX)) {
    if(Ind != 0) TX_Error (" Line %d undefined",Ind);
    ln1.p1 = DB_pt0;
    ln1.p2 = DB_pt0;


  } else {
    ln1 = ln_tab[Ind];

  }


  return ln1;

}




//********************************************************************
  long DB_StoreCirc (long Ind, Circ* ci1) {
//********************************************************************

// Achtung: wenn TypInd<0, dann muss ein dynam. Objekt noch
//  erzeugt werden !

  long dbi;




  // Dynam. Objekt erzeugen -
  if(Ind < 0) {
    dbi = DB_GetDynInd(Typ_CI);
    ci_dyn[-dbi] = *ci1;


  //---------------- normal ---------------------------
  } else {

    if(Ind > APT_CI_IND) {
      APT_CI_IND = Ind;

      if(Ind >= APT_CI_SIZ) {      // realloc
        if(DB_allocCirc (Ind) < 0) return 0;
      }
    }

    dbi = Ind;
    ci_tab[dbi] = *ci1;
  }



  //TX_Print("DB_StoreCirc %ld %ld\n",Ind,dbi);



/*
  printf("DB_StoreCirc %ld  c=%f,%f,%f\n",dbi,ci1->pc.x,ci1->pc.y,ci1->pc.z);
  printf("  p1=%f,%f,%f  p2=%f,%f,%f\n",ci1->p1.x,ci1->p1.y,ci1->p1.z,
                                        ci1->p1.x,ci1->p1.y,ci1->p1.z);
  printf("  vz=%f,%f,%f\n",ci1->vz.dx,ci1->vz.dy,ci1->vz.dz);
*/


  return dbi;

}



//********************************************************************
  Circ DB_GetCirc (long Ind) {
//********************************************************************
// DB_get_CI

  Circ ci1;

  //TX_Print ("DB_GetCirc %d",Ind);





  if(Ind < 0) {
    if (Ind <= -DYN_CI_SIZ) {
      TX_Error("- Zugriff auf dyn. Circ %d ****",Ind);
      return ci_tab[0];
    }
    ci1 = ci_dyn[-Ind];


  } else if((Ind >= APT_CI_SIZ) || (DB_isFree_CI (&ci_tab[Ind]))) {
            // (ci_tab[Ind].p1.x == UT_VAL_MAX)) {
    if(Ind != 0) TX_Error (" Circ %d undefined",Ind);
    // ci1.p1 = DB_pt0;
    // ci1.p2 = DB_pt0;
    // ci1.pc = DB_pt0;
    ci1.rad = 0.0;
    // ci1 = DB_pt0;


  } else {
    ci1 = ci_tab[Ind];

  }


/*
  printf("DB_GetCirc %ld  c=%f,%f,%f\n",Ind,ci1.pc.x,ci1.pc.y,ci1.pc.z);
  printf("  p1=%f,%f,%f  p2=%f,%f,%f\n",ci1.p1.x,ci1.p1.y,ci1.p1.z,
                                        ci1.p1.x,ci1.p1.y,ci1.p1.z);
  printf("  vz=%f,%f,%f\n",ci1.vz.dx,ci1.vz.dy,ci1.vz.dz);
*/

  return ci1;

}



//====================================================================
  int DB_Error (int typ) {
//====================================================================
// Gleiche Fehlermeldungen nur beim ersten Mal

  static int typOld = 0;

  if(typ == typOld) return 0;

  typOld = typ;


  switch (typ) {

    case 1:
      TX_Error("Overflow DB_cSav");
      break;

    default:
      TX_Error("DB_Error Typ=%d",typ);
      break;
  }

  return 0;
}


//====================================================================
void* DB_cPos () {
//====================================================================

  // printf("DB_cPos\n");

  return DB_CSEG.next;

}


//====================================================================
void* DB_cSav (long size, void *data) {
//====================================================================
/// \code
/// <size> Bytes aus data nach DB_CDAT speichern.
/// Output:
///   RetCod:   the position in DB_CDAT AFTER storage  (unlike UME_save !)
///             NULL is Error !
/// Get active position with DB_cPos();
/// \endcode


  int    irc;


  // printf("DB_cSav siz=%ld free %d\n",size,UME_ck_free(&DB_CSEG));

  // round up to 4
  size = UTI_round_4up (size);

  // if(size < (long)sizeof(void*)) {
  if(size < 4) {
    TX_Error("DB_cSav E1");
    return NULL;
  }


  //if((DB_CSEG.next + size) > DB_CSEG.end) {
  if(((char*)DB_CSEG.next + size) > (char*)DB_CSEG.end) {
    irc = DB_allocCDAT (1);
    if(irc < 0) {
      // if(irc == -2) return NULL;    // EOM
      DB_Error (1);
      return NULL;
    }
  }


  memcpy(DB_CSEG.next, data, size);


  //DB_CSEG.next += size;
  DB_CSEG.next = (char*)DB_CSEG.next + size;


  return DB_CSEG.next;

}



//====================================================================
void* DB_cGet (void *data, void *pos, long size) {
//====================================================================
/// returns newPos = pos + size

  void *sPos;

  memcpy(data, pos, size);

  sPos = (char*)pos + size;

  return sPos;

}


/* UU
//====================================================================
void* DB_ObjG2Tab_sav (ObjGX *ox1) {
//====================================================================
/// \code
/// ObjG2-Objekte in Typ_ObjGX-Objekte umwandeln
/// DO NOT USE
/// save ObjG2Tabelle -> DB; return the Pos. of the ObjGXTabelle.
/// \endcode


  int       i1, irc;
  unsigned long oSiz;
  char      memOG2[64];
  void      *startPos, *cPos;
  ObjGX     *oxTab, oxm;
  ObjG2     *oG2;



  // Startpos holen
  startPos = DB_cPos ();


  // objSize holen
  oSiz = sizeof(ObjGX);  // ObjG2-Objekt -> Typ_ObjGX-Objekt

  // Musterobj. laden (form und data spaeter aus dem ObjG2)
  oxm.typ  = Typ_ObjGX;
  oxm.siz  = 1;


  // obj speichern
  for(i1=0; i1<ox1->siz; ++i1) {
    DB_cSav(oSiz, (void*)&oxm);
  }


  // daten speichen
  oxTab = startPos;
  cPos  = DB_cPos ();
  oG2   = ox1->data;

  for(i1=0; i1<ox1->siz; ++i1) {

    // save typ
    oxTab[i1].form = oG2[i1].typ;

    // datenadresse korrigieren
    oxTab[i1].data = cPos;


    // change ObjG2-Objekt -> data-Obj
    irc = UT2D_void_obj2 (memOG2, &oSiz, &oG2[i1]);
    if(irc < 0) return NULL;

    // save -> DB
    cPos = DB_cSav(oSiz, memOG2);

  }



  return startPos;

}
*/


//========================================================================
long DB_Store_hdr_so (ObjGX **oxo, int typ, int form, int siz, long ind) {
//========================================================================
/// get index for solid AND store parent-record



  if(ind < 0) {  // no dynamic solids !
    TX_Error("DB_Store_hdr_so E001_%ld",ind);
    return -1L;
  }


  //---------------- normal ---------------------------
  if(ind > APT_SO_IND) {
    APT_SO_IND = ind;

    if(ind >= APT_SO_SIZ-1) {    // realloc
      if(DB_allocSol (ind) < 0) return -1;
    }
  }

  *oxo = &so_tab[ind];

  // write parent-record
  OGX_SET_OBJ (*oxo, typ, form, siz, DB_cPos());

  return ind;

}


//====================================================================
long DB_Store_hdr_su (ObjGX **oxo, long Ind) {
//====================================================================
/// get surf-Index/Parent-obj
/// Output:
///   oxo       pointer into su_dyn|su_tab for surface A<Ind>
///   RetCod:   index into su_dyn|su_tab


  // printf("DB_Store_hdr_su %ld\n",Ind);



  // Dynam. Objekt erzeugen -
  if(Ind < 0) {
    Ind = DB_GetDynInd(Typ_SUR);
    *oxo = &su_dyn[-Ind];


  //---------------- normal ---------------------------
  } else {

    if(Ind > APT_SU_IND) {
      APT_SU_IND = Ind;

      if(Ind >= APT_SU_SIZ) {    // realloc
        if(DB_allocSur (Ind) < 0) return 0;
      }
    }

    *oxo = &su_tab[Ind];
  }

    // printf("ex DB_Store_hdr_su %d\n",Ind);

  return Ind;

}



//====================================================================
long DB_Store_hdr_cv (ObjGX **cvo, long Ind) {
//====================================================================
// get DB-index (for dynam.obj's only) and get pointer to parent-ox-record
// Output:
//   cvo       pointer to oGX-parent-record
//   Retcod:   db-index

  // printf("DB_Store_hdr_cv  %d\n",Ind);


  // Dynam. Objekt erzeugen -
  if(Ind < 0) {

    if(Ind == -1) {
      Ind = DB_GetDynInd(Typ_CV);
    } else if (-Ind > DYN_CV_SIZ) {
      TX_Error ("DB_Store_hdr_cv E001");
      Ind = DB_GetDynInd(Typ_CV);
    }
    *cvo = &cv_dyn[-Ind];


  //---------------- normal ---------------------------
  } else {
    if(Ind > APT_CV_IND) {
      APT_CV_IND = Ind;

      if(Ind >= APT_CV_SIZ) {    // realloc
        if(DB_allocCurve (Ind) < 0) return 0;
      }
    }

    *cvo = &cv_tab[Ind];
  }

  return Ind;

}



//====================================================================
long DB_Store_hdr_nt (ObjGX **txo, long Ind) {
//====================================================================


  // Dynam. Objekt erzeugen -
  if(Ind < 0) {
    Ind = DB_GetDynInd(Typ_GTXT);
    *txo = &tx_dyn[-Ind];


  //---------------- normal ---------------------------
  } else {

    if(Ind > APT_TX_IND) {
      APT_TX_IND = Ind;

      if(Ind >= APT_TX_SIZ) {    // realloc
        if(DB_allocNote (Ind) < 0) return 0;
      }
    }

    *txo = &tx_tab[Ind];
  }

  return Ind;

}

//====================================================================
long DB_StoreClot (long Ind, CurvClot *cvCl) {
//====================================================================
// speichert auch das ParentObj !!


  long      l1;
  ObjGX     *cvo;


  // printf("DB_StoreClot %d\n",Ind);

  Ind = DB_Store_hdr_cv (&cvo, Ind);

  cvo->typ    = Typ_CVCLOT;
  cvo->form   = Typ_CVCLOT;
  cvo->siz    = 1;
  cvo->data   = DB_cPos ();

  // save cv
  l1 = sizeof(CurvClot);
  DB_cSav(l1, (void*)cvCl);

  return Ind;

}


//====================================================================
long DB_StoreCvBsp (long Ind, CurvBSpl *cvbsp) {
//====================================================================
// speichert auch das ParentObj !!

  long      l1;
  void      *cPos1, *cPos2, *cPos3;
  ObjGX     *cvo;

  // printf("===================================== \n");
  // printf("DB_StoreCvBsp %d\n",Ind);
  // DEB_dump_obj__ (Typ_CVBSP, cvbsp, "S%d", Ind);

  cPos1 = DB_cPos ();

  // save knotvals
  l1 = sizeof(double) * (cvbsp->ptNr + cvbsp->deg + 1);
  cPos2 = DB_cSav(l1, (void*)cvbsp->kvTab);

  // save controlpoints
  l1 = sizeof(Point) * cvbsp->ptNr;
  cPos3 = DB_cSav(l1, (void*)cvbsp->cpTab);

  // save CurvBSpl at cPos3
  l1 = sizeof(CurvBSpl);
  DB_cSav(l1, (void*)cvbsp);

  // fix posis
  ((CurvBSpl*)cPos3)->kvTab = cPos1;
  ((CurvBSpl*)cPos3)->cpTab = cPos2;


  Ind = DB_Store_hdr_cv (&cvo, Ind);

  cvo->typ    = Typ_CVBSP;
  cvo->form   = Typ_CVBSP;
  cvo->siz    = 1;
  cvo->data   = cPos3;

  // printf(" CvBsp-posi=%p\n",cPos3);
  // DEB_dump_obj__ (Typ_CVBSP, cvo->data, "S%d", Ind);

  return Ind;

}


//====================================================================
long DB_StoreCvRBsp (long Ind, CurvRBSpl *cvbsp) {
//====================================================================
// speichert auch das ParentObj !!

  long      l1;
  void      *cPos1, *cPos2, *cPos3, *cPos4;
  ObjGX     *cvo;


  // printf("DB_StoreCvRBsp %d\n",Ind);
  // DEB_dump_obj__ (Typ_CVRBSP, cvbsp, "S%d-1:", Ind);


  cPos1 = DB_cPos ();

  // save weights
  l1 = sizeof(double) * cvbsp->ptNr;
  cPos2 = DB_cSav (l1, (void*)cvbsp->wTab);

  // save knotvals
  l1 = sizeof(double) * (cvbsp->ptNr + cvbsp->deg + 1);
  cPos3 = DB_cSav (l1, (void*)cvbsp->kvTab);

  // save controlpoints
  l1 = sizeof(Point) * cvbsp->ptNr;
  cPos4 = DB_cSav (l1, (void*)cvbsp->cpTab);

  // save CurvBSpl at cPos4
  l1 = sizeof(CurvRBSpl);
  DB_cSav (l1, (void*)cvbsp);

  // fix posis
  ((CurvRBSpl*)cPos4)->cpTab = cPos3;
  ((CurvRBSpl*)cPos4)->kvTab = cPos2;
  ((CurvRBSpl*)cPos4)->wTab  = cPos1;

  // get dbi & parent-record-posi
  Ind = DB_Store_hdr_cv (&cvo, Ind);

  // write parent-record
  cvo->typ    = Typ_CVRBSP;
  cvo->form   = Typ_CVRBSP;
  cvo->siz    = 1;
  cvo->data   = cPos4;

  // printf(" CvRBsp-posi=%p\n",cPos4);
  // DEB_dump_obj__ (Typ_CVRBSP, cvo->data, "S%d:2", Ind);

  return Ind;

}

//====================================================================
long DB_StoreCvPl2 (long Ind, CurvPol2 *cvplg, int iNew) {
//====================================================================
// save data cvplg -> DB and write parent-ox
//   iNew = 0; new data; save curve & Dataspace
//          1; Derived (copied) curve; do not save lvTab,cpTab

// iNew unused - UTO_CUT__ uses UTO_copy_stru !


  long      l1;
  void      *cPos1, *cPos2, *cPos3, *cPos4;
  ObjGX     *cvo;
  CurvPol2  cv1;


  // printf("DB_StoreCvPlg %ld iNew=%d\n",Ind,iNew);
  // DEB_dump_obj__ (Typ_CVPOL, cvplg, "plg");
      
  cv1 = *cvplg;   // copy

  // get DB-index (for dynam.obj's only) and get pointer to parent-ox-record
  Ind = DB_Store_hdr_cv (&cvo, Ind);


  cPos1 = DB_cPos ();

  // save cpTab
  l1 = sizeof(Point2) * cv1.ptNr;
  cPos2 = DB_cSav (l1, (void*)cv1.pTab);
  cv1.pTab = cPos1;
    
  // save curve
  l1 = sizeof(CurvPol2);
  cPos3 = DB_cSav (l1, &cv1);
  if(cPos3 == NULL) return -2;


  cvo->typ  = Typ_CVPOL2;
  cvo->form = Typ_CVPOL2;
  cvo->siz  = 1;
  cvo->data = cPos2;

  return Ind;

}

//====================================================================
long DB_StoreCvPsp (long Ind, CurvPsp3 *cvpsp, int iNew) {
//====================================================================
// save data cvplg -> DB and write parent-ox
//   iNew = 0; new data; save curve & Dataspace
//          1; Derived (copied) curve; do not save lvTab,cpTab

// iNew unused - UTO_CUT__ uses UTO_copy_stru !


  long      l1;
  void      *cPos1, *cPos2, *cPos3;
  ObjGX     *cvo;
  CurvPsp3  cv1;


  // printf("DB_StoreCvPsp %ld iNew=%d\n",Ind,iNew);
  // DEB_dump_obj__ (Typ_CVPOL, cvplg, "plg");

  // cv1 = *cvpsp;   // copy

  // get DB-index (for dynam.obj's only) and get pointer to parent-ox-record
  Ind = DB_Store_hdr_cv (&cvo, Ind);


  cPos1 = DB_cPos ();

  // save polynome at cPos1
  l1 = sizeof(polynom_d3) * cvpsp->plyNr;
  cPos2 = DB_cSav(l1, (void*)cvpsp->plyTab);


  // save CurvPsp3 at cPos2
  l1 = sizeof(CurvPoly);
  cPos3 = DB_cSav (l1, cvpsp);
  if(cPos3 == NULL) return -2;


  // fix pos
  ((CurvPsp3*)cPos2)->plyTab = cPos1;


  cvo->typ  = Typ_CVPSP3;
  cvo->form = Typ_CVPSP3;
  cvo->siz  = 1;
  cvo->data = cPos2;

  return Ind;

}




//====================================================================
long DB_StoreCvPlg (long Ind, CurvPoly *cvplg, int iNew) {
//====================================================================
// save data cvplg -> DB and write parent-ox
//   iNew = 0; new data; save curve & Dataspace
//          1; Derived (copied) curve; do not save lvTab,cpTab

// iNew unused - UTO_CUT__ uses UTO_copy_stru !


  long      l1;
  void      *cPos1, *cPos2, *cPos3, *cPos4;
  ObjGX     *cvo;
  CurvPoly  cv1;


  // printf("DB_StoreCvPlg %ld iNew=%d\n",Ind,iNew);
  // DEB_dump_obj__ (Typ_CVPOL, cvplg, "plg");

  cv1 = *cvplg;   // copy

  // get DB-index (for dynam.obj's only) and get pointer to parent-ox-record
  Ind = DB_Store_hdr_cv (&cvo, Ind);


  cPos1 = DB_cPos ();

  // save cpTab
  l1 = sizeof(Point) * cv1.ptNr;
  cPos2 = DB_cSav (l1, (void*)cv1.cpTab);
  cv1.cpTab = cPos1;


  // save lvTab
  // if(!iNew) {
  // if(cv1.lvTab) {
    // save lvTab
    l1 = sizeof(double) * cv1.ptNr;
    cPos3 = DB_cSav (l1, (void*)cv1.lvTab);
    cv1.lvTab = cPos2;
  // }


  // save curve
  l1 = sizeof(CurvPoly);
  cPos4 = DB_cSav (l1, &cv1);
  if(cPos4 == NULL) return -2;


  cvo->typ  = Typ_CVPOL;
  cvo->form = Typ_CVPOL;
  cvo->siz  = 1;
  cvo->data = cPos3;

  return Ind;

}


/* UNUSED
//================================================================
  long DB_StoreObjGX (long Ind, ObjGX *cvi) {
//================================================================
// see also DB_StoreCvBsp

  long      l1;
  void      *cPos1;
  ObjGX     *cvo;

  
  printf("===================================== \n");
  printf("DB_StoreObjGX %d\n",Ind); 


  // get adress where to stor primary obj
  Ind = DB_Store_hdr_cv (&cvo, Ind);
      
  cPos1 = DB_cPos ();

  cvo->typ    = Typ_CVTRM;
  cvo->form   = Typ_CVTRM;
  cvo->siz    = cvi->siz;
  cvo->data   = cPos1;


  // save subparts of ccv
  l1 = Ind;
  DB_store_obj (&cPos1, Typ_ObjGX, cvi->data, cvi->siz, &l1);


    // for(l1=0; l1<cvNew->segNr; ++l1)
    // DEB_dump_obj__ (Typ_ObjGX, &((ObjGX*)cvNew->cvtab)[l1], "[%d]", l1);

    printf("ex DB_StoreObjGX %d\n",Ind);

  return Ind;

}
*/


//================================================================
  long DB_StoreCvCCV (long Ind, CurvCCV *cvi, int iNr) {
//================================================================
// Input:
//   Ind     DB-Index; -1 for new dynamic-DB-obj
//   iNr     nr of trimmed-curves in cvi

  long      l1;
  void      *cPos1;
  ObjGX     *cvo;
  CurvCCV   *cvNew;
    
    
  // printf("===================================== \n");
  // printf("DB_StoreCvCCV %d\n",Ind);
  // DEB_dump_obj__ (Typ_CVTRM, cvi, "S%d", Ind);

  // get adress where to stor primary obj
  Ind = DB_Store_hdr_cv (&cvo, Ind);        
      
  // save CurvCCV
  cvNew = DB_cPos ();
  DB_cSav(sizeof(CurvCCV) * iNr, (void*)cvi);


  cvo->typ    = Typ_CVTRM;
  cvo->form   = Typ_CVTRM;
  cvo->siz    = iNr;
  cvo->data   = cvNew;

  return Ind;

}


//====================================================================
long DB_StoreCvEll (long Ind, CurvPoly *cvell) {
//====================================================================

  long      l1;
  ObjGX     *cvo;


  // printf("DB_StoreCvEll %d\n",Ind);

  Ind = DB_Store_hdr_cv (&cvo, Ind);

  cvo->typ    = Typ_CVELL;
  cvo->form   = Typ_CVELL;
  cvo->siz    = 1;
  cvo->data   = DB_cPos ();

  // save Elli
  l1 = sizeof(CurvElli);
  DB_cSav(l1, (void*)cvell);

  return Ind;

}


//====================================================================
long DB_StoreCurv (long Ind, ObjGX *cv1, int iNew) {
//====================================================================
/// \code
/// 
/// Input:
///   Ind    0 = used for temp.object (in statu nascendi, design-process)
///         -1   get new dynamic cv
///         >0   normal
///   iNew = 0  new data; save curve & Dataspace
///          1  Derived (copied) curve; do not save curve and dataspace
///          2  trimmed curve; do not save dataspace (only ObjGX-Rec.i and curve)
///             Derived used by CUT ..
/// RetCod:  DB-Index
/// \endcode

// ACHTUNG: iNew dzt nur f. CVBSP !


/* zerlegen:
 Typ_CVPSP3
 Typ_CVPOL
 Typ_CVPOL2
 Typ_CVBSP
 Typ_CVRBSP
 Typ_CVELL
 Typ_CVTRM
 Typ_CVTRM2
 Typ_CVPSP3
*/



  void      *cPos;
  int       irc, typ, form, i1;
  long      l1;
  void      *cPos1, *cPos2, *cPos3;
  ObjGX     *ox1, *oTab, *cvo;
  CurvPoly   *cvplg;
  CurvBSpl  *cvbsp;


  // printf("======================================================== \n");
  // printf("\nDB_StoreCurv DBI=%ld typ=%d form=%d siz=%d iNew=%d\n",
          // Ind,cv1->typ,cv1->form,cv1->siz,iNew);
  // DEB_dump_ox_0 (cv1, "DB_StoreCurv");
  // printf("after dump 1 DB_StoreCurv --------------------\n");



  typ  = cv1->typ;
  form = cv1->form;



  //============ Typ_LN =======================
  if(form != Typ_LN) goto L_typ_psp3;

  // STORE POLYGON AS CURVE, NOT AS LINE !
  Ind = DB_Store_hdr_cv (&cvo, Ind);  // get pointer --> cv_tab or cv_dyn

  cvo->typ    = cv1->typ;
  cvo->form   = cv1->form;
  cvo->siz    = cv1->siz;
  cvo->data  = DB_cPos ();

  // save lines
  l1 = sizeof(Line) * cv1->siz;
  DB_cSav(l1, (void*)cv1->data);

  goto L_fertig;




  //============ Typ_CVPSP3 =======================
  L_typ_psp3:
  if(form != Typ_CVPSP3) goto L_typ_pol;

  Ind = DB_StoreCvPsp (Ind, cv1->data, iNew);

//   Ind = DB_Store_hdr_cv (&cvo, Ind);  // get pointer --> cv_tab or cv_dyn
//   cvo->typ    = cv1->typ;
//   cvo->form   = cv1->form;
//   cvo->siz    = cv1->siz;
//   cvo->data  = DB_cPos ();
//   // save polynome
//   l1 = sizeof(polynom_d3) * cv1->siz;
//   DB_cSav(l1, (void*)cv1->data);

  goto L_fertig;



  //============ Typ_CVPOL =======================
  L_typ_pol:
  if(form != Typ_CVPOL) goto L_typ_pol2;
  Ind = DB_StoreCvPlg (Ind, cv1->data, iNew);
  goto L_fertig;


  //============ Typ_CVPOL2 =======================
  L_typ_pol2:
  if(form != Typ_CVPOL2) goto L_typ_bsp;
  Ind = DB_StoreCvPl2 (Ind, cv1->data, iNew);
/*
  Ind = DB_Store_hdr_cv (&cvo, Ind);  // get pointer --> cv_tab or cv_dyn
  cvo->typ    = cv1->typ;
  cvo->form   = cv1->form;
  cvo->siz    = cv1->siz;
  cvo->data  = DB_cPos ();
  // save points
  l1 = sizeof(Point2) * cv1->siz;
  DB_cSav(l1, (void*)cv1->data);
*/
  goto L_fertig;



  //============ Typ_CVBSP =======================
  L_typ_bsp:
  if(form != Typ_CVBSP) goto L_typ_rbsp;

  if(iNew == 1) {
    // copy ObjGX only  (duplicate; trimming unmodified)
    Ind = DB_Store_hdr_cv (&cvo, Ind);  // get pointer --> cv_tab or cv_dyn
    *cvo = *cv1;    // copy ParentObj
  } else {  // 0, 2
    // save ObjGX & data 
    Ind = DB_StoreCvBsp (Ind, cv1->data);
  }
  return Ind;


  //============ Typ_CVRBSP =======================
  L_typ_rbsp:
  if(form != Typ_CVRBSP) goto L_typ_ell;
  Ind = DB_StoreCvRBsp (Ind, cv1->data);
  return Ind;


  //============ Typ_CVELL =======================
  L_typ_ell:
  if(form != Typ_CVELL) goto L_typ_clot;
  Ind = DB_StoreCvEll (Ind, cv1->data);
  goto L_fertig;


  //============ Typ_CVCLOT =======================
  L_typ_clot:
  if(form != Typ_CVCLOT) goto L_typ_cvc;
  Ind = DB_StoreClot (Ind, cv1->data);
  goto L_fertig;


  //============ Typ_CVTRM =======================
  L_typ_cvc:
  if(form != Typ_CVTRM) goto L_typ_nn;
  Ind = DB_StoreCvCCV (Ind, cv1->data, cv1->siz);
  goto L_fertig;


  //==============================================
  L_typ_nn:
  Ind = -1;
  TX_Error("DB_StoreCurv unknown form %d",form);
  DEB_dump_ox_0 (cv1, "ERR-DB_StoreCurv");



  L_fertig:
  // printf("ex DB_StoreCurv\n");

  return Ind;


  //========================================
  L_EOM:
  TX_Error("DB_StoreCurv out of memory");
  return -1;
}


//=======================================================================
  ObjGX* DB_GetCurv (long Ind) {
//=======================================================================
/// see also DB_get_CV

  ObjGX       *cv_out;


  // printf("DB_GetCurv %ld\n",Ind);


  if(Ind < 0) {
    Ind = -Ind;
    if (Ind >= DYN_CV_SIZ) {
      printf("******* DB_GetCurv I1 %ld\n",Ind);
      Ind = DYN_CV_SIZ - 1;
    }
    cv_out = &cv_dyn[Ind];

  } else {
    if (Ind >= APT_CV_SIZ) {
      printf("******* DB_GetCurv I2 %ld\n",Ind);
      Ind = APT_CV_SIZ - 1; 
    }
    cv_out = &cv_tab[Ind];
  }


  L_exit:

    // TESTBLOCK
    // printf("ex DB_GetCurv %ld typ=%d form=%d\n",Ind,cv_out->typ,cv_out->form);
    // if(cv_out->typ != Typ_Error) {
      // DEB_dump_ox_0 (cv_out, "ex DB_GetCurv\n");
      // // DEB_dump_ox_s_ (cv_out, "ex DB_GetCurv\n");
    // } else {
      // printf("************* Curve %ld undefined\n",Ind);
    // }
    // DEB_exit();
    // END TESTBLOCK
 
  return cv_out;

}

/*

//=======================================================================
long DB_StoreCurv (long Ind, Curv *cv1) {
//=======================================================================
// Wenn Ind>0: 
//  die Kurvenpunkte aus der Datei lesen und als dynam. Punkte anlegen
//  (Ind ist die Curv#).

// Wenn Ind = -1:
//   die Punkte der zugehörigen APT-Kurve (cv1->cvnr) verwenden.


// Input:
//   Anf/EndPkte:    cv1->p1 / cv1->p2
//   der Punktnummeroffset des ersten/letzten Polygonzwischenpunktes cv1->ind1/2
//     (0=der erste Zwischenpunkt ist cv1->ID1; 1:  (cv1->ID1)+1)

// Output
//   die dyn.PunktIndexe der Zwischenpunkte: cv1->ID1, cv1->ID2 
//   nur beim Neuanlegen; sonst einfach übernehmen.



// Achtung: wenn Ind<0, dann muss ein dynam. Objekt noch
//  erzeugt werden !


  int         i1;
  long        dbi, dbp;
  Point       pt1;
  // Point2      pt20;

  FILE        *auxFilLun;





  // Dynam. Objekt erzeugen -
  if(Ind < 0) {
    dbi = DB_GetDynInd(Typ_CV);
    Ind = cv1->cvnr;

    cv1->ID1 = cv_tab[Ind].ID1;
    cv1->ID2 = cv_tab[Ind].ID2;


    goto Fertig;



  } else {
    dbi = Ind;
  }

  // nur Test:
  //return dbi;





  //---------------------------------------------------------------------
  // ALLE  Punkte aus Datei bin - auslesen und als dynam. Punkte erzeugen
  UTCV_open_r (&auxFilLun, (int)Ind);

  i1 = 0;
  while (!feof (auxFilLun)) {

    if(fread (&pt1, sizeof(Point), 1, auxFilLun) == 0) break;
    //TX_Print(" next Vertex %d %f %f\n",i1, pt20.x, pt20.y);

    dbp = DB_GetDynInd(Typ_PT);
    // pt_tab[dbp] = UT3D_pt_pt2(&pt20);
    pt_tab[dbp] = pt1;

    // dyn.Ind des ersten Punktes in ID1 merken
    if(i1 == 0) cv1->ID1 = dbp;
    ++i1;

  }

  // dyn.Ind des letzten Punktes in ID2 merken
  cv1->ID2=dbp;

  fclose (auxFilLun);





  //=========================================================================
  Fertig:

  //TX_Print("ex DB_StoreCurv: ind1/2=%d/%d ID1/2=%d/%d",
  //          cv1->ind1,cv1->ind2,cv1->ID1,cv1->ID2);

  cv_tab[dbi] = *cv1;


  return dbi;

}
*/


//================================================================
  int DB_StoreTra (long Ind, ObjGX *ox1) {
//================================================================

  ObjGX     *oxo;
  void      *cPos1, *cPos2;


  // printf("DB_StoreTra %ld typ=%d form=%d\n",Ind,ox1->typ,ox1->form);


  if(Ind < 0) {
    TX_Error("DB_StoreTra E001 %d",Ind);
    return -1;
  }

    if(Ind > APT_TR_IND) {
      APT_TR_IND = Ind;

      if(Ind >= APT_TR_SIZ) {    // realloc
        if(DB_allocTra (Ind) < 0) return 0;
      }
    }


  oxo   = &tra_tab[Ind];
  *oxo  = *ox1;          // copy ObjGX



  //================================================================
  if(ox1->form == Typ_VC) {

    // besteht aus einem Vector
    cPos1 = DB_cPos ();       // Startpos Vector
    cPos2 = DB_cSav (sizeof(Vector), ox1->data);
    oxo->data   = cPos1;      // Startpos Vector


  //================================================================
  } else if(ox1->form == Typ_TraRot) {
 
    // besteht aus einem TraRot
    cPos1 = DB_cPos ();       // Startpos
    cPos2 = DB_cSav (sizeof(TraRot), ox1->data);
    oxo->data   = cPos1;      // Startpos


  //================================================================
  } else {
    TX_Error("DB_StoreTra E002_%ld",ox1->form);
    return -1;
  }
 
  return 0;

}


//================================================================
  ObjGX* DB_GetTra (long Ind) {
//================================================================
/// rc = -1 = Fehler (undefined ...)


  if((Ind < 0)||(Ind >= APT_TR_SIZ)) {
    TX_Error("- Zugriff auf Tra %d ****",Ind);
    return NULL;

  // } else if(tra_tab[Ind].typ == Typ_Error) {
  } else if(DB_isFree_Tra (&tra_tab[Ind])) {
    TX_Error (" Tra %d undefined",Ind);
    return NULL;
  }

  return &tra_tab[Ind];

}

/*
//================================================================
void DB_StoreTool (long Ind, BTool *tl1) {
//================================================================

  //TX_Print("DB_StoreTool %d %f\n",Ind, tl1->dmr);

  if(Ind < 0) {
    TX_Error("DB_StoreTool E001 %d",Ind);
    return;
  }

    if(Ind > APT_TL_IND) {
      APT_TL_IND = Ind;

      if(Ind >= APT_TL_SIZ) {    // realloc
        if(DB_allocTool (Ind) < 0) return;
      }
    }


  tl_tab[Ind] = *tl1;

  return;

}
*/

//=============================================================
  Activity* DB_get_Activ (long Ind) {
//=============================================================
 

  if(Ind >= APT_AC_SIZ) Ind = APT_AC_SIZ - 1;
  return &ac_tab[Ind];

}


//=============================================================
  int DB_StoreActiv (long Ind, Activity *data) {
//=============================================================
/// DB_StoreActiv      save Activity
  
  int      i1;
  Activity *ac1;
  char     *p1;


  // printf("=================================== \n");
  // printf("DB_StoreActiv %d\n",Ind);
  // DEB_dump_obj__ (Typ_Activ, data, "in DB_StoreActiv:");


  if(Ind < 0) {
    TX_Error("DB_StoreActiv E001_%ld",Ind);
    return -1;

  } else {

    if(Ind > APT_AC_IND) {
      APT_AC_IND = Ind;

      if(Ind >= APT_AC_SIZ) {    // realloc
        if(DB_allocAct (Ind) < 0) return 0;
      }
    }

    ac1 = &ac_tab[Ind];   // set Pointer to DB-struct
  }



  *ac1 = *data;                    // copy activity --> DB

  ac1->data = DB_cPos ();          // replace Pointer --> Text

  // den Text kopieren; skip lead. u. foll. "
  p1 = (char*)data->data;
  if(*p1 == '"') ++p1;
  // printf(" save |%s|\n",p1);

  i1 = strlen(p1) + 1;        // incl termin. '\0'
  // printf(" |%c| %d\n",p1[i1-2],i1);
  if(p1[i1-2] == '"') --i1;   // skip last "

  DB_cSav (i1, p1);                // save text
  
  // terminate in CSEG
  p1 = (char*)ac1->data;
  p1[i1-1] = '\0';
  // printf(" |%c| %d\n",p1[i1-1],i1);
  

  // DEB_dump_obj__ (Typ_Activ, &ac_tab[Ind], "ex DB_StoreActiv:");
  // DB_dump_Activ ();

  return 0;

}


//================================================================
  int DB_StoreJointNr (int jntNr) {
//================================================================
// DB_StoreJointNr    report last used jointNr
// necessary for getting next free jointNr
// DB does not keep the joint-data. Only DB_JNT_IND ist stored/restored.


  DB_JNT_IND = jntNr;

  return 0;

}

/*
// ********************************************************************
int DB_GetTool (BTool* tl1, long Ind) {
// ********************************************************************
// rc = -1 = Fehler (Tool undefined ...)





  if((Ind < 0)||(Ind >= APT_TL_SIZ)) {
    TX_Error("- Zugriff auf Tool %d ****",Ind);
    return -1;

  // } else if(DB_isFree_? (&mdr_tab[i1])) {
  } else if(tl_tab[Ind].dmr == UT_VAL_MAX) {
    TX_Error (" Tool %d undefined",Ind);
    return -1;


  } else {
    *tl1 = tl_tab[Ind];

  }

  return 1;

}
*/




//********************************************************************
int DB_GetConnObj (int* typ1, long* ind1, int* typ2, long* ind2, Point2 *pt1) {
//********************************************************************
/// \code
/// Jene Elemente suchen und Typ und Index ausgeben,
///   deren Endpunkte auf pt1 liegen.
/// 
/// RetCode: Anzahl der gefundenen Elemente; 0=nix,
///   1 = ein Obj, Ind out in ind_1
///   2 = zwei Obj, in ind_1 und ind_2
///   3 oder mehr möglich !
/// \endcode


  int      irc;
  Point2   EndP;
  long     i1;


  //TX_Print("DB_GetConnObj %f %f",pt1->x,pt1->y);


  irc = 0;


  // Zuerst Loop durch alle Lines
  for (i1 = 0; i1<APT_LN_SIZ; ++i1) {

    // if(ln_tab[i1].p1.x == UT_VAL_MAX) goto NextLine;
    if(DB_isFree_LN (&ln_tab[i1])) goto NextLine;

    EndP=UT2D_pt_pt3(&ln_tab[i1].p1);
    if((UT2D_comp2pt(pt1, &EndP, UT_TOL_ln)) == 1) goto FoundLine;

    EndP=UT2D_pt_pt3(&ln_tab[i1].p2);
    if((UT2D_comp2pt(pt1, &EndP, UT_TOL_ln)) != 1) goto NextLine;

    FoundLine:;
      ++irc;
      if(irc == 1) {
        *typ1 = Typ_LN2;
        *ind1 = i1;
      } else if (irc == 2) {
        *typ2 = Typ_LN2;
        *ind2 = i1;
      }


    NextLine:;
  }



  // Dann durch alle Cir's
  for (i1 = 0; i1<APT_CI_SIZ; ++i1) {

    // if(ci_tab[i1].p1.x == UT_VAL_MAX) goto NextArc;
    if(DB_isFree_CI (&ci_tab[i1])) goto NextArc;

    EndP=UT2D_pt_pt3(&ci_tab[i1].p1);
    if((UT2D_comp2pt(pt1, &EndP, UT_TOL_ln)) == 1) goto FoundArc;

    EndP=UT2D_pt_pt3(&ci_tab[i1].p2);
    if((UT2D_comp2pt(pt1, &EndP, UT_TOL_ln)) != 1) goto NextArc;

    FoundArc:;
      ++irc;
      if(irc == 1) {
        *typ1 = Typ_CI2;
        *ind1 = i1;
      } else if (irc == 2) {
        *typ2 = Typ_CI2;
        *ind2 = i1;
      }


    NextArc:;

  }


  //TX_Print("ex DB_GetConnObj %d %d %ld %d %ld",irc,*typ1,*ind1,*typ2,*ind2);

  return irc;

}




//********************************************************************
long   DB_GetObjTyp2Pt  (int *typ, Point2 *pt1, Point2 *pt2) {
//********************************************************************
/// \code
/// Jenes Element vom Typ typ suchen, dess Endpunkte pt1 / pt2 sind.
/// Output:
///   RetCode: der Index; -1 = nix gefunden.
///   typ
/// \endcode


  int      i1, i2;
  long     ind;
  Point2   EndP1, EndP2;


  //TX_Print ("DB_GetCirc %d",Ind);




  // Loop durch alle Lines
  for (ind = 0; ind<APT_LN_SIZ; ++ind) {

    *typ = Typ_LN2;

    // if(ln_tab[ind].p1.x == UT_VAL_MAX) goto NextLine;
    if(DB_isFree_LN (&ln_tab[ind])) goto NextLine;

    EndP1=UT2D_pt_pt3(&ln_tab[ind].p1);
    EndP2=UT2D_pt_pt3(&ln_tab[ind].p2);

    i1 = UT2D_comp2pt(pt1, &EndP1, UT_TOL_ln);
    i2 = UT2D_comp2pt(pt2, &EndP2, UT_TOL_ln);

    if((i1 == 1)&&(i2 == 1)) goto Found;


    i1 = UT2D_comp2pt(pt2, &EndP1, UT_TOL_ln);
    i2 = UT2D_comp2pt(pt1, &EndP2, UT_TOL_ln);

    if((i1 == 1)&&(i2 == 1)) goto Found;

    NextLine:;
  }




  // Check alle Cir's
  for (ind = 0; ind<APT_CI_SIZ; ++ind) {

    *typ = Typ_CI2;

    // if(ci_tab[ind].p1.x == UT_VAL_MAX) goto NextArc;
    if(DB_isFree_CI (&ci_tab[ind])) goto NextArc;

    EndP1=UT2D_pt_pt3(&ci_tab[ind].p1);
    EndP2=UT2D_pt_pt3(&ci_tab[ind].p2);

    i1 = UT2D_comp2pt(pt1, &EndP1, UT_TOL_ln);
    i2 = UT2D_comp2pt(pt2, &EndP2, UT_TOL_ln);

    if((i1 == 1)&&(i2 == 1)) goto Found;


    i1 = UT2D_comp2pt(pt2, &EndP1, UT_TOL_ln);
    i2 = UT2D_comp2pt(pt1, &EndP2, UT_TOL_ln);

    if((i1 == 1)&&(i2 == 1)) goto Found;

    NextArc:;
  }

  *typ = Typ_Error;
  ind  = -1;


  Found:
  //TX_Print(" ex DB_GetObjTyp2Pt %d %d",*typ,ind);

  return ind;

}


//====================================================================
  int DB_Query_siz (long *cSiz) {
//====================================================================

  *cSiz = (char*)DB_CSEG.next - (char*)DB_CSEG.start;

  return 0;

}


//====================================================================
  int DB_QueryDef (int typ, long ind) {
//====================================================================
/// \code
/// check if obj is defined in DB;
/// RetCod
///   0 = jes, OK
///  -1 = obj undefined.
/// \endcode

// Achtung: typ muss Basistyp sein; see AP_typDB_typ

  // printf("DB_QueryDef %d %d\n",typ,ind);


  if(typ == Typ_PT) {
    if(ind >= 0) {
      if((ind > APT_PT_SIZ)||(DB_isFree_PT(&pt_tab[ind]))) goto L_err;
    } else {
      if((-ind > DYN_PT_SIZ)||(DB_isFree_PT(&pt_dyn[-ind]))) goto L_err;
    }
    goto L_OK;


  } else if(typ == Typ_VC) {
    if(ind >= 0) {
      if((ind > APT_VC_SIZ)||(DB_isFree_VC(&vc_tab[ind]))) goto L_err;
    } else {
      if((-ind > DYN_VC_SIZ)||(DB_isFree_VC(&vc_dyn[-ind]))) goto L_err;
    }
    goto L_OK;


  } else if(typ == Typ_LN) {
    if(ind >= 0) {
      if((ind > APT_LN_SIZ)||(DB_isFree_LN(&ln_tab[ind]))) goto L_err;
    } else {
      if((-ind > DYN_LN_SIZ)||(DB_isFree_LN(&ln_dyn[-ind]))) goto L_err;
    }
    goto L_OK;


  } else if(typ == Typ_CI) {
    if(ind >= 0) {
      if((ind > APT_CI_SIZ)||(DB_isFree_CI(&ci_tab[ind]))) goto L_err;
    } else {
      if((-ind > DYN_CI_SIZ)||(DB_isFree_CI(&ci_dyn[-ind]))) goto L_err;
    }
    goto L_OK;


  } else if((typ == Typ_CV)     ||
            (typ == Typ_CVTRM))    {
    if(ind >= 0) {
      if((ind > APT_CV_SIZ)||(DB_isFree_CV(&cv_tab[ind]))) goto L_err;
    } else {
      if((-ind > DYN_CV_SIZ)||(DB_isFree_CV(&cv_dyn[-ind]))) goto L_err;
    }
    goto L_OK;


  } else if(typ == Typ_SUR) {
    if(ind >= 0) {
      if((ind > APT_SU_SIZ)||(DB_isFree_Sur(&su_tab[ind]))) goto L_err;
    } else {
      if((-ind > DYN_SU_SIZ)||(DB_isFree_Sur(&su_dyn[-ind]))) goto L_err;
    }
    goto L_OK;


  } else if(typ == Typ_SOL) {
    if((ind < 1)||(ind > APT_SO_SIZ)||
      (DB_isFree_Sol(&so_tab[ind]))) goto L_err;
    goto L_OK;


  } else if(typ == Typ_Activ) {
    if((ind < 1)||(ind > APT_AC_SIZ)||
      (DB_isFree_Act(&ac_tab[ind]))) goto L_err;
    goto L_OK;


  } else if(typ == Typ_PLN) {      // Refsys
    if(ind >= 0) {
      if((ind > APT_PL_SIZ)||(DB_isFree_PLN(&pln_tab[ind]))) goto L_err;
    } else {
      if((-ind > DYN_PL_SIZ)||(DB_isFree_PLN(&pln_dyn[-ind]))) goto L_err;
    }
    goto L_OK;


  } else if(typ == Typ_Tra) {
    if((ind < 1)||(ind > APT_TR_SIZ)||
      (DB_isFree_Tra(&tra_tab[ind]))) goto L_err;
    goto L_OK;


  } else if((typ == Typ_Model) ||        // ModelRef
            (typ == Typ_Mock))    {      // 
    if((ind < 1)||(ind > APT_MR_SIZ)||
      (DB_isFree_ModRef(&mdr_tab[ind]))) goto L_err;
    goto L_OK;


  } else if((typ == Typ_Note)  ||    // Text
            (typ == Typ_GTXT)  ||    // Text
            (typ == Typ_Tag))    {      // 
    if(ind >= 0) {
      if((ind > APT_TX_SIZ)||(DB_isFree_GTxt(&tx_tab[ind]))) goto L_err;
    } else {
      if((-ind > DYN_TX_SIZ)||(DB_isFree_GTxt(&tx_dyn[-ind]))) goto L_err;
    }
    goto L_OK;


  } else if(typ == Typ_VAR) {
    if(ind >= 0) {
      if((ind > APT_VR_SIZ)||(DB_isFree_Var(&vr_tab[ind]))) goto L_err;
    } else {
      if((-ind > DYN_VR_SIZ)||(DB_isFree_Var(&vr_dyn[-ind]))) goto L_err;
    }
    goto L_OK;


  } else {
     printf("DB_QueryDef E001_%d\n",typ);
  }


  L_OK:
  return 0;


  L_err:
    // printf("ex DB_QueryDef -1 typ=%d ind=%ld\n",typ,ind);
  return -1;

}


//====================================================================
  long DB_dbo_get_free (int typ) {
//====================================================================
/// \code
/// returns next free DB-index; 0=Error
/// \endcode

// was DB_dbo_get_free

// see also DB_QueryNxtUsed

  int     ie;
  long    i1;


  // printf("DB_dbo_get_free %d\n",typ);


  if(typ == Typ_VAR) {
    return APT_VR_IND;


  } else if(typ == Typ_VC) {
    return APT_VC_IND;


  } else if(typ == Typ_PT) {
    return APT_PT_IND;


  } else if(typ == Typ_LN) {
    return APT_LN_IND;


  } else if(typ == Typ_CI) {
    return APT_CI_IND;


  } else if(typ == Typ_CV) {
    return APT_CV_IND;


  } else if(typ == Typ_SUR) {
    return APT_SU_IND;


  } else if(typ == Typ_SOL) {
    return APT_SO_IND;


  } else if(typ == Typ_PLN) {      // Refsys
    return APT_PL_IND;


  } else if(typ == Typ_Tra) {
    return APT_TR_IND;


  } else if(typ == Typ_Model) {      // M = ModelRef
    return APT_MR_IND;


  } else if((typ < Typ_Joint)&&(typ >= Typ_Note)) {      // N = Text
    return APT_TX_IND;


  } else if(typ == Typ_Activ) {      // I = Activity
    return APT_AC_IND;


  } else if(typ == Typ_VAR) {
    return APT_VR_IND;
  }


  TX_Print("DB_dbo_get_free unknown typ %d",typ);


  return 0;

}


//====================================================================
  long DB_QueryPrvUsed (int typ, long istart) {
//====================================================================
/// \code
/// DB_QueryPrvUsed    get previous used index
/// RetCode:
///    0      Error; no obj with index istart or higher exists
///    >0     index of existing obj. Is equal to istart or higher.
/// \endcode

// see also DB_dbo_get_free


  int     ie;
  long    i1;


  // printf("DB_QueryPrvUsed %d %ld\n",typ,istart);


  if(typ == Typ_VC) {
    for (i1 = istart; i1>=0; --i1) {
      if(DB_isFree_VC (&vc_tab[i1])) continue;
      return i1;
    }


  } else if(typ == Typ_VAR) {
    for (i1 = istart; i1>=0; --i1) {
      if(DB_isFree_Var (&vr_tab[i1])) continue;
      return i1;
    }


  } else if(typ == Typ_Tra) {
    for (i1 = istart; i1>=0; --i1) {
      if(DB_isFree_Tra (&tra_tab[i1])) continue;
      return i1;
    }


  } else TX_Print("DB_QueryPrvUsed: objTyp %d not yet supported",typ);


  return 0;

}


//====================================================================
  long DB_QueryNxtUsed (int typ, long istart) {
//====================================================================
/// \code
/// DB_QueryNxtUsed       get next used DB-index, starting from istart
/// RetCode:
///    0      Error; no obj with index istart or higher exists
///    >0     index of existing obj. Is equal to istart or higher.
/// \endcode

// see also DB_dbo_get_free


  int     ie;
  long    i1;


  // printf("DB_QueryNxtUsed %d %ld\n",typ,istart);


  if(typ == Typ_VC) {
    for (i1 = istart; i1<APT_VC_SIZ; ++i1) {
      if(DB_isFree_VC (&vc_tab[i1])) continue;
      return i1;
    }


  } else if(typ == Typ_VAR) {
    for (i1 = istart; i1<APT_VR_SIZ; ++i1) {
      if(DB_isFree_Var (&vr_tab[i1])) continue;
      return i1;
    }


  } else if(typ == Typ_Tra) {
    for (i1 = istart; i1<APT_TR_SIZ; ++i1) {
      if(DB_isFree_Tra (&tra_tab[i1])) continue;
      return i1;
    }


  } else TX_Print("DB_QueryNxtUsed: objTyp %d not yet supported",typ);

  return 0;

}


//==================================================================
   int DB_Query__ (void **oTab, long *indLast, long *indMax, int typ) {
//==================================================================
// DB_Query__         get dataTable, lastIndex, tabSiz from typ
// indLast       last used index


  if(typ == Typ_VC) {
    *oTab    = vc_tab;
    *indLast = APT_VC_IND;
    *indMax  = APT_VC_SIZ;

  } else if(typ == Typ_PT) {
    *oTab    = pt_tab;
    *indLast = APT_PT_IND;
    *indMax  = APT_PT_SIZ;

  } else if(typ == Typ_LN) {
    *oTab    = ln_tab;
    *indLast = APT_LN_IND;
    *indMax  = APT_LN_SIZ;

  } else if(typ == Typ_CI) {
    *oTab    = ci_tab;
    *indLast = APT_CI_IND;
    *indMax  = APT_CI_SIZ;

  } else if(typ == Typ_CV) {
    *oTab    = cv_tab;
    *indLast = APT_CV_IND;
    *indMax  = APT_CV_SIZ;

  } else if(typ == Typ_SUR) {
    *oTab    = su_tab;
    *indLast = APT_SU_IND;
    *indMax  = APT_SU_SIZ;

  } else if(typ == Typ_SOL) {       // B Body
    *oTab    = so_tab;
    *indLast = APT_SO_IND;
    *indMax  = APT_SO_SIZ;

  } else if(typ == Typ_PLN) {       // Refsys
    *oTab    = pln_tab;
    *indLast = APT_PL_IND;
    *indMax  = APT_PL_SIZ;

  } else if(typ == Typ_Model) {     // ModelRef
    *oTab    = mdr_tab;
    *indLast = APT_MR_IND;
    *indMax  = APT_MR_SIZ;

  } else if(typ == Typ_GTXT) {      // Text
    *oTab    = tx_tab;
    *indLast = APT_TX_IND;
    *indMax  = APT_TX_SIZ;

  } else if(typ == Typ_VAR) {
    *oTab    = vr_tab;
    *indLast = APT_VR_IND;
    *indMax  = APT_VR_SIZ;

  } else if(typ == Typ_Tra) {
    *oTab    = tra_tab;
    *indLast = APT_TR_IND;
    *indMax  = APT_TR_SIZ;

  } else if(typ == Typ_Activ) {
    *oTab    = ac_tab;
    *indLast = APT_AC_IND;
    *indMax  = APT_AC_SIZ;


  } else {
    TX_Error("DB_Query__ E001 %d",typ);
  }

  return 0;

}


//================================================================
  long DB_QueryNxtFree (int typ, int istart) {
//================================================================
/// \code
/// returns next free index
/// search free record starting from istart up to last active index.
/// see also DB_dbo_get_free
/// \endcode

  long    i1;


  // printf("DB_QueryNxtFree %d %d\n",typ,istart);



  if(typ == Typ_VC) {
    // for (i1 = istart; i1>=0; --i1) {
    for (i1 = istart; i1<APT_VC_SIZ; ++i1) {
      if(DB_isFree_VC (&vc_tab[i1])) {
        //TX_Print("ex DB_QueryNxtFree PT %d",i1); 
        return i1;
      }
    }

  } else if(typ == Typ_PT) {
    for (i1 = istart; i1<APT_PT_SIZ; ++i1) {
      if(DB_isFree_PT (&pt_tab[i1])) {
        //TX_Print("ex DB_QueryNxtFree PT %d",i1); 
        return i1;
      }
    }

  } else if(typ == Typ_LN) {
     for (i1 = istart; i1<APT_LN_SIZ; ++i1) {
       if(DB_isFree_LN (&ln_tab[i1])) {
         //TX_Print("ex DB_QueryNxtFree LN %d",i1); 
         return i1;
       }
     }

  } else if(typ == Typ_CI) {
    for (i1 = istart; i1<APT_CI_SIZ; ++i1) {
      if(DB_isFree_CI (&ci_tab[i1])) {
        //TX_Print("ex DB_QueryNxtFree AC %d",i1); 
        return i1;
      }
    }

  } else if(typ == Typ_CV) {
    for (i1 = istart; i1<APT_CV_SIZ; ++i1) {
      if(DB_isFree_CV (&cv_tab[i1])) {
        //TX_Print("ex DB_QueryNxtFree CV %d",i1); 
        return i1;
      }
    }

  } else if(typ == Typ_SUR) {
    for (i1 = istart; i1<APT_SU_SIZ; ++i1) {
      if(DB_isFree_Sur (&su_tab[i1])) {
          // printf("ex DB_QueryNxtFree SUR %ld\n",i1);
        return i1;
      }
    }

  } else if(typ == Typ_SOL) {                      // B Body
    for (i1 = istart; i1<APT_SO_SIZ; ++i1) {
      if(DB_isFree_Sol (&so_tab[i1])) {
        //TX_Print("ex DB_QueryNxtFree CV %d",i1);
        return i1;
      }
    }

  } else if(typ == Typ_PLN) {      // Refsys
    for (i1 = istart; i1<APT_PL_SIZ; ++i1) {
      if(DB_isFree_PLN (&pln_tab[i1])) {
        //TX_Print("ex DB_QueryNxtFree CV %d",i1); 
        return i1;
      }
    }

  } else if(typ == Typ_Model) {      // ModelRef
    for (i1 = istart; i1<APT_MR_SIZ; ++i1) {
      if(DB_isFree_ModRef (&mdr_tab[i1])) {
        // TX_Print("ex DB_QueryNxtFree MR %d",i1);
        return i1;
      }
    }

  } else if((typ == Typ_GTXT)||      // Text
            (typ == Typ_Note))  {
    for (i1 = istart; i1<APT_TX_SIZ; ++i1) {
      if(DB_isFree_GTxt (&tx_tab[i1])) {
        //TX_Print("ex DB_QueryNxtFree CV %d",i1);
        return i1;
      }
    }

  } else if(typ == Typ_VAR) {
    for (i1 = istart; i1<APT_VR_SIZ; ++i1) {
      if(DB_isFree_Var (&vr_tab[i1])) {
        //TX_Print("ex DB_QueryNxtFree CV %d",i1);
        return i1;
      }
    }

  } else if(typ == Typ_Tra) {
    for (i1 = istart; i1<APT_TR_SIZ; ++i1) {
      if(DB_isFree_Tra (&tra_tab[i1])) {
        // printf("ex DB_QueryNxtFree %d typ=%d ista=%d\n",i1,typ,istart);
        return i1;
      }
    }

  } else if(typ == Typ_Activ) {
    for (i1 = istart; i1<APT_AC_SIZ; ++i1) {
      if(DB_isFree_Act (&ac_tab[i1])) {
        // printf("ex DB_QueryNxtFree %d typ=%d ista=%d\n",i1,typ,istart);
        return i1;
      }
    }

  } else if(typ == Typ_Joint) {
    return DB_JNT_IND + 1;    // return next free jointNr

  }


  TX_Print("***** DB_QueryNxtFree Error 1/%d",typ); 

  return 0;

}


//============================================================
  long DB_QueryPoint (Point *pt1) {
//============================================================


  long    i1;
  //Point   pt1;

  //pt1 = GR_sk2uk (xPos, yPos);



  //TX_Print("DB_QueryPoint %f %f\n",pt1->x,pt1->y);
  //TX_Print(" Points %d",apt_pt_siz);

  for (i1 = 1; i1<APT_PT_SIZ; ++i1) {
    // if(pt_tab[i1].x != UT_VAL_MAX) {
    if(DB_isFree_PT (&pt_tab[i1])) continue;
      if(UT3D_comp2pt(&pt_tab[i1], pt1, DB_sel_tol)) {
        //TX_Print ("Gefunden Punkt %d",i1);
        return i1;
      }
    // }
  }



  return -1;

}



//********************************************************************
long DB_QueryLine (Point *pt1) {
//********************************************************************


  long    i1;
  double  d1;
  double  cx_hi,cx_lo,cy_hi,cy_lo;
  double  x1,y1,x2,y2;

  //Point   pt1;
  Point2  pt20, pt21;
  Line2   ln20;

  // get Userkoords > pt20
  //pt1 = GR_sk2uk (xPos, yPos);

  pt20.x = pt1->x;
  pt20.y = pt1->y;


  //TX_Print("DB_QueryLine %f %f\n",pt20.x,pt20.y);
  //TX_Print(" Lines %d",apt_ln_siz);



  cx_hi=pt20.x - DB_sel_tol;
  cx_lo=pt20.x + DB_sel_tol;
  cy_hi=pt20.y - DB_sel_tol;
  cy_lo=pt20.y + DB_sel_tol;


  for (i1 = 0; i1<DYN_LN_SIZ; ++i1) {

    // if(ln_tab[i1].p1.x == UT_VAL_MAX) goto NextInd;
    if(DB_isFree_LN (&ln_tab[i1])) goto NextInd;

    // Zuerst testen, ob der selektierte Punkt in einem Rechteck um die Linie
    // herum ist
    x1=ln_tab[i1].p1.x;
    y1=ln_tab[i1].p1.y;
    x2=ln_tab[i1].p2.x;
    y2=ln_tab[i1].p2.y;
    if ((x1<cx_hi)&&(x2<cx_hi)) goto NextInd;
    if ((x1>cx_lo)&&(x2>cx_lo)) goto NextInd;
    if ((y1<cy_hi)&&(y2<cy_hi)) goto NextInd;
    if ((y1<cy_hi)&&(y2<cy_hi)) goto NextInd;
    if ((y1>cy_lo)&&(y2>cy_lo)) goto NextInd;

    // Der selektierte Punkt befindet sich im Quadrat!


    // den Abstand von der Linie errechnen
    ln20.p1.x = x1;
    ln20.p1.y = y1;
    ln20.p2.x = x2;
    ln20.p2.y = y2;

    // Punkt auf die Line projizieren;
    UT2D_pt_projptln (&pt21, &pt20, &ln20);
    //TX_Print("  pt21= %f %f",pt21.x,pt21.y);

    // Abstand darf max. DB_sel_tol sein.
    d1 = UT2D_len_2pt (&pt20, &pt21);
    //TX_Print(" abst %d %f",i1,d1);
    if(d1 < DB_sel_tol) {
      //TX_Print ("Gefunden Linie %d",i1);
      return i1;
    }
    NextInd:;
  }



  return -1;

}


//********************************************************************
long DB_QueryCirc (Point *pt1) {
//********************************************************************

  int     dreh, i1;
  long    ind;
  double  d1, w1, wa, we;
  //Point   pt1;
  Point2  pt20, pt2a, pt2e, pt2c;

  // get Userkoords > pt20
  //pt1 = GR_sk2uk (xPos, yPos);
  pt20.x = pt1->x;
  pt20.y = pt1->y;


  //TX_Print("DB_QueryCirc %f %f\n",pt1.x,pt1.y);



  // den Abstand zum Mittelpunkt messen; er muss auf DB_sel_tol stimmen.

  for (ind = 0; ind<DYN_CI_SIZ; ++ind) {

    // if(ci_tab[ind].p1.x == UT_VAL_MAX) goto NextInd;
    if(DB_isFree_CI (&ci_tab[ind])) goto NextInd;

    pt2c.x = ci_tab[ind].pc.x;
    pt2c.y = ci_tab[ind].pc.y;

    // Abstand darf max. DB_sel_tol sein.
    d1 = fabs(UT2D_len_2pt (&pt20, &pt2c) - fabs(ci_tab[ind].rad));
    //TX_Print(" abst %d %f",ind,d1);
    if(d1 > DB_sel_tol) goto NextInd;


    // Wenn es ein Kreis ist - gefunden.
    pt2a.x = ci_tab[ind].p1.x;
    pt2a.y = ci_tab[ind].p1.y;
    pt2e.x = ci_tab[ind].p2.x;
    pt2e.y = ci_tab[ind].p2.y;
    if((UT2D_comp2pt(&pt2a, &pt2e, UT_TOL_ln)) == 1) {
      //TX_Print ("Gefunden Kreis %d",ind);
      return ind;
    }


    // Es ist ein Arc. Die Winkel checken.
    dreh = CCW;
    if(ci_tab[ind].rad < 0.0) dreh = CW;
    w1 = UT2D_angr_ptpt (&pt2c, &pt20);
    wa = UT2D_angr_ptpt (&pt2c, &pt2a);
    we = UT2D_angr_ptpt (&pt2c, &pt2e);

    //TX_Print("  vor: w1=%f, wa=%f, we=%f, dreh=%d\n",w1,wa,we,dreh);

    // ist w1 zwischen wa, we ?
    i1 = UT2D_compPtOnAc (w1, wa, we, dreh);
    if(i1 == YES) {
      return ind;
    }

    NextInd:;
  }



  return -1;

}

//********************************************************************
long DB_QueryCurv (Point *pt1) {
//********************************************************************
/// befindet sich Punkt auf Curv ???
// Das Display einer Curv besteht aus einem Polygon.


  long    i1, i2;
  double  d1;
  double  cx_hi,cx_lo,cy_hi,cy_lo;
  double  x1,y1,x2,y2;

  //Point   pt1;
  Point2  pt20, pt21;
  Line2   ln20;




  // get Userkoords > pt20
  //pt1 = GR_sk2uk (xPos, yPos);
  pt20.x = pt1->x;
  pt20.y = pt1->y;


  TX_Print("DB_QueryCurv %f %f\n",pt20.x,pt20.y);


/*
  cx_hi=pt20.x - DB_sel_tol;
  cx_lo=pt20.x + DB_sel_tol;
  cy_hi=pt20.y - DB_sel_tol;
  cy_lo=pt20.y + DB_sel_tol;


  for (i1 = 0; i1<apt_cv_siz; ++i1) {

    if(cv_tab[i1].typ == Typ_Error) goto NextInd;

    //TX_Print(" nxt cv #%d von %d bis %d",i1, cv_tab[i1].ind1, cv_tab[i1].ind2);

    for(i2 = cv_tab[i1].ID1; i2 < cv_tab[i1].ID2; ++i2) {



      // Zuerst testen, ob der selektierte Punkt in einem Rechteck um die Linie
      // herum ist
      x1=pt_tab[i2].x;
      y1=pt_tab[i2].y;
      x2=pt_tab[i2+1].x;
      y2=pt_tab[i2+1].y;

      //TX_Print(" ck nxt %d / %d",i2, i2+1);
      //TX_Print(" ck nxt %f,%f  -  %f,%f",x1,y1,x2,y2);


      if ((x1<cx_hi)&&(x2<cx_hi)) goto NextSeg;
      if ((x1>cx_lo)&&(x2>cx_lo)) goto NextSeg;
      if ((y1<cy_hi)&&(y2<cy_hi)) goto NextSeg;
      if ((y1<cy_hi)&&(y2<cy_hi)) goto NextSeg;
      if ((y1>cy_lo)&&(y2>cy_lo)) goto NextSeg;

      // Der selektierte Punkt befindet sich im Quadrat!


      // den Abstand von der Linie errechnen
      ln20.p1.x = x1;
      ln20.p1.y = y1;
      ln20.p2.x = x2;
      ln20.p2.y = y2;

      // Punkt auf die Line projizieren;
      UT2D_pt_projptln (&pt21, &pt20, &ln20);
      //TX_Print("  pt21= %f %f",pt21.x,pt21.y);

      // Abstand darf max. DB_sel_tol sein.
      d1 = UT2D_len_2pt (&pt20, &pt21);
      //TX_Print(" abst %d %f",i1,d1);
      if(d1 < DB_sel_tol) {
        //TX_Print ("Gefunden Curve %d",i1);
        return i1;
      }

      NextSeg:;
     
    }

    NextInd:;
  }
*/


  return -1;

}


//================================================================
  int DB_Filter__ (ObjDB *grp, int *grpNr, ObjGX *flt, int fltNr) {
//================================================================
/// \code
/// DB_Filter__        get list of filtered DB-objects
/// Input:
///   flt     FilterRecords. see Filtering_Examples
///   fltNr - number of records in flt
///   grpNr - size of gr1
/// Output:
///   grpNr - nr of found objects.
/// Returncodes:
///   -1      not yet implemented
///
/// Filtering_Examples:
/// fltNr=1; flt-Rec1: typ=Typ_SURPTAB.
///           find all surfaces of type Typ_SURPTAB (A=PTAB ..)
///           returns grp.typ=Typ_SUR; grp.ind=DB-ind.
/// \endcode


  int    i1, i2, iflt, ii, ityp, grpSiz;
  ObjGX  *fltAct;



  printf("DB_Filter__ \n");

  grpSiz = *grpNr;
  *grpNr = 0;
  iflt = -1;

  L_nxt_flt:
  ++iflt;
  if(iflt >= fltNr) goto L_done;
  fltAct = &flt[iflt];
  ityp = AP_typDB_typ (fltAct->typ);
  ii = fltAct->typ;
    // printf(" flt[%d] typ=%d ii=%d\n",iflt,fltAct->typ,ii);


 
  // basetyp=ityp; filter surfTyp ii
  if(ityp == Typ_SUR) {
    for(i1 = 1; i1<APT_SU_SIZ; ++i1) {
      // if(su_tab[i1].typ == Typ_Error) continue;
      if(DB_isFree_Sur (&su_tab[i1])) continue;
        i2 = UTO_ck_surfTyp (&su_tab[i1]);
          // printf(" surfTyp[%d]=%d\n",i1,i2);
        if(i2 != ii) continue;
        // add this surf
        if(*grpNr < grpSiz) {
          grp[*grpNr].typ = ityp;
          grp[*grpNr].dbInd = i1;
          *grpNr += 1;
        } else {
          TX_Print("DB_Filter__ I001");
        }
    }



  } else return -1;

  goto L_nxt_flt;


  //----------------------------------------------------------------
  L_done:
    // for(i1=0; i1 < *grpNr; ++i1)
      // printf(" grp[%d]=%d %d\n",i1,grp[i1].typ,grp[i1].dbInd);

  return 0;

}


//=============================================================
  int DB_QueryActiv (Activity **act, int typ, long ind) {
//=============================================================
/// \code
/// query, if a interactivity is connected to obj
/// RectCod: -1=no, else DB-ind of activity.
/// \endcode

  int   i1;


  // printf("DB_QueryActiv %d %d\n",typ,ind);
  // DB_dump_Activ ();

  typ = AP_typDB_typ (typ);
  // printf("DB_QueryActiv %d %d\n",typ,ind);

  for(i1=0; i1<APT_AC_SIZ; ++i1) {
    // if(ac_tab[i1].typ == Typ_Error) continue;
    // printf(" _QueryActiv[%d] %d %d\n",i1,ac_tab[i1].typ,ac_tab[i1].ind);
    if(ac_tab[i1].typ != typ) continue;
    if(ac_tab[i1].ind != ind) continue;
    *act = &ac_tab[i1];
    return i1;
  }

 
  return -1;

}


/*
//=====================================================================
  int DB_QueryDynPoint (Point *pt_out, Point *pt1) {
//=========================
// Select Point; alle dyn. Elemente (Anfangs-, Endpunkte) checken.



  long    i1;
  //Point   pt1;

  //pt1 = GR_sk2uk (xPos, yPos);


  //TX_Print("DB_QueryDynPoint %f %f\n",pt1->x,pt1->y);
  //TX_Print("  DynLinesInd: %d - %d",apt_ln_siz,dyn_ln_ind);
  //TX_Print(" DB_sel_tol = %f",DB_sel_tol);

  // Check Lines
  for (i1 = apt_ln_siz+1; i1<=dyn_ln_ind; ++i1) {
    //TX_Print("Ln %d %f,%f / %f,%f",i1,ln_tab[i1].p1.x,ln_tab[i1].p1.y,
    //                                  ln_tab[i1].p2.x,ln_tab[i1].p2.y);
    if(UT3D_comp2pt(&ln_tab[i1].p1, pt1, DB_sel_tol)) {
      TX_Print ("Gefunden dyn LnAnf.Punkt %d",i1);
      *pt_out = ln_tab[i1].p1;
      return 0;
    } else if(UT3D_comp2pt(&ln_tab[i1].p2, pt1, DB_sel_tol)) {
      TX_Print ("Gefunden dyn LnEnd.Punkt %d",i1);
      *pt_out = ln_tab[i1].p2;
      return 0;
    }
  }


  // check Circ's
  for (i1 = apt_ci_siz+1; i1<=dyn_ci_ind; ++i1) {
    if(UT3D_comp2pt(&ci_tab[i1].p1, pt1, DB_sel_tol)) {
      TX_Print ("Gefunden dyn CiAnf.Punkt %d",i1);
      *pt_out = ci_tab[i1].p1;
      return 0;
    } else if(UT3D_comp2pt(&ci_tab[i1].p2, pt1, DB_sel_tol)) {
      TX_Print ("Gefunden dyn CiEnd.Punkt %d",i1);
      *pt_out = ci_tab[i1].p2;
      return 0;
    }
  }


  return -1;

}
*/


//================================================================
  void DB_Delete (int typ, long ind) {
//================================================================
/// \code
/// delete DB-object
/// \endcode


  // long i1;


  // printf(" XXXXXXXXXXX DB_Delete: %d %ld\n",typ,ind);


  switch (typ) {

    case Typ_VAR:
      if(ind < APT_VR_SIZ) DB_setFree_Var (ind);
      break;

    case Typ_VC:
      if(ind < APT_VC_SIZ) DB_setFree_VC (ind);
      if(ind == APT_VC_IND) {
        // free all unused objs
        while(DB_isFree_VC (&vc_tab[APT_VC_IND])) {
          if(APT_VC_IND <= 0) break;
          --APT_VC_IND;
        }
      }
      break;

    case Typ_PT:
      if(ind < APT_PT_SIZ) DB_setFree_PT (ind);
      break;

    case Typ_LN:
      if(ind < APT_LN_SIZ) DB_setFree_LN (ind);
      break;

    case Typ_CI:
      if(ind < APT_CI_SIZ) DB_setFree_CI (ind);
      break;

    case Typ_CV:
      if(ind < APT_CV_SIZ) DB_setFree_CV (ind);
      break;

    case Typ_GTXT:
      if(ind < APT_TX_SIZ) DB_setFree_TX (ind);
      break;

    case Typ_PLN:
      if(ind < APT_PL_SIZ) DB_setFree_PLN (ind);
      break;

    case Typ_SUR:
      if(ind < APT_SU_SIZ) DB_setFree_SU (ind);
      break;

    case Typ_SOL:
      if(ind < APT_SO_SIZ) DB_setFree_SO (ind);
      break;

    case Typ_Tra:
      if(ind < APT_TR_SIZ) DB_setFree_TR (ind);
      if(ind == APT_TR_IND) {
        while(DB_isFree_Tra (&tra_tab[APT_TR_IND])) {
          if(APT_TR_IND <= 0) break;
          --APT_TR_IND;
        }
      }
      break;

    case Typ_Model:
      if(ind < APT_MR_SIZ) DB_setFree_MR (ind);
      if(ind == APT_MR_IND) {
        while(DB_isFree_ModRef (&mdr_tab[APT_MR_IND])) {
          if(APT_MR_IND <= 0) break;
          --APT_MR_IND;
        }
      }
      break;

    case Typ_Activ:
      if(ind < APT_AC_SIZ) DB_setFree_Act (ind);
      break;
  }


  return;

}


//=============================================================
  int DB_store_ox (ObjGX *oxo, ObjGX *ox1, long *ind) {
//=============================================================
/// \code
/// DO NOT USE; use DB_store_obj
/// save obj --> DB
/// Input:
///   ox1    eine (ObjGX*)-Struktur
/// Output:
///   oxo    su_tab[Ind]
///   ind    wenn on Input < 0
/// RetCod: 
///   0 = OK
///  -1 = Error
/// \endcode


// see UTRA_app_oTab

  int    irc, i1, iTyp, iForm, iSiz, sTyp, sForm, sSiz;
  long   l1;
  void   *cPos1, *cPos2, *cPos3;
  char   *pi;
  // ObjGX  *oxo;


  // printf("DB_store_ox ind=%ld typ=%d form=%d\n",*ind,ox1->typ,ox1->form);
  // DEB_dump_ox_s_ (ox1, "DB_store_ox-in\n");
  // DEB_dump_ox_0 (ox1, "DB_store_ox-in\n");
  
  irc = 0;
  iForm = ox1->form;


  // copy ParentObj
  *oxo = *ox1;    
  cPos1 = DB_cPos ();


  // einfache struct direkt speichern ..
  if(iForm != Typ_ObjGX) {
    iSiz = ox1->siz;
    // save struct ox1->data --> DB; DB-Pos = cPos3;
    irc = DB_store_obj (&cPos3, iForm, iForm, ox1->data, iSiz, ind);
    // if(irc < 0) return irc;
    // change to Typ_Index; wenn jedoch ox1->siz > 1 (dzt nur Typ_PT), dann
    // wird data-pos korrigiert.
    if((ox1->siz > 1)         ||
       (iForm == Typ_CVTRM)   ||                      // trim-ccv-data
       (iForm == Typ_SURHAT))    oxo->data = cPos3;
    // ((ObjGX*)oxo)->form = Typ_Index;
    // (int)((ObjGX*)oxo)->data = l1;
    // goto L_fertig;
    // oxo->data   = cPos1;     // new address of oGX-Block

    return irc;
  }




  //----------------------------------------------------------------
  // save ObjGX
  iSiz = ox1->siz;
  if(ox1->data) pi = (char*)ox1->data;
  else          pi = (char*)&ox1[1];      // data followig this record 

  // den gesamten oGX Block in DB speichern
  l1 = sizeof(ObjGX) * iSiz;
  cPos2 = DB_cSav(l1, pi);

  // printf(" store_obj siz=%d typ=%d form=%d\n",ox1->siz,ox1->typ,ox1->form);

  pi = cPos1; // outputfeld

  // Daten der einzelnen oGX-Records speichern
  for(i1=0; i1<iSiz; ++i1) {
    sTyp  = ((ObjGX*)pi)->typ;
    sForm = ((ObjGX*)pi)->form;
    sSiz  = ((ObjGX*)pi)->siz;
    // printf(" o[%d] sTyp=%d sForm=%d sSiz=%d\n",i1,sTyp,sForm,sSiz);

    if(sTyp  == Typ_Typ)      goto L_GX_nxt;   // hat keine data
    if(sTyp  == Typ_modUndef) goto L_GX_nxt;   // hat keine data
    if(sForm == Typ_modUndef) goto L_GX_nxt;   // hat keine data

    if(sForm == Typ_ObjGX) { // recurse
      // printf("******** DB_store_ox I001\n");
      l1 = -1;
      DB_store_ox ((ObjGX*)pi, (ObjGX*)pi, &l1);


    } else {
      if((sForm == Typ_Index)&&(sSiz < 2)) goto L_GX_nxt;   // Index ist data
      l1 = -1;
      // siz=1: store dynam.Obj; else store as block
      irc = DB_store_obj (&cPos3, sForm, sForm, ((ObjGX*)pi)->data, sSiz, &l1);
      if(irc < 0) goto L_fertig;
      // siz=1: change to Typ_Index 
      if(sSiz < 2) {
        ((ObjGX*)pi)->form = Typ_Index;
        ((ObjGX*)pi)->data = PTR_LONG(l1);
      } else {
        ((ObjGX*)pi)->data = cPos3;
      }
    }

    L_GX_nxt:
    pi += sizeof(ObjGX);
  }


  // das ParentObj speichern ..
  L_fertig:
  // hier sollte eigenlich im InputObj ox1 gespeichert werden;
  oxo->data   = cPos1;     // new address of oGX-Block


    // TESTBLOCK
    // printf(" ex DB_store_ox irc=%d typ=%d form=%d ind=%ld\n",irc,
              // oxo->typ,oxo->form,*ind);
    // printf(" oxo-posi=%p\n",cPos1);
    // DEB_dump_ox_s_ (oxo, "DB_store_ox-out\n");
    // DEB_dump_ox_0 (oxo, "DB_store_ox-out\n");
    // END TESTBLOCK


  return irc;

}


//======================================================================
  int DB_store_obj (void **oo, int typ, int form, void *os1, int iNr,
                    long *ind) {
//======================================================================
/// \code
/// save struct os1 in DB with form=form;
/// can also save the parent-ox for S,A,B (see ind)
/// Input
///   typ   type of os1; only for form=Typ_ObjGX (SAB)
///   form  type of struct os1
///   os1   struct to save; datastruct, no parent-ox for curves.
///   iNr   nr of structs
///   ind   if(ind == -1) create dynam. obj; give back dynam. index
///         if(ind == 0)  DO NOT STORE parent-ox for S,A,B.
///         else          store data and parent-ox
/// Output:
///   oo    position of os1 (in DB_CSEG)
///   ind   dynam. Index (only if (ind == -1))
/// IRC  0 = OK
/// IRC -1 = Error
/// \endcode

// plg: form=Typ_CVPOL
// must not store the parentobjetc (ObjGX for curve eg !)
//   is done from DB_StoreCurv < DB_store_ox < DB_store_obj
// see also DB_store_ox (to be replaced !)
// see also UTO_copy_tab


  int      i1; //, sSiz;
  long     l1;
  void     *cPos1;
  ObjGX    *ox1, *oTab;


  // printf("--------------------- \n");
  // printf("DB_store_obj typ=%d form=%d ind=%ld iNr=%d\n",typ,form,*ind,iNr);
  // DEB_dump_obj__ (form, os1, "DB_store_obj-"); //not for ox !



  switch (form) {


    //================================================================
    case Typ_VAR:
    case Typ_Val:
    case Typ_XVal:
    case Typ_YVal:
    case Typ_ZVal:
    case Typ_Angle:
      if(iNr > 1) goto L_E_INR;
      *ind = DB_StoreVar (*ind, *((double*)os1));
      break;


    //================================================================
    case Typ_PT:
      // einen einzelnen Punkt als dynam. Punkt ablegen;
      if(iNr < 2) {
        *ind = DB_StorePoint (*ind, os1);

      // eine PunkteTabelle einfach kopieren; irc=0 retourgeben.
      } else {
        *oo = DB_cPos ();
        DB_cSav(sizeof(Point) * iNr, os1);
      }
      break;


    //================================================================
    case Typ_LN:
      if(iNr > 1) goto L_E_INR;
      *ind = DB_StoreLine (*ind, os1);
      break;


    //================================================================
    case Typ_VC:
      if(iNr > 1) goto L_E_INR;
      *ind = DB_StoreVector (*ind, os1);
      break;


    //================================================================
    case Typ_CI:
      *ind = DB_StoreCirc (*ind, os1);
      break;


    //================================================================
    //================================================================
    case Typ_CVCLOT:                        // DB_StoreCurv
      if(iNr > 1) goto L_E_INR;
      *ind = DB_StoreClot (*ind, os1);
      break;


    //================================================================
    case Typ_CVBSP:                        // DB_StoreCurv
      if(iNr > 1) goto L_E_INR;
      *ind = DB_StoreCvBsp (*ind, os1);
      break;


    //================================================================
    case Typ_CVRBSP:                        // DB_StoreCurv
      if(iNr > 1) goto L_E_INR;
      *ind = DB_StoreCvRBsp (*ind, os1);
      break;


    //================================================================
    case Typ_CVPOL:                       // DB_StoreCurv
      if(iNr > 1) goto L_E_INR;
      // store date & parent-ox
      *ind = DB_StoreCvPlg (*ind, os1, 0);
      break;


    //================================================================
    case Typ_CVELL:                       // DB_StoreCurv
      if(iNr > 1) goto L_E_INR;
      *ind = DB_StoreCvEll (*ind, os1);
      break;


    //================================================================
    case Typ_CVTRM:                       // DB_StoreCurv
      *ind = DB_StoreCvCCV (*ind, os1, iNr);
      break;


    //================================================================
    //================================================================
    case Typ_PLN:
      if(iNr > 1) goto L_E_INR;
      *ind = DB_StoreRef (*ind, os1);
      break;


    //================================================================
    case Typ_SURRV:
      if(iNr > 1) goto L_E_INR;
      *ind = DB_StoreSurRV (*ind, os1);
      break;


    //================================================================
    case Typ_SURBSP:
      if(iNr > 1) goto L_E_INR;
      *ind = DB_StoreSurBsp (*ind, os1);
      break;


    //================================================================
    case Typ_SURRBSP:
      if(iNr > 1) goto L_E_INR;
      *ind = DB_StoreSurRBsp (*ind, os1);
      break;


    //================================================================
    case Typ_SURHAT:
      if(iNr > 1) goto L_E_INR;
//TODO      // save parentrecord
      *oo = DB_cPos ();
      DB_cSav(sizeof(SurHat), os1);
      break;


    //================================================================
    case Typ_SURSTRIP:
      if(iNr > 1) goto L_E_INR;
      *oo = DB_cPos ();
      *ind = DB_StoreSurStripe (*ind, os1);
      break;



    //================================================================
    //================================================================
    case Typ_TOR:
      if(iNr > 1) goto L_E_INR;
      // save parentrecord (data = DB_cPos)
      l1 = DB_Store_hdr_so (&ox1, typ, form, iNr, *ind);
      if(l1 < 0L) return -1;
      DB_cSav(sizeof(Torus), os1);
      break;


    //================================================================
    case Typ_SPH:
      if(iNr > 1) goto L_E_INR;
      // save parentrecord (data = DB_cPos)
      l1 = DB_Store_hdr_so (&ox1, typ, form, iNr, *ind);
      if(l1 < 0L) return -1;
      DB_cSav(sizeof(Sphere), os1);
      break;


    //================================================================
    case Typ_CON:
      if(iNr > 1) goto L_E_INR;
      // save parentrecord (data = DB_cPos)
      l1 = DB_Store_hdr_so (&ox1, typ, form, iNr, *ind);
      if(l1 < 0L) return -1;
      DB_cSav(sizeof(Conus), os1);
      break;

/*
    //================================================================
    case Typ_BREP:
      if(iNr > 1) goto L_E_INR;
      *oo = DB_cPos ();
      DB_cSav(sizeof(Torus), os1);
      break;
*/

    //================================================================
    //================================================================
    case Typ_Index:
      if(iNr < 2) return 0; // nur 1 Index: data = Adresse; nix tun.
      *oo = DB_cPos ();
      DB_cSav(sizeof(long) * iNr, os1);
      // os1 = cPos1;
      break;


    //================================================================
    case Typ_ObjGX:
      *oo = DB_cPos ();
      oTab = *oo;
      // for solid-PRISM must save the parent-record
      if(typ == Typ_SOL) {
        // save parentrecord (data = DB_cPos)
        l1 = DB_Store_hdr_so (&ox1, typ, form, iNr, *ind);
        if(l1 < 0L) return -1;
      }
      // save prim.Objects
      l1 = sizeof(ObjGX) * iNr;
      DB_cSav (l1, os1);                // save all primary OGX-structs
      // save data-Objects; loop tru (new) ox-tab
      for(i1=0; i1<iNr; ++i1) {
        // ox1 = &((ObjGX*)os1)[i1];
        ox1 = &oTab[i1];
          // DEB_dump_obj__(Typ_ObjGX, ox1, "oTab[%d]", i1);
// Typ_Ptr Unused ??
        if(ox1->typ == Typ_Ptr) {
          cPos1 = DB_cPos ();
          // compute size
          if(ox1->form == Typ_Int1) {
            l1 = ox1->siz;
          } else if(ox1->form == Typ_Int4) {
            l1 = ox1->siz * 4;
          } else {
            l1 = UTO_siz_stru (ox1->form);
            if(l1 < 1) {TX_Error("DB_store_obj E002"); return -1;}
          }
          // copy binary-data-record
          DB_cSav (l1, ox1->data);
          // update data-position
          ox1->data = cPos1;
          continue;
        }
//-------
        // here is info in data itself:
        if(ox1->form == Typ_modUndef) continue;
        if(ox1->form == Typ_Index) continue;
        if(ox1->form == Typ_Int4) continue; // typ=Typ_Typ
        // save Point or Line in ox1->data -> DB; RECURSION !
        l1 = -1;
        DB_store_obj (&cPos1, ox1->typ, ox1->form, ox1->data, ox1->siz, &l1);
          // printf(" new dyn.Ind=%d (form=%d)\n",l1,ox1->form);
        // save dynam. Index in its parent-record
        OGX_SET_INDEX (ox1, ox1->typ, l1);
          // DEB_dump_obj__(Typ_ObjGX, ox1, "new oTab[%d]\n",i1);
      }
      break;



    //================================================================
    case Typ_modUndef:   // nothing to store ..
      break;



    //================================================================
    default:
      TX_Error("DB_store_obj form %d unsupp.",form);
      return -1;
  }


    //---- Anf Testausg:
    // ox1 = DB_GetCurv (*ind);
    // DEB_dump_ox_s_ (ox1, "DB_store_obj-out\n");
    // DEB_dump_ox_0 (ox1, "DB_store_obj-out\n");
    // printf("----------- ex DB_store_obj typ=%d ind=%ld\n",typ,*ind);
    // exit(0);
    //---- End Testausg:



  // (ind == 0) ist Fehler !!
  if(ind == 0) return -1;
  else return 0;



  L_E_INR:
    TX_Error("DB_store_obj E_INR %d %d",form,iNr);
    return -1;

}


//================================================================
  int DB_get_ObjNr (int typ) {
//================================================================
// DB_get_ObjNr       get nr of defined objects of given type

  int   i1, i2;


  i2 = 0;

  switch (typ) {

    case Typ_VAR:
       for(i1=1; i1<=25; ++i1) {
         if(DB_isFree_Var (&vr_tab[i1])) continue;
         ++i2;
       }
       if(APT_VR_IND > 25) i2 += APT_VR_IND - 25;
       break;


     case Typ_VC:
       for(i1=1; i1<=25; ++i1) {
         if(DB_isFree_VC (&vc_tab[i1])) continue;
         ++i2;
       }
       if(APT_VC_IND > 25) i2 += APT_VC_IND - 25;
       break;


     case Typ_PT:
       for(i1=1; i1<=25; ++i1) {
         if(DB_isFree_PT (&pt_tab[i1])) continue;
         ++i2;
       }
       if(APT_PT_IND > 25) i2 += APT_PT_IND - 25;
       break;


     case Typ_LN:
       for(i1=1; i1<=25; ++i1) {
         if(DB_isFree_LN (&ln_tab[i1])) continue;
         ++i2;
       }
       if(APT_LN_IND > 25) i2 += APT_LN_IND - 25;
       break;


     case Typ_CI:
       for(i1=1; i1<=25; ++i1) {
         if(DB_isFree_CI (&ci_tab[i1])) continue;
         ++i2;
       }
       if(APT_CI_IND > 25) i2 += APT_CI_IND - 25;
       break;


     case Typ_CV:
       for(i1=1; i1<=25; ++i1) {
         if(DB_isFree_CV (&cv_tab[i1])) continue;
         ++i2;
       }
       if(APT_CV_IND > 25) i2 += APT_CV_IND - 25;
       break;


     case Typ_PLN:
       for(i1=1; i1<=25; ++i1) {
         if(DB_isFree_PLN (&pln_tab[i1])) continue;
         ++i2;
       }
       if(APT_PL_IND > 25) i2 += APT_PL_IND - 25;
       break;


     case Typ_SUR:
       for(i1=1; i1<25; ++i1) {
         if(DB_isFree_Sur (&su_tab[i1])) continue;
           // printf(" used sur %d\n",i1);
         ++i2;
       }
       if(APT_SU_IND > 25) i2 += APT_SU_IND - 25;
       break;


     case Typ_SOL:
       for(i1=1; i1<=25; ++i1) {
         if(DB_isFree_Sol (&so_tab[i1])) continue;
         ++i2;
       }
       if(APT_SO_IND > 25) i2 += APT_SO_IND - 25;
       break;


     case Typ_Note:
     case Typ_GTXT:
       for(i1=1; i1<=25; ++i1) {
         if(DB_isFree_GTxt (&tx_tab[i1])) continue;
         ++i2;
       }
       if(APT_TX_IND > 25) i2 += APT_TX_IND - 25;
       break;


     case Typ_Model:
       for(i1=1; i1<=25; ++i1) {
         if(DB_isFree_ModRef (&mdr_tab[i1])) continue;
         ++i2;
       }
         // printf(" APT_MR_IND=%ld i2=%d\n",APT_MR_IND,i2);
       if(APT_MR_IND > 25) i2 += APT_MR_IND - 25;
       break;


     case Typ_Activ:
       for(i1=1; i1<=25; ++i1) {
         if(DB_isFree_Act (&ac_tab[i1])) continue;
         ++i2;
       }
         // printf(" APT_MR_IND=%ld i2=%d\n",APT_MR_IND,i2);
       if(APT_AC_IND > 25) i2 += APT_AC_IND - 25;
       break;


     case Typ_Joint:
       i2 = DB_JNT_IND;
       break;




     default:
       printf("ERROR DB_get_ObjNr %d\n",typ);

   }


  return i2;

}



//================================================================
  int DB_VC_isFree (long dbi) {
//================================================================
/// \code
/// test if DB-vector is in use or free
/// dbi < 0 - dynamic vector
/// RetCod: 
///     0     obj is in use
///     1     obj is free (unused)
/// \endcode

  // printf("DB_VC_isFree %ld %ld\n",dbi,APT_VC_IND);

  if(dbi < 0) {
    dbi = -dbi;
    return DB_isFree_VC (&vc_dyn[dbi]);
  }



  return DB_isFree_VC (&vc_tab[dbi]);

}


//================================================================
  int DB_PLN_isFree (long dbi) {
//================================================================
/// \code
/// test if DB-plane is in use or free
/// dbi < 0 - dynamic plane
/// RetCod: 
///     0     obj is in use
///     1     obj is free (unused)
/// \endcode


  // printf("DB_PLN_isFree %ld %ld\n",dbi,APT_PL_IND);

  if(dbi < 0) {
    dbi = -dbi;
    return DB_isFree_PLN (&pln_dyn[dbi]);
  }


  return DB_isFree_PLN (&pln_tab[dbi]);

}


//========================== EOF ========================================
