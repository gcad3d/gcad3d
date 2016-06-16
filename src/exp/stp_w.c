//     STEP -Files Write.                 RF   2005-08-28
/*
 *
 * Copyright (C) 2015 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
-----------------------------------------------------
TODO:
  ..

-----------------------------------------------------
Modifications:
  ..

-----------------------------------------------------
*/
/*!
\file  ../exp/stp_w.c
\brief write STeP file 
\code
=====================================================
List_functions_start:

STP_w__             main
STP_w_init
STP_w_os_init
STP_w_wf_init
STP_w_cs__          output of yet usused surfaces as CLOSED_SHELL  ???
STP_w_os__          output of yet usused surfaces as OPEN_SHELL
STP_w_wf__          wireframe-objects
STP_w_os_wo
STP_w_SURBSP
STP_w_SURRU
STP_w_LINEAR_EXTRUSION   TODO
STP_w_SURRV
STP_w_con_1
STP_w_SURCIR
STP_w_SURTP__
STP_w_ADVANCED_FACE
STP_w_EDGE_LOOP_b
STP_w_ORIENTED_EDGE
STP_w_EDGE_LOOP__
STP_w_wf_wo
STP_w_PT
STP_w_VC_d
STP_w_VCdv
STP_w_axis3_vz
STP_w_axis3_dbo
STP_w_axis3__
STP_w_LN
STP_w_AC__
STP_w_AC_CC
STP_w_EL_CC
STP_w_EL__
STP_w_CVPOL
STP_w_CVCCV
STP_w_CVBSP
STP_w_TRIMMED_CURVE_par
STP_w_TRIMMED_CURVE
STP_w_CURVE_SET
STP_w_CCV_seg
STP_w_CCV__
STP_w_CCV
STP_w_txt
STP_w_list_is
STP_w_list__
STP_w_stpPln_set
STP_w_sur_nvc
STP_w_plb_ini
STP_w_end

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



==============================
Doku STEP =  STP
/mnt/serv1/Devel/dev/gCAD3D/formate/step/part203.exp.html
evince /mnt/serv1/Devel/dev/gCAD3D/formate/step/Step_D_90_04.pdf
../exp/stp_w.c
../exp/stp_r.c

AP203 = allg. Masch.bau
AP214 = fuer KFZ



OPEN:
ADVANCED_FACE  last param T|F   was ist das ?

==========================================================
Fileformat:

ISO-10303-21;                 // erste Zeile ...
HEADER;
FILE_....                     // Filetyp usw ..
ENDSEC;
DATA;
#1=..                         // jede datzeile: #<objNr>=<obj>
ENDSEC;
END-ISO-10303-21;



Comments:                / * C-style (hier nicht ff!) auch inline * /  
Fortsetzungszeilen       beliebig; LineEnd ist ";"
Leerzeilen:              zulaessig.
Textparameter            in Hochkommas  zb  'Text'


Datenzeilen:
 Reihenfolge beliebig 
 Loecher in der Nummerierung   zulaessig.
 Der erste Parameter ist bei allen Objekten der Objektname;
  zB 'Line Origine' od '' od $             (ACHTUNG: nur ein $ - char !)
 Klammern bedeuten dass es ein Parameter mit variabler Anzahl Links ist;
   zB ..,(#110,#88),..




------------------------------------------------------
Punkte:
#51=CARTESIAN_POINT('Point.5',(100.,100.,0.));
// (X-coord, Y-coord, Z-coord)
#134=VERTEX_POINT('',#51);
// (#CARTESIAN_POINT)

------------------------------------------------------
Vector:
#110=VECTOR('',   #<DIRECTION>,  <length>);         // vector with length !
#169=DIRECTION('',(<x-val>,<y-val>,<z-val>));       // vector

------------------------------------------------------
Line (Point, Vector):           UNBEGRENZT !!!
LINE('Line',#<CARTESIAN_POINT>,#<VECTOR>) ;
//            startpoint         vector start-end

------------------------------------------------------
CIRCLE (name, <AXIS2_PLACEMENT_3D>, <radius>);
Drehrichtung ?

------------------------------------------------------
ELLIPSE(name,<AXIS2_PLACEMENT_3D>, <majAxisLength>, <minorAxisLength>);

------------------------------------------------------
B_SPLINE_CURVE_WITH_KNOTS('',2,(#98,#99,#100,#101),.UNSPECIFIED.,.F.,
.F.,(3,1,3),(0.,1.,2.),.UNSPECIFIED.);
// (2, (degree)
    (n * #CARTESIAN_POINT (Controlpoints),
    .UNSPECIFIED.,        (form: unspecified, polyline_form, parabolic_arc,
                           circular_arc
    .F.,                  // closed_curve
    .F.,                  // self_intersect
    (3,1,3),              (multiplicities = nr of identical knotValues)
    (0.,1.,2.),           (knot-values)
    .UNSPECIFIED.);       // knot_type ?
// knots: (0.,0.,0.,1.,2.,2.,2.) - 3 times 0., 1., 3 times 2.

------------------------------------------------------
PARABOLA|HYPERBOLA|B_SPLINE_CURVE|POLYLINE



------------------------------------------------------
Achsensystem:
AXIS2_PLACEMENT_3D (name, <CARTESIAN_POINT>, <DIRECTION>, <DIRECTION>)
                             origin,          z-axis,    x-axis)

AXIS1_PLACEMENT ('', <CARTESIAN_POINT>, <DIRECTION>);


------------------------------------------------------
supportSurfaces:
PLANE(name, <AXIS2_PLACEMENT_3D>)

CYLINDRICAL_SURFACE('',#100,22.0);  // (AXIS2_PLACEMENT_3D, dmr)
  unlimited: RECTANGULAR_TRIMMED_SURFACE

CONICAL_SURFACE('',#100,92.0,19.0); // (AXIS2_PLACEMENT_3D, radius, angle)
  unlimited: RECTANGULAR_TRIMMED_SURFACE

SPHERICAL_SURFACE('',#100,25.);       // (AXIS2_PLACEMENT_3D, dmr)
  unlimited: direct into GEOMETRIC_SET
  RECTANGULAR_TRIMMED_SURFACE -> GEOMETRIC_SET geht ned ..

TOROIDAL_SURFACE('',<axis>,rad1,rad2);
  unlimited: direct into GEOMETRIC_SET
  RECTANGULAR_TRIMMED_SURFACE -> GEOMETRIC_SET

SURFACE_OF_REVOLUTION('',<coverCurve>,<AXIS1_PLACEMENT>); // axis only vector !
  coverCurve: LINE|B_SPLINE_CURVE_WITH_KNOTS
  unlimited: direct into GEOMETRIC_SET
  RECTANGULAR_TRIMMED_SURFACE -> GEOMETRIC_SET geht nur in freecad
  Panda.stp tor_eng.stp

SURFACE_OF_LINEAR_EXTRUSION('',<curve>,<VECTOR>)
  curve: LINE|B_SPLINE_CURVE_WITH_KNOTS
         TRIMMED_CURVE from LINE wird aber (direct) nicht angezeigt ..
  unlimited: direct into GEOMETRIC_SET
  FreeCAD1.stp v4_MAN2.stp

 B_SPLINE_SURFACE_WITH_KNOTS(name, u_deg, v_deg,
   ((cv1_points),(cvn_points)),
   .UNSPECIFIED.,.U.,.U.,.U.,
   (u_multiplicities),(v_multiplicities),(u_knots),(v_knots),knot_type);
  unlimited: direct into GEOMETRIC_SET



------------------------------------------------------
RECTANGULAR_TRIMMED_SURFACE('*SUR1',#655,0.0,360.0,162.0,0.0,.T.,.T.);
  (4 parameters - u1,u2,v1,v2, usense, vsense)
  only for CYLINDRICAL_SURFACE,SPHERICAL_SURFACE,CONICAL_SURFACE,TOR.,planar.
  NOT for B_SPLINE_SURFACE
  Can only go into GEOMETRIC_SET; not into OPEN_SHELL.

------------------------------------------------------
CCV:
COMPOSITE_CURVE(name,  (name, (<COMPOSITE_CURVE_SEGMENT>s), self_intersect) ;
  // self_intersect  .U.|.T.|.F.

 COMPOSITE_CURVE_SEGMENT(.CONTINUOUS., .T., <TRIMMED_CURVE>) ;
   // transition      .CONTINUOUS.|.DISCONTINUOUS.   (closed or not)
   //     .DISCONTINUOUS. = open; .CONTINUOUS. = closed
   // same_sense      .T.|.F.

  TRIMMED_CURVE(name,<baseCurve>,(<startPoint>),(<endPoint>),.T.,.CARTESIAN.) ;


------------------------------------------------------
TRIMMED_CURVE(name,<baseCurve>,(<startPoint>),(<endPoint>),orient,flag2);
  // baseCurve: LINE|CIRCLE|ELLIPSE|B_SPLINE_CURVE_WITH_KNOTS
  // points: trimpoints|values; (CARTESIAN_POINT), (CARTESIAN_POINT)
  // orient: .T.|.F.; .T. = same direction as baseCurve;
  //   if the 2.point is before first Trimpoint, orient. is .F.
  // flag2: .UNSPECIFIED.|.CARTESIAN.|.PARAMETER.

TRIMMED_CURVE('',<LINE>,(<CARTESIAN_POINT>),(<CARTESIAN_POINT>),.T.,.CARTESIAN.);
//                       startpoint         endpoint          sameDir

TRIMMED_CURVE('',<LINE>,(PARAMETER_VALUE(0.0)),(PARAMETER_VALUE(1.0)),
  .T.,.UNSPECIFIED.);

TRIMMED_CURVE('',<CIRCLE>,(PARAMETER_VALUE(0.0)),(PARAMETER_VALUE(180.0)),
  .T.,.UNSPECIFIED.);
// parameter = angles-degree (startPt,endPt)

TRIMMED_CURVE('',#60,(0.0),16.0),.T.,.UNSPECIFIED.) ;
//             (#LINE,(offset1),(offset2), . . )



TRIMMED_CURVE('',#60,(CARTESIAN_POINT(#3368)), ..
TRIMMED_CURVE('',#17,(#21,PARAMETER_VALUE(0.)), .., .PARAMETER.);


------------------------------------------------------
?
QUASI_UNIFORM_CURVE                  conrod.stp
(BOUNDED_CURVE()B_SPLINE_CURVE( ...  conrod.stp


------------------------------------------------------
EDGE_CURVE (name, Trimpoint1, Trimpoint2, BaseCurve, sense)
// 3 Links, same_sense (.T.|.F.)
// BaseCurve: LINE¦CIRCLE|ELLIPSE|PARABOLA|HYPERBOLA|B_SPLINE_CURVE|POLYLINE


------------------------------------------------------
intersectioncurve of 2 surfaces:
INTERSECTION_CURVE (name, curv, (surf1, surf2), .CURVE_3D.)
  curv: LINE|CIRCLE
  surf: (PLANE,PLANE) for LINE,
        (PLANE,CYLINDRICAL_SURFACE) for CIRCLE



------------------------------------------------------
ADVANCED FACE ((Aussen/Innenbegrenzung), Stuetzflaeche)
  Stuetzflaeche: PLANE|CYLINDRICAL_SURFACE|SPHERICAL_SURFACE|CONICAL_SURFACE|
                 TOROIDAL_SURFACE

  Aussenbegrenzung: 
    FACE_OUTER_BOUND (1 EDGE_LOOP)
      EDGE_LOOP (n ORIENTED_EDGEs)      = closed CCV
        ORIENTED_EDGE (1 EDGE_CURVE)
          EDGE_CURVE (startPoint, endPoint, Curve)
            // Curve: LINE|CIRCLE

  Innenbegrenzungen:
    FACE_BOUND (1 EDGE_LOOP)   same as FACE_OUTER_BOUND
  startPoint, endPoint: VERTEX_POINTs


------------------------------------------------------
ORIENTED_EDGE (name, *, *, <EDGE_CURVE>, orientation)
  // orientation  .T.|.F. 


===========================================================================
Default:
   #15=(GEOMETRIC_REPRESENTATION_CONTEXT(3)..
   #16=PRODUCT_DEFINITION_SHAPE..
   #100=AXIS2_PLACEMENT_3D..


------------------------------------------------------
Points, curves,
untrimmed surfaces: TOROIDAL_SURFACE,SURFACE_OF_REVOLUTION,
  SURFACE_OF_LINEAR_EXTRUSION,B_SPLINE_SURFACE_WITH_KNOTS


#200=COMPOSITE_CURVE..
#201=COMPOSITE_CURVE..
#202=B_SPLINE_SURFACE_WITH_KNOTS..

#251=GEOMETRIC_SET('',(#200,#201,#202));
#252=GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION('',(#251),#15);

#253=SHAPE_REPRESENTATION('',(#100),#15);
#254=SHAPE_REPRESENTATION_RELATIONSHIP('','',#253,#252);
#255=SHAPE_DEFINITION_REPRESENTATION(#16,#253);


------------------------------------------------------
Trimmed/perforated surfaces:
ADVANCED_FACE < MANIFOLD_SURFACE_SHAPE_REPRESENTATION

#1060=SHAPE_DEFINITION_REPRESENTATION(#16,#1059);
#1059=SHAPE_REPRESENTATION('',(#100),#15);
#1061=SHAPE_REPRESENTATION_RELATIONSHIP('','',#1059,#1058);
#1058=MANIFOLD_SURFACE_SHAPE_REPRESENTATION('',(#1057),#15);
#1057=SHELL_BASED_SURFACE_MODEL('',(#1056));
#1056=OPEN_SHELL('',(#1055));
#1055=ADVANCED_FACE('',(#333),#1054,.T.);



------------------------------------------------------
Curves & surfaces:

#1055=ADVANCED_FACE('',(#333),#1054,.T.);
#1183=COMPOSITE_CURVE('S9',(#1093,#1120,#1153,#1182),.U.);

#1184=GEOMETRIC_SET('',(#1183));
#1185=GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION('',(#1184),#15);

#1056=OPEN_SHELL('',(#1055));
#1057=SHELL_BASED_SURFACE_MODEL('',(#1056));
#1058=MANIFOLD_SURFACE_SHAPE_REPRESENTATION('',(#1057),#15);

#1186=SHAPE_REPRESENTATION('',(#100),#15);
#1187=SHAPE_REPRESENTATION_RELATIONSHIP('','',#1186,#1185);
#1188=SHAPE_DEFINITION_REPRESENTATION(#16,#1186);
#1189=SHAPE_REPRESENTATION_RELATIONSHIP('','',#1186,#1058);


------------------------------------------------------
Solid:
 SHAPE_DEFINITION_REPRESENTATION(<PDS>,<ABSR>);
   ADVANCED_BREP_SHAPE_REPRESENTATION('',(<refsys>,<MANIFOLD_SOLID_BREP>),<GRC>);
     MANIFOLD_SOLID_BREP('B2',<CLOSED_SHELL>);
       CLOSED_SHELL('',(<ADVANCED_FACEs>));

//PDS=PRODUCT_DEFINITION_SHAPE #16
//GRC=GEOMETRIC_REPRESENTATION_CONTEXT..=#15
test_sol_sph1.stp


===========================================================================
------------------------------------------------------------------
Surfs:
------------------------------------------------------------------
OPEN_SHELL ((ADVANCED_FACEs))
 ADVANCED_FACE ((Aussen/Innenbegrenzung), Stuetzflaeche)





#88=GEOMETRIC_CURVE_SET('Standard_curve_set',(#98));
// ?



........................................................
Oberstes Objekt ist PRODUCT.
#46=PRODUCT('Part','','not specified',(#47));
 #45=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE('','',#46,.MADE.);
  #43=PRODUCT_DEFINITION('design','',#45,#44);
   #14=PRODUCT_DEFINITION_SHAPE('','Shape',#43);
    #13=SHAPE_DEFINITION_REPRESENTATION(#14,#100);
     #100=SHAPE_REPRESENTATION('',(#101),#149);  // wertlos
      #50=SHAPE_REPRESENTATION_RELATIONSHIP('','',#100,#51);
       #51=ADVANCED_BREP_SHAPE_REPRESENTATION('',(#110,#53),#149);


..................
Der Masterworkspace beinhaltet
#277=SHAPE_REPRESENTATION('*MASTER',(#441,#454,#463,#472),#45);
 #479=SHAPE_REPRESENTATION_RELATIONSHIP(' ',' ',#277,#91);
  #91=MANIFOLD_SURFACE_SHAPE_REPRESENTATION('*MASTER',(#92),#45);
   #92=SHELL_BASED_SURFACE_MODEL('NONE',(#93,#138));
    #93=OPEN_SHELL('NONE',(#90));
    #138=OPEN_SHELL('NONE',(#137));
 (REPRESENTATION_RELATIONSHIP(..              Ditto
  ITEM_DEFINED_TRANSFORMATION



.........................................
Das Detail "DET1" beginnt mit   C4_t01.stp   4 Dittos
#428=SHAPE_REPRESENTATION('D1',(#432,#446),#282);
 #434=SHAPE_REPRESENTATION_RELATIONSHIP(' ',' ',#428,#418);
  #418=ADVANCED_BREP_SHAPE_REPRESENTATION('D1',(#284),#282);
   MANIFOLD_SOLID_BREP
    CLOSED_SHELL
 #435=SHAPE_REPRESENTATION_RELATIONSHIP(' ',' ',#428,#420);
  #420=GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION('D1',(#421),#282);

NEXT_ASSEMBLY_USAGE_OCCURRENCE = Ditto !


.........................................
// Example assy4:
#37=PRODUCT('C104806-C108806','C104806-C108806',' ',(#36));
#46=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE('None','None',#37,
#98=PRODUCT_DEFINITION('None','None',#46,#97);
#128=PRODUCT_DEFINITION_SHAPE('PDS1','Test Part',#98);
#129=SHAPE_DEFINITION_REPRESENTATION(#128,#127);

#135=PRODUCT('HOUSING','HOUSING',' ',(#36));
#136=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE('None','None',#135,
#137=PRODUCT_DEFINITION('None','None',#136,#97);
#138=SHAPE_REPRESENTATION('SR2',(#126),#134)
#139=PRODUCT_DEFINITION_SHAPE('PDS2','Test Part',#137);
#140=SHAPE_DEFINITION_REPRESENTATION(#139,#138);
#4211=NEXT_ASSEMBLY_USAGE_OCCURRENCE('NAUO1','HOUSING_1','HOUSING_1',#98,#137,$);
#4201=SHAPE_REPRESENTATION_RELATIONSHIP('SRRPL1',' ',#4200,#138);
#4200=ADVANCED_BREP_SHAPE_REPRESENTATION('ABSR1',(#4199),#134);
#4199=MANIFOLD_SOLID_BREP('25F',#4198);
#4198=CLOSED_SHELL('',(#216,#258,#300,...

#4219=PRODUCT('BEARING','BEARING',' ',(#36));
#4220=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE('None','None',#4219,
#4221=PRODUCT_DEFINITION('None','None',#4220,#97);
#4223=PRODUCT_DEFINITION_SHAPE('PDS3','Test Part',#4221);
#4224=SHAPE_DEFINITION_REPRESENTATION(#4223,#4222);
#4398=NEXT_ASSEMBLY_USAGE_OCCURRENCE('NAUO2','BEARING_1','BEARING_1',#98,#4221,$);
#4388=SHAPE_REPRESENTATION_RELATIONSHIP('SRRPL2',' ',#4387,#4222);
#4387=ADVANCED_BREP_SHAPE_REPRESENTATION('ABSR2',(#4386),#4218);
#4386=MANIFOLD_SOLID_BREP('1C56',#4385);
#4385=CLOSED_SHELL('',(#4266,#4308,...



//--------------------------------------------------------
Dittos (subModelReferences):
//--------------------------------------------------------
Example ../formate/step/Models/v4_ditto1.stp    3 dittos in main

#25=MECHANICAL_CONTEXT(' ',#1,'mechanical');
#45=(GEOMETRIC_REPRESENTATION_CONTEXT(3) ..
#57=(GEOMETRIC_REPRESENTATION_CONTEXT(3)

#24=DESIGN_CONTEXT(' ',#1,'design');
#35=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE(' ',' ',#32,.NOT_KNOWN.);
#54=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE(' ',' ',#53,.NOT_KNOWN.);

// Master; #197=empty. //
#32=PRODUCT('*MASTER',' ',' ',(#25));
#35=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE(' ',' ',#32,.NOT_KNOWN.);
#41=PRODUCT_DEFINITION(' ',' ',#35,#24);
#44=PRODUCT_DEFINITION_SHAPE(' ',' ',#41);
#197=SHAPE_DEFINITION_REPRESENTATION(#44,#52);
#52=SHAPE_REPRESENTATION('*MASTER',(#169,#182,#191),#45);
#198=SHAPE_REPRESENTATION_RELATIONSHIP(' ',' ',#52,#48);
#48=GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION('*MASTER',(#49),#45);
#49=GEOMETRIC_SET(

// 3 dittos in master //
#170=NEXT_ASSEMBLY_USAGE_OCCURRENCE('*DIT1','*DIT1','*DIT1',#41,#55,' ');
#183=NEXT_ASSEMBLY_USAGE_OCCURRENCE('*DIT2','*DIT2','*DIT2',#41,#55,' ');
#192=NEXT_ASSEMBLY_USAGE_OCCURRENCE('*DIT3','*DIT3','*DIT3',#41,#55,' ');

#176=PRODUCT_DEFINITION_SHAPE(' ',' ',#170);
#185=PRODUCT_DEFINITION_SHAPE(' ',' ',#183);
#194=PRODUCT_DEFINITION_SHAPE(' ',' ',#192);

#178=CONTEXT_DEPENDENT_SHAPE_REPRESENTATION(#177,#176);
#187=CONTEXT_DEPENDENT_SHAPE_REPRESENTATION(#186,#185);
#196=CONTEXT_DEPENDENT_SHAPE_REPRESENTATION(#195,#194);

#177=(REPRESENTATION_RELATIONSHIP(' ',' ',#52,#156)
     REPRESENTATION_RELATIONSHIP_WITH_TRANSFORMATION(#175) ..
#186=(REPRESENTATION_RELATIONSHIP(' ',' ',#52,#156)
     REPRESENTATION_RELATIONSHIP_WITH_TRANSFORMATION(#184) ..
#195=(REPRESENTATION_RELATIONSHIP(' ',' ',#52,#156)
     REPRESENTATION_RELATIONSHIP_WITH_TRANSFORMATION(#193) ..

#175=ITEM_DEFINED_TRANSFORMATION(' ',' ',#174,#169);
#184=ITEM_DEFINED_TRANSFORMATION(' ',' ',#174,#182);
#193=ITEM_DEFINED_TRANSFORMATION(' ',' ',#174,#191);


// detail-solid; #161=empty. //
#53=PRODUCT('DET1',' ',' ',(#25));
#54=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE(' ',' ',#53,.NOT_KNOWN.);
#55=PRODUCT_DEFINITION(' ',' ',#54,#24);
#56=PRODUCT_DEFINITION_SHAPE(' ',' ',#55);
#161=SHAPE_DEFINITION_REPRESENTATION(#56,#156);
#162=SHAPE_REPRESENTATION_RELATIONSHIP(' ',' ',#156,#151);
#156=SHAPE_REPRESENTATION('DET1',(#160,#174),#57);
#151=ADVANCED_BREP_SHAPE_REPRESENTATION('DET1',(#59),#57);
#59=MANIFOLD_SOLID_BREP('*SOL1',#150);
#150=CLOSED_SHELL(

// curves for detail //
#163=SHAPE_REPRESENTATION_RELATIONSHIP(' ',' ',#156,#153);
#153=GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION('DET1',(#154),#57);
#154=GEOMETRIC_SET(


------------------------------------------------------------------------------
SHAPE_REPRESENTATION('*MASTER',(#169,#182,#191),#45);
  (detailName, (axis,axis,axis), GRC)
  gives primaryOrigin | refSys of all subModelReferences
  one SR per PRODUCT


PRODUCT_DEFINITION_SHAPE(id, description, link1)
  link1 = PRODUCT_DEFINITION | NEXT_ASSEMBLY_USAGE_OCCURRENCE
    PRODUCT_DEFINITION                 this obj is a subModel
    NEXT_ASSEMBLY_USAGE_OCCURRENCE     this is a subModelReference


NEXT_ASSEMBLY_USAGE_OCCURRENCE('','','',parent, child,'')
  parent=Link to PRODUCT_DEFINITION of main
  child=Link to PRODUCT_DEFINITION_WAD or PRODUCT_DEFINITION of Detail


// ITEM_DEFINED_TRANSFORMATION(,,parentAxis, childAxis)





//--------------------------------------------------------
// die folgenden sind scheinbar NICHT vom Model unabhaengig:

#1=APPLICATION_PROTOCOL_DEFINITION('','',1999,#49);
// (? ? APPLICATION_CONTEXT)

#49=APPLICATION_CONTEXT(
  'configuration controlled 3D design of mechanical parts and assemblies');
// C5 u VC

#44=DESIGN_CONTEXT('',#49,'design');
// (APPLICATION_CONTEXT)
// C5 u VC; nicht Rhi.

#47=MECHANICAL_CONTEXT('',#49,'mechanical');
// (APPLICATION_CONTEXT, .)
// nicht Rhi!

#13=PRODUCT_RELATED_PRODUCT_CATEGORY('','',(#46));
// (? PRODUCT)
// PRODUCT_RELATED_PRODUCT_CATEGORY bleibt offen; nicht referenziert. see Rhi1

#46=PRODUCT('Part','','not specified',(#47));
// ( . . (MECHANICAL_CONTEXT))

#45=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE('',' ',#46,.MADE.);
// ( 'last version', PRODUCT, .MADE./.NOT_KNOWN.)
// nicht C4 !

#43=PRODUCT_DEFINITION('','',#45,#44);
// ( . PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE, DESIGN_CONTEXT)

#40=PRODUCT_DEFINITION_SHAPE(' ',' ',#43) ;
// ( ? PRODUCT_DEFINITION)

#50=SHAPE_DEFINITION_REPRESENTATION(#40,#49) ;
// (PRODUCT_DEFINITION_SHAPE, SHAPE_REPRESENTATION)

#49=SHAPE_REPRESENTATION(' ',(#48),#223) ;
// ((1-n*AXIS2_PLACEMENT_3D), GEOMETRIC_REPRESENTATION_CONTEXT)

#223=(GEOMETRIC_REPRESENTATION_CONTEXT(3)
  GLOBAL_UNCERTAINTY_ASSIGNED_CONTEXT((#224))
  GLOBAL_UNIT_ASSIGNED_CONTEXT((#227,#226,#225))
  REPRESENTATION_CONTEXT('',''));
// (224=UNCERTAINTY_MEASURE_WITH_UNIT, 227=LENGTH_UNIT, 225=SI_UNIT)



//--------------------------------------------------------
// wireframe via GEOMETRICALLY_BOUNDED_WIREFRAME_SHAPE_REPRESENTATION
  GEOMETRICALLY_BOUNDED_WIREFRAME_SHAPE_REPRESENTATION
    GEOMETRIC_CURVE_SET
      objs...


// see t2.stp ex Rhino

#90=GEOMETRICALLY_BOUNDED_WIREFRAME_SHAPE_REPRESENTATION('',(#89),#107);
//  (#GEOMETRIC_CURVE_SET, #GEOMETRIC_REPRESENTATION_CONTEXT)

// Planare Begrenzte Flaeche; Methode2 (Rhi_sur1.stp):
#89=GEOMETRIC_CURVE_SET('Standard_curve_set',(#136,#137,#138));
// (3 x CARTESIAN_POINT)







//--------------------------------------------------------
// wireframe via GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION (
  GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION
    GEOMETRIC_SET
      objs...

// see point1.stp ex C5

#54=SHAPE_REPRESENTATION_RELATIONSHIP('','',#49,#52) ;
// (? SHAPE_REPRESENTATION, GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION)
// NICHT ERFORDERLICH !

#52=GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION('NONE',(#53),#46) ;
// (GEOMETRIC_SET, GEOMETRIC_REPRESENTATION_CONTEXT)
// nur ein SET !
// Pro SET je ein GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION !

#53=GEOMETRIC_SET('NONE',(#51,#55,#56)) ;
// zB 3 Punkte (CARTESIAN_POINT ..)
....


//--------------------------------------------------------
// Flaechen via MANIFOLD_SURFACE_SHAPE_REPRESENTATION:
  MANIFOLD_SURFACE_SHAPE_REPRESENTATION
    SHELL_BASED_SURFACE_MODEL
      OPEN_SHELL
        ADVANCED_FACEs...


#117=SHAPE_REPRESENTATION_RELATIONSHIP(' ',' ',#49,#116) ;
// (SHAPE_REPRESENTATION, MANIFOLD_SURFACE_SHAPE_REPRESENTATION)
// mehrere; je eine pro Flaeche ?

#116=MANIFOLD_SURFACE_SHAPE_REPRESENTATION('NONE',(#115),#46) ;
//  (SHELL_BASED_SURFACE_MODEL, GEOMETRIC_REPRESENTATION_CONTEXT)

#115=SHELL_BASED_SURFACE_MODEL('NONE',(#114));
// (OPEN_SHELL)      // ein oder mehrere !!! ZB: ('NONE',(#93,#138));

#114=OPEN_SHELL('Fill.4',(#113)) ;

#113=ADVANCED_FACE('',(#112),#86,.T.) ;
..


//--------------------------------------------------------
// Solid via MANIFOLD_SOLID_BREP
  ADVANCED_BREP_SHAPE_REPRESENTATION
    MANIFOLD_SOLID_BREP
      CLOSED_SHELL
        ADVANCED_FACEs...

// see wash1_mit.stp ex VC

#52=SHAPE_REPRESENTATION_RELATIONSHIP('','',#244,#51);
// ( ? SHAPE_REPRESENTATION ADVANCED_BREP_SHAPE_REPRESENTATION)
// SHAPE_REPRESENTATION_RELATIONSHIP ist offen (nicht mehr referenziert !)

#51=ADVANCED_BREP_SHAPE_REPRESENTATION('',(#158,#53),#223);
// ((AXIS2_PLACEMENT_3D, MANIFOLD_SOLID_BREP), GEOMETRIC_REPRESENTATION_CONTEXT)

#53=MANIFOLD_SOLID_BREP('',#52);
// (CLOSED_SHELL)

#52=CLOSED_SHELL('',(#96,#97,#98,#99,#100,#101));
// (lauter ADVANCED_FACEs)






//================================================================
ISO-10303-21;
HEADER;
FILE_DESCRIPTION((''),'2;1');
FILE_NAME('','2013-12-21T08:25:23+02:00',(''),(''),'gCAD3D','','');
FILE_SCHEMA (('CONFIG_CONTROL_DESIGN'));
ENDSEC;
DATA;
#1=(LENGTH_UNIT()NAMED_UNIT(*)SI_UNIT(.MILLI.,.METRE.));
#2=(NAMED_UNIT(*)PLANE_ANGLE_UNIT()SI_UNIT($,.RADIAN.));
#3=(NAMED_UNIT(*)SI_UNIT($,.STERADIAN.)SOLID_ANGLE_UNIT());
#4=UNCERTAINTY_MEASURE_WITH_UNIT(LENGTH_MEASURE(0.05),#1,'TOL_CRV','');
#5=DIMENSIONAL_EXPONENTS(0.,0.,0.,0.,0.,0.,0.);
#6=PLANE_ANGLE_MEASURE_WITH_UNIT(PLANE_ANGLE_MEASURE(0.017),#2);
#7=(CONVERSION_BASED_UNIT('DEGREES',#6)NAMED_UNIT(#5)PLANE_ANGLE_UNIT());
#8=APPLICATION_CONTEXT('design');


#10=PRODUCT('unknown','','None',(#11));
#11=PRODUCT_CONTEXT('None',#8,'mechanical');
#12=PRODUCT_DEFINITION_CONTEXT('3D Mechanical Parts',#8,'design');
#13=PRODUCT_DEFINITION('A','version 1',#14,#12);
#14=PRODUCT_DEFINITION_FORMATION('','None',#10);
#15=(GEOMETRIC_REPRESENTATION_CONTEXT(3)
GLOBAL_UNCERTAINTY_ASSIGNED_CONTEXT((#4))
GLOBAL_UNIT_ASSIGNED_CONTEXT((#3,#7,#1))
REPRESENTATION_CONTEXT('ID1','3D'));
#16=PRODUCT_DEFINITION_SHAPE('A','First version',#13);
#17=SHAPE_REPRESENTATION('',(#100),#15);
#18=SHAPE_DEFINITION_REPRESENTATION(#16,#17);


#100=AXIS2_PLACEMENT_3D('',#101,#102,#103);
#101=CARTESIAN_POINT('',(0.,0.,0.));
#102=DIRECTION('',(0.,0.,1.));
#103=DIRECTION('',(1.,0.,0.));

..

ENDSEC;
END-ISO-10303-21;
//================================================================





Modelfiles:
t01         ein C4-Modelfile !
tetr1       Tetraeder; V5



myTest1     manuell erstellt.





//==========================================================
*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
// die folgenden 2 Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int STP_w__ (char*);
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>              // for ...

#include <time.h>                // localtime


#include "../ut/ut_cast.h"             // LONG_PTR
#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../ut/ut_obj.h"              // UTO_stru_2_obj
#include "../ut/ut_txfil.h"            // UTF_GetPosLnr
#include "../ut/ut_memTab.h"           // MemTab

#include "../ut/func_types.h"               // UI_Func... SYM_..
#include "../gr/ut_DL.h"               // DL_GetAtt

#include "../db/ut_DB.h"               // DB_GetGTxt

#include "../xa/xa.h"                  // WC_modnam
#include "../xa/xa_mem.h"              // memspc201


// #include "../xa/xa_ui.h"               // UID_..
// #include "../xa/ut_ui_TX.h"            // TX_Print
// #include "../ut/byteorder.h"


typedef_MemTab(int);

static MemTab(int) ol_oSh = MemTab_empty;  // open-shell-objects
static MemTab(int) ol_GS  = MemTab_empty;  // obj's for GEOMETRIC_SET
// static MemTab(int) olSs = MemTab_empty;


static FILE *stpw_fp;
static int  stpw_li;
static int  stpw_OpenShell=0;    // link to MANIFOLD_SURFACE_SHAPE_REPRESENTATION

static char *stpwTrue={".T."}, *stpwFalse={".F."};
static char stptStrNull[4]={""};


// pb1, pb2  3D-box of outer-boundary
// bp        backplane of outer-boundary; 0,1,2
// ivx       -1 vector vx not yet created
// sro       senseOfRotation of outer-boundary; 0=CCW; 1=CW
// sri       senseOfRotation of inner-boundary;
// stat      0=create outer-boundary; 1=create inner-boundary;
typedef struct {Point po, pb1, pb2; Vector vz; int ipo, ivz, ivx;
                char bp, sro, sri, stat;}                           stpPln;




//================================================================
  int STP_w__ (char* fnam) {
//================================================================
// export step-file
// fnam muss korrekten fileTyp .stp haben !

  int   i1;
  long  l1;

 
  TX_Print ("%s exported ..",fnam);
  printf("test_stp_w |%s|\n",fnam);


  if ((stpw_fp = fopen (fnam, "w+")) == NULL) {
    TX_Error ("STP_w__ Open |%s|",fnam);
    return -1;
  }

  // init memspace for point-indextables imt
  MemTab_ini (&ol_oSh, sizeof(int), Typ_Int4, 10000);
  MemTab_ini (&ol_GS, sizeof(int), Typ_Int4, 10000);
  // MemTab_ini (&olSs, sizeof(int), Typ_Int4, 10000);


  STP_w_init ();

  stpw_li = 200;

  // output of solids (Bodies, as CLOSED_SHELL)
  STP_w_cs__ (&ol_oSh);

  // reset obj-list
  MemTab_clear (&ol_oSh);

  // output of yet usused surfaces as OPEN_SHELL
  STP_w_os__ (&ol_oSh);

  // reset obj-list
  MemTab_clear (&ol_oSh);
  // add Refsys to ol_oSh (cannot be empty)
  i1 = 100;
  MemTab_add (&ol_oSh, &l1, &i1, 1, 0);


  // output of yet usused curves as wireframe
  STP_w_wf__ (&ol_oSh);


  STP_w_end ();

  fclose (stpw_fp);

  // MemTab_free (&olSs);
  MemTab_free (&ol_GS);
  MemTab_free (&ol_oSh);


  return 0;

}


//================================================================
  int STP_w_init () {
//================================================================

  // extern double   UT_TOL_cv;

  char   cbuf[80];
  time_t actt;
  struct tm *Tm;

  // fix timeString
  time (&actt);
  Tm = localtime (&actt);
  strftime (cbuf, 80, "%Y-%m-%dT%H:%M:%S", Tm);
  // %Z (ZeitZone) liefert "CEST"
  // %z liefert "+0200" statt "+02:00"
  strcat(cbuf,"+02:00");
    printf(" timeString=|%s|\n",cbuf);



  fprintf(stpw_fp,"ISO-10303-21;\n");
  fprintf(stpw_fp,"HEADER;\n");
  fprintf(stpw_fp,"FILE_DESCRIPTION((''),'2;1');\n");
  fprintf(stpw_fp,"FILE_NAME('','%s',(''),(''),'gCAD3D','','');\n",cbuf);
  fprintf(stpw_fp,"FILE_SCHEMA (('CONFIG_CONTROL_DESIGN'));\n");
  fprintf(stpw_fp,"ENDSEC;\n");

  fprintf(stpw_fp,"DATA;\n");

  // Definition Units, Tolerances:
  // get UT_TOL_cv --> cbuf
  cbuf[0] = '\0';
  UTX_add_fl_u (cbuf, UT_DISP_cv);
    printf(" tolString=|%s|\n",cbuf);


  fprintf(stpw_fp,
"#1=(LENGTH_UNIT()NAMED_UNIT(*)SI_UNIT(.MILLI.,.METRE.));\n");
  fprintf(stpw_fp,
"#2=(NAMED_UNIT(*)PLANE_ANGLE_UNIT()SI_UNIT($,.RADIAN.));\n");
  fprintf(stpw_fp,
"#3=(NAMED_UNIT(*)SI_UNIT($,.STERADIAN.)SOLID_ANGLE_UNIT());\n");
  fprintf(stpw_fp,
"#4=UNCERTAINTY_MEASURE_WITH_UNIT(LENGTH_MEASURE(%s),#1,'TOL_CRV','');\n",
          cbuf); // Tol
/* #167=UNCERTAINTY_MEASURE_WITH_UNIT(LENGTH_MEASURE(0.001),#158,
'DISTANCE_ACCURACY_VALUE', ..*/


  fprintf(stpw_fp,
"#5=DIMENSIONAL_EXPONENTS(0.,0.,0.,0.,0.,0.,0.);\n");
// value 0.0174532925199 makes degrees -> radians !
  fprintf(stpw_fp,
"#6=PLANE_ANGLE_MEASURE_WITH_UNIT(PLANE_ANGLE_MEASURE(0.0174532925199),#2);\n");
  fprintf(stpw_fp,
"#7=(CONVERSION_BASED_UNIT('DEGREES',#6)NAMED_UNIT(#5)PLANE_ANGLE_UNIT());\n");
  fprintf(stpw_fp,
"#8=APPLICATION_CONTEXT('design');\n");



  fprintf(stpw_fp,
"\n\n/*===========================================*/\n");
  fprintf(stpw_fp,
"#10=PRODUCT('%s','','None',(#11));\n",WC_modnam);
  fprintf(stpw_fp,
"#11=PRODUCT_CONTEXT('None',#8,'mechanical');\n");
  fprintf(stpw_fp,
"#12=PRODUCT_DEFINITION_CONTEXT('3D Mechanical Parts',#8,'design');\n");
  fprintf(stpw_fp,
"#13=PRODUCT_DEFINITION('A','version 1',#14,#12);\n");
  fprintf(stpw_fp,
"#14=PRODUCT_DEFINITION_FORMATION('','None',#10);\n");
  fprintf(stpw_fp,
"#15=(GEOMETRIC_REPRESENTATION_CONTEXT(3)\n");
  fprintf(stpw_fp,
"GLOBAL_UNCERTAINTY_ASSIGNED_CONTEXT((#4))\n");
  fprintf(stpw_fp,
"GLOBAL_UNIT_ASSIGNED_CONTEXT((#3,#7,#1))\n");
  fprintf(stpw_fp,
"REPRESENTATION_CONTEXT('ID1','3D'));\n");
  fprintf(stpw_fp,
"#16=PRODUCT_DEFINITION_SHAPE('A','First version',#13);\n");
/*
  fprintf(stpw_fp,
"#17=SHAPE_REPRESENTATION('',(#100),#15);\n");
  fprintf(stpw_fp,
"#18=SHAPE_DEFINITION_REPRESENTATION(#16,#17);\n");
*/
  fprintf(stpw_fp,"\n");
  fprintf(stpw_fp,"\n");



  fprintf(stpw_fp,
"/*===========================================*/\n");


  fprintf(stpw_fp,"#100=AXIS2_PLACEMENT_3D('',#101,#102,#103);\n");
  fprintf(stpw_fp,"#101=CARTESIAN_POINT('',(0.,0.,0.));\n");
  fprintf(stpw_fp,"#102=DIRECTION('',(0.,0.,1.));\n");
  fprintf(stpw_fp,"#103=DIRECTION('',(1.,0.,0.));\n");




