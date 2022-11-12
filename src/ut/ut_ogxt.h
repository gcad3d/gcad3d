/* OgxTab                             2020-08-14   franz.reiter@cadcam.co.at
../ut/ut_ogxt.h
Functions: ../ut/ut_ogxt.c ../APP/ut_ogxt.c
*/


#define _OGXTAB_NUL { _MEMTAB_NUL, _MEMSPC_NUL }

typedef struct {
                MemTab ogx;   // MemTab(ObjGX)
                Memspc spc;   // Memspc
               }                                       OgxTab;


void*  OXMT_reserve_osp (OgxTab *oxtb, int osiz);
void*  OXMT_add_ume (OgxTab *oxtb, int form, int oNr, void *data);
int    OXMT_add_oxt (OgxTab *oxtb, ObjGX **pStart, ObjGX *oxa, int oNr);
ObjGX* OXMT_MEMTAB_DAT (OgxTab *oxtb);

int OXMT_init_mm (OgxTab *oxtb1, int rNr, int datSiz);
int OXMT_free (OgxTab *oxtb1);
int OXMT_dump__ (OgxTab *oxtb1, char *txt);
int OXMT_dump_1 (OgxTab *oxtb1, int ii, char *txt, ...);

// EOF
