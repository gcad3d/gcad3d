// ../xa/xa_cad_ui.h


// IE_get_inp_dli       get dli for active inputField
// dli = IE_get_inp_dli (IE_inpInd);
#define IE_get_inp_dli(iind)  -iind - 2
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
