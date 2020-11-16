/*

2001-05-04 Interf DB_Get_GR_Obj geandert !

BRAUCHT vorher include ut_geo.h !!!

*//*!
\file ../db/ut_DB.h
\ingroup grp_db

*/

// #define DB_dump_dbo DEB_dump_dbo
// 

// DB_isFree_<obj>           test if obj is free
// if(DB_isFree_ModRef (&mdr_tab[i1]))     printf("yes-its free \n");
// if(!(DB_isFree_ModRef (&mdr_tab[i1])))  printf("no, is in use\n");
#define DB_isFree_Var(obj) *(obj) == UT_VAL_MAX
#define DB_isFree_PT(obj) (obj)->x == UT_VAL_MAX
#define DB_isFree_LN(obj) (obj)->p1.x == UT_VAL_MAX
#define DB_isFree_VC(obj) (obj)->dx == UT_VAL_MAX
#define DB_isFree_CI(obj) (obj)->p1.x == UT_VAL_MAX
#define DB_isFree_PLN(obj) (obj)->p == UT_VAL_MAX
#define DB_isFree_CV(obj) (obj)->typ == Typ_Error
#define DB_isFree_GTxt(obj) (obj)->typ == Typ_Error
#define DB_isFree_Sur(obj) (obj)->typ == Typ_Error
#define DB_isFree_Sol(obj) (obj)->typ == Typ_Error
#define DB_isFree_Tra(obj) (obj)->typ == Typ_Error
#define DB_isFree_Act(obj) (obj)->typ == Typ_Error
#define DB_isFree_ModRef(obj) (obj)->po.x == UT_VAL_MAX

#define DB_setFree_Var(dbi) vr_tab[dbi] = UT_VAL_MAX
#define DB_setFree_VC(dbi) vc_tab[dbi].dx = UT_VAL_MAX
#define DB_setFree_PT(dbi) pt_tab[dbi].x = UT_VAL_MAX
#define DB_setFree_LN(dbi) ln_tab[dbi].p1.x = UT_VAL_MAX
#define DB_setFree_CI(dbi) ci_tab[dbi].p1.x = UT_VAL_MAX
#define DB_setFree_PLN(dbi) pln_tab[dbi].p = UT_VAL_MAX
#define DB_setFree_CV(dbi) cv_tab[dbi].typ = Typ_Error
#define DB_setFree_SU(dbi) su_tab[dbi].typ = Typ_Error
#define DB_setFree_SO(dbi) so_tab[dbi].typ = Typ_Error
#define DB_setFree_TR(dbi) tra_tab[dbi].typ = Typ_Error
#define DB_setFree_TX(dbi) tx_tab[dbi].typ = Typ_Error
#define DB_setFree_MR(dbi) mdr_tab[dbi].po.x = UT_VAL_MAX
#define DB_setFree_Act(dbi) ac_tab[dbi].typ = Typ_Error

#define DB_VCX_IND        -1   ///< DX
#define DB_VCY_IND        -2   ///< DY
#define DB_VCZ_IND        -3   ///< DZ
#define DB_VCIX_IND       -4   ///< DIX
#define DB_VCIY_IND       -5   ///< DIY
#define DB_VCIZ_IND       -6   ///< DIZ

#define DB_PLX_IND        -1   ///< RX
#define DB_PLY_IND        -2   ///< RY
#define DB_PLZ_IND        -3   ///< RZ
#define DB_PLIX_IND       -4   ///< RIX
#define DB_PLIY_IND       -5   ///< RIY
#define DB_PLIZ_IND       -6   ///< RIZ


extern long APT_VR_SIZ;
extern long APT_PT_SIZ;
extern long APT_VC_SIZ;
extern long APT_LN_SIZ;
extern long APT_CI_SIZ;
extern long APT_PL_SIZ;
extern long APT_MR_SIZ;
// extern long APT_TL_SIZ;
extern long APT_CV_SIZ;
extern long APT_TX_SIZ;
extern long APT_SU_SIZ;
extern long APT_SO_SIZ;
extern long DB_CSIZ;
extern long DB_MNM_SIZ;



// #define UTO__dbo        UTO__dbo



void   DB_Init          (int mode);

