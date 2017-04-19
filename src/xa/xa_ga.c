// PermanentAttributeList.          RF 2004-01-25.
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
2010-01-02 GA_fil_wri ckExist zu. RF.
2006-03-06 linTypes und sStyles neu zu. RF.
2004-01-25 Neu aus Func. DL_disp_save. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void GA(){}
#endif
/*!
\file  ../xa/xa_ga.c
\brief PermanentAttributeList. 
\code
=====================================================
List_functions_start:

GA_find_att_SU       get attribute for surface/solid from typ,DB-ind
GA_find__            find GA-rec if already exist
GA_getRec            get ga-record
GA_get_dbo           get GraficAttribute for DB-obj
GA_delRec            delete ga-record

GA_hide__            hide mainfunctions ..
GA_hide_fil__        save / restore  - with open File (binary file)
GA_hide_fil_tmp      save Hidelist --> File / restore Hidelist from File
GA_hideTab           Load Liste aus HIDE-TextLine
GA_view__            hide / view obj

GA_lTyp__            modify lineType
GA_LtypTab           load Ltyp-table

GA_sStyl__           modify surfStyle
GA_sStylTab          tabelle von objects laden ..   

GA_Col__             modify color

GA_Tex__             texture-functions (remove/load/apply/update texture)
GA_tex_ga2tr         get TexRef-nr from GA-record-nr
GA_hasTexture        INLINE
GA_getTexRefInd      INLINE

GA_Tra__             modify transparenz

GA_fil_wri           write out the PermanentAttributes (as sourceText)
GA_load__            decode GATAB-Line ?
GA_decode__          decode a runtime-attribute

GA_dump__
GA_dump_1
GA_dump_tex

- internal:
GA_newRec            get recordNr of next free record
GA_creRec
GA_addRec            add new Record
GA_ckEmpty
GA_clean1
GA_realloc

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


Functions for GraficAttributes (struct ObjAtt)

AttributeTable is:
ObjAtt GA_ObjTab[];

/// typ    DB-Typ;      Typ_Error = deleted Record.
/// ind    DB-Index

/// disp   0=normal; 1=hidden
  All objects:
    Object-hidden: ("H")  .disp = 1  

/// iatt   for typ=LN/AC/Curve: Ind_Att_ln
///        for typ=TAG/IMG: sx/sy
///        for typ=Surf/Model: ColRGB


  Line, Circ, Curve:
    Linetyp: ("T")        .iatt = Linetype

  Surfaces & Solids:
    Color:   ("C")        .iatt = Color
    Transparency: ("T")   .vtra =
    Style    ("S")        .vsym
    Texture  ("X")        .vtex


AttributData: in iatt.
  iatt seen as ColRGB or Ind_Att_ln

  if(((ColRGB*)GA_ObjTab[i1].iatt)->vtex != 0)      // this is a texture ..
    b123 = TextRef-ind (cr/cg/cb as ColRGB).
    b4   = is used only in mapping as a ColRGB.

  Function to get the TexRef-ind from the GA-record: GA_tex_ga2tr


Textures: see ../xa/xa_tex.c
 


Funktions:     ../xa/xa_ga.c  (this file)

---------------------------------------------------
*/


#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <string.h>                       /* strcmp ..   */
#include <math.h>                         /* fabs  ...   */
#include <stdio.h>                        /*  FILE .. */
#include <stdlib.h>                       // realloc


#include "../ut/ut_geo.h"                  // OFF, ON ..
#include "../ut/ut_txt.h"                  // fnam_del
#include "../ut/ut_memTab.h"           // MemTab
#include "../ut/ut_col.h"              // COL_INT32
#include "../ut/ut_os.h"               // OS_ ..

#include "../db/ut_DB.h"                   // DB_VCX_IND

#include "../ut/func_types.h"                   // UI_FuncUCB8

#include "../xa/xa_mem.h"                  // memspc51, mem_cbuf1

#include "../xa/xa_ga.h"                   // GA_hasTexture
#include "../xa/xa.h"                      // AP_STAT



//===============================================================
// Externe Variablen:

// aus ../gr/ut_DL.c:
extern DL_Att  *GR_ObjTab;  // das ist die DL
extern long    GR_TAB_IND;  // naechster freier DL-Index

// aus ../xa/xa_tex.c
extern int    TexRefNr;
extern TexRef *TexRefTab;


// ex xa_ui.c:
extern char      UI_stat_view, UI_stat_hide;


// aus xa.c:
// extern int      WC_modact_ind;        // Nr of the active submodel;
extern AP_STAT    AP_stat;
extern ColRGB     AP_actcol;
extern ColRGB     AP_defcol;




//===============================================================
// Interne Variablen:
ObjAtt  *GA_ObjTab=NULL;         // die PermanentAttributeTable
int     GA_recNr=0;              // die aktuelle Anzahl von Records

static int   GA_stat=0;          // 0=unmodified; 1=GA_ObjTab changed.
static long  GA_SIZ = 0;         // size of GA_ObjTab
#define GA_INC_SIZ  10000

static ObjAtt GA_DefRec;





//==== prototypes: ===============================================
  long GA_find__ (int typ, long ind);
  long GA_newRec ();
  long GA_addRec ();
  long GA_creRec (int typ, long ind);






///================================================================
  int GA_realloc (int newSiz) {
///================================================================
/// rc  0:  OK;
/// rc -2:  EOM. Exit.

  int    iniFlag;

  // printf(" GGGGGGGGGGGGGGGGG  GA_realloc %d\n",newSiz);


  if(GA_SIZ > newSiz) return 0;


  if(GA_ObjTab == NULL) iniFlag = 1;
  else iniFlag = 0;


  while (newSiz >= GA_SIZ) GA_SIZ += GA_INC_SIZ;  // minSiz = GA_SIZ

  printf("::GA_realloc %d %ld\n",newSiz,GA_SIZ);
  GA_ObjTab = (ObjAtt*)realloc(GA_ObjTab, GA_SIZ*sizeof(ObjAtt));
  if(GA_ObjTab == NULL) {
    TX_Error("GA_realloc EOM");
    return -2;
  }



  if(iniFlag == 1) {                    // init
    // init GA_DefRec
    GA_DefRec.ind     = 0;
    GA_DefRec.typ     = 0;
    GA_DefRec.iatt    = 0;
    GA_DefRec.lay     = 0;
    GA_DefRec.disp    = 0;
    // GA_DefRec.vtra    = 0;
    // GA_DefRec.vsym    = 0;
  }


  // printf("ex GA_realloc %d %d\n",newSiz,GA_SIZ);

  return 0;

}


/*
///===============================================================
  int GA_hide_fil_tmp (int mode) {
///===============================================================
/// mode = 1    save Hidelist --> File  (bin)
/// mode = 2    restore Hidelist from File  (bin)


  char   fmode[4], fnam[256];
  FILE   *fp1;


  // printf("GA_hide_fil_tmp %d %d\n",mode,GA_recNr);


  sprintf(fnam, "%shid.dat",OS_get_tmp_dir());

  if(mode == 1) strcpy(fmode, "w");
  else          strcpy(fmode, "r");

  if((fp1=fopen(fnam,fmode)) == NULL) {
    TX_Print("GA_hide_fil_tmp E001\n",fnam);
    return -1;
  }

  // save/restore hidetable
  GA_hide_fil__ (mode, fp1);

  fclose(fp1);


  return 0;

}



///===============================================================
  int GA_hide_fil__ (int mode, FILE *fp1) {
///===============================================================
/// write/read the GA-Tab to/from file

  // printf("GA_hide_fil__ %d\n",mode);


  if(mode == 1) {          // write

    fwrite(&TexRefNr, sizeof(int),  1, fp1);
    if(TexRefNr > 0)
    fwrite(TexRefTab, sizeof(TexRef), TexRefNr, fp1);

    fwrite(&GA_recNr, sizeof(int),  1, fp1);
    if(GA_ObjTab > 0)
    fwrite(GA_ObjTab, sizeof(ObjAtt), GA_recNr, fp1);

    GA_parent_file (mode, fp1);  // write|read ParentTable


  } else if(mode == 2) {   // read

    fread(&TexRefNr, sizeof(int),  1, fp1);
    if(TexRefNr > 0)
    fread(TexRefTab, sizeof(TexRef), TexRefNr, fp1);

    fread(&GA_recNr, sizeof(int),  1, fp1);
    if(GA_ObjTab > 0)
    fread(GA_ObjTab, sizeof(ObjAtt), GA_recNr, fp1);

    GA_parent_file (mode, fp1);  // write|read ParentTable
  }



  return 0;

}
*/

