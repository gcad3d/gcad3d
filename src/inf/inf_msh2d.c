/*   ../inf/inf_msh2d.c

see test_MSH2D.mak


What is MSH2D
- mesh a 2D-contour;
- tesselate contours - outer-boundary and inner-boundaries (holes)
    (create triangles inside outer-boundary; around inner-boundaries)

Features of MSH2D
- 2D only; no vertical (zero-area) triangles
- does not modify contours (no points inserted or deleted)
- for indexed pointsets only
- identical-points-tolerance respected
- provides list of additional created gridpoints (inside boundary)
  (if grid-size and -position is defined)
- optional: optimize (makes triangles less acute)
- TODO: add internal breaklines (contours not closed, eg roadsides)


Functions:
MSH2D_tess__
  in:  grid (x-width, y-width); 2D-boundaries (OB, IB)
  out: a 2D-mesh - struct ?



===================================================================
MSH2D_tess__()
- make gridbox - points covering whole OB (points in rows, columns)
- mesh gridbox (MSH2D_quad__)

  MSH2D_bnd__()
  - mesh all contoursegments
  - first contour (OB) makes breaklines in the gridbox;
    - should keep all gridpoints as edgepoints
    - first mesh the endpoints of the line (MSH2D_pt__)
    - then mesh the breakline (MSH2D_brkl__)
    - after OB is finished  all faces outside OB become removed (MSH2D_fac_del__)



===================================================================
Input to MSH2D:
- the support-surface as a 2D-GridBox (startpoint,uMin,uMax,vMin,vMax,uNr,vNr)
- boundaries (outer and inner) -


MSH2D creates the 2D-mesh -
- all points of the support-surface inside the boundary will be integrated 
  into the resulting mesh,
- the faces will not be broken at the connecting lines of the tiles
  of the support-surface. So all points of the input-boundaries must be 
  precise at the support-surface.


Output of MSH2D:
- double-indexed faces (Fac3)
- table of neighbour-faces


Userfunctions:
  MSH2D_tess__   MSH2D_*


Testprog:
  test_MSH2D.mak   ../xa/test_MSH2D.c   see myReadme.tess


//----------------------------------------------------------------
 TessStru - see ../APP/tst_msh2d.h

Boundaries storage:
  (TessStru).tab.rNr  - the nr of boundaries; every tab-record is a boundary.
    ibeg, iNr - the startindex and the number of indices of the polygon-points
      in (TessStru).ipa.
    typi - type of boundary: MSH_EDGLN_OB = 4 = outer, MSH_EDGLN_IB = 3 = inner
 

After tesselation:
- new Record (TessStru).tab added:
  (TessStru).tab.typi =  MSH_GRIDBOX = 8
  (TessStru).tab.stat 0=point unused, 1=point is inside surface and used




//================================================================
================================================================== */
void INF_MSH2D__ (){        /*! \code

 


//----------------------------------------------------------------
typedef struct { MemTab   tab;      // IndTab boundaries, tesselated patches
                 MemTab   pst;      // char   boundary-flags
                 MemTab   ipa;      // int    index to points
                 MemTab   seg;      // int    nr of points of segments of boundary
                 MemTab   pa2;      // Point2 2D-points
                 MemTab   pa3;      // Point  3D-points                   << -> MSH3D ?
                 MemTab   vc3;      // Vec3f  normalVectors               << -> MSH3D ?
                 MemTab   fac;      // Fac3   indexed faces
                 MemTab   fnb;      // Fac3   neighbour-faces
                 GridBox  gbx;
                 double   tol; }                                MshDat;

.tab
  IndTab boundaries (OB and IB's
  MSH2D_dump__("oi"

.ipa
  int    index to points (into pa2); data for IndTab's in .tab;
  MSH2D_dump__("oi"

.pst
  char   points-status; parallel pa2
  MSH2D_dump__("p"

.pa2
  Point2 - all 2D-points
  MSH2D_dump__("p"

.fac
  Fac3   indexed faces
  UFA_fnb_dump__

.fnb
  Fac3   neighbour-faces
  UFA_fnb_dump__

.gbx
  GridBox
  DEB_dump_obj__ (Typ_GridBox,


test_msh2d_1
  MSH2D_tess__
    MSH2D_init__
      MSH2D_init_qbx          // get box around OB
      MSH2D_iqxMax=0 MSH2D_iqyMax=0            // ???

- dump gbx:
    DEB_dump_obj__ (Typ_GridBox, &MSH2D_qbx, "ex-MSH2D_init__");
- dump fac and fnb:
    UFA_fnb_dump__ (ts1->fac.data, ts1->fnb.data, ts1->fac.rNr, " _tess__-S2");



//----------------------------------------------------------------
MSH2D_tess__                            // tesselate DB-surf
  MSH2D_bnd__                           // tess bounds in basic quad
    MSH2D_brkl__                        // set OB-segment as breakline
      MSH2D_brkl_bnd__                  // edges crossing ips-ipe ?
        MSH2D_brkl_bnd_1_               // find first edge crossing line ips-ipe
          UT2D_2pt_ck_int_2pt

 

================================================================== \endcode */}
// eof
