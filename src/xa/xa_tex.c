// Texture-Utility-Functions Tex_               2006-07-10   RF
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
2006-07-10 Neu. RF.

-----------------------------------------------------
*/
/*!
\file  ../xa/xa_tex.c
\brief Texture-Utility-Functions Tex_ 
\code
=====================================================
List_functions_start:

Tex_getBitmap    provide Bitmap in <tmpDir>; return filename of BitmapFile.

Tex_Init__
Tex_act_upd      GA_Tex__ (3)
Tex_setSurfInd   save & display surf-DBi & sur-dli merken

Tex_addBas__     load Texture from filename + dirname
Tex_addBas1      load Texture from symNam
Tex_getBas__     get BasicTextureNr for Image <symNam>; else create new TexBas
Tex_getBas1      get textureNr for File; else create new BasicTexture
Tex_actBas_get   get TexBasAct (index of active TexBas)
Tex_actBas_set   set TexBas-index TexBasAct

Tex_addRef       search if TexRef-Record exists else create new
Tex_setRefInd    save the active TexRef-index
Tex_getRefInd    returns TexRefAct
Tex_savRef       overwrite active TexRef-Record
Tex_getRef       get TexRef-record from TexRef-index
Tex_TexRef_UpdU  update the userparameters of the active texture
Tex_TexRef_InitU init the userparameters of the active texture
Tex_TexRef_InitS SurfData auf defaults setzen
Tex_get_fn       get textureFilename from TexRefNr
Tex_itb_itr      get TexBas-Nr from TexRef-Nr
Tex_tbNr         get nr TexBas-records

TexBas_realloc
TexRef_realloc
Tex_DelAll       delete all OpenGL-textures

Tex_dump__       dump TexBasTab and TexRefTab

List_functions_end:
=====================================================
Tex_loadBas      UNUSED
see also:
GA_Tex__
GL_Tex_Ini GL_Tex_End
GRU_tex_pos*
UI_Tex__

\endcode *//*----------------------------------------



ABLAUF:

GL_Tex_Load     Laden einer BMP-Datei in einen OpenGL-TexturBuffer.

APT_Draw__ > GL_DrawSur > GRU_tex_pos1
  Wenn vx und vy einer TexRef-struct NULL sind = noch nicht initialisiert:
  Normalvektor auf die Fläche bestimmen, Position und Scale für Textur berechnen.

GL_Tex_Ini      Einen OpenGL-TexturBuffer laden; Position und Scale setzen.
  Position und Scale werden aus TexRef-struct errechnet.

../xa/ui_tex.c
../xa/xa_tex.c ?

Testprogramme:
../xa/tst_gl2.c


-----------------------------------------------------
Betroffene Funktionen:

UI_WinSurfTex__
 UI_Tex_Load
  Tex_addBas
UI_GR_Select_work1
 GA_Tex__        fix DL
  Tex_addRef
  APT_Draw__
   APT_DrawSur
    GR_DrawSur
     TSU_DrawSurT_
      TSU_DrawSurTP
       GL_DrawSur
        GRU_tex_pos1
        GL_Tex_Ini
GA_fil_wri
GA_load__


---------------------------------------------------
Es gibt eine permanente Tabelle TexBasTab mit den Filenames der Texturen;
weiters eine TexRef-Tabelle TexRefTab mit dem Index auf die TexBas, Scale usw.
In der DL liegt der Index auf die TexRefTab.

Textures temporaer speichern (in der DL und in der GA):
  im iatt liegt ColRGB; dessen .vtex=1 (else 0);
  cr+cg+cb sind nun ein int, das die TexRefNummer beinhaltet.
  der TexRef-Record wird in ut_tex.c in TexRefTab gespeichert.

Textures permanent speichern (im Block DYNAMIC_DATA):
 in der GATAB; 
 X"Filename",sx,sy,dx,dy,ra,vx,vz


Der Vektor, mit dem die Textur auf die Flaeche gemappt wird, ist normal zu
  vx,vy im TexRef-Record und wird immer erst vor dem GL_Tex_Ini errechnet.
  GRU_tex_pos* ?



--------------------------
TexBasTab:   *TexBas
  fnam       symbolic Filename; pointer.
  dli        DispListindex of the Texture
  texNr      (OpenGL-) TextureNr
  ..

--------------------------
TexRefTab:   *TexRef
  ibas         index of the Base-Texure
  ..

--------------------------
Texture in der OpenGL:
  Texture erzeugen:      see GL_LoadTex
    eine Bitmap zuerst ins memory laden (bmp_load);
    eine TexturNummer besetzen (glBindTexture - glDeleteTextures/glGenTextures)
    vom memory mit glTexImage2D in eine glNewList; deren Nr -> dlTex
  Usage:   see GL_Tex_Ini
    mit glCallList(dlTex);  dlTex=DispListIndex of glTexImage2D
    die TexturPosition setzen mit glTexGendv (vs,vt).  ????



--------------------------
  Get index of active texture (in TexRefTab): Tex_getRefInd
  Get active TexRef: Tex_getRef
  Redraw: DL_Draw_obj

see also ../xa/xa_ga.c


---------------------------------------------------
*/

