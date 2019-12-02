/***************************************************************************
                          ut_uti.h  -  description
                             -------------------
    begin                : Fri Aug 25 2000
    copyright            : (C) 2017 by CADCAM-Services Franz Reiter
    email                : franz.reiter@cadcam.co.at

 ***************************************************************************


-------------------------------------------
Korr:
2018-01-15  extracted from ../ut/ut_geo.h. RF.


-------------------------------------------
*//*!
\file ../ut/ut_uti.h
\ingroup grp_ut_geom
*/




//================================================================
// prototypes

//----------------------------------------------------------------
 int     UTP_comp_0  (double);
 int     UTP_compdb0 (double, double);
 int     UTP_comp2x2db (double d11,double d12,double d21,double d22,double tol);
 int     UTP_db_ck_in2db (double v, double v1, double v2);
 int     UTP_db_ck_in2dbTol (double v, double v1, double v2, double tol);
 int     UTP_db_cknear_2db (double *db, double *d1, double *d2);
 int     UTP_db_cknear_ndb (double db1, int dbNr, double *dbTab);
 double  UTP_min_d3 (double *d1, double *d2, double *d3);
 double  UTP_max_d3 (double *d1, double *d2, double *d3);
 double  UTP_db_rnd1sig (double);
 double  UTP_db_rnd2sig (double);
 UINT_32 UTI_checksum__ (void *buf, int bufLen);


//================================================================
// inline functions


// I_BIN                  get int from binary  
// Example: i1 = I_BIN(110);  // sets i1 = 6
#define I_BIN(iBin) 0b##iBin


#define IABS(i)    (((i)<0)?-(i):(i))         ///< abs(int); always positive
#define ISIGN(i)   ((i>=0)?(1):(-1))          ///< sign of int; +1 or -1

/// change 0 -> 1, 1 -> 0;                   i1 = ICHG01 (i1);
#define ICHG01(i)  (((i)>0)?(0):(1))

// ICHG0-1                   change 0 > -1, 1 > -2, 2 > -3 
#define ICHG0x1(ii) ((ii * -1) - 1)


/// IMOD      int - division with remainder
/// Example: IMOD(&in, &ir, 43, 10); // returns in=4, ir=3
int IMOD (int *iNr, int *iRem, int ival, int idiv);
#define IMOD(iNr,iRem,ival,idiv){\
 *(iNr) = ival / idiv;\
 *(iRem) = ival - *(iNr) * idiv;}\


/// IMIN: return the smaller of 2 values
/// IMIN (val2,val2)     return val1 if val1<val2 else return val2
#define IMIN(x,y)  (((x)<(y))?(x):(y))

/// IMAX: return the bigger of 2 values
/// IMAX (val1,val2)     return val1 if val1>val2 else return val2
#define IMAX(x,y)  (((x)>(y))?(x):(y))


/// ILIM01                   returns 0 (ii <= 0) or 1 (ii >= 1)
#define ILIM01(ii) (((ii) > 0)?1:0)


/// ILIM0x1                   returns 0 (ii >= 0) or -1 (ii <= -1)
#define ILIMm10(ii) (((ii) < 0)?-1:0)


/// ILIM0X                    returns x = between 0 and hi
#define ILIM0X(x,hi) (((x)>(hi))?(hi):(((x)<0)?0:(x)))
/// \code
/// ii = ILIM2 (i1, 10, 20);
///   returns 10 if i1<10; else returns 20 if i1>20; else returns i1.
/// \endcode


/// ILIM2                     returns x = between lo and hi
#define ILIM2(x,lo,hi) (((x)>(hi))?(hi):(((x)<(lo))?(lo):(x)))
/// \code
/// ii = ILIM2 (i1, 10, 20);
///   returns 10 if i1<10; else returns 20 if i1>20; else returns i1.
/// \endcode


/// ILIMCK1                   check if x is between 0 and lim
/// \code
///   returns 0 if ix is between 0 and lim; else 1.
/// ii = ILIMCK1 (x, 10);
/// \endcode
#define ILIMCK1(x,lim) (((x)>(lim))?(1):(((x)<(0))?(1):(0)))


/// ILIMCK2                   check if x is between hi and lo
/// \code
///   returns 0 if ix is between iHi and iLo; else 1.
///   lo hi must be ascending. For int and double.
/// ii = ILIMCK2 (x, 10, 20);
/// \endcode
#define ILIMCK2(x,lo,hi) (((x)>(hi))?(1):(((x)<(lo))?(1):(0)))


