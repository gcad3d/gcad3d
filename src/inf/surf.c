/* ../inf/surf.c

New Version of Typ_SUTP.  Not yet implemented.
- will use new tesselator MSH2D and have boolean surface-modifications.



================================================================== */
void INEW_surf__(){                   /*! \code

All surfaces have structure ObjGX - type Typ_SUTP; with -
  support-surface (eg plane for Planar-surf) and one -
  outer-boundary  (eg a link to a DB-curve) and (optional) inner boundaries.
  

INEW_surf_types


================================================================== \endcode */}
INEW_surf_types(){                   /*! \code

Trimmed,perforated-surf (SurTP)                          Typ_SUTP        INF_SUTP
  struct = ObjGX; 


Support-surfaces:
  Planar-surf         plane                              Typ_PLN
  Ruled-surf          line+circ/point,bspl (cone cyl ..) Typ_SURRU       INF_SURRU
  Revolved-surf       Sph Tor                            Typ_SURRV
  Sweep-surf          contour moving along a path        Typ_SURSWP
  Bspl-surf           splines along across               Typ_SURBSP


================================================================== \endcode */}
INF_SURRU(){                   /*! \code

  point+line, point+circ, point+bspl,
  line+line (planar or not planar), line+circ, line+bspl,
  circ+circ (nicht cyl, conic), circ+bspl,
  bspl+bspl




================================================================== \endcode */}
// eof
