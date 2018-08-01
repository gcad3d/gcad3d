# . ../options.sh && make -f DemoAnim1.mak

DLLNAM = plugins/Demo_gui_anim1

# die hier befindlichen  Sourcen - recompile!
SRC1 =\
 ../APP/Demo_gui_anim1.c\

DIRSRC1 := ../APP/

include gcad_dll.mak

########################### EOF #######################
