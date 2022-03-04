//                                      2005-04-19    Reiter Franz
/*
 *
 * Copyright (C) 2015 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
  ..

-----------------------------------------------------
*/
#ifdef globTag
void UTI(){}
void UTP(){}
#endif
/*!
\file  ../ut/ut_uti.c
\brief auxiliary functions for integers, doubles (UTI_ UTP_ UTN_)
\code
=====================================================
List_functions_start:

--------- Addresses (Pointers)
UTA_reallTab              reallocate a table of pointers       UNFERTIG !

--------- integers
IMAX                      INLINE
IMIN                      INLINE
IABS                      INLINE
IMOD                      int - division with remainder (modul0)         INLINE
ISIGN                     get sign of int; +1 or -1                      INLINE
ICHAR                     get integer from character-digit               INLINE
IPOW                      power-func for int; eg  get bitVal for bitPos;

I_BIN                     get int from binary                            INLINE
UTI_I32_2I16              get int from 2 shorts                          INLINE
UTI_hiI16_I32             get hi (left) short out of 32-bit-int          INLINE
UTI_loI16_I32             get lo (right) short out of 32-bit-int         INLINE

ICHG01                    change 0 > 1, 1 > 0.                           INLINE
ICHG0x1                   change 0 > -1,-1 > 0, 1 > -2, -2 > 1           INLINE
ILIM01                    returns 0 (ii <= 0) or 1 (ii >= 1)             INLINE
ILIM0x1                   returns 0 (ii >= 0) or -1 (ii < 0)             INLINE
ILIM2                     returns x = between lo and hi                  INLINE
ILIM0X                    returns x = between 0 and hi                   INLINE
I_XOR_2I                  XOR exclusive or 2 ints                        INLINE
UTI_add_uniq              add int uniq to list
UTI_deleq                 delete equal records form 2 intLists
UTI_findeq                find 2 gleiche Elemente in 2 IntegerListen
UTI_is_seq_2i_ring        test if 2 ints are sequentional in closed group
UTI_i2_sort               sort 2 integers                                INLINE
UTI_ni_sort               sort integerList
UTI_ni_ind_sort           return sorted indexarray for integerarray
UTI_ind_iTab_i            get index of int in iTab
UTI_iNr_chrNr             give nr of ints for n characters (not including \0) INLINE
UTI_div4up                change nr to modulo(4)=0; increase (1|2|3|4 -> 4)   INLINE
UTI_div4diff              get nr of missing bytes for modulo-4 (multiple of 4)INLINE
UTI_round_4up             round integer up to 4                               INLINE
UTI_round_8up             round integer up to 8                               INLINE
UTI_round_32up            round integer up to 32                              INLINE
UTI_round_i2b             round integer to byte (back: UTI_round_b2i)
UTI_round_b2i             make integer from byte (back from UTI_round_i2b)
UTI_sum_row               sum up row from 1 to iend
UTI_ndig_int              split integer into digits
UTI_2dig_int              split integer into 2 digits                         INLINE
UTI_2int8_int16           get 2 characters from short-int                     INLINE
UTI_int16_2int8           get short-int from 2 characters                     INLINE

UTI_ni_ObjRange           get list of integers of interger-ranges
UTI_checksum__            get checksum for data

--------- integers and doubles
ILIMCK1                   check if x is between 0 and lim                INLINE
ILIMCK2                   check if x is between hi and lo                INLINE
UTN_LIMCK__               check if x is between v1 and v2                INLINE

--------- floats;
see DMAX DMIN DSIGN DSIGTOL
FDABS                     absolute value of float

--------- doubles;  see also UT1D_
UTP_dba_str               get table of doubles from textstring with n numbers

DMAX                      INLINE
DMIN                      INLINE
DMOD                      double - division with remainder (module)      INLINE
DSIGN                     sign of double; +1 or -1                       INLINE
DSIGTOL                   sign of double with tolerance; +1 or -1        INLINE
DLIM01                    0 if (d1 >= 0.); 1 if (d1 < 0.)                INLINE
DLIM10                    1 if (d1 >= 0.); 0 if (d1 < 0.)                INLINE
DLIM2                     limits double between hi and lo-value          INLINE
SQRT                      sqrt (sqrt(0.) gives -nan)                     INLINE
ACOS                      cos                                            INLINE
UTP_min_3                 find minimum double of 3; returns 0|1|2
UTP_min_d3                find minimum double of 3; returns double.
UTP_min_4                 find minimum double of 4; returns 0|1|2|3
UTP_min                   find index of minimum double out of dbTab
UTP_dbRec_min             find minimum double out of dbRecords
UTP_max_3                 find maximum double of 3
UTP_max_d3                find maximum double of 3; returns double.
UTP_max_4                 find maximum double of 4
UTP_max                   find minimum double out of dbTab
UTP_dbRec_max             find maximum double out of dbRecords
UTP_db_cknear_2db         find nearest double out of 2 doubles
UTP_db_cknear_ndb         find nearest double out of dbTab
UTP_db_ckNxt_ndb          find next double out of dbTab
UTP_db_ck_in2db           test if value of v is between v1 / v2
UTP_db_ck_in2dbTol        test if value of v is between v1 / v2
UTP_db_ck_in2db_ang1      test if angle vx is between / near values va, vb
UTP_2db_ck_in4db          die beiden inneren Werte aus 4 Zahlen finden
UTP_2db_ck_in2db          test if range2 overlaps range1
UTP_2db_ck_db_near        test if dx is near d0 or near d1                   INLINE

UTP_db_comp_0             if fabs(d1) < UT_TOL_min1) d1 = 0.;
UTP_comp_0                compare double (double == 0.0 + - UT_TOL_min1) INLINE
UTP_compdb0               compare double mit 0.0 mit Tol.
UTP_comp2db               compare 2 doubles (with tolerance)             INLINE
UTP_comp2x2db             compare 2 * 2 doubles (with tolerance)

UTP_sort_npar_npt         sort ascending parameters, points

UTP_par1_vMin_vMax_vx     get 0-1-parameter from vx in range vMin-vMax
UTP_vx_vMin_vMax_par1     get value from a 0-1-parameter in range vMin-vMax

UTP_db_rnd1sig            Zahl auf 1 signifikante Stelle runden
UTP_db_rnd2sig            Zahl auf 2 signifikante Stellen runden
UTP_db_rnd10              round dound double to next 10
UTP_db_rnd5               round dound double to next 5
UTP_dbsiz                 get size of double; (number of pos. digits);
UTP_dbqsiz                get quadr. size (estim.)

UTP_sincosTab_circ        sinus- und cosinuswerte fuer einen Vollkreis.

--------- binary
BIT_UPD                   set or clear bit                          INLINE
BIT_SET                   einzelne Bits setzen in einem int         INLINE
BIT_CLR                   einzelne Bits loeschen in einem int       INLINE
BIT_GET                   einzelne Bits filtern in einem int        INLINE

UTI_dump__                disp. hex

List_functions_end:
=====================================================
see also:
../ut/ut_mem.c            memory-funcs

\endcode *//*----------------------------------------

INLINE-functions in ../ut/ut_geo.h


UTI_ auxiliary functions for integers (lists ...)
UTP_ auxiliary functions for doubles (Parameters)
UTN_  auxiliary functions for integers AND doubles
UTA_  functions for pointers (addresses)




*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/ut_geo.h"             // including ../ut/ut_uti.h



// PROTOTYPES:
  int UTI_ni_ind_CB_sort (void **i1, void **i2);




//================================================================
  int IPOW (int ii) {
//================================================================
// IPOW          power-func for int; eg  get bitVal for bitPos;
//   eg 0 -> 1; 1 -> 2; 2 -> 4; 3 -> 8; ..

  int iOut;

  if(!ii) return 1;

  iOut = 1;
  do { iOut *= 2; --ii;} while (ii);

  return iOut;

}


//================================================================
  int UTI_ni_ObjRange (int *ia, IgaTab *ora, int orNr) {
//================================================================
// UTI_ni_ObjRange                get list of integers of integer-ranges
// Input:
//   orNr  nr of IgaTab-records
// Output:
//   ia    list of ints of orNr IgaTab-records  (get size if ia=NULL)
//   retCod  necessary size of ia (only if ia=NULL)
// ia must have size >= size reported if (ia=NULL)

  int   io, ir, i1, i2, ii;

  io = 0;

  if(!ia) {
    // ia = NULL: reported size of ia
    for(ir=0; ir<orNr; ++ir) if(ora[ir].iNr) io += ora[ir].iNr;
      // printf("ex-ni_IgaTab siz=%d \n",io);
    return io;
  }

  for(ir=0; ir < orNr; ++ir) {
    if(ora[0].iNr) {
      i1 = ora[ir].ibeg;
      i2 = i1 + ora[ir].iNr;
      for(ii=i1; ii<i2; ++ii) {
        ia[io] = ii;
        ++io;
      }
    }
  }

    // TESTBLOCK
    // printf("ex-ni_IgaTab ia %d\n",io);
    // for(i1=0; i1<io; ++i1) printf(" ni[%d] = %d\n",i1,ia[i1]);
    // END TESTBLOCK

  return 0;

}


//================================================================
  UINT_32 UTI_checksum__ (void *buf, int bufLen) {
//================================================================
// UTI_checksum__                   get checksum for data
// Adler-32bit-checksum
// Retcode = checksum

  int       i1;
  UINT_8    *ps = buf;
  UINT_32   ic1 = 1, ic2 = 0;

  for (i1 = 0; i1 < bufLen; i1++) {
    ic1 = (ic1 + ps[i1]) % 65521;
    ic2 = (ic2 + ic1) % 65521;
  }

    // printf("ex-UTI_checksum__  %d  len=%d\n",(ic2 << 16) | ic1,bufLen);

  return (ic2 << 16) | ic1;

}


//================================================================
  int UTP_sort_npar_npt (int iNr, double *da, Point *pa) {
//================================================================
// UTP_sort_npar_npt         sort DEscending parameters, points


  int     i1, i2;


  L_sort1:
  i1 = 0;
  for(i2=1; i2<iNr; ++i2) {
    if(da[i1] > da[i2]) {
        // printf(" _npar_npt swap %d %d\n",i1,i2);
      // swap value and point
      MEM_swap_2db (&da[i1], &da[i2]);
      MEM_swap__ (&pa[i1], &pa[i2], sizeof(Point));
      goto L_sort1;
    }
    ++i1;
  }


    // for(i1=0;i1<iNr;++i1)printf(" sort-%d-%lf\n",i1,da[i1]);

  return 0;

}


//======================================================================
  double UTP_par1_vMin_vMax_vx (double vMin, double vMax, double vx) {
//======================================================================
// UTP_par1_vMin_vMax_vx        get 0-1-parameter from vx in range vMin-vMax
//   eg get parameter from knotvalue of b-spline;
//
// Input:
//   vMin      extreme low value
//   vMax      extreme high value
//   px        get the parameter (0 - 1) from this value in the range vMin - vMax
// Output:
//   retCode   parameter between 0. to 1.
//
// Example:
//   vMin=5.; vMax=10.; vx=7.5;  retVal=0.5
//
// see also UTP_vx_vMin_vMax_par1


  double   d1;

  d1 = vMax - vMin;

  // if(fabs(d1) < UT_TOL_min2) { *parx = 0.5; return -1;}
  if(fabs(d1) < UT_TOL_min2) return (0.);

  return ((vx-vMin) / d1);

}


//================================================================
  int UTA_reallTab (char **startAdr, int offset, int recNr,
                    char *oldAdr, char *newAdr) {
//================================================================
/// \code
/// reallocate a table of pointers
/// startAdr is the first address to be modified.
/// offset   is the distance to the next adress (the recordsize)
/// \endcode

  unsigned long  i1, ad;

  ad = newAdr - oldAdr;

  // printf("UTA_reallTab recNr=%d offset=%d ad=%d\n",recNr,offset,ad);
  // printf("   %p %p\n",oldAdr,newAdr);


  for(i1=0; i1<recNr; ++i1) {
    if(*startAdr != NULL) {
      *startAdr += ad;            // incr Inhalt
    }
    *startAdr = (char*)*startAdr + offset;
  }

  return 0;

}


//================================================================
  int UTI_dump__ (void* pv, int oSiz) {
//================================================================

  int  i1, i2;

  printf("UTI_dump__ %d \n",oSiz);

  i2 = 0;

  for(i1=0; i1<oSiz; ++i1) {
    printf("%02x ",((unsigned char*)pv)[i1]);
    // printf("%d",((char*)pv)[i1]);
    ++i2;
    if(i2 >= 8) {
      i2 = 0;
      printf("\n");
    }
  }

  if(i2 > 0) printf("\n");

  return 0;

}


//================================================================
  int UTI_add_uniq (int *iNr, int *ia, int iNew) {
//================================================================
/// \code
/// UTI_add_uniq              add int uniq to list
/// NO CHECK FOR SIZE OF IA
/// \endcode

  int    i1;


  for(i1=0; i1 < *iNr; ++i1) if(ia[i1] == iNew) return 1;

  ia[*iNr] = iNew;

  *iNr += 1;

  return 0;

}


//======================================================================
  int UTI_deleq (int *i1Nr, int *i1Tab, int *i2Nr, int *i2Tab) {
//======================================================================
/// delete equal records form 2 intLists

  int   irc, i1, i2;

  // printf("UTI_deleq %d %d\n",*i1Nr,*i2Nr);


  L_nxt:
  // find 2 equal records
  irc = UTI_findeq (&i1, &i2, i1Tab, *i1Nr, i2Tab, *i2Nr);
  if(irc == 0) {
    irc = MEM_del_nrec (i1Nr, i1Tab, i1, 1, sizeof(int));
    if(irc < 0) return irc;
    irc = MEM_del_nrec (i2Nr, i2Tab, i2, 1, sizeof(int));
    if(irc < 0) return irc;
    goto L_nxt;
  }

  return 0;

}


//================================================================
  int UTI_ni_sort (int *iTab, int iNr) {
//================================================================
/// int-tabelle sortieren

// Vom index 0 bis iNr-2 gehen;
//  immer die niedrigste Zahl suchen, vertauschen, weiter.


  int    i1, i2, ii, imax;


  for(i1=0; i1<iNr; ++i1) {
    imax = UT_INT_MAX;
    ii = 0;
    // find lowest int from i1 to iNr
    for(i2=i1; i2<iNr; ++i2) {
      if(iTab[i2] < imax) {
        ii = i2;
        imax = iTab[i2];
      }
    }
    if(ii > i1) {
        // printf(" swap %d %d / %d %d\n",i1,ii,iTab[i1],iTab[ii]);
      MEM_swap_int(&iTab[i1], &iTab[ii]);
    }

  }


  return 0;

}


//================================================================
  int UTI_ni_ind_CB_sort (void **i1, void **i2) {
//================================================================
// sort pointers to integers by comparison of integers
// for UTI_ni_ind_sort

  // printf("UTI_ni_ind_CB_sort %d %d\n",*((int*)*i1),*((int*)*i2));

  if(*((int*)*i1) < *((int*)*i2)) return -1;
  else return 1;
  // if(*((int*)*i1) > *((int*)*i2)) return 1;

  return 0;

}

 
//================================================================
  int UTI_ni_ind_sort (int *iia, int *ia, int iNr) {
//================================================================
/// \code
/// UTI_ni_ind_sort    return sorted indexarray for integerarray
/// Input:     integerarray ia with iNr elements. 
/// Output:    indexarray iia with iNr elements.
///   Retcod:  0=OK; -1=out of memory
/// Example:
///   int ia[]={2,6,3,3,4};   int iia[5];
///   UTI_ni_ind_sort (iia, ia, 5);
///   for(i1=0;i1<5;++i1)printf(" ia[iia[%d]]=%d\n",i1,ia[iia[i1]]);
/// \endcode
 
  int    i1;
  void   **pia;


  // printf("UTI_ni_ind_sort %d\n",iNr);
  // for(i1=0; i1<iNr; ++i1) printf(" ia[%d]=%d\n",i1,ia[i1]);

  if(iNr < 2) {
    if(iNr == 1) iia[0] = 0;
    return 0;
  }


  // alloc iNr pointers;
  pia = MEM_alloc_tmp ((int)(iNr * sizeof(void*)));
  if(!pia) {TX_Error("UTI_ni_ind_sort EOM1"); return -1;}

  // set pointers
  for(i1=0; i1<iNr; ++i1) pia[i1] = &ia[i1];
    // for(i1=0; i1<iNr; ++i1) printf(" p[%d]=%p\n",i1,pia[i1]);
    // for(i1=0; i1<iNr; ++i1) printf(" *pia[%d]=%d\n",i1,*((int*)pia[i1]));

  // sort pointers to integers by comparison of integers
  qsort (pia, iNr, sizeof(void*), (void*)UTI_ni_ind_CB_sort);
    // for(i1=0; i1<iNr; ++i1) printf(" pia[%d]=%d\n",i1,*((int*)pia[i1]));
    // for(i1=0; i1<iNr; ++i1) printf(" p[%d]=%p\n",i1,pia[i1]);

  // make indexarray
  for(i1=0; i1<iNr; ++i1) {
    iia[i1] = ((char*)pia[i1] - (char*)pia[0]) / sizeof(int);
      // printf(" iia[%d]=%d\n",i1,iia[i1]);
  }
    // for(i1=0; i1<iNr; ++i1)
    // printf(" iia[%d]=%d; ia[%d]=%d\n",i1,iia[i1], ia[i1],ia[iia[i1]]);

  return 0;

}


//================================================================
  int UTI_is_seq_2i_ring (int i1, int i2, int iNr) {
//================================================================
// test if 2 ints are sequentional in closed group (have sequential indexes)
// returns 0=No; 1=Yes
// Example: if group has 7 members then index 0 and 6 are connected !
//   also connected: 0,6; 6,0; 3,4; i4,5; 4,3 ..
//   not connected:  3,5 ..

  if(i1+i2+1 == iNr) return 1;

  return (abs(i1-i2) == 1 ? 1 : 0);

}


//================================================================
  int UTI_findeq (int *if1, int *if2,
                  int *iTab1, int iNr1, int *iTab2, int iNr2) {
//================================================================
/// \code
/// find 2 gleiche Elemente in den Listen iTab1 / iTab1
/// if1, if2   Indices der gleichen Elemente
/// RetCode:
///  0 gleiche Elemente gefunden; Indices out in if1, if2
/// -1 keine gleichen Elemente gefunden.
/// \endcode


  int   i1, i2;

  for(i1=0; i1<iNr1; ++i1) {
    for(i2=0; i2<iNr2; ++i2) {
      if(iTab1[i1] == iTab2[i2]) {
        *if1 = i1;
        *if2 = i2;
        // printf("ex UTI_findeq %d %d\n",i1,i2);
        return 0;
      }
    }
  }

  return -1;

}


//================================================================
  int UTI_ind_iTab_i (int ii, int *iTab, int tabSiz) {
//================================================================
/// \code
/// UTI_ind_iTab_i            get index of int <ii> in iTab
/// RetCod: index into iTab  or -1 (not found)
/// \endcode


  int  i1;

  // printf("UTI_ind_iTab_i %d siz=%d\n",ii,tabSiz);
  // for(i1=0;i1<tabSiz; ++i1) printf(" iTab[%d]=%d\n",i1,iTab[i1]);


  for(i1=0; i1<tabSiz; ++i1) {
    if(iTab[i1] == ii) return i1;
  }

  return -1;

}


/* replaced by INLINE
//================================================================
  int UTI_div4up (int ii) {
//================================================================
/// \code
/// UTI_div4up          change nr to modulo(4)=0; increase (1|2|3|4 -> 4)
///                     returns ii as a multiple of 4.
/// \endcode

// Eine Funktion die eine Zahl durch 4 teilbar macht;
//   aus 1,2 od 3 wird 4; aus 5,6,7 wird 8 usw.


  int id;

  id = 4 - ii % 4;
  if(id < 4) ii += id;

    // printf(" id=%d\n",id);

  return ii;

}
*/

