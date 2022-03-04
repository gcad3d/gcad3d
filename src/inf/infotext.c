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
                    process-control Group
INF_GUI__           user-interaction, menus,     ckitgui ..
INF_CMD__           command-interpreter (resolving gcad-format)
INF_DB__            data-base for (binary) grafic objects
INF_DL__            DisplayList-functions                             ../gr/ut_DL.c
INF_GRAFIC__        display-module, Displist, OpenGL, GL2D (2D-buttons)
INF_MAN__           MAN
INF_CAD__           CAD
INF_Models__        import load save baseModel refModel                 MDLFN MDL

INF_DEV_GC__        workflow events select timer load_save models
INF_APP__           create modify undo resolve attributes Userinteractions coords
INF_OBJ_FORM__      INF_obj_types, source-obj SRC, atomic-obj ato, DBO dlo ..
INF_OBJ_CONV__      ObjectFormatConversions
INF_DEV_C__         C-software-developmant - MEM const tol C_types INF_debug

INF_geom            INF_func_3D    INF_func_2D      INF_surfaces   INEW_surf__
                    INF_Intersect_Surf INF_Intersect_Body INF_Create_Body

INF_Brw__           obj-browser
INF_PRCV            polygonal_representation_of_curve              ../ut/ut_prcv__.c
INF_CNTF            find and create contour (CCV = trimmed-curves) ../ut/ut_cntf.c
INF_EDMPT           move points                                    ../xa/edmpt.c
INF_Search          Search/Name

INF_GCAD_format__   Gcad-Format-native
INF_FMTB__          Gcad-Format-binary tess-format
INF_symDir          symbolic-directory modelfilename

INF_files__         list of all source-files ../inf/files.c
INF_DEV_tools__     development-tools
INF_DOC_U           user-documentation
INF_NamingConventions

recipes-for-geometric-problems  ../../doc/geom/
making a Video      ../../doc/html/Video_en.htm

Other modules:
INF_DBF             File-based DB; Save and retrieve key-value-Records
INF_GUI_exe         GUI-executables (messageBox, select-file, entry-with-buttons)
INF_catalog         catalog-parts, CTLG
INF_GIS1__
INF_GSRC            Geometric-Source-Relocatable-Code (add object-source from extern)
INF_tstf__          Testttool to execute commands in file

INF_TODO__          TODO_* BUG_*



