//**************************************************************************
/*
 *
 * Copyright (C) 2015 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
 * Copyright (C) 2007 Thomas Backmeister (t.backmeister@gmx.at)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
-----------------------------------------------------
TODO:
  ..

-----------------------------------------------------
Modifications:
2004-01-23 UTM_mat_CholFact u UTM_mat_solveChol von Thomas zu. RF.

-----------------------------------------------------
*/
/*!
\file  ../ut/ut_math.c
\brief math (Cholesky, Fresnel, ..)
\code
=====================================================
List_functions_start:

UTM_mat_CholFact        Cholesky-factorization of matrix
UTM_mat_solveChol       solve Cholesky linear system
UTM_SolLinEquSys        Solve <-- linear equation system
UMT_FresnelIntgls       Fresnel integrals <-- two parameters
UTM_geomSeq_solv        Solve geometr. Reihe ae = a + a*q + a*q^2 + .. + a*q^n


-------------- polynoms ----------------------------------
UTM_zeros_cubicpol        Zeros of a cubic polynomial.
UTM_zeros_quarticpol      Zeros of a quartic polynomial. 
UTM_pol_polmultpol        multiply two polynomials
UTM_pol_Bernstein         compute Bernstein polynomials


-------------- complex numbers ---------------------------
UTM_cx_alg2pol            Complex algebraic to polar.
UTM_cx_cxaddcx            Complex addition.
UTM_cx_cxmultcx           Complex multiplication.
UTM_cx_cxsubtcx           Complex subtraction.
UTM_cx_pol2alg            Complex polar to algebraic.
UTM_cx_root               Complex root.
UTM_sign_3cx              Sign 3 complex numbers.

-------------- double numbers ----------------------------
UTM_db_bincoeff           compute binomial coefficient
UTM_db_binquot            compute binomial quotient
UTM_scale_4db             Scale down 4 double numbers

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



Liste_Funktionen_Ende:
=====================================================

\endcode *//*----------------------------------------


*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_geo.h"
#include "ut_math.h"





//===========================================================================
  int UTM_geomSeq_solv (int mode, double *a, double *ae, double *q, int *n) {
//===========================================================================
// Solve geometr. Reihe ae = a + a*q + a*q^2 + .. + a*q^n
// solve geometric sequence

// Input:
//   mode=0: compute n from a, ae,q.
//           add a*q + a*q^2 + a+q^3 until totalSum ae is reached; return n.
//   mode=1: compute a from ae, q, n.
//            ae = a * ((1-q^(n-1))/(1-q))
//            ae = a * ((q^(n+1)-1)/(q-1))
//   a        first Value=a; second Value = a * q, 3. value = a * q * q ...
//   n        Anzahl der a-Teilstuecke - 1 (siehe Beispiel)
// Output:
//   a


// Beispiel f. mode=1:
// UTM_geomSeq_solv (&a, 10., 1.2, 3);
//   ae=10;   q=1.2;   n=3     ergibt a=1.86
//   10 = 1.86 + 1.86*1.2 + 1.86*1.2*1.2 + 1.86*1.2*1.2*1.2

  int      ii, iDir;
  double   d1;


  // printf("UTM_geomSeq_solv %d a=%f ae=%f q=%f n=%d\n",mode,*a,*ae,*q,*n);


  if(mode != 0) goto L_1;

  if(*ae < 0.) iDir = 1;  
  else         iDir = 0;

  d1 = 0;
  *n = 0;
  ii = 0;

  L_0:
  ++ii;
  if(ii > 100) {TX_Print("UTM_geomSeq_solv I001"); return 0;}
  d1 += *a * pow(*q, *n);
    // printf(" UTM_geomSeq_solv %d %f\n",*n, d1);
  if(iDir) {    // verkehrt
    if(d1 > *ae) { ++(*n); goto L_0;}
  } else {
    if(d1 < *ae) { ++(*n); goto L_0;}
  }
  // ++(*n);   // add first part ..
    // printf(" ex UTM_geomSeq_solv 0 n=%d\n",*n);
  return 0;


  //----------------------------------------------------------------
  L_1:
  *a = *ae * ((*q - 1.) / (pow(*q, *n+1) - 1.));
  // *a = *ae * ((1. - pow(*q, *n-1) - 1.) / (1. - *q));   FALSCH
    // printf(" ex UTM_geomSeq_solv 1 %f\n",*a);

  return 0;

}


/*=======================================================================*/
  int UTM_zeros_cubicpol (dcomplex zero[], int zmult[], polcoeff_d3 *pol) {
/*======================
UTM_zeros_cubicpol         zeros of cubic polynomial

UTM_zeros_cubicpol       Author: Thomas Backmeister       11.3.2003

Computation of all 3 zeros of a cubic polynomial.
We use the formula of Cardano.

IN:
  polcoeff_d3 *pol ... cubic polynomial
OUT:
  dcomplex zero[]  ... all zeros of the polynomial
  int zmult[]      ... multiplicities of the zeros:
                         1,2,3  ...  for real zeros
                         -1     ...  for pure complex zeros 
                         0      ...  no more zero
Returncodes:
  0 = OK
  1 = input error
  2 = WARNING: too large numbers
*/

  int    rc;
  double r1, s1, t1, p1, q1, u1, v1, h1, h2;
  double p13, q12, r13, discr, rd, phi;

  rc = 0;

  //printf("UTM_zeros_cubicpol: zeros of cubic polynomial\n");

  // check input
  if (UTP_compdb0 (pol->a, UT_TOL_min2)) {
    printf("polynomial ist not cubic\n");
    return 1;
  }
  if ((fabs(pol->a) > UT_VAL_MAX) || (fabs(pol->b) > UT_VAL_MAX) ||
      (fabs(pol->c) > UT_VAL_MAX) || (fabs(pol->d) > UT_VAL_MAX)) {
    printf("WARNING: too large numbers\n");
    rc = 2;
  }

  // init
  zmult[0] = zmult[1] = zmult[2] = 0;

  // coefficients of normalized polynomial
  r1 = pol->b / pol->a;
  s1 = pol->c / pol->a;
  t1 = pol->d / pol->a;
  if ((fabs(r1) > UT_VAL_MAX) || (fabs(s1) > UT_VAL_MAX) ||
      (fabs(t1) > UT_VAL_MAX)) {
    printf("WARNING: too large numbers\n");
    rc = 2;
  }

  // coefficients of reduced polynomial
  p1 = (3.0 * s1 - r1 * r1) / 3.0;
  q1 = (2.0 * r1 * r1  * r1) / 27.0 - (r1 * s1) / 3.0 + t1;
  if ((fabs(p1) > UT_VAL_MAX) || (fabs(q1) > UT_VAL_MAX)) {
    printf("WARNING: too large numbers\n");
    rc = 2;
  }

  // discriminant of reduced polynomial
  p13 = p1 / 3.0;
  q12 = q1 / 2.0;
  discr = p13 * p13 * p13 + q12 * q12;
  if (fabs(discr) > UT_VAL_MAX) {
    printf("WARNING: too large number\n");
    rc = 2;
  }

  r13 = r1 / 3.0;
  if (discr == UT_DB_NULL) {
    // zero discriminant

    if ((p1 == UT_DB_NULL) && (q1 == UT_DB_NULL)) {
      // 1 real zero with multiplicity 3
      zero[0].a = -r13;
      zero[0].b = 0.0;
      zmult[0] = 3;
    }
    else
    {
      // u1
      if (q12 > 0.0) u1 = -pow (q12, (1./3.));
      else u1 = pow (fabs(q12), (1./3.));

      // 2 real zeros with multiplicity 1 resp. 2
      zero[0].a = 2.0 * u1 - r13;
      zero[0].b = 0.0;
      zmult[0] = 1;
      zero[1].a = -u1 - r13;
      zero[1].b = 0.0;
      zmult[1] = 2;
    }
  } 
  else if (discr > 0.0) {
    // positiv discriminant

    // u1 and v1
    rd = sqrt (discr);
    h1 = rd - q12;
    if (h1 > 0) u1 = pow (h1, (1./3.));
    else u1 = -pow (fabs(h1), (1./3.));
    h1 = -rd - q12;
    if (h1 > 0) v1 = pow (h1, (1./3.));
    else v1 = -pow (fabs(h1), (1./3.));

    // 1 real and 2 pure complex zeros each with multiplicity 1
    h1 = u1 + v1;
    h2 = (u1 - v1) * sqrt(3.0) / 2.0;
    zero[0].a = h1 - r13;
    zero[0].b = 0.0;
    zmult[0] = 1;
    h1 /= 2.0;
    zero[1].a = -h1 - r13;
    zero[1].b = h2;
    zmult[1] = -1;
    zero[2].a = -h1 - r13;
    zero[2].b = -h2;
    zmult[2] = -1;
  }
  else if (discr < 0.0) {
    // negativ discriminant
    // casus irreducibilis

    phi = acos (-q12 / pow (fabs(p13), 3./2.)) / 3.0; 
    h1 = 2.0 * sqrt(fabs(p13));

    // 3 real zeros with multiplicity 1
    zero[0].a = h1 * cos(phi) - r13;
    zero[0].b = 0.0;
    zmult[0] = 1;
    zero[1].a = (-h1) * cos(phi - RAD_60) - r13;
    zero[1].b = 0.0;
    zmult[1] = 1;
    zero[2].a = (-h1) * cos(phi + RAD_60) - r13;
    zero[2].b = 0.0;
    zmult[2] = 1;
  }

  /*
  // out
  for (i1=0; i1<3; ++i1) {
    if (zmult[i1] == 0) break;
    printf("zero[%d]=  %15.13f  %15.13f  | %d\n",
            i1, zero[i1].a, zero[i1].b, zmult[i1]);
  }
  */

  return rc;
}




