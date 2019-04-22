//
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
2011-06-10 UME_reset replaced by UME_adjust
2009-05-30 UME_release replaced by UME_reset
2009-05-30 UME_ck_add replaced by UME_reserve
2009-03-24 UME_mTab_* replaced by MemTab_*
2008-03-21 UME_reall_add,UME_realloc,UME_reall_save: spcOff zu. RF.
2003-05-30 Erste Version aus ut_string.c. RF.

-----------------------------------------------------
*/
#ifdef globTag
void UME(){}
#endif
/*!
\file  ../ut/ut_umem.c
\brief variable-length-records in memory: reserve/connect/release .. UME_
\code
=====================================================
List_functions_start:

MEM_alloc_tmp         allocate mem for active function (temp.space)
MEM_MUST_FREE         1=memspc MUST be freed                               INLINE
MEM_CANNOT_FREE       1=memspc CANNOT be freed                             INLINE

UME_NEW               setup                                                INLINE
UME_malloc            init Memspc (malloc, does realloc, must free)
UME_init              init with fixed or stack-space (NO realloc, no free)
UME_reset             reset Memspc
UME_free              free complete

UME_nStru_get         get memspace for rNr records of size rSiz, no reserve
UME__getSpc           reserve space, reallocate if necessary
UME__copy             save data, reallocate if necessary

UME_reserve           get memSpacePos from datasize; reserve space; NO realloc
UME_adjust            set new memSpacePos; (reserve space; set .next) NO realloc
UME_save              copy data > Memspc;  NO realloc

UME_del               remove space (toPos - fromPos)
UME_set_free          correct free space (release n bytes)

UME_get_next          get actual memSpacePosition                          INLINE
UME_get_start         get startPosition of Memspc (NULL = uninitialized)   INLINE
UME_get_used          get used space                                       INLINE
UME_ck_free           return free space
UME_ck_tot            report total space
UME_dump              print space total / used

MEM_alloc_file        allocate temp.memspace for existing file              INLINE
UME_write__           write memSpc into file
UME_read__            read file into Memspc

UME_obj_IS_VALID
UME_realloc           realloc to newSiz internal

----- funcs for MemObj ------------------------
UME_obj_reserve       reserve space in Memspc and return a MemObj
UME_obj_get           get object-position from MemObj
UME_obj_save
UME_obj_invalid_set   set MemObj=Error
UME_obj_dump          dump MemObj
PTR_MEMOBJ
MEMOBJ_PTR

UME_connect           DO NOT USE (use UME_get_next)                        INLINE
UME_add    DO NOT USE Occupy spc. NO realloc.

UME_reall_add         reserve space, reallocate if necessary   
UME_reall_save        save data, reallocate if necessary

List_functions_end:
=====================================================
- see also:
INF_MEM__
../doc/gCAD3D_prog_de.txt section Memoryfunktionen

\endcode *//*----------------------------------------




Memspc            Variable-Length-Records      UME_          ../ut/ut_umem.c

- usage examples:

--------------------------
  // malloc space; does realloc, must free
  int i1, *ipa;
  Memspc oSpc=UME_NEW;          // define (empty) memSpc;

  // malloc space for ptNr points; increment with space for 100 points if too small
  irc = UME_malloc (&oSpc, ptNr * sizeof(Point), 100 * sizeof(Point));
  if(irc < 0) return -1;

  // copy pta = array of ptNr points -> oSpc
  irc = UME__copy (&oSpc, NULL, pta, ptNr * sizeof(Point));
  if(irc < 0) return -1;

  // get spc for 10 ints; save ints
  ipa = (int*) UME__getSpc (&oSpc, NULL, sizeof(int) * 10);
  if(!ipa) return -1;
  // copy data -> oSpc
  for(i1=0;i1<10;++i1) ipa[i1] = i1;

  .. 
  UME_free (&oSpc);


--------------------------
  // get tempspace (stack, only valid in acive func, no realloc):
  Memspc   tmpSeg1;
  UME_init (&tmpSeg1, MEM_alloc_tmp (oSiz), oSiz);


--------------------------
  // get fixed space (stack, only valid in acive func, no realloc)
  int      *iTab, iNr, i1;
  char     memspc51[50000];
  UME_init (&tmpSeg1, memspc51, sizeof(memspc51));
  iTab = UME_reserve (&tmpSeg1, iNr * sizeof(int));  // reserve spc for iNr int's
  for(i1=0;i1<iNr;++i1) iTab[i1] = i1;    // write into tmpSeg1
  ...                                     // (increments tmpSeg1.next !)
  UME_set_free (-1, &tmpSeg1);            // reset tmpSeg1


\endcode *//*----------------------------------------



*/