/*
  fprintf(fp,"#7=DESIGN_CONTEXT('',#6,'design');\n");
  fprintf(fp,"#8=MECHANICAL_CONTEXT('',#6,'mechanical');\n");
  fprintf(fp,"#9=PRODUCT_RELATED_PRODUCT_CATEGORY('','',(#10));\n");
  fprintf(fp,"#10=PRODUCT('ProdNam','','',(#8));\n");
  fprintf(fp,"#11=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE"
                  "('',' ',#10,.NOT_KNOWN.);\n");
  fprintf(fp,"#12=PRODUCT_DEFINITION('','',#11,#7);\n");
  fprintf(fp,"#13=PRODUCT_DEFINITION_SHAPE(' ',' ',#12);\n");
  fprintf(fp,"#14=SHAPE_DEFINITION_REPRESENTATION(#13,#15);\n");
  fprintf(fp,"#15=SHAPE_REPRESENTATION(' ',(#17),#16);\n");

*/


  return 0;

}


//================================================================
  int STP_w_os_init () {
//================================================================
// #114=OPEN_SHELL('Fill.4',(#213)) ;
// #115=SHELL_BASED_SURFACE_MODEL('NONE',(#114)) ;
// #116=MANIFOLD_SURFACE_SHAPE_REPRESENTATION('NONE',(#115),#15) ;
//
// #117=SHAPE_REPRESENTATION_RELATIONSHIP(' ',' ',#262,#116) ;

//#262=SHAPE_REPRESENTATION
//GRC=GEOMETRIC_REPRESENTATION_CONTEXT..=#15

// ol_oSh = list of surface-objects -> SHELL_BASED_SURFACE_MODEL

// NUR FUER SUPPORT-SURFACES ?

  
  int   iNr;
  char  s1[128];
  
  
  printf("STP_w_os_init \n");

  iNr = MEMTAB_IND (&ol_oSh);
  if(iNr < 1) return -1;


  fprintf(stpw_fp,"\n/* set of open-shell-objects */\n");

  sprintf(s1,"#%d=OPEN_SHELL(''",stpw_li);
  ++stpw_li;
  STP_w_list__ (s1, MEMTAB_DAT (&ol_oSh), iNr, 1, ",");
  fprintf(stpw_fp,"%s);\n",s1);

  fprintf(stpw_fp,
"#%d=SHELL_BASED_SURFACE_MODEL('',(#%d));\n",
          stpw_li, stpw_li - 1);
  ++stpw_li;

  fprintf(stpw_fp,
"#%d=MANIFOLD_SURFACE_SHAPE_REPRESENTATION('',(#%d),#15);\n",
          stpw_li, stpw_li - 1);
  ++stpw_li;

  // keep link -> MANIFOLD_SURFACE_SHAPE_REPRESENTATION
  stpw_OpenShell = stpw_li - 1;


/*
  fprintf(stpw_fp,
"#%d=SHAPE_REPRESENTATION_RELATIONSHIP('','',#17,#%d);\n",
          stpw_li, stpw_li - 1);
  ++stpw_li;
*/


/* no ADVANCED_FACEs with GEOMETRIC_SET
  sprintf(s1,"#%d=GEOMETRIC_SET(''",stpw_li);
  ++stpw_li;
  STP_w_list__ (s1, MEMTAB_DAT (&ol_oSh), iNr, 1, ",");
  fprintf(stpw_fp,"%s);\n",s1);
    
  fprintf(stpw_fp,
"#%d=GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION('',(#%d),#15);\n",
          stpw_li, stpw_li - 1);
  ++stpw_li;
*/

  fprintf(stpw_fp, "\n\n");

  return 0;

}


