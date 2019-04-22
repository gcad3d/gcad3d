//   Import STEP.                   2005-09-05    RF
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
modSiz nicht gut, da alle CART.PT benutzt werden; besser waere nur Punkte
 von echten Objekten (Flaechen usw) zu benutzen.
B_SPLINE_CURVE(2,(#9450,#9451,#9452),.CIRCULAR_ARC., ..
  create Circle, not RBSP from that ?
Colors:
../formate/step/Models/LC_X.stp
#534 = STYLED_ITEM( '', ( #1521 ), #1522 );
 // STYLED_ITEM ((style), object)
 #1521 = PRESENTATION_STYLE_ASSIGNMENT( ( #3021 ) );
  #3021 = SURFACE_STYLE_USAGE( .BOTH., #4502 );
   #4502 = SURFACE_SIDE_STYLE( '', ( #8848 ) );
    #8848 = SURFACE_STYLE_FILL_AREA( #10849 );
     #10849 = FILL_AREA_STYLE( '', ( #13527 ) );
      #13527 = FILL_AREA_STYLE_COLOUR( '', #17439 );
       #17439 = COLOUR_RGB( '',0.645180702,0.678915679,0.699999988 );
 #1522 = ADVANCED_FACE( '', ( #3022 ), #3023, .T. );
see also CURVE_STYLE

// Assembly1.stp:
#990=COLOUR_RGB('LIGHT_BLUE',0.0,0.660000026226044,1.0);
#991=DRAUGHTING_PRE_DEFINED_CURVE_FONT('continuous');
#992=CURVE_STYLE('',#991,POSITIVE_LENGTH_MEASURE(0.1),#990);
#993=FILL_AREA_STYLE_COLOUR('',#990);
#994=FILL_AREA_STYLE('',(#993));
#995=SURFACE_STYLE_FILL_AREA(#994);
#996=SURFACE_SIDE_STYLE('',(#995));
#997=SURFACE_STYLE_USAGE(.BOTH.,#996);
#998=PRESENTATION_STYLE_ASSIGNMENT((#992,#997));
// #999=DRAUGHTING_PRE_DEFINED_COLOUR('green');
// #1000=CURVE_STYLE('',#991,POSITIVE_LENGTH_MEASURE(0.1),#999);
#1448=STYLED_ITEM('',(#998),#1446);
#1446=GEOMETRIC_CURVE_SET('',(#1436,#1439));


-----------------------------------------------------
Modifications:
2017-04-07 new format trimmed-curve in gcad. RF.
2015-02-18 STP_r_readLn bug reading comments fixed. RF.
2014-02-09 Typ_AC -> Typ_CI
2012-11-28 rewritten. RF.
2012-11-01 Bug Step-import CYLINDRICAL_SURFACE from 2 circles fixed. RF.
2005-09-05 RF.

-----------------------------------------------------
*/
/*!
\file  ../exp/stp_r.c
\brief read STeP file 
\code

_skip..   skip in Step-Text
_dec..    decode from Step-Text

=====================================================
List_functions_start:

STP_r__                mainentry; Einlesen, intern speichern; abarbeiten
STP_r_dec0             Inputline zerlegen
STP_r_dec1             eine Zeile decodieren u intern speichern

STP_r_creMod_Run       main od subModel ausgeben
STP_r_cre2             resolv and create step-object sInd
STP_r_creObj1          cr. APT-Text from objekt (Ausgabe)

STP_r_crePt0           create Point from existing CARTESIAN_POINT
STP_r_creLn0           create Line from SC_LINE
STP_r_creLn1           create Line from SC_LINE
STP_r_creLn2           create Line from SC_LINE + 2 parameters (length)
STP_r_creCi1
STP_r_creCi2
STP_r_creCi_0          Text "ARC P# P# .." generieren ..
STP_r_creCi_1          create text "ARC ...."
STP_r_creCi_2          create text "cen rad dz"
STP_r_creEl1 2         S=ELL 
STP_r_crePln1          cr. R= from AXIS2_PLACEMENT_3D
STP_r_creSurCyl1       cr. CYLINDRICAL_SURFACE "A=CYL .."
STP_r_creSur1          cr. ADVANCED_FACE (A=FSUB)
STP_r_creSur2          cr. "SPH" "CYL" "SRV"
STP_r_creSur3          cr. CONICAL_SURFACE "A=SRV .."
STP_r_creSur4          cr. "SPH .."
STP_r_creSur5          cr. TOROIDAL_SURFACE "B=TOR .."
STP_r_creSur6          cr. SURFACE_OF_REVOLUTION "A=SRV .."
STP_r_creSur7          cr. SURFACE_OF_LINEAR_EXTRUSION "A=SRU .."
STP_r_creSur8          cr. B_SPLINE_SURFACE_WITH_KNOTS "A=BSP .."
STP_r_creSur9          cr. BOUNDED_SURFACE "A=RBSP .."

STP_r_addPT            add P5 or P(0 0 5) --> gTxt
STP_r_addVC            add D5 or D(0 0 5) --> gTxt

STP_r_PLN_AXIS2        Plane from s_tab-index of AXIS2_PLACEMENT_3D
STP_r_VC_DIR           Vector from s_tab-index of DIRECTION
STP_r_VC_VEC_1         Vector from s_tab-index of VECTOR (length unmodified; 1.)
STP_r_VC_VEC_t         Vector from s_tab-index of VECTOR with length
STP_r_PT_CARTPT        Point from s_tab-index of CARTESIAN_POINT
STP_r_PT_VERT          Point from s_tab-index of VERTEX_POINT
STP_r_PT_sInd          Point from s_tab-index of VERTEX_POINT or CARTESIAN_POINT

STP_r_decModel4

STP_r_savInit          init next s-obj-Record
STP_r_savInt           save 1 int
STP_r_savLDL           fixe Anzahl Links/doubles/Logical decodieren & speichern
STP_r_savLinksLog      decode & save Links + 1 Logical; NOT YET IMPL
STP_r_savLinkB         1-n bracketed Links decodieren und speichern
STP_r_savDB            1-n bracketed doubles decodieren und speichern

STP_r_ckTypB           check typ of Obj's in Klammer
STP_r_skipT            skip Textfield
STP_r_skipB            skip Block (...)
STP_r_skipLog1   
STP_r_skipObjNam
STP_r_skipWords        skip words (find next ',')
STP_r_skipTer1         skip terminating characters  "," or ")" or "),"

STP_r_decIntB
STP_r_decInts
STP_r_decInt1
STP_r_decDbB
STP_r_decDbs
STP_r_decDb1
STP_r_decLog1
STP_r_decLink2B        decode sqare-block of links
STP_r_decLinkB
STP_r_decLinks
STP_r_decLink1         dec
STP_r_decTx1           decode & save Textfield ('xxx') 

STP_r_getDb            eine db aus dem mem lesen; pointer weitersetzen.
STP_r_getInt           eine int aus dem mem lesen; pointer weitersetzen.
STP_r_getSkip          skip objects in dataArea
STP_r_Link_data        eine int aus dem mem lesen. REPLACE WITH STP_r_get_L1

STP_r_get_typ_L        get step-typ from step-link
STP_r_get_L1 2 3       get 1/2/3.step-link from s_tab-index
STP_r_findInd          get s_tab-index of Record from LinkNr
STP_r_find_sRec_TypIdL1 get index of Recod sTyp dessen ID == L1 of Record iL1
STP_r_find_sRec_TypL1  find index of Recod sTyp from its L1
STP_r_find_sRec_TypL2  find index of Recod sTyp from its L2
STP_r_findDetNam       find Detailname for Ditto
STP_r_findPROD         Index PRODUCT in alle SHAPE_REPRESENTATION eintragen
STP_r_wrong_SRR        check if REPRESENTATION_RELATIONSHIP is wrong (CATIA-V4)

STP_r_nxtSrec          Init s_tab;
STP_r_readLn           read next Line from File

TESTFUNKTIONEN:
STP_r_TypTxt_sTyp      give Text of styp ("DIRECTION" from 1)
STP_r_dispSym1         display Symbol at Position (star, triang ..)

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



===================================================================
Step-Specif/documentation siehe ../exp/stp_w.c
/mnt/serv1/Devel/dev/gCAD3D/formate/step/DIP-3631.pdf


Gesamte Datei einlesen; Zeilenende ist ";" alle brauchbaren Daten in eigenen
structs speichern.

Zeilenbuffer (Lesen aus step-Datei) ist mem_cbuf1

Fuer jedes gelesene Objekt gibt es eine struct mit Aufbau s_obj.

Die Daten zum Objekt sind in einem Memorybereich; der erforderliche Platz im 
 Datenbereich ist variabel; bei CARTESIAN_POINT sinds 3 doubles (Coords !)

Wenn alle eingelesen sind:
Alle verschiedenen Modelle im Stepfile TopDown decodieren.
 dazu zuerst die TopLevel-Objekte aktivieren, diese dann TopDown ausgeben.
1) Alle TopLevel-Objekte des OpenPart (WIREFRAME_SHAPE_REPRESENTATION und
   SURFACE_SHAPE_REPRESENTATION) aktivieren, dann ausgeben.
2) Alle TopLevel-Objekte der Solids aktivieren, dann ausgeben.



// read all step-records; each s_tab is one step-record.
s_tab[]   s_obj*
          .sTyp  the StepTyp, eg  SC_CARTESIAN_POINT
          .sDat  points to data (in s_datSpc)
          .gDat

i_tab     (int*) gives the s_tab-index from a step-link
          s_tab-index = i_tab[<step-link>];

mdlTab    STP_MDL
          .iPROD = link to PRODUCT,
          .nam = PRODUCT-name.

refTab    STP_I2
          .ii = link to subModelRef (NEXT_ASSEMBLY_USAGE_OCCURRENCE)
          .iMdl = ?   (s_tab-index of PRODUCT)

geoTab    STP_I2      List of models;
          .ii = link to ADVANCED_BREP_SR
          .iMdl = ?



//----------------------------------------------------------------
TESTMETHODE   DEBUG:
  activate AP_deb_stat (1) in STP_r__
    if compiled with -DDEB:
    all testoutput of "printd" goes into file <tempDir>/debug.dat

 f. Debug im ../.gdbinit     break OS_debug_dll_  u. run tst3
  make -f xa_stp_r.mak&&./go d

 see Lines printf("Import ...
 return -4    beendet sofort !


//----------------------------------------------------------------
ABLAUF:
STP_r__               mainentry
  STP_r_readLn        read next line -> mem_cbuf1
  STP_r_dec0          decode line & fill s_tab[]
    STP_r_dec1
  .. activate objects
  STP_r_creMod_Run     create all activated objects
    STP_r_cre2         create obj




=================================================================
Formate:
  NormalObj:
#167=LINE('Line',#164,#166);
  CompoundObj:    has more than 1 InfoBlock ...
#1=(subCmd1(..)subCmd2(..));


Surfaces:
#453=ADVANCED_FACE('F3',(#452),#416,.T.);
#416=(BOUNDED_SURFACE()B_SPLINE_SURFACE(..)
      B_SPLINE_SURFACE_WITH_KNOTS(..)
      GEOMETRIC_REPRESENTATION_ITEM()RATIONAL_B_SPLINE_SURFACE(..)
      REPRESENTATION_ITEM('F3')SURFACE());
// sequence can be different !




=================================================================
STEP: External files:

1. muss dazu ein Step-File als external Referenz moeglich sein
   ("on-the-fly-import) !
2. Unterscheidung internalModel - externalModel ?
   Wie externalModel im aktiven InputDirectory ?

Einziges Example:
/mnt/win_d/dev/gCAD3D/formate/step/Models/Motor_montiert214extra.stp

#496=DOCUMENT_TYPE('geometry') ;
#502=DOCUMENT_REPRESENTATION_TYPE('digital',#495) ;
#503=PROPERTY_DEFINITION('external definition','',#495) ;
#497=APPLIED_EXTERNAL_IDENTIFICATION_ASSIGNMENT('Kolben.stp',#498,#494,(#495)) ;
#494=EXTERNAL_SOURCE(IDENTIFIER('')) ;
#498=IDENTIFICATION_ROLE('external document id and location',$) ;
#495=DOCUMENT_FILE('Kolben.stp','','',#496,'',$) ;
#499=APPLIED_DOCUMENT_REFERENCE(#495,'',(#492)) ;
  // 492 ist PRODUCT_DEFINITION

Es kann (entspr.pdmug_release3.pdf) Filename aber in EXTERNAL_SOURCE sein !!!
 Catia: EXTERNAL_SOURCE(IDENTIFIER('')) meint wahrscheinlich:
   IDENTIFIER('') == Filename == DOCUMENT_FILE-ID








=================================================================
Ungeloest-Farben:

Voellig wirr ...
#2024=SURFACE_STYLE_FILL_AREA(#2023);
 #2023=FILL_AREA_STYLE('',(#2022));
  #2022=FILL_AREA_STYLE_COLOUR('',#2021);
   #2021=COLOUR_RGB('',0.78039215686,0.78039215686,0.0);

#2053=COLOUR_RGB('',1.0,0.56862745098,0.0);

FILL_AREA_STYLE_COLOUR FILL_AREA_STYLE
SURFACE_STYLE_FILL_AREA SURFACE_SIDE_STYLE SURFACE_STYLE_USAGE
PRESENTATION_STYLE_ASSIGNMENT OVER_RIDING_STYLED_ITEM
STYLED_ITEM SURFACE_STYLE_USAGE





*/



#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>              // for ...
#include <ctype.h>               // isalpha


#ifdef _MSC_VER
// die folgenden Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int STP_r__ (char*);
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_cast.h"                // INT_PTR
#include "../ut/ut_ox_base.h"             // OGX_SET_INDEX

#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_os.h"               // OS_get_bas_dir ..
#include "../ut/ut_obj.h"              // UTO_stru_2_obj
#include "../ut/ut_txfil.h"            // UTF_GetPosLnr
#include "../ut/ut_memTab.h"           // MemTab_..

#include "../ci/NC_Main.h"             // WC_ask_ModSiz

#include "../ut/func_types.h"               // UI_Func... SYM_..
#include "../ut/gr_types.h"               // SYM_* ATT_* LTYP_*

#include "../db/ut_DB.h"               // DB_GetGTxt

#include "../xa/xa_mem.h"              // memspc.., mem_cbuf1
#include "../xa/xa.h"                  // AP_STAT printd
#include "../xa/xa_obj_txt.h"          // AP_obj_add_obj




#define SC_DIRECTION                    1
#define SC_CARTESIAN_POINT              2
#define SC_VERTEX_POINT                 3
#define SC_VECTOR                       4
#define SC_AXIS1_PLACEMENT              5
#define SC_AXIS2_PLACEMENT_3D           6
#define SC_LINE                         7
#define SC_CIRCLE                       8
#define SC_ELLIPSE                      9

#define SC_COMPOSITE_CURVE             10
#define SC_COMPOSITE_CURVE_SEGMENT     11
#define SC_TRIMMED_CURVE               12
#define SC_QUASI_UNIFORM_CURVE         13
#define SC_B_SPLINE_CURVE              14
#define SC_B_SPLINE_CURVE_WITH_KNOTS   15
#define SC_RATIONAL_B_SPLINE_CURVE     16
#define SC_BOUNDED_CURVE               17
#define SC_INTERSECTION_CURVE          18
#define SC_VERTEX_LOOP                 19

#define SC_FACE_OUTER_BOUND            20
#define SC_FACE_BOUND                  21
#define SC_EDGE_LOOP                   22
#define SC_ORIENTED_EDGE               23
#define SC_EDGE_CURVE                  24
#define SC_CONNECTED_EDGE_SET          25     // see also GEOMETRIC_SET usw

#define SC_ITEM_DEFINED_TRANSFORMATION 26
#define SC_NEXT_ASSEMBLY_USAGE_OCCURR  27
#define SC_COLOUR_RGB                  28

#define SC_ADVANCED_FACE               30
#define SC_PLANE                       31
#define SC_SPHERICAL_SURFACE           32
#define SC_CYLINDRICAL_SURFACE         33
#define SC_CONICAL_SURFACE             34
#define SC_TOROIDAL_SURFACE            35
#define SC_DEGENERATE_TOROIDAL_SURFACE 36
#define SC_SURFACE_OF_REVOLUTION       37
#define SC_SURFACE_OF_LINEAR_EXTRUSION 38
#define SC_QUASI_UNIFORM_SURFACE       39
#define SC_B_SPLINE_SURFACE            40
#define SC_B_SPLINE_SURFACE_WITH_KNOTS 41
#define SC_RATIONAL_B_SPLINE_SURFACE   42
#define SC_BOUNDED_SURFACE             43

#define SC_RECTANGULAR_TRIMMED_SURFACE 45

#define SC_GEOMETRIC_SET               47
#define SC_GEOMETRIC_CURVE_SET         48

#define SC_EDGE_BASED_WIREFRAME_MODEL  50
#define SC_SHELL_BASED_SURFACE_MODEL   51
#define SC_OPEN_SHELL                  52
#define SC_CLOSED_SHELL                53
#define SC_MANIFOLD_SOLID_BREP         54

#define SC_GBOUNDED_SURFACE_SR         60 // GEOMETR.BOUNDED_SURF.SHAPE_REPRES.
#define SC_GBOUNDED_WIREFRAME_SR       61 // GEOMETR.BOUNDED_WIREFR.SHAPE_REPRES.
#define SC_EDGE_BASED_WIREFRAME_SR     62 // EDGE_BASED_WIREFR.SHAPE_REPRES.
#define SC_MANIFOLD_SURFACE_SR         63 // MANIFOLD_SURFACE_SHAPE_REPRESENTATION
#define SC_ADVANCED_BREP_SR            64 // ADVANCED_BREP_SHAPE_REPRESENTATION
#define SC_FACETED_BREP_SR             65 // FACETED_BREP_SHAPE_REPRESENTATION
#define SC_CONTEXT_DEPENDENT_SR        66

#define SC_SHAPE_DEFINITION_REPRES     70
        // SHAPE_DEFINITION_REPRESENTATION (2L)

#define SC_SHAPE_REPRESENTATION        71 // SHAPE_REPRESENTATION
#define SC_SHAPE_REPRES_RS             72 // SHAPE_REPRESENTATION_RELATIONSHIP
  // own record, not inside (REPRESENTATION_RELATIONSHIP - group !)

#define SC_REPRESENTATION_RELATIONSHIP 73
        // REPRESENTATION_RELATIONSHIP   (2L)
        // REPRESENTATION_RELATIONSHIP_WITH_TRANSFORMATION (1L)

#define SC_PRODUCT                     80
#define SC_PRODUCT_DEFINITION_FORMWSS  81
        // PRODUCT_DEFINITION_FORMATION (1L) and
        // PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE; (1L)

#define SC_PRODUCT_DEFINITION_WITH_AD  82
#define SC_PRODUCT_DEFINITION          83
        // PRODUCT_DEFINITION (1L)
#define SC_PRODUCT_DEFINITION_SHAPE    84

#define SC_GEOMETRIC_REPR_CTXT         90
#define SC_GLOBAL_UNCERT_ASS_CTXT      91
#define SC_UNCERT_MEASURE_WITH_UNIT    92
#define SC_LENGTH_MEASURE              93


// SURFACE_SIDE_STYLE
// SURFACE_STYLE_USAGE
// SURFACE_STYLE_FILL_AREA


typedef struct {int sInd, gInd; void *sDat, *gDat;
                unsigned char sTyp, gTyp, stat, uu;} s_obj;
// sInd  der StepIndex  aus #<sInd>=
// gInd  der gCad-APT-Index (nach Speichern; vorher -1)
// sDat  pointer zum Step-Datenblock --> s_datSpc
// gDat  pointer auf Daten zum gCad.obj; Spline: die Punkte zu v0/v1.
// sTyp  der StepTyp, zB  SC_CARTESIAN_POINT
// gTyp  der gCad-Typ; zB Typ_PT
// stat  temp; bit1- ob im aktiven Part; 2=done.

// if parameter starts with i, parametr is an index into s_tab;
//   else it is a step-link (starting with '#'))
typedef struct {int prod,        // "PRODUCT"
                    iProDefSha,  // "PRODUCT_DEFINITION_SHAPE"
                    iGeo,        // index of geometry-obj;
                    mode;        // 0=parent-axis; 1=child-axis
                char *nam;       // modelname (of "PRODUCT")
               } STP_MODEL;


typedef struct {int ii; int iMdl; } STP_I2;

typedef struct {int iPROD; char *nam; } STP_MDL;

// typedef_MemTab(int);
typedef_MemTab(STP_I2);
typedef_MemTab(STP_MDL);
// MemTab(int) mdlTab = _MEMTAB_NUL;
MemTab(STP_MDL) mdlTab = _MEMTAB_NUL;
MemTab(STP_I2) refTab = _MEMTAB_NUL;
MemTab(STP_I2) geoTab = _MEMTAB_NUL;




static s_obj     *s_tab;
static int       *i_tab,      // s_tab index from step-link
                 i_Max;       // nr of step-links (highest link)=size of i_tab
static int       s_Nr, s_Siz, s_Ind, s_Max, s_MainInd, MainStat;
static STP_MODEL mdl;
static int       mdlNr;
static Memspc    s_dat;
static Memspc    s_mSpc;                      // TempSpace

static char   *gTxt;
static int    resMod;
static int    angMod;
static int    wrong_SRR;    // 0=normal; 1=wrong
static int    wrong_ASS;    // 0=normal; 1=wrong
static int    indCol;
static int    modSiz;
static int    errTyp;       // 0=report error with TX_Print; else not
                            //  in STP_r_find_sRec_TypL1



// prototypes:
  char *STP_r_TypTxt_sTyp (int sTyp);
  void* STP_r_getInt (int *i1, void *ia);
  void* STP_r_getDb (double *d1, void *ia);
  void* STP_r_getSkip (void *pos, long csiz);


#define _tab_INC 50000
#define _dat_INC 5000000        // size for incrementing s_dat
#define MIND_SIZ 256000         // minimum free size in s_dat



// Protos:
  char* STP_r_mdl_nam__ (int is);
  // char* STP_r_mdl_nam1 (int iProDefSha);
  // char* STP_r_mdl_nam2 (int iProDef);


// get 1.link od s_tab-record sInd
#define STP_r_get_L1(sInd) ((int*)(s_tab[sInd].sDat))[0]
#define STP_r_get_L2(sInd) ((int*)(s_tab[sInd].sDat))[1]
#define STP_r_get_L3(sInd) ((int*)(s_tab[sInd].sDat))[2]

// get step-link from s_tab-index
#define IND_TAB(is) (is >= 0 ? s_tab[is].sInd : -1)


// STP_r_get_typ_L        get step-typ from step-link
#define STP_r_get_typ_L(sLink) (s_tab[i_tab[sLink]].sTyp)



/*
//================================================================
  int STP_r_get_ind_L1 (ind is) {
//================================================================
// get index from L1

  return i_tab[((int*)(s_tab[sInd].sDat))[0]]
*/


//================================================================
  int STP_r_alloc_tab (int newSiz) {
//================================================================
// alloc & realloc von s_tab.
// Bei realloc STP_r_reall_CB rufen.


  // int   newSiz;
  void  *oldAdr;


  if(s_tab == NULL) {
    // newSiz = 0;
    s_Siz  = newSiz;
  }

  newSiz = s_Siz + _tab_INC;

  printf("::::STP_r_alloc_tab %d %d %d\n",s_Siz,newSiz,_tab_INC);

  // while (Ind >= newSiz) newSiz += _tab_INC;
  // printf("  %d %d %d\n",Ind,APT_VR_SIZ,newSiz);

  oldAdr = s_tab;

  s_tab = (s_obj*)realloc(s_tab, newSiz*sizeof(s_obj));

  if(s_tab == NULL) {
    TX_Error ("******** out of memory - STP_r_alloc_tab *********");
    return -1;
  }

  L_init:
  s_Siz = newSiz;

  // correct adresses s_tab.sDat and s_tab.gDat
  if((oldAdr != NULL)&&(oldAdr != s_tab)) {
    if(STP_r_reall_CB (oldAdr, s_tab) < 0) return -1;
  }

  // printf("ex STP_r_alloc_tab %d %d\n",_tab_INC,newSiz);

  return 0;

}


//================================================================
  STP_r_reall_CB (void *oldAdr, void *newAdr) {
//================================================================
// Callback reallocate;
// correct adresses of s_tab.sDat and s_tab.gDat
// used by STP_r_alloc_tab and UME_add

  // printf("STP_r_reall_CB %p %p s_Nr=%d\n",oldAdr,newAdr,s_Nr);


  // modify all s_tab[].sDat - addresses
  // last record is already full ! (s_Nr + 1)
  // run UTA_reallTab (startAdr, offset, recNr, oldAdr, newAdr);
  if(UTA_reallTab (&(s_tab[0].sDat), sizeof(s_obj), s_Nr + 1,
                   oldAdr, newAdr) < 0) return -1;

  // modify all s_tab[].gDat - addresses
  if(UTA_reallTab (&(s_tab[0].gDat), sizeof(s_obj), s_Nr,
                   oldAdr, newAdr) < 0) return -1;

  return 0;
}
 
/*
///================================================================
  int _UTA_reallTab (char **startAdr, int offset, int recNr,
                    char *oldAdr, char *newAdr) {
///================================================================
/// reallocate a table of pointers
/// startAdr is the first address to be modified.
/// offset   is the distance to the next adress (the recordsize)

  unsigned long  i1, ad;
  char     *cpos;

  ad = newAdr - oldAdr;

  // printf("UTA_reallTab recNr=%d offset=%d ad=%d\n",recNr,offset,ad);
  // printf("   oldAd=%p newAd%p\n",oldAdr,newAdr);

  for(i1=0; i1<recNr; ++i1) {
    if(*startAdr != NULL) {
      *startAdr += ad;            // incr Inhalt
    }
    startAdr = (char*)startAdr + offset;
  }


  return 0;

}
*/


//================================================================
  int STP_r__ (char *fnam) {
//================================================================
// mainentry. 
// 1) Einlesen, intern speichern;
// 2) abarbeiten ..


 
  int     irc, ii, i1, i2, l1, l2, i3, i4, i5, i6, i7, oNr, *ia;
  int     isdr, lsdr, iobj, lobj;
  long    il1, il2;
  void    *vp;
  FILE    *fp1;
  char    *mdlNam, s1[256], s2[256], *p1;


  // printf("==================================\n");
  // printf("STP_r__ 2014-08-21 |%s|\n",fnam);


  // // prepare: remove all existing submodels ..
  // Mod_kill__ ();


  // TEST ONLY: set debug -> ON   ( vi ~/gCAD3D/tmp/debug.dat )
  // start debugging (following prints -> debug-file)
  // AP_deb_stat (1);          // 1=debug-ON (open file); 


  s_tab = NULL;
  s_Max = 0;
  indCol = 0;

  // get filsize
  il1 = OS_FilSiz (fnam);
    // printf(" fsiZ=%ld\n",il1);

  // Memory zuweisen.
  if(STP_r_alloc_tab (il1/50) < 0) return -4;

  // hier werden alle Daten (coords ...) gespeichert
  // UME_init (&s_dat, memspc501, sizeof(memspc501));
  //                      siz          sizInc
  il1 = (il1 / 2) + MIND_SIZ;
  il2 = il1 / 2;
  // UME_malloc_CB (&s_dat, _dat_INC*2, _dat_INC, STP_r_reall_CB);
  // UME_malloc_CB (&s_dat,  il1,        il2,     STP_r_reall_CB);
  UME_malloc (&s_dat, il1, il2);


  // get mem for tables 
  i1 = 16 + (il1 / 16000); // siz / inc-siz
    // printd(" memSizHint i1=%d\n",i1);

  // MemTab_ini (&mdlTab, sizeof(int), Typ_Int4, i1);
  MemTab_ini (&mdlTab, sizeof(STP_MDL), 0, i1);
  MemTab_ini (&refTab, sizeof(STP_I2), 0, i1);
  MemTab_ini (&geoTab, sizeof(STP_I2), 0, i1);


  // Ausgabezeile
  gTxt = memspc55;

  // // Tempspace
  // UME_init (&s_mSpc, memspc201, sizeof(memspc201));

  STP_r_addBox (0);   // init box fuer find Modelsize ...


  // Init (wird beim Einlesen schon korrigiert).
  angMod = 0;        // default = radians.

  // open File
  if((fp1=fopen(fnam,"r")) == NULL) {
    TX_Print("****** STP_r__ OPEN ERROR |%s| **********\n",fnam);
    irc = -1;
    goto L_exit;
  }


  //================================================================
  // alle daten in die tabelle s_tab und s_dat einlesen.

  //----------------------------------------------------------------
  // work FILE-section

  // skip alles bis "DATA;"
  irc = STP_r_read_start (fp1);
  if(irc < 0) {
    TX_Error("STP_r__ E0001");
    goto L_eof;
  }



  //----------------------------------------------------------------
  // work DATA-section
  // get next Line --> mem_cbuf1
  L_nxt_ln2:
  irc = STP_r_readLn (fp1);
  if(irc < 0) {
    TX_Error("STP_r__ E001");
    goto L_eof;
  }
    // printf("cbuf1=|%s|\n",mem_cbuf1);


  // jede Zeile muss beginnen mit "#<sInd>=<Hauptwort>"
  // decode sInd und Hauptwort from mem_cbuf1
  irc = STP_r_dec0 ();
  if(irc < 0) {
    if(irc == -5) goto L_eof;
    if(irc == -4) {               // irreparabler Fehler; stop.
      TX_Error("STP_r__ E002");
      fclose(fp1);
      goto L_exit;
    }
      printf (" STP_r_dec0-irc %d |%s|\n",irc,mem_cbuf1);
      // minor Error; skip this Line
  }

  goto L_nxt_ln2;




  //================================================================
  L_eof:
  fclose(fp1);

  ++s_Nr;   // make Anzahl aus Index


/*
  //---------- Testausgabe ..
  for(i1=0; i1<s_Nr; ++i1) {
    printf(" %d #%d = %s\n",i1,s_tab[i1].sInd,
           STP_r_TypTxt_sTyp(s_tab[i1].sTyp));
  }
  UME_dump (&s_dat, "sDat");
  // return 0;
  //---------- Testausgabe End
*/

  TX_Print(" importing %d STEP-records ...",s_Nr);


#ifdef DEB
    printd("----- s_tab - s_Nr=%d (sInd sTyp) ---------\n",s_Nr);
    for(i1=0; i1<s_Nr; ++i1) {
      printd(" %d #%d = %s\n",i1,s_tab[i1].sInd,
             STP_r_TypTxt_sTyp(s_tab[i1].sTyp));
      // UME_dump (&s_dat, "sDat");
    }
#endif

  // AP_debug__ ();


  STP_r_addBox (2);  // compute ModSiz from Box


  // find highest step-recordNumber
  i_Max = 0;
  // ii = s_Nr + 10000;
  // rec 0 unused und undefinierter Inhalt !!!!
  for(i1=1; i1<s_Nr; ++i1) {
    if(s_tab[i1].sInd > 1000000) {
      TX_Error("STP_r__ E003 %d %d\n",i1,s_tab[i1].sInd);
      goto L_exit;
    }
    if(s_tab[i1].sInd > i_Max) i_Max = s_tab[i1].sInd;
  }
  i_Max += 20;   // mind +1 !!!!
  // printf(" maxRec=%d\n",i_Max);


  // allocate indexTable 
  i_tab = NULL;
  i_tab = (int*) malloc (i_Max * sizeof(int));
    // printf(" i_tab=%p\n",i_tab);


  // load indexTable
  for(i1=0; i1<i_Max; ++i1) i_tab[i1] = i1;
  // rec 0 unused und undefinierter Inhalt !!!!
  for(i1=1; i1<s_Nr; ++i1) {
    // printf(" i1=%d srNr=%d\n",i1,s_tab[i1].sInd);
    i2 = s_tab[i1].sInd;
    if((i2 < 0)||(i2 >= i_Max))
      {printf(" STP_r__ i2=%d i1=%d\n",i2,i1);continue;}
    i_tab[i2] = i1;
  }



#ifdef DEB
  //---------- TESTDISP tables:
  STP_r_dump_mdlTab();
  STP_r_dump_geoTab();
  STP_r_dump_refTab();
#endif




  //================================================================
  // find all geometries for model mdlTab[i1]
  STP_r_mdl2geo ();
    STP_r_dump_geoTab(); //goto L_exit; // TEST


  // find all modelReferences for model mdlTab[i1]
  STP_r_mdl2ref ();
    STP_r_dump_refTab();


  // remove all models without geometry or references
  STP_r_ck_geo_used ();
    STP_r_dump_geoTab();


#ifdef DEB
// TEST ONLY:
  STP_r_dump_mdlTab();
  STP_r_dump_geoTab();
  STP_r_dump_refTab();
#endif



  //================================================================
  // subModels may not have identical names;
  // sometimes mainModel comes with identical name
  for(i1=0; i1 < mdlTab.rNr; ++i1) {
    p1 = mdlTab.data[i1].nam;
      // printf(" chMdlNames %d |%s|\n",i1,p1);
    for(i2=0; i2 < mdlTab.rNr; ++i2) {
      if(i2 == i1) continue;
      if(!strcmp(p1,mdlTab.data[i2].nam)) {
        TX_Print("**** modelname %s doubly defined; modified ..\n",p1);
        mdlTab.data[i2].nam[0] = '_';
      }
    }
  }


  // work models (write file for each subModel)
  for(i1=0; i1 < mdlTab.rNr; ++i1) {
    l2 = mdlTab.data[i1].iPROD;
    if(l2 < 0) continue;  // skip empty models (see STP_r_ck_geo_used)

    i2 = i_tab[l2];
    STP_r_mdl_export (i2, mdlTab.data[i1].nam);

    // mark PRODUCT successful exported ..
    if(irc >= 0) s_tab[i2].stat = 2;

    // reset all s_tab[].gInd
    STP_r_mdl_reset();
  }


/*
  // work models (PRODUCT_DEFINITION_SHAPE's)
  mdlNr = 0;
  for(i1=1; i1<s_Nr; ++i1) {

    if(s_tab[i1].sTyp != SC_PRODUCT) continue;

      printd("\n============= PRODUCT %d #%d =======\n", i1,s_tab[i1].sInd);

    // get geometry into struct mdl
    irc = STP_r_mdl_geo (i1);    // 1=no geom; check only dittos
    if(irc < 0) continue;

    // work this model; data is in struct mdl
    irc = STP_r_mdl__ ();

    // mark PRODUCT successful exported ..
    if(irc >= 0) s_tab[i1].stat = 2;

    // reset all s_tab[].gInd
    STP_r_mdl_reset();
  }
*/

/*
  //================================================================
  // work ..

  // Init Import ...
  // Init Objektindexe fuer Transformationsfunktion AP_obj_2_txt
  AP_obj_2_txt (NULL, 0L, NULL, 0L); // reset Startindizes
      
      
  UTF_clear1 ();  // den Buffer zuruecksetzen.
    
  sprintf(gTxt,"### STEP-Import %s",fnam);
  UTF_add1_line (gTxt);
  

  // init (clear) stat
  for(i1=0; i1<s_Nr; ++i1) s_tab[i1].stat = 0;

  // defaultfarbe raus (sollte entstr. Zugehoerigkeit zu BREP gemacht werden)
  if(indCol != 0) STP_r_creCol (indCol);


  // den Index des zugehoerigen PRODUCT in alle SHAPE_REPRESENTATION eintragen.
  STP_r_findPROD ();
    // printf(" nach STP_r_findPROD\n");
    // return 0;  // TEST


  // check if REPRESENTATION_RELATIONSHIP is wrong (nur CATIA-V4)
  STP_r_wrong_SRR ();
    // printf(" nach STP_r_wrong_SRR\n");
    // return 0;  // TEST


  // check if SHAPE_REPRESENTATION_RELATIONSHIP is wrong; korrigieren.
  STP_r_ckSRRSwrong ();
    // printf(" nach STP_r_ckSRRSwrong\n");


  //----------------------------------------------------------------
  // -) Activate next Model ( = next PRODUCT)
  //   -) Activate all Dittos referring to the active Model
  //   -) Activate all Workspaces referring to the active Model
  //   -) work all active obj's
  // -) Activate next Model

/

#140=SHAPE_DEFINITION_REPRESENTATION(#139,#138);
 #138=SHAPE_REPRESENTATION('SR2',(#126),#134);
  #4200=ADVANCED_BREP_SHAPE_REPRESENTATION('ABSR1',(#4199),#134);
   #4199=MANIFOLD_SOLID_BREP('25F',#4198);
    #4198=CLOSED_SHELL(
/

  // Ditto: 

  // resMod = 0;
  iNr = 0;   // modelNr !
  for(i1=1; i1<s_Nr; ++i1) {
    if(s_tab[i1].sTyp != SC_PRODUCT) continue;
      // printf(">>>>>>>>>> nxt Model %d #%d = PRODUCT\n",i1,s_tab[i1].sInd);


    // set modelname
    vp = STP_r_getInt (&i2, s_tab[i1].sDat);  // get Nr of words of modelname
    ModNam = vp;

      printd("\n\nImport Submodel '%s' from #%d=PRODUCT\n",
             ModNam,s_tab[i1].sInd);


    // Im SHAPE_DEFINITION_REPRESENTATION-Record wurde das 3. int-Wort
    // auf den Index des PRODUCT-Record gesetzt (von STP_r_findPROD).
    // Find this SHAPE_DEFINITION_REPRESENTATION-Record.
    oNr = 0;
    for(i2=0; i2<s_Nr; ++i2) {
      if(s_tab[i2].sTyp != SC_SHAPE_DEFINITION_REPRES) continue;
      ia = s_tab[i2].sDat;
      ii = ia[2];                                // get Link -> PRODUCT
      // printf(" %d -> PRODUCT = %d\n",i2,ii);
      if(ii == i1) goto L_a2;
    }
    TX_Print("STP_r__ E010");
    continue;


    L_a2:

    // der 2.Link der SHAPE_DEFINITION_REPRESENTATION ist die
    //   SHAPE_REPRESENTATION;/ merken (for the Dittos)
    // Dittos dzt via STP_r_creDit1
    lsdr = ii = ia[1];
    isdr = STP_r_findInd (lsdr, i2);

    
      printd(" %d #%d = SHAPE_DEFINITION_RS\n",i2,s_tab[i2].sInd);
      printd(" isdr %d lsdr = #%d\n",isdr,lsdr);
       // ,s_tab[isdr].sInd, STP_r_TypTxt_sTyp(s_tab[isdr].sTyp));


    // Wenn SHAPE_DEFINITION_REPRESENTATION/L2 auf SHAPE_REPRESENTATION zeigt:
    //   zugehoerige SHAPE_REPRESENTATION_RELATIONSHIP suchen
    //   oder zugehoerige PROPERTY_DEFINITION_REPRESENTATION suchen
    //     (PROPERTY_DEFINITION('external definition'.., DOCUMENT_FILE(..

    // if (isdr == SHAPE_REPRESENTATION):
    //   check if its L1 is a objGroup (ADVANCED_BREP_SHAPE_REPRESENTATION ..)
    //     yes: activate;
    //   else find (1-n) SHAPE_REPRESENTATION_RELATIONSHIP's
    //   its 1.Link must be == lsdr;  activate its second Link.
    if(s_tab[isdr].sTyp == SC_SHAPE_REPRESENTATION) {

      // find SHAPE_REPRESENTATION_RELATIONSHIP of record isdr
      ii=0;
      for(i6=0; i6<s_Nr; ++i6) {
        if(s_tab[i6].sTyp != SC_SHAPE_REPRES_RS) continue;
        vp = STP_r_getInt (&ii, s_tab[i6].sDat); // get Link -> PRODUCT
        // printf(" %d #%d=SHAPE_REPRESENTATION_RS(%d\n",i6,s_tab[i6].sInd,ii);
        if(ii == lsdr) {
          // we have the SHAPE_REPRESENTATION_RS; get & activate its 2.Link.
          vp = STP_r_getInt (&lobj, vp);
          iobj = STP_r_findInd (lobj, i6);
          // printf(" obj %d #%d=%s\n",iobj,lobj,
                 // STP_r_TypTxt_sTyp(s_tab[iobj].sTyp));
          // s_tab[iobj].stat = 1;   // activate
          STP_r_cre1 (iobj);  // activate
          ++ii;
        }
      }
      // Es kann aber auch eine externe Referenz sein !
      // Externe Referenz in C5:
      // #786=PROPERTY_DEFINITION_REPRESENTATION(#785,#770) ;
       // #785=PROPERTY_DEFINITION('external definition','',#777) ;
        // #777=DOCUMENT_FILE('Kerzentop.stp','','',#778,'',$) ;
        // see also:APPLIED_EXTERNAL_IDENTIFICATION_ASSIGNEMENT,EXTERNAL_SOURCE
      // aber auch:
      // #599=SHAPE_DEFINITION_REPRESENTATION(#598,#597);
       // #598=PROPERTY_DEFINITION('','Shape for Validation Properties',#584);
        // #584=SHAPE_ASPECT('#584','solid #570',#38,.F.);
         // #570=MANIFOLD_SOLID_BREP('#570',#569);
      // printf(" SDR ii=%d\n",ii);
      if(ii < 1) {
        // TX_Print("STP_r__ E014");
        // continue;
      }


    } else {
      // check if isdr is GeometryHeader(BREP,SURFACE or WIREFRAME)
      // activate rec. isdr if it is a objGroup.
      irc = STP_r_cre1 (isdr);
      // if(irc < 0) {
        // TX_Print("STP_r__ E015 %d",s_tab[isdr].sTyp);
      // }

/
    } else if((s_tab[isdr].sTyp == SC_GBOUNDED_WIREFRAME_SR) ||
              (s_tab[isdr].sTyp == SC_ADVANCED_BREP_SR)      ||
              (s_tab[isdr].sTyp == SC_MANIFOLD_SURFACE_SR))           {
      printf(" obj %d #%d=%s\n",isdr,lsdr,
             STP_r_TypTxt_sTyp(s_tab[isdr].sTyp));
      s_tab[isdr].stat = 1;   // activate


    } else {
      TX_Print("STP_r__ E015 %d",s_tab[isdr].sTyp);
      // continue;
/
    }




    // activate Dittos:
    // find (REPRESENTATION_RELATIONSHIP(
    // if (1.Link != SHAPE_REPRESENTATION(activeModel)) skip this
    // 2. Link zeigt auf #SHAPE_REPRESENTATION(Detail) to ref.
    ii=0;
    for(i6=0; i6<s_Nr; ++i6) {
      if(s_tab[i6].sTyp != SC_REPRESENTATION_RELATIONSHIP) continue;
      if(s_tab[i6].stat > 0) continue;
      ia = s_tab[i6].sDat;
      // get Link -> MasterSpace
      if(wrong_SRR == 0) lobj = ia[1];   // normal
      else            lobj = ia[0];   // verkehrt (CATIA-V4)
      // printf(" %d #%d = (REPRESENTATION_RS( %d\n",i6,s_tab[i6].sInd,lobj);
      if(lobj == lsdr) {
        // activate this ditto.
        // printf(" ditto %d #%d=%s\n",i6,s_tab[i6].sInd,
               // STP_r_TypTxt_sTyp(s_tab[i6].sTyp));
        s_tab[i6].stat = 1;   // activate
        ++ii;
      }
      // printf(" Dit ii=%d\n",ii);
    }


/
    // Link # 1 ist die ModelNr.
    // STP_r_creMod__ (*((int*)s_tab[i1].sDat));
    i2 = STP_r_creMod__ (s_tab[i1].sInd);  // activate obj's of Model in i1
    s_tab[i1].stat = 2;
    // i2 ist Anzahl Obj's in shape i1;
    if(i2 < 1) continue;   // leere shape ..
/

    // create all activated objects
    irc = STP_r_creMod_Run (iNr, ModNam);
    if(irc < 0) {
      if(irc == -4) goto L_exit;
    }

    L_nxt_mod:
    ++iNr;
    L_nxt_err:;
  }
*/


  //----------------------------------------------------------------
  // all PRODUDCT's are exported into files ..
  L_finish:
    printd("\n============= finish %d =============\n",mdlNr);
    // UI_men__ ("dump_ga");

// TEST ONLY
// exit(0);
// goto L_exit;


  if(mdlNr > 1) {

    // get modelname of the mainModel > mdl.nam
    // test if all PRODUCTs are used by NEXT_ASSEMBLY_USAGE_OCCURRENCE;
    // if not: create a mainModel usind this unused models.
    // irc = STP_r_mdl_sm ();  // returns nr of unused models
    STP_r_mdl_main1 ();
    // STP_r_mdl_main0 ();
      // printd(" main = |%s|\n",mdl.nam);
    // if(irc <= 0) {
// goto L_exit;


    // rename mainModel -> Model_
    if(mdl.nam) {
      sprintf(s1,"%sModel_%s",OS_get_tmp_dir(),mdl.nam);
      sprintf(s2,"%sModel_",OS_get_tmp_dir());
        // printd(" rename %s -> %s\n",mdl.nam,s2);
      OS_file_rename (s1, s2);
    }

    // join all files tmp/Model_* into file tmp/Model
    sprintf(s1,"%sMod.dat",OS_get_tmp_dir());
    Mod_sav_i (1);
// goto L_exit;
// exit(0);


  } else {
    // one model only;

    // find modelname of topmost (main-) model -> mdl.nam
    // STP_r_mdl_main1 ();
    STP_r_mdl_main0 ();


    // rename model -> importFileNme
    sprintf(s1,"%sModel_%s",OS_get_tmp_dir(),mdl.nam);
    sprintf(s2,"%sModel",OS_get_tmp_dir());
      // printd(" rename %s -> %s\n",mdl.nam,s2);
    OS_file_rename (s1, s2);


  }


  //----------------------------------------------------------------
  // load model <tmp>/Model into memory (removes all tmp/Model_*)
  // the following = from AP_Mod_load__
    // printf(" AP_mod_fnam=|%s|\n",AP_mod_fnam);
  sprintf(s1,"%sModel",OS_get_tmp_dir());
  Mod_load__ (0, s1, 1);
  Mod_mkList (0);       // create ../tmp/Mod.lst
  Brw_Mdl_init ();       // fill browserWin
  Mod_chg_tit ();        // set AP_mod_fnam as new title on top
  ED_Reset ();           // ED_lnr_act resetten
  ED_work_END (0);





  // activate new modSiz
  NC_setModSiz ((double)modSiz);


  irc = 0;


  L_exit:
  // printf(" i_tab=%p\n",i_tab);
  MemTab_free (&mdlTab);
  MemTab_free (&refTab);
  MemTab_free (&geoTab);

  free(i_tab);
  free(s_tab);
  UME_free (&s_dat);

  // stop debugging (following prints -> console)
  AP_deb_stat (0);
 
  // printf("ex STP_r__ |%s|\n",fnam);
  return irc;





/*
  //----------------------------------------------------------------
  // activate all
  //   GEOMETRICALLY_BOUNDED_WIREFRAME_SHAPE_REPRESENTATION and
  //   GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION (open parts)
  resMod = 0;
  printf("STP_r__ 000000000000000000000000000000\n");
  for(i1=0; i1<s_Nr; ++i1) {
    if((s_tab[i1].sTyp == SC_GBOUNDED_SURFACE_SR) ||
       (s_tab[i1].sTyp == SC_GBOUNDED_WIREFRAME_SR))   {
      s_tab[i1].stat = 1;
    } else {
      s_tab[i1].stat = 0;
    }
  }

  // resolv and create all active step-objects
  irc = STP_r_cre1 ();
  if(irc < 0) {
    if(irc == -4) return irc;              // irreparabler Fehler; stop.
  }

  //----------------------------------------------------------------
  // activate all OPEN_SHELLs
  resMod = 1;
  printf("STP_r__ 111111111111111111111111111111\n");
  for(i1=0; i1<s_Nr; ++i1) {
    if(s_tab[i1].sTyp == SC_OPEN_SHELL) {
      s_tab[i1].stat = 1;
    } else {
      s_tab[i1].stat = 0;
    }
  }

  // resolv and create all active step-objects
  irc = STP_r_cre1 ();
  if(irc < 0) {
    if(irc == -4) return irc;              // irreparabler Fehler; stop.
  }


  //----------------------------------------------------------------
  // then activate all closed parts, one after another.
  printf("STP_r__ 222222222222222222222222222222\n");
  resMod = 2;
  L_nxt_part:
  for(i1=0; i1<s_Nr; ++i1) {
    if(s_tab[i1].sTyp == SC_ADVANCED_BREP_SR) {
      if(s_tab[i1].stat > 1) continue;
      s_tab[i1].stat = 1;
      irc = STP_r_cre1 (); // resolv and create
      if(irc < 0) {
        if(irc == -4) return irc;              // irreparabler Fehler; stop.
      }
      s_tab[i1].stat = 2;
      goto L_nxt_part;
    }
  }



  //----------------------------------------------------------------
  L_write:
  sprintf(mem_cbuf1,"### End STEP-Import");
  UTF_add1_line (mem_cbuf1);


  // UTF_FilBuf1 an die Hauptdatei (UTF_FilBuf0, nur memory!) anfuegen
  UTF_insert1 (-1);


  return 0;
*/
}


//================================================================
  int STP_r_mdl_valid (int lm) {
//================================================================
// test if PRODUCT is valid; RectCod 0=OK, else not


  int     i1;


  for(i1=0; i1 < mdlTab.rNr; ++i1) {
    if(mdlTab.data[i1].iPROD == lm) return 0;
  }


  return -1;

}


//================================================================
  int STP_r_ck_geo_used () {
//================================================================
// remove all models without geometry
// remove all models without geometry
// loop tru models;
//   test if geometry exists for this model;
//     no: set model -> invalid.


  int     i1, i2, im1;

  printd("STP_r_ck_geo_used\n");


  // loop tru models;
  for(i1=0; i1 < mdlTab.rNr; ++i1) {

    im1 = i_tab[mdlTab.data[i1].iPROD];

    // loop tru geos
    for(i2=0; i2 < geoTab.rNr; ++i2) {
      if(im1 == geoTab.data[i2].iMdl) goto L_mdlOK;
    }

    // model i1/im1 does not have geometry;
    // test if it has references
    for(i2=0; i2 < refTab.rNr; ++i2) {
      if(im1 == refTab.data[i2].iMdl) goto L_mdlOK;
    }

      printd(" STP_r_ck_geo_used: deactivte mdl %d %d #%d\n",
             i1,im1,mdlTab.data[i1].iPROD);
    mdlTab.data[i1].iPROD = -1;

    L_mdlOK:
      continue;
  }


  return 0;

}


//================================================================
  int STP_r_ck_mdl_used (int im) {
//================================================================
// test if model has a geometry
// RetCod: 0=OK, model has geometry; -1=not.
 
  int     i1;


  printd("STP_r_ck_mdl_used %d #%d\n",im,s_tab[im].sInd);
  // printd("STP_r_ck_mdl_used %d #%d\n",i_tab[lm], lm);
  // STP_r_dump_geoTab();


  for(i1=0; i1 < geoTab.rNr; ++i1) {
    if(geoTab.data[i1].iMdl == im) return 0;
  }


  return -1;


}


//================================================================
  int STP_r_dump_mdlTab () {
//================================================================
// mdlTab is a list of step-links !
  
  long    i1, i2, l2;
  
  printd("----- STP_r_dump_mdlTab %d (.iPROD Link_of_iPROD .nam) ---------\n",
         geoTab.rNr);
  
  for(i1=0; i1 < mdlTab.rNr; ++i1) {
    // l2 = mdlTab.data[i1].iPROD;
    // if(l2 >= 0) i2 = i_tab[l2];
    // else        i2 = l2;
    // printd(" %d %d #%d\n",i1,i2,l2);
    printd(" %-8ld #%-8d %-8d |%s|\n",i1,
           mdlTab.data[i1].iPROD,
           i_tab[mdlTab.data[i1].iPROD],
           mdlTab.data[i1].nam);

  }
  
  return 0;
  
}   
    
//================================================================
  int STP_r_dump_geoTab () {
//================================================================
  
  int     i1, i2, l2;
  
  printd("----- STP_r_dump_geoTab %d (.ii Link_of_ii .iMdl) ---------\n",
         geoTab.rNr);
  
  for(i1=0; i1 < geoTab.rNr; ++i1) {
    // i2 = geoTab.data[i1].iMdl;
    // if(i2 >= 0) l2 = s_tab[i2].sInd;
    // printd(" %d ii=#%d Mdl=%d #%d\n",i1,geoTab.data[i1].ii,i2,l2);
    printd(" %-8d #%-8d %-8d %-8d\n",i1,
           geoTab.data[i1].ii,
           i_tab[geoTab.data[i1].ii],
           geoTab.data[i1].iMdl);
  }
  
  return 0;
  
}   
    

//================================================================
  int STP_r_dump_refTab () {
//================================================================

  int     i1, i2, l2;

  printd("----- STP_r_dump_refTab %d (.ii Link_of_ii .iMdl) ---------\n",
         geoTab.rNr);

  for(i1=0; i1 < refTab.rNr; ++i1) { 
    // i2 = refTab.data[i1].iMdl;
    // if(i2 >= 0) l2 = s_tab[i2].sInd;
    // printd(" %d ii=#%d Mdl=%d #%d\n",i1,refTab.data[i1].ii,i2,l2);
    printd(" %-8d #%-8d %-8d %-8d\n",i1,
           refTab.data[i1].ii,
           i_tab[refTab.data[i1].ii],
           refTab.data[i1].iMdl);
  }

  return 0;

}


//================================================================
  int STP_r_mdl_sm () {
//================================================================
// test if all PRODUCTs are used by NEXT_ASSEMBLY_USAGE_OCCURRENCE;
// if not: create a mainModel using this unused models.

// loop tru all PRODUCTs;
//   loop tru all NEXT_ASSEMBLYs; check if active PRODUCT is used.
//     yes: continue; else add PRODUCT to list of unused PRODUCTs.

// see STP_r_mdl_main2

  int   i1, i2, *iaProd, iaNr;


  printd("STP_r_mdl_sm\n");

  // get tempspace for list of unused PRODUCTs.
  iaProd = (int*) MEM_alloc_tmp ((int)(mdlNr * sizeof(int)));
  if(!iaProd) {TX_Error("STP_r_mdl_sm EOM"); return -1;}



  //----------------------------------------------------------------
  // set bitValue 4 for all used models
  iaNr = 0;
  for(i1=1; i1<s_Nr; ++i1) {

    // get next NEXT_ASSEMBLY_USAGE_OCCURR
    if(s_tab[i1].sTyp != SC_NEXT_ASSEMBLY_USAGE_OCCURR) continue;
      printd(" assy %d/#%d stat=%d\n",
             i1,s_tab[i1].sInd,s_tab[i1].stat);
    if(s_tab[i1].stat != 2) continue;  // skip if used

    // get PRODUCT of NEXT_ASSEMBLY_USAGE_OCCURR
    i2 = STP_r_mdl_res__ (i1);
      printd(" prod %d/#%d stat=%d used by %d/#%d stat=%d\n",
             i2,s_tab[i2].sInd,s_tab[i2].stat,
             i1,s_tab[i1].sInd,s_tab[i1].stat);

    // set bitValue 4 for .stat of model i2
    s_tab[i2].stat |= 4;
      printd(" set product %d #%d used\n",i2,s_tab[i2].sInd);

  }


  //----------------------------------------------------------------
  // loop tru all models; if not used (bitValue 4) add to list iaProd

  for(i1=1; i1<s_Nr; ++i1) {

    if(s_tab[i1].sTyp != SC_PRODUCT) continue;

    i2 = s_tab[i1].stat &= 6; // filter bitvalues 2 and 4
      printd(" prod %d/#%d stat=%d val=%d\n",
             i1,s_tab[i1].sInd,s_tab[i1].stat,i2);


    // val 6=exported, used; 2=exported, but not used.
    // this also finds the main-model !
    if(i2 != 2) continue;

    // store in tab
    iaProd[iaNr] = i1;
      printd(" _r_mdl_sm %d %d #%d\n",iaNr,i1,s_tab[i1].sInd);
    ++iaNr;

  }


  //----------------------------------------------------------------
  // if there is only one model: use this as the mainmodel.
  if(iaNr == 1) {
    // get name of model iaProd[0]
    strcpy(mdl.nam, STP_r_mdl_nam__(iaProd[0]));
    iaNr = 0;
    goto L_exit;
  }


  // else: create a new mainmodel with all subModels in list iaProd
  STP_r_mdl_main2 (iaProd, iaNr);


  L_exit:
    printd(" mdl.nam=|%s| iaNr=%d\n",mdl.nam,iaNr);

  return iaNr;

}


//================================================================
  int STP_r_mdl_reset () {
//================================================================
// reset all s_tab[].gInd

  int     i1;


  for(i1=0; i1<s_Nr; ++i1) {
    s_tab[i1].gInd = -1;
  }


  return 0;

}


//================================================================
  int STP_r_mdl_geo (int ii) {
//================================================================
// get geometry into struct mdl
// this function returns with error (-1) or when all necessary data are found
// and stored in mdl.

// Output:
//   mdl.iGeo   eg index of ADVANCED_BREP_SHAPE_REPRESENTATION

// Examples:

// mainpart:
// #5=PRODUCT('Produkt1','','',(#2)) ;
// #6=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE('',' ',#5,
// #17=PRODUCT_DEFINITION('Produkt1',' ',#6,#3) ;
// #45=PRODUCT_DEFINITION_SHAPE(' ',' ',#17) ;
// #43=SHAPE_DEFINITION_REPRESENTATION(#45,#44) ; // find #44 in ShaRepRel-L1
// 51=SHAPE_REPRESENTATION_RELATIONSHIP()         // empty: only dittos ..
// 44=SHAPE_REPRESENTATION(' ',(#609),#51) //609=axis,51=

// subModel:
// #54=PRODUCT('Part1','','',(#2)) ;
// #55=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE('',' ',#54,
// #56=PRODUCT_DEFINITION_WITH_ASSOCIATED_DOCUMENTS('Part1',' ',#55,#3,(#58)) ;
// #59=PRODUCT_DEFINITION_SHAPE(' ',' ',#56) ;
// 53=SHAPE_DEFINITION_REPRESENTATION(#59,#52) ;
// #603=SHAPE_REPRESENTATION_RELATIONSHIP(' ',' ',#52,#602) ;
// #602=ADVANCED_BREP_SHAPE_REPRESENTATION(
// #52=SHAPE_REPRESENTATION(' ',(#610),#618)

// #1583=PRODUCT('PLATE','PLATE','NOT SPECIFIED',(#1576));
// #1590=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE(,,#1583,.MADE.);
// #1845=PRODUCT_DEFINITION('design','',#1590,#1569);
// #1852=PRODUCT_DEFINITION_SHAPE('','SHAPE FOR PLATE.',#1845);
// #1868=SHAPE_DEFINITION_REPRESENTATION(#1852,#1840);
// #1840=ADVANCED_BREP_SHAPE_REPRESENTATION('',(#867,#1473),#1548);

// #53=PRODUCT('shackle','','None',(#52));
// #55=PRODUCT_DEFINITION_FORMATION('','None',#53);
// #57=PRODUCT_DEFINITION('None','None',#55,#56);
// #58=PRODUCT_DEFINITION_SHAPE('None','None',#57);
// #94=SHAPE_DEFINITION_REPRESENTATION(#58,#93);
// #4917=SHAPE_REPRESENTATION_RELATIONSHIP('','',#4916,#93);  // WRONG !
// #4916=ADVANCED_BREP_SHAPE_REPRESENTATION(

// geometric types:
// ADVANCED_BREP_SHAPE_REPRESENTATION > MANIFOLD_SOLID_BREP > CLOSED_SHELL
// MANIFOLD_SURFACE_SHAPE_REPRESENTATION > SHELL_BASED_SURFACE_MODEL > OPEN_SHELL
// GEOMETRICALLY_BOUNDED_WIREFRAME_SHAPE_REPRESENTATION > GEOMETRIC_CURVE_SET
// GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION > GEOMETRIC_SET
// EDGE_BASED_WIREFRAME_SHAPE_REPRESENTATION > EDGE_BASED_WIREFRAME_MODEL ..
//
// not yet implemented:
// FACETED_BREP_SHAPE_REPRESENTATION > FACETED_BREP > CLOSED_SHELL
// BREP_WITH_VOIDS > CLOSED_SHELL,(ORIENTED_CLOSED_SHELL ..
//
// SURFACE_CURVE (see step/ErrorModels/teampart.stp)


  int   li, i1, l1, i2, l2, *ia;



  if(ii < 0) return (ii);

  li = s_tab[ii].sInd;

  printd(" STP_r_mdl_geo %d #%d\n",ii,li);


  switch (s_tab[ii].sTyp) {


    //----------------------------------------------------------------
    case SC_PRODUCT:
      // primary input: index of s_tab-record "PRODUCT"
      mdl.prod = li;
      mdl.iGeo = -1;
      mdl.iProDefSha = -1;
      // mdl.mode = 1;

      // save modelname
      ia = s_tab[ii].sDat;
      mdl.nam = (char*)&ia[1];

      // find PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE 
      //   where L1=PRODUCT
      ii = STP_r_find_sRec_TypL1 (SC_PRODUCT_DEFINITION_FORMWSS, li);
      if(ii < 0) return -1;

      return STP_r_mdl_geo (ii);


    //----------------------------------------------------------------
    // PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE
    case SC_PRODUCT_DEFINITION_FORMWSS:
    
      // find PRODUCT_DEFINITION or
      //      PRODUCT_DEFINITION_WITH_ASSOCIATED_DOCUMENTS
      //   where L1=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE
      for(i1=0; i1<s_Nr; ++i1) {
        if(s_tab[i1].sTyp == SC_PRODUCT_DEFINITION) {
          if(STP_r_get_L1(i1) == li) goto L1_1;

        } else if(s_tab[i1].sTyp == SC_PRODUCT_DEFINITION_WITH_AD) {
          if(STP_r_get_L1(i1) == li) goto L1_2;
        }
      }
      goto L_err1;
      L1_1:    // PRODUCT_DEFINITION
      L1_2:    // PRODUCT_DEFINITION_WITH_ASSOCIATED_DOCUMENTS
      return STP_r_mdl_geo (i1);


    //----------------------------------------------------------------
    // PRODUCT_DEFINITION
    case SC_PRODUCT_DEFINITION:

      // find PRODUCT_DEFINITION_SHAPE
      //   where L1=PRODUCT_DEFINITION
      ii = STP_r_find_sRec_TypL1 (SC_PRODUCT_DEFINITION_SHAPE, li);
      if(ii < 0) return -1;

      return STP_r_mdl_geo (ii);


    //----------------------------------------------------------------
    // PRODUCT_DEFINITION_WITH_ASSOCIATED_DOCUMENTS
    case SC_PRODUCT_DEFINITION_WITH_AD:

      // find PRODUCT_DEFINITION_SHAPE
      //   where L1=PRODUCT_DEFINITION_WITH_ASSOCIATED_DOCUMENTS
      // ii = STP_r_find_sRec_TypL1 (SC_PRODUCT_DEFINITION_SHAPE, li);
      // if(ii < 0) return -1;

      // PROBLEM: can have more then 1 PRODUCT_DEFINITION_SHAPE;
      //   only one is useful ..
      for(i1=0; i1<s_Nr; ++i1) {
        if(s_tab[i1].sTyp != SC_PRODUCT_DEFINITION_SHAPE) continue; 
        ia = s_tab[i1].sDat;
        l1 = ia[0];
        if(l1 != li) continue;

        mdl.iProDefSha = i1;
          printd("  PDS %d #%d i1 = %d #%d\n",i_tab[l1],l1,i1,s_tab[i1].sInd);

        // found PRODUCT_DEFINITION_SHAPE; 
        // find SHAPE_DEFINITION_REPRESENTATION where
        //   L1 = PRODUCT_DEFINITION_SHAPE
        l1 = s_tab[i1].sInd;
        errTyp = 1;  // do not raise TX_Print
        ii = STP_r_find_sRec_TypL1 (SC_SHAPE_DEFINITION_REPRES, l1);
        if(ii < 0) continue;
          printd(" ii  %d #%d\n",ii,s_tab[ii].sInd);
        return STP_r_mdl_geo (ii);
      }
    
      return -1;
      // return STP_r_mdl_geo (ii);


    //----------------------------------------------------------------
    // PRODUCT_DEFINITION_SHAPE
    case SC_PRODUCT_DEFINITION_SHAPE:

      mdl.iProDefSha = ii;

      // find SHAPE_DEFINITION_REPRESENTATION
      //   where L1=PRODUCT_DEFINITION_SHAPE
      ii = STP_r_find_sRec_TypL1 (SC_SHAPE_DEFINITION_REPRES, li);
      if(ii < 0) return -1;

      return STP_r_mdl_geo (ii);


    //----------------------------------------------------------------
    // SHAPE_DEFINITION_REPRESENTATION
    case SC_SHAPE_DEFINITION_REPRES:

      ia = s_tab[ii].sDat;
      l2 = ia[1];            // l2=L2 of SHAPE_DEFINITION_REPRESENTATION
      i2 = i_tab[l2];
        printd(" SDR-L2: %d #%d\n",i2,l2);

      // L2 of SHAPE_DEFINITION_REPRESENTATION can be:
      //   - ADVANCED_BREP_SHAPE_REPRESENTATION
      //   - SHAPE_REPRESENTATION_RELATIONSHIP

      // if((s_tab[i2].sTyp == SC_ADVANCED_BREP_SR)     ||
         // (s_tab[i2].sTyp == SC_ADVANCED_BREP_SR)) {
      if(s_tab[i2].sTyp == SC_ADVANCED_BREP_SR)     {
        mdl.iGeo = i2;     // s_tab-index
        goto L_finish;
      }

      // find SHAPE_REPRESENTATION_RELATIONSHIP
      //   where L1=L2 of SHAPE_DEFINITION_REPRESENTATION=SHAPE_REPRESENTATION
      errTyp = 1;  // do not raise TX_Print
      ii = STP_r_find_sRec_TypL1 (SC_SHAPE_REPRES_RS, l2);
          printd(" SRR-1: %d #%d\n",ii,s_tab[ii].sInd);
/*
      if(ii < 0) {
        // SHAPE_REPRESENTATION_RELATIONSHIP CAN BE WRONG !
        // find SHAPE_REPRESENTATION_RELATIONSHIP
        //   where L2=L2 of SHAPE_DEFINITION_REPRESENTATION
        errTyp = 1;  // do not raise TX_Print
        ii = STP_r_find_sRec_TypL2 (SC_SHAPE_REPRES_RS, l2);
          printd(" SRR-2:%d\n",ii);
        if(ii < 0) {
          // SHAPE_REPRESENTATION_RELATIONSHIP empty: only dittos ?
          goto L_finish;
        }
        // L2 of SRR=SHAPE_REPRESENTATION; so L1 can be geometry.
        l2 = STP_r_get_L1 (ii);
        i2 = i_tab[l2];
        if(s_tab[i2].sTyp == SC_ADVANCED_BREP_SR)     {
          mdl.iGeo = i2;     // s_tab-index
          goto L_finish;
        }
      }
*/
      return STP_r_mdl_geo (ii);
      


    //----------------------------------------------------------------
    // SHAPE_REPRESENTATION_RELATIONSHIP
    case SC_SHAPE_REPRES_RS:

      //   save L2 of SHAPE_REPRESENTATION_RELATIONSHIP as iGeo
      ia = s_tab[ii].sDat;
      l2 = ia[1];         // get L2 of SHAPE_REPRESENTATION_RELATIONSHIP
      mdl.iGeo = i_tab[l2];   // s_tab-index

        printd(" geomTyp %d #%d %d\n",i_tab[l2],l2,s_tab[mdl.iGeo].sTyp);

      switch (s_tab[mdl.iGeo].sTyp) {
        case SC_CLOSED_SHELL:
        case SC_ADVANCED_BREP_SR:
        case SC_MANIFOLD_SURFACE_SR:  // < SHELL_BASED_SURFACE_MODEL < OPEN_SHELL
        case SC_GBOUNDED_SURFACE_SR:
          goto L_finish;

        default:
          TX_Print("***** STP_r_mdl_geo - NOT YET IMPLEMENTED #%d",l2);
          return -1;
      }



    //----------------------------------------------------------------
    default:
      TX_Error("STP_r_mdl_geo E001 #%d %d",s_tab[ii].sInd,s_tab[ii].sTyp);
// exit(0);
  }


  return 0;



  L_finish:
    STP_r_mdl_dump ();
    return 0;



  L_err1:
    TX_Print("STP_r_mdl_geo E002 %d %d",s_tab[ii].sInd);
    return -1;


}


//================================================================
  int STP_r_mdl2geo () {
//================================================================
// find model (PRODUCT) for geometries
// set geoTab[].iMdl to the s_tab-index of its PRODUCT.
// Loop tru geoTab; 
//   Input:    geoTab[].ii = link to ADVANCED_BREP_SR
//   Output:   set geoTab[].iMdl to the s_tab-index of the PRODUCT.
 
  int     i1, ii, ig;


  printd("STP_r_mdl2geo %d\n",geoTab.rNr);

  if(geoTab.rNr < 1)  return 0;

  for(ig=0; ig < geoTab.rNr; ++ig) {
    // find PRODUCT from geometry
    i1 = i_tab[geoTab.data[ig].ii];
      printd("--- test geo.%d = %d #%d\n",ig,i1,IND_TAB(i1));
    ii = STP_r_mdl_res__ (i1);
    geoTab.data[ig].iMdl = ii;
      printd(" geo-iMdl[%d] = %d #%d\n",ig,ii,IND_TAB(ii));
  }


  return 0;

}


//================================================================
  int STP_r_mdl2ref () {
//================================================================
// find model (PRODUCT) for all modelReferences

  int     i1, ii, ir;

  printd("STP_r_mdl2ref %d\n",refTab.rNr);

  if(refTab.rNr < 1)  return 0;


  for(ir=0; ir < refTab.rNr; ++ir) {
    // test if geometry geoTab[ig] belongs to model is
    // find PRODUCT from ref
    i1 = i_tab[refTab.data[ir].ii];
      printd("--- test ref.%d = %d #%d\n",ir,i1,s_tab[i1].sInd);
    ii = STP_r_mdl_res__ (i1);
    if(ii < 1) continue;
      printd(" ref-iMdl[%d] = %d #%d\n",ir,ii,s_tab[ii].sInd);
    refTab.data[ir].iMdl = ii;

  }





  return 0;

}
    

//================================================================
  int STP_r_mdl_export (int im, char *mdlNam) {
//================================================================
// create subModel; write file.
// Input:
//   im     s_tab-index of model (mdlTab = PRODUCT)


  int     ii, i1, l1, irc0, irc1, irc2;
  char    *p1;


  printd("==============================================\n");
  printd("STP_r_mdl_export %d #%d |%s|\n",im,s_tab[im].sInd,mdlNam);
  
  // mdl.nam = STP_r_mdl_nam__ (im);
  mdl.nam = mdlNam;  // else cannot hide !?


  //----------------------------------------------------------------
    // reset Startindizes
    AP_obj_2_txt (NULL, 0L, NULL, 0L);

    // den Buffer zuruecksetzen.
    UTF_clear1 ();

    // clear Hidelist
    GA_hide__ (-1, 0L, 0);


    sprintf(gTxt,"### STEP-Import");
    UTF_add1_line (gTxt);


  //----------------------------------------------------------------
  // export all geometries
  irc1 = 0;
  for(ii=0; ii < geoTab.rNr; ++ii) {
    if(geoTab.data[ii].iMdl != im) continue;
    l1 = geoTab.data[ii].ii;
      printd(" exp geo %d #%d\n",i_tab[l1],l1);
    if(l1 >= 0) { 
      irc0 = STP_r_cre2 (i_tab[l1]);
      if(irc0 < 0) { 
        if(irc0 == -4) {
          printf(" exit from STP_r_mdl_export err=%d\n",irc0);
          return irc0;              // irreparabler Fehler; stop.
        }
      } else {
        ++irc1;
      }
    }
  }



  // export all modelRefs
  irc2 = 0;
  for(ii=0; ii < refTab.rNr; ++ii) {
    if(refTab.data[ii].iMdl != im) continue;
    l1 = refTab.data[ii].ii;
    if(l1 < 0) continue;
    i1 = i_tab[l1];
      printd(" exp ref %d #%d\n",i1,l1);
    // // test if the model of the ref has a geometry
    // irc1 = STP_r_ck_mdl_used (i1);
    // if(irc1 < 0) {
      // TX_Print("STP_r_mdl_export I001");
      // continue;
    // }
    irc0 = STP_r_creDit3 (i1);
    if(irc0 < 0) {
      if(irc0 == -4) {
        printf(" exit from STP_r_mdl_export err=%d\n",irc0);
        return irc0;              // irreparabler Fehler; stop.
      }
    } else {
      ++irc2;
    }
  }


  //----------------------------------------------------------------
  sprintf(gTxt,"### End STEP-Import");
  UTF_add1_line (gTxt);

  L_end_sub:
    printd("  _export irc1=%d irc2=%d\n",irc1,irc2);

  if((irc1 >= 0) ||
     (irc2 > 0))      {
    ++mdlNr;
    irc0 = 0;

    // write subModel into file
    // Buffer + Hidelist in die Datei schreiben (UTF_FilBuf1; see UTF_dump1__)
      // printd(" savSubBuf1 |%s| %d\n",mdl.nam, modSiz);
    // Mod_savSubBuf1 (mdl.nam, modSiz);
    Mod_savSubBuf1 (mdlNam, modSiz);

  } else {
    irc0 = -1;
  }

  return irc0;

}


//================================================================
  int STP_r_mdl_dump () {
//================================================================

  int l1;

  if(mdl.iGeo >= 0) l1 = s_tab[mdl.iGeo].sInd;
  else              l1 = -1;
  // dump struct mdl:
  printd(" mdl.prod=#%d  PDF = %d\n",mdl.prod,mdl.iProDefSha);
  printd("     Geo=#%d\n",l1);
  printd("     nam |%s|\n",mdl.nam);


  return 0;

}



//================================================================
  int STP_r_mdl_main0 () {
//================================================================
// get modelname of the mainModel = first product -> mdl.nam

  int       i1, *ia;

  for(i1=1; i1<s_Nr; ++i1) {
    if(s_tab[i1].sTyp != SC_PRODUCT) continue;
    if(s_tab[i1].stat != 2) continue;

    // get modelname from PRODUCT
    ia = s_tab[i1].sDat;     // get data of s_tab-record i4
    mdl.nam = (char*)&ia[1];
    break;
  }

  printd("STP_r_mdl_main0 |%s|\n",mdl.nam);


  return 0;

}


//================================================================
  int STP_r_mdl_main1 () {
//================================================================
// get modelname of the mainModel

// how to find topmost model:
// get topModel = parent of first NEXT_ASSEMBLY_USAGE_OCCURRENCE (L1);
// Loop tru all NEXT_ASSEMBLY_USAGE_OCCURRENCE
//   if L2 (childModel) == topModel) restart Loop.
// if no change of topModel inside Loop: topModel found.
//


  // parent of first NEXT_ASSEMBLY_USAGE_OCCURRENCE (L1);

  int     i1, i2, im, lm, lc;


  printd("STP_r_mdl_main1 -------------- %d\n",refTab.rNr);

  mdl.nam = NULL;


  //----------------------------------------------------------------
  // find topmost model:
  // Loop tru all NEXT_ASSEMBLY_USAGE_OCCURRENCE
  lm = -1;
  for(i1=0; i1<refTab.rNr; ++i1) {

    // get next NEXT_ASSEMBLY_USAGE_OCCURR
    i2 = i_tab[refTab.data[i1].ii];

    // skip it if not worked
    if(s_tab[i2].stat < 2) continue;

    if(lm < 0) {
      // first modelRef; get parentLink
      lm = STP_r_get_L1 (i2);
        printd("  lm = #%d\n",lm);

    } else {
      // get childLink
      lc = STP_r_get_L2 (i2);
        printd("  lc = #%d\n",lc);
      if(lc == lm) {
        lm = STP_r_get_L1 (i2);
          printd("  %d #%d lm = #%d  lc = #%d\n",i1,s_tab[i1].sInd,lm,lc);
      }
    }
  }

  if(lm < 0) goto L_autoMain; // no topDitto-link found


  im = i_tab[lm];   // PRODUCT_DEFINITION from NEXT_ASSEMBLY_USAGE_OCCURRENCE
    printd(" mainModel = %d #%d\n",im,lm);


  // get modelname for this modelRef
  // mdl.nam = STP_r_mdl_nam2 (im);
  mdl.nam = STP_r_mdl_nam__ (im);

    printd(" ex STP_r_mdl_main1 |%s|\n",mdl.nam);

  return 0;



  //----------------------------------------------------------------
  L_autoMain:
    // test for unused models
    // for(i1=0; i1 < mdlTab.rNr; ++i1) {
      // i2 = i_tab[mdlTab.data[i1].iPROD];
        // printd(" %d #%d %d stat=%d\n",i1,i2,s_tab[i2].sInd,s_tab[i2].stat);
    // }


    // no NEXT_ASSEMBLY_USAGE_OCCURRENCE; make automatic main ..
    STP_r_mdl_main2 ();

    mdl.nam = NULL;
    return 1;


  //----------------------------------------------------------------
    // get first model ..
    i1 = i_tab[mdlTab.data[0].iPROD];
    mdl.nam = STP_r_mdl_nam__ (i1);


    return 2;

}


//================================================================
  int STP_r_mdl_main2 () {
//================================================================
// no NEXT_ASSEMBLY_USAGE_OCCURRENCE; make automatic main ..


  // static char *mNam="main";

  int       irc,  i1, i2, *ia;
  char      *p1;


  // mdl.nam = mNam;

  printd("STP_r_mdl_main2 ---------------- %d\n",mdlTab.rNr);

  // reset Startindizes
  AP_obj_2_txt (NULL, 0L, NULL, 0L);

  // den Buffer zuruecksetzen.
  UTF_clear1 ();

  // clear Hidelist
  GA_hide__ (-1, 0L, 0);

  sprintf(gTxt,"### Step-Model %s",mdl.nam);
  UTF_add1_line (gTxt);


  // add call for all processed PRODUCT's
  for(i1=0; i1<mdlTab.rNr; ++i1) {
    i2 = i_tab[mdlTab.data[i1].iPROD];

    // get modelname from PRODUCT
    p1 = STP_r_mdl_nam__ (i2);

    // create M20='id'
    sprintf(gTxt, "\"%s\"",p1);
    // make obj, save-> gCad, stor gC-Typ & Index -> s_tab
    irc = STP_r_creObj1 (-1, Typ_Model, Typ_Txt, gTxt);
    if(irc < 0) return irc;
  }


  sprintf(gTxt,"### End STEP-Import");
  UTF_add1_line (gTxt);

  // Buffer + Hidelist in die Datei schreiben,
  Mod_savSubBuf1 ("", modSiz);

  // den Buffer zuruecksetzen.
  UTF_clear1 ();

  return 0;

}
 


//================================================================
  char* STP_r_mdl_nam__ (int is) {
//================================================================
// get name of PRODUCT from s_tab-index

// see also STP_r_mdl_geo STP_r_cre2 STP_r_mdl_res__

  int  *ia, ii;

  if(is < 0) goto L_err1;

  printd("STP_r_mdl_nam__ %d #%d\n",is,s_tab[is].sInd);

  switch (s_tab[is].sTyp) {


    //----------------------------------------------------------------
    case SC_PRODUCT:
      // found
      // get modelname from PRODUCT
      ia = s_tab[is].sDat;     // get data of s_tab-record i4
        printd(" ex STP_r_mdl_nam__ |%s|\n",(char*) &ia[1]);
      return (char*) &ia[1];


    //----------------------------------------------------------------
    default:
      ii = STP_r_mdl_res__ (is);
      return STP_r_mdl_nam__ (ii);

  }


  return (NULL);


  L_err1:
    TX_Print("****** STP_r_mdl_nam__ E001 %d #%d",is,s_tab[is].sInd);
    return NULL;

}


/*
//================================================================
  char* STP_r_mdl_nam1 (int iProDefSha) {
//================================================================
// get PRODUCT_DEFINITION from PRODUCT_DEFINITION_SHAPE
// L1 of PRODUCT_DEFINITION_SHAPE = ?
// find 

  int  *ia, i1, l1, i2, l2;

  printd("STP_r_mdl_nam1 %d #%d\n",iProDefSha,s_tab[iProDefSha].sInd);


  ia = s_tab[iProDefSha].sDat;     // get data of s_tab-record iProDefSha
  l1 = ia[0];
  i1 = i_tab[l1];
    printd(" i=%d #%d typ=%d\n",i1,l1,s_tab[i1].sTyp);

  if(s_tab[i1].sTyp == SC_PRODUCT_DEFINITION) {
    // normal detail
    return STP_r_mdl_nam2 (i1);
  }

  if(s_tab[i1].sTyp == SC_NEXT_ASSEMBLY_USAGE_OCCURR) {
    // ditto
    // get PRODUCT_DEFINITION for ditto
    // use the child-link (L2) of NEXT_ASSEMBLY_USAGE_OCCURR
    ia = s_tab[i1].sDat;
    l2 = ia[1];
    i2 = i_tab[l2];
    return STP_r_mdl_nam2 (i2);
  }


  // unknown ..
    printf("***** ERROR STP_r_mdl_nam1 E001 \n");
  return NULL;

}


//================================================================
  char* STP_r_mdl_nam2 (int iProDef) {
//================================================================
// find subModelname from PRODUCT_DEFINITION

  int  *ia, i3, i4;

 
  // find the modelname 
  // PRODUCT_DEFINITION_FORMATION* from PRODUCT_DEFINITION
  ia = s_tab[iProDef].sDat;     // get data of s_tab-record i2
  i3 = i_tab[ia[0]];       // index into s_tab of step-record ia[0]
    // printd(" link1=#%d s_tab-index=%d typ=%d\n",ia[0],i3,s_tab[i3].sTyp);

  // PRODUCT from PRODUCT_DEFINITION_FORMATION*
  ia = s_tab[i3].sDat;     // get data of s_tab-record i3
  i4 = i_tab[ia[0]];       // index into s_tab of step-record ia[0]
    // printd(" link1=#%d s_tab-index=%d typ=%d\n",ia[0],i4,s_tab[i4].sTyp);

  // i4 must be a PRODUCT
  if(s_tab[i4].sTyp != SC_PRODUCT) {
    printd("***** STP_r_mdl_nam2 ERROR: should be SC_PRODUCT !\n");
    return NULL;
  }

  // get modelname from PRODUCT
  ia = s_tab[i4].sDat;     // get data of s_tab-record i4


  return (char*) &ia[1];

}


//================================================================
  int STP_r_mdl__ () {
//================================================================
// replaces STP_r_creMod_Run 
// Input: data is in struct mdl
// RetCod: 0=OK; else no model created.

// ModNr: 0=MainModel (hintanfuegen), ab 1 Submodel (write to File)
// Submodels: schreibt mit UTF_add1_line --> UTF_FilBuf1,
//   es wird aber die originale HIDE-Tabelle gesetzt;
//   muss also gespeichert/wiederhergestellt werden !


  int    irc0, irc1, irc2;


  printd("STP_r_mdl__ |%s|\n",mdl.nam);


  // STP_r_creMod_Run
  // STP_r_cre1 (iobj);  // activate



  //----------------------------------------------------------------
  // nur fuer Submodels:
  // if(mdlNr >= 1) {

    // reset Startindizes
    AP_obj_2_txt (NULL, 0L, NULL, 0L);

    // den Buffer zuruecksetzen.
    UTF_clear1 ();

    // den Namen in die ModelnameTable speichern
    // DB_StoreModBas (1, mdlNam); // 1=internalModel

    // nur vor dem ersten Submodel: save Hidelist --> File
    // if(mdlNr == 1)
      // GA_hide_fil_tmp (1);

    // clear Hidelist
    GA_hide__ (-1, 0L, 0);
  // }



  //----------------------------------------------------------------
  sprintf(gTxt,"### Step-Model %s",mdl.nam);
  UTF_add1_line (gTxt);


  // create the geometry ..
  if(mdl.iGeo >= 0) {
    irc1 = STP_r_cre2 (mdl.iGeo);
    if(irc1 < 0) {
      if(irc1 == -4) {
        printf(" exit from STP_r_mdl__ err=%d\n",irc1);
        return irc1;              // irreparabler Fehler; stop.
      }
    }
  }
    printd(" irc1=%d\n",irc1);


  // work all dittos for this product
  irc2 = STP_r_mdl_dit ();
    printd(" irc2=%d\n",irc2);



  //----------------------------------------------------------------
  // end of Mainmodel
  // if(mdlNr >= 1) goto L_end_sub;

  // STP_r_ckDit1 ();  // check if Dittos automat zu setzen sind ..

  sprintf(gTxt,"### End STEP-Import");
  UTF_add1_line (gTxt);

  // UTF_FilBuf1 an die Hauptdatei (UTF_FilBuf0, nur memory!) anfuegen
  // UTF_insert1 (-1L);

  // return 0;




  //----------------------------------------------------------------
  // end of Submodel
  L_end_sub:

  if((irc1 >= 0) ||
     (irc2 > 0))      {
    ++mdlNr;
    irc0 = 0;

    // Buffer + Hidelist in die Datei schreiben,
    Mod_savSubBuf1 (mdl.nam, modSiz);

  } else {
    irc0 = -1;
  }

  return irc0;

}
 

//================================================================
  int STP_r_mdl_geo (int iProDefSha) {
//================================================================
// create the geometry ..
// find primary geometric obj from ?
// primary geometric obj's are:
// ADVANCED_BREP_SHAPE_REPRESENTATION > MANIFOLD_SOLID_BREP
// Input = PRODUCT_DEFINITION_SHAPE


// ProDefSha:
//  SHAPE_DEFINITION_REPRESENTATION(ProDefSha,geomType(AdvBreShaRep))
//   ADVANCED_BREP_SHAPE_REPRESENTATION (,ManSolBre,)        // AdvBreShaRep
//    MANIFOLD_SOLID_BREP (,CloShe)                          // ManSolBre

// ProDefSha:
// SHAPE_DEFINITION_REPRESENTATION(ProDefSha,ShaRep);
//  SHAPE_REPRESENTATION_RELATIONSHIP(,,ShaRep,GBSSR);
//   GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION(,iGeoSet,,) // GBSSR


// ProDefSha:  get PRODUCT_DEFINITION_SHAPE of ditto
// SHAPE_DEFINITION_REPRESENTATION(ProDefSha,ShaRep);
//  NEXT_ASSEMBLY_USAGE_OCCURRENCE(,ShaRep,ProDef)
//   PRODUCT_DEFINITION_SHAPE(,ProDef)

// see also STP_r_findPROD


  int   irc, *ia, i1, li, ii, ProDefSha, iShaDefRep, lShaDefRep,
        iGeo, iShaRep, iShaRepRel, iNxtAss;
  int   ShaRep;


  ProDefSha = s_tab[iProDefSha].sInd;

  printd(" STP_r_mdl_geo %d #%d\n",iProDefSha,ProDefSha);


  // find SHAPE_DEFINITION_REPRESENTATION where its L1 = PRODUCT_DEFINITION_SHAPE
  iShaDefRep = STP_r_find_sRec_TypL1 (SC_SHAPE_DEFINITION_REPRES, ProDefSha);
  if(iShaDefRep < 0) return -1;
  ia = s_tab[iShaDefRep].sDat;
  ShaRep = ia[1];        // 2.link = ADVANCED_BREP_SHAPE_REPRESENTATION |
                         //          SHAPE_REPRESENTATION
  iGeo = i_tab[ShaRep];
    printd(" iGeo=%d ShaRep=#%d typ=%d\n",iGeo,ShaRep,s_tab[iGeo].sTyp);


  // iGeo = ADVANCED_BREP_SHAPE_REPRESENTATION > MANIFOLD_SOLID_BREP
  if(s_tab[iGeo].sTyp == SC_ADVANCED_BREP_SR) goto L_resolv;



  // if iGeo == SHAPE_REPRESENTATION: find SHAPE_REPRESENTATION_RELATIONSHIP
  if(s_tab[iGeo].sTyp == SC_SHAPE_REPRESENTATION) {

    // this is a subModelReference or a GEOMETRICALLY_BOUNDED_..
    // subModelReference ?
    // get L1 of PRODUCT_DEFINITION_SHAPE
    ia = s_tab[iProDefSha].sDat;
    li = ia[0]; // 1.link of PRODUCT_DEFINITION_SHAPE (PRODUCT_DEFINITION)
    ii = i_tab[li];
      printd(" PRODUCT_DEFINITION_SHAPE-L1 %d #%d typ=%d\n",ii,li,s_tab[ii].sTyp);


    // if L1 of PRODUCT_DEFINITION_SHAPE == PRODUCT_DEFINITION:
    if(s_tab[ii].sTyp == SC_PRODUCT_DEFINITION) goto L_SR_1; 


    // find NEXT_ASSEMBLY_USAGE_OCCURRENCE where L1 = ProDefSha_L1
    iNxtAss = STP_r_find_sRec_TypL1 (SC_NEXT_ASSEMBLY_USAGE_OCCURR, li);
    if(iNxtAss >= 0) {
      iGeo = iNxtAss;
      goto L_resolv;
    }
       

    // find SHAPE_REPRESENTATION_RELATIONSHIP where L1=ShaRep
    L_SR_1:
    iShaRepRel = STP_r_find_sRec_TypL1 (SC_SHAPE_REPRES_RS, ShaRep);
    if(iShaRepRel < 0) return -1;
    ia = s_tab[iShaRepRel].sDat;
    ii = ia[1];    // 2.link=geometry
    iGeo = i_tab[ii];
      printd(" iGeo=%d #%d typ=%d\n",iGeo,ii,s_tab[iGeo].sTyp);

  }



  //----------------------------------------------------------------
  L_resolv:
  irc = STP_r_cre2 (iGeo);
      if(irc < 0) {
        if(irc == -4) {
          printf(" exit from STP_r_creMod_Run err=%d\n",irc);
          return irc;              // irreparabler Fehler; stop.
        }
      }
  // s_tab[i1].stat = 2;  // done


}
*/

//================================================================
  int STP_r_mdl_dit () {
//================================================================
// work all dittos for the active product (struct mdl)
// RetCod: nr of exported dittos

  int    iNr, irc, *ia, ii, i1, l1, ProDefShaL1, iProDef, iNxtAssL2;


  printd("STP_r_mdl_dit %d #%d\n",mdl.iProDefSha,s_tab[mdl.iProDefSha].sInd);

  if(mdl.iProDefSha < 0) return -1;

  // find all NEXT_ASSEMBLY_USAGE_OCCURRENCE where L1 = L1 of ProDefSha
  ia = s_tab[mdl.iProDefSha].sDat;
  ProDefShaL1 = ia[0];

  iNr = 0;

  for(i1=1; i1<s_Nr; ++i1) {

    // get next NEXT_ASSEMBLY_USAGE_OCCURR
    if(s_tab[i1].sTyp != SC_NEXT_ASSEMBLY_USAGE_OCCURR) continue;

    // skip if already worked ..
    if(s_tab[i1].gInd >= 0) continue;


    ia = s_tab[i1].sDat;
    if(ia[0] != ProDefShaL1) continue;
      printd(" NEXT_ASSY %d #%d L1=#%d L2=#%d\n",i1,
             s_tab[i1].sInd,ProDefShaL1,ia[1]);

    // find PRODUCT_DEFINITION_SHAPE where L1 = iNxtAssL2
    // iNxtAssL2 = ia[1];
    // ii = STP_r_find_sRec_TypL1 (SC_PRODUCT_DEFINITION_SHAPE, iNxtAssL2);

    // find PRODUCT_DEFINITION_SHAPE where L1 = NEXT_ASSEMBLY_USAGE_OCCURRENCE
    l1 = s_tab[i1].sInd;
    ii = STP_r_find_sRec_TypL1 (SC_PRODUCT_DEFINITION_SHAPE, l1);
    if(ii < 0) continue;
      printd(" new iProDefSha=%d #%d\n",ii,s_tab[ii].sInd);

    // create ditto
    irc = STP_r_creDit2 (ii, i1);
    if(irc >= 0) ++iNr;


  }

  return iNr;

}


/*
//================================================================
  int STP_r_cre1 (int isdr) {
//================================================================
// activate rec. isdr if it is a objGroup.
 
  // printf("STP_r_cre1 %d\n",isdr);

  if((s_tab[isdr].sTyp == SC_GBOUNDED_WIREFRAME_SR) ||
     (s_tab[isdr].sTyp == SC_ADVANCED_BREP_SR)      ||
     (s_tab[isdr].sTyp == SC_GBOUNDED_SURFACE_SR)   ||
     (s_tab[isdr].sTyp == SC_MANIFOLD_SURFACE_SR))           {

    // printf(" obj %d #%d=%s\n",isdr,s_tab[isdr].sInd,
           // STP_r_TypTxt_sTyp(s_tab[isdr].sTyp));

    s_tab[isdr].stat = 1;   // activate


  } else {
    // TX_Print("STP_r__ E015 %d",s_tab[isdr].sTyp);
    TX_Print("STP_r__ E015 #%d=%s",s_tab[isdr].sInd,
           STP_r_TypTxt_sTyp(s_tab[isdr].sTyp));
    // printf(" obj %d #%d=%s\n",isdr,s_tab[isdr].sInd,
           // STP_r_TypTxt_sTyp(s_tab[isdr].sTyp));
    return -1;
  }


  // STP_r_creModSiz (isdr);


  return 0;

}


//================================================================
  int STP_r_creModSiz (int sInd) {
//================================================================
// #93=MANIFOLD_SURFACE_SHAPE_REPRESENTATION('NONE',(#52,#96,#129),#46);
//  #46=(GEOMETRIC_REPRESENTATION_CONTEXT(3)
//      GLOBAL_UNCERTAINTY_ASSIGNED_CONTEXT((#45))..
//   #45=UNCERTAINTY_MEASURE_WITH_UNIT(LENGTH_MEASURE(0.005)..

//



  int    *iap, il, ii;
  long   l1;
  double d1;
 
  printf("STP_r_creModSiz %d\n",sInd);


  // isdr sollte aus einem Linkblock und einem Link bestehen.
  // Der Link zeigt auf GEOMETRIC_REPRESENTATION_CONTEXT
  iap = s_tab[sInd].sDat;
  ii = iap[0];
  l1 = iap[ii+1];
    // printf(" ii=%d l1 = #%d\n",ii,l1);

  il = STP_r_findInd (l1, sInd);        // il = GEOMETRIC_REPRESENTATION_CONTEXT


  // GEOMETRIC_REPRESENTATION_CONTEXT hat einen Link -->
  //   UNCERTAINTY_MEASURE_WITH_UNIT. Dieser Record hat eine Toleranz.
  iap = s_tab[il].sDat;
  l1 = iap[0];
    // printf(" l1 = #%d\n",l1);
  il = STP_r_findInd (l1, il);        // il = UNCERTAINTY_MEASURE_WITH_UNIT.
    // printf(" l1=%d il=%d\n",il,l1);
  STP_r_getDb (&d1, s_tab[il].sDat);
    // printf(" il=%d d1=%f\n",il,d1);


  // Aus Toleranz ModSiz errechnen.


  // ModSiz ausgeben.

  return 0;

}
*/


//================================================================
  int STP_r_dec0 () {
//================================================================
// Input ist mem_cbuf1;  Kommentare ("/* ... */") sind schon entfernt,
// mem_cbuf1 ist eine komplette step-zeile; last char ist ';'.
// RetCod -2 = diese Zeile kann nicht decodiert werden; weiter zur naechsten.
// RetCod -3 = diese Zeile wird nicht verwendet ..
// RetCod -4 = irreparabler Fehler; stop.
// RetCod -5 = EOF


  int    irc, i1;
  char   *p1, *p2, *p3;
  char   *dc1, *dc2, ctc;


  // printf(" \n");
  // printf("STP_r_dec0 |");UTX_dump__(mem_cbuf1,60);printf("|\n");
  // printd("==========================================\n");
  // printd("STP_r_dec0 %60.60s\n",mem_cbuf1);



  if(mem_cbuf1[0] != '#') {
   if(!strcmp(mem_cbuf1, "ENDSEC;")) return -5;   // End of File 
  }


  // decode Index --> s_Ind
  dc1 = strchr(&mem_cbuf1[1], '=');
  if(dc1 == NULL) {TX_Error("STP_r_dec0 E002"); return -2;}
  *dc1 = '\0';
  UTX_skip_1bl (&dc1); // skip this char and following blanks

  s_Ind = atoi (&mem_cbuf1[1]);     // decode #<sInd>
  if(s_Ind > s_Max) s_Max = s_Ind;
    // printf(" s_Ind=%d dc1=|%c|\n",s_Ind,*dc1);
    // TESTEXIT
    // if(s_Ind > 3809) exit(0);  // TEST ONLY


  // is it a compoundLine ("#124=(subCmd1(..)subCmd2(..));") ?
  if(*dc1 != '(') return STP_r_dec1 (&dc1);


  UTX_skip_1bl (&dc1); // skip '(' and folowing blanks ..


  // den s_dat-Record fuer die compoundLine anlegen; dann s_Ind auf 0 setzen.
  // hier_weiter....


  s_MainInd = s_Ind;
  s_Ind = 0;              // following Records are subRecords
  MainStat = -1;          // s_MainInd-Record noch nicht angelegt.



  L_nxt:
  // printf(" subCmd\n");
  while (*dc1  == ' ')  ++dc1;    // ACIS hat: "#264 = SHAPE_..."
  if(*dc1 == ')') return 0;
  // if(*dc1 == ';') return 0;

  // resolv compoundLine; find '('
  p1 = strchr(dc1, '(');
  if(p1 == NULL) {TX_Error("STP_r_dec0 E003"); return -2;}

  // find the end of the command;
  p2 = UTX_pos_skipBrack (p1);
  if(*p2 == '\0') {TX_Error("STP_r_dec0 E004"); return -2;}
  // printf("  p2=|");UTX_dump__(p2,60);printf("|\n");
  // terminate the command
  ++p2;
  ctc = *p2;
  *p2 = ';';


  // work
  irc = STP_r_dec1 (&dc1);

  // restore termination of previous command; resolv next part.
  *p2 = ctc;
  dc1 = p2;
  goto L_nxt;


  return 0;

}


//================================================================
  int STP_r_dec1 (char **cBuf) {
//================================================================
// eine Zeile decodieren u intern speichern.
// cBuf is without "#<s_Ind>="
// cBuf is without "(" and ")" for compoundLines.
// speichern in struct s_tab[].
// decode s_Ind und Hauptwort from mem_cbuf1
// RetCod -1 = EOF
// RetCod -2 = diese Zeile kann nicht decodiert werden; weiter zur naechsten.
// RetCod -3 = diese Zeile wird nicht verwendet ..
// RetCod -4 = irreparabler Fehler; stop.


  int     irc, i1;
  long    l1, ld;
  char    *p1, *p2, *p3;
  char    *dc1, *dc2;
  STP_MDL actMdl;


  // printf("////////////////////////////////////////// %d\n",s_Ind);
  // printf("STP_r_dec1 |");UTX_dump__(*cBuf,60);printf("|\n");



  // // debug-halt, wenn ein ebstimmter Record eingespeichert wurde,
  // if(s_Nr == 825) AP_debug__ ();
  // if(strstr(*cBuf, "#10=")) AP_debug__ ();



  // check if mind MIND_SIZ in s_dat free ..
  if(((char*)s_dat.next + MIND_SIZ) > (char*)s_dat.end) {
    // reallocate s_dat
    irc = UME_reall_add (&l1, &s_dat, MIND_SIZ);
    if(irc < 0) {TX_Error("STP_r_dec1 EOM1"); return -4;}
    // if memspace has moved: modify all pointers s_tab[].sDat
    if(l1 != 0) STP_r_reall_CB ((char*)s_dat.start - l1, s_dat.start);
    // wieder weg - wurde von UME_reall_add zugefuegt !
    s_dat.next = (void*)((char*)s_dat.next - MIND_SIZ);
  }



/*
  //================================================================
  // geometryRecord or subRecord:
  // #123=CARTESIAN_POINT(..       = geometryRecord
  // B_SPLINE_CURVE(2,(#297,#..    = subRecord von zB BOUNDED_CURVE

  //------- geometryRecord -----------------------------------------
  if(mem_cbuf1[0] == '#') {          // geometryRecord 

    dc1 = strchr(&mem_cbuf1[1], '=');
    if(dc1 == NULL) {TX_Error("STP_r_dec1 E002"); return -2;}
    *dc1 = '\0'; ++dc1;
    while (*dc1  == ' ')  ++dc1;    // ACIS hat: "#264 = SHAPE_..."

    s_Ind = atoi (&mem_cbuf1[1]);     // decode #<sInd>
    if(s_Ind > s_Max) s_Max = s_Ind;





  //-------- subRecord ---------------------------------------------
  } else {
    if(!strcmp(mem_cbuf1, "ENDSEC;")) return -1;   // End of File
    // printf("|%s|\n",mem_cbuf1);
    // TX_Error("STP_r_dec1 E001");
    // return -2;

    dc1 = mem_cbuf1;
    s_Ind = 0;

  }

*/



  //================================================================
  // dc1 = Ausdruck; "( BOUNDED.." od "CARTESIAN::"
  // *dc1 ist the 1. char;  '(' or 'C'
  // *dc2 ist der 2 Char des DefCode.
  // p2  wird auf den ersten char des ersten word in der Klammer gesetzt;
  //     '(' nach dem DefCode wird durch '\0' ersetzt;
  dc1 = *cBuf;
  while (*dc1  == ' ')  ++dc1;  // skip leading blanks
  dc2 = dc1 + 1;   // second char of 1. word; 
  // p2 = dc1;


  // find next '(' -
  // terminate dc1 here with '\0'; set p2 behind \0
  p2 = strchr(dc2, '(');
  if(p2 == NULL) {TX_Error("STP_r_dec1 E003"); return -2;}
  *p2 = '\0';
  p3 = p2;
  UTX_skip_1bl (&p2); // skip this char and following blanks

  // remove following blanks from dc1
  L_nxtFollBlk:
  --p3;
  if(*p3 == ' ') {
    *p3 = '\0';
    goto L_nxtFollBlk;
  }
    // printd(" dc1=|%s|\n",dc1);


  irc = 1;



  switch(*dc1) {


    case 'A':
      // ADVANCED_BREP_SHAPE_REPRESENTATION
      if(!strcmp(dc1, "ADVANCED_BREP_SHAPE_REPRESENTATION")) {
        // ('',(#154,#53),#211);
        // ('',(#154),#211);
        // LinkBlock bestehend aus:
        //   1. Link --> AXIS2_PLACEMENT_3D
        //   2. Link --> MANIFOLD_SOLID_BREP
        // Link --> GEOMETRIC_REPRESENTATION_CONTEXT (unused)
        // irc = STP_r_decModel4 (p2);
        irc = STP_r_savLinkB (SC_ADVANCED_BREP_SR, &p2);
        if(irc < 0) return irc;
        i1 = 1;
        irc = STP_r_decLinks (&i1, &p2);  // decode & save Links
        STP_r_geoTab_add ();
        break;

      // ADVANCED_FACE
      } else if(!strcmp(dc1, "ADVANCED_FACE")) {
        irc = STP_r_decFace1 (p2);
        break;

      } else if(*dc2 == 'P') {
        // APPROVAL*
        // APPLICATION_CONTEXT
        // APPLICATION_PROTOCOL_DEFINITION
        goto L_skip;

      // AXIS1_PLACEMENT
      } else if(!strcmp(dc1, "AXIS1_PLACEMENT")) {
        // 2 Links (Point, DIRECTION)
        irc = STP_r_savLDL (SC_AXIS1_PLACEMENT, 2,0,0, p2);
        break;

      // AXIS2_PLACEMENT_3D
      } else if(!strcmp(dc1, "AXIS2_PLACEMENT_3D")) {
        // AXIS2_PLACEMENT_3D
        //  ('',#205,#163,#164)
        //  ('',#76,#77,$)
        //  ('',#76,$,$)
        // #CARTESIAN_POINT(Origin), #DIRECTION(Z-Achse), #DIRECTION(X-Achse)
        // 3 Links
        irc = STP_r_savLDL (SC_AXIS2_PLACEMENT_3D, 3,0,0, p2);
        break;
      }


    case 'B':
      // B_SPLINE_CURVE
      if(!strcmp(dc1, "B_SPLINE_CURVE")) {
        irc = STP_r_decSpl2 (p2);            // f. subCommand
        break;

      // B_SPLINE_CURVE_WITH_KNOTS
      } else if(!strcmp(dc1, "B_SPLINE_CURVE_WITH_KNOTS")) {
        if(s_Ind > 0) irc = STP_r_decSpl1 (p2);
        else          irc = STP_r_decSpl3 (p2);            // f. subCommand
        break;

      // B_SPLINE_SURFACE
      } else if(!strcmp(dc1, "B_SPLINE_SURFACE")) {
        // irc = STP_r_savInt (SC_B_SPLINE_SURFACE);
        // if(irc < 0) break;
        irc = STP_r_decBspSu1 (&p2);            // nur subCommand
        break;

      // B_SPLINE_SURFACE_WITH_KNOTS
      } else if(!strcmp(dc1, "B_SPLINE_SURFACE_WITH_KNOTS")) {
        if(s_Ind > 0) irc = STP_r_decBspSu_ (p2, s_Ind);
        else          irc = STP_r_decBspSu2 (&p2);            // f. subCommand
        break;

      // BREP_WITH_VOIDS
      } else if(!strcmp(dc1, "BREP_WITH_VOIDS")) {
        TX_Print("***** BREP_WITH_VOIDS - NOT YET IMPLEMENTED ***");
      }
      // BOUNDED_CURVE   (subCmd)
      // BOUNDED_SURFACE (subCmd)
      goto L_skip;



    case 'C':
      if(*dc2 == 'A') {
        // CARTESIAN_POINT
        if(!strcmp(dc1, "CARTESIAN_POINT")) {
          irc = STP_r_sav3DB (SC_CARTESIAN_POINT, p2);
          // der Punkt liegt nun auf s_tab[s_Nr].sDat
          STP_r_addBox (1);   // extend box
          break;
        }
        // CALENDAR_DATE
        goto L_skip;
      } else if(*dc2 == 'C') {
        // CC_DESIGN_APPROVAL
        // CC_DESIGN_SECURITY_CLASSIFICATION
        // CC_DESIGN_PERSON_AND_ORGANIZATION_ASSIGNMENT
        // CC_DESIGN_DATE_AND_TIME_ASSIGNMENT
        goto L_skip;

      } else if(*dc2 == 'I') {
        // CIRCLE
        if(!strcmp(dc1, "CIRCLE")) {
          // CIRCLE('',#377,20.);
          // AXIS2_PLACEMENT_3D, radius
          // 1 Link, 1 double
          // irc = STP_r_decCi1 (p2);
          irc = STP_r_savLDL (SC_CIRCLE, 1,1,0, p2);
          break;
        }

      } else if(*dc2 == 'L') {
        // CLOSED_SHELL            < MANIFOLD_SOLID_BREP
        if(!strcmp(dc1, "CLOSED_SHELL")) {
          // LinkBlock --> ADVANCED_FACEs
          irc = STP_r_savLinkB (SC_CLOSED_SHELL, &p2);
          // STP_r_geoTab_add ();
          break;
        }

      } else if(*dc2 == 'O') {
        // COLOUR_RGB
        if(!strcmp(dc1, "COLOUR_RGB")) {
          // #23=COLOUR_RGB('Colour',0.50,0.75,0.90) ;
          irc = STP_r_savLDL (SC_COLOUR_RGB, 0,3,0, p2);
          // aktuellen ind s_Nr merken
          indCol = s_Nr;
          goto L_skip;
        // COMPOSITE_CURVE
        } else if(!strcmp(dc1, "COMPOSITE_CURVE")) {
          irc = STP_r_decCCV1 (p2);
          break;
        // COMPOSITE_CURVE_SEGMENT
        } else if(!strcmp(dc1, "COMPOSITE_CURVE_SEGMENT")) {
          irc = STP_r_decCCV2 (p2);
          break;
        // CONICAL_SURFACE
        } else if(!strcmp(dc1, "CONICAL_SURFACE")) {
          // CONICAL_SURFACE('NONE',#54,92.0,15.0);
          // 1 Link  --> AXIS
          // 2 doubles (radius, halber_Oeffnungswinkel)
          irc = STP_r_savLDL (SC_CONICAL_SURFACE, 1,2,0, p2);
          break;

        // (CONVERSION_BASED_UNIT
        } else if(!strcmp(dc1, "CONVERSION_BASED_UNIT")) {
          // CONVERSION_BASED_UNIT('DEGREE',#308)
          // CONVERSION_BASED_UNIT('INCH',#309)      // unused ..
          // openAngles (of CONICAL_SURFACE) = DEGREES (nur von CATIA-V4)
          // else = default = radians !!!!
          if(!strncmp(p2, "'DEGREE'",8)) angMod = 1;
          // printf(" angMod=%d p2=|%s|\n",angMod,p2);
          goto L_skip;


        // CONNECTED_EDGE_SET
        } else if(!strcmp(dc1, "CONNECTED_EDGE_SET")) {
          // CONNECTED_EDGE_SET('',(#168));
          // LinkBlock --> EDGE_CURVEs
          irc = STP_r_savLinkB (SC_CONNECTED_EDGE_SET, &p2);
          break;

        // CONTEXT_DEPENDENT_SHAPE_REPRESENTATION(#607,#605);
        } else if(!strcmp(dc1, "CONTEXT_DEPENDENT_SHAPE_REPRESENTATION")) {
          // is open item (topmost item of dittos)
          // L1 = REPRESENTATION_RELATIONSHIP (needed for Transformation)
          // L2 = PRODUCT_DEFINITION_SHAPE
          irc = STP_r_nxtSrec ();  // next s-obj-Record
          if(irc < 0) return irc;
          s_tab[s_Nr].sTyp = SC_CONTEXT_DEPENDENT_SR;
          i1 = 2;
          irc = STP_r_decLinks (&i1, &p2);  // decode & save Links
        break;
        }
        // skip these:
        // COORDINATED_UNIVERSAL_TIME_OFFSET
        goto L_skip;
      } else if(*dc2== 'Y') {
        // CYLINDRICAL_SURFACE
        if(!strcmp(dc1, "CYLINDRICAL_SURFACE")) {
          // CYLINDRICAL_SURFACE('',#276,6.35);
          // 1 Link  --> AXIS
          // 1 double   (radius)
          irc = STP_r_savLDL (SC_CYLINDRICAL_SURFACE, 1,1,0, p2);
          break;
        }
      }
      // CURVE (subCmd)
      goto L_skip;


    case 'D':
      if(!strcmp(dc1, "DEGENERATE_TOROIDAL_SURFACE")) {
        // =DEGENERATE_TOROIDAL_SURFACE('',#3,9,10.,.T.) ;
        // Link Axis,
        // Radius des WurstZentrumsKreises.
        // Radius des Wurstquerschnittes;
        irc = STP_r_savLDL (SC_DEGENERATE_TOROIDAL_SURFACE, 1,2,0, p2);
        break;

      } else if(!strcmp(dc1, "DIRECTION")) {
        // irc = STP_r_decDir1 (p2);
        irc = STP_r_sav3DB (SC_DIRECTION, p2);
        break;
      }
      // DATE_AND_TIME
      // DATE_TIME_ROLE
      goto L_skip;


    case 'E':
      if(*dc2 == 'D') {
        // EDGE_CURVE
        if(!strcmp(dc1, "EDGE_CURVE")) {
          // EDGE_CURVE('',#178,#171,#176,.F.) ;
          // #VERTEX_POINT   #VERTEX_POINT    #LINE/CIRCLE reverse(.T./.F.)
          // 3 Links, 1 int
          // irc = STP_r_decCurv2 (p2);
          irc = STP_r_savLDL (SC_EDGE_CURVE, 3,0,1, p2);
          break;
        // EDGE_LOOP
        } else if(!strcmp(dc1, "EDGE_LOOP")) {
          // EDGE_LOOP('',(#109,#110,#111)) ;
          // n ORIENTED_EDGEs
          // Nr of Links, Links
          // irc = STP_r_decCurv3 (p2);
          // printf(" EDGE_LOOP|%s|\n",p2);
          irc = STP_r_savLinkB (SC_EDGE_LOOP, &p2);
          break;
        // EDGE_BASED_WIREFRAME_MODEL
        } else if(!strcmp(dc1, "EDGE_BASED_WIREFRAME_MODEL")) {
          // EDGE_BASED_WIREFRAME_MODEL('',(#179,#191,#203));
          // LinkBlock --> CONNECTED_EDGE_SETs
          irc = STP_r_savLinkB (SC_EDGE_BASED_WIREFRAME_MODEL, &p2);
          break;
        // EDGE_BASED_WIREFRAME_SHAPE_REPRESENTATION
        } else if(!strcmp(dc1, "EDGE_BASED_WIREFRAME_SHAPE_REPRESENTATION")) {
          // EDGE_BASED_WIREFRAME_SHAPE_REPRESENTATION('',(#179));
          // LinkBlock --> EDGE_BASED_WIREFRAME_MODELs
          irc = STP_r_savLinkB (SC_EDGE_BASED_WIREFRAME_SR, &p2);
          break;
        }

      } else if(*dc2== 'L') {
        // ELLIPSE
        if(!strcmp(dc1, "ELLIPSE")) {
          // ELLIPSE('',#82,14.7,12.12);
          // #AXIS, A-AchsLaenge, B-AchsLaenge
          // 1 Link -Refsys
          // 2 x double
          // irc = STP_r_decElli1 (p2);
          irc = STP_r_savLDL (SC_ELLIPSE, 1,2,0, p2);
          break;
        }
      } else break;


    case 'F':
      // FACE_BOUND
      if(!strcmp(dc1, "FACE_BOUND")) {
        irc = STP_r_savLDL (SC_FACE_BOUND, 1,0,0, p2);
        break;

      // FACE_OUTER_BOUND
      } else if(!strcmp(dc1, "FACE_OUTER_BOUND")) {
        // FACE_OUTER_BOUND('',#108,.T.);
        // 1 Link = EDGE_LOOP
        irc = STP_r_savLDL (SC_FACE_OUTER_BOUND, 1,0,0, p2);
        break;

      } else if(!strcmp(dc1, "FACETED_BREP_SHAPE_REPRESENTATION")) {
        irc = STP_r_savLinkB (SC_FACETED_BREP_SR, &p2);
        if(irc < 0) return irc;
        i1 = 1;
        irc = STP_r_decLinks (&i1, &p2);  // decode & save Links
        break;
      }

      if(*dc2 == 'I') {   
        // FILL_AREA_STYLE
        // FILL_AREA_STYLE_COLOUR
        goto L_skip;
      }




    case 'G':
      // GEOMETRIC_CURVE_SET
      if(!strcmp(dc1, "GEOMETRIC_CURVE_SET")) {
        irc = STP_r_savLinkB (SC_GEOMETRIC_CURVE_SET, &p2);
        break;

      // GEOMETRIC_SET
      } else if(!strcmp(dc1, "GEOMETRIC_SET")) {
        // n Links --> geometr. obj's
        irc = STP_r_savLinkB (SC_GEOMETRIC_SET, &p2);
        break;

      // GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION
      } else if(!strcmp(dc1,
               "GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION")) {
        // n Links --> GEOMETRIC_SET
        // printf(" SC_GBOUNDED_SURFACE_SR/1 |%s|\n",p2);
        irc = STP_r_savLinkB (SC_GBOUNDED_SURFACE_SR, &p2);
        if(irc < 0) return irc;
        // printf(" SC_GBOUNDED_SURFACE_SR/2 |%s|\n",p2);
        i1 = 1;
        irc = STP_r_decLinks (&i1, &p2);  // decode & save Links
        STP_r_geoTab_add ();
        break;

    
      } else if(!strcmp(dc1,
               "GEOMETRICALLY_BOUNDED_WIREFRAME_SHAPE_REPRESENTATION")) {
        // GEOMETRICALLY_BOUNDED_WIREFRAME_SHAPE_REPRESENTATION('',(#1),#2);
        // #1 = linkBlock GEOMETRIC_CURVE_SET
        // #2 = link GEOMETRIC_REPRESENTATION_CONTEXT
        // printf(" SC_GBOUNDED_WIREFRAME_SR/1 |%s|\n",p2);
        // decode block GEOMETRIC_CURVE_SET
        irc = STP_r_savLinkB (SC_GBOUNDED_WIREFRAME_SR, &p2);
        if(irc < 0) return irc;
        // printf(" SC_GBOUNDED_WIREFRAME_SR/2 |%s|\n",p2);
        // decode link GEOMETRIC_REPRESENTATION_CONTEXT
        i1 = 1;
        irc = STP_r_decLinks (&i1, &p2);  // decode & save Links
        STP_r_geoTab_add ();
        break;

      // GLOBAL_UNCERTAINTY_ASSIGNED_CONTEXT
      // } else if(!strcmp(dc1, "GLOBAL_UNCERTAINTY_ASSIGNED_CONTEXT")) {
        // // GLOBAL_UNCERTAINTY_ASSIGNED_CONTEXT((#45))
        // // subCommend im (GEOMETRIC_REPRESENTATION_CONTEXT - Block;
        // // der Link zeigt auf UNCERTAINTY_MEASURE_WITH_UNIT
        // // printf(" SC_GLOBAL_UNCERT_ASS_CTXT |%s|\n",p2);
        // ++p2;   // eigentlich Block!
        // i1 = 1;
        // irc = STP_r_decLinks (&i1, &p2);  // decode & save Links
        // break;

      // (GEOMETRIC_REPRESENTATION_CONTEXT
      // } else if(!strcmp(dc1, "GEOMETRIC_REPRESENTATION_CONTEXT")) {
          // irc = STP_r_decSubCmd (p2, SC_GEOMETRIC_REPR_CTXT);
          // break;
        // (LENGTH_UNIT
        // (NAMED_UNIT
      }

      // GEOMETRIC_REPRESENTATION_ITEM (subCmd)
      // GLOBAL_.. (subCmd)
      goto L_skip;


    case 'I':
      // ITEM_DEFINED_TRANSFORMATION
      if(!strcmp(dc1, "ITEM_DEFINED_TRANSFORMATION")) {
        // ITEM_DEFINED_TRANSFORMATION(' ',' ',#174,#191);
        // 2 Links (2 x AXIS2_PLACEMENT_3D)
        irc = STP_r_savInit (SC_ITEM_DEFINED_TRANSFORMATION, &p2);
        if(irc < 0) return irc;
        STP_r_skipT (&p2);                // skip Textfield
        i1 = 2;
        irc = STP_r_decLinks (&i1, &p2);  // decode & save Links
        break;

      // INTERSECTION_CURVE
      } else if(!strcmp(dc1, "INTERSECTION_CURVE")) {
        // INTERSECTION_CURVE(name, curv, (surf1, surf2), .CURVE_3D.)
          irc = STP_r_savLDL (SC_INTERSECTION_CURVE, 1,0,0, p2);
        break;
      }
      goto L_skip;


    case 'L':
      // LINE
      if(!strcmp(dc1, "LINE")) {
          // LINE('',#377,#176);
          // #CART.POINT,#DIRECTION
          // 2 Links
          irc = STP_r_savLDL (SC_LINE, 2,0,0, p2);
          break;
      }
      // LENGTH_MEASURE_WITH_UNIT
      // LOCAL_TIME
      // LENGTH_UNIT (subCmd)
      goto L_skip;



    case 'M':
      if(*dc2 == 'A') {
        // MANIFOLD_SOLID_BREP
        if(!strcmp(dc1, "MANIFOLD_SOLID_BREP")) {
          // 1 Link --> CLOSED_SHELL
          irc = STP_r_savLDL (SC_MANIFOLD_SOLID_BREP, 1,0,0, p2);
          break;

        //                OPEN_SHELL > SHELL_BASED_SURFACE_MODEL >
        // MANIFOLD_SURFACE_SHAPE_REPRESENTATION
        } else if(!strcmp(dc1, "MANIFOLD_SURFACE_SHAPE_REPRESENTATION")) {
          // MANIFOLD_SURFACE_SHAPE_REPRESENTATION('*MASTER',(#92),#45);
          // Block of Links -> subObjects
          irc = STP_r_savLinkB (SC_MANIFOLD_SURFACE_SR, &p2);
          if(irc < 0) return irc;
          i1 = 1;
          irc = STP_r_decLinks (&i1, &p2);  // decode & save Links
          STP_r_geoTab_add ();
          break;
        }
        // MAPPED_ITEM
        

      } else if(*dc2 == 'E') {
        // MEASURE_REPRESENTATION_ITEM
        // MECHANICAL_CONTEXT
        goto L_skip;
      }


    case 'N':
      // NAME_ATTRIBUTE
      // NAMED_UNIT (subCmd)

      // NEXT_ASSEMBLY_USAGE_OCCURRENCE('','','',#17,#56,'');
      // - Link to PRODUCT_DEFINITION of main
      // - Link to PRODUCT_DEFINITION_WAD or PRODUCT_DEFINITION of Detail
      // Dzt nur Link1 speichern !
      if(!strcmp(dc1, "NEXT_ASSEMBLY_USAGE_OCCURRENCE")) {
        irc = STP_r_savInit (SC_NEXT_ASSEMBLY_USAGE_OCCURR, &p2);
        if(irc < 0) return irc;
        STP_r_skipT (&p2);                // skip Textfield
        STP_r_skipT (&p2);                // skip Textfield
        i1 = 2;
        irc = STP_r_decLinks (&i1, &p2);  // decode & save Links
        STP_r_refTab_add ();
        break;
      }
      goto L_skip;


    case 'O':
      // OPEN_SHELL
      if(!strcmp(dc1, "OPEN_SHELL")) {
        irc = STP_r_savLinkB (SC_OPEN_SHELL, &p2);
        break;
      // ORGANIZATION
      } else if(!strcmp(dc1, "ORGANIZATION")) {
        goto L_skip;
      // ORIENTED_EDGE
      } else if(!strcmp(dc1, "ORIENTED_EDGE")) {
          irc = STP_r_savEdge1 (p2);
          break;
      }


    case 'P':
      if(*dc2 == 'E') {
        // PERSON
        // PERSON_AND_ORGANIZATION
        // PERSON_AND_ORGANIZATION_ROLE
        // PERSONAL_ADDRESS
        goto L_skip;
      } else if(*dc2 == 'L') {
        // PLANE
        if(!strcmp(dc1, "PLANE")) {
          // PLANE('',#325);
          //  AXIS
          // 1 Link --> AXIS (RefSys)
          // irc = STP_r_decPln1 (p2);
          irc = STP_r_savLDL (SC_PLANE, 1,0,0, p2);
          break;
        }
        // PLANE_ANGLE_MEASURE_WITH_UNIT
        // PLANE_ANGLE_UNIT (subCmd)
        goto L_skip;

      } else if(*dc2 == 'R') {
        // PRODUCT('Part','','not specified',(#47));
        // (Modelname, name, description, product_context);
        if(!strcmp(dc1, "PRODUCT")) {
          // irc = STP_r_savLDL (SC_PRODUCT, 0,0,0, p2);
          irc = STP_r_nxtSrec ();  // next s-obj-Record
          if(irc < 0) return irc;
          s_tab[s_Nr].sTyp = SC_PRODUCT;
          irc = STP_r_decTx1 (&p1, 1, &p2);  // decode & save Modelname in s_dat
          if(irc < 0) return irc;
          // MemTab_sav (&mdlTab, &ld, &s_Ind, 1);   // add 1 record
          actMdl.iPROD = s_Ind;
          actMdl.nam   = p1;
            printd("	add PROD %d |%s|\n",actMdl.iPROD,actMdl.nam);
          MemTab_sav (&mdlTab, &ld, &actMdl, 1);   // add 1 record
          break;

        // PRODUCT_CATEGORY_RELATIONSHIP
        // PRODUCT_CONTEXT

        // PRODUCT_DEFINITION_FORMATION
        // PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE
        } else if(
          (!strcmp(dc1,"PRODUCT_DEFINITION_FORMATION"))  ||
          (!strcmp(dc1,"PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE"))) {
          // PRODUCT_DE...CIFIED_SOURCE('','',#46,.MADE.);
          // (id, description, #PRODUCT, make_or_buy)
          // I-DEAS verwendet PDF;
          // C4,C5 u Rhino PDFWSS. Inhalt scheinbar ident; daher gleiches SC_.
          // see STP_r_findPROD
          irc = STP_r_savInit (SC_PRODUCT_DEFINITION_FORMWSS, &p2);
            // printf(" PDF-1-p2=|%s|\n",p2);
          if(irc < 0) return irc;
          STP_r_skipT (&p2);                // skip Textfield
            // printf(" PDF-2-p2=|%s|\n",p2);
          i1 = 1;
          irc = STP_r_decLinks (&i1, &p2);  // decode & save Links
          break;

        // PRODUCT_DEFINITION
        } else if(!strcmp(dc1, "PRODUCT_DEFINITION")) {
          // PRODUCT_DEFINITION('design','',#45,#44);
          // (id, description, #PRODUCT_DEFINITION_FWSS, #DESIGN_CONTEXT)
          irc = STP_r_savInit (SC_PRODUCT_DEFINITION, &p2);
          if(irc < 0) return irc;
          STP_r_skipT (&p2);                // skip Textfield
          i1 = 1;
          irc = STP_r_decLinks (&i1, &p2);  // decode & save Links
          break;

        // PRODUCT_DEFINITION_CONTEXT
        // PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE

        // PRODUCT_DEFINITION_SHAPE
        } else if(!strcmp(dc1, "PRODUCT_DEFINITION_SHAPE")) {
          // PRODUCT_DEFINITION_SHAPE('','Shape',#43);
          // (id, description, link1)
          // link1 = PRODUCT_DEFINITION | NEXT_ASSEMBLY_USAGE_OCCURRENCE
          //   PRODUCT_DEFINITION                 this obj is a subModel
          //   NEXT_ASSEMBLY_USAGE_OCCURRENCE     this is a subModelReference
          irc = STP_r_savInit (SC_PRODUCT_DEFINITION_SHAPE, &p2);
          if(irc < 0) return irc;
          STP_r_skipT (&p2);                // skip Textfield
          i1 = 1;
          irc = STP_r_decLinks (&i1, &p2);  // decode & save Links
          break;

        // PRODUCT_DEFINITION_WITH_ASSOCIATED_DOCUMENTS('','',#55,#3,(#58));
        } else if(!strcmp(dc1,"PRODUCT_DEFINITION_WITH_ASSOCIATED_DOCUMENTS")) {
          irc = STP_r_savInit (SC_PRODUCT_DEFINITION_WITH_AD, &p2);
          if(irc < 0) return irc;
          STP_r_skipT (&p2);                // skip Textfield
          i1 = 1;
          irc = STP_r_decLinks (&i1, &p2);  // decode & save Links
          break;
        }

        // PRODUCT_RELATED_PRODUCT_CATEGORY
        // PROPERTY_DEFINITION
        // PROPERTY_DEFINITION_REPRESENTATION
        goto L_skip;
      }


    case 'Q':
      // QUASI_UNIFORM_CURVE; .POLYLINE_FORM. only for boundaries
      if(!strcmp(dc1, "QUASI_UNIFORM_CURVE")) {
        if(s_Ind > 0) irc = STP_r_decCvUni (p2); // fuer mainCommand;
        else          goto L_skip;               // skip als subCommand
        break;
      // QUASI_UNIFORM_SURFACE
      // wird bei ADVANCED_FACE als Stuetzflaeche fuer planare Flaechen benutzt;
      // nur Header, keine daten speichern ..
      } else if(!strcmp(dc1, "QUASI_UNIFORM_SURFACE")) {
        if(s_Ind > 0)
          irc = STP_r_savInit (SC_QUASI_UNIFORM_SURFACE, &p2);
        else goto L_skip;
        break;
      }
      goto L_skip;



    case 'R':
      // RATIONAL_B_SPLINE_CURVE
      if(!strcmp(dc1, "RATIONAL_B_SPLINE_CURVE")) {
        irc = STP_r_decSpl4 (p2);            // f. subCommand
        break;

      // RATIONAL_B_SPLINE_SURFACE
      } else if(!strcmp(dc1, "RATIONAL_B_SPLINE_SURFACE")) {
        irc = STP_r_decBspSu3 (&p2);            // nur subCommand
        break;

      // RECTANGULAR_TRIMMED_SURFACE
      } else if(!strcmp(dc1, "RECTANGULAR_TRIMMED_SURFACE")) {
        // RECTANGULAR_TRIMMED_SURFACE('*SUR1',#55,0.,360.,162.4,0.,.T.,.T.);
        // 1 Link --> Stuetzflaeche (CONICAL_SURFACE/CYLINDRICAL_SURFACE ..)
        // 4 doubles (u1,u2, v1,v2)
        irc = STP_r_savLDL (SC_RECTANGULAR_TRIMMED_SURFACE, 1,4,0, p2);
        break;

      
      // REPRESENTATION_RELATIONSHIP
      } else if(!strcmp(dc1, "REPRESENTATION_RELATIONSHIP")) {
          irc = STP_r_decDit1 (p2, 0);
          break;

      // REPRESENTATION_RELATIONSHIP_WITH_TRANSFORMATION
      } else if(!strcmp(dc1,"REPRESENTATION_RELATIONSHIP_WITH_TRANSFORMATION")){
          irc = STP_r_decDit1 (p2, 1);
          break;

      }
      // REPRESENTATION
      // REPRESENTATION_MAP
      // REPRESENTATION_ITEM (subCmd)
      goto L_skip;




    case 'S':
      if(*dc2 == 'E') {
        // SECURITY_CLASSIFICATION
        // SECURITY_CLASSIFICATION_LEVEL
        goto L_skip;

      } else if(*dc2 == 'H') {
        // SHELL_BASED_SURFACE_MODEL
        if(!strcmp(dc1, "SHELL_BASED_SURFACE_MODEL")) {
          // SHELL_BASED_SURFACE_MODEL('NONE',(#93,#138));
          irc = STP_r_savLinkB (SC_SHELL_BASED_SURFACE_MODEL, &p2);
          break;

        // SHAPE_ASPECT

        // SHAPE_DEFINITION_REPRESENTATION(#14,#100);
        // 1.Link: PRODUCT_DEFINITION_SHAPE
        // 2.Link: SHAPE_REPRESENTATION or
        //         ADVANCED_BREP_SHAPE_REPRESENTATION
        //         GBOUNDED_WIREFRAME_SR
        // ACHTUNG: als 3. Int wird ein LeerInf fuer den PRODUCT-Index
        //          reserviert.
        } else if(!strcmp(dc1, "SHAPE_DEFINITION_REPRESENTATION")) {
          irc = STP_r_nxtSrec ();  // keine id to skip ..
          if(irc < 0) return irc;
          s_tab[s_Nr].sTyp = SC_SHAPE_DEFINITION_REPRES;
          i1 = 2;
          irc = STP_r_decLinks (&i1, &p2);  // decode & save Links
          // ein leeres Int-Feld speichern; wird spaeter mit
          // dem Index des PRODUCT-Record befuellt !!
          i1 = 0;
          UME_save (&s_dat, &i1, sizeof(int));
          break;


        // SHAPE_REPRESENTATION
        } else if(!strcmp(dc1, "SHAPE_REPRESENTATION")) {
          // SHAPE_REPRESENTATION('*MASTER',(#169,#182,#191),#45);
          // Block ... alles AXIS2_PLACEMENT_3D; refSys of dittos.
          // Link  --> SHAPE_REPRESENTATION_RELATIONSHIP od 
          //           GEOMETRIC_REPRESENTATION_CONTEXT  od
          //           ADVANCED_BREP_SHAPE_REPRESENTATION ..
          irc = STP_r_savInit (SC_SHAPE_REPRESENTATION, &p2);
          if(irc < 0) return irc;
          // STP_r_skipB (&p2);
          // i1 = 1;
          // irc = STP_r_decLinks (&i1, &p2);  // decode & save Links
          break;

        // SHAPE_REPRESENTATION_RELATIONSHIP
        } else if(!strcmp(dc1, "SHAPE_REPRESENTATION_RELATIONSHIP")) {
          // SHAPE_REPRESENTATION_RELATIONSHIP(' ',' ',#156,#151);
          // 1. Link -> Model (SHAPE_REPRESENTATION)
          // 2. Link -> GEOM_BOUND_SURF_SH_REP od ADV_BREP_SH_REP ..
          // Achtung: ist als subCommand leer - skippen.
          if(s_Ind == 0) goto L_skip;  // subCmd in REPRESENTATION_RELATIONSHIP
          irc = STP_r_savInit (SC_SHAPE_REPRES_RS, &p2);
          if(irc < 0) return irc;
          STP_r_skipT (&p2);                // skip Textfield
          i1 = 2;
          irc = STP_r_decLinks (&i1, &p2);  // decode & save Links
          break;
        }
        goto L_skip;

      } else if(*dc2 == 'I') {
        // SI_UNIT (subCmd)
        goto L_skip;

      } else if(*dc2 == 'O') {
        // SOLID_ANGLE_UNIT (subCmd)
        goto L_skip;

      } else if(*dc2 == 'P') {
        // SPHERICAL_SURFACE('',#53,25.0);
        // 1 Link --> AXIS
        // 1 double = Radius
        irc = STP_r_savLDL (SC_SPHERICAL_SURFACE, 1,1,0, p2);
        break;
      } else if(*dc2 == 'T') {
        // STYLED_ITEM
        goto L_skip;
      } else if(*dc2 == 'U') {
        // SURFACE_OF_REVOLUTION
        if(!strcmp(dc1, "SURFACE_OF_REVOLUTION")) {
          // irc = STP_r_savLDL (SC_ELLIPSE, 1,2,0, p2);
          // 2 Links; Countour - axis
          irc = STP_r_savLDL (SC_SURFACE_OF_REVOLUTION, 2,0,0, p2);
          break;

        // SURFACE_OF_LINEAR_EXTRUSION
        } else if(!strcmp(dc1, "SURFACE_OF_LINEAR_EXTRUSION")) {
          // SURFACE_OF_LINEAR_EXTRUSION('',#<KonturKurve>,#<VEKTOR>);
          irc = STP_r_savLDL (SC_SURFACE_OF_LINEAR_EXTRUSION, 2,0,0, p2);
          break;

        }
        // SURFACE_STYLE_FILL_AREA
        // SURFACE_SIDE_STYLE
        // SURFACE_STYLE_USAGE
        // SURFACE
        goto L_skip;
      }



    case 'T':
      // TOROIDAL_SURFACE
      // 1.Link = AXIS2_PLACEMENT_3D
      // 2.double = rad1 (mainradius)
      // 3.double = rad2 (radius of contourcircle)
      if(!strcmp(dc1, "TOROIDAL_SURFACE")) {
        // 1 Link, 2 doubles.
        irc = STP_r_savLDL (SC_TOROIDAL_SURFACE, 1,2,0, p2);
        break;

      // TRIMMED_CURVE
      } else if(!strcmp(dc1, "TRIMMED_CURVE")) {
        irc = STP_r_decCurv1 (p2);
        break;
      }




    case 'U':
/*
      // UNCERTAINTY_MEASURE_WITH_UNIT(LENGTH_MEASURE(0.005)..
      if(!strcmp(dc1, "UNCERTAINTY_MEASURE_WITH_UNIT")) {
        // skip "LENGTH_MEASURE("
        if(!strncmp(p2, "LENGTH_MEASURE(", 15)) p2 += 15;
        else { printf("UNCERTAINTY_MEASURE_WITH_UNIT E001"); goto L_skip;}
        irc = STP_r_nxtSrec ();
        if(irc < 0) return irc;
        s_tab[s_Nr].sTyp = SC_UNCERT_MEASURE_WITH_UNIT;
          printf(" SC_UNCERT_MEASURE_WITH_UNIT s_Nr=%d |%s|\n",s_Nr,p2);
        i1 = 1;
        irc = STP_r_decDbs (&i1, &p2);
        break;
      }
*/
      goto L_skip;



    case 'V':
      // VECTOR
      if(!strcmp(dc1, "VECTOR")) {
        // VECTOR ('',#201,1.);
        // 1 Link, 1 double.
        // irc = STP_r_decVec1 (p2);
        irc = STP_r_savLDL (SC_VECTOR, 1,1,0, p2);
        break;
      // VERTEX_LOOP
      } else if(!strcmp(dc1, "VERTEX_LOOP")) {
        // #110=FACE_BOUND('',#113,.T.);
        // #113=VERTEX_LOOP('',#111)
        // #111=VERTEX_POINT('',#112);
        // VERTEX_LOOP is a curve from one point only ..
        irc = STP_r_savLDL (SC_VERTEX_LOOP, 1,0,0, p2);
        break;
      // VERTEX_POINT
      } else if(!strcmp(dc1, "VERTEX_POINT")) {
        // VERTEX_POINT ('',#309);
        // ein Link.
        // irc = STP_r_decPoint2 (p2);
        irc = STP_r_savLDL (SC_VERTEX_POINT, 1,0,0, p2);
        break;
      }




    // unknown word; display it !
    default:
      TX_Print("*** unknown stepCode |%s|",dc1);
      printf("********** unknown stepCode:******************\n|%s|\n",dc1);
  }

  if(irc == 1) TX_Print("*** unknown stepCode |%s|",dc1);

  // printf("ex STP_r_dec1 %d %d %s\n",irc,s_Ind,dc1);


  return irc;

  L_skip:
  return 0;

}


//================================================================
  int STP_r_geoTab_add () {
//================================================================
// add active inputrecord to geoTab
      
  long    ld;
  STP_I2  r1;
    

  printd("STP_r_geoTab_add %d\n",s_Ind);
      
  r1.ii = s_Ind;
  r1.iMdl = -1;
        
  MemTab_sav (&geoTab, &ld, &r1, 1);   // add 1 record

  return 0;
        
}     
      

//================================================================
  int STP_r_refTab_add () {
//================================================================
// add active inputrecord to refTab
 
  long    ld;
  STP_I2  r1;

  r1.ii = s_Ind;
  r1.iMdl = -1;

  MemTab_sav (&refTab, &ld, &r1, 1);   // add 1 record

  return 0;

}


//================================================================
  int STP_r_addBox (int mode) {
//================================================================
// errechnen einer ModSiz.
// Das Einlesen der Step-Records erfolgt mit dem gerade aktiven ModSiz. !?
// Beim Einlesen jedes CARTPT wird mode=1 gerufen == Box bilde.
// Nach dem Einlesen - vor dem Abarbeiten wird (mode=2) ModSiz gesetzt.

// Besser waere:  do not use hidden obj's for ModelSize ...



static Point p1, p2;
       Point *pp;

#define VAL_INFIN 10000

  int    i1;
  // double d1;


  if(mode == 0) {
    UT3D_box_ini0 (&p1, &p2);

    // beim Einlesen ist die Toleranz wichtig !
    // Coupling_Link: wenn ueber 100 werden die Circs in subModel Bushing
    // nicht begrenzt !
    WC_Init_Modsiz (100.);
    // wird spaeter nochmal gestzt ..
    return 0;
  }



  // wird bei CARTESIAN_POINT gerufen ...
  if(mode == 1) {
    // DEB_dump_obj__ (Typ_PT, s_tab[s_Nr].sDat, "    ext.");
    // Werte ausserhalb der Norm eliminieren (C4 hat schreckliches ..)
    pp = (Point*)s_tab[s_Nr].sDat;
    if((fabs(pp->x) > VAL_INFIN) ||
       (fabs(pp->y) > VAL_INFIN) ||
       (fabs(pp->z) > VAL_INFIN))  return 0;
    UT3D_box_extend (&p1, &p2, (Point*)s_tab[s_Nr].sDat);
    return 0;
  }




  if(mode == 2) {
    // DEB_dump_obj__ (Typ_PT, &p1, "box-p1:");
    // DEB_dump_obj__ (Typ_PT, &p2, "box-p2:");
    modSiz = UTP_db_rnd5 (UT3D_len_2pt (&p1, &p2));
    // printf(" ModSiz --> %d\n",modSiz);
  }



  return 0;

}


/*    UNUSED
//================================================================
  int STP_r_creMod__ (int mNr) {
//================================================================
// alle Objs des Model mNr aktiv setzen. Das sind:
//      "SHAPE_REPRESENTATION_RELATIONSHIP"s od
//      "(REPRESENTATION_RELATIONSHIP("s


  int     i1, i2, im, io, iNr;
  void   *vp;

  // printf("\n");
  // printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM \n");
  // printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM \n");
  // printf("STP_r_creMod__ #%d\n",mNr);


  // Activate all "SHAPE_REPRESENTATION_RELATIONSHIP"s
  // referring to Model mNr

  iNr = 0;

  for(i1=0; i1<s_Nr; ++i1) {

    //----------------------------------------------------------------
    if(s_tab[i1].sTyp == SC_SHAPE_REPRES_RS) {
      if(s_tab[i1].stat > 0) continue; // alread done

      // check if Model (1. Link) == active Model.
      vp = STP_r_getInt (&im, s_tab[i1].sDat);     // get Link -> Model
      if(im != mNr) continue;
      // activate jenes objekt, auf das die SHAPE_REPRESENTATION_RELATIONSHIP
      // zeigt (2. Link).
      vp = STP_r_getInt (&io, vp);                 // get Link -> obj's
      // io ist nun ein Link auf:
      //   GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION oder
      //   ADVANCED_BREP_SHAPE_REPRESENTATION.  Index suchen;
      i2 = STP_r_findInd (io, i1);


    //----------------------------------------------------------------
    } else if(s_tab[i1].sTyp == SC_REPRESENTATION_RELATIONSHIP) {  // Ditto
      if(s_tab[i1].stat > 0) continue; // alread done

      // check if Model (1. Link) == active Model.
      vp = STP_r_getInt (&im, s_tab[i1].sDat);     // get Link -> Model
      if(im != mNr) continue;
      // activate this Record.
      i2 = i1;


    //----------------------------------------------------------------
    } else continue;


    // printf(" [%d] SRR #%d\n",i2,s_tab[i2].sInd);
    s_tab[i2].stat = 1;   // activate
    ++iNr;
  }

  return iNr;

}


//================================================================
  int STP_r_creMod_Run (int ModNr, char *ModNam) {
//================================================================
// work
// ModNr: 0=MainModel (hintanfuegen), ab 1 Submodel (write to File)
// Submodels: schreibt mit UTF_add1_line --> UTF_FilBuf1,
//   es wird aber die originale HIDE-Tabelle gesetzt;
//   muss also gespeichert/wiederhergestellt werden !


  int   i1, irc;


  // printf("STP_r_creMod_Run %d |%s|\n",ModNr,ModNam);


  // nur fuer Submodels:
  if(ModNr >= 1) {

    // reset Startindizes
    AP_obj_2_txt (NULL, 0L, NULL, 0L);

    // den Buffer zuruecksetzen.
    UTF_clear1 ();

    // den Namen in die ModelnameTable speichern
    DB_StoreModBas (1, ModNam); // 1=internalModel

    // nur vor dem ersten Submodel: save Hidelist --> File
    if(ModNr == 1) GA_hide_fil_tmp (1);

    // clear Hidelist
    GA_hide__ (-1, 0L, 0);
  }



  sprintf(gTxt,"### Step-Model %s",ModNam);
  UTF_add1_line (gTxt);
 

  for(i1=0; i1<s_Nr; ++i1) {
    if(s_tab[i1].stat == 1) {

      // printf("      creMod_Run Ind=%d\n",i1);
      irc = STP_r_cre2 (i1);
      // printf(" n.cre2 %d\n",irc);
      if(irc < 0) {
        if(irc == -4) {
          printf(" exit from STP_r_creMod_Run err=%d\n",irc);
          return irc;              // irreparabler Fehler; stop.
        }
      }
      s_tab[i1].stat = 2;  // done
    }
  }


  //----------------------------------------------------------------
  // end of Mainmodel
  if(ModNr >= 1) goto L_end_sub;

  STP_r_ckDit1 ();  // check if Dittos automat zu setzen sind ..

  sprintf(gTxt,"### End STEP-Import");
  UTF_add1_line (gTxt);

  // UTF_FilBuf1 an die Hauptdatei (UTF_FilBuf0, nur memory!) anfuegen
  UTF_insert1 (-1L);

  return 0;
  


  //----------------------------------------------------------------
  // end of Submodel
  L_end_sub:

  // Buffer + Hidelist in die Datei schreiben,
  Mod_savSubBuf1 (ModNam, modSiz);
/
  // UTF_add1_line ("# End Submodel\n");
  // sprintf(gTxt,"%stmp%cModel_%s",OS_get_bas_dir(),fnam_del,ModNam);
  // UTF_file_Buf1 (gTxt);
/


  return 0;
 
}
*/

//================================================================
  int STP_r_creCol (int ii) {
//================================================================

  int    i1, i2, i3;
  double d1, *dap;


  // printf("STP_r_creCol %d\n",ii);
  // printf(" C sdat[]=%p %f\n",s_tab[563].sDat,*((double*)s_tab[563].sDat));

  dap = (double*)s_tab[ii].sDat;
    // printf(" sdat = %p\n",dap);

  i1 = *dap * 255.;
    // printf(" i1=%d d1=%f\n",i1,*dap);

  ++dap;
  i2 = *dap * 255.;
    // printf(" i2=%d d1=%f\n",i2,*dap);

  ++dap;
  i3 = *dap * 255.;
    // printf(" i3=%d d1=%f\n",i3,*dap);

/*
  iap = STP_r_getDb (&d1, s_tab[ii].sDat);
  i1 = d1 * 255;
    printf(" i1=%d d1=%f\n",i1,d1);

  iap = STP_r_getDb (&d1, iap);
  i2 = d1 * 255;
    printf(" i2=%d d1=%f\n",i2,d1);

  iap = STP_r_getDb (&d1, iap);
  i3 = d1 * 255;
    printf(" i3=%d d1=%f\n",i3,d1);
*/

  AP_SetCol3i (i1, i2, i3);

/*
  sprintf(gTxt,"DEFCOL %d %d %d\n",i1,i2,i3);
  irc = UTF_add1_line (gTxt);
*/

  return 0;

}


/*
//================================================================
  int STP_r_cre1 () {
//================================================================
// resolv and create all active step-objects

  int   i1, irc;

  for(i1=0; i1<s_Nr; ++i1) {
    if(s_tab[i1].stat != 0) {

      printf("STP_r_cre1 %d %d\n",i1,resMod);
      irc = STP_r_cre2 (i1);
      printf(" n.cre2 %d\n",irc);
      if(irc < 0) {
        if(irc == -4) return irc;              // irreparabler Fehler; stop.
      }
    }
  }
  return 0;
}
*/

//================================================================
  int STP_r_cre2 (int sInd) {
//================================================================
// resolv geometry-record; create cad-object.
// Input: sInd = index into s_tab

  int     irc, iNr, iTyp, i1, i2, ii, iDir, *iap, iTab[2], lInd;
  long    l1;
  double  d1, d2;
  s_obj   *so1;
  Plane   pl1;
  ObjGX   ox1;
  // Point *pt1;



  if(sInd < 0) return -1;

  // printf("\n");
  // printf("////////////////////////////////////////// \n");
    printd("STP_r_cre2 %d #%d=%s\n",sInd,s_tab[sInd].sInd,
           STP_r_TypTxt_sTyp(s_tab[sInd].sTyp));
  // debug-halt, wenn ein bestimmter Record eingespeichert wurde,
  // if(sInd == 80) AP_debug__ ();


  // check if obj already exists
    // printf("   exist: gInd[%d]=%d\n",sInd,s_tab[sInd].gInd);
  if(s_tab[sInd].gInd >= 0) return 0;


  // check for enough Memory in s_dat; wenn ein record halb rausgeschrieben
  // ist, ist ein reallocate fehlerhaft (der momentane Record wird nicht 
  // korrekt behandelt)

  so1 = &s_tab[sInd];
  iap = so1->sDat;



  switch (so1->sTyp) {


    //----------------------------------------------------------------
    case SC_DIRECTION:
      irc = STP_r_creVc1 (sInd);
      if(irc < 0) return irc;
      break;


    //----------------------------------------------------------------
    case SC_CARTESIAN_POINT:
      // nur 3 doubles 
      // DEB_dump_obj__ (Typ_PT, so1->sDat, "");

      // make obj, save-> gCad, stor gC-Typ & Index -> s_tab
      irc = STP_r_creObj1 (sInd, Typ_PT, Typ_PT, so1->sDat);
      if(irc < 0) return irc;

      if(resMod == 2)     // hide bei Solids
        // GA_hide__ (0, s_tab[sInd].gInd, s_tab[sInd].gTyp);
        GA_view__ (-1L, 1, s_tab[sInd].gTyp, s_tab[sInd].gInd);

      break;


    //----------------------------------------------------------------
    case SC_VERTEX_POINT:
    case SC_VERTEX_LOOP:                    // 2012-11-26
      // 1 Link --> CARTESIAN_POINT
        // printf(" VERTEX_POINT #%d L1=#%d\n",s_tab[sInd].sInd,iap[0]);
      i1 = STP_r_findInd (iap[0], sInd);
      STP_r_cre2 (i1);  // recurse !
      // copy gTyp gInd sDat from CART_P --> VERT_P
      // damit hat VERTEX keinen Link mehr sondern direkt die Koordinaten !!
      // printf(" copy %d <-- %d\n",sInd,i1);
      s_tab[sInd].gTyp = s_tab[i1].gTyp;
      s_tab[sInd].gInd = s_tab[i1].gInd;
      // KATASTROPH: overwrites step-links !
      // s_tab[sInd].sDat = s_tab[i1].sDat; // damit man coords bekommt ..
      break;


    //----------------------------------------------------------------
    case SC_LINE:
      // besteht aus Point + Vektor; wenn eine TRIMMED_CURVE darueber liegt,
      // dann Point+Vektor ignorieren (Line from trimpoints).
      // Wird auch benutzt fuer eine SRV-Conturline; hier wirds gebraucht !!
      break;


    //----------------------------------------------------------------
    case SC_QUASI_UNIFORM_CURVE:
      i1 = iap[0];
      i2 = iap[1];
      irc = STP_r_creLn1 (i1, i2, 0, sInd);
      break;


    //----------------------------------------------------------------
    case SC_CIRCLE:
      irc = STP_r_creCi1 (0, 0, 0, sInd);
      if(irc < 0) return irc;
      break;


    //----------------------------------------------------------------
    case SC_ELLIPSE:
      return STP_r_creEl0 (sInd);


    //----------------------------------------------------------------
    case SC_TRIMMED_CURVE:    // SEE ALSO STP_r_creCurv1
      irc = STP_r_creCurv2 (sInd);
      if(irc < 0) return irc;
      break;


    //----------------------------------------------------------------
    case SC_B_SPLINE_CURVE_WITH_KNOTS:
      irc = STP_r_creSpl1 (sInd);
      if(irc < 0) return irc;
      break;


    //----------------------------------------------------------------
    case SC_BOUNDED_CURVE:
      irc = STP_r_creSplBC (sInd);
      if(irc < 0) return irc;
      break;


    //----------------------------------------------------------------
    case SC_COMPOSITE_CURVE_SEGMENT:
      // printf(" =============================\n");
      // printf(" CCV-Seg #%d %d\n",s_tab[sInd].sInd,sInd);
      i1 = STP_r_findInd (iap[0], sInd);
      STP_r_cre2 (i1);  // recurse !
        // printf(" seg #%d(%d) <-- #%d(%d)\n",s_tab[sInd].sInd,sInd,
                                            // s_tab[i1].sInd,i1);
        // printf("    copy typ=%d ind=%d\n",s_tab[i1].gTyp,s_tab[i1].gInd);
      // copy gTyp u gInd (for CCV)
      s_tab[sInd].gTyp = s_tab[i1].gTyp;
      s_tab[sInd].gInd = s_tab[i1].gInd;
      break;


    //----------------------------------------------------------------
    case SC_COMPOSITE_CURVE:
      // zuerst alle Subelemente, am Ende die CCV ausgeben.
      // Anzahl < 2: keine CCV ausgeben.
      // erste Zahl ist die Anzahl

      // test if all segments are TRIMMED_CURVE/LINE
      // (wenn alle Subelemente Lines sind, dann keine CCV sondern ein Polygon)
      irc = STP_r_creCCV_ckl (iap);
      if(irc == 0) {
        STP_r_crePlg (iap, sInd);
        break;
      }

      // create subelements
      ii = sInd;
      iNr = iap[0];
      ++iap;
      for(i1=0; i1<iNr; ++i1) {
        ii = STP_r_findInd (iap[i1], ii);
        // printf(" res1 CCV-obj Nr.%d - ind=%d typ=%d\n",i1,ii,s_tab[ii].sTyp);
        STP_r_cre2 (ii);  // recurse !
        // printf(" made %d %d %d\n",s_Nr,s_tab[s_Nr].gTyp,s_tab[s_Nr].gInd);
      }
      if(iNr < 2) break;
      gTxt[0] = '\0';
      for(i1=0; i1<iNr; ++i1) {
        ii = STP_r_findInd (iap[i1], ii);
          // printd(" CCV-obj-%d - %d #%d typ=%d\n",i1,
                  // ii,s_tab[ii].sInd,s_tab[ii].sTyp);
        AP_obj_add_obj (gTxt, s_tab[ii].gTyp, s_tab[ii].gInd);
      }
      irc = STP_r_creObj1 (sInd, Typ_CVTRM, Typ_Txt, gTxt);
      if(irc < 0) return irc;

      if(resMod == 2)     // hide bei Solids
        // GA_hide__ (0, s_tab[sInd].gInd, s_tab[sInd].gTyp);
        GA_view__ (-1L, 1, s_tab[sInd].gTyp, s_tab[sInd].gInd);

      // return -4;  // TEST
      break;




    //----------------------------------------------------------------
    case SC_EDGE_CURVE:
    case SC_ORIENTED_EDGE:
      irc = STP_r_creCurv1 (sInd);
      if(irc < 0) return irc;
      break;




    //----------------------------------------------------------------
    case SC_AXIS2_PLACEMENT_3D:
      irc = STP_r_crePln1 (sInd);
      if(irc < 0) return irc;
      break;




    //----------------------------------------------------------------
    case SC_CYLINDRICAL_SURFACE:
      irc = STP_r_creSurCyl1 (sInd);
      if(irc < 0) return irc;
      break;


    //----------------------------------------------------------------
    case SC_CONICAL_SURFACE:
      irc = STP_r_creSur3 (sInd);
      if(irc < 0) return irc;
      break;


    //----------------------------------------------------------------
    case SC_SPHERICAL_SURFACE:
      irc = STP_r_creSur4 (sInd);
      if(irc < 0) return irc;
      break;


    //----------------------------------------------------------------
    case SC_TOROIDAL_SURFACE:
    case SC_DEGENERATE_TOROIDAL_SURFACE:
      irc = STP_r_creSur5 (sInd);
      if(irc < 0) return irc;
      break;


    //----------------------------------------------------------------
    case SC_SURFACE_OF_REVOLUTION:
      irc = STP_r_creSur6 (sInd);
      if(irc < 0) return irc;
      break;


    //----------------------------------------------------------------
    case SC_SURFACE_OF_LINEAR_EXTRUSION:
      irc = STP_r_creSur7 (sInd);
      if(irc < 0) return irc;
      break;


    //----------------------------------------------------------------
    case SC_B_SPLINE_SURFACE_WITH_KNOTS:
      irc = STP_r_creSur8 (sInd);
      if(irc < 0) return irc;
      break;


    //----------------------------------------------------------------
    case SC_BOUNDED_SURFACE:
      irc = STP_r_creSur9 (sInd);
      if(irc < 0) return irc;
      break;


    //----------------------------------------------------------------
    case SC_RECTANGULAR_TRIMMED_SURFACE:
      irc = STP_r_creSur2 (sInd);
      // printf(" n.creSur2 %d\n",irc);
      if(irc < 0) return irc;
      break;


    //----------------------------------------------------------------
    case SC_ADVANCED_FACE:
      irc = STP_r_creSur1 (sInd);
      if(irc < 0) return irc;
      break;


    //----------------------------------------------------------------
    case SC_GEOMETRIC_SET:
    case SC_GEOMETRIC_CURVE_SET:
    case SC_OPEN_SHELL:
    case SC_CLOSED_SHELL:          // resolv Block aus ADVANCED_FACEs
    case SC_CONNECTED_EDGE_SET:    // resolv Block of EDGE_CURVEs
      ii = sInd;
      // erste Zahl ist die Anzahl
      iNr = iap[0];                // printf(" iNr =%d\n",iNr);
      ++iap;
      // resolv Block
      for(i1=0; i1<iNr; ++i1) {
        // printf(">>>>>>>>>>>> nxt srObj[%d] = #%d\n",i1,iap[i1]);
        ii = STP_r_findInd (iap[i1], ii);
        irc = STP_r_cre2 (ii);  // recurse !
        if(irc < 0) {
          if(irc == -4) {TX_Error("STP_r_cre2 EOM1"); return -4;}
        }
      }
      break;


    //----------------------------------------------------------------
    case SC_GBOUNDED_WIREFRAME_SR:
    case SC_GBOUNDED_SURFACE_SR:
    case SC_MANIFOLD_SURFACE_SR:
    case SC_SHELL_BASED_SURFACE_MODEL:
    case SC_EDGE_BASED_WIREFRAME_SR:
    case SC_EDGE_BASED_WIREFRAME_MODEL:
      // erste Zahl ist die Anzahl
      ii = sInd;
      iNr = iap[0];
      ++iap;
      for(i1=0; i1<iNr; ++i1) {
        // printf(">>>>>>>>>>>> nxt wObj[%d] = #%d\n",i1,iap[i1]);
        ii = STP_r_findInd (iap[i1], ii);
        irc = STP_r_cre2 (ii);  // recurse !
        if(irc < 0) {
          if(irc == -4) {TX_Error("STP_r_cre2 EOM2"); return -4;}
        }
      }
      break;


    //----------------------------------------------------------------
    case SC_MANIFOLD_SOLID_BREP:
      ii = STP_r_findInd (iap[0], sInd);  // L1=CLOSED_SHELL
      irc = STP_r_cre2 (ii);  // recurse !
      if(irc < 0) {
        if(irc == -4) {TX_Error("STP_r_cre3 EOM1"); return -4;}
      }
      break;


    //----------------------------------------------------------------
    case SC_ADVANCED_BREP_SR:
      // besteht aus AXIS2_PLACEMENT_3D und MANIFOLD_SOLID_BREP
      resMod = 2;   // hide Planes, Stuetzflaechen ..
      iNr = iap[0];
      ii = sInd;
      ++iap;
      for(i1=0; i1<iNr; ++i1) {
        ii = STP_r_findInd (iap[i1],  ii);  // L2=MANIFOLD_SOLID_BREP
        if(s_tab[ii].sTyp != SC_MANIFOLD_SOLID_BREP) continue;
        irc = STP_r_cre2 (ii);  // recurse !
        if(irc < 0) {
          if(irc == -4) {TX_Error("STP_r_cre3 EOM4"); return -4;}
        }
      }
      resMod = 0;   // hide Planes, Stuetzflaechen ..
      break;


    //----------------------------------------------------------------
    // UNUSED
    case SC_NEXT_ASSEMBLY_USAGE_OCCURR: // Ditto subModelReference
      // get PRODUCT_DEFINITION_SHAPE from NEXT_ASSEMBLY_USAGE_OCCURRENCE
      // find PRODUCT_DEFINITION_SHAPE where L1=NEXT_ASSEMBLY_USAGE_OCCURRENCE
      lInd = s_tab[sInd].sInd;
      ii = STP_r_find_sRec_TypL1 (SC_PRODUCT_DEFINITION_SHAPE, lInd);
      if(ii < 0) return -1;
      return STP_r_creDit2 (ii, sInd);


    //----------------------------------------------------------------
    // case SC_REPRESENTATION_RELATIONSHIP:             // Ditto
      // irc = STP_r_creDit1 (sInd);
      // if(irc < 0) return irc;
      // break;



    //----------------------------------------------------------------
    default:
      TX_Error("STP_r_cre2 E001 %d #%d",so1->sTyp,s_tab[sInd].sInd);
  }


  return 0;

}


//================================================================
  int STP_r_creObj1 (int sInd, int typ, int form, void* data) {
//================================================================
// create new gcad-obj
// Input:
//   sInd      s_tab-index (set gTyp & gInd) or -1 (new Obj)
//   data      must have <form> for <typ>  (see ObjGX)
// Output:
//   retCod    is the gcad-DB-index
//
// make obj, save -> gCad, stor gC-Typ & Index -> s_tab[sInd].gTyp/gInd

  int    irc, i1;
  long   l1;
  char   s1[32];
  ObjGX  ox1;


  printd("STP_r_creObj1 %d %d %d\n",sInd,typ,form);
  // if(form == Typ_Txt) printd(" txt=|%s|\n",(char*)data);


  // ox1 = UTO_stru_2_obj (typ, form, data);
  OGX_SET_OBJ (&ox1, typ, form, 1, data);

  // Objekt in Text umwandeln und save with UTF_add1_line
  // create new gcad-obj
  irc = AP_obj_2_txt (mem_cbuf1, mem_cbuf1_SIZ, &ox1, -1L);
  if(irc < 0) return irc;
    // printf(" _creObj1 |%s|\n",mem_cbuf1);
   


  AP_obj_2_txt_query (&i1, &l1);

  // gTyp und gInd merken
  if(sInd < 0) return l1;  // Testobjekte

  s_tab[sInd].gTyp = i1;
  s_tab[sInd].gInd = l1;


  // TESTAUSGABE Import (liefert DB-ID from Step-ID) !
  // char cBuf[32]; APED_oid_dbo__(cBuf,i1,l1);
  // printf("Import %s from #%d=%s\n",cBuf,s_tab[sInd].sInd,
         // STP_r_TypTxt_sTyp(s_tab[sInd].sTyp));
  // if(s_tab[sInd].sInd == 3809) {TX_Error("TESTEX1"); exit(0);}
    APED_oid_dbo__ (s1, i1, l1);
    printd("%s= STP_r_creObj1 .gTyp=%d .gInd=%ld |%s|\n",
           s1,i1,l1,mem_cbuf1);
 


  return l1;

}



//================================================================
  int STP_r_creVc1 (int sInd) {
//================================================================
/// \code
/// SC_DIRECTION  (koords wie bei punkt)
/// Input:
///   sInd   index of vector in s_tab
/// Output:
///   s_tab[sInd].gTyp = Typ_VC;
///   s_tab[sInd].gInd = ????
/// \endcode

  int     irc;
  Vector  *vc1;

  vc1 = s_tab[sInd].sDat;

  if(sInd==4226)DEB_dump_obj__ (Typ_VC, vc1, "STP_r_creVc1:");


  // Vektoren: test for Defaultvecs (DX/DY/DZ)
  irc = APED_oid_vc (gTxt, vc1);
    if(sInd==4226)printf("ex AP_txt_vec irc=%d |%s|\n",irc,gTxt);
  if(irc != 0) {
    s_tab[sInd].gTyp = Typ_VC;
    s_tab[sInd].gInd = irc;
    return 0;
  }

  gTxt[0] = '\0';

  // AP_obj_add_vc0 (gTxt, vc1);
  AP_obj_add_nval (gTxt, 3, (void*)vc1);

  irc = STP_r_creObj1 (sInd, Typ_VC, Typ_Txt, gTxt);
  if(irc < 0) return irc;


  return 0;

}


    
//================================================================
  int STP_r_crePt0 (int sInd) {
//================================================================
// STP_r_crePt0         create Point from existing CARTESIAN_POINT
// sInd = index of CARTESIAN_POINT-record

  int  irc;


  if(s_tab[sInd].gInd > 0) return 0;

  irc = STP_r_creObj1 (sInd, Typ_PT, Typ_PT, s_tab[sInd].sDat);
  if(irc < 0) return irc;

  return 0;

}
      

//================================================================
  int STP_r_creLn0 (int sInd) {
//================================================================
// create Line from SC_LINE

  int     irc, *iap, i1, i2;
  double  d1;
  Point   pt1, pt2;
  Vector  vc1;


  printd("STP_r_creLn0 %d #%d\n",sInd,s_tab[sInd].sInd);

  // sInd is a LINE; LinkPoint + LinkVec
  iap = s_tab[sInd].sDat;
  i1 = STP_r_findInd (iap[0], sInd);  // PT
  i2 = STP_r_findInd (iap[1], sInd);  // DIR
    printd("  creLn0 i1 %d #%d\n",i1,s_tab[i1].sInd);
    printd("  creLn0 i2 %d #%d\n",i2,s_tab[i2].sInd);


  STP_r_cre2 (i1);  // create origin

  if(s_tab[i1].gInd < 0)
    {TX_Error("STP_r_creLn0 E1_%d",s_tab[sInd].sInd); return -1;}


  STP_r_PT_CARTPT (&pt1, i1);
  STP_r_VC_VEC_1 (&vc1, &d1, i2);
    // printf(" len VEC %d=%f\n",sInd,d1);

  // pt2 = from pt1 in Richtg vc1 len = modSiz
  UT3D_pt_traptvclen (&pt2, &pt1, &vc1, d1);


  gTxt[0] = '\0';
  AP_obj_add_obj (gTxt, Typ_PT, s_tab[i1].gInd);
  AP_obj_add_pt (gTxt, &pt2);

  // printf(" _creLn0 |%s|\n",gTxt);


  irc = STP_r_creObj1 (sInd, Typ_LN, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  if(resMod == 2)     // hide bei Solids
    // GA_hide__ (0, s_tab[sInd].gInd, s_tab[sInd].gTyp);
    GA_view__ (-1L, 1, s_tab[sInd].gTyp, s_tab[sInd].gInd);

  return 0;

}


//================================================================
  int STP_r_creLn1 (int iL1, int iL2, int iDir, int sInd) {
//================================================================
// trim == Points
// create Line between Points iL1 iL2 (Links !)
// iL1, iL2 sind VERTEX; sInd ist Index of LINE.
// Beispiel: s_tab[sInd].sInd=96
// #96=LINE('Line',#93,#95) ;
//  #93=CARTESIAN_POINT('Line Origine',(-3.1,1.15,5.35)) ;
//  #95=VECTOR('Line Direction',#94,1.) ;
//   #94=DIRECTION('Vector Direction',(0.,0.,-1.)) ;


  int i1, i2, irc;


  printd("STP_r_creLn1 #%d #%d #%d %d\n",iL1,iL2,s_tab[sInd].sInd,sInd);


  i1 = STP_r_findInd (iL1, iL1-3);
  STP_r_cre2 (i1);  // recurse !


  i2 = STP_r_findInd (iL2, iL2-3);
  STP_r_cre2 (i2);  // recurse !


  return STP_r_creLn3 (i1, i2, iDir,sInd);

}


//================================================================
  int STP_r_creLn3 (int i1, int i2, int iDir, int sInd) {
//================================================================
// create Line from existing points
// create Line between Points i1 i2 (Points, already created)

  int   irc;


  // printf("STP_r_creLn3 %d %d %d %d\n",i1,i2,iDir,sInd);
  // printf("             #%d #%d #%d\n",s_tab[i1].sInd,s_tab[i2].sInd,
                                   // s_tab[sInd].sInd);


  if(iDir != 0) MEM_swap_int (&i1, &i2);

  gTxt[0] = '\0';
  AP_obj_add_obj (gTxt, Typ_PT, s_tab[i1].gInd);
  AP_obj_add_obj (gTxt, Typ_PT, s_tab[i2].gInd);

  // make obj, save-> gCad, stor gC-Typ & Index -> s_tab
  irc = STP_r_creObj1 (sInd, Typ_LN, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  if(resMod == 2)     // hide bei Solids
    // GA_hide__ (0, s_tab[sInd].gInd, s_tab[sInd].gTyp);
    GA_view__ (-1L, 1, s_tab[sInd].gTyp, s_tab[sInd].gInd);

  return 0;

}


//================================================================
  int STP_r_creLn2 (double u1, double u2, int iDir, int sInd) {
//================================================================
// STP_r_creLn2           create Line from SC_LINE + 2 parameters (length)

  int     irc, *iap, i1, i2;
  // double  d1;
  Point   pt1, pt2, pt3;
  Vector  vc1, vc2;


  printd("STP_r_creLn2 %d %f %f\n",sInd,u1,u2);

  // sInd is a LINE; LinkPoint + LinkVec
  iap = s_tab[sInd].sDat;
  i1 = STP_r_findInd (iap[0], sInd);  // CARTESIAN_POINT
  i2 = STP_r_findInd (iap[1], sInd);  // VECTOR

  // get PT
  STP_r_PT_CARTPT (&pt1, i1);
  // get VECTOR but with true length !
  STP_r_VC_VEC_t (&vc1, i2);


  //----------------------------------------------------------------
  // u1, u2 are parameter-values
  UT3D_vc_multvc (&vc2, &vc1, u1);
  UT3D_pt_traptvc (&pt2, &pt1, &vc2);
  
  UT3D_vc_multvc (&vc2, &vc1, u2);
  UT3D_pt_traptvc (&pt3, &pt1, &vc2);


/*
  //----------------------------------------------------------------
  // u1, u2 are distance-values
  // pt2 = from pt1 in Richtg vc1 len = u1
  UT3D_pt_traptvclen (&pt2, &pt1, &vc1, u1);
    // GR_Disp_pt (&pt2, SYM_TRI_S, 2);

  // pt3 = from pt1 in Richtg vc1 len = u2
  UT3D_pt_traptvclen (&pt3, &pt1, &vc1, u2);
    // GR_Disp_pt (&pt3, SYM_TRI_S, 2);
*/

  //----------------------------------------------------------------
  L_8:
  if(iDir != 0) MEM_swap__ (&pt2, &pt3, sizeof(Point));


  gTxt[0] = '\0';
  AP_obj_add_pt (gTxt, &pt2);
  AP_obj_add_pt (gTxt, &pt3);
  
  irc = STP_r_creObj1 (sInd, Typ_LN, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  if(resMod == 2)     // hide bei Solids
    // GA_hide__ (0, s_tab[sInd].gInd, s_tab[sInd].gTyp);
    GA_view__ (-1L, 1, s_tab[sInd].gTyp, s_tab[sInd].gInd);

  return 0;

}



//================================================================
  int STP_r_creLn4 (int sInd) {
//================================================================
// create Line from AXIS1_PLACEMENT
// create origin; make L= Porigin P(origin+vector)

  int    irc, ipo, ivc;
  void   *vp;
  Point  pt1;
  Vector vc1;

  printd("STP_r_creLn4 %d #%d\n",sInd,s_tab[sInd].sInd);

  vp = STP_r_getInt (&ipo, s_tab[sInd].sDat);   // get Link -> origin
  vp = STP_r_getInt (&ivc, vp);                 // get Link -> DIR

  ipo = STP_r_findInd (ipo, sInd);
  ivc = STP_r_findInd (ivc, sInd);
    printd(" ipo = %d #%d\n",ipo,s_tab[ipo].sInd);
    printd(" ivc = %d #%d\n",ivc,s_tab[ivc].sInd);

  STP_r_cre2 (ipo);  // create origin

  STP_r_PT_CARTPT (&pt1, ipo); // load origin
  STP_r_VC_DIR (&vc1, ivc);    // load DIR

  UT3D_pt_traptvc (&pt1, &pt1, &vc1);


  gTxt[0] = '\0';
  AP_obj_add_obj (gTxt, Typ_PT, s_tab[ipo].gInd);
  AP_obj_add_pt (gTxt, &pt1);

  // make obj, save-> gCad, stor gC-Typ & Index -> s_tab
  irc = STP_r_creObj1 (sInd, Typ_LN, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  if(resMod == 2)     // hide bei Solids
    // GA_hide__ (0, s_tab[sInd].gInd, s_tab[sInd].gTyp);
    GA_view__ (-1L, 1, s_tab[sInd].gTyp, s_tab[sInd].gInd);

  return 0;

}


//================================================================
  int STP_r_creCi1 (int iL1, int iL2, int iDir, int sInd) {
//================================================================
// create Circle (full circle or trimmed circle)
// full circle if iL1 == iL2.
// trim == Points
// sInd = Index Circle
// iL1, iL2 = step-links of 2 points.
// Input:
//   iDir       0=T, 1=F


  int     i1, i2, ic, ii, irc, ipo, ivz;
  double  d1;
  void    *vp, *vp1;
  Point   pt1, pt2;
  Plane   pl1;


  printd("STP_r_creCi1 %d %d %d #%d\n",iL1,iL2,sInd,s_tab[sInd].sInd);



  // get AXIS from Circ
  vp = STP_r_getInt (&ic, s_tab[sInd].sDat);  // AXIS = 1.link of CIRC
  ii = STP_r_findInd (ic, ic-3);
    // printf(" circ-Ax %d %d\n",ic,ii);


/*
  // decode AXIS; create origin & Z-vec.
  vp1  = STP_r_getInt (&ipo, s_tab[ii].sDat);
  ipo  = STP_r_findInd (ipo, ii);
    if(ipo < 0) return -2;

  vp1  = STP_r_getInt (&ivz, vp1);
  ivz  = STP_r_findInd (ivz, ii);
    if(ivz < 0) return -2;

  // create origin ..
  STP_r_cre2 (ipo);

  // create Z-vec
  STP_r_cre2 (ivz);
*/


  // get Centerline from basis_curve (po and vz)
  STP_r_PLN_AXIS2 (&pl1, ii);
        


  // Vollkreis-------------------
  // C = cen rad dz
  if(iL1 == iL2) {
    // get Radius
    STP_r_getDb (&d1, vp);
      // printf(" circ-Rd %f\n",d1);

    // create text
    STP_r_creCi_2 (&pl1.po, d1, &pl1.vz, iDir);


  // Kreisbogen------------------
  // C = ARC p1 p2 pc dz
  } else {

    i1 = STP_r_findInd (iL1, iL1-3);
    STP_r_cre2 (i1);

    i2 = STP_r_findInd (iL2, iL2-3);
    STP_r_cre2 (i2);

    // create Text
    STP_r_creCi_0 (s_tab[i1].gInd, s_tab[i2].gInd, &pl1.po, &pl1.vz, iDir);
  }


  // make obj, save-> gCad, stor gC-Typ & Index -> s_tab
  irc = STP_r_creObj1 (sInd, Typ_CI, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  if(resMod == 2)     // hide bei Solids
    // GA_hide__ (0, s_tab[sInd].gInd, s_tab[sInd].gTyp);
    GA_view__ (-1L, 1, s_tab[sInd].gTyp, s_tab[sInd].gInd);


// TEST ONLY
// if(!strcmp(mdl.nam,"16")) exit(0);

  return 0;

}


//================================================================
  int STP_r_creCi2 (double u1, double u2, int iDir, int sInd) {
//================================================================
// create trimmed circle.
// Input:
//   sInd   s_tab-index of CIRCLE

  int     irc, *iap, i1, i2;
  Point   pt1, pt2, pt3;
  double  rdc;
  Plane   pl1;


  printd("STP_r_creCi2 %d #%d u1=%f u2=%f dir=%d\n",sInd,s_tab[sInd].sInd,
         u1,u2,iDir);

  // sInd is a CIRC; #RefSys + (db)radius
  iap = s_tab[sInd].sDat;
  i1 = STP_r_findInd (iap[0], 0); 

  STP_r_PLN_AXIS2 (&pl1, i1);  // AXIS --> Plane
  STP_r_getDb (&rdc, &iap[1]); // radius
    // printf(" rdc=%f\n",rdc);


  // pt1 = from center(pl1.po) in Richtg X (pl1.vx) len = rdc
  UT3D_pt_traptvclen (&pt1, &pl1.po, &pl1.vx, rdc);
    GR_Disp_pt (&pt1, SYM_TRI_S, 2);

  // pt2 = pt1 um pl1.po/pl1.vz drehen um Winkel = u1
  UT3D_pt_rotptptvcangr (&pt2, &pt1, &pl1.po, &pl1.vz, UT_RADIANS(u1));

  // pt3 = pt1 um pl1.po/pl1.vz drehen um Winkel = u2
  UT3D_pt_rotptptvcangr (&pt3, &pt1, &pl1.po, &pl1.vz, UT_RADIANS(u2));

  // create Text "ARC p1 p2 pc vz [CW]" -> gTxt
  // STP_r_creCi_1 (&pt2, &pt3, &pl1.po, &pl1.vz, iDir);
  STP_r_creCi_1 (&pt2, &pt3, &pl1.po, &pl1.vz, iDir);

  // make obj, save-> gCad, stor gC-Typ & Index -> s_tab
  irc = STP_r_creObj1 (sInd, Typ_CI, Typ_Txt, gTxt);
    printd("  creCi2 irc=%d\n",irc);
  if(irc < 0) return irc;


  // hide bei Solids
  if(resMod == 2)
    GA_view__ (-1L, 1, s_tab[sInd].gTyp, s_tab[sInd].gInd);


  return 0;

}


//===========================================================================
  int STP_r_creCi_0 (int i1, int i2, Point *pc, Vector *vz, int iDir) {
//===========================================================================
// create text "ARC P<i1> P<i2> P(<pc>) D(<vz>) [CW]" generieren ..
// Input:
//   iDir       0=T, 1=F


  printd("STP_r_creCi_0 %d %d %d\n",i1,i2,iDir);

  strcpy(gTxt, "ARC");
  AP_obj_add_obj (gTxt, Typ_PT, i1);
  AP_obj_add_obj (gTxt, Typ_PT, i2);
  AP_obj_add_pt (gTxt, pc);
  AP_obj_add_vc (gTxt, vz);

  if(iDir != 0) strcat(gTxt, " CW");

    // printf("ex STP_r_creCi_0 |%s|\n",gTxt);

  return 0;

}


//===========================================================================
  int STP_r_creCi_1 (Point *p1, Point *p2, Point *pc, Vector *vz, int iDir) {
//===========================================================================
// create text "ARC P(<p1>) P(<p1>) P(<pc>) D(<vz>) [CW]" generieren ..
// Input:
//   iDir       0=T, 1=F



  printd("STP_r_creCi_1 %d\n",iDir);

  strcpy(gTxt, "ARC");
  AP_obj_add_pt (gTxt, p1);
  AP_obj_add_pt (gTxt, p2);
  AP_obj_add_pt (gTxt, pc);
  AP_obj_add_vc (gTxt, vz);

  if(iDir != 0) strcat(gTxt, " CW");

    // printf("ex STP_r_creCi_1 |%s|\n",gTxt);

  return 0;

}


//===========================================================================
  int STP_r_creCi_2 (Point *pc, double radc, Vector *vz, int iDir) {
//===========================================================================
// nur den Text "cen rad dz" generieren ..

    gTxt[0] = '\0';
    AP_obj_add_pt (gTxt, pc);  // cen
    // AP_obj_add_obj (gTxt, s_tab[ipo].gTyp, s_tab[ipo].gInd);  // cen
    AP_obj_add_val (gTxt, radc);
    // add Z-vec
    AP_obj_add_vc (gTxt, vz);

  if(iDir != 0) strcat(gTxt, " CW");

    // printf("ex STP_r_creCi_2 |%s|\n",gTxt);

  return 0;

}



//======================================================================
  int STP_r_creSplTri1 (int iL1, int iL2, int ibc, int iDir, int sInd) {
//======================================================================
// trimmed B_SPLINE_CURVE_WITH_KNOTS

  int    irc, i1, i2, iTyp;
  // double *da;
  Point  *pa, pt1, pt2;


  // printf("=========================================== \n");
  // printf("STP_r_creSplTri1 L1=%d L2=%d cv=%d #%d dir=%d %d #%d\n",iL1,iL2,
         // ibc,s_tab[ibc].sInd,iDir,sInd,s_tab[sInd].sInd);


  irc = STP_r_cre2 (ibc);  // create baseCurve
  if(irc < 0) return irc;

  i1 = STP_r_findInd (iL1, sInd);
  i2 = STP_r_findInd (iL2, sInd);
  // printf(" Trim1: %d #%d=%s\n",i1,iL1,STP_r_TypTxt_sTyp(s_tab[i1].sTyp));
  // printf(" Trim2: %d #%d=%s\n",i2,iL2,STP_r_TypTxt_sTyp(s_tab[i2].sTyp));

  // Punkte holen
  irc = STP_r_PT_sInd (&pt1, i1);    if(irc < 0) goto L_cut;
  irc = STP_r_PT_sInd (&pt2, i2);    if(irc < 0) goto L_cut;


    // printf(" ibc=%d i1=%d i2=%d\n",ibc,i1,i2);


  // check if CUT necessary or useless ..
  pa = s_tab[ibc].gDat;
  if(pa == NULL) goto L_cut;
  // zuerst 2 doubles (v0/v1); skip.
  pa = STP_r_getSkip (pa, sizeof(double)*2);
    // printf(" v0=%f v1=%f\n",da[0],da[1]);
    // DEB_dump_obj__ (Typ_PT, &pa[0], " v0");
    // DEB_dump_obj__ (Typ_PT, &pa[1], " v1");
    // DEB_dump_obj__ (Typ_PT, s_tab[i1].sDat, " p1");
    // DEB_dump_obj__ (Typ_PT, s_tab[i2].sDat, " p2");

  if(UT3D_comp2pt(&pa[0],  &pt1, UT_TOL_cv) == 1) {  // 1=gleich
    if(UT3D_comp2pt(&pa[1],&pt2, UT_TOL_cv) == 1) goto L_skip;
  }
  if(UT3D_comp2pt(&pa[0],  &pt2, UT_TOL_cv) == 1) {
    if(UT3D_comp2pt(&pa[1],&pt1, UT_TOL_cv) == 1) goto L_skip;
  }
  goto L_cut;


  L_skip:
      s_tab[sInd].gTyp = s_tab[ibc].gTyp;
      s_tab[sInd].gInd = s_tab[ibc].gInd;
  return 0;



  //----------------------------------------------------------------
  L_cut:
  STP_r_cre2 (i1);  // recurse !
  STP_r_cre2 (i2);  // recurse !

  strcpy(gTxt, "CUT");
  AP_obj_add_obj (gTxt, s_tab[ibc].gTyp, s_tab[ibc].gInd);
  AP_obj_add_obj (gTxt, Typ_PT, s_tab[i1].gInd);
  AP_obj_add_obj (gTxt, Typ_PT, s_tab[i2].gInd);
  // Achtung: gleicher Typ wie BaseCurve (Spline degrade to Line)
  // iTyp = AP_typ_2_bastyp (s_tab[ibc].gTyp); // Typ_CVBSP -> TYP_CV
  iTyp = Typ_CV;  // 2019-04-20  all trimmed-curves -> S
  irc = STP_r_creObj1 (sInd, iTyp, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  return 0;

}


//======================================================================
  int STP_r_creSplTri2 (double u1, double u2, int ibc, int iDir, int sInd) {
//======================================================================
// trimmed B_SPLINE_CURVE_WITH_KNOTS

  int    irc, i1, i2, iTyp;
  double *da;
  // Point  *pa, pt1;

  // printf("=========================================== \n");
  // printf("STP_r_creSplTri2 %f %f %d %d %d\n",u1,u2,ibc,iDir,sInd);

  irc = STP_r_cre2 (ibc);  // create baseCurve
  if(irc < 0) return irc;

  // get v0/v1 from gCad-object
  da = s_tab[ibc].gDat;
  if(da == NULL) goto L_cut;
    // printf(" v0=%f v1=%f\n",da[0],da[1]);
    // DEB_dump_obj__ (Typ_PT, &pa[0], " v0");
    // DEB_dump_obj__ (Typ_PT, &pa[1], " v1");
    // DEB_dump_obj__ (Typ_PT, s_tab[i1].sDat, " p1");
    // DEB_dump_obj__ (Typ_PT, s_tab[i2].sDat, " p2");

  // check if CUT necessary or useless ..
  // check if intersectionValues are identical with v0,v1
  //   Toleranz hier problematisch; muesste den Bereich und UT_TOL_cv beachten.
  if(UTP_comp2x2db(u1,u2, da[0],da[1], UT_TOL_cv) >= 0) goto L_skip;
  goto L_cut;


  L_skip:
      s_tab[sInd].gTyp = s_tab[ibc].gTyp;
      s_tab[sInd].gInd = s_tab[ibc].gInd;
  return 0;


  //----------------------------------------------------------------
  L_cut:

  strcpy(gTxt, "CUT");
  AP_obj_add_obj (gTxt, s_tab[ibc].gTyp, s_tab[ibc].gInd);
  AP_obj_add_val (gTxt, u1);
  AP_obj_add_val (gTxt, u2);
  // Achtung: gleicher Typ wie BaseCurve (Spline degrade to Line)
  // iTyp = AP_typ_2_bastyp (s_tab[ibc].gTyp); // Typ_CVBSP -> TYP_CV
  iTyp = Typ_CV;  // 2019-04-20  all trimmed-curves -> S
  irc = STP_r_creObj1 (sInd, iTyp, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  return 0;

}


//================================================================
  int STP_r_creEl0 (int si) {
//================================================================
// #5624=ELLIPSE('',#5620,1.38152913798744,0.518073426745292);
// axis, length main, lenght normal

  int     irc, l1, *iap;
  double  d1, d2;
  Vector  vca, vcb;
  Plane   pl1;


  printd("STP_r_creEl0 %d #%d\n",si,s_tab[si].sInd);


  l1 = STP_r_get_L1 (si);

  // copy length
  iap = s_tab[si].sDat;
  ++iap;   // skip 1.link
  iap = STP_r_getDb (&d1, iap);
  iap = STP_r_getDb (&d2, iap);

    printd("  l1=#%d d1=%lf d2=%lf\n",l1,d1,d2);


  STP_r_PLN_AXIS2 (&pl1, i_tab[l1]);  // AXIS --> Plane

  // create EllipsenHauptachsen
  UT3D_vc_multvc (&vca, &pl1.vx, d1);
  UT3D_vc_multvc (&vcb, &pl1.vy, d2);

  // set length
  UT3D_vc_setLength (&vca, &vca, d1);
  UT3D_vc_setLength (&vcb, &vcb, d2);

  strcpy(gTxt, "ELL");
  AP_obj_add_pt (gTxt, &pl1.po);  // origin

  // add vectors
  AP_obj_add_vc0 (gTxt, &vca);
  AP_obj_add_vc0 (gTxt, &vcb);

    // make obj, save-> gCad, stor gC-Typ & Index -> s_tab
  irc = STP_r_creObj1 (si, Typ_CVELL, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  if(resMod == 2)     // hide bei Solids
    // GA_hide__ (0, s_tab[sInd].gInd, s_tab[sInd].gTyp);
    GA_view__ (-1L, 1, s_tab[si].gTyp, s_tab[si].gInd);

  return 0;

}


//================================================================
  int STP_r_creEl1 (int iL1, int iL2, int iDir, int sInd) {
//================================================================
// trim == points
// Begrenzte Ellipse aus Mittelpunkt, Vektor Hauptachse, Vektor Nebenachse,
//   Startpunkt, Endpunkt:
// S1=ELL P(0 0) D(10 0) D(0 5) P(10 0) P(-10 0)

  int     irc, *iap, i1, i2;
  Point   pt1, pt2, pt3;
  Vector  vca, vcb;
  double  d1, d2;
  Plane   pl1;


  printd("STP_r_creEl1 %d %d %d\n",sInd,iL1,iL2);


  // sInd is a ELLIPSE; #RefSys + (db)rMay + (db)rMin
  iap = s_tab[sInd].sDat;
  i1 = STP_r_findInd (iap[0], sInd);

  STP_r_PLN_AXIS2 (&pl1, i1);  // AXIS --> Plane
    // STP_r_dispSym1 (52, &pl1.po);
  iap = STP_r_getDb (&d1, &iap[1]);
  iap = STP_r_getDb (&d2, iap);
    // printf(" d1=%f d2=%f\n",d1,d2);

  // create EllipsenHauptachsen
  UT3D_vc_multvc (&vca, &pl1.vx, d1);
  UT3D_vc_multvc (&vcb, &pl1.vy, d2);


  // create the points
  i1 = STP_r_findInd (iL1, 0);
  STP_r_cre2 (i1);

  i2 = STP_r_findInd (iL2, 0);
  STP_r_cre2 (i2);


  if(iDir != 0) MEM_swap_int (&i1, &i2);


  strcpy(gTxt, "ELL");
  AP_obj_add_pt (gTxt, &pl1.po);  // origin

  // add Hauptachsen (NICHT AP_obj_add_vc - Laenge muss erhalten bleiben !)
  AP_obj_add_vc0 (gTxt, &vca);
  AP_obj_add_vc0 (gTxt, &vcb);

  // add start / endPoints
  AP_obj_add_obj (gTxt, Typ_PT, s_tab[i1].gInd);
  AP_obj_add_obj (gTxt, Typ_PT, s_tab[i2].gInd);

  // make obj, save-> gCad, stor gC-Typ & Index -> s_tab
  irc = STP_r_creObj1 (sInd, Typ_CVELL, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  if(resMod == 2)     // hide bei Solids
    // GA_hide__ (0, s_tab[sInd].gInd, s_tab[sInd].gTyp);
    GA_view__ (-1L, 1, s_tab[sInd].gTyp, s_tab[sInd].gInd);

  return 0;

}


//================================================================
  int STP_r_creEl2 (double u1, double u2, int iDir, int sInd) {
//================================================================
// create Ellipse; trim == doubles

  int     irc, *iap, i1, i2;
  Point   pt2, pt3;
  Vector  vca, vcb;
  double  d1, d2;
  Plane   pl1;


  printd("STP_r_creEl2 %d %f %f\n",sInd,u1,u2);


  // sInd is a ELLIPSE; #RefSys + (db)rMay + (db)rMin
  iap = s_tab[sInd].sDat;
  i1 = STP_r_findInd (iap[0], sInd);

  STP_r_PLN_AXIS2 (&pl1, i1);  // AXIS --> Plane
    // STP_r_dispSym1 (52, &pl1.po);
  iap = STP_r_getDb (&d1, &iap[1]);
  iap = STP_r_getDb (&d2, iap);
    // printf(" d1=%f d2=%f\n",d1,d2);

  strcpy(gTxt, "ELL");
  AP_obj_add_pt (gTxt, &pl1.po);

/*
  // TEST; volle Ellipse. (ELL,cen,P(may), P(min))
  // pt1 = from center(pl1.po) in Richtg X (pl1.vx) len = rdc
  UT3D_pt_traptvclen (&pt1, &pl1.po, &pl1.vx, d1);
  UT3D_pt_traptvclen (&pt2, &pl1.po, &pl1.vy, d2);
  AP_obj_add_pt (gTxt, &pt1);
  AP_obj_add_pt (gTxt, &pt2);
*/


  // UT3D_vc_setLength
  UT3D_vc_multvc (&vca, &pl1.vx, d1);
  UT3D_vc_multvc (&vcb, &pl1.vy, d2);
  

  // add Hauptachsen (NICHT AP_obj_add_vc - Laenge muss erhalten bleiben !)
  AP_obj_add_vc0 (gTxt, &vca);
  AP_obj_add_vc0 (gTxt, &vcb);

  // pt2 = pt1 um pl1.po/pl1.vz drehen um Winkel = u1
  UT3D_pt_elangd (&pt2, &pl1.po, &vca, &vcb, UT_RADIANS(u1));

  // pt3 = pt1 um pl1.po/pl1.vz drehen um Winkel = u2
  UT3D_pt_elangd (&pt3, &pl1.po, &vca, &vcb, UT_RADIANS(u2));

  AP_obj_add_pt (gTxt, &pt2);
  AP_obj_add_pt (gTxt, &pt3);

  // make obj, save-> gCad, stor gC-Typ & Index -> s_tab
  irc = STP_r_creObj1 (sInd, Typ_CVELL, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  if(resMod == 2)     // hide bei Solids
    // GA_hide__ (0, s_tab[sInd].gInd, s_tab[sInd].gTyp);
    GA_view__ (-1L, 1, s_tab[sInd].gTyp, s_tab[sInd].gInd);

  return 0;

}


//================================================================
  int STP_r_crePlg (int *iap, int sInd) {
//================================================================
// iap ist data of CCV; all segs are Lines - create Polygon.
// get all trimPoints of 
// COMPOSITE_CURVE
//   COMPOSITE_CURVE_SEGMENT
//     TRIMMED_CURVE
//       LINE

  int    irc, iNr, i1, is, ii1, ii2, ii3, iTyp, *iTab, lcvb, icvb, iDir;
  void   *vp1;
  double d1, d2;
  // Point  pt1;
  Line   ln1;


  // printf("==========================\n");
  printd("STP_r_crePlg %d %d\n",*iap,sInd);

  iNr = *iap;
  ++iap;

  is = sInd;
  gTxt[0] = '\0';


  for(i1=0; i1<iNr; ++i1) {

    // decode COMPOSITE_CURVE
    // iap is a group of COMPOSITE_CURVE_SEGMENT's
    ii1 = STP_r_findInd (iap[i1], is);
    // printf(" res CCV-obj Nr.%d - ind=%d typ=%d\n",i1,ii1,s_tab[ii1].sTyp);
    if(s_tab[ii1].sTyp != SC_COMPOSITE_CURVE_SEGMENT) {
      TX_Error("STP_r_crePlg E001 %d",s_tab[ii1].sTyp);
      goto L_Err;
    }

    // decode COMPOSITE_CURVE_SEGMENT; first Link = TRIMMED_CURVE
    is = STP_r_Link_data (0, s_tab[ii1].sDat); // get 1. link of CC = TC
    ii2 = STP_r_findInd (is, ii1);             // ii2 = index of TC
      printd(" res CCV-obj Nr.%d - ind=%d typ=%d\n",i1,ii2,s_tab[ii2].sTyp);
    if(s_tab[ii2].sTyp != SC_TRIMMED_CURVE) {
      TX_Error("STP_r_crePlg E002 %d",s_tab[ii1].sTyp);
      goto L_Err;
    }

    // get Typ of trimObjects for TRIMMED_CURVE;
    iTyp = STP_r_Link_data (1, s_tab[ii2].sDat); // 2. = typ of trimObj's
      printd(" TC-iTyp=%d\n",iTyp);

    if(iTyp == 0) goto L_iTyp_0;

    //----------------------------------------------------------------
    // iTyp=1 (double)
    lcvb = STP_r_Link_data (0, s_tab[ii2].sDat); // TC: 1. = Link to baseCurv
    icvb = STP_r_findInd (lcvb, 0);              // index of basecurve
      printd(" typCvb=%d\n",s_tab[icvb].sTyp);

    if(s_tab[icvb].sTyp != SC_LINE) {
      TX_Error("STP_r_crePlg E005 %d",s_tab[icvb].sTyp);
      goto L_Err;
    }


    // get 2 parameters
    vp1 = s_tab[ii2].sDat;
    // vp1 += sizeof(int);     // skip 1. Link (int)
    // vp1 += sizeof(int);     // skip type of trimObj 1
    vp1 = (char*)vp1 + (2 * sizeof(int));
    
    vp1 = STP_r_getDb (&d1, vp1);
      printd(" d1=%lf\n",d1);
    STP_r_getDb (&d2, vp1);
      printd(" d2=%lf\n",d2);
    // d1, d2 are now the parametres to line

    // get data of basicCurve (line) lcvb
      printd(" lcvb=%d icvb=%d\n",lcvb,icvb);

    // get the startpoint of the line icvb
    iTab = s_tab[icvb].sDat;
    ii3 = STP_r_findInd (iTab[0], 0);  // PT

    // add the startpoint to the polygon, then continue ..
    // create point if not yet done
    if(STP_r_crePt0 (ii3) < 0) return -1;
    // add point --> gTxt
    AP_obj_add_obj (gTxt, Typ_PT, s_tab[ii3].gInd);

    continue;



    //----------------------------------------------------------------
    // iTyp=0 (Link to point) ...
    L_iTyp_0:
    // is TRIMMED_CURVE == LINE; 3. u 4. Link sind die TrimmingObjects
    is = STP_r_Link_data (2, s_tab[ii2].sDat); // get 3. link of TC
    ii3 = STP_r_findInd (is, ii2);
    // printf(" res CCV-obj Nr.%d - ind=%d typ=%d\n",i1,ii3,s_tab[ii2].sTyp);
    if(s_tab[ii3].sTyp != SC_CARTESIAN_POINT) {
      TX_Error("STP_r_crePlg E003 %d",s_tab[ii3].sTyp);
      goto L_Err;
    }

    // wenn Punkt noch nicht angelegt: anlegen !
    if(STP_r_crePt0 (ii3) < 0) return -1;
    // add point --> gTxt
    AP_obj_add_obj (gTxt, Typ_PT, s_tab[ii3].gInd);

  }


  //----------------------------------------------------------------
  // add the last point to the polygon
  if(iTyp == 0) {
    // typ=0: trimmed by points.
    is = STP_r_Link_data (3, s_tab[ii2].sDat); // get 4. Link of TC (endPt)
    ii3 = STP_r_findInd (is, ii2);
    // create point if not yet done
    if(STP_r_crePt0 (ii3) < 0) return -1;
    ii1 = s_tab[ii3].gInd;  


  } else {
    // iTyp=1: trimmed by doubles - PARAMETER_VALUE
    // get endpoint of LINE icvb; see STP_r_creLn2
    // get Line from LINE
    STP_r_LN_LINE (&ln1, icvb);
    // get parametric point on Line
    // ...
    // create new point pt1
    ii1 = STP_r_creObj1 (-1, Typ_PT, Typ_PT, &ln1.p2);
  }

  // add last point --> gTxt
  AP_obj_add_obj (gTxt, Typ_PT, ii1);


  // create polygon
  if(iNr > 1) {
    irc = STP_r_creObj1 (sInd, Typ_CVPOL, Typ_Txt, gTxt);
  } else {
    irc = STP_r_creObj1 (sInd, Typ_LN, Typ_Txt, gTxt);
  }
  if(irc < 0) return irc;


  if(resMod == 2)     // hide bei Solids
    GA_view__ (-1L, 1, s_tab[sInd].gTyp, s_tab[sInd].gInd);


  return 0;

  L_Err:
  return -1;

}


//================================================================
  int STP_r_creCCV_ckl (int *iap) {
//================================================================
// test if all segments are TRIMMED_CURVE/LINE
// Retcod 0 = yes, all segments are TRIMMED_CURVE/LINE
// Retcod 1 = no;

  int  iNr, i1, is, ii1, ii2;

  // printf(">>>>>>>>>>> STP_r_creCCV_ckl %d\n",*iap);

  iNr = *iap;
  ++iap;

  is = 0;

  for(i1=0; i1<iNr; ++i1) {

    // decode COMPOSITE_CURVE
    ii1 = STP_r_findInd (iap[i1], is);
    // printf(" res CCV-obj Nr.%d - ind=%d typ=%d\n",i1,ii1,s_tab[ii1].sTyp);
    if(s_tab[ii1].sTyp != SC_COMPOSITE_CURVE_SEGMENT) {
      TX_Error("STP_r_creCCV_ckl E001 %d",s_tab[ii1].sTyp);
      goto L_NO;
    }

    // decode COMPOSITE_CURVE_SEGMENT
    is = STP_r_Link_data (0, s_tab[ii1].sDat); // get first link of CCS
    ii2 = STP_r_findInd (is, ii1);
    // printf(" res CCV-obj Nr.%d - ind=%d typ=%d\n",i1,ii2,s_tab[ii2].sTyp);
    if(s_tab[ii2].sTyp != SC_TRIMMED_CURVE) goto L_NO;

    // is TRIMMED_CURVE; 1. Link == basis_curve - must be LINE
    is = STP_r_Link_data (0, s_tab[ii2].sDat); // get first link of CCS
    ii2 = STP_r_findInd (is, ii1);
    // printf(" res CCV-obj Nr.%d - ind=%d typ=%d\n",i1,ii2,s_tab[ii2].sTyp);
    if(s_tab[ii2].sTyp != SC_LINE) goto L_NO;

  }
 
  return 0;

  L_NO:
  return 1;

}


//================================================================
  int STP_r_creCurv1 (int sInd) {
//================================================================
// create trimmed Curve  from ORIENTED_EDGE, EDGE_CURVE

  int     irc, i1, i2, ii, iNr, iDir, *iap;

  // printf("=============================================== \n");
  printd(" STP_r_creCurv1 %d #%d typ=%d\n",sInd,
           s_tab[sInd].sInd,s_tab[sInd].sTyp);


  //----------------------------------------------------------------
  // ORIENTED_EDGE: get EDGE_CURVE.
  if(s_tab[sInd].sTyp == SC_ORIENTED_EDGE) {

    iap = s_tab[sInd].sDat;
    ii = STP_r_findInd (iap[0], sInd-3);
      printd(" Curv1 ind=%d #%d typ=%d\n",ii,s_tab[ii].sInd,s_tab[ii].sTyp);


  //----------------------------------------------------------------
  // EDGE_CURVE.
  } else if(s_tab[sInd].sTyp == SC_EDGE_CURVE) {
    ii = sInd;


  //----------------------------------------------------------------
  } else {
    TX_Error("STP_r_creCurv1 E001 %d #%d",sInd,s_tab[sInd].sInd);
    return -1;
  }



  //----------------------------------------------------------------
  // handle EDGE_CURVE ii.
  if(s_tab[ii].sTyp != SC_EDGE_CURVE) {
    TX_Error("STP_r_creCurv1 E002 %d",sInd);
    return -1;
  }


  // EDGE_CURVE ist eine begrenzte Curve;
  iap = s_tab[ii].sDat;
  // die Links zu den Trimpoints
  i1 = iap[0];
  i2 = iap[1];
  // get ii = index of BaseCurve
  ii = STP_r_findInd (iap[2], i2);
  // get direction (logical); 0=T, 1=F
  iDir = iap[3];



  L_start1:

/*
    printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
    printf(" Trimpoint1=#%d #%d\n",i1,s_tab[i1].sInd);
    printf(" Trimpoint2=#%d #%d\n",i2,s_tab[i2].sInd);
    printf(" ind curv=%d #%d=%s iDir=%d\n",ii, s_tab[ii].sInd,
           STP_r_TypTxt_sTyp(s_tab[ii].sTyp),iDir);
    printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \n");
*/




  //----------------------------------------------------------------
  // LINE
  if(s_tab[ii].sTyp == SC_LINE) {

    irc = STP_r_creLn1 (i1, i2, iDir, ii);
    if(irc < 0) return irc;



  //----------------------------------------------------------------
  // POLYGON   als Line abspeichern ..
  } else if(s_tab[ii].sTyp == SC_QUASI_UNIFORM_CURVE) {

    irc = STP_r_creLn1 (i1, i2, iDir, ii);
    if(irc < 0) return irc;



  //----------------------------------------------------------------
  // CIRCLE
  } else if(s_tab[ii].sTyp == SC_CIRCLE) {

    irc = STP_r_creCi1 (i1, i2, iDir, ii);
    if(irc < 0) return irc;



  //----------------------------------------------------------------
  // ELLIPSE
  } else if(s_tab[ii].sTyp == SC_ELLIPSE) {

    irc = STP_r_creEl1 (i1, i2, iDir, ii);
    if(irc < 0) return irc;



  //----------------------------------------------------------------
  // B_SPLINE
  } else if(s_tab[ii].sTyp == SC_B_SPLINE_CURVE_WITH_KNOTS) {

    irc = STP_r_creSplTri1 (i1, i2, ii, iDir, sInd);
    if(irc < 0) return irc;
    return 0;   // .gTyp & .gInd already set.



  //----------------------------------------------------------------
  // Rat.B_SPLINE
  } else if(s_tab[ii].sTyp == SC_BOUNDED_CURVE) {

    irc = STP_r_creSplTri1 (i1, i2, ii, iDir, sInd);
    if(irc < 0) return irc;
    return 0;   // .gTyp & .gInd already set.



  //----------------------------------------------------------------
  // INTERSECTION_CURVE
  } else if(s_tab[ii].sTyp == SC_INTERSECTION_CURVE) {
      // printf(" ..SC_INTERSECTION_CURVE ind=%d\n",ii);
    // irc = STP_r_findInd (ii, sInd-3);
      // printf(" ind=%d\n",irc);
    iap = s_tab[ii].sDat;
    // Link zur basecurve
    ii = iap[0];
      // printf(" basecurve-step-ind=#%d\n",ii);
    // search index of step-record ii
    ii = STP_r_findInd (ii, 0);
      // printf(" basecurve-ind=#%d\n",ii);
    goto L_start1;

  



  //----------------------------------------------------------------
  } else {
    TX_Error("STP_r_creCurv1 E003 %d %d %d", sInd, ii, s_tab[ii].sTyp);
    printf("#%d = %s\n",s_tab[sInd].sInd,STP_r_TypTxt_sTyp(s_tab[sInd].sTyp));
    printf("#%d = %s\n",s_tab[ii].sInd,STP_r_TypTxt_sTyp(s_tab[ii].sTyp));
    return -1;
  }


  // copy gTyp u gInd from EDGE_CURVE --> ORIENTED_EDGE
  s_tab[sInd].gTyp = s_tab[ii].gTyp;
  s_tab[sInd].gInd = s_tab[ii].gInd;

  return 0;

}


//================================================================
  STP_r_creCurv2 (int sInd) {
//================================================================
// create trimmed Curve  from
// TRIMMED_CURVE (#basis_curve, #trimObj1, #trimObj2)


  int     irc, ii, *iap, iTyp, i1, i2, iDir;
  double  d1, d2;


  // printf("=============================================== \n");
  printd(" STP_r_creCurv2 %d #%d typ=%d\n",sInd,
           s_tab[sInd].sInd,s_tab[sInd].sTyp);


  iap = s_tab[sInd].sDat;

  // 1.Link = basis_curve
  // wenn basis_curve== Line: nutzlos, Line direkt via ihre 2 TrimPoints
  // ausgeben.
  ii = STP_r_findInd (iap[0], sInd);
    // printf(" basis_curve #%d=%s (%d)\n",s_tab[ii].sInd,
      // STP_r_TypTxt_sTyp(s_tab[ii].sTyp),ii);
  ++iap;

  // next int ist der Typ der trimObj's
  iTyp = iap[0];         // printf(" trimTyp=%d\n",iTyp);
  ++iap;

  // 2. u 3. Link sind die TrimmingObjects; Links auf Points od doubles.
  if(iTyp == 0) {   // Links
    iap = STP_r_getInt (&i1, iap);
    iap = STP_r_getInt (&i2, iap);
      // printf(" typ=%d Links %d %d\n",iTyp,i1,i2);

  } else {    // doubles
    iap = STP_r_getDb (&d1, iap);
    iap = STP_r_getDb (&d2, iap);
      // printf(" typ=%d vals %f %f\n",iTyp,d1,d2);
  }

  iap = STP_r_getInt (&iDir, iap);


  //................................
  if(s_tab[ii].sTyp == SC_LINE) {
    if(iTyp == 0) {   // trim == points
      irc = STP_r_creLn1 (i1, i2, iDir, ii);
      if(irc < 0) return irc;

    } else { // trim == doubles
      irc = STP_r_creLn2 (d1, d2, iDir, ii);
      if(irc < 0) return irc;
    }


  //................................
  } else if(s_tab[ii].sTyp == SC_CIRCLE) {
    // dzt neuanlegen Circ; koennte auch mit C=CUT C gehen !
    if(iTyp == 0) {   // trim == points
      irc = STP_r_creCi1 (i1, i2, iDir, ii);
      if(irc < 0) return irc;

    } else { // trim == doubles
      STP_r_creCi2 (d1, d2, iDir, ii);
    }


  //................................
  } else if(s_tab[ii].sTyp == SC_ELLIPSE) {
    if(iTyp == 0) {   // trim == points
      STP_r_creEl1 (i1, i2, iDir, ii);

    } else { // trim == doubles
      STP_r_creEl2 (d1, d2, iDir, ii);
    }


  //................................
  } else if(s_tab[ii].sTyp == SC_B_SPLINE_CURVE_WITH_KNOTS) {
    if(iTyp == 0) {   // trim == points
      irc = STP_r_creSplTri1 (i1, i2, ii, iDir, sInd);
      if(irc < 0) return irc;


    } else { // trim == doubles
      irc = STP_r_creSplTri2 (d1, d2, ii, iDir, sInd);
      if(irc < 0) return irc;

    }



  //................................
  } else if(s_tab[ii].sTyp == SC_BOUNDED_CURVE) {
    // muss noch gemacht werden ...
    printf("********* STP_r_creCurv2 I001 *********** \n");
    // TX_Print("STP_r_creCurv2 I001");
    


  //................................
  } else {
    TX_Error("STP_r_creCurv2 %d #%d",s_tab[ii].sTyp,s_tab[sInd].sInd);
    return -1; //exit(0);
  }


  // gTyp/gInd der TrimmedCurve(sInd) = gTyp/gInd der BaseCurve(ii)
  s_tab[sInd].gTyp = s_tab[ii].gTyp;
  s_tab[sInd].gInd = s_tab[ii].gInd;
  // break;

  return 0;

}
 

//==========================================================================
  int STP_r_savCUT1 (int sInd, double v0, double v1, Point *p1, Point *p2) {
//==========================================================================
// es werden nun v0, v1 und die 2 Points noch gespeichert !!!
// v0/v1 noch speichern (fuer CUT-necessary-test)
// reserv space for 2 doubles ...

  int       irc;
  double    *da;
  Point     *pa;
 

  // printf("STP_r_savCUT1 %d\n",sInd);


  // v0, v1 und die 2 Points speichern ..
  s_tab[sInd].gDat = s_dat.next;

  da = s_dat.next;
  irc = UME_add (&s_dat, sizeof(double)*2);
  if(irc < 0) {TX_Error("STP_r_savCUT1 EOM1"); return -4;}
  da[0] = v0;
  da[1] = v1;


  // reserv space for 2 points ...
  pa = s_dat.next;
  irc = UME_add (&s_dat, sizeof(Point)*2);
  if(irc < 0) {TX_Error("STP_r_savCUT1 EOM2"); return -4;}
  pa[0] = *p1;
  pa[1] = *p2;

  return 0;

}
      

//================================================================
  int STP_r_creSpl1 (int sInd) {
//================================================================

  int       irc, i1, i2, i3, iNr, lpt, ipt, ii, *iap, *ia1;
  double    d1, *da;
  Point     *pa, pt1, pt2;
  CurvBSpl  cv1;


  // printf("=============================================\n");
  // printf("STP_r_creSpl1 %d #%d\n",sInd,s_tab[sInd].sInd);

  iap = s_tab[sInd].sDat;

  cv1.deg = iap[0];   // (int) Degree
  ++iap;

  cv1.ptNr = iap[0];  // (int) Nr of Controlpoints
  ++iap;
    // printf(" deg=%d ptNr=%d\n",cv1.deg,cv1.ptNr);


  // Degree 1: make Line.
  if(cv1.deg == 1) {
    if(cv1.ptNr != 2) {
      // UT3D_bsp_degrad
      gTxt[0] = '\0';  // strcpy(gTxt, "POL");
      ipt = sInd;
      for(i1=0; i1<cv1.ptNr; ++i1) {
        iap = STP_r_getInt (&lpt, iap);
        ipt = STP_r_findInd (lpt, ipt);
        STP_r_PT_CARTPT (&pt2, ipt);  // copy point
        AP_obj_add_pt (gTxt, &pt2);
        if(i1 == 0) pt1 = pt2;
      }
      irc = STP_r_creObj1 (sInd, Typ_CVPOL, Typ_Txt, gTxt);
      if(irc < 0) return irc;
      return STP_r_savCUT1 (sInd, 0., 1., &pt1, &pt2);

    } else {
      // create Line
      iap = STP_r_getInt (&i1, iap);
      i1 = STP_r_findInd (i1, sInd);
      iap = STP_r_getInt (&i2, iap);
      i2 = STP_r_findInd (i2, sInd);
      STP_r_cre2 (i1);
      STP_r_cre2 (i2);
      STP_r_creLn3 (i1, i2, 0, sInd);
      // Punkte holen
      STP_r_PT_CARTPT (&pt1, i1);
      STP_r_PT_CARTPT (&pt2, i2);
      return STP_r_savCUT1 (sInd, 0., 1., &pt1, &pt2);
    }
  }


  // Tempspace
  UME_init (&s_mSpc, memspc501, sizeof(memspc501));


  // reserve ptNr points in s_mSpc
  cv1.cpTab = s_mSpc.next;
  irc = UME_add (&s_mSpc, sizeof(Point) * cv1.ptNr);
  if(irc < 0) {TX_Error("STP_r_creSpl1 EOM1"); return -4;}


  // TEST ONLY:
  // for(i1=0;i1<cv1.ptNr; ++i1) printf(" lpt[%d] = #%d\n",i1,iap[i1]);





  ipt = sInd;
  for(i1=0; i1<cv1.ptNr; ++i1) {
    // iap = STP_r_getInt (&lpt, iap);
      // printf(" %d lpt=%d ipt=%d\n",i1,lpt,ipt);
    ipt = STP_r_findInd (iap[i1], ipt);
      // printf(" ipt[%d] ipt=%d #%d\n",i1,ipt,s_tab[ipt].sInd);
    STP_r_PT_CARTPT (&cv1.cpTab[i1], ipt);  // copy point
      // printf(" cpTab[%d] = %lf %lf %lf\n",i1,
             // cv1.cpTab[i1].x,cv1.cpTab[i1].y,cv1.cpTab[i1].z);
  }

  iap = &iap[cv1.ptNr];



  // get nr of ints 
  iap = STP_r_getInt (&iNr, iap);
    // printf(" nr_of_ints=%d\n",iNr);


  ia1 = &iap[iNr]; // startaddres knotvaltab

  i1 = ia1[0];
  ++ia1;
  if(i1 != iNr) TX_Print("STP_r_creSpl1 I001");


  // nr of knotvalues must be ptNr+deg+1
  i1 = cv1.ptNr + cv1.deg + 1;
    // printf(" nr_knotvals = %d\n",i1);


  // reserve ptNr doubles in s_mSpc
  cv1.kvTab = s_mSpc.next;
  irc = UME_add (&s_mSpc, sizeof(double) * i1);
  if(irc < 0) {TX_Error("STP_r_creSpl1 EOM2"); return -4;}


  // load and copy knotvalues
  i3 = 0;
  for(i1=0; i1<iNr; ++i1) {
    iap = STP_r_getInt (&ii, iap); // get next multiplicity
    ia1 = STP_r_getDb (&d1, ia1);  // get next knotval
      // printf(" ii=%d knotval=%lf\n",ii,d1);
    for(i2=0; i2<ii; ++i2) {
      // copy knotvalue -->cv1.kvTab[i1]
      cv1.kvTab[i3] = d1;
        // printf(" kvTab[%d] = %lf\n",i3,d1);
      ++i3;
    }
  }

  cv1.v0 = cv1.kvTab[0];
  cv1.v1 = cv1.kvTab[i3-1];
    // DEB_dump_obj__ (Typ_CVBSP, &cv1, "");



  // es werden nun v0, v1 und die 2 Points noch gespeichert !!!
  // v0/v1 noch speichern (fuer CUT-necessary-test)
  // reserv space for 2 doubles ...
  s_tab[sInd].gDat = s_dat.next;

  da = s_dat.next;
  irc = UME_add (&s_dat, sizeof(double)*2);
  if(irc < 0) {TX_Error("STP_r_creSpl1 EOM3"); return -4;}
  da[0] = cv1.v0;
  da[1] = cv1.v1;

  // reserv space for 2 points ...
  pa = s_dat.next;
  irc = UME_add (&s_dat, sizeof(Point)*2);
  if(irc < 0) {TX_Error("STP_r_creSpl1 EOM4"); return -4;}
  UT3D_pt_evalparCv (&pa[0], &cv1, cv1.v0);
  UT3D_pt_evalparCv (&pa[1], &cv1, cv1.v1);
    // DEB_dump_obj__ (Typ_CVBSP, &cv1, "");


  irc = STP_r_creObj1 (sInd, Typ_CVBSP, Typ_CVBSP, &cv1);
  if(irc < 0) return irc;

  // return -4;

  return 0;

}


//================================================================
  int STP_r_creSplBC (int sInd) {
//================================================================
// BOUNDED_CURVE-Record hat keine eigene Daten; erst die nachfolgenden Records ..
// see STP_r_creSpl1


  int       irc, i1, i2, i3, iNr, lpt, ipt, ii, *iap, *ia1;
  int       ir1, ir2, ir3, *ia;
  double    d1, *da;
  Point     *pa;
  CurvRBSpl cv1;
  ObjGX     o1;


  // printf("=============================================\n");
  // printf("STP_r_creSplBC %d #%d\n",sInd,s_tab[sInd].sInd);

  ia = s_tab[sInd].sDat;

  // ir1 = index of SC_B_SPLINE_CURVE suchen
  // ir2 = index of B_SPLINE_CURVE_WITH_KNOTS suchen
  // ir3 = index of RATIONAL_B_SPLINE_CURVE suchen
  for(i1=0;i1<3;++i1) {
    ir1 = ia[i1];
    if(s_tab[ir1].sTyp == SC_B_SPLINE_CURVE) goto L_ir2;
  }
  TX_Error("STP_r_creSplBC E0I1"); return -1;

  L_ir2:
  for(i1=0;i1<3;++i1) {
    ir2 = ia[i1];
    if(s_tab[ir2].sTyp == SC_B_SPLINE_CURVE_WITH_KNOTS) goto L_ir3;
  }
  ir2 = -1;  // es gibt keine knots.

  L_ir3:
  for(i1=0;i1<3;++i1) {
    ir3 = ia[i1];
    if(s_tab[ir3].sTyp == SC_RATIONAL_B_SPLINE_CURVE) goto L_ir_;
  }
  TX_Error("STP_r_creSplBC E0I3"); return -1;


  L_ir_:
  // printf(" ir1=%d ir2=%d ir3=%d\n",ir1,ir2,ir3);




  //================================================================
  // get from B_SPLINE_CURVE-Record: Degree und ControlPoints

  iap = s_tab[ir1].sDat;
  cv1.deg = iap[0];   // (int) Degree
  ++iap;
  cv1.ptNr = iap[0];  // (int) Nr of Controlpoints
  ++iap;

  // check for Line: see STP_r_creSpl1

  // Tempspace
  UME_init (&s_mSpc, memspc501, sizeof(memspc501));

  // reserve ptNr points in s_mSpc
  cv1.cpTab = s_mSpc.next;
  irc = UME_add (&s_mSpc, sizeof(Point) * cv1.ptNr);
  if(irc < 0) {TX_Error("STP_r_creSplBC EOM1"); return -4;}

  ipt = ir1;
  for(i1=0; i1<cv1.ptNr; ++i1) {
    iap = STP_r_getInt (&lpt, iap);
    ipt = STP_r_findInd (lpt, ipt);
      // printf(" %d lpt=%d ipt=%d\n",i1,lpt,ipt);
    STP_r_PT_CARTPT (&cv1.cpTab[i1], ipt);  // copy point
  }






  //================================================================
  // B_SPLINE_CURVE_WITH_KNOTS ex STP_r_decSpl3
  if(ir2 < 0) {     // .. keine knots; zB .CIRCULAR_ARC.
    // es gibt keine knots;
    // create & copy knotvektor
    irc = UT3D_knotvec_m (&cv1.kvTab, &cv1.v0, &cv1.v1,
                          cv1.ptNr, cv1.deg, &s_mSpc);
    if(irc < 0) return irc;
    goto L_weights;
  }


  // get nr of ints
  iap = s_tab[ir2].sDat;
  iap = STP_r_getInt (&iNr, iap);

  ia1 = &iap[iNr]; // startaddres knotvaltab
  i1 = ia1[0];     // Anzahl knotValues
  ++ia1;
  if(i1 != iNr) TX_Print("STP_r_creSplBC I001");


  // nr of knotvalues must be ptNr+deg+1
  i1 = cv1.ptNr + cv1.deg + 1;


  // reserve ptNr doubles in s_mSpc
  cv1.kvTab = s_mSpc.next;
  irc = UME_add (&s_mSpc, sizeof(double) * i1);
  if(irc < 0) {TX_Error("STP_r_creSplBC EOM1"); return -4;}


  // load and copy knotvalues
  i3 = 0;
  for(i1=0; i1<iNr; ++i1) {
    iap = STP_r_getInt (&ii, iap); // get next multiplicity
    ia1 = STP_r_getDb (&d1, ia1);  // get next knotval
    for(i2=0; i2<ii; ++i2) {
      // copy knotvalue -->cv1.kvTab[i1]
      cv1.kvTab[i3] = d1;
      ++i3;
    }
  }

  cv1.v0 = cv1.kvTab[0];
  cv1.v1 = cv1.kvTab[i3-1];





  //================================================================
  // RATIONAL_B_SPLINE_CURVE ex STP_r_decSpl4
  L_weights:
  // get from RATIONAL_B_SPLINE_CURVE-Record: weights-data

  // get nr of weigths
  iap = s_tab[ir3].sDat;
  iap = STP_r_getInt (&iNr, iap);
  if(iNr != cv1.ptNr) TX_Print("STP_r_creSplBC I002");

  // reserve ptNr doubles in s_mSpc
  cv1.wTab = s_mSpc.next;
  irc = UME_add (&s_mSpc, sizeof(double) * iNr);
  if(irc < 0) {TX_Error("STP_r_creSplBC EOM2"); return -4;}

  // copy weigths
  for(i1=0; i1<iNr; ++i1) {
    iap = STP_r_getDb (&d1, iap);
      // printf(" %d lpt=%d ipt=%d\n",i1,lpt,ipt);
    cv1.wTab[i1] = d1;
  }





  //================================================================
  // v0/v1 noch speichern (fuer CUT-necessary-test)
  // reserv space for 2 doubles ...
  s_tab[sInd].gDat = s_dat.next;
  da = s_dat.next;
  irc = UME_add (&s_dat, sizeof(double)*2);
  if(irc < 0) {TX_Error("STP_r_creSplBC EOM3"); return -4;}
  da[0] = cv1.v0;
  da[1] = cv1.v1;
  // reserv space for 2 points ...
  pa = s_dat.next;
  irc = UME_add (&s_dat, sizeof(Point)*2);
  if(irc < 0) {TX_Error("STP_r_creSplBC EOM4"); return -4;}
  UT3D_pt_evparCrvRBSpl (&pa[0], &d1, &cv1, cv1.v0);
  UT3D_pt_evparCrvRBSpl (&pa[1], &d1, &cv1, cv1.v1);



  // fertig, ausgeben ..
  // DEB_dump_obj__ (Typ_CVRBSP, &cv1, "&cv1:");


  //================================================================
  // testen, ob man Rat.B-Spline degraden kann
  irc = UT3D_obj_cnvt_rbspl (&o1, &cv1, &s_mSpc);
  if(irc >= 0) {
    irc = STP_r_creObj1 (sInd, o1.typ, o1.form, o1.data);

  } else {
    irc = STP_r_creObj1 (sInd, Typ_CVRBSP, Typ_CVRBSP, &cv1);
  }

  if(irc < 0) return irc;


  return 0;

}


//================================================================
  int STP_r_creCont1 (int sInd) {
//================================================================
// create CCV  from EDGE_LOOP from FACE_OUTER_BOUND
// create CCV  from EDGE_LOOP from FACE_BOUND
// create ?    from VERTEX_LOOP from FACE_BOUND
//   sInd muss FACE_OUTER_BOUND od FACE_BOUND sein

  int     irc, i1, ii, iNr, *iap;

  // printf("=============================================== \n");
  printd("STP_r_creCont1 %d #%d typ=%d\n",sInd,
          s_tab[sInd].sInd,s_tab[sInd].sTyp);


  if((s_tab[sInd].sTyp != SC_FACE_OUTER_BOUND) &&
     (s_tab[sInd].sTyp != SC_FACE_BOUND))         {
    TX_Error("STP_r_creCont1 E001 %d",sInd);
    return -1;
  }

  // get EDGE_LOOP from FACE_OUTER_BOUND
  iap = s_tab[sInd].sDat;
  ii = STP_r_findInd (iap[0], 0);
  // printf(" Cont2 ind=%d #%d typ=%d\n",ii,s_tab[ii].sInd,s_tab[ii].sTyp);


  if(s_tab[ii].sTyp == SC_VERTEX_LOOP)     {   // via VERTEX_LOOP
    // VERTEX_LOOP has 1 point only
    ii = STP_r_findInd (iap[0], ii);
    // create this point
    i1 = STP_r_cre2 (ii);
    if(i1 < 0) {
      TX_Error("STP_r_creCont1 E002 %d",ii);
      return -1;
    }
    // copy gTyp u gInd from CART_P --> VERT_P
      printd(" copy cont %d -> %d\n",ii,sInd);
    s_tab[sInd].gTyp = s_tab[ii].gTyp;
    s_tab[sInd].gInd = s_tab[ii].gInd;
    return 0;
    // TX_Error("STP_r_creCont1 E002 %d",ii);
    // return -1;
  }


  if(s_tab[ii].sTyp != SC_EDGE_LOOP)     {   // via VERTEX_LOOP
    TX_Error("STP_r_creCont1 E003 %d",ii);
    return -1;
  }

  // die EDGE_LOOP enthaelt 2-n ORIENTED_EDGEs (trimmedCurves)
  iap = s_tab[ii].sDat;
  iNr = iap[0];
  ++iap;

  // loop tru trimmedCurves.
  for(i1=0; i1<iNr; ++i1) {
    ii = STP_r_findInd (iap[i1], ii);
      printd(" CCVseg %d %d #%d typ=%d\n",i1,ii,s_tab[ii].sInd,s_tab[ii].sTyp);
    irc = STP_r_creCurv1 (ii);
    if(irc < 0) return irc;
  }


  // only one segment: skip CCV, but copy gTyp & gInd.
  if(iNr < 2) {
    ii = STP_r_findInd (iap[0], ii);
    // copy gTyp u gInd from CART_P --> VERT_P
    s_tab[sInd].gTyp = s_tab[ii].gTyp;
    s_tab[sInd].gInd = s_tab[ii].gInd;
    return 0;
  }


  // create CCV from segments
  gTxt[0] = '\0';
  for(i1=0; i1<iNr; ++i1) {
    ii = STP_r_findInd (iap[i1], ii);
    // printf(" Crv. %d ind=%d #%d typ=%d\n",i1,ii,s_tab[ii].sInd,s_tab[ii].sTyp);
    AP_obj_add_obj (gTxt, s_tab[ii].gTyp, s_tab[ii].gInd);
  }
  irc = STP_r_creObj1 (sInd, Typ_CVTRM, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  if(resMod == 2)     // hide bei Solids
    // GA_hide__ (0, s_tab[sInd].gInd, s_tab[sInd].gTyp);
    GA_view__ (-1L, 1, s_tab[sInd].gTyp, s_tab[sInd].gInd);

  // return -4;
  return 0;

}


//================================================================
  int STP_r_creSur1 (int sInd) {
//================================================================
// create ADVANCED_FACE((Aussen/Innenbegrenzung), Stuetzflaeche);
// create Aussenkontur
// create Innenkonturen
// create Stuetzflaeche
// create begrenzte Flaeche

  int     irc, i1, i2, i3, i4,ii, iNr, *iap, isa, iss;
  int     ic1, ic2;
  Point   pt1, pt2;


  // printf("=============================================== \n");
  // printf("SSSSSSSSSSS STP_r_creSur1 ind=%d #%d\n",sInd,s_tab[sInd].sInd);
  printd("STP_r_creSur1 %d #%d\n",sInd,s_tab[sInd].sInd);


  iap = s_tab[sInd].sDat;
  iNr = iap[0];
  ++iap;
  ii = sInd;

  // loop tru Konturen.
  for(i1=0; i1<iNr; ++i1) {
    ii = STP_r_findInd (iap[i1], ii);
      printd(" Cont. %d ind=%d #%d typ=%d\n",
             i1,ii,s_tab[ii].sInd,s_tab[ii].sTyp);
    irc = STP_r_creCont1 (ii);
    if(irc < 0) return irc;
  }


  // nun sind alle Aussen/InnenKonturen erzeugt.

  // check typ of Stuetzflaeche
  isa = iap[iNr];
  iss = STP_r_findInd (isa, ii);
    printd(" StuetzFl ind=%d typ=%d\n",iss,s_tab[iss].sTyp);
    printd(" SA ind=%d #%d=%s\n",iss,isa,STP_r_TypTxt_sTyp(s_tab[iss].sTyp));



  //----------------------------------------------------------------
  // planar
  if((s_tab[iss].sTyp == SC_PLANE) ||
     (s_tab[iss].sTyp == SC_QUASI_UNIFORM_SURFACE))  {

    // create begrenzte Flaeche
    gTxt[0] = '\0';


  //----------------------------------------------------------------
  } else if((s_tab[iss].sTyp == SC_CONICAL_SURFACE)             ||
            (s_tab[iss].sTyp == SC_CYLINDRICAL_SURFACE)         ||
            (s_tab[iss].sTyp == SC_TOROIDAL_SURFACE)            ||
            (s_tab[iss].sTyp == SC_DEGENERATE_TOROIDAL_SURFACE) ||
            (s_tab[iss].sTyp == SC_SURFACE_OF_REVOLUTION)       ||
            (s_tab[iss].sTyp == SC_SURFACE_OF_LINEAR_EXTRUSION) ||
            (s_tab[iss].sTyp == SC_B_SPLINE_SURFACE_WITH_KNOTS) ||
            (s_tab[iss].sTyp == SC_BOUNDED_SURFACE)             ||
            (s_tab[iss].sTyp == SC_SPHERICAL_SURFACE))             {


    irc = STP_r_cre2 (iss); // create SRV
      // printf(" nach STP_r_cre2\n");
    if(irc < 0) return irc;

    // hide Stuetzflaeche
    // GA_hide__ (0, s_tab[iss].gInd, s_tab[iss].gTyp);
    GA_view__ (-1L, 1, s_tab[iss].gTyp, s_tab[iss].gInd);
      // printf(" nach GA_view__\n");
     

    // make Stuetzflaeche symbolic
    GA_sStyl__ (-1L, 2, s_tab[iss].gTyp, s_tab[iss].gInd);
      // printf(" nach GA_sStyl__\n");


    // create begrenzte Flaeche
    strcpy(gTxt, "FSUB");
    AP_obj_add_obj (gTxt, s_tab[iss].gTyp, s_tab[iss].gInd);

    // printd(" A-out:|%s|\n",gTxt);
    // return 0;




  //----------------------------------------------------------------
  } else {
    TX_Error("STP_r_creSur1 E001 %d",s_tab[iss].sTyp);
    return -1;
  }

  if(s_tab[iss].sTyp == SC_TOROIDAL_SURFACE) {
    printd(" |%s| TOR %d\n",gTxt,iNr);
    if(iNr == 2) {  // nr of contours
      i1 = STP_r_findInd (iap[0], ii);
      i2 = STP_r_findInd (iap[1], ii);
      // test contours for being circles
      ic1 = STP_r_cir_ck1 (&i3, i1);
      ic2 = STP_r_cir_ck1 (&i4, i2);
      // create revolved surface
      if((!ic1) && (!ic2)) {
        STP_r_SurTor_cre1 (iss,i1,i2,i3,i4);
        goto L_CreObj;
      }
    }
  }


  // test CYLINDER & CONE:
  //   if it has 2 Circles only: create ruledSurf (ignore supportSurface)
  if((s_tab[iss].sTyp == SC_CYLINDRICAL_SURFACE)  ||
     (s_tab[iss].sTyp == SC_CONICAL_SURFACE))         {
      printd(" |%s| CYL | CON %d\n",gTxt,iNr);
  
    if(iNr == 2) {  // nr of contours
      // get types of contours
      i1 = STP_r_findInd (iap[0], ii);
      i2 = STP_r_findInd (iap[1], ii);
        printd(" types=%d %d\n",s_tab[i1].gTyp,s_tab[i2].gTyp);
      if((s_tab[i1].gTyp == Typ_CI) &&
         (s_tab[i2].gTyp == Typ_CI)) {
        sprintf(gTxt, "SRU C%d C%d",s_tab[i1].gInd,s_tab[i2].gInd);
        goto L_CreObj;
      }
      // if one is a point: make a SRU
      if((s_tab[i1].gTyp == Typ_PT) ||
         (s_tab[i2].gTyp == Typ_PT)) {
        sprintf(gTxt, "SRU %c%d %c%d",
                AP_typChar_typ(s_tab[i1].gTyp),s_tab[i1].gInd,
                AP_typChar_typ(s_tab[i2].gTyp),s_tab[i2].gInd);
        goto L_CreObj;
      }
    }
  }




  // loop tru Konturen; add as Text
  // zuerst Aussenkontur
  ii = iss;
  // ic1 = 0;
  // ic2 = 0;
  i2 = 0;
  // first add the OUTER_BOUND(s) - cylinder can have 2 OUTER_BOUNDs !
  for(i1=0; i1<iNr; ++i1) {
    ii = STP_r_findInd (iap[i1], ii);
    if(s_tab[ii].sTyp == SC_FACE_BOUND) continue;
    // SC_FACE_OUTER_BOUND
      // printf(" add OUTER_BOUND %d ind=%d #%d typ=%d\n",
              // i1,ii,s_tab[ii].sInd,s_tab[ii].sTyp);
    AP_obj_add_obj (gTxt, s_tab[ii].gTyp, s_tab[ii].gInd);
      // printf(" gTxt=|%s|\n",gTxt);
    // if(i1 == 0) ic1 = ii;
    // if(i1 == 1) ic2 = ii;
    ++i2;
  }

  // add Loecher
  ii = iss;

/*
  // cylinder can have first and last bound as normal FACE_BOUND only !
  if((s_tab[iss].sTyp == SC_CYLINDRICAL_SURFACE)    &&
     (iNr > 2)                                      &&
     (i2 < 1))                                           {
    // cylinder, 3 or more bounds.
    i2 = 0;
    ii = STP_r_findInd (iap[i2], ii);
    AP_obj_add_obj (gTxt, s_tab[ii].gTyp, s_tab[ii].gInd);       // first
      printf(" gTxt=|%s|\n",gTxt);
    L_nxtCylObj:
    i2 = iNr -1;
    ii = STP_r_findInd (iap[i2], ii);
    AP_obj_add_obj (gTxt, s_tab[ii].gTyp, s_tab[ii].gInd);     // last
      printf(" gTxt=|%s|\n",gTxt);
    --iNr;
    if(iNr > 1) goto L_nxtCylObj;
if(strlen(gTxt) > 20) exit(0);
    goto L_CreObj;
  }
*/

  for(i1=0; i1<iNr; ++i1) {
    ii = STP_r_findInd (iap[i1], ii);
    if(s_tab[ii].sTyp == SC_FACE_OUTER_BOUND) continue;
    // SC_FACE_BOUND
      // printf(" add BOUND %d ind=%d #%d typ=%d\n",
              // i1,ii,s_tab[ii].sInd,s_tab[ii].sTyp);
    AP_obj_add_obj (gTxt, s_tab[ii].gTyp, s_tab[ii].gInd);
      // printf(" gTxt=|%s|\n",gTxt);
    // CYLINDRICAL_SURFACE from 2 CIRCLE's can have 2 FACE_BOUND's
    // if(i1 == 0) ic1 = ii;
    // if(i1 == 1) { ic2 = ii; i2 = 1;}
  }

/*
  // test: CYLINDRICAL_SURFACE with 2 circles as contours
    printf(" test-cylSur: Nr=%d surTyp=%d cont1Typ=%d cont2Typ%d\n",
           iNr,s_tab[iss].sTyp,s_tab[ic1].gTyp,s_tab[ic2].gTyp);
  if(s_tab[iss].sTyp == SC_CYLINDRICAL_SURFACE) {
    if((iNr == 2)                  && 
       (s_tab[ic1].gTyp == Typ_CI) &&
       (s_tab[ic2].gTyp == Typ_CI))    {
      // printd(" - is CYLINDRICAL_SURFACE from 2 circles ..\n");
      if(i2) {
        // 2 x SC_FACE_BOUND
        sprintf(gTxt, "SRU C%d C(C%d,REV)",s_tab[ic1].gInd,s_tab[ic2].gInd);
      } else {
        // 2 x SC_FACE_OUTER_BOUND
        sprintf(gTxt, "SRU C%d C%d)",s_tab[ic1].gInd,s_tab[ic2].gInd);
      }
    }
  }
*/

  L_CreObj:

  irc = STP_r_creObj1 (sInd, Typ_SUR, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  // printf("ex STP_r_creSur1\n");

  return 0;

}


//================================================================
  int STP_r_creSur2 (int sInd) {
//================================================================
// RECTANGULAR_TRIMMED_SURFACE
// rechteckig begrenzte CONICAL_SURFACE/CYLINDRICAL_SURFACE
// 1 Link --> Stuetzflaeche (CONICAL_SURFACE/CYLINDRICAL_SURFACE ..)
// 4 doubles (u1,u2, v1,v2)
// out: A = SRV axis mantel ANG(a1) ANG(a2)

  int     irc, lsa, isa, lax, iax;
  double  u1,u2, v1,v2, rdc,oa, rd1, rd2, d1;
  Point   px1, px2, pm1, pm2;
  void    *vp;
  Plane   pl1;



  // printf("===========================================\n");
    printd("STP_r_creSur2 %d #%d\n",sInd,s_tab[sInd].sInd);
  


  vp = STP_r_getInt (&lsa, s_tab[sInd].sDat);
  vp = STP_r_getDb (&u1, vp);
  vp = STP_r_getDb (&u2, vp);
  vp = STP_r_getDb (&v1, vp);
  vp = STP_r_getDb (&v2, vp);
    // printf(" u1=%f u2=%f v1=%f v2=%f\n",u1,u2,v1,v2);


  isa = STP_r_findInd (lsa, sInd-3);  // index Stuetzflaeche suchen
  if(isa < 0) return -1;



  //----------------------------------------------------------------
  // CONICAL_SURFACE
  if(s_tab[isa].sTyp != SC_CONICAL_SURFACE) goto L_cyl;

  // 1 Link  --> AXIS
  // 2 doubles (radius, halber_Oeffnungswinkel)

  // Der Nullpunkt (pl1.po) ist die Spitze des Konus; von dort in Z-Richtung
  // mit Oeffnungswinkel oa.

  vp = STP_r_getInt (&lax, s_tab[isa].sDat);
  vp = STP_r_getDb (&rdc, vp);
  vp = STP_r_getDb (&oa, vp);
    // printf(" cone-Rd=%f angOe=%f\n",rdc,oa);

  iax = STP_r_findInd (lax, isa);  // index AXIS suchen
  if(iax < 0) return -1;
  STP_r_PLN_AXIS2 (&pl1, iax); // get Plane from AXIS

  // Mittelachse
  px1 = pl1.po;
  UT3D_pt_traptvc (&px2, &px1, &pl1.vz);

  // 2 Punkte auf der Mittelachse im Abstand v1, v2.
  UT3D_pt_traptvclen (&pm1, &pl1.po, &pl1.vz, v1);
  UT3D_pt_traptvclen (&pm2, &pl1.po, &pl1.vz, v2);
    // STP_r_dispSym1 (52, &pm1);
    // STP_r_dispSym1 (52, &pm2);


  // compute Radien from Oeffnungswinkel u Radius
  d1 = tan (UT_RADIANS(oa));
  rd1 = v1 * d1;
  rd2 = v2 * d1;
    printf(" rd1=%f rd2=%f\n",rd1,rd2);


  // pm1 um rdc entlang X-Achse rausschieben
  UT3D_pt_traptvclen (&pm1, &pm1, &pl1.vx, rd1);
  UT3D_pt_traptvclen (&pm2, &pm2, &pl1.vx, rd2);
    // STP_r_dispSym1 (63, &pm1);
    // STP_r_dispSym1 (63, &pm2);

  // return -4; // TEST
  goto L_write;



  //----------------------------------------------------------------
  L_cyl:   // CYLINDRICAL_SURFACE
  if(s_tab[isa].sTyp != SC_CYLINDRICAL_SURFACE) goto L_sph;
  // untrimmed Cyl: STP_r_creSurCyl1

  // axis und radius der CYLINDRICAL_SURFACE holen
  vp = STP_r_getInt (&lax, s_tab[isa].sDat);
  vp = STP_r_getDb (&rdc, vp);

  iax = STP_r_findInd (lax, isa);  // index AXIS suchen
  if(iax < 0) return -1;

  STP_r_cre2 (iax);  // create Axis

  strcpy(gTxt, "CYL");
  AP_obj_add_obj (gTxt, s_tab[iax].gTyp, s_tab[iax].gInd);  // refsys
  AP_obj_add_val (gTxt, rdc);

  AP_obj_add_val (gTxt, u1);
  AP_obj_add_val (gTxt, u2);
  AP_obj_add_val (gTxt, v1);
  AP_obj_add_val (gTxt, v2);

  // printf(" gTxt=|%s|\n",gTxt);
  // return -4;
  goto L_wri_9;



  //----------------------------------------------------------------
  L_sph:   // SPHERICAL_SURFACE
  if(s_tab[isa].sTyp != SC_SPHERICAL_SURFACE) {
    TX_Error("STP_r_creSur2 E002 %d %d %d",sInd,isa,s_tab[isa].sTyp);
    return -1;
  }

  // printf(" sph %f %f %f %f\n",u1,u2, v1,v2);

  // SPHERICAL_SURF: Axis, radius
  vp = STP_r_getInt (&lax, s_tab[isa].sDat);
  vp = STP_r_getDb (&rdc, vp);

  iax = STP_r_findInd (lax, isa);  // index AXIS suchen
  if(iax < 0) return -1;

  STP_r_cre2 (iax);

  strcpy(gTxt, "SPH");

  AP_obj_add_obj (gTxt, s_tab[iax].gTyp, s_tab[iax].gInd);
  
  AP_obj_add_val (gTxt, rdc);

  AP_obj_add_val (gTxt, u1);
  AP_obj_add_val (gTxt, u2);
  AP_obj_add_val (gTxt, v1+90.);
  AP_obj_add_val (gTxt, v2+90.);

  goto L_wri_9;



  //----------------------------------------------------------------
// A20=SRV L() L() ANG(a1) ANG(a2)
  L_write:
  strcpy(gTxt, "SRV");

  AP_obj_add_ln (gTxt, 0, &px1, &px2);
  AP_obj_add_ln (gTxt, 0, &pm1, &pm2);

  L_wri_8:
  AP_obj_add_val (gTxt, u1);
  AP_obj_add_val (gTxt, u2);


  // make obj, save-> gCad, stor gC-Typ & Index -> s_tab
  L_wri_9:
  irc = STP_r_creObj1 (sInd, Typ_SUR, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  // return -4;  //TEST
  return 0;

}


//================================================================
  int STP_r_creSurCyl1 (int sInd) {
//================================================================
// CYLINDRICAL_SURFACE  --> A20=CYL L1 L5
// 1 Link  --> AXIS
// 1 double   (radius)

  int     irc, lax, iax;
  void    *vp;
  double  rdc;
  Point   px1, px2, pm1, pm2;
  Plane   pl1;

  // printf("STP_r_creSurCyl1 %d\n",sInd);


  vp = STP_r_getInt (&lax, s_tab[sInd].sDat);   // get Link -->AXIS
  vp = STP_r_getDb (&rdc, vp);                  // get radius
    // printf(" rdc=%f\n",rdc);

  iax = STP_r_findInd (lax, sInd-3);   // index Stuetzflaeche suchen


  STP_r_cre2 (iax);

  strcpy(gTxt, "CYL");
  AP_obj_add_obj (gTxt, s_tab[iax].gTyp, s_tab[iax].gInd);
  AP_obj_add_val (gTxt, rdc);


/*
  STP_r_PLN_AXIS2 (&pl1, iax);         // getPlane from AXIS


  px1 = pl1.po;
  UT3D_pt_traptvc (&px2, &px1, &pl1.vz);

  
  // 2 Punkte auf der Mittelachse im Abstand rdc
  UT3D_pt_traptvclen (&pm1, &px1, &pl1.vx, rdc);
  UT3D_pt_traptvclen (&pm2, &px2, &pl1.vx, rdc);

  strcpy(gTxt, "SRV");

  AP_obj_add_ln (gTxt, 0, &px1, &px2);
  AP_obj_add_ln (gTxt, 0, &pm1, &pm2);
*/

  // make obj, save-> gCad, stor gC-Typ & Index -> s_tab
  irc = STP_r_creObj1 (sInd, Typ_SUR, Typ_Txt, gTxt);
    // printf(" created: %d %d %d\n",sInd,s_tab[sInd].gTyp,s_tab[sInd].gInd);

  if(irc < 0) return irc;

  return 0;

}
 


//================================================================
  int STP_r_creSur3 (int sInd) {
//================================================================
// CONICAL_SURFACE  -->  A=SRV L L
// 1 Link  --> AXIS
// 2 doubles (radius, halber_Oeffnungswinkel)


  int     irc, lsa, isa, lpo, lvx, lvz;
  long    ipt, ipc;
  double  h1, rdc, oa;
  void    *vp;
  Point   po, pc, ptt;
  Vector  vx, vz;
  Plane   pl1;


  // printf("种种种种种种种种种种种种种种种种种种种种种\n");
  // printf("STP_r_creSur3 %d\n",sInd);



  // mal die Stuetzflaeche generieren
  vp = STP_r_getInt (&lsa, s_tab[sInd].sDat);
  vp = STP_r_getDb (&rdc, vp);
  vp = STP_r_getDb (&oa, vp);
    // printf(" iAx=%d cone-Rd=%f angOe=%f\n",lsa,rdc,oa);

  isa = STP_r_findInd (lsa, sInd-3);


  // get Index of origin of Axis
  vp = STP_r_getInt (&lpo, s_tab[isa].sDat);
  lpo = STP_r_findInd (lpo, lpo);
  // get Index of VZ of Axis
  vp = STP_r_getInt (&lvz, vp);
  lvz = STP_r_findInd (lvz, lvz);
  // get Index of VX of Axis
  vp = STP_r_getInt (&lvx, vp);
  lvx = STP_r_findInd (lvx, lvx);
    // printf(" lpo=%d lvz=%d lvx=%d\n",lpo,lvz,lvx);

  // create PT origin
  irc = STP_r_cre2 (lpo);
  if(irc < 0) return irc;

  // create VC vz
  irc = STP_r_cre2 (lvz);
  if(irc < 0) return irc;

  // create VC vx
  irc = STP_r_cre2 (lvx);
  if(irc < 0) return irc;


  // load po
  STP_r_PT_CARTPT (&po, lpo);

  // load vz
  STP_r_VC_DIR (&vz, lvz);

  // load vx
  STP_r_VC_DIR (&vx, lvx);


  // STP_r_PLN_AXIS2 (&pl1, isa); // get Plane from AXIS
    // DEB_dump_obj__ (Typ_PLN, &pl1, "cone:");




  // compute Konushoehe from Oeffnungswinkel u Radius
  // Oeffnungswinkel ex CATIA-V4: Degree !!
  // Oeffnungswinkel ex Varicad: Radians !!
  if(angMod == 0) {        // default = radians.
    h1 = rdc / tan (oa);
  } else {                 // 1 = Degree
    h1 = rdc / tan (UT_RADIANS(oa));   // ex CATIA-V4: Degree !!
  }

    // printf(" KonHoe=%f rdc=%f\n",h1,rdc);


  // ipt = create point on top;
  UT3D_pt_traptvclen (&ptt, &po, &vz, -h1);
    // STP_r_dispSym1 (52, &ptt);
  ipt = STP_r_creObj1 (-1, Typ_PT, Typ_PT, &ptt);
  if(ipt < 0) return ipt;

  if(resMod == 2)     // hide bei Solids
    // GA_hide__ (0, ipt, Typ_PT);
    GA_view__ (-1L, 1, Typ_PT, ipt);



  // wenn h1 zu klein (zB weil rdc == 0 (Spitze gegeben)
  if(h1 < WC_ask_ModSiz() / 500.) goto L_V2;

  // centerpt = Axis-Origin
  ipc = s_tab[lpo].gInd;

  // point on circ
  UT3D_pt_traptvclen (&pc, &po, &vx, rdc);

  goto L_create;



  //----------------------------------------------------------------
  L_V2:

  h1 = WC_ask_ModSiz() / 10.;
  if(angMod == 0) {        // default = radians.
    rdc = h1 * tan (oa);
  } else {                 // 1 = Degree
    rdc = h1 * tan (UT_RADIANS(oa));   // ex CATIA-V4: Degree !!
  }
    // printf(" korr: KonHoe=%f rdc=%f\n",h1,rdc);

  // top ipt/ptt ist OK; 

  // ipc: move top um h1
  UT3D_pt_traptvclen (&po, &ptt, &vz, h1);
  ipc = STP_r_creObj1 (-1, Typ_PT, Typ_PT, &po);
  if(ipc < 0) return ipc;
    // STP_r_dispSym1 (52, &po);


  if(resMod == 2)     // hide bei Solids
    // GA_hide__ (0, ipc, Typ_PT);
    GA_view__ (-1L, 1, Typ_PT, ipc);



  // point on circ
  UT3D_pt_traptvclen (&pc, &po, &vx, rdc);
    // STP_r_dispSym1 (52, &pc);







  //----------------------------------------------------------------
  // A = SRC  L(ipc ipt) L(P(pc) ipt)
  //
  //       ipt
  //        |\
  //        | \
  //        |  \
  //        |   \
  //        |    \
  //        +-----+
  //       ipc     pc
  //
  L_create:

  // Axis = L ( P[ipc] - Pipt)
  // Contour = L (P[pc]  Pipt)
  strcpy(gTxt, "SRV L(");

  // ind ipt = s_tab[ipt].gInd
  sprintf(gTxt,"SRV L(P%ld P%ld) L(",ipc,ipt);

  // point on circ as P(x y z)
  // strcat(gTxt, ") L(");
  AP_obj_add_pt (gTxt, &pc);

  AP_obj_add_obj (gTxt, Typ_PT, ipt);
  strcat(gTxt, ")");

    // printf(" gTxt=|%s|\n",gTxt); // return -4;

  // make obj, save-> gCad, stor gC-Typ & Index -> s_tab
  irc = STP_r_creObj1 (sInd, Typ_SUR, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  // return -4; // TEST
  return 0;

}


//================================================================
  int STP_r_creSur4 (int sInd) {
//================================================================
// CONICAL_SURFACE (nicht getrimmt)  ausgeben als
// A1=SPH R(P<origin>) <radius>

  int    irc, iax, ipo, *iap;
  void   *vp;
  double rdc;


  // printf("00000000000000000000000000000000000000000000000 \n");
  // printf("STP_r_creSur4 %d\n",sInd);


  // SPHERICAL_SURF: Axis, radius
  vp = STP_r_getInt (&iax, s_tab[sInd].sDat);
  vp = STP_r_getDb (&rdc, vp);

  iax = STP_r_findInd (iax, sInd);  // index AXIS suchen
  if(iax < 0) return -1;
    // printf(" iax=%d r=%f\n",iax,rdc);

  // get index of origin of refsys
  vp = STP_r_getInt (&ipo, s_tab[iax].sDat); // first ling of AXIS is origin
  ipo = STP_r_findInd (ipo, sInd);           // index origin suchen
    // printf(" ipo=%d\n",ipo);

  STP_r_cre2 (ipo);  // create origin

  strcpy(gTxt, "SPH R(");
  AP_obj_add_obj (gTxt, s_tab[ipo].gTyp, s_tab[ipo].gInd);
  strcat(gTxt, ")");
  AP_obj_add_val (gTxt, rdc);

  irc = STP_r_creObj1 (sInd, Typ_SUR, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  return 0;

}


//================================================================
  int STP_r_creSur5 (int sInd) {
//================================================================
// TOROIDAL_SURFACE (nicht getrimmt)  ausgeben als
// B20=TOR P5 D(L1) C2

  int    irc, iax, ipo, ivz, *iap;
  void   *vp;
  double rd1, rd2;


  // printf("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n");
  // printf("STP_r_creSur5 %d\n",sInd);


  // TOROIDAL_SURFACE: Axis, rad1, rad2
  vp = STP_r_getInt (&iax, s_tab[sInd].sDat);
  vp = STP_r_getDb (&rd1, vp);
  vp = STP_r_getDb (&rd2, vp);

  iax = STP_r_findInd (iax, sInd);  // index AXIS suchen
  if(iax < 0) return -1;
    // printf(" iax=%d r1=%f r2=%f\n",iax,rd1,rd2);


  // get index of origin of refsys
  vp = STP_r_getInt (&ipo, s_tab[iax].sDat); // 1. link of AXIS is origin
  vp = STP_r_getInt (&ivz, vp);              // 2. link of AXIS is Z-vec
  ipo = STP_r_findInd (ipo, sInd);           // index origin suchen
  ivz = STP_r_findInd (ivz, sInd);           // index Z-vec suchen
    // printf(" ipo=%d ivz=%d\n",ipo,ivz);

  STP_r_cre2 (ipo);  // create origin
  STP_r_cre2 (ivz);  // create Z-Vec


  // Body erzeugen
  strcpy(gTxt, "TOR");
  AP_obj_add_obj (gTxt, s_tab[ipo].gTyp, s_tab[ipo].gInd);
  AP_obj_add_obj (gTxt, s_tab[ivz].gTyp, s_tab[ivz].gInd);
  AP_obj_add_val (gTxt, rd1);
  AP_obj_add_val (gTxt, rd2);
    // printf(" creSur5 |%s|\n",gTxt);


  irc = STP_r_creObj1 (sInd, Typ_SOL, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  return 0;

}


//================================================================
  int STP_r_creSur6 (int sInd) {
//================================================================
// SURFACE_OF_REVOLUTION (nicht getrimmt)  ausgeben als
// B20=TOR P5 D(L1) C2

  int    irc, iax, ico;
  void   *vp;


  // printf("RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n");
  printd("STP_r_creSur6 %d #%d\n",sInd,s_tab[sInd].sInd);


  // SURFACE_OF_REVOLUTION: Contour, Axis

  vp = STP_r_getInt (&ico, s_tab[sInd].sDat);
  vp = STP_r_getInt (&iax, vp);

  ico = STP_r_findInd (ico, sInd);  // index Contour suchen
  iax = STP_r_findInd (iax, sInd);  // index Axis suchen
    printd(" ico=%d #%d\n",ico,s_tab[ico].sInd);
    printd(" iax=%d #%d\n",iax,s_tab[iax].sInd);


  // create axis
  STP_r_creLn4 (iax);


  // create contour
  if(s_tab[ico].sTyp == SC_LINE) STP_r_creLn0 (ico);
  else                           STP_r_cre2 (ico);


  // TEST/DEBUG:
  // if((s_tab[iax].gTyp==3)&&(s_tab[iax].gInd==723)) TX_Error("E");

  if(s_tab[iax].gInd < 0)
    {TX_Error("STP_r_creSur6 E1_%d",s_tab[sInd].sInd); return -1;}
  if(s_tab[ico].gInd < 0)
    {TX_Error("STP_r_creSur6 E2_%d",s_tab[sInd].sInd); return -1;}


  // Surf erzeugen
  strcpy(gTxt, "SRV");
  AP_obj_add_obj (gTxt, s_tab[iax].gTyp, s_tab[iax].gInd); // Axis
  AP_obj_add_obj (gTxt, s_tab[ico].gTyp, s_tab[ico].gInd); // contour

  irc = STP_r_creObj1 (sInd, Typ_SUR, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  // return -4;
  return 0;

}

//================================================================
  int STP_r_creSur7 (int sInd) {
//================================================================
// SURFACE_OF_LINEAR_EXTRUSION
// A20=SRU S2 D(0 100 0)

  int     irc, ico, ivc;
  // double  d1;
  void    *vp;
  Vector  vc1;


  // printf("RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n");
  printd("STP_r_creSur7 %d #%d\n",sInd,s_tab[sInd].sInd);


  // SURFACE_OF_LINEAR_EXTRUSION (#<KonturKurve>,#<VEKTOR>)
  vp = STP_r_getInt (&ico, s_tab[sInd].sDat);
  vp = STP_r_getInt (&ivc, vp);

  ico = STP_r_findInd (ico, sInd);  // index AXIS suchen
  if(ico < 0) return -1;

  ivc = STP_r_findInd (ivc, sInd);  // index AXIS suchen
  if(ivc < 0) return -1;
    printd(" ico=%d ivc=%d\n",ico,ivc);

  STP_r_cre2 (ico);  // create contour

  // den Vektor holen
  if(s_tab[ivc].sTyp != SC_VECTOR) {
    TX_Error ("STP_r_creSur7 E001");
    return -1;
  }
  // STP_r_VC_VEC_1 (&vc1, &d1, ivc);
  STP_r_VC_VEC_t (&vc1, ivc);

  strcpy(gTxt, "SRU");
  AP_obj_add_obj (gTxt, s_tab[ico].gTyp, s_tab[ico].gInd);

  // create Vektor; aber nicht normieren !
  AP_obj_add_vc0 (gTxt, &vc1);

  irc = STP_r_creObj1 (sInd, Typ_SUR, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  return 0;

}


//================================================================
  int STP_r_creSur8 (int sInd) {
//================================================================
// B_SPLINE_SURFACE_WITH_KNOTS
  // u_multiplicities : LIST [2:?] OF INTEGER;
  // v_multiplicities : LIST [2:?] OF INTEGER;
  // u_knots          : LIST [2:?] OF parameter_value;
  // v_knots          : LIST [2:?] OF parameter_value;
  // knot_spec        : knot_type;

// B_SPLINE_SURFACE_WITH_KNOTS (id,
// #,#,(?),             // pointNrs ?
// (),                  // controlpoints
// ?,L,L,L
// (#,#), 
// (#,#), 
// KNOT_TYPE);

// KNOT_TYPE:
//   uniform_knots|quasi_uniform_knots|piecewise_bezier_knots|unspecified



  int     irc, i1, i2, i3, iNr, *iap, lpt, ipt, *uia, *via, *ukv, *vkv,
          uNr, vNr, ii;
  double  d1;
  SurBSpl su1;
  ObjGX   o1;

/*
// Area: B-Spline-Surf   Typ_SURBSP
typedef struct {long ptUNr, ptVNr, degU, degV;
                Point *cpTab; double *kvTabU, *kvTabV;}             SurBSpl;
// cpTab[ptUNr*ptVNr]    u0v0,u1v0..uUv0;u1v0,u1v1..uUv1;..;uUv0,uUv1..uUvV;
// kvTabU[ptUNr+degU+1]
// kvTabV[ptVNr+degV+1]
*/



  // printf("=============================================\n");
  // printf("STP_r_creSur8 %d #%d\n",sInd,s_tab[sInd].sInd);

  iap = s_tab[sInd].sDat;

  su1.degV = iap[0];
  ++iap;
  su1.degU = iap[0];
  ++iap;
    // printf(" degu=%d degV=%d\n",su1.degU,su1.degV);


  su1.ptUNr = iap[0];
  ++iap;
  su1.ptVNr = iap[0];
  ++iap;
    // printf(" ptUNr=%d ptVNr=%d\n",su1.ptUNr,su1.ptVNr);


  // Tempspace
  UME_init (&s_mSpc, memspc501, sizeof(memspc501));


  // reserve ptUNr*ptVNr points in s_mSpc
  su1.cpTab = s_mSpc.next;
  iNr = su1.ptUNr * su1.ptVNr;
  irc = UME_add (&s_mSpc, sizeof(Point) * iNr);
  if(irc < 0) {TX_Error("STP_r_creSur8 EOM1"); return -4;}

  ipt = sInd;
  for(i1=0; i1<iNr; ++i1) {
    iap = STP_r_getInt (&lpt, iap);
    ipt = STP_r_findInd (lpt, ipt);
    if(ipt < 0) return -2;
      // printf(" %d lpt=%d ipt=%d\n",i1,lpt,ipt);
    STP_r_PT_CARTPT (&su1.cpTab[i1], ipt);  // copy point
      // STP_r_dispSym1 (52, &su1.cpTab[i1]);
  }



  iap = STP_r_getInt (&uNr, iap);  // get nr of u-ints
  uia = iap;                       // startaddres u-iTab;
  iap = &iap[uNr];                 // skip over u-iTab

  iap = STP_r_getInt (&vNr, iap);  // get nr of v-ints
  via = iap;                       // startaddres v-iTab;
  iap = &iap[vNr];                 // skip over v-iTab
    // printf(" uNr=%d vNr=%d\n",uNr,vNr);



  // reserve u-knotVals in s_mSpc
  i1 = su1.ptVNr + su1.degV + 1;
  su1.kvTabV = s_mSpc.next;
  irc = UME_add (&s_mSpc, sizeof(double) * i1);
  if(irc < 0) {TX_Error("STP_r_creSur8 EOM2"); return -4;}
    // printf(" nr of kvVr=%d\n",i1);


  // load and copy u-knotvalues
  iap = STP_r_getInt (&i1, iap);  // get nr of u-knotVals
  i3 = 0;
  for(i1=0; i1<uNr; ++i1) {
    uia = STP_r_getInt (&ii, uia); // get next multiplicity
    // printf(" u-%d multip=%d i3=%d\n",i1,ii,i3);
    iap = STP_r_getDb (&d1, iap);  // get next knotval
    // printf(" u-%d kVal=%f ii=%d\n",i1,d1,ii);
    for(i2=0; i2<ii; ++i2) {
      // printf(" v%d",i3);
      // copy knotvalue -->cv1.kvTab[i1]
      su1.kvTabV[i3] = d1;
      ++i3;
    }
    // printf(" \n",i3);
  }



  // reserve v-knotVals in s_mSpc
  i1 = su1.ptUNr + su1.degU + 1;
  su1.kvTabU = s_mSpc.next;
  irc = UME_add (&s_mSpc, sizeof(double) * i1);
  if(irc < 0) {TX_Error("STP_r_creSur8 EOM3"); return -4;}
    // printf(" Nr of kvU=%d\n",i1);


  // load and copy v-knotvalues
  iap = STP_r_getInt (&i1, iap);  // get nr of v-knotVals
  i3 = 0;
  for(i1=0; i1<vNr; ++i1) {
    via = STP_r_getInt (&ii, via); // get next multiplicity
    // printf(" v-%d multip=%d\n",i1,ii);
    iap = STP_r_getDb (&d1, iap);  // get next knotval
    // printf(" v-%d kVal=%f\n",i1,d1);
    for(i2=0; i2<ii; ++i2) {
      // printf(" u%d",i3);
      // copy knotvalue -->cv1.kvTab[i1]
      su1.kvTabU[i3] = d1;
      ++i3;
    }
    // printf(" \n",i3);
  }


  //----------------------------------------------------------------
  // nun ist die B-Spl.Surf in struct su1;
  // try to degrade ..
  irc = UT3D_obj_cnvt_sbsp (&o1, &su1, &s_mSpc);
  if(irc >= 0) {
    irc = STP_r_creObj1 (sInd, o1.typ, o1.form, o1.data);

  } else {
    irc = STP_r_creObj1 (sInd, Typ_SURBSP, Typ_SURBSP, &su1);
  }

  if(irc < 0) return irc;

    // TEST:
    // DEB_dump_obj__ (Typ_SURBSP, &su1, "");
    // if((su1.ptVNr + su1.degV + 1) == 17) return -4;
    // for(i1=0;i1<12;++i1){if(i1<6)su1.kvTabU[i1]=0.;else su1.kvTabU[i1]=1.;}
    // for(i1=0;i1<10;++i1){if(i1<5)su1.kvTabV[i1]=0.;else su1.kvTabV[i1]=1.;}


  return 0;

}


//================================================================
  int STP_r_creSur9 (int sInd) {
//================================================================
// BOUNDED_SURFACE   (Rat.Bspl-Surf.)

  int      irc, i1, i2, i3, iNr, *iap, lpt, ipt, *uia, *via, *ukv, *vkv,
           uNr, vNr, ii, ir1, ir2, ir3;
  double   d1;
  SurRBSpl su1;
  ObjGX    o1;


  // printf("=============================================\n");
  // printf("STP_r_creSur9 %d #%d\n",sInd,s_tab[sInd].sInd);


  iap = s_tab[sInd].sDat;


  // ir1 = index of SC_B_SPLINE_SURFACE suchen
  // ir2 = index of SC_B_SPLINE_SURFACE_WITH_KNOTS suchen
  // ir3 = index of SC_RATIONAL_B_SPLINE_SURFACE suchen
  for(i1=0;i1<3;++i1) {
    ir1 = iap[i1];
    if(s_tab[ir1].sTyp == SC_B_SPLINE_SURFACE) goto L_ir2;
  }
  TX_Error("STP_r_creSplBC E0I1"); return -1;

  L_ir2:
  for(i1=0;i1<3;++i1) {
    ir2 = iap[i1];
    if(s_tab[ir2].sTyp == SC_B_SPLINE_SURFACE_WITH_KNOTS) goto L_ir3;
  }
  ir2 = -1;  // es gibt keine knots.

  L_ir3:
  for(i1=0;i1<3;++i1) {
    ir3 = iap[i1];
    if(s_tab[ir3].sTyp == SC_RATIONAL_B_SPLINE_SURFACE) goto L_ir_;
  }
  TX_Error("STP_r_creSplBC E0I3"); return -1;


  L_ir_:
    // printf(" ir1=%d ir2=%d ir3=%d\n",ir1,ir2,ir3);






  //================================================================
  iap = s_tab[ir1].sDat;        // B_SPLINE_SURFACE


  su1.degV = iap[0];
  ++iap;
  su1.degU = iap[0];
  ++iap;
    // printf(" degu=%d degV=%d\n",su1.degU,su1.degV);


  su1.ptUNr = iap[0];
  ++iap;
  su1.ptVNr = iap[0];
  ++iap;
    // printf(" ptUNr=%d ptVNr=%d\n",su1.ptUNr,su1.ptVNr);


  // Tempspace
  UME_init (&s_mSpc, memspc501, sizeof(memspc501));


  //----------------------------------------------------------------
  // reserve ptUNr*ptVNr points in s_mSpc
  su1.cpTab = s_mSpc.next;
  iNr = su1.ptUNr * su1.ptVNr;
  irc = UME_add (&s_mSpc, sizeof(Point) * iNr);
  if(irc < 0) {TX_Error("STP_r_creSur9 EOM1"); return -4;}

  // load and copy points
  ipt = sInd;
  for(i1=0; i1<iNr; ++i1) {
    iap = STP_r_getInt (&lpt, iap);
    ipt = STP_r_findInd (lpt, ipt);
    if(ipt < 0) return -2;
      // printf(" %d lpt=%d ipt=%d\n",i1,lpt,ipt);
    STP_r_PT_CARTPT (&su1.cpTab[i1], ipt);  // copy point
      // STP_r_dispSym1 (52, &su1.cpTab[i1]);
      // DEB_dump_obj__ (Typ_PT, &su1.cpTab[i1], "P[%d]",i1);
  }



  //================================================================
  if(ir2 < 0) {                // keine B_SPLINE_SURFACE_WITH_KNOTS
    // es gibt keine knots;
    // create & copy knotvektor
    irc = UT3D_knotvec_m (&su1.kvTabV, &su1.v0V, &su1.v1V,
                          su1.ptVNr, su1.degV, &s_mSpc);
    if(irc < 0) return irc;

    irc = UT3D_knotvec_m (&su1.kvTabU, &su1.v0U, &su1.v1U,
                          su1.ptUNr, su1.degU, &s_mSpc);
    if(irc < 0) return irc;

    goto L_weights;
  }


  iap = s_tab[ir2].sDat;        // B_SPLINE_SURFACE_WITH_KNOTS

  // anzahl der u-multipicity-values --> uNr
  // startaddress u-multipicity-values --> uia
  iap = STP_r_getInt (&uNr, iap);  // get nr of u-ints
  // skip over u-multipicity-values
  uia = iap;                       // startaddres u-iTab;
  iap = &iap[uNr];                 // skip over u-iTab

  // anzahl der v-multipicity-values --> vNr
  // startaddress v-multipicity-values --> via
  iap = STP_r_getInt (&vNr, iap);  // get nr of v-ints
  // skip over u-multipicity-values
  via = iap;                       // startaddres v-iTab;
  iap = &iap[vNr];                 // skip over v-iTab
    // printf(" uNr=%d vNr=%d\n",uNr,vNr);

  // iap ist nun Startadreese des knot-value-block


  //----------------------------------------------------------------
  // reserve u-knotVals in s_mSpc
  i1 = su1.ptVNr + su1.degV + 1;
  su1.kvTabV = s_mSpc.next;
  irc = UME_add (&s_mSpc, sizeof(double) * i1);
  if(irc < 0) {TX_Error("STP_r_creSur9 EOM2"); return -4;}
    // printf(" nr of kvVr=%d\n",i1);


  // load and copy u-knotvalues
  iap = STP_r_getInt (&i1, iap);  // get nr of u-knotVals
  // printf(" nr of u-knotValues=%d\n",i1);
  if(i1 != uNr) {TX_Error("STP_r_creSur9 E0U1"); return -1;}
  i3 = 0;
  for(i1=0; i1<uNr; ++i1) {
    uia = STP_r_getInt (&ii, uia); // get next multiplicity
    // printf(" u-%d multip=%d i3=%d\n",i1,ii,i3);
    iap = STP_r_getDb (&d1, iap);  // get next knotval
    // printf(" u-%d kVal=%f ii=%d\n",i1,d1,ii);
    for(i2=0; i2<ii; ++i2) {
      // printf(" v%d",i3);
      // copy knotvalue -->cv1.kvTab[i1]
      su1.kvTabV[i3] = d1;
      ++i3;
    }
    // printf(" \n",i3);
  }



  //----------------------------------------------------------------
  // reserve v-knotVals in s_mSpc
  i1 = su1.ptUNr + su1.degU + 1;
  su1.kvTabU = s_mSpc.next;
  irc = UME_add (&s_mSpc, sizeof(double) * i1);
  if(irc < 0) {TX_Error("STP_r_creSur9 EOM3"); return -4;}
    // printf(" Nr of kvU=%d\n",i1);


  // load and copy v-knotvalues
  iap = STP_r_getInt (&i1, iap);  // get nr of v-knotVals
  // printf(" nr of v-knotValues=%d\n",i1);
  if(i1 != vNr) {TX_Error("STP_r_creSur9 E0V1"); return -1;}
  i3 = 0;
  for(i1=0; i1<vNr; ++i1) {
    via = STP_r_getInt (&ii, via); // get next multiplicity
    // printf(" v-%d multip=%d\n",i1,ii);
    iap = STP_r_getDb (&d1, iap);  // get next knotval
    // printf(" v-%d kVal=%f\n",i1,d1);
    for(i2=0; i2<ii; ++i2) {
      // printf(" u%d",i3);
      // copy knotvalue -->cv1.kvTab[i1]
      su1.kvTabU[i3] = d1;
      ++i3;
    }
    // printf(" \n",i3);
  }





  //================================================================
  L_weights:
  if(ir3 < 0) {TX_Error("STP_r_creSur9 E0U0"); return -1;}
  iap = s_tab[ir3].sDat;        // RATIONAL_B_SPLINE_SURFACE

  // has ptVNr Blocks of ptUNr Values.
  // squareBlock - first the dimensions

  iap = STP_r_getInt (&i1, iap); // get nr of doubles
  iap = STP_r_getInt (&i2, iap);
    // printf(" i1=%d i2=%d\n",i1,i2);
  if(i1 != su1.ptUNr) {TX_Error("STP_r_creSur9 E0U2"); return -1;}
  if(i2 != su1.ptVNr) {TX_Error("STP_r_creSur9 E0V2"); return -1;}

  // reserve weigths in s_mSpc

  iNr = su1.ptUNr * su1.ptVNr;

  su1.wTab = s_mSpc.next;
  irc = UME_add (&s_mSpc, sizeof(double) * iNr);
  if(irc < 0) {TX_Error("STP_r_creSur9 EOM4"); return -4;}
    // printf(" nr of kvVr=%d\n",i1);


  // load and copy u-knotvalues
  for(i1=0; i1<iNr; ++i1) {
    iap = STP_r_getDb (&d1, iap);  // get next knotval
    // printf(" u-%d wVal=%f ii=%d\n",i1,d1,ii);
    su1.wTab[i1] = d1;
  }



  //----------------------------------------------------------------
    // TEST:
    // DEB_dump_obj__ (Typ_SURRBSP, &su1, "Step-su1");

  // testen, ob man Rat.B-SplSurf degraden kann
  // create object, save obj
  irc = UT3D_obj_cnvt_srbsp (&o1, &su1, &s_mSpc);
  if(irc >= 0) {
    irc = STP_r_creObj1 (sInd, o1.typ, o1.form, o1.data);

  } else {
    irc = STP_r_creObj1 (sInd, Typ_SURRBSP, Typ_SURRBSP, &su1);
  }

  if(irc < 0) return irc;

    // if((su1.ptVNr + su1.degV + 1) == 17) return -4;


  // return -4; // TEST
  return 0;

}




//================================================================
  int STP_r_cir_ck1 (int *gc, int is) {
//================================================================
// test obj for being a circle
// RetCod: 0=yes, i1 is a circle; -1=not.

  int     i1, i2, ii, iNr, *iap;

  printd("STP_r_cir_ck1 %d #%d typ=%d\n",is,s_tab[is].sInd,s_tab[is].gTyp);

  // circle - OK
  if(s_tab[is].gTyp == Typ_CI) {
    *gc = s_tab[is].gInd;
      printd(" Circ %d gInd=%d\n",is,s_tab[is].gInd);
    return 0;
  }

  // check also CCV (can consist of 2 circles)
  if(s_tab[is].gTyp != Typ_CVTRM) return -1;


// see also STP_r_creCont1
  if((s_tab[is].sTyp != SC_FACE_OUTER_BOUND) &&
     (s_tab[is].sTyp != SC_FACE_BOUND))         {
    return -1;
  }

  // get EDGE_LOOP from FACE_OUTER_BOUND
  iap = s_tab[is].sDat;
  ii = STP_r_findInd (iap[0], 0);


  if(s_tab[ii].sTyp != SC_EDGE_LOOP)     {
    return -1;
  }


  // test subParts of CCV if all are circles
  iap = s_tab[ii].sDat;
  iNr = iap[0];
  ++iap;

  for(i1=0; i1<iNr; ++i1) {
    i2 = STP_r_findInd (iap[i1], 0);
      printd(" CCVseg %d %d #%d typ=%d gInd=%d\n",i1,i2,s_tab[i2].sInd,
              s_tab[i2].gTyp,s_tab[i2].gInd);
    if(s_tab[i2].gTyp != Typ_CI) return -1;
    *gc = s_tab[i2].gInd;
  }

  return 0;

}


//==========================================================================
  int STP_r_SurTor_cre1 (int ib, int ic1, int ic2, int gc1, int gc2) {
//==========================================================================
// create SRV-surface for torus from2 circles
// SRV axis contour point1 point2
// Input:
//   ib   torus-body (s_tab-index)
//   ic1  contour1 (s_tab-index)
//   ic2  contour2 (s_tab-index)
//   gc1  circle1  (gcad-index)
//   gc2  circle2  (gcad-index)
 
// see also STP_r_creSur5

  int      lax, iax, ipo, ivz, *iap;
  void     *vp;
  double   rd1, rd2;
  char     sax[32], sco[32];


  printd("STP_r_creSurTo1 %d %d %d\n",ib,ic1,ic2);


  // get mainaxis of torus
  vp = STP_r_getInt (&lax, s_tab[ib].sDat);
  vp = STP_r_getDb (&rd1, vp);
  vp = STP_r_getDb (&rd2, vp);
    // printd(" lax,rd1,rd2=%d %lf %lf\n",lax,rd1,rd2);

  iax = i_tab[lax];
  iap = s_tab[iax].sDat;
  ipo = STP_r_findInd (iap[0], 0);   // origin
  ivz = STP_r_findInd (iap[1], 0);   // Z-vec
    // printd(" ipo,ivz=%d %d\n",ipo,ivz);


  //----------------------------------------------------------------
  // create axis  sax
  STP_r_cre2 (ipo);  // create origin
  STP_r_cre2 (ivz);  // create Z-Vec

  sax[0] = '\0';
  AP_obj_add_obj (sax, s_tab[ipo].gTyp, s_tab[ipo].gInd);
  AP_obj_add_obj (sax, s_tab[ivz].gTyp, s_tab[ivz].gInd);



  // create radius f contour
  sco[0] = '\0';
  AP_obj_add_val (sco,rd2);



  //----------------------------------------------------------------
  sprintf(gTxt,"SRV L(%s) C(P(C%d) %s) P(C%d) P(C%d) CW",
         sax,                                                  // axis
         gc1, sco,                                             // contour
         gc1,gc2);      // points

  return 0;

}


//================================================================
  int STP_r_crePln1 (int sInd) {
//================================================================
// SC_AXIS2_PLACEMENT_3D  (Origin Z-Achse X-Achse)

 // R = ori  Z-vec Punkt vom ori auf X-achse
// R = P#     D#     P(..)            

  int    irc, ipo, ivz, ivx;
  void   *vp;
  Plane  pl1;
  Point  pt1;
  Vector vc1, vc2;



  // strcpy(gTxt, "Z(100)");

  vp  = STP_r_getInt (&ipo, s_tab[sInd].sDat);
  ipo = STP_r_findInd (ipo, sInd);
    if(ipo < 0) return -2;

  vp  = STP_r_getInt (&ivz, vp);
  ivz = STP_r_findInd (ivz, sInd);
    if(ivz < 0) return -2;

  vp  = STP_r_getInt (&ivx, vp);
  ivx = STP_r_findInd (ivx, sInd);
    if(ivx < 0) return -2;

  // printf("========================================\n");
  // printf("STP_r_crePln1 %d %d %d %d\n",sInd,ipo,ivz,ivx);

  // create origin ..
  STP_r_cre2 (ipo);

  // create Z-vec
  STP_r_cre2 (ivz);

  // create X-vec
  STP_r_cre2 (ivx);

  // gTxt[0] = '\0';
  strcpy(gTxt, "PERP");


  // add the origin
  AP_obj_add_obj (gTxt, s_tab[ipo].gTyp, s_tab[ipo].gInd);

  // add the z-vec
  // AP_obj_add_obj (gTxt, s_tab[ivz].gTyp, s_tab[ivz].gInd);
  STP_r_add_vec (ivz);

  // add the x-vec
  // AP_obj_add_obj (gTxt, s_tab[ivx].gTyp, s_tab[ivx].gInd);
  STP_r_add_vec (ivx);

  // printf(" R? = |%s|\n",gTxt);

  // make obj, save-> gCad, stor gC-Typ & Index -> s_tab
  irc = STP_r_creObj1 (sInd, Typ_PLN, Typ_Txt, gTxt);
    // printf(" created: %d %d %d\n",sInd,s_tab[sInd].gTyp,s_tab[sInd].gInd);
  if(irc < 0) return irc;


  if(resMod == 2)     // hide bei Solids
    // GA_hide__ (0, s_tab[sInd].gInd, s_tab[sInd].gTyp);
    GA_view__ (-1L, 1, s_tab[sInd].gTyp, s_tab[sInd].gInd);


  return 0;

}


//================================================================
  int STP_r_mdl_res__ (int iAct) {
//================================================================
// get s_tab-index of PRODUCT from any s_tab-index; (resolv revers !)
// resolve step: see also STP_r_mdl_geo and STP_r_cre2
// Input:
//   iAct      s_tab-index of record inside the product to find
// Output:
//   retCod  s_tab-index of PRODUCT


// #604=NEXT_ASSEMBLY_USAGE_OCCURRENCE('','','',#17,#56,' ') ;
// #56=PRODUCT_DEFINITION_WITH_ASSOCIATED_DOCUMENTS('Part1','',#55,#3,(#58)) ;
// #55=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE('','',#54,.NOT_KNOWN.) ;
// #54=PRODUCT('Part1','','',(#2)) ;


// #52=GEOMETRICALLY_BOUNDED_SURFACE_SHAPE_REPRESENTATION('NONE',(#53),#46) ;
// #54=SHAPE_REPRESENTATION_RELATIONSHIP(' ',' ',#49,#52)
// #50=SHAPE_DEFINITION_REPRESENTATION(#40,#49)
// #40=PRODUCT_DEFINITION_SHAPE(' ',' ',#14)
// #14=PRODUCT_DEFINITION(' ',' ',#6,#3)
// #6=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE('',' ',#5,
// #5=PRODUCT('Part1','','',(#2)) ;

  int    l1, l2, li, i1, *ia, lAct, typ_l1, ii, ll;
  int    i2;


  lAct = s_tab[iAct].sInd;

  printd("STP_r_mdl_res__ %d #%d typ=%d\n",iAct,s_tab[iAct].sInd,s_tab[iAct].sTyp);


  switch (s_tab[iAct].sTyp) {
    // iAct      s_tab-index of record inside the product to find
    // lAct      the step-index of rec. iAct


    //----------------------------------------------------------------
    case SC_PRODUCT:
      // found PRODUCT; return 
      break;


    //----------------------------------------------------------------
    case SC_NEXT_ASSEMBLY_USAGE_OCCURR:
      // // get the child-Link L2 (PRODUCT_DEFINITION)
      // l1 = STP_r_get_L2 (iAct);
      // get the parent-Link L1 (PRODUCT_DEFINITION)
      goto L_res_l1;


    //----------------------------------------------------------------
    case SC_PRODUCT_DEFINITION:
      // PRODUCT_DEFINITION_FORMATION = Link1 of PRODUCT_DEFINITION
      // get L1=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE
      goto L_res_l1;


    //----------------------------------------------------------------
    // PRODUCT_DEFINITION_FORMATION and
    // PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE
    case SC_PRODUCT_DEFINITION_FORMWSS:
      // PRODUCT = Link1 of PRODUCT_DEFINITION_FORMATION
      // get L1=PRODUCT
      goto L_res_l1;




    //----------------------------------------------------------------
    // PRODUCT_DEFINITION_WITH_ASSOCIATED_DOCUMENTS
    case SC_PRODUCT_DEFINITION_WITH_AD:
      // get L1=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE
      goto L_res_l1;
  


    //----------------------------------------------------------------
    // #45=PRODUCT_DEFINITION_SHAPE
    case SC_PRODUCT_DEFINITION_SHAPE:
      // PRODUCT_DEFINITION = Link1 of PRODUCT_DEFINITION_SHAPE
      // get L1=PRODUCT_DEFINITION
      goto L_res_l1;


    // //----------------------------------------------------------------
    // case SC_SHAPE_REPRES_RS:
      // // get L1 of SHAPE_DEFINITION_REPRESENTATION = PRODUCT_DEFINITION_SHAPE
      // l1 = STP_r_get_L1 (iAct);
      // return STP_r_mdl_res__ (i_tab[l1]);


    //----------------------------------------------------------------
    // SHAPE_DEFINITION_REPRESENTATION
    case SC_SHAPE_DEFINITION_REPRES:
      // PRODUCT_DEFINITION_SHAPE = Link1 of SHAPE_DEFINITION_REPRESENTATION
      // get L1=PRODUCT_DEFINITION_SHAPE
      goto L_res_l1;


    //----------------------------------------------------------------
    // SHAPE_REPRESENTATION_RELATIONSHIP
    case SC_SHAPE_REPRES_RS:
      // direction of links of SC_SHAPE_REPRES_RS is undefined; test both ..
      // get l1 = L1 of this SC_SHAPE_REPRES_RS
      l1 = STP_r_get_L1 (iAct);
      li = l1;
      // test if l1 = SC_ADVANCED_BREP_SR
        // printf(" SHAPE_REPRES_RS: l1=%d ind=%d typ=%d\n",l1,i2,s_tab[i2].sTyp);
      // get typ_l1 = typ of Link1
      // i2 = i_tab[l1]; typ_l1 = s_tab[i2].sTyp;   
      typ_l1 = STP_r_get_typ_L (l1);
        printd(" SC_SHAPE_REPRES_RS: typ_l1=%d\n",typ_l1);
      if((typ_l1 == SC_ADVANCED_BREP_SR)        ||
         (typ_l1 == SC_GBOUNDED_WIREFRAME_SR))      {
        l2 = STP_r_get_L2 (iAct);
        // would be loop; find SHAPE_DEFINITION_REPRESENTATION with L2=l2
        li = l2;
      }
      // find SHAPE_DEFINITION_REPRESENTATION where
      //   stepLink = L1|L2 of SHAPE_REPRESENTATION_RELATIONSHIP
      errTyp = 1;  // do not raise TX_Print
      ii = STP_r_find_sRec_TypL2 (SC_SHAPE_DEFINITION_REPRES, li);
      if(ii > 0) goto L_res_ii;

      // Link2 of SHAPE_DEFINITION_REPRESENTATION =
      //   Link2 of SHAPE_REPRESENTATION_RELATIONSHIP

      // find SHAPE_DEFINITION_REPRESENTATION where
      //   its Link2 == (Link2 of active SHAPE_REPRESENTATION_RELATIONSHIP)
      ii = STP_r_find_sRec_TypL2 (SC_SHAPE_DEFINITION_REPRES, li);
      if(ii > 0) goto L_res_ii;
      return -1;



    //----------------------------------------------------------------
    case SC_GBOUNDED_SURFACE_SR:
      // find SHAPE_REPRESENTATION_RELATIONSHIP where
      //   L2 = GEOMETRICALLY_BOUNDED_SURFACE_SR
      ii = STP_r_find_sRec_TypL2 (SC_SHAPE_REPRES_RS, s_tab[iAct].sInd);
      if(ii > 0) goto L_res_ii;
      return -1;


    //----------------------------------------------------------------
/*
    case SC_MANIFOLD_SURFACE_SR: // MANIFOLD_SURFACE_SHAPE_REPRESENTATION
      // get L1=PRODUCT_DEFINITION
      l1 = STP_r_get_L1 (iAct);
      return STP_r_mdl_res__ (i_tab[l1]);
*/



    //----------------------------------------------------------------
    case SC_MANIFOLD_SURFACE_SR: // MANIFOLD_SURFACE_SHAPE_REPRESENTATION
      // find SHAPE_REPRESENTATION_RELATIONSHIP where L2 = iAct
    case SC_ADVANCED_BREP_SR:
    case SC_GBOUNDED_WIREFRAME_SR:
      // find SHAPE_REPRESENTATION_RELATIONSHIP where
      //   L2 = ADVANCED_BREP_SHAPE_REPRESENTATION
      errTyp = 1;  // do not raise TX_Print
      ii = STP_r_find_sRec_TypL2 (SC_SHAPE_REPRES_RS, s_tab[iAct].sInd);
      if(ii > 0) goto L_res_ii;

      // find SHAPE_DEFINITION_REPRESENTATION where
      //   L2 = ADVANCED_BREP_SHAPE_REPRESENTATION
      errTyp = 1;  // do not raise TX_Print
      i1 = STP_r_find_sRec_TypL2 (SC_SHAPE_REPRES_RS, s_tab[iAct].sInd);
      errTyp = 1;  // do not raise TX_Print
      ii = STP_r_find_sRec_TypL2 (SC_SHAPE_DEFINITION_REPRES, s_tab[iAct].sInd);
      if(ii > 0) goto L_res_ii;

      // find SHAPE_REPRESENTATION_RELATIONSHIP where
      //   L1 = ADVANCED_BREP_SHAPE_REPRESENTATION        (eg assy4.stp #4201)
      ii = STP_r_find_sRec_TypL1 (SC_SHAPE_REPRES_RS, s_tab[iAct].sInd);
      if(ii > 0) goto L_res_ii;
      return -1;


    //----------------------------------------------------------------
    default:
      goto L_err1;
  }


  return (iAct);


  L_res_ii:  // resolv next index
    return STP_r_mdl_res__ (ii);

  L_res_l1:  // resolv next step-Link1 of iAct
    ll = STP_r_get_L1 (iAct);

  L_res_ll:  // resolv next step-link
    return STP_r_mdl_res__ (i_tab[ll]);  // s_tab-index from step-link


  L_err1:
    TX_Print("****** STP_r_mdl_res__ E001 %d #%d",iAct,s_tab[iAct].sInd);
    return -1;

}


//================================================================
  int STP_r_mdl_pos (int is) {
//================================================================
// returns the position & orientation (AXIS2_PLACEMENT_3D) from s_tab-index.
// NEXT_ASSEMBLY_USAGE_OCCURRENCE - return child-axis !

  // find CONTEXT_DEPENDENT_SHAPE_REPRESENTATION where
  //     L2 = PRODUCT_DEFINITION_SHAPE
  // get REPRESENTATION_RELATIONSHIP from L1 of CONTEXT_DEPENDENT_SHAPE_REP.
  //   (L1 = index of "(REPRESENTATION_RELATIONSHIP("
  // ITEM_DEFINED_TRANSFORMATION = L1 of REPRESENTATION_RELATIONSHIP_WITH_TRANSF.
  // get L3 of REPRESENTATION_RELATIONSHIP = ITEM_DEFINED_TRANSFORMATION
  // get Transformation (2. Link of ITEM_DEFINED_TRANSFORMATION)


// #99=NEXT_ASSEMBLY_USAGE_OCCURRENCE( ,#12,#57,
// #100=PRODUCT_DEFINITION_SHAPE('L1_2','L1_2',#99);
// #109=(REPRESENTATION_RELATIONSHIP('L1_2','L1_2',#93,#48)
// #95=AXIS2_PLACEMENT_3D('',#96,#97,#98);


// #1588=PRODUCT('ROD','ROD','NOT SPECIFIED',(#1581));
// #1595=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE( #1588,
// #1850=PRODUCT_DEFINITION('design','',#1595,#1574);
// #1866=PRODUCT_DEFINITION_SHAPE('','SHAPE FOR ROD.',#1850);
// #1885=NEXT_ASSEMBLY_USAGE_OCCURRENCE( #1851,#1850,  // main,ditto
// // SECOND PRODUCT_DEFINITION_SHAPE - for the placement !
// #1867=PRODUCT_DEFINITION_SHAPE('Placement of ROD',#1885);
// #1623=CONTEXT_DEPENDENT_SHAPE_REPRESENTATION(#1614,#1867)        // open
// #1614=(REPRESENTATION_RELATIONSHIP_WITH_TRANSFORMATION(#1605)
// #1605=ITEM_DEFINED_TRANSFORMATION('','',#866,#873)    // use axis 866 !


  int    i1, i2, l1, l2, *ia;


  if(is < 0) return (is);

  printd("STP_r_mdl_pos %d #%d\n",is,s_tab[is].sInd);
  
  switch (s_tab[is].sTyp) {

    
    //----------------------------------------------------------------
    case SC_AXIS2_PLACEMENT_3D:
      // found; return 
      return (is);


    //----------------------------------------------------------------
    // NEXT_ASSEMBLY_USAGE_OCCURRENCE
    case SC_NEXT_ASSEMBLY_USAGE_OCCURR:
      // find PRODUCT_DEFINITION_SHAPE where L1=NEXT_ASSEMBLY_USAGE_OCCURRENCE
      i1 = STP_r_find_sRec_TypL1 (SC_PRODUCT_DEFINITION_SHAPE, s_tab[is].sInd);
      return STP_r_mdl_pos (i1);


    //----------------------------------------------------------------
    // PRODUCT_DEFINITION_SHAPE
    case SC_PRODUCT_DEFINITION_SHAPE:
      mdl.iProDefSha = is;
        printd("  set mdl.iProDefSha %d\n",is);
      // find CONTEXT_DEPENDENT_SHAPE_REPRESENTATION
      //   where L2=PRODUCT_DEFINITION_SHAPE
      i1 = STP_r_find_sRec_TypL2 (SC_CONTEXT_DEPENDENT_SR, s_tab[is].sInd);
      if(i1 < 0) return i1;
      // get REPRESENTATION_RELATIONSHIP_WITH_TRANSFORMATION
      //   from L1 of the CONTEXT_DEPENDENT_SHAPE_REPRESENTATION
      l2 = STP_r_get_L1 (i1);
      return STP_r_mdl_pos (i_tab[l2]);


    //----------------------------------------------------------------
    // REPRESENTATION_RELATIONSHIP_WITH_TRANSFORMATION
    case SC_REPRESENTATION_RELATIONSHIP:
      // get 3.link of SC_REPRESENTATION_RELATIONSHIP -
      //   this is L1 of REPRESENTATION_RELATIONSHIP_WITH_TRANSFORMATION
      //   this is the ITEM_DEFINED_TRANSFORMATION
      l1 = STP_r_get_L3 (is);
      return STP_r_mdl_pos (i_tab[l1]);


    //----------------------------------------------------------------
    // ITEM_DEFINED_TRANSFORMATION
    case SC_ITEM_DEFINED_TRANSFORMATION:
      // get L1 of ITEM_DEFINED_TRANSFORMATION = AXIS2_PLACEMENT_3D
      if(wrong_ASS == 0) {
        i1 = STP_r_get_L2 (is);                  // 1=child-axis
      } else {
        i1 = STP_r_get_L1 (is);                  // 0=parent-axis
      }
      return STP_r_mdl_pos (i_tab[i1]);


    //----------------------------------------------------------------
    default:
      goto L_err1;
  }


  return (is);


  L_err1:
    TX_Print("****** STP_r_mdl_pos E001 %d #%d",is,s_tab[is].sInd);
    return -1;

}


//================================================================
  int STP_r_mdl_chd ( int iNxtAss) {
//================================================================
// get s_tab-index of childmodel of NEXT_ASS

  int    l1, i1;


  // get the child-Link L2 (PRODUCT_DEFINITION)
  l1 = STP_r_get_L2 (iNxtAss);
  i1 = i_tab[l1];
  
  printd("ex STP_r_mdl_chd %d #%d\n",i1,l1);

  return i1;

}

      
//================================================================
  int STP_r_creDit3 (int iNxtAss) {
//================================================================
// create Ditto;
// iNxtAss    = s_tab-index of NEXT_ASSEMBLY_USAGE_OCCURRENCE
// iProDefSha = s_tab-index of PRODUCT_DEFINITION_SHAPE of this ditto
      
      
  // int    irc, *ia, ii, i1, l1, i2, l2, i3, l3, i4, l4, icm;
  int    irc, *ia, i1, i4, icm;
  char   *p1;
        
          
  printd("STP_r_creDit3 PDS: %d #%d\n",iNxtAss,s_tab[iNxtAss].sInd);


  //----------------------------------------------------------------
  // get s_tab-index of childmodel of NEXT_ASS
  i1 = STP_r_mdl_chd (iNxtAss);

  // // i1 = STP_r_mdl_res__ (iNxtAss);
  // p1 = STP_r_mdl_nam__ (i1);


  // get s_tab-index of PRODUCT of childModel
  icm = STP_r_mdl_res__ (i1);
    printd(" ref-childModel = %d #%d\n",icm,s_tab[icm].sInd);


  // test if PRODUCT is valid
  irc = STP_r_mdl_valid (s_tab[icm].sInd);
  if(irc) return -1;


  // mark used
  s_tab[iNxtAss].stat = 2;


  // get modelname of childModel
  p1 = STP_r_mdl_nam__ (icm);



  // get position/orientation of child
  i4 = STP_r_mdl_pos (iNxtAss);
  if(i4 < 0) return irc;
    printd(" axis: %d #%d\n",i4,s_tab[i4].sInd);


  // // get modelName of child of iNxtAss
  // p1 = STP_r_mdl_nam__ (mdl.iProDefSha);


  // add the detailname
  sprintf(gTxt, "\"%s\"",p1);



  // add the origin of the refsys iAx (Origin / Z-Achse / X-Achse) to gTxt
  ia = s_tab[i4].sDat;
  if(STP_r_addPT (ia[0]) < 0) return -2;
  if(STP_r_addVC (ia[1]) < 0) return -2;
  if(STP_r_addVC (ia[2]) < 0) return -2;
    printd(" creDit3 |%s|\n",gTxt);


  // make obj, save-> gCad, stor gC-Typ & Index -> s_tab
  irc = STP_r_creObj1 (iNxtAss, Typ_Model, Typ_Txt, gTxt);
  if(irc < 0) return irc;


  return 0;

}


//================================================================
  int STP_r_creDit2 (int iProDefSha, int iNxtAss) {
//================================================================
// create Ditto;
// iNxtAss    = s_tab-index of NEXT_ASSEMBLY_USAGE_OCCURRENCE
// iProDefSha = s_tab-index of PRODUCT_DEFINITION_SHAPE of this ditto


  int    irc, *ia, ii, i1, l1, i2, l2, i3, l3, i4, l4;
  char   *p1;


  p1 = STP_r_mdl_nam__ (iProDefSha);

  printd("STP_r_creDit2 PDS: %d #%d NA: %d #%d |%s|\n",
         iProDefSha,s_tab[iProDefSha].sInd,
         iNxtAss,   s_tab[iNxtAss].sInd, p1);


  // add the detailname
  sprintf(gTxt, "\"%s\"",p1);


  //----------------------------------------------------------------
  // mark used
  s_tab[iNxtAss].stat = 2;



  //----------------------------------------------------------------
  // get transformation for models
  // i4 = STP_r_mdl_pos (iNxtAss);       // new meth
  i4 = STP_r_mdl_pos (iProDefSha);       // old meth
  if(i4 < 0) return i4;
    printd(" axis: %d #%d\n",i4,s_tab[i4].sInd);


  // add the origin of the refsys iAx (Origin / Z-Achse / X-Achse) to gTxt
  ia = s_tab[i4].sDat;
  if(STP_r_addPT (ia[0]) < 0) return -2;
  if(STP_r_addVC (ia[1]) < 0) return -2;
  if(STP_r_addVC (ia[2]) < 0) return -2;
    printd(" creDit2 |%s|\n",gTxt);


  // make obj, save-> gCad, stor gC-Typ & Index -> s_tab
  irc = STP_r_creObj1 (iNxtAss, Typ_Model, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  return 0;

}

/*
//================================================================
  int STP_r_creDit1 (int ii) {
//================================================================
// Ditto; ii = index of "(REPRESENTATION_RELATIONSHIP("

  int    irc, i1, iDet, lTra, iTra, *ia;
  char   *detNam;


  // printf("STP_r_creDit1 %d\n",ii);

  ia = s_tab[ii].sDat;

  // Link to the Detail
  if(wrong_ASS == 0) iDet = ia[0];   // normal
  else               iDet = ia[1];   // verkehrt (CATIA-V4)

  // 3. Link = Link to the Transformation
  lTra = ia[2];
    // printf(" iDet=%d iTra=%d\n",iDet,iTra);


  // find Detailname of Detail iDet from SHAPE_REPRESENTATION
  irc = STP_r_findDetNam (&detNam, iDet);
  if(irc < 0) return irc;


  iTra = STP_r_findInd (lTra, ii);
  if(iTra < 0) return -2;

  // get Axis from Transformation (2. Link of ITEM_DEFINED_TRANSFORMATION)
  ia = s_tab[iTra].sDat;
  iTra = STP_r_findInd (ia[1], ii);
  if(iTra < 0) return -2;
    // printf(" iAx=%d\n",iTra);


  sprintf(gTxt, "\"%s\"",detNam);


  // add the origin of the refsys iAx (Origin / Z-Achse / X-Achse)
  ia = s_tab[iTra].sDat;
  if(STP_r_addPT (ia[0]) < 0) return -2;
  if(STP_r_addVC (ia[1]) < 0) return -2;
  if(STP_r_addVC (ia[2]) < 0) return -2;


  // printf(" creDit1 |%s|\n",gTxt);

  // make obj, save-> gCad, stor gC-Typ & Index -> s_tab
  irc = STP_r_creObj1 (ii, Typ_Model, Typ_Txt, gTxt);
  if(irc < 0) return irc;

  return 0;

}
*/

//================================================================
  int STP_r_addPT (int ii) {
//================================================================
// STP_r_addPT            add P5 or P(0 0 5) --> gTxt

  // printf("STP_r_addPT %d\n",ii);

  ii = STP_r_findInd (ii, ii); // origin
  if(ii < 0) return -2;

  // if( != POINT) ...

  if(s_tab[ii].gInd >= 0) {
    AP_obj_add_obj (gTxt, Typ_PT, s_tab[ii].gInd);
  } else {
    AP_obj_add_pt (gTxt, s_tab[ii].sDat);
  }

  // printf(" ex STP_r_addPT |%s|\n",gTxt);

  return 0;

}


//================================================================
  int STP_r_addVC (int ii) {
//================================================================
// STP_r_addVC            add D5 or D(0 0 5) --> gTxt

  // printf("STP_r_addVC %d\n",ii);

  ii = STP_r_findInd (ii, ii); // origin
  if(ii < 0) return -2;

  // if( != DIR) ...

  return STP_r_add_vec (ii);

}


//================================================================
  int STP_r_add_vec (int ii) {
//================================================================
// STP_r_addVC            add D5 or D(0 0 5) --> gTxt
  
  // printf("STP_r_add_vec %d\n",ii);

  
  if(s_tab[ii].gInd >= 0) {
    AP_obj_add_obj (gTxt, Typ_VC, s_tab[ii].gInd);
  } else {
    AP_obj_add_vc (gTxt, s_tab[ii].sDat);
  } 
  
  // printf(" ex STP_r_add_vec |%s|\n",gTxt);
  
  return 0;
  
}
  


//================================================================
  int STP_r_wrong_SRR () {
//================================================================
// test if SHAPE_REPRESENTATION_RELATIONSHIP is wrong;
// Normal is:
//   SHAPE_REPRESENTATION_RELATIONSHIP - L1=SHAPE_REPRESENTATION; L2=geometry;
// Wrong is (g assy3):
//   SHAPE_REPRESENTATION_RELATIONSHIP - L1=geometry; L2=SHAPE_REPRESENTATION

// if 2.link of REPRESENTATION_RELATIONSHIP = SHAPE_REPRESENTATION
//   wrong; (use parent-link of NEXT_ASSY
//   else OK; use child-link of NEXT_ASSY

// see also STP_r_ckRRwrong


  int   i1, ii, l1, *ia, iNr;


  printd("STP_r_wrong_SRR\n");
  // i1=1018;
  // printd(" rec %d #%d %d\n",i1,s_tab[i1].sInd,s_tab[i1].sTyp);

  iNr = 0;
  wrong_SRR = 0;

  for(i1=0; i1<s_Nr; ++i1) {

    // if(s_tab[i1].sTyp != SC_REPRESENTATION_RELATIONSHIP) continue;

    // own "SHAPE_REPRESENTATION_RELATIONSHIP"-record !
    if(s_tab[i1].sTyp != SC_SHAPE_REPRES_RS) continue;

    ++iNr;
    ia = s_tab[i1].sDat;
    l1 = ia[0];   // get L1
      printd(" %d #%d SRRS (,,, #%d,)\n",i1,s_tab[i1].sInd,l1);
    // find typ of Link l1
    ii = STP_r_findInd (l1, 0);
      printd(" ii=%d #%d = %s \n",ii,s_tab[ii].sInd,
           STP_r_TypTxt_sTyp(s_tab[ii].sTyp));

    // normal ist 1.Link SHAPE_REPRESENTATION; wenn der 2.Link: verkehrt !
    if(s_tab[ii].sTyp != SC_SHAPE_REPRESENTATION) {
      wrong_SRR = 1;
      goto L_wrong;
    }

    goto L_exit;
  }

  // no SHAPE_REPRESENTATION_RELATIONSHIP found !
  goto L_exit;



  //----------------------------------------------------------------
  L_wrong:   // verkehrt, alle umdrehen.
  printd("***** STP_r_wrong_SRR swap SHAPE_REPRESENTATION_RELATIONSHIP L1,L2\n");

  for(i1=0; i1<s_Nr; ++i1) {
    if(s_tab[i1].sTyp != SC_SHAPE_REPRES_RS) continue;
    ia = s_tab[i1].sDat;
    MEM_swap_int (&ia[0], &ia[1]);
  }


  L_exit:
    printd("ex STP_r_wrong_SRR iNr=%d wrong_SRR=%d\n",iNr,wrong_SRR);
    return iNr;

}

//================================================================
  int STP_r_wrong_RRS () {
//================================================================
// test if SHAPE_REPRESENTATION_RELATIONSHIP is wrong;
// Normal is:
//   SHAPE_REPRESENTATION_RELATIONSHIP - L1=SHAPE_REPRESENTATION; L2=geometry;
// Wrong is (g assy3):
//   SHAPE_REPRESENTATION_RELATIONSHIP - L1=geometry; L2=SHAPE_REPRESENTATION
  
// if 2.link of REPRESENTATION_RELATIONSHIP = SHAPE_REPRESENTATION
//   wrong; (use parent-link of NEXT_ASSY
//   else OK; use child-link of NEXT_ASSY
  
// see also STP_r_ckRRwrong


  int   i1, ii, l1, *ia;


  printd("STP_r_wrong_RRS\n");

  wrong_ASS = 0;

  for(i1=0; i1<s_Nr; ++i1) {
     
    // group-record (REPRESENTATION_RELATIONSHIP(),..
    if(s_tab[i1].sTyp != SC_REPRESENTATION_RELATIONSHIP) continue;
  
    ia = s_tab[i1].sDat;
    l1 = ia[1];   // get L2
      printd(" %d #%d RRS (,,, L2=#%d,)\n",i1,s_tab[i1].sInd,l1);
    // find typ of Link l1
    ii = STP_r_findInd (l1, 0);
      printd(" ii=%d #%d = %s \n",ii,s_tab[ii].sInd,
           STP_r_TypTxt_sTyp(s_tab[ii].sTyp));

    // normal ist 1.Link SHAPE_REPRESENTATION; wenn der 2.Link: verkehrt !
    if(s_tab[ii].sTyp != SC_SHAPE_REPRESENTATION) {
      goto L_wrong;
    }

    goto L_exit;
  }

  // no SHAPE_REPRESENTATION_RELATIONSHIP found !
  goto L_exit;


  //----------------------------------------------------------------
  L_wrong:   // verkehrt, alle umdrehen.
  printd("***** STP_r_wrong_RRS swap SHAPE_REPRESENTATION_RELATIONSHIP L1,L2\n");
  wrong_ASS = 1; // WRONG !



  L_exit:
    printd("ex STP_r_wrong_RRS wrong_ASS=%d\n",wrong_ASS);
    return 0;

}


//================================================================
  int STP_r_wrong_ASS () {
//================================================================
// check if NEXT_ASSEMBLY_USAGE_OCCURRENCE is wrong
// NEXT_ASSEMBLY_USAGE_OCCURRENCE zeigt immer main,detail
// normal:   (Detail, Master)  
// wrong:    (Master, Detail).   

// the only wrong example is ../formate/step/ErrorModels/as1_pe.stp
//   - NOT YET IMPOLEMENTED !

  int  ip1, ip2, la, l1, l2, i1, i2, i3, i4, *ia;


  printd("STP_r_wrong_ASS\n");

  // wrong_ASS = 0; // normal ..
  wrong_ASS = 1; // WRONG !

/*
  // find 1.NEXT_ASSEMBLY_USAGE_OCCURRENCE;
  //   L1 merken (zeigt auf eine PRODUCT_DEFINITION)
  for(i1=0; i1<s_Nr; ++i1) {
    if(s_tab[i1].sTyp != SC_NEXT_ASSEMBLY_USAGE_OCCURR) continue;
    ia = s_tab[i1].sDat;
    l1 = ia[0];             // parent-model of 1.NEXT_ASS.
      printf(" _wrong_ASS %d #%d ( %d,%d)\n",i1,s_tab[i1].sInd,l1,ia[1]);
    goto L_1;
  }
  // es gibt keine dittos ..
  return 0;


  L_1:
  // get index of PRODUCT of l1 = PRODUCT_DEFINITION
  ip1 = STP_r_find_Prod_ProdDef (l1);
  if(ip1 < 0) return -1;

  // printf(" ip1 of PROD = %d\n",ip1);




  //================================================================
  // find (REPRESENTATION_RELATIONSHIP( from NEXT_ASSEMBLY_USAGE_OCCURRENCE

  // #442=NEXT_ASSEMBLY_USAGE_OCCURRENCE('','','',#41,#280,' ');
  //  #448=PRODUCT_DEFINITION_SHAPE(' ',' ',#442);
  //   #450=CONTEXT_DEPENDENT_SHAPE_REPRESENTATION(#449,#448);
  //    #449=(REPRESENTATION_RELATIONSHIP(' ',' ',#277,#428) TRAFO(#447)
  //     #478=SHAPE_DEFINITION_REPRESENTATION(#44,#277);


  l1 = s_tab[i1].sInd;  // ID of NEXT_ASSEMBLY_USAGE_OCCURRENCE
  // printf(" l1=%d\n",l1);

// get index of Record with Typ sTyp dessen L1 gegeben ist
  

  // find PRODUCT_DEFINITION_SHAPE from its L1
  i1 = STP_r_find_sRec_TypL1 (SC_PRODUCT_DEFINITION_SHAPE, l1);
  if(i1 < 0) return i1;

  l2 = s_tab[i1].sInd;  // ID of PRODUCT_DEFINITION_SHAPE

  // find CONTEXT_DEPENDENT_SR(L2) from PRODUCT_DEFINITION_SHAPE
  // get index of CONTEXT_DEPENDENT_SR-Record dessen L2 gegeben ist
  i1 = STP_r_find_sRec_TypL2 (SC_CONTEXT_DEPENDENT_SR, l2);
  if(i1 < 0) return i1;

  // get ID of REPRESENTATION_RELATIONSHIP - must be like L1 of rec i1
  i1 = STP_r_find_sRec_TypIdL1 (SC_REPRESENTATION_RELATIONSHIP, i1);
  if(i1 < 0) return i1;

  // L1 of REPRESENTATION_RELATIONSHIP is the SHAPE_REPRESENTATION-ID.
  ia = s_tab[i1].sDat;
  l1 = ia[0];


  // get index of SHAPE_DEFINITION_REPRESENTATION-Record; its 
  // L2 must be the SHAPE_REPRESENTATION-ID l1.
  i1 = STP_r_find_sRec_TypL2 (SC_SHAPE_DEFINITION_REPRES, l1);
  if(i1 < 0) return i1;


  // Im SHAPE_DEFINITION_REPRESENTATION-Record wurde das 3. int-Wort
  // auf den Index des PRODUCT-Record gesetzt (von STP_r_findPROD).
  ia = s_tab[i1].sDat;
  ip2 = ia[2];
  // printf(" ip2 of PROD = %d\n",ip2);


  // Check if ip1 and ip2 both belong to the same Part;
  //   yes: verkehrt (CATIA-V4);
  //   else normal.
  if(ip1 == ip2) {
    wrong_ASS = 1;     // yes: verkehrt (CATIA-V4);
  } else {
    wrong_ASS = 0;     //   else normal
  }

*/
  printd("ex STP_r_wrong_ASS wrong_ASS=%d\n",wrong_ASS);

  return 0;

}

/*
//================================================================
  int STP_r_ckDit1 () {
//================================================================
// Problem mit 'ACIS Interop R13 SP5' - Files:
//  keine Dittos (keine REPRESENTATION_RELATIONSHIP);
// alle automatisch hinsetzen (M20='id') 

  int   irc, i1, i2, i3;
  char  *ModNam;


  // printf("STP_r_ckDit1\n");


  // if(Anzahl PRODUCT > 1)
  i2 = 0;
  for(i1=1; i1<s_Nr; ++i1) {
    if(s_tab[i1].sTyp != SC_PRODUCT) continue;
    ++i2;
    if(i2 > 1) goto L_ck_RR;
  }
  return 0;



  // Anzahl REPRESENTATION_RELATIONSHIP suchen ...
  L_ck_RR:
  i2 = 0;
  for(i1=1; i1<s_Nr; ++i1) {
    if(s_tab[i1].sTyp != SC_REPRESENTATION_RELATIONSHIP) continue;
    ++i2;
  }
  // printf(" nr SHAPE_REPRESENTATION_RS=%d\n",i2);
  if(i2 > 0) return 0;


  // es gibt keine REPRESENTATION_RELATIONSHIP !
  // fuer alle submodels automat ein Ditto generieren.



  i2 = 0;
  for(i1=0; i1<s_Nr; ++i1) {
    if(s_tab[i1].sTyp != SC_PRODUCT) continue;
    // skip first model (Master!)
    if(i2 == 0) {++i2; continue;}
    ModNam = STP_r_getInt (&i3, s_tab[i1].sDat);
    // printf(" model |%s|\n",ModNam);

    // create M20='id'
    sprintf(gTxt, "\"%s\"",ModNam);
    // make obj, save-> gCad, stor gC-Typ & Index -> s_tab
    irc = STP_r_creObj1 (0, Typ_Model, Typ_Txt, gTxt);
    if(irc < 0) return irc;
  }

  // if(Anzahl REPRESENTATION_RELATIONSHIP < 1)
    // alle PRODUCT suchen; add Model="id" for each product.




  return 0;

}
*/

//================================================================
  int STP_r_find_sRec_TypL1 (int sTyp, int lL1) {
//================================================================
// get index of Record with Typ sTyp dessen L1 gegeben ist.

// [iso]   . = sTyp (lL1 ..)

// Out: Index iso

  int     i1, *ia;

  // printf("STP_r_find_sRec_TypL1 %s %d\n",STP_r_TypTxt_sTyp(sTyp),lL1);


  for(i1=0; i1<s_Nr; ++i1) {
    if(s_tab[i1].sTyp != sTyp) continue;
    ia = s_tab[i1].sDat;
    if(ia[0] != lL1) continue;
    goto L_ok;
  }

  // es gibt nix  ..
  if(!errTyp)
    TX_Print("STP_r_find_sRec_TypL1 E001 %d #%d",sTyp,lL1);

   i1 = -1;



  L_ok:
    // { int ii=-1; if(i1>0) ii=s_tab[i1].sInd;
    // printf("ex STP_r_find_sRec_TypL1 %d #%d=%s (.., #%d\n",i1,ii,
            // STP_r_TypTxt_sTyp(sTyp),lL1); }

  errTyp = 0;     // reset

  return i1;

}


//================================================================
  int STP_r_find_sRec_TypL2 (int sTyp, int lL2) {
//================================================================
// get index of Record with Typ sTyp dessen L2 gegeben ist.

// [iso]   . = sTyp (.., lL2 ..)

// Out: Index iso

  int     i1, *ia;


  // printf("STP_r_find_sRec_TypL2 %s %d\n",STP_r_TypTxt_sTyp(sTyp),lL2);


  for(i1=1; i1<s_Nr; ++i1) {
    if(s_tab[i1].sTyp != sTyp) continue;  // 2014-09-08
    ia = s_tab[i1].sDat;
    if(ia[1] != lL2) continue;
    goto L_ok;
  }
  // es gibt nix  ..
  if(!errTyp)
    TX_Print("STP_r_find_sRec_TypL2 E001 %d #%d",sTyp,lL2);

  i1 = -1;



  L_ok:
    // { int ii=-1; if(i1>0) ii=s_tab[i1].sInd;
    // printf("ex STP_r_find_sRec_TypL2 %d #%d=%s (.., #%d\n",i1,ii,
            // STP_r_TypTxt_sTyp(sTyp),lL2); }

  errTyp = 0;     // reset

  return i1;

}


//================================================================
  int STP_r_find_sRec_TypIdL1 (int sTyp, int iL1) {
//================================================================
// get index of Record with Typ sTyp dessen ID gleich ist
// wie L1 des Record mit Index iL1

// [iL1]  lpd = sTyp (lL1 ..)
// [iso]    lL1 =

// Out: Index iso 

  int     i1, lL1, *ia;


  // printf("STP_r_find_sRec_TypIdL1 %d %d\n",sTyp,iL1);


  ia = s_tab[iL1].sDat;
  lL1 = ia[0];

  // find Record typ = sTyp
  for(i1=0; i1<s_Nr; ++i1) {
    if(s_tab[i1].sTyp != sTyp) continue;
    if(s_tab[i1].sInd != lL1) continue;
    goto L_ok;
  }

  // es gibt keinen rec ..
  TX_Print("STP_r_find_sRec_TypIdL1 E001 #%d=%d",lL1,sTyp);
  return -1;


  L_ok:
  // printf("ex STP_r_find_sRec_TypIdL1 %d #%d=%s / %d #%d\n",i1,s_tab[i1].sInd,
          // STP_r_TypTxt_sTyp(sTyp),iL1,lL1);

  return i1;

}


//================================================================
   int STP_r_find_Prod_ProdDef (int lpd) {
//================================================================
// get index of PRODUCT of #lpd=PRODUCT_DEFINITION

// [ipd] lpd=PRODUCT_DEFINITION (l1a ..)
// [idf]   l1a=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE (l1b ..)
// [ipo]     l1b=PRODUCT

// Output: ipo = Index of PRODUCT-Record of PRODUCT_DEFINITION-Record

  int    sTyp, ipd, idf, ipo, *ia;


  // printf("STP_r_find_Prod_ProdDef %d\n",lpd);

  // get index of Link lpd
  ipd = STP_r_findInd (lpd, 3); // origin
  if(ipd < 0) return -2;


  // get index of SC_PRODUCT_DEFINITION_FORMWSS-Record dessen ID gleich ist
  // wie L1 des Record mit Index ipd
  idf = STP_r_find_sRec_TypIdL1  (SC_PRODUCT_DEFINITION_FORMWSS, ipd);
  if(idf < 0) return idf;


  // get index of PRODUCT-Record dessen ID gleich ist
  // wie L1 des Record mit Index idf
  ipo = STP_r_find_sRec_TypIdL1  (SC_PRODUCT, idf);
  // if(ipo < 0) return ipo;

  // printf("ex STP_r_find_Prod_ProdDef %d %d\n",ipo,lpd);

  return ipo;

}


/*
//================================================================
  int STP_r_findPROD () {
//================================================================
// geht nur tw, weil zB:
#599=SHAPE_DEFINITION_REPRESENTATION(#598,#597);
 #598=PROPERTY_DEFINITION('','Shape for Validation Properties',#584);
   (hier sollte aber PRODUCT_DEFINITION_SHAPE stehen ..)
  #584=SHAPE_ASPECT('#584','solid #570',#38,.F.);
   #570=MANIFOLD_SOLID_BREP('#570',#569);


// Im SHAPE_DEFINITION_REPRESENTATION-Record das 3. int-Wort
// auf den Index des PRODUCT-Record (auf i1) setzen.

// i1 L1=PRODUCT
// i2  L2=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE (L1,
// i3   L3=PRODUCT_DEFINITION/PRODUCT_DEFINITION_WITH_ASSOCIATED_DOCUMENTS (L2
// i4    L4=PRODUCT_DEFINITION_SHAPE (L3
// i5     L5=SHAPE_DEFINITION_REPRESENTATION (L4,    // Index auf PRODUCT setzen


  int     irc, i1, i2, i3, i4, i5;
  int     *ip;


  printf("STP_r_findPROD \n");


  for(i5=0; i5<s_Nr; ++i5) {
    if(s_tab[i5].sTyp != SC_SHAPE_DEFINITION_REPRES) continue;


    // find PRODUCT_DEFINITION_SHAPE dessen ID==(1.Link of Rec.i5)
    i4 = STP_r_find_sRec_TypIdL1 (SC_PRODUCT_DEFINITION_SHAPE, i5);
    if(i4 < 0) {TX_Error("STP_r_findPROD E001"); continue;}


    // find PRODUCT_DEFINITION dessen ID==(1.Link of Rec.i4)
    i3 = STP_r_find_sRec_TypIdL1 (SC_PRODUCT_DEFINITION, i4);
    if(i3 < 0) {TX_Error("STP_r_findPROD E002"); continue;}


    // find PRODUCT_DEFINITION_FORMATION_WSS dessen ID==(1.Link of Rec.i3)
    i2 = STP_r_find_sRec_TypIdL1 (SC_PRODUCT_DEFINITION_FORMWSS, i3);
    if(i2 < 0) {TX_Error("STP_r_findPROD E003"); continue;}


    // find PRODUCT dessen ID==(1.Link of Rec.i2)
    i1 = STP_r_find_sRec_TypIdL1 (SC_PRODUCT, i2);
    if(i1 < 0) {TX_Error("STP_r_findPROD E004"); continue;}


    // index auf PRODUCT-Record merken
    ip = s_tab[i5].sDat;
    ip[2] = i1;
    printf("   %d set sDat[3] to %d\n",i5,i1);

  }



  for(i1=0; i1<s_Nr; ++i1) {
    if(s_tab[i1].sTyp != SC_SHAPE_DEFINITION_REPRES) continue;
    ip = s_tab[i1].sDat;
    printf(" SDR[%d]=%d\n",i1,ip[2]);
  }



  return 0;

}
*/


//================================================================
  int STP_r_findPROD () {
//================================================================
// STP_r_findPROD 

// Im SHAPE_DEFINITION_REPRESENTATION-Record das 3. int-Wort
// auf den Index des PRODUCT-Record (auf i1) setzen.

/*
// 'ST-DEVELOPER v8', (Rhino)           (Rhi1.stp)
// CATIA-V4                             (C4_LnCiElCCV.stp)
// CATIA-V5                             (C5_plg1.stp)
#17=PRODUCT('1','Rhino Product','Rhino converted to STEP',(#16));
 #12=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE('A','..',#17,.MADE.);
  #11=PRODUCT_DEFINITION('A','First version',#12,#10);
   #18=PRODUCT_DEFINITION_SHAPE('A','First version',#11);
    #19=SHAPE_DEFINITION_REPRESENTATION(#18,#29);


// ('SDRC'),'I-DEAS Master Series 9','UNIX','Yes');               (4bar.stp)
#236=PRODUCT('L4','','None',(#235));
 #238=PRODUCT_DEFINITION_FORMATION('','None',#236);
  #240=PRODUCT_DEFINITION('None','None',#238,#239);
   #241=PRODUCT_DEFINITION_SHAPE('None','None',#240);
    #277=SHAPE_DEFINITION_REPRESENTATION(#241,#276);
*/


  int     irc, ii, i1, i2, i3, i4, i5;
  int     isdr, lsdr, *ip;
  void    *vp;


  // printf("STP_r_findPROD \n");


/*
  for(i1=0; i1<s_Nr; ++i1) {
    if(s_tab[i1].sTyp != SC_SHAPE_DEFINITION_REPRES) continue;
    ip = s_tab[i1].sDat;
    ip[2] = -1;
  }
*/


  // loop tru PRODUCT's
  for(i1=0; i1<s_Nr; ++i1) {
    if(s_tab[i1].sTyp != SC_PRODUCT) continue;
    // printf(".......... nxt Model %d #%d = PRODUCT\n",i1,s_tab[i1].sInd);


    // find PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE;
    // its 1.Link must be == #PRODUCT
    for(i2=0; i2<s_Nr; ++i2) {
      if(s_tab[i2].sTyp != SC_PRODUCT_DEFINITION_FORMWSS) continue;
      STP_r_getInt (&ii, s_tab[i2].sDat); // get Link -> PRODUCT
      // printf(" %d #%d = PRODUCT_DEFINITION_FORMWSS\n",i2,s_tab[i2].sInd);
      if(ii == s_tab[i1].sInd) goto L_a2;
    }
    TX_Print("STP_r_findPROD E010");
    goto L_err1;


    L_a2:

    // find PRODUCT_DEFINITION oder PRODUCT_DEFINITION_WITH_ASSOCIATED_DOCUMENTS;
    // its 1.Link must be == #PRODUCT_DEFINITION_FWSS
    for(i3=0; i3<s_Nr; ++i3) {
      if(s_tab[i3].sTyp == SC_PRODUCT_DEFINITION) {
        STP_r_getInt (&ii, s_tab[i3].sDat); // get 1.Link -> PRODUCT
        // printf(" %d #%d = PRODUCT_DEFINITION\n",i3,s_tab[i3].sInd);
        if(ii == s_tab[i2].sInd) goto L_a3;

      } else if(s_tab[i3].sTyp == SC_PRODUCT_DEFINITION_WITH_AD) {
        STP_r_getInt (&ii, s_tab[i3].sDat); // get 1.Link -> PRODUCT
        // printf(" %d #%d = PRODUCT_DEFINITION\n",i3,s_tab[i3].sInd);
        if(ii == s_tab[i2].sInd) goto L_a3;
      }
    }
    TX_Print("STP_r_findPROD E011");
    goto L_err1;


    L_a3:


    // find PRODUCT_DEFINITION_SHAPE;
    // its Link must be == #PRODUCT_DEFINITION
    for(i4=0; i4<s_Nr; ++i4) {
      if(s_tab[i4].sTyp != SC_PRODUCT_DEFINITION_SHAPE) continue;
      STP_r_getInt (&ii, s_tab[i4].sDat); // get Link -> PRODUCT
      // printf(" %d #%d = PRODUCT_DEFINITION_SHAPE\n",i4,s_tab[i4].sInd);
      if(ii == s_tab[i3].sInd) goto L_a4;
    }
    TX_Print("STP_r_findPROD E012");
    goto L_err1;


    L_a4:


    // find SHAPE_DEFINITION_REPRESENTATION;
    // its 1.Link must be == #PRODUCT_DEFINITION_SHAPE
    // its 3.Link = Leer fuer die IndexNr des PRODUCT; setzen !
    for(i5=0; i5<s_Nr; ++i5) {
      if(s_tab[i5].sTyp != SC_SHAPE_DEFINITION_REPRES) continue;
      // printf(" %d #%d = SHAPE_DEFINITION_RS\n",i5,s_tab[i5].sInd);
      ip = s_tab[i5].sDat;
      ii = ip[0];                     // get Link -> PRODUCT_DEFINITION_SHAPE
      if(ii != s_tab[i4].sInd) continue;

      // Im SHAPE_DEFINITION_REPRESENTATION-Record das 3. int-Wort
      // auf den Index des PRODUCT-Record (auf i1) setzen.
      ip[2] = i1;
      // printf("        %d set sDat[3] to %d\n",i5,i1);
    }
    continue;


    L_err1:
    continue;

  }


  // printf("ex STP_r_findPROD \n");


/*
  for(i1=0; i1<s_Nr; ++i1) {
    if(s_tab[i1].sTyp != SC_SHAPE_DEFINITION_REPRES) continue;
    ip = s_tab[i1].sDat;
    printf(" SDR[%d]=%d\n",i1,ip[2]);
  }
*/


  return 0;

}


/*
//================================================================
  int STP_r_findDetNam (char **detNam, int lDet) {
//================================================================
// STP_r_findDetNam       find Detailname for Ditto
// lDet ist LinkNr der SHAPE_REPRESENTATION des Details.

// Find SHAPE_DEFINITION_REPRESENTATION from lDet.
// 2.Word of SHAPE_DEFINITION_REPRESENTATION is #SHAPE_REPRESENTATION
// 3.Word of SHAPE_DEFINITION_REPRESENTATION is index of PRODUCT.
// 1.Words in PRODUCT is Detailname.


  static char cbuf[32];  // wenns keinen name gibt: einen generieren ..

  int    i1, i2, ii, *ia;
  char   *p1;


  // printf("STP_r_findDetNam %d\n",lDet);


  // Find SHAPE_DEFINITION_REPRESENTATION from lDet (SHAPE_REPRESENTATION)
  // 2.Word of SHAPE_DEFINITION_REPRESENTATION is #SHAPE_REPRESENTATION
  for(i1=0; i1<s_Nr; ++i1) {
    if(s_tab[i1].sTyp != SC_SHAPE_DEFINITION_REPRES) continue;
    ia = s_tab[i1].sDat;
    ii = ia[1];                                // get Link -> Detail
    // ii = ia[0];                                // get Link -> Detail
    // printf(" %d -> PRODUCT = %d\n",i1,ii);
    if(ii == lDet) goto L_a2;
  }
  TX_Print("STP_r_findDetNam E001");
  return -1;


  L_a2:
  // printf(" %d #%d = SHAPE_DEFINITION_RS\n",i1,s_tab[i1].sInd);
  // 3.Word of SHAPE_DEFINITION_REPRESENTATION is index of PRODUCT.
  ii = ia[2];
  // printf(" ind of PROD = %d\n",ii);

  // 1.Words in PRODUCT is Detailname.
  p1 = STP_r_getInt (&i2, s_tab[ii].sDat);  // get Nr of words of modelname
    // printf(" Modelname |%s| siz=%d\n",p1,i2);

  if(strlen(p1) < 1) {
    sprintf(cbuf, "D%d",lDet);
    *detNam = cbuf;

  } else {
    *detNam = p1;
  }


  // printf("ex STP_r_findDetNam |%s|\n",*detNam);

  return 0;

}
*/


//================================================================
  int STP_r_findInd (int ii, int is) {
//================================================================
// returns s_tab-index from step-link
// find index of s-rec "#<ii>="
// is = startIndex for search      UNUSED !
// manchmal ist es der vorherige Satz; also bei is-3 beginnen.

// RetCod 0>   der Index auf den Record "#<ii>="
// RetCod -1   Error; nix gefunden !


  if((ii < 0)||(ii > i_Max)) {
    TX_Error("STP_r_findInd E002 #%d %d",ii,i_Max);
    return -1;
  }


  if(i_tab[ii] < 0) {
    TX_Error("STP_r_findInd E001 #%d",ii);
    return -1;
  }

    // printf("  ex STP_r_findInd %d %d\n",i_tab[ii],ii);

  return i_tab[ii];



/*
  int   i1;


  // printf("STP_r_findInd %d %d %d\n",ii,is,s_Nr);

  // if(*((int*)s_tab[824].sDat) != 97) printf(" i0=%d\n",*((int*)s_tab[824].sDat));


  if((ii < 0)||(ii > s_Max)) {
    TX_Error("STP_r_findInd E002 #%d %d",ii,s_Max);
    return -1;
  }

  is -= 3;
  if((is < 0)||(is >= s_Nr)) is = 0;


  for(i1=is; i1<s_Nr; ++i1) {
    if(s_tab[i1].sInd == ii) goto L_found;
  }

  // nix gefunden: suchen von 0 - is.
  if(is < 1) goto L_err;
  for(i1=0; i1<is; ++i1) {
    if(s_tab[i1].sInd == ii) goto L_found;
  }

  L_err:
  TX_Error("STP_r_findInd E001 #%d",ii);
  return -1;

  L_found:
    // printf("  ex STP_r_findInd %d %d %d\n",i1,ii,is);
  return i1;
*/
}


//================================================================
  int STP_r_Link_data (int offset, void* data) {
//================================================================
// get LinkNr <offset> out of data.
// offset = 0 returns the first integer, 1 the second ..

  return ((int*)data)[offset];

}
 

//================================================================
  int STP_r_LN_LINE (Line *ln1, int sInd) {
//================================================================
// give Line from s_tab-index of LINE
// see STP_r_creLn0
  
  int     *iap, i1, i2;
  // double  d1;
  Vector  vc1;

    
  printd("STP_r_LN_LINE %d #%d\n",sInd,s_tab[sInd].sInd);
  if(sInd >= s_Nr) return -1;

  // sInd is a LINE; LinkPoint + LinkVec
  iap = s_tab[sInd].sDat;
  i1 = STP_r_findInd (iap[0], sInd);  // PT
  i2 = STP_r_findInd (iap[1], sInd);  // DIR

  STP_r_PT_CARTPT (&ln1->p1, i1);       // get pt
  STP_r_VC_VEC_t (&vc1, i2);       // get vc

  // pt2 = from pt1 in Richtg vc1 len = d1
  UT3D_pt_traptvc (&ln1->p2, &ln1->p1, &vc1);
  // UT3D_pt_traptvclen (&ln1->p2, &ln1->p1, &vc1, d1);
    // GR_Disp_pt (&ln1->p2, SYM_TRI_S, 2);

  return 0;

}


//================================================================
  int STP_r_PLN_AXIS2 (Plane *pl1, int sInd) {
//================================================================
// give Plane from s_tab-index of AXIS2_PLACEMENT_3D
// see STP_r_decRefSys1

// Origin / Z-Achse / X-Achse

  int    i1, l1, i2, ii;
  Point  pt1;
  Vector vc1, vc2;

  printd("STP_r_PLN_AXIS2 %d #%d\n",sInd,s_tab[sInd].sInd);
  if(sInd >= s_Nr) return -1;

  // get the origin
  // i1 = STP_r_Link_data (0, s_tab[sInd].sDat); // get AXIS = 1. link of CIRC
  l1 = STP_r_get_L1 (sInd);
  ii = STP_r_findInd (l1, sInd);
  // printf(" ori=%d\n",ii);
  STP_r_PT_CARTPT (&pt1, ii);
    printd("  ori=%lf %lf %lf\n",pt1.x,pt1.y,pt1.z);
    // DEB_dump_obj__ (Typ_PT, &pt1, "  ori pt1:");


  // get the Z-vec
  // i1 = STP_r_Link_data (1, s_tab[sInd].sDat); // get zVc = 2. link of CIRC
  l1 = STP_r_get_L2 (sInd);
  if(l1 < 0) {  // Z-Vec Link = "$"
    // give Defaultplane
    // TX_Error("STP_r_PLN_AXIS2 E001");
    // try UT3D_pl_ptvc
    vc2 = UT3D_VECTOR_Z;
  } else {
    ii = STP_r_findInd (l1, sInd);
    // printf(" zVc=%d\n",ii);
    STP_r_VC_DIR (&vc2, ii);     // Z-vec --> vc2
    UT3D_vc_setLength (&vc2, &vc2, 1.);
  }
    printd("  vcZ=%lf %lf %lf\n",vc2.dx,vc2.dy,vc2.dz);
    // DEB_dump_obj__ (Typ_VC, &vc2, "  vz:");


  // get the X-vec
  // i1 = STP_r_Link_data (2, s_tab[sInd].sDat); // get xVc = 2. link of CIRC
  l1 = STP_r_get_L3 (sInd);
  if(l1 < 0) {  // X-Vec Link = "$"
    // X,yVec generieren.
    // TX_Error("STP_r_PLN_AXIS2 E002");
    // try UT3D_pl_ptvc
    vc1 = UT3D_VECTOR_X;
    // plane from Origin, Z-Vec
    UT3D_pl_ptvc (pl1, &pt1, &vc2);

  } else {
    ii = STP_r_findInd (l1, sInd);
    // printf(" zVc=%d\n",ii);
    STP_r_VC_DIR (&vc1, ii);           // X-Vec --> vc1
    UT3D_vc_setLength (&vc1, &vc1, 1.);
      // DEB_dump_obj__ (Typ_VC, &vc1, "  vx:");

    // plane from Origin, X-vec, Z-Vec
    UT3D_pl_pto_vcx_vcz (pl1, &pt1, &vc1, &vc2);
  }
    printd("  vcX=%lf %lf %lf\n",vc1.dx,vc1.dy,vc1.dz);


    // DEB_dump_obj__ (Typ_PLN, pl1, "STP_r_PLN_AXIS2:");

  return 0;

}


//================================================================
  int STP_r_PT_sInd (Point *pt1, int sInd) {
//================================================================
// Point from s_tab-index of VERTEX_POINT or CARTESIAN_POINT

  // printf("STP_r_PT_sInd %d #%d typ=%d\n",sInd,
          // s_tab[sInd].sInd,s_tab[sInd].sTyp);

  if(s_tab[sInd].sTyp == SC_VERTEX_POINT) {
    STP_r_PT_VERT (pt1, sInd);

  } else if(s_tab[sInd].sTyp == SC_CARTESIAN_POINT) {
    STP_r_PT_CARTPT (pt1, sInd);

  } else {
    TX_Print("STP_r_PT_sInd I001 %d",s_tab[sInd].sTyp);
    return -1;
  }

  // DEB_dump_obj__ (Typ_PT, pt1, "r_PT:");

  return 0;

}


//================================================================
  int STP_r_PT_VERT (Point *pt1, int is) {
//================================================================
// give Point from s_tab-index of VERTEX_POINT

  int   i1, *ia;
  
  // printf("STP_r_PT_VERT %d gTyp=%d\n",is,s_tab[is].gTyp);

  // if(s_tab[is].sTyp != SC_VERTEX_POINT) {
    // TX_Print("***** STP_r_PT_VERT E001 %d #%d",i1,s_tab[i1].sInd);
  // }

  ia = s_tab[is].sDat;            // 1.Link
    // printf(" ip[0]=%d %p\n",ip[0],&ip[0]);
  i1 = STP_r_findInd (ia[0], 0);

  if(s_tab[i1].sTyp != SC_CARTESIAN_POINT) {
    TX_Print("***** STP_r_PT_VERT E001 %d #%d",i1,s_tab[i1].sInd);
  }

  memcpy (pt1, s_tab[i1].sDat, sizeof(Point));

  // DEB_dump_obj__ (Typ_PT, pt1, " PT_VERT %d #%d:",sInd,s_tab[sInd].sInd);

  return 0;

}


//================================================================
  int STP_r_PT_CARTPT (Point *pt1, int is) {
//================================================================
// give Point from s_tab-index of CARTESIAN_POINT

  // int       i1;

  // printf("STP_r_PT_CARTPT %d #%d\n",is,s_tab[is].sInd);

  if(s_tab[is].sTyp != SC_CARTESIAN_POINT)
    TX_Print("***** STP_r_PT_CARTPT E001 %d #%d",is,s_tab[is].sInd);

  memcpy (pt1, s_tab[is].sDat, sizeof(Point));

  // DEB_dump_obj__ (Typ_PT, pt1, " PT_CART %d:",sInd);

  return 0;

}


//================================================================
  int STP_r_VC_DIR (Vector *vc1, int is) {
//================================================================
// give Vector from s_tab-index of DIRECTION
  
  if(s_tab[is].sTyp != SC_DIRECTION)
    TX_Print("***** STP_r_VC_DIR E001 %d #%d",is,s_tab[is].sInd);
  
  memcpy (vc1, s_tab[is].sDat, sizeof(Vector));
    
  // DEB_dump_obj__ (Typ_VC, vc1, " VC_DIR:");
    
  return 0;
    
}


//================================================================
  int STP_r_VC_VEC_t (Vector *vc1, int sInd) {
//================================================================
// STP_r_VC_VEC_t         Vector from s_tab-index of VECTOR with length
// give Vector from s_tab-index of VECTOR
// Output:

  int     i1, l1, *iap;
  double  d1;


  printd("STP_r_VC_VEC_t %d #%d\n",sInd,s_tab[sInd].sInd);


  if(s_tab[sInd].sTyp != SC_VECTOR) {
    TX_Error("STP_r_VC_VEC_t E1 #%d",s_tab[sInd].sInd);
    return -1;
  }


  // find index of DIRECTION
  l1 = STP_r_get_L1 (sInd);
  i1 = i_tab[l1];
    printd("  _VC_VEC %d #%d\n",i1,s_tab[i1].sInd);


  if(s_tab[i1].sTyp != SC_DIRECTION) {
    TX_Error("STP_r_VC_VEC_t E2 #%d #%d",s_tab[sInd].sInd,s_tab[i1].sInd);
    return -1;
  }


  // get vector
  memcpy (vc1, s_tab[i1].sDat, sizeof(Vector));


  // get length
  iap = s_tab[sInd].sDat;
  ++iap;   // skip link -> DIR
  iap = STP_r_getDb (&d1, iap);
    printd(" VEC-len %lf\n",d1);


  // multiply vector
  UT3D_vc_multvc (vc1, vc1, d1);

  printd("ex VC_VEC_t %lf %lf %lf\n",vc1->dx,vc1->dy,vc1->dz);


  return 0;

}


//================================================================
  int STP_r_VC_VEC_1 (Vector *vc1, double *d1, int sInd) {
//================================================================
// give Vector from s_tab-index of VECTOR
// Output:
//   d1      length

  int  i1, l1, *iap;


  printd("STP_r_VC_VEC_1 %d #%d\n",sInd,s_tab[sInd].sInd);


  if(s_tab[sInd].sTyp != SC_VECTOR) {
    TX_Error("STP_r_VC_VEC_1 E1 #%d",s_tab[sInd].sInd);
    return -1;
  }


  // find index of DIRECTION
  // i1 = STP_r_findInd (*((int*)s_tab[sInd].sDat), sInd);
  l1 = STP_r_get_L1 (sInd);
  // i1 = STP_r_findInd (sInd, 0);
  i1 = i_tab[l1];
    printd("  _VC_VEC %d #%d\n",i1,s_tab[i1].sInd);
  

  if(s_tab[i1].sTyp != SC_DIRECTION) {
    TX_Error("STP_r_VC_VEC_1 E2 #%d #%d",s_tab[sInd].sInd,s_tab[i1].sInd);
    return -1;
  }


  // copy Vector
  memcpy (vc1, s_tab[i1].sDat, sizeof(Vector));


  // copy length
  iap = s_tab[sInd].sDat;
  ++iap;   // skip link -> DIR
  iap = STP_r_getDb (d1, iap);
  // memcpy (d1, s_tab[i1].sDat, sizeof(double));


  // DEB_dump_obj__ (Typ_VC, vc1, " VC_VEC:");

  return 0;

}


//================================================================
  void* STP_r_getInt (int *i1, void *ia) {
//================================================================
// eine int aus dem mem lesen, pointer dann weitersetzen.
// return-adress can be used for textpointer (see STP_r_decTx1)

  int *iTab;

  memcpy (i1, ia, sizeof(int));

  // printf("ex STP_r_getInt %d\n",*i1);

  // return (void*)(++(int*)ia);

  iTab = ia;
  ++iTab;
  return (void*)iTab;

  // // ia = (void*)((int*)ia + 1);
  // return (void*)((int*)ia + 1);

}


//================================================================
  void* STP_r_getDb (double *d1, void *ia) {
//================================================================
// eine db aus dem mem lesen, pointer dann weitersetzen.
 
  double *da;

  da = ia;

  memcpy (d1, da, sizeof(double));

  ++da;

  // printf("ex STP_r_getDb %f\n",*d1);

  return (void*)da;

}


//================================================================
  void* STP_r_getSkip (void *pos, long csiz) {
//================================================================
 
  char *p1;

  p1 = (char*)pos + csiz;

  return (void*)p1;

}


//================================================================
  int STP_r_sav3DB (int sTyp, char *cbuf) {
//================================================================
// DIRECTION ('',(1.,0.,0.));

// es gehen die 3 doubles direkt --> s_dat.

  int    irc, iNr;
  double d1;

  irc = STP_r_skipObjNam (&cbuf);        // skip ObjNam
  if(irc < 0) return irc;


  // printf("STP_r_sav3DB #%d |%s|\n",s_Ind,cbuf);


  // next s-obj-Record
  irc = STP_r_nxtSrec ();
  if(irc < 0) return irc;


  s_tab[s_Nr].sTyp = sTyp;


  // if(UME_ck_free(&s_dat) < (sizeof(double)*3)) ..

  // doubles sind in Klammern eingebettet
  L_start:
  if(*cbuf == '(') {
    ++cbuf;
  } else if(*cbuf == ' ') {
    ++cbuf;
    goto L_start;
  } else {
    TX_Error("STP_r_sav3DB E001 |%s|",cbuf);
    return -2;
  }


  iNr = 0;
  irc = STP_r_decDbs (&iNr, &cbuf);  // decode & save 3 doubles
  if(irc < -1) return irc;

  // if(iNr != 3) {
  L_fill_zeros:
  if(iNr < 3) {
    // set missing values to 0
    d1 = 0.;
    UME_save (&s_dat, &d1, sizeof(double));
    ++iNr;
    goto L_fill_zeros;
  }
    
    // TX_Error("STP_r_sav3DB E002 |%s|",cbuf);
    // return -2;
  // }

  // DEB_dump_obj__ (Typ_PT, s_tab[s_Nr].sDat, "ex STP_r_sav3DB");
  return 0;

}


//================================================================
  int STP_r_decCurv1 (char *cbuf) {
//================================================================
// TRIMMED_CURVE('',#54,(#55),(#56),.T.,.CARTESIAN.);
// TRIMMED_CURVE('',#76,(16.855),(31.999),.T.,.UNSPECIFIED.);
// TRIMMED_CURVE('',#3367,(CARTESIAN_POINT(#3368)),(CARTES..
// TRIMMED_CURVE('',#17,(#21,PARAMETER_VALUE(0.)),(#22,PARAMETER_VALUE(33.)),.T.,.PARAMETER.);
// base_curve
// trim_1:  (#55)|CARTESIAN_POINT(#3368)|#21,PARAMETER_VALUE(0.)
// trim_2
// direction(fwd-rev): .T.|.F.
// master_representation trimming_preference:
//    .CARTESIAN.|.UNSPECIFIED.|.PARAMETER.

// 1 Link     basis_curve
// 1 int      typ of trim_1,trim_2; 0=Link, 1=Db
// 1 Link/Db  trim_1
// 1 Link/Db  trim_2
// logical 

  int   irc, i1, iNr, iTyp, iForm;
  // char  c1;


  irc = STP_r_skipObjNam (&cbuf);        // skip ObjNam
  if(irc < 0) return irc;


  printd("============================ \n");
  printd("STP_r_decCurv1 |%s|\n",cbuf);


  // next s-obj-Record
  irc = STP_r_nxtSrec ();
  if(irc < 0) return irc;


  s_tab[s_Nr].sTyp = SC_TRIMMED_CURVE;


  // decode & save 1 Link (baseCurve)
  iNr = 1;
  irc = STP_r_decLinks (&iNr, &cbuf);
  if(irc < 0) return irc;


  // check typ of trimObj's
  irc = STP_r_ckTypB (&iTyp, &iForm, cbuf);
    printd(" n-_ckTypB-irc=%d iTyp=%d iForm=%d\n",irc,iTyp,iForm);
  if(irc < 0) return irc;


  // save typ of trimObjs
  UME_save (&s_dat, &iTyp, sizeof(int));


  if(iForm == 0) {   // LINK
    irc = STP_r_decLinkB (&iNr, &cbuf);  // decode & save Link trim1
    if(irc < -1) return irc;

    irc = STP_r_decLinkB (&iNr, &cbuf);  // decode & save Link trim2
    if(irc < -1) return irc;



  } else if(iForm == 2) {  // indirect LINK
    irc = STP_r_decLinkxB (&iNr, &cbuf);  // decode & save Link trim1
    if(irc < -1) return irc;

    irc = STP_r_decLinkxB (&iNr, &cbuf);  // decode & save Link trim2
    if(irc < -1) return irc;



  } else if(iForm == 1) {  // DOUBLE
    irc = STP_r_decDbB (&iNr, &cbuf);  // decode & save db trim1
    if(irc < -1) return irc;

    irc = STP_r_decDbB (&iNr, &cbuf);  // decode & save db trim2
    if(irc < -1) return irc;



  } else if(iForm == 3) {  // indirect DOUBLE (PARAMETER_VALUE)
    irc = STP_r_decDbiB (&iNr, &cbuf);  // decode & save db trim1
    if(irc < -1) return irc;

    irc = STP_r_decDbiB (&iNr, &cbuf);  // decode & save db trim2
    if(irc < -1) return irc;
  }


    // printf(" cbuf now |%s|\n",cbuf);


  // decode Logical (.T./.F.)
  irc = STP_r_decLog1 (&i1, &cbuf);
  UME_save (&s_dat, &i1, sizeof(int)); // save log.


  return 0;

}

/*
//================================================================
  int STP_r_decSubCmd (char *cbuf, int typ) {
//================================================================
// resolv subCommands:
// BOUNDED_CURVE  BOUNDED_SURFACE  GEOMETRIC_REPRESENTATION_CONTEXT
// und call STP_r_dec1.
// Der Record BOUNDED_x wird ohne Daten angelegt; alle ZusatzRecords
// (unmittelbar danach) bekommen sInd = 0 !

//  BOUNDED_CURVE()
//    B_SPLINE_CURVE(..)
//    B_SPLINE_CURVE_WITH_KNOTS(.. .CIRCULAR_ARC. ..)
//    RATIONAL_B_SPLINE_CURVE ( .. weights_data ..)

//  BOUNDED_SURFACE()
//    B_SPLINE_SURFACE(..)
//    B_SPLINE_SURFACE_WITH_KNOTS(..)
//    RATIONAL_B_SPLINE_SURFACE(..)

  int   irc;
  char  *pNxt, nxtCmd[2];




  // irc = STP_r_skipObjNam (&cbuf);        // skip ObjNam
  // if(irc < 0) return irc;

  // printf("============================ \n");
  // printf("STP_r_decSubCmd %d |%s|\n",typ,cbuf);



  // next s-obj-Record
  irc = STP_r_nxtSrec ();
  if(irc < 0) return irc;

  s_tab[s_Nr].sTyp = typ;

  // keine eigenen daten; alle nachfolgenden Records mit (sInd == 0) 
  // sind zugehoerige Subrecords.


  //----------------------------------------------------------------
  // cbuf sollte mit ')' beginnen (von "BOUNDED_CURVE()")
  pNxt = cbuf;
  if(*pNxt == ')') {
    ++pNxt;
  } else {
    pNxt = strchr(cbuf, ')');
    if(pNxt == NULL) {TX_Error("STP_r_decSubCmd E001"); return -1;}
    ++pNxt;
  }


  // printf("STP_r_decSubCmd |%s|\n",pNxt);
  // printf("     mem_cbuf1 |%s|\n",mem_cbuf1);

  // in mem_cbuf1 steht ca  "#110\0(BOUNDED_CURVE()...."
  strncpy(nxtCmd, pNxt, 2);  // die chars NACH "BOUNDED_CURVE()"

  // remove Subrecord in mem_cbuf1;
  // get next Subrecords; zB B_SPLINE_CURVE(2,(#99,#..),...)
  // terminate Subrecord mit ";\0"
  L_nxt_SubCmd:
  irc = STP_r_readSubCmd (&pNxt, nxtCmd);
  if(irc < 0) goto L_done;

  // // in einen Subcommand sind die folgenden Records oft ohne Aussage (Inhalt)
  // if(STP_r_skipSubCmd() < 0) goto L_nxt_SubCmd;

  // subCommand decodieren und normal einspeichern.
  irc = STP_r_dec1 (&pNxt);
  if(irc < 0) return irc;


  goto L_nxt_SubCmd;


  //----------------------------------------------------------------
  L_done:

  // if(typ == SC_BOUNDED_SURFACE) return -4;      // TEST
  return 0;

}
*/

//================================================================
  int STP_r_decDit1 (char *cbuf, int mode) {
//================================================================
// mode=0: REPRESENTATION_RELATIONSHIP('','',#52,#156)
//     1. Link zu SHAPE_REPRESENTATION of the Ditto (zB Master)
//     2. Link zu SHAPE_REPRESENTATION of the Detail
// mode=1: REPRESENTATION_RELATIONSHIP_WITH_TRANSFORMATION(#175)
//     Link zu ITEM_DEFINED_TRANSFORMATION

// wird gespeichert als SC_REPRESENTATION_RELATIONSHIP mit 3 Links.

 
  int    irc, i1;
  char   *p1;


  // printf("===========================================\n");
  // printf("STP_r_decDit1 %d |%s|\n",mode,cbuf);


  // den primaryRecord speichern.
  if(MainStat < 0) {
    irc = STP_r_decSubHdr (SC_REPRESENTATION_RELATIONSHIP, 0);
    if(irc < 0) return irc;

    // Reihenfolge ist hier fix; zuerst muss mode=0 kommen;
    if(mode != 0) {TX_Error("STP_r_decDit1 E001"); return -1;}
  }

  // es gibt nur einen primaryRecord !


  //================================================================
  if(mode != 0) goto L_tra__;   // mode=0:  REPRESENTATION_RELATIONSHIP
  // skip 2 texte
  STP_r_skipObjNam (&cbuf);
  STP_r_skipObjNam (&cbuf);

  // save 2 Links
  i1 = 2;
  irc = STP_r_decLinks (&i1, &cbuf);  // decode & save Links
  return irc;



  //================================================================
  L_tra__:   // mode=1: REPRESENTATION_RELATIONSHIP_WITH_TRANSFORMATION

  // save 1 Link
  i1 = 1;
  irc = STP_r_decLinks (&i1, &cbuf);  // decode & save Links

  return irc;

}


//================================================================
  int STP_r_decCvUni (char *cbuf) {
//================================================================
// QUASI_UNIFORM_CURVE('E1',1,(#132,#133),.POLYLINE_FORM.,.F.,.U.);
// degree, controlpointlist, b_spline_curve_form, closed_curve, self_intersect
// b_spline_curve_form: elliptic_arc, polyline_form, parabolic_arc,
//                      circular_arc, unspecified, hyperbolic_arc;


  int    irc, i1;


  // printf("============================ \n");
  printd("STP_r_decCvUni |%s|\n",cbuf);


  irc = STP_r_savInit (SC_QUASI_UNIFORM_CURVE, &cbuf);
  if(irc < 0) return irc;

  // degree lesen aber nicht speichern. Muss 1 sein.
  STP_r_decInt1 (&i1, &cbuf);
  // printf(" deg=%d\n",i1);

  if(i1 != 1) TX_Print("STP_r_decCvUni E001 %d",i1);

  // decode & save Links in Klammern
  i1 = 0;  // all links
  irc = STP_r_decLinkB (&i1, &cbuf);
  if(irc < -1) return irc;

  return 0;

}
 

//================================================================
  int STP_r_decSpl1 (char *cbuf) {
//================================================================
// decode & save B_SPLINE_CURVE_WITH_KNOTS

// B_SPLINE_CURVE_WITH_KNOTS('',5,(#78,#79,#..),.UNSPECIFIED.,.U.,.U.,
//                           (6,3,4,3,6),(0.0,1.0,...),.UNSPECIFIED.);
//   degree,(control_points_list), curve_form, closed_curve, self_intersect
//   (knot_multiplicities), (knots), knot_type

// curve_form:  unspecified, polyline_form, parabolic_arc, circular_arc,
//              hyperbolic_arc, elliptic_arc


// (int) Degree
// (int) Nr of Controlpoints
// Links to Controlpoints
// (int) Nr of ints
// table of ints
// (int) Nr of doubles
// table of doubles


  int   irc, i1, iNr, ii;
  int   *nPos;


  irc = STP_r_skipObjNam (&cbuf);        // skip ObjNam
  if(irc < 0) return irc;

  // printf("============================ \n");
  // printf("STP_r_decSpl1 |%s|\n",cbuf);

  // next s-obj-Record
  irc = STP_r_nxtSrec ();
  if(irc < 0) return irc;

  s_tab[s_Nr].sTyp = SC_B_SPLINE_CURVE_WITH_KNOTS;

  //----------------------------------------------------------------
  // decode and save degree
  iNr = 1;
  irc = STP_r_decInts (&iNr, &cbuf);  // decode & save 1 ints
  if(irc < 0) return irc;
  if(iNr != 1) {
    TX_Error("STP_r_decSpl1 E001 |%s|",cbuf);
    return -2;
  }


  // reserve Space for the nr of Points
  nPos = s_dat.next;
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) {TX_Error("STP_r_decSpl1 EOM1"); return -4;}

  // get & save Controlpoints
  iNr = 0;
  irc = STP_r_decLinkB (&iNr, &cbuf);
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *nPos = iNr;
    // printf(" nr_of_CPts=%d\n",iNr);


  irc = STP_r_skipLog1 (&cbuf);
  irc = STP_r_skipLog1 (&cbuf);
  irc = STP_r_skipLog1 (&cbuf);


  // reserve Space for the nr of iTab
  nPos = s_dat.next;
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) {TX_Error("STP_r_decSpl1 EOM2"); return -4;}

  // get & save iTab
  irc = STP_r_decIntB (&iNr, &cbuf);
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *nPos = iNr;
    // printf(" nr_of_ints=%d\n",iNr);



  // reserve Space for the nr of Knots
  nPos = s_dat.next;
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) {TX_Error("STP_r_decSpl1 EOM3"); return -4;}

  // get & save Knots
  irc = STP_r_decDbB (&iNr, &cbuf);
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *nPos = iNr;
    // printf(" nr_of_doubles=%d\n",iNr);

  return 0;

}

//================================================================
  int STP_r_decSubHdr (int typ, int iNr) {
//================================================================
// den Header fuer einen Record aus subCommands anlegen.
// soll iNr ints bekommen (reserve space in s_dat)


  int    irc, i1, *ia;

  irc = STP_r_nxtSrec ();  // next s-obj-Record
  if(irc < 0) return irc;

  s_tab[s_Nr].sTyp = typ;
  s_tab[s_Nr].sInd = s_MainInd;  // override 0 !!!

  MainStat = s_Nr;

  // 3 ints reservieren
  if(iNr > 0) {
    UME_add (&s_dat, sizeof(int)*iNr);
    ia = s_tab[s_Nr].sDat;
    for(i1=0; i1<iNr; ++i1) ia[i1] = 0;
  }

  // printf("ex STP_r_decSubHdr %d #%d\n",MainStat,s_tab[MainStat].sInd);

  return irc;

}
 

//================================================================
  int STP_r_decSpl2 (char *cbuf) {
//================================================================
// decode & save B_SPLINE_CURVE  fuer subCommand:
//   subCommand hat meist:
//   B_SPLINE_CURVE + B_SPLINE_CURVE_WITH_KNOTS + RATIONAL_B_SPLINE_CURVE

// B_SPLINE_CURVE(2,(#139,#140,#141),.CIRCULAR_ARC.,.F.,.U.)
//   degree,(control_points_list), curve_form, closed_curve, self_intersect)

// curve_form:  unspecified, polyline_form, parabolic_arc, circular_arc,
//              hyperbolic_arc, elliptic_arc

// (int) Degree
// (int) Nr of Controlpoints
// Links to Controlpoints

  int   irc, iNr;
  int   *nPos, *ia;


  // printf("============================ \n");
  // printf("STP_r_decSpl2 |%s|\n",cbuf);

  if(MainStat < 0) {
    // soll 3 int bekommen; Index auf B_SPLINE_CURVE, B_SPLINE_CURVE_WITH_KNOTS
    // und auf RATIONAL_B_SPLINE_CURVE.
    irc = STP_r_decSubHdr (SC_BOUNDED_CURVE, 3); // den MainRecord anlegen
    if(irc < 0) return irc;
  }

  // next s-obj-Record
  irc = STP_r_nxtSrec ();
  if(irc < 0) return irc;

  s_tab[s_Nr].sTyp = SC_B_SPLINE_CURVE;


  // das erste int-Word des s_MainInd-Record auf s_Nr setzen.
  ia = s_tab[MainStat].sDat;
  ia[0] = s_Nr;



  //----------------------------------------------------------------
  // decode and save degree
  iNr = 1;
  irc = STP_r_decInts (&iNr, &cbuf);  // decode & save 1 ints
  if(irc < 0) return irc;
  if(iNr != 1) {
    TX_Error("STP_r_decSpl2 E001 |%s|",cbuf);
    return -2;
  }


  // reserve Space for the nr of Points
  nPos = s_dat.next;
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) {TX_Error("STP_r_decSpl2 EOM2"); return -4;}

  // get & save Controlpoints
  iNr = 0;
  irc = STP_r_decLinkB (&iNr, &cbuf);
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *nPos = iNr;
    // printf(" nr_of_CPts=%d\n",iNr);

  return 0;

}


//================================================================
  int STP_r_decSpl3 (char *cbuf) {
//================================================================
// decode & save B_SPLINE_CURVE_WITH_KNOTS  fuer subCommand:
//   subCommand hat meist:
//   B_SPLINE_CURVE + B_SPLINE_CURVE_WITH_KNOTS + RATIONAL_B_SPLINE_CURVE

// B_SPLINE_CURVE_WITH_KNOTS((3,2,2,3),(0.0,0.33,0.66,1.0),.UNSPECIFIED.)
//   (knot_multiplicities), (knots), knot_type


// (int) Nr of ints
// table of ints                 (knot-multiplicities)
// (int) Nr of doubles
// table of doubles              (knot-data)


  int   irc, i1, iNr, ii;
  int   *nPos, *ia;


  // printf("============================ \n");
  // printf("STP_r_decSpl3 |%s|\n",cbuf);

  // next s-obj-Record
  irc = STP_r_nxtSrec ();
  if(irc < 0) return irc;

  s_tab[s_Nr].sTyp = SC_B_SPLINE_CURVE_WITH_KNOTS;


  // das 2. int-Word des s_MainInd-Record auf s_Nr setzen.
  ia = s_tab[MainStat].sDat;
  ia[1] = s_Nr;


  //----------------------------------------------------------------

  // reserve Space for the nr of iTab
  nPos = s_dat.next;
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) {TX_Error("STP_r_decSpl3 EOM1"); return -4;}

  // get & save iTab
  irc = STP_r_decIntB (&iNr, &cbuf);
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *nPos = iNr;
    // printf(" nr_of_ints=%d\n",iNr);



  // reserve Space for the nr of Knots
  nPos = s_dat.next;
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) {TX_Error("STP_r_decSpl3 EOM2"); return -4;}

  // get & save Knots
  irc = STP_r_decDbB (&iNr, &cbuf);
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *nPos = iNr;
    // printf(" nr_of_doubles=%d\n",iNr);


  return 0;

}


//================================================================
  int STP_r_decSpl4 (char *cbuf) {
//================================================================
// decode & save RATIONAL_B_SPLINE_CURVE  fuer subCommand:
//   subCommand hat meist:
//   B_SPLINE_CURVE + B_SPLINE_CURVE_WITH_KNOTS + RATIONAL_B_SPLINE_CURVE

// RATIONAL_B_SPLINE_CURVE((1.0,0.33,0.33,1.0))
//   (weights-data)

// (int) Nr of doubles
// table of doubles                (weights-data)


  int   irc, iNr;
  int   *nPos, *ia;


  // printf("============================ \n");
  // printf("STP_r_decSpl4 |%s|\n",cbuf);

  // next s-obj-Record
  irc = STP_r_nxtSrec ();
  if(irc < 0) {TX_Error("STP_r_decSpl4 EOM1"); return -4;}

  s_tab[s_Nr].sTyp = SC_RATIONAL_B_SPLINE_CURVE;


  // das 3. int-Word des s_MainInd-Record auf s_Nr setzen.
  ia = s_tab[MainStat].sDat;
  ia[2] = s_Nr;



  //----------------------------------------------------------------
  // reserve Space for the nr of Knots
  nPos = s_dat.next;
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) {TX_Error("STP_r_decSpl4 EOM2"); return -4;}

  // get & save Knots
  irc = STP_r_decDbB (&iNr, &cbuf);
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *nPos = iNr;
    // printf(" nr_of_doubles=%d\n",iNr);

  return 0;

}


//================================================================
  int STP_r_decBspSu_ (char *cbuf, int sInd) {
//================================================================
// B_SPLINE_SURFACE_WITH_KNOTS nur als mainCommand

  int   irc;


  // printf("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS %d\n",s_Ind);
  // printf("STP_r_decBspSu_\n");


  irc = STP_r_skipObjNam (&cbuf);        // skip ObjNam
  if(irc < 0) return irc;

  // next s-obj-Record
  irc = STP_r_nxtSrec ();
  if(irc < 0) return irc;

  s_tab[s_Nr].sTyp = SC_B_SPLINE_SURFACE_WITH_KNOTS;


  // get ControlPoints
  STP_r_decBspSu1 (&cbuf, sInd);

  // get knots
  STP_r_decBspSu2 (&cbuf, sInd);

  return 0;

}


//================================================================
  int STP_r_decBspSu1 (char **cbuf) {
//================================================================
// B_SPLINE_SURFACE     normal & subCommand
//  B_SPLINE_SURFACE (int u_deg, int v_deg, ((control_points_list..),
//    surface_form,
//    log u_closed, log v_closed, log self_intersect)

// surface_form:
//   plane_surf, generalised_cone, toroidal_surf, conical_surf,
//   spherical_surf, unspecified, ruled_surf, surf_of_revolution,
//   cylindrical_surf, quadric_surf


  int   irc, iNr, *ia;


  // printf("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS %d\n",s_Ind);
  // printf("STP_r_decBspSu1 |");UTX_dump__(*cbuf,60);printf("|\n");


  // normal (von STP_r_decBspSu_ used)
  if(s_Ind > 0) goto L_dec;


  // subCommand:
  if(MainStat < 0) {
    // soll 3 int bekommen; Index auf B_SPLINE_CURVE, B_SPLINE_CURVE_WITH_KNOTS
    // und auf RATIONAL_B_SPLINE_CURVE.
    irc = STP_r_decSubHdr (SC_BOUNDED_SURFACE, 3); // MainRecord in s_tab anlegen
    if(irc < 0) return irc;
  }

  // next s-obj-Record
  irc = STP_r_nxtSrec ();  // init s_tab[s_Nr]
  if(irc < 0) return irc;

  s_tab[s_Nr].sTyp = SC_B_SPLINE_SURFACE;


  // das erste int-Word des s_MainInd-Record auf s_Nr setzen.
  ia = s_tab[MainStat].sDat;
  ia[0] = s_Nr;




  //----------------------------------------------------------------
  L_dec:
  // decode and save uDeg, vDeg.
  iNr = 2;
  irc = STP_r_decInts (&iNr, cbuf);  // decode & save ints
  if(irc < 0) return irc;
  if(iNr != 2) {
    TX_Error("STP_r_decBspSu1 E001 |%s|",cbuf);
    return -2;
  }



  // decode sqare-block of links
  irc = STP_r_decLink2B (cbuf);
  if(irc < 0) return irc;



  // 4 logicals; skippen.
  if(STP_r_skipLog1 (cbuf) < 0) return -1;  // surface_form
  if(STP_r_skipLog1 (cbuf) < 0) return -1;  // u_closed
  if(STP_r_skipLog1 (cbuf) < 0) return -1;  // v_closed
  if(STP_r_skipLog1 (cbuf) < 0) return -1;  // self_intersect

// if(MainStat == 3535) exit(0);
// exit(0);

  return 0;

}


//================================================================
  int STP_r_decBspSu2 (char **cbuf) {
//================================================================
// B_SPLINE_SURFACE_WITH_KNOTS   fuer normal & subCommand !

  int   irc, iNr, *ia;
  int   *nPos;


  // printf("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS %d\n",s_Ind);
  // printf("STP_r_decBspSu2 |");UTX_dump_s__ (*cbuf); printf("|\n");


  // normal (von STP_r_decBspSu_ used)
  if(s_Ind > 0) goto L_dec;


  // subCommand:
  if(MainStat < 0) {
    // soll 3 int bekommen; Index auf B_SPLINE_CURVE, B_SPLINE_CURVE_WITH_KNOTS
    // und auf RATIONAL_B_SPLINE_CURVE.
    irc = STP_r_decSubHdr (SC_BOUNDED_SURFACE, 3); // den MainRecord anlegen
    if(irc < 0) return irc;
  }

  // next s-obj-Record
  irc = STP_r_nxtSrec ();
  if(irc < 0) return irc;

  s_tab[s_Nr].sTyp = SC_B_SPLINE_SURFACE_WITH_KNOTS;


  // das 2. int-Word des s_MainInd-Record auf s_Nr setzen.
  ia = s_tab[MainStat].sDat;
  ia[1] = s_Nr;





  //----------------------------------------------------------------
  L_dec:

  // reserve Space for the nr of uTab
  nPos = s_dat.next;
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) {TX_Error("STP_r_decBspSu2 EOM1"); return -4;}

  // get & save iTab
  irc = STP_r_decIntB (&iNr, cbuf);
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *nPos = iNr;
    // printf(" nr_of_U-ints=%d\n",iNr);



  // reserve Space for the nr of vTab
  nPos = s_dat.next;
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) {TX_Error("STP_r_decBspSu2 EOM2"); return -4;}

  // get & save iTab
  irc = STP_r_decIntB (&iNr, cbuf);
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *nPos = iNr;
    // printf(" nr_of_V-ints=%d\n",iNr);



  // reserve Space for the nr of U-Knots
  nPos = s_dat.next;
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) {TX_Error("STP_r_decBspSu2 EOM3"); return -4;}

  // get & save Knots
  irc = STP_r_decDbB (&iNr, cbuf);
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *nPos = iNr;
    // printf(" nr_of_U-Knots=%d\n",iNr);


  // reserve Space for the nr of V-Knots
  nPos = s_dat.next;
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) {TX_Error("STP_r_decBspSu2 EOM4"); return -4;}

  // get & save Knots
  irc = STP_r_decDbB (&iNr, cbuf);
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *nPos = iNr;
    // printf(" nr_of_V-Knots=%d\n",iNr);


  return 0;

}


//================================================================
  int STP_r_decBspSu3 (char **cbuf) {
//================================================================
// RATIONAL_B_SPLINE_SURFACE             nur subCommand
// weights


  int  irc, *ia;


  // printf("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS\n");
  // printf("STP_r_decBspSu3 |");UTX_dump_s__ (*cbuf); printf("|\n");



  if(MainStat < 0) {
    // soll 3 int bekommen; Index auf B_SPLINE_CURVE, B_SPLINE_CURVE_WITH_KNOTS
    // und auf RATIONAL_B_SPLINE_CURVE.
    irc = STP_r_decSubHdr (SC_BOUNDED_SURFACE, 3); // den MainRecord anlegen
    if(irc < 0) return irc;
  }

  // next s-obj-Record
  irc = STP_r_nxtSrec ();
  if(irc < 0) return irc;

  s_tab[s_Nr].sTyp = SC_RATIONAL_B_SPLINE_SURFACE;


  // das 3. int-Word des s_MainInd-Record auf s_Nr setzen.
  ia = s_tab[MainStat].sDat;
  ia[2] = s_Nr;




  //----------------------------------------------------------------

  // decode sqare-block of doubles
  irc = STP_r_decDb2B (cbuf);
  if(irc < 0) return irc;

  return 0;

}

/*
//================================================================
  int STP_r_decBspSu1 (char *cbuf, int sInd) {
//================================================================
// B_SPLINE_SURFACE_WITH_KNOTS

// 2 ints (deg. u,v)
// 2 ints (u-Nr, v-Nr)
// [u-Nr * v-Nr] Links to points; 
// ..


  int   irc, i1, iNr, ii;
  int   *nPos;


  if(sInd >0) {                            // subCommand hat sInd = 0 !
    irc = STP_r_skipObjNam (&cbuf);        // skip ObjNam
    if(irc < 0) return irc;
  }

  printf("============================ \n");
  printf("STP_r_decBspSu1 %d |%s|\n",cbuf,sInd);


  // next s-obj-Record
  irc = STP_r_nxtSrec ();
  if(irc < 0) return irc;

  s_tab[s_Nr].sTyp = SC_B_SPLINE_SURFACE_WITH_KNOTS;


  //----------------------------------------------------------------
  // decode and save uDeg, vDeg.
  iNr = 2;
  irc = STP_r_decInts (&iNr, &cbuf);  // decode & save 1 ints
  if(irc < 0) return irc;
  if(iNr != 2) {
    TX_Error("STP_r_decBspSu1 E001 |%s|",cbuf);
    return -2;
  }



  // decode sqare-block of links
  irc = STP_r_decLink2B (&cbuf);
  if(irc < 0) return irc;



  // 4 logicals; skippen.
  if(STP_r_skipLog1 (&cbuf) < 0) return -4;
  if(STP_r_skipLog1 (&cbuf) < 0) return -4;
  if(STP_r_skipLog1 (&cbuf) < 0) return -4;
  if(STP_r_skipLog1 (&cbuf) < 0) return -4;


  // reserve Space for the nr of uTab
  nPos = s_dat.next;
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) return -4;

  // get & save iTab
  irc = STP_r_decIntB (&iNr, &cbuf);
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *nPos = iNr;
    // printf(" nr_of_U-ints=%d\n",iNr);



  // reserve Space for the nr of vTab
  nPos = s_dat.next;
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) return -4;

  // get & save iTab
  irc = STP_r_decIntB (&iNr, &cbuf);
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *nPos = iNr;
    // printf(" nr_of_V-ints=%d\n",iNr);



  // reserve Space for the nr of U-Knots
  nPos = s_dat.next;
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) return -4;

  // get & save Knots
  irc = STP_r_decDbB (&iNr, &cbuf);
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *nPos = iNr;
    // printf(" nr_of_U-Knots=%d\n",iNr);


  // reserve Space for the nr of V-Knots
  nPos = s_dat.next;
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) return -4;

  // get & save Knots
  irc = STP_r_decDbB (&iNr, &cbuf);
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *nPos = iNr;
    // printf(" nr_of_V-Knots=%d\n",iNr);


  return 0;

}
*/

//================================================================
  int STP_r_decCCV1 (char *cbuf) {
//================================================================
// COMPOSITE_CURVE('',(#64,#72,#80),.U.) ;
// segments,
// .U. self_intersect (oder closed_curve ?)


  int   irc, iNr;

  irc = STP_r_skipObjNam (&cbuf);        // skip ObjNam
  if(irc < 0) return irc;

  // printf("================================ \n");
  // printf("STP_r_decCCV1 |%s|\n",cbuf);

  // next s-obj-Record
  irc = STP_r_nxtSrec ();
  if(irc < 0) return irc;

  s_tab[s_Nr].sTyp = SC_COMPOSITE_CURVE;

  // reserve Space for the nr of Links
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) {TX_Error("STP_r_decCCV1 EOM1"); return -4;}

  iNr = 0;
  irc = STP_r_decLinkB (&iNr, &cbuf);  // decode & save Links in Klammern
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *((int*)(s_tab[s_Nr].sDat)) = iNr;
    // printf(" nr_of_links=%d\n",iNr);

  return 0;

}


//================================================================
  int STP_r_decCCV2 (char *cbuf) {
//================================================================
// COMPOSITE_CURVE_SEGMENT(.DISCONTINUOUS.,.T.,#62);
// .CONTINUOUS./.DISCONTINUOUS.     transition
// .T./.F.                          same_sense
// Link --> parent_curve

// 1 Link --> parent_curve, sonst nix

  int   irc, iNr;

  // printf("================================ \n");
  // printf("STP_r_decCCV2 |%s|\n",cbuf);

  // next s-obj-Record
  irc = STP_r_nxtSrec ();
  if(irc < 0) return irc;


  s_tab[s_Nr].sTyp = SC_COMPOSITE_CURVE_SEGMENT;


  // if(UME_ck_free(&s_dat) < (sizeof(int)*3)) ..

  // skip a logical
  STP_r_skipLog1 (&cbuf);

  // skip a logical
  STP_r_skipLog1 (&cbuf);

  // save 1 Link
  iNr = 1;
  irc = STP_r_decLinks (&iNr, &cbuf);  // decode & save 2 Links
  if(irc < 0) return irc;



  return 0;

}


//================================================================
  int STP_r_decFace1 (char *cbuf) {
//================================================================
// ADVANCED_FACE('',(#54,#60),#56,.T.)
// ((FACE_BOUND, FACE_OUTER_BOUND) Stuetzflaeche, T/F)

// Nr of Links, Links
// Link --> Stuetzflaeche

  int   irc, iNr;

  irc = STP_r_skipObjNam (&cbuf);        // skip ObjNam
  if(irc < 0) return irc;

  // printf("============================ \n");
  // printf("STP_r_decFace1 |%s|\n",cbuf);

  // next s-obj-Record
  irc = STP_r_nxtSrec ();
  if(irc < 0) return irc;

  s_tab[s_Nr].sTyp = SC_ADVANCED_FACE;

  // reserve Space for the nr of Links
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) {TX_Error("STP_r_decFace1 EOM1"); return -4;}

  iNr = 0;
  irc = STP_r_decLinkB (&iNr, &cbuf);  // decode & save Links in Klammern
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *((int*)(s_tab[s_Nr].sDat)) = iNr;
    // printf(" nr_of_links=%d\n",iNr);

  // save Stuetzflaeche
  iNr = 1;
  irc = STP_r_decLinks (&iNr, &cbuf);  // decode & save 1 Link
  if(irc < 0) return irc;
  if(iNr != 1) {
    TX_Error("STP_r_decFace1 E001 |%s|",cbuf);
    return -2;
  }

  return 0;

}



//================================================================
  int STP_r_savInit (int sTyp, char **cbuf) {
//================================================================
// create new record, set sTyp.


  int    irc, i1;

  irc = STP_r_skipObjNam (cbuf);        // skip ObjNam
  if(irc < 0) return irc;


  // printf("STP_r_savInit %d |%s|\n",sTyp,*cbuf);


  // init next s-obj-Record; its new index will be s_Nr
  irc = STP_r_nxtSrec ();
  if(irc < 0) return irc;


  s_tab[s_Nr].sTyp = sTyp;

  return 0;

}


//================================================================
  int STP_r_savInt (int ii) {
//================================================================
// save int ii

  // printf("STP_r_savInt %d %d\n",ii,s_Nr);

  if(UME_save (&s_dat, &ii, sizeof(int)) == NULL) return -1;

  return 0;

}


//================================================================
  int STP_r_savLDL (int sTyp, int iNr, int dNr, int lNr, char *cbuf) {
//================================================================
// fixe Anzahl Links/doubles/Logicals decodieren und speichern
// iNr nr Links.
// dNr nr doubles.
// lNr nr logicals (T/Fi).


  int    irc, i1;

  // printf("STP_r_savLDL %d %d %d |%s|\n",iNr, dNr,lNr,cbuf);


  // init next s-obj-Record  & skip objName
  irc = STP_r_savInit (sTyp, &cbuf);
  if(irc < 0) return irc;
    // printf("  s_tab[%d].sTyp = %d\n",s_Nr,s_tab[s_Nr].sTyp);


  // if(UME_ck_free(&s_dat) < (sizeof(int)*3)) ..


  if(iNr > 0) {
    i1 = iNr;
    irc = STP_r_decLinks (&i1, &cbuf);  // decode & save Links
    if(irc < 0) return irc;
    if(i1 != iNr) {
      TX_Error("STP_r_savLDL E001 |%s|",cbuf);
      return -2;
    }
  }


  if(dNr > 0) {
    i1 = dNr;
    irc = STP_r_decDbs (&i1, &cbuf);  // decode & save doubles
    if(irc < -1) return irc;
    if(i1 != dNr) {
      TX_Error("STP_r_savLDL E002 |%s|",cbuf);
      return -2;
    }
      // TEST BEGIN
      // { double *dp1; dp1 = s_tab[s_Nr].sDat; printf(" sdat = %p\n",dp1);
        // for(i1=0;i1<dNr; ++i1) { printf(" d[%d]=%f\n",i1,*dp1); ++dp1; } }
      // TEST END
  }


  if(lNr > 0) {
    irc = STP_r_decLog1 (&i1, &cbuf);  // decode Logical
    // if(irc < 0) return irc;

    // save 1 int --> s_dat
    UME_save (&s_dat, &i1, sizeof(int));
  }




  return 0;

}


//================================================================
  int STP_r_savLinkB (int settyp, char **cbuf) {
//================================================================
// 1-n bracketed Links decodieren und speichern
// - must starts with char '('
// GEOMETRIC_SET('',(#24,#25,#26)) ;
// GEOMETRIC_CURVE_SET('',(#24,#25,#26)) ;
// OPEN_SHELL('',(#190,#213,#229,#240)) ;

// Anzahl Links, Links.

  int   irc, iNr;

  irc = STP_r_skipObjNam (cbuf);        // skip ObjNam
  if(irc < 0) return irc;


  // printd("================================ \n");
  // printd("STP_r_savLinkB |%s|\n",*cbuf);


  // next s-obj-Record
  irc = STP_r_nxtSrec ();
  if(irc < 0) return irc;


  s_tab[s_Nr].sTyp = settyp;


  // if(UME_ck_free(&s_dat) < (sizeof(int)*3)) ..

  // reserve Space for the nr of Links
  irc = UME_add (&s_dat, sizeof(int));
  if(irc < 0) {TX_Error("STP_r_savLinkB EOM1"); return -4;}


  iNr = 0;
  irc = STP_r_decLinkB (&iNr, cbuf);  // decode & save Links in Klammern
  if(irc < -1) return irc;

  // save nr of Links (first word !)
  *((int*)(s_tab[s_Nr].sDat)) = iNr;
    // printf(" nr_of_links=%d\n",iNr);

  // printd("ex STP_r_savLinkB |%s|\n",*cbuf);
  // UTX_dump_s__ (*cbuf); printf("|\n");


  return 0;

}


//================================================================
  int STP_r_savEdge1 (char *cbuf) {
//================================================================
// ORIENTED_EDGE('',*,*,#266,.F.);
// 1 EDGE_CURVE
// 1 Link ...


  int    irc, i1;

  irc = STP_r_skipObjNam (&cbuf);        // skip ObjNam
  if(irc < 0) return irc;


  // printf("=====================\n");
  // printf("STP_r_savEdge1 |%s|\n",cbuf);


  // next s-obj-Record
  irc = STP_r_nxtSrec ();
  if(irc < 0) return irc;

  s_tab[s_Nr].sTyp = SC_ORIENTED_EDGE;


  // skip 2 words ("*,*,"   oder " *, *,")
  irc = STP_r_skipWords (&cbuf, 2);
  if(irc < 0) return irc;



/*
  // skip "*,*,"
  if(strncmp(cbuf, "*,*,", 4)) {
    TX_Error("STP_r_savEdge1 E001 |%s|",cbuf);
    return -2;
  }
  cbuf += 4;
*/


  i1 = 1;
  irc = STP_r_decLinks (&i1, &cbuf);  // decode & save 1 Link
  if(irc < 0) return irc;
  if(i1 != 1) {
    TX_Error("STP_r_savEdge1 E002 |%s|",cbuf);
    return -2;
  }


  return 0;

}

//================================================================
  int STP_r_skipWords (char **cbuf, int wNr) {
//================================================================
// STP_r_skipWords        skip words (find next ',')

  int     i1;
  char    *p1;

  p1 = *cbuf;

  for(i1=0; i1<wNr; ++i1) {

    p1 = strchr(p1, ',');
    if(p1 == NULL) {
      TX_Error("STP_r_skipWords E001 |%s|",*cbuf);
      return -1;
    }
    ++p1;  // skip the ','
  }

  *cbuf = p1;

  return 0;

}


//================================================================
  int STP_r_skipTer1 (char **cBuf) {
//================================================================
// skip terminating characters  "," or ")" or "),"
// RetCode:  0  wenn ","
// RetCode: -1  wenn ")"
// es werden "," oder ")" oder ")," geskippt !

  int    irc;
  char   *p1;

  p1 = *cBuf;

  L_bl1:
  if(*p1 == ',') {
    irc =  0;
    ++p1;

  } else if(*p1 == ' ') {
    ++p1;
    goto L_bl1;

  } else if(*p1 == ')') {
    irc = -1;            // ')' - End of Record
    ++p1;
    // skip noch nachfolgendes ','
    L_bl2:
    if(*p1 == ',') {
      ++p1;
    } else if(*p1 == ' ') {
      ++p1;
      goto L_bl2;
    }
  }

  *cBuf = p1;

  return irc;

}


//================================================================
  int STP_r_skipT (char **cbuf) {
//================================================================
// STP_r_skipT            skip Textfield 'xxx' or $ and delimiter

  char    *p1;


  p1 = *cbuf;

  // printd("STP_r_skipT |%s|\n",p1);


  // Textfield'$' 
  if(*p1 == '$') { ++p1; goto L_6; }


  // skip starting ' and blanks before
  L_start:
  if(*p1 == '\'') {
    ++p1;

  } else if(*p1 == ' ') {
    ++p1;
    goto L_start;

  } else {
    TX_Error("STP_r_skipT E001 |%s|",*cbuf);
    return -1;
  }


  // find closing '
  p1 = strchr(p1, '\'');
  if(p1 == NULL) {
    TX_Error("STP_r_skipT E002 |%s|",*cbuf);
    return -1;
  }
  ++p1;  // skip it


  // skip following blanks
  L_6:
  while(*p1 == ' ') ++p1;

  // skip the following ',' od ')'
  ++p1;

  *cbuf = p1;

    // printf("ex STP_r_skipT |%s|\n",p1);

  return 0;

}



//================================================================
  int STP_r_skipB (char **cbuf) {
//================================================================
// STP_r_skipB            skip Block (...)
// ACHTUNG: nicht fuer ((..),(..))

  char    *p1;

  p1 = *cbuf;

  // printf("STP_r_skipB |%s|\n",p1);

  if(*p1 == '(') {
    ++p1;
  } else {
    TX_Error("STP_r_skipB E001 |%s|",*cbuf);
    return -1;
  }


  p1 = strchr(p1, ')');
  if(p1 == NULL) {
    TX_Error("STP_r_skipB E002 |%s|",*cbuf);
    return -1;
  }
  ++p1;

  // skip the following ',' od ')'
  ++p1;

  *cbuf = p1;


  return 0;

}


//================================================================
  int STP_r_skipLog1 (char **cbuf) {
//================================================================
// skip a logical
// .T. .F. .DISCONTINUOUS.  usw

  char    *p1;

  p1 = *cbuf;

  // printf("STP_r_skipLog1 |%s|\n",p1);

  L_start:
  if(*p1 == '.') {
    ++p1;

  } else if(*p1 == ' ') {
    ++p1;
    goto L_start;

  } else {
    TX_Error("STP_r_skipLog1 E001 |%s|",*cbuf);
    return -1;
  }

  p1 = strchr(p1, '.');
  if(p1 == NULL) {
    TX_Error("STP_r_skipLog1 E002 |%s|",*cbuf);
    return -1;
  }
  ++p1;

  // skip the following ',' od ')'
  ++p1;

  *cbuf = p1;

  return 0;

}


//================================================================
  int STP_r_skipObjNam (char **cbuf) {
//================================================================
// skip ObjName; zB 'Line Origine' od '' od $    (ACHTUNG: nur ein $ - char !)

  char    *p1;

  p1 = *cbuf;


  // printf("STP_r_skipObjNam |%s|\n",p1);


  L_start:
  if(*p1 == '\'') {
    ++p1;
    p1 = strchr(p1, '\'');
    if(p1 == NULL) {
      TX_Error("STP_r_skipObjNam E001 |%s|",*cbuf);
      return -1;
    }
    ++p1;


  } else if(*p1 == ' ') {
    ++p1;
    goto L_start;


  } else if(*p1 == '$') {
    ++p1;

  } else {
      TX_Error("STP_r_skipObjNam E002 |%s|",*cbuf);
      return -1;
  }



  // skip the following ','
  while (*p1  == ' ')  { ++p1; }
  if(*p1 == ',') {
    ++p1;

  } else {
      TX_Error("STP_r_skipObjNam E003 |%s|",*cbuf);
      return -1;
  }


  *cbuf = p1;

  return 0;

}


//================================================================
  int STP_r_ckTypB (int *oTyp, int *oForm, char *p1) {
//================================================================
// check typ of Obj's in Klammer
// Input:
//   p1   eg "(#55)"
// Output:
//   oTyp=0 oForm=0: direct Link     "(#55)"
//   oTyp=1 oForm=1: direct double   "(276.3)"
//   oTyp=0 oForm=2: indirect Link   "(CARTESIAN_POINT(#3368))"
//   oTyp=1 oForm=3: indirect double "(PARAMETER_VALUE(358.240717106))"

// RetCod:  0  = OK;
//         -2  = Formatfehler.



  int irc;

  // printf("STP_r_ckTypB |%s|\n",p1);

  L_start1:
  if(*p1 == '(') {
    ++p1;

  } else if(*p1 == ' ') {
    ++p1;
    goto L_start1;

  } else {
    TX_Error("STP_r_ckTypB E001 |%s|",*p1);
    return -2;
  }



  L_start2:
  if(*p1 == '#') {
    *oTyp  = 0;
    *oForm = 0;

  } else if(*p1 == ' ') {
    ++p1;
    goto L_start2;

  } else if(!strncmp(p1,"CARTESIAN_POINT",15)) {
    *oTyp  = 0;
    *oForm = 2;

  } else if(!strncmp(p1,"PARAMETER_VALUE",15)) {
    *oTyp  = 1;
    *oForm = 3;

  } else if (isalpha(*p1)) {  // isalpha - Fehler !
    TX_Error("STP_r_ckTypB E002 |%s|",*p1);
    return -2;

  } else {
    *oTyp  = 1; // is numer - double.
    *oForm = 1;
  }

  // printf("ex STP_r_ckTypB typ=%d form=%d\n",*oTyp,*oForm);

  return 0;

}


//================================================================
  int STP_r_decIntB (int *iNr, char **cbuf) {
//================================================================
// decode Ints in Klammer eingebettet
// iNr out:  Anzahl decodierter Links.
// RetCod  0 - nach (..) war ein ',' (wird geskippt !)
// RetCod -1 - nach (..) war ein ')' (wird geskippt !)
// RetCod -2 - Fehler; stop.

  int    irc;
  char   *p1;

  p1 = *cbuf;

  L_start:
  if(*p1 == '(') {
    ++p1;

  } else if(*p1 == ' ') {
    ++p1;
    goto L_start;

  } else {
    TX_Error("STP_r_decIntB E001 |%s|",*cbuf);
    return -2;
  }

  *iNr = 0;
  irc = STP_r_decInts (iNr, &p1);  // decode & save all Ints
  if(irc < 0) return irc;


  // nach ')':
  if(*p1 == ',') irc =  0;
  else           irc = -1;            // ')' - End of Record

  ++p1;
  *cbuf = p1;

  return irc;

}


//================================================================
  int STP_r_decInts (int *iNr, char **cbuf) {
//================================================================
// decode iNr Ints; wenn (iNr==0) decode bis ')';
// iNr out:  Anzahl decodierter Ints.
// ein '$' wird als Int -1 gepeichert.
// RetCod  0 - OK
// RetCod -2 - Fehler; stop.

  int   irc, i1, iend;

  // printf("STP_r_decLinks %d |%s|\n",*iNr,*cbuf);

  iend = *iNr;
  if(iend < 1) iend = 99999;

  *iNr = 0;

  // get next Link
  L_nxt_l:
  irc = STP_r_decInt1 (&i1, cbuf);
  if(irc == -2) return irc;

  *iNr += 1;

  // save Link --> s_dat
    // printf("  %d save int %d\n",*iNr,i1);
  UME_save (&s_dat, &i1, sizeof(int));


  if(irc == 0) {
    if(*iNr < iend) goto L_nxt_l;
  }

  return 0;

}


//================================================================
  int STP_r_decInt1 (int *iVal, char **cbuf) {
//================================================================
// decode but do not save 1 Int.
// RetCod  0 - nach Db war ein ',' (wird geskippt !)
// RetCod -1 - nach Db war ein ')' (wird geskippt !)
// RetCod -2 - Fehler; stop.


  int    irc, i1;
  char   *p1, *p2;

  // printf("STP_r_decInt1 |%s|\n",*cbuf);

  // find next ',' od ')'
  p1 = *cbuf;
  for(i1=0; i1<24; ++i1) {
    ++p1;
    if(*p1 == ',') {*p1 = '\0'; irc =  0; goto L_dec;}
    if(*p1 == ')') {*p1 = '\0'; irc = -1; goto L_dec;}
  }
  TX_Error("STP_r_decInt1 E001 |%s|",p1);
  return -2;

  L_dec:
  *iVal = atoi(*cbuf);

  ++p1;
  *cbuf = p1;

  // printf("ex STP_r_decInt1 %d |%s|\n",*iVal,*cbuf);

  return irc;

}

//================================================================
  int STP_r_decDbiB (int *iNr, char **cbuf) { 
//================================================================
// decode & save Dbs in Klammer eingebettet (PARAMETER_VALUE(358.240717106))
// iNr out:  Anzahl decodierter Dbs.
// RetCod  0 - nach (..) war ein ',' (wird geskippt !)
// RetCod -1 - nach (..) war ein ')' (wird geskippt !)
// RetCod -2 - Fehler; stop. 

  int    irc;
  char   *p1; 
    
    
  // printf("STP_r_decDbiB |");UTX_dump_s__(*cbuf);printf("|\n");
  printd("STP_r_decDbiB %d |%s|\n",*iNr,*cbuf);


    
  p1 = *cbuf;

  L_start0:
  if(*p1 == '(') {
    ++p1;

  } else if(*p1 == ' ') {
    ++p1;
    goto L_start0;

  } else {
    TX_Error("STP_r_decDbB E001 |%s|",*cbuf);
    return -2;
  }


    
  L_start1:
  if(!strncmp(p1, "PARAMETER_VALUE", 15)) {
              //   123456789012345
    p1 += 15;
    goto L_start1;

  } else if(*p1 == '(') {
    ++p1;

  } else if(*p1 == ' ') {
    ++p1;
    goto L_start1;

  } else {
    TX_Error("STP_r_decDbiB E002 |%s|",*cbuf);
    return -2;
  }

  *iNr = 0;

  *cbuf = p1;

  // decode & save iNr doubles; wenn (iNr==0) decode bis ')';
  irc = STP_r_decDbs (iNr, cbuf);

  // skip foll term.
  STP_r_skipTer1 (cbuf);

  return irc;

}


//================================================================
  int STP_r_decDbB (int *iNr, char **cbuf) {
//================================================================
// decode Dbs in Klammer eingebettet
// iNr out:  Anzahl decodierter Dbs.
// RetCod  0 - nach (..) war ein ',' (wird geskippt !)
// RetCod -1 - nach (..) war ein ')' (wird geskippt !)
// RetCod -2 - Fehler; stop.

  int    irc;
  char   *p1;


  // printf("STP_r_decDbB |");UTX_dump_s__(*cbuf);printf("|\n");


  p1 = *cbuf;

  L_start:
  if(*p1 == '(') {
    ++p1;
  } else if(*p1 == ' ') {
    ++p1;
    goto L_start;
  } else {
    TX_Error("STP_r_decDbB E001 |%s|",*cbuf);
    return -2;
  }

  *iNr = 0;

  *cbuf = p1;

  return STP_r_decDbs (iNr, cbuf);
/*
  irc = STP_r_decDbs (iNr, &p1);  // decode & save all Dbs
  if(irc < 0) return irc;


  // nach ')':
  if(*p1 == ',') irc =  0;
  else           irc = -1;            // ')' - End of Record

  ++p1;
  *cbuf = p1;

  return irc;
*/
}


//================================================================
  int STP_r_decDbs (int *iNr, char **cbuf) {
//================================================================
// decode & save iNr doubles; wenn (iNr==0) decode bis ')';
// iNr out:  Anzahl decodierter doubles.
// RetCod  0 - nach Db war ein ',' (wird geskippt !)
// RetCod -1 - nach Db war ein ')' (wird geskippt !)
// RetCod -2 - Fehler; stop.


  int    irc, iend;
  double d1;
  void   *mp1, *p1;


  // printf("STP_r_decDbs %d |",*iNr);UTX_dump_s__(*cbuf);printf("|\n");
  // printd("STP_r_decDbs %d |%s|\n",*iNr,*cbuf);

  iend = *iNr;
  if(iend < 1) iend = 99999;

  *iNr = 0;

  // get next double
  L_nxt_l:
  irc = STP_r_decDb1 (&d1, cbuf);
  if(irc < -1) return irc;             // exit bei Fehler

  *iNr += 1;

  // save double --> s_dat
    // printf("  %d save db %f\n",*iNr,d1);
    // p1 = s_dat.next; printd(" p1=%p\n",p1);
  mp1 = UME_save (&s_dat, &d1, sizeof(double));
  if(!mp1) {TX_Error("STP_r_decDbs E001"); return -3;}
    // printd(" p1=%lf\n",*((double*)p1));

  if(irc == 0) {
    if(*iNr < iend) goto L_nxt_l;
  }

  // printf("ex STP_r_decDbs %d %d\n",irc,*iNr);
  return irc;

}


//================================================================
  int STP_r_decDb1 (double *db, char **cbuf) {
//================================================================
// eine Db ("123.4") decodieren; out als iDb.
// RetCod  0 - nach Db war ein ',' (wird geskippt !)
// RetCod -1 - nach Db war ein ')' (wird geskippt !)
// RetCod -2 - Fehler; stop.

  int   irc, i1;
  char  *p1, *p2, *p3, cd;


  // printf("STP_r_decDb1 |%s|\n",*cbuf);

  p1 = *cbuf;

/*
  // find next ',' od ')'
  p2 = p1;
  for(i1=0; i1<24; ++i1) {
    ++p2;
    if(*p2 == ',') {*p2 = '\0'; irc =  0; goto L_dec;}
    if(*p2 == ')') {*p2 = '\0'; irc = -1; goto L_dec;}
  }
  TX_Error("STP_r_decDb1 E002 |%s|",p1);
  return -2;
  // L_dec:
*/

#ifdef _MSC_VER
  // MS-win bug
  p3 = p1;
  UTX_pos_skip_num (&p3);
  cd = *p3;
  *p3 = '\0';
#endif

  *db = strtod (p1, &p2);

#ifdef _MSC_VER

  *p3 = cd;
#endif

  *cbuf = p2;

    // printd("ex STP_r_decDb1 %lf\n",*db);

  // skip terminating characters  "," or ")" or "),"
  return STP_r_skipTer1 (cbuf);

}


//================================================================
  int STP_r_decLog1 (int *iVal, char **cbuf) {
//================================================================
// decode Logical (.T./.F.)
// iVal  0 = .T.
// iVal  1 = .F.
// RetCod  0 - nach .T. war ein ',' (wird geskippt !)
// RetCod -1 - nach .T. war ein ')' (wird geskippt !)
// RetCod -2 - Fehler; stop.

  int    irc;
  char   *p1;
  // char   c1;

  p1 = *cbuf;

  L_start:
  if(*p1 == '.') {
    ++p1;

  } else if(*p1 == ' ') {
    ++p1;
    goto L_start;

  } else {
    TX_Error("STP_r_decLog1 E001 |%s|",*cbuf);
    return -2;
  }


  if(*p1 == 'T') {
    // c1 = 0;
    *iVal = 0;
  } else if(*p1 == 'F') {
    // c1 = 1;
    *iVal = 1;
  } else {
    TX_Error("STP_r_decLog1 E002 |%s|",*cbuf);
    return -2;
  }

  ++p1;

  if(*p1 == '.') {
    ++p1;
  } else {
    TX_Error("STP_r_decLog1 E003 |%s|",*cbuf);
    return -2;
  }


  // // save 1 byte --> s_dat
  // UME_save (&s_dat, &c1, 1);


  // nach ')':
  if(*p1 == ',') irc =  0;
  else           irc = -1;            // ')' - End of Record

  ++p1;
  *cbuf = p1;

  // printf("ex STP_r_decLog1 irc=%d log=%d\n",irc,*iVal);

  return irc;

}


//================================================================
  int STP_r_decLink2B (char **cbuf) {
//================================================================
// STP_r_decLink2B     decode sqare-block of links

  int      irc, *iap, i1, i2;
  char     *p1;


  // printf("STP_r_decLink2B |");UTX_dump__(*cbuf,60);printf("|\n");


  p1 = *cbuf;

  L_start1:
  if(*p1 == '(') {
    ++p1;
  } else if(*p1 == ' '){
    ++p1;
    goto L_start1;
  } else {
    TX_Error("STP_r_decLink2B E001 |%s|",*cbuf);
    return -2;
  }


  // reserve space for 2 dimensions
  iap = s_dat.next;
  irc = UME_add (&s_dat, sizeof(int) * 2);
  if(irc < 0) {TX_Error("STP_r_decLink2B EOM1"); return -4;}


  i2 = 0; // Anzahl blocks

  L_nxt_blk:
  ++i2;
  irc = STP_r_decLinkB (&i1, &p1);
  if(irc < 0) {
    if(irc == -2) goto L_weiter;
  }

    // printf(" n.decLinkB |%s|\n",p1);
  while (*p1  == ' ')  {++p1;}
  if(*p1 == '(') goto L_nxt_blk;


  L_weiter:
  iap[0] = i1;
  iap[1] = i2;
    // printf(" uNr=%d vNr=%d\n",i1,i2);


  // skip terminating characters  "," or ")" or "),"
  irc = STP_r_skipTer1 (&p1);

  *cbuf = p1;

  // printf("ex STP_r_decLink2B irc=%d |",irc);
  // UTX_dump__ (p1,60); printf("|\n");

  return 0;

}


//================================================================
  int STP_r_decDb2B (char **cbuf) {
//================================================================
// STP_r_decDb2B     decode sqare-block of doubles
// Gespeichert werden zuerst die 2 Dimensions, dann die Werte.


  int      irc, *iap, i1, i2;
  char     *p1;


  // printf("STP_r_decDb2B |");UTX_dump__(*cbuf,60);printf("|\n");


  p1 = *cbuf;

  if(*p1 == '(') {
    ++p1;
  } else {
    TX_Error("STP_r_decDb2B E001 |%s|",*cbuf);
    return -2;
  }


  // reserve space for 2 dimensions
  iap = s_dat.next;
  irc = UME_add (&s_dat, sizeof(int) * 2);
  if(irc < 0) {TX_Error("STP_r_decDb2B EOM1"); return -4;}


  i2 = 0; // Anzahl blocks

  L_nxt_blk:
  ++i2;
  irc = STP_r_decDbB (&i1, &p1);
  if(irc < 0) {
    if(irc < -1) return irc;
  }

    // printf(" n.decLinkB |");UTX_dump__(p1,60);printf("|\n");

  while(*p1 == ' ')  ++p1;
  if(*p1 == '(') goto L_nxt_blk;


  L_weiter:
  iap[0] = i1;
  iap[1] = i2;
    // printf(" uNr=%d vNr=%d\n",i1,i2);


  // nach ')':
  if(*p1 == ',') irc =  0;
  else           irc = -1;            // ')' - End of Record

  ++p1;
  *cbuf = p1;

  // printf("ex STP_r_decDb2B %d %d\n",i1,i2);

  return 0;

}



//================================================================
  int STP_r_decLinkxB (int *iNr, char **cbuf) {
//================================================================
// decode Links in Klammer eingebettet mit Codewort
// zB "( CARTESIAN_POINT( #3368 ) ),"
// Codewort wird geskippt.
// iNr out:  Anzahl decodierter Links.
// RetCod  0 - nach (..) war ein ',' (wird geskippt !)
// RetCod -1 - nach (..) war ein ')' (wird geskippt !)
// RetCod -2 - Fehler; stop.

  int    irc;
  char   *p1;

  // printf("STP_r_decLinkxB |%s|\n",*cbuf);

  p1 = *cbuf;

  // skip '('
  L_start1:
  if(*p1 == '(') {
    ++p1;
  } else if(*p1 == ' '){
    ++p1;
    goto L_start1;
  } else {
    TX_Error("STP_r_decLinkxB E001 |%s|",*cbuf);
    return -1;
  }


  // skip Codewort; zB CARTESIAN_POINT - find following '('
  p1 = strchr(p1, '(');
  if(p1 == NULL) {
    TX_Error("STP_r_decLinkxB E002 |%s|",*cbuf);
    return -1;
  }


  // decode the Linkblock
  irc = STP_r_decLinkB (iNr, &p1);  // decode & save Link trim1
  if(irc < -1) return irc;



  // skip following ')' oder ','
  // printf(" decLinkxB |%s|\n",p1);


  // skip terminating characters  "," or ")" or "),"
  irc = STP_r_skipTer1 (&p1);


  *cbuf = p1;

  // printf("ex STP_r_decLinkxB |\n");
  // UTX_dump_s__ (p1); printf("|\n");


  return 0;

}


//================================================================
  int STP_r_decLinkB (int *iNr, char **cbuf) {
//================================================================
// decode Links in Klammer eingebettet
// iNr out:  Anzahl decodierter Links.
// - must starts with char '('
// - exists with char after ')'
// RetCod  0 - nach (..) war ein ',' (wird geskippt !)
// RetCod -1 - nach (..) war ein ')' (wird geskippt !)
// RetCod -2 - Fehler; stop.

  int    irc;
  char   *p1, *p2;


  // printd("STP_r_decLinkB %d |%s|\n",*iNr,*cbuf);

  p1 = *cbuf;

  L_start:
  if(*p1 == '(') {
    ++p1;
  } else if(*p1 == ' '){
    ++p1;
    goto L_start;
  } else {
    TX_Error("STP_r_decLinkB E001 |%s|",*cbuf);
    return -2;
  }

  // *iNr = 0;    // 0 = all Links
  irc = STP_r_decLinks (iNr, &p1);  // decode & save Links
  if(irc < 0) goto L_exit;
  // (irc == 1): ')' already skipped


  // skip all until ')'     2017-04-08
  if(irc < 1) {
    // end of block was not ')'; skip all until ')'
    p2 = UTX_pos_skipBrack1(p1);
    if(!p2) {TX_Error("STP_r_decLinkB E000|%s|",*p1); return -1;}
    p1 = ++p2; // skip ')'
  }


  *cbuf = p1;


  L_exit:
    // printd("ex STP_r_decLinkB irc=%d iNr=%d |%s|\n",irc,*iNr,*cbuf);
    // UTX_dump_s__ (*cbuf); printf("|\n");

  return irc;

}


//================================================================
  int STP_r_decLinks (int *iNr, char **cbuf) {
//================================================================
// decode iNr Links; wenn (iNr==0) decode bis ')';
// iNr out:  Anzahl decodierter Links.
// ein '$' wird als Link -1 gepeichert.
// - must start with char '#'
// - exits with char after ',' or ')'  (see RetCod)
// RetCod  2 - OK; last char was ');' (skipped)
// RetCod  1 - OK; last char was ')' (skipped)
// RetCod  0 - OK
// RetCod -2 - Fehler; stop.

  int   irc, i1, iend;
  void  *mp1;


  // printd("STP_r_decLinks %d |%s|\n",*iNr,*cbuf);

  iend = *iNr;
  if(iend < 1) iend = 99999;

  *iNr = 0;

  // get next Link
  L_nxt_l:
  irc = STP_r_decLink1 (&i1, cbuf);
  if(irc < 0) return irc;

  *iNr += 1;

  // save Link --> s_dat
  mp1 = UME_save (&s_dat, &i1, sizeof(int));
  if(!mp1) return -2;
    // printd("  %d saved-link %d\n",*iNr,i1);

  if(irc == 0) {
    if(*iNr < iend) goto L_nxt_l;

  } else {
    if(**cbuf == ';') {
      *cbuf += 1;
      irc = 2;
    }
  }


  // printd("ex STP_r_decLinks irc=%d iNr=%d |%s|\n",irc,*iNr,*cbuf);
  // UTX_dump_s__ (*cbuf); printf("|\n");

  return irc;

}
 
 
//================================================================
  int STP_r_decLink1 (int *iLink, char **cbuf) {
//================================================================
// einen Link (#77) decodieren; out als iLink.
// Link == '$': iLink = -1
// - must start with char '#'
// - exits with char after ',' or ')'  (see RetCod)
// RetCod  1 - nach Link war ein ')' (wird geskippt !)
// RetCod  0 - nach Link war ein ',' (wird geskippt !)
// RetCod -2 - Fehler; stop.

  int   irc, i1;
  char  *p1, *p2;


  printd("STP_r_decLink1 |%s|\n",*cbuf);

  p1 = *cbuf;

  // first char muss '#' sein
  L_start:
  if(*p1 != '#') {
    if(*p1 == ' ') {++p1; goto L_start;}
    if(*p1 != '$') {
      TX_Error("STP_r_decLink1 E001 |%s|",p1);
      return -2;
    }

  } else {
    ++p1;
  }


  // find next ',' od ')'
  p2 = p1;
  for(i1=0; i1<12; ++i1) {
    ++p2;
    if(*p2 == ')') {
      *p2 = '\0';  // fuer atoi
      irc = 1; 
      ++p2;
      // wenn nachfolgend noch ein ',' -  remove it !
      L_l3:
      if(*p2 == ' ') {++p2; goto L_l3;}
      if(*p2 == ',') ++p2;
      goto L_dec;

    } else if(*p2 == ',') {
      *p2 = '\0';
      irc =  0;
      ++p2;
      goto L_dec;
    }
  }

  TX_Error("STP_r_decLink1 E002 |%s|",p1);
  return -2;



  L_dec:
  if(*p1 == '$') {
    *iLink = -1;
  } else {
    *iLink = atoi (p1);
  }

  *cbuf = p2;

  // printd("ex STP_r_decLink1 irc=%d iLink=%d |%s|\n",irc,*iLink,*cbuf);
  // UTX_dump_s__ (p2); printf("|\n");

  return irc;

}


//================================================================
   int STP_r_decTx1 (char **sPos, int mode, char **cbuf) {
//================================================================
// STP_r_decTx1           decode & save Textfield ('xxx') 
// Input:
//   mode: 1=change blanks into '_'
// Output:
//   sPos  pointer to stored text
//
// save nr of i4-words first; next saves text as i4-words.
// get text: ia = s_tab[ii].sDat; s1 = (char*)&ia[1];
// see also STP_r_getInt

  int    i1, i2;
  char   *p1, *p2;

  // printf("STP_r_decTx1 |%s|\n",*cbuf);

  p1 = *cbuf;

  L_start:
  if(*p1 == '\'') {
    ++p1;
  } else if(*p1 == ' ') {
    ++p1;
    goto L_start;
  } else {
    TX_Error("STP_r_decTx1 E001 |%s|",*cbuf);
    return -2;
  }

  p2 = strchr(p1, '\'');
  if(p2 == NULL) {
    TX_Error("STP_r_decTx1 E002 |%s|",*cbuf);
    return -1;
  }

  *p2 = '\0';

  if(mode == 1) {  // 1=change blanks into '_'
    UTX_chg_chr1 ('_', ' ', p1);
  }

  i1 = p2 - p1;
  // printf(" txLen=%d\n",i1);
  // i2 = Anzahl words for i1 chars
  i2 = UTI_iNr_chrNr (i1);
  // printf(" iLen=%d\n",i2);


  // save TextLen
  if(UME_save (&s_dat, &i2, sizeof(int)) == NULL) 
    {TX_Error("STP_r_decTx1 EOM1"); return -4;}

  // save Text
  *sPos = UME_save (&s_dat, p1, sizeof(int)*i2);
  if(*sPos == NULL)
    {TX_Error("STP_r_decTx1 EOM2"); return -4;}


  // skip the following ',' od ')'
  ++p2;
  ++p2;
  *cbuf = p2;

  return 0;

}


//================================================================
  int STP_r_nxtSrec () {
//================================================================
// set s_Nr to Index of s_tab.
// Init s_tab; set dataspaceadress

// RetCod -4  EOM
 
  ++s_Nr;


  // printf("STP_r_nxtSrec init s_tab[%d]\n",s_Nr);


  if(s_Nr >= s_Siz) {
    if(STP_r_alloc_tab(0) < 0) {TX_Error("STP_r_nxtSrec EOM"); return -4;}
  }


  s_tab[s_Nr].sInd = s_Ind;        // wird momentan decodiert ..
  s_tab[s_Nr].sDat = s_dat.next;   // set dataspaceadress
  s_tab[s_Nr].gDat = NULL;

  s_tab[s_Nr].gInd = -1;           // empty
  s_tab[s_Nr].gTyp =  0;           // keine neg. Zahlen moeglich !
  s_tab[s_Nr].stat =  0;           // unused ..

  return 0;

}

//================================================================
  char *STP_r_TypTxt_sTyp (int sTyp) {
//================================================================
// styp-text from sTyp-Nr

  static char cbuf[64];

  // printf("STP_r_TypTxt_sTyp %d\n",sTyp);

  switch (sTyp) {
    case  1: return "DIRECTION";
    case  2: return "CARTESIAN_POINT";
    case  3: return "VERTEX_POINT";
    case  4: return "VECTOR";
    case  5: return "AXIS1_PLACEMENT";
    case  6: return "AXIS2_PLACEMENT_3D";
    case  7: return "LINE";
    case  8: return "CIRCLE";
    case  9: return "ELLIPSE";

    case 10: return "COMPOSITE_CURVE";
    case 11: return "COMPOSITE_CURVE_SEGMENT";
    case 12: return "TRIMMED_CURVE";
    case 13: return "QUASI_UNIFORM_CURVE";
    case 14: return "B_SPLINE_CURVE";
    case 15: return "B_SPLINE_CURVE_WITH_KNOTS";
    case 16: return "RATIONAL_B_SPLINE_CURVE";
    case 17: return "BOUNDED_CURVE";
    case 18: return "INTERSECTION_CURVE";
    case SC_VERTEX_LOOP:                                   // 19
             return "VERTEX_LOOP";

    case 20: return "FACE_OUTER_BOUND";
    case 21: return "FACE_BOUND";
    case 22: return "EDGE_LOOP";
    case 23: return "ORIENTED_EDGE";
    case 24: return "EDGE_CURVE";
    case 25: return "CONNECTED_EDGE_SET";

    case 26: return "ITEM_DEFINED_TRANSFORMATION";
    case 27: return "NEXT_ASSEMBLY_USAGE_OCCURRENCE";
    case 28: return "COLOUR_RGB";

    case 30: return "ADVANCED_FACE";
    case 31: return "PLANE";
    case 32: return "SPHERICAL_SURFACE";
    case 33: return "CYLINDRICAL_SURFACE";
    case 34: return "CONICAL_SURFACE";
    case 35: return "TOROIDAL_SURFACE";
    case 36: return "DEGENERATE_TOROIDAL_SURFACE";
    case 37: return "SURFACE_OF_REVOLUTION";
    case 38: return "SURFACE_OF_LINEAR_EXTRUSION";
    case 39: return "QUASI_UNIFORM_SURFACE";
    case 40: return "B_SPLINE_SURFACE";
    case 41: return "B_SPLINE_SURFACE_WITH_KNOTS";
    case 42: return "RATIONAL_B_SPLINE_SURFACE";
    case 43: return "BOUNDED_SURFACE";

    case 45: return "RECTANGULAR_TRIMMED_SURFACE";

    case 47: return "GEOMETRIC_SET";
    case 48: return "GEOMETRIC_CURVE_SET";

    case 50: return "EDGE_BASED_WIREFRAME_MODEL";
    case 51: return "SHELL_BASED_SURFACE_MODEL";
    case 52: return "OPEN_SHELL";
    case 53: return "CLOSED_SHELL";
    case 54: return "MANIFOLD_SOLID_BREP";

    case 60: return "GBOUNDED_SURFACE_SR";
    case 61: return "GBOUNDED_WIREFRAME_SR";
    case 62: return "EDGE_BASED_WIREFRAME_SR";
    case 63: return "MANIFOLD_SURFACE_SR";
    case 64: return "ADVANCED_BREP_SR";
    case SC_FACETED_BREP_SR:
             return "FACETED_BREP_SHAPE_REPRESENTATION";
    case SC_CONTEXT_DEPENDENT_SR:
             return "CONTEXT_DEPENDENT_SR";

    case 70: return "SHAPE_DEFINITION_REPRESENTAT";
    case 71: return "SHAPE_REPRESENTATION";
    case 72: return "SHAPE_REPRESENTATION_REL.SHIP";
    case 73: return "REPRESENTATION_RELATIONSHIP";

    case 80: return "PRODUCT";
    case 81: return "PRODUCT_DEFINITION_FORMWSS";
    case 82: return "PRODUCT_DEFINITION_WITH_AD";
    case 83: return "PRODUCT_DEFINITION";
    case 84: return "PRODUCT_DEFINITION_SHAPE";

    case 90: return "GEOMETRIC_REPRESENTATION_CONTEXT";
    case 91: return "GLOBAL_UNCERTAINTY_ASSIGNED_CONTEXT";
    case 92: return "UNCERTAINTY_MEASURE_WITH_UNIT";
    case 93: return "LENGTH_MEASURE";
  }

  sprintf(cbuf, "STP_r_TypTxt_sTyp E %d",sTyp);
  return cbuf;

}


//================================================================
 int STP_r_dispSym1 (int typ, Point *pt1) {
//================================================================
// STP_r_dispSym1         display Symbol at Position (star, triang ..)
// typ: 52 = stern rot,
// typ: 63 = triang. gruen,
// typ: 75 = rect. gelb,

  char cbuf[64];

  sprintf(cbuf,"TAG %d",typ);
  AP_obj_add_pt (cbuf, pt1);
  STP_r_creObj1 (-1, Typ_GTXT, Typ_Txt, cbuf);

  return 0;

}
 

//================================================================
  int STP_r_read_start (FILE *fp1) {
//================================================================
// skip all until text "DATA;"

  int  ll;
  char *p1;


  //----------------------------------------------------------------
  // work FILE-section
  // get next Line
  p1 = mem_cbuf1;

  L_nxt_ln1:
    if(fgets (mem_cbuf1, 2048, fp1) == NULL) return -1;

    // skip foll. blanks, CR's, LF's;
    ll = strlen(p1);
    ll -= 1;                // zeigt nun auf last char
    while((p1[ll] == ' ')  ||
          (p1[ll] == '\n') ||
          (p1[ll] == '\r'))     {p1[ll] = '\0';  ll -= 1;}


    // printf(" read_start |%s|\n",mem_cbuf1);

  // skip alles bis "DATA;"
  if(strcmp(mem_cbuf1, "DATA;")) goto L_nxt_ln1;


  return 0;

}
 

//================================================================
  int STP_r_readLn (FILE *fp1) {
//================================================================
/// \code
/// get next step-expr. into mem_cbuf1
/// End of expr = ';'
/// RetCod -1 = EOF
/// \endcode

  int       ll, lMax, l1;
  char      *p1, *p2;
  int       flgComm=0;  // 0=normal, 1=comment_is_active.
  char      *commBeg, *commEnd;


  // printf("STP_r_readLn ======================= \n");

  L_start:
    p1   = mem_cbuf1;
    lMax = mem_cbuf1_SIZ;


  L_get_next:
    if(fgets (p1, mem_cbuf1_SIZ, fp1) == NULL) return -1;
    // read until LF;
    ll = strlen(p1) - 1;    // points to last char



    //----------------------------------------------------------------
    // remove Blanks, Carriage Returns und Linefeeds
    // skip foll. blanks, CR's, LF's;
    L_skip_1:
    if(ll < 0) goto L_start;  // empty line ..
    if((p1[ll] == ' ')  ||
       (p1[ll] == '\n') ||
       (p1[ll] == '\r'))     {
      p1[ll] = '\0';
      ll -= 1;
      goto L_skip_1;
    }
      // printf(" str r ll=%d\n",ll);
    lMax = mem_cbuf1_SIZ - ll ;


#ifdef DEB
  printd("  readLn-1 |%s| ll=%d\n",p1,ll);
#endif
   

    
    //----------------------------------------------------------------
    // skip comments like "/* ...*/"   (ex CATIA V15 !)
    if(!flgComm) {
      p2 = p1;
      while(*p2 == ' ') ++p2;
      // check for comment only at start of line
      if(*p2 == '/') {
        if(p2[1] == '*') {
          // found comment;
          commBeg = p2;
          flgComm = 1;
            // printf(" comment ..\n");
        }
      }
    }


    // find end "*/", remove comment.
    if(flgComm) {
      commEnd = strstr(commBeg, "*/");
      if(commEnd) {
        // found end of string, check remaining nr of chars
        commEnd += 2;
        flgComm = 0;
        l1 = commEnd - commBeg;
          // printf(" stp comm %d %d\n",ll,l1);
        ll -= l1;
        if(ll < 1) goto L_start; // complete line is comment; skip line ..
        // remove commBeg-commEnd, work rest
        memmove (p1, commEnd, ll);
        p1[ll] = '\0';
      } else {
        // test next line
        commBeg = mem_cbuf1;
        goto L_start;
      }
    }



    //----------------------------------------------------------------
    // read until ';' (end of expr.)
    if(p1[ll] != ';') {
      p1 = &p1[ll+1];
      if(lMax < 40) {
        TX_Error("STP_r_readLn E001");
        return -2;
      }
      goto L_get_next;
    }

#ifdef DEB
  printd("  readLn-e |%s| ll=%d\n",p1,ll);
#endif
   

  return 0;

}


//================================================================
  int STP_r_readSubCmd (char **pNxt, char *nxtCmd) {
//================================================================
// remove next Subrecords out of mem_cbuf1
// Inputs: pNxt zeigt auf nachfolgenden Subrecord in mem_cbuf1, dessen
//   ersten 2 chars nxtCmd sind.
// Remove Subrecords beginning at &mem_cbuf1[0], terminate new Subrecord
//   with ";\0"; return these (replaced) characters in nxtCmd.
// ReturnCode: 0=ok, -1=endOfLine.




  int    irc, siz;
  char   *p1;


  // printf("STP_r_readSubCmd |%c%c|\n",nxtCmd[0],nxtCmd[1]);
  // printf("STP_r_readSubCmd |%s|%c%c|\n",pNxt,nxtCmd[0],nxtCmd[1]);


  if(!strncmp(nxtCmd, ");", 2)) return -1;


  p1 = *pNxt;
  siz = strlen(p1+2) + 2;
    // printf(" siz=%d\n",siz);

  // remove das aktive Subcommand;
  memmove (mem_cbuf1, *pNxt, siz+1);


  // die beiden Characters ";\0" wieder durch Originale ersetzen
  strncpy (mem_cbuf1, nxtCmd, 2);


  // den closing ')' suchen; die 2 chars danach nach nxtCmd kopieren und
  // durch ";\0" ersetzen.
  // search first "("
  *pNxt = strchr(mem_cbuf1, '(');
  if(*pNxt == NULL) {
    // TX_Error("STP_r_readSubCmd E001");
    return -1;
  }

  // search corresponding ")"
  *pNxt = UTX_pos_skipBrack (*pNxt);
  if(strlen(*pNxt) < 2) {
    TX_Error("STP_r_readSubCmd E002");
    return -1;
  }

  // save first 2 chars
  ++(*pNxt);
  strncpy(nxtCmd, *pNxt, 2);

  // terminate Subcommand
  strncpy(*pNxt, ";\0", 2);


  // Cr Lf Tab durch blanks ersetzen ..

  // printf("ex STP_r_readSubCmd |%s|%c%c|\n",mem_cbuf1,nxtCmd[0],nxtCmd[1]);
  // printf(" pNxt |%s|\n",(*pNxt)+2);



  return 0;

}


//=================== EOF ========================================
