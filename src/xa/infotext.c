// infotext.c                 RF                     2016-12-29
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
Contains infotext only, no code.
Use it with "vi -t <code>"

ReCreate tag-file with:
ctags -f ../tags/infotext.tag ../xa/infotext.c


\file  ../xa/infotext.c
\brief Dokumentation with tags
\verbatim */
#ifdef globTag
/* \endverbatim */


void INF(){                   /*! \code
the following can be used in Doxygen (Programming-Helpfiles.dox)
and with the tagfiles (../tags/infotext.tag)

INF_types       list of    Typ_x    and corresponding struct
INF_COL_CV      list of colors for curves
INF_tol__       tolerances

INF_struct_ObjGX

INF_OGX_DBO
INF_OGX_SUR__
INF_OGX_SUR_TPS
INF_OGX_SUR_PLN

INF_MSG_new     create new message

\endcode */}



void INF_types (){        /*! \code

shortNam   funcGrp      struct    form          descr
-----------------------------------------------------------------------------------
                        double    Typ_VAR       numer. variable
                        Vector    Typ_VC
  pt       UT3D_        Point     Typ_PT        point
                        Line      Typ_LN
                        Circ      Typ_CI
                        CurvPoly  Typ_CVPOL     polygon
                        CurvElli  Typ_CVELL
                        CurvBez   Typ_CVBEZ
                        CurvBSpl  Typ_CVBSP
                        CurvRBSpl Typ_CVRBSP
                        CurvClot  Typ_CVCLOT    ClothoidCurve
                        CurvCCV   Typ_CVTRM

  obj      UTO_         typ+data  int+void*     binary-obj               INF_ObjGX
                        ObjBin    - undef !
  ox       OGX          ObjGX     Typ_ObjGX     complex-object
  dbo      DB_          typ+dbi   int+long      DB-object (dataBase)
                        ObjDB     Typ_ObjDB
  ato      ATO_         ObjAto    Typ_ObjAto    atomic-object
  txo      APED_txo_    ObjTXTSRC Typ_ObjTXTSRC source-object
                        ObjSRC    Typ_ObjSRC

  odl      DL_          DL_Att    - undef !     DisplayListRecord

see also ../../doc/gcad_doxygen/NamingConventions.txt





\endcode */} void INF_ObjGX (){        /*! \code
ObjGX                 complex-object         definition
OGX                   ../ut/ut_ox_base.c     functions

INF_OGX_DBO
INF_OGX_SUR__
INF_OGX_SUR_TPS
INF_OGX_SUR_PLN



\endcode */} void INF_OGX_DBO (){        /*! \code
- data-base-object (objectType and dataBaseIndex)

struct ObjGX
  typ  = geom.object; eg Typ_PT ..     see ../ut/AP_types.h
  form = Typ_Index                     see ../ut/AP_types.h
  size = 1

Functions:

../../doc/gcad_doxygen/ObjectFormatConversions.dox DB-Objects

\endcode */}



void INF_OGX_SUR__ (){        /*! \code
complexObject-surface

  ox.typ=Typ_SURSUP, .form=Typ_ObjGX, .siz=2+<Nr-innerBounds>, .data=oxTab
    oxTab[0] supporting_surface (PLN/CON/TOR/SRU/SRV/SBS/Undef)
    oxTab[1] outer-boundary (Undef: unlimited supporting_surface)
    [oxTab[2-n] inner-boundaries]

OGX = ox = struct ObjGX Typ_ObjGX
see ../../doc/gcad_doxygen/ObjectFormatConversions.dox complexObject
\endcode */}



void INF_OGX_SUR_TPS (){        /*! \code
complexObject - A surface-trimmed-perforated-supported

struct ObjGX
         typ  = Typ_SURTPS;
         form = Typ_ObjGX;
         siz  = nr of ObjGX-structs in data
         data = oxTab[] - supporting-surface, outer and inner-boundary

    oxTab[0] supporting-surface (DB-obj (INF_OGX_DBO))
             typ=Typ_CON| ..; form=Typ_Index
             typ=Typ_modUndef = planar-surface; compute plane from outer-boundary
    oxTab[1] outer-boundary (DB-obj (INF_OGX_DBO))
             typ=Typ_CI| ..; form=Typ_Index
             typ=Typ_modUndef = unlimited supporting_surface
   [oxTab[2-n] inner-boundaries(DB-objs (INF_OGX_DBO))]

FSUB     T_FSUB  Typ_SURTPS    APT_decode_s_pln


\endcode */}



