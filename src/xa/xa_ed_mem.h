// ../xa/xa_ed_mem.h
// Prerequisites:
// #include "../ut/ut_memTab.h"           // MemTab
// #include "../xa/xa_mem.h"              // mem_cbuf1 mem_cbuf1_SIZ

// ../xa/xa_ed_mem.c
// #include "../xa/xa_ed_mem.h"           // APED_..


typedef_MemTab(ObjSRC);


#define  APED_SRCLN_BUFSIZ mem_cbuf1_SIZ   // 200000 ../xa/xa_mem.h
#define  APED_SRCLN_BUF__  mem_cbuf1



  int APED_find_dep__ (MemTab(ObjSRC) *oa, int typ, long dbi);
  char* APED_onam_search (char *ps, char *pe);
  char* APED_nxt_obj (char *cps);
  char* APED_lNr_objNam (long *lNr, long *lLen, char **oNam,
                         char *wTab[], long lSta);
  int APED_search_defLn (char **lSta, long *lNr, int *lLen,
                         char *otx, long lNEnd, int src);
  int APED_objID_defLn (char *oid, char *defLn);



// INCLUDE_FULL defined only in ../ut/ut_face.c
#define _OSRC_NUL {0, 0, 0, NULL, 0, 0, 0}
#ifndef INCLUDE_FULL
extern ObjSRC OSRC_NUL;
#else
ObjSRC OSRC_NUL = _OSRC_NUL;
#endif




// EOF
