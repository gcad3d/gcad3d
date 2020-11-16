//  DataBaseFile.                                2011-05-26     RF
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
2020-10-12 DBF_add__ can delete existing records. RF.
2011-05-26 Erstmalig erstellt. RF.

-----------------------------------------------------
*/
#ifdef globTag
 void DBF(){}
#endif
/*!
\file  ../ut/ut_dbf.c
\brief save retrieve key-value-Records in file 
\code
=====================================================
List_functions_start:

DBF_init             open DataBaseFile
DBF_exit             close DB
DBF_add__            add new or replace existing record
DBF_find__           find key; exact.
DBF_find_nxt         find key; wildcard. Test if key starts with findKey
DBF_getNxtKey        get key next record
DBF_getVal           get the value of the active record

DBF_dump             dump whole DBF

List_functions_end:
=====================================================
see INF_DBF
see test_DBF.c

\endcode *//*----------------------------------------


Save and retrieve key-value-Records in file (func DBF_add__())
Existing keys become replaced (old record overwritten).
Value can have size=0.
Max keySize = SizkDBF = 1024



File structure:
Record:
| recordHeader | key | value | mod4-padding-blanks |
|<-- 8 chars-->|<------------ sizTot ------------->|
              

recordHeader struct hdrDBF - size = 8 chars
| sizTot | sizKey | Filteyp | npb |

size-value = sizTot - sizKey - npb
npb =  nr of padding bytes



\endcode *//*==============================================================



Offline testen:
cc ut_dbf.c -DOFFLINE -lm&&./a.out
*/




#include <stdio.h>
#include <stdlib.h>    
#include <string.h>    

#include "../ut/ut_types.h"               // INT_8 - UINT_64
#include "../ut/ut_uti.h"       // UTI_div4up
#include "../xa/xa_msg.h"              // MSG_*


#define SizkDBF 1024            // max. keySize !


/// sizTot   size of total record, including complete header (8 bytes).
/// sizKey   size of key (is stored with padding blanks to a multiple of 4)
///          size=0: record deleted
/// ft       Filteyp; always 'D'
/// npb      nr of padding bytes
  typedef struct {int sizTot; short sizKey; char ft; char npb;}    hdrDBF;


  static FILE    *fDBF = NULL;
  static int     DBF_stat;          // 0=write;1=readHeader;2=readValue;-1=undef;
  static int     iDBF = -1;         // recordNr of active record;-1=undef; 0=first
  static int     nDBF = 0;          // Nr of records
  static hdrDBF  hDBF;              // header of the active record
  static char    kDBF[SizkDBF + 4];




//================================================================
  int DBF_init (char *fNam) {
//================================================================
/// DBF_init         open DataBaseFile

  // printf("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF \n");
  // printf("DBF_init |%s|\n",fNam);


  if ((fDBF = fopen (fNam, "w+b")) == NULL) {
    printf ("Fehler open Datei %s\n", fNam);
    return -1;
  }

  hDBF.ft = 'D';

  DBF_stat = -1;

  return 0;

}


//================================================================
  int DBF_exit () {
//================================================================
// DBF_exit                 close DB

  // printf("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF \n");
  // printf("DBF_exit \n");


  if(fDBF) fclose (fDBF);
  fDBF = NULL;

  return 0;

}


