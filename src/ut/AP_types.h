/***************************************************************************
                          AP_types.h  -  description
                             -------------------
    begin                : Fri Aug 25 2000
    copyright            : (C) 2000 by CADCAM-Services Franz Reiter
    email                : franz.reiter@cadcam.co.at

 ***************************************************************************


-------------------------------------------
Korr:
2013-08-24  extracted from ut_geo.h. RF.


-------------------------------------------
*/
/*!
\file ../ut/AP_types.h
\ingroup grp_ut_geom
*/
#ifdef globTag
void INF_OTYP (){}
#endif
//----------------------------------------------------------------
//   01 -  19  DB-primitives (V.D,P,L,C)
//   20 -  39  DB-curves (S)
//   40 -  79  surfs (A)
//   80 -  89  bodies (B)
//   90 - 109  notes, images
//  110 - 119  temp objs
//  120 - 129  geom. attributes
//  130 - 149  geom. parameters
//  150 - 169  attributes
//  170 - 189  dataFormats (Int, matrix, Polynom..)
//  190 - 199  Text
//  200 - 209  containers
//  210 - 219  transformations
//  220 - 229  operators
//  230 - 249  modifiers
//  250 - 259  events
//  260 - 269  functions
//  270 - 279  function parameters
//  400 - 499  ../gui/gui_types.h      GUI-types TYP_Event* TYP_Device* TYP_GUI_*
// 1000 -1099  ../xa/xa_sele.h         selection-modfiers
//
// Get infoText for types: AP_src_typ__ ();
// See also xa_sele.h
// FIX (UPDATE) ALSO TypTxtTab* in ../ut/ut_gtypes.c
// TYP_IS_CV() TYP_IS_OPM .. in ../ut/ut_geo.h



#define Typ_ALL_OBJS      -1
#define Typ_Error          0  ///<     U0 = undefined

/// DB-primitives 1-19
#define Typ_VAR            1  ///< V   numer. variable
#define Typ_VC             2  ///< D   Vector
#define Typ_PT             3  ///< P   Point
#define Typ_LN             4  ///< L   Line (CVLn3)
#define Typ_CI             5  ///< C   Circ
// #define Typ_VCP               ///< D  position-vector
#define Typ_VC2           10  ///< D   Vector2
#define Typ_PT2           11  ///< P   Point2
#define Typ_LN2           12  ///< L   Line2
#define Typ_CI2           13  ///< C   Circ2
#define Typ_CI2C          14  ///< C   Circ2C centerPosition
#define Typ_VC3F          15  ///< D   Vec3f
#define Typ_CVLN3         16  ///< S   CVLn3 (Line Typ_LN)
// #define Typ_VCP2              ///< D   position-vector, Plane-2D

/// DB-curves 20-39
#define Typ_CV            20  ///< S   all curves
#define Typ_CVPOL         21  ///< S   CurvPoly
#define Typ_CVPSP3        22  ///< S   polynom_d3
#define Typ_CVBSP         23  ///< S   CurvBSpl
#define Typ_CVRBSP        24  ///< S   CurvRBSpl
#define Typ_CVELL         25  ///< S   CurvElli
#define Typ_CVBEZ         26  ///< S   CurvBez
#define Typ_CVRBEZ        27  ///< S   CurvRBez
#define Typ_CVCLOT        28  ///< S   ClothoidCurve  (CurvClot)
#define Typ_CVLNA         29  ///< S   array of lines (form=Typ_LN)
#define Typ_CV2           30  ///< S   all 2D-curves
#define Typ_CVPOL2        31  ///< S   CurvPoly-2D    (CurvPol2)
#define Typ_CVPSP2        32  ///< S   polynom_?
#define Typ_CVBSP2        33  ///< S   CurvBSpl2
#define Typ_CVRBSP2       34  ///< S   CurvRBSpl-2D
#define Typ_CVELL2        35  ///< S   CurvEll2
#define Typ_CVELL2C       36  ///< S   CurvEll2C centerPosition
#define Typ_CVBEZ2        37  ///< S   CurvBez2 bezier-curve-2D
#define Typ_CVTRM         38  ///< S   CurvCCV trimmed-curve

/// DB-surfs 40-79
#define Typ_PLN           40  ///< R  Plane
#define Typ_Refsys        41  ///< -  Refsys