/*=======================================================================*/
  int UTM_zeros_quarticpol (dcomplex zero[], polcoeff_d4 *pol) {
/*========================
UTM_zeros_quarticpol      zeros of quartic polynomial

UTM_zeros_quarticpol      Author: Thomas Backmeister     13.3.2003

Computation of all 4 zeros of a quartic polynomial.
The zeros are computed via cubic resolvent of the polynomial
and the formula of Cardano.

IN:
  polcoeff_d4 *pol ... quartic polynomial
OUT:
  dcomplex zero[]  ... all zeros of the polynomial
Returncodes:
  0 = OK
  1 = input error
  2 = WARNING: too large numbers
*/

  int zmult[3], i1, i2, ind, rc;
  double a2, a3, a4, b2, b3, b4, h1;
  double p1, q1, r1;
  dcomplex zero_res[3], zero3[3], cx;
  polcoeff_d3 pol_res;  

  rc = 0;

  //printf("UTM_zeros_quarticpol: zeros of quartic polynomial\n");

  // check input
  if (UTP_compdb0 (pol->a, UT_TOL_min2)) {
    printf("polynomial ist not quartic\n");
    return 1;
  }
  if ((fabs(pol->a) > UT_VAL_MAX) || (fabs(pol->b) > UT_VAL_MAX) ||
      (fabs(pol->c) > UT_VAL_MAX) || (fabs(pol->d) > UT_VAL_MAX) ||
      (fabs(pol->e) > UT_VAL_MAX)) {
    printf("WARNING: too large numbers\n");
    rc = 2;
  }

  // coefficients of reduced polynomial
  a2 = pol->a * pol->a;
  a3 = a2 * pol->a;
  a4 = a3 * pol->a;
  b2 = pol->b * pol->b;
  b3 = b2 * pol->b;
  b4 = b3 * pol->b;
  p1 = (-3.0 * b2 + 8.0 * pol->a * pol->c) / (8.0 * a2);
  q1 = (2.0 * b3 - 8.0 * pol->a * pol->b * pol->c + 16.0 * a2 * pol->d)
       / (16.0 * a3);
  r1 = (-3.0 * b4 + 16.0 * pol->a * b2 * pol->c - 64.0 * a2 * pol->b * pol->d
       + 256.0 * a3 * pol->e) / (256.0 * a4);
  if ((fabs(a4) > UT_VAL_MAX) || (fabs(b4) > UT_VAL_MAX) ||
      (fabs(p1) > UT_VAL_MAX) || (fabs(q1) > UT_VAL_MAX) ||
      (fabs(r1) > UT_VAL_MAX)) {
    printf("WARNING: too large numbers\n");
    rc = 2;
  }

  // coefficients of cubic resolvent
  pol_res.a = 1.0;
  pol_res.b = 2.0 * p1;
  pol_res.c = p1 * p1 - 4.0 * r1;
  pol_res.d = -(q1 * q1);

  // zeros of cubic resolvent
  rc = UTM_zeros_cubicpol (zero_res, zmult, &pol_res);

  // all 3 zeros separately
  ind = 0;
  for (i1=0; i1<3; ++i1) {
    if (zmult[i1] == 0) break;
    for (i2=0; i2<abs(zmult[i1]); ++i2) {
      zero3[ind] = zero_res[i1];
      ++ind;
    }  
  }

  // root of each zero
  for (i1=0; i1<3; ++i1) zero3[i1] = UTM_cx_root (&zero3[i1]);

  // sign roots of zeros such that their product has same sign as q1
  UTM_sign_3cx (&zero3[0], &zero3[1], &zero3[2], q1); 

  // zeros of the quartic polynomial
  h1 = pol->b / (4.0 * pol->a);
  cx = UTM_cx_cxsubtcx (&zero3[1], &zero3[2]);
  zero[0] = UTM_cx_cxaddcx (&cx, &zero3[0]);
  zero[0].a = 0.5 * zero[0].a - h1;
  zero[0].b *= 0.5; 
  cx = UTM_cx_cxsubtcx (&zero3[0], &zero3[1]);
  zero[1] = UTM_cx_cxaddcx (&cx, &zero3[2]);
  zero[1].a = 0.5 * zero[1].a - h1;
  zero[1].b *= 0.5;
  cx = UTM_cx_cxsubtcx (&zero3[1], &zero3[0]); 
  zero[2] = UTM_cx_cxaddcx (&cx, &zero3[2]);
  zero[2].a = 0.5 * zero[2].a - h1;
  zero[2].b *= 0.5; 
  cx.a = -zero3[0].a;
  cx.b = -zero3[0].b;
  cx = UTM_cx_cxsubtcx (&cx, &zero3[1]);
  zero[3] = UTM_cx_cxsubtcx (&cx, &zero3[2]);
  zero[3].a = 0.5 * zero[3].a - h1;
  zero[3].b *= 0.5; 

  /*
  // out
  for (i1=0; i1<4; ++i1) {
    printf("zero[%d]=  %15.13f  %15.13f\n", i1, zero[i1].a, zero[i1].b);
  }
  */

  return rc;
}




