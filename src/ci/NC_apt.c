// CommandInterpreter - decode-functions
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
2001-09-20 enum Typ_Cmd1 -> NC_apt.h. RF.
2001-06-05 Portierung Unix

-----------------------------------------------------
*/
/*!
\file  ../ci/NC_apt.c
\brief CommandInterpreter - decode-functions 
\code
=====================================================
List_functions_start:

APT_get_modMax1           return APT_modMax1

APED_dbo_oid             Text, zB "P12" zerlegen into objTyp und objInd.
APT_decode_txt           remove "" from "xx"
APT_dec_Ang_Obj          Angle from Value or Point (out als rad)
APT_decode_angd__        Angle in Degrees
APT_decode_angd__d1
APT_decode_angd__r1
APT_decode_mod
APT_dec_Par1_Obj         parameter from curve-obj
APT_decode_func          decode functions (<fncnam>(<parameters>))
APT_decode_inp           aus_typ/aus_tab -> IN_obj

// APT_decode_ausdr         DO NOT USE; new func: ATO_ato_srcLn__
APT_decode_ausdr1        decode expr -> aus_typ/aus_tab
APT_decode_ValTab        aus_typ/aus_tab -> fTab
APT_decode_comp1         do math operations in aus_typ/aus_tab
APT_decode_Opm           do math operations
APT_decode_Fmc           execute math.function

APT_decode_var
APT_decode_pt
  APT_decode_pt0
  APT_decode_pt1
  APT_decode_pt2
APT_decode_vc
APT_decode_ln
APT_decode_ci
  APT_decode_ci1
  APT_decode_cttr
  APT_decode_cttt
APT_decode_ell
APT_decode_clot
APT_decode_psp3
APT_decode_bsp_
APT_decode_pt2bsp
APT_decode_conv_cv
APT_decode_conv_pol
APT_decode_conv_pol2
APT_decode_rbsp
APT_decode_rec
APT_decode_rect1
APT_decode_rect2
APT_decode_cv
APT_decode_pol
APT_decode_pol2
APT_decode_ccv__
APT_decode_ccv_nxt
APT_decode_ccv2
APT_decode_gtxt
APT_decode_dim3
APT_decode_dima
APT_decode_dimen
APT_decode_dimdia
APT_decode_ldr_
APT_decode_ldrs
APT_decode_ldrc
APT_decode_ldrp
APT_decode_tag
APT_decode_note
APT_decode_img
APT_decode_msh__
APT_decode_msh_p
APT_decode_pln1          plane -
APT_decode_pln2          plane PERP
APT_decode_pln_rsys      plane RSYS
APT_decode_sur
APT_decode_spl
APT_decode_sru
APT_decode_srv
APT_decode_sbsp
APT_decode_s_sph
APT_decode_s_cyl
APT_decode_s_con  ?
APT_decode_hat
APT_decode_fsub
APT_decode_sol
APT_decode_bsph
APT_decode_bcon
APT_decode_btor
APT_decode_model
APT_decode_refsys1
APT_decode_Activ
APT_decode_att
APT_decode_tra
APT_decode_tool

APT_decode_goAxis        change  atomic-obj (Typ_goAxis)) into DB-plane
APT_decode_goRadius
APT_decode_modUnlim      get UNL|UNL2|UNL1

APT_store_obj            create DB-object from aus_typ/aus_tab

APT_cp_ausd              copy expr
APT_get_String           copy String until 1. ""
APT_get_TxAll            copy complete String; remove 1. & last ""
APT_get_Txt              copy next word from string
APT_get_Val
APT_str2fnam
APT_cmp_ato2             compare 2 atomic objects (DB-Objects only) if equal
APT_check_cmdSub         search word in ObjCodTab

APT_grp_decode1          get startIndex and objNr of objGrp 1
APT_grp_decode2          make a list of db-obj's (copy iNr records from aus_.)

APT_iso_cktyp            define type of resulting object for isoparmetric curve
APT_iso_obj              decode "ISO A# <parVal> [CX]"

APT_TNG__                tangent

APT_INT__                Intersection
APT_int_cktyp            define type of resulting object for intersect

APT_CUT__                Cut (trim) object
APT_trim_obj
APT_trim_u0
APT_trim_u1

APT_tra_obj              decode Line "TRA P23 R20 [REV]"
APT_prj_obj_vc
APT_prj_obj_perp
APT_mir_obj

APT_solv_ln_parl_mod     parallel line from DB-line, mod, offset
APT_solv_vc_mod          perp. vector from vector, normal-vector, mod
APT_solv_mod
APT_solv_mod_1
APT_solv_x_max
APT_solv_y_max
APT_solv_x_min
APT_solv_y_min
APT_solv3D_mod
APT_solv3d_LOZ
APT_solv3d_LOX
APT_solv3d_HIZ
APT_solv3d_HIY
APT_solv3d_HIX

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



Compile:

cc -c NC_apt.c 



=============================================================
*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>                        /* printf ..   */
#include <stdlib.h>                       /* atoi ..     */
#include <string.h>                       /* strcmp ..   */
#include <math.h>                         /* sin ..      */
#include <ctype.h>                // f. isdigit ..

// Achtung: braucht mit w32 __pctype u ___mb_cur_max / unresolvbar !
//#include <ctype.h>                        /* isdigit ... */


// #include "../ut/ut_umem.h"                // UME_save
#include "../ut/ut_geo.h"              // Point
#include "../ut/ut_geo_const.h"        // UT2D_PT_NUL
// #include "../ut/ut_tol_const.h"        // UT_DISP_ln UT_LEN_TNG
#include "../ut/gr_types.h"               // SYM_* ATT_* Typ_Att_* LTYP_*
#include "../ut/ut_txt.h"
#include "../ut/ut_TX.h"
#include "../ut/ut_bspl.h"             // UT3D_parbsp_par1
#include "../ut/ut_elli.h"             // UT3D_angr_par1_ell
#include "../ut/ut_plg.h"              // UT3D_par_par1plg
#include "../ut/ut_ox_base.h"          // OGX_SET_INDEX
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_os.h"               // OS_..
#include "../ut/func_types.h"                  // Typ_Att_hili
#include "../ut/ut_gtypes.h"           // AP_src_typ__
#include "../ut/ut_memTab.h"           // MemTab_..

#include "../gr/ut_gr.h"               /* Typ_PT ...  */
#include "../gr/ut_DL.h"

#include "../db/ut_DB.h"                  // DB_GetCurv ..

#include "../ci/NC_Main.h"
#include "../ci/NC_apt.h"                 // T_CW

#include "../xa/xa.h"                  // mem_cbuf1_SIZ
#include "../xa/xa_ed.h"               // AP_mode_enter
#include "../xa/xa_mem.h"              // memspc51-55
#include "../xa/xa_edi__.h"            // ED_GetNxtLin
#include "../xa/xa_uid.h"              // UI_MODE_CAD
#include "../xa/xa_msg.h"                 // MSG_*







//===========================================================================

// Externals aus ../ci/NC_Main.c:
extern double    APT_ModSiz;
extern int       APT_lNr;           // die momentane APT-LineNr
extern int       APT_hide_parent;        // 0=not, else yes


//double    APT_infin  = 1000.0;
// extern double    APT_infin;
//double    APT_ln_len =  100.0;
// extern double APT_ln_len;        // unbegrenzte Linien; wenn Wert höher,
//double    APT_equ_pt   = 0.0002;   // equal PointsTol
// extern double    APT_TOL_pt;   // equal PointsTol
//double    APT_rad_max  = 9999.0;   // groessere Radien zerlegen;
// extern double    APT_rad_max;   // groessere Radien zerlegen;

extern int     APT_obj_stat;
// extern int     APT_obj_typ;
extern int     APT_subTyp;               // subTyp of created obj;
extern int     APT_3d_mode;

extern char*   APT_defTxt;          // die Textzeile ohne N#=

extern Memspc  APTSpcObj;
extern Memspc  APTSpcTmp;


// ex ../ut/ut_gtypes.c
extern char *ObjCodTab[];



// ex ../xa/xa.c:
extern int       WC_sur_ind;            // Index auf die ActiveConstrPlane
extern Plane     WC_sur_act;            // Constr.Plane
extern Mat_4x3   WC_sur_mat;            // TrMat of ActiveConstrPlane
extern Mat_4x3   WC_sur_imat;           // inverse TrMat of ActiveConstrPlane
extern int       AP_modact_ind;         // -1=primary Model is active;
                                        // else subModel is being created
extern int       aus_SIZ;
extern AP_STAT   AP_stat;



// ex ../xa/xa_ui.c:
extern int       UI_InpMode;


// ex ../gr/ut_DL.c
extern long DL_temp_ind;        // if(>0) fixed temp-index to use; 0: get next free


//===========================================================================
// Local:

ObjG         IN_obj[20];           // die zwischengespeicherten Input-objects
int          IN_anz;               // Anzahl Objekte in IN_obj

int          IN_Dreh;              // die aktuelle Drehrichtung
int          IN_ModIn, IN_Mod2In;  // Nebenwort T_IN oder T_OUT
int          ModTab[4], ModAnz;    // die zwischengespeicherten NC-Nebenwörter


static int   APT_modMax1=4, APT_modMax2;        // nr of solutions ..

static char  APT_spc1[512];


// info for obj where point is on:
//   APT_prim_typ APT_prim_ind APT_prim_par APT_prim_seg APT_prim_sg2
       int    APT_prim_typ;
static long   APT_prim_ind;
static double APT_prim_par;
static int    APT_prim_seg;       // active MOD(); segNr; 1=first
static int    APT_prim_sg2;
// static void  *APT_prim_dat;


static struct {double du, dv;} APT_ptPars;  // parameters of temporary points







//================================================================
  int APT_set_primSeg (int iMod) {
//================================================================
/// keep solution-nr (MOD-#)

  APT_prim_seg = iMod;

  return 0;

}


//================================================================
  int APT_set_modMax (int iMod) { 
//================================================================
/// keep max nr of solutions, max. deg of b-splines
 
  APT_modMax1 = iMod;

  return 0;

}


//================================================================
  int APT_get_modMax1 () {
//================================================================

  // printf("MMMMMMMMMMMMM APT_get_modMax1 %d\n",APT_modMax1);

  return APT_modMax1;

}


//================================================================
  int APT_get_modMax2 () {
//================================================================

  // printf("MMMMMMMMMMMMM APT_get_modMax2 %d\n",APT_modMax2);

  return APT_modMax2;

}



//================================================================
  int APT_get_mod1 () {
//================================================================
/// get MOD()
 
  // printf("MMMMMMMMMMMMM APT_get_mod1 %d\n",APT_modMax1);

  return (IMAX(APT_prim_seg,0));

}



//=========================================================================
  int APT_decode_pt0 (Point *pt_out, int *ausInd,
                      int aus_anz, int *aus_typ, double *aus_tab){
//=========================================================================

  int    ind;



  ind = *ausInd;

  if       (aus_typ[ind]   == Typ_PT) {
    *pt_out = DB_GetPoint ((long)aus_tab[ind]);
    ++ind;
    goto Fertig;
  }


  if(aus_typ[ind]   != Typ_Val) goto ParErr;
  pt_out->x = aus_tab[ind];
  ++ind;


  if(aus_typ[ind]   != Typ_Val) goto ParErr;
  pt_out->y = aus_tab[ind];
  ++ind;


  if(aus_typ[ind] == Typ_ZVal) {
    pt_out->z = aus_tab[ind];
    ++ind;
  } else {
    pt_out->z = 0.;
  }




  Fertig:
  // DEB_dump_obj__ (Typ_PT, pt_out, "");
  *ausInd = ind;
  return ind;




  ParErr:
  TX_Error(" Punkt - Parameter Error in APT_decode_pt0");
  return -1;


}

/*
//=========================================================================
  int APT_decode_pt3 (Point *pt1, 
                      int aus_anz, int aus_typ[],double aus_tab[]){
//=========================================================================
// dzt unused; ersetzt durch APT_decode_pt

  int       i1, i2, rc;
  long      l1;
  Circ      ci1;



  printf("APT_decode_pt3 %d\n",aus_anz);
  for(i1=0; i1<aus_anz; ++i1) {
    printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  }




    //----------------------------------------------------------------
    if((aus_anz == 3)              &&
      ((aus_typ[0] == Typ_Val)     &&
       (aus_typ[1] == Typ_Val)     &&
       (aus_typ[2] == Typ_Val)))   {
      pt1->x = aus_tab[0];
      pt1->y = aus_tab[1];
      pt1->z = aus_tab[2];


    //----------------------------------------------------------------
    } else if(aus_anz == 2)    {

      if((aus_typ[0] == Typ_Val)   &&
         (aus_typ[1] == Typ_Val))     {
        pt1->x = aus_tab[0];
        pt1->y = aus_tab[1];
        pt1->z = 0.;

      } else if((aus_typ[1] == Typ_modif) ||
                (aus_typ[1] == Typ_Val))      {
        // printf(" point = start/endpt\n");
        // 0: Startpunkt; 1: endpunkt von Obj aus_xx[0] holen
        i1 = aus_typ[0];     // apt_typ
        l1 = aus_tab[0];     // apt_ind
        i2 = aus_tab[1];     // ptNr
        rc = UTO_pt_objind (pt1, i1, l1, i2);
        if(rc < 0) return -1;

      } else {
        return -1;
      }



    //----------------------------------------------------------------
    } else if(aus_anz == 1)    {

      if(aus_typ[0] == Typ_CI)   {
        ci1 = DB_GetCirc  ((long)aus_tab[0]);
        *pt1 = ci1.pc;

      } else {
        i1 = aus_typ[0];     // apt_typ
        l1 = aus_tab[0];     // apt_ind
        rc = UTO_pt_objind (pt1, i1, l1, 0);
        if(rc < 0) return -1;


      }


    //----------------------------------------------------------------
    } else {
      return -1;
    }



  DEB_dump_obj__ (Typ_PT, pt1, "ex APT_decode_pt3:");
  return 0;

}
*/


//=========================================================================
  int APT_decode_pt2 (long *ptInd, int *ausInd,
                      int aus_anz, int aus_typ[],double aus_tab[]){
//=========================================================================
// get DB-point from atomic-point (returns dynamic point-index)
// einen Punktindex aus austab holen.
// Wenn Punkt direkt angegeben:
//   als dynam. punkt anlegen und dessen Index retournieren.

// Achtung: RC=aus_anz (wird durch Fortsetzungszeilen veraendert !)

// Input:
//   *ausInd  startindex in aus_typ, aus_tab
// Output:
//   *ausInd  startindex in aus_typ, aus_tab

// TODO: integrate into DBO_dbo_ato__


  int   i1, ind;
  long  l1;
  char   *p1;
  Point pt1;


  printf("APT_decode_pt2 %d %d\n",*ausInd,aus_anz);


  ind = *ausInd;


  L_start:


  if(aus_typ[ind] == Typ_PT) {
    *ptInd = (long)aus_tab[ind];
    *ausInd = ind + 1;
    goto L_OK;
  }

 
  if ((aus_anz >= ind + 3)        &&
      (aus_typ[ind]  ==Typ_Val)   &&
      (aus_typ[ind+1]==Typ_Val)   &&
      (aus_typ[ind+2]==Typ_ZVal)) {
     
    // dynam. punkt anlegen
    pt1.x = aus_tab[ind];
    pt1.y = aus_tab[ind+1];
    pt1.z = aus_tab[ind+2];
    l1 = -1;
    *ptInd = DB_StorePoint (l1, &pt1);
    *ausInd = ind + 3;
    goto L_OK;
  }


  if ((aus_anz >= ind + 2)        &&
      (aus_typ[ind]  ==Typ_Val)   &&
      (aus_typ[ind+1]==Typ_Val)) {

    // dynam. punkt anlegen
    pt1.x = aus_tab[ind];
    pt1.y = aus_tab[ind+1];
    pt1.z = 0.;
    l1 = -1;
    *ptInd = DB_StorePoint (l1, &pt1);
    *ausInd = ind + 2;
    goto L_OK;
  }



  printf(" Error APT_decode_pt2\n");
  return -1;




  L_OK:;
    printf("ex APT_decode_pt2 ausInd=%d ptInd=%ld\n",*ausInd,*ptInd);
  return aus_anz;

}


//================================================================
  int APT_decode_pt1 (Point *pt_out,
                      int aus_ind, int aus_typ[], double aus_tab[]){
//================================================================
// Get point out of aus_typ/aus_tab; return next free objIndex.
// Input:
//   aus_ind   first free objIndex
//   aus_typ,aus_tab
// RetCod:     next free objIndex (aus_ind)

// aus u_tab einen Punkt lösen; bestehen aus 2 Vars oder einem APT_PT.
// aus_ind zeigt beim Input auf das erste zugehörige aus_tab-Objekt,
// am Ende auf das naechste freie.



  int ind = aus_ind;


  //TX_Print("APT_decode_pt1 %d %d  (%d)",aus_typ[ind],aus_typ[ind+1],ind);



  if       (aus_typ[ind]   == Typ_PT) {
    *pt_out = DB_GetPoint ((long)aus_tab[ind]);
    ++ind;
    goto Fertig;
  }



  // if(APT_3d_mode == OFF) {
  if((aus_typ[ind]   == Typ_Val)        &&
     (aus_typ[ind+1] == Typ_Val)        &&
     (aus_typ[ind+2] == Typ_Val))    {
      pt_out->x = aus_tab[ind];
      ++ind;
      pt_out->y = aus_tab[ind];
      ++ind;
      pt_out->z = aus_tab[ind];
      ++ind;
      goto Fertig;
  }

  if((aus_typ[ind]   == Typ_Val)        &&
       (aus_typ[ind+1] == Typ_Val))    {
      pt_out->x = aus_tab[ind];
      ++ind;
      pt_out->y = aus_tab[ind];
      ++ind;
      pt_out->z = 0.;
      goto Fertig;
  }



  goto ParErr;



  Fertig:
  return ind;




  ParErr:
  TX_Error(" Punkt - Parameter Error ****");
  return -1;


}


//=======================================================================
  int APT_decode_ci1 (Circ *ci1, Point *old_pt,
                      int aus_anz, int *aus_typ, double *aus_tab) {
//=======================================================================
// APT_decode_pt1 geht ned, Z-Koord muss Typ_ZVal sein (nicht Typ_Val)
// APT_decode_pt2 geht ned, weil es die Punkte anlegt !!
// siehe auch APT_wcut_line

  int    irc, ii, i1, ityp;
  Point  pt1;
  Vector vc1;

  

  // printf("APT_decode_ci1 %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }



  ii = 0;

  // typ 0: Pcen rad [vz] [dir]
  // typ 1: Pcen Pcir [vz] [dir]
  // typ 2: p1 p2 pc [vz] [dir]           dzt ohne "ARC" !!!


  irc = APT_decode_pt0 (&ci1->pc, &ii, aus_anz, aus_typ, aus_tab);
  if(irc < 0) return irc;


  if(aus_typ[ii] == Typ_PT) {
    irc = APT_decode_pt0 (&ci1->p2, &ii, aus_anz, aus_typ, aus_tab);
    if(irc < 0) return irc;

    // kommt noch ein Punkt ?
    if(aus_typ[ii] == Typ_PT) {
      ci1->p1 = ci1->pc;
      irc = APT_decode_pt0 (&ci1->pc, &ii, aus_anz, aus_typ, aus_tab);
      if(irc < 0) return irc;
      ityp = 2;  // ango fehlt

    } else {
      ci1->p1   = ci1->p2;
      ci1->ango = RAD_360;
      ityp = 1;
    }
    ci1->rad  = UT3D_len_2pt (&ci1->pc, &ci1->p1);


  } else if((aus_typ[ii] == Typ_Val)||(aus_typ[ii] == Typ_Angle)) {
    ci1->rad  = aus_tab[ii];
    ci1->ango = RAD_360;
    ++ii;
    ityp = 0;  // p1, p2 fehlt
  }


  // nun ev. Vektor
  if(aus_typ[ii] == Typ_VC) {
    ci1->vz = DB_GetVector((long)aus_tab[ii]);
    ++ii;
  } else {
    ci1->vz = WC_sur_act.vz;
  }



  // zuletzt ev. Drehsinn
  if(aus_typ[ii] == Typ_cmdNCsub) {   // 11=CW
    i1 = aus_tab[ii];
    if(i1 == T_CW) {
      ci1->rad = -ci1->rad;
    // }else if(i1 == T_CCW) {
    }
  }


  if(ityp == 0) {     // p1 und p2 fehlen noch ..
    // create the X-vector = Normalvektor auf vc, der in der X-Y-Plane liegt.
    UT3D_vc_perpvcplXY (&vc1, &ci1->vz);
    UT3D_pt_traptvclen (&ci1->p1, &ci1->pc, &vc1, ci1->rad);
    ci1->p2 = ci1->p1;

  } else if(ityp == 2) {     // ango fehlt
    ci1->ango = UT3D_angr_ci_p1_pt (ci1, &ci1->p2);
  }

  *old_pt = ci1->p2;

  // exit(0);
  // DEB_dump_obj__ (Typ_CI, ci1, "ex APT_decode_ci1\n");

  return 0;

}

/*
//================================================================
  int APED_dbo_oid (int *defTyp, int *defInd, char* txtIn) {
//================================================================
// Einen Text, zB "P12" zerlegen into objTyp = Typ_PT und objInd = 12.
    // die Zielvariable bestimmen (besteht aus einem Typ und einem Index)
    // Zuerst in Uppercase umwandeln
// see also AP_cut_defHdr

  // IN:
// txtIn zB  "P12"; ergibt defTyp=Typ_PT, defInd=12.

  // OUT:
// defTyp: siehe wincut_base.h

// Retcodes:
  // 0 = OK
 // -1 = nur defTyp gefunden (zB P(..) oder D(..))
 // -2 = Fehler


  int   rc, i1, ilen;
  char  *txt, typ, s1[4];   //txt[80];
  // char  *buf;


  // printf("APED_dbo_oid |%s| %p\n",txtIn,defInd);


  if(strlen(txtIn) < 2) {return -2;}

  // skip lead. blanks
  // buf = UTX_pos_1n (txtIn);
  txt = txtIn;
  while (*txt  == ' ')  { ++txt; }

  // to Upper
  // UTX_cp_word_2_upper (txt, buf);
  if(islower(txt[0])) {
    typ = toupper(txt[0]);
  } else {
    typ = txt[0];
  }
  


  // find typ
  // *defTyp = AP_typ_typChar (*txt);
  *defTyp = AP_typ_typChar (typ);
  if(*defTyp == Typ_Error) {
    *defInd = 0;
    rc = -2;
    goto Fertig;
  }


  // check for P(..) oder D(..)
  if(txt[1] == '(') {
    rc = -1;
    goto Fertig;
  }


  // numerisch ?
  // printf(" txt[1]=|%c|\n",txt[1]);
  if (isdigit(txt[1])) {
    *defInd = atoi (&txt[1]);
    rc = 0;
    goto Fertig;
  }


  rc = -2;

  if(*defTyp == Typ_VC) {  // test DX DY DZ DIX DIY DIZ
    ++txt;
    i1 = strlen(txt);
    if(i1 > 2) goto Fertig;
    UTX_cp_nchr_2_upper (s1, txt, i1);
    s1[i1] ='\0';

    if (!strcmp (s1, "X")) {
      *defInd = DB_VCX_IND;

    } else if (!strcmp (s1, "Y")) {
      *defInd = DB_VCY_IND;

    } else if (!strcmp (s1, "Z")) {
      *defInd = DB_VCZ_IND;

    } else if (!strcmp (s1, "IX")) {
      *defInd = DB_VCIX_IND;

    } else if (!strcmp (s1, "IY")) {
      *defInd = DB_VCIY_IND;

    } else if (!strcmp (s1, "IZ")) {
      *defInd = DB_VCIZ_IND;

    } else { 
      goto Fertig;
    }
    rc = 0;


  } else if(*defTyp == Typ_PLN) {  // test DX DY DZ DIX DIY DIZ
    ++txt;
    i1 = strlen(txt);
    if(i1 > 1) goto Fertig;
    UTX_cp_nchr_2_upper (s1, txt, i1);
    s1[i1] ='\0';

    if (!strcmp (s1, "X")) {
      *defInd = DB_PLX_IND;

    } else if (!strcmp (s1, "Y")) {
      *defInd = DB_PLY_IND;

    } else if (!strcmp (s1, "Z")) {
      *defInd = DB_PLZ_IND;

    } else {
      goto Fertig;
    }
    rc = 0;
  }



  Fertig:
    // printf("ex APED_dbo_oid: rc=%d,Typ=%d, Ind=%d\n",rc,*defTyp,*defInd);
    // printf("                   txtIn=|%s|\n",txtIn);
    // printf("ex APED_dbo_oid |%s| %p\n",txt,defInd);

  return rc;

}
*/

/*=====================================================================*/
  char** APT_decode_txt (char** buf) {
/*=====================
*/

  /* int       ctyp; */
  /* double    d1; */
  /* char      deli, *w, *w_next, *cp1; */
  /* char      txtOut[32]; */

     char*     endpos;
  static       char**    cbuf;


  /* Ist 1. Zeichen " ?? */
  if(**buf == '"') {
    (*buf)++;
  }


  /* Ist letztes Zeichen " ?? */
  endpos = *buf;
  endpos = &endpos[strlen(endpos)-1];
  /* TX_Print("end |%s|\n",endpos); */

  if(*endpos == '"') {
    *endpos = '\0';
  }


  cbuf = buf;

  //TX_Print("APT_decode_txt |%s|\n",*cbuf);

  return cbuf;

}


//===========================================================================
  int APT_decode_mod (double *d1, char* cBuf) {
//===========================================================================

  int  mtyp;
  // int  ityp;
  // char *p1;


  // printf("APT_decode_mod |%s|\n",cBuf);




  // Typ_Txt -
  if       (cBuf[0] == 'H') {
    // ityp = Typ_modCoord;
    if        (!strcmp (cBuf, "HIX")) {
      mtyp = Typ_modHIX;
      goto L_weiter;

    } else if (!strcmp (cBuf, "HIY")) {
      mtyp = Typ_modHIY;
      goto L_weiter;

    } else if (!strcmp (cBuf, "HIZ")) {
      mtyp = Typ_modHIZ;
      goto L_weiter;

    } else {
      goto L_err1;
    }


  // kann noch sein LOX LOY LOZ
  } else if(cBuf[0] == 'L') {
    // ityp = Typ_modCoord;
    if        (!strcmp (cBuf, "LOX")) {
      mtyp = Typ_modLOX;
      goto L_weiter;

    } else if (!strcmp (cBuf, "LOY")) {
      mtyp = Typ_modLOY;
      goto L_weiter;

    } else if (!strcmp (cBuf, "LOZ")) {
      mtyp = Typ_modLOZ;
      goto L_weiter;

    } else {
      goto L_err1;
    }




  // kann noch sein IN OUT
  } else if(!strcmp(cBuf,"IN")) {
    // ityp = Typ_modInOut;
    mtyp = Typ_modIN;
    goto L_weiter;


  } else if(!strcmp(cBuf,"OUT")) {
    mtyp = Typ_modOUT;
    goto L_weiter;


  } else if(!strcmp(cBuf,"CW")) {
    mtyp = Typ_modCW;
    goto L_weiter;


  } else if(!strcmp(cBuf,"CCW")) {
    mtyp = Typ_modCCW;
    goto L_weiter;



  // } else {
    // goto L_err1;
  }




  // kann sein Zahl 1-Typ_modHIX
  // mtyp = strtol(cBuf, &p1, 10) - 1;
  mtyp = atoi(cBuf) - 1;
  // printf(" got num.modif %d\n",mtyp);




  L_weiter:
  *d1 = mtyp;

  return 0;



  L_err1:
  // printf("IE_decode_Ln: Err dec |%s|\n",cBuf);
  return -1;

}

//=============================================================================
  int APT_decode_angd__r1 (float *f1, int *ind, int aus_typ[], double aus_tab[]) {
//=============================================================================
// Typ_Angle|Typ_VC --> angr

  int      ii;
  Vector   *vcp;

  ii = *ind;

  if((aus_typ[ii] == Typ_Angle)  ||
     (aus_typ[ii] == Typ_Val))       {
    *f1 = UT_RADIANS(aus_tab[ii]);

  } else if(aus_typ[ii] == Typ_VC) {
    vcp = DB_get_VC ((long)aus_tab[ii]);
    *f1 = UT2D_angr_vc ((Vector2*)vcp);  // Transformation ?

  } else return -1;


  ++ii;
  *ind = ii;
  return 0;

}


//=============================================================================
  int APT_decode_angd__d1 (float *f1, int *ind, int aus_typ[], double aus_tab[]) {
//=============================================================================
// Typ_Angle|Typ_VC --> angd

  int      ii;
  double   d1;
  Vector   *vcp;

  ii = *ind;

  if((aus_typ[ii] == Typ_Angle)  ||
     (aus_typ[ii] == Typ_Val))       {
    *f1 =aus_tab[ii];

  } else if(aus_typ[ii] == Typ_VC) {
    vcp = DB_get_VC ((long)aus_tab[ii]);
    d1 = UT2D_angr_vc ((Vector2*)vcp);  // Transformation ?
    *f1 = UT_DEGREES(d1);

  } else return -1;


  ++ii;
  *ind = ii;
  return 0;

}

  
//============================================================================
  int APT_decode_xyzval (double *d1,
                         int aus_anz, int aus_typ[], double aus_tab[],
                         int *oTyp) {
//============================================================================
/// get X or y ot Z-component of point,vector,line.
// Typ_XVal/Y/Z

  int     i1, irc;
  Vector  vc1;
  Point   pt1;
  void    *p1;


  // printf("APT_decode_xyzval %d\n",*oTyp);
  // for(i1=0;i1<aus_anz;++i1) printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);


  *d1 = 0.;
  irc = 0;



  //==================================================================
  if        (aus_anz    ==   1)     {
  //==================================================================


    //----------------------------------------------------------------
    if((aus_typ[0] >= Typ_Val)&&(aus_typ[0] < Typ_Par1)) {
      *d1 = aus_tab[0];
      goto L_exit;


    //----------------------------------------------------------------
    } else if(aus_typ[0] == Typ_VC) {
      vc1 = DB_GetVector ((long)aus_tab[0]);


    //----------------------------------------------------------------
    } else if(aus_typ[0] == Typ_PT) {
      p1 = DB_get_PT ((long)aus_tab[0]);
      memcpy(&vc1, p1, sizeof(Point));
      

    //----------------------------------------------------------------
    } else if(aus_typ[0] == Typ_LN) {
      p1 = (void*)DB_get_LN ((long)aus_tab[0]);
      UT3D_vc_ln (&vc1, (Line*)p1);


    } else goto L_err_par;




  //==================================================================
  } else goto L_err_par;
  //==================================================================



 L_vc:
   if(AP_IS_2D) UT3D_vc_tra_vc_m3 (&vc1, WC_sur_imat, &vc1);

   if(*oTyp == Typ_XVal) {
     *d1 = vc1.dx;
   } else if(*oTyp == Typ_YVal) {
     *d1 = vc1.dy;
   } else if(*oTyp == Typ_ZVal) {
     *d1 = vc1.dz;
   }


  L_exit:
    // printf(" ex APT_decode_xyzval irc=%d d1=%lf\n",irc,*d1);
  return irc;


  L_err_par:
    TX_Error("Function X() or Y() or Z(): ParameterError ****");
    irc = -1;
    goto L_exit;

}
    

//====================================================================
  int APT_decode_angd__ (double *ang, 
                         int aus_anz,int aus_typ[],double aus_tab[]) {
//====================================================================
// Angle in Degrees
// V#=ANG L# PERP


  int    ii, i1, iRev, iCx, iPerp;
  double d1, d2, d3;
  void   *p1, *p2;
  Vector v1, v2;
  Line   *lnp;


  // printf("APT_decode_angd__ |%d|\n",aus_anz);
  // for(i1=0;i1<aus_anz;++i1) printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);


  *ang = UT_DB_LEER;

  iCx   = 0;     // 0=normal; 1=complementary
  iRev  = 0;     // 0=normal; 1=revers
  iPerp = 0;     // 0=rot-ang; 1=tilt-ang

  // "CX" als letzen parameter abspalten und ganz am Schluss anwenden
  if((aus_typ[aus_anz - 1] == Typ_cmdNCsub) &&
     (aus_tab[aus_anz - 1] == T_CX))                      {
    iCx = 1;
    --aus_anz;
  }


  // "REV" als vorletzen parameter abspalten und ganz am Schluss anwenden
  if((aus_typ[aus_anz - 1] == Typ_cmdNCsub) &&
     (aus_tab[aus_anz - 1] == T_REV))                      {
    iRev = 1;
    --aus_anz;
  }

  // get "PERP"
  if((aus_typ[aus_anz - 1] == Typ_cmdNCsub) &&
     (aus_tab[aus_anz - 1] == T_PERP))                      {
    iPerp = 1;
    --aus_anz;
  }




  //================================================================
  if(aus_anz == 1) {
  //================================================================



  //-----------------------------------------------------------------
  // D [PERP]
  if(aus_typ[0] == Typ_VC)     {
    // (Vector*)p1 = DB_get_VC ((long)aus_tab[0]);
    v1 = DB_GetVector ((long)aus_tab[0]);
      // DEB_dump_obj__(Typ_VC, &v1, "  angd-1D-v1");

    // input is absolute; if constrPlane is active, transfer input into UCS
    UTRA_UCS_WCS_VC (&v2, &v1);
      // DEB_dump_obj__(Typ_VC, &v2, "  angd-1D-v2");

    // get rot- and tilt-angle
    UT3D_2angr_vc__ (&d1, &d2, &v2);

    if(iPerp) *ang = UT_DEGREES(d2);    // tilt-ang
    else      *ang = UT_DEGREES(d1);    // rot-ang

    // *ang = UT_DEGREES(UT2D_angr_vc((Vector2*)p1));

    goto L_exit;


  //-----------------------------------------------------------------
  // <val> [REV]
  } else if(aus_typ[0] == Typ_Val)     {
    *ang = aus_tab[0];
    goto L_exit;


  //-----------------------------------------------------------------
  // L [REV]
  } else if (aus_typ[0] == Typ_LN)     {
    p1 = (void*)DB_get_LN ((long)aus_tab[0]);
    UT3D_vc_ln (&v1, (Line*)p1);

    // input is absolute; if constrPlane is active, transfer input into UCS
    UTRA_UCS_WCS_VC (&v2, &v1);
      // DEB_dump_obj__(Typ_VC, &v2, "  angd-1D-v2");


// TODO: if constrPln is active - compute angles to its X-axis/Z-axis
    UT3D_2angr_vc__ (&d1, &d2, &v2);
    // UT3D_3angr_2vc (&d1, &d2, &d3, &v1, &WC_sur_act.vz);

    if(iPerp) *ang = UT_DEGREES(d2);    // tilt-ang
    else      *ang = UT_DEGREES(d1);    // rot-ang

    // *ang = UT_DEGREES(UT2D_angr_ptpt((Point2*)&((Line*)p1)->p1,
                                      // (Point2*)&((Line*)p1)->p2));
    goto L_exit;




  //-----------------------------------------------------------------
  // C [REV]                    // openingAngle
  } else if (aus_typ[0] == Typ_CI)     {
    p1 = DB_get_CI ((long)aus_tab[0]);
    *ang = UT_DEGREES(((Circ*)p1)->ango);
    goto L_exit;


  } else goto Par_err;




  //================================================================
  } else if(aus_anz == 2) {
  //================================================================


  //-----------------------------------------------------------------
  // P P [REV]                        // angle of line
  if((aus_typ[0] == Typ_PT)   &&
     (aus_typ[1] == Typ_PT))     {

    // (Point*)p1 = DB_get_PT ((long)aus_tab[0]);
    // (Point*)p2 = DB_get_PT ((long)aus_tab[1]);
    p1 = (void*)DB_get_PT ((long)aus_tab[0]);
    p2 = (void*)DB_get_PT ((long)aus_tab[1]);

    *ang = UT_DEGREES(UT2D_angr_ptpt((Point2*)p1,(Point2*)p2));
    goto L_exit;
  } 

  //-----------------------------------------------------------------
  // P L|D L|D [REV]                        // angle of line
  if(aus_typ[0] == Typ_LN) {
     lnp = DB_get_LN ((long)aus_tab[0]);
     UT3D_vc_ln (&v1, lnp);
  } else if(aus_typ[0] == Typ_VC) {
     v1 = DB_GetVector ((long)aus_tab[0]);
  } else goto Par_err;
                     // angle of line
  if(aus_typ[1] == Typ_LN) {
     lnp = DB_get_LN ((long)aus_tab[1]);
     UT3D_vc_ln (&v2, lnp);
  } else if(aus_typ[1] == Typ_VC) {
     v2 =DB_GetVector ((long)aus_tab[1]);
  } else goto Par_err;
  goto L_2vc;





  //================================================================
  }  // end (aus_anz == 2)
  //================================================================
  goto Par_err;



  //----------------------------------------------------------------
  L_2vc:
    *ang = UT_DEGREES(UT3D_angr_2vc__ (&v1, &v2));



  //----------------------------------------------------------------
  L_exit:
    // angle = ang
    if(*ang == UT_DB_LEER) goto Par_err;

    // apply CX
    if(iCx) *ang = fabs(360. - *ang);

    // apply REV
    // if(iRev) *ang = UT2D_angd_invert (ang);
    if(iRev) *ang = fabs(180. - *ang);

    APT_subTyp = Typ_Angle;

      // printf("ex-APT_decode_angd__ %f\n",*ang);

    return 0;


  Par_err:
  TX_Error("Angledefinition: ParameterError ****");
  goto Error;



  Error:
  return -1;


}


//====================================================================
  int APT_decode_var (double *d1,
                      int aus_anz, int aus_typ[], double aus_tab[]) {
//====================================================================

  int              i1, irc;
  // double           db1;
  Point            pt1, pt2;
  Vector           vc1, vc2;
  Line             ln1;
  Circ             ci1;
  void             *p1;


  // printf("APT_decode_var %d\n",aus_anz);
  // for(i1=0;i1<aus_anz;++i1) printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);


  irc = 0;


  //----------------------------------------------------------------
  // decode functions ..
  if(aus_typ[0] == Typ_cmdNCsub) {
    if((int)aus_tab[0] != T_ANG) goto L_not_imp;
    return APT_decode_angd__ (d1, aus_anz - 1, &aus_typ[1], &aus_tab[1]); 
  }




  //-----------------------------------------------------
  if(aus_anz == 1)  {
  //-----------------------------------------------------

    // V21 = V20                 (f.zB "P = P21 Z(V20)")
    if (aus_typ[0] == Typ_VAR)              {
      *d1 = DB_GetVar (aus_tab[0]);
      goto L_fertig;


    // V20 = X(P20)
    } else if((aus_typ[0] == Typ_Val)    ||
              (aus_typ[0] == Typ_XVal)   ||
              (aus_typ[0] == Typ_YVal)   ||
              (aus_typ[0] == Typ_ZVal))      {
      *d1 = aus_tab[0];
      goto L_fertig;


    // V = L          // Line: Laenge
    } else if(aus_typ[0] == Typ_LN)        {
      ln1 = DB_GetLine ((long)aus_tab[0]);
      *d1 = UT3D_len_2pt (&ln1.p1, &ln1.p2);
      goto L_fertig;


    // V = C              (Radius)
    } else if(aus_typ[0] == Typ_CI)        {
      ci1 = DB_GetCirc  ((long)aus_tab[0]);
      *d1 = UT3D_len_2pt (&ci1.p1, &ci1.pc);
      goto L_fertig;


    // V = D              // length of vector
    } else if(aus_typ[0] == Typ_VC)        {
      p1 = DB_get_VC ((long)aus_tab[0]);
      *d1 = UT3D_len_vc((Vector*)p1);
      goto L_fertig;



    // V = ANG(val)       (copy angle)
    } else if(aus_typ[0] == Typ_Angle)        {
      *d1 = aus_tab[0];
      goto L_fertig;



    } else goto L_not_imp;




  //-----------------------------------------------------------------
  } else if (aus_anz == 2)  {
  //-----------------------------------------------------


    //-----------------------------------------------------------------
    // V = P P                        // Abstand
    if        ((aus_typ[0] == Typ_PT)   &&
               (aus_typ[1] == Typ_PT))     {
 
      pt1 = DB_GetPoint ((long)aus_tab[0]);
      pt2 = DB_GetPoint ((long)aus_tab[1]);
      goto L_V_dispPP;
 

    //-----------------------------------------------------------------
    // V = P L                        // Normalabstand
    } else if ((aus_anz    == 2)        &&
               (aus_typ[0] == Typ_PT)   &&
               (aus_typ[1] == Typ_LN))     {

      pt1 = DB_GetPoint ((long)aus_tab[0]);
      ln1 = DB_GetLine ((long)aus_tab[1]);
      UT3D_pt_projptln (&pt2, NULL, NULL, &pt1, &ln1);
      L_V_dispPP:
      *d1 = UT3D_len_2pt (&pt1, &pt2);
      goto L_fertig;


    //-----------------------------------------------------------------
    } else goto L_not_imp;





  //-----------------------------------------------------------------
  } else if (aus_anz == 3)  {
  //-----------------------------------------------------


    //-----------------------------------------------------------------
    // V = P P D                      // distance in direction
    if        ((aus_typ[0] == Typ_PT)   &&
               (aus_typ[1] == Typ_PT)   &&
               (aus_typ[2] == Typ_VC))       {
 
      pt1 = DB_GetPoint ((long)aus_tab[0]);
      pt2 = DB_GetPoint ((long)aus_tab[1]);
      vc1 = DB_GetVector ((long)aus_tab[2]);

      
      UT3D_vc_2pt (&vc2, &pt1, &pt2);
      *d1 = UT3D_slen_projvcvc (&vc1, &vc2);
      goto L_fertig;


    //-----------------------------------------------------------------
    } else goto L_not_imp;



  //-----------------------------------------------------
  } else goto L_not_imp;



  //TX_Print("exit APT_decode_var %f\n",*d1);


  L_fertig:

  APT_subTyp = Typ_VAR;

  return irc;


  Error:
  return -1;


  L_not_imp:
    TX_Error(" APT_decode_var - unimplemented\n");
    goto Error;

}


//=============================================================================
  int APT_decode_conv_pol2 (ObjGX *cv_out,
                            int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
// 3D-Polygon -> 2D-Polygon     (1=3D-Polygon, 2=Refsys, 3=Tol)


  int       i1, pTabNr, pTabSiz, ptNr;
  long      icv;
  double    tol;
  Point     pt1, *pTabIn;
  ObjGX     *cv_in;

  Point2    *pTab;
  // ObjGX     cv2;
  Plane     pl1;
  // Mat_4x3   m1;


  // printf("APT_decode_conv_pol2\n");

  icv = aus_tab[1];

  cv_in = DB_GetCurv (icv);
  if(cv_in->typ != Typ_CVPOL) {
    TX_Error("nur mit 3D-Pol!");
    return -1;
  }
  ptNr   = cv_in->siz;
  pTabIn = cv_in->data;


    // printf("Refsys typ=%d %f\n",aus_typ[2],aus_tab[2]);
  if(aus_typ[2] != Typ_PLN) {
    TX_Error("Parameter 2 muss Plane sein");
    return -1;
  }
  i1=aus_tab[2]; 
  DB_GetRef (&pl1, i1);


  if(aus_typ[3] != Typ_Val) {
    TX_Error("Parameter 3 muss Toleranz sein");
    return -1;
  }
  tol = aus_tab[3];

  // printf("ptNr=%d tol=%f\n",ptNr,tol);


  pTab = (Point2*) memspc55;
  pTabSiz = sizeof(memspc55) / sizeof(Point2);


  //------------------------------------------------------
  // 3D-Poly -> 2D-Poly

  for(i1=0; i1<ptNr; ++i1) {
    // UT3D_pt_projptpl(&pt1, &pl1, &pTabIn[i1]);
    pt1 = pTabIn[i1];
    // pTab[ptNr] = pt1;
    pTab[i1] = UT2D_pt_pt3(&pt1);
    // printf(" pt %d = %f,%f\n",i1,pTab[i1].x,pTab[i1].y);
  }




  //------------------------------------------------------
  cv_out->typ   = Typ_CVPOL2;
  cv_out->form  = Typ_PT2;
  cv_out->siz   = ptNr;
  cv_out->data  = pTab;

/*
  for(i1=0; i1<ptNr; ++i1) {
    printf(" pt2 %d = %f,%f\n",i1,
             pTab[i1].x,pTab[i1].y);
       // cv_out->ptTab[i1].x, cv_out->ptTab[i1].y);
  }
*/

  return 0;

}


//=============================================================================
  int APT_decode_conv_pol (ObjGX *cv_out, Memspc *memSeg1,
                           int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
// BSP -> 3D-Polygon

static  CurvPoly plg1;

  int           i1, pTabNr, pTabSiz;
  long          icv;
  double        tol;
  Point         *pTab;
  Plane         pl1;
  // Mat_4x3       m1;
  ObjGX         *cv2;
  // CurvBSpl *cvBsp;


  // printf("APT_decode_conv_pol\n");

  icv = aus_tab[1];


  cv2 = DB_GetCurv (icv);



  //----------------------------------------
  // Polygon = project Polygon -> Plane
  if(cv2->typ == Typ_CVPOL) {
    // aus[2] muss Plane sein
    if(aus_typ[2] != Typ_PLN) {
      TX_Error("Polygon = Polygon Plane ??");
      return -1;
    }

    DB_GetRef (&pl1, (long)aus_tab[2]);
    i1 = UT3D_plg_projplgpln (&plg1, cv2->data, &pl1, memSeg1);
    if(i1 < 0) return i1;
    goto L_out;
  }



  //----------------------------------------
  // Polygon = B-Spline Toleranz
  if(cv2->typ != Typ_CVBSP) {
    TX_Error("nur mit B-Spline!");
    return -1;
  }


  tol = aus_tab[2];


  pTab = (void*) memspc51;
  pTabSiz = sizeof(memspc51) / sizeof(Point);
  pTabNr  = 0;


  bspl_pol_bsp (&pTabNr, pTab, cv2->data, pTabSiz, tol);

  // make plg aus pTab
  UT3D_plg_pta (&plg1, pTab, pTabNr, memSeg1);




  //----------------------------------------
  // Polygon plg1 fertig.
  L_out:
  cv_out->typ   = Typ_CVPOL;
  cv_out->form  = Typ_CVPOL;
  cv_out->siz   = 1;
  cv_out->data  = &plg1;

  return 0;


}


//=============================================================================
  int APT_decode_conv_cv (CurvBSpl *cv1, Memspc *tbuf1, Memspc *tbuf2,
                           int aus_anz, int aus_typ[], double aus_tab[]) {
//=============================================================================
// aus_[1] = Typ_CV
// iTyp: 0=Interpolierend 1=Controlpoints
// ACHTUNG: liefert Knotentabelle in memspc51


  int      i1, ii, deg, iTyp;
  long     icv;
  ObjGX    *cv2;
  CurvPoly  *plg1;
	Memspc   memSeg1;


/*
  printf("APT_decode_conv: %d\n",aus_anz);
  for(i1=0; i1<aus_anz; ++i1) {
    printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  }
*/




  // iTyp (last word CTRL): 0=Interpolierend 1=Controlpoints
  ii = aus_anz - 1;
  if(aus_typ[ii] == Typ_cmdNCsub)  {
    if((int)aus_tab[0] == T_CTRL) iTyp = 1;
    --ii;
  } else {
    iTyp = 0;
  }


  // Degree
  if(aus_typ[ii] == Typ_Val)  {
    deg = aus_tab[ii];
  } else {
    deg = 2;
  }




  //====================================================
  // ? BSP kopieren ?
  icv = aus_tab[1];


  // printf("APT_decode_conv_cv S-Ind=%d deg=%d\n",icv,deg);


  // das Polygon holen
  cv2 = DB_GetCurv (icv);
  if(cv2->typ != Typ_CVPOL) {
    TX_Error("nur mit Polygon!");
    return -1;
  }

  plg1 = cv2->data;




  // umwandeln in BSP;
  if(iTyp == 0) {   // iTyp: 0=Interpolierend
    // i1 = UCBS_cbsp_npt (cv1, plg1->ptNr, plg1->cpTab, deg, tbuf1, tbuf2);
    iTyp = 0; //0=Standard; 1=optimized;
    // optimized=genauer, aber f GordonSurfs nicht verwendbar).
    i1 = UCBS_BspCrvPts (cv1, tbuf1, plg1->ptNr, plg1->cpTab, deg, iTyp, tbuf2);

  } else {          // iTyp: 1=Controlpoints
    i1 = UT3D_cbsp_ptn (cv1, tbuf1, plg1->cpTab, plg1->ptNr, deg);
  }


  return i1;

}


//=============================================================================
  int APT_decode_rect2 (ObjGX *ocv, Memspc *tbuf1, Memspc *tbuf2,
                        int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
/*
S24=CCV Eckpunkt DX DY Radius        - verrundetes Rechteck

tbuf1 = hier werden die ObjGX der Ausgabeobjekte abgelegt; Anzahl=cv1->siz.
tbuf2 = hier werden die Ausgabeobjekte selbst abgelegt.
*/


  int        iOut;
  double     dx, dy, r1;
  Line       ln1;
  Circ       ci1;
  ObjGX      o1, *oxTab;
  CurvCCV    *ccvDat;


  iOut = 0;

  // get space for ccv
  ccvDat = UME_reserve (tbuf2, sizeof(CurvCCV));


  oxTab = tbuf1->next;


  dx = aus_tab[2];
  dy = aus_tab[3];
  r1 = aus_tab[4];
  printf("APT_decode_rect2 dx=%f dy=%f r1=%f\n",dx,dy,r1);



  //------------------------------------------------
  ln1.p1 = DB_GetPoint ((long)aus_tab[1]);
  ln1.p2 = ln1.p1;

  ln1.p2.x += dx;

  // DEB_dump_obj__(Typ_LN, &ln1, "ln u:\n");


  o1.typ  = Typ_LN;
  o1.form = Typ_LN;
  o1.data = UME_save (tbuf2, &ln1, sizeof(Line));

  
  oxTab[iOut] = o1;
  ++iOut;




  //------------------------------------------------
  ci1.p1   = ln1.p2;
  ci1.pc   = ci1.p1;
  ci1.pc.y += r1;
  ci1.p2   = ci1.pc;
  ci1.p2.x += r1;
  ci1.rad  = r1;
  ci1.vz   = WC_sur_act.vz;
  // DEB_dump_obj__(Typ_CI, &ci1, "ci ru:\n");


  o1.typ  = Typ_CI;
  o1.form = Typ_CI;
  o1.data = UME_save (tbuf2, &ci1, sizeof(Circ));


  oxTab[iOut] = o1;
  ++iOut;


  //------------------------------------------------
  ln1.p1   = ci1.p2;
  ln1.p2   = ln1.p1;
  ln1.p2.y += dy;



  o1.typ  = Typ_LN;
  o1.form = Typ_LN;
  o1.data = UME_save (tbuf2, &ln1, sizeof(Line));


  oxTab[iOut] = o1;
  ++iOut;




  //------------------------------------------------
  ln1.p1   = ln1.p2;
  ln1.p2.x -= dx;



  o1.typ  = Typ_LN;
  o1.form = Typ_LN;
  o1.data = UME_save (tbuf2, &ln1, sizeof(Line));


  oxTab[iOut] = o1;
  ++iOut;



  //------------------------------------------------
  // ccvDat->segNr = oNr;
  // ccvDat->cvtab = oTab;
  ccvDat->v0    = 0.;
  ccvDat->is0   = 0;
  ccvDat->v1    = 1.;
  ccvDat->is1   = iOut - 1;


  // add ccvDat as last record into oxTab
  o1.typ     = Typ_Ptr;
  o1.form    = Typ_CVTRM;
  o1.siz     = 1;
  o1.data    = ccvDat;
  oxTab[iOut] = o1;
  ++iOut;



  //------------------------------------------------
  ocv->typ   = Typ_CVTRM;
  ocv->form  = Typ_ObjGX;
  ocv->siz   = iOut;
  ocv->data  = oxTab;


  // create Obj CCV from contours
  // return UT3D_ccv_contour (ocv, ccvDat, oxTab, iOut);

  // DEB_dump_ox_0 (oxTab, iOut);

  return 0;

}

/* UNUSED
//=============================================================================
  int APT_decode_ccv (ObjGX *ocv, Memspc *tbuf1, Memspc *tbuf2,
                      int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
// 
// DO NOT USE; replaced by APT_decode_cvcomp
// S24=CCV P L C P        - Contour
// 
// tbuf1 = hier werden die ObjGX der Ausgabeobjekte abgelegt.
// tbuf2 = hier werden die Ausgabeobjekte selbst abgelegt.


  int     i1, i2, iOut, iIn, dreh, imod;
  long    l1;
  char    cbuf[32];
  ObjGX   *oxTab, oo1, oo2, oi;
  CurvCCV *ccvDat;
  // Memspc  oxData;



  // 0 typ=170 tab=19.000000
  // 1 typ=4 tab=22.000000



  //========== Testausg. aus_..
  // printf("APT_decode_ccv %d\n",aus_anz);
  // printf("          AP_mode__ %d\n",AP_mode__);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }
  //========== Testausg. aus_..




  // Par.1= "CCV"!
  if(aus_anz < 2) {
    TX_Error(" zuenig Parameter");
    return -1;
  }



  // verrundetes Rechteck
  if((aus_anz == 5)&&(aus_typ[2] == Typ_Val)) {
    return APT_decode_rect2 (ocv,tbuf1,tbuf2,aus_anz,aus_typ,aus_tab);
  }



  // // erster par. muss Punkt sein !
  // if(aus_typ[1] != Typ_PT) {
    // TX_Error(" erster Parameter muss Startpunkt sein");
    // return -1;
  // }


  // Test ob alle Obj existieren ..
  for(i1=1; i1<aus_anz; ++i1) {
    if(aus_typ[i1] == Typ_modif) continue;     // skip MOD()
    if(aus_typ[i1] == Typ_cmdNCsub) continue;  // skip CW ..
    l1 = aus_tab[i1];
    if(DB_QueryDef(aus_typ[i1],l1) < 0) {
      APED_oid_dbo__ (cbuf, aus_typ[i1],l1);
      TX_Error("CCV: %s (Obj %d) not defined",cbuf,i1);
      return -1;
    }
  }


  // get space for ccv
  ccvDat = UME_reserve (tbuf2, sizeof(CurvCCV));


  // init objTabSpace
  // oxTab = (void*)memspc55;
  oxTab = tbuf1->next;

  // init Dataspace
  // UME_init (&oxData, memspc101, sizeof(memspc101));




  iOut = 0;
  dreh = 0;

  // init Kontursucher
  AP_cont_nxt (NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0);

  iIn=1;

  //---------------------------------------------------
  L_next:
    // printf(" ::::::: next %d typ=%d tab=%f\n",iIn,aus_typ[iIn],aus_tab[iIn]);


    // skip CW/CCW und MOD()
    if(AP_cont_skip(&iIn, aus_anz, aus_typ, aus_tab) == 0) goto L_next;


    // get rotSense, MOD out of input
    // CW/CCW und MOD(#) aus daten holen (ein Element im voraus)
    AP_cont_mod (&dreh, &imod, iIn, aus_anz, aus_typ, aus_tab);



    oi.typ   = aus_typ[iIn];
    oi.form  = Typ_Index;
    oi.siz   = 1;
    i1 = aus_tab[iIn];
    oi.data  = PTR_INT(i1);
      // DEB_dump_obj__ (Typ_ObjGX, &oi, "oi:");


    // provide next input-obj oi, compute output-obj's oo1 and oo2
    i1 = AP_cont_nxt (&oo1, &i2, &oo2, &i2, tbuf2, &oi, 0, dreh, imod);
    if(i1 < 0) goto L_weiter;

    if(oo1.typ == Typ_Error) goto L_weiter;  // kein Ausg.Element 1
    // store output-obj oo1
    oxTab[iOut] = oo1;
    ++iOut;

    if(oo2.typ == Typ_Error) goto L_weiter;  // kein Ausg.Element 2
    // store output-obj oo1
    oxTab[iOut] = oo2;
    ++iOut;

  L_weiter:
  ++iIn;
  if(iIn < aus_anz) goto L_next;  // get next iput-obj
  //-------- ende for-loop ------------




  L_inp_fertig:

  // // wenn AP_mode__ ==go, Endelement fertigmachen;
  // // wenn AP_mode__ ==enter bei Last Obj == CI halbkreis machen
  // // if(AP_mode__ == AP_mode_go) {
  // // PROBLEM: diese Funktion muesste man ganz am Anfang beim Init rufen; 
  // // in AP_cont_nxt intern merken und duerfte im gesamten Verlauf
  // // keinen Kreis halbieren !
  // i1 = WC_get_obj_stat();
  // // printf("obj_stat=%d\n",i1);
  // if(i1 == 0) {
    // i1 = TYP_FuncEnd;
  // } else {
    // i1 = TYP_FuncInit;
  // }


  // CloseContourFunktion rufen
  oi.typ = TYP_FuncEnd; // i1;
  i1 = AP_cont_nxt (&oo1, &i2, &oo2, &i2, tbuf2, &oi, 0, dreh, imod);
  if(oo1.typ != Typ_Error) {
    // store output-obj oo1
    oxTab[iOut] = oo1;
    ++iOut;
  }


  //------------------------------------------------
  // make CCV from oxTab

  // create ccvDat-record
  ccvDat->v0    = 0.;
  ccvDat->is0   = 0;
  ccvDat->v1    = 1.;
  ccvDat->is1   = iOut - 1;


  // add ccvDat as last record into oxTab
  oo1.typ     = Typ_Ptr;
  oo1.form    = Typ_CVTRM;
  oo1.siz     = 1;
  oo1.data    = ccvDat;
  oxTab[iOut] = oo1;
  ++iOut;


  //------------------------------------------------
  // create CCV
  ocv->typ   = Typ_CVTRM;
  ocv->form  = Typ_ObjGX;
  ocv->siz   = iOut;
  ocv->data  = oxTab;


    // TESTONLY:
    DEB_dump_ox_0 (ocv, "ex decode_ccv:");
    printf("exiting decode_ccv: \n");



  return 0;

  // // create Obj CCV from contours
  // return UT3D_ccv_contour (ocv, cv1, oxTab, iOut);

}
*/


//=============================================================================
  int APT_decode_ltab (ObjGX *ocv, Memspc *spcObj,
                       int aus_anz, int aus_typ[], double aus_tab[]) {
//=============================================================================
// decode connention line
// S#=MSH U(P1 P2) [line [distance]]

// RetCod:   1=only first point ..

#define maxPtNr   10

  int    i1, i2, ii, grpNr, lNr;
  long   dbi;
  double d1, off;
  Point  *pt1, p1, p2, ptm, pTab[maxPtNr];
  Vector vch, vcv;
  Line   ln1;
  void   *oTab;


  // printf("APT_decode_ltab |%d|\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1)
  // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);


  if(aus_anz < 4) return 1;
  lNr = 0;


  // get group
  if(aus_typ[1] != Typ_Group) goto L_inpErr;
  grpNr = aus_tab[1];
  if(grpNr > maxPtNr) goto Err_2;


  // load pointers to points
  ii = 2;
  for(i1=0; i1 < grpNr; ++i1) {
    dbi = aus_tab[i1 + ii];
    UTO__dbo ((void**)&pt1, &i2, aus_typ[i1 + ii], dbi);
    pTab[i1] = *pt1;  // copy point
      // DEB_dump_obj__(Typ_PT, &pTab[i1], "  _pTab %d",i1);
  }



  // load Line (get ptm, horiz.vector vch & vertic.vector vcv).
  ii += grpNr;
  if((aus_anz > ii) && (aus_typ[ii] == Typ_LN)) {
    dbi = aus_tab[ii];
    ln1 = DB_GetLine (dbi);
      // DEB_dump_obj__(Typ_LN, &ln1, "  _ln1 ");
    ptm = ln1.p1;
    UT3D_vc_2pt (&vcv, &ln1.p1, &ln1.p2);
    // normal
    UT3D_vc_perp2vc (&vch, (Vector*)&WC_sur_act.vz, &vcv);
    ++ii;


  } else if (aus_typ[ii] == Typ_VC) {
    dbi = aus_tab[ii];
    vcv = DB_GetVector (dbi);
    UT3D_vc_perp2vc (&vch, (Vector*)&WC_sur_act.vz, &vcv);
    ++ii;


  } else {
    // defaults for point ptm & direction vcv
    // get ptm = midpoint of all points
    UT3D_pt_mid2pt (&ptm, &pTab[0], &pTab[1]);
    if(grpNr > 2) {
      for(i1=2; i1 < grpNr; ++i1) {
        UT3D_pt_mid2pt (&ptm, &ptm, &pTab[i1]);
      }
    }

    // get horiz.vector vch & vertic.vector vcv
    vch = UT3D_VECTOR_X;
    vcv = UT3D_VECTOR_Y;
  }
    // DEB_dump_obj__ (Typ_PT, &ptm, "  ptm:");
    // DEB_dump_obj__ (Typ_VC, &vcv, "  vcv:");
    // DEB_dump_obj__ (Typ_VC, &vch, "  vch:");



  // get offset -> off
  off = 0.;
  if(aus_anz > ii) {
    if(aus_typ[ii] != Typ_Val) goto L_inpErr;
    off = aus_tab[ii];

  } else {
    // no offset given: ptm = midpoint of first / last-point
    UT3D_pt_mid2pt (&ptm, &pTab[0], &pTab[grpNr - 1]);
  }

    // printf(" off=%f\n",off);


  // move ptm in dirction vch - distance off
  if(off != 0.)
  UT3D_pt_traptvclen (&ptm, &ptm, &vch, off);


  // adress
  oTab = UME_get_next (spcObj);


  // create all horiz. lines
  for(i1=0; i1 < grpNr; ++i1) {
    ln1.p1 = pTab[i1];
    UT3D_pt_projptptvc (&ln1.p2, &d1, NULL, &pTab[i1], &ptm, &vcv);
      // DEB_dump_obj__(Typ_LN, &ln1, "  hor.ln[%d]",i1);
    // save line -> spcObj
    UME_save (spcObj, &ln1, sizeof(Line));
    ++lNr;
  }


  // p1, p2 = find the extreme points of pTab along vector vcv
  // always same side !
  UT3D_ipt2_nptvc (&p1, &p2, 0, pTab, grpNr, &vcv);



  // project p1, p2 -> vertic line
  UT3D_pt_projptptvc (&ln1.p1, &d1, NULL, &p1, &ptm, &vcv);
  UT3D_pt_projptptvc (&ln1.p2, &d1, NULL, &p2, &ptm, &vcv);


  // create vertic. line
  UME_save (spcObj, &ln1, sizeof(Line));
  ++lNr;
  

  // create lineTable
  ocv->typ   = Typ_CVLNA;
  ocv->form  = Typ_LN;
  ocv->siz   = lNr;
  ocv->data  = oTab;

    // DEB_dump_ox_0 (ocv, " _decode_ltab:");

  return 0;



  L_inpErr:
    TX_Error("APT_decode_ltab: ParameterError ****");
    goto Error;

  Err_2:
    TX_Error("APT_decode_ltab: max 19 points ****");
    goto Error;


  Error:
  return -1;


}


//===================================================================
  int APT_decode_ccv__ (ObjGX *ocv, Memspc *tbuf1, Memspc *tbuf2,
                        int aus_anz, int aus_typ[], double aus_tab[]) {
//===================================================================
// was APT_decode_cvcomp was APT_decode_ccv
// RetCod:  0   Ok
//          1   single point; startpos ..
//         -1   Error


  int      irc, i1, iIn, isr, imod, ccaSiz, ccNr;
  long     l1;
  char     cbuf[32];
  ObjGX    oxi;
  CurvCCV  *cca;


  // // printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC \n");
  // printf("APT_decode_ccv__ %d\n",aus_anz);
  // for(i1=0;i1<aus_anz;++i1)
    // printf(" typ[%d]=%d val=%lf\n",i1,aus_typ[i1],aus_tab[i1]);
  // printf("  UT_TOL_pt=%lf UT_TOL_cv=%lf\n",UT_TOL_pt,UT_TOL_cv);


  // Par.1= "CCV"!
  if(aus_anz < 2) {
    TX_Error(" not enough parameters");
    return -1;
  }



  // Test if all obj's exist ..
  for(i1=1; i1<aus_anz; ++i1) {
    if(aus_typ[i1] == Typ_modif) continue;     // skip MOD()
    if(aus_typ[i1] == Typ_cmdNCsub) continue;  // skip CW ..
    l1 = aus_tab[i1];
    if(DB_QueryDef(aus_typ[i1],l1) < 0) {
      APED_oid_dbo__ (cbuf, aus_typ[i1],l1);
      TX_Error("CCV: %s (Obj %d) not defined",cbuf,i1);
      return -1;
    }
  }

  // only a point: temp-disp.
  if(aus_anz == 2) {
    if(aus_typ[1] == Typ_PT) {
      UI_disp_dbo ((int)aus_typ[1], (long)aus_tab[1], 1);
      return 1;
    }
  }


  // get memSpc for cca (CurvCCV-records) in tbuf1
  // max memspc needed:
  ccaSiz = UME_ck_free (tbuf1) / sizeof(CurvCCV);
    // printf(" ccaSiz=%d\n",ccaSiz);
  i1 = aus_anz + 4;  // minsiz
  if(ccaSiz > i1) ccaSiz = i1;
  cca = UME_get_start (tbuf1);


  iIn  = 1;

  // init
  CNTF_init__ (cca, ccaSiz, UT_TOL_cv, UT_TOL_pt);


  //----------------------------------------------------------------
  L_nxt_obj:
  // loop tru objects ..
      // printf("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
      // printf("decode_ccv__ ========== L_nxt_obj: iIn=%d\n",iIn);


    // get next obj with MOD and SR
    iIn = APT_decode_ccv_nxt (&oxi, &isr, &imod, iIn, aus_anz, aus_typ, aus_tab);
    if(iIn < 0) return -1;
    if(!iIn) goto L_close;

    // process next obj (add obj's to output)
    // irc = APT_decode_cvco_add (&ccNr, cca, &oxi, isr, imod);
    irc = CNTF_add__ (&oxi, isr, imod);
    if(irc < 0) return -1;

    goto L_nxt_obj;


  //----------------------------------------------------------------
  // after last obj: add pending obj's to output
  L_close:
  irc = CNTF_exit__ (&ccNr);        // finish pending obj's
  if(irc < 0) return -1;


  //------------------------------------------------
  // reserve memspce in tbuf1
  if(ccNr > 0) UME_reserve (tbuf1, sizeof(CurvCCV) * ccNr);

  // create CCV (top-obj)
  OGX_SET_OBJ (ocv, Typ_CVTRM, Typ_CVTRM, ccNr, cca);


    // TESTBLOCK
    // printf("ex-APT_decode_ccv__-ccNr=%d ------------------------\n",ccNr);
    // DEB_dump_nobj__ (Typ_CVTRM, ccNr, cca, " ex-APT_decode_ccv__ :");
    // DEB_dump_ox_s_ (ocv, "ex-APT_decode_ccv__");
    // DEB_dump_ox_0 (ocv, "ex-APT_decode_ccv__");
    // if(ccNr == 14)
    // return MSG_ERR__ (ERR_TEST, "ex-APT_decode_ccv__");
    // {   Point *pp1;
    // for(i1=0; i1<ccNr; ++i1) {
      // DEB_dump_obj__ (Typ_CVTRM, &cca[i1], "[%d]",i1);
      // l1 = cca[i1].ip0;
      // if(l1){ 
        // pp1 = DB_get_PT (l1);
        // DEB_dump_obj__ (Typ_PT, pp1, "   P[%ld]",l1);
      // } 
      // l1 = cca[i1].ip1;
      // if(l1){
        // pp1 = DB_get_PT (l1);
        // DEB_dump_obj__ (Typ_PT, pp1, "   P[%ld]",l1);
    // }}}
    // fflush (stdout);exit(-1);
    // END TESTBLOCK


  return 0;

}


//================================================================
  int APT_decode_ccv_nxt (ObjGX *oxi, int *isr,  int *imod,
           int iIn, int aus_anz, int *aus_typ, double *aus_tab) {
//================================================================
// get next obj with MOD and SR
/// Output:
///   isr       1=CCW; -1=CW; 0=undefined
///   imod      0=undefined, else MOD#
///   retCod    -1=Error; 0=endOfLine; else next unused aus_typ/aus_tab


  int     i1, typ, iOut;
  long    dbi;



  // printf("APT_decode_ccv_nxt %d %d\n",iIn,aus_anz);

  if(iIn >= aus_anz) return 0; // 0=endOfLine;


  *isr = 0;
  *imod = 0;
  iOut = iIn;


  // first obj must be a geom.obj (DB-obj)
  typ = aus_typ[iIn];
  dbi = aus_tab[iIn];
    // printf(" ccv_nxt-typ=%d dbi=%ld\n",typ,dbi);
  OGX_SET_INDEX (oxi, typ, dbi);
    // DEB_dump_obj__ (Typ_ObjGX, oxi, "ccv_nxt-oxi");
  goto L_nxt_inp;



  //----------------------------------------------------------------
  L_noAmoi:
  // check for REV
  if(aus_typ[iOut] != Typ_cmdNCsub) goto L_mod;

  if(aus_tab[iOut] == T_REV) {
    *isr = 1;
  } else {
    // ERROR
    i1 = aus_tab[iOut];
    TX_Print ("**** APT_decode_ccv_nxt I_NCsub %d",i1);
    printf("**** APT_decode_ccv_nxt I_NCsub %d\n",i1);

  }
  goto L_nxt_inp;


  L_mod:
  // check for MOD
  if(aus_typ[iOut] != Typ_modif) goto L_exit;

  *imod = aus_tab[iOut];


  //----------------------------------------------------------------
  L_nxt_inp:
  iOut += 1;
  if(iOut < aus_anz) goto L_noAmoi;



   //----------------------------------------------------------------
  L_exit:
    // TESTBLOCK
    // printf("ex-APT_decode_ccv_nxt retCod=%d sr=%d mod=%d\n",iOut,*isr,*imod);
    // DEB_dump_obj__ (Typ_ObjGX, oxi, "ex-ccv_nxt-oxi");
    // END TESTBLOCK

  return iOut;

}


//=============================================================================
  int APT_decode_ccv2 (ObjGX *cv1,int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
/*
S24=CCV2,S23,0.2        - Circ/Line from 2D-Polygon, tol
*/

  int       i1, ptNr, oTabSiz, oTabNr;
  long      icv;
  // Point2Tab *cv_in;
  Point2    *pTabIn;
  ObjGX     *cv2;
  ObjG2     *oTab;



  // printf("APT_decode_ccv2: %d\n",aus_anz);


  icv = aus_tab[1];


  cv2 = DB_GetCurv (icv);
  if(cv2->typ != Typ_CVPOL2) {
    TX_Error("nur mit 2D-Polygon!");
    return -1;
  }
  ptNr   = cv2->siz;
  pTabIn = cv2->data;



  if(aus_typ[2] != Typ_Val) {
    TX_Error("Parameter 2 muss Toleranz sein");
    return -1;
  }

  // printf("ptNr=%d tol=%f\n",ptNr,aus_tab[2]);


  
  oTab = (ObjG2*) memspc51;
  oTabSiz = sizeof(memspc51) / sizeof(ObjG2);


  // UT_TOL_cv u UT_TOL_ln setzen und nach cvApp_app
  // wieder zuruecksetzen !!!


  // Polygon pTabIn in Kreisbogen/Linien (oTab) umwandeln
  oTabNr  = oTabSiz;
  cvApp_app (oTab, &oTabNr, pTabIn, ptNr);
  // liefert oTabNr ObjG2-Objekte in oTab.

  // liefert Typ_CI und Typ_LN statt Typ_CI2 und Typ_LN2.
  for(i1=0; i1<oTabNr; ++i1) {
    if(oTab[i1].typ == Typ_LN) oTab[i1].typ = Typ_LN2;
    if(oTab[i1].typ == Typ_CI) oTab[i1].typ = Typ_CI2;
  }


  cv1->typ   = Typ_CVTRM;  // Typ_CVTRM2;
  cv1->form  = Typ_CVTRM;  // Typ_ObjG2;
  cv1->siz   = oTabNr;
  cv1->data  = memspc51;



  return 0;

}


//=============================================================================
  int APT_decode_pt2bsp (CurvBSpl *cv1, Memspc *tbuf1, Memspc *tbuf2,
                         int aus_anz, int *aus_typ, double *aus_tab){
//=============================================================================
// ACHTUNG: verwendet memspc51 f. kvTab.
// iTyp: 0=Interpolierend 1=Controlpoints
// tbuf1=memspc201, tbuf2=memspc501


  int      i1, ptNr, ii, deg, iTyp, pTabSiz;
  long     l1;
  Point    *pTab;



  // printf("APT_decode_pt2bsp: %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }



  if(aus_anz < 2) {
    TX_Error(" zuenig Parameter");
    return -1;
  }


  // iTyp (last word CTRL): 0=Interpolierend 1=Controlpoints
  ii = aus_anz - 1;
  if(aus_typ[ii] == Typ_cmdNCsub)  {
    if((int)aus_tab[ii] == T_CTRL) iTyp = 1;
    --ii;
  } else {
    iTyp = 0;
  }


  // Degree
  if(aus_typ[ii] == Typ_Val)  {
    deg = aus_tab[ii];
    --ii;
  } else {
    deg = 2;
  }


  // Alle Punkte in eine Tabelle
  ptNr = 0;
  pTab = (void*) tbuf1->next;
  pTabSiz = UME_ck_free (tbuf1) / sizeof(Point);

  if(pTabSiz < ii) {
    printf(" ii=%d pTabSiz=%d\n",ii,pTabSiz);
    TX_Error("APT_decode_pt2bsp EOM");
    return -1;
  }

  for(i1=1; i1<=ii; ++i1) {
    if(aus_typ[i1] != Typ_PT) {
      TX_Error("APT_decode_pt2bsp E001");
      return -1;
    }
    pTab[ptNr] = DB_GetPoint ((long)aus_tab[i1]);
    ++ptNr;
  }

  // fixate space in tbuf1
  UME_add (tbuf1, ptNr * sizeof(Point));

  if(ptNr < 3) deg = 1;

  if(ptNr < 2) {
    l1 = -1;
    // GL_DrawPoint (&l1, Typ_Att_hili1, pTab);
    // GR_CrePoint (&l1, 0, pTab);
    DL_temp_ind = GR_TMP_I0;
    GR_temp_symB (pTab, SYM_CIR_S, ATT_COL_RED);
    return 1;  // nur 1. Punkt anzeigen
  }


  // umwandeln in BSP;
  if(iTyp == 0) {   // iTyp: 0=Interpolierend
    // i1 = UCBS_cbsp_npt (cv1, ptNr, pTab, deg, tbuf1, tbuf2);
    iTyp = 0; //0=Standard; 1=optimized;
    // optimized=genauer, aber f GordonSurfs nicht verwendbar).
    i1 = UCBS_BspCrvPts (cv1, tbuf1, ptNr, pTab, deg, iTyp, tbuf2);

  } else {          // iTyp: 1=Controlpoints
    i1 = UT3D_cbsp_ptn (cv1, tbuf1, pTab, ptNr, deg);
  }

  cv1->clo = -1;

  return i1;

}


//================================================================
int APT_BLEND__  (ObjGX *oxo,  
                  int aus_anz, int aus_typ[], double aus_tab[],
                  Memspc *tSpc1) {
//================================================================

  int   ii;


  // printf("---------------------------------------------- \n");
  printf("APT_BLEND__: %d\n",aus_anz);
  for(ii=0; ii<aus_anz; ++ii) {
    printf(" %d typ=%d tab=%f\n",ii,aus_typ[ii],aus_tab[ii]);
  }


  return 0;

}


//=============================================================================
  int APT_decode_bsp1 (ObjGX *ox1, Memspc *tbuf1, Memspc *tbuf2,
                      int aus_anz, int aus_typ[], double aus_tab[]) {
//=============================================================================
// B-Spline convert & join
// ox1=memspc101, tbuf1=memspc201, tbuf2=memspc501
// <objects> smoothVal        eg. BSP1 L22 L21 VAL(0.25)
// input[0] = 170, 67  (BSP1)
// RetCod:  0=OK; -1=Err; 1=not enough inputObjs


  int       irc, ii, o1typ, o2typ;
  long      dbi;
  double    smv;
  void      *o1dat, *o2dat;
  ObjGX     oxh;
  CurvBSpl  cv1, cv2;
  static CurvBSpl  cv3;


  // printf("---------------------------------------------- \n");
  // printf("APT_decode_bsp1: %d\n",aus_anz);
  // for(ii=0; ii<aus_anz; ++ii) {
    // printf(" %d typ=%d tab=%f\n",ii,aus_typ[ii],aus_tab[ii]);
  // }


  // remove smoothValue if exists (last value)
  if(aus_typ[aus_anz-1] == Typ_Val) {
    --aus_anz;
    smv = aus_tab[aus_anz];
  } else {
    smv = 0.;
  }
    // printf(" smv=%f\n",smv);



  // 1=not enough inputObjs
  if(aus_anz < 2) return 1;


  // init
  ii = 1;


  // convert obj1 into bspl cv1
  dbi = aus_tab[ii];
  OGX_SET_INDEX (&oxh, aus_typ[ii], dbi);
  irc = UT3D_cbsp_ox (&cv1, &oxh, smv, tbuf1, tbuf2);
  if (irc < 0) return -1;

  ++ii;

  if(ii >= aus_anz) {
    // only one (converted) obj
    cv3 = cv1;
    goto L_done;
  }


  L_nxt_obj:
  // convert obj2 into bspl cv2
  dbi = aus_tab[ii];
  OGX_SET_INDEX (&oxh, aus_typ[ii], dbi);
  irc = UT3D_cbsp_ox (&cv2, &oxh, smv, tbuf1, tbuf2);
  if (irc < 0) return -1;


  // join 2 bspl-curves
  // TODO: soll smv nur auf die cv2 anwenden ..
  if(smv < 0.) smv = 0.;
  irc = UT3D_cbsp_2cbsp (&cv3, tbuf1, smv, &cv1, &cv2, tbuf2);
  if (irc < 0) return -1;



  ++ii;
  if(ii < aus_anz) {
    cv1 = cv3;
    goto L_nxt_obj;
  }

  L_done:
  ox1->typ    = Typ_CVBSP;
  ox1->form   = Typ_CVBSP;
  ox1->siz    = 1;
  ox1->data  = &cv3;


  // DEB_dump_obj__ (Typ_CVBSP, &cv3, "ex APT_decode_bsp1:");



  return 0;

}


//=============================================================================
  int APT_decode_bsp_ (ObjGX *ox1, Memspc *tbuf1, Memspc *tbuf2,
                      int aus_anz, int aus_typ[], double aus_tab[]) {
//=============================================================================
// B-Spline (Sx=BSP .. oder S=TRA S..
// ACHTUNG: CurvBSpl: cv1->polTab = memspc55
// iTyp: 0=Interpolierend 1=Controlpoints
// ox1=memspc101, tbuf1=memspc201, tbuf2=memspc501
// RetCod:  0=OK; -1=Err; 1=not enough inputObjs


  int      irc, i1, i2, i3, fTabNr, fTabSiz;
  long     dbi, dli;
  double   *fTab;
  // CurvBSpl cv2, cv3;
  ObjGX    oxh;

  static CurvBSpl cv1;


  // printf("---------------------------------------------- \n");
  // printf("APT_decode_bsp_: %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }



  // values only:
  if(aus_typ[1] == Typ_Val) goto L_valTab;  // value = direct



    // check if inputObject is a polygon: convert ..
  if(aus_typ[1] == Typ_CV) {
      dbi = aus_tab[1];
      DB_get_CV (&i1, dbi);
      if(i1 == Typ_CVPOL) {
        // B-Spline-Curve from Polygon
        i1 = APT_decode_conv_cv (&cv1, tbuf1, tbuf2, aus_anz, aus_typ, aus_tab);
        if(i1 != 0) return i1;
        goto L_cv_ok;
      }
  }



  if(aus_anz < 3) {     // only 1 inputObject

    // Permanent-mode: convert element to CvBsp
    if(APT_obj_stat == 0) {
      OGX_SET_INDEX (&oxh, aus_typ[1], (long)aus_tab[1]);
      irc = UT3D_cbsp_ox (&cv1, &oxh, 0., tbuf1, tbuf2);
      if (irc < 0) return -1;
      goto L_cv_ok;


    // Temp.Mode: hilite it, nothing else ..
    } else {
      UI_disp_dbo ((int)aus_typ[1], (long)aus_tab[1], 1);
        // printf(" after GR_tDyn_dbo\n");
      return 1;
    }
  }



  // there are more than 1 inputObjects;
  // check for points
  if((aus_typ[1] == Typ_PT)&&(aus_typ[2] == Typ_PT)) {
    // B-Spline-Curve from Points
    i1 = APT_decode_pt2bsp (&cv1, tbuf1, tbuf2, aus_anz, aus_typ, aus_tab);
    if(i1 != 0) return i1;
    goto L_cv_ok;
  }


  // error;
  goto Par_err;

/*  now APT_decode_bsp1 - 2011-08-08
  // check if smoothFactor is given (last obj)
  // d1 = smooth factor for curve connection
  if(aus_typ[aus_anz-1] == Typ_Val) {
    --aus_anz;
    d1 = aus_tab[aus_anz];
  } else {
    d1 = 0.;
  }

  // convert elements to CvBsp and join elements
  // convert element1 to CvBsp cv2
  OGX_SET_INDEX (&oxh, aus_typ[1], (long)aus_tab[1]);
  irc = UT3D_cbsp_ox (&cv2, &oxh, tbuf1, tbuf2);
  if (irc < 0) return -1;

  // convert element2 to CvBsp cv3
  i1 = 2;
  L_add_cv:
  OGX_SET_INDEX (&oxh, aus_typ[i1], (long)aus_tab[i1]);
  irc = UT3D_cbsp_ox (&cv3, &oxh, tbuf1, tbuf2);
  if (irc < 0) return -1;

  // join elements (cv1 = cv2 + cv3)
  irc = UT3D_cbsp_2cbsp (&cv1, tbuf1, d1, &cv2, &cv3, tbuf2);
  if (irc < 0) return -1;


  // more than 2 obj's to join: cv2 = cv1; add new curve
    ++i1;
  if(i1 < aus_anz) {
    cv2 = cv1;
    goto L_add_cv;
  }
  goto L_cv_ok;
*/



  //================================================================
  // B-Spline-Curve from direct values ..
  L_valTab:
  // alle Werte fuer die struct CurvBSpl stehen direkt in der Tabelle !


  // alle variablen in eine tabelle fTab kopieren
  fTab = (void*) tbuf1->next;      
  fTabSiz = UME_ck_free(tbuf1) / sizeof(double);
  fTabNr  = 0;
    // printf(" fTabSiz=%d aus_anz=%d\n",fTabSiz,aus_anz);


  // copy coords, values -> fTab
  i1 = APT_decode_ValTab (fTab, &fTabNr, fTabSiz, aus_anz, aus_typ, aus_tab);
  if(i1 < 0) return i1;

  // printf(" ptNr=%f deg=%f\n",fTab[0],fTab[1]);



  // fill       iTyp: 1=Controlpoints
  cv1.ptNr  = fTab[0];
  cv1.deg   = fTab[1];

  i1 = 2;
  cv1.cpTab = (Point*)&fTab[i1];
  i1 += cv1.ptNr * 3;
  cv1.kvTab = &fTab[i1];
  i1 += cv1.ptNr + cv1.deg + 1;
  cv1.v0    = fTab[i1];
  ++i1;
  cv1.v1    = fTab[i1];
  cv1.clo   = -1;




  //----------------------------------------------------
  L_cv_ok:
  ox1->typ    = Typ_CVBSP;
  ox1->form   = Typ_CVBSP;
  ox1->siz    = 1;
  ox1->data  = &cv1;


  // save max. degree in temp-mode
  if(APT_obj_stat != 0) {
    APT_set_modMax (UT3D_bsp_ck_maxDeg(&cv1));
  }


  // DEB_dump_obj__ (Typ_CVBSP, &cv1, "decode_bsp");


  return 0;



  //================= Fehler =============================
  Par_err:
  TX_Error("Curvedefinition: ParameterError ****");
  // goto Error;


  Error:
  return -1;


}


//=============================================================================
  int APT_decode_rbsp (ObjGX *ox1,int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
// Rat.B-Spline (Sx=RBSP .. oder S=TRA S..
// ACHTUNG: CurvRBSpl: cv1->polTab = memspc55
// ACHTUNG: verwendet memspc51 f. cv1.kvTab.


  int    i1, i2, i3, fTabNr, fTabSiz;
  double *fTab;

  static CurvRBSpl cv1;


/*
  printf("APT_decode_rbsp: %d\n",aus_anz);
  for(i1=0; i1<aus_anz; ++i1) {
    printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  }
*/



  // B-Sp <- other type of curve (convert)
  if(aus_typ[1] == Typ_CV) {
    // i1 = APT_decode_conv_cv (&cv1,aus_anz,aus_typ,aus_tab);
    TX_Error("APT_decode_rbsp E001");
    i1 = -1;
    if(i1 != 0) return i1;
    goto L_cv_ok;

  } else if(aus_typ[1] == Typ_PT) {
    // i1 = APT_decode_pt2bsp (&cv1,aus_anz,aus_typ,aus_tab);
    TX_Error("APT_decode_rbsp E002");
    i1 = -1;
    if(i1 != 0) return i1;
    goto L_cv_ok;
  }





  // zuerst alle variablen in eine tabelle fTab kopieren
  fTab = (void*) memspc102;
  fTabSiz = sizeof(memspc102) / sizeof(double);
  fTabNr  = 0;
  // printf(" fTabSiz=%d aus_anz=%d\n",fTabSiz,aus_anz);


  i1 = APT_decode_ValTab (fTab, &fTabNr, fTabSiz, aus_anz, aus_typ, aus_tab);
  if(i1 < 0) return i1;

  // printf(" ptNr=%f deg=%f\n",fTab[0],fTab[1]);




  // fill
  cv1.ptNr  = fTab[0];
  cv1.deg   = fTab[1];

  i1 = 2;
  cv1.cpTab = (Point*)&fTab[i1];

  // nr of vals for cpTab
  i1 += cv1.ptNr * 3;

  cv1.kvTab = &fTab[i1];

  // nr of vals for kvTab
  i1 += cv1.ptNr + cv1.deg + 1;

  cv1.wTab = &fTab[i1];

  // nr of vals for eights
  i1 += cv1.ptNr;


  cv1.v0    = fTab[i1];
  ++i1;
  cv1.v1    = fTab[i1];


  //----------------------------------------------------
  L_cv_ok:
  ox1->typ    = Typ_CVRBSP;
  ox1->form   = Typ_CVRBSP;
  ox1->siz    = 1;
  ox1->data  = &cv1;


  // DEB_dump_obj__ (Typ_CVBSP, &cv1, "ex APT_decode_rbsp");


  return 0;



  //================= Fehler =============================
  Par_err:
  TX_Error("Curvedefinition: ParameterError ****");
  // goto Error;


  Error:
  return -1;


}


//=============================================================================
  int APT_decode_ValTab (double *fTab, int *fTabNr, int fTabSiz,
                         int aus_anz, int aus_typ[], double aus_tab[]) {
//=============================================================================
// copy Coords from Points in aus_typ/tab into fTab (3 doubles per point)
//   mixed with values !!!
// Output:
//   fTab    Coords (x,y,z,x,y,z ..)
//   fTabNr  nr of doubles in fTab
// ACHTUNG: CurvPol3: cv1->polTab = memspc55


  int    i1, i2, iTabNr;
  char   *p1;
  Point  *pt1;


  // printf("APT_decode_ValTab: %d\n",aus_anz);

  iTabNr = *fTabNr;


  // aus_typ[0] = 170 = Curvetyp.
  for(i1=1; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d val=%f\n",i1,aus_typ[i1],aus_tab[i1]);

    if(aus_typ[i1] == Typ_Val) {
      fTab[iTabNr] = aus_tab[i1];
      ++iTabNr;
      if(iTabNr > fTabSiz) goto EOM_Error;


    } else if(aus_typ[i1] == Typ_PT) {
      pt1 = DB_get_PT ((long)aus_tab[i1]);
      if(iTabNr+4 > fTabSiz) goto EOM_Error;
      fTab[iTabNr] = pt1->x;   ++iTabNr;
      fTab[iTabNr] = pt1->y;   ++iTabNr;
      fTab[iTabNr] = pt1->z;   ++iTabNr;
      // printf("point %d %f %f %f\n",iTabNr,pt1.x,pt1.y,pt1.z);
 

    } else if(aus_typ[i1] == Typ_StrDel) {    // Typ_StrDel "," (fortsetzen)
      break;


    } else {
      TX_Error("APT_decode_ValTab: %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
      return -1;
    }
  }


/*
  // alle Folgezeilen noch holen ...
  // Es sind sicher noch ein paar in Fortsetzungszeilen ..
  // - wenn die Zeile ein "," am Ende hat.
  while (aus_typ[aus_anz-1] == Typ_StrDel) {
    // printf(" Fortsetzungszeile holen\n");

    ED_GetNxtLin (&i2, APT_spc1);
    // printf(" nxt Ln |%s|\n",APT_spc1);
    // erster Char muss ein "&" sein
    if(APT_spc1[0] != '&') {
      TX_Error("Fortsetzungszeile fehlt");
      goto Error;
    }


    p1 = &APT_spc1[1];  // ohne "&"
    aus_anz = APT_decode_ausdr (aus_typ, aus_tab, &p1);

    for(i1=0; i1<aus_anz; ++i1) {
      // printf(" %d typ=%d\n",i1,aus_typ[i1]);
      if(aus_typ[i1] == Typ_Val) {
        fTab[iTabNr] = aus_tab[i1];
        // printf("val %d %f\n",iTabNr,fTab[iTabNr]);
        ++iTabNr;
        if(iTabNr > fTabSiz) return -1;

      } else if(aus_typ[i1] == Typ_PT) {
        pt1 = DB_GetPoint ((long)aus_tab[i1]);
        if(iTabNr+4 > fTabSiz) return -1;
        fTab[iTabNr] = pt1.x;   ++iTabNr;
        fTab[iTabNr] = pt1.y;   ++iTabNr;
        fTab[iTabNr] = pt1.z;   ++iTabNr;
        // printf("point %d %f %f %f\n",iTabNr,pt1.x,pt1.y,pt1.z);
      }
    }
  }
*/




  // printf("ex APT_decode_ValTab %d\n",iTabNr);
  // for(i1=0; i1<iTabNr; ++i1) printf(" [%d] %f\n",i1,fTab[i1]);




  *fTabNr = iTabNr;

  return 0;

  //--------------------------------------------------
  EOM_Error:
  TX_Error("APT_decode_ValTab EOM - E001");
  return -1;

}


//=============================================================================
  int APT_decode_psp3(ObjGX *cv_out,int aus_anz,int aus_typ[],double aus_tab[]){
//=============================================================================
// polynom. Spline
// ACHTUNG: CurvPol3: cv1->polTab = memspc55


  int    i1, i2, fTabNr, fTabSiz, polNr;
  char   *p1;
  double *fTab;
  polynom_d3  *polTab;
  // CurvPol3 *cv1;


  // printf("APT_decode_psp3: %d\n",aus_anz);



  // zuerst alle variablen in eine tabelle fTab kopieren
  fTab = (void*) memspc51;
  fTabSiz = sizeof(memspc51) / sizeof(double);
  fTabNr  = 0;


  i1 = APT_decode_ValTab (fTab, &fTabNr, fTabSiz, aus_anz, aus_typ, aus_tab);
  if(i1 < 0) return i1;



  // printf(" fTabNr=%d\n",fTabNr);
  if(fTabNr%13 != 0) {
    TX_Error("Data Error polynom.Curve");
    goto Error;
  }




  // fill 
  polNr = fTabNr / 13;
  polTab = (void*) memspc55;
  i2 = 0;
  for(i1=0; i1<polNr; ++i1) {
    
    polTab[i1].u = fTab[i2]; ++i2;

    polTab[i1].x.a = fTab[i2]; ++i2;
    polTab[i1].y.a = fTab[i2]; ++i2;
    polTab[i1].z.a = fTab[i2]; ++i2;

    polTab[i1].x.b = fTab[i2]; ++i2;
    polTab[i1].y.b = fTab[i2]; ++i2;
    polTab[i1].z.b = fTab[i2]; ++i2;

    polTab[i1].x.c = fTab[i2]; ++i2;
    polTab[i1].y.c = fTab[i2]; ++i2;
    polTab[i1].z.c = fTab[i2]; ++i2;

    polTab[i1].x.d = fTab[i2]; ++i2;
    polTab[i1].y.d = fTab[i2]; ++i2;
    polTab[i1].z.d = fTab[i2]; ++i2;

    //printf(" seg %d u=%f %f  %f  %f\n",i1,polTab[i1].u,
    //       polTab[i1].x.a,polTab[i1].y.a,polTab[i1].z.a);


  }


  cv_out->typ   = Typ_CVPSP3;
  cv_out->form  = Typ_CVPSP3;         // Typ_polynom_d3;
  cv_out->siz   = polNr;
  cv_out->data  = (void*)polTab;




  return 0;



  //--------------------------------------------------
  Error:
  return -1;
}


//=============================================================================
  int APT_decode_rec (ObjGX *cv_out, Memspc *memSeg1,
                      int aus_anz, int aus_typ[], double aus_tab[]){
//=============================================================================
// Rectangle
// war APT_decode_rect1


static CurvPoly plg1;

  int      i1;
  Point    *pt1, *pTab;
  Vector   *vcx, *vcy;



  // printf("APT_decode_rec %d typ=%f\n",aus_anz,aus_tab[0]);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  // S=REC Point vector vector
  if((aus_anz    == 4)         &&
             (aus_typ[1] == Typ_PT)    &&
             (aus_typ[2] == Typ_VC)    &&
             (aus_typ[3] == Typ_VC))     {

    pt1 = DB_get_PT ((long)aus_tab[1]);
    vcx = DB_get_VC ((long)aus_tab[2]);
    vcy = DB_get_VC ((long)aus_tab[3]);


  } else {
    TX_Error("Definition nicht implementiert");
    return -1;
  }



  // den Platz im memSeg1 reservieren
  pTab = memSeg1->next;
  i1 = UME_add (memSeg1, sizeof(Point) * 5);
  if(i1 < 0) {
    TX_Error("APT_decode_rec EOM");
    return -1;
  }

    // DEB_dump_obj__ (Typ_VC, vcx, "vcx");
    // DEB_dump_obj__ (Typ_VC, vcy, "vcy");


  pTab[0] = *pt1;
  UT3D_pt_traptvc (&pTab[1], pt1, vcx);
  UT3D_pt_traptvc (&pTab[2], &pTab[1], vcy);
  UT3D_pt_traptvc (&pTab[3], pt1, vcy);
  pTab[4] = *pt1;



  // CurvPolygon erstellen
  UT3D_plg_pta (&plg1, pTab, 5, memSeg1);

  cv_out->typ   = Typ_CVPOL;
  cv_out->form  = Typ_CVPOL;
  cv_out->siz   = 1;
  cv_out->data  = &plg1;

    // DEB_dump_obj__ (Typ_CVPOL, &plg1, "ex decode_rec");


  return 0;

}


//=============================================================================
  int APT_decode_clot (ObjGX *cv_out,
                      int aus_anz,int aus_typ[],double aus_tab[]){
//=============================================================================
// clothoid;

static CurvClot cl;

  int      i1;
  double   d1, d2, a1;
  Point    *p1;
  Vector   vc1, vcz;


  // printf("APT_decode_clot: %d typ=%f\n",aus_anz,aus_tab[0]);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  // <Startpunkt>
  if(aus_typ[1] != Typ_PT) goto L_err1;
  p1 = DB_get_PT ((long)aus_tab[1]);


  //----------------------------------------------------------------
  // Startvektor|StartAngle   .stv
  if(aus_typ[2] == Typ_VC) {
    vc1 = DB_GetVector ((long)aus_tab[2]);

  } else if(aus_typ[2] == Typ_Angle) {
    // ACHTUNG: gilt nur fuer aktives RefSys & DZ ! 
    UT3D_vc_angr (&vc1, UT_RADIANS(aus_tab[2]));

  } else goto L_err1;


  //----------------------------------------------------------------
  // <Endwinkel/Differenzwinkel>
  if(aus_typ[3] == Typ_Angle) {
    a1 = UT_RADIANS(aus_tab[3]);

  } else if(aus_typ[3] == Typ_VC) {
    vcz = DB_GetVector ((long)aus_tab[3]);
    UT3D_vc_normalize (&vc1, &vc1);
    UT3D_vc_normalize (&vcz, &vcz);
      // DEB_dump_obj__ (Typ_VC, &vc1, "vc1:\n");
      // DEB_dump_obj__ (Typ_VC, &vcz, "vcz:\n");
    a1 = -ACOS(UT3D_skp_2vc(&vcz, &vc1));
    printf(" a1=%f\n",a1);

  } else goto L_err1;


  //----------------------------------------------------------------
  // <StartRadius> .cs
  if(aus_typ[4] == Typ_Val) {
    if(aus_tab[4] == 0.) d1 = 0.;
    else d1 = 1. / aus_tab[4];
  } else goto L_err1;


  // <EndRadius> .ce
  if(aus_typ[5] == Typ_Val) {
    if(aus_tab[5] == 0.) d2 = 0.;
    else d2 = 1. / aus_tab[5];
  } else goto L_err1;


  // [<Normalvektor>]  .plv
  if(aus_anz > 6) {
    if(aus_typ[6] != Typ_VC) goto L_err1;
    vcz = DB_GetVector ((long)aus_tab[6]);
  } else {
    vcz = WC_sur_act.vz;
  }


  // load cloto-struct cl
  UCV_CltCrvCurvWind (&cl, d1, d2, a1, p1, &vc1, &vcz);


  L_fertig:
  cv_out->typ   = Typ_CVCLOT;
  cv_out->form  = Typ_CVCLOT;
  cv_out->siz   = 1;
  cv_out->data  = (void*)&cl;

  return 0;


  L_err1:
    TX_Error("definition of clothoid not implemented\n");
    return -1;
}


//=============================================================================
  int APT_decode_ell (ObjGX *cv_out,
                      int aus_anz,int aus_typ[],double aus_tab[]){
//=============================================================================
// ellipse; 

static  CurvElli          el1;


  int              irc, i1;
  Point            *p1, *p2, *p3, pta[4], pt1, pt2;
  Vector           *v1, *v2;
  Circ             *ci1;
  Plane            pl1;
  ObjGX            *ox1;
  // Mat_4x3          m1;



  // printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE \n");
  // printf("APT_decode_ell: %d typ=%f\n",aus_anz,aus_tab[0]);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }



  irc = 0;




  //-----------------------------------------------------------------
  // S = ELL C R                            // elli = proj CIR -> PLN
  if        ((aus_anz    == 3)         &&
             (aus_typ[1] == Typ_CI)    &&
             (aus_typ[2] == Typ_PLN))     {


    ci1 = DB_get_CI ((long)aus_tab[1]);
    DB_GetRef (&pl1, (long)aus_tab[2]);

    // pTab = (Point*) memspc55;

    UT3D_el_projcipl (&el1, &pl1, ci1);

    goto L_fertig;





  //-----------------------------------------------------------------
  // S = ELL ELL PLN                        // elli = proj ELL -> PLN
  } else if ((aus_anz    == 3)         &&
             (aus_typ[1] == Typ_CV)    &&
             (aus_typ[2] == Typ_PLN))     {


    ox1 = DB_GetCurv ((long)aus_tab[1]);
    DB_GetRef(&pl1, (long)aus_tab[2]);

    if(ox1->typ != Typ_CVELL) goto Error;

    UT3D_el_projelpl (&el1, &pl1, ox1->data);

    goto L_fertig;






  //-----------------------------------------------------------------
  // S = ELL P P P                 // ELL = center Hauptachsende Nebenachsende
  } else if ((aus_anz    ==   4)            &&
             (aus_typ[1] == Typ_PT)         &&
             (aus_typ[2] == Typ_PT)         &&
             (aus_typ[3] == Typ_PT))            {

    p1 = DB_get_PT ((long)aus_tab[1]);
    p2 = DB_get_PT ((long)aus_tab[2]);
    p3 = DB_get_PT ((long)aus_tab[3]);
    irc = UT3D_el_ptc_ptx_pt (&el1, p1, p2, p3);
    if(irc < 0) goto Error;

    goto L_fertig;





  //-----------------------------------------------------------------
  // S = ELL P D D [ P P ]  // ELL = center Hauptachse Nebenachse [startPt endPt]
  } else if ((aus_typ[1] == Typ_PT)         &&
             (aus_typ[2] == Typ_VC)         &&
             (aus_typ[3] == Typ_VC))            {


    p1 = DB_get_PT ((long)aus_tab[1]);
    v1 = DB_get_VC ((long)aus_tab[2]);
    v2 = DB_get_VC ((long)aus_tab[3]);

    // elli from centerPt,endPtMajAxis,ptOnElli (360-deg)
    UT3D_pt_traptvc (&pt2, p1, v1);
    UT3D_pt_traptvc (&pt1, p1, v2);
    irc = UT3D_el_ptc_ptx_pt (&el1, p1, &pt2, &pt1);
      // DEB_dump_obj__(Typ_CVELL, &el1, "  el1-1");

    if((aus_anz >= 6) &&
             (aus_typ[4] == Typ_PT)         &&
             (aus_typ[5] == Typ_PT))            {
      // project points onto ellipse
      pt1 = DB_GetPoint ((long)aus_tab[4]);
      irc = UT3D_pt_projptel (&i1, pta, &el1, &pt1);
      pt1 = pta[0];
      pt2 = DB_GetPoint ((long)aus_tab[5]);
      irc = UT3D_pt_projptel (&i1, pta, &el1, &pt2);
      pt2 = pta[0];
      // set closed-flag
      if(UT3D_comp2pt(&pt1, &pt2, UT_TOL_pt))
        el1.clo = 0;    // yes closed
      else
        el1.clo = 1;    // no, different points
      // get CW-flag
      if((aus_typ[6] == Typ_cmdNCsub)&&(aus_tab[6] == T_CW))  {
        // set curve CW
        el1.srot = 1;    // CW
        // if(el1.ango > 0.) el1.ango *= -1.;
          // DEB_dump_obj__ (Typ_CVELL, &el1, "_decode_ell-CW");
      }

      // update ango of elli
      UT3D_el_el2pt (&el1, &pt1, &pt2);
        // DEB_dump_obj__(Typ_CVELL, &el1, "  el1-2");
    }


    goto L_fertig;




/*
  //-----------------------------------------------------------------
  // S = ELL PT VC VC P
  // S = ELL PT VC VC P P
  // ELL = Center VecHauptachse VecNebenachse Startpunkt Endpunkt
  } else if ((aus_anz    >=   5)            &&
             (aus_typ[1] == Typ_PT)         &&
             (aus_typ[2] == Typ_VC)         &&
             (aus_typ[3] == Typ_VC)         &&
             (aus_typ[4] == Typ_PT))            {


    el1.pc = DB_GetPoint ((long)aus_tab[1]);
    el1.va = DB_GetVector ((long)aus_tab[2]);
    el1.vb = DB_GetVector ((long)aus_tab[3]);

    el1.p1 = DB_GetPoint ((long)aus_tab[4]);

    if((aus_anz == 6) && (aus_typ[5] == Typ_PT)) {
      el1.p2 = DB_GetPoint ((long)aus_tab[5]);
    } else {
      UT3D_pt_opp2pt (&el1.p2, &el1.pc, &el1.p1);
    }


    UT3D_vc_perp2vc (&el1.vz, &el1.va, &el1.vb);
    UT3D_vc_setLength (&el1.vz, &el1.vz, 1.);

    // DEB_dump_obj__ (Typ_CVELL, &el1, "el1=");

    goto L_fertig;





  //-----------------------------------------------------------------
  // S = ELL PT VC VC P P
  // ELL = Center VecHauptachse VecNebenachse Startpunkt Endpunkt
  } else if ((aus_anz    ==   6)            &&
             (aus_typ[1] == Typ_PT)         &&
             (aus_typ[2] == Typ_VC)         &&
             (aus_typ[3] == Typ_VC)         &&
             (aus_typ[4] == Typ_PT)         &&
             (aus_typ[5] == Typ_PT))            {



    el1.pc = DB_GetPoint ((long)aus_tab[1]);
    el1.va = DB_GetVector ((long)aus_tab[2]);
    el1.vb = DB_GetVector ((long)aus_tab[3]);

    el1.p1 = DB_GetPoint ((long)aus_tab[4]);
    el1.p2 = DB_GetPoint ((long)aus_tab[5]);


    UT3D_vc_perp2vc (&el1.vz, &el1.va, &el1.vb);
    UT3D_vc_setLength (&el1.vz, &el1.vz, 1.);

    // DEB_dump_obj__ (Typ_CVELL, &el1, "el1=");

    goto L_fertig;
*/






  //-----------------------------------------------------------------
  } else {

    TX_Error(" Ellipsendefinition nicht implementiert\n");
    return -1;
  }




  L_fertig:

  cv_out->typ   = Typ_CVELL;
  cv_out->form  = Typ_CVELL;
  cv_out->siz   = 1;
  cv_out->data  = (void*)&el1;


    // DEB_dump_obj__ (Typ_CVELL, &el1, "ex APT_decode_ell:\n");


  return irc;



  //--------------------------------------------------
  Error:
  return -1;
}

  


//=============================================================================
  int APT_decode_rect1 (ObjGX *cv_out,  Memspc *memSeg1,
                        int aus_anz,int aus_typ[],double aus_tab[]){
//=============================================================================
// eine Kurve aus der Inputsource einlesen und retournieren.
// Wird in memSeg1 geliefert.

static  CurvPoly plg1;

  int      ptAnz, pTabSiz;
  double   dx, dy;
  Point    pt1, *pTab;



  pt1 = DB_GetPoint ((long)aus_tab[1]);
  dx  = aus_tab[2];
  dy  = aus_tab[3];


  printf("APT_decode_rect1 dx=%f dy=%f\n",dx,dy);
  if((dx < UT_TOL_pt)||(dx < UT_TOL_pt)) {
    TX_Error("Rectangle-parameter error");
    return -1;
  }



  pTab = memSeg1->next;
  pTabSiz = UME_ck_free(memSeg1) / sizeof(Point);
  if(pTabSiz < 5) {
    TX_Error("Rectangle-out of mem");
    return -1;
  }


  pTab[0] = pt1;

  pt1.x += dx;
  pTab[1] = pt1;

  pt1.y += dy;
  pTab[2] = pt1;

  pt1.x -= dx;
  pTab[3] = pt1;

  pt1.y -= dy;
  pTab[4] = pt1;



  // den Platz im memSeg1 reservieren
  UME_add (memSeg1, sizeof(Point) * 5);


  // CurvPolygon erstellen
  UT3D_plg_pta (&plg1, pTab, 5, memSeg1);



  cv_out->typ   = Typ_CVPOL;
  cv_out->form  = Typ_CVPOL;
  cv_out->siz   = 1;
  cv_out->data  = &plg1;


  return 0;

}


//=============================================================================
  int APT_decode_cv (ObjGX *cvo, int *iNew,
                     int aus_anz, int aus_typ[], double aus_tab[]) {
//=============================================================================
// Der Output-Curvetyp ist cvo->typ.
// iNew = 0; Standard; new data; save also Dataspace
// iNew = 1; Derived (copied) curve; do not save dataspace (only ObjGX-Rec.)

// ACHTUNG: uses memspc101


  int       irc, i1, i2, typ;
  long      l1, dbi;
  double    kv0, kv1;
  ObjGX     *ox1;
  Memspc    workSeg;
  char      obj2[OBJ_SIZ_MAX];
  void      *vp1;

  static char      obj1[OBJ_SIZ_MAX];


  // printf("APT_decode_cv %d\n",aus_anz);
  // for(i1=0;i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  //----------------------------------------------------------------
  // weiterverzweigen ?  bei POL BSP ...
  if(aus_typ[0] == Typ_cmdNCsub) {
    i1 = aus_tab[0];

    if(i1 == T_POL) {
      UME_init (&workSeg, memspc101, sizeof(memspc101));
      return APT_decode_pol (cvo, &workSeg, aus_anz, aus_typ, aus_tab);

    } else if(i1 == T_BSP0) {
      UME_init (&workSeg, memspc101, sizeof(memspc101));
      return APT_decode_bsp_ (cvo, &workSeg, NULL, aus_anz, aus_typ, aus_tab);

    } else if(i1 == T_BSP1) {
      UME_init (&workSeg, memspc101, sizeof(memspc101));
      return APT_decode_bsp1 (cvo, &workSeg, NULL, aus_anz, aus_typ, aus_tab);

    } else goto Par_err;
  }





  //================================================================
  if(aus_anz == 1)  {
  //================================================================


    //----------------------------------------------------------------
    // 'S#'
    if(aus_typ[0] == Typ_CV) {
      ox1 = DB_GetCurv ((long)aus_tab[0]);
      *cvo = *ox1;
      *iNew = 1;        // do not copy data ...


    //----------------------------------------------------------------
    } else goto Par_err;



  //================================================================
  } else if (aus_anz == 2)          {
  //================================================================


    //-----------------------------------------------------------------
    // S = S SEG                          // extract Curve aus CCV
    // S = S MOD    OBSOLETE              // extract Curve aus CCV
    if  ((aus_typ[0] == Typ_CV)         &&
         ((aus_typ[1] == Typ_SEG)    ||
          (aus_typ[1] == Typ_modif)))        {

      if(aus_typ[1]==Typ_modif) MSG_ERR__ (ERR_obsolete, "change MOD to SEG");

      *iNew = 1;        // do not copy data ...

      ox1 = DB_GetCurv  ((long)aus_tab[0]);
        // DEB_dump_obj__ (Typ_ObjGX, ox1, "ex _GetCurv");

      l1 = (long)aus_tab[0];   // dbi of S
      i1 = aus_tab[1] - 1;     // segNr -> indNr; 0=first.

      // ox1 can be polygon or contour; get segment <i1> -> obj1


      //----------------------------------------------------------------
      if(ox1->typ == Typ_CVPOL) {
        irc = CVTRM__plg_iseg ((CurvCCV*)obj1,
                               i1, l1, (CurvPoly*)ox1->data);
        if(irc < 0) return -1;
        OGX_SET_OBJ (cvo, Typ_CVTRM, Typ_CVTRM, 1, obj1);


      //----------------------------------------------------------------
      } else if(ox1->typ == Typ_CVTRM) {
        if(i1 >= ox1->siz) goto Par_err;
        // copy trimmedCurve[i1] -> obj1
        memcpy(obj1, &((CurvCCV*)ox1->data)[i1], sizeof(CurvCCV));
          // DEB_dump_obj__ (Typ_CVTRM, obj1, " curv-S-MOD");
        OGX_SET_OBJ (cvo, Typ_CVTRM, Typ_CVTRM, 1, obj1);
        

      //----------------------------------------------------------------
      } else goto NotImp_err;



/*
    //-----------------------------------------------------------------
    // S = A uPar                      // extract Curve aus Surf
    } else if((aus_typ[0] == Typ_SUR)     &&
              (aus_typ[1] == Typ_Val))         {

        printf(" isoCurv %f\n",aus_tab[1]);
*/


    //----------------------------------------------------------------
    } else goto Par_err;




  //================================================================
  } else if (aus_anz    == 3)          {
  //================================================================


    //-----------------------------------------------------------------
    // S = S MOD MOD                          // extract Curve aus CCV
    if  ((aus_typ[0] == Typ_CV)     &&
         (aus_typ[1] == Typ_modif)  &&
         (aus_typ[2] == Typ_modif))         {

      *iNew = 1;        // do not copy data ...

      ox1 = DB_GetCurv  ((long)aus_tab[0]);
        // DEB_dump_obj__ (Typ_ObjGX, ox1, " ex _GetCurv");

      l1 = (long)aus_tab[0];   // dbi of S
      i1 = aus_tab[1] - 1;     // segNr -> indNr; 0=first.
      i2 = aus_tab[2] - 1;     // segNr -> indNr; 0=first.


      if(ox1->typ == Typ_CVTRM) {
        // get obj2 = basicCurve of segment i1
        // irc = CVTRM__basCv__ (&obj2, &vp1, &((CurvCCV*)ox1->data)[i1]);
        // if(irc < 0) return -1;
        // typ = ((CurvCCV*)obj2)->typ;
        // dbi = ((CurvCCV*)obj2)->dbi;

        // get obj2 = standardCurve of segment i1
        irc = UTO_cv_cvtrm (&typ, &obj2, NULL, &((CurvCCV*)ox1->data)[i1]);
        if(irc < 0) return -1;
          // DEB_dump_obj__ (typ, obj2, " ex _cv_cvtrm");

        // get typ and db-index of basic-curve obj2
        CVTRM_basCv_trmCv (&typ, &dbi, &vp1, &((CurvCCV*)ox1->data)[i1]);
          // printf(" typ=%d dbi =%ld\n",typ,dbi);
          // DEB_dump_obj__ (typ, vp1, " ex _basCv_trmCv");

        if(typ == Typ_CVPOL) {
          // get parameters par1,par2 of segment <i2> of trimmed-polygon obj2
          UPLG_2par_iseg (&kv0, &kv1, i2, obj2);
          // get trimmed-curve of basicCurve vp1 and parameters kv0,kv1
          CVTRM__plg_2par ((CurvCCV*)obj1, &kv0, &kv1, dbi, vp1);
          // CurvCCV -> ObjGX
          OGX_SET_OBJ (cvo, Typ_CVTRM, Typ_CVTRM, 1, obj1);


        } else goto NotImp_err;


      } else goto NotImp_err;


/*
    //-----------------------------------------------------------------
    // 'A# vPar CX'                      // extract Curve aus Surf
    } else if((aus_typ[0] == Typ_SUR)     &&
              (aus_typ[1] == Typ_Val)     &&
              (aus_typ[2] == Typ_cmdNCsub))         {

        printf(" isoCurv %f CX\n",aus_tab[1]);
*/


    //----------------------------------------------------------------
    } else goto Par_err;




  //================================================================
  } else { goto Par_err; }
  //================================================================


    // TESTBLOCK
    // DEB_dump_obj__ (Typ_ObjGX, cvo, "ex_APT_decode_cv");

  return 0;




  //================= Fehler =============================
  Par_err:
  TX_Error("CurveDefinition: ParameterError ****");
  goto Error;

  Geom_err:
  TX_Error("CurveDefinition:: GeometryError ****");
  goto Error;

  NotImp_err:
  TX_Error("CurveDefinition:: NOT YET IMPLEMENTED ****");

  Error:
  return -1;


}

/* ersetzt durch APT_decode_cv
//=============================================================================
  int APT_decode_curv(ObjGX *cvo,int aus_anz,int aus_typ[],double aus_tab[]){
//=============================================================================

  int   i1;
  ObjGX *ox1;


  // printf("APT_decode_curv %d\n",aus_anz);
  // for(i1=0;i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }



  if(aus_typ[0] == Typ_CV) {
    ox1 = DB_GetCurv ((long)aus_tab[0]);
    *cvo = *ox1;

  } else {
    TX_Print("APT_decode_curv E001 %d",aus_typ[0]);
  }

  return 0;

}
*/

//=============================================================================
  int APT_decode_pol (ObjGX *cv_out,  Memspc *memSeg1,
                      int aus_anz,int aus_typ[],double aus_tab[]){
//=============================================================================
// eine Kurve aus der Inputsource einlesen und retournieren.
// Wird in memSeg1 geliefert.
// Input:
//   cv_out     size=OBJ_SIZ_MAX
//   memSeg1    memspace for cpTab,[lvTab] ..   memspc501 



  int      i1, i2, rc, ptNr, ptAnz, typAct, pTabSiz;
  char     *w;
  Point    pt1, *pTab;
  CurvPoly *plg1;




  // printf("APT_decode_pol: %d typ=%f\n",aus_anz,aus_tab[0]);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  // only one point: tempDisp
  if(aus_anz == 2) {
    if(aus_typ[1] == Typ_PT) {
      UI_disp_dbo ((int)aus_typ[1], (long)aus_tab[1], 1);
      return 1;
    }
  }


  // Rechteck
  if((aus_anz == 4)&&(aus_typ[2] == Typ_Val)) {
    return APT_decode_rect1 (cv_out, memSeg1, aus_anz, aus_typ, aus_tab);
  }



  // Polygon <- other type of curve (convert)
  if(aus_typ[1] == Typ_CV) {
    return APT_decode_conv_pol (cv_out, memSeg1, aus_anz, aus_typ, aus_tab);
  }




  // den CV-Typ holen
  if((int)aus_tab[0] == T_POL2) {    // 16
    typAct = Typ_CV2;

  } else if((int)aus_tab[0] == T_POL) {
    typAct = Typ_CV;

  } else {
    goto Error;
  }

  // printf(" typAct = %d\n",typAct);




  rc = 0;


/*
  // die ersten Punkte nach ObjG2 IN_obj[IN_anz].
  i1 = APT_decode_inp (aus_anz-1, &aus_typ[1], &aus_tab[1]);
  ptNr = IN_anz;

  if(i1 < 0) goto Error;
*/




  // get space for plg1 in memSeg1
  plg1 = UME_reserve (memSeg1, sizeof(CurvPoly));
  *plg1 = UT3D_PLG_NUL;




  // nun alle Punkte in die Datei raus, den ersten und den letzen < cv_out.
  // UTCV_open_w (&dxf_auxFilLun, oInd);


  ptAnz = 0;
  pTab = memSeg1->next;
  pTabSiz = UME_ck_free(memSeg1) / sizeof(Point);
    // printf(" pSiz=%d\n",pTabSiz);



  for (i1=1; i1<aus_anz; ++i1) {

    if(ptAnz >= pTabSiz) {
      TX_Error("APT_decode_pol pTab overflow");
      goto Error;
    }

    // pt1 = IN_obj[i1].p1;  - mit APT_decode_inp ..

    if(aus_typ[i1] ==  Typ_PT) {
      pTab[ptAnz] = DB_GetPoint ((long)aus_tab[i1]);
        // DEB_dump_obj__ (Typ_PT, &pTab[ptAnz], "pTab[%d]",ptAnz);


    } else {
      TX_Error("APT_decode_pol E001_%d_%f\n",aus_typ[i1],aus_tab[i1]);
      return -1;
    }


    ++ptAnz;
  }


  // den Platz im memSeg1 reservieren
  i1 = UME_add (memSeg1, sizeof(Point) * ptAnz);
  if(i1 < 0) {
    TX_Error("APT_decode_pol EOM");
    return -1;
  }




  // CurvPolygon erstellen
  UT3D_plg_pta (plg1, pTab, ptAnz, memSeg1);

 

  cv_out->typ   = Typ_CVPOL;
  cv_out->form  = Typ_CVPOL;
  cv_out->siz   = 1;
  cv_out->data  = plg1;

    // DEB_dump_obj__ (Typ_CVPOL, plg1, "ex decode_pol");

  return rc;



  //--------------------------------------------------
  Error:
  return -1;
}


//=============================================================================
  int APT_decode_pol2 (ObjGX *cv_out, Memspc *memSeg1,
                       int aus_anz, int aus_typ[], double aus_tab[]){
//=============================================================================
// eine Kurve aus der Inputsource einlesen
// see APT_decode_pol
// Input:
//   cv_out     size=OBJ_SIZ_MAX
//   memSeg1    memspace for cpTab,[lvTab] ..   memspc501 


  int      i1, ptNr, pTabSiz;
  Point    *ppt;
  Point2   *pTab;
  CurvPol2 *plg1;


  printf("APT_decode_pol2: %d typ=%f\n",aus_anz,aus_tab[0]);
  for(i1=0; i1<aus_anz; ++i1) {
    printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  }


  // only one point: tempDisp
  if(aus_anz == 2) {
    if(aus_typ[1] == Typ_PT) {
      UI_disp_dbo ((int)aus_typ[1], (long)aus_tab[1], 1);
      return 1;
    }
  } 
    

  // Polygon <- other type of curve (convert)
  if(aus_typ[1] == Typ_CV) {
    return APT_decode_conv_pol2 (cv_out,aus_anz,aus_typ,aus_tab);
  }


  // get space for plg1 in memSeg1
  plg1 = UME_reserve (memSeg1, sizeof(CurvPol2));

  // check space for points in memSeg1
  pTab = memSeg1->next;
  pTabSiz = UME_ck_free(memSeg1) / sizeof(Point);
  ptNr = aus_anz - 1;
  if(ptNr >= pTabSiz) {
    TX_Error("APT_decode_pol2 EOM");
    return -1;
  }

  // copy points -> memSeg1
  ptNr = 0;
  for(i1=1; i1<aus_anz; ++i1) {

    if(aus_typ[i1] ==  Typ_PT) {
      ppt = DB_get_PT ((long)aus_tab[i1]);         // get 3D-DB-point
      pTab[ptNr] = UT2D_pt_pt3 (ppt);              // remove z-coord
        // DEB_dump_obj__ (Typ_PT, &pTab[ptAnz], "pTab[%d]",ptAnz);

    } else {
      TX_Error("APT_decode_pol E001_%d_%f\n",aus_typ[i1],aus_tab[i1]);
      return -1;
    }

    ++ptNr;
  }

  // occupy space for points in memSeg1
  i1 = UME_add (memSeg1, sizeof(Point) * ptNr);

  // update curve
  plg1->ptNr = ptNr;
  plg1->pTab = pTab;

  // make ox
  cv_out->typ   = Typ_CVPOL2;
  cv_out->form  = Typ_CVPOL2;
  cv_out->siz   = 1;
  cv_out->data  = plg1;


/*
  ptNr = 0;
  pTab = (void*) memspc55;
  pTabSiz = sizeof(memspc55) / sizeof(Point2);

  i1 = APT_decode_inp (aus_anz, aus_typ, aus_tab);
  if(i1 < 0) {
    TX_Error("Formaterror");
    return -1;
  }

  if(IN_anz >= pTabSiz) {
    // printf(" IN_anz=%d pTabSiz=%d\n",IN_anz,pTabSiz);
    TX_Error("APT_decode_pol2 pTab zuklein");
    return -1;
  }

  for(i1=0; i1<IN_anz; ++i1) {
    // pTab[ptNr] = IN_obj[i1].p1;
    // hier sollte man eigentlich alle Punkte auf die aktive plane projizieren
    // Ergebnis soll aber ein 2D-Punkt sein!
    pTab[ptNr].x = IN_obj[i1].p1.x;
    pTab[ptNr].y = IN_obj[i1].p1.y;
    ++ptNr;
  }

  cv_out->typ   = Typ_CVPOL2;
  cv_out->form  = Typ_PT2;
  cv_out->siz   = ptNr;
  cv_out->data  = (void*)pTab;
*/

     // DEB_dump_ox_0 (cv_out, "ex APT_decode_pol2:");

  return 0;

}


//=============================================================================
  int APT_decode_tra (ObjGX *ox1,int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
/// returns Vector (Typ_VC) or TraRot (Typ_Tra)

static  Vector  vc1;
static  TraRot  trr;

  int     i1;
  Point   pto;
  Line    *lnp;
  Circ    *cip;
  Plane   *plp;
  Mat_4x4 m2;
  ObjGX   *oxh;


  // printf("APT_decode_tra |%d|\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }



  //----------------------------------------------------------------
  // T = VC Len
  if(aus_typ[0] == Typ_VC)  {

    vc1 = DB_GetVector ((long)aus_tab[0]);

    if(aus_typ[1] == Typ_Val) {
      UT3D_vc_setLength (&vc1, &vc1, aus_tab[1]);
    }

    // ox1->form  = Typ_TraTra;
    ox1->form  = Typ_VC;
    ox1->data  = (void*)&vc1;





  //================================================================
  //================================================================
  // T = ROT L|C|R Ang
  // see also APT_decode_goAxis
  } else if ((aus_typ[0] == Typ_cmdNCsub)  &&
             (aus_tab[0] == T_ROT))            {


    //----------------------------------------------------------------
    if(aus_typ[1] == Typ_PT)                   {
      pto = DB_GetPoint ((long)aus_tab[1]);
      vc1 = WC_sur_act.vz;


    //----------------------------------------------------------------
    } else if(aus_typ[1] == Typ_LN)            {
      lnp = DB_get_LN ((long)aus_tab[1]);
      UT3D_vc_ln (&vc1, lnp);
      pto = lnp->p1;


    //----------------------------------------------------------------
    } else if(aus_typ[1] == Typ_CI)            {
      cip = DB_get_CI ((long)aus_tab[1]);
      vc1 = cip->vz;
      pto = cip->pc;


    //----------------------------------------------------------------
    } else if(aus_typ[1] == Typ_PLN)            {
      plp = DB_get_PLN ((long)aus_tab[1]);
      vc1 = plp->vz;
      pto = plp->po;


    //----------------------------------------------------------------
    } else goto Par_err;


    //----------------------------------------------------------------
    if(aus_typ[2] == Typ_Angle) {
      trr.angr = UT_RADIANS(aus_tab[2]);
    } else goto Par_err;


    // Load TraRot trr;
    trr.vz   = vc1;


    // load matrix
    UT3D_m3_inirot_angr (trr.ma, &pto, &trr.vz, trr.angr);
      // DEB_dump_obj__ (Typ_M4x3, trr.ma, "trr.ma");


    ox1->form  = Typ_TraRot;
    ox1->data  = (void*)&trr;


  //================================================================
  // T = T
  } else if(aus_typ[0] == Typ_Tra)  {
    // copy
    oxh = DB_GetTra ((long)aus_tab[0]);
    *ox1 = *oxh;



  //================================================================
  } else {
    goto Par_err;
  }


  ox1->typ   = Typ_Tra;
  ox1->siz   = 1;

    // DEB_dump_ox_0 (ox1, "ex APT_decode_tra:");

  return 0;

  //================= Fehler =============================
  Par_err:
  TX_Error("Definition Transformation: ParameterError ****");
  goto Error;

  Geom_err:
  TX_Error("Definition Transformation: GeometryError ****");
  goto Error;

  NotImp_err:
  TX_Error("Definition Transformation: NOT YET IMPLEMENTED ****");

  Error:
  return -1;

}

/*
//=============================================================================
  int APT_decode_tool (BTool *tl1,int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================


  // TX_Print("APT_decode_tool ");



  if       ((aus_anz    ==   1)         &&
            (aus_typ[0] ==Typ_Val)) {

    tl1->dmr    = aus_tab[0];


  } else {
    TX_Error(" Tooldefinition nicht implementiert");
    goto Error;
  }




  // Fertig:
  //TX_Print("Nullpunkt %.2f,%.2f,%.2f",pt1.x,pt1.y,pt1.z);
  return 1;


  Error:
  return -1;

}
*/
      
//=============================================================================
  int APT_decode_Joint (Memspc *spcObj,
                        int aus_anz, int aus_typ[], double aus_tab[],
                        Memspc *spcTmp) {
//=============================================================================
// pack joint into ObjGX in spcObj (get primary obj with UME_get_start)
// All data must be inside spcObj (serialized).
// TODO: change into a single binary obj .. (size+typ+data)
 
  int       ii, i1;
  void      *vp0, *vp1, *vp2, *vp3, *vpd;
  ObjGX     *oxo, *oTab;

//          typ          form        siz
// vp0   Typ_Joint     Typ_ObjGX       1
// vp1   <aus_typ>     Typ_ObjGX     <aus_anz>
// vp2   <aus_typ>     <aus_typ>       1


  // printf("\nAPT_decode_Joint %d\n",aus_anz);
  // printf("   APT_obj_stat=%d\n",APT_obj_stat);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }



  oTab = MEM_alloc_tmp ((int)(sizeof(ObjGX) * aus_anz));

  for(i1=0; i1<aus_anz; ++i1) {
    OGX_ox_ato1 (&oTab[i1], &aus_typ[i1], &aus_tab[i1]);
  }

  oxo = OGX_oxm_copy_ox (spcObj, oTab, aus_anz, 1);
  if(!oxo) goto Par_err;

    // TESTBLOCK
    // DEB_dump_ox_0 (oxo, "APT_decode_Joint");
    // DEB_dump_ox_s_ (oxo, "APT_decode_Joint");
    // END TESTBLOCK


  return 0;


  //================= Fehler =============================
  Par_err:
    TX_Error("Joint-Definition: unsupported object");
    goto Error;


  Error:
  return -1;

}


//=============================================================================
  int APT_decode_Activ (Activity *act, Memspc *oSpc,
                        int aus_anz, int aus_typ[], double aus_tab[]) {
//=============================================================================
// obj[0] = anchor-object       (.typ,.ind)
// all following data = command (.data)


  int   i1;
  long  ind;
  char  *p1, cbuf[40];



  // printf("APT_decode_Activ |%s|\n",APT_defTxt);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


/*
  // check for activation-object or program-activation
  if(aus_typ[0] != Typ_String) goto L_act_obj;

  //================================================================
  // no activation-object
  act->typ = Typ_Error;           // no obj.
  act->ind = 0;

  // I1="FORM f1"
    APT_get_String (APT_spc1, APT_defTxt, aus_tab[0]);
    act->data = APT_spc1;

  // activate now !


  goto L_exit;
*/



  //================================================================
  // Activity connected to aus_xx[0]
  L_act_obj:
  act->typ = aus_typ[0];
  act->ind = aus_tab[0];


  // I20=L20 "VIEW 0 0 1 -4 -21 0 1 0 1 0"
  // I21=L21 "EXECM tst3"
  // I23=N22 "LDMOD "Data/sample_mod_blech1.gcad""
  // I23=N22 "MODVAL P123 1"


  //----------------------------------------------------------------
  if(aus_anz < 2) {                  // eg I20=MODVAL P20 1
    p1 = APT_defTxt;
    goto L_exit;
  }



  //----------------------------------------------------------------
  i1 = 1;
  if(aus_typ[i1] == Typ_Activ) {    // eg I21=N20 I20
    ind = aus_tab[i1];
    APED_oid_dbo__ (cbuf, Typ_Activ, ind);
    p1 = cbuf;
    goto L_add_cmd;


  } else if ((aus_typ[i1] == Typ_String) ||
             (aus_typ[i1] == Typ_Txt))      {
    // aus_xx[i1]: commandtext.
    // APT_get_TxAll (APT_spc1, APT_defTxt, aus_tab[i1]);
    // act->data = APT_spc1;
    // aus_tab[i1] = offset in APT_defTxt
    i1 = aus_tab[i1];
    p1 = &APT_defTxt[i1];
    goto L_add_cmd;

  }
  goto L_err1;



  //----------------------------------------------------------------
  L_add_cmd:
    i1 = strlen (p1);
    if(i1 < 2) goto L_err1;
    ++i1; // add closing 0
    // add commandText into oSpc
    act->data = UME_save (oSpc, p1, i1);



  //----------------------------------------------------------------
  L_exit:
    // DEB_dump_obj__ (Typ_Activ, act, "ex APT_decode_Activ:");
  return 0;


  L_err1:
    TX_Error("Parameter APT_decode_Activ");
    return -1;


}


//=============================================================================
  int APT_decode_pln_rsys (Plane *pln1,
                           int aus_anz, int aus_typ[], double aus_tab[]) {
//=============================================================================
// decode R=RSYS [Origin] [Refsys] [offset-X-axis] [offset-Y-axis] [offset-Z-axis] 
//               [Angle-around-Z] [Angle-around-Y]

  int      i1, iNxt, iTst;
  Point    pto;
  double   ar;
  Mat_4x3  ma;


  printf("APT_decode_pln_rsys %d\n",aus_anz);
  for(i1=0; i1<aus_anz; ++i1)
  printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);


  iNxt = 1;


  // get origin
  if(aus_typ[iNxt] == Typ_PT) {
    pto = DB_GetPoint ((long)aus_tab[iNxt]);
    ++iNxt;
    iTst = 0;
  } else iTst = 1;


  // get base-refsys
  if(aus_typ[iNxt] == Typ_PLN) {
    DB_GetRef (pln1, (long)aus_tab[iNxt]);
    ++iNxt;
  } else {
    *pln1 = WC_sur_act;
  }


  // fix origin pto
  if(iTst) pto = pln1->po;
    // DEB_dump_obj__ (Typ_PT, &pto, "_pln_rsys: pto");


  // get offsets for origin
  if(aus_typ[iNxt] == Typ_XVal) {
    // move po along X-axis
    UT3D_pt_traptmultvc (&pto, &pto, &pln1->vx, aus_tab[iNxt]);
    ++iNxt;
  }
  if(aus_typ[iNxt] == Typ_YVal) {
    // move po along Y-axis
    UT3D_pt_traptmultvc (&pto, &pto, &pln1->vy, aus_tab[iNxt]);
    ++iNxt;
  }
  if(aus_typ[iNxt] == Typ_ZVal) {
    // move po along Z-axis
    UT3D_pt_traptmultvc (&pto, &pto, &pln1->vz, aus_tab[iNxt]);
    ++iNxt;
  }

  // get angle - az
  if(aus_typ[iNxt] == Typ_Angle) {
    ar = UT_RADIANS (aus_tab[iNxt]);
    ar *= -1.;   // CHANGE ROTATION TO CCW
    if(fabs(ar) > UT_TOL_min0) {
      // rotate x-axis around z-axis see UT3D_vc_rotvcangr
      UT3D_m3_inirot_angr (ma, NULL, &pln1->vz, ar);
      UT3D_vc_tra_vc_m3 (&pln1->vx, ma, &pln1->vx);
      // rotate y-axis around z-axis
      UT3D_vc_tra_vc_m3 (&pln1->vy, ma, &pln1->vy);
    }
    ++iNxt;
  }

  // get angle - ay (tilt-angle)
  if(aus_typ[iNxt] == Typ_Angle) {
    ar = UT_RADIANS (aus_tab[iNxt]);
    if(fabs(ar) > UT_TOL_min0) {
      // rotate x-axis around y-axis
      UT3D_m3_inirot_angr (ma, NULL, &pln1->vy, ar);
      UT3D_vc_tra_vc_m3 (&pln1->vx, ma, &pln1->vx);
      UT3D_vc_tra_vc_m3 (&pln1->vy, ma, &pln1->vy);
      UT3D_vc_tra_vc_m3 (&pln1->vz, ma, &pln1->vz);
    }
  }


  // fix new origin
  UT3D_pl_ptpl (pln1, &pto);


    // DEB_dump_obj__ (Typ_PLN, pln1, "ex APT_decode_pln_rsys");

  return 0;

}


//=============================================================================
  int APT_decode_pln1 (Plane *pln1,
                       int aus_anz, int aus_typ[], double aus_tab[]) {
//=============================================================================
// decode R=Plane/Refsys   (Plane from X-axis)

  int       i1, iPerp;
  double    d1, d2;
  Point     pt1, pt2, pt3;
  Vector    vc1, vc2, vcx, vcy, vcz;
  Line      *lna;
  Circ      *cip;
  Plane     *plp1;
  ModelRef  *mdr;


  // printf("APT_decode_pln1 %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1)
  // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);


  // pt1 = WC_sur_act.po;
  pt1 = UT3D_PT_NUL;

  // PERP
  if(aus_typ[0] == Typ_cmdNCsub)
    return APT_decode_pln2 (pln1, aus_anz, aus_typ, aus_tab);




  //================================================================
  if(aus_anz == 1) {
  //================================================================


    //----------------------------------------------------------------
    // R = R
    if(aus_typ[0] == Typ_PLN) {
      DB_GetRef (pln1, (long)aus_tab[0]);
      goto Fertig;


    //----------------------------------------------------------------
    // R = D               (Plane from its X-Axis)
    } else if(aus_typ[0] == Typ_VC) {
      vcx = DB_GetVector ((long)aus_tab[0]);
      UT3D_pl_ptvxpl (pln1, &pt1, &vcx, &WC_sur_act);
      goto Fertig;
      // goto L_pvxvz;   // pln from  pt1 vcx vcz
      // UT3D_pl_bp (pln1, BCKVEC_X, aus_tab[0]);
      // goto Fertig;



    //----------------------------------------------------------------
    // R = x(val)                       
    } else if(aus_typ[0] == Typ_XVal) {
      UT3D_vc_invert (&vcx, &WC_sur_act.vz);
      vcz = WC_sur_act.vx;
      pt1.x += aus_tab[0];
      goto L_pvxvz;   // pln from  pt1 vcx vcz
      // UT3D_pl_bp (pln1, BCKVEC_X, aus_tab[0]);
      // goto Fertig;


    //----------------------------------------------------------------
    // R = y(val)
    } else if(aus_typ[0] == Typ_YVal) {
      vcx = WC_sur_act.vx;
      vcz = WC_sur_act.vy;
      pt1.y += aus_tab[0];
      goto L_pvxvz;   // pln from  pt1 vcx vcz
      // UT3D_pl_bp (pln1, BCKVEC_Y, aus_tab[0]);
      // goto Fertig;


    //----------------------------------------------------------------
    // R = z(val)
    } else if(aus_typ[0] == Typ_ZVal) {
      vcx = WC_sur_act.vx;
      vcz = WC_sur_act.vz;
      pt1.z += aus_tab[0];
      goto L_pvxvz;   // pln from  pt1 vcx vcz
      // UT3D_pl_bp (pln1, BCKVEC_Z, aus_tab[0]);
      // goto Fertig;


    //----------------------------------------------------------------
    // R = PT                plane Point in active Refsys (WC_sur_act)
    } else if(aus_typ[0] == Typ_PT) {
      pt1 = DB_GetPoint ((long)aus_tab[0]);
      *pln1 = WC_sur_act;           // die active ConstrPlane
      // UT3D_pl_XYZ (pln1);
      UT3D_pl_ptpl (pln1, &pt1);     // move Plane


    //----------------------------------------------------------------
    // R = L            plane from line (origin = Linestartpoint; Line = z-axis)
    } else if(aus_typ[0] == Typ_LN) {

      lna = DB_get_LN ((long)aus_tab[0]);

      UT3D_vc_ln (&vcz, lna);
      UT3D_pl_ptvc (pln1, &lna->p1, &vcz);     // move Plane


    //----------------------------------------------------------------
    // R = C                                       plane from circ
    } else if(aus_typ[0] == Typ_CI) {

      cip = DB_get_CI ((long)aus_tab[0]);

      UT3D_pl_pto_vcz_ptx (pln1, &cip->pc, &cip->vz, &cip->p1);


    //----------------------------------------------------------------
    // R = M                                       plane from model
    } else if(aus_typ[0] == Typ_Model) {

      // get ModelReference
      mdr = DB_get_ModRef ((long)aus_tab[0]);
        // DEB_dump_obj__(Typ_Model, mdr, " decode_pln1-M1 %d  ",i1);
      // get plane from ModelReference
      UT3D_pl_pto_vcz_vcx (pln1, &mdr->po, &mdr->vz, &mdr->vx);
        // DEB_dump_obj__(Typ_PLN, pln1, " decode_pln1-M2 %d  ",i1);


    //----------------------------------------------------------------
    } else {
      goto Error;
    }






  //==================================================================
  } else if(aus_anz == 2) {
  //==================================================================


    //----------------------------------------------------------------
    // R = val val                 //  R(P(val val))
    if((aus_typ[0] == Typ_Val)  &&
       (aus_typ[1] == Typ_Val))       {
      plp1 = &WC_sur_act;
      pt1 = WC_sur_act.po;                     // origin
      pt1.x = aus_tab[0];
      pt1.y = aus_tab[1];
      goto L_pln_pt;


    //----------------------------------------------------------------
    // R = P   P|D|L                    origin, X-axis; changed 2011-10-28
    } else if((aus_typ[0] == Typ_PT)  &&
      ((aus_typ[1] == Typ_PT) ||
       (aus_typ[1] == Typ_VC) ||
       (aus_typ[1] == Typ_LN)))       {

      pt1 = DB_GetPoint ((long)aus_tab[0]);     // origin

      if(aus_typ[1] == Typ_PT)  {
        pt2 = DB_GetPoint ((long)aus_tab[1]);
        UT3D_vc_2pt (&vcx, &pt1, &pt2);
  
      } else if(aus_typ[1] == Typ_VC)  {
        vcx = DB_GetVector ((long)aus_tab[1]);
  
      } else if(aus_typ[1] == Typ_LN)  {
        lna = DB_get_LN ((long)aus_tab[1]);
        UT3D_vc_ln (&vcx, lna);
      }


      L_pln1_DP:  // Plane from X-vector and active Z-vector
        // DEB_dump_obj__ (Typ_VC, &vcx, "vcx:");
      // test if vectors are parallel ..
      if(UT3D_comp2vc_d (&vcx, &WC_sur_act.vz, UT_TOL_min0) != 0) {
        if(vcx.dz > WC_sur_act.vx.dz) 
          UT3D_vc_invert (&vc1, &WC_sur_act.vx);
        UT3D_pl_pto_vcx_vcz (pln1, &pt1, &vcx, &vc1);
      } else {
        UT3D_pl_pto_vcx_vcz (pln1, &pt1, &vcx, &WC_sur_act.vz);
      }
      goto Fertig;


    //----------------------------------------------------------------
    // R = VC VC    // X-axis, Y-Axis
    } else if((aus_typ[0] == Typ_VC)  &&
              (aus_typ[1] == Typ_VC))     {

      vcx = DB_GetVector ((long)aus_tab[0]);
      vcy = DB_GetVector ((long)aus_tab[1]);

      UT3D_pl_pto_vcx_vcy (pln1, &pt1, &vcx, &vcy);
      goto Fertig;


    //----------------------------------------------------------------
    // R = VC PT    // X-axis, point in plane; changed 2011-10-31
    } else if((aus_typ[0] == Typ_VC)  &&
              (aus_typ[1] == Typ_PT))     {

      vcx = DB_GetVector ((long)aus_tab[0]);
      pt1 = DB_GetPoint ((long)aus_tab[1]);

      // project pt1 -> line 0,0,0 - vcx
      UT3D_pt_projptptvc (&pt2, &d1, NULL, &pt1, (Point*)&UT3D_PT_NUL, &vcx);

      UT3D_pl_ptvc (pln1, &pt2, &vcx);
      goto Fertig;


    //----------------------------------------------------------------
    // R = VC value   
    // R = VC ANG(value)
    } else if((aus_typ[0] == Typ_VC)  &&
             ((aus_typ[1] == Typ_Val)      ||
              (aus_typ[1] == Typ_Angle)))       {

      vcz = DB_GetVector ((long)aus_tab[0]);
      d1  = aus_tab[1];

      // relative to the active UCS !
      pt1 = WC_sur_act.po;
      UT3D_pt_traptvclen (&pt1, &pt1, &vcz, d1);

      UT3D_pl_ptvc (pln1, &pt1, &vcz);
      goto Fertig;


    //----------------------------------------------------------------
    // R = PT value                  // origin + angle
    } else if((aus_typ[0] == Typ_PT)  &&
              (TYP_IS_VAL(aus_typ[1])))     {

      pt1 = DB_GetPoint ((long)aus_tab[0]);
      d1  = UT_RADIANS(aus_tab[1]);

      *pln1 = WC_sur_act;            // die active ConstrPlane
      UT3D_pl_ptpl (pln1, &pt1);     // move Plane

      UT3D_pl_rotZangr (pln1, &d1);   // rot plane around Z

      goto Fertig;



    //----------------------------------------------------------------
    // R = PT R            // move Refsys
    } else if((aus_typ[0] == Typ_PT)  &&
              (aus_typ[1] == Typ_PLN))     {

      pt1  = DB_GetPoint ((long)aus_tab[0]);
      plp1 = DB_get_PLN ((long)aus_tab[1]);
      goto L_pln_pt;


    //----------------------------------------------------------------
    // R = PT R            // move Refsys
    } else if((aus_typ[0] == Typ_PLN)  &&
              (aus_typ[1] == Typ_PT))     {

      plp1 = DB_get_PLN ((long)aus_tab[0]);
      pt1  = DB_GetPoint ((long)aus_tab[1]);
      goto L_pln_pt;


    //----------------------------------------------------------------
    // R = L P|D|L                  // Line (x-axis) + Vector (y-axis)
    } else if((aus_typ[0] == Typ_LN)  &&
              (aus_typ[1] == Typ_VC))     {

      lna = DB_get_LN ((long)aus_tab[0]);
      pt1 = lna->p1;
      UT3D_vc_ln (&vc1, lna);

      if(aus_typ[1] == Typ_PT)  {
        pt2 = DB_GetPoint ((long)aus_tab[1]);
        UT3D_vc_2pt (&vc2, &pt1, &pt2);
  
      } else if(aus_typ[1] == Typ_VC)  {
        vc2 = DB_GetVector ((long)aus_tab[1]);
  
      } else if(aus_typ[1] == Typ_LN)  {
        lna = DB_get_LN ((long)aus_tab[1]);
        UT3D_vc_ln (&vc2, lna);
      }



      vc2 = DB_GetVector ((long)aus_tab[1]);
      goto L_done_1;



    //----------------------------------------------------------------
    // R = R Val(offset)          // move Refsys
    } else if((aus_typ[0] == Typ_PLN)  &&
              (aus_typ[1] == Typ_Val))     {

      DB_GetRef (pln1, (long)aus_tab[0]);
      // move px along vz length aus_tab[1]
      UT3D_pt_traptvclen (&pln1->po, &pln1->po, &pln1->vz, aus_tab[1]);
      UT3D_pl_p (pln1);



    //----------------------------------------------------------------
    } else {
      goto Error;
    }





  //==================================================================
  } else if(aus_anz == 3) {
  //==================================================================


    //----------------------------------------------------------------
    // R = val val val             //  R(P(val val val))
    if((aus_typ[0] == Typ_Val)  &&
       (aus_typ[1] == Typ_Val)  &&
       (aus_typ[2] == Typ_Val))       {
      plp1 = &WC_sur_act;
      pt1.x = aus_tab[0];
      pt1.y = aus_tab[1];
      pt1.z = aus_tab[2];
      goto L_pln_pt;


    //----------------------------------------------------------------
    // R = P P|D|L P|D|L                   plane from 3 points (ori, Px, Pxy)
    } else if((aus_typ[0] == Typ_PT)     &&
             ((aus_typ[1] == Typ_PT) ||
              (aus_typ[1] == Typ_VC) ||
              (aus_typ[1] == Typ_LN))    &&
             ((aus_typ[2] == Typ_PT) ||
              (aus_typ[2] == Typ_VC) ||
              (aus_typ[2] == Typ_LN)))       {

      goto L_do_DDO;


    //----------------------------------------------------------------
    // R = PT val val                   origin, rotAng, tiltAng
    // R = PT ANG(val) ANG(val)         origin, rotAng, tiltAng
    } else if((aus_typ[0] == Typ_PT)   &&
             ((aus_typ[1] == Typ_Val)      ||
              (aus_typ[1] == Typ_Angle))  &&
             ((aus_typ[2] == Typ_Val)      ||
              (aus_typ[2] == Typ_Angle)))       {


      pt1 = DB_GetPoint ((long)aus_tab[0]);
      d1  = UT_RADIANS(aus_tab[1]);
      d2  = UT_RADIANS(aus_tab[2]) * -1.;     // tilt = negativ
        // printf("  d1=%lf d2=%lf\n",d1,d2);

      *pln1 = WC_sur_act;            // die active ConstrPlane
      UT3D_pl_ptpl (pln1, &pt1);     // move Plane

      UT3D_pl_rotZangr (pln1, &d1);   // rot plane around Z
      UT3D_pl_rotYangr (pln1, &d2);   // rot plane around Y

      goto Fertig;



    //----------------------------------------------------------------
    // R = P R offset
    } else if((aus_typ[0] == Typ_PT)      &&
             ((aus_typ[1] == Typ_PLN)     &&
              (aus_typ[2] == Typ_Val)))       {

        // printf(" R = P R offset\n");

      pt1 = DB_GetPoint ((long)aus_tab[0]);
        // DEB_dump_obj__ (Typ_PT, &pt1, "  pt1");
      DB_GetRef (pln1, (long)aus_tab[1]);
        // DEB_dump_obj__ (Typ_PLN, pln1, "  pln1");
      // move po along vz length aus_tab[2]
      UT3D_pt_traptvclen (&pln1->po, &pt1, &pln1->vz, aus_tab[2]);
      UT3D_pl_p (pln1);
        // DEB_dump_obj__ (Typ_PLN, pln1, "  pln1");
      goto Fertig;



    //==================================================================
    } else {
      goto Error;
    }



  //==================================================================
  } else if(aus_anz == 4) {
  //==================================================================


    // R = P PX|DX PY|DY offset
    L_do_DDO:

      pt1 = DB_GetPoint ((long)aus_tab[0]);     // origin

      if(aus_typ[1] == Typ_PT)  {
        pt2 = DB_GetPoint ((long)aus_tab[1]);
        UT3D_vc_2pt (&vcx, &pt1, &pt2);

      } else if(aus_typ[1] == Typ_VC)  {
        vcx = DB_GetVector ((long)aus_tab[1]);

      } else if(aus_typ[1] == Typ_LN)  {
        lna = DB_get_LN ((long)aus_tab[1]);
        UT3D_vc_ln (&vcx, lna);
      }


      if(aus_typ[2] == Typ_PT)  {
        pt2 = DB_GetPoint ((long)aus_tab[2]);
        UT3D_vc_2pt (&vcy, &pt1, &pt2);
    
      } else if(aus_typ[2] == Typ_VC)  {
        vcy = DB_GetVector ((long)aus_tab[2]);
        
      } else if(aus_typ[2] == Typ_LN)  {
        lna = DB_get_LN ((long)aus_tab[2]);
        UT3D_vc_ln (&vcy, lna);
      } 

      UT3D_pl_pto_vcx_vcy (pln1, &pt1, &vcx, &vcy);
      if(aus_anz == 4) {
        // move px along vz length aus_tab[3]
        UT3D_pt_traptvclen (&pln1->po, &pln1->po, &pln1->vz, aus_tab[3]);
        UT3D_pl_p (pln1);
      }
      goto Fertig;






    // Plane from origin pt1, X-vector vc1, Y-vector vc2 in XY-plane
    L_done_1:
    UT3D_pl_pto_vcx_vcy (pln1, &pt1, &vc1, &vc2);



  } else goto Error;

  goto Fertig;




  //----------------------------------------------------------------
  // Plane aus Nullpunkt(pt1), Z-Vektor(vcz) und X_Vektor(vcx)
  L_pvxvz:
  if(AP_IS_2D) {
    UT3D_pt_tra_pt_m3 (&pt1, WC_sur_mat, &pt1);
      // DEB_dump_obj__ (Typ_PT, &pt1, " onConstrPln:");
  }

  L_pvxvz1:
  UT3D_pl_pto_vcx_vcz (pln1, &pt1, &vcx, &vcz);
  goto Fertig;


  L_pln_pt:
      *pln1 = *plp1;              // copy plane
      UT3D_pl_ptpl (pln1, &pt1);  // change origin of plane
      goto Fertig;



  //----------------------------------------------------------------
  Fertig:
    // DEB_dump_obj__ (Typ_PLN, pln1, "ex APT_decode_pln1");
  return 1;




  Error:
  TX_Error(" Definition Ref.sys nicht implementiert %d %d %d",
                  aus_typ[0],aus_typ[1],aus_typ[2]);
  return -1;



}


//=============================================================================
  int APT_decode_pln2 (Plane *pl1,int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
// decode R=PERP Plane/Refsys


  int    i1;
  double d1, d2;
  Point  *pt1, *pt2, *pt3, px;
  Vector *vc1, *vc2, vz, vx;
  Line   *lna;


  // printf("APT_decode_pln2 %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }
  // DEB_dump_obj__ (Typ_PLN, &WC_sur_act, "  WC_sur_act");


  if((int)aus_tab[0] != T_PERP) goto Err1;

  // if(aus_typ[1] != Typ_PT) goto Err1;


  //==================================================================
  if(aus_anz == 2) {
  //==================================================================

    px = UT3D_PT_NUL;

    //----------------------------------------------------------------
    // R = PERP D                              Plane from Z-Vector
    if(aus_typ[1] == Typ_VC) {
      vz = DB_GetVector ((long)aus_tab[1]);
      UT3D_pl_ptvzpl (pl1, &px, &vz, &WC_sur_act);


    //----------------------------------------------------------------
    } else {
      goto Err1;
    }



  //==================================================================
  } else if(aus_anz == 3) {
  //==================================================================


    //==================================================================
    // R = PERP D(z-axis) VAL(offset-z-axis)  
    if((aus_typ[1] == Typ_VC) &&
       (aus_typ[2] == Typ_Val))    {

      vz = DB_GetVector ((long)aus_tab[1]);
      // move px along vz length aus_tab[2]
      UT3D_pt_traptvclen (&px, (Point*)&UT3D_PT_NUL, &vz, aus_tab[2]);
      UT3D_pl_ptvzpl (pl1, &px, &vz, &WC_sur_act);


    //==================================================================
    // R = PERP D(z-axis) D(x-axis)
    } else if((aus_typ[1] == Typ_VC) &&
              (aus_typ[2] == Typ_VC))    {

      pt1 = (Point*)&UT3D_PT_NUL;

      vz = DB_GetVector ((long)aus_tab[1]);
      vx = DB_GetVector ((long)aus_tab[2]);
      UT3D_pl_pto_vcx_vcz (pl1, pt1, &vx, &vz);



    //==================================================================
    // R = PERP P  P|D|L             // origin   z-axis
    } else if((aus_typ[2] == Typ_PT) ||
              (aus_typ[2] == Typ_VC) ||
              (aus_typ[2] == Typ_LN))    {

      pt1 = DB_get_PT ((long)aus_tab[1]);

      if(aus_typ[2] == Typ_PT)  {
        pt2 = DB_get_PT ((long)aus_tab[2]);
        UT3D_vc_2pt (&vz, pt1, pt2);

      } else if(aus_typ[2] == Typ_VC)  {
        vz = DB_GetVector ((long)aus_tab[2]);

      } else if(aus_typ[2] == Typ_LN)  {
        lna = DB_get_LN ((long)aus_tab[2]);
        UT3D_vc_ln (&vz, lna);
      }

      UT3D_pl_ptvc (pl1, pt1, &vz);



    //==================================================================
    } else goto Err1;



  //==================================================================
  } else if(aus_anz == 4) {
  //==================================================================

    pt1 = DB_get_PT ((long)aus_tab[1]);


    //==================================================================
    // R = PERP P D Val  // origin z-axis z-offset
    if(((aus_typ[1] == Typ_PT) &&
        (aus_typ[2] == Typ_VC) &&
        (aus_typ[3] == Typ_Val)))       {

      vz = DB_GetVector ((long)aus_tab[2]);

      // move pt1 along vz length aus_tab[3]
      UT3D_pt_traptvclen (&px, pt1, &vz, aus_tab[3]);

      // Plane from Origin, xVec, zVec
      UT3D_pl_ptvc (pl1, &px, &vz);



    //==================================================================
    // R = PERP D(z-axis)  D(x-axis)  Val(Z-offset) 
    } else if(((aus_typ[1] == Typ_VC) &&
               (aus_typ[2] == Typ_VC) &&
               (aus_typ[3] == Typ_Val)))       {

      vz = DB_GetVector ((long)aus_tab[1]);
      vx = DB_GetVector ((long)aus_tab[2]);
      // move px along vz length aus_tab[3]
      UT3D_pt_traptvclen (&px, &WC_sur_act.po, &vz, aus_tab[3]);
      UT3D_pl_pto_vcx_vcz (pl1, &px, &vx, &vz);



    //==================================================================
    // R = PERP P  P|D|L    P|D|L      // origin   z-axis  x-axis
    } else if(((aus_typ[2] == Typ_PT) ||
               (aus_typ[2] == Typ_VC) ||
               (aus_typ[2] == Typ_LN))    &&
              ((aus_typ[3] == Typ_PT) ||
               (aus_typ[3] == Typ_VC) ||
               (aus_typ[3] == Typ_LN)))       {


      if(aus_typ[2] == Typ_PT)  {
        pt2 = DB_get_PT ((long)aus_tab[2]);
        UT3D_vc_2pt (&vz, pt1, pt2);
  
      } else if(aus_typ[2] == Typ_VC)  {
        vz = DB_GetVector ((long)aus_tab[2]);

      } else if(aus_typ[2] == Typ_LN)  {
        lna = DB_get_LN ((long)aus_tab[2]);
        UT3D_vc_ln (&vz, lna);
      }  
  
  
      if(aus_typ[3] == Typ_PT)  {
        pt3 = DB_get_PT ((long)aus_tab[3]);
        UT3D_vc_2pt (&vx, pt1, pt3);

      } else if(aus_typ[3] == Typ_VC)  {
        vx = DB_GetVector ((long)aus_tab[3]);

      } else if(aus_typ[3] == Typ_LN)  {
        lna = DB_get_LN ((long)aus_tab[3]);
        UT3D_vc_ln (&vx, lna);
      }

      // Plane from Origin, xVec, zVec
      UT3D_pl_pto_vcx_vcz (pl1, pt1, &vx, &vz);



    //==================================================================
    // R = PERP PO val val
    } else if((aus_typ[2] == Typ_Val)  &&
              (aus_typ[3] == Typ_Val))     {

      d1  = UT_RADIANS(aus_tab[2]);
      d2  = UT_RADIANS(aus_tab[3]);     // tilt = negativ
        // printf("  d1=%lf d2=%lf\n",d1,d2);

      UT3D_vc_2angr (&vz, d1, d2);
      UT3D_pl_ptvc (pl1, pt1, &vz);


    
    //==================================================================
    } else goto Err1;




  //==================================================================
  } else if(aus_anz == 5) {
  //==================================================================

      pt1 = DB_get_PT ((long)aus_tab[1]);
      vz = DB_GetVector ((long)aus_tab[2]);
      vx = DB_GetVector ((long)aus_tab[3]);

      // px = move pt1 along vz length aus_tab[4]
      UT3D_pt_traptvclen (&px, pt1, &vz, aus_tab[4]);
      UT3D_pl_pto_vcx_vcz (pl1, &px, &vx, &vz);



  //==================================================================
  } else goto Err1;


  // Fertig:
    // DEB_dump_obj__ (Typ_PLN, pl1, "ex APT_decode_pln2\n");
  return 1;


  Err1:
  TX_Error("Parametererror APT_decode_pln2");
  return -1;

}


//=============================================================================
  int APT_decode_spl (ObjGX *ox1, int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
// SPL (planare surf)
/* TODO:
change to SurStd -> Typ_SUTP

see ../../doc/gcad_doxygen/Objects-Format.dox

SUR_ck_typ   ANPASSEN !!!!    In die Doku !

P24=P(100 500 0)
S20=POL P24 P(1500 1500 0) P(500 1500 0) P24
C20=P(200 100 0) P(400 100 0)
C21=P(300 100 0) P(300 150 0)
A22=S20
A23=C20 C21
#

WC_Work1
  APT_work_def
    APT_store_obj
      APT_decode_sur
        APT_decode_spl                             <<<<<<
      DB_StoreSur
    APT_Draw__
      APT_DrawSur
        GR_DrawSur
          TSU_DrawSurT_
            TSU_DrawSurTS    Tess / Disp ungetrimmte/ungelochte Planare Flaeche
            TSU_DrawSurTP    Tess / Disp getrimmte/gelochte Planare Flaeche


change APT_decode_spl
see APT_decode_fsub
- provide backplane as supporting surface (ObjGX[0]) Typ_PLN
*/


// static SurStd  sus;

  int      i1;
  long     l1;
  // ObjGX    *ox1p;
  ObjGX    *oxTab;




  // printf("APT_decode_spl %d\n",aus_anz);
    // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }



  oxTab = (ObjGX*)memspc51;

    OGX_SET_INT (&oxTab[0], Typ_Typ, Typ_SURPLN);
    // oxTab[0].typ  = Typ_Typ;
    // oxTab[0].form = Typ_SURPLN;
    // oxTab[0].siz  = 1;


  for(i1=0; i1<aus_anz ; ++i1) {
    oxTab[i1+1].typ  = aus_typ[i1];
    oxTab[i1+1].form = Typ_Index;
    oxTab[i1+1].siz  = 1;
    l1 = aus_tab[i1];
    oxTab[i1+1].data = PTR_LONG(l1);
  }


  ox1->typ    = Typ_SUR;
  ox1->form   = Typ_ObjGX;
  ox1->siz    = aus_anz + 1;
  ox1->data   = oxTab;

  // DEB_dump_ox_0 (ox1, "Pln.Sur:");

  return 0;






/*
  //--------------------- contour ----------------------------
  if((aus_typ[0] == Typ_CI)||(aus_typ[0] == Typ_CI)) {
    sus.cvTyp  = Typ_CI;


  } else if(aus_typ[0] == Typ_CV) {
    sus.cvTyp  = Typ_CV;

    // // decode CCV; nur Test!
    // ox1p = DB_GetCurv ((long)aus_tab[0]);
    // if(ox1p->typ != Typ_CVTRM) goto L_err_par;


  } else goto L_err_par;


  


  //--------------------- Stuetzflaeche ----------------------------
  ox1->typ   = Typ_SUR;
  ox1->form  = Typ_SUR;
  ox1->siz   = 1;
  ox1->data  = (void*)&sus;

  sus.cvID   = aus_tab[0]; // DB-index of Boundary



  //-------------------------------------------------------
  // keine Stuetzflaeche: planar.
  if(aus_anz < 2) {
    sus.typ    = Typ_PLN;

    // eine temporaere Plane erzeugen und deren Index als
    // baseID merken ? Oder nur den Z-Vector davon ?
    sus.baseID = 0; // DB-index of BasicSurface

    goto L_fertig;
  }



  //-------------------------------------------------------
  sus.baseID = aus_tab[1]; // DB-index of BasicSurface

  // Conus, Torus
  if(aus_typ[1] == Typ_SOL) {

    // ox1p = DB_GetSol ((long)aus_tab[1]);
    // sus.typ  = ox1p->typ;

    sus.typ  = Typ_SOL;



  //-------------------------------------------------------
  // SUR (SURRU / SURBSP)
  } else if(aus_typ[1] == Typ_SUR) {

    // ox1p = DB_GetSur ((long)aus_tab[1]);
    // sus.typ  = ox1p->typ;

    sus.typ  = Typ_SUR;








  //-------------------------------------------------------
  } else goto L_err_par;



  //-------------------------------------------------------
  L_fertig:
  // printf(" sur-typ=%d\n",sus.typ);
  return 0;
*/



  L_err_par:
    TX_Error("Parametererror APT_decode_spl");
    return -1;

}


//=============================================================================
  int APT_decode_hat (ObjGX *ox1, int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
// HAT (Hatch)


  static SurHat   hat;

  int      i1;
  ObjGX    *cvp;


/*
  printf("APT_decode_hat %d\n",aus_anz);
  for(i1=0; i1<aus_anz; ++i1) {
    printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  }
*/

  // test curv
  if(aus_typ[1] == Typ_CV) {
    // if(aus_typ[2] != Typ_CV) {TX_Error("Hatch: Par.1 must be CCV"); return -1;}
/*
    // test type of curve
    cvp = DB_GetCurv ((long)aus_tab[1]);
      // printf(" cvp typ=%d form=%d\n",cvp->typ,cvp->form);

    if((cvp->typ != Typ_CVTRM)  &&
       (cvp->typ != Typ_CVPOL)  &&
       (cvp->typ != Typ_CVELL))   goto L_err1;
*/
    hat.cvTyp  = Typ_CV;

  } else if(aus_typ[1] == Typ_CI) {
    hat.cvTyp  = Typ_CI;
 
  } else goto L_err1;


  hat.cvID   = aus_tab[1];
  hat.off    = aus_tab[2];
  hat.dir    = aus_tab[3];


  //------------------------------------------------------
  L_fertig:
  // printf(" r1=%f r2=%f\n",to1.r1,to1.r2);

  ox1->typ   = Typ_SURHAT;
  ox1->form  = Typ_SURHAT;
  ox1->siz   = 1;
  ox1->data  = (void*)&hat;


  // DEB_dump_obj__ (Typ_SURHAT, &hat, "ex APT_decode_hat:\n");


  return 0;

  L_err1:
    TX_Error("Hatch: typ of boundary not yet supported .."); return -1;

}


//=============================================================================
  int APT_decode_fsub (ObjGX *ox1, int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
//

  int     i1, i2;
  long    l1;
  ObjGX   *oxTab;


  // printf("APT_decode_fsub %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  if(aus_anz < 2) return -1;


  oxTab = (ObjGX*)memspc51;

  for(i1=0; i1<aus_anz - 1; ++i1) {
    i2 = i1 + 1;
    oxTab[i1].typ  = aus_typ[i2];
    oxTab[i1].form = Typ_Index;
    oxTab[i1].siz  = 1;
    l1 = aus_tab[i2];
    oxTab[i1].data = PTR_LONG(l1);
  }


  ox1->typ    = Typ_SUR;
  ox1->form   = Typ_ObjGX;
  ox1->siz    = aus_anz - 1;
  ox1->data   = oxTab;

  return 0;

}


//================================================================
  int APT_dec_Par1_Obj (double *po, int aus_typ, double aus_tab,
                       int cvTyp,  long cvInd) {
//================================================================
// APT_dec_Par1_Obj          give parameter 0-1 from curve-obj
//  po = parameter of obj (aus_typ,aus_tab) on DB-curve cvInd
//       parametertype vTyp=1 (always 0-1, see INF_struct_par)


  // void  *oDat;
  Point *pp1;


  // printf("APT_dec_Par1_Obj %d %f %d %d\n",aus_typ,aus_tab,cvTyp,cvInd);


  if(aus_typ == Typ_Val) {
    *po = aus_tab;


  } else if(aus_typ == Typ_PT) {

    pp1 = DB_get_PT ((long)aus_tab);

    // get parameter from point; parameter always 0-1 (vTyp=1, see INF_struct_par)
    UTO_par__pt_dbo (po, 1, pp1, cvTyp, cvInd);


  } else return -1;


  // printf("ex APT_dec_Par1_Obj %f\n",*po);

  return 0;

}


//================================================================
  int APT_dec_Ang_Obj (double *ao, int mode,
                      int *aus_typ, double *aus_tab, Plane *pl1) {
//================================================================
// decode Angle ao from Value or Point or Vector (out als rad)
// mode=0 = gives horizontal-Angle (in X-Y-Plane of pl1, around Z)
// mode=1 = gives vertical-Angle (tilt-angle, in X-Z-Plane of pl1)
//          dz=0, dx=90, diz=180

  int    i1;
  Point  *pt1, ptp;
  Vector *vc1;


  // printf("APT_dec_Ang_Obj %d %d %f\n",mode,*aus_typ,*aus_tab);


    if(*aus_typ == Typ_Val) {
      *ao = UT_RADIANS(*aus_tab);


    } else if(*aus_typ == Typ_Angle) {
      *ao = UT_RADIANS(*aus_tab);


    } else if(*aus_typ == Typ_PT) {
      pt1 = DB_get_PT ((long)*aus_tab);
      // compute angle
      UT3D_angsph_pt (ao, mode, pl1, pt1);


    } else if(*aus_typ == Typ_VC) {
      vc1 = DB_get_VC ((long)*aus_tab);
      if(mode == 0) {
        *ao = UT3D_angr_vcpl_z (&WC_sur_act, vc1);
      } else {
        *ao = UT3D_angr_vcpl_tilt (&WC_sur_act, vc1);
        // change into degrees: dz=0, dx=90, diz=180
        if(*ao >= RAD_270) *ao -= RAD_360;
        if(*ao > RAD_90) *ao -= RAD_180;
        *ao = RAD_90 - *ao;
      }


    } else return -1;


  // printf("ex APT_dec_Ang_Obj %f\n",*ao);

  return 0;

}


//=============================================================================
  int APT_decode_tps_opeur (ObjGX *ox1, Memspc *tbuf1, Memspc *tbuf2, Memspc *tbuf3,
                       int aus_anz, int aus_typ[], double aus_tab[]) {
//=============================================================================
// Surf = CUT Surf Curv|Surf
// tbuf1   use for data for ox1     APTSpcObj   memspc201
// tbuf2   use for temp data        APTSpcTmp   memspc501
// tbuf3   use for temp data                    memspc101  

// see also APT_trim_obj   LN|CI|CV = CUT ..

  
  int     irc, i1;


  printf("APT_decode_tps_opeur %d\n",aus_anz);
  for(i1=0; i1<aus_anz; ++i1) {
    printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  }


  // get ObjGX* from trimCrv
  // DB_GetObjGX


  // Create relimited surface
  // A# = INT A# S# [MOD(#)]
    // Funktionsweise:
    // aktuelle Boundary und IntersectCurve nach 2D;
    // Intersection in 2D - neue 2D-boundary erzeugen.
    // Ev. die boundary als dynamic DB-Obj speichern ?




  return -1;

}


//=============================================================================
  int APT_decode_su_sweep (ObjGX *ox1,
                          int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
// SWEEP-surface SurSwp
// SWP contour path


  int     irc, i1, iDir;
  // double  d1;
  // Point   pt1;
  SurSwp  *swp;
  SurRev  *srv;
  // Plane   *pla, pln;



  // printf("APT_decode_su_sweep %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);

  if(aus_anz < 3) goto L_Err_par;

  swp = (void*)&APT_spc1[0];

  ox1->typ   = Typ_SURSWP;
  ox1->form  = Typ_SURSWP;
  ox1->siz   = 1;
  ox1->data  = swp;

  swp->typCov = aus_typ[1];  // contour
  swp->indCov = aus_tab[1];

  swp->typPath = aus_typ[2];
  swp->indPath = aus_tab[2];

  swp->u0 = 0.;
  swp->u1 = 1.;
  swp->v0 = 0.;
  swp->v1 = 1.;
    // DEB_dump_ox_0 (ox1, "SurSwp");


/*
  // test for cylinder
  if(swp->typCov == Typ_CI) {
    if(swp->typPath == Typ_LN) {
        // printf(" CYL=AC+LN\n");
      return 0;
    // test for torus
    } else if(swp->typPath == Typ_CI) {
        // printf(" TOR=AC+AC-\n");
      return 0;
    // test for bspl
    } else if(swp->typPath == Typ_CV) {
        // printf(" PIP=AC+CV");
      return 0;
    }
  }
  goto L_Err_nyi;
*/

  return 0;

  L_Err_nyi:
    TX_Error(" Definition A=SWP not yet implemented");
    return -1;


  L_Err_par:
    TX_Error(" Definition A=SWP - Parametererror");
    return -1;

}


//=============================================================================
  int APT_decode_srv (ObjGX *ox1, int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
// SRV (RevolvedSurf)
//
// RevolvedSurface from axis and contour.
// contour CANNOT be normal to axis;
//   eg contour of cylinder is line parallel to axis.
// Examples: "SRV L() L() ANG(a1) ANG(a2)"               // cyl or cone
//           "SRV L(%s) C(P(C%d) %s) P(C%d) P(C%d) CW"   // torus
//
// see INF_FMTB_Surface_RV



  int     irc, i1, iDir, oTyp, axTyp;
  double  d1;
  long    oDbi, axDbi;
  Point   pt1;
  SurRev  *srv;
  Plane   pln;



  // printf("APT_decode_srv %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }

  if(aus_anz < 3) goto L_parErr;

  // prepare static memspc for srv
  srv = (void*)&APT_spc1[0];


  //----------------------------------------------------------------
  // get iDir; get/remove last word if it is "CW"
  iDir = 0;  // CCW
  if(aus_typ[aus_anz - 1] == Typ_cmdNCsub) {
    if(aus_tab[aus_anz - 1] == T_CW) {
      iDir = 1;  // CW
      --aus_anz;
    } else {
      goto L_parErr;
    }
  }


  //----------------------------------------------------------------
  // change atomic-obj (aus_typ[1],aus_tab[1] (Typ_goAxis)) into DB-plane;
  // get (Plane) pln and DB-plane in (aus_typ[1],aus_tab[1])
  // pln.vz = rotAxis
  irc = APT_decode_goAxis (&pln, &aus_typ[1], &aus_tab[1]);
  if(irc < 0) goto Error;


  //----------------------------------------------------------------
  axTyp = aus_typ[1];   // typ of axis
  axDbi = aus_tab[1];   // Ind of axis
  oTyp  = aus_typ[2];   // typ of contourelement
  oDbi  = aus_tab[2];   // Ind of contourelement


  // if contourelement is trimmed-curve: change to standard-curve
  irc = DBO_cvStd_cvTrm (&oTyp, &oDbi);
  if(irc < 0) return irc;



  //----------------------------------------------------------------
  srv->typCen = axTyp;        // typ of Centerline
  srv->indCen = axDbi;        // Ind of Centerline (Typ = Typ_LN)

  srv->typCov = oTyp;         // typ of contourelement
  srv->indCov = oDbi;         // Ind of contourelement

  srv->dir = iDir;


  //----------------------------------------------------------------
  // rotate Refsys pln - x-axis goes tru midPoint of contourelement

  // get pt1 = midpoint of contourelement
  UTO_pt_eval_par1_dbo (&pt1, 0.5, srv->typCov, srv->indCov);
    // GR_tDyn_symB__ (&pt1, SYM_STAR_S, 2);

  // rot. Refsys around axis; point pt1 gives new X-direction
  UT3D_pl_rotpt (&pln, &pln, &pt1);
    // GR_Disp_pln (&pln, 9);
    // GR_tDyn_vc__ (&pla->vx, &pla->po, 9, 0);


  //----------------------------------------------------------------
  // get Angle from value/Point
  if(aus_anz > 3) {
    APT_dec_Ang_Obj (&d1, 0, &aus_typ[3], &aus_tab[3], &pln);
    // srv->ang1 = UT_DEGREES (d1);
    srv->ang1 = d1;
  } else
    srv->ang1 = 0.;


  //----------------------------------------------------------------
  // get Angle from value/Point
  if(aus_anz > 4) {
    APT_dec_Ang_Obj (&d1, 0, &aus_typ[4], &aus_tab[4], &pln);
    // srv->ang2 = UT_DEGREES (d1);
    srv->ang2 = d1;
  } else
    srv->ang2 = RAD_360;

  // printf(" a1=%f a2=%f\n",srv->ang1,srv->ang2);


    // printf(" vor: a1=%f a2=%f\n",srv->ang1,srv->ang2);
  // if(fabs(srv->ang1 - srv->ang2) < UT_TOL_pt) srv->ang2 += RAD_360;
    // printf(" a1=%f a2=%f\n",srv->ang1,srv->ang2);



  //----------------------------------------------------------------
  // parameter on curve; defaults = 0., 1.;
  if(aus_anz > 5) {
    i1 = APT_dec_Par1_Obj (&srv->v0, aus_typ[5], aus_tab[5],
                          srv->typCov, srv->indCov);
    if(i1 < 0) goto Error;
  } else {
    // if(srv->typCov == Typ_CV) srv->v0 = UT_DB_LEER;  // mark as Default
    // else
      srv->v0 = 0.;   // 2011-12-22 SRV-from-Polygon
  }


  if(aus_anz > 6) {
    i1 = APT_dec_Par1_Obj (&srv->v1, aus_typ[6], aus_tab[6],
                          srv->typCov, srv->indCov);
    if(i1 < 0) goto Error;
  } else {
    // if(srv->typCov == Typ_CV) srv->v1 = UT_DB_LEER;  // mark as Default
    // else
      srv->v1 = 1.;   // 2011-12-22 SRV-from-Polygon
  }

  if(srv->v1 < srv->v0) MEM_swap_2db (&srv->v0, &srv->v1);
 

  ox1->typ   = Typ_SURRV;
  ox1->form  = Typ_SURRV;
  ox1->siz   = 1;
  ox1->data  = srv;

    // DEB_dump_obj__ (Typ_SURRV, srv, "ex-APT_decode_srv");

  return 0;

  Error:
    TX_Error(" Definition A=SRV not implemented");
    return -1;

  L_parErr:
    TX_Error(" Definition A=SRV - Parametererror");
    return -1;

}


//=============================================================================
  int APT_decode_s_sph (ObjGX *ox1,int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
// create standard Typ_SURRV - Record; mit 180-Grad-circ als Kontur,
  // A = SPH R radius [ang1 ang2 [par1 par2]]

  int     irc, i1;
  long    l1;
  double  rdc, d1;
  Point   pt1, *pp1;
  Vector  vcz, vcx, vc1;
  SurRev  *srv;
  Circ    *ci1;
  Line    *lna;
  Plane   *pla, pln;



  // printf("\nAPT_decode_s_sph |%d|\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }



  // prepare memspc for srv & ci1
  srv = (void*)&APT_spc1[0];
  ci1 = (void*)&APT_spc1[sizeof(SurRev)]; // liegt hinter srv


  // aus_[0] = "SPH"

  // decode Typ_goAxis
  irc = APT_decode_goAxis (&pln, &aus_typ[1], &aus_tab[1]);
  if(irc < 0) goto Error;
  pla = &pln;

  // save the refPlane
  srv->indCen = aus_tab[1];   // Ind of CenterObj (Plane)
  srv->typCen = aus_typ[1];



  // decode Typ_goRadius
  irc = APT_decode_goRadius (&rdc, 2, &aus_typ[2], &aus_tab[2], &pln);
  if(irc < 0) goto Error;




  //----------------------------------------------------------------
  // create a (dynamic) 180-Deg-Circ as Contourelment; add its typ & index.
  // Der Circ immer out als 180-Grad Circ !
  // UT3D_vc_invert (&vcz, &pla->vy); // Circ-Axis = iY.
  vcz = pla->vy;   // Circ-Axis = Y.
  // UT3D_vc_invert (&vcx, &pla->vz); // Circ-vx = iZ.
  vcx = pla->vz;   // Circ-Axis = Y.
  UT3D_ci_pt2vcr (ci1, &pla->po, &vcz, &vcx, rdc);
  UT3D_ci_cipt180 (ci1, &ci1->p1);  // change -> 180 deg
    // GR_tDyn_symB__ (&ci1->p1, SYM_STAR_S, 2);



  //----------------------------------------------------------------
  // get Angle from value/Point
  if(aus_anz > 3) {
    APT_dec_Ang_Obj (&d1, 0, &aus_typ[3], &aus_tab[3], pla);
    // srv->ang1 = UT_DEGREES (d1);
    srv->ang1 = d1;
  } else
    srv->ang1 = 0.;


  //----------------------------------------------------------------
  // get Angle from value/Point
  if(aus_anz > 4) {
    APT_dec_Ang_Obj (&d1, 0, &aus_typ[4], &aus_tab[4], pla);
    // srv->ang2 = UT_DEGREES (d1);
    srv->ang2 = d1;
  } else
    srv->ang2 = RAD_360;

  // printf(" a1=%f a2=%f\n",srv->ang1,srv->ang2);



  //----------------------------------------------------------------
  // Inputwerte fuer v0/v1:
  // Input: Angles in degree;
  // Output: Parameter fuer den 180-deg-Circ (0-1)

  if(aus_anz < 7) {
    srv->v0 = 0.;
    srv->v1 = 1.;
    goto L_done;
  }


  // srv->v0 = UT_RADIANS (aus_tab[5]) / RAD_180;
  APT_dec_Ang_Obj (&d1, 1, &aus_typ[5], &aus_tab[5], pla);
    // printf(" v[5]=%lf\n",d1);
  srv->v0 = d1 / RAD_180;

  // srv->v1 = UT_RADIANS (aus_tab[6]) / RAD_180;
  APT_dec_Ang_Obj (&d1, 1, &aus_typ[6], &aus_tab[6], pla);
    // printf(" v[6]=%lf\n",d1);
  // max 180 deg !
  if(d1 > RAD_180) d1 = RAD_180;
  srv->v1 = d1 / RAD_180;

  if(srv->v1 < srv->v0) MEM_swap_2db (&srv->v0, &srv->v1);


/* alte Version; hat den Circ korrigiert !!!!
  // Vertikale Endpunkte am Konturkreis sofort korrigieren.
  // ZUERST den Endpunkt (es wird der Startpunkt gedreht !)
  if(aus_anz > 6) {
    if(aus_typ[6] == Typ_Val) {
      srv->v1 = UT_RADIANS (aus_tab[6]);

    } else if (aus_typ[6] == Typ_PT) {
      pp1 = DB_get_PT ((long)aus_tab[6]);
      // oeffnungswinkel errechnen
      UT3D_vc_2pt (&vc1, &ci1->pc, pp1);
      srv->v1 = fabs(UT3D_angr_3vc__ (&vcz, &vcx, &vc1));
    }

    if(srv->v1 < 0.) goto L_err_p;
    // den Endpunkt p2 des Circ ci1 aendern; Angle ist d1
    d1 = srv->v1;
    if(UTP_comp2db(d1, RAD_180 ,UT_TOL_pt) == 0) {
      UT3D_pt_rotciangr (&pt1, d1, ci1);
      ci1->p2 = pt1;
      ci1->ango -= RAD_180 - d1;
    }
    srv->v1 = 1.; // der Circ ist schon korrigiert !

  } else
    srv->v1 = 1.;

  if(aus_anz > 5) {
    if(aus_typ[5] == Typ_Val) {
      srv->v0 = UT_RADIANS (aus_tab[5]);

    } else if (aus_typ[5] == Typ_PT) {
      pp1 = DB_get_PT ((long)aus_tab[5]);
      // oeffnungswinkel errechnen
      UT3D_vc_2pt (&vc1, &ci1->pc, pp1);
      srv->v0 = fabs(UT3D_angr_3vc__ (&vcz, &vcx, &vc1));
    }

    // den Startpunkt p1 des Circ ci1 aendern; Angle ist d1
    d1 = srv->v0;
    if(fabs(d1) > UT_TOL_pt) {
      UT3D_pt_rotciangr (&pt1, d1, ci1);
      ci1->p1 = pt1;
      ci1->ango -= d1;
    }
    srv->v0 = 0.; // der Circ ist schon korrigiert !

  } else
    srv->v0 = 0.;
*/

  // printf(" v1=%f v2=%f\n",srv->v0,srv->v1);



  //----------------------------------------------------------------
  // save dynam circle
  L_done:
  l1 = DB_StoreCirc (-1L, ci1);
    // DEB_dump_obj__ (Typ_CI, ci1, "_s_sph-ci1:");
    // GR_Disp_ac (ci1, 12);

  srv->indCov = l1;           // Ind of contourelement
  srv->typCov = Typ_CI;       // typ of contourelement


  ox1->typ   = Typ_SURRV;
  ox1->form  = Typ_SURRV;
  ox1->siz   = 1;
  ox1->data  = srv;



  Fertig:
    // DEB_dump_obj__ (Typ_SURRV, srv, "ex APT_decode_s_sph\n");
  return 0;



  Error:
  TX_Error(" Definition A=SPH not implemented");
  return -1;


  L_err_p:
  TX_Error(" Definition A=SPH - Parametererror");
  return -1;

}


//=============================================================================
  int APT_decode_s_cyl (ObjGX *ox1,int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
// APT_decode_s_cyl                decode cylindirc surface; out as SurRev
// inputs  aus_typ,aus_tab:
//           refsys, radius, ang1, ang2, dist1, dist2
// output  SurRev:
//           refsys, contourline, ang1, ang2, par1, par2
//           par1,par2 = parameters on contourline

// A = CYL axis radius rotAngle1 rotAngle2 height1 height2
//    Axis:     The main axis of the cylinder; line, vector or plane (z-axis).
//    Radius:   KeyIn radius or sel point.
//    rotAngle1 KeyIn launch angle (Def = 0) or sel point.
//    rotAngle2 KeyIn End piece (Def = 360) or sel point.
//    height1   KeyIn elevation (from Achsstartpunkt; Def = 0) or sel point.
//    height2   KeyIn elevation (from Achsstartpunkt) or sel point.


  int     irc, i1;
  long    l1;
  double  rdc, d1;
  SurRev  *srv;
  Point   pt1, pt2;
  Line    lnCo, *lna;  //, *ln1, *lna;
  Circ    *ci1;
  Plane   *pla, pln;



  // printf("\nAPT_decode_s_cyl |%d|\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }
  // DEB_dump_dbo (aus_typ[1], (long)aus_tab[1], "");


  // prepare memspc for srv  pt1  li1  pt2
  srv = (void*)&APT_spc1[0];  // Startposi der srv
  i1  = sizeof(SurRev);


  // get pln = plane from inputs aus_typ[1],aus_tab[1]
  irc = APT_decode_goAxis (&pln, &aus_typ[1], &aus_tab[1]);
  if(irc < 0) goto Error;
  pla = &pln;


  // get rdc = radius from aus_typ[2],aus_tab[2]
  irc = APT_decode_goRadius (&rdc, 1, &aus_typ[2], &aus_tab[2], &pln);
  if(irc < 0) goto Error;


  // set defaults
  srv->ang1 = 0.;
  srv->ang2 = RAD_360;
  srv->v0   = 0.;
  srv->v1   = 1.;


  //----------------------------------------------------------------
  // get ang1 from value/Point
  if(aus_anz > 3)
    APT_dec_Ang_Obj (&srv->ang1, 0, &aus_typ[3], &aus_tab[3], pla);

  // get ang2 from value/Point
  if(aus_anz > 4)
    APT_dec_Ang_Obj (&srv->ang2, 0, &aus_typ[4], &aus_tab[4], pla);

  if(fabs(srv->ang1-srv->ang2) < UT_TOL_pt) srv->ang2 += RAD_360;
    // printf(" a1=%f a2=%f\n",srv->ang1,srv->ang2);



  //----------------------------------------------------------------
  // set DBO CenterObj (Plane)
    srv->indCen = aus_tab[1];   // Ind of CenterObj (Plane)
    srv->typCen = aus_typ[1];


  //----------------------------------------------------------------
  // create lnCo = a dynam Line - parallel to line lnAx;
  //   offset from lnAx is rdc;
  //   startpoint is in plane at x-axis
  //  length is 1;

  // lnCo->p1 = from pla->po along pla->vx dist = rdc
  UT3D_pt_traptvclen (&lnCo.p1, &pla->po, &pla->vx, rdc);  // am Mantel;

  // lnCo->p2 = from ln1->p1 along pla->vz dist 1.
  // UT3D_pt_traptvclen (&lnCo->p2, &ln1->p1, &pla->vz, dz);
  lnCo.p2 = lnCo.p1;
  UT3D_pt_add_vc__ (&lnCo.p2, &pla->vz);

  l1 = DB_StoreLine (-1L, &lnCo);
    // DEB_dump_obj__ (Typ_LN, &lnCo, "decode_s_cyl-lnCo\n");

  // set DBO contourelement (line)
  srv->indCov = l1;           // Ind of contourelement
  srv->typCov = Typ_LN;       // typ of contourelement


  //==================================================================
  // Inputwerte fuer v0/v1:
  // Zahlenwerte: sind die Laenge entlang der Z-Achse;
  //   umrechnen in Parameterwerte.


  // get v0 = startParameter on contourelement
  if(aus_anz > 5) {

    if(aus_typ[5] == Typ_Val) {
      // UT3D_pt_traptptlen (pt1, &ln1->p1, &ln1->p2, aus_tab[5]);
      srv->v0 = aus_tab[5];

    } else if (aus_typ[5] == Typ_PT) {
      pt2 = DB_GetPoint ((long)aus_tab[5]);
      // pt1 = project point pt2 --> line ln1
      UT3D_pt_projptln (&pt1, NULL, NULL, &pt2, &lnCo);
      // get parameter of pt1 on lnCo.
      UT3D_parpt_ln__ (&srv->v0, &pt1, &lnCo);

    } else goto L_err1;
  }


  // get v1 = endParameter on contourelement
  if(aus_anz > 6) {

    if(aus_typ[6] == Typ_Val) {
      // UT3D_pt_traptptlen (pt1, &ln1->p1, &ln1->p2, aus_tab[6]);
      srv->v1 = aus_tab[6];

    } else if (aus_typ[6] == Typ_PT) {
      pt2 = DB_GetPoint ((long)aus_tab[6]);
      // pt1 = project point pt2 --> line ln1
      UT3D_pt_projptln (&pt1, NULL, NULL, &pt2, &lnCo);
      // get parameter of pt1 on ln1
      UT3D_parpt_ln__ (&srv->v1, &pt1, &lnCo);

    } else goto L_err1;
  }



  //----------------------------------------------------------------
  ox1->typ   = Typ_SURRV;
  ox1->form  = Typ_SURRV;
  ox1->siz   = 1;
  ox1->data  = srv;

    // DEB_dump_obj__ (Typ_SURRV, srv, "ex APT_decode_s_cyl\n");

  return 0;


  //----------------------------------------------------------------
  Error:
  TX_Error(" Definition A=CYL not implemented");
  return -1;

  L_err1:
  TX_Error("APT_decode_s_cyl parameter-error");
  return -1;


}


//=============================================================================
  int APT_decode_s_con (ObjGX *ox1,int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================

  int   i1;

  printf("\nAPT_decode_s_con |%d|\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  return 0;

}


//================================================================
  int APT_decode_goAxis (Plane *pln, int *aus_typ, double *aus_tab) {
//================================================================
// decode Axis, store as DB-plane; provide Plane in aus_typ & aus_tab.
// if input==Line: create plane, store it as dynamic plane.

  int     typ;
  long    l1, ind;
  Point   *pt1;
  Line    *lna;
  Circ    *ci1;


  typ = *aus_typ;
  ind = (long)(*aus_tab);

  // printf("APT_decode_goAxis %d %d\n",typ,ind);

  if(typ == Typ_PT) {
    pt1 = DB_get_PT (ind);
    *pln = WC_sur_act;
    UT3D_pl_ptpl (pln, pt1);


  } else if(typ == Typ_LN) {
    lna = DB_get_LN (ind);
      // DEB_dump_obj__ (Typ_LN, lna, " ln in APT_decode_goAxis");
    UT3D_pl_perp_ln (pln, lna);


  } else if(typ == Typ_CI) {
    ci1 = DB_get_CI (ind);
    UT3D_pl_ci (pln, ci1);


  } else if(typ == Typ_PLN) {
    DB_GetRef (pln, ind);
    // typ & tab schon OK; pln already in DB ..
    goto L_done;


  } else return -1;



  l1 = DB_StoreRef (-1L, pln);
  *aus_typ = Typ_PLN;
  *aus_tab = (double)l1;

  L_done:
    // DEB_dump_obj__ (Typ_PLN, pln, "ex APT_decode_goAxis");
  return 0;

}


//=======================================================================
  int APT_decode_modUnlim (int *aus_anz, int *aus_typ, double *aus_tab) {
//=======================================================================
/// \code
/// APT_decode_modUnlim      get UNL=3|UNL2=2|UNL1=1
/// retCod   0 (limited)
///          1 (UNL1 = side 1 limited)
///          2 (UNL2 = side 2 limited)
///          3 (UNL  = both sides unlimited
/// \endcode

  int  i1, lTyp;

  i1 = *aus_anz - 1;

  lTyp = 0;  // both sides limited

  if(aus_typ[i1] == Typ_cmdNCsub)     {
    if((int)aus_tab[i1] == T_UNL) {
      *aus_anz = i1;
      lTyp = 3;  // both sides unlimited
    } else if((int)aus_tab[i1] == T_UNL1) {
      *aus_anz = i1;
      lTyp = 1;  // both sides unlimited
    } else if((int)aus_tab[i1] == T_UNL2) {
      *aus_anz = i1;
      lTyp = 2;  // both sides unlimited
    } 
  }
    // printf("APT_decode_modUnlim lTyp=%d\n",lTyp);

  return lTyp;

}


//=========================================================================
  int APT_decode_goRadius (double *rdc,  int mode,
                           int *aus_typ, double *aus_tab, Plane *pln) {
//=========================================================================
// decode Radius;
// mode: 1 Input PT: Radius = abstand PT - Achse pln->vz;
// mode: 2 Input PT: Radius = abstand PT - pln->po;


  int     typ;
  long    ind;
  Point   *pp1, pt1;
  Circ    *ci1;


  typ = *aus_typ;


  if(typ == Typ_Val) {
    *rdc = *aus_tab;


  } else if (typ == Typ_PT) {
    ind = (long)(*aus_tab);
    pp1 = DB_get_PT (ind);
    if(mode == 1) {
      UT3D_pt_projptptvc (&pt1, rdc, NULL, pp1, &pln->po, &pln->vz);
    } else if(mode == 2) {
      *rdc = UT3D_len_2pt (pp1, &pln->po);
    }


  } else if(typ == Typ_CI) {
    ind = (long)(*aus_tab);
    ci1 = DB_get_CI (ind);
    *rdc = fabs(ci1->rad);


  } else return -1;


  return 0;

}


/*
//=============================================================================
  int APT_dec_sursrv (ObjGX *ox1) {
//=============================================================================
// create standard Typ_SURRV - Record;


  ox1->typ   = Typ_SURRV;
  ox1->form  = Typ_SURRV;
  ox1->siz   = 1;
  ox1->data  = (void*)&srv;


  return 0;

}
*/


//=============================================================================
  int APT_decode_sru (ObjGX *ox1, int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
// SRU (RuledSurf)
// Input:
//   atomic-obj1   contour
//   atomic-obj2   vector
// Output:
//   ox1           RuledSurf Typ_SURRU
//   
// "A20=SRU C20 D(0 0 16)"


static ObjGX   oxa[2];

  int       irc, i1, oTyp, vTyp;
  long      l1, oDbi, vDbi;
  ObjGX     *o1, *o2;


  // printf("APT_decode_sru %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  oTyp  = aus_typ[1];   // typ of contourelement
  oDbi  = aus_tab[1];   // Ind of contourelement
  vTyp  = aus_typ[2];   // typ of vector
  vDbi  = aus_tab[2];   // Ind of vector

  // if contourelement is trimmed-curve: change to standard-curve
  irc = DBO_cvStd_cvTrm (&oTyp, &oDbi);
  if(irc < 0) return irc;



  //----------------------------------------------------------------
  // set contour
  OGX_SET_INDEX (&oxa[0], oTyp, oDbi);

  // set vector
  OGX_SET_INDEX (&oxa[1], vTyp, vDbi);

  // get reverse-direction
  if((aus_anz == 4)                &&
    ((aus_typ[3] == Typ_cmdNCsub)||(aus_typ[3] == Typ_modif))  &&
     (aus_tab[3] == T_CW))           {           // Typ_modCW
    // printf(" Obj 2 verkehrt !\n");
    // ox1->dir = 1;
    oxa[1].dir  = 1;
  }


  // set RuledSurf
  OGX_SET_OBJ (ox1, Typ_SURRU, Typ_ObjGX, 2, (void*)oxa);


    // TESTBLOCK
    // DEB_dump_ox_0 (ox1, "ex-APT_decode_sru");  // full decode ..
    // DEB_dump_ox_s_ (ox1, "APT_decode_sru");
    // o1 = ox1->data; o2 = &o1[0];
    // printf("  SurRu 0 typ=%d ind=%d\n",o2->typ,(long)o2->data);
    // o2 = &o1[1];
    // printf("  SurRu 1 typ=%d ind=%d\n",o2->typ,(long)o2->data);
    // END TESTBLOCK


  return 0;

}


//=============================================================================
  int APT_decode_sbsp (ObjGX *ox1, Memspc *tbuf1, Memspc *tbuf2, Memspc *tbuf3,
                       int aus_anz, int aus_typ[], double aus_tab[]) {
//=============================================================================
// create surface from B-Spline-Curves
// RetCod:
//   -99        curves do not end on surface - boundaries


  int      irc, iu1, iu2, i1, i2, i3, typ, cv1Nr, cv2Nr, deg1, deg2;
  long     ind;
  CurvBSpl **cv1tab, **cv2tab;
  SurBSpl  *su1;

  int      fTabNr, fTabSiz;
  double   *fTab;


  // printf("================================================ \n");
  // printf("APT_decode_sbsp: %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  // reserve space for output-surf su1 in tbuf1
  // su1 = tbuf1->next;
  su1 = UME_reserve (tbuf1, sizeof(SurBSpl));
  if(su1 == NULL) return -1;


  // check for group in Input
  if(aus_typ[1] == Typ_Val) goto L_direct;
  goto L_curves;



  //================================================================
  // BSP direct-values
  L_direct:

  // zuerst alle variablen in eine tabelle fTab kopieren
  fTab = (void*) tbuf1->next;
  fTabSiz = UME_ck_free(tbuf1) / sizeof(double);
  fTabNr  = 0;


  irc = APT_decode_ValTab (fTab, &fTabNr, fTabSiz, aus_anz, aus_typ, aus_tab);
  if(irc < 0) return irc;
    // printf(" pt1/2Nr=%f,%f deg1/2=%f,%f\n",fTab[0],fTab[1],fTab[2],fTab[3]);



  // fill
  su1->ptUNr  = fTab[0];
  su1->ptVNr  = fTab[1];
  su1->degU   = fTab[2];
  su1->degV   = fTab[3];
  su1->cpTab  = (Point*)&fTab[4];

  i1 = 4 + (su1->ptUNr * su1->ptVNr * 3);
  su1->kvTabU = &fTab[i1];

  i2 = su1->ptUNr + su1->degU + 1;           // Anzahl U-Knots
  su1->kvTabV = &fTab[i1+i2];

  goto L_done;





  //================================================================
  // BSP U(Curves) [deg]
  // Surfcae from 2-n Querschnittskurven: SkinnedSurface; exact.
  // exact: es werden die Knotentabellen aller curves exact gleich gemacht
  //   (sehr hoher Rechenaufwand)
  // max 19 curves.
  L_curves:
    // printf(" L_curves:\n");


  if(aus_typ[1] == Typ_Group) {         // group of curves
    cv1Nr = aus_tab[1];
    if(cv1Nr > 19) goto Err_2;
    if(cv1Nr < 1) return -1;
    iu1 = 2;
    i1 = iu1 + cv1Nr;

  } else if(aus_typ[1] == Typ_CV) {     // 1. Group = single curve
    cv1Nr = 1;
    iu1 = 1;
    i1 = 2;

  } else goto Par_err;



  L_curvg1:
  if((aus_typ[i1] == Typ_Group) ||
     (aus_typ[i1] == Typ_CV))        goto L_2curves;

  if(i1 > aus_anz) {
    deg1 = aus_tab[i1];
    ++i1;
  } else {
    deg1 = 0;
  }


    // TEST
    // for(i1=0; i1<cv1Nr; ++i1)
    // printf(" G1[%d] typ=%d ind=%d\n",i1,aus_typ[i1+iu1],(int)aus_tab[i1+iu1]);


  // reserve space for cv1tab inside tbuf1
  // cv1tab = tbuf1->next;
  cv1tab = UME_reserve (tbuf1, cv1Nr * sizeof(void*));   //CurvBSpl));
  if(cv1tab == NULL) return -1;


  // put 1. group of curves > cv1tab
  for(i1=0; i1<cv1Nr; ++i1) {
    i2 = iu1 + i1;
    typ = aus_typ[i2];
    ind = aus_tab[i2];
      // printf(" G1[%d] typ=%d ind=%d\n",i1,typ,ind);
    // add lfig(LN/CI/CV) (typ,ind) as cbsp to cv1tab
    irc = UT3D_cbsp_dbo (&cv1tab[i1], tbuf1, typ, ind, tbuf3);
    if(irc < 0) return irc;
      // DEB_dump_obj__ (Typ_CVBSP, cv1tab[i1], "U1[%d]",i1);
  }

  // orient B-SplCurves cv1tab (same direction)
  i1 = UT3D_ncvbsp_orient (cv1Nr, cv1tab, tbuf1);
  if(i1 < 0) return -1;


  // create bspl-surf from 2-n across-curves
  irc = UT3D_sbsp_ncv (su1, tbuf1,
                  cv1Nr, cv1tab, deg1,
                  tbuf2, tbuf3);
  if(irc < 0) return irc;

  goto L_done;





  //================================================================
  // BSP U(Curves) U(Curves) [deg1] [deg2]
  // GordonSurface;
  // braucht 2 Basiflaechentypen:
  // - SkinnedSurface
  //     (Flaeche nur aus Querschnittskurven) und
  // - punktinterpolierende Flaeche
  //     (spannt eine Flaeche ueber einen Punkteraster)
  // 
  L_2curves:
    // printf(" L_2curves:\n");

  if(aus_typ[i1] == Typ_Group) {         // group of curves
    cv2Nr = aus_tab[i1];
    if(cv2Nr > 19) goto Err_2;
    iu2 = i1 + 1;
    i1 = iu2 + cv2Nr;

  } else if(aus_typ[i1] == Typ_CV) {     // a single curve
    cv2Nr = 1;
    iu2 = i1;
    i1 = iu2 + 1;

  } else goto Par_err;
    // printf("iu1=%d cv1Nr=%d iu2=%d cv2Nr=%d i1=%d\n",iu1,cv1Nr,iu2,cv2Nr,i1);

  if(i1 > aus_anz) {
    deg1 = aus_tab[i1];
    ++i1;
  } else {
    deg1 = 0;
  }

  if(i1 > aus_anz) {
    deg2 = aus_tab[i1];
    ++i1;
  } else {
    deg2 = 0;
  }


  //----------------------------------------------------------------
  // reserve space for cv1tab & cv2tab inside tbuf1
  // Minimum space is for 2 curves !!
  i1 = IMAX(cv1Nr, 2);
  cv1tab = UME_reserve (tbuf1, i1 * sizeof(void*));   //CurvBSpl));
  if(cv1tab == NULL) return -1;

  i1 = IMAX(cv2Nr, 2);
  cv2tab = UME_reserve (tbuf1, i1 * sizeof(void*));   //CurvBSpl));
  if(cv2tab == NULL) return -1;


  // put 1. group of curves > cv1tab
  for(i1=0; i1<cv1Nr; ++i1) {
    i2 = iu1 + i1;
    typ = aus_typ[i2];
    ind = aus_tab[i2];
    // change points, lines -> bsp-curves
    irc = UT3D_cbsp_dbo (&cv1tab[i1], tbuf1, typ, ind, tbuf3);
    if(irc < 0) return irc;
      // printf(" U1[%d] typ=%d ind=%d\n",i1,typ,ind);
      // DEB_dump_obj__ (irc, cv1tab[i1], "U1[%d]",i1);
  }


  // put 2. group of curves > cv2tab
  for(i1=0; i1<cv2Nr; ++i1) {
    i2 = iu2 + i1;
    typ = aus_typ[i2];
    ind = aus_tab[i2];
    // change points, lines -> bsp-curves
    irc = UT3D_cbsp_dbo (&cv2tab[i1], tbuf1, typ, ind, tbuf3);
    if(irc < 0) return irc;
      // printf(" U2[%d] typ=%d ind=%d\n",i1,typ,ind);
      // DEB_dump_obj__ (irc, cv2tab[i1], "U2[%d]",i1);
  }


  //----------------------------------------------------------------
  // single-curve: add a second (degenerated) curve
  if(cv1Nr < 2) {
    irc = UT3D_4cvbsp_3cvbsp (cv1tab, cv2tab, tbuf1);
    if(irc < 0) goto Err_3;
    cv1Nr = 2;
    // MEM_swap_2vp (cv1tab, cv2tab);
    goto L_sdraw;

  } else if(cv2Nr < 2) {
    irc = UT3D_4cvbsp_3cvbsp (cv2tab, cv1tab, tbuf1);
    if(irc < 0) goto Err_3;
    cv2Nr = 2;
    goto L_sdraw;

  }


  //----------------------------------------------------------------
  // orient 2 groups of curves for surf
    // printf(" - orient curves 1+2 ..\n");
  i1 = UT3D_2ncvbsp_orient (cv1Nr, cv1tab, cv2Nr, cv2tab, tbuf1);
  if(i1 < 0) return -1;



  L_sdraw:
  // create bspl-surf from 2 groups of curves
  irc = UT3D_sbsp_2ncv (su1, tbuf1, 
                        cv1Nr, cv1tab, deg1, cv2Nr, cv2tab, deg2,
                        tbuf2, tbuf3);
  if(irc < 0) return irc;



  //================================================================
  L_done:

    // DEB_dump_obj__ (Typ_SURBSP, su1, "" );
    // printf("ex APT_decode_sbsp irc=%d\n",irc);


  ox1->typ   = Typ_SURBSP;
  ox1->form  = Typ_SURBSP;
  ox1->siz   = 1;
  ox1->data  = su1;


  return irc;



  //================= Fehler =============================
  Par_err:
    TX_Error("BsplSurfDefinition: ParameterError ****");
    goto Error;

  Err_2:
    TX_Error("BsplSurf: max 19 curves ****");
    goto Error;

  Err_3:
    TX_Print("**** BsplSurf: Error CoonsPatchSurf - add curve ****");


  Error:
  return -1;


}


//======================================================================
  int APT_decode_msh_p (ObjGX *ox1, long dbi,
                        int aus_anz, int aus_typ[], double aus_tab[]) {
//======================================================================
// A1=PTAB "filNam"
// eigentlich nix tun; dummy ..


  // printf("APT_decode_msh_p %d\n",aus_anz);
  // { int i1; for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]); }}

  

//   ox1->typ    = Typ_SURPTAB;   // dummyMode (DB_StoreSur)
//   ox1->form   = Typ_NULL;
//   ox1->siz    = 0;
//   ox1->data   = NULL;

  // create ObjGX for DB
  OGX_SET_INDEX (ox1, Typ_SURPTAB, dbi);

  return 0;

}


//======================================================================
  int APT_decode_msh__ (ObjGX *ox1,
                        int aus_anz, int aus_typ[], double aus_tab[]) {
//======================================================================
// create Mesh from PTAB.
// Inputs:
//   - "MSH"
//   -  pTab-Surface

  long     surNr;


  // printf("APT_decode_msh__ %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  surNr = aus_tab[1];


  // create ObjGX for DB
  OGX_SET_INDEX (ox1, Typ_SURPMSH, surNr);


  return 0;

}


//=============================================================================
  int APT_decode_suStrip (ObjGX *ox1, int aus_anz,int aus_typ[],double aus_tab[],
                         Memspc *wrkSpc) {
//=============================================================================
// tesselated surfaces - Typ_SURSTRIP


  int       irc, i1, sTyp, uNr, vNr;
  long      dbi, *ia;
  ObjGX     *oa;
  SurStripe *sus;



  // printf("APT_decode_suStrip %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  // first obj is typ; Typ_SURSTRIP
  sTyp = aus_tab[0];
    // printf(" sTyp=%d\n",sTyp);  // T_RSTRIP
  --aus_anz;
  ++aus_typ;
  ++aus_tab;


  // second obj is nr-of-points-per-row;
  uNr = aus_tab[0];
  --aus_anz;
  ++aus_typ;
  ++aus_tab;
  vNr = aus_anz / uNr;
    // printf(" uNr=%d vNr=%d aus_anz=%d\n",uNr,vNr,aus_anz);


  // get space for SurStripe
  sus = UME_reserve (wrkSpc, sizeof(SurStripe));
  if(sus == NULL) return -1;


  // get space for aus_anz complex objects
  oa = UME_reserve (wrkSpc, sizeof(ObjGX) * uNr * vNr );
  if(oa == NULL) return -1;


  // all following objs must be db-points;
  for(i1=0; i1<aus_anz; ++i1) {
    dbi = aus_tab[i1];
    OGX_SET_INDEX (&oa[i1], Typ_PT, dbi);
  }


  sus->ptUNr  = uNr;
  sus->ptVNr  = vNr;
  sus->pTab   = oa;


  ox1->typ    = Typ_SURSTRIP;
  ox1->form   = Typ_SURSTRIP;
  ox1->siz    = 1;
  ox1->data   = sus;


    //---- Anf Testausg:
    // printf(" ex APT_decode_suStrip RSTRIP %d\n",aus_anz);
    // DEB_dump_ox_s_ (ox1, "RSTRIP");
    // DEB_dump_ox_0 (ox1, "RSTRIP");
    //---- End Testausg:


  return 0;

}


//=============================================================================
  int APT_decode_suCir (ObjGX *ox1, int aus_anz, int aus_typ[], double aus_tab[],
                         Memspc *wrkSpc) {
//=============================================================================
// tesselated surfaces - Typ_SURCIR


  int      irc, i1, sTyp, oNr;
  long     dbi;
  ObjGX    *oa;


  // printf("APT_decode_suCir %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }



  // first obj is typ; Typ_SURCIR
  sTyp = aus_tab[0];
    // printf(" sTyp=%d\n",sTyp);  // T_RCIR
  --aus_anz;
  ++aus_typ;
  ++aus_tab;


/*
  // change aus_typ/aus_tab into ato
  ATO_ato_typTab (&ato, aus_anz, aus_typ, aus_tab);
    ATO_dump__ (ato, "after ATO_ato_typTab");


  // get all objects out of aus_typ,aus_tab as db-objects into oa
  irc = ATO_ato_eval__ (&ato);
  if(irc < 0) return irc;
    ATO_dump__ (ato, "after ATO_ato_eval__");
*/


  // get space for aus_anz complex objects
  oa = UME_reserve (wrkSpc, sizeof(ObjGX) * aus_anz);
  if(oa == NULL) return -1;
  oNr = 0;


  // all following objs must be db-points;
  for(i1=0; i1<aus_anz; ++i1) {
    dbi = aus_tab[i1];
    OGX_SET_INDEX (&oa[i1], Typ_PT, dbi);
  }

  ox1->typ    = Typ_SURCIR;
  ox1->form   = Typ_ObjGX;
  ox1->siz    = aus_anz;
  ox1->data   = oa;


    //---- Anf Testausg:
    // printf(" ex APT_decode_suCir %d\n",aus_anz);
    // DEB_dump_ox_s_ (ox1, "RCIR");
    // DEB_dump_ox_0 (ox1, "RCIR");
    //---- End Testausg:

  return 0;


}


//=============================================================================
  int APT_decode_sur (ObjGX *ox1, int aus_anz, int aus_typ[], double aus_tab[],
                      Memspc *wrkSpc) {
//=============================================================================
// Surf in Punkte zerlegen und -> ??
// ACHTUNG: ox1->ptiTab = memspc55

  int      i1, i2, i3, irc, ausInd, ptNr, fTabNr, iRow;
  long     *iTab;
  long     iTabNr, fTabSiz;
  double   *fTab;
  Point    *pt1, *pTab1, *pTab2;
  ObjGX    *ox2, *ox3;

static  SurStripe sus1;
static  SurBSpl   su1;
static  SurRBSpl  rsu1;



  // printf("APT_decode_sur %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }



  i1 = (int)aus_tab[0];
 
  // erster wert muss SURF-Typ sein
  if(aus_typ[0] == Typ_cmdNCsub) {

    if(i1 == T_SPH) {
      return APT_decode_s_sph (ox1,aus_anz,aus_typ,aus_tab);

    } else if(i1 == T_CYL) {
      return APT_decode_s_cyl (ox1,aus_anz,aus_typ,aus_tab);

    } else if(i1 == T_CON) {
      return APT_decode_s_con (ox1,aus_anz,aus_typ,aus_tab);

    } else if(i1 == T_BSP0) {
      goto L_BSP;

    } else if(i1 == T_RBSP) {
      goto L_RBSP;

    } else if(i1 == T_RCIR) {
      // goto L_work1;
      return APT_decode_suCir (ox1,aus_anz,aus_typ,aus_tab,wrkSpc);

    } else if(i1 == T_RSTRIP) {
      // goto L_work1;
      return APT_decode_suStrip (ox1,aus_anz,aus_typ,aus_tab,wrkSpc);

    } else if (i1 == T_SRU)  {               // 22 = SRU (RuledSurf)
      return APT_decode_sru (ox1,aus_anz,aus_typ,aus_tab);

    } else if (i1 == T_HAT)  {               // 23 = HAT (hatch)
      return APT_decode_hat (ox1,aus_anz,aus_typ,aus_tab);

    } else if (i1 == T_SRV)  {               // 24 = SRV (RevolvedSurf)
      return APT_decode_srv (ox1,aus_anz,aus_typ,aus_tab);

    } else if (i1 == T_SWEEP)  {
      return APT_decode_su_sweep (ox1,aus_anz,aus_typ,aus_tab);

    } else if (i1 == T_FSUB)  { 
      return APT_decode_fsub (ox1,aus_anz,aus_typ,aus_tab);


    } else {
      TX_Error(" APT_decode_sur E000");
      return -1;
    }

  }



  // Standardflaechen (planar, Conic, Torus
  return APT_decode_spl (ox1,aus_anz,aus_typ,aus_tab);



  //---------------------------------------------------------
  // 20 = RCIR, 21 = RSTRIP
  L_work1:

  if((int)aus_tab[0] != T_RCIR) goto L_STRIP;

  // alle Punkte in einen workSpace (nicht als DB-dynPt's) speichern;
  // beim DB_StoreSur werden dann alle in den DB_CSEG kopiert.

  ausInd = 1;
  ptNr  = 0;

/*
  // VARIANTE 1:  als PunkteTabelle
  ox2 = wrkSpc->next;
  UME_add (wrkSpc, sizeof(ObjGX));
  pTab1 = wrkSpc->next;

  // alle Punkte -> wrkSpc
  L_rcir_1:
  if(ausInd >= aus_anz) goto L_rcir_9;

  printf(" decode_sur CIR aus_typ[%d]=%d\n",ausInd,aus_typ[ausInd]);

  // irc = APT_decode_pt2 (&iTab[ptNr],&ausInd,aus_anz,aus_typ,aus_tab);
  // if(irc < 0) goto Error;

  irc = UME_add (wrkSpc, sizeof(Point));
  if(irc < 0) goto L_EOM;

  ausInd = APT_decode_pt1 (&pTab1[ptNr], ausInd, aus_typ, aus_tab);
    DEB_dump_obj__ (Typ_PT, &pTab1[ptNr], "cirP[%d]",ptNr);

  ++ptNr;
  goto L_rcir_1;

  L_rcir_9:
  ox1->typ    = Typ_SURCIR;
  ox1->form   = Typ_PT;
  ox1->siz    = ptNr;
  ox1->data   = pTab1;
*/

  // VARIANTE 2:   via IndexTabelle

  // ox2 = wrkSpc->next;
  ox2 = UME_reserve (wrkSpc, sizeof(ObjGX));
  if(ox2 == NULL) return -1;

  iTab = wrkSpc->next;
  iTabNr = sizeof(memspc55) / sizeof(long);


  // alle Punkte -> wrkSpc
  L_rcir_1:
  if(ausInd >= aus_anz) goto L_rcir_9;

  // printf(" decode_sur CIR aus_typ[%d]=%d\n",ausInd,aus_typ[ausInd]);
  if(ptNr >= iTabNr) goto L_EOM;

  // get DB-point from atomic-point (returns dynamic point-index)
  irc = APT_decode_pt2 (&iTab[ptNr], &ausInd, aus_anz, aus_typ, aus_tab);
  if(irc < 0) goto Error;

  ++ptNr;
  goto L_rcir_1;

  L_rcir_9:
  UME_add (wrkSpc, sizeof(long) * ptNr);

  ox1->typ    = Typ_SURCIR;
  ox1->form   = Typ_ObjGX;
  ox1->siz    = 1;
  ox1->data   = ox2;

  ox2->typ    = Typ_PT;
  ox2->form   = Typ_Index;
  ox2->siz    = ptNr;
  ox2->data   = iTab;



  //---- Anf Testausg:
  // printf(" ex APT_decode_sur RCIR %d\n",ptNr);
  // DEB_dump_ox_s_ (ox1, "RCIR");
  // DEB_dump_ox_0 (ox1, "RCIR");
  //---- End Testausg:

  return 0;





  //---------------------------------------------------------
  L_STRIP:
  if((int)aus_tab[0] != T_RSTRIP) goto Error;

  if(aus_typ[1] != Typ_Val) {
    TX_Error(" APT_decode_sur E002 %d",aus_typ[1]);
    return -1;
  }

/*
  // VARIANT mit struct SurStripe
  sus1.ptUNr   = aus_tab[1];
  // printf(" sus1.ptUNr=%d\n",sus1.ptUNr);

  ausInd = 2;
  ptNr  = 0;

  // alle Punkte -> 
  L_rstrip_1:
  if(ausInd >= aus_anz) goto L_rstrip_9;

  // printf(" decode_sur  aus_typ[%d]=%d\n",ausInd,aus_typ[ausInd]);

  aus_anz = APT_decode_pt2 (&iTab[ptNr],&ausInd,aus_anz,aus_typ,aus_tab);
  if(aus_anz < 0) goto Error;
  ++ptNr;

  if(ptNr >= iTabNr) {
    TX_Error(" APT_decode_sur E003");
    return -1;
  }

  goto L_rstrip_1;



  L_rstrip_9:
  sus1.ptVNr   = ptNr / sus1.ptUNr;
  sus1.ptiTab  = iTab;

  ox1->typ    = Typ_SURSTRIP;
  ox1->form   = Typ_SURSTRIP;
  ox1->siz    = 1;
  ox1->data   = (void*)&sus1;
*/


  // VARIANTE 1:  als PunkteTabelle in 2oGX-Records
  ptNr  = aus_tab[1];

  // iRow = Anzahl Reihen 
  iRow = (aus_anz - 2) / ptNr;

  ox2 = wrkSpc->next;
  UME_add (wrkSpc, sizeof(ObjGX) * iRow);

  pTab1 = wrkSpc->next;
  irc = UME_add (wrkSpc, sizeof(Point) * ptNr * iRow);
  if(irc < 0) goto L_EOM;



  ox1->typ    = Typ_SURSTRIP;
  ox1->form   = Typ_ObjGX;
  ox1->siz    = iRow;
  ox1->data   = ox2;


  // alle Punkte -> wrkSpc
  ausInd = 2;
  i3 = 0;
  for(i2=0; i2<iRow; ++i2) {
    ox3 = &ox2[i2];
    ox3->typ    = Typ_PT;
    ox3->form   = Typ_PT;
    ox3->siz    = ptNr;
    ox3->data   = &pTab1[i3];
    for(i1=0; i1<ptNr; ++i1) {
      ausInd = APT_decode_pt1 (&pTab1[i3], ausInd, aus_typ, aus_tab);
        // DEB_dump_obj__ (Typ_PT, &pTab1[i3], "Strip1[%d]",i1);
      ++i3;
    }
  }


/*
  //---- Anf Testausg:
  printf(" ex APT_decode_sur RSTRIP %d %d\n",sus1.ptUNr,sus1.ptVNr);
  for(i2=1; i2<sus1.ptVNr; ++i2) {
    i3 = i2 * sus1.ptUNr;
    for(i1=0; i1<sus1.ptUNr; ++i1) {
      printf(" %d %d %d\n",i1,sus1.ptiTab[i1+i3-sus1.ptUNr],sus1.ptiTab[i1+i3]);
    }
  }
  //---- End Testausg:
*/

  return 0;









  //-----------------------------------------------
  // B-Spline-Surf
  L_BSP:
  if((int)aus_tab[0] != T_BSP0) goto Error;
 

  // zuerst alle variablen in eine tabelle fTab kopieren
  fTab = (void*) memspc51;
  fTabSiz = sizeof(memspc51) / sizeof(double);
  fTabNr  = 0;


  i1 = APT_decode_ValTab (fTab, &fTabNr, fTabSiz, aus_anz, aus_typ, aus_tab);
  if(i1 < 0) return i1;

  // printf(" pt1/2Nr=%f,%f deg1/2=%f,%f\n",fTab[0],fTab[1],fTab[2],fTab[3]);



  // fill
  su1.ptUNr  = fTab[0];
  su1.ptVNr  = fTab[1];
  su1.degU   = fTab[2];
  su1.degV   = fTab[3];
  su1.cpTab  = (Point*)&fTab[4];

  i1 = 4 + (su1.ptUNr * su1.ptVNr * 3);
  su1.kvTabU = &fTab[i1];

  i2 = su1.ptUNr + su1.degU + 1;           // Anzahl U-Knots
  su1.kvTabV = &fTab[i1+i2];


  ox1->typ   = Typ_SURBSP;
  ox1->form  = Typ_SURBSP;
  ox1->siz   = 1;
  ox1->data  = (void*)&su1;


  //---- Anf Testausg:
  // DEB_dump_obj__ (Typ_SURBSP, ox1->data, "SURBSP: "); // display Inhalt


/*
  printf(" BSp-Sur %d %d %d %d\n",su1.ptUNr,su1.ptVNr,su1.degU,su1.degV);
  for(i1=0; i1<su1.ptUNr; ++i1) {
    for(i2=0; i2<su1.ptVNr; ++i2) {
      pt1 = &su1.cpTab[(i1*su1.ptUNr) + i2];
      printf(" pt %d %d %f,%f,%f\n",i1,i2,pt1->x,pt1->y,pt1->z);
    }
  }
  // Anzahl Knots in U:
  i2 = su1.ptUNr + su1.degU + 1;           // Anzahl U-Knots
  for(i1=0; i1<i2; ++i1) {
    printf(" kU %d %f\n",i1,su1.kvTabU[i1]);
  }
  // Anzahl Knots in V:
  i2 = su1.ptVNr + su1.degV + 1;           // Anzahl V-Knots
  for(i1=0; i1<i2; ++i1) {
    printf(" kV %d %f\n",i1,su1.kvTabV[i1]);
  }
  //---- End Testausg:
*/



  return 0;






  //-----------------------------------------------
  // Rat.B-Spline-Surf
  L_RBSP:

  // zuerst alle variablen in eine tabelle fTab kopieren
  fTab = (void*) memspc201;                          // was memspc51 2006-01-19
  fTabSiz = sizeof(memspc201) / sizeof(double);
  fTabNr  = 0;


  i1 = APT_decode_ValTab (fTab, &fTabNr, fTabSiz, aus_anz, aus_typ, aus_tab);
  if(i1 < 0) return i1;

  // printf(" pt1/2Nr=%f,%f deg1/2=%f,%f\n",fTab[0],fTab[1],fTab[2],fTab[3]);


  // fill
  rsu1.ptUNr  = fTab[0];
  rsu1.ptVNr  = fTab[1];
  rsu1.degU   = fTab[2];
  rsu1.degV   = fTab[3];

  rsu1.cpTab  = (Point*)&fTab[4];

  i1 = 4 + (rsu1.ptUNr * rsu1.ptVNr * 3);    // size cpTab
  rsu1.kvTabU = &fTab[i1];

  i2 = rsu1.ptUNr + rsu1.degU + 1;           // size kvTabU - Anzahl U-Knots
  rsu1.kvTabV = &fTab[i1+i2];

  i3 = rsu1.ptVNr + rsu1.degV + 1;           // size kvTabU - Anzahl U-Knots
  rsu1.wTab = &fTab[i1+i2+i3];


  ox1->typ   = Typ_SURRBSP;
  ox1->form  = Typ_SURRBSP;
  ox1->siz   = 1;
  ox1->data  = (void*)&rsu1;

    // DEB_dump_obj__ (Typ_SURRBSP, &rsu1, "rSur");

  return 0;









  //===============================================================000
  Error:
    TX_Error(" APT_decode_sur E099");
    return -1;

  L_EOM:
    TX_Error(" APT_decode_sur EOM");
    return -1;
}



//=============================================================================
  int APT_decode_bsph (ObjGX *ox1, int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================

static Sphere sp1;

  int      i1;
  Point    *pt1, *pt2;
  Circ     *ci1;



  // printf("\nAPT_decode_bsph |%d|\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  // B1=SPH P(0 0 0) 12
  if(aus_typ[1] == Typ_PT) {
    pt1 = DB_get_PT ((long)aus_tab[1]);
    sp1.pc = *pt1;

    if(aus_typ[2] == Typ_Val) {
      sp1.rad = aus_tab[2];

    // B1=SPH P(cen) P(rad)
    } else if(aus_typ[2] == Typ_PT) {
      pt2 = DB_get_PT ((long)aus_tab[2]);
      sp1.rad = UT3D_len_2pt (pt1, pt2);

    } else goto L_err;



  //================================================================
  // B2=SPH C1
  } else if(aus_typ[1] == Typ_CI)    {

    ci1 = DB_get_CI ((long)aus_tab[1]);
    sp1.pc = ci1->pc;
    sp1.rad = fabs(ci1->rad);


  } else goto L_err;


  ox1->typ   = Typ_SPH;
  ox1->form  = Typ_SPH;
  ox1->siz   = 1;
  ox1->data  = (void*)&sp1;


  return 0;


  L_err:
  TX_Error (" Parameterror APT_decode_bsph");
  return -1;

}




//=============================================================================
  int APT_decode_bcon (ObjGX *ox1, int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
// B#=CON ..

static Conus co1;  // {Plane pl; double r1, r2, h;}

  int      i1;
  Point    pt1, pt2;
  Vector   vc1;
  Circ     ci1, ci2;



  // printf("\nAPT_decode_bcon |%d|\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  // 1.record = "CON"

  //------------------------------------------------------
  if(aus_anz == 3) {

    //----------------------------------------------------------------
    // B=CI CI
    if((aus_typ[1] == Typ_CI)    &&
       (aus_typ[2] == Typ_CI))     {

      ci1 = DB_GetCirc ((long)aus_tab[1]);
      ci2 = DB_GetCirc ((long)aus_tab[2]);

      UT3D_pl_ptvc (&co1.pl, &ci1.pc, &ci1.vz);

      co1.r1 = fabs(ci1.rad);
      co1.r2 = fabs(ci2.rad);
      co1.h  = UT3D_len_2pt (&ci1.pc, &ci2.pc);

      goto L_fertig;


    //----------------------------------------------------------------
    // B = CI PT
    } else if((aus_typ[1] == Typ_CI)    &&
              (aus_typ[2] == Typ_PT))     {

      ci1 = DB_GetCirc ((long)aus_tab[1]);
      pt1 = DB_GetPoint ((long)aus_tab[2]);

      UT3D_pl_ptvc (&co1.pl, &ci1.pc, &ci1.vz);
      co1.r1 = fabs(ci1.rad);
      co1.r2 = 0.;
      co1.h  = UT3D_slen_2ptvc (&ci1.pc, &pt1, &ci1.vz);

      goto L_fertig;


    //----------------------------------------------------------------
    } else goto L_err1;




  //================================================================
  } else if(aus_anz == 4) {
  //================================================================


    //----------------------------------------------------------------
    // B = CI h r2
    if((aus_typ[1] == Typ_CI)       &&
       (TYP_IS_GEOMPAR(aus_typ[2])) &&   //(aus_typ[2] == Typ_Val)   &&
       (TYP_IS_GEOMPAR(aus_typ[3])))   { //(aus_typ[3] == Typ_Val))     {

      ci1 = DB_GetCirc ((long)aus_tab[1]);

      UT3D_pl_ptvc (&co1.pl, &ci1.pc, &ci1.vz);
      co1.r1 = fabs(ci1.rad);
      co1.r2 = aus_tab[3];
      co1.h  = aus_tab[2];

      goto L_fertig;


    //----------------------------------------------------------------
    } else goto L_err1;

  }


  //================================================================
  // ausanz == 5
  //================================================================

  //------------------------------------------------------
  // B=PT PT rad rad
  pt1 = DB_GetPoint ((long)aus_tab[1]);
  pt2 = DB_GetPoint ((long)aus_tab[2]);


  UT3D_bcon_pt2rd2 (&co1, &pt1, &pt2, aus_tab[3], aus_tab[4]);
/*
  UT3D_vc_2pt (&vc1, &pt1, &pt2);

  UT3D_pl_ptvc (&co1.pl, &pt1, &vc1);

  co1.r1     = aus_tab[3];
  co1.r2     = aus_tab[4];
  co1.h      = UT3D_len_2pt (&pt1, &pt2);
*/


  //------------------------------------------------------
  L_fertig:
  // printf(" r1=%f r2=%f h=%f\n",co1.r1,co1.r2,co1.h);

  ox1->typ   = Typ_CON;
  ox1->form  = Typ_CON;
  ox1->siz   = 1;
  ox1->data  = (void*)&co1;

  // DEB_dump_obj__ (Typ_CON, &co1, "ex APT_decode_bcon\n");

  return 0;

  L_err1:
    TX_Error("Definition of Cone not implemented");
    return -1;

}


//=============================================================================
  int APT_decode_btor (ObjGX *ox1, int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================

static Torus to1;

  int      i1;
  double   d1;
  Point    pt1, pt2;
  Line     *lnp;
  Vector   vc1;
  Circ     ci1, *cip;
  Plane    *plp;



  // printf("\nAPT_decode_btor |%d|\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }





  //------------------------------------------------------
  // B=TOR L C
  if(aus_anz == 3) {
    if  ((aus_typ[1] == Typ_LN)     &&
         (aus_typ[2] == Typ_CI))         {

      lnp = DB_get_LN ((long)aus_tab[1]);
      cip = DB_get_CI ((long)aus_tab[2]);

      to1.r2 = fabs(cip->rad);
      UT3D_vc_ln (&vc1, lnp);
      // prj CircCen --> Line
      UT3D_pt_projptptvc (&pt2, &d1, NULL, &cip->pc, &lnp->p1, &vc1);

      UT3D_pl_ptvc (&to1.pl, &pt2, &vc1);

      // wenn nun d1 < to1.r2 ?
      to1.r1 = d1 + to1.r2;
      goto L_fertig;
      


    //------------------------------------------------------
    // B=TOR C C
    } else if ((aus_typ[1] == Typ_CI)     &&
               (aus_typ[2] == Typ_CI))         {

      cip = DB_get_CI ((long)aus_tab[1]);
      ci1 = DB_GetCirc ((long)aus_tab[2]);

      to1.r2 = fabs(ci1.rad);
      vc1 = cip->vz;
      // prj CircCen --> 
      UT3D_pt_projptptvc (&pt2, &d1, NULL, &ci1.pc, &cip->pc, &vc1);

      UT3D_pl_ptvc (&to1.pl, &pt2, &vc1);

      // wenn nun d1 < to1.r2 ?
      to1.r1 = d1 + to1.r2;
      goto L_fertig;


    //------------------------------------------------------
    // B=TOR R C
    } else if ((aus_typ[1] == Typ_PLN)     &&
               (aus_typ[2] == Typ_CI))         {

      plp = DB_get_PLN ((long)aus_tab[1]);
      cip = DB_get_CI ((long)aus_tab[2]);

      to1.pl = *plp;

      to1.r2 = fabs(cip->rad);
      // prj CircCen -->
      UT3D_pt_projptptvc (&pt2, &d1, NULL, &cip->pc, &plp->po, &plp->vz);

      // wenn nun d1 < to1.r2 ?
      to1.r1 = d1 + to1.r2;
      goto L_fertig;



    //------------------------------------------------------
    }
  }

  //------------------------------------------------------
  // B=TOR PT VC rad rad
  // if(aus_anz == 5) 
  if(aus_typ[3] == Typ_Val) {

    pt1 = DB_GetPoint ((long)aus_tab[1]);
    vc1 = DB_GetVector((long)aus_tab[2]);

    UT3D_pl_ptvc (&to1.pl, &pt1, &vc1);

    to1.r1     = aus_tab[3] + aus_tab[4];   // outermost !
    to1.r2     = aus_tab[4];

    goto L_fertig;
  }




  //------------------------------------------------------
  // B=TOR PT VC CI [mod] // Variant1 r1 < r2 via MOD()

    pt1 = DB_GetPoint ((long)aus_tab[1]);
    vc1 = DB_GetVector((long)aus_tab[2]);
    ci1 = DB_GetCirc ((long)aus_tab[3]);

    UT3D_pt_projptptvc (&pt2, &d1, NULL, &ci1.pc, &pt1, &vc1);

    UT3D_pl_ptvc (&to1.pl, &pt2, &vc1);

    to1.r2     = fabs(ci1.rad);

    // wenn nun d1 < to1.r2 ?
    to1.r1     = UT3D_len_2pt (&ci1.pc, &pt2) + to1.r2;

    // ??
    if(aus_typ[4] == Typ_modif) {
      if(aus_tab[4] > 0.5) {
        to1.r1 -= to1.r2;
      }
    }




  //------------------------------------------------------
  L_fertig:
  // printf(" r1=%f r2=%f\n",to1.r1,to1.r2);

  ox1->typ   = Typ_TOR;
  ox1->form  = Typ_TOR;
  ox1->siz   = 1;
  ox1->data  = (void*)&to1;

    // DEB_dump_obj__ (Typ_TOR, &to1, "ex APT_decode_btor\n");

  return 0;

}


//=============================================================================
  int APT_decode_refsys1 (Vector *vx, Vector *vz, double *scl,
                          int aus_anz, int aus_typ[], double aus_tab[]) {
//=============================================================================
// den vx und vz aus Inputdaten holen;
// Input:
//   [scale] [Z-Vektor [X-Vektor]] oder
//   [scale] [Refsys]
// Output:
//   vz   Z-Vektor
//   vx   X-Vektor
//   scl  scale


  int      i1;
  Plane    *pln1;


/*
  printf("APT_decode_refsys1 %d\n",aus_anz);
  for(i1=0; i1<aus_anz; ++i1) {
    printf("   aus_[%d] _typ=%d _tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  }
*/



  // Scale ---------------------------------------------------
  i1 = 0;
  if(i1 >= aus_anz) goto L_fertig;
  if(aus_typ[i1] != Typ_Val) {*scl = 1.; goto L_vz;}
  *scl = aus_tab[i1];



  // Vector vz ---------------------------------------------------
  ++i1; // next Inputparam.
  L_vz:
  if(i1 >= aus_anz) goto L_fertig;
  if(aus_typ[i1] != Typ_VC) goto L_refsys;

    *vz = DB_GetVector ((long)aus_tab[i1]);
    if(i1 == (aus_anz-1)) {
      // Normalvektor auf vc1, der in der X-Y-Plane liegt.
      UT3D_vc_perpvcplXY (vx, vz);
      goto L_fertig;
    }


  // Vector vx ---------------------------------------------------
  ++i1; // next Inputparam.
  if(i1 >= aus_anz) goto L_fertig;
  if(aus_typ[i1] != Typ_VC) goto L_refsys;

    *vx = DB_GetVector ((long)aus_tab[i1]);
    goto L_fertig;



  // Refsys ---------------------------------------------------
  L_refsys:
  if(aus_typ[i1] != Typ_PLN) goto Error;

    pln1 = DB_get_PLN ((long)aus_tab[i1]);
    *vz = pln1->vz;
    *vx = pln1->vx;





  //------------------------------------------------
  L_fertig:

    // printf("ex APT_decode_refsys1 scl=%f\n",*scl);
    // DEB_dump_obj__(Typ_VC, vx, "  vx=");
    // DEB_dump_obj__(Typ_VC, vz, "  vz=");


  return 0;


  Error:
  TX_Error (" Parameterror APT_decode_refsys1");
  return -1;


}

/* seit 2006-02-10 unused !!!!!
//=============================================================================
  int APT_decode_mock (ObjGX *oxo,
                       int aus_anz, int aus_typ[], double aus_tab[],
                       Memspc *wrkSpc) {
//=============================================================================
// ACHTUNG: bd1->oiTab = memspc55
// Obj ModelRef (mod1) wird in den wrkSpc gelegt.

  int       irc, i1, mdbNr;
  ObjGX     *ox1;
  ModelRef  *mod1;
  ModelBas  *mdb1;



  // printf("\nAPT_decode_mock %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  mod1 = wrkSpc->next;



  // modelname (Filename)
  if(aus_typ[1] != Typ_String) goto L_parErr;
    APT_get_String (APT_spc1, APT_defTxt, aus_tab[1]);
    mod1->mnam = APT_spc1;
    // printf(" Modelname=|%s|\n",mod1->mnam);


  // Point ---------------------------------------------------
  if(aus_typ[2] != Typ_PT) goto L_parErr;
    mod1->po = DB_GetPoint ((long)aus_tab[2]);


  // check if already registered
  mdbNr = DB_get_ModNr (APT_spc1);
  if(mdbNr < 0) {
    mdbNr = DB_StoreModBas (3, APT_spc1);
  }


  //----------------------------------------------------------------
  // den vx und vz aus Inputdaten holen;
  if(aus_anz > 3) {
    irc = APT_decode_refsys1 (&mod1->vx, &mod1->vz, &mod1->scl,
                              aus_anz-3, &aus_typ[3], &aus_tab[3]);
    if(irc < 0) return irc;

  } else {
    mod1->vx = UT3D_VECTOR_NUL;
    mod1->vz = UT3D_VECTOR_NUL;
  }



  //----------------------------------------------------------------
  // ModelRef: mnam modNr po vx vz scl
  mod1->modNr = mdbNr;



  oxo->typ   = Typ_Model;
  oxo->form  = Typ_Model;
  oxo->siz   = 1;
  oxo->data  = mod1;


  return 0;


  L_parErr:
  TX_Error(" ParameterError MOCK");

  return -1;

}
*/


//=============================================================================
  int APT_decode_model (ObjGX *bd1,int aus_anz,int aus_typ[],double aus_tab[]) {
//=============================================================================
// ACHTUNG: bd1->oiTab = memspc55

// static int      idebug=0;
static ModelRef *mod1, modR1;

  int      irc, i1, i2, i3, ptFlag, modNr, mTyp;
  long     ind, dbi;
  char     *p1, mnam[128];
  double   d1;
  Point    pt1;
  // Vector   vc1;
  Plane    *pln1;
  ModelBas *mdb1;

  mod1 = &modR1;




  // printf("\nAPT_decode_model %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  // printf("  mac_fil=%d\n",ED_get_mac_fil()); //

  // Default position
  mod1->scl = 1.;

  mod1->po  = UT3D_PT_NUL;
  mod1->vx  = UT3D_VECTOR_X;
  mod1->vz  = UT3D_VECTOR_Z;

  // mod1->po  = WC_sur_act.po;
  // mod1->vx  = WC_sur_act.vx;
  // mod1->vz  = WC_sur_act.vz;



  //----  CATALOG (Typ_cmdNCsub T_CTLG)  ---------------------------
  i1 = 0;
  if(aus_typ[i1] == Typ_cmdNCsub) {
    if(aus_tab[i1] != T_CTLG) goto Error;
    ++i1;
    if(aus_typ[i1] != Typ_String) goto Error;
    APT_get_String (APT_spc1, APT_defTxt, aus_tab[i1]); // get text > APT_spc1
       // printf(" _decode catPart |%s|\n",APT_spc1);

    // check if is "M20"
    if(APT_spc1[0] == 'M') {
      // test modNam, return dbi
      irc = APED_dbo_oid (&i2, &dbi, APT_spc1);
      if(irc != 0) goto L_ctlg_5;
      // get Modelname of modelRef dbi
      mod1 = DB_get_ModRef (dbi);
      modNr = mod1->modNr;
        // printf(" CTLG dbi=%ld modNr=%d mnam=|%s|\n",dbi,modNr,mod1->mnam);
      goto L_ctlg_9;
    } 


    // besteht aus "catalog/part" 
    L_ctlg_5:
    // UTX_safeName (APT_spc1, 1); // change / into _
    UTX_safeName (APT_spc1, 0); // change / into _           2011-12-18
    // eine Ref auf internes Model anlegen
    mTyp = -2;
    modNr = DB_StoreModBas (mTyp, APT_spc1);
      // printf(" modNr=%d mnam=|%s| spc1=|%s|\n",modNr,mod1->mnam,APT_spc1);

    L_ctlg_9:
    ++i1;
    goto L_point;

  }


  // Typ_String (44) -----------------------------------------
  if(aus_typ[i1] == Typ_String) {

    APT_get_String (APT_spc1, APT_defTxt, aus_tab[i1]);
    // printf(" Filename |%s|\n",APT_spc1);
 
    mTyp = Mod_get_typ2 (APT_spc1);

    // get ModelNr from Modelname
    // modNr = DB_get_ModNr (APT_spc1);
    modNr = DB_StoreModBas (mTyp, APT_spc1);

    ++i1;


  } else if(aus_typ[i1] == Typ_Model) {
    ind = aus_tab[i1];
    mod1 = DB_get_ModRef (ind);
    modNr = mod1->modNr;

    ++i1;


  } else goto Error;



  // Point ---------------------------------------------------
  L_point:
  if(aus_anz <= i1) goto L_fertig;
  ptFlag = 0;
  if(aus_typ[i1] == Typ_PT) {
    ptFlag = 1;
    pt1 = DB_GetPoint ((long)aus_tab[i1]);
    // der Punkt ist absolutKoordinataen; umrechnen in relative Koordinaten
      // DEB_dump_obj__(Typ_PT, &pt1, "  1.pt=");
    if(AP_IS_2D) UT3D_pt_tra_pt_m3 (&pt1, WC_sur_imat, &pt1);
      // DEB_dump_obj__(Typ_PT, &pt1, "  2.pt=");
    mod1->po = pt1;
    ++i1;
  }


  // scale ..
  L_scale:
  if(aus_anz <= i1) goto L_fertig;
  if(aus_typ[i1] == Typ_Val) {
    mod1->scl = aus_tab[i1];
    ++i1;
  }


  // RefSys
  if(aus_anz <= i1) goto L_fertig;
  if(aus_typ[i1] == Typ_PLN) {
    pln1 = DB_get_PLN ((long)aus_tab[i1]);
    mod1->vz = pln1->vz;
    mod1->vx = pln1->vx;
    if(ptFlag == 0) mod1->po = pln1->po;
    ++i1;
    goto L_scale;
    // goto L_fertig;
  }



  //----------------------------------------------------------------
  // den vx und vz aus Inputdaten holen;
    // // decode [scale] [Z-Vektor [X-Vektor]]
    // irc = APT_decode_refsys1 (&mod1->vx, &mod1->vz, &mod1->scl,
                              // aus_anz-2, &aus_typ[2], &aus_tab[2]);
    // if(irc < 0) return irc;



  if(aus_typ[i1] == Typ_VC) {

    mod1->vz = DB_GetVector ((long)aus_tab[i1]);
    if(AP_IS_2D) UT3D_vc_tra_vc_m3 (&mod1->vz, WC_sur_imat, &mod1->vz);
    UT3D_vc_setLength (&mod1->vz, &mod1->vz, 1.);

    // folgt X-vec ?
    ++i1;
    if((aus_anz > i1)&&(aus_typ[i1] == Typ_VC)) {
      mod1->vx = DB_GetVector ((long)aus_tab[i1]);
      ++i1;
      if(AP_IS_2D) UT3D_vc_tra_vc_m3 (&mod1->vx, WC_sur_imat, &mod1->vx);

    } else {
      // Normalvektor auf vc1, der in der X-Y-Plane liegt.
      UT3D_vc_perpvcplXY (&mod1->vx, &mod1->vz);
    }
    

  } else {
    mod1->vz = WC_sur_act.vz;
    mod1->vx = WC_sur_act.vx;
  }


  if(aus_typ[i1] == Typ_Angle) {
    d1=UT_RADIANS(aus_tab[i1]);
    ++i1;
    // rotate vx around vz
    UT3D_vc_rotvcvcangr (&mod1->vx, &mod1->vz, &mod1->vx, d1);
  }




  //------------------------------------------------
  L_fertig:
    // printf(" Model-Refsys:\n");
    // DEB_dump_obj__(Typ_PT, &mod1->po, "  pt=");
    // DEB_dump_obj__(Typ_VC, &mod1->vx, "  vx=");
    // DEB_dump_obj__(Typ_VC, &mod1->vz, "  vz=");


  // translate
  // if(f_tra != 0) {
    // if(AP_IS_2D) {
      // UT3D_pt_tra_pt_m3 (&mod1->po, WC_sur_imat, &mod1->po);
      // UT3D_vc_tra_vc_m3 (&mod1->vx, WC_sur_imat, &mod1->vx);
      // UT3D_vc_tra_vc_m3 (&mod1->vz, WC_sur_imat, &mod1->vz);
    // }
  // }


    // DEB_dump_obj__(Typ_PT, &mod1->po, "  pt=");
    // DEB_dump_obj__(Typ_VC, &mod1->vx, "  vx=");
    // DEB_dump_obj__(Typ_VC, &mod1->vz, "  vz=");

   // mod1->po.x =   50.;
   // mod1->po.y =    0.;
   // mod1->po.z =    0.;
   // mod1->vx   = UT3D_VECTOR_Y;
   // mod1->vz   = UT3D_VECTOR_X;



  // erst ganz am Ende, sonst kein Parameter mehr lesbar (alle aus_tab leer)
    mod1->modNr = modNr;
    mdb1 = DB_get_ModBas (mod1->modNr);


/*
  if(mdb1->typ < 0) {       // Problem mit -2=catalogModel
    if(mdb1->DLind < 0) {   // noch nicht aufgeloest ..
      TX_Error("APT_decode_model E001-cannot resolve Submodel");
      return -1;
    }
  }
*/


  // printf("  .......... mac_fil=%d\n",ED_get_mac_fil());
  // printf("ex APT_decode_model %d\n",mod1->modNr);
  // DEB_dump_obj__(Typ_PT, &mod1->po, "  po=");
  // DEB_dump_obj__(Typ_Model, mod1, "ex APT_decode_model:\n");


  bd1->typ   = Typ_Model;
  bd1->form  = Typ_Model;
  bd1->siz   = 1;
  bd1->data  = mod1;


    // printf("ex APT_decode_model\n");

  return 0;


  Error:
  TX_Error (" Parameterror APT_decode_model");
  return -1;

}


//=============================================================================
  int APT_decode_sol (ObjGX *bd1,int aus_anz,int aus_typ[],double aus_tab[],
                      Memspc *oSpc) {
//=============================================================================
// ACHTUNG: bd1->oiTab = memspc55
// B20=SPH center radius

  int    i1, i2;
  long   l1;
  ObjGX  *oTab;
  ObjDB  *sba;
  


  // printf("APT_decode_sol %d\n",aus_typ[0]);
  // for(i1=0;i1<aus_anz;++i1)printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);



  //================================================================
  if(aus_anz == 1)  {
  //================================================================

    if(aus_typ[0] == Typ_SOL)     {

      oTab = DB_GetSol ((long)aus_tab[0]);
      *bd1 = *oTab;
      return 0;

    } else goto Error;
  }



  //================================================================
  // erster wert muss SOL-Typ sein
  if(aus_typ[0] != Typ_cmdNCsub) goto L_comp;

  // PRISM = 25.
  // SPH   = 26.
  // CONUS = 27
  // TORUS = 28


  i1 = aus_tab[0];

  if(i1 == T_SPH) {          // Sphere
    return APT_decode_bsph (bd1, aus_anz,aus_typ,aus_tab);

  } else if(i1 == T_CON) {          // CONUS
    return APT_decode_bcon (bd1, aus_anz,aus_typ,aus_tab);

  } else if(i1 == T_TOR) {   // TORUS
    return APT_decode_btor (bd1, aus_anz,aus_typ,aus_tab);


  } else if(i1 != T_PRISM) {   // PRISM
    goto Error;
  }



  //================================================================
  // PRISM
  // B#=PRISM obj1 Abstand
  // B#=PRISM obj1 obj2
  // obj1, obj2: Circ od Pt.


  // get space for 2 ObjGX
  // oTab = (ObjGX*) memspc55;
  oTab = UME_reserve (oSpc, sizeof(ObjGX) * 2);

  // Obj.1: Ci. oder Curve(Rect)
  oTab[0].typ  = aus_typ[1];
  oTab[0].form = Typ_Index;
  oTab[0].siz  = 1;
  l1 = aus_tab[1];
  oTab[0].data = PTR_LONG(l1);

  // Obj.2: Ci od Pt od Val. od VC
  oTab[1].typ  = aus_typ[2];
  oTab[1].form = Typ_Index;
  oTab[1].siz  = 1;
  l1 = aus_tab[2];
  oTab[1].data = PTR_LONG(l1);


  // wenn 2. Wert Typ_Val statt Obj (Hoehe):
  if(oTab[1].typ == Typ_Val) {
    oTab[1].typ  = Typ_VAR;
    l1 = DB_StoreVar (-1L, aus_tab[2]);
    oTab[1].data = PTR_LONG(l1);
  }



  bd1->typ   = Typ_SOL;
  bd1->form  = Typ_ObjGX;
  bd1->siz   = 2;
  bd1->data  = oTab;


  // TESTBLOCK
  // printf("  typ=%d ind=%ld\n",oTab[0].typ,oTab[0].data);
  // printf("  typ=%d ind=%ld\n",oTab[1].typ,oTab[1].data);
  // DEB_dump_obj__(Typ_ObjGX, bd1, "ex-APT_decode_sol:");
  // DEB_dump_ox_0(bd1, " _decode_sol:");
  // END TESTBLOCK


  return 1;



  //================================================================
  // brep-solid (2-n surfaces)
  L_comp:

  // get space for aus_anz-1 ObjDB
  // sba = (ObjDB*) memspc55;
  i2 = aus_anz - 1;
  sba = UME_reserve (oSpc, sizeof(ObjGX) * i2);



// TODO: check size of memspc55
  for(i1=0; i1<aus_anz; ++i1) {
    if(aus_typ[i1] != Typ_SUR) goto Error;
    i2 = aus_tab[i1]; // DB-ind
    sba[i1].dbInd   = i2;
    sba[i1].dlInd   = 0;  // UNUSED
    sba[i1].typ     = Typ_SUR;
    sba[i1].stat    = 0;
      // DEB_dump_obj__ (Typ_SURBND, &sba[i1], " _decode_sol:");
  }


  bd1->typ   = Typ_BREP;
  bd1->form  = Typ_ObjDB;
  bd1->siz   = aus_anz;
  bd1->data  = sba;  // table of BndSur's

    // DEB_dump_ox_0(bd1, " _decode_sol:");

  return 1;





  //================================================================
  Error:
  TX_Error (" Parameterror APT_decode_sol");
  return -1;

}


//====================================================================
  int APT_decode_att (Att_ln *att1,
                      int aus_anz, int aus_typ[], double aus_tab[]) {
//====================================================================
// decode a linetype given in source from eg:
// G20 = color lineTyp thickness
// TODO: change Att_ln -> Ind_Att_ln

  // int i1;


  //TX_Print("APT_decode_att |%d|\n",aus_anz);



  if       ((aus_anz    ==   3)         &&
            (aus_typ[0] ==Typ_Val)      &&
            (aus_typ[1] ==Typ_Val)      &&
            (aus_typ[2] ==Typ_Val)) {


    AttLn_Set1 (att1, (int)aus_tab[0], (int)aus_tab[1], (int)aus_tab[2]);
    goto Fertig;




  //-----------------------------------------------------------------
  } else {

    TX_Error(" Definition Graf. Attrib. nicht implementiert %d",aus_typ[0]);
    goto Error;
  }


  Fertig:

    // TX_Print(" att=%d,%d,%d",att1->col,att1->ltyp,att1->lthick);

  return 1;


  Error:
  return -1;

}


//=======================================================================
  int APT_decode_vc (Vector *vc_out,
                     int aus_anz, int aus_typ[], double aus_tab[]) {
//=======================================================================
// decode vectors; eg D(S val|MOD)
// OFFEN:
// Perpendic/Tangential an Surfaces:
// V=P surf Richtung
//  (Richtung normal/along/across)
//  T_PERP = Normal; T_CX = across;  T_FW = forward
//  PT + BspSur   T_PERP/T_FW/T_CX
//  RetCod:
//    -3     object not yet complete


  int             irc, i1, i2, rc, lenStat, iRev, f_tra, iParl, oNr,
                  iSeg1, iSeg2,  mtyp, typ1;
  long            l1;
  double          d1, d2, d3, lenVc;
  Point           pt1, pt2, *pp1, *pp2, *pp3;
  Line            ln1, ln2, *lnp;
  Vector          vc1, vc2, vc3, vc4, vc5, vc6, *pd1, *pd2, *pd3;
  Circ            ci1, *cip;
  Plane            pl1;
  // Mat_4x3          m1;
  ObjGX           ox1, *ox1p, *ox2p;
  ModelRef        *mr1;
  Memspc          memSeg1;
  char            obj1[OBJ_SIZ_MAX];
  void            *vp1;




  // printf("APT_decode_vc |%d|\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }
  // printf("APT_obj_stat=%d\n",APT_obj_stat);


  // init
  iParl = 0;    // 0=perp; 1=parl, 2=across;
  iRev = 0;     // 0=normal; 1=revers


  L_get_cmd:
  if(aus_typ[aus_anz - 1] != Typ_cmdNCsub) goto L_vc_1;

  // "REV" als letzen parameter abspalten und ganz am Schluss anwenden
  if(aus_tab[aus_anz - 1] == T_REV)                      {
    iRev = 1;
    --aus_anz;
    goto L_get_cmd;
  }

  // "PARL" als letzen parameter abspalten und ganz am Schluss anwenden
  if(aus_tab[aus_anz - 1] == T_PARL)                      {
    iParl = 1;
    --aus_anz;
    goto L_get_cmd;
  }

  // "CX" als letzen parameter abspalten und ganz am Schluss anwenden
  if(aus_tab[aus_anz - 1] == T_CX)                      {
    iParl = 2;
    --aus_anz;
    goto L_get_cmd;
  }



  //----------------------------------------------------------------
  L_vc_1:

  // VAL() / Laenge als letzen parameter abspalten und ganz am Schluss anwenden
  lenStat = NO;
  if(aus_typ[aus_anz - 1] == Typ_Val) {
    if(aus_typ[aus_anz - 2] != Typ_Int1) {  // skip vc-mult & vc-div
      lenStat = YES;                        // 0
      lenVc = aus_tab[aus_anz - 1];
      --aus_anz;
    }
  }
    

  IN_anz = aus_anz;
  rc     = 0;
  f_tra  = 0;


  // already extracted: REV - PARL - CX - VAL()
  //                    T_REV T_PARL T_CX Typ_Val
    // printf(" IN_anz=%d iParl=%d iRev=%d\n",IN_anz,iParl,iRev);
    // if(lenStat == YES) printf(" lenStat-lenVc = %f\n",lenVc);




  //================================================================
  if(IN_anz == 1) {
  //================================================================


  //-----------------------------------------------------------------
  // D = P                             // D(P(0 0 0) P#)
  if (aus_typ[0] == Typ_PT) {
    pp1 = DB_get_PT ((long)aus_tab[0]);
    UTRA_UCS_WCS_PT (&pt1, pp1);     // point -> UCS
      // DEB_dump_obj__(Typ_PT, &pt1, "  decode_vc:pt1");
    UT3D_vc_pt (&vc1, &pt1);
      // DEB_dump_obj__(Typ_VC, &vc1, "  decode_vc:vc1");
    f_tra  = 1;
    goto Fertig;


  //-----------------------------------------------------------------
  // D = dx dy                          // second value = lenVc
  } else if((aus_typ[0]==Typ_Val)      &&
            (lenStat   == YES)) {
    vc1.dx = aus_tab[0];
    vc1.dy = lenVc;         
    vc1.dz = 0.;
    goto L_exit;   // do not use length again


  //-----------------------------------------------------------------
  // D = ANG(30)                     (Drehwinkel)
  } else if(aus_typ[0] == Typ_Angle)  {
    d1=UT_RADIANS(aus_tab[0]);
    vc1.dx = cos(d1);
    vc1.dy = sin(d1);
    vc1.dz = 0.;
    f_tra  = 1;
    goto Fertig;


  //-----------------------------------------------------------------
  // D = D                           (Vektor)
  } else if(aus_typ[0]==Typ_VC)     {
    vc1 = DB_GetVector((long)aus_tab[0]);
    // CX:iParl=2; REV:iRev=1;
    if(iParl == 2) goto Lvc_cx1;
    goto Fertig;


  //-----------------------------------------------------------------
  // D = R                           (Vektor von Plane)
  } else if(aus_typ[0]==Typ_PLN)  {
    DB_GetRef (&pl1, (long)aus_tab[0]);
    if(iParl == 0) {
      vc1 = pl1.vz;
    } else if(iParl == 1) {
      vc1 = pl1.vx;
    } else if(iParl == 2) {
      vc1 = pl1.vy;
    }
    goto Fertig;


  //-----------------------------------------------------------------
  // D = L [laenge]                  (Richtung LN)
  } else if(aus_typ[0]==Typ_LN)    {
    ln1 = DB_GetLine  ((long)aus_tab[0]);
    UT3D_vc_2pt (&vc1, &ln1.p1, &ln1.p2);
    // CX:iParl=2; REV:iRev=1;
    if(iParl == 2) goto Lvc_cx1;
    goto Fertig;


  //-----------------------------------------------------------------
  // D = C [length]                  (Z-Vektor von Circ)
  // D = C parameter                 (tangent to Circ
  } else if(aus_typ[0]==Typ_CI)    {

    ci1 = DB_GetCirc  ((long)aus_tab[0]);

    if(lenStat != YES) {
      vc1 = ci1.vz;
      goto Fertig;
    }

    d1 = lenVc;
    UT3D_pt_vc__par_ci (&pt1, &vc1, &ci1, d1);
    // UT3D_vc_tng_ci_pt (&vc1, &pt1, &ci1);
    UT3D_vc_setLength (&vc1, &vc1, 1.);
      // DEB_dump_obj__ (Typ_VC, &vc1, "vc1 in APT_decode_vc");

    lenStat = NO;
    goto Fertig;


  //-----------------------------------------------------------------
  // D = S [par1]             // tangent to curve
  } else if(aus_typ[0]==Typ_CV)    {
    iSeg1 = 0;
    iSeg2 = 0;
    goto L_D__S_SEG_SEG1;


  //-----------------------------------------------------------------
  // D = M                   (Z-Vector of model)
  } else if(aus_typ[0] == Typ_Model)    {
    l1 = aus_tab[0];
    mr1 = DB_get_ModRef (l1);
    vc1 = mr1->vz;
    goto L_exit;


  //-----------------------------------------------------------------
  // D = A                   (Z-Vector of surf)
  } else if(aus_typ[0] == Typ_SUR)    {
    ox1p = DB_GetSur ((long)aus_tab[0], 0);

    UME_init (&memSeg1, memspc101, sizeof(memspc101));  // Init Datasegment
    // UT3D_vcn_cvo (&vc1, &pt1, ox1p, &memSeg1);
    // UT3D_ptvc_sus (&pt1, &vc1, &memSeg1, ox2p);
    rc = UT3D_ptvc_sur (&pt1, &vc1, &memSeg1, ox1p);
    if(rc < 0) goto L_parErr;
    goto L_exit;



  //-----------------------------------------------------------------
  } else goto L_parErr;




  //================================================================
  } else if(IN_anz == 2) {
  //================================================================

  //  next functions need correct sequence of parameters:
  
  // D = L|D|R L|D|R                    2 Obj's -> crossprod
  if((aus_typ[0] < Typ_SymB)&&(aus_typ[1] < Typ_SymB)) {
    if     (((aus_typ[0]==Typ_LN)  ||
             (aus_typ[0]==Typ_VC)  ||
             (aus_typ[0]==Typ_PLN))    &&
            ((aus_typ[1]==Typ_LN)  ||
             (aus_typ[1]==Typ_VC)  ||
             (aus_typ[1]==Typ_PLN)))     {

      // ob1 -> vc2
      if(aus_typ[0]==Typ_LN) {
        ln1 = DB_GetLine ((long)aus_tab[0]);
        UT3D_vc_2pt (&vc2, &ln1.p1, &ln1.p2);

      } else if(aus_typ[0]==Typ_VC) {
        vc2 = DB_GetVector((long)aus_tab[0]);

      } else if(aus_typ[0]==Typ_PLN) {
        DB_GetRef (&pl1, (long)aus_tab[0]);
        vc2 = pl1.vz;
      }

      // ob2 -> vc3
      if(aus_typ[1]==Typ_LN) {
        ln1 = DB_GetLine ((long)aus_tab[1]);
        UT3D_vc_2pt (&vc3, &ln1.p1, &ln1.p2);

      } else if(aus_typ[1]==Typ_VC) {
        vc3 = DB_GetVector((long)aus_tab[1]);

      } else if(aus_typ[1]==Typ_PLN) {
        DB_GetRef (&pl1, (long)aus_tab[1]);
        vc3 = pl1.vz;

      } else goto L_parErr;

      goto L_perp;  // vc1 < vc2,vc3
    }
  }


  //================================================================
  // make ascending sequence: D P L C S R A N
  // test if ato-1 == geom-obj
  if(aus_typ[0] < Typ_SymB) {
    // yes, ato-1 is geom-obj; test if also ato-2 == geom-obj
    if(aus_typ[1] < Typ_SymB) {
      // yes, bring first two geom.objects into correct sequence (D P L C S R A N)
      if(aus_typ[0] > aus_typ[1]) ATO_swap (aus_typ, aus_tab, 0, 1);
    }
  }


  //================================================================
  // D = P ..
  if (aus_typ[0]==Typ_PT)   {
    pp1 = DB_get_PT ((long)aus_tab[0]);


    //-----------------------------------------------------------------
    // D = P P
    if      (aus_typ[1]==Typ_PT) {
    pt2 = DB_GetPoint ((long)aus_tab[1]);
    vc1.dx = pt2.x - pp1->x;
    vc1.dy = pt2.y - pp1->y;
    vc1.dz = pt2.z - pp1->z;
    goto Fertig;


    //...........................................
    // D = P L                                 // parallel line
    } else if(aus_typ[1]==Typ_LN) {
      ln1 = DB_GetLine ((long)aus_tab[1]);
      UT3D_vc_2pt (&vc2, &ln1.p1, &ln1.p2);
      UT3D_vc_2pt (&vc3, &ln1.p1, pp1);
      goto L_perp;


    //...........................................
    // D = P C                                 // tangent to circ
    } else if(aus_typ[1]==Typ_CI) {
        // printf(" D = P C iParl=%d\n",iParl);
      cip = DB_get_CI ((long)aus_tab[1]);

      if(iParl == 2) {
        UT3D_vc_2pt (&vc1, pp1, &cip->pc);    // normal
      } else {
        UT3D_vc_tng_ci_pt (&vc1, pp1, cip);   // tangent to circ
      }
        // DEB_dump_obj__ (Typ_VC, &vc1, "  vc1");
      UT3D_vc_setLength (&vc1, &vc1, 1.);
      goto Fertig;


    //...........................................
    // D = P S                               // tangent to curve
    } else if(aus_typ[1]==Typ_CV) {
      // ox1p = DB_GetCurv ((long)aus_tab[1]);

      irc = UTO_objDat_dbS (&vp1, &typ1, (long)aus_tab[1], 0, 0, obj1);
      if(irc < 0) goto L_parErr;

      // get tangent-vector to curve
      rc = UT3D_vc_tng_crv_pt (&vc1, pp1, typ1, vp1);
      if(rc < 0) return rc;

      // ellipse / trimmed circ: CX
      if(iParl == 2) {
        vc2 = vc1;
        if(typ1 == Typ_CVELL) {
          // normalvector vc1 vz (get X from Y(vc2) and Z(vz
          UT3D_vc_perp2vc (&vc1, &vc2, &((CurvElli*)vp1)->vz);
          iParl = 0;
        } else if(typ1 == Typ_CI) {
          UT3D_vc_perp2vc (&vc1, &vc2, &((Circ*)vp1)->vz);
          iParl = 0;
        }
      }
      goto Fertig;


    //...........................................
    // D = P A                              // tangent to surf
    } else if(aus_typ[1]==Typ_SUR) {
        // printf(" APT_ptPars: %lf %lf\n",APT_ptPars.du,APT_ptPars.dv);
        // muesste man löschen; mode=via point|parameters.
      ox1p = DB_GetSur ((long)aus_tab[1], 0);
      rc = UTO_vc_perp_sur (&vc1, ox1p, pp1, iParl, &APTSpcTmp);
      if(rc < 0) return rc;
      goto Fertig;


    //...........................................
    // D = P D
    } else if(aus_typ[1]==Typ_VC) {
      if(APT_obj_stat != 0) return -3;  // not-yet-complete
      else goto L_parErr;


    //...........................................
    } else goto L_parErr;


  //-----------------------------------------------------------------
  // D = ANG ANG                      (Drehwinkel Kippwinkel)
  } else if((aus_typ[0]==Typ_Angle)   &&
            (aus_typ[1]==Typ_Angle))    {

    UT3D_vc_2angr (&vc1,UT_RADIANS(aus_tab[0]),UT_RADIANS(aus_tab[1]));
    f_tra  = 1;
    goto Fertig;


  //-----------------------------------------------------------------
  // D = dx dy dz                             // vector from 3 values
  } else if((aus_typ[0]==Typ_Val)     &&
            (aus_typ[1]==Typ_Val)     &&
            (lenStat   == YES)) {           // 3. value = Z-value
    vc1.dx = aus_tab[0];
    vc1.dy = aus_tab[1];
    vc1.dz = lenVc;
    lenStat = NO;    // do not use length again
    f_tra = 1;
    goto Fertig;


  //-----------------------------------------------------------------
  // D = D ANG(rot)                     
  } else if((aus_typ[0]==Typ_VC)      &&
            (aus_typ[1]==Typ_Angle))    {
    vc2 = DB_GetVector((long)aus_tab[0]);
    UT3D_vc_rotvcvcangr (&vc1, &WC_sur_act.vz, &vc2, UT_RADIANS(aus_tab[1]));
    goto Fertig;


  //-----------------------------------------------------------------
  // D = C ..     
  } else if(aus_typ[0]==Typ_CI) {
    cip = DB_get_CI ((long)aus_tab[0]);
    typ1 = aus_typ[0];

    //----------------------------------------------------------------
    // D = C PTS            // standard-points of circ; start-, end-, center
    if(aus_typ[1]==Typ_PTS) {
      i1 = aus_tab[1];
      APT_modMax1 = UT3D_ptvcpar_std_obj (NULL, &vc1, NULL, 0, i1, typ1, cip);
      if(APT_modMax1 < 0) goto L_parErr;
      goto Fertig;
  
    //-----------------------------------------------------------------
    // D = C MOD            // normal-axis / center-startpoint / center-endpoint
    } else if(aus_typ[1]==Typ_modif) {
      i1 = aus_tab[1];
      APT_modMax1 = UT3D_vc_mod_obj (&vc1, i1, typ1, cip);
      if(APT_modMax1 < 0) goto L_parErr;
      goto Fertig;

    //-----------------------------------------------------------------
    }  else goto L_parErr;


  //-----------------------------------------------------------------
  // D = R MOD
  } else if((aus_typ[0]==Typ_PLN)      &&
            (aus_typ[1]==Typ_modif)) {

    APT_modMax1 = 2;
    DB_GetRef (&pl1, (long)aus_tab[0]);
    i1 = aus_tab[1];

    if(i1 == 1) {vc1 = pl1.vx; goto Fertig;}       // 1=va
    else if(i1 == 2) {vc1 = pl1.vy; goto Fertig;}  // 2=vb
    else {vc1 = pl1.vz; goto Fertig;}              // 0=Z
//     if(i1 == 3) {                             // 3=-va
//       UT3D_vc_invert (&vc1, &pl1.vx);
//       goto Fertig;
//     }
//     UT3D_vc_invert (&vc1, &pl1.vy);           // 4=-vb
    goto Fertig;




    //================================================================
    // D = S ..        
    } else if(aus_typ[0] == Typ_CV) {
        // printf(" D = S x\n");
        iSeg1 = 0;
        iSeg2 = 0;

      //----------------------------------------------------------------
      // D = S PTS                   // start- endPt od curve /standrdPoints
      if(aus_typ[1] == Typ_PTS)   {
        i1 = aus_tab[1];  // eg Ptyp_end ...

        // get bin.obj of curv
        irc = UTO_objDat_dbS (&vp1, &typ1, (long)aus_tab[0], iSeg1, iSeg2, obj1);
        if(irc < 0) goto L_parErr;

        // get point and vector of tangent
        L_D_S_PTS1:  // ptNr=i1, obj=(typ1,vp1)
        irc = UT3D_ptvcpar_std_obj (NULL, &vc1, NULL, 0, i1, typ1, vp1);
        if(irc < 0) goto L_parErr;
        goto Fertig;


      //----------------------------------------------------------------
      // D = S SEG [par]             // segment of polygon
      } else if(aus_typ[1] == Typ_SEG) {
        iSeg1 = aus_tab[1];
        goto L_D__S_SEG_SEG1;


      //----------------------------------------------------------------
      // D = S MOD [par]             // ellipse/trimmed-circle
// TODO: MOD - the nr of the intersection; eg ellipse 2 intersections !
      } else if(aus_typ[1] == Typ_modif)    {
        i1 = aus_tab[1];  // eg Ptyp_end ...

        // get bin.obj of curv
        irc = UTO_objDat_dbS (&vp1, &typ1, (long)aus_tab[0], iSeg1, iSeg2, obj1);
        if(irc < 0) goto L_parErr;
  
        // get typical vectors for curve
        APT_modMax1 = UT3D_vc_mod_obj (&vc1, i1, typ1, vp1);
        if(APT_modMax1 < 0) goto L_parErr;
        goto Fertig;


    //-----------------------------------------------------------------
    } else goto L_parErr;



  //-----------------------------------------------------------------
  } else goto L_parErr;  // end (IN_anz==2)


  //================================================================
  } else if(IN_anz == 3) {
  //================================================================

  //-----------------------------------------------------------------
  // D = TNG ..
  if(aus_typ[0] == Typ_cmdNCsub) {

    if(aus_tab[0] == T_TNG) {
      if(aus_typ[1]==Typ_PT) {
        pp1 = DB_get_PT ((long)aus_tab[1]);


        // D = TNG P L
        if(aus_typ[2]==Typ_LN) {
          lnp = DB_get_LN ((long)aus_tab[2]);
          UT3D_vc_tng_crv_pt (&vc1, pp1, Typ_LN, lnp);
          goto Fertig;

        // D = TNG P C
        } else if(aus_typ[2]==Typ_CI) {
          cip = DB_get_CI ((long)aus_tab[2]);
          UT3D_vc_tng_crv_pt (&vc1, pp1, Typ_CI, cip);
          goto Fertig;

        // D = TNG P S
        } else if(aus_typ[2]==Typ_CV) {
          ox1p = DB_GetCurv ((long)aus_tab[2]);
          UT3D_vc_tng_crv_pt (&vc1, pp1, ox1p->typ, ox1p->data);
          goto Fertig;

      } else goto L_parErr;

    } else goto L_parErr;


    //------------------------------------------------------
    } else goto L_D_sort;    // skip sec with 3 geom.objs

  }


  //================================================================
  //  next functions need correct sequence of parameters:

  //-----------------------------------------------------------------
  // D = P|D|L   P|D|L   P|D|L                                // normalVector
  if((aus_typ[0] < Typ_CI)  &&
     (aus_typ[1] < Typ_CI)  &&
     (aus_typ[2] < Typ_CI))     {

    if(aus_typ[1] == Typ_PT) {
      pp2 = DB_get_PT ((long)aus_tab[1]);
    } else goto L_noFunc;


    //----------------------------------------------------------------
    // D = P P ..
    if(aus_typ[0] == Typ_PT) {
      pp1 = DB_get_PT ((long)aus_tab[0]);
      UT3D_vc_2pt (&vc2, pp1, pp2);

      //----------------------------------------------------------------
      // D = P P D                          // normalVector
      if(aus_typ[2] == Typ_VC) {
        vc3 = DB_GetVector ((long)aus_tab[2]);

      //----------------------------------------------------------------
      // D = P P L                          // normalVector
      } else if(aus_typ[2] == Typ_LN) {
        lnp = DB_get_LN ((long)aus_tab[2]);
        UT3D_vc_ln (&vc3, lnp);

      //----------------------------------------------------------------
      // D = P P P                          // normalVector
      } else if(aus_typ[2] == Typ_PT) {
        pp3 = DB_get_PT ((long)aus_tab[2]);
        UT3D_vc_2pt (&vc3, pp1, pp3);


      //----------------------------------------------------------------
      } else goto L_noFunc;
   

    //----------------------------------------------------------------
    // D = . P P                          // normalVector
    } else if(aus_typ[2] != Typ_PT) {
      pp3 = DB_get_PT ((long)aus_tab[2]);
      UT3D_vc_2pt (&vc3, pp2, pp3);

      //----------------------------------------------------------------
      // D = D P P                          // normalVector
      if(aus_typ[0] == Typ_VC) {
        vc2 = DB_GetVector ((long)aus_tab[0]);


      //----------------------------------------------------------------
      // D = L P P                          // normalVector
      } else if(aus_typ[0] == Typ_VC) {
        lnp = DB_get_LN ((long)aus_tab[0]);
        UT3D_vc_ln (&vc2, lnp);


      //----------------------------------------------------------------
      } else goto L_noFunc;

    //----------------------------------------------------------------
    } else goto L_noFunc;

    goto L_perp;

  }


  //================================================================
  // sort atomic objects ascending
  L_D_sort:
  ATO_sort1 (IN_anz, aus_typ, aus_tab);


  //----------------------------------------------------------------
  // D = D ..
  if(aus_typ[0] == Typ_VC) {
    vc2 = DB_GetVector ((long)aus_tab[0]);


    //-----------------------------------------------------------------
    // D = D ANG(rot) ANG(tilt)
    if((aus_typ[1]==Typ_Angle)      &&
       (aus_typ[2]==Typ_Angle))    {
      // vc3 = rot vc2 around Z-axis
      UT3D_vc_rotvcvcangr (&vc3, &WC_sur_act.vz, &vc2, UT_RADIANS(aus_tab[1]));
      // tilt = rotate around a normal in XY-plane
      // compute vc4 = normal to vc3 in XY-plane (Get VX from VY, VZ)
      UT3D_vc_perp2vc (&vc4, &vc3, &WC_sur_act.vz);
      // vc1 = rot vc3 around vc4
      UT3D_vc_rotvcvcangr (&vc1, &vc4, &vc3, UT_RADIANS(aus_tab[2]));
      goto Fertig;


    //-----------------------------------------------------------------
    // D = D * Val
    // D = D / Val                   // multiply or divide Vec
    } else if((aus_typ[1]==Typ_Int1)    &&
              (aus_typ[2]==Typ_Val))       {
      i1 = aus_tab[1];  // '*' or '/'
      d1 = aus_tab[2];
        // printf(" mult vc %d %f\n",i1,d1);
        // DEB_dump_obj__ (Typ_VC, &vc2, "vc2=");
      if(i1 == '*') {          // '*'
        UT3D_vc_multvc (&vc1, &vc2, d1);
      } else if(i1 == '/') {   // '/'
        d1 = 1. / d1;
        UT3D_vc_multvc (&vc1, &vc2, d1);
      } else goto L_parErr;
      goto Fertig;
    }


  //-----------------------------------------------------------------
  // D = S ..                     
  } else if(aus_typ[0]==Typ_CV)     {


    //-----------------------------------------------------------------
    // D = S SEG ..             // get direction of Polygonsegment of CCV
    // D = S MOD ..             // OBSOLET
    if((aus_typ[1]==Typ_modif)||(aus_typ[1]==Typ_SEG))        {


      //----------------------------------------------------------------
      // D = S SEG SEG [par]           // get direction of Polygonsegment of CCV
      // D = S MOD MOD [par]            // OBSOLET
      if((aus_typ[2]==Typ_modif)||(aus_typ[2]==Typ_SEG))        {
      if(aus_typ[2]==Typ_modif)
          MSG_ERR__ (ERR_obsolete, "change MOD to SEG");
        iSeg1 = aus_tab[1];
        iSeg2 = aus_tab[2];

        // get bin.obj of curv-0; iSeg1 iSeg2
        L_D__S_SEG_SEG1:
        irc = UTO_objDat_dbS (&vp1, &typ1, (long)aus_tab[0], iSeg1, iSeg2, obj1);
        if(irc < 0) goto L_parErr;
          // DEB_dump_obj__(typ1, vp1, "  D=S_SEG-1");
          
        // extract Line from Polygon
        if(typ1 == Typ_LN) {
          UT3D_vc_ln (&vc1, (Line*)vp1);
          goto Fertig;

        } else {
          // get tangent to curve (typ1,vp1)
          irc = UT3D_pt_vc__par_cv (NULL, &vc1, typ1, vp1, 1, lenVc);
          if(irc < 0) goto L_parErr;
        }

        // test for parameter (tangent)
        if(lenStat == YES) {
          UT3D_vc_setLength (&vc1, &vc1, 1.);
          lenStat = NO;
          goto Fertig;
        } else {
          // no parameter; test D = S  (elli or (trimmed)circ)
          if(typ1 == Typ_CVELL) {
            vc1 = ((CurvElli*)vp1)->vz;
            goto Fertig;
          } else if(typ1 == Typ_CVELL) {
            vc1 = ((Circ*)vp1)->vz;
            goto Fertig;
          }
          goto L_parErr;
        }


      //----------------------------------------------------------------
      // D = S SEG PTS
      } else if(aus_typ[2]==Typ_PTS)        {
        i1 = aus_tab[1];    // segmentNr; first=1
        i2 = aus_tab[2];    // pointTyp; eg Ptyp_start

        // get seg <i1> out of curve
        irc = UTO_objDat_dbS (&vp1, &typ1, (long)aus_tab[0], i1, 0, obj1);
        if(irc < 0) goto L_parErr;
          // DEB_dump_obj__(typ1, vp1, "  _decode_pt-S_SEG-PTI");

        // get pointTyp <i2> from geom-obj (typ1,vp1);
        irc = UT3D_ptvcpar_std_obj (NULL, &vc1, NULL, 0, i2, typ1, vp1);
        if(irc < 0) goto L_noFunc;
        APT_modMax1 = irc;
        goto Fertig;  // vc1


      //----------------------------------------------------------------
      } else goto L_parErr;

    //----------------------------------------------------------------
    } else goto L_parErr;

  //-----------------------------------------------------------------
  } else goto L_parErr;







  //================================================================
  } else {

    TX_Error(" Vektordefinition nicht implementiert\n");
    goto Error;
  }



  //=================================================================
  L_perp:         // In: vc2, vc3
  // make crossProd
  if(iParl) goto L_parl;
  if(lenStat == NO) { lenStat = YES; lenVc = 1.; } // normieren
  UT3D_vc_perp2vc (&vc1, &vc2, &vc3);
  goto Fertig;


  //=================================================================
  L_parl:
  // in: vc2, vc3;
  UT3D_parvc_2vc (&d1, &vc3, &vc2);
  UT3D_vc_multvc (&vc1, &vc2, d1);
  if(iParl == 2) goto L_cx_lin;
  goto Fertig;


  //=================================================================
  L_cx_lin:           // in: vc1, pp1, pp3
  UT3D_pt_traptvc (&pt1, pp1, &vc1);   // pt1 = pp1 + vc1
  UT3D_vc_2pt (&vc1, &pt1, pp3);       // vc1 = pt2 -> pp3
  goto Fertig;


  //=================================================================
  Lvc_cx1:            // in: vc1, CX:iParl=2
  // get normal vec from vc1, ConstrPln.vz
  UT3D_vc_perp2vc (&vc1, &WC_sur_act.vz, &vc1);
  goto Fertig;


  //=================================================================
  Fertig:    // input: vc1;   todo: iRev lenStat f_tra

  if(lenStat   == YES) {
    UT3D_vc_setLength (&vc1, &vc1, lenVc);
  }

  if(iRev) {
    UT3D_vc_invert (&vc1, &vc1);
  }


  // translate
  if(f_tra != 0) UTRA_WCS_UCS_VC (&vc1, &vc1);   // WCS <- UCS  (3D <- 2D)


  //----------------------------------------------------------------
  L_exit:
  *vc_out = vc1;
  rc  = 0;

    // DEB_dump_obj__ (Typ_VC, vc_out, "ex APT_decode_vc");

  return rc;


  Error:
  return -1;


  L_noFunc:
  TX_Error(" Vectordefinition not yet implemented ..");
  goto Error;


  L_parErr:
    TX_Error("Definition Vector: Error\n");
    return -1;
}


//===========================================================================
  int APT_decode_dimen (Dimen *dim1,int aus_anz,int aus_typ[],double aus_tab[]){
//===========================================================================
// dtyp: 0=Linearmasz 1=Durchmesser 2=Leader 3=Winkelmasz

  int      irc, i1, i2;
  double   d1;
  Point    pt1;
  Point2   pt21, pt22;
  Vector2  vc20;




  // printf("APT_decode_dimen %d\n",aus_anz);
  // for(i1=0;i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }



  // Dfaults laden
  // aus_typ[0] = 170 = T_DIM
  dim1->a1   = UT_DB_LEER;
  dim1->a2   = UT_DB_LEER;
  dim1->dtyp = 0;                 // 0=Linearmasz
  dim1->hd   = 12;
  dim1->ld   = 11;
  dim1->txt  = NULL;


/*
  // check for Circ als 1 Parameter == DurchmesserMasz.
  if(aus_typ[1] == Typ_CI)
    return APT_decode_dimdia (dim1, aus_anz,aus_typ, aus_tab);
*/


  i1 = 1;


  // P1
  if(aus_typ[i1] == Typ_PT) {
    pt1 = DB_GetPoint ((long)aus_tab[i1]);
    dim1->p1 = UT2D_pt_pt3 (&pt1);
    ++i1;

  } else if((aus_typ[i1] == Typ_Val)&&(aus_typ[i1+1] == Typ_Val)) {
    dim1->p1.x = aus_tab[i1];
    dim1->p1.y = aus_tab[i1+1];
    i1 += 2;
  } 


  // P2
  if(aus_typ[i1] == Typ_PT) {
    pt1 = DB_GetPoint ((long)aus_tab[i1]);
    dim1->p2 = UT2D_pt_pt3 (&pt1);
    ++i1;

  } else if((aus_typ[i1] == Typ_Val)&&(aus_typ[i1+1] == Typ_Val)) {
    dim1->p2.x = aus_tab[i1];
    dim1->p2.y = aus_tab[i1+1];
    i1 += 2;
  }

  // P-Txt
  if(aus_typ[i1] == Typ_PT) {
    pt1 = DB_GetPoint ((long)aus_tab[i1]);
    dim1->p3 = UT2D_pt_pt3 (&pt1);
    ++i1;

  } else if((aus_typ[i1] == Typ_Val)&&(aus_typ[i1+1] == Typ_Val)) {
    dim1->p3.x = aus_tab[i1];
    dim1->p3.y = aus_tab[i1+1];
    i1 += 2;
  }

  // A1
  if(i1 >= aus_anz) goto L_ang2;
  if((aus_typ[i1] == Typ_Angle) ||
     (aus_typ[i1] == Typ_VC)) {
    irc =  APT_decode_angd__d1 (&dim1->a1, &i1, aus_typ, aus_tab);
    if(irc < 0) goto L_parErr;
    // printf(" ang=%f\n",dim1->a1);
    // dim1->a2 = dim1->a1;
  }

/*
  // A2
  if((aus_typ[i1] == Typ_Angle) ||
     (aus_typ[i1] == Typ_VC)) {
    irc =  APT_decode_angd__r1 (&dim1->a2, &i1, aus_typ, aus_tab);
    if(irc < 0) goto L_parErr;
  }
*/

  // headtypes; 0=nix, 1=<, 2=>, 3=/, 4=o;  -1=Default;
  // Default ist also 12.
  if(aus_typ[i1] == Typ_Val) {
    if(aus_tab[i1] >= 0)  dim1->hd = aus_tab[i1];
    ++i1;
  }

  // Leaderlinestypes; 0=nix, 1=normale Line.  -1=Default;
  // Default ist also 11.
  if(aus_typ[i1] == Typ_Val) {
    if(aus_tab[i1] >= 0)  dim1->ld = aus_tab[i1];
    ++i1;
  }

  // Zusatztext
  if(aus_typ[i1] == Typ_String) {
    APT_get_String (APT_spc1, APT_defTxt, aus_tab[i1]);
    // printf(" zusTxt=|%s|\n",APT_spc1);
    dim1->txt = APT_spc1;
    ++i1;
  }


  // Aufbereitung; hier machen, sonst bei jedem Redraw erforderlich !
  // printf(" a1=%f LEER=%f\n",dim1->a1,UT_DB_LEER);
  // if(dim1->a1 == (float)UT_DB_LEER) {    // kein A1: Parallelmass !!
  // ACHTUNG: obige Zeile (Vergleich float-double) geht im MS-Win nicht!
  L_ang2:
  if(dim1->a1 == dim1->a2) {    // kein A1: Parallelmass !!
    vc20.dx = dim1->p2.x - dim1->p1.x;
    vc20.dy = dim1->p2.y - dim1->p1.y;
    d1 = UT_DEGREES(UT2D_angr_vc (&vc20));
    // printf(" dim: parallelwink.=%f\n",dim1->a1);
    // if(dim1->a1 >= 180.) dim1->a1 -= 180.;
    if((d1 > 135.)&&(d1 < 315.)) {
      // printf(" >>>>>> Masztext auf die andere Seite <<<<<<<<<<<<\n");
      d1 -= 180.;
    }
    dim1->a1 = d1;


  } else {        // dim1->a1 manuell eingegeben ..
    d1 = dim1->a1;
    if((d1 > 135.)&&(d1 < 315.)) {
      // printf(" >>>>>> Masztext auf die andere Seite <<<<<<<<<<<<\n");
      d1 -= 180.;
    }
    dim1->a1 = d1;
  }




  L_exit:


  // TESTAUSG
  // DEB_dump_obj__ (Typ_Dimen, dim1, "ex-decode_dimen");



  return 0;


  L_parErr:
  TX_Error(" ParameterError DIMEN Par. Nr. %d",i1);
  return -1;

}




//===========================================================================
  int APT_decode_dimdia(Dimen *dim1,int aus_anz,int aus_typ[],double aus_tab[]){
//===========================================================================
// dtyp: 0=Linearmasz 1=Durchmesser 2=Leader 3=Winkelmasz

  int      i1, i2;
  double   rdc, d1;
  Point    *pt1;
  Point2   pc;
  Circ     *ci1;
  // Vector2  vc20;



  // printf("APT_decode_dimdia %d\n",aus_anz);
  // for(i1=0;i1<aus_anz; ++i1) {
    // printf(" %d %d\n",i1,aus_typ[i1]);
  // }


  if(aus_typ[0] != Typ_cmdNCsub) goto L_parErr;
    if((int)aus_tab[0] == T_DIMD) {
      dim1->dtyp = 1;                 // 1=DurchmesserMasz
      dim1->hd   = 12;
    } else if((int)aus_tab[0] == T_DIMR) {
      dim1->dtyp = 2;                 // 2=RadiusMasz
      dim1->hd   = 1;
    } else goto L_parErr;


  i1 = 1;

  // Kreis
  if(aus_typ[i1] != Typ_CI) goto L_parErr;
    ci1 = DB_get_CI ((long)aus_tab[i1]);
    dim1->p1 = UT2D_pt_pt3 (&ci1->pc);
    ++i1;


  // p3
  if(aus_typ[i1] != Typ_PT) goto L_parErr;
    pt1 = DB_get_PT ((long)aus_tab[i1]);
    dim1->p3 = UT2D_pt_pt3 (pt1);
    ++i1;


  // headtypes; 0=nix, 1=<, 2=>, 3=/, 4=o;
  // Default ist also 12.
  if((aus_typ[i1] == Typ_Val)    ||
     (aus_typ[i1] == Typ_modif))     {
    i2 = aus_tab[i1];
    if((i2 < 0)||(i2 > 4)) goto L_errhd;
    dim1->hd = i2;
    ++i1;
  }


  // Zusatztext
  if(aus_typ[i1] == Typ_String) {
    APT_get_String (APT_spc1, APT_defTxt, aus_tab[i1]);
    // printf(" zusTxt=|%s|\n",APT_spc1);
    dim1->txt = APT_spc1;
    ++i1;
  } else {
    dim1->txt = NULL;
  }



  // in a2 den Radius liefern
  rdc = fabs(ci1->rad);
  dim1->a2 = rdc;

  // p2 = project p3 => Circ
  UT2D_pt_tra2ptlen (&dim1->p2, &dim1->p1, &dim1->p3, rdc);

  // den textBlockWinkel => a1
  dim1->a1 =  UT_DEGREES(UT2D_angr_ptpt (&dim1->p1, &dim1->p2));
  // if(dim1->a1 >= 180.) dim1->a1 -= 180.;


/*
  // Variante innen oder aussen ?
  d1 = UT2D_len_2pt (&pc, &dim1->p3);
  if(d1 > rdc) {
    dim1->dtyp = 11;                 // 11=DurchmesserMasz.aussen
    UT2D_pt_tra2ptlen (&dim1->p1, &pc, &dim1->p3, -rdc);
    UT2D_pt_tra2ptlen (&dim1->p2, &pc, &dim1->p3, rdc);
  } else {
    dim1->dtyp = 12;                 // 12=DurchmesserMasz.innen
    UT2D_pt_tra2ptlen (&dim1->p1, &pc, &dim1->p3, rdc);
    UT2D_pt_tra2ptlen (&dim1->p2, &pc, &dim1->p3, -rdc);
  }
*/

  APT_modMax1 = 4;    // nr of arrowtypes; 0-4

    // TESTBLOCK
    // DEB_dump_obj__ (Typ_Dimen, dim1, "ex-APT_decode_dimdia");
    // END TESTBLOCK
 

  return 0;

  L_errhd:
  TX_Error(" ParameterError  min 0; max 4");
  return -1;


  L_parErr:
  TX_Error(" ParameterError DIM-dia Par. Nr. %d",i1);
  return -1;

}


//===========================================================================
  int APT_decode_ldrp (AText *atx,int aus_anz,int aus_typ[],double aus_tab[]){
//===========================================================================
// N=LDRP TextPosition [Punkt] [Linetyp] ["ZusatzText"]
// Punktkoordinaten (Kote; mit Hilfslinie, 3D-Text mit Sonderzeichen)
// GL_DrawTxtLG

  int     ii;

  *atx = AText_NUL;

  ii = 1;

  atx->aTyp = 4;  // 4=PointCoord

  if(aus_typ[ii] != Typ_PT) goto L_parErr;
  atx->p1 = DB_GetPoint ((long)aus_tab[ii]);
  ++ii;


  if(aus_typ[ii] == Typ_PT) {
    atx->p2 = DB_GetPoint ((long)aus_tab[ii]);
    ++ii;
    if(ii >= aus_anz) goto L_done;
  }


  // copy Zusatztext -> APT_spc1
  if(aus_typ[ii] == Typ_String) {
    APT_get_String (APT_spc1, APT_defTxt, aus_tab[ii]);
    atx->txt = APT_spc1;
    // printf(" zusTxt=|%s|\n",atx->txt);
    ++ii;
  }


  L_done:
  // DEB_dump_obj__ (Typ_ATXT, atx, "ex APT_decode_ldrp");


  return 0;


  L_parErr:
    TX_Error(" ParameterError LDRP");
    return -1;

}


//===========================================================================
  int APT_decode_ldrc (AText *atx,int aus_anz,int aus_typ[],double aus_tab[]){
//===========================================================================
// Leaderline + Balloon + Text
// N=LDRC TextPosition [Linienstartpunkt Linetyp] [Text]
// GL_DrawTxtLBG

  int     ii;


  printf("APT_decode_ldrc |%d| APT_prim_typ=%d\n",aus_anz,APT_prim_typ);
  for(ii=0; ii<aus_anz; ++ii)
  printf(" %d typ=%d tab=%f\n",ii,aus_typ[ii],aus_tab[ii]);


  ii = 1;

  atx->aTyp = 3;      // 3=Label-Kreis
  atx->txt  = NULL;   // no text

  if(aus_typ[ii] != Typ_PT) goto L_parErr;
  atx->p1 = DB_GetPoint ((long)aus_tab[ii]);
  atx->p1.z = 0.;    // Kreis dzt nur 2D ..
  ++ii;

  if(aus_typ[ii] == Typ_PT) {
    atx->p2 = DB_GetPoint ((long)aus_tab[ii]);
    atx->p2.z = 0.;    // Kreis dzt nur 2D ..
    ++ii;
    if(ii >= aus_anz) goto L_done;
  }

  // copy Zusatztext -> APT_spc1
  if(aus_typ[ii] == Typ_String) {
    APT_get_String (APT_spc1, APT_defTxt, aus_tab[ii]);
    atx->txt = APT_spc1;
      printf(" zusTxt=|%s|\n",atx->txt);
    ++ii;
  }

  L_done:
    // TESTBLOCK
    // DEB_dump_obj__ (Typ_ATXT, atx, "ex APT_decode_ldrc");
    // END TESTBLOCK
 
  return 0;


  L_parErr:
    TX_Error(" ParameterError LDRC");
    return -1;

}


//===========================================================================
  int APT_decode_tag (AText *atx,int aus_anz,int aus_typ[],double aus_tab[]){
//===========================================================================
// N=TAG TextPosition [Linienstartpunkt] [tagColor] [Text]
// GL_Draw_Tag

  int     ii, ip;


  // printf("APT_decode_tag %d\n",aus_anz);
  // for(ii=0;ii<aus_anz; ++ii) {
    // printf(" %d %d %f\n",ii,aus_typ[ii],aus_tab[ii]);
  // }

  *atx = AText_NUL;

  ii = 1;
  ip = 0;

  atx->aTyp  = 0;  // 0=2D-Text


  L_nxt:
  if(aus_typ[ii] == Typ_PT) {
    if(!ip) {
      atx->p1 = DB_GetPoint ((long)aus_tab[ii]);
      ++ip;
    } else {
      atx->p2 = DB_GetPoint ((long)aus_tab[ii]);
      atx->ltyp = 0;
    }
    ++ii;
    goto L_nxt;
  }

  // tagColor
  if((aus_typ[ii] == Typ_Val)   ||
     (aus_typ[ii] == Typ_modif))    {
    atx->aTyp = 2;                     // Block
    atx->col = aus_tab[ii];            // col; 1=gelb, 2=rot,a 3=gruen, 4=blau
    ++ii;
    if(ii >= aus_anz) goto L_done;
  }

  // copy Zusatztext -> APT_spc1
  if(aus_typ[ii] == Typ_String) {
    APT_get_String (APT_spc1, APT_defTxt, aus_tab[ii]);
    atx->txt = APT_spc1;
      // printf(" zusTxt=|%s|\n",atx->txt);
  }

  // if(ii < aus_anz) goto L_parErr;

  // create pointposition if no text is given ..
  if(!atx->txt) {
    APT_spc1[0] = '\0';
    UTX_add_fl_u3 (APT_spc1, atx->p1.x,atx->p1.y,atx->p1.z,',');
    atx->txt = APT_spc1;
    atx->col  = -1; // kein Block
  }


  L_done:
  APT_modMax1 = 10;    // nr of colors; 2-10


    // DEB_dump_obj__ (Typ_ATXT, atx, "ex APT_decode_tag");

  return 0;


  L_parErr:
    TX_Error(" ParameterError TAG");
    return -1;

}


//===========================================================================
  int APT_decode_ldrs (AText *atx,int aus_anz,int aus_typ[],double aus_tab[]){
//===========================================================================
// N=LDRS SymbolPos. [StartPoint Line] [symbolTyp] [Color] [EndpointVector]
// GL_Draw_Tag

  int     ii, i1;
  // Point   *pp1;
  Vector  *vc1;


  // printf("APT_decode_ldrs %d\n",aus_anz);
  // for(i1=0;i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  *atx = AText_NUL;

  ii = 1;

  atx->aTyp = 5;  // 4=PointCoord


  // SymbolPosition
  if(aus_typ[ii] != Typ_PT) goto L_parErr;
  atx->p1 = DB_GetPoint ((long)aus_tab[ii]);
  ++ii;
  if(ii >= aus_anz) goto L_done;


  // symbolTyp
  if((aus_typ[ii] == Typ_Val)   ||
     (aus_typ[ii] == Typ_modif))    {
    i1 = aus_tab[ii];
    if((i1 >= 0)&&(i1<250)) atx->aTyp = i1 + 5;  // offset 5 !  ???
    else                    goto L_parErr;
    ++ii;
    if(ii >= aus_anz) goto L_done;
  }


  // color
  if((aus_typ[ii] == Typ_Val)   ||
     (aus_typ[ii] == Typ_modif))    {
    i1 = aus_tab[ii];
    if((i1 > 0)&&(i1<256)) atx->col = i1;
    ++ii;
    if(ii >= aus_anz) goto L_done;
  }

  // for vectors ..
  if(atx->aTyp >= 4) {
    if(aus_typ[ii] == Typ_PT) {
      // pp1 = DB_get_PT ((long)aus_tab[ii]);
      // UT3D_vc_2pt ((Vector*)&atx->p2, &atx->p1, pp1);
      atx->p2 = DB_GetPoint ((long)aus_tab[ii]);
  
    } else if (aus_typ[ii] == Typ_VC) {
      // *((Vector*)&atx->p2) = DB_GetVector ((long)aus_tab[ii]);
      // atx->scl = UT3D_len_vc ((Vector*)&atx->p2);
      vc1 = DB_get_VC ((long)aus_tab[ii]);
      atx->p2 = atx->p1;
      UT3D_pt_add_vc__ (&atx->p2, vc1);
  
    } else {
      atx->p2.x = 0.;
      atx->p2.y = 0.;
      atx->p2.z = 1.;
    }
  }

/*
  // scale
  if((atx->aTyp == 4)||(atx->aTyp == 5)) {
    // f Vektoren auch die Laenge -> scale
    atx->scl = UT3D_len_vc ((Vector*)&atx->p2);
  } else {
    atx->scl = 1.;   // for Arrowhead
  }
*/

  L_done:

  APT_modMax1 = 10; // colors
  // APT_modMax2 = 9;    // nr of colors; 0-8

    // TESTBLOCK
    // DEB_dump_obj__ (Typ_ATXT, atx, "ex APT_decode_ldrs");
    // END TESTBLOCK
 

  return 0;


  L_parErr:
    TX_Error(" ParameterError LDRS");
    return -1;

}


//===========================================================================
  int APT_decode_ldr_ (Dimen *dim1,int aus_anz,int aus_typ[],double aus_tab[]){
//===========================================================================
// N=LDR TextPosition [Startpunkt L1] [Startpunkt L2] [Textwinkel] [Text]
// GL_DrawLdr


  int      irc, i1, i2;
  double   d1;
  Point    *pt1;
  // Vector2  vc20;


  // printf("APT_decode_ldr_ %d\n",aus_anz);
  // for(i1=0;i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }



  // Defaults laden
  // aus_typ[0] = 170 = T_DIM
  dim1->a1   = 0.;
  dim1->a2   = UT_DB_LEER;
  dim1->dtyp = 21;                 // 21=Leader
  dim1->hd   = 1;
  dim1->ld   = 0;
  dim1->txt  = NULL;


  i1 = 1;

  // P1  (Arrow)
  if(aus_typ[i1] != Typ_PT) goto L_parErr;
    pt1 = DB_get_PT ((long)aus_tab[i1]);
    dim1->p1 = UT2D_pt_pt3 (pt1);
    ++i1;


  // P2
  if(aus_typ[i1] != Typ_PT) goto L_parErr;
    pt1 = DB_get_PT ((long)aus_tab[i1]);
    dim1->p2 = UT2D_pt_pt3 (pt1);
    ++i1;


  // P3
  if(aus_typ[i1] == Typ_PT) {
    pt1 = DB_get_PT ((long)aus_tab[i1]);
    dim1->p3 = UT2D_pt_pt3 (pt1);
    ++i1;
  } else dim1->p3.x = UT_DB_LEER;  //


  // Angle
  if((aus_typ[i1] == Typ_Angle) ||
     (aus_typ[i1] == Typ_VC)) {
    irc =  APT_decode_angd__d1 (&dim1->a1, &i1, aus_typ, aus_tab);
    if(irc < 0) goto L_parErr;

  } else {
    // keine Angabe: Richtung von p1-p2 oder p2-p3
    if(dim1->p3.x == UT_DB_LEER) {
      d1 = UT2D_angr_ptpt ((Point2*)&dim1->p1, (Point2*)&dim1->p2);
    } else {
      d1 = UT2D_angr_ptpt ((Point2*)&dim1->p2, (Point2*)&dim1->p3);
    }
    dim1->a1 = UT_DEGREES(d1);
  }


  // Typ Head (arrow, balloon ..)
  if(aus_typ[i1] == Typ_Val) {
    dim1->hd = aus_tab[i1];
    ++i1;
  }


  // Zusatztext
  if(aus_typ[i1] == Typ_String) {
    APT_get_String (APT_spc1, APT_defTxt, aus_tab[i1]);
    // printf(" zusTxt=|%s|\n",APT_spc1);
    dim1->txt = APT_spc1;
    ++i1;
  }


  // TESTAUSG
  // DEB_dump_obj__ (Typ_Dimen, dim1, "_decode_ldr");

  return 0;



  L_parErr:
  TX_Error(" ParameterError LDR Par. Nr. %d",i1);
  return -1;

}


//===========================================================================
  int APT_decode_dima (Dimen *dim1,int aus_anz,int aus_typ[],double aus_tab[]){
//===========================================================================
// 

  int      irc, i1, i2;
  Point    pt1;
  Vector2  vc20;




  // printf("APT_decode_dima %d\n",aus_anz);
  // for(i1=0;i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  // aus_typ[0] = 170 = T_DIMA
  dim1->a1   = UT_DB_LEER;
  dim1->a2   = UT_DB_LEER;
  dim1->dtyp = 3;                  // 3=Winkelmasz
  dim1->hd   = 12;
  dim1->ld   = 11;
  dim1->txt  = NULL;

  

  i1 = 1;


  // P1
  if(aus_typ[i1] != Typ_PT) goto L_parErr;
    pt1 = DB_GetPoint ((long)aus_tab[i1]);
    dim1->p1 = UT2D_pt_pt3 (&pt1);
    ++i1;


  // Winkel Hilfslinie 1
  irc =  APT_decode_angd__r1 (&dim1->a1, &i1, aus_typ, aus_tab);
  if(irc < 0) goto L_parErr;


  // P2
  if(aus_typ[i1] != Typ_PT) goto L_parErr;
    pt1 = DB_GetPoint ((long)aus_tab[i1]);
    dim1->p2 = UT2D_pt_pt3 (&pt1);
    ++i1;


  // Winkel Hilfslinie 2
  irc =  APT_decode_angd__r1 (&dim1->a2, &i1, aus_typ, aus_tab);
  if(irc < 0) goto L_parErr;


  // P-Txt
  if(aus_typ[i1] != Typ_PT) goto L_parErr;
  pt1 = DB_GetPoint ((long)aus_tab[i1]);
  dim1->p3 = UT2D_pt_pt3 (&pt1);
  ++i1;
  if(i1 >= aus_anz) goto L_exit;


  // headtypes (optional)
  // if(aus_typ[i1] != Typ_Val) goto L_parErr;
  if(aus_typ[i1] == Typ_Val) {
// TODO: make function to test 2 digits in range 0-4   ILIMCK2 UTI_dig_ck_range
    dim1->hd = aus_tab[i1];
    ++i1;
    if(i1 >= aus_anz) goto L_exit;

    // type leaderlines (optional)
    // if(aus_typ[i1] != Typ_Val) goto L_parErr;
    if(aus_typ[i1] == Typ_Val) {
// TODO: make function to test 2 digits in range 0-1   ILIMCK2 UTI_dig_ck_range
      dim1->ld = aus_tab[i1];
      ++i1;
      if(i1 >= aus_anz) goto L_exit;
    }
  }


  // Zusatztext (optional)
  if(aus_typ[i1] == Typ_String) {
    APT_get_String (APT_spc1, APT_defTxt, aus_tab[i1]);
    // printf(" zusTxt=|%s|\n",APT_spc1);
    dim1->txt = APT_spc1;
    ++i1;
  }


  // Aufbereitung; hier machen, sonst bei jedem Redraw erforderlich !
  L_exit:


    // TESTBLOCK
    // DEB_dump_obj__ (Typ_Dimen, dim1, "ex-APT_decode_dima");
    // END TESTBLOCK


  return 0;

 

  L_parErr:
  TX_Error(" ParameterError DIMA Par. Nr. %d",i1);
  return -1;

}

/*
//=============================================================================
  int APT_decode_tex (TexBas *tex,int aus_anz,int aus_typ[],double aus_tab[]){
//=============================================================================
// decode Texture;
// N = IMG Filename

  int    i1;


  printf("APT_decode_tex %d\n",aus_anz);
  for(i1=0;i1<aus_anz; ++i1) {
    printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  }

  // filename of texture 
  if(aus_typ[1] != Typ_String) goto L_parErr;
  APT_get_String (APT_spc1, APT_defTxt, aus_tab[1]);  // get string > APT_spc1
    printf(" fNam of tex=|%s|\n",APT_spc1);
  tex->fnam = &APT_spc1;

  tex->dli   = -1;
  tex->texNr = -1;
  tex->xSiz  = -1;
  tex->ySiz  = -1;

  DEB_dump_obj__ (Typ_TEXB, tex, "ex APT_decode_tex");

  return 0;


  L_parErr:
  TX_Error(" ParameterError TEX");

  return -1;


}
*/

//=============================================================================
  int APT_decode_img (AText *atx,int aus_anz,int aus_typ[],double aus_tab[]){
//=============================================================================
// N = IMG p1 [p2] [LineAtt] Filename [Scale]           // 2D-Image, immer normal

// see APT_decode_tag: N = TAG Labeltyp p1 [p2 LineAtt] Text    // TextLabel

  int    i1, ii;
  char   cBuf[32];


  // printf("APT_decode_img %d\n",aus_anz);
  // for(i1=0;i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }

  *atx = AText_NUL;

  // Def:
  atx->aTyp = 1;

  ii = 1;
/*
  // IMG hat keinen Labeltyp
  if((int)aus_tab[0] == T_IMG) {
    atx->typ = 1;
    goto L_P1;
  }


  // N=TAG:   1=Labeltyp
  if(aus_typ[ii] != Typ_Val) goto L_parErr;
  // erste Ziffer ist Modus; (atx.typ)
  i1 = aus_tab[ii];
  sprintf(cBuf, "%d", i1);
  atx->typ = cBuf[0] - '0';
  // zweite Ziffer ist Farbe des Label (atx.col)
  if(strlen(cBuf) < 1) atx->col = 0;
  else {
    atx->col = atoi (&cBuf[1]);
  }
  if(atx->typ == 0) atx->col = -1; // kein Block !
  // printf(" typ=%d col=%d |%s|\n",atx->typ,atx->col,cBuf);
  ++ii;


  // Typ=1 ist Image; wird aber via "N=IMG" definiert; Error.
  if(atx->typ == 1) goto L_parErr;
*/



  // Position img ---------------------------------
  L_P1:
  if(aus_typ[ii] != Typ_PT) goto L_parErr;
  atx->p1 = DB_GetPoint ((long)aus_tab[ii]);
  ++ii;


  // Position leaderline ---------------------------------
  // Leaderline; atx.p2 und atx.ltyp
  if(aus_typ[ii] == Typ_PT) {
    atx->p2 = DB_GetPoint ((long)aus_tab[ii]);
    ++ii;
    atx->ltyp = 0;

  } else {
    atx->p2 = UT3D_PT_NUL;
    atx->ltyp = -1;
  }
  // printf(" ltyp=%d\n",atx->ltyp);



  // linetyp of leaderline ------------------------
  if((aus_typ[ii] == Typ_Val)    ||
     (aus_typ[ii] == Typ_modif))     {
    atx->ltyp = aus_tab[ii];;
    ++ii;
  }


  // ImageFilename ------------------------
  if(aus_typ[ii] == Typ_String) {
    APT_get_String (APT_spc1, APT_defTxt, aus_tab[ii]);
    atx->txt = APT_spc1;
    // printf(" zusTxt=|%s|\n",atx->txt);
    ++ii;
  }


  // ImageScale ------------------------
  if((aus_typ[ii] == Typ_Val)&&(aus_anz > ii)) {
    atx->scl = aus_tab[ii];
  } 


  // APT_modMax1 = 4;    // nr of linetypes; 0-3
  APT_modMax1 = DL_GetAttNr ();   // nr of defined linetypes


  // DEB_dump_obj__ (Typ_ATXT, atx, "ex APT_decode_img");

  return 0;


  L_parErr:
  TX_Error(" ParameterError TAG");

  return -1;


}


//============================================================================
  int APT_decode_note (ObjGX *oxo,int aus_anz,int aus_typ[],double aus_tab[]){
//============================================================================

  int   i1;
  ObjGX *ox1;


  // printf("APT_decode_note %d\n",aus_anz);
  // for(i1=0;i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }



  if(aus_typ[0] == Typ_GTXT) {        // Typ_ATXT/Typ_GTXT/Typ_Dimen
    ox1 = DB_GetGTxt ((long)aus_tab[0]);
    *oxo = *ox1;

  } else {
    TX_Print("APT_decode_note E001 %d",aus_typ[0]);
  }


  return 0;

}
 

//==============================================================================
  int APT_decode_dim3 (Dim3 *ds,int aus_anz,int aus_typ[],double aus_tab[]){
//==============================================================================
/*

MP1 = Maszpunkt1
MP2 = Maszpunkt2
TP  = Textpunkt - Input, in der ConstrPlane.
MP3 = der echte, errechnete Textpunkt
MHL = Maszhilfslinie
ML  = Maszlinie
MR  = Maszrichtung; X od Y od Z od P (parallel)
SL  = Sichtline; ein Vektor

Die MHL's muessen in einer Ebene normal auf die MR durch MP1 od MP2
 liegen; die ML muss parallel zur MR liegen.

Ablauf:
AP_PT2EyePln:
  Mittelpunkt zwischen MP1/MP2; durch diesen eine Ebene normal auf MR;
    der Durchstoszpunkt von TP+SL ist der MP3
GL_DrawDim3:
  Durch den MP3 nun eine Linie in Richtg MR;
  durch MP1 und MP2 eine Ebene normal auf MR; die Durchstoszpunkte sind
  die Endpunkte der zugehoerigen MHL.

bp wird scheinbar fuer die Textrichtung benutzt ..

Offen:
- kann man den Masztext als Billboard machgen (dass er sich immer nach
  der gleichen Seite dreht ?
- oder wenigstens als Alfatext in der Maszlinienmitte



See AP_PT2EyePln
*/


  int     irc, ind, i1, i2;
  char    *cp1;
  Point   *pp1, *pp2, *pp3;



  // printf("APT_decode_dim3 %d\n",aus_anz);
  // for(i1=0;i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }



  if(aus_typ[1] != Typ_PT) goto L_parErr;
  ds->ip1 = aus_tab[1];

  if(aus_typ[2] != Typ_PT) goto L_parErr;
  ds->ip2 = aus_tab[2];

  if(aus_typ[3] != Typ_PT) goto L_parErr;
  ds->ipt = aus_tab[3];


  // defaults setzen
  ds->dtyp = ' ';    // def=parallel
  ds->txt  = NULL;   // defText


  i1 = 4;
  if((aus_typ[i1] == Typ_String) ||
     (aus_typ[i1] == Typ_Txt))      {
    i2 = aus_tab[i1];
    cp1 = &APT_defTxt[i2];
    // printf(" plnTyp=|%s|\n",cp1);
    ds->dtyp = cp1[1];      // zB "Z0"
    ds->bp   = cp1[2]-'0';  // make int from char
    ++i1;
  }



  // Zusatztext laden ..
  if(aus_typ[i1] == Typ_String) {
    i2 = aus_tab[i1];
    APT_get_String (APT_spc1, APT_defTxt, aus_tab[i1]);
    // printf(" zusTxt=|%s|\n",APT_spc1);
    ds->txt = APT_spc1;
    ++i1;

  }



  //----------------------------------------------------------------
  // pp1 = DB_get_PT (ds->ip1);
  // pp2 = DB_get_PT (ds->ip2);
  // pp3 = DB_get_PT (ds->ipt);
    // DEB_dump_obj__ (Typ_PT, pp3, "  pt:");



    // DEB_dump_obj__ (Typ_Dim3, ds, "_decode_dim3");

  return 0;


  L_parErr:
    TX_Error(" Parameter decode Dim3");
    return -1;

}


//==============================================================================
  int APT_decode_gtxt (GText *gtx1,int aus_anz,int aus_typ[],double aus_tab[]){
//==============================================================================
// decode normal Text.


  int       irc, ind, i1, ii;


  // printf("APT_decode_gtxt %d\n",aus_anz);
  // for(i1=0;i1<aus_anz; ++i1) {
    // printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }




  // Posi. links unten
  if(aus_typ[0] != Typ_PT) goto L_parErr;
    gtx1->pt = DB_GetPoint ((long)aus_tab[0]);


  i1 = 1;


  // size
  if(aus_typ[i1] == Typ_Val)  {
    gtx1->size = aus_tab[i1];
    if(gtx1->size < 0.001)  gtx1->size = 0.;    // size 0 ist Defaultsize
    ++i1;
  } else gtx1->size = 0.;



  // Direction.
  if((aus_typ[i1] == Typ_Angle) ||
     (aus_typ[i1] == Typ_VC)) {
    irc =  APT_decode_angd__d1 (&gtx1->dir, &i1, aus_typ, aus_tab);
    if(irc < 0) goto L_parErr;

  } else {
    gtx1->dir = 0.;
  }



  // Text
  if(aus_typ[i1] != Typ_String) goto L_parErr;
    APT_get_String (APT_spc1, APT_defTxt, aus_tab[i1]);
    gtx1->txt = APT_spc1;
      // printf(" zusTxt=|%s|\n   ",APT_spc1);
      // for(i1=0;i1<strlen(APT_spc1);++i1) printf(" %d",APT_spc1[i1]);
      // printf("\n");



  // Displayable Characters: 32-128; change special-characters into '_'
  ii = strlen(APT_spc1);
  for(i1=0; i1<ii; ++i1) {
    //if((APT_spc1[i1] >= 32) && (APT_spc1[i1] <= 127)) continue;
    if(APT_spc1[i1] >= 32) continue;
    APT_spc1[i1] = '_';
    //if((APT_spc1[i1] < 32) || (APT_spc1[i1] > 128)) APT_spc1[i1] = '_';
  }


    // TESTAUSG
    // DEB_dump_obj__ (Typ_GTXT, gtx1, "_decode_gtxt");

  return 0;




  L_parErr:
    TX_Error(" Parameterfehler GrafText\n");
    return -1;
  
}



//===========================================================================
  int APT_solv3d_HIX (Point *pta, int PtAnz) {
//===========================================================================
/*
find max X-Wert
Rückgabewert ist der gefundene Index.
*/

  int     i1, ind;
  double  d1;

  d1 = UT_VAL_MIN;
  ind  = -1;


  for(i1=0; i1<PtAnz; ++i1) {
    if(pta[i1].x > d1) {
      d1 = pta[i1].x;
      ind = i1;
    }
  }

  // printf("ex APT_solv3d_HIX %d\n",ind);
  return ind;

}


//===========================================================================
  int APT_solv3d_HIY (Point *pta, int PtAnz) {
//===========================================================================
/*
find max Y-Wert
Rückgabewert ist der gefundene Index.
*/

  int     i1, ind;
  double  d1;

  d1 = UT_VAL_MIN;
  ind  = -1;


  for(i1=0; i1<PtAnz; ++i1) {
    if(pta[i1].y > d1) {
      d1 = pta[i1].y;
      ind = i1;
    }
  }

  // printf("ex APT_solv3d_HIY %d\n",ind);
  return ind;

}


//===========================================================================
  int APT_solv3d_HIZ (Point *pta, int PtAnz) {
//===========================================================================
/*
find max Z-Wert
Rückgabewert ist der gefundene Index.
*/

  int     i1, ind;
  double  d1;

  d1 = UT_VAL_MIN;
  ind  = -1;


  for(i1=0; i1<PtAnz; ++i1) {
    if(pta[i1].z > d1) {
      d1 = pta[i1].z;
      ind = i1;
    }
  }

  // printf("ex APT_solv3d_HIZ %d\n",ind);
  return ind;

}

//===========================================================================
  int APT_solv3d_LOX (Point *pta, int PtAnz) {
//===========================================================================
/*
find min X-Wert
Rückgabewert ist der gefundene Index.
*/

  int     i1, ind;
  double  d1;

  d1 = UT_VAL_MAX;
  ind  = -1;


  for(i1=0; i1<PtAnz; ++i1) {
    if(pta[i1].x < d1) {
      d1 = pta[i1].x;
      ind = i1;
    }
  }

  // printf("ex APT_solv3d_LOX %d\n",ind);
  return ind;

}



//===========================================================================
  int APT_solv3d_LOY (Point *pta, int PtAnz) {
//===========================================================================
/*
find min Y-Wert
Rückgabewert ist der gefundene Index.
*/

  int     i1, ind;
  double  d1;

  d1 = UT_VAL_MAX;
  ind  = -1;


  for(i1=0; i1<PtAnz; ++i1) {
    if(pta[i1].y < d1) {
      d1 = pta[i1].y;
      ind = i1;
    }
  }

  // printf("ex APT_solv3d_LOY %d\n",ind);
  return ind;

}





//===========================================================================
  int APT_solv3d_LOZ (Point *pta, int PtAnz) {
//===========================================================================
/*
find min Z-Wert
Rückgabewert ist der gefundene Index.
*/

  int     i1, ind;
  double  d1;

  d1 = UT_VAL_MAX;
  ind  = -1;


  for(i1=0; i1<PtAnz; ++i1) {
    if(pta[i1].z < d1) {
      d1 = pta[i1].z;
      ind = i1;
    }
  }

  // printf("ex APT_solv3d_LOZ %d\n",ind);
  return ind;

}



//===========================================================================
  int APT_solv3D_mod (Point pa[], int ptNr, int modTab[], int modNr) {
//===========================================================================
// 2D-Version ist APT_solv_mod + APT_solv_mod_1
// dzt nur modNr = 1 !!!

  int   i1, ModAct;


  ModAct = modTab[0];




  if(ModAct < Typ_modHIX) {
    i1 = ModAct;
    goto L_fertig;

  } else if(ModAct == Typ_modHIX) {
    i1 = APT_solv3d_HIX (pa, ptNr);
    goto L_fertig;
    
  } else if (ModAct == Typ_modLOX) {
    i1 = APT_solv3d_LOX (pa, ptNr);
    goto L_fertig;
    
  } else if(ModAct == Typ_modHIY) {
    i1 = APT_solv3d_HIY (pa, ptNr);
    goto L_fertig;
    
  } else if (ModAct == Typ_modLOY) {
    i1 = APT_solv3d_LOY (pa, ptNr);
    goto L_fertig;
    
  } else if(ModAct == Typ_modHIZ) {
    i1 = APT_solv3d_HIZ (pa, ptNr);
    goto L_fertig;
    
  } else if (ModAct == Typ_modLOZ) {
    i1 = APT_solv3d_LOZ (pa, ptNr);
    goto L_fertig;
    
  } else {
    TX_Error("Modfifier unknown");
    return -1;
  }


  L_fertig:
  // printf("ex APT_solv3D_mod ind=%d mod=%d\n",i1,ModAct);
  return i1;


}


//=============================================================================
  int APT_decode_pt (Point *pt_out,int aus_anz,int aus_typ[],double aus_tab[]){
//=============================================================================
// APT_decode_pt          decode point 
//  Output:
//    retCode        -3 obj not complete

  int              i1, i2, irc, modFlag, imod, ptNr, oNr, modTab[2], mtyp;
  long             il1, il2;
  double           d1, d2, d3, rad, da[20];
  void             *vp1;
  Point            pt0, pt1, pt2, pa[16];
  Point2           pt20, pt21, pt22, pt23, ptArr[2];
  Vector           vc1, vc2, vc3;
  Vector2          vc20;
  Line             ln1, ln2;
  Line2            ln20, ln21;
  Circ             ci1, ci2, *cip;
  CurvBSpl         *cvs;
  CurvRBSpl        *cvrs;
  Mat_4x3          trmat;
  Plane            pl1;
  ObjGX            ox1, *ox1p, ox2, *ox2p, *oxTab;
  Memspc           memSeg1;
  ModelRef         *mdr;
  char             obj1[OBJ_SIZ_MAX];





  // printf("APT_decode_pt |%d|\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1)
  // printf("  %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);


  irc = 0;


  // letzter Wert MOD(): -> imod einlesen u entfernen
  // first value = 1
  if(aus_typ[aus_anz-1] == Typ_modif) {
    --aus_anz;
    imod = aus_tab[aus_anz];
    APT_prim_seg = imod;
    modFlag = 1;
  } else {
    imod = 0;
    APT_prim_seg = -1;
    modFlag = 0;
  }
    // printf(" modFlag=%d imod=%d\n",modFlag,imod);


  // save prim.Obj (das Obj auf dem der Punkt liegt)
  APT_prim_typ = aus_typ[0];
  APT_prim_ind = aus_tab[0];
  APT_prim_par = UT_DB_LEER;
  APT_prim_sg2 = -1;








  //==================================================================
  if        (aus_anz == 1)     {
  //==================================================================


    //-----------------------------------------------------------------
    // P = P               // copy point
    if      (aus_typ[0] == Typ_PT)            {

      pt1 = DB_GetPoint ((long)aus_tab[0]);
      goto Fertig3D;





    //-----------------------------------------------------------------
    // P = L [MOD]        // midpoint, endpoints ..     OBSOLETE - 
// OBSOLETE - replace with P = L [PTS]
    } else if(aus_typ[0] == Typ_LN)            {

      APT_modMax1 = 3;                          // 2013-03-17

      ln1 = DB_GetLine ((long)aus_tab[0]);

      // P=L MOD     Anf-/Endpkt
      if(modFlag > 0) {
        MSG_ERR__ (ERR_obsolete, "change MOD to PTS");
        if(imod == 1) pt1 = ln1.p1;
        else if(imod == 2) pt1 = ln1.p2;
        else if(imod == 3) UT3D_pt_mid2pt (&pt1, &ln1.p1, &ln1.p2);
        goto Fertig3D;
      }

      // UT3D_pt_mid2pt (&pt1, &ln1.p1, &ln1.p2); // P=L    Mittelpunkt
      pt1 = ln1.p1;  // P=L  - startpoint
      goto Fertig3D;





    //-----------------------------------------------------------------
    // P = C [MOD]         (C-Mittelpt)
// OBSOLETE - replace with P = C [PTS]
    } else if(aus_typ[0] == Typ_CI)            {

      APT_modMax1 = 3;

      ci1 = DB_GetCirc ((long)aus_tab[0]);

      // P = C mod           // Anf od End
      if(modFlag > 0) {
        MSG_ERR__ (ERR_obsolete, "change MOD to PTS");
        if(imod == 1) pt1 = ci1.p1;
        else if(imod == 2) pt1 = ci1.p2;
        else if(imod == 3) pt1 = ci1.pc;
        goto Fertig3D;
      }

      // P = C               (C-Mittelpt)
      pt1 = ci1.pc;
      goto Fertig3D;





    //-----------------------------------------------------------------
    // P = S [MOD]                         // Anf od End
// OBSOLETE - replace with P = S [PTS]
    } else if(aus_typ[0] == Typ_CV)           {

      if(imod)
        MSG_ERR__ (ERR_obsolete, "change MOD to PTS");

      il1 = aus_tab[0];
      ox1p = DB_GetCurv (il1);
      mtyp = ox1p->form;

      APT_modMax1 = UTO_ptnr_std_obj (ox1p->form, il1);
        // printf(" curv[%d]form=%d siz=%d\n",(int)aus_tab[0],mtyp,ox1p->siz);

      i1 = imod;
      // Typ_CVBSP, Typ_CVRBSP: MOD(1)=StartPt; MOD(2)=EndPt

      if       ((mtyp == Typ_CVELL)  ||
                (mtyp == Typ_CVCLOT))   {
        // MOD -> Ptyp_
        // 1, 2, 3->32, 4->64, 5->65
        if(imod == 3) i1 = 32;
        if(imod > 3) i1 += 60;

      } else if(mtyp == Typ_CVPOL)      {
        // Polygon: get controlpoint
        i1 = -imod;
      }


      irc = UT3D_ptvcpar_std_obj (pt_out, NULL, NULL, 0, i1, Typ_ObjGX, ox1p);
      goto Exit;

/*
      if(ox1p->typ == Typ_CVELL) {
        if(modFlag == 0) { // kein MOD; centerpoint
          *pt_out = ((CurvElli*)ox1p->data)->pc;
          goto Exit;
        }
        // handle MOD
        if(imod == 1) {
          *pt_out = ((CurvElli*)ox1p->data)->p1;

        } else if(imod == 2) {
          *pt_out = ((CurvElli*)ox1p->data)->p2;

        } else if(imod == 3) {   // center
          *pt_out = ((CurvElli*)ox1p->data)->pc;

        } else if(imod == 3) {   // focus on va                  // 2013-03-17
          d1 = UT3D_lne_ell (ox1p->data);
          UT3D_pt_traptvclen (pt_out, &((CurvElli*)ox1p->data)->pc,
                                      &((CurvElli*)ox1p->data)->va, d1);
        } else if(imod == 4) {   // focus on -va
          d1 = UT3D_lne_ell (ox1p->data);
          UT3D_pt_traptvclen (pt_out, &((CurvElli*)ox1p->data)->pc,
                                      &((CurvElli*)ox1p->data)->va, -d1);
        }
        UT3D_ptvcpar_std_obj (pt_out, NULL, NULL, 0, mType, Typ_CVELL, ox1p->data);
        APT_modMax1 = 5;                                         // 2013-03-17
        goto Exit;


      } else if(ox1p->typ == Typ_CVBSP) {
        // UT3D_pt_evalparCv (&pa[0], ox1p->data, ((CurvBSpl*)ox1p->data)->v0);
        // UT3D_pt_evalparCv (&pa[1], ox1p->data, ((CurvBSpl*)ox1p->data)->v1);
        cvs = (CurvBSpl*)ox1p->data;
        pt1 = cvs->cpTab[imod];
        APT_modMax1 = cvs->ptNr;                                // 2013-03-17
        goto Fertig3D;


      } else if(ox1p->typ == Typ_CVRBSP) {
        // UT3D_pt_evalparCv (&pa[0], ox1p->data, ((CurvBSpl*)ox1p->data)->v0);
        // UT3D_pt_evalparCv (&pa[1], ox1p->data, ((CurvBSpl*)ox1p->data)->v1);
        cvrs = (CurvRBSpl*)ox1p->data;
        pt1 = cvrs->cpTab[imod];
        APT_modMax1 = cvrs->ptNr;                                // 2013-03-17
        goto Fertig3D;


      } else if(ox1p->typ == Typ_CVPOL) {
        // pa[0] = ((CurvPoly*)ox1p->data)->cpTab[0];
        // i1 = ((CurvPoly*)ox1p->data)->ptNr - 1;
        // pa[1] = ((CurvPoly*)ox1p->data)->cpTab[i1];
        pt1 = ((CurvPoly*)ox1p->data)->cpTab[imod];
        APT_modMax1 = ((CurvPoly*)ox1p->data)->ptNr;            // 2013-03-17
        goto Fertig3D;


      } else if(ox1p->typ == Typ_CVTRM) {
        // UT3D_pt_endptccv (pa, ox1p);
        if(imod == 0) {  // startpt
          irc = UT3D_ptvcpar_std_obj (&pt1, NULL, NULL, 0, Ptyp_start, Typ_ObjGX, ox1p);
        } else if(imod == 1) {  // end Pt
          irc = UT3D_ptvcpar_std_obj (&pt1, NULL, NULL, 0, Ptyp_end, Typ_ObjGX, ox1p);
        }
        APT_modMax1 = 2;
        goto Fertig3D;


      } else {
        goto L_noFunc;
      }

      pt1 = pa[imod];
      goto Fertig3D;
*/





    //-----------------------------------------------------------------
    // P = X / Y / Z
    } else if(aus_typ[0] == Typ_XVal)            {
      pt1    = WC_sur_act.po;
      vc1    = UT3D_VECTOR_NUL;
      vc1.dx = aus_tab[0];
      goto EX_PTVC;

    } else if(aus_typ[0] == Typ_YVal)            {
      pt1    = WC_sur_act.po;
      vc1    = UT3D_VECTOR_NUL;
      vc1.dy = aus_tab[0];
      goto EX_PTVC;

    } else if(aus_typ[0] == Typ_ZVal)            {
      pt1    = WC_sur_act.po;
      vc1    = UT3D_VECTOR_NUL;
      vc1.dz = aus_tab[0];
      goto EX_PTVC;



    //-----------------------------------------------------------------
    // P = R                 (origin of Plane)
    } else if(aus_typ[0] == Typ_PLN)            {
      DB_GetRef (&pl1, (long)aus_tab[0]);
      *pt_out = pl1.po;
      APT_modMax1 = 0;                          // 2013-03-17
      goto Exit;


    //-----------------------------------------------------------------
    // P = M                 (origin of subModel)
    } else if(aus_typ[0] == Typ_Model)            {
      mdr = DB_get_ModRef ((long)aus_tab[0]);
      *pt_out = mdr->po;
      APT_modMax1 = 0;
      goto Exit;


    //-----------------------------------------------------------------
    // P = N                 // point from text ..
    } else if(aus_typ[0] == Typ_Note)            {
      ox1p = DB_GetGTxt ((long)aus_tab[0]);
      if(ox1p->form == Typ_GTXT) {
        *pt_out = ((GText*)ox1p->data)->pt;
        APT_modMax1 = 0;
        goto Exit;
      } else goto Error;


    //-----------------------------------------------------------------
    } else goto Error;

  



  //==================================================================
  } else if(aus_anz == 2)        {
  //==================================================================

    // test if ato-1 == geom-obj
    if(aus_typ[0] < Typ_SymB) {
      

      // yes, ato-1 is geom-obj; test if also ato-2 == geom-obj
      if(aus_typ[1] < Typ_SymB) {

        // yes, bring first two geom.objects into correct sequence (D P L C S R A N)
        if(aus_typ[0] > aus_typ[1]) ATO_swap (aus_typ, aus_tab, 0, 1);

      } else {
        // yes, ato-1 is geom-obj; handle PTS() and PTI()

        //----------------------------------------------------------------
        if(aus_typ[1] == Typ_PTS) {        //  P = LCS PTS()
          // see Ptyp_def, Ptyp_start, Ptyp_end
          i1 = (int)aus_tab[1];
          // get obj (mtyp,vp1) from ato
          mtyp = UTO__dbo (&vp1, &oNr, aus_typ[0], (long)aus_tab[0]);
          if(mtyp <= 0) goto L_noFunc;
            // DEB_dump_obj__(mtyp, vp1, "decode_pt-2-PTS");
          // get point from geom-obj (mtyp,vp1);
          irc = UT3D_ptvcpar_std_obj (&pt1, NULL, NULL, 0, i1, mtyp, vp1);
          if(irc < 0) goto L_noFunc;
          APT_modMax1 = irc;
          goto EX_PT;  // pt1

        //----------------------------------------------------------------
        } else if(aus_typ[1] == Typ_PTI) {          //  P = LCS PTI()
          // get indexed-point from geom-obj
          i1 = (int)aus_tab[1];
          // get obj (mtyp,vp1) from ato
          mtyp = UTO__dbo (&vp1, &oNr, aus_typ[0], (long)aus_tab[0]);
          if(mtyp <= 0) goto L_noFunc;
            // DEB_dump_obj__(mtyp, vp1, "decode_pt-2-PTI");
          // get point from geom-obj (mtyp,vp1);
          irc = UT3D_ptvcpar_ipt_obj (&pt1, NULL, NULL, i1, mtyp, vp1);
          if(irc < 0) goto L_noFunc;
          APT_modMax1 = irc;
          goto EX_PT;  // pt1
        }
      }
    }
      // printf(" _decode_pt-2-types %d %d\n",aus_typ[0],aus_typ[1]);



    //-----------------------------------------------------------------
    // P = VAL VAL                              // X-coord Y-coord
    if  ((aus_typ[0] == Typ_Val)     &&
         (aus_typ[1] == Typ_Val))         {
      pt1.x = aus_tab[0];
      pt1.y = aus_tab[1];
      pt1.z = 0.;
      goto EX_PT;


    //-----------------------------------------------------------------
    // P = X/Y/Z   X/Y/Z          (2 doubles)
    } else if((aus_typ[0] >= Typ_XVal)&&(aus_typ[0] <= Typ_ZVal)   &&
              (aus_typ[1] >= Typ_XVal)&&(aus_typ[1] <= Typ_ZVal))    {
      pt1   = UT3D_PT_NUL;
      if(aus_typ[0] == Typ_XVal) pt1.x += aus_tab[0];
      if(aus_typ[0] == Typ_YVal) pt1.y += aus_tab[0];
      if(aus_typ[0] == Typ_ZVal) pt1.z += aus_tab[0];
      if(aus_typ[1] == Typ_XVal) pt1.x += aus_tab[1];
      if(aus_typ[1] == Typ_YVal) pt1.y += aus_tab[1];
      if(aus_typ[1] == Typ_ZVal) pt1.z += aus_tab[1];
      goto EX_PT;  // pt1


    //-----------------------------------------------------------------
    } else if(aus_typ[0] == Typ_VC) {
      //-----------------------------------------------------------------
      vc1 = DB_GetVector ((long)aus_tab[0]);

      // P = D P     (P D)                        // point = point + vector
      if(aus_typ[1] == Typ_PT)      {
        pt1 = DB_GetPoint ((long)aus_tab[1]);
        goto L_P__P_D;   // pt1, vc1


      //-----------------------------------------------------------------
      // P = D ??
      } else goto L_noFunc;



    //-----------------------------------------------------------------
    } else if(aus_typ[0] == Typ_PT) {
      pt0 = DB_GetPoint ((long)aus_tab[0]);


      //----------------------------------------------------------------
      // P = P   X/Y/Z
      if((aus_typ[1] >= Typ_XVal)&&(aus_typ[1] <= Typ_ZVal))  {
        vc1 = UT3D_VECTOR_NUL;
        pt1 = pt0;
        if(aus_typ[1] == Typ_XVal) pt1.x += aus_tab[1];
        if(aus_typ[1] == Typ_YVal) pt1.y += aus_tab[1];
        if(aus_typ[1] == Typ_ZVal) pt1.z += aus_tab[1];
        goto EX_PTVC;  // pt1+vc1


      //-----------------------------------------------------------------
      // P = P P                                   // midpoint
      } else if(aus_typ[1] == Typ_PT)         {
        pt2 = DB_GetPoint ((long)aus_tab[1]);
        UT3D_pt_mid2pt (&pt1, &pt0, &pt2);
        goto Fertig3D;


      //-----------------------------------------------------------------
      // P = P L [MOD]               // proj pt -> line
      } else if(aus_typ[1] == Typ_LN)      {
        ln1 = DB_GetLine ((long)aus_tab[1]);
        UT3D_pt_projptln (&pt1, NULL, NULL, &pt0, &ln1);
        APT_modMax1 = 1;
        goto Fertig3D; // pt1


      //-----------------------------------------------------------------
      // P = P C [MOD]               // proj pt -> circ
      } else if(aus_typ[1] == Typ_CI)      {
        ci1 = DB_GetCirc ((long)aus_tab[1]);
  
        i1 = UT3D_pt_projptci (&pt1, &pt2, &pt0, &ci1);
        if(i1 < 0) TX_Print(" - in Line %d:",APT_lNr);

        if(imod == 1) pt1 = pt2;
        APT_modMax1 = 2;
        goto Fertig3D;  // pt1


      //-----------------------------------------------------------------
      // P = P S [MOD]                  // Proj PT -> Curve
      } else if(aus_typ[1] == Typ_CV)             {
        ox1p = DB_GetCurv ((long)aus_tab[1]);
        // printf(" CurvTyp=%d\n",ox1p->typ);

        if(ox1p->typ == Typ_CVELL) {
          UT3D_pt_projptel(&i1, pa, ox1p->data, &pt0);
          APT_modMax1 = i1;
          pt1 = pa[imod];
          goto Fertig3D;

        } else if(ox1p->typ == Typ_CVBSP) {
          // UME_init (&memSeg1, memspc101, sizeof(memspc101));  // Init Datasegment
          i1 = 10; // size of pa, da
          UT3D_pt_projptbspl (&i1, pa, da, ox1p->data, &pt0);
          APT_modMax1 = i1;
          // pt1 = pa[imod];
          pt1 = pa[UT3D_ipt_cknearn_npt (&pt0, pa, i1, imod)];
          goto Fertig3D;

        } else if(ox1p->typ == Typ_CVPOL) {
          // proj. Pt -> PolygonCurve
          i1 = 10;  // size of pTab !
          UT3D_pt_projptplg (&i1, pa, da, ox1p->data, &pt0);
          APT_modMax1 = i1;
          pt1 = pa[UT3D_ipt_cknearn_npt (&pt0, pa, i1, imod)];
          goto Fertig3D;

        } else {
          goto L_noFunc;
        }


      //-----------------------------------------------------------------
      // P = P R                  (project pt -> pln)
      } else if(aus_typ[1] == Typ_PLN)             {
        DB_GetRef (&pl1, (long)aus_tab[1]);
        UT3D_pt_projptpl (&pt1, &pl1, &pt0);
        goto Fertig3D;
  
/*
    //-----------------------------------------------------------------
    // P = R P                  (translate pt -> r1)
    } else if((aus_typ[0] == Typ_PLN)         &&
              (aus_typ[1] == Typ_PT))             {
      DB_GetRef (&pl1, (long)aus_tab[0]);
      pt0 = DB_GetPoint ((long)aus_tab[1]);
      UT3D_m3_loadpl (trmat, &pl1);
      UT3D_pt_tra_pt_m3 (&pt1, trmat, &pt0);
      goto Fertig3D;
*/


      //-----------------------------------------------------------------
      // P = P A [MOD]                  // Proj PT -> Surf
      } else if(aus_typ[1] == Typ_SUR)        {
        ox1p = DB_GetSur ((long)aus_tab[1], 0);
        if(ox1p->typ == Typ_Error) goto L_noFunc;
          // DEB_dump_obj__ (Typ_ObjGX, ox1p, "Sur=");
  
        // SUR_ck_typ
        // UTO_obj_dbo UTO_objDat_ox UTO_obj_get UTO_objx_get DB_GetObjGX

        if(ox1p->form == Typ_SURBSP) {
          // i1 = 10;  // size of pTab !
          i1 = USBS_pt_projptsbsp (&i1,pa, da,&da[1],ox1p->data,&pt0,&APTSpcTmp);
          if(i1 < 0) goto Error;
          // APT_modMax1 = i1;
          // pt1 = pa[UT3D_ipt_cknearn_npt (&pt0, pa, i1, imod)];
          pt1 = pa[0];
          goto Fertig3D;

        } else goto L_noFunc;


      //-----------------------------------------------------------------
      // P = P SOL [MOD]
      } else if(aus_typ[1] == Typ_SOL)             {
        ox1p = DB_GetSol ((long)aus_tab[1]);
        // printf(" proj PT -> SOL soltyp=%d\n",oxp->typ);

        if(ox1p->typ == Typ_CON) {
          UT3D_pt_projptbcon (&pt1, ox1p->data, &pt0);

        } else if(ox1p->typ == Typ_TOR) {
          UT3D_pt_projptbtor (&pt1, ox1p->data, &pt0);

        } else goto L_noFunc;

        APT_modMax1 = 1;
        goto Fertig3D;


      //-----------------------------------------------------------------
      // P = P ??
      } else goto L_noFunc;


    //-----------------------------------------------------------------
    // P = L ..
    } else if(aus_typ[0] == Typ_LN)   {
      ln1 = DB_GetLine ((long)aus_tab[0]);


      //-----------------------------------------------------------------
      // P = L VAL                 // point on Line (parameter)
      if(aus_typ[1] == Typ_Val)            {
        UT3D_pt_evparln (&pt1, aus_tab[1], &ln1);
        goto Fertig3D;


      //-----------------------------------------------------------------
      // P = L L [MOD]             // Schnitt LN - LN
      } else if(aus_typ[1] == Typ_LN)            {
        // printf("P = L L   %f %f\n",aus_tab[0],aus_tab[1]);
        ln2 = DB_GetLine ((long)aus_tab[1]);
        i1 = UT3D_pt_int2ln (&pa[0], &pa[1], NULL, &ln1, &ln2);
        if(i1 < 0) {
          TX_Error(" Linien parallel ****\n");
          goto Error;
        }
        pt1 = pa[imod];
        APT_modMax1 = 1;
        goto Fertig3D;


      //-----------------------------------------------------------------
      // P = L C [MOD]             // Schnitt LN - CIR
      } else if(aus_typ[1] == Typ_CI)   {
        ci1 = DB_GetCirc ((long)aus_tab[1]);
        UT3D_pt_intlnci__ (&i1, pa, &ln1, &ci1);
        APT_modMax1 = i1;
        if(i1 == 0) {
          TX_Error(" kein Schnittpunkt ****\n");
          goto Error;
        }
        pt1 = pa[imod];
        goto Fertig3D;



      //-----------------------------------------------------------------
      // P = L S [MOD]              intersect Line X Curve
      } else if(aus_typ[1] == Typ_CV)           {
        ox1p = DB_GetCurv ((long)aus_tab[1]);

        // B_Spline:
        if(ox1p->typ == Typ_CVBSP) {
          // Init Datesegment
          UME_init (&memSeg1, memspc101, sizeof(memspc101));
          ptNr = 16;  // size of pTab !
          UT3D_pt_intlnbspl (&ptNr, pa, &memSeg1, &ln1, ox1p->data);

        // Ellipse:
        } else if(ox1p->typ == Typ_CVELL) {
          UT3D_pt_intlnel__ (&ptNr, pa, &ln1, ox1p->data);

        // Polygon:
        } else if(ox1p->typ == Typ_CVPOL) {
          ptNr = 16;  // size of pTab !
          UT3D_pt_intlnplg (&ptNr, pa, NULL, &ln1, ox1p->data, UT_TOL_cv);

        } else {
          goto L_noFunc;
        }

          // printf(" ptNr=%d\n",ptNr);
        if((ptNr - 1) < imod) goto L_Err_Int;
        APT_modMax1 = ptNr;
        pt1 = pa[imod];
        goto Fertig3D;


      //-----------------------------------------------------------------
      // P = L A [MOD]                     // intersect Line Surface
      } else if(aus_typ[1] == Typ_SUR)            {
        ox1p = DB_GetSur ((long)aus_tab[1], 0);
        if(ox1p->typ == Typ_Error) goto L_noFunc;
          // DEB_dump_obj__ (Typ_ObjGX, ox1p, "Sur=");

        // get typ of surface
        i1 = UTO_ck_surfTyp (ox1p);
          // printf(" surTyp=%d\n",i1);

        // SUR_ck_typ
        // UTO_obj_dbo UTO_objDat_ox UTO_obj_get UTO_objx_get DB_GetObjGX

        if(i1 == Typ_SURBSP) {
           // 10 = size of pTab
          irc = UT3D_pt_intlnsbs (&i1, pa, 10, &ln1, ox1p->data, &APTSpcTmp);
          if(irc < 0) goto Error;
          APT_modMax1 = i1;
          pt1 = pa[UT3D_ipt_cknearn_npt (&pt0, pa, i1, imod)];
          // pt1 = pa[0];
          goto Fertig3D;

        } else if(i1 == Typ_SURRU) {
          // intersect line X ruledSurf
          TSU_tr_init_ (ox1p);
          SRU_init_basPts();
          SRU_parsru_intObj (&d1, &d2, &pt1, Typ_LN, (void*)&ln1);
          goto Fertig3D;

        } else goto L_noFunc;


      //-----------------------------------------------------------------
      // P = L R                        // intersect LN - PLN
      } else if(aus_typ[1] == Typ_PLN)           {
        DB_GetRef (&pl1, (long)aus_tab[1]);
  
        i1 = UT3D_pt_intlnpl (&pt1, &pl1, &ln1);
        if(i1 == 0) {
          TX_Error(" kein Schnittpunkt ****\n");
          goto Error;
        }
        goto Fertig3D;


      //-----------------------------------------------------------------
      // P = L ??
      } else goto L_noFunc;


    //-----------------------------------------------------------------
    // P = C ..
    } else if(aus_typ[0] == Typ_CI)        {
      ci1 = DB_GetCirc ((long)aus_tab[0]);


      //-----------------------------------------------------------------
      // P = C C [MOD]             // Schnitt CIR - CIR
      if(aus_typ[1] == Typ_CI)          {
        ci2 = DB_GetCirc ((long)aus_tab[1]);
  
        i1 = UT3D_pt_intcici (pa, &ci1, 1, &ci2, 1);
        if(i1 == 0) {
          TX_Error(" kein Schnittpunkt ****\n");
          goto Error;
        }
        pt1 = pa[imod];
        APT_modMax1 = i1;
        goto Fertig3D;


      //-----------------------------------------------------------------
      // P = C VAL            // Parameter on Cir
      } else if(aus_typ[1] == Typ_Val)            {

        UT3D_pt_vc__par_ci (&pt1, NULL, &ci1, aus_tab[1]);
        goto Fertig3D;


      //-----------------------------------------------------------------
      // P = C S [MOD]
      } else if(aus_typ[1] == Typ_CV)            {
        ox1p = DB_GetCurv ((long)aus_tab[1]);
        UME_init (&memSeg1, memspc201, sizeof(memspc201));  // Init Datasegment

        if(ox1p->typ == Typ_CVELL) {          // Ell X Circ
          // intersect Circ Elli
          UT3D_pt_intciel (&ptNr, pa, &ci1, ox1p->data, &memSeg1);

        } else if(ox1p->typ == Typ_CVBSP) {   // B-Spl X Circ
          // intersect Circ BSP; bis 16 Pt's !
          ptNr = 16;
          UT3D_pt_intcibspl (&ptNr, pa, &ci1, ox1p->data, &memSeg1);

        // Polygon:
        } else if(ox1p->typ == Typ_CVPOL) {
          ptNr = 16;  // size of pTab !
          UT3D_pt_intciplg (&ptNr, pa, da, &ci1, ox1p->data);

        } else goto L_noFunc;

        pt1 = pa[imod];
        APT_modMax1 = ptNr;
        goto Fertig3D;


      //-----------------------------------------------------------------
      // P = C R [MOD]             // Schnitt CIR - PLN
      } else if(aus_typ[1] == Typ_PLN)           {
        DB_GetRef (&pl1, (long)aus_tab[1]);
  
        i1 = UT3D_pt_intplnci (pa, &pl1, &ci1, 1);
        if(i1 < 1) {
          TX_Error(" kein Schnittpunkt ****\n");
          goto Error;
        }
      
        pt1 = pa[imod];
        APT_modMax1 = i1;
        goto Fertig3D;


      //-----------------------------------------------------------------
      // P = C ??
      } else goto L_noFunc;



    //================================================================
    // P = S ..
    } else if(aus_typ[0] == Typ_CV)         {
      ox1p = DB_GetCurv ((long)aus_tab[0]);
        // printf(" typ=%d form=%d siz=%d\n",ox1p->typ,ox1p->form,ox1p->siz);
        // DEB_dump_obj__ (ox1p->form, ox1p->data, " curv-S-val");


      //-----------------------------------------------------------------
      // P = S VAL                 // point on Curve (parameter)
      if(aus_typ[1] == Typ_Val)             {

        APT_prim_par = aus_tab[1];
        // i1 = UT3D_pt_evparcrv (&pt1, APT_prim_par, ox1p->form, ox1p->data);
        i1 = UT3D_pt_vc__par_cv (&pt1,NULL, ox1p->form,ox1p->data, 1,APT_prim_par);
        if(i1 < 0) return -1;
        goto Fertig3D;


      //-----------------------------------------------------------------
      // P = S R [MOD]             // Intersect Curve - Plane
      } else if(aus_typ[1] == Typ_PLN)           {
        DB_GetRef (&pl1, (long)aus_tab[1]);
        ptNr = 16;

        if(ox1p->typ == Typ_CVBSP) {
          UME_init (&memSeg1, memspc101, sizeof(memspc101));
          UT3D_pt_intplbspl (&ptNr, pa, &pl1, ox1p->data, &memSeg1, UT_TOL_cv);

        } else if(ox1p->typ == Typ_CVPOL) {
          UT3D_pt_intplplg (&ptNr, pa, &pl1, ox1p->data);

        } else if(ox1p->typ == Typ_CVELL) {
          UT3D_pt_intplell (&ptNr, pa, &pl1, ox1p->data);

        } else if(ox1p->typ == Typ_CVTRM) {
          TX_Error(" intersect Plane X CCV-curve not yet implemented ..");
          goto Error;

        } else goto L_noFunc;

        pt1 = pa[imod];
        APT_modMax1 = ptNr;
        goto Fertig3D;


      //-----------------------------------------------------------------
      // P = S MOD [MOD]      OBSOLETE      // S=Polygon, CCV-segNr, polgonPtNr
      } else if((aus_typ[1] == Typ_modif)      &&
                (modFlag    == 1))                   {
         MSG_ERR__ (ERR_obsolete, "change MOD to SEG");

        if(ox1p->form != Typ_CVTRM) goto L_ParErr;

        // get APT_prim_seg = subcurve-Nr of CCV
        APT_prim_seg = aus_tab[1];
        // get APT_prim_sg2 = segment-Nr in curve <APT_prim_seg>
        APT_prim_sg2 = imod;
        if(APT_prim_sg2 < 0) goto L_ParErr;

        // irc = UT3D_obj_segccv (&ox1, APT_prim_seg, ox1p);
        // get typ,obj1 = subcurve of CCV
        i1 = APT_prim_seg - 1;  // segNr -> indexNr; first=0;
        irc = UTO_obj__ccv_segnr (&mtyp, obj1, i1, ox1p);
        if(irc < 0) goto L_noFunc;

        // get 1=startPt 2=endPt
        i1 = APT_prim_sg2;
        if(mtyp == Typ_CVPOL) i1 *= -1;  // only polygon: get cornerpoints
        irc = UT3D_ptvcpar_std_obj (&pt1, NULL, NULL, 0, i1, mtyp, obj1);
        if(irc < 0) goto L_noFunc;
        goto Fertig3D;


      //-----------------------------------------------------------------
      // P = S S [MOD]                              // inters. 2 curves
      } else if(aus_typ[1] == Typ_CV)             {
        ox2p = DB_GetCurv ((long)aus_tab[1]);
// TODO: make INT_cv_cv__   see UTO_npt_int_2ox UTO_stru_int

        // Init Datesegment - ca 100 K !
        UME_init (&memSeg1, memspc101, sizeof(memspc101));
        ptNr = 16;  // size of pTab !

        if(ox1p->typ == Typ_CVBSP) {     // B_Spline:

          if(ox2p->typ == Typ_CVPOL) {          // Polygon   X   B-Spl
            UT3D_pt_intbspplg (&ptNr, pa, da, &memSeg1, ox1p->data, ox2p->data);

          } else if(ox2p->typ == Typ_CVBSP) {   // B-Spl   X   B-Spl
            UT3D_pt_int2bspl (&ptNr, pa, &memSeg1, ox1p->data, ox2p->data);

          } else if(ox2p->typ == Typ_CVELL) {   // B-Spl   X   Elli
            UT3D_pt_intelbspl (&ptNr, pa, ox2p->data, ox1p->data, &memSeg1);

          } else {
            goto L_noFunc;
          }


        } else if(ox1p->typ == Typ_CVELL) {     // Elli:
  
          if(ox2p->typ == Typ_CVBSP) {   // Elli   X   B-Spl
            UT3D_pt_intelbspl (&ptNr, pa, ox1p->data, ox2p->data, &memSeg1);
  
          } else if(ox2p->typ == Typ_CVPOL) {   // Polygon   X   Elli
            UT3D_pt_intelplg (&ptNr, pa, NULL, NULL, ox1p->data, ox2p->data);
  
          } else if(ox2p->typ == Typ_CVELL) {   // Elli   X   Elli
            UT3D_pt_intelel (&ptNr, pa, ox1p->data, ox2p->data, &memSeg1);
  
          } else {
            goto L_noFunc;
          }


        } else if(ox1p->typ == Typ_CVPOL) {     // Polygon:
  
        if(ox2p->typ == Typ_CVBSP) {          // Polygon   X   B-Spl
            UT3D_pt_intbspplg (&ptNr, pa, da, &memSeg1, ox2p->data, ox1p->data);
  
          } else if(ox2p->typ == Typ_CVELL) {   // Polygon   X   Elli
            UT3D_pt_intelplg (&ptNr, pa, NULL, NULL, ox2p->data, ox1p->data);
  
          } else if(ox2p->typ == Typ_CVPOL) {   // Polygon   X   Polygon
            // DEB_dump_ox_0 (ox1p, 1);
            // DEB_dump_ox_0 (ox2p, 1);
            UT3D_pt_int2plg (&ptNr, pa, da, da, ox2p->data, ox1p->data);

          } else {
            goto L_noFunc;
          }



        } else {
          goto L_noFunc;
        }

        pt1 = pa[imod];
        APT_modMax1 = ptNr;
        goto Fertig3D;
        // goto L_modify;


      //-----------------------------------------------------------------
      // P = S ??
      } else goto L_noFunc;



    //-----------------------------------------------------------------
    // P = A VAL              // point inside CCV on surface (uPar)
    } else if((aus_typ[0] == Typ_SUR)         &&
              (aus_typ[1] == Typ_Val))           {

      d1 = aus_tab[1];
      d2 = 0.;
      goto L_isoSur;






    //-----------------------------------------------------------------
    }




  //==================================================================
  } else if(aus_anz == 3)        {
  //==================================================================

    // // sort atomic objects
    // ATO_sort1 (aus_anz, aus_typ, aus_tab);



    //-----------------------------------------------------------------
    // P = VAL VAL VAL  // x-coord y-coord z-coord, eg: P21=P(X(P20) Y(P20) Z(P20))
    if (((aus_typ[0] >= Typ_Val)&&(aus_typ[0] < Typ_Angle))     &&
        ((aus_typ[1] >= Typ_Val)&&(aus_typ[0] < Typ_Angle))     &&
        ((aus_typ[2] >= Typ_Val)&&(aus_typ[0] < Typ_Angle)))         {
      vc1.dx = aus_tab[0];
      vc1.dy = aus_tab[1];
      vc1.dz = aus_tab[2];
      goto EX_VC;   // vc1



    //-----------------------------------------------------------------
    // P = P ..
    } else if(aus_typ[0] == Typ_PT)     {
      pt1 = DB_GetPoint ((long)aus_tab[0]);

      //----------------------------------------------------------------
      // P = P  X|Y|Z  X|Y|Z
      if((aus_typ[1] >= Typ_XVal)&&(aus_typ[1] <= Typ_ZVal)    &&
         (aus_typ[2] >= Typ_XVal)&&(aus_typ[1] <= Typ_ZVal))      {
  
        vc1 = UT3D_VECTOR_NUL;
  
        if(aus_typ[1] == Typ_XVal) vc1.dx = aus_tab[1];
        if(aus_typ[1] == Typ_YVal) vc1.dy = aus_tab[1];
        if(aus_typ[1] == Typ_ZVal) vc1.dz = aus_tab[1];

        if(aus_typ[2] == Typ_XVal) vc1.dx = aus_tab[2];
        if(aus_typ[2] == Typ_YVal) vc1.dy = aus_tab[2];
        if(aus_typ[2] == Typ_ZVal) vc1.dz = aus_tab[2];

        goto EX_PTVC;   // pt1 vc1


      //-----------------------------------------------------------------
      // P = P P ..
      } else if(aus_typ[1] == Typ_PT) {
        pt2 = DB_GetPoint ((long)aus_tab[1]);

        //-----------------------------------------------------------------
        // P = P P VAL            // p1 -> p2 - length
        if(aus_typ[2] == Typ_Val)           {
          UT3D_pt_traptptlen(&pt1, &pt1, &pt2, aus_tab[2]);
          goto Fertig3D;   // pt1

        //-----------------------------------------------------------------
        // P = P P ANGrot                // rotate p2 around p1-vz
        } else if(aus_typ[2] == Typ_Angle)         {
          d1  = UT_RADIANS (aus_tab[2]);
          d2 = 0.;
          goto L_P__PPAA;   // pt1 pt2 vc1 d1

        //-----------------------------------------------------------------
        // P = P P D            (project point -> ray from pt2, vc)
        } else if(aus_typ[2] == Typ_VC)         {
          vc1 = DB_GetVector ((long)aus_tab[2]);
          UT3D_pt_projptptvc (&pt1, &d1, NULL, &pt1, &pt2, &vc1);
          goto Fertig3D;


        //-----------------------------------------------------------------
        } else goto L_noFunc;


      //-----------------------------------------------------------------
      // P = P D ..
      } else if(aus_typ[1] == Typ_VC)         {
        vc1 = DB_GetVector ((long)aus_tab[1]);

        //-----------------------------------------------------------------
        // P = P D VAL             // from P into direction D length=VAL
        if(aus_typ[2] == Typ_Val)          {
          goto L_P__P_D;  // pt1, vc1

        //-----------------------------------------------------------------
        // P = P D ANG             // length missing
        } else if(aus_typ[2] == Typ_Angle)           {
          goto L_P__nyc;        // -3  object not yet complete

        //-----------------------------------------------------------------
        // P = P D Plane             project point along vector -> plane
        } else if(aus_typ[2] == Typ_PLN)           {
          DB_GetRef (&pl1, (long)aus_tab[2]);
          UT3D_ptDi_intptvcpln (&pt1, &d1, &pl1, &pt1, &vc1);
          goto Fertig3D;  // pt1

        //-----------------------------------------------------------------
        } else goto L_noFunc;


      //-----------------------------------------------------------------
      // P = P L ..
      } else if(aus_typ[1] == Typ_LN)         {
        ln1 = DB_GetLine ((long)aus_tab[1]);

        //-----------------------------------------------------------------
        // P = P L ANG             // rotate P around L
        if(aus_typ[2] == Typ_Angle)          {
          d1  = UT_RADIANS (aus_tab[2]);
          UT3D_vc_ln (&vc1, &ln1);
          UT3D_vc_normalize (&vc1, &vc1);
          UT3D_pt_rotptptvcangr (&pt1, &pt1, &ln1.p1, &vc1, d1);
          goto EX_PT;   // pt1

        //-----------------------------------------------------------------
        } else goto L_noFunc;


      //-----------------------------------------------------------------
      // P = P ANG VAL             // Polar - basepoint, direction, distance
      } else if((aus_typ[1] == Typ_Angle)      &&
                (aus_typ[2] == Typ_Val))           {
        d1  = UT_RADIANS (aus_tab[1]);
        d2  = aus_tab[2];

        // pt2 = pt0;
        // pt2.x += d2;
        // UT3D_vc_2angr (&vc1, d1, 0.);
        // UT3D_pt_traptvclen (&pt2, &pt0, &vc1, d2);
        // UT3D_pt_rotptptvcangr (&pt1, &pt2,   &pt0, &WC_sur_act.vz, d1);

        UT3D_vc_angr (&vc1, d1);
        UT3D_vc_setLength (&vc1, &vc1, d2);
        goto EX_PTVC;     // pt1 vc1


      //-----------------------------------------------------------------
      } else goto L_noFunc;


    //-----------------------------------------------------------------
    } else if (aus_typ[0] == Typ_CV) {
    // P = S ..


      //----------------------------------------------------------------
      // P = S SEG VAL             // point on curve inside CCV
      // P = S MOD VAL                 // OBSOLETE
      if(((aus_typ[1] == Typ_SEG)||(aus_typ[1] == Typ_modif))      &&
          (aus_typ[2] == Typ_Val))           {
       if(aus_typ[1] == Typ_modif)
         MSG_ERR__ (ERR_obsolete, "change MOD to SEG");

        ox1p = DB_GetCurv ((long)aus_tab[0]);
        imod = aus_tab[1] - 1;  // segmentNr -> segmentIndex.
        d1   = aus_tab[2];

        APT_prim_seg = imod;
        APT_prim_par = d1;

        if(ox1p->typ == Typ_CVTRM) {
          // UT3D_pt_segparccv (&pt1, imod, d1, ox1p);
          UT3D_pt_ccv_segnr_par1 (&pt1, imod, d1, ox1p);
          goto Fertig3D;
  

        } else if(ox1p->typ == Typ_CVLNA) {
          i1 = UT3D_pt_seg_par_nln (&pt1, imod, d1, ox1p);
          if(i1 < 0) goto L_ParErr;
          goto Fertig3D;

        } else goto L_noFunc;


      //----------------------------------------------------------------
      // P = S SEG PTI     // point inside CCV on polygon
      } else if((aus_typ[1] == Typ_SEG)      &&
                (aus_typ[2] == Typ_PTI))           {

        i1 = aus_tab[1];    // segmentNr; first=1
        i2 = aus_tab[2];    // pointNr; first=1

        // get seg <i1> out of curve
        irc = UTO_objDat_dbS (&vp1, &mtyp, (long)aus_tab[0], i1, 0, obj1);
        if(irc < 0) goto L_ParErr;
          // DEB_dump_obj__(mtyp, vp1, "  _decode_pt-S_SEG-PTI");

        // get point with index <i2> from geom-obj (mtyp,vp1);
        irc = UT3D_ptvcpar_ipt_obj (&pt1, NULL, NULL, i2, mtyp, vp1);
        if(irc < 0) goto L_noFunc;
        goto EX_PT;  // pt1


      //----------------------------------------------------------------
      // P = S SEG PTS     // start(end)Point inside CCV on polygon
      } else if((aus_typ[1] == Typ_SEG)      &&
                (aus_typ[2] == Typ_PTS))           {

        i1 = aus_tab[1];    // segmentNr; first=1
        i2 = aus_tab[2];    // pointTyp; eg Ptyp_start

        // get seg <i1> out of curve
        irc = UTO_objDat_dbS (&vp1, &mtyp, (long)aus_tab[0], i1, 0, obj1);
        if(irc < 0) goto L_ParErr;
          // DEB_dump_obj__(mtyp, vp1, "  _decode_pt-S_SEG-PTI");

        // get pointTyp <i2> from geom-obj (mtyp,vp1);
        irc = UT3D_ptvcpar_std_obj (&pt1, NULL, NULL, 0, i2, mtyp, vp1);
        if(irc < 0) goto L_noFunc;
        goto EX_PT;  // pt1


      //----------------------------------------------------------------
      } else goto L_noFunc;



    //-----------------------------------------------------------------
    // P = A VAL VAL           // point inside sur (uPar vPar)
    } else if((aus_typ[0] == Typ_SUR)         &&
              (aus_typ[1] == Typ_Val)      &&
              (aus_typ[2] == Typ_Val))           {

      d1 = aus_tab[1];
      d2 = aus_tab[2];
      goto L_isoSur;





    //-----------------------------------------------------------------
    }





  //==================================================================
  } else if  (aus_anz == 4)           {
  //==================================================================


    if(aus_typ[0] == Typ_PT) {
        pt1 = DB_GetPoint ((long)aus_tab[0]);

      //-----------------------------------------------------------------
      // P = P VAL VAL VAL               // point + vector
      if       ((TYP_IS_DIST(aus_typ[1]))    &&
                (TYP_IS_DIST(aus_typ[2]))    &&
                TYP_IS_DIST((aus_typ[3])))      {
  
        vc1.dx = aus_tab[1];
        vc1.dy = aus_tab[2];
        vc1.dz = aus_tab[3];
        goto EX_PTVC;



      //-----------------------------------------------------------------
      // P = P D ANG VAL           // point + (vetor+length); rotate around p1
      } else if((aus_typ[1] == Typ_VC)         &&
                (aus_typ[2] == Typ_Angle)      &&
                (aus_typ[3] == Typ_Val))            {
        vc1 = DB_GetVector ((long)aus_tab[1]);
        UT3D_pt_traptvclen (&pt2, &pt1, &vc1, aus_tab[3]);
        d1  = UT_RADIANS (aus_tab[2]);
        d2  = 0.;
        goto L_P__PPAA;   // pt1 pt2 d1 d2


      //-----------------------------------------------------------------
      // P = P D ANG ANG           // not yet complete ..
      } else if((aus_typ[1] == Typ_VC)         &&
                (aus_typ[2] == Typ_Angle)      &&
                (aus_typ[3] == Typ_Angle))            {
        goto L_P__nyc;


      //-----------------------------------------------------------------
      // P = P ANG ANG VAL           // Polar (basePoint angRot angTilt distance)
      } else if((aus_typ[1] == Typ_Angle)      &&
                (aus_typ[2] == Typ_Angle)      &&
                (aus_typ[3] == Typ_Val))            {


        d1  = UT_RADIANS (aus_tab[1]);
        d2  = UT_RADIANS (aus_tab[2]);
        d3  = aus_tab[3];
          // printf(" d1,2,3=%lf %lf %lf\n",d1,d2,d3);


        UT3D_vc_2angr (&vc1, d1, d2);
          // DEB_dump_obj__ (Typ_VC, &vc1, " vc1:");
        UT3D_vc_setLength (&vc1, &vc1, d3);

        goto EX_PTVC;  // pto = pt1 + vc1



      //-----------------------------------------------------------------
      } else if(aus_typ[1] == Typ_PT)         {
        pt2 = DB_GetPoint ((long)aus_tab[1]);

        //----------------------------------------------------------------
        // P = P P ..
        //----------------------------------------------------------------
        // P = P P VAL ANG   // P-P = direction; VAL=length, rotate around p1
        if     ((aus_typ[2] == Typ_Val)        &&
                (aus_typ[3] == Typ_Angle))          {


        // // von P1 in Richtung P1 -> P2  abschlagen
        // pt21 = UT2D_pt_pt3 (&pt0);
        // pt22 = UT2D_pt_pt3 (&pt1);
        // UT2D_pt_tra2ptlen (&pt20, &pt21, &pt22, aus_tab[2]);
        // // P2 rund um P1 drehen
        // UT2D_pt_rotptangr (&pt20, &pt21, &pt20, UT_RADIANS(aus_tab[3]));

        UT3D_vc_2ptlen (&vc1, &pt1, &pt2, aus_tab[2]);
        if(fabs(aus_tab[3]) > UT_TOL_min0)
          UT3D_vc_rotangr (&vc1, &vc1, &aus_tab[3]);

        goto EX_PTVC;


        //----------------------------------------------------------------
        // P = P P ANGrot ANGtilt  // rotate p2 around p1-vz; tilt in plane p1-p2-vz
        } else if((aus_typ[2] == Typ_Angle)      &&
                  (aus_typ[3] == Typ_Angle))            {

          d1  = UT_RADIANS (aus_tab[2]);
          d2  = UT_RADIANS (aus_tab[3]);
          goto L_P__PPAA;     


        //----------------------------------------------------------------
        // P = P P ANG D       // rotate p2 around p1-D
        // replace with "rotate around line .."
        } else if((aus_typ[2] == Typ_Angle)      &&
                  (aus_typ[3] == Typ_VC))            {

          d1  = UT_RADIANS (aus_tab[2]);
          vc1 = DB_GetVector ((long)aus_tab[3]);
          UT3D_vc_normalize (&vc1, &vc1);
          d2 = 0.;
          goto L_P__PPAA;


        //----------------------------------------------------------------
        } else goto L_noFunc;




      //-----------------------------------------------------------------
      // P = P L VAL ANG     // from P into direction of L, VAL=distance;
                             // rotate around P/Z-axis
      } else if((aus_typ[1] == Typ_LN)         &&
                (aus_typ[2] == Typ_Val)        &&
                (aus_typ[3] == Typ_Angle))          {

        pt1 = DB_GetPoint ((long)aus_tab[0]);
        ln1 = DB_GetLine ((long)aus_tab[1]);

        UT3D_vc_2ptlen (&vc1, &ln1.p1, &ln1.p2, aus_tab[2]);
        if(fabs(aus_tab[3]) > UT_TOL_min0)
          UT3D_vc_rotangr (&vc1, &vc1, &aus_tab[3]);

        goto EX_PTVC;  // pt1 vc1


      //-----------------------------------------------------------------
      // P = P D VAL VAL    // from P into direction of D, length = VAL1;
                            // normal to D/Z-axis, length = VAL2;
      } else if((aus_typ[1] == Typ_VC)         &&
                (aus_typ[2] == Typ_Val)        &&
                (aus_typ[3] == Typ_Val))           {
        vc1 = DB_GetVector ((long)aus_tab[1]);

        L_P__P_D:  // pt1, vc1,

          if(aus_anz < 3) {
            UT3D_pt_traptvc (&pt1, &pt1, &vc1);
          } else {
            UT3D_pt_traptvclen (&pt1, &pt1, &vc1, aus_tab[2]);
            if(aus_anz > 3) {
              // einen Normalvektor auf vc1 und DZ
              UT3D_vc_perp2vc (&vc2, (Vector*)&WC_sur_act.vz, &vc1);
              // DEB_dump_obj__(Typ_VC, &vc2, "vc2=");
              UT3D_pt_traptvclen (&pt1, &pt1, &vc2, aus_tab[3]);
            }
          }
          goto Fertig3D;


      //-----------------------------------------------------------------
      } else goto L_noFunc;


    //-----------------------------------------------------------------
    } else goto L_noFunc;



  //==================================================================
  } else if(aus_anz == 5)        {
  //==================================================================

    if(aus_typ[0] == Typ_PT) {
        pt1 = DB_GetPoint ((long)aus_tab[0]);

      //-----------------------------------------------------------------
      // P = P P ..
      if(aus_typ[1] == Typ_PT) {
        pt2 = DB_GetPoint ((long)aus_tab[1]);

     
        //----------------------------------------------------------------
        // P = P P ANG ANG VAL          // point + vector (p1-p2), length=VAL;
                               // rotate around p1-vz; tilt in plane p1-p2-vz
        if((aus_typ[2] == Typ_Angle)      &&
           (aus_typ[3] == Typ_Angle)      &&
            TYP_IS_DIST((aus_typ[4])))          {

          // pt2 = dist from pt1
          UT3D_pt_traptptlen(&pt2, &pt1, &pt2, aus_tab[4]);

          d1  = UT_RADIANS (aus_tab[2]);
          d2  = UT_RADIANS (aus_tab[3]);


          L_P__PPAA:   // pt1 pt2 d1 d2
            UT3D_pt_rot2ptpl2angr (&pt0, &pt1, &pt2, &WC_sur_act, &d1, &d2);
            pt1 = pt0;
            goto Fertig3D;


        //-----------------------------------------------------------------
        } else goto L_noFunc;



      //-----------------------------------------------------------------
      // P = P D ..
      } else if(aus_typ[1] == Typ_VC) {       
        vc1 = DB_GetVector ((long)aus_tab[1]);


        //----------------------------------------------------------------
        // P = P D ANG ANG VAL          // point + vector, length=VAL;
                               // rotate around p1-vz; tilt in plane p1-p2-vz
        if((aus_typ[2] == Typ_Angle)      &&
           (aus_typ[3] == Typ_Angle)      &&
            TYP_IS_DIST((aus_typ[4])))          {
          // pt2 = dir+dist from pt1
          UT3D_pt_traptvclen (&pt2, &pt1, &vc1, aus_tab[4]);
          d1  = UT_RADIANS (aus_tab[2]);
          d2  = UT_RADIANS (aus_tab[3]);
          goto L_P__PPAA;   // pt1 pt2 d1 d2


        //-----------------------------------------------------------------
        } else goto L_noFunc;



      //-----------------------------------------------------------------
      } else goto L_noFunc;


    //-----------------------------------------------------------------
    } else goto L_noFunc;


  } // end loop 5 objs


  //================================================================
  printf(" different nr of inputObjs ... %d\n",aus_anz);
  goto L_noFunc;
  //================================================================







  ///////////////////////////////////////////////////////////////
  // ALTE VERSION:


  // i1 = APT_decode_inp (aus_anz, aus_typ, aus_tab);
  // if(i1 < 0) goto Error;




/*
  //-----------------------------------------------------------------
  // P=[X(dx) Y(DY) Z(DZ)]
  if      (((IN_anz       ==   1)        &&
            (IN_obj[0].typ>=Typ_XVal)    &&
            (IN_obj[0].typ<=Typ_ZVal))        ||

           ((IN_anz       ==   2)        &&
            (IN_obj[0].typ>=Typ_XVal)    &&
            (IN_obj[0].typ<=Typ_ZVal)    &&
            (IN_obj[1].typ>=Typ_XVal)    &&
            (IN_obj[1].typ<=Typ_ZVal))        ||

           ((IN_anz       ==   3)        &&
            (IN_obj[0].typ==Typ_ZVal)    &&
            (IN_obj[1].typ==Typ_XVal)    &&
            (IN_obj[2].typ==Typ_ZVal)))          {


    pt1 = UT3D_PT_NUL;

    if(IN_obj[0].typ==Typ_XVal) pt1.x = IN_obj[0].rad;
    if(IN_obj[0].typ==Typ_YVal) pt1.y = IN_obj[0].rad;
    if(IN_obj[0].typ==Typ_ZVal) pt1.z = IN_obj[0].rad;
    if(IN_anz == 1) goto Fertig3D;

    if(IN_obj[1].typ==Typ_YVal) pt1.y = IN_obj[1].rad;
    if(IN_obj[1].typ==Typ_ZVal) pt1.z = IN_obj[1].rad;
    if(IN_anz == 2) goto Fertig3D;

    pt1.z = IN_obj[2].rad;
    goto Fertig3D;






  //-----------------------------------------------------------------
  // P = P
  } else if((IN_anz       ==   1)        &&
            (IN_obj[0].typ==Typ_PT)) {

    pt1 = IN_obj[0].p1;
    goto Fertig3D;









  //-----------------------------------------------------------------
  // p1=P1 R1                  (project p1 -> r1)
  } else
*/

/*
 if((IN_anz       ==   2)        &&
            (IN_obj[0].typ==Typ_PT)      &&
            (IN_obj[1].typ==Typ_PLN))    {

    DB_GetRef(&pl1, trmat, (long)IN_obj[1].ID);
    UT3D_pt_projptpl (&pt1, &pl1, &IN_obj[0].p1);
    goto Fertig3D;





  //-----------------------------------------------------------------
  // p1=P1 DX/DY/DZ            (relativ offset)
  } else if((IN_anz       ==   2)        &&
            (IN_obj[0].typ==Typ_PT)      &&
            ((IN_obj[1].typ>=Typ_XVal)&&(IN_obj[1].typ<=Typ_ZVal)))  {

    pt1 = IN_obj[0].p1;

    if(IN_obj[1].typ==Typ_XVal) pt1.x += IN_obj[1].rad;
    if(IN_obj[1].typ==Typ_YVal) pt1.y += IN_obj[1].rad;
    if(IN_obj[1].typ==Typ_ZVal) pt1.z += IN_obj[1].rad;

    goto Fertig3D;




  // p1=P1 DX/DY/DZ            (relativ offset)
  } else if((IN_anz       ==   3)        &&
            (IN_obj[0].typ==Typ_PT)      &&
            ((IN_obj[1].typ>=Typ_XVal)&&(IN_obj[1].typ<=Typ_YVal))   &&
            ((IN_obj[2].typ>=Typ_YVal)&&(IN_obj[2].typ<=Typ_ZVal)))  {

    pt1 = IN_obj[0].p1;

    if(IN_obj[1].typ==Typ_XVal) pt1.x += IN_obj[1].rad;
    if(IN_obj[1].typ==Typ_YVal) pt1.y += IN_obj[1].rad;

    if(IN_obj[2].typ==Typ_YVal) pt1.y += IN_obj[1].rad;
    if(IN_obj[2].typ==Typ_ZVal) pt1.z += IN_obj[1].rad;

    goto Fertig3D;




  // p1=P1 DX/DY/DZ            (relativ offset)
  } else if((IN_anz       ==   4)        &&
            (IN_obj[0].typ==Typ_PT)      &&
            ((IN_obj[1].typ==Typ_XVal)  ||
             (IN_obj[2].typ==Typ_YVal)  ||
             (IN_obj[3].typ==Typ_ZVal)))    {

    pt1 = IN_obj[0].p1;

    pt1.x += IN_obj[1].rad;
    pt1.y += IN_obj[2].rad;
    pt1.z += IN_obj[3].rad;

    goto Fertig3D;









  //-----------------------------------------------------------------
  // p1=R1 P1                  (translate p1 -> r1)
  } else if((IN_anz       ==   2)        &&
            (IN_obj[0].typ==Typ_PLN)     &&
            (IN_obj[1].typ==Typ_PT))    {

    DB_GetRef(&pl1, trmat, (long)IN_obj[0].ID);
    UT3D_pt_tra_pt_m3 (&pt1, trmat, &IN_obj[1].p1);
    goto Fertig3D;

*/



/*
  //-----------------------------------------------------------------
  // p1=a(45),50    (Polar-Absolut)
  } else if((IN_anz       ==   2)        &&
            (IN_obj[0].typ==Typ_Angle)   &&
            (IN_obj[1].typ==Typ_Val)) {

    // Angle+len > Vector2
    UT2D_vc_angrlen (&vc20, UT_RADIANS (IN_obj[0].rad), IN_obj[1].rad);
    pt1.x = vc20.dx;
    pt1.y = vc20.dy;
    pt1.z = 0.;
    goto Fertig3D;




  //-----------------------------------------------------------------
  // p1=p1,c1             (pt -> Ci proj.)
  } else if((IN_anz       ==   2)       &&
            (IN_obj[0].typ==Typ_PT)     &&
            (IN_obj[1].typ==Typ_CI)) {

    // proj. pt -> cir-vz
    UT3D_pt_projptptvc(&pt0,&d1,NULL, &IN_obj[0].p1,&IN_obj[1].pc,&IN_obj[1].vz);


    UT3D_vc_2pt (&vc1, &pt0, &IN_obj[0].p1);
    d1 = IN_obj[1].rad;
    UT3D_pt_traptvclen(&pt1, &IN_obj[1].pc, &vc1, d1);
    UT3D_pt_traptvclen(&pt2, &IN_obj[1].pc, &vc1, -d1);

    ptArr[0] = UT2D_pt_pt3 (&pt1);
    ptArr[1] = UT2D_pt_pt3 (&pt2);
    i1 = APT_solv_mod (ptArr, 2);
    if(i1 > 0) pt1 = pt2;
    goto Fertig3D;




  //-----------------------------------------------------------------
  // p2=p1,l1        (proj pt -> ln)
  } else if((IN_anz       ==   2)       &&
            (IN_obj[0].typ==Typ_PT)     &&
            (IN_obj[1].typ==Typ_LN)) {


    // ln20 = UT2D_ln_obj2 (&IN_obj[1]);
    // UT2D_pt_projptln (&pt20,  &IN_obj[0].p1, &ln20);

    ln1.p1 = IN_obj[1].p1;
    ln1.p2 = IN_obj[1].p2;
    UT3D_pt_projptln (&pt1, &d1, &IN_obj[0].p1, &ln1);
    goto Fertig3D;



  //-----------------------------------------------------------------
  // p1=l1,l2        (Schnittpt / kuerzester Abst)
  } else if((IN_anz       ==   2)        &&
            (IN_obj[0].typ==Typ_LN)     &&
            (IN_obj[1].typ==Typ_LN)) {

    //printf(" PT = LN , LN\n");
    // ln20 = UT2D_ln_obj2 (&IN_obj[0]);
    // ln21 = UT2D_ln_obj2 (&IN_obj[1]);
    // // Schnittpunkt
    // i1 = UT2D_pt_int2ln (&pt20, &ln20, &ln21);

    ln1.p1 = IN_obj[0].p1;
    ln1.p2 = IN_obj[0].p2;
    ln2.p1 = IN_obj[1].p1;
    ln2.p2 = IN_obj[1].p2;
    i1 = UT3D_pt_int2ln(&pt1, &pt2, &d1, &ln1, &ln2);
    if(i1 < 0) {
      TX_Error(" Linien parallel ****\n");
      goto Error;
    }
    if(d1 > UT_TOL_pt) {
      ptArr[0] = UT2D_pt_pt3 (&pt1);
      ptArr[1] = UT2D_pt_pt3 (&pt2);
      i1 = APT_solv_mod (ptArr, 2);
      if(i1 > 0) pt1 = pt2;
    }
    goto Fertig3D;
*/




/*
  //-----------------------------------------------------------------
  // p1=L1,C1,HI           (intersect ln - ci)
  } else if((IN_anz       ==   2)        &&
            (IN_obj[0].typ==Typ_LN)     &&
            (IN_obj[1].typ==Typ_CI)) {

    //printf(" PT = LN , AC\n");

    // Line > vc20
    pt20 = UT2D_pt_pt3 (&IN_obj[0].p1);
    pt21 = UT2D_pt_pt3 (&IN_obj[0].p2);
    UT2D_vc_2pt (&vc20, &pt20, &pt21);


    pt22 = UT2D_pt_pt3 (&IN_obj[1].pc);
    i1=UT2D_2pt_intciptvc(&ptArr[0], &ptArr[1],
            &pt22,  fabs(IN_obj[1].rad),
            &pt20,  &vc20);

    // nun die Lösung entspr. ModTab suchen
    if(i1 == 0) {
      i1 = APT_solv_mod (ptArr, 2);

    // nur Berührung
    } else if (i1 == 1) {
      i1 = 0;

    } else {
      TX_Error(" Kein Schnittpunkt ****");
      goto Error;
    }

    pt20 = ptArr[i1];
    goto Fertig;
*/




/*
  //-----------------------------------------------------------------
  // p1=C1,C2,HI           (intersect ci - ci)
  } else if((IN_anz       ==   2)        &&
            (IN_obj[0].typ==Typ_CI)     &&
            (IN_obj[1].typ==Typ_CI)) {


    pt20 = UT2D_pt_pt3 (&IN_obj[0].pc);
    pt21 = UT2D_pt_pt3 (&IN_obj[1].pc);

    i1=UT2D_2pt_intcici (&ptArr[0], &ptArr[1],
                       &pt20,  fabs(IN_obj[0].rad),
                       &pt21,  fabs(IN_obj[1].rad));


    // nun die Lösung entspr. ModTab suchen
    if(i1 == 0) {
      i1 = APT_solv_mod (ptArr, 2);

    // nur Berührung
    } else if (i1 == 1) {
      i1 = 0;

    } else {
      TX_Error(" Kein Schnittpunkt ****");
      goto Error;
    }

    pt20 = ptArr[i1];
    goto Fertig;
*/







/*
  //-----------------------------------------------------------------
  // p1=p1,a(45),50               (Polar - relativ)
  } else if((IN_anz       ==   3)        &&
            (IN_obj[0].typ==Typ_PT)      &&
            (IN_obj[1].typ==Typ_Angle)   &&
            (IN_obj[2].typ==Typ_Val)) {

    // Angle+len > Vector2
    UT2D_vc_angrlen (&vc20, UT_RADIANS (IN_obj[1].rad), IN_obj[2].rad);

    pt1.x = IN_obj[0].p1.x + vc20.dx;
    pt1.y = IN_obj[0].p1.y + vc20.dy;
    pt1.z = IN_obj[0].p1.z;

    goto Fertig3D;
*/



/*
  //-----------------------------------------------------------------
  // p1=p1,p2,50          (von p1 -> p2 - abst)
         if((IN_anz       ==   3)       &&
            (IN_obj[0].typ==Typ_PT)     &&
            (IN_obj[1].typ==Typ_PT)     &&
            (IN_obj[2].typ==Typ_Val)) {

    // von P1 in Richtung P1 -> P2  abschlagen
    // UT2D_pt_tra2ptlen (&pt20, &IN_obj[0].p1, &IN_obj[1].p1, IN_obj[2].rad);
    // goto Fertig;

    UT3D_pt_traptptlen(&pt1,&IN_obj[0].p1,&IN_obj[1].p1,IN_obj[2].rad);
    goto Fertig3D;





  //-----------------------------------------------------------------
  // p1=p1,d1,50               (von p1 in Richtg d1 mit Abst)
  } else if((IN_anz       ==   3)        &&
            (IN_obj[0].typ==Typ_PT)      &&
            (IN_obj[1].typ==Typ_VC)      &&
            (IN_obj[2].typ==Typ_Val)) {


    UT3D_pt_traptvclen (&pt1, &IN_obj[0].p1, &IN_obj[1].vz, IN_obj[2].rad);

    goto Fertig3D;  




  
  //-----------------------------------------------------------------
  // p1=p1,p2,a(45)         (P2 rund um P1 drehen)
  } else if((IN_anz       ==   3)       &&
            (IN_obj[0].typ==Typ_PT)     &&
            (IN_obj[1].typ==Typ_PT)     &&
            (IN_obj[2].typ==Typ_Angle)) {

    // P2 rund um P1 drehen
    pt21 = UT2D_pt_pt3 (&IN_obj[0].p1);
    pt22 = UT2D_pt_pt3 (&IN_obj[1].p1);

    UT2D_pt_rotptangr (&pt20, &pt21, &pt22,
                       UT_RADIANS(IN_obj[2].rad));

    goto Fertig;





  //-----------------------------------------------------------------
  // p1=p1,l1,10       (von p1 in Richtg l1 Abst)
  } else if((IN_anz       ==   3)       &&
            (IN_obj[0].typ==Typ_PT)     &&
            (IN_obj[1].typ==Typ_LN)     &&
            (IN_obj[2].typ==Typ_Val)) {


    // Line > vc
    UT3D_vc_2pt (&vc1, &IN_obj[1].p1, &IN_obj[1].p2);

    // von P1 weg in Richtung vc20 abschlagen
    UT3D_pt_traptvclen (&pt1, &IN_obj[0].p1, &vc1, IN_obj[2].rad);
    
    goto Fertig3D;




  //-----------------------------------------------------------------
  // p1=p1,l1,dx,dy      (von pi1 in Richtg l1 mit dx, dy)
  } else if((IN_anz       ==   3)       &&
            (IN_obj[0].typ==Typ_PT)     &&
            (IN_obj[1].typ==Typ_LN)     &&
            (IN_obj[2].typ==Typ_PT)) {


    // Line > vc20
    pt21 = UT2D_pt_pt3 (&IN_obj[1].p1);
    pt22 = UT2D_pt_pt3 (&IN_obj[1].p2);
    UT2D_vc_2pt (&vc20, &pt21, &pt22);

    // von P1 weg in Richtung vc20 abschlagen
    pt21 = UT2D_pt_pt3 (&IN_obj[0].p1);
    UT2D_pt_traptvc2len(&pt20,&pt21,&vc20,IN_obj[2].p1.x,IN_obj[2].p1.y);
    
    goto Fertig;




  //-----------------------------------------------------------------
  // p1=p1,p2,50,A(45)    (von p1 weg in richt p2 mit Abst, dann noch drehen)
  } else if((IN_anz       ==   4)        &&
            (IN_obj[0].typ==Typ_PT)      &&
            (IN_obj[1].typ==Typ_PT)      &&
            (IN_obj[2].typ==Typ_Val)     &&
            (IN_obj[3].typ==Typ_Angle)) {

    // von P1 in Richtung P1 -> P2  abschlagen
    pt21 = UT2D_pt_pt3 (&IN_obj[0].p1);
    pt22 = UT2D_pt_pt3 (&IN_obj[1].p2);
    UT2D_pt_tra2ptlen (&pt20, &pt21, &pt22, IN_obj[2].rad);
    
    // P2 rund um P1 drehen
    UT2D_pt_rotptangr (&pt20, &pt21, &pt20, UT_RADIANS(IN_obj[3].rad));


    goto Fertig;




  //-----------------------------------------------------------------
  // p1=p1,l1,50,A(45)
  } else if((IN_anz       ==   4)        &&
            (IN_obj[0].typ==Typ_PT)      &&
            (IN_obj[1].typ==Typ_LN)      &&
            (IN_obj[2].typ==Typ_Val)     &&
            (IN_obj[3].typ==Typ_Angle)) {

    // von P1 in Richtung P1 -> P2  abschlagen
    pt21 = UT2D_pt_pt3 (&IN_obj[0].p1);
    pt22 = UT2D_pt_pt3 (&IN_obj[1].p1);
    pt23 = UT2D_pt_pt3 (&IN_obj[1].p2);
    UT2D_pt_tra3ptlen (&pt20, &pt21, &pt22, &pt23, IN_obj[2].rad);

    // P2 rund um P1 drehen
    UT2D_pt_rotptangr (&pt20, &pt21, &pt20, UT_RADIANS(IN_obj[3].rad));


    goto Fertig;




  //-----------------------------------------------------------------
  // p1=A1,Seite
  } else if((IN_anz       ==   1)        &&
            (IN_obj[0].typ==Typ_CVPOL2) ) {

    //TX_Print(" PT = CV-Anf/End");

    ptArr[0] = IN_obj[0].p1;
    ptArr[1] = IN_obj[0].p2;

    i1 = APT_solv_mod (ptArr, 2);

    pt20 = ptArr[i1];

    goto Fertig;





  //-----------------------------------------------------------------
  // p1=p1,A1
  } else if((IN_anz       ==   2)        &&
            (IN_obj[0].typ==Typ_PT2)     &&
            (IN_obj[1].typ==Typ_CVPOL2) ) {



    //TX_Print(" PT > CV");

    UTCV2D_pt_projptcv (&pt20,&vc20,&i1,&i2,&IN_obj[0].p1,(int)IN_obj[1].rad);


    goto Fertig;
*/




  //----------------------------------------------------------------
  L_isoSur:   // d1,d2=parameters; 
      // save parameters; at the moment only one point !
      APT_ptPars.du = d1;
      APT_ptPars.dv = d2;


      ox1p = DB_GetSur ((long)aus_tab[0], 0);
      if(ox1p->typ == Typ_Error) goto L_noFunc;
        // DEB_dump_obj__ (Typ_ObjGX, ox1p, "Sur=");
      // get typ of surface 
      // i1 = UTO_ck_surfTyp (ox1p);
      irc = SUR_ck_typ (&i1, &ox2p, &i2, ox1p);
        // printf(" surTyp=%d\n",i1);
     

      if(i1 == Typ_SURBSP) {          // create point on B-SplineSurface
        irc = UT3D_pt_evparsbsp (&pt1, ox2p->data, d1, d2, &APTSpcTmp);
        if(irc < 0) goto Error;
        goto Fertig3D;

      } else if(i1 == Typ_SURRU) {    // create point on RuledSurface
        TSU_tr_init_ (ox2p);
        irc = UT3D_pt_evparsru (&pt1, d1, d2);
        if(irc < 0) goto Error;
        goto Fertig3D;

      } else if(i1 == Typ_SURRV) {    // create point on RevolvedSurface
        TSU_tr_init_ (ox2p);
        irc = SRV_pt_evparsrv (&pt1, d1, d2);
        if(irc < 0) goto Error;
        goto Fertig3D;

      } else {
        goto L_noFunc;
      }






/*
  //-----------------------------------------------------------------
  } else {

    TX_Error(" Punktdefinition nicht implementiert\n");
    goto Error;
  }

*/


    TX_Error(" Punktdefinition nicht implementiert\n");
    goto Error;

    


  Fertig:
  //TX_Print("exit APT_decode_pt %f %f\n",pt20.x,pt20.y);
  *pt_out = UT3D_pt_pt2(&pt20);
  goto Exit;



/*
  L_modify:
    i1 = aus_anz - 1;
    if(aus_typ[i1] == Typ_modif) {
      modTab[0] = aus_tab[i1];
      i2 = APT_solv3D_mod (pa, ptNr, modTab, 1);
    } else {
      i2 = 0;
    }
    pt1 = pa[i2];
*/



  //================================================================
  Fertig3D:    // in pt1
  *pt_out = pt1;
  goto Exit;




  //================================================================
  // pt1 allein ist der Ausgabepunkt.
  EX_PT:
  // transport into ActiveConstrPlane
  if(AP_IS_2D) {
    UT3D_pt_tra_pt_m3 (pt_out, WC_sur_mat, &pt1);
  } else {
    *pt_out = pt1;
  }
  goto Exit;



  //================================================================
  // der Vector vc1 allein ist der Ausgabepunkt.
  EX_VC:
    // DEB_dump_obj__ (Typ_VC, &vc1, "EX_VC-vc1:");
  // transport into ActiveConstrPlane
  if(AP_IS_2D) {
    UT3D_pt_vc (&pt1, &vc1);
    UT3D_pt_tra_pt_m3 (pt_out, WC_sur_mat, &pt1);
  } else {
    UT3D_pt_vc (pt_out, &vc1);
  }
  goto Exit;



  //================================================================
  // pt1+vc1 sind der Ausgabepunkt. Nur vc1 transformieren.
  EX_PTVC:
    // DEB_dump_obj__ (Typ_PT, &pt1, "  EX_PTVC: pt1");
    // DEB_dump_obj__ (Typ_VC, &vc1, "  EX_PTVC: vc1");
  // transport nur vc1 into ActiveConstrPlane
  if(AP_IS_2D) {
    UT3D_vc_tra_vc_m3 (&vc1, WC_sur_mat, &vc1);
  }
  UT3D_pt_traptvc (pt_out, &pt1, &vc1);



  Exit:  
    // DEB_dump_obj__ (Typ_PT, pt_out, "ex APT_decode_pt:");
  return irc;


  Error:
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" ptErr [%d] typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }
  return -1;


  L_noFunc:
  TX_Error(" Pointdefinition not yet implemented ..");
  goto Error;
  // return -1;


  L_ParErr:
  TX_Error(" Pointdefinition: Error at Parameter  ..");
  goto Error;
  // return -1;


  L_Err_Int:
  TX_Error(" Pointdefinition: no intersectionpoint ..");
  goto Error;


  L_P__nyc:  // -3  object not yet complete
    return -3;

}


//================================================================
  int APT_decode_ln (Line *ln_out,
                     int aus_anz, int aus_typ[], double aus_tab[]) {
//================================================================
// APT_decode_ln                get vector or line from ATO
//   Input:
//
// noch offen:
// mit nur einem Punkt: den vorherigen Endpunkt als Startpunkt nehmen
// war vorher in APT_decode_func implementiert !!!!!!!


  int             irc, i1, i2, ii, rc, modNr, modTab[2], typ1, oNr, iSeg1, iSeg2;
  int           f_tra, f_angz, f_angy, f_offx, f_offy, f_offz, f_val, f_perp, f_mod;
  double          ang1, d1, d2, dx, dy, da[10], angz, angy, offx, offy, offz, dVal;
  // Line            ln1;
  Vector          vc1, vc2, vc3;
  Vector2         vc20, vc21;
  // Point           pt1;
  Point           pt1, pt2, pt3, pta, pte, pa[10];
  Point2          pi20,pt20,pt21,pt22,pt23,ptArr[2],ptArr1[2],ptAux[2];
  Line            ln1, ln2, *lnp;
  Line2           li20, ln20, ln21;
  Circ            ci1, ci2, *cip;
  Circ2           ci20, ci21;
  Plane           pl1, pl2, *plp;
  CurvBSpl        cv1;
  // Mat_4x3         m1;
  ObjGX           ox1, *ox1p, ox2, *ox2p;
  Memspc          memSeg1;
  void            *vp1;
  char            obj1[OBJ_SIZ_MAX], lTyp;




  // printf("APT_decode_ln |%d| APT_prim_typ=%d\n",aus_anz,APT_prim_typ);
  // for(i1=0; i1<aus_anz; ++i1)
  // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);



  rc     = 0;
  lTyp   = 0;    // limited
  f_tra  = 0;   // 0=do not UCS-translate; 1=yes,do.
  f_angz = 0;
  f_angy = 0;
  f_offx = 0;
  f_offy = 0;
  f_offz = 0;
  f_val  = 0;
  f_perp = 0;
  f_mod  = 0;
  iSeg1  = 0;
  iSeg2  = 0;
  // lnLen = APT_ln_len;



  if(aus_anz > 1) {
    // Typ_modUnlim must be last parameter
    // get UNL=3|UNL2=2|UNL1=1; def=0
    lTyp = APT_decode_modUnlim (&aus_anz, aus_typ, aus_tab);
      // printf(" f-modUnlim-lTyp=%d f_tra=%d aus_anz=%d\n",lTyp,f_tra,aus_anz);
  }

  i1 = aus_anz - 1;


  // get MOD
  if(aus_typ[i1] == Typ_modif) {
    f_mod = aus_tab[i1];
    --i1;
  }


  // get "CX" (perpendicular as last parameter -> f_perp
  if(aus_typ[i1] == Typ_cmdNCsub) {
    if(aus_tab[i1] == T_CX)  {
      f_perp = 1;
      --i1;
    }
  }


 
  L_start1:
       // printf(" L_start1: i1=%d\n",i1);
  if(i1 >= 1) {        // i1 = index to last param
    if(aus_typ[i1] == Typ_Angle) {
      // 1 angle: angz
      // 2 angles: first = angz, nxt = angy
      if(f_angz) {
        angy = angz;
        angz = aus_tab[i1];
        f_angy = 1;
      } else {
        angz = aus_tab[i1];
        f_angz = 1;
      }
      --i1;
      goto L_start1;
    }
  }

  L_start2:
  if(i1 >= 1) {        // i1 = index to last param
       // printf(" L_start2: i1=%d\n",i1);
    // Typ_Val = before angles and before Typ_XVal
    if(TYP_IS_VAL(aus_typ[i1])) {

      if(aus_typ[i1] == Typ_XVal) {
        offx = aus_tab[i1];
        f_offx = 1;
        --i1;
        goto L_start2;
      }
  
      if(aus_typ[i1] == Typ_YVal) {
        offy = aus_tab[i1];
        f_offy = 1;
        --i1;
        goto L_start2;
      }
  
      if(aus_typ[i1] == Typ_ZVal) {
        offz = aus_tab[i1];
        f_offz = 1;
        --i1;
        goto L_start2;
      }

      if(aus_typ[i1] == Typ_Val) {
        dVal = aus_tab[i1];
        f_val = 1;
        --i1;
      }
    }
  }

  // sequence of removal of parameters is:
  //   Val X|Y|Z angRot angTilt CX MOD UNL|1|2


  aus_anz = i1 + 1;

    // TESTBLOCK
    // printf(" decode_ln-aus_anz=%d f_angz=%d f_angy=%d lTyp=%d f_perp=%d f_mod=%d\n",
                       // aus_anz,    f_angz,   f_angy,   lTyp,   f_perp,   f_mod);
    // printf(" decode_ln-f_offx=%d f_offy=%d f_offz=%d f_val=%d\n",
                        // f_offx,   f_offy,   f_offz,   f_val);
    // for(i1=0; i1<aus_anz; ++i1)
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
    // END TESTBLOCK







  //================================================================
  if(aus_anz == 1)  {
  //================================================================



  //-----------------------------------------------------------------
  // L=Y(10)          // X-parallel; alt
  if(aus_typ[0] == Typ_YVal)     {

    vc1 = UT3D_VECTOR_X;  // X-parallel
    pt1 = UT3D_PT_NUL;
    pt1.y = aus_tab[0];
    f_tra = 1;
    lTyp  = 3;
    goto LL_exit_p_v;


  //-----------------------------------------------------------------
  // L=X(10)                          // Y-parallel; alt
  } else if(aus_typ[0] == Typ_XVal)     {

    vc1 = UT3D_VECTOR_Y;  // Y-parallel
    pt1 = UT3D_PT_NUL;
    pt1.x = aus_tab[0];
    f_tra = 1;  // transform = yes
    lTyp  = 3;
    goto LL_exit_p_v;


  //-----------------------------------------------------------------
  // L = D [X] [Y] [Z]
  } else if(aus_typ[0] == Typ_VC)     {
    vc1 = DB_GetVector ((long)aus_tab[0]);  // schon transformiert !
    pt1 = WC_sur_act.po; // UT3D_PT_NUL;
    goto L_pt_vc1;  // Input: pt1,vc1


  //-----------------------------------------------------------------
  // L = P                   // Copy Point | Tangente an Punkt auf Obj.
  } else if(aus_typ[0] == Typ_PT)     {

    pt1 = DB_GetPoint ((long)aus_tab[0]);

      // APT_prim_* must have been set from previous Point-Resolv (APT_decode_pt)
      // printf(" old prim.Obj: %d %ld %d %d %f\n",APT_prim_typ,APT_prim_ind,
              // APT_prim_seg,APT_prim_sg2,APT_prim_par);

    if((APT_prim_typ == 0) ||
       (APT_prim_typ == Typ_PT))     {  // normal .. (TraRot ..)
      vc1 = WC_sur_act.vz;
      goto L_pt_vc1;    // pt1 vc1

    } else if(APT_prim_typ == Typ_LN) {  // point on Line
      lnp = DB_get_LN (APT_prim_ind);
      UT3D_vc_ln (&vc1, lnp);
      goto L_pt_vc1;    // pt1 vc1

    } else if(APT_prim_typ == Typ_CI) {  // point on Circ
      cip = DB_get_CI (APT_prim_ind);
      UT3D_vc_tng_ci_pt (&vc1, &pt1, cip);
      goto L_pt_vc1;    // pt1 vc1

    } else if(APT_prim_typ == Typ_CV) {
      ox1p = DB_GetCurv (APT_prim_ind);

      if(ox1p->typ == Typ_CVPOL) {
        // get segment of plg;
        if(APT_prim_seg > 0) {
          rc = UT3D_2pt_plg_iseg (&pt1, &pt2, APT_prim_seg, ox1p->data);
          UT3D_vc_2pt (&vc1, &pt1, &pt2);
          if(rc < 0) return -1;
        } else {
          d1 = UT3D_par_par1plg (APT_prim_par, ox1p->data);
          UT3D_vc_evalplg (&vc1, ox1p->data, d1);
        }
        goto L_pt_vc1;    // pt1 vc1

      } else if(ox1p->typ == Typ_CVBSP) {
        d1 = UT3D_parbsp_par1 (APT_prim_par, ox1p->data);
        UT3D_vc_evalparCv (&vc1, ox1p->data, d1);
        goto L_pt_vc1;    // pt1 vc1

      } else if(ox1p->typ == Typ_CVELL) {
        UT3D_vc__pt_el (&vc1, &pt1, ox1p->data);
        goto L_pt_vc1;    // pt1 vc1

      } else if(ox1p->typ == Typ_CVTRM) {
        // get segment of ccv;
        // rc = UT3D_obj_segccv (&ox1, APT_prim_seg, ox1p);
        // rc = UT3D_crv_segccv (&i2, &ox1, APT_prim_seg, ox1p, &ln1);
        rc = UTO_obj__ccv_segnr (&typ1, obj1, APT_prim_seg, ox1p);
        if(rc < 0) return -1;

        if(typ1 == Typ_LN) {
          UT3D_vc_ln (&vc1, (Line*)obj1);
          goto L_pt_vc1;    // pt1 vc1

        } else if(typ1 == Typ_CI) {
          UT3D_vc_tng_ci_pt (&vc1, &pt1, (Circ*)obj1);
          goto L_pt_vc1;    // pt1 vc1

        } else if(typ1 == Typ_CVBSP) {
          d1 = UT3D_parbsp_par1 (APT_prim_par, (CurvBSpl*)obj1);
          UT3D_vc_evalparCv (&vc1, (CurvBSpl*)obj1, d1);
          goto L_pt_vc1;    // pt1 vc1

        } else if(typ1 == Typ_CVELL) {
          UT3D_vc__pt_el (&vc1, &pt1, (CurvElli*)obj1);
          goto L_pt_vc1;    // pt1 vc1

        } else if(typ1 == Typ_CVPOL) {
          if(APT_prim_par != UT_DB_LEER) {
            d1 = UT3D_par_par1plg (APT_prim_par, (CurvPoly*)obj1);
            UT3D_vc_evalplg (&vc1, obj1, d1);
          } else {
            rc = UT3D_2pt_plg_iseg (&pt1, &pt2, APT_prim_sg2, (CurvPoly*)obj1);
            if(rc < 0) return -1;
            UT3D_vc_2pt (&vc1, &pt1, &pt2);
          }
          goto L_pt_vc1;    // pt1 vc1

        } else goto NotImp_err;

      } else goto Par_err;

    } else goto NotImp_err;
    // goto LL_exit_c; // pta=midPt; vc1=Vc.



  //-----------------------------------------------------------------
  // L = S  [parameter]                   // S(lnTrm) parameter # tangent
  // L = S  [MOD]       OBSOLETE          // S(plg) MOD  # subSegment line
  } else if(aus_typ[0] == Typ_CV)     {
    ox1p = DB_GetCurv  ((long)aus_tab[0]);
    if(f_val) {
      // parameter + ellipse, bspl, plg ..
      // get point and vector, create line
      irc = UT3D_pt_vc__par_cv (&pt1, &vc1, ox1p->form, ox1p->data, 1, dVal);
      if(irc < 0) goto NotImp_err;
      // if(typ1 == Typ_LN) f_val = 0;
      // else              
        dVal = UT_LEN_TNG;        // tangent-length
      goto LL_exit_e_v;
    }
    if(f_mod) {
      iSeg1 = f_mod;
      f_mod = 0;
    MSG_ERR__ (ERR_obsolete, "change MOD to SEG");
    } else iSeg1 = 1;    // segment-index; 1=first.
    goto L_L_S_SEG1;


  //-----------------------------------------------------------------
  // L = L [parameter] [modif]             //  par = distance aside
  } else if(aus_typ[0] == Typ_LN)     {
    APT_modMax1 = 4;
    if(f_val) {
      APT_solv_ln_parl_mod (ln_out,f_mod,dVal,(long)aus_tab[0]);
      goto LL_exit2;
    }
    ln1 = DB_GetLine ((long)aus_tab[0]);
    pta = ln1.p1;
    pte = ln1.p2;
    goto LL_exit;


  //-----------------------------------------------------------------
  // L = C ..
  } else if(aus_typ[0] == Typ_CI)     {
    cip = DB_get_CI ((long)aus_tab[0]);


    // L = C MOD                  // line from centerPt to start/endPt
    if(f_mod) {
      // get vec
      UT3D_vc_mod_obj (&vc1, f_mod, Typ_CI, cip);
      pt1 = cip->pc;
      goto LL_exit_p_v;   // from pt1, vc1
    }

    // L = C param                // tangent to parametric point on circ
    if(f_val) {
      UT3D_pt_vc__par_ci (&pt1, &vc1, cip, dVal);
      // UT3D_vc_tng_ci_pt (&vc1, &pt1, cip);
      UT3D_vc_setLength (&vc1, &vc1, UT_LEN_TNG);  // length tangent
      goto LL_exit_p_v;   // from pt1, vc1
    }

    // L = C                      // line tru center into Z-direction
    pt1 = cip->pc;
    vc1 = cip->vz;
    UT3D_vc_setLength (&vc1, &vc1, UT_LEN_TNG);  // length tangent
    goto LL_exit_e_v;   // from pt1, vc1


  //-----------------------------------------------------------------
  // L = R                     line tru origin into Z-direction
  } else if(aus_typ[0] == Typ_PLN)     {

    plp = DB_get_PLN ((long)aus_tab[0]);
    pt1 = plp->po;
    vc1 = plp->vz;
    goto LL_exit_e_v;


  //-----------------------------------------------------------------
  } else goto Par_err;





  //================================================================
  } else if (aus_anz == 2)          {
  //================================================================

    // test if ato-1 == geom-obj
    if(aus_typ[0] < Typ_SymB) {
      // yes, ato-1 is geom-obj; test if also ato-2 == geom-obj
      if(aus_typ[1] < Typ_SymB) {
        // yes, bring first two geom.objects into correct sequence (D P L C S R A N)
        if(aus_typ[0] > aus_typ[1]) ATO_swap (aus_typ, aus_tab, 0, 1);
      }
    }


    //-----------------------------------------------------------------
    // L = D ..
    if(aus_typ[0] == Typ_VC) {
      vc1 = DB_GetVector ((long)aus_tab[0]);

      //-----------------------------------------------------------------
      // L = D P [X] [Y] [Z] [length] [angRot] [angTilt]
      if(aus_typ[1] == Typ_PT)         {
        pt1 = DB_GetPoint ((long)aus_tab[1]);
        UT3D_vc_setLength (&vc1, &vc1, UT_DISP_ln);  // length tangent
        goto L_pt_vc1;   // pt1, vc1

      //-----------------------------------------------------------------
      // L = D C           // tangent with fixed vec to circ ( + rotAng)
      } else if(aus_typ[1] == Typ_CI)         {
        cip = DB_get_CI ((long)aus_tab[2]);
        UT3D_pt_tng_ci_vc (&pt1, cip, &vc1, f_mod);
        goto L_pt_vc1;   // pt1, vc1

      //-----------------------------------------------------------------
      } else goto Par_err;


    //-----------------------------------------------------------------
    // L = P ..
    } else if(aus_typ[0] == Typ_PT) {
      pt1 = DB_GetPoint ((long)aus_tab[0]);

      //-----------------------------------------------------------------
      // L = P P                       // Linie P -> P
      if(aus_typ[1] == Typ_PT)         {
        pta = pt1;
        pte = DB_GetPoint ((long)aus_tab[1]);
        goto LL_exit; // from pta, pte, lTyp

      //-----------------------------------------------------------------
      // L = P L
      } else if (aus_typ[1] == Typ_LN)        {
        lnp = DB_get_LN ((long)aus_tab[1]);
        UT3D_vc_ln (&vc1, lnp);
        goto LL_exit_p_v;   // from pt1, vc1

      //-----------------------------------------------------------------
      // L = P D [length] [angRot] [angTilt]  
      } else if (aus_typ[1] == Typ_VC)        {
        vc1 = DB_GetVector ((long)aus_tab[1]);
        goto L_pt_vc1;  // pt1, vc1

      //-----------------------------------------------------------------
      // L = P R
      } else if (aus_typ[1] == Typ_PLN)         {
        plp = DB_get_PLN ((long)aus_tab[1]);
        vc1 = plp->vz;
        goto LL_exit_e_v;

      //-----------------------------------------------------------------
      // L = P C [mod]                 // Tangente an Circ durch Punkt
      } else if (aus_typ[1] == Typ_CI) {       
        // pta = DB_GetPoint ((long)aus_tab[0]);
        cip = DB_get_CI ((long)aus_tab[1]);

        APT_modMax1 = UT3D_pt_tangptci (&pte, &pt2, &pt1, cip);
        if(APT_modMax1 < 0) goto Par_err;
        if(f_mod == 2) pte = pt1;
        goto LL_exit;

      //---------------------------------------------------------------
      // L = P S [MOD]         // Line P tang to Curve (also L=TNG S P
      } else if (aus_typ[1] == Typ_CV) {
        ii = f_mod;
        f_mod = 0;

        pt1  = DB_GetPoint ((long)aus_tab[0]);
        ox1p = DB_GetCurv ((long)aus_tab[1]);
          // printf(" cvtyp=%d\n",ox1p->typ);

        if(ox1p->typ == Typ_CVELL) {
          // tangent thru point ON ellipse
          UT3D_vc__pt_el (&vc1, &pt1, ox1p->data);
          UT3D_vc_setLength (&vc1, &vc1, UT_LEN_TNG);  // length tangent
          // um pt1 eine Linie mit Richtung vc1.
          goto LL_exit_p_v;  // pt1, vc1


        } else if(ox1p->typ == Typ_CVBSP) {
          // UME_init (&memSeg1, memspc101, sizeof(memspc101));  // Init Datasegment
          // den Punkt pta auf die Kurve projizieren
          i1 = 10; // size of pa, da
          UT3D_pt_projptbspl (&i1, pa, da, ox1p->data, &pt1);
          if(i1 < 1) return -1;
          printf(" i1=%d da=%f\n",i1,da[0]);
          // Tangente zu da[0] holen   
          // printf(" modif=%d\n",ii);
          pt1 = pa[ii];
          UT3D_vc_evalparCv (&vc1, ox1p->data, da[ii]);
          UT3D_vc_setLength (&vc1, &vc1, UT_LEN_TNG);  // length tangent
            // DEB_dump_obj__ (Typ_PT, &pt1, " L=P S9 ");
            // DEB_dump_obj__ (Typ_VC, &vc1, " L=P S9 ");
          goto LL_exit_p_v; // pt1, vc1


        } else if(ox1p->typ == Typ_CVPOL) {

          // proj. Pt -> PolygonCurve
          i1 = 10;  // size of pTab !
          UT3D_pt_projptplg (&i1, pa, da, ox1p->data, &pt1);
          i2 = UT3D_ipt_cknear_npt (&pt1, pa, i1);

          // Tangente erzeugen
          UT3D_vc_evalplg (&vc1, ox1p->data, da[i2]);
          UT3D_vc_setLength (&vc1, &vc1, UT_LEN_TNG);  // length tangent
          // printf(" vc1=%f,%f,%f\n",vc1.dx,vc1.dy,vc1.dz);
          // um pt1 eine Linie mit Richtung vc1.
          LL_P_S_MOD9:
          if(f_val) {
            UT3D_vc_setLength (&vc1, &vc1, dVal);  // length tangent
          } else {
            UT3D_vc_setLength (&vc1, &vc1, UT_LEN_TNG);  // length tangent
          }
          goto LL_exit_p_v;


        } else goto NotImp_err;


      //-----------------------------------------------------------------
      } else goto Par_err;


    //-----------------------------------------------------------------
    // L = L ..
    } else if(aus_typ[0] == Typ_LN) {
      ln1 = DB_GetLine ((long)aus_tab[0]);

      //-----------------------------------------------------------------
      // L = L L [length] [angRot] [angTilt]      // inters.Pt
      if(aus_typ[1] == Typ_LN)         {
        ln2 = DB_GetLine ((long)aus_tab[1]);
        // get pt1 = intersectionPoint
        i1 = UT3D_pt_int2ln (&pt1, &pte, &d1, &ln1, &ln2);
        if(i1 < 0) {           // Linien parallel
          goto Parall_err;
        }
        if(i1 < 1) {          // dist > UT_TOL_pt
          UT3D_pt_mid2pt (&pt1, &pt1, &pte);
        }
        UT3D_vc_2pt (&vc1, &ln1.p1, &ln1.p2);
        UT3D_vc_2pt (&vc2, &ln2.p1, &ln2.p2);
        UT3D_vc_setLength (&vc1, &vc1, 1.);
        UT3D_vc_setLength (&vc2, &vc2, 1.);
        // vc1 -> Mittelllinie
        UT3D_vc_2vc (&vc1, &vc1, &vc2);
        if(!f_val) { f_val = 1; dVal = UT_DISP_ln; }  // default length line
        goto L_pt_vc2;  // pt1,vc1

      //-----------------------------------------------------------------
      // L = L R                         // proj. LN -> PLN
      } else if (aus_typ[1] == Typ_PLN)     {

        // printf("LN <- proj. LN -> PLN\n");
        ln1 = DB_GetLine ((long)aus_tab[0]);
        DB_GetRef(&pl1, (long)aus_tab[1]);

        UT3D_pt_projptpl (&pta, &pl1, &ln1.p1);
        UT3D_pt_projptpl (&pte, &pl1, &ln1.p2);
  
        goto LL_exit;


      //-----------------------------------------------------------------
      // 
      } else goto Par_err;



    //-----------------------------------------------------------------
    // L = C ..
    } else if(aus_typ[0] == Typ_CI) {
      cip = DB_get_CI ((long)aus_tab[0]);

      //-----------------------------------------------------------------
      // L = C C MOD                           // LN tang an 2 Circs
      if (aus_typ[1] == Typ_CI)             {
        ci1 = DB_GetCirc  ((long)aus_tab[1]);
  
        APT_modMax1 = UT3D_ln_tng_ci_ci (ln_out, cip, &ci1, f_mod);
          // printf(" APT_modMax1=%d\n",APT_modMax1);
        if(APT_modMax1 < 0) goto Par_err;
        goto LL_exit2;

      //-----------------------------------------------------------------
      // L = C PTS                // LN tang to standardPt on circ
      } else if (aus_typ[1] == Typ_PTS) {
        i1 = aus_tab[1];
        typ1 = Typ_CI;
        vp1 = cip;
        goto L_L_S_PTS1;


      //-----------------------------------------------------------------
      } else goto Par_err;


    //-----------------------------------------------------------------
    // L = S ..
    } else if(aus_typ[0] == Typ_CV) {
      ox1p = DB_GetCurv  ((long)aus_tab[0]);
        // DEB_dump_obj__(Typ_ObjGX, ox1p, "  decode_ln-S:");


      //-----------------------------------------------------------------
      // L = S PTS                  // get point and vector of tangent
      if(aus_typ[1] == Typ_PTS) {
        i1 = aus_tab[1];  // eg Ptyp_end ...

        // get bin.obj of curv
        irc = UTO_objDat_dbS (&vp1, &typ1, (long)aus_tab[0], iSeg1, iSeg2, obj1);
        if(irc < 0) goto Geom_err;

        // get point and vector of tangent
        L_L_S_PTS1:  // ptNr=i1, obj=(typ1,vp1)
        irc = UT3D_ptvcpar_std_obj (&pt1, &vc1, NULL, 0, i1, typ1, vp1);
        UT3D_vc_setLength (&vc1, &vc1, UT_LEN_TNG);  // length tangent
        if(irc < 0) goto Geom_err;
        goto LL_exit_e_v;

   
      //-----------------------------------------------------------------
      // L = S MOD param  // OBSOLETE version; if par given MOD not in f_mod
      } else if(aus_typ[1] == Typ_modif) {
        MSG_ERR__ (ERR_obsolete, "change MOD to SEG");

        // L = S MOD MOD                   OBSOLETE
        if(f_mod) {
          iSeg1 = aus_tab[1];
          iSeg2 = f_mod;
          goto L_L_S_SEG_SEG1;
        }

        // L = S MOD [parameter                // OBSOLETE version
        iSeg1 = aus_tab[1] - 1;    // segment-index; 0=first
        goto L_L_S_SEG1;


      //-----------------------------------------------------------------
      // L = S SEG [parameter]              // Line of plg or tangent to curve 
      } else if(aus_typ[1] == Typ_SEG) {
        iSeg1 = aus_tab[1];    // segment-index; 1=first.

        L_L_S_SEG1:    // S  iSeg1=segmentNr(1=first), f_val/dVal = parameter
        if(iSeg1 < 0) goto Geom_err;

        // extract Line from connectLine
        if(ox1p->form == Typ_LN) {
          rc = UT3D_2pt_segnln (&pta, &pte, iSeg1 - 1, ox1p->data);
          if(rc < 0) return -1;
          goto LL_exit;

        // extract Line from Polygon
        } else if(ox1p->form == Typ_CVPOL) {
          rc = UT3D_2pt_plg_iseg (&pta, &pte, iSeg1, ox1p->data);
          if(rc < 0) return -1;
          goto LL_exit;

        // get tangent to elli bspl trimmedCurve
        } else if((ox1p->form == Typ_CVELL)  ||
                  (ox1p->form == Typ_CVBSP)  ||
                  (ox1p->form == Typ_CVTRM))     {
          // get bin.obj of curv (standardCurve of trimmedCurve)
          irc = UTO_objDat_dbS (&vp1, &typ1, (long)aus_tab[0], iSeg1, iSeg2, obj1);
              // printf(" ex-UTO_objDat_dbS irc=%d typ1=%d\n",irc,typ1);
          if(irc < 0) goto Geom_err;
          if(typ1 == Typ_LN) {
            // line eg ex (CVPLG SEG)
            *ln_out = *((Line*)obj1);
            goto LL_exit1;
          }
          if(f_val) {
            // get point and vector of tangent from point on curve from parameter
            irc = UT3D_pt_vc__par_cv (&pt1, &vc1, typ1, vp1, 1, dVal);
            if(irc < 0) goto NotImp_err;
            if(typ1 == Typ_LN) f_val = 0;
            else               dVal = UT_LEN_TNG;        // tangent-length

          } else {
            // get point and vector of tangent from standardPoint on curve
            irc = UT3D_ptvcpar_std_obj (&pt1, &vc1, NULL, 0, i1, typ1, vp1);
            UT3D_vc_setLength (&vc1, &vc1, UT_LEN_TNG);  // length tangent
          }
          if(irc < 0) goto Geom_err;
          goto LL_exit_e_v;

        } else goto NotImp_err;


      //-----------------------------------------------------------------
      } else goto NotImp_err;


    //-----------------------------------------------------------------
    // L = R R                                  // L = Schnittlinie PLN - PLN
    } else if ((aus_typ[0] == Typ_PLN)   &&
               (aus_typ[1] == Typ_PLN))     {

      // printf("LN <- PLN PLN\n");
      DB_GetRef (&pl1, (long)aus_tab[0]);
      DB_GetRef (&pl2, (long)aus_tab[1]);

      UT3D_ptvc_int2pl (&pta, &vc1, &pl1, &pl2);
/* 2014-01-05 raus
      d1 = APT_ln_len / 2.;
      UT3D_pt_traptvclen (&pta, &pt1, &vc1, -d1);
      UT3D_pt_traptvclen (&pte, &pt1, &vc1, d1);
*/
      UT3D_pt_traptvc (&pte, &pta, &vc1);

      goto LL_exit;




    //-----------------------------------------------------------------
    } // else goto Par_err;



  //================================================================
  } else if (aus_anz == 3) {
  //================================================================


    //-----------------------------------------------------------------
/*
    // L = P S PTI                      // Linie from P tang. to Curve
    if        ((aus_typ[0] == Typ_PT)    &&
               (aus_typ[1] == Typ_CV))       {
      } else if (aus_typ[2] == Typ_PTS)      {
      } else goto Geom_err;
*/

    //-----------------------------------------------------------------
    // L = S . .
    if (aus_typ[0] == Typ_CV)   {
      // ox1p = DB_GetCurv ((long)aus_tab[0]);
      // DEB_dump_dbo (Typ_CV, (long)aus_tab[0], "  _decode_ln-L-S . .");

      //----------------------------------------------------------------
      // L = S SEG .
      if(aus_typ[1] == Typ_SEG)  {
        iSeg1 = aus_tab[1];  // 1=first

        //----------------------------------------------------------------
        // L = S SEG SEG                        // Linie ex CCV/Polygon
        if(aus_typ[2] == Typ_SEG)     {
          iSeg2 = aus_tab[2];  // 1=first
  
          L_L_S_SEG_SEG1:
          irc = UTO_objDat_dbS (&vp1, &typ1, (long)aus_tab[0], iSeg1, iSeg2, obj1);
          if(irc < 0) goto Geom_err;
            // DEB_dump_obj__(typ1, vp1, "  _decode_ln-L-Si_SEG-SEG");
  
          if(typ1 == Typ_LN) {
            pt1 = ((Line*)vp1)->p1;
            UT3D_vc_ln (&vc1, (Line*)vp1);
            goto L_pt_vc1;  // pt1 vc1
  
          } else goto Par_err;

        //----------------------------------------------------------------
        // L = S SEG PTS                        // Linie at CCV/Circ ..
        } else if(aus_typ[2] == Typ_PTS)    {
          iSeg2 = 0;  // 1=first
          i1 = aus_tab[2];    // 1=start, 2=endPt

          L_L_S_SEG_PTS1:
          // get standardCurve from trimmed-curve
          irc = UTO_objDat_dbS (&vp1, &typ1, (long)aus_tab[0], iSeg1, iSeg2, obj1);
          if(irc < 0) goto Geom_err;
            // DEB_dump_obj__(typ1, vp1, "  _decode_ln-L-Si_SEG-PTS");
          // get pt & vec at standard-point i1
          irc = UT3D_ptvcpar_std_obj (&pt1, &vc1, NULL, 0, i1, typ1, vp1);
          if(irc < 0) goto Par_err;
          UT3D_vc_setLength (&vc1, &vc1, UT_LEN_TNG);  // length tangent
          goto L_pt_vc1;  // pt1,vc1

        //----------------------------------------------------------------
        // L = S SEG PTI                        // Linie ex CCV/Polygon
        } else if(aus_typ[2] == Typ_PTI)    {
          // for polygon: find segmentNr from point-index
          iSeg2 = aus_tab[2];  // 1=first

          goto NotImp_err;

        //----------------------------------------------------------------
        } else goto NotImp_err;

      //----------------------------------------------------------------
      } else goto NotImp_err;

    //----------------------------------------------------------------
    } else goto NotImp_err;


/*
  //================================================================
  } else if (aus_anz == 4) {
  //================================================================
    return MSG_ERR__ (ERR_TODO_I, "BAUSTELLE-L-4");
*/

  //-----------------------------------------------------------------
  } else goto NotImp_err;




  //================================================================
  //================================================================
  // L = P D [Val] [Angle-Rot.] [Angle-Tilt]
  L_pt_vc0:
      // printf(" L_pt_vc0: lTyp=%d\n",lTyp);
    pt1 = DB_GetPoint ((long)aus_tab[0]);
    vc1 = DB_GetVector ((long)aus_tab[1]);


  L_pt_vc1: // Input: pt1,vc1
    // set [X] [Y] [Z]
    if(f_offx) pt1.x += offx;
    if(f_offy) pt1.y += offy;
    if(f_offz) pt1.z += offz;

    // if(lTyp == 0) lTyp = 1;  // default = side 1 unlimited


  L_pt_vc2: // Input: pt1,vc1
    // set [angRot] [angTilt]
    // if(aus_typ[i1] != Typ_Angle) goto Par_err;
    if(f_angz) {
      d1 = UT_RADIANS(angz) * -1.;                // revert 2011-10-14
      UT3D_vc_rotvcangr (&vc1, d1, &vc1, &WC_sur_act.vz);
      // ++i1;
      // if(aus_anz == i1) goto LL_exit_e_v;

      if(f_angy) {
        // vc1 = Normalwinkel auf pt1-pt0 in der X-Y-Ebene.
        UT3D_vc_perp2vc (&vc2, &vc1, (Vector*)&WC_sur_act.vz);
        UT3D_vc_setLength (&vc2, &vc2, 1.);
          // GR_tDyn_vc__ (&vc2, &pt1, 2, 0);

        // [Angle-Tilt]
        // if(aus_typ[i1] != Typ_Angle) goto Par_err;
        d1 = UT_RADIANS(angy) * -1.;                // revert 2011-10-14
        UT3D_vc_rotvcangr (&vc1, d1, &vc1, &vc2);
      }
    }



  //================================================================
  LL_exit_e_v: // Input: pt1,vc1
  // set length from dVal
  if(f_val) {
    pta = pt1;
    // get pte - distance <dVal> from pta
    UT3D_pt_traptvclen (&pte, &pt1, &vc1, dVal);
    goto LL_exit;  // pta pte
  }


  //----------------------------------------------------------------
  LL_exit_p_v:  // Input: pt1, vc1; Out: pta, pte, lTyp.
    // printf(" LL_exit_p_v: lTyp=%d\n",lTyp);
  // create Ray from point pt1 and vector vc1.
  // create unlimited line (ray) from Inputs: pt1, vc1(normalized).  2014-01-07
  pta = pt1;
  // get pte from vc1
  UT3D_pt_traptvc (&pte, &pta, &vc1);

/* removed - polygon-segments come without length ..
  // if no ltyp is given and no length (f_val) is given,
  //   then set lintype = 1 (UNL1 = side 1 unlimited)
  if((lTyp == 0)&&(f_val == 0)) lTyp = 1;
*/

  goto LL_exit;


  //----------------------------------------------------------------
  LL_exit_p_l:
  // Input sind pt1, vc1.
  // pt1 = LineStartpunkt, LineEndpunkt = pt1+vc1.
  pta = pt1;
  UT3D_pt_traptvc (&pte, &pt1, &vc1);
  goto LL_exit;


  //----------------------------------------------------------------
  LL_exit_c:
  // Input sind pta, pte.
  // Verlaengern; pta soll Mitte sein.
  // create 2 opposite Points aus Mittelpunkt, Vektor, Abstand.
  UT3D_vc_2pt (&vc1, &pte, &pta);
  UT3D_pt_traptvc (&pta, &pta, &vc1);
  // goto LL_exit;


  //----------------------------------------------------------------
  // Transform Line pta-pte
  LL_exit:  // Inp: pta, pte, f_perp f_tra lTyp
    // printf(" _decode_ln-LL_exit-lTyp=%d f_tra=%d\n",lTyp,f_tra);

  ln_out->p1  = pta;
  ln_out->p2  = pte;
  ln_out->typ = lTyp;
    // DEB_dump_obj__ (Typ_LN, ln_out, "  _decode_ln-LL_exit");


  //----------------------------------------------------------------
  LL_exit1:  // Inp: ln_out
  // work perp (CX)
  if(f_perp) {
    // get normal vec from vc1, ConstrPln.vz
    UT3D_ln_rot_90_sr (ln_out, ln_out, &WC_sur_act.vz, 0);
  }

  //----------------------------------------------------------------
  LL_exit2:   // Inp: ln_out
  // work unlimited 
  if(lTyp != 0) {
    UT3D_ln_unlim (ln_out, lTyp);
    APT_subTyp = Typ_Att_dim;
  }


  // translate ln_out
  if(f_tra != 0) {
    if(AP_IS_2D) {
      UT3D_pt_tra_pt_m3 (&ln_out->p1, WC_sur_mat, &ln_out->p1);
      UT3D_pt_tra_pt_m3 (&ln_out->p2, WC_sur_mat, &ln_out->p2);
      goto Exit;
    }
  }


  //----------------------------------------------------------------
  Exit:
    // DEB_dump_obj__ (Typ_LN, ln_out, "ex APT_decode_ln");
  return rc;


  //================= Fehler =============================
  Par_err:
    TX_Error("Linedefinition: ParameterError ****");
    goto Error;

  Geom_err:
    TX_Error("Linedefinition: GeometryError ****");
    goto Error;

  NotImp_err:
    TX_Error("Linedefinition: NOT YET IMPLEMENTED ****");
    goto Error;

  Parall_err:
    TX_Error("Linedefinition: Lines may not be parallel ****");
    goto Error;

  Error:
  return -1;

}

/*  APT_decode_pt - old functions:

        // extract Line from CCV
        } else if(ox1p->form == Typ_CVTRM) {
          // get ccv-seg
          irc = UTO_obj__ccv_segnr (&i2, obj1, iSeg1, ox1p); // i2=subSegTyp
          if(irc < 0) return -1;
          if(i2 == Typ_LN) {
            pta = ((Line*)obj1)->p1;
            pte = ((Line*)obj1)->p2;
            goto LL_exit;

          } else {
            if(f_val) {
              // have L = S MOD(<segNr> parameter   (parameter is in f_val/dVal)
              rc = UT3D_pt_vc__par_cv (&pt1, &vc1, i2, obj1, 0, dVal);
              if(rc < 0) goto NotImp_err;
              f_val = 0;
              goto LL_exit_e_v;
            } else {
              printf(" L_L_S_SEG1-subSegTyp=%d\n",i2);
              goto Geom_err;
            }
          }

      //----------------------------------------------------------------
      // L = S MOD MOD          extract Line aus Polygon aus CCV
      // 1. MOD = segmentNr of CCV; 1=first
      // 2. MOD = segmentNr of Plg; 1=first
      // get segment of ccv;
      if(ox1p->typ != Typ_CVTRM) goto Par_err;
      i1 = aus_tab[1] - 1;   // segNr -> indexNr; 0=first seg.
        // get bin-obj of CCV-segment
        irc = UTO_obj__ccv_segnr (&i2, obj1, i1, ox1p);
        if(irc < 0) return -1;
        // if(UTO_ck_curvForm(i1) != Typ_ObjGX) goto Par_err;
        if(i2 != Typ_CVPOL) goto Par_err;
        // get Line of Polygon
        i1 = aus_tab[2];  // segNr
        rc = UT3D_2pt_plg_iseg (&pta, &pte, i1, obj1);
        if(rc < 0) return -1;
        goto LL_exit;


    //--------------------------------------------------------------------
    // L = S modif val                    // Tangente an AC|Bsp|Ell ex CCV
    } else if ((aus_typ[0] == Typ_CV)       &&
               (aus_typ[1] == Typ_modif)    &&
               (aus_typ[2] == Typ_Val))         {


      ox1p = DB_GetCurv ((long)aus_tab[0]);

      // extract Line aus Polygon aus CCV
      // 1. MOD = segment of CCV;
      // 2. MOD = segment of Plg;

      // get segment of ccv;
      if(ox1p->typ != Typ_CVTRM) goto Par_err;
      i1 = aus_tab[1];
      rc = UT3D_obj_segccv (&ox1, i1, ox1p);
      if(rc < 0) return -1;

      if(ox1.form == Typ_CI) {
        UT3D_pt_vc__par_ci (&pt1, aus_tab[2], ox1.data);
        UT3D_vc_tng_ci_pt (&vc1, &pt1, ox1.data);
        goto LL_exit_p_v;

      } else if(ox1.form == Typ_CVBSP) {
        d1 = UT3D_parbsp_par1 (aus_tab[2], ox1.data);
        UT3D_vc_evalparCv (&vc1, ox1.data, d1);
        UT3D_pt_evalparCv (&pt1, ox1.data, d1);
        goto LL_exit_p_v;

      } else if(ox1.form == Typ_CVELL) {
        d1 = UT3D_angr_par1_ell (aus_tab[2], ox1.data);
        UT3D_pt_eval_ell_par1 (&pt1, ox1.data, d1);
        UT3D_vc__pt_el (&vc1, &pt1, ox1.data);
        goto LL_exit_p_v;

      } else goto Par_err;


  //================================================================
  // ab hier aus_anz gemischt;
  //================================================================
  } // else goto Par_err;


  // get UNL=3|UNL2=2|UNL1=1; def=0
  lTyp = APT_decode_modUnlim (&aus_anz, aus_typ, aus_tab);
    // printf(" lTyp=%d f_tra=%d aus_anz=%d\n",lTyp,f_tra,aus_anz);

  i1 = aus_anz - 1;

  if(aus_typ[i1] == Typ_Angle) {
    f_angz = 1;
    angz = aus_tab[i1];
    --i1;
  }

  if(aus_typ[i1] == Typ_Angle) {
    f_angy = 1;
    if(angz != UT_DB_LEER) angy = angz;



  //-----------------------------------------------------------------
  // L = L Abstand Position          // Parallel-Line
  } else if ((aus_anz    ==   3)             &&
             (aus_typ[0] == Typ_LN)          &&
             (aus_typ[1] == Typ_Val)         &&
             (aus_typ[2] == Typ_modif))       {

    APT_modMax1 = 4;
    APT_solv_ln_parl_mod (ln_out,(int)aus_tab[2],aus_tab[1],(long)aus_tab[0]);
    goto Exit;

    ln1 = DB_GetLine ((long)aus_tab[0]);
    d1  = aus_tab[1];
    i1 = aus_tab[2];

    pta = ln1.p1;
    pte = ln1.p2;
    UT3D_vc_2pt (&vc1, &pta, &pte);


    if(i1 == 0) {
       // d1 = -d1;

    // } else if(i1 == 1) {

    } else if(i1 == Typ_modHIX) {
      // vec soll von oben nach unten
      if(vc1.dy > 0.) UT3D_vc_invert (&vc1, &vc1);

    } else if(i1 == Typ_modLOX) {
      // vec soll von unten nach oben
      if(vc1.dy < 0.) UT3D_vc_invert (&vc1, &vc1);
    L_parall_2:
      // printf(" L_parall_2: i1=%d d1=%f\n",i1,d1);
    // vc1 = Richtung der Inputline.
      // DEB_dump_obj__(Typ_VC, &vc1, "  vc1:");
      // DEB_dump_obj__(Typ_VC, &WC_sur_act.vz, "  WC_sur_act.vz:");
    // test if vc1 is in ConstrPlane. If not: Error !
    UT3D_vc_setLength (&vc3, &vc1, 1.);
    // project vc1 -> WC_sur_act.vz; must be 0 !
    UT3D_parvc_2vc (&d2, &vc1, &WC_sur_act.vz);
      // printf(" par. vc1 -> ConstrPlane-Z=%f\n",d2);
    if(fabs(d2) > UT_TOL_Ang1) {
      TX_Error("Line must be parallel to ConstructionPlane ..");
      return -1;
    }


    // vc2 = Normalvec mit der Z-Achse
    UT3D_vc_perp2vc (&vc2, (Vector*)&WC_sur_act.vz, &vc1);


    if((i1 == Typ_modLOX)||(i1 == 1)) {
      d1 = -d1;

    } else if((i1 == Typ_modHIZ)||(i1 == 2)) {
      // vc2 = Normalvec mit der Z-Achse
      UT3D_vc_perp2vc (&vc2, &vc1, &vc2);

    } else if((i1 == Typ_modLOZ)||(i1 == 3)) {
      UT3D_vc_perp2vc (&vc2, &vc2, &vc1);
    }


    // pta, pte = Line;
    // vc2 = normal to this line; move points in this direction, length = d1
    UT3D_pt_traptvclen (&pta, &pta, &vc2, d1);
    UT3D_pt_traptvclen (&pte, &pte, &vc2, d1);
    APT_modMax1 = 4;
    goto LL_exit;


    } else if(i1 == Typ_modHIY) {
      // vec soll von links nach rechts
      if(vc1.dx < 0.) UT3D_vc_invert (&vc1, &vc1);

    } else if(i1 == Typ_modLOY) {
      // vec soll von rechts nach links
      if(vc1.dx > 0.) UT3D_vc_invert (&vc1, &vc1);

    } else if((i1 == Typ_modHIZ)||(i1 == 2)) {
      // vec soll von unten nach oben
      if(vc1.dz < 0.) UT3D_vc_invert (&vc1, &vc1);

    } else if((i1 == Typ_modLOZ)||(i1 == 3)) {
      // vec soll von oben nach unten
      if(vc1.dz > 0.) UT3D_vc_invert (&vc1, &vc1);
    }


    angy = aus_tab[i1];
    --i1;
  }


  aus_anz = i1 + 1;


  //-----------------------------------------------------------------
  // L = C C InOut Position          // Parallel-Line
  } else if ((aus_anz    ==   4)             &&
             (aus_typ[0] == Typ_CI)          &&
             (aus_typ[1] == Typ_CI)          &&
             (aus_typ[2] == Typ_modif)    &&
             (aus_typ[3] == Typ_modif))       {


    ci1 = DB_GetCirc  ((long)aus_tab[0]);
    ci2 = DB_GetCirc  ((long)aus_tab[1]);


    // Typ_modInOut komt als Typ_modif IN=2 OUT=3
    if(aus_tab[2] > 2.5) {
      i2 = 0;  // OUT
    } else {
      i2 = 1;  // IN
    }
    i1 = UT3D_ln_tng_ci_ci (&ln1, &ci1, &ci2, i2);
    if(i1 < 0) {
      TX_Error("keine Tangente moeglich");
      return -1;
    }

    // test Startpkt of ln.
    modTab[0] = aus_tab[3];
    pa[0] = ln1.p1;
    pa[1] = ln2.p1;
    i2 = APT_solv3D_mod (pa, 2, modTab, 1);
    if(i2 == 0) {
      pta = ln1.p1;
      pte = ln1.p2;
    } else {
      pta = ln2.p1;
      pte = ln2.p2;
    }
    goto LL_exit;


  ///////////////////////////////////////////////////////////////
  //ALTE METHODE:


  i1 = APT_decode_inp (aus_anz, aus_typ, aus_tab);
  if(i1 < 0) goto Error;


  //-----------------------------------------------------------------
  // l1=l2
  if        ((IN_anz       ==   1)        &&
             (IN_obj[0].typ==Typ_LN)) {


    pta = IN_obj[0].p1;
    pte = IN_obj[0].p2;

    goto LL_exit;


  //-----------------------------------------------------------------
  // l1=p1,p2
  } else if ((IN_anz       ==   2)       &&
             (IN_obj[0].typ==Typ_PT)     &&
             (IN_obj[1].typ==Typ_PT)) {

    pta = IN_obj[0].p1;
    pte = IN_obj[1].p1;

    goto LL_exit;


  //-----------------------------------------------------------------
  // l1=p1,a(0)
  } else if ((IN_anz       ==   2)       &&
             (IN_obj[0].typ==Typ_PT)     &&
             (IN_obj[1].typ==Typ_Angle)) {


    // Angle+len > Vector2
    UT2D_vc_angrlen (&vc20, UT_RADIANS (IN_obj[1].rad), APT_ln_len);


    pta.x = IN_obj[0].p1.x + vc20.dx;
    pta.y = IN_obj[0].p1.y + vc20.dy;
    pta.z = IN_obj[0].p1.z;

    pte.x = IN_obj[0].p1.x - vc20.dx;
    pte.y = IN_obj[0].p1.y - vc20.dy;
    pte.z = IN_obj[0].p1.z;

    goto LL_exit;


  //-----------------------------------------------------------------
  // l1=l1,10,HI             (paralell-linie im abst)
  } else if ((IN_anz       ==   2)       &&
             (IN_obj[0].typ==Typ_LN)     &&
             (IN_obj[1].typ==Typ_Val)) {


    // Line > Vec
    UT3D_vc_2pt (&vc1, &IN_obj[0].p1, &IN_obj[0].p2);

    // VC halbieren
    vc1.dx = vc2.dx / 2.0;
    vc1.dy = vc2.dy / 2.0;
    vc1.dz = vc2.dz / 2.0;
    //printf("  vc2=%f,%f\n",vc2.dx,vc2.dy);

    // nun den Mittelpunkt von l1
    // pt20.x = (IN_obj[0].p2.x + IN_obj[0].p1.x) / 2.0;
    // pt20.y = (IN_obj[0].p2.y + IN_obj[0].p1.y) / 2.0;
    UT3D_pt_mid2pt(&pt1, &IN_obj[0].p1, &IN_obj[0].p2);
    pt20.x = pt1.x;
    pt20.y = pt1.y;
    //printf("  ln-mitte=%f,%f\n",pt20.x,pt20.y);

    // nun normal darauf mit Abstand Val; 2 Lösungen
    pt21 = UT2D_pt_pt3 (&IN_obj[0].p2);
    UT2D_pt_tranor2ptlen (&ptArr[0], &pt20, &pt21, IN_obj[1].rad);
    UT2D_pt_tranor2ptlen (&ptArr[1], &pt20, &pt21, -IN_obj[1].rad);

    // nun die Lösung entspr. ModTab suchen
    i1 = APT_solv_mod (ptArr, 2);

    pta.x = ptArr[i1].x + vc1.dx;
    pta.y = ptArr[i1].y + vc1.dy;
    pta.z = pt1.z       + vc1.dz;

    pte.x = ptArr[i1].x - vc1.dx;
    pte.y = ptArr[i1].y - vc1.dy;
    pte.z = pt1.z       - vc1.dz;

    goto LL_exit;


  //-----------------------------------------------------------------
  // l1=l1,l2               (Mittellinie (ev noch drehen))
  } else if(((IN_anz       ==   2)       &&
             (IN_obj[0].typ==Typ_LN)     &&
             (IN_obj[1].typ==Typ_LN))    ||

            ((IN_anz       ==   3)       &&
             (IN_obj[0].typ==Typ_LN)     &&
             (IN_obj[1].typ==Typ_LN)     &&
             (IN_obj[2].typ==Typ_Angle))) {

    ang1 = 0.0;
    if(IN_obj[2].typ==Typ_Angle) ang1 = UT_RADIANS (IN_obj[2].rad);


    // die 2 engsten Punkte suchen
    ln1.p1=IN_obj[0].p1;
    ln1.p2=IN_obj[0].p2;
    ln2.p1=IN_obj[1].p1;
    ln2.p2=IN_obj[1].p2;
    i1 = UT3D_pt_int2ln(&pt1,&pt2,&d1,&ln1,&ln2);

    if(i1 < 0) {    // Lines parallel or zero-length
      UT3D_pt_mid2pt(&pt1,&ln1.p1,&ln2.p1);
      UT3D_pt_mid2pt(&pt2,&ln1.p2,&ln2.p2);
      UT3D_vc_2pt (&vc1, &IN_obj[0].p1, &IN_obj[0].p2);
      goto L_L_LL_9;


    } else if(i1 == 1) {   // dist gives the minimum distance.
      UT3D_pt_mid2pt(&pt1,&pt1,&pt2);

    }
    // Mittelpt = pt1.
    // Mittelvec.
    UT3D_vc_2pt (&vc2, &IN_obj[0].p1, &IN_obj[0].p2);
    UT3D_vc_2pt (&vc3, &IN_obj[1].p1, &IN_obj[1].p2);
    UT3D_vc_2vc (&vc1, &vc2, &vc3);

    L_L_LL_9:
    UT3D_vc_setLength (&vc1, &vc1, APT_ln_len);

    // vc2 wenn nowendig drehen
    if(!UTP_comp_0(ang1)) {
      vc20.dx = vc1.dx;
      vc20.dy = vc1.dy;
      UT2D_vc_rotangr (&vc20, &vc20, ang1);
      vc1.dx = vc20.dx;
      vc1.dy = vc20.dy;
    }

    pta.x = pt1.x + vc1.dx;
    pta.y = pt1.y + vc1.dy;
    pta.z = pt1.z + vc1.dz;

    pte.x = pt1.x - vc1.dx;
    pte.y = pt1.y - vc1.dy;
    pte.z = pt1.z - vc1.dz;
    goto LL_exit;


  //-----------------------------------------------------------------
  // l1=p1,l1                 (Parallel-Linie von p1 weg)
  } else if ((IN_anz       ==   2)        &&
             (IN_obj[0].typ==Typ_PT)     &&
             (IN_obj[1].typ==Typ_LN))   {

    // LN > VC
    UT3D_vc_2ptlen (&vc1, &IN_obj[1].p1, &IN_obj[1].p2, APT_ln_len);

    pta.x = IN_obj[0].p1.x + vc1.dx;
    pta.y = IN_obj[0].p1.y + vc1.dy;
    pta.z = IN_obj[0].p1.z + vc1.dz;

    pte.x = IN_obj[0].p1.x - vc1.dx;
    pte.y = IN_obj[0].p1.y - vc1.dy;
    pte.z = IN_obj[0].p1.z - vc1.dz;

    goto LL_exit;


  //-----------------------------------------------------------------
  // l1=p1,c1,HI            (Tangente von p1 an c1)  Nur 2D.
  } else if ((IN_anz       ==   2)        &&
             (IN_obj[0].typ==Typ_PT)     &&
             (IN_obj[1].typ==Typ_CI)) {

    pi20 = UT2D_pt_pt3 (&IN_obj[0].p1);

    ci20.p1  = UT2D_pt_pt3 (&IN_obj[1].p1);
    ci20.p2  = UT2D_pt_pt3 (&IN_obj[1].p2);
    ci20.pc  = UT2D_pt_pt3 (&IN_obj[1].pc);
    ci20.rad = IN_obj[1].rad;

    // b = d3 = Kathete; von Cir.pc zum gesuchten Tangentenpunkt = Radius
    d3 = fabs(IN_obj[1].rad);

    // c = d4 = Hypotenuse Cir.pc  -> P1
    d4 = UT2D_len_2pt (&pi20, &ci20.pc);


    if(UTP_comp2db(d3, d4, UT_TOL_pt)) {
      goto PT_PT_2;

    } else if(d4 < d3) {
      TX_Error(" Punkt innerhalb Kreis ****\n");
      goto Error;
    }

    //UT2D_solvtriri_ac ( *q, *hc, b,  c)
    UT2D_solvtriri_ac   (&d1, &d2, d3, d4);

    //printf(" d1-d4=%f %f %f %f\n",d1,d2,d3,d4);

    // vom Cir.pc in Richtung P1 d1 als dx und d2 als dy abtragen
    UT2D_pt_tra2pt2len (&ptArr[0], &ci20.pc, &pi20, d1, d2);

    // 2. Lösung mit negativem d2=dy.
    UT2D_pt_tra2pt2len (&ptArr[1], &ci20.pc, &pi20, d1, -d2);

    // nun die Lösung entspr. ModTab suchen
    i1 = APT_solv_mod (ptArr, 2);

    pta = IN_obj[0].p1;
    pte = UT3D_pt_pt2(&ptArr[i1]);
    goto LL_exit;

    // Lösung, wenn Punkt schon am Kreis liegt:
    PT_PT_2:
    UT2D_vc_2ptlen (&vc20,  &pi20, &ci20.pc, APT_ln_len);
    // nun um 90 Grad drehen
    UT2D_vc_rot_90_ccw (&vc20, &vc20);

    pta.x = IN_obj[0].p1.x + vc20.dx;
    pta.y = IN_obj[0].p1.y + vc20.dy;
    pta.z = IN_obj[0].p1.z;

    pte.x = IN_obj[0].p1.x - vc20.dx;
    pte.y = IN_obj[0].p1.y - vc20.dy;
    pte.z = IN_obj[0].p1.z;

    goto LL_exit;


  //-----------------------------------------------------------------
  // L1=C1,A(45),HI               Tangente an ci m. Winkel   Nur 2D.
  } else if ((IN_anz       ==   2)       &&
             (IN_obj[0].typ==Typ_CI)     &&
             (IN_obj[1].typ==Typ_Angle)) {

    ci20.p1  = UT2D_pt_pt3 (&IN_obj[0].p1);
    ci20.p2  = UT2D_pt_pt3 (&IN_obj[0].p2);
    ci20.pc  = UT2D_pt_pt3 (&IN_obj[0].pc);
    // ci20.rad = IN_obj[1].rad;

    // b = d3 = Kathete; von Cir.pc zum gesuchten Tangentenpunkt = Radius
    d1 = fabs(IN_obj[0].rad);

    // rad von pc in Richtg Ang
    ang1 = UT_RADIANS (IN_obj[1].rad);
    d2 = ang1 + RAD_90;
    UT2D_pt_traptangrlen (&ptArr[0], &ci20.pc, d2, d1);

    // 2. Lösung mit negativem d2=dy.
    UT2D_pt_traptangrlen (&ptArr[1], &ci20.pc, d2, -d1);

    // nun die Lösung entspr. ModTab suchen
    i1 = APT_solv_mod (ptArr, 2);

    // einen Vektor in Richtung Ang Länge ln_len
    UT2D_vc_angrlen (&vc20, ang1, APT_ln_len);

    pta.x = ptArr[i1].x + vc20.dx;
    pta.y = ptArr[i1].y + vc20.dy;
    pta.z = IN_obj[0].pc.z;

    pte.x = ptArr[i1].x - vc20.dx;
    pte.y = ptArr[i1].y - vc20.dy;
    pte.z = IN_obj[0].pc.z;

    goto LL_exit;


  //-----------------------------------------------------------------
  // L1=L1,C1,HI        (Tangente parellel zu L1 an C1)     Nur 2D.
  } else if ((IN_anz       ==   2)       &&
             (IN_obj[0].typ==Typ_LN)     &&
             (IN_obj[1].typ==Typ_CI)) {

    li20.p1 =  UT2D_pt_pt3 (&IN_obj[0].p1);
    li20.p2 =  UT2D_pt_pt3 (&IN_obj[0].p2);

    ci20.p1  = UT2D_pt_pt3 (&IN_obj[1].p1);
    ci20.p2  = UT2D_pt_pt3 (&IN_obj[1].p2);
    ci20.pc  = UT2D_pt_pt3 (&IN_obj[1].pc);


    d1 = fabs(IN_obj[1].rad);

    //Schnittpunkt am Kreis mit Normaler zu Linie
    UT2D_pt_tranor2ptlen (&pt20, &li20.p1,  &li20.p2,  d1);

    // nun ist der gesuchte Punkt auf ptArr aber von l.p1 weg statt von c.pc.
    dx = pt20.x - li20.p1.x;
    dy = pt20.y - li20.p1.y;

    ptArr[0].x = ci20.pc.x + dx;
    ptArr[0].y = ci20.pc.y + dy;

    ptArr[1].x = ci20.pc.x - dx;
    ptArr[1].y = ci20.pc.y - dy;
    //printf("p1,p2=%f,%f / %f,%f / %f,%f\n",ptArr[0].x,ptArr[0].y,
    //             ptArr[1].x,ptArr[1].y, dx,dy);

    // nun die Lösung entspr. ModTab suchen
    i1 = APT_solv_mod (ptArr, 2);

    // einen Vector von Länge ln_len in Richtg der Linie
    UT2D_vc_2ptlen (&vc20, &li20.p1, &li20.p2, APT_ln_len);

    pta.x = ptArr[i1].x + vc20.dx;
    pta.y = ptArr[i1].y + vc20.dy;
    pta.z = IN_obj[1].pc.z;

    pte.x = ptArr[i1].x - vc20.dx;
    pte.y = ptArr[i1].y - vc20.dy;
    pte.z = IN_obj[1].pc.z;

    goto LL_exit;


  //-----------------------------------------------------------------
  // L1=C1,C2,OUT,HI               (Tangente an ci - ci)    Nur 2D.
  } else if ((IN_anz       ==   2)       &&
             (IN_obj[0].typ==Typ_CI)     &&
             (IN_obj[1].typ==Typ_CI)) {

    ci20.p1  = UT2D_pt_pt3 (&IN_obj[0].p1);
    ci20.p2  = UT2D_pt_pt3 (&IN_obj[0].p2);
    ci20.pc  = UT2D_pt_pt3 (&IN_obj[0].pc);

    ci21.p1  = UT2D_pt_pt3 (&IN_obj[1].p1);
    ci21.p2  = UT2D_pt_pt3 (&IN_obj[1].p2);
    ci21.pc  = UT2D_pt_pt3 (&IN_obj[1].pc);

    // Abst. Mittelpunkte
    d4 = UT2D_len_2pt (&ci20.pc, &ci21.pc);

    if(UTP_comp_0(d4)) {
      TX_Error(" konzentrische Kreise ****\n");
      goto Error;
    }

    d1 = fabs(IN_obj[0].rad);
    d2 = fabs(IN_obj[1].rad);

    if(IN_ModIn == T_IN) {
      d3 = d1 + d2;
    } else {
      d3 = d1 - d2;
      if(UTP_comp_0(d3)) {

        // die Kreise sind gleich gross !
        // T_OUT: nur die Mittellinie parallelverschieben
        UT2D_pt_tranor2ptlen (&ptArr[0],  &ci20.pc, &ci21.pc, d1);
        UT2D_pt_tranor2ptlen (&ptArr[1],  &ci20.pc, &ci21.pc, -d1);
        UT2D_pt_tranor2ptlen (&ptArr1[0], &ci21.pc, &ci20.pc, -d2);
        UT2D_pt_tranor2ptlen (&ptArr1[1], &ci21.pc, &ci20.pc, d2);
        goto AC_AC_2;
      }
    }


    //UT2D_solvtriri_ac ( *q, *hc, b,  c)
    UT2D_solvtriri_ac   (&dx, &dy, d3, d4);

    //printf(" d1-d4=%f %f %f %f %f %f\n",d1,d2,d3,d4,dx,dy);

    // das geht für beide OUT:
    if(d3 < 0.0) {
      dx = -dx;
      dy = -dy;
    }

    // nun beide Radien in beide Richtungen abtragen
    UT2D_pt_tra2pt3len (&ptArr[0], &ci20.pc, &ci21.pc, dx, dy, d1);
    UT2D_pt_tra2pt3len (&ptArr[1], &ci20.pc, &ci21.pc, dx,-dy, d1);

    // das geht für beide IN:
    if(IN_ModIn == T_IN) {
      dx = -dx;
      dy = -dy;
    }

    UT2D_pt_tra2pt3len (&ptArr1[0],&ci21.pc, &ci20.pc,-dx,-dy, d2);
    UT2D_pt_tra2pt3len (&ptArr1[1],&ci21.pc, &ci20.pc,-dx, dy, d2);

    // vom Cir.pc in Richtung P1 d1 als dx und d2 als dy abtragen
    //UT2D_pt_tra2pt2len (&ptArr[0], &ci21.pc, &IN_obj[0].p1, d1, d2);


    AC_AC_2:

    // nun die Punkte mitteln
    UT2D_pt_mid2pt (&ptAux[0], &ptArr[0], &ptArr1[0]);
    UT2D_pt_mid2pt (&ptAux[1], &ptArr[1], &ptArr1[1]);

    // nun die Lösung entspr. ModTab suchen
    i1 = APT_solv_mod (ptAux, 2);

    pta = UT3D_pt_pt2 (&ptArr[i1]);

    pte = UT3D_pt_pt2 (&ptArr1[i1]);

    goto LL_exit;


  //-----------------------------------------------------------------
  // l1=p1,l1,a(0)              (durch p1 parallel zu l1 drehen um a)
  } else if ((IN_anz       ==   3)       &&
             (IN_obj[0].typ==Typ_PT)     &&
             (IN_obj[1].typ==Typ_LN)     &&
             (IN_obj[2].typ==Typ_Angle)) {

    // LN > VC
    UT3D_vc_2pt (&vc2, &IN_obj[1].p1, &IN_obj[1].p2);

    // VC drehen
    vc20.dx = vc2.dx;
    vc20.dy = vc2.dy;
    UT2D_vc_rotangr (&vc20, &vc20, UT_RADIANS (IN_obj[2].rad));
    vc2.dx = vc20.dx;
    vc2.dy = vc20.dy;

    // VC halbieren
    vc2.dx = vc2.dx / 2.0;
    vc2.dy = vc2.dy / 2.0;
    vc2.dz = vc2.dz / 2.0;

    pta.x = IN_obj[0].p1.x + vc2.dx;
    pta.y = IN_obj[0].p1.y + vc2.dy;
    pta.z = IN_obj[0].p1.z + vc2.dz;

    pte.x = IN_obj[0].p1.x - vc2.dx;
    pte.y = IN_obj[0].p1.y - vc2.dy;
    pte.z = IN_obj[0].p1.z - vc2.dz;

    goto LL_exit;


  //-----------------------------------------------------------------
  // l1=p1,A1,A(0)
  } else if((IN_anz       ==   2)        &&
            (IN_obj[0].typ==Typ_PT2)     &&
            (IN_obj[1].typ==Typ_CVPOL2)) {
    IN_obj[2].rad = 0.0;
    goto L_P_CV_Ang;


  //-----------------------------------------------------------------
  // l1=p1,A1,A(0)
  } else if((IN_anz       ==   3)        &&
            (IN_obj[0].typ==Typ_PT2)     &&
            (IN_obj[1].typ==Typ_CVPOL2)  &&
            (IN_obj[2].typ==Typ_Angle)) {

    L_P_CV_Ang:

    //TX_Print("L=P,CV,Ang");

    // Punkt auf die Curv proj. und Tangente an die Curve bilden
    UTCV2D_pt_projptcv (&pt20, &vc2, &i1, &i2, &IN_obj[0].p1, (int)IN_obj[1].rad);

    // pt21 = pt20 in Richtung cv2
    UT2D_pt_traptvclen (&pt21, &pt20, &vc2, APT_ln_len);

    // um den Winkel drehen
    UT2D_pt_rotptangr (&pta, &pt20, &pt21, UT_RADIANS (IN_obj[2].rad));

    // vis-vis Punkt
    UT2D_pt_opp2pt (&pte, &pt20, &pta);
    goto LL_exit;


  //-----------------------------------------------------------------
  } else {

    TX_Error(" Liniendefinition nicht implementiert\n");
    goto Error;
  }


    //-----------------------------------------------------------------
    // L = C D MOD angle  // Tangent to circ fixed direction from vec + rotAng
    if       (aus_typ[0] == Typ_CI)          &&
             (aus_typ[1] == Typ_VC)          &&
             (aus_typ[2] == Typ_modif)    &&
             (aus_typ[3] == Typ_Angle)))           {
  
    ci1 = DB_GetCirc  ((long)aus_tab[0]);
    vc1 = DB_GetVector ((long)aus_tab[1]);
    // modTab[0] = aus_tab[2];
          

    if(aus_anz == 4) {
      d1 = UT_RADIANS(aus_tab[3]);
      UT3D_vc_rotvcangr (&vc1, d1, &vc1, &ci1.vz);
    }

    // Normalvektor auf vc1/ci.vz; damit den Cir schneiden.
    UT3D_vc_perp2vc (&vc2, &vc1, &ci1.vz);

    // create 2 opposite Points aus Mittelpunkt, Vektor, Abstand.
    UT3D_2pt_oppptvclen (&pa[0], &pa[1],  &ci1.pc, &vc2, fabs(ci1.rad));


    // i2 = APT_solv3D_mod (pa, 2, modTab, 1);
    // pta = pa[i2];
    i1 = aus_tab[2];
    pt1 = pa[i1];

    // UT3D_pt_traptvclen (&pte, &pta, &vc1, APT_ln_len);
    // APT_subTyp = ATT_LN_RAY0;
    // UT3D_pt_traptvclen (&pte, &pta, &vc1, 1.);  // create ray; // 2014-01-07

    APT_modMax1 = 2;
    // goto LL_exit_c;
    goto LL_exit_p_v;

*/




//=====================================================================
  int APT_decode_inp (int aus_anz, int aus_typ[], double aus_tab[]) {
//=====================================================================
// Befuellt IN_anz  - Anzahl Obj. in IN_obj.
// Dahinein werden jeweils 2 Werte als Punkt abgelegt usw
// Einzelne Werte bleiben als Werte.
// In ObjG2[]  IN_obj[IN_anz] sind hinterher Typ und Werte abholbar.


  int              indi, i1, modanz;
  Point            pt1;
  Vector           vc1;
  Line             ln1;
  Circ             ci1;


  // printf("APT_decode_inp %d\n",aus_anz);


  //rc      =  0;

  indi      = -1;
  IN_anz    = -1;
  ModAnz    =  0;
  IN_Dreh   =  T_CCW;
  IN_ModIn  =  T_OUT;
  IN_Mod2In =  T_OUT;
  modanz    =  0;

  //-----------------------------------------------------------------

  NextObj:
  // printf(" next obj: %d=%d %d=%d %d=%d\n",indi,aus_typ[indi],
           // indi+1,aus_typ[indi+1],indi+2,aus_typ[indi+2]);


  ++indi;
  if(indi >= aus_anz) goto Fertig;


  //-----------------------------------
  if ((aus_typ[indi]  ==Typ_Val)   &&
      (aus_typ[indi+1]==Typ_Val)   &&
      (aus_typ[indi+2]==Typ_ZVal)) {
    ++IN_anz;
    IN_obj[IN_anz].typ = Typ_PT;
    IN_obj[IN_anz].p1.x = aus_tab[indi];
    ++indi;
    IN_obj[IN_anz].p1.y = aus_tab[indi];
    ++indi;
    IN_obj[IN_anz].p1.z = aus_tab[indi];
    goto NextObj;


  //-----------------------------------
  } else if ((aus_typ[indi]  ==Typ_Val)&&
             (aus_typ[indi+1]==Typ_Val)) {
    ++IN_anz;
    IN_obj[IN_anz].typ = Typ_PT;
    IN_obj[IN_anz].p1.x = aus_tab[indi];
    ++indi;
    IN_obj[IN_anz].p1.y = aus_tab[indi];
    IN_obj[IN_anz].p1.z = 0.0;
    goto NextObj;


  //-----------------------------------
  } else if(aus_typ[indi] == Typ_Val) {
    ++IN_anz;
    IN_obj[IN_anz].typ = Typ_Val;
    IN_obj[IN_anz].rad = aus_tab[indi];
    goto NextObj;



  //-----------------------------------
  } else if(aus_typ[indi] == Typ_XVal) {
    ++IN_anz;
    IN_obj[IN_anz].typ = Typ_XVal;
    IN_obj[IN_anz].rad = aus_tab[indi];
    goto NextObj;



  //-----------------------------------
  } else if(aus_typ[indi] == Typ_YVal) {
    ++IN_anz;
    IN_obj[IN_anz].typ = Typ_YVal;
    IN_obj[IN_anz].rad = aus_tab[indi];
    goto NextObj;



  //-----------------------------------
  } else if(aus_typ[indi] == Typ_ZVal) {
    ++IN_anz;
    IN_obj[IN_anz].typ = Typ_ZVal;
    IN_obj[IN_anz].rad = aus_tab[indi];
    goto NextObj;



  //-----------------------------------
  } else if(aus_typ[indi] == Typ_Angle) {
    ++IN_anz;
    IN_obj[IN_anz].typ = Typ_Angle;
    IN_obj[IN_anz].rad = aus_tab[indi];
    goto NextObj;



  //-----------------------------------
  } else if(aus_typ[indi] == Typ_modif) {
    i1 = (int)aus_tab[indi];

    if((i1 == T_CCW)||(i1 == T_CW)) {
      IN_Dreh = i1;

    } else if((i1 == T_IN)||(i1 == T_OUT)) {
      if(modanz == 0) {
        IN_ModIn = i1;
        ++modanz;
      } else {
        IN_Mod2In = i1;
      }

    } else if(ModAnz < 4) {
      ModTab[ModAnz] = i1;
      ++ModAnz;
    }
    goto NextObj;



  //-----------------------------------
  } else if(aus_typ[indi] == Typ_PT) {
    ++IN_anz;
    IN_obj[IN_anz].typ = Typ_PT;
    IN_obj[IN_anz].p1 = DB_GetPoint ((long)aus_tab[indi]);
    // IN_obj[IN_anz].p1  = UT2D_pt_pt3 (&pt1);
    // IN_obj[IN_anz].rad = pt1.z;
    goto NextObj;



  //-----------------------------------
  } else if(aus_typ[indi] == Typ_VC) {
    ++IN_anz;
    IN_obj[IN_anz].typ = Typ_VC;
    IN_obj[IN_anz].vz = DB_GetVector ((long)aus_tab[indi]);
    // IN_obj[IN_anz].p1.x = vc1.dx;
    // IN_obj[IN_anz].p1.y = vc1.dy;
    // IN_obj[IN_anz].rad  = vc1.dz;
    goto NextObj;



  //-----------------------------------
  } else if(aus_typ[indi] == Typ_LN) {
    ++IN_anz;
    IN_obj[IN_anz].typ = Typ_LN;
    ln1 = DB_GetLine ((long)aus_tab[indi]);
    IN_obj[IN_anz] = UT3D_obj_ln (&ln1);
    // IN_obj[IN_anz] = UT2D_obj_ln3 (&ln1);
    goto NextObj;



  //-----------------------------------
  } else if(aus_typ[indi] == Typ_CI) {
    ++IN_anz;
    IN_obj[IN_anz].typ = Typ_CI;
    ci1 = DB_GetCirc ((long)aus_tab[indi]);
    IN_obj[IN_anz] = UT3D_obj_ci (&ci1);
    // IN_obj[IN_anz] = UT2D_obj_ci3 (&ci1);
    goto NextObj;



  //-----------------------------------
  } else if(aus_typ[indi] == Typ_CV) {
    ++IN_anz;
    IN_obj[IN_anz].typ = Typ_CV;
    // printf("XXXXXXXXXX DB_GetCurv XXXXXXXXX\n");
    // cv1 = DB_GetCurv ((long)aus_tab[indi]);
    // IN_obj[IN_anz] = UT2D_obj_cv3 (&cv1);
    goto NextObj;



  //-----------------------------------
  } else if(aus_typ[indi] == Typ_PLN) {
    ++IN_anz;
    IN_obj[IN_anz].typ = Typ_PLN;
    IN_obj[IN_anz].ID  = (long)aus_tab[indi];  // DB-Index
    // printf("Refsys %f\n",aus_tab[indi]);
    goto NextObj;




  //-----------------------------------
  // } else if(aus_typ[indi] == Typ_Txt) {
    // printf("Text\n");



  //-----------------------------------
  } else if(aus_typ[indi] == Typ_String) {
    ++IN_anz;
    IN_obj[IN_anz].typ = Typ_String;
    IN_obj[IN_anz].rad = aus_tab[indi];
    // printf("String %f\n",aus_tab[indi]);





  //-----------------------------------
  } else if(aus_typ[indi] == Typ_StrDel) {     // Fortsetzungszeile ignorieren
    goto NextObj;



  } else {
    goto ParErr;

  }



  Fertig:;
  ++IN_anz;



/*
  // nur Testausgaben:
  printf("ex APT_decode_inp  IN_anz=%d\n",IN_anz);
  for(i1=0; i1<IN_anz; ++i1) {
    // TX_Print("  %d %d %f,%f   %f,%f",i1,IN_obj[i1].typ,
    printf("  %d %d %f,%f,%f   %f,%f,%f  %f\n",i1,IN_obj[i1].typ,
      IN_obj[i1].p1.x,IN_obj[i1].p1.y,IN_obj[i1].p1.z,
      IN_obj[i1].p2.x,IN_obj[i1].p2.y,IN_obj[i1].p2.z,
      IN_obj[i1].rad);
  }
*/



  return 0;



  //================= Fehler =============================
  ParErr:
  TX_Error(" APT_decode_inp: Parametertyp %d ****",aus_typ[indi]);
  return -1;

}


//=============================================================================
  int APT_decode_cttt (Circ *ciO, int aus_anz, int aus_typ[], double aus_tab[],
                       int Dreh, int iMod) {
//=============================================================================
// decode circle from tangent, tangent, tangent
// tangent can be point, line or circ.

  int        irc, i1, oNr;
  Plane      pl1;
  Point      pth;
  Mat_4x3    m1, mi1;
  ObjGX      ox1, oa1[3], oa2[3], oa3[8];



  // printf("\nAPT_decode_cttt %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  if((aus_anz < 3)||(aus_anz > 4)) goto Par_err;


  if(
     ((aus_typ[0] == Typ_PT)||(aus_typ[0] == Typ_LN)||(aus_typ[0] == Typ_CI)) &&
     ((aus_typ[1] == Typ_PT)||(aus_typ[1] == Typ_LN)||(aus_typ[1] == Typ_CI)) &&
     ((aus_typ[2] == Typ_PT)||(aus_typ[2] == Typ_LN)||(aus_typ[2] == Typ_CI))) 
    goto L_1;
  goto NotImp_err;  // inputObjerr


  L_1:
     // die 3 Objekte aus der DB holen
     oa1[0] = DB_GetObjGX (aus_typ[0], (long)aus_tab[0]);
     oa1[1] = DB_GetObjGX (aus_typ[1], (long)aus_tab[1]);
     oa1[2] = DB_GetObjGX (aus_typ[2], (long)aus_tab[2]);
        // for(i1=0;i1<3;++i1) DEB_dump_obj__ (oa1[i1].form,oa1[i1].data,"");

  //----------------------------------------------------------------
  // die 3 3D-Objekte in 2D-Objekte umwandeln und nach oa2 kopieren

  // die Transformationsmatrix zu den 3 Objekten suchen ..
  UT3D_pl_nobj (&pl1, 3, oa1);       // get plane from 3 objects
  UT3D_m3_loadpl (m1, &pl1);         // get trMat from plane
  UT3D_m3_invm3 (mi1, m1);           // invert trMat
  UTRA_def__ (1, Typ_M4x3, mi1);   // load trMat

  // die 3 Objekte transformieren (in die absolute X-Y-Ebene)
  for(i1=0; i1<3; ++i1) {
    // transform obj
    UTRA_nobj_tra (&ox1, 1, &oa1[i1], &APTSpcObj, &APTSpcTmp);
      // DEB_dump_ox_0 (&ox1, "ox1:");

    // in 2D-Objekte umwandeln (remove Z-Wert)
    UT2D_obj_obj3 (&oa2[i1], &ox1, &APTSpcTmp);
  }

 // for (i1=0; i1<nr_moeb; ++i1)
   // DEB_dump_obj__ (oxT[i1].form, oxT[i1].data, "");



  //----------------------------------------------------------------
  // die Beruehrungskreise errechnen
  irc = UCV_Touch3MoebCirs (&oNr, oa3, &APTSpcObj, oa2, &APTSpcTmp);
    // printf(" oNr=%d\n",oNr);
  if (irc < 0) goto Geom_err;



  //----------------------------------------------------------------
//   // die gefundenen Beruehrungskreise filtern;
//   UCV_FilterTouch3MCirs (&oNr, oa3, &APTSpcTmp, oa2, oNr, oa3);
//     printf(" oNr/2=%d\n",oNr);

  // modfifier given or automatic
  // if(aus_typ[3] == Typ_modif) {
    // i1 = aus_tab[3];
    if(iMod >= oNr) iMod = oNr - 1;
  // } else {  // no modfifier; 
    // // den Mittelpunkt der EingabeElemente suchen;
    // irc = UTO_pt_midnobj (&pth, oa2, 3);
    // // den Kreis, dessen Mittelpunkt pt21 am naechsten ist, auswaehlen
    // // (als erstes obj in die tabelle)
    // iMod = UTO_ck_nearptnobj (oa3, oNr, &pth);
  // }

  oa3[0] = oa3[iMod];



  //----------------------------------------------------------------
  // 2D-Objekte retour nach 3D.
  // rueckwaertstransformation laden
  UTRA_def__ (1, Typ_M4x3, m1);

  // den ersten Beruehrungskreise nach 3D zurueckTransformieren
  // 2D-Objekte in 3D-Objekte umwandeln (add Z=0)
  UT3D_obj_obj2 (&ox1, &oa3[0], &APTSpcObj);
  // transformieren
  UTRA_nobj_tra (&oa3[0], 1, &ox1, &APTSpcObj, &APTSpcTmp);

    // TestDisplay  (UCV_DispMoebCir)
    // UTO_obj_Disp__ (&oa3[0], NULL, 9);

  *ciO = *((Circ*)oa3[0].data);

  APT_modMax1 = oNr;

  return 0;



  Par_err:
  TX_Error(" with parameter - Circ-definition ****");
  return -1;

  Geom_err:
  TX_Error(" geometry (no solution) - Circ-definition ****");
  return -1;

  NotImp_err:
  TX_Error("Circ-definition: NOT YET IMPLEMENTED ****");
  return -1;



}
 //=============================================================================
  int APT_decode_ctc (Circ *ciO, int aus_anz, int aus_typ[], double aus_tab[]) {
//=============================================================================
// Circ = P  C [mod]
    
  int        iMod;
  double     rdc;
  // ObjGX      ox1, oa1[2], oa2[4];
  // Point      pth;
  Circ       ci1;
      


  ciO->pc = DB_GetPoint ((long)aus_tab[0]);
  ci1 = DB_GetCirc ((long)aus_tab[1]);

  // modfifier given or automatic
  if((aus_anz > 2) && (aus_typ[2] == Typ_modif)) {
    iMod = aus_tab[2];
  } else {
    iMod = 0;
  }

  rdc = fabs(ci1.rad);
  if(iMod == 1) rdc *= -1.;
    // printf(" iMod=%d rdc = %f\n",iMod,rdc);

  
  UT3D_pt_traptptlen (&ciO->p1, &ci1.pc, &ciO->pc, rdc);
  ciO->p2   = ciO->p1;
  ciO->vz   = ci1.vz;
  ciO->rad  = UT3D_len_2pt (&ciO->pc, &ciO->p1);
  ciO->ango = RAD_360;

    // DEB_dump_obj__ (Typ_CI, ciO, "ex APT_decode_ctc:\n");

  return 0;

}


//=============================================================================
  int APT_decode_cttr (Circ *ciO, int aus_anz, int aus_typ[], double aus_tab[],
                       int Dreh, int iMod) {
//=============================================================================
// Circ = Tang Tang Radius
//   iMod    0-7  (0=first,1=second !!)

  int        i1, oNr;
  double     rdc;
  ObjGX      ox1, oa1[2], oa2[4];
  Point      pth;
  Circ       cia[8];


  // printf("\nAPT_decode_cttr aus_anz=%d iMod=%d\n",aus_anz,iMod);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  if((aus_anz < 3)||(aus_anz > 4)) goto Par_err;


  if(
     ((aus_typ[0] == Typ_PT)||(aus_typ[0] == Typ_LN)||(aus_typ[0] == Typ_CI)) &&
     ((aus_typ[1] == Typ_PT)||(aus_typ[1] == Typ_LN)||(aus_typ[1] == Typ_CI)) &&
     (aus_typ[2] == Typ_Val))
    goto L_1;
  goto NotImp_err;  // inputObjerr


  L_1:
    // printf("APT_decode_cttr L1: iMod=%d\n",iMod);


  // die 2 Objekte aus der DB holen
  oa1[0] = DB_GetObjGX (aus_typ[0], (long)aus_tab[0]);
  oa1[1] = DB_GetObjGX (aus_typ[1], (long)aus_tab[1]);
  rdc = aus_tab[2];
    // for(i1=0;i1<2;++i1) DEB_dump_obj__ (oa1[i1].form,oa1[i1].data,"");


  // in Reihenfolge P-L-C bringen
  // if((oa1[1].form == Typ_PT)||(oa1[0].form == Typ_CI))  do NOT swap same types
  if(oa1[1].form < oa1[0].form)
    MEM_swap__ (&oa1[0], &oa1[1], sizeof(ObjGX));

    // DEB_dump_obj__(Typ_ObjGX, &oa1[0], "  _cttr-oa1[0]\n");
    // DEB_dump_obj__(Typ_ObjGX, &oa1[1], "  _cttr-oa1[1]\n");


  // Circs berechnen
  //----------------------------------------------------------------
  if(oa1[0].form == Typ_PT)  {

    if(oa1[1].form == Typ_PT)  {
      // PT-PT
      oNr = UT3D_ci_2ptvcrd (cia, oa1[0].data, oa1[1].data, &WC_sur_act.vz, rdc);


    } else if (oa1[1].form == Typ_LN) {
      // PT-LN
      oNr = UT3D_ci_lnptrd (cia, oa1[1].data, oa1[0].data, &WC_sur_act.vz, rdc);


    } else if (oa1[1].form == Typ_CI) {
      // PT-AC
      oNr = UT3D_ci_ciptrd (cia, oa1[1].data, oa1[0].data, rdc);
      // kein MOD: Loesung-1 (aussen)
      if(iMod < 0) iMod = 0;
    }


  //----------------------------------------------------------------
  } else if (oa1[0].form == Typ_LN) {


    if (oa1[1].form == Typ_LN) {
      // LN-LN
      // liefert nur einen Circ, nicht alle 4; daher keine Auswahl moeglich ..
      if(iMod < 0) iMod = 0;
      oNr = UT3D_ci_2lnrd (cia, oa1[0].data, oa1[1].data, rdc, iMod);
      iMod = 0;


    } else if (oa1[1].form == Typ_CI) {
      // LN-AC
      if(iMod < 0) iMod = 0;
      oNr = UT3D_ci_lncird (cia, oa1[0].data, oa1[1].data, rdc, iMod);
      iMod = 0;

    }



  //----------------------------------------------------------------
  } else if (oa1[0].form == Typ_CI) {
    // AC-AC
    if(iMod < 0) iMod = 0;
    oNr = UT3D_ci_cicird (cia, oa1[0].data, oa1[1].data, rdc, iMod);
    iMod = 0;

  }


  if(oNr < 0) goto Par_err;
  if(oNr < 1) goto Geom_err;

  
  APT_modMax1 = oNr;
    // printf(" APT_modMax1=%d\n",APT_modMax1);

  
  // modfifier given or automatic
  if(iMod >= 0) {
    if(iMod >= oNr) iMod = oNr - 1;


  } else {  // no modfifier;
    // den Mittelpunkt der EingabeElemente suchen;
    UTO_pt_midnobj (&pth, oa1, 2);

    // den Kreis, dessen Mittelpunkt pt21 am naechsten ist, auswaehlen
    for(i1=0; i1<oNr; ++i1) {
      OGX_SET_OBJ (&oa2[i1], Typ_CI, Typ_CI, 1, &cia[i1]);
    }
    iMod = UTO_ck_nearptnobj (oa2, oNr, &pth);
  }



  L_99:
  *ciO = cia[iMod];



  return 0;


  Par_err:
  TX_Error(" with parameter - Circ-definition ****");
  return -1;

  Geom_err:
  TX_Error(" geometry (no solution) - Circ-definition ****");
  return -1;

  NotImp_err:
  TX_Error("Circ-definition: NOT YET IMPLEMENTED ****");
  return -1;

}


//==============================================================================
  int APT_decode_ci (Circ *ciO, int aus_anz, int aus_typ[], double aus_tab[]) {
//==============================================================================
// TODO:
// CenterPoint, Tangent an B-spline:
//   offsetcurves an die Inputelemente; diese schneiden ...


  int              irc, i1, i2, ii, rc, oTyp, Dreh, iMod, iRev,
                   modTab[2], modFlag, inxt;
  double           d1, d2, d3;
  Circ             ci1, ci2, ci3d, ca[8];
  Circ2            ci20;
  // Line             ln1, ln2, ln3;
  Line2            ln20, ln21;
  Line             ln1, ln2;
  Point            *pp1, *pp2, *pp3, pt1, pt2, pa[4];
  Point2           pt20, pta[4];
  Vector           *vz, *pv1, vc1, vc2;
  Vector2          vc20, vc21;
  // Point            pt1;
  ObjGX            *ox1p, oxo;
  void             *o1, *o2;
  char             obj1[OBJ_SIZ_MAX];




  // printf("\nAPT_decode_ci %d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }



  rc     =  0;             // Defaultwert RetCode

  ci3d.ango = RAD_360;     // Defaultwert = Vollkreis
  ci3d.vz   = WC_sur_act.vz;

  Dreh      = 1;           // Default CCW
  iMod      = 0;           // Default Version
  iRev      = 0;           // Default; do not revert
  APT_modMax1 = 4;


  // get iMod from last parameter [Version]
  ii = aus_anz - 1;
  if(aus_typ[ii] == Typ_modif) {
    iMod = aus_tab[ii];
    if(iMod) --iMod;  // make 0=0; 1=0; 2=1; 3=2; ..
    --ii;
    --aus_anz;
  }

  // get Dreh from last parameter [CW/CCW]
  if(aus_typ[ii] == Typ_cmdNCsub) {
    i1 = aus_tab[ii];
    if(i1 == T_REV) {
      iRev = 1;
    } else if(i1 == T_CW) {
      Dreh = -1;
    } else if(i1 == T_CCW){
      Dreh = 1;
    } else goto NotImp_err;
    --ii;
    --aus_anz;
  }



  // removed: CW|CCW   MOD


  // Bearbeitungsreihenfolge:
  // zuerst alle C=ARC,
  // dann alle C=ARC1
  // dann alle uebrigen.
    // for(i1=0; i1<aus_anz; ++i1)
      // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
    // printf(" Dreh=%d iMod=%d\n",Dreh,iMod);




  //==================================================================
  // work ARC and ARC1
  if(aus_typ[0] == Typ_cmdNCsub)            {


  //==================================================================
  if((aus_tab[0] == T_ARC))            {
  //==================================================================


    //================================================================
    if (aus_anz == 5)   {

      //-----------------------------------------------------------------
      // C = ARC PT VC Radius [Angle]  [Z-Axis-Vector] [CW/CCW]
      if        ((aus_typ[1] == Typ_PT)    &&
                 (aus_typ[2] == Typ_VC)    &&
                 (aus_typ[3] == Typ_Val))  {
        goto L_ARC_pt_vc_rad;
      }


    }


    //----------------------------------------------------------------
    // C = ARC PTs PTe Radius [Z-axis] [CW|CCW] [Version] 
    //   Version 1 and Default = openingAngle < 180 degree
    //   Version 2 = openingAngle > 180 degree

    if((aus_typ[1] == Typ_PT)        &&
       (aus_typ[2] == Typ_PT)        &&
       (aus_typ[3] == Typ_Val))           {
 
       APT_modMax1 = 2;

      // printf(" C=ARC A E r\n");
      pp1 = DB_get_PT ((long)aus_tab[1]);  // pa
      pp2 = DB_get_PT ((long)aus_tab[2]);  // pe
        // GR_tDyn_symB__ (pp1, SYM_TRI_S, 2);
        // GR_tDyn_symB__ (pp2, SYM_TRI_S, 2);
      ci3d.rad = fabs(aus_tab[3]);
      i1 = 4;

      if(aus_typ[i1] == Typ_VC) {
        ci3d.vz = DB_GetVector ((long)aus_tab[i1]);
        ++i1;
      }

      // i1 = UT3D_ci_2ptvcrd (ca, pp1, pp2, &ci3d.vz, ci3d.rad); // i1=circNr
        // printf(" ex ci_2ptvcrd %d\n",i1);
      // i2 = IMIN (iMod-1,i1);
      // ci3d = ca[iMod];
      i1 = UT3D_ci_ptptvcrd (&ci3d, pp1, pp2, ci3d.rad, &ci3d.vz, Dreh, iMod);
      if(i1 < 0) goto Geom_err;
      goto Fertig3D;
    }


    //----------------------------------------------------------------
    // C = ARC PT VC Radius [Angle]  [Z-Axis-Vector] [CW/CCW]
    if((aus_typ[1] == Typ_PT)        &&
       (aus_typ[2] == Typ_VC)        &&
       (aus_typ[3] == Typ_Val))           {
      goto L_ARC_pt_vc_rad;
    }


    //----------------------------------------------------------------
    // C = ARC PTcen ANG1 ANG2 Radius [Dreh]
    if((aus_typ[1] == Typ_PT)           &&
       (aus_typ[2] == Typ_Angle)        &&
       (aus_typ[3] == Typ_Angle)        &&
       (aus_typ[4] == Typ_Val))           {         // Rad

      pp1 = DB_get_PT ((long)aus_tab[1]);           // pc
      d1 = fabs(aus_tab[4]);                        // rad.
      d2 = UT_RADIANS(aus_tab[2]);                  // ang1
      d3 = UT_RADIANS(aus_tab[3]);                  // ang2
        // printf(" rad=%f ang1=%f ang2=%f\n",d1,d2,d3);

      // circ from center, Radius, Z-Axis, X-Axis, startAngle, endAngle
      UT3D_ci_ptrd2vc2angr (&ci3d, pp1, d1,
                            &WC_sur_act.vz, &WC_sur_act.vx,
                            d2, d3);

      goto Fertig3D;

    }


    //-----------------------------------------------------------------
    // C= ARC P P ANG() [DZ]
    if        ((aus_typ[1] == Typ_PT)    &&
               (aus_typ[2] == Typ_PT)    &&
               (aus_typ[3] == Typ_Angle))     {


      pp1 = DB_get_PT ((long)aus_tab[1]);        // pc
      ci3d.p1 = DB_GetPoint ((long)aus_tab[2]);  // p1
      ci3d.ango = UT_RADIANS(aus_tab[3]);

      if(aus_anz == 5) {
        ci3d.vz = DB_GetVector ((long)aus_tab[4]);
      } else {
        ci3d.vz = WC_sur_act.vz;
      }
        // DEB_dump_obj__ (Typ_CI, &ci3d.vz, " ci-vz\n");

      // pc = proj p1 --> achse pc - vz
      UT3D_pt_projptptvc (&ci3d.pc, &ci3d.rad, NULL, &ci3d.p1, pp1, &ci3d.vz);
      // p2 = rotate p1
      UT3D_pt_rotptptvcangr (&ci3d.p2, &ci3d.p1, &ci3d.pc, &ci3d.vz, ci3d.ango);
      if(ci3d.ango < 0.) ci3d.rad *= -1.;
        // DEB_dump_obj__ (Typ_CI, &ci3d, " ci-ARC P P ANG");
      goto Fertig3D;

    }


    //----------------------------------------------------------------
    // C = ARC Pa Pe Pc [Vec]
    //      0  1  2  3   4  
    if((aus_typ[1] != Typ_PT)        ||
       (aus_typ[2] != Typ_PT)        ||
       (aus_typ[3] != Typ_PT))           {
      goto Par_err;
    }
        // printf(" C = ARC Pa Pe Pc\n");
      ci3d.p1 = DB_GetPoint ((long)aus_tab[1]);
      ci3d.p2 = DB_GetPoint ((long)aus_tab[2]);
      ci3d.pc = DB_GetPoint ((long)aus_tab[3]);

      // check for VEC
      ii = 4;
      if(aus_anz > ii)  {
       if(aus_typ[ii] == Typ_VC) {
          ci3d.vz = DB_GetVector ((long)aus_tab[4]);
          // fix centerpoint (project p1 -> pc-vz
          UT3D_pt_projptptvc (&ci3d.pc, &d1, NULL, &ci3d.p1, &ci3d.pc, &ci3d.vz);
        }
      }

      // rad
      ci3d.rad = UT3D_len_2pt (&ci3d.p1, &ci3d.pc);
        // printf(" Dreh=%d rad=%f\n",Dreh,ci3d.rad);


    // wenn Z-Val von allen 3 Punkten nicht gleich, den Z-Vektor errechen
    if(aus_typ[4] == Typ_VC) goto L_APPP_a;

      d1 = UT3D_skp_2ptvc (&ci3d.p1, &ci3d.pc, &ci3d.vz);
        // printf(" d1p1,pc=%f\n",d1);
      if(d1 < UT_TOL_pt) goto L_APPP_a;
      // d1 = UT3D_skp_vc2pt (&ci3d.vz, &ci3d.p2, &ci3d.pc);
      // printf(" d1p2,pc=%f\n",d1);
      // printf(" fix z-vec\n");
      // compute Z-vec of 3 points
      UT3D_vc_2pt (&vc1, &ci3d.pc, &ci3d.p1);
      UT3D_vc_2pt (&vc2, &ci3d.pc, &ci3d.p2);
      UT3D_vc_perp2vc (&ci3d.vz, &vc1, &vc2);
      // DEB_dump_obj__ (Typ_VC, &ci3d.vz, "  vcz =");


    // openingAngle errechen              2005-09-16 zu.RF.
    L_APPP_a:
    if(Dreh < 0) {
      ci3d.rad  = -fabs(ci3d.rad);
      ci3d.ango = -fabs(ci3d.ango);
    }

    if(UT3D_comp2pt(&ci3d.p1,&ci3d.p2,UT_TOL_pt) != 0) {
      ci3d.ango = RAD_360;
    } else {
      ci3d.ango = UT3D_angr_ci_p1_pt (&ci3d, &ci3d.p2);
      // DEB_dump_obj__ (Typ_CI, &ci3d, "  ci3d =");
    }
      // DEB_dump_obj__ (Typ_CI, &ci3d, "  circ=");
    goto Fertig1;




  //==================================================================
  } else if((aus_tab[0] == T_ARC1))            {
  //==================================================================


    //================================================================
    if (aus_anz == 4)   {

      //----------------------------------------------------------------
      // C = ARC1 PT PT PT [CW/CCW]
      if      ((aus_typ[1] == Typ_PT)    &&
               (aus_typ[2] == Typ_PT)    &&
               (aus_typ[3] == Typ_PT))             {

        pp1  = DB_get_PT ((long)aus_tab[1]);      // startPt
        pp2  = DB_get_PT ((long)aus_tab[2]);      // midPt
        pp3  = DB_get_PT ((long)aus_tab[3]);      // endPt

        // make 360-deg-circ from points
        i1 = UT3D_ci_3pt (&ci3d, pp1, pp2, pp3);
        if(i1 < 0) goto Geom_err;

        // set pp3 endpoint
        UT3D_ci_cip2 (&ci3d, pp3);

        goto Fertig3D;

      }

    //================================================================
    } else if (aus_anz == 5)   {

/*
      //----------------------------------------------------------------
      // C = ARC1 P P P CW                          // C = ARC 3 points
      if        ((aus_typ[1] == Typ_PT)    &&
                 (aus_typ[2] == Typ_PT)    &&
                 (aus_typ[3] == Typ_PT)   &&
                 (aus_typ[4] == Typ_cmdNCsub))      { // Typ_modCW|Typ_modCCW

        pp1  = DB_get_PT ((long)aus_tab[1]);
        pp2  = DB_get_PT ((long)aus_tab[2]);
        pp3  = DB_get_PT ((long)aus_tab[3]);

        // circ from 3 points
        i1 = UT3D_ci_3pt (&ci3d, pp1, pp2, pp3);
        if(i1 < 0) goto Geom_err;

        UT3D_ci_cip2 (&ci3d, pp3);
        // Dreh = aus_tab[4];
        // if(Dreh == Typ_modCW) UT3D_ci_inv2 (&ci3d);
        if(aus_tab[4] == T_CW) UT3D_ci_inv1 (&ci3d);

        goto Fertig1;
      }
*/

      goto NotImp_err;
    }



  } // end (aus_tab[0] == T_ARC1)
  } // end (aus_typ[0] == Typ_cmdNCsub)


  //==================================================================
  // Circ ohne ARC/ARC1
  //==================================================================
  if (aus_anz == 1)     {


    //---------------------------------------------
    // C = C [REV]
    if (aus_typ[0] == Typ_CI)           {
      ci3d = DB_GetCirc  ((long)aus_tab[0]);
      if(iRev) UT3D_ci_inv1 (&ci3d);
      goto Fertig3D;


    //-----------------------------------------------------------------
    // C = S MOD(<segNr>)                         // copy Circ out of CCV
    } else if(aus_typ[0] == Typ_CV)     {

      ox1p = DB_GetCurv  ((long)aus_tab[0]);
      // i1 = aus_tab[1];

      if(ox1p->typ == Typ_CVTRM) {
        // rc = UT3D_stru_segccv (ciO, Typ_CI, i1, ox1p);
        rc = UTO_obj__ccv_segnr (&i2, obj1, iMod, ox1p);
        if(rc < 0) goto Par_err;
        if(i2 != Typ_CI) goto Geom_err;
        *ciO = *(Circ*)obj1;
        goto L_exit9;


      } else goto NotImp_err;


    //---------------------------------------------
    } else goto NotImp_err;



  //==================================================================
  } else if (aus_anz == 2)   {
  //==================================================================


    //---------------------------------------------
    // C = P .
    if(aus_typ[0] == Typ_PT)  {
      pp1 = DB_get_PT ((long)aus_tab[0]);


      //---------------------------------------------
      // C = P radius                   // p1=center, radius.
      if (aus_typ[1] == Typ_Val)             {
  
        ci3d.pc  = *pp1;
        ci3d.rad = aus_tab[1];
        ci3d.vz  = WC_sur_act.vz;

        // Vektor aus DX und Radius
        UT3D_vc_setLength (&vc1, &WC_sur_act.vx, ci3d.rad);
        UT3D_pt_traptvc (&ci3d.p1, &ci3d.pc, &vc1);
        ci3d.p2  = ci3d.p1;
        goto Fertig3D;


      //---------------------------------------------
      // C = P P [CW|CCW]            (p1=center, p2 = point am Umfang)
      } else if (aus_typ[1] == Typ_PT)             {

        ci3d.pc = *pp1;
        ci3d.p1 = DB_GetPoint ((long)aus_tab[1]);
        ci3d.p2 = ci3d.p1;
        ci3d.rad = UT3D_len_2pt (&ci3d.p1, &ci3d.pc);
        goto Fertig3D; // update Dreh


      //---------------------------------------------
      // C = P L                     (p1=center, tangential an l1)
      } else if (aus_typ[1] == Typ_LN)             {

        APT_modMax1 = -2;

        ci3d.pc = *pp1;
        ln1     = DB_GetLine ((long)aus_tab[1]);
        i1 = UT3D_pt_projptln (&ci3d.p1, &ci3d.rad, NULL, &ci3d.pc, &ln1);
        if(ci3d.rad < UT_TOL_pt) return -1;
        ci3d.p2 = ci3d.p1;
        UT3D_vc_ln (&vc1, &ln1);
        UT3D_vc_2pt (&vc2, &ci3d.p1, &ci3d.pc);
        UT3D_vc_perp2vc (&ci3d.vz, &vc1, &vc2);
        goto Fertig3D;


      //-----------------------------------------------------------------
      // C = P C                     (p1=center, tangential an CIR)
      } else if (aus_typ[1] == Typ_CI)             {
        // return APT_decode_ctc (ciO, aus_anz, aus_typ, aus_tab);
        ci3d.pc = *pp1;
        ci1 = DB_GetCirc ((long)aus_tab[1]);
        UT3D_pt_traptptlen (&ci3d.p1, &ci1.pc, &ci3d.pc, fabs(ci1.rad));
        ci3d.p2 = ci3d.p1;
        ci3d.vz = ci1.vz;
        ci3d.rad = UT3D_len_2pt (&ci3d.pc, &ci3d.p1);
        goto Fertig3D;


      //-----------------------------------------------------------------
      // C = P S                     (p1=center, tangential to curve
      } else if (aus_typ[1] == Typ_CV)             {
        ci3d.pc = *pp1;
          // DEB_dump_obj__(Typ_PT, &ci3d.pc, "pt:");
        i1 = UTO__dbo (&o1, &i2, Typ_CV, (long)aus_tab[1]);
          // DEB_dump_obj__(i1, oDat, "curv:");
        // prj point -> curve
        UPRJ_def__ (i1, o1, 1, NULL);
        ii = sizeof(memspc011);
        o2 = memspc011;
        APT_prim_seg = iMod;  // MOD() !
        // rc = UPRJ_app__ (o2, &i2, &ii, 1, &ci3d.pc, Typ_PT, &APTSpcTmp);
        rc = UPRJ_app__ (&oxo, 1, &ci3d.pc, Typ_PT, &APTSpcTmp);
        if(rc < 0) goto Par_err;
          // DEB_dump_ox_0(&oxo, "prjPt %d:",rc);
        // circ from center & p1
        ci3d.p1 = *(Point*)oxo.data;
        ci3d.p2 = ci3d.p1;
        ci3d.vz = WC_sur_act.vz; // TODO: vector from curve !
        ci3d.rad = UT3D_len_2pt (&ci3d.pc, &ci3d.p1);
        goto Fertig3D;


      //-----------------------------------------------------------------
      } else goto NotImp_err;



    //================================================================
    // C = L .
    } else if (aus_typ[0] == Typ_LN)     {
      ln1 = DB_GetLine ((long)aus_tab[0]);

      //-----------------------------------------------------------------
      // C = C P                          // centerline point-on-circ
      if (aus_typ[1] == Typ_PT)    {
        ci3d.p1 = DB_GetPoint ((long)aus_tab[1]);
        ci3d.p2 = ci3d.p1;
        UT3D_pt_projptln (&ci3d.pc, &d1, NULL, &ci3d.p1, &ln1);  // project pt onto line
        UT3D_vcn_len__ln (&ci3d.vz, NULL, &ln1);
        ci3d.rad = d1;
        ci3d.ango = RAD_360;
        goto Fertig3D;


      //-----------------------------------------------------------------
      } else goto NotImp_err;



    //================================================================
    // C = C .
    } else if (aus_typ[0] == Typ_CI)     {
      ci3d = DB_GetCirc ((long)aus_tab[0]);

      //-----------------------------------------------------------------
      // C = C radius                           (Konzentr. Kreis)
      if (aus_typ[1] == Typ_Val)    {
        // eventuell noch die Drehrichtung des alten Kreise kopieren erforderlich !
        d1 = fabs(ci3d.rad) - fabs(aus_tab[1]);
        UT3D_pt_traptptlen(&ci3d.p1, &ci3d.pc, &ci3d.p1, d1);
        UT3D_pt_traptptlen(&ci3d.p2, &ci3d.pc, &ci3d.p2, d1);
        ci3d.rad = aus_tab[1] * Dreh;
        goto Fertig3D;


      //-----------------------------------------------------------------
      } else goto NotImp_err;


    //================================================================
    // C = S .
    } else if (aus_typ[0] == Typ_CV)     {
      // get bin.obj of curv


      //-----------------------------------------------------------------
      // C = S SEG                     // copy trimmed-circ
      if (aus_typ[1] == Typ_SEG)    {
        i1 = aus_tab[1];  // segmentNr; first=1
        i2 = 0;
        irc = UTO_objDat_dbS (&o1, &oTyp, (long)aus_tab[0], i1, i2, obj1);
        if(irc < 0) goto Geom_err;
        if(oTyp == Typ_CI) {
          ci3d = *((Circ*)o1); 
          goto Fertig3D;
        } else goto NotImp_err;

      //-----------------------------------------------------------------
      } else goto NotImp_err;


    //-----------------------------------------------------------------
    } else goto NotImp_err;



  //==================================================================
  } else if (aus_anz == 3)   {
  //==================================================================


    //-----------------------------------------------------------------
    // C = P Radius Z-Achse             // p1=center, radius.
    if        ((aus_typ[0] == Typ_PT)    &&
               (aus_typ[1] == Typ_Val)   &&
               (aus_typ[2] == Typ_VC))             {

      ci3d.pc  = DB_GetPoint ((long)aus_tab[0]);
      ci3d.vz  = DB_GetVector ((long)aus_tab[2]);
      ci3d.rad = aus_tab[1];


      // Z-Achse normieren
      UT3D_vc_normalize (&ci3d.vz, &ci3d.vz);

      // zu Drehachse eine X-Achse generieren
      UT3D_vc_perpvcplXY (&vc1, &ci3d.vz);

      UT3D_pt_traptvclen (&ci3d.p1, &ci3d.pc, &vc1, ci3d.rad);

      ci3d.p2   = ci3d.p1;

      goto Fertig3D;


    //-----------------------------------------------------------------
    // P = PT-Cen Z-Vector PT-Umfang                   // P D P
    } else if ((aus_typ[0] == Typ_PT)    &&
               (aus_typ[1] == Typ_VC)    &&
               (aus_typ[2] == Typ_PT))             {

      ci3d.pc = DB_GetPoint ((long)aus_tab[0]);
      ci3d.vz = DB_GetVector ((long)aus_tab[1]);
      ci3d.p1 = DB_GetPoint ((long)aus_tab[2]);
      ci3d.p2 = ci3d.p1;
      ci3d.rad = UT3D_len_2pt (&ci3d.p1, &ci3d.pc);

      goto Fertig3D;



    //-----------------------------------------------------------------
    // C = P P P
    } else if ((aus_typ[0] == Typ_PT)    &&
               (aus_typ[1] == Typ_PT)    &&
               (aus_typ[2] == Typ_PT))             {

      pp1  = DB_get_PT ((long)aus_tab[0]);
      pp2  = DB_get_PT ((long)aus_tab[1]);
      pp3  = DB_get_PT ((long)aus_tab[2]);

      UT3D_ci_3pt (&ci3d, pp1, pp2, pp3);

      goto Fertig3D;




    //-----------------------------------------------------------------
    //C = P P D                   // CI  =  CenterPT   RadiusPT   Z-Vektor
    } else if ((aus_typ[0] == Typ_PT)    &&
               (aus_typ[1] == Typ_PT)    &&
               (aus_typ[2] == Typ_VC))      {


      ci3d.pc = DB_GetPoint ((long)aus_tab[0]);     // centerpoint
      ci3d.vz = DB_GetVector ((long)aus_tab[2]);    // Z-vec
      pp1 = DB_get_PT ((long)aus_tab[1]);         // RadiusPT
      // project RadiusPT --> Axis; dist == radius
      UT3D_pt_projptptvc (&pt1, &ci3d.rad, NULL, pp1, &ci3d.pc, &ci3d.vz);
      // Circ-p1 = pt1 --> pp1
      UT3D_vc_2pt (&vc1, &pt1, pp1);
      UT3D_pt_traptvc (&ci3d.p1, &ci3d.pc, &vc1);
      ci3d.p2 = ci3d.p1;
      goto Fertig3D;




    //-----------------------------------------------------------------
    //C = P rad CW|CCW              // CI  =  CenterPT   Radius   Dreh
    } else if ((aus_typ[0] == Typ_PT)         &&
               (aus_typ[1] == Typ_Val)        &&
               (aus_typ[2] == Typ_cmdNCsub))      {

      ci3d.pc  = DB_GetPoint ((long)aus_tab[0]);
      ci3d.rad = aus_tab[1];

      ci3d.vz  = WC_sur_act.vz;

      // Vektor aus DX und Radius
      UT3D_vc_setLength (&vc1, &WC_sur_act.vx, ci3d.rad);
      UT3D_pt_traptvc (&ci3d.p1, &ci3d.pc, &vc1);
      ci3d.p2  = ci3d.p1;

      i1 = aus_tab[2];
      if(i1 == T_CW) ci3d.rad = fabs(ci3d.rad) * -1;
      goto Fertig3D;


    //-----------------------------------------------------------------
    // C = P|L|C  P|L|C  val                 // CI  =  Tang Tang Radius [CW|CCW]
    } else if (aus_typ[2] == Typ_Val) {
      return APT_decode_cttr (ciO, aus_anz, aus_typ, aus_tab, Dreh, iMod);


    //-----------------------------------------------------------------
    // C = P|L|C P|L|C P|L|C                    // C = Tang Tang Tang
    } else {
      return APT_decode_cttt (ciO, aus_anz, aus_typ, aus_tab, Dreh, iMod);
    }




    //-----------------------------------------------------------------
    // circs mit 4 parameters ohne ARC
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
/*
    // c1=p1,p2,Radius,Seite        // 2 Punkte am Umfang Radius
    if        ((aus_typ[0] == Typ_PT)    &&
               (aus_typ[1] == Typ_PT)    &&
               (aus_typ[2] == Typ_Val)   &&
               (aus_typ[3] == Typ_modif))      {

      ci3d.vz = WC_sur_act.vz;
      goto L_PPrs;



    //-----------------------------------------------------------------
    //C = P L radius Seite          // CIr durch P und Line mit Radius
    } else if ((aus_typ[0] == Typ_PT)    &&
               (aus_typ[1] == Typ_LN)    &&
               (aus_typ[2] == Typ_Val)   &&
               (aus_typ[3] == Typ_modif))      {


      pt1 = DB_GetPoint ((long)aus_tab[0]);
      ln1 = DB_GetLine ((long)aus_tab[1]);
      goto L_LPr1;

    //-----------------------------------------------------------------
    //C = L P radius Seite          // CI durch P und Line mit Radius
    } else if ((aus_typ[0] == Typ_LN)    &&
               (aus_typ[1] == Typ_PT)    &&
               (aus_typ[2] == Typ_Val)   &&
               (aus_typ[3] == Typ_modif))      {


      ln1 = DB_GetLine ((long)aus_tab[0]);
      pt1 = DB_GetPoint ((long)aus_tab[1]);

      L_LPr1:
      d1  = aus_tab[2];   // r
      iNr = aus_tab[3];   // MOD
      APT_modMax1 = UT3D_ci_lnptrd (ca, &i1, &ln1, &pt1, d1, iNr);
      if(APT_modMax1 < 0) goto Geom_err;
      goto L_work_mod;




    //-----------------------------------------------------------------
    //C = P C radius Seite          // CIr durch P und Line mit Radius
    } else if ((aus_typ[0] == Typ_PT)    &&
               (aus_typ[1] == Typ_CI)    &&
               (aus_typ[2] == Typ_Val)   &&
               (aus_typ[3] == Typ_modif))      {


      pt1 = DB_GetPoint ((long)aus_tab[0]);
      ci1 = DB_GetCirc ((long)aus_tab[1]);
      goto L_CPr1;

    //-----------------------------------------------------------------
    //C = C P radius Seite          // CI durch P an CI mit Radius
    } else if ((aus_typ[0] == Typ_CI)    &&
               (aus_typ[1] == Typ_PT)    &&
               (aus_typ[2] == Typ_Val)   &&
               (aus_typ[3] == Typ_modif))      {


      ci1 = DB_GetCirc ((long)aus_tab[0]);
      pt1 = DB_GetPoint ((long)aus_tab[1]);

      L_CPr1:
      d1  = aus_tab[2];   // r
      iNr = aus_tab[3];   // MOD
      APT_modMax1 = UT3D_ci_ciptrd (ca, &i1, &ci1, &pt1, d1, iNr);
      if(APT_modMax1 < 0) goto Geom_err;
      goto L_work_mod;




    //-----------------------------------------------------------------
    //C = L L radius Seite          // 2 Lines verrunden mit Radius
    } else if ((aus_typ[0] == Typ_LN)    &&
               (aus_typ[1] == Typ_LN)    &&
               (aus_typ[2] == Typ_Val)   &&
               (aus_typ[3] == Typ_modif))      {

      ln1 = DB_GetLine ((long)aus_tab[0]);
      ln2 = DB_GetLine ((long)aus_tab[1]);
      d1  = aus_tab[2];   // r
      iNr = aus_tab[3];   // MOD

      APT_modMax1 = UT3D_ci_2lnrd (ca, &i1, &ln1, &ln2, d1, iNr);
      if(APT_modMax1 < 0) goto Geom_err;
      goto L_work_mod;






    //-----------------------------------------------------------------
    //C = C L radius Seite          // C = LN-Tang, CIR-Tang, radius
    } else if ((aus_typ[0] == Typ_CI)    &&
               (aus_typ[1] == Typ_LN)    &&
               (aus_typ[2] == Typ_Val)   &&
               (aus_typ[3] == Typ_modif))      {

      ln1 = DB_GetLine ((long)aus_tab[1]);
      ci1 = DB_GetCirc ((long)aus_tab[0]);
      goto L_LCr1;

    //-----------------------------------------------------------------
    //C = L C radius Seite          // C = LN-Tang, CIR-Tang, radius
    } else if ((aus_typ[0] == Typ_LN)    &&
               (aus_typ[1] == Typ_CI)    &&
               (aus_typ[2] == Typ_Val)   &&
               (aus_typ[3] == Typ_modif))      {

      ln1 = DB_GetLine ((long)aus_tab[0]);
      ci1 = DB_GetCirc ((long)aus_tab[1]);
     

      L_LCr1:
      d1  = aus_tab[2];   // r
      iNr = aus_tab[3];   // MOD
      APT_modMax1 = UT3D_ci_lncird (ca, &i1, &ln1, &ci1, d1, iNr);
      if(APT_modMax1 < 0) goto Geom_err;
      goto L_work_mod;







    //-----------------------------------------------------------------
    //C = C C radius Seite          // C = CIR-Tang, CIR-Tang, radius
    } else if ((aus_typ[0] == Typ_CI)    &&
               (aus_typ[1] == Typ_CI)    &&
               (aus_typ[2] == Typ_Val)   &&
               (aus_typ[3] == Typ_modif))      {

      ci1 = DB_GetCirc ((long)aus_tab[0]);
      ci2 = DB_GetCirc ((long)aus_tab[1]);
      d1  = aus_tab[2];   // r
      iNr = aus_tab[3];   // MOD

      APT_modMax1 = UT3D_ci_cicird (ca, &i1, &ci1, &ci2, d1, iNr);
      if(APT_modMax1 < 0) goto Geom_err;
      goto L_work_mod;



  //==================================================================
  } else if (aus_anz == 4)   {
  //==================================================================


    //-----------------------------------------------------------------
    }
*/



  //==================================================================
  } else if (aus_anz == 5)   {
  //==================================================================


    //-----------------------------------------------------------------
    // C = P P Radius Seite Z-Achsvektor       // 2 Punkte am Umfang Radius Z-Vec
    // OLD VERSION; Typ_modif is not last !!
    if        ((aus_typ[0] == Typ_PT)    &&
               (aus_typ[1] == Typ_PT)    &&
               (aus_typ[2] == Typ_Val)   &&
               (aus_typ[3] == Typ_modif) &&
               (aus_typ[4] == Typ_VC))      {


      pp1 = DB_get_PT ((long)aus_tab[1]);  // VERKEHRT; wie UT3D_ci_2ptvcrd
      pp2 = DB_get_PT ((long)aus_tab[0]);
      ci3d.rad = aus_tab[2];
      ii = aus_tab[3];
      ci3d.vz = DB_GetVector ((long)aus_tab[4]);
      // goto L_PPrs;
      i1 = UT3D_ci_2ptvcrd (ca, pp1, pp2, &ci3d.vz, ci3d.rad); //i1=circNr
      if(i1 < 2) {
        if(i1 < 1) goto Geom_err;
        ci3d = ca[0];
      } else {
        ci3d = ca[ii];
      }
      goto Fertig3D;


    //-----------------------------------------------------------------
    } else {
      goto NotImp_err;
    }



  //==================================================================
  }
  L_undef:
    printf(" ci - L_undef:\n");
    goto NotImp_err;





/*
  //-----------------------------------------------------------------
  // ALTE METHODE:
  i1 = APT_decode_inp (aus_anz, aus_typ, aus_tab);
  if(i1 < 0) goto Error;

  Dreh   =  IN_Dreh;




  //-----------------------------------------------------------------
  // C2=C1
  if      ((IN_anz       ==   1)        &&
           (IN_obj[0].typ==Typ_CI)) {


    ci3d = UT3D_ci_obj (&IN_obj[0]);

    goto Fertig3D;





  //-----------------------------------------------------------------
  // c1=p1,l1                    (p1=center, tangential an l1)
  } else if ((IN_anz       ==   2)       &&
             (IN_obj[0].typ==Typ_PT)     &&
             (IN_obj[1].typ==Typ_LN)) {

    // ln20 = UT2D_ln_obj2 (&IN_obj[1]);
    // UT2D_pt_projptln (&pt20, &IN_obj[0].p1, &ln20);

    ln1.p1 = IN_obj[1].p1;
    ln1.p2 = IN_obj[1].p2;

    UT3D_pt_projptln (&pt1, &ci3d.rad, &IN_obj[0].p1, &ln1);

    // ci3d.rad = UT3D_len_2pt (&IN_obj[0].p1, &pt1);

    ci3d.pc = IN_obj[0].p1;

    ci3d.p1.x = ci3d.pc.x + ci3d.rad;
    ci3d.p1.y = ci3d.pc.y;
    ci3d.p1.z = ci3d.pc.z;


    ci3d.p2 = ci3d.p1;

    ci3d.vz  = UT3D_VECTOR_Z;
    goto Fertig3D;
*/


/*
  //-----------------------------------------------------------------
  // c1=pc,vc,radius         (Mittelpunkt, Drehachse, Radius, Drehrichtung)
  } else if ((IN_anz       ==   3)       &&
             (IN_obj[0].typ==Typ_PT)     &&
             (IN_obj[1].typ==Typ_VC)     &&
             (IN_obj[2].typ==Typ_Val)) {

    ci3d.rad  = IN_obj[2].rad;

    ci3d.pc = IN_obj[0].p1;

    ci3d.vz = IN_obj[1].vz;

    // Z-Achse normieren
    UT3D_vc_normalize (&ci3d.vz, &ci3d.vz);

    // zu Drehachse eine X-Achse generieren
    UT3D_vc_perpvcplXY (&vc1, &ci3d.vz);

    UT3D_pt_traptvclen (&ci3d.p1, &ci3d.pc, &vc1, ci3d.rad);

    ci3d.p2   = ci3d.p1;

    goto Fertig3D;





  //-----------------------------------------------------------------
  // c1=p1,p2,p3            (Anfangspunkt, Endpunkt, Mittelpunkt, Drehrichtung)
  } else if ((IN_anz       ==   3)       &&
             (IN_obj[0].typ==Typ_PT)     &&
             (IN_obj[1].typ==Typ_PT)     &&
             (IN_obj[2].typ==Typ_PT)) {


    ci3d.p1 = IN_obj[0].p1;
    ci3d.p2 = IN_obj[1].p1;
    ci3d.pc = IN_obj[2].p1;
  
    ci3d.vz   = UT3D_VECTOR_Z;

    ci3d.rad = UT3D_len_2pt (&ci3d.p1, &ci3d.pc);

    goto Fertig3D;
*/



/*
  //-----------------------------------------------------------------
  // c1=l1,l2,25,CW          (Tang. an 2 Linien, Radius)
  } else if ((IN_anz       ==   3)       &&
             (IN_obj[0].typ==Typ_LN)     &&
             (IN_obj[1].typ==Typ_LN)     &&
             (IN_obj[2].typ==Typ_Val)) {


// 2D-Version:
    ci20.rad = IN_obj[2].rad;

    // Lines > 2D-Lines
    ln20 = UT2D_ln_obj2 (&IN_obj[0]);
    ln21 = UT2D_ln_obj2 (&IN_obj[1]);

    UT2D_vc_ln (&vc20, &ln20);             // ln1 > vc20
    UT2D_vc_ln (&vc21, &ln21);             // ln2 > vc21

    i1 = UT2D_ci_2vc2ptrd (pta, &ln20.p1, &vc20, &ln21.p1, &vc21, IN_obj[2].rad);
    if(i1 == 0) {
      TX_Error(" Lines parallel");
      goto Exit;
    }

    // nun die Lösung entspr. ModTab suchen
    i1 = APT_solv_mod (pta, 4);

    ci20.pc = pta[i1];

    ci20.p1.x = ci20.pc.x + ci20.rad;
    ci20.p1.y = ci20.pc.y;

    ci20.p2 = ci20.p1;

    goto Fertig;

*/






/*
// 2D-Version:

  //-----------------------------------------------------------------
  // c1=l1,c1,Radius          (Tang. an l, c, mit Radius)
  } else if ((IN_anz       ==   3)       &&
             (IN_obj[0].typ==Typ_LN)     &&
             (IN_obj[1].typ==Typ_CI)     &&
             (IN_obj[2].typ==Typ_Val)) {


    // Line > vc20
    UT2D_vc_2pt (&vc20, &IN_obj[0].p1, &IN_obj[0].p2);

    // d1=Radius des gegebenen Kreises
    d1=fabs(IN_obj[1].rad);

    //i2=Side; inside=0, outside=1
    if(IN_ModIn == T_IN) {
      i2=0;
    } else {
      i2=1;
    }

    // verrunden Circ/Line; 4 Lösungen
    i2=UT2D_ci_ciptvcrd(pta,&IN_obj[1].pc,d1,i2,&IN_obj[0].p1,&vc20,IN_obj[2].rad);


    // nun die Lösung entspr. ModTab suchen
    if(i2 > 0) {
      i1 = APT_solv_mod (pta, i2);

    } else {
      TX_Error(" Kein Schnittpunkt ****");
      goto Error;
    }



    ci20.rad = IN_obj[2].rad;  // Dreh wird nachträglich noch appliziert.

    ci20.pc  = pta[i1];

    ci20.p1.x = ci20.pc.x + ci20.rad;
    ci20.p1.y = ci20.pc.y;

    ci20.p2 = ci20.p1;

    goto Fertig;
    TX_Error(" derzeit nicht aktiv");
    goto Exit;





  //-----------------------------------------------------------------
  // c1=c1,c2,Radius        (Tang. an C1,C2 Radius)
  } else if ((IN_anz       ==   3)       &&
             (IN_obj[0].typ==Typ_CI)     &&
             (IN_obj[1].typ==Typ_CI)     &&
             (IN_obj[2].typ==Typ_Val)) {

    // d1 wird der Radius 1; je nach IN oder OUT klein od groß
    if(IN_ModIn == T_IN) {
      d1 = fabs(IN_obj[0].rad) - IN_obj[2].rad;
    } else {
      d1 = fabs(IN_obj[0].rad) + IN_obj[2].rad;
    }

    // d2 wird der Radius 2; je nach IN oder OUT klein od groß
    if(IN_Mod2In == T_IN) {
      d2 = fabs(IN_obj[1].rad) - IN_obj[2].rad;
    } else {
      d2 = fabs(IN_obj[1].rad) + IN_obj[2].rad;
    }

    //TX_Print("AC-AC %d %d %f %f %f",IN_ModIn,IN_Mod2In,d1,d2,IN_obj[2].rad);

    i1=UT2D_2pt_intcici (&pta[0], &pta[1], &IN_obj[0].pc, d1, &IN_obj[1].pc, d2);

    // nun die Lösung entspr. ModTab suchen
    if(i1 == 0) {
      i1 = APT_solv_mod (pta, 2);

    // nur Berührung
    } else if (i1 == 1) {
      i1 = 0;

    } else {
      TX_Error(" Kein Schnittpunkt ****");
      goto Error;
    }


    ci20.rad = IN_obj[2].rad;

    ci20.pc = pta[i1];

    ci20.p1.x = ci20.pc.x + ci20.rad;
    ci20.p1.y = ci20.pc.y;

    ci20.p2 = ci20.p1;

    goto Fertig;
    TX_Error(" derzeit nicht aktiv");
    goto Exit;






  //-----------------------------------------------------------------
  // c1=p1,p2,p3,d1      (Startpkt Endpkt Mittelpkt Z-Achse)
  } else if ((IN_anz       ==   4)       &&
             (IN_obj[0].typ==Typ_PT)     &&
             (IN_obj[1].typ==Typ_PT)     &&
             (IN_obj[2].typ==Typ_PT)     &&
             (IN_obj[3].typ==Typ_VC)) {


    ci3d.p1 = IN_obj[0].p1;
    ci3d.p2 = IN_obj[1].p1;
    ci3d.pc = IN_obj[2].p1;
  
    ci3d.vz = IN_obj[3].vz;

    ci3d.rad = UT3D_len_2pt (&ci3d.p1, &ci3d.pc);


    goto Fertig3D;







  //-----------------------------------------------------------------
  } else {
    TX_Error(" Kreisdefinition nicht implementiert\n");
    goto Error;
  }

*/

/*
  L_work_mod:   // i1=Nr_of_points;
    // printf(" L_work_mod: APT_modMax1=%d mod=%f\n",APT_modMax1,aus_tab[3]);

  
  if(i1 < 1) {
    if(iNr >= 0) TX_Error("CIRC: no solution Nr %d",iNr);
    else         TX_Error("CIRC: no solution");
    return -1;
  }
  ci3d = ca[0];
  goto Fertig3D;
*/



/*
    if(i1 < 0) {
      TX_Error("keine Loesung");
      return -1;
    }

    if(APT_modMax1 == 1) {
      i2 = 0;

    } else if(APT_modMax1 == 2) {
      i2 = aus_tab[3];     // 0-3;  (Input - 1 !!)
      i2 = i2%2;           // 0->0, 1->1, 2->0, 3->1
      printf(" \n");

    } else if(APT_modMax1 == 3) {
      i2 = aus_tab[3];     // 0-3;  (Input - 1 !!)
      i2 = i2%3;           // 0->0, 1->1, 2->0, 3->1
      printf(" \n");

    } else {
      modTab[0] = aus_tab[3];
      for(i2=0; i2<i1; ++i2) {
        pa[i2] = ca[i2].pc;
      }
      i2 = APT_solv3D_mod (pa, i1, modTab, 1);
    }


    ci3d = ca[i2];
    goto Fertig3D;
*/






/*
  //---------------------------------------------------------------------
  // nun ist der Bogen als Circ2D in ci20.
  Fertig:

  if(Dreh == T_CW) {
    ci20.rad = -ci20.rad;
  }

  *ciO = UT3D_ci_ci2 (&ci20);

  ciO->vz = UT3D_VECTOR_Z;



  //TX_Print("2D APT_decode_ci %f,%f",ciO->pc.x,ciO->pc.y);
  //TX_Print("  p1/2=%f,%f/%f,%f",ciO->p1.x,ciO->p1.y,ciO->p2.x,ciO->p2.y);
  //TX_Print("  r=%f",ciO->rad);

  goto Exit;
*/





  //---------------------------------------------------------------------
  // nun ist der Bogen als Circ in ci3d; Radius aber immer positiv !
  // vz wird noch normiert; alles andere muss fertig sein !
  Fertig3D:        // printf(" Dreh=%d\n",Dreh);

  if(Dreh < 0) {
    ci3d.rad  = -fabs(ci3d.rad);
    // ci3d.ango = -fabs(ci3d.ango);
    UT3D_ci_setangr (&ci3d);
  }

  // vz normieren
  Fertig1:   // in: ci3d
  UT3D_vc_setLength (&ci3d.vz, &ci3d.vz, 1.);
  *ciO = ci3d;





  Exit:    // ciO complete

  // wenn Vollkreis und vz = DIZ: vz umdrehen !
  // if(UT3D_comp2pt(&ciO->p1,&ciO->p2,UT_TOL_pt)) {
  if(UTP_comp2db (fabs(ciO->ango), RAD_360, UT_TOL_pt) == 1) {
    if(UT3D_comp2vc_p(&ciO->vz, (Vector*)&UT3D_VECTOR_IZ, UT_TOL_min1) != 0)
      UT3D_vc_invert (&ciO->vz, &ciO->vz);
  }

  L_exit9:

    // DEB_dump_obj__(Typ_CI, ciO, "ex APT_decode_ci:\n");

  return rc;



  //================================================================
  // C = ARC PTstart VCstart Radius [Angle] [Z-Axis-Vector] [CW/CCW]
  //      0    1       2       3       4         5
  L_ARC_pt_vc_rad:
    // 1(pt), 2(vc), 3(Rad) sind schon getestet.
    pp1 = DB_get_PT ((long)aus_tab[1]);
    vc2 = DB_GetVector ((long)aus_tab[2]);
    d1 = fabs(aus_tab[3]);       //  Radius -> d1
    d2 = 0.;                     //  Angle (full circle)
    vc1 = WC_sur_act.vz;         // vz

    // get Angle -> d2
    ii = 4;
    if(aus_anz > ii)  {
      if(aus_typ[ii] == Typ_Angle) {
        d2  = fabs(UT_RADIANS(aus_tab[ii]));
        ++ii;
      }
    }

    // get Axis
    if(aus_anz > ii)  {
      if(aus_typ[ii] == Typ_VC) {
        vc1  = DB_GetVector ((long)aus_tab[ii]);
        ++ii;
      }
    }

    // [CW|CCW]
    if(Dreh < 0) {
          // d2 = RAD_360 - d2;     // ang
          UT3D_vc_invert (&vc1, &vc1);  // vz
          // UT3D_vc_invert (&vc2, &vc2);  // vx
    }
      // printf(" rad d1=%f ang d2=%f\n",d1,d2);
      // DEB_dump_obj__(Typ_VC, &vc1, "L_ARC_pt_vc_rad vz:");

    // create Circ (ci,  ps,  vs,  rd, vz, a1);
    UT3D_ci_ptvcrd (ciO, pp1, &vc2, d1, &vc1, d2);

    goto Exit;






  //================= Fehler =============================
  Par_err:
  TX_Error(" with parameter - Circ-definition ****");
  goto Error;

  Geom_err:
  TX_Error(" geometry (no solution) - Circ-definition ****");
  goto Error;

  NotImp_err:
  TX_Error("Circ-definition: NOT YET IMPLEMENTED ****");
  goto Error;

  EOM_err:
  TX_Error("out of memory-error ****");


  Error:
    printf("ex APT_decode_ci -1\n");

  return -1;

}   // end of APT_decode_ci


/*
//================================================================
  int APT_decode_ausdr (int *aus_typ, double *aus_tab,
                        int aus_siz, char** data) {
//================================================================
/// \code
/// DO NOT USE; new func: ATO_ato_srcLn__
/// decode sourceObj -> atomicObj
/// Names ("xx #name") must have been removed
/// \endcode
//
//   die Ausdrücke aus data einlesen, decodieren und merken
//   Returncode = Nr of Records in aus_typ/aus_tab
//   Die Ausdrücke zerlegen (Trennzeichen ist ",")
//   Bei Typ_Txt ist die Posi in aus_tab.
//   Typ_String: 
//      in aus_tab ist die TextStartposition in aus_tab;
//      der Text ist APT_defTxt[(int)aus_tab].
//
  int    aus_anz, iNr, i1, sLen, aus_rest;
  //int    ctyp, oTyp, oInd;
  //double d1;
  //char   deli, txtOut[32];
  char   Line[2056], *w_next, *w, *lp, *sptr;




  // printf("///////////////////////////////////////////////////\n");
  // printf("APT_decode_ausdr |%s|\n",*data);


  aus_anz = 0;
  sLen    = 0;
  sptr    = *data;
  w       = *data;                  // w = act.startpos.

  if(strlen(*data) < 1) return 0;




  //==========================================================================
  // Die Zeile muss in Ausdrücke zerlegt werden; delimiters: ' ', ','
  NextAus:
    // printf("    -APT_decode_ausdr weiter %d |%s|\n",aus_anz,w);

  // skip the leading blanks
  while (*w == ' ') {
    ++w;
  }


  // skip rest of line if it is comment/name
  if(*w == '#') {                       // 2013-09-18
    // printf(" skip name ..\n");
    --aus_anz;
    goto Fertig;
  }


  // den naechsten Ausdruck aus <w> nach <Line> kopieren
  // wenn vor den naechsten "," ein "[" kommt, dann muss der gesamte
  // Klammerausdruck mit !
  // Also "p1[10,10]" darf nicht zerschnitten werden !
  // lp = Line;
  w_next = APT_cp_ausd (Line, w, 2056);
    // printf(" nach APT_cp_ausd: w=|%s| Line=|%s| w_next=|%s|\n",w,Line,w_next);


 

  //===================== einen Ausdruck abarbeiten ===========================
  // APT_decode_ausdr1: den Ausdruck <Line> decodieren und in aus_typ,aus_tab
  // zufuegen.
  aus_rest = aus_siz - aus_anz;
  iNr = APT_decode_ausdr1 (&aus_typ[aus_anz], &aus_tab[aus_anz], aus_rest, Line);
  if(iNr < 0) {   // <0=Error
    // TX_Print("exit APT_decode_ausdr -1\n");
    return -1;
  }
    // printf(" ex _ausdr1 iNr=%d\n",iNr);
    // for(i1=aus_anz;i1<iNr+aus_anz; ++i1) 
    // printf(" aus[%d] = typ=%d val=%f\n",i1,aus_typ[i1],aus_tab[i1]);


    // TESTBLOCK
    // for(i1=0;i1<aus_anz; ++i1) 
    // printf(" aus[%d] = typ=%d val=%f\n",i1,aus_typ[i1],aus_tab[i1]);



  //wenn ein Typ_Txt gefunden wurde noch die Position des 1 char into
  // aus_tab geben.
  // Die APT_decode_ausdr1 kennt diese Position nicht !
  if((aus_typ[aus_anz] == Typ_Txt)||(aus_typ[aus_anz] == Typ_String)) {
    //printf(" find Textpos %d\n",sLen);

    i1 = w - sptr;
    aus_tab[aus_anz] = (double)i1;

  }


  if(*w_next == '\0') {
// .. 
// ..   // wenn der letzte char von w ein "," ist:
// ..   w = w_next - 1;
// ..   if(*w == '\\') {
// ..     // printf(" Fortsetzungszeile erforderlich %d!\n",aus_anz);
// ..     ++aus_anz;
// ..     aus_typ[aus_anz] = Typ_StrDel;
// ..     aus_tab[aus_anz] = 0.0;
// ..   }
// .. 
    goto Fertig;
  }


  // ';' kommt nur mehr als terminator eines IF
  if(*w_next == ';')  goto Fertig;


  w = w_next;
  // ++aus_anz;
  aus_anz += iNr;
  if(aus_anz >= aus_siz) {
    TX_Error("APT_decode_ausdr E001");
    return -1;
  }
  goto NextAus;




  //===========================================================================
  Fertig:
  aus_anz += iNr;
  aus_typ[aus_anz] = Typ_Error;


/// .. 
// ..   // // Nur Testausgaben:
// ..   printf("ex APT_decode_ausdr %d\n",aus_anz);
// ..   for(i1=0; i1<aus_anz; ++i1) {
// ..     // TX_Print("    %d %d %f",i1,aus_typ[i1],aus_tab[i1]);
// ..     printf("    %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
// ..   }
// ..   printf("/////////////////////////////////////////\n");
// .. 


  //-----------------------------------------------------
  // math operationen auswerten:
  if(aus_anz > 2)
    aus_anz = APT_decode_comp1 (aus_typ, aus_tab, aus_anz);


  // L_exit:
  return aus_anz;
}
*/


//================================================================
  int APT_decode_Fmc (void *data, int *typ, ObjAto *ato) {
//================================================================
// execute math.function
// Input:
//   typ     typ of math.function
//   ato     parameters
// Output:
//   typ     typ of result
//   data    result

// see also APT_decode_func

  double   *aus_tab, d2;
  long     l1;


  // printf("APT_decode_Fmc typ=%d\n",*typ);
  // ATO_dump__ (ato);


  aus_tab = ato->val;

  switch (*typ) {

    case Typ_FcmSQRT:
      *(double*)data = sqrt(aus_tab[0]);
      break;
    case Typ_FcmSIN:
      *(double*)data = sin(aus_tab[0]);
      break;
    case Typ_FcmCOS:
      *(double*)data = cos(aus_tab[0]);
      break;
    case Typ_FcmTAN:
      *(double*)data = tan(aus_tab[0]);
      break;
    case Typ_FcmASIN:
      *(double*)data = asin(aus_tab[0]);
      break;
    case Typ_FcmACOS:
      *(double*)data = acos(aus_tab[0]);
      break;
    case Typ_FcmATAN:
      *(double*)data = atan(aus_tab[0]);
      break;
    case Typ_FcmABS:
      *(double*)data = fabs(aus_tab[0]);
      break;
    case Typ_FcmFIX:
      l1 = aus_tab[0];
      break;
    case Typ_FcmRND:
      d2 = 0.5;
      if(aus_tab[0] < 0.0) d2 = -0.5;
      l1 = (int)(aus_tab[0] + d2);
      *(double*)data = l1;
      break;
    default:
      TX_Error("APT_decode_Fmc E001 %d",*typ);
      return -1;
  }

  *typ = Typ_Val;

  return 0;
}


//================================================================
  int APT_decode_Opm (double *val, ObjAto *ato) {
//================================================================
// APT_decode_Opm                 do math operations

// see also APT_decode_comp1

  int  ii, io;

  printf("APT_decode_Opm\n");

  ii = ato->nr - 1;

  L_nxt:

  if(ii < 1) {
    *val = ato->val[0];
    return 0;
  }

  io = ii - 2;
  if(io < 0) {TX_Error("APT_decode_Opm E001"); return -1;}

  switch (ato->typ[ii - 1]) {
      case TYP_OpmPlus:
        ato->val[io] = ato->val[io] + ato->val[ii];
        break;
      case TYP_OpmMinus:
        ato->val[io] = ato->val[io] - ato->val[ii];
        break;
      case TYP_OpmMult:
        ato->val[io] = ato->val[io] * ato->val[ii];
        break;
      case TYP_OpmDiv:
        ato->val[io] = ato->val[io] / ato->val[ii];
        break;
      case TYP_OpmPow:
        ato->val[io] = pow(ato->val[io],ato->val[ii]);
        break;
      default:
        TX_Error("APT_decode_Opm E002");
        ATO_dump__ (ato, "");
        return -1;
  }

  ii -= 2;
  goto L_nxt;


  return 0;

}
 

//================================================================
  int APT_decode_comp1 (int *aus_typ,double *aus_tab,int aus_anz) {
//================================================================
// UNUSED !
// do math operations; compute, reduce aus_anz, modify aus_typ/aus_tab.
// Output:
//   RetCod: modified aus_anz

// see also APT_decode_Opm

  int     i0, o0, i1, o1, i2, i3;


  printf("APT_decode_comp1 %d\n",aus_anz);
  for(i1=0;i1<aus_anz;++i1)printf(" [%d] %d %f\n",i1,aus_typ[i1],aus_tab[i1]);



  //----------------------------------------------------------------
  // if 1.Obj. = '-' and 1.Obj = value:  value=-value; delete 1.Obj.
  if(aus_anz < 2) goto L_exit;
  if((aus_typ[0] == Typ_Int1)&&(aus_typ[1] == Typ_Val)) {
    if(aus_tab[0] == 45.0) { // '+'=43, '-'=45
      aus_tab[1] = -aus_tab[1];
    } else if(aus_tab[0] == 43.0) { // '+'=43, '-'=45
      // OK ..
    } else {   // '*'=42, '/'=47
      goto L_test2; // keep operators  * and /
    }
    i1 = aus_anz;
    MEM_del_DbRec (&i1, aus_tab, 0);
    MEM_del_IndRec (&aus_anz, aus_typ, 0);
  }



  //----------------------------------------------------------------
  // check for 2 consecutive Operators; if the second is '-':
  //   if the following Obj is a value: value=-value; delete '-'-Operator.
  L_test2:
  if(aus_anz < 4) goto L_test_math;
  for(i0=1; i0<aus_anz-2; ++i0) {
    if(aus_typ[i0] != Typ_Int1) continue;
    i1 = i0 + 1;
      // printf(" i0=%d i1=%d\n",i0,i1);
    if(aus_typ[i1] != Typ_Int1) continue;
    i2 = i1 + 1;
      // printf(" i0=%d i1=%d i2=%d\n",i0,i1,i2);
    if(aus_typ[i2] != Typ_Val) goto L_exit;    // Error . ?
    aus_tab[i2] = -aus_tab[i2];
    i3 = aus_anz;
    MEM_del_DbRec (&i3, aus_tab, i1);
    MEM_del_IndRec (&aus_anz, aus_typ, i1);
    goto L_test2;
  }


  //----------------------------------------------------------------
  L_test_math:
  if(aus_anz < 3) goto L_exit;
  i2 = aus_anz - 1;
  if(aus_typ[i2] != Typ_Val) goto L_exit;

  if(aus_typ[i2-1] != Typ_Int1) goto L_exit;
  o1 = aus_tab[i2-1];   // operator = middlePos

  i1 = i2 - 2;
  if(aus_typ[i1] != Typ_Val) goto L_exit;

    // printf(" i1=%d o1=%d i2=%d\n",i1,o1,i2);


  // test priority
  if(aus_anz < 5) goto L_comp;
  if((o1 != '+')&&(o1 != '-')) goto L_comp; 
  o0 = aus_tab[i1-1];   // operator = middlePos
  if((o0 == '+')||(o0 == '-')) goto L_comp; 

  // change
  //  2   *   3   +   2
  // i0  o0  i1  o1  i2

  // swap i0 - i2; swap o0 - o1;
  i0 = i1 - 2;
  MEM_swap_2db (&aus_tab[i0], &aus_tab[i2]);
  MEM_swap_2db (&aus_tab[i1-1], &aus_tab[i2-1]);
  o1 = o0;


  L_comp:
    if(o1 == '*')      aus_tab[i1] *= aus_tab[i2];
    else if(o1 == '/') aus_tab[i1] /= aus_tab[i2];
    else if(o1 == '+') aus_tab[i1] += aus_tab[i2];
    else if(o1 == '-') aus_tab[i1] -= aus_tab[i2];
    else if(o1 == '^') aus_tab[i1] = pow(aus_tab[i1], aus_tab[i2]);
      // printf(" aus_tab[%d] = %d\n",i1,aus_tab[i1]);

    aus_anz -= 2;
    aus_typ[aus_anz] = Typ_Error;
    goto L_test_math;


  L_exit:

    // printf("ex APT_decode_comp1 %d\n",aus_anz);
    // for(i1=0;i1<aus_anz;++i1)
    // printf(" [%d] %d %f\n",i1,aus_typ[i1],aus_tab[i1]);

  return aus_anz;

}


/*
//================================================================
  int APT_decode_ausdr1 (int *aus_typ,double *aus_tab,int aus_siz,char *data) {
//================================================================
/// .. 
// .. EINEN Ausdruck aus data einlesen u. decodieren
// .. hier kommt nur mehr ein Ausdruck; er kann aber noch Funktionen beinhalten.
// .. Enhaelt er einen Klammerausdruck: extrahieren und mit ?? abarbeiten.
// ..  Also zB In  > Out:
// ..        "3+5" > 8
// .. 
// .. ReturnCode:  -1 = Error; else nr of records added to aus_typ/aus_tab.
// .. 
// .. FEN:
// .. man sollte alle so aendern, dass bei Rekursion in aus_typ/aus_tab
// .. hineinaddiert wird; also bei der Rekursion mit einem Offset und korrigierter
// .. TabSiz rufenTabSiz.
// .. 
// .. 

  int    irc, i1, ityp, ctyp, oTyp;
  long   l1, oInd;
  double d1;
  char   deli, txtOut[40], cmd[40], LineK[256];
  char   vorz, *w_next, *w, *eod;

  int    w_anz;
  int    w_typ[200];
  double w_tab[200];
  // char   w_del[200];
  // char   w_ope[200];
  int    w_siz;

  int    k_anz = -1;
  int    k_typ[200];
  double k_tab[200];
  Vector vc1;


  // printf("1111111111111111111111111111111111111111111111111111111 \n");
  // printf("APT_decode_ausdr1 |%s| siz=%d\n",data,aus_siz);

  eod = &data[strlen(data)];  // 2014-12-17 2013-10-08

  *aus_typ = 0;
  *aus_tab = 0.0;

  irc   = 1;
  w_anz = -1;
  w_next = data;

  vorz = '+';
  deli = ' ';


  //============================================================================
  Next:
  ++w_anz;

  Nex_1:
  if(w_next >= eod) goto Auswerten; // 2014-12-17 2013-10-08; needed for bug with "..))"
    // printf("... Nex_1: |%s| w_anz=%d deli=|%c|\n",w_next,w_anz,deli);
  if(w_anz >= 200) goto L_err2;
  w_tab[w_anz] = 0.;

/// .. 
// .. // 2013-10-08
// .. if(deli == '(') {
// ..     w_siz = 200 - w_anz;
// ..     i1 = APT_decode_ausdr1 (&w_typ[w_anz], &w_tab[w_anz], w_siz, w_next);
// ..     if(i1 < 0) goto Error;
// ..     w_anz += i1;
// ..     w_next += strlen(w_next);
// ..     goto Next;
// .. }
// .. 


  NunWert:
  w = w_next;                            // startpos
  if(*w == '\0') goto Auswerten;
  ctyp = UTX_get_word (&w, &w_next, txtOut, &d1, &deli);
    // printf("UTX_get_word: ctyp=%d deli=|%c| txtOut=|%s| w_next=|%s| %f\n",
                          // ctyp,   deli,     txtOut,     w_next,     d1);


  UTX_cp_word_2_upper (cmd, txtOut);
    // printf(" next word cmd=|%s| deli=|%c|\n",cmd,deli);



  //-------------------------------------------------------------------
  if(ctyp == TXT_Typ_num) {    // 3
      goto L_fix_numVal;


  //-------------------------------------------------------------------
  } else if (ctyp == TXT_Typ_string) {   // 4
    w_typ[w_anz] = Typ_String;
    // w_tab[w_anz] = 1.;
    // tmptxbuf[1] = w;
    // printf(" got string |%s|\n",tmptxbuf[1]);

    goto Next;



  //-------------------------------------------------------------------
  // 'P'     deli '('    od
  // 'P20'
  } else if (ctyp == TXT_Typ_text) {   // 2  zB 'P' mit deli '('


    // Ist es ein gueltiges Nebenwort ?
    i1 = UTX_cmp_word_wordtab (ObjCodTab, cmd);
    if (i1 >= 0) {
      // printf(" found SubCmd %d\n",i1);
      w_typ[w_anz] = Typ_cmdNCsub;
      w_tab[w_anz] = i1;
      goto Next;



    // check for PI
    } else if (!strcmp (cmd, "PI")) {
      d1 = RAD_180;
      goto L_fix_numVal;


    } else if (!strcmp (cmd, "EQ")) {
      w_typ[w_anz] = Typ_ope_eq;
      goto Next;


    } else if (!strcmp (cmd, "NE")) {
      w_typ[w_anz] = Typ_ope_ne;
      goto Next;


    } else if (!strcmp (cmd, "LT")) {
      w_typ[w_anz] = Typ_ope_lt;
      goto Next;


    } else if (!strcmp (cmd, "GT")) {
      w_typ[w_anz] = Typ_ope_gt;
      goto Next;


    } else if (!strcmp (cmd, "G_E")) {
      w_typ[w_anz] = Typ_ope_ge;
      goto Next;


    } else if (!strcmp (cmd, "L_E")) {
      w_typ[w_anz] = Typ_ope_le;
      goto Next;



    } else if (cmd[0] == 'D') {

      if(isdigit(cmd[1])) goto L_ck_obj;
      if(cmd[1] == '\0') goto L_ck_fnc;    // test for "D(0 0 1)"
      
      if (!strcmp (cmd, "DX")) {
        l1 = DB_VCX_IND;

      } else if (!strcmp (cmd, "DY")) {
        l1 = DB_VCY_IND;

      } else if (!strcmp (cmd, "DZ")) {
        l1 = DB_VCZ_IND;

      } else if (!strcmp (cmd, "DIX")) {
        l1 = DB_VCIX_IND;

      } else if (!strcmp (cmd, "DIY")) {
        l1 = DB_VCIY_IND;

      } else if (!strcmp (cmd, "DIZ")) {
        l1 = DB_VCIZ_IND;

      } else goto L_ck_fnc;    // is normal Text .. eg "DISP_PT"

      // l1 now is the index of a standardvector; if a UCS is active:
      // translate the standardvector into the UCS.
      if(AP_IS_2D) {
        vc1 = DB_GetVector (l1);
        UT3D_vc_tra_vc_m3 (&vc1, WC_sur_mat, &vc1);
        l1 = DB_StoreVector (-1L, &vc1);
      } 
      w_typ[w_anz] = Typ_VC;
      w_tab[w_anz] = l1;
      goto Next;



    } else if (cmd[0] == 'R') {

      if(cmd[1] == 'A') {
        if (!strcmp (cmd, "RAD_360")) {
          d1 = RAD_360;
          goto L_fix_numVal;
  
        } else if (!strcmp (cmd, "RAD_180")) {
          d1 = RAD_180;
          goto L_fix_numVal;
  
        } else if (!strcmp (cmd, "RAD_90")) {
          d1 = RAD_90;
          goto L_fix_numVal;
  
        } else if (!strcmp (cmd, "RAD_1")) {
          d1 = RAD_1;
          goto L_fix_numVal;
        }
      }

      if(cmd[1] == 'I') {
        if (!strcmp (cmd, "RIX")) {
          w_typ[w_anz] = Typ_PLN;
          w_tab[w_anz] = DB_PLIX_IND;
          goto Next;
  
        } else if (!strcmp (cmd, "RIY")) {
          w_typ[w_anz] = Typ_PLN;
          w_tab[w_anz] = DB_PLIY_IND;
          goto Next;
  
        } else if (!strcmp (cmd, "RIZ")) {
          w_typ[w_anz] = Typ_PLN;
          w_tab[w_anz] = DB_PLIZ_IND;
          goto Next;
        }
      }

      if (!strcmp (cmd, "RX")) {
        w_typ[w_anz] = Typ_PLN;
        w_tab[w_anz] = DB_PLX_IND;
        goto Next;

      } else if (!strcmp (cmd, "RY")) {
        w_typ[w_anz] = Typ_PLN;
        w_tab[w_anz] = DB_PLY_IND;
        goto Next;

      } else if (!strcmp (cmd, "RZ")) {
        w_typ[w_anz] = Typ_PLN;
        w_tab[w_anz] = DB_PLZ_IND;
        goto Next;
      }




    } else if (cmd[0] == 'S') {

      if (!strcmp (cmd, "SR_3")) {
        d1 = SR_3;
        goto L_fix_numVal;

      } else if (!strcmp (cmd, "SR_2")) {
        d1 = SR_2;
        goto L_fix_numVal;

      }
    }




    //------------------------------------------------------------------
    L_ck_fnc:
    // Text; ist es ein Function ? (zB SIN od eingebetteter Punkt (P(x,y,z) ..)
    // dazu muss der delimiter '(' sein !
    if(deli == '(') {                // 2006-02-26
      // i1 = APT_decode_func (&ityp, &d1, txtOut, w_next);
      w_siz = 200 - w_anz;
      i1 = APT_decode_func (&w_typ[w_anz], &w_tab[w_anz], w_siz, txtOut, w_next);
        // for(i1=0;i1<w_anz;++i1)
        // printf(" L_ck_fnc-typ[%d]=%d tab=%f\n",i1,w_typ[i1],w_tab[i1]);
      if (i1 > 0) {
        // den Text in der Klammer skippen
        w_next = UTX_pos_skipBrack1 (w_next);   // 2008-06-03
        ++w_next;
        w_anz += i1;
        goto Nex_1;
        // goto Fertig;

      } else {
        return -1;
      }
    }



    // Text; zB V12 - ist es ein Object ?
    L_ck_obj:
    i1 = APED_dbo_oid (&oTyp, &oInd, txtOut);
    if (i1 < 0) {

      // kein Objekt, also normaler Text.
      *aus_typ = Typ_Txt;
      // *aus_tab = (double)strlen(txtOut);
      //printf("   Text: |%s| len=%f\n",txtOut,*aus_tab);
      //printf("**** Unbekanntes Objekt: %s ****\n",txtOut);
      //os_beep ();
      //rc = -1;
      goto Fertig;
    }



    //------------------------------------------------------------------
    // wenn einem Object ein Klammerausdruck mit eckigen Klammern folgt:
    // Direktzuweisung.
    if(deli == '[') {

      // printf(" decode [..]\n");

      // den Text in der Klammer kopieren
      //w_next = UTX_cp_word_term (LineK, w_next, ']');
      // den starting [ nochmal holen
      while(*w_next != '[') {
        --w_next;
      }
      w_next = APT_cp_ausd (LineK, w_next, 256);

      // den Klammernausdruck auswerten (ein bis n Objekte)
      w = LineK;
      ++w; // den starting [ wieder weg
      // printf(" nun vor APT_decode_ausdr |%s|\n",w);

      k_anz = APT_decode_ausdr (k_typ, k_tab, 200, &w);
      if(k_anz < 0) goto Error;
 
      // und zuweisen
      i1 = APT_store_obj (&oTyp, &oInd, k_anz, k_typ, k_tab);
      if(i1 < 0) goto Error;

      // nun noch intern zuweisen
      //goto Next;

    }


      // normal das Object laden (aus DB auslesen ..)
      // printf(" decode oTyp=%d ind=%d\n",oTyp,oInd);

      switch (oTyp) {


        case Typ_VAR:
          w_typ[w_anz] = Typ_Val;
          w_tab[w_anz] = DB_GetVar (oInd);
          if(deli != ' ') {   // ebenso bei ctyp=TXT_Typ_num
            ++w_anz;
            w_typ[w_anz] = Typ_Int1;
            w_tab[w_anz] = deli;
          }
          goto Next;


        case Typ_VC:
          w_typ[w_anz] = Typ_VC;
          w_tab[w_anz] = (double)oInd;
          goto Next;


        case Typ_PT:
          w_typ[w_anz] = Typ_PT;
          w_tab[w_anz] = (double)oInd;
          goto Next;


        case Typ_LN:
          w_typ[w_anz] = Typ_LN;
          w_tab[w_anz] = (double)oInd;
          goto Next;


        case Typ_CI:
          w_typ[w_anz] = Typ_CI;
          w_tab[w_anz] = (double)oInd;
          goto Next;


        case Typ_CV:
          w_typ[w_anz] = Typ_CV;
          w_tab[w_anz] = (double)oInd;
          goto Next;


        case Typ_Angle:
          w_typ[w_anz] = Typ_Angle;
          w_tab[w_anz] = (double)oInd;
          goto Next;


        case Typ_G_Att:
          w_typ[w_anz] = Typ_G_Att;
          w_tab[w_anz] = (double)oInd;
          goto Next;


        case Typ_Tra:
          w_typ[w_anz] = Typ_Tra;
          w_tab[w_anz] = (double)oInd;
          goto Next;


        case Typ_Tool:
          w_typ[w_anz] = Typ_Tool;
          w_tab[w_anz] = (double)oInd;
          goto Next;


        case Typ_PLN:
          w_typ[w_anz] = Typ_PLN;
          w_tab[w_anz] = (double)oInd;
          goto Next;


        case Typ_SUR:
          w_typ[w_anz] = Typ_SUR;
          w_tab[w_anz] = (double)oInd;
          goto Next;


        case Typ_SOL:
          w_typ[w_anz] = Typ_SOL;
          w_tab[w_anz] = (double)oInd;
          goto Next;


        case Typ_Model:                   // M20
          w_typ[w_anz] = Typ_Model;
          w_tab[w_anz] = (double)oInd;
          goto Next;


        // case Typ_GTXT:                    // N20
        case Typ_Note:                    // N20
          // w_typ[w_anz] = Typ_GTXT;
          w_typ[w_anz] = Typ_Note;
          w_tab[w_anz] = (double)oInd;
          goto Next;


        case Typ_Activ:                   // I20
          w_typ[w_anz] = Typ_Activ;
          w_tab[w_anz] = (double)oInd;
          goto Next;


        case Typ_Joint:                   // J20
          w_typ[w_anz] = Typ_Joint;
          w_tab[w_anz] = (double)oInd;
          goto Next;



        default:
          //TX_Error("**** Fehler APT_decode_ausdr1 1 ****");
          TX_Error(" Unbekannte Funktion: %s ****",data);
      }


  //-----------------------------------------------------------------------
  } else if (ctyp == TXT_Typ_spec) {        // 1

    // if (deli == '(') {
    if (txtOut[0] == '(') {
      // eg *w="(3 + 2)"
      // den Text in der Klammer > LineK kopieren
      ++w;             // skip '('
      // w_next = UTX_cp_word_term (LineK, w, ')');
      w_next = UTX_cp_expr (LineK, w);
        // printf(" LineK=|%s| w_next=|%s|\n",LineK,w_next);


      // Klammer abarbeiten;
      w_siz = 200 - w_anz;
      i1 = APT_decode_ausdr1 (&w_typ[w_anz], &w_tab[w_anz], w_siz, LineK);
      if(i1 < 0) goto Error;
      // printf("   TXT_Typ_spec: w_anz=%d,%f\n",w_anz,w_tab[w_anz]);
      goto Next;



    } else {
      // wenn es ein Operator (* oder /) ist -
      // printf(" ope |%c| w_anz=%d\n",txtOut[0],w_anz);     
      // if(w_anz == 0) {
        // vorz = deli;
        w_typ[w_anz] = Typ_Int1;
        w_tab[w_anz] = txtOut[0];
        // deli = ' ';  // Minus macht minus ... ??
      // } else {
        //printf("vorz = |%c|\n",deli);
        // w_ope[w_anz-1] = deli;
      // }
      goto Next;
      // goto NunWert;

    }




  //-----------------------------------------------------------------------
  } else if (ctyp == TXT_Typ_EOF) {
    goto Auswerten;


  } else {

    TX_Error(" APT_decode_ausdr1 2 ****");
    goto Auswerten;

  }



  //-----------------------------------------------------------------------
  L_fix_numVal:
     // printf(" _num: w_anz=%d\n",w_anz);

    if(deli != ' ') {   // "4*2" liefert d1=4, deli='*', nxt="2"
      w_typ[w_anz] = Typ_Val;
      w_tab[w_anz] = d1;
      ++w_anz;
      w_typ[w_anz] = Typ_Int1;
      w_tab[w_anz] = deli;
      goto Next;
    }

    // zB "PI/6"   "-3-2"
    if(w_anz > 0) {
      if(w_typ[w_anz-1] != Typ_Int1) {  // if last object is not operator -
        w_typ[w_anz] = Typ_Int1;
        if(deli == ' ') {
          w_tab[w_anz] = '+';
        } else {
          w_tab[w_anz] = deli;
        }
        ++w_anz;
      }
    }

    w_typ[w_anz] = Typ_Val;
    w_tab[w_anz] = d1;
    goto Next;






  //-----------------------------------------------------
  Auswerten:

    // TEST
    // printf("Auswerten: %d\n",w_anz);
    // for(i1=0;i1<w_anz;++i1)
    // printf(" typ[%d]=%d tab=%f\n",i1,w_typ[i1],w_tab[i1]);
    // TEST END


  // evaluate group
  if(w_typ[0] == Typ_Group) {
    irc = w_tab[0] + 1;   // Anzahl
    for(i1=0; i1<=irc; ++i1) {
      aus_typ[i1] = w_typ[i1];
      aus_tab[i1] = w_tab[i1];
    }
    goto Fertig;
  }


  if(vorz == '-') w_tab[0] = -w_tab[0];

  if(w_anz > 1) {
    // mehr als 1 objekt:
    // math operationen auswerten: es muss ein Ausdruck erzeugt werden
    i1 = APT_decode_comp1 (w_typ, w_tab, w_anz);
    if(i1 < 1) {
      TX_Error ("*** expression may have numeric values only ***");
      return -1;
    }
  }

  *aus_typ = w_typ[0];
  *aus_tab = w_tab[0];


  Fertig:
    // printf("ex APT_decode_ausdr1 irc=%d %d %f\n",irc,*aus_typ,*aus_tab);
  return irc;


  Error:
    TX_Print("ex APT_decode_ausdr1 E001");
    return -1;


  L_err2:
    TX_Print("ex APT_decode_ausdr1 E002");
    return -1;

}
  

//================================================================
  int APT_decode_func (int* w_typ, double* w_tab, int w_siz,
                       char* func, char* ausd) {
//================================================================
// TODO: replace with ATO_ato_srcLn__
// 
// Eine Function abarbeiten und das Ergebnis retournieren.
// Der Returncode ist die Anzahl retournierter Wert in w_typ/w_tab.
// 
// Eine Function ist: "<name>(<Ausdruck>)"
// 
// Wird rekursiv verwendet, daher lokale aus_typ, aus_tab.
// 
// Does store objects in dynamic area and returns the dynamic DB-index.
// 
// Inp:
//   func = FunctionName; zB "D"  oder "COS" od "sqrt"
//   ausd = der Funktionstext ohne starting "(" aber MIT closing ")"
//          also zB "1 0 9)" oder "V34)"
//   w_siz         size of tables w_typ and w_tab
// Out:
//   w_typ,w_tab   a single atomicObject (type, value);
//   RetCod        nr of objects; 1; Typ_Group: nr of objects in Grp.
// 
// MEM:
//   memspc101 f temp BSP-Curves
// 
// see also SRC_typ_FncNam
// 
// .. 

  static Point     pt_old;


  int      i1, i2, irc;
  // int      typ1, tab1;
  long     l1;
  double   d1;
  char     LineF[10000], *w_next, *w, funcU[8];


  int       aus_anz;
  int       aus_typ[1000];
  double    aus_tab[1000];
  Point     pt1;
  Line      ln1;
  Circ      ci1;
  Vector    vc1;
  Plane     pl1;
  ObjGX     ox1;
  Memspc    workSeg;


  // printf("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF\n");
  // printf("APT_decode_func |%s|%s| %d\n",func,ausd,w_siz);


  irc = 1;

  UTX_cp_word_2_upper (funcU, func);

/// .. 
// .. for (i1=0; i1<Cmd0Anz; i1++) {
// ..   //printf("vergl %d !%s!%s!\n",i1,*(&Cmd0Tab[i1]),func);
// ..   if (!strcmp(*(&Cmd0Tab[i1]),funcU)) {
// .. 
// ..     goto WorkFunc;
// ..   }
// .. }
// .. 
// .. 
// .. *w_typ = Typ_Error;
// .. rc = -1;
// .. goto Fertig;
// .. 



  //===========================================================
  WorkFunc:
    // printf("  funcU |%s|\n",funcU);






  //===========================================================
  // den Text in der Klammer > LineF kopieren
  // w_next = UTX_cp_word_term (LineF, ausd, ')');
  // w = strchr(ausd, ')');
  w = UTX_pos_skipBrack1 (ausd);


  if(w == NULL) {TX_Error("APT_decode_func E2002"); return -1;}
  // i1 = w - ausd + 1;
  i1 = w - ausd;                 // ohne closing ")"  2008-06-03
  strncpy(LineF, ausd, i1);
  L_skipfb:  // skip following blanks
  LineF[i1] = '\0';
  --i1;
  if(LineF[i1] == ' ') goto L_skipfb;

    // printf(" _decode_func LineF=|%s|\n",LineF);





  // einfache Funktionen ohne APT_decode_ausdr abarbeiten (schneller)
  if(!strcmp(funcU, "MOD")) {
    // printf(" work mod ..\n");
    // *w_typ = Typ_cmdNCsub;
    // *w_tab = 42;
    *w_typ = Typ_modif;
    i1 = APT_decode_mod (w_tab, LineF);
    if(i1 < 0) goto ParErr;
    goto Fertig;
  }

/// .. 
// .. if(!strcmp(funcU, "NEW")) {
// ..   // printf(" ********* NEW |%s| *********\n",LineF);
// ..   *w_typ = Typ_Val;
// ..   i1 = DB_QueryNew (LineF);
// ..   *w_tab = i1;
// ..   goto Fertig;
// .. }
// .. 


  // nun den Klammerausdruck abarbeiten; der kann aber mehrere
  // Werte liefern!
  w = LineF;
  aus_anz = APT_decode_ausdr (aus_typ, aus_tab, 1000, &w);
  if(aus_anz < 0) goto Error;


/// .. 
// .. //=== Testausg: ==================================================
// .. printf(" in APT_decode_func aus_anz=%d\n",aus_anz);
// .. for(i1=0; i1<aus_anz; ++i1) {
// ..   printf("  Funpar: %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
// .. }
// .. //=== Testausg: ==================================================
// .. 


  if(!strcmp(funcU, "VAL")) {
    i1 = APT_decode_var (w_tab, aus_anz, aus_typ, aus_tab);
    if(i1 < 0) goto ParErr;

    *w_typ = Typ_Val;



  } else if(!strcmp(funcU, "SQRT")) {
    //printf("SQRT %f\n",aus_tab[0]);
    *w_typ = Typ_Val;
    *w_tab = sqrt(aus_tab[0]);

  
  } else if(!strcmp(funcU, "SIN")) {
    //printf("SQRT %f\n",aus_tab[0]);
    *w_typ = Typ_Val;
    *w_tab = sin(aus_tab[0]);

  
  } else if(!strcmp(funcU, "COS")) {
      printf("COS %f\n",aus_tab[0]);
    *w_typ = Typ_Val;
    *w_tab = cos(aus_tab[0]);

  
  } else if(!strcmp(funcU, "TAN")) {
    //printf("SQRT %f\n",aus_tab[0]);
    *w_typ = Typ_Val;
    *w_tab = tan(aus_tab[0]);

  
  } else if(!strcmp(funcU, "ASIN")) {
    //printf("SQRT %f\n",aus_tab[0]);
    *w_typ = Typ_Val;
    *w_tab = asin(aus_tab[0]);


  } else if(!strcmp(funcU, "ACOS")) {
    //printf("SQRT %f\n",aus_tab[0]);
    *w_typ = Typ_Val;
    *w_tab = acos(aus_tab[0]);


  } else if(!strcmp(funcU, "ATAN")) {
    //printf("SQRT %f\n",aus_tab[0]);
    *w_typ = Typ_Val;
    *w_tab = atan(aus_tab[0]);

  
  } else if(!strcmp(funcU, "ABS")) {
    //printf("SQRT %f\n",aus_tab[0]);
    *w_typ = Typ_Val;
    *w_tab = fabs(aus_tab[0]);

  
  } else if(!strcmp(funcU, "FIX")) {
    //printf(" %f %d %f\n",aus_tab[0],l1,*w_tab);
    *w_typ = Typ_Val;
    l1 = (int)(aus_tab[0]);
    *w_tab = (double)l1;

  
  } else if(!strcmp(funcU, "RND")) {
    //printf("SQRT %f\n",aus_tab[0]);
    *w_typ = Typ_Val;
    d1 = 0.5;
    if(aus_tab[0] < 0.0) d1 = -0.5;
    l1 = (int)(aus_tab[0] + d1);
    *w_tab = (double)l1;

  
  } else if(!strcmp(funcU, "X")) {
    *w_typ = Typ_XVal;
    i1 = APT_decode_xyzval (w_tab, aus_anz, aus_typ, aus_tab, w_typ);
    if(i1 < 0) goto ParErr;

  } else if(!strcmp(funcU, "Y")) {
    *w_typ = Typ_YVal;
    i1 = APT_decode_xyzval (w_tab, aus_anz, aus_typ, aus_tab, w_typ);
    if(i1 < 0) goto ParErr;

  } else if(!strcmp(funcU, "Z")) {
    *w_typ = Typ_ZVal;
    i1 = APT_decode_xyzval (w_tab, aus_anz, aus_typ, aus_tab, w_typ);
    if(i1 < 0) goto ParErr;


  } else if(!strcmp(funcU, "ANG")) {
    // printf("Angle %d %f\n",aus_typ[0],aus_tab[0]);
    i1 = APT_decode_angd__ (w_tab, aus_anz, aus_typ, aus_tab);
    if(i1 < 0) goto ParErr;
    *w_typ = Typ_Angle;


  } else if(!strcmp(funcU, "RAD")) {
    // printf("RAD %f\n",aus_tab[0]);
    *w_typ = Typ_Rad;
    *w_tab = aus_tab[0];


  } else if(!strcmp(funcU, "DIST")) {
    // printf("DIST %f\n",aus_tab[0]);
    *w_typ = Typ_Dist;
    *w_tab = aus_tab[0];




  //===========================================================
  } else if(!strcmp(funcU, "P")) {

    // i1 = APT_decode_pt3 (&pt1, aus_anz, aus_typ, aus_tab);
    i1 = APT_decode_pt (&pt1, aus_anz, aus_typ, aus_tab);
    if(i1 < 0) goto ParErr;

    pt_old = pt1;
    *w_typ = Typ_PT;
    l1 = DB_StorePoint (-1L, &pt1);
    *w_tab = l1;





  //===========================================================
  } else if(!strcmp(funcU, "L")) {

    i1 = APT_decode_ln (&ln1, aus_anz, aus_typ, aus_tab);
    if(i1 < 0) goto ParErr;

    pt_old = ln1.p2;
    *w_typ = Typ_LN;
    l1 = DB_StoreLine (-1L, &ln1);
    *w_tab = l1;





  //===========================================================
  } else if(!strcmp(funcU, "C")) {

    // siehe auch APT_wcut_line 
    // rc = APT_decode_ci1 (&ci1, &pt_old, aus_anz, aus_typ, aus_tab);
    // APT_decode_ci1 kann kein embedded; 2005-12-22 raus. pt_old ????
    i1 = APT_decode_ci (&ci1, aus_anz, aus_typ, aus_tab);
    if(i1 < 0) goto ParErr;

    *w_typ = Typ_CI;
    l1 = DB_StoreCirc (-1L, &ci1);
    *w_tab = l1;





  //===========================================================
  } else if(!strcmp(funcU, "D")) {          // Direction - Vector

    i1 = APT_decode_vc (&vc1, aus_anz, aus_typ, aus_tab);
    if(i1 < 0) goto ParErr;

    *w_typ = Typ_VC;
    l1 = DB_StoreVector (-1L, &vc1);
    *w_tab = l1;



  //===========================================================
  } else if(!strcmp(funcU, "R")) {           // Plane

    i1 = APT_decode_pln1 (&pl1, aus_anz, aus_typ, aus_tab);
    if(i1 < 0) goto ParErr;

    *w_typ = Typ_PLN;
    l1 = DB_StoreRef (-1L, &pl1);
    *w_tab = l1;



  //===========================================================
  } else if(!strcmp(funcU, "S")) {             // Curve

    i1 = APT_decode_cv (&ox1, &i2, aus_anz, aus_typ, aus_tab);
    if(i1 < 0) goto ParErr;

    // *w_typ = ox1.typ;
    *w_typ = Typ_CV;

    l1 = DB_StoreCurv (-1L, &ox1, i2);
    *w_tab = l1;



  //===========================================================
  } else if(!strcmp(funcU, "U")) {             // Union - Group
    // add all objekts of group into table;
    // the first record is "Typ_Group" / <Nr of following objects>.

    w_typ[0] = Typ_Group;
    w_tab[0] = aus_anz;       // Anzahl

      
    // printf(" APT_decode_func - U %d\n",aus_anz);
    // copy the local table up
    if(aus_anz >= w_siz) {TX_Error("APT_decode_func E001"); return -1;}
    for(i1=0; i1<aus_anz; ++i1) {
      w_typ[i1+1] = aus_typ[i1];
      w_tab[i1+1] = aus_tab[i1];
        // printf(" group[%d]-typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
    }
    irc = aus_anz+1;


/// .. 
// .. //===========================================================
// .. } else if(!strcmp(funcU, "V")) {             // Value
// .. 
// ..   printf(" ************ VALUE ***********\n");
// .. 
// ..   *w_typ = Typ_Val;
// ..   *w_tab = 123.45;
// .. 




  //===========================================================
  } else {
    TX_Error("APT_decode_func: Function %s not supported",funcU);
    *w_typ = Typ_Error;
  }




  Fertig:
    // printf("exit APT_decode_func irc=%d typ=%d %f\n",irc,*w_typ,*w_tab);
    // printf("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF\n");
  return irc;



  ParErr:
  TX_Error(" Parameter Error: %s",func);
  // TX_Error(" Parameter Error: %s(%s ****",func,ausd);

  Error:
  //TX_Error("ex  Parameter Error: %s(%s ****",func,ausd);
  return -1;

}
*/


/*==========================================================================*/
  int APT_store_obj (int *eTyp, long *eInd,
                     int aus_anz, int* aus_typ, double* aus_tab) {
/*==========================================================================*/
/// \code
/// Ein Objekt eTyp aus den Parametern aus_typ/aus_tab erzeugen;
///   Objekt in der DB speichern.
/// MemoryUsage:
///   Die decode-Funktionen benutzen die globalen Memspc APTSpcObj,APTSpcTmp.
///   Sind nach DB-speichern wieder frei.
/// Input:
///   eTyp eInd            ErgebnisTyp u DB-Index 
///   aus_typ[] aus_tab[]  die decodierten Eingabeparameter
///                        if(!aus_anz) return typ and ind of last processed obj
/// Output:
///   Retcode -1: Error;
///           -2: do not display (yet);
///           -3  object not yet complete;
///
/// see also APT_obj_ato
/// \endcode

// TODO: change Att_ln -> Ind_Att_ln



  static int  defTyp;
  static long defInd;

  int       i1, oTyp, iNew, iCmd;
  long      dbi, dli, oSiz;
  double    d1;
  char      obj1[OBJ_SIZ_MAX];
  Vector    vc1;
  Point     pt1;
  Line      ln1;
  Circ      ci1;
  // Point2Tab cvpol2;
  // PointTab  cvpol;
  // CurvPol3  cvpsp3;
  // CurvBSpl  cvbsp;
  // Curv2CCV  cvccv2;
  Att_ln    att1;
  Plane     pln1;
  // AText     atx1;
  GText     gtx1;
  Dimen     dim1;
  Dim3      dim3;
  ObjGX     ox1, *oxp1;
  Memspc    tbuf1;

  char      *cp1, *cp2, *cp3; // temp test




  // printf("APT_store_obj typ=%d ind=%ld aus_anz=%d\n",*eTyp,*eInd,aus_anz);
  // printf("  APT_obj_stat=%d\n",APT_obj_stat);          // 0=perm; 1=temp
  // printf("  IE_outTxt=|%s|\n",IE_outTxt);
  // for(i1=0;i1<aus_anz; ++i1) printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);



  // testen, ob noch Platz in DB
  if(DB_CSEG_ck() < 0) return -1;

  // DB_CSEG__ (APT_obj_stat);  // reset DB_CSEG


  if(!aus_anz) {
      // printf(" query defTyp=%d defInd=%ld\n",defTyp,defInd);
    *eTyp = defTyp;
    *eInd = defInd;
    return 0;
  }


  defTyp = *eTyp;
  defInd = *eInd;
    // printf(" set defTyp=%d defInd=%ld\n",defTyp,defInd);


  iNew = 0;                 // def; save data.


  // prepare Workspaces;
  // this memspace can be accessed directly (must not be defined in the
  // function-interfaces);
  // This memSpc can be used recursive, since you may only use UME_add,
  // not UME_init again !
  // APTSpcObj f. das Ausgabeobjekt und seine Bestandteile
  // APTSpcTmp f. temporaere Berechnungen
  UME_init (&APTSpcTmp, memspc501, sizeof(memspc501));
  UME_init (&APTSpcObj, memspc201, sizeof(memspc201));


  // Test for modify-function
  if(aus_typ[0] == Typ_cmdNCsub) {

    //----------------------------------------------------------------
    if((int)aus_tab[0] == T_CUT) {      // trim
      i1 = APT_CUT__ (&ox1, &APTSpcObj, aus_anz, aus_typ, aus_tab, &APTSpcTmp);
      if(i1 == -3) goto L_notYetComplete;  // not yet complete
      if(i1 < 0) return -1;

    //----------------------------------------------------------------
    } else if((int)aus_tab[0] == T_INT) {   // intersect
      i1 = APT_INT__ (&ox1, &APTSpcObj, aus_anz, aus_typ, aus_tab, &APTSpcTmp);
        // printf("after APT_INT__: %d %d\n",i1,APT_modMax1);
      if(i1 < 0) return -1;
      if(APT_modMax1 < 1) 
        TX_Print("***** no solution for %s - %ld ..",AP_src_typ__(*eTyp),*eInd);

    //----------------------------------------------------------------
    } else if((int)aus_tab[0] == T_TRA) {   // S = TRA S
      oSiz = OBJ_SIZ_MAX;
      i1 = APT_tra_obj (&ox1, &APTSpcObj, aus_anz, aus_typ, aus_tab, &APTSpcTmp);
      if(i1 < 0) return -1;

    //----------------------------------------------------------------
    } else if((int)aus_tab[0] == T_IMP) {   // Import joints
      i1 = APT_IMP__ (&APTSpcObj, aus_anz, aus_typ, aus_tab, &APTSpcTmp);
      if(i1 < 0) return -1;
      // copy first obj from APTSpcObj -> ox1
      memcpy(&ox1, UME_get_start(&APTSpcObj), sizeof(ObjGX));

    //----------------------------------------------------------------
    } else if((int)aus_tab[0] == T_MIR) {    // mirror obj's
      i1 = APT_mir_obj (&ox1, aus_anz, aus_typ, aus_tab, &APTSpcTmp);
      if(i1 < 0) return -1;
      // goto L_sub_store;

    //----------------------------------------------------------------
    } else if((int)aus_tab[0] == T_TNG) {    // tangential line
      i1 = APT_TNG__ (&ox1, aus_anz, aus_typ, aus_tab, &APTSpcTmp);
        // DEB_dump_obj__ (ox1.form, obj1, "ex APT_TNG__");
      if(i1 < 0) return -1;
      // goto L_sub_store;


    //----------------------------------------------------------------
    } else if((int)aus_tab[0] == T_BLEND) {     // TODO !
      i1 = APT_BLEND__ (&ox1, aus_anz, aus_typ, aus_tab, &APTSpcTmp);
        // DEB_dump_obj__ (ox1.form, obj1, "ex APT_TNG__");
      if(i1 < 0) return -1;
      // goto L_sub_store;

    //----------------------------------------------------------------
    } else if((int)aus_tab[0] == T_PRJ) { // project normal or in direction vec
      i1 = APT_prj_obj_perp (&ox1, aus_anz, aus_typ, aus_tab, &APTSpcTmp);
        // printf(" APT_modMax1=%d\n",APT_modMax1);
      if(i1 < 0) return -1;

    //----------------------------------------------------------------
    } else if((int)aus_tab[0] == T_PARL) { // offset-curve or surface
      i1 = APT_PARL__ (&ox1, &APTSpcObj, aus_anz, aus_typ, aus_tab, &APTSpcTmp);
        // printf(" APT_modMax1=%d\n",APT_modMax1);
      if(i1 < 0) return -1;

    //----------------------------------------------------------------
    } else if((int)aus_tab[0] == T_REV) {  // reverse obj
      i1 = APT_REV__ (&ox1, &APTSpcObj, aus_anz, aus_typ, aus_tab);
        // printf(" APT_modMax1=%d\n",APT_modMax1);
      if(i1 < 0) return -1;

    //----------------------------------------------------------------
    } else goto L_work1;

      // resulting object now ox1
        // DEB_dump_obj__ (Typ_ObjGX, &ox1, "_store_obj-ox1");

      // retrieve obj
      *eTyp = AP_typDB_typ (ox1.typ);   // Typ_CVBSP -> Typ_CV.
        // printf(" _store_obj-eTyp=%d defTyp=%d defInd=%ld\n",*eTyp,defTyp,defInd);
        // DEB_dump_ox_0 (&ox1, "store_obj-ox1-1");
        // printf(" IE_outTxt =|%s|\n",IE_outTxt);


      // test if created objType *eTyp == predefined objTyp defTyp
      if(*eTyp != defTyp) {
        // other output-form than IE_cad_typ,IE_objInd:
        defTyp = *eTyp;
        // get next free DB-index for type *eTyp
        i1 = AP_typDB_typ (defTyp);   // get basic-type
        *eInd = DB_QueryNxtFree (i1, 20);
        defInd = *eInd;
        // change header of definition-code in IE_outTxt
        if(UI_InpMode == UI_MODE_CAD) IE_cad_upd_hdr (defTyp, defInd);
      }


      // make DB-struct obj1 from ox1
      UTO_dbs_ox (obj1, &i1, &ox1);
        // DEB_dump_obj__ (i1, obj1, "ex APT_INT|CUT|PRJ");

      // do not save aux-data (polygon/knots ..)
      iNew = 2;


      L_sub_store:    // store outputObj
      if     (*eTyp == Typ_PT)    goto L_PT_sav;          // obj1
      else if(*eTyp == Typ_LN)    goto L_LN_sav;          // obj1
      else if(*eTyp == Typ_CI)    goto L_AC_sav;          // obj1
      else if(*eTyp == Typ_CV)    goto L_CV_sav;          // obj1
      else if(*eTyp == Typ_PLN)   goto L_PLN_sav;         // obj
      else if(*eTyp == Typ_SUR)   goto L_SUR_sav;         // obj
      else if(*eTyp == Typ_SOL)   goto L_SOL_sav;         // obj
      else if(*eTyp == Typ_Model) goto L_Mdl_sav;         // ox1

      else if(*eTyp == Typ_Note) {
        if(ox1.form == Typ_GTXT) {
          DB_StoreGTxt (defInd, (GText*)obj1);
          goto Fertig;
        }
      } else {
        TX_Error("APT_store_obj eTyp_%d",*eTyp);
        return -1;
      }

        // printf(" cut/int/imp: typ=%d form=%d\n",ox1.typ,ox1.form);

/*
    // APT_int_obj: uses memspc501 for additional data
    i1 = APT_int_obj (eTyp, obj1, &oSiz, &aus_anz, aus_typ, aus_tab);
    if(i1 < 0) return -1;
    if(i1 > 0) goto L_work1;   // 2008-10-20
  
    // store --> DB
    APT_obj_typ = *eTyp; // wurde ev in APT_int_obj korrigiert !
    // if     (*eTyp == Typ_PT) aus_anz = i1;
    if     (*eTyp == Typ_LN) goto L_LN_sav;
    else if(*eTyp == Typ_CI) goto L_AC_sav;
    else if(*eTyp == Typ_CV) { ox1 = *(ObjGX*)obj1; goto L_CV_sav; }
*/

    // goto Fertig;

  }


  // obj not yet created; (parameters prepared)
  L_work1:
  oTyp = *eTyp;
  // oInd = *eInd;

    // for(i1=0;i1<aus_anz;++i1)
    // printf(" aus_typ[%d] %d aus_tab %f\n",i1,aus_typ[i1],aus_tab[i1]);
    // printf(" APT_store_obj oTyp=%d oInd=%d\n",oTyp,oInd);




  switch (oTyp) {


    /*
    //---------------------------------------------------------------
    case Typ_Txt:                     // zB $20=..
      DB_StoreTxt (defInd, *APT_spc1);
      break;
    */


    //---------------------------------------------------------------
    case Typ_Note:                    // N20=..
        // printf(" Typ_Note- %d\n",(int)aus_tab[0]);
      if((int)aus_tab[0] == T_IMG)  {
        i1 = APT_decode_img ((AText*)obj1, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;
        DB_StoreATxt (defInd, (AText*)obj1);

      } else if((int)aus_tab[0] == T_TAG){
        i1 = APT_decode_tag ((AText*)obj1, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;
        DB_StoreATxt (defInd, (AText*)obj1);

      } else if((int)aus_tab[0] == T_DIM) {
        i1 = APT_decode_dimen (&dim1, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;
        DB_StoreDim_ (defInd, &dim1);

      } else if((int)aus_tab[0] == T_DIMA){
        i1 = APT_decode_dima (&dim1, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;
        DB_StoreDim_ (defInd, &dim1);

      } else if(((int)aus_tab[0] == T_DIMD) ||
                ((int)aus_tab[0] == T_DIMR))   {
        i1 = APT_decode_dimdia (&dim1, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;
        DB_StoreDim_ (defInd, &dim1);

      } else if((int)aus_tab[0] == T_DIM3){
        i1 = APT_decode_dim3 (&dim3, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;
        DB_StoreDim3 (defInd, &dim3);

      } else if((int)aus_tab[0] == T_LDRP){
        i1 = APT_decode_ldrp ((AText*)obj1, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;
        DB_StoreATxt (defInd, (AText*)obj1);

      } else if((int)aus_tab[0] == T_LDR){
        i1 = APT_decode_ldr_ (&dim1, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;
        DB_StoreDim_ (defInd, &dim1);

      } else if((int)aus_tab[0] == T_LDRC){
        i1 = APT_decode_ldrc ((AText*)obj1, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;
        DB_StoreATxt (defInd, (AText*)obj1);

      } else if((int)aus_tab[0] == T_LDRS){
        i1 = APT_decode_ldrs ((AText*)obj1, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;
        DB_StoreATxt (defInd, (AText*)obj1);

      } else {
        i1 = APT_decode_gtxt (&gtx1, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;
        DB_StoreGTxt (defInd, &gtx1);
      }

      break;


    //---------------------------------------------------------------
    case Typ_VAR:

      i1 = APT_decode_var (&d1, aus_anz, aus_typ, aus_tab);
      //TX_Print ("Store Var %d %f",defInd, d1);
      if(i1 < 0) goto Problem1;

      // if(APT_obj_stat == 0) {
        // must store also for "CALL CTLG" and submodel calls .. 2012-03-28
        DB_StoreVar (defInd, d1);

      // } else {
        // if(AP_mode__ == AP_mode_enter) {
          // UI_disp_var1 (&d1);
          // if(!SRCU_ck_act__()) {
            // TX_Print("... value[%d] is %lf",defInd,d1);
          // }
        // } 
      // }

      // exit without draw (no attributes)
      return -2;



    //---------------------------------------------------------------
    case Typ_VC:    // Vektor IMMER 3D

      i1 = APT_decode_vc (&vc1, aus_anz, aus_typ, aus_tab);
        // printf ("_store D%ld i1=%d\n",defInd,i1);
      if(i1 == -3) goto L_notYetComplete;  // not yet complete
      if(i1 < 0) goto Problem1;

      // if(APT_obj_stat == 0) {
        // store perm.vec
        DB_StoreVector (defInd, &vc1);

        // // create a dummy DL-record
        // GR_create_dummy (defTyp, defInd);
      // }

        // // printf(" AP_mode__ =%d\n",AP_mode__);
      // if(AP_mode__ == AP_mode_enter) {
        // UI_disp_vec1 (Typ_VC, (void*)&vc1, NULL); // 2013-10-11
      // }

      // exit without draw (no attributes)
      return -2;    
      // break;



    //---------------------------------------------------------------
    case Typ_PT:

      i1 = APT_decode_pt ((Point*)obj1, aus_anz, aus_typ, aus_tab);
      if(i1 == -3) goto L_notYetComplete;  // not yet complete
      if(i1 < 0) goto Problem1;
      // }

      L_PT_sav:
      DB_StorePoint (defInd, (Point*)obj1);

      break;



    //---------------------------------------------------------------
    case Typ_LN:

      if(aus_typ[0] == Typ_cmdNCsub)  {
        iCmd = aus_tab[0];

        if(iCmd == T_ISO) {
          i1 = APT_iso_obj (obj1, aus_anz, aus_typ, aus_tab, &APTSpcTmp);
          if(i1 < 0) return -1;

        } else goto Problem2;
        goto L_LN_sav;
      }


      i1 = APT_decode_ln ((Line*)obj1, aus_anz, aus_typ, aus_tab);
      // DB_dyn__ (1, Typ_PT, 0L);   // reset to previous saved
      if(i1 < 0) goto Problem0;

      L_LN_sav:
      DB_StoreLine (defInd, (Line*)obj1);

      break;



    //---------------------------------------------------------------
    case Typ_CI:

      if(aus_typ[0] == Typ_cmdNCsub)  {
        iCmd = aus_tab[0];

        if       (iCmd == T_ARC) {
          goto L_AC_dec;

        } else if(iCmd == T_ARC1) {
          goto L_AC_dec;

        } else if(iCmd == T_ISO) {
          i1 = APT_iso_obj (obj1, aus_anz, aus_typ, aus_tab, NULL);
          if(i1 < 0) return -1;

        } else goto Problem2;     // ARC, ARC1
        goto L_AC_sav;
      }

      // resolve ohne & mit "ARC|ARC1"
      L_AC_dec:
      i1 = APT_decode_ci ((Circ*)obj1, aus_anz, aus_typ, aus_tab);
      if(i1 < 0) goto Problem1;

      L_AC_sav:
      DB_StoreCirc (defInd, (Circ*)obj1);

      break;



    //---------------------------------------------------------------
    case Typ_CV:  // from "S"
        // printf(" APT_store_obj - Curve - typ=%f\n",aus_tab[0]);

      if(aus_typ[0] == Typ_cmdNCsub)  {
      iCmd = aus_tab[0];
        // printf(" _store_obj curve NCsub %d\n",iCmd);

      if(iCmd == T_ISO) {
        i1 = APT_iso_obj (obj1, aus_anz, aus_typ, aus_tab, NULL);
        if(i1 < 0) return -1;

      } else if(iCmd == T_POL2) {
        i1 = APT_decode_pol2 ((ObjGX*)obj1, &APTSpcTmp, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;

      } else if(iCmd == T_POL) {
        i1 = APT_decode_pol ((ObjGX*)obj1, &APTSpcTmp, aus_anz, aus_typ, aus_tab);
        if(i1 == 1) { DL_Redraw (); return -1; }     // nur 1. Punkt
        if(i1 < 0) goto Problem1;

      } else if(iCmd == T_REC) {
        i1 = APT_decode_rec ((ObjGX*)obj1, &APTSpcTmp, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;

      } else if(iCmd == T_ELL) {
        i1 = APT_decode_ell ((ObjGX*)obj1, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;

      } else if(iCmd == T_CLOT) {
        i1 = APT_decode_clot ((ObjGX*)obj1, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;

      } else if(iCmd == T_PSP3) {
        i1 = APT_decode_psp3 ((ObjGX*)obj1, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;

      } else if(iCmd == T_BSP0) {
        i1 = APT_decode_bsp_ ((ObjGX*)obj1, &APTSpcObj, &APTSpcTmp,
                             aus_anz, aus_typ, aus_tab);
        // DB_dump_dyn__ ();
        // DB_dyn__ (1, Typ_PT, 0L);   // reset to previous saved
        if(i1 == 1) { DL_Redraw (); return -1; }     // nur 1. Punkt
        if(i1 < 0) goto Problem1;

      } else if(iCmd == T_BSP1) {
        i1 = APT_decode_bsp1 ((ObjGX*)obj1, &APTSpcObj, &APTSpcTmp,
                             aus_anz, aus_typ, aus_tab);
        if(i1 == 1) { DL_Redraw (); return -1; }     // nur 1. Punkt
        if(i1 < 0) goto Problem1;

      } else if(iCmd == T_RBSP) {
        i1 = APT_decode_rbsp ((ObjGX*)obj1, aus_anz, aus_typ, aus_tab);
        if(i1 == 1) return -1;     // nur 1. Punkt
        if(i1 < 0) goto Problem1;

      } else if(iCmd == T_CCV) {
        i1 = APT_decode_ccv__ ((void*)obj1, &APTSpcObj, &APTSpcTmp,
                               aus_anz, aus_typ, aus_tab);
        if(i1 == 1) { DL_Redraw (); return -1; }     // nur 1. Punkt
        if(i1 < 0) goto Problem1;

      } else if(iCmd == T_CCV2) {
        i1 = APT_decode_ccv2 ((ObjGX*)obj1, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;

      } else if(iCmd == T_MSH) {
        i1 = APT_decode_ltab ((ObjGX*)obj1, &APTSpcObj,
                              aus_anz, aus_typ, aus_tab);
        if(i1 == 1) { DL_Redraw (); return -1; }     // nur 1. Punkt
        if(i1 < 0) goto Problem1;


      } else goto Problem2;


      // } else if((int)aus_typ[0] == Typ_CV) { // schon fertig (copy ..)
      } else {
        i1 = APT_decode_cv ((ObjGX*)obj1, &iNew, aus_anz, aus_typ, aus_tab);
        if(i1 < 0) goto Problem1;

      // } else goto Problem2;
      }


      L_CV_sav:
        // DEB_dump_ox_0 (obj1, "S[%d]:",defInd);
        // DEB_dump_obj__ (Typ_ObjGX, obj1, "APT_store_obj-8");
      i1 = DB_StoreCurv (defInd, (ObjGX*)obj1, iNew);
      if(i1 < 0) goto Problem1;

      break;




    //---------------------------------------------------------------
    case Typ_G_Att:
      i1 = APT_decode_att (&att1, aus_anz, aus_typ, aus_tab);
      if(i1 < 0) goto Problem1;

      // DL_StoreAtt (defInd, &att1);
      DL_AttLn_store ((int)defInd, &att1);
      GL_InitNewAtt (0, (int)defInd);
      break;




    //---------------------------------------------------------------
    case Typ_PLN:

      if(aus_typ[0] == Typ_cmdNCsub)  {

        if((int)aus_tab[0] == T_RSYS) {   // R = RSYS ..
          i1 = APT_decode_pln_rsys ((Plane*)obj1, aus_anz, aus_typ, aus_tab);
          goto L_PLN_sav;
        }

      }
      i1 = APT_decode_pln1 ((Plane*)obj1, aus_anz, aus_typ, aus_tab);
      // DEB_dump_ox_0 (&ox1, "T[%d]:",defInd);
 
      L_PLN_sav:
      if(i1 < 0) goto Problem1;
      DB_StoreRef (defInd, (Plane*)obj1);
      break;



    //---------------------------------------------------------------
    case Typ_SUR:

      if(aus_typ[0] == Typ_cmdNCsub) {

      iCmd = aus_tab[0];
        // printf(" Typ_SUR: iCmd=%d\n",iCmd);

        if(iCmd == T_BSP0) {
          UME_init (&tbuf1, mem_cbuf1, mem_cbuf1_SIZ); // 2014-01-23
          i1 = APT_decode_sbsp (&ox1, &APTSpcObj, &APTSpcTmp, &tbuf1,
                                aus_anz,aus_typ,aus_tab);
          if(i1 == -99) {
            if(APT_obj_stat == 0) {
              TX_Error("BsplSur - curves do not end on surface - boundaries");
            } else {
              APT_set_modMax (0);
            }
            return -1;
          }
          if(i1 == 1) return -1;
          if(i1 < 0) return -1;
          goto L_SUR_sav;


        } else if(iCmd == T_PTAB) {
          i1 = APT_decode_msh_p (&ox1, defInd, aus_anz, aus_typ, aus_tab);
          if(i1 < 0) return -1;
          goto L_SUR_sav;


        } else if(iCmd == T_MSH) {
          i1 = APT_decode_msh__ (&ox1, aus_anz, aus_typ, aus_tab);
          if(i1 < 0) return -1;
          goto L_SUR_sav;


        }
      }

      i1 = APT_decode_sur (&ox1, aus_anz, aus_typ, aus_tab, &APTSpcTmp);
      if(i1 == -2) {   // nur Display (zB WCUT)
        DB_StoreDummy (Typ_SUR, defInd);   // nur den Platz besetzen;
        return i1;
      }
      if(i1 < 0) goto Problem1;


      L_SUR_sav:
      dbi = defInd;
      i1 = DB_StoreSur (&dbi, &ox1);
      if(i1 < 0) goto Problem1;
        // TEST ONLY:
        // test001();
      break;



    //---------------------------------------------------------------
    case Typ_SOL:            // B  (Body)
/*
      if(aus_typ[0] == Typ_cmdNCsub)  {
        if((int)aus_tab[0] == T_TRA) {   // B = TRA B
          oxp1 = (ObjGX*)memspc101;
          oSiz = sizeof(memspc101);
          i1 = APT_tra_obj (oxp1, oTyp, &oSiz,
                            aus_anz, aus_typ, aus_tab, &APTSpcObj);
          if(i1 < 0) return -1;
          ox1 = *oxp1;
          goto L_SOL_sav;
        }
      }
*/
      i1 = APT_decode_sol (&ox1, aus_anz, aus_typ, aus_tab, &APTSpcObj);
      if(i1 < 0) goto Problem1;

      L_SOL_sav:
      dbi = defInd;
      i1 = DB_StoreSol (&dbi, &ox1);
      // printf("StSol OK\n");
      if(i1 < 0) goto Problem1;
      break;




    //---------------------------------------------------------------
    case Typ_Model:                // "M"  referenceModel
/*
      if(aus_typ[0] == Typ_cmdNCsub)  {
        if((int)aus_tab[0] == T_MOCK) {
          UME_init (&tbuf1, memspc55, sizeof(memspc55));
          i1 = APT_decode_mock (&ox1, aus_anz, aus_typ, aus_tab, &tbuf1);
          if(i1 < 0) goto Problem1;
        }

      } else {
*/
        // printf("decode Typ_Model\n");
        i1 = APT_decode_model (&ox1, aus_anz, aus_typ, aus_tab);
        if(i1 == -2) return i1;   // nach ED_Run mehrmals unnutz !
        if(i1 < 0) goto Problem1;
      // }


      L_Mdl_sav:
      DB_StoreModel (defInd, &ox1);
      break;




    //---------------------------------------------------------------
    case Typ_Tra:

      i1 = APT_decode_tra (&ox1, aus_anz, aus_typ, aus_tab);
      if(i1 < 0) goto Problem1;
      // DEB_dump_ox_0 (&ox1, "T[%d]:",defInd);

      DB_StoreTra (defInd, &ox1);
      break;


    //---------------------------------------------------------------
    case Typ_Activ:
      i1 = APT_decode_Activ ((Activity*)obj1, &APTSpcObj,
                             aus_anz, aus_typ, aus_tab);
      if(i1 < 0) goto Problem1;
/*
      // display 
      UI_disp_activ (APT_obj_stat, defInd, (Activity*)obj1);
*/
      if(APT_obj_stat == 0) {
        i1 = DB_StoreActiv (defInd, (Activity*)obj1);
        if(i1 < 0) goto Problem1;
      }
      
      // exit without draw (no attributes)
      return -2;



    //---------------------------------------------------------------
    case Typ_Joint:
      i1 = APT_decode_Joint (&APTSpcObj, aus_anz, aus_typ, aus_tab, &APTSpcTmp);
      if(i1 < 0) goto Problem1;
/*
      // display 
      UI_disp_joint (APT_obj_stat, defInd, &APTSpcObj);
*/
      if(AP_modact_ind < 0) {                  // 0-n = sind Submodel; -1=main
        if(APT_obj_stat != 0) goto L_jnt_ex;   // temp-mode: do not store ..

      } else {
        // hide original obj
        JNT_parent_hide (aus_typ[0], (long)aus_tab[0], AP_modact_ind);
      }

      // save data in DBFile
      i1 = JNT_exp__ (defInd, &APTSpcObj);
      if(i1 < 0) goto Problem1;

      // exit without draw (no attributes)
      L_jnt_ex:
      return -2;




    //---------------------------------------------------------------
    default:
      TX_Error(" APT_store_obj: Fehler Typ %d unbekannt ****",oTyp);
      return -1;
  }


  Fertig:


    // free APTSpcObj APTSpcTmp 2019-03-18
    if(MEM_MUST_FREE(APTSpcObj.spcTyp)) UME_free (&APTSpcObj);
    if(MEM_MUST_FREE(APTSpcTmp.spcTyp)) UME_free (&APTSpcTmp);


  return -AP_stat.errStat;   // 2009-06-20
  // return 0;



  //==============================
  // Problem:
  // TX_Error(" APT_store_obj Fehler: Funktion unbekannt ****");


  Problem0:
  return -1;

  Problem1:
  TX_Error("ex APT_store_obj E001");
  return -1;

  Problem2:
  TX_Error("APT_store_obj: Func. not implemented");
  return -1;

  L_notYetComplete:
    TX_Print("- object not yet complete ..");
    return -3;
}




/*============================================================= */
  char* APT_cp_ausd (char* txtOut, char* txtIn, int maxSiz) {
/*=================
  den nächsten Ausdruck > txtOut kopieren
  (Ausdruck ist ein durch , oder blank begrenzter Text)
  Eigentlich ist auch ein Blank, wenn danach kein Operator (+ - / *) kommt,
  Ende Ausdruck.
  Wenn vor den nächsten "," ein "[" kommt, muss der gesamte Klammerausdruck mit !
    Also "p1[10,10]" darf nicht zerschnitten werden !
  Eine Funktion muss komplett mit dem Parameterblock kopiert werden;
    "sqrt (9)"  muss also komplett kopiert werden !
    "D20 (5 * 4)" muss aber zertrennt werden ..
  Operator mit folgendem Klammernblock: ("+ (2*3)")
    nur Operator allein ausgeben.
  Der Returncode ist die Pos. HINTER dem Terminator.
*/
 
  int     iAnz, kAnz, sAnz;
  char    *iPos, *p1;


  // printf("APT_cp_ausd in: |%s|\n",txtIn);

  while(*txtIn == ' ') ++txtIn;    // skip leading blanks

  kAnz = 0;
  sAnz = 0;
  iAnz = 0;
  iPos = txtIn;
  goto Start;


  //----------------------------
  Next:
  ++iPos;
  ++iAnz;

    // printf(" iPos=%c kAnz=%d sAnz=%d iAnz=%d\n",*iPos,kAnz,sAnz,iAnz);


  Start:
  if (*iPos == '\0') {
    //printf("Error UTX_cp_word_term: Zeichen %c nicht gef.\n",term);
    //--iAnz;
    goto Fertig;



  // Zwischen Anführungszeichen kann Klammer sein, NICHT berücksichtigen!
  } else if (*iPos == '"') {
    // skip textstrings
    NextAnfZ:
      ++iPos;
      ++iAnz;
      if(*iPos == '\0') {
        goto Fertig;
      }
      if(*iPos == '"') {
        goto Next;
      } else {
        goto NextAnfZ;
      }

/*
  } else if (*iPos == '(') {
    NextAnfK:
    ++iPos;
    ++iAnz;
    if (*iPos == '\0') {
      goto Fertig;
    }
    if (*iPos == ')') {
      ++iPos;
      ++iAnz;
      goto Fertig;
    } else {
      goto NextAnfK;
    }
*/

  } else if ((*iPos == '[')||(*iPos == '(')) {
    ++kAnz;
    //printf(" incr kAnz=%d\n",kAnz);
    goto Next;


  } else if ((*iPos == ']')||(*iPos == ')')) {
    --kAnz;
    //printf(" decr kAnz=%d\n",kAnz);
    goto Next;


  // Ende Ausdruck gefunden
  } else if ((*iPos == ',')&&(kAnz == 0)&&(sAnz == 0)) {
    //printf(" found term |%c| %d %d\n",*iPos,kAnz,sAnz);
    ++iPos;               // skip auch den term
    goto Fertig;


  // Ende Ausdruck gefunden
  } else if ((*iPos == ' ')&&(kAnz == 0)&&(sAnz == 0)) {
    //printf(" found term |%c| %d %d\n",*iPos,kAnz,sAnz);
    ++iPos;               // skip ' '
    while (*iPos == ' ') ++iPos;
    // do not cut function-parameters   2008-10-16   RF
    if(*iPos == '(') {
      // check for operator (eg "* (3+3)" - do not add bracketBlock)
      // operators: + - * / 
      if(ispunct(*txtIn)) goto Fertig;
      p1 = UTX_pos_skipBrack (iPos);
      iPos = p1 + 1;
      iAnz = iPos - txtIn;
    }
    goto Fertig;


  } else {
    goto Next;

  }



  Fertig:
    if(iAnz >= maxSiz) {iAnz = maxSiz-1; TX_Error("APT_cp_ausd E001");}
    strncpy (txtOut, txtIn, iAnz),
    txtOut[iAnz] = '\0';
    if(*iPos == ' ') ++iPos;  // skip foll. blank


    // printf("ex APT_cp_ausd out:|%s| rc:|%s|\n",txtOut,iPos);


  return iPos;

}


/*===========================================================================*/
  char* APT_get_Val (double *v1, char* inBuf, double offset) {
/*====================
Normalerweise nach decode_ausdr.
Nur herauskopiern sonst nix. 
Das nächte Wort kopieren.

Offen: Pointer out auf das next Word ...

*/
  char   *cp;


  inBuf = inBuf + (int)offset;

  *v1 = strtod(inBuf, &cp);
  if(cp) ++cp;

  //TX_Print("exit APT_get_Val %f |%s|  %f |%s|\n",*v1,cp,offset,inBuf);


  return cp;

}


//================================================================
  char* APT_get_Txt (char* outBuf, char* inBuf, double offset) {
//================================================================
/// \code
/// APT_get_Txt              copy next word from string
/// starting-position is inBuf[offset]
/// 
/// see also APT_get_String APT_get_Val APT_cp_ausd
/// \endcode

// Normalerweise nach decode_ausdr.
// Nur herauskopiern sonst nix. 

// Offen: Pointer out auf das next Word ...

  int    ilen;
  char   *cp;



/*
  // diese Version lässt keine Worte, die mit Ziffern beginnen, zu.
  cp = inBuf + (int)offset;
  UTX_get_word (&cp, &cp_next, outBuf, &d1, &deli);
*/


  // den Delimiter suchen
  inBuf = inBuf + (int)offset;
  cp = strpbrk(inBuf, " ,/;");
  if(cp) {
    ilen = cp-inBuf;
    //TX_Print(" copy %d chars",ilen);
    strncpy (outBuf,inBuf, ilen);
    outBuf[ilen] = '\0';
    ++cp;

  } else {
    ilen = strlen(inBuf);
    if(ilen > 0) {
      strcpy(outBuf,inBuf);
      // remove following '\n'
      --ilen;
      if(outBuf[ilen] == '\n') outBuf[ilen] = '\0';
    } else {
      outBuf[0] = '\0';
    }

  }


    // printf("exit APT_get_Txt |%s|%f|%s|\n",outBuf,offset,inBuf);


  return cp;

}


//================================================================
  int APT_str2fnam (char *fnam, char *str) {
//================================================================
/// \code
/// make (absolute) filename from string
/// Retcode:
///    0 = abs. Pfad;
///    1 = was relativ; changed to absolut <base</tmp/
/// 
/// TODO: replace with OS_dirAbs_fNam
/// \endcode

  int  irc, ifs;
  char *cp;

  // printf("APT_str2fnam |%s|\n",str);

  cp = str;
  if(*cp == '"') {++cp; ifs = 1;}
  else ifs = 0;

  irc = OS_ck_DirAbs(cp);
  if(irc == 0) {    // abs.Path
    sprintf(fnam, "%s",cp);
  } else {
    sprintf(fnam, "%s%s",OS_get_tmp_dir(),cp);
  }

  // remove following '"'
  if(ifs == 1) UTX_endDelChar (fnam, '"');

    // printf(" ex APT_str2fnam %d |%s|\n",irc,fnam);

  return irc;

}


//================================================================
  int APT_get_String (char* outBuf, char* inBuf, double offset) {
//================================================================
/// \code
/// copy word until double-quote
/// Input:
///   inBuf         startpos of string, surrounded by '"',
///                 starting with word-delimiter (blank or ',')
///   offset        startpos in inBuf
/// Output:
///   outBuf        size max 80
/// 
/// see also APT_get_Txt APT_get_TxAll
/// \endcode


  char   *cp;

  // printf("APT_get_String |%s| %d\n",inBuf,offset);

  // cp = inBuf + (int)offset + 1;
  cp = strchr(&inBuf[(int)offset], '"');
  if(!cp) {outBuf[0] = '\0'; return -1;}
  ++cp;  // skip the starting '"'
  // copy to (excluding) '"'
  UTX_cp_word_term (outBuf, cp, '"');


  // printf("exit APT_get_String |%s|%f|%s|\n",outBuf,offset,inBuf);

  return 0;

}


//================================================================
  void APT_get_TxAll (char* outBuf, char* inBuf, double offset) {
//================================================================
/// \code
/// copy string, remove outer enclosing double-quotes ".."
/// eg in: "LDMOD "Data/sample_mod_blech1.gcad""
///    out: LDMOD "Data/sample_mod_blech1.gcad"
/// 
/// see also APT_get_String APT_get_Txt
/// \endcode

  char   *cp;

  // printf("APT_get_String |%s| %d\n",inBuf,offset);

  cp = inBuf + (int)offset;

  if(*cp == '"') ++cp;  //skip "

  strcpy(outBuf, cp);

  cp = &outBuf[strlen(outBuf) - 1];
  if(*cp == '"') *cp = '\0';   ;  //skip "


  // printf("exit APT_get_TxAll |%s|%f|%s|\n",outBuf,offset,inBuf);

}


/*===========================================================================*/
  int APT_solv_mod (Point2 *pta, int PtAnz) {
/*================
den Index der Lösung retournieren
see also UTO_MOD_resolv_open
*/

  int    i1, ind1, ind2, ind, ModAct, PtInd[8];


  //printf("APT_solv_mod %d %d\n",PtAnz,ModAnz);



  for(i1=0; i1<PtAnz; ++i1) {
    PtInd[i1] = ON;
  }



  // 2 Modifier: alle bis auf 2 lösen
  if(ModAnz < 2 ) { goto Solve_1; }

  ModAct = ModTab[1];
  ind1 = APT_solv_mod_1 (pta, PtAnz, PtInd, ModAct);
  PtInd[ind1] = OFF;
  ind2 = APT_solv_mod_1 (pta, PtAnz, PtInd, ModAct);
  // nun alle ausser ind1 und ind2 auf OFF setzen.
  for(i1=0; i1<PtAnz; ++i1) {
    PtInd[i1] = OFF;
  }
  PtInd[ind1] = ON;
  PtInd[ind2] = ON;




  // 1 Modifier nur: das ist leicht!
  Solve_1:
  if(ModAnz < 1 ) { ind = 0; goto Fertig; }

  ModAct = ModTab[0];
  ind = APT_solv_mod_1 (pta, PtAnz, PtInd, ModAct);



  Fertig:
  //printf("APT_solv_mod %d \n",ind);

  return ind;
}


/*==========================================================================*/
  int APT_solv_mod_1 (Point2 *pta, int PtAnz, int *PtInd, int ModAct) {
/*==================
den Index der Lösung retournieren
*/

  int    ind;
  double max;


  //printf("APT_solv_mod_1 %d %d\n",PtAnz,ModAct);


  if(ModAct == T_HI) {
    ind=APT_solv_y_max (&max, pta, PtAnz, PtInd);
    goto Fertig;

  } else if (ModAct == T_LO) {
    ind=APT_solv_y_min (&max, pta, PtAnz, PtInd);
    goto Fertig;

  } else if (ModAct == T_LFT) {
    ind=APT_solv_x_min (&max, pta, PtAnz, PtInd);
    goto Fertig;

  } else if (ModAct == T_RGT) {
    ind=APT_solv_x_max (&max, pta, PtAnz, PtInd);
    goto Fertig;

  }


  Fertig:
  //printf("APT_solv_mod_1 %d \n",ind);

  return ind;
}




/*==========================================================================*/
  int APT_solv_x_max (double *max, Point2 *pta, int PtAnz, int *PtInd) {
/*====================
find min /max X- Wert
PtInd ist ein IntArr parallel zu pta; ON = der Punkt ist zu untersuchgen, OF - der
Punkt ist nicht mehr interressant.
Rückgabewert ist der gefundene Index.
*/

  int i1, ind;

  *max = UT_VAL_MIN;
  ind  = -1;


  for(i1=0; i1<PtAnz; ++i1) {
    //printf("  %d %d %f\n",i1, PtInd[i1], pta[i1].x);
    if((PtInd[i1] == ON)&&(pta[i1].x > *max)) {
      *max = pta[i1].x;
      ind = i1;
    }
  }

  //printf("APT_solv_x_max %d %f\n",ind,*max);

  return ind;

}

/*===========================================================================*/
  int APT_solv_y_max (double *max, Point2 *pta, int PtAnz, int *PtInd) {
/*====================
find min /max y- Wert
PtInd ist ein IntArr parallel zu pta; ON = der Punkt ist zu untersuchgen, OF - der
Punkt ist nicht mehr interressant.
Rückgabewert ist der gefundene Index.
*/

  int i1, ind;

  *max = UT_VAL_MIN;
  ind  = -1;


  for(i1=0; i1<PtAnz; ++i1) {
    if((PtInd[i1] == ON)&&(pta[i1].y > *max)) {
      *max = pta[i1].y;
      ind = i1;
    }
  }

  //printf("APT_solv_y_max %d %f\n",ind,*max);

  return ind;

}

/*==========================================================================*/
  int APT_solv_x_min (double *min, Point2 *pta, int PtAnz, int *PtInd) {
/*====================
find min /min X- Wert
PtInd ist ein IntArr parallel zu pta; ON = der Punkt ist zu untersuchgen,
 OF - der Punkt ist nicht mehr interressant.
Rückgabewert ist der gefundene Index.
*/

  int i1, ind;

  *min = UT_VAL_MAX;
  ind  = -1;


  for(i1=0; i1<PtAnz; ++i1) {
    if((PtInd[i1] == ON)&&(pta[i1].x < *min)) {
      *min = pta[i1].x;
      ind = i1;
    }
  }

  //printf("APT_solv_x_min %d %f\n",ind,*min);

  return ind;

}

/*=========================================================================*/
  int APT_solv_y_min (double *min, Point2 *pta, int PtAnz, int *PtInd) {
/*====================
find min /min X- Wert
PtInd ist ein IntArr parallel zu pta; ON = der Punkt ist zu untersuchgen,
 OF - der Punkt ist nicht mehr interressant.
Rückgabewert ist der gefundene Index.
*/

  int i1, ind;

  *min = UT_VAL_MAX;
  ind  = -1;


  for(i1=0; i1<PtAnz; ++i1) {
    if((PtInd[i1] == ON)&&(pta[i1].y < *min)) {
      *min = pta[i1].y;
      ind = i1;
    }
  }

  //printf("APT_solv_x_min %d %f\n",ind,*min);

  return ind;

}

/* replaced by APT_CUT__
//============================================================================
  int APT_trim_obj (void *oxo, int aus_anz, int aus_typ[], double aus_tab[],
                    Memspc *wrkSpc) {
//============================================================================
// LN|CI|CV = CUT ..
// oxo ist struct wie obj aus aus_tab[1]; ACHTUNG: kann gleiche struct sein !!
//     Size ist OBJ_SIZ_MAX !
// RetCod:
//  1-n  Ok, nr of possible resultions
// -1 =  Par_err
// -2 =  Geom_err;
// -3 =  NotImp_err;
//
//  Gegeben sind:
//    Altes Obj besteht aus 2 Punkten  (p1,p2) oder 2 Parametern (v1,v2);
//      LN,AC,CVELL: p1, p2;
//      CVBSP,CVPOL: v1, v2.
//    1 oder 2 CuttingObjs;
//    Modeziffer zu jedem CuttingObj;
//      imod1, imod2.
//
//  Weiters sind 1-n Schnittpunkte gegeben.
//    Schnittpunkte werden der Reihe nach mit Hilfe des Mode abgearbeitet; 
//    Mode 0/1 benutzen Schnittpunkt 0; Mode 2/3 benutzen Schnittpunkt 1 usw.
//    Von der Intersect-Funktion muss der am naechsten liegende Schnittpunkt
//      als erster geliefert werden. 
//
//  1 CuttingObj: test if intersectPoint ipt is equal to objEndPoint (p1|p2)
//    if(imod1 == 0)  // (Default):
//      if(ipt = p1) objNeu=(ipt - p2);
//      objNeu=(ipt - p1);
//
//    if(imod1 == 1)  // andere Seite:
//      if(pCut1 = p2) objNeu=(pCut1 - p1);
//      objNeu=(pCut1 - p2);
//
//
//  2 CuttingObjs:
//    objNeu=(pCut1 - pCut2);
//
//
//
//Testmodell: sample_inters_curv1
//
 
  int    irc, i1Nr, i2Nr, inxt, ii2, o2Typ, oSiz, pNr;
  long   ic1, ic2, ii;
  void   *wrkPos;
  ObjGX  oc1, oc2, oi, oo;




  printf("APT_trim_obj |%d|\n",aus_anz);
  for(ii=0; ii<aus_anz; ++ii) {
    printf(" %d typ=%d tab=%f\n",ii,aus_typ[ii],aus_tab[ii]);
  }



  i1Nr = 0;
  i2Nr = 0;


  // memsiz merken 
  wrkPos = wrkSpc->next;


  // obj[0] ist 170/30
  // obj[1] ist das obj to cut (typ/index)
  // obj[2] ist das 1. cuttingObJ, typ/index  oder Typ_Val/val.

  // obj[3] ist NULL
  //        od MOD1
  //        od 2. SchnittObj; typ/index  oder Typ_Val/val.
  inxt = 3;
  if((aus_anz > 3)&&(aus_typ[inxt] == Typ_modif)) {
    i1Nr = aus_tab[inxt];
    ++inxt;
  }


  // obj[4] ist NULL od 2. SchnittObj od MOD2
  // obj[inxt] 2. SchnittObj (auf [3] od [4])
  if(inxt >= aus_anz) {
    o2Typ = Typ_Error;
    goto L_trim_1;
  } else {
    ii2 = inxt;
    o2Typ = aus_typ[ii2];
    ic2   = aus_tab[ii2];
    ++inxt;
  }


  // obj[5] ist NULL od MOD2
  // obj[inxt] = MOD2 ([4] od [5])
  if(inxt >= aus_anz) goto L_trim_1;
  if(aus_typ[inxt] == Typ_modif) {
    i2Nr = aus_tab[inxt];
  }



  //----------------------------------------------------------------
  L_trim_1:

  // obj to cut                  obj[1] ist zu bearbeitendes obj
  oi.typ   = aus_typ[1];
  oi.form  = Typ_Index;
  ii = aus_tab[1];
  oi.data  = (void*)ii;



  // 1. cutting obj:             obj[2] muss 1. SchnittObj sein
  oc1.typ  = aus_typ[2];
  if(o2Typ == Typ_Val) {
    oc1.form = Typ_Val;
    oc1.data = (void*)&aus_tab[2];
  } else {
    oc1.form = Typ_Index;
    ic1 = aus_tab[2];
    oc1.data = (void*)ic1;
  }


  // 2. cutting obj:
  oc2.typ  = o2Typ;
  if(o2Typ == Typ_Val) {
    oc2.form = Typ_Val;
    oc2.data = (void*)&aus_tab[ii2];
  } else {
    oc2.form = Typ_Index;
    oc2.data = (void*)ic2;
  }


  // trim obj
  irc = UTO_obj_trim (&oo, &oi, &oc1, i1Nr, &oc2, i2Nr, wrkSpc);
  if(irc < 0) return irc;
  pNr = irc;

  // copy resulting struct back; ACHTUNG: kann gleiche struct wie Input sein !!
  // (Input = aus aus_tab[1])
  oSiz = OBJ_SIZ_MAX;  // hoffentlich ..
  irc = UTO_copy_stru (oxo, &oSiz, oo.form, oo.data, 1);
  if(irc < 0) return irc;

  // reset memspc
  wrkSpc->next = wrkPos;

  return pNr;


// /
//  int    irc, i1, ii, inxt, o0Typ, o1Typ, o2Typ, pNr, oSiz,
//         modFlag1, imod1, modFlag2, imod2, io2, cvTyp;
//  long   o1i, o2i;
//  double va[20], v1,  v2, vi1, vi2;
//  char   cbuf[32];
//  void   *obj0, *obj1, *obj2;
//  Point  pa[20], pt1, pt2, pti1, pti2;
//
//
//
//  // printf("APT_trim_obj |%d|\n",aus_anz);
//  // for(i1=0; i1<aus_anz; ++i1) {
//    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
//  // }
//  // UME_dump(wrkSpc, " wrkSpc:");
//
//
//  modFlag1 = 0;
//  modFlag2 = 0;
//
//
//  // obj[1] ist zu bearbeitendes obj
//  // erstes Obj laden
//  o0Typ = aus_typ[1];
//
//
//  // obj[2] muss 1. SchnittObj sein
//  o1Typ = aus_typ[2];
//
//
//  // obj[3] ist NULL od MOD1  od 2. SchnittObj
//  inxt = 3;
//  if(aus_typ[inxt] == Typ_modif) {
//    modFlag1 = 1;
//    imod1 = aus_tab[inxt];
//    ++inxt;
//  }
//
//
//  // obj[4] ist NULL od 2. SchnittObj od MOD2
//  // obj[inxt] 2. SchnittObj (auf [3] od [4])
//  if(inxt >= aus_anz) {
//    o2Typ = Typ_Error;
//    goto L_trim_1;
//  } else {
//    o2Typ = aus_typ[inxt];
//    io2 = inxt;
//    ++inxt;
//  }
//
//
//  // obj[5] ist NULL od MOD2
//  // obj[inxt] = MOD2 ([4] od [5])
//  if(inxt >= aus_anz) goto L_trim_1;
//  if(aus_typ[inxt] == Typ_modif) {
//    modFlag2 = 1;
//    imod2 = aus_tab[inxt];
//  }
//
//
//
//
//  //================================================================
//  // Inputdaten fertig analysiert; 
//  // Objekte aus DB holen.
//  L_trim_1:
//  // printf(" modFlag1=%d modFlag2=%d\n",modFlag1,modFlag2);
//  // printf(" o0Typ=%d ind=%d\n",o0Typ,(long)aus_tab[1]);
//  // printf(" o1Typ=%d o1i=%d\n",o1Typ,o1i);
//  // printf(" o2Typ=%d o2i=%d\n",o2Typ,o2i);
//  // o0Typ obj0         obj to cut
//  // o1Typ obj1         1. cutting obj
//  // o2Typ obj2         2. cutting obj  (Typ_Error: kein obj.)
//  // modFlag1           0: kein Mode; 1: imod1 = Mode;
//  // modFlag2           0: kein Mode; 1: imod2 = Mode;
//
//
//  // get obj to cut
//  irc = UTO_obj_dbo (&obj0, &o0Typ, (long)aus_tab[1]);
//  if(irc < 0) {i1 = o0Typ; o1i = (long)aus_tab[1]; goto ObjErr;}
//    // DEB_dump_obj__ (o0Typ, obj0, "APT_trim_obj");
//
//
//
//  // get cutting obj 1
//  // printf(" o1Typ=%d o1Ind=%d\n",o1Typ,o1Ind);
//  if(o1Typ == Typ_Val) {
//    obj1 = (void*)&aus_tab[2];
//  } else {
//    o1i = aus_tab[2]; 
//    irc = UTO_obj_dbo (&obj1, &o1Typ, o1i);
//    if(irc < 0) {i1 = o1Typ; goto ObjErr;}
//  }
//
//
//  // get cutting obj 2
//  if(o2Typ != Typ_Error) {
//    // printf(" o2Typ=%d o2Ind=%d\n",o2Typ,o2Ind);
//    if(o2Typ == Typ_Val) {
//      obj2 = (void*)&aus_tab[io2];
//    } else {
//      o2i = aus_tab[io2];
//      irc = UTO_obj_dbo (&obj2, &o2Typ, o2i);
//      if(irc < 0) {i1 = o2Typ; o1i = o2i; goto ObjErr;}
//    }
//  }
//
//
//  //----------------------------------------------------------------
//  // copy obj to cut out
//  oSiz = OBJ_SIZ_MAX;
//  UTO_copy_stru (oxo, &oSiz, o0Typ, obj0, 1);
//
//  // check if CurveLimits are Points or parameters
//  cvTyp = UTO_ck_curvLimTyp (o0Typ);  // 0=points, 1=Parameters
//
//  // get limits of obj to cut -> pt1/pt2 or v1/v2
//  irc = UTO_2pt_limstru (&pt1, &pt2, &v1, &v2, o0Typ, obj0);
//  if(irc < 0) return irc;
//    // GR_tDyn_symB__ (&pe1, SYM_TRI_S, 2);
//    // GR_tDyn_symB__ (&pe2, SYM_TRI_S, 2);
//
//
//
//
//  //----------------------------------------------------------------
//  // intersect with 1. cutting obj
//  pNr = 20; // maxNr
//  irc = UTO_stru_int (&pNr, pa, va, o0Typ, obj0, o1Typ, obj1, wrkSpc);
//  if(irc < 0) goto GeomErr;
//    // GR_tDyn_npt__ (pNr, pa, SYM_STAR_S, 2);
//
//
//  // imod korrigieren; index > pa/va bestimmen
//  if(modFlag1 > 0) APT_trim_u0 (&ii, &imod1, pNr);
//  else   { ii = 0;  imod1 = 0;}
//
//  pti1 = pa[ii];
//  vi1  = va[ii];
//
//
//
//
//  // intersect with 2. cutting obj
//  if(o2Typ == Typ_Error) goto L_cut1;
//  // Sind die beiden cuttingObjects ident ? (zb CUT C20 L20 L20)
//  if(o1Typ == Typ_Val) goto L_int_o2_1;
//  if(o1Typ != o2Typ) goto L_int_o2_1;
//  if(o1i != o2i) goto L_int_o2_1;
//  // hier muesste man auch Typ_VAL checken !
//  if(modFlag2 == 0) { // use den 2. IntersectionPunkt
//    ii = 1;
//    goto L_int_o2_3;
//  }
//  // select entspr. modFlag2
//  goto L_int_o2_2;
//
//
//  // intersect
//  L_int_o2_1:
//  pNr = 20; // maxNr
//  irc = UTO_stru_int (&pNr, pa, va, o0Typ, obj0, o2Typ, obj2, wrkSpc);
//  if(irc < 0) goto GeomErr;
//    // GR_tDyn_npt__ (pNr, pa, SYM_STAR_S, 2);
//
//
//  // imod korrigieren; index > pa/va bestimmen
//  L_int_o2_2:
//  if(modFlag2 > 0) APT_trim_u0 (&ii, &imod2, pNr);
//  else   { ii = 0;  imod2 = 0;}
//
//  L_int_o2_3:
//  pti2 = pa[ii];
//  vi2  = va[ii];
//
//  goto L_cut2;
//
//
//
//
//  //----------------------------------------------------------------
//  // nur 1 CuttingObj:
//  // pa1[ii] ist der eine Punkt; der andere entsprechend imod1.
//  L_cut1:
//
//
//  // Circ u Ellipse: mit Startpunkt halbieren, wenn 360-Grad.
//  if(o0Typ == Typ_CI) {
//    if(UT3D_ck_ci360((Circ*)oxo) == YES) {
//      // UT3D_ci_cipt180 ((Circ*)oxo, &pti1);
//      UT3D_pt_opp2pt (&pt1, &((Circ*)oxo)->pc, &pti1);
//      pt2 = pt1;
//    }
//  } else if(o0Typ == Typ_CVELL) {
//    if(UT3D_ck_el360((CurvElli*)oxo) == YES) {
//      // UT3D_ci_cipt180 ((Circ*)oxo, &pti1);
//      UT3D_pt_opp2pt (&pt1, &((CurvElli*)oxo)->pc, &pti1);
//      pt2 = pt1;
//    }
//  }
//
//
//  // trim----------
//  if(imod1 == 0)  {                // Default; replace pt1, keep pt2
//    // pti1-pt2 duerfen nicht gleich sein; else ..
//    pt1 = pti1;
//    v1 = vi1;
//
//
//  } else {          // andere Seite: keep pt1, replace pt2
//    // pti1-pt1 duerfen nicht gleich sein; else ..
//    pt2 = pti1;
//    v2 = vi1;
//  }
//
//
//
//  goto L_out;
//
//
//  //----------------------------------------------------------------
//  // 2 CuttingObjs:
//  L_cut2:
//
//
//  // Circ u Ellipse: entspr imod umdrehen
//  if(o0Typ == Typ_CI) {
//    if(imod1 > 0) UT3D_ci_inv2 ((Circ*)oxo);
//  } else if(o0Typ == Typ_CVELL) {
//    if(imod1 > 0) UT3D_el_inv2 ((CurvElli*)oxo);
//  }
//
//
//
//    // replace pt1 & pt2
//    pt1 = pti1;
//    pt2 = pti2;
//
//    v1 = vi1;
//    v2 = vi2;
//
//
//
//
//
//
//  //----------------------------------------------------------------
//  // create outputObj and replace points
//  L_out:
//
//  // oOut = set endpoints or parameters;
//  // LN,AC,CVELL: p1, p2 setzen;
//  // CVBSP,CVPOL: v1, v2 setzen.
//  UTO_set_ptlim (o0Typ, oxo, &pt1, &v1, &pt2, &v2, wrkSpc);
//    // DEB_dump_obj__ (o0Typ, oxo, "ex APT_trim_obj\n");
//
//
//
//
//  return 0;
///
//
//
/// old Solution korr 2005-10-19
//=============================================================
//  // die Endpunkte von o0 nach pe1, pe2 holen
//  UTO_2pt_limstru (&pe1, &ve1, &pe2, &ve2, o0Typ, obj0);
//    // GR_tDyn_symB__ (&pe1, SYM_TRI_S, 2);
//    // GR_tDyn_symB__ (&pe2, SYM_TRI_S, 2);
//
//
//  // den Schnittpunkt auswaehlen und > pt1
//  // printf(" modFlag1=%d imod1=%d\n",modFlag1,imod1);
//  // if(modFlag1 == 1) ie = APT_trim_u0 (&pt1, imod1, pNr, pa);
//  if(modFlag1 == 1) ie = APT_trim_u0 (&ii, imod1, pNr);
//  // waehle Punkt aus pa jedoch NICHT p1, p2 ..
//  // geht nicht bei step; liefert oft CUTS mit Start/Endpunkt !
//  // else ie = APT_trim_u1 (&ii, pNr, pa, &pe1, &pe2);
//  else {
//    if(UT3D_comp2pt(&pa[0],&pe1,UT_TOL_pt) == 0) ie = 1;
//    else ie = 2;
//    ii = 0;
//    // printf(" ie=%d\n",ie);
//  }
//    // GR_tDyn_symB__ (&pt1, SYM_TRI_S, 2);
//
//  pt1 = pa[ii];
//  v1  = va[ii];
//  // DEB_dump_obj__ (Typ_PT, &pt1, "v1=%f pt1=",v1);
//
//
//  // nun ist pt1 der gewaehlte intersectPoint;
//  // ie definiert den Endpunkt des o0 (1 den pt1, 2 den pt2).
//  if(ie == 1) {pt2 = pe2; v2 = ve2;}  // use endPt
//  else        {pt2 = pe1; v2 = ve1;}  // use startPt
//
//
//
//
//
//  // copy obj
//  oSiz = OBJ_SIZ_MAX;
//  UTO_copy_stru (o0, &oSiz, o0Typ, obj0, 1);
//
//
//  // Vollkreis oder 360-Grad-Ellipse: beim Startpunkt halbieren
//  // und entspr imod umdrehen
//  if(o0Typ == Typ_CI) {
//    if(UT3D_ck_ci360((Circ*)o0) == YES) {
//      // UT3D_ci_cipt180 ((Circ*)o0, &pt1);
//      UT3D_pt_opp2pt (&pt2, &((Circ*)o0)->pc, &pt1);
//      if(ie > 1) UT3D_ci_inv2 ((Circ*)o0);
//    }
//  } else if(o0Typ == Typ_CVELL) {
//    if(UT3D_ck_el360((CurvElli*)o0) == YES) {
//      // UT3D_ci_cipt180 ((Circ*)o0, &pt1);
//      UT3D_pt_opp2pt (&pt2, &((CurvElli*)o0)->pc, &pt1);
//      if(ie > 1) UT3D_el_inv2 ((CurvElli*)o0);
//    }
//
//  }
//
//  // set endpoints;
//  // LN,AC,CVELL: p1, p2 setzen;
//  // CVBSP,CVPOL: v1, v2 setzen.
//  UTO_set_ptlim (o0Typ, o0, &pt1, &v1, &pt2, &v2, wrkSpc);
//    // DEB_dump_obj__ (o0Typ, o0, " nach cut1");
//
//
//
//
//
//  //------------ zweite teilung -------------------------
//  if(o2Typ == Typ_Error) goto L_trim9;
//
//  // printf(" o2Typ=%d o2Ind=%d\n",o2Typ,o2Ind);
//  if(o2Typ == Typ_Val) {
//    obj2 = (void*)&aus_tab[io2];
//  } else {
//    oInd = aus_tab[io2]; 
//    i1 = UTO_obj_dbo (&obj2, &o2Typ, oInd);
//    if(i1 < 0) goto ParErr;
//  }
//
//
//  // intersect
//  pNr = 20; // maxNr
//  i1 = UTO_stru_int (&pNr, pa, va, o0Typ, obj0, o2Typ, obj2, wrkSpc);
//  if(i1 < 0) goto GeomErr;
//    // GR_tDyn_npt__ (pNr, pa, SYM_STAR_S, 2);
//
//
//  // die Endpunkte von o0 nach pe1, pe2 holen
//  pe1 = pt1;
//  pe2 = pt2;
//
//  // pt1 bleibt vom ersten Schnitt; nur mehr pt2 ermitteln
//  // printf(" modFlag2=%d imod2=%d\n",modFlag2,imod2);
//  // if(modFlag2 == 1) {pt2 = pa[IMIN(imod2,pNr-1)];}
//  if(modFlag2 == 1) {ii = IMIN(imod2,pNr-1);}
//  // waehle Punkt aus pa jedoch NICHT p1, p2 ..
//  else ie = APT_trim_u1 (&ii, pNr, pa, &pe1, &pe2);
//  // else ie = APT_trim_u1 (&pt2, pNr, pa, &pe1, &pe2);
//
//  pt2 = pa[ii];
//  v2  = va[ii];
//
//
//  // set endpoints (pt1 bleibt vom ersten Schnitt)
//  UTO_set_ptlim (o0Typ, o0, &pt1, &v1, &pt2, &v2, wrkSpc);
//    // DEB_dump_obj__ (o0Typ, o0, " nach cut2");
//
//=============================================================
//
//
//  L_trim9:
//  // obj nach draussen kopieren
//  oSiz = OBJ_SIZ_MAX;
//  UTO_copy_stru (oxo, &oSiz, o0Typ, o0, 1);
//    DEB_dump_obj__ (o0Typ, oxo, "ex APT_trim_obj");
//
//  return 0;
//
///
//
///
//  GeomErr:
//  TX_Error("Trim: - Geometry Error");
//  return -1;
//
//  ParErr:
//  TX_Error("Trim: - Parameter Error");
//  return -1;
//
//  ObjErr:
//  APED_oid_dbo__ (cbuf, i1, o1i);
//  TX_Error("Obj %s not defined Error",cbuf);
//  return -1;
//
//  NotImp_err:
//  TX_Error("Trim: - not yet implemented Error");
//  return -1;
///

}
*/

/*
//=========================================================================
  int APT_trim_u0 (int *ii, int *imod, int pnr) {
//=========================================================================

// Input:
//   imod (Zahl 0 - n)
//   ptnr (Anzahl Schnittpunkte);
// Output:
//   imod korrigiert (nur mehr 0 oder 1) und Punktindex indPt (0-(pnr-1))
//   imod = 0:  use ElementStartpunkt
//   imod = 1:  use ElementEndpunkt
//
//Beispiel: Inp imod pnr    out imod indPt
//              0    1          0    0
//              1    1          1    0
//              2    2          0    1
//              3    2          1    1        usw
//



  // printf("APT_trim_u0 imod=%d pnr=%d\n",imod,pnr);


  *ii = *imod / 2;
  if(*ii >= pnr) *ii = pnr - 1;

  *imod = *imod % 2;  // modulo 2

  // printf(" ex APT_trim_u0 ii=%d imod=%d pnr=%d\n",*ii,*imod,pnr);

  return 0;


/
//  int ip, ie;
//
//  if(pnr == 1) {
//    // *pto = pa[0];
//    *ii = 0;
//    ie = imod + 1;
//    goto L_fertig;
//  }
//
//
//  // intIndex = imod / intAnzahl
//  ip = imod / pnr;
//  *ii = IMIN(ip,pnr-1);
//  ie = imod % pnr;
//  ++ie;
//
//
//  L_fertig:
//  // printf(" ex APT_trim_u0 imod=%d ii=%d ie=%d\n",imod,*ii,ie);
//
//  return ie;
/
}
*/

/*
//=========================================================================
  int APT_trim_u1 (int *ii, int pnr, Point *pa, Point *p1, Point *p2) {
//=========================================================================
// waehle Punkt aus pa jedoch NICHT p1, p2 ..
// irc = 0:  OK
// irc = 1:  p1 in pa enthalten; set p2.
// irc = 2:  p2 in pa enthalten; set p1.
// irc > 2:  beide Punkte gleich; nix tun, aber OK

// Output: pto ist ein Punkt, der nicht in pa enthalten ist;
//   irc = 1: einer der Punkte von pa == p1;
//   irc = 2: pto ist pa[pnr-1] aber nicht p1;
//   irc = 3: kein neuer Punkt in pa; nix tun.

  int i1, irc;

  // printf("APT_trim_u1 pnr=%d\n",pnr);
  // DEB_dump_obj__ (Typ_PT, p1, "p1");
  // DEB_dump_obj__ (Typ_PT, p2, "p2");


  irc = 0;
  *ii = pnr - 1;

  for(i1=0; i1<pnr; ++i1) {
    // Test Startpunkt
    if(UT3D_comp2pt(&pa[i1],p1,UT_TOL_pt) == 1) {irc += 1; continue;}
    // pa[i1] nicht ident mit p1;
    // Test Endpunkt
    if(UT3D_comp2pt(&pa[i1],p2,UT_TOL_pt) == 1) {irc += 2; continue;}
    // pa[i1] nicht ident mit p1 und nicht ident m p2; nehmen.
    *ii = i1;
  }

  // pa Punkte in p1 und/oder p2 enthalten:
  if(((pnr == 1)&&(irc > 0))  ||
     ((pnr > 1)&&(irc > 3)))      {

    irc = 3;
  }

  // // beide Punkte gleich; nix tun, aber OK
  // // if(irc == 0) *pto = pa[0];
  // if(irc == 3) *ii = 0;


  L_fertig:
  // printf("ex APT_trim_u1 ie=%d ii=%d\n",irc,*ii);
  return irc;

}
*/


//============================================================================
  int APT_tra_obj (ObjGX *oxo, Memspc *wrkSpc,
                  int aus_anz, int aus_typ[], double aus_tab[],
                  Memspc *tSpc1) {
//============================================================================
// decode Line "TRA P23 R20 [REV]"
// Ausgabeobjekt entspricht dem ersten Eingabeobjekt (aus_typ[1]).
// fuer P/L/C/ muss struct geliefert werden
// fuer S muss oGX geliefert werden
// Input:
//   oTyp    outputtype; Line for Typ_LN, ObjGX for Typ_CV|Typ_SUR|Typ_SOL
//   oSiz    size of outputfield oxo
// Output:
//   oxo     output-objecet (struct of type oTyp) 
//   RetCod:  >=0 - the struct type of the transformed object
//            -1 =  Par_err;   -2 =  Geom_err;   -3 =  NotImp_err;
// TODO: es sollte oTyp geliefert werden ..




  int     irc, i1, i2, o0Typ, oForm, iDir, iNr;
  long    il, dbi;
  double  d1;
  void    *obji, *objo, *vp1, *poo;
  Point   pt1, pa[2];
  Vector  *vc1, vt;
  Plane   *pl1;
  ObjGX   *ox1, ox2;
  Mat_4x3 m1, mi1;


  // printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa \n");
  // printf("APT_tra_obj anz=%d\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  // set APT_hide_parent (keep-parent) from last parameter
  if(aus_typ[aus_anz - 1] == Typ_cmdNCsub) {
    if(aus_tab[aus_anz - 1] == T_REPL) {
      APT_hide_parent = 1;    // 0=not, else yes
      --aus_anz;
    }
  }
    // printf(" APT_hide_parent=%d\n",APT_hide_parent);


  // iDir = letzter Parameter "REV" optional
  iDir = 1;
  if(aus_anz > 3) {
    if(aus_typ[3] == Typ_cmdNCsub) {
      i1 = aus_tab[3];
      if(i1 == T_REV) {
        iDir = -1;
      }
    }
  }





  //================================================================
  // obj[2] muss TraMat sein
  if(aus_typ[2] == Typ_Tra) {

    // TraMat holen 
    il = aus_tab[2];
    ox1 = DB_GetTra (il);
    if(ox1 == NULL) return -2;
      // DEB_dump_ox_0 (ox1, "T[%d]:",(long)aus_tab[2]);
    // UTO_siz_obj (ox1);

    // Load Translation or Rotation
    // UTO_obj_apptra (NULL, ox1->form, ox1->data, NULL);
    UTRA_def__ (1, ox1->form, ox1->data);
  


  //----------------------------------------------------------------
  // oder Vector [Laenge]
  } else if(aus_typ[2] == Typ_VC) {

    il = aus_tab[2];
    vc1 = DB_get_VC (il);
      // DEB_dump_obj__ (Typ_VC, vc1, " vc1 = ");
      // printf(" iDir=%d\n",iDir);
     

    if(aus_typ[3] == Typ_Val) {
      UT3D_vc_setLength (&vt, vc1, aus_tab[3]);
    } else {
      vt = *vc1;
    }

    if(iDir < 0) {
      UT3D_vc_invert (&vt, &vt);
    }
      // DEB_dump_obj__ (Typ_VC, &vt, " vt = ");

    // Load Translation
    // UTO_obj_apptra (NULL, Typ_VC, &vt, NULL);
    UTRA_def__ (1, Typ_VC, &vt);



  //----------------------------------------------------------------
  // oder Plane
  } else if(aus_typ[2] == Typ_PLN) {
 
    il = aus_tab[2];
    pl1 = DB_get_PLN (il);

    UT3D_m3_loadpl (m1, pl1);

    if(iDir < 0) {
      UT3D_m3_invm3 (mi1, m1);
      memcpy (m1, mi1, sizeof(Mat_4x3));
    }

    // init transf.
    UTRA_def__ (1, Typ_M4x3, m1);



  //----------------------------------------------------------------
  } else goto ParErr;


  //================================================================
  // obj[1] ist zu bearbeitendes obj
  // erstes Obj laden
  o0Typ = aus_typ[1];
  dbi = aus_tab[1];
    // printf(" tra_obj-o0Typ=%d dbi=%ld\n",o0Typ,dbi);


  // get a ObjGX of the obj to transform
  ox2 = DB_GetObjGX (o0Typ, dbi);
  if(ox2.typ == Typ_Error) return -1;
    // DEB_dump_ox_0  (&ox2, "tra_obj-ox2:");
    // DEB_dump_ox_s_  (&ox2, "APT_tra_obj-in:");
    // eg: typ=CvBsp(23) form=CvBsp(23)
    // eg: typ=SurRU(53) form=ObjGX(204)


  // get space for ox2.siz * ox2.form in wrkSpc
  il = ox2.siz * UTO_siz_stru (ox2.form);
  objo = UME_reserve (wrkSpc, il);
  if(objo == NULL) goto L_err_EOM;
    // printf(" tra_obj-il=%ld\n",il);

  // transform 
  irc = UTRA_app__ (objo, ox2.typ, ox2.form, ox2.siz, ox2.data, wrkSpc);
  if(irc < 0) return irc;

  // make ObjGX
  OGX_SET_OBJ (oxo, ox2.typ, ox2.form, ox2.siz, objo);



    // TESTBLOCK
    // DEB_dump_ox_0  (oxo, "ex-APT_tra_obj");
    // if(!UTO_ck_dbsTyp (ox2.form)) {
      // DEB_dump_obj__ (ox2.form, oxo, "ex-APT_tra_obj-out-1");
    // } else {
      // DEB_dump_ox_s_  (oxo, "ex-APT_tra_obj-out-2");
    // }
    // printf("ex APT_tra_obj AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
    // END TESTBLOCK


  return ox2.form;


  //----------------------------------------------------------------
  ParErr:
    TX_Error("Transform: - Parameter Error");
    return -1;


  L_err_EOM:
    TX_Error("APT_tra_obj: - EOM");
    return -1;

}

/*
//============================================================================
  int APT_prj_obj_vc (void *oxo,
                  int aus_anz, int aus_typ[], double aus_tab[],
                  Memspc *wrkSpc) {
//============================================================================


  int       i1, i2, typt, typi, typo, oSiz, ia[2];
  void      *obji, *objo;
  Vector    vc1;
  Plane     pl1;
  ObjGX     *ox1;
  void      *tSur;


  printf("APT_prj_obj_vc |%d|\n",aus_anz);
  for(i1=0; i1<aus_anz; ++i1) {
    printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  }


  // obj[1] ist zu bearbeitendes obj
  typi = aus_typ[1];


  //================================================================
  // obj[2] muss Vector sein
  if(aus_typ[2] == Typ_VC) {
    vc1 = DB_GetVector ((long)aus_tab[2]);
  } else goto ParErr;


  //================================================================
  // obj[3] - where to project - target.
  typt = aus_typ[3];
  if(typt == Typ_PLN) {
    DB_GetRef (&pl1, (long)aus_tab[3]);
    tSur = &pl1;

  } else if(typt == Typ_SUR) {
    ox1 = DB_GetSur ((long)aus_tab[3], 1);
    if(ox1->typ == Typ_Error) goto ParErr;

    // Typ_SURPMSH:
    // load surMeshNr and surPtabNr into ia for projTarget
    if(ox1->typ == Typ_SURPMSH) {
      typt = Typ_SURPMSH;
      ia[0] = aus_tab[3];                // surMeshNr
      ia[1] = OGX_GET_INT (ox1);       // surPtabNr
      tSur = ia;

    } else goto ParErr;

  } else goto ParErr;


  //================================================================
  // load obj to project
  i1 = UTO_obj_dbo (&obji, &i2, &typi, (long)aus_tab[1]);
  if(i1 < 0) return -1;


  // init; load Plane -> transf.Func
  // UTO_obj_appprj (Typ_PLN, &pl1, TYP_FuncInit, &vc1, NULL);
  UPRJ_def__ (typt, tSur, &vc1);


  // ein Circ wird zu einer Ellipse !!!
  if(typi == Typ_CI) typo = Typ_CVELL;
  else               typo = typi;



  // transform ..
  if(AP_typDB_typ(typo) == Typ_CV)   {

    // das Ausgabeobjekt im wrkSpc anlegen
    objo = wrkSpc->next;
    i1 = UME_add (wrkSpc, OBJ_SIZ_MAX);
    if(i1 < 0) return -1;

    OGX_SET_OBJ ((ObjGX*)oxo, typo, typo, 1, objo);


  } else { // PT/LN direkt -> oxo
    objo = oxo;
  }


  // i1 = UTO_obj_appprj (typo, objo, typi, obji, wrkSpc);
  oSiz = OBJ_SIZ_MAX;
  i1 = UPRJ_app__ (objo, &typo, &oSiz, 1, obji, typi, wrkSpc);
  if(i1 < 0) return i1;


  return 0;

  GeomErr:
  TX_Error("Project: - Geometry Error");
  return -1;

  ParErr:
  TX_Error("Project: - Parameter Error");
  return -1;

  NotImp_err:
  TX_Error("Project: - not yet implemented Error");
  return -1;

}
*/

//============================================================================
  int APT_prj_obj_perp (ObjGX *oxo,
                  int aus_anz, int aus_typ[], double aus_tab[],
                  Memspc *wrkSpc) {
//============================================================================
// project object-what normal or in direction vector onto object-where 
// Input:
//   oxo          outputobject; data-struct is is wrkSpc
//   aus_typ[0] = Typ_cmdNCsub; aus_tab[0] = T_PRJ;
//   aus_typ[1] = what
//   aus_typ[.] = vector (optional)
//   aus_typ[.] = where
// Output:
//   oxo          binaryObj
//   RetCod       typ of oxo; Typ_PT|Typ_LN or curve (Typ_ObjGX)

// PT = PT -> LN 


  int       i1, i2, oSiz, ia[2], oNr;
  void      *obji, *datWhere, *objo;
  // Plane     pl1;
  Vector    *vc1;
  ObjDB     oWhat, oWhere, oDir;
  // Mat_4x3   m1;


  // printf("PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP \n");
  // printf("APT_prj_obj_perp |%d|\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%lf\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  // set APT_hide_parent (keep-parent) from last parameter
  if(aus_typ[aus_anz - 1] == Typ_cmdNCsub) {
    if(aus_tab[aus_anz - 1] == T_REPL) {
      APT_hide_parent = 1;    // 0=not, else yes
      --aus_anz;
    }
  }
    // printf(" APT_hide_parent=%d\n",APT_hide_parent);


  // get last val = solution-nr MOD(): -> APT_prim_seg
  if(aus_typ[aus_anz-1] == Typ_modif) {
    --aus_anz;
    APT_prim_seg = aus_tab[aus_anz];
    if(APT_prim_seg) --APT_prim_seg;  // make 0=0; 1=0; 2=1 ....
  } else {
    APT_prim_seg = 0;
  }

  // get obj-what    obj[1] ist zu bearbeitendes obj
  // typi = aus_typ[1];
  oWhat.typ   = aus_typ[1];
  oWhat.dbInd = aus_tab[1];

  // vector ?
  if(aus_typ[2] == Typ_VC) {
    i1 = 2;  i2 = 3;

  } else if(aus_typ[3] == Typ_VC) {
    i1 = 3;  i2 = 2;

  } else {
    i1 = -1; i2 = 2;
  }

  // get obj-where
  oWhere.typ   = aus_typ[i2];
  oWhere.dbInd = aus_tab[i2];

  // "project along vector"
  if(i1 > 0) {
    vc1 = DB_get_VC ((long)aus_tab[i1]);

  } else {
    vc1 = NULL;
  }

    // DEB_dump_obj__ (Typ_Group, &oWhat,  "what:");
    // DEB_dump_obj__ (Typ_Group, &oWhere, "where:");
    // DEB_dump_obj__ (Typ_VC, vc1, "direction:");


  // return APT_prj_obj_vc (oxo, aus_anz, aus_typ, aus_tab, wrkSpc);


  //================================================================
  // load obj-what
  oWhat.typ = UTO__dbo (&obji, &i1, oWhat.typ, oWhat.dbInd);
  if(oWhat.typ <= 0) return -1;
    // printf(" what.typ %d\n",oWhat.typ);


  //================================================================
  // load obj-where
  oWhere.typ = UTO__dbo (&datWhere, &oNr, oWhere.typ, oWhere.dbInd);
  if(oWhere.typ <= 0) return -1;
    // printf(" where.typ=%d oNr=%d\n",oWhere.typ,oNr);


  // Typ_SURPMSH:
  // load surMeshNr and surPtabNr into ia for projTarget
  if(oWhere.typ == Typ_SUR) {
    if(((ObjGX*)datWhere)->typ == Typ_SURPMSH) {
      oWhere.typ = Typ_SURPMSH;
      ia[0] = oWhere.dbInd;                       // surMeshNr
      ia[1] = OGX_GET_INT ((ObjGX*)datWhere);         // surPtabNr
      datWhere = ia;
    }
  }


  //================================================================
  // create projection (normal projection)
  // load Plane -> transf.Func
  // UTO_obj_appprj (NULL, NULL, NULL, &pl1, NULL);
  // UTO_obj_appprj (Typ_PLN, &pl1, TYP_FuncInit, NULL, NULL);
  UPRJ_def__ (oWhere.typ, datWhere, oNr, vc1);


/*
  // ein Circ wird zu einer Ellipse !!!
  if(oWhat.typ == Typ_CI) typo = Typ_CVELL;
  else                    typo = oWhat.typ;
    printf(" typo=%d\n",typo);


  // get space for output-obj
  if(AP_typDB_typ(typo) == Typ_CV)   {

    // das Ausgabeobjekt im wrkSpc anlegen
    objo = wrkSpc->next;
    i1 = UME_add (wrkSpc, OBJ_SIZ_MAX);
    if(i1 < 0) return -1;

    OGX_SET_OBJ ((ObjGX*)oxo, typo, typo, 1, objo);


  } else { // PT/LN direkt -> oxo
    objo = oxo;
  }
*/

  // get space for outputObj


  // apply projection
  // i1 = UTO_obj_appprj (typo, objo, typi, obji, wrkSpc);
  // oSiz = OBJ_SIZ_MAX;
  // i1 = UPRJ_app__ (objo, &typo, &oSiz, 1, obji, oWhat.typ, wrkSpc);
  i1 = UPRJ_app__ (oxo, 1, obji, oWhat.typ, wrkSpc);
  if(i1 < 0) return i1;

    // DEB_dump_ox_0 (oxo, "ex APT_prj_obj_perp");
    // printf("PPPPPPPPPPPPPPPPPPPPPPPPPP ex APT_prj_obj_perp\n");


  return (oxo->typ);


  GeomErr:
  TX_Error("Project: - Geometry Error");
  return -1;

  ParErr:
  TX_Error("Project: - Parameter Error");
  return -1;

  NotImp_err:
  TX_Error("Project: - not yet implemented Error");
  return -1;

}


//============================================================================
  int APT_mir_obj (ObjGX *oxo,
                  int aus_anz, int aus_typ[], double aus_tab[],
                  Memspc *wrkSpc) {
//============================================================================
// -1 =  Par_err

  int       i1, i2, o0Typ;
  void      *obji, *objo, *mo;


  // printf("APT_mir_obj |%d|\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  // obj[1] ist zu bearbeitendes obj
  o0Typ = aus_typ[1];


  //================================================================
  // obj[2] muss Plane od Line sein
  if(aus_typ[2] == Typ_PLN) {
    mo = DB_get_PLN ((long)aus_tab[2]);

  //----------------------------------------------------------------
  } else if(aus_typ[2] == Typ_LN) {
    mo = DB_get_LN ((long)aus_tab[2]);

  } else goto ParErr;


  //================================================================
  // load mo -> tansf.Func
  UTO_obj_appmir (NULL, aus_typ[2], mo, NULL);


  // load DB-obj
  i1 = UTO_obj_dbo (&obji, &i2, &o0Typ, (long)aus_tab[1]);
  if(i1 < 0) return -1;


  // transform ..
    // ein ObjGX f die Curve -> wrkSpc
    objo = wrkSpc->next;
    i1 = UME_add (wrkSpc, OBJ_SIZ_MAX);
    if(i1 < 0) return -1;

    oxo->typ    = o0Typ;
    oxo->form   = o0Typ;
    oxo->siz    = 1;
    oxo->data   = objo;

    i1 = UTO_obj_appmir (objo, o0Typ, obji, wrkSpc);
    if(i1 < 0) return i1;

      // DEB_dump_obj__ (oxo->form, objo, "ex APT_mir_obj:");


  return 0;

  GeomErr:
  TX_Error("Mirror: - Geometry Error");
  return -1;

  ParErr:
  TX_Error("Mirror: - Parameter Error");
  return -1;

  NotImp_err:
  TX_Error("Mirror: - not yet implemented Error");
  return -1;

}


//============================================================================
  int APT_iso_obj (void *oxo,
                  int aus_anz, int aus_typ[], double aus_tab[],
                  Memspc *wrkSpc) {
//============================================================================
// decode "ISO A# <parVal> [CX]"
// -1 =  Par_err
// Input:
//   oxo      NULL: only return objTyp as RetCod.
// Output:
//   oxo      (ObjGX*)


// see also APT_iso_cktyp

  int       irc, i1, iTyp, oTyp, iDir, sTyp;
  void      *obji, *objo;
  double    dp;
  Point     *pt1;
  ObjGX     *ox1p;

static char     oAux[OBJ_SIZ_MAX];

  // printf("APT_iso_obj |%d|\n",aus_anz);
  // for(i1=0; i1<aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }


  // if(wrkSpc) DEB_dump_obj__ (Typ_Memspc, wrkSpc, "wrkSpc:\n");


  // obj[1] ist inputObj Surf.
  iTyp = aus_typ[1];


    ox1p = DB_GetSur ((long)aus_tab[1], 0);
    if(ox1p->typ == Typ_Error) goto L_noFunc;


  //----------------------------------------------------------------
  // obj[2] - parameterValue
    if(aus_typ[2] == Typ_Val) {
      dp = aus_tab[2];    // parameterValue

    // } else if(aus_typ[2] == Typ_PT) {
      // pt1 = DB_get_PT ((long)aus_tab[2]);
/* see TSU_tr_init_ :3433
ev eine eigene struct fuer die Gittersurf (mit ptab, iu,iv,du,dv)
      // get parameters of point on surf
      UT3D_uvNr_sbsp (&i1, &i2, TSU_sbsp, &wrkSpc);
      UT3D_ptgrid_sbsp (TSU_grid, &du, &dv, TSU_sbsp, i1, i2, &wrkSpc);
      UT3D_parsbsp_pt (&d1, &d2, pt1,
                       TSU_grid, du, dv, iu, iv,
                       ox1p->data, &APTSpcTmp);
*/
    } else goto L_noFunc;
   

  //----------------------------------------------------------------
    if((aus_typ[3] == Typ_cmdNCsub)&&((int)aus_tab[3] == T_CX)) iDir = 2;
    else iDir = 1;   // 1=along, 2=across
      // printf(" dp=%f iDir=%d\n",dp,iDir);



  sTyp = UTO_ck_surfTyp (ox1p);
    // printf(" sTyp=%d dp=%f iDir=%d\n",sTyp,dp,iDir);




  //----------------------------------------------------------------
  if(sTyp == Typ_SURBSP) {            //  printf(" Typ_SURBSP\n");

    oTyp = Typ_CVBSP;

    if(oxo == NULL) goto L_exit;

    // get memspc for curve from APTSpcTmp
    // objo = APTSpcTmp.next;
    // irc = UME_add (&APTSpcTmp, sizeof(CurvBSpl));
    // if (irc < 0) return -1;

    // IsoCurve from BSP-Surf
    // see also UT3D_bspU_sbsp_v UT3D_bspV_sbsp_u
    irc = UCBS_IsoBspCrvBspSur (oAux, &APTSpcTmp, ox1p->data, dp, iDir);
    if (irc < 0) return -1;

    // GR_Disp_CvBSp ((CurvBSpl*)oAux, 9, &APTSpcTmp);



  //----------------------------------------------------------------
  // handle SRU
  } else if(sTyp == Typ_SURRU) {        // printf(" Typ_SURRU\n");

    // get iso-curve
    irc = SRU_CvIso_parsru (oAux, &oTyp, &APTSpcTmp, ox1p, dp, iDir);
    if (irc < 0) return -1;

    if(oxo == NULL) goto L_exit;  // only get outObjTyp

    // put Curves into ObjGX
    if(UTO_ck_curvForm (oTyp) != Typ_ObjGX) {
      memcpy (oxo, oAux, OBJ_SIZ_MAX);
      goto L_exit;
    }
  


  //----------------------------------------------------------------
  // handle SRV
  } else if(sTyp == Typ_SURRV) {        // printf(" Typ_SURRV\n");

    // get iso-curve
    irc = SRV_CvIso_parsrv (oAux, &oTyp, &APTSpcTmp, ox1p, dp, iDir);
    if (irc < 0) return -1;

    if(oxo == NULL) goto L_exit;  // only get outObjTyp

    // put Curves into ObjGX
    if(UTO_ck_curvForm (oTyp) != Typ_ObjGX) {
      memcpy (oxo, oAux, OBJ_SIZ_MAX);
      goto L_exit;
    }


  // //----------------------------------------------------------------
  // // handle SPLN
  // } else if(sTyp == Typ_SURPLN) {          printf(" Typ_SURPLN\n");



  //----------------------------------------------------------------
  } else {
    goto L_noFunc;
  }


    //----------------------------------------------------------------
    ((ObjGX*)oxo)->typ    = oTyp;
    ((ObjGX*)oxo)->form   = oTyp;
    ((ObjGX*)oxo)->siz    = 1;
    ((ObjGX*)oxo)->data   = oAux;
    // DEB_dump_ox_0 (oxo, "");

  L_exit:
  return oTyp;


  L_noFunc:
    TX_Error(" ISO-Objectdefinition not yet implemented ..");
    return -1;

}


//============================================================================
  int APT_prj_cktyp (char *ep[]) {
//============================================================================
// define type of resulting object for projection.

// see also APT_int_cktyp

  int    typ1, typ2, irc, rNr;
  long   dbi1, dbi2;
  void   *o1, *o2;


  printf("APT_prj_cktyp \n");
  printf(" ep0=|%s|\n",ep[0]);
  printf(" ep1=|%s|\n",ep[1]);
  // printf(" ep2=|%s|\n",ep[2]);

  typ1 = AP_typ_typChar (ep[0][0]);
  typ2 = AP_typ_typChar (ep[1][0]);


  //----------------------------------------------------------------
  // test if C -> R gives ellipse (CV,std) or Circ
  if((typ1 == Typ_CI) && (typ2 == Typ_PLN)) {
    dbi1 = atoi(&ep[0][1]);
    dbi2 = atoi(&ep[1][1]);
    // get data
    irc = UTO__dbo (&o1, &rNr, typ1, dbi1);
    if(irc < 0) return -1;
    irc = UTO__dbo (&o2, &rNr, typ2, dbi2);
    if(irc < 0) return -1;
    irc = 0;
    if(UT3D_comp2vc_d(&((Circ*)o1)->vz, &((Plane*)o2)->vz, UT_TOL_min0)) {
        printf(" is circ ..\n");
      irc = Typ_CI;
    }
  }

    printf("ex APT_prj_cktyp %d\n",irc);

  return irc;

}


//============================================================================
  int APT_iso_cktyp (char *ep[]) {
//============================================================================
// define type of resulting object for isoparmetric curve

// see also APT_int_cktyp

  int    typ1, iTyp, ia[4];
  double da[4];
  long   dbi1;


  printf("APT_iso_cktyp \n");
  printf(" ep0=|%s|\n",ep[0]);
  printf(" ep1=|%s|\n",ep[1]);
  printf(" ep2=|%s|\n",ep[2]);

  // [0] = "ISO"
  // [1] = Surf
  // [2] = parameter
  // [3] = "CX"

  typ1 = AP_typ_typChar (ep[0][0]);
  dbi1 = atoi(&ep[0][1]);
    printf(" _cktyp %d %ld\n",typ1,dbi1);

  ia[0] = 0;
  da[0] = 0.;

  ia[1] = typ1;
  da[1] = dbi1;
    printf(" _cktyp %d %f\n",ia[1],da[1]);

  ia[2] = Typ_Val;
  da[2] = 0.;

  ia[3] = Typ_cmdNCsub;
  if(!strcmp(ep[2], "CX")) da[3] = T_CX;
  else                     da[3] = 0.;


  iTyp = APT_iso_obj (NULL, 3, ia, da, NULL);


  //----------------------------------------------------------------

    printf("ex APT_iso_cktyp %d\n",iTyp);

  return iTyp;

}


//============================================================================
  int APT_int_cktyp (char *ep[]) {
//============================================================================
// define type of resulting object for intersect

  int    iTyp, typ1, typ2, i1, i2;
  long   ind;
  char   *so1, *so2;  // string object
  ObjGX  *ox1;


  // printf("APT_int_cktyp |%s|%s|\n",ep[0],ep[1]);

  iTyp = -1;

  // get typ from APT-TypChars (Typ_PT from 'P')
  typ1 = AP_typ_typChar (ep[0][0]);
  typ2 = AP_typ_typChar (ep[1][0]);
    // printf(" typ1=%d typ2=%d\n",typ1,typ2);

  // i1/i2 = Typ_lFig (Curve; (LN|AC|CV))  oder Typ_SUR (PLN|SUR|SOL)
  i1 = UTO_ck_typTyp(typ1);
  i2 = UTO_ck_typTyp(typ2);

  // if((typ1 == surf)&&((typ2 == surf)) result == curve else point.



  // surface x surface
  if((i1 == Typ_SUR)&&(i2 == Typ_SUR)) {

    so1 = ep[0];
    so2 = ep[1];

    // make Plane obj # 1
    if(typ2 == Typ_PLN) {
      MEM_swap_int (&typ1, &typ2);
      MEM_swap_2vp (&so1, &so2);
    }

    // Plane x Surface -
    if(typ1 == Typ_PLN) {
      // PLN x PLN = Line
      if(typ2 == Typ_PLN) {iTyp = Typ_LN; goto L_done;}

      // PLN x Sph = Circ
      if(typ2 == Typ_SOL) {
        ind = atoi(&so2[1]);
        ox1 = DB_GetSol (ind);
          // printf(" o2: ind=%d typ=%d\n",ind,ox1->typ);
        if(ox1->typ == Typ_SPH) {
          iTyp = Typ_CI; goto L_done;
        }
      }
    }


    // surface x surface = curve
    iTyp = Typ_CV;
    goto L_done;


  // Surf X Curv = Point
  } else {
    iTyp = Typ_PT;
    goto L_done;
  }


  //----------------------------------------------------------------



  L_done:
  // printf("ex APT_int_cktyp %d\n",iTyp);
  return iTyp;

}


/*
//============================================================================
  int APT_int_obj (int *oTyp, void *oxo, int *oSiz,
                  int *aus_anz, int aus_typ[], double aus_tab[]) {
//============================================================================
// intersect;                   zB   S21=INT A20 R20 [MOD(3)] [POL]
// oxo hat size OBJ_SIZ_MAX
// use memspc501 for additional data
// see also APT_int_cktyp (define type of resulting object for intersect)
// Output:
//   oTyp    Typ_CVPOL|Typ_CV (corresponding to last par "POL")
//   RetCod:
//      1    Obj not yet created; (parameters prepared)
//      0    Obj created;
//     -1    Error.



  int    i1, i2, to1, to2, io1, io2, no1, no2, imod, outTyp, sTyp[100];
  long   sTab[100];
  Plane  *pl1, *pl2;
  ObjGX  *ox1;


  // printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC \n");
  // printf("APT_int_obj |%d| oSiz=%d\n",*aus_anz,*oSiz);
  // for(i1=0; i1 < *aus_anz; ++i1) {
    // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);
  // }



  //----------------------------------------------------------------
  // if last word is "POL"
  // outTyp (Typ_CVBSP (10) | Typ_CVPOL (8))
  outTyp = Typ_CVBSP;  //  (10) od Typ_CVPOL (8))
  i1 = *aus_anz - 1;
  if(aus_typ[i1] == Typ_cmdNCsub) {
    i2 = aus_tab[i1];
    if(i2 == T_POL) {
      outTyp = Typ_CVPOL;
      --(*aus_anz);
    } else { i1 = 4; goto ParErr; }
  }



  //----------------------------------------------------------------
  // if last word is MOD
  i1 = *aus_anz - 1;
  if(aus_typ[i1] == Typ_modif) {
    imod = aus_tab[i1];
    --(*aus_anz);
  } else {
    imod = 0;
  }
    // printf(" outTyp=%d imod=%d\n",outTyp,imod);



  //----------------------------------------------------------------
  // fix index to obj1 und obj2
  io1 = 1;                   // index of 1. obj of group1
  io2 = 2;
  if(aus_typ[io1] == Typ_Group) {
    no1 = aus_tab[io1];      // nr of objects of group1
    io2 = 2 + no1;
    ++io1;
  } else {
   no1 = 1;
  }
  if(aus_typ[io2] == Typ_Group) {
    no2 = aus_tab[io2];      // nr of objects of group2
    ++io2;
  } else {
    no2 = 1;
  }
    // printf(" io1=%d io2=%d\n",io1,io2);
    // printf(" no1=%d no2=%d\n",no1,no2);

  // get typTyps (Typ_lFig=curv(LN|AC|CV)=82 | Typ_SUR=surf(PLN|SUR|SOL)=150)
  to1 = UTO_ck_typTyp(aus_typ[io1]);
  to2 = UTO_ck_typTyp(aus_typ[io2]);
    // printf(" to1=%d to2=%d\n",to1,to2);

  


  //----------------------------------------------------------------
  // SURF X SURF 
  if((to1 == Typ_SUR)&&(to2 == Typ_SUR)) goto L_int_sur_sur;

  // CURV X SURF
  if((to1 != Typ_lFig)||(to2 != Typ_lFig)) goto L_int_cv_surf;



  //----------------------------------------------------------------
  // 1 or 2 lfigs;
  // result is point(s); modify sourceObj, return
  //   (handle in function APT_decode_pt)
  L_int_cv_x:

  // hier muss die Curve Obj 1 sein und Plane Obj # 2 sein !!!
    // if(aus_typ[io1] == Typ_PLN) {
      if(io1 > io2) {   // indices wurden NICHT vertauscht
        // aus_ record 1 und 2 vertauschen ...
        // ACHTUNG: hier wird nur 1 obj vertauscht; nicht group of obj's !
        MEM_swap_int (&aus_typ[io1], &aus_typ[io2]);
        MEM_swap_2db (&aus_tab[io1], &aus_tab[io2]);
      }
    // }


  // delete record 0 (das INT)
  i1 = *aus_anz;
  MEM_del_nrec (&i1, aus_typ, 0, 1, sizeof(int));
  i1 = *aus_anz;
  MEM_del_nrec (&i1, aus_tab, 0, 1, sizeof(double));
  --(*aus_anz);


  // ein ev geloschtes MOD(imod) wieder zu ..
  if(imod != 0) {
    i1 = *aus_anz;
    aus_typ[i1] = Typ_modif;
    aus_tab[i1] = imod;
    ++(*aus_anz);
  }
    // printf(" ex APT_int_obj %d\n",*aus_anz);
    // for(i1=0; i1<*aus_anz; ++i1)
      // printf(" %d typ=%d tab=%f\n",i1,aus_typ[i1],aus_tab[i1]);

  // an Funktion APT_decode_pt uebergeben ...
  return 1;






  //================================================================
  // 1 surface, 1 lfig
  L_int_cv_surf:

  // surf muss obj # 2 sein !!!
  // if(aus_typ[io1] != Typ_PLN) {
    // if(aus_typ[io2] == Typ_PLN) {
  if(to2 == Typ_lFig) {
    if(to1 == Typ_SUR) {
      // hier werden nur Startindices und Anzahl von Groups vertauscht !
      MEM_swap_int (&io1, &io2);
      MEM_swap_int (&to1, &to2);
      MEM_swap_int (&no1, &no2);
    }
      // printf(" io1=%d io2=%d\n",io1,io2);
      // printf(" to1=%d to2=%d\n",to1,to2);
  }

  goto L_int_cv_x;

/
//  //================================================================
//  // to1 = curve; to2 = surf.
//  // CURVE X SURF
//  // L = INT L R
//  if((aus_typ[io1] == Typ_PLN)     ||
//     (aus_typ[io2] == Typ_PLN))         {
//    // printf(" INT %d-%f %d-%f\n",aus_typ[io1],aus_tab[io1],
//                                // aus_typ[io2],aus_tab[io2]);
//    goto L_int_cv_x;
//
//
//
//
//  //================================================================
//  } else goto NotImp_err;
//
//    printf(" NotImp_err %d-%f %d-%f\n",aus_typ[io1],aus_tab[io1],
//                                       aus_typ[io2],aus_tab[io2]);
/






  //================================================================
  // 2 Typ_SUR:
  L_int_sur_sur:                       // surface x surface

  // L = INT R R
  if((aus_typ[io1] == Typ_PLN)     &&
     (aus_typ[io2] == Typ_PLN))         {

    // printf(" INT R%f R%f\n",aus_tab[io1],aus_tab[io2]);

    pl1 = DB_get_PLN ((long)aus_tab[io1]);
    pl2 = DB_get_PLN ((long)aus_tab[io2]);


    UT3D_ln_int2pl ((Line*)oxo, pl1, pl2);
    UT3D_ln_setLen ((Line*)oxo, &((Line*)oxo)->p1, APT_ln_len, (Line*)oxo);
      // GR_Disp_ln ((Line*)oxo, 2);


     *oTyp = Typ_LN;
     goto Done;




  //================================================================
  // L = INT R A                     // Plane X Surf
  } else if((aus_typ[io1] == Typ_PLN)     &&
            (aus_typ[io2] == Typ_SUR))         {

    pl1 = DB_get_PLN ((long)aus_tab[io1]);
    goto L_RxS;  // intersect Plane X Surf/Sol.





  //================================================================
  // L = INT R B        // Plane X Body
  } else if((aus_typ[io1] == Typ_PLN)     &&
            (aus_typ[io2] == Typ_SOL))         {

    pl1 = DB_get_PLN ((long)aus_tab[io1]);
    ox1 = DB_GetSol ((long)aus_tab[io2]);
    // UTO_obj_get (ox1, ox1);  // resolv Typ_Index
      // DEB_dump_ox_0 (ox1, "o2");


   if(ox1->form == Typ_ObjGX) i1 = ox1->typ;   // zb Typ_SOL ..
   else i1 = ox1->form;

    if(i1 == Typ_SPH) {       // Sphere --> immer Circ
      UT3D_ci_intplsph ((Circ*)oxo,  pl1, ox1->data);
      *oTyp = Typ_CI;

    } else if(i1 == Typ_CON) {
      // Cone: Circ oder Ellipse; Ellipse not yet implemented;
      // daher  nur Circ; else goto Plane X Surf/Sol
      // testen ob KonusAchse parallel zu Plane-Z-Achse
      i1 = INT_intplcon (oTyp, oxo,  pl1, ox1->data);
      if(i1 == -3) goto L_RxS;
      if(i1 < 0) return i1;


    } else if(i1 == Typ_TOR) {
      // Torus: testen ob TorusHauptachse in Plane liegt
      i1 = INT_intpltor (oTyp, oxo,  pl1, ox1->data);
      if(i1 == -3) goto L_RxS;
      if(i1 < 0) return i1;


    } else if(i1 == Typ_SOL) {
      goto L_RxS;  // intersect Plane X Surf/Sol.


    } else {
      printf("- APT_int_obj R|Sol typ=%d form=%d\n",ox1->typ,ox1->form);
      goto NotImp_err;
    }




  //================================================================
  // L = INT A|B A|B                     // Surf X Surf
  } else if(
   ((aus_typ[io1] == Typ_SUR)||(aus_typ[io1] == Typ_SOL)) &&
   ((aus_typ[io2] == Typ_SUR)||(aus_typ[io2] == Typ_SOL)))         {


   // TEST: in xa_test.c
   sTyp[0] = aus_typ[io1];
   sTyp[1] = aus_typ[io2];
   sTab[0] = aus_tab[io1];
   sTab[1] = aus_tab[io2];
   i1 = INT_intsursur (oTyp, oxo, &sTyp[0], sTab[0],
                                  &sTyp[1], sTab[1], imod, outTyp);
   return i1;



  //================================================================
  } else goto NotImp_err;
  goto Done;



  //----------------------------------------------------------------
  // int Plane X Surf/Sol
  L_RxS:
    // alle SurfNr's via Int-Liste uebergeben. aus_typ dazu benutzen !
    if(no2 > 100) {TX_Error("APT_int_obj E001"); return -1;}
    for(i1=0; i1<no2; ++i1) {
      i2 = io2 + i1;
      sTyp[i1] = aus_typ[i2];
      sTab[i1] = aus_tab[i2];
    }
    // ACHTUNG das Tess. benutzt fast alle Speicher; loescht aus_typ usw !
    i1 = INT_intplsur (oTyp, oxo, pl1, sTab, sTyp,no2, imod, outTyp);
    goto Done;



  Done:
  return 0;


  NotImp_err:
  TX_Error("Intersect: - not yet implemented Error");
  return -1;

  ParErr:
  TX_Error("Intersect: - Error Parameter %d",i1);
  return -1;


}
*/


//================================================================
  int APT_IMP__  (Memspc *oSpc,
                  int aus_anz, int aus_typ[], double aus_tab[],
                  Memspc *tSpc1) {
//================================================================
// Import joints.   eg "R20=IMP M20 J1"
// Input:
//   aus_anz/ausTyp/ausTab   atomicObjs
//   oxo    outputfield, sizeof ObjGX
// Output:
//   oSpc   the first ObjGX in oSpc ist the resulting outputObj

  int       irc, i1, ii, ibMdl, iJnt, jTyp;
  long      irMdl;
  double    aa;
  ObjGX     *ox1;
  Mat_4x3   mat1;
  void      *obj;


  // printf("------------------------------------------- \n");
  // printf("APT_IMP__ %d\n",aus_anz);
  // for(i1=0;i1<aus_anz; ++i1) printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);



  // get ModelNr
  ii = 1; // 0=IMP; 1=Model
  if(aus_typ[ii] != Typ_Model) goto L_ParErr;
  irMdl = aus_tab[ii];

  // get matrix of referenceModel 
  Mod_m3_mdr (mat1, irMdl);
    // DEB_dump_obj__ (Typ_M4x3, mat1, " _IMP__-mat1\n");


  //----------------------------------------------------------------
  // get JointNr
  ++ii; // 2=Joint
  if(aus_typ[ii] != Typ_Joint) goto L_ParErr;
  iJnt = aus_tab[ii];
  ++ii;

  // import joint from DB-file
  irc = JNT_imp__ (oSpc, &ibMdl, irMdl, iJnt);
  if(irc < 0) return -1;

  // get ox1 = primary-obj of joint
  ox1 = UME_get_start (oSpc);
    // printf(" ox1.typ=%d bmdlNr=%d\n",ox1->typ,ibMdl);
    // DEB_dump_ox_0 (ox1, " _IMP__-ox1(joint)");

  // get type and pointer to obj
  OGX_GET_OBJ (&jTyp, &obj, &ii, ox1);
    // DEB_dump_obj__ (jTyp, obj, " _IMP__-obj");


  //----------------------------------------------------------------
  if(jTyp == Typ_PT) {
//     // get pointAdress
//     pt1 = ox1->data;

    // transform point
    UT3D_pt_tra_pt_m3 (obj, mat1, obj);


/*
  //----------------------------------------------------------------
  } else if(ox1->typ == Typ_Note) {
    if(jTyp == Typ_GTXT) { 
      // get pointAdress
      pt1 = &((GText*)obj)->pt;
        // DEB_dump_obj__ (Typ_PT, pt1, " pt1\n");

      // transform point
      UT3D_pt_tra_pt_m3 (pt1, mat1, pt1);

      if(aus_typ[ii] == Typ_String) {
        APT_get_String (memspc011, APT_defTxt, aus_tab[ii]);
          // printf(" zusTxt=|%s|\n",memspc011);
        ++ii;
        i1 = strlen (memspc011);
        if(i1 > 0) {
          ++i1;  // save also deli
          // save new text
          vp1 = UME_save (oSpc, memspc011, i1);
          // modify text ..
          ((GText*)obj)->txt = (char*)vp1;
        }
      }

      // get the plane of refModel <irMdl>
      mr1 = DB_get_ModRef (irMdl);

      // get the angle of the x-axis of the referenceModel on the XY-plane;
      aa = UT3D_angr_vc2vc (&mr1->vx, (Vector*)&UT3D_VECTOR_X,
                                      (Vector*)&UT3D_VECTOR_Y);
        // printf(" aa=%f\n",UT_DEGREES(aa));

      // add to GText-angle <dir>
      aa = UT_DEGREES(aa) + ((GText*)obj)->dir;
      UT2D_angd_set (&aa);
      ((GText*)obj)->dir = aa;
        // DEB_dump_obj__ (Typ_GTXT, ox1->data, "mod.note\n");

    } else goto L_err_unsupp;
*/

  //----------------------------------------------------------------
  } else if(jTyp == Typ_PLN) {
    // get refSys of joint (position and orientation in subModel)

//     // get ModelRef of subModel (= active position of subModel in active-model)
//     mr1 = DB_get_ModRef (irMdl);
//       DEB_dump_obj__ (Typ_Model, mr1, " _IMP__-mr1");
//     // mr1.po = origin of subModel in active-model;
//     // mr1.vx and .vz is the orientation.

    // obj = the plane of refModel <irMdl>
    // Transfer coordinates from subModel -> active-model;
    UT3D_pl_tra_m3 (obj, obj, mat1);


  //----------------------------------------------------------------
  } else goto L_err_unsupp;


    // TESTBLOCK
    // printf("ex-APT_IMP__ \n");
    // DEB_dump_ox_0 (ox1, "ex-APT_IMP__-oSpc ");
    // END TESTBLOCK
 

  return 0;


  L_ParErr:
    TX_Error("Joint-import: - Error Parameters");
    return -1;

  L_err_unsupp:
    TX_Error("Joint-import: - obj %d not supported",ox1->form);
    return -1;

}


//================================================================
  int APT_TNG__  (ObjGX *oxo,
                  int aus_anz, int aus_typ[], double aus_tab[],
                  Memspc *tSpc1) {
//================================================================
// tangential line
// see also APT_decode_ln
// TODO: UTO_TNG_cv_cv does same ...

static Line lno;

  int       irc, i1, i2, typ1, typ2, form1, form2, imod, iswap, lTyp, iRev;
  double    d1, d2;
  Point     pt1, pt2;
  Vector    vco, vci;
  ObjGX     o1, o2;
  void      *obj1, *obj2;


  // printf("APT_TNG__ %d\n",aus_anz);
  // for(i1=0;i1<aus_anz;++i1)printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);


  // defaults
  irc = 0;



  // remove last "UNL"  UNL=3|UNL2=2|UNL1=1
  lTyp = APT_decode_modUnlim (&aus_anz, aus_typ, aus_tab);
    // printf(" aus_anz=%d lTyp=%d\n",aus_anz,lTyp);


  // get iRev from last parameter
  i1 = aus_anz - 1;
  if(aus_typ[i1] == Typ_cmdNCsub)     {
    if(aus_tab[i1] == T_REV) {
      iRev = 1;
      --i1;
    }
  } else iRev = 0;



  // get imod from last parameter
  if(aus_typ[i1] == Typ_modif) {
    imod = aus_tab[i1];
    if(imod) --imod;  // make 0=0; 1=0; 2=1; 3=2; ..
    --i1;
  } else {
    imod = 0;
  }

  aus_anz = i1 + 1;


  // change sequence 1,2 -> P D L
  if(aus_typ[1] > aus_typ[2]) {
    iswap = 1;
    ATO_swap (aus_typ, aus_tab, 1, 2);
  } else
    iswap = 0;

  // if(aus_typ[2] == Typ_VC) ATO_swap (aus_typ, aus_tab, 1, 2);
  // if(aus_typ[2] == Typ_LN) ATO_swap (aus_typ, aus_tab, 1, 2);
  // if(aus_typ[2] == Typ_PT) ATO_swap (aus_typ, aus_tab, 1, 2);


  // make complex-objects DB-objects o1 and o2
  OGX_SET_INDEX (&o1, aus_typ[1], (long)aus_tab[1]);
  OGX_SET_INDEX (&o2, aus_typ[2], (long)aus_tab[2]);


  typ1 = aus_typ[1]; 
  typ2 = aus_typ[2]; 
    // printf(" _TNG__ typ1=%d typ2=%d iswap=%d\n",typ1,typ2,iswap);
    // printf(" _TNG__ lTyp=%d imod=%d iRev=%d\n",lTyp,imod,iRev);


  // make input = 2 ObjGX 
  form1 = UTO_objDat_ox (&obj1, &i1, &o1);
  form2 = UTO_objDat_ox (&obj2, &i1, &o2);


  //================================================================
  // L = TNG P P
  if(typ2 == Typ_PT) {
    if(typ1 == Typ_PT) {
      lno.p1 = *(Point*)obj1;
      lno.p2 = *(Point*)obj2;
      lno.typ = 0;
      APT_modMax1 = 1;
      goto L_tng_9;


    //----------------------------------------------------------------
    } else goto L_err1;


  //================================================================
  } else if(typ2 == Typ_CI) {

    //----------------------------------------------------------------
    // L = TNG P C
    if(typ1 == Typ_PT) {
      irc = UT3D_pt_tangptci (&pt1, &pt2, obj1, obj2);
      if (imod) pt1 = pt2;
      UT3D_ln_ptpt (&lno, (Point*)obj1, &pt1);
      if(irc >= 0) APT_modMax1 = irc;
      goto L_tng_9;   // lno


    //----------------------------------------------------------------
    // L = TNG D C
    } else if(typ1 == Typ_VC) {
      UT3D_ln_tng_ci_vc (&lno, obj2, obj1, imod);
      APT_modMax1 = 2;
      goto L_tng_9;


    //----------------------------------------------------------------
    // L = TNG L C
    } else if(typ1 == Typ_LN) {
      UT3D_vc_ln (&vci, (Line*)obj1);  // vc from ln
      UT3D_ln_tng_ci_vc (&lno, obj2, &vci, imod);
      APT_modMax1 = 2;
      goto L_tng_9;


    //----------------------------------------------------------------
    // TNG C C
    } else if(typ1 == Typ_CI) {
      irc = UT3D_ln_tng_ci_ci (&lno, obj1, obj2, imod);
      if(irc >= 0) APT_modMax1 = irc;
      goto L_tng_9;
    }


  //================================================================
  } else if (typ2 == Typ_CV) {


    //----------------------------------------------------------------
    // L = TNG D S                 // tangent with fixed vector
    // L = TNG L S                 // tangent with fixed vector
    if((typ1 == Typ_VC)||(typ1 == Typ_LN)) {
      irc = UTO_TNG_vc_cv (&lno, &o1, &o2, imod, tSpc1);
      APT_modMax1 = 1;
      goto L_tng_9;

    //----------------------------------------------------------------
    // L = TNG P S
    // if(iswap=0) and first obj was point then tng with fixed vector is wanted;
    } else if((typ1 == Typ_PT) && (iswap == 0)) {
      pt1 = DB_GetPoint ((long)aus_tab[1]);
        // DEB_dump_obj__ (Typ_PT,  &pt1, " _TNG__-pt1 i1=%d",i1);
      // get tangent-vector from point on curve
      irc = UT3D_vc_tng_crv_pt (&vco, &pt1, form2, obj2);
        // DEB_dump_obj__ (Typ_VC,  &vco, " _TNG__-vc1 irc=%d",irc);
      // make lno from pt1, vc1
      UT3D_vc_setLength (&vco, &vco, UT_LEN_TNG);
      UT3D_ln_ptvc (&lno, &pt1, &vco);
      lno.typ = 0;
      APT_modMax1 = 1;
      goto L_tng_9;


    // L = TNG P|C|S P|C|S               // not A B
    } else {
      irc = UTO_TNG_cv_cv (&lno, &o1, &o2, imod, tSpc1);
      if(irc >= 0) APT_modMax1 = irc;
      goto L_tng_9;
    }


  //================================================================
  } else goto L_NotImp;



  //----------------------------------------------------------------
  L_tng_9:                         // Input: lno
      // DEB_dump_obj__ (Typ_LN,  &lno, "  APT_TNG__-lno9");
    if(irc < 0) {
      // APT_modMax1 = 0;   keep 
      goto L_err1;
    }

    // work iRev
    if(iRev) {
      UT3D_pt_opp2pt (&lno.p2, &lno.p1, &lno.p2);
    }

    // work "UNL" unlimited ..
    if(lTyp != 0) {
      UT3D_ln_unlim (&lno, lTyp);
      APT_subTyp = Typ_Att_dim;
    }


    OGX_SET_OBJ (oxo, Typ_LN, Typ_LN,  1, &lno);
      // DEB_dump_obj__ (Typ_LN,  &lno, "ex-APT_TNG__");


/*
  // swap objects if 2. obj is point
  if(aus_typ[2] == Typ_PT) ATO_swap (aus_typ, aus_tab, 1, 2);
  if(aus_typ[1] == Typ_PT) goto L_o1_pt;

  if(aus_typ[2] == Typ_LN) ATO_swap (aus_typ, aus_tab, 1, 2);
  if(aus_typ[1] == Typ_LN) goto L_o1_ln;

  if(aus_typ[2] == Typ_VC) ATO_swap (aus_typ, aus_tab, 1, 2);
  if(aus_typ[1] == Typ_VC) goto L_o1_vc0;



  //================================================================
  // obj-1 is line
  L_o1_ln:
  // make vector vci from line aus[1]
  obj1 = DB_get_LN ((long)aus_tab[1]);
  UT3D_vc_ln (&vci, obj1);
  goto L_o1_vc1;


  //================================================================
  // obj-1 is vector
  L_o1_vc0:
  vci = DB_GetVector ((long)aus_tab[1]);

  //----------------------------------------------------------------
  L_o1_vc1:
  typ2 = aus_typ[2];
  obj1 = &vci;
  UT3D_vc_setLength (&vci, &vci, APT_ln_len);
    DEB_dump_obj__ (Typ_VC, &vci, "  vci:");
  if(typ2 == Typ_CI) {
    // TNG D C
    obj2 = DB_get_CI ((long)aus_tab[2]);
    // get tangent-point
    UT3D_pt_tng_ci_vc (&pt1, obj2, obj1, imod);
    // if((imod % 2) > 0) UT3D_vc_invert (&vci, &vci); 
    if((imod > 0)&&(imod < 3)) UT3D_vc_invert (&vci, &vci); 
    // create line lno
    UT3D_ln_ptvc (&lno, &pt1, &vci);
    goto L_out;

  //----------------------------------------------------------------
  // TNG D S
  } else if(typ2 == Typ_CV) {
    OGX_SET_INDEX (&o1, typ2, (long)aus_tab[2]);
    typ2 = UTO_objDat_ox (&obj2, &o1);

  //----------------------------------------------------------------
  // TNG D ellipse
    if(typ2 == Typ_CVELL) {
      // tangent vector - ellipse
      i1 = UT3D_pt_tng_ell_vc__ (&pt1, obj1, obj2, imod);
      if(i1 < 0) goto L_Par_err;
      UT3D_pt_traptvclen (&pt2, &pt1, obj1, APT_ModSiz);
      UT3D_ln_2pt (&lno, &pt1, &pt2);
      APT_modMax1 = 2;
      goto L_out;


  //----------------------------------------------------------------
  // TNG D bSpline
    } else if(typ2 == Typ_CVBSP) {
      // tangent vector - bSpline
      i1 = UT3D_pt_tng_cbsp_vc__ (&pt1, obj1, obj2, imod);
      if(i1 < 0) goto L_Par_err;
      UT3D_pt_traptvclen (&pt2, &pt1, obj1, APT_ModSiz);
      UT3D_ln_2pt (&lno, &pt1, &pt2);
      goto L_out;


    //----------------------------------------------------------------
    } else {
      goto L_err1;
    }

  //----------------------------------------------------------------
  } else {
    goto L_err1;
  }


  //================================================================
  // obj-1 is point
  L_o1_pt:
  obj1 = (void*)DB_get_PT ((long)aus_tab[1]);
  typ2 = aus_typ[2];

  //----------------------------------------------------------------
  // TNG P C
  if(typ2 == Typ_CI) {
    obj2 = DB_get_CI ((long)aus_tab[2]);

    APT_modMax1 = UT3D_pt_tangptci (&lno.p2, &lno.p1, obj1, obj2);
    if(APT_modMax1 < 0) goto L_Par_err;
    if(APT_modMax1 == 1) {  // 1 point
      if(imod < 1) UT3D_pt_opp2pt (&lno.p2, obj1, &lno.p2);
      APT_modMax1 = 2;
    } else {  // 2 points
      if(imod < 1) lno.p2 = lno.p1;
    }
    lno.p1 = *(Point*)obj1;
    goto L_out;


  //----------------------------------------------------------------
  // TNG P S
  } else if(typ2 == Typ_CV) {
    OGX_SET_INDEX (&o1, typ2, (long)aus_tab[2]);
    typ2 = UTO_objDat_ox (&obj2, &o1);

  //----------------------------------------------------------------
  // TNG P ellipse
    if(typ2 == Typ_CVELL) {
      i1 = UT3D_vc_tng_elpt__ (&vco, obj1, obj2, imod);
      if(i1 < 0) goto L_Par_err;
      if(i1 > 0) UT3D_vc_multvc (&vco, &vco, APT_ln_len);
      APT_modMax1 = 2;
      UT3D_ln_ptvc (&lno, (Point*)obj1, &vco);
      goto L_out;


  //----------------------------------------------------------------
  // TNG P bSpline
    } else if(typ2 == Typ_CVBSP) {
      // tangent point - bSpline
      i1 = UT3D_vc_tng_cbsp_pt__ (&vco, obj1, obj2, imod);
      if(i1 < 0) goto L_Par_err;
      UT3D_ln_ptvc (&lno, (Point*)obj1, &vco);
      goto L_out;


    //----------------------------------------------------------------
    } else {
      goto L_err1;
    }


  //----------------------------------------------------------------
  } else {
    goto L_err1;
  }


  //================================================================
  L_ci1:
  if(typ1 != Typ_CI) goto L_err1;
    obj1 = DB_get_CI  ((long)aus_tab[1]);
  if(typ2 != Typ_CI) goto L_err1;
    obj2 = DB_get_CI  ((long)aus_tab[2]);


  //================================================================
    // TNG C C 
    ++imod;
    APT_modMax1 = UT3D_ln_tng_ci_ci (&lno, obj1, obj2, imod);
      printf(" APT_modMax1=%d\n",APT_modMax1);
    if(APT_modMax1 < 0) goto L_Par_err;
    goto L_out;


  
  //================================================================
  L_out:
  OGX_SET_OBJ (oxo, Typ_LN, Typ_LN,  1, &lno);
*/

    // printf("ex APT_TNG__ APT_modMax1=%d\n",APT_modMax1);

  return 0;


  L_err1:
  TX_Error("Definition Tangent: Error\n");
    goto L_Error;

  L_Par_err:
    TX_Error("Linedefinition: ParameterError ****");
    goto L_Error;

  L_NotImp:
    TX_Error("Linedefinition: NOT YET IMPLEMENTED ****");
    // goto L_Error;

  L_Error:
    return -1;

}


//================================================================
  int APT_INT__  (ObjGX *oxo, Memspc *oSpc,
                  int aus_anz, int aus_typ[], double aus_tab[],
                  Memspc *tSpc1) {
//================================================================
// Intersection
// Curv-Curv-Int. returns points;
// Surf-Surf-Int. returns curve.
// Input: atomicObjs in  aus_anz/ausTyp/ausTab; last word 
//  from source eg "INT A20 R20 [MOD(3)] [POL]"
// Input:
//   aus_anz/ausTyp/ausTab   atomicObjs
//   oxo    outputfield, sizeof ObjGX
// Output:
//   oxa    resulting obj
//   RetCod 
//   APT_modMax1 global
//
// was APT_int_obj
// see also UTO_obj_int UNUSED
// see also UTO_obj_int, UTO_stru_int



  int     dbTyp1, dbTyp2, oTyp1, oTyp2, *typ1Tab, *typ2Tab, iMaxSol;
  long    dbInd1, dbInd2, *dbi1Tab, *dbi2Tab;
  int     irc, i1, i2, ii, oTyp, imod, outTyp, pNr;
  int     io1, no1, io2, no2, to1, to2;
  void    *oDat, *o1, *o2;
  double  va[20];
  Point   pa[20];
  ObjGX   ox1, ox2;



  // printf("iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii \n");
  // printf("APT_INT__ %d\n",aus_anz);
  // for(i1=0;i1<aus_anz; ++i1) printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);



  //----------------------------------------------------------------
  // get outTyp; Typ_CVPOL if last word is "POL"; else Typ_CVBSP
  // outTyp (Typ_CVBSP (10) | Typ_CVPOL (8))
  outTyp = Typ_CVBSP;  //  (10) od Typ_CVPOL (8))
  i1 = aus_anz - 1;
  if(aus_typ[i1] == Typ_cmdNCsub) {
  
    i2 = aus_tab[i1];
    if(i2 == T_POL) {
      outTyp = Typ_CVPOL;
      --aus_anz;
    } else { i1 = 4; goto ParErr; }
  }
    // printf(" _INT__-outTyp=%d\n",outTyp);



  //----------------------------------------------------------------
  // get imod; value from last word MOD; else default = 0;
  i1 = aus_anz - 1;
  if(aus_typ[i1] == Typ_modif) {
    imod = aus_tab[i1];
    if(imod) --imod;  // make 0=0; 1=0; 2=1; 3=2; ..
    --aus_anz;
  } else {
    imod = 0;
  }
    // printf(" _INT__-outTyp=%d imod=%d\n",outTyp,imod);



  //----------------------------------------------------------------
  // get startIndex io# and objNr no# of objGrp 1
  ii = 1;
  APT_grp_decode1 (&io1, &no1, &ii, aus_typ, aus_tab);

  // get startIndex and objNr of objGrp 2
  APT_grp_decode1 (&io2, &no2, &ii, aus_typ, aus_tab);

    // printf(" io1=%d io2=%d\n",io1,io2);
    // printf(" no1=%d no2=%d\n",no1,no2);




  //----------------------------------------------------------------
  // get types (Typ_lFig=curv(LN|AC|CV)=82 | Typ_SUR=surf(PLN|SUR|SOL)=150)
  dbTyp1 = aus_typ[io1];
  dbTyp2 = aus_typ[io2];
    // printf(" dbTyp1=%d dbTyp2=%d\n",dbTyp1,dbTyp2);

  dbInd1 =  aus_tab[io1];
  dbInd2 =  aus_tab[io2];
    // printf(" dbInd1=%ld dbInd2=%ld\n",dbInd1,dbInd2);

  to1 = UTO_ck_typTyp(dbTyp1);
  to2 = UTO_ck_typTyp(dbTyp2);
    // printf(" to1=%d to2=%d\n",to1,to2);


  // get datastructs from typ, DB-index; for curves we need the subtyp
  oTyp1 = UTO__dbo (&o1, &i1, dbTyp1, dbInd1);
    // DEB_dump_obj__ (oTyp1, o1, " o1 nach UTO__dbo");


  if(dbTyp2 == Typ_Val) {
    oTyp2 = Typ_Val;
    o2 = &aus_tab[io2];

  } else {
    oTyp2 = UTO__dbo (&o2, &i1, dbTyp2, dbInd2);
      // DEB_dump_obj__ (oTyp2, o2, " o2 nach UTO__dbo");
  }

    // printf(" oTyp1=%d oTyp2=%d xxxxxxxxxxxxxxxxxxxxxxxxxx\n",oTyp1,oTyp2);
    // DEB_dump_obj__ (oTyp1, o1, " o1-1:");
    // DEB_dump_obj__ (oTyp2, o2, " o2-1:");


  // separate (result = points) from (result = curve)
  // Curv-Curv|Curv-Surf returns points;  Surf-Surf returns curve.


  // ck SURF X SURF 
  if((to1 == Typ_SUR)&&(to2 == Typ_SUR)) goto L_int_sur_sur;
  // intersect CURV X SURF | CURV X CURV;  result = points
  if((dbTyp1 == Typ_SUR)||(dbTyp2 == Typ_SUR)) goto L_int_crv_sur;
  if((dbTyp1 == Typ_SOL)||(dbTyp2 == Typ_SOL)) goto L_int_crv_sol;



  //================================================================
  // intersect Curve X Curve
  // intersect 2 structs; result = points
  OGX_SET_INDEX (&ox1, dbTyp1, dbInd1);
  OGX_SET_INDEX (&ox2, dbTyp2, dbInd2);
  pNr = 0;
  irc = UTO_npt_int_2ox (&pNr, pa, NULL, 20, 0, &ox1, &ox2, tSpc1);
    // printf("ex _npt_int_2ox irc=%d pNr=%d\n",irc,pNr);
  if(irc < 0) return irc;


  iMaxSol = pNr;  // keep nr of sulutions ..

  // create outputObjects
  oDat = UME_reserve (oSpc, sizeof(Point));
  memcpy (oDat, &pa[imod], sizeof(Point));
  oTyp = Typ_PT;
  OGX_SET_OBJ (oxo, oTyp, oTyp,  1, oDat);


  goto L_exit;



  //================================================================
  L_int_crv_sur:    // intersect CRV X SUR; result = points
  if(dbTyp1 == Typ_SUR) {   // swap
     MEM_swap_int (&oTyp1, &oTyp2);
     MEM_swap_2vp (&o1, &o2);
  }
  pNr = 20; // maxNr
  irc = UTO_INT_crv_sur (&pNr, pa,
                          oTyp1, o1,  o2,             // crv, sur
                          tSpc1);
  if(irc < 0) return irc;
  iMaxSol = pNr;

  // create outputObjects
  oDat = UME_reserve (oSpc, sizeof(Point));
  memcpy (oDat, &pa[imod], sizeof(Point));
  oTyp = Typ_PT;
  OGX_SET_OBJ (oxo, oTyp, oTyp,  1, oDat);

  goto L_exit;



  //================================================================
  L_int_crv_sol:    // intersect CRV X SOL; result = points
  goto NotImp_err;



  //================================================================
  L_int_sur_sur:    // intersect SURF X SURF; result = curve


  // intersect 2 surfs; result = curve
  // make 2 lists of db-obj's 
  typ1Tab = (int*) MEM_alloc_tmp ((int)(no1 * sizeof(int)));
  dbi1Tab = (long*) MEM_alloc_tmp ((int)(no1 * sizeof(long)));
  APT_grp_decode2 (typ1Tab, dbi1Tab, io1, no1, aus_typ, aus_tab);

  typ2Tab = (int*) MEM_alloc_tmp ((int)(no2 * sizeof(int)));
  dbi2Tab = (long*) MEM_alloc_tmp ((int)(no2 * sizeof(long)));
  APT_grp_decode2 (typ2Tab, dbi2Tab, io2, no2, aus_typ, aus_tab);

  oDat = (char*) MEM_alloc_tmp (OBJ_SIZ_MAX);

  iMaxSol = UTO_INT_susu (&oTyp, &oDat,
                          typ1Tab, dbi1Tab, no1,
                          typ2Tab, dbi2Tab, no2,
                          imod, outTyp, tSpc1);
  if(oTyp < 1) return -1;
  if(iMaxSol < 0) return -1;

    // DEB_dump_obj__ (oTyp, oDat, "ex APT_INT__");


  // copy oDat -> permanent space
  memcpy (memspc011, oDat, OBJ_SIZ_MAX);

  // create primary outputObject
  if(oTyp == Typ_CV) {
    OGX_SET_OBJ (oxo, oTyp, Typ_ObjGX,  1, memspc011);
  } else {
    OGX_SET_OBJ (oxo, oTyp, oTyp,  1, memspc011);
  }


  L_exit:
  // report nr of solutions
  APT_set_modMax (iMaxSol);

    // DEB_dump_ox_0 (oxo, "ex APT_INT__");


  return 0;

  NotImp_err:
  TX_Error("Intersect: - not yet implemented Error");
  return -1;

  ParErr:
  TX_Error("Intersect: - Error Parameter %d",i1);
  return -1;

}


//================================================================
  int APT_PARL__  (ObjGX *oxo, Memspc *oSpc,
                  int aus_anz, int aus_typ[], double aus_tab[],
                  Memspc *tSpc) {
//================================================================
/// \code
/// offset-curve or surface
/// S42=PARL S34 VAL(18) [R20]   [2]        [POL]
///              offset [plane] [outCrvNr] [rounded]
///
/// Input:    
///   aus_*   object to be offset (surf or curv), basic surf (default: Constr.Plane)
/// Output:
///   oxo     new (parallel) obj
/// RetCod:   0     OK;
///          -3     object not yet complete
///   APT_modMax1 (global) nr of solutions
///
/// oSpc=memspc201; tSpc=memspc501
/// \endcode

// TODO: replace WC_sur_act.vz with inputPlane

  int     irc, ii, oNr, typSu, typCv, iMod=0, iAux=0;
  long    dbi;
  void    *oCv, *oo1, *oPln;
  double  dist, d1, d2;
  Vector  vc1, vc2;
  Plane   pl1;
  ObjGX   oxCv, oxSur;


  // printf("---------------------------------------------- \n");
  // printf("APT_PARL__ %d\n",aus_anz);
  // for(ii=0; ii<aus_anz; ++ii) {
    // printf(" %d typ=%d tab=%f\n",ii,aus_typ[ii],aus_tab[ii]);
  // }

  APT_modMax1 = 0;


  //----------------------------------------------------------------
  // get oxCv = curve to offset ..
  ii = 1;
    typCv = aus_typ[ii];
    dbi = aus_tab[ii];
    // get data-struct from dbo
    irc = UTO_obj_dbo (&oCv, &oNr, &typCv, dbi);
    if(irc) goto L_err1;
    OGX_SET_OBJ (&oxCv, typCv, typCv, oNr, oCv);
      // DEB_dump_ox_0 (&oxCv, " APT_PARL__-oxCv");


  //----------------------------------------------------------------
  // get offset-distance
  ++ii;
  if(aus_typ[ii] == Typ_Val) {
    dist = aus_tab[ii];
  } else goto L_err1;


  //----------------------------------------------------------------
  // remove last parameter "POL"
  ii = aus_anz-1;
  if(aus_typ[ii] == Typ_cmdNCsub) {
    if(aus_tab[ii] == T_POL) {
      iAux = 1;
      --ii;
    }
  }


  //----------------------------------------------------------------
  // get last modifiers as iMod
  if(aus_typ[ii] == Typ_modif)  {
    iMod = aus_tab[ii];
    --ii;
  }
    // printf(" iMod=%d iAux=%d\n",iMod,iAux);


  //----------------------------------------------------------------
  // get last par = basic-surface
  if(aus_typ[ii] == Typ_PLN)  {

    // get data-struct from dbo
    typSu = aus_typ[ii];
    dbi = aus_tab[ii];
    irc = UTO_obj_dbo (&oPln, &oNr, &typSu, dbi);
    if(irc) goto L_err1;
    OGX_SET_OBJ (&oxSur, typSu, typSu, 1, oPln);
    --ii;

  } else if(aus_typ[ii] == Typ_SUR)  {
    return MSG_ERR__ (ERR_func_not_impl, "offsetcurve on surface");

  } else {
    // no plane defined; get plane from curve
    irc = UT3D_pl_obj (&pl1, typCv, oCv, oNr);
    if(irc) goto L_err1;
    OGX_SET_OBJ (&oxSur, Typ_PLN, Typ_PLN, 1, &pl1);
  }
    // DEB_dump_ox_0 (&oxSur, " APT_PARL__-oxSur");


  //----------------------------------------------------------------
  // test for special-case:
  // test for line + sharp corners ("POL")
  // test for circ-360-deg
  // test for circ + sharp corners ("POL")
  if(typCv == Typ_LN) {
    if(iAux) goto L_line;

  } else if(typCv == Typ_CI) {
    if(!UT3D_ck_ci360(oCv)) goto L_circ;
    if(iAux) goto L_circ;
  }


  //----------------------------------------------------------------
  // NOT-YET: sharp corners;
  if(iAux) return MSG_ERR__ (ERR_func_not_impl, "COFF2-sharp-edges");

  // get offsetContour nr. <iMod>
  // iMod=0 returns all contours; not allowed as single CCV; get first loop
  if(!iMod) iMod = 1;
  irc = CVOFF2__ (oxo, oSpc, &oxCv, &oxSur, iMod, dist);
  if(irc < 0) {
    TX_Error("APT_PARL__ E001 %d",typCv);
    return -1;
  }
  APT_modMax1 = irc;
      

  //----------------------------------------------------------------
  L_exit:
    // TESTBLOCK
    // if(irc >= 0) DEB_dump_ox_0 (oxo, "  ex-APT_PARL__");
    // END TESTBLOCK


  return irc;


  //----------------------------------------------------------------
  L_circ:
    // get space for circ in oSpc
    oo1 = UME_reserve (oSpc, sizeof(Circ));
    // get d2 = new rad
    d1 = ((Circ*)(oCv))->rad;
    if(d1 > 0.) d2 = d1 - dist;
    else        d2 = dist - d1;
    UT3D_ci_cirad (oo1, oCv, d2);
    OGX_SET_OBJ (oxo, Typ_CI, Typ_CI, 1, oo1);
    goto L_exit;


  //----------------------------------------------------------------
  L_line:
    // make offsetted line
    // get space for Line in oSpc
    oo1 = UME_reserve (oSpc, sizeof(Line));
    // get vz out of plane in oxSur
    if(oxSur.form != Typ_PLN) goto L_err1;
    oPln = oxSur.data;
    UT3D_ln_tra_parl (oo1, oCv, dist, &((Plane*)oPln)->vz);
    OGX_SET_OBJ (oxo, Typ_LN, Typ_LN, 1, oo1);
    goto L_exit;


  //----------------------------------------------------------------
  L_err1:
    return MSG_ERR__ (ERR_func_not_impl, "at Parameter %d",ii);

}


//================================================================
  int APT_REV__  (ObjGX *oxo, Memspc *oSpc,
                  int aus_anz, int aus_typ[], double aus_tab[]) {
//================================================================
/// \code
/// APT_REV__                      reverse curve (trimmed-curve, bwd)
/// obj = REV obj_to_reverse
/// Resulting objType is alwas CurvCCV
///
/// Input:    
///   aus_*   object to te trimmed (surf or curv), trimming obj (surf or curv)
/// Output:
///   oxo     new (trimmed) obj (CurvCCV)
/// RetCod:   0     OK;
///          -3     object not yet complete
///   APT_modMax1 (global) nr of solutions
/// \endcode


  int     iTyp, oNr;
  long    iDbi;
  void    *oCv, *iCv;


  // get oi = obj_to_reverse
  // aus[1] ist das obj to cut (typ/index)
  // UTO_obj_ato1 (&oi, &inxt, aus_typ, aus_tab);
  iTyp = aus_typ[1];
  iDbi = aus_tab[1];


    // printf("RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR \n");
    // printf("APT_REV__ %d %ld\n",iTyp,iDbi);

  //----------------------------------------------------------------
  // make trimmed-curve
  // get spc for CurvCCV in oSpc
  oCv = UME_reserve (oSpc, sizeof(CurvCCV));

  // change db-obj -> CurvCCV
  CVTRM__dbo (oCv, iTyp, iDbi);
    // DEB_dump_obj__ (Typ_CVTRM, oCv, " ex-CVTRM__dbo-2");

  // reverse output-object oCv
  UTO_stru_inv (Typ_CVTRM, oCv);
    // DEB_dump_obj__ (Typ_CVTRM, oCv, " ex-UTO_stru_inv-2");

  // complexObject (ObjGX) from binObj (struct)
  OGX_SET_OBJ (oxo, Typ_CVTRM, Typ_CVTRM, 1, oCv);
  // goto L_exit;


  //----------------------------------------------------------------
  // L_exit:

    // DEB_dump_ox_0 (oxo, "ex APT_REV__");

  return 0;


  //----------------------------------------------------------------
  L_ParErr:
    TX_Error("APT_REV__: Trim: - Parameter Error");
    return -1;


}


//================================================================
  int APT_CUT__  (ObjGX *oxo, Memspc *oSpc,
                  int aus_anz, int aus_typ[], double aus_tab[],
                  Memspc *tSpc1) {
//================================================================
/// \code
/// Cut (trim) object
/// obj = CUT obj CuttingElement (variant) (CuttingElement (variant))
/// Resulting objType is alwas the same as the obj to trim (1. parameter)
/// "CUT S20 L20"     // trim S20
/// "CUT L20 S20"     // trim L20
///   variant:  MODE(1) or MODE(2)  // MODE(1) gives aus_typ=175; aus_tab=0.;
///
/// Input:    
///   aus_*   object to te trimmed (surf or curv), trimming obj (surf or curv)
/// Output:
///   oxo     new (trimmed) obj (LN|AC|CV|Surf|Body)
/// RetCod:   0     OK;
///          -3     object not yet complete
///   APT_modMax1 (global) nr of solutions
/// \endcode
 

// see UTO_obj_trim UTO_CUT__

  int     irc, i1, iTyp, oTyp, imod, inxt, irev;
  long    ii, iDbi;
  ObjGX   oc1, oc2;
  ObjDB   odbi;
  CurvCCV *oCv;
  void    *pDat;


  
  // printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCC  APT_CUT__ %d\n",aus_anz);
  // for(i1=0;i1<aus_anz; ++i1) printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);


  //----------------------------------------------------------------
  // remove last parameter "REV"
  ii = aus_anz-1;
  irev = 0;
  if(aus_typ[ii] == Typ_cmdNCsub) {
    if(aus_tab[ii] == T_REV) {
      irev = 1;
      --aus_anz;
    } else goto L_ParErr;
  }



  //----------------------------------------------------------------
  // load aus_typ/tab:
  //  get objToCut -> oi
  //  get 1. CuttingObj -> oc1
  //  get 2. CuttingObj -> oc2   (= not defined)
  //  get modifier for 1. CuttingObj -> imod1   (0 = not defined; 1=first sol ..)
  //  get modifier for 2. CuttingObj -> imod2   (0 = not defined; 1=first sol ..)

  // aus[0] ist 170/30
  inxt = 1;
  imod = 0;


  // get spc for CurvCCV in oSpc
  oCv = UME_reserve (oSpc, sizeof(CurvCCV));


  // get oi = obj to cut
  // aus[1] ist das obj to cut (typ/index)
  // UTO_obj_ato1 (&oi, &inxt, aus_typ, aus_tab);
  iTyp = aus_typ[inxt];
  iDbi = aus_tab[inxt];
  // OGX_ox_ato1 (&oi, &aus_typ[inxt], &aus_tab[inxt]);
  ODB_set_odb (&odbi, iTyp, iDbi);
  ++inxt;


  // get oc1 = 1. cuttingObj
  if(inxt >= aus_anz) goto L_rev_i;
  // aus[2] ist das 1. cuttingObj, typ/index  oder Typ_Val/val.
  // cuttingObj1 und cuttingObj2 duerfen nicht gleich ObjToTrim sein
  if(APT_cmp_ato2 (1, 2, aus_typ, aus_tab) == 0) goto L_ParErr;
  // UTO_obj_ato1 (&oc1, &inxt, aus_typ, aus_tab);
  OGX_ox_ato1 (&oc1, &aus_typ[inxt], &aus_tab[inxt]);
  ++inxt;


  // get oc2 = 2. cuttingObj
  // [aus[3] = 2. CuttingObj]
  oc2.typ = Typ_Error;
  if(inxt >= aus_anz) goto L_trim__;
  if(aus_typ[inxt] != Typ_modif)  {
    if(APT_cmp_ato2 (1, inxt, aus_typ, aus_tab) == 0) goto L_ParErr;
    // get 2. cuttingObj
    // UTO_obj_ato1 (&oc2, &inxt, aus_typ, aus_tab);
    OGX_ox_ato1 (&oc2, &aus_typ[inxt], &aus_tab[inxt]);
    ++inxt;
  }

  // [aus[4] = modifier for 2. CuttingObj]
  if(inxt >= aus_anz) goto L_trim__;
  if(aus_typ[inxt] == Typ_modif)  {
    imod = aus_tab[inxt];
    // ++inxt;
  }




  //----------------------------------------------------------------
  L_trim__:
  // trim obj
    // printf(" L_trim__: odbi=%d oc1=%d oc2=%d imod=%d\n",
           // odbi.typ,oc1.typ,oc2.typ,imod);


  // irc = UTO_CUT__ (oxo, &oi, &oc1, &oc2, imod, tSpc1);
  irc = UTO_CUT__ (oCv, &odbi, &oc1, &oc2, imod, tSpc1);
    // printf(" ex UTO_CUT__ %d\n",irc);
  if(irc < 0) return irc;

  // complexObject (ObjGX) from binObj (struct)
  OGX_SET_OBJ (oxo, Typ_CVTRM, Typ_CVTRM, 1, oCv);

  // report nr of solutions
  APT_set_modMax (irc);

    // DEB_dump_ox_0 (oxo, "L_trim__");

  goto L_rev_o;



  //----------------------------------------------------------------
  L_rev_i:
    // reverse input-object odbi
      // printf(" L_rev_i: irev=%d\n",irev);
    // change obj -> CurvCCV
    CVTRM__dbo (oCv, iTyp, iDbi);
    // reverse output-object oCv
    UTO_stru_inv (Typ_CVTRM, oCv);
    // complexObject (ObjGX) from binObj (struct)
    OGX_SET_OBJ (oxo, Typ_CVTRM, Typ_CVTRM, 1, oCv);
    goto L_exit;



  //----------------------------------------------------------------
  L_rev_o:
    // reverse output-object oxo
    if(irev) {
      UTO_stru_inv (Typ_CVTRM, oCv);
        // printf(" L_rev_o: irev=%d\n",irev);
    }



  //----------------------------------------------------------------
  L_exit:

    // TESTBLOCK
    // DEB_dump_ox__ (oxo, "ex-APT_CUT__");
    // DEB_dump_ox_0 (oxo, "ex-APT_CUT__");
    // printf("ex-APT_CUT__ CCCCCCCCCCCCCCCCCCCCCCCCCCCC  APT_CUT__\n");
    // return MSG_ERR__ (ERR_TEST, "ex-APT_CUT__");
    // END TESTBLOCK

  return 0;


  //----------------------------------------------------------------
  L_ParErr:
    TX_Error("APT_CUT__: Trim: - Parameter Error");
    return -1;


}
 

//=======================================================================
  int APT_grp_decode1 (int *iStart, int *iNr,
                       int *iPos, int *aus_typ, double *aus_tab) {
//=======================================================================
// get startIndex and objNr of objGrp 1
// returns index of next (free) position in aus_tab


  // printf("APT_grp_decode1 iPos=%d\n",*iPos);


  if(aus_typ[*iPos] == Typ_Group) {
    *iStart = *iPos + 1;
    *iNr = aus_tab[*iPos];                 // nr of objects of group1
    *iPos = *iStart + *iNr;


  } else {
   *iStart = *iPos;
   *iNr = 1;
    *iPos += 1;
  }


  // printf("ex APT_grp_decode1 iStart=%d iNr=%d\n",*iStart,*iNr);

  return 0;

}


//=======================================================================
  int APT_grp_decode2 (int *ia, long *la,
                       int iStart, int iNr,
                       int *aus_typ, double *aus_tab) {
//=======================================================================
// make a list of db-obj's (copy iNr records from aus_typ & aus_tab)
// you must provide memspace for i1 & la !
// ia = (int*) MEM_alloc_tmp (iNr * sizeof(int));
// la = (long*) MEM_alloc_tmp (iNr * sizeof(long));
// Input:
//   iStart    startPos in aus..
//   iNr       nr of objs
// Output:
//   ia
//   la

  
  
  int    i1;
  // int    *ia;
  // long   *la;


  // *typTab = UME_reserve (oSpc, iNr * sizeof(int));
  // ia = *typTab;
  // *dbiTab = UME_reserve (oSpc, iNr * sizeof(long));
  // la = *dbiTab;

  for(i1=0; i1<iNr; ++i1) {
    ia[i1] = aus_typ[iStart];
    la[i1] = aus_tab[iStart];
    ++iStart;
  }
  
  return 0;
  
} 


//================================================================
  int APT_cmp_ato2 (int io1, int io2, int *aus_typ, double *aus_tab) {
//================================================================
// compare 2 atomic objects (DB-Objects only) if equal
// RetCod  0   aus..[io1] == aus..[io2] - is the same
// RetCod -1   aus..[io1] != aus..[io2] - is different

  int i1, i2;

  if(aus_typ[io1] != aus_typ[io2]) return -1;

  i1 = aus_tab[io1];
  i2 = aus_tab[io2];
  if(i1 != i2) return -1;

  return 0;  // is equal

}


//=========================================================================
  int APT_solv_vc_mod (Vector *vco, Vector *vci, Vector *vcn, int imod) {
//=========================================================================
// APT_solv_vc_mod    perp. vector from vector, normal-vector, mod
// Input:
//   vcn     vector normal to vector vci (= imod-1 / lateral left)
//   imod:   1, 2 lateral left / right; 3, 4 top / bottom. 0=undefined=1
//           


  if        (imod < 2)  {   // 0,1 left
    *vco = *vcn;

  } else if (imod == 2) {   // 2   right
    UT3D_vc_invert (vco, vcn);

  } else if (imod == 3) {   // 3   top
    // make z from x=vci   y=vcn
    UT3D_vc_perp2vc (vco, vci, vcn);

  } else                {   // 4   bottom
    // make z from x=vci, y=vcn then invert
    UT3D_vc_perp2vc (vco, vci, vcn);
    UT3D_vc_invert (vco, vco);

  }

    // DEB_dump_obj__ (Typ_VC, vco, "ex APT_decode_vc_mod ");

  return 0;

}


//=========================================================================
  int APT_solv_ln_parl_mod (Line *lno, int imod, double dist, long dbiLn) {
//=========================================================================
// APT_solv_ln_parl_mod     parallel line from DB-line, mod, offset
// Input:
//   imod     1, 2 lateral left / right; 3, 4 top / bottom
//   dist     distance 
//   dbiLn    DB-index of line

    int      i1, i2;
    Vector   vci, vcn, vco, *vz;
    void     *vp1;


    // get data
    i1 = UTO__dbo (&vp1, &i2, Typ_LN, dbiLn);
      // DEB_dump_obj__ (i1, vp1, " o ");

    UT3D_vc_ln (&vci, (Line*)vp1);
      // DEB_dump_obj__ (Typ_VC, &vci, " vci ");

    vz = &WC_sur_act.vz;

    // get normal-vector vcn from vci and WC_sur_act
    UT3D_vc_perp2vc (&vcn, vz, &vci);
      // DEB_dump_obj__ (Typ_VC, &vcn, " vcn ");
      // GR_tDyn_vc__ (&vcn, &((Line*)vp1)->p1, 8, 1);

    // test if vcn == 0 (vci == vcz). Yes: set vcn = WC_sur_act.vy
    if(UT3D_compvc0(&vcn, UT_TOL_min0)) vcn = WC_sur_act.vy;

    // make line-offset-vector vco from mod and normal-vector vcn
    APT_solv_vc_mod (&vco, &vci, &vcn, imod);
    UT3D_vc_setLength (&vco, &vco, dist);
      // DEB_dump_obj__ (Typ_VC, &vco, " vco-len-1: ");

    // translate line
    UT3D_ln_tra_vc (lno, (Line*)vp1, &vco);
      // GR_Disp_ln (lno,  9);

    return 0;

}


/* ----------- eof -------------- */
