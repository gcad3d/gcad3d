
typedef struct {
  int  nrVal;      // nr of values per line
  char **pTab;     // table of <nrVal> pointers; strings; for out and in.
  char delVal;     // value-delimiter-char; comma or semicolon
  } CSV_struct;


  int CSV_write__ (FILE *fpo, CSV_struct *fTyp);
  int CSV_read__ (CSV_struct *fTyp, char *sb, int sSiz, FILE *fpi);

// EOF