//================================================================
  int GA_fil_wri (FILE *fpo, int iGrp, int src, int ckExist) {
//================================================================
/// \code
/// write out the PermanentAttributes. HIDE, G#, SSTYLS ..
/// see Mod_savSubBuf1 DL_wri_dynDat0 Grp_exp
/// Input:
///   iGrp    0 = all Obj's;
///           1 = only objs's in Group.
///   src     0=UTF_FilBuf0; 1=UTF_FilBuf1
///   ckExist 0=no, do not check; 1=yes.
/// \endcode




  int    irc, i1, i2, typ;
  long   gaNr;
  char   cbuf[256], *p1;
  TexRef *txr;
  TexBas *txb;
  ObjGX  ox1;


  // printf("GA_fil_wri grp=%d nr=%d\n",iGrp,GA_recNr);
  // printf("  WC_modact_ind=%d\n",WC_modact_ind);
  // printf("  WC_modact_nam=|%s|d\n",WC_modact_nam);


  if(GA_recNr < 1) return -1;


  fprintf(fpo, "GATAB\n");


  for(gaNr=0; gaNr<GA_recNr; ++gaNr) {
      // printf(" ....%d\n",gaNr);
      // GA_dump_1 (gaNr);


    typ = GA_ObjTab[gaNr].typ;
    // skip delete-record
    if(typ == Typ_Error) continue;

    // Obj. mit neg. Index skippen (Teile von dittos)
    if(GA_ObjTab[gaNr].ind < 0) continue;

    // check if obj is in group
    if(iGrp == 1)
      if(Grp_ck_def(GA_ObjTab[gaNr].typ, GA_ObjTab[gaNr].ind) < 0) continue;


    //----------------------------------------------------------------
    // make name from typ and DB-index
    APED_oid_dbo__ (mem_cbuf1, typ, GA_ObjTab[gaNr].ind);
      // printf("  GA_fil_wri nxt %d |%s|\n",gaNr,mem_cbuf1);


    // check if object still exists ..
    //----------------------------------------------------------------
    // geht bei STP_r_creMod_Run (Step-Import) ned !
    if(ckExist) {
      ox1 = DB_GetObjGX (typ, GA_ObjTab[gaNr].ind);
      if(ox1.typ == Typ_Error) continue;  // skip GA-Record; object is deleted ..
    }

    // ACHTUNG: funktioniert dzt nur mit MainModel !
    // APED_search_defLn extrem langsam !!!!!!!
    // irc = APED_search_defLn (&p1, &i1, &i2, mem_cbuf1, -1, src);
    // if(irc < 0) continue;  // skip GA-Record; object is deleted ..


    //----------------------------------------------------------------
    i2 = 0;


    //----------------------------------------------------------------
    // add Hide-Info
    if(GA_ObjTab[gaNr].disp != 0) {
      strcat(mem_cbuf1, " H");
      ++i2;
    }


    //----------------------------------------------------------------
    if((typ == Typ_SUR)||
       (typ == Typ_SOL))   goto L_surf;


    //----------------------------------------------------------------
    // Line, Circ, Curve:
    if(GA_ObjTab[gaNr].iatt != 0) {
      // printf(" lTyp:\n");
      sprintf(cbuf, " T%d", GA_ObjTab[gaNr].iatt);
      strcat(mem_cbuf1, cbuf);
      ++i2;
    }

    goto L_write;



    //----------------------------------------------------------------
    //----------------------------------------------------------------
    L_surf:
    // add Color "C.."
    if(((ColRGB*)&GA_ObjTab[gaNr].iatt)->color != 0) {
      // printf(" color:\n");
      sprintf(cbuf, " C%02x%02x%02x", ((ColRGB*)&GA_ObjTab[gaNr].iatt)->cr,
                                      ((ColRGB*)&GA_ObjTab[gaNr].iatt)->cg,
                                      ((ColRGB*)&GA_ObjTab[gaNr].iatt)->cb);
      strcat(mem_cbuf1, cbuf);
      ++i2;
    }


    //----------------------------------------------------------------
    // add Transparenz "T#"
    if(((ColRGB*)&GA_ObjTab[gaNr].iatt)->vtra != 0) {
      sprintf(cbuf, " T%d", ((ColRGB*)&GA_ObjTab[gaNr].iatt)->vtra);
      strcat(mem_cbuf1, cbuf);
      ++i2;
    }


    //----------------------------------------------------------------
    // add Style "S#"
    if(((ColRGB*)&GA_ObjTab[gaNr].iatt)->vsym != 0) {
      sprintf(cbuf, " S%d", ((ColRGB*)&GA_ObjTab[gaNr].iatt)->vsym);
      strcat(mem_cbuf1, cbuf);
      ++i2;
    }


    //----------------------------------------------------------------
    // add Texture X"Filename",sx,sy,dx,dy,ra,vx,vz
    if(((ColRGB*)&GA_ObjTab[gaNr].iatt)->vtex != 0) {
      // get the TexRef-Record from iatt
      i1 = ((stru_c3c1*)&GA_ObjTab[gaNr].iatt)->b123;
      Tex_getRef (&txr, i1);   
      // get the TexBas-Record from txr->ibas
      i1 = txr->ibas;
      Tex_getBas__ (&txb, i1);   
      // get filename
      sprintf(cbuf, " X\"%s\",",txb->fnam);
      UTX_add_fl_u2(cbuf, 1. / txr->uscx, 1. / txr->uscy);
      strcat(cbuf, ",");
      UTX_add_fl_u2(cbuf, txr->udx,txr->udy);
      strcat(cbuf, ",");
      UTX_add_fl_u(cbuf, txr->uar);
      // strcat(cbuf, ",");
      // die Vektoren txr->vx,vy
      // p1 = &cbuf[strlen(cbuf)];
      // AP_obj_add_vc (p1, &txr->vx);
      // strcat(p1, ",");
      // p1 = &p1[strlen(p1)];
      // AP_obj_add_vc (p1, &txr->vy);
      strcat(mem_cbuf1, cbuf);
      ++i2;
    }



    //----------------------------------------------------------------
    L_write:
    if(i2 > 0) fprintf(fpo, "%s\n", mem_cbuf1);


  }


  fprintf(fpo, "ENDGATAB\n");

  // printf("ex GA_fil_wri nxt\n");

  return 0;

}




