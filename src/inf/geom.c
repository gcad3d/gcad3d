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
                                     
                                     surf3       surf3        surf3
 surf1     surf2      surf1,surf2     NOT          OR          AND
+---+     .   .      +---+          +---+        +---+        .   .
| . | .     +---+    | +-+-+        | +-+ .      | . +-+        +-+
| . | .     +---+    | +-+-+        | +-+ .      | . +-+        +-+
+---+     .   .      +---+          +---+        +---+        .   .


NOT: new-surfs = in area of su1 not covered by su2;
- new-OB is area of su1 excluding area su2
- new-OB is IB of su2 in area covered by su1 and also covered by su2


OR: areas of su1 and su2 or both;


AND: only areas where su1 covers su2;



OPE = operation - NOT or OR or NAD
IPT = intersectionpoint on surf1 and surf2
FWD = following CCW the active boundary-segment
REV = following CW the active boundary-segment
OB1 = outerBoundary = first boundary of surf1
OB2 = outerBoundary = first boundary of surf2
IB1 = innerBoundary = boundaries following OB1, hole(s) in surf1
IB2 = innerBoundary = boundaries following OB2, hole(s) in surf2
SUI = surface-index; 1=sur1, 2=sur2
BDI1 = boundary-index on surf1; 0=outerBoundary; 1-n=innerBoundary
BDI2 = boundary-index on surf2; 0=outerBoundary; 1-n=innerBoundary
ACTPT = active point, startpoint of a new output-segment
NXTPT = endpoint of a new output-segment

FWD  = follow boundary in same direction = CCW
BWD  = follow boundary in same direction = CCW

O1   = outerBoundary of surf1 (surf to modify)
I1   = innerBoundary of surf1 (0 - n inner boundaries)
O2   = outerBoundary of surf2 (modifying (cutting) surface)
I2   = innerBoundary of surf2 (0 - n inner boundaries)

Grafic:
X  startpoint boundary surf1, surf2
x  intersectionPoint and startpoint new boundary surf3
+  intersectionPoint

For all opTypes:                                              | SID_CCW
------ get intersectionpoints IP# and side SI#;               |
  SID_CCW if intersection goes to the left side;  0      >----x------
  SID_CW  if intersection goes to the right side; 1           |
                                                              | SID_CW

Intersectiontypes:
 O2_O1   is intersection of OB (outerBoundary) of surf1 with OB of surf2;
 - coming on O1 (OB surf1) turning into O2 - following O2.


Find startpoints:
  - for intersections I X x get point with SI0
  - for intersections O X x get point with SI1

//================================================================
NOT:    (surf1 NOT surf2)    (FSUB, CUT)
- surf3 = surf1 without parts of surf2 covering surf1
- ignore IBs of surf2

      O1
 X------------------------------------------------o
 V   X    X        X         X       X       X    |
 |      X   I1          X             sur3        |
 |   X--------------------------.            X   /\
 |X  V                          |D       X       A|
 |   |   X-----------O2---------3->---------------1-----------o
 |   |   V                      |                 |           |
 |X  |   |                      |                 |           |
 |   |   |                      |                 |           |
 |   o---2----------I1----------o                 |           |
 |      C|                                        |           |
 |X  X   |                                        |           |
 |X  X  /\                                        |           |
 |      B|                                        |           |
 o-------0-------------------O1-------------------o           |
         |                                                    |
         |                                                    |
         o-------------------------O2-------------------------o
                                                 
- find first intersectionpoint along O1 X O2 with SID_CCW
O2 X O1: SID_CCW, find point B;

B  DO2_O1  BWD following O2 at intersectionPoint O2 X O1
C  DI1_O2  BWD following I1 at intersectionPoint I1 X O2
D  DO2_I1  BWD following O2 at intersectionPoint O2 X I1
A  DO1_O2  FWD following O1 at intersectionPoint O1 X O2



