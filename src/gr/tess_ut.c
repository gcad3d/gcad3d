//    tess_ut.c                              Reiter Franz 2005-03-26
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
 VRML2: 
   Externe Modelcalls sollten ev via EXTERNPROTO gehen !  See below.

 Damit mehrere Dreiecke (oder Quads) gemeinsam texturiert werden (ohne
  dass man fuer jedesDdreieck eigene texCoord -s angeben muss) -
  muessen alle 3-Ecke im gleichen IndexedFaceSet sein !!!!!

 Grundsaetzlich wird das gesamte Dreieck texturiert (repetitiv !)
  Verschieben, Skalieren und Drehen der Textur mit textureTransform.
  See Textest2.wrl

  ..

-----------------------------------------------------
Modifications:
2013-08-19 TSU_exp_objFac orientation for obj-exp. korr. RF.

-----------------------------------------------------
*/
/*!
\file  ../gr/tess_ut.c
\brief write STL, OBJ, WRl files
\code
=====================================================
List_functions_start:

TSU_exp__            main
TSU_exp_sm_sur       export all faces from Mockup-struct
TSU_exp_sm__         load sm.tess, transform, write
TSU_exp_init
TSU_exp_close
TSU_exp_sur
TSU_exp_Open         start open subModel or mainModel
TSU_exp_Mod          export subModel-Call (Ditto)
TSU_exp_Ditto
TSU_exp_fac          wr faces
TSU_exp_stlFac       wr faces stl
TSU_exp_stl1Fac      wr face stl
TSU_exp_objFac       wr face obj
TSU_exp_wrl1Mat      wr mat VRML1
TSU_exp_wrl2Mat      wr mat VRML2
TSU_exp_wrl1Fac      wr faces for VRML1
TSU_exp_wrl2Fac      wr faces for VRML2
TSU_exp_wrl2Col      wr color VRML2
TSU_exp_wrlCol       wr appearance/color VRML
TSU_exp_dxfFac       wr faces dxf
TSU_exp_dxf1Fac      wr face dxf
TSU_exp_Defaults     wr default-colors

TSU_imp_tess         import Mockup from file into Mockup-struct

List_functions_end:
=====================================================

\endcode *//*----------------------------------------





Funktionsweise:
TSU_store  kopiert die Vertices in den Buffer.
GLT_start__, GLT_spp_pp GLT_spp_ 
DemoPlugin_Create.c / cre_8 zeigt creation of Typ_GL_Sur.


RausSchreiben binaer (als "Save Mockup"):
 man muesste noch die Data-Referenzen von absolut auf relativ umstellen koennen.
 (relocaten ..)
 Mit Funktion ObjWrite / ObjRead ?


---------------------------------------------------
Ablauf export VRML2:
Zuerst werden alle subModels nur in Files ausgegeben; (Mod_load_allsm ..)
  wenn Dittos in den subModels, wird (beim ersten Dittoaufruf) das
  File reinkopiert.
Dann wird das main abgearbeitet (ED_work_CurSet);
  hier werden Dittos aufgerufen (GR_DrawModel);
  beim ersten Aufruf muss das File eingefuegt werden.

ED_work_END
 Mod_load_allsm
  Mod_load_sm
   TSU_exp_Open(<subModelName>); // open file for next subModel
    TSU_exp_init        X  Init Modelfile
    TSU_exp_sur         -
     TSU_exp_fac        -
       TSU_exp_wrl*Fac  X
         TSU_exp_wrlCol X  Color
    TSU_exp_close       X  Close Modelfile; wird aber auch von TSU_exp_
 ED_work_CurSet
  TSU_exp_Open ("");             // start Main
  ..
   GR_DrawModel
    TSU_exp_Mod
     TSU_exp_Ditto      X        // copy file
      TSU_exp_wrl*Mat   X  PosMatrix for ditto;



===========================================================================
VRML2:
/mnt/win_d/dev/gCAD3D/formate/vrml/doc/



#VRML V2.0 utf8

Transform { children [

# DefaulColor Main
DEF c_main Appearance { material Material {
 diffuseColor 0.91 0.39 0.19  } }

# DefaulColor Main
DEF c_Submodel1 Appearance { material Material {
 diffuseColor 0.91 0.39 0.19  } }

Transform {                      # 1. occurence of subModel Submodel1
  translation 50 0 0             # Position des 1. Ditto
  children  DEF Submodel1 Group { children [
    Shape {
      appearance Appearance { material Material {
          diffuseColor .6 .6 1
          transparency 0.25          # auch ohne ..
      } }
      geometry IndexedFaceSet {
        solid FALSE
        coord Coordinate { point [ .. ] }
        coordIndex [ .. ]
      }
    }
    Shape {
      appearance USE c_Submodel1
      .. }
  ] }
}

Transform {                      # 2. occurence of subModel Submodel1
  translation -2 0 0
  children [ USE SubModel1 ]
}

Shape {                          # object of mainModel
  appearance USE c_main
  .. }

] }                              # end of main
# EOF



===========================================================================
#VRML V1.0 ascii
# SubModels: Beim 1. Ditto muss DEF und das gesamte Detail ausgegeben werden;
# bei nachfolgenden Calls mit USE.
# Submodels zuerst in eine Hifsdatei ausgeben; beim 1 use einfuegen.

# Keine Umwandlung des Koordinatensystems; wie soll man Matrix angeben ?
# - die Drehung um die Achse geht ned; 


Separator {

  # first occurence of subModel
  Separator {
    MatrixTransform { matrix ... }      # Origin of 1. Ditto
    DEF Submodel1
    Separator {
      Material { diffuseColor .. }      # Defaultfarbe des subModel
      Separator {                       # Flaeche 1
        Coordinate3 { point [ .. ] }
        IndexedFaceSet { coordIndex [ .. ] }
      }
      Separator {                       # Flaeche 2
        Material { diffuseColor .. }    # Farbe nur fuer aktive Flaeche
        Coordinate3 { point [ .. ] }
        IndexedFaceSet { coordIndex [ .. ] }
      }
    }
  }

  # second occurence of subModel
  Separator {
    MatrixTransform { .. }      # Origin of 2. Ditto
    USE Submodel1
  }

}


---------------------------------------------
 TODO VRML2: 
   Externe Modelcalls sollten ev via EXTERNPROTO gehen !
     Dazu muesste man den Aufruf via EXTERNPROTO machen und
       (ev hinterher) alle externen Dittos als PROT-Files schreiben.
        Wo ? Im tmp oder im quell-directory ?
        Wie geht eigentlich ein export nach wrl mit einem wrl-Ditto ?
          
   Check PROTO und EXTERNPROTO
     Man koennte die ../tmp/xx.tess benutzen ..
   ...........................................
   wrl-subModels koennen so benutzt werden:
   Transform {
    translation 10 0 0
    children [ Inline { url ".0.wrl" } ]
    }

*/

#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <GL/gl.h>
#include <GL/glu.h>


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../ut/ut_obj.h"              // UTO_stru_2_obj
#include "../ut/ut_txfil.h"            // UTF_GetPosLnr
#include "../ut/ut_iTab.h"             // I4Tab
#include "../ut/ut_cast.h"             // LONG_PTR
#include "../ut/ut_memTab.h"           // MemTab_..

#include "../db/ut_DB.h"               // DB_get_ModBas

#include "../ut/func_types.h"               // UI_Func... SYM_..
// #include "../gr/ut_DL.h"               // DL_GetAtt
// #include "../gr/ut_GL.h"               // GL_GetCen
// #include "../gr/vf.h"                  // GL_vf1_CS
// #include "../gr/ut_GL_bitmaps.h"       // GL_vf1_CS; braucht GL/gl.h !

// #include "../xa/xa_ui.h"               // UID_..
#include "../xa/xa_mem.h"              // memspc51, mem_cbuf1
#include "../xa/xa_tex.h"              // Tex_get_fn
// #include "../xa/ut_ui_TX.h"            // TX_Print
#include "../xa/xa.h"                  // AP_STAT AP_mod_fnam



typedef_MemTab(ColRGB);

/* ============ Externe Var: ======================================= */
// aus xa.c:
extern ColRGB    AP_defcol;
extern AP_STAT   AP_stat;



//========= local Vars: ====================
static FILE *TSU_fp=NULL;
static char *TSU_fnam;
static int  TSU_ftyp;
static int  TSU_ptNr;
static int  TSU_facNr;
static int  TSU_errStat;          // Anzahl Errors ..

// static void *smNameTab;          // stack of sm-names
// static char *actModNam;          // name of subModel being resolved ...
static char actModNam[80];          // name of subModel being resolved ...

static char layNam[] = "0";




//================================================================
  int TSU_exp_sm_sur (ObjGX *oxi) {
//================================================================
// export all faces from Mockup-struct
// see TSU_exp_sur

  int      i1, rSiz;


  // skip sizeRecord
  L_OBJ_1:
  i1 = 0;

  
  // wenn das erste obj Adress: skip it
  if(oxi->typ == Typ_Address) ++oxi;

  L_next:
  if(oxi->typ  == Typ_Done)  goto L_fertig;

  // erster Record muss size of following Record sein
  if(oxi->typ  != Typ_Size) goto L_Err2;
  rSiz = (long)oxi->data;
  // printf("Record %d size=%d\n",i1,rSiz);

  ++oxi;  //(char*)oxi += sizeof(ObjGX);

  TSU_exp_fac (oxi);

  // (char*)oxi += rSiz;
  oxi = (ObjGX*)((char*)oxi + rSiz);
  ++i1;
  goto L_next;


  L_fertig:
  // TSU_exp_close ();  // write exitRecords ..
  return 0;


  L_Err2:
    TX_Error("TSU_exp_sm_sur E002 typ %d",oxi->typ);
    return -1;

}


//================================================================
  int TSU_exp_sm__ (char *mnam, ModelRef *mdr) {
//================================================================
// das sm.tess laden;
// ins sm-refsys transformieren
// und ausgeben


  int      irc;
  char     cBuf[256];
  Mat_4x3  m1;
  Memspc   impSpc;


  printf("TSU_exp_sm__ |%s|\n",mnam);

  // den mnam des .tess ermitteln; in "IGS__0_stl";
  // change to "../tmp/IGS__0.tess
  sprintf(cBuf, "%s%s",OS_get_tmp_dir(),mnam);
  // cBuf[strlen(cBuf)-4] = '\0';
  // strcat(cBuf, ".tess");
    printf(" fTess=|%s|\n",cBuf);


  // laden
  if(OS_checkFilExist (cBuf, 1) == 0) goto L_err1;
  irc = TSU_imp_tess (&impSpc, cBuf);
  if(irc < 0) goto L_err1;


  // refsys laden
  UT3D_m3_load_povxvz (m1, &mdr->po, &mdr->vx, &mdr->vz);


  // ins sm-refsys transformieren
  irc = tess_tra__ (impSpc.next, m1);
  if(irc < 0) goto L_exit;


  // ausgeben (analog TSU_exp_sur)
  if(!strcmp(actModNam, "main")) {     // mainModel
    TSU_exp_sm_sur (impSpc.next);
  } else {                             // subModel
    // man muss tess impSpc.next zum TSU_vMem dazumergen 
    TSU_tess_addf (impSpc.next);
    printf(" merge 2 x tess........\n");
  
    // tess_write_f_ (TSU_fp, impSpc.next);
  }





  L_exit:

  UME_free (&impSpc);
  return 0;


  L_err1:
    TX_Print("TSU_exp_sm__ E001");
    return -1;

}


