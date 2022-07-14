# . ../options.sh && make -f GIS1.mak


DLLNAM = plugins/GIS1

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
 ../myGIS1/Gis_u1.c\
 ../myGIS1/GIS__.c\
 ../APP/GIS1.c


DIRSRC1 := ../myGIS1/

include gcad_dll.mak

########################### EOF #######################
