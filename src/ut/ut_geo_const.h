/***************************************************************************
                          ut_geo_const.h  -  description
                             -------------------
    begin                : Fri Aug 25 2000
    copyright            : (C) 2000 by CADCAM-Services Franz Reiter
    email                : franz.reiter@cadcam.co.at

****************************************************************************

*//*!
\file ../ut/ut_geo_const.h
\ingroup grp_const


\code

full include in ../ut/ut_geo2d.c

\endcode *//*

*/



//================================================================
// constant geometric objects:
#ifndef INCLUDE_FULL
extern const Point   UT3D_PT_NUL;
extern const Vector  UT3D_VECTOR_NUL;

extern const Vector  UT3D_VECTOR_X;
extern const Vector  UT3D_VECTOR_Y;
extern const Vector  UT3D_VECTOR_Z;

extern const Vector  UT3D_VECTOR_IX;
extern const Vector  UT3D_VECTOR_IY;
extern const Vector  UT3D_VECTOR_IZ;

extern const Mat_3x3 UT3D_MAT_3x3;
extern const Mat_4x3 UT3D_MAT_4x3;
extern const Mat_4x4 UT3D_MAT_4x4;


 
//----------------------------------------------------------------
#else


const Point2  UT2D_PT_NUL     = { 0.0, 0.0 };
const Point   UT3D_PT_NUL     = { 0.0, 0.0, 0.0 };

const Point2  UT2D_PT_INFTY   = {9999999999999.999, 9999999999999.999};

const Vector2 UT2D_VECTOR_NUL = { 0.0, 0.0 };
const Vector2 UT2D_VECTOR_X   = { 1.0, 0.0 };
const Vector2 UT2D_VECTOR_Y   = { 0.0, 1.0 };

const Vector  UT3D_VECTOR_NUL = { 0.0, 0.0, 0.0 };

const Vector  UT3D_VECTOR_X   = { 1.0, 0.0, 0.0 };
const Vector  UT3D_VECTOR_Y   = { 0.0, 1.0, 0.0 };
const Vector  UT3D_VECTOR_Z   = { 0.0, 0.0, 1.0 };

const Vector  UT3D_VECTOR_IX  = {-1.0, 0.0, 0.0 };
const Vector  UT3D_VECTOR_IY  = { 0.0,-1.0, 0.0 };
const Vector  UT3D_VECTOR_IZ  = { 0.0, 0.0,-1.0 };


const Mat_3x3 UT3D_MAT_3x3    = { {1.0, 0.0, 0.0},
                                  {0.0, 1.0, 0.0},
                                  {0.0, 0.0, 1.0} };

                                // VX   VY   VZ   PO
const Mat_4x3 UT3D_MAT_4x3    = { {1.0, 0.0, 0.0, 0.0},
                                  {0.0, 1.0, 0.0, 0.0},
                                  {0.0, 0.0, 1.0, 0.0} };

const Mat_4x4 UT3D_MAT_4x4    = { {1.0, 0.0, 0.0, 0.0},
                                  {0.0, 1.0, 0.0, 0.0},
                                  {0.0, 0.0, 1.0, 0.0},
                                  {0.0, 0.0, 0.0, 1.0} };
#endif


//================  EOF =====================
