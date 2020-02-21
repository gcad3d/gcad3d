
//  void* GUI_file_get ();

// ../gui/GUI_file_open.c
//   int GUI_file_open__ (char *filNam, int fSiz, char *dirNam, int dSiz,
//                      char *filterO, void *dirLst,
//                      char *title, char *filterI);
//   int GUI_file_save__ (char *filNam, int fSiz, char *dirNam, int dSiz,
//                      void *getDirLst,
//                      char *title, char *filter);


  int GUI_file_open__ (char *filNam, int fSiz,
                       char *dirLst, char *sFilter, char *sTit);

  int GUI_file_save__ (char *filNam, int fSiz,
                       char *dirLst, char *sFilter, char *sTit);

// EOF