#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                     // fuer Variad. Funktionen (...)


#include "../ut/ut_cast.h"
#include "../ut/ut_umem.h"              // Memspc
#include "../ut/ut_mem.h"               // MEM_..
#include "../ut/ut_types.h"             // MEMSPCTYP_..
#include "../ut/ut_umb.h"               // UMB_
#include "../ut/ut_TX.h"                // TX_Error

#include "../ut/AP_types.h"             // Typ_Memspc

// static int (*UME_CB__)();    // Deklaration realloc-Callback


const Memspc UME_NUL = UME_NEW;



//================================================================
  int UME_write__ (char *fnam, Memspc *memSpc) {
//================================================================
/// write memSpc into file

// see also tess_write_f_ tess_write__ 

  long l1;
  FILE *fp=NULL;


  printf("UME_write__ |%s|\n",fnam);

  fp = fopen(fnam, "wb");
  if(!fp) return -1;

  l1 = UME_get_used (memSpc); // nr of bytes to save

  fwrite (&l1, sizeof(long), 1, fp);

  fwrite (memSpc->start, l1, 1, fp);

  fclose(fp);

  return 0;

}


//================================================================
  int UME_read__ (Memspc *memSpc, char *fnam) {
//================================================================
/// \code
/// read file into Memspc;
/// Memspc must have enough free space;
/// RetCod:  0=OK; -1=memSpc_too_small; -2=file_error;
///
/// Example:
///  l1 = OS_FilSiz (fnam);
///  ..
///  UME_read__ (&tmpSpc1, fnam);
/// \endcode


  long  l1;
  void  *ps;
  FILE  *fp=NULL;


  printf("UME_read__ |%s|\n",fnam);

  fp = fopen(fnam, "rb");
  if(!fp) return -2;

  fread (&l1, sizeof(long), 1, fp);

  // clear space
  UME_set_free (-1, memSpc);

  // reserve space
  ps = UME_reserve (memSpc, l1);
  if(!ps) { // not enough space
    fclose(fp);
    return -1;
  }

  // load into space
  fread (ps, l1, 1, fp);

  fclose(fp);

  return 0;

}



/*
//================================================================
  int UME_start (Memspc **memSpc, void* objDat, int osiz) {
//================================================================
// UME_start             init Memspc; return Memspc with TimeStamp.

// puts memSpc as first Record into objDat; gives back its Adress.


  Memspc  *spc;

  // load memSpc as first record in memSpace
  spc = objDat;


  spc->start = (char*)objDat + sizeof(Memspc);
  spc->next  = spc->start;
  spc->end   = (char*)objDat + osiz;

  OS_TimeStamp (&spc->ts);

  *memSpc = spc;

  return 0;

}
*/


