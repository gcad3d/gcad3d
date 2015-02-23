/* ../gui/gui__.h
 *
 * Copyright (C) 2012 CADCAM-Servies Franz Reiter (franz.reiter@cadcam.co.at)
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

-----------------------------------------------------
gui-includes


prerequisites:
#include "../ut/ut_types.h"
#include "../ut/ut_umem.h"

OR (gCAD3D only):
#include "../ut/ut_geo.h"

#include "../gui/gui__.h"              // GUI_*



*/
#ifdef globTag
void GUI(){}
#endif
/*!
\file ../gui/gui__.h
\ingroup grp_gui
\code
*/



#include "../gui/gui_types.h"              // TYP_GUI_*
#include "../gui/gui_base.h"               // Obj_Unknown


#ifdef gtk2
#include "../gui_gtk2/gui_gtk2.h"
#endif

#ifdef gtk3
#include "../gui_gtk3/gui_gtk3.h"
#endif

#ifdef gtk2_MS
#include "../gui_gtk2_MS/gui__.h"
#endif


  extern int GUI_FuncInit;
  extern int GUI_FuncUCB1;
  extern int GUI_FuncUCB2;
  extern int GUI_FuncUCB3;
  extern int GUI_FuncUCB4;
  extern int GUI_FuncUCB5;
  extern int GUI_FuncUCB6;
  extern int GUI_FuncUCB7;
  extern int GUI_FuncUCB8;
  extern int GUI_FuncUCB9;
  extern int GUI_FuncUCB10;
  extern int GUI_FuncUCB11;
  extern int GUI_FuncUCB12;
  extern int GUI_FuncUCB13;
  extern int GUI_FuncWork;
  extern int GUI_FuncExit;
  extern int GUI_FuncKill;


// EOF

