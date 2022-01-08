//     gCAD3D STEP-Files export                 RF   2005-08-28
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
STP_w_SURRU       ruled-surf   create baoundary ...
-

-----------------------------------------------------
Modifications:
2020-12-10   Rewritten. RF.
2005-08-28   First version. RF.

-----------------------------------------------------
*/
/*!
\file  ../exp/stp_w.c
\brief write STeP file 
\code
=====================================================
List_functions_start:

STP_w__             main
STP_w_init__
STP_w_os_init
STP_w_wf_init
STP_w_cs__          output of yet unused surfaces as CLOSED_SHELL
STP_w_os__          output of yet unused surfaces as OPEN_SHELL
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
STP_w_ORIENTED_EDGE
STP_w_EDGE_LOOP_out
STP_w_wf_wo
STP_w_PT
STP_w_VC_d
STP_w_VCdv
STP_w_axis3_vz     axis from Z-vec only
STP_w_axis3_dbo    
STP_w_axis3__      only AXIS2_PLACEMENT_3D + 3 step-indices
STP_w_axis__       pt + vz + vz
STP_w_LN
STP_w_CIRCLE
STP_w_AC_CC
STP_w_EL_CC
STP_w_EL__
STP_w_CVPOL
STP_w_CVCCV
STP_w_CVBSP
STP_w_CVTRM__
STP_w_TRIMMED_CURVE_par
STP_w_TRIMMED_CURVE
STP_w_CURVE_SET
STP_w_CCV_SEG
STP_w_COMPOSITE_CURVE
STP_w_CCV
STP_w_txt
STP_w_list_is
STP_w_list__
STP_w_objAct_set
STP_w_sur_nvc
STP_w_objAct_ini
STP_w_end

STP_w_log_inf              log info
STP_w_log_err              log error

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


Build:
. ../options.sh && make -f xa_stp_w.mak




==============================
Doku STEP =  STP
firefox /mnt/serv1/Devel/cadfiles/step/part203.exp.html
evince /mnt/serv1/Devel/cadfiles/step/Step_D_90_04.pdf
../exp/stp_w.c
../exp/stp_r.c

AP203 = allg. Masch.bau
AP214 = fuer KFZ



OPEN:
ADVANCED_FACE  last param T|F   was ist das ?

==========================================================
STP_w__
  STP_w_init__               // write initBlock
  STP_w_mdl__                // process mainModel or subModel
    STP_w_mdlTab_upd           // find all used subModels and add as new into mdlTab
    STP_w_mdl_start            // write model-startBlock
      STP_w_ox__                   // export a single obj
        STP_w_SUR__                  // export surface
        STP_w_MdlRef                 // export subModel-reference
    STP_w_mdl_close            // write model-closeBlock


abbreviviations in following description:
L:  <code>(P3): the link left of '=' is referenced in parameter 3 of <code>
P3: <code>: the parameter # 3 is a link to <code>


==========================================================
FILE_INITBLOCK:      STP_w_init__()
==========================================================
ISO-10303-21;
HEADER;
FILE_DESCRIPTION((''),'2;1');
FILE_NAME(
  't1.stp',
  '2020-12-15T17:21:17+02:00','','','',
  'preprocessor-none','',
  'gCAD3D-STP_w-V0.2','');
FILE_SCHEMA (('CONFIG_CONTROL_DESIGN'));
ENDSEC;
DATA;

#1=(LENGTH_UNIT()NAMED_UNIT(*)SI_UNIT(.MILLI.,.METRE.));
#2=(NAMED_UNIT(*)PLANE_ANGLE_UNIT()SI_UNIT($,.RADIAN.));
#3=(NAMED_UNIT(*)SI_UNIT($,.STERADIAN.)SOLID_ANGLE_UNIT());
#4=UNCERTAINTY_MEASURE_WITH_UNIT(LENGTH_MEASURE(0.005),#1,'TOL_CRV','');
#5=DIMENSIONAL_EXPONENTS(0.,0.,0.,0.,0.,0.,0.);
#6=PLANE_ANGLE_MEASURE_WITH_UNIT(PLANE_ANGLE_MEASURE(0.0174532925199),#2);
#7=(CONVERSION_BASED_UNIT('DEGREES',#6)NAMED_UNIT(#5)PLANE_ANGLE_UNIT());
#8=APPLICATION_CONTEXT('design');
#9=PRODUCT_CONTEXT('',#8,'mechanical');
#10=PRODUCT_DEFINITION_CONTEXT('',#8,'');
/ * refSys default * /
#11=AXIS2_PLACEMENT_3D('',#12,#13,#14);
#12=CARTESIAN_POINT('',(0.,0.,0.));
#13=DIRECTION('',(0.,0.,1.));
#14=DIRECTION('',(1.,0.,0.));
/ * tolerances default * /
#15=(GEOMETRIC_REPRESENTATION_CONTEXT(3)
  GLOBAL_UNCERTAINTY_ASSIGNED_CONTEXT((#4))
  GLOBAL_UNIT_ASSIGNED_CONTEXT((#3,#7,#1))
REPRESENTATION_CONTEXT('ID1','3D'));




==========================================================
model_startBlock:        STP_w_mdl_start()
==========================================================
// used for mainmodel and all submodels:

#32=PRODUCT('*MASTER',' ',' ',(#25));
  // P4: PRODUCT_CONTEXT (global)  oder MECHANICAL_CONTEXT

#35=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE(' ',' ',#32,.NOT_KNOWN.);
  // L:  PRODUCT_DEFINITION (P3, following)
  // P3: PRODUCT (before)


#41=PRODUCT_DEFINITION(' ',' ',#35,#24);
  // L:  PRODUCT_DEFINITION_SHAPE (following)
  // P3: PDFWSS (above)
  // P4: PRODUCT_DEFINITION_CONTEXT (global) or DESIGN_CONTEXT

#44=PRODUCT_DEFINITION_SHAPE(' ',' ',#41);
  // L:  SHAPE_DEFINITION_REPRESENTATION(P1)
  // P3: PRODUCT_DEFINITION (above)


==========================================================
geometric_objects:
==========================================================

//----------------------------------------------------------------
POINT:
#25=CARTESIAN_POINT('',(-155.465126,-81.131368,0.000000));
// L:  LINE|B_SPLINE_CURVE_WITH_KNOTS|..|TRIMMED_CURVE

#297=VERTEX_POINT('NONE',#296);
// L:  EDGE_CURVE (face-bound)
// P2: CARTESIAN_POINT

//----------------------------------------------------------------
LINE:
#209=CARTESIAN_POINT('',(-101.199995,-23.999999,0.000000));
#210=CARTESIAN_POINT('',(-58.599997,4.000000,0.000000));
#211=DIRECTION('',(42.599998,27.999999,0.000000));
#212=VECTOR('',#211,1.);
#213=LINE('',#209,#212);

//----------------------------------------------------------------
CIRCLE:
#217=CARTESIAN_POINT('',(-105.599995,-26.599999,0.000000));
#218=DIRECTION('',(0.000000,0.000000,1.000000));
#219=DIRECTION('',(1.000000,0.000000,0.000000));
#220=AXIS2_PLACEMENT_3D('',#217,#218,#219);
#221=CIRCLE('',#220,12.000000);




==========================================================
wireframe_objects:
==========================================================
display curve: use TRIMMED_CURVE-COMPOSITE_CURVE_SEGMENT-COMPOSITE_CURVE
  add all COMPOSITE_CURVEs into GEOMETRIC_CURVE_SET


#214=TRIMMED_CURVE('',#213,(#209),(#210),.T.,.CARTESIAN.);
// L:  COMPOSITE_CURVE_SEGMENT
// P2: baseCurve LINE|CIRCLE|ELLIPSE|B_SPLINE_CURVE_WITH_KNOTS
// P3: trimPoint1 pointID for .CARTESIAN.; 
// P4: trimPoint2
// P5: sense     .T: = fwd; .F. = bwd;
// P6: trimType; .CARTESIAN.

#215=COMPOSITE_CURVE_SEGMENT(.DISCONTINUOUS.,.T.,#214);
// L:  COMPOSITE_CURVE
// P1: .CONTINUOUS. = closed-CCV; .DISCONTINUOUS. = open-CCV
// P2: sense     .T: = fwd; .F. = bwd;
// P3: TRIMMED_CURVE

#216=COMPOSITE_CURVE('L20',(#215),.U.);
// L:  GEOMETRIC_CURVE_SET
// P2: list of COMPOSITE_CURVE_SEGMENTs
// P3: self_intersect; .U. undefined


==========================================================
openShell_objects:
==========================================================
display support-surface: use RECTANGULAR_TRIMMED_SURFACE
  add al  RECTANGULAR_TRIMMED_SURFACEs into GEOMETRIC_CURVE_SET

display surface: ADVANCED_FACE-OPEN_SHELL-SHELL_BASED_SURFACE_MODEL
  add all SHELL_BASED_SURFACE_MODELs into MANIFOLD_SURFACE_SHAPE_REPRESENTATION


// ADVANCED_FACE:        sur2tria.stp

#65=EDGE_CURVE('',#62,#64,#60,.T.);  // analytic curve of boundary
// P2: VERTEX_POINT curve-trimPoint
// P3: VERTEX_POINT curve-trimPoint
// P4: curve; LINE|CIRCLE|ELLIPSE|B_SPLINE_CURVE_WITH_KNOTS
// P5: orientation; always .T.

#79=ORIENTED_EDGE('',*,*,#65,.T.);   // one boundary-segment (a curve)
// P4: edge_element EDGE_CURVE
// P5: orientation; .T. = sense identical EDGE_CURVE; .F. = reverse to EDGE_CURVE

#78=EDGE_LOOP('',(#79,#80,#81));    
// P2: list of ORIENTED_EDGEs (list of boundary-segments)

// VERTEX_LOOP only one point; 
// POLY_LOOP   n points

#82=FACE_OUTER_BOUND('',#78,.T.);  // definition outer boundary
// L:  ADVANCED_FACE
// P2: EDGE_LOOP
// P3: orientation; .T. or .F.

// #95=FACE_BOUND('NONE',#94,.F.);     // definition inner boundaries
// P3: orientation; .T. or .F.

#83=ADVANCED_FACE('Trennen.1',(#82),#56,.T.); // trimmed, perforated surface
// L:  OPEN_SHELL
// P2: FACE_OUTER_BOUND
// P3: supporting-surface
// P4: .T. orientation


//----------------------------------------------------------------
// ADVANCED_FACE - supporting surfaces:
// - PLANE

#92=PLANE('',#91) ;
#91=AXIS2_PLACEMENT_3D('Plane Axis2P3D',#88,#89,#90) ;
// P2: CARTESIAN_POINT origin
// P3: DIRECTION Z-vector
// P4: DIRECTION X-vector



//----------------------------------------------------------------
// SUPPORT-SURFACES:
//----------------------------------------------------------------

//----------------------------------------------------------------
// CONICAL_SURFACE

#45=CONICAL_SURFACE('A20',#44,0.000000,23.918522);
// P2: refSys (AXIS2_PLACEMENT_3D); origin = cone-tip; Z-vec=cone-axis
// P3: radius of the small-circle.
// P4: opening-angle degrees. May not be negative.
//   The origin of the refsys is the small circle of the cone; P3 ist the height.
//   The bottom-circle must be bigger than the circle at the origin (P4 postive).

#46=RECTANGULAR_TRIMMED_SURFACE('',#45,0.0,360.0,0.0,109.394476,.T.,.T.);
// P2: CONICAL_SURFACE
// P3: u1 angle in refSys 1 (startAngle around axis)
// P4: u2 angle in refSys 2 (endAngle around axis)
// P5: v1 dist of center of small-circle from origin
// P6: v2 dist of center of big-circle from origin (must be > than P5)
// P7: direction in u (.T.)
// P8: direction in v (.T.)

//----------------------------------------------------------------
// CYLINDRICAL_SURFACE
#37=CYLINDRICAL_SURFACE('A20',#36,25.000000);
// P2: refSys (AXIS2_PLACEMENT_3D); origin = center; Z-vec=cyl-axis
// P3: radius

//----------------------------------------------------------------
// SPHERICAL_SURFACE
#29=SPHERICAL_SURFACE('A21',#28,43.011626);
// P2: refSys (AXIS2_PLACEMENT_3D); origin = center; Z-vec=axis
// P3: radius

//----------------------------------------------------------------
// TOROIDAL_SURFACE
  major_radius
  minor_radius

//----------------------------------------------------------------
// B_SPLINE_SURFACE_WITH_KNOTS 


//----------------------------------------------------------------
// SURFACE_OF_LINEAR_EXTRUSION ('',<curve>,<VECTOR>)
// P2: LINE|CIRCLE|TRIMMED_CURVE|B_SPLINE_CURVE_WITH_KNOTS
// P3: VECTOR

C5_sur_ru1.stp:

#115=CARTESIAN_POINT('Vertex',(110.,-50.,0.)) ;
#116=VERTEX_POINT('',#115) ;
..
#119=EDGE_CURVE('',#116,#118,#114,.T.) ;
#138=EDGE_CURVE('',#116,#137,#120,.T.) ;
#145=EDGE_CURVE('',#137,#144,#142,.T.) ;
#162=EDGE_CURVE('',#118,#144,#146,.T.) ;
#163=EDGE_LOOP('',(#164,#165,#166,#167)) ;

#164=ORIENTED_EDGE('',*,*,#119,.F.) ;
#165=ORIENTED_EDGE('',*,*,#138,.T.) ;
#166=ORIENTED_EDGE('',*,*,#145,.T.) ;
#167=ORIENTED_EDGE('',*,*,#162,.F.) ;

#163=EDGE_LOOP('',(#164,#165,#166,#167)) ;
#168=FACE_OUTER_BOUND('',#163,.T.) ;
#110=SURFACE_OF_LINEAR_EXTRUSION('generated tabulated cylinder',#92,#109) ;
#169=ADVANCED_FACE('',(#168),#110,.T.) ;
#90=OPEN_SHELL('Extrudieren.1',(#169)) ;


//----------------------------------------------------------------
// B_SPLINE_SURFACE_WITH_KNOTS
u mult-list
v mult-list
u_knots-list
v_knots-list
knot_type



==========================================================
closedShell_objects:
==========================================================
TODO ..


==========================================================
subModelReferences:
==========================================================

#442=NEXT_ASSEMBLY_USAGE_OCCURRENCE('*DIT112','*DIT112','*DIT112',#41,#280,' ');
  // L:  PRODUCT_DEFINITION_SHAPE (following))  
  // P4: PRODUCT_DEFINITION of active-assy
  // P5: PRODUCT_DEFINITION of subModel

#448=PRODUCT_DEFINITION_SHAPE(' ',' ',#442);
  // L:  CONTEXT_DEPENDENT_SHAPE_REPRESENTATION (following)
  // P3: NEXT_ASSEMBLY_USAGE_OCCURRENCE (above)

#450=CONTEXT_DEPENDENT_SHAPE_REPRESENTATION(#449,#448);
  // L:  unreferenced.
  // P1: REPRESENTATION_RELATIONSHIP (following)
  // P2: PRODUCT_DEFINITION_SHAPE    (above)

#449=(REPRESENTATION_RELATIONSHIP(' ',' ',#277,#428)
    // RR-P3: SHAPE_REPRESENTATION of open-part (parent)
    // RR-P4: SHAPE_REPRESENTATION of subModel
  REPRESENTATION_RELATIONSHIP_WITH_TRANSFORMATION(#447)
    // RRWT-P1: ITEM_DEFINED_TRANSFORMATION (following)
  SHAPE_REPRESENTATION_RELATIONSHIP());

#447=ITEM_DEFINED_TRANSFORMATION(' ',' ',#446,#441);
  // P3: AXIS - refSys Assy   (active parent-model - eg default-Axis initBlock)
  // P4: AXIS - refSys subModel

// subModel-refSys
#441=AXIS2_PLACEMENT_3D(name, <CARTESIAN_POINT>, <DIRECTION>, <DIRECTION>)
  // P2: origin
  // P3: z-axis
  // P4: x-axis


==========================================================
model_endBlock:          STP_w_mdl_close()
==========================================================

//------ close wireframe_obj_list -------//

#227=GEOMETRIC_CURVE_SET('',(#216,#226));
// List of COMPOSITE_CURVes

#228=GEOMETRICALLY_BOUNDED_WIREFRAME_SHAPE_REPRESENTATION('',(#227),#121);
// P2: GEOMETRIC_CURVE_SET (above)
// P3: GEOMETRIC_REPRESENTATION_CONTEXT

// also possible:
// #227=GEOMETRIC_SET('',(#216,#226));
// #228=GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION('',(#227),#15);


//------ close openShell_obj_list -------//

// OPEN_SHELL can have 1-n ADVANCED_FACEs;
// MANIFOLD_SURFACE_SHAPE_REPRESENTATION can have 1-n openShells
// Example OPEN_SHELL from 2 ADVANCED_FACEs:        sur2tria.stp

#83=ADVANCED_FACE('',(#82),#56,.T.) ;
#119=ADVANCED_FACE('',(#118),#92,.T.) ;

// --- close open-shell:
#50=OPEN_SHELL('',(#83)) ;
// L:  SHELL_BASED_SURFACE_MODEL (following)
// P2: ADVANCED_FACE

#51=SHELL_BASED_SURFACE_MODEL('NONE',(#50)) ;
// L:  MANIFOLD_SURFACE_SHAPE_REPRESENTATION (following)
// P2: OPEN_SHELL (above)

#86=OPEN_SHELL('',(#119)) ;
// L:  SHELL_BASED_SURFACE_MODEL (following)
// P2: ADVANCED_FACE
#87=SHELL_BASED_SURFACE_MODEL('NONE',(#86)) ;
// L:  MANIFOLD_SURFACE_SHAPE_REPRESENTATION (following)
// P2: OPEN_SHELL (above)

#84=MANIFOLD_SURFACE_SHAPE_REPRESENTATION('NONE',(#51,#87),#45) ;
// P1: List of SHELL_BASED_SURFACE_MODELs  (above)
// P2: GRC (tolerances)

#85=SHAPE_REPRESENTATION_RELATIONSHIP(' ',' ',#48,#84) ;
// L:  unref.
// P3: SHAPE_REPRESENTATION of part
// P4: MANIFOLD_SURFACE_SHAPE_REPRESENTATION (above)


//------ close model -------//

#277=SHAPE_REPRESENTATION('*MASTER',(#441,#454,#463,#472),#45);
  // P2: axis of model (eg SI_AXM) and axes of all subModels (from IDT)
  // P3: GEOMETRIC_REPRESENTATION_CONTEXT (eg SI_GRC0)

#478=SHAPE_DEFINITION_REPRESENTATION(#44,#277);
  // P1: PRODUCT_DEFINITION_SHAPE
  // P2: SHAPE_REPRESENTATION (above)

// only with wireframe-objects:
#479=SHAPE_REPRESENTATION_RELATIONSHIP(' ',' ',#277,#91);
  // P3: SHAPE_REPRESENTATION of refsys of model
  // P4: GBWSR (wireframe_obj_list)

// only with openShell-objects:
#435=SHAPE_REPRESENTATION_RELATIONSHIP(' ',' ',#428,#420);
  // P3: SHAPE_REPRESENTATION of refsys of model
  // P4: MSSR (openShell_obj_list)


==========================================================
FILE_EXITBLOCK:
==========================================================
ENDSEC;
END-ISO-10303-21;


//================================================================
TODO:

// external model:
#270=PRODUCT('doc_4711','packaging guide','packaging guideline for part',(#260));
#300=PRODUCT_RELATED_PRODUCT_CATEGORY('document',$,(#270));
#310=DOCUMENT_TYPE('unspecified');
#320=DOCUMENT_FILE('t1','text.doc','file with text for the guide',#310,'','');
#330=PRODUCT_DEFINITION_FORMATION('ver3.2','version 3.2',#270);
#340=DOCUMENT_FILE('l1','',$,#310,'','');
#440=DOCUMENT_REPRESENTATION_TYPE('digital',#340);
#350=PRODUCT_DEFINITION_WITH_ASSOCIATED_DOCUMENTS('rep_id_3.2',
  'representation of version 3.2',#330,#250,(#340,#320));


//================================================================
 


//==========================================================
*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>              // for ...

#include <setjmp.h>              // longjmp
#include <time.h>                // localtime


// #include <GL/gl.h>                        // GL_TRIANGLES ..
// CANNOT include <GL/gl.h>
// /usr/include/GL/gl.h
#define GL_TRIANGLES        0x0004
#define GL_TRIANGLE_STRIP   0x0005
#define GL_TRIANGLE_FAN     0x0006


#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
// die folgenden 2 Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int STP_w__ (char*);
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif


#include "../ut/ut_cast.h"             // LONG_PTR
#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_ox_base.h"          // OGX_SET_INDEX
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../ut/ut_obj.h"              // UTO_stru_2_obj
#include "../ut/ut_txfil.h"            // UTF_GetPosLnr
#include "../ut/ut_memTab.h"           // MemTab

#include "../ut/func_types.h"               // UI_Func... SYM_..
#include "../gr/ut_DL.h"               // DL_GetAtt

#include "../db/ut_DB.h"               // DB_GetGTxt

#include "../xa/xa.h"                  // AP_mod_fnam
#include "../xa/xa_msg.h"                 // MSG_ERR_typ_*
#include "../xa/xa_mem.h"              // memspc201


// #include "../xa/xa_ui.h"               // UID_..
// #include "../xa/ut_ui_TX.h"            // TX_Print
// #include "../ut/byteorder.h"


//----------------------------------------------------------------
#define STP_VERSION "V0.2"
//----------------------------------------------------------------




//----------------------------------------------------------------
// EXTERN VARS:
// ../xa/xa.h
extern char AP_mod_fnam[SIZMFNam];     // active Modelname without filetyp

//----------------------------------------------------------------
// LOCAL VARS:
typedef_MemTab(int);
static MemTab(int) ol_ref = _MEMTAB_NUL;  // list of reference-systems
static MemTab(int) ol_oSh = _MEMTAB_NUL;  // open-shell-objects
static MemTab(int) ol_GS  = _MEMTAB_NUL;  // obj's for GEOMETRIC_SET
// static MemTab(int) olSs = _MEMTAB_NUL;


static int  exp_errNr = 0;
static int  exp_objNr = 0;
static FILE *stpw_fp;
static int  stpw_li;
static int  stpw_OpenShell=0;    // link to MANIFOLD_SURFACE_SHAPE_REPRESENTATION
static int  si_assy_pds = -1;    // PRODUCT_DEFINITION_SHAPE of active assy
static int  si_assy_sr  = -1;    // SHAPE_REPRESENTATION     of active assy

static char MdlNam[256];         // name of active model / subModel
static char *stpwTrue={".T."};
static char *stpwFalse={".F."};
static char stpwClo[] = ".CONTINUOUS.";
static char stpwOpe[] = ".DISCONTINUOUS.";
static char stptStrNull[4]={""};


// //---- step-refereneces of active model --------------------------
// // pds   PRODUCT_DEFINITION_SHAPE
// // grc   GEOMETRIC_REPRESENTATION_CONTEXT
// // bmi   gcad-basicModelIndex; -1=main, 0-n=subModel
// static struct  {int pds, grc, bmi;} actMdl;



// fixed (global) step-indexes
#define SI_PC    9 // PRODUCT_CONTEXT
#define SI_PDC  10 // PRODUCT_DEFINITION_CONTEXT
#define SI_AXM  11 // main-AXIS2_PLACEMENT_3D
#define SI_GRC0 15 // GEOMETRIC_REPRESENTATION_CONTEXT tolerances default


// step-indexes of basicModels
// mNam  (sub)Modelname; "-" in rec 0 = main
// stat  0=new; 1=initialized; 2=closed.
// mbi   cad-basicModelIndex; (only valid in state model-create)
// p__   PRODUCT
// pd_   PRODUCT_DEFINITION
// pds   PRODUCT_DEFINITION_SHAPE
// ax_   AXIS2_PLACEMENT_3D
// sr_   SHAPE_REPRESENTATION
typedef struct  {int stat, mbi, p__, grc, pds, pd_, ax_, sr_;
                 char mNam[128]; }                                  oMdl;

typedef_MemTab(oMdl);
static MemTab(oMdl) mdlTab = _MEMTAB_NUL;
static oMdl         *actTab;    // mdlTab[bmi]
//                        stat, mbi, p__, grc, pds, pd_, ax_, sr_
static oMdl MDLTAB_NUL = { -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}; 


// DB points
typedef struct  {int dbi, iPt;}  stp_pt;
typedef_MemTab(stp_pt);
static MemTab(stp_pt) oDB_pt = _MEMTAB_NUL;    // List cad-index, step-index


// DB trimmed-curves
// typ, dbi   cad-typ and index;
// iCv        step-index of curve (LINE|CIRCLE|B_SPLINE_CURVE_WITH_KNOTS
// iP1,iP2    step-indexes of the trimpoints of curve iCv
typedef struct  {int typ, dbi, iCv, iP1, iP2;}  stp_cv;
typedef_MemTab(stp_cv);
static MemTab(stp_cv) oDB_cv = _MEMTAB_NUL;   // List cad-typ, cad-index, step-index
// static stp_cv OBJDB_NUL = { 0,  0,  0}; 


static MemTab(int) obj_in  = _MEMTAB_NUL;   // DL-indexes of objs to export


// pb1, pb2  3D-box of outer-boundary
// bp        backplane of outer-boundary; 0,1,2
// ivx       -1 vector vx not yet created
// sro       senseOfRotation of outer-boundary; 0=CCW; 1=CW
// sri       senseOfRotation of inner-boundary;
// stat      0=create outer-boundary; 1=create inner-boundary;
// typ       typ of active obj
// odc       data of active obj
typedef struct {Point po, pb1, pb2; Vector vx, vz;
                int typ, ipo, ivz, ivx, iCv, iP1, iP2;
                void  *odc;
                char bp, sro, sri, stat;}                           objAct;

static objAct obj_act;

static jmp_buf     jmp1;

//----------------------------------------------------------------
// PROTOTYPES:
  void STP_w_ERREX (char *s1);
  int STP_w_log_err (char *eTxt, ...);




//================================================================
  int STP_w_mdlTab_ck_nam (char *mnam) {
//================================================================
// STP_w_mdlTab_ck_nam       check if model already exists in mdlTab
// retCode:   0: model mnam not yet defined;   1: alredy exist;

  int    irc, i1;
  oMdl   *oma;

  printf("STP_w_mdlTab_ck_nam %d |%s|\n",mdlTab.rNr,mnam);

  irc = -1;
  if(mdlTab.rNr < 2) goto L_exit;     // rec 0 = "-main-"

  oma = MEMTAB_DAT(&mdlTab);

  for(i1=0; i1<mdlTab.rNr; ++i1) {
    if(!strcmp(mnam, oma[i1].mNam)) {irc = i1; break;}
  }

  L_exit:
    printf("ex-STP_w_mdlTab_ck_nam %d\n",irc);

  return irc;

}

//================================================================
  int STP_w_mdlTab_ck_nxt () {
//================================================================
// STP_w_mdlTab_ck_nxt       find next new subModel in mdlTab
// retCode:   -1: no new model exists; >=0: index in mdlTab
  
  int    irc, i1;
  oMdl   *oma;


  irc = -1;
  oma = MEMTAB_DAT(&mdlTab);
  
  for(i1=1; i1<mdlTab.rNr; ++i1) {
    if(oma[i1].stat) continue;
    irc = i1;
    break;
  }
  
     printf("ex-STP_w_mdlTab_ck_nxt %d\n",irc);

  return irc;

}


//================================================================
  int STP_w_mdlTab_add (char *mnam) {
//================================================================
// STP_w_mdlTab_add              create new record in mdlTab
// retCod:   0=created; 1=did already exist;

  int         mbi;
  oMdl        om1;
  long        l1;


  printf("STP_w_mdlTab_add |%s|\n",mnam);

  // test if sm already exists
  if(STP_w_mdlTab_ck_nam (mnam) >= 0) return 1;

  // get mbi from modelName
  mbi = MDL_imb_mNam (mnam, 0);

  // create new record
  om1 = MDLTAB_NUL;
  strcpy(om1.mNam, mnam);
  om1.stat = 0;
  om1.mbi  = mbi;
  MemTab_add (&mdlTab, &l1, &om1, 1, 0);

  return 0;

}


//================================================================
  int STP_w_mdlTab_main () {
//================================================================

  oMdl        om1;
  long        l1;

  om1 = MDLTAB_NUL;
  strcpy(om1.mNam, "-main-");
  om1.stat = 0;
  om1.mbi  = -1;
  MemTab_add (&mdlTab, &l1, &om1, 1, 0);

  return 0;

}


//================================================================
  int STP_w__ (char* fnam) {
//================================================================
// export step-file
// fnam muss korrekten fileTyp .stp haben !


  int         irc, i1, ii;
  long        l1;
  char        s1[256];
  ModelBas    *mbo;

 
  printf("test_stp_w |%s|\n",fnam);


//   // remove all old files
//   sprintf (s1, "%sexp*.exp",OS_get_tmp_dir());
//   OS_file_delGrp (s1);


  LOG_A_init ("exp_export");

  if ((stpw_fp = fopen (fnam, "w+")) == NULL) {
    TX_Error ("STP_w__ Open |%s|",fnam);
    return -1;
  }

  // get ii = max nr of models
  ii = DB_get_ModBasNr() + 1; // get nr of basicModels + mainModel
    printf(" ModBasNr=%d\n",ii);

  // init list DL-indexes of objs to export
  MemTab_ini__ (&obj_in, sizeof(int), Typ_Int4, 1000);

  // init list points
  MemTab_ini__ (&oDB_pt, sizeof(stp_pt), Typ_unknown, 10000);

  // init list curves
  MemTab_ini__ (&oDB_cv, sizeof(stp_cv), Typ_unknown, 1000);


  // init list of structs for step-references of all models
  MemTab_ini__ (&mdlTab, sizeof(oMdl), Typ_unknown, ii);
  // mdltab: make sure that enough records free - else realloc = actTab KO
  MemTab_add (&mdlTab, &l1, NULL, ii, 2);

  // reference-systems 
  MemTab_ini__ (&ol_ref, sizeof(int), Typ_Int4, 1000);

  // init memspace for step-indextables 
  MemTab_ini__ (&ol_GS, sizeof(int), Typ_Int4, 10000);   // wireframe-objects
  MemTab_ini__ (&ol_oSh, sizeof(int), Typ_Int4, 10000);  // open-shell-objects
  // MemTab_ini__ (&olSs, sizeof(int), Typ_Int4, 10000);


  i1 = setjmp(jmp1); // set errex-address
  if(i1) goto L_finish;


  // write header
  STP_w_init__ ();


  //================================================================
  // process mainModel
  //================================================================


  // get obj_in = list of objs in DL 
  // get nr of objs in active group
  i1 = Grp_get_nr ();
  if(i1 > 0) {
    // copy objs of active group into private group obj_in
    Grp1_add__ (&obj_in);

  } else {
    // get all objs in primary-model
    Grp1_add_sm_dl (&obj_in, -1);  // -1=main
  }

  // init oDB_cv
  MEMTAB_CLEAR (&oDB_pt);
  MEMTAB_CLEAR (&oDB_cv);

  // add main into mdlTab
  STP_w_mdlTab_main ();

  stpw_li = 20;


    // TESTBLOCK
    // MemTab_dump (&obj_in, "exp_export-L1");
    // return -1;
    // END TESTBLOCK


   // get actTab out of mdlTab  (main always first)
   actTab = MEMTAB__ (&mdlTab, 0);
     STP_w_dump_mdlTab ("STP_w__-L1");

    printf("\n====================== MAIN ======================= \n");
    // printf("exp_export ============================ %s\n",actTab->bmi);
    // printf("  actTab-ax_=%d pd_=%d sr_=%d\n",
           // actTab->ax_, actTab->pd_, actTab->sr_);


  // export all objs of obj_in
  irc = STP_w_mdl__ ();
  if(irc < 0) goto L_finish;




  //================================================================
  // process subModels
  //================================================================
  if(mdlTab.rNr < 2) goto L_finish;

  // store active DB
  DB_save__ ("");

 
  //----------------------------------------------------------------
  L_sm_start:
    // find next new subModel in mdlTab
    i1 = STP_w_mdlTab_ck_nxt ();
    if(i1 < 0) goto L_sm_done; // no more unprocessed sm

    // get actTab = mdlTab-record
    actTab = MEMTAB__ (&mdlTab, i1);
      STP_w_dump_mdlTab ("STP_w__-L2");


      printf("\n\n============ SM =========================\n");
      printf(" ------- nxt-sm mbi=%d |%s|\n",actTab->mbi,actTab->mNam);
      // printf("  actTab-ax_=%d pd_=%d sr_=%d\n",
             // actTab->ax_, actTab->pd_, actTab->sr_);

    // test typ of subModel
    mbo = DB_get_ModBas (actTab->mbi);
      DEB_dump_obj__(Typ_SubModel, mbo, "sm %d",actTab->mbi);

    if(mbo->typ != MBTYP_INTERN) {
      // get external-modelfile (use existing <model>.wrl
      sprintf(s1,"***** external-modelfile not yet supported ***");
      TX_Print(s1);
      LOG_A__ (MSG_ERR_typ_INF, s1);
      ++exp_errNr;
      // STP_w_ox_sm_ext (mbo->mnam);
      // goto L_sm_cont;
    }

    // activate subModel
    DB_load__ (actTab->mNam);

    // get group obj_in of all objs of subModel mbi in DL
    Grp1_add_sm_dl (&obj_in, actTab->mbi);

    // init oDB_cv
    MEMTAB_CLEAR (&oDB_pt);
    MEMTAB_CLEAR (&oDB_cv);

    // export subModel
    irc = STP_w_mdl__ ();
    if(irc < 0) goto L_finish;

    goto L_sm_start;


  //----------------------------------------------------------------
  L_sm_done:
  // reload main-DB
  DB_load__ ("");



  //================================================================
  L_finish:
  //================================================================

  // printf("\n\n================================================== \n");
  // printf(" ------- finish\n");

  // output exitBlock
  STP_w_end ();

  fclose (stpw_fp);

  LOG_A__ (MSG_ERR_typ_INF, "nr of errors = %d", exp_errNr);
  LOG_A__ (MSG_ERR_typ_INF, "nr of objs exported = %d", exp_objNr);
  LOG_A_exit (exp_errNr);

  MemTab_free (&ol_ref);
  // MemTab_free (&olSs);
  MemTab_free (&ol_GS);
  MemTab_free (&ol_oSh);
  MemTab_free (&oDB_cv);
  MemTab_free (&oDB_pt);
  MemTab_free (&obj_in);



  TX_Print("%s exported ..",fnam);

    // TESTBLOCK
    printf(" ex-STP_w__ %s\n",fnam);
    // LOG_A_disp ();
    // END TESTBLOCK

  // join all files ext_*.ext" into single file "export.ext
  // STP_w_join ();


  return exp_errNr;

}

/*
//================================================================
  int STP_w_actTab (int bmi) {
//================================================================
// get actTab (find record with bmi)

  int   i1, ii;

  ii = mdlTab.rNr;

  for(i1=0; i1<ii; ++i1) {
    actTab = MEMTAB__ (&mdlTab, i1);
    if(actTab->bmi == bmi) goto L_exit;
  }

	printf("***** ERR STP_w_actTab bmi=%d\n",bmi);

  return -1;

  L_exit:
  return 0;

}


//================================================================
  int STP_w_MdlNam_bmi (int bmi) {
//================================================================
// bmi -1 =main, else index basicModel

  ModelBas    *mbo;

 
  // fix modelname and mbi
  if(bmi < 0) {
    strcpy(MdlNam, AP_mod_fnam); // MainModel = empty name

  } else {
    // external-subModel can have symbolic directory; remove
    // get modelName from index
    mbo = DB_get_ModBas (bmi);
      // DEB_dump_obj__(Typ_SubModel, mbo, "sm %d",bmi);
    strcpy(MdlNam, mbo->mnam);
    UTX_ftyp_cut (MdlNam);
    UTX_safeName (MdlNam, 1);
  }
    printf("exSTP_w_MdlNam_bmi bmi=%d MdlNam |%s|\n",bmi,MdlNam);

  return 0;

}
*/