//=======================================================================
  int UME_realloc (long *spcOff, Memspc *memSpc, long newSiz) {
//=======================================================================
/// \code
/// UME_realloc           realloc to newSiz internal
/// 
/// Input:
///   newSiz   is new total size of memSpc
/// Output:
///   spcOff   distance from active memSpc->start to new memSpc->start
///   retCod   0=OK; -1=Error
/// \endcode


  long  actSiz;
  void  *oldAdr;


  // test if can realloc
  if((memSpc->spcTyp < MEMSPCTYP_MALLOC__)    ||
     (memSpc->spcTyp > MEMSPCTYP_MALLOC_FIX))     {

    // if no allocation yet; change to MEMSPCTYP_MALLOC__
    if(memSpc->spcTyp == MEMSPCTYP_NONE) {
      memSpc->spcTyp = MEMSPCTYP_MALLOC__;
    } else {
      DEB_dump_obj__ (Typ_Memspc, memSpc, "UME_realloc E1");
      TX_Error("UME_realloc E1");
      return -1;
    }
  }


  actSiz = (char*)memSpc->next - (char*)memSpc->start;
  oldAdr = memSpc->start;

  printf("::::UME_realloc %ld %ld %p\n",newSiz,actSiz,memSpc->start);



  memSpc->start = realloc(memSpc->start, newSiz);

  if(memSpc->start == NULL) {
    TX_Error ("UME_realloc EOM *********");
    return -1;
  }

  memSpc->next  = (char*)memSpc->start + actSiz;
  memSpc->end   = (char*)memSpc->start + newSiz;

  if(spcOff) {
    *spcOff = (char*)memSpc->start - (char*)oldAdr;
      printf(" realloc-spcOff =%ld\n",*spcOff);
  }


  // printf("ex UME_realloc %d %d %p\n",newSiz,actSiz,memSpc->start);

  return 0;

}


//=======================================================================
  int UME_init (Memspc *memSpc, void* objDat, int osiz) {
//=======================================================================
/// \code
/// UME_init              init with fixed or stack-space (no realloc, no free)
/// \endcode
// see also ../doc/gCAD3D_prog_de.txt section Memoryfunktionen



  // printf("UME_init  siz=%d pos=%p\n",osiz,objDat);
  // fflush (stdout);

  memSpc->start = objDat;
  memSpc->next  = objDat;
  // memSpc->end   = objDat + osiz;
  memSpc->end   = (char*)objDat + osiz;

  memSpc->spcTyp = MEMSPCTYP_TMP;

    // UME_dump (memSpc, "ex-UME_init");

  return 0;

}


//================================================================
  int UME_malloc (Memspc *memSpc, long spcSiz, long memInc) {
//================================================================
/// \code
/// UME_malloc            init Memspc (malloc, can realloc, must free)
/// Memspace must be freed with UME_free
/// Input:
///   spcSiz  malloc this size in bytes
///   memInc  if reallocate: increase <memInc> bytes
/// Output:
///   RetCode 0=OK; -1=outOfMemory-Error.
/// 
/// Example:
/// Memspc tmpSpc=UME_UME_NEW;
/// UME_malloc (&tmpSpc, 1000 * sizeof(int), 500 * sizeof(int));
/// ...
/// UME_free (&tmpSpc);
/// \endcode


  // printf("::::UME_malloc %ld\n",spcSiz);
  // printf("  mem=%p %p %p\n",memSpc->start,memSpc->next,memSpc->end);


  // change memInc into byte and save in incSiz
  memSpc->incSiz = UTI_round_i2b(memInc);

  memSpc->start = NULL;

  memSpc->start = realloc(memSpc->start, spcSiz);

  memSpc->next  = memSpc->start;

  if(memSpc->start == NULL) {
    TX_Error ("******** out of memory - UME_malloc *********");
    return -1;
  }

  memSpc->end   = (char*)memSpc->start + spcSiz;
  memSpc->spcTyp = MEMSPCTYP_MALLOC__;


  // printf("ex UME_malloc %d %p\n",spcSiz,memSpc->start);

  return 0;

}