/*=======================================================================*/
  dcomplex UTM_cx_alg2pol (dcomplex *cx) {
/*=======================
UTM_cx_alg2pol           complex number algebraic -> polar

UTM_cx_alg2pol         Author: Thomas Backmeister      13.3.2003

Convert a complex number from algebraic to polar notation:
a + b*i  |-->  (sqrt(a*a+b*b), phi)
phi element of [0, RAD_360)

IN:
  dcomplex *cx  ... complex number in algebraic notation
Returnvalue:
  the complex number in polar coordinates
*/

  double phi;
  dcomplex cx_pol;

  if (UTP_compdb0 (cx->a, UT_TOL_min2)) {
    if (UTP_compdb0 (cx->b, UT_TOL_min2)) {
      cx_pol.a = 0.0;
      cx_pol.b = 0.0;
    }
    else if (cx->b > 0.0) {
      cx_pol.a = fabs(cx->b);
      cx_pol.b = RAD_90;
    }
    else {
      cx_pol.a = fabs(cx->b);
      cx_pol.b = RAD_270;
    }
  }
  else if (cx->a > 0.0) {
    cx_pol.a = sqrt(cx->a*cx->a + cx->b*cx->b);
    phi = atan(cx->b / cx->a);
    if (cx->b >= 0.0) cx_pol.b = phi;
    else cx_pol.b = phi + RAD_360;
  }
  else {
    cx_pol.a = sqrt(cx->a*cx->a + cx->b*cx->b);
    cx_pol.b = atan(cx->b / cx->a) + RAD_180;
  }

  return cx_pol;
}




