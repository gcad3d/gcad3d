/* htmTree.c                           2009-04-23 RF

Create a HTMl-Treeview (JavaScript)

-----------------------------------------------------
TODO:


-----------------------------------------------------
Modifications:
2009-04-25 add source for branchLines. RF.
2009-03-23 Created. RF.


-----------------------------------------------------

Wie kann man die Blanks der naechsten zeile wissen ?
Eine zeile vorauslesen, aber alte zeile liefern.
von der gelesenen zeile die Blanks zaehlen und mit alter zeile mitliefern



Man muss nun ganz genau wissen, wievile blanks die naechste zeile hat
 - damit weiss man ob aktuelle zeile Branch oder Leaf ist !


add source to branch
ohne source:
<img src="bc.png" border="0" id="I1">

mit source:
<a class=sOn href="t3.htm" TARGET="frmr"><img src="bc.png" border=0 id="I1">CAD1</a>



mg src="bc.png" border="0" id="I1">-----------------------------------------------------
Files:
htmTree.c                   // this file
htmTree.js
htmTree.Readme



-----------------------------------------------------
cc -o htmTree htmTree.c &&./htmTree index_tree &&firefox index.htm



*/

#define FRNAM    "frmr"          // name of right frame
#define IMG_LEAF "p.png"         // filname of image for leaf
#define IMG_BRAN "bc.png"        // filname of image for branch



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>


#include <unistd.h>          // f. access, R_OK 


FILE  *fpi, *fpo;


  int UTX_endDelChar (char* txtbuf, char chr);
  void UTX_CleanCR (char* string);
  void TX_Error (char* txt, ...);
  void TX_Write (char* txt);




//================================================================
  int main (int argc, char *argv[]) {
//================================================================

  char fNam[80];
  char s1[512];


  // first parameter must be htm-filename without any filetype.
  // filetype must be <fn>.htm.htm

  if(argc < 2) {
    printf("ERROR: give filename of inputfile as 1. parameter ******\n");
    return -1;
  }


  // fix infilename (add ".htm.htm")
  sprintf(fNam, "%s.htm.htm",argv[1]);
    printf(" fn=|%s|\n",fNam);


  // test if file exists
  if(OS_checkFilExist(fNam,1) == 0) {
    printf("ERROR: file %s does not exist ******\n",fNam);
    return -1;
  }


  // open in- & outfile
  if((fpi=fopen(fNam,"r")) == NULL) {
    printf("ERROR: open infile %s fails ******\n",fNam);
    return -1;
  }


  // fix outfilename
  sprintf(fNam, "%s.htm",argv[1]);
  if((fpo=fopen(fNam,"w")) == NULL) {
    printf("ERROR: open outfile %s fails ******\n",fNam);
    return -1;
  }



  // read & copy file
  while (!feof (fpi)) {
    if (fgets (s1, 510, fpi) == NULL) break;
      // printf("%s",s1);

    // check for "<menu>"
    if(strncmp(s1, "<menu>", 6)) {
      fprintf(fpo, "%s",s1);
    } else {
      do_menu__ (fpo, fpi);
    }
  }

  fclose(fpi);
  fclose(fpo);


  return 0;
}


//================================================================
  int do_menu__ (FILE *fpo, FILE *fpi) {
//================================================================
// work menu-block; 1. incoming line = "<menu>"; return after "</menu>"
// read next Line; read level (nr of leading blanks) of both lines;
//   

  char s1[512], s2[512];
  char *p1, *p2;
  int  lev1, lev2;

  fgets (s1, 510, fpi);
  lev1 = 0;

  // work until "</menu>"
  while (!feof (fpi)) {
    if (fgets (s2, 510, fpi) == NULL) goto L_e1;
      printf(">>>>> |%s|",s1);

    // skip empty lines
    if(strlen(s2) < 2) continue;   // "LF"

    // skip lines starting with "//"  (commnets)
    if(!strncmp(s2, "//", 2)) continue;


    lev2 = getLev(s2);

    // work next menuline
    UTX_CleanCR (s1);
    do_menu_l (fpo, s1, lev1, lev2);


    // check for "</menu>" (end of block)
    if(!strncmp(s2, "</menu>", 7)) {
      do_menu_l (fpo, NULL);
      return 0;
    }

    strcpy(s1, s2);
    lev1 = lev2;

  }


  return 0;

  L_e1:
    printf("ERROR: </menu> not found\n");
    exit(-1);

}


