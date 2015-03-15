//     ut_mem.c                            Reiter Franz
/*
 *
 * Copyright (C) 2015 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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
2004-12-15 uebernommen aus ut_geo.c               RF

-----------------------------------------------------
*/
#ifdef globTag
 void MEM(){}
#endif
/*!
\file  ../ut/ut_mem.c
\brief Memory manipulations: swap, insert, delete, invert .. 
\code
=====================================================
List_functions_start:

MEM_swap_short
MEM_swap_int              2 int vertauschen
MEM_swap_2db              2 doubles vertauschen                          INLINE
MEM_swap_2vp              swap 2 pointers                                INLINE
MEM_swap_2bit             swap 2 bits
MEM_swap__                2 structs vertauschen

MEM_ptr_mov               move a point <dist> bytes                      INLINE

MEM_chg_str               change a part of a string in mem
MEM_ins_str               insert s2 into s1 (s1 = s2 + s1)
MEM_ins_rec               insert data into mem
MEM_ins_IndRec            insert one integer into intArray
MEM_ins_nrec              insert dataRecord(s) into table
MEM_chg_rec               change a record in mem (change line; delete+insert)
MEM_del_str               delete n chars out of string
MEM_del_IndRec            delete 1 integer from intArray
MEM_del_DbRec             delete 1 double in double-Array
MEM_del_pt                delete point in Point-Array
MEM_del_nrec              delete <delRecNr> records from table
MEM_inv_itab              invert table of intData; 1/2/3 --> 3/2/1
MEM_inv_dtab              invert table of dbData; 1/2/3 --> 3/2/1
MEM_inv_rtab              invert table of dataRecords; 1/2/3 --> 3/2/1

MEM_copy_tab              copy <recNr> records of size <sizRec>
MEM_copy_oTab             copy <recNr> records of size <sizRec>, set pointers
MEM_set_1recn             write record <recNr> times

MEM_cmp__                 compare memoryspaces (n bytes)

MEM_wri_spc               write n chars from mem -> file
MEM_get_file              read file -> mem

List_functions_end:
=====================================================
- see also:
UME_alloc_tmp             get temp-space
UTX_memstr
UTX_memFind
UTX_memFind1
UTX_memcmp
memchr
memcmp
memccpy
memcpy
memmove
memset
UTA_reallTab

\endcode *//*----------------------------------------

*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_geo.h"       // MEM_swap_int



//==========================================================================
  int MEM_chg_str (char *datStart, long maxSiz, long delSiz, char *insDat) {
//==========================================================================
/// \code
/// MEM_chg_str          change a part of a string in mem 
/// 
/// Input:
///   datStart     Startposition of string
///   maxSiz       max size of string starting at datStart
///   delSiz       how much bytes to delete at Pos datStart
///   insDat       data to insert (null-terminated) at datStart
/// 
///  strcpy(cbuf, "1234567890");
///  cp1 = strchr(cbuf, '5');
///  il = sizeof(cbuf) - (cp1 - cbuf);
///  MEM_chg_str (cp1, &il, 1, "AAA"); // 5 -> AAA
/// 
/// see also MEM_chg_rec
/// \endcode

  long  movSiz, insSiz, datSiz;
  char  *newPos, *movPos;


  // printf("MEM_chg_str maxSiz=%ld delSiz=%ld\n",maxSiz,delSiz);
  // printf("  datStart=|%s|\n",datStart);
  // printf("  insDat=|%s|\n",insDat);


  insSiz = strlen(insDat);
  datSiz = strlen(datStart);
    // printf(" insSiz=%ld, datSiz=%ld\n",insSiz,datSiz);

  if((datSiz - delSiz + insSiz) > maxSiz) {
    TX_Print("MEM_chg_str E001");
    return -1;
  }

  movPos = (char*)datStart + delSiz;   // von wo weg

  movSiz = datSiz - delSiz + 1;   // +1 = add \0 !!!
  // printf(" movSiz=%ld\n",movSiz);

  newPos = (char*)datStart + insSiz;   // wohin

  // den hinteren Teil der Daten auseinanderschieben
  //       dest    src     siz
  memmove(newPos, movPos, movSiz);

  // in das entstandene Loch hineinkopieren
  //       dest   src      siz
  memcpy(datStart, insDat, insSiz);

    // printf("ex MEM_chg_rec |%s|\n",datStart);

  return 0;

}


//================================================================
   int MEM_del_str (char *cp1, int sizDel) {
//================================================================
// delete n chars out of string cp1 starting at pos cp1
// Input:
//   cp1     startpos of string = startpos where to deleten
//   ls      nr of characters to delete
// Output:
//   cp1

  int   sizTot, movSiz;
  char  *oldPos;


  // printf("MEM_del_str %d |%s|\n",sizDel,cp1);

  sizTot = strlen (cp1);
    // printf(" sizTot=%d\n",sizTot);

  if((sizTot < sizDel)||(sizDel < 1)) {
    TX_Error("MEM_del_str E001\n");
    return -1;
  }


  movSiz = sizTot - sizDel + 1;  // + '\0'

  oldPos = cp1 + sizDel;

  // memcpy (cp1, oldPos, movSiz);
  memmove (cp1, oldPos, movSiz);

  // printf("ex MEM_del_str |%s|\n",cp1);

  return 0;

}


//================================================================
   int MEM_ins_str (int *s2Siz, char *s1, char *s2, int maxlen) {
//================================================================
// insert s2 into s1 (s1 = s2 + s1)
//
// Input:
//   s2      string to insert into s1
//   maxlen  max size of s1
// Output:
//   insSiz  startpos of string = startpos where to deleten
//   s1      s1 = s2 + s1
//   RetCod:  0   OK
//           -1   s1 too small
             

  int   s1Siz, irc=0;
  char  *oldPos;


  // printf("MEM_ins_str %d |%s|%s|\n",maxlen,s1,s2);

  *s2Siz = strlen (s2);
  s1Siz = strlen (s1) + 1;        // add '\0'

  if(*s2Siz + s1Siz >= maxlen) {
    // TX_Error("MEM_ins_str E001\n");
    TX_Print("MEM_ins_str E001\n");
    if(*s2Siz >= maxlen) return -2;
    s1Siz = maxlen - *s2Siz - 1;
    irc = -1;
  }


  memmove (&s1[*s2Siz], s1, s1Siz);
  memcpy (s1, s2, *s2Siz);

  // printf("ex MEM_ins_str %d |%s|\n",*s2Siz,s1);

  return 0;

}

//======================================================================
  int MEM_get_file (char* txbuf, long *fSiz, char *fnam) {
//======================================================================
/// \code
/// MEM_get_file              read file -> mem
/// Input:
///   fSiz       must have exact filesize; see OS_FilSiz.
///   fnam       file to copy -> txbuf
/// Output:
///   txbuf      must have size *fSiz+1
/// See UTX_str_file()
/// \endcode

  FILE  *fpi;

  // printf("MEM_get_file %ld |%s|\n",*fSiz,fnam);
  // {char s1[256];sprintf(s1,"type \"%s\"",fnam);system(s1);}

  // ACHTUNG MS: liest Mist wenn open nicht binary !
  if ((fpi = fopen (fnam, "rb")) == NULL) {
    TX_Error ("MEM_get_file: Error open %s",fnam);
    *fSiz = -2;
    return -1;
  }

  fread (txbuf, *fSiz, 1, fpi);

  txbuf[*fSiz] = '\0';

  fclose(fpi);

    // printf(" _get_file|%s|\n",txbuf);

  return 0;

}


//================================================================
  int MEM_wri_spc (char *fnam, char *data, long cNr) {
//================================================================
/// MEM_wri_spc               write n chars from mem -> file

  FILE   *fpo;


  printf("MEM_wri_spc %ld |%s|\n",cNr,fnam);


  if((fpo=fopen(fnam,"wb")) == NULL) {
    return -1;
  }

  fwrite (data, 1, cNr, fpo);

  fclose(fpo);


  return 0;

}


/*
//=======================================================================
  int MEM_swap_int (int *i1, int *i2) {
//=======================================================================
// MEM_swap_int              2 int vertauschen

  int  i3;

  i3  = *i1;
  *i1 = *i2;
  *i2 = i3;

  return 0;

}


//=======================================================================
  int MEM_swap_2db (double *d1, double *d2) {
//=======================================================================
// MEM_swap_2db              2 doubles vertauschen

  double  d3;

  d3  = *d1;
  *d1 = *d2;
  *d2 = d3;

  return 0;

}
*/


