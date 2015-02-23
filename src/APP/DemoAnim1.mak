# make -f DemoAnim1.mak  "OS=Linux32"

DLLNAM = plugins/DemoAnim1

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
 ../xa/DemoAnim1.c\

DIRSRC1 := ../xa/

include gcad_dll.mak

########################### EOF #######################