/*
//================================================================
  int UME_malloc (Memspc *memSpc, long spcSiz, long memInc) {
//================================================================
  return UME_malloc_CB (memSpc, spcSiz, memInc, NULL);
}


//==============================================================================
  int UME_malloc_CB (Memspc *memSpc, long spcSiz, long memInc, int reall_CB()) {
//==============================================================================
/// ACHTUNG: nur einmal rufen - kein realloc !!!
/// UME_free


  // printf("::::UME_malloc %d\n",spcSiz);

  UME_INC  = memInc;
  UME_CB__ = reall_CB;

  memSpc->start = NULL;

  memSpc->start = realloc(memSpc->start, spcSiz);

  memSpc->next  = memSpc->start;
  memSpc->end   = (char*)memSpc->start + spcSiz;

  if(memSpc->start == NULL) {
    TX_Error ("******** out of memory - UME_malloc *********");
    return -1;
  }

  // printf("ex UME_malloc %d %p\n",spcSiz,memSpc->start);

  return 0;

}
*/


//=======================================================================
  int UME_free (Memspc *memSpc) {
//=======================================================================
/// \code
/// das komplette memseg mit free im OS freigeben !
/// auch:
///   free (memSpc->start);
/// \endcode


  // printf("::::UME_free %p\n",memSpc->start);

  if(MEM_CANNOT_FREE(memSpc->spcTyp)) return 1;

  if(memSpc->start) free (memSpc->start);

  
  *memSpc = UME_NUL;
  // memSpc->start = NULL;
  // memSpc->spcTyp = MEMSPCTYP_NONE;  // 0

  return 0;

}


//================================================================
  int UME_adjust (Memspc *memSpc, void *nxtFree) {
//================================================================
// UME_adjust            set memSpacePos (reserve space)


  if(nxtFree < memSpc->start) { TX_Error("UME_adjust E001"); return -1; }
  if(nxtFree > memSpc->end)   { TX_Error("UME_adjust E002"); return -1; }

  memSpc->next = nxtFree;

  return 0;

}



//================================================================
  void* UME_reserve (Memspc *memSpc, int osiz) {
//================================================================
/// \code
/// UME_reserve           get memSpacePos & reserve space
/// returns position for osiz data and sets this space occupied;
/// but does not copy any data.
/// Only for temp-space; does NOT reallocate.
///   RetCode      the address (starting-Position) of the memSpace;
///                NULL if EOM (not enough memspace)
/// Example:
///   ipa = (int*) UME_reserve (&oSpc, sizeof(int) * 10);
///   for(i1=0;i1<10;++i1) ipa[i1] = i1;
///
/// see also UME_nStru_get
/// \endcode

  void *actPos;

  // printf("UME_reserve siz=%d next=%d tot=%d\n",osiz,
          // memSpc->next - memSpc->start,    // next-start
          // memSpc->end - memSpc->start);    // tot = end - start

  if(!memSpc) {
    printf("UME_reserve E000\n");
    return NULL;
  }

  if(((char*)memSpc->next + osiz) > (char*)memSpc->end) {
    TX_Print("***** UME_reserve E001 - OUT OF SPACE\n");
    return NULL;
  }

  actPos = memSpc->next;

  memSpc->next = (char*)memSpc->next + osiz;

  return actPos;

}


//=======================================================================
  int UME_add (Memspc *memSpc, int osiz) {
//=======================================================================
/// \code
/// DO NOT USE (use UME_reserve)
///
/// UME_add               occupy memspc; does not save data; does NOT realloc.
///
///   RetCod   0=OK, -1=OutOfSpace
/// 
/// Example:
/// 
///   // get pointer for data;
///   Point  *pTab;
///   pTab = UME_get_next (&memSeg1);    // pTab = (Point*)memSeg1.next;
/// 
///   // reserve space for ptNr points, check if OK
///   i1 = UME_add (&memSeg1, sizeof(Point) * ptNr);
///   if(i1 < 0) goto L_eom;
/// 
///   // copy data from pta -> memSeg1    
///   memcpy (pTab, pta, sizeof(Point) * ptNr);
/// \endcode



  // printf("UME_add %d next=%d tot=%d\n",osiz,memSpc->next - memSpc->start,
          // memSpc->end - memSpc->start);


  if(memSpc == NULL) { TX_Error("UME_add E002"); return -1; }



  //memSpc->next += osiz;
  memSpc->next = (char*)memSpc->next + osiz;

  if(memSpc->next >= memSpc->end) {
    TX_Error("UME_add E001 %d",((char*)memSpc->next-(char*)memSpc->start));
    return -1;
  }

  return 0;

}


