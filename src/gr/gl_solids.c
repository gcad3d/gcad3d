//   gl_solids.c                            2005-05-30       RF.
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
TODO:
  ..

-----------------------------------------------------
Modifications:
  ..

-----------------------------------------------------
*/
/*!
\file  ../gr/gl_solids.c
\brief OpenGL: create solid surfaces symbolic or shaded
\code
=====================================================
List_functions_start:

GL_disp_sph          sphere
GL_disp_cone         cone
GL_disp_tor          torus
GL_disp_prism

List_functions_end:
=====================================================
- see also:
GR_CreSol

\endcode *//*----------------------------------------


 _disp_-Funktionen (ohne glNewList)
 mit BACKFACECULLING OFF (Innenseite nicht anzeigen)
 braucht Func BACKFACECULLING on/off


Man braucht fuer
 Schnitt mit komplexen Flaechen: Tesselieren;
   die einzelnen Dreiecke sind erforderlich;
 Export nach VRML: kein Tesselieren erforderlich.
   Sphere nicht, Cone nur wenn != cylinder; Torus immer.



*/


#ifdef _MSC_VER
#include "../xa/MS_Def1.h"
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>



#include "../ut/ut_geo.h"

#include "../gr/ut_GL.h"                 //
#include "../gr/ut_UI.h"                 // UI_Func... SYM_..

#include "../xa/xa_mem.h"      // memspc.


// aus NC_Main.c:
// extern int     APT_dispSOL;


// aus ../gr/ut_GLU.c:
extern Point     *GLT_pta;
extern long      GLT_pta_SIZ;


// aus ../gr/tess_su.c:
extern int TSU_mode;   // 0=normal darstellen; 1=speichern





