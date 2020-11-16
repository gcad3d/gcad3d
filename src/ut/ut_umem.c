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
- all MemObj -funcs (eg UME_obj_get) into separate source

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
       ../ut/ut_umem.h   struct Memspc
\brief variable-length-records in memory: reserve/connect/release .. UME_
\code
=====================================================
List_functions_start:

MEM_alloc_tmp         allocate mem for active function (temp.space)
MEM_MUST_FREE         1=memspc MUST be freed                               INLINE
MEM_CANNOT_FREE       1=memspc CANNOT be freed                             INLINE
MEM_CANNOT_ALLOC      1=memspc CANNOT be (re)allocated                     INLINE
MEM_CAN_ALLOC         1=memspc CAN be (re)allocated                        INLINE

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
UME_add_obj           add oNr objs into Memspc (serialize)
UME_add_nRec           copy data from pos into memSpc, update pointer <pos>

UME_del               remove space (toPos - fromPos)
UME_set_unused          correct free space (release n bytes)

UME_get_next          get actual memSpacePosition                          INLINE
UME_get_start         get startPosition of Memspc (NULL = uninitialized)   INLINE
UME_get_used          get used space                                       INLINE
UME_ck_free           return free space
UME_ck_tot            report total space

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
UME_add     DO NOT USE Occupy spc. NO realloc (use UME_reserve)
UME_cp_obj  DO NOT USE; replaced by UME_add_obj              

UME_reall_add         reserve space, reallocate if necessary   
UME_reall_save        save data, reallocate if necessary

UME_dump              print space total / used
UME_test1             test UME_add_obj

List_functions_end:
=====================================================
- see also:
INF_MEM__
../doc/gCAD3D_prog_de.txt section Memoryfunktionen

UME_add_oxt           replaced by UME_add_obj

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
  UME_set_unused (-1, &tmpSeg1);            // reset tmpSeg1


\endcode *//*----------------------------------------



*/



