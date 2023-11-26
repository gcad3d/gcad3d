# . ./devbase.sh && make -f gMsh1.mak


DLLNAM = plugins/gMsh1

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
 ../APP/gMsh1.c


#../myGIS1/Gis_u1.c\
#../myGIS1/GIS__.c\
#../APP/GIS1.c


DIRSRC1 := ../APP/

include gcad_dll.mak

########################### EOF #######################