// int    DB_StoreAtt      (long, GR_Att*);
int    DB_Store_obj     (long *dbi, int typ, void *data);
int    DB_StoreTxt      (char **pos, char *text);
long   DB_StoreDim_     (long Ind, Dimen *dim1);
long   DB_StoreDim3     (long Ind, Dim3 *dim3);
long   DB_StoreGTxt     (long Ind, GText *gtx1);
int    DB_StoreSur      (long *Ind, ObjGX *ox1);
int    DB_StoreTra      (long Ind, ObjGX *ox1);
long   DB_StoreVar      (long, double);
long   DB_StoreVector   (long, Vector*);
long   DB_StorePoint    (long, Point*);
long   DB_StoreLine     (long, Line*);
long   DB_StoreCirc     (long, Circ*);
long   DB_StoreCvEll    (long Ind, CurvPoly *cvell);
long   DB_StoreCvPlg    (long Ind, CurvPoly *cvplg, int iNew);
long   DB_StoreCvBsp    (long Ind, CurvBSpl *cvbsp);
long   DB_StoreCurv     (long, ObjGX*, int);
long   DB_StoreRef      (long, Plane*);
// void   DB_StoreTool     (long, BTool*);
int    DB_StoreDummy    (int typ, long Ind);

long      DB_Store_hdr_cv (ObjGX **cvo, long Ind);
long      DB_Store_hdr_su (ObjGX **oxo, long Ind);
long      DB_Store_hdr_nt (ObjGX **oxo, long Ind);

void      DB_Delete        (int, long);

long      DB_FindVector    (Vector* vc1);

long      DB_GetDynInd     (int);
char**    DB_GetTxt        (long);
double    DB_GetVar        (long);
double*   DB_get_Var       (long Ind);
Vector    DB_GetVector     (long);
Point     DB_GetPoint      (long);
Line      DB_GetLine       (long);
Circ      DB_GetCirc       (long);
ObjGX*    DB_GetCurv       (long);
ObjGX*    DB_GetSol        (long);
ObjGX*    DB_GetGTxt       (long Ind);
// int       DB_GetTool       (BTool* tl1, long Ind);
ObjGX*    DB_GetSur        (long Ind, int mode);
ObjGX*    DB_GetTra        (long Ind);

Point*    DB_get_PT        (long Ind);
Line*     DB_get_LN        (long Ind);
Vector*   DB_get_VC        (long Ind);
Circ*     DB_get_CI        (long Ind);
void*     DB_get_CV        (int *ityp, long Ind);
Plane*    DB_get_PLN       (long Ind);
ModelRef* DB_get_ModRef    (long Ind);
ModelBas* DB_get_ModBas    (int Ind);
ModelNode* DB_get_ModNod   (int ind);

int       UTO__dbo     (void **pDat, int *oNr, int dbTyp, long dbInd);
int       DB_GetObj        (long, long*, int*);
ObjGX     DB_GetObjGX      (int typ, long apt_ind);
// int    DB_GetObjUX      (ObjUX *ox, int typ, long apt_ind);
int       DB_GetConnObj    (int*, long*, int*, long*, Point2 *);
long      DB_GetObjTyp2Pt  (int*, Point2 *, Point2 *);
void      DB_GetRef        (Plane*, long);
Activity* DB_get_Activ     (long Ind);

Plane*    DB_DefRef        (long Ind);

void*     DB_cPos          ();
void*     DB_cSav          (long size, void *data);
void*     DB_cGet          (void *data, void *pos, long size);

int       DB_Lay           (int, int, int);
int       DB_Lay_add       (int, int, char*, int*, double*);

long   DB_QueryPoint    (Point*);
long   DB_QueryLine     (Point*);
long   DB_QueryCirc     (Point*);
long   DB_QueryCurv     (Point*);
int    DB_QueryDynPoint (Point *, Point *);

long DB_QueryPrvUsed (int typ, long istart);
long DB_QueryNxtUsed (int typ, long istart);
long   DB_QueryNxtFree   (int typ, int istart);
long   DB_dbo_get_free  (int typ);

void   DB_fix_tol       (double);
int    DB_2DTyp_3DTyp   (int);
int    DB_Typ_Char      (char*);
// char*  DB_Char_Typ      (int typ);

char*  DB_mdlNam_iBas   (int bmNr);

void   DB_Stat          ();
int    DB_save__        (char *mNam);
int    DB_load__        (char *mNam);
long   DB_dyn__         (int mode, int typ, long ind);


// EOF
