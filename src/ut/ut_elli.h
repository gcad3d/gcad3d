
  double UT3D_angr_el_corr (double aa, double da, double db);
  double UT3D_angr_elpt (Point *pti,Point *ptc,Vector *va,Vector *vb);
  double UT3D_angr_par1_ell (double lpar, CurvElli *el1);
  double UT3D_par1_angr_ell (double *lpar, CurvElli *el1);
  int    UT3D_2angr_el (double *angs, double *ango, CurvEll2C *el2c);
  double UT3D_lne_ell (CurvElli *el1);
  int    UT3D_el_ptc_ptx_pt (CurvElli *el1, Point *ptc, Point *ptx, Point *pt1);
  int    UT3D_npt_ell (int ptNr, Point *pa, CurvElli *el3);

//=========== EOF ==============
