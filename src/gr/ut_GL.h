/*


Korr:
2001-06-08 Taginfos zu.


=======================================================================*/
#define GR_MODE_DRAW       0   // display according DL
#define GR_MODE_SELECT     1
#define GR_MODE_FEEDBACK   2
#define GR_MODE_NORMAL     3   // do NOT display hilites
#define GR_MODE_PRINT1     4
#define GR_MODE_PRINT2     5
#define GR_MODE_2DSELECT   6

#define GR_STATE_WIRE      0
#define GR_STATE_SHADE     1
#define GR_STATE_HID1      2
#define GR_STATE_HID2      3
#define GR_STATE_HID3      4



//----------------------------------------------------------------
#define DL_base_font1    32   // End of tempObj-range (1-DL_base_font1)
                              // start of Bitmapfont (alfatext)
#define DL_Ind_ScBack   129   // index of the scaleback-record (glPopMatrix)
                              // 130-138 Bitmap-Symbols
                              // 139     Vector
                              // 140-149 Vector-Symbols (scaled)
#define DL_base_LnAtt   150   // LineAttributes until DL_base_PtAtt (max 45 types)
#define DL_base_PtAtt   195   // PointAttributes until DL_base_font2 (4 types used)
#define DL_base_font2   200   // fontObjs until DL_Ind_Scl2D (200=Blank=Ascii 32)
#define DL_Ind_Scl2D    297   // DL_Set_Scl2D 
#define DL_Ind_Cen2D    298   // set to 2D-screenCenter  DL_Set_Cen2D
#define DL_Img_ScBack   299   // index for scale images (DL_Img_ScBack)
#define DL_OnTop__      300   // overwrite-all (glDepthFunc (GL_ALWAYS))
#define DL_OnTopOff     301   // reset "overwrite-all"
#define DL_base_defCol  302   // default-color             UNUSED
#define DL_base_grid    303   // grid  (GL_grid__())       UNUSED
#define DL_shade_wire   304   // activate shaded or wireframe for surface
// free 303-309
#define DL_base_mod     310   // first index of BasModelnames (DL_base_mod)
                              // next is DL_base__ - first index of normal objects



//----------------------------------------------------------------
void GL_set_tria (Point *p1, Point *p2, Point *p3);
void GL_set_quad (Point *pa, Point *pb);
void GL_set_npt (Point *pta, int ptNr);
void GL_set_pt2 (Point2 *pt21);
void GL_set_ln_2pt (Point *p1, Point *p2);
void GL_set_pcv (int pnr, Point *pta, int clo);
void GL_set_symV3 (int symTyp, Point *pt1, Vector *vc1, double scale);
void GL_set_symVX (Plane *pln1, int typ, double scale);
void GL_set_ang (int att, Point *ptc, Vector *vx, Vector *vz, double ang1);
void GL_set_arrh2D (Point *p1, Point *p2);
void GL_set_p2cv (int pnr, Point2 *pta);
void GL_set_ln2_2pt (Point2 *p1, Point2 *p2);
void GL_set_ci2 (Point2 *pc, Point2 *p1, double ao);

void GL_set_symB (int symTyp, Point *pt1);
void GL_set_sym2B (int symTyp, Point2 *pt1, double scale);
void GL_set_symV_r (int symTyp, Point *pt1, double angd, double scale);
void GL_set_sym2V_r (int symTyp, Point2 *pt1, Point2 *pt2, double scale);
void GL_set_symV_o (int symTyp, Point *pt1, double az,double ay,double scale);

void GL_set_txtG (GText *tx1);
void GL_set_txtA (Point*, char*);
void GL_set_txt__ (int dMod, int bMod, Point *ptx,
                   double txAng, double ay, double az, double scale, char *txt);
void GL_set_Dim3 (Dim3 *dim3);
void GL_set_Ldr (int att, Dimen *dim1);

void GL_set_mdr (ModelRef *mdr, ModelBas *mdb);

//----------------------------------------------------------------
void   GL2D_pos_move        (int dx, int dy);
void   GL_DefineView        (int);
void   GL_DefineDisp        (int, int);

void   GL_Init__            (int, int, int);
void   GL_Init0             ();
void   GL_Init1             ();
void   GL_Init_View         ();
void   GL_InitPtAtt         (int ithick);
int    GL_InitNewAtt        (int, int);
void   GL_InitSymb          ();
void   GL_InitAFont         ();
void   GL_InitGFont         ();
int    GL_InitGF2 (int ind, int ianz, char mode[], Point2 *pta);
void   GL_InitModelSize     (double, int);

void   GL_Redraw            ();
void   GL_Redraw1           ();

void   GL_Reframe           ();
void   GL_Reframe1          ();

void   GL_Translate         ();
void   GL_Transl_Vert       ();

