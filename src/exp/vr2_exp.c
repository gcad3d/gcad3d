/*
 *
 * Copyright (C) 2020 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
\file  ../APP/VR2_export.c
\brief resolve model

Demo export active group or whole model

\code
=====================================================
List_functions_start:

VR2_exp__             main entry, export into VRLM2 format;
VR2_exp_export        export active group or whole model
VR2_exp_mdl__         export all objs of grp1
VR2_exp_ox_sm_ext     export external-submodel
VR2_exp_ox__          export obj
VR2_exp_int_mnam      fix proto-name internal submodel
VR2_exp_ext_mnam      fix proto-name external submodel
VR2_exp_col_sur       test if surface <dbi> has extra-color
VR2_exp_ori__         get orientation for subModel

VR2_exp_init          write vr-init-file export_init.exp
VR2_exp_w_pta         write coords of group of points
VR2_exp_w_ia          write indexes 
VR2_exp_tess          write tesselated faces
VR2_exp_fac1          write tesselated face
VR2_exp_fac2          write tesselated face
VR2_exp_col__         write color
VR2_exp_tex           write texture
VR2_exp_tex_fn        add texture; Texref-index is <iTex>
VR2_exp_proto_tex     write texture-prototype
VR2_exp_pt            VR2-point from CAD-point
VR2_exp_vc            VR2-vector from CAD-vector
VR2_exp_join          join files to export.exp

List_functions_end:
=====================================================

Build:
. ../options.sh && make -f xa_vr2_exp.mak


WHAT IS:
creaseAngle 1.0  for IndexedFaceSet ?
solid FALSE      for IndexedFaceSet: both sides shaded




//----------------------------------------------------------------
COORDINATES:
     VRML      CAD
     X    =    X                 X-coord unmodified
     Y    =    Z
     Z    =   -Y

     -Y
      |
      |
      |        Z
      o---------
       \
        \
         X

See VR2_exp_ori__ VR2_exp_vc VR2_exp_pt

Transform rotation:
  vcx vcy vcz angle




//----------------------------------------------------------------
FORMAT VRML2:
//----------------------------------------------------------------
#VRML V2.0 utf8

# define geometry of all internal and external submodels
# define default-color for subModel
PROTO mySubModelName_col_0 [] {
  Appearance { material Material { diffuseColor 0.58 0.58 0.68 }}}

PROTO mySubModelName [] { Group { children [
 Shape { geometry Box { size 100 20 50 }}
 Shape { geometry Sphere { radius 60 }}
 Shape { geometry IndexedLineSet { coord Coordinate { point [
  -2 0 -2
  2 0 2
 ]} coordIndex [ 0 1 ]}}
]}}


# define geometry of main-model
# define default-color
PROTO myModelName_col_0 [] {
  Appearance { material Material { diffuseColor 0.58 0.58 0.68 }}}

PROTO myModelName [] { Group { children [
  # geometry in mainModel
  Shape { geometry Box { size 100 20 50 } }
  Shape { appearance col_0 {} geometry IndexedFaceSet {..} }
  # subModel or external subModel
  Transform {
    translation -92.336683 0 29.522613
    rotation 0 1 0 0
    children [ mySubModelName {} ]}
]}}

DEF ROOT Group { children [ myModelName {} ]}

# external-subModel: copy model.wrl into outfile, remove "DEF ROOT Group {..}"


//----------------------------------------------------------------
# Geometry:
# shape can only keep 1 geometry-obj !
Shape {
  appearance ..
  geometry ..
}

# geometry-types:
- Box
- Cone
- Cylinder
- ElevationGrid           x-y-z-surface
- Extrusion              
- IndexedFaceSet
- IndexedLineSet
- PointSet
- Sphere
- Text

//----------------------------------------------------------------
 geometry IndexedLineSet { coord Coordinate { point [
   -1.0 1.0 1.0,
   1.0 1.0 1.0,
   1.0 1.0 -1.0,
   -1.0 1.0 -1.0 ] } coordIndex [

..............................................
 geometry IndexedFaceSet { solid FALSE coord Coordinate { point [
   5 0 0,
   5 0 10,
   -10 -10 0,
   -10 -10 10 ] } coordIndex [
   0,1,2,-1,
   2,1,3,-1 ] } }


..............................................
geometry Box { size 6 4 2 }           # l b h


..............................................
geometry Cylinder { <parts> radius <r> height <h>
parts: side bottom top / FALSE TRUE


..............................................
geometry Cone { <parts> bottomRadius <r> height <h> }
parts: side bottom / FALSE TRUE
  geometry Cone { bottom FALSE }


..............................................
geometry Sphere { radius <r> }

..............................................
Extrusion can be a surface rotated around axis (spine=line)
- crossSection is 2D; can be scaled and rotated (by spine).
- Spine provides direction and twist (needs vectors along and normal to spine)
- scale factor for each spine-position

geometry Extrusion {
    creaseAngle 1.57
    endCap FALSE
    solid  FALSE
    crossSection [
    # Circle
       1.00  0.00,   0.92 -0.38,
       0.71 -0.71,   0.38 -0.92,
       0.00 -1.00,  -0.38 -0.92,
      -0.71 -0.71,  -0.92 -0.38,
      -1.00 -0.00,  -0.92  0.38,
      -0.71  0.71,  -0.38  0.92,
       0.00  1.00,   0.38  0.92,
       0.71  0.71,   0.92  0.38,
       1.00  0.00
    ]
    spine [
    # Straight-line
      0.0 0.0 0.0,  0.0 0.4 0.0,
      0.0 0.8 0.0,  0.0 1.2 0.0,
      0.0 1.6 0.0,  0.0 2.0 0.0,
      0.0 2.4 0.0,  0.0 2.8 0.0,
      0.0 3.2 0.0,  0.0 3.6 0.0,
      0.0 4.0 0.0
    ]
    scale [
      1.8  1.8,  1.95 1.95,
      2.0  2.0,  1.95 1.95
      1.8  1.8,  1.5  1.5
      1.2  1.2,  1.05 1.05,
      1.0  1.0,  1.05  1.05,
      1.15 1.15,
    ]
  }

Extrusion -> Torus:
  geometry Extrusion {
    creaseAngle 1.57
    beginCap FALSE
    endCap   FALSE
    crossSection [
    # Circle
       1.00  0.00,   0.92 -0.38,
       0.71 -0.71,   0.38 -0.92,
       0.00 -1.00,  -0.38 -0.92,
      -0.71 -0.71,  -0.92 -0.38,
      -1.00 -0.00,  -0.92  0.38,
      -0.71  0.71,  -0.38  0.92,
       0.00  1.00,   0.38  0.92,
       0.71  0.71,   0.92  0.38,
       1.00  0.00
    ]
    spine [
    # Circle
       2.00 0.0  0.00,   1.85 0.0 0.77,
       1.41 0.0  1.41,   0.77 0.0 1.85,
       0.00 0.0  2.00,  -0.77 0.0 1.85,
      -1.41 0.0  1.41,  -1.85 0.0 0.77,
      -2.00 0.0  0.00,  -1.85 0.0 -0.77,
      -1.41 0.0 -1.41,  -0.77 0.0 -1.85,
       0.00 0.0 -2.00,   0.77 0.0 -1.85,
       1.41 0.0 -1.41,   1.85 0.0 -0.77,
       2.00 0.0  0.00,
    ]
  }

Extrusion -> Helix:


..............................................
Text { string "0" 
       fontStyle FontStyle { family "SANS" style "BOLD" justify "MIDDLE" size 2 }
     }

DEF MyFont FontStyle { size 0.005 justify "MIDDLE" style "BOLD" }



//----------------------------------------------------------------
# definition of attributs with DEF - USE:
DEF att_sm0 Appearance {
  material Material { #diffuseColor 0.85 0.85 0.20  }
}
Shape {
  appearance USE att_sm0
  geometry ..
}

//----------------------------------------------------------------
# definition of attributs direct:
  appearance Appearance {
    material Material {
      ambientIntensity 0.5
      diffuseColor 0.85 0.85 0.20
      emissiveColor 0.000000  0.000000  0.000000
      specularColor 0.75 0.67 0.34
      shininess 0.3
      transparency  0.000000
    }
  }

//----------------------------------------------------------------
 Damit mehrere Dreiecke (oder Quads) gemeinsam texturiert werden (ohne
  dass man fuer jedesDdreieck eigene texCoord -s angeben muss) -
  muessen alle 3-Ecke im gleichen IndexedFaceSet sein !!!!!

 Grundsaetzlich wird das gesamte Dreieck texturiert (repetitiv !)
  Verschieben, Skalieren und Drehen der Textur mit textureTransform.
  See Textest2.wrl

Texture in Appearance:

DEF c_main Appearance {
 texture ImageTexture {
   url "/mnt/serv1/Devel/dev/gCAD3D/formate/jpg/HausH1_2.JPG"
 }
#textureTransform TextureTransform {
# center -0.3 -0.3
#  scale 0.5 0.5
#  rotation 0
#}
}

Transform { children [
 Shape {
 appearance USE c_main
 geometry IndexedFaceSet { solid FALSE coord Coordinate { point [
 5 0 0,
 5 0 10,
 -10 -10 0,
 -10 -10 10 ] } coordIndex [
 0,1,2,-1,
 2,1,3,-1 ] } }
 Shape {
 appearance USE c_main
 geometry IndexedFaceSet { solid FALSE coord Coordinate { point [
 10 0 0,
 10 10 0,
 30 -15 0,
 30 -5 0 ] } coordIndex [
 0,1,2,-1,
 2,1,3,-1 ] } }
] }


//----------------------------------------------------------------
# f. 2D-text: ?
Billboard {
  axisOfRotation 0 0 0
  children [
    Shape { .. }
  [
}







\endcode *//*----------------------------------------


*/

