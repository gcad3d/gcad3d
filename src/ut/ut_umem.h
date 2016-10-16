/*
 * fn.ft
 *
 * Copyright (C) 2012 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
*//*!
\file ../ut/ut_umem.h
\ingroup grp_ut_obj
*/


/// \brief Typ_Memspc (a class for variable-length-records in memory)
/// \code
/// start   startpos of memspc
/// next    first free pos of memspc
/// end     position of start + memspcSiz
/// typ     0. Reserved.
/// incSiz  if Memspc is too small: add UTI_round_b2i(incSiz) bytes
/// \endcode
typedef struct {void *start, *next, *end;
                unsigned char typ, incSiz, u3, u4;}                 Memspc;



  int   UME_init (Memspc *memSpc, void* objDat, int osiz);
  int   UME_alloc_tmp (Memspc *memSpc, long spcSiz);
  int   UME_malloc (Memspc *memSpc, long spcSiz, long memInc);
  void* UME_save (Memspc *memSpc, void* objDat, int osiz);
  void* UME_reserve (Memspc *memSpc, int osiz);
  int   UME_add (Memspc *memSpc, int osiz);
  int   UME_adjust (Memspc *memSpc, void* newStart);
  int   UME_ck_free (Memspc *memSpc);
  int   UME_set_free (int sizFree, Memspc *memSpc);
  int   UME_dump    (Memspc *memSpc, char *txt);
  int   UME_del (Memspc *memSpc, void *nach, void *von, ...);
  void  UME_connect (void *memPos, Memspc *memSeg);

  int   UME_reall_add (long *spcOff, Memspc *memSpc, long addSiz);
  int   UME_realloc (long *spcOff, Memspc *memSpc, long newSiz);
  int   UME_reall_save (long *spcOff, Memspc *memSpc, void* objDat, int osiz);

  void* UME_get_next (Memspc *memSpc);
  void* UME_get_start (Memspc *memSpc);
  long  UME_get_used (Memspc *memSpc);


/// init Memspc
#define UME_NEW {NULL, NULL, NULL, 0}


/// \code
/// UME_alloc_tmp                allocate temp. space for Memspc
/// DO NOT GET > 100000 byte
/// memspace exists until active function returns (using alloca)
/// MUST BE INLINE; after return of function memspace is lost !
/// Example:
/// Memspc   tmpSpc=UME_NEW;
/// UME_alloc_tmp (&tmpSpc, 1000);   // get a Memspc size = 1000 bytes
/// \endcode
int   UME_alloc_tmp (Memspc*, long);
#define UME_alloc_tmp(memSeg,memsiz)\
  UME_init (memSeg, MEM_alloc_tmp(memsiz), memsiz)


/// UME_get_next          get actual memSpacePosition (.next; next free position)
#define UME_get_next(memSeg) ((memSeg)->next)


/// UME_set_next          set actual memSpacePosition (.next; next free position)
/// reset after UME_get_next (regain memspace)
#define UME_set_next(memPos,memSeg) (memSeg)->next=memPos


/// UME_get_start         get startPosition of Memspc
/// returns start position in Memspc (spc.start; first obj)
#define UME_get_start(memSeg) (memSeg)->start


/// UME_get_used          get used space
#define UME_get_used(memSeg) ((char*)(memSeg)->next - (char*)(memSeg)->start)


/// UME_connect    DO NOT USE   get actual memSpacePosition
///   use UME_get_next
/// Output:
///   memPos        next free position in memSeg (as void*)
#define UME_connect(memPos,memSeg)(memPos)=(memSeg)->next


// /// UME_TMP_FILE          allocate temp.memspace for file
// int UME_TMP_FILE (void**, long*,  char*);
// #define UME_TMP_FILE(fBuf, fSiz, fNam)\
//   *fSiz = OS_FilSiz (fNam);\
//    *fBuf = (void*)MEM_alloc_tmp (*fSiz)



//----------------------------------------------------------------
/// \code
/// MemObj a pointer to a object in a UMB-memoryblock
///   - same size as void*; see memobj_union
/// mbID    memoryblocknumber
/// ioff    offset in the memoryblock in bytes
/// \endcode
#ifdef _LP64
typedef struct { int mbID; int ioff; }                              MemObj;
// #define MEMOBJ_NR_MAX 1000
#else
typedef struct { unsigned mbID:8, ioff:24;}                         MemObj;
// #define MEMOBJ_NR_MAX 128
#endif

// get mem_obj as void*  (have same size)
typedef union {void* vp; MemObj mo;} memobj_union;
// example:
//   ((memobj_union)((Obj_Unknown*)gWin)->mem_obj).vp);


// pointer from MemObj
#define PTR_MEMOBJ(MO) ((memobj_union*)&MO)->vp
// MemObj from pointer
// #define MEMOBJ_PTR(p) ((memobj_union)p).mo
#define MEMOBJ_PTR(p) ((memobj_union*)p)->mo


/// test if object is valid; >=0 is OK; else invalid object.
/// MemObj *mo;  if(UME_obj_IS_VALID(mo) < 0) ERROR
#define UME_obj_IS_VALID(mo1) (int)((mo1)->ioff - 4)


  MemObj UME_obj_reserve (void **oPos, Memspc *mSpc, int siz, int mbId);
  int    UME_obj_save (int mbId, Memspc *mSpc, int recNr);
  void*  UME_obj_get (MemObj *mo);
  MemObj UME_obj_invalid_set (int iErr);



//======================== EOF ======================================
