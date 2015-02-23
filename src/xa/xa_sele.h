/* ../xa/xa_sele.h

*//*!
\file ../xa/xa_sele.h
\ingroup grp_ap
*/



// selectionGroups (cannot be used as chars):      TYP_IS_SELGRP()
#define Typ_goGeom      1000  ///< all       (USED AS CHAR U)
#define Typ_goPrim      1001  ///< PT/LN/CI/Spline; nicht Plg,CCV
#define Typ_goGeo1      1002  ///< LN/CI/CV/PL/Sur/Sol (nicht PT)
#define Typ_goGeo2      1003  ///< CI/ELL (prj -> PLN)
#define Typ_goGeo3      1004  ///< UNUSED
#define Typ_goGeo4      1005  ///< UNUSED VC/LN/PLN
#define Typ_goGeo5      1006  ///< CIR/ELL/CCV (closed cont.)
#define Typ_goGeoSUSU   1007  ///< Sur|Sol   (Supporting Surface)
#define Typ_goGeo6      1008  ///< PT|LN|AC
#define Typ_goGeo7      1009  ///< ANG/PT  angle (0-360) od angle from point
#define Typ_goGeo8      1010  ///< Val/PT  parameter (0-1) od par. from point
#define Typ_goAxis      1011  ///< RotAxis LN/PT+PT/PT+LN/PT+VC/PLN
#define Typ_go_LCS      1012  ///< Line,Circ,Curv
#define Typ_go_LR       1013  ///< Line,Plane

// selectionModifiers
#define Typ_FncVAR1     1020  ///< next DB-var
#define Typ_FncVAR2     1021  ///< previous DB-var
#define Typ_FncVC1      1022  ///< next DB-vector
#define Typ_FncVC2      1023  ///< previous DB-vector
#define Typ_FncNxt      1024  ///< next version/solution (Typ_modCoord)
#define Typ_FncPrv      1025  ///< previous version/solution (Typ_modCoord)
#define Typ_FncNxt      1024  ///< next version/solution (Typ_modCoord)
#define Typ_FncPrv      1025  ///< previous version/solution (Typ_modCoord)
#define Typ_FncDirX     1026  ///< parallel (U) or across (V)
#define Typ_FncPtOnObj  1027  ///< parametric point on obj
#define Typ_FncPtOnCP   1028  ///< position on constrPln

// aux
#define Typ_EOT         1050  ///< last typ
// #define Typ_Val_symTyp  1100
// #define Typ_Str_Dir1    1101
// #define Typ_Obj_0       1102  


  typedef struct {int typ; char oid[128];}            subCurv;
// L|C|S(basecurve v1)    typ=L|C|S; v1=segNr; v2=0.; eg L in CCV
// L(basecurve v1 v2)     typ=L; v1=segNr; v2=segNr(>=1); eg L from plg in CCV
// P(basecurve v2)        typ=P; v1<1.; v2=parameter;     eg P on plg
// P(basecurve v1 v2)     typ=P; v1=segNr(>=1); v2=parameter; eg P on L in CCV
                          // segmentNumbers: 1 = first segment.



// EOF