/*=======================================================================*/
  dcomplex UTM_cx_pol2alg (dcomplex *cx) {
/*=======================
UTM_cx_pol2alg          complex number polar -> algebraic

UTM_cx_pol2alg       Author: Thomas Backmeister      13.3.2003

Convert a complex number from polar to algebraic notation:
(l, phi)  |-->  l * cos(phi) + l * sin(phi) * i
phi element of [0, RAD_360)

IN:
  dcomplex *cx  ... complex number in polar notation
Returnvalue:
  the complex number in algebraic notation
*/

  dcomplex cx_alg;

  cx_alg.a = cx->a * cos(cx->b);
  cx_alg.b = cx->a * sin(cx->b);

  return cx_alg;
}




/*=======================================================================*/
  dcomplex UTM_cx_root (dcomplex *cx) {
/*====================
UTM_cx_root         root of complex number

UTM_cx_root       Author: Thomas Backmeister      13.3.2003

Compute the root of a complex number.
The root is chosen such that its polar angle is in [0, RAD_180).

IN:
  dcomplex *cx  ... complex number in algebraic notation
Returnvalue:
  root of the complex number in algebraic notation
*/

  dcomplex cx_pol, cx_root;

  // complex number in polar notation
  cx_pol = UTM_cx_alg2pol (cx);

  // root in polar notation
  cx_pol.a = sqrt(cx_pol.a);
  cx_pol.b /= 2.0;

  // root in algebraic notation
  cx_root = UTM_cx_pol2alg (&cx_pol);

  return cx_root;
}




/*=======================================================================*/
  int UTM_sign_3cx (dcomplex *cx1, dcomplex *cx2, dcomplex *cx3, double q) {
/*================
UTM_sign_3cx       sign 3 complex numbers

UTM_sign_3cx      Author: Thomas Backmeister      13.3.2003

Sign 3 complex numbers such that the real part of their product
has the same sign as q.
If q or the real part of the product is zero, nothing is done.

IN:
  dcomplex *cx1 ... 1. complex number in algebraic notation
  dcomplex *cx2 ... 2. complex number in algebraic notation
  dcomplex *cx3 ... 3. complex number in algebraic notation
  double q      ... a double number
OUT:
  dcomplex *cx1 ... 1. complex number in algebraic notation
                    (possibly changed by a multiplication with -1)
Returncode:
  0 = OK
*/

  dcomplex cx;

  if (UTP_compdb0 (q, UT_TOL_min2)) return 0;
  cx = UTM_cx_cxmultcx (cx1, cx2);
  cx = UTM_cx_cxmultcx (&cx, cx3);
  if (q > 0.0) {
    if (cx.a < 0.0) {
      cx1->a = -cx1->a;
      cx1->b = -cx1->b;
    }
  }
  if (q < 0.0) {
    if (cx.a > 0.0) {
      cx1->a = -cx1->a;
      cx1->b = -cx1->b;
    }
  }
  
  return 0;
}




/*=======================================================================*/
  dcomplex UTM_cx_cxaddcx (dcomplex *cx1, dcomplex *cx2) {
/*=======================
UTM_cx_cxaddcx       add 2 complex numbers

UTM_cx_cxaddcx       Author: Thomas Backmeister      13.3.2003

Add 2 complex numbers.

IN:
  dcomplex *cx1 ... 1. complex number in algebraic notation
  dcomplex *cx2 ... 2. complex number in algebraic notation
Returnvalue:
  sum of the 2 complex numbers in algebraic notation
*/

  dcomplex cx_sum;

  cx_sum.a = cx1->a + cx2->a;
  cx_sum.b = cx1->b + cx2->b;
  
  return cx_sum;
}




/*=======================================================================*/
  dcomplex UTM_cx_cxsubtcx (dcomplex *cx1, dcomplex *cx2) {
/*========================
UTM_cx_cxsubtcx       subtract 2 complex numbers

UTM_cx_cxsubtcx       Author: Thomas Backmeister      13.3.2003

Subtract 2 complex numbers.

IN:
  dcomplex *cx1 ... 1. complex number in algebraic notation
  dcomplex *cx2 ... 2. complex number in algebraic notation
Returnvalue:
  difference of the 2 complex numbers in algebraic notation
*/

  dcomplex cx_diff;

  cx_diff.a = cx1->a - cx2->a;
  cx_diff.b = cx1->b - cx2->b;
  
  return cx_diff;
}




