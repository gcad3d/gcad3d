//    byteorder.c                   Franz Reiter       2005-03-15
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
  ..

-----------------------------------------------------
*/
/*!
\file  ../ut/byteorder.c
\brief read short,long from BigEndian/LittleEndian
\code
=====================================================
List_functions_start:

CBO_r_i2       read signed short (16 bit)
CBO_r_ui2      read unsigned short (16 bit, WORD)
CBO_r_i4       read signed long (32 bit)
CBO_r_ui4      read unsigned long (32 bit, DWORD)

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


BigEndian        LittleEndian
SGI/Irix,        Intel-Ix86
RS6000/AIX,      DEC-Alpha
SUN
NETZWERK!!!
links            rechts       ist das most significant Byte


#ifdef _BIG_END
#else 

Compile mit -D_BIG_END  !!!!


#include <netinet/in.h>
unsigned short int ntohs(unsigned short int netshort);
unsigned long int ntohl(unsigned long int netlong);
 short ex Netz-Byteordnung (Big) => Rechner-Byteordnung (Host = Little)

unsigned short int htons(unsigned short int hostshort);
unsigned long int htonl(unsigned long int hostlong);
 aus Rechner(Host, Little) => Netzwerk-Byteordnung (Big)


Microsoft-Datentypen:

WORD       unsigned short  (16 Bit)
DWORD      unsigned long   (32 Bit)
LONG       signed long     (32 Bit)
LPSTR      void*  Pointer -> char*

------------------------------------------------------------------------------

  //! Little/Big endian (from Steve Baker's Cute Code Collection)
  //!
  //! Extra comments by Kenny Hoff:
  //! Determines the byte-ordering of the current machine (little or big endian)
  //! by setting an integer value to 1 (so least significant bit is now 1); take
  //! the address of the int and cast to a byte pointer (treat integer as an
  //! array of four bytes); check the value of the first byte (must be 0 or 1).
  //! If the value is 1, then the first byte least significant byte and this
  //! implies LITTLE endian. If the value is 0, the first byte is the most
  //! significant byte, BIG endian. Examples:
  //!      integer 1 on BIG endian: 00000000 00000000 00000000 00000001
  //!   integer 1 on LITTLE endian: 00000001 00000000 00000000 00000000
  //!---------------------------------------------------------------------------
  int IsLittleEndian()  { int x=1;  return ( ((char*)(&x))[0] );  }
  inline_ char  LittleEndian()            { int i = 1; return *((char*)&i);     }


*/

#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/byteorder.h"


static FILE *CBO_fp = NULL;



//================================================================
  int CBO_close () {
//================================================================

  fclose (CBO_fp);

  CBO_fp = NULL;

  return 0;

}


//================================================================
  int CBO_open (char *fnam) {
//================================================================
// ACHTUNG: Open muss im Hauptmodule (nicht in DLL) erfolgen, wenn eine
// DLL eine im Hauptmodul befindliche Lesefuktion rufen moechte. Security ??

 if(CBO_fp != NULL) fclose(CBO_fp);


  if ((CBO_fp = fopen (fnam, "rb")) == NULL) {
    TX_Print("****** OPEN ERROR FILE %s **********",fnam);
    return -1;
  }

  return 0;
}


//================================================================
  int CBO_read (void *po, int bNr) {
//================================================================

  return fread(po, 1, bNr, CBO_fp);
  // return fread(po, bNr, 1, CBO_fp);

}



//================================================================
  unsigned short CBO_r_ui2 () {
//================================================================
// read unsigned short (16 bit, WORD)

  unsigned short i2;

  // printf("CBO_r_ui2  \n");


/*
  unsigned char b0, b1;
  b0 = getc(fpi);
  b1 = getc(fpi);

  printf(" |%c|%c|\n",b0,b1);

  // return ((b1 << 8) | b0);
  // return (b0 | b1<<8);
  *i2 = (b1 << 8) | b0;
  return 0;
*/

  // return fgetc(fp) | (fgetc(fp)<<8);


#ifdef _BIG_END
  // Version AIX:
  ((char*)i2)[1] = getc(CBO_fp);
  ((char*)i2)[0] = getc(CBO_fp);

#else
  // Version Intel / PC:
  // ((char*)i2)[0] = getc(CBO_fp);
  // ((char*)i2)[1] = getc(CBO_fp);
  fread (&i2, 2, 1, CBO_fp);
#endif


  return i2;

}


//================================================================
  unsigned int CBO_r_ui4 () {
//================================================================
// read unsigned long (32 bit, DWORD)
/*
  unsigned char b0, b1, b2, b3;

  b0 = getc(fp);
  b1 = getc(fp);
  b2 = getc(fp);
  b3 = getc(fp);

  // return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
  return (b0 | b1<<8 | b2<<16 | b3<<24);
*/

  int  i4;

#ifdef _BIG_END
  // Version AIX:
  ((char*)&i4)[3] = getc(CBO_fp);
  ((char*)&i4)[2] = getc(CBO_fp);
  ((char*)&i4)[1] = getc(CBO_fp);
  ((char*)&i4)[0] = getc(CBO_fp);

#else
  // Version Intel / PC:
  // ((char*)&i4)[0] = getc(CBO_fp);
  // ((char*)&i4)[1] = getc(CBO_fp);
  // ((char*)&i4)[2] = getc(CBO_fp);
  // ((char*)&i4)[3] = getc(CBO_fp);
  fread ((char*)&i4, 4, 1, CBO_fp);
#endif

  return i4;

}


//================================================================
  int CBO_r_i4 () {
//================================================================
// CBO_r_i4       read signed long (32 bit)

  int  i4;

#ifdef _BIG_END
  // Version AIX:
  ((char*)&i4)[3] = getc(CBO_fp);
  ((char*)&i4)[2] = getc(CBO_fp);
  ((char*)&i4)[1] = getc(CBO_fp);
  ((char*)&i4)[0] = getc(CBO_fp);

#else
  // Version Intel / PC:
  fread ((char*)&i4, 4, 1, CBO_fp);
#endif

  return i4;

}


//====================== EOF =============================