//============================================================================
  int STP_w_wf_init () {
//============================================================================
// output of obj-list ol_GS as GEOMETRIC_SET of _WIREFRAME_SHAPE_REP..

// // create a GEOMETRIC_SET from all objs in ol_GS
// #261=GEOMETRIC_SET('',(#233,#234));
// list GEOMETRIC_CURVE_SET in ol_GS
// #262=SHAPE_REPRESENTATION('',(#240,#247,#254,#260,#261),#15);
// #263=SHAPE_DEFINITION_REPRESENTATION(#16,#262);
// // pointsAndCurves, OpenShells
// #264=SHAPE_REPRESENTATION_RELATIONSHIP('','',#262,#232);

// #15=(GEOMETRIC_REPRESENTATION_CONTEXT
// #16=PRODUCT_DEFINITION_SHAPE



  int   iNr, iPt, iSs, iSR, iGBSR;
  long  l1;
  char  s1[128];


  printf("STP_w_wf_init \n");


  fprintf(stpw_fp,"\n/* set of wireframe-objects */\n");


  // create GEOMETRIC_SET from RECTANGULAR_TRIMMED_SURFACEs
  // only for supportSurfaces (eg RECTANGULAR_TRIMMED_SURFACE):
  iPt = MEMTAB_IND (&ol_GS);
  if(iPt > 0) {
    sprintf(s1,"#%d=GEOMETRIC_SET(''",stpw_li);
    ++stpw_li;
    STP_w_list__ (s1, MEMTAB_DAT (&ol_GS), iPt, 1, ",");
    fprintf(stpw_fp,"%s);\n",s1);

    iGBSR = stpw_li;
    fprintf(stpw_fp,
      "#%d=GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION('',(#%d),#15);\n",
          stpw_li, stpw_li - 1);
    // same results with GEOMETRICALLY_BOUNDED_WIREFRAME_SHAPE_REPRESENTATION ..
    ++stpw_li;
  }


  // primaryOrigin | refSys of all subModelReferences
  // one SR per PRODUCT
  iSR = stpw_li;
  sprintf(s1,"#%d=SHAPE_REPRESENTATION(''",stpw_li);
  ++stpw_li;
  fprintf(stpw_fp,"%s,(#100),#15);\n",s1);

  

  //   link to GEOMETRICALLY_BOUNDED_WIREFRAME_SHAPE_REPRESENTATION
  if(iPt> 0) {
    fprintf(stpw_fp,
            "#%d=SHAPE_REPRESENTATION_RELATIONSHIP('','',#%d,#%d);\n",
            stpw_li, iSR, iGBSR);
    ++stpw_li;
  }


  //   link to SHAPE_REPRESENTATION
  fprintf(stpw_fp,
          "#%d=SHAPE_DEFINITION_REPRESENTATION(#16,#%d);\n",
          stpw_li, iSR);
  ++stpw_li;


  // links to SHAPE_REPRESENTATION & MANIFOLD_SURFACE_SHAPE_REPRESENTATION
  // only if a openShell exists
  if(stpw_OpenShell) {
    fprintf(stpw_fp,
            "#%d=SHAPE_REPRESENTATION_RELATIONSHIP('','',#%d,#%d);\n",
            stpw_li, iSR, stpw_OpenShell);
    ++stpw_li;
  }

  fprintf(stpw_fp, "\n\n");




  return 0;

}

//================================================================
  int STP_w_cs__ () {
//================================================================
// output of yet usused surfaces as OPEN_SHELL

  int      irc, apt_typ, anz_obj;
  long     l1, apt_ind, gr_ind;
  char     oid[40];
  void     *vDat;
  DL_Att   dla;
  ObjGX    ox1, *op1;


  printf("STP_w_cs__ \n");

  fprintf(stpw_fp,"\n\n/*===========================================*/\n");
  fprintf(stpw_fp,"/*  closed-shell-objects */\n");

  return 0;

}


