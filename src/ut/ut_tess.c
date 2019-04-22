//    ut_tess.c                    2005-10-05      RF
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
\file  ../ut/ut_tess.c
\brief tesselated-Surfaces  
\code
=====================================================
List_functions_start:

tess_res_CB__         resolv tess-model in memory with callback
tess_ntri_tfac__      get triangles from tesselated face
tess_ntri_tfac_add    copy 3 (pointers of) points -> Triangle            INLINE

tess_analyz__
tess_analyz_sur
tess_analyz_sph
tess_analyz_con
tess_analyz_fac

tess_box_get          find box of tess-model in memory
tess_box_sur
tess_box_fac

tess_origin_box       get origin from box of tess-model
tess_origin_set__     subract origin of tess-model in memory
tess_origin_set_sur
tess_origin_set_fac

tess_dump_f_          dump file-oriented obj;  structured display.
tess_read_f           read tesselated surf (file-oriented)
tess_write_f_         tesselated surf in Datei ausgeben.
tess_reloc_f_         relocate tesselated surf (read from file)

tess_read_            read .tess-file from temp-dir
tess_write__          write .tess-file into temp-dir

tess_triaNr_bMsh             find nr of surfaces & (total) nr of triangles
tess_triaNr_patch             find nr of triangles in tesselated surface

List_functions_end:
=====================================================
TODO: tess_origin_set_sph tess_origin_set_con tor?
- see also:
TSU_tess_addf       add Mockup-Filestruct oxi to existing Mockup-struct TSU_vMem
TSU_imp_tess        import Mockup from file into Mockup-struct
TSU_exp_sm_sur      export all faces from Mockup-struct
TSU_ntria_bMsh__      make Triangles from Mockup-struct
TSU_DrawSurTess     draw Mockup-struct (tesselated surf)
wrl_reloc__         see also tess_reloc_f_

\endcode *//*----------------------------------------


*/



#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <GL/gl.h>                     // GL_TRIANGLE_FAN ..


#include "../ut/ut_geo.h"              // Point ...
#include "../ut/ut_cast.h"             // INT_PTR
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_os.h"               // OS_ ..

#include "../ut/func_types.h"               // SYM_STAR_S
#include "../xa/xa_mem.h"              // memspc501


// tess_ntri_tfac_add                 copy 3 (pointers of) points -> Triangle
// see also UTRI_tria_3pt
#define tess_ntri_tfac_add(tria,pt1,pt2,pt3)\
 {(tria)->pa[0] = pt1;\
  (tria)->pa[1] = pt2;\
  (tria)->pa[2] = pt3;}



//===========================================================================
//=============== extern glob. vars ======================
// ex ../xa/xa.c:
extern Plane     WC_sur_act;            // Constr.Plane



//===========================================================================
// PROTOTYPES:

int tess_analyz_CB (ObjGX*);






//=======================================================================
  int tess_dump_f_ (ObjGX *oxi, char *txt) {
//=======================================================================
// dump obj - structured display
// see DEB_dump_ox_1

  int  irc, i1, rSiz, totSiz;
  char cbuf[32];


  printf("=================== tess_dump_f_ %s ============ \n",txt);
  i1 = 0;
  totSiz = 0;

  // erste Record ev. Adress
  if(oxi->typ == Typ_Address) {
    printf(" Adress = %p\n",oxi->data);
    ++oxi; //(char*)oxi += sizeof(ObjGX);
  }


  // endRecord ?
  L_next:
  if(oxi->typ  == Typ_Done)  goto L_fertig;

  // erster Record muss size of following Record sein
  if(oxi->typ  != Typ_Size) goto L_Err2;
  rSiz = (long)oxi->data;
  printf("Record %d size=%d\n",i1,rSiz);

  ++oxi;  //(char*)oxi += sizeof(ObjGX);

  sprintf(cbuf, "Rec %d",i1);
  DEB_dump_ox_s_ (oxi, cbuf);

  // (char*)oxi += rSiz;
  oxi = (ObjGX*)((char*)oxi + rSiz);
  totSiz += rSiz;

  ++i1;
  goto L_next;


  L_fertig:
  printf("ex tess_dump_f_ total size = %d\n",totSiz);
  return 0;


  //----------------------------------------------------------------
  L_Err1:
    TX_Error("tess_dump_f_ E001 form %d",oxi->form);
    return -1;


  //----------------------------------------------------------------
  L_Err2:
    TX_Error("tess_dump_f_ E002 typ %d",oxi->typ);
    return -1;

}


//================================================================
  int tess_fnam (char *fnam, char *mnam) {
//================================================================
// returns filename (in temp-dir) end
// retcod; 1=.tess-file alread exists; -1=not.

  int   i1;

  sprintf(fnam, "%s%s.tess",OS_get_tmp_dir(),mnam);

  // check if pretesselated file exists for 
  i1 = OS_checkFilExist (fnam, 1);  //0=no, 1=yes.
  if(i1 == 0) i1 = -1;

    // printf("ex tess_fnam %d |%s|%s|\n",i1,fnam,mnam);

  
  return i1;

}
 

//================================================================
  int tess_read_ (Memspc *impSpc, char *mnam) {
//================================================================
// tess_read_                read .tess-file from temp-dir
// RetCod -1 = pretesselated file does not exist
//         0 = ok; loaded into impSpc->next

  int  i1;
  char cBuf[256];

  printf("tess_read_ |%s|\n",mnam);

  // get filename fnam.tess & check if pretesselated file exists
  i1 = tess_fnam (cBuf, mnam);
    printf("ex-tess_fnam %d |%s|\n",i1,cBuf);
  if(i1 < 0) return i1;

  // tesselate
  return TSU_imp_tess (impSpc, cBuf);
  
}


//================================================================
  int tess_write__ (char *fnam, ObjGX *oxi) {
//================================================================
// tess_write__               write .tess-file into temp-dir

  char cBuf[256];
  FILE *fp=NULL;


  // printf("tess_write__ |%s|\n",fnam);

  sprintf(cBuf, "%s%s.tess",OS_get_tmp_dir(),fnam);
    // printf("    _write_ |%s|\n",cBuf);

  fp = fopen(cBuf,"wb");
  tess_write_f_ (fp, oxi);

  fclose(fp);

  return 0;

}