------------------------------------------------------------------
NOT - no intersection O1 X O2

      O1
 X------------------------------------------------o
 V                                                |
 |          I1                                    |
 |   X--------------------------.                 |
 |   V                          |D                |
 |   |   X-----------O2---------1->--------o      |
 |   |   V                      |          |      | 
 |   |   |                      |          |      |
 |   |   |                      |          |      |
 |   o-<-0----------I1----------o          |      |
 |      C|                                 |      |
 |       |                                 |      |
 |       o-------------------------O2------o      |
 |                                                |
 |                                                |
 o---------------------------O1-------------------o
                                                  
                                                 
O1 X O2: - find SI1
I1 X O2: - find SI1

A  DO1_O2  FWD following O1 at intersectionPoint O1 X O2
B  DO2_O1  BWD following O2 at intersectionPoint O2 X O1

C  DI1_O2  BWD following I1 at intersectionPoint I1 X O2
D  DO2_I1  BWD following O2 at intersectionPoint O2 X I1

- find startpoint O1 X O2 = first intersectionpoint along O1 with SI1 = none
- find startpoint I1 X O2 = first intersectionpoint along I1 with SI1 = IP1
  - start along O2
  at IP1  D=DO2_I1  BWD  to IP0
  at IP0  C=DI1_O2  BWD  to IP1; endOfBnd. No more IPs.





//================================================================
OR:   (surf1 OR surf2)   
//================================================================
- surf3 = all parts of surf1 and surf2 and also covered parts

      sur1-OB
 X---------------------------------O1-------------o
 V                                                |
 |     sur1-IB                                    |
 |   X--------------------------o                 |
 |   V                          |                 |
 |   |    sur3-IB2              |                 /\
 |   |                         D|                 |A           
 |   |       X----------------<-3----O2-----------1-----------o
 |   |       V                  |                 |           |
 |   |       |                  |                 |           |
 |   |       |                  |                 |           |
 |   |       |        X-------<-6---I2------------5------o    |
 |   |       |        V        F|                 |G     |    |
 |   |       |        |   s3/3  |                 v      |    |
 |   |      C|        |E        |                 |      |    |
 |   o-----<-2--------7->--I1---o                 |      |    |
 |           |        |                          O1     I2    O2
 |           |        |                           |      |    |
 O1          O2       I2                          |      |    |
 |           |        |                           |      |    |
 |           |        |                           |      |    |
 o-----------0--------4------------O1-------------o      |    |
            B|        |H                                 |    |
             v        v                   sur3-IB1       |    |
             |        o------------I2--------------------o    |
             |                                 sur2-IB        |
             |                                                |
             o------------------------------------------------o
                                               sur2-OB

O1 X O2:  find SI1
  IP0 SI1
  IP1 SI0
O1 X I2:  find SI1
  IP4 SI1
  IP5 SI0
I1 X O2:  find SI0
  IP2 SI1
  IP3 SI0
I1 X I2:  find SI0
  IP7 SI1
  IP6 SI0

A  DO1_O2  FWD   following O1 at intersectionPoint O1 X O2
B  DO2_O1  FWD   following O2 at intersectionPoint O2 X O1

C  DI1_O2  BWD   following I1 at intersectionPoint I1 X O2
D  DO2_I1  FWD   following O2 at intersectionPoint O2 X I1

E  DI1_I2  FWD   following I1 at intersectionPoint I1 X I2
F  DI2_I1  FWD   following I2 at intersectionPoint I2 X I1

G  DO1_I2  BWD   following O1 at intersectionPoint I1 X I2
H  DI2_O1  FWD   following I2 at intersectionPoint O2 X O1

- find startpoint O1 X O2 = first intersectionpoint along O1 with SI1 = IP0
  - start along O2
  at IP0; B=DO2_O1 to IP1;
  at IP1; A=DO1_O2, O1 finished; sur3-OB out.

- find startpoint O1 X I2 = first intersectionpoint along O1 with SI1 = IP4      <<<<<<
  - start along I2
  at IP4; H=DI2_O1 to IP5;
  at IP5; G=DO1_I2, sur3-IB1 out.

