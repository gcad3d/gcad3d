// ../xa/xa_cad_ui.h


// IE_GET_INP_DLI       get dli for active inputField
// dli = IE_GET_INP_DLI (IE_inpInd);
#define IE_GET_INP_DLI(iind)  -iind - 2
// dli f. Feld 0: -2, Feld 1: -3 ....
// see also DLI_TMP



#define INTPTYANZ     16

#define INPRECANZ      8         // Anzahl Eingabefelder




typedef struct   { Point pos;          // P: thisPoint; L,C,Curv: selectionPoint
                   Vector vx, vz;      // Vector: thisVector, Angle: vx,vz
                   double d1;          // Angle: angle.
                   char subTyp[16];    // word 3 of .info
                 } inpAuxDat;




// EOF
