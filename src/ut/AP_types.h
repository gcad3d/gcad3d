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
*//*!
\file ../ut/AP_types.h
\ingroup grp_ut_geom
*/


//----------------------------------------------------------------
/// types: nr = TYP_SIZ.    See also xa_sele.h   typText: TypTxtTab0
/// \code
///   01 -  19  DB-primitives (V.D,P,L,C)
///   20 -  39  DB-curves (S)
///   40 -  79  surfs (A)
///   80 -  89  bodies (B)
///   90 - 109  notes, images
///  110 - 119  temp objs
///  120 - 129  geom. attributes
///  130 - 149  geom. parameters
///  150 - 169  attributes
///  170 - 189  dataFormats (Int, matrix, Polynom..)
///  190 - 199  Text
///  200 - 209  containers
///  210 - 219  transformations
///  220 - 229  operators
///  230 - 249  modifiers
///  250 - 259  events
///  260 - 269  functions
///  270 - 279  function parameters
///  400 - 499  ../gui/gui_types.h      GUI-types TYP_Event* TYP_Device* TYP_GUI_*
/// 1000 -1099  ../xa/xa_sele.h         selection-modfiers
///
/// Get infoText for types: AP_src_typ__ ();
///
/// SEE (UPDATE) ALSO TypTxtTab** in ../ut/ut_gtypes.c
/// \endcode



#define Typ_ALL_OBJS      -1
#define Typ_Error          0  ///<     U0 = undefined

/// DB-primitives 1-19
#define Typ_VAR            1  ///< V   numer. variable
#define Typ_VC             2  ///< D   Vector
#define Typ_PT             3  ///< P   Point
#define Typ_LN             4  ///< L   Line
#define Typ_CI             5  ///< C   Circ
#define Typ_VC2           10  ///< D   Vector2
#define Typ_PT2           11  ///< P   Point2
#define Typ_LN2           12  ///< L   Line2
#define Typ_CI2           13  ///< C   Circ2
#define Typ_CI2C          14  ///< C   Circ2C centerPosition

/// DB-curves 20-39
#define Typ_CV            20  ///< S   all curves
#define Typ_CVPOL         21  ///< S   CurvPoly
#define Typ_CVPSP3        22  ///< S   polynom_d3
#define Typ_CVBSP         23  ///< S   CurvBSpl
#define Typ_CVRBSP        24  ///< S   CurvRBSpl
#define Typ_CVCCV         25  ///< S   CurvCCV
#define Typ_CVELL         26  ///< S   CurvElli
#define Typ_CVBEZ         27  ///< S   CurvBez
#define Typ_CVRBEZ        28  ///< S   CurvRBez
#define Typ_CVCLOT        29  ///< S   ClothoidCurve  (CurvClot)
#define Typ_CVLNA         30  ///< S   array of lines (form=Typ_LN)
#define Typ_CV2           31  ///< S   all 2D-curves
#define Typ_CVPOL2        32  ///< S   CurvPoly-2D
#define Typ_CVPSP2        33  ///< S   polynom_?
#define Typ_CVBSP2        34  ///< S   CurvBSpl2
#define Typ_CVRBSP2       35  ///< S   CurvRBSpl-2D
#define Typ_CVCCV2        36  ///< S   CCV-2D
#define Typ_CVELL2        37  ///< S   CurvEll2
#define Typ_CVELL2C       38  ///< S   CurvEll2C centerPosition
#define Typ_CVBEZ2        39  ///< S   CurvBez2

/// DB-surfs 40-79
#define Typ_PLN           40  ///< R  plane/Refsys
#define Typ_SUR           50  ///< A all surfaces (support-surface, bounded)
// SupportSurfaces (nicht getrimmt,gelocht):
#define Typ_SURCON        51  ///< Conus (unused)
#define Typ_SURTOR        52  ///< Torus (unused)
#define Typ_SURRU         53  ///< A Ruled Surf
#define Typ_SURRV         54  ///< A Revolved Surf
#define Typ_SURSWP        55  ///< A Sweep Surf          SurSwp
#define Typ_SURBSP        56  ///< A B_Spline_Surf       SurBSpl
#define Typ_SURRBSP       57  ///< A Rat.B_Spline Surf   SurRBSpl