- find startpoint I1 X O2 = first intersectionpoint along I1 with SI0 = IP3
  - start along O2
  at IP3; D=DO2_I1 to IP2;
  at IP2; C=DI1_O2, sur3-IB2 finished.

- find startpoint I1 X I2 = first intersectionpoint along I2 with SI0 = IP6
  - start along I2
  at IP6; F=DI2_I1 to IP7;
  at IP7; E=DI1_I2, sur3-IB3 finished.


// - start at first endpoint of intersect-Curves at OB of surf1
// - L1: follow OB of surf-1 fwd to next intersectionpoint with surf-2
//   ( = endPt of a intersect-Curve) or endPt of curve or end contour
//   - next intersectionpoint found: 
//     - follow OB of surf-2 fwd to next intersectionpoint; goto L1.


//================================================================
AND:  (surf1 AND surf2)
//================================================================
- surf3 = only parts where surf1 is covering surf2
- start at first endpoint of intersect-Curves at OB of surf1
- L1: follow OB of surf-2 fwd to next intersectionpoint with surf-1
  ( = endPt of a intersect-Curve) or endPt of curve or end contour
  - next intersectionpoint found: 
    - follow surf-1 fwd to next intersectionpoint; goto L1.



- create intersect-Curves surf1 - surf2
- create points at all start- and endPoints of the intersect-Curves



             
 X-------------------O1---------------------------o
 V                                                |
 |                                                |
 |   X--------I1----------------.                 |
 |   V                          |                 |
 |   |   X----------O2----------5---------------<-1-----------o
 |   |   V                      |C               D|           |
 |   |   |                      V                 |           |
 |   |   |                      |     sur3/2      |           |
 |   |   |                      |                /\           |
 |   |   |                      |F               G|           |
 |   |   |                X-----7->------I2-------3------o    |
 |   |   |                V     |                 |      |    |
 |   |   |                |     |                 |      |    |
 |   |   |                |     |                 |      |    |
 |   o---4------I1------<-6-----o                 |      |    |
 |       |D              E|                       |      |    |
 |       V                |                       |      |    |
 |       |    sur3/1      |                       |      |    |
 |       |               /\                       |      |    |
 |       |A              H|                       |      |    |
 o-------0->--------------2-----O1----------------o      |    |
         |                |                              |    |
         |                |                              |    |
         |                o--------------I2--------------o    |
         |                                                    |
         |                                                    |
         o-----------------------O2---------------------------o
                                                      

O1 X O2:  find SI0
  IP0 SI1
  IP1 SI0
O1 X I2:  find SI1
  IP2 SI1
  IP3 SI0
I1 X O2:  find SI0
  IP4 SI1
  IP5 SI0
I1 X I2:  find SI0
  IP6 SI1
  IP7 SI0

A=DO1_O2 FWD   following O1 at intersectionPoint O1 X O2
B=DO2_O1  WD   following O2 at intersectionPoint O2 X O1

C=DI1_O2 BWD   following I1 at intersectionPoint I1 X O2
D=DO2_I1 FWD   following O2 at intersectionPoint O2 X I1

E=DI1_I2 BWD   following I1 at intersectionPoint I1 X I2
F=DI2_I1 FWD   following I2 at intersectionPoint I2 X I1

G=DO1_I2 FWD   following O1 at intersectionPoint I1 X I2
H=DI2_O1 BWD   following I2 at intersectionPoint O2 X O1


- find startpoint O1 X O2 = first intersectionpoint along O1 with SI0 = IP1
  - start along O2
  at IP1  D=DO2_O1 FWD to IP5
  at IP5  C=DI1_O2 BWD to IP7
  at IP7  F=DI2_I1 FWD to IP3
  at IP3  G=DO1_I2 FWD to IP1 - finish; out as O3/1.

- find startpoint O1 X I2 = first intersectionpoint along O1 with SI1 = IP2
  - start along I2
  at IP2  H=DI2_O1 BWD to IP6
  at IP6  E=DI1_I2 BWD to IP4
  at IP4  D=DO2_I1 FWD to IP0
  at IP0  A=DO1_O2 FWD to IP2 - finish; out as O3/2.



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
