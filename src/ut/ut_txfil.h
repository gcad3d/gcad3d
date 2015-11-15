// ut_txfil.h           RF

  int   UTF_clear_     ();
  long  UTF_offset_    (char* cpos);
  void  UTF_load_      (char* txbuf, long *txlen);
  int   UTF_add_file   (char* fn);
  int   UTF_add_line   (char* txt);
  int   UTF_add_nl     ();
  int   UTF_txt_chg    (char *cpos, char *txt, long txlen);
  // int   UTF_wri_file   (char* fn, int mode);
  int   UTF_wri_file   (char* fn, int iniFunc(FILE*));

  long  UTF_GetLen0    ();
  char* UTF_GetLinNr   (char* lbuf, long *llen, long lNr);
  char* UTF_GetPosLnr  (long *llen, long lNr);
  int   UTF_GetLnrPos  (long ipos);
  char* UTF_GetnextLnPos (char *cpos);
  char* UTF_GetPrevLnPos (long *lNr, char *cpos);

  char* UTF_find_tx1   (long *lNr, char *tx1);

  int   UTF_clear1     ();
  char* UTF_add1_file  (char* fn);
  int   UTF_add1_line  (char* txt);
  char* UTF_get1_LnPos (long lNr);
  long  UTF_get1_LnLen (char **lEnd, char *lStart);
  char* UTF_get1_nxtLnPos (char *actPos);
  int   UTF_insert1    (long cpos);

  int   UTF_del_line1  (long lNr);
  // int   UTF_del_rest   (char *text);

  long  UTF_GetLinLen  (char **lEnd, char *lStart);


/*==================== EOF =================*/
