/***************************************************************************
                   u3d.h - general 3d-geometric functions
                   --------------------------------------
    begin                : Tue May 01 2007
    copyright            : (C) 2007 by Thomas Backmeister
    email                : t.backmeister@gmx.at

***************************************************************************/




// ------------- Prototypes -------------------------------
//
int  UT3D_ChkPlanPts (int piT[3], int, Point*);
int  UT3D_ch_lnbox (Line*, Point*, Point*, double);
int  UT3D_CompPts (int, Point*, double);
int  UT3D_GravCentPts (Point*, int, Point*);
int  UT3D_VcPts2Crvs (Vector*, void*, int, void*, int, int);

// ------------- EOF --------------------------------------