/* replaced by INLINE
//================================================================
  int UTI_div4diff (int ii) {
//================================================================
/// \code
/// UTI_div4diff        get nr of missing bytes for modulo-4.
///  ii = 5 returns 3;    6 returns 2 .. 
/// \endcode


  int id;

  // printf("UTI_div4diff %d\n",ii);

  ii %= 4;
  if(ii) ii = 4 - ii;

    // printf("ex UTI_div4diff %d\n",ii);

  return ii;

}
*/

//================================================================
  int UTI_round_i2b (int ii) {
//================================================================
/// \code
/// UTI_round_i2b             round integer to byte (back: UTI_round_b2i)
///  0,1 -> 0; 2 -> 1; 3,4 -> 2; 5-8 -> 3; 9-16 -> 4; 17-32 -> 5; .. 
///   Purpose: save Integers in a single byte, rounding up.
///     UTI_round_i2b(20)    -> 5;     // all values from 17 to 32 give 5;
///     UTI_round_b2i(5)     -> 32
/// \endcode

//  ii = 10000;
//  printf(" ii=%d, i2b=%d; b2i=%d\n",ii,
//           UTI_round_i2b(ii),UTI_round_b2i(UTI_round_i2b(ii)));


  int   ib = -1;

  if(ii < 2) return 0;

  ii *= 2;
  --ii;

  while(ii) {ii /= 2; ++ib;}

  return ib;

}