//================================================================
  int tess_read_f (void *oi, int *oSiz, long fSiz, FILE *fpi) {
//================================================================
// read tesselated surf (file-oriented)
// skip first record (address)
// Format: siehe tess_write_f_

  long   l1;
  void   *ba;
  ObjGX  *oxi;


  // printf("tess_read_f %d %d\n",*oSiz,fSiz);



  // gesamtes file als block lesen ..
  fread (oi, fSiz, 1, fpi);


  // erster Record ist die alte basisadresse
  oxi = oi;
  // if((oxi->typ != Typ_Address)||(oxi->form != Typ_ObjGX)) {
  if(oxi->typ != Typ_Address) {    // 2013-11-08
    TX_Error("tess_read_f E001 - file KO ?");
    return -1;
  }
  // printf(" oldBase %d %d %p\n",oxi->typ,oxi->form,oxi->data);
  ba = oxi->data; // old baseAdress --> ba


  ++oxi; //(char*)oxi += sizeof(ObjGX); // skip baseAdressRecord
  // (char*)oxi += sizeof(ObjGX); // skip RecordSizeRecord
  // printf(" newBase %p\n",(void*)oxi);


  // die Basisadresse korrigieren
  l1 = (char*)oxi - (char*)ba;
  tess_reloc_f_ (oxi, l1);


  // tess_dump_f_ (oi, "ex tess_read_f");
  return 0;

}


//================================================================
  int tess_write_f_ (FILE *fpo, ObjGX *oxi) {
//================================================================
// tesselated surf in Datei ausgeben.
// Format:
//
// Address(107) 143 1 BaseAdress
// Size   (96)  143 1 Size_of_following_OX-Record
//   (OX-Record)
// Size ...
//   (OX-Record)
// Done   (241) 143 1 NULL


  int   irc, i1, rSiz, pNew;
  ObjGX ox1;


  // tess_dump_f_ (oxi, "tess_write_f_ ");

  // die Basisadresse schreiben
  ox1.typ  = Typ_Address;
  ox1.form = Typ_Data;                  // 2013-11-08
  ox1.siz  = 1;
  ox1.data = oxi;
  fwrite (&ox1, sizeof(ObjGX), 1, fpo);     // write Size-Record
    // printf(" Base %p\n",ox1.data);
    // UTI_dump__ (&ox1, sizeof(ObjGX));


  i1 = 0;

  // skip sizeRecord

  L_next:
  if(oxi->typ  == Typ_Done)  goto L_fertig;

  // erster Record muss size of following Record sein
  if(oxi->typ  != Typ_Size) goto L_Err2;
  rSiz = (long)oxi->data;
    // printf("Record %d size=%d\n",i1,rSiz);

  fwrite (oxi, sizeof(ObjGX), 1, fpo);     // write Size-Record
    // UTI_dump__ (&ox1, sizeof(ObjGX));

  ++oxi;  //(char*)oxi += sizeof(ObjGX);

  // den gesamten BLOCK schreiben !!!!
  fwrite ((void*)oxi, rSiz, 1, fpo);
    // UTI_dump__ (oxi, rSiz);
    // DEB_dump_ox_0 (oxi, "tess_write_f_-rec");

/*
  // funktioniert nicht richtig :
  // hier oberstes parent raus ..      see UTO_copy_obj
  fwrite (oxi, sizeof(ObjGX) * oxi->siz, 1, fpo); // write all ObjGX
  // write obj
  UTO_write_obj (fpo, oxi);
*/

  // (char*)oxi += rSiz; // goto next 211-Record
  oxi = (ObjGX*)((char*)oxi + rSiz); // goto next 211-Record
  ++i1;
  goto L_next;


  L_fertig:
  fwrite (oxi, sizeof(ObjGX), 1, fpo);     // write End-Record
    // UTI_dump__ (oxi, sizeof(ObjGX));
  return 0;




  //----------------------------------------------------------------
  L_Err1:
    TX_Error("tess_write_f_ E001 form %d",oxi->form);
    return -1;


  //----------------------------------------------------------------
  L_Err2:
    TX_Error("tess_write_f_ E002 typ %d",oxi->typ);
    return -1;

}


//=======================================================================
  int tess_reloc_f_ (ObjGX *oxi, long pDist) {
//=======================================================================
// relocate tesselated surf (read from file)
// data_neu = data_alt + pDist;
// pDist = newAdress - oldAdress;


  int   irc, rNr;
  long  rSiz;
  ObjGX ox1;


  // printf("tess_reloc_f_ %ld\n",pDist);

  if(pDist == 0) goto L_fertig;


  rNr = 0;


  L_next:
  if(oxi->typ  == Typ_Done)  goto L_fertig;
    // DEB_dump_ox_0 (oxi, "tess_reloc_f_ %d",rNr);


  // erster Record muss size of following Record sein
  if(oxi->typ  != Typ_Size) goto L_Err2;
  rSiz = (long)oxi->data;
    // printf("Record %d size=%d\n",rNr,rSiz);

  ++oxi;  //(char*)oxi += sizeof(ObjGX);

  // relocate record
  UTO_reloc_obj (oxi, pDist);
  // DEB_dump_ox_0 (oxi, "nach tess_reloc_obj");

  // (char*)oxi += rSiz;
  oxi = (ObjGX*)((char*)oxi + rSiz);
  ++rNr;
  goto L_next;


  L_fertig:
  return 0;


  L_Err1:
    TX_Error("tess_reloc_f_ E001 %d form %d",rNr,oxi->form);
    return -1;

  L_Err2:
    TX_Error("tess_reloc_f_ E002 %d typ %d",rNr,oxi->typ);
    return -1;
}


//================================================================
  int tess_tra__ (ObjGX *oxi, Mat_4x3 m1) {
//================================================================
// apply transformation; modify (overwrite) points
// see UTRA_app__ UTRA_app_obj UTRA_tra_pt


  int     i1, i2, i3, i1Nr, i2Nr, iTyp, rSiz;
  ObjGX   *oxc, *oxs;
  Point   *pTab;


  printf("tess_tra__ \n");

  // wenn das erste obj Adress: skip it
  if(oxi->typ == Typ_Address) ++oxi;


  L_next_rec:
  if(oxi->typ  == Typ_Done)  goto L_fertig;


  // erster Record muss size of following Record sein
  if(oxi->typ  != Typ_Size) goto L_Err2;
  rSiz = (long)oxi->data;
  // printf("Record %d size=%d\n",i1,rSiz);

  ++oxi;  //(char*)oxi += sizeof(ObjGX);  // skip prim. Obj ?

  if(oxi->typ == Typ_Color) goto L_rec_done;

  i1Nr = oxi->siz;    // ex Sur  211 143 5
  oxc = oxi->data;    // skip 211 143 5


  //----------------------------------------------------------------
  for(i1=0; i1<i1Nr; ++i1) { // loop tru curves

    // Patch kann ein ObjGX oder nur pTab sein
    if(oxc->form == Typ_ObjGX) {
      i2Nr  = oxc->siz;
      oxs = oxc->data;

    } else {
      i2Nr  = 1;
      oxs = oxc;

    }
    // printf(" ..TSU_exp_fac s[%d] %d %d %d\n",i1,oxs->typ,oxs->form,i2Nr);



    for(i2=0; i2<i2Nr; ++i2) {
        // printf(" ..TSU_exp_fac[%d] typ=%d form=%d siz=%d\n",i2,
          // oxs->typ, oxs->form, oxs->siz);
      iTyp = oxs->form;  // 2005-08-10

      // if(oxs->typ == Typ_Color) { ++oxs; continue;}

      if(iTyp == Typ_PT) {
        // if(oxs->siz < 3)       // auch letzer Punkt !!!
        // printf(" %d Typ_PT [%d]\n",i2,oxs->siz);
        pTab = oxs->data;
        // pt's transformieren
        for(i3=0; i3<oxs->siz; ++i3) {
          UT3D_pt_tra_pt_m3 (&pTab[i3], m1, &pTab[i3]);
        }

      }
      ++oxs;  //(char*)oxs += sizeof(ObjGX);
    }

    ++oxc;  //(char*)oxi += sizeof(ObjGX);

  }



  // (char*)oxi += rSiz;
  L_rec_done:
  oxi = (ObjGX*)((char*)oxi + rSiz);
  ++i1;
  goto L_next_rec;



  L_fertig:
  return 0;


  L_Err2:
    TX_Error("tess_tra_ E002 typ %d",oxi->typ);
    return -1;

}