#ifdef _MSC_VER
#include "MS_Def0.h"
#endif

#include <string.h>                       /* strcmp ..   */
#include <math.h>                         /* fabs  ...   */
#include <stdio.h>                        /*  FILE .. */
#include <stdlib.h>                       // realloc


#include "../ut/ut_geo.h"        
#include "../ut/ut_txt.h"                // fnam_del
#include "../ut/ut_txTab.h"              // TxtTab
#include "../ut/ut_os.h"                 // OS_ ..
#include "../ut/func_types.h"                 // UI_FuncUCB8
#include "../ut/ut_memTab.h"           // MemTab

#include "../xa/xa_mem.h"                // memspc011
#include "../xa/xa.h"                      // AP_STAT



//===============================================================
// Externe Variablen:
// xa.c
extern AP_STAT   AP_stat;



//===============================================================
// Interne Variablen:

static int    TexBasNr  = 0;
static int    TexBasSiz = 0;

/// StringList TextureFilenames
       UtxTab_NEW (TexTbFn);
       // void   *TexTbFn = NULL;

#define TexBasINC  32

       int    TexRefNr  = 0;
static int    TexRefSiz = 0;
static int    TexRefAct = 0;
       TexRef *TexRefTab = NULL;
static long   actSur, actDli;

       int    TexBasAct = -1;      // TexBasTab-index of the active TexBasRecord
       TexBas *TexBasTab = NULL;
static TexBas *actTexBas;


#define TexRefINC  32



//==== prototypes: ===============================================


//================================================================
   int Tex_tbNr () {
//================================================================
// Tex_tbNr         get nr TexBas-records

  return TexBasNr;

}


//================================================================
   char* Tex_get_fn (int itr) {
//================================================================
// Tex_get_fn       get textureFilename from TexRefNr

  int     itb;
  char   *p1;
  TexRef *tr;
  TexBas *tb;


  Tex_getRef (&tr, itr);
  // get TexBas-Record
  itb = tr->ibas;
  Tex_getBas__ (&tb, itb);

  return tb->fnam;

}

//================================================================
  int Tex_itb_itr (int itr) {
//================================================================
// Tex_itb_itr      get TexBas-Nr from TexRef-Nr

  TexRef *tr;

  Tex_getRef (&tr, itr);

  return tr->ibas;

}