================================================================== \endcode */}
void INF_GCAD_format__ (){        /*! \code


Gcad-Format-binary:
  INF_obj_types     list of binay objtypes
  INF_FMTB__        ../inf/Objects-Format.c           Gcad-Format-binary
INF_import
INF_DynamicDataArea  DYNAMIC_DATA-block



File-Format:
  ../../doc/html/file_format_en.htm              structure gcad modelfile ..

Gcad-Format-native:                              Source-format (text)
  ../../doc/html/CAD_Base_en.htm                 descr. all geom. objects
    ../../doc/html/CAD_Base_en.htm#P             Point
    ../../doc/html/CAD_Base_en.htm#L             Line
    ../../doc/html/format_gcad_A_en.htm          Surfaces
    ../../doc/html/Appli_en.htm#Program_codes    Program_codes



//----------------------------------------------------------------
- design-codes:  - native Format gcad - text-format
    P#=<coordinates>
    C#=<centerPoint> <outerPoint>   // circle
    A#=C# ..     // surface = outer boundary (C#) [inner boundaries ..]
    B#=A# A# ..  // Body = surfaces or boundary+thicknes ..

  - mockup-native-format (Text):
    A#=RCIR ..   // triangle
    TODO: obsolete "SECTION MESH" and "SECTION PTAB" (indexed-surfaces)
    - ev muss dazu auch PRCV ausgegeben werden (siehe "SECTION PTAB")
    - Texturinfos (dzt attribute)
    - see TODO_FMT_MOCK_NAT
    - Import-mockup-formats (obj, stl, vr1, vr2):                INF_import

      - create tess-format-binary
      - oder create mockup-native-format then mockup-binary
      TODO:  save mockup-formats in mockup-native-format
      - or save always as mockup-native-format and load binary ?
      TODO: "Import as model or mockup" dzt nur obj, für alle.
      - würde entfallen wenn save always mockup-native-format


- binary-format
    DB           // P L C S A ..                                  INF_DB__
    PRCV         // DB for Polygonal_representation_of_curve      INF_PRCV
  TODO: surfDB     // DB for points and meshes inside surfaces
    - descr. surface (outerBound, innerBounds, breakLines)
    - see obsolete "SECTION MESH" and "SECTION PTAB"
  TODO: bodyDB     // DB for net of a closed shell (connected curves, fwd and bwd)

  - mockup-format binary: file with tesselated data; dzt .tess
    - filetyp .tess; ev neues format .gcat
    TODO: div. Formate für indexed-surfaces, color, transp., textur





================================================================== \endcode */}
void INF_CMD__ (){        /*! \code
INF_CMD__           command-interpreter (resolving gcad-format)

Files:
../ci/NC_Main.c


Functions:
WC_Work1
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

Functions:
UTO_obj_dbo       get DB-data-struct from typ, DB-index


================================================================== \endcode */}
void INF_GRAFIC__ (){        /*! \code
INF_GRAFIC__        display-module, Displist and OpenGL-binding

INF_GR_RECORDS      Source-record Database-record DisplayList-record
                    permanent-obj           GR_perm_*
                    dynamic-obj             GR_perm_*   with negative DB-index
                    temporary-dynamic-obj   GR_tDyn_*
                    temporary-obj           GR_temp_*
                    
INF_GR_preview      temp. display of symbolic-objects ((VC,Tra,VAR,Activ)

INF_GR_attrib       attributes (color, linetype ..)

INF_GR_group        list of selected objects
INF_GR_RotCen       set rotation-center
INF_rescale__       rescale active model ("Scale All")


see also
INF_workflow_events         main-events

GR               create graphic objects;                           ../gr/ut_gr.c
INF_GL__         OpenGL INF_GL_surfaces INF_GL2D__                 ../gr/ut_GL.c



================================================================== \endcode */}
void INF_MAN__ (){        /*! \code

see INF_workflow_MAN

Functions:
ED_GR_CB1__         callback from grafic-window (mouse-key press)
ED_GR_CB2__         callback from grafic-window (selection)
EDI_CB__            editor-callback of GUI_edi__ (mouse/key into editor)

ED_unload__         copy editor -> memory



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
INF_workflow_Hide_View
INF_workflow_display        display obj; GR - GL
INF_workflow_MAN
INF_workflow_CAD__
INF_workflow_CAD_edit
INF_workflow_models         load / save model ..

INF_basModels__
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
 imr   index ReferenceModel
 mdb   BasicModel
 imb   index BasicModel

 cv crv  curve, linear object; LN,CI,EL,CV
 cvbez   CurvBez, Typ_CVBEZ, Bezier-curve.
 cvclt   CurvClot, Typ_CVCLOT, clothoid-curve.
 cvbsp  CurvBSpl, Typ_CVBSP, B-Spline-Curve; was bspl
 cvcon  Conic-Curve; hyperbola|parabola.
 cvpnm  pspl  Typ_CVPSP3, Polynom.Spline.
 cvbnd  contour; bnd  boundary; curve, closed.
 cvtrm  trimmed (limited) curve Typ_CVTRM
 ncv    Array of curves;  was cva

 sur   Surface (all types)
 stps  trimmedPerforateSurface, Typ_SUTP
 sru   RuledSurface, Typ_SURRU
 srv   RevolvedSurface, Typ_SURRV
 sbsp  B-SplineSurface, SurBSpl, Typ_SURBSP
 srbsp Rat.B-SplineSurface, SurRBSpl, Typ_SURRBSP
 sswp  Sweep Surf, SurSwp, Typ_SURSWP
 spu   Surf-planar-unperforated;  spp Surf-planar-perforated.
 sus   SurStd, StandardSurface.
 Tes   tesselated surface (openGlSurface)
 rsys  struct Refsys = plane, Mat_4x3, inverted mat, backplane;  Typ_Refsys
 constrPln   construction-plane
 vtx   vertex

 bbox  boundingBox
 tria  Triang (3 points (pointers))
 rbox  rectangleBox; Viereck)
 edg   Edge (Kante)
 fac   Fac3, Typ_Fac3: Face; indexed-triangle; 3 int's (pointIndices)
 -"-   FacNf  Face with its NeigbourFaces
 pMsh  Typ_SURPMSH, point-mesh
 bMsh  Typ_GL_Sur, binary-mesh           INF_FMTB_BinaryMesh
 patch Opengl-patch; type, points (GL_TRIANGLE_STRIP|GL_TRIANGLE_FAN.. ))
 ipatch indexed-Opengl-patch; type, indexTable, points.

 ox   Complex-Object                     ObjGX  OGX_..
 otb  struct ObjTab - table of binaray objects
 crv  linear object; LN,CI,EL,CV ..      typ+struct
 sur  planimetric obj; plane, surf       typ+struct
 obj  object-structure; any object       typ+struct   typ==form; eg Typ_PT
 dbo  DB-object; any obj                 typ+dbInd                        INF_DBO__
 ccv  Concatenated-Curve (ObjGX)
 ato  atomic-object; any obj             austyp/austab (int/double)
 src  Source-object (text)
 oid  ObjectID                           string       (of DB-object eg "P20")
 osrc source-obj; eg "P20" or "P(L20 PTS(1))"
 onam object-name                        string (eg: onam="height" for oid=V20)
 ffnam  full-filename (/path/filename.filetyp)
 syFn   symbolic-filename, eg "Data/Niet1.gcad";  "Data" = syDir
 syDir  symbolic-directory; list is ../../gCAD3D/cfg_Linux/dir.lst
 oFn    filename-obj (stru_FN)

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
 dst   distance
 gcp   gravity-center-point (Schwerpunkt)
 lva   length-value-table
 pva   parameter-value-table

<propertyCodes> ------------------------------------
 clo     closed (curve, surface)
 cyc     cyclic, passing endpoint / startpoint, not ending at endpoint
 rev     reverse, not forward
 perp    Orthogonal (Normal)
 parl    parallel
 perp    perpendicular = senkrecht/lotrecht/normal   nor
 on      auf
 in      zwischen between
 mid
 near
 equ     identisch
 norm    normieren
 orient  ausrichten
 appr    approximate, not precise (Abschaetzung) 


<functionCodes> ------------------------------------
  __      "encode from"
  get   
  set
 add
 div     teilen (divide)
 sub     subtract
 mult    multiply
 decr    decrement
 incr    increment
  ck      check
  cpy     copy
  ev      evaluate (parameter)
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
  tri     Triang, Dreiecksberechnung
  tng     Tangente, tangential
  std     Standard (characteristic-points,)
  swap
  reset
  resolv
  reserve
  encode
  decode
  hili    viewstate hilited
  dim     viewstate dimmed

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
            p=PT v=VC c=CI l=LN s=Curv ox=ObjGX m=Memspc 
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
                MEM_ins_* MEM_del_* MEM_swap_*              ../ut/ut_mem.h
INF_const__     constants
INF_tol__       tolerances
INF_C_types     [U]INT_[8|16|32]                            ../ut/ut_types.h

INF_debug       errormessages     ../../doc/gcad_doxygen/Debugging.dox
INF_MSG_new     create new message

INF_TxtTab      Textstrings:     TxtTab   UtxTab_NEW UtxTab_add


================================================================== \endcode */}
void INF_APP__ (){        /*! \code
INF_APP__           create modify resolve, attributes, Userinteractions, coords


INF_GEOM_OBJ_CR   create modify geom.objects
INF_UNDO__        undo creation of geom-obj
INF_GEOM_OBJ_RES  resolve geom.objects
INF_GR_attrib     attributes for geom.objects (linetyp, color ..) INF_COL_CV
INF_INTER_USER    Userinteractions
INF_COORD_CONV__  coordinates 3D -> 2D -> 3D;  see also INF_ConstructionPlane
INF_ConstructionPlane   activated plane = 2D
INF_basModels__




Tests:
../mytmp/


Old devDoc: ../../doc/gcad_doxygen/Programminginfos.dox
            ../../doc/gcad_doxygen/Tables-Format.dox



================================================================== \endcode */}
void INF_OBJ_FORM__ (){        /*! \code
binary-obj-types obj, source-obj SRC, atomic-obj ato, DBO dlo

INF_obj_types   every struct has a corresponding typ (integer)  ../ut/AP_types.h

INF_obj__       binary-obj, from typ + binary-struct
INF_SRC__       source (asciiText with objectIDs and functions ..) _src_
INF_ato__       atomic-objects ato
INF_DBO__       DB-obj DB-type & DB-index (int, long) dbo
INF_dlo__       dispList-object    dlo



================================================================== \endcode */}
void INF_obj__ (){        /*! \code

INF_obj_types   object-types and corresponding struct (Point ..)

INF_obj_ID      obj-ID is eg "P123"      _oid_
INF_obj_dat     get binary data of binary-obj from typ + binary-struct
INF_obj_names   objectName is eg "#height 2 floor" 

INF_OGX_CV_CCV  concatenated-curve (CCV)
INF_OGX_DBO
INF_OGX_SUR__
INF_FMTB_SUTP


Functions:
DEB_dump_obj__        
UTO_siz_stru        get size of structure
AP_typDB_typ        get DB-typ from struct-typ
UTO_copy_stru       copy struct (no resolve - not recursive)
UTO_ck_surfTyp      returns surfSubTyp (see also SUR_ck_typ)

INF_binaryObjects   get binObj from other objects





================================================================== \endcode */}
void INF_obj_types (){        /*! \code
every struct has a corresponding typ (interger)

eg the type of struct Point is Typ_PT
   the type of struct int   is Typ_Int4

see ../ut/AP_types.h
see INF_groupTypes


short
name     funcGrp      struct    form          descr                    files,funcs
-----------------------------------------------------------------------------------
                    double    Typ_VAR       numer. variable
                    Vector    Typ_VC
pt     UT3D_        Point     Typ_PT        point                    INF_Typ_PT
ln                  Line      Typ_LN
                    Circ      Typ_CI
                    CurvPoly  Typ_CVPOL     polygon                  INF_Typ_CVPOL
                    CurvPol2  Typ_CVPOL2    polygon-2D               ../ut/ut_plg.c
                    CurvElli  Typ_CVELL     ellipse                  INF_Typ_CVELL
                    CurvBez   Typ_CVBEZ                              ut_bez.c
                    CurvBSpl  Typ_CVBSP                              INF_Typ_CVBSP
                    CurvRBSpl Typ_CVRBSP                             INF_Typ_CVRBSP
                    CurvClot  Typ_CVCLOT    ClothoidCurve
       CVTRM_       CurvCCV   Typ_CVTRM     TrimmedCurve             INF_Typ_CVTRM
....................................................................................
stp    SUTP_        ObjGX     Typ_SUTP    trimmed,perforated surf (SurTP)  INF_SUTP
       UT3D_pl_*    Plane     Typ_PLN       Plane                    INF_Typ_PLN
....................................................................................
                    Refsys    Typ_Refsys    (back)plane,tra.matrix   INF_Typ_Refsys
                    GText     Typ_GTXT
                    AText     Typ_ATXT, Typ_Tag
....................................................................................
       UTcol_       ColRGB    Typ_Color     color                    INF_Typ_Color
ox     OGX_         ObjGX     Typ_ObjGX     complex-object           INF_Typ_ObjGX
obj    UTO_         typ+data  int+void*     binary-obj               INF_UTO__
otb    OTB_         ObjTab    - undef !     obj+xDat[]               INF_ObjTab
                    ObjBin    - undef !
dbo    DB_          typ+dbi   int+long      DB-object (dataBase)     INF_DBO__
                    ObjDB     Typ_ObjDB
ato    ATO_         ObjAto    Typ_ObjAto    atomic-object
txo    APED_txo_    ObjSRC    Typ_ObjSRC    source-object
jnt    JNT_         -         Typ_Joint                              INF_joints
bbx    BBX2_        BBox2     Typ_BBox2     boundingBox-2D           INF_BBOX
odl    DL_          DL_Att    - undef !     DisplayListRecord
                    int[]                   table of ints            ut_iTab.c
sr                                          sense-of-rotation;       INF_sr
....................................................................................
mtb    MemTab_      MemTab                  fixed-length-records     INF_MemTab
msp    UME_         Memspc                  Variable-Length-Records  INF_Memspc
otb    OXMT_        OgxTab                  ObjGX + var-len-record   INF_OgxTab
       UtxTab_      UtxTab                  Textstrings              INF_UtxTab
       BitTab_      BitTab                  Bit-arrays               INF_BitTab
....................................................................................
mdb                 ModelBas  Typ_SubModel  basicModel               INF_basicModel
mdr                 ModelRef  Typ_Model     modelReference           INF_modelRef
....................................................................................




see -
INF_struct_ObjGX
INF_struct_dir
INF_struct_par
INF_struct_closed


Functions:

TYP_IS_CV            check DB-typ
AP_typDB_typ         get DB-typ from typ
AP_typ_typChar       make typ from typChar  ("P" -> Typ_PT)
AP_typ_FncNam        get type of function
AP_typ_srcExpr       get type of expression
AP_typTxt_typ        get typTxt from typ (eg "Typ_PT" from Typ_PT)

AP_typ_ck_sym        check if typ is symbolic-typ / not basic typ (VC,Tra,VAR,Act)

DEB_dump_obj__       dump binary obj (typ,obj)


================================================================== \endcode */}
void INF_groupTypes (){        /*! \code

Typ_goGeom    all (D P L C S A B)

Typ_goPrim    P|L|C|S(Ell,Bsp,Plg,CCV) NOT D|R|A|B
                                       Konturobjs ruledSurf,
                                       tang.Objs for a Circ;
                                       obj's for a ruled surf;

Typ_go_LCS    L|C|S(Ell,Bsp,Plg,CCV)   NOT P|D|A|B
                                       Konturobj RevSur; Konturobj RuledSur;

Typ_go_lf1    L|C|S(Ell,Bsp,Plg,CCV)   NOT P|D|A|B|contour
              only single trimmed curve; for standard- and control-points of obj

Typ_go_lf2    L|C|S(Ell,Bsp,Plg)       NOT P|D|A|B|CCV
              no trimmed-curves:       for: CUT (result = trimmed-curve)

Typ_goGeo1    Typ_go_LCS|R|Sur|Sol     alle curves,  NOT P|D;
                                       parametr. point on obj;
                                       intersection - 2objs

Typ_goGeo2    C|Ell                    Centerpt Circ,Elli; Tang.|Z-Axis;         UU

Typ_go_PD     PT|VC                    gives Point or Vector

Typ_goGeo4    UNUSED   D|L|Pln            liefert ein ModelRefsys                UU

Typ_goGeo5    C|Ell|Plg|Bsp|CCV         NOT D,P,L,A,B
                                        closed Contour

Typ_goGeo6    P|L|C                     NOT D|S|A|B
              L(Plg),L(CCV),C(CCV)      Line(also from Plg,CCV),Circ.
                                        "CIR Tang Tang ..

Typ_goGeo7    Val|V|P|D                 NOT R|A|B
              D(LN),D(Plg),D(CCV)       Direction from ang,vec or pt. RevSur.
              Val|V  out as value (not VAL, not ANG)

Typ_goGeo8    Val|V|P                   Distance, Parameter (f.RevSur)|Point
              VAL(C)=Radius;            "CIR Cen Radius (Radius)

Typ_go_RA     plane|surface             eg surface for offset-curve

Typ_go_JNT    P|Typ_GTXT                Joint

Typ_goGeoSUSU Sur|Sol                   Supporting Surface CON/TOR/SRU/SRV/SBS



================================================================== \endcode */}
void INF_obj_dat (){        /*! \code
INF_obj_dat     get binary data of binary-obj from typ + binary-struct

Functions:
UTO__dbo    get binary data of binary-obj from typ + binary-struct
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
DBO = DB-object (typ,dbi(DB-index))

APED_dbo_oid        get type and dbi from object-ID
DBO__cmp_dbo        compare 2 DB-objects

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
void INF_Typ_CVPOL (){        /*! \code

Functions:
UT3D_plg_pta


Files:
../ut/ut_plg.c

================================================================== \endcode */}
void INF_Typ_CVBSP (){        /*! \code

Functions:
APT_decode_cv APT_decode_bsp_ APT_decode_bsp1
UT3D_bsp_ox
UT3D_cbsp_ptn
UT3D_pta_bsp


Files:
../ut/ut_bspl.c	           B-Spline-Curve-Utilityfunctions 
../ut/ut_bsp_approx.c	     Approximation Polygon -> B-Spline 
../ut/cirMoeb.c	           Curves (bsplines, conic sections, etc.) 


================================================================== \endcode */}
void INF_Typ_CVRBSP (){        /*! \code

Functions:
APT_decode_rbsp


Files:
../ut/ut_rbspl.c           functions for Rat.B-Spline-Curves




================================================================== \endcode */}
void INF_Typ_PLN (){        /*! \code
Plane
see INF_ConstructionPlane

Defaultplanes are:
DB-indexes    obj_ID    normal-to
DB_PLX_IND     DX         X-axis
DB_PLY_IND     DY         Y-axis
DB_PLZ_IND     DZ         Z-axis
DB_PLIX_IND    DIX        negative-X-axis
DB_PLIY_IND    DIY        negative-Y-axis
DB_PLIY_IND    DIZ        negative-Z-axis


================================================================== \endcode */}
void INF_sr (){        /*! \code
sense-of-rotation;   0=CCW, 1=CW.

  sr = DLIM01(rad);   // 0 if (rad >= 0.); 1 if (rad < 0.)

  sri = ICHG01(sr);   // change sr; 0 -> 1, 1 -> 0;

was: 1=CCW, -1=CW
  change to 0=CCW; 1=CW; with:   sr1 = (sr0 > 0) ? 0 : 1;



================================================================== \endcode */}
void INF_UTO__ (){        /*! \code
- binary-obj - defined by typ+data
  type eg = Typ_PT, data = struct Point
  see also complex-obj; typ = Typ_ObjGX, struct = ObjGX; func OGX_*, INF_Typ_ObjGX

../ut/ut_obj.c
obj    UTO_         typ+data  int+void*     binary-obj               INF_UTO__


//----------------------------------------------------------------
serialize:   copy all objs and all data sequentially into a given memchunk
relocate:    after reading obj-tree from file must relocate ..
isolate:     change obj-links into structs; eg ObjGX "P1" into struct (Point)


..............
serialize:      use UME_add_obj
UME_cpy_nRec     copy bin. records into Memspc. see UME_add_nRec
UME_add_nRec      copy bin. records into Memspc. see UME_cpy_nRec

..............
UTO_copy_obj     copy ObjGX-tree    - used by TSU_store
  UTO_copy_tab     copy ObjGX-object-data
    UTO_copy_stru    nur copy n structs


..............
wrl_reloc__  tess_reloc_f_
  UTO_reloc_obj   change ox-ptr data old -= new  (aber nicht ptrs in structs)

..............
OGX_tst_load_ox < OGX_tst__
JNT_exp__
  OGX_deloc__ > OGX_reloc_ox
  OGX_reloc__      relocate all ptrs (must have been seralized-copied before)
    OGX_reloc_ox     add or subtract all ptrs
      OGX_reloc_adr    add or subtract ptr
OGX_tst_load_ox
  OGX_ox_copy__    copy (serialize) a ObjGX-Tree into a single memChunk.
    OGX_ox_copy_obj
      MEM_copy_oTab    copy records

..............
UTO_isol__   replaced by OGX_ox_copy__ UU
  UTO_copy_0   copy ObjGX; replaced by OGX_ox_copy__
    UTO_copy_tab     copy ObjGX-object-data
  UTO_isol_l0  replaced by OGX_ox_copy__   change Links into structs
    UTO_isol_ll1       change Links into structs; replaced by OGX_ox_copy__
    UTO_isol_llt       change indexed-point -> struct-point

..............
OGX_ox_copy__ < OGX_tst_load_ox  UU
  OGX_ox_copy_obj copy (serialize)



================================================================== \endcode */}
void INF_ObjTab (){        /*! \code
functions for
  ObjTab =  list of [oTyp, pointer to oDat, Memspc for oDat, aux.obj]     OTB_*
../xa/tst_ut_objtab.c

type of memSpc = MEMTYP_STACK__  (realloc - NO,    malloc - NO,   free - NO.)

struct ObjTab has memspaces:
- Memspc oSpc    keeps oNr different binary records
- void   **oDat  list of pointers to the records in oSpc
- int    *oTyp,  list of types of records in oSpc
- void   *xDat;  keeps oNr records with recordsize = xSiz = UTO_siz_stru(xTyp);



================================================================== \endcode */}
void INF_Typ_ObjGX (){        /*! \code
ObjGX                 complex-object         definition
OGX                   ../ut/ut_ox_base.c     functions

INF_FMTB_ObjGX          description binary format
INF_OGX_DBO
INF_OGX_SUR__
INF_FMTB_SUTP


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
  Parameters v0,v1 (startpoint, endPoint) of Circ, CurvElli are normalized (0-1)
                   CurvPoly has length, CurvBSpl has knotvalues;


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
void INF_FMTB_SUTP (){        /*! \code
complexObject - A surface-trimmed-perforated-supported

sutp    SUTP_        ObjGX     Typ_SUTP    trimmed,perforated surf (SurTP)

  - ObjGX typ=Typ_SUTP with support-surface and all boundaries (1 outer, n inner)

  - Record[0] = link to support-surf; PLN|CON|SPH|TOR/SRU/SRV/SBS   INEW_surf_types
  - Record[1] = link to outer boundary (DB-obj - typ,index)
  - Record[2-] - links to inner boundaries


struct ObjGX
         typ  = Typ_SUTP;
         form = Typ_ObjGX;
         siz  = nr of ObjGX-structs in data
         data = oxTab[] - supporting-surface, outer and inner-boundary

    oxTab[0] supporting-surface (DB-obj (INF_OGX_DBO))
             typ=Typ_CON| ..; form=Typ_Index  (PLN/CON/TOR/SRU/SRV/SBS)
             typ=Typ_PLN = planar-surface
             typ=Typ_modUndef = planar-surface; OBSOLETE
    oxTab[1] outer-boundary (DB-obj (INF_OGX_DBO))
             typ=Typ_CI| ..; form=Typ_Index
             typ=Typ_modUndef = unlimited supporting_surface
   [oxTab[2-n] inner-boundaries(DB-objs (INF_OGX_DBO))]


FSUB     T_FSUB  Typ_SUTP    


Functions:
  APT_decode_tps__   new Version, not-yet-active  ox from ato
  SUTP_decode_sSur
  DB_StoreSur         // save ox (surface) in DB)

see also APT_decode_fsub APT_decode_spl
see also INF_OGX_SUR__



================================================================== \endcode */}
void INF_COL_CV (){        /*! \code

  attribute of line/curve.

The attribute is the index, defining color, linetyp and thickness.

Example:
  att = 9; // col hilite-red (822), full-line (0), lineThickness 4 pixels wide


sourcefile-runtime: <cfgdir>/ltyp.rc    /mnt/serv1/Devel/gcad3d/gCAD3D/cfg_Linux/ltyp.rc
sourcefile-devel:   ../../gCAD3D/cfg_Linux/ltyp.rc -> examples.gz

ind col typ thk
  0 000  0  1      // Normal               Typ_Att_def
  1 116  0  2      // text blue            Typ_Att_blue
  2 000  2  1      // dashed               Typ_Att_dash__
  3 000  1  1      // dash-dot             Typ_Att_dash_dot
  4 755  0  1      // faces1 thick1        Typ_Att_Fac
  5 116  3  2      // faces2 thick1        Typ_Att_dash_long
  6 555  0  1      // faces3 thick1        Typ_Att_Fac2
  7 990  0  3      // yellow thick3        Typ_Att_Symb
  8 090  0  3      // green  thick3        Typ_Att_hili
  9 822  0  4      // hilite thick4        Typ_Att_hili1
 10 447  0  1      // dimmed thick4        Typ_Att_dim
 11 000  0  4      // black  thick4        Typ_Att_top1
 12 990  2  6      // yellow dash thick6   Typ_Att_top2

#
# Format: indexNr colour lineTyp lineThick // comment
#
#   colour     3 digits; red, green, blue.
#              900 = red,   090 = green, 009 = blue;
#              000 = black; 999 = white, 990 = yellow,  909 = cyan,  099 = magenta;
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
void INF_struct_dir (){/*! \code


direction: 0=forward, curve along ascending parameters;      (char dir)
           1=backward, reverse; curve along descending parameters.
See also INF_struct_closed
See also INF_struct_par


Circle and Ellipse:
  default-direction is counter-clockwise (0 = fwd)
  can be set to CW (clockwise, 1 = bwd).


These objects can be reversed (CAD - Modify - REVert)
  Circle, ellipse, polygon, bspline.
  Result is a trimmed-curve (struct CurvCCV) with .rev = 1 (direction reverse).

  Parameters for reversed curves also start with 0 and go up to 1.
  The same point for a fwd-curve and its reversed curve has different parameters;
    the startpoint on the fwd-curve has parameter 0;
    this point is endpoint of the reversed-curve and has here parameter 1.


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
void INF_struct_closed (){        /*! \code

closed; 0=yes, 1=not_closed; -1=undefined;      (char clo)
trimmed; 0=yes, 1=not_trimmed, -1=undef;        (char trm)
See also INF_struct_dir
See also INF_struct_par

closed: always the state of the basic-curve (also for trimmed-curves)
  closed=yes endpoint == startpoint
  - curve can be trimmed
  - see UTO_cv_ck_dir_clo

trimmed: curve of a part of the basic-curve;
  a basic curve always has trimmed=no
  trimmed=yes: this curve is a trimmed part of the basic-curve;
  the basic curve can be closed or not 

cyclic: curve is not closed, curve is passing endpoint=startpoint   
  - curve is trimmed, its basic curve is a closed curve.
  - closed curve from startpoint to endpoint is NOT cylic
  - see UTO_cv_ck_cyc

trimmed: if curve is closed: curve passes whole cycle or not  (char trm)
  -  not-trimmed: curve from startpoint to endpoint
  -  not-trimmed: curve from midpoint to midpoint


Funcs:
  clo   UTO_cv_ck_dir_clo


================================================================== \endcode */}
void INF_BBOX (){        /*! \code
../ut/ut_box.c

Point *pb1, Point *pb2,        // LL (lower-left), UR (upper-right)

bbx    BBX2_        BBox2     Typ_BBox2     boundingBox-2D           INF_BBOX





================================================================== \endcode */}
void INF_MSG_new (){        /*! \code

- constant messages: see INF_MSGconst_new

Messages are displayed in the messagewindow below the grafic-window.
Messages are selected by a keyword and have translations
  in the messagefiles ../../doc/msg/msg_<lang>.txt


Functions:
  MSG_pri_0            // message without parameters
  MSG_err_1            // message with 1 parameter


Files:
../xa/xa_msg.c
../../doc/msg/msg_*.txt
vi ../../doc/msg/msg_en.txt
vi ../../doc/msg/msg_de.txt
vi `ls ../../doc/msg/*|grep msg_...txt`


TODO
UNUSED keyWords:
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

Examples:
./lang_ins.csh "E_PRJ_1" "E_INT_1=no result from intersection"


kompare ../../doc/msg/msg_en.txt /usr/share/gcad3d/doc/msg/msg_en.txt


================================================================== \endcode */}
void INF_MSGconst_new (){        /*! \code

identified by integers (msgCode - enum in ../../src/xa/xa_msg.h)

Functions:
  MSG_const__(<msgCode>));


FILES:
  ../../doc/msg/msg_const_*.txt        msg-text
  ../../src/xa/xa_msg.h                msgCode - enum MSG_*

Example:
  TX_Print(MSG_const__(MSG_GrpSel));
  GUI_Tip        (MSG_const__(MSG_upd));

Add new constants: 
  add ID in xa_msg.h;
  add msg in msg_const_de.txt (and all others, msg_const_en.txt ..)
    cd ../../doc/msg/


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

Functions:
WC_Init_Tol
UI_WinToler



================================================================== \endcode */}
void INF_MEM__ (){        /*! \code

INF_MEM_ORG_TYP   get organized memspc as -

INF_MEM_SPC     get memspc - temporary | static | permanent


INF_MemTab      Fixed-Length-Records                         MemTab   
INF_Memspc      Variable-Length-Records                      Memspc
INF_OgxTab      Fixed-Length + Variable-Length-Records       OgxTab
INF_TxtTab      Textstrings                                  UtxTab
INF_BitTab      BitArray                                     BitTab

INF_ObjTab      Variable-Length-Records
                  + list of pointer and types
                  + Fixed-Length-Records

INF_MEM_TYP     type of memspc; if can reallocate, must be freed ..
                if stack|malloc, protected,expandable ..

Functions:
MEM_..          Swap-Invert-Delete copy compare write read ..    ../ut/ut_mem.c




../myUnused/gcad_doxygen/Tools-MemoryFunctions.dox


================================================================== \endcode */}
void INF_MEM_SPC (){        /*! \code
  get memSpc  -  temporary | permanent | static


----------------------------------------------------------
MEM_alloc_tmp     get temporary-memspc
  calls alloca; memspace exists until active function returns.
  max size should be SPC_MAX_STK = 32767 bytes
  cannot be increased, must not free.
  Example:
  int   *iTab2;
  iTab2 = (int*) MEM_alloc_tmp (sizeof(int) * lnNr);
  if(!iTab2) goto L_EOM;


- tempSpace (fixed size) > SPC_MAX_STK: ?


----------------------------------------------------------
-                 get permanent-memSpace         
malloc () .. free


----------------------------------------------------------
-                 get static-memSpace                ../xa/xa_mem.h
DO NOT USE this memSpaces - cannot be locked 





================================================================== \endcode */}
void INF_MEM_ORG_TYP (){        /*! \code

the memory-space types are defined in ../ut/ut_types.h as MEMTYP_*


INF_MemTab      Fixed-Length-Records         MemTab_       ../ut/ut_memTab.c
INF_Memspc      Variable-Length-Records      UME_          ../ut/ut_umem.c
INF_OgxTab      ObjGX + var-len-record       OXMT_
INF_TxtTab      Textstrings                  UtxTab_       ../ut/ut_txTab.c
INF_BitTab      Bit-arrays                   BitTab_       ../ut/ut_BitTab.h


================================================================== \endcode */}
void INF_MEM_TYP (){        /*! \code

type of memspc; if can reallocate, must be freed ..

see MEMTYP_NONE - MEMTYP_STACK_EXPND                               ../ut/ut_types.h

see MEM_CANNOT_FREE                                                ../ut/ut_mem.h


---------------------------------------
free:
  MUST free:  MEMTYP_ALLOC__ 
  else cannot free

realloc:
  CAN realloc:     MEMTYP_NONE
                   MEMTYP_ALLOC__
                   MEMTYP_ALLOC_PROT

  CANNOT realloc:  MEMTYP_ALLOC_EXPND
                   MEMTYP_STACK__
                   MEMTYP_STACK_EXPND

expand:  (malloc & copy)
  CAN expand:      MEMTYP_ALLOC_EXPND
                   MEMTYP_STACK_EXPND
  


================================================================== \endcode */}
void INF_OgxTab (){        /*! \code

otb    OXMT_        OgxTab                  ObjGX + var-len-record   INF_OgxTab

OgxTab is a container of a MemTab .ogx plus a Memspc .spc -
  MemTab is a list of Fixed-Length-Records                           see INF_MemTab
  Memspc is a list of Variable-Length-Records                        see INF_Memspc


Examples:
  OgxTab otb1 = _OGXTAB_NUL;
  OXMT_test_1 ()


Files:
../APP/ut_ogxt.c

../ut/ut_ogxt.c
../ut/ut_ogxt.h


================================================================== \endcode */}
void INF_UtxTab (){        /*! \code

add, find, reallocate - strings terminated with '\0'.

../ut/ut_txTab.c
../ut/ut_txTab.h

see INF_TxtTab


================================================================== \endcode */}
void INF_MemTab (){        /*! \code

MemTab            Fixed-Length-Records         MemTab_       ../ut/ut_memTab.c
                                                             ../ut/ut_memTab.h

  MemTab_add ..      // add or reserve records
  MEMTAB_IND         // get or set index (next free = nr of used)
  MEMTAB_DAT         // get data-record complete

-----------------------------------------------------------------------
# get temporary-memspc:

  // for known nr of records:
  MemTab(int) mtbi1 = _MEMTAB_NUL;
  MemTab_ini_temp (&mtbi1, Typ_Int4, rNr);   // does malloc if > SPC_MAX_STK

  // max nr of records 
  MemTab(Point) mtpa = _MEMTAB_NUL;
  MemTab_ini_fixed (&mtpa, MEM_alloc_tmp (SPC_MAX_STK), SPC_MAX_STK,
                  sizeof(Point), Typ_PT);
  ..
  MemTab_add (&mtpa, &l1, &pt1, 1, 0);
  ..
  MemTab_free ((MemTab*)&mtba);  // if realloc permanent-memspc (gt SPC_MAX_STK)


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
void INF_Memspc (){        /*! \code

Memspc            Variable-Length-Records      UME_          ../ut/ut_umem.c

-----------------------------------------------------------------------
# get temporary memspc:
  char      tmpspc[100000];
  Memspc    tmpSeg;
  UME_init (&tmpSeg, tmpspc, sizeof(tmpspc));


# get stack-space
  #define SIZ_TMP 2000
  Memspc    memSeg1;
  UME_init (&memSeg1, MEM_alloc_tmp(SIZ_TMP), SIZ_TMP);


-----------------------------------------------------------------------
# get static memspc:
  UME_init (&s_mSpc, memspc501, sizeof(memspc501));


-----------------------------------------------------------------------
# get permanent memspc:
  irc = UME_malloc (&impSpc, spcSiz, incSiz);



================================================================== \endcode */}
void INF_BitTab (){        /*! \code

BitTab            Bit-arrays                   BitTab_       ../ut/ut_BitTab.h



================================================================== \endcode */}
void INF_basicModel (){        /*! \code

mdb                 ModelBas  Typ_SubModel  basicModel               INF_basicModel


Variables:
DYN_MB_IND      nr of basic models

Functions:
../xa/xa_mod.c
Mod_mNam_mdb      get ModbasName from basicModelNr


================================================================== \endcode */}
void INF_modelRef (){        /*! \code

mdr                 ModelRef  Typ_Model     modelReference           INF_modelRef

Variables:
mdr_tab[APT_MR_IND]


Functions:
DB_StoreModel

../xa/xa_mod.c



================================================================== \endcode */}
void INF_workflow__ (){        /*! \code
 sequence functions

INF_workflow_events         main-events
INF_workflow_select         select-process
INF_workflow_models         load model in temp.dir at startup


------ main-startup:
main                      ../xa/xa_main.c
  AP_get_dir__    // set OS_get_tmp_dir OS_get_cfg_dir OS_get_doc_dir OS_get_ico_dir
  // if cfg_<os>/gCAD3D.rc does not exist:
  //   UX: extract examples.gz and rename dir. /cfg/ /cfg_Linux/ or /cfg_MS/
  //   MS: copy config-files from \cfg\ -> \cfg_MS\.
  //   write or load defaults (cfg/xa.rc)
  //   dirLocal is base of tmp, dat, cfg_<os>, ctlg, prg
    AP_defaults_write  create cfg_<os>/xa.rc
    AP_defaults_dir    create file cfg_<os>/dir.lst = symDirs Data, CATALOG, APPLI
  UI_win_main             create main-window
    UI_GL_draw__            callback OpenGL-startup
      AP_init__                 startup (UI_GL_draw__
        AP_work__                 work startparameters

ED_work_END
  ED_work_CurSet            work n lines of src-code
    WC_Work1

  GUI_Win_exit      // Exit mit X:
    UI_win_main "UI_FuncExit"
      AP_exit__
      GUI_Win_kill
      exit

  AP_work__ ("crashEx", NULL);
    AP_exit__
    exit

UI_men__ ("new")    //  File/New

AP_mdl_init       // File/New, File/Open



------ CAD:
WC_Work1                      execute codeline
  APT_work_def                handle definition-line
    APT_store_obj             store in DB
      APT_decode_pt
      APT_decode_bsp_
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
    DL_txtSelect              test if note/tag selected
    sele_src_cnvt__           add all useful components of selected obj
    UI_GR_Select2             callback von GUI_Popup (Liste der sel.Obj)
      UI_GR_Select_work1       work GR_Sel_Filter 3-7
        //sele_decode             change selected_object into requested_object
      UI_GR_Select_work2      work  hide,view,SM or call
        IE_sel_CB__            write selection -> CAD-inputfield
       IE_cad_sel1            CAD-report sel.
       ED_add_Text            MAN
    UI_popSel_CB_prev         hilite obj and/or unhilite last displayed object
  UI_vwz_set
  UI_GR_Indicate

UI_popSel_CB__


TODO: types Typ_XVal,Typ_YVal,Typ_ZVal: select P D L as X(P...) ...


-------------------------------------------------------------------------
UI_GR_Select1         // get selectionMenu (sel.Obj + useful subObjs)
  sele_src_cnvt__         // sourceCode of all useful components of selected obj
    SRC_src_pt_dbo         // create src from point and DB-obj (curve, surface)
      ATO_ato_obj_pt         // get all atomicObjs for selection
        GR_pt_par_sel_npt      // get selectionpoint and parameter on polygon
      SRC_src_ato            // sourceObj from ato


-------------------------------------------------------------------------
FUNCTIONS:
sele_set_pos__         // set GR_selPos_CP and GR_selPos_vtx
sele_get_pos_CP        // get GR_selPos_CP (on constrPln, WCS)
sele_get_pos_vtx       // get GR_selPos_vtx (vertex, WCS)
sele_get_pos_UC        // get UCS-coords of GR_selPos_CP (curPos on constrPln)

sele_set_add       add obj to selectionfilter reqObjTab; eg Typ_go_PD = P+D
IE_inpCkTyp        connect selectType (eg Typ_go_JNT) with outType (eg Typ_Joint)
IE_txt2par1        get next parameter(s) from atomicObjects IF USEFUL
sele_src_cnvt__    test if component of obj is useful
IE_inpTxtOut


UNUSED:
  sele_decode     converts the selected obj into a requested obj

-------------------------------------------------------------------------
VARIABLES:
static Point  GR_selPos_vtx;     // last sel. vertex in WCS
static Point  GR_selPos_CP;      // curPos last sel. on constPlane in WCS




================================================================== \endcode */}
INF_workflow_Hide_View (){        /*! \code

// activate Hide (set in DL ((disp == 1)&&(hili == 1)))
UI_CB_hide

// activate View (set in DL ((disp == 0)&&(hili == 1)))
UI_CB_view
  Grp_Clear

DL_disp_def (1);       // set hidden for following DL-obj
DL_disp_def (0);       // reset hidden to normal




================================================================== \endcode */}
INF_workflow_display (){        /*! \code

For all visible object a record in "DL" (DisplayList) exists.
../gr/ut_DL.c   DisplayList DL (INF_DisplayList-record)

../gr/ut_GR.c   display objects. Primary display-functions, using OpenGL-functions.
../gr/ut_GL.c   OpenGL-functions


INF_GR_RECORDS
INF_GR_preview       temp. disp symbolic-obj (VC,Tra,VAR,Activ)


Persistence of displayed objects:
- permanet                  INF_Fixed_Database_record
- permanent/dynamic         INF_Dynamic_Database_record
- temporary                 INF_Temporary_DisplayList_record


GR_perm_<obj>           create permanent obj


Workflow GR:
  GR_perm_<obj>|GR_tDyn_<obj>|GR_temp_<obj>        create perm|tDyn|temp disp-obj
    GR_set_<obj>
      DL_perm_init|DL_tDyn_init|DL_temp_init       create DL-record
        GL_list_open                               open GL-list
      GL_att_..                                    init GL-attributes pt|sym|cv|sur
      GL_set_<obj>                                 add obj into open GL-list
      GL_list_close                                close GL-list


Workflow GL:
  GL_list_open  open GL-record
  GL_att_*      set attributes for following obj into open GL-list
  GL_set_*      add obj into open GL-record
  GL_list_close close GL-record
  

// disp revolved surface 
APT_Draw__
  GR_perm_sur
    GR_set_sur
      GR_set_sur
        GR_set_osu
          DL_perm_init|DL_tDyn_init|DL_temp_init
            DL_set__                                 // create DL-record
            GL_list_open
          TSU_SUR__
            TSU_DrawSurTRV
              GR_set_strip_v
                GL_set_strip_v
          GL_list_close


OBSOLETE - TODO:
INT_intsursur INT_intplsur DXFW_3DFACE__  is using - only for intersection -
  TSU_tess_sTab TSU_tessObj APT_DrawSur GR_DrawSur TSU_DrawSurT_




================================================================== \endcode */}
INF_workflow_MAN (){        /*! \code

// activate MAN
UI_butCB |MAN|
  UI_MAN_ON
    UI_src_edi            // copy complete SRC from mem -> edi

// editor-callback - mouse/key into editor                   
EDI_CB__
  ED_unload__ ();        // test if modified - if yes: copy editor -> memory

// callback mouseButton
UI_GL_mouse__
  ED_GR_CB1__            // analyze
    UI_GR_selMen_init      // create menu accord. sel. obj
  UI_EdKeyCR (2)         // process complete line
    UI_AP (UI_FuncSet, UID_Edit_Line, term_buf);
      GUI_edi_Insert         // insert text

// List-selection:
GUI_popup_cb1
  UI_popSel_CB__
    UI_GR_Select_selLst        // entry in popup-list selected
      // handle "Vertex" "ConstrPlane" Typ_Activ
      UI_GR_Select2

// add selection
UI_GR_Select_work1
  UI_GR_Select_work2
    ED_GR_CB2__
      ED_add_Text
        UI_AP (UI_FuncSet, UID_Edit_Line, term_buf);
          GUI_edi_Insert




================================================================== \endcode */}
INF_workflow_CAD__ (){        /*! \code

IE_cad_test__      // test if input complete, create sourcline, activate OK-butt
  ED_work_CAD        // input is complete; preview or store & display
    WC_Work1          // preview or store & display
      WC_Work1           // preview or store & display
        APT_work_def       // Work DefinitionLine
          APT_ato_par_srcLn  // perm: get parents and atomic-objects
          APT_store_obj      // create binary obj; store obj in DB
          APT_parent_set     // perm: set isParent-bit and hide parent
          PRCV_set_dbo__     // perm: create PRCV
          GA_find__          // perm: find GA-rec
          APT_Draw__         // preview or display obj



================================================================== \endcode */}
INF_workflow_CAD_edit (){        /*! \code


IE_edit_dbo               // M3 - obj + "edit" selected
  ED_set_lnr_act
  IE_activate               // start edit ?
    ED_work_CurSet            // work lines up to selected obj
    IE_modif__
      IE_decode_Ln              // get ato from src
      IE_cad_init1              // activate CAD-subMenu
      IE_cad_test__             // preview
IE_cad_Inp_cancel         // user pressed CAD-Cancel-button
  IE_cad_exitFunc
IE_cad_OK                 // user pressed CAD-OK-button




================================================================== \endcode */}
INF_COORD_CONV__ (){        /*! \code
coordinate-transformations;
see INF_ConstructionPlane

FILES:
../ut/ut_transform.c


FUNCTIONS:

//----------------------------------------------------------------
// worldCoords -> userCoords:
UTRA_UCS_WCS_VC                      transfer vector from WCS into UCS
UTRA_UCS_WCS_PT                      transfer point from WCS into UCS

UT3D_pt_tra_wcs2ucs_pl   point-worldCoords -> userCoords from plane
UT3D_pt_tra_ucs2wcs_pl   point-userCoords from plane -> worldCoords

UTRA_pt_ucs2wcs     point from constructionplane (relativ) to absolut (UCS -> WCS)

UTRA_pt_wcs2ucs     point from absolut to relativ (constructionplane) (WCS -> UCS)


//----------------------------------------------------------------
// userCoords -> worldCoords:
UTRA_UCS_WCS_VC
UTRA_UCS_WCS_PT


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

screenCoords     int, pixels, 2D; 

WCS world-coordinates: absolut; origin = 0,0,0;

UCS user-coordinates:  coords in the current constrPlane;
    coordinats along the axes of the current constrPlane;
    relativ to the origin of the current constrPlane.

The displayed cursorPosition (above grafic-window) is UCS.

DB-coordinates of all objects (on ConstrPlane or not) are WCS (absolut).

SRC-coordinates of objects on a ConstructionPlane are userCoords (relativ);

3D:
  The default-ConstructionPlane is active; all coordinates are WCS (absolute):

2D:
  A user-defined plane is the active ConstructionPlane;
  the cursorPosition (above grafic-window) is UCS;
  the coordinates stored in model (SRC, can be seen in MAN) are UCS).

Making constrPlane ON and OFF both create a DL-record Typ_constrPln

see INF_COORD_CONV__  (transformations)


//----------------------------------------------------------------
VARIABLES:
GL_mouse_x_act,GL_mouse_y_act   // mousepos in screenCoords
Point GL_curPos_SC;  // curPos in screenCoords
Point GL_curPos_WC;  // curPos in WCS on constrPlane
Point GR_curPos_WC   // curPos in WCS on constructionPlane
char  GR_actPos[60]     // "P(<x> <y> <z>)"  set by GR_get_curPos_UC

int GR_actView      // FUNC_ViewIso FUNC_ViewFront FUNC_ViewSide
Point GL_cen        // centerpoint of the Viewport always WCS                       
Plane GL_eye_pln with
Point GL_eyeY GL_eyeZ GL_eyeX (GL_eye_upd())

Plane     WC_sur_act    // ActiveConstrPlane
int       WC_sur_ind;    DB-Index of the ActiveConstrPlane;
                         (WC_sur_ind == 0) is 3D      (CONSTRPLN_IS_OFF)
                         (WC_sur_ind != 0) is 2D      (CONSTRPLN_IS_ON)
char      WC_sur_txt[16];   displayed name_of_Constr.Plane; is "R0" or "R20" or "RX"
Mat_4x3   WC_sur_mat;            // TrMat of ActiveConstrPlane
Mat_4x3   WC_sur_imat;           // inverse TrMat of ActiveConstrPlane



//----------------------------------------------------------------
FUNCTIONS:
CONSTRPLN_IS_ON     // test if constrPln is active;    1=yes, 0=no
CONSTRPLN_IS_OFF

GL_DefineView       // change GR_actView - Front,Side,Axo ..; set GL_eyeX ..
GL_Do_Pan__
NC_setRefsys        // change WC_sur_act WC_sur_ind WC_sur_mat ..

UI_CurPos_upd       // display cursor-position above grafic-window in UCS
UI_GR_actPos        // write actPos -> GR_actPos as "P(...)"
GR_set_curPos_CP    // set GR_curPos_WC = cursorPosition on constructionPlane ??
GR_get_curPos_WC    // return GR_curPos_WC (WCS ??)
GR_get_curPos_UC  // get UCS-coords from curPos (GR_curPos_WC)

GL_get_curPos_SC    // get cursorPosition in screenkoords
GL_get_curPos_WC    // get cursorPosition in WCS (not yet on constrPlane)
GL_get_curPos_CP_pt // get inters.point WCS on constrPln of point along eyeVec
GL_get_curPos_last  // get Coords of last selectionPoint (scrCoords and ??)
GL_get_curPos_CP__  // get curPos in WCS on constrPlane along eyeLine
GL_set_curPos_CP    // save screenCoords & userCoords of selectionPoint
GL_vertex_curPos    // get vertex (WCS) from active curPos
GL_vertex_SC        // get vertex (WCS) from screenCoords

..............................
DL_setRefSys
  NC_setRefsys      Change active Plane (Refsys)
    UI_lb_2D_upd           update label "2D" | "3D"
DL_GetTrInd         get the refsys-nr (dbi of Plane) for DL-record <dli>

AP_Set_ConstPl_Z    write Label name_of_Constr.Plane
AP_Get_ConstPl_Z    get ConstPLn as text or Z-vec
AP_Get_ConstPl_vz   get Z-vec of ConstructionPlane


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
void INF_DBF (){        /*! \code
File-based DB; Save and retrieve key-value-Records
Used with joints.

Files:
../ut/ut_dbf.c
../APP/test_DBF.c          testprog

For examining DB-file use wxHexEditor <tmp/>joints



================================================================== \endcode */}
void INF_catalog (){        /*! \code

Symbol for directory with catalog-files is CATALOG
  Default is ~/gCAD3D/ctlg/
Here are the files <partTyp>.ctlg
  and the necessary partfiles <catalogPart>.gcad
Partfiles can also be in subDirectories of directory <CATALOG>


//----------------------------------------------------------------
Simple example for a catalog-part:  (also in ../../doc/html/Catalog_*.htm
//----------------------------------------------------------------
# Create following model test_ctlg1.gcad in directory <CATALOG>
V20=VAL(10)
CALL CTLG
C20=P(0 0 0) V20
A20=C20

# create catalog-file test_ctlg1.gcap in directory <CATALOG> with -
# CAD - Catalog/Create Catalog / test_ctlg1 OK

# edit catalog-file test_ctlg1.gcap
# CAD - Catalog/Modify Catalog / test_ctlg1

# insert following lines:
R_25;CATALOG/test_ctlg1;V20=25;
R_100;CATALOG/test_ctlg1;V20=100;

# use new catalog:
# CAD - subModels "M CatalogPart" /  List /


//----------------------------------------------------------------
Files in <tmpDir>:
<catalogName>_<partName>.ctlg_dat     eg Schrauben_SKS_6x30.ctlg_dat
DB__<catalogName>_<partName>.dat      eg DB__Schrauben_SKS_6x30.dat





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


see below:
DEB_dump_obj__            DEB_dump_ox_0 ..
MSG_ERR__
DEB_stop                  (conditional) stop in gdb;
                          see also  UI_wait_time UI_wait_Esc__ ERR_raise
DEB_exit                  exit prog; disp func and lineNr
printd                    DEB_prt_init ..
LOG_A__                   LOG_A_init LOG_A_exit ..



-------------- Errormessages: ../xa/xa_msg.h ../xa/xa_msg.c
MSG_ERR__         errormessage (key, additional_text)      see INF_DEB_MSG_ERR
MSG_ERR_*


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

UI_wait_Esc__                  wait for pressing the Esc-Key

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
Debug-file for applications:

  LOG_A_init ("appNam");

  // write into debugFile
  LOG_A__ (MSG_ERR_typ_INF, "exp_export typ=%d dbi=%ld", 3, 20L);
  LOG_A__ (MSG_ERR_typ_ERR, "exp_export typ=%d dbi=%ld", 3, 20L);


  // close debugfile with nr of errorMessages
  LOG_A_exit (2);

  // display debugfile with editor
  LOG_A_disp ();






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



<HTML>
<HEAD>
  <META HTTP-EQUIV="CONTENT-TYPE" CONTENT="text/html; charset=utf-8">
  <TITLE></TITLE>
  <meta name="description" .. ">
  <meta name="keywords" content=".. ">
<STYLE TYPE="text/css">
  H1 { color: #000000; font-family:"Helvetica"; font-size:32pt; font-weight:medium }
  H3 { color: #000000; font-family:"Helvetica"; font-size:16pt; font-weight:medium }
  H4 { color: #000000; font-family:"Helvetica"; font-weight:medium }
  hr { border: none; width: 100%; height: 8; background: #ffcc99;}
</STYLE>
</HEAD>
<BODY LANG="en-US"
      style="font-family:Helvetica; font-size:12pt; font-weight:normal;">
<PRE>

..
<HR><!-- ============================================================ -->

..
<hr>
Send bug-reports, suggestions for improvement to
  franz.reiter@cadcam.co.at
</PRE>
</BODY>
</HTML>





//----------------------------------------------------------------
Tools:
change text in all files:
cd ../../doc/html/
cscope *.htm




================================================================== \endcode */}
// eof