// #ifdef _MSC_VER
// #include "../xa/MS_Def1.h"
// #endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>              // for ...

// #include <GL/gl.h>                        // GL_TRIANGLES ..
// CANNOT include <GL/gl.h>
// /usr/include/GL/gl.h
#define GL_TRIANGLES        0x0004
#define GL_TRIANGLE_STRIP   0x0005
#define GL_TRIANGLE_FAN     0x0006



#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
__declspec(dllexport) int VR2_exp__ (char*);
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif


#include "../ut/ut_cast.h"                // INT_PTR
#include "../ut/ut_geo.h"                 // Point ...
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_os.h"                  // AP_get_bas_dir ..
#include "../ut/ut_memTab.h"              // MemTab_..
#include "../ut/ut_iTab.h"                // I4Tab
#include "../ut/ut_col.h"                 // COL_DL_ATT
#include "../ut/func_types.h"             // UI_Func...

// #include "../gr/ut_DL.h"               // DB_GetDLatt

#include "../db/ut_DB.h"                  // DB_GetObjGX

#include "../xa/xa.h"                     // MDL_IS_MAIN
#include "../xa/xa_msg.h"                 // ERR_*
#include "../xa/xa_tex.h"                 // Tex_get_fn



//----------------------------------------------------------------
typedef_MemTab(int);



//----------------------------------------------------------------
// PROTOTYPES:
Point VR2_exp_pt (Point *p1);
Vector VR2_exp_vc (Vector *v1);



//----------------------------------------------------------------
// EXTERN VARS:
// ../xa/xa.c:
extern ColRGB     AP_defcol;

// ../gr/ut_GL.c:
// extern ColRGB  GL_defCol;
// extern ColRGB  GL_actCol;




//----------------------------------------------------------------
// LOCAL VARS:
static int  exp_mdli;        // subModelNr; 0-n = sind in Submodel; -1=main    
static int  exp_errNr = 0;
static int  exp_objNr = 0;
static FILE *exp_fpo = NULL;
static char exp_mdlNam[128];
static char exp_sMdlNam[256];
static ColRGB *exp_sCol;



//=========================================================
  int VR2_exp__ (char *sPar) {
//=========================================================
// VR2_exp__             main entry, export into VRLM2 format;
// output is file <tmpDir>export.exp


  // TX_Print(">>>>>>>>> VR2_exp__ <<<<<<<<<<");


  // VR2_exp_test (); goto L_exit;

  VR2_exp_export (); // export the active model

  // gCad_fini ();   // close plugin

  return 0;

}


