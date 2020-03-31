/*   ../inf/Objects-Format.c

Dokumentation binary formats gcad3d

Update tag-file:
ctags -f ../tags/Objects-Format.tag ../inf/Objects-Format.c


TODO:  translate into english ..



================================================================== */
void INF_FMTB__ (){                   /*! \code

INF_FMTB__                            Gcad-Format-binary

Definition of Datenstructs see ../ut/ut_geo.h.

Es gibt typenspezifische Strukturen -
  (Point, Line, Circ, Vector, CurvPoly, CurvBSpl ..)

Es gibt ein Containerobjekt: die struct ObjGX.

INF_FMTB_Vector
INF_FMTB_Point
INF_FMTB_Line
INF_FMTB_Circle
INF_FMTB_Plane

INF_FMTB_NOTES
  INF_FMTB_Bitmaps                    AText Text-2D, Image, tag, balloon, symbols
  INF_FMTB_Text_3D                    GText
  INF_FMTB_Dimensions                 Dimen
  INF_FMTB_Dimensions_3D              Dim3

INF_FMTB_CURVES
  INF_FMTB_Ellipse
  INF_FMTB_Curve_Polygon
  INF_FMTB_Curve_Clot                 Clothoid-curve
  INF_FMTB_Curve_Splines
  INF_FMTB_Curve_BSpl                 B-spline-curve
  INF_FMTB_Curve_RBSpl                Rational-B-spline-curve
  INF_FMTB_Curve_Bez                  Bezier-curve
  INF_FMTB_Curve_RBez                 Rational-Bezier-curve
  INF_FMTB_Curve_CCV                  trimmed-curve, Contour

INF_FMTB_SURFACES
  INF_FMTB_Surface_TPS                trimmed, perforated, supported surface
  INF_FMTB_Surface_PLN                OBSOLETE; DO NOT USE
  INF_FMTB_Surface_RU                 ruled
  INF_FMTB_Surface_RV                 revolved (spherical)
  INF_FMTB_Surface_SWP                Sweep-Surface (contour moving along a path)
  INF_FMTB_Surface_BSP                B-Spline-Surface, loft surface
  INF_FMTB_Surface_RBSP               Rational-B-Spline-Surface
  INF_FMTB_Surface_PTAB               points
  INF_FMTB_Surface_MSH                mesh
  INF_FMTB_Surface_GL_Sur             tesselated patches
  INF_FMTB_Surface_RCIR               tesselated fan
  INF_FMTB_Surface_RSTRIP             tesselated stripe

INF_FMTB_Bodies
  INF_FMTB_Body_Sphere
  INF_FMTB_Body_Cone
  INF_FMTB_Body_Torus
  INF_FMTB_Body_PRISM
  INF_FMTB_Body_BREP

INF_FMTB_ComplexObject
INF_FMTB_Models
INF_FMTB_BinaryMesh
INF_FMTB_Transformation





================================================================== \endcode */}
void INF_FMTB_ComplexObject (){        /*! \code

In der DB werden alle Kurven und Flaechen (S,A) als
  ObjGX-Strukturen gespeichert (retourniert);
  Punkte, Linien, Kreise, Vektoren .. werden direkt (mit ihrer
  typenspezifischen Struktur) gespeichert und retourniert.




form = ObjGX
typ = Typ_ObjGX

Inhalte einer struct ObjGX:

  typ    die Bedeutung der Daten im Feld <data>
  form   die Struktur der Daten im Feld <data>
         Typ_PT: data ist (Point*).
         Typ_Index: data ist keine Adresse, sondern direkt die Zahl (long)
         Typ_Int4: data ist keine Adresse, sondern direkt die Zahl (long)
  siz    Anzahl der Datenrecords in data
  data   die Daten als struct vom typ <form> Anzahl <siz>
         z.B: siz=10; form=Typ_PT: data=(Point*)PointArray[10]
         
 
Beispiele ObjGX:
  typenspezifische DB-Speicherbereiche:
    .typ=Typ_PT;    .form=Typ_Index; .siz=1;     .data=(int)DB-index_of_obj
    .typ=Typ_LN;    .form=Typ_LN;    .siz=1;     .data=address_of_(Line*)
    .typ=Typ_CVPLG; .form=Typ_CVPLG; .siz=1;     .data=(CurvPoly*)
    .typ=Typ_CVTRM; .form=Typ_CVTRM; .siz=3;     .data=(CurvCCV[3])

  keine typenspezifischen DB-Speicherbereiche:
    .typ=Typ_Typ;  .form=Typ_Int4;  .siz=1;            .data=(int)typ


Laden mit DB-Typ und DB-Index:
  statt:
    ox.form  = Typ_Index;
    ox.typ   = dbTyp;
    ox->data = (void*)dbInd;
  besser (inline):
OGX_SET_INDEX (dbTyp, dbInd, ox);


Functions see Get_ComplexObject


Funktionen:
  DB_store_ox
  DB_store_obj
  UTO_isol__         duplicate/isolate object (resolve Links)
  UTRA_app_oTab
  
  UTO_dump__
  UTO_dump_s_

  UTO_ck_oTyp        is typ a data-struct or a complexObject
  UTO_siz_stru       get size of struct
  AP_typDB_typ    get groupCode 

UTO_rec_set             einen Record schreiben
UTO_copy_0              duplicate obj (oGX-tree; keep data-pointers)
UTO_reloc_obj           relocate obj (oGX-tree)

UTO_copy_tab            ObjGX-Table (rekursiv) kopieren
UTO_copy_stru           copy struct (ohne resolve - nicht recursiv)
UTO_obj_save            Obj (nur struct) speichern (copy -> Memspc)
UTO_obj_2_mem           Ein Obj seriell in einen Speicherbereich ablegen.




================================================================== \endcode */}
void INF_FMTB_Vector (){        /*! \code

\ref ut_geo_const.h               Constant Vectors




Examples:
  Vector    v1={1.0, 0.0, 0.0};           // Typ_VC

  UT3D_vc_3db

  GR_Disp_vc




================================================================== \endcode */}
void INF_FMTB_Point (){        /*! \code


------------------
2D-Point
------------------

Point2  pt2D;
  pt2D.x     = (double) X-Coord
  pt2D.y     = (double) Y-Coord

ObjGX  ox1;
  ox1.typ   = Typ_PT
  ox1.form  = Typ_PT2
  ox1.siz   = 1
  ox1.data  = (void*)&pt2D



------------------
3D-Point
------------------

Point   pt3D;
  pt3D.x     = (double) X-Coord
  pt3D.y     = (double) Y-Coord
  pt3D.z     = (double) Z-Coord

ObjGX  ox1;
  ox1.typ   = Typ_PT
  ox1.form  = Typ_PT
  ox1.siz   = 1
  ox1.data  = (void*)&pt3D


oder:
  ox1.typ   = Typ_PT
  ox1.form  = Typ_Index
  ox1.siz   = 1
  ox1.data  = (void*)(long)DB_index;


oder:
  ox1.typ   = Typ_PT
  ox1.form  = Typ_Txt
  ox1.siz   = 1
  ox1.data  = (void*)"P(0 0)"



Examples:
  Point     pt1;        // Typ_PT

  Point     pt1={1.0, 0.0, 0.0};

  UT3D_pt_3db

  GL_DrawPoint (&dli, 0, pt1);

see also
  GL_DrawSymB (&dli, 2, SYM_TRI_S, p1);    // red triangle




================================================================== \endcode */}
void INF_FMTB_Line (){        /*! \code



------------------
2D-Line
------------------

Line2  ln2D;
  ln2D.p1   = (Point2) Startpunkt
  ln2D.p2   = (Point2) Endpunkt

ObjGX  ox1;
  ox1.typ   = Typ_LN
  ox1.form  = Typ_LN2
  ox1.siz   = 1
  ox1.data  = (void*)&ln2D




------------------
3D-Line
------------------

Line   ln3D;
  ln3D.p1   = (Point) Startpunkt
  ln3D.p2   = (Point) Endpunkt


ObjGX  ox1;
  ox1.typ   = Typ_LN
  ox1.form  = Typ_LN
  ox1.siz   = 1
  ox1.data  = (void*)&ln3D




================================================================== \endcode */}
void INF_FMTB_Circle (){        /*! \code


------------------
2D-Circle
------------------

Circ2  ci2D;
  ci2D.p1   = (Point2) Startpunkt
  ci2D.p2   = (Point2) Endpunkt
  ci2D.pc   = (Point2) centerpoint
  ci2D.rad  = (double) Radius; positive for CCW, negative for CW.

ObjGX  ox1;
  ox1.typ   = Typ_CI
  ox1.form  = Typ_CI2
  ox1.siz   = 1
  ox1.data  = (void*)&ci2D






------------------
3D-Circle
------------------
Circ   ci3D;
  ci3D.p1   = (Point) Startpunkt
  ci3D.p2   = (Point) Endpunkt
  ci3D.pc   = (Point) centerpoint
  ci3D.rad  = (double) Radius; positive for CCW, negative for CW.
  ci3D.vz   = (Vector) axis direction

// size = 104


ObjGX  ox1;
  ox1.typ   = Typ_CI
  ox1.form  = Typ_CI
  ox1.siz   = 1
  ox1.data  = (void*)&ci3D







================================================================== \endcode */}
void INF_FMTB_Curve_Clot (){        /*! \code
 Clothoid-curve

Typ_CVCLOT  CurvClot 
 
\endcode Source: ../ut/cvClot.c  

Functions:
  UT3D_ptvc_evparclot              // point from parameter


================================================================== \endcode */}
void INF_FMTB_Ellipse (){        /*! \code


------------------
3D-Ellipse
------------------

CurvElli ell1;
  ell1.p1   = (Point) Startpunkt
  ell1.p2   = (Point) Endpunkt
  ell1.pc   = (Point) Mittelpunkt
  ell1.vz   = (Vector) Z-Achsrichtung
  ell1.va   = (Vector) Richtung und Groesse Hauptachse
  ell1.vb   = (Vector) Richtung und Groesse Nebenachse

// Typ  = Typ_CVELL
// size = 148 Byte


\endcode SourceFiles:
 ../ut/ut_elli.c

Functions:
  UT3D_ck_el360
  UT3D_ptvc_eval_ell_par    get point/tangentVector on ellipse from parameter


================================================================== \endcode */}
void INF_FMTB_Plane (){        /*! \code


------------------
Plane
------------------
Plane   pln;
  pln.po   = (Point)  Nullpunkt
  pln.vx   = (Vector) X-Vektor
  pln.vy   = (Vector) Y-Vektor
  pln.vz   = (Vector) Z-Vektor
  pln.p    = (double) Achsrichtung

ObjGX  ox1;
  ox1.typ   = Typ_PLN
  ox1.form  = Typ_PLN
  ox1.siz   = 1
  ox1.data  = (void*)&pln


Examples:
  Plane     pln1;    // Typ_PLN

  UT3D_pl_ptvc

  GR_Disp_pln
  GL_DrawSymV3

  int      i1, ips, lnAtt, iTyp;
  long     dli;
  double   scl;
  Point    *pto;
  Vector   *vcn;
  Plane    pl1;

  // plane from oroginPoint & normalVector
  UT3D_pl_ptvc (&pl1, &pto, &UT3D_VECTOR_X);


  // display
  lnAtt = 9;          // lineAttribute, see DL_InitAttRec
  iTyp = 1;           // 1=Plane; 2=Axis; 4=Axis+Chars; 5=Plane+Axis;
  scl = 1.;
  GL_DrawSymVX (&dli, lnAtt, &pl1, iTyp, scl);


  // create a complexObj
  OGX_SET_OBJ (&ox1, Typ_PLN, Typ_PLN, 1, &pl1);



================================================================== \endcode */}
void INF_FMTB_NOTES (){        /*! \code

text, images, dimensions


================================================================== \endcode */}
void INF_FMTB_Text_3D (){        /*! \code

3D-Text

SourceObj:
N31=P(40 35 0) "3D-Text;[nZeile2.[nZeile3."


ObjGX (typ=Typ_Note; form=Typ_GTXT; data=(GText*))


GText     tx1
  tx1.pt    = (Point) der linke untere Startpunkt
  tx1.size  = (double) Texthoehe; bei Bitmap-Notes -1; Defaulthoehe: 0.
  tx1.dir   = (double) Richtung; horizontal = 0.
  txt1.txt  = (char*)




Functions:
  APT_DrawTxtG
  UT3D_box_GText


================================================================== \endcode */}
void INF_FMTB_Dimensions (){        /*! \code

Dimensions:

SourceObj:
P3=P(-110 20)
P4=P(-65 10)
N14=DIM P3 P4 P(-90 -10) ANG(0) "[+0.5"

ObjGX (typ=Typ_Note; form=Typ_Dimen; data=(Dimen*))


Dimen dim1
  dim1.p1   = (Point) Maszpunkt 1
  dim1.p2   = (Point) Maszpunkt 2
  dim1.p3   = (Point) Textposition
  dim1.a1   = (float) Winkel Maszlinie/ Textblock in Degree
  dim1.a2   = (float)
  dim1.dtyp = (char)
  dim1.hd   = (char) heads: 0=none, 1=<, 2=>, 3=/, 4=X; Default is 12
  dim1.ld   = (char) lines: 0=none, 1=line; Default is 11
  dim1.txt  = (char*) Zusatztext


  LinearMasz:
    dtyp = 0
    p1 = Maszpunkt 1
    p2 = Maszpunkt 2
    p3 = Textposition
    a1 = Winkel Maszlinie/ Textblock in Grad
    hd, ld (Def = 12 11)
    txt


  DurchmesserMasz:
    dtyp = 1
    hd < 10 = Radiusmasz; else Durchmessermasz.
    p1 = Kreis-Center
    p2 = Punkt am Kreis
    p3 = Textposition
    a1 = Winkel Maszlinie/ Textblock in Grad
    a2 = Radius
    hd   (Def = 12)
    txt


  RadiusMasz:
    dtyp = 2
    hd < 10 = Radiusmasz; else Durchmessermasz.
    p1 = Kreis-Center
    p2 = Punkt am Kreis
    p3 = Textposition
    a1 = Winkel Maszlinie/ Textblock in Grad
    a2 = Radius
    hd   (Def = 1)
    txt


  WinkelMasz:
    dtyp = 3
    p1 = Maszpunkt 1
    p2 = Maszpunkt 2
    p3 = Textposition
    a1 = Winkel Hilfslinie 1
    a2 = Winkel Hilfslinie 2
    hd, ld (Def = 12 11)
    txt
        Maszlinie-1:  p1-a1
        Maszlinie-2:  p2-a2
        Die Maszlinien liegen CCW !
        a1,a2 zeigen von Schnittpunkt der Maszlinien nach aussen !


 Leader:
    dtyp = 21
    p1 = Startpunkt (PfeilPos.)
    p2 = Endpunkt1
    p3 = Endpunkt2 (wenn x-Wert != UT_DB_LEER)
    a1 = Winkel Textblock
    hd   (Def = 1)
    txt




Functions:
  APT_DrawDimen
  UT3D_box_Dimen


================================================================== \endcode */}
void INF_FMTB_Dimensions_3D (){        /*! \code

Dimension-3D    form=Typ_Dim3  struct=Dim3


SourceObj:
P3=P(-110 20)
P4=P(-65 10)
N20=DIM3 P3 P4 P(-85 -14 50) "X"

ObjGX (typ=Typ_Note; form=Typ_Dim3; data=(Dim3*))


Functions:
  APT_DrawDim3
  UT3D_box_Dim3


================================================================== \endcode */}
void INF_FMTB_Bitmaps (){        /*! \code
Images, Tags:

SourceObj:

# typ=0  (2D-Text)
N30=TAG P(60 25 0) P(40 20 0) "2D-Text"

# typ=1  (image)
N20=IMG P(0 0 0) "Data/sample_bmp1.bmp"

# typ=2  (tag)
N21=TAG P(0 0 0) P(-23.1 -10 0) 2 "Tag rot"

# typ=3 (Balloon + leaderline)
N29=LDRC P(60 -40 0) P(40 -35 10) "Ball+Txt"

# typ=4 (LeaderLine + 3D-Text)
N20 = LDRP P(50 50 50) P(10 10 0.12345) "[3"

# typ >= 5 (symbol)
N23=LDRS P(0.7 -29.3 0) 0


SourceFiles:
../gr/ut_gtx.c
DB-Obj:
ObjGX (typ=Typ_Note; form=Typ_ATXT|Typ_Tag; data=(AText*))

Functions:
APT_DrawTxtA
  GL_Draw_BMP         1 Image
  GL_Draw_Tag         2 Tag
  GL_DrawTxtLBG       3 LeaderLine + Balloon + 3D-Text
  GL_DrawTxtLG        4 LeaderLine + 3D-Text
  GL_DrawTxtsym      >4 Symbol
  DL_txtSelect
  UT3D_box_AText
  GR_img_get_obj

================================================================== \endcode */}
void INF_FMTB_CURVES (){        /*! \code


Functions:
  DB_GetCurv     get *Curve (ObjGX*)
  DB_get_CV      get form and struct ((ObjGX*)"S")* of DB-curve


================================================================== \endcode */}
void INF_FMTB_Curve_Polygon (){        /*! \code


-----------------------
Curve-2D-Polygon:                       APT_decode_pol2
-----------------------

Point2 pa[iNr]  die PolygonPunkte; meist via memspc55


ObjGX     cv1
  cv1.typ   = Typ_CVPOL2
  cv1.form  = Typ_PT2
  cv1.siz   = Anzahl Punkte
  cv1.data  = (Point(2)*)pa=PolygonPunkte[siz]





-----------------------
Curve-3D-Polygon:                         APT_decode_pol
-----------------------

ObjGX     cv1

  cv1.typ   = Typ_CVPOL
  cv1.form  = Typ_CVPOL              [Typ_PT]
  cv1.siz   = 1                      [pointNr]
  cv1.data  = (CurvPoly*)cvPlg;      [(Point*)]

.....................
  CurvPoly    *cvPlg;
  Point       *pTab;
  int         ptNr;
  double      vo, v1, *vTab;

  cvPlg = cv1.data;
  pTab  = cvPlg->cpTab;        // cornerpoints
  ptNr  = cvPlg->ptNr;         // number of points
  v0    = cvPlg->v0;           // startparameter (length absolut)
  v1    = cvPlg->v1;           // endparameter (length absolut)
  vTab  = cvPlg->lvTab;        // parameters at cornerpoints (length absolut)


 
  (Point*) -> (CurvPoly*):   UT3D_plg_pta()


Examples:

  // polygon from array of points
  GL_DrawPoly (&dli, iAtt, pNr, pa1);




================================================================== \endcode */}
void INF_FMTB_Curve_Splines (){        /*! \code


-----------------------
Curve - Polynom.Spline                  APT_decode_psp3
-----------------------


polynom_d3  Polynome


ObjGX     cv1
  cv1.typ   = Typ_CVPSP3
  cv1.form  = Typ_Polynom3
  cv1.siz   = Anzahl Polynome
  cv1.data  = (polynom_d3*)=Polynome[siz]




================================================================== \endcode */}
void INF_FMTB_Curve_BSpl (){        /*! \code

CurvBSpl    Typ_CVBSP


-----------------------
Curve - B-Spline                        APT_decode_bsp
-----------------------


CurvBSpl  bSpl
  bSpl.ptNr  = (int) nr of controlpoints
  bSpl.deg   = (int) degree
  bSpl.v0    = (double) curve start parameter
  bSpl.v1    = (double) curve end parameter
  bSpl.kvTab = (double*) knotvalues[ptNr + deg + 1]
  bSpl.cpTab = (Point*) controlpoints[ptNr]


ObjGX     cv1
  cv1.typ   = Typ_CVBSP;
  cv1.form  = Typ_CVBSP;
  cv1.siz   = 1
  cv1.data  = (CurvBSpl*)=bSpl


Examples:

  int deg = 2;
  int iTyp = 0; //0=Standard; 1=optimized;
  Memspc   wrkSeg, tmpSeg;
  CurvBSpl cv1;
  ObjGX    o1;


  // curveObj from points
  UME_init (&wrkSeg, memspc101, sizeof(memspc101));
  UME_init (&tmpSeg, memspc201, sizeof(memspc201));
  irc = UCBS_BspCrvPts (&cv1, &wrkSeg, pNr, pta, deg, iTyp, &tmpSeg);
  if(irc < 0) { printf("***** UCBS_BspCrvPts %d\n"); continue; }

  OGX_SET_OBJ (&o1, Typ_CVBSP, Typ_CVBSP, 1, &cv1);




================================================================== \endcode */}
void INF_FMTB_Curve_RBSpl (){        /*! \code

CurvRBSpl Typ_CVRBSP



================================================================== \endcode */}
void INF_FMTB_Curve_Bez (){        /*! \code


CurvBez  Typ_CVBEZ


-----------------------
Curve - BezierCurve                      APT_decode_bsp
-----------------------


CurvBez  cvBez
  .ptNr  = nr of controlpoints
  .cpTab = (Point*) controlpoints[ptNr]

// ptNr  ... number of control points; degree = (ptNr - 1)
// size = 8 Byte




================================================================== \endcode */}
void INF_FMTB_Curve_RBez (){        /*! \code

CurvRBez  Typ_CVRBEZ



================================================================== \endcode */}
void INF_FMTB_Curve_CCV (){        /*! \code

Concatenated curves, Typ_CVTRM, struct CurvCCV

Source: ../ut/ut_cvtrm.c ut_cntf.c

Functions:
  UTO_cv_cvtrm       // make normal object of trimmedCurve

ObjGX     cv1
  cv1.typ   = Typ_CVTRM;
  cv1.form  = Typ_CVTRM;
  cv1.siz   = nr of subCurves
  cv1.data  = (CurvCCV*)=array of subCurves (trimmedCurves)


Example decode ccv:

  int    typ, i1, rNr;
  long   dbi;
  char   obj1[OBJ_SIZ_MAX];
  void   *o1, *o2;

  // get curve S20 (a CCV) from DB
  typ = Typ_CV;
  dbi = 20L;

  typ = DB_GetObjDat (&o1, &rNr, typ, dbi);
    printf(" typ=%d nr=%d\n",typ,rNr);

  o2 = o1;
  for(i1=0; i1<rNr; ++i1) {
    UT3D_stru_dump(typ, o2, " o2");
    if(typ == Typ_CVTRM) {
      // change trimmedCurve into standardCurve
      UTO_cv_cvtrm (&typ1, obj1, NULL, o2);
      UT3D_stru_dump(typ1, obj1, " ccv-o2");
    }
    o2 += UTO_siz_stru (typ); // next subObj
  }






================================================================== \endcode */}
void INF_FMTB_SURFACES__ (){        /*! \code


Functions:
  DB_GetSur          get Surf ((ObjGX*)"A")
  UTO_ck_surfTyp      returns surfSubTyp
  SUR_ck_typ          returns details of surf



------------------------------------------------
                   Surfaces:
------------------------------------------------


Tesselated surfaces (not perforated):
  Typ_SURCIR 
  Typ_SURSTRIP


Supportsurfaces (not trimmed, not perforated):
  Typ_SURRU   (ruled, extrusion) 
  Typ_SURRV   (revolved, sweep) 
  Typ_SURBSP  (B-spline, loft) 
  Typ_SURRBSP (rat.B-spline) 


Planar Trimmed-Perforated-Surface:
    (has a outer-boundary and 0-n inner-boundaries):
  Typ_SUR

Trimmed-Perforated-Surface with Supportsurface:
    (has a Supportsurface, outer-boundary and 0-n inner-boundaries):
  Typ_SUTP


Other:
  Typ_SURPTAB
  Typ_SURMSH
  Typ_SURHAT


------------------------------------------------






---------------------
Schraffierte Flaeche:
---------------------

ObjGX    os1;
  os1->typ   = Typ_SURHAT;
  os1->form  = Typ_SURHAT;
  os1->siz   = 1;
  os1->data  = (SurHat*)hat;


Offen:
  SurHat dzt ohne Inseln; ev wie Typ_SUTP (Aussengrenze & Inseln)





================================================================== \endcode */}
void INF_FMTB_Surface_RCIR (){        /*! \code

Pretesselated surface (tesselated fan).

SourceObj:
  A# = RCIR points ..

ObjGX    os1;
  os1.typ  = Typ_SURCIR
  os1.form = Typ_ObjGX
  os1.siz  = nr-of-points
  os1.data = (ObjGX*)opt=db-points (nr = os1.siz)

      opt.typ  = Typ_PT;
      opt.form = Typ_Index;
      opt.siz  = 1
      opt.data = (LONG_PTR)=DB-index of point


Functions:
  TSU_DrawRCIR
  UT3D_cv_scir__       get boundary-points from SURCIR (tesselated fan)
  OGX_GET_INDEX
  UT3D_box_obja

================================================================== \endcode */}
void INF_FMTB_Surface_RSTRIP (){        /*! \code

Pretesselated surface (tesselated stripe).

SourceObj:
  A30=RSTRIP,4, P20,P21,P22,P23, P30,P31,P32,P33


ObjGX    os1;
  os1.typ  = Typ_SURSTRIP
  os1.form = Typ_SURSTRIP
  os1.siz  = 1
  os1.data = (SurStripe*)sus

    sus.ptUNr = nr of points per stripe
    sus.ptVNr = nr of stripes
    sus.pTab  = (ObjGX*)opt=db-points  (nr = sus.ptUNr * sus.ptVNr)

      opt.typ  = Typ_PT;
      opt.form = Typ_Index;
      opt.siz  = 1
      opt.data = (long*)=DB-index of point



Functions:
  GR_DrawStrip
  UT3D_box_surStripe

================================================================== \endcode */}
void INF_FMTB_Surface_PLN (){        /*! \code
OBSOLETE; DO NOT USE    (replaced by Surface_TPS)

Planar Trimmed-Perforated-Surface:
    (has a outer-boundary and 0-n inner-boundaries):

SourceObj:
  A = outer-boundary [inner-boundaries ...]

 (ObjGX) typ  = Typ_SUR;
         form = Typ_ObjGX;
         siz  = Anzahl von ObjGX in data
         data = (ObjGX[0])=Aussenkontur (Kreis, Polygon, CCV)
                [(ObjGX[1-n])=Lochkonturen]
or
         data = (ObjGX[0]) typ=Typ_Typ; form=Typ_Int4; data=Typ_SURPLN;
                (ObjGX[1])=Aussenkontur (Kreis, Polygon, CCV)
                [(ObjGX[2-n])=Lochkonturen]

Functions:
  TSU_DrawSurTS

================================================================== \endcode */}
void INF_FMTB_Surface_RU (){        /*! \code

Ruled Surface, Extrusion-surface;
  support-surface - not trimmed, not perforated

SourceObj:
  # A = SRU contour extrusion-vector
  L21=P(1.9 -35.7) P(1.3 -18.7)
  A22=SRU L21 D(0 0 20)

  # A = SRU cont1 cont2 [CW]
  L20=P(0 -10 20) P(0 10 20)
  C20=ARC P(0 -10) P(0 10) P(0 0)
  A20=SRU C20 L20

 (ObjGX) typ  = Typ_SURRU;
         form = Typ_ObjGX;
         siz  = 2
         data = (ObjGX[2]) ObjGX[0]=contour
                           ObjGX[1]=extrusion-vector or point or second contour

Functions:
  TSU_DrawSurT_
  UT3D_box_surRU


================================================================== \endcode */}
void INF_FMTB_Surface_RV (){        /*! \code

Revolved Surface, Spherical surface,
  cylinder, cone, torus, ..
  support-surface - not trimmed, not perforated:
  
see typedef SurRev
see ../../doc/html/CAD_SUR_en.htm#F2

SourceObj:
  # Spherical surface:
  A = SPH axis radius [rotang1 rotang2 [height_angle1 height_angle2]]
  A20=SPH RZ 12 0 90 0 62
  # revolved
  A21=SRV L(P(75 75) P(125 75)) L(P(150 40) P(100 50)) 0 90

 (ObjGX) typ  = Typ_SURRV;
         form = Typ_SURRV;
         siz  = 1
         data = (SurRev*)RevolvedSurface


Functions:
  APT_decode_srv
  TSU_tr_init_
  TSU_DrawSurTRV
  UT3D_box_surRV
  SRV_pt_int_ln_srv SRV_pt_int_ln_sCyl SRV_pt_int_ln_sCon

Files:
../ut/ut_srv.c


================================================================== \endcode */}
void INF_FMTB_Surface_SWP (){        /*! \code

Sweep Surface,   defined by a contour moving along a path;
  support-surface - not trimmed, not perforated:
  A = SWP contour path


SourceObj:
P20=P(30, -4280, 0)
P21=P20 Z(2000)
L20=P20 P21
P23=P(900.8038175 -3177.98753 0)
C20=P23 P20
A21=SWP C20 L20

 (ObjGX) typ  = Typ_SURSWP;
         form = Typ_SURSWP;
         siz  = 1
         data = (SurSwp*)SweepSurf


// indPath  DB-index path
// typPath  typ of path (Typ_LN ..)
// indCov   DB-index contour(-cover)element
// typCov   typ of contourelement (Typ_CI ..)
// u0,u1    start/end-parameters of path; for all types of contour 0-1
// v0,v1    start/end-parameters of contour; for all types of contour 0-1
// dir      direction; 0=CCW, 1=CW.


Functions:
  Tess_sur__
  Tess_sSym__
  UT3D_box_surSwp



================================================================== \endcode */}
void INF_FMTB_Surface_BSP (){        /*! \code
Loft Surface, B-Spline-Surface, from curves along (and across) surface
  support-surface - not trimmed, not perforated:

SourceObj:
  # Loft surface from curves across surface
  # A = BSP U(curves_across)
  L20=P(-100 60 0) P(-100 30 0)
  L21=P(-50 60 0) P(-60 80 0)
  A21=BSP U(L20 L21)

  # Loft surface from curves along and across
  # A = BSP U(curves_across) U(curves_along)

  # native B-Spline-Surf
  # A = BSP ptUNr, ptVNr, degU, degV, points, kvTabU, kvTabV
  P20=-41.190914,0,-4.996784
  P21=-19.376413, 0, 16.615933
  P22=20.818615, 0, 44.904567
  P23=92.424123, 0, 66.115783
  P24=109.740798, 0, 51.056768
  P25=123.769592, 0, -6.306126
  P30=-41.790491,100,7.509394
  P31=-10.139126,100,31.36722
  P32=21.527774,100,41.634623
  P33=63.803676,100,22.896914
  P34=98.163285,100,5.995995
  P35=123.061099,100,-16.096618
  A1=BSP,6,2,5,1,P20 P21 P22 P23 P24 P25 P30 P31 P32 P33 P34 P35 0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1


 (ObjGX) typ  = Typ_SURBSP;
         form = Typ_SURBSP;
         siz  = 1
         data = (SurBSpl*)

Functions:
  TSU_DrawSurBsp
  UT3D_box_surBsp

================================================================== \endcode */}
void INF_FMTB_Surface_RBSP (){        /*! \code

Loft Surface, Rational-B-Spline-Surface, from curves across surface
  support-surface - not trimmed, not perforated:

SourceObj:
  A = RBSP curves_across

 (ObjGX) typ  = Typ_SURRBSP;
         form = Typ_SURRBSP;
         siz  = 1
         data = (SurRBSpl*)

Functions:
  TSU_DrawSurRBsp


================================================================== \endcode */}
void INF_FMTB_Surface_TPS  (){        /*! \code

Surface with supporting_surface, trimmed, perforated

SEE ALSO NEW VERSION INF_SUTP

SourceObj:
  A = FSUB supportSurf contour [inner_contours]

 (ObjGX) typ  = Typ_SUTP
         form = Typ_ObjGX;
         siz  = nr of ObjGX in data
         data = (ObjGX[0])=supporting_surface (CON/TOR/SRU/SRV/SBS)
                           typ=Typ_SUR; form=Typ_Index
                           planar: typ=Typ_Typ; form=Typ_Int4; data=Typ_SURPLN;
                [(ObjGX[1])=outer_boundary (none: unlimited supporting_surface)
                [(ObjGX[2-n])=inner_boundaries]]


Functions:
  TSU_DrawSurT_
  UT3D_box_surPln

================================================================== \endcode */}
void INF_FMTB_Surface_PTAB (){        /*! \code

Flaeche von PunkteTabelle (A=PTAB)           Func: APT_decode_msh_p
 (ObjGX) typ  = Typ_SURPTAB
         form = Typ_NULL
         siz  = 0
         data = NULL



================================================================== \endcode */}
void INF_FMTB_Surface_MSH (){        /*! \code

Flaeche von Mesh                             Func: APT_decode_msh__
 (ObjGX) typ  = Typ_SURMSH
         form = Typ_Index;
         siz  = 1
         data = (long) Index of suface of its PTAB






================================================================== \endcode */}
void INF_FMTB_Surface_GL_Sur (){        /*! \code


GL_Surface     (bestehend aus fertig tesselierten planaren Patches)
  (ObjGX) typ  = Typ_GL_Sur;
          form = Typ_ObjGX;
          siz  = nr of planar patches + Colors
          data = PlanarPatches

  (ObjGX) typ  = Typ_GL_PP;          (planar patch)
          form = Typ_ObjGX;
          siz  = nr of contours (first obj is Z-vector)
          data = Normalvektor und Contours

  (ObjGX) typ  = Typ_PT;          (Contour)
          form = Typ_PT;
          siz  = nr of points
          aux  = GL-Typ; 4=GL_TRIANGLES 5=GL_TRIANGLE_STRIP 6=GL_TRIANGLE_FAN
                         16=GL_TRIANGLE_NPFAN
          data = *Point[siz]      (geschlossenes Polygon)


  (ObjGX) typ  = Typ_Typ;          (start of new surface)
          form = Typ_Int4;
          siz  = 1
          data = (long) SurfTyp; zB Typ_SURPLN ..



Funktionen:
 GL_Disp_sur        Darstellung eines einzelnen Data-record.
 UTO_dump_f_        dump tesselated data

// Beispiel: 210=Typ_GL_Sur; 211=Typ_GL_PP;
//           143=Typ_ObjGX;  5=Typ_VC; 2=Typ_PT ..
//    typ-form-siz-dat
//    210 143 2 a1
// a1   211 143 3 a2
//      211 143 4 a3
// a2     5 5 1 a4
//        2 2 5 a5
//        2 2 5 a6
// a3     5 5 1 a7
//        2 2 5 a8
//        2 2 5 a9
//        2 2 5 a10
// a4 (struct VC)
// a5 (struct PT * 5)
// ....

Version ohne Normalvektoren und Colour fuer Intersect:
//    210 143 2 a1
// a1   211 2 8 a2
//      211 2 5 a3
// a2 (strut PT * 8)
// a3 (strut PT * 5)




---------------------------------------------------------------
Konische/Toroide Flaeche; ungetrimmt, ungelocht:      UNUSED
 (ObjGX) typ  = Typ_SUR;
         form = Typ_ObjGX;
         siz  = Anzahl von ObjGX in data
         data = (ObjGX[0])=Stuetzflaeche; typ=Typ_SOL (Conus/Torus)




============================================================================



================================================================== \endcode */}
void INF_FMTB_Bodies__ (){        /*! \code

solids - 


================================================================== \endcode */}
void INF_FMTB_Body_BREP (){        /*! \code

Body_BREP      closed shell

SourceObj:
  ..

SurBnd   sba[]
  sba[i].suID = surface-ID (DB-index A)
  sba[i].seNr = nr of EDGSURs
  sba[i].sea  = *EDGSUR


ObjGX     ox1
  ox1.typ     = Typ_BREP;
  ox1.form    = Typ_SURBND;
  ox1.siz     = sbaNr;          // nr of SurBnd's
  ox1.data    = (SurBnd*)&sba;  // table of SurBnd's



================================================================== \endcode */}
void INF_FMTB_Body_Sphere (){        /*! \code

Body_Sphere    Kugel

SourceObj:
  ..

Sphere     sph
  sph.pc     = (Point)  Centerpoint
  sph.rad    = (double) Radius

ObjGX     ox1
  ox1.typ    = Typ_SPH;
  ox1.form   = Typ_SPH;
  ox1.siz    = 1;
  ox1.data   = (Sphere*)&sph;





================================================================== \endcode */}
void INF_FMTB_Body_Cone (){        /*! \code

Body_Cone           Konus, Zylinder ..

SourceObj:
  ..

Conus     con
  con.pl     = (Plane)  Achsensystem als (pl.po ist Center-bottom)
  con.r1     = (double) Radius bottom
  con.r2     = (double) Radius top
  sph.h      = (double) Hoehe  (height)

ObjGX     ox1
  ox1.typ    = Typ_CON;
  ox1.form   = Typ_CON;
  ox1.siz    = 1;
  ox1.data   = (Conus*)&con;




================================================================== \endcode */}
void INF_FMTB_Body_Torus (){        /*! \code

Body_Torus       

SourceObj:
  ..

Torus     tor
  tor.pl     = (Plane)  Achsensystem als (pl.po ist Center)
  tor.r1     = (double) Radius of outermost circle
  tor.r2     = (double) Radius of torusring (Querschnitt)

ObjGX     ox1
  ox1.typ    = Typ_TOR;
  ox1.form   = Typ_TOR;
  ox1.siz    = 1;
  ox1.data   = (Torus*)&tor;

Functions:
  UT3D_box_Torus


================================================================== \endcode */}
void INF_FMTB_Body_PRISM (){        /*! \code

Body_PRISM

SourceObj:
  ..

ObjGX     ox2[2]
  ox2[0].typ    = Typ_CIR   oder Typ_CV (ex S=REC..)
  ox2[0].form   = Typ_Index;
  ox2[0].data   = (void*)&ci1;

  ox2[1].typ    = Typ_CIR od Typ_PT od Typ_VAR od Typ_VC
  ox2[1].form   = Typ_Index;
  ox2[1].data   = (void*)data entspr. typ



ObjGX     ox1
  ox1.typ    = Typ_SOL;
  ox1.form   = Typ_ObjGX;
  ox1.siz    = 2;
  ox1.data   = (void*)&ox2;


Functions:
  GL_disp_prism


============================================================================

================================================================== \endcode */}
void INF_FMTB_Model (){        /*! \code


---------------
Model-Reference
---------------

ModelRef   mr1;
  mr1.mnam
  mr1.modNr  = modelnumber of ModelBas-obj. (DB_get_ModNr())
  mr1.scl
  mr1.po     = position of ditto
  mr1.vx
  mr1.vz





ObjGX     ox1
  ox1.typ    = Typ_Model;
  ox1.form   = Typ_Model;
  ox1.siz    = 1;
  ox1.data   = (void*)&mr1;









================================================================== \endcode */}
void INF_FMTB_BinaryMesh (){        /*! \code

Imports of mockup-files (.wrl, .obj, .stl) are stored as .tess-files in {tmpdir}.

Files:
../ut/ut_tess.c             func for tesselated surfaces   tess_*
../gr/ut_tess_su.c          tesselate analytic surfaces    Tess_*
../xa/tst_tess_1.c          create a .tess ? (tst_tess.mak)
../xa/tst_surfaces.c        new version APT_decode_su_swp_cyl (tst.mak)


See also:
INF_FMTB_Surface_GL_Sur
INF_FMTB_Surface_RCIR
INF_FMTB_Surface_RSTRIP


------------
BinaryMesh:
------------

Topmost record: type ObjGX
  keeps n surfaces (surface is a ObjGX-record where typ=Typ_GL_PP)
  typ    Typ_GL_Sur
  form   Typ_ObjGX (form of records in data)
  data   array of surfs
  siz    nr of surfs

Surface-record (type ObjGX):
  can have planar-patches, normal-vectors, color, texture.
   normal-vector: typ=Typ_VC, form=Typ_VC, data=Vector
   color: typ=Typ_Color, form=Typ_Int4, data=ColRGB (see OGX_SET_COLOR)

PlanarPatch:   (face)
  typ    Typ_PT
  form   Typ_PT (form of records in data)
  data   array of points
  siz    nr of points
  aux    structure of data; eg GL_TRIANGLES|GL_TRIANGLE_FAN|..



Functions:
  obj_read__                          read WaveFront-OBj-File - write .tess
  wrl_readTess__                      import VRML-Version-1 as .tess
  TSU_imp_tess       load file > mem
  TSU_DrawSurTess    Darstellung gesamtes BinaryModel
  UTO_dump_f_        dump tesselated data

  GL_DrawSur
  GL_Disp_sur        Darstellung eines einzelnen Data-record.
  TSU_Init, TSU_store
  OGX_siz__            // get total size of bMsh
  OGX_oxm_copy_ox      // serialize bMsh (copy objects & data into single memspc)
  OGX_deloc__          // delocate
  UME_write__          // write into file
  UME_read__           // read from file
  OGX_reloc__          // relocate
  TSU_exp__ TSU_exp_Open TSU_exp_sur  // export bMesh
  TSU_tsu2tria__ TSU_tsu2tria_rec     // intersect INT_intplsur|INT_intsursur


Derzeit nur fuer die Mockup-Models.
Enthaelt abwechselnd SizeDefinition-Record, ObjGX-Struktur,  dann wieder
  SizeDefinition-Record, ObjGX-Struktur.
Am Ende steht statt dem naechsten SizeDefinition-Record ein Schlussrecord.
Decodierfunktion UTO_dump_f_
Wird ein BinaryModel in eine Datei geschrieben, so wird zusaetzlich als erster
 Record ein AdressRecord geschrieben.


AdressRecord (nur in Datei):
  add.typ    = Typ_Address
  add.form   = Typ_Data
  add.siz    = 1;
  add.data   = address of first data-record (for relocation)
               (nur size of data; ohne add- u rfi-Record)

SizeDefinition-Record:
ObjGX  rsz
  rsz.typ    = Typ_Size
  rsz.form   = Typ_Int4
  rsz.siz    = 1;
  rsz.data   = (long)size of following Data-record in byte

Data-records:
  ..

TerminationRecord rfi:
  used only for filed meshes
ObjGX  rfi
  rfi.typ    = Typ_Done
  rfi.form   = Typ_ObjGX
  rfi.siz    = 1;
  rfi.data   = NULL





================================================================== \endcode */}
void INF_FMTB_Transformation (){        /*! \code


============================================================================
---------------
Transformation:
---------------

ObjGX     tr1
  ox1.typ    = Typ_Tra;
  ox1.form   = Typ_VC (Translation) oder Typ_TraRot (Rotation)
  ox1.siz    = 1;
  ox1.data   = (Vector*) fuer Translation oder (TraRot*) f. Rotation


TraRot    rr1
  rr1.ma     = (Mat_4x3) ma; Drehpunkt ist ma[0][3], ma[1][3], ma[2][3]
  rr1.vz     = (Vector) Drehachse
  rr1.angr   = (double) Drehwinkel in Grad


Functions:
  DB_GetTra          get Transformation ((ObjGX*)"T")


================================================================== \endcode */}
// eof