#define Typ_SUR           50  ///< A all surfaces (support-surface, bounded)
// SupportSurfaces (not trimmed, not punched)
#define Typ_SURCON        51  ///< Conus (unused)
#define Typ_SURTOR        52  ///< Torus (unused)
#define Typ_SURRU         53  ///< A Ruled Surf
#define Typ_SURRV         54  ///< A Revolved Surf       SurRev
#define Typ_SURSWP        55  ///< A Sweep Surf          SurSwp
#define Typ_SURBSP        56  ///< A B_Spline_Surf       SurBSpl
#define Typ_SURRBSP       57  ///< A Rat.B_Spline Surf   SurRBSpl
#define Typ_SURHAT        60  ///< A Hatch

// trimmed, perforated surfaces:
#define Typ_SURPLN        58  ///< A Planar surf.
#define Typ_SURSUP        69  ///< A support-surface 
#define Typ_SUTP          59  ///< A surface-trimmed-perforated-supported

// pretesselated surfaces
#define Typ_SURCIR        61  ///< tesselated fan
#define Typ_SURSTRIP      62  ///< tesselated strip
#define Typ_SURBND        63  ///< A bounded surface
#define Typ_SURPMSH       64  ///< A Mesh from group of points
#define Typ_SURPTAB       65  ///< A boundary around group of points

//#define Typ_SURSPI       158  ///< Spine (Kontur + Verfahrweg; zB Spiralfl.)
//#define Typ_SURSK        153  ///< Skin

// tesselated surfaces
#define Typ_GL_Sur        70  ///< tesselated surface, bMsh
#define Typ_GL_PP         71  ///< tesselated planar patch
#define Typ_GL_CV         72  ///< Polygon
#define Typ_GL_iSur       73  ///< tesselated indexed-surface
#define Typ_EdgSur        74  ///< EdgSur see also IntTab
#define Typ_Fac3          75  ///< Triang; Form = Fac3
#define Typ_Tria          76  ///< Triangle form 3 points
#define Typ_GL_Trias      77  ///< GL_TRIANGLES
#define Typ_GL_Tria_fan   78  ///< GL_TRIANGLE_FAN
#define Typ_GL_Tria_strip 79  ///< GL_TRIANGLE_STRIP

/// DB-bodies 80-89
#define Typ_SOL           80  ///< B  body (CON, TOR, ..)
#define Typ_SPH           81  ///< B  Sphere
#define Typ_CON           82  ///< B  Conus (Cylinder)
#define Typ_TOR           83  ///< B  Torus
#define Typ_PRI           84  ///< B  Prism
#define Typ_BREP          85  ///< B  shell from connected faces


/// notes, images
#define Typ_Note          90     ///< GroupCode f Typ_ATXT|Typ_GTXT|Typ_Dimen..
#define Typ_ATXT          91     ///< N  AText (2D-text, tag, bitmap, symbol ..)
#define Typ_GTXT          92     ///< N  GText (grafic text)
#define Typ_Dimen         93     ///< N  Dimen (dimension)
#define Typ_Dim3          94     ///< N  Dim3  (3D-dimension)
#define Typ_Tag           95     ///< N Bitmap (Blocknote, Image ..)

#define Typ_Joint         96     ///< connection, exported;

/// symbols
#define Typ_SymB         100     ///< bitmap symbols: SYM_TRI_S SYM_STAR_S ..
#define Typ_SymV         101     ///< Vektorymbole: SYM_PLANE SYM_ARROW ..
#define Typ_SymRef       102  ///<     SymRef
#define Typ_SymRef2      103  ///<     SymRef2

#define Typ_Texture      105     ///< index to TexRef
#define Typ_TEXB         106     ///< Base-Texture    TexBas
#define Typ_TEXR         107     ///< Ref-Texture     TexRef

#define Typ_BBox         110     ///< bounding-box, 3D
#define Typ_BBox2        111     ///< bounding-box, 2D
#define Typ_GridBox      112     ///< symbolic box, GridBox
#define Typ_IntTab       113     ///< IntTab (../ut/ut_msh.h)

/// temp objs
#define Typ_TmpPT        115     ///< temporary Point
#define Typ_Vertex       116  ///< Vertex; point on existing object
#define Typ_EyePT        117     ///y eye-point
#define Typ_TmpGrp       118     ///< temporary Point   ??
// #define Typ_TmpLN        111     ///< temporary Line
// #define Typ_TmpVC        113     ///< temporary Vector
// #define Typ_TmpSym       114     ///< temporary Symbol
// #define Typ_TmpTRI       115     ///< temporary triangle
// #define Typ_TmpPLN       116     ///< temporary Plane