//================================================================
  int TSU_exp_stl1Fac (Point *p1, Point *p2, Point *p3) {
//================================================================


  Vector     vc;

  // printf("TSU_exp_stl1Fac x\n");
    // GR_tDyn_pcv (p1, p2, 9);
    // GR_tDyn_pcv (p2, p3, 9);
    // GR_tDyn_pcv (p3, p1, 9);


  UT3D_vc_perp3pt (&vc, p1, p2, p3);
  UT3D_vc_setLength (&vc, &vc, 1.);

  fprintf(TSU_fp,"facet normal %f %f %f\n",vc.dx,vc.dy,vc.dz);
  fprintf(TSU_fp,"outer loop\n");
  fprintf(TSU_fp,"vertex %f %f %f\n",p1->x,p1->y,p1->z);
  fprintf(TSU_fp,"vertex %f %f %f\n",p2->x,p2->y,p2->z);
  fprintf(TSU_fp,"vertex %f %f %f\n",p3->x,p3->y,p3->z);
  fprintf(TSU_fp,"endloop\n");
  fprintf(TSU_fp,"endfacet\n");

  return 0;

}

//================================================================
  int TSU_exp_stlFac (ObjGX *oxi) {
//================================================================
// write vertices and faces for STL
// nur mehr pointTables;
// glTyp: 4=GL_TRIANGLES   5=GL_TRIANGLE_STRIP   6=GL_TRIANGLE_FAN
//        16=GL_FAC_PLANAR

  int   i0, i1, i2, i3, ie;
  Point *pTab;
  char  cbuf[128];


  // printf("TSU_exp_stlFac ptNr=%d glTyp=%d\n",oxi->siz,oxi->aux);

  ie   = oxi->siz;
  pTab = oxi->data;


  // ACHTUNG: der letzte Punkt ist nicht gleich dem ersten; fehlt also !
  // ie += TSU_ptNr;    // TSU_ptNr + siz
  // for(i1=0;i1<oxi->siz;++i1) DEB_dump_obj__(Typ_PT,&pTab[i1],"p[%d]",i1);




  //================================================================
  // TRIANGLE_FAN u. nonPlanar_TRIANGLE_FAN:
  if((oxi->aux != GL_TRIANGLE_FAN) &&
     (oxi->aux != GL_FAC_PLANAR))  goto L_GLS;
  // printf("fffffffffffffffffffffffff fan fffffffffffffffff\n");

  //    1------2           ptAnz = 4
  //    |    /  |
  //    |   /   |
  //    |  /    |
  //    | /     |
  //    0.------3
  //   5  \     /
  //        \  /
  //         4
  //
  // sollte so zerlegt werden:
  // 0 1 2
  // 0 2 3
  // 0 3 4

  ie -= 1;

  i0 = 0;
  i2 = 1;

  L_f_nxt:
  i1 = i2;
  ++i2;

  TSU_exp_stl1Fac (&pTab[i0], &pTab[i1], &pTab[i2]);
    // printf(" i0=%d i1=%d i2=%d\n",i0,i1,i2);

  if(i2 < ie) goto L_f_nxt;

  goto L_fertig;



  //================================================================
  L_GLS:
  if(oxi->aux != GL_TRIANGLE_STRIP) goto L_GLT;
  // printf("SSSSSSSSSSSSSSSSSSSSSSSSS strip SSSSSSSSSSSSSSSSS\n");

// GL_TRIANGLE_STRIP:
//
//    0--2--4--6
//    | /| /| /|
//    |/ |/ |/ |
//    1--3--5--7
//
// sollte so zerlegt werden:
// 0 1 2
// 2 1 3
// 2 3 4
// 4 3 5
// 4 5 6
// 6 5 7  aus


  i1 = 0;
  i3 = i1 + 1;       // 1 - 2


  L_s_nxt:
  // i1 bleibt
  i2 = i3;
  ++i3;          // 1 2 3       3 4 5

  // if(i3 < (ie-1)) { } else { }
  TSU_exp_stl1Fac (&pTab[i1], &pTab[i2], &pTab[i3]);
    // printf(" i1=%d i2=%d i3=%d\n",i1,i2,i3);


  i1 = i3;
  // i2 bleibt
  ++i3;           // 3 2 4
  if(i3 >= ie) goto L_fertig;


  // if(i3 < (ie-1)) { } else { }
  TSU_exp_stl1Fac (&pTab[i1], &pTab[i2], &pTab[i3]);
    // printf(" i1=%d i2=%d i3=%d\n",i1,i2,i3);

  if(i3 < (ie-1)) goto L_s_nxt;
  goto L_fertig;



  //================================================================
  L_GLT:
  // printf("TTTTTTTTTTTTTTTTTTTTTTTTT trias TTTTTTTTTTTTTTTTT\n");
  if(oxi->aux != GL_TRIANGLES) {
    // TX_Print("TSU_exp_stlFac E001 %d",oxi->aux);
    return -1;
  }


  // ie -= 1;   // letzter Punkt nicht notwendig

  i3 = -1;

  L_t_nxt:
    i1 = i3 + 1;
    i2 = i1 + 1;
    i3 = i2 + 1;

    if((i3+3) < ie) {} else {}
    TSU_exp_stl1Fac (&pTab[i1], &pTab[i2], &pTab[i3]);
      // printf(" i1=%d i2=%d i3=%d\n",i1,i2,i3);

    if((i3+3) < ie) goto L_t_nxt;




  //================================================================
  L_fertig:
  // TSU_ptNr = ie;
  TSU_ptNr = oxi->siz;
  ++TSU_facNr;
  return 0;

}


//================================================================
  int TSU_exp_objFac (ObjGX *oxi, ColRGB *sCol) {
//================================================================
// write vertices and faces for WaveFront-obj
// nur mehr pointTables;
// glTyp: 4=GL_TRIANGLES   5=GL_TRIANGLE_STRIP   6=GL_TRIANGLE_FAN

/*
If there comes a TRIANGLE_FAN of 7 points, write:
v1
v2
v3
v4
v5
v6
v7
f 1 2 3 4
f 1 4 5 6
f 1 6 7
*/

  int   i0, i1, i2, i3, ie;
  Point *pTab;
  char  cbuf[128];


  // printf("TSU_exp_objFac ptNr=%d glTyp=%d\n",oxi->siz,oxi->aux);

  ie   = oxi->siz;
  pTab = oxi->data;

  if((void*)sCol) TSU_exp_objCol (0, sCol);

  for(i1=0; i1<ie; ++i1) {
    // DEB_dump_obj__ (Typ_PT, &pTab[i1], "P[%d]",i1);
    // GR_tDyn_symB__ (&pTab[i1], SYM_STAR_S, 2);

    // change orientation: obj-y = cad-z; obj-z = cad-y * -1   2013-08-19
    strcpy(cbuf, "v ");
    UTX_add_fl_u (cbuf, pTab[i1].x); strcat(cbuf, " ");
    UTX_add_fl_u (cbuf, pTab[i1].z); strcat(cbuf, " ");
    UTX_add_fl_u (cbuf, pTab[i1].y * -1.);
    fprintf(TSU_fp,"%s\n",cbuf);
      // printf("%s\n",cbuf);
  }

  ie += TSU_ptNr;    // TSU_ptNr + siz


  // write faces ..


  //================================================================
  // TRIANGLE_FAN:
  if(oxi->aux != GL_TRIANGLE_FAN) goto L_GLS; // 6 L_GLNPF;
  // printf("fffffffffffffffffffffffff fan fffffffffffffffff\n");

// sollte so zerlegt werden:
// 1 2 3
// 1 3 4
// 1 4 5

  // die ersten 4
  i0 = TSU_ptNr + 1;
  i2 = TSU_ptNr + 2;

  L_f_nxt:
  i1 = i2;
  ++i2;

  sprintf(cbuf, "f %d %d %d",i0,i1,i2);
  fprintf(TSU_fp,"%s\n",cbuf);
    // printf("L_f_nxt: %s\n",cbuf);

  if(i2 < ie) goto L_f_nxt;

  goto L_fertig;


/*
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

    if(i1 < (ie-2)) fprintf(TSU_fp," 0,%d,%d,-1,\n",i1,i1+1);
    else        fprintf(TSU_fp," 0,%d,%d,-1 ] } }\n",i1,i1+1);
      // printf(" npf: 0,%d,%d,-1\n",i1,i1+1);

    ++i1;
    if(i1 < (ie-1)) goto L_n_nxt;

  goto L_fertig;

*/

  //================================================================
  L_GLS:
  if(oxi->aux != GL_TRIANGLE_STRIP) goto L_GLT;        // 5  (gl.h)
  // printf("SSSSSSSSSSSSSSSSSSSSSSSSS strip SSSSSSSSSSSSSSSSS\n");

// GL_TRIANGLE_STRIP:
//
//    1--3--5--7
//    | /| /| /|
//    |/ |/ |/ |
//    2--4--6--8
//
// sollte so zerlegt werden:
// 1 2 3
// 3 2 4
// 3 4 5
// 5 4 6
// 5 6 7   usw ...

  i1 = TSU_ptNr + 1;
  i3 = i1 + 1;       // 1 - 2


  L_s_nxt:
  // i1 bleibt
  i2 = i3;
  ++i3;          // 1 2 3       3 4 5

  sprintf(cbuf, "f %d %d %d",i1,i2,i3);
  fprintf(TSU_fp,"%s\n",cbuf);
    // printf("%s\n",cbuf);

  if(i3 >= ie) goto L_fertig;

  i1 = i3;
  // i2 bleibt
  ++i3;           // 3 2 4

  sprintf(cbuf, "f %d %d %d",i1,i2,i3);
  fprintf(TSU_fp,"%s\n",cbuf);
    // printf("%s\n",cbuf);

  if(i3 >= ie) goto L_fertig;
  goto L_s_nxt;



  //================================================================
  L_GLT:
  // printf("TTTTTTTTTTT trias TTTTTTTTTTTTTTTTT\n");
// sollte so zerlegt werden:
// 1 2 3
// 4 5 6
  if(oxi->aux != GL_TRIANGLES) {                        // 4 (gl.h)
    TX_Print("TSU_exp_objFac E001 %d",oxi->aux);
    return -1;
  }

  i3 = TSU_ptNr;

  L_t_nxt:
  i1 = i3 + 1;
  i2 = i1 + 1;
  i3 = i2 + 1;

  sprintf(cbuf, "f %d %d %d",i1,i2,i3);
  fprintf(TSU_fp,"%s\n",cbuf);
    // printf("|%s| i3=%d ie=%d\n",cbuf,i3,ie);

  if((i3+2) < ie) goto L_t_nxt;




  //================================================================
  L_fertig:
  TSU_ptNr = ie;
    // printf(" exit TSU_exp_objFac\n");

  return 0;

}


