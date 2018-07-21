// ../ut/ut_mem.h
// ../ut/ut_mem.c


// max size for allocation on stack:
#define SPC_MAX_STK 32767


 // ut_mem.o:
 int MEM_swap__      (void *stru1, void* stru2, long strSiz);


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


//----------------------------------------------------------------
/// compare memoryspaces (n bytes); returns 0 for equal.
#define MEM_cmp__ memcmp

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

// swap structs: MEM_swap__ ();

/// \code
/// MEM_ptr_mov         move a pointer <dist> bytes
/// Example:
/// ptr = MEM_ptr_mov (ptr, 16);      // move ptr 16 bytes in memory
/// \endcode
#define MEM_ptr_mov(ptr,dist) ((char*)(ptr) + (dist))


/// \code
/// MEM_alloc_tmp             allocate mem for active function
/// >>>>  USE ONLY IF SIZE < SPC_MAX_STK byte  <<<<
/// calls alloca; memspace exists until active function returns.
/// ptab = (Point*) MEM_alloc_tmp (nr_of_points * sizeof(Point));
/// ATTENTION: MinGW: use also alloca, not _alloca !!!
/// ATTENTION: MS needs more space!
/// \endcode
void* MEM_alloc_tmp (int);
/*
#ifdef _MSC_VER
#define MEM_alloc_tmp(siz) _alloca(siz + 64)
//#define MEM_alloc_tmp(siz) alloca(siz)
#else
#define MEM_alloc_tmp(siz) alloca(siz)
#endif
*/
#ifdef _MSC_VER
#define MEM_alloc_tmp(siz) _alloca(siz + 64)
//#define MEM_alloc_tmp(siz) alloca(siz)
#else
#define MEM_alloc_tmp(siz)\
 alloca(siz);\
 if(siz >= SPC_MAX_STK) printf("*** ERR MEM_alloc_tmp |%s| %d\n",__func__,siz);\
 fflush(stdout)
#endif


/// UME_TMP_FILE          allocate temp.memspace for file
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



//=============== EOF ============================================
