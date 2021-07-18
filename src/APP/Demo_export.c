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
\file  ../APP/Demo_export.c
\brief resolve model

Demo export active group or whole model

\code
=====================================================
List_functions_start:

Demo_exp_export        main entry, export active group or whole model
Demo_exp_mdl__         export all objs of grp1
Demo_exp_exp_ox        export obj

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


*/

#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef _MSC_VER
// die folgenden 2 Funktionen exportieren (werden vom Main gerufen):
__declspec(dllexport) int gCad_main ();
__declspec(dllexport) int gCad_fini ();
// nachfolgende externals werden aus dem Main-Exe imported:
#define extern __declspec(dllimport)
#endif


#include "../ut/ut_geo.h"                 // Point ...
#include "../ut/func_types.h"             // UI_Func...
#include "../ut/ut_memTab.h"              // MemTab_..

// #include "../gr/ut_DL.h"               // DB_GetDLatt

#include "../db/ut_DB.h"                  // DB_GetObjGX

#include "../xa/xa.h"                     // MDL_IS_MAIN
#include "../xa/xa_msg.h"                 // MSG_ERR_typ_*

#include "../gr/ut_DL.h"               // DL_OBJ_IS_ACTMDL



// prototypes:
typedef_MemTab(int);



static int exp_errNr = 0;
static int exp_objNr = 0;



//=========================================================
  int gCad_fini () {
//=========================================================

  TX_Print("exiting Demo_export.");

  // DL_hili_off (-1L);         //  -1 = unhilite all
  DL_Redraw ();               // Redraw DispList

  AP_User_reset ();           // close application

  return 0;
}


//=========================================================
  int gCad_main () {
//=========================================================


  TX_Print(">>>>>>>>> gCad_main aus Demo_export <<<<<<<<<<");


  // Demo_exp_test (); goto L_exit;

  Demo_exp_export (); // export the active model

  gCad_fini ();   // close plugin

  return 0;

}


//================================================================
  int Demo_exp_export () {
//================================================================
// Demo_exp_export           export active group or whole model
 

  int         irc, oNr, i1, iTyp, mbi, iMdl;
  long        l1;
  char        safNam[SIZMFTot], safPrim[SIZMFTot], *p1;
  ObjDB       *oTab;
  ObjGX       ox1;
  ModelRef    *mr;
  ModelBas    *mbo;
  MemTab(int) grp1 = _MEMTAB_NUL;    // DL-indexes of objs to export
  MemTab(int) smTab = _MEMTAB_NUL;   // basMdl-indexes of used submodels

#define SIZ_SMTAB 1000               // size list subModelnames



  exp_errNr = 0;
  exp_objNr = 0;


  printf("\n \n");
  printf("exp_export ============================\n");

  MemTab_ini__ (&smTab, sizeof(int), Typ_Int4, SIZ_SMTAB);

  LOG_A_init ("exp_export");


  //================================================================
  // process primary model
  //================================================================
  // get the active modelIndex
  iMdl = AP_get_modact_ind ();          // -1 = main, else a subModel is active
  if(MDL_IS_MAIN) {
     printf("main is active ..\n");
  } else {
    printf("subModel is active ..\n");
  }


    printf("\n====================== MAIN ======================= \n");
    printf("exp_export ============================ %d\n",iMdl);

  
  //----------------------------------------------------------------
  // get grp1 = list of objs in DL 
  // get nr of objs in active group
  i1 = Grp_get_nr ();
  if(i1 > 0) {
    // copy objs of active group into private group grp1
    Grp1_add__ (&grp1);

  } else {
    // get all objs in primary-model
    Grp1_add_sm_dl (&grp1, iMdl);
  }

    // TESTBLOCK
    // MemTab_dump (&grp1, "exp_export-L1");
    // return -1;
    // END TESTBLOCK


  //----------------------------------------------------------------
  // export all objs of grp1
  Demo_exp_mdl__ (&grp1, &smTab, iMdl);

    // return -1;


  //================================================================
  // process subModels
  //================================================================
  if(smTab.rNr < 1) goto L_finish;

  // get safe name of primary model
  MDL_safNam_mnam (safPrim, MDL_mNam_prim ());

  // store active DB
  DB_save__ (safPrim);


  i1 = 0;

  L_nxt_sm:

    // get baseModelIndex iMdl
    MemTab_get (&iMdl, &smTab, i1, 1);

    // get modelName from index
    mbo = DB_get_ModBas (iMdl);
      // DEB_dump_obj__(Typ_SubModel, mbo, "sm %d",iMdl);

      printf("\n\n================================================== \n");
      printf(" ------- nxt-sm %d %d |%s|\n",i1,iMdl,mbo->mnam);

    // activate subModel
    DB_load__ (MDL_safNam_mnam(safNam, mbo->mnam));

    // get group smTab of all objs of subModel mbi in DL
    Grp1_add_sm_dl (&grp1, iMdl);

    // export subModel
    Demo_exp_mdl__ (&grp1, &smTab, iMdl);

    ++i1;
    if(i1 < smTab.rNr) goto L_nxt_sm;


    // reload primary model
    DB_load__ (safPrim);



  //================================================================
  L_finish:
  //================================================================

  printf("\n\n================================================== \n");
  printf(" ------- finish\n");


  LOG_A__ (MSG_ERR_typ_INF, "nr of errors = %d", exp_errNr);
  LOG_A__ (MSG_ERR_typ_INF, "nr of objs exported = %d", exp_objNr);

  LOG_A_exit (exp_errNr);

  MemTab_free (&smTab);
  MemTab_free (&grp1);

    // TESTBLOCK
    // LOG_A_disp ();
    // END TESTBLOCK


  return exp_errNr;

}


