
  int  dxfw_init      (FILE *, int dxfVersion);
  void dxfw_end       (FILE *);
  int  dxfw_rec       (ObjGX *, FILE *);
  int  dxfw_point     (int, Point *, FILE *);
  int  dxfw_vector    (Vector*, FILE*);
  void dxfw_load_mat  (Mat_4x3, Vector*);


  //int dxfw_head (ObjG *, FILE *);


//=============== EOF ====================0
