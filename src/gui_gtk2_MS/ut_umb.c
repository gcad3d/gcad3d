/* ../ut/ut_umb.c
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
Store memoryblocks (malloc, free).                                  UMB_

TODO:


-----------------------------------------------------
Modifications:
2012-04-13 First version; extracted from Gtk-toolbox. RF.

-----------------------------------------------------
*/
#ifdef globTag
void UMB(){}
#endif
/*!
\file ../ut/ut_umb.c
\ingroup grp_ut_obj
\code

=====================================================
Liste_Funktionen_Beginn:

UMB_reserve__   reserve a memoryblockId;
UMB_pos__       get startpos of saved memblock
UMB_stat__      return umbTab[mbId].umbStat
UMB_save        copy a local memoryblock (malloc, memcpy)
UMB_free        free memoryblock from memoryblockId
UMB_is_alive    check if blockId is active / valid
UMB_reload      reload memBlock ii; update pNxt

UME_reuse       increment blockId
UMB_dump__

Liste_Funktionen_Ende:
=====================================================

- see also:
../doc/gCAD3D_prog_de.txt section Memoryfunktionen

\endcode *//*----------------------------------------


Save memoryblocks (by malloc new space, copy to new space) and get a
 memoryblockId.
UMB keeps a table of all memoryblocks in use.


umbNr     nr of memoryblocks in use; size of umbTab.

umbTab    has umbNr records, typ = umb_stru (Pos,Siz,Stat,Id).






=====================================================
compile mit gcc:
cc -c -O0 -g3 -Wall ut_umb.c

Windows:
cl -c ut_umb.c




test offline:
cc ut_umb.c ut_umem.c ut_mainut.c -DOFFLINE_UMB -lm&&./a.out
ctags -f ut_umb.tag ut_umb.c

*/


// #include "../xa/MS_Def0.h"

#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>                     // memcpy

#include "../ut/ut_umem.h"              // Memspc
#include "../ut/ut_umb.h"               // UMB_
#include "../ut/ut_TX.h"                // TX_Error




/// umbPos     startposition of memoryblock
/// umbSiz     size of block
/// umbStat    0  memoryblock uninitialized (deleted), free for next use
///            1  memoryblock reserved - local use
///            2  memoryblock stored (with malloc)
/// umbId      is incremented with each new use
/// recNr      nr of records (UI_srcId);
typedef struct {void *umbPos; long umbSiz; int umbStat, umbId, recNr;} umb_stru;

// #define UMB_NEW {NULL, 0L, 0}


static int      umbNr = 0;            // nr of memoryblocks in use
static int      umbSiz = 0;           // size of umbTab
static int      umbIncSiz = 100;
static umb_stru *umbTab = NULL;
static int      umbId = -1;            // is incremented


#ifdef _LP64
#define UMB_MAX_ID 4294967295
#else
#define UMB_MAX_ID 255
#endif






//========================================================
//========================================================
#ifdef OFFLINE_UMB


typedef struct {double d1, d2; } myObj1;



//============================
  int main (int paranz, char *params[]) {
//============================

  int     mbId1, mbId2, ii1, ii2;
  myObj1  *o1, *o2;
  char    s1[256], *p1;
  Memspc  mSpc;
  MemObj  mo1, mo2;

  printf("OFFLINE ut_umb \n");


  // get memspc
  UME_init (&mSpc, s1, sizeof(s1));


  // get memBlockId
  mbId1 = UMB_reserve__ (&ii1);
  UMB_dump__ ();


  // fill obj, move free-space-pointer.
  mo1 = UME_obj_reserve ((void**)&o1, &mSpc, sizeof(myObj1), mbId1);
  // o1 has now space for myObj1;
  // mo1 has infos for the position of o1.
  o1->d1 = 12.34;
  

  // get another memBlockId
  mbId2 = UMB_reserve__ (&ii2);
  UMB_dump__ ();


  // save tempSpace -> memBlock
  UME_obj_save (mbId1, &mSpc, 1);
  UMB_dump__ ();


  // get obj (now from memBlock)
  o2 = UME_obj_get (&mo1);
  printf(" o2.d1=%lf\n",o2->d1);


  // clear the memBlock
  UMB_free (mbId1);



  return 0;

}

#endif


//================================================================
  int UMB_is_alive (int id) {
//================================================================
/// \code
/// check if blockId <id> is active / valid
/// 0=no, 1=yes
/// \endcode

  int        i1;

  for(i1=0; i1<umbNr; ++i1) {
    if(umbTab[i1].umbStat < 1) continue;          // skip deleted block
    if(umbTab[i1].umbId != id) continue;
    return (1);
  }

  return (0);

}


