// #include "../ut/ut_memTab.h"           // MemTab       PREREQ
// #include "../xa/xa_ed_mem.h"           // APED_..

typedef_MemTab(ObjSRC);

  int APED_find_dep__ (MemTab(ObjSRC) *oa, int typ, long dbi);
  char* APED_onam_search (char *ps, char *pe);
  char* APED_nxt_obj (char *cps);
  char* APED_lNr_objNam (long *lNr, long *lLen, char **oNam,
                         char *wTab[], long lSta);
  int APED_search_defLn (char **lSta, long *lNr, int *lLen,
                         char *otx, long lNEnd, int src);
  int APED_objID_defLn (char *oid, char *defLn);

// EOF