//================================================================
  int Tex_getBas1 (TexBas **tbAct, char *symNam) {
//================================================================
/// \code
/// get BasicTextureNr for Image <symNam>; else create new BasicTexture
/// get textureNr of File; else create new BasicTexture
/// search ImageName in BasicTextureTable;
/// sets TexBasAct = index of tbAct
///  Input:
///    symbolicFilename
///  Output:
///    tbAct     Pointer into TexBasTab
///    RetCode:  -1=Error. 0=new; 1=exists.
/// \endcode


// see also Tex_getBas1  (macht zusaetzlich LoadTexture)

  int    irc, iNr, i1;
  long   dli;
  char   *p1;



  // printf("Tex_getBas1 |%s|\n",symNam);
  // printf("  TexBasNr=%d\n",TexBasNr);


  if(TexBasNr >= TexBasSiz)
    if(TexBas_realloc() < 0) return -1;


  for(i1=0; i1<TexBasNr; ++i1) {
      // printf(" tbt[%d]=|%s|\n",i1,TexBasTab[i1].fnam);
    if(strcmp(TexBasTab[i1].fnam,symNam)) continue;
    // already exists;
    *tbAct = &TexBasTab[i1];
    TexBasAct = i1;
    irc = 1;
    goto L_done;
  }


  // eigentlich sollte iNr (Recordnr in UtxTab) gleich sein wie TexBasNr



  //----------------------------------------------------------------
  // neuen TexBas-Record anlegen ..
  // save fnam; muss aber permanent sein (nicht loeschen mit RUN)
  UtxTab_add (&TexTbFn, symNam);

  // query pos of fnam
  iNr = UtxTab_query (&p1, &TexTbFn);
    // printf(" p1=|%s|\n",p1);


  // fix iTex
  // keepFlag: 0=do not keep active texture; overwrite active.
  //           1=keep.
  // if(keepFlag == 1) {
  // hier sollte eine unused TexBasNr gesucht werden !
    TexBasAct = TexBasNr;
    ++TexBasNr; // keep: do not overWrite last texture
  // }
    // printf(" create new basic texture:\n");
    // printf(" TexBasAct=%d TexBasNr=%d\n",TexBasAct,TexBasNr);


  // init TexBasRecord
  TexBasTab[TexBasAct].fnam = p1;
    // printf(" TexBasTab %d |%s|\n",TexBasAct,TexBasTab[TexBasAct].fnam);

  // init
  // TexBasTab[TexBasAct].dli = -1;                 UNUSED
  TexBasTab[TexBasAct].texNr = TexBasAct + 1;
  TexBasTab[TexBasAct].keep = -1;

  *tbAct = &TexBasTab[TexBasAct];
  irc = 0;   // new


  L_done:
  // // act. TexRef: defaults  laden ..
  // Tex_TexRef_InitU ();


    // printf("ex ut_tex_getBas %d %d\n",irc,TexBasAct);
  return irc;

}
 

//================================================================
  int Tex_DelAll () {
//================================================================
/// GL_Tex_DelAll       delete all OpenGL-textures

  int     i1;

  // printf("Tex_DelAll %d\n",TexBasNr);

  for(i1=0; i1<TexBasNr; ++i1) GL_Tex_Del (i1);

  return 0;

}


//================================================================
  int Tex_act_upd () {
//================================================================
/// DoUpdate

  // Point  po;

  // printf("Tex_act_upd %d\n",actSur);


  if(actSur < 1) return -1;

  GA_Tex__ (actDli, 3, Typ_SUR, actSur);  // calls APT_Draw__

  DL_hili_off (actDli);  // unhilite

  // po.x = TexRefTab[TexRefAct].px;
  // po.y = TexRefTab[TexRefAct].py;
  // po.z = TexRefTab[TexRefAct].pz;
  // GR_Disp_rect1 (&po, &TexRefTab[TexRefAct].vx, 10.,
                      // &TexRefTab[TexRefAct].vy, 3., 9);

  DL_Redraw ();

  return 0;

}

 
//================================================================
  int Tex_setSurfInd (long sur, long  dli) {
//================================================================
/// save & display surf-DBi & sur-dli merken

  // printf("Tex_setSurfInd  sur=%d dli=%d\n",sur,dli);

  if(actSur != sur) {
    actSur = sur;
    // // updatedisplay of act.surf
    // UI_Tex__ (NULL, (void*)UI_FuncUCB10);
  }
  actDli = dli;

  return 0;
}


//================================================================
  int Tex_TexRef_UpdU (TexRef *rtex) {
//================================================================
/// update the userparameters of the active texture
// activeTexture is TexRefAct; set it with ??

  // DEB_dump_obj__ (Typ_TEXR, rtex, "Tex_TexRef_UpdU");

  if(TexRefAct >= TexRefNr) return -1;         // skips 0,0

  TexRefTab[TexRefAct].uscx = rtex->uscx;
  TexRefTab[TexRefAct].uscy = rtex->uscy;
  TexRefTab[TexRefAct].udx = rtex->udx;
  TexRefTab[TexRefAct].udy = rtex->udy;
  TexRefTab[TexRefAct].uar = rtex->uar;

  return 0;
}


//================================================================
  int Tex_TexRef_InitU (int ii) {
//================================================================
/// UserData auf defaults setzen

  TexRefTab[ii].ibas = TexBasAct;
  TexRefTab[ii].uscx = 1.;
  TexRefTab[ii].uscy = 1.;
  TexRefTab[ii].udx  = 0.;
  TexRefTab[ii].udy  = 0.;
  TexRefTab[ii].uar  = 0.;

  return 0;
}


