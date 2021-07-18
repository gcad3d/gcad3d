/*  ../inf/inf_help1.c


read filelist "srcFiles"; write ../inf/files.c
get "\brief    fileInfo" out of sourcefiles ..



LINUX ONLY.

# create ./srcFiles
make -f gcad3d.mak srclst

# create ../inf/files.c
gcc ../inf/inf_help1.c && ./a.out



---------------------------------------------------------
Modifications:
2019-08-05 rewritten from doxy_help1.c               RF

*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>          // f. DIR ..
#include <stdarg.h>          // va_list va_arg
#include <sys/stat.h>        // f. mkdir , stat



// global
int  dNr, fNr;
char *dList, *fList, **fTab, **dTab;

// protos:
void UTX_CleanCR (char* string);
long OS_FilSiz (char* filnam);
char** UTX_wTab_file (char *memspc, int memSiz, char *fnam);
int MEM_get_file (char* txbuf, long *fSiz, char *fnam);

void TX_Print (char* txt, ...);
int TX_Error(char* txt, ...);






//=======================================================================
  int main (int paranz, char** argv) {
//=======================================================================

  FILE    *fpo, *fpi;
  long    l1, l2;
  int     i1, i2;
  char    s1[512];


  printf("--------- inf_help1 ---------------%d\n",paranz);
  // if(paranz < 2)  {printf("******** doxy_help1 E000\n"); return -1;}



  // get fileList out of file srcFiles
  // get filesize
  l1 = OS_FilSiz ("srcFiles");
  if(l1 < 2) {printf("******** inf_help1 E002\n"); return -1;}

  // get space
  l2 = l1 + 64;
  fList = alloca (l2);
  fTab = alloca (l2);
  fNr = l2 / sizeof(void*);
    printf(" fNr alloc=%d\n",fNr);

  // get fList from file srcFiles
  if((fpi=fopen("srcFiles","r")) == NULL) {
    printf("****** doxy_help1 OPEN ERROR FILE srcFiles **********\n");
    return -1;
  }
  // fscanf (fpi, "%s", fList);
  i1 = 0;
  L_nxt_ln:
    fgets (&fList[i1], l2, fpi);  // concatenate
    UTX_CleanCR (&fList[i1]);     // Delete following Blanks, CR's u. LF's
      // printf("\n\nfList=|%s|\n",fList);

    if(!feof (fpi)) {
      i1 = strlen(fList);
      fList[i1] = ' ';
      ++i1;
      fList[i1] = '\0';
      goto L_nxt_ln;
    }

  fclose(fpi);
    // printf("\n\nfList=|%s|\n",fList);

  // get pointerlist fTab from fList
  fNr = UTX_wTab_strg (fTab, fNr, fList);
  l1 = 0;
    // for(l1=0; l1<fNr; ++l1) printf(" fTab[%ld]=|%s|\n",l1,fTab[l1]);
    // printf(" %d files found\n",fNr);



  //========================================================
  // if((fpo=fopen("Sourcefiles.dox","w")) == NULL) {
  if((fpo=fopen("../inf/files.c","w")) == NULL) {
    printf("****** inf_help1 OPEN ERROR FILE Sourcefiles.dox **********\n");
    return -1;
  }

  // fprintf(fpo, "/*! \\page %s\n\n",argv[1]);
  // fprintf(fpo, "<h2><CENTER>Sourcefiles</CENTER></h2>\n\n");
  // TT proportional (each char same width)
  fprintf(fpo, "/*  ../inf/files.c\n\n");
  fprintf(fpo, "see ../inf/inf_help1.c\n\n");
  fprintf(fpo, "*/ \n\n");
  fprintf(fpo, "void INF_files__ () { }  /*\n\n");


  for(l1=0; l1<fNr; ++l1) {
      // printf(" fTab[%ld]=|%s|\n",l1,fTab[l1]);
    getFileInfo (s1, fTab[l1]);
    fprintf(fpo, "%-28s %s\n", fTab[l1], s1);
  }


  // fprintf(fpo, "\n</TT><hr></pre>\n");
  fprintf(fpo, "*/\n");
  fclose(fpo);

  L_exit:
  return 0;

}