//================================================================
  int tess_triaNr_bMsh (int *surNr, int *triNr, ObjGX *oxi) {
//================================================================
// find nr of surfaces & (total) nr of triangles in tesselated surface.


  int     rSiz, pAct, cAct, surTyp;
  ObjGX   *actPP, *actCont;

  // int     i1, i2, i3, rSiz, totSiz, Snr, Pnr, pAct, cNr, cAct;
  // int     surTyp;
  // ObjGX   *Sdat, *Pdat, *Prec;
  // ObjGX   *actPP, *actCont;



  *surNr = 0;
  *triNr = 0;
  surTyp = Typ_SURPLN;


  // erste Record ev. Adress
  if(oxi->typ == Typ_Address) ++oxi;


  L_next_Rec:
  if(oxi->typ  == Typ_Done)  goto L_fertig;

  // erster Record muss size of following Record sein
  if(oxi->typ  != Typ_Size) goto L_Err;
  rSiz = (long)oxi->data;
    // printf("Record size=%d\n",rSiz);

  ++oxi;  //(char*)oxi += sizeof(ObjGX);

  // printf(" data: typ=%d form=%d siz=%d\n",oxi->typ,oxi->form,oxi->siz);
  if(oxi->form != Typ_ObjGX) goto L_weiter;



  // Loop ueber alle Patches
  for(pAct=0; pAct<oxi->siz; ++pAct) {

    actPP = &((ObjGX*)oxi->data)[pAct];   // current Patch
      // printf(" p%d typ=%d form=%d siz=%d\n",pAct,
            // actPP->typ,actPP->form,actPP->siz);

    if(actPP->typ == Typ_Typ) {
      // markiert den Start einer neuen surface ..
      ++(*surNr);
      surTyp = (long)actPP->data;

    } else if(actPP->form == Typ_PT) {
      // find nr of tringles for actPP
      // see also TSU_ntria_bMsh_p
      tess_triaNr_patch (triNr, actPP, &surTyp);
      continue;
    }

    if(actPP->form != Typ_ObjGX) continue;



    //---------------------------------------------------
    // Loop ueber alle Contours im current Patch
    for(cAct=0; cAct<actPP->siz; ++cAct) {

      actCont = &((ObjGX*)actPP->data)[cAct];
        // printf(" c%d typ=%d form=%d siz=%d aux=%d\n",cAct,
               // actCont->typ,actCont->form,actCont->siz,actCont->aux);

      if(actCont->form == Typ_PT) {
        // find nr of tringles for actPP
        tess_triaNr_patch (triNr, actCont, &surTyp);
      }
    }
  }



  L_weiter:
  // (char*)oxi += rSiz;
  oxi = (ObjGX*)((char*)oxi + rSiz);
  goto L_next_Rec;



  //----------------------------------------------------------------
  L_fertig:
  if(*surNr < 1) *surNr = 1;
    printf("ex tess_triaNr_bMsh %d %d\n",*surNr,*triNr);
  return 0;


  L_Err:
    TX_Error("tess_triaNr_bMsh E001 form %d",oxi->form);
    return -1;

}


//================================================================
  int tess_triaNr_patch (int *triNr, ObjGX *oxi, int *surTyp) {
//================================================================
// find nr of triangles in tesselated surface.
// surTyp UNUSED

  int   i1;
  
  // printf("tess_triaNr_patch %d typ=%d\n",oxi->siz,oxi->aux);


  if(oxi->siz < 3) {
    printf(" ********** tess_triaNr_patch I001 ***************\n");
    return 0;
  }


  switch (oxi->aux) {

    case GL_TRIANGLES:           // 4
      i1 = oxi->siz / 3;
      break;

    case GL_TRIANGLE_STRIP:      // 5
      i1 = oxi->siz - 2;
      break;

    case GL_FAC_PLANAR:          // 16
    case GL_TRIANGLE_FAN:        // 6
      i1 = oxi->siz - 2;
      break;

    default:
      TX_Error("***** tess_triaNr_patch E002 %d",oxi->aux);

  }


  L_fertig:
    *triNr += i1;
  return 0;
}


//================================================================
  int tess_analyz__ (Memspc *impSpc) {
//================================================================
// see TSU_exp_sm_sur

  ObjGX    ox1;


  printf("tess_analyz__ \n");


  // init
  ox1.typ = TYP_FuncInit;
  tess_analyz_CB (&ox1);


  // resolv all obj's, call tess_analyz_CB
  tess_res_CB__ (impSpc, tess_analyz_CB);


  // exit
  ox1.typ = TYP_FuncExit;
  tess_analyz_CB (&ox1);


  return 0;

}