//================================================================
  int Tex_TexRef_InitS (int ii) {
//================================================================
/// SurfData auf defaults setzen

  // TexRefTab[ii].ssx = 1.;
  // TexRefTab[ii].ssy = 1.;
  // TexRefTab[ii].px  = 0.;
  // TexRefTab[ii].py  = 0.;
  TexRefTab[ii].vx  = UT3D_VECTOR_NUL;
  TexRefTab[ii].vy  = UT3D_VECTOR_NUL;

  return 0;
}


//================================================================
  int Tex_actSur_get () {
//================================================================
// get DB-Ind of act. surf

  return actSur;
}


//================================================================
  long Tex_actDli_get () {
//================================================================
/// get DB-Ind of act. surf

  return actDli;
}




//================================================================
  int Tex_actBas_set (int ii) {
//================================================================
/// Tex_actBas_set      set TexBas-index TexBasAct

  // printf("Tex_actBas_set %d\n",ii);

  TexBasAct = ii;

  return 0;
}


//================================================================
  int Tex_actBas_get () {
//================================================================
/// Tex_actBas_get   get TexBasAct (index of active TexBas)

  // printf("Tex_actBas_get %d\n",TexBasAct);

  return TexBasAct;
}



//================================================================
  int Tex_getBitmap (char *bNam, char *symNam, int overWrite) {
//================================================================
/// \code
/// provide Bitmap in <tmpDir>; return filename of BitmapFile.
/// test if Bitmap already exists; if not: create it.
/// copy BMP; convert JPG.
///
/// Input:
///   symNam  symbolic-name
///   overWrite    0=do nothing if destinationfile exists;
///                1 overWrite
/// Output:
///   bNam    Bitmapfilename
///   RetCod: 0=Ok; file created;
///           1=Ok; file did exist;
///          -1=Error; cannot create BitmapFile.
///          -2=Error; unsupported Filetyp.
/// \endcode

// see Tex_getBitmap

  int    irc;
  char   fNam[256], safNam[128], fTyp[16], cbuf[512];


  // printf("Tex_getBitmap |%s|\n",symNam);


  if(!symNam) return -3;

  // create safe-name fron symbolic-name
  strcpy(safNam, symNam);
  UTX_safeName (safNam, 1);
    // printf(" safNam |%s|\n",safNam);


  // create Bitmapfilename from safe-name
  sprintf(bNam, "%s%s.bmp", OS_get_tmp_dir(), safNam);
    // printf(" getBitmap-bNam |%s|\n",bNam);


  // check if TextureFile (bmp) existes;
  if(OS_checkFilExist(bNam, 1) == 1) {
    if(overWrite < 1) return 1;
  }


  // get real filename from symbolic filename -> fNam
  // irc = Mod_get_path (fNam, symNam);
  irc = Mod_fNam_sym (fNam, symNam);
  if(irc < 0) return -1;
    // printf(" ex-fNam_sym- |%s|%s|\n",fNam,symNam);


  // check if sourcefiles exists
  if(OS_checkFilExist(fNam, 1) == 0) return -1;


  // get filetyp of symNam
  UTX_ftyp_s (fTyp, symNam, 1);
    // printf(" fTyp = |%s|\n",fTyp);


  //----------------------------------------------------------------
  // copy BMP
  if(!strcmp(fTyp, "BMP")) {
      // printf(" copy bitmap\n");
    OS_file_copy (fNam, bNam);  //  printf(" rc OS_file_copy=%d\n",irc);
    goto L_done;
  }


  //----------------------------------------------------------------
  // convert JPG -> BMP
  if((strcmp(fTyp, "JPG"))&&(strcmp(fTyp, "JPEG"))) {
    TX_Print("**** cannot use filetype %s for texture *****",fTyp);
    return -2;
  }


  // check if file <fNam> exists ..
  if(OS_FilSiz(fNam) < 8) {              // 2016-03-21
    Mod_sym_get__ (&cbuf[0], &cbuf[100], &cbuf[300], symNam);
    TX_Print("**** ERROR; no file \"%s\" in directory \"%s\" - symbolic \"%s\"",
                             &cbuf[300], &cbuf[100], &cbuf[0]);
    return -1;
  }


  // djpeg -bmp fnIn.jpg > fnOut.bmp
  sprintf(cbuf, "%s -bmp \"%s\" > \"%s\"",OS_get_imgConv1(),fNam,bNam);
    // printf(" |%s|\n",cbuf);
  OS_system(cbuf);


  //----------------------------------------------------------------
  L_done:

  // check if TextureFile (bmp) existes;
  if(OS_checkFilExist(bNam, 1) == 1) {
    if(OS_FilSiz(bNam) > 8) return 0;      // 2016-03-21
  }


  return -1;

}