/* Alte Version mit HIDE und SSTYLS
//================================================================
  int GA_fil_wri (FILE *fpo, int iGrp) {
//================================================================
// write out the PermanentAttributes. HIDE, G#, SSTYLS ..
// see Mod_savSubBuf1 DL_wri_dynDat0 Grp_exp
// iGrp 0 = all Obj's;
//      1 = only objs's in Group.

  int    i1, i2, aNr;
  long   gaNr;
  char   cbuf[32];


  // printf("GA_fil_wri %d\n",GA_recNr);


  if(GA_recNr < 1) return -1;




  //----------------------------------------------------------------
  // write HIDE-Line; zB "HIDE L20 .."

  // create textCode from Hidelist --> mem_cbuf1
  strcpy(mem_cbuf1, "HIDE");

  i2 = 0;
  for(gaNr=0; gaNr<GA_recNr; ++gaNr) {
    if(GA_ObjTab[gaNr].typ == Typ_Error) continue; // skip delete-record
    // Obj. mit neg. Index skippen (Teile von dittos)
    if(GA_ObjTab[gaNr].ind < 0) continue;
    // skip normal (not-hidden)
    if(GA_ObjTab[gaNr].disp < 1) continue;
    // check if obj is in group
    if(iGrp == 1)
      if(Grp_ck_def(GA_ObjTab[gaNr].typ, GA_ObjTab[gaNr].ind) < 0) continue;

    APED_oid_dbo__(cbuf, GA_ObjTab[gaNr].typ, GA_ObjTab[gaNr].ind);
    strcat(mem_cbuf1, " ");
    // printf("  add |%s|\n",cbuf);
    strcat(mem_cbuf1, cbuf);
    ++i2;
  }

  // write HIDE-Line
  if(i2 > 0) fprintf(fpo, "%s\n", mem_cbuf1);



  //----------------------------------------------------------------
  // write Linetypes; zB "G2 L20 .."

  aNr = DL_GetAttNr(); // Anzahl definierter Attribute

  for(i1=1; i1<aNr; ++i1) {          // 0=DefaultLineTyp
    sprintf(mem_cbuf1, "G%d",i1);
    i2 = 0;
    for(gaNr=0; gaNr<GA_recNr; ++gaNr) {
      if((GA_ObjTab[gaNr].typ == Typ_Error) ||          // skip delete-record
         (GA_ObjTab[gaNr].typ == Typ_SUR)   ||          // skip surfs/Models
         (GA_ObjTab[gaNr].typ == Typ_SOL)   ||          // skip surfs/Models
         (GA_ObjTab[gaNr].typ == Typ_Model) ||
         (GA_ObjTab[gaNr].typ == Typ_Tag)) continue;    // skip TAG/IMG
      // Obj. mit neg. Index skippen (Teile von dittos)
      if(GA_ObjTab[gaNr].ind < 0) continue;
      // skip all other linetypes
      if(GA_ObjTab[gaNr].iatt != i1) continue;
      // check if obj is in group
      if(iGrp == 1)
        if(Grp_ck_def(GA_ObjTab[gaNr].typ, GA_ObjTab[gaNr].ind) < 0) continue;
  
      APED_oid_dbo__ (cbuf, GA_ObjTab[gaNr].typ, GA_ObjTab[gaNr].ind);
      strcat(mem_cbuf1, " ");
      // printf("  add |%s|\n",cbuf);
      strcat(mem_cbuf1, cbuf);
      ++i2;
    }
    if(i2 > 0) fprintf(fpo, "%s\n", mem_cbuf1);
  }



  //----------------------------------------------------------------
  // write SurfaceStyle symbolic SSTYLS

  sprintf(mem_cbuf1, "SSTYLS",i1);

  i2 = 0;
  for(gaNr=0; gaNr<GA_recNr; ++gaNr) {
    if((GA_ObjTab[gaNr].typ == Typ_SOL)   &&
       (GR_ObjTab[gaNr].typ == Typ_SUR)) continue;
    // Obj. mit neg. Index skippen (Teile von dittos)
    if(GA_ObjTab[gaNr].ind < 0) continue;
    // skip normal visible surfs
    if(((ColRGB*)&(GA_ObjTab[gaNr].iatt))->vsym != 1) continue;
    // check if obj is in group
    if(iGrp == 1)
      if(Grp_ck_def(GA_ObjTab[gaNr].typ, GA_ObjTab[gaNr].ind) < 0) continue;

    APED_oid_dbo__ (cbuf, GA_ObjTab[gaNr].typ, GA_ObjTab[gaNr].ind);
    strcat(mem_cbuf1, " ");
    // printf("  add |%s|\n",cbuf);
    strcat(mem_cbuf1, cbuf);
    ++i2;
  }
  if(i2 > 0) fprintf(fpo, "%s\n", mem_cbuf1);





  return 0;

}
*/


///================================================================
  long GA_addRec () {
///================================================================
/// eine neuen GA-Record anlegen und desen Nr retournieren ..

  long gaNr;

  if(GA_recNr >= GA_SIZ) {
    gaNr = GA_realloc (GA_recNr);
    if(gaNr < 0) return gaNr;
  }
  gaNr = GA_recNr;
  ++GA_recNr;

  // printf("ex GA_addRec %d\n",gaNr);
  return gaNr;

}


//================================================================
  int GA_decode__ (char *cbuf) {
//================================================================
/// \code
/// GA_decode__          decode a runtime-attribute
/// Example: 
/// ":ATTRIB:A20 C39cc30"          sets color for A20
/// runtime-attributes become disactivated after beeing worked (in ED_Run).
/// The Attributes are stored even if the obj does not yet exist.
///   (attributes can be defined before creating the obj).
/// \endcode

  int    typ, i1, i2, i3;
  long   dbi;
  char   *p1;


  // printf("GA_decode__ |%s|\n",cbuf);

  // decode objtyp & index
  APED_dbo_oid (&typ, &dbi, cbuf);


  // proceed to next word
  p1 = UTX_pos_skipWord (cbuf);
  // if(! *p1) 
  ++p1; // skip deli
    // printf(" typ=%d dbi=%ld code |%s|\n",typ,dbi,p1);


  // dispatch
  //----------------------------------------------------------------
  if(*p1 == 'C') {
    ++p1;
    sscanf(p1,"%02x%02x%02x",&i1,&i2,&i3);   // get col out of string
      // printf(" rgb = %d %d %d\n",i1,i2,i3);
    Col_set__ (&AP_actcol, i1, i2, i3);
    GA_Col__ (-1L, 0, typ, dbi);



  //----------------------------------------------------------------
  } else if(*p1 == 'H') {
    ++p1;
    GA_view__ (-1L, 1, typ, dbi);



  //----------------------------------------------------------------
  } else {
    TX_Print("Error GA_decode__ %s",cbuf);
  }


  return 0;

}


//===============================================================
  int GA_load__ (char *cbuf) {
//===============================================================
///  parameter ist die GATAB-Line; mem_cbuf1
///  wenn parameter == NULL, dann delete GA-Tab.

  int        i1;
  long       gaNr;
  double     d1, d2;
  char       *cp1, *cp2, *cpe, symNam[128];
  ColRGB     *col;
  TexBas     *tbAct;
  TexRef     *trAct;
  TexRef     txr;



  if(cbuf == NULL) {
    // printf("GA_load__ init\n");
    GA_recNr = 0;    // clear List
    return 0;
  }


  // printf("GA_load__ |%s|\n",cbuf);

  // set end of line
  cpe = &cbuf[strlen(cbuf)];


  // see GA_newRec
  gaNr = GA_addRec ();

  // write defaults;
  GA_ObjTab[gaNr] = GA_DefRec;

  // decode Typ/Index
  GA_ObjTab[gaNr].typ = AP_typ_typChar (cbuf[0]);
  GA_ObjTab[gaNr].ind = strtol(&cbuf[1], &cp1, 10);
  ++cp1; // skip delimiter ' '
    // printf(" %d typ=%d ind=%d |%s|\n",gaNr,
      // GA_ObjTab[gaNr].typ,GA_ObjTab[gaNr].ind,cp1);



  //----------------------------------------------------------------
  // first parameter: 'H'
  if(*cp1 == 'H') {
    cp1 += 2;
    // write new disp
    GA_ObjTab[gaNr].disp = 1;  // 0=normal; 1=hidden
  }



  //----------------------------------------------------------------
  // Line, Circ, Curve:
  if((GA_ObjTab[gaNr].typ == Typ_SUR) ||
     (GA_ObjTab[gaNr].typ == Typ_SOL))     goto L_surf;



  //----------------------------------------------------------------
  // nxt parameter: 'T'
  if(*cp1 == 'T') {
    ++cp1;
    GA_ObjTab[gaNr].iatt = strtol(cp1, &cp1, 10);;
    ++cp1;
  }




  goto L_done;




  //----------------------------------------------------------------
  //----------------------------------------------------------------
  L_surf:
  // nxt parameter: 'C'
  if(*cp1 == 'C') {
    ++cp1;
    cp2 = (char*)&GA_ObjTab[gaNr].iatt;
    sscanf(cp1,"%02x%02x%02x",(unsigned int *)&cp2[0],
                              (unsigned int *)&cp2[1],
                              (unsigned int *)&cp2[2]);
    ((ColRGB*)&GA_ObjTab[gaNr].iatt)->color = 1;
    cp1 += 7;
    if(cp1 >= cpe) goto L_done;
  }


  //----------------------------------------------------------------
  // nxt parameter: 'T'
  if(*cp1 == 'T') {
    ++cp1;
    i1 = atoi(cp1);
    ((ColRGB*)&GA_ObjTab[gaNr].iatt)->vtra = i1;
    cp1 += 2;
    if(cp1 >= cpe) goto L_done;
  }


  //----------------------------------------------------------------
  // nxt parameter: 'S'
  if(*cp1 == 'S') {
    ++cp1;
    i1 = atoi(cp1);
    ((ColRGB*)&GA_ObjTab[gaNr].iatt)->vsym = i1;
    cp1 += 2;
  }


  //----------------------------------------------------------------
  // nxt parameter: 'X'
  if(*cp1 == 'X') {
    if(AP_stat.texture == 0) goto L_exit;
    cp1 += 2;
    // get TextureFilename --> memspc011
    cp1 = UTX_cp_word_term (symNam, cp1, '"');
      // printf(" fn=|%s|\n",memspc011);
    ++cp1;  // skip ','
    // get Parameters: scx,scy,udx,udy,uar
    cp1 = UTX_2db_tx (&d1, &d2, cp1);  txr.uscx = 1. / d1;  txr.uscy = 1. / d2;
      // printf(" sc=%f,%f\n",d1,d2);
    ++cp1;  // skip ','
    cp1 = UTX_2db_tx (&d1, &d2, cp1);  txr.udx = d1;   txr.udy = d2;
      // printf(" d=%f,%f\n",d1,d2);
    ++cp1;  // skip ','
    d1 = UTX_db_tx (&cp1, cp1);        txr.uar = d1;
      // printf(" a=%f\n",d1);
    ++cp1;  // skip ','

    // get BitmapFile -> <tempDir>
    // TexBas generieren, Texture laden
    // load bitmage-imagefile -> OpenGL-Textur.
    i1 = Tex_addBas1 (symNam, 0);
    if(i1 < 0) goto L_exit;

    // TexRef generieren ...
    txr.ibas = i1;   // = TexBasAct;  set index of basic-texture
    txr.vx   = UT3D_VECTOR_NUL;
    txr.vy   = UT3D_VECTOR_NUL;
    // i1 = GA_ck_TEXR (GA_ObjTab[gaNr].typ, GA_ObjTab[gaNr].ind);
    i1 = Tex_addRef ();          // create TexRef
    Tex_savRef (&txr);           // save TexRef
    // den GA_ObjTab-Record erzeugen ...
    ((stru_c3c1*)&GA_ObjTab[gaNr].iatt)->b123 = i1;
    ((ColRGB*)&GA_ObjTab[gaNr].iatt)->vtex = 1;
      // exit(0);
  }




  //----------------------------------------------------------------
  L_done:
  GA_stat  = 1;  // modified


    // GA_dump_1 (gaNr);
    // Tex_dump__ (NULL);
    // exit (0);
    // printf("ex GA_load__|\n");

  L_exit:

  return 0;

}