//================================================================
  int tess_analyz_CB (ObjGX *oxi) {
//================================================================

  static int      iCol;
  static FILE     *fpo;

  int       i1, triNr, triSiz;
  char      s1[256];
  ColRGB    sCol, actCol, defCol;
  Triangle  *triTab, *tr1;


  // printf("tess_analyz_CB %d\n",oxi->form);


  switch (oxi->typ) {


    //----------------------------------------------------------------
    case TYP_FuncInit:        // INIT
      // open outputfile fpo
      sprintf(s1, "%sanalyz.txt",OS_get_tmp_dir());
      fpo = NULL;
      if((fpo = fopen(s1, "w")) == NULL) {
        TX_Print("tess_analyz_CB E001");
        fpo = stdout;
      } else {
        TX_Print(" file %s created ..\n",s1);
      }
      break;


    //----------------------------------------------------------------
    case TYP_FuncExit:         // exit
      fprintf (fpo, "// EOF");
      if(fpo != stdout) {
        fclose (fpo);
      }
        printf(" file closed ..\n");
      break;


    //----------------------------------------------------------------
    case Typ_GL_PP:           // normal faces
      if(oxi->form == Typ_PT) {
        triTab = (Triangle*)memspc501;
        triSiz = sizeof(memspc501) / sizeof(Triangle);
        triNr = 0;
        // get triangles from tesselated face
        tess_ntri_tfac__ (triTab, &triNr, triSiz, oxi);
          // printf("  fac ptNr=%d glTyp=%d fNr=%d\n",oxi->siz,oxi->aux,triNr);
        if(triNr < 1) {
          fprintf(fpo,"****** EMPTY FACE typ=%d form=%d glTyp=%d\n",
                  oxi->typ, oxi->form, oxi->aux);
          return -1;
        }
        for(i1=0; i1<triNr; ++i1) {
          tr1 = &triTab[i1];
          fprintf(fpo,"face %f %f %f - %f %f %f - %f %f %f\n",
            tr1->pa[0]->x,tr1->pa[0]->y,tr1->pa[0]->z,
            tr1->pa[1]->x,tr1->pa[1]->y,tr1->pa[1]->z,
            tr1->pa[2]->x,tr1->pa[2]->y,tr1->pa[2]->z);
        }
        break;
      }


    //----------------------------------------------------------------
    case Typ_Color:   // color of subsequent faces
      if(oxi->form == Typ_Int4) {    // color of subsequent faces
        sCol = *((ColRGB*)&oxi->data);
          fprintf(fpo,"Col r%d g%d b%d\n",sCol.cr,sCol.cg,sCol.cb);
        break;
      }

    //----------------------------------------------------------------
    default:
      printf(" tess_analyz_CB UNUSED typ=%d form=%d\n",oxi->typ,oxi->form);

  }

  return 0;

}


/*
//================================================================
  int tess_analyz__ (Memspc *impSpc) {
//================================================================
// see TSU_exp_sm_sur

  int      rSiz;
  char     s1[256];
  ObjGX    *ox1;
  FILE     *fpo;


  printf("tess_analyz__ \n");


  // open outputfile fpo
  sprintf(s1, "%sanalyz.txt",OS_get_tmp_dir());
  if((fpo = fopen(s1, "w")) == NULL) {
    TX_Print("tess_analyz__ E001");
    return -1;
  }


  // get 1.obj = tess. model
  ox1 = UME_get_start (impSpc);

  // wenn das erste obj Adress: skip it
  if(ox1->typ == Typ_Address) ++ox1;


  //----------------------------------------------------------------
  L_next:
    // erster Record muss size of following Record sein
    if(ox1->typ != Typ_Size) goto L_Err2;
    rSiz = (long)ox1->data;
    // printf("Recordsize=%d\n",rSiz);

    // next record
    ++ox1;  //(char*)oxi += sizeof(ObjGX);

    // analyze this record
    tess_analyz_sur (ox1, fpo);

    // (char*)oxi += rSiz;
    ox1 = (ObjGX*)((char*)ox1 + rSiz);
    if(ox1->typ != Typ_Done) goto L_next;


  //----------------------------------------------------------------
  L_fertig:
  fprintf (fpo, "// EOF");
  fclose (fpo);
  printf(" file %s created ..\n",s1);

  return 0;


  L_Err2:
    TX_Error("tess_analyz__ E002 typ %d",ox1->typ);
    fclose (fpo);
    return -1;

}


//================================================================
  int tess_analyz_sur (ObjGX *oxi, FILE *fpo) {
//================================================================
// ..sur
// ..pat
// facCol
// fac
// see TSU_exp_fac GL_Disp_sur

  int     iSur, iPat, i1Nr, i2Nr, iTyp, iTex;
  ColRGB  *sCol, defCol;
  Vector  *vcn;
  ObjGX   *oxs, *oxd;


  fprintf(fpo, "\n====================\n");
  fprintf(fpo, "tess_analyz_sur typ=%d form=%d siz=%d\n",
          oxi->typ,oxi->form,oxi->siz);


  if(oxi->typ == Typ_GL_Sur) {
    // typ=Typ_GL_Sur, form=Typ_ObjGX
    goto L_GL_Sur;


  } else if(oxi->typ == Typ_Color) {
    // TSU_exp_wrlCol (&oxi->data);  // data ist selbst Color !
    defCol = *((ColRGB*)&oxi->data);
    fprintf(fpo, " defCol %d %d %d\n",defCol.cr,defCol.cg,defCol.cb);
    return 0;


  } else {
    printf("***** tess_analyz_sur E001: typ=%d form=%d siz=%d\n",
           oxi->typ,oxi->form,oxi->siz);
    DEB_dump_obj__(Typ_ObjGX, oxi, " _origin_sur oxi");
    return -1;
  }



  //----------------------------------------------------------------
  // loop tru surfs
  L_GL_Sur:

  i1Nr = oxi->siz;              // ex Sur  211 143 5
  oxd  = oxi->data;

  sCol = NULL;   // defaultCol
  iTex = -1;

  for(iSur=0; iSur<i1Nr; ++iSur) {

    fprintf(fpo, "\n ..sur[%d] typ=%d form=%d siz=%d\n",iSur,
           oxd->typ, oxd->form, oxd->siz);

    // Patch kann ein ObjGX oder nur pTab sein
    if(oxd->form == Typ_ObjGX) {
      i2Nr  = oxd->siz;
      oxs = oxd->data;

    } else {
      i2Nr  = 1;
      oxs = oxd;
    }


    //----------------------------------------------------------------
    // loop tru patches
    for(iPat=0; iPat<i2Nr; ++iPat) {
        fprintf(fpo," ..pat[%d] typ=%d form=%d siz=%d\n",iPat,
          oxs->typ, oxs->form, oxs->siz);


      if(oxs->form == Typ_PT) {
        tess_analyz_fac (oxs, fpo);


      } else if(oxs->form == Typ_VC) {
        // Normalvektor
        vcn = (Vector*)oxs->data;
        fprintf(fpo,"   %d Vector %f %f %f\n",iPat,
          vcn->dx, vcn->dy, vcn->dz);


      } else if(oxs->form == Typ_SPH) {
        tess_analyz_sph (oxs, fpo);


      } else if(oxs->form == Typ_CON) {
        tess_analyz_con (oxs, fpo);


      } else if(oxs->form == Typ_Int4) {

        if(oxs->typ == Typ_Color) {
          // form = Typ_Int4 ?
          sCol = (ColRGB*)&oxs->data;
          iTex = -1;
          fprintf(fpo, "   %d facCol r%d g%d b%d\n",iPat,
                  sCol->cr,sCol->cg,sCol->cb);

        } else if(oxs->typ == Typ_Texture) {
          // sCol = NULL;   form = Typ_Int4 ?
          iTex = INT_PTR (oxs->data);
          fprintf(fpo, "   %d facTex=%d\n",iPat,iTex);

        
        } else if(oxs->typ == Typ_Typ) {
          // markiert den Beginn einer neuen Surf - dzt nur f Intersect used
          // Typ_Typ, Typ_Int4, Typ_SURPLN (58) - supporting surface is planar
          iTyp = INT_PTR (oxs->data);
          fprintf(fpo, "   %d typ=%d\n",iPat,iTyp);
        }


      } else {
        fprintf(fpo, "****** UNKNOWN PATCH typ=%d form=%d\n",oxs->typ,oxs->form);
      }


      ++oxs;  //(char*)oxs += sizeof(ObjGX);
    }
    //----------------------------------------------------------------



    ++oxd;  //(char*)oxi += sizeof(ObjGX);
  }
  //----------------------------------------------------------------




  return 0;

}


//================================================================
  int tess_analyz_sph (ObjGX *oxi, FILE *fpo) {
//================================================================
// sphere
// see GL_disp_sph GR_Disp_patch
//
TODO:
L_disp_sph zerteilen:
  .    get triangles for circ
  .    display triangles in OpenGL

............................
test und use in tess_analyz_sph;
make GL_TRIANGLE_STRIP-s
make 2 GL_TRIANGLE_FAN-s

............................
use in GL_disp_sph
//

  long   dli;
  Sphere *sp1;


  sp1 = oxi->data;


  DEB_dump_obj__ (Typ_SPH, sp1, " sp1: ");


  // dli = DL_StoreObj (Typ_SUR, -1L, 1);  // typ, dbi, att
  // GL_Surf_Ini (&dli, NULL);
  // GL_Col__ (2);
  // GL_disp_sph (&(sp1->pc), sp1->rad, 0);
  // GL_EndList ();

  return 0;

}


//================================================================
  int tess_analyz_con (ObjGX *oxi, FILE *fpo) {
//================================================================
// cone
// see GL_disp_cone
//
TODO:
GL_disp_cone zerteilen:
  .    get triangles for cone
  .    display triangles in OpenGL

............................
test und use in tess_analyz_sph;
make GL_TRIANGLE_STRIP-s
make 2 GL_TRIANGLE_FAN-s

............................
use in GL_disp_cone
//

  long   dli;
  Conus *co1;


  co1 = oxi->data;


  DEB_dump_obj__ (Typ_CON, co1, " co1: ");

  // dli = DL_StoreObj (Typ_SUR, -1L, 1);
  // GL_Surf_Ini (&dli, NULL);
  // GL_Col__ (2);
  // GL_disp_cone (co1, 7, 0);
  // GL_EndList ();

  return 0;

}


//================================================================
  int tess_analyz_fac (ObjGX *oxi, FILE *fpo) {
//================================================================
// Input:
//   oxi->typ  = Typ_GL_PP
//   oxi->form = Typ_PT
//   oxi->aux  = glTyp
//   oxi->siz  = nr of points
//
//   glTyp: 4=GL_TRIANGLES
//          5=GL_TRIANGLE_STRIP
//          6=GL_TRIANGLE_FAN
//          16=GL_FAC_PLANAR
//
// see TSU_ntria_bMsh_p GL_Disp_sur GL_Disp_patch
//     TSU_exp_dxfFac TSU_exp_stlFac TSU_exp_objFac
//     TSU_exp_wrl1Fac TSU_exp_wrl2Fac


  int       i1, triNr, triSiz;
  Triangle  *triTab;


  triTab = (Triangle*)memspc501;
  triSiz = sizeof(memspc501) / sizeof(Triangle);
  triNr = 0;


  // get triangles from tesselated face
  tess_ntri_tfac__ (triTab, &triNr, triSiz, oxi);

  fprintf(fpo,"    fac ptNr=%d glTyp=%d fNr=%d\n",oxi->siz,oxi->aux,triNr);

  if(triNr < 1) {
    fprintf(fpo, "****** EMPTY FACE typ=%d form=%d glTyp=%d\n",
            oxi->typ, oxi->form, oxi->aux);
    return -1;
  }


    // fprintf(fpo,"    fNr=%d\n",triNr);
  for(i1=0; i1<triNr; ++i1) {
    GR_Disp_tria (&triTab[i1], 9);
    // GR_Disp_pt (p1, SYM_STAR_S, 2);
    // GR_Disp_pt (p2, SYM_STAR_S, 2);
    // GR_Disp_pt (p3, SYM_STAR_S, 2);
  }

  return 0;

}
*/