// trimmed, perforated surfaces:
#define Typ_SURPLN        58  ///< A Planar surf.
#define Typ_SURTPS        59  ///< A surface-trimmed-perforated-supported
#define Typ_SURHAT        60  ///< A Hatch

// pretesselated surfaces
#define Typ_SURCIR        61  ///< tesselated fan
#define Typ_SURSTRIP      62  ///< tesselated strip

#define Typ_SURBND        63  ///< A bounded surface
#define Typ_SURMSH        64  ///< A Mesh
#define Typ_SURPTAB       65  ///< A surf from PTAB

#define Typ_Fac3          66  ///< Triangle; Form = Fac3
#define Typ_QFac          67  ///< Triangle; 
#define Typ_Tria          68  ///< Triangle; Form = Triangle  (unused)

// tesselated surfaces
#define Typ_GL_Sur        70  ///< tesselated surface
#define Typ_GL_PP         71  ///< tesselated planar patch
#define Typ_GL_CV         72  ///< Polygon
#define Typ_GL_iSur       73  ///< tesselated indexed-surface
#define Typ_EdgSur        74  ///< EdgSur see also EdgeLine

//#define Typ_SURSPI       158  ///< Spine (Kontur + Verfahrweg; zB Spiralfl.)
//#define Typ_SURSK        153  ///< Skin


/// DB-bodies 80-89
#define Typ_SOL           80  ///< B  body (CON, TOR, ..)
#define Typ_SPH           81  ///< B  Sphere
#define Typ_CON           82  ///< B  Conus (Cylinder)
#define Typ_TOR           83  ///< B  Torus
#define Typ_PRI           84  ///< B  Prism
#define Typ_BREP          85  ///< B  shell from connected faces


/// notes, images
#define Typ_Note          90     ///< GroupCode f Typ_ATXT|Typ_GTXT|Typ_Dimen..
#define Typ_ATXT          91     ///< N  AText
#define Typ_GTXT          92     ///< N  GText
#define Typ_Dimen         93     ///< N  
#define Typ_Dim3          94     ///< N
#define Typ_Tag           95     ///< N Bitmap (Blocknote, Image ..)

/// symbols
#define Typ_SymB         100     ///< bitmap symbols: SYM_TRI_S SYM_STAR_S ..
#define Typ_SymV         101     ///< Vektorymbole: SYM_PLANE SYM_ARROW ..
#define Typ_SymRef       102  ///<     SymRef
#define Typ_SymRef2      103  ///<     SymRef2
#define Typ_Texture      104     ///< index to TexRef
#define Typ_TEXB         105     ///< Base-Texture    TexBas
#define Typ_TEXR         106     ///< Ref-Texture     TexRef
#define Typ_BoxH         107     ///< symbolic box
#define Typ_EdgeLine     108     ///< EdgeLine (../ut/ut_msh.h)

/// temp objs
#define Typ_TmpPT        110     ///< temporary Point
#define Typ_Vertex       111  ///< Vertex; point on existing object
#define Typ_EyePT        112     ///y eye-point
// #define Typ_TmpLN        111     ///< temporary Line
// #define Typ_TmpVC        113     ///< temporary Vector
// #define Typ_TmpSym       114     ///< temporary Symbol
// #define Typ_TmpTRI       115     ///< temporary triangle
// #define Typ_TmpPLN       116     ///< temporary Plane