//================================================================
  int STP_w_os__ () {
//================================================================
// output of yet usused surfaces as OPEN_SHELL

  int      irc, apt_typ, anz_obj;
  long     l1, apt_ind, gr_ind;
  char     oid[40];
  void     *vDat;
  DL_Att   dla;
  ObjGX    ox1, *op1;


  printf("STP_w_os__ \n");


  fprintf(stpw_fp,"\n\n/*===========================================*/\n");
  fprintf(stpw_fp,"/*  open-shell-objects */\n");

  anz_obj = 0;
  gr_ind = GL_Get_DLind();

    
  for(l1=0; l1<gr_ind; ++l1) {

    // dla = DL_GetAtt(l1);                      // get alle DL-Infos
    irc = DL_get_dla (&dla, l1);                      // get alle DL-Infos
    if(!irc) continue;

    // if(dla.disp == OFF) continue;           // skip hidden obj's
    if(DL_OBJ_IS_HIDDEN(dla)) continue;        // skip hidden obj's

    apt_typ = dla.typ;
    apt_ind = dla.ind;

    if(apt_typ != Typ_SUR) continue;
    

    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;  // skip unknown types

    // dTyp = DB_GetObjDat (&vDat, apt_typ, apt_ind);
    // if(dTyp == Typ_Error) continue;

    APED_oid_dbo__ (oid, apt_typ, apt_ind);

    irc = STP_w_os_wo (&ox1, oid); // Element ausgeben
    // printf(" ex dxfw_rec %d\n",irc);
    if(irc >= 0) ++anz_obj;

  }


  // output of obj-list ol_oSh as OPEN_SHELL
  irc = MEMTAB_IND (&ol_oSh);
  if(irc > 0) {
    STP_w_os_init ();
  }
  return 0;

}



//================================================================
  int STP_w_wf__ () {
//================================================================
 
  int         irc, anz_obj, apt_typ, anz_sur, dTyp;
  long        l1, apt_ind, gr_ind;
  char        oid[40];
  void        *vDat;
  DL_Att      dla;
  ObjGX       ox1, *op1;


  printf("STP_w_wf__ \n");


  fprintf(stpw_fp,"\n\n/*===========================================*/\n");
  fprintf(stpw_fp,"/*  wireframe-objects */\n");

  anz_obj = 0;
  anz_sur = 0;
  gr_ind = GL_Get_DLind();


  for(l1=0; l1<gr_ind; ++l1) {

    // dla = DL_GetAtt(l1);                      // get alle DL-Infos
    irc = DL_get_dla (&dla, l1);  
    if(!irc) continue;

    // if(dla.disp   == OFF) continue;           // skip hidden obj's
    if(DL_OBJ_IS_HIDDEN(dla)) continue;        // skip hidden obj's

    apt_typ = dla.typ;
    apt_ind = dla.ind;

    if(apt_typ == Typ_SUR) { anz_sur += 1; continue; }   // erst em Ende ..

    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;  // skip unknown types

    // dTyp = DB_GetObjDat (&vDat, apt_typ, apt_ind);
    // if(dTyp == Typ_Error) continue;

    APED_oid_dbo__ (oid, apt_typ, apt_ind);

    irc = STP_w_wf_wo (&ox1, oid, 2); // Element ausgeben
    // printf(" ex dxfw_rec %d\n",irc);
    if(irc >= 0) ++anz_obj;

  }

  // output points,curves,untrimmedSurfs in GEOMETRIC_SET
  STP_w_wf_init ();

  return 0;

}


//=====================================================================
  int STP_w_os_wo (ObjGX *ox1, char *oid) {
//=====================================================================
// export surface ox1

  int     typ, form, sTyp, ii;
  long    l1;
  void    *data;


  printf("========================================= \n");
  printf("STP_w_os_wo typ=%d form=%d\n",ox1->typ,ox1->form);
  typ  = ox1->typ;
  form = ox1->form;
  data = ox1->data;

  // get surfaceType
  sTyp = UTO_ck_surfTyp (ox1); // Typ_SURTPS| ?
    printf(" sTyp=%d\n",sTyp);


  fprintf(stpw_fp,"\n/* Surface %s */\n",oid);


  switch(typ) {

    //----------------------------------------------------------------
    case Typ_SURCIR:
      // ADVANCED_FACE
      ii = STP_w_SURCIR ((ObjGX*)ox1, oid);
      break;


    //----------------------------------------------------------------
    case Typ_SUR:         // sTyp Typ_SURTPS
      // ADVANCED_FACE < surpln
      ii = STP_w_SURTP__ ((ObjGX*)ox1, oid);
      break;

    //----------------------------------------------------------------
    case Typ_SURRV:    // 54
      ii = STP_w_SURRV (data, oid, 4);
      break;

    //----------------------------------------------------------------
    case Typ_SURRU:    // 53
      ii = STP_w_SURRU (data, oid, 4);
      break;

    //----------------------------------------------------------------
    case Typ_SURBSP:    // 55
      ii = STP_w_SURBSP (data, oid);
      // add unlimited support-surface to GEOMETRIC_SET
      if(ii > 0) MemTab_add (&ol_GS, &l1, &ii, 1, 0);
      return 0;

    //----------------------------------------------------------------
    default:
      printf("***** STP_w_os_wo unsupported: %d\n",typ);
      return -1;
  }



  // add trimmed surf to active-list
  if(ii > 0) MemTab_add (&ol_oSh, &l1, &ii, 1, 0);

  return 0;

}


//================================================================
  int STP_w_SURBSP (SurBSpl *su1, char *oid) {
//================================================================
// Area: B-Spline-Surf   Typ_SURBSP (55)
//
// B_SPLINE_SURFACE_WITH_KNOTS(name, u_deg, v_deg,
//   ((cv1_points),(cvn_points)),
//   .UNSPECIFIED.,.U.,.U.,.U.,
//   (u_multiplicities),(v_multiplicities),(u_knots),(v_knots),knot_type);
//
// surf_form, u-closed, v-closed_curve, self_intersect, .. ??
//    .UNSPECIFIED.,.F.,.F.,.U.,
//    .PLANE_SURF.,.F.,.F.,.U.,     // degrees 1,1,((2-points),(2-points))
//                                  // (2,2),(2,2),(0.,1.),(0.,1.)
//
// surface_form:
//   plane_surf, generalised_cone, toroidal_surf, conical_surf,
//   spherical_surf, unspecified, ruled_surf, surf_of_revolution,
//   cylindrical_surf, quadric_surf
// knot_type:
//   uniform_knots|quasi_uniform_knots|piecewise_bezier_knots|unspecified

// knot_multiplicities; total size is ptNr+deg+1

// ('',1,1,((2-points),(2-points)),.PLANE_SURF.,.F.,.F.,.U.,
//         (2,2),(2,2),(0.,1.),(0.,1.),.UNSPECIFIED.);
// ('',1,5,((6-points),(6-points)),.UNSPECIFIED.,.U.,.U.,.U.,
//         (2,2),(6,6),(0.,1.),(0.,1.),.UNSPECIFIED.);
// ('',5,5,((6*6-points)),.UNSPECIFIED.,.U.,.U.,.U.,
//         (6,6),(6,6),(0.,1.),(0.,1.),.UNSPECIFIED.);

// typedef struct {long ptUNr, ptVNr; int  degU, degV;
//                 double v0U, v1U, v0V, v1V, *kvTabU, *kvTabV;
//                 Point *cpTab;}                                      SurBSpl;
// cpTab[ptUNr*ptVNr]    u0v0,u1v0..uUv0;u1v0,u1v1..uUv1;..;uUv0,uUv1..uUvV;
// kvTabU[ptUNr+degU+1]
// kvTabV[ptVNr+degV+1]


  int     ipa, i1, i2, iss, iba, iU, iV, uNr, vNr, *mU, *mV, im;
  long    l1;
  double  d1, d2, d3, d4, *dU, *dV;
  char    s1[128], *p1;
  Point   *pa;


  printf("STP_w_SURBSP %s\n",oid);
  UT3D_stru_dump (Typ_SURBSP, su1, "SurBSpl");
  // UTO_dump__ (oxi, "SurBSpl");
    

  fprintf(stpw_fp,"/* Bspline-Surface %s */\n",oid);

  // define points
  ipa = stpw_li;
  pa = su1->cpTab;
  i2 = su1->ptUNr * su1->ptVNr;
  for(i1=0; i1<i2; ++i1) STP_w_PT (&pa[i1], "");


  // bspline (degree,
  p1 = oid;
  iss = stpw_li;
  sprintf(s1, "#%d=B_SPLINE_SURFACE_WITH_KNOTS('%s',%d,%d,",stpw_li,
          p1, su1->degV, su1->degU);   // V then U !! ??
          // p1, su1->degU, su1->degV);   // V then U !! ??


  // add list of points
  i2 = ipa;
  STP_w_list_is (s1, i2, su1->ptUNr, "(");
  for(i1=1; i1<su1->ptVNr; ++i1) {
    i2 += su1->ptUNr;
    STP_w_list_is (s1, i2, su1->ptUNr, ",");
  }


  // curve_form:  unspecified, polyline_form, parabolic_arc, circular_arc,
  //              hyperbolic_arc, elliptic_arc
  STP_w_txt (s1, "),.UNSPECIFIED.");

  // u-closed, v-closed, self_intersect
  STP_w_txt (s1, ",.U.,.U.,.U.");

  // loop tru kvTab; size is ptNr+deg+1
  // set multiplicity = nr of identical values.
  iU = su1->ptUNr + su1->degU + 1;
  iV = su1->ptVNr + su1->degV + 1;
    printf(" iU=%d iV=%d\n",iU,iV);
  mU = (int*)UME_alloc_tmp (iU * sizeof(int));
  mV = (int*)UME_alloc_tmp (iV * sizeof(int));
  dU = (double*)UME_alloc_tmp (iU * sizeof(double));
  dV = (double*)UME_alloc_tmp (iV * sizeof(double));

  //----------------------------------------------------------------
  uNr = 0;
  im = 0;
  i1 = 0;
  for(i2=1; i2<iU; ++i2) {
    i1 = i2 - 1;
    ++im;
      printf(" ck i1=%d i2=%d im=%d\n",i1,i2,im);
    if(su1->kvTabU[i1] == su1->kvTabU[i2]) continue;
    // add [i1-1] to ia,da.
    mU[uNr] = im;
    dU[uNr] = su1->kvTabU[i1];
    ++uNr;
    im = 0;
  }
  // ouput last
  ++im;
  mU[uNr] = im;
  dU[uNr] = su1->kvTabU[i1];
  ++uNr;
    for(i1=0;i1<uNr;++i1) printf("   m=%d k=%lf\n",mU[i1],dU[i1]);

  //----------------------------------------------------------------
  vNr = 0;
  im = 0;
  i1 = 0;
  for(i2=1; i2<iV; ++i2) {
    i1 = i2 - 1;
    ++im;
      printf(" ck i1=%d i2=%d im=%d\n",i1,i2,im);
    if(su1->kvTabV[i1] == su1->kvTabV[i2]) continue;
    // add [i1-1] to ia,da.
    mV[vNr] = im;
    dV[vNr] = su1->kvTabV[i1];
    ++vNr;
    im = 0;
  }
  // ouput last
  ++im;
  mV[vNr] = im;
  dV[vNr] = su1->kvTabV[i1];
  ++vNr;
    for(i1=0;i1<vNr;++i1) printf("   m=%d k=%lf\n",mV[i1],dV[i1]);

  //----------------------------------------------------------------
  // output multiplicities iV
  STP_w_list__ (s1, mV, vNr, 0, ",");
  // output multiplicities iU
  STP_w_list__ (s1, mU, uNr, 0, ",");
  // output knots dV
  STP_w_list__ (s1, dV, vNr, 2, ",");
  // output knots dU
  STP_w_list__ (s1, dU, uNr, 2, ",");
/*
  // output multiplicities iU
  STP_w_list__ (s1, mU, uNr, 0, ",");
  // output multiplicities iV
  STP_w_list__ (s1, mV, vNr, 0, ",");
  // output knots dU
  STP_w_list__ (s1, dU, uNr, 2, ",");
  // output knots dV
  STP_w_list__ (s1, dV, vNr, 2, ",");
*/

  // knot_type
  STP_w_txt (s1, ",.UNSPECIFIED.");

  fprintf(stpw_fp,"%s);\n",s1);
  ++stpw_li;

  return iss;

}


//================================================================
  int STP_w_SURRU (ObjGX *srTab, char *oid, int mode) {
//================================================================
// SURFACE_OF_REVOLUTION('',<coverCurve>,<AXIS1_PLACEMENT>);
// coverCurve: LINE|B_SPLINE_CURVE_WITH_KNOTS
// unlimited: direct into GEOMETRIC_SET
//
//   srTab   2 ObjGX-objects     
//   mode    3=trimmed,perforated;
//           4=unlimited
 

  // ObjGX  *oTab;


  printf("STP_w_SURRU %s %d\n",oid,mode);
  UT3D_stru_dump (srTab[0].form, srTab[0].data, "SurRU[0]=");
  UT3D_stru_dump (srTab[1].form, srTab[1].data, "SurRU[1]=");
  // oTab = sru->data;


  // if one obj is a vector: create a SURFACE_OF_LINEAR_EXTRUSION
  // if(oTab[1].typ == Typ_VC)




  return 0;

}


//================================================================
  int STP_w_LINEAR_EXTRUSION () {
//================================================================
// SURFACE_OF_LINEAR_EXTRUSION('',<curve>,<VECTOR>)
//   curve: LINE|B_SPLINE_CURVE_WITH_KNOTS
// unlimited: direct into GEOMETRIC_SET
//
//   mode    3=trimmed,perforated;
//           4=unlimited

 
  printf("****** TODO STP_w_LINEAR_EXTRUSION \n");

  return 0;

}


//================================================================
  int STP_w_SURRV (SurRev *srv, char *oid, int mode) {
//================================================================
// Typ_SURRV (54)
// mode   3    create x_SURFACE
//        4    create RECTANGULAR_TRIMMED_SURFACE < x_SURFACE

// typedef struct {double ang1, ang2, v0, v1;
//                 long indCen, indCov;  short typCen, typCov;
//                 unsigned dir:1;}                              SurRev;   
 

  int     typCov, isu, iRef, typCen, rNr;
  long    l1;
  double  d0, d1, d2, d3, d4;
  char    s1[128];
  void    *vpCov, *vpCen;


  printf("STP_w_SURRV %s\n",oid);
  UT3D_stru_dump (Typ_SURRV, srv, "SurRev=");


  // get centerline
  typCen = DB_GetObjDat (&vpCen, &rNr, srv->typCen, srv->indCen);
    UT3D_stru_dump (typCen, vpCen, "Cen=");


  // get Cov
  typCov = DB_GetObjDat (&vpCov, &rNr, srv->typCov, srv->indCov);
    UT3D_stru_dump (typCov, vpCov, "Cov=");



  // get iRef = link for refSys from centerline
  iRef = 100;
  if(srv->typCen == Typ_PLN) {
    iRef = STP_w_axis3_dbo (srv->typCen, srv->indCen);

  } else {
    TX_Print("STP_w_SURRV E001 %d",srv->typCen);
    return -1;
  }


  // get link for contourLine from CoverObject


  //----------------------------------------------------------------
  // cov=LINE; CONE|CYLINDER
  if(typCov == Typ_LN) {

    // get d1=distance and d2=opening-angle
    STP_w_con_1 (&d1, &d2, vpCen, vpCov);

    // for openingAngle == 0 must use CYLINDRICAL_SURFACE
    if(fabs(d2) < UT_TOL_Ang1) {
      sprintf(s1, "#%d=CYLINDRICAL_SURFACE('%s',#%d,%lf)",stpw_li,oid,iRef,d1);

    } else {
      // #300=CONICAL_SURFACE('',#100,radius,angle)
      sprintf(s1, "#%d=CONICAL_SURFACE('%s',#%d,%lf,%lf)",stpw_li,oid,iRef,d1,d2);
    }
    fprintf(stpw_fp,"%s;\n",s1);
    ++stpw_li;



  //----------------------------------------------------------------
  // cov=unknown type
  } else {
    TX_Print("STP_w_SURRV E002 %d",srv->typCov);
    return -1;
  }


  if(mode == 3) return (stpw_li - 1);


  //----------------------------------------------------------------
  // mode = 4:  RECTANGULAR_TRIMMED_SURFACE < x_SURFACE
// RECTANGULAR_TRIMMED_SURFACE('',#300, 180., 360.0, 0.0, 50.0,.T.,.T.);
// #401=RECTANGULAR_TRIMMED_SURFACE
//   u1            : parameter_value;
//   u2            : parameter_value;
//   v1            : parameter_value;
//   v2            : parameter_value;
//   usense        : BOOLEAN;
//   vsense        : BOOLEAN;

// CONICAL_SURFACE: degStart, degEnd, dist1, dist2, T T
// SPHERICAL_SURFACE TOROIDAL_SURFACE
// PLANE CYLINDRICAL_SURFACE SURFACE_OF_REVOLUTION

  d1 = UT_DEGREES(srv->ang1);
  d2 = UT_DEGREES(srv->ang2);


  // height from v0,v1
  if(typCov == Typ_LN) {
    d0 = UT3D_len_ln (vpCov);
    d3 = d0 * srv->v0;
    d4 = d0 * srv->v1;
  } else {
    TX_Print("STP_w_SURRV E004 %d",srv->typCov);
    return -1;
  }



  sprintf(s1,"#%d=RECTANGULAR_TRIMMED_SURFACE('',#%d,%lf,%lf,%lf,%lf,.T.,.T.)",
          stpw_li, stpw_li - 1, d1, d2, d3, d4);
  fprintf(stpw_fp,"%s;\n",s1);
  MemTab_add (&ol_GS, &l1, &stpw_li, 1, 0);
  ++stpw_li;

    
  // return isu;
  return (stpw_li - 1);  // 2015-02-18  ????

}


