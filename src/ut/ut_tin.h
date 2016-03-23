// ../ut/ut_tin.h
// prereq: ../ut/ut_memTab.h


typedef_MemTab(int);
typedef_MemTab(char);
typedef_MemTab(Point);
typedef_MemTab(Fac3);
typedef_MemTab(EdgeLine);



typedef struct {
  MemTab(EdgeLine) *eTab;  // breaklines (BL,AB,OB,IB)
  MemTab(int)  *eDat;      // data of eTab
  MemTab(int)  *iTab;      // size pNr; temp. contour (eg AB)
  MemTab(Fac3) *fTab;      // faces
  Fac3 *fnb;               // neigbour-faces
  Point2 *p2a;             // size pNr; points
  char *sap;               // size pNr; status points
  int *pNr;
               } FcNbP;



  int UFA_fnb_dump_1 (Fac3 *fac, Fac3 *fnb, char *txt, ...);


#define TYP_EDGLN_BL  1  // BreakLine
#define TYP_EDGLN_AB  2  // OuterBound - automatic created
#define TYP_EDGLN_IB  3  // InnerBound - user defined points
#define TYP_EDGLN_OB  4  // OuterBound - user defined points
#define TYP_EDGLN_IC  5  // InnerBound - computed points
#define TYP_EDGLN_OC  6  // OuterBound - computed points
#define TYP_EDGLN_FAC 7  // faces (eg from GLU)
#define TYP_EDGLN_LN  8  // lines (test)



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
#define UFA_psn_opp_esn(esn) ((esn) < 2 ? 3 : esn - 1) 


// get edgeNr opposit to pointNr; e2 <- p1; e3 <- p2; e1 <- p3; 
#define UFA_esn_opp_psn(esn) ((esn) > 2 ? 1 : esn + 1)


// get next esn; 1 -> 2; 2 -> 3; 3 -> 1.
#define UFA_esn_nxt(esn) ((esn) > 2 ? 1 : esn + 1)


// get previous esn; 1 -> 3; 2 -> 1; 3 -> 2.
#define UFA_esn_prv(esn) ((esn) < 2 ? 3 : esn - 1)


#define UFA_fNr_max(pNr) (pNr * 4)


// EOF
