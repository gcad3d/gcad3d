/*
 * Copyright (C) 2012 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
-----------------------------------------------------
*//*!
\file ../gr/ut_gr.h
\brief includes for GR_*    ../gr/ut_gr.c

Prerequisites:
#include "../ut/ut_memTab.h"           // MemTab_..
#include "../ut/ut_itmsh.h"            // MSHIG_EDGLN_.. typedef_MemTab.. Fac3
#include "../ut/ut_memTab.h"           // MemTab_..

-----------------------------------------------------
Modifications:
2020-04-10 GR_OTYP_* zu. GR_TMP_* u. RF.
2001-06-06 die GR_ funktionen zu.

-----------------------------------------------------
See INF_GR_RECORDS

GR_temp_*       display temporary;
                for using fixed temp-index set "DL_temp_ind = <fixed-index>;"
                                             or "DL_temp_ind_set (<fixed-index>);"
   att          for points see INF_COL_PT
                for curves see INF_COL_CV
                for surfaces tDyn|temp GR_TMP_HILI|GR_TMP_DIM|GR_TMP_DEF
                             perm 0=defaultColor or struct ColRGB

==============================================================*/


// object-persistence   see INF_GR_RECORDS
#define OPERS_NONE  0  // do not open and not close objectList
#define OPERS_CLOSE 1  // close objectList
#define OPERS_TEMP  2  // open objectList for Temporary-Object
#define OPERS_TDYN  4  // open objectList for Temporary-Dynamic-Object
#define OPERS_PERM  8  // open objectList for Permanent-Object


#define GR_OTYP_NONE 0
#define GR_OTYP_PERM 1
#define GR_OTYP_TDYN 2
#define GR_OTYP_TEMP 3

// GL-dispList-index of the temporary outputObject of WC_Work1
// GL-dispList-index of the CAD-inputObjects are -1 to -7; see IE_GET_INP_DLI
// Last temp. GL-dispList-index is (DL_base_font1 - 1)
#define GR_TMP_I0        8        // primary temporary obj
#define GR_TMP_IDO       9        // GL-recordNr for obj-names
#define GR_TMP_IDIR     10        // direction with arrow
#define GR_TMP_IPOS     11        // vertexposition - small circle
#define GR_TMP_IMEN     12        // preview activ option-menu-entry

#define GR_TMP_DEF       1        // default; normal; use with DL_att_temp
#define GR_TMP_HILI      2        // hilite;  use with DL_att_temp
#define GR_TMP_DIM       4        // dimmed;  use with DL_att_temp



//----------------------------------------------------------------
// this file is without extern used only in ../gr/ut_gr.c
extern float LN_WIDTH_ADJUST;     // adjust linewidth to total screensize (multiply)
extern float LN_WIDTH_DEF;        // default linewidth (1.)
extern float LN_WIDTH_FAT;        // linewidth (4.)
extern char  MODE_DISP_ONAM_NT;   // 0=do not displayobjNames for notes, 1=display


//----------------------------------------------------------------
// prototypes
int GR_set_obj (int opers, long dbi, int form, void *obj, int att, int mode);
int GR_set_ocv (int opers, long dbi, int form, void *obj, int att);
int GR_set_ccv (int opers, CurvCCV *cva, int cvNr, long dbi, int att);
int GR_set_pcv (int opers, Point *pta, int ptNr, int att);
int GR_set_symV3 (int opers, int typ, int att,
                     Point *p1, Vector *v1, double scale);
int GR_set_dimen (int opers, Dimen *dim1, long dbi, int att);
int GR_set_dim3 (int opers, Dim3 *dim1, long dbi, int att);
int GR_set_txt__ (int opers, AText *tx1, long dbi, int att);
int GR_set_txtA (int opers, Point *pt1, char *txtA, int att);
int GR_set_txtG (int opers, GText *txG, long dbi, int att);



void GR_temp_pt (Point *pt1, int att);
//   att      ATT_PT_BLACK .. see INF_COL_PT
#define GR_temp_pt(pt1,att)\
 GR_set_obj(OPERS_TEMP+OPERS_CLOSE,0L,Typ_PT,pt1,att,0);