//======================================================================
  int DBF_add__ (char* key, int keySiz, char* val, int valSiz) {
//======================================================================
/// \code
/// DBF_add__          add or replace record
/// Input:
///   keySiz    strlen(key); must not exceed SizkDBF
/// Output:
///   RetCod:   -1 keySiz > SizkDBF
///             -2 no DBF_init
/// \endcode
    

  int    ir, ii, nPad;
  char   sPad0[] = "\0\0\0\0";


  // printf("------------------------------------------ \n");
  // printf("DBF_add__ |%s| keySiz=%d valSiz=%d\n",key,keySiz,valSiz);

      
  if((keySiz > SizkDBF)||(keySiz < 1)) {
    printf("***** ERROR DBF_add__ keySiz max %d\n",SizkDBF);
    return -1;
  }

  if(!fDBF) {
    printf("***** ERROR DBF_add__ no init\n");
    return -2;
  }


  // goto start
  rewind (fDBF);
  iDBF = -1;

  //----------------------------------------------------------------
  L_nxt_rec:
  ++iDBF;
    // printf(" ---- L_nxt_rec %d\n",iDBF);


  // read header
  ir = fread (&hDBF, sizeof(hDBF), 1, fDBF);
  if(ir < 1) goto L_add_1;  // EOF
  if(hDBF.ft != 'D') {
    printf("***** DBF_add__ E001 in rec %d\n",iDBF);
    return -1;
  }
    // printf(" iDBF=%d sizTot=%d sizKey=%d npb=%d\n",
             // iDBF,hDBF.sizTot,hDBF.sizKey,hDBF.npb);

  // test invalid record
  if(hDBF.sizKey == 0) goto L_skip_rec; // skip invalid record

  // test keySize
  if(keySiz != hDBF.sizKey) goto L_skip_rec;
    // skip record

  // read key into kDBF
  ir = fread (kDBF, hDBF.sizKey, 1, fDBF);
  if(ir < 1) {
    printf("***** DBF_add__ E002 in rec %d\n",iDBF);
    return -1;
  }

  // compare keys
  if(!strncmp (kDBF, key, keySiz)) {
      // printf(" invalidate-rec %d\n",iDBF);
    // key exists; set invalid
    // set back to start of header
    ii = -(sizeof(hdrDBF) + hDBF.sizKey);
    ir = fseek (fDBF, ii, 1);
    // invalidate key
    hDBF.sizKey = 0;
    if(ir) {
      printf("***** DBF_add__ E003 in rec %d\n",iDBF);
      return -1;
    }
    // wr header
    fwrite (&hDBF, sizeof(hDBF), 1, fDBF);
    // goto EOF
    fseek (fDBF, 0L, 2);
    DBF_stat = 0;
    goto L_add_1;
  }

  // skip value and padding-bytes
  ir = fseek (fDBF, hDBF.sizTot - hDBF.sizKey, 1);
  if(ir) {
    printf("***** DBF_add__ E004 in rec %d\n",iDBF);
    return -1;
  }
  goto L_nxt_rec;


  //----------------------------------------------------------------
  L_skip_rec:
    // skip record (key+value)
    ir = fseek (fDBF, hDBF.sizTot, 1);
    if(ir) {
      printf("***** DBF_add__ E005 in rec %d\n",iDBF);
      return -1;
    }
    goto L_nxt_rec;



  //================================================================
  L_add_1:
  // add record


  // keySiz must be a multiple of 4
  nPad = UTI_div4diff (keySiz + valSiz);
    // printf("   _add__ keySiz=%d valSiz=%d nPad=%d\n",keySiz,valSiz,nPad);

  hDBF.sizTot = keySiz + valSiz + nPad;
  hDBF.sizKey = keySiz;
  hDBF.ft     = 'D';
  hDBF.npb    = nPad;
    // printf("  sizTot=%d nPad=%d\n",hDBF.sizTot,hDBF.npb);

  // wr header
  fwrite (&hDBF, sizeof(hDBF), 1, fDBF);

  // wr key
  fwrite (key, keySiz, 1, fDBF);

  // wr val
  fwrite (val, valSiz, 1, fDBF);

  // wr nPad
  if(nPad) fwrite (sPad0, nPad, 1, fDBF);      // pad val

  ++nDBF;

  return 0;

}


