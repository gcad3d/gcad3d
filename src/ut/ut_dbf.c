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
DBF_add__            add a new record
// DBF_add_uniq      test if key already exists;
DBF_find_key         find key; exact.
DBF_find_nxt         find key; wildcard. Test if key starts with findKey
DBF_find__           find key
DBF_getNxtKey        get key next record
DBF_getVal           get the value of the active record
// DBF_mod           modify active record

DBF_dump             dump whole DBF

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


Save and retrieve key-value-Records in file.
No multiple keys.
Value can have size=0.
Max keySize = SizkDBF = 1024



File structure:
Record:
| recordHeader | key | value |

recordHeader (struct hdrDBF):
| FT | RT | sizTot | sizKey |


\endcode *//*==============================================================



Offline testen:
cc ut_dbf.c -DOFFLINE -lm&&./a.out
*/




#include <stdio.h>
#include <stdlib.h>    
#include <string.h>    


#define SizkDBF 1024            // max. keySize !


/// ft       Filteyp; always 'D'   test-data
/// rt       Recordtyp; 0     active record,   yet unused
/// sizTot   size of total record, including complete header (8 bytes).
/// sizKey   size of key (is stored with padding blanks to a multiple of 4)
  typedef struct {char ft; char rt; int sizTot; short sizKey;}    hdrDBF;


  static FILE    *fDBF = NULL;
  static int     DBF_stat;          // 0=write;1=readHeader;2=readValue.
  static hdrDBF  hDBF;              // header of the active record
  static int     iDBF = -1;         // recordNr of active record
  static int     nDBF = 0;          // Nr of records
  static char    kDBF[SizkDBF + 4];




//================================================================
  int DBF_init (char *fNam) {
//================================================================
/// DBF_init         open DataBaseFile


  printf("DBF_init |%s|\n",fNam);


  if ((fDBF = fopen (fNam, "w+b")) == NULL) {
    printf ("Fehler open Datei %s\n", fNam);
    return -1;
  }

  hDBF.ft = 'D';

  DBF_stat = -1;

  return 0;

}


//======================================================================
  int DBF_add__ (char* key, int keySiz, char* val, int valSiz) {
//======================================================================
/// \code
/// DBF_add__          add a new record
/// keySiz      must not exceed SizkDBF
/// RetCod:   -1 keySiz > SizkDBF
///           -2 no DBF_init
/// \endcode


  int    keyPad, valPad;
  char   sPadb[] = "    ";
  char   sPad0[] = "\0\0\0\0";




  printf("DBF_add__ |%s| keySiz=%d valSiz=%d\n",key,keySiz,valSiz);



  if(keySiz > SizkDBF) {
    printf("***** ERROR DBF_add__ keySiz max %d\n",SizkDBF);
    return -1;
  }


  if(!fDBF) return -2;


  // goto eof
  if(DBF_stat != 0) {
    fseek (fDBF, 0L, 2);
    DBF_stat = 0;
  }


  // keySiz must be a multiple of 4
  keyPad = UTI_div4diff (keySiz);


  // valSiz must be a multiple of 4
  valPad = UTI_div4diff (valSiz);

  // printf("DBF_add__ %d %d %d %d\n",keySiz,keyPad,valSiz,valPad);


  hDBF.rt = 0;   // 0=ctive record

  hDBF.sizTot = keySiz + keyPad + valSiz + valPad + 8;
  hDBF.sizKey = keySiz;
    // printf("     tot=%d key=%d\n",hDBF.sizTot,hDBF.sizKey);

  fwrite (&hDBF, sizeof(hDBF), 1, fDBF);

  fwrite (key, keySiz, 1, fDBF);

  if(keyPad) fwrite (sPadb, keyPad, 1, fDBF);       // pad key

  fwrite (val, valSiz, 1, fDBF);

  if(valPad) fwrite (sPad0, valPad, 1, fDBF);      // pad val

  ++nDBF;

  return 0;

}