//================================================================
  int GL_disp_sph (Point *ptc, double radius, int sStyl) {
//================================================================
// sStyl 0=shaded, 1=symbolic

  int     i1, ia, ih, iv, hNr, vNr, pSiz;
  // double  z0,z1, r0,r1, *sa, *ca, z0r, z1r, r0r, r1r;
  double  *sa, *ca, z1r, r1r;
  double  dx, dy, dz;
  Point   pc1, *pa1, *pa2, *pa3, *pa4, *pTab;


  // printf("GL_disp_sph %f styl=%d\n",radius,sStyl);


  // get memSpc
  pTab = (Point*)memspc201;
  pSiz = sizeof(memspc201) / sizeof(Point);


  dx = ptc->x;
  dy = ptc->y;
  dz = ptc->z;


  // Anzahl Ecken errechnen
  ih = UT2D_ptNr_ci (radius, RAD_360, UT_DISP_cv*2.);
  // printf(" ptNr_ci=%d\n",ih);
  
  //  Anzahl Scheiben in hor u vert - Richtung.
  // hNr = 24;  // 8 12 16 24 48 64
  hNr = ih - ih % 4;
  if(hNr < 8) hNr = 8;
  // vNr = hNr / 2; fuer alte Loseung ...
  vNr = hNr / 4;

  ++hNr; // erster u letzter Punkt am Kreis ident !!
  // printf(" hNr=%d vNr=%d\n",hNr,vNr);


  // memspc fuer sin u. cos-Reihe; Anz. hNr.
  i1 = hNr + (hNr * 4);
  // einmal f. sin u. cos-Reihe; hNr*hNr fuer alle Punkte
  if(pSiz < i1) goto L_EOM;


  // memspc fuer sin u. cos-Reihe; Anz. iNr.
  ia = 0;
  i1 = (hNr / 3) + 1;
  sa = (double*)&pTab[ia];  ia += i1;
  ca = (double*)&pTab[ia];  ia += i1;


  // die Punkte fuer den ersten Kreisring errechnen
  UTP_sincosTab_circ (sa, ca, hNr);




  // Solid oder Wireframe -
  if(sStyl == 0) goto L_sol;


  //======= disp Wireframe =========================================

  // memspc fuer die PunkteTablle.
  pa1 = &pTab[ia];    ia += hNr;
  pa2 = &pTab[ia];

  for(ih=0; ih<hNr; ++ih) {
    pa1[ih].x = ca[ih]*radius + dx;
    pa1[ih].y = sa[ih]*radius + dy;
		pa1[ih].z = dz;

    pa2[ih].x = pa1[ih].x;
    pa2[ih].y = dy;
		pa2[ih].z = pa1[ih].y - dy + dz;
  }

  glDisable (GL_LIGHTING);

  glBegin (GL_LINE_STRIP);
    for(ih=0; ih<hNr; ++ih) {
      glVertex3dv ((double*)&pa1[ih]);
    }
  glEnd ();

  glBegin (GL_LINE_STRIP);
    for(ih=0; ih<hNr; ++ih) {
      glVertex3dv ((double*)&pa2[ih]);
    }
  glEnd ();

  return 0;





  //======= disp solid =============================================
  L_sol:

/*
  z1 = ca[1]; z1r = z1 * radius;
  r1 = sa[1]; r1r = r1 * radius;
  // Deckel oben als GL_TRIANGLE_FAN
  glBegin(GL_TRIANGLE_FAN);

      glNormal3d(0,0,-1);
      glVertex3d(dx,dy,dz+radius);

      for (ih=hNr; ih>=0; ih--) {
          glNormal3d (-ca[ih]*r1,  -sa[ih]*r1,  -z1);
          glVertex3d ( ca[ih]*r1r+dx,  sa[ih]*r1r+dy,  z1r+dz);
      }

  glEnd();
*/

  pa1 = &pTab[ia];  ia += hNr;
  pa2 = &pTab[ia];  ia += hNr;
  pa3 = &pTab[ia];  ia += hNr;
  pa4 = &pTab[ia];  ia += hNr;


  // Top-Fan
  pc1.x = dx;
  pc1.y = dy;
  pc1.z = dz + radius;

  r1r = sa[1] * radius;
  z1r = ca[1] * radius;
  for (ih=0; ih<hNr; ++ih) {
    pa1[ih].x = dx + ca[ih] * r1r;
    pa1[ih].y = dy + sa[ih] * r1r;
    pa1[ih].z = dz + z1r;
  }
  GL_DrawFan (&pc1, hNr, pa1, 1, 0);

  // Bottom-Fan
  pc1.z = dz - radius;
  for (ih=0; ih<hNr; ++ih) {
    pa1[ih].z = dz - z1r;
  }
  GL_DrawFan (&pc1, hNr, pa1, 1, 0); // verkehrt !



/*
  // Cover each stack with a quad strip, except the top and bottom stacks
  for( iv=1; iv<vNr-1; iv++ ) {
      z0 = z1;       z0r = z1r;
      z1 = ca[iv+1]; z1r = z1 * radius;
      r0 = r1;       r0r = r1r;
      r1 = sa[iv+1]; r1r = r1 * radius;
      glBegin(GL_QUAD_STRIP);
          for(ih=0; ih<=hNr; ih++) {
              glNormal3d(-ca[ih]*r1, -sa[ih]*r1, -z1);
              glVertex3d(ca[ih]*r1r+dx,  sa[ih]*r1r+dy, z1r+dz);
              glNormal3d(-ca[ih]*r0, -sa[ih]*r0, -z0);
              glVertex3d(ca[ih]*r0r+dx,  sa[ih]*r0r+dy, z0r+dz);
          }
      glEnd();
  }
*/


  // die Mittelreihe nach pa1 und pa2
  for(ih=0; ih<hNr; ++ih) {
    pa1[ih].x = dx + ca[ih]*radius;
    pa1[ih].y = dy + sa[ih]*radius;
		pa1[ih].z = dz;
      // GL_Disp_symB (SYM_STAR_S, &pa1[ih]);
  }
  for(ih=0; ih<hNr; ++ih) pa2[ih] = pa1[ih];
  
  // Halbkugel errechnen; Punkte auf andere Halbkugel kopieren.
  for(iv=1; iv<vNr; ++iv) {
    r1r = ca[iv] * radius;
    z1r = sa[iv] * radius;
    // printf(" r1r=%f z1r=%f\n",r1r,z1r);
    for(ih=0; ih<hNr; ih++) {
      pa3[ih].x  = dx + ca[ih] * r1r;
      pa3[ih].y  = dy + sa[ih] * r1r;
      pa3[ih].z  = dz + z1r;
        // GL_Disp_symB (SYM_STAR_S, &pa3[ih]);

      // glVertex3d(ca[ih]*r0r+dx,  sa[ih]*r0r+dy, z0r+dz);
      pa4[ih].x  = pa3[ih].x;
      pa4[ih].y  = pa3[ih].y;
      pa4[ih].z  = dz - z1r;
     
    }
    GL_DrawStrip2 (pa1, pa3, hNr, 0);
    GL_DrawStrip2 (pa2, pa4, hNr, 0);

    // Adressen vertauschen ..
    MEM_swap__ (&pa1, &pa3, sizeof(void*));
    MEM_swap__ (&pa2, &pa4, sizeof(void*));

  }





/*
  // The bottom stack is covered with a triangle fan
  z1r = z1 * radius;
  r1r = r1 * radius;
  glBegin(GL_TRIANGLE_FAN);

      glNormal3d(0,0,1);
      glVertex3d(dx,dy,dz-radius);

      for (ih=0; ih<=hNr; ih++) {
          glNormal3d(-ca[ih]*r1, -sa[ih]*r1,  -z1);
          glVertex3d(ca[ih]*r1r+dx,  sa[ih]*r1r+dy,  z1r+dz);
      }

  glEnd();
*/


  return 0;


  L_EOM:
  TX_Error("GL_disp_sph EOM");
  return -1;

}



