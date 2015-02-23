
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


/// \code
/// ipt:  index points; 3 or 4
/// inf:  index neighbor-faces; 3 or 4
/// \endcode
typedef struct {int i1, i2, i3;}                                    Fac3;


/// \code
/// fTyp = 3: Face/TriangleMesh; fTab = *Face; Face->ipt = int[3];
/// fTyp = 4: Face/QuadMesh;
/// fTyp = 5: Fac3/tableOfTriangleIndices; fTab = *int; iTab[fNr][3];
/// fTyp = 6: optimized format
/// \endcode
typedef struct {void *fTab; int fNr; int fTyp;}                     Faces;


/// \brief Face with NeigbourFaces
/// \code
/// ip1, ip2, ip3   index to points
/// if1, if2, if3   index to NeigbourFaces. -1=outerBound; -2=unresolved; -3=new
///                ip3
///         if3   /  |
///            /     | if2
///         /        |
///   ip1 ----------ip2
///          if1
/// \endcode
typedef struct {int ip1, ip2, ip3, if1, if2, if3;}                  FacNf;


/// ipt:  index points;
typedef struct {int ipt[2];}                                        Edge;


/// i1, i2 index to points
typedef struct {int i1, i2;}                                        Edg3;


/// \brief Typ_EdgeLine
/// \code
///  iNr    nr of objects in ia
///  ia     Indexes into pointTable
///  typ    TYP_EDGLN_BL   2 EdgeLine (BreakLine)
///         TYP_EDGLN_IB   3 InnerBound
///         TYP_EDGLN_OB   4 OuterBound
///         TYP_EDGLN_AB   5 OuterBound - automatic created
///         TYP_EDGLN_FAC  6 faces (eg from GLU)
///  aux    for TYP_EDGLN_FAC: GL-typ; GL_TRIANGLE_STRIP|GL_TRIANGLE_FAN|..
/// \endcode
typedef struct {int *ia, iNr; char typ, aux, stat;}                 EdgeLine;


/// \code
/// eTyp[eNr]: NULL=undefined
///            0=internalEdge       (Edge*)
///            1=boundaryEdge       (Edge*)
///            2=EdgeLine           (EdgeLine*)
///            3=InnerBound         (EdgeLine*)
///            4=OuterBound         (EdgeLine*)
/// \endcode
typedef struct {Edge *eTab; int eNr; char *eTyp;}                   Edges;


/// \brief Typ_SURMSH  2007-03-24 TB.
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
/// see also EdgeLine
/// \endcode
typedef struct {int vxSt, vxNr, vxMax, surNb;}                      EdgSur;
*/


/// \brief Typ_SURBND
/// \code
/// suID    surface-ID (DB-index A)
/// contNr  contour-nr; first1=1, ..
/// typb    TYP_EDGLN_BL   2 EdgeLine (BreakLine)
///         TYP_EDGLN_IB   3 InnerBound
///         TYP_EDGLN_OB   4 OuterBound
///         TYP_EDGLN_AB   5 OuterBound - automatic created
///         TYP_EDGLN_FAC  6 faces (eg from GLU)
/// typt    Typ_SURCIR   (fan)
///         Typ_SURSTRIP (strip)
/// dir     0=undefined; 1=CCW; -1=CW
/// \endcode
typedef struct {int suID, contNr; char typb, typt, dir, stat;}      BndSur;
//                                                                  ^


/*
/// \brief Index-table; Typ_IndTab
/// \code
///  ibeg   begin-index; points to first object of index-list
///  iNr    nr of objects in index-list
///  typi   typ of indextable (form int4)
///  typd   typ of datatable
///  aux    for TYP_EDGLN_FAC: GL-typ; GL_TRIANGLE_STRIP|GL_TRIANGLE_FAN|..
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

/// UT3D_tria_pta_fac          create triangle from Fac3
void   UT3D_tria_pta_fac(Triangle*, Fac3*, Point*);
#define UT3D_tria_pta_fac(tri,fac,pTab){\
 (tri)->pa[0] = &(pTab)[(fac)->i1];\
 (tri)->pa[1] = &(pTab)[(fac)->i2];\
 (tri)->pa[2] = &(pTab)[(fac)->i3];}



/// EOF