void INF_OGX_SUR_PLN (){        /*! \code
complexObject-surface-planar

  ox.typ=Typ_SURSUP, .form=Typ_ObjGX, .siz=2+<Nr-innerBounds>, .data=oxTab
    oxTab[0] ox.typ=Typ_modUndef; .form=Typ_modUndef
             supporting_surface (PLN/CON/TOR/SRU/SRV/SBS)
             planar- Typ_modUndef - compute plane from outer-boundary
    oxTab[1] outer-boundary (none: unlimited supporting_surface)
             unlimited supporting_surface: typ=Typ_modUndef
    [oxTab[2-n] inner-boundaries
  
Functions:
  APT_decode_su_pln   // ox from ato
  DB_StoreSur         // save ox (surface) in DB)

see also INF_OGX_SUR__
\endcode */}





void INF_COL_CV (){        /*! \code

  attribute of line/curve.

The attribute is the index, defining color, linetyp and thickness.

Example:
  att = 9; // col hilite-red (822), full-line (0), lineThickness 4 pixels wide


Defaultvalues - from file ~/gCAD3D/cfg/ltyp.rc:

  0 000  0  1         // Normal            Typ_Att_PT
  1 116  0  1         // text blue         Typ_Att_def
  2 000  2  1         // dashed            Typ_Att_go
  3 000  1  1         // dash-dot          Typ_Att_rp
  4 755  0  1         // faces1 thick1     Typ_Att_Fac
  5 116  3  2         // faces2 thick1     Typ_Att_Fac1
  6 555  0  1         // faces3 thick1     Typ_Att_Fac2
  7 990  0  3         // yellow thick3     Typ_Att_Symb
  8 090  0  3         // green  thick3     Typ_Att_hili
  9 822  0  4         // hilite thick4     Typ_Att_hili1
 10 447  0  1         // dimmed thick1     Typ_Att_dim
 11 000  0  4         // black  thick4     Typ_Att_top1
 12 900  2  2         // red dashed thick2 Typ_Att_top2
#
# Format: indexNr colour lineTyp lineThick // comment
#
#   colour     3 digits; red, green, blue.
#              900 = red,   090 = green, 009 = blue;
#              000 = black; 999 = white, 990 = yellow ..
#   lineTyp:   0 = full-line (VollLinie);    1 = dash-dot (Strich-Punkt),
#              2 = dashed (kurz strichliert) 3 = dashed-long (lang strichliert),
#   lineThick: 1-6, thickness in pixels


Change / add attribute with DL_InitAttRec	();
Reload default-attibutes with DL_InitAttTab ().

\endcode */}


void INF_struct_ObjGX (){/*! \code

form: type of record of *data

.form=Typ_Index: .data=index into DB
  if size=1 data = long-value;
    set: PTR_LONG, OGX_SET_INDEX; get: LONG_PTR, OGX_GET_INDEX
  if size>1 data = address of long-table;

.form=Typ_Int4:  .data=int-value; (.typ=Typ_Texture,Typ_Typ)
  if size=1 data = int-value;
    set: PTR_INT, OGX_SET_INT; get: INT_PTR, OGX_GET_INT
  if size>1 data = address of int-table;

.form=Typ_Int8:  .data=long-value;
  if size=1 data = long-value;
    set: PTR_LONG; get: LONG_PTR.
  if size>1 data = address of int-table;

.form=Typ_Float4: .data=float-value;
   pointer to table-of-float-values;

.form=Typ_Float8: Typ_Val Typ_Angle Typ_Par1
  pointer to table-of-double-values;
  set: OGX_SET_Float8

.form=Typ_Color: .data=ColRGB
  set: OGX_SET_COLOR; get: OGX_GET_COLOR

See ../../doc/gcad_doxygen/Objects-Format.dox ComplexObject
\endcode */}