//================================================================
  int TSU_exp_objCol (int mode, ColRGB  *newCol) {
//================================================================
/// \code
/// mode  0=activate color;
///       1=write material-libraryFile
///
/// write color; obj-spec see ../exp/obj_ut.c
/// \endcode

static  int iMtl = -1;
static MemTab(ColRGB) colTab = _MEMTAB_NUL;


  int     i1, i2;
  long    ld;
  char    s1[256];


  if(mode == 1) goto L_exp;


  if((void*)newCol) {
    // DEB_dump_obj__ (Typ_Color, newCol, "TSU_exp_objCol ");


    // init mtlTab
    if(iMtl < 0) {
      if(!(void*)newCol) return 0;  // defCol only ..
      MemTab_ini__ (&colTab, sizeof(ColRGB), Typ_Color, 30);
      MemTab_sav (&colTab, &ld, &AP_defcol, 1);
      iMtl = 0;
      // write "mtllib <modelnam>.mtl"
      UTX_fnam_s (s1, TSU_fnam);
      // replace last 3 chars = "mtl"
      UTX_chg_right (s1, "mtl");
      fprintf(TSU_fp,"mtllib %s\n",s1);
    }

    // test if matl already exists; loop tru mtlTab ..
    for(i1=0; i1<colTab.rNr; ++i1) {
      // compare colTab.data[i1], newCol
      if(MEM_cmp__(newCol, &colTab.data[i1], sizeof(ColRGB)) == 0) {
        iMtl = i1;
        goto L_new_wri;
      }
    }

    // add newCol as new matl
    iMtl = colTab.rNr;
    MemTab_sav (&colTab, &ld, newCol, 1);

    L_new_wri:
    fprintf(TSU_fp,"usemtl Matl.%d\n",iMtl);


  } else {
    // default matl = Matl.0
    if(iMtl > 0) fprintf(TSU_fp,"usemtl Matl.0");
  }

  return 0;



  //================================================================
  L_exp:
    printf(" exp.matltab |%s|\n",TSU_fnam);


  if(iMtl < 1) return 0;

  strcpy(s1, TSU_fnam);
  // replace last 3 chars = "mtl"
  UTX_chg_right (s1, "mtl");

  if((TSU_fp = fopen(s1, "w")) == NULL) {
    TX_Print("TSU_exp_objCol E002");
    return -1;
  }

  
  // loop tru colTab
  for(i1=0; i1<colTab.rNr; ++i1) {
    TSU_exp_objMtl (i1, &colTab.data[i1]);  // write matl
  }

  fclose (TSU_fp); TSU_fp = NULL;

  MemTab_free (&colTab);
  iMtl = -1;

  return 0;

}

//================================================================
  int TSU_exp_objMtl (int iNr, ColRGB  *newCol) {
//================================================================
/// write matl

  int     i2;
  float   f1, f2, f3;


  // printf("TSU_exp_objMtl %d\n",iNr);
  // DEB_dump_obj__ (Typ_Color, newCol, "");


    fprintf(TSU_fp,"newmtl Matl.%d\n",iNr);

    f1 = newCol->cr;
    f2 = newCol->cg;
    f3 = newCol->cb;
    f1 /= 255;
    f2 /= 255;
    f3 /= 255;
    fprintf(TSU_fp,"Kd %f %f %f\n",f1,f2,f3);

    i2 = newCol->vtra;
    if(i2 == 1)      fprintf(TSU_fp,"d 0.8\n");
    else if(i2 == 2) fprintf(TSU_fp,"d 0.5\n");


  return 0;

}
 

//================================================================
  int TSU_exp_wrl1Mat (Vector *vcx, Vector *vcz, Point *pto) {
//================================================================
// Write VRML1 - Pos.Mat (of Ditto ..)
// See also wrl1_r_dec_matrix.

//  MatrixTransform { matrix
//   1 0 0 0                       // vx
//   0 1 0 0                       // vy
//   0 0 1 0                       // vz
//   10 0 0 1                      // po
//  }


  char   cbuf[128];
  Vector vx, vy, vz;
  Point  po;


  // printf("TSU_exp_wrl1Mat:\n");
  // DEB_dump_obj__ (Typ_VC, vcx, "vx=");
  // DEB_dump_obj__ (Typ_VC, vcz, "vz=");
  // DEB_dump_obj__ (Typ_PT, pto, "po=");


    fprintf(TSU_fp," MatrixTransform { matrix\n");

    po = *pto;
    // VR_pt_cad2vr (&po);
    
    vx = *vcx;
    vz = *vcz;
    // VR_vc_cad2vr (&vx);
    // VR_vc_cad2vr (&vz);
    
    // vx,vz normiert; daher kein normieren notwendig ..
    UT3D_vc_perp2vc (&vy, &vz, &vx);


    cbuf[0] = '\0';
    UTX_add_fl_10 (cbuf, vx.dx);
    strcat (cbuf, " ");
    UTX_add_fl_10 (cbuf, vx.dy);
    strcat (cbuf, " ");
    UTX_add_fl_10 (cbuf, vx.dz);
    strcat (cbuf, " 0");
    fprintf(TSU_fp,"  %s\n",cbuf);

    cbuf[0] = '\0';
    UTX_add_fl_10 (cbuf, vy.dx);
    strcat (cbuf, " ");
    UTX_add_fl_10 (cbuf, vy.dy);
    strcat (cbuf, " ");
    UTX_add_fl_10 (cbuf, vy.dz);
    strcat (cbuf, " 0");
    fprintf(TSU_fp,"  %s\n",cbuf);

    cbuf[0] = '\0';
    UTX_add_fl_10 (cbuf, vz.dx);
    strcat (cbuf, " ");
    UTX_add_fl_10 (cbuf, vz.dy);
    strcat (cbuf, " ");
    UTX_add_fl_10 (cbuf, vz.dz);
    strcat (cbuf, " 0");
    fprintf(TSU_fp,"  %s\n",cbuf);

    cbuf[0] = '\0';
    UTX_add_fl_10 (cbuf, po.x);
    strcat (cbuf, " ");
    UTX_add_fl_10 (cbuf, po.y);
    strcat (cbuf, " ");
    UTX_add_fl_10 (cbuf, po.z);
    strcat (cbuf, " 1");
    fprintf(TSU_fp,"  %s\n",cbuf);


    fprintf(TSU_fp," }\n");


  return 0;

}


//================================================================
  int TSU_exp_wrl2Mat (Vector *vx, Vector *vz, Point *po) {
//================================================================
// Write VRML2 - Pos. (of Ditto ..)


  char   cbuf[128];
  double q1[4], ar;
  Vector vcx, vcz, vcr;
  Point  p1;


  // printf("TSU_exp_wrl2Mat:\n");
  // DEB_dump_obj__ (Typ_VC, vx, "vx=");
  // DEB_dump_obj__ (Typ_VC, vz, "vz=");
  // DEB_dump_obj__ (Typ_PT, po, "po=");


  // origin from cad-coords -> vr-coords
  p1 = *po;
  VR_pt_cad2vr (&p1);
  // vcx = *vx;
  // VR_vc_cad2vr (&vcx);
  // vcz = *vz;
  // VR_vc_cad2vr (&vcz);


  // use origin po for translation
  cbuf[0] = '\0';
  UTX_add_fl_ua (cbuf, (double*)&p1, 3, ' '); 
/*
  UTX_add_fl_10 (cbuf, po->x);
  strcat (cbuf, " ");
  UTX_add_fl_10 (cbuf, po->y);
  strcat (cbuf, " ");
  UTX_add_fl_10 (cbuf, po->z);
*/
  fprintf(TSU_fp,"  translation %s\n",cbuf);


  // make quat from vx, vz
  // UQT_qt_vxvz (q1, &vcx, &vcz);
  UQT_qt_vxvz (q1, vx, vz);

  // quat --> Vec + Angle
  UQT_vcar_qt (&vcr, &ar, q1);

  if(fabs(ar) < UT_TOL_min1) return 0;

  // Drehachse und -winkel ausgeben
  VR_vc_cad2vr (&vcr);        // CAD -> VR
  cbuf[0] = '\0';
  UTX_add_fl_ua (cbuf, (double*)&vcr, 3, ' '); 
/*
  UTX_add_fl_10 (cbuf, vcr.dx);
  strcat (cbuf, " ");
  UTX_add_fl_10 (cbuf, vcr.dy);
  strcat (cbuf, " ");
  UTX_add_fl_10 (cbuf, vcr.dz);
*/
  strcat (cbuf, " ");
  UTX_add_fl_10 (cbuf, ar);
  fprintf(TSU_fp,"  rotation %s\n",cbuf);



  return 0;

}


//================================================================
  int TSU_exp_wrl1Fac (ObjGX *oxi, ColRGB *sCol) {
//================================================================
// write vertices and faces for VRML1
// nur mehr pointTables;
// glTyp: 4=GL_TRIANGLES   5=GL_TRIANGLE_STRIP   6=GL_TRIANGLE_FAN



  int   i0, i1, i2, i3, ie;
  Point *pTab, p1;
  char  cbuf[128];


  ie   = oxi->siz;  // Anzahl von Punkten 
  pTab = oxi->data;


  // printf("---------------------------------------------\n");
  // printf("TSU_exp_wrl1Fac ptNr=%d glTyp=%d\n",oxi->siz,oxi->aux);
    // for(i1=0; i1<oxi->siz; ++i1)
      // printf(" P[%d] %f %f %f\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z);


  // if(TSU_facNr != 4) goto L_fertig;  // TEST ONLY



  //================================================================
  // write points ..

  fprintf(TSU_fp,"Separator {\n");

  if(sCol != NULL) {
    TSU_exp_wrlCol (sCol);
  }

  fprintf(TSU_fp,"  Coordinate3 { point [\n");



  // fprintf(TSU_fp,"Separator { Coordinate3 { point [\n",cbuf);
    // printf(" Points:\n");


  // letzter Punkt nicht notwendig.
  for(i1=0; i1<ie; ++i1) {
    // DEB_dump_obj__ (Typ_PT, &pTab[i1], "P[%d]",i1);
    // GR_tDyn_symB__ (&pTab[i1], SYM_STAR_S, 2);
    // GR_tDyn_txiA (&pTab[i1], i1, 0);

    strcpy(cbuf, " ");
    // change CAD-coordinates into VR-Coordinates!
    p1 = pTab[i1];
    // VR_pt_cad2vr (&p1);
    UTX_add_fl_ua (cbuf, (double*)&p1, 3, ' ');

    if(i1 < (ie-1)) fprintf(TSU_fp,"%s,\n",cbuf);
    else            fprintf(TSU_fp,"%s ] }\n",cbuf);
      // printf(" p[%d]=%s\n",i1,cbuf);

    // UTX_add_fl_15 (cbuf, &pTab[i1].x);
  }

  // ie += TSU_ptNr;    // TSU_ptNr + siz


  //================================================================
  // write faces ..
  fprintf(TSU_fp,"IndexedFaceSet { coordIndex [\n"); //,cbuf);
    // printf(" FaceSet:\n");


  //================================================================
  // TRIANGLE_FAN:
  if(oxi->aux != GL_TRIANGLE_FAN) goto L_GLNPF;
  // printf("fffffffffffffffffffffffff fan fffffffffffffffff %d\n",ie);

  // die ersten 4 
  i0 = 0;

  fprintf(TSU_fp," %d",i0);
    // printf(" %d",i0);
  ++i0;

  L_f_nxt:

    fprintf(TSU_fp,",%d",i0);
      // printf(",%d",i0);
    ++i0;
    if(i0 < ie) goto L_f_nxt;

  fprintf(TSU_fp,",-1 ] } }\n");
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

    if(i1 < (ie-2)) fprintf(TSU_fp," 0,%d,%d,-1,\n",i1,i1+1);
    else        fprintf(TSU_fp," 0,%d,%d,-1 ] } }\n",i1,i1+1);
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

  if(i3 < (ie-1)) fprintf(TSU_fp," %d,%d,%d,-1,\n",i1,i2,i3);
  else        fprintf(TSU_fp," %d,%d,%d,-1 ] } }\n",i1,i2,i3);
  // if(i3 < (ie-1)) fprintf(TSU_fp," %d,%d,%d,-1,\n",i1,i3,i2);
  // else        fprintf(TSU_fp," %d,%d,%d,-1 ] } }\n",i1,i3,i2);
    // printf(" facOut %d,%d,%d,-1\n",i1,i2,i3);


  i1 = i3;
  // i2 bleibt
  ++i3;           // 3 2 4
  if(i3 >= ie) goto L_fertig;


  if(i3 < (ie-1)) fprintf(TSU_fp," %d,%d,%d,-1,\n",i1,i2,i3);
  else        fprintf(TSU_fp," %d,%d,%d,-1 ] } }\n",i1,i2,i3);
  // if(i3 < (ie-1)) fprintf(TSU_fp," %d,%d,%d,-1,\n",i1,i3,i2);
  // else        fprintf(TSU_fp," %d,%d,%d,-1 ] } }\n",i1,i3,i2);
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

    if((i3+3) < ie) fprintf(TSU_fp," %d,%d,%d,-1,\n",i1,i2,i3);
    else        fprintf(TSU_fp," %d,%d,%d,-1 ] } }\n",i1,i2,i3);
      // printf(" %d,%d,%d,-1\n",i1,i2,i3);

    if((i3+3) < ie) goto L_t_nxt;




  //================================================================
  L_fertig:
  TSU_ptNr = ie;
  ++TSU_facNr;
  return 0;

}