//================================================================
  int DBF_find__ (int *sizVal, char *key, int sizKey) {
//================================================================
/// \code
/// DBF_find__         find key; is active record if found
/// Output:
///   sizVal     size of value; padded with max 3 0-bytes (to a multiple of 4)
/// RetCod: -1   key not found
///              else recordnumber of found record; is now the active record.
///              Get value with DBF_getVal
/// \endcode


  int   irc, il, iActRec, iLoop;
  char  *p1;


  printf("DBF_find__ |%s| %d\n",key,iDBF);

  iActRec = iDBF;
  iLoop = 0;

/*
  if(iDBF < 0) {
    iActRec = 0;
    rewind (fDBF);  // for MS necessary !!
  } else {
    iActRec = iDBF;
  }
  iLoop = 0;
*/


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

    *sizVal = hDBF.sizTot - sizKey - 8;

  L_exit:

    printf("ex DBF_find__ irc=%d sizVal=%d iDBF=%d\n",irc,*sizVal,iDBF);

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
   *sizVal = hDBF.sizTot - sizKey - 8;


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
///   key     a pointer to key
///   sizKey  size of key in bytes
///   RetCod  recordnumber; first = 0
///           -1  file empty or damaged
/// \endcode


  int    keySiz, ir;
  // int    irun;
  long   lo;


  // printf("DBF_getNxtKey iDBF=%d stat=%d\n",iDBF,DBF_stat);

  // irun = 0;


  if(!fDBF) {
    // printf("***** DBF_getNxtKey E000\n");
    return -1;
  }


  if(DBF_stat < 1) {
    rewind (fDBF);
    iDBF = -1;
  }
  DBF_stat = 1;



  //----------------------------------------------------------------
  // skip value of active record ...
  if(iDBF < 0) goto L_nxt;
  if(!hDBF.sizTot) goto L_nxt;            // value already read   
  keySiz = UTI_div4up (hDBF.sizKey);      // add padBytes
  lo = hDBF.sizTot - keySiz - 8;
  ir = fseek (fDBF, lo, 1);
  if(ir) {
    printf("***** DBF_getNxtKey E001 in rec %d\n",iDBF);
  }



  //----------------------------------------------------------------
  // get nxt hdr
  L_nxt:
  ++iDBF;
  ir = fread (&hDBF, sizeof(hDBF), 1, fDBF);
  if(ir < 1) {   // normal EOF
    rewind (fDBF);
    iDBF = 0;
    // ++irun;
      // printf(" _getNxtKey rewind %d\n",irun);

    ir = fread (&hDBF, sizeof(hDBF), 1, fDBF);
    if(ir < 1) {
      printf("***** DBF_getNxtKey E004\n");  // empty file
      return -1;
    }
  }

  if(hDBF.ft != 'D') {
    printf("***** DBF_getNxtKey E002 in rec %d\n",iDBF);
    return -1;
  }
    // printf(" _getNxtKey typ=%d sizTot=%d sizKey=%d\n",
            // hDBF.rt,hDBF.sizTot,hDBF.sizKey);



  //----------------------------------------------------------------
  // get key
  keySiz = UTI_div4up (hDBF.sizKey);    // add pad-bytes
  ir = fread (kDBF, keySiz, 1, fDBF);
  if(ir < 1) {
    printf("***** DBF_getNxtKey E003 in rec %d\n",iDBF);
    return -1;
  }
  // remove pad-bytes
  kDBF[hDBF.sizKey] = '\0';
    // printf(" ir=%d keySiz=%d |%-16s|\n",ir,keySiz,kDBF);




  *sizKey = hDBF.sizKey;     // without padBytes
  *key    = kDBF;            // pointer to key

    // printf("ex DBF_getNxtKey iDBF=%d typ=%d siz=%d key=|%s|\n",iDBF,
           // hDBF.rt,*sizKey,*key);

  return iDBF;

}


//================================================================
  int DBF_getVal (void *val, int *sizVal) {
//================================================================
/// \code
/// get the value of the active record
/// Input:
///   sizVal   size of array val in bytes
/// Output:
///   val      the value of the active record
///   sizVal   size of value; padded with max 3 0-bytes (to a multiple of 4)
/// \endcode


  int    keySiz, valSiz, ir;


  if(DBF_stat != 1) {
    printf("***** DBF_getVal E003 - no value present\n");
    return -2;
  }
  DBF_stat = 2;

  keySiz = UTI_div4up (hDBF.sizKey);
  valSiz = hDBF.sizTot - keySiz - 8;
    // printf(" keySiz=%d valSiz=%d sizVal=%d\n",keySiz,valSiz,*sizVal);


  if(*sizVal < valSiz) {
    printf("***** DBF_getVal E001 - buffer too small\n");  // empty file
    return -1;
  }

  ir = fread (val, valSiz, 1, fDBF);
  if(ir < 1) {   // normal EOF
    rewind (fDBF);
    printf("***** DBF_getVal E002\n");  // empty file
    return -1;
  }


  // ((char*)val)[valSiz] = '\0';          // remove padding

  *sizVal = valSiz;


	hDBF.sizTot = 0;                      // value already read   

    // printf("ex DBF_getVal siz=%d\n",*sizVal);
    // printf(" val=|%s|\n",(char*)val);
    // DEB_dump_ox__ (val);

  return 0;

}