//================================================================
  int STP_w_con_1 (double *d1, double *d2, Plane *pl1, Line *ln1) {
//================================================================
// axis=plane, coverObj=line
// get d1=distance and d2=opening-angle

  double  ao, dCos;
  Point   px;
  Vector  vcl;

  UT3D_vc_ln (&vcl, ln1);
  UT3D_vc_setLength (&vcl, &vcl, 1.);

    UT3D_stru_dump (Typ_VC, &vcl, "vcl");
    UT3D_stru_dump (Typ_VC, &pl1->vz, "vz");

  
  // get opening-angle of axis-centerline
  dCos = UT3D_acos_2vc (&pl1->vz, &vcl);
  ao = ACOS(dCos);
  *d2 = UT_DEGREES(ao);
    printf(" ao=%lf dCos=%lf deg=%lf\n",ao,dCos,*d2);


  // get intersection-point pl1-ln1
  UT3D_pt_intlnpl (&px, pl1, ln1);

  *d1 = UT3D_len_2pt (&px, &pl1->po);

    printf("ex STP_w_con_1 d1=%lf d2=%lf\n",*d1,*d2);

  return 0;

}


//================================================================
  int STP_w_SURCIR (ObjGX *oxi, char *oid) {
//================================================================
// planar surface.
// ADVANCED FACE ((outerBoundary/InnerBoundary), supportSurf)
    
    
      
// see Tess_bound_SURPLN TSU_DrawSurTP SUP_addBound
      
  int       i1, ii, ptNr, ia[5], iel, iss, ipvx, ipvz, ipc;
  char      s1[128], *p1;
  Point     pa[5], *pTab;
  Line      ln1;
  Vector    vcn, vx;
  stpPln    plb;
    
    
  printf("STP_w_SURCIR %s\n",oid);
  UTO_dump__ (oxi, "S-planar");


  // init plb
  STP_w_plb_ini (&plb);


  // get all points of RCIR > pTab
  i1 = UT3D_cv_scir__ (&ptNr, pa, oxi);
  if(i1 < 0) return i1;
  pTab = pa;

  // add 1.point as last point
  pa[ptNr] = pa[0];
  ++ptNr;


  // create outer boundary as EDGE_LOOP
  // FACE_OUTER_BOUND < outerBoundary ob

  ln1.p1 = pa[0];
  for(i1=1; i1<ptNr; ++i1) {
    ln1.p2 = pa[i1];
    ii = STP_w_LN (&ln1, "", 3);
    ln1.p1 = ln1.p2;
    // add to list
    ia[i1-1] = ii;
  }

  // fprintf(stpw_fp,"/* %s outer-boundary */\n",oid);
  plb.stat = 0;
  iel = STP_w_EDGE_LOOP__ (ia, ptNr-1); //, oid);

  if(!plb.sro) p1 = stpwTrue;
  else         p1 = stpwFalse;
  sprintf(s1,"#%d=FACE_OUTER_BOUND('',#%d,%s)",stpw_li, iel, p1);
  fprintf(stpw_fp,"%s;\n",s1);
  iel =stpw_li;
  ++stpw_li;

  // get normalvector for polygon; sr always CCW
  UT3D_vc_perppta (&vcn, ptNr, pa);

  // create origin+axis as AXIS2_PLACEMENT_3D
  STP_w_stpPln_set (&plb, &pa[0], -1, &vcn, -1, 0);
  iss = STP_w_axis3_vz (&plb);

  // supporting surface PLANE
  sprintf(s1, "#%d=PLANE('',#%d)", stpw_li, iss);
  fprintf(stpw_fp,"%s;\n",s1);
  iss = stpw_li;
  ++stpw_li;



  // ADVANCED_FACE < FACE_OUTER_BOUND, FACE_BOUNDs, supportSurf
  return STP_w_ADVANCED_FACE (&iel, 1, iss);

}


//================================================================
  int STP_w_SURTP__ (ObjGX *oxi, char *oid) {
//================================================================
// trimmed, perforated surface.
// ADVANCED FACE ((outerBoundary/InnerBoundary), supportSurf)



// see Tess_bound_SURPLN TSU_DrawSurTP SUP_addBound

  int     irc, ssTyp, i1, io, pNr, ebeg, cnAct, isu, ibNr, *iba, iss;
  char    s1[128];
  ObjGX   *oa, *ob;
  void    *o1, *ssDat;
  Point   *pa;
  stpPln  plb;


  printf("STP_w_SURTP__ %s\n",oid);
  UTO_dump__ (oxi, "SURTP__");


  iba = (int*)UME_alloc_tmp (oxi->siz * sizeof(int));

  // init plb
  STP_w_plb_ini (&plb);


  // first record can be Typ_Typ="SurPLN" - ignore
  // or supportSurface;
  oa = oxi->data;
  io = 0;
  // if(oa[io].typ == Typ_Typ) styp = planar ..
  ++io;
  cnAct = 1;


  // next record is outer-boundary
  ob = &oa[io];
    // UTO_dump__ (ob, "outer-boundary[%d]",io);

  // create outer boundary as EDGE_LOOP
  // FACE_OUTER_BOUND < outerBoundary ob
  plb.stat = 0;
  iba[0] = STP_w_EDGE_LOOP_b (&plb, ob, oid);
  ibNr = 1;


  // all following records are inner-boundaries
  L_ib_nxt:
  ++io;
  ++cnAct;
  if(io >= oxi->siz) goto L_w1;
  ob = &oa[io];
      // UTO_dump__ (ob, "inner-bound[%d]",io);

    // create inner boundaries as EDGE_LOOP
    // FACE_BOUND from innerBoundary ob
    plb.stat = 1;
    iba[ibNr] = STP_w_EDGE_LOOP_b (&plb, ob, oid);

    ++ibNr;
    goto L_ib_nxt;


  //----------------------------------------------------------------
  L_w1:

  // create support-surface
    fprintf(stpw_fp,"/* supportSurface for %s */\n",oid);


  //----------------------------------------------------------------
  // get supportSurfaceType
  ssTyp = UTO_get_suppSurf (&ssDat, oxi);
    printf(" s.Typ=%d\n",ssTyp);



  //----------------------------------------------------------------
  // planar surface:
  if(ssTyp == Typ_SURPLN) {
    // create origin+axis as AXIS2_PLACEMENT_3D
    iss = STP_w_axis3_vz (&plb);

    sprintf(s1, "#%d=PLANE('',#%d)", stpw_li, iss);
    fprintf(stpw_fp,"%s;\n",s1); 
    iss = stpw_li;
    ++stpw_li;


  //----------------------------------------------------------------
  // B_Spline_Surf
  } else if(ssTyp == Typ_SURBSP) {
    iss = STP_w_SURBSP ((SurBSpl*)ssDat, oid);


  //----------------------------------------------------------------
  // Revolved surf
  } else if(ssTyp == Typ_SURRV) {   // 54
    iss = STP_w_SURRV ((SurRev*)ssDat, oid, 3);


  //----------------------------------------------------------------
  // Ruled Surf
  } else if(ssTyp == Typ_SURRU) {  // 53
    iss = STP_w_SURRU ((ObjGX*)ssDat, oid, 3);


  //----------------------------------------------------------------
  } else {TX_Error("STP_w_SURTP__ E001 %d",ssTyp); return -1;}


  // ADVANCED_FACE < FACE_OUTER_BOUND, FACE_BOUNDs, supportSurf
  isu = STP_w_ADVANCED_FACE (iba, ibNr, iss);

  return isu;

}


//================================================================
  int STP_w_ADVANCED_FACE (int *ia, int iNr, int iss) {
//================================================================
// ADVANCED_FACE < FACE_OUTER_BOUND, FACE_BOUNDs, supportSurf
// ADVANCED_FACE ((outerBound[,innerBounds]), supportSurf, T|F)
//   T|F ?

  char   s1[128], *p1;


// TODO ?
  p1 = stpwTrue;

  sprintf(s1,"#%d=ADVANCED_FACE(''",stpw_li);
  ++stpw_li;
  STP_w_list__ (s1, ia, iNr, 1, ",");
  fprintf(stpw_fp,"%s,#%d,%s);\n",s1,iss,p1);


  return (stpw_li - 1);

}


//================================================================
  int STP_w_EDGE_LOOP_b (stpPln *plb, ObjGX *oxi, char *oid) {
//================================================================
// create outer boundary as EDGE_LOOP (closed contour).
// plb->stat 0  outer-boundary   FACE_OUTER_BOUND
//           1  inner-boundary   FACE_BOUND
// vcn     output for outer-boundary;
//         input  for inner-boundaries

// FACE_OUTER_BOUND < EDGE_LOOP < ORIENTED_EDGE < EDGE_CURVE < outerBoundary
// create all subParts of boundary as EDGE_CURVEs
// create 1 ORIENTED_EDGE from 1 EDGE_CURVE
// create 1 EDGE_LOOP from all ORIENTED_EDGEs

// FACE_OUTER_BOUND (EDGE_LOOP, orientation);
// FACE_BOUND (EDGE_LOOP, orientation);
//   orientation: T|F

  int     i1, i2, ii, iob, typ, ip1, ip2, ipc, *ia, iNr;
  char    s1[128], *p1;
  void    *odc;
  Point   actPos;
  ObjGX   *oxa, *ox1;


  printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE \n");
  UTO_dump__ (oxi, "STP_w_EDGE_LOOP_b stat=%d",plb->stat);

  // get contour-data (resolv links)
  typ = UTO_obj_getp (&odc, &ii, oxi); 

  // typ = oxi->typ;
  ia = &ipc;
  iNr = 1;

  switch(typ) {

    //----------------------------------------------------------------
    case Typ_ObjGX:           // CCV
      // UTO_dump__ (odc, " CCV:");
      ia = (int*)UME_alloc_tmp (((ObjGX*)odc)->siz * sizeof(int));
      iNr = STP_w_CVCCV (plb, ia, odc, "", 3);
      break;


    //----------------------------------------------------------------
    case Typ_CVELL:  // must be closed !
      iob = STP_w_EL__ (plb, (CurvElli*)odc, "");
      ip1 = STP_w_PT (&((CurvElli*)odc)->p1, "");
      // ORIENTED_EDGE < EDGE_CURVE < outerBoundary
      ipc = STP_w_ORIENTED_EDGE (ip1, ip1, iob, 0);
      break;


    //----------------------------------------------------------------
    case Typ_CI:  // must be closed !
      // ORIENTED_EDGE < EDGE_CURVE < outerBoundary
      ipc = STP_w_AC_CC (plb, (Circ*)odc, "", 3);
      break;


    //----------------------------------------------------------------
    case Typ_CVPOL:
        // UT3D_stru_dump (Typ_CVPOL, odc, " PLG:");
      // ORIENTED_EDGE < EDGE_CURVE < outerBoundary
      ipc = STP_w_CVPOL (plb, (CurvBSpl*)odc, "", 3);
      break;


    //----------------------------------------------------------------
    case Typ_CVBSP:
      // ORIENTED_EDGE < EDGE_CURVE < outerBoundary
      ipc = STP_w_CVBSP (plb, &actPos, (CurvBSpl*)odc, "", 3);
      break;

    //----------------------------------------------------------------
    default:
      printf("******* STP_w_EDGE_LOOP_b unsupported: %d\n",typ);
      return -1;
  }

  

  if(plb->stat == 0) {   // outer-boundary
    fprintf(stpw_fp,"/* outer-boundary for %s*/\n",oid);
  } else {          // inner-boundary
    fprintf(stpw_fp,"/* inner-boundary for %s */\n",oid);
  }


  // EDGE_LOOP < ORIENTED_EDGE
  ipc = STP_w_EDGE_LOOP__ (ia, iNr);


  // create FACE_OUTER_BOUND from EDGE_LOOP ipc
  // set orientation T|F
  if(plb->stat == 0) {   // outer-boundary; CCW
    if(!plb->sro) p1 = stpwTrue;
    else          p1 = stpwFalse;
    sprintf(s1,"#%d=FACE_OUTER_BOUND('',#%d,%s)",stpw_li, ipc, p1);
  } else {          // inner-boundary; CW
    if(plb->sro)  p1 = stpwTrue;   // should use plb->sri
    else          p1 = stpwFalse;
    sprintf(s1,"#%d=FACE_BOUND('',#%d,%s)",stpw_li, ipc, p1);
  }
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;


  return (stpw_li - 1);

}


//================================================================
  int STP_w_ORIENTED_EDGE (int ip1, int ip2, int ipc, int iDir) {
//================================================================
// Input:
//   iDir   0=True(defined direction)
//          1=False(inverted direction)
// create EDGE_CURVE from 2 Trimpoints, 1 BaseCurve
// EDGE_CURVE (name, Trimpoint1, Trimpoint2, BaseCurve, T|F)
//   T|F same_sense; 
//
// ORIENTED_EDGE (name,*,*,1 EDGE_CURVE, T|F))
//   T|F orientation; always .T:; already set in EDGE_CURVE.

  int     iob, typ, iv1, iv2;
  char    s1[128], *p1;



  iv1 = stpw_li;
  sprintf(s1, "#%d=VERTEX_POINT('',#%d)",stpw_li, ip1);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;

  if(ip2 == ip1) {
    iv2 = iv1;
  } else {
    iv2 = stpw_li;
    sprintf(s1, "#%d=VERTEX_POINT('',#%d)",stpw_li, ip2);
    fprintf(stpw_fp,"%s;\n",s1);
    ++stpw_li;
  }



  if(iDir) p1 = stpwFalse;
  else     p1 = stpwTrue;
  sprintf(s1, "#%d=EDGE_CURVE('',#%d,#%d,#%d,%s)",
              stpw_li,           iv1, iv2, ipc, p1);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;



  p1 = stpwTrue;
  sprintf(s1, "#%d=ORIENTED_EDGE('',*,*,#%d,%s)",
              stpw_li, stpw_li - 1, p1);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;

  return (stpw_li - 1);

}

//================================================================
  int STP_w_EDGE_LOOP__ (int *ia, int iNr) {
//================================================================
// EDGE_LOOP < ORIENTED_EDGEs


  char    s1[128];


  sprintf(s1,"#%d=EDGE_LOOP(''",stpw_li);
  ++stpw_li;
  STP_w_list__ (s1, ia, iNr, 1, ",");
  fprintf(stpw_fp,"%s);\n",s1);

  return (stpw_li - 1);

}