/*=======================================================================*/
  dcomplex UTM_cx_cxmultcx (dcomplex *cx1, dcomplex *cx2) {
/*========================
UTM_cx_cxmultcx       multiplicate 2 complex numbers

UTM_cx_cxmultcx       Author: Thomas Backmeister      13.3.2003

Multiplicate 2 complex numbers.

IN:
  dcomplex *cx1 ... 1. complex number in algebraic notation
  dcomplex *cx2 ... 2. complex number in algebraic notation
Returnvalue:
  product of the 2 complex numbers in algebraic notation
*/

  dcomplex cx_prod;

  cx_prod.a = cx1->a * cx2->a - cx1->b * cx2->b;
  cx_prod.b = cx1->a * cx2->b + cx1->b * cx2->a;
  
  return cx_prod;
}




/*=======================================================================*/
  int UTM_scale_4db (double *scf, double *d1, double *d2, double *d3,
                     double *d4, double lim) {
/*=================
UTM_scale_4db        scale down 4 double numbers

UTM_scale_4db        Author: Thomas Backmeister      21.3.2003

Scale down 4 double numbers.
scf is such that fabs(di) * scf <= lim for all i = 1,2,3,4.

IN:
  double *d1  ... double number 1
  double *d2  ... double number 2
  double *d3  ... double number 3
  double *d4  ... double number 4
  double lim  ... positiv limit value
OUT:
  double *scf ... positiv scaling factor
  double *d1  ... scaled down double number 1
  double *d2  ... scaled down double number 2
  double *d3  ... scaled down double number 3
  double *d4  ... scaled down double number 4
Returncode:
  0 = OK
*/

  int nd;
  double d;

  // printf("UTM_scale_4db\n");

  // init
  *scf = 1.0;

  d = 10.0;
  nd = 0;
  for (;;) {
    if ((fabs(*d1) > lim) || (fabs(*d2) > lim) ||
        (fabs(*d3) > lim) || (fabs(*d4) > lim)) {
      *d1 /= d;
      *d2 /= d;
      *d3 /= d;
      *d4 /= d;
      ++nd;
    }
    else break;
  }
  if (nd > 0) *scf = 1.0 / pow (d,(double)nd);  
    
  return 0;
}



/*=======================================================================*/
  double UTM_db_bincoeff (long n, long k) {
/*======================
UTM_db_bincoeff        compute binomial coefficient

UTM_db_bincoeff        Author: Thomas Backmeister      2.6.2003

Computation of binomial coefficient of two long numbers.
Binomial coefficient of n over k := n! / (k! * (n-k)!)

IN:
  long n  ... number 1
  long k  ... number 2
OUT:
Returncode:
  binomial coefficient of n over k.
*/

  long i1;
  double d;

  if (n < 0 || k < 0 || n < k) return 0;
  if (k == 0) return 1;
  if (k == n) return 1;
  if (n == 0) return 1;
  d = 1.0;
  for (i1=1; i1<=n-k; ++i1) {
    d *= (double)(k + i1) / (double)i1;
  }
  if (d > UT_VAL_MAX) {
    printf("UTM_db_bincoeff: !ATTENTION!: very large value:\n");
    printf("  ===> %f from %ld %ld <===\n",d,n,k);
  }

  return d;
}


/*=======================================================================*/
  double UTM_db_binquot (long n, long i, long j) {
/*=====================
UTM_db_binquot         compute binomial quotient

UTM_db_binquot         Author: Thomas Backmeister      3.6.2003

Computation of quotient of binomial coefficients: (i over j) / (n over j)

IN:
  long n  ... number 1  (0 <= n)
  long i  ... number 2  (0 <= i <= n)
  long j  ... number 3  (0 <= j <= i)
OUT:
Returncode:
  0 <= binomial quotient <= 1
*/

  long i1, k;
  double d;

  if (n < 0) return 0;
  if (i < 0 || i > n) return 0;
  if (j < 0 || j > i) return 0;
  d = 1.0;
  for (i1=1; i1<=n-i; ++i1) {
    k = i + i1;
    d *= (double)(k - j) / (double)k;
  }

  return d;
}