//================================================================
  int UTI_round_b2i (int i1) {
//================================================================
/// \code
/// UTI_round_b2i         make integer from byte (back from UTI_round_i2b)
///   returns (2 to the power of i1);
/// 0 -> 0; 1 -> 2; 2 -> 4; 3 -> 8; 4 -> 16; 5 -> 32; .. 8=256; .. 10=1024 ..
/// \endcode

  int ii=1;

  if(i1) {
    while(i1) {--i1; ii *= 2;}
  } else return i1;

  return ii;

}


//================================================================
  int UTI_sum_row (int iend) {
//================================================================
// sum up row from 1 to iend;
// iend=2 gives 3; iend=3 gives 6, 4 gives 10 ..
// faktorielle ?


  int   ii, iSum = 0;

  for (ii = 1; ii <= iend; ++ii) iSum += ii;

  return iSum;

}


//================================================================
  int UTI_ndig_int (int *digTab, int digNr, int ii) {
//================================================================
/// \code
/// UTI_ndig_int                   split integer into digits
/// Example:
/// ii=123 returns digTab[0]=3; digTab[2]=2; digTab[2]=1;
/// \endcode
 
  int    i1, ipos = 0;
    
  // printf("UTI_ndig_int %d\n",ii);


  L_nxt:
    digTab[ipos] = ii % 10;
    ii /= 10;
      // printf(" digTab[%d] %d %d\n",ipos,digTab[ipos],ii);
    ++ipos;
    if(ipos < digNr) goto L_nxt;

  return ipos;

} 