//================================================================
  int getFileInfo (char *sInfo, char *fnam) {
//================================================================
// get \brief-text out of file <fnam>
 
  int     irc = 0;
  FILE    *fpi;
  char    s1[1024], *p1;

  // printf("getFileInfo |%s|\n",fnam);

  if((fpi=fopen(fnam,"r")) == NULL) {
    TX_Error("****** getFileInfo  E001 %s\n",fnam);
    goto L_err9;
  }

  while (!feof (fpi)) {
    if (fgets (s1, 1024, fpi) == NULL) break;
    if (!strncmp (s1, "\\brief ", 7)) {
      p1 = &s1[7];
      while(*p1 == ' ') ++p1;
      strcpy (sInfo, p1);
      UTX_CleanCR (sInfo);
      ++irc;
      break;
    }
  }

  fclose (fpi);

  L_err9:
  if(!irc) strcpy (sInfo, "-");

  return irc;

}


//================================================================
  int UTX_strcmp_right (char *string, char *text) {
//================================================================
/// \code
/// UTX_strcmp_right              check if string1 ends with string2
/// RetCode: 0=equal; else=notEqual.
/// \endcode

  int   ll;
  char  *cp1;

  // printf("UTX_strcmp_right |%s|%s|\n",string,text);


  // get length of both strings
  ll = strlen(string) - strlen(text);
  if(ll < 0) return 1;


  cp1 = &string[ll];
    // printf(" cp1=|%s| %d\n",cp1,ll);


  return strcmp(cp1, text);

}


//=============================================================
  long OS_FilSiz (char* filnam) {
//=============================================================
/// OS_FilSiz               returns filesize in bytes

  int         rc;
  struct stat fileStatus;



  rc = stat (filnam, &fileStatus);

  // printf("rc=%d siz=%d\n",rc,fileStatus.st_size);

  if(rc < 0) {  // File nicht vorh.
    printf("ex OS_FilSiz -1 |%s|\n",filnam);
    return -1;
  }

  // printf("ex OS_FilSiz %d |%s|\n", fileStatus.st_size,filnam);

  return fileStatus.st_size;

}


///===========================================================
  void UTX_CleanCR (char* string) {
///===========================================================
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


//=========================================================================
  int UTX_wTab_strg (char **wTab, int wSiz, char *sTxt) {
//=========================================================================
// make table of words from file. End of List is NULL.
// Input:
//   sTxt     list of words, separated with blanks
//   wSiz     size of wTab
// Output:
//   wTab     pointerlist 
//   retCod   nr of pointers in wTab

  int  i1;
  char *p1, *p2;


  // get nr of words
  i1 = 0;
  // separate words
  p1 = sTxt;
    // printf("sTxt=|%s|\n",p1);



  //========================================================
  // loop tru directories in with
  L_nxt:
  p2 = strtok (p1, " ");
  if(!p2) goto L_fertig;
    // printf(" nxt dir |%s|\n",p2);

  wTab[i1] = p2;
  ++i1;
  if(i1 >= wSiz) {TX_Print("***** UTX_wTab_strg E001"); return -1;}
  p1 = NULL;
  goto L_nxt;


  //================================================================
  L_fertig:

  return i1;

}


//======================================================================
  int UTX_str_file (char* txbuf, long *fSiz, char *fnam) {
//======================================================================
/// \code
/// UTX_str_file           read file -> string; remove ending '\n'
/// fSiz must have exact filesize; see OS_FilSiz.
/// See MEM_get_file()
/// \endcode

  int   irc;


  // get file
  irc = MEM_get_file (txbuf, fSiz, fnam);
  if(irc < 0) return irc;

  // remove ending '\n'
   printf(" fSiz=%ld\n",*fSiz);

  if(txbuf[*fSiz - 1] == '\n') {    // LF
    *fSiz -= 1;
    txbuf[*fSiz] = '\0';
  }

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
    return;
  }

  fread (txbuf, *fSiz, 1, fpi);

  txbuf[*fSiz] = '\0';

  fclose(fpi);

    // printf(" _get_file|%s|\n",txbuf);

  return;

}


void TX_Print (char* txt, ...) { printf("%s\n",txt); }

int TX_Error(char* txt, ...) {
    va_list va;
    va_start(va,txt);
    printf("*** Fehler: ");
    vprintf(txt, va);
    printf("\n");
    va_end(va);
  return 0;

}

// eof
