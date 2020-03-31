/*     ../inf/geom.c

================================================================== */
void INF_geom(){                   /*! \code


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

- get all inner and outer contours
- get 2D-points for all contours
- all contours must be CCW

- make a gridbox for the support-surface (points defining quads)
  the gridbox must cover all contours

- tesselate OB, IB
- split at gridbox-points


MSH2D_tess__


================================================================== \endcode */}
void INF_Intersect_Surf(){                   /*! \code

Operations for surfaces: intersect
 - intersect surface - plane;
 - intersect all surfaces of 2 bodies

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


Functions:
MSHI_int_pln
  INT_nln_i2fac_pln1
  INT_npt_nln
  MSHI_split__

see myReadme.surfaces


================================================================== \endcode */}
void INF_Intersect_Body(){                   /*! \code

Operations for bodies: subtract join
- intersect all surfaces of 2 bodies
   result = 2 groups of surfaces (left/right of intersection-curve)
- Join:    join resulting surfaces (left + right) to new body
- Subract: join resulting surfaces (left + left) to new body



- intersect all surfaces of body

see INF_Intersect_Surf


================================================================== \endcode */}
void INF_Create_Body(){                   /*! \code

- simple-body = cylinder or sphere or ...
  - create boundaries + supporting-surfaces
- complex-body = list of surfaces

- store body in DB (body = list of connected surfaces)


================================================================== \endcode */}
// eof