//================================================================
//================================================================
//================================================================



/*
//======================================================================
  int UTP_comp_0 (double db) {
//=============
// UTP_comp_0                compare double (double == 0.0 + - UT_TOL_min1)
//
// Retcode 0:   db <> 0.0
//         1:   db = 0.0
//
// if(UTP_comp_0(d1)) printf("d1 ist 0.0!\n");


  return (fabs(db) < UT_TOL_min1);

}
*/

/*
//======================================================================
  int UTP_compdb0 (double db, double tol) {
//======================================================================
/// \code
/// UTP_compdb0               compare double mit 0.0 mit Tol.
/// 
/// Retcode 0:   db <> 0.0 (gr. oder kl. tol)
///         1:   db = 0.0 (innerhalb tol)
/// \endcode

// if(UTP_compdb0(d1,tol)) printf("d1 ist 0.0!\n");
// if(UTP_compdb0(d1,tol) == 0) printf("d1 ist nicht 0.0!\n");
// if(UTP_compdb0(d1,tol) == 1) printf("d1 ist 0.0!\n");


  return (fabs(db) < tol);

}
*/


//======================================================================
  int UTP_db_ck_in2db (double v, double v1, double v2) {
//======================================================================
/// \code
/// test if value of v is between v1 / v2
/// RC =  0:   YES, v is between v1 / v2
/// RC =  1:   NO,  v is outside near v1; v2 > v1.
/// RC =  2:   NO,  v is outside near v2; v2 > v1.
/// RC = -1:   NO,  v is outside near v1; v2 < v1.
/// RC = -2:   NO,  v is outside near v2; v2 < v1.
/// \endcode

  // printf("UTP_db_ck_in2db %f %f %f\n",v,v1,v2);

  if(v2 > v1) {      // v1 < v2
    if(v > v2) return 2;
    if(v < v1) return 1;

  } else {           // v2 < v1
    if(v > v1) return -1;
    if(v < v2) return -2;

  }

  return 0;

}



//======================================================================
  int UTP_db_ck_in2dbTol (double v, double v1, double v2, double tol) {
//======================================================================
/// \code
/// test if value of v is between v1 / v2
/// RC = 0:   YES, v is between v1 / v2
/// RC = 1:   NO,  v is outside v1 / v2
/// \endcode


  if(v2 > v1) {      // v1 < v2
    if((v > v2+tol)||(v < v1-tol)) return 1;

  } else {           // v2 < v1
    if((v > v1+tol)||(v < v2-tol)) return 1;

  }

  return 0;

}


