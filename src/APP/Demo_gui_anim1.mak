# make -f DemoAnim1.mak  "OS=Linux32"

DLLNAM = plugins/Demo_gui_anim1

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
 ../xa/Demo_gui_anim1.c\

DIRSRC1 := ../xa/

include gcad_dll.mak

########################### EOF #######################
