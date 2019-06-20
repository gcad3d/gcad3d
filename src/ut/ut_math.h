/***************************************************************************
                          ut_math.h  -  description
                             -------------------
    begin                : Tue Mar 11 2003
    copyright            : (C) 2003 by CADCAM-Services Franz Reiter
    email                : franz.reiter@cadcam.co.at

 ***************************************************************************


-------------------------------------------
Korr:
2003-xx-xx  xxxx


-------------------------------------------
Offen:


-------------------------------------------
*/




/* ------------- prototypes for ut_math.c ------------------- */
int    UTM_geomSeq_solv (int mode, double *a, double *ae, double *q, int *n);
int    UMT_FresnelIntgls (double *fci, double *fsi, double t1, double t2);
int    UTM_SolLinEquSys (double*, int, double*, double*);
double UTM_db_bincoeff (long n, long k);
double UTM_db_binquot (long n, long i, long j);
int    UTM_pol_polmultpol (double *polm, int deg1, double *pol1, int deg2,
                           double *pol2);


dcomplex UTM_cx_alg2pol       (dcomplex *cx);
dcomplex UTM_cx_cxaddcx       (dcomplex *cx1, dcomplex *cx2);
dcomplex UTM_cx_cxmultcx      (dcomplex *cx1, dcomplex *cx2);
dcomplex UTM_cx_cxsubtcx      (dcomplex *cx1, dcomplex *cx2);
dcomplex UTM_cx_pol2alg       (dcomplex *cx);
dcomplex UTM_cx_root          (dcomplex *cx);

int      UTM_sign_3cx         (dcomplex *, dcomplex *, dcomplex *, double);
int      UTM_scale_4db        (double *, double *, double *, double *,
                               double *, double lim);

int      UTM_zeros_cubicpol   (dcomplex zero[], int zmult[], polcoeff_d3 *pol);
int      UTM_zeros_quarticpol (dcomplex zero[], polcoeff_d4 *pol);


#ifdef _MSC_VER
#define IS_NAN _isnan
#else
#define IS_NAN isnan
#endif
/*======================== EOF ======================================*/