void INF_struct_dir (){/*! \code
B-spline:
  dir  0=forward, curve along ascending parameters;
       1=backward, reverse; curve along descending parameters.
Circle:
  dir  0 = same direction as parentcurve
       1 = reverse direction as parentcurve

  v0/ip0/p1 always gives the startpoint (even if dir=bwd)
  v1/ip1/p2 always gives the endpoint (even if dir=bwd)

See UTO_cv_ck_dir_clo INF_struct_par
\endcode */}


void INF_struct_par (){        /*! \code
v0  the parameter of a point on the curve. Has values 0. - 1.

  B-spline:
    The value gives the position on the fwd-curve (also for bwd-curves).
    Ascending parameters always go fwd, along the control-points,
      even for bwd-curves.
    If curve is not closed: fwd-curve: v0 < v1; bwd-curve: v1 < v0.
      v0=0.5; v1=1.0; dir=0; - curve starts at midpoint and goes fwd to the end
      v0=1.0; v1=0.5; dir=1; - curve starts at endpoint and goes bwd to midpoint
    If curve is closed:
      v0=0.5; v1=0.5; dir=0; - curve fwd from midpoint to midpoint, tru endpoint
      v0=0.5; v1=0.5; dir=1; - curve bwd from midpoint to midpoint, tru endpoint

  Circle:
    Ascending parameters can go CCW or CW (dependent on rad)
    Val. 0. is always the startpoint (p1).
    Val. 0.5 is the midpoint; dependent on CCW or CW.
    Val. 1. is always the endpoint (p2).

  Ellipse:
    Ascending parameters can go CCW or CW (dependent on srot)
    Val. 0. is always the startpoint (p1).
    v0=0.25; v1=0.5; srot=0;   // CCW-ellipse; 2.quarter of curve.
    v0=0.25; v1=0.5; srot=1;   // CW-ellipse; 3.quarter of curve.

\endcode */}


void INF_struct_closed (){        /*! \code

closed: endpoint == startpoint
  - curve can be trimmed
  - char clo: 0=closed, 1=not_closed, -1=undefined
  - see UTO_cv_ck_dir_clo

cyclic: curve is passing endpoint=startpoint   
  - curve can be trimmed
  - closed curve from startpoint to endpoint is NOT cylic
  - see UTO_cv_ck_cyc

trimmed: if curve is closed: curve passes whole cycle or not  (char trm)
  -  not-trimmed: curve from startpoint to endpoint
  -  not-trimmed: curve from midpoint to midpoint
  -  char trm: 0=trimmed, 1=not_trimmed, -1=undef;


Funcs:
  clo   UTO_cv_ck_dir_clo
\endcode */}


void INF_MSG_new (){        /*! \code
Messagefiles:
  ../../doc/msg/msg_*.txt

Tools for manipulating this-files:

Howto create new message:
  edit messagefiles (add new message):          ./lang_ed.csh

  find a keyword:      ./lang_find.csh keyWd 
  modify keyword       /mnt/serv1/Linux/bin/changeall old new files
  modify line          ./lang_line keyWd 'line words ..'
or                     ./lang_mod.csh keyWd 'new text'
  delete Line          ./lang_del.csh keyWd
  insert Line          ./lang_ins.csh keyWd_before 'new line words ..'
  save the lang.files: ./lang_save.csh
  restore              ./lang_rest.csh
\endcode */}



void INF_tol__ (){        /*! \code
tolerances
../../doc/gcad_doxygen/Constants-Tolerances.dox

../ut/ut_tol_const.h    Basic constants CAD tolerances (UT_TOL_..)
  UT_TOL_pt         identical Points-tolerence
  UT_TOL_cv         max analytical deviation for curves  

../ut/ut_geo_const.h    Basic constants geometry (UT3D_PT_NUL, UT3D_VECTOR_X, ..

../ut/ut_const.h        Basic constants math (int,double-MIN|MAX)

../ut/ut_types.h        max/min-values for signed/unsigned char/short/int

\endcode */}




/* \verbatim */
#endif
/* \endverbatim */

// eof
