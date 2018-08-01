// ../ut/msh2d.h
// prereq:
// ../ut/ut_memTab.h
// ../ut/ut_itmsh.h


typedef struct {
                 MemTab   tab;      // IndTab boundaries, tesselated patches
                 MemTab   ipa;      // int    index to points
                 MemTab   pa2;      // Point2 2D-points
                 MemTab   pa3;      // Point  3D-points
                 MemTab   vc3;      // Vec3f  normalVectors
                 MemTab(Fac3)   fac;      // Fac3   indexed faces
                 MemTab   fnb;      // Fac3   neighbour-faces
                 GridBox  gbx;
                 double   tol;
               }                                TessStru;

#ifdef _MSC_VER
#define MSH2D_STRU(mshs)\
  TessStru (mshs);\
  (mshs).tab = *((MemTab_IndTab*)&MEMTAB_NUL);\
  (mshs).ipa = *((MemTab_int*)&MEMTAB_NUL);\
  (mshs).pa2 = *((MemTab_Point2*)&MEMTAB_NUL);\
  (mshs).pa3 = *((MemTab_Point*)&MEMTAB_NUL);\
  (mshs).vc3 = *((MemTab_Vec3f*)&MEMTAB_NUL);\
  (mshs).fac = *((MemTab_Fac3*)&MEMTAB_NUL);\
  (mshs).fnb = *((MemTab_Fac3*)&MEMTAB_NUL);


#else
  // C99
#define MSH2D_STRU(mshs)\
  TessStru (mshs) = {\
                   .tab = _MEMTAB_NUL,\
                   .ipa = _MEMTAB_NUL,\
                   .pa2 = _MEMTAB_NUL,\
                   .pa3 = _MEMTAB_NUL,\
                   .vc3 = _MEMTAB_NUL,\
                   .fac = _MEMTAB_NUL,\
                   .fnb = _MEMTAB_NUL,\
                   .gbx = _GRIDBOX_NUL\
                   };
#endif


// get nr of quads (ix * iy; 0 is first)
#define MSH2D_quadNr()\
  (MSH2D_qbx.ix * MSH2D_qbx.iy)

// get quad-index (itx + (ity * MSH2D_qbx.ix)
#define MSH2D_quadInd(itx,ity)\
  itx + (ity * MSH2D_qbx.ix)


// get itx=col-index, dx=dist-horiz
void MSH2D_quadX (int *itx, double *dx, Point2 *ptx);
#define MSH2D_quadX(itx,dpx,ptx){\
  *(itx) = (ptx->x + MSH2D_tol - MSH2D_qbx.pMin.x) / MSH2D_qbx.dx;\
  *(dpx) = (*(itx) * MSH2D_qbx.dx) - (ptx->x - MSH2D_qbx.pMin.x);}


// get ity=row-index, dy=dist-vert
void MSH2D_quadY (int *ity, double *dy, Point2 *ptx);
#define MSH2D_quadY(ity,dpy,ptx){\
  *(ity) = (ptx->y + MSH2D_tol - MSH2D_qbx.pMin.y) / MSH2D_qbx.dy;\
  *(dpy) = (*(ity) * MSH2D_qbx.dy) - (ptx->y - MSH2D_qbx.pMin.y);}
  // DMOD (ity, (dy), ptx->y, MSH2D_ts->gbx.dy)


// get quadNr of left-quad of quad iqx; -1 = iqx is on left boundary; invalid.
int MSH2D_quadLeft (int iqx, int itx);
#define MSH2D_quadLeft(iqx,itx)\
  ((itx) == 0 ? -1  : iqx - 1)


// get quadNr of left-quad of quad iqx; -1 = iqx is on left boundary; invalid.
int MSH2D_quadBelow (int iqx);
#define MSH2D_quadBelow(iqx)\
  (iqx - MSH2D_qbx.ix)

#define MSH2D_IS_PLANAR MSH2D_qbx.ix == 0

// MSH2D_pst_set         set MSH2D_psta (grid-point-status)
#define MSH2D_pst_set(ipgb,mode){\
  ((char*)MSH2D_mpst.data)[ipgb - MSH2D_pgb_ips] = mode;}
#define MSH2D_pgst_set(ipgb,mode){\
  ((char*)MSH2D_mpst.data)[ipgb] = mode;}


// EOF