/*=======================================================================*/
  int UTM_pol_polmultpol (double *polm, int deg1, double *pol1,
                                        int deg2, double *pol2) {
/*======================
UTM_pol_polmultpol       multiply two polynomials

UTM_pol_polmultpol       Author: Thomas Backmeister      31.5.2003

Multiplication of two polynomials.

Polynomial 1: pol1[0] * X**0 + pol1[1] * X**1 + ... + pol1[deg1] * X**deg1
Polynomial 2: pol2[0] * X**0 + pol2[1] * X**1 + ... + pol2[deg2] * X**deg2

Attention: for polm (deg1+deg2+1)*sizeof(double) memory space must be provided !
The calling function can use variable polm for pol1 and pol2.

IN:
  int deg1     ... degree of polynomial 1
  double *pol1 ... coefficients of polynomial 1 (number = deg1 + 1)
  int deg2     ... degree of polynomial 2
  double *pol2 ... coefficients of polynomial 2 (number = deg2 + 1)
OUT:
  double *polm ... coefficients of resulting polynomial
                   (number of coefficients = deg1 + deg2 + 1)
Returncode:
  0 = OK
*/

  int    i1, j1, k1, l1;
  double *pol;


  // Testausg:
  // printf("UTM_pol_polmultpol %d %d\n",deg1,deg2);
  // for(i1=0; i1<=deg1; ++i1)  printf(" pol1[%d]=%f\n",i1,pol1[i1]);
  // for(i1=0; i1<=deg2; ++i1)  printf(" pol2[%d]=%f\n",i1,pol2[i1]);


  // number of coefficients of polm
  l1 = deg1 + deg2 + 1;

  // allocate memory
  pol = (double*) malloc (l1 * sizeof(double));

  // product of polynomials
  for (k1=0; k1<l1; ++k1) {
    pol[k1] = 0.0;
    for (i1=0; i1<=k1; ++i1) {
      if (i1 > deg1) break;
      j1 = k1 - i1;
      if (j1 > deg2) continue;
      pol[k1] += pol1[i1] * pol2[j1];
    }
  }

  // copy result
  for (k1=0; k1<l1; ++k1) {
    polm[k1] = pol[k1];
  }

  // free memory
  if (pol != NULL) free (pol);

  return 0;
}



/*=======================================================================*/
  int UTM_pol_Bernstein (Polynom_ *polB) {
/*=====================
UTM_pol_Bernstein       compute Bernstein polynomials

UTM_pol_Bernstein       Author: Thomas Backmeister      31.5.2003

Computation of coefficients of Bernstein polynomials B(i,deg) of degree deg.
The number of Bernstein polynomials is deg + 1,
and each Bernstein polynomial has deg + 1 coefficients:
i.e. for i = 0, ..., deg:
B(i,deg) = polB[i][0] * X**0 + .... + polB[i][deg] * X**deg

Attention: for polB[][] (deg+1)*(deg+1)*sizeof(double) memory space must
           be provided !

IN:
  int deg         ... degree of Bernstein polynomials
OUT:
  double polB[][] ... coefficients of Bernstein polynomials
Returncodes:
  0 = OK
 -1 = input error
*/

  int    i1, i2, i3, deg, lr;
  double pol1X[2];



  // printf("UTM_pol_Bernstein deg1=%d deg2=%d\n",polB->dim1,polB->dim2);

  deg = polB->dim2 - 1;

  // check degree
  if (deg < 0) {
    printf("UTM_pol_Bernstein: negativ degree!\n");
    return -1;
  }
  if (deg > UT_BEZDEG_MAX) {
    printf("UTM_pol_Bernstein: degree of Bernstein polynomials too large!\n");
    return -1;
  }

  lr = polB->dim2;  // Recordlaenge 

  // init
  i3=0;
  for (i1=0; i1<=deg; ++i1) {
    for (i2=0; i2<=deg; ++i2) {
      polB->pol[i3] = 0.0;
      ++i3;
    }
  }
  polB->pol[0] = 1.0;



  // polynomial 1-X
  pol1X[0] = 1.0;
  pol1X[1] = -1.0;


  // compute Bernstein polynomials
  for (i1=1; i1<=deg; ++i1) {
    for (i2=i1; i2>=0; --i2) {
      if (i2 > 0) {
        // (1-X) * polB[i2][]
        // UTM_pol_polmultpol (&polB[i2][0], 1, pol1X, i1-1, &polB[i2][0]);
        UTM_pol_polmultpol (&polB->pol[i2*lr],1,pol1X,i1-1,&polB->pol[i2*lr]);

        // + X * polB[i2-1][]
        for (i3=1; i3<=i1; ++i3) {
          // polB[i2][i3] += polB[i2-1][i3-1];
          polB->pol[i2*lr+i3] += polB->pol[(i2-1)*lr+(i3-1)];
        }
      }
      else {
        // (1-X) * polB[i2][]
        // UTM_pol_polmultpol (&polB[i2][0], 1, pol1X, i1-1, &polB[i2][0]);
        UTM_pol_polmultpol (&polB->pol[i2*lr],1,pol1X,i1-1,&polB->pol[i2*lr]);
      }
    }
  }


  return 0;
}