//================================================================
  int STP_w_mdl__ () {
//================================================================
// Input:
//   actTab   the model zu export
//   obj_in     objects of model <actTab> to export

  int         irc, i1, rNr, oNr, iTyp, mbi;
  long        l1, dbi, dli;
  char        oid[40];
  DL_Att      *dla;
  ModelBas    *mbo;
  ModelRef    *mr;
  ObjGX       ox1;


  printf("\n\n=========STP_w_mdl__=======================\n");
  printf(" actTab.mNam-1 |%s|\n",actTab->mNam);



  MEMTAB_CLEAR (&ol_GS);    // clear wireframe-obj-list
  MEMTAB_CLEAR (&ol_oSh);   // clear openShell-obj-list
  // MEMTAB_CLEAR (&ol_cSh);   // clear closedShell-obj-list


  // add global refsys
  obj_act.iCv = SI_AXM;
  STP_w_oDBcv_add (Typ_PLN, DB_PLZ_IND, SI_AXM, 0, 0);


  // get rNr = nr of used subModels and add as new into mdlTab
  rNr = STP_w_mdlTab_upd ();

//   if(rNr) {
//     // model has subModel(s); create assy first
//     irc = STP_w_ass__ (rNr);
//   }


  //================================================================
  // write model_startBlock
  STP_w_mdl_start ();

  //----------------------------------------------------------------
  // export objs
  oNr = obj_in.rNr;
  DL_get__ (&dla);
    printf(" w_mdl__-oNr=%d\n",oNr);


  for(i1=0; i1<oNr; ++i1) {
      printf("\n ------- nxt-obj %d\n",i1);

    dli = *((int*)MEMTAB__ (&obj_in, i1));
    iTyp = dla[dli].typ;

    // skip this types:
    if(iTyp == Typ_PT) continue;
    if(iTyp == Typ_Ditto) continue;
    if(iTyp == Typ_apDat) continue;


    dbi = dla[dli].ind;

    // get DB-obj
    ox1 = DB_GetObjGX (iTyp, dbi);
    if(ox1.typ == Typ_Error) {
      LOG_A__ (MSG_ERR_typ_ERR, "exp_export typ=%d dbi=%ld",iTyp,dbi);
      ++exp_errNr;
      continue;
    }

    // get objID
    APED_oid_dbo__ (oid, iTyp, dbi);

    // export DB-obj
    irc = STP_w_ox__ (&ox1, dbi, oid);
    if(!irc) ++exp_objNr;
  }

  //----------------------------------------------------------------

  // close model
  STP_w_mdl_close ();

  return 0;

}


//================================================================
  int STP_w_mdlTab_upd () {
//================================================================
// STP_w_mdlTab_upd         find all used subModels and add as new into mdlTab

  int         i1, i2, rNr, oNr, iTyp, mbi;
  long        l1, dbi, dli;
  DL_Att      *dla;
  ModelRef    *mr;
  ModelBas    *mbo;
  ObjGX       ox1;


  oNr = obj_in.rNr;
  DL_get__ (&dla);
    printf("STP_w_mdlTab_upd-oNr=%d\n",oNr);

  rNr = 0;

  for(i1=0; i1<oNr; ++i1) {
  
    dli = *((int*)MEMTAB__ (&obj_in, i1));
    iTyp = dla[dli].typ;
      printf(" mdlTab_dl-iTyp=%d\n",iTyp);
  
    if((iTyp == Typ_Model)||(iTyp == Typ_Mock)) {
      ++rNr;
      dbi = dla[dli].ind;
      ox1 = DB_GetObjGX (iTyp, dbi);  // get DB-obj
      mr = ox1.data;                  // is a ModelReference
      mbi = mr->modNr;                // basicModelIndex
      mbo = DB_get_ModBas (mbi);      // get basicModel
        DEB_dump_obj__(Typ_SubModel, mbo, "sm %d",mbi);
    
      // create new record in mdlTab
      i2 = STP_w_mdlTab_add (mbo->mnam);

    }
  }

    printf("ex-STP_w_mdlTab_upd rNr=%d\n",rNr);

  return rNr;

}


//================================================================
  int STP_w_ox__ (ObjGX *ox1, long dbi, char *oid) {
//================================================================
// export DB-obj
// Input:


  int      irc, typ, form, ii, iStp, mode;
  long     l1;
  void     *obj;
  char     s1[256];
  Point    actPos;
  ModelBas *mbo;
  ModelRef *mro;


  printf("STP_w_ox__ |%s| ox1-typ=%d ox1-form=%d dbi=%ld\n",
         oid, ox1->typ, ox1->form, dbi);
  DEB_dump_ox__ (ox1, "");


  obj = ox1->data;
  typ = ox1->typ;
  form = ox1->form;


  //----------------------------------------------------------------
  // test export surface
  if(TYP_IS_SUR(typ)) {
    // sur or sol
    iStp = STP_w_SUR__ (ox1, dbi, oid);

    // add stepID irc to openshell_objects-list ol_oSh
    if(iStp > 0) MemTab_add (&ol_oSh, &l1, &iStp, 1, 0);


  //----------------------------------------------------------------
  // test export curve (L|C|elli|polg|bspl)
  } else if((TYP_IS_CV(typ))||(typ == Typ_LN)||(typ == Typ_CI)) {

    // create basicCurve (L|C|Elli|bspl)
    iStp = STP_w_CRV__ (ox1, dbi, oid, 2);  // 2=wireframe-obj

    // // add step-index of curve into oDB_cv 
    // STP_w_oDBcv_add (ox1->typ, dbi);

    // write COMPOSITE_CURVE < from stepID iStp 
    if(iStp > 0) iStp = STP_w_COMPOSITE_CURVE (&iStp, 1, oid);

    // add stepID iStp to wireframe_objects-list ol_GS
    if(iStp > 0) MemTab_add (&ol_GS, &l1, &iStp, 1, 0);

    
  //----------------------------------------------------------------
  // test export subModel
  } else if(typ == Typ_Model) {
    iStp = STP_w_MdlRef ((ModelRef*)ox1->data);


  //----------------------------------------------------------------
  } else {
    sprintf(s1,"  STP_w_ox__ skip form=%d dbi=%ld",ox1->form,dbi);
    LOG_A__ (MSG_ERR_typ_ERR, s1);
    ++exp_errNr;
    iStp = -1;
  }


  //----------------------------------------------------------------
  L_exit:
    printf(" ex-STP_w_ox__ iStp=%d\n",iStp);

  return iStp;

}

/*
//================================================================
  ObjGX OGX_oxDat_oxInd (ObjGX *oxi) {
//================================================================
// OGX_oxDat_oxInd            complexObj with data from complexObj with DB-index 
// - resolv Typ_Index

  int    typ, form, oNr;
  long   dbi;
  void   *data;
  ObjGX  ox1;


  if(oxi->form != Typ_Index) return *oxi;

  DEB_dump_ox__ (oxi, "OGX_oxDat_oxInd-in");


  OGX_GET_INDEX (&typ, &dbi, oxi);
    printf(" oxDat_oxInd typ=%d dbi%ld\n",typ,dbi);

  form = UTO__dbo (&data, &oNr, typ, dbi);
    printf(" oxDat_oxInd form=%d oNr=%d\n",typ,oNr);

  OGX_SET_OBJ (&ox1, form, form, oNr, data);

    DEB_dump_ox__ (&ox1, "ex-OGX_oxDat_oxInd");

  return ox1;

}
*/

