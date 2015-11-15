
#define TYP_EDGLN_BL  2  // BreakLine
#define TYP_EDGLN_IB  3  // InnerBound
#define TYP_EDGLN_OB  4  // OuterBound - user defined
#define TYP_EDGLN_AB  5  // OuterBound - automatic created
#define TYP_EDGLN_FAC 6  // faces (eg from GLU)
#define TYP_EDGLN_LN  7  // lines (test)

#define UFA_esn_opp_psn(esn) ((esn) < 3 ? ++esn : 1)
#define UFA_esn_nxt(esn) ((esn) < 3 ? ++esn : 1)
#define UFA_esn_prv(esn) ((esn) < 2 ? 3 : --esn)


// EOF