///===============================================================
  int GA_sStylTab (int aus_anz, int *aus_typ, double *aus_tab) {
///===============================================================
/// eine SSTYLS-tabelle von objects laden ..
/// zB "G2 L20 .."

  int  i1;
  long ind;


  // printf("GA_sStylTab\n");
  // for(i1=0;i1<aus_anz;++i1) printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);

  // create/modify Ltyp-Records ..
  for(i1=0;i1<aus_anz;++i1) {
    ind = aus_tab[i1];
    GA_sStyl__ (-1L, 2, aus_typ[i1], ind);
  }

  return 0;

}


///===============================================================
  int GA_LtypTab (int aus_anz, int *aus_typ, double *aus_tab) {
///===============================================================
/// eine Ltyp-tabelle von objects laden ..
/// zB "G2 L20 .."

  int  i1, lTyp;
  long ind;


  // printf("GA_LtypTab\n");
  // for(i1=0;i1<aus_anz;++i1) printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);

  lTyp = aus_tab[0];

  // create/modify Ltyp-Records ..
  for(i1=1;i1<aus_anz;++i1) {
    ind = aus_tab[i1];
    GA_lTyp__ (-1L, lTyp, aus_typ[i1], ind, 1);
  }

  DL_Redraw ();

  return 0;

}


///===============================================================
  int GA_hideTab (int aus_anz, int *aus_typ, double *aus_tab) {
///===============================================================
/// eine hide-tabelle von objects laden ..

  int  i1, i2;
  long ind;


  // printf("GA_hideTab %d\n",GA_recNr);
  // for(i1=0;i1<aus_anz;++i1) printf(" %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);
  // GA_dump__ ();  // dump HideList



  // wenn Tabelle leer: direkt laden.
  if(GA_recNr > 0) goto L_add;

  i1 = GA_realloc (aus_anz);
  if(i1 < 0) return -1;

  for(i1=0; i1<aus_anz; ++i1) {
    // printf(" GA_load__ %d %d %f\n",i1,aus_typ[i1],aus_tab[i1]);

    // write defaults;
    GA_ObjTab[i1] = GA_DefRec;
    // write typ, ind
    GA_ObjTab[i1].typ = aus_typ[i1];
    GA_ObjTab[i1].ind = aus_tab[i1];
    // write new disp
    GA_ObjTab[i1].disp = 1;  // 0=normal; 1=hidden
  }

  GA_recNr = aus_anz;
  goto L_done;



  //----------------------------------------------------------------
  // Tabelle nicht leer; zufuegen
  L_add:
    for(i1=0; i1<aus_anz; ++i1) {
      ind = aus_tab[i1];

      // add Obj typ/ind to HideList
      i2 = GA_view__ (-1L, 1, aus_typ[i1], ind);
      if(i2 < 0) return i2;
    }


  //----------------------------------------------------------------
  L_done:
  GA_stat  = 1;

  return 0;

}


///================================================================
  int GA_view__ (long dli, int mode, int typ, long dbi) {
///================================================================
/// GA_view__            hide / view obj
/// Input:
///   mode   0=view;Redraw,  1=hide;Redraw
///          2=view;noRedraw 3=hide;noRedraw
///   dli    dispListIndex given: typ,dbi unused.
///            if dli not given: dispList is not updated !
///   typ,dbi  dataBaseTyp und -index given: dli unused (set it to -1L)
///
/// Example:
/// GA_view__ (-1, 1, s_tab[sInd].gTyp, s_tab[sInd].gInd);

  int  iFunc, iDraw;
  long gaNr;


  // printf("GA_view__ mode=%d typ=%d dbi=%ld dli=%ld\n",mode,typ,dbi,dli);
  // printf(" UI_stat_view=%d\n",UI_stat_view);



  iFunc = mode & 1;
  iDraw = mode & 2;
    // printf(" iFunc=%d iDraw=%d\n",iFunc,iDraw);


  if(dli >= 0) {
    if(dli >= GR_TAB_IND) {TX_Error("GA_view__ E001");return -1;}
    typ = GR_ObjTab[dli].typ;
    dbi = GR_ObjTab[dli].ind;
  }
    // printf(" typ=%d dbi=%ld\n",typ,dbi);
    // if((typ==50)&&(dbi==4)) gdb_halt ();


  // find or create GA-record
  gaNr = GA_creRec (typ, dbi);
  if(gaNr < 0) return -1;
    // printf(" gaNr=%ld\n",gaNr);
    // if(gaNr==48) gdb_halt ();


  // write new ltyp
  GA_ObjTab[gaNr].disp = iFunc;   // mode; 2013-01-15

  // check if record now is empty
  if(iFunc == 0) GA_ckEmpty (gaNr);

  if(dli < 0) return 0;  // preload = entry vor Draw


  // // modify DL.disp, DL.hili; (do not create new DL-Record)
  // DL_SetInd (dli);   // 2012-10-29

  // save the index in the DL-Record
  // DL_hide__ (dli, mode);
  // if "View" is active: all DL.disp are inverted !
  if(UI_stat_view == 0) iFunc = ICHG01 (iFunc);
  DL_hide__ (dli, iFunc);      // OFF=1=hide; 0=show  // 2012-12-22


  // die GL-DrawFunction mit neuem attInd rufen ..
/*  2012-10-29
  if(typ == Typ_CVTRM) typ = Typ_CV;
  APT_Draw__ (GR_ObjTab[dli].iatt, typ, dbi);
*/

  if(iDraw == 0) DL_Redraw ();

  return 0;

}