//================================================================
  int TSU_exp_dxf1Fac (Point *p1, Point *p2, Point *p3) {
//================================================================


  char  cbuf[128];

  fprintf(TSU_fp,"0\n3DFACE\n");
  fprintf(TSU_fp,"8\n%s\n",layNam);
  sprintf(cbuf, "10\n%f\n20\n%f\n30\n%f",p1->x,p1->y,p1->z);
  fprintf(TSU_fp,"%s\n",cbuf);
  sprintf(cbuf, "11\n%f\n21\n%f\n31\n%f",p2->x,p2->y,p2->z);
  fprintf(TSU_fp,"%s\n",cbuf);
  sprintf(cbuf, "12\n%f\n22\n%f\n32\n%f",p3->x,p3->y,p3->z);
  fprintf(TSU_fp,"%s\n",cbuf);
  sprintf(cbuf, "13\n%f\n23\n%f\n33\n%f",p3->x,p3->y,p3->z);
  fprintf(TSU_fp,"%s\n",cbuf);

  return 0;

}


//================================================================
  int TSU_exp_dxfFac (ObjGX *oxi) {
//================================================================
// write vertices and faces for DXF
// nur mehr pointTables;
// glTyp: 4=GL_TRIANGLES   5=GL_TRIANGLE_STRIP   6=GL_TRIANGLE_FAN



  int   i0, i1, i2, i3, ie;
  Point *pTab;
  // char  cbuf[128];


  // printf("TSU_exp_dxfFac ptNr=%d glTyp=%d\n",oxi->siz,oxi->aux);
  // printf("   TSU_ptNr=%d\n",TSU_ptNr);



  ie   = oxi->siz;
  pTab = oxi->data;

  // ie += TSU_ptNr;    // TSU_ptNr + siz




  //================================================================
  // TRIANGLE_FAN:
  if(oxi->aux != GL_TRIANGLE_FAN) goto L_GLNPF;
  // printf("fffffffffffffffffffffffff fan fffffffffffffffff %d\n",ie);
  // sollte so zerlegt werden:
  // 1 2 3
  // 1 3 4
  // 1 4 5

  ie -= 1;   // letzter Punkt nicht notwendig

  // die ersten 4
  i0 = 0;
  i2 = 1;

  L_f_nxt:
  i1 = i2;
  ++i2;

  TSU_exp_dxf1Fac (&pTab[i0], &pTab[i1], &pTab[i2]);
    // printf(" i0=%d i1=%d i2=%d\n",i0,i1,i2);

  if(i2 < ie) goto L_f_nxt;

  goto L_fertig;




  //================================================================
  // nonPlanar TRIANGLE_FAN:
  L_GLNPF:
  if(oxi->aux != GL_FAC_PLANAR) goto L_GLS;
  // printf("fffffffffffffffffffffffff np_fan fffffffffffffffff %d\n",ie);

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

  ie -= 1;   // letzter Punkt nicht notwendig

  i0 = 0;
  i1 = 1;

  L_n_nxt:

    // if(i1 < (ie-2)) {} else {}
    TSU_exp_dxf1Fac (&pTab[i0], &pTab[i1], &pTab[i1+1]);
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

  // if(i3 < (ie-1)) { } else { }
  TSU_exp_dxf1Fac (&pTab[i1], &pTab[i2], &pTab[i3]);
    // printf(" fac1Out %d,%d,%d,-1\n",i1,i2,i3);


  i1 = i3;
  // i2 bleibt
  ++i3;           // 3 2 4
  if(i3 >= ie) goto L_fertig;


  // if(i3 < (ie-1)) { } else { }
  TSU_exp_dxf1Fac (&pTab[i1], &pTab[i2], &pTab[i3]);
    // printf(" fac2Out %d,%d,%d,-1\n",i1,i2,i3);

  if(i3 < (ie-1)) goto L_s_nxt;
  goto L_fertig;




  //================================================================
  L_GLT:
  // printf("TTTTTTTTTTTTTTTTTTTTTTTTT trias TTTTTTTTTTTTTTTTT %d\n",ie);
  if(oxi->aux != GL_TRIANGLES) {
    TX_Print("TSU_exp_objFac E001 %d",oxi->aux);
    return -1;
  }

  // ie -= 1;   // letzter Punkt nicht notwendig

  i3 = -1;

  L_t_nxt:
    i1 = i3 + 1;
    i2 = i1 + 1;
    i3 = i2 + 1;

    if((i3+3) < ie) {} else {}
    TSU_exp_dxf1Fac (&pTab[i1], &pTab[i2], &pTab[i3]);
      // printf(" %d,%d,%d,-1\n",i1,i2,i3);

    if((i3+3) < ie) goto L_t_nxt;




  //================================================================
  L_fertig:
  // TSU_ptNr = ie;
  TSU_ptNr = oxi->siz;
  ++TSU_facNr;
  return 0;

}


//================================================================
  int TSU_exp_wrl2Tex (Point *pTab, int pNr, int iTex) {
//================================================================
// errechnen der TextureCoordinates.
// 0,0  ist links untem am Image; 1,1 ist rechts-oben;
// 2,2  ist rechts-oben im 2.Image (also 2 x 2 Images auf der Fläche).
 
  int     i1;
  char    s1[80];
  double  dx, dy, sx, sy, ox, oy, aa;
  TexRef  *tr;
  Point   po;
  Point2  p21, p2Cen;
  Vector  vx, vy;
  Mat_3x2 ma;

  // printf("TSU_exp_wrl2Tex pNr=%d iTex=%d\n",pNr,iTex);

  fprintf(TSU_fp,"texCoord TextureCoordinate{point [\n");

  Tex_getRef (&tr, iTex);     // get TexRef-record from TexRef-index
    // DEB_dump_obj__ (Typ_TEXR, tr, "TexRef[%d]:\n",iTex);
  po.x = tr->px;
  po.y = tr->py;
  po.z = tr->pz;
  dx = tr->ssx;
  dy = tr->ssy;
  sx = 1. / tr->uscx;
  sy = 1. / tr->uscy;
  ox = tr->udx;
  oy = tr->udy;
  aa = tr->uar;
  if(UTP_comp_0(aa)) {
    aa = 0.;
  } else {
    aa = UT_RADIANS (aa) * -1.;
    p2Cen.x = -ox / sx;
    p2Cen.y = -oy / sy;
    UT2D_m2_init_rot (ma, aa, &p2Cen);
  }
    // printf(" sx,sy=%f,%f\n",sx,sy);


  // set length of vectors to surface-size
  // UT3D_vc_setLenLen (&vx, 
  UT3D_vc_setLength (&vx, &tr->vx, dx);
  UT3D_vc_setLength (&vy, &tr->vy, dy);
    // DEB_dump_obj__(Typ_VC, &vx, "vx");
    // DEB_dump_obj__(Typ_VC, &vy, "vy");

  for(i1=0; i1<pNr; ++i1) {
    // dx = UT3D_skp_2ptvc (&po, &pTab[i1], &vx);
    // dy = UT3D_skp_2ptvc (&po, &pTab[i1], &vy);
    // get parameter of pTab-point on vectors vx,vy
    UT3D_parpt_ptvc (&dx, &pTab[i1], &po, &vx);
    UT3D_parpt_ptvc (&dy, &pTab[i1], &po, &vy);
      // printf(" pt[%d] - dx=%f dy=%f\n",i1,dx,dy);
    // add offsets
    // rotate
    if(aa) {
        // printf(" rot ..\n");
      p21.x = dx;
      p21.y = dy;
      UT2D_pt_traptm2 (&p21, ma, &p21);
      dx = p21.x;
      dy = p21.y;
    }
    // scale
    dx *= sx;
    dy *= sy;
    // add offsets
    dx += ox;
    dy += oy;
    sprintf(s1, "%f %f",dx,dy);
    if(i1 < (pNr-1)) fprintf(TSU_fp,"%s,\n",s1); // nicht fuer letzte Zeile
    else             fprintf(TSU_fp,"%s ] }\n",s1);
  }


  return 0;

}


