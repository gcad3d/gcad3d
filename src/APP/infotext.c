// infotext.c                 RF                     2016-12-29
/*
 *
 * Copyright (C) 2015 CADCAM-Services Franz Reiter (franz.reiter@cadcam.co.at)
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
Contains infotext only, no code.
Use it with "vi -t <code>"

ReCreate tag-file with:
ctags -f ../tags/infotext.tag ../APP/infotext.c

*/


#ifdef globTag

void INF(){}


void INF_struct_dir (){/*
B-spline:
  dir  0=forward, curve along ascending parameters;
       1=backward, reverse; curve along descending parameters.
Circle:
  dir  0 = same direction as parentcurve
       1 = reverse direction as parentcurve

  v0/ip0/p1 always gives the startpoint (even if dir=bwd)
  v1/ip1/p2 always gives the endpoint (even if dir=bwd)

See UTO_cv_ck_dir_clo INF_struct_par
*/}


void INF_struct_par (){        /*
v0  the parameter of a point on the curve. Has values 0. - 1.

  B-spline:
    The value gives the position on the fwd-curve (also for bwd-curves).
    Ascending parameters always go fwd, along the control-points,
      even for bwd-curves.
    If curve is not closed: fwd-curve: v0 < v1; bwd-curve: v1 < v0.
      v0=0.5; v1=1.0; dir=0; - curve starts at midpoint and goes fwd to the end
      v0=1.0; v1=0.5; dir=1; - curve starts at endpoint and goes bwd to midpoint
    If curve is closed:
      v0=0.5; v1=0.5; dir=0; - curve fwd from midpoint to midpoint, tru endpoint
      v0=0.5; v1=0.5; dir=1; - curve bwd from midpoint to midpoint, tru endpoint

  Circle:
    Ascending parameters can go CCW or CW (dependent on rad)
    Val. 0. is always the startpoint (p1).
    Val. 0.5 is the midpoint; dependent on CCW or CW.
    Val. 1. is always the endpoint (p2).

  Ellipse:
    Ascending parameters can go CCW or CW (dependent on srot)
    Val. 0. is always the startpoint (p1).
    v0=0.25; v1=0.5; srot=0;   // CCW-ellipse; 2.quarter of curve.
    v0=0.25; v1=0.5; srot=1;   // CW-ellipse; 3.quarter of curve.

*/}


void INF_struct_closed (){        /*

closed: endpoint == startpoint
  - curve can be trimmed
  - char clo: 0=closed, 1=not_closed, -1=undefined
  - see UTO_cv_ck_dir_clo

cyclic: curve is passing endpoint=startpoint   
  - curve can be trimmed
  - closed curve from startpoint to endpoint is NOT cylic
  - see UTO_cv_ck_cyc

trimmed: if curve is closed: curve passes whole cycle or not  (char trm)
  -  not-trimmed: curve from startpoint to endpoint
  -  not-trimmed: curve from midpoint to midpoint
  -  char trm: 0=trimmed, 1=not_trimmed, -1=undef;


Funcs:
  clo   UTO_cv_ck_dir_clo
*/}


void INF_MSG_new (){        /*
Howto create new message:
  edit messagefiles (add new message):          ./lang_ed.csh

  find a keyword:      ./lang_find.csh keyWd 
  modify keyword       /mnt/serv1/Linux/bin/changeall old new files
  modify line          ./lang_line keyWd "line words .."
  delete Line          ./lang_del.csh keyWd
  insert Line          ./lang_ins.csh keyWd_before new line words ..
  save the lang.files: ./lang_save.csh
*/}

#endif
// eof
