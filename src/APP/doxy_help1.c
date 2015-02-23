/*  doxy_help1.c


loop tru all directories, write Sourcefiles.dox.
get "\brief " out of sourcefiles ..


Inputs:
parameter 1  pagename
Environment  dox_path_src        // path from doxygen-outputDirectory -> srcBase
Environment  srcDirs             // list of directories
File         srcFiles            // list of files


LINUX ONLY.

Link: ../doc/gui_doxygen/doxy_help1.sh


---------------------------------------------------------
Modifications:
2015-02-09 dir-info def = "-". RF.

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
char *dList, *fList, **fTab, **dTab, *sOut2Src;

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


  printf("--------- doxy_help1 ---------------%d\n",paranz);
  if(paranz < 2)  {printf("******** doxy_help1 E000\n"); return -1;}



  // path from doxygen-outputDirectory -> srcBase
  sOut2Src = getenv ("dox_path_src");
  if(!sOut2Src) {printf("******** doxy_help1 E001\n"); return -1;}



  //========================================================



  //----------------------------------------------------------------
  // get dList from envVar srcDirs
  dList = getenv("srcDirs");
  if(!dList) {printf("******** doxy_help1 E003\n"); return -1;}
  // get spc
  l2 = strlen(dList) * 2;
    printf(" l2-dirs=%ld |%s|\n",l2,dList);
  dTab = alloca (l2);
  dNr = l2 / sizeof(void*);
  // get pointerlist fTab from fList
  dNr = UTX_wTab_strg (dTab, dNr, dList);
  l1 = 0;
    // for(l1=0; l1<dNr; ++l1) printf(" dTab[%ld]=|%s|\n",l1,dTab[l1]);
    printf(" %d dirs found\n",dNr);



  //----------------------------------------------------------------
  // get fileList out of file srcFiles
  // get filesize
  l1 = OS_FilSiz ("srcFiles");
  if(l1 < 2) {printf("******** doxy_help1 E002\n"); return -1;}

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
      // printf("fList=|%s|\n",fList);

    if(!feof (fpi)) {
      i1 = strlen(fList);
      fList[i1] = ' ';
      ++i1;
      fList[i1] = '\0';
      goto L_nxt_ln;
    }
  fclose(fpi);

  // get pointerlist fTab from fList
  fNr = UTX_wTab_strg (fTab, fNr, fList);
  l1 = 0;
    // for(l1=0; l1<fNr; ++l1) printf(" fTab[%ld]=|%s|\n",l1,fTab[l1]);
    printf(" %d files found\n",fNr);



  //========================================================
  if((fpo=fopen("Sourcefiles.dox","w")) == NULL) {
    printf("****** doxy_help1 OPEN ERROR FILE Sourcefiles.dox **********\n");
    return -1;
  }

  fprintf(fpo, "/*! \\page %s\n\n",argv[1]);
  fprintf(fpo, "<h2><CENTER>Sourcefiles</CENTER></h2>\n\n");
  // TT proportional (each char same width)
  fprintf(fpo, "<pre><TT>\n\n");


  do_dirs1 (fpo);


  fprintf(fpo, "\n</TT><hr></pre>\n");
  fprintf(fpo, "*/\n");
  fclose(fpo);

  L_exit:
  return 0;

}


//================================================================
  int do_dirs1 (FILE *fpo) {
//================================================================
// get all directories out of envVar srcDirs

  int     ii, irc;
  long    l1;
  char    *s1, *s2, *s3, *p1, *p2, fn[200], sfmt[40], sDirInf[200];

  printf("do_dirs1 \n");


  //========================================================
  // loop tru directories in with
  for(ii=0; ii<dNr; ++ii) {
    p2 = dTab[ii];
    // p2 = strtok (p1, " ");
    // if(!p2) goto L_fertig;
      // // printf(" nxt dir |%s|\n",p2);
    if(strncmp(p2, "../", 3)) {
      printf("******** do_dirs1 E002\n");
      continue;
    }
      printf(" nxt dir %d %s\n",ii,p2);
    // test if at least one file in fTab
    irc = do_ck_files (p2);
    if(irc < 0) continue;

    // get fn = filename of dirinfo-file
    sprintf(fn, "%s/%s/dirinf.txt",sOut2Src,&p2[3]);
      // printf(" fn-dirInf=|%s|\n",fn);
    // get <dir>/dirinf.txt -> sDirInf  // get file into string
    l1 = OS_FilSiz (fn);
    if(l1 < 2) {
      strcpy(sDirInf, "-");
    } else if(l1 > 200) {
      TX_Error("do_dirs1 E002\n");
      sDirInf[0] = '\0';
    } else {
      irc = UTX_str_file (sDirInf, &l1, fn);
        // printf(" UTX_str_file %d |%s|\n",irc,sDirInf);
    }
    // output dirctory-info
    fprintf(fpo, "\n\n\\subpage\n");
  
    // fprintf(fpo, "<b><hr>%s%-30s %s</b>\n",&p2[3],"/",sDirInf);
  
    sprintf(sfmt, "<b><hr>%%s%%-%lds %%s</b>\n\n",40-strlen(p2));
      // printf(" sfmt=|%s|\n",sfmt);
    fprintf(fpo, sfmt, &p2[3], "/", sDirInf);
  
    // add all files in dir. p2
    do_files1 (fpo, p2);
  }


  //================================================================
  L_fertig:
  return 0;

}

 
//================================================================
  int do_ck_files (char *actDir) {
//================================================================
// test if at least one file in fTab; 1=yes, -1=no

  int    i1, ii;

  ii = strlen (actDir);

  for(i1=0; i1<fNr; ++i1) {
    // skip file if not in actDir
    if(!strncmp (actDir, fTab[i1], ii)) return 1;
  }

  return -1;

}