//=======================================================================
  int MEM_swap__ (void *stru1, void* stru2, long strSiz) {
//=======================================================================
/// \code
/// MEM_swap__              2 structs vertauschen
/// 
/// MEM_swap__(&p1, &p2, sizeof(Point));
/// \endcode


  char  cbuf[OBJ_SIZ_MAX];
  void  *tmpSpc;


  if(strSiz > OBJ_SIZ_MAX) {
    tmpSpc = UME_alloc_tmp (strSiz);
    memcpy (tmpSpc, stru1, strSiz);
    memcpy (stru1, stru2, strSiz);
    memcpy (stru2, tmpSpc, strSiz);

  } else {
    memcpy ((void*)cbuf, stru1, strSiz);
    memcpy (stru1, stru2, strSiz);
    memcpy (stru2, (void*)cbuf, strSiz);
  }

  return 0;

}


//================================================================
  int MEM_swap_2bit (int *ii, int ib1, int ib2) {
//================================================================
/// ib muessen Values sein (1, 2, 4, 8 usw.)

  int    i1, i2;


  // filtern
  i1 = *ii & ib1;
  i2 = *ii & ib2;


  // setzen
  if(i1 > 0) {
    *ii |= ib2;    // set
  } else {
    *ii &= ~ib2;   // clear
  }

  if(i2 > 0) {
    *ii |= ib1;    // set
  } else {
    *ii &= ~ib1;   // clear
  }

  return 0;

}