/*=======================================================================*/
  int UTM_mat_CholFact (double *w, int nbands, int nrow, double *diag) {
/*====================
UTM_mat_CholFact     Cholesky-factorization of matrix

UTM_mat_CholFact     Author: Thomas Backmeister       2.12.2003

Cholesky-factorization of a symmetric, positiv-semidefinite banded matrix.
The factorization has the form C = L*D*L-transpose, where L is a unit
lower triangle matrix (i.e. 1's on the main diagonal) and D is a diagonal
matrix.

IN:
  double *w         ... contains the diagonals of the matrix:
                        w[0,j] = main diagonal, j=0,...,nrow-1
												w[i,j] = i-th diagonal, j=0,...,nrow-1, i=1,...,nbands-1
  int nbands        ... number of diagonals of the matrix
  int nrow          ... order of the matrix (i.e. matrix = nrow x nrow)
  double *diag      ... work array (length = nrow)	
OUT:
  double *w         ... contains the Cholesky-factorization:
	                      w[0,j] = 1.0 / D(j,j), j=0,...,nrow-1
                        w[i,j] = L(i-1+j,j), j=0,...,nrow-i-1, i=1,...,nbands-1
Returncode:
  0 = OK
*/

  int i, j, n, imax, jmax;
	double ratio;
					
  //printf("UTM_mat_CholFact: Cholesky-factorization of matrix\n");

  if (nrow <= 1) {
		if (w[0] > 0.0) w[0] = 1.0/w[0];
    return 0;
  }		

  // store the diagonal
	for (i=0; i<nrow; ++i) diag[i] = w[i];

	// factorization
	//
	for (n=0; n<nrow; ++n) {
    if (w[n]+diag[n] <= diag[n]) {
			for (j=0; j<nbands; ++j) w[j*nrow+n] = 0.0;
		}
		else {
		  w[n] = 1.0 / w[n];
			imax = IMIN(nbands-1, nrow-n-1);
			jmax = imax;
			for (i=0; i<imax; ++i) {
				ratio = w[(i+1)*nrow+n] * w[n];
	      for (j=0; j<jmax; ++j) w[j*nrow+n+i+1] -= w[(j+i+1)*nrow+n] * ratio;
				--jmax;
				w[(i+1)*nrow+n] = ratio;
			}
		}
	}

	return 0;
}



/*=======================================================================*/
  int UTM_mat_solveChol (double *b, double *w, int nbands, int nrow) {
/*=====================
UTM_mat_solveChol     solve Cholesky linear system

UTM_mat_solveChol     Author: Thomas Backmeister       2.12.2003

Compute the solution of a banded, symmetric, positive definite system.
The system is of the form C*X=B, where C = L*D*L-transpose, L a unit
lower triangular and D a triangular matrix.

IN:
  double *w         ... contains the Cholesky-factorization:
	                      w[0,j] = 1.0 / D(j,j), j=0,...,nrow-1
                        w[i,j] = L(i-1+j,j), j=0,...,nrow-i-1, i=1,...,nbands-1
  int nbands        ... number of diagonals of the matrix
  int nrow          ... order of the matrix (i.e. matrix = nrow x nrow)
	double *b         ... right side of the system (nrow elements)
OUT:
  double *b         ... solution of the system
Returncode:
  0 = OK
*/

	int n, m, j;

  //printf("UTM_mat_solveChol: solve Cholesky linear system\n");

  if (nrow <= 1) {
		b[0] *= w[0];
		return 0;
	}	

  // forward substitution; solve L * Y = B
	for (n=0; n<nrow; ++n) {
		m = IMIN(nbands-1, nrow-n-1);
		for (j=0; j<m; ++j) b[j+n+1] -= w[(j+1)*nrow+n] * b[n];
  }

	// backsubstitution; solve L-transp * X = D**(-1) * Y
  for (n=nrow-1; n>=0; --n) {
		b[n] *= w[n];
		m = IMIN(nbands-1, nrow-n-1);
    for (j=0; j<m; ++j) b[n] -= w[(j+1)*nrow+n] * b[j+n+1];
  }

	return 0;
}