//=========================================================================
  int GL_disp_cone (Conus *con, int mode, int sStyl) {
//=========================================================================
// mode = 1+2+4 = 7 = all
//        1: circ. bottom-face
//        2: cyl. side
//        4: circ. top-face

//  // eine plane aus Point, Vector:
//  UT3D_pl_ptvc (&co1.pl, &pt1, &vc1);


  int     i1, ia, ih, hNr, pSiz;
  double  d1, d2, *sa, *ca;
  Point   pc1, pc2, *pa1, *pa2, *pTab;
  Mat_4x3 ma;


  // printf("GL_disp_cone %d r1=%f r2=%f h=%f\n",mode,con->r1,con->r2,con->h);
  // printf(" &1=%d &2=%d &3=%d &4=%d\n",mode&1, mode&2, mode&4, mode&8);

  // get memSpc
  pTab = (Point*)memspc201;
  pSiz = sizeof(memspc201) / sizeof(Point);


  // Anzahl Ecken errechnen
  d1 = DMAX(con->r1, con->r2);
  ih = UT2D_ptNr_ci (d1, RAD_360, UT_DISP_cv*2.);
    // printf(" ptNr_ci=%d\n",ih);

  hNr = ih - ih % 4;
  if(hNr < 6) hNr = 6;

  ++hNr; // erster u letzter Punkt am Kreis ident !!
    // printf(" hNr=%d\n",hNr);


  // die trMat laden
  UT3D_m3_loadpl (ma, &con->pl);


  // memspc fuer sin u. cos-Reihe; Anz. hNr.
  i1 = hNr + (hNr * 2);
  // einmal f. sin u. cos-Reihe; hNr+hNr fuer alle Punkte
  if(pSiz < i1) goto L_EOM;


  // memspc fuer sin u. cos-Reihe; Anz. iNr.
  ia = 0;
  i1 = (hNr / 3) + 1;
  sa = (double*)&pTab[ia];  ia += i1;
  ca = (double*)&pTab[ia];  ia += i1;


  // memspc fuer die PunkteTablle.
  pa1 = &pTab[ia];  ia += hNr;
  pa2 = &pTab[ia];  ia += hNr;


  // die sin/cos-Tabelle fuer den Kreisring errechnen
  UTP_sincosTab_circ (sa, ca, hNr);

  // Mittelpunkte pc1 (bottom), pc2 (top).
  UT3D_pt_3db (&pc1, 0., 0., 0.);
  UT3D_pt_3db (&pc2, 0., 0., con->h);
  UT3D_pt_traptm3 (&pc1, ma, &pc1);
  UT3D_pt_traptm3 (&pc2, ma, &pc2);


  // PunkteTabelle bottom und top befuellen und transformieren
  // glBegin (GL_POINTS);  // TEST
  d1 = con->r1;
  d2 = con->r2;
  for (i1=0; i1<hNr; i1++) {
    pa1[i1].x = ca[i1]*d1;
    pa1[i1].y = sa[i1]*d1;
    pa1[i1].z = 0.;

    pa2[i1].x = ca[i1]*d2;
    pa2[i1].y = sa[i1]*d2;
    pa2[i1].z = con->h;

    UT3D_pt_traptm3 (&pa1[i1], ma, &pa1[i1]);
    UT3D_pt_traptm3 (&pa2[i1], ma, &pa2[i1]);
    // glVertex3dv ((double*)&pa1[i1]);
    // glVertex3dv ((double*)&pa2[i1]);
  }
  // glEnd ();    // TEST
  // return 0;



  // Solid oder Wireframe -
  if(sStyl == 0) goto L_sol;


  //======= disp Wireframe =========================================

  glDisable (GL_LIGHTING);

  glBegin (GL_LINE_STRIP);
    for(ih=0; ih<hNr; ++ih) {
      glVertex3dv ((double*)&pa1[ih]);
    }


    for(ih=0; ih<hNr; ++ih) {
      glVertex3dv ((double*)&pa2[ih]);
    }
  glEnd ();

  return 0;





  //======= disp solid =============================================
  L_sol:
    // UT3D_stru_dump (Typ_PT, &pc1, "  pc1:");
    // for(i1=0;i1<hNr;++i1)UT3D_stru_dump (Typ_PT,&pa1[i1], "  pa1[%d]:",i1);
    // UT3D_stru_dump (Typ_PT, &pc2, "  pc2:");
    // for(i1=0;i1<hNr;++i1)UT3D_stru_dump (Typ_PT,&pa2[i1], "  pa2[%d]:",i1);


  //----------------------------------------------------------------
  if((mode & 1) == 0) goto L_bot;  // if bit1 nicht gesetzt: skip.
      // printf(" disp fan pc1, pa1\n");
    GL_DrawFan (&pc1, hNr, pa1, 0, 1); // GL_TRIANGLE_FAN


  //----------------------------------------------------------------
  L_bot:
  if((mode & 4) == 0) goto L_side;    // if bit3 nicht gesetzt: skip
      // printf(" disp fan pc2, pa2\n");
    GL_DrawFan (&pc2, hNr, pa2, 0, 1); // GL_TRIANGLE_FAN


  //----------------------------------------------------------------
  L_side:
  if((mode & 2) == 0) goto L_done;  // if bit2 nicht gesetzt: skip
      // printf(" disp strip pa1, pa2\n");
    // Mantel
    if(UTP_comp2db(con->r1, 0., UT_TOL_cv)) {
        // printf(" r1 is 0\n");
      GL_DrawFan (&pc1, hNr, pa2, 1, 1); // GL_TRIANGLE_FAN
      goto L_done;
    }

    if(UTP_comp2db(con->r2, 0., UT_TOL_cv)) {
        // printf(" r2 is 0\n");
      GL_DrawFan (&pc2, hNr, pa1, 1, 1); // GL_TRIANGLE_FAN
      goto L_done;
    }


    GLT_stor_rec (6, NULL, NULL, Typ_SURCON);
    GL_DrawStrip2 (pa1, pa2, hNr, 1);





  //----------------------------------------------------------------
  L_done:
  return 0;


  L_EOM:
  TX_Error("GL_disp_con EOM");
  return -1;
}