//==========================================================================
  int STP_w_wf_wo (ObjGX *ox1, char *oid, int mode) {
//==========================================================================
// write wireframe-object
// mode 2 GEOMETRIC_SET < COMPOSITE_CURVE < COMPOSITE_CURVE_SEGMENT
 

  int     typ, form, ii, *ia;
  long    l1;
  void    *data;
  Point   actPos;
  stpPln  plb;


  printf("STP_w_wf_wo typ=%d form=%d\n",ox1->typ,ox1->form);
  typ  = ox1->typ;
  form = ox1->form;
  data = ox1->data;
    printf(" typ=%d form=%d\n",typ,form);



  switch(form) {

    //----------------------------------------------------------------
    case Typ_PT:
      ii = STP_w_PT ((Point*)ox1->data, oid);
      MemTab_add (&ol_GS, &l1, &ii, 1, 0);
      return 0;

    //----------------------------------------------------------------
    case Typ_LN:
      ii = STP_w_LN ((Line*)ox1->data, oid, 2);
      break;

    //----------------------------------------------------------------
    case Typ_CI:
      ii = STP_w_AC_CC (NULL, (Circ*)ox1->data, oid, 2);
      break;

    //----------------------------------------------------------------
    case Typ_CVELL:
      ii = STP_w_EL_CC (NULL, (CurvElli*)ox1->data, oid, 2);
      break;

    //----------------------------------------------------------------
    case Typ_CVPOL:
      ii = STP_w_CVPOL (NULL, (CurvPoly*)ox1->data, oid, 2);
      break;

    //----------------------------------------------------------------
    case Typ_CVBSP:
      ii = STP_w_CVBSP (NULL, &actPos, (CurvBSpl*)ox1->data, oid, 2);
      break;
  
    //----------------------------------------------------------------
    case Typ_ObjGX:
      ia = (int*)UME_alloc_tmp (((ObjGX*)ox1)->siz * sizeof(int));
      ii = STP_w_CVCCV (NULL, ia, ox1, oid, 2); // makes COMPOSITE_CURVE !
      goto L_exit;



    //----------------------------------------------------------------
    default:
      printf("***** STP_w_wf_wo unsupported: %d\n",typ);
      return -1;
  }

  
/*
  // add point|curve to active-list of curves
  if(mode == 1) {
    // GEOMETRIC_CURVE_SET
    if(ii > 0) MemTab_add (&ol_GS, &l1, &ii, 1, 0);
    return 0;
  }
*/

  // mode=2: 
  // write COMPOSITE_CURVE < from single obj ii
  ii = STP_w_CCV__ (&ii, 1, oid);


  // add point|curve ii to GEOMETRIC_SET
  L_exit:
  if(ii > 0) MemTab_add (&ol_GS, &l1, &ii, 1, 0);

  return 0;

}

 
//================================================================
  int STP_w_PT (Point *pt1, char *oid) {
//================================================================
// #288=CARTESIAN_POINT('',(-1314.598162,-89.562827,0.000000));
 
  char    s1[128];

  UT3D_stru_dump (Typ_PT, pt1, "STP_w_PT");
  // if(strlen(oid)) fprintf(stpw_fp,"/* Point %s */\n",oid);


  sprintf(s1, "#%d=CARTESIAN_POINT('%s',(%lf,%lf,%lf))",
              stpw_li, oid, pt1->x, pt1->y, pt1->z);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;

  return (stpw_li - 1);

}


//================================================================
  int STP_w_VC_d (Vector *vc1, char *oid) {
//================================================================
// #38=DIRECTION('',(1.,0.,0.));
// #37=VECTOR('',#38,1.);
  
  // int     ip1;
  char    s1[128];
  
  UT3D_stru_dump (Typ_VC, vc1, "STP_w_VC_d");


  sprintf(s1, "#%d=DIRECTION('',(%lf,%lf,%lf))",
              stpw_li, vc1->dx, vc1->dy, vc1->dz);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;
  
  return (stpw_li - 1);
  
} 
              
  

//================================================================
  int STP_w_VCdv (Vector *vc1, char *oid) {
//================================================================
// #38=DIRECTION('',(1.,0.,0.));
// #37=VECTOR('',#38,1.);

  char    s1[128];

  UT3D_stru_dump (Typ_VC, vc1, "STP_w_VCdv");


  STP_w_VC_d (vc1, oid); // DIRECTION

  sprintf(s1, "#%d=VECTOR('%s',#%d,1.)",
              stpw_li, oid, stpw_li - 1);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;

  return (stpw_li - 1);

}


//================================================================
  int STP_w_axis3_vz (stpPln *plb) {
//================================================================
// create AXIS2_PLACEMENT_3D from Z-vector
// if ipc>0 then this centerpoint is to be used; else get workPlaneOrigin.

// {Point po; Vector vz; int ipo, ivz;}     stpPln;

  Vector   vx;


  //----------------------------------------------------------------
  // origin at plb->po;
  // if ipo==0 then origin is not mandatory;
  //   test distance plb->po - workPlane-origin along vz;
  //     if d1 < tol then use active workPlane-origin
  if(plb->ipo < 1) {
    // test distance of po along vz; if 
// TODO: get origin of workPlane
    // plb->ipo = 101;     
    plb->ipo = STP_w_PT (&plb->po, "");
  }





  //----------------------------------------------------------------
  // create z-axis
  plb->ivz = STP_w_VC_d (&plb->vz, ""); // DIRECTION

  // create x-axis
  if(plb->ivx < 1) {
    UT3D_vc_perpvcplXY (&vx, &plb->vz);
    plb->ivx = STP_w_VC_d (&vx, "");           // DIRECTION
  }


  //----------------------------------------------------------------
  // create axis (AXIS2_PLACEMENT_3D(centerpoint, Z-axis, x-axis))
  return STP_w_axis3__ (plb->ipo, plb->ivz, plb->ivx, "");

}
  

//================================================================
  int STP_w_axis3_dbo (int typ, long dbi) {
//================================================================
// create AXIS2_PLACEMENT_3D from database-object
// dbo must be Typ_PLN

  int    ipc, ipvz, ipvx, ipx;
  void   *vp;

  if(typ != Typ_PLN) {
    TX_Print("STP_w_axis3_dbo E001 %d",typ);
    return -1;
  }

  // {Point po; Vector vx, vy, vz; double p;}  Plane;


  vp = DB_get_PLN (dbi);


  ipc = STP_w_PT (&((Plane*)vp)->po, "");

  // create z-axis
  ipvz = STP_w_VC_d (&((Plane*)vp)->vz, ""); // DIRECTION
  
  // create x-axis
  ipvx = STP_w_VC_d (&((Plane*)vp)->vx, ""); // DIRECTION

  // create axis for circ (AXIS2_PLACEMENT_3D(centerpoint, Z-axis, x-axis))
  ipx = STP_w_axis3__ (ipc, ipvz, ipvx, "");


  return ipx;


}

 
//================================================================
  int STP_w_axis3__ (int ipc, int ipvz, int ipvx, char *oid) {
//================================================================
// #100=AXIS2_PLACEMENT_3D('',#101,#102,#103);  // origin,z-vector,x-vector

  char    s1[128];

  sprintf(s1, "#%d=AXIS2_PLACEMENT_3D('%s',#%d,#%d,#%d)",
              stpw_li, oid, ipc,ipvz,ipvx);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;

  return (stpw_li - 1);

}


//================================================================
  int STP_w_LN (Line *ln1, char *oid, int mode) {
//================================================================
// Line
// mode=1 create GEOMETRIC_CURVE_SET
//      2 create COMPOSITE_CURVE_SEGMENT
//      3 create ORIENTED_EDGE < EDGE_CURVE < LINE

// #35=CARTESIAN_POINT('',(10.,0.,0.));
// #36=CARTESIAN_POINT('',(100.,0.,0.));
// #38=DIRECTION('',(1.,0.,0.));
// #37=VECTOR('',#38,1.);
// #34=LINE('',#35,#37);
// #33=TRIMMED_CURVE('',#34,(#35),(#36),.T.,.CARTESIAN.);
// #32=COMPOSITE_CURVE_SEGMENT(.CONTINUOUS.,.T.,#33);
// #31=COMPOSITE_CURVE('LN1',(#32),.U.) ;


  int     ip1, ip2, ivc, iln;
  char    s1[128];
  Vector  vc1;


  // skip lines with length < UT_TOL_cv;  (comes from CCV)
  if(UT3D_len_ln (ln1) < UT_TOL_cv) return 0;


  printf("STP_w_LN %s %d\n",oid,mode);
  fprintf(stpw_fp,"/* Line %s */\n",oid);


  ip1 = STP_w_PT (&ln1->p1, "");
  ip2 = STP_w_PT (&ln1->p2, "");

  UT3D_vc_ln (&vc1, ln1);
  ivc = STP_w_VCdv (&vc1, "");      // VECTOR, DIRECTION


  iln = stpw_li;
  sprintf(s1, "#%d=LINE('',#%d,#%d)",
              stpw_li, ip1, ivc);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;


  // mode=3: ORIENTED_EDGE < EDGE_CURVE
  if(mode == 3) {
    // EDGE_CURVE and ORIENTED_EDGE from baseCurve & 2 points
    return STP_w_ORIENTED_EDGE (ip1, ip2, iln, 0);
  }


  // mode=1|2
  iln = STP_w_TRIMMED_CURVE (iln, ip1, ip2, stpwTrue);

  if(mode == 1)
    return STP_w_CURVE_SET (&iln, 1);


  // mode=2:
  // COMPOSITE_CURVE < COMPOSITE_CURVE_SEGMENT < TRIMMED_CURVE
  return STP_w_CCV_seg (iln, oid);

}


//================================================================
  int STP_w_AC__ (stpPln *plb, Circ *ac1, char *oid) {
//================================================================
// create Circ-unlimited
// #101=CARTESIAN_POINT('',(0.,0.,0.));       // circ-center
// #102=DIRECTION('',(0.,0.,1.));             // circ-z-axis
// #103=DIRECTION('',(1.,0.,0.));             // circ-x-axis
// #100=AXIS2_PLACEMENT_3D('',#101,#102,#103);
// #43=CIRCLE('',#100,25.0);                  // axis, radius

// {Point p1, p2, pc; Vector vz; double rad, ango;} Circ;


  int     ipc, ipvz, ipvx, ipx, ici, sr;
  double  d1;
  char    s1[128], *p1;
  Vector  vz;
  Point   pts, pte;


  // printf("STP_w_AC__ %s\n",oid);
  UT3D_stru_dump (Typ_CI, ac1, "STP_w_AC__ %s",oid);
  // if(oid[0]) fprintf(stpw_fp,"/* Circle %s */\n",oid);


  d1 = fabs(ac1->rad);

  // centerpoint, startpoint, endpoint
  ipc = STP_w_PT (&ac1->pc, "");

  sr = UT3D_sr_ci (ac1);  // 0=CCW; 1=CW.

  vz = ac1->vz;
  if(sr) UT3D_vc_invert (&vz, &vz);  // inverted Z-vector

  STP_w_stpPln_set (plb, &ac1->pc, ipc, &vz, -1, sr);

  // create origin+axis as AXIS2_PLACEMENT_3D
  ipx = STP_w_axis3_vz (plb);

  // #43=CIRCLE('',#100,25.0);                  // axis, radius
  ici = stpw_li;
  sprintf(s1, "#%d=CIRCLE('%s',#%d,%lf)", stpw_li, oid, ipx, d1);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;

  return ici;

}


//================================================================
  int STP_w_AC_CC (stpPln *plb, Circ *ac1, char *oid, int mode) {
//================================================================
// mode=1 create GEOMETRIC_CURVE_SET from circ
//      2 create COMPOSITE_CURVE_SEGMENT from circ
//      3 create ORIENTED_EDGE < EDGE_CURVE < circ


// mode==1
// #43=CIRCLE('',#100,25.0);                  // axis, radius
// #42=TRIMMED_CURVE('',#43,(#44),(#45),.T.,.CARTESIAN.);
// #43=GEOMETRIC_CURVE_SET('',(#42));

// mode==2
// #43=CIRCLE('',#100,25.0);                  // axis, radius
// #42=TRIMMED_CURVE('',#43,(#44),(#45),.T.,.CARTESIAN.);
// #41=COMPOSITE_CURVE_SEGMENT(.CONTINUOUS.,.T.,#42);

// {Point p1, p2, pc; Vector vz; double rad, ango;} Circ;


  int     i1, ipc, ip1, ip2, ipvz, ipvx, ipx, ici, i360;
  double  d1;
  char    s1[128], *p1;
  Vector  vcn, vx;
  Point   pts, pte;
  stpPln  lPlb;


  // ck 360-deg
  i360 = UT3D_ck_ci360 (ac1);
    printf(" i360=%d\n",i360);
  if(!i360) p1 = oid;
  else      p1 = stptStrNull;

  printf("STP_w_AC_CC %s %d\n",oid,mode);
  // if(oid[0]) 
  fprintf(stpw_fp,"/* Circle %s */\n",oid);

  // must have axis
  if(!plb) plb = &lPlb;

  // create Circ-unlimited
  ici = STP_w_AC__ (plb, ac1, p1);

  // do not trim 360-deg-circ 
  if(i360) {
    // not 360: creat startPt/endPt
    ip1 = STP_w_PT (&ac1->p1, "");
    ip2 = STP_w_PT (&ac1->p2, "");
  } else {
    // 360 deg
    ip1 = STP_w_PT (&ac1->p1, "");
    ip2 = ip1;
  }
  
  // mode=3: ORIENTED_EDGE < EDGE_CURVE
  if(mode == 3) {
    // EDGE_CURVE and ORIENTED_EDGE from baseCurve & 2 points
    return STP_w_ORIENTED_EDGE (ip1, ip2, ici, 0);
    // if(ac1->rad > 0.) i1 = 0;
    // else              i1 = 1;  // CW
    // return STP_w_ORIENTED_EDGE (ip1, ip2, ici, i1);
  } 


  // mode=1|2
  ici = STP_w_TRIMMED_CURVE (ici, ip1, ip2, stpwTrue);
  // if(ac1->rad > 0) p1 = stpwTrue;
  // else             p1 = stpwFalse;
  // ici = STP_w_TRIMMED_CURVE (ici, ip1, ip2, p1);
  // if(ac1->rad > 0) ici = STP_w_TRIMMED_CURVE (ici, ip1, ip2, stpwTrue);
  // else             ici = STP_w_TRIMMED_CURVE (ici, ip2, ip1, stpwTrue);

  
  // GEOMETRIC_CURVE_SET < TRIMMED_CURVE
  if(mode == 1)
    return STP_w_CURVE_SET (&ici, 1); // CCW

  // mode=2:
  // COMPOSITE_CURVE_SEGMENT
  return STP_w_CCV_seg (ici, oid);

}


//================================================================
  int STP_w_EL_CC (stpPln *plb, CurvElli *el1, char *oid, int mode) { 
//================================================================
// mode=1 create GEOMETRIC_CURVE_SET from ellipse
//      2 create COMPOSITE_CURVE_SEGMENT from ellipse    (WF)
//      3 create ORIENTED_EDGE < EDGE_CURVE < elli


  int     i1, ip1, ip2, ici, i360;
  char    s1[128], *p1;
  Vector  vx;
  Point   pts, pte;



  // ck 360-deg
  i360 = UT3D_ck_el360 (el1);
    printf(" i360=%d\n",i360);
  if(!i360) p1 = oid;
  else p1 = stptStrNull;

  printf("STP_w_EL_CC %s %d\n",oid,mode);
  fprintf(stpw_fp,"/* Ellipse %s */\n",oid);


  // create Circ-unlimited
  ici = STP_w_EL__ (plb, el1, p1);

  // do not trim 360-deg-circ 
  if(!i360) goto L_CS;

  // creat startPt/endPt
  ip1 = STP_w_PT (&el1->p1, "");
  ip2 = STP_w_PT (&el1->p2, "");


  // mode=3: ORIENTED_EDGE < EDGE_CURVE
  if(mode == 3) {
    // EDGE_CURVE and ORIENTED_EDGE from baseCurve & 2 points
    if(el1->dir > 0) i1 = 0;
    else             i1 = 1;   // CW
    return STP_w_ORIENTED_EDGE (ip1, ip2, ici, i1);
  }


  // mode=1|2
  if(el1->dir > 0)  p1 = stpwTrue;
  else              p1 = stpwFalse;
  ici = STP_w_TRIMMED_CURVE (ici, ip1, ip2, p1);
  // if(el1->dir > 0) ici = STP_w_TRIMMED_CURVE (ici, ip1, ip2, stpwTrue);
  // else             ici = STP_w_TRIMMED_CURVE (ici, ip2, ip1, stpwTrue);


  // GEOMETRIC_CURVE_SET < TRIMMED_CURVE
  L_CS:
  if(mode == 1)
    ici = STP_w_CURVE_SET (&ici, 1); // CCW


  // mode=2:
  // COMPOSITE_CURVE_SEGMENT
  return STP_w_CCV_seg (ici, oid);

}