//================================================================
  int TSU_exp_wrl2Fac (ObjGX *oxi, ColRGB *sCol, int iTex) {
//================================================================
// write vertices and faces for VRML2
// nur mehr pointTables;
// glTyp: 4=GL_TRIANGLES   5=GL_TRIANGLE_STRIP   6=GL_TRIANGLE_FAN



  int   i0, i1, i2, i3, ie;
  Point *pTab, p1;
  char  cbuf[128];


  ie   = oxi->siz;  // Anzahl von Punkten
  pTab = oxi->data;


  // printf("---------------------------------------------\n");
  // printf("TSU_exp_wrl2Fac ptNr=%d glTyp=%d iTex=%d\n",oxi->siz,oxi->aux,iTex);
  // if(sCol) DEB_dump_obj__ (Typ_Color, sCol, " sCol ");
  // fprintf(TSU_fp, "# TSU_exp_wrl2Fac\n");
    // for(i1=0; i1<oxi->siz; ++i1)
      // printf(" P[%d] %f %f %f\n",i1,pTab[i1].x,pTab[i1].y,pTab[i1].z);


  // if(TSU_facNr != 4) goto L_fertig;  // TEST ONLY



  //================================================================
  // write points ..

  fprintf(TSU_fp,"Shape {\n");


  // if(sCol != NULL) {
  if(iTex < 0) {
    TSU_exp_wrl2Col (sCol);

  } else {
    TSU_exp_wrlTexFn (iTex);
  }


  // write faces ..
  fprintf(TSU_fp,
    "geometry IndexedFaceSet { solid FALSE coord Coordinate { point [\n");
    //, cbuf);


  // letzter Punkt nicht notwendig.
  for(i1=0; i1<ie; ++i1) {
    // DEB_dump_obj__ (Typ_PT, &pTab[i1], "P[%d]",i1);
    // GR_tDyn_symB__ (&pTab[i1], SYM_STAR_S, 2);
    // GR_tDyn_txiA (&pTab[i1], i1, 0);

    strcpy(cbuf, " ");
    // change CAD-coordinates into VR-Coordinates!
    p1 = pTab[i1];
    VR_pt_cad2vr (&p1);
    UTX_add_fl_ua (cbuf, (double*)&p1, 3, ' '); 

    if(i1 < (ie-1)) fprintf(TSU_fp,"%s,\n",cbuf); // nicht fuer letzte Zeile
    else            fprintf(TSU_fp,"%s ] }\n",cbuf);
      // printf(" p[%d]=%s\n",i1,cbuf);

    // UTX_add_fl_15 (cbuf, &pTab[i1].x);
  }

  // ie += TSU_ptNr;    // TSU_ptNr + siz


  // textures: write TextureCoordinate-table
  if(iTex < 0) goto L_ci;
  TSU_exp_wrl2Tex (pTab, ie, iTex);


  // start coordIndextable ..
  L_ci:
  fprintf(TSU_fp,"coordIndex [\n");



  //================================================================
  // TRIANGLE_FAN:
  if(oxi->aux != GL_TRIANGLE_FAN) goto L_GLNPF;
  // printf("fffffffffffffffffffffffff fan fffffffffffffffff %d\n",ie);

  // die ersten 4
  i0 = 0;

  fprintf(TSU_fp," %d",i0);
    // printf(" %d",i0);
  ++i0;

  L_f_nxt:

    fprintf(TSU_fp,",%d",i0);
      // printf(",%d",i0);
    ++i0;
    if(i0 < ie) goto L_f_nxt;

  fprintf(TSU_fp,",-1 ] } }\n");
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

    if(i1 < (ie-2)) fprintf(TSU_fp," 0,%d,%d,-1,\n",i1,i1+1);
    else        fprintf(TSU_fp," 0,%d,%d,-1 ] } }\n",i1,i1+1);
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

  if(i3 < (ie-1)) fprintf(TSU_fp," %d,%d,%d,-1,\n",i1,i2,i3);
  else        fprintf(TSU_fp," %d,%d,%d,-1 ] } }\n",i1,i2,i3);
  // if(i3 < (ie-1)) fprintf(TSU_fp," %d,%d,%d,-1,\n",i1,i3,i2);
  // else        fprintf(TSU_fp," %d,%d,%d,-1 ] } }\n",i1,i3,i2);
    // printf(" facOut %d,%d,%d,-1\n",i1,i2,i3);


  i1 = i3;
  // i2 bleibt
  ++i3;           // 3 2 4
  if(i3 >= ie) goto L_fertig;


  if(i3 < (ie-1)) fprintf(TSU_fp," %d,%d,%d,-1,\n",i1,i2,i3);
  else        fprintf(TSU_fp," %d,%d,%d,-1 ] } }\n",i1,i2,i3);
  // if(i3 < (ie-1)) fprintf(TSU_fp," %d,%d,%d,-1,\n",i1,i3,i2);
  // else        fprintf(TSU_fp," %d,%d,%d,-1 ] } }\n",i1,i3,i2);
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

    if((i3+3) < ie) fprintf(TSU_fp," %d,%d,%d,-1,\n",i1,i2,i3);
    else        fprintf(TSU_fp," %d,%d,%d,-1 ] } }\n",i1,i2,i3);
      // printf(" %d,%d,%d,-1\n",i1,i2,i3);

    if((i3+3) < ie) goto L_t_nxt;




  //================================================================
  L_fertig:
  TSU_ptNr = ie;
  ++TSU_facNr;
  return 0;

}


//================================================================
  int TSU_exp_fac (ObjGX *oxi) {
//================================================================
// see also TSU_ntria_bMsh_p GL_set_bMsh

  int     i1, i2, i1Nr, i2Nr, iTyp;
  int     iTex;
  ColRGB  *sCol, defCol;
  ObjGX   *oxs;


  // printf("\nTSU_exp_fac %d %d %d\n",oxi->typ,oxi->form,oxi->siz);
  // DEB_dump_ox_s_ (oxi, "TSU_exp_fac");
  // DEB_dump_ox_0 (oxi, "TSU_exp_fac");

  i1Nr = oxi->siz;              // ex Sur  211 143 5


  // defCol kommt als single Record;
  if(oxi->typ == Typ_Color) {
    // TSU_exp_wrlCol (&oxi->data);  // data ist selbst Color !
    defCol = *((ColRGB*)&oxi->data);
      // printf(" defCol %d %d %d\n",defCol.cr,defCol.cg,defCol.cb);
    // fprintf(TSU_fp,"# fac col %d %d %d\n",defCol.cr,defCol.cg,defCol.cb);
    return 0;
  }


  // Parentrecord
  // (char*)oxi += sizeof(ObjGX); // skip 210 143 1
  oxi = oxi->data; // skip 211 143 5

  // printf(" ..TSU_exp_fac %d %d %d\n",oxi->typ,oxi->form,oxi->siz);
  sCol = NULL;   // defaultCol
  iTex = -1;

  for(i1=0; i1<i1Nr; ++i1) { // loop tru curves

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
          // printf(" %d facCol %d %d %d\n",i2,sCol->cr,sCol->cg,sCol->cb);


      } else if(oxs->typ == Typ_Texture) {
        // sCol = NULL;
        iTex = INT_PTR (oxs->data);
          // printf(" facTex=%d\n",iTex);



      } else if(iTyp == Typ_PT) {
        if(oxs->siz < 3) {      // auch letzer Punkt !!!
          printf(" TSU_exp_fac I001\n");
          continue;
        }
        // printf(" %d Typ_PT\n",i2);
        // if(!strcmp(mode, "OBJ")) TSU_exp_objFac (oxs);
        if     (TSU_ftyp == Mtyp_WRL)   TSU_exp_wrl1Fac (oxs, sCol);  // VRML1
        else if(TSU_ftyp == Mtyp_WRL2)  TSU_exp_wrl2Fac (oxs, sCol, iTex); // VR2
        else if(TSU_ftyp == Mtyp_DXF)   TSU_exp_dxfFac (oxs);
        else if(TSU_ftyp == Mtyp_STL)   TSU_exp_stlFac (oxs);
        else if(TSU_ftyp == Mtyp_OBJ)   TSU_exp_objFac (oxs, sCol);
      }
      ++oxs;  //(char*)oxs += sizeof(ObjGX);
    }

    ++oxi;  //(char*)oxi += sizeof(ObjGX);

  }



  return 0;

}




//================================================================
  int TSU_exp_wrl2Col (ColRGB  *newCol) {
//================================================================

  float  f1, f2, f3;
  char   cBuf[80];


  if(newCol == NULL) {
      // printf(" defCol !\n");
    fprintf(TSU_fp,"appearance col_0 {}\n");
    return 0;
  }




  // DEB_dump_obj__ (Typ_Color, newCol, "TSU_exp_wrl2Col ");

  f1 = newCol->cr;
  f1 /= 255;
  f2 = newCol->cg;
  f2 /= 255;
  f3 = newCol->cb;
  f3 /= 255;
    // printf(" col %f %f %f\n",f1,f2,f3);
    // fprintf(TSU_fp,"# TSU_exp_wrlCol %.2f %.2f %.2f\n",f1,f2,f3);


  fprintf(TSU_fp,"appearance Appearance{material Material{"); //,actModNam);
  sprintf(cBuf, " diffuseColor %.2f %.2f %.2f ",f1,f2,f3);

  if(newCol->vtra != 0) {
    if(newCol->vtra == 1) strcat(cBuf, "transparency 0.6 ");
    else                  strcat(cBuf, "transparency 0.8 ");
  }

  fprintf(TSU_fp,"%s}}\n",cBuf);  //,f1,f2,f3);

  return 0;

}


//================================================================
  int TSU_exp_wrlProto (int pnr, void *vp) {
//================================================================
// save protoNr pnr; if it is new & uniq: return 0;
// else return -1
// pnr negativ: init for -pnr ints. vp is fixed space (in).
// pnr >= 0:    add pnr to vp==iTab1 if uniq
// RetCode: 0=new rec added; 1=pnr already exists

static I4Tab_NEW  (iTab1);         // init integer-table

  int  i1, irc;


  // printf("TSU_exp_wrlProto %d\n",pnr);


  if(pnr < 0) {   // init
    i1 = sizeof(int) * -pnr;
    I4Tab_init_spc (&iTab1, vp, i1);
    return 0;
  }

  // test if pnr is uniq
  return I4Tab_add_uniq (&i1, &iTab1, pnr);

}


//================================================================
  int TSU_exp_wrlInit (int mode, int ii) {
//================================================================
// write first part of wrl-file; Header & Protos.
// mode 0    close file.
// mode 1    init; open outFile ../tmp/export1.temp  and create Header
// mode 2    create a texture-proto;

static FILE *fpo = NULL;

  int    irc, i1;
  char   s1[300], s2[300], *p1;
  float  f1, f2, f3;


  // printf("TSU_exp_wrlInit %d %d\n",mode,ii);


  //----------------------------------------------------------------
  if(mode == 0) { // close file
    if(fpo) {
      fclose (fpo);
    } else {
      TX_Print("TSU_exp_wrlInit E001");
    }


  //----------------------------------------------------------------
  } else if(mode == 1) { // init;
    // open outFile ../tmp/export1.temp  and create Header
    sprintf(s1, "%sexport1.temp",OS_get_tmp_dir());
    if((fpo = fopen(s1, "w")) == NULL) {
      TX_Print("TSU_exp_wrlInit E002");
      return -1;
    }
    // see also TSU_exp_init
    fprintf(fpo,"#VRML V2.0 utf8\n");
    fprintf(fpo,"# Model %s       %s\n",AP_mod_fnam,OS_date1());

    // create defCol -PROTO
    f1 = AP_defcol.cr; f1 /= 255;
    f2 = AP_defcol.cg; f2 /= 255;
    f3 = AP_defcol.cb; f3 /= 255;
    fprintf(TSU_fp,
"PROTO col_0 [] {Appearance{material Material{diffuseColor %.2f %.2f %.2f}}}\n",
            f1,f2,f3);



  //----------------------------------------------------------------
  } else if(mode == 2) { // create a texture-proto;
    // ii = TexRefNr; get TexbasNr.
    i1 = Tex_itb_itr (ii);
      printf(" ibas=%d\n",i1);
    // test if texture <ii> already worked
    irc = TSU_exp_wrlProto (i1, NULL);
    if(irc > 0) return i1;   // proto already exists
    // get filename of TexRef[<ii>]
    p1 = Tex_get_fn (ii);
    if(p1 == NULL) return -1;
    // make filename absolut (expand symbol)
    irc = Mod_get_path (memspc011, p1);
    if(irc < 0) return -1;
      printf(" fn-abs |%s|\n",memspc011);
    // get outputdirectory
    strcpy(memspc012, TSU_fnam);
      printf(" outfn |%s|\n",memspc012);
    // directory from filename
    UTX_fdir_s (s2, memspc012);
      printf(" outdir |%s|\n",s2);
    // make filename relativ
    UTX_fnam_abs2rel (s1, memspc011, s2);
      printf(" fn-rel |%s|\n",s1);
    fprintf(fpo,"PROTO tex_%d [] {Appearance{",i1);
    fprintf(fpo,"texture ImageTexture{url \"%s\"}}}\n",s1);

    return i1;  // returns TexBas-Nr


  //----------------------------------------------------------------
  }
  return 0;

}


