/* infotext.c                 RF                     2016-12-29
 *
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

ReCreate tag-file with:     ./do inf     or
ctags -f ../tags/infotext.tag ../xa/infotext.c


\file  ../xa/infotext.c
\brief Dokumentation with tags
\verbatim */
#ifdef globTag
/* \endverbatim */



void INF(){                   /*! \code
the following can be used in Doxygen (Programming-Helpfiles.dox)
and with the tagfiles (../tags/infotext.tag)

INF_C_types     [U]INT_[8|16|32]                            ../ut/ut_types.h
INF_const__     constants
INF_tol__       tolerances
INF_MEM__       get memSpc ..

INF_SRC__       source (asciiText with objectIDs and functions ..) _src_
INF_ato__       atomic-objects
INF_obj__       binary-obj, from typ + binary-struct
INF_DBO__       DB-type & DB-index;               (int, long)
INF_dlo__       dispList-object

INF_COL_CV      list of colors for curves  (attribute curves)
INF_MSG_new     create new message

INF_workflow__  sequence functions  startup CAD
INF_ConstructionPlane
INF_subModels
INF_NamingConventions                 ../../doc/gcad_doxygen/NamingConventions.txt
INF_debug       errormessages ..      ../../doc/gcad_doxygen/Debugging.dox

INF_GUI__       ckitgui ..




================================================================== \endcode */}
void INF_obj__ (){        /*! \code
INF_obj_types   object-types and corresponding struct (Point ..)

INF_obj_ID      obj-ID is eg "P123"      _oid_
INF_obj_dat     get binary data of binary-obj from typ + binary-struct
INF_obj_names   objectName is eg "#height 2 floor" 

INF_OGX_CV_CCV  concatenated-curve (CCV)
INF_OGX_DBO
INF_OGX_SUR__
INF_OGX_SUR_TPS
INF_OGX_SUR_PLN


Functions:
DEB_dump_obj__        
UTO_siz_stru        get size of structure
AP_typ_2_bastyp     get DB-typ from struct-typ
UTO_copy_stru       copy struct (no resolve - not recursive)
UTO_ck_surfTyp      returns surfSubTyp (see also SUR_ck_typ)

--------- get binaryObject from DB-Obj
 UTO_get_DB     get dataStruct from DB-Obj Keep Refs.   Give pointer.
 DB_GetObjDat   get dataStruct from DB-Obj Resolv Refs. Give pointer.

--------- get binaryObject from complexObj
 UTO_obj_getp   get dataStruct from ObjGX  Resolv Refs. Give pointer.
 UTO_obj_get    get dataStruct from ObjGX  Resolv Refs. Give a copy.

--------- get binaryObject from sourceObj
APT_obj_expr        Create struct from ModelCode (text).
APT_decode_objTx    Create struct from ModelCode (text).
UT3D_pt_txt         get point from text
UT3D_vc_txt         get vector from text
AP_vec_txt          give vector-struc from vector-text (only defvecs)
APT_obj_expr APT_obj_ato
APT_decode_pt APT_decode_ln APT_decode_ci
APT_decode_ang APT_decode_goAxis APT_decode_goRadius (../ci/NC_apt.c)


see also ../../doc/gcad_doxygen/NamingConventions.txt
empty prototypes (eg UT3D_PT_NUL) see ../ut/ut_geo_const.h




================================================================== \endcode */}
void INF_obj_types (){        /*! \code

short
name     funcGrp      struct    form          descr                    files,funcs
-----------------------------------------------------------------------------------
                    double    Typ_VAR       numer. variable
                    Vector    Typ_VC
pt     UT3D_        Point     Typ_PT        point                    INF_Typ_PT
ln                  Line      Typ_LN
                    Circ      Typ_CI
                    CurvPoly  Typ_CVPOL     polygon                  ../ut/ut_plg.c
                    CurvPol2  Typ_CVPOL2    polygon-2D               ../ut/ut_plg.c
                    CurvElli  Typ_CVELL     ellipse                  INF_Typ_CVELL
                    CurvBez   Typ_CVBEZ                              ut_bez.c
                    CurvBSpl  Typ_CVBSP                              INF_Typ_CVBSP
                    CurvRBSpl Typ_CVRBSP
                    CurvClot  Typ_CVCLOT    ClothoidCurve
       CVTRM_       CurvCCV   Typ_CVTRM     TrimmedCurve             INF_Typ_CVTRM
       UT3D_pl_*    Plane     Typ_PLN
                    Refsys    Typ_Refsys    (back)plane,tra.matrix   INF_Typ_Refsys
....................................................................................
obj    UTO_         typ+data  int+void*     binary-obj               INF_UTO__
otb    OTB_         ObjTab    - undef !     obj+xDat[]               INF_ObjTab
                    ObjBin    - undef !
ox     OGX          ObjGX     Typ_ObjGX     complex-object           INF_ObjGX
dbo    DB_          typ+dbi   int+long      DB-object (dataBase)
                    ObjDB     Typ_ObjDB
ato    ATO_         ObjAto    Typ_ObjAto    atomic-object
txo    APED_txo_    ObjTXTSRC Typ_ObjTXTSRC source-object
                    ObjSRC    Typ_ObjSRC

bbx    BBX2_        BBox2     Typ_BBox2     boundingBox-2D           INF_BBOX
odl    DL_          DL_Att    - undef !     DisplayListRecord

                    int[]                   table of ints            ut_iTab.c

sr                                          sense-of-rotation;       INF_sr

see -
INF_struct_ObjGX
INF_struct_dir
INF_struct_par
INF_struct_closed

TYP_IS_CV                check DB-typ
AP_typ_2_bastyp          give basictyp from typ
DEB_dump_obj__           dump binary obj


================================================================== \endcode */}
void INF_obj_dat (){        /*! \code
INF_obj_dat     get binary data of binary-obj from typ + binary-struct

Functions:
DB_GetObjDat    get binary data of binary-obj from typ + binary-struct



================================================================== \endcode */}
void INF_obj_ID (){        /*! \code
obj-ID is eg "P123"      _oid_

APED_oid_dbo__      make name from typ and DB-Index  (visible object types)
APED_oid_dbo_all    make name from typ and DB-index  (all types)
SRC_src_dbo         create sourceCode of requested type from Database-object
APED_oid_vc          get oid for Vector; (DX or DIX or D#)

AP_obj_add_dbo      add objname to string
AP_obj_add_pt       add Point* as "P(xyz)"
AP_obj_add_vc       add struct Vector* to string  " D(x y z)"
AP_obj_add_val      add double > text  ( xa_obj_txt.c )

AP_cre_defHdr       create new (unused) objHeader
AP_src_typ__        get typText from typ (eg "PT" for Typ_PT)
AP_src_typMod       get modifier-text from (modifier)typ
APT_decode_print    create sourceObj from atomicObjs (for PRInt-cmd)


================================================================== \endcode */}
void INF_SRC__ (){        /*! \code
source (asciiText with objectIDs and functions ..) _src_

TODO:
AP_stru_2_txt  sollte -> AP_obj_add_ox


Files:
../xa/xa_ed.c          ED_ .. Neutral EditorFunctions (not Gtk-specific)
../xa/xa_edi__.c       ED_ .. Neutral EditorFunctions (not Gtk-specific)
../xa/xa_ed_mem.c      APED_  Textfunctions in Memory
../xa/xa_src.c         SRC_   check, modify, convert source-objects (text)


Functions:
SRC_src_ato            sourceObj (text) from atomicObjs

AP_stru_2_txt       sourceObj (text) from complex-obj
SRC_src_pt3_10      write struct Point* to string "P(<x> <y> <z>)" precision 10
AP_obj_add_pt       add struct Point* to string  " P(<x> <y> [<z>])"
AP_obj_add_dbo      add DBO (typ,dbi) to string
AP_obj_add_val
AP_obj_add_vc
..
AP_obj_2_txt        change obj to text and save it with UTF_add1_line



================================================================== \endcode */}
void INF_DBO__ (){        /*! \code

APED_dbo_oid        get type and dbi from object-ID

UTO_ck_dbsTyp       check object-typ (struct (V,D,P,L,C) or object (S,A,B)) 

DBO_dbo_src__       give DB-Obj (typ and index) from sourceObj eg "L(S1 MOD(1))"
APED_dbo_oid        give DB-Obj (typ and index) from sourceObj eg "P12"

UTO_dbs_ox          DB-struct (data for VDPLC, ox for SAB) from ox
DBO_sel__           change selection into DB-object
DBO_dump__          dump DB-object -> debug-window

ATO_ato_eval__      create dynam DB-obj from atomic-obj


================================================================== \endcode */}
void INF_ato__ (){        /*! \code
../xa/xa_ato.c         atomic-objects

  atomicObjects (ausTyp/ausTab, array) _ato_:
    (list of objTyp and list of objValues)



Functions:
ATO_ato_srcLn__         get atomicObjects from sourceLine; full evaluated.
ATO_ato_txo__           get atomic-objects from source-objects
ATO_ato_eval__          evaluate atomic-objects (compute); reduce records.
ATO_ato_eval_geom       evaluate geometrical functions
APT_decode__        sourceObj -> atomicObj (ausTyp/ausTab-array)
ATO_sort1
ATO_getSpc__
ATO_getSpc1
ATO_srcTxt          get atomicObj from Typ_Txt (after AP_typ_srcExpr)
ATO_swap            swap 2 records
APT_decode_ausdr    decode sourceObj -> atomicObj
APT_decode_ausdr1   operators constantObjects constantValues

SRC_ato_SIZ, SRC_ato_typ, SRC_ato_tab = global memspc




================================================================== \endcode */}
void INF_obj_names (){        /*! \code
objectName is eg "#height 2 floor"

APED_lNr_objNam            get sourcelineNr from ObjName
APED_onam_search           search objName between ps and pe
APED_onam_cut              cut/copy objName


================================================================== \endcode */}
void INF_dlo__ (){        /*! \code
INF_dlo__       dispList-object

Functions:
DL_find_obj                get dispListIndex of (last) DB-obj from typ/dbi

Files:
../gr/ut_DL.c



================================================================== \endcode */}
void INF_Typ_PT (){        /*! \code

Functions:
UT3D_pta_*

Files:
../ut/ut_geo2d.c    UT2D
../ut/ut_geo3d.c    UT3D

../ut/ut2d_npt.c    UT2D_npt     polygon (list of points) from curves, surfaces
../ut/ut3d_npt.c    UT3D_npt     polygon (list of points) from curves, surfaces

../xa/cvoff2.c      CVOFF2       offset polygon in same plane
../ut/ut_prcv.c	    PRCV         Polygonal_representation_of_curve (struct CurvPrcv)

../ut/ut_cvApp.c	               Change polygon -> circles, lines 



================================================================== \endcode */}
void INF_Typ_CVELL (){        /*! \code


Functions:
APT_decode_ell
UT3D_el_ci

Files:
../ut/ut_elli.c
../ut/ut_elli.h


================================================================== \endcode */}
void INF_Typ_CVBSP (){        /*! \code

Functions:
APT_decode_cv APT_decode_bsp_ APT_decode_bsp1
UT3D_bsp_ox
bspl_bsp_ptn
UT3D_pta_bsp


Files:
../ut/ut_bspl.c	           B-Spline-Curve-Utilityfunctions 
../ut/ut_bsp_approx.c	     Approximation Polygon -> B-Spline 
../ut/cirMoeb.c	           Curves (bsplines, conic sections, etc.) 


================================================================== \endcode */}
void INF_sr (){        /*! \code
sense-of-rotation;   0=CCW, 1=CW.

  sr = DLIM01(rad);   // 0 if (rad >= 0.); 1 if (rad < 0.)

  sri = ICHG01(sr);   // change sr; 0 -> 1, 1 -> 0;

was: 1=CCW, -1=CW
  change to 0=CCW; 1=CW; with:   sr1 = (sr0 > 0) ? 0 : 1;



================================================================== \endcode */}
void INF_spcTyp (){        /*! \code
type of memory-space
- used in CurvPrcv, MemTab (../ut/ut_memTab.h)

MEMTYP_NONE         0  // undefined/empty;  can-reallocate,   must-NOT-free;
MEMTYP_ALLOC__      1  // malloc;           can-reallocate,   must-free;
MEMTYP_ALLOC_PROT   2  // malloc,protected: can-reallocate,   must-NOT-free;
MEMTYP_FIXED_PROT   3  // malloc,protected; CANNOT-reallocate,must-NOT-free;
MEMTYP_STACK        4  // stack;            CANNOT-reallocate,must-NOT-free;



================================================================== \endcode */}
void INF_UTO__ (){        /*! \code
../ut/ut_obj.c
obj    UTO_         typ+data  int+void*     binary-obj               INF_UTO__



================================================================== \endcode */}
void INF_ObjTab (){        /*! \code
functions for
  ObjTab =  list of [oTyp, pointer to oDat, Memspc for oDat, aux.obj]
../xa/tst_ut_objtab.c



================================================================== \endcode */}
void INF_ObjGX (){        /*! \code
ObjGX                 complex-object         definition
OGX                   ../ut/ut_ox_base.c     functions

INF_OGX_DBO
INF_OGX_SUR__
INF_OGX_SUR_TPS
INF_OGX_SUR_PLN


Functions:
OGX_SET_OBJ             complexObject from binObj (typ,form,siz,dat)
OGX_SET_INDEX           complexObject from DB-obj
DEB_dump_ox_0           dump complex-object


================================================================== \endcode */}
void INF_OGX_DBO (){        /*! \code
- data-base-object (objectType and dataBaseIndex)

struct ObjGX
  typ  = geom.object; eg Typ_PT ..     see ../ut/AP_types.h
  form = Typ_Index                     see ../ut/AP_types.h
  size = 1

Functions:

../../doc/gcad_doxygen/ObjectFormatConversions.dox DB-Objects



================================================================== \endcode */}
void INF_Typ_CVTRM (){        /*! \code

struct CurvCCV

Functions:
  UTO_cv_cvtrm       // make normal object of trimmedCurve

Files:
../ut/ut_cvtrm.c        CVTRM_*      trimmed-curve functions 
../ut/ut_cntf.c         CNTF_        create trimmed-curve from object-table
../xa/xa_cad_ccv_ui.c   IE_ccv_*     GUI simple CurveEditor



================================================================== \endcode */}
void INF_OGX_CV_CCV (){        /*! \code
only concatenated-curve (CCV) is a complexObject-curve
ObjGX     cv1
  cv1.typ   = Typ_CVTRM;
  cv1.form  = Typ_CVTRM;
  cv1.siz   = nr of subCurves
  cv1.data  = (CurvCCV*)=array of subCurves (trimmedCurves)

UTO_cv_cvtrm   // change trimmedCurve into standardCurve

see ../../doc/gcad_doxygen/Objects-Format.dox Curve-CCV



================================================================== \endcode */}
void INF_Typ_Refsys (){        /*! \code
Refsys    =   plane + backplane + transformation-matrix ..

UT3D_rsys_pl



================================================================== \endcode */}
void INF_OGX_SUR__ (){        /*! \code
complexObject-surface

  ox.typ=Typ_SURSUP, .form=Typ_ObjGX, .siz=2+<Nr-innerBounds>, .data=oxTab
    oxTab[0] supporting_surface (PLN/CON/TOR/SRU/SRV/SBS/Undef)
    oxTab[1] outer-boundary (Undef: unlimited supporting_surface)
    [oxTab[2-n] inner-boundaries]

OGX = ox = struct ObjGX Typ_ObjGX
see ../../doc/gcad_doxygen/ObjectFormatConversions.dox complexObject



================================================================== \endcode */}
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