/// models, submodels
#define Typ_Part         120
#define Typ_Group        121  ///< U ObjDB DB-Objects (union-group)
#define Typ_SubModel     122  ///< M  - basicModel                      ModelBas
#define Typ_Model        123  ///< M  ModelReference of native-subModel ModelRef
#define Typ_Mock         124  ///< M  ModelReference of mockup-subModel
#define Typ_Ditto        125  ///< mockup-subModel (unvisible)
#define Typ_ModelNode    126  ///< model-hierarchy                       UNUSED
#define Typ_CtlgPart     127  ///  catalog-part

#define Typ_GEOB_2D      128  ///< geometric obj binary format 2D
#define Typ_GEOB_3D      129  ///< geometric obj binary format 3D


/// END OF DB-OBJECTS
/// geom. parameters  TYP_IS_GEOMPAR                       TypTxtTab130
#define Typ_Val          130  ///< double-Value
#define Typ_XVal         131  ///< X-value (X())
#define Typ_YVal         132  ///< Y-value (Y())
#define Typ_ZVal         133  ///< Z-value (Z())
#define Typ_Angle        134  ///< Angle (in degree; ANG())
#define Typ_Rad          135  ///< Radius   UU ?
#define Typ_PTS          136  // standard-point-Nr; (Ptyp_* eg Ptyp_start,Ptyp_end)
#define Typ_PTI          137  // index controlpoint for eg polygon, bspl, ..
#define Typ_SEG          138  // segment-Nr (of compound-objs, eg CCV)
#define Typ_Par1         139  ///< Parameter 0-1; for knotvals use Typ_Val


#define Typ_Dist         140  // unused
#define Typ_UVal         141  // unused
#define Typ_VVal         142  // unused
#define Typ_AngN         143  // unused
#define Typ_UU1          144  // unused
#define Typ_UU2          145  // unused
#define Typ_UU3          146  // unused



/// attributes
#define Typ_Typ          150  ///< form of ObjGX-Record defines Typ
#define Typ_Subtyp       151
#define Typ_cmdNCsub     152  ///  see enum Typ_Cmd1
#define Typ_Address      153  ///< form of ObjGX-Record defines dataPosition
#define Typ_Size         154  ///< recordSize for following record Typ_Int4
#define Typ_Index        155  ///< long (in pointer)
#define Typ_Name         156
#define Typ_Color        157  ///< ColRGB
#define Typ_Ltyp         158  ///< line type
#define Typ_Thick        159  ///< line thickness
#define Typ_G_Att        160  ///< G
#define Typ_Activ        161  ///< I = Interactivity
#define Typ_Layer        162

#define Typ_APPOBJ       163  /// Typ_apDat 
#define Typ_apDat        164  ///< application-data Typ_APPOBJ; invisible;
#define Typ_dynSym       165  /// dynamic obj without DB-obj
#define Typ_PRCV         166  ///< polygonal representation curve
#define Typ_constrPln     167  ///< construction-plane (2D-plane)
#define Typ_Process      168  ///  Process (NC ..)


/// dataFormats (Int, matrix, Polynom..)
#define Typ_Ptr          170   ///< binary data (pointer, form of Typ_Address)
#define Typ_Int1         171   ///<  8 bit (char)
#define Typ_Int2         172   ///< 16 bit (short)
#define Typ_Int4         173   ///< 32 bit (int, long)
#define Typ_Int8         174   ///< 64 bit (long)
#define Typ_Float4       175   ///< 32 bit (float)
#define Typ_Float8       176   ///< 64 bit (double)


#define Typ_Polynom_     180
#define Typ_Polynom1     181   ///< Polynom1
#define Typ_Polynom3     182   ///< Polynom3
#define Typ_polynom_d3   183
#define Typ_WPT          184   ///< wPoint (weighted point)

#define Typ_M3x2         185   ///< Mat_3x2
#define Typ_M3x3         186   ///< Mat_3x3
#define Typ_M4x3         187   ///< Mat_4x3
#define Typ_M4x4         188   ///< Mat_4x4


/// text
#define Typ_Txt          190   /// $  ohne ", normal mit , begrenzt f. $20=
#define Typ_String       191   /// String = text enclosed between two " - eg "abc"
#define Typ_StrDel       192   /// StringDelimiter "\" Fortsetzungszeile 
#define Typ_StrDel1      193   /// StringDelimiter 1 "|"
#define Typ_FncNam       194   /// geom-function; see ObjCodTab Typ_Cmd1
#define TYP_FilNam       195   /// filename
#define Typ_NumString    196   /// numeric string; eg -123.4
#define Typ_ConstVal     197   /// constant value; eg PI RAD_90 SR_2
#define Typ_ConstOG      198   /// geometric-constant-object; eg DX RZ