/*
//================================================================
  int DBF_mod () {
//================================================================


  return 0;

}
*/

//================================================================
  int DBF_dump () {
//================================================================
/// DBF_dump         dump whole DBF


  int    iNr, keySiz, ir;
  long   lo;


  printf("DBF_dump --------------------------------\n");
  if(!fDBF) {printf(" empty ..\n"); return -1;}


  rewind (fDBF);
  iNr = 0;


  //----------------------------------------------------------------
  // get nxt hdr
  L_nxt:
  ir = fread (&hDBF, sizeof(hDBF), 1, fDBF); 
  if(ir < 1) {   // normal EOF
    rewind (fDBF);
    iDBF = -1;
    DBF_stat = -1;
    goto L_eof;
  }


  if(hDBF.ft != 'D') {
    printf("***** DBF_dump E001 in rec %d\n",iNr);
    return -1;
  }
  // printf (" typ=%d sizTot=%d sizKey=%d\n",hDBF.rt,hDBF.sizTot,hDBF.sizKey);


  //----------------------------------------------------------------
  // read key
  keySiz = UTI_div4up (hDBF.sizKey);
  ir = fread (&kDBF, keySiz, 1, fDBF); 
  if(ir < 1) {
    printf("***** DBF_dump E002 in rec %d\n",iNr);
    return -1;
  }
  kDBF[hDBF.sizKey] = '\0';    // remove pad-bytes
  printf("%d totSiz=%d keySiz=%d key=|%s|\n",iNr,hDBF.sizTot,keySiz,kDBF);


  //----------------------------------------------------------------
  // skip value
  lo = hDBF.sizTot - keySiz - 8;
  ir = fseek (fDBF, lo, 1); 
  if(ir) {
    printf("***** DBF_dump E004 in rec %d\n",iNr);
  }


  ++iNr;
  goto L_nxt;
  //----------------------------------------------------------------


  L_eof:
  printf("DBF_dump end ----------------------------\n");
  return 0;

}



//========================================================
//========================================================
#ifdef OFFLINE


int UTI_div4up (int ii) {
  int id;
  id = 4 - ii % 4;
  if(id < 4) ii += id;
  return ii;
}


int UTI_div4diff (int ii) {
  int id;
  ii %= 4;
  if(ii) ii = 4 - ii;
  return ii;
}


//============================
  int main (int paranz, char *params[]) {
//============================

  int     i1, i2, ii, lv;
  char    *p1, c1, s1[256], s2[256], *p2;

  DBF_init ("export");

  strcpy(s1, "key-012");
  DBF_find__ (&lv, s1, strlen(s1));


  strcpy(s1, "abc");
  strcpy(s2, "");
  DBF_add__ (s1, strlen(s1),  s2, strlen(s2));

  strcpy(s1, "1");
  strcpy(s2, "v2");
  DBF_add__ (s1, strlen(s1),  s2, strlen(s2));

  strcpy(s1, "ke2");
  strcpy(s2, "val2");
  DBF_add__ (s1, strlen(s1),  s2, strlen(s2));

  strcpy(s1, "key-5");
  strcpy(s2, "value2");
  DBF_add__ (s1, strlen(s1),  s2, strlen(s2));

  strcpy(s1, "key-002");
  strcpy(s2, "value-012");
  DBF_add__ (s1, strlen(s1),  s2, strlen(s2));


  DBF_dump ();


  strcpy(s1, "key-003");
  strcpy(s2, "value-03");
  DBF_add__ (s1, strlen(s1),  s2, strlen(s2));


  DBF_dump ();


  strcpy(s1, "key-002");
  i1 = DBF_find__ (&lv, s1, strlen(s1));
  i1 = 240; DBF_getVal ((void*)s1, &i1);


  // find all records starting with "key"
  ii = 0;
  strcpy(s1, "key");
  while (DBF_find_nxt (&p1, &i1, &i2, s1, strlen(s1), &ii) >= 0) {
    i1 = 240; DBF_getVal ((void*)s2, &i1);
  }


  strcpy(s1, "key");
  i1 = DBF_find__ (&lv, s1, strlen(s1));
  i1 = 240; DBF_getVal ((void*)s1, &i1);



  DBF_getNxtKey (&p1, &i1);
  DBF_getNxtKey (&p1, &i1);
  i1 = 240; DBF_getVal ((void*)s1, &i1);


  DBF_getNxtKey (&p1, &i1);
  i1 = 240; DBF_getVal ((void*)s1, &i1);

  DBF_getNxtKey (&p1, &i1);
  DBF_getNxtKey (&p1, &i1);
  DBF_getNxtKey (&p1, &i1);


  return 0;
}

#endif
//====================== EOF ===========================
