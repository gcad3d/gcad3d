/* BitTab.h       Functions for bit-arrays         2010-04-29  RF
for setting bits see also BIT_SET BIT_GET ..
*/
#ifdef globTag
void BitTab(){}
#endif
/*!
\file ../ut/ut_BitTab.h
\ingroup grp_ut_obj
\code

Functions for bit-arrays

=====================================================
Liste_Funktionen_Beginn:

BitTab                define bitarray
BitTab_ini0           init all bits to value 0
BitTab_ini1           init all bits to value 1
BitTab_set            set bit
BitTab_get            get bit
BitTab_clr            reset (clear) bit

BitTab_dump

Liste_Funktionen_Ende:
=====================================================

Example:

#include "../ut/ut_BitTab.h"           // bit-arrays

  BitTab (ba,256);          // define array of 256 bits; creates "char bt[32];"


  BitTab_ini0 (ba);         // init all bits to value 0;

  BitTab_set (ba,20);       // set bit nr 20

  i1 = BitTab_get (ba,20);  // test bit nr 20; returns != 0 (is set;  0=not set).

  BitTab_clr (ba,20);       // reset (clear) bit nr 20.

\endcode *//*----------------------------------------


*/

/// define bitArray; bt = arrayName; siz = number of bits.
#define BitTab(bt,siz)\
 unsigned char bt[(siz >> 3) + 1]

/// init bitArray. Fill bitArray with zeros.
#define BitTab_ini0(bt)\
 memset(bt,'\0',sizeof(bt))

/// init bitArray. Fill bitArray with 1.
#define BitTab_ini1(bt)\
 memset(bt,255,sizeof(bt))

/// set bit.  bt = arrayName; bitnr = ii.
#define BitTab_set(bt,ii)\
  (bt[ii >> 3] |= 1 << ii % 8)

/// read bit. bt = arrayName; bitnr = ii.  RetVal: 0 = not_set;  else set.
#define BitTab_get(bt,ii)\
  (bt[ii >> 3] & 1 << ii % 8)

/// clear bit (reset bit). bt = arrayName; bitnr = ii.
#define BitTab_clr(bt,ii)\
  bt[ii >> 3] &= ~(1 << ii % 8)

/// copy bitArray. (bt2 = bt1)
#define BitTab_cpy(bt2,bt1)\
   memcpy (bt2, bt1, sizeof(bt1))

/// get nr of bits
#define BitTab_bNr(bt1)\
   (sizeof(bt1) << 3)


// Proto:
int BitTab_dump (unsigned char *bt, int bNr);


// EOF
