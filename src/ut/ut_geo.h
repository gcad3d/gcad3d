/***************************************************************************
                          ut_geo.h  -  description
                             -------------------
    begin                : Fri Aug 25 2000
    copyright            : (C) 2000 by CADCAM-Services Franz Reiter
    email                : franz.reiter@cadcam.co.at

 ***************************************************************************

Achtung: Bug in MS-Windows: never use "double rad1"


-------------------------------------------
TODO:
All curves: (Circ,CurvElli) add flag trimmed or closed;
New curve "trimmed_curve": 2 points/parameters + Link unlimited_curve
  points/parameters via dbi-index; 0=invalid; neg=temp, pos=normal.



-------------------------------------------
Korr:
2004-08-06  struct Circ: ango added !! RF.
2003-05-10  Memspc neu zu. RF.
2002-02-12  Ditto neu zu. RF.
2001-04-26  alle #define Typ_xx aus ut_gr zu, RAD_1 genauer.


------------------------------------
../ut/ut_tol_const.h         UT_TOL_pt UT_DISP_cv UT_TOL_min0 ..
../ut/ut_geo_const.h         UT3D_PT_NUL UT3D_VECTOR_X


-------------------------------------------
*//*!
\file ../ut/ut_geo.h
\ingroup grp_ut_geom
*/

#include "../ut/ut_types.h"               // INT_8 - UINT_64
#include "../ut/gr_types.h"               // SYM_* ATT_* Typ_Att_* LTYP_*
#include "../ut/AP_types.h"               // Typ_PT ..
#include "../ut/ut_mem.h"                 // MEM_*
#include "../ut/ut_uti.h"                 // UTI UTP BIT I* D* 
#include "../ut/ut_umem.h"                // Memspc MemObj UME_*
// ../gui/gui_types.h
// ../ut/func_types.h                        // ATT_COL_. SYM_. Typ_Att_
// ../xa/xa_sele.h


/*
// NUR bei LINUX (ex math.h):
#define RAD_360 M_2_PI                    // 360 Grad
#define RAD_180 M_PI                      // 180 Grad
#define RAD_90  M_PI_2                    //  90 Grad
#define RAD_45  M_PI_4                    //  45 Grad
#define RAD_30  ??                        //  30 Grad
*/

// angles ..
#define RAD_360   6.2831853071795862319    ///< 360 degree
#define RAD_315   5.497787144              ///< 315
#define RAD_270   4.7123889803846896739    ///< 270
#define RAD_225   3.92699081698724151736   ///< 225
#define RAD_180   3.14159265358979323846   ///< 180
#define RAD_135   2.3561944901923448368    ///< 135
#define RAD_120   2.0943951023931953735    ///< 120
#define RAD_90    1.5707963267948965579    ///<  90
#define RAD_60    1.0471975511965976313    ///<  60
#define RAD_45    0.7853981633974482789    ///<  45
#define RAD_30    0.5235987755982988156    ///<  30
#define RAD_10    0.174532925199432954     ///<  10
#define RAD_1     0.0174532925199432954    ///<   1 degree
#define RAD_01    0.0017453292519943295    ///<   0.1
#define RAD_1_360 0.15915494309189534561   ///<  1 / 6.28 
#define SR_3      1.7320508075688772       ///< SQRT(3)
#define SR_2      1.4142135623730951       ///< SQRT(2)
#define SR_PI     1.7724538509055159       ///< SQRT(PI) = SQRT(RAD_180)


#define CCW                1             ///< counterclockwise
#define CW                -1             ///< clockwise

#define NOT                !             /// "0 ==" or "!"

#define YES                0             ///< FALSE
#define NO                 1             ///< TRUE

#define ON                 0             ///< FALSE
#define OFF                1             ///< TRUE

#define DIR_FWD            0
#define DIR_BWD            1

#define LIMITED            0
#define UNLIMITED          1






#define UT_BEZDEG_MAX       50        ///< maximal degree of Bezier curve







/*============== Belegung der att - Bitfelder ============================= */
/// das Absolut-Bit; Achtung: 0=ON=absolut, 1=relativ.
#define ATT_BIT_ABS          1
/*
zB. Absolut-Bit setzen (mit OR):      obj.att = obj.att | ATT_BIT_ABS;
    Absolut-Bit filtern (mit AND; ergibt 0 oder ATT_BIT_ABS):
      i1 = obj.att & ATT_BIT_ABS;
      Achtung: entspricht NICHT ON/OFF!
*/


/// Limited; 0=On; 2=unbegrenzte Linie/Vollkreis
#define ATT_BIT_LIM          2


/// OBJ_SIZ_MAX        max. obj-size (for get, copy ..) see UTO_siz_stru
// max = CurvElli;         160 = 6 Points + 16 byte | 20 doubles
#define OBJ_SIZ_MAX        256


/// OBJ_UNKNOWN        get memoryspace for any type of object (has maximum size)
// Size of OBJ_UNKNOWN is OBJ_SIZ_MAX bytes.
// OBJ_SIZ_MAX (ou1);    // gives "char ou1[OBJ_SIZ_MAX];"
// For curves with point-tables (polygon, spline) and value-arrays (splines)
// these additional memory-spaces cannot be allocated in OBJ_UNKNOWN.
// For this curves additional 'Memspc' is necessary.
//typedef struct {char dat[OBJ_SIZ_MAX];} ObjUnknown;
typedef char ObjUnknown[OBJ_SIZ_MAX];
// see also ObjBin



//....................... aux. structs: ..................................
/// union long / short[2]
typedef union {long i4; short i2[2];}                               uni_i4i2;

/// int = short + short
typedef struct {short i20, i21;}                                    stru_2i2;

/// int = char[4] + char
typedef struct {unsigned b123:24, b4:8;}                            stru_c3c1;



//....................... Math. Objects: ..................................
typedef double Mat_3x2[2][3];            ///< Typ_M3x2
typedef double Mat_3x3[3][3];            ///< Typ_M3x3
typedef double Mat_4x3[3][4];            ///< Typ_M4x3 size 96
typedef double Mat_4x4[4][4];            ///< Typ_M4x4

/// quaternion
typedef struct {double e0, e1, e2, e3;}                             Quat;

typedef struct {double a, b;}                                       dcomplex;
typedef struct {double a, b, c, d;}                                 polcoeff_d3;
typedef struct {double a, b, c, d, e;}                              polcoeff_d4;
typedef struct {double a, b, c, d, e, f;}                           polcoeff_d5;

/// Typ_polynom_d3 Typ_CVPSP3
typedef struct {double u; polcoeff_d3  x, y, z;}                    polynom_d3;
// size = 104


/// Typ_Polynom1
typedef struct {int polNr; double *pol;}                            Polynom1;
// Struktur von pol:  double pol[polNr]


/// Typ_Polynom3
typedef struct {int polNr; double *pol[3];}                         Polynom3;
// pol:  double pol[3][polNr]


/// Typ_Polynom_
// pol:  double pol[dim1][dim2]
// Access: pol[ind1*dim2*ind2] = pol[ind1][ind2] !
typedef struct {int dim1, dim2; double *pol;}                       Polynom_;






//....................... Geom. Objects: ..................................

/// 2D-point, Typ_PT2
typedef struct {double x, y;}                                       Point2;
// size = 16

typedef struct {float x, y, z;}                                     Pointf;
// size = 12

/// 3D-point, Typ_PT
typedef struct {double x, y, z;}                                    Point;
// size = 24

/// 3D-weighted-point, Typ_WPT
typedef struct {double x, y, z, w;}                                 wPoint;
// size = 32

/// 2D-vector, Typ_VC2
typedef struct {double dx, dy;}                                     Vector2;
// size = 16

/// 3D-vector, Typ_VC3F
typedef struct {float dx, dy, dz;}                                  Vec3f;

/// 3D-vector, Typ_VC
typedef struct {double dx, dy, dz;}                                 Vector;
// size = 24



//....................... containerobjects: ................................

/// \brief Index-table; Typ_IndTab; _INDTAB_NUL
/// \code
///  ibeg   begin-index; points to first object of index-list
///  iNr    nr of objects in index-list
///  typi   typ of indextable (form int4)     MSH_EDGLN_OB
///  typd   typ of datatable
///  aux    for MSH_PATCH: GL-typ; GL_TRIANGLE_STRIP|GL_TRIANGLE_FAN|..
///         for MSH_EDGLN_OB/IB: 0=fwd (CCW) 1=bwd (CW)
///  stat   -
/// \endcode
typedef struct {int ibeg, iNr; char typi, typd, aux, stat;}         IndTab;
// size = 12


/// \brief indexGroupArray  Typ_IgaTab;   _IGATAB_NUL
///  ibeg   begin-index; points to first object of index-list
///  iNr    nr of objects in index-list
///  typ    typ of objects                 eg MSH_EDGLN_OB
///  stat
typedef struct {int ibeg, iNr, iRef; short ind; char typ, stat;}    IgaTab;
// size = 16



/// \brief Typ_ObjGX
/// \code
/// complex-object; can hold primitives (Point|Line ..),
///   or a table of ObjGX
/// typ   what info is in the record in *data
/// form  type of record of *data
/// data  address of data; if(form==Typ_Index) data=(long)DB-Index
/// siz   number of records in *data; all of them have structure "form"
/// dir   direction; 0=normal, 1=reverse.
/// \endcode
typedef struct {void *data; short typ, form;
                unsigned siz:24, dir:1, aux:7;}                     ObjGX;
// size = 16


/// \brief table of binary-objects      ObjTab    Typ_ObjTab    INF_ObjTab
/// \code
/// oSiz   size of oTyp, oDat (max. nr. of records)
/// oNr    used nr of objects int oTyp, oDat
/// oTyp   table of types
/// oDat   table of pointers to binary-object into oSpc
/// oSpc   keeps the binary-objects
/// xDat   auxilary-objects; type = xTyp, NULL = none
/// xTyp   type of objects at xDat; 0=none, else eg BBox2|BBox ..
/// fmtb   format of binary obj's: Typ_GEOB_2D|Typ_GEOB_3D
/// spcTyp  type of memory;                                   See INF_MEM_TYP
///
/// Functions: OTB_
/// ObjTab = 	list of [oTyp, pointer to obj, Memspc for obj, aux.obj]
/// \endcode
typedef struct {void **oDat, *xDat; Memspc oSpc; 
                int *oTyp, oSiz, oNr; UINT_16 xSiz;
                short xTyp, fmtb; char spcTyp, u2, u3, u4;}         ObjTab;
// size = ??
// use xDat = BBox2 eg ((BBox2*)otb->xDat)[ii].pb1


/// \brief Typ_Group DB-Objects        Typ_ObjDB
/// \code
/// typ   DB-typ
/// dbInd DB-index
/// dlInd DispListIndex; 0=undefined
/// \endcode
typedef struct {long dbInd, dlInd; short typ, stat;}                ObjDB;


/// \brief Typ_ObjSRC sourceObject
/// \code
/// typ,dbi    DB-typ and DB-Index
/// lnr        source-lineNr
/// dli        DispListIndex
/// lPos       startpos of source-line
/// ll         length of sourceline in chars
/// iPar       index to parentrecord; -1=primary obj
/// \endcode
typedef struct {long dbi, dli, lnr; char *lPos; int iPar, typ, ll;} ObjSRC;


/// \brief Typ_ObjTXTSRC type of expression of sourceObject 
/// \code
/// typ,form    type & form of expression; see SRC_txo_srcLn__()
/// ilen        length in chars of obj
/// ipar        index to parent (into tso); -1=primary level.
/// ioff        offset in chars from start of codestring.
/// see APED_txo_..
/// \endcode
typedef struct {short typ, form, ilen, ipar; int ioff;}             ObjTXTSRC;


/// \brief atomicObjects Typ_ObjAto ATO_
/// \code
/// nr         nr of used records
/// siz        nr of typ,val - records
/// txsiz      size of txt (if(!txt))
/// ilev       level; -1=primary level. NULL=unused
/// txt        for strings; NULL for none (was APT_defTxt)
/// spcTyp     type of memory;                                   See INF_MEM_TYP
///
/// see ../xa/xa_ato.h _OBJATO_NUL
/// \endcode
typedef struct {int nr, siz, txsiz; int *typ; double *val; short *ilev;
                char *txt; char spcTyp, uu1, uu2, uu3;}             ObjAto;


/// \brief          DO NOT USE; replaced by ObjGX.
/// \code
/// primitive 2D-object.
/// \endcode
typedef struct {int typ, att, ID, ID1; Point2 p1, p2, pc;
                 double rad; long nam;}                             ObjG2;


/// \brief          DO NOT USE; replaced by ObjGX.
/// \code
/// primitive object; Pt, Ln or Ci.
/// \endcode
typedef struct {int typ, att; long ID; void *data;
                Point p1,p2,pc; Vector vz; double rad;}             ObjG;


/* UNUSED
/// \brief          DO NOT USE; replaced by ObjGX.
/// \code
/// can be replaced by ObjGX.data
/// \endcode
typedef union {Point *pt; Line *ln; Vector *vc;
               Circ *ci; ObjGX *cv; CurvElli *el;
               CurvPoly *plg; CurvBSpl *bsp;
               void *vp;}                                           ObjX;
// size = void* = 4


/// \brief          DO NOT USE; replaced by ObjGX.
/// \endcode
typedef union {Point pt; Vector vc; Line ln; Circ ci;}              ObjUX;
*/


/// \brief binary-object    Typ_ObjBin
/// \code
/// can only hold L C R Elli; 
/// For curves with point-tables (polygon, spline) and value-arrays (splines)
/// these additional memory-spaces must be allocated in additional Memspc
/// \endcode
typedef struct {int typ; char obj[OBJ_SIZ_MAX];}                    ObjBin;
// see also ObjUnknown


/// \brief ContourCurve
/// \code
/// pa      PointArray 3D-Points
/// p2a     PointArray 2D-Points (Format 3D-Point!)
/// iNr     Nr of points
/// p1,p2   BoundingBox
/// typ     C=Contour A=Automatic G=Grid
/// use     A=active, I=inactive,
///         B=Base, C=Cut-Fenster, M=Merged (results), D=deleted.
/// dir     '+'=counterclockwise, '-'=clockwise
/// \endcode
typedef struct {Point *pa; Point *p2a; int iNr;
                Point p1, p2;
                unsigned char cTyp, use, dir, temp;}                ContTab;


// /// \brief group of consecutive objects   REPLACED WITH IgaTab
// /// \code
// /// ind     index of first obj
// /// oNr     nr of obj's; index of last obj is (ind + nr - 1)
// /// \endcode
// typedef struct {long ind; unsigned typ:8, oNr:24;}                  ObjRange;




//....................... Geom. Objects: ..................................

/// 2D-line, Typ_LN2
typedef struct {Point2 p1, p2; char typ;}                           Line2;
///  typ      0  both sides limited                               -
///           1  1 side limited  (p1 is startpoint, p2 unlimited) UNL1
///           2  1 side limited  (p2 is startpoint, p1 unlimited) UNL2
///           3  both sides unlimited                             UNL

/// 3D-line, Typ_LN
typedef struct {Point p1, p2; char typ, aux, stat, uu1;}            Line;
///  typ      0  both sides limited                               -
///           1  1 side-limited  (p2 is startpoint, p1 unlimited) UNL2
///           2  1 side-limited  (p2 is startpoint, p1 unlimited) UNL2
///           3  both-sides-unlimited                             UNL
// size 56; 4 char unused

/// 3D-line, Typ_CVLN3
typedef struct {Point pt0; Vector vcl; double lnl;}                 CVLn3;



/// triangle, Typ_Tria
typedef struct {Point *pa[3];}                                      Triang;
// size = 12; 3 pointers !

/// triangle-2D, Typ_Tria2
typedef struct {Point2 *pa[3];}                                     Triang2;
// size = 12; 3 pointers !



/// \brief 2D-circle, Typ_CI2, Typ_CI2
/// \code
/// p1     .. startpoint
/// p2     .. endpoint
/// pc     .. Centerpoint
/// rad    .. radius; positiv for CCW, negative for CW
/// ango   .. opening angle in rad; positiv for CCW, negative for CW
/// \endcode
/// 2004-08-06 ango added. RF.
typedef struct {Point2 p1, p2, pc; double rad, ango;}               Circ2;
// size = 64


/// \brief 2D-circle in centerPosition, Typ_CI2C
/// \code
/// rad    .. radius; positiv for CCW, negative for CW
/// angs   .. angle at startpoint in rad; positiv for CCW, negative for CW
/// ango   .. opening angle in rad; positiv for CCW, negative for CW
/// \endcode
typedef struct {double rad, angs, ango;}                            Circ2C;
// size = 24


/// \brief 3D-circle, Typ_CI
/// \code
/// p1     .. startpoint
/// p2     .. endpoint
/// pc     .. Centerpoint
/// vz     .. axis, normal-vector; must be normalized
/// rad    .. radius; positiv for CCW, negativ for CW
/// ango   .. opening-angle in rad, negativ for CW
/// \endcode
typedef struct {Point p1, p2, pc; Vector vz; double rad, ango;}     Circ;
// size = 112


/// \brief 2D-ellipse in centerPosition Typ_CVELL2C
/// \code
/// p1     .. startpoint
/// p2     .. endpoint
/// a      .. major axis (lenght; is parallel to x-axis)
/// b      .. minor axis (lenght; is parallel to y-axis)
/// srot       direction; 0=CCW, 1=CW.  See INF_struct_dir.
/// clo        closed; 0=yes, 1=not_closed; -1=undefined; -2=degen;
/// trm        trimmed; 0=yes, 1=not_trimmed, -1=undef; see INF_struct_closed
/// \endcode
typedef struct {Point2 p1, p2; double a, b; double angs, ango;
                char srot, clo, trm, uu2;}                          CurvEll2C;
