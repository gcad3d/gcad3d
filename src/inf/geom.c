/*     ../inf/geom.c

================================================================== */
void INF_geom(){                   /*! \code

INF_symbols            SYM_PLANE,SYM_AXIS,SYM_STAR_S, ..
INF_func_3D
INF_func_2D
INF_Intersect_Surf
INF_Intersect_Body
INF_Create_Body

INF_SUTP

INF_files__



================================================================== */}
void INF_func_3D(){                   /*! \code
3D Functions for:
Point Line Vector Circ Plane Mat_4x3 Mat_4x4:
../ut/ut_geo3d.c
../ut/ut_geo.h


================================================================== \endcode */}
void INF_func_2D(){                   /*! \code
2D Functions for:
Point Line Vector Circ Plane Mat_3x2
../ut/ut_geo2d.c
../ut/ut_geo.h



================================================================== \endcode */}
void INF_tess__(){                   /*! \code


Bspl-surfaces will be created from 2D;

For self-intersecting sweep-surfaces 2D-tesselation is too complex.
  - create direct 3D-faces.


- get all inner and outer contours
- get 2D-points for all contours
- all contours must be CCW

- make a gridbox for the support-surface (points defining quads)
  the gridbox must cover all contours

- tesselate OB, IB
- split at gridbox-points


MSH2D_tess__


================================================================== \endcode */}
void INF_surfaces(){                   /*! \code

Create surface:
  For trimmed,perforated surfaces create a supportsurface, the outer boudary
    all inner boundaries.
  (planar trimmed,perforated surface does not need supportsurface);

INF_FMTB_SURFACES    binary formatdescription
INF_surf_opers       boolean operations; CUT, BLEND, INTersect;
INF_MSH2D__          tesselate
INF_SURF_trim-punch-2D     
INF_SURF_trim-punch-3D
INF_surf_funcs___
INF_tess__
INF_Intersect_Surf
INF_Intersect_Body
INF_Create_Body




================================================================== \endcode */}
void INF_surf_opers(){                   /*! \code

 surf1     surf2      surf1,surf2        NOT           OR         AND
+---+     .   .      +---+          +---+        +---+        .   .
| . | .     +---+    | +-+-+        | +-+ .      | . +-+        +-+
| . | .     +---+    | +-+-+        | +-+ .      | . +-+        +-+
+---+     .   .      +---+          +---+        +---+        .   .


//================================================================
NOT:    (surf1 NOT surf2)
- surf3 = surf1 without parts of surf2 covering surf1
- start at first endpoint of intersect-Curves at OB of surf1
- L1: follow surf-1 fwd to next intersectionpoint with surf-2 ( = endPt of 
  a intersect-Curve) or endPt of curve or end contour
  - next intersectionpoint found: 
    - follow surf-2 bwd to next intersectionpoint ( = startPt of active
      intersect-Curve); goto L1.


//================================================================
OR:   (surf1 OR surf2)
- surf3 = all parts of surf1 and surf2 and also covered parts
- start at first endpoint of intersect-Curves at OB of surf1
- L1: follow OB of surf-1 fwd to next intersectionpoint with surf-2
  ( = endPt of a intersect-Curve) or endPt of curve or end contour
  - next intersectionpoint found: 
    - follow OB of surf-2 fwd to next intersectionpoint; goto L1.


//================================================================
AND:  (surf1 AND surf2)
- surf3 = only parts where surf1 is covering surf2
- start at first endpoint of intersect-Curves at OB of surf1
- L1: follow OB of surf-2 fwd to next intersectionpoint with surf-1
  ( = endPt of a intersect-Curve) or endPt of curve or end contour
  - next intersectionpoint found: 
    - follow surf-1 fwd to next intersectionpoint; goto L1.



- create intersect-Curves surf1 - surf2
- create points at all start- and endPoints of the intersect-Curves




================================================================== \endcode */}
void INF_SURF_trim-punch-3D(){                   /*! \code

3D-Trim-Punch-Operation:  (input mesh and analytical curve):
  - create n lineSegments from curve;
  - test every face of surface with all lines if -
    - if line is near enough or intersecting:
      cut face into 2 faces; remove inner part
    - continue with sequntial faces and line until closed or exiting.

Prerquisites: mesh must provide neighbour-faces.


================================================================== \endcode */}
void INF_SURF_trim-punch-2D(){                   /*! \code

- support-surf -> 2D
- tesselate
- outer boundary -> 2D
- inner boundaries -> 2D
- 2D-grid bestimmen
- included gridpoints mitvermehsen


================================================================== \endcode */}
void INF_Intersect_Surf(){                   /*! \code


Operations for surfaces: AND OR NOT  - see INF_surf_opers
 - intersect surface - surface;
 - intersect surface - plane;
 - intersect surface - curve;

- get Intersect-Surface-Curve as n polgons;
- change n polygons into n analytic curves (line|circle|conic|bspline)
- store n analytic curves n DB
- Rebuild the boundaries of the Intersect-Surface
- ReMesh the Intersect-Surface
- reBuild the bodies including this surface

see GLU - ../gr/ut_GLU.c:130

  ===============================================================
  create new surface by boolean operation split.
  - split = create new outer/inner contour(s)
  - loop tru all (old) boundary-segments of surface in actMsh
    - find intersection-points on old contour,
    - make new trimmed-curve of curve in actMsh
    - add new trimmed-curve to new boundary
    - add intersection-curve to new boundary
    - continue with next old boundary-segments


- intersect 2 surfaces
   result = 2 groups of surfaces (left/right of intersection-curve)
- AND:     ??
- OR:      join resulting surfaces (left + right) to new body
- NOT:     join resulting surfaces (left + left) to new body



Functions:
MSHI_int_pln
  INT_nln_nfac_pln2
  INT_nln_nfac_pln1
  MSHI_split__

see myReadme.surfaces


================================================================== \endcode */}
void INF_Intersect_Body(){                   /*! \code

 - intersect all surfaces of 2 bodies

Operations for bodies: AND OR NOT
- intersect all surfaces of 2 bodies

see INF_Intersect_Surf


================================================================== \endcode */}
void INF_Create_Body(){                   /*! \code

- simple-body = cylinder or sphere or ...
  - create boundaries + supporting-surfaces
- complex-body = list of surfaces


- assemble body from surfaces  (module BDAS)
       in:  surfaces
       out: body

- store body in DB (body = list of connected surfaces)


================================================================== \endcode */}
void INF_symbols(){                   /*! \code
INF_symbols            SYM_PLANE,SYM_AXIS,SYM_STAR_S, ..

- symbols have always same size;

//----------------------------------------------------------------
- Vectorsymbols:
  - can scale, can rotate (2D) or orient (3D, 2 angles)
  SYM_ARRO3H    3D-arrowHead
  SYM_VEC       vector (line)
  SYM_AXIS      with chars x and z
  SYM_AXIS1
  SYM_SQUARE
  SYM_PLANE
  SYM_CROSS
  SYM_TRIANG
  SYM_CROSS1   scissors
  SYM_ARROH    2D-arrowHead
  SYM_ARROW    arrow ?
  SYM_LENGTH  true length, no scaleBack


//----------------------------------------------------------------
- Bitmapsymbols:
  - cannot scale, cannot be rotated, oriented;
  SYM_TRI_S|SYM_STAR_S|SYM_CIR_S|SYM_SQU_S|SYM_TRI_B|SYM_SQU_B



================================================================== \endcode */}
// eof
