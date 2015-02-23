
#define APP_NAME  "APP_GIS1"
#define VERSION " APP_GIS1 Version 2.00 / 2011-01-03"

#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


typedef struct {int indp, indm, ptNr;
  unsigned visi:1, exi_p:1, exi_m:1, modif:1, unused:29;}  typ_sur_ptab;
// indp    DB-Index of PTAB-surf
// indm    DB-Index of MSH-surf
// exi_p   1: PTAB-surf-Code already exists in Model; 0=no.
// exi_m   1: MSH-surf-Code already exists in Model; 0=no.
// visi    0=not visible; 1=visible;
// modif   1=points modified; 0=not.



# define TOL_GIS_PT  0.1

#define APPTYP_GISPT 0
#define APPTYP_EDGLN 1
#define APPTYP_MESH  2

#define MAX_SURPTAB 50

#define INC_Ptab 10000
#define INC_Ftab 10000
#define INC_Etab 10
#define INC_datEtab 50
#define INC_actEl 100


#define GISFUNC_Edit    1
#define GISFUNC_prjMsh  2
#define GISFUNC_crEdge  3

// EOF