//================================================================
  int VR2_exp_export () {
//================================================================
// VR2_exp_export           export active group or whole model
 
#define SIZ_SMTAB 1000               // size list subModelnames


  int         irc, i1, iTyp, mbi;
  long        l1;
  char        s1[256];
  void        *pv1;
  ObjDB       *oTab;
  ObjGX       ox1;
  ModelRef    *mr;
  ModelBas    *mbo;

  MemTab(int) grp1 = _MEMTAB_NUL;   // DL-indexes of objs to export
  MemTab(int) smTab = _MEMTAB_NUL;  // basMdl-indexes of used submodels


  exp_errNr = 0;
  exp_objNr = 0;

  // get modelName AP_mod_fnam
  // strcpy(exp_mdlNam, AP_mod_fnam);
  sprintf(s1, "%s_%s", AP_mod_sym, AP_mod_fnam);
  UTX_safeName (s1, 1);
  strcpy(exp_mdlNam, s1);


  // printf("\n \n");
  // printf("exp_export ============================ |%s|\n",exp_mdlNam);


  // remove all old files
  sprintf (s1, "%sexp*.exp",AP_get_tmp_dir());
  OS_file_delGrp (s1);



  MemTab_ini__ (&smTab, sizeof(int), Typ_Int4, SIZ_SMTAB);

  LOG_A_init ("exp_export");



  // prepare ProtoList
  // get nr of basic-textures TexBasNr
  i1 = Tex_tbNr ();
  if(i1 > 0) {
    pv1 = MEM_alloc_tmp ((int)(i1 * sizeof(int)));
    VR2_exp_proto_tex (-i1, pv1);
  }


  // init primary model
  VR2_exp_init ();



  //================================================================
  // process mainModel
  //================================================================
  // get the active modelIndex
  exp_mdli = AP_get_modact_ind ();          // -1 = main, else a subModel is active
  // if(MDL_IS_MAIN) {
     // printf("main is active ..\n");
  // } else {
    // printf("subModel is active ..\n");
  // }


    // printf("\n====================== MAIN ======================= \n");
    // printf("exp_export ============================ %d\n",exp_mdli);

  
  //----------------------------------------------------------------
  // get grp1 = list of objs in DL 
  // get nr of objs in active group
  i1 = Grp_get_nr ();
  if(i1 > 0) {
    // copy objs of active group into private group grp1
    Grp1_add__ (&grp1);

  } else {
    // get all objs in primary-model
    Grp1_add_sm_dl (&grp1, exp_mdli);
  }

    // TESTBLOCK
    // MemTab_dump (&grp1, "exp_export-L1");
    // return -1;
    // END TESTBLOCK


  //----------------------------------------------------------------
  // export all objs of grp1
  VR2_exp_mdl__ (&grp1, &smTab);



  //================================================================
  // process subModels
  //================================================================
  if(smTab.rNr < 1) goto L_finish;

  // store active DB
  DB_save__ ("");

  i1 = 0;

  L_nxt_sm:

    // get baseModelIndex exp_mdli
    MemTab_get (&exp_mdli, &smTab, i1, 1);

    // get modelName from index
    mbo = DB_get_ModBas (exp_mdli);
      // DEB_dump_obj__(Typ_SubModel, mbo, "sm %d",exp_mdli);

      // printf("\n\n================================================== \n");
      // printf(" ------- nxt-sm %d %d |%s|\n",i1,exp_mdli,mbo->mnam);


    if(mbo->typ != MBTYP_INTERN) {
      // get external-modelfile (use existing <model>.wrl
      VR2_exp_ox_sm_ext (mbo->mnam);
      goto L_sm_cont;
    }

    // activate subModel
    DB_load__ (mbo->mnam);

    // get group smTab of all objs of subModel mbi in DL
    Grp1_add_sm_dl (&grp1, exp_mdli);

    // export subModel
    VR2_exp_mdl__ (&grp1, &smTab);

    L_sm_cont:
      ++i1;
      if(i1 < smTab.rNr) goto L_nxt_sm;


  // reload main-DB
  DB_load__ ("");



  //================================================================
  L_finish:
  //================================================================

  // printf("\n\n================================================== \n");
  // printf(" ------- finish\n");


  LOG_A__ (ERR_INF, "nr of errors = %d", exp_errNr);
  LOG_A__ (ERR_INF, "nr of objs exported = %d", exp_objNr);

  LOG_A_exit (exp_errNr);

  MemTab_free (&smTab);
  MemTab_free (&grp1);

    // TESTBLOCK
    // LOG_A_disp ();
    // END TESTBLOCK



  // join all files ext_*.ext" into single file "export.ext
  VR2_exp_join ();


  // OS_file_delGrp (s1);



  return exp_errNr;

}