//================================================================
  int UTP_2db_ck_in4db (int *ii1, int *ii2, double *dbTab) {
//================================================================
/// UTP_2db_ck_in4db         die beiden inneren Werte aus 4 Zahlen finden

// man koennte auch in einer einzigen Loop den hoechsten und den zweithoechsten,
// den tiefsten und den zweit-tiefsten wert suchen.

  int    i1, i2, i3, i4;


  i1 = UTP_min (4, dbTab);
  i2 = UTP_max (4, dbTab);

  // die beiden uebriggebliebenen Indizes --> ii1/ii2
  i3 = 0;
  i4 = 0;

  L_1:
  if(i3 == i1) ++i3;
  if(i3 == i2) {++i3; goto L_1;}
  // printf(" nach L1 %d %d %d\n",i1,i2,i3);

  L_2:
  if(i4 == i1) ++i4;
  if(i4 == i2) {++i4; goto L_2;}
  if(i4 == i3) {++i4; goto L_2;}
  // printf(" nach L2 %d %d %d %d\n",i1,i2,i3,i4);

  *ii1 = i3;
  *ii2 = i4;
  // printf("ex UTP_2db_ck_in4db %d %d\n",*ii1,*ii2);

  return 0;

}


//======================================================================
  int UTP_db_ck_in2db_ang1 (double vx, double va, double vb, int dir) {
//======================================================================
// UTP_db_ck_in2db_ang1       test if angle vx is between / near values va, vb
//   all values must be between 0. and 2PI.
//
// dir    direction; 0=fwd 1=bwd
// retCod:   1   vx is between va-vb near va
//           2   vx is between va-vb near vb
//          -1   vx is outside va
//          -2   vx is outside vb
// 
// Tests:
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (6.1, 0.0, 2.0, 0));
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (0.1, 0.0, 2.0, 0));
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (1.9, 0.0, 2.0, 0));
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (2.1, 0.0, 2.0, 0));
// 
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (1.9, 2.0, 6.1, 0));
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (2.1, 2.0, 6.1, 0));
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (6.0, 2.0, 6.1, 0));
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (0.1, 2.0, 6.1, 0));
// 
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (4.9, 5.0, 2.0, 0));
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (5.1, 5.0, 2.0, 0));
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (1.9, 5.0, 2.0, 0));
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (2.1, 5.0, 2.0, 0));
// 
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (2.1, 2.0, 0.0, 1));
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (1.9, 2.0, 0.0, 1));
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (0.1, 2.0, 0.0, 1));
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (6.1, 2.0, 0.0, 1));
// 
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (2.1, 2.0, 5.0, 1));
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (1.9, 2.0, 5.0, 1));
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (5.1, 2.0, 5.0, 1));
//   printf(" %d\n", UTP_db_ck_in2db_ang1 (4.9, 2.0, 5.0, 1));



  int       ii, iout, iswap;
  double    d1, d2, v1=RAD_360;


  // printf("UTP_db_ck_in2db_ang1  vx=%f   va=%f   vb=%f\n",vx,va,vb);

  if(dir) {
    // circle bwd = CW
    MEM_swap_2db (&va, &vb);
    iswap = 1;
  } else iswap = 0;


  // if(!dir) {
    // circle fwd = CCW
    if(va > vb) vb += v1;
    if(vx < va) vx += v1;
      // printf("  vx=%f   va=%f   vb=%f\n",vx,va,vb);

    // test in(iout=) or out(iout=1);
    if((vx < va) || (vx > vb)) iout = -1;   // out
    else                       iout = 1;    // in

    // test near va (inear=1) or near vb (inear=2)
    if(vx > vb) d1 = fabs((vx-v1) - va);
    else        d1 = fabs(vx - va);

    if(vx < va) d2 = fabs((vx+v1) - vb);
    else        d2 = fabs(vx - vb);
      // printf("  iout=%d d1=%f d2=%f\n",iout,d1,d2);

    if(!iswap) {
      // fwd CCW
      if(d1 < d2) ii = 1 * iout;
      else        ii = 2 * iout;
    } else {
      // bwd CW
      if(d1 < d2) ii = 2 * iout;
      else        ii = 1 * iout;
    }

    // printf(" ex-UTP_db_ck_in2db_ang1 %d\n",ii);

  return ii;

}


//================================================================
  int UTP_2db_ck_in2db (double r1Start, double r1End,
                    double r2Start, double r2End) {
//================================================================
// test if range2 overlaps range1
// range1: from r1Start - r1End;
// range2: from r2Start - r2End;
//
// DOES NOT TEST TOLERANCE; see UT2D_ck_2par_pt_in_2par_2pt
//
// Retcod:
//    -4                     1s-----------1e      outLow
//              2s------2e
//
//    -3        1s------1e                        outHigh
//                           2s--------2e
//
//    -2            1s-----------1e               overlap low
//              2s--------2e
//
//    -1        1s-----------1e                   overlap high
//                  2s-----------2e
//
//    1          1s-----------1e                  enclose cv2
//                  2s-----2e
//
//    2             1s-----1e                     enclose cv1
//              2s------------2e          
//
//
// see also UT2D_ck_2par_pt_in_2par_2pt

  int  irc;

  // printf("UTP_2db_ck_in2db %lf %lf | %lf %lf\n",r1Start,r1End,r2Start,r2End);

  // sort
  if(r1Start > r1End) MEM_swap_2db (&r1Start, &r1End);
  if(r2Start > r2End) MEM_swap_2db (&r2Start, &r2End);


  if (r2End <= r1Start) {
    irc = -4;


  } else if (r2Start >= r1End) {
    irc = -3;


  } else if (r1Start > r2Start) { 
    //  2s___1s---
    if (r2End > r1End) {
      irc = 2;
    } else {
      irc = -2;
    }


  } else {
    //  1s---2s___
    if (r2End < r1End) {
      irc = 1;
    } else {
      irc = -1;
    }
  }

    // printf("ex UTP_2db_ck_in2db irc=%d\n",irc);

  return irc;

}


/*
//=========================================================
  int UTP_comp2db (double db1, double db2, double tol) {
//=========================================================
// UTP_comp2db               compare 2 doubles (with tolerance)

// Retcode 0 = Differenz der Werte > tol
        // 1 = Differenz der Werte < tol

// if(UTP_comp2db(d1, d2, tol)) printf("d1 ist gleich d2! \n");

  return (fabs(db2-db1) < tol);

}
*/

