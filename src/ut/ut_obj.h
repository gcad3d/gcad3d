
  int UTO_obj_dbo  (void **oDat, int *oNr, int *oTyp, long dbInd);
  int UTO_sav_ost (int *dbTyp, long *dbInd,
                   int typ, int form, int siz, void *obj);

int   UTO_objDat_ox     (void **objOut, int *oNr, ObjGX *ox1);
int   UTO_pt_eval_par1_dbo (Point *pto, double lpar, int typ, long ind);
int   UT3D_par_pt__pt_prj_cv (double *pao, Point *pto, int vTyp, int mode,
                              Point *pti, int oTyp, void *oDat, double tol);
int   UTO_par__pt_dbo (double *lpar, int vTyp, Point *pti, int typ, long ind);
int   UTO_npt_Tes      (int *pNr, Point **ppa, ObjGX *os);

  void* UTO_obj_save     (Memspc *memSpc, ObjGX *oxi);

  int   UTO_mem_seri     (void** end, void* start, ObjGX *ox1);
  void* UTO_save__       (Memspc *memSpc, void* objDat, int osiz);
  int   UTO_Memspc_init  (Memspc *memSpc, void* objDat, int osiz);

  int   DEB_dump_ox_0       (ObjGX *xTab, char *txt, ...);
  int   DEB_dump_ox_1       (ObjGX *xTab, char *txt, ...);

  int   UTO_invert       (ObjGX *oxi);



//========================== EOF ========================================