//=======================================================================
  int Demo_exp_mdl__ (MemTab(int) *grp1, MemTab(int) *smTab, int iMdl) {
//=======================================================================
// Demo_exp_mdl__              export all objs of grp1
//   add all used subModels to smTab

  int         irc, i1, oNr, iTyp, mbi;
  long        dbi, dli, dlNr, l1;
  DL_Att      *dla;
  ObjGX       ox1;
  ModelRef    *mr;


  printf(" Demo_exp_mdl__ %d\n",iMdl);

  oNr = grp1->rNr;

  dlNr = DL_get__ (&dla);


  for(i1=0; i1<oNr; ++i1) {
    dli = *((int*)MEMTAB__ (grp1, i1));
    iTyp = dla[dli].typ;
      printf("\n ------- nxt-obj %d typ=%d\n",i1,iTyp);
      DL_DumpObj__ (dli, "Demo_exp_mdl__");

    // skip this types:
    if(iTyp == Typ_Ditto) continue; // mockup-baseModel

    // get the DB-index
    dbi = dla[dli].ind;

    // get DB-obj
    ox1 = DB_GetObjGX (iTyp, dbi);
    if(ox1.typ == Typ_Error) {
      LOG_A__ (MSG_ERR_typ_ERR, "exp_export typ=%d dbi=%ld",iTyp,dbi);
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
    irc = Demo_exp_ox (&ox1, dbi);
    if(!irc) ++exp_objNr;
  }

  return 0;

}


//=====================================================================
  int Demo_exp_ox (ObjGX *ox1, long dbi) {
//=====================================================================
// Demo_exp_exp_ox                 export obj


  printf("Demo_exp_exp_ox ox1-typ=%d ox1-form=%d dbi=%ld\n",
         ox1->typ,ox1->form,dbi);
  // DEB_dump_ox_s_ (ox1, "");
  DEB_dump_ox__ (ox1, "");


  return 0;

}


//================================================================
  int Demo_exp_test () {
//================================================================
 



  return 0;

}


//======================== EOF ======================
