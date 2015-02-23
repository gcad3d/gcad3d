/*
Korr:
2001-04-24  neu aus ut_os_aix.c
*/

#ifndef TimeStamp
#define TimeStamp  float
// typedef float TimeStamp;
#endif


  int    OS_Init_  (char *cmd);
  char*  OS_os_c   ();
  void   OS_date   (long *i1, long *i2, long *i3);
  char*  OS_date1  ();
  void   OS_time   (long *i1, long *i2, long *i3);
  double OS_rTime ();
  double OS_prc_time_diff (double timStart);
  double OS_prc_time_start ();
  int    OS_system (char *buf);
  int    OS_browse_ (char *filnam);
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

  void   OS_get_curDir    (char *sd, int sSiz);

// ex ../xa/xa_main.c
  char*  OS_get_bas_dir   ();
  char*  OS_get_tmp_dir   ();
  char*  OS_get_loc_dir   ();
  char*  OS_get_cfg_dir   ();
  char*  OS_get_ico_dir   ();
  char*  OS_get_bin_dir   ();
  char*  OS_get_doc_dir   ();

  char*  OS_get_edi       ();
  char*  OS_get_lang      ();
  char*  OS_get_browser   ();
  char*  OS_get_printer   ();
  char*  OS_get_imgConv1  ();
  char*  OS_get_vwr_ps    ();
  char*  OS_os_s          ();

  char*  OS_get_dir_pwd   ();

  int    OS_file_copy (char *fnOld, char *fnNew);
  int    OS_file_rename (char *fnOld, char *fnNew);
  int    OS_file_delete (char *fNam);
  int    OS_file_readm1 (char *cbuf, int cSiz, char *fnam);
  int    OS_file_waitExist (char *fn, int maxTim);

  int    OS_dll__ (int mode, void *fDat);
  int    OS_dll_global (char *dllNam);
  int    OS_debug_dll_ (char *dllNam);
  int    OS_ck_libcVersion (int vMaj, int vMin);

  int    OS_file_sig_cre   (int mode, void *data);
  int    OS_file_sig_wait  (int mode, void *data);

  int    OS_stdout__ (int mode, char *fn);

/* ====================== EOF ============================ */