//===================================================================
  int MEM_ins_IndRec (int *iTab, int *recNr, int recPos, int reVal) {
//===================================================================
/// \code
/// insert one new record into indexTable at position recPos.
/// Beispiel:
///   In:   recNr=3; iTab=0,1,2; recPos=0; reVal=3;
///   Out:  recNr=4; Tab=3,0,1,2;
/// \endcode

  int   i1, irc;


  // printf("MEM_ins_IndRec %d\n",*recNr);
  // for(i1=0; i1<*recNr; ++i1) printf(" %d,",iTab[i1]);
  // printf("\n");


  irc = MEM_ins_nrec (recNr, iTab, recPos, &reVal, 1, sizeof(int));


  // printf("MEM_ins_IndRec %d\n",*recNr);
  // for(i1=0; i1<*recNr; ++i1) printf(" %d,",iTab[i1]);
  // printf("\n");


  return irc;

}


//==========================================================================
  int MEM_ins_rec (void *insPos, long movSiz, void *insDat, long insSiz) {
//==========================================================================
/// \code
/// MEM_ins_rec          insert data into mem
/// 
/// insDat data to insert
/// insSiz size of insDat
/// insPos where to insert
/// movSiz how much bytes to move from insPos to insPos+insSiz
/// \endcode

  char  *newPos;


  newPos = (char*)insPos + insSiz;

  // den hinteren Teil der Daten auseinanderschieben
  //       dest           src     siz
  memmove(newPos, insPos, movSiz);


  // in das entstandene Loch hineinkopieren
  //       dest   src      siz
  memcpy(insPos, insDat, insSiz);


  return 0;

}

//==========================================================================
  int MEM_ins_nrec (int *recNr, void *recTab,
                   int ipos, void *insDat, int insRecNr, int sizRec) {
//==========================================================================
/// \code
/// insert records into table.
/// insert data BEFORE Record with index ipos.
/// insert dataBlock insDat BEFORE dataRecord Nr ipos into Table recTab
/// recTab consists of *recNr records of size sizRec
/// 
/// Input/Output:
/// *recNr   nr of records of table recTab;
///          after exit recNr += insRecNr
/// recTab   table with *recNr records; each with a size of sizRec bytes.
///          after exit record nr ipos is the firsdt record of insDat ..
/// 
/// Input:
/// ipos     index in recTab of the first inserted record of insDat
/// insDat   data to insert; size = sizRec * insRecNr
/// insRecNr nr of records to insert
/// sizRec   size of records (of recTab and insDat)
/// 
/// Example insert 1 point p31 into 3. position (index nr 2) of pTab:
/// MEM_ins_nrec (&recNr, pTab, 2, &p31, 1, sizeof(Point));
/// \endcode


  int   movSiz, insSiz;
  char  *insPos;


  // printf("MEM_ins_nrec %d %d\n",*recNr,ipos);


  if((ipos > *recNr)||(ipos < 0)) {
    TX_Error("MEM_ins_nrec E001 %d %d\n",*recNr,ipos);
    return -1;
  }


  insPos = (char*)recTab + (ipos * sizRec);
  movSiz = (*recNr  - ipos) * sizRec;

  *recNr += insRecNr;

  return MEM_ins_rec (insPos, movSiz, insDat, sizRec*insRecNr);

}