/* Version add multiple
//======================================================================
  int DBF_add__ (char* key, int keySiz, char* val, int valSiz) {
//======================================================================
/// \code
/// DBF_add__          add record
/// Input:
///   keySiz    strlen(key); must not exceed SizkDBF
/// Output:
///   RetCod:   -1 keySiz > SizkDBF
///             -2 no DBF_init
/// \endcode


  int    nPad;
  char   sPad0[] = "\0\0\0\0";


  printf("DBF_add__ |%s| keySiz=%d valSiz=%d\n",key,keySiz,valSiz);


  if(keySiz > SizkDBF) {
    printf("***** ERROR DBF_add__ keySiz max %d\n",SizkDBF);
    return -1;
  }

  if(!fDBF) return -2;

  // goto eof
  fseek (fDBF, 0L, 2);
  DBF_stat = 0;

  // keySiz must be a multiple of 4
  nPad = UTI_div4diff (keySiz + valSiz);
    printf("DBF_add__ %d %d %d\n",keySiz,valSiz,nPad);

//   hDBF.rt = 0;   // 0=ctive record

  hDBF.sizTot = keySiz + valSiz + nPad;
  hDBF.sizKey = keySiz;
  hDBF.ft     = 'D';
  hDBF.npb    = nPad;
    printf("   nPad=%d sizTot=%d\n", hDBF.npb, hDBF.sizTot);

  // wr header
  fwrite (&hDBF, sizeof(hDBF), 1, fDBF);

  // wr key
  fwrite (key, keySiz, 1, fDBF);

  // wr val
  fwrite (val, valSiz, 1, fDBF);

  // wr nPad
  if(nPad) fwrite (sPad0, nPad, 1, fDBF);      // pad val

  ++nDBF;
 
  return 0;

}
*/

//================================================================
  int DBF_find__ (int *sizVal, char *key, int sizKey) {
//================================================================
/// \code
/// DBF_find__         find key; is active record if found
///   
/// Output:
///   sizVal     size of value; padded with max 3 0-bytes (to a multiple of 4)
/// RetCod: -1   key not found
///              else recordnumber of found record; is now the active record.
///              Get value with DBF_getVal
/// \endcode


  int   irc, il, iActRec, iLoop;
  char  *p1;


  // printf("DBF_find__ |%s| %d\n",key,iDBF);

  iActRec = iDBF;
  iLoop = 0;


  L_nxt_key:
    irc = DBF_getNxtKey (&p1, &il);
      // printf(" after DBF_getNxtKey %d %d\n",irc,il);
    if(irc < 0) return -1;


    // test if record already tested ..
    if(irc == iActRec) {
      ++iLoop;
      if(iLoop > 1) {   // starting second loop ..
        irc = -1;
        goto L_exit;
      }
    }

    if(il != sizKey) goto L_nxt_key;
    if(strcmp (p1, key)) goto L_nxt_key;

    *sizVal = hDBF.sizTot - hDBF.sizKey - hDBF.npb;


  //----------------------------------------------------------------
  L_exit:

    // printf("ex DBF_find__ irc=%d sizVal=%d iDBF=%d\n",irc,*sizVal,iDBF);

  return irc;

}


//=====================================================================
  int DBF_find_nxt (char **foundKey, int *sizFoundKey, int *sizVal,
                    char *findKey, int sizKey, int *iStart) {
//=====================================================================
/// \code
/// DBF_find_nxt         find key; wildcard. Test if key starts with findKey
/// Input:
///   iStart     must be set to 0 at start; do not modify.
/// RetCod: -1   key not found
///         -2   file not found
///         >=0  recordnumber of found record; is now the active record.
///
/// Example:
///  // find all records starting with "abc"
///  ii = 0;
///  while (DBF_find_nxt (1, "abc", 3, &ii) >= 0) {
///    i1 = 240; DBF_getVal ((void*)s2, &i1);           // get value
///  }
/// \endcode

  static int iActRec, iLoop;

  int   ir, il;
  char  *p1;


  // printf("fffffffffffffffffffffffffffffffff \n");
  // printf("DBF_find_nxt |%s| siz=%d iStart=%d iDBF=%d stat=%d\n",
         // findKey,sizKey,*iStart,iDBF,DBF_stat);



  L_nxt_key:
    *iStart += 1;
    ir = DBF_getNxtKey (&p1, &il);
      // printf(" after DBF_getNxtKey %d %d %d\n",ir,il,iActRec);
    L_nxt_1:
    if(ir < 0) return -2;


    if(*iStart == 1) {
      iActRec = iDBF;


    } else {
      // test if record already tested ..
      if(ir == iActRec) {
        ir = -1;
        goto L_exit;
      }
    }


    if(il < sizKey) goto L_nxt_key;
    if(strncmp (p1, findKey, sizKey)) goto L_nxt_key;

   *foundKey = p1;
   *sizFoundKey = il;
   *sizVal = hDBF.sizTot - hDBF.sizKey - hDBF.npb;


  //----------------------------------------------------------------
  // found it ..
  L_exit:

    // printf("ex DBF_find_nxt %d %d\n",ir,iDBF);

  return ir;

}


