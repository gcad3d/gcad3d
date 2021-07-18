/* ../inf/Attributes.c


================================================================== */
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
UP: INF_permanent_attributes


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
UP: INF_permanent_attributes


- for surfaces (A) and solids (B)
- struct ColRGB type is Typ_Color
- source ../ut/ut_col.c and ../ut/ut_col.h
- ColRGB is used as INT32 for GL-functions (COL_INT32 / INT32P_COL)


================================================================== \endcode */}
void INF_Texture_attributes (){        /*! code
UP: INF_permanent_attributes


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



M3_surf_texture:
TED_Tex_Load
  Tex_addBas1
    Tex_getBitmap


Modify_SurfaceTexture:
UI_Tex__  UI_FuncUCB3
  UI_Tex_Load
    Tex_addBas1
      Tex_getBitmap
      Tex_getBas1
      GL_Tex_Load



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
void INF_refModels_DB (){        /*! code

DB-list mdr_tab[APT_MR_IND] is the ModelReferenceList.
The mdr_tab-index is the Model-dbi.

Get the ModelRef for subModel M20 with: DB_get_ModRef (20L);



// Typ_Model
typedef struct {char *mnam; int modNr; double scl;
                Point po; Vector vx, vz;}                           ModelRef;
// modNr .. modelnumber of ModelBas-obj. (DB_mbi_mRefID())
// po    .. position of ditto


../db/ut_DB.c:
static ModelRef   *mdr_tab;
long APT_MR_IND


================================================================== \endcode */}
void INF_basModels_DB (){        /*! code

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
void INF_Typ_Color (){        /*! code

struct ColRGB  Typ_Color

Colors for surfaces:
- are stored in the PermanentAttributeList (ObjAtt)GA_ObjTab (../xa/xa_ga.c)
  in (ObjAtt)GA_ObjTab[i].iatt

Color is only active/valid if (ColRGB)col.color = 1.

R-value (float) from ColRGB: (float)rVal = <col>.cr; rVal /= 255;



Functions:
COL_DL_ATT         get color out of struct DL_Att
                   r-g-b- values from ColRGB


Files:
../ut/ut_col.h      COL_DL_ATT INT32P_COL ..
../xa/xa.c          APcol_* COL_* UTcol_* 


================================================================== \endcode */}
void INF_joints (){        /*! code
TODO: myReadme.joints

Sourcefiles:
../xa/xa_joint.c
../ut/ut_dbf.c            store/retrieve joint-objects with key;
../../doc/html/CAD_Joint_en.htm           user-doc.

DB only DB_JNT_IND = nr of used joints / index of next free.
Joints are stored in file <tmp>/joints, not in DB.


//----------------------------------------------------------------
Functions:
APT_decode_Joint
JNT_exp__             export joints into file "<tmp>/joints"
JNT_imp__             import
APT_IMP__

DBF_init
DBF_add__        store binary obj joint
DBF_find__       find dataPosition from key
DBF_getVal       get binary obj joint
- like basicModels (mdb_dyn,mdb_nam) joints cannot be unloaded
  with the DB of the active model.


Temp-file:
<tmp>/joints       binary,     see with wxHexEditor




//----------------------------------------------------------------
Models:
Data/test_joint_1.gcad   tst_jnt1.gcad my_jnt2.gcad
Data/sample_DiagEl1.gcad


//----------------------------------------------------------------
TODO:
- joint-object should be a struct: {(int)size-(ObjGX)obj-objData}
  all pointers must be inside the struct.
- export-import Plane
- export-import Text does not work at the moment


================================================================== \endcode */}
// eof