/*=====================================================================*/
  int UTM_SolLinEquSys (double *vloes, int anz_var, double *mata,
		        double *matb) {
/*====================
UTM_SolLinEquSys         Solve <-- linear equation system

UTM_SolLinEquSys          Author: Thomas Backmeister          8.8.2007

Solve a system of linear equations in at most 100 unknowns.
e.g. anz_var = 3:
mata[0][0]*vloes[0] + mata[0][1]*vloes[1] + mata[0][2]*vloes[2] = matb[0]
mata[1][0]*vloes[0] + mata[1][1]*vloes[1] + mata[1][2]*vloes[2] = matb[1]
mata[2][0]*vloes[0] + mata[2][1]*vloes[1] + mata[2][2]*vloes[2] = matb[2]
The system is assumed to have a unique solution (regular matrix).

(The origin of this function is ucp_gauss, TB, 18.9.1997).

IN:
  int anz_var   ... number of variables
  double *mata  ... coefficients of linear equations
  double *matb  ... constant vector
OUT:
  double *vloes ... solution vector
Returncodes:
  0 = solution found
 -1 = input error
 -2 = no unique solution
*/

  int    i1, j1, k1, ind[100];
  int    indi, indj, hv;
  double a1[100][100], b1[100], x1[100];
  double pivmax, ddd, rhilf;

  //printf("UTM_SolLinEquSys: Solve <-- linear equation system\n");

  // check input
  if (anz_var < 1 || anz_var > 100) goto L_InputErr; 
  hv = anz_var - 1;

  // special case
  if (anz_var == 1) {
    if (mata[0] == 0.0) goto L_SingMat;
    vloes[0] = matb[0] / mata[0];
    return 0;
  }

  // copy matrix
  for (j1=0; j1<anz_var; ++j1) {
    ind[j1] = j1;
    for (i1=0; i1<anz_var; ++i1) a1[i1][j1] = mata[i1*anz_var+j1];
    b1[j1] = matb[j1];
  }

  // compute upper triangle matrix
  // -----------------------------
  for (i1=0; i1<hv; ++i1) {
    pivmax = 0.0;
    for (j1=i1; j1<anz_var; ++j1) {
      for (k1=i1; k1<anz_var; ++k1) {
        ddd = fabs (a1[j1][k1]);
        if (ddd > pivmax) {
          pivmax = ddd;
          indi = j1;
          indj = k1;
        }
      }
    }
    if (pivmax == 0.0) goto L_SingMat;

    // switch rows
    if (indi != i1) { 
      for (j1=i1; j1<anz_var; ++j1) {
        rhilf = a1[indi][j1];
        a1[indi][j1] = a1[i1][j1];
        a1[i1][j1] = rhilf;
      }
      rhilf = b1[indi];
      b1[indi] = b1[i1];
      b1[i1] = rhilf; 
    }

    // switch columns
    if (indj != i1) { 
      for (j1=0; j1<anz_var; ++j1) {
        rhilf = a1[j1][indj];
        a1[j1][indj] = a1[j1][i1];
        a1[j1][i1] = rhilf;
      }
      j1 = ind[indj];
      ind[indj] = ind[i1];
      ind[i1] = j1;
    }

    for (j1=i1+1; j1<anz_var; ++j1) {
      rhilf = a1[j1][i1] / a1[i1][i1];
      b1[j1] = b1[j1] - b1[i1] * rhilf;
      for (k1=i1+1; k1<anz_var; ++k1) {
        a1[j1][k1] = a1[j1][k1] - a1[i1][k1] * rhilf;
      }
    }
  }

  // compute values of variables
  if (UTP_comp_0(a1[hv][hv])) goto L_SingMat;
  x1[hv] = b1[hv] / a1[hv][hv];
  for (i1=anz_var-2; i1>=0; --i1) {
    rhilf = 0.0;
    for (j1=i1+1; j1<anz_var; ++j1) rhilf = rhilf + a1[i1][j1] * x1[j1];
    x1[i1] = (b1[i1] - rhilf) / a1[i1][i1];
  }

  // copy solution
  for (i1=0; i1<anz_var; ++i1) vloes[ind[i1]] = x1[i1];

  return 0;

L_InputErr:
  TX_Error("UTM_SolLinEquSys: input error");
  return -1;

L_SingMat:  
  TX_Error("UTM_SolLinEquSys: no unique solution");
  return -2;
}


/*=====================================================================*/
  int UMT_FresnelIntgls (double *fci, double *fsi, double t1, double t2) {
/*=====================
UMT_FresnelIntgls         Fresnel integrals <-- two parameters

UMT_FresnelIntgls          Author: Thomas Backmeister        17.9.2007

Computation of the Fresnel integrals
FCI(t1,t2) := Integral(t1,t2) [cos(pi*u*u/2)*du] and
FSI(t1,t2) := Integral(t1,t2) [sin(pi*u*u/2)*du]
The integration parameters t1,t2 are non-negativ and unequal.
The integrals are computed with Simpson's approximation formula.

IN:
  double t1    ... 1. paramter (>=0)
  double t2    ... 2. paramter (>=0, !=t1)
OUT:
  double *fci  ... value of FCI(t1,t2)
  double *fsi  ... value of FSI(t1,t2)
Returncode:
  0 = OK
*/

// maximal parameter-step within [t1,t2] resp. [t2,t1];
// determines the accuracy of the integrals;
// as smaller MAX_FRESNEL as more precise the computation!
#define MAX_FRESNEL 0.00001
	                    
  int ns, i1;
  double ls, u, u1, v;

  //printf("UMT_FresnelIntgls: Fresnel integrals <-- two parameters\n");

  // ns = number of segments of [t1,t2] resp. [t2,t1];
  // fabs(ls) = length of 1 segment
  u = t2 - t1;
  ls = u / 100.0;
  if (ls > 0.0) {
    if (ls > MAX_FRESNEL) ls = MAX_FRESNEL;
  }
  else {
    if (-ls > MAX_FRESNEL) ls = -MAX_FRESNEL;
  }	  
  ns = (int)(u / ls); 
  if (ns%2 != 0) {
    ns += 1;
    ls = u / (double)ns;
  }    

  // value at t1
  v = RAD_90 * t1 * t1;
  *fci = cos(v);
  *fsi = sin(v);

  // + sum of values within (t1,t2) resp. (t2,t1)
  u1 = t1;
  for (i1=1; i1<ns; ++i1) {
    u1 += ls;
    v = RAD_90 * u1 * u1;
    if (i1 % 2 == 1) {
      *fci += (4.0 * cos(v));
      *fsi += (4.0 * sin(v));
    }  
    else {
      *fci += (2.0 * cos(v));
      *fsi += (2.0 * sin(v));
    }  
  }	  

  // + value at t2 
  v = RAD_90 * t2 * t2;
  *fci += cos(v);
  *fsi += sin(v);

  // final result
  v = ls / 3.0;
  *fci *= v;
  *fsi *= v;

  return 0;
}


/*======================== EOF ======================================*/