/// models, submodels
#define Typ_Part         120
#define Typ_Group        121  ///< U ObjDB DB-Objects (union-group)
#define Typ_SubModel     122  ///< M  - basicModel
#define Typ_Model        123  ///< M  ModelReference of native-subModel ModelRef
#define Typ_Mock         124  ///< M  ModelReference of mockup-subModel
#define Typ_Ditto        125  ///< mockup-subModel (unvisible)
#define Typ_Joint        126  ///< connection, exported;
#define Typ_Process      127  


/// geom. parameters  TYP_IS_GEOMPAR
#define Typ_Val          130  ///< double-Value
#define Typ_ValX         131  ///< X-coord
#define Typ_ValY         132  ///< Y-coord
#define Typ_ValZ         133  ///< Z-coord
#define Typ_XVal         134  ///< X-distance
#define Typ_YVal         135  ///< Y-distance
#define Typ_ZVal         136  ///< Z-distance
#define Typ_Angle        137  ///< Angle (in degree)
#define Typ_Rad          138  ///< Radius
#define Typ_Dist         139  ///< distance
#define Typ_Par1         140  ///< Parameter 0-1; for knotvals use Typ_Val
#define Typ_UVal         141
#define Typ_VVal         142
#define Typ_AngN         143
#define Typ_AngP         144
#define Typ_RadISO       145
#define Typ_Conic        146
// #define Typ_Side         121  ///< hi, lo, le or ri
// #define Typ_Side1        122  ///< in or out
// #define Typ_DirS         123  ///< X / Y / Z / P


/// attributes
#define Typ_Typ          150  ///< form of ObjGX-Record defines Typ
#define Typ_Subtyp       151
#define Typ_cmdNCsub     152  ///  see enum Typ_Cmd1
#define Typ_Address      153  ///< form of ObjGX-Record defines dataPosition
#define Typ_Size         154  ///< recordSize for following record
#define Typ_Index        155  ///< int or long
#define Typ_Name         156
#define Typ_Color        157  ///< ColRGB
#define Typ_Ltyp         158  ///< line type
#define Typ_Thick        159  ///< line thickness
#define Typ_G_Att        160  ///< G
#define Typ_Activ        161  ///< I = Interactivity
#define Typ_Layer        162

#define Typ_APPOBJ       163  /// Typ_apDat 
#define Typ_apDat        164  ///< application-data Typ_APPOBJ
#define Typ_Tool         165


/// dataFormats (Int, matrix, Polynom..)
#define Typ_Data         170   ///< binary data
#define Typ_Int1         171   ///<  8 bit (char)
#define Typ_Int2         172   ///< 16 bit (short)
#define Typ_Int4         173   ///< 32 bit (int, long)
#define Typ_Float4       174   ///< 32 bit (float)
#define Typ_Float8       175   ///< 64 bit (double)
#define Typ_IndTab       176   ///< 32-bit-int-list (struct Int4ITab)

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
#define Typ_Txt          190     ///< $  ohne ", normal mit , begrenzt f. $20=
#define Typ_String       191     ///< durch " begrenzt
#define Typ_StrDel       192     ///< StringDelimiter "\" Fortsetzungszeile 
#define Typ_StrDel1      193     ///< StringDelimiter 1 "|"
#define Typ_FncNam       194
#define TYP_FilNam       195  ///< filename
#define Typ_CtlgPart     196  ///< catalogpart
#define Typ_NumString    197  /// numeric string; eg -123.4
#define Typ_ConstVal     198  /// constant value; eg PI RAD_90 SR_2
#define Typ_ConstOG      199  /// geometric-constant-object; eg DX RZ


/// containers
#define Typ_Memspc       200   ///< Memspc
#define Typ_MemTab       201   ///< MemTab   ../ut/ut_memTab.h

#define Typ_ObjRange     202   ///< ObjRange
#define Typ_ObjSRC       203   ///< ObjSRC
#define Typ_ObjGX        204   ///< ObjGX
#define Typ_ObjG2        205   ///< deprec
#define Typ_ObjG         206   ///< deprec
#define Typ_ObjDB        207   ///< ObjDB


