
  int UTO_objDat_dbo  (void **oDat, int *oNr, int *oTyp, long dbInd);
  int UTO_sav_ost (int *dbTyp, long *dbInd,
                   int typ, int form, int siz, void *obj);

  // int   UTO_get_PT      (ObjX *oxo, ObjGX *oxi);
  // int   UTO_get_LN      (ObjX *oxo, ObjGX *oxi);
  // int   UTO_get_CI      (ObjX *oxo, ObjGX *oxi);
  // int   UTO_get_ELL     (ObjX *oxo, ObjGX *oxi);
  // int   UTO_objx_get    (ObjX *oxo, ObjGX *oxi);

int   UTO_objDat_ox     (void **objOut, int *oNr, ObjGX *ox1);
// int   UTO_2pt_lim_ox   (Point *ps,Point *pe, double *v1,double *v2,ObjGX *oxi);
int   UTO_pt_eval_par1_dbo (Point *pto, double lpar, int typ, long ind);
int   UTO_par__pt_obj (double *po, int mode, Point *pti, int oTyp, void *oDat);
int   UTO_par__pt_dbo (double *lpar, int mode, Point *pti, int typ, long ind);
int   UTO_npt_Tes      (int *pNr, Point **ppa, ObjGX *os);

  // int   UTO_set_ptlim    (ObjGX *oxo, Point *pti, int mode);

//  int UTO_obj_trim      (ObjGX *oo, ObjGX *oi, ObjGX *oc1, int i1Nr,
//                         ObjGX *oc2, int i2Nr, Memspc *wrkSpc);
  void* UTO_obj_save     (Memspc *memSpc, ObjGX *oxi);

  // ObjGX UTO_obj_load    (int typ, int form, int size, void* data);
  // ObjGX UTO_stru_2_obj  (int typ, int form, void* data);

  int   UTO_mem_seri     (void** end, void* start, ObjGX *ox1);
  void* UTO_save__       (Memspc *memSpc, void* objDat, int osiz);
  int   UTO_Memspc_init  (Memspc *memSpc, void* objDat, int osiz);

  int   DEB_dump_ox_0       (ObjGX *xTab, char *txt, ...);
  int   DEB_dump_ox_1       (ObjGX *xTab, char *txt, ...);

  int   UTO_invert       (ObjGX *oxi);



//========================== EOF ========================================
