/*

Modifications:
2001-06-05 Portierung Unix

needs ut_geo.h

*//*!
\file ../ci/NC_Main.h
\ingroup grp_ci
*/


#define     APT_ind_INC   2500               // siz of NC_stat__
#define     NC_UPLISTSIZ  100                // siz of NC_up_tab


typedef struct {Point2 actPos; long lNr;
                unsigned tlNr:16, rpd:1, unused:15;}                NC_recTyp;

typedef struct {Ditto ditt; Point2 ditEnd; long lNr;
                char cNam[32], mode;}                               NC_up_rec;




void   WC_Init_all       (int);
void   WC_Init           ();
void   WC_Init_ObjG2     (ObjG2*);
void   WC_Init_Tol       ();
void   WC_EOF            ();
int    WC_PP_open        (int mode);

void   WC_setPosKreuz    ();
void   WC_setDisp3D      (int);
int    NC_setRefsys      (int);
void   NC_setGrafAtt     (int AttInd);
void   NC_set_actPos     (Point2*);
int    WC_set_obj_stat   (int stat);

// int    WC_actPos_reset   (int lNr);
// int    APT_disp_hili     (int lNr);
// int    APT_disp_reset    (int lNr);

void   APT_Init          ();
void   APT_Reset         ();

int    WC_Work__         (int, char*);
int    WC_Work1          (int, char*);
int    APT_work_def      (char*, char**);
int    APT_work_NCCmd    (char*, char**);
int    APT_work_AppCodTab  (char*, char**);

int APT_Lay_add(int layNr,int aus_anz,char* sptr,int* aus_typ,double* aus_tab);

int    APT_UP_up         (char);
int    APT_UP_down       ();


int    APT_stack_NCCmd   (char* buf, int);

int    APT_input_get     (char*);

int    APT_ausg_NCCmd    (int);
int    APT_ausg_chTool   (char*);

int    APT_PP_Write_Lin  (Point*);
int    APT_PP_Write_Cir  (Circ*);
int    APT_PP_Write      ();
void   APT_PP_FROM       ();

Point2 APT_transl2       (Point2*);
Point2 APT_rotate3       (Point*);
double APT_transl_rad    (double);

int    APT_disp_obj      (ObjG2*, int, double);
int    APT_disp_SymB     (int typ, int att, Point* pt1);
void   APT_disp_SymV     (int, int att, Point2*, double);
void   APT_disp_SymV1    (int, int att, Point*, double);
void   APT_disp_SymV2    (int, int att, Point*, Point*, double);
void   APT_disp_SymV3    (int, int att, Point*, Vector*, double);
void   APT_disp_TxtA     (int, Point*, char*);
// void   APT_disp_TxtG     (int typ,Point* pt1,float size,float angle,char* txt);
// void   APT_disp_Vec      (int att, long, Point*, Vector*);
void   APT_disp_ln       (Point2*, Point2*, int, double, double);
void   APT_disp_ac       (ObjG2*, int, double);
void   APT_disp_cv       (ObjG2*, int, double);
void   APT_disp_nam      (int, long, void*);

void   APT_hiliObj       (long*, ObjG2*, int, double);

void   APT_DrawDimen     (int typ, long apt_ind, ObjGX *dim1);
void   APT_DrawTxtG      (int typ, long apt_ind, GText *tx1);
// void   APT_DrawTxtA      (int typ, long apt_ind, GText *tx1);
void   APT_DrawPoint     (int, long, Point *);
void   APT_DrawLine      (int, long, Line *);
void   APT_DrawCirc      (int, long, Circ *);
void   APT_DrawCurv      (int, long, ObjGX *, double, Memspc*);
int    APT_DrawModel     (int, long, ModelRef *);
// void   APT_DrawRFac      (int, ObjG2 *,double,ObjG2 *,double);
int    APT_DrawSur (int typ, long apt_ind);
int    APT_DrawSol (int typ, long apt_ind);
int    APT_DrawPln (int typ, long objInd, Plane *pl1);



void   APT_curv2ptArr    (Point2*, int*, ObjG2*);

int    PP_up_list        (NC_up_rec **upAct, char* macnam, int workmode);

// int    WC_ask_WC_mode    ();
// Point2 WC_ask_actPos     ();
// ObjG2  WC_ask_actObj     ();
// double WC_ask_actZsur    ();
// double WC_ask_Zsur2      ();
// double WC_ask_Zsur1      ();

double WC_ask_ModSiz     ();


void   APT_get_dreh      (int *, double *, double);


//int    WC_ask_geo        ();

//=============== END OF nc_main.h =============================
