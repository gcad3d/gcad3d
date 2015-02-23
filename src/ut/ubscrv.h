/***************************************************************************
                     ubscrv.h - bspline curves
                     -------------------------
    begin                : Sat Apr 28 2007
    copyright            : (C) 2007 by Thomas Backmeister
    email                : t.backmeister@gmx.at

***************************************************************************/



// ------------- Geometric Objects ------------------------
//


// ------------- Constants defined in ubscrv.c ------------
//


// ------------- inline functions -------------------------
//


// ------------- Prototypes -------------------------------
//
int UCBS_BspCrvPts (CurvBSpl*, Memspc*, int, Point*, int, int, Memspc*);
int UCBS_CpyBspCrv (CurvBSpl*, Memspc*, CurvBSpl*);
int UCBS_CrssParVecBspCrvs (double**, Memspc*, int, CurvBSpl*);
int UCBS_CtrlPtsIntpol (Point*, int, Point*, int, double*, double*, Memspc*);
int UCBS_DegElevBspCrv (CurvBSpl*, Memspc*, CurvBSpl*, int);
int UCBS_DifKntsKntVecBspCrv (int*, double**, Memspc*, int, double*, CurvBSpl*);
int UCBS_DirIndIsoBspSur (CurvBSpl*, Memspc*, int, int, double*, int, int,
			  double*, Point*, double, int);
int UCBS_EvPtBspCrv (Point*, CurvBSpl*, double, Memspc*);
int UCBS_IsoBspCrvBspSur (CurvBSpl*, Memspc*, SurBSpl*, double, int);
int UCBS_MkeCmpBspCrvs (CurvBSpl*, Memspc*, int, CurvBSpl**, int, Memspc*);
int UCBS_MrgKntVecBspCrvs (int*, double**, Memspc*, int, CurvBSpl*, Memspc*);
int UCBS_TrfKntVecBspCrv (CurvBSpl *bspo, Memspc *memSeg, CurvBSpl *bspi,
                      double scf, double trv);
int UCBS_NxtPtBspCrv (Point*, double*, CurvBSpl*, Point*, Memspc*);
int UCBS_OffSurBspCrv (CurvBSpl*, Memspc*, CurvBSpl*, SurBSpl*, double,
                       Memspc *memSegT[4], Memspc*);
int UCBS_PrepCoons3BspCrvs (CurvBSpl*, CurvBSpl*, Memspc*, CurvBSpl**);
int UCBS_PtUBspCrv (Point*, CurvBSpl*, double, Memspc*);
int UCBS_RefKntVecBspCrv (CurvBSpl*, Memspc*, CurvBSpl*, int, double*);
int UCBS_RevBspCrv (CurvBSpl*, Memspc*, CurvBSpl*);
int UCBS_SubDivBspCrv (CurvBSpl*, Memspc*, CurvBSpl*, Memspc*, CurvBSpl*,
	               double);
int UCBS_TestBezBspCrv (CurvBSpl*, Memspc*);
int UCBS_TestSegsBspCrv (CurvBSpl*, Memspc*);
int UCBS_TestSegsRBspCrv (CurvRBSpl*, Memspc*);
int UCBS_TypCon2BspCrvs (CurvBSpl*, CurvBSpl*, Memspc*);
int UCBS_XPtsBspCrvNet (Point**, double**, double**, Memspc*, int, CurvBSpl*,
		        int, CurvBSpl*, Memspc*);
int UCBS_3BspCrvsBspCrv (CurvBSpl *bsplTab[3], Memspc*, CurvBSpl*, Memspc*);

// ------------- EOF --------------------------------------