//================================================================
  int Tex_Init__ () {
//================================================================


  // printf("TTTTTTTTTTTTTTTTTTTTTT Tex_Init__ TTTTTTTTTTTTTTTT\n");
  
  // init (malloc ..)
  UtxTab_init__ (&TexTbFn);

  TexRefNr  = 0;

  TexBasNr  = 0;

  TexBasAct = -1;

  return 0;

}
 

/*
//================================================================
  int Tex_Init_Ref () {
//================================================================


  printf("Tex_Init_Ref\n");

  TexRefNr  = 0;

  // alle Indices in der PermanentAttributeList loeschen
  GA_Tex_reset ();


  return 0;

}
*/

//================================================================
  int TexRef_realloc () {
//================================================================

  TexRefSiz += TexRefINC;


  printf("::TexRef_realloc %d %d\n",TexRefNr,TexRefSiz);

  TexRefTab = (TexRef*)realloc(TexRefTab, (2+TexRefSiz)*sizeof(TexRef));
  if(TexRefTab == NULL) {
    TX_Error("TexRef_realloc EOM");
    return -2;
  }

  return 0;

}



//================================================================
  int TexBas_realloc () {
//================================================================

  TexBasSiz += TexBasINC;


  printf("::TexBas_realloc %d %d\n",TexBasNr,TexBasSiz);

  TexBasTab = (TexBas*)realloc(TexBasTab, (2+TexBasSiz)*sizeof(TexBas));
  if(TexBasTab == NULL) {
    TX_Error("TexBas_realloc EOM");
    return -2;
  }

  return 0;

}

/*
//================================================================
  int Tex_loadBas () {
//================================================================
/// alle TexBas-DL-Records neu erzeugen
// werden leider (mit der gesamten DL) manchmal geloescht.
// Ev muesste man die Texturen gar nicht neu laden, sondern nur
// den DL-Record erzeugen ?


  int    i1;
  long   dli;

  // printf("LLLLLLLLLL Tex_loadBas %d\n",TexBasNr);

  APT_line_set (-1); // damit wirds nicht geloescht m 

  for(i1=0; i1<TexBasNr; ++i1) {

    // APT_LoadTex (0L, &TexBasTab[TexBasAct]);
    dli = DL_StoreObj (Typ_TEXB, 1L, 0);
    GL_LoadTex (&dli, &TexBasTab[i1]);

  }



  return 0;

}
*/

//================================================================
  int Tex_addBas__ (char *fnam, char *dirNam, int keepFlag) {
//================================================================
/// \code
/// save & load Texture
/// Input:
///   fnam
///   dirNam
///   keepFlag: 0=do not keep active texture;
///             1=keep.
/// RetCod:     iTex; -1=error
/// \endcode

// setzt TexBasAct as active TexBasIndex.
// save filename, texture-Nr and texture-DL-index in struct ...
// statt APT_decode_tex DB_StoreTex APT_LoadTex GL_LoadTex
/*
GL_LoadTex
  Input:
    Filename
    TexturNr
  Output:
    DL-Nr (wird vom useTexture gebraucht)


*/

  int    i1;
  char   symNam[200];
  TexBas *tbAct;


  if(AP_stat.texture == 0) return 0;


  // create the symbolic name (in die GATAB soll der symbolicName !)
  strcpy(symNam, dirNam);
  strcat(symNam, "/");          //fnam_del_s);
  strcat(symNam, fnam);
    printf("  fnam=|%s|\n",symNam);


  // load Texture
  i1 = Tex_addBas1 (symNam, keepFlag);
  if(i1 < 0) return -1;


  // printf("ex Tex_addBas1 %d\n",TexBasAct);
  return TexBasAct;    // iTex !

}