///===============================================================
  int GA_hide__ (int mode, long dli, int typ) {
///===============================================================
///   Input:
/// typ  = APT-Typ
/// dli  = mode==0: DB-index; mode==3: DL-Index
/// mode =-1: clear hidden-Elements-Tabelle
/// mode = 1: update (hidden Elements aus Tabelle wieder hiden)
/// mode = 5: set reverseMode (display all hidden objects;
/// mode = 6: set reverseMode off
/// mode = 7: Display Info Hidden about Objects
/// mode = 8: ask state; 0=normal, 1=hidden, -1=notExist


  static int   reverse=0;    // reverseMode (display all hidden objects)

  long  i1, i2, gaNr;
  char  cbuf[32];


  // printf(">>>> GA_hide__ %d %ld %d\n",mode,dli,typ);



  //==========================================================
  if(mode < 0) {
    GA_recNr = 0;    // clear List
    return 0;
  }




/*
  //==========================================================
  // mode = 0: add Obj typ/ind to HideList
  // ERSETZT durch Funktion GA_view__
  if(mode != 0) goto L_hide;

  // printf("HHHH GA_hide__ hide %ld %d\n",dli,typ);

  // reduce typ -> Basistyp (Typ_SURRU -> Typ_SUR ..)
  typ = AP_typ_2_bastyp (typ);


  // find or create GA-record
  gaNr = GA_creRec (typ, dli);
  if(gaNr < 0) return -1;
    // printf(" gaNr=%d\n",gaNr);

  // write new disp
  GA_ObjTab[gaNr].disp = 1;  // 0=normal; 1=hidden

  return 0;
*/


  //==========================================================
  // mode = 1: update (hidden Elements aus Tabelle wieder hiden)
  // Wird von GL_Redraw gerufen.
  // loop durch gesamte DL; test each object if to hide;
  // (copy hide-attribut from GA-Tab --> DL).
  // if yes: set .disp & .hili to OFF (=hidden)
  L_hide:
  if(mode != 1) goto L_rev;

  // printf("   update rev=%d mod=%d typ=%d\n",reverse,GA_stat,typ);

  if(reverse != 0) return 0;

  // typ ist in diesem Fall GL_modified !!
  if((GA_stat == 0)&&(typ == 0)) return 0;
  // if(GA_stat < 1) return 0;
  // printf("   update work %d %d\n",GR_TAB_IND,GA_recNr);

  for(i1=0; i1<GR_TAB_IND; ++i1) {  // loop uebr alle DL-obj's
    // printf(" DL[%d]=%d %d\n",i1,GR_ObjTab[i1].typ,GR_ObjTab[i1].ind);

    for(i2=0; i2<GA_recNr; ++i2) {  // loop ueber alle GA-obj's
      if(GA_ObjTab[i2].ind != GR_ObjTab[i1].ind) continue;
      if(GA_ObjTab[i2].typ == Typ_Error) continue; // skip delete-record
      if(AP_cmp_typ(GA_ObjTab[i2].typ, GR_ObjTab[i1].typ) != 0) continue;
      // found; hide it.
      // printf("   hide typ=%d ind=%ld\n",GA_ObjTab[i2].typ,GA_ObjTab[i2].ind);
      GR_ObjTab[i1].disp  = OFF;
      GR_ObjTab[i1].hili  = OFF;
      break;
    }
  }
  GA_stat = 0;
  return 0;



/*
  //==========================================================
  // mode = 2: create HIDE-Line f. DYNAMIC_AREA
  L_save:
  if(mode != 2) goto L_add;

  mem_cbuf1[0] = '\0';
  i1 = 0;

  if(GA_recNr < 1) return -1;

  strcpy(mem_cbuf1, "HIDE");

  for(gaNr=0; gaNr<GA_recNr; ++gaNr) {
    if(GA_ObjTab[gaNr].typ == Typ_Error) continue; // skip delete-record
    // Obj. mit neg. Index skippen (Teile von dittos)
    if(GA_ObjTab[gaNr].ind < 0) continue;
    // skip normal (not-hidden)
    if(GA_ObjTab[gaNr].disp < 1) continue;

    APED_oid_dbo__(cbuf, GA_ObjTab[gaNr].typ, GA_ObjTab[gaNr].ind);
    strcat(mem_cbuf1, " ");
    // printf("  add |%s|\n",cbuf);
    strcat(mem_cbuf1, cbuf);
    ++i1;
  }

  return i1;  // nr of obj's
*/


/*
  //==========================================================
  // mode = 3: add DL-Obj dli to Hide-table (ist noch nicht in GA)
  // ERSETZT durch Funktion GA_view__
  L_add:
  if(mode != 3) goto L_rem__;
  typ = GR_ObjTab[dli].typ;
  dli = GR_ObjTab[dli].ind;

  // in DispList ON
  DL_hide__ (dli, ON);

  // see GA_creRec .......
  gaNr = GA_newRec ();  // get recordNr of next free record
  if(gaNr < 0) return gaNr;

  // write defaults;
  GA_ObjTab[gaNr] = GA_DefRec;

  // printf(" save newRec %d typ=%d ind=%d\n",i2,typ,dli);
  GA_ObjTab[gaNr].typ = typ;
  GA_ObjTab[gaNr].ind = dli;

  GA_stat = 1;  // stat_changed = yes

  // write new disp  - set Hide-Flag
  GA_ObjTab[gaNr].disp = 1;  // 0=normal; 1=hidden

  return 0;




  //================================================================
  // mode = 4: remove DL-Obj dli from Hide-table. (typ -> Typ_Error)
  // ERSETZT durch Funktion GA_view__
  L_rem__:
  if(mode != 4) goto L_rev;

  // delete Hide-Flage in DispList
  DL_hide__ (dli, OFF);

  typ = GR_ObjTab[dli].typ;
  dli = GR_ObjTab[dli].ind;
  // printf("  rem__ typ=%3d ind=%6d\n",typ,dli);

  // find record in GA
  gaNr = GA_find__ (typ, dli);
  if(gaNr < 0) return 0;

  // remove Hide-Flag
  GA_ObjTab[gaNr].disp = 0;  // 0=normal; 1=hidden

  // check if record now is empty
  GA_ckEmpty (gaNr);

  return 0;
*/



  //==========================================================
  // mode = 5: set reverseMode (display all hidden objects;
  L_rev:
  if(mode != 5) goto L_rvo;
  reverse = 1;
  GA_stat = 1;
  return 0;




  //==========================================================
  // mode = 6: set reverseMode off
  L_rvo:
  if(mode != 6) goto L_info;
  reverse = 0;
  GA_stat = 1;
  return 0;




  //==========================================================
  // mode = 7: Display Info Hidden about Objects
  L_info:
  if(mode != 7) goto L_ask_8;
  TX_Print("  %d hidden objects",GA_recNr);
  return 0;




  //==========================================================
  // mode = 8: ask state; 0=normal, 1=hidden, -1=notExist
  L_ask_8:
  if(mode != 8) goto L_err;

  // reduce typ -> Basistyp (Typ_SURRU -> Typ_SUR ..)
  typ = AP_typ_2_bastyp (typ);

  gaNr = GA_find__ (typ, dli);
  if(gaNr < 0) return 0;

  return GA_ObjTab[gaNr].disp;  // 0=normal; 1=hidden




  //==========================================================
  // mode = 9: Dump HideList
  L_err:
  printf("GA_hide__ E001 %d\n",mode);


  return 0;

}


///================================================================
  int GA_getRec (ObjAtt **ga_rec, long ind) {
///================================================================
/// \code
/// get GraficAttribute for GA_ObjTab-record ind
/// see AP_GA_get
/// \endcode

  // printf("GA_getRec %d\n",ind);

  *ga_rec = &GA_ObjTab[ind];

  return 0;

}


//================================================================
  int GA_get_dbo (int *iAtt, int typ, long dbi) {
//================================================================
/// \code
/// get GraficAttribute for DB-obj
/// Output:
///  retCod    1  no GA-Record exists
///            0  OK, iAtt = GraficAttribute
/// \endcode

  int      basTyp;
  long     gaNr;
  ObjAtt   *ga1;


  basTyp = AP_typ_2_bastyp (typ);

  // find GA-rec if already exist
  gaNr = GA_find__ (basTyp, dbi);
  if(gaNr < 0) {
    *iAtt = Typ_Att_hili1;    // curves
    return 1;                 // skip, no GA-Record exists
  }


  GA_getRec (&ga1, gaNr);
  // if(typTyp == Typ_go_LCS) {
    *iAtt = ga1->iatt;


  return 0;

}