//=======================================================================
  int VR2_exp_mdl__ (MemTab(int) *grp1, MemTab(int) *smTab) {
//=======================================================================
// VR2_exp_mdl__              export all objs of grp1
//   add all used subModels to smTab

  int         irc, i1, oNr, iTyp, mbi;
  long        dbi, dli, dlNr, l1;
  char        s1[400];
  DL_Att      *dla;
  ObjGX       ox1;
  ModelRef    *mr;
  ModelBas    *mbo;




  // printf(" VR2_exp_mdl__ %d\n",exp_mdli);

  // get modelName from index
  mbo = DB_get_ModBas (exp_mdli);
    // DEB_dump_obj__(Typ_SubModel, mbo, "sm %d",exp_mdli);



  // open outfile
  if(exp_mdli < 0) {
    exp_sMdlNam[0] = '\0';  // MainModel = empty name
  } else {
    // external-subModel can have symbolic directory; remove
    strcpy(exp_sMdlNam, mbo->mnam);
    UTX_ftyp_cut (exp_sMdlNam);
    UTX_safeName (exp_sMdlNam, 1);
  }


  // set s1 = filename for temp.outfile
  sprintf(s1, "%sexp_%s.exp",AP_get_tmp_dir(),exp_sMdlNam);
  if((exp_fpo = fopen (s1, "w")) == NULL) {
    TX_Error ("VR2_exp_mdl__ open file %s",s1);
    return -1;
  }


  // init mainModel
  if(exp_mdli < 0) {
    fprintf(exp_fpo,"\n#<<<<<<<<<<<<<< Beg. mainModel\n");
    // fprintf(exp_fpo,"DEF ROOT Group { children [\n");
    fprintf(exp_fpo,"PROTO %s [] { Group { children [\n",exp_mdlNam);

  } else {
    // init subModel
    fprintf(exp_fpo,"\n#<<<<<<<<<<<<<< Beg. subModel %s\n",mbo->mnam);
    fprintf(exp_fpo, "PROTO %s_%s [] { Group { children [\n",
            exp_mdlNam,mbo->mnam);
    // fprintf(exp_fpo, "PROTO %s_%s [\n",exp_mdlNam,mbo->mnam);
    // fprintf(exp_fpo, "  field SFVec3f pInspoint 0 0 0\n");
    // fprintf(exp_fpo, "  field SFRotation pRotation 0 1 0 0\n");
    // fprintf(exp_fpo, " ] { Transform {\n");
    // fprintf(exp_fpo, "  translation IS pInspoint rotation IS pRotation children [\n");
  }


  //----------------------------------------------------------------
  // export objs
  oNr = grp1->rNr;
  dlNr = DL_get__ (&dla);

  for(i1=0; i1<oNr; ++i1) {
      // printf("\n ------- nxt-obj %d\n",i1);

    dli = *((int*)MEMTAB__ (grp1, i1));
    iTyp = dla[dli].typ;

    // skip this types:
    if(iTyp == Typ_PT) continue;
    if(iTyp == Typ_Ditto) continue;

    dbi = dla[dli].ind;

    // get DB-obj
    ox1 = DB_GetObjGX (iTyp, dbi);
    if(ox1.typ == Typ_Error) {
      LOG_A__ (ERR_ERR, "exp_export typ=%d dbi=%ld",iTyp,dbi);
      ++exp_errNr;
      continue;
    }

    // Typ_Model/Typ_Mock: add to list of subModels
    if((ox1.typ == Typ_Model)||(ox1.typ == Typ_Mock)) {
      mr = ox1.data;
      mbi = mr->modNr;
      // add blockName to list of subModels
      MemTab_uniq_sav (smTab, &l1, &mbi);
    }

    // export DB-obj
    irc = VR2_exp_ox__ (&ox1, dbi);
    if(!irc) ++exp_objNr;
  }


  //----------------------------------------------------------------
  // close outfile
  if(exp_mdli < 0) {
    fprintf(exp_fpo,"]}}\n");
    fprintf(exp_fpo,"\nDEF ROOT Group { children [ %s {} ]}\n\n",exp_mdlNam);
    fprintf(exp_fpo, "# EOF\n");

  } else {
    fprintf(exp_fpo,"# End subModel\n");
    fprintf(exp_fpo,"]}}\n");
  }
  fclose(exp_fpo);
  exp_fpo = NULL;


  return 0;

}


//=====================================================================
  int VR2_exp_ox__ (ObjGX *ox1, long dbi) {
//=====================================================================
// VR2_exp_ox__                 export obj
// typ ?

  int      irc, i1, i2, i3, i4, ii, oNr, form, ssTyp, att;
  char     s1[128], s2[128], s3[256];
  double   q1[4];
  void     *obj;
  Point    pt1, *pta;
  ObjGX    *vTab = NULL;
  ModelBas *mbo;
  ModelRef *mro;
  ColRGB   col1;


  // printf("VR2_exp_ox__ ox1-typ=%d ox1-form=%d dbi=%ld\n",
         // ox1->typ,ox1->form,dbi);
  // DEB_dump_ox_s_ (ox1, "");
  // DEB_dump_ox__ (ox1, "");


  obj = ox1->data;
  form = ox1->form;


  //----------------------------------------------------------------
  if(TYP_IS_SUR(ox1->typ)) {

    // get color for surf
    irc = VR2_exp_col_sur (&col1, dbi);
    if(!irc)  exp_sCol = &col1;
    else      exp_sCol = NULL;
    // att = INT32I_COL (&col1);
    
    // start tesselation-mode store
    irc = TSU_Init (1, &vTab);
    if(irc < 0) {TX_Error("VR2_exp_ox__ E1"); return -1;}

    att = 0;

    // tesselate into buffer vTab
    irc = TSU_SUR__ (ox1, att, dbi);

    // reset tesselation; get accumulated Vertices (of MainModel)
    oNr = TSU_Init (0, &vTab);
      // printf(" VR2_exp_ox__-tess-oNr=%d\n",oNr);

    // write faces
    if(!irc) VR2_exp_tess (vTab);

    // free memspace
    TSU_Init (5, &vTab); 
      
    goto L_exit;
  }


  //----------------------------------------------------------------
// test TYP_IS_CV ?
  switch (form) {

    //----------------------------------------------------------------
    case Typ_LN:
      fprintf(exp_fpo, 
        " Shape { geometry IndexedLineSet { coord Coordinate { point [\n");

      VR2_exp_w_pta (&((Line*)obj)->p1, 1);   // write points
      VR2_exp_w_pta (&((Line*)obj)->p2, 1);   // write points
      
      fprintf(exp_fpo, " ]} coordIndex [ 0 1 ]}}\n");

      break;


    //----------------------------------------------------------------
    case Typ_CI:
    case Typ_CVPOL:
    case Typ_CVBSP:
    case Typ_CVELL:
    case Typ_CVCLOT:
    case Typ_CVTRM:
      // get polygon from PRCV
      irc = PRCV_npt_dbo__ (&pta, &oNr, form, dbi, exp_mdli);
        // DEB_dump_nobj__ (Typ_PT, oNr, pta, "VR2_exp_ox__ pta");

      fprintf(exp_fpo, 
        " Shape { geometry IndexedLineSet { coord Coordinate { point [\n");
      VR2_exp_w_pta (pta, oNr);   // write points
      fprintf(exp_fpo, " ]} coordIndex [\n");
      i1 = 0;  // startindex
      VR2_exp_w_ia (&i1, oNr, 1);
      fprintf(exp_fpo, " ]}}\n");
      break;


    //----------------------------------------------------------------
    case Typ_Model:
      mro = (ModelRef*)obj;
      // get mbo = its basicModel
      mbo = DB_get_ModBas (mro->modNr);
      // internal or external model
      if(mbo->typ == MBTYP_INTERN) {
        // create modelname internal-submodel
        // sprintf(s3, "%s_%s",exp_mdlNam,mbo->mnam);
        VR2_exp_int_mnam (s3, mbo->mnam);
      } else {
        // MBTYP_EXTERN - export external-submodel
        VR2_exp_ext_mnam (s3, mbo->mnam);
      }
      // get s1 = position
      s1[0] = '\0';
      pt1 = VR2_exp_pt (&mro->po);
      UTX_add_fl_ua (s1, (double*)&pt1, 3, ' ');
      // get S4 = orientation
      VR2_exp_ori__ (q1, &mro->vx, &mro->vz);
      s2[0] = '\0';
      UTX_add_fl_ua (s2, q1, 4, ' ');
      // fprintf(exp_fpo, " %s { pInspoint %s pRotation %s }\n",s3,s1,s2);
      fprintf(exp_fpo, " Transform {\n");
      fprintf(exp_fpo, "  translation %s\n",s1);
      fprintf(exp_fpo, "  rotation %s\n",s2);
      fprintf(exp_fpo, "  children [ %s {} ]}\n",s3);
      break;


    //=========================================================
    default:
        printf("  VR2_exp_ox__ skip form=%d dbi=%ld\n",ox1->form,dbi);
      return -1;

  }



  L_exit:

    // printf(" ex-VR2_exp_ox__ irc=%d\n",irc);

  return 0;

}