//================================================================
  int UME_reall__ (long *spcOff, Memspc *memSpc, void *newNext) {
//================================================================


  int  irc;
  long totSiz, incSiz;
  void *newEnd;


  // get size-to-increase defined in UME_malloc
  incSiz = UTI_round_b2i(memSpc->incSiz);
    // printf("UME_reall__ %ld %ld\n", newNext - memSpc->next, incSiz);

  // newEnd = (void*)((char*)memSpc->end + incSiz);
  // test if enough for addSiz
  // while(newEnd < newNext) newEnd = (void*)((char*)newEnd + incSiz);

  newEnd = (char*)newNext + incSiz;

  totSiz = (char*)newEnd - (char*)memSpc->start;
    printf("  reall__-totSiz=%ld\n",totSiz);

  return UME_realloc (spcOff, memSpc, totSiz);

}


//================================================================
 int UME_reall_add (long *spcOff, Memspc *memSpc, long addSiz) {
//================================================================
/// \code
/// UME_reall_add         reserve space, reallocate if necessary
/// add = allocate Space for <addSiz> bytes; reallocate if necessary.
/// next-Pointer is incremented <addSiz> bytes
/// Input:
///   addSiz   occupy <addSiz> bytes in memSpc
///             0: must increase memSpc
/// Output:
///   spcOff   displacement-offset; 0: no reallocate.
///   RetCode  0=OK; -1=outOfMemory-Error.
/// \endcode

  int  irc;
  long totSiz, incSiz;
  void *newNext, *newEnd;


  newNext = (char*)memSpc->next + addSiz;


  // test enough space
  if((newNext >= memSpc->end)||(addSiz == 0)) {
    // printf("     actSiz=%d\n",(char*)memSpc->end-(char*)memSpc->start);

    // realloc
    irc = UME_reall__ (spcOff, memSpc, newNext);
    if(irc < 0) return -1;

    // next ist changed; add the requested space
    newNext = (char*)memSpc->next + addSiz;

  } else {
    *spcOff = 0;
  }

  memSpc->next = newNext;

  return 0;

}


//================================================================
 void* UME__getSpc (Memspc *memSpc, long *spcOff, long addSiz) {
//================================================================
/// \code
/// UME__getSpc         reserve space, reallocate if necessary
///   allocate Space for <addSiz> bytes; reallocate if necessary.
///    next-Pointer is incremented <addSiz> bytes
/// Input:
///   addSiz   occupy <addSiz> bytes in memSpc
///            0: increase memSpc (size defined in UME_malloc)
///   spcOff   NULL = caller does not need offset
/// Output:
///   spcOff   displacement-offset; 0: no reallocate.
///   RetCode  position where to store data with size <addSiz>
///            NULL = Error
/// \endcode


  int  irc;
  long totSiz, oldSiz, incSiz;
  void *newNext, *oldNext;


  oldNext = memSpc->next;
  newNext = (char*)oldNext + addSiz;


// 2019-03-17
  if(addSiz < sizeof(void*)) {
    printf("**** UME__getSpc I1\n");
    return oldNext; 
  }


  // test enough space
  if((newNext >= memSpc->end)||(addSiz == 0)) {
    // printf("     actSiz=%d\n",(char*)memSpc->end-(char*)memSpc->start); 

    oldSiz  = (char*)memSpc->next - (char*)memSpc->start;

    // realloc (changes memSpc)
    irc = UME_reall__ (spcOff, memSpc, newNext);
    if(irc < 0) return NULL;

    // next ist changed; add the requested space
    oldNext = (char*)memSpc->start + oldSiz;
    newNext = (char*)oldNext + addSiz;
    
  } else {
    if(spcOff) *spcOff = 0;
  } 

  memSpc->next = newNext;

  return oldNext;

}