void GR_tDyn_pt (Point *pt1, int att);
//   att      ATT_PT_BLACK .. see INF_COL_PT
#define GR_tDyn_pt(pt1,att)\
 GR_set_obj(OPERS_TDYN+OPERS_CLOSE,0L,Typ_PT,pt1,att,0);

void GR_perm_pt (long dbi, Point *pt1, int att);
#define GR_perm_pt(dbi,pt1,att)\
 GR_set_obj(OPERS_PERM+OPERS_CLOSE,dbi,Typ_PT,pt1,att,0);



void GR_temp_ln (Line *ln1, int att);
#define GR_temp_ln(ln1,att)\
 GR_set_obj(OPERS_TEMP+OPERS_CLOSE,0L,Typ_LN,ln1,att,0);

void GR_tDyn_ln (Line *ln1, int att);
#define GR_tDyn_ln(ln1,att)\
 GR_set_obj(OPERS_TDYN+OPERS_CLOSE,0L,Typ_LN,ln1,att,0);

void GR_perm_ln (long dbi, Line *ln1, int att);
#define GR_perm_ln(dbi,ln1,att)\
 GR_set_obj(OPERS_PERM+OPERS_CLOSE,dbi,Typ_LN,ln1,att,0);



void GR_temp_pln (Plane *pl1, int att, int mode);
#define GR_temp_pln(pl1,att,mode)\
 GR_set_obj(OPERS_TEMP+OPERS_CLOSE,0L,Typ_PLN,pl1,att,mode);

void GR_tDyn_pln (Plane *pl1, int att, int mode);
#define GR_tDyn_pln(pl1,att,mode)\
 GR_set_obj(OPERS_TDYN+OPERS_CLOSE,0L,Typ_PLN,pl1,att,mode);

void GR_perm_pln (long dbi, Plane *pl1, int att, int mode);
#define GR_perm_pln(dbi,pl1,att,mode)\
 GR_set_obj(OPERS_PERM+OPERS_CLOSE,dbi,Typ_PLN,pl1,att,mode);


// display polygon temp.
void GR_temp_pcv__ (Point *pta, int ptnr, int att);
#define GR_temp_pcv__(pta,ptnr,att)\
 GR_set_pcv(OPERS_TEMP+OPERS_CLOSE,pta,ptnr,att);

// display polygon temp-dyn.
//   att      see INF_COL_CV
void GR_tDyn_pcv (Point *pta, int ptnr, int att);
#define GR_tDyn_pcv(pta,ptnr,att)\
 GR_set_pcv(OPERS_TDYN+OPERS_CLOSE,pta,ptnr,att);



void GR_temp_ocv (int form, void *obj, long dbi, int att);
//  display all types of curves
//  dbi used for get PRCV-polygon, else set to 0
//  att     see INF_COL_CV
#define GR_temp_ocv(form,obj,dbi,att)\
 GR_set_ocv(OPERS_TEMP+OPERS_CLOSE,dbi,form,obj,att);

void GR_tDyn_ocv (int form, void *obj, long dbi, int att);
//  display all types of curves
//  dbi used for get PRCV-polygon, else set to 0
//  att     see INF_COL_CV
#define GR_tDyn_ocv(form,obj,dbi,att)\
 GR_set_ocv(OPERS_TDYN+OPERS_CLOSE,dbi,form,obj,att);

void GR_perm_ocv (int form, void *obj, long dbi, int att);
//  display all types of curves
//  dbi used for get PRCV-polygon, else set to 0
//  att     see INF_COL_CV
#define GR_perm_ocv(form,obj,dbi,att)\
 GR_set_ocv(OPERS_PERM+OPERS_CLOSE,dbi,form,obj,att);



void GR_temp_txtA (Point *pt1, char *txtA, int att);
//   txtA     textstring; null-terminated; maxLen 256
//   att      see INF_COL_SYMB
#define GR_temp_txtA(pt1,txtA,att)\
 GR_set_txtA(OPERS_TEMP+OPERS_CLOSE,pt1,txtA,att);