//================================================================
  int VR2_exp_int_mnam (char *so, char *mnam) {
//================================================================
 
  sprintf(so, "%s_%s", exp_mdlNam, mnam);
  // UTX_safeName (so, 1);

  return 0;

}


//================================================================
  int VR2_exp_ext_mnam (char *so, char *mnam) {
//================================================================

  strcpy(so, mnam);
  UTX_ftyp_cut (so);
  UTX_safeName (so, 1);

  return 0;

}


//================================================================
  int VR2_exp_ox_sm_ext (char *mnam) {
//================================================================
// VR2_exp_ox_sm_ext                  export external-submodel
// Input:
//   fno      symbolic filename of model
// Output:
//   fno      filename-out, size max 256

  int        irc;
  char       s1[400], s2[256];
  stru_FN stNam;
  FILE       *fpo, *fpi;
 

  // printf(".... VR2_exp_ox_sm_ext |%s|\n",mnam);


  // make outfilname <tmpDir>/exp_<modelName>.exp
  VR2_exp_ext_mnam (s2, mnam);
  sprintf(s1, "%sexp_%s.exp",AP_get_tmp_dir(),s2);
    // printf(" ox_sm_ext-L1 |%s|\n",s1);


  if((fpo = fopen (s1, "w")) == NULL) {
    TX_Error ("VR2_exp_ox_sm_ext open file %s",s1);
    return -1;
  }

  fprintf(fpo, "#<<<<<<<<<<<<<< Beg. extern subModel %s\n",mnam);


// TODO: use MDLFN_get_fnAbs
  // decode mnam - get dir, fnam, ftyp
  irc = MDLFN_oFn_fNam (&stNam, mnam);
  if(irc < 0) {TX_Error("VR2_exp_ox_sm_ext E1 %d",irc); irc  = -1; goto L_exit;}
    // MDLFN_dump_ofn (&stNam, "ox_sm_ext-L2");

  // get full filename of external modelfile - type wrl
  sprintf(s1, "%s%s.wrl",stNam.fDir,stNam.fNam);
    // printf(" ox_sm_ext-L3 |%s|\n",s1);


  // test if file exists
  irc = OS_checkFilExist (s1, 2);
  if(!irc) {
    // sprintf(s1," for export external submodel export model \"%s\" before",mnam);
    TX_Print(s1);
    LOG_A__ (ERR_ERR, s1);
    ++exp_errNr;
    irc = -1;
    goto L_exit;
  }


  // copy model.wrl into outfile
  // skip first 2 lines, remove all following "DEF ROOT Group "
  if((fpi = fopen (s1, "r")) == NULL)
    {TX_Error("VR2_exp_ox_sm_ext E2"); irc  = -1; goto L_exit;}

  // skip first 2 lines
  fgets (s1, 250, fpi);
  fgets (s1, 250, fpi);
  
  while(!feof (fpi)) {
    if(fgets (s1, 250, fpi) == NULL) break;
    // UTX_CleanCR (s1);    // cut off CR,LF 
      // printf("%s\n",cBuf);
    
    if(!strncmp(s1, "DEF ROOT Group ",15)) break;
    fprintf (fpo, "%s", s1);
  }

  fclose(fpi);


  irc = 0;

  L_exit:
  fclose(fpo);

  return irc;

}


//================================================================
  int VR2_exp_w_pta (Point *pta, int ptNr) {
//================================================================
// VR2_exp_w_pta          write icoords of group of points

  int    i1;
  char   s1[128];
  Point  pt1;


  for(i1=0; i1<ptNr; ++i1) {
    s1[0] = '\0';
    pt1 = VR2_exp_pt (&pta[i1]);
    UTX_add_fl_ua (s1, (double*)&pt1, 3, ' ');
    fprintf(exp_fpo, "  %s\n",s1);
  }

  return 0;

}

//================================================================
  int VR2_exp_w_ia (int *ia, int iNr, int mode) {
//================================================================
// VR2_exp_w_ia           write indexes 
//   write groups of 10 numbers
// Input:
//   ia       indexes to write out
//   mode     0 = write out iNr numbers from ia
//            1 = write iNr consecutive numbers; start with int in ia[0];

  int    i1, i2, i3, i4, ii;
  char   s1[128];
  Point  pt1;

  // printf("VR2_exp_w_ia ia[0]=%d iNr=%d mode=%d\n",ia[0],iNr,mode);

  i2 = 0;
  i3 = 0;

  L_nxt_ind:
  if(i3 >= iNr) return 0;
  ii = 0;
  for(i1=0; i1<10; ++i1) {
    i3 = i1 + i2;
    if(i3 >= iNr) break;
    i4 = sprintf(&s1[ii], "%d ", i3);
    ii += i4;
  }
  s1[ii] = '\0';
  fprintf(exp_fpo, "  %s\n",s1);
  i2 += 10;
  goto L_nxt_ind;

}