///================================================================
  int GA_ckEmpty (long ind) {
///================================================================

  GA_stat = 1;

  if((GA_ObjTab[ind].iatt    != 0) ||
     (GA_ObjTab[ind].lay     != 0) ||
     (GA_ObjTab[ind].disp    != 0))  return 0;
     // (GA_ObjTab[ind].vtra    != 0) ||
     // (GA_ObjTab[ind].vsym    != 0))  return 0;




  // record ist leer ..
  GA_ObjTab[ind].typ     = 0;
  GA_ObjTab[ind].ind     = 0;


  // cleanup
  GA_clean1 ();

  return 0;

}


///================================================================
  int GA_clean1 () {
///================================================================

  // reduce recNr, wenn dahinter geloeschte.
  L_rem_nxt:

  if(GA_recNr < 1) return 0;

  // if(GA_ObjTab[GA_recNr-1].typ == Typ_Error) {

  if(memcmp(&GA_ObjTab[GA_recNr-1], &GA_DefRec, sizeof(ObjAtt)) == 0) {
    // Record und Defaultrecord gleich; daher leer = loeschen ..
    printf(" remove GA-rec %d\n",GA_recNr-1);
    --GA_recNr;
    goto L_rem_nxt;
  }

  return 0;

}


//================================================================
  int GA_delRec (int typ, long ind) {
//================================================================
// find & delete Attribute-record

  long iga;

  iga = GA_find__ (typ, ind);

  if(iga >= 0) {
    GA_ObjTab[iga].typ = Typ_Error;
  }

  return 0;

}


//================================================================
  int GA_find_att_SU (int typ, long ind) {
//================================================================
/// \code
/// GA_find_att_SU    get attribute for surface/solid from typ,DB-ind
///
/// other method (using DispListIndex dli): iatt = DL_get_iatt (dli);
///   or: iAtt = GR_ObjTab[dli].iatt
/// \endcode

  long   gaNr, iAtt;
  ObjAtt *ga1;


  // find GA-rec from DB-typ,ind
  gaNr = GA_find__ (typ, ind);

  if(gaNr < 0) {
    iAtt = 0;  // use GL_defCol | AP_defcol

  } else {
    GA_getRec (&ga1, gaNr);
    iAtt = ga1->iatt;
  }

    printf("ex GA_find_att_SU iAtt=%ld gaNr=%ld\n",iAtt,gaNr);

  return iAtt;

}
 
 
///================================================================
  long GA_find__ (int typ, long ind) {
///================================================================
/// GA_find__         find GA-rec if already exist
/// ACHTUNG: fix typ vorher mit AP_typ_2_bastyp !

  long i1;


  // gibst diesen rec schon ?
  for(i1=0; i1<GA_recNr; ++i1) {
    if(GA_ObjTab[i1].typ != typ) continue;
    if(GA_ObjTab[i1].ind == ind) return i1;  // fertig (schon vorh.)
  }

  return -1L;

}


///================================================================
  long GA_newRec () {
///================================================================
/// GA_newRec         get recordNr of next free record


  long i1;

  // printf("GA_newRec \n");


  // find next deleted GA_recNr
  for(i1=0; i1<GA_recNr; ++i1) {
    if(GA_ObjTab[i1].typ == Typ_Error) goto L_done;
  }

  // neu anlegen
  i1 = GA_addRec ();


  L_done:
  // write defaults;
  GA_ObjTab[i1] = GA_DefRec;

  // printf("ex GA_newRec %d\n",i1);
  return i1;

}


///================================================================
  long GA_creRec (int typ, long ind) {
///================================================================

  static int itest1=0;

  long gaNr;


  ++itest1;

  // printf("GA_creRec %d %ld %d\n",typ,ind,itest1);


  // reduce typ -> Basistyp (Typ_SURRU -> Typ_SUR ..)
  typ = AP_typ_2_bastyp (typ);
    // printf(" Basistyp=%d\n",typ);


  // find GA-rec if already exist
  gaNr = GA_find__ (typ, ind);

  if(gaNr < 0) {
    gaNr = GA_newRec ();  // get recordNr of next free record
    if(gaNr < 0) return gaNr;
  }

  // printf(" gaNr=%d\n",gaNr);

  // write typ, ind
  GA_ObjTab[gaNr].typ = typ;
  GA_ObjTab[gaNr].ind = ind;

  GA_stat = 1;  // stat_changed = yes

  // printf("ex GA_creRec %d\n",gaNr);
  return gaNr;

}


//================================================================
  int GA_Col__ (long dli, int mode, int typ, long ind) {
//================================================================
/// \code
/// GA_Col__            modify color
/// mode = 0:  modify Color (apply AP_actcol)
///        1:  reset Color (set color not active)
///        2:  select_as_new_color
/// set AP_actcol with APcol_actCol__()
/// \endcode


  long       gaNr;


  // printf("GA_Col__ mode=%d dli=%ld typ=%d ind=%ld\n",mode,dli,typ,ind);

  // typ kann Typ_SURBSP sein  !!
  typ = AP_typ_2_bastyp (typ);

  // check typ; muss SUR od SOL od Model sein ...
  if((typ == Typ_SUR)||(typ ==Typ_SOL)) goto L_1;

  if(dli >= 0) TX_Print("sel surf or body ..");
  return 0;



  // find or create GA-record
  L_1:
  gaNr = GA_creRec (typ, ind);
  if(gaNr < 0) return -1;
    // printf(" gaNr=%d\n",gaNr);

  
  //================================================================
  if(mode == 2) {
    // get color from obj as new active color AP_actcol
    APcol_actColTra ((ColRGB*)&GA_ObjTab[gaNr].iatt);
    return 0;
  }


  //================================================================
  // write new col > PermanentAttributeTable GA_ObjTab
  if(mode == 0) {
    GA_ObjTab[gaNr].iatt = *((long*)&AP_actcol);
    ((ColRGB*)&GA_ObjTab[gaNr].iatt)->vtex  = 0;
    ((ColRGB*)&GA_ObjTab[gaNr].iatt)->color = 1;   // color active ..


  } else {
    ((ColRGB*)&GA_ObjTab[gaNr].iatt)->color = 0;   // color not active ..
  }

  if(dli < 0) return 0;  // preload = entry vor Draw


  // save the index in the DL-Record
  if(mode == 0) {
    GR_ObjTab[dli].iatt = *((long*)&AP_actcol);
    ((ColRGB*)&GR_ObjTab[dli].iatt)->vtex  = 0;
    ((ColRGB*)&GA_ObjTab[dli].iatt)->color = 1;   // color active ..

  } else {
    ((ColRGB*)&GR_ObjTab[dli].iatt)->color = 0;   // color not active ..
  }


  // redraw single obj, use existing DL-record
  DL_Draw_obj (dli, typ, ind);

  DL_hili_off (-1L);             //  -1 = unhilite all

  DL_Redraw ();


  return 0;

}

/*
//================================================================
  int GA_Tex_reset () {
//================================================================
 
  int    i1, typ;
  ColRGB *col;


  for(i1=0; i1<GA_recNr; ++i1) {

    typ = GA_ObjTab[i1].typ;

    if(typ == Typ_SUR)  {
      // (typ == Typ_SOL)  ||
      // (typ == Typ_Model))   {
      col = (ColRGB*)&GA_ObjTab[i1].iatt;
      // printf(" cr=%d cg=%d cb=%d col=%d tra=%d sym=%d tex=%d\n",
        // col->cr,col->cg,col->cb,col->color,col->vtra,col->vsym,col->vtex);
      if(col->vtex == 1) ((stru_c3c1*)&GA_ObjTab[i1].iatt)->b123 = 0;

    }
  }

  return 0;

}
*/