//================================================================
  int TSU_exp_wrlTexFn (int iTex) {
//================================================================
// add texture; Texref-index is <iTex>

  int  irc;

  // sprintf(cBuf, "Material { diffuseColor %.2f %.2f %.2f ",f1,f2,f3);

  irc = TSU_exp_wrlInit (2, iTex);

  fprintf(TSU_fp, " appearance tex_%d {}\n", irc);
 
  return 0;

}


//================================================================
  int TSU_exp_wrlCol (ColRGB  *newCol) {
//================================================================
// write fileheaders out ...
// kommt scheinbar am FileEnde (wenn keine faces; vorher subModelCalls)

  float  f1, f2, f3;
  char   cBuf[80];


  // if(newCol) DEB_dump_obj__ (Typ_Color, newCol, "TSU_exp_wrlCol ");
  // printf("   actModNam=|%s|\n",actModNam);


  if(newCol) {

    if(newCol->color == 0) {
      f1 = AP_defcol.cr;
      f2 = AP_defcol.cg;
      f3 = AP_defcol.cb;
    } else {
      f1 = newCol->cr;
      f2 = newCol->cg;
      f3 = newCol->cb;
        // printf(" col %f %f %f\n",f1,f2,f3);
        // fprintf(TSU_fp,"# TSU_exp_wrlCol %.2f %.2f %.2f\n",f1,f2,f3);
    }
    f2 /= 255;
    f1 /= 255;
    f3 /= 255;
  }


  // ======== WRL / VRML1 =====================================0
  // if(!strcmp(mode, "WRL")) {
  if(TSU_ftyp == Mtyp_WRL) {

    // Color od transparency oder beides ..
    // if(newCol->color != 0) {    // geht nicht f defCol !
      // macht Crash; das File ist nicht open; nur ins Memory speichern !!!
       sprintf(cBuf, "Material { diffuseColor %.2f %.2f %.2f ",f1,f2,f3);
    // } else {
      // strcpy(cBuf, "Material { ");
    // }

    if(newCol->vtra != 0) {
      if(newCol->vtra == 1) strcat(cBuf, "transparency 0.6 ");
      else                  strcat(cBuf, "transparency 0.8 ");
    }

    // fprintf(TSU_fp,"Material { diffuseColor %.2f %.2f %.2f }\n",f1,f2,f3);
    fprintf(TSU_fp,"%s }\n",cBuf);   //,f1,f2,f3);




  // ======== WRL / VRML2 =====================================
  } else if(TSU_ftyp == Mtyp_WRL2) {

    if(newCol == NULL) {    // use DefaultColor
      fprintf(TSU_fp," appearance USE c_%s\n",actModNam);

    } else {
      if(newCol->color == 0) {
        fprintf(TSU_fp," appearance USE c_%s\n",actModNam);

      } else {
        fprintf(TSU_fp," appearance Appearance { material Material {\n");
        sprintf(cBuf, " diffuseColor %.2f %.2f %.2f ",f1,f2,f3);

        if(newCol->vtra != 0) {
          if(newCol->vtra == 1) strcat(cBuf, "transparency 0.6 ");
          else                  strcat(cBuf, "transparency 0.8 ");
        }

        fprintf(TSU_fp,"%s } }\n",cBuf);
      }

    }

  }

  return 0;

}


//================================================================
  int TSU_exp_init (char *mnam) {
//================================================================
// write fileheaders out ...
// ACHTUNG: mit subModels ist gerade das erste subModel aktiv !

  // ColRGB  *defCol;


  // printf("TSU_exp_init |%s|\n",mnam);
    // fprintf(TSU_fp,"# TSU_exp_init |%s|\n",mnam);




  // ======== WRL /VRML1 ===================================
  if(TSU_ftyp == Mtyp_WRL) {
    if(strlen(mnam) < 1) {       // init mainModel
      TSU_exp_Mod (-2, NULL);    // reset used-Model-buffer
      // init-Text mainModel TSU_exp_init !
      fprintf(TSU_fp,"#VRML V1.0 ascii\n");
      fprintf(TSU_fp,"# Model %s       %s\n",AP_mod_fnam,OS_date1());
      fprintf(TSU_fp,"Separator {\n");

    //----------------------------------------------------------------
    } else {                     // init subModel
      // init-Text subModel
      fprintf(TSU_fp,"#<<<<<<<<<<<<<< Beg. subModel %s\n",mnam);
      fprintf(TSU_fp,"Separator {\n");
    }

    return 0;

  }



  // ======== WRL /VRML2 ===================================
  // if(!strcmp(mode, "WRL")) {
  if(TSU_ftyp == Mtyp_WRL2) {
    if(strlen(mnam) < 1) {       // init mainModel
      TSU_exp_Mod (-2, NULL);    // reset used-Model-buffer
      // init-Text mainModel TSU_exp_init !
      // fprintf(TSU_fp,"#VRML V2.0 utf8\n");
      // fprintf(TSU_fp,"# Model %s       %s\n",AP_mod_fnam,OS_date1());
      TSU_exp_wrlInit (1,0);  // init export1.temp
      fprintf(TSU_fp,"Transform { children [\n");


      // DefaultColor main ausgeben
      // DEB_dump_obj__ (Typ_Color, &AP_defcol, "GL_DefColGet");
      strcpy(actModNam, "main");  // name of subModel being resolved ...
      // TSU_exp_wrl2Col (&AP_defcol);




    //----------------------------------------------------------------
    } else {                     // init subModel
      // fprintf(TSU_fp,"# C%d %d %d\n",AP_defcol.cr,AP_defcol.cg,AP_defcol.cb);
      // AP_defcol und mnam passen nicht zueinender !!
      // GL_DefColGet (&defCol);            // get DefaultColor
      // TSU_exp_wrl2Col (&defCol);

      // init-Text subModel
      fprintf(TSU_fp,"#<<<<<<<<<<<<<< Beg. subModel %s\n",mnam);
      // fprintf(TSU_fp,"Transform {\n");
    }

    return 0;

  }


  // ======== STL =====================================0
  if(TSU_ftyp == Mtyp_STL) {
    if(strlen(mnam) < 1) {       // init mainModel
      fprintf(TSU_fp,"solid %s %s gCAD3D\n",AP_mod_fnam,OS_date1());
    // } else {                     // init subModel: nix tun ..
    }
  }


  // ======== OBJ =====================================0
  if(TSU_ftyp == Mtyp_OBJ) {
    if(strlen(mnam) < 1) {       // init mainModel
      fprintf(TSU_fp,"# Model %s       %s\n",AP_mod_fnam,OS_date1());
    //----------------------------------------------------------------
    } else {                     // init subModel
      fprintf(TSU_fp,"#<<<<<<<<<<<<<< Beg. subModel %s\n",mnam);
    }
  }




  TSU_facNr = 0;

  return 0;

}


//================================================================
  int TSU_exp_close (char *mnam) {
//================================================================
// write exitRecords ..


  // printf("TSU_exp_close |%s| CCCCCCCCCCCCCCCCCCCCCCC\n",actModNam);
 


  //======== WRL / VRML1 =====================================
  if(TSU_ftyp == Mtyp_WRL)   {               //  10=VRML1

    if(strlen(mnam) < 1) {       // close mainModel
      fprintf(TSU_fp,"}\n");
      fprintf(TSU_fp,"# EOF\n");

    } else {                     // close subModel
      fprintf(TSU_fp,"}\n");
      fprintf(TSU_fp,"# End subModel\n");
    }



  //======== WRL / VRML2 =====================================
  } else if(TSU_ftyp == Mtyp_WRL2)   {             // 110=VRML2

    // if(strlen(actModNam) < 1) {       // close Model
    if(!strcmp(actModNam, "main")) {     // init mainModel
      fprintf(TSU_fp,"] }\n");
      fprintf(TSU_fp,"# EOF\n");

    } else {                     // close subModel
      fprintf(TSU_fp,"#>>>>>>>>>>>>>> End subModel\n");
    }





  //======== DXF =====================================
  } else if(TSU_ftyp == Mtyp_DXF)   {

    if(!strcmp(actModNam, "main")) {     // init mainModel
      fprintf(TSU_fp,"0\nENDSEC\n0\nEOF\n");
    }
  }



  // ======== STL =====================================0
  if(TSU_ftyp == Mtyp_STL) {
    if(strlen(mnam) < 1) {          // mainModel
      fprintf(TSU_fp,"endsolid %s\n",AP_mod_fnam);
    // } else {                     // subModel
    }
  }


  //======== OBJ =====================================
  // if(TSU_ftyp != Mtyp_OBJ) goto L_TESS;      // 11=OBJ


  //======== TESS =====================================
  // if(TSU_ftyp != Mtyp_TESS) goto L_TESS;                 // 11=TESS






  // back to previous subModel
  // UtxTab_query (&actModNam, smNameTab);
  // UtxTab_rem (smNameTab);              // remove last ModelName



  return 0;

}



//================================================================
  int TSU_exp_sur (ObjGX *oxi) {
//================================================================
// export surfaces of main-model

  int     irc, i1, rSiz;
  // ColRGB  defCol;


  printf("TSU_exp_sur ftyp=%d typ=%d form=%d siz=%d\n",
         TSU_ftyp,oxi->typ,oxi->form,oxi->siz);
  // printf("   _exp_sur actModNam=|%s|\n",actModNam);
  // BMSH_dump_bfMsh (oxi, "TSU_exp_sur ");


    // TESTBLOCK
    // fprintf(TSU_fp,"# TSU_exp_sur |%s|\n",actModNam);
    // TSU_ftyp = Mtyp_STL;
    // END TESTBLOCK



  //========== STL ================================================
  if(TSU_ftyp == Mtyp_STL) {

    if(!strcmp(actModNam, "main")) {     // mainModel
      goto L_OBJ_1;
    } else {                             // subModel
      tess_write_f_ (TSU_fp, oxi);
      return 0;
    }
  }


  //========== TESS ================================================
  // if(strcmp(mode, "TESS")) goto L_OBJ;
  if(TSU_ftyp == Mtyp_TESS) {
    tess_write_f_ (TSU_fp, oxi);
    return 0;
  }


  //========== DXF ===========================================
  L_DXF_0:
  // if(TSU_ftyp != Mtyp_DXF) goto L_OBJ_0;
  // return 0;


  //========== OBJ, WRL ===========================================
  // L_OBJ_0:
  // if((strcmp(mode, "OBJ")) &&
     // (strcmp(mode, "WRL"))) goto L_Err0;
  if((TSU_ftyp == Mtyp_WRL)   ||
     (TSU_ftyp == Mtyp_WRL2)  ||
     (TSU_ftyp == Mtyp_DXF)   ||
     (TSU_ftyp == Mtyp_STL)   ||
     (TSU_ftyp == Mtyp_OBJ))  goto L_OBJ_1;
  goto L_Err0;

  // TSU_exp_init ();  // write fileheaders out ...


  // skip sizeRecord 
  L_OBJ_1:
  i1 = 0;
  
  L_next:
  if(oxi->typ  == Typ_Done)  goto L_fertig;

  // erster Record muss size of following Record sein
  if(oxi->typ  != Typ_Size) goto L_Err2;
  rSiz = (long)oxi->data;
  // printf("Record %d size=%d\n",i1,rSiz);

  ++oxi;  //(char*)oxi += sizeof(ObjGX);

  TSU_exp_fac (oxi);

  // (char*)oxi += rSiz;
  oxi = (ObjGX*)((char*)oxi + rSiz);
  ++i1;
  goto L_next;


  L_fertig:
  // TSU_exp_close ();  // write exitRecords ..
  return 0;



  //----------------------------------------------------------------
  L_Err0:
    TX_Error("TSU_exp_sur E001 Filetyp %s",TSU_ftyp);
    return -1;


  L_Err1:
    TX_Error("TSU_exp_sur E001 form %d",oxi->form);
    return -1;


  L_Err2:
    TX_Error("TSU_exp_sur E002 typ %d",oxi->typ);
    return -1;

}


