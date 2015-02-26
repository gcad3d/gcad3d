/* hello-world.c         C-KIT-GUI-GTK - Demoprog.

Compile, link, start with:
Link with Gtk2:
gcc `pkg-config --cflags gtk+-2.0` hello-world.c `pkg-config --libs gtk+-2.0` ~/binLinux32/libgui_gtk-2.so

Start:
export LD_LIBRARY_PATH=~/binLinux32 && ./a.out
./a.out

Link with Gtk3:
gcc `pkg-config --cflags gtk+-3.0` hello-world.c gtk-toolbox.a `pkg-config --libs gtk+-3.0`


Link with make-script:
make run -f hello-world.mak "OS=Linux32"


*//*!
\file ../gui/hello-world.c
\ingroup grp_exa_gui
*//*

*/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_umem.h"
#include "../gui/gui__.h"





//================================================================
  int callback_button1 (void *parent, void **data) {
//================================================================
// called when button is clicked ..
 
  printf("callback_button1\n");

  return 1;

}


//================================================================
  int main (int argc, char *argv[]) {
//================================================================

  MemObj window, boxv0;     // the widgets

  // init
  GUI_Init__ ("");


  // Create Mainwindow
  window = GUI_Win__ ("Gtk-Toolbox test", GUI_main_quit, "");

  // get a vertical box in window
  boxv0 = GUI_box_v (&window, "");

  // button in box
  GUI_button__ (&boxv0, "press me", callback_button1, NULL, "");

  // windowSetup finished; display it ..
  GUI_Win_go (&window);

  return 0;
}


// EOF