//================================================================
  int GA_Tex__ (long dli, int mode, int typ, long ind) {
//================================================================
/// \code
/// mode 0 = remove texture
/// mode 1 = apply texture, redraw.
/// mode 2 = load texture
/// mode 3 = update texture
/// mode 5 = apply texture, but do not (yet) redraw.
/// \endcode


  static long gaNr = -1;

  int        irc;
  int        iTexRef;
  TexRef     *rtex;


  printf("GA_Tex__ mode=%d dli=%ld typ=%d ind=%ld\n",mode,dli,typ,ind);

  // typ kann Typ_SURBSP sein  !!
  typ = AP_typ_2_bastyp (typ);

  // check typ; muss SUR od SOL od Model sein ...
  if(typ == Typ_SUR) goto L_1;

  if(dli >= 0) TX_Print("sel surf ..");
  return 0;



  //----------mode 0 = remove texture-------------------------------
  L_1:
  if(mode == 0) {          // remove
    gaNr = GA_find__ (typ, ind);
    if(gaNr < 0) {TX_Print(" surf does not have texture .."); return -1;}
    goto L_2;
  }


  //----------mode 2 = load texture from surf-----------------------
  if(mode == 2) {          // load texture
    // find GA-rec if already exist
    gaNr = GA_find__ (typ, ind);
    if((gaNr < 0) ||
       ((ColRGB*)&GA_ObjTab[gaNr].iatt)->vtex == 0) {   // no texture yet ..
      TX_Print(" surf does not have texture ..");
      return -1;
    }


    // die textureParameter aus der surf extrahieren
    iTexRef = ((stru_c3c1*)&GA_ObjTab[gaNr].iatt)->b123;  // GA_tex_ga2tr
    Tex_setRefInd (iTexRef);  // set TexRef-Index

    // load TexBas from TexRef
    Tex_getRef (&rtex, iTexRef);
    Tex_actBas_set (rtex->ibas);

    // // display new textureInfo
    // UI_Tex__ (NULL, (void*)UI_FuncUCB9);
    // TX_Print("key Return in inputFields = update surface");

    // // save & display surf-DB-Nr 
    // Tex_setSurfInd (ind, dli);
    // UI_Tex__ (NULL, (void*)UI_FuncUCB10);

    // // display userParameters of this TexRef
    // // UI_Tex__ (NULL, (void*)UI_FuncUCB7);
    // UI_Tex_EntUpd (-1, rtex);

    return 0;
  }


  //-----------mode 1 = apply texture-------------------------------
  // exit if no texture is active ..
  irc = Tex_actBas_get ();
  if(irc < 0) {
    TX_Print("**** no texture is active ******");
    return 0;
  }


  // save sur-DB-ind & dli
  Tex_setSurfInd (ind, dli);

  // find or create GA-record > PermanentAttributeTable GA_ObjTab
  gaNr = GA_creRec (typ, ind);
  if(gaNr < 0) return -1;
    // printf(" gaNr=%ld\n",gaNr);
    // GA_dump_1 (gaNr);

    // ((ColRGB*)&GA_ObjTab[gaNr].iatt)->vtex = 1;   // texure applied

  goto L_work;





  //-----------mode 3 = update texture------------------------------
  // exit if no texture is active ..
  // irc = Tex_actBas_get ();
  // if(irc < 0) {
    // TX_Print("**** no texture is active ******");
    // return 0;
  // }
  // goto L_work;





  //----------------------------------------------------------------
  // does a texture exist ?
  L_work:
  if(((ColRGB*)&GA_ObjTab[gaNr].iatt)->vtex == 0) {   // no texture yet ..
    // search if TexRef-Record exists else create new
    // loop durch GA; wenn vtex==1 und typ und ind gleich sind ..
    // if(GA_ObjTab[gaNr].iatt == 0) {
      printf(" neuer GA-Record\n");
      // create new TexRef-Record
      iTexRef = Tex_addRef ();
      Tex_TexRef_InitU (iTexRef);
      Tex_TexRef_InitS (iTexRef);
    // }

  } else {    // get iTexRef from (cr+cg+cb)
    iTexRef = ((stru_c3c1*)&GA_ObjTab[gaNr].iatt)->b123;
  }
    // printf(" iTexRef=%d\n",iTexRef);


  // save the active TexRef-index
  irc = Tex_setRefInd (iTexRef);
  if(irc < 0) return irc;


  // check if TexBas did change; if yes: reset userData
  Tex_RefTest ();


  // write new tex > PermanentAttributeTable GA_ObjTab
  L_2:
  if(mode != 0) {
    ((stru_c3c1*)&GA_ObjTab[gaNr].iatt)->b123 = iTexRef;
    ((ColRGB*)&GA_ObjTab[gaNr].iatt)->vtex  = 1;   // texure applied
    ((ColRGB*)&GA_ObjTab[gaNr].iatt)->vtra  = 0;   // remove transp.
    ((ColRGB*)&GA_ObjTab[gaNr].iatt)->color = 0;   // remove color
  } else {
    // GA_ObjTab[gaNr].iatt = *((long*)&AP_defcol);
    GA_ObjTab[gaNr].iatt = 0;
    // ((ColRGB*)&GA_ObjTab[gaNr].iatt)->vtex = 0;   // no texture
  }

  if(dli < 0) return 0;  // preload = entry vor Draw


  // save the index in the DL-Record
  if(mode != 0) {    // apply
    ((stru_c3c1*)&GR_ObjTab[dli].iatt)->b123 = iTexRef;
    ((ColRGB*)&GR_ObjTab[dli].iatt)->vtex  = 1;   // texure applied
    ((ColRGB*)&GR_ObjTab[dli].iatt)->vtra  = 0;   // remove transp.
    ((ColRGB*)&GR_ObjTab[dli].iatt)->color = 0;   // remove color
    // TX_Print("texture applied ..");

  } else {           // remove
    // GR_ObjTab[dli].iatt = *((long*)&AP_defcol);
    GR_ObjTab[dli].iatt = 0;
    ((ColRGB*)&GR_ObjTab[dli].iatt)->vtex = 0;   // no texture
    TX_Print("texture removed ..");
  }


  if(mode < 4) {
    // modify (do not create new DL-Record)
    DL_Draw_obj (dli, typ, ind);
    // DL_Redraw ();
  }


  return 0;

}


//================================================================
  int GA_tex_ga2tr (long iga) {
//================================================================
// GA_tex_ga2tr         get TexRef-nr from GA-record-nr

// see also GA_getTexRefInd

  // GA_ObjTab[].iatt of sur/sol is a ColRGB
  // first 3 chars of ColRGB (cr, cg, cb) contain TexRef-nr as 24-bit-int
  // first 24 bits of int -> TexRef-nr
  return ((stru_c3c1*)&GA_ObjTab[iga].iatt)->b123;

}


//================================================================
  int GA_Tra__ (long dli, int mode, int typ, long ind) {
//================================================================
/// \code
/// GA_Tra__            modify transparenz
/// mode: 0 = reset transpar.
///       1 = apply 50 % transpar.
///       2 = apply 100 % transpar.
/// \endcode

  long       gaNr;

  printf("GA_Tra__ %d %ld %d %ld\n",mode,dli,typ,ind);


  // typ kann Typ_SURBSP sein  !!
  typ = AP_typ_2_bastyp (typ);

  // check typ; muss SUR od SOL sein ...
  if((typ == Typ_SUR) ||
     (typ == Typ_SOL))   goto L_1;

  if(dli >= 0) TX_Print("sel surf ..");
  return 0;


  // find or create GA-record
  L_1:
  gaNr = GA_creRec (typ, ind);
  if(gaNr < 0) return -1;
    // printf(" gaNr=%d\n",gaNr);


  // wenn die Color die defCol ist - die color kopieren !
  // die Transparenz wird in eigenem GL-Redraw-Durchlauf generiert,
  // und da stimmt die DefCol nicht (wenn zB Flaeche in einem subModel ist)
  // > PermanentAttributeTable GA_ObjTab
  if(mode != 0) {  // nur apply
    // nur wenn dzt keine Spezialfarbe aktiv
    if(((ColRGB*)&GA_ObjTab[gaNr].iatt)->color == 0) {
      // UT3D_stru_dump (Typ_Color, &AP_defcol, "Def-Color:  ");
      GA_ObjTab[gaNr].iatt = *((long*)&AP_defcol);  // overwrites tra 
      ((ColRGB*)&GA_ObjTab[gaNr].iatt)->color = 1;

      if(dli >= 0) {
        GR_ObjTab[dli].iatt = *((long*)&AP_defcol);  // overwrites tra 
        ((ColRGB*)&GR_ObjTab[dli].iatt)->color = 1;
      }
    }
  }


  // write new transparency
  if(mode == 0) {
    ((ColRGB*)&GA_ObjTab[gaNr].iatt)->vtra = 0;   // transp not active ..
  } else if(mode == 1) {
    ((ColRGB*)&GA_ObjTab[gaNr].iatt)->vtra = 1;   // transp 50 %
  } else {
    ((ColRGB*)&GA_ObjTab[gaNr].iatt)->vtra = 2;   // transp 100 %
  }


    GA_dump_1 (gaNr, stdout);    // 2013-08-15


  if(dli < 0) return 0;  // preload = entry vor Draw


  // modify (do not create new DL-Record)
  DL_SetInd (dli);

  // save the index in the DL-Record
  if(mode == 0) {
    ((ColRGB*)&GR_ObjTab[dli].iatt)->vtra = 0;
  } else if(mode == 1) {
    ((ColRGB*)&GR_ObjTab[dli].iatt)->vtra = 1;
  } else {
    ((ColRGB*)&GR_ObjTab[dli].iatt)->vtra = 2;
  }


  // die GL-DrawFunction mit neuem attInd rufen ..
  APT_Draw__ (GR_ObjTab[dli].iatt, typ, ind);

  DL_hili_off (-1L);             //  -1 = unhilite all

  DL_Redraw ();


  return 0;

}