//================================================================
  int TSU_exp_Defaults () {
//================================================================
// nun sind die Defaults geladen; zB AD_defCol

  // printf("DDDDDDDDDD TSU_exp_Defaults %d\n",TSU_ftyp);
  // fprintf(TSU_fp,"#TSU_exp_Defaults %d %d %d\n",
                 // AP_defcol.cr,AP_defcol.cg,AP_defcol.cb);

/*
  if(TSU_ftyp == Mtyp_WRL2) {
    TSU_exp_wrl2Col (&AP_defcol);
  }
*/

  return 0;

}


//================================================================
  int TSU_exp_Open (char *modNam) {
//================================================================
// start open subModel or mainModel.
// mainModel wenn (mnam == "")
// wird dzt nur von VRML und STL verwendet !
// Es wird Open vorbereitet durch setzen von actModNam;
//   dann das ausgeben der SurfDaten,
//   dann close momentan-active-Model,
//   dann open new Model actModNam.

  int     i1;
  char    cbuf[256], mnam[80];
  ObjGX   *vTab;
  // ColRGB  defCol;

// static char oldNam[128];


  printf("TSU_exp_Open |%s|\n",modNam);
  printf("   TSU_ftyp=%d\n",TSU_ftyp);


  if(modNam == NULL) {    // Init
    // UtxTab_clear (&smNameTab);             // init (malloc ..)
    // UtxTab_add (smNameTab, "main");        // add ModelName to textTable
    // UtxTab_query (&actModNam, smNameTab);
    strcpy(actModNam, "main");  // name of subModel being resolved ...
    return 0;
  }


  // Names in WRL duerfen keinen Punkt im Modelname haben !!!
  strcpy(mnam, modNam);
  // UTX_chg_chr1 ('_', '.', mnam);
  UTX_safeName (mnam, 1); // change '. /\\'

  
  // AP_dir_save AP_mod_fnam TSU_ftyp
  if(strlen(mnam) > 0) goto L_SM;      // goto main / subModel




  //=================================================================
  // open mainModel
  // fprintf(TSU_fp,"# TSU_exp_Open |%s|\n",mnam); // CRASH !
  // printf(" TSU_exp_Open mainModel |%s|\n",TSU_fnam);

  
  if(TSU_ftyp == Mtyp_WRL2) {
    sprintf(cbuf, "%sexport2.temp",OS_get_tmp_dir());

  } else {
    strcpy(cbuf, TSU_fnam);

  }

  goto L_write;




  //=================================================================
  // open subModel
  L_SM:


  // STL: explode subModel
  // explode subModel: dazu muss ein .tess ausgegeben werden;
  if(TSU_ftyp == Mtyp_STL) {
    sprintf(cbuf, "%s%s.tess",OS_get_tmp_dir(),mnam);


  // WRL: echte subModels exportieren.
  } else {
    // create filename /tmp/Exp_<subModelname>
    sprintf(cbuf,"%sExp_%s",OS_get_tmp_dir(),mnam);

    // geht nicht - es kommt falscher modelName !
    // i1 = strlen(mnam);
    // if(i1 > 128) i1 = 125;
    // strncpy(oldNam, mnam, i1);
    // oldNam[i1] = '\0';
  }

    // printf(" _exp_Open |%s|\n",cbuf);



  //----------------------------------------------------------------
  L_write:
  if(TSU_fp) {

    // es ist ein subModel open; write Daten, close File.

    // die gepufferten Daten holen ...
    i1 = TSU_Init (3, &vTab);
    if(i1 > 1) {       // 1 (size)record is empty !
    // die gepufferten Daten ausgeben ...
      TSU_exp_sur (vTab);
    }

    // close-Text subModel (mainModel: TSU_exp_close)
    TSU_exp_close (mnam);

    // reset memSpace
    TSU_Init (2, NULL);

    // close file
    if(TSU_fp) {
        // printf(" fclose TSU_fp ..\n");
      fclose (TSU_fp);
      TSU_fp = NULL;
    }
  }


  // open file for next subModel
    // printf(" fopen TSU_fp |%s|%d\n",cbuf,TSU_ftyp);
  if(TSU_ftyp == Mtyp_DXF) {
    TSU_fp = fopen(cbuf,"a");   // add to File; Lines usw schon draussen ..

  } else if(TSU_ftyp == Mtyp_WRL2) {
    TSU_fp = fopen(cbuf,"w");

  } else {
    TSU_fp = fopen(cbuf,"wb");
  }


  if(TSU_fp == NULL) {
    TX_Print("TSU_exp__ Open E001 %s",mnam);
    return -1;
  }



  //----------------------------------------------------------------
  // Write init-Texte.
  TSU_exp_init (mnam);

  // printf(" TSU_exp_Open subModel |%s|\n",mnam);
  // UtxTab_add (smNameTab, mnam);    // add ModelName to textTable
  // UtxTab_query (&actModNam, smNameTab);
  if(strlen(mnam) > 0)  {
    strcpy(actModNam, mnam);
    // UTX_chg_chr1 ('_', '.', actModNam);
    UTX_safeName (actModNam, 1); // change '. /\\'

  } else {
    strcpy(actModNam, "main");
  }

  return 0;


}


//================================================================
  int TSU_exp_Ditto (int iNr, char *modNam, ModelRef *mdr) {
//================================================================
// iNr: 0=first call, 1=further call.

  int        i1, f_tess;
  char       cbuf[256], cbuf1[256], mnam[128], *cp;
  ColRGB     defCol;


  printf("TSU_exp_Ditto |%s| iNr=%d ftyp=%d\n",modNam,iNr,TSU_ftyp);


  // fprintf(TSU_fp,"#TSU_exp_Ditto |%s| %d\n",mnam,iNr);


  //----------------------------------------------------------------
  // testen, ob ein .tess-Model existiert und flag f_tess setzen
  // remove filetyp from modelname
  strcpy(cbuf, modNam);
  cp = strrchr(cbuf, '.');
  if(cp) *cp = '\0';
  UTX_safeName (cbuf, 1); // change '. /\\'
  strcat(cbuf, ".tess");
  sprintf(cbuf1, "%s%s",OS_get_tmp_dir(),cbuf);
  f_tess = OS_checkFilExist (cbuf1, 1);   // 0=does not exist
    printf(" f_tess=%d |%s|\n",f_tess,cbuf1);



  //----------------------------------------------------------------
  // Names in WRL duerfen keinen Punkt im Modelname haben !!!
  strcpy(mnam, modNam);
  // UTX_chg_chr1 ('_', '.', mnam);
  UTX_safeName (mnam, 1); // change '. /\\'
    // printf(" mnam=|%s|\n",mnam);



  // ======== DXF =============================================  2016-05-23
  if(TSU_ftyp == Mtyp_DXF) {
      printf(" DXF < tess\n");




  // ======== WRL / VRML1 =====================================
  } else if(TSU_ftyp == Mtyp_WRL) {                 // 10=WRL
    
    if(f_tess == 0) goto L_wrl1_1;
    TSU_exp_sm__ (cbuf, mdr);
    goto L_done;


    //----------------------------------------------------------------
    L_wrl1_1:
    fprintf(TSU_fp,"Separator {\n");

    // newPosition: mdr->po, mdr->vx, mdr->vz);
    // Write Matrix.
    TSU_exp_wrl1Mat (&mdr->vx, &mdr->vz, &mdr->po);

    if(iNr == 0) {                       // 0=first call

      // Add "DEF <modelname>", Include .EXP-File.
      fprintf(TSU_fp," DEF %s\n",mnam);
      sprintf(cbuf,"%sExp_%s",OS_get_tmp_dir(),mnam);
      i1 = UTX_cat_file (TSU_fp, cbuf);
      if(i1 < 0) goto L_e1;

    } else {
      // this is further call. Add "USE <modelname>"
      fprintf(TSU_fp,"  USE %s\n",mnam);
    }

    fprintf(TSU_fp,"}\n");

    goto L_done;



  // ======== WRL / VRML2 =====================================
  } else if(TSU_ftyp == Mtyp_WRL2) {


    if(f_tess == 0) goto L_wrl2_1;
    TSU_exp_sm__ (cbuf, mdr);
    goto L_done;


    //----------------------------------------------------------------
    L_wrl2_1:
/*
    // DefaultColor raus ..
    if(iNr == 0) {                       // 0=first call
      // hier colors falsch ..
      // GL_DefColGet (&defCol);            // get DefaultColor
      // DEB_dump_obj__ (Typ_Color, &defCol, "GL_DefColGet");
      // TSU_exp_wrl2Col (&defCol);
      // TSU_exp_wrl2Col (&AP_defcol);
    }
*/

    fprintf(TSU_fp,"Transform {\n");

    // Write "translation .. rotation .."
    // newPosition: mdr->po, mdr->vx, mdr->vz);
    TSU_exp_wrl2Mat (&mdr->vx, &mdr->vz, &mdr->po);

    if(iNr == 0) {                       // 0=first call

      // Add "DEF <modelname>", Include .EXP-File.
      // fprintf(TSU_fp," DEF %s children [\n",mnam);
      fprintf(TSU_fp," children DEF %s Group { children [\n",mnam);
      sprintf(cbuf,"%sExp_%s",OS_get_tmp_dir(),mnam);
      // fprintf(TSU_fp,"#--------- importfile start\n");
        // printf(" wrl- cat_file |%s|\n",cbuf);
      i1 = UTX_cat_file (TSU_fp, cbuf); // fertige Datei hintanhaengen
      // fprintf(TSU_fp,"#--------- importfile end\n");
      if(i1 < 0) goto L_e1;
      fprintf(TSU_fp,"] } }\n");

    } else {
      // this is further call. Add "USE <modelname>"
      fprintf(TSU_fp," children [ USE %s ] }\n",mnam);
    }


    goto L_done;



  // ======== STL =====================================
  } else if(TSU_ftyp == Mtyp_STL) {            // 11=OBJ
    // das sm.tess laden; ins sm-refsys transformieren; ausgeben.
    if(f_tess == 0) goto L_e1;
    TSU_exp_sm__ (cbuf, mdr);
    goto L_done;




  // ======== OBJ =====================================
  } else if(TSU_ftyp == Mtyp_OBJ) {            // 11=OBJ
    if(iNr == 0) {                       // 0=first call
      sprintf(cbuf,"%sExp_%s",OS_get_tmp_dir(),mnam);
      i1 = UTX_cat_file (TSU_fp, cbuf);
      if(i1 < 0) goto L_e1;
    }

    ++TSU_errStat;     // subModels werden nicht positioniert !!!
    goto L_done;




  // ======== TESS =====================================
  } else if(TSU_ftyp == Mtyp_TESS) {            // 12=TESS

    ++TSU_errStat;     // subModels werden nicht behandelt
    goto L_done;



  // ======== ?? =====================================
  } else {
    printf("***** TSU_exp_Ditto I001 %d\n",TSU_ftyp);
  }




  L_done:
  return 0;


  //----------------------------------------------------------------
  L_e1:
    // TX_Error("TSU_exp_Ditto E001");
    TX_Print("cannot export %s",mnam);
    ++TSU_errStat;
    return -1;

}