// size = 56

/// \brief ellipse, Typ_CVELL2
/// \code
/// dir    .. 1=CCW, -1=CW
/// p1     .. startpoint
/// p2     .. endpoint
/// pc     .. Centerpoint
/// va     .. major axis (lenght!)
/// vb     .. minor axis (lenght!)
/// srot       direction; 0=CCW, 1=CW.  See INF_struct_dir
/// clo        closed; 0=yes, 1=not_closed; -1=undefined; -2=degen;
/// trm        trimmed; 0=yes, 1=not_trimmed, -1=undef; see INF_struct_closed
/// \endcode
typedef struct {Point2 p1, p2, pc; Vector2 va, vb; double ango;
                char srot, clo, trm, uu2;}                          CurvEll2;
// size = 96

/// \brief ellipse, Typ_CVELL
/// \code
/// p1     .. startpoint
/// p2     .. endpoint
/// pc     .. Centerpoint
/// vz     .. z-axis
/// va     .. major axis (lenght!)
/// vb     .. minor axis (lenght!)
/// ango   .. opening angle in rad; positiv for CCW, negative for CW
/// srot       direction; 0=CCW, 1=CW.  See INF_struct_dir.
/// clo        closed; 0=yes, 1=not_closed; -1=undefined; -2=degen
/// trm        trimmed; 0=yes, 1=not_trimmed, -1=undef; see INF_struct_closed
/// \endcode
typedef struct {Point p1, p2, pc; Vector vz, va, vb; double ango;
                char srot, clo, trm, uu2;}                          CurvElli;
// size = 160


/// \brief Curve: 2D-polygon, Typ_CVPOL2
/// \code
/// ptNr        number of points
/// pTab[ptNr]  cornerpoints
/// \endcode
typedef struct {int ptNr; Point2 *pTab;}                            CurvPol2;
//   dir:  only necessary for closed, cyclic curve
//   size =


/// \brief Curve: polygon, Typ_CVPOL INF_Typ_CVTRM
/// \code
/// ptNr    ... number of control points
/// v0      ... start parameter (len-offset; see INF_struct_par
/// v1      ... end parameter (len-offset)
/// lvTab[ptNr] length absolut NULL = undefined
/// cpTab[ptNr] cornerpoints
/// dir        direction; 0=fwd, 1=bwd. See INF_struct_dir.
/// clo        closed; 0=yes, 1=not_closed; -1=undefined; -2=degen
/// trm        trimmed; 0=yes, 1=not_trimmed, -1=undef; see INF_struct_closed
/// \endcode
typedef struct {int ptNr; double v0, v1, *lvTab; Point *cpTab;
                char dir, clo, trm, uu4;}                           CurvPoly;
// dir:  only necessary for closed, cyclic curve
// v0,v1 (parameters of startpoint, endPoint) of Circ, CurvElli are normalized (0-1)
//       CurvPoly has length, CurvBSpl has knotvalues;
// size = 28


/// \brief Curve: B-spline   Typ_CVBSP2
/// \code
/// ptNr    ... number of control points
/// deg     ... degree of B-spline curve
/// v0      ... B-spline curve start parameter; see INF_struct_par
/// v1      ... B-spline curve end parameter; see INF_struct_par
/// kvTab[ptNr+deg+1] knot values (non-decreasing, <= v0 < v1 <= )
/// cpTab[ptNr]       control points
/// dir        direction; 0=fwd, 1=bwd  See INF_struct_dir.
/// clo        closed; 0=yes, 1=not_closed; -1=undefined; -2=degen
/// trm        trimmed; 0=yes, 1=not_trimmed, -1=undef; see INF_struct_closed
/// \endcode
typedef struct {int ptNr; double v0, v1, *kvTab;
                Point2 *cpTab; char deg, dir, clo, trm;}            CurvBSpl2;
// size = 32


/// \brief Curve: B-spline   Typ_CVBSP _CVBSP_NUL
/// \code
/// ptNr    ... number of control points
/// deg     ... degree of B-spline curve
/// v0      ... B-spline curve start parameter; see INF_struct_par
/// v1      ... B-spline curve end parameter
/// kvTab[ptNr+deg+1] knot values (non-decreasing, <= v0 < v1 <= )
/// cpTab[ptNr]       control points
/// dir        direction; 0=fwd, 1=bwd  See INF_struct_dir.
/// clo        closed; 0=yes, 1=not_closed; -1=undefined; -2=degen
/// trm        trimmed; 0=yes, 1=not_trimmed, -1=undef; see INF_struct_closed
/// \endcode
typedef struct {int ptNr; double v0, v1, *kvTab;
                Point *cpTab; char deg, dir, clo, trm;}       CurvBSpl;
// size = 32


/// \brief Rational B-Spline-curve; Typ_CVRBSP
/// \code
/// ptNr              ... number of control points
/// deg               ... degree
/// v0                ... start parameter; see INF_struct_par
/// v1                ... end parameter
/// kvTab[ptNr+deg+1] ... knot values (non-decreasing)
/// wTab[ptNr]        ... weights
/// cpTab[ptNr]       ... control points
/// dir        direction; 0=fwd, 1=bwd  See INF_struct_dir.
/// clo        closed; 0=yes, 1=not_closed; -1=undefined; -2=degen
/// trm        trimmed; 0=yes, 1=not_trimmed, -1=undef; see INF_struct_closed
/// \endcode
typedef struct {int ptNr; double v0, v1, *kvTab, *wTab;
                Point *cpTab; char deg, dir, clo, trm;}             CurvRBSpl;
// size = 36


/// 2D-Bezier curve  Typ_CVBEZ2
typedef struct {int ptNr; Point2 *cptab; double va, vb;}            CurvBez2;


/// \brief Bezier curve     Typ_CVBEZ
/// \code
/// ptNr   ... number of control points; degree = (ptNr - 1)
/// cptab  ... control points of Bezier curve (number = ptNr)
/// va, vb ... global end-parameters of Bezier curve
/// \endcode
typedef struct {int ptNr; Point *cptab; double va, vb;}             CurvBez;
// size = 24


/// \brief Rational Bezier curve     Typ_CVRBEZ
/// \code
/// ptNr   ... number of control points; degree = (ptNr - 1)
/// cptab[ptNr] control points of rational Bezier curve
/// wtab[ptNr]  weights of rational Bezier curve (number = ptNr)
/// va, vb ... global end-parameters of rational Bezier curve
/// \endcode
typedef struct {int ptNr;Point *cptab;double *wtab;double va, vb;}  CurvRBez;


// // Typ_CVVC    2007-03-24 RF.
// // Curve with Vectors for all Curvepoints from Offsetcurve.
// typedef struct {void *cv1; void *cv2; int cTyp;}                    CurvVec;


/// \brief Clothoid curve   Typ_CVCLOT    ../ut/cvClot.c
/// \code
/// stp        startpoint
/// stv        startvector
/// plv        normalvector of plane of curve (not parallel stv)
/// pc         curvetype parameter (>0)
/// cs         curvature at startpoint (>=0)
/// ce         curvature at endpoint (>=0, !=cs)
/// lr         0/1 <=> left/right bended curve
/// \endcode
typedef struct {Point stp; Vector stv, plv;
            double pc, cs, ce; int lr;}                             CurvClot;



/// \brief polygonal_representation_of_curve   CurvPrcv   Typ_PRCV   functions: PRCV
/// \code
/// dbi        database index of basecurve;
/// mdli       subModelNr; 0-n = sind in Submodel; -1=main
/// ptNr       nr of used points, parameters, indexes
/// siz        size of npt, npar, nipt
/// npt        Point[ptNr] - polygon
/// npar       parameter of point on basecurve (UT_VAL_MAX=undefined)
/// nipt       database index of points; 0=undefined
/// spcTyp     type of memory;                                   See INF_MEM_TYP
/// stat       0=free; 1=in-use
/// \endcode
typedef struct {long dbi; int mdli, ptNr, siz;
                Point *npt; double *npar; long *nipt;
                short typ; char spcTyp, stat;}                      CurvPrcv;


/// \brief Trimmed curve      CurvCCV        Typ_CVTRM   _CCV_NUL
/// \code
/// typ        type baseCurv (L,C,S)
/// dbi        database index baseCurv (0=undef)
/// is0        segmentNr of start-parameter (for CurvCCV in CurvCCV only)
/// is1        segmentNr of end-parameter (for CurvCCV in CurvCCV only)
/// v0         start-parameter on baseCurv (UT_VAL_MAX=undefined)
///            v0 gives the startpoint even if dir=bwd.      See INF_struct_par
/// v1         end-parameter on baseCurv (UT_VAL_MAX=undefined)
/// ip0        db-index of startpoint on baseCurv (0=undefined)
///            ip0 gives the startpoint even if dir=bwd.
/// ip1        db-index of endpoint on baseCurv (0=undefined)
/// dir        0=forward, curve along ascending parameters;  See INF_struct_dir
///            1=backward, reverse; curve along descending parameters.
/// clo        closed; 0=yes, 1=not_closed; -1=undefined; see INF_struct_closed
//  trm        trimmed; 0=yes, 1=not_trimmed, -1=undef; see INF_struct_closed
/// stat       0=uninitialized; 1=baseCurvePRCV-exists;
/// \endcode
typedef struct {double v0, v1; long dbi, ip0, ip1; 
                unsigned short is0, is1; 
                short typ, us1; char dir, clo, trm, stat;}          CurvCCV;
// size = 56 (4 free)



/*
/// \brief Concatenated curve      CurvAssy        Typ_CurvAssy
/// \code
/// dbi        database index trimmed-curve (CurvCCV)
/// rev        direction reverse; 0=not, 1=reverse
/// clo        closed; 0=not, 1=yes-contour-is-closed
/// \endcode
typedef struct {long dbi;
                unsigned char rev, clo, uu1, uu2;}                  CurvAssy;
*/


// // position-vector,2D-Plane   Typ_VCP2
// typedef struct {Point2 po; Vector2 vx;}                             VCP2;
// // get vy with UT2D_vc_rot_90_ccw()
// // size = 32


/// \brief 3D-plane, Typ_PLN
/// \code
/// (pln.vz.dx * p.x) + (pln.vz.dy * p.y) + (pln.vz.dz * p.z) + pln.p = 0
/// p      normal-distance of plane from 0,0,0
/// \endcode
typedef struct {Point po; Vector vx, vy, vz; double p;}             Plane;
// size = 112
/// bpi    backplane-index; BCKPLN_UNDEF|BCKPLN_FREE|BCKPLN_XY ..
/// bpv    longest vector of plane


/// \brief Typ_Refsys
/// \code
/// mat1   from Plane; does eg 2D->3D
/// mat2   inverted mat1; does eg 3D->2D
/// bpi    backplane-index; BCKPLN_UNDEF|BCKPLN_FREE|BCKPLN_XY ..
/// bpv    longest vector of plane
/// bpd    distance from origint to backplane
/// \endcode
typedef struct {Plane *pln; Mat_4x3 mat1, mat2; double bpd;
                char bpi, bpv, uu3, uu4;}                           Refsys;
// size = 216



/// \brief Hatch  Typ_SURHAT
/// \code
/// off = offset of hatchlines
/// dir = direction in degree
/// \endcode
typedef struct {long cvID; float off, dir; short cvTyp;}            SurHat;


/// \brief RevolvedSurf Typ_SURRV
/// \code
/// indCen  DB-index Centerline
/// typCen  typ of Centerline (Typ_LN)
/// indCov  DB-index contour(-cover)element
/// typCov  typ of contourelement (Typ_CI (circle for torus) or Typ_LN for cone)
/// ang1    starting angle in rad (0 deg = startingpoint = contour at v0)
/// ang2    ending angle in rad
/// v0,v1   start/end-parameters of contour; for all types of contour 0-1
/// dir     direction; 0=CCW, 1=CW.
/// \endcode
typedef struct {double ang1, ang2, v0, v1;
                long indCen, indCov;  short typCen, typCov;
                unsigned dir:1;}                                    SurRev;
// the startPoint of the contourelement defines angle = 0 rad.


/// \brief SweepSurf Typ_SURSWP
/// \code
/// indPath  DB-index path
/// typPath  typ of path (Typ_LN ..)
/// indCov   DB-index contour(-cover)element
/// typCov   typ of contourelement (Typ_CI ..)
/// u0,u1    start/end-parameters of path; for all types of contour 0-1
/// v0,v1    start/end-parameters of contour; for all types of contour 0-1
/// dir      direction; 0=CCW, 1=CW.
/// \endcode
typedef struct {double u0, u1, v0, v1;
                long indPath, indCov;  short typPath, typCov;
                unsigned dir:1;}                                    SurSwp;


/// \brief StandardSurf (planar, conic, toroid)  Typ_SUR
/// \code
/// typ = Typ_SURPLN (planar), Typ_SURCON (Conus), Typ_SURTOR (Torus)
/// base = basic body (Conus, Torus, Sur-RU, SurBSP ...)
/// cv   = limitcurve (CI, CCV ..)
/// \endcode
typedef struct {long baseID, cvID;
                unsigned typ:8, cvTyp:8;}                           SurStd;


/// \brief surface from group of stripes; stripe from 2 polygons.
/// \code
/// Area: 1-n Stripes;
/// ptUNr   nr of points per stripe
/// ptVNr   nr of stripes
/// pTab    table of (ptUNr * ptVNr) points
/// 1. stripe: pt[0]-pt[ptUNr-1], pt[ptUNr]-pt[ptUNr*2-1]
/// 2. stripe: pt[ptUNr]-pt[ptUNr*2-1], pt[ptUNr*2]-pt[ptUNr*3-1]
/// \endcode
typedef struct {int ptUNr, ptVNr; ObjGX *pTab;}                     SurStripe;


/// \brief Area: B-Spline-Surf   Typ_SURBSP
/// \code
/// cpTab[ptUNr*ptVNr]    u0v0,u1v0..uUv0;u1v0,u1v1..uUv1;..;uUv0,uUv1..uUvV;
/// kvTabU[ptUNr+degU+1]
/// kvTabV[ptVNr+degV+1]
/// \endcode
typedef struct {int ptUNr, ptVNr, degU, degV;
                double v0U, v1U, v0V, v1V, *kvTabU, *kvTabV;
                Point *cpTab;}                                      SurBSpl;
// size = 60


/// \brief Area: Rat.B-Spline-Surf   Typ_SURRBSP
/// \code
/// cpTab[ptUNr*ptVNr]    u0v0,u1v0..uUv0;u1v0,u1v1..uUv1;..;uUv0,uUv1..uUvV;
/// kvTabU[ptUNr+degU+1]
/// kvTabV[ptVNr+degV+1]
/// wTab[ptUNr*ptVNr]
/// \endcode
typedef struct {int ptUNr, ptVNr, degU, degV;
                double v0U, v1U, v0V, v1V, *kvTabU, *kvTabV, *wTab;
                Point *cpTab;}                                      SurRBSpl;
// size = 64



/// \brief bounding-box 2D
/// \code
/// pb1     lower left boxPoint
/// pb2     upper right boxPoint
/// \endcode
typedef struct {Point2 pb1, pb2;}                                   BBox2;     
// size = 32


/// \brief bounding-box 3D
/// \code
/// ind     index of first obj
/// oNr     nr of obj's; index of last obj is (ind + nr - 1)
/// \endcode
typedef struct {Point pb1, pb2;}                                    BBox;   
// size = 48



/// \brief color, Typ_Color
/// \code
/// vtra   view transparent; 0=not, 1=25% transparent, 2=50%, 3=75%
/// vtex   has texture; 0=not, 1=yes; (cr+cg+cb) = textureRefNr
/// vsym   view normal (0,shaded) or symbolic (1,wireframeboundary)
/// color  0=color not active (use AP_defcol); 1=color from cr/cg/cb
/// cr,cg,cb  red, green, blue; 0-255
/// Grafic-Attribute for surface, solid (not line, curve - see Att_ln).
/// \endcode
typedef struct {unsigned cr:8, cg:8, cb:8,
                vtra:2,vsym:1,vtex:1,color:1, hili:1,dim:1,UU:1;}   ColRGB;
// size = 4


/// \brief grafic text; Typ_GTXT
/// \code
/// pt           lower left position
/// size         textheight in usercoords; default = 0.; 2D-text: -1.
/// dir          direction in degree
/// xSiz, ySiz   max_nr_of_chars, nr_of_lines
/// \endcode
typedef struct {Point pt; float size, dir; char *txt;
                 short xSiz, ySiz;}                                 GText;
// size = 36


/// \brief text or image or tag or label; Typ_ATXT, Typ_Tag
/// \code
/// p1   Textblock-/Imageposition (left middle)
/// p2   Startpoint leaderline
/// aTyp 0=Text, 1=Image, 2=Tag, 3=Balloon  4=PointCoord
///      5=Symbol SYM_STAR_S (Stern klein) 6=Symbol SYM_TRI_S (Dreieck klein)
///      7=Symbol SYM_CIR_S (Kreis klein)  8=SYM_SQU_B (Viereck)
///      9=Vector (normalized)            10=Vector (true length)
///     11=Arrowhead (normalized)
/// txt  Text or ImageFilename; none: noteindex
/// scl          Image: Scale
/// xSiz, ySiz   Image, Tag: size of image / tag in pixels
///              Balloon: xSiz=stringLength
/// col  color of Label; -1=no Label   see INF_COL_SYMB
/// ltyp Linetyp Leaderline; -1=no Leaderline.
/// \endcode
typedef struct {Point p1, p2; char *txt; short xSiz, ySiz;
                float scl; char aTyp, col, ltyp, UU;}               AText;


