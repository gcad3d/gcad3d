
//   int  IGE_r_init  ();
//   int  IGE_r_rec   (ObjGX **, int *, FILE *, FILE *, FILE *);
// 
//   int  IGE_r_dump  ();
//   int  IGE_r_readP (int , FILE *);
//   int  IGE_r_getPP (double [], int , FILE *);
//   int  IGE_r_decodePP (double [], int raSiz, char *);


  
  int IGE_w_obj    (ObjGX *ox1, int aptTyp, long aptInd);
  int IGE_w_subObjs (ObjGX *el);
  int IGE_w_CVCCV (ObjGX *el);
  int IGE_w_SUR (ObjGX *el);
  int IGE_w_init  ();
  int IGE_w_rec   (ObjGX *el, int, int, long);
  int IGE_w_rD    (ObjGX *el);
  int IGE_w_rP    (ObjGX *el);
  int IGE_w_rP1   (int mode, int pi, double pd);
  int IGE_w_rPP   (Point*);
  int IGE_w_rPT   (char *txt);
  int IGE_w_rT    ();
  int IGE_w_D_Ln  ();
  int IGE_w_P_MD (ObjGX *el);
  int IGE_w_P_SRC (ObjGX *el);
  int IGE_w_P_142 (ObjGX *el);
  int IGE_w_P_144 (ObjGX *el);
  int IGE_w_P_SBS (ObjGX *el);
  int IGE_w_P_SRV (ObjGX *el);
  int IGE_w_P_SRU (ObjGX *el);
  int IGE_w_P_TX (ObjGX *el);
  int IGE_w_P_CCV (ObjGX *el);
  int IGE_w_P_BS (ObjGX *el);
  int IGE_w_P_PS (ObjGX *el);
  int IGE_w_P_POL2 (ObjGX *el);
  int IGE_w_P_POL (ObjGX *el);
  int IGE_w_P_ELL (ObjGX *ox);
  int IGE_w_P_PLN (ObjGX *ox);
  int IGE_w_P_PT (ObjGX *ox);
  int IGE_w_P_LN (ObjGX *ox);
  int IGE_w_P_CI (ObjGX *ox);
  
  int IGE_w_Mat   (Mat_4x3);
  int IGE_w_124 (Mat_4x3 m1);


//======================= EOF ====================
