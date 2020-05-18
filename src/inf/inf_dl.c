/* ../inf/inf_dl.c

The Displist is GR_ObjTab.
For each visible obj one DL-record is necessary (see typ DL_Att)



================================================================== */
void INF_DL__ (){        /*! \code

Layout GR_ObjTab = displayList:
startIndex        used for -
 0                 temp. objects
 DL_base_font1     Fontcharacters (32-128, polygons)
 DL_Ind_ScBack     index of the scaleback-record 
 DL_base_LnAtt     attributes for curves
 DL_base_PtAtt
 DL_base_font2
 DL_Ind_Scl2D
 DL_Ind_Cen2D
 DL_Img_ScBack
 DL_OnTop__
 DL_OnTopOff
 DL_base_mod       first index of BasModelnames
 DL_base__         normal objects

 GR_TAB_IND        next free index in GR_ObjTab
 DL_temp_nxt        next free temp-index


INF_DisplayList-record
INF_Temporary-DisplayList-record


INF_DL__         DisplayList-functions                             ../gr/ut_DL.c

DL_dli__dbo      get DisplistIndex from DB-obj (typ,dbi)
DL_hili_on       set obj hilited
DL_Get_lNr_dli   get source-lineNumber from DispListIndex

DL_grp1__        add to / remove obj from group


================================================================== \endcode */}
// eof