/*
//======================================================================
  double UTP_db_rndsig (double d1, int isig) {
//======================================================================
// UTP_db_rndsig             Zahl auf n signifikante Stellen runden

// (0.00500, 1) -> 0.005
// (0.19500, 1) -> 0.2           (gerundet !!)
// (1.00500, 1) -> 1.
// (167.500, 1) -> 200.

// ACHTUNG: derzeit nur isig=1
//   auf 2 sig. Stellen bringen:
//     solange * 10, bis >= 10.; dann nach int; dann wieder dividieren.



  int    i1, il, idp, isp, ii;
  double d2;
  char   cbuf[32], *p1;


  // printf("UTP_db_rndsig %f %d\n",d1,isig);


  // zum Runden multiplizieren; isig=1: * 1.1;  isig=2: ??
  if(isig != 1) {
    // derzeit nur isig=1
    TX_Error("UTP_db_rndsig E001");
    return d1;
  }


  sprintf(cbuf, "%.12f", d1);
  il=strlen(cbuf);


  // find DecPunktPosi idp
  // for(i1=0; i1<il; ++i1) {
    // if(cbuf[i1] != '.') continue;
    // idp = i1;
    // break;
  // }
  p1 = strchr(cbuf, '.');
  if(p1 == NULL) {
    TX_Print("UTP_db_rndsig E001 |%s| - check locale !!",cbuf);
    return d1;
  }
  idp = p1 - cbuf;
  // printf(" |%s| %d %d\n",cbuf,il,idp);



  // find 1. sig. pos isp
  for(i1=0; i1<il; ++i1) {
    if((cbuf[i1] == '0')||(cbuf[i1] == '.')) continue;
    isp = i1;
    break;
  }

  if(isp < idp) { // > 0.
    ii = idp - 1;

  } else {        // < 0;
    ii = 1 - isp;

  }

  d2 = pow(10., ii) / 2.;


  // printf("  rndsig d1=%f dp=%d sp=%d ii=%d d2=%f\n",d1,idp,isp,ii,d2);


  d1 += d2;

  sprintf(cbuf, "%.12f", d1);
  il=strlen(cbuf);


  // find DecPunktPosi idp
  for(i1=0; i1<il; ++i1) {
    if(cbuf[i1] != '.') continue;
    idp = i1;
    break;
  }



  // find 1. sig. pos isp
  for(i1=0; i1<il; ++i1) {
    if((cbuf[i1] == '0')||(cbuf[i1] == '.')) continue;
    isp = i1;
    break;
  }

  if(isp < idp) { // Zahl > 0.
    cbuf[idp] = '\0';   // ab komma wegschneiden
    // von isp bis Komma mit Nullen fuellen
    ii = isp + isig;
    for(i1=ii; i1<idp; ++i1) {
      cbuf[i1] = '0';
    }

  } else {        // Zahl < 0;
    ii = isp + 1;
    if(ii < il) cbuf[ii] = '\0';
  }


  d2 =atof(cbuf);


  // printf("ex UTP_db_rndsig %f |%s| %f\n",d2,cbuf,d1);
  return d2;

}
*/

//================================================================
  int UTP_dba_str (double *da, int siz, char *str1) {
//================================================================
// UTP_dba_str               get table of doubles from textstring with n numbers
// Input:
//   siz      size of da 
// Output:
//   da       numbers of str1; size must be 
//   retCod   number of doubles in da


  int    ii=0;
  char   *p1, *p2;

  // printf("UTP_dba_str |%s|\n",str1);

  
  p1 = str1;

  L_nxt1:
    da[ii] = strtod (p1, &p2);
    ++ii;
    if(ii >= siz) goto L_exit;
    if(!*p1) goto L_exit;

    da[ii] = strtod (p2, &p1);
    ++ii;
    if(ii >= siz) goto L_exit;
    if(!*p1) goto L_exit;
    goto L_nxt1;

  L_exit:

    // {int i1; for(i1=0; i1<ii; ++i1) printf("UTP_dba_str[%d] = %f\n",i1,da[i1]);}

  return ii;

}


//======================================================================
  double UTP_db_rnd1sig (double d1) {
//======================================================================
/// \code
///   auf 1 sig. Stelle bringen:
/// 
/// (0.00579, 1) -> 0.006
/// (0.19500, 1) -> 0.2           (gerundet !!)
/// (1.00500, 1) -> 1.
/// (167.500, 1) -> 200.
/// \endcode


//     solange * 10, bis >= 1.; dann nach int; dann wieder dividieren.

  int      iv = 1;
  double   dd, di;

  dd  = d1;
  di  = 1.;

  if(fabs(dd) < UT_TOL_min1) {dd = 0.; goto L_done;}

  if(dd < 0.) {dd = -d1; iv = -1;}

  if(dd > 1.) goto L_dwn;


  L_up:
    if(dd < 1.) {dd *= 10.; di *= 10.; goto L_up;}
    dd = floor(dd+0.5);
    dd /= di;
    goto L_done;


  L_dwn:
   if(dd > 10.) {dd /= 10.; di *= 10; goto L_dwn;}
    dd = floor(dd+0.5);
    dd *= di;


  L_done:

  if(iv < 0) dd = -dd;

    // printf("ex UTP_db_rnd1sig out=%f in=%f %f\n",dd, d1, di);

  return dd;

}


//======================================================================
  double UTP_db_rnd2sig (double d1) {
//======================================================================
/// \code
///   auf 2 sig. Stellen runden
/// 
/// (0.00579, 1) -> 0.0058
/// (0.19500, 1) -> 0.19          (gerundet !!)
/// (1.00500, 1) -> 1.
/// (167.500, 1) -> 170.
/// \endcode


//     solange * 10, bis >= 10.; dann nach int; dann wieder dividieren.

  int      iv = 1;
  double   dd, di;

  dd  = d1;
  di  = 1.;


  if(fabs(dd) < UT_TOL_min1) {dd = 0.; goto L_done;}

  if(dd < 0.) {dd = -d1; iv = -1;}

  if(dd > 10.) goto L_dwn;


  L_up:
    if(dd < 10.) {dd *= 10.; di *= 10.; goto L_up;}
    dd = floor(dd+0.5);
    dd /= di;
    goto L_done;


  L_dwn:
   if(dd > 100.) {dd /= 10.; di *= 10; goto L_dwn;}
    dd = floor(dd+0.5);
    dd *= di;


  L_done:

  if(iv < 0) dd = -dd;

  // printf("ex UTP_db_rnd2sig %f %f %f\n",dd, d1, di);

  return dd;

}