void GR_tDyn_txtA (Point *pt1, char *txtA, int att);
//   txtA     textstring; null-terminated; maxLen 256
//   att      see INF_COL_SYMB
#define GR_tDyn_txtA(pt1,txtA,att)\
 GR_set_txtA(OPERS_TDYN+OPERS_CLOSE,pt1,txtA,att);



void GR_temp_symV3 (int typ, int att, Point *p1, Vector *v1, double sc);
#define GR_temp_symV3(typ,att,p1,v1,sc)\
 GR_set_symV3(OPERS_TEMP+OPERS_CLOSE,typ,att,p1,v1,sc);

void GR_tDyn_symV3 (int typ, int att, Point *p1, Vector *v1, double sc);
#define GR_tDyn_symV3(typ,att,p1,v1,sc)\
 GR_set_symV3(OPERS_TDYN+OPERS_CLOSE,typ,att,p1,v1,sc);



void GR_temp_dimen (Dimen *dim1, int att);
#define GR_temp_dimen(dim1,att)\
 GR_set_dimen(OPERS_TEMP+OPERS_CLOSE,dim1,0L,att);

void GR_perm_dimen (Dimen *dim1, long dbi, int att);
#define GR_perm_dimen(dim1,dbi,att)\
 GR_set_dimen(OPERS_PERM+OPERS_CLOSE,dim1,dbi,att);



void GR_temp_dim3 (Dim3 *dim1, long dbi, int att);
#define GR_temp_dim3(dim1,dbi,att)\
 GR_set_dim3(OPERS_TEMP+OPERS_CLOSE,dim1,dbi,att);

void GR_perm_dim3 (Dim3 *dim1, long dbi, int att);
#define GR_perm_dim3(dim1,dbi,att)\
 GR_set_dim3(OPERS_PERM+OPERS_CLOSE,dim1,dbi,att);



void GR_temp_txt__ (AText *txa, int att);
#define GR_temp_txt__(txa,att)\
 GR_set_txt__(OPERS_TEMP+OPERS_CLOSE,txa,0L,att);

void GR_perm_txt__ (AText *txa, long dbi, int att);
#define GR_perm_txt__(txa,dbi,att)\
 GR_set_txt__(OPERS_PERM+OPERS_CLOSE,txa,dbi,att);



void GR_temp_txtG (GText *txG, int att);
#define GR_temp_txtG(txG,att)\
 GR_set_txtG(OPERS_TEMP+OPERS_CLOSE,txG,0L,att);

void GR_tDyn_txtG (GText *txG, int att);
#define GR_tDyn_txtG(txG,att)\
 GR_set_txtG(OPERS_TDYN+OPERS_CLOSE,txG,0L,att);

void GR_perm_txtG (GText *txG, long dbi, int att);
#define GR_perm_txtG(txG,dbi,att)\
 GR_set_txtG(OPERS_PERM+OPERS_CLOSE,txG,dbi,att);



void GR_temp_mdr (ModelRef *mdr, int att);
#define GR_temp_mdr(mdr,att)\
 GR_set_mdr(OPERS_TEMP+OPERS_CLOSE,mdr,0L,att);

void GR_tDyn_mdr (ModelRef *mdr, int att);
#define GR_tDyn_mdr(mdr,att)\
 GR_set_mdr(OPERS_TDYN+OPERS_CLOSE,mdr,0L,att);

void GR_perm_mdr (ModelRef *mdr, long dbi);
#define GR_perm_mdr(mdr,dbi)\
 GR_set_mdr(OPERS_PERM+OPERS_CLOSE,mdr,dbi,GR_TMP_DEF);



// display MemTab
void GR_temp_mtb (MemTab *mtb, int att);
#define GR_temp_mtb(mtb,att)\
 GR_set_mtb(OPERS_TEMP+OPERS_CLOSE,mtb,att);

void GR_tDyn_mtb (MemTab *mtb, int att);
#define GR_tDyn_mtb(mtb,att)\
 GR_set_mtb(OPERS_TDYN+OPERS_CLOSE,mtb,att);



void GR_temp_osu (ObjGX *oxi, long dbi, int att);
//  display binary surface
//  dbi used for get surf-patches, else set to 0
//  att     see INF_?
#define GR_temp_osu(oxi,dbi,att)\
 GR_set_osu(OPERS_TEMP+OPERS_CLOSE,oxi,dbi,att);



