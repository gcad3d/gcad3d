/*
 * ../ut/ut_umem.c
 *
 * Copyright (C) 2012 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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


-----------------------------------------------------
Store variable-length-records in memory: reserve/connect/release .. UME_

TODO:
UME_alloc   get temp. memspace & init.
  MUST BE INLINE; after return of function memspace is lost !
 eg: UME_init (&memSeg1, UME_alloc_tmp (oSiz), oSiz);
 int* UME_alloca (Memspc *memSpc, long spcSiz);
#define UME_alloca(memSeg,memsiz)\
  UME_init (memSeg, UME_alloc_tmp(memsiz), memsiz)


-----------------------------------------------------
Modifications:
2011-0610  UME_reset replaced by UME_adjust
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
\file ../ut/ut_umem.c
\ingroup grp_ut_obj
\code

=====================================================
Liste_Funktionen_Beginn:

UME_init              init Memspc with existing space
UME_malloc            init Memspc, malloc
UME_realloc
UME_free
UME_alloca            allocate temp. space for Memspc (until functions ends)

UME_save              save Obj -> Memspc
UME_reall_save        save and reallocate if necessary
UME_reall_add         add and reallocate if necessary

UME_del               remove space (toPos - fromPos)

UME_reserve           get memSpacePos & reserve space
UME_adjust            set memSpacePos (reserve space; set .next)
UME_set_free          correct free space (release n bytes)

UME_get_next          get actual memSpacePosition                 INLINE
UME_get_start         get startPosition of Memspc (NULL = uninitialized) INLINE
UME_get_used          get used space                              INLINE
UME_ck_free           return free space
UME_ck_tot            report total space
UME_dump              display free space

UME_alloc_tmp         allocate mem for active function (temp.space)

UME_obj_reserve       reserve space in Memspc and return a MemObj
UME_obj_get           get object-position from MemObj
UME_obj_invalid_set   set MemObj=Error
UME_obj_dump          dump MemObj

UME_connect           DO NOT USE (use UME_get_next)               INLINE
UME_add               DO NOT USE (use UME_reserve)
UME_NEW               setup. unused

Liste_Funktionen_Ende:
=====================================================

- see also:
../doc/gCAD3D_prog_de.txt section Memoryfunktionen

- usage examples:

  Memspc   memSeg1;
  int      *iTab;

  // tempspace :
  UME_init (&memSeg1, UME_alloc_tmp (oSiz), oSiz);

--------------------------
  or:
  char     memspc51[50000];
  UME_init (&memSeg1, memspc51, sizeof(memspc51));
  UME_connect (iTab, &memSeg1);           // give space to iTab
  iSiz = UME_ck_free (&memSeg1);          // query free size in bytes
  UME_reserve (&memSeg1, iNr * sizeof(int));  // reserve spc for iNr long's
  ...                                     // (increments memSeg1.next !)
  UME_release (iTab, &memSeg1);           // reset memSeg1

--------------------------
  or:
  // query max nr of points in &memSeg1
  ipMax = UME_ck_free (&memSeg1) / sizeof(Point);
  // reserve space for ipMax points in memSeg1
  pTab = UME_reserve (&memSeg1, ipMax * sizeof(Point));

--------------------------
  or:
  // need max nr of points + parameters for all points ..
  paSiz = UME_ck_free (&memSeg1) / (sizeof(Point) + sizeof(double));
  pTab = UME_reserve (&memSeg1, paSiz * sizeof(Point));
  UME_connect (vTab, &memSeg1);


\endcode *//*----------------------------------------



=====================================================
compile mit gcc:
cc -c -O0 -g3 -Wall ut_umem.c

Windows:
cl -c ut_umem.c

ctags -f ut_umem.tag ut_umem.c

*/


// #include "../xa/MS_Def0.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                     // fuer Variad. Funktionen (...)


#include "../ut/ut_cast.h"
#include "../ut/ut_umem.h"              // Memspc
#include "../ut/ut_umb.h"               // UMB_
#include "../ut/ut_TX.h"                // TX_Error



