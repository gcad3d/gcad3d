/* ../inf/inf_gis1.c

================================================================== */
void INF_GIS1__(){                   /*! \code
see INF_GIS1_TODO1


Files:
../../doc/html/APP_GIS1_en.htm
GIS1.mak
../myAPPS/GIS1.c
-Kunden/Mader/gcad3d_gMsh.c
-devel/cadfiles/gcad/GIS1

//----------------------------------------------------------------
Import_Points:
../myAPPS/GIS1.c
gis_init_UI
  GUI_button__ (&w2, "Import Points from File",
                        gis_CB1, (void*)"opePtab", "");

  GUI_button__ (&w2, "Export Points",
                        gis_CB1, (void*)"savPtab", "");

  GUI_button__ (&w2, "move all points",
                        gis_CB1, (void*)"movPts", "");

  GUI_button__ (&w2, "delete range of points",
                        gis_CB1, (void*)"delPtR", "");

  GUI_button__ (&w2, "dump all points",
                        gis_CB1, (void*)"dumpPt", "");

//----------------------------------------------------------------
Import_EdgeLines:
gis_etab_load        import edgelinefile



================================================================== \endcode */}
INF_GIS1_TODO1(){                   /*! \code

- make plugin import pointfile from opePtab
 
//----------------------------------------------------------------
- add PTAB / MSH with lev = -1 into LoadMAP, create new types;
- Load pTAB / MSH : change used-bit in LoadMAP;
- save: get names of used PTAB/MSH out of LoadMAP,



================================================================== \endcode */}
// eof