//================================================================
  int getLev (char *sIn) {
//================================================================
// returns nr of leading blanks

  char  *p1;

  p1 = sIn;
  while (*p1 == ' ')  ++p1;

  return p1 - sIn;

}

 
//================================================================
  int do_menu_l (FILE *fpo, char *sIn, int newLev, int nxtLev) {
//================================================================
// work a single line of the menu-block

  static int iInd=0;        // indent; must be equal Level !
  static int actLev=0;      // the level after last line
  static int blkNr=0;
  static int oldTyp=-1;

  int  iFunc, iTyp;
  char *p1, *p2, *p3, *pe, sTit[256], sSrc[256];
 

  printf("do_menu_l |%s| %d %d\n",sIn,newLev,nxtLev);


  //----------------------------------------------------------------
  // exit menu; close branches
  if(sIn == NULL) {
      printf(" exit menu %d\n",actLev);
    while(actLev > 0) {
      wrUp (fpo, &blkNr);
      --actLev;
    }

    return 0;
  }


  //----------------------------------------------------------------
  // check for horizontal-ruler ("---")
  if(!strncmp(p1, "---", 3)) {
    iTyp = 3;      // ruler
    goto L_7;
  }


  //----------------------------------------------------------------
  // get title (1. word)
  p1 = strchr(sIn, '\"');
  if(p1 == NULL) {
    printf("ERROR in Line |%s|\n");
    printf("ERROR: Title does not start with \" !\n");
    exit(-1);
  }
  ++p1;
  strcpy(sTit, p1); 
  p2 = strchr(sTit, '\"');
  if(p2 == NULL) {
    printf("ERROR in Line |%s|\n",sIn);
    printf("ERROR: Title does not end with \" !\n");
    exit(-1);
  }
  *p2 = '\0';
  ++p2;
    printf(" sTit = |%s|\n",sTit);


  //----------------------------------------------------------------
  // get source (2. word)
  // skip blanks
  while (*p2 == ' ')  ++p2;
  pe = strchr(p2, '\0');
  if(p2 >= pe) {
    sSrc[0] = '\0';

  } else {
    strcpy(sSrc, p2);
  }
    printf(" sSrc = |%s|\n",sSrc);



  //----------------------------------------------------------------
  iFunc = newLev - actLev;
  if(iFunc > 1) {
    printf("ERROR in Line |%s|\n",sIn);
    printf("ERROR: line is intended more than one place !\n");
    exit(-1);
  }



  //----------------------------------------------------------------
  // branch (iTyp=0) or Leaf (iTyp=1)
  iFunc = nxtLev - newLev;
  if(iFunc > 0)      iTyp = 0;      // branch
  else               iTyp = 1;      // leaf



  //----------------------------------------------------------------
  // close branches
  L_7:
    printf(" actLev = %d; newLev = %d nxtLev = %d\n",actLev,newLev,nxtLev);
  // while(actLev != newLev) {
  while(actLev > newLev) {
    wrUp (fpo, &blkNr);
    --actLev;
  }



  //----------------------------------------------------------------
  if(iTyp == 3) {              // ruler
    fprintf(fpo, "<hr>\n\n");
 

  } else if(iTyp == 1) {       // Leaf
    // create new Leaf
    wrLeaf (fpo, sTit, sSrc);

  } else {
    // create new Header
    wrHdr (fpo, sTit, sSrc, &blkNr);

  }


  //----------------------------------------------------------------
  // branch: increment indent !
  if(iTyp == 0) ++actLev;

  oldTyp = iTyp;

  return 0;

}


//================================================================
  int wrHdr (FILE *fpo, char *txt, char *src, int *iNr) {
//================================================================
// create a new header

  printf("wrHdr |%s| %d\n",txt,*iNr);



  ++(*iNr);

  fprintf(fpo, "\n\n");
  fprintf(fpo, "<div class=\"sHdr\" onClick=\"brSwap('%d')\">\n",*iNr);

  if(strlen(src) < 4) {         // no source
  fprintf(fpo, "<img src=\"bc.png\" border=\"0\" id=\"I%d\">\n",*iNr);
  fprintf(fpo, "%s\n",txt);

  } else {                     // with source
  fprintf(fpo,
  "<a class=sOn href=\"%s\" TARGET=\"%s\"><img src=\"%s\" border=0 id=\"I%d\">%s</a><br>\n",
    src,FRNAM,IMG_BRAN,*iNr,txt);

  }
  fprintf(fpo, "</div>\n");


  // create the LeafHeader
  fprintf(fpo, "<span class=\"sOff\" id=\"B%d\">\n",*iNr);


  return 0;

}