//================================================================
  int tess_ntri_tfac__  (Triangle *triTab, int *triNr, int triSiz,
                       ObjGX *oxi) {
//================================================================
// get triangles from tesselated face
// Input:
//   triSiz    size of triTab
// Output:
//   triTab
//   triNr
//
// TODO: use UTRI_ntria_patch
// see TSU_ntria_bMsh_p

  int   i0, i1, i2, i3, ia, ie, it, surTyp;
  Point *pTab;



  surTyp = oxi->aux;


  if(oxi->siz < 3) {
    printf("tess_analyz_tri I001\n");
    return -1;
  }


  ia   = 0;
  ie   = oxi->siz - 1;
  pTab = oxi->data;

  it   = 0;   





  //----------------------------------------------------------------
  if((surTyp != GL_TRIANGLE_FAN) &&
     (surTyp != GL_FAC_PLANAR))          goto L_GLS;
    // 6  = TRIANGLE_FAN
    // 16 = nonPlanar TRIANGLE_FAN:
    //    1------2           ptAnz = 4
    //    |    /  |
    //    |   /   |
    //    |  /    |
    //    | /     |
    //    0.------3
    //     \     /
    //       \  /
    //        4
    //
    // sollte so zerlegt werden:
    // 0 1 2
    // 0 2 3
    // 0 3 4

    // make ie = nr of points without last point
    // pTab can have last point
    if(UT3D_comp2pt(&pTab[ia],&pTab[ie],UT_TOL_pt) == 1) ie -= 1;
      // printf(" ie=%d pNr=%d\n",ie,oxi->siz);

    // compute nr of triangles
    *triNr = ie - 1;
    if(*triNr > triSiz) goto L_err1;

    i0 = ia;
    i2 = i0 + 1;


    //----------------------------------------------------------------
    L_f_nxt:
    i1 = i2;
    ++i2;

    // add next triangle
    tess_ntri_tfac_add (&triTab[it], &pTab[i0],&pTab[i1],&pTab[i2]);
    ++it;

    if(i2 < ie) goto L_f_nxt;
    //----------------------------------------------------------------

    goto L_fertig;





  //----------------------------------------------------------------
  L_GLS:
  if(surTyp != GL_TRIANGLE_STRIP) goto L_GLT;
    // 5 = GL_TRIANGLE_STRIP
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

    // compute nr of triangles
    *triNr = ie - 1;
    if(*triNr > triSiz) goto L_err1;

    i1 = ia;
    i3 = i1 + 1;       // 1 - 2


    //----------------------------------------------------------------
    L_s_nxt:
    // i1 bleibt
    i2 = i3;
    ++i3;          // 1 2 3       3 4 5

    tess_ntri_tfac_add (&triTab[it], &pTab[i1],&pTab[i2],&pTab[i3]);
      // printf(" i1=%d i2=%d i3=%d it=%d\n",i1,i2,i3,it);
    ++it;

    if(i3 >= ie) goto L_fertig;

    // i2 bleibt
    i1 = i3;
    ++i3;           // 3 2 4

    tess_ntri_tfac_add (&triTab[it], &pTab[i1],&pTab[i2],&pTab[i3]);
      // printf(" i1=%d i2=%d i3=%d it=%d\n",i1,i2,i3,it);
    ++it;

    if(i3 >= ie) goto L_fertig;
    goto L_s_nxt;








  //================================================================
  L_GLT:
    //
    if(surTyp != GL_TRIANGLES) {
      TX_Print("tess_ntri_tfac__ E001 %d",surTyp);
      return -1;
    }
    // 4 = GL_TRIANGLES
    //
    // 0 1 2
    // 3 4 5
    // 6 7 8

    // compute nr of triangles
    *triNr = oxi->siz / 3;
    if(*triNr > triSiz) goto L_err1;
  
    i3 = ia - 1;


    //----------------------------------------------------------------
    L_t_nxt:
      i1 = i3 + 1;
      i2 = i1 + 1;
      i3 = i2 + 1;
  
      // add next triangle
      tess_ntri_tfac_add (&triTab[it], &pTab[i1],&pTab[i2],&pTab[i3]);
      ++it;
  
      if(i3 < ie) goto L_t_nxt;
    //----------------------------------------------------------------
  
    // goto L_fertig;



  //================================================================
  L_fertig:

    // TESTBLOCK
    // if(*triNr > 1) AP_debug__ ("tess_ntri_tfac__");
    // ENDTESTBLOCK

    return 0;


  L_err1:
    TX_Print ("TSU_exp_objFac E002");
    return -1;

}


