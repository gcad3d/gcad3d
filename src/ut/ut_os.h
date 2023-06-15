/*
Korr:
2001-04-24  neu aus ut_os_aix.c
*/

#ifndef TimeStamp
#define TimeStamp  float
// typedef float TimeStamp;
#endif



#include "../xa/ap_dir.h"      // prototypes for gcad3d-directories (AP_get_tmp_dir ..)


  // int    OS_Init_  (char *cmd);
  char*  OS_os_c   ();
  void   OS_date   (long *i1, long *i2, long *i3);
  char*  OS_date1  ();
  void   OS_time   (long *i1, long *i2, long *i3);
  double OS_rTime ();
  double OS_prc_time_diff (double timStart);
  double OS_prc_time_start ();
  int    OS_system (char *buf);
  int    OS_browse_htm (char *filnam, char *label);
  void   OS_Wait   (int millisecs);
  void   OS_beep   ();
  char*  OS_prc__  ();

  int    OS_check_isDir   (char* dirnam);
  int    OS_checkFilExist (char* filnam, int mode);
  int    OS_checkDirExist (char* dirnam);
  long   OS_FilSiz    (char* filnam);

  int    OS_dir_root  (char *cbuf, int csiz);
  int    OS_dir_ck1   (char *p1);
  int    OS_dir_ck2   (char *p1);
  int    OS_dir_scan_ (char *cbuf, int *iNr);
  int    OS_dir_cre   (char *dn);

  void   OS_get_curDir    (char *sd, int sSiz);

  char*  OS_bin_dir_get   ();          // ../ut/os__.c
  char*  OS_get_tmp_dir   ();
  char*  OS_get_env       ();
  char*  OS_get_user      ();
  char*  OS_get_term      ();
  char*  OS_get_edi       ();
  int    OS_get_lang      (char *sLang);
  char*  OS_get_browse_htm   ();
  char*  OS_get_printer   ();
  char*  OS_get_imgConv1  ();
  char*  OS_fVwr_set      (char *pVwr, char *sVwr);
  char*  OS_fVwr_get      (char *pVwr);
  int    OS_get_GUI       ();
  char*  OS_get_dir_pwd   ();
  char*  OS_get_os__      ();
  char*  OS_get_os_bits      ();

  int    OS_osVar_eval__ (char *fno, char *fni, int fnoSiz);
  int    OS_file_copy (char *fnOld, char *fnNew);
  int    OS_file_rename (char *fnOld, char *fnNew);
  int    OS_file_delete (char *fNam);
  int    OS_file_readm1 (char *cbuf, int cSiz, char *fnam);
  int    OS_file_waitExist (char *fn, int maxTim);
  // int OS_file_concat (char *fno, char *fn1, char *fn2);
  int OS_files_join (char *fno, char *fn1, char *fn2, ...);

  int    OS_debug_dll_ (char *dllNam);
  int    OS_ck_libcVersion (int vMaj, int vMin);

  int    OS_file_sig_cre   (int mode, void *data);
  int    OS_file_sig_wait  (int mode, void *data);

  int    OS_stdout__ (int mode, void *data);



//----------------------------------------------------------------
// exported VARS:     (in ../ut/ut_os_aix.c and ../ut/ut_os_w32.c without extern)
#ifdef LOCAL_DEF_VAR
char OS_browser[80] = {"\0"};                         // HTML-browser
#else
extern char OS_browser[80];                           // HTML-browser
#endif


// OS_FIND_STR_DELI     find first occurence of  directory-delimiter '/' or '\'
// OS_FIND_STRR_DELI    find last occurence of  directory-delimiter '/' or '\'
#if defined _MSC_VER || __MINGW64__
#define OS_FIND_STR_DELI(pStr) strpbrk(pStr,"/\\")
#define OS_FIND_STRR_DELI(pStr) UTX_find_strrchrn(pStr,"/\\")
#else
#define OS_FIND_STR_DELI(pStr) strchr(pStr,fnam_del)
#define OS_FIND_STRR_DELI(pStr) strrchr(pStr,fnam_del)
#endif


/* ====================== EOF ============================ */
