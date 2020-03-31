/*   ../inf/inf_msh2d.c

see test_MSH2D.mak

================================================================== */
void INF_MSH2D__ (){        /*! \code


What is MSH2D
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





================================================================== \endcode */}
// eof