#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                     // fuer Variad. Funktionen (...)


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_cast.h"
#include "../ut/ut_umb.h"               // UMB_
#include "../ut/ut_ox_base.h"          // OGX_SET_..
#include "../ut/ut_TX.h"                // TX_Error

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
  UME_set_unused (-1, memSpc);

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


  if(memSpc->spcTyp == MEMTYP_NONE) memSpc->spcTyp = MEMTYP_ALLOC__;


  // test if can realloc
  // if((memSpc->spcTyp < MEMSPCTYP_MALLOC__)    ||
     // (memSpc->spcTyp > MEMSPCTYP_MALLOC_FIX))     {

    // if no allocation yet; change to MEMSPCTYP_MALLOC__
  if(MEM_CANNOT_ALLOC(memSpc->spcTyp)) {
    DEB_dump_obj__ (Typ_Memspc, memSpc, "UME_realloc E1");
    TX_Error("UME_realloc E1");
    return -1;
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
// UME_init              init with fixed or stack-space (no realloc, no free)
//
// Examples:
// Memspc m1Spc = _MEMSPC_NUL; int siz = 16000; char spc[4000];
// // get stack-spaces - is lost at return of func. Cannot free, cannot realloc.
// UME_init (&m1Spc, MEM_alloc_tmp (siz), siz);
// UME_init (&m2Spc, spc1, sizeof(spc1));
// 
// see also INF_MEM__
// TODO:
//   make also UME_init_fe (fixed-expandable (MEMTYP_ALLOC_EXPND))



  // printf("UME_init  siz=%d pos=%p\n",osiz,objDat);
  // fflush (stdout);

  memSpc->start = objDat;
  memSpc->next  = objDat;
  // memSpc->end   = objDat + osiz;
  memSpc->end   = (char*)objDat + osiz;

  memSpc->spcTyp = MEMTYP_STACK__;

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
  memSpc->spcTyp = MEMTYP_ALLOC__;


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

  
  *memSpc = UME_NUL;             // {NULL, NULL, NULL, 0}
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
  void* UME__copy (Memspc *memSpc, long *spcOff, void* objDat, long osiz) {
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
///   RetCode  position of copied data in memSpc; NULL=outOfMemory-Error.
/// \endcode


  int    irc;
  void   *vpd;
  long   l1;


  vpd = UME__getSpc (memSpc, spcOff, osiz);
  if(!vpd) return NULL; // -1;

  // copy data
  memcpy (vpd, objDat, osiz);

  return vpd;

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


//======================================================================
  int UME_add_nRec (Memspc *memSpc, void **pos, int recNr, int sizRec) {
//======================================================================
// UME_add_nRec       copy data from pos into memSpc, update pointer <pos>
// 
// see UME_cpy_nRec UME__copy MEM_copy_oTab

  long   osiz, spcOff;
  void   *vpd;



  printf("UME_add_nRec recNr=%d sizRec=%d\n",recNr,sizRec);
    printf(" pos-1=%p\n",*pos);

  osiz = recNr * sizRec;

  vpd = UME__getSpc (memSpc, &spcOff, osiz);
  if(!vpd) return -1;
  if(spcOff) printf("***** UME_add_nRec-realloc \n");

  // copy data
  memcpy (vpd, *pos, osiz);

  // update ptr
  *pos = vpd;

    printf(" pos-2=%p\n",*pos);

  return 0;

}


/* replaced by UME_add_obj
//==================================================================
  int UME_add_oxt (Memspc *mSpc, void **pObj, ObjGX *oxa, int oNr) {
//==================================================================
// UME_add_oxt               add obj-tree into Memspc (serialize)

// see also OXMT_add_oxt


  int      irc, i1;
  long     l1, osiz;
  void     *pDat;
  ObjGX    *oxo, *ox1;


  printf("\n::::::::::::::::::::::::::::::::::::::::::::::: \n");
  DEB_dump_obj__ (Typ_Memspc, mSpc, "UME_add_oxt-in\n");
  DEB_dump_nobj__ (Typ_ObjGX, oNr, oxa, "UME_add_oxt-in\n");


  // first copy all ObjGX-records
  osiz = oNr * sizeof(ObjGX);
  oxo = UME__getSpc (mSpc, &l1, osiz);
  if(!oxo) {TX_Error("UME_add_oxt EOM"); return -1;}

  // copy all ObjGX-records
  memcpy (oxo, oxa, osiz);


  // loop tru records, add data into oxtb->spc
  for(i1=0; i1<oNr; ++i1) {
    ox1 = &oxo[i1];
      DEB_dump_obj__ (Typ_ObjGX, ox1, " add_oxt-ox1[%d]",i1);

    // test if ObjGX-record has extern-data to store,
    if(!ox1->data) continue;

// TODO: if obj is ObjGX recurse
    if(ox1->form == Typ_ObjGX) {TX_Error("UME_add_oxt TODO-1"); return -1;}

    // copy the objects
    pDat = UME_add_obj (mSpc, ox1->form, ox1->siz, ox1->data);
    if(!pDat) return -1;

    // update ObjGX-pointer
    ox1->data = pDat;
  }

  *pObj = oxo;

    DEB_dump_nobj__ (Typ_ObjGX, oNr, oxo, "ex-UME_add_oxt\n");
    // DEB_dump_ox_s_ (oxo, "ex-UME_add_oxt");

  return 0;

}
*/

//============================================================================
  int UME_add_obj (Memspc *memSpc, void **po, int form, int oNr, void *data) {
//============================================================================
// UME_add_obj         add oNr objs into Memspc (serialize)
//   does resolv subObjs of ObjGX
// Input:
//   oNr   nr of child-record of struct <form> in data copy with subObjs
// Output:
//   memSpc  data is added here
//   retCod  pointer to copied obj in spc; NULL=error;
// see UME_cp_obj

  int     irc, i1, xSiz, sNr;
  void    *pStart, *p1;
  ObjGX   *ox1;


  printf("--------- UME_add_obj form=%d oNr=%d\n",form,oNr);
  DEB_dump_nobj__ (form, oNr, data, "UME_add_obj-in");
  if(form == Typ_ObjGX) DEB_dump_ox_s_ (data, "UME_add_obj-in");
  pStart = data;


  //----------------------------------------------------------------
  switch (form) {

  //----------------------------------------------------------------
  case Typ_VC:
    xSiz = sizeof(Vector);
    goto L_cpy_noPtr;

  //----------------------------------------------------------------
  case Typ_PT:
    xSiz = sizeof(Point);
    goto L_cpy_noPtr;

  //----------------------------------------------------------------
  case Typ_LN:
    xSiz = sizeof(Line);
    goto L_cpy_noPtr;

  //----------------------------------------------------------------
  case Typ_CI:
    xSiz = sizeof(Circ);
    goto L_cpy_noPtr;

  //----------------------------------------------------------------
  case Typ_CVELL:
    xSiz = sizeof(CurvElli);
    goto L_cpy_noPtr;

  //----------------------------------------------------------------
  case Typ_CVPOL:
    if(oNr != 1) goto L_errNr;
    if(UME_add_nRec (memSpc, &pStart, oNr, sizeof(CurvPoly)) < 0) goto L_err_ex;
    //............................
    // copy cpTab
    oNr = ((CurvPoly*)pStart)->ptNr;
    if(UME_add_nRec (memSpc,
                    (void**)&(((CurvPoly*)pStart)->cpTab),
                    oNr,
                    sizeof(Point)) < 0) goto L_err_ex;
    //............................
    // copy lvTab
    if(UME_add_nRec (memSpc,
                    (void**)&(((CurvPoly*)pStart)->lvTab),
                    oNr,
                    sizeof(double)) < 0) goto L_err_ex;
    break;

//   //-------------------------------------------------------
//   case Typ_CVBSP:   see OGX_ox_copy_obj
//     if(oNr != 1) goto L_errNr;
//     xSiz = sizeof(Typ_CVBSP);
//       if(OGX_reloc_adr ((void**)&((CurvBSpl*)po)->kvTab, rd)) goto L_err_adr;
//       if(OGX_reloc_adr ((void**)&((CurvBSpl*)po)->cpTab, rd)) goto L_err_adr;
// 
// 
//   //-------------------------------------------------------
//   case Typ_SURBSP:   see OGX_ox_copy_obj
//     if(oNr != 1) goto L_errNr;
//     xSiz = sizeof(?);
//       if(OGX_reloc_adr ((void**)&((SurBSpl*)po)->cpTab, rd)) goto L_err_adr;
//       if(OGX_reloc_adr ((void**)&((SurBSpl*)po)->kvTabU, rd)) goto L_err_adr;
//       if(OGX_reloc_adr ((void**)&((SurBSpl*)po)->kvTabV, rd)) goto L_err_adr;


  //-------------------------------------------------------
  case Typ_GTXT:
    if(oNr != 1) goto L_errNr;
    if(UME_add_nRec (memSpc, &pStart, oNr, sizeof(GText)) < 0) goto L_err_ex;

    //............................
    // copy txt
    // set position of data in primary-obj
    ((GText*)pStart)->txt = UME_get_next (memSpc);
    // copy
    p1 = ((GText*)data)->txt;
    sNr = UTI_div4up (strlen((char*)p1) + 1);
    if(UME_add_nRec (memSpc, &p1, sNr, 1) < 0) goto L_err_ex;
    break;

  //-------------------------------------------------------
  case Typ_ObjGX:
    xSiz = sizeof(ObjGX);
    // copy the primary objs
    if(UME_add_nRec (memSpc, &pStart, oNr, xSiz) < 0) goto L_err_ex;

    // copy all childs
    //   address of childs is ox.data;
    for(i1=0; i1<oNr; ++i1) {
      ox1 = &((ObjGX*)pStart)[i1];
        DEB_dump_obj__ (ox1->form, ox1->data, "add_obj-rec-%d",i1);
      // ignore all primary ObjGX-Records where data ist not pointer
      if(!ox1->data)                     continue;
      if((ox1->form == Typ_Index) ||
         (ox1->form == Typ_Int4)  ||
         (ox1->form == Typ_Typ))         continue;
      // recurse
      irc = UME_add_obj (memSpc, &ox1->data, ox1->form, ox1->siz, ox1->data);
      if(irc < 0) return irc;
    }
    break;

  //----------------------------------------------------------------
  default:
    TX_Error("UME_add_obj E002 - unsupported struct %d\n",form);
    goto L_err_ex;
  }

  goto L_exit;


  //----------------------------------------------------------------
  L_cpy_noPtr:
    // copy struct with no pointers
    if(UME_add_nRec (memSpc, &pStart, oNr, xSiz) < 0) goto L_err_ex;



  //================================================================
  L_exit:
    *po = pStart;

    // TESTBLOCK
    printf("ex-UME_add_obj\n");
    DEB_dump_obj__ (form, pStart, "ex-UME_add_obj");
      // // DEB_dump_ox_0 (objo, "ex OGX_ox_copy_obj");
    // END TESTBLOCK

    return 0;



  //================================================================
  L_errNr:
    TX_Error("UTO__rdf_ox E002 - mult.objs not supp.\n");

  L_err_ex:
    return -1;



}


//======================================================================
  int UME_cp_obj (Memspc *memSpc, int form, void *obj) {
//======================================================================
// DO NOT USE; replaced by UME_add_obj
// UME_cp_obj                  copy data of struct of type <form>
//   Copy all data of struct <obj> into memSpc and update the pointers of <obj>
//   Eg form=CurvBSpl: copy cpTab and kvTab
// Input
//   form    type of struct <obj>
//   obj     struct with pointers. Copy all date into memSpc and
//           update the pointers.
// see OGX_ox_copy_obj



  DEB_dump_obj__ (form, obj, "\nUME_cp_obj-in");


  //----------------------------------------------------------------
  if(form == Typ_CVPOL) {     // DB_StoreCvPlg
    printf(" lv-1=%p\n",((CurvPoly*)obj)->lvTab);


    // copy lvTab --> objo
    if(((CurvPoly*)obj)->lvTab) {

      if(UME_add_nRec (memSpc,
                      (void**)&((CurvPoly*)obj)->lvTab,
                      ((CurvPoly*)obj)->ptNr,
                      sizeof(double)) < 0) goto L_err_ex;
    }
    printf(" lv-2=%p\n",((CurvPoly*)obj)->lvTab);
    // copy cpTab --> objo
    if(UME_add_nRec (memSpc,
                    (void**)&((CurvPoly*)obj)->cpTab,
                    ((CurvPoly*)obj)->ptNr,
                    sizeof(Point)) < 0) goto L_err_ex;

    printf(" lv-3=%p\n",((CurvPoly*)obj)->lvTab);

  //-------------------------------------------------------
  } else if(form == Typ_CVBSP) {

    // save knotvals
    if(UME_add_nRec (memSpc,
                    (void**)&((CurvBSpl*)obj)->kvTab,
                    ((CurvBSpl*)obj)->ptNr + ((CurvBSpl*)obj)->deg + 1,
                    sizeof(double)) < 0) goto L_err_ex;

    // save controlpoints
    if(UME_add_nRec (memSpc,
                    (void**)&((CurvBSpl*)obj)->cpTab,
                    ((CurvBSpl*)obj)->ptNr,
                    sizeof(Point)) < 0) goto L_err_ex;


  //-------------------------------------------------------
  } else if(form == Typ_SURBSP) {

    // save cpTab
    if(UME_add_nRec (memSpc,
                    (void**)&((SurBSpl*)obj)->cpTab,
                    ((SurBSpl*)obj)->ptUNr * ((SurBSpl*)obj)->ptVNr,
                    sizeof(Point)) < 0) goto L_err_ex;

    // save kvTabU
    if(UME_add_nRec (memSpc,
                    (void**)&((SurBSpl*)obj)->kvTabU,
                    ((SurBSpl*)obj)->ptUNr + ((SurBSpl*)obj)->degU + 1,
                    sizeof(double)) < 0) goto L_err_ex;

    // save kvTabV
    if(UME_add_nRec (memSpc,
                    (void**)&((SurBSpl*)obj)->kvTabV,
                    ((SurBSpl*)obj)->ptVNr + ((SurBSpl*)obj)->degV + 1,
                    sizeof(double)) < 0) goto L_err_ex;


  //-------------------------------------------------------
  } else if(form == Typ_GTXT) {
      // printf(" Typ_GTXT |%s|\n",((GText*)obj)->txt);

    // copy text (increase to a multiple of 4)
    if(UME_add_nRec (memSpc,
                    (void**)&((GText*)obj)->txt,
                    UTI_div4up(strlen(((GText*)obj)->txt) + 1),
                    1) < 0) goto L_err_ex;


  //----------------------------------------------------------------
/*
  } else if(form == Typ_Model) {
      printf(" Typ_Model |%s| iSiz=%d\n",((ModelRef*)pi)->mnam,iSiz);
    // copy modelname
    ((ModelRef*)obj)->mnam = *objo;
    memcpy (*objo, ((ModelRef*)pi)->mnam, 4);
    *oSiz -= 4;
    *objo = (char*)*objo + 4; // *objo += 4;
      DEB_dump_obj__ (Typ_Model, obj, "model");
*/

  //----------------------------------------------------------------
  } else {
    TX_Error("UME_cp_obj E002 - unsupported struct %d\n",form);
    goto L_err_ex;
  }


  //================================================================
  L_exit:
      // DEB_dump_ox_0 (objo, "ex OGX_ox_copy_obj");
      DEB_dump_obj__ (form, obj, "\nex-UME_cp_obj");
    return 0;

  //----------------------------------------------------------------
  L_err_ex:
    return -1;

}


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
  int UME_set_unused (int sizFree, Memspc *memSpc) {
//=======================================================================
/// \code
/// UME_set_unused          correct free space (release n bytes)
/// 
/// usage:
/// UME_set_unused (sSiz, wrkSpc);    // release sSiz bytes)
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


  printf("%s Memspc- used=%ld total=%ld spcTyp=%d\n",txt,
       (long)((char*)memSpc->next - (char*)memSpc->start),
       (long)((char*)memSpc->end  - (char*)memSpc->start),
       memSpc->spcTyp);

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
//   but do not (yet) reserve space;
// Input:
//   rNr      0=reserve total space, else get space for <rNr> structs
//   rSiz     size of struct in bytes
// Output:
//   ptr      memspc for <retCod> structs in memSpc
//   retCod   -1=EOM-error; else nr of allocated structs

  int    oNr;


  // printf("UME_nStru_get rNr=%d rSiz=%d\n",rNr,rSiz);

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


//================================================================
  int UME_test1 () {
//================================================================
// UME_test1           test UME_add_obj

#define TMPSIZ 4000

  int      irc; 
  Point    pp1[]={600., 0., 0., 650., 0., 0.,
                  650.,20., 0., 600.,20., 0., 600., 0., 0.,};
  CurvPoly plg1;
  void     *p1, *p2, *p3, *p4;
  ObjGX    ox1;
  Memspc   mSpc = _MEMSPC_NUL, tmpSpc = _MEMSPC_NUL;


  irc = UME_malloc (&mSpc, 16000, 16000);
    UME_dump (&mSpc, "\nUME_test1-in");

  UME_init (&tmpSpc, MEM_alloc_tmp(TMPSIZ), TMPSIZ);

  // copy Points into ObjGX-record
  irc = UME_add_obj (&mSpc, &p1, Typ_PT, 5, pp1);
  if(irc < 0) return -1;
    DEB_dump_nobj__ (Typ_PT, 5, p1, "f-add_obj-p1");

  // copy DB-Link to P123 into ObjGX-record
  OGX_SET_INDEX (&ox1, Typ_PT, 123L);
  irc = UME_add_obj (&mSpc, &p2, Typ_ObjGX, 1, &ox1);
  if(irc < 0) return -1;
    DEB_dump_obj__ (Typ_ObjGX, p2, "UME_test1-p2");

  // copy polygon
  UT3D_plg_pta (&plg1, pp1, 5, &tmpSpc);
  irc = UME_add_obj (&mSpc, &p3, Typ_CVPOL, 1, &plg1);
  if(irc < 0) return -1;
    DEB_dump_obj__ (Typ_ObjGX, p3, "UME_test1-p3");

  // copy polygon
  UT3D_plg_pta (&plg1, pp1, 5, &tmpSpc);
  OGX_SET_OBJ (&ox1, Typ_CVPOL, Typ_CVPOL, 1, &plg1);
  irc = UME_add_obj (&mSpc, &p4, Typ_ObjGX, 1, &ox1);
  if(irc < 0) return -1;
    DEB_dump_ox_s_ (p4, "UME_add_obj-p4");


  //----------------------------------------------------------------
  L_exit:
  UME_free (&mSpc);

  return 0;

}


//======================== EOF ======================================*/
