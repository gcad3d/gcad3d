enum {
  ICO_Var,  ICO_VC,    ICO_PT,     ICO_LN,    ICO_CI,             // 0-4
  ICO_CV,   ICO_PLN,   ICO_SUR,    ICO_SOL,   ICO_Img,            // 5-9
  ICO_refM, ICO_natML, ICO_extM,   ICO_libM,  ICO_data,           // 10-14
  ICO_link, ICO_proc,  ICO_natMA,  ICO_natMU, ICO_natM0           // 15-
};

// stat 0 ICO_natM0  red       19    unused-model             mdlStat_unused   -1
//      1 ICO_natMU  gray      18    unused-in-primay-model   mdlStat_passive   0
//      2 ICO_natML  white     11    used-in-primay-model     mdlStat_active    1
//      - ICO_natMA  green     17    primary|main-model       mdlStat_primary   2
// EOF