/// I_XOR_2I                        XOR exclusive or;
/// \code
///  0,0 -> 0;                       printf(" %d\n",I_XOR_2I(0,0));
///  0,1 -> 1;
///  1,0 -> 1;
///  1,1 -> 0;
/// \endcode
#define I_XOR_2I(i1,i2)i1 ^ i2


/// UTN_LIMCK__                  check if x is between v1 and v2
/// \code
///   returns 0 if x is between v1 and v2; else 1 (not between v1 and v2).
///   lo hi can be ascending or descending. For int and double.
/// d1 = UTN_LIMCK__ (x, 1., 2.);
/// \endcode
// ? for ascending; : for descending.
#define UTN_LIMCK__(x,v1,v2) (((v1)<(v2))\
?(((x)>(v2))?(1):(((x)<(v1))?(1):(0)))\
:(((x)>(v1))?(1):(((x)<(v2))?(1):(0))))


///  char --> int;   eg get 1 from '1'
///   same as (int = charDig - 48) or (int = charDig - '0')
#define ICHAR(x) ((x) & 15)


// UTI_round_4up              round integer up to 4
//   eg 2 -> 4; 4 -> 4;  5 -> 8;
#define UTI_round_4up(ii) (ii + 3) & ~0x3


// UTI_round_32up              round integer up to 32
//   eg 2 -> 32; 14 -> 32;  60 -> 64; 1036 -> 1056
#define UTI_round_32up(ii) ii + 32 & ~31


// UTI_I32_2I16                get int from 2 shorts
int UTI_I32_2I16 (short hi, short lo);
#define UTI_I32_2I16(hi,lo) ((hi<<16)+lo)


// UTI_hiI16_I32               get hi (left) short out of 32-bit-int
int UTI_hiI16_I32 (int i32);
#define UTI_hiI16_I32(i32) (i32>>16)


// UTI_loI16_I32               get lo (right) short out of 32-bit-int
int UTI_loI16_I32 (int i32);
#define UTI_loI16_I32(i32) (i32&65535)


// UTI_i2_sort               sort 2 integers
void UTI_i2_sort (int*,int*);
#define UTI_i2_sort(i1,i2)if(*i1>*i2){\
  int _i3 = *i1; *i1 = *i2; *i2 = _i3;}


/// \brief UTI_iNr_chrNr       give nr of ints for n characters (not including \0)
/// \code
///   makes 2 from 4  (4 chars + \0 needs 2 ints to store in int-words)
/// \endcode
#define UTI_iNr_chrNr(i1) (((i1) >> 2) + 1)   // (((i1) + 4) / 4)

/// FDABS                     absolute value of float
#define FDABS(df) ((df>=0.f)?(df):(-df))

#define DMIN(x,y)  (((x)<(y))?(x):(y))
#define DMAX(x,y)  (((x)>(y))?(x):(y))


/// DMOD      double - division with remainder
/// Example: DMOD(&in, &dr, 43., 10.); // returns in=4, dr=3.
double DMOD (int *iNr, double *dRem, double dval, double ddiv);
#define DMOD(iNr,dRem,dval,ddiv){\
  *(dRem) = dval / ddiv;\
  *(iNr) = *(dRem);\
  *(dRem) -= *(iNr) * ddiv;}
// *(dRem) = remquo (dval, ddiv, iNr);}  DOES ROUNDING !


/// DLIM01                     0 if (d1 >= 0.); 1 if (d1 < 0.)
int DLIM01 (double dd);
#define DLIM01(dd) ((dd >= 0.)?0:1)
/// \code
/// i1 = DLIM01 (d1);
///   returns 0 if (d1 >= 0.); else returns 1 if (d1 < 0.)
/// \endcode


/// DLIM10                     1 if (d1 >= 0.); 0 if (d1 < 0.)
int DLIM10 (double dd);
#define DLIM10(dd) ((dd >= 0.)?1:0)
/// \code
/// i1 = DLIM10 (d1);
///   returns 1 if (d1 >= 0.); else returns 0 if (d1 < 0.)
/// \endcode



/// DLIM2                     returns x = between lo and hi
double DLIM2 (double, double, double);
#define DLIM2(x,lo,hi) (((x)>(hi))?(hi):(((x)<(lo))?(lo):(x)))
/// \code
/// di = DLIM2 (d1, 0., 10.);
///   returns 0 if d1<0; else returns 10 if d1>10; else returns d1.
/// \endcode