//============================================================================
  int UME__copy (Memspc *memSpc, long *spcOff, void* objDat, long osiz) {
//============================================================================
/// \code
/// UME__copy           save data, reallocate if necessary
///   next-Pointer is incremented <osiz> bytes and
///   <osiz> bytes from position <objDat> are saved into memSpc.
/// Input:
///   oSiz     nr of bytes to save
///   objDat   data to be saved in memSpc
/// Output:
///   spcOff   displacement-offset; 0: no reallocate.
///   RetCode  0=OK; -1=outOfMemory-Error.
/// \endcode


  int    irc;
  void   *vpd;
  long   l1;


  vpd = UME__getSpc (memSpc, spcOff, osiz);
  if(!vpd) return -1;

  // copy data
  memcpy (vpd, objDat, osiz);

  return 0;

}


/*
//============================================================================
  int UME_reall_save (long *spcOff, Memspc *memSpc, void* objDat, int osiz) {
//============================================================================
/// \code
/// UME_reall_save        save data, reallocate if necessary
///   next-Pointer is incremented <osiz> bytes and
///   <osiz> bytes from position <objDat> are saved into memSpc.
/// Input:
///   oSiz     nr of bytes to save
///   objDat   data to be saved in memSpc
/// Output:
///   spcOff   displacement-offset; 0: no reallocate.
///   RetCode  0=OK; -1=outOfMemory-Error.
/// \endcode


  int  irc;
  long totSiz, incSiz;
  void *newNext, *newEnd;

  // printf("UME_save siz=%d next=%d tot=%d\n",osiz,
          // memSpc->next - memSpc->start,    // next-start
          // memSpc->end - memSpc->start);    // tot = end - start


  newNext = (char*)memSpc->next + osiz;

  *spcOff = 0;

  if((char*)newNext < (char*)memSpc->end) goto L_save;
    // printf("     actSiz=%d\n",(char*)memSpc->end-(char*)memSpc->start);

    // (char*)newEnd = (char*)memSpc->end + UME_INC;
    incSiz = UTI_round_b2i(memSpc->incSiz);
      printf("UME_reall_add %d %ld\n",osiz,incSiz);
    newEnd = (void*)((char*)memSpc->end + incSiz);

    // while(newEnd < newNext) (char*)newEnd += UME_INC;
    while(newEnd < newNext) newEnd = (void*)((char*)newEnd + incSiz);

    totSiz = (char*)newEnd - (char*)memSpc->start;
    // printf("     totSiz=%d %d\n",totSiz,UME_INC);

    irc = UME_realloc (spcOff, memSpc, totSiz);
    if(irc < 0) { TX_Error("UME_reall_save EOM"); return irc;}

    // next ist changed; add the requested space
    newNext = (char*)memSpc->next + osiz;


  L_save:
  // copy data
  memcpy(memSpc->next, objDat, osiz);

  // update pointer to free space
  memSpc->next = (char*)newNext;

  return 0;

}
*/


//=======================================================================
  void* UME_save (Memspc *memSpc, void* objDat, int osiz) {
//=======================================================================
/// \code
/// UME_save              copy data > Memspc; does NOT realloc
/// RetCode: the address (starting-Position) of the memSpace;
///          NULL if EOM
/// \endcode

  void *actPos;


  // printf("UME_save siz=%d used=%d tot=%d\n",osiz,
          // memSpc->next - memSpc->start,    // next-start
          // memSpc->end - memSpc->start);    // tot = end - start

  actPos = memSpc->next;

  if(((char*)memSpc->next + osiz) > (char*)memSpc->end) return NULL;

  memcpy(memSpc->next, objDat, osiz);

  memSpc->next = (char*)memSpc->next + osiz;

  return actPos;

}


//=======================================================================
  int UME_ck_free (Memspc *memSpc) {
//=======================================================================
/// \code
/// UME_ck_free           return free space
/// Example:
///  pTabMaxSiz = (UME_ck_free(memSeg1) / sizeof(Point)) - 1;
/// \endcode


  int i1;

  // i1 = memSpc->end - memSpc->next;
  i1 = (char*)memSpc->end - (char*)memSpc->next;


  // printf("ex UME_ck_free %d\n",i1);

  return i1;

}