// static int (*UME_CB__)();    // Deklaration realloc-Callback



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
/// memspc vergroessern; die Pointer start, next und end korrigieren.
/// Input:
///   newSiz   is new total size of memSpc
/// Output:
///   spcOff   distance from active memSpc->start to new memSpc->start
/// \endcode


  long  actSiz;
  void  *oldAdr;


  actSiz = (char*)memSpc->next - (char*)memSpc->start;

  printf("::::UME_realloc %ld %ld %p\n",newSiz,actSiz,memSpc->start);


  oldAdr = memSpc->start;

  memSpc->start = realloc(memSpc->start, newSiz);

  if(memSpc->start == NULL) {
    TX_Error ("******** out of memory - UME_realloc *********");
    return -1;
  }


/* muss nun manuell gemacht werden !
  if(UME_CB__) {
    if(oldAdr != memSpc->start) {
      if(UME_CB__ (oldAdr, memSpc->start) < 0) {
        TX_Error ("UME_malloc_CB E001");
        return -1;
      }
    }
  }
*/


  memSpc->next  = (char*)memSpc->start + actSiz;
  memSpc->end   = (char*)memSpc->start + newSiz;

  *spcOff = (char*)memSpc->start - (char*)oldAdr;


  // printf("ex UME_realloc %d %d %p\n",newSiz,actSiz,memSpc->start);

  return 0;

}


//================================================================
  int UME_malloc (Memspc *memSpc, long spcSiz, long memInc) {
//================================================================
/// \code
/// malloc <spcSiz> bytes and provide it in memSpc.
/// Call UME_malloc only once!
/// Memspace must be freed with UME_free
/// Input:
///   spcSiz  malloc this size
///   memInc  if reallocate: increase <memInc> bytes
/// Output:
///   RetCode 0=OK; -1=outOfMemory-Error.
/// 
/// Example:
/// Memspc tmpSpc=UME_UME_NEW;
/// UME_malloc (&tmpSpc, 100 *sizeof(int), 50 *sizeof(int));
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
  memSpc->end   = (char*)memSpc->start + spcSiz;

  if(memSpc->start == NULL) {
    TX_Error ("******** out of memory - UME_malloc *********");
    return -1;
  }


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

  if(memSpc->start) free (memSpc->start);

  memSpc->start = NULL;

  // UME_CB__ = NULL;

  return 0;

}


//=======================================================================
  int UME_init (Memspc *memSpc, void* objDat, int osiz) {
//=======================================================================
/// \code
/// UME_init              init Memspc
/// 
/// puts memSpc as first Record into objDat; gives back its Adress.
/// 
///   Examples:
/// char     memspc51[50000];
/// Memspc   memSeg1;
/// int      *iTab, sizTab;
/// // attach memspc51
/// UME_init (&memSeg1, memspc51, sizeof(memspc51));
/// 
/// // give whole space to iTab; do not fix memspc.next
/// UME_connect (iTab, &memSeg1);
/// // set max size for iTab
/// sizTab = UME_ck_free (&memSeg1) / sizeof(int);
/// 
/// // reserve 100 int's in memSeg1; update memspc.next.
/// iTab = UME_reserve (&memSeg1, 100 * sizeof(int));
/// // give the remaining space to (char*)cbuf
/// UME_connect (cbuf, memSeg1);
/// // query size of cbuf
/// cbufSiz = UME_ck_free (memSeg1);
/// 
/// see also ../doc/gCAD3D_prog_de.txt section Memoryfunktionen
/// \endcode



  // printf("UME_init  siz=%d\n",osiz);

  memSpc->start = objDat;
  memSpc->next  = objDat;
  // memSpc->end   = objDat + osiz;
  memSpc->end   = (char*)objDat + osiz;

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
/// RetCode: the address (starting-Position) of the memSpace;
///   NULL if EOM (not enough memspace)
/// \endcode

  void *actPos;

  // printf("UME_reserve siz=%d next=%d tot=%d\n",osiz,
          // memSpc->next - memSpc->start,    // next-start
          // memSpc->end - memSpc->start);    // tot = end - start

  if(!memSpc) {
    printf("UME_reserve E000\n");
    return NULL;
  }

  actPos = memSpc->next;

  if(((char*)memSpc->next + osiz) > (char*)memSpc->end) {
    printf("UME_reserve E001 - OUT OF SPACE\n");
    return NULL;
  }

  memSpc->next = (char*)memSpc->next + osiz;

  return actPos;

}