//======================================================================
  int tess_box_get (Point *pb1, Point *pb2, Memspc *impSpc) {
//======================================================================
// find box of tess-model in memory


  int         rSiz;
  // char     s1[256];
  ObjGX       *ox1;


  printf("tess_box_get\n");

  // init box
  UT3D_box_ini0 (pb1, pb2);

  // get 1.obj = tess. model
  ox1 = UME_get_start (impSpc);

  // wenn das erste obj Adress: skip it
  if(ox1->typ == Typ_Address) ++ox1;

  //----------------------------------------------------------------
  L_next:
    // erster Record muss size of following Record sein
    if(ox1->typ != Typ_Size) goto L_Err2;
    rSiz = (long)ox1->data;
    // printf("Recordsize=%d\n",rSiz);

    // next record
    ++ox1;  //(char*)oxi += sizeof(ObjGX);

    // test points of this record
    tess_box_sur (pb1, pb2, ox1);

    // (char*)oxi += rSiz;
    ox1 = (ObjGX*)((char*)ox1 + rSiz);
    if(ox1->typ != Typ_Done) goto L_next;


  //----------------------------------------------------------------
  L_fertig:

      // TESTBLOCK
      // DEB_dump_obj__(Typ_PT, pb1, "ex-tess_box_get pb1=");
      // DEB_dump_obj__(Typ_PT, pb2, "ex-tess_box_get pb2=");
      // END TESTBLOCK

    return 0;


  L_Err2:
    TX_Error("tess_box_get E002 typ %d",ox1->typ);
    return -1;

}


//================================================================
  int tess_box_sur (Point *pb1, Point *pb2, ObjGX *oxi) {
//================================================================

  int     iSur, iPat, i1Nr, i2Nr;
  ObjGX   *oxs, *oxd;


  if(oxi->typ == Typ_GL_Sur) {
    // typ=Typ_GL_Sur, form=Typ_ObjGX
    goto L_GL_Sur;

  // defCol kommt als single Record;
  } else if(oxi->typ == Typ_Color) {
    // ignore defCol
    return 0;

  } else {
    printf("***** tess_box_sur E001: typ=%d form=%d siz=%d\n",
           oxi->typ,oxi->form,oxi->siz);
    DEB_dump_obj__(Typ_ObjGX, oxi, " _origin_sur oxi");
    return -1;
  }



  //----------------------------------------------------------------
  // loop tru surfs
  L_GL_Sur:

  i1Nr = oxi->siz;              // ex Sur  211 143 5
  oxd  = oxi->data;


  for(iSur=0; iSur<i1Nr; ++iSur) {


    // Patch kann ein ObjGX oder nur pTab sein
    if(oxd->form == Typ_ObjGX) {
      i2Nr  = oxd->siz;
      oxs = oxd->data;

    } else {
      i2Nr  = 1;
      oxs = oxd;
    }


    //----------------------------------------------------------------
    // loop tru patches
    for(iPat=0; iPat<i2Nr; ++iPat) {

      if(oxs->form == Typ_PT) {
        tess_box_fac (pb1, pb2, oxs);


      } else if(oxs->form == Typ_SPH) {
        UT3D_box_Sphere (pb1, pb2, (Sphere*)oxs->data);


      } else if(oxs->form == Typ_CON) {
        UT3D_box_Conus (pb1, pb2, (Conus*)oxs->data);


      } else if(oxs->form == Typ_VC) {
        // normalVector; // ignore

      } else if(oxs->form == Typ_Int4) {
        // Typ_Color Typ_Texture Typ_Typ; ignore


      } else {
        printf("****** tess_box_sur: UNKNOWN PATCH typ=%d form=%d\n",
                oxs->typ,oxs->form);
      }


      ++oxs;  //(char*)oxs += sizeof(ObjGX);
    }
    //----------------------------------------------------------------


    ++oxd;  //(char*)oxi += sizeof(ObjGX);
  }
  //----------------------------------------------------------------


  return 0;

}

/*
//================================================================
  int UT3D_box_Conus (Point *pb1, Point *pb2, ObjGX *oxi) {
//================================================================
// 2 points; cen + r-vx + r-vy + r-vz

  Conus     *co1;
  Circ      ci1;
  Point     po, p1, p2;
  

  co1 = oxi->data;

  DEB_dump_obj__ (Typ_CON, co1, " UT3D_box_Conus ");

  // get bottom-circ
  po = co1->pl.po;
  UT3D_ci_ptvcr (&ci1, &po,  &co1->pl.vz,  co1->r1);
  UT3D_box_ci (&p1, &p2, &ci1);
  UT3D_box_extend (pb1, pb2, &p1);
  UT3D_box_extend (pb1, pb2, &p2);


  // UT3D_pt_traptmultvc (&p1, &sp1->pc, &WC_sur_act.vx, sp1->rad)
// 
  // UT3D_pt_traptmultvc (&p2, &p1, &WC_sur_act.vy, sp1->rad)
// 
  // UT3D_box_extend (pb1, pb2, &p2);
// 
  // UT3D_pt_opp2pt (&p1, &sp1->pc, &p2);
// 
  // UT3D_box_extend (pb1, pb2, &p1);

  return 0;

}


//================================================================
  int UT3D_box_Sphere (Point *pb1, Point *pb2, ObjGX *oxi) {
//================================================================
// 2 points; cen + r-vx + r-vy + r-vz

  Sphere    *sp1;
  Point     p1, p2;

  sp1 = oxi->data;

  // DEB_dump_obj__ (Typ_SPH, sp1, " UT3D_box_Sphere ");

  UT3D_pt_traptmultvc (&p1, &sp1->pc, &WC_sur_act.vx, sp1->rad)

  UT3D_pt_traptmultvc (&p2, &p1, &WC_sur_act.vy, sp1->rad)

  UT3D_box_extend (pb1, pb2, &p2);

  UT3D_pt_opp2pt (&p1, &sp1->pc, &p2);

  UT3D_box_extend (pb1, pb2, &p1);

  return 0;

}
*/

