// ITMSH irregular-triangle-mesh generator functions
//
// prerequisites:
// #include "../ut/ut_geo.h"              // Point ...
// #include "../ut/ut_memTab.h"           // MemTab_..



#define MSH_EDGLN_BL    1  // BreakLine
#define MSH_EDGLN_AB    2  // OuterBound - automatic created
#define MSH_EDGLN_IB    3  // InnerBound - user defined points
#define MSH_EDGLN_OB    4  // OuterBound - user defined points
#define MSH_EDGLN_IC    5  // InnerBound - computed points
#define MSH_EDGLN_OC    6  // OuterBound - computed points
#define MSH_EDGLN_LN    7  // lines (test)
#define MSH_EDGLN_INT  13  // intersection-curve
#define MSH_EDGLN_SB   -1  // surface-boundary (tile)
#define MSH_EDGLN_ERR -16  // unset, invalid ..
#define MSH_GRIDBOX     8  // points in a rectangular grid
#define MSH_nFAC        9  // indexed faces (Fac3)
#define MSH_PATCH      10  // patch = group of faces (eg from GLU)
#define MSH_SURF       11  // surface =  group of patches
#define MSH_SKIN       12  // skin = group of surfaces; body if closed.
#define MSH_SUPP_SUR   12  // supporting-surface
#define MSH_ISEG_B     14  // segmentNrs for boundaries; aux=bndNr



/// \code
/// pTyp[pNr]: NULL=undefined;
///            0=internal; 1=internal&crease;
///            2=boundary; 3=boundary&crease
/// \endcode
typedef struct {Point *pTab; int pNr; char *pTyp;}                  Verts;


/// \code
/// ipt:  index points; 3 or 4
/// inf:  index neighbor-faces; 3 or 4
/// \endcode
typedef struct {int *ipt; int *inf;}                                Face;


// i1,i2,i3   index points
// fst0       status face; 16=marked-for-delete
//                i3
//        e3   /  |
//          /     | e2
//       /        |
//   i1 ----------i2
//          e1
typedef struct {int i1, i2, i3; char fst0, fst1, fst2, fst3;}       Fac3;


/// \code
/// fTyp = 3: Face/TriangMesh; fTab = *Face; Face->ipt = int[3];
/// fTyp = 4: Face/QuadMesh;
/// fTyp = 5: Fac3/tableOfTriangIndices; fTab = *int; iTab[fNr][3];
/// fTyp = 6: optimized format
/// \endcode
typedef struct {void *fTab; int fNr; int fTyp;}                     Faces;


// /// \brief Face with NeigbourFaces
// /// \code
// /// ip1, ip2, ip3   index to points
// /// if1, if2, if3   index to NeigbourFaces. -1=outerBound; -2=unresolved; -3=new
// /// \endcode
// typedef struct {int ip1, ip2, ip3, if1, if2, if3;}                  FacNf;


/// ipt:  index points;
typedef struct {int ipt[2];}                                        Edge;


/// i1, i2 index to points
typedef struct {int i1, i2;}                                        Edg3;


// replaced by IntTab
// /// \brief Typ_IntTab
// // better use struct IndTab
// /// \code
// ///  iNr    nr of objects in ia
// ///  ia     Indexes into pointTable
// ///  typ    MSHIG_EDGLN_*    see ../ut/ut_face.h
// ///  aux    for MSH_PATCH: GL-typ; GL_TRIANGLE_STRIP|GL_TRIANGLE_FAN|..
// //   stat   0=default; 1=overlapped; 2=overlapping
// /// \endcode
// typedef struct {int *ia, iNr; char typ, aux, stat;}                 IntTab;


/// \code
/// eTyp[eNr]: NULL=undefined
///            0=internalEdge       (Edge*)
///            1=boundaryEdge       (Edge*)
///            2=IntTab           (IntTab*)
///            3=InnerBound         (IntTab*)
///            4=OuterBound         (IntTab*)
/// \endcode
typedef struct {Edge *eTab; int eNr; char *eTyp;}                   Edges;


/// \brief Typ_SURPMSH  2007-03-24 TB.
/// \code
/// f:    Faces; NULL=not yet created
/// e:    Edges; NULL=not yet created
/// mTyp: 0=open_mesh; 1=closed_mesh.
/// \endcode
typedef struct {Verts *p; Faces *f; Edges *e; int mTyp;}            Mesh;


/// \brief Typ_EdgSur
/// \code
/// ipt       index endpoint of segment
/// nbsid     neighbourSurfaceID
/// \endcode
typedef struct {int ipt, nbsid;}                                    SegBnd;

/*
/// \brief Typ_EdgSur
/// \code
/// vxSt    index to startPoint
/// vxNr    nr of points following startPoint
/// surNb   surfaceIndex of neighbourSurface (surf-record in BODY)
/// see also IntTab
/// \endcode
typedef struct {int vxSt, vxNr, vxMax, surNb;}                      EdgSur;
*/