================================================================== \endcode */}
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



================================================================== \endcode */}
void INF_COL_CV (){        /*! \code

  attribute of line/curve.

The attribute is the index, defining color, linetyp and thickness.

Example:
  att = 9; // col hilite-red (822), full-line (0), lineThickness 4 pixels wide


sourcefile-runtime: <cfgdir>/ltyp.rc     /mnt/serv1/Devel/gcad3d/gCAD3D/cfg/ltyp.rc
sourcefile-devel:   ../../gCAD3D/cfg/ltyp.rc -> examples.gz

  0 000  0  1    black  full                 Typ_Att_def        default
  1 116  0  1    blue   full blue            Typ_Att_blue
  2 000  2  1    black  dash                 Typ_Att_dash__
  3 000  1  1    black  dash-dot             Typ_Att_dash_dot
  4 755  0  1    gray   full                 Typ_Att_Fac
  5 116  3  2    blue   dash-long  thick2    Typ_Att_dash_long
  6 555  0  1    gray   full                 Typ_Att_Fac2      faces3
  7 990  0  3    yellow full       thick3    Typ_Att_Symb
  8 090  0  3    green  full       thick3    Typ_Att_hili
  9 822  0  4    red    full       thick4    Typ_Att_hili1     hilite
 10 447  0  1    gray   full                 Typ_Att_dim
 11 000  0  4    black  full       thick4    Typ_Att_top1
 12 900  2  2    red    dashed     thick2    Typ_Att_top2
#
# Format: indexNr colour lineTyp lineThick // comment
#
#   colour     3 digits; red, green, blue.
#              900 = red,   090 = green, 009 = blue;
#              000 = black; 999 = white, 990 = yellow ..
#   lineTyp:   0 = full-line (VollLinie);    1 = dash-dot (Strich-Punkt),
#              2 = dashed (kurz strichliert) 3 = dashed-long (lang strichliert),
#   lineThick: 1-6, thickness in pixels

attribute-definitions: ../ut/gr_types.h            Typ_Att_def = 0 ..

Change / add attribute with DL_InitAttRec	();
Reload default-attibutes with DL_InitAttTab ().




================================================================== \endcode */}
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