//================================================================
  int STP_w_CRV__ (ObjGX *ox1, long dbi, char *oid, int mode) {
//================================================================
// STP_w_CRV__                        create curve
// Input:
//   mode    2 = wireframe;   3 = shell-boundary;


  int      form, iStp;
  char     s1[256];
  ObjGX    ox2;


  printf("STP_w_CRV__ |%s| ox1-typ=%d ox1-form=%d dbi=%ld mode=%d\n",
         oid, ox1->typ, ox1->form, dbi, mode);
  DEB_dump_ox__ (ox1, "STP_w_CRV__-in");

  form = ox1->form;


  if(form == Typ_Index) {
    ox2 = OGX_oxDat_oxInd (&dbi, ox1);
    ox1 = &ox2;
    form = ox1->form;
  }


  //----------------------------------------------------------------
// test TYP_IS_CV 
  switch (form) {

    //----------------------------------------------------------------
    case Typ_LN:
      iStp = STP_w_LN ((Line*)ox1->data, dbi, oid, mode);
      break;

    //----------------------------------------------------------------
    case Typ_CI:
      iStp = STP_w_AC_CC ((Circ*)ox1->data, dbi, oid, mode);
      break;

    //----------------------------------------------------------------
    case Typ_CVELL:
      iStp = STP_w_EL_CC ((CurvElli*)ox1->data, dbi, oid, mode);
      break;

    //----------------------------------------------------------------
    case Typ_CVPOL:
      iStp = STP_w_CVPOL ((CurvPoly*)ox1->data, dbi, oid, mode);
      break;

    //----------------------------------------------------------------
    case Typ_CVBSP:
      iStp = STP_w_CVBSP ((CurvBSpl*)ox1->data, dbi, oid, mode);
      break;

    //----------------------------------------------------------------
    case Typ_CVTRM:
//       ia = (int*)MEM_alloc_tmp ((int)(((ObjGX*)ox1)->siz * sizeof(int)));
//       iStp = STP_w_CVCCV (NULL, ia, ox1, oid, 2); // makes COMPOSITE_CURVE !
      iStp = STP_w_CVTRM__ ((CurvCCV*)ox1->data, ox1->siz, oid, mode);
      // if(iStp > 0) goto L_wf;
      break;


    //=========================================================
    default:
      sprintf(s1,"STP_w_CRV__ %s - skip form=%d dbi=%ld",oid,ox1->form,dbi);
      LOG_A__ (MSG_ERR_typ_ERR, s1);
      ++exp_errNr;
      iStp = -1;
  }

    printf("ex-STP_w_CRV__ %s mode=%d %d\n",oid,mode,iStp);


  return iStp;

}


//================================================================
  int STP_w_MdlRef (ModelRef *mro) {
//================================================================
// STP_w_MdlRef             write mdlRef (referenced part)

  int      i1, mbi, iAX;
  long     l1;
  char     s1[256];
  ModelBas *mbo;
  oMdl     *refTab;


  // mbi = basicModelIndex
  mbi = mro->modNr;

  // get mbo = its basicModel
  mbo = DB_get_ModBas (mbi);
    printf("STP_w_MdlRef-L1 %s\n",mbo->mnam);

  // get mdlTab for model <mbo->mnam>
  i1 = STP_w_mdlTab_ck_nam (mbo->mnam);
  if(i1 < 0) {sprintf(s1,"STP_w_MdlRef E1 %s",mbo->mnam);return STP_w_log_inf(s1);}
  refTab = MEMTAB__ (&mdlTab, i1);
    STP_w_dump_oMdl (refTab, ":::::::::: STP_w_MdlRef-L2 :::::::::");

//   // create new record in mdlTab
//   STP_w_mdlTab_add (s1);



  // get s1 = mdlNam
  // internal or external model
  if(mbo->typ == MBTYP_INTERN) {
    // create modelname internal-submodel
    // sprintf(s3, "%s_%s",exp_mdlNam,mbo->mnam);
    STP_w_int_mnam (s1, mbo->mnam);
  } else {
    // MBTYP_EXTERN - export external-submodel
    STP_w_ext_mnam (s1, mbo->mnam);
  }

  // reserve index for PRODUCT_DEFINITION of subModel
  if(refTab->pd_ < 0) {refTab->pd_ = stpw_li; ++stpw_li;}

  // reserve index for SHAPE_REPRESENTATION of subModel
  if(refTab->sr_ < 0) {refTab->sr_ = stpw_li; ++stpw_li;}

  fprintf(stpw_fp,
    "/* modelref %s */\n", s1);

    printf("STP_w_MdlRef mbi=%d |%s|\n",mbi,s1);


  //----------------------------------------------------------------
  fprintf(stpw_fp,
    "#%d=NEXT_ASSEMBLY_USAGE_OCCURRENCE('','','',#%d,#%d,$);\n",
    stpw_li, actTab->pd_,  refTab->pd_);
  // L:  PRODUCT_DEFINITION_SHAPE (following))  
  // P4: PRODUCT_DEFINITION of active-assy
  // P5: PRODUCT_DEFINITION of subModel
  ++stpw_li;


  fprintf(stpw_fp,
    "#%d=PRODUCT_DEFINITION_SHAPE('','',#%d);\n",
    stpw_li, stpw_li-1);
  // L:  CONTEXT_DEPENDENT_SHAPE_REPRESENTATION (following)
  // P3: NEXT_ASSEMBLY_USAGE_OCCURRENCE (above)
  ++stpw_li;


  fprintf(stpw_fp,
    "#%d=CONTEXT_DEPENDENT_SHAPE_REPRESENTATION(#%d,#%d);\n",
    stpw_li, stpw_li+1, stpw_li-1);
  // L:  unreferenced.
  // P1: REPRESENTATION_RELATIONSHIP (following)
  // P2: PRODUCT_DEFINITION_SHAPE    (above)
  ++stpw_li;


  fprintf(stpw_fp,
    "#%d=(REPRESENTATION_RELATIONSHIP('','',#%d,#%d)\n",
    stpw_li, actTab->sr_, refTab->sr_);
    // RR-P3: SHAPE_REPRESENTATION of open-part (parent)
    // RR-P4: SHAPE_REPRESENTATION of subModel
  fprintf(stpw_fp,
    "  REPRESENTATION_RELATIONSHIP_WITH_TRANSFORMATION(#%d)\n",
    stpw_li+1);
    // RRWT-P1: ITEM_DEFINED_TRANSFORMATION (following)
  fprintf(stpw_fp,
    "  SHAPE_REPRESENTATION_RELATIONSHIP());\n");
  ++stpw_li;


  fprintf(stpw_fp,
    "#%d=ITEM_DEFINED_TRANSFORMATION('','',#%d,#%d);\n",
    stpw_li, SI_AXM, stpw_li+1);
    // stpw_li, actTab->ax_, iAX);
  // L:  REPRESENTATION_RELATIONSHIP_WITH_TRANSFORMATION(P1) (above)
  // P3: AXIS - refSys Assy   (parent-model)
  // P4: AXIS - refSys Detail (active-model)
  ++stpw_li;


  // set refsys of active model
  i1 = stpw_li;
  ++stpw_li;
  STP_w_axis__ (&i1, &mro->po, &mro->vz, &mro->vx);

  // add refsys to ol_ref
  MemTab_add (&ol_ref, &l1, &i1, 1, 0);
    // MemTab_dump (&ol_ref, "STP_w_MdlRef-L9");

  return 0;

}


//================================================================
  int STP_w_mdl_start () {
//================================================================
// write model_startBlock
// was STP_w_wf_init STP_w_os_init

  int   i1;
  long  l1;


  printf("STP_w_mdl_start \n");


  // init ol_ref, add default-refSys
  MEMTAB_CLEAR (&ol_ref);
  i1 = SI_AXM;
  MemTab_add (&ol_ref, &l1, &i1, 1, 0);


  // reserve step-index for PRODUCT_DEFINITION
  //   (for main; for submodels already fixed)
  if(actTab->pd_ < 0) {actTab->pd_ = stpw_li; ++stpw_li;}

  // reserve index for SHAPE_REPRESENTATION of subModel
  if(actTab->sr_ < 0) {actTab->sr_ = stpw_li; ++stpw_li;}


  //----------------------------------------------------------------
  fprintf(stpw_fp,
    "\n\n/*============= part_%s =============*/\n",
    actTab->mNam);


  fprintf(stpw_fp,
    "#%d=PRODUCT('part_%s','','None',(#%d));\n",
    stpw_li, actTab->mNam, SI_PC);
  // P4: PRODUCT_CONTEXT (global)  oder MECHANICAL_CONTEXT
  actTab->p__ = stpw_li;
  ++stpw_li;


  fprintf(stpw_fp,
    "#%d=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE('','',#%d,.NOT_KNOWN.);\n",
    stpw_li, stpw_li-1); // actTab->pd_);
  // L:  PRODUCT_DEFINITION (P3, following)
  // P3: PRODUCT (before)
  ++stpw_li;

  fprintf(stpw_fp,
    "#%d=PRODUCT_DEFINITION('%s','',#%d,#%d);\n",
    actTab->pd_, actTab->mNam, stpw_li-1, SI_PDC);
  // L:  PRODUCT_DEFINITION_SHAPE (following)
  // P3: PDFWSS (above)
  // P4: PRODUCT_DEFINITION_CONTEXT (global) or DESIGN_CONTEXT


  fprintf(stpw_fp,
    "#%d=PRODUCT_DEFINITION_SHAPE('','',#%d);\n",
    stpw_li, actTab->pd_);
  // L:  SHAPE_DEFINITION_REPRESENTATION(P1)
  // P3: PRODUCT_DEFINITION (above)
  actTab->pds = stpw_li;
  ++stpw_li;


//   fprintf(stpw_fp,
//     "#%d=PRODUCT_DEFINITION_FORMATION('','None',#%d);\n",
//     stpw_li, actTab->p__);
//   // P3: PRODUCT (following2)
//   ++stpw_li;
// 
// 
//   fprintf(stpw_fp,
//     "#%d=(GEOMETRIC_REPRESENTATION_CONTEXT(3)\n",
//     stpw_li);
//   fprintf(stpw_fp,
//     "  GLOBAL_UNCERTAINTY_ASSIGNED_CONTEXT((#4))\n");
//   fprintf(stpw_fp,
//     "  GLOBAL_UNIT_ASSIGNED_CONTEXT((#3,#7,#1))\n");
//   fprintf(stpw_fp,
//     "REPRESENTATION_CONTEXT('ID1','3D'));\n");
//   actTab->grc = stpw_li;
//   ++stpw_li;


  fprintf(stpw_fp,"\n");
  fprintf(stpw_fp,"\n");


  actTab->stat = 1;  // start done
    // printf(" actMdl pds=%d\n",actTab.pds);


  return 0;

}


//================================================================
  int STP_w_mdl_close () {
//================================================================
// write model_endBlock
// was STP_w_wf_init STP_w_os_init

  int    i1, wfNr, osNr, *ia, iNr, iSR, iWF, iOS;
  char   s1[2048];

  printf("STP_w_mdl_close |%s|\n",actTab->mNam);
  printf(" wfNr=%d\n",MEMTAB_IND(&ol_GS));
  printf(" osNr=%d\n",MEMTAB_IND(&ol_oSh));


  fprintf(stpw_fp,
    "\n\n/*============= close part_%s =============*/\n",actTab->mNam);


  // close wireframe_obj_list
  wfNr = MEMTAB_IND(&ol_GS);
  if(wfNr == 0) goto L_close_openShell;


  //----------------------------------------------------------------
  // close wireframe_obj_list

  // write GEOMETRIC_CURVE_SET
  ia = MEMTAB_DAT (&ol_GS);
  i1 = STP_w_CURVE_SET (ia, wfNr);

  fprintf(stpw_fp,
    "#%d=GEOMETRICALLY_BOUNDED_WIREFRAME_SHAPE_REPRESENTATION('',(#%d),#%d);\n",
    stpw_li, i1, SI_GRC0);
  // P2: GEOMETRIC_CURVE_SET
  // P3: GEOMETRIC_REPRESENTATION_CONTEXT (def=SI_GRC0)
  iWF = stpw_li;
  ++stpw_li;

  // also possible:
  // #227=GEOMETRIC_SET('',(#216,#226));
  // #228=GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION('',(#227),#15);

  fprintf(stpw_fp, "\n");


  //----------------------------------------------------------------
  // close openShell_obj_list
  L_close_openShell:
  iOS = STP_w_mdl_clo_oSh ();


  //----------------------------------------------------------------
  // close closedShell_obj_list
  // TODO ..


  //================================================================
  // close model
  L_close_model:


  ia = MEMTAB_DAT (&ol_ref);
  iNr = MEMTAB_IND (&ol_ref);
  s1[0] = '\0';
  sprintf(s1,"#%d=SHAPE_REPRESENTATION(''",actTab->sr_);
  STP_w_list__ (s1, sizeof(s1), ia, iNr, 1, ",");
  fprintf(stpw_fp,"%s,#%d);\n", s1, SI_GRC0);


  fprintf(stpw_fp,
    "#%d=SHAPE_DEFINITION_REPRESENTATION(#%d,#%d);\n",
    stpw_li, actTab->pds, actTab->sr_);
  // P1: PRODUCT_DEFINITION_SHAPE
  // P2: SHAPE_REPRESENTATION (above)
  ++stpw_li;



  // add wireframe_obj_list to model:
  if(wfNr) {
    fprintf(stpw_fp,
      "#%d=SHAPE_REPRESENTATION_RELATIONSHIP('','',#%d,#%d);\n",
    stpw_li, actTab->sr_, iWF);
  // P3: SHAPE_REPRESENTATION of refsys of model
  // P4: GBWSR (wireframe_obj_list)
    ++stpw_li;
  }


  // add openShell_obj_list to model:
  if(iOS) {
    fprintf(stpw_fp,
      "#%d=SHAPE_REPRESENTATION_RELATIONSHIP('','',#%d,#%d);\n",
    stpw_li, actTab->sr_, iOS);
  // P3: SHAPE_REPRESENTATION of refsys of model
  // P4: MSSR (openShell_obj_list)
    ++stpw_li;
  }



  //----------------------------------------------------------------
  L_exit:

    MEMTAB_CLEAR (&ol_GS);
    MEMTAB_CLEAR (&ol_oSh);

  return 0;

}


//================================================================
  int STP_w_mdl_clo_oSh () {
//================================================================
// STP_w_mdl_clo_oSh               close openShell_obj_list

  int     osNr, *ia, ii, iOS = 0;
  char    s1[256];

  osNr = MEMTAB_IND(&ol_oSh);
  if(!osNr) return 0;


  ia = MEMTAB_DAT (&ol_oSh);

  fprintf(stpw_fp,"/* set of open-shell-objects */\n");


  for(ii=0; ii<osNr; ++ii) {

    fprintf(stpw_fp,
      "#%d=OPEN_SHELL('',(#%d));\n",
      stpw_li, ia[ii]);
    // L:  SHELL_BASED_SURFACE_MODEL (following)
    // P2: ADVANCED_FACE
    ++stpw_li;


    fprintf(stpw_fp,
      "#%d=SHELL_BASED_SURFACE_MODEL('',(#%d));\n",
      stpw_li, stpw_li - 1);
    // L:  MANIFOLD_SURFACE_SHAPE_REPRESENTATION (following)
    // P2: OPEN_SHELL (above)
    ia[ii] = stpw_li; // keep link for MANIFOLD_SURFACE_SHAPE_REPRESENTATION
    ++stpw_li;

  }


  sprintf(s1,"#%d=MANIFOLD_SURFACE_SHAPE_REPRESENTATION(''",stpw_li);
  iOS = stpw_li;  // keep link for SHAPE_REPRESENTATION_RELATIONSHIP
  ++stpw_li;
  STP_w_list__ (s1, sizeof(s1), ia, osNr, 1, ",");
  fprintf(stpw_fp,"%s,#%d);\n\n",s1, SI_GRC0);
  // P1: List of SHELL_BASED_SURFACE_MODELs  (above)
  // P2: GRC (tolerances)



  return iOS;

}
 

/*
//================================================================
  int STP_init_mdl__ (MemTab(int) *obj_in) {
//================================================================
// STP_init_mdl__             init all new models in mdlTab

  int    ii;
  long   l1;
  oMdl   *om1;

  fprintf(stpw_fp,
    "\n\n//============= init models ==============================//\n");


  //----------------------------------------------------------------
  ii = -1;

  // loop tru all new basicModels in mdlTab
  L_nxt:
    ++ii;
    if(ii >= mdlTab.rNr) goto L_exit;
    // test if basicModel is new
    om1 = MEMTAB__(&mdlTab, ii);
    if(om1->stat != 0) goto L_nxt;

    // write model-initcode for model om1
    STP_init_mdl_w (om1);
    om1->stat = 1;
    goto L_nxt;


  //----------------------------------------------------------------
  L_exit:

    STP_w_dump_mdlTab ("ex-STP_init_mdl__");

  return 0;

}


//================================================================
  int STP_init_mdl_w (oMdl *om1) {
//================================================================
// write model-initcode for model om1.bmi
// Input:
//   om1.bmi

  int         bmi;
  ModelBas    *mbo;


  printf("STP_init_mdl_w |%s|\n",om1->mNam);

  // get basicModelIndex from basicModelName
  bmi = MDL_imb_mNam (om1->mNam);
  if(bmi < -1) {++exp_errNr; TX_Error("STP_init_mdl_w e1"); return -1;}


//   bmi = om1->bmi;
//     printf("STP_init_mdl_w bmi=%d\n",bmi);
// 
// 
//   // get modelName from index
//   mbo = DB_get_ModBas (bmi);
//     // DEB_dump_obj__(Typ_SubModel, mbo, "sm %d",actTab.bmi);


//   // fix MdlNam
//   // fix modelname and mbi
//   if(bmi < 0) {
//     strcpy(MdlNam, AP_mod_fnam); // MainModel = empty name
// 
//   } else {
//     // external-subModel can have symbolic directory; remove
//     strcpy(MdlNam, mbo->mnam);
//     UTX_ftyp_cut (MdlNam);
//     UTX_safeName (MdlNam, 1);
//   }
  

  strcpy(MdlNam, om1->mNam);

  fprintf(stpw_fp, "// model %s //\n", MdlNam);



  fprintf(stpw_fp,
    "#%d=PRODUCT('%s','','None',(#%d));\n",
    stpw_li, MdlNam, SI_PC);
  // P4: PRODUCT_CONTEXT
  om1->p__ = stpw_li;
  ++stpw_li;


  fprintf(stpw_fp,
    "#%d=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE('','',#%d,.NOT_KNOWN.);\n",
    stpw_li, stpw_li-1); // actTab->pd_);
  // L:  PRODUCT_DEFINITION (P3, following)
  // P3: PRODUCT (above)
  ++stpw_li;


  fprintf(stpw_fp,
    "#%d=PRODUCT_DEFINITION('','',#%d,#%d);\n",
    stpw_li, stpw_li-1, SI_PDC);
  // L:  PRODUCT_DEFINITION_SHAPE (following)
  // P3: PDFWSS (above)
  // P4: PRODUCT_DEFINITION_CONTEXT (global)
  ++stpw_li;


  fprintf(stpw_fp,
    "#%d=PRODUCT_DEFINITION_SHAPE('','',#%d);\n",
    stpw_li, stpw_li-1);
  // L:  SHAPE_DEFINITION_REPRESENTATION(P1)
  // P3: PRODUCT_DEFINITION (above)
  om1->pds = stpw_li;
  ++stpw_li;


  return 0;

}
*/
 
//================================================================
  int STP_w_init__ () {
//================================================================
// write header

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
    // printf(" timeString=|%s|\n",cbuf);


  fprintf(stpw_fp,"ISO-10303-21;\n");
  fprintf(stpw_fp,"HEADER;\n");
  fprintf(stpw_fp,"FILE_DESCRIPTION((''),'1');\n");
    // P1=comments
  fprintf(stpw_fp,"FILE_NAME(\n");
  fprintf(stpw_fp,"  '%s.%s',\n", AP_mod_fnam, AP_mod_ftyp);
    // P1: filename
  fprintf(stpw_fp,"  '%s',(''),(''),\n",cbuf);
    // P2: date_time
    // P3: author
    // P4: organization
  fprintf(stpw_fp,"  'preprocessor-none',\n");
    // P5: preprocessor
  fprintf(stpw_fp,"  'gCAD3D-STP_w-%s','');\n",STP_VERSION);
    // P6: originating_system
    // P7: authorization
  fprintf(stpw_fp,"FILE_SCHEMA (('CONFIG_CONTROL_DESIGN'));\n");
  fprintf(stpw_fp,"ENDSEC;\n");
  fprintf(stpw_fp,"DATA;\n\n");


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
"#9=PRODUCT_CONTEXT('',#8,'mechanical');\n");

  fprintf(stpw_fp,
"#10=PRODUCT_DEFINITION_CONTEXT('',#8,'');\n");

  fprintf(stpw_fp,"/* refSys default */\n");
  fprintf(stpw_fp,"#11=AXIS2_PLACEMENT_3D('',#12,#13,#14);\n");
  fprintf(stpw_fp,"#12=CARTESIAN_POINT('',(0.,0.,0.));\n");
  fprintf(stpw_fp,"#13=DIRECTION('',(0.,0.,1.));\n");
  fprintf(stpw_fp,"#14=DIRECTION('',(1.,0.,0.));\n");

  fprintf(stpw_fp,"/* tolerances default */\n");
  fprintf(stpw_fp,
    "#15=(GEOMETRIC_REPRESENTATION_CONTEXT(3)\n");
  fprintf(stpw_fp,
    "  GLOBAL_UNCERTAINTY_ASSIGNED_CONTEXT((#4))\n");
  fprintf(stpw_fp,
    "  GLOBAL_UNIT_ASSIGNED_CONTEXT((#3,#7,#1))\n");
  fprintf(stpw_fp,
    "REPRESENTATION_CONTEXT('ID1','3D'));\n");


// TODO: add basicPlane RZ to list of stored objects



  return 0;

}


