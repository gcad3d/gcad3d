/***************************************************************************
                          ut_const.h  -  description
                             -------------------
    begin                : Fri Aug 25 2000
    copyright            : (C) 2000 by CADCAM-Services Franz Reiter
    email                : franz.reiter@cadcam.co.at

****************************************************************************

*//*!
\file ../ut/ut_const.h
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
// needs ../ut/ut_types.h

// constants:
// see also <float.h> <limits.h>
const int    UT_INT_MAX  = 999999999;
const double UT_VAL_MIN  = -FLT_32_MAX;
const double UT_VAL_MAX  =  FLT_32_MAX;
const double UT_DB_LEER  = -99999999.99999999;
const double UT_DB_NULL  = 0.0;
const char   UT_CHR_NULL = '\0';


//================  EOF =====================