/// \brief basic texture description; Typ_TEXB
/// \code
/// dli          DispListindex of the Texture UNUSED
/// texNr        (OpenGL-) TextureNr
/// keep         0=do not keep texture; 1=keep.
/// fnam         Filename (symbolicPath/Filename) of the Texturefile
/// xSiz, ySiz   size of the Texure in pixels
/// \endcode
typedef struct {char *fnam;
                short xSiz, ySiz, texNr, keep;}                     TexBas;


/// \brief texture reference; Typ_TEXR
/// \code
/// ibas         index of the Base-Texure
/// uscx uscy    User-defined-scale in X- and Y-direction;
/// udx  udy     User-defined-offset of texture; 0-1
/// uar          User-defined-rotationAngle for texture in degree
/// ssx ssy      surfaceSize  in X- and Y-direction;
/// px,py,pz     origin of surfac
/// vx, vy       X- and Y-vector of textureplane
/// \endcode
typedef struct {int ibas; float uscx, uscy, udx, udy, uar,
                ssx, ssy, px, py, pz, fx, fy; Vector vx, vy;}       TexRef;



/// internal submodel (block); Typ_Ditto
typedef struct {Point po; long ind, siz;}                           Ditto;


/* UNUSED
/// \brief internal submodel reference; Typ_Mock
/// \code
/// ind   DL-startindex of ditto
/// siz   nr of DL-objects of ditto
/// po    position of ditto
/// \endcode
typedef struct {char *mnam; long ind, siz;
                Point po; Vector vx, vz;}                           ModelMock;
*/


/// \brief basic model description; Typ_SubModel
/// \code
/// mnam  .. Modelname
/// pb1,pb2  boxPoints
/// DLind .. ind of first obj in DL
/// DLsiz .. nr of objects in DL
/// typ   .. MBTYP_[EXTERN|INTERN|CATALOG] for native-subModels
///          Mtyp_[WRL|WRL2|OBJ|STL|TESS]  for mockup-subModels
/// \endcode
typedef struct {char *mnam; Point pb1, pb2;
                long DLind; int DLsiz;
                short typ, uu;}                       ModelBas;
// 56


/// \brief model reference; Typ_Model
/// \code
/// modNr .. modelnumber of ModelBas-obj. (DB_mbi_mRefID())
/// po    .. position of ditto
/// \endcode
typedef struct {int modNr; double scl;
                Point po; Vector vx, vz;}                           ModelRef;


/// \brief sphere; Typ_SPH
/// \code
/// pc     ... center
/// rad    ... radius
/// \endcode
typedef struct {Point pc; double rad;}                              Sphere;
// siz = 32


/// \brief cone; Typ_CON
/// \code
/// pl.po ... center of circle 1
/// pl.vx ... vec -> startpoint of circle 1 (radius = r1)
/// pl.vz ... axis
/// h     ... distance circle 1 - circle 2
/// \endcode
typedef struct {Plane pl; double r1, r2, h;}                        Conus;
// siz = 128


/// \brief torus; Typ_TOR
/// \code
/// pl.po ... center
/// pl.vz ... axis
/// pl.vx ... vec -> startpoint of major circle (radius = r1-r2)
/// r1    ... radius of outermost circle !!
/// r2    ... radius of torusring
/// \endcode
typedef struct {Plane pl; double r1, r2;}                           Torus;
// siz = 120


// Typ_REV


/// \brief dimension; Typ_Dimen
/// \code
/// p1    dimensionpoint 1
/// p2    dimensionpoint 2
/// p3    textpoint
/// dtyp: 0=linear 1=diameter 2=radius 3=angle 21=leader
/// hd:   heads: 0=none, 1=<, 2=>, 3=/, 4=X; Default is 12.
/// ld    lines: 0=none, 1=line; Default is 11.
/// a1    angle (linear:textline; angle: line1; leader:text)
/// a2    angle (angle: line2)
/// \endcode
typedef struct {Point2 p1, p2, p3; float a1, a2;
			          char dtyp, hd, ld, uu; char *txt;}                  Dimen;


/// \brief 3D-dimension; Typ_Dim3
/// \code
/// dtyp: X Y Z blank=parallel
/// bp    Backplane  0=BCKPLN_YZ; 1=BCKPLN_XZ; 2=BCKPLN_XY
/// \endcode
typedef struct {long ip1, ip2, ipt;
                char dtyp, hd, ld, bp; char *txt;}                  Dim3;


/// \brief 2D-symbol; Typ_SymRef2 Typ_SymB Typ_SymV
/// \code
/// ang        angle in rad
/// \endcode
typedef struct {Point2 pt; float ang, scl; int typ;}                SymRef2;
// GL_DrawSymV2



/// \brief 3D-symbol; Typ_SymRef
typedef struct {Point pt; Vector dx, dy; float scl; int typ;}       SymRef;
// GL_DrawSymV3




//....................... Geom. Attributes: ..................................

/// \brief  grafic attribute; Typ_G_Att
/// \code
/// dash       0 = full-line (VollLinie);    1 = dash-dot (Strich-Punkt),
///            2 = dashed (kurz strichliert) 3 = dashed-long (lang strichliert),
/// thick      line-thickness in pixels; 1-6
/// cr cg cb   color red, green, blue;  0-9  (digit; not hex-char)
/// used for line,circle,curve,text
/// \endcode
typedef struct {unsigned dash:8, thick:8,
                         cr:4, cg:4, cb:4, uu:4;}                   Att_ln;

/// \brief  grafic attribute for lines, curves; Typ_Ltyp
/// \code
/// indAtt     LineTypNr (color/pattern/thick in file cfg/ltyp.rc)
/// lim        sides unlimited: 0 = both sides; 1 = side one; 2 = sides two;
/// uu         UNUSED
/// Grafic-Attribute for line,circle,curve (not surface, solid - see ColRGB)
/// \endcode
typedef struct {short indAtt; char lim, uu;}                        Ind_Att_ln;

// DO NOT USE:
// typedef struct {unsigned  col:8, ltyp:8, lthick:8, 
                          // unused:7, used:1;}                        GR_Att;
// size = 4


/// \brief GraficAttribute
/// \code
/// typ    DB-Typ
/// ind    DB-Index
/// iatt   for typ=LN/AC/Curve: Ind_Att_ln
///        for typ=TAG/IMG: sx/sy
///        for typ=Surf/Model: ColRGB
/// disp   0=normal; 1=hidden
/// oNam   objectname; not yet impl.
/// \endcode
typedef struct {long ind;
                UINT_32 iatt;
                unsigned lay:16, typ:8,
                         unused:7, disp:1;}                         ObjAtt;
// size = 12


/// \brief DisplayListRecord  _DL_ATT_NUL 
/// \code
/// lNr    APTlineNr.
/// typ    DB-Typ
/// ind    DB-Index, typ = DB-Typ
/// irs    index RefSys  (can be int; 
/// modInd index baseModel of owning model
/// iatt   for typ=LN/AC/Curve: LineTypNr.
///        for typ=TAG/IMG: sx/sy
///        for typ=Surf/Model: ColRGB
///        in Parent-state DL-index of child
/// disp, hili:
///        normal = ((disp == 0)&&(hili == 1))
///        hilite = ((disp == 1)&&(hili == 0))
///        hidden = ((disp == 1)&&(hili == 1))
/// pick   0=unpickable, 1=pickable
/// dim    0=dimmed,     1=normal
/// grp_1  0=belongs to active Group, 1=not
/// unvis  0=visible; 1=obj does not have graph. representation
/// sChd   0=independent; 1=Child-state is active; obj is child - has parent(s)
/// sPar   0=independent; 1=Parent-state is active; obj is parent; has child(s)
/// \endcode
typedef struct {long ind, lNr;
                INT_32 irs; UINT_32 iatt;
                short modInd, typ;
                unsigned disp:1,  hili:1,  pick:1,  dim:1,
                         grp_1:1, unvis:1, sChd:1,  sPar:1,
                         UU:24;}                                    DL_Att;
// size = 32


/// \brief Typ_TraRot:
/// \code
/// rotationOrigin = ma[0][3], ma[1][3], ma[2][3].
/// vz = rotationAxis
/// angr = rotationAngle (rad)
/// \endcode
typedef struct {Mat_4x3 ma; Vector vz; double angr;}                TraRot;
// size = 128


/// \brief Typ_Activ ("I")
/// \code
/// typ,ind     Activity connected to this obj
/// data        commandText
/// \endcode
typedef struct {int typ; long ind; char *data;}                     Activity;


/// \brief Typ_GridBox _GRIDBOX_NUL
/// \code
/// horizontal/vertical gridBox
/// pMin          position of lower left startpoint (xMin, yMin, zMin)
/// dx, dy, dz    size of a gridsection
/// ix, iy, iz    nr of points in x-direction, y-direction, z-direction
/// \endcode
typedef struct {Point pMin; int ix, iy, iz; double dx, dy, dz;}     GridBox;



/*
//....................... Tess. Objects: ..................................

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
/// fTyp = 3: Face/TriangMesh; fTab = *Face; Face->ipt = int[3];
/// fTyp = 4: Face/QuadMesh;
/// fTyp = 5: Fac3/tableOfTriangIndices; fTab = *int; iTab[fNr][3];
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
///  typ    MSH_EDGLN_BL   2 EdgeLine (BreakLine)
///         MSH_EDGLN_IB   3 InnerBound
///         MSH_EDGLN_OB   4 OuterBound
///         MSHIG_EDGLN_AB   5 OuterBound - automatic created
///         MSH_PATCH  6 faces (eg from GLU)
///  aux    for MSH_PATCH: GL-typ; GL_TRIANGLE_STRIP|GL_TRIANGLE_FAN|..
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


// /// \brief Typ_EdgSur
// /// \code
// /// vxSt    index to startPoint
// /// vxNr    nr of points following startPoint
// /// surNb   surfaceIndex of neighbourSurface (surf-record in BODY)
// /// see also EdgeLine
// /// \endcode
// typedef struct {int vxSt, vxNr, vxMax, surNb;}                      EdgSur;



/// \brief Typ_SURBND
/// \code
/// suID    surface-ID (DB-index A)
/// contNr  contour-nr; first1=1, ..
/// typb    MSH_EDGLN_BL   2 EdgeLine (BreakLine)
///         MSH_EDGLN_IB   3 InnerBound
///         MSH_EDGLN_OB   4 OuterBound
///         MSHIG_EDGLN_AB   5 OuterBound - automatic created
///         MSH_PATCH  6 faces (eg from GLU)
/// typt    Typ_SURCIR   (fan)
///         Typ_SURSTRIP (strip)
/// dir     0=undefined; 1=CCW; -1=CW
/// \endcode
typedef struct {int suID, contNr; char typb, typt, dir, stat;}      BndSur;
//                                                                  ^
*/



//.........................................................................

//----------------------------------------------------------------
#include "../ut/ut_const.h"
/*
extern const double UT_VAL_MIN;
extern const double UT_VAL_MAX;
extern const double UT_DB_LEER;
extern const double UT_DB_NULL;
extern const int    UT_INT_MAX;
extern const char   UT_CHR_NULL;
*/



//----------------------------------------------------------------
#include "../ut/ut_tol_const.h"
/*
// constants:
extern double UT_TOL_min0;
extern double UT_TOL_min1;
extern double UT_TOL_min2;
/// tolerance for parallelism
extern double UT_TOL_PAR;
/// tolerance for Angle 0.01 degree
extern double UT_TOL_Ang1;
/// tolerance for Angle 0.1 degree
extern double UT_TOL_Ang2;

// tolerances depending from Modelsize:
/// identical Points-tolerence
extern double UT_TOL_pt;
/// max deviation from curve  (distance analyticalCurve -> Curvepoint)
extern double UT_TOL_cv;
/// max deviation of a point from a line
extern double UT_TOL_ln;
/// max deviation from curve  (distance analyticalCurve -> displayPolygon)
extern double UT_DISP_cv;
/// length of construction-lines
extern double UT_DISP_ln;
*/







//----------------------------------------------------------------
#include "../ut/ut_geo_const.h"
/*
extern const Point2  UT2D_PT_NUL;
extern const Point   UT3D_PT_NUL;

extern const Point2 UT2D_PT_INFTY;

extern const Vector2 UT2D_VECTOR_NUL;
extern const Vector2 UT2D_VECTOR_X;
extern const Vector2 UT2D_VECTOR_Y;

extern const Vector2 UT2D_VECTOR_IX;
extern const Vector2 UT2D_VECTOR_IY;


extern const Vector  UT3D_VECTOR_NUL;

extern const Vector  UT3D_VECTOR_X;
extern const Vector  UT3D_VECTOR_Y;
extern const Vector  UT3D_VECTOR_Z;

extern const Vector  UT3D_VECTOR_IX;
extern const Vector  UT3D_VECTOR_IY;
extern const Vector  UT3D_VECTOR_IZ;

extern const Mat_3x3 UT3D_MAT_3x3;
extern const Mat_4x3 UT3D_MAT_4x3;
extern const Mat_4x4 UT3D_MAT_4x4;
*/




//================================================================
// prototypes for gcad_ut_geo.c