//================================================================
  int Tex_addBas1 (char *symNam, int overWrite) {
//================================================================
/// \code
/// load Texture:
/// get BitmapFile -> <tempDir>
/// TexBas generieren, Texture laden; Texture aktiv setzen.
/// load bitmage-imagefile -> OpenGL-Textur.
/// Input:
///   overWrite    0 normal; do not load if basictexture exist.
///                1 overwrite
/// RetCod:        iTex; -1=error
/// \endcode

  int   i1, irc;

  // printf("Tex_addBas1 |%s| %d\n",symNam, overWrite);

  // get BitmapFile -> <tempDir>
  i1 = Tex_getBitmap (memspc011, symNam, overWrite);
  if(i1 < 0) return -1;


  // get BasicTextureNr for Image <symNam>; else create new BasicTexture
  // sets TexBasAct !
  i1 = Tex_getBas1 (&actTexBas, symNam);
  if(i1 < 0) return -1;


  // load bitmage-imagefile -> OpenGL-Textur.
  if((i1 == 0)||(overWrite == 1)) {
    irc = GL_Tex_Load (actTexBas, memspc011);
    if(irc) return -1;
  }


  // // act. TexRef: UserData auf defaults setzen ..
  // Tex_TexRef_InitU ();


  // printf("ex Tex_addBas1 %d\n",TexBasAct);
  return TexBasAct;    // iTex !

}

  
/*
//================================================================
  int Tex_addBas1 (char *fnam, int keepFlag) {
//================================================================
/// save & load Texture

// save filename, texture-Nr and texture-DL-index in struct ...
// save (static) the Nr of the active TexBas
// statt APT_decode_tex DB_StoreTex APT_LoadTex GL_LoadTex

//GL_LoadTex
//  Input:
//    Filename
//    keepFlag: 0=do not keep active texture;         dzt unused ?
//              1=keep.
//  Output:
//    TEXB-Index



 
  int    irc, iNr;
  long   dli;
  char   *p1;


  printf("Tex_addBas1 |%s| keep=%d\n",fnam,keepFlag);


  if(TexBasNr >= TexBasSiz)
    if(TexBas_realloc() < 0) return -1;


  // testen, ob fnam schon existiert ...
  iNr = UtxTab_find (fnam, &TexTbFn);
  if(iNr >= 0) {   // schon geladen ..
    TexBasAct = iNr;
    goto L_done;
  }


  // eigentlich sollte iNr (Recordnr in UtxTab) gleich sein wie TexBasNr

  // save fnam; muss aber permanent sein (nicht loeschen mit RUN)
  iNr = UtxTab_add (&TexTbFn, fnam);

  // query pos of fnam
  iNr = UtxTab_query (&p1, &TexTbFn);
    // printf(" p1=|%s|\n",p1);



  // fix iTex
  // keepFlag: 0=do not keep active texture; overwrite active.
  //           1=keep.
  if(keepFlag == 1) {
    TexBasAct = TexBasNr;
    ++TexBasNr; // keep: do not overWrite last texture
  }
    // printf(" create new basic texture:\n");
    // printf(" TexBasAct=%d TexBasNr=%d\n",TexBasAct,TexBasNr);


  // init TexBasRecord
  TexBasTab[TexBasAct].fnam = p1;
    // printf(" TexBasTab %d |%s|\n",TexBasAct,TexBasTab[TexBasAct].fnam);


  // load Texture
  // APT_LoadTex (0L, &TexBasTab[TexBasAct]);
  APT_line_set (-1); // damit wirds nicht geloescht
  dli = DL_StoreObj (Typ_TEXB, 1L, 0);
  irc = GL_LoadTex (&dli, &TexBasTab[TexBasAct]);
  if(irc < 0) return -1;


  L_done:
  // act. TexRef: defaults  laden ..
  Tex_TexRef_InitU ();


  // printf("ex Tex_addBas1 %d\n",TexBasAct);
  return TexBasAct;    // iTex !

}
*/


//================================================================
  int Tex_getBas__ (TexBas **txb, int ind) {
//================================================================
/// get the Bas.Texture from BasicTextureIndex

  // printf("Tex_getBas__ %d\n",ind);

  *txb = &TexBasTab[ind];

  return 0;

}