int GR_set_nifac (int opers, long dbi, MshFac *nifa, int att);
void GR_temp_sur (long dbi, int att);
#define GR_temp_sur(dbi,att)\
 GR_set_sur(OPERS_TEMP+OPERS_CLOSE,dbi,att);

void GR_tDyn_sur (long dbi, int att);
#define GR_tDyn_sur(dbi,att)\
 GR_set_sur(OPERS_TDYN+OPERS_CLOSE,dbi,att);

void GR_tDyn_osu (ObjGX *oxi, long dbi, long att);
#define GR_tDyn_osu(oxi,dbi,att)\
 GR_set_osu(OPERS_PERM+OPERS_CLOSE,oxi,dbi,att);

void GR_perm_sur (long dbi, long att);
#define GR_perm_sur(dbi,att)\
 GR_set_sur(OPERS_PERM+OPERS_CLOSE,dbi,att);



void GR_temp_nifac (long dbi, MshFac *nifa, int att);
#define GR_temp_nifac(dbi,nifa,att)\
 GR_set_nifac(OPERS_TEMP+OPERS_CLOSE,dbi,nifa,att);

void GR_tDyn_nifac (long dbi, MshFac *nifa, int att);
#define GR_tDyn_nifac(dbi,nifa,att)\
 GR_set_nifac(OPERS_TDYN+OPERS_CLOSE,dbi,nifa,att);

void GR_perm_nifac (long dbi, MshFac *nifa, int att);
#define GR_perm_nifac(dbi,nifa,att)\
 GR_set_nifac(OPERS_PERM+OPERS_CLOSE,dbi,nifa,att);




//----------------------------------------------------------------
int GR_temp_src (char *src, int att, int mode);
int GR_temp_nobj (int ind, long dbi,
                   int form, void *obj, int oNr, int att, int mode);
int GR_temp_dbo (int ind,  int typ, long dbi, int att, int mode);
int GR_temp_otb (long *dli, int att, ObjTab *otb1);

int GR_temp_symB (Point *pt1, int typ, int att);
int GR_temp_vc (Vector *vc1, Point *pt1, int att, int mode);
int GR_temp_ang (int att, Point *ptc, Vector *vx, Vector *vz, double ang1);
int GR_temp_rect1 (Point *p1, Vector *vx, double dx, Vector *vy, double dy,
                     int att);

int GR_temp_tx2A (Point *pt1, char *txtA, int att);
int GR_temp_txiA (Point *pt1, int ii, int att);
int GR_temp_txi2 (Point2 *pt1, int ii, int att);

int GR_temp_att__ (int iAtt);
int GR_temp_att_1 (int iAtt);

//----------------------------------------------------------------
int GR_tDyn_dbo (int typ, long dbi, int att, int mode);
int GR_tDyn_pcv2 (Point2 *pta, int ptnr, int att);
int GR_tDyn_ell (CurvElli *el1, int att);
int GR_tDyn_el2c (CurvEll2C *el2c, int att);
int GR_tDyn_pcv3p2 (Point2 *p1, Point2 *p2, Point2 *p3, int att);
int GR_tDyn_bez (CurvBez *bez, int att, Memspc *workSeg);
void GR_tDyn_psp3 (long *ind, long dbi, int attInd, ObjGX *cv1, double zval);
int GR_tDyn_npt_2 (Point2 *pta, int ptnr, int att);
int GR_tDyn_ln_2pt (Point *pt1, Point *pt2, int att);
int GR_tDyn_npt__ (Point *pta, int ptnr, int att);
int GR_tDyn_npti (int ptNr, Point *pTab, int typ, int ptAtt, int txAtt);
int GR_tDyn_symB__ (Point *pt1, int typ, int att);
int GR_tDyn_nsymB (int ptNr, Point *pTab, int typ, int att);

int GR_tDyn_tx2A (Point2 *pt1, char *txtA, int att);
int GR_tDyn_txiA (Point *pt1, int ii, int att);
int GR_tDyn_txi2 (Point2 *pt1, int ii, int att);

