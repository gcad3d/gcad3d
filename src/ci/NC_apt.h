/*
Modifications:
2001-06-05 Portierung Unix

*//*!
\file ../ci/NC_apt.h
\ingroup group_xx
*/

/*
nach NC_cut.h verlegt
#define KONIK_0            0
#define KONIK_1            1
#define KONIK_2            2
#define KONIK_3            3
*/

// #define Typ_nc_cut       0
// #define Typ_nc_drill     1


// Typ_Cmd1 is parallel to ObjCodTab in ../ut/ut_gtypes.c
enum Typ_Cmd1 {
   T_ON_,       T_OFF_,      T_IN,        T_OUT,       T_ARC,       // 0-4
   T_HI,        T_LO,        T_MA,        T_TAG,       T_PERP,      // 5-9
   T_CCW,       T_CW,        T_ELL,       T_IMG,       T_CCV2,      // 10-14
   T_POL2,      T_POL,       T_PSP3,      T_BSP0,      T_CCV,       // 15-19
   T_RCIR,      T_RSTRIP,    T_SRU,       T_HAT,       T_SRV,       // 20-24
   T_PRISM,     T_SPH,       T_CON,       T_TOR,       T_CYL,       // 25-29
   T_CUT,       T_REV,       T_SWEEP,     T_RGT,       T_LFT,       // 30-34
   T_DIM,       T_DIMA,      T_LDR,       T_LOFT,      T_FSUB,      // 35-39
   T_PRJ,       T_TRA,       T_ROT,       T_MIR,       T_TXA,       // 40-44
   T_DIMD,      T_DIMR,      T_DIM3,      T_REC,       T_INT,       // 45-49
   T_RSYS,      T_REPL,      T_RBSP,      T_ARC1,      T_CTRL,      // 50-54
   T_LDRP,      T_LDRC,      T_LDRS,      T_CTLG,      T_ISO,       // 55-59
   T_FW,        T_CX,        T_PTAB,      T_MSH,       T_CLOT,      // 60-64
   T_TNG,       T_IMP,       T_BSP1,      T_PARL,      T_BLEND,     // 65-69
   T_UNL,       T_UNL1,      T_UNL2,      T_DISP_PT,   T_DISP_PL};  // 70-74 


char** APT_decode_txt    (char**);
int    APT_decode_var    (double*,int, int*, double*);
int    APT_decode_vc     (Vector*, int, int*, double*);
int    APT_decode_pt     (Point*, int, int*, double*);
int    APT_decode_ln     (Line *, int, int*, double*);
int    APT_decode_ci     (Circ *, int, int*, double*);
int    APT_decode_pln1   (Plane *, int, int*, double*);
int    APT_decode_pln2   (Plane *, int, int*, double*);
// int    APT_decode_tool   (BTool *, int , int *, double *);
int    APT_decode_inp    (int , int *, double *);
// int    APT_decode_ccv    (ObjGX *cv1, Memspc *tbuf1, Memspc *tbuf2,
                         // int aus_anz,int aus_typ[],double aus_tab[]);
int    APT_decode_ccv2   (ObjGX *cv1,int aus_anz,int aus_typ[],double aus_tab[]);


// int    APT_decode_ausdr (int *aus_typ,double *aus_tab,int aus_siz,char** data);
// int    APT_decode_ausdr1 (int* aus_typ,double* aus_tab,int aus_siz,char* data);
// int    APT_decode_func(int* w_typ,double* w_tab,int w_siz,char* func,char* ausd);

int    APT_store_obj     (int*,  long*,  int,  int*,  double*);

char*  APT_cp_ausd       (char*, char*, int);

char*  APT_get_Val       (double*, char*, double);
char*  APT_get_Txt       (char*, char*, double);
void   APT_get_TxAll     (char* outBuf, char* inBuf, double offset);
int    APT_get_String    (char*, char*, double);
int    APT_solv_mod      (Point2 *, int);
int    APT_solv_mod_1    (Point2 *, int, int *, int);
int    APT_solv_x_max    (double *, Point2 *, int, int *);
int    APT_solv_y_max    (double *, Point2 *, int, int *);
int    APT_solv_x_min    (double *, Point2 *, int, int *);
int    APT_solv_y_min    (double *, Point2 *, int, int *);

int    APT_decode_pt1    (Point*, int, int*, double*);
//===================== EOF ==========================