/// containers
#define Typ_Memspc       200   ///< Memspc
#define Typ_MemTab       201   ///< MemTab   ../ut/ut_memTab.h
#define Typ_IndTab       202   ///< IndTab, container
#define Typ_IgaTab       203   ///< IndTab, container
// #define Typ_ObjRange     204   ///< ObjRange     REPLACED WITH IgaTab
#define Typ_ObjSRC       204   ///< ObjSRC
// #define Typ_ObjTXTSRC    206   ///  expression of sourceObject
#define Typ_ObjGX        205   ///< ObjGX,  container
#define Typ_ObjTab       206   ///< ObjTab, container         UU
#define Typ_ObjBin       207   ///< ObjBin  binary-object             
#define Typ_ObjDB        208   ///< ObjDB
#define Typ_ObjAto       209   ///< ObjAto


/// transformations
#define Typ_Tra          210   ///< all transformations
#define Typ_TraTra       211   ///< Translation dzt unused; Typ_VC used
#define Typ_TraRot       212   ///< Rotation TraRot
// #define Typ_TraMat       213   ///< Matrixtransformation


/// operators
#define Typ_ope__        220  ///< operator =
#define Typ_ope_eq       221  ///< operator =
#define Typ_ope_ne       222  ///< operator !=
#define Typ_ope_lt       223  ///< operator <
#define Typ_ope_gt       224  ///< operator >
#define Typ_ope_ge       225  ///< operator >=
#define Typ_ope_le       226  ///< operator <=
#define Typ_ope_and      227  ///< operator &
#define Typ_ope_or       228  ///< operator |


/// modifiers  TYP_IS_MOD
#define Typ_modif        229  ///< Modifier  MOD
#define Typ_modRepl      230  ///< Replace (default is create, add) REPL T_REPL
#define Typ_mod1         231  ///< Modifier; Value depends
#define Typ_mod2         232  ///< Modifier; Value depends
#define Typ_modCWCCW     233  ///< Modifier; CW CCW
#define Typ_modREV       234  ///< Modifier; Reverse
#define Typ_modCX        235  ///< Modifier ACROSS (V-direction)
#define Typ_modCTRL      236
#define Typ_modPERP      237  ///< perpendicular (right angled; default = parall)
#define Typ_modPARL      238  ///< parallel (U-direction)
#define Typ_modUnlim     239  ///< "UNL|UNL1|UNL2"
#define Typ_modLOX       240
#define Typ_modHIY       241
#define Typ_modLOY       242
#define Typ_modHIZ       243
#define Typ_modLOZ       244
#define Typ_modCCW       245
#define Typ_modCW        246
#define Typ_modIN        247
#define Typ_modOUT       248
#define Typ_modAux       249  ///< Modifier; on|off; text=last infoWord
#define Typ_modHIX       250  // X-vec of plane
#define Typ_modUndef     259  ///< undefined



// /// events  DO NOT USE - replaced by TYP_Event* ../gui/gui_types.h
// #define GUI_MouseL     250  ///< event left mousebutton
// #define GUI_MouseM     251  ///< event mid mousebutton
// #define GUI_MouseR     252  ///< event right mousebutton
// #define GUI_Mouse2L    253  ///< event doubleclick


/// functions  see also UI_Func* ../gui/gui_types.h
/// check: TYP_IS_FNC()
#define TYP_FuncInit     260  ///< function init
#define TYP_FuncInit1    261  ///< function 
#define TYP_FuncMod      262  ///< function modify
#define TYP_FuncAdd      263  ///< function add
#define TYP_FuncDel      264  ///< function delete
#define TYP_FuncEnd      265  ///< function end
#define TYP_FuncExit     266  ///< function exit



/// function parameters
#define Typ_ALL          270
#define Typ_Done         271
#define Typ_NULL         272  ///< nothing, empty;
#define Typ_last         273  ///< 
#define TYP_SIZ          274  ///< nr of defined types
#define Typ_unknown      275  ///<


/// math.operators
#define TYP_OpmPlus      280  ///<
#define TYP_OpmMinus     281  ///< 
#define TYP_OpmMult      282  ///<
#define TYP_OpmDiv       283  ///<
#define TYP_OpmPow       284  ///<