//================================================================
  int VR2_exp_init () {
//================================================================

  float       f1, f2, f3;
  char        s1[256];
 
  sprintf(s1, "%sexport_init.exp",AP_get_tmp_dir());
  if((exp_fpo = fopen (s1, "w")) == NULL) {
    TX_Error ("VR2_exp_init open file %s",s1);
    return -1;
  }


  fprintf(exp_fpo, "#VRML V2.0 utf8\n");
  fprintf(exp_fpo,"# gCAD3D %s\n",OS_date1());
  // fprintf(fpo,"# Model %s\n",AP_mod_fnam);


  // create defCol -PROTO
  f1 = AP_defcol.cr; f1 /= 255;
  f2 = AP_defcol.cg; f2 /= 255;
  f3 = AP_defcol.cb; f3 /= 255;
  fprintf(exp_fpo, "\nPROTO %s_col_0 [] { Appearance { material Material {\n",
          exp_mdlNam);
  fprintf(exp_fpo, " diffuseColor %.2f %.2f %.2f }}}\n", f1,f2,f3);
  // fprintf(exp_fpo, " emissiveColor %.2f %.2f %.2f }}}\n", f1,f2,f3);


  fclose(exp_fpo);
  exp_fpo = NULL;


  return 0;

}


//================================================================
   int VR2_exp_tess (ObjGX *oxi) {
//================================================================
// VR2_exp_tess                  write tesselated faces
// see also TSU_exp_sur

  int     irc, rSiz;


  // printf("VR2_exp_tess typ=%d form=%d siz=%d\n",
         // oxi->typ,oxi->form,oxi->siz);

  L_next:
  if(oxi->typ  == Typ_Done)  goto L_exit;

  // first record must be size of following Record
  if(oxi->typ  != Typ_Size) goto L_Err2;
  rSiz = (long)oxi->data;
  // printf("Record %d size=%d\n",i1,rSiz);

  ++oxi;  //(char*)oxi += sizeof(ObjGX);

  // export
  VR2_exp_fac1 (oxi);

  // (char*)oxi += rSiz;
  oxi = (ObjGX*)((char*)oxi + rSiz);
  // ++i1;
  goto L_next;



  L_exit:

  return 0;

  L_Err2:
    TX_Error("VR2_exp_tess E002 typ %d",oxi->typ);
    return -1;

}
      

//================================================================
  int VR2_exp_fac1 (ObjGX *oxi) {
//================================================================
// VR2_exp_fac1          write tesselated face
// see also TSU_ntria_bMsh_p GL_set_bMsh
// was TSU_exp_fac

  int     i1, i2, i1Nr, i2Nr, iTyp;
  int     iTex;
  ColRGB  *sCol;
  ObjGX   *oxs;


  // printf("\nVR2_exp_fac1 %d %d %d\n",oxi->typ,oxi->form,oxi->siz);
  // DEB_dump_ox_s_ (oxi, "VR2_exp_fac1");
  // DEB_dump_ox_0 (oxi, "TSU_exp_fac");

  i1Nr = oxi->siz;              // ex Sur  211 143 5
   // printf(" exp_fac1-i1Nr=%d\n",i1Nr);

  iTex = -1;
  sCol = exp_sCol;
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
        VR2_exp_fac2 (oxs, sCol, iTex); // VR2
      }
      ++oxs;  //(char*)oxs += sizeof(ObjGX);
    }

    ++oxi;  //(char*)oxi += sizeof(ObjGX);

  }


  return 0;

}


//================================================================
  int VR2_exp_fac2 (ObjGX *oxi, ColRGB *sCol, int iTex) {
//================================================================
// was TSU_exp_wrl2Fac

  int    i0, i1, i2, i3, ie;
  Point  *pTab, p1;
  char   cbuf[128];
  

  // printf("VR2_exp_fac2 iTex=%d\n",iTex);


  fprintf(exp_fpo," Shape {\n");

  // if(sCol != NULL) {
  if(iTex < 0) {
    VR2_exp_col__ (sCol);

  } else {
    VR2_exp_tex (iTex);
  }


  // write faces ..
  fprintf(exp_fpo,
    "  geometry IndexedFaceSet { solid FALSE coord Coordinate { point [\n");
    //, cbuf);

  ie   = oxi->siz;  // Anzahl von Punkten 
  pTab = oxi->data;

  // export points
  VR2_exp_w_pta (pTab, ie);
  // letzter Punkt nicht notwendig.


  // textures: write TextureCoordinate-table
  if(iTex < 0) goto L_ci;
  TSU_exp_wrl2Tex (pTab, ie, iTex);


  // start coordIndextable ..
  L_ci:
  fprintf(exp_fpo,"  ]} coordIndex [\n");


  //================================================================
  // TRIANGLE_FAN:
  if(oxi->aux != GL_TRIANGLE_FAN) goto L_GLNPF;
  // printf("fffffffffffffffffffffffff fan fffffffffffffffff %d\n",ie);

  // die ersten 4
  i0 = 0;

  fprintf(exp_fpo,"  %d",i0);
    // printf(" %d",i0);
  ++i0;

  L_f_nxt:

    fprintf(exp_fpo,",%d",i0);
      // printf(",%d",i0);
    ++i0;
    if(i0 < ie) goto L_f_nxt;

  fprintf(exp_fpo,",-1 ] } }\n");
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

    if(i1 < (ie-2)) fprintf(exp_fpo,"  0,%d,%d,-1,\n",i1,i1+1);
    else        fprintf(exp_fpo,"  0,%d,%d,-1 ] } }\n",i1,i1+1);
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

  if(i3 < (ie-1)) fprintf(exp_fpo,"  %d,%d,%d,-1,\n",i1,i2,i3);
  else        fprintf(exp_fpo,"  %d,%d,%d,-1 ] } }\n",i1,i2,i3);
  // if(i3 < (ie-1)) fprintf(exp_fpo," %d,%d,%d,-1,\n",i1,i3,i2);
  // else        fprintf(exp_fpo," %d,%d,%d,-1 ] } }\n",i1,i3,i2);
    // printf(" facOut %d,%d,%d,-1\n",i1,i2,i3);


  i1 = i3;
  // i2 bleibt
  ++i3;           // 3 2 4
  if(i3 >= ie) goto L_fertig;


  if(i3 < (ie-1)) fprintf(exp_fpo,"  %d,%d,%d,-1,\n",i1,i2,i3);
  else        fprintf(exp_fpo,"  %d,%d,%d,-1 ] } }\n",i1,i2,i3);
  // if(i3 < (ie-1)) fprintf(exp_fpo," %d,%d,%d,-1,\n",i1,i3,i2);
  // else        fprintf(exp_fpo," %d,%d,%d,-1 ] } }\n",i1,i3,i2);
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

    if((i3+3) < ie) fprintf(exp_fpo,"  %d,%d,%d,-1,\n",i1,i2,i3);
    else        fprintf(exp_fpo,"  %d,%d,%d,-1 ] } }\n",i1,i2,i3);
      // printf(" %d,%d,%d,-1\n",i1,i2,i3);

    if((i3+3) < ie) goto L_t_nxt;




  //================================================================
  L_fertig:
  // TSU_ptNr = ie;
  // ++TSU_facNr;
  return 0;

}


