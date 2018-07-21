/*
2001-06-06 die GR_ funktionen zu.


==============================================================*/



  void GR_Init1 ();
  void GR_CreTxtA   (long *ind, int attInd, Point *pt1, char *txt);
  void GR_CrePoint  (long *ind, int attInd, Point *pt1);
  void GR_CreLine   (long *ind, int attInd, Line *ln1);
  void GR_CreCirc   (long *ind, int attInd, Circ *ci1);

  void GR_CreDisk   (long *ind, int attInd, Point *pc, Vector *vz,
                                          double rd1, double rd2);
  void GR_CreTriaFan(long *ind, int attInd, Point *pc, int anz, Point *pa1);
  void GR_CrePrism  (long *ind, int attInd, int anz, Point *pa1, Point *pa2);
  void GR_CreCyl    (long *ind, int attInd,Point *pt1, Point *pt2,
                                       double rd1, double rd2);
  void GR_CreTor    (long *ind, int attInd, Circ *ci1, double rd1);
  void GR_CreTorSeg (long *ind, int attInd, Circ *ci1, Circ *ci2);
  int  GR_CreSol    (long *ind, int attInd, ObjGX *db1);


  void GR_DrawDimen (long *ind, int attInd, Dimen *dim1);
  void GR_DrawTxtA  (long *ind, int attInd, Point *pt1, char *txt);
  void GR_DrawTxtG  (long *dli, int attInd, GText *tx1, long dbi);
  // void GR_DrawTxtG  (long *ind, int attInd,
                    // Point *pt1, float size, float ang, char *txt);
  void GR_DrawPoint (long*, int, Point*);
  void GR_DrawLine  (long *ind, int attInd, Line *ln1);
  int  GR_DrawCirc  (long *dli, long dbi, int attInd, Circ *ci1);
  void GR_DrawPoly  (long *ind, int attInd, int, Point*);
  void GR_DrawDitto2 (long *ind, Point *pt, double az, char mir, Ditto *di);
  int  GR_DrawPlane (long *ind, int attInd, Plane *pl1);
  int  GR_DrawSur   (ObjGX *oxi, int att, long apt_ind);

  void GR_DrawDisk  (long *ind, int attInd, Point *pc, Vector *vz,
                                          double rd1, double rd2);
  void GR_DrawTriaFan(long *ind, int attInd, Point *pc, int anz, Point *pa1);
  int  GR_DrawTriaStrip(long *ind,int attInd,
                        int ptUNr,int ptVNr,Point *pa,int newS);

  void GR_DrawPrism (long *ind, int attInd, int anz, Point *pa1, Point *pa2);
  void GR_DrawCyl   (long *ind, int attInd, Point *pt1, Point *pt2,
                                          double rd1, double rd2);
  void GR_DrawTor   (long *ind, int attInd, Circ *ci1, double rd1);
  void GR_DrawTorSeg(long *ind, int attInd, Circ *ci1, Circ *ci2);

  void GR_DrawCvPpsp3 (long *ind, int attInd, ObjGX *cv1, double zval);
  int  GR_DrawCvBSp   (long *ind, long dbi, int attInd, CurvBSpl *cv1);

  int GR_DrawFan (Point *pt1, int ptAnz, Point *pa1, int typ, int newS);
  int GR_DrawStrip (Point *pa1, Point *pa2, int ptAnz, int newS);
  int GR_Draw_spu (long *ind, int att, int pNr, Point *pTab);

  int GR_Disp_pt (Point *pt1, int typ, int att);
  int GR_Disp_pt2 (Point2 *pt1, int typ, int att);
  int GR_Disp_npti (int ptNr, Point *pTab, int typ, int att, int itx);
  int GR_Disp_pTab (int ptNr, Point *pTab, int typ, int att);
  int GR_Disp_p2Tab (int ptNr, Point2 *p2Tab, int typ, int att);
  int GR_Disp_vc (Vector *vc1, Point *pt1, int att, int mode);
  int GR_Disp_vc2 (Vector2 *vc1, Point2 *pt1, int att, int mode);
  int GR_Disp_ln (Line *ln1, int att);
  int GR_Disp_ac (Circ *ci1, int att);
  int GR_Disp_ell (CurvElli *el1, int att);
  int GR_Disp_pol (CurvPoly *pol, int att);
  int GR_Disp_cv (Point *pta, int ptnr, int att);
  int GR_Disp_cv2 (Point2 *pta, int ptnr, int att);
  int GR_Disp_bez (CurvBez *bez, int att, Memspc *workSeg);
  int GR_Disp_CvBSp (CurvBSpl *bspl, int att);
  int GR_Disp_tria (Triangle *tria, int att);
  int GR_Disp_box (Point *p1, Point *p2, int att);
  int GR_Disp_pln (Plane *pl1, int att);

  int GR_Disp_patch (int gTyp, int icol, int pNr, Point *pa);
  int GR_Disp_ipatch (int gTyp, int icol, int iNr, int *ia, Point *pa);
  int GR_Disp_fan (Point *ps, Point *pa1, Point *pa2, int pNr, int att);
  int GR_Disp_spu (int pNr, Point *pTab, int att);
  int GR_Disp_spu1 (int pNr, Point *pTab, int att);
  int GR_Disp_sru (ObjGX *oxi, int att);
  int GR_Disp_sur (ObjGX *oxi, int att);
  int GR_Disp_sbsp (int ptUNr, int ptVNr,  Point *pTab, int att);

  double GR_gtx_ckBlockWidth (char*, double);
  int    GR_gxt_strLen (char*);
  double GTX_chw_  (double);
  double GTX_cw_   ();
  double GTX_chh_  (double);
  double GTX_chhl_ (double);
  double GTX_chhl2 (double);
  double GTX_scale (double);
  double GTX_siz__ (double);



//================================================================
// inline functions

/// \code
/// GR_gxt_strLen         get stringlength of textnote
/// ignore all controlcharacters '['
/// \endcode
#define GR_gxt_strLen(txt)\
 strlen(txt) - UTX_cnr_chr (txt, '[')



/*========== EOF ==============*/