//================================================================
  int tess_box_fac (Point *pb1, Point *pb2, ObjGX *oxi) {
//================================================================
// Input:
//   oxi->typ  = Typ_GL_PP
//   oxi->form = Typ_PT
//   oxi->aux  = glTyp
//   oxi->siz  = nr of points
//   oxi->data = array of <oxi->siz> points
//
//  
// see gis_ori__
    
    
  int   i1, pNr;
  Point *pTab;


  pNr  = oxi->siz;
  pTab = oxi->data;

  // printf("tess_box_fac %d\n",pNr);

  // loop tru points; extend box
  for(i1=0; i1<pNr; ++i1) {
    UT3D_box_extend (pb1, pb2, &pTab[i1]);
      // GR_Disp_pt (&pTab[i1], SYM_STAR_S, 2);
      // DEB_dump_obj__(Typ_PT, &pTab[i1], " _origin_fac p[%d]",i1);
  }


  return 0;

}


//================================================================
  int tess_origin_box (Point *pOri, Point *pb1, Point *pb2) {
//================================================================
// get origin from box of tess-model


  double  midDist, boxDist;
  Point   midPt;
  Vector  boxSiz;


  // get midPt of box
  UT3D_pt_mid2pt (&midPt, pb1, pb2);

  // set midDist = max dist of midPt (max val of dx | dy | dz)
  UT3D_bplen_vc (&midDist, (Vector*)&midPt);

  // set boxSiz = Vector pb1 - pb2
  UT3D_vc_2pt (&boxSiz, pb1, pb2);

  // set boxDist = max dist of boxSiz (max val of dx | dy | dz)
  UT3D_bplen_vc (&boxDist, &boxSiz);
    printf(" _origin_box midDist=%lf boxDist=%lf\n",midDist,boxDist);

  // if (midDist > (boxSiz / 2)) then pOri = midPt; else pOri = pNull;
  if (midDist > (boxDist / 2)) {
    // round 2 sig
    pOri->x = UTP_db_rnd2sig (midPt.x);
    pOri->y = UTP_db_rnd2sig (midPt.y);
    pOri->z = UTP_db_rnd2sig (midPt.z);

  } else {
    *pOri = UT3D_PT_NUL;
  }

    DEB_dump_obj__ (Typ_PT, pOri, "ex tess_origin_box");

  return 0;

}


//================================================================
  int tess_origin_set__ (Memspc *impSpc, Point *pOri) {
//================================================================
// subract origin of tess-model in memory
// Input:
//   oxi       tesss.model
// see tess_analyz__ tess_box_get 

  int         i1, rSiz;
  ObjGX       *ox1;


  // get 1.obj = tess. model
  ox1 = UME_get_start (impSpc);
  
  // wenn das erste obj Adress: skip it
  if(ox1->typ == Typ_Address) ++ox1;

  //----------------------------------------------------------------
  L_next:
    // erster Record muss size of following Record sein
    if(ox1->typ != Typ_Size) goto L_Err2;
    rSiz = (long)ox1->data;
    // printf("Record %d size=%d\n",i1,rSiz);

    // next record
    ++ox1;  //(char*)oxi += sizeof(ObjGX);

    // subtract
    tess_origin_set_sur (pOri, ox1);

    ox1 = (ObjGX*)((char*)ox1 + rSiz);
    if(ox1->typ != Typ_Done) goto L_next;


  //----------------------------------------------------------------
  L_fertig:

  return 0;

  L_Err2:
    TX_Error("tess_origin_set__ E002 typ %d",ox1->typ);
    return -1;


}


//================================================================
  int tess_origin_set_sur (Point *pOri, ObjGX *oxi) {
//================================================================

  int     iSur, iPat, i1Nr, i2Nr;
  ObjGX   *oxs, *oxd;


  if(oxi->typ == Typ_GL_Sur) {
    // typ=Typ_GL_Sur, form=Typ_ObjGX
    goto L_GL_Sur;

  // defCol kommt als single Record;
  } else if(oxi->typ == Typ_Color) {
    // ignore defCol
    return 0;

  } else {
    printf("***** tess_origin_set_sur E001: typ=%d form=%d siz=%d\n",
           oxi->typ,oxi->form,oxi->siz);
    DEB_dump_obj__(Typ_ObjGX, oxi, " _origin_sur oxi");
    return -1;
  }



  //----------------------------------------------------------------
  // loop tru surfs
  L_GL_Sur:

  i1Nr = oxi->siz;              // ex Sur  211 143 5
  oxd  = oxi->data;


  for(iSur=0; iSur<i1Nr; ++iSur) {


    // Patch kann ein ObjGX oder nur pTab sein
    if(oxd->form == Typ_ObjGX) {
      i2Nr  = oxd->siz;
      oxs = oxd->data;

    } else {
      i2Nr  = 1;
      oxs = oxd;
    }


    //----------------------------------------------------------------
    // loop tru patches
    for(iPat=0; iPat<i2Nr; ++iPat) {

      if(oxs->form == Typ_PT) {
        tess_origin_set_fac (pOri, oxs);


      // } else if(oxs->form == Typ_SPH) {
        // tess_origin_set_sph (oxs, fpo);


      // } else if(oxs->form == Typ_CON) {
        // tess_origin_set_con (oxs, fpo);


      } else if(oxs->form == Typ_VC) {
        // normalVector; // ignore

      } else if(oxs->form == Typ_Int4) {
        // Typ_Color Typ_Texture Typ_Typ; ignore


      } else {
        printf("****** tess_origin_set_sur: UNKNOWN PATCH typ=%d form=%d\n",
                oxs->typ,oxs->form);
      }


      ++oxs;  //(char*)oxs += sizeof(ObjGX);
    }
    //----------------------------------------------------------------

    ++oxd;  //(char*)oxi += sizeof(ObjGX);
  }
  //----------------------------------------------------------------



  return 0;

}