/*
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
  char  s1[2048];
  
  
  printf("STP_w_os_init \n");

  iNr = MEMTAB_IND (&ol_oSh);
  if(iNr < 1) return -1;


  fprintf(stpw_fp,"\n// set of open-shell-objects //\n");

  sprintf(s1,"#%d=OPEN_SHELL(''",stpw_li);
  ++stpw_li;
  STP_w_list__ (s1, sizeof(s1), MEMTAB_DAT (&ol_oSh), iNr, 1, ",");
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


//
  fprintf(stpw_fp,
"#%d=SHAPE_REPRESENTATION_RELATIONSHIP('','',#17,#%d);\n",
          stpw_li, stpw_li - 1);
  ++stpw_li;
//


// no ADVANCED_FACEs with GEOMETRIC_SET
  sprintf(s1,"#%d=GEOMETRIC_SET(''",stpw_li);
  ++stpw_li;
  STP_w_list__ (s1, MEMTAB_DAT (&ol_oSh), iNr, 1, ",");
  fprintf(stpw_fp,"%s);\n",s1);
    
  fprintf(stpw_fp,
"#%d=GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION('',(#%d),#15);\n",
          stpw_li, stpw_li - 1);
  ++stpw_li;
//

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
  char  s1[2048];

  MemTab(int) obj_in = _MEMTAB_NUL;    // DL-indexes of objs to export



  printf("STP_w_wf_init \n");


  fprintf(stpw_fp,"\n// set of wireframe-objects //\n");


  // create GEOMETRIC_SET from RECTANGULAR_TRIMMED_SURFACEs
  // only for supportSurfaces (eg RECTANGULAR_TRIMMED_SURFACE):
  iPt = MEMTAB_IND (&ol_GS);
  if(iPt > 0) {
    sprintf(s1,"#%d=GEOMETRIC_SET(''",stpw_li);
    ++stpw_li;
    STP_w_list__ (s1, sizeof(s1), MEMTAB_DAT (&ol_GS), iPt, 1, ",");
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
// output of yet unused surfaces as OPEN_SHELL

  int      irc, apt_typ, anz_obj;
  long     l1, apt_ind, gr_ind;
  char     oid[40];
  void     *vDat;
  DL_Att   dla;
  ObjGX    ox1, *op1;


  printf("STP_w_cs__ \n");

  fprintf(stpw_fp,"\n\n//===========================================//\n");
  fprintf(stpw_fp,"//  closed-shell-objects //\n");

  return 0;

}


//================================================================
  int STP_w_os__ () {
//================================================================
// output of yet unused surfaces as OPEN_SHELL

  int      irc, apt_typ, anz_obj;
  long     l1, apt_ind, gr_ind;
  char     oid[40];
  void     *vDat;
  DL_Att   dla;
  ObjGX    ox1, *op1;


  printf("STP_w_os__ \n");


  fprintf(stpw_fp,"\n\n//===========================================//\n");
  fprintf(stpw_fp,"//  open-shell-objects //\n");

  anz_obj = 0;
  gr_ind = GL_Get_DLind();

    
  for(l1=0; l1<gr_ind; ++l1) {

    // dla = DL_GetAtt(l1);                      // get alle DL-Infos
    irc = DL_dlRec__dli (&dla, l1);                      // get alle DL-Infos
    if(!irc) continue;

    // if(dla.disp == OFF) continue;           // skip hidden obj's
    if(DL_IS_HIDDEN(dla)) continue;        // skip hidden obj's

    apt_typ = dla.typ;
    apt_ind = dla.ind;

    if(apt_typ != Typ_SUR) continue;
    

    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;  // skip unknown types

    // dTyp = UTO__dbo (&vDat, apt_typ, apt_ind);
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
  // output of yet unused curves as wireframe
 
  int         irc, anz_obj, apt_typ, anz_sur, dTyp;
  long        l1, apt_ind, gr_ind;
  char        oid[40];
  void        *vDat;
  DL_Att      dla;
  ObjGX       ox1, *op1;


  printf("STP_w_wf__ \n");


  fprintf(stpw_fp,"\n\n//===========================================//\n");
  fprintf(stpw_fp,"//  wireframe-objects //\n");

  anz_obj = 0;
  anz_sur = 0;
  gr_ind = GL_Get_DLind();


  for(l1=0; l1<gr_ind; ++l1) {

    // dla = DL_GetAtt(l1);                      // get alle DL-Infos
    irc = DL_dlRec__dli (&dla, l1);  
    if(!irc) continue;

    // if(dla.disp   == OFF) continue;           // skip hidden obj's
    if(DL_IS_HIDDEN(dla)) continue;        // skip hidden obj's

    apt_typ = dla.typ;
    apt_ind = dla.ind;

    if(apt_typ == Typ_SUR) { anz_sur += 1; continue; }   // erst em Ende ..

    ox1 = DB_GetObjGX (apt_typ, apt_ind);
    if(ox1.typ == Typ_Error) continue;  // skip unknown types

    // dTyp = UTO__dbo (&vDat, apt_typ, apt_ind);
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
  sTyp = UTO_ck_surfTyp (ox1); // Typ_SUTP| ?
    printf(" sTyp=%d\n",sTyp);


  fprintf(stpw_fp,"\n// Surface %s //\n",oid);


  switch(typ) {

    //----------------------------------------------------------------
    case Typ_SURCIR:
      // ADVANCED_FACE
      ii = STP_w_SURCIR ((ObjGX*)ox1, oid);
      break;


    //----------------------------------------------------------------
    case Typ_SUR:         // sTyp Typ_SUTP
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
*/