//================================================================
  int UME_ck_tot (Memspc *memSpc) {
//================================================================
/// UME_ck_tot            report total space

  return (char*)memSpc->end - (char*)memSpc->start;

}


//=======================================================================
  int UME_set_free (int sizFree, Memspc *memSpc) {
//=======================================================================
/// \code
/// UME_set_free          correct free space (release n bytes)
/// 
/// usage:
/// UME_set_free (sSiz, wrkSpc);    // release sSiz bytes)
///   sSiz       >0    cut off <sSiz> from end of data
///              -1    clear whole data
/// \endcode


  // int i1;

  if(sizFree < 0) {
    memSpc->next = memSpc->start;

  } else {
    // (char*)memSpc->next = (char*)memSpc->end - sizFree;
    memSpc->next = (void*)((char*)memSpc->end - sizFree);
  }

  // printf("ex UME_ck_free %d\n",i1);

  return 0;

}




//=======================================================================
  int UME_dump (Memspc *memSpc, char *txt) {
//=======================================================================
/// UME_dump              print space total / used


  printf("%s Memspc- used=%ld total=%ld\n",txt,
       (long)((char*)memSpc->next - (char*)memSpc->start),
       (long)((char*)memSpc->end  - (char*)memSpc->start));

  return 0;

}


//========================================================================
  int UME_del (Memspc *memSpc, void *nach, void *von, ...) {
//========================================================================
/// \code
/// Datenbereich (u Pointer) loeschen (nach links verschieben);
///   "nach" muss also kleiner als "von" sein !!
/// \endcode

// nach-Adresse (muss links von von-Adresse liegen!)
// von-Adresse  (der Datenbereich von "von" bis memSpc->next wird verschoben)
// ... (void**) zu verschiebende Pointer, mit NULL terminiert. Diese Pointer
//              werden um (von - nach) verschoben. Diese Pointer muessen als
//              (void**) uebergeben werden !!
// NULL         Ende der Pointerliste.
//
//
// Beispiel:
//   Im Memspc m1 liegen die Punkte p1, p2, p3 und p4.
//   p1 und p2 werden nicht mehr gebraucht und geloescht.
// char   myMemSpc[1000];
// Memspc myMemSeg;
// Point  *p1, *p2, *p3, *p4;
// UME_init (&myMemSeg, myMemSpc, sizeof(myMemSpc));
// p1=myMemSeg.next; if(UME_add(&myMemSeg,sizeof(Point)) < 0) goto L_outOfMem;
// p2=myMemSeg.next; if(UME_add(&myMemSeg,sizeof(Point)) < 0) goto L_outOfMem;
// p3=myMemSeg.next; if(UME_add(&myMemSeg,sizeof(Point)) < 0) goto L_outOfMem;
// p4=myMemSeg.next; if(UME_add(&myMemSeg,sizeof(Point)) < 0) goto L_outOfMem;
// UME_del (&myMemSeg, p1, p3, &p3, &p4, NULL);
// // den Memorybereich ab p3 nach links bis p1 verschieben.
// // Dann die Pointer p3 u. p4 korrigieren.
// // Vorher:  |p1   |p2   |p3   |p4   |next              |end
// // Nachher: |p3   |p4   |next                          |end



  long    sizMov, sizDat;
  char    **vPtr;
  va_list va;

  sizMov = (char*)von - (char*)nach;
  sizDat = (char*)memSpc->end  - (char*)von;
  printf("UME_del sizMov=%ld sizDat=%ld\n",sizMov,sizDat);


  
  // Daten verschieben ....
  // memmove(nach, von, size)
  memmove (nach, von, sizDat);
  // memcpy (nach, von, sizDat);



  // Memspce-interne-pointer korrigieren
  // (char*)memSpc->next -= sizMov;
  memSpc->next = (void*)((char*)memSpc->next - sizMov);
  UME_dump (memSpc, "UME_del:");



  // die nun folgenden Pointer into Memspc korrigieren ....
  va_start (va, von);
  vPtr = va_arg (va, char**);


  while(vPtr) {
    *vPtr -= sizMov; // MUSS (char*) sonst wird Obj.Size angenommen
    vPtr = va_arg (va, char**);
  }


  va_end (va); // wozu ?

  return 0;

}