//================================================================
  int STP_w_EL__ (stpPln *plb, CurvElli *el1, char *oid) {
//================================================================
// create untrimmed ellipse
// #101=CARTESIAN_POINT('',(0.,0.,0.));       // center
// #54=CARTESIAN_POINT('',(60.,0.,0.));       // startpt
// #55=CARTESIAN_POINT('',(0.,20.,0.));       // endpt
// #53=ELLIPSE('',#100,60.,20.);              // plane,majAxis,minorAxis

// {Point p1, p2, pc; Vector vz, va, vb; int dir;}      CurvElli;

  int     ipc, ip1, ip2, ipvz, ipvx, ipx, iel, i360, sr;
  double  d1, d2;
  char    s1[128], *p1;
  stpPln  lPlb;


  printf("STP_w_EL__ %s\n",oid);

  // centerpoint, startpoint, endpoint
  ipc = STP_w_PT (&el1->pc, "");

  // must have axis
  if(!plb) plb = &lPlb;

  sr = UT3D_sr_el (el1);
  STP_w_stpPln_set (plb, &el1->pc, ipc, &el1->vz, -1, sr);

  // need x-axis
  plb->ivx = STP_w_VC_d (&el1->va, "");

  // create origin+axis as AXIS2_PLACEMENT_3D
  ipx = STP_w_axis3_vz (plb);

  // ELLIPSE
  iel = stpw_li;
  d1 = UT3D_len_vc (&el1->va);
  d2 = UT3D_len_vc (&el1->vb);
  sprintf(s1, "#%d=ELLIPSE('%s',#%d,%lf,%lf)", stpw_li, oid, ipx, d1, d2);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;

  return (stpw_li - 1);

}


//================================================================
  int STP_w_CVPOL (stpPln *plb, CurvPoly *cv1, char *oid, int mode) {
//================================================================
// mode=1 create GEOMETRIC_CURVE_SET
//      2 create COMPOSITE_CURVE_SEGMENT
//      3 create ORIENTED_EDGE < EDGE_CURVE
// Output:
//   vcn    normalvector
// see STP_w_CVBSP
// {int ptNr; double v0, v1, *lvTab; Point *cpTab;} CurvPoly;

  int     i1, i2, ipa, iNr, *ia, icv, ip1, ip2;
  Point   *pa, pt1;
  double  *da;
  char    s1[128], *p1;
  Vector  vcn;


  UT3D_stru_dump (Typ_CVPOL, cv1, "STP_w_CVPOL");
  fprintf(stpw_fp,"/* Polygon %s */\n",oid);

  // define points
  ipa = stpw_li;
  pa = cv1->cpTab;
  for(i1=0; i1 < cv1->ptNr; ++i1) STP_w_PT (&pa[i1], "");


  if(plb) {
    // get normal-vector, bp, sr.
    STP_w_sur_nvc (plb, cv1->ptNr, pa);
  }


  p1 = oid;

  // bspline (degree,
  icv = stpw_li;
  sprintf(s1, "#%d=B_SPLINE_CURVE_WITH_KNOTS('%s',%d", stpw_li, p1, 1);
  // add list of points
  STP_w_list_is (s1, ipa, cv1->ptNr, ",");
  // curve_form:  unspecified, polyline_form, parabolic_arc, circular_arc,
  //              hyperbolic_arc, elliptic_arc
  STP_w_txt (s1, ",.UNSPECIFIED.");
  // closed_curve, self_intersect
  STP_w_txt (s1, ",.U.,.U.");


  // loop tru kvTab; size is ptNr+deg+1
  // set multiplicity = nr of identical values.

  iNr = cv1->ptNr;
  ia = (int*)UME_alloc_tmp (iNr * sizeof(int));
  // da = (double*)UME_alloc_tmp (iNr * sizeof(double));

  for(i1=0; i1 < iNr; ++i1) {
    ia[i1] = 1;
    // da[i1] = cv1->lvTab[i1];
  }

  ia[0] = 2;
  ia[iNr - 1] = 2;
    // for(i1=0;i1<iNr;++i1) printf("   m=%d k=%lf\n",ia[i1],da[i1]);

  // output knot_multiplicities ia
  STP_w_list__ (s1, ia, iNr, 0, ",");

  // output knots da
  // STP_w_list__ (s1, da, iNr, 2);
  STP_w_list__ (s1, cv1->lvTab, iNr, 2, ",");

  // knot_type
  STP_w_txt (s1, ",.UNSPECIFIED.");

  fprintf(stpw_fp,"%s);\n",s1);
  ++stpw_li;


  // GEOMETRIC_CURVE_SET
  if(mode == 1)
    return STP_w_CURVE_SET (&icv, 1); // CCW


  // set limits
  if(UTP_comp2db(cv1->v0, cv1->lvTab[0], UT_TOL_min1)) {
    ip1 = ipa;
  } else {
    UTO_2pt_limstru (&pt1, NULL, NULL, NULL, Typ_CVBSP, cv1);
    ip1 = STP_w_PT (&pt1, "");
  }

  i1 = cv1->ptNr - 1;
  if(UTP_comp2db(cv1->v1, cv1->lvTab[i1], UT_TOL_min1)) {
    if(UT3D_comp2pt(&pa[0], &pa[i1], UT_TOL_pt) != 0) ip2 = ip1;
    else ip2 = ipa + i1;
  } else {
    UTO_2pt_limstru (NULL, &pt1, NULL, NULL, Typ_CVBSP, cv1);
    ip2 = STP_w_PT (&pt1, "");
  }


  // mode=2: TRIMMED_CURVE (v0,v1 ?)
  if(mode == 2) {
    icv = STP_w_TRIMMED_CURVE (icv, ip1, ip2, stpwTrue);
    // icv = STP_w_TRIMMED_CURVE_par (icv, cv1->v0, cv1->v1, stpwTrue);
    return STP_w_CCV_seg (icv, oid);
  }


  // mode=3: ORIENTED_EDGE < EDGE_CURVE < baseCurve
  // get trim-points
  // EDGE_CURVE and ORIENTED_EDGE from baseCurve & 2 points
  if(cv1->v1 > cv1->v0) i1 = 0;
  else                  i1 = 1;
  return STP_w_ORIENTED_EDGE (ip1, ip2, icv, i1);

}


/*
//================================================================
  int STP_w_CVPOL (CurvPoly *cv1, char *oid) {
//================================================================
// unused
// alternate method: as bspl with deg=1; length=knot-value !
// #34=CARTESIAN_POINT('',(0.,0.,0.));
// #35=CARTESIAN_POINT('',(10.,0.,0.));
// #36=CARTESIAN_POINT('',(10.,10.,0.));
// #40=DIRECTION('',(1.,0.,0.));
// #41=DIRECTION('',(0.,1.,0.));
// #42=VECTOR('',#38,1.);
// #43=VECTOR('',#38,1.);
// #50=LINE('',#34,#42);
// #51=LINE('',#35,#43);
// #60=TRIMMED_CURVE('',#50,(#34),(#35),.T.,.CARTESIAN.);
// #61=TRIMMED_CURVE('',#51,(#35),(#36),.T.,.CARTESIAN.);
// #70=COMPOSITE_CURVE_SEGMENT(.DISCONTINUOUS.,.F.,#60);
// #71=COMPOSITE_CURVE_SEGMENT(.DISCONTINUOUS.,.F.,#61);
// #80=COMPOSITE_CURVE('LN1',(#70,#71),.U.) ;

// {int ptNr; double v0, v1, *lvTab; Point *cpTab;} CurvPoly;

  
  int     ipp, ipd, ipv, ipl, ipt, ips, ipc, i1;
  Point   *pa;
  char    s1[128];
  Vector  vc1;



  UT3D_stru_dump (Typ_CVPOL, cv1, "STP_w_CVPOL");


  fprintf(stpw_fp,"/ Polygon %s /\n",oid);

  // define points
  ipp = stpw_li;
  pa = cv1->cpTab;
  for(i1=0; i1 < cv1->ptNr; ++i1) STP_w_PT (&pa[i1], "");


  // output directions
  ipd = stpw_li;
  for(i1=1; i1 < cv1->ptNr; ++i1) {
    UT3D_vc_2pt (&vc1, &pa[i1 - 1], &pa[i1]);
    UT3D_vc_setLength (&vc1, &vc1, 1.);
    STP_w_VC_d (&vc1, "");
  }

  // output vectors
  ipv = stpw_li;
  for(i1=1; i1 < cv1->ptNr; ++i1) {
    sprintf(s1, "#%d=VECTOR('',#%d,1.)",stpw_li,ipd+i1-1);
    fprintf(stpw_fp,"%s;\n",s1);
    ++stpw_li;
  }

  // output lines
  ipl = stpw_li;
  for(i1=1; i1 < cv1->ptNr; ++i1) {
    sprintf(s1, "#%d=LINE('',#%d,#%d)",stpw_li,ipp+i1-1,ipv+i1-1);
    fprintf(stpw_fp,"%s;\n",s1);
    ++stpw_li;
  }

  // output trimmed curve
  ipt = stpw_li;
  for(i1=1; i1 < cv1->ptNr; ++i1) {
    sprintf(s1, "#%d=TRIMMED_CURVE('',#%d,(#%d),(#%d),.T.,.CARTESIAN.)",
            stpw_li,ipl+i1-1, ipp+i1-1, ipp+i1);
    fprintf(stpw_fp,"%s;\n",s1);
    ++stpw_li;
  }

  // output composite curve segment
  ips = stpw_li;
  for(i1=1; i1 < cv1->ptNr; ++i1) {
    sprintf(s1, "#%d=COMPOSITE_CURVE_SEGMENT(.DISCONTINUOUS.,.F.,#%d)",
            stpw_li, ipt+i1-1);
    fprintf(stpw_fp,"%s;\n",s1);
    ++stpw_li;
  }


  // compositeCurve from list of compositeCurveSegments
  ipc = stpw_li;
  sprintf(s1, "#%d=COMPOSITE_CURVE('%s'", stpw_li, oid);
  STP_w_list_is (s1, ips, cv1->ptNr - 1, ",");
  fprintf(stpw_fp,"%s,.U.);\n",s1);
  ++stpw_li;


  // GEOMETRIC_CURVE_SET from CCV
  STP_w_CURVE_SET (&ipc, 1);

  return (stpw_li - 1);

}
*/