void   GL_Del0              (long);
void   GL_Delete            (long);
  
void   GL_GetGLMat0         ();
void   GL_GetNormal0        ();
void   GL_GetActPlane       ();
Point  GL_get_curPos_CP__        ();
Point  GL_get_curPos_WC         ();
Point  GL_get_curPos_CP_pt      (Point *pti);
Point  GL_GetCen            ();
Vector GL_GetEyeX           ();

long   GL_Get_DLind         ();
long   GL_GetActInd         ();

double GL_get_Scale         ();

void*  GL_Print1            (int *iw, int *ih);
// int    GL_Select            (int, int*, long*, long*);
int    GL_Feedback          (int*, float*, int);

void   GL_Sk2Uk             (double *,double *,double *,double,double,double);
void   GL_Uk2Sk             (double *,double *,double *,double,double,double);

void   GL_Do_Idle           (int*, int*, int, int);
void   GL_Do_Pan            (int, int);
void   GL_Do_Rot            (int, int);
void   GL_Do_Rot1           ();
void   GL_Do_CenRot         (double);
void   GL_Do_Scale          (int, int);
void   GL_Set_Scale         (double);
int    GL_View_set          (double[]);
int    GL_View_get          (double[]);
int    GL_Set_Cen           (Point *Ucen);

void   GL_Do_RubberBox      ();
void   GL_Start_RubberBox   ();
void   GL_Stop_RubberX      (double *, Point *);

void   GL_Reshape           (int, int);
void   GL_Clear             ();
void   GL_Resize            (int, int);
void   GL_Rescale           (double, Point*);


void   GL_Draw_Ini          (long*, int);
int    GL_set_patch        (int gTyp, int ptNr, Point *pa);
int    GL_set_ipatch       (int gTyp, int iNr, int *ia, Point *pa);

// void   GL_DrawQFac          (long*, int, ObjG2*, double, ObjG2*, double);
// void   GL_DrawRFac          (long*, int, ObjG2*, double, ObjG2*, double);
// void   GL_DrawRCone         (long*, int, ObjG2*, double, ObjG2*, double);

void   GL_DrawRSur     (long *ind,int attInd,int ptAnz,Point *pa1,Point *pa2);
int    GL_set_strip1        (Point *ps, Point *pa1, Point *pa2, int ptAnz);
int    GL_set_strip2        (Point *pa1, Point *pa2, int ptAnz, int newS);
void   GL_set_fan           (Point *pt1,int ptAnz,Point *pa1,int side, int newS);

void   GL_DrawDitto2 (long *ind, Point *p1, double az, char mir, Ditto *di);


  void GL_temp_del_1      (int ind);
  void GL_temp_del_all      ();
  long GL_temp_iNxt      ();
  long GL_temp_iLast ();


int    GRU_teileArc         (Point2*, int*, ObjG2*);
int    GRU_teileArc1        (Point2*, int,  ObjG2*);
int    GRU_teileLin         (Point2*, int,  Point2*, Point2*);

void   GRU_calc_normal      (Vector*, Point*, Point*, Point*);
void   GRU_calc_normal2     (Vector*, Point2*,Point2*,Point2*, double,double);

void   GL_list_close ();

// void   GL_Test              (long);


// #define GL_ptArr30Siz    1000
// #define GL_ptArr31Siz     140
// #define GL_ptArr32Siz     140


// size of Alfa-text (2D-Text, always normal to eye)
#define GLTXA_sizCX    8         // width character-bitmap
#define GLTXA_sizCY   13
#define GLTXA_sizBX   10         // width block; 8 + 2;
#define GLTXA_sizBY   17         // height block; 2 + sizCY + 2;


// relative move of the screenPos in screencoords
#define GL2D_pos_move(dx,dy) glBitmap (0,0,0.f,0.f,(float)dx,(float)dy,NULL)

// save the current color & rasterPosition
#define GL2D_pos_set()    glPushAttrib (GL_CURRENT_BIT)

// restore color & rasterPosition
#define GL2D_pos_get    glPopAttrib


// GL_list_open         start GL-list (glNewList)
#define GL_list_open(gli) glNewList (gli, GL_COMPILE);


// // GL_list_close        close GL-record (glEndList ())
// #define GL_list_close  glEndList

// GL_set_txt2D           text alphanum.(fixed size, horiz)
//   sTxt   null-terminated string to display
//   rasterPosition must have been set
//   color must be set before glRasterPos3dv
void GL_set_txt2D (char *sTxt);
#define GL_set_txt2D(sTxt)\
 glCallLists (strlen(sTxt), GL_UNSIGNED_BYTE, (GLubyte*)sTxt)

extern int GL_pickSiz ;        // SelectionDistance in Pixel (ScreenCoords)


/* ==================== EOF =============================== */