/// DLIM3                     test if x outside lo - hi
int DLIM3 (double, double, double);
#define DLIM3(x,lo,hi) (((x)>(hi))?(1):(((x)<(lo))?(-1):(0)))
/// \code
/// returns -1 if x < lo; else returns 1 if x > hi; else returns 0
/// \endcode

/// DSIGN                     sign of double; +1 or -1
int DSIGN (double);
#define DSIGN(d)   ((d>=0.)?(1):(-1))
// see also DLIM01 

/// DSIGTOL                              sign of double with tolerance (-1|0|1)
int DSIGTOL (double, double);
#define DSIGTOL(dd,tol) ((dd>tol)?(1):((dd<-(tol))?(-1):(0)))
/// \code
/// replacing code:
///   if (fabs(dd) < tol) rc =  0;
///   else if (dd < 0.)   rc =  1;
///   else                rc = -1;
/// \endcode


//----------------------------------------------------------------
/// \brief UTP_comp_0             compare double (double == 0.0 + - UT_TOL_min1)
/// \code
/// Retcode 0:   db <> 0.0
///         1:   db = 0.0
/// UTP_comp_0 (1.);   // returns 0  (false, not equal to 0.0)
/// UTP_comp_0 (0.);   // returns 1  (true, is equal 0)
/// see also UTP_db_comp_0
/// \endcode
int UTP_comp_0 (double);
#define UTP_comp_0(db) (fabs(db) < UT_TOL_min1)


/// \brief UTP_comp2db               compare 2 doubles (with tolerance)
/// \code
/// Retcode 0 = Differenz der Werte > tol   - different
/// Retcode 1 = Differenz der Werte < tol   - ident
/// \endcode
int UTP_comp2db (double d1, double d2, double tol);
#define UTP_comp2db(d1,d2,tol) (fabs(d2-d1) < (tol))


/// \brief UTP_px_paramp0p1px        value_x from valStart, valEnd, param_x
/// \code
/// see also UTP_param_p0p1px
/// param_x   between 0. to 1.
/// Example: p0=5, p1=10, par=0.5; retVal=7.5.
/// \endcode
double UTP_px_paramp0p1px (double, double, double);
#define UTP_px_paramp0p1px(p0,p1,par)\
  ((p1) - (p0)) * (par) + (p0);


double UTP_db_comp_0 (double);
/// UTP_db_comp_0              if fabs(d1) < UT_TOL_min1) d1 = 0.;
#define UTP_db_comp_0(d1) ((fabs(d1) < UT_TOL_min1) ? 0.0 : d1)


// UTP_compdb0               compare double mit 0.0 mit Tol.
// 
// Retcode 0:   db <> 0.0 (gr. oder kl. tol)
//         1:   db = 0.0 (innerhalb tol)
int UTP_compdb0 (double db, double tol);
#define UTP_compdb0(db,tol)  (((fabs(db))<(tol))?(1):(0))


//----------------------------------------------------------------
// sqrt(0) makes -nan
// SQRT_12 - multiply first and second parameter
double SQRT_12 (double, double, double);
#define SQRT_12(dd,d1,d2){\
 dd = (d1*d1) - (d2*d2);\
 dd = fabs(dd) > UT_TOL_min1 ? sqrt(dd) : 0.;}


// sqrt(0) makes -nan
double SQRT__ (double);
#define SQRT__(dd) (fabs(dd) > UT_TOL_min1 ? sqrt(dd) : 0.)




double ACOS (double);
#define ACOS(dCos) ((dCos>=1.)?(0.):((dCos<=-1.)?(RAD_180):acos(dCos)))



//----------------------------------------------------------------
// set/clr/get bits in byte|short|int|long; see also ../ut/ut_BitTab.h

/// BIT_SET                 set bits;    BITSET(data,value)
/// data:        byte|short|int|long
/// data,value:  value of bit to set (1|2|4..)
int BIT_SET (int, int);
#define BIT_SET(i,b) (i)|=(b)

/// BIT_CLR                 clear bits;  BITCLR(data,value)
/// data:        byte|short|int|long
/// data,value:  value of bit to test (1|2|4..)
/// Example: BITCLR(i1,3);    // clear bit-0 and bit-1 of i1
int BIT_CLR (int, int);
#define BIT_CLR(i,b) (i)&=~(b)

/// BIT_GET                 filter bits;  BITGET(data,value)
/// data:        byte|short|int|long to test
/// data,value:  value of bit to test (1|2|4..)
/// RetCod: 0 (not set) or value (set)
int BIT_GET (int, int);
#define BIT_GET(i,b) ((i)&(b))


// EOF
