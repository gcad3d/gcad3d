/* ../inf/infotext.c                 RF                     2016-12-29
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
  vi -t INF

./do inf            ReCreate all tag-files
  (does ctags -f ../tags/<fnam>.tag ../inf/<fnam>.c)

HOWTO ADD new tags:
- tag must start with INF_
- tag may NOT have  a '-' - character

HOWTO ADD new tagfile:
- add in ../inf/<newTagfileName>.c



-------------------------------------------
TODO:
- make script to get html from inf (and tags)
- translate ../inf/Objects-Format.c into english ..

Old doku:
../../doc/gcad_doxygen/ *


-------------------------------------------
\file  ../inf/infotext.c
\brief Dokumentation with tags


================================================================== */
void INF(){                   /*! \code
                    development-dokumentation gcad3d
-----------------------------------------------------------------------------------
INF_CORE__          messagewindow import-export plugin app-script remote-control
                    process-control
INF_GUI__           user-interaction, menus,     ckitgui ..
INF_CMD__           command-interpreter (resolving gcad-format)
INF_DB__            data-base for (binary) grafic objects
INF_DL__            DisplayList-functions                             ../gr/ut_DL.c
INF_GRAFIC__        display-module, Displist, OpenGL, GL2D
INF_MAN__           MAN
INF_CAD__           CAD

INF_DEV_GC__        workflow events select timer
INF_APP__           create modify undo resolve attributes Userinteractions coords
INF_OBJ_FORM__      SRC ato obj DBO dlo
INF_OBJ_CONV__      ObjectFormatConversions
INF_DEV_C__         MEM const tol C_types INF_debug
INF_files__         ../inf/files.c

Geom.modules:
INF_PRCV            polygonal_representation_of_curve              ../ut/ut_prcv__.c
INF_CNTF            find and create contour (CCV = trimmed-curves) ../ut/ut_cntf.c
INF_EDMPT           move points                                    ../xa/edmpt.c
INF_Search          Search/Name

INF_GCAD_format__   Gcad-Format-native Gcad-Format-binary
INF_FMTB__          Gcad-Format-binary

INF_DEV_tools__     development-tools
INF_DOC_U           user-documentation
INF_NamingConventions
recipes-for-geometric-problems  ../../doc/geom/



================================================================== \endcode */}
void INF_GCAD_format__ (){        /*! \code

File-Format:
  ../../doc/html/file_format_en.htm              structure gcad modelfile ..

Gcad-Format-native:                              Source-format (text)
  ../../doc/html/CAD_Base_en.htm                 descr. all geom. objects
    ../../doc/html/CAD_Base_en.htm#P             Point
    ../../doc/html/CAD_Base_en.htm#L             Line
    ../../doc/html/format_gcad_A_en.htm          Surfaces
    ../../doc/html/Appli_en.htm#Program_codes    Program_codes


Gcad-Format-binary:
  INF_FMTB__    ../inf/Objects-Format.c           Gcad-Format-binary




================================================================== \endcode */}
void INF_CMD__ (){        /*! \code
INF_CMD__           command-interpreter (resolving gcad-format)

Files:
../ci/NC_Main.c


Functions:
WC_Work__
  WC_Work1
    APT_work_def            work DefinitionLine
      APT_store_obj         decode APT-object, store object.
        APT_decode_pt       decode point
        APT_decode_var
        APT_decode_ln
        APT_decode_ci       APT_DrawCirc
        APT_decode_ell
        APT_decode_pol
        APT_decode_bsp
        APT_decode_ccv__
          AP_cont_nxt       Kontursucher ..
        APT_decode_pln1     Plane/Refsys
        APT_decode_pln2     Plane   R = PERP ..
        APT_decode_sbsp     decode b-spline-surf
        APT_decode_sur
        APT_decode_srv
          APT_decode_s_sph
          APT_decode_s_cyl
          APT_decode_s_con
        APT_decode_sol
          APT_decode_bsph   Sphere
          APT_decode_bcon   CONUS
          APT_decode_btor   TORUS
          APT_decode_btor   PRISM
        ..
        DB_StoreCurv
      APT_Draw__
        APT_DrawCurv
    APT_work_PrgCmd         Programming-Function (goto, call, WORK, if ...)
    APT_do_auxCmd           G20;R20 ..


../../doc/gcad_doxygen/ProgramFlow.dox


================================================================== \endcode */}
void INF_DB__ (){        /*! \code
INF_DB__            data-base for (binary) grafic objects

Files:
../db/ut_DB.c        DB          store/retrieve binary-obj in DB



================================================================== \endcode */}
void INF_GRAFIC__ (){        /*! \code
INF_GRAFIC__        display-module, Displist and OpenGL-binding

see also
INF_workflow_events         main-events

GR               create graphic objects;                           ../gr/ut_gr.c
INF_GL__         OpenGL                                            ../gr/ut_GL.c



================================================================== \endcode */}
void INF_MAN__ (){        /*! \code

Functions:
ED_GR_CB1__         callback from grafic-window (mouse-key press)
ED_GR_CB2__         callback from grafic-window (selection)
EDI_CB__            editor-callback of GUI_edi__ (mouse/key into editor)



Files:
../xa/xa_edi__.c             Neutral EditorFunctions (not Gtk-specific).
../xa/xa_ed.c                Neutral EditorFunctions (not Gtk-specific).
../xa/xa_ed_mem.c            ApplicationSpecific Textfunctions in Memory
../xa/xa_src.c               check, modify, convert source-objects (text)  (SRC_)


================================================================== \endcode */}
void INF_CAD__ (){        /*! \code
vi -t CAD

IE_sel_CB__    grafic reports selection => CAD

Files:
../xa/xa_cad_ui.c



================================================================== \endcode */}
void INF_DEV_GC__ (){        /*! \code
INF_DEV_GC__    workflow events select subModels

INF_workflow__              sequence functions  main-startup  CAD
INF_workflow_events         main-events
INF_workflow_select         select-process
INF_subModels
INF_timer__


================================================================== \endcode */}
void INF_NamingConventions (){        /*! \code

gCAD3D naming conventions

------------------------------------------------------------------------------
FunktionsNamesgebung:

 A functionname can have this parts:
 <FNC-Group>_<outputObjs>_<Functyp>_<inputObjects>_<extraInfos>

FNC-Groups    UT2D
              UT3D
              UTI UTP
              UTX MEM ..
              UME ..
outputObjs    pt ln ...
Functyp       int proj ..
inputObjects  pt 2pt  ..



<outputObjs> / <inputObjs> ------------------------------------
  object types:

 pt    Point/Typ_PT (pt3), Point2/Typ_PT (pt2);
 npt   table of points = PointArray;
 inpt  indexed polygon (index table to table of points)
 vc    Vector, Typ_VC
 vcn   Vector with length=1 (normalized)
 ln    Line, Typ_LN
 nln   array of Lines, Typ_LN
 plg   CurvPoly/Typ_CVPOL, CurvPol2/Typ_CVPOL2, Polygon (3D with parameterTable)
 ci    Circ; Typ_CI, ac,  trimmed-circle
 ell   CurvElli, Typ_CVELL, Ellipse
 pln   Plane, Typ_PLN; Ebene.
 sph   Sphere, Typ_SPH, Kugel.
 mdr   ReferenceModel Ditto
 mdb   BasicModel

 cv crv  curve, linear object; LN,CI,EL,CV
 cvbez   CurvBez, Typ_CVBEZ, Bezier-curve.
 cvclt   CurvClot, Typ_CVCLOT, clothoid-curve.
 cvbsp  CurvBSpl, Typ_CVBSP, B-Spline-Curve; was bspl
 cvcon  Conic-Curve; hyperbola|parabola.
 cvpnm  pspl  Typ_CVPSP3, Polynom.Spline.
 cvcnt  contour; bnd  boundary; curve, closed.
 cvtrm  trimmed (limited) curve Typ_CVTRM
 ncv    Array of curves;  was cva

 sur   Surface (all types)
 stps  trimmedPerforateSurface, Typ_SURTPS
 sru   RuledSurface, Typ_SURRU
 srv   RevolvedSurface, Typ_SURRV
 sbsp  B-SplineSurface, SurBSpl, Typ_SURBSP
 srbsp Rat.B-SplineSurface, SurRBSpl, Typ_SURRBSP
 sswp  Sweep Surf, SurSwp, Typ_SURSWP
 spu   Surf-planar-unperforated;  spp Surf-planar-perforated.
 sus   SurStd, StandardSurface.
 Tes   tesselated surface (openGlSurface)
 rsys  struct Refsys = plane, Mat_4x3, inverted mat, backplane;  Typ_Refsys

 bbox  boundingBox
 tria  Triangle (3 points (pointers))
 rbox  rectangleBox; Viereck)
 edg   Edge (Kante)
 fac   Fac3, Typ_Fac3: Face; indexed-triangle; 3 int's (pointIndices)
 -"-   FacNf  Face with its NeigbourFaces
 msh   Typ_SURMSH, Mesh
 patch Opengl-patch; type, points (GL_TRIANGLE_STRIP|GL_TRIANGLE_FAN.. ))
 ipatch indexed-Opengl-patch; type, indexTable, points.

 ox   Complex-Object                     ObjGX
 otb  struct ObjTab - table of binaray objects
 crv  linear object; LN,CI,EL,CV ..      typ+struct
 sur  planimetric obj; plane, surf       typ+struct
 obj,ost  object-structure; any object   typ+struct
 dbo  DB-object; any obj                 typ+dbInd
 ccv  Concatenated-Curve (ObjGX)
 ato  atomic-object; any obj             austyp/austab (int/double)
 src  Source-object (text)
 oid  ObjectID                           string       (of DB-object eg "P20")
 onam object-name                        string (eg: onam="height" for oid=V20)

Line = linesegment (2 points, both sides limited (lnLL)
Ray  = 1 point and 1 vector; limited on one side (lnLU).
       both sides unlimited  (lnUU) ?

 side     side (before behind above below), int
 sidPerp  side  before or behind perpendic. Line/Plane
 par    parameter, native (eg knotValue) or normalized (0-1)
 par1   parameter, normalized (0-1)
 parvc  parameter of point on vector
 parln  parameter of point on line
 parci  parameter of point on circ
 parbsp (parameter of point on B-SplineCurve)
 parsbsp (parameter of point on B-SplineSurface)
 len   (length)       slen (signed length)
 sr    sense of rotation
 ar    Area
 angr  Angle-Radians
 angd  Angle-Degrees
 rd    Radius
 bp    BackPlane; XY, XZ, ZY
 hbp   Hoehenwert einer BackPlane
 gcp   gravity-center-point (Schwerpunkt)
 lva   length-value-table
 pva   parameter-value-table

<propertyCodes> ------------------------------------
 clo     closed (curve, surface)
 cyc     cyclic, passing endpoint / startpoint, not ending at endpoint
 rev     reverse, not forward
 perp    Orthogonal (Normal)
 parl    parallel
 on      auf
 in      zwischen between
 mid
 near
 equ     identisch
 norm    normieren
 div     teilen (divide)
 orient  ausrichten
 appr    approximate, not precise (Abschaetzung) 


<functionCodes> ------------------------------------
  __      "encode from"
  get   
  set
  add
  ck      check
  cpy     copy
  sub     subtract
  mult    multiply
  ev      evaluate (parameter)
  perp    perpendicular = senkrecht/lotrecht/normal   nor
  int     intersect
  ipl     interpolate
  lim     limits (endpoints), limited
  unl(im) unlimited
  dec     decode
  find    find
  prj     Projektion proj
  inv     invertieren (umdrehen)
  cnvt    convert
  mir     mirror gespiegelt
  rot     drehen, verdreht
  tr tra  Translation
  trk     Translation kartesisch (x, y);
  trv     Translation Vectoriell (vector, length);
  trp     Translation polar (angle, length);
  tri     Triangle, Dreiecksberechnung
  tng     Tangente, tangential
  std     Standard (characteristic-points,)
  swap

  Funktionen:
        Keine Funktionsangabe bei Umwandlungen
        get, set, init ..
        ins, del       (insert)
        ck      = allg. Testfunktionen
        fix     = allgem. Korrekturen
        len     = Laengenberechnung,   lenq = SquareLen (len*len)
                  slen=signed len, nlen =lenght of a normal ..

---------------------------------------------------------------------------
 mem   memory-space, struct Memspc
 str   (string, terminiert mit \0)
 strLF (string, terminiert mit LF !)
 ind   index (int, long)
 db    = doublezahl            ndb     = table of doubles
 i     int4;                   ni      = table of int4
 m2    3x2-Matrix;
 m3    4x3-Matrix;
 m4    4x4-Matrix;

---------------------------------------------------------------------------
  Testfunktionen:
    XXX_test_xxx
    werden ex Src ausgeblendet mit
    #ifdef globTag
       int XXX_test_xxx ()  {}
    #endif

---------------------------------------------------------------------------
Returncodes:
 0 (meistens !) OK
>0 OK mit Zusatzinfo
<0 Error mit Zusatzinfo




---------------------------------------------------------------------------
VariablenNamesgebung:
1.char: typ i=int d=double t=text
            p=PT v=VC c=CI l=LN s=Curv o=ObjGX m=Memspc 
2.char: Datentypinfo; kann beim DefaultTyp fehlen.
        DefaultTyp fuer int=i4, double=d8, char=s1
        p=pointer d=data <ziffer>=size f=function
3.-  Beschreibung; der erste Character ist Uppercase !
Beispiel:
 int    i1, *ip1,  iNr;   short  i2Typ (Int-2Byte)
 double d1, *dp1,  dSum;
 char   t1, *sp1,  sdBuf1[256]



---------------------------------------------------------------------------

================================================================== \endcode */}
void INF_DEV_C__ (){        /*! \code
INF_DEV_C__     MEM const tol C_types INF_debug

INF_MEM__       get memSpc ..
INF_const__     constants
INF_tol__       tolerances
INF_C_types     [U]INT_[8|16|32]                            ../ut/ut_types.h

INF_debug       errormessages     ../../doc/gcad_doxygen/Debugging.dox
INF_MSG_new     create new message




================================================================== \endcode */}
void INF_APP__ (){        /*! \code
INF_APP__           create modify resolve, attributes, Userinteractions, coords


INF_GEOM_OBJ_CR   create modify geom.objects
INF_UNDO__        undo creation of geom-obj
INF_GEOM_OBJ_RES  resolve geom.objects
INF_GEOM_ATTRIB   attributes for geom.objects (linetyp, color ..) INF_COL_CV
INF_INTER_USER    Userinteractions
INF_COORD_CONV__  coordinates 3D -> 2D -> 3D;  see also INF_ConstructionPlane
INF_ConstructionPlane   activated plane = 2D
INF_subModels




Tests:
../mytmp/


Old devDoc: ../../doc/gcad_doxygen/Programminginfos.dox
            ../../doc/gcad_doxygen/Tables-Format.dox


================================================================== \endcode */}
void INF_OBJ_FORM__ (){        /*! \code
INF_OBJ_FORM__       SRC ato obj DBO dlo

INF_SRC__       source (asciiText with objectIDs and functions ..) _src_
INF_ato__       atomic-objects
INF_obj__       binary-obj, from typ + binary-struct
INF_DBO__       DB-type & DB-index;               (int, long)
INF_dlo__       dispList-object



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
AP_typDB_typ        get DB-typ from struct-typ
UTO_copy_stru       copy struct (no resolve - not recursive)
UTO_ck_surfTyp      returns surfSubTyp (see also SUR_ck_typ)

INF_binaryObjects   get binObj from other objects





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
                    GText     Typ_GTXT
                    AText     Typ_ATXT, Typ_Tag
....................................................................................
       UTcol_       ColRGB    Typ_Color     color
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
AP_typDB_typ             get DB-typ from typ
DEB_dump_obj__           dump binary obj


================================================================== \endcode */}
void INF_obj_dat (){        /*! \code
INF_obj_dat     get binary data of binary-obj from typ + binary-struct

Functions:
DB_GetObjDat    get binary data of binary-obj from typ + binary-struct
UTO_objDat_dbS  get standardCurve from DB-curve S



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
see ../../doc/html/Search_en.htm - "has infotext"

APED_lNr_objNam            get sourcelineNr from ObjName
APED_onam_search           search objName between ps and pe
APED_onam_cut              cut/copy objName


================================================================== \endcode */}
void INF_dlo__ (){        /*! \code
INF_dlo__       dispList-object

Functions:
DL_dli__dbo                get dispListIndex of (last) DB-obj from typ/dbi

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
v0  is the parameter of the startpoint of a curve.
    v0 gives the startpoint even if dir=bwd.
v1  is the parameter of the endpoint of a curve.
    if curve is reversed (backwards) then v0 should be > v1; 
      exception: curve is closed (clo=1) and goes tru the startpoint.
    
//   vTyp     0  get knotvalues for splines,polygon; else values 0-1
//            1  get parametervalues from 0 -1 also for splines. See INF_struct_par.


In the following structs v0-v1 are knotvalues:
  CurvBSpl
  CurvRBSpl
  CurvCCV
  CurvPoly (len-offset)
- all other structs have parameters in the range 0.0 - 1.0.
- binary structs have vTyp=0

Parameters in sourceobjects are always in range 0.0 - 1.0.
- sourceCodes have vTyp=1
  Examples:
  - select the midpoint of a spline - returns eg P(S20 0.5)
  - Code "P20=P(S20 0.5)"   gives the midpoint of curve S20; also for splines.
            




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

TODO
UNUSEDi keyWords:
MMsePcp
MMsePso
MMseLoO


Tools for manipulating Messagefiles:

Howto find keyWd or messageText from code: grep 'text' ../../doc/msg/msg_en.txt

Howto create new message:
  add new message:     ./lang_ins.csh keyWd_before 'newKeywd=new line words ..'

  find a keyword:      ./lang_find.csh keyWd 
  modify keyword       /mnt/serv2/Linux/bin/changeall old new files
  modify line          ./lang_line keyWd 'line words ..'
  modify lineText      ./lang_mod.csh keyWd 'new text without keyWd'
  delete Line          ./lang_del.csh keyWd
  insert Line          ./lang_ins.csh keyWd_before 'new line words ..'
  save the lang.files: ./lang_save.csh
  restore              ./lang_rest.csh

see primary language-file:
  vi ../../doc/msg/msg_en.txt
  vi ../../doc/msg/msg_de.txt

kompare ../../doc/msg/msg_en.txt /usr/share/gcad3d/doc/msg/msg_en.txt


================================================================== \endcode */}
void INF_const__ (){        /*! \code
constants

../ut/ut_geo_const.h    Basic constants geometry (UT3D_PT_NUL, UT3D_VECTOR_X, ..
../ut/ut_geo.h          RAD_360 .. CW CCW

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

INF_MEM_ORG_TYP   get organized memspc as -
                  Fixed-Length-Records|Variable-Length-Records|Textstrings|BitArray

INF_MEM_TYP       type of memspc; stack|malloc, protected,expandable

INF_MEM_SPC       get memspc - temporary | static | permanent


Functions:
MEM_..            Swap-Invert-Delete copy compare write read ..    ../ut/ut_mem.c




../myUnused/gcad_doxygen/Tools-MemoryFunctions.dox


================================================================== \endcode */}
void INF_MEM_TYP (){        /*! \code

see ../ut/ut_types.h MEMTYP_NONE MEMTYP_ALLOC__ ..

- used in CurvPrcv, MemTab (../ut/ut_memTab.h)
- used in Memspc (UME_.. ../ut/ut_umem.h)


================================================================== \endcode */}
void INF_MEM_SPC (){        /*! \code
  get memSpc  -  temporary | permanent | static


----------------------------------------------------------
MEM_alloc_tmp     get temporary-memspc
  calls alloca; memspace exists until active function returns.
  max size should be SPC_MAX_STK = 32767 bytes



----------------------------------------------------------
-                 get permanent-memSpace         
malloc () .. free


----------------------------------------------------------
-                 get static-memSpace                ../xa/xa_mem.h
DO NOT USE this memSpaces - cannot be locked 





================================================================== \endcode */}
void INF_MEM_ORG_TYP (){        /*! \code

the memory-space types are defined in ../ut/ut_types.h as MEMTYP_*


INF_MEM_ORG_LFIX     Fixed-Length-Records         MemTab_       ../ut/ut_memTab.c
INF_MEM_ORG_LVAR     Variable-Length-Records      UME_          ../ut/ut_umem.c
INF_MEM_ORG_TXT      Textstrings                  UtxTab_       ../ut/ut_txTab.c
INF_MEM_ORG_BIT      Bit-arrays                   BitTab_       ../ut/ut_BitTab.h


================================================================== \endcode */}
void INF_MEM_ORG_LFIX (){        /*! \code

MemTab            Fixed-Length-Records         MemTab_       ../ut/ut_memTab.c
                                                             ../ut/ut_memTab.h

-----------------------------------------------------------------------
# get temporary-memspc:
  // for known nr of records:
  MemTab(int) mtbi1 = _MEMTAB_NUL;
  MemTab_ini_temp (&mtbi1, rNr);   // does malloc if > SPC_MAX_STK

  // max nr of records 
  MemTab(Point) mtpa = _MEMTAB_NUL;
  MemTab_ini_fixed (&mtpa, MEM_alloc_tmp (SPC_MAX_STK), SPC_MAX_STK,
                  sizeof(Point), Typ_PT);
  ..
  MemTab_free ((MemTab*)&mtbi1);  // if realloc permanent-memspc (gt SPC_MAX_STK)


  Point pta[200];
  MemTab_ini_fixed (&mtpa, pta, sizeof(pta), sizeof(Point), Typ_PT);
  ..




-----------------------------------------------------------------------
# get permanent-memspc:
  MemTab(int) mtbi1 = _MEMTAB_NUL;
  MemTab_ini__ (&mtbi1, sizeof(int), Typ_Int4, 10000);
  .
  MemTab_free (&mtbi1);


-----------------------------------------------------------------------
# get static-memspc:                (../xa/xa_mem.h)
DO NOT USE the static-memspces ../xa/xa_mem.h
  - static-memspces cannot be locked - have no flag inUse or unused
  - use temporary-memspc or permanent-memspc:



================================================================== \endcode */}
void INF_MEM_ORG_LVAR (){        /*! \code

Memspc            Variable-Length-Records      UME_          ../ut/ut_umem.c

-----------------------------------------------------------------------
# get temporary memspc:
  char      tmpspc[100000];
  Memspc    tmpSeg;
  UME_init (&tmpSeg, tmpspc, sizeof(tmpspc));


-----------------------------------------------------------------------
# get static memspc:
  UME_init (&s_mSpc, memspc501, sizeof(memspc501));


-----------------------------------------------------------------------
# get permanent memspc:
  irc = UME_malloc (&impSpc, spcSiz, incSiz);



================================================================== \endcode */}
void INF_MEM_ORG_TXT (){        /*! \code

TxtTab            Textstrings                  UtxTab_       ../ut/ut_txTab.c



================================================================== \endcode */}
void INF_MEM_ORG_BIT (){        /*! \code

BitTab            Bit-arrays                   BitTab_       ../ut/ut_BitTab.h





================================================================== \endcode */}
void INF_workflow__ (){        /*! \code
 sequence functions

INF_workflow_events         main-events
INF_workflow_select         select-process
INF_workflow_models         load model in temp.dir at startup


------ main-startup:
main                      ../xa/xa_main.c
  UI_win_main             create main-window
    UI_GL_draw__            callback OpenGL-startup
      AP_init__                 startup (UI_GL_draw__
        AP_work__                 work startparameters

ED_work_END
  ED_work_CurSet
    WC_Work__


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
UI_GL_draw__          Redraw whole scene
UI_GL_move__          mouseMoveCallback from Grafic-window
UI_GL_mouse__         mouseButtonCallback from Grafic-window INF_workflow_select
UI_GL_keys__          KeybCallback from Grafic-window
Brw_CB_sel            MouseButtonCallback from Browser-window
EDI_CB__              KeybCallback from Editor-window

OMN_CB_popup          callback of popup-menu

// handle special-keycodes:
  UI_key_spcCtrl    // Ctrl on
  UI_key_spcAlt     // Alt ON
  UI_key_spcShAlt   // Shift + Alt ON

UI_CursorWait UI_ChangeCursor UI_ResetCursor



================================================================== \endcode */}
void INF_workflow_select (){        /*! \code
  select-process

../xa/xa_sele.h            definition of selection-groups

UI_GL_mouse__                 OpenGL-callback for user-selection
  UI_GR_Select1               get selectionMenu (sel.Obj + useful subObjs)
    GL_sel_sel
    sele_src_cnvt__           add all useful components of selected obj
    UI_GR_Select2             callback von GUI_Popup (Liste der sel.Obj)
      UI_GR_Select_work1       work GR_Sel_Filter 3-7
        //sele_decode             change selected_object into requested_object
      UI_GR_Select_work2      work  hide,view,SM or call
        IE_sel_CB__            write selection -> CAD-inputfield
       IE_cad_sel1            CAD-report sel.
       ED_add_Text            MAN
    UI_GR_Select3             hilite obj and/or unhilite last displayed object
  UI_GR_view_set_Cen__
  UI_GR_Indicate

UI_GR_CB_Sel2


-------------------------------------------------------------------------
UI_GR_Select1         // get selectionMenu (sel.Obj + useful subObjs)
  sele_src_cnvt__         // sourceCode of all useful components of selected obj
    SRC_src_pt_dbo         // create src from point and DB-obj (curve, surface)
      ATO_ato_obj_pt         // get all atomicObjs for selection
        GR_pt_par_sel_npt      // get selectionpoint and parameter on polygon
      SRC_src_ato            // sourceObj from ato


-------------------------------------------------------------------------
Functions doing select-process:
  sele_set_add            add obj to selectionfilter reqObjTab; eg Typ_go_PD = P+D
  sele_src_cnvt__                    test if component of obj is useful
  IE_cad_Inp2_Info
  IE_inpTxtOut
  IE_txt2par1

UNUSED:
  sele_decode     converts the selected obj into a requested obj




================================================================== \endcode */}
INF_workflow_models (){        /*! \code

load model in temp.dir at startup

Functions:
  Mod_sav_(-1)          copy empty model -> tmp/Model
  Mod_sav_i (0);        save Model+Submodels into tempDir as "Model" nativ
  Mod_sav_ck (0);       make a copy of Model for ck-modified


Files in <tempDir>:
  Mod.lst               List of all internal subModels + primary model "-main-"
  Model                 the complete model after prg.exit
  Model_                primary model, native
  Model_<subModelname>  subModel, native, eg Model_L1
  Mod_in                a copy of Model for check if modified

  MdlLst.txt            list of models last used

  Exp_<subModelname>    subModel im zuletzt exportierten Format
  M#A#.msh              binary; Mesh-Faces & Edgelines.
  M#A#.ptab             binary; Mesh-points.
  <subModelname>.tess   externes Mockup-subModel (zB wrl, stl)

  Catalog.lst     eine Liste aller existierenden CatalogFiles
  catParts.act    der Filename des momentan aktiven CatalogFile
  catParts.lst    die PartList des momentan aktiven CatalogFile






================================================================== \endcode */}
INF_subModels (){        /*! \code

see INF_ModelReferenceList
see INF_BasicModelList


mainModel          is the root-model using all subModels
active-model       is the open subModel or mainModel being modified

// While loading subModels AP_modact_ind is the index of the basicModel of the
//   subModel being loaded.
//  -1       primary Model is active (can also be a submodel);
//  (>= 0)   subModel is active
// After all subModels have been loaded AP_modact_ind = -1.
// Diplaylist (DL_Att*) GR_ObjTab[].modInd gives the basicModel-index.


MDL_IS_SUB
// test if active model is a subModel being created or the active-model
//   active-model can be a subModel; see MDL_IS_MAIN

MDL_IS_MAIN
// test if active model is the mainModel




Variables:
char AP_modact_nam[128];   // name of the active submodel; def="" (main)
int      AP_modact_ind           the index into subModelList
  // get it with AP_get_modact_ind()
  //  -1: primary-Model is active. The primaryModel can be subModel.
  // >=0: subModel is being created.
  mdb_dyn             List of basicModels           see INF_BasicModelList
  mdr_tab             List of modelReferences       see INF_ModelReferenceList
  DL_Att.modInd       -1=mainmodel, else basicModelNr (index mdb_dyn)
  WC_modnam           Modelname
  WC_modact_nam       name of the active submodel
  DL_Att.modInd       -1=mainmodel, else basicModelNr (index mdb_dyn)



Functions:
  AP_get_modact_ind                         get SM-index
  APT_get_line_act                          get lineNr in mainModel
  ED_get_lnr_SM                             get lineNr in subModel
  DB_mdlNam_iBas(AP_modact_ind)             get the subModelname from  SM-index
  dla = DL_GetAtt(DL Index);  // get DL-record
  model_nr = dla.modInd;      // -1=active Model, 0-n=Submodel

  ModelBas * mdr;
  // the BasicModelRecord of a Submodels get
  mdr = DB_get_ModBas (dla.modInd);
  // mdr >mnam is now the Modelname of the Submodel.

  Mod_ck_isMain       check if the active model is the mainmodel
  DB_get_ModRef       get Ditto from Index
  DB_get_ModBas       get the basicModel with index <Ind>


Files in <tempDir>:
- see INF_workflow_models



================================================================== \endcode */}
INF_COORD_CONV__ (){        /*! \code
coordinate-transformations;

screenCoords     int, pixels, 2D.
userCoords       double; relativ to the active constrPln
worldCoords      double, absolut usercoords. DB keeps worldCoords.

worldCoords -> userCoords
worldCoords-3D -> worldCoords-2D            see also INF_ConstructionPlane


Functions:

//----------------------------------------------------------------
// worldCoords -> userCoords
UT3D_pt_tra_wcs2ucs_pl   point-worldCoords -> userCoords from plane
UT3D_pt_tra_ucs2wcs_pl   point-userCoords from plane -> worldCoords

UTRA_pt_ucs2wcs     point from constructionplane (relativ) to absolut (UCS -> WCS)

UTRA_UCS_WCS_VC                      transfer vector from WCS into UCS
UTRA_UCS_WCS_PT                      transfer point from WCS into UCS
UTRA_pt_wcs2ucs     point from absolut to relativ (constructionplane) (WCS -> UCS)


//----------------------------------------------------------------
// worldCoords-3D -> worldCoords-2D
UT3D_rsys_pl              get refsys / backplane
UTO2_obj_tra_obj3_rsys    3D->2D
  UT2D_ln_tra_ln3_rsys

UTO_obj_tra_obj2_rsys     2D->3D
  UT3D_ln_tra_ln2_rsys 


UT3D_m3_loadpl
UT3D_m3_invm3
UT3D_pt_tra_pt_m3        3D->2D or 2D->3
UT3D_ln_ln2
UT2D_ln_ln3__

-----------------
UTRA_def__
UTRA_app__
 




================================================================== \endcode */}
INF_ConstructionPlane (){        /*! \code

SRC-coordinates of objects on a ConstructionPlane are userCoords (relativ);
DB-coordinates of objects on a ConstructionPlane are worldCoords (absolut);
see INF_COORD_CONV__


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

Main-Window using Gtk3 or Gtk2; 
- before Gtk is up using zenity (OS_get_GUI ..)

CAD-GUI:

../gui/gui_base.c          interface-functions


../gui_gtk2/               Gtk2-sources
../gui_gtk2_MS             MS-Win-Gtk2-sources
../gui_gtk3/               Gtk3-sources



---------------------------------------------
CKITGUI:
../ckitgui                 testfunctions for GUI-interface-functions

cd ../ckitgui
. ../options.sh && make run -f hello-world.mak




================================================================== \endcode */}
void INF_timer__ (){        /*! \code

AP_tmr_init         init the remoteControl timer
AP_tmr_CB__

SRCU_tmr_CB__

GUI_timer__



================================================================== \endcode */}
void INF_DEV_tools__ (){        /*! \code

vim                 /usr/bin/vim
                    Install: cp .vim* ~/.
  ~/.vimrc          Ctrl-r (jump to tag), disp. htm, jpg ..
  ~/.vimdoc         Helpfile for vim; disp. with Ctrl-h
  ../tags/*.tag     tagfiles for vim

cscope              /usr/bin/cscope, source-file admin program
  ./ed              sh-script; starts cscope

../../doc/html/Plugin_en.htm#P_cust


../../doc/dev_doc_en.txt


================================================================== \endcode */}
void INF_debug (){        /*! \code
 \code

../doc/gcad_doxygen/Debugging.dox


-------------- Errormessages: ../xa/xa_msg.h ../xa/xa_msg.c
MSG_ERR_*         errormessage (key, additional_text)      see INF_DEB_MSG_ERR


-------------- dump objects:
DEB_dump_obj__                dump geom. element (2D or 3D) (../ut/ut_dump.c)

DEB_dump_ox_0                 dump object
DEB_dump_ox_s_                dump obj - structured display

DEB_dump_ox__
DEB_dump_nobj_1
DEB_dump_obj_1                dump stru and its pointers, do not resolve.



-------------- auxFuncs:
AP_debug__ ("func xy");        stop in debugger and display message

UI_wait_time                   wait <msTim> millisecs or stop with Esc

UI_wait_Esc                    wait for pressing the Esc-Key

ERR_raise                      exit plugin immediate



-------------- debugging-output:
// Write into file <tempDir>/debug.dat (not active if -DDEB is not ON):
DEB_prt_init (1);           // open debugfile
  printd (...);             // print into debugfile
DEB_prt_init (0);           // close debugfile


  // block of debug-messages
#ifdef DEB
    for(i1=0;i1<i1; ++i1)
      printd("tab[%d]=%d\n",i1,ia[i1]);
#endif

// check debugfile:
vi <tmpdir>/tmp/debug.dat


DEB_std_file                   redirect stdout -> file or back


--------------------------------------------
see also:  remove graf objects ..



================================================================== \endcode */}
void INF_DEB_MSG_ERR (){        /*! \code

Errormessages: ../xa/xa_msg.h ../xa/xa_msg.c

MSG_ERR__                      errormessage (key, additional_text)
MSG_ERR_out                    internal; MSG_ERR__ -> TX_Error ..

MSG_ERR_typ_                   type of errormessage; info|warning|error
MSG_ERR_txt                    messagetxt for ERRMSG_ERR_typ_INF/WNG/ERR

MSG_ERR_tab                    messages for ERRMSG__

TX_Error                       Errormessage direct
AP_err_hide_set                enable/disable errormessages


#include "../xa/xa_msg.h"              // MSG_*

return MSG_ERR__ (ERR_func_not_impl, "COFF2-sharp-edges");
return MSG_ERR__ (ERR_EOM, "COFF2-sharp-edges");
return MSG_ERR__ (ERR_TODO_I, "BAUSTELLE-1");
return MSG_ERR__ (ERR_EOM, "varnam");            // out of mem
return MSG_ERR__ (ERR_TEST, "label");            // testExit




================================================================== \endcode */}
void INF_DOC_U (){        /*! \code
INF_DOC_U           user-documentation

TODO:
only _en (../../doc/html/index_en.htm)  up-to-date;
- _de _fr _es _it not !


../../doc/html/*.htm




<STYLE TYPE="text/css">
  H1 { color: #000000; font-family:"Helvetica"; font-size:32pt; font-weight:medium }
  H3 { color: #000000; font-family:"Helvetica"; font-size:16pt; font-weight:medium }
  H4 { color: #000000; font-family:"Helvetica"; font-weight:medium }
  hr { border: none; width: 100%; height: 8; background: #ffcc99;}
</STYLE>
</HEAD>
<BODY LANG="en-US"
      style="font-family:Helvetica; font-size:12pt; font-weight:normal;">

..
<HR><!-- ============================================================ -->

..
<hr>
Send bug-reports, suggestions for improvement to
  franz.reiter@cadcam.co.at
</PRE>
</BODY>
</HTML>




================================================================== \endcode */}
// eof