//----------------------------------------------------------------
 int    UT1D_ndb_npt_bp (double *da, Point *pa, int pNr, int bp);
 double UT2D_lva_plg (double *lva, Point2 *pta, int ptNr);

 double UT_DEGREES (double);
 double UT_RADIANS (double);

 int    UT2D_sidPerp_3pt (Point2 *p1, Point2 *p2, Point2 *p3);

 int    UT2D_angr_set (double *angr);
 int    UT2D_2angr_set (double *ang1, double *ang2, int irot);
 double UT2D_angr_2angr (double ang1, double ang2, int irot);
 // double UT2D_angr_set_2angr (double as, double aa, int sr);
 int    UT2D_ptNr_ci  (double rdc, double ao, double tol);

 int    UT2D_solvtriri_a (double *a, double b, double c);
 int    UT2D_solvtriri_bc (double *,double *,double,double);
 int    UT2D_solvtri_abc (double *,double *,double,double,double);

 int    UT2D_ckBoxinBox1 (Point2 *p1, Point2 *p2, Point2 *p3, Point2 *p4);
 int    UT2D_ckBoxinBox2 (Point2*, Point2*, Point2*, Point2*, double);

 double UT2D_slen_vc_vc__ (Vector2 *vac, Vector2 *vab);
 double UT2D_slen_vc_vcNo (Vector2 *vac, Vector2 *vnab);
 double UT2D_slen_pt_pt_vc__ (Point2 *pc,  Point2 *pa, Vector2 *vab);
 double UT2D_skp_2vc (Vector2 *, Vector2 *);
 double UT2D_skp_vc2pt (Vector2 *, Point2 *, Point2 *);
 double UT2D_acos_2vc (Vector2 *, Vector2 *);
 double UT2D_sar_2vc (Vector2 *, Vector2 *);
 double UT2D_acos_vc2pt (Vector2 *, Point2 *, Point2 *);
 double UT2D_crossprod_2vc (Vector2 *, Vector2 *);

 double UT2D_angd_angr (double);
 double UT2D_angd_invert (double *andi);
 double UT2D_angr_angd (double);
 double UT2D_angr_triri_ab (double, double);
 double UT2D_angr_ptpt (Point2 *, Point2 *);
 double UT2D_angr_vc   (Vector2 *);
 double UT2D_angr_2ln (Line2 *, Line2 *);
 double UT3D_angr_vc2pt (Point *pz, Vector *vz, Point *p1, Point *p2);
 double UT3D_angr_vcpl_z (Plane *pl1, Vector *vc1);
 double UT3D_angr_vcpl_tilt (Plane *pl1, Vector *vc1);
 double UT3D_angr_vc2vc (Vector *vc1, Vector *vcx, Vector *vcy);
 double UT3D_angr_2pl (Plane* pl1, Plane* pl2);
 double UT2D_angr_2vc_ccw (Vector2 *, Vector2 *);
 double UT2D_angr_2vcn_ccw (Vector2 *, Vector2 *);
 double UT2D_angr_3pt_sr (Point2 *, Point2 *, Point2 *, int);
 double UT3D_angr_4pt (Point *p11, Point *p12, Point *p21, Point *p22);
 double UT2D_angr_ci (Point2 *pa,Point2 *pe,Point2 *pc,double rad_in);
 double UT3D_angr_ci_p1_pt (Circ *ci1, Point *pti);
 double UT2D_angr_perpangr (double*);

 double UT2D_len_vc (Vector2 *);
 double UT2D_lenq_vc (Vector2 *);
 void   UT2D_lenq_2pt (double *lq, Point2 *p1, Point2 *p2);
 double UT2D_len_2pt (Point2 *,Point2 *);
 double UT2D_lenB_2pt (Point2 *p1, Point2 *p2);
 double UT2D_lenS_2pt (Point2 *p1, Point2 *p2);
 double UT2D_lenS_vc  (Vector2 *vc1);
 int    UT2D_lenq_ck_pt_ln (Point2 *pa, Point2 *pb, Point2 *px, double distq);
 int    UT2D_minLenB_4pt (double *dp,Point2*,Point2*,Point2*,Point2*);
 int    UT3D_minLen_3pt (double*,Point*,Point*,Point*);
 int    UT3D_minLen_4pt (double*,Point*,Point*,Point*,Point*);
 int    UT3D_minLen_npt (Point *p0, Point *pa, int pNr);
 double UT2D_len_ptln (Point2 *pt, Point2 *pa, Point2 *pe);
 int    UT2D_3len_ptln (double*, double*, double*, Point2*, Point2*, Point2*);
 int    UT2D_slenq_3pt (double *qlen, Point2 *pa, Point2 *pb, Point2 *pc);
 double UT2D_slen_nor3pt (Point2 *p1, Point2 *p2, Point2 *p3);
 int    UT2D_slen_nor2vc (double *slen, Vector2 *v1, Vector2 *v2);
 int    UT2D_slenq_nor_2vc (double *slen, Vector2 *v1, Vector2 *v2);
 double UT2D_slen_nor_2pt_vc__ (Point2 *pt,  Point2 *pl, Vector2 *vl);
 double UT2D_slen_nor_2pt_vcNo (Vector2 *v1, Point2 *p1, Point2 *p2);
 int    UT2D_2slen_vc_vc__ (double *dx, double *dy, Vector2 *vp, Vector2 *vl);
 int    UT2D_2slen_2pt_vc__ (double *dx, double *dy,
                             Point2 *px, Point2 *pl, Vector2 *vl);

 // int    UT2D_sid_2vc (Vector2 *v1, Vector2 *v2, double tol);  // DO NOT USE
 int    UT2D_sid_2vc__ (Vector2 *v1, Vector2 *v2);
 int    UT2D_sid_2vc_tol (Vector2 *v1, Vector2 *v2, double *tol);
 // int    UT2D_sid_ptvc__ (Point2*, Point2*, Vector2*);            // DO NOT USE
 int    UT2D_sid_ptvc____ (Point2*, Point2*, Vector2*);
 int    UT2D_sid_ptvc___tol (Point2*, Point2*, Vector2*, double*);
 int    UT2D_sid_3pt (Point2 *pt,  Point2 *p1, Point2 *p2);

 int    UT2D_sidPerp_2vc (Vector *v1, Vector *v2);
 int    UT2D_sidPerp_ptvc (Point2 *pt,  Point2 *pl, Vector2 *vl);
 int    UT2D_sidPerp_3pt (Point2 *p1, Point2 *p2, Point2 *p3);

 int    UT3D_sid_2vc (Vector *v1, Vector *v2);
 int    UT3D_sid_3pt (Point *p1, Point *p2, Point *p3);
 int    UT3D_sid_ptpl (Point *pt, Plane *pl);
 int    UT3D_sid_ptptvc (Point *ptx, Point *pto, Vector *vz);

 int    UT2D_parLn_pt2pt (double *d1, Point2 *p1, Point2 *p2, Point2 *px);
 double UT2D_par_nor_2vc (Vector2 *vc1, Vector2 *vc2);

 double UT2D_ar_3pt (Point2 *p1, Point2 *p2, Point2 *p3);

 int    UT2D_comp2pt (Point2*, Point2*, double);
 int    UT2D_comp4pt (Point2*, Point2*, Point2*, Point2*, double);
 int    UT2D_pt_ck_int4pt (Point2*, Point2*, Point2*, Point2*);
 int    UT2D_pt_ck_onLine (Point2 *po,
                           Point2 *p1,Point2 *p2,Point2 *p3,double tol);
 int    UT2D_pt_ck_inLine (Point2 *p1, Point2 *p2, Point2 *p3, double tol);
 int    UT2D_pt_ck_inplg (Point2 * pTab, int pNr, Point2 *ptx, int iClo);
 int    UT2D_pt_ck_linear (int np, Point2 *ptab, double tol);
 int    UT2D_pt_ck_inBox (Point2 *p1, Point2 *p2, Point2 *p);
 int    UT2D_pt_ck_inBoxTol (Point2 *p1, Point2 *p2, Point2 *p, double tol);
 int    UT2D_pt_ck_inAc (Point2 *pt, Circ2 *ci);
 int    UT2D_pt_ck_inTriang (Point2 *p1, Point2 *p2, Point2 *p3, Point2 *p);
 int    UT2D_pt_ck_inCv3 (Point *ptx, int pNr, Point *pTab);
 int    UT2D_pt_cknear_npt (Point2 *p0, Point2 *ptTab, int ptNr);

 void   UT2D_swap2pt (Point2 *p1, Point2 *p2);
 Point2 UT2D_pt_pt3 (Point *);
 void   UT2D_pt_2db (Point2 *, double, double);
 void   UT3D_pt_tra_pt_bp (Point*,Point*,int);
 int    UT3D_pt_tra_pt2_bp (Point *p3, Point2 *p2, int bp, double *bph);
 void   UT2D_pt_addpt (Point2 *, Point2 *);
 void   UT2D_pt_add_vc__ (Point2 *, Vector *);
 void   UT2D_pt_sub_pt3 (Point2 *, Point *);
 void   UT2D_pt_opp2pt (Point2 *, Point2 *, Point2 *);
 void   UT2D_pt_mid2pt (Point2 *, Point2 *, Point2 *);
 void   UT2D_pt_traptvc (Point2 *, Point2 *, Vector2 *);
 void   UT2D_pt_tra2ptlen (Point2 *, Point2 *, Point2 *, double);
 void   UT2D_pt_tra_pt_pt_par (Point2 *po, Point2 *p1, Point2 *p2, double d1);
 void   UT2D_pt_tra3ptlen (Point2 *, Point2 *, Point2 *, Point2 *,double);
 void   UT2D_pt_tra2pt2len (Point2 *,Point2 *,Point2 *,double,double);
 void   UT2D_pt_tra2pt3len (Point2 *,Point2 *,Point2 *,double,double,double);
 void   UT2D_pt_traptvclen (Point2 *,Point2 *,Vector2 *,double);
 void   UT2D_pt_traptangrlen (Point2 *,Point2 *,double,double);
 void   UT2D_pt_tranor2ptlen (Point2 *,Point2 *,Point2 *,double);
 void   UT2D_pt_tranorptvclen (Point2 *, Point2 *, Vector2 *, double);
 void   UT2D_pt_traptvc2len (Point2 *,Point2 *,Vector2 *,double,double);
 void   UT3D_ln_tra_ln_m3 (Line *lno, Mat_4x3 trmat, Line *lni);
 void   UT2D_pt_tra_pt_ci_len (Point2 *, Point2 *, Point2 *, double, double);
 void   UT2D_pt_rotptangr (Point2*,Point2*,Point2*,double);
 int    UT2D_pt_projptptvc (Point2 *, Point2 *, Point2 *, Vector2 *);
 int    UT2D_pt_projpt2pt(Point2 *pp,  double *len,
                        Point2 *pt, Point2 *p1, Point2 *p2);
 int    UT2D_pt_projptln (Point2 *, Point2 *, Line2 *);
 void   UT2D_2pt_projptci (Point2*,Point2*,Point2*,Point2*,double);
 int    UT2D_pt_projptci (Point2 *pp, Point2 *pt, Circ2 *ci1);
 int    UT3D_ptDi_intptvcptvc (Point *ip, double *dist, Point *ptl, Vector *vcl,
                               Point *ptp, Vector *vcp);
 int    UT3D_ptDi_intptvcpln (Point *ip, double *dist,
                              Plane *pl, Point *pt, Vector *vln);
 int    UT2D_pt_int_4pt (Point2 *px, double *tol,
                         Point2 *pa, Point2 *pb,
                         Point2 *pc, Point2 *pd);
 int    UT2D_pt_int4pt (Point2*,double*,double*,double*,
                        Point2*,Point2*,Point2*,Point2*);
 int    UT2D_pt_int2ln (Point2*, Line2*, Line2*);
 int    UT2D_pt_intptvcy (Point2 *pto, Point2 *ptl, Vector2 *vcl, double yVal);
 int    UT2D_pt_intlny (Point2 *pto, Point2 *lp1, Point2 *lp2, double yVal,
                        double tol);
 int    UT2D_pt_intlnx (Point2 *pto, Point2 *lp1, Point2 *lp2, double xVal,
                        double tol);
 int    UT2D_pt_int2pt2vc (Point2 *ip, Point2 *pt1, Vector2 *vc1,
                                     Point2 *pt2, Vector2 *vc2);
 int    UT2D_pt_int2vc2pt (Point2 *, Point2 *, Vector2 *, Point2 *, Vector2 *);
 int    UT2D_2pt_intciptvc (Point2*,Point2*,Point2*,double,Point2*,Vector2*);
 int    UT2D_2pt_intlnci (Point2 *ip1, Point2 *ip2,
                          Line2 *ln, int lnMode, Circ2 *ci, int ciMode);
 int    UT2D_2pt_intcici (Point2*,Point2*,Point2*,double,Point2*,double);
 int    UT2D_2pt_int2ci (Point2 *ip1, Point2 *ip2,
                         Circ2 *ci1, int ci1Mode, Circ2 *ci2, int ci2Mode);
 void   UT2D_pt_traptm2 (Point2 *, Mat_3x2, Point2 *);
 Point2 UT2D_pt_obj2 (ObjG2 *obj1);

 void   UT2D_ln_ptpt (Line2*, Point2*, Point2*);
 void   UT2D_ln_ptvc (Line2*, Point2*, Vector2*);
 void   UT2D_ln_ln3__ (Line2 *ln2, Line *ln3);
 void   UT2D_ln_inv (Line2 *ln1);
 void   UT2D_ln_4db (Line2 *ln2, double xs, double ys, double xe, double ye);
 Line2  UT2D_ln_obj2 (ObjG2*);
 int    UT2D_lncoe_ln (double *k, double *d, Line2 *ln);
 int    UT2D_ln_pts_dmax (Line2 *ln, int np, Point2 *ptab, double tol);


 int    UT2D_ck_parla_2vc (Vector2*, Vector2*, double);
 int    UT2D_ck_parl_2vc (Vector2*, Vector2*, double);
 int    UT2D_ckvc_in2vc (Vector2 *v1, Vector2 *v2, Vector2 *v3);
 void   UT2D_vc_vc3 (Vector2*, Vector*);
 int    UT2D_2parvc_3vc(double*,double*,Vector2*,Vector2*,Vector2*);
 void   UT2D_vc_2db (Vector2 *, double, double);
 void   UT2D_vc_angr (Vector2 *, double);
 void   UT2D_vc_angrlen (Vector2 *, double, double);
 void   UT2D_vc_2pt (Vector2 *, Point2 *, Point2 *);
 void   UT2D_vc_2pt3 (Vector2 *vc, Point *p1, Point *p2);
 int    UT2D_vc_2pt3_bp (Vector2 *vo, Point *p1, Point *p2, int bp);
 void   UT2D_vc_2ptlen (Vector2 *, Point2 *, Point2 *, double);
 void   UT2D_vc_ln  (Vector2 *, Line2 *);
 void   UT2D_vc_invert (Vector2 *, Vector2 *);
 int    UT3D_vc_Zup (Vector *v2, Vector *v1);
 void   UT2D_vc_rot_90_ccw (Vector2 *, Vector2 *);
 void   UT2D_vc_rot_90_cw (Vector2 *, Vector2 *);
 void   UT2D_vc_perp2pt (Vector2 *, Point2 *, Point2 *);
 void   UT2D_vc_normalize (Vector2*,Vector2*);
 void   UT2D_vc_setLength (Vector2 *, Vector2 *, double);
 void   UT2D_vc_mid_2vc__ (Vector2*,Vector2*,Vector2*);
 void   UT2D_vc_merge2vc (Vector2 *vm, Vector2 *v1, Vector2 *v2);
 void   UT2D_vc_rotangr (Vector2 *,Vector2 *,double);
 void   UT2D_vc_travcm2 (Vector2 *vo, Mat_3x2 mata, Vector2 *vi);

 double UT2D_angr_ciSec (double hc, double radc);
 double UT2D_len_ciSec (double hSec, double rCi);
 double UT2D_len_cia (double rad, double ango);
 double UT2D_len_cir (double *angOpe,
                       Point2 *pa,Point2 *pe,Point2 *pc,double rad_in);
 int    UT2D_compPtOnAc (double, double, double, int);
 Circ2  UT2D_ci_obj2 (ObjG2 *);
 int    UT2D_ci_ptrd (Circ2 *ci, Point2 *ptc, double rdc);
 int    UT2D_ci_ci3 (Circ2 *ci2, Circ *ci3);
 int    UT2D_ci_2vc2ptrd (Point2*,Point2*,Vector2*,Point2*,Vector2*,double);
 int    UT2D_ci_ciptvcrd (Point2*,Point2*,double,int,Point2*,Vector2*,double);
 int    UT2D_ci_ptvcpt (Point2 *,double *,Point2 *,Vector2 *,Point2 *);

 int    UT2D_obj_obj3 (ObjGX *oo, ObjGX *oi, Memspc *memSeg);

 // ObjG2  UT2D_obj_pt3 (Point*);
 // ObjG2  UT2D_obj_ln3 (Line*);
 ObjG2  UT2D_obj_ci2 (Circ2*);
 // ObjG2  UT2D_obj_ci3 (Circ*);
 // ObjG2  UT2D_obj_cv3 (Curv *);

 int    UT2D_void_obj2 (void *memObj, unsigned long *oSiz, ObjG2 *og2);

 void   UT2D_cv_ci360 (Point2 *cv, int ptNr, double rd, Point2 *ptCen);
 int    UT2D_cv_ln (Point2 *cv, int ptAnz, Point2 *p1, Point2 *p2);
 int    UT2D_npt_ci (Point2 *pa, int pNr, Circ2 *ci1);
 int    UT2D_cv_ci (Point2[],int*,Point2*,Point2*,Point2*,int);
 int    UT2D_cv3_linear (int *pNr, Point *pTab, double tol);
 int    UT2D_sr_npt (double *aro, int ptNr, Point2 *pa);


void   UT2D_m2_load (Mat_3x2,  Vector2 *, Point2 *);
void   UT2D_m2_loadtravcm2 (Mat_3x2 ma, Vector2 *vx, Vector2 *vy, Vector2 *vt);
int    UT2D_m2_invtravcm2 (Mat_3x2 im1, Mat_3x2 m1);
void   UT2D_m2_init_rot (Mat_3x2 ma, double angle, Point2 *cen);


/*===============================================================*/


int    DEB_dump_obj__ (int typ, void *data, char *txt, ...);


double UT3D_len_vc (Vector *);
double UT3D_lenq_vc (Vector *);
int    UT3D_bplen_vc (double *lnv, Vector *vc1);
double UT3D_lenB_vc (Vector *vc1);
double UT3D_lenB_2pt (Point *p1, Point *p2);
double UT3D_len_2pt (Point*,Point*);
double UT3D_lenq_PtPt (Point *p1, Point *p2);
int    UT3D_lenq_PtLn (double *lenq, Point *p, Point *p1, Point *p2);
double UT3D_len_ln (Line *);
double UT3D_len_ci (Circ *ci1);
double UT3D_slen_3pt (Point *pt, Point *p1, Point *p2);
double UT3D_slen_projvcvc (Vector *vc1, Vector *vc2);
double UT3D_slenq_projvcvc (Vector *vc1, Vector *vc2);
double UT3D_slenq_2ptvc (Point *p1, Point *p2, Vector *vc);
double UT3D_nlen_projvcvc (Vector *vc1, Vector *vc2);
double UT3D_slen_2ptvc (Point *p1, Point *p2, Vector *vc);
double UT3D_slen_ptpl (Point *pt, Plane *pl);
double UT3D_nlen_2ptvc (Point *p1, Point *pv, Vector *vc);
double UT3D_nlen_3pt (Point *p1, Point *p2, Point *p3);
int    UT3D_par_pt_2pt (double *pl, Point *ptx, Point *pl1, Point *pl2);
int    UT3D_parpt_ptvc (double *pl, Point *ptx, Point *pt1, Vector *vc1);
double UT3D_parpt_lnbp (Point *pti, Line *ln1, int bp);
double UT3D_par1_ci_angr (Circ *ci1, double angr);
double UT3D_par1_ci_pt   (Circ *ci1, double *dpc, Point *pt1);
int    UT3D_par_pt__pt_prj_ci (double *par, Point *pto, double *dpc,
                               Circ *ci1, Point *pt1, int mode, double tol);

void   UT3D_pt_setFree (Point*);
int    UT3D_pt_isFree (Point*);
int    UT3D_pt_ck_npt (Point *p0, Point *pTab, int pNr, double tol);
int    UT3D_ipt_cknear_npt (Point *p0, Point *ptTab, int ptNr);
int    UT3D_ipt_ckfar_npt (Point *p0, Point *ptTab, int ptNr);
int    UT3D_ipt_cknearn_npt (Point *p0, Point *ptTab, int ptNr, int distNr);
int    UT3D_ipt2_nptvc (Point *pt1, Point *pt2, int mode,
                        Point *pta, int iNr, Vector *vc);
int    UT3D_pt_ck_onel (Point *pt, CurvElli *el);
int    UT3D_pt_ck_ptvc (Point *pt, Point *pl, Vector *vl, double tol);
int    UT3D_pt_ck_2pt (Point *pt, Point *pl1, Point *pl2, double tol);
int    UT3D_pt_ck_onLine (Point *p1, Point *p2, Point *p3, double tol);
int    UT3D_pt_ck_inLine (Point *p1, Point *p2, Point *p3, double tol);
int    UT3D_pt_ck_mid_2pt (Point *p1, Point *p2, Point *p3, double *tol);
int    UT3D_pt_ck_on_pta (Point *pTab, int ptNr, Point *pt1, double tol);
int    UT3D_pt_ck_in2pt (Point *p1,Point *p2,Point *p3, double tol);
int    UT3D_pt_ck_inCirc (Circ *ci1, Point *pt1, double tol);
int    UT3D_pt_ck_inSph (Point *pt, Point *ps, double rs);

