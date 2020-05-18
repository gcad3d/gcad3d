/***************************************************************************
                          ut_const.h  -  description
                             -------------------
    begin                : Fri Aug 25 2000
    copyright            : (C) 2000 by CADCAM-Services Franz Reiter
    email                : franz.reiter@cadcam.co.at

****************************************************************************

*//*!
\file ../ut/ut_tol_const.h
\ingroup grp_const


\code

FOR INFORMATION ONLY;
DO NOT INCLUDE.
USE WITH:
// includes from ../ut/ut_geo_const.h
extern <typ> <name>;

(is included in ../ut/ut_geo2d.c)

\endcode *//*

*/



//================================================================
// constant geometric objects:
#ifndef INCLUDE_FULL

// constant Tolerances:
extern double UT_TOL_min2;
extern double UT_TOL_min1;
extern double UT_TOL_min0;
extern double UT_TOL_PAR;      ///< tolerance for parallelism
extern double UT_TOL_Ang1;     ///< Angle 0.01 degree
extern double UT_TOL_Ang2;     ///< Angle 0.1 degree
extern double UT_TOL_0Cos;     ///< tolerance for zero cosine

// Angles: degree-val: 1=0.017  0.1:0.002  0.01=0.0002


//================================================================
// Tolerances depending on Modelsize: APT_ModSiz (../ci/NC_Main.c)

/// identical Points-tolerence
extern double UT_TOL_pt;

/// max deviation of points from line
extern double UT_TOL_ln;

/// max analytical deviation for curves
extern double UT_TOL_cv;

/// max. display-deviation for circles, curves
extern double UT_DISP_cv;

/// length of construction-lines ;  see also UT_LEN_TNG; see also UT_LEN_TNG
extern double UT_DISP_ln;



//================================================================
#else


// constant Tolerances:
double UT_TOL_min2 = 1E-12;
double UT_TOL_min1 = 0.00000001;
double UT_TOL_min0 = 0.0000005;
double UT_TOL_PAR  = 0.00001;    ///< tolerance for parallelism
double UT_TOL_Ang1 = 0.00017;    ///< Angle 0.01 degree
double UT_TOL_Ang2 = 0.0002;     ///< Angle 0.1 degree
double UT_TOL_0Cos = 0.0000001;  ///< tolerance for zero cosine

// Angles: degree-val: 1=0.017  0.1:0.002  0.01=0.0002


//================================================================
// Tolerances depending on Modelsize: APT_ModSiz (../ci/NC_Main.c)
// Defaultvalues with Modelsize=500:

/// identical Points-tolerence
double UT_TOL_pt   = 0.0001;

/// max deviation of points from line
double UT_TOL_ln   = 0.001;

/// max analytical deviation for curves
double UT_TOL_cv   = 0.005;    // Angle: ca 0.3 degree

/// max. display-deviation for circles, curves
double UT_DISP_cv  = 0.05;         // 2003-09-05: 0.05; war 0.01   

/// length of construction-lines ;  see also UT_LEN_TNG; see also UT_LEN_TNG
double UT_DISP_ln = 1000.;

#endif



//================  EOF =====================