//=========================================================================
  int GL_disp_tor (Torus *tor, int sStyl) {
//=========================================================================


  int     i1, ia, ih, iv, hNr, vNr, pSiz;
  double  r1, r2, *sah, *sav, *cah, *cav;
  double  dh, dv;
  Point   *pa1, *pa2, *pTab;
  Mat_4x3 ma;


  // printf("GL_disp_tor %f %f\n",tor->r1,tor->r2);


  // get memSpc
  pTab = (Point*)memspc501;
  pSiz = sizeof(memspc501) / sizeof(Point);


  r1 = tor->r1 - tor->r2;
  r2 = tor->r2;

  // der Torus liegt im Nullpunkt um die Z-Achse. Die grossen Ringe sind also
  // horizontal, die kleinen vertikal.


  // Anzahl Ecken errechnen
  ih = UT2D_ptNr_ci (r1, RAD_360, UT_DISP_cv*2.); // vom groessten Radius
  // printf(" ptNr_ci=%d\n",ih);
  hNr = ih - ih % 4;
  if(hNr < 6) hNr = 6;
  ++hNr; // erster u letzter Punkt am Kreis ident !!
  // printf(" hNr=%d\n",hNr);


  iv = UT2D_ptNr_ci (r2, RAD_360, UT_DISP_cv*2.); // vom Querschnitt
  // printf(" ptNr_ci=%d\n",iv);
  vNr = iv - iv % 4;
  if(vNr < 6) vNr = 6;
  ++vNr; // erster u letzter Punkt am Kreis ident !!
  // printf(" vNr=%d\n",vNr);


  // die trMat laden
  UT3D_m3_loadpl (ma, &tor->pl);


  // memspc fuer sin u. cos-Reihe; Anz. hNr.
  ih = vNr + hNr + hNr + hNr + (hNr * vNr);
  // hNr: 1mal fuer sax,cax;  hNr mal ivNr fuer alle Punkte
  if(pSiz < ih) goto L_EOM;


  ih = (hNr / 3) + 1;
  i1 = 0;
  sah = (double*)&pTab[i1];  i1 += ih;
  cah = (double*)&pTab[i1];  i1 += ih;

  iv = (vNr / 3) + 1;
  sav = (double*)&pTab[i1];  i1 += iv;
  cav = (double*)&pTab[i1];  i1 += iv;


  // die sin/cos-Tabelle fuer den Kreisring errechnen
  UTP_sincosTab_circ (sah, cah, hNr);
  UTP_sincosTab_circ (sav, cav, vNr);





  //======= disp Wireframe =========================================
  // Solid oder Wireframe -
  if(TSU_mode != 0) goto L_sol;
  if(sStyl == 0) goto L_sol;

  pa1 = &pTab[i1]; // Adresse naechste Punktereihe

  glDisable (GL_LIGHTING);

  glBegin (GL_LINE_STRIP);
    dh = r1 - r2;
    for (ih=0; ih<hNr; ih++) {
      pa1[ih].x = cah[ih] * dh;
      pa1[ih].y = sah[ih] * dh;
      pa1[ih].z = 0.;
      UT3D_pt_traptm3 (&pa1[ih], ma, &pa1[ih]);
        // GR_Disp_pt (&pa1[ih], SYM_STAR_S, 2);
      glVertex3dv ((double*)&pa1[ih]);
    }
  glEnd ();

  glBegin (GL_LINE_STRIP);
    dh = r1 + r2;
    for (ih=0; ih<hNr; ih++) {
      pa1[ih].x = cah[ih] * dh;
      pa1[ih].y = sah[ih] * dh;
      pa1[ih].z = 0.;
      UT3D_pt_traptm3 (&pa1[ih], ma, &pa1[ih]);
        // GR_Disp_pt (&pa1[ih], SYM_STAR_S, 2);
      glVertex3dv ((double*)&pa1[ih]);
    }
  glEnd ();

  glBegin (GL_LINE_STRIP);
    dh = cah[0] * r1;
    dv = r2;
    for (iv=0; iv<vNr; iv++) {
      pa1[iv].x = dh + (cav[iv] * dv);
      pa1[iv].y = 0.;
      pa1[iv].z = sav[iv] * dv;
      UT3D_pt_traptm3 (&pa1[iv], ma, &pa1[iv]);
        // GR_Disp_pt (&pa1[iv], SYM_STAR_S, 2);
      glVertex3dv ((double*)&pa1[iv]);
    }
  glEnd ();

  goto L_done;








  //======= disp solid =============================================
  L_sol:

  // die radien ..
  dh = r1;
  dv = r2;
  // printf(" dh=%f dv=%f\n",dh,dv);

  // alle streifen errechnen; man koennte auf Viertelkreis reduzieren und
  // die anderen 3 Viertel kopieren
  ia = i1;
  pa1 = &pTab[i1]; // Adresse naechste Punktereihe
  for (iv=1; iv<vNr; iv++) {  // skip erste Reihe; gleich mit letzter.
    for (ih=0; ih<hNr; ih++) {
      pa1[ih].x = cah[ih] * (dh + cav[iv] * dv);
      pa1[ih].y = sah[ih] * (dh + cav[iv] * dv);
      pa1[ih].z = sav[iv] * dv;
      UT3D_pt_traptm3 (&pa1[ih], ma, &pa1[ih]);
        // GR_Disp_pt (&pa1[ih], SYM_STAR_S, 2);
    }
    i1 += hNr;
    pa1 = &pTab[i1];
  }


  // ausgeben;
  pa2 = &pTab[i1-hNr];  // mit letzter Reihe starten ..
  i1 = ia;
  for (iv=1; iv<vNr; iv++) {
    pa1 = pa2;
    pa2 = &pTab[i1];
    i1 += hNr;
    GL_DrawStrip2 (pa1, pa2, hNr, 0);
  }



  //----------------------------------------------------------------
  L_done:
  return 0;

  L_EOM:
  TX_Error("GL_disp_tor EOM");
  return -1;

}