//================================================================
  int tess_origin_set_fac (Point *pOri, ObjGX *oxi) {
//================================================================
// subtract vector
// Input:
//   oxi->typ  = Typ_GL_PP
//   oxi->form = Typ_PT
//   oxi->aux  = glTyp
//   oxi->siz  = nr of points
//   oxi->data = array of <oxi->siz> points
//
//  
// see gis_ori__


  int   i1, pNr;
  Point *pTab;


  pNr  = oxi->siz;
  pTab = oxi->data;

  // printf("tess_box_fac %d\n",pNr);

  // loop tru points; extend box
  for(i1=0; i1<pNr; ++i1) {
    UT3D_pt_sub_pt3 (&pTab[i1], pOri);
      // GR_Disp_pt (&pTab[i1], SYM_STAR_S, 2);
      // DEB_dump_obj__(Typ_PT, &pTab[i1], " _origin_fac p[%d]",i1);
  }


  return 0;

}


//================================================================
  int tess_res_CB__ (Memspc *impSpc, int (*funcnam)()) {
//================================================================
// see TSU_exp_sm_sur

  int      rSiz;
  char     s1[256];
  ObjGX    *ox1;
  FILE     *fpo;


  printf("tess_res_CB__ \n");


  // get 1.obj = tess. model
  ox1 = UME_get_start (impSpc);

  // wenn das erste obj Adress: skip it
  if(ox1->typ == Typ_Address) ++ox1;


  //----------------------------------------------------------------
  L_next:
    // erster Record muss size of following Record sein
    if(ox1->typ != Typ_Size) goto L_Err2;
    rSiz = (long)ox1->data;
    // printf("Recordsize=%d\n",rSiz);

    // next record
    ++ox1;  //(char*)oxi += sizeof(ObjGX);

    // analyze this record
    tess_res_CB_sur (ox1, funcnam);

    // (char*)oxi += rSiz;
    ox1 = (ObjGX*)((char*)ox1 + rSiz);
    if(ox1->typ != Typ_Done) goto L_next;


  //----------------------------------------------------------------
  L_fertig:

  return 0;


  L_Err2:
    TX_Error("tess_res_CB__ E001 typ=%d form=%d",ox1->typ,ox1->form);
    return -1;

}


//================================================================
  int tess_res_CB_sur (ObjGX *oxi, int (*funcnam)()) {
//================================================================
// ..sur
// ..pat
// facCol
// fac
// see TSU_exp_fac GL_Disp_sur

  int     iSur, iPat, i1Nr, i2Nr, iTyp, iTex;
  ColRGB  *sCol;
  Vector  *vcn;
  ObjGX   *oxs, *oxd;


  printf("\n====================\n");
  printf("tess_res_CB_sur typ=%d form=%d siz=%d\n",
          oxi->typ,oxi->form,oxi->siz);


  if(oxi->typ == Typ_GL_Sur) {
    // typ=Typ_GL_Sur, form=Typ_ObjGX
    goto L_GL_Sur;


  } else if(oxi->typ == Typ_Color) {
    sCol = (ColRGB*)&oxi->data;
      printf("  defCol r%d g%d b%d\n",sCol->cr,sCol->cg,sCol->cb);
    funcnam (oxi);
    return 0;


  } else {
    printf("***** tess_analyz_sur E001: typ=%d form=%d siz=%d\n",
           oxi->typ,oxi->form,oxi->siz);
    DEB_dump_obj__(Typ_ObjGX, oxi, " _origin_sur oxi");
    return -1;
  }



  //----------------------------------------------------------------
  // loop tru surfs
  L_GL_Sur:

  i1Nr = oxi->siz;              // ex Sur  211 143 5
  oxd  = oxi->data;

  // sCol = NULL;   // defaultCol
  iTex = -1;

  for(iSur=0; iSur<i1Nr; ++iSur) {
    // printf("\n ..sur[%d] typ=%d form=%d siz=%d\n",iSur,
           // oxd->typ, oxd->form, oxd->siz);

    // Patch kann ein ObjGX oder nur pTab sein
    if(oxd->form == Typ_ObjGX) {
      i2Nr  = oxd->siz;
      oxs = oxd->data;

    } else {
      i2Nr  = 1;
      oxs = oxd;
    }


    //================================================================
    // loop tru patches
    for(iPat=0; iPat<i2Nr; ++iPat) {
        // printf(" ..pat[%d] typ=%d form=%d siz=%d\n",iPat,
          // oxs->typ, oxs->form, oxs->siz);


      //----------------------------------------------------------------
      if(oxs->form == Typ_PT) {
        funcnam (oxs);


      //----------------------------------------------------------------
      } else if(oxs->form == Typ_VC) {
        // Normalvektor
        printf("****** tess_res_CB_sur UNUSED RECORD Typ_VC\n");
        // vcn = (Vector*)oxs->data;
          // printf("tess_res_CB_sur vcn %f %f %f\n",vcn->dx,vcn->dy,vcn->dz);


      //----------------------------------------------------------------
      } else if(oxs->form == Typ_SPH) {
        // tess_analyz_sph (oxs, fpo);
        printf("****** tess_res_CB_sur UNUSED RECORD Typ_SPH\n");


      //----------------------------------------------------------------
      } else if(oxs->form == Typ_CON) {
        // tess_analyz_con (oxs, fpo);
        printf("****** tess_res_CB_sur UNUSED RECORD Typ_CON\n");


      //----------------------------------------------------------------
      } else if(oxs->form == Typ_Int4) {

        //----------------------------------------------------------------
        if(oxs->typ == Typ_Color) {
          iTex = -1;
          sCol = (ColRGB*)&oxs->data;
            printf("  Col r%d g%d b%d\n",sCol->cr,sCol->cg,sCol->cb);
          funcnam (oxs);


        //----------------------------------------------------------------
        } else if(oxs->typ == Typ_Texture) {
          // sCol = NULL;   form = Typ_Int4 ?
          iTex = INT_PTR (oxs->data);
          // fprintf(fpo, "   %d facTex=%d\n",iPat,iTex);
            printf("****** tess_res_CB_sur UNUSED RECORD Typ_Texture\n");


        //----------------------------------------------------------------
        } else if(oxs->typ == Typ_Typ) {
          // markiert den Beginn einer neuen Surf - dzt nur f Intersect used
          // Typ_Typ, Typ_Int4, Typ_SURPLN (58) - supporting surface is planar
          iTyp = INT_PTR (oxs->data);
          // fprintf(fpo, "   %d typ=%d\n",iPat,iTyp);
        }


      //----------------------------------------------------------------
      } else {
        printf("****** UNKNOWN PATCH typ=%d form=%d\n",oxs->typ,oxs->form);
      }


      ++oxs;  //(char*)oxs += sizeof(ObjGX);
    }
    //----------------------------------------------------------------



    ++oxd;  //(char*)oxi += sizeof(ObjGX);
  }
  //----------------------------------------------------------------




  return 0;

}


//============================= EOF ==============================
