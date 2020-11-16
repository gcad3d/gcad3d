// ../ut/ut_mem.h
// ../ut/ut_mem.c


// max size for allocation on stack:
#define SPC_MAX_STK 32767


 // ut_mem.o:
 int MEM_ins_rec     (void *insPos, long movSiz, void *insDat, long insSiz);
 int MEM_chg_rec     (void *datStart, long *datSiz,
                      void *insDat,   long insSiz,
                      void *delPos,   long delSiz);
 int MEM_ins_nrec    (int *recNr, void *recTab,
                     int ipos, void *insDat, int insRecNr, int sizRec);
 int MEM_del_nrec    (int *recNr, void *recTab,
                     int ipos, int delRecNr, int sizRec);
 int MEM_del_IndRec  (int *recNr, void *recTab, int ipos);
 int MEM_del_DbRec   (int *recNr, void *recTab, int ipos);
 int MEM_del_ShRec   (int *recNr, void *recTab, int ipos);
 void*  MEM_ptr_mov  (void *ptr, long dist);
 int MEM_cmp__       (void *obj1, void *obj2, int size);
 int MEM_swap_nrec   (void *na, int rNr, int rSiz);


//----------------------------------------------------------------
/// compare memoryspaces (n bytes); returns 0 for equal.
#define MEM_cmp__ memcmp

/// swap 2 structs
void MEM_swap__ (void *stru1, void* stru2, long strSiz);
#define MEM_swap__(sp1,sp2,sSiz){\
 char *sx = MEM_alloc_tmp(sSiz);\
 memcpy(sx,sp1,sSiz); memcpy(sp1,sp2,sSiz); memcpy(sp2,sx,sSiz);}
// Example: MEM_swap__ (&pt1,&pt2,sizeof(Point));

/// swap 2 bytes
void MEM_swap_byte  (char *c1, char *c2);
#define MEM_swap_byte(c1,c2){\
  char _c3 = *(c1); *(c1) = *(c2); *(c2) = _c3;}

/// swap 2 shorts
void MEM_swap_short  (short *i1, short *i2);
#define MEM_swap_short(i1,i2){\
  short _i3 = *(i1); *(i1) = *(i2); *(i2) = _i3;}

/// swap 2 ints
void MEM_swap_int (int *i1, int *i2);
#define MEM_swap_int(i1,i2){\
  int _i3 = *(i1); *(i1) = *(i2); *(i2) = _i3;}

/// swap 2 longs
void MEM_swap_2lg (long *i1, long *i2);
#define MEM_swap_2lg(i1,i2){\
  long _i3 = *(i1); *(i1) = *(i2); *(i2) = _i3;}

/// swap 2 doubles
void MEM_swap_2db (double*, double*);
#define MEM_swap_2db(d1,d2){\
  double _d3 = *(d1); *(d1) = *(d2); *(d2) = _d3;}

/// swap 2 pointers (void*)
void MEM_swap_2vp    (void **v1, void **v2);
#define MEM_swap_2vp(v1,v2){\
  void *_v3 = *(v1); *(v1) = *(v2); *(v2) = _v3;}
// void MEM_swap_2vp (void **p1, void **p2)

/// \code
/// MEM_ptr_mov         move a pointer <dist> bytes
/// Example:
/// ptr = MEM_ptr_mov (ptr, 16);      // move ptr 16 bytes in memory
/// \endcode
#define MEM_ptr_mov(ptr,dist) ((char*)(ptr) + (dist))


/// \code
/// MEM_alloc_tmp             allocate mem for active function
/// >>>>  USE ONLY IF SIZE < SPC_MAX_STK byte  <<<<
/// calls alloca; memspace exists until active function returns. DO NOT free.
/// Example:
///   ptab = (Point*) MEM_alloc_tmp (nr_of_points * sizeof(Point));
/// ATTENTION: MinGW: use also alloca, not _alloca !!!
/// ATTENTION: MS needs more space!
/// \endcode
void* MEM_alloc_tmp (int);
#ifdef _MSC_VER
#define MEM_alloc_tmp(siz) _alloca(siz + 64)
#else
#define MEM_alloc_tmp(siz) alloca(siz)
#endif

// #define MEM_alloc_tmp(siz)\
//  alloca(siz);\
//  if(siz >= SPC_MAX_STK) printf("*** ERR MEM_alloc_tmp |%s| %d\n",__func__,(int)siz);\
//  fflush(stdout)
// #endif


/// MEM_alloc_file          allocate temp.memspace for existing file
int MEM_alloc_file (void**, long*,  char*);
#define MEM_alloc_file(fBuf, fSiz, fNam)\
 *fSiz = OS_FilSiz (fNam);\
 *fBuf = (void*)MEM_alloc_tmp ((int)*fSiz)

// #ifdef _MSC_VER
// #define MEM_alloc_file(fBuf, fSiz, fNam)\
//   *fSiz = OS_FilSiz (fNam);\
//  _alloca(*fSiz + 64)
// #else
// #define MEM_alloc_file(fBuf, fSiz, fNam)\
//   *fSiz = OS_FilSiz (fNam);\
//  alloca(*fSiz);
// #endif


//----------------------------------------------------------------
// see MEMTYP_* ../ut/ut_types.h     INF_MEM_TYP

// returns 0 - memspc MUST be freed   / MEMTYP_ALLOC__
//      else - memspc CANNOT be freed / all but MEMTYP_ALLOC__
#define MEM_CANNOT_FREE(mTyp)\
 (mTyp != MEMTYP_ALLOC__)
//((!mTyp)||mTyp & 4)


// get 0 - CANNOT be freed  / all but MEMTYP_ALLOC__
//  else - MUST be freed    / MEMTYP_ALLOC__
#define MEM_MUST_FREE(mTyp)\
 (mTyp == MEMTYP_ALLOC__)
//((mTyp)&&(mTyp & 4) - 4)


// get 0 - CANNOT (re)alloc / MEMTYP_ALLOC_EXPND,MEMTYP_STACK__,MEMTYP_STACK_EXPND
//  else - ok, can(re)alloc / MEMTYP_NONE,MEMTYP_ALLOC__,MEMTYP_ALLOC_PROT
#define MEM_CAN_ALLOC(mTyp)\
 ((!mTyp)||(mTyp & 1))


//    0 can (re)alloc    / MEMTYP_NONE,MEMTYP_ALLOC__,MEMTYP_ALLOC_PROT
// else CANNOT (re)alloc / MEMTYP_ALLOC_EXPND,MEMTYP_STACK__,MEMTYP_STACK_EXPND
#define MEM_CANNOT_ALLOC(mTyp)\
  ((mTyp)&&(mTyp & 1) - 1)


//    0 CANNOT expand / MEMTYP_NONE,MEMTYP_ALLOC__,MEMTYP_ALLOC_PROT,MEMTYP_STACK__
// else can expand    / MEMTYP_ALLOC_EXPND,MEMTYP_STACK_EXPND
//   expand = malloc & copy
#define MEM_CAN_EXPND(mTyp)\
 (mTyp & 2)








//=============== EOF ============================================