/// transformations
#define Typ_Tra          210  ///< all transformations
#define Typ_TraTra       211  ///< Translation dzt unused; Typ_VC used
#define Typ_TraRot       212  ///< Rotation TraRot
#define Typ_TraMat       213  ///< Matrixtransformation


/// operators
#define Typ_ope_eq       220  ///< operator =
#define Typ_ope_ne       221  ///< operator !=
#define Typ_ope_lt       222  ///< operator <
#define Typ_ope_gt       223  ///< operator >
#define Typ_ope_ge       224  ///< operator >=
#define Typ_ope_le       225  ///< operator <=
#define Typ_ope_and      226  ///< operator &
#define Typ_ope_or       227  ///< operator |


/// modifiers  TYP_IS_MOD
#define Typ_modif        230  ///< Modifier  MOD
#define Typ_mod1         231  ///< Modifier; Value depends
#define Typ_mod2         232  ///< Modifier; Value depends
#define Typ_modCWCCW     233  ///< Modifier; CW CCW
#define Typ_modREV       234  ///< Modifier; Reverse
#define Typ_modCX        235  ///< Modifier ACROSS (V-direction)
#define Typ_modCTRL      236
#define Typ_modPERP      237  ///< perpendicular (right angled; default = parall)
#define Typ_modPARL      238  ///< parallel (U-direction)
#define Typ_modHIX       239
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
#define Typ_modUnlim     250  ///< "UNLIM" or not

// /// events  DO NOT USE - replaced by TYP_Event* ../gui/gui_types.h
// #define GUI_MouseL     250  ///< event left mousebutton
// #define GUI_MouseM     251  ///< event mid mousebutton
// #define GUI_MouseR     252  ///< event right mousebutton
// #define GUI_Mouse2L    253  ///< event doubleclick


/// functions  see also UI_Func* ../gui/gui_types.h
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
#define Typ_NULL         272  ///< nothing, empty; Typ_unknown
#define Typ_last         273  ///< 
#define TYP_SIZ          274  ///< nr of defined types


/// math.operators
#define TYP_OpmPlus      280  ///<
#define TYP_OpmMinus     281  ///< 
#define TYP_OpmMult      282  ///<
#define TYP_OpmDiv       283  ///<

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


// ../gui/gui_types.h     400-499      TYP_Event* TYP_Device* TYP_GUI_*

// ../xa/xa_sele.h       1000-1099    Typ_go* Typ_Fnc*


//----------------------------------------------------------------
#define BCKPLN_XY          2  ///< Z-Vec BCKVEC_Z
#define BCKPLN_XZ          1  ///< Y-Vec BCKVEC_Y 
#define BCKPLN_YZ          0  ///< X-Vec BCKVEC_X

#define BCKVEC_Z           2  ///< Z-Vec BCKPLN_XY
#define BCKVEC_Y           1  ///< Y-Vec BCKPLN_XZ
#define BCKVEC_X           0  ///< X-Vec BCKPLN_YZ


#define GL_FAC_PLANAR     16  ///< planar fan (GL_TRIANGLE_FAN = planar)



// AP_ck_ftyp
#define Mtyp_Gcad       0
#define Mtyp_DXF        1
#define Mtyp_Iges       2
#define Mtyp_Step       3
#define Mtyp_3DS        4
#define Mtyp_LWO        5
#define Mtyp_XML        6
#define Mtyp_WRL       10    ///< VRML1      10-19  tess-Formate
#define Mtyp_WRL2     110    ///< VRML2
#define Mtyp_OBJ       11
#define Mtyp_STL       12
#define Mtyp_TESS      13
#define Mtyp_BMP       20    ///< 20-29 PixelImages
#define Mtyp_JPG       21


// basicModeltypes
#define MBTYP_EXTERN    0
#define MBTYP_INTERN   -1
#define MBTYP_CATALOG  -2


#ifndef TimeStamp
#define TimeStamp  float
// typedef float TimeStamp;
#endif

// EOF