/// \brief Typ_SURBND
/// \code
/// suID    surface-ID (DB-index A)
/// contNr  contour-nr; first1=1, ..
/// typb    MSH_EDGLN_BL   2 IntTab (BreakLine)
///         MSH_EDGLN_IB   3 InnerBound
///         MSH_EDGLN_OB   4 OuterBound
///         MSHIG_EDGLN_AB   5 OuterBound - automatic created
///         MSH_PATCH  6 faces (eg from GLU)
/// typt    Typ_SURCIR   (fan)
///         Typ_SURSTRIP (strip)
/// dir     0=undefined; 1=CCW; -1=CW
/// \endcode
typedef struct {int suID, contNr; char typb, typt, dir, stat;}      BndSur;
//


/*
/// \brief Index-table; Typ_IndTab
/// \code
///  ibeg   begin-index; points to first object of index-list
///  iNr    nr of objects in index-list
///  typi   typ of indextable (form int4)
///  typd   typ of datatable
///  aux    for MSH_PATCH: GL-typ; GL_TRIANGLE_STRIP|GL_TRIANGLE_FAN|..
///  stat   -
/// \endcode
typedef struct {int ibeg, iNr; char typi, typd, aux, stat;}         IndTab;
// size = 12
*/

//----------------------------------------------------------------
// ut_tria

/// \brief UT3D_ck_triaOrientI        get orientation from indices
/// \code
/// CCW is 01 12 20;      CW ist 02 21 10;    Val0=1, Val1=2, Val2=4.
/// \endcode
#define UT3D_ck_triaOrientI(i0,i1)\
 (((((i0)==0)&&((i1)==1))||(((i0)==1)&&((i1)==2))||(((i0)==2)&&((i1)==0))) ? CCW : CW)

/// \brief UT3D_ck_triaOrientV        get orientation from indices
/// \code
/// CCW is 01 12 20;      CW ist 02 21 10;    Val0=1, Val1=2, Val2=4.
/// \endcode
#define UT3D_ck_triaOrientV(i0,i1)\
 (((((i0)==1)&&((i1)==2))||(((i0)==2)&&((i1)==4))||(((i0)==4)&&((i1)==1))) ? CCW : CW)

/// \brief UT3D_ind3Tria_2ind         give index of 3.point of triangle (0/1/2)
/// \code
///                            0+1 -> 2; 1+2 -> 0; 2+0 -> 1.
#define UT3D_ind3Tria_2ind(i1,i2) (IABS((i1)+(i2)-3))

/// UT3D_tria_fac          create triangle from indexed-triangle (Fac3)
void   UT3D_tria_fac(Triang*, Fac3*, Point*);
#define UT3D_tria_fac(tri,fac,pTab){\
 (tri)->pa[0] = &(pTab)[(fac)->i1];\
 (tri)->pa[1] = &(pTab)[(fac)->i2];\
 (tri)->pa[2] = &(pTab)[(fac)->i3];}



// estimate nr of faces necessary for edgNr edges
// In: nr of edges; Out: nr of faces
#define MSH2D_facnr_edgnr(edgNr) edgNr+4

// MSH2D_edgnr_facnr     estimate nr of edges for facnr faces
#define MSH2D_edgnr_facnr(facNr) (facNr*3)+1




//----------------------------------------------------------------
// struct for nifac (indexed-faces)
// init MshFac with _MSHFAC_NUL
// oTyp    type of surface; 
// ipa     indexarray paralle fac; can be NULL - use only indexes of Fac3
// vca     normalVectors; one for every point; if (oTyp==Typ_SURPLN) only one vector
// mSpc    memSpc for fac,pa3,ipa,vc3; must be freed if(mSpc) UME_free

typedef struct {int     fNr,       // nr of faces
                        ptNr,      // nr of points
                        oTyp;      // type of surface
                Fac3    *fac;      // indexed faces (3 indexes into ipa or pa3)
                Point   *pa3;      // 3D-points
                int     *ipa;      // index to points
                Vec3f   *vc3;      // normalVectors; one for every point
                Memspc   mSpc;     // memSpc for fac,pa3,ipa,vc3;
               }                                          MshFac;

// see also struct MshDat MshSur

#define _MSHFAC_NUL {0,0,0, NULL,NULL,NULL,NULL, UME_NEW}
//                          fac  pa3  ipa  vc3   mSpc




//----------------------------------------------------------------
extern MemTab MEMTAB_NUL;



typedef_MemTab(int);
typedef_MemTab(char);
typedef_MemTab(Point2);
typedef_MemTab(Point);
typedef_MemTab(Vec3f);
typedef_MemTab(Line2);
typedef_MemTab(Line);
typedef_MemTab(Fac3);
typedef_MemTab(IntTab);
typedef_MemTab(IndTab);
// typedef_MemTab(IgaTab);
typedef_MemTab(ObjDB);
typedef_MemTab(CurvCCV);
typedef_MemTab(ObjGX);

// DOES NOT WORK:
// #define MemTab_int MemTab
// #define MemTab_char MemTab
// #define MemTab_Point2 MemTab
// #define MemTab_Point MemTab
// #define MemTab_Vec3f MemTab
// #define MemTab_Line MemTab
// #define MemTab_Fac3 MemTab
// #define MemTab_IntTab MemTab
// #define MemTab_IndTab MemTab
// EOF
