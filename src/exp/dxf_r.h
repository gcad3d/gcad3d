
  int dxf_ckFileFormat (char *fnam);

  int dxfr_init       (double*, FILE*, FILE*);
  int dxfr_rec__      (ObjGX **, FILE*, FILE*, Memspc*);
  int dxfr_rec_read   (FILE *, FILE *);
  int dxfr_block_skip (FILE *, FILE *);
  int dxfr_block_find (FILE *, FILE *, char *);
  ObjGX* dxfr_stru_2_obj (int typ, int form, int siz, void* data);

  void dxfr_load_mat  (Mat_4x3, Vector*);
  void dxfr_tra_arc   (Circ*, Mat_4x3, Point*, double, double, double);

//=============== EOF ====================0