/// math.functions
#define Typ_FcmSQRT      290
#define Typ_FcmSIN       291
#define Typ_FcmCOS       292
#define Typ_FcmTAN       293
#define Typ_FcmASIN      294
#define Typ_FcmACOS      295
#define Typ_FcmATAN      296
#define Typ_FcmABS       297
#define Typ_FcmFIX       298
#define Typ_FcmRND       299

#define TYPE_STRU_NR     300  ///<   nr of obj-struct-types

// ../gui/gui_types.h     400-499      TYP_Event* TYP_Device* TYP_GUI_*

// ../xa/xa_sele.h       1000-1099    Typ_go* Typ_Fnc*


//----------------------------------------------------------------
#define BCKPLN_XY          2  ///< Z-Vec BCKVEC_Z  parall. X-Y
#define BCKPLN_XZ          1  ///< Y-Vec BCKVEC_Y  parall. X-Z (rot 90 deg around X)
#define BCKPLN_YZ          0  ///< X-Vec BCKVEC_X  parall. Y-Z (rot 90 deg around Z)
#define BCKPLN_FREE       -1  ///< free-3D-plane; use matrix-transformations
#define BCKPLN_UNDEF      -2  ///< undefined

#define BCKVEC_Z           2  ///< Z-Vec BCKPLN_XY
#define BCKVEC_Y           1  ///< Y-Vec BCKPLN_XZ
#define BCKVEC_X           0  ///< X-Vec BCKPLN_YZ


#define GL_FAC_PLANAR     16  ///< planar fan (GL_TRIANGLE_FAN = planar)



// AP_iftyp_ftyp
#define Mtyp_Gcad       0    // extern-native

#define Mtyp_DXF        1    // native-import; 1 - 9;
#define Mtyp_Iges       2
#define Mtyp_Step       3
#define Mtyp_XML        4

#define Mtyp_3DS        5
#define Mtyp_LWO        6
#define Mtyp_SVG        7    // only export

#define Mtyp_TESS      10    // gcad-tesselated-data - 10 - 19
#define Mtyp_WRL       11    // VRML1 
#define Mtyp_WRL2      12    // VRML2
#define Mtyp_OBJ       13
#define Mtyp_STL       14

#define Mtyp_BMP       20    // .bmp       20-29 PixelImages
#define Mtyp_JPG       21    // .jpg


// basicModeltypes
#define MBTYP_EXTERN    0      // extern-native
#define MBTYP_INTERN   -1
#define MBTYP_CATALOG  -2      // .ctlg
#define MBTYP_APPLI    -3      // .gcap
#define MBTYP_DUP      -4      // duplicate model
#define MBTYP_UNDEF    -5
#define MBTYP_ERROR    -6


// 3D-mode or 2D-mode
#define Typ_geom_3D    0
#define Typ_geom_2D    1


// UT3D_ptvc_obj UTO_2pt_limstru UTO_ptnr_std_obj UT3D_ptvcpar_std_obj
#define Ptyp_def        0    // default
#define Ptyp_start      1    // startpoint (trimmed)
#define Ptyp_end        2    // endpoint   (trimmed)
#define Ptyp_mid        3    // midpoint          1     Circ, elli: 180-deg-point
#define Ptyp_cen        4    // centerpoint       1     Circ, elli, plane
#define Ptyp_90_deg     5    // 25 % point        1     Circ, elli: 90-deg-point
#define Ptyp_270_deg    6    // 75 % point        1     Circ, elli: 270-deg-point
#define Ptyp_focus1     7    // focus points      1|2   Elli, hyp ..
#define Ptyp_focus2     8    // focus points      1|2   Elli, hyp ..
#define Ptyp_start_abs  9    // startpoint (untrimmed)
#define Ptyp_end_abs   10    // endpoint   (untrimmed)


#define EditMode_add     0
#define EditMode_insert  1


// #define Dtyp_Z        0
// #define Dtyp_X        1
// #define Dtyp_Y        2


#ifndef TimeStamp
#define TimeStamp  float
// typedef float TimeStamp;
#endif


#define OBJSTAT_undef     -1
#define OBJSTAT_perm       0   // APT_obj_stat lifespan
#define OBJSTAT_temp       1

#define MDL_BMI_ACT       -1   // index baseModel of active primary model

#define MDLSTAT_empty      0
#define MDLSTAT_loading    1
#define MDLSTAT_loaded     2
#define MDLSTAT_save_as    3
#define MDLSTAT_save_quick 4
#define MDLSTAT_save_exit  5


#define BRWSTAT_OFF        0
#define BRWSTAT_init       1   // do not process callBacks (rowSelect ..)
#define BRWSTAT_active     2


// EOF