//================================================================
  int Tex_getRef (TexRef **tex, int ind) {
//================================================================
/// \code
/// get TexRef-record from TexRef-index
/// (returns a pointer)
/// \endcode

  // printf("Tex_getRef %d\n",ind);

  if(TexRefTab == NULL) return -1;

  *tex = &TexRefTab[ind];

    // DEB_dump_obj__ (Typ_TEXR, *tex, "TB[%d]:  ",ind);

  return 0;

}


//================================================================
  int Tex_addRef () {
//================================================================
/// create new TexRef-Record
/// RetCod = Index of TexRef-Record; is also stored as TexRefAct

// search if TexRef-Record exists else create new


  int   i1;

  // printf("Tex_addRef \n");


  //----------------------------------------------------------------
  // loop durch TexRefTab hilft nix; enthaelt nur ibas u Userdaten.
  // loop durch GA; wenn vtex==1 und typ und ind gleich sind ..
  // for(i1=0; i1<TexRefNr; ++i1) {
    // if(GA_ObjTab[gaNr].iatt == 0) {

  // }




  //----------------------------------------------------------------
  // printf(" neuer GA-Record\n");

  if(TexRefNr >= TexRefSiz)
    if(TexRef_realloc() < 0) return -1;


  // init TexRefRecord
  // TexRefTab[TexRefNr]...

  TexRefAct = TexRefNr;
  ++TexRefNr;

  // printf("ex Tex_addRef %d\n",TexRefAct);
  return TexRefAct;

}



//================================================================
  int Tex_savRef (TexRef *texr) {
//================================================================
/// copy the Ref-Record texr -> Record TexRefAct

  // printf("Tex_savRef %d\n",TexRefAct);

  TexRefTab[TexRefAct] = *texr;

  return 0;

}


//================================================================
  int Tex_getRefInd () {
//================================================================
/// Tex_getRefInd      returns TexRefAct

  printf("Tex_getRefInd %d\n",TexRefAct);

  return TexRefAct;

}


//================================================================
  int Tex_setRefInd (int ind) {
//================================================================
// set TexRef-Index TexRefAct = ind


  int irc;
 
  // printf("Tex_setRefInd %d\n",ind);

  if((ind < 0)||(ind >= TexRefNr)) {TX_Print("Tex_setRefInd E001");return -1;}

  // // printf(" ibas=%d\n",TexBasAct);
  // if(TexBasAct < 0) {TX_Print("no texture is active ..");return -1;}

  TexRefAct = ind;

  // // save parameters from UI_Tex__; calls Tex_savRef to save
  // irc = UI_Tex__ (NULL, (void*)UI_FuncUCB8);

  return 0;

}


//================================================================
  int Tex_RefTest () {
//================================================================
// check if TexBas did change; if yes: reset userData


  // TexBasAct should be TexRefTab[TexRefAct].ibas;
  if(TexBasAct == TexRefTab[TexRefAct].ibas) return 0;

    printf("Tex_RefTest TexRefTab[TexRefAct].ibas=%d TexBasAct=%d\n",
            TexRefTab[TexRefAct].ibas,TexBasAct);


  TexRefTab[TexRefAct].ibas = TexBasAct;

  // yes; different Image; reset userData
  Tex_TexRef_InitU (TexRefAct);

  return 0;

}
 

//================================================================
  int Tex_dump__ (FILE *fpo) {
//================================================================
/// print to terminal: fpo=NULL

  int     i1;
  TexBas  *tb;
  TexRef  *tr;
 

  if(fpo) DEB_dump_obj__ (TYP_FuncInit1, fpo, NULL);

  // dump GA-records
  GA_dump_tex (fpo);  // alle GA-records mit textures raus ..


  // dump TexRef-records
  for(i1=0; i1<TexRefNr; ++i1) {
    tr = &TexRefTab[i1];
    DEB_dump_obj__ (Typ_TEXR, tr, "Texture-Reference[%d]:\n",i1);
  }


  // dump TexBas-records
  for(i1=0; i1<TexBasNr; ++i1) {
    tb = &TexBasTab[i1];
    DEB_dump_obj__ (Typ_TEXB, tb, "Basic-Texture[%d]:\n",i1);
  }

  if(fpo) DEB_dump_obj__ (TYP_FuncExit, NULL, NULL);

  return 0;

}


//========================= EOF =================================
