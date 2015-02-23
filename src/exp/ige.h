
  int  IGE_r_init  ();
  int  IGE_r_rec   (ObjGX **, int *, FILE *, FILE *, FILE *);

  int  IGE_r_dump  ();
  int  IGE_r_readP (int , FILE *);
  int  IGE_r_getPP (double [], int , FILE *);
  int  IGE_r_decodePP (double [], int raSiz, char *);


  int  IGE_w_init  (FILE *fp_o1);
  int  IGE_w_rec   (ObjGX *el, long, int, long, FILE*, FILE*);
  int  IGE_w_rD    (ObjGX *el, FILE *fp_o1);
  int  IGE_w_rP    (ObjGX *el, FILE *fp_o2);
  int  IGE_w_rPP   (FILE *fp_o2, Point*);
  int  IGE_w_rP1   (FILE *fp_o2, int mode, int pi, double pd);
  int  IGE_w_rT    (FILE *fp_o1, FILE *fp_o2);
  int  IGE_w_D_Ln  (FILE *fp_o1);
  long IGE_w_Mat   (Mat_4x3, FILE *fp_o1, FILE *fp_o2);


//======================= EOF ====================