//================================================================
  int VR2_exp_col_sur (ColRGB *col1, long dbi) {
//================================================================
// VR2_exp_col_sur          test if surface <dbi> has extra-color
//  retCod    1  no GA-Record exists
//            2  no color exists
//            0  OK, col1 as color set


  int     irc;
  ObjAtt  *gat1;
  
  irc = GA_get_dbo (&gat1, Typ_SUR, dbi);
  if(irc) goto L_exit;  // no attr exists

  *col1 =  *(COL_DL_ATT (gat1));

  if(!col1->color) {irc = 2; goto L_exit;}

  irc = 0;


  L_exit:
    // if(!irc) UTcol_dump (col1, "ex-VR2_exp_col_sur");
  return irc;

}


//================================================================
  int VR2_exp_col__ (ColRGB  *newCol) {
//================================================================

  float  f1, f2, f3;
  char   cBuf[128];


  if(newCol == NULL) {
      // printf(" defCol !\n");
    fprintf(exp_fpo,"  appearance %s_col_0 {}\n",exp_mdlNam);
    return 0;
  }


  // DEB_dump_obj__ (Typ_Color, newCol, "VR2_exp_col__ ");

  f1 = newCol->cr;
  f1 /= 255;
  f2 = newCol->cg;
  f2 /= 255;
  f3 = newCol->cb;
  f3 /= 255;
    // printf(" col %f %f %f\n",f1,f2,f3);
    // fprintf(exp_fpo,"# TSU_exp_wrlCol %.2f %.2f %.2f\n",f1,f2,f3);


  fprintf(exp_fpo,"  appearance Appearance{material Material{");
  sprintf(cBuf, " diffuseColor %.2f %.2f %.2f ",f1,f2,f3);

  if(newCol->vtra != 0) {
    if(newCol->vtra == 1) strcat(cBuf, "transparency 0.6 ");
    else                  strcat(cBuf, "transparency 0.8 ");
  }

  fprintf(exp_fpo,"%s}}\n",cBuf);  //,f1,f2,f3);

  return 0;

}


//================================================================
  int VR2_exp_tex (int iTex) {
//================================================================
// add texture; Texref-index is <iTex>

  int    irc, i1;
  char   *p1, s1[300], s2[300], s3[300];

  // sprintf(cBuf, "Material { diffuseColor %.2f %.2f %.2f ",f1,f2,f3);

  // irc = TSU_exp_wrlInit (2, iTex);

 // create a texture-proto;
    // iTex = TexRefNr; get TexbasNr.
    i1 = Tex_itb_itr (iTex);
      printf(" ibas=%d\n",i1);

    // test if texture <iTex> already worked
    irc = VR2_exp_proto_tex (i1, NULL);
    if(irc > 0) return i1;   // proto already exists


    // get filename of TexRef[<iTex>]
    p1 = Tex_get_fn (iTex);
    if(p1 == NULL) return -1;

    // make filename absolut (expand symbol)
    irc = MDLFN_ffNam_fNam (s1, p1);
    if(irc < 0) return -1;
      printf(" fn-abs |%s|\n",s1);

    // get outputdirectory
    strcpy(s3, "TSU_fnam");               // TODO
      printf(" outfn |%s|\n",s3);

    // directory from filename
    UTX_fdir_s (s2, s3);
      printf(" outdir |%s|\n",s2);

    // make filename relativ          TODO: ev into prolog-file ???
    UTX_fnam_abs2rel (s3, s1, s2);
      printf(" fn-rel |%s|\n",s3);
    fprintf(exp_fpo,"PROTO tex_%d [] {Appearance{",i1);
    fprintf(exp_fpo,"texture ImageTexture{url \"%s\"}}}\n",s3);

    // return i1;  // returns TexBas-Nr


  fprintf(exp_fpo, " appearance tex_%d {}\n", i1);

  return 0;

}


//================================================================
  int VR2_exp_tex_fn (int iTex) {
//================================================================
// add texture; Texref-index is <iTex>

//   int  irc;

  // sprintf(cBuf, "Material { diffuseColor %.2f %.2f %.2f ",f1,f2,f3);

  // irc = TSU_exp_wrlInit (2, iTex);
//   } else if(mode == 2) { // create a texture-proto;
//     // ii = TexRefNr; get TexbasNr.
//     i1 = Tex_itb_itr (ii);
//       printf(" ibas=%d\n",i1);
//     // test if texture <ii> already worked
//     irc = TSU_exp_wrlProto (i1, NULL);
//     if(irc > 0) return i1;   // proto already exists
//     // get filename of TexRef[<ii>]
//     p1 = Tex_get_fn (ii);
//     if(p1 == NULL) return -1;
//     // make filename absolut (expand symbol)
//     irc = Mod_get_path (memspc011, p1);
//     if(irc < 0) return -1;
//       printf(" fn-abs |%s|\n",memspc011);
//     // get outputdirectory
//     strcpy(memspc012, TSU_fnam);
//       printf(" outfn |%s|\n",memspc012);
//     // directory from filename
//     UTX_fdir_s (s2, memspc012);
//       printf(" outdir |%s|\n",s2);
//     // make filename relativ
//     UTX_fnam_abs2rel (s1, memspc011, s2);
//       printf(" fn-rel |%s|\n",s1);
//     fprintf(fpo,"PROTO tex_%d [] {Appearance{",i1);
//     fprintf(fpo,"texture ImageTexture{url \"%s\"}}}\n",s1);


//   fprintf(exp_fpo, " appearance tex_%d {}\n", irc);

  return 0;

}



//================================================================
  int VR2_exp_proto_tex (int pnr, void *vp) {
//================================================================
// save protoNr pnr; if it is new & uniq: return 0;
// else return -1
// pnr negativ: init for -pnr ints. vp is fixed space (in).
// pnr >= 0:    add pnr to vp==iTab1 if uniq
// RetCode: 0=new rec added; 1=pnr already exists

static I4Tab_NEW  (iTab1);         // init integer-table

  int  i1, irc;


  // printf("VR2_exp_proto_tex %d\n",pnr);


  if(pnr < 0) {   // init
    i1 = sizeof(int) * -pnr;
    I4Tab_init_spc (&iTab1, vp, i1);
    return 0;
  }

  // test if pnr is uniq
  return I4Tab_add_uniq (&i1, &iTab1, pnr);

}