//================================================================
  int do_files1 (FILE *fpo, char *actDir) {
//================================================================
// get all files in directory actDir out of list of files (?)


  int    i1, ii;
  char   *p1, *p2, sbuf[200], sfmt[40], sInfo[200];  // *ss="source</A>";


  printf("do_files1 |%s|\n",actDir);

  ii = strlen (actDir);

  for(i1=0; i1<fNr; ++i1) {
    p2 = fTab[i1];
    // skip file if not in actDir
    if(strncmp (actDir, p2, ii)) continue;
    if(p2[ii] != '/') continue;
    if(strncmp(p2, "../", 3)) {
      TX_Error("******** do_files1 E002_%s\n",p2);
      continue;
    }
      printf(" fTab[%d]=|%s|\n",i1,p2);

    // get brief-info from file p2
    getFileInfo (sInfo, p2);

    // output
    // need filename from doxygen-homeDir (use sOut2Src)
    sprintf(sbuf, "<A HREF=\"%s/%s\">source</A>",sOut2Src,&p2[3]);
      // printf(" sbuf=|%s|\n",sbuf);
    fprintf(fpo, "\\ref %-40s %s    %s\n",&p2[3],sbuf,sInfo);

    // sprintf(sfmt, "\\ref %%-%ds %%s    %%s\n",60-strlen(p2));
    // fprintf(fpo, sfmt, &p2[3], sbuf, sInfo);
  }


  L_fertig:
  return 0;

}


//================================================================
  int getFileInfo (char *sInfo, char *fnam) {
//================================================================
 
  int     irc = 0;
  FILE    *fpi;
  char    s1[1024], *p1;

  printf("getFileInfo |%s|\n",fnam);

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
  int do_files_ (FILE *fpo, char *sDir) {
//================================================================
// write for each .c .h in sDir
// \ref APP/hello-world.c     simple window with button
 
  int   iNr;
  char  cbuf[256], sbuf[200], *p1;



  printf("do_files_ |%s|\n", sDir);

  // get a list of all *.c, *.h files in sDir



  iNr = 0;
  strcpy(cbuf, sDir);  // in: Path; Out: File !
  OS_dir_scan_ (cbuf, &iNr);   // Init

  if(iNr > 0)  {

    for (;;) {

      OS_dir_scan_ (cbuf, &iNr);
      if(iNr < 0) break;
        // printf(" n.scan %d |%s|\n",iNr,cbuf);


      // skip files not ending with ".c" or .h" 
      // 0=equal; else=notEqual.
      if(!UTX_strcmp_right (cbuf, ".c")) goto L_OK;
      if(!UTX_strcmp_right (cbuf, ".h")) goto L_OK;
      continue;

      L_OK:
      // cbuf = filename
        printf(" L_OK %d |%s|\n",iNr,cbuf);
      sprintf(sbuf, "<A HREF=\"%s/%s\">source</A>",sOut2Src,cbuf);
        printf(" sbuf=|%s|\n",sbuf);
      // printf(" fNam %d |%s|%s|\n",iNr,cbuf,p1);
      fprintf(fpo, "\\ref %-40s %s %s\n",cbuf, sbuf, "yy");
      // ++iOut;

    }
  }



  // printf(" nach OS_dir_scan_ %d\n",iNr);



  return 0;

}


//==========================================================================
  int OS_dir_scan_ (char *cbuf, int *iNr) {
//==========================================================================
/// \code
/// scan directory
/// iNr = 0: init suche; zu scannender Pfad ist cbuf.
/// iNr > 0: cbuf ist next found file; do not change iNr! (Filename ohne Path !)
/// iNr < 0; kein weiteres File found; directory closed.
/// \endcode 

  static DIR     *dir1;
  static struct  dirent* fn1;
  static char    dirAct[256];

  int            i1, irc;
  char           *p1;


  // printf("OS_dir_scan_ |%s| %d\n",cbuf,*iNr);
  // if(*iNr > 100) exit(0);



  // opendir, closedir, readdir (braucht sys/types und dirent.h)

  if(*iNr == 0) {
    strcpy(dirAct, cbuf);
    if(dirAct[strlen(dirAct)-1] != '/') strcat(dirAct, "/");  // add "\\"
    // if(dirAct[strlen(dirAct)-1] == '/') dirAct[strlen(dirAct)-1] = '\0';
    // printf("init OS_dir_scan_ |%s| %d\n",dirAct,*iNr);

    dir1 = opendir (dirAct);
    if(dir1 == NULL) {
      printf("nix gfundn\n");
      *iNr = -1;
      irc  = -1;
      goto L_exit;
    }
    goto L_fertig;
  }


  NextNam:
  fn1 = readdir (dir1);
  if(fn1 == NULL) {
    closedir (dir1);
    *iNr = -1;
    irc  = -1;
    goto L_exit;
  }
  p1 = fn1->d_name;
  // printf(" next1 |%s|\n",p1);


  // skip "." und ".."
  if(!strcmp (p1, ".")) goto NextNam;
  if(!strcmp (p1, "..")) goto NextNam;


  strcpy(cbuf, dirAct);
  strcat(cbuf, p1);
  // printf(" next2 |%s|\n",cbuf);


  // mindestens lesbar ..
  // i1 = access (cbuf, R_OK);  // W_OK od W_ACC
  // // printf(" access=%d\n",i1);
  // if(i1 != 0) goto NextNam;


  L_fertig:
  *iNr += 1;
  irc  = 0;

  L_exit:
  // printf("ex OS_dir_scan_ %d |%s| %d\n",irc,cbuf,*iNr);
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
  return 0;

}

// eof
