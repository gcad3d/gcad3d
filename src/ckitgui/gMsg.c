/*!
\file ../ckitgui/gMsg.c
\brief simple window with button
 *
 * Copyright (C) 2016 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
-----------------------------------------------------
TODO:
  ..

-----------------------------------------------------
Modifications:
  ..

-----------------------------------------------------


Compile, link, start with:
make run -f gMsg.mak

Start:
export LD_LIBRARY_PATH=~/gCAD3D/binLinux64 && ~/gCAD3D/binLinux64/gMsg


Kill with:
pkill gMsg
wmctrl -c gMsg-Info

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ut/ut_umem.h"
#include "../gui/gui__.h"





//================================================================
  int main (int argc, char *argv[]) {
//================================================================

  MemObj window, boxv0;     // the widgets

  if(argc < 2) {printf("***** no parameter (message-text).\n"); return -1;}


  // init
  GUI_Init__ ("");


  // Create Mainwindow
  window = GUI_Win__ ("gMsg-Info", GUI_main_quit, "");

  // get a vertical box in window
  boxv0 = GUI_box_v (&window, "");

  GUI_spc__ (&boxv0,  1, 10);

  // button in box
  // GUI_label_htm__ (&boxv0, "<b>  Text .. </b>", "");
  GUI_label_htm__ (&boxv0, argv[1], "");

  GUI_spc__ (&boxv0,  0, 100);
  GUI_spc__ (&boxv0,  1, 10);

  // windowSetup finished; display it ..
  GUI_Win_go (&window);

  GUI_Win_up (NULL, &window, 1);

  return 0;
}


// EOF