//================================================================
  int DBF_getNxtKey (char **key, int *sizKey) {
//================================================================
/// \code
/// read next key (and record-header)
/// Output:
///   iDBF    nr of found record; -1=none.
///   key     a pointer to key
///   sizKey  size of key in bytes
///   RetCod  recordnumber; first = 0
///           -1  file empty or damaged
/// \endcode


  int    keySiz, ir;
  // int    irun;
  long   lv;


  // printf("DBF_getNxtKey iDBF=%d stat=%d\n",iDBF,DBF_stat);

  // irun = 0;


  if(!fDBF) {
    // printf("***** DBF_getNxtKey E000\n");
    return -1;
  }


  if(DBF_stat < 0) {
    // read-pos undef.
    rewind (fDBF);
    DBF_stat = 0;   // 0 = active position is at start of header
    iDBF = -1;
  }



  //----------------------------------------------------------------
  // skip value of active record ...
  if(DBF_stat == 0) goto L_nxt;
  // position is at start of value; skip value
  lv = hDBF.sizTot - hDBF.sizKey;
    // printf(" getNxtKey-skip %ld\n",lv);
  if(lv) {
    ir = fseek (fDBF, lv, 1);
    if(ir) {
      printf("***** DBF_getNxtKey E001 in rec %d\n",iDBF);
    }
  }



  //----------------------------------------------------------------
  // position now is nxt hdr.
  // read hdr #iDBF into hDBF
  L_nxt:
  ++iDBF;
  ir = fread (&hDBF, sizeof(hDBF), 1, fDBF);
  if(ir < 1) {   // normal EOF
    rewind (fDBF);
    iDBF = 0;

    // read again from start of file
    ir = fread (&hDBF, sizeof(hDBF), 1, fDBF);
    if(ir < 1) {
      printf("***** DBF_getNxtKey E002\n");  // empty file
      return -1;
    }
  }

  if(hDBF.ft != 'D') {
    printf("***** DBF_getNxtKey E003 in rec %d\n",iDBF);
    return -1;
  }
    // printf(" _getNxtKey typ=%d sizTot=%d sizKey=%d\n",
            // hDBF.rt,hDBF.sizTot,hDBF.sizKey);



  //----------------------------------------------------------------
  // skip deleted record
  if(!hDBF.sizKey) {
    // skip key value padbytes
    ir = fseek (fDBF, hDBF.sizTot, 1);
    if(ir) {
      printf("***** DBF_getNxtKey E004 in rec %d\n",iDBF);
    }
    goto L_nxt;
  }


  //----------------------------------------------------------------
  // copy key into kDBF; positon now is value.
  *sizKey = hDBF.sizKey;
  *key    = kDBF;            // pointer to key
  ir = fread (kDBF, *sizKey, 1, fDBF);
  if(ir < 1) {
    printf("***** DBF_getNxtKey E005 in rec %d\n",iDBF);
    return -1;
  }
  // terminate
  kDBF[*sizKey] = '\0';

  DBF_stat = 1;    // // 1 = active position is at start of value

    // TESTBLOCK
    // printf("ex DBF_getNxtKey iDBF=%d sizKey=%d key=|%s|\n",iDBF,
           // *sizKey,*key);
    // END TESTBLOCK

  return iDBF;

}