//================================================================
  int STP_w_CVCCV (stpPln *plb, int *ia,
                   ObjGX *cv, char *oid, int mode) {
//================================================================
// mode=2
//   COMPOSITE_CURVE < COMPOSITE_CURVE_SEGMENT
// mode=3
//   EDGE_CURVE; retCode = nr of links in ia


  int     irc, i1, ii, iNr, pNr;
  long    l1;
  void    *odc;
  Point   *pt1, *pa, actPos;
  Line    ln1;
  ObjGX   *oxa, *ox1;

  printf("STP_w_CVCCV %s %d\n",oid,mode);
  UT3D_stru_dump (Typ_ObjGX, cv, "CCV");

  fprintf(stpw_fp,"/* CompositeCurve %s */\n",oid);

  oxa = ((ObjGX*)cv)->data;

  if(plb) {
    // get polygon from CCV
    pa = (void*)memspc201;
    pNr = sizeof(memspc201) / sizeof(Point);
    irc = UT3D_pta_ccv (&pNr, pa, cv, 0, NULL, UT_DISP_cv);
      printf(" ccv-pNr=%d\n",pNr);
    if(irc < 0) {TX_Error("STP_w_CVCCV EOM"); return -1;}
    // set normal-vector, sense-of-rotation ..
    STP_w_sur_nvc (plb, pNr, pa);
  }


  iNr = 0;
  for(i1=0; i1<cv->siz; ++i1) {
    ox1 = &oxa[i1];
      UTO_dump__ (ox1, " CCV[%d]",i1);

    // skip data, form=Typ_CVCCV
    if(ox1->form == Typ_CVCCV) {
      continue;

    //----------------------------------------------------------------
    } else if(ox1->typ == Typ_PT) {
      if(i1 == 0) {
        UTO_obj_getp ((void**)&pt1, &ii, ox1);
        actPos = *pt1;
        continue;
      }
      ln1.p1 = actPos;
      UTO_obj_getp ((void**)&pt1, &ii, ox1);
      ln1.p2 = *pt1;
      actPos = *pt1;
      // skip 
      ii = STP_w_LN (&ln1, "", mode);

    //----------------------------------------------------------------
    // Lines: LINE > TRIMMED_CURVE > COMPOSITE_CURVE_SEGMENT
    } else if(ox1->typ == Typ_LN) {
      UTO_obj_getp (&odc, &ii, ox1);
      ii = STP_w_LN (odc, "", mode);
      actPos = ((Line*)odc)->p2;

    //----------------------------------------------------------------
    // CIRCLE > TRIMMED_CURVE > COMPOSITE_CURVE_SEGMENT
    } else if(ox1->typ == Typ_CI) {
      UTO_obj_getp (&odc, &ii, ox1);
        // UT3D_stru_dump (Typ_CI, odc, " CCV[%d]",i1);
      ii = STP_w_AC_CC (NULL, odc, "", mode);
      actPos = ((Circ*)odc)->p2;

    //----------------------------------------------------------------
    // B_SPLINE_CURVE > TRIMMED_CURVE > COMPOSITE_CURVE_SEGMENT
    } else if(ox1->typ == Typ_CVBSP) {
      UTO_obj_getp (&odc, &ii, ox1);
        UT3D_stru_dump (Typ_CVBSP, odc, " CCV[%d]",i1);
      ii = STP_w_CVBSP (NULL, &actPos, odc, "", mode);

    //----------------------------------------------------------------
    } else if(ox1->typ == Typ_CVELL) {
      UTO_obj_getp (&odc, &ii, ox1);
        UT3D_stru_dump (Typ_CVELL, odc, " CCV[%d]",i1);
      ii = STP_w_EL_CC (NULL, odc, "", mode);
      actPos = ((CurvElli*)odc)->p2;

    //----------------------------------------------------------------
    } else {
      TX_Error ("STP_w_CVCCV %d",ox1->typ);
          return -1;
    }

    // minor error; continue
    if(ii == 0) continue;
    // unrecoverablle error
    if(ii < 0) return ii;

    // add to list
    ia[iNr] = ii;
    ++iNr;
  }

  if(ii < 1) return -1;


  if(mode == 2) {
    // create COMPOSITE_CURVE from list ia
    return STP_w_CCV__ (ia, iNr, oid);

    // // create GEOMETRIC_CURVE_SET
    // return STP_w_CURVE_SET (&ii, 1);
  }


  // mode=3: ORIENTED_EDGEs
  if(mode != 3) TX_Error ("STP_w_CVCCV E001 mode %d",mode);

  return iNr;

}

 
//================================================================
  int STP_w_CVBSP (stpPln *plb, Point *actPos, 
                   CurvBSpl *cv1, char *oid, int mode) {
//================================================================
// create B_SPLINE_CURVE_WITH_KNOTS
// mode=1 create GEOMETRIC_CURVE_SET
//      2 create COMPOSITE_CURVE_SEGMENT
//      3 create ORIENTED_EDGE < EDGE_CURVE
//   plb    set normalvector if !NULL

// B_SPLINE_CURVE_WITH_KNOTS (name, degree,
//   (control_points),
//   curve_form, closed_curve, self_intersect
//   (knot_multiplicities), (knots), knot_type);
// curve_form:  unspecified, polyline_form, parabolic_arc, circular_arc,
//              hyperbolic_arc, elliptic_arc
//    .POLYLINE_FORM.,.F.,.U.                 // for deg. 1
// knot_multiplicities; total size is ptNr+deg+1

// mode==1
// #32=CARTESIAN_POINT($,(-28.7838380753703,-2.82510993966685,0.));
// #33=CARTESIAN_POINT($,(9.46719821079103,-39.9928938095413,0.));
// #31=B_SPLINE_CURVE_WITH_KNOTS($,1,(#32,#33),.UNSPECIFIED.,.F.,.F.,(2,2),
// (0.,1.),.UNSPECIFIED.);
// #40=TRIMMED_CURVE('',#31,(0.000000),(1.000000),.T.,.UNSPECIFIED.);
// #41=GEOMETRIC_CURVE_SET('',(#40));

// mode==2
// #31=B_SPLINE_CURVE..
// #40=TRIMMED_CURVE('',#31,(0.000000),(1.000000),.T.,.UNSPECIFIED.);
// #41=COMPOSITE_CURVE_SEGMENT(.DISCONTINUOUS.,.F.,#40);



// {int ptNr, deg; double v0, v1, *kvTab; Point *cpTab;} CurvBSpl;

  int     i1, i2, kNr, im, ipa, iNr, *ia, icv, iDir;
  int     ip1, ip2;  // trim-points
  Point   *pa, pt1, pt2;
  double  *da, v0, v1;
  char    s1[128], *p1;
  Vector  vcn;


  UT3D_stru_dump (Typ_CVBSP, cv1, "STP_w_CVBSP %d",mode);


  fprintf(stpw_fp,"/* Bspline-Curve %s */\n",oid);

  // define points
  ipa = stpw_li;
  pa = cv1->cpTab;
  for(i1=0; i1 < cv1->ptNr; ++i1) STP_w_PT (&pa[i1], "");


  // set normalvector
  if(plb) {
    // get normal-vector, bp, sr.
    STP_w_sur_nvc (plb, cv1->ptNr, pa);
  }



  p1 = oid;

  // bspline (degree,
  icv = stpw_li;
  sprintf(s1, "#%d=B_SPLINE_CURVE_WITH_KNOTS('%s',%d", stpw_li, p1, cv1->deg);
  // add list of points
  STP_w_list_is (s1, ipa, cv1->ptNr, ",");
  // curve_form:  unspecified, polyline_form, parabolic_arc, circular_arc,
  //              hyperbolic_arc, elliptic_arc
  STP_w_txt (s1, ",.UNSPECIFIED.");
  // closed_curve, self_intersect
  STP_w_txt (s1, ",.U.,.U.");


  // loop tru kvTab; size is ptNr+deg+1
  // set multiplicity = nr of identical values.
  iNr = cv1->ptNr + cv1->deg + 1;
    printf(" iNr=%d\n",iNr);
  ia = (int*)UME_alloc_tmp (iNr * sizeof(int));
  da = (double*)UME_alloc_tmp (iNr * sizeof(double));

  kNr = 0;
  im = 0;
  i1 = 0;
  for(i2=1; i2<iNr; ++i2) {
    i1 = i2 - 1;
    ++im;
      printf(" ck i1=%d i2=%d im=%d\n",i1,i2,im);
    if(cv1->kvTab[i1] == cv1->kvTab[i2]) continue;
    // add [i1-1] to ia,da.
    ia[kNr] = im;
    da[kNr] = cv1->kvTab[i1];
    ++kNr;
    im = 0;
  }
  // ouput last
  ++im;
  ia[kNr] = im;
  da[kNr] = cv1->kvTab[i1];
  ++kNr;
    for(i1=0;i1<kNr;++i1) printf("   m=%d k=%lf\n",ia[i1],da[i1]);

  // output knot_multiplicities ia
  STP_w_list__ (s1, ia, kNr, 0, ",");

  // output knots da
  STP_w_list__ (s1, da, kNr, 2, ",");

  // knot_type
  STP_w_txt (s1, ",.UNSPECIFIED.");

  fprintf(stpw_fp,"%s);\n",s1);
  ++stpw_li;


  UTO_2pt_limstru (&pt1, &pt2, NULL, NULL, Typ_CVBSP, cv1);
  *actPos = pt2;

  if(cv1->v1 > cv1->v0) {
    iDir = 0;
    v0 = cv1->v0;
    v1 = cv1->v1;
  } else {
    iDir = 1;
    v0 = cv1->v1;
    v1 = cv1->v0;
    MEM_swap__ (&pt1, &pt2, sizeof(Point));
  }

  // set limits
  // startpoint
  if(UTP_comp2db(v0, cv1->kvTab[0], UT_TOL_min1)) {
    ip1 = ipa;
  } else {
    ip1 = STP_w_PT (&pt1, "");
  }

  // endpoint
  i1 = iNr - 1;
  // check if closed
  if(UT3D_comp2pt(&pt1, &pt2, UT_TOL_pt) != 0) {
    ip2 = ip1;

  } else {
    if(UTP_comp2db(v1, cv1->kvTab[i1], UT_TOL_min1)) {
      ip2 = ipa + cv1->ptNr - 1;
    } else {
      ip2 = STP_w_PT (&pt2, "");
    }
  }


  // mode=3: ORIENTED_EDGE < EDGE_CURVE < baseCurve
  // get trim-points
  if(mode == 3) {
    // EDGE_CURVE and ORIENTED_EDGE from baseCurve & 2 points
    return STP_w_ORIENTED_EDGE (ip1, ip2, icv, iDir);
  }


  // mode=1|2: TRIMMED_CURVE (v0,v1 ?)
    // icv = STP_w_TRIMMED_CURVE_par (icv, cv1->v0, cv1->v1, stpwTrue);
    if(cv1->v1 > cv1->v0) p1 = stpwTrue;
    else                  p1 = stpwFalse;
    icv = STP_w_TRIMMED_CURVE (icv, ip1, ip2, p1);


  // GEOMETRIC_CURVE_SET < TRIMMED_CURVE
  if(mode == 1)
    return STP_w_CURVE_SET (&icv, 1);
    // return STP_w_CCV_seg (icv, oid);


  // mode=2:
  // COMPOSITE_CURVE_SEGMENT
  return STP_w_CCV_seg (icv, oid);

}


//================================================================
  int STP_w_TRIMMED_CURVE_par (int icv, double v0, double v1) {
//================================================================
// icv  baseCurve
// ips  startPt
// ipe  endPt
// #42=TRIMMED_CURVE('',#43,(#44),(#45),.T.,.CARTESIAN.);


  char    s1[128];


  // TRIMMED_CURVE (baseCurve,startPt,endPt)
  sprintf(s1, "#%d=TRIMMED_CURVE('',#%d,(%lf),(%lf),.T.,.UNSPECIFIED.)",
              stpw_li, icv, v0, v1);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;

  return (stpw_li - 1);

}


//===================================================================
  int STP_w_TRIMMED_CURVE (int icv, int ips, int ipe, char *orient) {
//===================================================================
// Input:
//   icv       baseCurve
//   ips       startPt
//   ipe       endPt
//   orient    .T. same direction as baseCurve
//             .F. reverse direction

// #42=TRIMMED_CURVE('',#43,(#44),(#45),.T.,.CARTESIAN.);
// baseCurve: LINE|CIRCLE|ELLIPSE|B_SPLINE_CURVE_WITH_KNOTS
// points: trimpoints|values; (CARTESIAN_POINT), (CARTESIAN_POINT)
// orient: .T.|.F.; .T. = same direction as baseCurve;
//   if the 2.point is before first Trimpoint, orient. is .F.
// flag2: .UNSPECIFIED.|.CARTESIAN.|.PARAMETER.



  char    s1[128];


  // TRIMMED_CURVE (baseCurve,startPt,endPt)
  sprintf(s1, "#%d=TRIMMED_CURVE('',#%d,(#%d),(#%d),%s,.CARTESIAN.)",
              stpw_li, icv, ips, ipe, orient);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;

  return (stpw_li - 1);

}


//==================================================================
  int STP_w_CURVE_SET (int *ia, int iNr) {
//==================================================================
// create GEOMETRIC_CURVE_SET from a list of basecurves
// #232=GEOMETRIC_CURVE_SET('',(#231));

// jede line eigener curveset; points aber auch mehrere .


  int     i1;
  char    s1[128];
  

  sprintf(s1, "#%d=GEOMETRIC_CURVE_SET(''", stpw_li); //, stpw_li - 1);

  STP_w_list__ (s1, ia, iNr, 1, ",");
  fprintf(stpw_fp,"%s);\n",s1);
  ++stpw_li;

  return (stpw_li - 1);
  
} 


//==================================================================
  int STP_w_CCV_seg (int icv, char *oid) {
//==================================================================
// create COMPOSITE_CURVE_SEGMENT from a TRIMMED_CURVE
//
// COMPOSITE_CURVE_SEGMENT(.CONTINUOUS., .T., <TRIMMED_CURVE>) ;
//   // transition      .CONTINUOUS.|.DISCONTINUOUS.   (closed or not)
//   //     .DISCONTINUOUS. = open; .CONTINUOUS. = closed
//   // same_sense      .T.|.F.    Always True;
//   //           direction already set in TRIMMED_CURVE


  
  int     i1;
  char    s1[128];
  

  // compositeCurveSegment = trimmedCurve
  // .CONTINUOUS.,.T.         closed CCV
  // .DISCONTINUOUS.,.F.      open CCV
  sprintf(s1, "#%d=COMPOSITE_CURVE_SEGMENT(.DISCONTINUOUS.,.T.,#%d)",
              stpw_li, icv);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;

  return (stpw_li - 1);

}


//==================================================================
  int STP_w_CCV__ (int *ias, int iNr, char *oid) {
//==================================================================
// create COMPOSITE_CURVE from list of COMPOSITE_CURVE_SEGMENTs
  
  int     i1;
  char    s1[128];

  // compositeCurve from list of compositeCurveSegments
  sprintf(s1, "#%d=COMPOSITE_CURVE('%s'", stpw_li, oid);
  STP_w_list__ (s1, ias, iNr, 1, ",");
  fprintf(stpw_fp,"%s,.U.);\n",s1); 
  ++stpw_li;
  
  return (stpw_li - 1);

}


//==================================================================
  int STP_w_CCV (int *icv, int *ips, int *ipe, int iNr, char *oid) {
//==================================================================
// create COMPOSITE_CURVE from a list of basecurves,startpoints,enpoints.
 
  int     i1;
  char    s1[128];


  // loop tru list
  for(i1=0; i1<iNr; ++i1) {

    // TRIMMED_CURVE(baseCurve,startPt,endPt)
    STP_w_TRIMMED_CURVE (icv[i1], ips[i1], ipe[i1], stpwTrue);

    // compositeCurveSegment = trimmedCurve
    // .CONTINUOUS.,.T.         closed CCV
    // .DISCONTINUOUS.,.F.      open CCV
    sprintf(s1, "#%d=COMPOSITE_CURVE_SEGMENT(.DISCONTINUOUS.,.F.,#%d)",
                stpw_li, stpw_li - 1);
    fprintf(stpw_fp,"%s;\n",s1);
    ++stpw_li;
  }
  
// TODO: build list of compositeCurveSegments. UME_alloc_tmp; 
// see STP_w_wf_init-GEOMETRIC_SET

  // compositeCurve from list of compositeCurveSegments
  sprintf(s1, "#%d=COMPOSITE_CURVE('%s',(#%d),.U.)",
              stpw_li, oid, stpw_li - 1);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;
  
  return (stpw_li - 1);
  
} 


//================================================================
  int STP_w_txt (char *s1, char *txt) {
//================================================================
// add ',' and text to s1. If s1 gets to long: write -> stpw_fp

  int    l1, l2;

 
  l1 = strlen(s1);
  l2 = strlen(txt);

  if (l1 > 80) {
    fprintf (stpw_fp,"%s\n",s1);
    sprintf (s1, "  %s",txt);
    goto L_exit;
  }


  // sprintf (&s1[l1], ",%s",txt);
  sprintf (&s1[l1], "%s",txt);

  L_exit:
  return 0;

}


//================================================================
  int STP_w_list_is (char *s1, int is, int iNr, char *sPre) {
//================================================================
// write list of links; starting with is.
// eg (#200,#201,#202,#203,#204)
// s1 = outputline with len=128
// sPre   add this to s1 before adding list
  

  int     i1, *ia;


  ia = (int*)UME_alloc_tmp (iNr * sizeof(int));
  for(i1=0; i1<iNr; ++i1) ia[i1] = is + i1;

  return STP_w_list__ (s1, ia, iNr, 1, sPre);

}


//========================================================================
  int STP_w_list__ (char *s1, void *data, int iNr, int mode, char *sPre) {
//========================================================================
// s1    outputline with len=128
// mode  0=int (eg (1,2,66,1234))
//       1=links from iTab eg  (#200,#101,#202)
//       2=doubles;        eg  (-1.74860160797,-5.48849526774,0.)
// sPre  add this to s1 before adding list
// does not write line if < 80 chars.


  int     i1, ie, ll, *ia;
  char    *p1;
  double  *da;


  printf("STP_w_list__ iNr=%d mode=%d\n",iNr,mode);


  if(mode < 2) ia = data;
  else         da = data;

  strcat (s1, sPre);
  strcat (s1, "(");

  i1 = 0;
  ie = iNr;
  ll = strlen(s1);
  p1 = &s1[ll];
  goto L_nxt1;

  L_nxt0:
    if(mode == 0) sprintf(p1, "%d,", ia[i1]);
    else if(mode == 1) sprintf(p1, "#%d,", ia[i1]);
    else if(mode == 2) sprintf(p1, "%lf,", da[i1]);
    ll = strlen(s1);
    p1 = &s1[ll];
    ++i1;

    L_nxt1:
    if(ll > 70) {
      if(i1 == ie) s1[ll - 1] = '\0';  // remove ',' for last char
      fprintf(stpw_fp,"%s\n",s1);
      strcpy(s1, "  ");
      ll = 2;
      p1 = &s1[ll];
    }

    if(i1 < ie) goto L_nxt0;


  //----------------------------------------------------------------
  if(ll > 3) { --ll; s1[ll] = '\0'; }    // remove ',' for last char
  s1[ll] = ')';
  ++ll;
  s1[ll] = '\0';
  // fprintf(stpw_fp,"%s)",s1);

  return 0;

}


//================================================================
  int STP_w_end () {
//================================================================

  fprintf(stpw_fp,"ENDSEC;\n");
  fprintf(stpw_fp,"END-ISO-10303-21;\n");

  return 0;

}


//================================================================
  int STP_w_stpPln_set (stpPln *plb, Point *po, int ipo, Vector *vz, int ivz,
                        int sro) {
//================================================================
// set outer-boundary
// Input:
//   ipo     -1 point po not yet created
//   ivz     -1 vector vz not yet created
//   ivx     -1 vector vx not yet created
//   sr      0=CCW; 1=CW; 2=undefined
 
  plb->po    = *po;
  plb->ipo   = ipo;
  plb->vz    = *vz;
  plb->ivz   = ivz;
  plb->ivx   = -1;
  plb->sro   = sro;

  return 0;

}


//================================================================
  int STP_w_sur_nvc (stpPln *plb, int ptNr, Point *pa) {
//================================================================
// outerBoundary: get normal-vector, sense-of-rotation ..

  int    ia[6];


  printf("STP_w_sur_nvc ptNr=%d stat=%d\n",ptNr,plb->stat);

  if(plb->stat) goto L_ib;

  //----------------------------------------------------------------
  // outerBoundary: get normal-vector, sense-of-rotation ..

  // get 3D-box from npt
  UT3D_box_ix_npt (&plb->pb1, &plb->pb2, ia, ptNr-1, pa);
    GR_Disp_box  (&plb->pb1, &plb->pb2, 2);

  // get backPlane from 3D-box
  plb->bp = UT3D_bp_2pt (&plb->pb1, &plb->pb2);
    printf(" bp=%d\n",plb->bp);

  // get normalvector plb->vz from backPlane and extreme-points
  UT3D_vc_perp_npt_bp_ix (&plb->vz, pa, ia, plb->bp);

  // get sr from npt&bp
  plb->sro = UT3D_sr_npt_bp (ptNr-1, pa, plb->bp);

  return 0;



  //----------------------------------------------------------------
  L_ib:
  // innerBoundary: get sense-of-rotation from bp
  // get normalvector and sr from npt&bp
  plb->sri = UT3D_sr_npt_bp (ptNr-1, pa, plb->bp);

  return 0;

}


//================================================================
  int STP_w_plb_ini (stpPln *plb) {
//================================================================
 
  plb->ipo   = 0;
  plb->ivz   = 0;

  return 0;

}


// ================ EOF =========================