//================================================================
  int VR2_exp_ori__ (double q1[4], Vector *vx, Vector *vz) {
//================================================================
// VR2_exp_ori__                   get orientation for subModel
// was TSU_exp_wrl2Mat

  double   ar;
  Vector   vcr1, vcr2;


  // printf("VR2_exp_ori__ -------------------------------\n");
  // DEB_dump_obj__ (Typ_VC, vx, "exp_ori__-vx");
  // DEB_dump_obj__ (Typ_VC, vz, "exp_ori__-vz");

  // make quat from vx, vz
  UQT_qt_vxvz (q1, vx, vz);
    // printf(" VR2_exp_ori__ %f %f %f %f\n",q1[0],q1[1],q1[2],q1[3]);

  // quat --> Vec + Angle
  UQT_vcar_qt (&vcr1, &ar, q1);
    // DEB_dump_obj__ (Typ_VC, &vcr1, "exp_ori__-vcr1");
    // printf(" exp_ori__-ang=%f\n",ar);

  // if(fabs(ar) < UT_TOL_min1) return 0;

  // cad -> VR2
  vcr2 = VR2_exp_vc (&vcr1);
    // DEB_dump_obj__ (Typ_VC, &vcr2, "exp_ori__-vcr2");

  // set axis and angle
  q1[0] = vcr2.dx;
  q1[1] = vcr2.dy;
  q1[2] = vcr2.dz;
  q1[3] = ar;

  return 0;

}


//================================================================
  Vector VR2_exp_vc (Vector *v1) {
//================================================================
// translate vecs CAD -> VR

  Vector   v2;

  v2.dx = v1->dx;
  v2.dy = v1->dz;
  v2.dz = -v1->dy;

  return v2;

}

//================================================================
  Point VR2_exp_pt (Point *p1) {
//================================================================
// translate points CAD -> VR

  Point   p2;

  p2.x = p1->x;
  p2.y = p1->z;
  p2.z = -p1->y;

  return p2;

}


//================================================================
  int VR2_exp_join () {
//================================================================
// VR2_exp_join                    join files to export.exp
// make list of files: 
// - add initfiles (export_xx.exp)
// - add all files exp_*.exp
//
// see also DXFW_cat_file


  int    irc;
  char   fn1[256], fn2[256];
  FILE   *fpi, *fpo;


  printf("VR2_exp_join --------------------------------\n");

  sprintf(fn1, "%sexport.exp",AP_get_tmp_dir());

  // try to open outfile
  if((fpo=fopen(fn1, "w")) == NULL) {
    TX_Print("VR2_exp_join E001 %s",fn1);
    return -1;
  }


  //----------------------------------------------------------------
  // add initfile "export_init.exp"
  sprintf(fn1, "%sexport_init.exp",AP_get_tmp_dir());
    printf(" VR2_exp_join-init-add |%s|\n",fn1);
  VR2_cat_file (fpo, fn1);


  //----------------------------------------------------------------
  // add all subModels
  // get list of subModels
  sprintf(fn2, "%sexport_smLst.exp",AP_get_tmp_dir());
  irc = UTX_dir_listf (fn2, AP_get_tmp_dir(), "exp_", ".exp");
  if(irc < 0) {TX_Error("VR2_exp_join E001"); irc = -1; goto L_exit; }


  if((fpi=fopen(fn2,"r")) == NULL) {
    TX_Error("VR2_exp_join E002 %s", fn2);
    goto L_exit;
  }

  while (!feof (fpi)) {
    if (fgets (fn1, 250, fpi) == NULL) break;
    UTX_CleanCR (fn1);
    if(!strcmp (fn1, "exp_.exp")) continue;  // skip main
    sprintf(fn2, "%s%s",AP_get_tmp_dir(),fn1);
      printf(" VR2_exp_join-add |%s|\n",fn2);
    VR2_cat_file (fpo, fn2);
  }

  irc = 0;


  //----------------------------------------------------------------
  // add primary file
  sprintf(fn1, "%sexp_.exp",AP_get_tmp_dir());
    printf(" VR2_exp_join-cat |%s|\n",fn1);
  VR2_cat_file (fpo, fn1);


  //----------------------------------------------------------------
  L_exit:
  fclose (fpo);

    printf(" ex-VR2_exp_join %d\n",irc);

//   // rename
//   OS_file_delete (fnamO);
//   rename (tempFilNam, fnamO);

  return irc;


}

 
//==========================================================================
  int VR2_cat_file (FILE *fpo, char *fnam) {
//==========================================================================
// UTX_cat_file           add file into open fileunit
// add file 
// ATTENTION:
// MICROSOFT-BUG: you may not write into a file opened in dll with core-function
//  is identic with core func UTX_cat_file

#define bSiz 16384

  int     i1, i2, rSiz, rTot;
  long    fSiz;
  char    *fBuf;
  div_t   div1;
  FILE    *fpi;


  // printf("UTX_cat_file |%s|\n",fnam);


  fSiz = OS_FilSiz (fnam);


  div1 = div (fSiz, bSiz);
    // printf(" _cat_file %d %d\n",div1.quot,div1.rem);

  // get spc
  fBuf = MEM_alloc_tmp (bSiz + 32);

  if((fpi = fopen(fnam, "rb")) == NULL) return -1;


  if(div1.quot > 0) {
    for(i1=0; i1<div1.quot; ++i1) {
      // read into fBuf
      fread(fBuf, 1, bSiz, fpi);
      // write fBuf
      fwrite(fBuf, 1, bSiz, fpo);
    }
  }

  // write rest
  if(div1.rem > 0) {
    // read into fBuf
    fread(fBuf, 1, div1.rem, fpi);
    // write fBuf
    fwrite(fBuf, 1, div1.rem, fpo);
  }

  fclose(fpi);

    // printf("ex UTX_cat_file\n");

  return 0;

}



// //================================================================
//   int VR2_exp_test () {
// //================================================================
//  
// 
// 
// 
//   return 0;
// 
// }


//======================== EOF ======================
