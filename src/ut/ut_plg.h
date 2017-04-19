
double UT3D_ck_pta_maxDev (int ptNr, Point *pTab);
double UT3D_plg_lvTab (double *lTab, Point *pTab, int ptNr);
double UT3D_par_par1plg (double pv, CurvPoly* cv1);
double UT3D_par1_parplg (double *kv, CurvPoly* cv1);


/// UPLG_pare_unl               get unlimited endParameter of CurvPoly  INLINE
double UPLG_pare_unl (CurvPoly*);
#define UPLG_pare_unl(cvp)\
 ((cvp)->lvTab[(cvp)->ptNr - 1])



//===================== EOF =============================