================================================================== \endcode */}
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


================================================================== \endcode */}
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



================================================================== \endcode */}
void INF_struct_closed (){        /*! \code

closed: state of the basic-curve; closed or not.
  a basic curve always has trimmed=no
  closed=yes endpoint == startpoint
  - curve can be trimmed
  - char clo: 0=closed, 1=not_closed, -1=undefined
  - see UTO_cv_ck_dir_clo

trimmed: state of a copy of the basic-curve;
  trimmed=yes: this curve is a trimmed part of the basic-curve;
  the basic curve can be closed or not 

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


================================================================== \endcode */}
void INF_BBOX (){        /*! \code
../ut/ut_box.c

Point *pb1, Point *pb2,        // LL (lower-left), UR (upper-right)

bbx    BBX2_        BBox2     Typ_BBox2     boundingBox-2D           INF_BBOX





================================================================== \endcode */}
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

see primary language-file:
  vi ../../doc/msg/msg_en.txt
  vi ../../doc/msg/msg_de.txt



================================================================== \endcode */}
void INF_const__ (){        /*! \code
constants

../ut/ut_geo_const.h    Basic constants geometry (UT3D_PT_NUL, UT3D_VECTOR_X, ..

../ut/ut_const.h        Basic constants math (int,double-MIN|MAX)

../ut/ut_types.h        max/min-values for signed/unsigned char/short/int



================================================================== \endcode */}
void INF_tol__ (){        /*! \code
tolerances
../../doc/gcad_doxygen/Constants-Tolerances.dox

../ut/ut_tol_const.h    Basic constants CAD tolerances (UT_TOL_..)
  UT_TOL_pt         identical Points-tolerence
  UT_TOL_cv         max analytical deviation for curves  



================================================================== \endcode */}
void INF_MEM__ (){        /*! \code
  get memSpc ..

MemTab            Fixed-Length-Records         MemTab_       ../ut/ut_memTab.c
                                                             ../ut/ut_memTab.h
Memspc            Variable-Length-Records      UME_          ../ut/ut_umem.c
TxtTab            Textstrings                  UtxTab_       ../ut/ut_txTab.c
MEM_alloc_tmp     temporary memspc


../xa/xa_mem.h    includefile static memSpaces


../../doc/gcad_doxygen/Tools-MemoryFunctions.dox



================================================================== \endcode */}
void INF_workflow__ (){        /*! \code
 sequence functions

------ startup:
main                      ../xa/xa_main.c
  UI_win_main             create main-window
    UI_GL_draw__            callback OpenGL-startup
      AP_init__                 startup (UI_GL_draw__
        AP_work__                 work startparameters



------ CAD:
WC_Work1                      execute codeline
  APT_work_def                handle definition-line
    APT_store_obj             store in DB
      APT_decode_pt
      APT_decode_bsp
      APT_decode_ccv__
    APT_Draw__                display
      APT_DrawCurv
  APT_work_AppCodTab          do eg HIDE VIEW MODSIZ DEFTX EXECM ..
  APT_do_auxCmd               do eg R# or G#



main-events                   INF_workflow_events
select-process                INF_workflow_select




../../doc/gcad_doxygen/ProgramFlow.dox


================================================================== \endcode */}
void INF_workflow_events (){        /*! \code
  main-events

------------------- OpenGL-Events:
UI_GL_draw__                  Redraw whole scene
UI_GL_move__                  mouseMoveCallback from Grafic-window
UI_GL_mouse__                 mouseButtonCallback from Grafic-window
UI_GL_keys__                  KeybCallback from Grafic-window
Brw_CB_sel                    MouseButtonCallback from Browser-window
EDI_CB__                      KeybCallback from Editor-window

OMN_CB_popup                  callback of popup-menu

// handle special-keycodes:
  UI_key_spcCtrl    // Ctrl on
  UI_key_spcAlt     // Alt ON
  UI_key_spcShAlt   // Shift + Alt ON

UI_CursorWait UI_ChangeCursor UI_ResetCursor



================================================================== \endcode */}
void INF_workflow_select (){        /*! \code
  select-process

UI_GL_mouse__                 OpenGL-callback for user-selection
  UI_GR_Select1
    GL_sel_sel
    UI_GR_Select2             callback von GUI_Popup (Liste der sel.Obj)
     UI_GR_Select_work1       work GR_Sel_Filter 3-7
      sele_decode             change selected_object into requested_object
      UI_GR_Select_work2      work  hide,view,SM or call
       IE_sel_CB__            write selection -> CAD-inputfield
       IE_cad_sel1            CAD-report sel.
       ED_add_Text            MAN
    UI_GR_Select3             hilite obj and/or unhilite last displayed object
  UI_GR_view_set_Z
  UI_GR_Indicate

UI_GR_CB_Sel2


================================================================== \endcode */}
INF_subModels (){        /*! \code

mainModel          is the root-model using all subModels
active-model       is the open subModel or mainModel being modified

// While loading subModels AP_modact_ind is the index of the basicModel of the
//   subModel being loaded.
// -1=primary Model is active (can also be a submodel);
//   else a subModel is being loaded.
// After all subModels have been loaded AP_modact_ind = -1.
// Diplaylist (DL_Att*) GR_ObjTab[].modInd gives the basicModel-index.


MDL_IS_SUB
// test if active model is a subModel being created or the active-model
//   active-model can be a subModel; see MDL_IS_MAIN

MDL_IS_MAIN
// test if active model is the mainModel



int      AP_modact_ind
  //  -1: primary-Model is active. The primaryModel can be subModel.
  // >=0: subModel is being created.


AP_modact_nam

Functions:
DB_mdlNam_iBas(AP_modact_ind)


================================================================== \endcode */}
INF_ConstructionPlane (){        /*! \code

screenCoords     int, pixels, 2D.
userCoords       double; relativ to the active constrPln
worldCoords      double, absolut usercoords. DB keeps worldCoords.

SRC-coordinates of objects on a ConstructionPlane are userCoords (relativ);
DB-coordinates of objects on a ConstructionPlane are worldCoords (absolut);

int       WC_sur_ind;    DB-Index of the ActiveConstrPlane;
                         (WC_sur_ind > 0)  is 2D      (AP_IS_2D)
                         (WC_sur_ind <= 0) is 3D      (AP_IS_3D)
double    WC_sur_Z;      the active Z-Offset to the ConstructionPlane
char WC_ConstPl_Z[16];   displayed name_of_Constr.Plane; is "DZ" or "R20"


Plane     WC_sur_act;    the ConstructionPlane; in xa.c
Mat_4x3   WC_sur_mat;            // TrMat of ActiveConstrPlane
Mat_4x3   WC_sur_imat;           // inverse TrMat of ActiveConstrPlane

Making constrPlane ON and OFF both create a DL-record Typ_constPln


Functions ConstrPlane:  -------------------------
DL_setRefSys
  NC_setRefsys      Change active Plane (Refsys)
    GL_SetConstrPln
DL_GetTrInd         get the refsys-nr (dbi of Plane) for DL-record <dli>
UTRA_pt_ucs2wcs     point from constructionplane (relativ) to absolut (UCS -> WCS)
UTRA_pt_wcs2ucs     point from absolut to relativ (constructionplane) (WCS -> UCS)

GL_SetConstrPln     GL_constr_pln=(WC_sur_act+WC_sur_Z); write Label Z-Offset
UI_Set_ConstPl_Z    write Label name_of_Constr.Plane
AP_Set_ConstPl_Z    write Label name_of_Constr.Plane

UI_suract_keyIn     mode=2: set & display WC_sur_Z
AP_Get_ConstPl_Z    gives ConstPLn as text or Z-vec
AP_Get_ConstPl_vz   give Z-vec of ConstructionPlane


../../doc/gcad_doxygen/Userinteractions.dox


================================================================== \endcode */}
void INF_C_types (){        /*! \code

     [U]INT_[8|16|32]
#include "../ut/ut_types.h"               // INT_8 - UINT_64


                            Linux     MS-Win
                      byte   bit-gcc   bit-MS-cl
32-bit: sizeof(int)    4       32        32
        sizeof(long)   4       32        32
        sizeof(float)  4       32        32
        sizeof(double) 8       64        64
        sizeof(void*)  4       32        32

64-bit: sizeof(int)    4       32        32
        sizeof(long)   4|8     64        32 | 64  (Compilerswitch) set to 64 !!!
        sizeof(float)  4       32        32
        sizeof(double) 8       64        64
        sizeof(void*)  8       64        64


_MSC_VER         // MS-Win; gcc & MS_VS; beim gcc von extern mit -D_MSC_VER
_MFC_VER         // MS-Win; nur mit MS-Compiler (MFC-Version).
_WIN32 & _WIN64  // MS-Win; nur mit MS-Compiler
_LP64            // vom gcc auf 64-bit-OS.





================================================================== \endcode */}
void INF_GUI__ (){        /*! \code

CAD-GUI:
../gui



CKITGUI:
/mnt/serv1/Devel/gcad3d/src/ckitgui
. ../options.sh && make -f ../ckitgui/hello-world.mak
DZT:
xa_gui.so: Nicht definierter Verweis auf `glX*`
??









================================================================== \endcode */}
void INF_debug (){        /*! \code
 \code

../doc/gcad_doxygen/Debugging.dox


-------------- Errormessages: ../xa/xa_msg.h ../xa/xa_msg.c
MSG_ERR__                      errormessage (key, additional_text)
MSG_ERR_out                    internal; MSG_ERR__ -> TX_Error ..

MSG_ERR_typ_                   type of errormessage; info|warning|error
MSG_ERR_txt                    messagetxt for ERRMSG_ERR_typ_INF/WNG/ERR

MSG_ERR_tab                    messages for ERRMSG__

TX_Error                       Errormessage direct
AP_err_hide_set                enable/disable errormessages


-------------- dump objects:
DEB_dump_obj__                dump geom. element (2D or 3D) (../ut/ut_dump.c)

DEB_dump_ox_0                 dump object
DEB_dump_ox_s_                dump obj - structured display

DEB_dump_ox__
DEB_dump_nobj_1
DEB_dump_obj_1                dump stru and its pointers, do not resolve.



-------------- auxFuncs:
AP_debug__  ("func xy");       stop in debugger and display message
UI_wait_time                   wait <msTim> millisecs or stop with Esc
UI_wait_Esc                    wait for pressing the Esc-Key
ERR_raise                      exit plugin immediate

see also:  remove graf objects ..

--------------------------------------------
// Write into file <tempDir>/debug.dat:
// (not active if -DDEB is not ON)
AP_deb_stat (1);           // open debugfile
  printd ..                // print into debugfile
AP_deb_stat (0);           // close debugfile


  // block of debug-messages
#ifdef DEB
    for(i1=0;i1<i1; ++i1)
      printd("tab[%d]=%d\n",i1,ia[i1]);
#endif

// check debugfile:
vi <tmpdir>/tmp/debug.dat


\endcode */}


/* \verbatim */
#endif
/* \endverbatim */

// eof
