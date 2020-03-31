// ../ut/ut_sur.h
// prereq:
// ../APP/tst_msh2d.h


#define TessDatSiz 256


// defaultsize points for a single surf
#define SUR_NR_PTS 1000



// tesselation-data for surfaces
// oxs      ObjGX of surface (support, boundaries)
// ssTyp    form of support-surface; Typ_PLN|Typ_SURSWP
// ssDbi    DB-index of support-surface
// ssDat    data of support-surface (was TSU_sbsp ..)
// tr_*     functions 3D to 2D and retour
// vc_pt    function to get vector for 3d-point on surface
// rx, ry   2D-range
// dx, dy   2D-increment (grid)
// tol      2D-tol
// td       individual for surface-type;
//          TessDat_SWP if typ = Typ_SURSWP
typedef struct {long dbi; int mdli; ObjGX *oxs;
                int ssTyp; long ssDbi; void *ssDat;
                int (*tr_2D_3D)();
                int (*tr_3D_2D)();
                int (*VC3D_PT3D)();
                double rx, ry, dx, dy, tol;
                char td[TessDatSiz];
                Vector         vcz;
                ColRGB         col;}                                SurStru;


//----------------------------------------------------------------
//   vcz        NULL: surface not planar; vc3 set.
//              else normalvector of planar surface; vc3 not set.
//              if planar surf has n patches: all patches have same normalvector
//----------------------------------------------------------------


typedef struct {
                 int bPln;
               }                    SUR_DAT_pln;


typedef struct {
                 double d1;
               }                    SUR_DAT_bsp;


// EOF