//=======================================================================
  int UME_add (Memspc *memSpc, int osiz) {
//=======================================================================
/// \code
/// DO NOT USE (use UME_reserve)
///
/// UME_add               den next-Pointer korrigieren (Obj. manuell speichern)
/// 
/// RetCod 0=OK, -1=OutOfSpace
/// 
/// Platz reservieren; wird nachfolgend NICHT mit UME_save belegt.
/// 
/// Achtung Reihenfolge:
/// 
/// 1) den Pointer merken
///   Point  *pTab;
///   pTab = (Point*)memSeg1->next;
/// 
/// 2) den Platz reservieren (aendert den Pointer memSeg1->next)
///   i1 = UME_add (memSeg1, sizeof(Point) * ptNr);
///   if(i1 < 0) goto L_eom;  // Test ob genug Platz im memSeg
/// 
/// 3) den Platz belegen
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
 int UME_reall_add (long *spcOff, Memspc *memSpc, long addSiz) {
//================================================================
/// \code
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


  if((newNext >= memSpc->end)||(addSiz == 0)) {
    // printf("     actSiz=%d\n",(char*)memSpc->end-(char*)memSpc->start);

    // (char*)newEnd = (char*)memSpc->end + UME_INC;
    incSiz = UTI_round_b2i(memSpc->incSiz);
    printf("UME_reall_add %ld %ld\n",addSiz,incSiz);
    newEnd = (void*)((char*)memSpc->end + incSiz);

    // while(newEnd < newNext) (char*)newEnd += UME_INC;
    while(newEnd < newNext) newEnd = (void*)((char*)newEnd + incSiz);

    totSiz = (char*)newEnd - (char*)memSpc->start;
    // printf("     totSiz=%d %d\n",totSiz,UME_INC);

    irc = UME_realloc (spcOff, memSpc, totSiz);
    if(irc < 0) { TX_Error("UME_reall_add EOM"); return irc;}

    // next ist changed; add the requested space
    newNext = (char*)memSpc->next + addSiz;


  } else {
    *spcOff = 0;
  }

  memSpc->next = newNext;

  return 0;

}


//============================================================================
  int UME_reall_save (long *spcOff, Memspc *memSpc, void* objDat, int osiz) {
//============================================================================
/// \code
/// UME_reall_save              save Obj -> Memspc
/// next-Pointer is incremented <addSiz> bytes and
/// <osiz> bytes from position <objDat> are saved into memSpc.
/// Input:
///   oSiz     nr of bytes to save
///   objDat   date to be saved in memSpc
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
  memcpy(memSpc->next, objDat, osiz);

  // memSpc->next += osiz;
  memSpc->next = (char*)newNext;

  return 0;

}


//=======================================================================
  void* UME_save (Memspc *memSpc, void* objDat, int osiz) {
//=======================================================================
/// \code
/// UME_save              save Obj -> Memspc
/// RetCode: the address (starting-Position) of the memSpace;
///   NULL if EOM
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
/// UME_ck_free           return free space

// usage:
//  pTabSiz = UME_ck_free(memSeg1) / sizeof(Point);


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
/// UME_dump              display free space


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


//================================================================
  void UME_obj_dump (MemObj *mo) {
//================================================================
// UME_obj_dump          dump MemObj


  printf("UME_obj_dump  mbID=%d ioff=%d\n",
      mo->mbID, mo->ioff);

}


//======================== EOF ======================================*/