int GR_tDyn_box__ (Point *p1, Point *p2, int att);
int GR_tDyn_box2__ (Point2 *p1, Point2 *p2, int att);


//----------------------------------------------------------------
  void GR_Init1 ();
// void GR_CrePoint  (long *ind, int attInd, Point *pt1);
int AP_add_pt (long *dbi, int att, Point *pt1);

  void GR_CreDisk   (long *ind, int attInd, Point *pc, Vector *vz,
                                          double rd1, double rd2);
  void GR_CreTriaFan(long *ind, int attInd, Point *pc, int anz, Point *pa1);
  void GR_CrePrism  (long *ind, int attInd, int anz, Point *pa1, Point *pa2);
  void GR_CreCyl    (long *ind, int attInd,Point *pt1, Point *pt2,
                                       double rd1, double rd2);
  void GR_CreTor    (long *ind, int attInd, Circ *ci1, double rd1);
  void GR_CreTorSeg (long *ind, int attInd, Circ *ci1, Circ *ci2);
  int  GR_CreSol    (long *ind, int attInd, ObjGX *db1);


  int  GR_Draw_oid_dir_npt (long *dli, Point *pta, int ptNr);

  void GR_DrawDitto2 (long *ind, Point *pt, double az, char mir, Ditto *di);
  int  GR_DrawSur   (ObjGX *oxi, int att, long apt_ind);

  void GR_DrawDisk  (long *ind, int attInd, Point *pc, Vector *vz,
                                          double rd1, double rd2);
  void GR_DrawTriaFan(long *ind, int attInd, Point *pc, int anz, Point *pa1);
  int  GR_DrawTriaStrip(long *ind,int attInd,
                        int ptUNr,int ptVNr,Point *pa,int newS);

  void GR_DrawPrism (long *ind, int attInd, int anz, Point *pa1, Point *pa2);
  void GR_DrawCyl   (long *ind, int attInd, Point *pt1, Point *pt2,
                                          double rd1, double rd2);
  void GR_DrawTor   (long *ind, int attInd, Circ *ci1, double rd1);
  void GR_DrawTorSeg(long *ind, int attInd, Circ *ci1, Circ *ci2);


  int GR_DrawFan (Point *pt1, int ptAnz, Point *pa1, int typ, int newS);
  int GR_DrawStrip (Point *pa1, Point *pa2, int ptAnz, int newS);
  int GR_Draw_spu (long *ind, int att, int pNr, Point *pTab);

  int GR_Disp_vc2 (Vector2 *vc1, Point2 *pt1, int att, int mode);
  int GR_Disp_tria (Triang *tria, int att);

  int GR_Disp_patch (int gTyp, int icol, int pNr, Point *pa);
  int GR_Disp_fan (Point *ps, Point *pa1, Point *pa2, int pNr, int att);
  int GR_Disp_spu (int pNr, Point *pTab, int att);
  int GR_Disp_spu1 (int pNr, Point *pTab, int att);
  int GR_Disp_sru (ObjGX *oxi, int att);
  int GR_Disp_sur (ObjGX *oxi, int att);
  int GR_Disp_sbsp (int ptUNr, int ptVNr,  Point *pTab, int att);

  int GR_set_z2    (double dz);
  double GR_gtx_ckBlockWidth (char*, double);
  int    GR_gxt_strLen (char*);
  double GTX_chw_  (double);
  double GTX_cw_   ();
  double GTX_chh_  (double);
  double GTX_chhl_ (double);
  double GTX_chhl2 (double);
  double GTX_scale (double);
  double GTX_siz__ (double);



//================================================================
// inline functions

// GR_gxt_strLen         get stringlength of textnote
// ignore all controlcharacters '['
#define GR_gxt_strLen(txt)\
 strlen(txt) - UTX_cnr_chr (txt, '[')

void GR_pt3_pt2 (Point *p3, Point2 *p2);
#define GR_pt3_pt2(p3,p2)\
  {(p3)->x = (p2)->x; (p3)->y = (p2)->y; (p3)->z = GL2D_Z;}

/*========== EOF ==============*/
