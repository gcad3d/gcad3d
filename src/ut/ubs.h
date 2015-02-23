/***************************************************************************
                        ubs.h - bsplines
                        ----------------
    begin                : Sat Apr 28 2007
    copyright            : (C) 2007 by Thomas Backmeister
    email                : t.backmeister@gmx.at

***************************************************************************/


#define TabS1      100


// ------------- Geometric Objects ------------------------
//


// ------------- Constants defined in ubs.c ---------------
//


// ------------- inline functions -------------------------
//


// ------------- Prototypes -------------------------------
//
int UBS_DivCtrlPtsBsp (Point*, Point*, int, double*, int, Point*,
		       double, int, int, int, Memspc*);
int UBS_DivKnotVec (double**, Memspc*, double**, Memspc*, int*,
	            double*, int, int, double);
int UBS_FdSpnKntVec (double*, int, int, double);
int UBS_KntVecParVec (double**, Memspc*, int, int, double*);
int UBS_ParVecPts (double**, Memspc*, int, Point*, int);
int UBS_ParVecUVPts (double**, Memspc*, int, int, Point*, int);
// ------------- EOF --------------------------------------