//================================================================
  int TSU_exp_Mod (int mode, ModelRef *mdr) {
//================================================================
// export subModel-Call (Ditto).
// DZT NUR FUER WRL !!
// Must be initialized with (mdr == NULL) !!
// VRML2: see [ Inline { url "fn.wrl" } ]

//  mode =  0: normal Operation; export Model
//  mode = -1: primary Init; remove all Files tmp/Exp_*;  Init TSU_exp_Open
//  mode = -2: Init mStat (ob Model already used or not)



static char  mStat[1024];

  int        i1;
  ModelBas   *mdb;
  char       cbuf[256];



  if(mode  < 0) {

    // Init mStat (ob Model already used or not) = clear used-buffer
    // use it mit mode=-2
    for(i1=0; i1<1024; ++i1) mStat[i1] = 0;


    if(mode == -1) {
      // printf("TSU_exp_Mod Init\n");  // File not yet open !
      // delete all ../tmp/Exp_*
      sprintf(cbuf,"%sExp_*",OS_get_tmp_dir());
      OS_file_delGrp (cbuf);
      TSU_exp_Open (NULL);        // actModNam=main (wenns keine UP's gibt ?)
    }

    return 0;
  }


  if(mdr->modNr >= 1024) {TX_Error("TSU_exp_Mod E001"); return -1;}

  // get basicModel
  mdb = DB_get_ModBas (mdr->modNr);
  if(mdb == NULL) {TX_Error("TSU_exp_Mod E002"); return -1;}
    // printf(" mnam=|%s|\n",mdb->mnam);


  // printf("===========================================\n");
  // printf("TSU_exp_Mod %d |%s|\n",mdr->modNr,mdb->mnam);
  // fprintf(TSU_fp, "# TSU_exp_Mod %d |%s|\n",mdr->modNr,mdb->mnam);


  TSU_exp_Ditto (mStat[mdr->modNr], mdb->mnam, mdr);

  // Submodel als existent merken ..
  mStat[mdr->modNr] = 1;

  return 0;

}


//================================================================
  int TSU_exp__ (char *mode, char *fnam) {
//================================================================
// initiate Redraw, collect all vertices, give out vertices as:
// mode="obj" - write WaveFront-OBJ-File. Only v (Vertices) anf f (faces).
//
// Input:
//   mode   "               Mtyp_WRL2
//                          Mtyp_WRL
//                          Mtyp_OBJ
//          "DXF"           Mtyp_DXF
//                          Mtyp_TESS
// Output:
//   RetCod:     nr of tesselated triangles;  negativ=error.
//


// TSU_exp__
//   ED_work_END ED_work_CurSet
//     TSU_exp_Open
//       TSU_exp_init
//     TSU_exp_Ditto   < TSU_exp_Mod < GR_DrawModel
//       TSU_exp_wrl2Mat
//       TSU_exp_sm__
//         TSU_exp_sm_sur
//           TSU_exp_fac
//   TSU_exp_sur
//     TSU_exp_wrl2Fac
//   TSU_exp_close


  int    irc, i1, recNr;
  long   l1;
  void   *pv1;
  ObjGX  *vTab;


  printf("||||||||||||||||||||| TSU_exp__ |||||||||||||||||||||||||||||\n");
  printf(" |%s|%s|\n",mode,fnam);


  TSU_errStat = 0;
  TSU_fnam = fnam;
  TSU_ftyp = AP_iftyp_ftyp (mode);
    printf(" TSU_ftyp=%d\n",TSU_ftyp);


  // VRML-1 or VRML-2 ?
  if(TSU_ftyp == Mtyp_WRL) { // 2010-01-21
    if(AP_stat.subtyp == 1) TSU_ftyp = Mtyp_WRL2;
  }

/*
  if((TSU_fp=fopen(fnam,"wb")) == NULL) {
    TX_Print("TSU_exp__ Open E001 %s",fnam);
    return -1;
  }
*/

  irc = 0;
  TSU_ptNr = 0;


  // init
  TSU_exp_Mod (-1, NULL);


  // prepare ProtoList (VR2 only)
  if(TSU_ftyp == Mtyp_WRL2) { 
    // get nr of basic-textures TexBasNr
    i1 = Tex_tbNr ();
    pv1 = MEM_alloc_tmp ((int)(i1 * sizeof(int)));
    TSU_exp_wrlProto (-i1, pv1);
  }


  //----------------------------------------------------------------
  // start tesselation-mode store
  irc = TSU_Init (1, &vTab);
  if(irc < 1) {
    // we must close dxf-file !
    if(TSU_ftyp == Mtyp_DXF) {
      TSU_fp = fopen(fnam,"a");
      TSU_exp_close ("");
      fclose (TSU_fp);
      TSU_fp = NULL;
    }
    return irc;
  }


  // Start a Redraw (fills vTab)
  irc = ED_work_END (0);


  // reset tesselation; get accumulated Vertices (of MainModel)
  recNr = TSU_Init (0, &vTab);
    printf(" recNr=%d\n",recNr);
  if(recNr < 0) {irc = -1; goto L_fertig;}
  // recNr ist nun die Anzahl exportierter surfs (nicht die Anzahl von Dittos);

  if(irc < 0) goto L_fertig;  // zB Fehler Open ..
  irc = recNr;


  //----------------------------------------------------------------
  // obj und tess haben dzt keine Dittos.
  // TESS kann keine subModels exportieren; nur primary Model.
  if(TSU_ftyp == Mtyp_TESS) {  // TESS
    if(recNr < 2) {
      TX_Error("0 surfaces exported (TESS cannot export subModels)");
      goto L_fertig;
    }
  }

  // // Test: geht Display wieder ?
  // UT3D_pt_3db (&pt1, 36., 36., 10.);
  // GR_tDyn_symB__ (&pt1, SYM_STAR_S, 2);


  // work (export ....); macht auch closing "}".
  TSU_exp_sur (vTab);


  // WRL: noch ein closing "}"
  if(TSU_fp) TSU_exp_close("");


  // // TEST ONLY: Display
  // TSU_DrawSurTess (vTab);



  // free memspace
  L_fertig:
    printf(" TSU_errStat=%d irc=%d\n",TSU_errStat,irc);


  // OBJ: exportiert subModels aber ohne positionieren;
  if(TSU_ftyp == Mtyp_WRL) {         // WRL
    if(TSU_errStat > 0) {
      TX_Print("%d  ERRORS detected",TSU_errStat);
    }

  } else if(TSU_ftyp == Mtyp_OBJ) {         // OBJ
    if(TSU_errStat > 0) {
      TX_Print("%d  ERRORS detected (cannot set subModel-position)",TSU_errStat);
    }

  } else if(TSU_ftyp == Mtyp_TESS) {  // TESS
    if(TSU_errStat > 0) {
      TX_Print("%d  ERRORS detected / (cannot export subModels)",TSU_errStat);
    }

  } else if(TSU_ftyp == Mtyp_DXF) {  // DXF
    TX_Print("   surfaces exported:  %d",recNr);
  }


  if(AP_errStat_get() == 0) {
    TX_Print("%s exported %s",mode,fnam);
  }



  if(vTab) free (vTab);

  if(TSU_fp) {fclose (TSU_fp); TSU_fp = NULL;}


  // finish
  if(TSU_ftyp == Mtyp_WRL2) {
    // VRML2: close Headerfile, concatenate files.
    TSU_exp_wrlInit (0, 0);
    // rename fnam -> export2.temp
    sprintf(memspc011, "%sexport1.temp",OS_get_tmp_dir());
    sprintf(memspc012, "%sexport2.temp",OS_get_tmp_dir());
    // OS_file_rename (fnam, memspc012);
    // concatenate: fnam = export1.temp + export2.temp
    OS_file_concat (fnam, memspc011, memspc012);


  } else if(TSU_ftyp == Mtyp_OBJ) {         // OBJ
    // OBJ: write matl-libraryFile
    TSU_exp_objCol (1, NULL);
  }


  // UI_wait_Esc ();   // TESTONLY; display 


  printf("||||||||||||||||||||| ex TSU_exp__ |||||||||||||||||||||||||||||\n");

  // exit(0); // TEST

  return irc;

}


//================================================================
  int TSU_imp_tess (Memspc *impSpc, char *fnam) {
//================================================================
// import Mockup from file into Mockup-struct
// must free impSpc (UME_free (&impSpc))


  int    irc, oSiz;
  long   il;
  FILE   *fpi=NULL;


  printf("TSU_imp_tess |%s|\n",fnam);

  // get filesize
  il = OS_FilSiz (fnam);
  if(il < 1) {TX_Print("TSU_imp_tess FileExist E001 %s",fnam); return -1;}
  printf(" fSiz=%ld\n",il);


  if((fpi=fopen(fnam,"rb")) == NULL) {
    TX_Print("TSU_imp_tess Open E001 %s",fnam);
    return -1;
  }


  oSiz = il + 2046;


  irc = UME_malloc (impSpc, oSiz, 500000);
  if(irc < 0) return irc;

  // gesamtes file einlesen und relocaten; skips 1 record (address)
  irc = tess_read_f (impSpc->start, &oSiz, il, fpi);
  if(irc < 0) UME_free (impSpc);
  fclose (fpi);
  // printf("ex TSU_imp_tess %d\n",irc);

  return irc;

}


/*
//================================================================
  int TSU_imp__ (Memspc *impSpc, char *fnam) {
//================================================================
// TSU_imp__         import tesselated surf
// ACHTUNG: hier malloc fuer impSpc; Caller must free impSpc !


  int    irc, oSiz;
  long   il;
  char   mode[32];


  printf("|||||||||||| TSU_imp__ |%s|\n",fnam);


  // get filesize
  il = OS_FilSiz (fnam);
  if(il < 1) {TX_Print("TSU_imp__ FileExist E001 %s",fnam); return -1;}
  printf(" fSiz=%d\n",il);



  // extract filetype.
  irc = UTX_ftyp_s (mode, fnam, 1);
  if(irc < 0) {TX_Print("TSU_imp__ FileType not found"); return -1;}



  if((TSU_fp=fopen(fnam,"rb")) == NULL) {
    TX_Print("TSU_imp__ Open E001 %s",fnam);
    return -1;
  }



  //======= VRML ====================================================
  // import VRML as tesselated; see also obj_read__
  if(strcmp(mode, "WRL")) goto L_tess;
  irc = wrl_readTess__ (impSpc, TSU_fp, il);
  goto L_exit;



  //======= TESS ===================================================
  // import preTesselated
  L_tess:
  if(strcmp(mode, "TESS")) goto L_obj;
  oSiz = il + 2046;


  irc = UME_malloc (impSpc, oSiz, 500000);
  if(irc < 0) goto L_exit;

  // gesamtes file einlesen und relocaten
  irc = tess_read_f (impSpc->start, &oSiz, il, TSU_fp);
  goto L_exit;




  //======= OBJ ====================================================
  // import OBJ as tesselated; see also obj_read__
  L_obj:
  if(strcmp(mode, "OBJ")) goto L_imp_err;
  irc = obj_readTess__ (impSpc, TSU_fp);
  goto L_exit;



  //================================================================
  L_imp_err:
  TX_Print("TSU_imp__ FilType not supported |%s|",mode);
  irc = -1;
  // goto L_exit;


  //================================================================
  L_exit:
  fclose (TSU_fp);
  printf("ex TSU_imp__ %d\n",irc);
  return irc;

}
*/

//====================== EOF =============================
