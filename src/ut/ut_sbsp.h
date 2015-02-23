
  int UT3D_sbsp_ncv (SurBSpl *su1, Memspc *tbuf1,
               int cvNr, CurvBSpl **cvtab, int deg,
               Memspc *tbuf2, Memspc *tbuf3);

  int UT3D_sbsp_2ncv (SurBSpl *su1, Memspc *tbuf1,
                int cv1Nr, CurvBSpl **cv1tab, int deg1,
                int cv2Nr, CurvBSpl **cv2tab, int deg2,
                Memspc *tbuf2, Memspc *tbuf3);

  int UT3D_ptvc_evparsbsp (Point *pto, Vector *vn, int mode, int iDir,
                           SurBSpl *sbs, double pu, double pv, Memspc *workSeg);

  int UT3D_bspV_sbsp_u (CurvBSpl *bspl, SurBSpl *sbs, double pu,
                         Memspc *workSeg);

  int UT3D_bspU_sbsp_v (CurvBSpl *bspl, SurBSpl *sbs, double pv,
                         Memspc *workSeg);

  int UT3D_obj_cnvt_sbsp (ObjGX *ox1, SurBSpl *su1, Memspc *memSeg1);


//========== EOF =================================================
