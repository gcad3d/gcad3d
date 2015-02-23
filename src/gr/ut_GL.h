/*


Korr:
2001-06-08 Taginfos zu.


=======================================================================*/
#define GR_MODE_DRAW       0
#define GR_MODE_SELECT     1
#define GR_MODE_FEEDBACK   2
#define GR_MODE_NORMAL     3
#define GR_MODE_PRINT1     4
#define GR_MODE_PRINT2     5

#define GR_STATE_WIRE      0
#define GR_STATE_SHADE     1
#define GR_STATE_HID1      2
#define GR_STATE_HID2      3
#define GR_STATE_HID3      4


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
int    GL_InitGF2 (int ind, int ianz, char mode[], double cx[], double cy[]);
void   GL_InitModelSize     (double, int);

void   GL_Redraw            ();
void   GL_Redraw1           ();

void   GL_Reframe           ();
void   GL_Reframe1          ();

void   GL_Translate         ();
void   GL_Translate1        ();
void   GL_Transl_Vert       ();
void   GL_Transl_VertP      (Point*);

void   GL_Del0              (long);
void   GL_Delete            (long);
  
void   GL_GetGLMat0         ();
void   GL_GetNormal0        ();
void   GL_GetActPlane       ();
Point  GL_GetViewPos        ();
Point  GL_GetCurPos         ();
Point  GL_GetConstrPos      (Point *pti);
Point  GL_GetCen            ();
Point  GL_GetViewPt         (Point *pti);
Vector GL_GetEyeX           ();

long   GL_Get_DLind         ();
long   GL_GetActInd         ();

double GL_get_Scale         ();
double GL_query_ViewZ       ();


void*  GL_Print1            (int *iw, int *ih);
// int    GL_Select            (int, int*, long*, long*);
int    GL_Feedback          (int*, float*);

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
void   GL_Disp_ln           (Point*, Point*);
int    GL_Disp_face         (int gTyp, int ptNr, Point *pa);
int    GL_Disp_iface        (int gTyp, int iNr, int *ia, Point *pa);

void   GL_DrawPoint         (long*, int, Point*);
void   GL_DrawLine          (long*, int, Line*);
void   GL_DrawLtab          (long *ind, int attInd, Line *lna, int lNr);
void   GL_DrawVec           (long*, int, Point*, Vector*);
void   GL_DrawPoly2D        (long*, int, int, Point2[], double);
void   GL_DrawPoly          (long*, int, int, Point*);

// void   GL_DrawQFac          (long*, int, ObjG2*, double, ObjG2*, double);
// void   GL_DrawRFac          (long*, int, ObjG2*, double, ObjG2*, double);
// void   GL_DrawRCone         (long*, int, ObjG2*, double, ObjG2*, double);

void   GL_DrawRSur     (long *ind,int attInd,int ptAnz,Point *pa1,Point *pa2);
int    GL_DrawStrip1        (Point *ps, Point *pa1, Point *pa2, int ptAnz);
int    GL_DrawStrip2        (Point *pa1, Point *pa2, int ptAnz, int newS);
void   GL_DrawFan           (Point *pt1,int ptAnz,Point *pa1,int side, int newS);

void   GL_DrawSymB          (long*, int, int att, Point*);
void   GL_DrawSymV          (long*, int, int att, Point*, double);
void   GL_DrawSymV2         (long*, int, int att, Point*, Point*, double);
void   GL_DrawSymV3         (long*, int, int att, Point*, Vector*, double);
void   GL_DrawTxtA          (long*, int, Point *, char *);
void   GL_DrawTxtG          (long *ind, int att,
                             Point *P1, float size, float ang, char *txt);
void   GL_DrawDimen         (long *ind, int att, Dimen *dim1);
void   GL_DrawDitto2 (long *ind, Point *p1, double az, char mir, Ditto *di);
void   GL_DrawModel  (long *ind, int att, ModelRef *mdr, ModelBas *mdb);

int    GL_txt__             (int dMod, int bMod,
                            Point *ptx, double txAng, double ay, double az,
                            double scale, char *txt);

  void GL_temp_Delete      (long ind);
  void GL_temp_delete      ();
  long GL_temp_GetInd      ();
  long GL_GetInd_temp      ();
  long GL_GetInd_last_temp ();


int    GRU_teileArc         (Point2*, int*, ObjG2*);
int    GRU_teileArc1        (Point2*, int,  ObjG2*);
int    GRU_teileLin         (Point2*, int,  Point2*, Point2*);

void   GRU_calc_normal      (Vector*, Point*, Point*, Point*);
void   GRU_calc_normal2     (Vector*, Point2*,Point2*,Point2*, double,double);

int    GL_icons_dispTags    (int iNr, char *txt, char stat);
// void   GL_Test              (long);


// #define GL_ptArr30Siz    1000
// #define GL_ptArr31Siz     140
// #define GL_ptArr32Siz     140


// relative move of the screenPos in screencoords
#define GL2D_pos_move(dx,dy) glBitmap (0,0,0.f,0.f,(float)dx,(float)dy,NULL)


extern int GL_pickSiz ;        // SelectionDistance in Pixel (ScreenCoords)




/* ==================== EOF =============================== */
