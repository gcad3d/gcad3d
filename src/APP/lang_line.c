// cc lang_line.c -o lang_line && ./lang_line LN_parallel Linie parallel zu Linie/Vektor
// ./rename_blanks

// change all blanks and '&' in filenames into underscores ..
// remove characters `and '
// for use with messagefiles msg_... ?



#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>                     // isdigit


main (int paranz, char *params[]) {

  int    i1, i2, iStat, sLen;
  char   s1[1024], s2[256], fn[256], ss[128], lNew[1024];
  FILE   *fp1, *fp2, *fp3;


  printf("modify a line %d |%s|\n",paranz,params[0]);
  if(paranz < 2) exit(0);


  strcpy(ss, params[1]);
  strcat(ss, " ");
  printf("  search |%s|\n",ss);
  sLen = strlen(ss);

  strcpy(lNew, ss);
  strcat(lNew, params[2]);
  for(i1=3; i1 < paranz; ++i1) {
    strcat(lNew, " ");
    strcat(lNew, params[i1]);
    // printf("%d |%s|\n",i1,lNew);
  }
  printf("  lNew |%s|\n",lNew);


  system ("ls -1 | grep msg_...txt > /tmp/t1.dat");


  if ((fp1 = fopen ("/tmp/t1.dat", "r")) == NULL) {
    printf ("Fehler open Datei %s\n", "/tmp/t1.dat");
    exit(-1);
  }

  // loop tru fileList
  while (!feof (fp1)) {
    iStat = 0;
    if (fgets (fn, 250, fp1) == NULL) break;

    // remove the \n
    i2 = strlen(fn);
    --i2;
    fn[i2] = '\0';

    sprintf(s1, "cp -f %s actMsgFilR.txt", fn);
      printf("%s\n",s1);
    system(s1);


    //----------------------------------------------------------------
    // loop thru file
    iStat = 0;

    if ((fp2 = fopen ("actMsgFilR.txt", "r")) == NULL) {
      printf ("Fehler open Datei R\n");
      exit(-1);
    }

    if ((fp3 = fopen ("actMsgFilW.txt", "w")) == NULL) {
      printf ("Fehler open Datei W\n");
      exit(-1);
    }

    while (!feof (fp2)) {
      if (fgets (s1, 1000, fp2) == NULL) break;

      // test if s1 starts with ss
      if(!strncmp(s1, ss, sLen)) {

        // write new line
        fprintf(fp3, "%s\n",lNew);
        ++iStat;

      } else {
        // write unmodified
        fprintf(fp3, "%s",s1);
      }

    }
    fclose (fp2);
    fclose (fp3);

    // copy modified back
    if(iStat) {
      sprintf(s1, "cp -f actMsgFilW.txt %s", fn);
        printf("%s\n",s1);
      system(s1);
    }

  }
  fclose(fp1);


}
