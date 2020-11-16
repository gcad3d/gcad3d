/***************************************************************************
                      ubssur.h - bspline surfaces
                      ---------------------------
    begin                : Sat Apr 28 2007
    copyright            : (C) 2007 by Thomas Backmeister
    email                : t.backmeister@gmx.at

***************************************************************************/



// ------------- Geometric Objects ------------------------
//


// ------------- Constants defined in ubssur.c ------------
//
extern double UT_TOL_0Cos;


// ------------- inline functions -------------------------
//


// ------------- Prototypes -------------------------------
//
int USBS_CpyBspSur (SurBSpl*, Memspc*, SurBSpl*);
int USBS_DegElevBspSur (SurBSpl*, Memspc*, SurBSpl*, int, int);
int USBS_DerivBspSur (SurBSpl*, Memspc*, SurBSpl*, int);
int USBS_DifKntsKntVecBspSur (int*, double**, Memspc*, int, double*,
	                      SurBSpl*, int);
int USBS_DirIndSdBspSur (double**, double**, int*, Point**, Memspc*,
	                 double**, double**, int*, Point**, Memspc*,
		         int, double*, double*, int, int, Point*, int, 
		         double, int);
int USBS_EvPtBspSur (Point*, SurBSpl*, double, double, Memspc*);
int USBS_GordSurBspCrvNet (SurBSpl*, Memspc*, int, CurvBSpl**, int, CurvBSpl**,
			   int, int, Memspc*, Memspc*);
int USBS_IntpolBspSur (SurBSpl*, Memspc*, int, int, Point*, int, int,
	               double*, double*, int, Memspc*);
int USBS_IntsectLnBspSur (int*, Point*, int, Line*, SurBSpl*, void*, int,
		          Memspc *memSegT[4], Memspc*);
int USBS_LinCombBspSurs (SurBSpl*, Memspc*, int, SurBSpl*, double*);
int USBS_MkeCmpBspSurs (SurBSpl*, Memspc*, int, SurBSpl**, int, int, Memspc*);
int USBS_MrgKntVecBspSurs (int*, double**, Memspc*, int, SurBSpl*, int, Memspc*);
int USBS_NxtPtBdryBspSur (Point*, double*, SurBSpl*, int, Point*, Memspc*);
int USBS_NxtPtBspSur (Point*, double*, double*, SurBSpl*, int, int, Point*,
	              int, int, Memspc*);
int USBS_NvPtBspSur (Vector*, SurBSpl*, Point*, Memspc *memSegT[4], Memspc*);
int USBS_RefKntVecBspSur (SurBSpl*, Memspc*, SurBSpl*, int, double*, int);
int USBS_SkinSurBspCrvs (SurBSpl*, Memspc*, int, CurvBSpl*, int, int, double*,
                         int, Memspc*);
int USBS_SubDivBspSur (SurBSpl*, Memspc*, SurBSpl*, Memspc*, SurBSpl*,
	               double, int);
int USBS_SwUVBspSur (SurBSpl*, Memspc*, SurBSpl*);
int USBS_TestPatsBspSur (SurBSpl*, Memspc*);
int USBS_TgVecIsoBspSur (Vector*, SurBSpl*, double, double, int, Memspc*);
int USBS_UVPtBspSur (double*, double*, double*, double*, SurBSpl*, void*,
	             int, Point*, double, Memspc *memSegT[4], Memspc*);
int USBS_4QuBspSur (SurBSpl *surT[4], Memspc *memsurT[4], SurBSpl*,
		    double, double, Memspc*);
// ------------- EOF --------------------------------------
