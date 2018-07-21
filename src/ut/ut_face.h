// ../ut/ut_face.h
// prereq:
// ../ut/ut_memTab.h
// ../ut/ut_itmsh.h        // Fac3 MSHIG_EDGLN_..   typedef_MemTab..


  int UFA_fnb_dump_1 (Fac3 *fac, Fac3 *fnb, char *txt, ...);
  void UFA_3esn_ips (int *ep, int *es, int *en, Fac3 *fac1, int ips);
  void UFA_3esn_ipe (int *ep, int *es, int *en, Fac3 *fac1, int ipe);



//             3
//            /|    
//          /  |
//        /   2|               
//      /3     |
//    /   ifc  |
//  /     1    |
// 1-----------2
//          


// get pointNr opposit to edgeNr; p3 <- e1; p1 <- e2; p2 <- e3;
int UFA_psn_opp_esn (int);
#define UFA_psn_opp_esn(esn) ((esn) < 2 ? 3 : esn - 1) 


// get edgeNr opposit to pointNr; e2 <- p1; e3 <- p2; e1 <- p3; 
int UFA_esn_opp_psn (int);
#define UFA_esn_opp_psn(esn) ((esn) > 2 ? 1 : esn + 1)


// get next esn; 1 -> 2; 2 -> 3; 3 -> 1.
int UFA_esn_nxt (int);
#define UFA_esn_nxt(esn) ((esn) > 2 ? 1 : esn + 1)


// get previous esn; 1 -> 3; 2 -> 1; 3 -> 2.
int UFA_esn_prv (int);
#define UFA_esn_prv(esn) ((esn) < 2 ? 3 : esn - 1)


// get previous and next edge-sequence-nr; eg es=2 gives ep=1 and en=3
void UFA_2esn_prvnxt (int *ep, int *en, int es);
#define UFA_2esn_prvnxt(ep,en,es)\
 {if(es<2){*ep=3;*en=2;}else if(es<3){*ep=1;*en=3;}else{*ep=2;*en=1;}}
//   es=1                       es=2                    es=3


#define UFA_fNr_max(pNr) (pNr * 4)


// INCLUDE_FULL defined only in ../ut/ut_face.c
// Fac3 fnb_empty = _UFA_NUL;
#define _UFA_NUL {0, 0, 0, 16}
#ifndef INCLUDE_FULL
extern Fac3 UFA_NUL;
#else
Fac3 UFA_NUL = _UFA_NUL;
#endif


// EOF