//==========================================================================
  int MEM_chg_rec (void *datStart, long *datSiz,
                   void *insDat,   long insSiz,
                   void *delPos,   long delSiz) {
//==========================================================================
/// \code
/// MEM_chg_rec          change a record in mem (change line)
/// 
/// Input:
///   datStart     Startposition des Datenblock
///   datSiz       Size Datenblock (zB ex strlen)
///   insDat       data to insert (len = insSiz)
///   insSiz       size of insDat
///   delPos       Position of old record (delete) and new record (insert)
///   delSiz       how much bytes to delete at Pos delPos
/// Output:
///   datSiz  wird korrigiert
/// 
/// 
///  strcpy(cbuf, "1234567890");   l1 = strlen(cbuf);
///  MEM_chg_rec (cbuf, &l1, "AAA", 3, &cbuf[4], 1); // 5 -> AAA
/// 
/// strcpy(cbuf, "1234567890");   l1 = strlen(cbuf);
/// MEM_chg_rec (cbuf, &l1, "A", 1, &cbuf[4], 3);   // 567 -> A
///
/// see also MEM_chg_str
/// \endcode



// movSiz  Laenge der Daten die (hinterher) hinter insRec liegt


  long  movSiz;
  char  *newPos, *movPos;


  // printf("MEM_chg_rec siz=%d ins=%d del=%d\n",*datSiz,insSiz,delSiz);

  // printf(" del=|");  UTX_dump_c__ (delPos, delSiz); printf("|\n");
  // printf(" ins=|");  UTX_dump_c__ (insDat, insSiz); printf("|\n");


  movPos = (char*)delPos + delSiz;   // von wo weg

  movSiz = (char*)datStart + *datSiz - movPos + 1;   // +1 = add \0 !!!
  // printf(" movSiz=%d\n",movSiz);

  newPos = (char*)delPos + insSiz;   // wohin

  *datSiz += insSiz - delSiz;

  // den hinteren Teil der Daten auseinanderschieben
  //       dest    src     siz
  memmove(newPos, movPos, movSiz);

  // in das entstandene Loch hineinkopieren
  //       dest   src      siz
  memcpy(delPos, insDat, insSiz);

  // printf(" new=|");  UTX_dump_c__ (delPos, insSiz); printf("|\n");
  // printf("ex MEM_chg_rec %d\n",*datSiz);

  return 0;

}


//==========================================================================
  int MEM_del_IndRec (int *recNr, void *recTab, int ipos) {
//==========================================================================
/// \code
/// MEM_del_IndRec          delete 1 integer in intArray
/// recNr  is decremented
/// \endcode

  return MEM_del_nrec (recNr, recTab, ipos, 1, sizeof(int));

}


//==========================================================================
  int MEM_del_DbRec (int *recNr, void *recTab, int ipos) {
//==========================================================================
/// \code
/// MEM_del_DbRec          delete 1 double in double-Array
/// recNr  is decremented
/// \endcode

  return MEM_del_nrec (recNr, recTab, ipos, 1, sizeof(double));

}


//==========================================================================
  int MEM_del_nrec (int *recNr, void *recTab,
                   int ipos, int delRecNr, int sizRec) {
//==========================================================================
/// \code
/// delete <delRecNr> records from table.
/// first Record to delete has index ipos.
/// recTab consists of *recNr records of size sizRec
/// 
/// Input/Output:
/// *recNr   nr of records of table recTab;
///          after exit recNr -= delRecNr
/// recTab   table with *recNr records; each with a size of sizRec bytes.
/// 
/// Input:
/// ipos     index in recTab of the first record to delete ..
/// delRecNr nr of records to delete
/// sizRec   size of records of recTab
/// 
/// Example delete the 3. point (index nr 2) out of pTab:
/// MEM_del_nrec (&pNr, pTab, 2, 1, sizeof(Point));
/// \endcode


  int   movSiz;
  char  *oldPos, *newPos;


  // printf("MEM_del_nrec %d %d %d\n",*recNr,ipos,delRecNr);

  if((delRecNr < 1)||(ipos < 0)||((ipos+delRecNr) > *recNr)) {
    TX_Error("MEM_del_nrec E001 %d %d\n",*recNr,ipos);
    return -1;
  }

  movSiz = (*recNr  - ipos) * sizRec;

  newPos = (char*)recTab + (ipos * sizRec);

  oldPos = newPos + (delRecNr * sizRec);

  *recNr -= delRecNr;

  // memcpy (newPos, oldPos, movSiz);
  memmove (newPos, oldPos, movSiz);    // 2014-12-15

  return 0;

}