//================================================================
  int STP_w_SURBSP (SurBSpl *su1, char *oid) {
//================================================================
// Area: B-Spline-Surf   Typ_SURBSP (55)
//
// TODO: - BsplineSurface (SurBsp ObjGX typ=56)
// - function STP_w_SURSUP__ STP_w_SURBSP creates B_SPLINE_SURFACE_WITH_KNOTS - OK
// - adds link to B_SPLINE_SURFACE_WITH_KNOTS into OPEN_SHELL                   BUG
// - must create boundary - eg B_SPLINE_CURVE_WITH_KNOTS (also Line -> BsplineCurve) -
//   - EDGE_CURVE from curve; ORIENTED_EDGE from EDGE_CURVE;
//   - EDGE_LOOP from the closed contour of ORIENTED_EDGE's
//   - FACE_OUTER_BOUND from EDGE_LOOP
//   - ADVANCED_FACE from FACE_OUTER_BOUND and B_SPLINE_SURFACE_WITH_KNOTS
//   - add ADVANCED_FACE into OPEN_SHELL
//     - test also with A<trimmedSur> = FSUB A<suppSur> A<suppSur>
// 
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
  DEB_dump_obj__ (Typ_SURBSP, su1, "SurBSpl");
  // DEB_dump_ox_0 (oxi, "SurBSpl");
    

  fprintf(stpw_fp,"/* Bspline-Surface %s */\n",oid);

  // define points
  ipa = stpw_li;
  pa = su1->cpTab;
  i2 = su1->ptUNr * su1->ptVNr;
  for(i1=0; i1<i2; ++i1) STP_w_PT (&pa[i1], 0, "");


  // bspline (degree,
  p1 = oid;
  iss = stpw_li;
  sprintf(s1, "#%d=B_SPLINE_SURFACE_WITH_KNOTS('%s',%d,%d,",stpw_li,
          p1, su1->degV, su1->degU);   // V then U !! ??
          // p1, su1->degU, su1->degV);   // V then U !! ??


  // add list of points
  i2 = ipa;
  STP_w_list_is (s1, sizeof(s1), i2, su1->ptUNr, "(");
  for(i1=1; i1<su1->ptVNr; ++i1) {
    i2 += su1->ptUNr;
    STP_w_list_is (s1, sizeof(s1), i2, su1->ptUNr, ",");
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
  mU = (int*)MEM_alloc_tmp ((int)(iU * sizeof(int)));
  mV = (int*)MEM_alloc_tmp ((int)(iV * sizeof(int)));
  dU = (double*)MEM_alloc_tmp ((int)(iU * sizeof(double)));
  dV = (double*)MEM_alloc_tmp ((int)(iV * sizeof(double)));

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
  STP_w_list__ (s1, sizeof(s1), mV, vNr, 0, ",");
  // output multiplicities iU
  STP_w_list__ (s1, sizeof(s1), mU, uNr, 0, ",");
  // output knots dV
  STP_w_list__ (s1, sizeof(s1), dV, vNr, 2, ",");
  // output knots dU
  STP_w_list__ (s1, sizeof(s1), dU, uNr, 2, ",");
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
  int STP_w_PLANE (Plane *pl1, int dbi) {
//================================================================
// STP_w_PLANE                        create PLANE

  int     iax, iss;
  char    s1[128], oid[40];


  APED_oid_dbo__ (oid, Typ_PLN, dbi);


  // test if axis alread exists


  // create AXIS2_PLACEMENT_3D from plane
  iax = STP_w_axis3_dbo (pl1, oid, dbi);

  sprintf(s1, "#%d=PLANE('%s',#%d)", stpw_li, oid, iax);
  fprintf(stpw_fp,"%s;\n",s1);
  iss = stpw_li;
  ++stpw_li;


  return iss;

}


//=======================================================================
  int STP_w_SURRU (ObjGX *oa, char *oid) {
//=======================================================================
// Typ_SURRU  53
// 2 objects; eg line + vector
// coverCurve: LINE|B_SPLINE_CURVE_WITH_KNOTS
// unlimited: direct into GEOMETRIC_SET
//
//   srTab   2 ObjGX-objects     
// //   mode    3=trimmed,perforated;
// //           4=unlimited
 

  int    irc, typ1, typ2, oNr1, oNr2, ivc, icv, isu, ibd;
  long   dbi1, dbi2;
  void   *obj1, *obj2;
 

  printf("STP_w_SURRU %s\n",oid);

  STP_w_log_err (
     "NOT-YET-IMPLEMENTED - %s - ruled-surf SURFACE_OF_LINEAR_EXTRUSION",oid);
  return -1;

  OGX_GET_INDEX (&typ1, &dbi1, &oa[0]);
  OGX_GET_INDEX (&typ2, &dbi2, &oa[1]);


    // TESTBLOCK
    typ1 = UTO__dbo (&obj1, &oNr1, typ1, dbi1);
    typ2 = UTO__dbo (&obj2, &oNr2, typ2, dbi2);
    DEB_dump_obj__ (typ1, obj1, "SurRU[0]=");
    DEB_dump_obj__ (typ2, obj2, "SurRU[1]=");
    // END TESTBLOCK



  //----------------------------------------------------------------
  // if typ2 is vector: SURFACE_OF_LINEAR_EXTRUSION
  if(typ2 == Typ_VC) {
    typ2 = UTO__dbo (&obj2, &oNr2, typ2, dbi2);
    // VECTOR
    ivc = STP_w_VC_d ((Vector*)obj2, "");

    if(typ1 == Typ_LN) {
      // LINE
      icv = STP_w_CRV__ (&oa[0], dbi1, "", 3);

    } else goto L_err1;

// TODO:
// get ibd = trimmed curves for boundary of ruled surf

// STP_w_CVTRM__  mode=3  // create FACE_OUTER_BOUND

// see also:
// STP_w_FACE_BOUND FACE_OUTER_BOUND + EDGE_LOOP
//   STP_w_EDGE_LOOP_out (EDGE_LOOP)


    // create SURFACE_OF_LINEAR_EXTRUSION
// TODO: get oid 
    isu = STP_w_LINEAR_EXTRUSION (ibd, icv, ivc, "");

    goto L_exit;
  }




//   //----------------------------------------------------------------
//   // 2 circles: STP_w_CONICAL_SURFACE
//   // create SurRev
//   isu = STP_w_CONICAL_SURFACE (srv, vpCen, vpCov, iRef, oid);


  goto L_err1;


  //----------------------------------------------------------------
  L_exit:
      printf(" ex-STP_w_SURRU %d\n",isu);

    return isu;


  //----------------------------------------------------------------
  L_err1:
   return -1;

}


//===================================================================
  int STP_w_LINEAR_EXTRUSION (int ibd, int icv, int ivc, char *oid) {
//===================================================================
// SURFACE_OF_LINEAR_EXTRUSION('',<curve>,<VECTOR>)
//   curve: LINE|B_SPLINE_CURVE_WITH_KNOTS
// unlimited: direct into GEOMETRIC_SET

  
  long      l1;
  char      s1[256];

 
  printf("STP_w_LINEAR_EXTRUSION %d %d %s\n",icv,ivc,oid);



  sprintf(s1,"#%d=SURFACE_OF_LINEAR_EXTRUSION('',#%d,#%d)",
          stpw_li, icv, ivc);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;


  // ADVANCED_FACE < FACE_OUTER_BOUND, supportSurf
  return STP_w_ADVANCED_FACE (&ibd, 1, oid, stpw_li - 1);

}


//================================================================
  int STP_w_SURRV (SurRev *srv, char *oid, int mode) {
//================================================================
// Typ_SURRV (54)    CONE|CYLINDER|SPHERE|TOROID
// mode   3    create x_SURFACE
//        4    create RECTANGULAR_TRIMMED_SURFACE < x_SURFACE

// typedef struct {double ang1, ang2, v0, v1;
//                 long indCen, indCov;  short typCen, typCov;
//                 unsigned dir:1;}                              SurRev;   
 

  int     i1, typCov, isu, ist, iRef, typCen, rNr;
  long    l1;
  double  d0, d1, d2, d3, d4;
  char    s1[128];
  void    *vpCov, *vpCen;


  printf("STP_w_SURRV %s %d\n",oid,mode);
  DEB_dump_obj__ (Typ_SURRV, srv, "SurRev=");


  // get refsys (plane; origin, z-vec=axis)
  typCen = UTO__dbo (&vpCen, &rNr, srv->typCen, srv->indCen);
    DEB_dump_obj__ (typCen, vpCen, "Cen=");


  // get Cov (curve rotating around axis)
  typCov = UTO__dbo (&vpCov, &rNr, srv->typCov, srv->indCov);
    DEB_dump_obj__ (typCov, vpCov, "Cov=");



  // get iRef = link for refSys from centerline
  if(srv->typCen == Typ_PLN) {
    iRef = STP_w_axis3_dbo (NULL, "", srv->indCen);

  } else {
    STP_w_log_err ("STP_w_SURRV E001 %d",srv->typCen);
    return -1;
  }


  //----------------------------------------------------------------
  if(typCov == Typ_LN) {
    // cov=LINE; CONE|CYLINDER
    STP_w_log_err (
      "BUG TODO STP_w_SURRV - revolved-surf-line -CONICAL_SURFACE - %s",oid);
    isu = STP_w_CONICAL_SURFACE (srv, vpCen, vpCov, iRef, oid);


  //----------------------------------------------------------------
  } else if(typCov == Typ_CI) {
    // cov=circle: SPHERE | TOROID
    // test if sphere or torus - is center of circle is on rev-axis
    d1 = UT3D_nlen_2ptvc (&((Plane*)vpCen)->po,
                          &((Circ*)vpCov)->pc,
                          &((Plane*)vpCen)->vz);
      printf(" w_SURRV-d1=%f\n",d1);

    if(d1 < UT_TOL_cv) {
      // sphere
      isu = STP_w_SPHERICAL_SURFACE (srv, (Circ*)vpCov, iRef, oid);

    } else {
      // torus
      STP_w_log_err (
        "NOT-YET-IMPLEMENTED - %s - revolved-surf-circ TOROIDAL_SURFACE",oid);
    }


  //----------------------------------------------------------------
  // cov=unknown type
  } else {
    STP_w_log_err ("STP_w_SURRV revolved-surf E002 %s %d",oid,srv->typCov);
    // TX_Print("STP_w_SURRV E002 %d",srv->typCov);
    return -1;
  }


  //----------------------------------------------------------------
  if(mode == 3) return (stpw_li - 1);

// TODO: ist - index support-surface for STP_w_SURSUP__

  return ist;

}

//===========================================================================
  int STP_w_CONICAL_SURFACE (SurRev *srv, Plane *pl1, Line *ln1, int iRef, char *oid) {
//===========================================================================
// TODO:
// - set the origin of axis to the centerpoint of the smaller-circle;
// - set vz into direction big circ
// - set vx into direction startPt circ
// - compute startpoint along contourelement; project onto axis
// - compute endpoint along contourelement; project onto axis
// - height = RECTANGULAR_TRIMMED_SURFACE-P6; P5 immer fix 0.
// - angle = .. must be positive
// - radius small-circle: (CONICAL_SURFACE-P3) 12.3


  int       isu, ist;
  double    d1, d2, u1, u2, v1, v2;
  char      s1[200];


  printf("STP_w_CONICAL_SURFACE %s\n",oid);


  // get d1=distance inters.point and d2=opening-angle
  STP_w_con_1 (&d1, &d2, pl1, ln1);

  // for openingAngle == 0 must use CYLINDRICAL_SURFACE
  if(fabs(d2) < UT_TOL_Ang1) {
    sprintf(s1, "#%d=CYLINDRICAL_SURFACE('%s',#%d,%lf)",stpw_li,oid,iRef,d1);

  } else {
    // #300=CONICAL_SURFACE('',#100,radius,angle)
    sprintf(s1, "#%d=CONICAL_SURFACE('%s',#%d,%lf,%lf)",stpw_li,oid,iRef,d1,d2);
  }
  fprintf(stpw_fp,"%s;\n",s1);
  isu = stpw_li;
  ++stpw_li;

  u1 = UT_DEGREES(srv->ang1);
  u2 = UT_DEGREES(srv->ang2);


  // height from v0,v1
  d1 = UT3D_len_ln (ln1);
  v1 = d1 * srv->v0;
  v2 = d1 * srv->v1;

  //----------------------------------------------------------------
  // def surf-trimmed
  ist = STP_w_RECTANGULAR_TRIMMED_SURFACE (isu, u1, u2, v1, v2);

  return ist;

}



//===========================================================================
  int STP_w_SPHERICAL_SURFACE (SurRev *srv, Circ *ci1, int iRef, char *oid) {
//===========================================================================
 
  int       isu, ist;
  double    d1, u1, u2, v1, v2;
  char      s1[128];

  // sphere
  d1 = fabs(ci1->rad);
  isu = stpw_li;
  sprintf(s1, "#%d=SPHERICAL_SURFACE('%s',#%d,%lf)",stpw_li,oid,iRef,d1);

  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;

  // def surf-trimmed
  u1 = UT_DEGREES(srv->ang1);   // angle
  u2 = UT_DEGREES(srv->ang2);   // angle
  v1 = 90. - (srv->v0 * 180.);  // angle  below centerplane; -90 - 0
  v2 = 90. - (srv->v1 * 180.);  // angle  above centerplane; 0 - 90


  ist = STP_w_RECTANGULAR_TRIMMED_SURFACE (isu, u1, u2, v1, v2);


  return ist;

}


//================================================================
  int STP_w_RECTANGULAR_TRIMMED_SURFACE (int isu, double u1, double u2,
                                         double v1, double v2) {
//================================================================
// RECTANGULAR_TRIMMED_SURFACE('',#300, 180., 360.0, 0.0, 50.0,.T.,.T.);
// #401=RECTANGULAR_TRIMMED_SURFACE
//   u1            : parameter_value;
//   u2            : parameter_value;
//   v1            : parameter_value;
//   v2            : parameter_value;
//   usense        : BOOLEAN;
//   vsense        : BOOLEAN;

  char      s1[256];
  long      l1;

  sprintf(s1,"#%d=RECTANGULAR_TRIMMED_SURFACE('',#%d,%lf,%lf,%lf,%lf,.T.,.T.)",
          stpw_li, isu, u1, u2, v1, v2);
  fprintf(stpw_fp,"%s;\n",s1);
  MemTab_add (&ol_GS, &l1, &stpw_li, 1, 0);
  ++stpw_li;


  return (stpw_li - 1);

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

    DEB_dump_obj__ (Typ_PLN, pl1, "w_con_1-pl1");
    DEB_dump_obj__ (Typ_VC, &vcl, "vcl");
    DEB_dump_obj__ (Typ_VC, &pl1->vz, "vz");

  
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
    
    
      
// see Tess_bound_SURPLN TSU_DrawSurTP1 SUP_addBound
      
  int       i1, ii, ptNr, ia[5], iel, iss, ipvx, ipvz, ipc;
  char      s1[128], *p1;
  Point     pa[5], *pTab;
  Line      ln1;
  Vector    vcn, vx;
    
    
  printf("STP_w_SURCIR %s\n",oid);
  DEB_dump_ox_0 (oxi, "S-planar");


  // init obj_act.
  STP_w_objAct_ini ();


  // get all points of RCIR > pTab
  ptNr = 0;
  i1 = UT3D_cv_scir__ (&ptNr, pa, 5, oxi, 2);
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
    ii = STP_w_LN (&ln1, 0, "", 3);
    ln1.p1 = ln1.p2;
    // add to list
    ia[i1-1] = ii;
  }

  // fprintf(stpw_fp,"/* %s outer-boundary */\n",oid);
  obj_act.stat = 0;
  iel = STP_w_EDGE_LOOP_out (ia, ptNr-1); //, oid);

  if(!obj_act.sro) p1 = stpwTrue;
  else         p1 = stpwFalse;
  sprintf(s1,"#%d=FACE_OUTER_BOUND('',#%d,%s)",stpw_li, iel, p1);
  fprintf(stpw_fp,"%s;\n",s1);
  iel =stpw_li;
  ++stpw_li;

  // get normalvector for polygon; sr always CCW
  UT3D_vc_perppta (&vcn, ptNr, pa);

  // create origin+axis as AXIS2_PLACEMENT_3D
  STP_w_objAct_set (&pa[0], -1, &vcn, -1, 0);
  iss = STP_w_axis3_vz ();

  // supporting surface PLANE
  sprintf(s1, "#%d=PLANE('',#%d)", stpw_li, iss);
  fprintf(stpw_fp,"%s;\n",s1);
  iss = stpw_li;
  ++stpw_li;



  // ADVANCED_FACE < FACE_OUTER_BOUND, FACE_BOUNDs, supportSurf
  return STP_w_ADVANCED_FACE (&iel, 1, "", iss);

}


//================================================================
  int STP_w_SURTP__ (ObjGX *oxi, char *oid) {
//================================================================
// trimmed, perforated surface.
// ADVANCED FACE ((outerBoundary/InnerBoundary), supportSurf)



// see Tess_bound_SURPLN TSU_DrawSurTP1 SUP_addBound

  int     irc, ssTyp, i1, i2, i3, io, pNr, ebeg, cnAct, isu, ibNr, *iba, iss;
  char    s1[128];
  ObjGX   *oa, *ob;
  void    *o1, *ssDat;
  Point   *pa;


  printf("STP_w_SURTP__ %s\n",oid);
  DEB_dump_ox__ (oxi, "SURTP__\n");

  // ID's of boundaries
  iba = (int*)MEM_alloc_tmp ((int)(oxi->siz * sizeof(int)));

  // init obj_act.
  STP_w_objAct_ini ();


  oa = oxi->data;


  //----------------------------------------------------------------
  // creat outer and inner bounds

  io = 1;      // record 1 = outer bound
  cnAct = 1;
  ob = &oa[io];
    DEB_dump_ox__ (ob, "outer-boundary[%d]",io);

  // create outer boundary as EDGE_LOOP
  // FACE_OUTER_BOUND < outerBoundary ob
  obj_act.stat = 0;
  // iba[0] = STP_w_BOUND (ob, oid, 3);
  iba[0] = STP_w_CRV__ (ob, 0L, oid, 3);
  ibNr = 1;


  // all following records are inner-boundaries
  L_ib_nxt:
  ++io;
  ++cnAct;
  if(io >= oxi->siz) goto L_w1;
  ob = &oa[io];
      // DEB_dump_ox_0 (ob, "inner-bound[%d]",io);

    // create inner boundaries as EDGE_LOOP
    // FACE_BOUND from innerBoundary ob
    obj_act.stat = 1;
    // iba[ibNr] = STP_w_BOUND (ob, oid, 4);
    iba[ibNr] = STP_w_CRV__ (ob, 0L, oid, 4);
    ++ibNr;
    goto L_ib_nxt;


  //----------------------------------------------------------------
  L_w1:

  // create support-surface from record # 0
  iss = STP_w_SURSUP__ (&oa[0], oid);
  if(iss < 0) return -1;

  // ADVANCED_FACE < FACE_OUTER_BOUND, FACE_BOUNDs, supportSurf
  isu = STP_w_ADVANCED_FACE (iba, ibNr, oid, iss);

  return isu;

}


//================================================================
  int STP_w_SURPLN (ObjGX *oss) {
//================================================================
// STP_w_SURPLN                 get plane for planar surface

  int     irc;
  Plane   pl1;

  
  DEB_dump_ox__ (oss, "STP_w_SURPLN");


  // get plane for: Circ, ellipse, polygon
  irc = UT3D_pl_obj (&pl1, obj_act.typ, obj_act.odc, 1);
  if(irc < 0) return irc;


  // Store plane in obj_act with ori, vx, vz.
  obj_act.po = pl1.po;
  obj_act.vx = pl1.vx;
  obj_act.vz = pl1.vz;

  return 0;

}


//================================================================
  int STP_w_ADVANCED_FACE (int *ia, int iNr, char *oid, int iss) {
//================================================================
// ADVANCED_FACE < FACE_OUTER_BOUND, FACE_BOUNDs, supportSurf
// ADVANCED_FACE ('oid', (outerBound[,innerBounds]), supportSurf, T|F)
// Input:
//   ia       list of bounds
//   iNr      recNr of ia
//   oid      faceID
//   iss      supportSurf
//
// TODO: orientation always .T. ?

  int    i1;
  char   s1[128], *p1;

  printf("STP_w_ADVANCED_FACE iNr=%d iss=%d\n",iNr,iss);
  for(i1=0;i1<iNr;++i1) printf("ADVANCED_FACE[%d]=%d\n",i1,ia[i1]);



// TODO ?
  p1 = stpwTrue;

  sprintf(s1,"#%d=ADVANCED_FACE('%s'",stpw_li,oid);
  ++stpw_li;
  STP_w_list__ (s1, sizeof(s1), ia, iNr, 1, ",");
  fprintf(stpw_fp,"%s,#%d,%s);\n",s1,iss,p1);


  return (stpw_li - 1);

}


//================================================================
  int STP_w_oDBpt_add (int *dbi, int *iPt) {
//================================================================
// STP_w_oDBpt_add                add point into point-DB
 
  long    l1;
  stp_pt  opt1;

  opt1.dbi = *dbi;
  opt1.iPt = *iPt;
  return MemTab_add (&oDB_pt, &l1, &opt1, 1, 0);

}


//================================================================
  int STP_w_oDBpt_ck (int dbi) {
//================================================================
// STP_w_oDBpt_ck              check if point is already defined

  int     i1, oNr, *ipa, iStp;
  stp_pt  *opt1, *opta;


  printf("STP_w_oDBpt_ck %d\n",dbi);

  // loop tru oDB_cv
  oNr = MEMTAB_IND (&oDB_pt); // get nr
  opta = MEMTAB_DAT (&oDB_pt);
  for(i1=0; i1<oNr; ++i1) {
    opt1 = &opta[i1];
    if(opt1->dbi != dbi) continue;
    iStp = opt1->iPt;
    goto L_exit;
  }

  iStp = 0;

  L_exit:

    printf(" ex-STP_w_oDBpt_ck %d\n",iStp);

  return iStp;

}



//===================================================================
  int STP_w_oDBcv_add (int typ, int dbi, int iCv, int iP1, int iP2) {
//===================================================================
// STP_w_oDBcv_add          store step-index of curve

  long   l1;
  stp_cv    odb1;


  printf("STP_w_oDBcv_add %d %d cv=%d p1=%d p2=%d\n",typ,dbi,
         iCv,obj_act.iP1,obj_act.iP2);


  odb1.typ  = AP_typDB_typ (typ);
  odb1.dbi  = dbi;
  odb1.iCv  = iCv;             // link of curve (eg CIRCLE)
  odb1.iP1  = iP1;             // link startPt
  odb1.iP2  = iP2;             // link endPt

  MemTab_add (&oDB_cv, &l1, &odb1, 1, 0);

  return 0;

}


//================================================================
  int STP_w_oDBcv_ck__ (int typ, int dbi) {
//================================================================
// STP_w_oDBcv_ck__   check if curve is already in oDB_cv
// retCode:   0 = obj not yet created; else stepID of obj

  int    i1, oNr, iStp;
  stp_cv *odb1, *odba;


  printf("STP_w_oDBcv_ck__ %d %d\n",typ,dbi);

  // loop tru oDB_cv
  // ii = 0;
  oNr = MEMTAB_IND (&oDB_cv); // get nr
  odba = MEMTAB_DAT (&oDB_cv);
  for(i1=0; i1<oNr; ++i1) {
    odb1 = &odba[i1];
      STP_w_oDBcv_dump (odb1, i1);
    if(odb1->typ != typ) continue;
    if(odb1->dbi != dbi) continue;
    iStp = odb1->iCv;
    goto L_exit;
  }

  iStp = 0;

  L_exit:

    printf(" ex-STP_w_oDBcv_ck__ %d\n",iStp);


  return iStp;

}


//================================================================
  stp_cv* STP_w_oDBcv_ck_o (int typ, int dbi) {
//================================================================
// STP_w_oDBcv_ck_ox   check if curve is already in oDB_cv
// retCode:   0 = obj not yet created; else stepID of obj

  int    i1, oNr;
  stp_cv *odb1, *odba;


  // printf("STP_w_oDBcv_ck_o %d %d\n",typ,dbi);


  // loop tru oDB_cv
  oNr = MEMTAB_IND (&oDB_cv); // get nr
  odba = MEMTAB_DAT (&oDB_cv);
  for(i1=0; i1<oNr; ++i1) {
    odb1 = &odba[i1];
      STP_w_oDBcv_dump (odb1, i1);
    if(odb1->typ != typ) continue;
    if(odb1->dbi != dbi) continue;
    goto L_exit;
  }

  odb1 = NULL;

  L_exit:

    if(odb1) printf(" ex-STP_w_oDBcv_ck_o %d %d %d cv=%d p1=%d p2=%d\n",
                    typ,dbi,i1,odb1->iCv,odb1->iP1,odb1->iP2);
    else     printf(" ex-STP_w_oDBcv_ck_o %d %d NULL\n",typ,dbi);


  return odb1;

}


//================================================================
  int STP_w_oDBcv_dump (stp_cv *odb1, int ii) {
//================================================================
 
  if(odb1) {
    printf(" STP_w_oDBcv_dump[%d] %d %d %d %d %d\n",ii,
      odb1->typ,odb1->dbi,odb1->iCv,odb1->iP1,odb1->iP2);

  } else {
    printf(" STP_w_oDBcv_dump[%d] stp_cv is NULL\n",ii);
  }

  return 0;

}


/*
//================================================================
  int STP_w_EDGE_LOOP__ (ObjGX *oxi, char *oid) {
//================================================================
// create outer boundary as EDGE_LOOP (closed contour).
// obj_act.stat 0  outer-boundary   FACE_OUTER_BOUND
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

  int     i1, i2, ii, iob, typ, ip1, ip2, ipc, *ia, oNr, iNr, iStp;
  char    s1[128], *p1;
  void    *odc, *ob1;
  Point   actPos;
  ObjGX   *oxa, *ox1;
  stp_cv  *odb1;


  printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE \n");
  DEB_dump_ox__ (oxi, "STP_w_EDGE_LOOP__ stat=%d\n",obj_act.stat);
  DEB_dump_ox_0 (oxi, "STP_w_EDGE_LOOP__ stat=%d\n",obj_act.stat);

  // get contour-data (resolv links)
  typ = UTO_objDat_ox (&odc, &oNr, oxi); 
    DEB_dump_obj__ (typ, odc, "EDGE_LOOP__-L1");


  // make boundary from 1-n trimmed-curves
  if(typ == Typ_CVTRM) {
    // recurse
    ia = (int*) MEM_alloc_tmp (oNr * sizeof(int));
    for(i1=0; i1<oNr; ++i1) {
      ob1 = CVTRM_set (odc, i1);
      iNr = STP_w_CVTRM__ (&ia[i1], ob1, oid, 3);
    }
STP_w_ERREX ("STP_w_SURTP__-L1");
    goto L_out_2;
  }


  //----------------------------------------------------------------
  // make boundary from single-closed-curve - only basCurve

  // keep typ, data:
  obj_act.typ = typ;
  obj_act.odc = odc;


  // more than one trimmed-curves or single-closed-curve
STP_w_ERREX ("STP_w_EDGE_LOOP__-L2");


  // typ = oxi->typ;
  ia = &ipc;  // only a single closed curve
  iNr = 1;

  // check if curve is already in oDB_cv
  odb1 = STP_w_oDBcv_ck_ox (oxi);
  if(odb1) {
    // out trimPoints, EDGE_CURVE, ORIENTED_EDGE
    ipc = STP_w_ORIENTED_EDGE (odb1->iP1, odb1->iP2, odb1->iCv, 0);
    goto L_out_2; // curve already exists
  }


  switch(typ) {

//     //----------------------------------------------------------------
//     case Typ_ObjGX:           // CCV
//       // DEB_dump_ox_0 (odc, " CCV:");
//       ia = (int*)MEM_alloc_tmp ((int)(((ObjGX*)odc)->siz * sizeof(int)));
//       iNr = STP_w_CVCCV (ia, odc, "", 3);
//       break;
// 
// 
    //----------------------------------------------------------------
    case Typ_CVELL:  // must be closed !
      iob = STP_w_EL__ ((CurvElli*)odc, "");
      ip1 = STP_w_PT (&((CurvElli*)odc)->p1, 0, "");
      // ORIENTED_EDGE < EDGE_CURVE < outerBoundary
      ipc = STP_w_ORIENTED_EDGE (ip1, ip1, iob, 0);
      break;


    //----------------------------------------------------------------
    case Typ_CI:  // must be closed !
      // ORIENTED_EDGE < EDGE_CURVE < outerBoundary
      ipc = STP_w_AC_CC ((Circ*)odc, "", 3);
      break;


    //----------------------------------------------------------------
    case Typ_CVPOL:
        // DEB_dump_obj__ (Typ_CVPOL, odc, " PLG:");
      // ORIENTED_EDGE < EDGE_CURVE < outerBoundary
      ipc = STP_w_CVPOL ((CurvBSpl*)odc, "", 3);
      break;


    //----------------------------------------------------------------
    case Typ_CVBSP:
      // ORIENTED_EDGE < EDGE_CURVE < outerBoundary
      ipc = STP_w_CVBSP (&actPos, (CurvBSpl*)odc, "", 3);
      break;

    //----------------------------------------------------------------
    default:
      printf("******* STP_w_EDGE_LOOP__ unsupported: %d\n",typ);
      return -1;
  }

  

  L_out_2:

  if(obj_act.stat == 0) {   // outer-boundary
    fprintf(stpw_fp,"// outer-boundary for %s//\n",oid);
  } else {          // inner-boundary
    fprintf(stpw_fp,"// inner-boundary for %s //\n",oid);
  }


  // EDGE_LOOP < ORIENTED_EDGE
  ipc = STP_w_EDGE_LOOP_out (ia, iNr);


  // create FACE_OUTER_BOUND from EDGE_LOOP ipc
  // set orientation T|F
  if(obj_act.stat == 0) {   // outer-boundary; CCW
    if(!obj_act.sro) p1 = stpwTrue;
    else          p1 = stpwFalse;
    sprintf(s1,"#%d=FACE_OUTER_BOUND('',#%d,%s)",stpw_li, ipc, p1);
  } else {          // inner-boundary; CW
    if(obj_act.sro)  p1 = stpwTrue;   // should use obj_act.sri
    else          p1 = stpwFalse;
    sprintf(s1,"#%d=FACE_BOUND('',#%d,%s)",stpw_li, ipc, p1);
  }
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;


  return (stpw_li - 1);

}
*/

//================================================================
  int STP_w_ORIENTED_EDGE (int ip1, int ip2, int ipc, int iDir) {
//================================================================
// Input:
//   iDir   0=True(defined direction)
//          1=False(inverted direction)
// Output:
//   retCode     stepIndex of COMPOSITE_CURVE_SEGMENT|ORIENTED_EDGE
//
// create EDGE_CURVE from 2 Trimpoints, 1 BaseCurve
// EDGE_CURVE (name, Trimpoint1, Trimpoint2, BaseCurve, T|F)
//   T|F same_sense; 
//
// ORIENTED_EDGE (name,*,*,1 EDGE_CURVE, T|F))
//   T|F orientation; always .T:; already set in EDGE_CURVE.

  int     iob, typ, iv1, iv2;
  char    s1[128], *p1;


  printf("STP_w_ORIENTED_EDGE %d %d %d %d\n",ip1,ip2,ipc,iDir);


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
              // stpw_li,           iv1, iv2, ipc, p1);
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
  int STP_w_EDGE_LOOP_out (int *ia, int iNr) {
//================================================================
// EDGE_LOOP < ORIENTED_EDGEs


  char    s1[128];


  sprintf(s1,"#%d=EDGE_LOOP(''",stpw_li);
  ++stpw_li;
  STP_w_list__ (s1, sizeof(s1), ia, iNr, 1, ",");
  fprintf(stpw_fp,"%s);\n",s1);

  return (stpw_li - 1);

}

/*
//==========================================================================
  int STP_w_wf_wo (ObjGX *ox1, char *oid, int mode) {
//==========================================================================
// write wireframe-object
// mode 2 GEOMETRIC_SET < COMPOSITE_CURVE < COMPOSITE_CURVE_SEGMENT
 

  int     typ, form, ii, *ia;
  long    l1;
  void    *data;
  Point   actPos;


  printf("STP_w_wf_wo typ=%d form=%d\n",ox1->typ,ox1->form);
  typ  = ox1->typ;
  form = ox1->form;
  data = ox1->data;
    printf(" typ=%d form=%d\n",typ,form);



  switch(form) {

    //----------------------------------------------------------------
    case Typ_PT:
      ii = STP_w_PT ((Point*)ox1->data, 0, oid);
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
      ia = (int*)MEM_alloc_tmp ((int)(((ObjGX*)ox1)->siz * sizeof(int)));
      ii = STP_w_CVCCV (NULL, ia, ox1, oid, 2); // makes COMPOSITE_CURVE !
      goto L_exit;



    //----------------------------------------------------------------
    default:
      printf("***** STP_w_wf_wo unsupported: %d\n",typ);
      return -1;
  }

  
// //
//   // add point|curve to active-list of curves
//   if(mode == 1) {
//     // GEOMETRIC_CURVE_SET
//     if(ii > 0) MemTab_add (&ol_GS, &l1, &ii, 1, 0);
//     return 0;
//   }
// //

  // mode=2: 
  // write COMPOSITE_CURVE < from single obj ii
  ii = STP_w_COMPOSITE_CURVE (&ii, 1, oid);


  // add point|curve ii to GEOMETRIC_SET
  L_exit:
  if(ii > 0) MemTab_add (&ol_GS, &l1, &ii, 1, 0);

  return 0;

}
*/
 
//================================================================
  int STP_w_PT (Point *pt1, int dbi, char *oid) {
//================================================================
// #288=CARTESIAN_POINT('',(-1314.598162,-89.562827,0.000000));
// VERTEX for TRIMMED_CURVE workes for all OpenCascade-products
//   but not for IDA-STEP
 
  int     iPt;
  char    s1[128];

  // DEB_dump_obj__ (Typ_PT, pt1, "STP_w_PT %d",dbi);
  // if(strlen(oid)) fprintf(stpw_fp,"/* Point %s */\n",oid);

  if(dbi) {
    iPt = STP_w_oDBpt_ck (dbi);
    if(iPt) goto L_exit;
  }

  sprintf(s1, "#%d=CARTESIAN_POINT('%s',(%lf,%lf,%lf))",
              stpw_li, oid, pt1->x, pt1->y, pt1->z);
  fprintf(stpw_fp,"%s;\n",s1);
  iPt = stpw_li;
  ++stpw_li;


  if(dbi) {
    STP_w_oDBpt_add (&dbi, &iPt);
  }

  L_exit:
    printf("ex-STP_w_PT dbi=%d iPt=%d\n",dbi,iPt);

  return iPt;

}


//================================================================
  int STP_w_VC_d (Vector *vc1, char *oid) {
//================================================================
// #38=DIRECTION('',(1.,0.,0.));
// #37=VECTOR('',#38,1.);
  
  // int     ip1;
  char    s1[128];
  
  DEB_dump_obj__ (Typ_VC, vc1, "STP_w_VC_d");


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

  DEB_dump_obj__ (Typ_VC, vc1, "STP_w_VCdv");


  STP_w_VC_d (vc1, oid); // DIRECTION

  sprintf(s1, "#%d=VECTOR('%s',#%d,1.)",
              stpw_li, oid, stpw_li - 1);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;

  return (stpw_li - 1);

}


//================================================================
  int STP_w_axis3_vz () {
//================================================================
// create AXIS2_PLACEMENT_3D from Z-vector
// if ipc>0 then this centerpoint is to be used; else get workPlaneOrigin.
// Input:
//   obj_act.po   origin
//   obj_act.vz   z-vector
//   obj_act.po   origin


  Vector   vx;


  //----------------------------------------------------------------
  // origin at obj_act.po;
  // if ipo==0 then origin is not mandatory;
  //   test distance obj_act.po - workPlane-origin along vz;
  //     if d1 < tol then use active workPlane-origin
  if(obj_act.ipo < 1) {
    // test distance of po along vz; if 
// TODO: get origin of workPlane
    // obj_act.ipo = 101;     
    obj_act.ipo = STP_w_PT (&obj_act.po, 0, "");
  }





  //----------------------------------------------------------------
  // create z-axis
// TODO: if(obj_act.ivz < 1) {  // get active Z-axis
  obj_act.ivz = STP_w_VC_d (&obj_act.vz, ""); // DIRECTION

  // create x-axis
  if(obj_act.ivx < 1) {
    UT3D_vc_perpvcplXY (&vx, &obj_act.vz);
    obj_act.ivx = STP_w_VC_d (&vx, "");           // DIRECTION
  }


  //----------------------------------------------------------------
  // create axis (AXIS2_PLACEMENT_3D(centerpoint, Z-axis, x-axis))
  return STP_w_axis3__ (obj_act.ipo, obj_act.ivz, obj_act.ivx, "");

}
  

//================================================================
  int STP_w_axis3_dbo (void *vp, char *oid, long dbi) {
//================================================================
// create AXIS2_PLACEMENT_3D from database-object
// Input:
//   vp        Plane or NULL
//   dbi       DB-index of plane

  int    ipc, ipvz, ipvx, ipx;


  printf("STP_w_axis3_dbo %s %ld\n",oid,dbi);

  if(!vp) {
    vp = DB_get_PLN (dbi);
  }

  if(dbi) {
    ipx = STP_w_oDBcv_ck__ (Typ_PLN, dbi);
    if(ipx) goto L_exit;
  }

  ipc = STP_w_PT (&((Plane*)vp)->po, 0, "");

  // create z-axis
  ipvz = STP_w_VC_d (&((Plane*)vp)->vz, ""); // DIRECTION
  
  // create x-axis
  ipvx = STP_w_VC_d (&((Plane*)vp)->vx, ""); // DIRECTION

  // create axis for circ (AXIS2_PLACEMENT_3D(centerpoint, Z-axis, x-axis))
  ipx = STP_w_axis3__ (ipc, ipvz, ipvx, oid);

  if(dbi) {
    obj_act.iCv = ipx;
    STP_w_oDBcv_add (Typ_PLN, dbi, ipx, ipvz, ipvx);
  }


  L_exit:

  return ipx;


}

 
//================================================================
  int STP_w_axis3__ (int ipc, int ipvz, int ipvx, char *oid) {
//================================================================
// #100=AXIS2_PLACEMENT_3D('',#101,#102,#103);  // origin,z-vector,x-vector
// Input:
//   ipc     link origin
//   ipvz    link z-axis
//   ipvx    link x-axis
//   oid     objID
// returns link of axis

  char    s1[128];

  sprintf(s1, "#%d=AXIS2_PLACEMENT_3D('%s',#%d,#%d,#%d)",
              stpw_li, oid, ipc,ipvz,ipvx);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;

  return (stpw_li - 1);

}


//===============================================================================
  int STP_w_SURBND__ (int icv, int ip1, int ip2, int iDir, char *oid, int mode) {
//===============================================================================
// STP_w_SURBND__        STP_w_ORIENTED_EDGE + STP_w_FACE_BOUND
// Input:
//   iDir      0=True(defined direction)
//             1=False(inverted direction)// Output:
//   mode      3=FACE_OUTER_BOUND; 4=FACE_BOUND
//   retCode   stepIndex of FACE_[OUTER_]BOUND

  int   iEdg, iBnd;

  // EDGE_CURVE + ORIENTED_EDGE
  iEdg =  STP_w_ORIENTED_EDGE (ip1, ip2, icv, iDir);

  // EDGE_LOOP + FACE_[OUTER_]BOUND 
  iBnd = STP_w_FACE_BOUND (&iEdg, 1, oid, mode);

    printf(" ex-STP_w_SURBND__ %d\n",iBnd);

  return iBnd;

}


//================================================================
  int STP_w_LN (Line *ln1, int dbi, char *oid, int mode) {
//================================================================
// Line
// mode=1 create LINE - baseCurve only    
//      2 create COMPOSITE_CURVE_SEGMENT                 wireframe
//      3 create ORIENTED_EDGE < EDGE_CURVE < LINE       shell
// Output:
//   retCode     stepIndex of COMPOSITE_CURVE_SEGMENT

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
  stp_cv  *dbCv;


  // skip lines with length < UT_TOL_cv;  (comes from CCV)
  if(UT3D_len_ln (ln1) < UT_TOL_cv) return 0;

  printf("STP_w_LN %s dbi=%d mode=%d\n",oid,dbi,mode);
  fprintf(stpw_fp,"/* Line %s */\n",oid);

  // check if curve already exists
  if(dbi) {
    dbCv = STP_w_oDBcv_ck_o (Typ_LN, dbi);
    if(dbCv) {
      iln = dbCv->iCv;
      ip1 = dbCv->iP1;
      ip2 = dbCv->iP2;
      goto L_nxt;
    }
  }

  ip1 = STP_w_PT (&ln1->p1, 0, "");
  obj_act.iP1 = ip1;

  // if(mode > 1)
  ip2 = STP_w_PT (&ln1->p2, 0, "");
  obj_act.iP2 = ip2;

  UT3D_vc_ln (&vc1, ln1);
  ivc = STP_w_VCdv (&vc1, "");      // VECTOR, DIRECTION


  iln = stpw_li;
  sprintf(s1, "#%d=LINE('',#%d,#%d)", stpw_li, ip1, ivc);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;


  if(!dbCv) {
    // store basicCurve in DB
    STP_w_oDBcv_add (Typ_LN, dbi, iln, ip1, ip2);
  }


  L_nxt:
    printf(" STP_w_LN iln=%d ip1=%d ip2=%d\n",iln,ip1,ip2);


  //----------------------------------------------------------------
  // mode=1: 
  if(mode == 1) {
    // obj_act.iCv = iln;
    // obj_act.iP1 = ip1;
    // obj_act.iP2 = ip2;
    return (iln);
    // return STP_w_CURVE_SET (&iln, 1);
  }


  //----------------------------------------------------------------
  // mode=3: ORIENTED_EDGE < EDGE_CURVE
  if(mode >= 3) {
    // EDGE_CURVE and ORIENTED_EDGE from baseCurve & 2 points
    return STP_w_ORIENTED_EDGE (ip1, ip2, iln, 0);
  }


  //----------------------------------------------------------------
  // mode=2  COMPOSITE_CURVE_SEGMENT < TRIMMED_CURVE
  return STP_w_CRV_wf (iln, ip1, ip2, 0, 1, oid);  // 0=fwd; 1=open
  // iln = STP_w_TRIMMED_CURVE__ (iln, ip1, ip2, stpwTrue);
  // return STP_w_CCV_SEG (iln, 1, oid);

}


//================================================================
  int STP_w_CIRCLE (Circ *ac1, char *oid) {
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


  // printf("STP_w_CIRCLE %s\n",oid);
  DEB_dump_obj__ (Typ_CI, ac1, "STP_w_CIRCLE %s",oid);
  // if(oid[0]) fprintf(stpw_fp,"/* Circle %s */\n",oid);


  d1 = fabs(ac1->rad);

  // centerpoint, startpoint, endpoint
  ipc = STP_w_PT (&ac1->pc, 0, "");

  sr = UT3D_sr_ci (ac1);  // 0=CCW; 1=CW.

  vz = ac1->vz;
  if(sr) UT3D_vc_invert (&vz, &vz);  // inverted Z-vector

  STP_w_objAct_set (&ac1->pc, ipc, &vz, -1, sr);

  // create origin+axis as AXIS2_PLACEMENT_3D
  ipx = STP_w_axis3_vz ();

  // #43=CIRCLE('',#100,25.0);                  // axis, radius
  ici = stpw_li;
  sprintf(s1, "#%d=CIRCLE('%s',#%d,%lf)", stpw_li, oid, ipx, d1);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;

  return ici;

}


//================================================================
  int STP_w_AC_CC (Circ *ac1, int dbi, char *oid, int mode) {
//================================================================
// mode=1 create CIRCLE - baseCurve only
//      2 create COMPOSITE_CURVE_SEGMENT from circ
//      3 create ORIENTED_EDGE < EDGE_CURVE < circ
// Output:
//   retCode     stepIndex of COMPOSITE_CURVE_SEGMENT

// mode==1
// #43=CIRCLE('',#100,25.0);                  // axis, radius
// #42=TRIMMED_CURVE('',#43,(#44),(#45),.T.,.CARTESIAN.);
// #43=GEOMETRIC_CURVE_SET('',(#42));

// mode==2
// #43=CIRCLE('',#100,25.0);                  // axis, radius
// #42=TRIMMED_CURVE('',#43,(#44),(#45),.T.,.CARTESIAN.);
// #41=COMPOSITE_CURVE_SEGMENT(.CONTINUOUS.,.T.,#42);

// {Point p1, p2, pc; Vector vz; double rad, ango;} Circ;


  int     i1, ipc, ip1, ip2, ipvz, ipvx, ipx, ici, dir, i360, iStp;
  double  d1;
  char    s1[128], *p1;
  Vector  vcn, vx;
  Point   pts, pte;
  stp_cv  *dbCv;


  // ck 360-deg
  i360 = UT3D_ck_ci360 (ac1);   // o=yes,360; 1=trimmed
    printf(" i360=%d\n",i360);
  // if(!i360) p1 = oid;
  // else      p1 = stptStrNull;

  printf("STP_w_AC_CC %s %d %d\n",oid,dbi,mode);
  // if(oid[0]) 
  fprintf(stpw_fp,"/* Circle %s */\n",oid);

  // check if curve already exists
  if(dbi) {
    dbCv = STP_w_oDBcv_ck_o (Typ_CI, dbi);
    if(dbCv) {
      ici = dbCv->iCv;
      ip1 = dbCv->iP1;
      ip2 = dbCv->iP2;
      goto L_nxt;
    }
  }

  // create Circ-unlimited
  ici = STP_w_CIRCLE (ac1, oid); // p1);


  L_nxt:
    printf(" w_AC_CC ici=%d ip1=%d ip2=%d\n",ici,ip1,ip2);


  //----------------------------------------------------------------
  // mode=1: ORIENTED_EDGE < EDGE_CURVE
  if(mode == 1) return ici;


  //----------------------------------------------------------------
  // do not trim 360-deg-circ 
  if(i360) {
    // not 360: creat startPt/endPt
    ip1 = STP_w_PT (&ac1->p1, 0, "");
    ip2 = STP_w_PT (&ac1->p2, 0, "");
  } else {
    // 360 deg
    ip1 = STP_w_PT (&ac1->p1, 0, "");
    ip2 = ip1;
  }


  if(!dbCv) {
    // store basicCurve in DB
    STP_w_oDBcv_add (Typ_CI, dbi, ici, ip1, ip2);
  }

  
  //----------------------------------------------------------------
  // mode=3,4: FACE_[OUTER_]BOUND < EDGE_LOOP < ORIENTED_EDGE < EDGE_CURVE
  if(mode >= 3) {
    iStp = STP_w_SURBND__ (ici, ip1, ip2, 0, oid, mode);
//     // EDGE_CURVE and ORIENTED_EDGE from baseCurve & 2 points
//     i1 = STP_w_ORIENTED_EDGE (ip1, ip2, ici, 0);
//     // EDGE_LOOP FACE_[OUTER_]BOUND 
//     iStp = STP_w_FACE_BOUND (&i1, 1, oid, mode);
      printf(" ex-STP_w_AC_CC %d\n",iStp);
    return iStp;
  } 


  //----------------------------------------------------------------
  // mode=2
  // fwd-bwd already fixed by inverting z-axis for basic-circle
  dir = 0;
  // if(ac1->rad > 0) dir = 0;  // fwd
  // else             dir = 1;  // bwd

  // create wirefame-curve (COMPOSITE_CURVE_SEGMENT < TRIMMED_CURVE)
  return STP_w_CRV_wf (ici, ip1, ip2, dir, i360, oid);

}


//================================================================
  int STP_w_EL_CC (CurvElli *el1, int dbi, char *oid, int mode) { 
//================================================================
// mode=1 create pure geometric obj, not trimmed
//      2 create COMPOSITE_CURVE_SEGMENT from ellipse    (WF)
//      3 create ORIENTED_EDGE < EDGE_CURVE < elli
// Output:
//   retCode     stepIndex of COMPOSITE_CURVE_SEGMENT


  int     i1, ip1, ip2, ici, i360;
  char    s1[128], *p1;
  Vector  vx;
  Point   pts, pte;
  stp_cv  *dbCv = NULL;


  DEB_dump_obj__(Typ_CVELL, el1, "STP_w_EL_CC-in %d %s %d",dbi,oid,mode);

  // ck 360-deg
  i360 = UT3D_ck_el360 (el1);
    printf(" i360=%d\n",i360);
  if(!i360) p1 = oid;
  else p1 = stptStrNull;

    printf("STP_w_EL_CC %s %d\n",oid,mode);
  fprintf(stpw_fp,"/* Ellipse %s */\n",oid);

  // check if curve already exists
  if(dbi) {
    dbCv = STP_w_oDBcv_ck_o (Typ_CV, dbi);
    if(dbCv) {
      ici = dbCv->iCv;
      ip1 = dbCv->iP1;
      ip2 = dbCv->iP2;
      goto L_nxt;
    }
  }

  // create Circ-unlimited
  ici = STP_w_EL__ (el1, p1);

  // // do not trim 360-deg-circ 
  // if(!i360) goto L_CS;


  L_nxt:
    printf(" w_EL_CC  ici=%d ip1=%d ip2=%d\n",ici,ip1,ip2);

  //----------------------------------------------------------------
  // pure geometric obj, not trimmed
  if(mode == 1) return ici;
    // return STP_w_CURVE_SET (&ici, 1); // CCW


  //----------------------------------------------------------------
  // creat startPt/endPt
  ip1 = STP_w_PT (&el1->p1, 0, "");
  ip2 = STP_w_PT (&el1->p2, 0, "");


  if(!dbCv) {
    // store basicCurve in DB
    STP_w_oDBcv_add (Typ_CV, dbi, ici, ip1, ip2);
  }


  //----------------------------------------------------------------
  // mode=3,4: FACE_[OUTER_]BOUND < EDGE_LOOP < ORIENTED_EDGE < EDGE_CURVE
  if(mode >= 3) {
    // EDGE_CURVE and ORIENTED_EDGE from baseCurve & 2 points
    i1 = el1->srot; // 0=CCW, 1=CW.
    return STP_w_SURBND__ (ici, ip1, ip2, i1, oid, mode);
//     return STP_w_ORIENTED_EDGE (ip1, ip2, ici, i1);
  }


  //----------------------------------------------------------------
  // mode=2
  // if(el1->srot)  dir = stpwFalse;
  // else           dir = stpwTrue;

  // create wirefame-curve (COMPOSITE_CURVE_SEGMENT < TRIMMED_CURVE)
  return STP_w_CRV_wf (ici, ip1, ip2, el1->srot, i360, oid); 

//   ici = STP_w_TRIMMED_CURVE__ (ici, ip1, ip2, p1);
//   // if(el1->dir > 0) ici = STP_w_TRIMMED_CURVE__ (ici, ip1, ip2, stpwTrue);
//   // else             ici = STP_w_TRIMMED_CURVE__ (ici, ip2, ip1, stpwTrue);
//   // GEOMETRIC_CURVE_SET < TRIMMED_CURVE
//   if(mode == 1)
//     ici = STP_w_CURVE_SET (&ici, 1); // CCW
//   //----------------------------------------------------------------
//   // mode=2: COMPOSITE_CURVE_SEGMENT < TRIMMED_CURVE
//   return STP_w_CCV_SEG (ici, i360, oid);   // 1=closed

}


//================================================================
  int STP_w_EL__ (CurvElli *el1, char *oid) {
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
//   objAct  lPlb;


  printf("STP_w_EL__ %s\n",oid);

  // centerpoint, startpoint, endpoint
  ipc = STP_w_PT (&el1->pc, 0, "");

//   // must have axis
//   if(!plb) plb = &lPlb;

  sr = UT3D_sr_el (el1);
  STP_w_objAct_set (&el1->pc, ipc, &el1->vz, -1, sr);

  // need x-axis
  obj_act.ivx = STP_w_VC_d (&el1->va, "");

  // create origin+axis as AXIS2_PLACEMENT_3D
  ipx = STP_w_axis3_vz ();

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
  int STP_w_CVPOL (CurvPoly *cv1, int dbi, char *oid, int mode) {
//================================================================
// mode=1 create pure geometric obj, not trimmed
//      2 create COMPOSITE_CURVE_SEGMENT        (curve)
//      3 create ORIENTED_EDGE < EDGE_CURVE
// Output:
//   vcn    normalvector
// {int ptNr; double v0, v1, *lvTab; Point *cpTab;} CurvPoly;

  int     i1, i2, ipa, iNr, *ia, icv, ip1, ip2, iStp;
  Point   *pa, pt1;
  double  *da;
  char    s1[1024], *p1;
  Vector  vcn;
  stp_cv  *dbCv = NULL;



  DEB_dump_obj__ (Typ_CVPOL, cv1, "STP_w_CVPOL dbi=%di mode=%d",dbi,mode);
  fprintf(stpw_fp,"/* Polygon %s */\n",oid);

  // check if curve already exists
  if(dbi) {
    dbCv = STP_w_oDBcv_ck_o (Typ_CV, dbi);
    if(dbCv) {
      icv = dbCv->iCv;
      ip1 = dbCv->iP1;
      ip2 = dbCv->iP2;
      goto L_nxt;
    }
//     icv = STP_w_oDBcv_ck__ (Typ_CV, dbi);
//     if(icv) { obj_act.iCv = icv; goto L_nxt;}
  }

  // define points
  ipa = stpw_li;
  pa = cv1->cpTab;
  for(i1=0; i1 < cv1->ptNr; ++i1) STP_w_PT (&pa[i1], 0, "");

//   // get normal-vector, bp, sr.
//   if(plb) STP_w_sur_nvc (plb, cv1->ptNr, pa);

  p1 = oid;

  // bspline (degree,
  icv = stpw_li;
  obj_act.iCv = icv;  // store curve
  sprintf(s1, "#%d=B_SPLINE_CURVE_WITH_KNOTS('%s',%d", stpw_li, p1, 1);
  // add list of points
  STP_w_list_is (s1, sizeof(s1), ipa, cv1->ptNr, ",");
  // curve_form:  unspecified, polyline_form, parabolic_arc, circular_arc,
  //              hyperbolic_arc, elliptic_arc
  STP_w_txt (s1, ",.UNSPECIFIED.");
  // closed_curve, self_intersect
  STP_w_txt (s1, ",.U.,.U.");


  // loop tru kvTab; size is ptNr+deg+1
  // set multiplicity = nr of identical values.

  iNr = cv1->ptNr;
  ia = (int*)MEM_alloc_tmp ((int)(iNr * sizeof(int)));
  // da = (double*)MEM_alloc_tmp (iNr * sizeof(double));

  for(i1=0; i1 < iNr; ++i1) {
    ia[i1] = 1;
    // da[i1] = cv1->lvTab[i1];
  }

  ia[0] = 2;
  ia[iNr - 1] = 2;
    // for(i1=0;i1<iNr;++i1) printf("   m=%d k=%lf\n",ia[i1],da[i1]);

  // output knot_multiplicities ia
  STP_w_list__ (s1, sizeof(s1), ia, iNr, 0, ",");

  // output knots da
  // STP_w_list__ (s1, da, iNr, 2);
  STP_w_list__ (s1, sizeof(s1), cv1->lvTab, iNr, 2, ",");

  // knot_type
  STP_w_txt (s1, ",.UNSPECIFIED.");

  fprintf(stpw_fp,"%s);\n",s1);
  ++stpw_li;


  L_nxt:

  //----------------------------------------------------------------
  // mode=1: pure geometric obj, not trimmed
  if(mode == 1) return icv;
    // return STP_w_CURVE_SET (&icv, 1); // CCW


  //----------------------------------------------------------------
  // mode 2,3,4: get trimPoint
  // set limits
  if(UTP_comp2db(cv1->v0, cv1->lvTab[0], UT_TOL_min1)) {
    // ip1 = ipa;
    ip1 = icv - cv1->ptNr;
  } else {
    // get startpoint
    // UTO_2pt_limstru (&pt1, NULL, NULL, NULL, Typ_CVBSP, cv1);
    UT3D_ptvcpar_std_obj (&pt1, NULL, NULL, 0, Ptyp_start, Typ_CVBSP, cv1);
    ip1 = STP_w_PT (&pt1, 0, "");
  }

  i1 = cv1->ptNr - 1;
  if(UTP_comp2db(cv1->v1, cv1->lvTab[i1], UT_TOL_min1)) {
    if(UT3D_comp2pt(&pa[0], &pa[i1], UT_TOL_pt) != 0) ip2 = ip1;
    // else ip2 = ipa + i1;
    else ip2 = icv - 1;
  } else {
    // get endpoint
    // UTO_2pt_limstru (NULL, &pt1, NULL, NULL, Typ_CVBSP, cv1);
    UT3D_ptvcpar_std_obj (&pt1, NULL, NULL, 0, Ptyp_end, Typ_CVBSP, cv1);
    ip2 = STP_w_PT (&pt1, 0, "");
  }
    printf(" w_CVPOL ip1=%d ip2=%d\n",ip1,ip2);

  if(!dbCv) {
    // store basicCurve in DB
    STP_w_oDBcv_add (Typ_CV, dbi, icv, ip1, ip2);
  }


  //----------------------------------------------------------------
  // mode=2: COMPOSITE_CURVE_SEGMENT < TRIMMED_CURVE
  if(mode == 2) {
    // create wirefame-curve (COMPOSITE_CURVE_SEGMENT < TRIMMED_CURVE)
    return STP_w_CRV_wf (icv, ip1, ip2, cv1->dir, cv1->clo, oid);
//     icv = STP_w_TRIMMED_CURVE__ (icv, ip1, ip2, stpwTrue);
//     // icv = STP_w_TRIMMED_CURVE_par (icv, cv1->v0, cv1->v1, stpwTrue);
//     // store in obj_act
//     obj_act.iP1 = ip1;
//     obj_act.iP2 = ip2;
// 
//     return STP_w_CCV_SEG (icv, cv1->clo, oid);
  }


  //----------------------------------------------------------------
  // mode=3,4: FACE_[OUTER_]BOUND < EDGE_LOOP < ORIENTED_EDGE < EDGE_CURVE
  // if(cv1->v1 > cv1->v0) i1 = 0;  // direction
  // else                  i1 = 1;
  i1 = cv1->dir;
  iStp = STP_w_SURBND__ (icv, ip1, ip2, i1, oid, mode);
//   i2 =  STP_w_ORIENTED_EDGE (ip1, ip2, icv, i1);
//   // EDGE_LOOP FACE_[OUTER_]BOUND 
//   iStp = STP_w_FACE_BOUND (&i2, 1, oid, mode);

    printf(" ex-STP_w_CVTRM__ %d\n",iStp);

  return iStp;

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



  DEB_dump_obj__ (Typ_CVPOL, cv1, "STP_w_CVPOL");


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


//================================================================
  int STP_w_CVCCV (int *ia,
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
  DEB_dump_obj__ (Typ_ObjGX, cv, "CCV");

  fprintf(stpw_fp,"// CompositeCurve %s //\n",oid);

  oxa = ((ObjGX*)cv)->data;

//   if(plb) {
    // get polygon from CCV
    pa = (void*)memspc201;
    pNr = sizeof(memspc201) / sizeof(Point);
    irc = UT3D_pta_ox_lim (&pNr, pa, cv, 0, NULL, UT_DISP_cv, 0);
      printf(" ccv-pNr=%d\n",pNr);
    if(irc < 0) {TX_Error("STP_w_CVCCV EOM"); return -1;}
    // set normal-vector, sense-of-rotation ..
    STP_w_sur_nvc (pNr, pa);
//   }


  iNr = 0;
  for(i1=0; i1<cv->siz; ++i1) {
    ox1 = &oxa[i1];
      DEB_dump_ox_0 (ox1, " CCV[%d]",i1);

    // skip data, form=Typ_CVTRM
    if(ox1->form == Typ_CVTRM) {
      continue;

    //----------------------------------------------------------------
    } else if(ox1->typ == Typ_PT) {
      if(i1 == 0) {
        UTO_objDat_ox ((void**)&pt1, &ii, ox1);
        actPos = *pt1;
        continue;
      }
      ln1.p1 = actPos;
      UTO_objDat_ox ((void**)&pt1, &ii, ox1);
      ln1.p2 = *pt1;
      actPos = *pt1;
      // skip 
      ii = STP_w_LN (&ln1, "", mode);

    //----------------------------------------------------------------
    // Lines: LINE > TRIMMED_CURVE > COMPOSITE_CURVE_SEGMENT
    } else if(ox1->typ == Typ_LN) {
      UTO_objDat_ox (&odc, &ii, ox1);
      ii = STP_w_LN (odc, "", mode);
      actPos = ((Line*)odc)->p2;

    //----------------------------------------------------------------
    // CIRCLE > TRIMMED_CURVE > COMPOSITE_CURVE_SEGMENT
    } else if(ox1->typ == Typ_CI) {
      UTO_objDat_ox (&odc, &ii, ox1);
        // DEB_dump_obj__ (Typ_CI, odc, " CCV[%d]",i1);
      ii = STP_w_AC_CC (odc, "", mode);
      actPos = ((Circ*)odc)->p2;

    //----------------------------------------------------------------
    // B_SPLINE_CURVE > TRIMMED_CURVE > COMPOSITE_CURVE_SEGMENT
    } else if(ox1->typ == Typ_CVBSP) {
      UTO_objDat_ox (&odc, &ii, ox1);
        DEB_dump_obj__ (Typ_CVBSP, odc, " CCV[%d]",i1);
      ii = STP_w_CVBSP (NULL, &actPos, odc, "", mode);

    //----------------------------------------------------------------
    } else if(ox1->typ == Typ_CVELL) {
      UTO_objDat_ox (&odc, &ii, ox1);
        DEB_dump_obj__ (Typ_CVELL, odc, " CCV[%d]",i1);
      ii = STP_w_EL_CC (odc, "", mode);
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
    return STP_w_COMPOSITE_CURVE (ia, iNr, oid);

    // // create GEOMETRIC_CURVE_SET
    // return STP_w_CURVE_SET (&ii, 1);
  }


  // mode=3: ORIENTED_EDGEs
  if(mode != 3) TX_Error ("STP_w_CVCCV E001 mode %d",mode);

  return iNr;

}
*/
 
//================================================================
  int STP_w_CVBSP (CurvBSpl *cv1, int dbi, char *oid, int mode) {
//================================================================
// create B_SPLINE_CURVE_WITH_KNOTS
// mode=1 create pure geometric obj, not trimmed
//      2 create wireframe-obj, trimmed
//      3 create shell-outerBoundary
//      4 create shell-innerBoundary
//   objAct    set normalvector if !NULL

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
  stp_cv  *dbCv = NULL;


  DEB_dump_obj__ (Typ_CVBSP, cv1, "STP_w_CVBSP %d",mode);


  fprintf(stpw_fp,"/* Bspline-Curve %s */\n",oid);

  // check if curve already exists
  if(dbi) {
    dbCv = STP_w_oDBcv_ck_o (Typ_CV, dbi);
    if(dbCv) {
      icv = dbCv->iCv;
      ip1 = dbCv->iP1;
      ip2 = dbCv->iP2;
      goto L_nxt;
    }
  }


  // define points
  ipa = stpw_li;
  pa = cv1->cpTab;
  for(i1=0; i1 < cv1->ptNr; ++i1) STP_w_PT (&pa[i1], 0, "");


  // set normalvector
//   if(plb) {
    // get normal-vector, bp, sr.
    STP_w_sur_nvc (cv1->ptNr, pa);
//   }



  p1 = oid;

  // bspline (degree,
  icv = stpw_li;
  sprintf(s1, "#%d=B_SPLINE_CURVE_WITH_KNOTS('%s',%d", stpw_li, p1, cv1->deg);
  // add list of points
  STP_w_list_is (s1, sizeof(s1), ipa, cv1->ptNr, ",");
  // curve_form:  unspecified, polyline_form, parabolic_arc, circular_arc,
  //              hyperbolic_arc, elliptic_arc
  STP_w_txt (s1, ",.UNSPECIFIED.");
  // closed_curve, self_intersect
  STP_w_txt (s1, ",.U.,.U.");


  // loop tru kvTab; size is ptNr+deg+1
  // set multiplicity = nr of identical values.
  iNr = cv1->ptNr + cv1->deg + 1;
    printf(" iNr=%d\n",iNr);
  ia = (int*)MEM_alloc_tmp ((int)(iNr * sizeof(int)));
  da = (double*)MEM_alloc_tmp ((int)(iNr * sizeof(double)));

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
  STP_w_list__ (s1, sizeof(s1), ia, kNr, 0, ",");

  // output knots da
  STP_w_list__ (s1, sizeof(s1), da, kNr, 2, ",");

  // knot_type
  STP_w_txt (s1, ",.UNSPECIFIED.");

  fprintf(stpw_fp,"%s);\n",s1);
  ++stpw_li;



  L_nxt:

  //----------------------------------------------------------------
  // GEOMETRIC_CURVE_SET < TRIMMED_CURVE
  if(mode == 1) return icv;
    // return STP_w_CURVE_SET (&icv, 1);
    // return STP_w_CCV_SEG (icv, oid);


  //----------------------------------------------------------------
  // get startpoint, endpoint
  // UTO_2pt_limstru (&pt1, &pt2, NULL, NULL, Typ_CVBSP, cv1);
  UT3D_ptvcpar_std_obj (&pt1, NULL, NULL, 0, Ptyp_start, Typ_CVBSP, cv1);
  UT3D_ptvcpar_std_obj (&pt2, NULL, NULL, 0, Ptyp_end, Typ_CVBSP, cv1);
  // *actPos = pt2;

// TODO: why not cv1->dir ?
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
    ip1 = STP_w_PT (&pt1, 0, "");
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
      ip2 = STP_w_PT (&pt2, 0, "");
    }
  }


  if(!dbCv) {
    // store basicCurve in DB
    STP_w_oDBcv_add (Typ_CV, dbi, icv, ip1, ip2);
  }


  //----------------------------------------------------------------
  // mode=3,4: FACE_[OUTER_]BOUND < EDGE_LOOP < ORIENTED_EDGE < EDGE_CURVE
  // get trim-points
  if(mode >= 3) {
    // EDGE_CURVE and ORIENTED_EDGE from baseCurve & 2 points
    // return STP_w_ORIENTED_EDGE (ip1, ip2, icv, iDir);
    return STP_w_SURBND__ (icv, ip1, ip2, iDir, oid, mode);
  }


  //----------------------------------------------------------------
  // mode=2: TRIMMED_CURVE
  // create wirefame-curve (COMPOSITE_CURVE_SEGMENT < TRIMMED_CURVE)
  return STP_w_CRV_wf (icv, ip1, ip2, cv1->dir, cv1->clo, oid);

    // icv = STP_w_TRIMMED_CURVE_par (icv, cv1->v0, cv1->v1, stpwTrue);
//     if(cv1->v1 > cv1->v0) p1 = stpwTrue;
//     else                  p1 = stpwFalse;
// 
//     icv = STP_w_TRIMMED_CURVE__ (icv, ip1, ip2, p1);
// 
//   // COMPOSITE_CURVE_SEGMENT
//   return STP_w_CCV_SEG (icv, cv1->clo, oid);

}


//===========================================================================
  int STP_w_CRV_wf (int icv, int ip1, int ip2, int dir, int clo, char *oid) {
//===========================================================================
// STP_w_CRV_wf               create wirefame-curve
//   write TRIMMED_CURVE > COMPOSITE_CURVE_SEGMENT
// Input:
//   icv     baseCurve
//   ip0,ip1 trimPoints
//   dir     0=.T. = fwd; 1=.F. = bwd
//   clo     0=yes, 1=not_closed; -1=undefined;


  printf("STP_w_CRV_wf %s %d %d %d dir=%d clo=%d\n",oid,icv,ip1,ip2,dir,clo);

  icv = STP_w_TRIMMED_CURVE__ (icv, ip1, ip2, dir, oid);

  // COMPOSITE_CURVE_SEGMENT < TRIMMED_CURVE
  return STP_w_CCV_SEG (icv, clo, oid);

}



//================================================================
  int STP_w_CVTRM__ (CurvCCV *cva, int oNr, char *oid, int mode) {
//================================================================
// STP_w_CVTRM__                    create trimmed-curve 
// in: a group of trimmed-curves; 
// out: 
// Input:
// Output:
//   retCode:
//     mode = 2: COMPOSITE_CURVE < COMPOSITE_CURVE_SEGMENTs < TRIMMED_CURVEs ..
//     mode = 3: FACE_OUTER_BOUND < ..
//     mode = 4: FACE_BOUND < ..


// was STP_w_CVCCV


  int      i1, iStp, iCv, *ia, iP0, iP1, sP0, sP1, iClo;
  long     l1, dbi;
  char     p0id[40], p1id[40], cid[40];
  Point    *pt0, *pt1;
  Line     ln1;
  CurvCCV  *cv1;
  ObjGX    ox1;


  printf("\nTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT \n");
  printf(" STP_w_CVTRM__ %s %d\n",oid, mode);
  for(i1=0;i1<oNr;++i1) { cv1 = &cva[i1];
    DEB_dump_obj__ (Typ_CVTRM, cv1, "STP_w_CVTRM__ [%d]",i1);}


  if(mode == 2) fprintf(stpw_fp,"/* contour-Curve %s */\n",oid);

  // iNr = 0;

  ia = (int*)MEM_alloc_tmp (oNr * sizeof(int));


  if(oNr == 1) iClo = cva[0].clo;
  else         iClo = 1;  // not closed


  //----------------------------------------------------------------
  // create basic-curves for wireframe|shell-boundary
  for(i1=0; i1<oNr; ++i1) {
    // next curve
    cv1 = &cva[i1];
      DEB_dump_obj__ (Typ_CVTRM, cv1, "----------- w_CVTRM__ [%d]",i1);

    // get basicCurve 
    dbi = cv1->dbi;
    APED_oid_dbo__ (cid, cv1->typ, dbi);

    // get trimPoints
    iP0 = cv1->ip0;
    pt0 = DB_get_PT (iP0);

    iP1 = cv1->ip1;
    pt1 = DB_get_PT (iP1);


    if((cv1->typ == Typ_LN)&&(dbi == 0)) {
      // dynamic-line (dbi=0) 
      UT3D_ln_ptpt (&ln1, pt0, pt1);
      OGX_SET_OBJ (&ox1, cv1->typ, cv1->typ, 1, &ln1);
    } else {
      ox1 = DB_GetObjGX (cv1->typ, dbi);
    }

    // create basic-curve; add iCv to list of curve-segments
    ia[i1] = STP_w_CRV__ (&ox1, dbi, cid, 1);


    // create trimPoints as CARTESIAN_POINTs
    APED_oid_dbo__ (p0id, Typ_PT, iP0);
    sP0 = STP_w_PT (pt0, iP0, p0id);
    APED_oid_dbo__ (p1id, Typ_PT, iP1);
    sP1 = STP_w_PT (pt1, iP1, p1id);

    // for wireframe: from baseCurve create TRIMMED_CURVE > COMPOSITE_CURVE_SEGMENT
    if(mode == 2) {
      sprintf(cid,"%s-%d",oid,i1);
      ia[i1] = STP_w_CRV_wf (ia[i1], sP0, sP1, cv1->dir, iClo, cid);

    // for shell: from baseCurve create EDGE_CURVE > ORIENTED_EDGE
    } else if(mode >= 3) {
      ia[i1] = STP_w_ORIENTED_EDGE (sP0, sP1, ia[i1], cv1->dir);
    }
//     ia[i1] = STP_w_CVTRM_1 (ia[i1], sP0, sP1, cv1->clo, cv1->dir, mode);

  }


  //----------------------------------------------------------------
  if(mode == 2) {
    // wireframe-obj: from wireframe-segments create COMPOSITE_CURVE
    iStp = STP_w_COMPOSITE_CURVE (ia, oNr, oid);
    // add COMPOSITE_CURVE to ol_GS
    if(iStp > 0) MemTab_add (&ol_GS, &l1, &iStp, 1, 0);
  }


  //----------------------------------------------------------------
  if(mode >= 3) {
    // boundary-obj: from boundary-segments create FACE_OUTER_BOUND|FACE_BOUND
    iStp = STP_w_FACE_BOUND (ia, oNr, oid, mode);
  }


  //----------------------------------------------------------------
  printf(" ex-STP_w_CVTRM__ %d\n",iStp);

 
  return iStp;

}

/*
//=======================================================================
  int STP_w_CVTRM_1 (int icv, int ip0, int ip1, 
                     int clo, int dir, int mode) {
//=======================================================================
// STP_w_CVTRM_1               trimmedCurv > COMPOSITE_CURVE_SEGMENT|ORIENTED_EDGE
// Input:
//   icv     baseCurve
//   ip0,ip1 trimPoints
//   clo     0=yes, 1=not_closed; -1=undefined;
//   dir     0=.T. = fwd; 1=.F. = bwd
//   mode    2 wireframe: baseCurve > TRIMMED_CURVE > COMPOSITE_CURVE_SEGMENT
//           3 shell: baseCurve > EDGE_CURVE > ORIENTED_EDGE


  int     scv, iStp;


  printf("STP_w_CVTRM_1 -------- icv=%d ip0=%d ip1=%d clo=%d dir=%d mode=%d\n",
         icv, ip0, ip1, clo, dir, mode);

  //----------------------------------------------------------------
  if(mode == 2) {

    iStp = STP_w_CRV_wf (icv, ip0, ip1, dir, clo, "");
//     // TRIMMED_CURVE
//     scv = STP_w_TRIMMED_CURVE__ (icv, ip0, ip1, dir);
//     // COMPOSITE_CURVE_SEGMENT
//     // set dir = 0 = fwd (already set in TRIMMED_CURVE)
//     iStp = STP_w_COMPOSITE_CURVE_SEGMENT (clo, 0, scv);


  //----------------------------------------------------------------
  } else if(mode == 3) {

    // out EDGE_CURVE and ORIENTED_EDGE
    iStp = STP_w_ORIENTED_EDGE (ip0, ip1, icv, dir);

  }

  //----------------------------------------------------------------

    printf("ex-STP_w_CVTRM_1 %d\n",iStp);


  return iStp;

}
*/

//================================================================
  int STP_w_COMPOSITE_CURVE_SEGMENT (int clo, int dir, int icv) {
//================================================================
// create COMPOSITE_CURVE_SEGMENT from a TRIMMED_CURVE
// Input:
//   clo       0=yes, 1=not_closed; -1=undefined;
//   ori       .T. = fwd; .F. = bwd
//
// COMPOSITE_CURVE_SEGMENT(.CONTINUOUS., .T., <TRIMMED_CURVE>) ;
//   transition      .CONTINUOUS.|.DISCONTINUOUS.   (closed or not)
//     .DISCONTINUOUS. = open; .CONTINUOUS. = closed
//   same_sense      .T.|.F.  fwd|bwd
//   

// static char sc[] = ".CONTINUOUS.";
// static char so[] = ".DISCONTINUOUS.";

  int     i1;
  char    *sDir, *sClo, s1[128];


  // compositeCurveSegment = trimmedCurve
  // .CONTINUOUS.,.T.         closed CCV
  // .DISCONTINUOUS.,.F.      open CCV
  if(clo) sClo = stpwOpe;   // open
  else    sClo = stpwClo;

  if(dir) sDir = stpwFalse; // bwd
  else    sDir = stpwTrue;

  sprintf(s1, "#%d=COMPOSITE_CURVE_SEGMENT(%s,%s,#%d)",
              stpw_li, sClo, sDir, icv);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;

  return (stpw_li - 1);

}


//================================================================
  int STP_w_SUR__ (ObjGX *oxs, long dbi, char *oid) {
//================================================================
// STP_w_SUR__              export surface
//   retCode     step-index of 

  int      iStp, typ;
  // int      irc, att, oNr;
  // ObjGX    *vTab = NULL;


  DEB_dump_obj__ (Typ_ObjGX, oxs, "STP_w_SUR__  %s",oid);
    
  fprintf(stpw_fp,"/* open-shell-Surface %s */\n",oid);

  typ  = oxs->typ;


  //----------------------------------------------------------------
  if(typ == Typ_SUR) {
    // sTyp Typ_SUTP  trimmed-perforated-surf
    // ADVANCED_FACE < surpln
    iStp = STP_w_SURTP__ ((ObjGX*)oxs, oid);
    goto L_exit;

  //----------------------------------------------------------------
  } else if(typ == Typ_SURCIR) {     // ?? 
    iStp = STP_w_SURCIR ((ObjGX*)oxs, oid);
    goto L_exit;

  //----------------------------------------------------------------
  } else if(typ == Typ_SURBSP) {     // B_Spline_Surf  SurBSpl
    // supportSurfaces
    iStp = STP_w_SURSUP__ (oxs, oid);
    iStp = 0;  // do not yet add to OPEN_SHELL - make ADVANCED_FACE if suppSur is not hidden
    STP_w_log_err (
      "NOT-YET-IMPLEMENTED - STP_w_SUR__ - B_Spline_Surf E001 %s",oid);
    goto L_exit;

  //----------------------------------------------------------------
  } else {
    STP_w_log_err (
      "NOT-YET-IMPLEMENTED - %s - typ = %d",typ,oid);
    return -1;
  }

/*
  switch(typ) {

    //----------------------------------------------------------------
    case Typ_SUR:         // sTyp Typ_SUTP  trimmed-perforated-surf
      // ADVANCED_FACE < surpln
      iStp = STP_w_SURTP__ ((ObjGX*)oxs, oid);
      break;

// TODO: the following are supportSurfaces ony; see STP_w_SURSUP__
    //----------------------------------------------------------------
    case Typ_SURRV:    // 54
      iStp = STP_w_SURRV (oxs->data, oid, 4);
      break;
    //----------------------------------------------------------------
    case Typ_SURRU:    // 53
      iStp = STP_w_SURRU (oxs->data, oid, 4);
      break;
    //----------------------------------------------------------------
    case Typ_SURBSP:    // 55
      iStp = STP_w_SURBSP (oxs->data, oid);
//       // add unlimited support-surface to GEOMETRIC_SET
//       if(iStp > 0) MemTab_add (&ol_GS, &l1, &iStp, 1, 0);
      return 0;
    // other surfaces:
    //----------------------------------------------------------------
    case Typ_SURCIR:
      // ADVANCED_FACE
      iStp = STP_w_SURCIR ((ObjGX*)oxs, oid);
      break;

    //----------------------------------------------------------------
    default:
//  TODO: export tesselated ..
      printf("***** STP_w_os_wo unsupported: %d\n",typ);
      return -1;
  }
*/

  L_exit:

    printf("ex-STP_w_SUR__ %d\n",iStp);

  return iStp;


/*
  //================================================================
  // tesselated-surface:
  // get color: see STP_w_col_sur

  // start tesselation-mode store
  irc = TSU_Init (1, &vTab);
  if(irc < 0) {TX_Error("STP_w_ox__ E1"); return -1;}

  att = 0;
  
  // tesselate into buffer vTab
  irc = TSU_SUR__ (oxs, att, dbi);

  // reset tesselation; get accumulated Vertices (of MainModel)
  oNr = TSU_Init (0, &vTab);
    // printf(" STP_w_ox__-tess-oNr=%d\n",oNr);

  // write faces
  if(!irc) STP_w_tess (vTab);

  // free memspace
  TSU_Init (5, &vTab); 
*/

}


//================================================================
  int STP_w_SURSUP__ (ObjGX *oss, char *oid) {
//================================================================
// STP_w_SURSUP__          create supportsurface
// TODO use STP_w_SURSUP__ in STP_w_SUR__ for suppSur only
// extract support-surface aus STP_w_SURTP__ :2824 und STP_w_SUR__

  int     ssTyp, typ2, iss, isu, oNr;
  long    ssDbi, dbi2;
  void    *ssDat;
  ObjGX   *oa;


  printf("STP_w_SURSUP__ %s\n",oid);
  DEB_dump_ox__ (oss, "SURSUP__\n");

  // create support-surface from record # 0
    fprintf(stpw_fp,"/* supportSurface for %s */\n",oid);

  //----------------------------------------------------------------
  // ssTyp = UTO_get_suppSurf (&ssDat, oss);
  if(oss->form == Typ_Index) {    // return STP_w_log_err ("STP_w_SURSUP__ E1");
    OGX_GET_INDEX (&ssTyp, &ssDbi, oss);
      printf(" ssTyp=%d dbi=%ld\n",ssTyp,ssDbi);
    // get support-surface from DB
    ssTyp = UTO__dbo (&ssDat, &oNr, ssTyp, ssDbi);
      DEB_dump_obj__(ssTyp, ssDat, "w_SURSUP__-ss");


  //----------------------------------------------------------------
  } else if(oss->form == Typ_ObjGX) { 
    // ruled surface from eg 2 circles
    if(oss->typ == Typ_SURRU) {
      iss = STP_w_SURRU (oss[0].data, oid);
      goto L_exit;
    }


  //----------------------------------------------------------------
  } else {
    ssTyp = oss->form;
    ssDat = oss->data;
  } 


  //----------------------------------------------------------------
  // planar surface:
  if(ssTyp == Typ_PLN) {
    // create PLANE
    iss = STP_w_PLANE (ssDat, ssDbi);
       printf(" w_SURSUP__-iss=%d\n",iss);


  //----------------------------------------------------------------
  // Revolved surf
  } else if(ssTyp == Typ_SURRV) {   // 54
    iss = STP_w_SURRV ((SurRev*)ssDat, oid, 4);  // 3 without trimmed !


  //----------------------------------------------------------------
  // B_Spline_Surf
  } else if(ssTyp == Typ_SURBSP) {
    iss = STP_w_SURBSP ((SurBSpl*)ssDat, oid);


/*
  //----------------------------------------------------------------
  // Ruled Surf
  } else if(ssTyp == Typ_SURRU) {  // 53
    iss = STP_w_SURRU ((ObjGX*)ssDat, oid, 3);
*/

  //----------------------------------------------------------------
  } else goto L_err1;


  L_exit:
  //----------------------------------------------------------------
  return iss;


  //----------------------------------------------------------------
  L_err1:
    return STP_w_log_err("STP_w_SURSUP__ E2 %s %d",oid,ssTyp);

}


/*
//================================================================
  int STP_w_tess (ObjGX *oxi) {
//================================================================
// STP_w_tess               write tesselated faces
// TODO

  int     irc, i1, rSiz;
    

  printf("STP_w_tess typ=%d form=%d siz=%d\n",
         oxi->typ,oxi->form,oxi->siz);

  L_next:
  if(oxi->typ  == Typ_Done)  goto L_exit;

  // first record must be size of following Record
  if(oxi->typ  != Typ_Size) goto L_Err2;
  rSiz = (long)oxi->data;
  // printf("Record %d size=%d\n",i1,rSiz);

  ++oxi;  //(char*)oxi += sizeof(ObjGX);

  // export
  STP_w_fac1 (oxi);

  // (char*)oxi += rSiz;
  oxi = (ObjGX*)((char*)oxi + rSiz);
  ++i1;
  goto L_next;


  
  L_exit:

  return 0;

  L_Err2:
    TX_Error("STP_w_tess E002 typ %d",oxi->typ);
    return -1;

}


//================================================================
  int STP_w_fac1 (ObjGX *oxi) {
//================================================================
// STP_w_fac1          write tesselated face
// see also TSU_ntria_bMsh_p GL_set_bMsh
// was TSU_exp_fac
// TODO

  int     i1, i2, i1Nr, i2Nr, iTyp;
  int     iTex;
  ColRGB  *sCol;
  ObjGX   *oxs;


  // printf("\nSTP_w_fac1 %d %d %d\n",oxi->typ,oxi->form,oxi->siz);
  // DEB_dump_ox_s_ (oxi, "STP_w_fac1");
  // DEB_dump_ox_0 (oxi, "TSU_exp_fac");

  i1Nr = oxi->siz;              // ex Sur  211 143 5
   // printf(" exp_fac1-i1Nr=%d\n",i1Nr);

  iTex = -1;
  // sCol = exp_sCol;
    // if(sCol) UTcol_dump (sCol, "exp_fac1-sCol-L1");


  // color comes as single Record; - here always the default-color; skip it ..
  if(oxi->typ == Typ_Color) {
    // exp_sCol = ((ColRGB*)&oxi->data);
      // UTcol_dump (sCol, "exp_fac1-sCol-L2");
    return 0;
  }


  // Parentrecord
  // typ=SurGL_(70),form=ObjGX(205),siz=1 -> typ=SurGLpp(71),form=PT(3)
  oxi = oxi->data; // skip 211 143 5


  for(i1=0; i1<i1Nr; ++i1) { // loop tru patches

    // Patch kann ein ObjGX oder nur pTab sein
    if(oxi->form == Typ_ObjGX) {
      i2Nr  = oxi->siz;
      oxs = oxi->data;

    } else {
      i2Nr  = 1;
      oxs = oxi;

    }
      // printf(" ..TSU_exp_fac s[%d] %d %d %d\n",i1,oxs->typ,oxs->form,i2Nr);

    for(i2=0; i2<i2Nr; ++i2) {
        // printf(" ..TSU_exp_fac[%d] typ=%d form=%d siz=%d\n",i2,
          // oxs->typ, oxs->form, oxs->siz);
      iTyp = oxs->form;  // 2005-08-10

      if(oxs->typ == Typ_Color) {
        sCol = (ColRGB*)&oxs->data;
        iTex = -1;
          // printf("%d exp_fac1-sCol-L3 %d %d %d\n",i2,sCol->cr,sCol->cg,sCol->cb);


      } else if(oxs->typ == Typ_Texture) {
        // sCol = NULL;
        iTex = INT_PTR (oxs->data);
          // printf(" facTex=%d\n",iTex);


      } else if(iTyp == Typ_PT) {
        if(oxs->siz < 3) {      // auch letzer Punkt !!!
          printf(" TSU_exp_fac I001\n");
          continue;
        }
        // TSU_exp_objFac (oxs);
        STP_w_fac2 (oxs, sCol, iTex); // VR2
      }
      ++oxs;  //(char*)oxs += sizeof(ObjGX);
    }

    ++oxi;  //(char*)oxi += sizeof(ObjGX);

  }


  return 0;

}


//================================================================
  int STP_w_fac2 (ObjGX *oxi, ColRGB *sCol, int iTex) {
//================================================================
// was TSU_exp_wrl2Fac
// TODO

  int    i0, i1, i2, i3, ie;
  Point  *pTab, p1;
  char   cbuf[128];


  // printf("STP_w_fac2 iTex=%d\n",iTex);


  fprintf(stpw_fp," Shape {\n");

  // if(sCol != NULL) {
//   if(iTex < 0) {
//     STP_w_col__ (sCol);
// 
//   } else {
//     STP_w_tex (iTex);
//   }


  // write faces ..
  fprintf(stpw_fp,
    "  geometry IndexedFaceSet { solid FALSE coord Coordinate { point [\n");
    //, cbuf);

  ie   = oxi->siz;  // Anzahl von Punkten 
  pTab = oxi->data;

  // export points
  STP_w_pta (pTab, ie);
  // letzter Punkt nicht notwendig.


  // textures: write TextureCoordinate-table
  if(iTex < 0) goto L_ci;
  TSU_exp_wrl2Tex (pTab, ie, iTex);


  // start coordIndextable ..
  L_ci:
  fprintf(stpw_fp,"  ]} coordIndex [\n");


  //================================================================
  // TRIANGLE_FAN:
  if(oxi->aux != GL_TRIANGLE_FAN) goto L_GLNPF;
  // printf("fffffffffffffffffffffffff fan fffffffffffffffff %d\n",ie);

  // die ersten 4
  i0 = 0;

  fprintf(stpw_fp,"  %d",i0);
    // printf(" %d",i0);
  ++i0;

  L_f_nxt:

    fprintf(stpw_fp,",%d",i0);
      // printf(",%d",i0);
    ++i0;
    if(i0 < ie) goto L_f_nxt;

  fprintf(stpw_fp,",-1 ] } }\n");
    // printf(",-1 ] } }\n");
  goto L_fertig;




  //================================================================
  // nonPlanar TRIANGLE_FAN:
  L_GLNPF:
  if(oxi->aux != GL_FAC_PLANAR) goto L_GLS;
  // printf("fffffffffffffffffffffffff fan fffffffffffffffff %d\n",ie);

//    1------2           ptAnz = 4
//    |    /  |
//    |   /   |
//    |  /    |
//    | /     |
//    0.------3
//      \     /
//        \  /
//         4
//
// sollte so zerlegt werden:
// 0 1 2
// 0 2 3
// 0 3 4

  i1 = 1;

  L_n_nxt:

    if(i1 < (ie-2)) fprintf(stpw_fp,"  0,%d,%d,-1,\n",i1,i1+1);
    else        fprintf(stpw_fp,"  0,%d,%d,-1 ] } }\n",i1,i1+1);
      // printf(" npf: 0,%d,%d,-1\n",i1,i1+1);

    ++i1;
    if(i1 < (ie-1)) goto L_n_nxt;

  goto L_fertig;



  //================================================================
  L_GLS:
  if(oxi->aux != GL_TRIANGLE_STRIP) goto L_GLT;
  // printf("SSSSSSSSSSSSSSSSSSSSSSSSS strip SSSSSSSSSSSSSSSSS %d\n",ie);

// GL_TRIANGLE_STRIP:
//
//    0--2--4--6      ie = 8
//    | /| /| /|
//    |/ |/ |/ |
//    1--3--5--7
//
// sollte so zerlegt werden:
// 0 1 2                       0 2 1
// 2 1 3                       2 3 1
// 2 3 4                       2 4 3
// 4 3 5
// 4 5 6   usw ...

  i1 = 0;
  i3 = i1 + 1;       // 1 - 2


  L_s_nxt:
  // i1 bleibt
  i2 = i3;
  ++i3;          // 1 2 3       3 4 5

  if(i3 < (ie-1)) fprintf(stpw_fp,"  %d,%d,%d,-1,\n",i1,i2,i3);
  else        fprintf(stpw_fp,"  %d,%d,%d,-1 ] } }\n",i1,i2,i3);
  // if(i3 < (ie-1)) fprintf(stpw_fp," %d,%d,%d,-1,\n",i1,i3,i2);
  // else        fprintf(stpw_fp," %d,%d,%d,-1 ] } }\n",i1,i3,i2);
    // printf(" facOut %d,%d,%d,-1\n",i1,i2,i3);


  i1 = i3;
  // i2 bleibt
  ++i3;           // 3 2 4
  if(i3 >= ie) goto L_fertig;


  if(i3 < (ie-1)) fprintf(stpw_fp,"  %d,%d,%d,-1,\n",i1,i2,i3);
  else        fprintf(stpw_fp,"  %d,%d,%d,-1 ] } }\n",i1,i2,i3);
  // if(i3 < (ie-1)) fprintf(stpw_fp," %d,%d,%d,-1,\n",i1,i3,i2);
  // else        fprintf(stpw_fp," %d,%d,%d,-1 ] } }\n",i1,i3,i2);
    // printf(" facOut %d,%d,%d,-1\n",i1,i2,i3);

  if(i3 < (ie-1)) goto L_s_nxt;
  goto L_fertig;



  //================================================================
  L_GLT:
  // printf("TTTTTTTTTTTTTTTTTTTTTTTTT trias TTTTTTTTTTTTTTTTT %d\n",ie);
  if(oxi->aux != GL_TRIANGLES) {
    TX_Print("TSU_exp_objFac E001 %d",oxi->aux);
    return -1;
  }

  i3 = -1;

  L_t_nxt:
    i1 = i3 + 1;
    i2 = i1 + 1;
    i3 = i2 + 1;

    if((i3+3) < ie) fprintf(stpw_fp,"  %d,%d,%d,-1,\n",i1,i2,i3);
    else        fprintf(stpw_fp,"  %d,%d,%d,-1 ] } }\n",i1,i2,i3);
      // printf(" %d,%d,%d,-1\n",i1,i2,i3);

    if((i3+3) < ie) goto L_t_nxt;




  //================================================================
  L_fertig:
  // TSU_ptNr = ie;
  // ++TSU_facNr;


  return 0;

}
*/

//================================================================
  int STP_w_pta (Point *pta, int ptNr) {
//================================================================
// STP_w_pta          write icoords of group of points

  int    i1;
  char   s1[128];
  // Point  pt1;


  for(i1=0; i1<ptNr; ++i1) {
    s1[0] = '\0';
    // pt1 = VR2_exp_pt (&pta[i1]);
    UTX_add_fl_ua (s1, (double*)&pta[i1], 3, ' ');
    fprintf(stpw_fp, "  %s\n",s1);
  }

  return 0;

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


//===========================================================================
  int STP_w_TRIMMED_CURVE__ (int icv, int ips, int ipe, int dir, char *oid) {
//===========================================================================
// write curve trimmed with 2 CARTESIAN_POINT's
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



  char    s1[128], *sDir;

  printf("STP_w_TRIMMED_CURVE__ %d %d %d %d\n",icv,ips,ipe,dir);


    if(dir) sDir = stpwFalse; // bwd
    else    sDir = stpwTrue;

  // TRIMMED_CURVE (baseCurve,startPt,endPt)
  sprintf(s1, "#%d=TRIMMED_CURVE('%s',#%d,(#%d),(#%d),%s,.CARTESIAN.)",
              stpw_li, oid, icv, ips, ipe, sDir);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;

  obj_act.iP1 = ips;
  obj_act.iP2 = ipe;

  return (stpw_li - 1);

}


//==================================================================
  int STP_w_CURVE_SET (int *ia, int iNr) {
//==================================================================
// create GEOMETRIC_CURVE_SET from a list of basecurves
// #232=GEOMETRIC_CURVE_SET('',(#231));

// jede line eigener curveset; points aber auch mehrere .


  int     i1;
  char    s1[2048];
  

  sprintf(s1, "#%d=GEOMETRIC_CURVE_SET(''", stpw_li); //, stpw_li - 1);

  STP_w_list__ (s1, sizeof(s1), ia, iNr, 1, ",");
  fprintf(stpw_fp,"%s);\n",s1);
  ++stpw_li;

  return (stpw_li - 1);
  
} 


//==================================================================
  int STP_w_CCV_SEG (int icv, int clo, char *oid) {
//==================================================================
// create COMPOSITE_CURVE_SEGMENT from a TRIMMED_CURVE
// Input:
//   clo       0=yes, 1=not_closed; -1=undefined;
//
// COMPOSITE_CURVE_SEGMENT(.CONTINUOUS., .T., <TRIMMED_CURVE>) ;
//   // transition      .CONTINUOUS.|.DISCONTINUOUS.   (closed or not)
//   //     .DISCONTINUOUS. = open; .CONTINUOUS. = closed
//   // same_sense      .T.|.F.    Always True;
//   //           direction already set in TRIMMED_CURVE


  
  int     i1;
  char    s1[128], *sClo;
  
  if(clo) sClo = stpwOpe;   // open
  else    sClo = stpwClo;


  // compositeCurveSegment = trimmedCurve
  // .CONTINUOUS.,.T.         closed CCV
  // .DISCONTINUOUS.,.F.      open CCV
  sprintf(s1, "#%d=COMPOSITE_CURVE_SEGMENT(%s,.T.,#%d)",
              stpw_li, sClo, icv);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;

  return (stpw_li - 1);

}


//==================================================================
  int STP_w_COMPOSITE_CURVE (int *ias, int iNr, char *oid) {
//==================================================================
// STP_w_COMPOSITE_CURVE        create COMPOSITE_CURVE for wireframe-obj
// create COMPOSITE_CURVE from list of COMPOSITE_CURVE_SEGMENTs
  
// #216=COMPOSITE_CURVE('L20',(#215),.U.);
// L:  GEOMETRIC_CURVE_SET
// P2: list of COMPOSITE_CURVE_SEGMENTs
// P3: self_intersect; .U. undefined

  int     i1;
  char    s1[2048];

  // compositeCurve from list of compositeCurveSegments
  sprintf(s1, "#%d=COMPOSITE_CURVE('%s'", stpw_li, oid);
  STP_w_list__ (s1, sizeof(s1), ias, iNr, 1, ",");
  fprintf(stpw_fp,"%s,.U.);\n",s1); 
  ++stpw_li;
  
  return (stpw_li - 1);

}


//================================================================
  int STP_w_FACE_BOUND (int *ia, int iNr, char *oid, int mode) {
//================================================================
// STP_w_FACE_BOUND           FACE_OUTER_BOUND|FACE_BOUND from boundary-segments
// Input:
//   mode    3=FACE_OUTER_BOUND, 4=FACE_BOUND

  int    ipc;
  char   *p1, s1[128];


  printf("STP_w_FACE_BOUND %si %d\n",oid,mode);


  if(mode == 3) {   // outer-boundary
    fprintf(stpw_fp,"/* outer-boundary for %s*/\n",oid);
  } else {          // inner-boundary
    fprintf(stpw_fp,"/* inner-boundary for %s */\n",oid);
  }


  // EDGE_LOOP < ORIENTED_EDGE
  ipc = STP_w_EDGE_LOOP_out (ia, iNr);


  // create FACE_OUTER_BOUND from EDGE_LOOP ipc
  // set orientation T|F
  if(mode == 3) {   // outer-boundary; CCW
    if(!obj_act.sro) p1 = stpwTrue;
    else             p1 = stpwFalse;
    sprintf(s1,"#%d=FACE_OUTER_BOUND('%s',#%d,%s)",stpw_li,oid,ipc,p1);

  } else {          // inner-boundary; CW
    if(obj_act.sro)  p1 = stpwTrue;   // should use obj_act.sri
    else             p1 = stpwFalse;
    sprintf(s1,"#%d=FACE_BOUND('%s',#%d,%s)",stpw_li,oid,ipc,p1);
  }

  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;


  return (stpw_li - 1);

}

/*
//==================================================================
  int STP_w_CCV (int *icv, int *ips, int *ipe, int iNr, char *oid) {
//==================================================================
// create COMPOSITE_CURVE from a list of basecurves,startpoints,enpoints.
 
  int     i1;
  char    s1[128];


  // loop tru list
  for(i1=0; i1<iNr; ++i1) {

    // TRIMMED_CURVE(baseCurve,startPt,endPt)
    STP_w_TRIMMED_CURVE__ (icv[i1], ips[i1], ipe[i1], stpwTrue);

    // compositeCurveSegment = trimmedCurve
    // .CONTINUOUS.,.T.         closed CCV
    // .DISCONTINUOUS.,.F.      open CCV
    sprintf(s1, "#%d=COMPOSITE_CURVE_SEGMENT(.DISCONTINUOUS.,.F.,#%d)",
                stpw_li, stpw_li - 1);
    fprintf(stpw_fp,"%s;\n",s1);
    ++stpw_li;
  }
  
// TODO: build list of compositeCurveSegments. MEM_alloc_tmp; 
// see STP_w_wf_init-GEOMETRIC_SET

  // compositeCurve from list of compositeCurveSegments
  sprintf(s1, "#%d=COMPOSITE_CURVE('%s',(#%d),.U.)",
              stpw_li, oid, stpw_li - 1);
  fprintf(stpw_fp,"%s;\n",s1);
  ++stpw_li;
  
  return (stpw_li - 1);
  
} 
*/

//================================================================
  int STP_w_axis__ (int *ii, Point *pto, Vector *vz, Vector *vx) {
//================================================================
// write position, orientation
// Input:
//   ii    step-Index; if 0 get next

  int   ip, iz, ix;

  printf("STP_w_axis__ %d\n",*ii);

  if(*ii == 0) { *ii = stpw_li; ++stpw_li;}


  fprintf(stpw_fp,"#%d=AXIS2_PLACEMENT_3D('',#%d,#%d,#%d);\n",
    *ii, stpw_li, stpw_li+1, stpw_li+2);

  fprintf(stpw_fp,"#%d=CARTESIAN_POINT('',(%f,%f,%f));\n",
    stpw_li, pto->x, pto->y, pto->z);
  ++stpw_li;

  fprintf(stpw_fp,"#%d=DIRECTION('',(%f,%f,%f));\n",
    stpw_li, vz->dx, vz->dy, vz->dz);
  ++stpw_li;


  fprintf(stpw_fp,"#%d=DIRECTION('',(%f,%f,%f));\n",
    stpw_li, vx->dx, vx->dy, vx->dz);
  ++stpw_li;

  return 0;

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
  int STP_w_list_is (char *s1, int sSiz, int is, int iNr, char *sPre) {
//================================================================
// write list of links; starting with is.
// eg (#200,#201,#202,#203,#204)
// s1 = outputline with len=sSiz
// sPre   add this to s1 before adding list
  

  int     i1, *ia;


  ia = (int*)MEM_alloc_tmp ((int)(iNr * sizeof(int)));
  for(i1=0; i1<iNr; ++i1) ia[i1] = is + i1;

  return STP_w_list__ (s1, sSiz, ia, iNr, 1, sPre);

}


//========================================================================
  int STP_w_list__ (char *s1, int sSiz,
                    void *data, int iNr, int mode, char *sPre) {
//========================================================================
// STP_w_list__        format list of integer or float-values
//   If length > 80 chars, write list out, return rest of lisat.
// Input:
//   mode  0=int (eg (1,2,66,1234))
//         1=links from iTab eg  (#200,#101,#202)
//         2=doubles;        eg  (-1.74860160797,-5.48849526774,0.)
//   sPre  add this to s1 before adding list
// Output:
//   s1    outputline with len=sSiz
//   sSiz  size of s1 (should be 128) 
// does not write line if < 80 chars.


  int     i1, ie, ll, *ia;
  char    *p1, s2[80];
  double  *da;


  printf("STP_w_list__ iNr=%d mode=%d\n",iNr,mode);
  if(mode==1) {
    for(i1=0;i1<iNr;++i1) printf("  list-i[%d]=%d\n",i1,((int*)data)[i1]);}


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
    if(i1 >= sSiz) {
       strcpy(s2, "STP_w_list__ overflow");
       TX_Print(s2); LOG_A__ (MSG_ERR_typ_INF, s2); ++exp_errNr; return -1;}
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

    printf("ex-STP_w_list__ |%s|\n",s1);


  return 0;

}


//================================================================
  int STP_w_end () {
//================================================================

  fprintf(stpw_fp,"\n");
  fprintf(stpw_fp,"\n");

  fprintf(stpw_fp,"ENDSEC;\n");
  fprintf(stpw_fp,"END-ISO-10303-21;\n");

  return 0;

}


//================================================================
  int STP_w_objAct_set (Point *po, int ipo, Vector *vz, int ivz,
                        int sro) {
//================================================================
// set outer-boundary
// Input:
//   ipo     -1 point po not yet created
//   ivz     -1 vector vz not yet created
//   ivx     -1 vector vx not yet created
//   sr      0=CCW; 1=CW; 2=undefined
 
  obj_act.po    = *po;
  obj_act.ipo   = ipo;
  obj_act.vz    = *vz;
  obj_act.ivz   = ivz;
  obj_act.ivx   = -1;
  obj_act.sro   = sro;

  return 0;

}


//================================================================
  int STP_w_sur_nvc (int ptNr, Point *pa) {
//================================================================
// outerBoundary: get normal-vector, sense-of-rotation ..

  int    ia[6];


  printf("STP_w_sur_nvc ptNr=%d stat=%d\n",ptNr,obj_act.stat);

  if(obj_act.stat) goto L_ib;

  //----------------------------------------------------------------
  // outerBoundary: get normal-vector, sense-of-rotation ..

  // get 3D-box from npt
  UT3D_box_ix_npt (&obj_act.pb1, &obj_act.pb2, ia, ptNr-1, pa);
    // GR_tDyn_box__ (&obj_act.pb1, &obj_act.pb2, 2);

  // get backPlane from 3D-box
  obj_act.bp = UT3D_bp_perp_2pt (&obj_act.pb1, &obj_act.pb2);
    printf(" bp=%d\n",obj_act.bp);

  // get normalvector obj_act.vz from backPlane and extreme-points
  UT3D_vc_perp_npt_bp_ix (&obj_act.vz, pa, ia, obj_act.bp);

  // get sr from npt&bp
  obj_act.sro = UT3D_sr_npt_bp (ptNr-1, pa, obj_act.bp);

  return 0;



  //----------------------------------------------------------------
  L_ib:
  // innerBoundary: get sense-of-rotation from bp
  // get normalvector and sr from npt&bp
  obj_act.sri = UT3D_sr_npt_bp (ptNr-1, pa, obj_act.bp);

  return 0;

}


//================================================================
  int STP_w_objAct_ini () {
//================================================================
 
  obj_act.ipo   = 0;
  obj_act.ivz   = 0;

  return 0;

}


//================================================================
  int STP_w_int_mnam (char *so, char *mnam) {
//================================================================

  // sprintf(so, "%s_%s", MdlNam, mnam);
  sprintf(so, "%s", mnam);
  // UTX_safeName (so, 1);

  return 0;

}


//================================================================
  int STP_w_ext_mnam (char *so, char *mnam) {
//================================================================

  strcpy(so, mnam);
  UTX_ftyp_cut (so);
  UTX_safeName (so, 1);

  return 0;

}


//================================================================
  int STP_w_dump_mdlTab (char *txt) {
//================================================================
// 

  int    i1, ii;
  oMdl   *om1;

  ii = mdlTab.rNr;

  printf(":::::::::: STP_w_dump_mdlTab %d :::::::::::::::::::::\n",ii);
  printf(" tabSiz=%d used=%d\n",mdlTab.rMax,mdlTab.rNr);



  for(i1=0; i1<ii; ++i1) {
    om1 = MEMTAB__(&mdlTab, i1);
    printf("%2d: ",i1);
    STP_w_dump_oMdl (om1, NULL);
  }

  printf(" actTab.mNam |%s|\n",actTab->mNam);


  return 0;

}


//================================================================
  int STP_w_dump_oMdl (oMdl *om1, char *txt) {
//================================================================
// 
    
  if(txt) printf("     %s\n",txt);

  
  printf("stat=%d mbi=%3d p__=%4d pd_=%4d |%s|\n",
         om1->stat,om1->mbi,om1->p__,om1->pd_, om1->mNam);
  
  return 0;
    
}   
  

//================================================================
  int STP_w_log_inf (char *eTxt) {
//================================================================
// STP_w_log_inf           log info

  TX_Print(eTxt);
  LOG_A__ (MSG_ERR_typ_INF, eTxt);
  ++exp_errNr;

  return -1;

}
    
    
//================================================================
  int STP_w_log_err (char *eTxt, ...) {
//================================================================
// STP_w_log_err              log error

  va_list va;
  char    s1[800], s2[400];
  
  sprintf(s2, "%s #%d ",eTxt,stpw_li);

  va_start(va,eTxt);

  vsprintf(s1,s2,va);

  va_end(va);


  // sprintf(s1, "%s #%d",eTxt,stpw_li);
  TX_Print(s1); 
  LOG_A__ (MSG_ERR_typ_ERR, s1);
  ++exp_errNr;

  return -1;
  
} 

 
//================================================================
  void STP_w_ERREX (char *s1) {
//================================================================
 
  printf("******************************************* \n");
  printf("STP_w_ERREX %s\n",s1);

  STP_w_log_err (s1);

  longjmp(jmp1, 1); // jump to last setjmp und give retCode 1

}


// ================ EOF =========================