void   UT3D_swap2pt (Point *p1, Point *p2);
int    UT3D_comp4pt (Point *p1a,Point *p1e,Point *p2a,Point *p2e,double tol);
int    UT3D_compptpl (Point*, Plane*);
Point  UT3D_pt_pt2 (Point2 *);
void   UT3D_pt_pt2_0 (Point *, Point2 *);                            // INLINE
Point  UT3D_pt_pt2z (Point2 *pt20, double zVal);
void   UT3D_pt_pt2_z (Point *, Point2 *, double);                    // INLINE
void   UT3D_pt_3db (Point *, double, double, double);
void   UT3D_pt_vc (Point*, Vector*);
void   UT3D_pt_txt (Point *, char *);
void   UT3D_pt_mid2pt (Point *, Point *, Point *);
int    UT3D_pt_mid_pta (Point *pto, Point *pTab, int ptNr, int iExact);
void   UT3D_pt_mid_ci (Point *, Circ *);
void   UT3D_pt_opp2pt (Point *, Point *, Point *);
int    UT3D_pt_oppptptvc (Point *po, Point *pi, Point *pl, Vector *vl);
int    UT3D_2pt_oppptvclen (Point*,Point*,Point*,Vector*,double);
void   UT3D_pt_addpt (Point *, Point *);
void   UT3D_pt_add_vc__ (Point *, Vector *);
void   UT3D_pt_add_vc_par (Point *, Vector *, double);
void   UT3D_pt_add_3vc_3par (Point*,Vector*,Vector*,Vector*,double,double,double);
void   UT3D_pt_add_pt2 (Point *, Point2 *);
void   UT3D_pt_add2pt (Point *, Point *, Point *);
void   UT3D_pt_sub_pt2 (Point *, Point *, Point2 *);
void   UT3D_pt_sub_pt3 (Point *, Point *);
void   UT3D_pt_tra_pt_dx (Point*, Point*, double);
void   UT3D_pt_tra_pt_dy (Point*, Point*, double);
void   UT3D_pt_traptvc (Point *, Point *, Vector *);
void   UT3D_pt_traptvclen (Point *po,Point *pi,Vector *vc,double dist);
void   UT3D_pt_tra_pt_pt_par (Point *pt3, Point *pt1, Point *pt2, double fakt);
void   UT3D_pt_trapt2vc (Point *po,Point *pi,Vector *vc1, Vector *vc2);
void   UT3D_pt_trapt2vc2len (Point *,Point *,Vector *,double,Vector *,double);
void   UT3D_pt_tra_pt_2vc_2par (Point *,Point *,Vector *,double,Vector *,double);
void   UT3D_pt_trapt3vc3len (Point *po,Point *pi,
              Vector *vx,double dx, Vector *vy,double dy, Vector *vz,double dz);
void   UT3D_pt_traptptlen (Point *po,Point *pi,Point *pDir,double lenv);
void   UT3D_pt_trapt2pt (Point *po, Point *pi, Point *p1, Point *p2);
int    UT3D_pt_tracirlen (Point *pto, Point *pti, Circ *cii, double clen);
int    UT3D_pt_rotptptangr (Point *pto, Point *ptc, Point *pti, double *ar);
void   UT3D_pt_rotptptvcangr (Point *pto,
                              Point *pti,Point *ptc,Vector *vz,double angr);
int    UT3D_pt_rotptm3 (Point *p2, Point *p1, Mat_4x3 ma);
int    UT3D_pt_rotciangr (Point *pto, double angr, Circ *ci1);
int    UT3D_pt_projpt2pt (Point *pp,double *len,Point *pt,Point *p1,Point *p2);
int    UT3D_pt_projptln (Point*, double*, double*, int, Point*, Line*);
int    UT3D_pt_projptci (Point *ptn, Point *ptf, Point *pt1, Circ *ci1);
int    UT3D_pt_projptptvc(Point *pp,double *len,double *par,
                          Point *pt,Point *pl,Vector *vl);
int    UT3D_pt_projptel (int *numpe, Point *pe, CurvElli *ell, Point *pt);
int    UT3D_pt_projptbspl (int *nxp, Point *ptab, double *ttab,
                           CurvBSpl *bspl, Point *pt);
int    UT3D_pt_projptptnvc (Point *po, Point *pi, Point *plo, Vector *plz);
void   UT3D_pt_projptpl (Point *, Plane *, Point *);
int    UT3D_pt_int2pt2vc (Point *ip1, Point *ip2, double *dist,
                         Point *ptu, Vector *vcu, Point *ptv, Vector *vcv);
int    UT3D_pt_intptvcplx (Point *px, Point *pl, Vector *vl, double plx);
int    UT3D_pt_intptvcply (Point *px, Point *pl, Vector *vl, double ply);
int    UT3D_pt_intptvcplz (Point *px, Point *pl, Vector *vl, double plz);
int    UT3D_pt_intptvcln (Point *ip1, Point *ip2, double *dist,
                         Point *pt1, Vector *vc1, Line *ln2);
int    UT3D_pt_intptvcxpln (Point *pti, Point *ptl,
                           Point *ptpl,  Vector *vcpl);
int    UT3D_pt_intptvcypln (Point *pti, Point *ptl,
                           Point *ptpl,  Vector *vcpl);
int    UT3D_pt_intptvczpln (Point *pti, Point *ptl,
                           Point *ptpl,  Vector *vcpl);
int    UT3D_pt_intptvcsph (Point *pa, Point *pl, Vector *vl,
                          Point *ps, double rs);
int    UT3D_2pt_int2ln (Point*,Point*,double*,Line*,Line*);
int    UT3D_pt_intperp2ln (Point *po,Point *p1,Vector *v1,Point *p2,Vector *v2);
int    UT3D_pt_intlnci__ (int *np, Point xp[], Line *ln, Circ *ci1);
int    UT3D_pt_intlnci_p (int *np, Point xp[], Line *ln, Circ *ci1);
int    UT3D_pt_intlnsph (Line *ln1, Point *ps, double rs);
int    UT3D_pt_intcici (Point pa[], Circ *ci1,int ci1Lim, Circ *ci2,int ci2Lim);
int    UT3D_pt_intplnln (Point *ip, double *dist, Plane *pl, Line *ln);
int    UT3D_pt_intlnpl (Point *, Plane *, Line *);
int    UT3D_pt_intlnpl1 (Point*, double*, Point*, Point*, double, double);
int    UT3D_pt_intptvcpln (Point *pti,
                          Point *ptl, Vector *vcl, Point *ptpl, Vector *vcpl);
int    UT3D_pt_intplnci (Point pa[], Plane *pl1, Circ *ci1, int ciLim);
int    UT3D_pt_intcidpln (int *np, Point xp[],
                         Point *pc, Point *p1, Vector *vz, double dx);
int    UT3D_pt_intptvcpl_ (Point *, Plane *, Point *, Vector *);
int    UT3D_pt_intlnel__ (int *np, Point xp[], Line *ln, CurvElli *el);
int    UT3D_pt_intlnel_p (int *np, Point xp[], Line *ln, CurvElli *el);
int    UT3D_pt_ipl_2ptpt2 (Point *pi3, Point *p1, Point *p2, Point2 *pi2);
int    UT3D_pt_mirptpl (Point *pto, Point *pti, Plane *pln);
int    UT3D_pt_mirptln (Point *pto, Point *pti, Line *ln);
int    UT3D_pt_tangptci (Point *po1, Point *po2, Point *pt1, Circ *ci1);
int    UT3D_pt_elfoc (Point *fp1, Point *fp2, CurvElli *el);

int UT3D_pt_evparln (Point *pto, double lpar, Line *ln1);
int UT3D_pt_vc__par_ci (Point *pto, Vector *vco, Circ *ci1, double par);

int    UT3D_pt_m3 (Point *pto, Mat_4x3 ma);
void   UT2D_pt_tra_pt_m3  (Point2 *p2, Mat_4x3 mata, Point2 *p1);
void   UT3D_pt_tra_pt_m3  (Point*, Mat_4x3, Point*);
void   UT3D_pt_tra_pt_m4 (Point *p2, Mat_4x4 ma, Point *p1);

int    UT2D_ptvc_ck_int2pt (int mode, Point2 *p1s, Vector2 *v1,
                                      Point2 *p2s, Point2 *p2e);
int    UT3D_ptvc_int2pl (Point *pt, Vector *vc, Plane *pl1, Plane *pl2);
int    UT3D_ptvc_int2pln (Point *pti, Vector *vci,
                  Point *pl1pt, Vector *pl1vz, Point *pl2pt, Vector *pl2vz);
int    UT3D_ptvc_ox (Point *pta, Vector *vca, ObjGX *oxi);

// double UT3D_acos_2vc (Vector*,Vector*);

double UT3D_angr_3pt (Point *p1, Point *pc, Point *p2);
double UT3D_angr_2vc__ (Vector*,Vector*);
double UT3D_angr_3vc__ (Vector *vz, Vector *v1, Vector *v2);
double UT3D_angr_3vcn_CCW (Vector *vz, Vector *v1, Vector *v2);
double UT3D_angr_ci_par1 (Circ *ci1, double par1);
int    UT3D_2angr_vc__ (double *az, double *ay, Vector *vc1);
int    UT3D_atan_vcpl (double *kvc, Vector *vci, Plane *pli);

int    UT3D_compvc0 (Vector *v1, double tol);
int    UT3D_comp2vc_d (Vector *, Vector *, double);
int    UT3D_comp2vc_p (Vector *, Vector *, double);
void   UT3D_vc_pt (Vector*, Point*);
int    UT3D_vc_ck_parpl (Vector *vci, Plane *pli, double tol);
int    UT3D_vc_ckperp_2vc1 (Vector *vc1, Vector *vc2, double tol);
int    UT3D_vc_ck_std (Vector *vc1);
int    UT3D_parvc_2vcbp (double *dl, int mainPln, Vector *v1, Vector *v2);
int    UT3D_2parvc_3vcbp (double*,double*,int,Vector*,Vector*,Vector*);
// int    UT3D_vc_multvc (Vector *vo, Vector *vi, double d);
// void   UT3D_vc_3db (Vector *, double, double, double);
// void   UT3D_vc_pt (Vector *, Point *);
void   UT3D_vc_txt (Vector *vc, char *txt);
// void   UT3D_vc_2pt (Vector *, Point *, Point *);
void   UT3D_vc_2ptlen (Vector *, Point *, Point *, double);
void   UT3D_vc_angr (Vector *vc, double angr);
void   UT3D_vc_2angr (Vector *, double, double);
void   UT3D_vc_2vc (Vector *, Vector *, Vector *);
// void   UT3D_vc_ln (Vector *, Line *);
int    UT3D_vcz_bp (Vector *vn, int bp);
void   UT3D_vc_invert (Vector*, Vector*);
// int    UT3D_vc_add2vc (Vector *v3, Vector *v1, Vector *v2);
// int    UT3D_vc_sub2vc (Vector *v3, Vector *v1, Vector *v2);
void   UT3D_vc_perp1vc (Vector *vp, Vector *vi);
void   UT3D_vc_perp2vc (Vector *, Vector *, Vector *);
int    UT3D_vc_perpvc2pt (Vector *vp, Vector *vx, Point *pc, Point *py);
int    UT3D_vc_perp3pt (Vector *vp, Point *ptc, Point *ptx, Point *pty);
int    UT3D_vc_perp4pt (Vector *vp, Point *p1, Point *p2, Point *p3, Point *p4);
void   UT3D_vc_perpTria (Vector *vn, Triang *tri);
void   UT3D_vc_perpvcplXY (Vector *, Vector *);
int    UT3D_vc_perppta (Vector *vcn, int pNr, Point *pa);
void   UT3D_vc_normalize (Vector*,Vector*);
void   UT3D_vc_setLength (Vector *, Vector *, double);
int    UT3D_vc_setLenLen (Vector *vco,Vector *vci,double newLen,double actLen);
int    UT3D_vc_tng_ci_pt (Vector *vt, Point *p1, Circ *ci);
int    UT3D_vc_rot_90_sr (Vector *vcOut, Vector *vcAxis, Vector *vcIn, int sr);
Vector UT3D_vc_rotvc_angZ (Vector *vci, double angZ);
Vector UT3D_vc_rotvc_angX (Vector *vci, double angX);
void   UT3D_vc_rot3angr (Vector *,Vector *, double, double, double);
int    UT3D_vc_rotvcvcangr (Vector *vo, Vector *va, Vector *vi, double angr);
int    UT3D_vc_projvcvc (Vector *v3, Vector *v1, Vector *v2);
int    UT3D_vc_projvc2vc (Vector *vo, Vector *vi, Vector *v1, Vector *v2);
int    UT3D_vc_projvcnvc (Vector *vo, Vector *vi, Vector *vz);
void   UT3D_vc_projvcpl (Vector *, Plane *, Vector *);
int    UT3D_vc_mirvcpl (Vector *vco, Vector *vci, Plane *pln);
int    UT3D_vc_mirvcln (Vector *vco, Vector *vci, Line *ln);
void   UT3D_vc_tra_vc_m3 (Vector *b, Mat_4x3 ma, Vector *a);
void   UT3D_vc_travcm4 (Vector *b, Mat_4x4 ma, Vector *a);

int    UT3D_comp2ln (Line *pa1, Line *pa2, double tol);
int    UT3D_ln_ck_on_ln (Point*,Point*,Point*,Point*,Point*,Point*,double tol);
int    UT3D_ln_ck_parpl (double *dist, Line *ln, Plane *pl, double tol);
void   UT3D_ln_6db (Line*, double, double, double, double, double, double);
void   UT3D_ln_ptpt (Line *ln1, Point *pta, Point *pte);
void   UT3D_ln_2pt2 (Line *ln1, Point2 *pta, Point2 *pte);
int    UT3D_ln_int2pl (Line *ln, Plane *pl1, Plane *pl2);
int    UT3D_ln_tng_ci_ci (Line *ln1, Circ *ci1, Circ *ci2, int sNr);
int    UT3D_ln_parl2ln (Point*, Point*, Point*, Point*, Point*, Point*);
int    UT3D_ln_pts_dmax (Line *ln, int np, Point *ptab, double tol);
int    UT3D_ln_setLen (Line *lno, Point *pc, double lnlg, Line *lni);
void   UT3D_ln_inv (Line *ln1);
void   UT3D_ln_ln2 (Line*, Line2*);

int    UT3D_rdc_3pt (double *rdc, Point *pp1, Point *pp2, Point *pp3);
int    UT3D_ck_ci180 (Circ *ci1);
int    UT3D_ck_ci360 (Circ *ci1);
int    UT3D_ci_cip1  (Circ *ci1, Point *p1);
int    UT3D_ci_ptvcr (Circ *ci1, Point *pc, Vector *vz, double rc);
int    UT3D_ci_pt2vcr (Circ *ci1, Point *pc, Vector *vz, Vector *vx, double rc);
int    UT3D_ci_cip2   (Circ *ci1, Point *p2);
int    UT3D_ci_cip3   (Circ *ci1, Point *p1, Point *p2);
int    UT3D_ci_ciangr (Circ *ci, double ao2);
void   UT3D_ci_setangr (Circ *ci1);
int    UT3D_ci_ptptvcangr (Circ *ci,Point *pc,Point *p1,Vector *vz,double ao);
int    UT3D_ci_ptptvcrd (Circ *ci, Point *p1,Point *p2,double rd,Vector *vz,
                         int sr, int iMod);
int    UT3D_ci_cipt180 (Circ *ci1, Point *p1);
int    UT3D_ci_cipt360   (Circ *ci1, Point *p1);
int    UT3D_ci_pcvzpt180 (Circ *ci1,Point *pc,Vector *vz,Point *p1,int dreh);
int    UT3D_ptNr_ci (Circ *ci1, double tol);
int    UT3D_ci_inv1 (Circ *ci1);
int    UT3D_ci_inv2 (Circ *ci1);
int    UT3D_ci_inv3 (Circ *ci1);
void   UT3D_ci_ci2  (Circ*, Circ2*);
Circ   UT3D_ci_obj2 (ObjG2 *);
// Circ   UT3D_ci_obj (ObjG *ci_in);
int    UT3D_ci_ptvcrd (Circ *ci, Point *ps, Vector *vs, double rd,
                       Vector *vz, double a1);
int    UT3D_ci_2ptvcrd (Circ *cia,Point *pp1,Point *pp2,Vector *vz,double rdc);
int    UT3D_ci_3pt (Circ *cpo, Point *pp1, Point *pp2, Point *pp3);
int    UT3D_ci_ptptrd (Circ ca[], Point *pt1, Point *pt2, double radi);
int    UT3D_ci_lnptrd (Circ ca[], Line *ln1,Point *pt1,Vector *vz,double rdc);
int    UT3D_ci_ciptrd (Circ ca[], Circ *ci1, Point *pt1, double rdc);
int    UT3D_ci_2lnrd  (Circ ca[], Line *ln1, Line *ln2, double rdc, int);
int    UT3D_ci_cicird (Circ ca[], Circ *ci1, Circ *ci2, double rdc, int);
int    UT3D_ci_lncird (Circ ca[], Line *ln1, Circ *ci1, double rdc, int);
int    UT3D_ci_intplsph (Circ *cio, Plane *pli, Sphere *spi);
int UT3D_ci_ptrd2vc2angr (Circ *ci, Point *pc, double rd,
                            Vector *vz, Vector *vx,
                            double a1, double a2);
int UT3D_ci_ptvcpt2angr (Circ *ci,
                           Point *pc, Point *p1, Vector *vz,
                           double a1, double a2);
void UT3D_ci_tra_ci_m3 (Circ *cio, Mat_4x3 trmat, Circ *cii);

// source ../ut/ut_elli.c
int UT3D_ck_el360 (CurvElli *el1);
int UT3D_el_elpt180 (CurvElli *el1, Point *p1);
double UT3D_angr_elpt (Point *pti,Point *ptc,Vector *va,Vector *vb);
int UT2D_pt_elangd (Point2 *pto, double lx, double ly, double angr);
int UT3D_pt_elangd (Point *pto,Point *ptc,Vector *va,Vector *vb,double angr);
int UT3D_el_pt2vc2a (CurvElli *el, Point *ptc, Vector *vcx, Vector *vcy,
                       double a1, double a2, int idir);