//================================================================
  int UTP_db_rnd10 (double d1) {
//================================================================
/// \code
/// UTP_db_rnd10
///    2 gives     10
///   40 gives    100
///  777 gives   1000
/// \endcode


  int  i1, id, is;

  id = 1;

  is = UTP_dbsiz (&i1, d1);

  if(is > 0) {
    for(i1=0; i1<is; ++i1) id *= 10;
  }

  return id;

}


//================================================================
  int UTP_db_rnd5 (double d1) {
///================================================================
// UTP_db_rnd5
/// \code
///    2 gives      5
///    7 gives     10
///   40 gives     50
///   77 gives    100
///  777 gives   1000
/// \endcode


  int  i1, id, is, i5;

  id = 1;

  is = UTP_dbsiz (&i5, d1);

  if(is > 0) {
    for(i1=0; i1<is; ++i1) id *= 10;
  }

  if(i5 < 0) id /= 2;

  return id;

}


//================================================================
  int UTP_dbsiz (int *i5, double d1) {
//================================================================
/// \code
/// UTP_dbsiz    get size of double; (number of pos. digits);
///   5 gives 1, 12 gives 2 ...
///  i5 gives if d1 is over(+1) or under(-1) 5.
///   d1=22  gives i5=-1;
///   d1=777 gives i5=+1;
/// \endcode

  int           i1;
  unsigned long l1;

  l1 = d1;

  for(i1=1; i1<100; ++i1) {
    if(l1 < 10) {
      if(l1 < 5) *i5 = -1;
      else       *i5 = 1;
      return i1;
    }
    l1 /= 10;
  }

  return -1;

}


//================================================================
  int UTP_dbqsiz (double d1) {
//================================================================
/// UTP_dbqsiz                get quadr. size (estim.)

  int     ii=0;
  double  d2;


  L1:
  d2 = sqrt(d1);
  if(d2 > 2.) { ++ii; d1 = d2; goto L1; }

    // printf("ex UTP_dbqsiz ii=%d\n",ii);

  return ii;

}


//=======================================================================
  int UTP_db_cknear_2db (double *db, double *d1, double *d2) {
//=======================================================================
/// \code
/// UTP_db_cknear_2db         find nearest double out of 2 doubles
/// RetCod 0 = db-d1 ist smaller than db-d2
/// RetCod 1 = db-d2 ist smaller than db-d1
/// \endcode


  if(fabs(*db - *d1) < fabs(*db - *d2)) return 0;
  else                                  return 1;

}


//=======================================================================
  int UTP_db_ckNxt_ndb (double db1, int dbNr, double *dbTab) {
//=======================================================================
/// UTP_db_ckNxt_ndb          find next double out of dbTab
/// Output:
///   retCod   index of value >= db1 into dbTab
///            -1  if db1 > dbTab[dbNr - 1]


  int    i1, ind;
  double d1;


  ind = 0;
  d1  = UT_VAL_MAX;


  for(i1=0; i1<dbNr; ++i1) {
    if(dbTab[i1] > db1) return i1;
  }

  return -1;

}


//=======================================================================
  int UTP_db_cknear_ndb (double db1, int dbNr, double *dbTab) {
//=======================================================================
/// UTP_db_cknear_ndb         find nearest double out of dbTab


  int    i1, ind;
  double d1;


  ind = 0;
  d1  = UT_VAL_MAX;


  for(i1=0; i1<dbNr; ++i1) {
    if(fabs(dbTab[i1] - db1) < d1) {
      d1 = fabs(dbTab[i1] - db1);
      ind = i1;
    }
  }

  return ind;

}


//================================================================
  int UTP_comp2x2db (double d11,double d12,
                     double d21,double d22, double tol) {
//================================================================
/// \code
/// UTP_comp2x2db             compare 2 * 2 doubles (with tolerance)
/// 2 * 2 Zahlen vergleichen; gleich=0; verkehrt=1; else=-1.
/// RetCod:
///   0    d11 == d21; d12 == d22;  beide Paare gleich.
///   1    d11 == d22; d12 == d21;  beide Paare gleich, aber verkehrt.
///  -1    d11 == d21; d21 != d22;
///  -2    d11 == d22; d12 != d21
///  ACHTUNG: d12 == d22 wird nicht getestet !
///  -3    beide Paare ungleich.
/// \endcode

  int irc;

  // printf("UTP_comp2x2db %f %f %f %f %f\n",d11,d12,d21,d22,tol);

  irc = -1;

  if(fabs(d11-d21) < tol) {             // d11 == d21
    if(fabs(d12-d22) < tol) irc = 0;    // d12 == d22
    else                    irc = -1;

  } else if(fabs(d11-d22) < tol) {      // d11 == d22
    if(fabs(d12-d21) < tol) irc = 1;    // d12 == d21
    else                    irc = -2;

  // hier sollte noch d12 == d22 getestet werden, wenn das wichtig ist !
  } else irc = -3;

  // printf("ex UTP_comp2x2db %d\n",irc);

  return irc;

}


//========================================================
  double UTP_min_d3 (double *d1, double *d2, double *d3) {
//========================================================
/// UTP_min_d3                find minimum double of 3; returns double.


  if(*d1 > *d2) {
    // d1>d2
    if(*d3 < *d2) return *d3;
    return *d2;

  } else {
    // d2>d1
    if(*d3 < *d1) return *d3;
    return *d1;
  }

    printf("UTP_min_d3 ERR -1\n");

  return *d1;

}


//====================================================
  int UTP_min_3 (double *d1, double *d2, double *d3) {
//====================================================
/// \code
/// UTP_min_3                 find minimum double of 3
/// returns 0|1|2
/// \endcode


  if(*d1 > *d2) {
    // d1>d2
    if(*d3 < *d2) return 2;
    return 1;

  } else {
    // d2>d1
    if(*d3 < *d1) return 2;
    return 0;
  }

    printf("UTP_min_3 ERR -1\n");

  return -1;

}