//================================================================
  int wrUp (FILE *fpo, int *iNr) {
//================================================================
// close a branch

  printf("wrUp %d\n",*iNr);

  // --(*iNr);

  fprintf(fpo, "</span>\n\n");

  return 0;

}


//================================================================
  int wrLeaf (FILE *fpo, char *tit, char *src) {
//================================================================
// create a leaf

  printf("wrLeaf |%s|%s|\n",tit,src);


  fprintf(fpo,"<a class=sOn href=\"%s\" TARGET=\"%s\"><img src=\"%s\" border=0>%s</a><br>\n",
          src,FRNAM,IMG_LEAF,tit);


  return 0;

}


//=============================================================
  int OS_checkFilExist (char* filnam, int mode) {
//=============================================================
/// OS_checkFilExist         check if File or Directory exists
/// mode = 0: display message sofort;
/// mode = 1: just fix returncode, no message
/// mode = 2: make Errormessage (TX_Error) if File does not exist
/// 
/// rc = 0 = Fehler; datei existiert NICHT.
/// rc = 1 = OK, Datei existiert.


  static char    buf[256];

  // printf("OS_checkFilExist |%s| %d\n", filnam, mode);

  // remove following "/"
  strcpy (buf, filnam);
  UTX_endDelChar (buf, '/');

  /* Version PC: (braucht IO.h) */
  /* if ((access (buf, 0)) != 0) { */


  /* if ((access (buf, R_OK|W_OK)) != 0) { */

  if ((access (buf, R_OK)) != 0) {
    if (mode == 0) {
      printf ("*** Error OS_checkFilExist: %s does not exist\n",filnam);
      // printf ("%s\n",buf);
      /* UI_Dialog (NULL, buf); */

    } else if (mode == 2) {
      TX_Error(" - File %s does not exist ..",filnam);
    }

    return 0;
  }

  // printf("ex OS_checkFilExist YES |%s| %d\n",filnam,mode);
  return 1;

}

///=============================================================
  int UTX_endDelChar (char* txtbuf, char chr) {
///=============================================================
/// UTX_endDelChar           if last char of string == chr: remove it.
  
  
  if (txtbuf[strlen(txtbuf)-1] == chr) {
    txtbuf[strlen(txtbuf)-1] = '\0';
  }
  
  /* printf("UTX_endDelChar |%s| %c\n", txtbuf,chr); */

  return 1;

} 
    
//==========================================================
  void TX_Error (char* txt, ...) {
//==========================================================


  char   TX_buf1[1024];


  va_list va;
  // void    *p1,*p2,*p3,*p4,*p5,*p6,*p7,*p8,*p9,*p10,*p11;


  va_start(va,txt);

  // p1  = va_arg(va,void*);
  // p2  = va_arg(va,void*);
  // p3  = va_arg(va,void*);
  // p4  = va_arg(va,void*);
  // p5  = va_arg(va,void*);
  // p6  = va_arg(va,void*);
  // p7  = va_arg(va,void*);
  // p8  = va_arg(va,void*);
  // p9  = va_arg(va,void*);
  // p10 = va_arg(va,void*);
  // p11 = va_arg(va,void*);
  // va_end(va);


  // sprintf(TX_buf1,txt,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11);
  strcpy(TX_buf1, "*** Fehler: ");
                // o123456789012
  vsprintf(&TX_buf1[12],txt,va);
  // UTX_CleanCR (TX_buf1);
  TX_Write (TX_buf1);

  /* os_beep (); */


}


//==========================================================
  void TX_Write (char* txt) {
//==========================================================
///  direkt rausschreiben.


  // if(TX_mode == OFF) {
    // return;
  // }


  printf("%s\n",txt);

}


//===========================================================
  void UTX_CleanCR (char* string) {
//===========================================================
/// UTX_CleanCR                  Delete Blanks, CR's u. LF's am Ende.
///   Ersetzung von Blanks, Carriage Returns und Linefeeds durch
///   Nullzeichen von hinten nach vorne bis zum ersten von diesen
///   verschiedenen Zeichen.
    
    
  int  ilen;
  char *tpos;
    
  ilen = strlen (string);
    
  if(ilen < 1) goto L_exit;
      
  tpos = &string[ilen];
  --tpos;
    
    
  while ((*tpos  == ' ')  ||
         (*tpos  == '\t') ||          /* tab */
         (*tpos  == '\n') ||          /* newline */
         (*tpos  == '\r'))   {        /* CR */

    *tpos    = '\0';
    --tpos;
    if(tpos < string) break;
  }

  L_exit:
  // printf("ex UTX_CleanCR |%s|\n", string);

  return;
}



// EOF