int    UT3D_el_projcipl (CurvElli *el, Plane *pl, Circ *ci);
int    UT3D_el_projelpl (CurvElli *elo, Plane *pl, CurvElli *eli);
int    UT3D_el_elcoe(CurvElli *,polcoeff_d5 *,Point2 *pa,Point2 *pe,double zt);
int    UT3D_elcoe_el(polcoeff_d5*, CurvElli*);


int    UT3D_pta_ck_planar (int pNr, Point *pTab, Point *pPln, Vector *vc);
int    UT3D_rMin_pta (double *rMin, int pNr, Point *pTab);
int    UT3D_sr_polc (int ptNr, Point *pa, Vector *vcn, int plMain);
int    UT3D_pta_dbo (Point **pTab, int *pNr, int typ, long ind, double tol);
// Curv   UT3D_cv_obj2 (ObjG2 *);
void   UT3D_cv_ln (Point *cv, int *ptAnz, Point *p1, Point *p2);
int    UT3D_npt_ci (Point *pa, int pNr, Circ *ci1);
void   UT3D_cv_ci (Point cv[], int *ptanz, Circ *ci1, int ptmax, double tol);
int    UT3D_cv_delia (int *pNr, Point *pTab, char *ptStat);
int    UT3D_cv3_linear (int *pNr, Point *pTab, double tol);

// ObjG   UT3D_obj_pt  (Point*);
ObjG   UT3D_obj_ln  (Line*);
// ObjG   UT3D_obj_ci2 (Circ2 *ci1, double zVal);
ObjG   UT3D_obj_ci  (Circ*);
// ObjG   UT3D_obj_obj2 (ObjG2 *o2);

int    UT3D_bp_perp_2pt (Point *pt1, Point *pt2);
int    UT3D_bp_perp_vc (int*, Vector*);
int    UT3D_bp_pta (int pNr, Point *pa);

void   UT3D_pl_XYZ (Plane *pl1);
int    UT3D_pl_bp (Plane *plo, int bp, double dbc);
int    UT3D_pl_3pt (Plane *, Point *, Point *, Point *);
int    UT3D_pl_ptvc (Plane *, Point *, Vector *);
int    UT3D_pl_ptvzpl (Plane *pl, Point *pt, Vector *vcz, Plane *basPln);
int    UT3D_pl_ptvxpl (Plane *pl, Point *pt, Vector *vcx, Plane *basPln);
int    UT3D_pl_pto_vcz_vcx (Plane *pl1, Point *po, Vector *vz, Vector *vx);
void   UT3D_pl_pto_vcx_vcz (Plane *pl1, Point *po, Vector *vx, Vector *vz);
void   UT3D_pl_pto_vcx_vcy (Plane *pl1, Point *po, Vector *vx, Vector *vy);
void   UT3D_pl_pto_vcz_ptx (Plane *pl1, Point *po, Vector *vz, Point *ptx);
int    UT3D_pl_perp_ln (Plane *pln, Line *lna);
void   UT3D_pl_2ln (Plane *pl1, Line *ln1, Line *ln2);
int    UT3D_pl_ci (Plane *pl1, Circ *ci1);
int    UT3D_pl_nobj (Plane *pl1, int oNr, ObjGX *oTab);
void   UT3D_pl_invert (Plane *pl);
int    UT3D_pl_rotpt (Plane *plo, Plane *pli, Point *ptx);
void   UT3D_pl_ptpl (Plane *pl, Point *pt1);
int    UT3D_pl_plcoe (Plane *pl, polcoeff_d3 *co);
int    UT3D_plcoe_pl (polcoeff_d3 *co, Plane *pl);
void   UT3D_pl_p (Plane *pl);

double UT3D_plg_lvTab (double *lvTab, Point *pta, int ptNr);
int    UT3D_pta_plg (int *ptNr, Point *pta, CurvPoly *plg);


int UT2D_box_ini0 (Point2 *pt1, Point2 *pt2);
int UT2D_box_extend (Point2 *pb1, Point2 *pb2, Point2 *pt1);
int UT2D_rect_pts (Point2 *pmin, Point2 *pmax, int nump, Point2 *ptab);
int UT2D_ckBoxinBox1 (Point2 *p1, Point2 *p2, Point2 *p3, Point2 *p4);
int UT2D_ckBoxinBox2 (Point2 *p1, Point2 *p2,Point2 *p3, Point2 *p4, double tol);
int UT2D_pt_ck_inBox (Point2 *p1, Point2 *p2, Point2 *p);
int UT2D_pt_ck_inBoxTol (Point2 *p1, Point2 *p2, Point2 *p, double tol);

int UT3D_cv_boxxy (Point* pb1,double x1,double x2,double y1,double y2);
int UT2D_box_ini0 (Point2 *pt1, Point2 *pt2);
int UT3D_box_extend (Point* pb1, Point* pb2, Point* pt1);
int UT3D_box_2pt__ (Point *pb1, Point *pb2, Point *pt1, Point *pt2);
int UT3D_box_2pt_tol (Point *pb1, Point *pb2, Point *pt1, Point *pt2,double tol);
int UT3D_box_pts (Point *pmin, Point *pmax, int nump, Point *ptab);
int UT3D_box_tria (Point *pb1, Point *pb2, Triang *tr, double tol);
int UT3D_box_addTol (Point *pb1, Point *pb2, double tol);
int UT3D_ck_ptInBox (Point *p1, Point *p2, Point *px);
int UT3D_ckBoxinBox1 (Point *p1, Point *p2, Point *p3, Point *p4);
int UT3D_box_ck_intLnLim (Line *ln, Point *pmin, Point *pmax, double tol);
int UT3D_box_ck_intpl (Plane *pln, Point *p1, Point *p2, double tol);
int UT3D_rbox_loadxy (Point* pb1,double x1,double x2,double y1,double y2);
int UT3D_ptvc_intbox (Point *pl, Vector *vl, Point *bp1, Point *bp2);
int UT3D_ln_intbox (Line *ln1, Point *bp1, Point *bp2);


// void   UT3D_tria_fac(Triang*, Fac3*, Point*);

int    UT3D_m3_inirot_angr (Mat_4x3 ma, Point *pa, Vector *va, double angr);
void   UT3D_m3_loadpl   (Mat_4x3, Plane *);
void   UT3D_m3_load     (Mat_4x3, Vector *, Vector *, Vector *);
void   UT3D_m3_load_povxvy (Mat_4x3 ma, Point *ori, Vector *vx, Vector *vy);
void   UT3D_m3_load_povxvz (Mat_4x3 ma, Point *ori, Vector *vx, Vector *vz);
void   UT3D_m3_load_o   (Mat_4x3, Point *);
// int    UT3D_m3_get      (void *out, int mode, Mat_4x3 ma);
// int    UT3D_m3_set      (Mat_4x3 ma, int mode, void *da);
int    UT3D_m3_invm3    (Mat_4x3, Mat_4x3);
void   UT3D_m3_multm3   (Mat_4x3 m3, Mat_4x3 m1, Mat_4x3 m2);
int    UT3D_m3_tram3m3  (Mat_4x3 m3, Mat_4x3 m1, Mat_4x3 m2);

void UT3D_m4_init       (Mat_4x4 ma);
void UT3D_m4_init_ori   (Mat_4x4 ma, double px, double py, double pz);
void UT3D_m4_init_rot   (Mat_4x4 ma, double angle, Vector *axis);
void UT3D_m4_loadpl     (Mat_4x4 m1, Plane *pl1);
void UT3D_m4_load_o     (Mat_4x4 ma,  Point *ori);
void UT3D_m4_addrot     (Mat_4x4 mo, Mat_4x4 ma, Mat_4x4 mb);
void UT3D_m4_addtra     (Mat_4x4 ma, double px, double py, double pz );

double UT3D_sbs_ck_planar (SurBSpl *su1);
double UT3D_sru_ck_planar (ObjGX *ru1);



//----------------------------------------------------------------
// ../ut/ut_transform
// Vector UTRA_vc_abs2rel__ (Vector *vcAbs);
// Vector UTRA_vc_rel2abs__ (Vector *vcRel);
// Point UTRA_pt_rel2abs__ (Point *pRel);
// Point UTRA_pt_abs2rel__ (Point *pAbs);


//================================================================
// inline functions

/// UT_RADIANS                radians from degrees
#define UT_RADIANS(angDeg) ((angDeg)*(RAD_1))
/// UT_DEGREES                degrees from radians
#define UT_DEGREES(angRad) ((angRad)/(RAD_1))

#define UT3D_vc_crossprod2vc      UT3D_vc_perp2vc
#define UT2D_acos_innerprod2vc    UT2D_acos_2vc


//----------------------------------------------------------------
/// UT2D_angr_set_2angr_sr        set angle following|preceding ang1 (-2Pi to 2Pi)
///   CCW: change ang2 to a value following ang1; (opening-angle is 0 to 2Pi)
///   CW:  change ang2 to a value preceding ang1; (opening-angle is -2Pi to 0)
/// Input:
///   ang1  angle of startpoint of limited circle; 0-2Pi (in rad)
///   ang2  angle of endpoint of limited circle; 0-2Pi (in rad)
///   sr    sense-of-rotation; 0=CCW; 1=CW
/// Output:
///   ang2  angle of endpoint of limited circle; -2Pi to 2Pi
/// \endcode
double UT2D_angr_set_2angr_sr(double ang1, double ang2, int sr);
#define UT2D_angr_set_2angr_sr(ang1,ang2,sr)\
 ((!sr)?((ang1>ang2)?ang2+RAD_360:ang2):((ang1<ang2)?ang2-RAD_360:ang2))
 // ((!sr)?((ang1>ang2)?ang2+RAD_360:ang2):((ang1-RAD_360>ang2)?ang2+RAD_360:ang2))
 // ((!sr)?((ang1>ang2)?ang2+RAD_360:ang2):((ang1<ang2)?ang2-RAD_360:ang2))
//
// was UT2D_angr_set_2angr
// get angle (from 0 to 2Pi) with UT2D_angr_ptpt
// get sr from rad or ango with DLIM01
//   if(!sr) if(ang1 > ange) ange += RAD_360;    // CCW
//   else    if(ang1 < ange) ange -= RAD_360;    // CW
//           a1 a2 sr  -> a2
// Examples: 5, 4, 0   -> 10        printf(" %f\n",UT2D_angr_set_2angr_sr(5.,4.,0));
//           2  1  0   ->  7
//           1  5  0   ->  5
//           4  2  0   ->  8
//           4  5  1   -> -1 
//           1  2  1   -> -4
//           5  1  1   ->  1
//           2  4  1   -> -2
//           7  0  1   ->  6



/// UT2D_ANGR_ADD_4PI         get angle of endpoint of circ  (0 to 4Pi)
///   from angle at startpoint and opening angle
///   set angle at start- and endpoint of circle according sense of rotation
///   sense of rotation from sign of ango; pos = CCW; neg = CW.
///  Input:
///    a1    starting angle of circle (must be 0 to 2Pi)
///    ango  opening angle of circle; CW: (-2Pi - 0) | CCW: (0 - 2Pi)
///  Output:
///    a1    angle at startpoint of circle; (0 to 4Pi)   - can become modified
///    a2    angle at endpoint of circ;     (0 to 4Pi)
/// 
#define UT2D_ANGR_ADD_4PI(a1,a2,ango){\
 a2 = a1 + ango;\
 if(a2 < 0.) { a1 += RAD_360; a2 += RAD_360;}}\
 // if(ango > 0.) { a2 = a1 + ango;}\
//
// see UT2D_ANGR_ADD_2PI ((-2Pi to 2Pi)
// see also UT2D_2angr_set UT2D_angr_set_2angr_sr  (-2Pi to 2Pi)
// Why is (0 to 4Pi) better than (-2Pi to 2Pi):
//   for (0 to 4Pi) add RAD_360 to new points on arc (UT2D_angr_set_2angr_sr)
//   for (-2Pi to 2Pi) must add OR SUBTRACT RAD_360




/// UT2D_ANGR_ADD_2PI           get angle of endpoint of circ  (-2Pi to 2Pi) 
//   DO NOT USE; use UT2D_ANGR_ADD_4PI
//   get ange from angs and ango;   can modify angs
/// Input:
///   angs   angle of startpoint of circ (0 - 2Pi)
///   ango   opening angle of circle; CW: (-2Pi - 0) | CCW: (0 - 2Pi)
/// Output:
///   angs   angle of startpoint of circ (-2Pi - 2Pi)   - can become modified
///   ange   angle of endpoint of circle (-2Pi - 2Pi)
///
/// get angs eg from UT2D_angr_ptpt
///
void UT2D_ANGR_ADD_2PI (double ange, double angs, double ango);
#define UT2D_ANGR_ADD_2PI(ange,angs,ango){\
ange = angs + ango;\
if(ange>RAD_360){angs -= RAD_360; ange -= RAD_360;}}\
///
/// Examples: angs ango   ->  angs ange
///            5    5          -1   4


// UT2D_sr_ci             return sense of direction; 0=CCW; 1=CW.
int UT2D_sr_ci (Circ2 *cii);
#define UT2D_sr_ci(cii) ((cii)->rad >= 0.) ? 0 : 1
// #define UT2D_sr_ci(cii) (((Circ2*)(cii))->rad >= 0.) ? 0 : 1




//----------------------------------------------------------------
/// UT2D_lenq_vc         quadr.length of 2D-vector
#define UT2D_lenq_vc(vc) ((vc)->dx*(vc)->dx + (vc)->dy*(vc)->dy)

/// UT2D_len_vc          length of 2D-vector
#define UT2D_len_vc(vc) (sqrt((vc)->dx*(vc)->dx + (vc)->dy*(vc)->dy))

/// UT2D_lenS_2pt             dx+dy-distance point-point
#define UT2D_lenS_2pt(p1,p2) (fabs((p2)->x - (p1)->x) + fabs((p2)->y - (p1)->y))

/// UT2D_lenS_vc              dx+dy-distance 2D-vector
#define UT2D_lenS_vc(vc1) (fabs((vc1)->dx) + fabs((vc1)->dy))

/// UT2D_lenq_2pt        quadr. distance pt - pt
#define UT2D_lenq_2pt(lq,p1,p2){\
  double _dx = (p2)->x - (p1)->x;\
  double _dy = (p2)->y - (p1)->y;\
  *(lq) = _dx * _dx + _dy * _dy;}

/// \brief UT2D_comp2pt              compare 2 points
/// \code
/// RC = 0: points not identical; distance > tolerance
/// RC = 1: points are identical; distance < tolerance
/// \endcode
#define UT2D_comp2pt(p1,p2,tol)\
  ((fabs((p2)->x - (p1)->x) < tol) &&\
   (fabs((p2)->y - (p1)->y) < tol))

/// \brief UT2D_swap2pt                  swap 2 2D-points
/// \code
/// see also MEM_swap_
/// \endcode
#define UT2D_swap2pt(p1,p2){\
  Point2 p3 = *(p1); *(p1) = *(p2); *(p2) = p3;}

/// UT2D_pt_2db              2D-Point < 2 doubles (x, y)
#define UT2D_pt_2db(pt2,dx,dy){\
  (pt2)->x = dx; (pt2)->y = dy;}

/// UT2D_pt_pt3               2D-Point = 3D-Point (cut off z-coord)
#define UT2D_pt_pt3(pt3)\
  (*(Point2*)pt3)

///// UT2D_pt_pt                2D-Point = 3D-Point
//#define UT2D_pt_pt(pt2,pt3) (memcpy((pt2), (pt3), sizeof(Point2)))



/// UT2D_vc_tra_vc3_bp        2D-Vector = 3D-Vector on Backplane
void UT2D_vc_tra_vc3_bp (Vector2*,Vector*,int);
#define UT2D_vc_tra_vc3_bp(v2o,v3i,bpi) {\
  if     (bpi == BCKPLN_XY) {(v2o)->dx = (v3i)->dx; (v2o)->dy = (v3i)->dy;} \
  else if(bpi == BCKPLN_XZ) {(v2o)->dx = (v3i)->dx; (v2o)->dy = (v3i)->dz;} \
  else if(bpi == BCKPLN_YZ) {(v2o)->dx = (v3i)->dy; (v2o)->dy = (v3i)->dz;}}




/// UT3D_pt_tra_pt_bp       transf. 3D-Point => 2D-Point with Z-value from backplane
void UT3D_pt_tra_pt_bp (Point*,Point*,int);
#define UT3D_pt_tra_pt_bp(p2o,p3i,bpi) {\
  if     (bpi == BCKPLN_XY) {\
    (p2o)->x = (p3i)->x; (p2o)->y = (p3i)->y; (p2o)->z = (p3i)->z;\
  } else if(bpi == BCKPLN_XZ) {\
    (p2o)->x = (p3i)->x; (p2o)->y = (p3i)->z; (p2o)->z = -(p3i)->y;\
  } else if(bpi == BCKPLN_YZ) {\
    (p2o)->x = (p3i)->y; (p2o)->y = (p3i)->z; (p2o)->z = -(p3i)->x;\
  }}


/// UT2D_pt_tra_pt3_bp        2D-Point = 3D-Point on Backplane
void UT2D_pt_tra_pt3_bp (Point2*,Point*,int);
#define UT2D_pt_tra_pt3_bp(p2o,p3i,bpi) {\
  if     (bpi == BCKPLN_XY) {(p2o)->x = (p3i)->x; (p2o)->y = (p3i)->y;} \
  else if(bpi == BCKPLN_XZ) {(p2o)->x = (p3i)->x; (p2o)->y = (p3i)->z;} \
  else if(bpi == BCKPLN_YZ) {(p2o)->x = (p3i)->y; (p2o)->y = (p3i)->z;}}

