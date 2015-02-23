

  int UMB_is_alive  (int id);
  int UMB_reserve__ (int *actId);
  int UMB_save      (int mbId, void *memPos, long memSiz, int recNr);
  int UMB_reload    (int *recNr, void *pStart, void **pNxt, void *pEnd, int ii);
  void* UMB_pos__   (int mbId);
  int UMB_stat__    (int mbId);
  int UMB_free      (int mbId);
  int UMB_dump__    ();

// EOF