//===========================================================================
  int GL_disp_prism (int p1Nr, Point *pa1, int p2Nr, Point *pa2,
                     int sStyl) {
//===========================================================================
// sStyl 0=shaded, 1=symbolic

  int     i1, iCon;

  // printf("GL_disp_prism %d %d\n",p1Nr,p2Nr);
  // for(i1=0; i1<p2Nr; ++i1) {
   // UT3D_stru_dump(Typ_PT, &pa1[i1], "1-%d",i1);
   // UT3D_stru_dump(Typ_PT, &pa2[i1], "2-%d",i1); }



  //================================================================
  // disp Wireframe
  if(sStyl == 0) goto L_sol;

  glDisable (GL_LIGHTING);


  glBegin (GL_LINE_STRIP);

    // 1. Deckflaeche
    for(i1=0; i1<p1Nr; ++i1) {
      glVertex3dv ((double*)&pa1[i1]);
    }

    // hier wird automat 1. Verbindungslinie generiert !

    // 2. Deckflaeche
    for(i1=0; i1<p2Nr; ++i1) {
      glVertex3dv ((double*)&pa2[i1]);
    }

  glEnd ();


  // 2. Verbindungslinie
  glBegin (GL_LINE_STRIP);
    i1 = p1Nr / 2;
    glVertex3dv ((double*)&pa1[i1]);
    i1 = p2Nr / 2;
    glVertex3dv ((double*)&pa2[i1]);
  glEnd ();


  glEnable (GL_LIGHTING);


  goto L_done;





  //================================================================
  // disp Solid
  L_sol:


  //----------------------------------------------------------------
  // generieren Mantelflaeche
  // Konus oder normales prisma -
  if(p2Nr < 2) {                 // Konische Mantelflaeche

    GR_DrawFan (pa2, p1Nr, pa1, 1, Typ_SUR);
    goto L_done;                 // keine Deckflaeche mehr


  //..................................................................
  } else {                       // normale Prismatische Mantelflaeche
    GR_DrawStrip (pa1, pa2, p1Nr, 1);
  }



  //----------------------------------------------------------------
  // check if is contour convex or concave
  iCon = UT3D_isConvx_ptTab (p1Nr, pa1);    // 1=Konvex; -1=Konkav
    // printf(" iCon=%d\n",iCon);

  // generieren Basisflaeche
  // MODIFIES pa1;
  GR_DrawSup (p1Nr, pa1, iCon);

  // generieren Deckflaeche
  GR_DrawSup (p2Nr, pa2, iCon);





  //================================================================
  L_done:
  return 0;

}

 
/*
//======================================================================
  int GL_disp_box (Point *pt1, Vector *vcx, Vector *vcy, Vector *vcz) {
//======================================================================
// see also GR_Disp_box

  Point   pt2, pt3, pt4, pt5, pt6, pt7, pt8;


    UT3D_pt_traptvc (&pt2, pt1,  vcx);
    UT3D_pt_traptvc (&pt3, &pt2, vcy);
    UT3D_pt_traptvc (&pt4, pt1,  vcy);

    UT3D_pt_traptvc (&pt5, pt1,  vcz);
    UT3D_pt_traptvc (&pt6, &pt2, vcz);
    UT3D_pt_traptvc (&pt7, &pt3, vcz);
    UT3D_pt_traptvc (&pt8, &pt4, vcz);

  glBegin( GL_QUADS );

    glNormal3dv ((double*)vcz);
    glVertex3dv ((double*)pt1);
    glVertex3dv ((double*)&pt2);
    glVertex3dv ((double*)&pt3);
    glVertex3dv ((double*)&pt4);

    glNormal3dv ((double*)vcz);
    glVertex3dv ((double*)&pt5);
    glVertex3dv ((double*)&pt6);
    glVertex3dv ((double*)&pt7);
    glVertex3dv ((double*)&pt8);

    glNormal3dv ((double*)vcy);
    glVertex3dv ((double*)pt1);
    glVertex3dv ((double*)&pt5);
    glVertex3dv ((double*)&pt6);
    glVertex3dv ((double*)&pt2);

    glNormal3dv ((double*)vcy);
    glVertex3dv ((double*)&pt3);
    glVertex3dv ((double*)&pt4);
    glVertex3dv ((double*)&pt8);
    glVertex3dv ((double*)&pt7);

    glNormal3dv ((double*)vcz);
    glVertex3dv ((double*)pt1);
    glVertex3dv ((double*)&pt5);
    glVertex3dv ((double*)&pt8);
    glVertex3dv ((double*)&pt4);

    glNormal3dv ((double*)vcz);
    glVertex3dv ((double*)&pt2);
    glVertex3dv ((double*)&pt3);
    glVertex3dv ((double*)&pt7);
    glVertex3dv ((double*)&pt6);

  glEnd();


  return 0;

}
*/

//====================== EOF =============================