// /// UT3D_pt_tra_pt2_bp        3D-Point = 2D-Point on Backplane
// void UT3D_pt_tra_pt2_bp (Point*,Point2*,double,int);
// #define UT2D_pt_tra_pt3_bp(p3o,p2i,d0,bpi) {\
//   if     (bpi == BCKPLN_XY)\
//     {(p3o)->x = (p2i)->x; (p3o)->y = (p2i)->y; (p3o)->z = (d0);}\
//   else if(bpi == BCKPLN_XZ)\
//     {(p3o)->x = (p2i)->x; (p3o)->y = (p2i)->z; (p3o)->z = (d0);} \
//   else if(bpi == BCKPLN_YZ)\
//     {(p3o)->x = (p2i)->y; (p3o)->y = (p2i)->z; (p3o)->z = (d0);}}




/// UT2D_pt_addpt                 Add two points:      po += p1
#define UT2D_pt_addpt(po,p1){\
 (po)->x = (po)->x + (p1)->x;\
 (po)->y = (po)->y + (p1)->y;}

/// UT2D_pt_add_vc__            add vector:  pt += vc
#define UT2D_pt_add_vc__(pt,vc){\
 (pt)->x += (vc)->dx;\
 (pt)->y += (vc)->dy;}
// see UT2D_pt_traptvc

/// UT2D_pt_sub_pt               subtract point p2 from p1
#define UT2D_pt_sub_pt(po,p1,p2){\
 (po)->x = (p1)->x - (p2)->x;\
 (po)->y = (p1)->y - (p2)->y;}

/// UT2D_pt_tra_pt_ivc           subtract vector vc from p1
#define UT2D_pt_tra_pt_ivc(po,pi,vc){\
 (po)->x = (pi)->x - (vc)->dx;\
 (po)->y = (pi)->y - (vc)->dy;}

/// UT2D_pt_opp2pt                opposite point (p1 = center)
#define UT2D_pt_opp2pt(po,p1,p2){\
  (po)->x = (p1)->x - ((p2)->x - (p1)->x);\
  (po)->y = (p1)->y - ((p2)->y - (p1)->y);}

/// UT2D_pt_mid2pt                midpoint between 2 points
#define UT2D_pt_mid2pt(po,p1,p2){\
  (po)->x = ((p1)->x + (p2)->x) / 2.0;\
  (po)->y = ((p1)->y + (p2)->y) / 2.0;}

/// UT2D_pt_traptvc               2D Point = Point + Vector
#define UT2D_pt_traptvc(po,pi,vc){\
 (po)->x = (pi)->x + (vc)->dx;\
 (po)->y = (pi)->y + (vc)->dy;}
// see UT2D_pt_add_vc__

/// UT2D_pt_pt_mult_vc            po = pi + (vi * d)                MULT
#define UT2D_pt_pt_mult_vc(po,pi,vi,d){\
 (po)->x = (vi)->dx * (d) + (pi)->x;\
 (po)->y = (vi)->dy * (d) + (pi)->y;}


//----------------------------------------------------------------
/// \code
/// UT2D_slen_vc_vcNo         signed length of vector on norm.vector
/// vnab must be normalized;  else returns (slen * length_of_vAB)
///           0
///     neg   .  pos   C 
///           .       /|                   A-C  = vector vac
///           .     /  |                   A-B  = vector vnab  (normalized)
///           .   /    |
/// -0.9      . /      |            0.9
///    . . .  A--------X---------B    1
/// -0.9      |--slen--|            0.9
///           .
///     neg   .   pos
///      -0.1 . 0.1
///           0
///
///  Input:
///    (Vector2)vAC;
///    (Vector2)vAB;   if not normalized: returns (slen * length_of_vab)
///  Returns:
///    (double)slen;   - parametric value of distance A-X; X is C normal on A-B
///                      negative if C is left of a normal to A-B in A
///                    - the cos of the opening angle of 2 vecs (dot=scalarprod)
///                    
/// \endcode
#define UT2D_slen_vc_vcNo UT2D_skp_2vc
/// UT2D_skp_2vc        cos of opening angle of 2 vecs (dot=scalarprod) DOT
#define UT2D_acos_2vc UT2D_skp_2vc

#define UT2D_skp_2vc(v1,v2)\
 ((v1)->dx * (v2)->dx + (v1)->dy * (v2)->dy)
//
// skp (vx,vx) gives quadratic-length of vx


//----------------------------------------------------------------
/// UT2D_sar_2vc        signed area of 2 vectors (scalarprod)
/// sign gives side
///       0.9 1  0.9
///              X
///           v2/        pos  (0-1)
///   0.1      /          0.1
/// ----------X---v1----------->  0
///  -0.1                -0.1
///       -0.9  -0.9
///           -1
///                      neg  (-1 - 0)
/// UT2D_crossprod_2vc        crossprod of 2 2D-Vectors
// see UT2D_sid_2vc__  (test if v2 is CCW (pos) or CW (neg) from v1)
// #define UT2D_crossprod_2vc UT2D_sar_2vc
#define UT2D_sar_2vc(v1,v2)\
 ((v1)->dx * (v2)->dy - (v1)->dy * (v2)->dx)


//----------------------------------------------------------------
/// UT3D_acos_vc2pt      cos of opening angle of vec-(pt-pt) (dot=scalarprod) DOT
#define UT2D_acos_vc2pt UT2D_skp_vc2pt
#define UT2D_skp_vc2pt(v1,p1,p2)\
 ((v1)->dx * ((p2)->x - (p1)->x) +\
  (v1)->dy * ((p2)->y - (p1)->y))

/// UT2D_vc_pt                2D-Vector from 0,0 -> point
void   UT2D_vc_pt (Vector2*, Point2*);
#define UT2D_vc_pt(vc,pt) (memcpy((vc), (pt), sizeof(Point2)))

/// UT2D_vc_vc3              2D-Vector from 3D-Vector
#define UT2D_vc_vc3(vc2,vc3)\
  (memcpy((vc2), (vc3), sizeof(Vector2)))

/// UT2D_vc_2db              2D-Vector < 2 doubles (x, y)
#define UT2D_vc_2db(vc,dx,dy){\
  (vc)->dx = (dx);\
  (vc)->dy = (dy);}

/// UT2D_vc_invert          2D-Vector invert (change direction)
#define UT2D_vc_invert(vo,vi){\
  (vo)->dx = -(vi)->dx;\
  (vo)->dy = -(vi)->dy;}

/// UT2D_vc_add2vc                v3 = v1 + v2                ADD
#define UT2D_vc_add2vc(v3,v1,v2){\
 (v3)->dx = (v1)->dx + (v2)->dx;\
 (v3)->dy = (v1)->dy + (v2)->dy;}

/// UT2D_vc_sub2vc                v3 = v1 - v2                SUB
#define UT2D_vc_sub2vc(v3,v1,v2){\
 (v3)->dx = (v1)->dx - (v2)->dx;\
 (v3)->dy = (v1)->dy - (v2)->dy;}

void UT2D_vc_div_d (Vector2*, Vector2*, double);
/// UT2D_vc_div_d             divide vector
#define UT2D_vc_div_d(vo,vi,d){\
 (vo)->dx = (vi)->dx / (d);\
 (vo)->dy = (vi)->dy / (d);}

/// UT2D_vc_multvc                vo = vi * d                 MULT
#define UT2D_vc_multvc(vo,vi,d){\
 (vo)->dx = (vi)->dx * (d);\
 (vo)->dy = (vi)->dy * (d);}

/// UT2D_vc_rot_90_ccw            vector = perpendic. to vector ( + 90 degrees)
#define UT2D_vc_rot_90_ccw(vo,vi)\
 {double _dx = (vi)->dx; (vo)->dx = -(vi)->dy; (vo)->dy = _dx;}

/// UT2D_vc_rot_90_cw             vector = perpendic. to vector ( + 90 degrees)
#define UT2D_vc_rot_90_cw(vo,vi)\
 {double _dx = -(vi)->dx; (vo)->dx = (vi)->dy; (vo)->dy = _dx;}


/// UT2D_vcPerpAppr_vc_len create vector with fixed length normal to vector
/// approximation (not precise)
/// Vector2 vp, vc={ 0.5, -1.}; double dd = 0.1;
/// UT2D_vcPerpAppr_vc_len (&vp, &vc, &dd); printf(" dx=%f dy=%f\n",vp.dx,vp.dy);
void UT2D_vcPerpAppr_vc_len (Vector2 *vco, Vector2 *vci, double *len);
#define UT2D_vcPerpAppr_vc_len(vco,vci,len){\
 if(fabs((vci)->dx) > fabs((vci)->dy))\
 {(vco)->dx = 0.; (vco)->dy = ((vci)->dx > 0.) ? *len : -*len;}\
 else\
 {(vco)->dy = 0.; (vco)->dx = ((vci)->dy > 0.) ? -*len : *len;}}


/// UT2D_vc_perp2pt           vector = perpendic. to Line ( + 90 degrees)
#define UT2D_vc_perp2pt(vo,p1,p2){\
  (vo)->dx = (p1)->y - (p2)->y;\
  (vo)->dy = (p2)->x - (p1)->x;}

/// UT2D_vc_angr            2DVector =  angle (radians)
#define UT2D_vc_angr(vc,angr){\
  (vc)->dx = cos(angr);\
  (vc)->dy = sin(angr);}

/// UT2D_vc_vc3bp        2D-Vector = 3D-Vector on Backplane
#define UT2D_vc_vc3bp(v2o,v3i,bpi) {\
 if     (bpi == BCKPLN_XY) {(v2o)->dx = (v3i)->dx; (v2o)->dy = (v3i)->dy;} \
 else if(bpi == BCKPLN_XZ) {(v2o)->dx = (v3i)->dx; (v2o)->dy = (v3i)->dz;} \
 else if(bpi == BCKPLN_YZ) {(v2o)->dx = (v3i)->dy; (v2o)->dy = (v3i)->dz;}}

/// UT2D_vc_2pt               2D-Vector = 2D-Point -> 2D-Point
#define UT2D_vc_2pt(vc,p1,p2){\
 (vc)->dx = (p2)->x - (p1)->x;\
 (vc)->dy = (p2)->y - (p1)->y;;}

/// UT2D_vc_ln               2D-Vector from 2D-Line
#define UT2D_vc_ln(vc,ln){\
 (vc)->dx = (ln)->p2.x - (ln)->p1.x;\
 (vc)->dy = (ln)->p2.y - (ln)->p1.y;}


/// UT2D_vc_tra_vc3_m3        transf. 3D-vector => 2D-Vector (with 4x3-matrix
void UT2D_vc_tra_vc3_m3 (Vector2*, Mat_4x3, Vector*);
#define UT2D_vc_tra_vc3_m3(vco,mat,vci)\
  UT2D_pt_tra_pt3_m3((Point2*)vco,mat,(Point*)vci)



//----------------------------------------------------------------

// UT2D_ln_ptpt                  2D-Line from 2 2D-points
#define UT2D_ln_ptpt(ln1,pt1,pt2){\
 (ln1)->p1 = *(pt1);\
 (ln1)->p2 = *(pt2);}

/// UT2D_ln_ptvc                  2D-Line from 2D-point and 2D-vector
#define UT2D_ln_ptvc(ln,pt,vc){\
 (ln)->p1 = *(pt);\
 (ln)->p2.x = (pt)->x + (vc)->dx;\
 (ln)->p2.y = (pt)->y + (vc)->dy;}

/// UT2D_ln_ln3__                   2D-Line = 3D-Line
#define UT2D_ln_ln3__(ln2,ln3){\
 (ln2)->p1.x = (ln3)->p1.x;\
 (ln2)->p1.y = (ln3)->p1.y;\
 (ln2)->p2.x = (ln3)->p2.x;\
 (ln2)->p2.y = (ln3)->p2.y;}

/// UT2D_len_cia                  length arc (from rad & angle)
#define UT2D_len_cia(radius, ango) (fabs((radius)*(ango)))

/// UT2D_ln_inv                Invert 2D-Line
#define UT2D_ln_inv(ln){\
  Point2 _pt = (ln)->p1; (ln)->p1 = (ln)->p2; (ln)->p2 = _pt;}

/*
// UT2D_ci_ci3                  2D-Circ = 3D-Circ
#define UT2D_ci_ci3(ci2,ci3){\
 (ci2)->p1.x = (ci3)->p1.x;\
 (ci2)->p1.y = (ci3)->p1.y;\
 (ci2)->p2.x = (ci3)->p2.x;\
 (ci2)->p2.y = (ci3)->p2.y;\
 (ci2)->pc.x = (ci3)->pc.x;\
 (ci2)->pc.y = (ci3)->pc.y;\
 (ci2)->rad = (ci3)->rad;\
 (ci2)->ango = (ci3)->ango;}
*/


//----------------------------------------------------------------
// UT3D_sr_ci             return sense of direction; 0=CCW; 1=CW.
int UT3D_sr_ci (Circ *cii);
#define UT3D_sr_ci(cii) (((Circ*)(cii))->rad >= 0. ? 0 : 1)



//----------------------------------------------------------------
/// UT3D_lenq_vc         quadr.length of 3D-vector
#define UT3D_lenq_vc(vc)\
 ((vc)->dx*(vc)->dx + (vc)->dy*(vc)->dy + (vc)->dz*(vc)->dz)

/// UT3D_len_vc          length of 3D-vector
#define UT3D_len_vc(vc)\
 (sqrt((vc)->dx*(vc)->dx + (vc)->dy*(vc)->dy + (vc)->dz*(vc)->dz))

/// \brief UT3D_skp_2vc         scalarprod of 2 vecs (dotprod,Skalarprod)
/// \code
/// UT3D_acos_2vc        cos of opening angle of 2 vecs; see double UT3D_acos_2vc
/// get opening-angle: oa = ACOS(UT3D_acos_2vc (&v1, &v2));
/// test if 2 vectors normal: if(fabs(UT3D_skp_2vc(&v1,&v2)) < UT_TOL_min1) yes
/// if (length of v1 = 1): signed length of v2 along v1 (see UT3D_slen_projvcvc)
/// 
///            /|
///        V2/  |
///        /    |
///      /      |
///    x--------+------>  V1
///    <---rc--->
/// \endcode
// skp mit sich selbst = Laenge * Laenge
#define UT3D_skp_2vc(v1,v2)\
 ((v1)->dx*(v2)->dx + (v1)->dy*(v2)->dy + (v1)->dz*(v2)->dz)

#define UT3D_skp_2ptvc(p1,p2,v1)\
 ((v1)->dx * ((p2)->x - (p1)->x) +\
  (v1)->dy * ((p2)->y - (p1)->y) +\
  (v1)->dz * ((p2)->z - (p1)->z))

/// UT3D_acos_vc2pt      cos of opening angle of vec-(pt-pt) (dot=scalarprod) DOT
#define UT3D_acos_2vc UT3D_skp_2vc
#define UT3D_acos_vc2pt UT3D_skp_2ptvc


//----------------------------------------------------------------
/// UT3D_pt_NEW              create empty point (UT3D_pt_isFree) 
#define UT3D_pt_NEW {UT_VAL_MAX, 0., 0.}
    
/// UT3D_pt_setFree          set point not valid (not initialized)
#define UT3D_pt_setFree(obj) (obj)->x = UT_VAL_MAX

/// \code
/// UT3D_pt_isFree           check if pt is empty (UT3D_pt_setFree UT3D_pt_NEW)
/// 0 = point is valid )(pt->x != FLT_32_MAX)
/// 1 = point is free (NEW, not set).
/// \endcode
#define UT3D_pt_isFree(obj) ((obj)->x == UT_VAL_MAX)

/// UT3D_pt_pt2               3D-Point = 2D-Point + Z-Val 0.
#define UT3D_pt_pt2_0(pt3,pt2){\
  memcpy((pt3), (pt2), sizeof(Point2)); (pt3)->z = (0.);}

/// UT3D_pt_pt2_z              3D-Point = 2D-Point + Z-Val
#define UT3D_pt_pt2_z(pt3,pt2,zVal){\
  memcpy((pt3), (pt2), sizeof(Point2)); (pt3)->z = (zVal);}

/// UT3D_pt_multpt                po = pi * d
#define UT3D_pt_multpt(po,pi,d){\
 (po)->x = (pi)->x * (d);\
 (po)->y = (pi)->y * (d);\
 (po)->z = (pi)->z * (d);}

/// \brief UT3D_pt_LinComb2Pts   Linear combination of two points:
/// \code
///                   po = a1 * p1 + a2 * p2
/// \endcode
#define UT3D_pt_LinComb2Pts(po,a1,p1,a2,p2){\
 (po)->x = a1 * (p1)->x + a2 * (p2)->x;\
 (po)->y = a1 * (p1)->y + a2 * (p2)->y;\
 (po)->z = a1 * (p1)->z + a2 * (p2)->z;}

/// UT3D_pt_addpt             Add point:      po += p1
#define UT3D_pt_addpt(po,p1){\
 (po)->x += (p1)->x;\
 (po)->y += (p1)->y;\
 (po)->z += (p1)->z;}

/// UT3D_pt_add_pt2             Add 2D-point:      po += p1
#define UT3D_pt_add_pt2(po,p1){\
 (po)->x += (p1)->x;\
 (po)->y += (p1)->y;}

/// UT3D_pt_add_vc__            add vector:  pt += vc
#define UT3D_pt_add_vc__(pt,vc){\
 (pt)->x += (vc)->dx;\
 (pt)->y += (vc)->dy;\
 (pt)->z += (vc)->dz;}

/// UT3D_pt_add_vc_rev          add vector:  pt -= vc
#define UT3D_pt_add_vc_rev(pt,vc){\
 (pt)->x -= (vc)->dx;\
 (pt)->y -= (vc)->dy;\
 (pt)->z -= (vc)->dz;}

/// UT3D_pt_add_vc_par        add (vector * lpar)  po += (vc * lpar)
#define UT3D_pt_add_vc_par(pt,vc,lpar){\
  (pt)->x += (vc)->dx * lpar;\
  (pt)->y += (vc)->dy * lpar;\
  (pt)->z += (vc)->dz * lpar;}
