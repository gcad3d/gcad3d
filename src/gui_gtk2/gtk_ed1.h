MemObj GUI_edi__ (MemObj *o_par, void *funcnam, int mode, char *opts);
int GUI_edi_Insert (MemObj *mo, long cpos, char* txbuf, long txlen);
int GUI_edi_Write (MemObj *mo, char* txbuf, long *txlen, long maxlen);
// int GUI_edi_Read (MemObj *mo, char* txbuf, long *txlen);
int GUI_edi_Read (char* txbuf, long *txlen, long p1, long p2, MemObj *mo);
long GUI_edi_getEof (MemObj *mo);
long GUI_edi_getCpos (MemObj *mo);
int GUI_edi_setCpos (MemObj *mo, long cpos);
long GUI_edi_getLnr (MemObj *mo);
int GUI_edi_setLnr (MemObj *mo, long lNr);
int GUI_edi_scroll_s (MemObj *mo);
void GUI_edi_Focus (MemObj *mo);
int GUI_ed1_decode (MemObj *mo);
int GUI_edi_sel_get (long *p1, long *p2, MemObj *mo);
int GUI_edi_sel_ln (MemObj *mo, long lNr);
int GUI_edi_sel_wrf (MemObj *mo, char *fnam);
int GUI_edi_mod_ln (MemObj *mo, long lNr, char *newLn);
// EOF
