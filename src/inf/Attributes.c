/* ../inf/Attributes.c
../../doc/gcad_doxygen/Attributes.dox


================================================================== */
void INF_GEOM_ATTRIB(){                   /*! \code

INF_COL_CV      list of colors for curves  (attribute curves)

INF_Hilite_attribute
INF_Hide_attribute
INF_Pick_attribute
INF_Linetyp_attributes
INF_Color_attributes
INF_Texture_attributes
INF_permanent_attributes

Analysieren: see DL_dlRec__dli
  // Jede Aenderung der Graf. Attribute wird erst nach dem Neuzeichnen
  // der gesamten Displayliste sichtbar:
  DL_Redraw ();


================================================================== \endcode */}
void INF_Hilite_attribute (){        /*! code


// hilite single Object (not ApplicationObjects)
DL_hili_on ((long)DispListIndex);
// unhilite single Object
DL_hili_off ((long)DispListIndex);
// unhilite all Objects
DL_hili_off (-1L);


================================================================== \endcode */}
void INF_Hide_attribute (){        /*! code


// change hidden/visible for single obj; ON=0 OFF=1
DL_hide__ ((long)DispListIndex, OFF);


================================================================== \endcode */}
void INF_Pick_attribute (){        /*! code


// Set to not pickable:
DL_pick_set ((long)DispListIndex, ON);
// Abfragen ob pickable:
i1 = DL_GetPick((long)DispListIndex); // OFF = ja, ON = Nein.


================================================================== \endcode */}
void INF_Linetyp_attributes (){        /*! code


- for lines (L) circles (C) and curves (S)
- struct Ind_Att_ln type is Typ_Ltyp
- struct Att_ln type is Typ_G_Att
- source .. ?
- Ind_Att_ln is used as INT32 for GL-functions (TODO)
Defaultwerte siehe Hauptdoku "Farben, Linientypen, Strichstärken".
// Modify or create new line-Attribute:
DL_InitAttRec ((int)AttributIndex,(int)color,(int)linetyp,(int)thick);
// colour 3 digits; red, green, blue.
// 900 = red, 090 = green, 009 = blue;
// 000 = black; 999 = white, 990 = yellow ..
// lineTyp: 0 = full-line (VollLinie); 1 = dash-dot (Strich-Punkt),
// 2 = dashed (kurz strichliert) 3 = dashed-long (lang strichliert),
// lineThick: 1-6, thickness in pixels
// Defaults see File ~/gCAD3D/cfg/ltyp.rc
// - readonly <cfgdir>/ltyp.rc
// Example: Attribut 20 = green, dash, thickness 3:
DL_InitAttRec (20, 90, 2, 3);
// use this att for line:
Line ln1={{0., 0., 0.},{10., 5., 0.}};
GR_Disp_ln (&ln1, 20);
// restore defaultRecords from file <cfgdir>/ltyp.rc
DL_InitAttTab ();


================================================================== \endcode */}
void INF_Color_attributes (){        /*! code


- for surfaces (A) and solids (B)
- struct ColRGB type is Typ_Color
- source ../ut/ut_col.c and ../ut/ut_col.h
- ColRGB is used as INT32 for GL-functions (COL_INT32 / INT32_COL)


================================================================== \endcode */}
void INF_Texture_attributes (){        /*! code


- DisplayList GR_ObjTab - struct DL_Att.iatt keeps ColRGB for obj
- PermanentAttribute-Table GA_ObjTab - struct ObjAtt.iatt keeps ColRGB for obj
- struct ColRGB.vtex keeps Info Color or Texture (yes if .vtex = 1)
- struct ColRGB.cr .cg .cb keeps textureRefNr (index into TexRefTab)
- List *TexRefTab - struct TexRef.ibas keeps Base-Texure (index into TexBasTab)
- List *TexBasTab - struct TexBas keeps basic texture-data.
Global Vars:
GL_actTex
Functions:
GA_hasTexture()
GA_getTexRefInd()
Tex_getRef()
Tex_getBas__()
GRU_tex_rec // find rectangle for texture in modelspace
GL_Tex_Ini
See:
INF_TextureReferenceList
INF_BasicTexureList


================================================================== \endcode */}
void INF_permanent_attributes (){        /*! code


In the Modelfile:
# example: surface A20 has color e2dc27; surf. A21 is symbolic (not shaded)
GATAB
A20 Ce2dc27
A21 S1
ENDGATAB
See also: DynamicDataArea
GA_ObjTab ist the permanent-attributes-table (../xa/xa_ga.c).
Functions:
Color:
APcol_actColTra // set active col.
GA_Col__ // modify/reset/set-new color



================================================================== \endcode */}
void INF_TextureReferenceList (){        /*! code

// Typ_TEXR
typedef struct {long ibas; float uscx, uscy, udx, udy, uar;
                float scx, scy, dx, dy; Vector vx, vy;}             TexRef;
// ibas         index of the Base-Texure
// uscx uscy    User-defined-scale in X- and Y-direction;
// udx  udy     User-defined-offset of texture; 0-1
// uar          User-defined-rotationAngle for texture in degree
// scx scy      scale in X- and Y-direction;
// dx, dy       offset of texture; 0-1
// vx, vy       X- and Y-vector of textureplane


../xa/xa_tex.c:
static TexRef *TexRefTab;
static int    TexRefNr;           // next free index

Ist ebenfalls permanent wie die PermanentAttributeList und die BasicTexureList;
beim Exit wird die GATAB daraus generiert.



================================================================== \endcode */}
void INF_BasicTexureList (){        /*! code

// Typ_TEXB
typedef struct {long dli; char *fnam;
                short xSiz, ySiz, texNr, keep;}                     TexBas;
// dli          DispListindex of the Texture
// texNr        (OpenGL-) TextureNr
// keep         0=do not keep texture; 1=keep.
// fnam         Filename (symbolicPath/Filename) of the Texturefile
// xSiz, ySiz   size of the Texure in pixels



../xa/xa_tex.c:
static TexBas *TexBasTab;
static int    TexBasNr;           // next free index



================================================================== \endcode */}
void INF_ModelReferenceList (){        /*! code

// Typ_Model
typedef struct {char *mnam; int modNr; double scl;
                Point po; Vector vx, vz;}                           ModelRef;
// modNr .. modelnumber of ModelBas-obj. (DB_get_ModNr())
// po    .. position of ditto


../db/ut_DB.c:
static ModelRef   *mdr_tab;
long APT_MR_IND


================================================================== \endcode */}
void INF_BasicModelList (){        /*! code

../ut/ut_geo.h
/// mnam  .. Modelname
/// po    .. origin
/// pb1,pb2  boxPoints
/// DLind .. ind of first obj in DL
/// DLsiz .. nr of objects in DL
/// typ   .. MBTYP_[EXTERN|INTERN|CATALOG] for native-subModels
///          Mtyp_[WRL|WRL2|OBJ|STL|TESS]  for mockup-subModels
/// \endcode
typedef struct {char *mnam; Point po, pb1, pb2; long DLind, DLsiz;
                short typ, seqNr;}                                  ModelBas;


../db/ut_DB.c:
static ModelBas   *mdb_dyn;     the list of basicModel
static long DYN_MB_IND          the nr of basicModels

Functions:
  DB_StoreModBas      store basicModel
  DB_get_ModBas       get the basicModel





================================================================== \endcode */}
// eof
