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

\endcode *//*

*/




//================================================================
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




//================  EOF =====================
