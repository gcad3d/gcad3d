// ../ut/ut_bspl.h

int UT3D_cbsp_2pt (CurvBSpl *bsp, Memspc *memSeg, Point *p1, Point *p2);
int UT3D_cbsp_ci (CurvBSpl *bsp, Memspc *memSeg, Circ *ci1, Memspc *tmpSeg);
int UT3D_cbsp_ell (CurvBSpl *bsp, Memspc *memSeg, CurvElli *el1, Memspc *tmpSeg);

  int UT3D_pt_intlnbspl (int *nxp, Point *xptab,
                         Memspc *memSeg1, Line *ln, int iUnl, CurvBSpl *bspl);
  int UT3D_pt_intplbspl (int *pNr, Point *pa,
                      Plane *pln, CurvBSpl *bspl, Memspc *memSeg1, double tol);
  int UT3D_pt_int2bspl (int *nxp, Point *xptab,
                        Memspc *memSeg1, CurvBSpl *bspl1, CurvBSpl *bspl2);

  int UT3D_pt_projptbspl (int *nxp, Point *ptab, double *ttab,
                           CurvBSpl *bspl, Point *pt);
  int UT3D_pt_projptbsp_ext (double *uo,Point *po,Point *p0,CurvBSpl *bspl);
  int UT3D_pt_nearptbspl (Point *ptx, double *px, CurvBSpl *cv1, Point *pt);
  int UT3D_pt_evparCrvBSpl (Point *pt, CurvBSpl *bspl, double u);
  int UT3D_pt_evalparCv (Point *pto, CurvBSpl *cv1,  double uVal);

  int UT3D_vc_evalparCv (Vector *vco, CurvBSpl *cv1,  double uVal);


  int UT3D_bsp_ck_minSiz (CurvBSpl *cv1);
  int UT3D_bsp_ck_closed_tr (CurvBSpl *cv1);
  int UT3D_bsp_degrad (ObjGX *ox1, CurvBSpl *cv1, Memspc *memSeg1);
  int UT3D_bsp_infTg (int *dNr, double *dTab, CurvBSpl *bsp1);

  double UT3D_par1_parbsp (double *kv, CurvBSpl* cv1);
  double UT3D_parbsp_par1 (double pv, CurvBSpl* cv1);
  int UT3D_par_pt__pt_cvbsp (double *up, Point *pto, double *dist,
                         CurvBSpl *bspl,Point *pti);

  int UT3D_ptNr_bsp (int *iu, CurvBSpl *bsp, double tol1);

  int UT3D_bsp_cpsbsp (CurvBSpl *cvo, SurBSpl *su1, char cDir, int cvNr,
                        Memspc *memSeg1);

  int UT3D_cbsp_ptn (CurvBSpl *cvo,Memspc *memSeg1,Point *pTab,int pNr,int deg);
  int bspl_pol2_bsp2 (Point *pTab1, Point *pTab2, int *ptNr, int ptMax,
                    CurvBSpl *cv1, CurvBSpl *cv2, double tol);
  int UT3D_knotvec_m (double **knTab, double *v0, double *v1,
                      int ptNr, int deg, Memspc *workSeg);
  int bspl_knotvec__ (double *knTab, int ptNr, int deg);

  int bspl_cv_bsp (int *ptNr, Point *pTab, CurvBSpl *cv1);

  int bspl_pol_bsp (int *ptNr, Point *pTab,
                    CurvBSpl *cv1, int ptMax, double tol);

  int bspl_cvpol_cvbsp (int *ptNr, Point ptPol[],
                        int ianz, int ideg, double ps, double pe,
                        Point ptCrv[], double *vTab);

  int bspl_eval_Pt (int    ptNr, int    ideg, Point  ContPt[],
                     double *vk,  double uVal, Point  *P);
  int bspl_eval_expPt (Point *pto, CurvBSpl *cv1, double uv, double dist);
  int bspl_eval_Tg (int    segNr,  int    ideg,   Point  ContPt[],
                    double *vk,   double uVal,    Vector  *tg);

  int UT3D_bsp_ck_maxDeg (CurvBSpl *cv1);


/// UT3D_bsp_ck_maxDeg         returns max degree of bspline-curve
#define UT3D_bsp_ck_maxDeg(cv1) ((cv1)->ptNr - 1)

/// UT3D_bsp_tmpSpc_siz    get size of necessary tempSpace for UT3D_pt_projptbspl
#define bspl_tmpSpc_siz(cvBsp)\
  (cvBsp)->ptNr * (cvBsp)->deg * 10240


//===================== EOF =============================
