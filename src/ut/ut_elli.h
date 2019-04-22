
  double UT3D_angr_el_corr (double aa, double da, double db);
  double UT3D_angr_elpt (Point *pti,Point *ptc,Vector *va,Vector *vb);
  double UT3D_angr_par1_ell (double lpar, CurvElli *el1);
  double UT3D_par1_angr_ell (double *lpar, CurvElli *el1);
  int    UT2D_2angr_el2c_c (double *angs, double *ango, CurvEll2C *el2c);
  double UT3D_lne_ell (CurvElli *el1);
  int    UT3D_el_ptc_ptx_pt (CurvElli *el1, Point *ptc, Point *ptx, Point *pt1);
  int    UT3D_npt_ell (int *ptNr, Point *pa, CurvElli *el3);
  void   UT3D_el_tra_el_m3 (CurvElli *elo, CurvElli *eli, Mat_4x3 trmat);


// copy defaults (ango,srot,clo,trm)
void UT3D_el_cpyDef_el (CurvElli *elo, CurvElli *eli);
#define UT3D_el_cpyDef_el(elo,eli)\
  elo->ango = eli->ango;\
  elo->srot = eli->srot;\
  elo->clo  = eli->clo;\
  elo->trm  = eli->trm


 void UT3D_el_cpyDef_el2 (CurvElli *elo, CurvEll2 *eli);
#define UT3D_el_cpyDef_el2(elo,eli)\
  elo->ango = eli->ango;\
  elo->srot = eli->srot;\
  elo->clo  = eli->clo;\
  elo->trm  = eli->trm


void UT2D_el_cpyDef_el3 (CurvEll2 *elo, CurvElli *eli);
#define UT2D_el_cpyDef_el3(elo,eli)\
  elo->ango = eli->ango;\
  elo->srot = eli->srot;\
  elo->clo  = eli->clo;\
  elo->trm  = eli->trm


void UT2D_elc_cpyDef_el3 (CurvEll2C *elo, CurvElli *eli);
#define UT2D_elc_cpyDef_el3(elo,eli)\
  elo->ango = eli->ango;\
  elo->srot = eli->srot;\
  elo->clo  = eli->clo;\
  elo->trm  = eli->trm

void UT2D_elc_cpyDef_el2 (CurvEll2C *elo, CurvEll2 *eli);
#define UT2D_elc_cpyDef_el2(elo,eli)\
  elo->ango = eli->ango;\
  elo->srot = eli->srot;\
  elo->clo  = eli->clo;\
  elo->trm  = eli->trm



//=========== EOF ==============