//================================================================
  int UMB_reserve__ (int *actId) {
//================================================================
// return next free memblockID; -1=Error (out of memory);
// returns index into umbTab

  int winID, i1;


  // printf("UMB_reserve__ %d\n",umbNr);

  // find next free (deleted) block
  for(i1=0; i1<umbNr; ++i1) {
    if(umbTab[i1].umbStat) continue;
      // printf(" free umb = %d %d\n",i1,umbTab[i1].umbStat);
    winID = i1;
    goto L_reserve;
  }

  // open a new block
    // printf(" open new block ..\n");
  winID = umbNr;
  ++umbNr;



  if(umbNr >= UMB_MAX_ID) {
    TX_Error("UMB_reserve__ E001 (too much mem-pages)\n");
    return -1;
  }
  

  if(umbNr >= umbSiz) {
    if(umbNr >= UMB_MAX_ID) umbNr = UMB_MAX_ID;
    umbSiz += umbIncSiz;
    umbTab = realloc (umbTab, sizeof(umb_stru) * umbSiz);
    if(!umbTab) { winID = -1; goto L_exit; }
  }


  L_reserve:
  ++umbId;
  umbTab[winID].umbId   = umbId;
  umbTab[winID].umbStat = 1;           // reserved for local use
  umbTab[winID].umbPos  = NULL;


  L_exit:
    // printf("ex UMB_reserve__ %d\n",winID);

  *actId = umbId;

  return winID;

}


//================================================================
  int UME_reuse (int actId) {
//================================================================
/// increment blockId

  ++umbId;
  umbTab[actId].umbId   = umbId;
  umbTab[actId].umbStat = 1;

  return umbId;

}
 

//===========================================================================
  int UMB_reload (int *recNr, void *pStart, void **pNxt, void *pEnd, int ii) {
//===========================================================================
/// reload memBlock ii; update pNxt
 
  void   *vpn;

  // test size
  // vpn = pStart + umbTab[ii].umbSiz;
  vpn = (char*)pStart + umbTab[ii].umbSiz;

  if(pEnd < vpn) {
    TX_Error("UMB_reload E001 %d\n",ii);
    return -1;
  }

  // copy back -> UI_tmpSpc
  // *start, *next, *end; typ, incSiz;
  memcpy (pStart,                // to,
          umbTab[ii].umbPos,     // from,
          umbTab[ii].umbSiz);    // size
  *pNxt = vpn;


  umbId  = umbTab[ii].umbId;     // memBlockName

  *recNr = umbTab[ii].recNr;     // nr of records in memBlock

  return 0;

}


//================================================================
  int UMB_save (int mbId, void *memPos, long memSiz, int recNr) {
//================================================================
/// \code
/// save a memblock (malloc & memcpy);
/// set its umbStat to 2.
/// \endcode

  // void   *p1;


  // printf("UMB_save Id=%d Siz=%ld\n",mbId,memSiz);


  umbTab[mbId].umbPos = malloc (memSiz);
  if(!umbTab[mbId].umbPos) {
    TX_Error("UMB_save E001 %d\n",mbId);
    return -1;
  }

  memcpy (umbTab[mbId].umbPos, memPos, memSiz);

  umbTab[mbId].umbSiz  = memSiz;
  umbTab[mbId].umbStat = 2;
  umbTab[mbId].recNr   = recNr;

  return 0;

}


//================================================================
  int UMB_stat__ (int mbId) {
//================================================================
// return umbTab[mbId].umbStat
// -1=block does not exist, 0=free, 1=reserved, 2=stored

  if(mbId >= umbNr) return -1;

  return umbTab[mbId].umbStat;

}


//================================================================
  void* UMB_pos__ (int mbId) {
//================================================================
/// \code
/// get startpos of memblock <mbId>
/// NULL if memblock not reserved or not yet saved ..
/// \endcode
 
  if((mbId < 0)        ||
     (mbId >= umbNr))      {
     printf ("***** UMB_pos__ E001\n");
     return NULL;
  }

  return umbTab[mbId].umbPos;

}


//================================================================
  int UMB_free (int mbId) {
//================================================================
/// delete a memoryblock


  if((mbId < 0)||(mbId >= umbNr)) {
    TX_Error("UMB_free E001 %d\n",mbId);
    return -1;
  }

  if(umbTab[mbId].umbStat == 2) {
    // free memblock <mbId>
      printf("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF \n");
      printf(" UMB_free %d\n",mbId);
    free (umbTab[mbId].umbPos);
  }

  umbTab[mbId].umbStat = 0;

  return 0;

}
 

//================================================================
  int UMB_dump__ () {
//================================================================
/// dump table of memoryblocks
 
  int   i1, ioff;
  void  *pos;
  char  *cp1;


  printf("UMB_dump__ Nr=%d Siz=%d Inc=%d\n",umbNr,umbSiz,umbIncSiz);

  for(i1=0; i1<umbNr; ++i1) {
    printf(" [%2d] pos=%12p siz=%10ld Stat=%2d Id=%6d\n", i1,
            umbTab[i1].umbPos,
            umbTab[i1].umbSiz,
            umbTab[i1].umbStat,
            umbTab[i1].umbId);
  }


  for(i1=0; i1<umbNr; ++i1) {
    if(umbTab[i1].umbStat != 2) continue;
    ioff = 0;
    pos = umbTab[i1].umbPos;
      printf(" umbId =%d\n",*((int*)pos));
    // pos = ((char*)pos + sizeof(int));
    cp1 = (char*)pos;
    cp1 += 4;
    pos = cp1;
    printf(" umbTab[%d] \n",i1);
    GUI_obj_dump_o (pos);


  }

  printf("exit UMB_dump__\n");


  return 0;

}

// EOF