//=======================================================================
  MemObj UME_obj_reserve (void **oPos, Memspc *mSpc, int siz, int mbId) {
//=======================================================================
// reserve <siz> bytes in mSpc; return its adress (in mSpc); and
//   return its position as MemObj.

  MemObj  mo;

  // printf("UME_obj_reserve %d\n",siz);

  *oPos = UME_reserve (mSpc, siz);
  if(! *oPos) return (UME_obj_invalid_set (-1));

  mo.mbID = mbId;
  // mo.ioff = *oPos - mSpc->start;
  mo.ioff = (char*)*oPos - (char*)mSpc->start;

  return (mo);

}


//================================================================
  int UME_obj_save (int mbId, Memspc *mSpc, int recNr) {
//================================================================
/// save Memspc -> memoryblock (malloc & memcpy)

  long    mSiz;
  void    *mPos;

  // save tempSpace -> memBlock

  mPos = UME_get_start (mSpc);
  mSiz = UME_get_used (mSpc);

  return UMB_save (mbId, mPos, mSiz, recNr);

}


//================================================================
  void* UME_obj_get (MemObj *mo) {
//================================================================
/// \code
/// get object-position from MemObj
/// MemObj contains memblock-nr and the data-offset in this memoryblock.
/// \endcode

  void  *gBas;


  // get baseAdress of memSpc for memblock <mbID>
  gBas = UMB_pos__ (mo->mbID);

  if(!gBas) {
    printf("***** UME_obj_get E001\n");
    return NULL;
  }


  // return address of obj gb
  // return gBas + mo->ioff;
  return (char*)gBas + mo->ioff;

}


//================================================================
  MemObj UME_obj_invalid_set (int iErr) {
//================================================================
/// \code
/// Set MemObj to Error.
/// iErr must be 0-3
/// -4  0   uninitialized or deleted;
/// -3  1   error with parent-object
/// -2  2   application-error
/// -1  3   out-of-space-error
/// \endcode

// mo.ioff   content 0-3 is Error; >3 is OK. unsigned.

// -4 ?
// -3 GUI_obj_typ
//    GUI_obj_parentBox
// -1 GUI_obj_init UME_obj_save UMB_save
//    GUI_obj_spc UME_reserve

  MemObj  mo;

  // mo.mbID = iErr;
  mo.ioff = iErr + 4;

  return (mo);

}


//=====================================================================
  int UME_nStru_get (void **ptr, int rNr, int rSiz, Memspc *memSpc) {
//=====================================================================
// UME_nStru_get         get memspace for rNr records of size rSiz
// Input:
//   rNr      0=reserve total space, else get space for <rNr> structs
//   rSiz     size of struct in bytes
// Output:
//   ptr      memspc for <retCod> structs in memSpc
//   retCod   -1=EOM-error; else nr of allocated structs

  int    oNr;

  printf("UME_npt_get__ rNr=%d rSiz=%d\n",rNr,rSiz);

  oNr = (UME_ck_free (memSpc) / rSiz) - 1;

  if(oNr < rNr) return -1;

  if(!rNr) rNr = oNr;

  // *ptr = UME_reserve (memSpc, rNr * rSiz);
  *ptr = memSpc->next;

  return rNr;

}


//================================================================
  void UME_obj_dump (MemObj *mo) {
//================================================================
// UME_obj_dump          dump MemObj


  printf("UME_obj_dump  mbID=%d ioff=%d\n",
      mo->mbID, mo->ioff);

}


//======================== EOF ======================================*/
