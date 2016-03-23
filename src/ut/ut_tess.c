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

tess_dump_f_        dump file-oriented obj;  structured display.
tess_read_f         read tesselated surf (file-oriented)
tess_write_f_       tesselated surf in Datei ausgeben.
tess_reloc_f_       relocate tesselated surf (read from file)

tess_read_          read .tess-file from temp-dir
tess_write__        write .tess-file into temp-dir

tess_siz1           find nr of surfaces & (total) nr of triangles
tess_siz2           find nr of triangles in tesselated surface

List_functions_end:
=====================================================
- see also:
TSU_tess_addf       add Mockup-Filestruct oxi to existing Mockup-struct TSU_vMem
TSU_imp_tess        import Mockup from file into Mockup-struct
TSU_exp_sm_sur      export all faces from Mockup-struct
TSU_tsu2tria__      make Triangles from Mockup-struct
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
#include "../ut/ut_txt.h"              // fnam_del
#include "../ut/ut_os.h"               // OS_ ..






//=======================================================================
  int tess_dump_f_ (ObjGX *oxi, char *txt) {
//=======================================================================
// dump obj - structured display
// see UTO_dump_1

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
  UTO_dump_s_ (oxi, cbuf);

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
    // UTO_dump__ (oxi, "tess_write_f_-rec");

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
    // UTO_dump__ (oxi, "tess_reloc_f_ %d",rNr);


  // erster Record muss size of following Record sein
  if(oxi->typ  != Typ_Size) goto L_Err2;
  rSiz = (long)oxi->data;
    // printf("Record %d size=%d\n",rNr,rSiz);

  ++oxi;  //(char*)oxi += sizeof(ObjGX);

  // relocate record
  UTO_reloc_obj (oxi, pDist);
  // UTO_dump__ (oxi, "nach tess_reloc_obj");

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
          UT3D_pt_traptm3 (&pTab[i3], m1, &pTab[i3]);
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
  int tess_siz1 (int *surNr, int *triNr, ObjGX *oxi) {
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
      // see also TSU_tsu2tria_rec
      tess_siz2 (triNr, actPP, &surTyp);
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
        tess_siz2 (triNr, actCont, &surTyp);
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
    printf("ex tess_siz1 %d %d\n",*surNr,*triNr);
  return 0;


  L_Err:
    TX_Error("tess_siz1 E001 form %d",oxi->form);
    return -1;

}


//================================================================
  int tess_siz2 (int *triNr, ObjGX *oxi, int *surTyp) {
//================================================================
// find nr of triangles in tesselated surface.

  int   i1;
  
  // printf("tess_siz2 %d typ=%d\n",oxi->siz,oxi->aux);


  if(oxi->siz < 3) {
    printf(" ********** tess_siz2 I001 ***************\n");
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
      TX_Error("***** tess_siz2 E002 %d",oxi->aux);

  }


  L_fertig:
    *triNr += i1;
  return 0;
}

//============================= EOF ==============================