/// see also UT3D_pt_traptmultvc

/// UT3D_pt_add_3vc_3par        add (vector * lpar)  po += (vc * lpar)
/// - eg transport point (lp1/lp2/lp3) into refsys v1/v2/v3
#define UT3D_pt_add_3vc_3par(pt,v1,v2,v3,lp1,lp2,lp3){\
  (pt)->x += (v1)->dx * lp1 + (v2)->dx * lp2 + (v3)->dx * lp3;\
  (pt)->y += (v1)->dy * lp1 + (v2)->dy * lp2 + (v3)->dy * lp3;\
  (pt)->z += (v1)->dz * lp1 + (v2)->dz * lp2 + (v3)->dz * lp3;}
/// see also UT3D_pt_trapt3vc3len

/// UT3D_pt_add2pt   Add two points:               po = p1 + p2
#define UT3D_pt_add2pt(po,p1,p2){\
 (po)->x = (p1)->x + (p2)->x;\
 (po)->y = (p1)->y + (p2)->y;\
 (po)->z = (p1)->z + (p2)->z;}

/// UT3D_pt_sub_pt3             subtract point    po = p1 - p2
#define UT3D_pt_sub_pt3(pto,pti){\
 (pto)->x -= (pti)->x;\
 (pto)->y -= (pti)->y;\
 (pto)->z -= (pti)->z;}

/// UT3D_pt_sub_pt2             subtract 2D-point    po = p1 - p2
#define UT3D_pt_sub_pt2(po,p1,p2){\
 (po)->x = (p1)->x - (p2)->x;\
 (po)->y = (p1)->y - (p2)->y;}

/// \brief UT3D_swap2pt                  swap 2 3D-points
/// \code
/// see also MEM_swap_
/// \endcode
#define UT3D_swap2pt(p1,p2){\
  Point p3 = *(p1); *(p1) = *(p2); *(p2) = p3;}

/// UT3D_pt_vc                Point = Vector
#define UT3D_pt_vc(pt,vc) (memcpy((pt), (vc), sizeof(Point)))
// // p1 = UT3D_pt_vc (&vz);
// #define UT3D_pt_vc__(vc) *((Point*)vc)


/// UT3D_pt_tra_pt_dx         Point = Point + dx
#define UT3D_pt_tra_pt_dx(po,pi,dx)\
 *po = *pi; (po)->x += dx

/// UT3D_pt_tra_pt_dy         Point = Point + dy
#define UT3D_pt_tra_pt_dy(po,pi,dy)\
 *po = *pi; (po)->y += dy


/// UT3D_pt_traptvc               Point = Point + Vector
#define UT3D_pt_traptvc(po,pi,vc){\
 (po)->x = (pi)->x + (vc)->dx;\
 (po)->y = (pi)->y + (vc)->dy;\
 (po)->z = (pi)->z + (vc)->dz;}

/// UT3D_pt_traptivc               Point = Point - Vector
#define UT3D_pt_traptivc(po,pi,vc){\
 (po)->x = (pi)->x - (vc)->dx;\
 (po)->y = (pi)->y - (vc)->dy;\
 (po)->z = (pi)->z - (vc)->dz;}


/// UT3D_pt_traptmultvc       point = point + (vector * factor)   (INLINE)
#define UT3D_pt_traptmultvc(po,pi,vc,fac){\
 (po)->x = (pi)->x + (vc)->dx * fac; \
 (po)->y = (pi)->y + (vc)->dy * fac;\
 (po)->z = (pi)->z + (vc)->dz * fac;}


/// UT3D_pt__par_pt_vc          evaluate param.point on line from point+vector
#define UT3D_pt__par_pt_vc(pto,par,ptl,vcl){\
 (pto)->x = (ptl)->x + (vcl)->dx * (par);\
 (pto)->y = (ptl)->y + (vcl)->dy * (par);\
 (pto)->z = (ptl)->z + (vcl)->dz * (par);}


/// \brief u3d_LinComb2Pts    Linear combination of two points:
/// \code
///                   po = a1 * p1 + a2 * p2
/// \endcode
#define u3d_LinComb2Pts(po,a1,p1,a2,p2){\
 (po)->x = a1 * (p1)->x + a2 * (p2)->x;\
 (po)->y = a1 * (p1)->y + a2 * (p2)->y;\
 (po)->z = a1 * (p1)->z + a2 * (p2)->z;}



/// \brief UT3D_vc_isFree     check if vector is valid
/// \code
/// returns 0 if vector is valid (not free)
/// returns 1 if vector is free (after calling UT3D_vc_setFree)
/// \endcode
#define UT3D_vc_isFree(obj) (obj)->dx == UT_VAL_MAX


/// UT3D_vc_setFree           set vector not valid (not initialized)
#define UT3D_vc_setFree(obj) (obj)->dx = UT_VAL_MAX


/// UT3D_vc_vc2               3D-vector = 2D-vector
#define UT3D_vc_vc2(v3o,v2i){\
 (v3o)->dx = (v2i)->dx;\
 (v3o)->dy = (v2i)->dy;\
 (v3o)->dz = UT_DB_NULL;}

/// UT3D_vc_3db               Vector < 3 doubles (x, y, z)
#define UT3D_vc_3db(vc,x,y,z){\
 (vc)->dx = (x);\
 (vc)->dy = (y);\
 (vc)->dz = (z);}

/// UT3D_vc_pt                Vector = Point
#define UT3D_vc_pt(vc,pt) (memcpy((vc), (pt), sizeof(Point)))
// // v1 = UT3D_vc_pt (&pt);
// #define UT3D_vc_pt__(pt) *((Vector*)pt)

/// UT3D_vc_pt3db        Vector from point p1 to point from 3 doubles
void   UT3D_vc_pt3db (Vector*, Point*, double, double, double);
#define UT3D_vc_pt3db(vc,p1,px,py,pz) {\
 (vc)->dx = (px) - (p1)->x;\
 (vc)->dy = (py) - (p1)->y;\
 (vc)->dz = (pz) - (p1)->z;}

/// UT3D_vc_2pt               Vector = Point -> Point
#define UT3D_vc_2pt(vc,p1,p2){\
 (vc)->dx = (p2)->x - (p1)->x;\
 (vc)->dy = (p2)->y - (p1)->y;\
 (vc)->dz = (p2)->z - (p1)->z;}

/// UT3D_vc_ln                Vector = Line
void   UT3D_vc_ln (Vector*, Line*);
#define UT3D_vc_ln(vc,ln1){\
  (vc)->dx = (ln1)->p2.x - (ln1)->p1.x;\
  (vc)->dy = (ln1)->p2.y - (ln1)->p1.y;\
  (vc)->dz = (ln1)->p2.z - (ln1)->p1.z;}

/// UT3D_vc_perpTria          vector = perpendic. to Triang (crossprod)
#define UT3D_vc_perpTria(vn,tri)\
  UT3D_vc_perp3pt ((vn),(tri)->pa[0],(tri)->pa[1],(tri)->pa[2])

/// UT3D_vc_invert            invert Vector
#define UT3D_vc_invert(vio,vii){\
  (vio)->dx = -(vii)->dx;\
  (vio)->dy = -(vii)->dy;\
  (vio)->dz = -(vii)->dz;}

/// UT3D_vc_addvc                vo += vi
#define UT3D_vc_addvc(vo,vi){\
 (vo)->dx += (vi)->dx;\
 (vo)->dy += (vi)->dy;\
 (vo)->dz += (vi)->dz;}

/// UT3D_vc_add2vc                v3 = v1 + v2                ADD
#define UT3D_vc_add2vc(v3,v1,v2){\
 (v3)->dx = (v1)->dx + (v2)->dx;\
 (v3)->dy = (v1)->dy + (v2)->dy;\
 (v3)->dz = (v1)->dz + (v2)->dz;}

/// UT3D_vc_addvc2pt       v3 = v1 + (p2-p1)      Vector + (Vector from 2 points)
#define UT3D_vc_addvc2pt(v3,v1,p1,p2){\
 (v3)->dx = (v1)->dx + (p2)->x - (p1)->x;\
 (v3)->dy = (v1)->dy + (p2)->y - (p1)->y;\
 (v3)->dz = (v1)->dz + (p2)->z - (p1)->z;}

/// UT3D_vc_add3vc                vo = v1 + v2 + v3
#define UT3D_vc_add3vc(vo,v1,v2,v3){\
 (vo)->dx = (v1)->dx + (v2)->dx + (v3)->dx;\
 (vo)->dy = (v1)->dy + (v2)->dy + (v3)->dy;\
 (vo)->dz = (v1)->dz + (v2)->dz + (v3)->dz;}

/// UT3D_vc_sub2vc                v3 = v1 - v2                SUB
#define UT3D_vc_sub2vc(v3,v1,v2){\
 (v3)->dx = (v1)->dx - (v2)->dx;\
 (v3)->dy = (v1)->dy - (v2)->dy;\
 (v3)->dz = (v1)->dz - (v2)->dz;}

/// UT3D_vc_div_d             divide vector
void   UT3D_vc_div_d (Vector*, Vector*, double);
#define UT3D_vc_div_d(vo,vi,d){\
 (vo)->dx = (vi)->dx / (d);\
 (vo)->dy = (vi)->dy / (d);\
 (vo)->dz = (vi)->dz / (d);}

/// UT3D_vc_multvc                vo = vi * d                 MULT
void   UT3D_vc_multvc (Vector*, Vector*, double);
#define UT3D_vc_multvc(vo,vi,d){\
 (vo)->dx = (vi)->dx * (d);\
 (vo)->dy = (vi)->dy * (d);\
 (vo)->dz = (vi)->dz * (d);}

// UT3D_pt_tra_pt_vc_par          transl. point into dir vc; multiply vc
//   po = pi + (vi * par)
void   UT3D_pt_tra_pt_vc_par (Point *po, Point *pi, Vector *vi, double par);
#define UT3D_pt_tra_pt_vc_par(po,pi,vi,par){\
 (po)->x = (pi)->x + ((vi)->dx * (par));\
 (po)->y = (pi)->y + ((vi)->dy * (par));\
 (po)->z = (pi)->z + ((vi)->dz * (par));}

/// UT3D_vc_merge2vc              merge 2 vectors   vo = (v1 + v2) / 2
#define UT3D_vc_merge2vc(vo,v1,v2){\
 (vo)->dx = ((v1)->dx + (v2)->dx) / 2.;\
 (vo)->dy = ((v1)->dy + (v2)->dy) / 2.;\
 (vo)->dz = ((v1)->dz + (v2)->dz) / 2.;}



//----------------------------------------------------------------
/// UT3F_len_vc          length of 3D-vector
float UT3F_len_vc (Vec3f *);
#define UT3F_len_vc(vc)\
 (sqrtf((vc)->dx*(vc)->dx + (vc)->dy*(vc)->dy + (vc)->dz*(vc)->dz))


//----------------------------------------------------------------
/// \brief UT3D_comp2pt              compare 2 points with tolerance
/// \code
/// RC = 0: Punkte nicht gleich; Abstand > tol.
/// RC = 1: Punkte sind gleich; Abstand < tol.
/// Example:
///   if(UT3D_comp2pt(&pt1, &pt2, UT_TOL_pt) != 0) printf(" identical points\n");
/// see also UTP_comp2db
/// \endcode
int    UT3D_comp2pt (Point *, Point *, double);
#define UT3D_comp2pt(p1,p2,tol)\
  ((fabs((p2)->x - (p1)->x) < tol) &&\
   (fabs((p2)->y - (p1)->y) < tol) &&\
   (fabs((p2)->z - (p1)->z) < tol))


/// \brief UT3D_ck2D_equ_2pt          2D-compare of 2 3D-points with tolerance
/// \code
/// RC = 0: Punkte nicht gleich; Abstand > tol.
/// RC = 1: Punkte sind gleich; Abstand < tol.
/// Example:
///   if(UT3D_ck2D_equ_2pt(&pt1,&pt2,UT_TOL_pt) != 0) printf(" equal points\n");
/// \endcode
int    UT3D_ck2D_equ_2pt (Point*, Point*, double);
#define UT3D_ck2D_equ_2pt(p1,p2,tol)\
  ((fabs((p2)->x - (p1)->x) < tol) &&\
   (fabs((p2)->y - (p1)->y) < tol))


/// UT3D_ln_inv                Invert 3D-Line
#define UT3D_ln_inv(ln)\
  {Point _pt = (ln)->p1; (ln)->p1 = (ln)->p2; (ln)->p2 = _pt;}

/// UT3D_ln_ptpt                Line = Point, Point
#define UT3D_ln_ptpt(ln1,pt1,pt2)\
 {(ln1)->p1 = *(pt1);\
  (ln1)->p2 = *(pt2);}

/// UT3D_ln_ptvc                Line = Point, Vector
#define UT3D_ln_ptvc(ln,pt,vc)\
 {(ln)->p1 = *(pt);\
  (ln)->p2.x = (pt)->x + (vc)->dx;\
  (ln)->p2.y = (pt)->y + (vc)->dy;\
  (ln)->p2.z = (pt)->z + (vc)->dz;}

/// UT3D_ln_ln2                 3D-Line from 2D-Line (Z=0)
#define UT3D_ln_ln2(ln3,ln2)\
 {(ln3)->p1.x = (ln2)->p1.x;\
  (ln3)->p1.y = (ln2)->p1.y;\
  (ln3)->p1.z = UT_DB_NULL;\
  (ln3)->p2.x = (ln2)->p2.x;\
  (ln3)->p2.y = (ln2)->p2.y;\
  (ln3)->p2.z = UT_DB_NULL;}


//----------------------------------------------------------------
double UT3D_park__par1_clot (double cs, double ce, double par1);
#define UT3D_park__par1_clot UTP_vx_vMin_vMax_par1




//----------------------------------------------------------------
/*
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

/// UT3D_tria_fac          create triangle from Fac3
#define UT3D_tria_fac(tri,fac,pTab){\
 (tri)->pa[0] = &(pTab)[(fac)->i1];\
 (tri)->pa[1] = &(pTab)[(fac)->i2];\
 (tri)->pa[2] = &(pTab)[(fac)->i3];}
*/



//----------------------------------------------------------------
void ODB_set_odb (ObjDB *odb, int oTyp, long oDbi);
#define ODB_set_odb(odb,oTyp,oDbi){\
  (odb)->dbInd = (oDbi);\
  (odb)->dlInd = 0L;\
  (odb)->typ   = (oTyp);\
  (odb)->stat  = 0;}


//----------------------------------------------------------------
/// check if typ is a DB-object (); returns 0=no; 1=yes.
#define TYP_IS_DBO(typ) ((typ>Typ_Error)&&(typ<Typ_Val))

/// check if typ is a curve; returns 0=no; 1=yes.
/// curve is > Typ_CV but < Typ_PLN; not LN,CI
#define TYP_IS_CV(typ) ((typ>=Typ_CV)&&(typ<Typ_PLN))

// check if typ is a surface; returns 0=no; 1=yes.
#define TYP_IS_SUR(typ) ((typ>=Typ_SUR)&&(typ<Typ_SOL))

// check if typ is a solid; returns 0=no; 1=yes.
#define TYP_IS_SOL(typ) ((typ>=Typ_SOL)&&(typ<Typ_Note))

/// check if typ is a math.operator (+-/*); returns 0=no; 1=yes.
#define TYP_IS_OPM(typ) ((typ>=TYP_OpmPlus)&&(typ<Typ_FcmSQRT))

/// check if typ is a geom.parameter; returns 0=no; 1=yes.
///   TYP_IS_GEOMPAR include all types of values and parameters;
#define TYP_IS_GEOMPAR(typ) ((typ>=Typ_Val)&&(typ<Typ_Typ))

/// check if typ is a value; returns 0=no; 1=yes.
///   values: from Typ_Val to Typ_Par1 (with Angle ..)
#define TYP_IS_VAL(typ) ((typ>=Typ_Val)&&(typ<Typ_Par1))

/// check if typ is a distance; returns 0=no; 1=yes.
///   distance: from Typ_Val to Typ_Par1 (all *val; not Angle, Rad, )
#define TYP_IS_DIST(typ) ((typ>=Typ_Val)&&(typ<Typ_Angle))

/// check if typ is a function; returns 0=no; 1=yes.
#define TYP_IS_FNC(typ) ((typ>=TYP_FuncInit)&&(typ<Typ_ALL))

/// check if typ is a math.function; returns 0=no; 1=yes.
#define TYP_IS_FCM(typ) ((typ>=Typ_FcmSQRT)&&(typ<310))

/// check if typ is a modifier; returns 0=no; 1=yes.
///   modifiers: from Typ_modif to TYP_FuncInit
#define TYP_IS_MOD(typ) ((typ>=Typ_modif)&&(typ<TYP_FuncInit)||\
 (typ>=Typ_FncVAR1)&&(typ<Typ_EOT))




// CVTRM_set           set pointer to trimmed-curve from index
void* CVTRM_set (void* ccvTab, int cvSegNr);
#define CVTRM_set(ccvTab,cvSegNr) ccvTab + sizeof(CurvCCV) * cvSegNr

//----------------------------------------------------------------
/// check if typ is a selectionGroup; returns 0=no; 1=yes.
#define TYP_IS_SELGRP(typ) ((typ>=Typ_goGeom)&&(typ<Typ_FncVAR1))
// #define TYP_IS_SELGRP(typ) ((typ>=Typ_goGeom)&&(typ<Typ_FncVAR1)||\
 // (typ==Typ_lFig))

// see also DL_typ_is_visTyp

//----------------------------------------------------------------
#define UT_LEN_TNG (UT_DISP_ln / 25.)


// EOF