//================================================================
  int DBF_getVal (void *val, int *sizVal) {
//================================================================
/// \code
/// DBF_getVal               get the value of the active record
///   READ ONLY ONCE !
/// Input:
///   sizVal   size of array val in bytes
/// Output:
///   val      the value of the active record
///   sizVal   size of value; padded with max 3 0-bytes (to a multiple of 4)
/// \endcode


  int    keySiz, valSiz, ir;


  // printf("DBF_getVal \n");


  if(DBF_stat != 1) {
    printf("***** DBF_getVal E003 - no value present\n");
    return -2;
  }

  valSiz = hDBF.sizTot - hDBF.sizKey - hDBF.npb;
    // printf("  valSiz=%d sizVal=%d\n",valSiz,*sizVal);


  if(*sizVal < valSiz) {
    printf("***** DBF_getVal E001 - buffer too small\n");  // empty file
    return -1;
  }

  DBF_stat = 0;

  if(valSiz) {
    ir = fread (val, valSiz, 1, fDBF);
    if(ir < 1) {
      rewind (fDBF);
      printf("***** DBF_getVal ERR-rv\n");
      return -1;
    }
  } else {
    ir = 0;
    memcpy(val, &ir, 1);
  }

  *sizVal = valSiz;

  // skip the padding
  if(hDBF.npb) {
    ir = fseek (fDBF, hDBF.npb, 1);
    if(ir) {
      rewind (fDBF);
      printf("***** DBF_getVal ERR-rp\n");
      return -1;
    }
  }

    // printf("ex DBF_getVal siz=%d\n",*sizVal);
    // printf(" val=|%s|\n",(char*)val);
    // DEB_dump_ox__ (val);

  return 0;

}


//================================================================
  int DBF_dump () {
//================================================================
/// DBF_dump         dump whole DBF


  int    iNr, keySiz, ir;
  long   lv;


  printf("DBF_dump --------------------------------\n");
  if(!fDBF) {printf(" empty ..\n"); return -1;}


  rewind (fDBF);
  iNr = 0;


  //----------------------------------------------------------------
  // get nxt hdr
  L_nxt:
  ir = fread (&hDBF, sizeof(hDBF), 1, fDBF); 
  if(ir < 1) goto L_exit;    // normal EOF

  if(hDBF.ft != 'D') {
    printf("***** DBF_dump E001 in rec %d\n",iNr);
    printf (" sizTot=%d sizKey=%d npb=%d\n",hDBF.sizTot,hDBF.sizKey,hDBF.npb);
    return -1;
  }

  // skip deleted record
  if(!hDBF.sizKey) {
    printf("%d deleted -\n",iNr);
    // skip record
    lv = hDBF.sizTot;
    goto L_skip;
  }


  //----------------------------------------------------------------
  // read key
  keySiz = hDBF.sizKey;
  ir = fread (&kDBF, keySiz, 1, fDBF); 
  if(ir < 1) {
    printf("***** DBF_dump E002 in rec %d\n",iNr);
    return -1;
  }
  kDBF[keySiz] = '\0';    // terminate
  printf("%d totSiz=%d keySiz=%d key=|%s|\n",iNr,hDBF.sizTot,keySiz,kDBF);


  // skip value and padding bytes
  lv = hDBF.sizTot - hDBF.sizKey;


  //----------------------------------------------------------------
  // skip lv bytes
  L_skip:
  ir = fseek (fDBF, lv, 1); 
  if(ir) {
    printf("***** DBF_dump E004 in rec %d\n",iNr);
  }


  ++iNr;
  goto L_nxt;


  //----------------------------------------------------------------
  L_exit:

  L_eof:
  printf("DBF_dump end ----------------------------\n");
  return 0;

}


//====================== EOF ===========================