//================================================================
  int UTP_min_4 (double *d1, double *d2, double *d3, double *d4) {
//================================================================
/// \code
/// UTP_min_4                 find minimum double of 4
/// returns 0|1|2|3
/// \endcode


  if(*d1 < *d2) {
    if(*d3 < *d4) {
      // d3<d4
      if(*d1 < *d3) {
        return 0;
      } else {
        return 2;
      }
    } else {
      // d4<d3
      if(*d1 < *d4) {
        return 0;
      } else {
        return 3;
      }
    }

  } else {
    // d2<d1
    if(*d3 < *d4) {
      // d3<d4
      if(*d2 < *d3) {
        return 1;
      } else {
        return 2;
      }
    } else {
      // d4<d3
      if(*d2 < *d4) {
        return 1;
      } else {
        return 3;
      }
    }
  }

    printf("UTP_min_4 ERR -1\n");

  return -1;

}


//================================================================
  int UTP_min (int dbNr, double *dbTab) {
//================================================================
/// \code
/// UTP_min         find minimum double out of dbTab
/// returns index of min. double.
/// \endcode


  int    i1, ind;
  double d1;


  // printf("UTP_min %d\n",dbNr);
  // for(i1=0;i1<dbNr;++i1) printf(" d[%d]=%f\n",i1,dbTab[i1]);

  ind = 0;
  d1  = UT_VAL_MAX;


  for(i1=0; i1<dbNr; ++i1) {
    if(dbTab[i1] < d1) {
      d1 = dbTab[i1];
      ind = i1;
    }
  }

  // printf("ex UTP_min %d %f\n",ind,dbTab[ind]);
  return ind;

}


//================================================================
  int UTP_dbRec_min (int dbNr, double *dTab, int recSiz) {
//================================================================
/// \code
/// UTP_dbRec_min    find minimum double out of dbRecords
/// dbNr records of size recSiz; first obj is a double; find index of min 
/// \endcode


  int    i1, ind;
  double d1;


  ind = 0;
  d1  = UT_VAL_MAX;

  for(i1=0; i1<dbNr; ++i1) {
    if(*dTab < d1) {
      d1 = *dTab;
      ind = i1;
    }
    // (char*)dTab += recSiz;
    dTab = (double*)((char*)dTab + recSiz);
  }

    // printf("ex UTP_min %d %f\n",ind,dTab[ind]);

  return ind;

}


//========================================================
  double UTP_max_d3 (double *d1, double *d2, double *d3) {
//========================================================
/// UTP_max_d3                find maximum double of 3; returns double.


  if(*d1 < *d2) {
    // d1>d2
    if(*d3 > *d2) return *d3;
    return *d2;

  } else {
    // d2>d1
    if(*d3 > *d1) return *d3;
    return *d1;
  }

    printf("UTP_max_d3 ERR -1\n");

  return *d1;

}


//====================================================
  int UTP_max_3 (double *d1, double *d2, double *d3) {
//====================================================
/// \code
/// UTP_max_3                 find maximum double of 3
/// returns 0|1|2
/// \endcode


  if(*d1 < *d2) {
    // d1<d2
    if(*d3 > *d2) return 2;
    return 1;

  } else {
    // d2<d1
    if(*d3 > *d1) return 2;
    return 0;
  }

    printf("UTP_max_3 ERR -1\n");

  return -1;

}


//================================================================
  int UTP_max_4 (double *d1, double *d2, double *d3, double *d4) {
//================================================================
// UTP_max_4                 find maximum double of 4
// returns 0|1|2|3


  if(*d1 > *d2) {
    if(*d3 > *d4) {
      // d3>d4
      if(*d1 > *d3) {
        return 0;
      } else {
        return 2;
      }
    } else {
      // d4>d3
      if(*d1 > *d4) {
        return 0;
      } else {
        return 3;
      }
    }

  } else {
    // d2>d1
    if(*d3 > *d4) {
      // d3>d4
      if(*d2 > *d3) {
        return 1;
      } else {
        return 2;
      }
    } else {
      // d4>d3
      if(*d2 > *d4) {
        return 1;
      } else {
        return 3;
      }
    }                   
  }

    printf("UTP_max_4 ERR -1\n");

  return -1;

}


//================================================================
  int UTP_dbRec_max (int dbNr, double *dTab, int recSiz) {
//================================================================
/// \code
/// UTP_dbRec_max         find maximum double out of dbRecords
/// dbNr records of size recSiz; first obj is a double; find index of min
/// \endcode


  int    i1, ind;
  double d1;


  ind = 0;
  d1  = UT_VAL_MIN;

  for(i1=0; i1<dbNr; ++i1) {
    if(*dTab > d1) {
      d1 = *dTab;
      ind = i1;
    }
    // (char*)dTab += recSiz;
    dTab = (double*)((char*)dTab + recSiz);
  }

    // printf("ex UTP_max %d %f\n",ind,dTab[ind]);

  return ind;

}


//================================================================
  int UTP_max (int dbNr, double *dbTab) {
//================================================================
/// UTP_min         find minimum double out of dbTab


  int    i1, ind;
  double d1;


  ind = 0;
  d1  = UT_VAL_MIN;


  for(i1=0; i1<dbNr; ++i1) {
    if(dbTab[i1] > d1) {
      d1 = dbTab[i1];
      ind = i1;
    }
  }

  // printf("ex UTP_max %d %f\n",ind,dbTab[ind]);
  return ind;

}


//================================================================
  int UTP_sincosTab_circ (double *sa,double *ca, int dNr) {
//================================================================
/// \code
/// die sinus- und cosinuswerte fuer einen Vollkreis.
/// insgesamt dNr Werte; erster und letzter sind gleich.
/// sa = sin-Tabelle;
/// ca = cos-Tabelle
/// 
///  xxxx
///   |   xxx
///   |       x
///   |   r / | x
///   |   /   |  x
///   | /    sin  x
/// --x-------|---x-------------
///      cos      x
/// \endcode

// punkt.x = cos(ang) * radius;
// punkt.y = sin(ang) * radius;




  int    i1;
  double da, aa, *sTab, *cTab;


  // printf("UTP_sincosTab_circ %d\n",dNr);

  da = RAD_360 / (double)(dNr-1);

  aa = 0.;
  sTab = sa;
  cTab = ca;

  for (i1=0; i1<dNr; ++i1) {
    *sTab = sin(aa);
    *cTab = cos(aa);
    aa += da;
    ++sTab;
    ++cTab;
  }

  *sTab = 0.;
  *cTab = 1.;

  return 0;

}


//====================== EOF ===========================