//===================================================================
  int GA_lTyp__ (long dli, int lTyp, int typ, long ind, int iRedraw) {
//===================================================================
/// \code
/// GA_lTyp__            modify lineType
/// Input:
///   lTyp       index into attributeTable; 0-50.
///   iRedraw    0=do_redraw; 1=not.
/// \endcode


// Fuer ein Modify Curve-Linetyp muss man
//  1) den GL-Attribut-Record aendern (GL_InitNewAtt) und
//  2) die GL-DrawFunction fuer das obj rufen; die attInd ist nun anders.


  int        btyp;
  long       gaNr;
  Ind_Att_ln iAtt;


  printf("GA_lTyp__ lTyp=%d Atyp=%d Aind=%ld dli=%ld\n",lTyp,typ,ind,dli);

  btyp = AP_typ_2_bastyp (typ);

  // check typ; muss LN/AC/Curve sein ..
  if((btyp == Typ_LN)   ||
     (btyp == Typ_CI)   ||
     (btyp == Typ_CV)) goto L_1;

  if(dli >= 0) TX_Print("sel LN or Circ or Curve ..");
  return 0;


  // find or create GA-record
  L_1:
  gaNr = GA_creRec (btyp, ind);
  if(gaNr < 0) return -1;
    printf(" gaNr=%ld\n",gaNr);

  // write new ltyp > PermanentAttributeTable GA_ObjTab
  GA_ObjTab[gaNr].iatt = lTyp;

  if(dli < 0) return 0;  // preload = entry vor Draw


  // save the new linetype in the DL-Record
  IndAttLn_Set1 (&GR_ObjTab[dli].iatt, lTyp, -1);


  // modify (do not create new DL-Record)
  DL_SetInd (dli);

  // redraw with new linetype
  APT_Draw__ (GR_ObjTab[dli].iatt, btyp, ind);

  if(!iRedraw) DL_Redraw ();

  return 0;

}


///================================================================
  int GA_sStyl__ (long dli, int sStyl, int typ, long dbi) {
///================================================================
/// GA_sStyl__           modify surfStyle
/// 
/// Input:
///   dli     DL-Ind;
///           if(dli==-1) GA-Record anlegen od. modifizieren
///           if(dli>=0): modify obj;
///   sStyl   1=shaded, 2=symbolic, 3=transparent.


  long       gaNr;
  ColRGB     *col;


  // printf("GA_sStyl__ dli=%ld sStyl=%d typ=%d dbi=%ld\n",dli,sStyl,typ,dbi);


  // typ kann Typ_SURBSP sein  !!
  typ = AP_typ_2_bastyp (typ);
    // printf(" typ=%d\n",typ);


  // check typ; muss SUR od SOL od Model sein ...
  if((typ == Typ_SUR)   ||
     (typ == Typ_SOL)) goto L_1;

  if(dli >= 0) TX_Print("sel surf or solid ..");
  return 0;



  // find or create GA-record > PermanentAttributeTable GA_ObjTab
  L_1:
  gaNr = GA_creRec (typ, dbi);
  if(gaNr < 0) return -1;
    // printf(" gaNr=%d\n",gaNr);


  col = COL_INT32(&GA_ObjTab[gaNr].iatt); // col = (ColRGB*)&GA_ObjTab[gaNr].iatt;
    // UT3D_stru_dump (Typ_Color, col, " col ex ga ");

  if(sStyl == 1) {               // reset style to 1=normal
    col->vsym = 0;

  } else if(sStyl == 2) {        // set style to 2=symbolic
    col->vsym = 1;

  } else if(sStyl == 3) {        // set style to 3=transparent
    col->vsym = 0;
  }




  if(dli < 0) return 0;  // preload = entry vor Draw



  // modify (do not create new DL-Record)
  DL_SetInd (dli);

  // save the index in the DL-Record
  GR_ObjTab[dli].iatt = GA_ObjTab[gaNr].iatt;


  // die GL-DrawFunction mit neuem attInd rufen ..
  APT_Draw__ (GA_ObjTab[gaNr].iatt, typ, dbi);

  DL_hili_off (dli);             // unhilite


  DL_Redraw ();

  // printf("ex GA_sStyl__ %d %d %d\n",sStyl,typ,ind);

  return 0;

}


///================================================================
  int GA_dump_tex (FILE *fpo) {
///================================================================
/// alle GA-records mit textures raus ..

// vi -t ObjAtt

  int    i1, it, typ;
  char   cbuf[32];


  for(i1=0; i1<GA_recNr; ++i1) {

    if(((ColRGB*)&GA_ObjTab[i1].iatt)->vtex != 0) {
      typ = GA_ObjTab[i1].typ;
      it = ((stru_c3c1*)&GA_ObjTab[i1].iatt)->b123;
      APED_oid_dbo__ (cbuf, typ, GA_ObjTab[i1].ind);
      if(!fpo) fpo = stdout;
      fprintf(fpo, " GA[%d] %s tr = %d\n",i1,cbuf,it);
    }
  }

  return 0;

}

 
///================================================================
  int GA_dump__ (FILE *fpo) {
///================================================================
/// write into open file; do not close ..

  long   i1;

  printf(" GA_dump__\n");

  if(!fpo) fpo = stdout;

  fprintf(fpo, "============= PermanentAttributeList recNr=%d siz=%ld\n",
          GA_recNr,GA_SIZ);

  for(i1=0; i1<GA_recNr; ++i1) {
    GA_dump_1 (i1, fpo);
  }

  return 0;

}


///================================================================
  int GA_dump_1 (long i1, FILE *fpo) {
///================================================================
 
  int    typ;
  ColRGB *col;



    typ = GA_ObjTab[i1].typ;

    fprintf(fpo,"  %ld typ=%3d ind=%6ld disp=%d",i1, typ,GA_ObjTab[i1].ind,
      GA_ObjTab[i1].disp);

    if((typ == Typ_SUR)  ||                   // surfs
       (typ == Typ_SOL)  ||                   // surfs
       (typ == Typ_Model))   {                // surfs
      col = COL_INT32(&GA_ObjTab[i1].iatt); // col = (ColRGB*)&GA_ObjTab[i1].iatt;
      fprintf(fpo," cr=%d cg=%d cb=%d col=%d tra=%d sym=%d tex=%d\n",
        col->cr,col->cg,col->cb,col->color,col->vtra,col->vsym,col->vtex);

    } else if(typ == Typ_Tag) {          // Tag,Images ...
    // } else if(typ == Typ_GTXT) {          // Tag,Images ...

    } else {                              // curves
      fprintf(fpo," attInd=%d\n",GA_ObjTab[i1].iatt);
    }



  return 0;

}


//========================= EOF =================================