//================================================================
  int MEM_inv_itab (int recNr, int *recTab) {
//================================================================
/// MEM_inv_itab              invert table of intData; 1/2/3 --> 3/2/1

  int    inach, ivon;

  // printf("MEM_inv_itab %d\n",ptNr);

  inach = 0;
  ivon = recNr - 1;

  L_umd_next:
  // printf("   vert. %d -> %d\n",ivon,inach);
  MEM_swap_int (&recTab[inach], &recTab[ivon]);
  ++inach;
  --ivon;
  if(inach < ivon) goto L_umd_next;

  // int i1;for(i1=0;i1<recNr;++i1)printf(" [%d]=%d\n",i1,recTab[i1]);

  return 0;

}


//================================================================
  int MEM_inv_dtab (int recNr, double *recTab) {
//================================================================
/// MEM_inv_dtab              invert table of dbData; 1/2/3 --> 3/2/1

  int    inach, ivon;

  // printf("MEM_inv_dtab %d\n",recNr);

  if(recNr < 1) return 0;  // Crash m. 0 !

  inach = 0;
  ivon = recNr - 1;

  L_umd_next:
    // printf("   vert. %d -> %d\n",ivon,inach);
  MEM_swap_2db (&recTab[inach], &recTab[ivon]);
  ++inach;
  --ivon;
  if(inach < ivon) goto L_umd_next;

  // int i1;for(i1=0;i1<recNr;++i1)printf(" [%d]=%f\n",i1,recTab[i1]);

  return 0;

}


//================================================================
  int MEM_inv_rtab (int recNr, void *recTab, int sizRec) {
//================================================================
/// MEM_inv_rtab              invert table of dataRecords; 1/2/3 --> 3/2/1

  int    inach, ivon;
  char   *pnach, *pvon;

  // printf("MEM_inv_rtab %d %d\n",recNr,sizRec);

  inach = 0;
  ivon = recNr - 1;
  pnach = (char*)recTab;
  pvon  = &(((char*)recTab)[ivon * sizRec]);

  L_umd_next:
    // printf("   vert. %d -> %d\n",ivon,inach);
  MEM_swap__ (pnach, pvon, sizRec);
  ++inach;
  --ivon;
  pnach += sizRec;
  pvon  -= sizRec;
  if(inach < ivon) goto L_umd_next;

  // int i1;for(i1=0;i1<recNr;++i1)printf(" [%d]=%f\n",i1,((double*)recTab)[i1]);

  return 0;

}


//================================================================
  int MEM_copy_tab (void *tabo, void *tabi, int recNr, int sizRec) {
//================================================================
/// MEM_copy_tab                  copy <recNr> records of size <sizRec>
///  Input:
///    tabi:      <recNr> records; each has size <sizRec>


  memcpy (tabo, tabi, recNr * sizRec);

  return 0;

}


//===================================================================
  int MEM_copy_oTab (void **poso, long *sizo,
                     void **posi, int recNr, int sizRec) {
//===================================================================
/// \code
/// copy <recNr> records of size <sizRec> from <posi> to <poso>;
/// reduce sizo and update <poso>
/// poso    pointer to next free position; is updated
/// sizo    size of free space in poso; is reduced
/// posi    pointer to data to be copied; is changed to new position poso !
/// see also MEM_copy_tab
/// \endcode

  long  osiz;


  printf("MEM_copy_oTab sizo=%ld recNr=%d sizRec=%d\n",*sizo,recNr,sizRec);


  osiz = recNr * sizRec;
  if(osiz > *sizo) {
    TX_Error("MEM_copy_oTab E001 - EOM %ld %ld\n",osiz,*sizo);
    return -1;
  }


  memcpy (*poso, *posi, osiz);

  // set the pointer to the new position
  *poso = (char*)*poso + osiz;
  *sizo -= osiz;
    printf("ex MEM_copy_oTab %ld\n",*sizo);



  return 0;

}


//================================================================
  int MEM_set_1recn (void *tabo, void *tabi, int recNr, int sizRec) {
//================================================================
/// MEM_set_1recn                write record <recNr> times
///  Input:
///    tabi:      one record of size <sizRec>

  int      i1;


  for(i1=0; i1<recNr; ++i1) {
    memcpy (tabo, tabi, sizRec);
    // (char*)tabo += sizRec;
    tabo = (char*)tabo + sizRec;
  }

  return 0;

}


//====================== EOF ===========================
