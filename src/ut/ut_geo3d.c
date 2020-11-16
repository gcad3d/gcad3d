//**************************************************************************
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
alle UT3D_ci auslagern -> ut_circ1.c
remove ObjG2, ObjG
polygon (_pta_ _cv_) auslagern nach ut_npt (UT3D_cv_..)

-----------------------------------------------------
Modifications:
2003-05-23 UT3D_vc_tng_ci_pt statt UT3D_vc_cistart. RF.
2003-04-13 Neu: UT3D_vc_perp1vc UT3D_pl_plcoe UT3D_plcoe_pl
2003-03-28 Bei mehreren 3D_pl_ war p nicht belegt. RF.
2003-03-23 UT3D_pt_projptln, UT3D_pt_projptptvc: neuer Par. len zu !! RF.
2002-04-21 UT3D_m3_load_povxvz,UT3D_pt_rotptptvcangr neu. RF.
2002-03-20 UT2D_ci.. zu. RF.
2002-02-12 UT3D_angr_2vc__: UTP_comp_0 -> fabs. RF.
2001-10-16 UT3D_ci_obj2 u. UT3D_ci_obj: vz zu. RF.
2001-04-22 UT3D_vc_cistart,UT3D_pt_traptptlen neu zu.
2001-04-21 UT3D_vc_tra_vc_m3 korr.
2001-04-19 UT3D_vc_tra_vc_m3,UT3D_pl_XYZ neu zu.
2001-04-18 Unterschiede zu HNT:\wc3\wincut_ut_geo.cpp:
           UT3D_vc_crossprod2vc; ident mit UT3D_vc_perp2vc

-----------------------------------------------------
*/
#ifdef globTag
void UT3D(){}
#endif
/*!
\file  ../ut/ut_geo3d.c
       ../ut/ut_geo.h
\brief 3D geometric point vector line circle 
\code
=====================================================
List_functions_start:

DEB_dump_obj__            dump geom. element (2D or 3D) (../ut/ut_dump.c)

-------------- sense_of_rotation -------------------------------------
UT3D_sr_polc              sense_of_rotation of a closed polygon
UT3D_sr_npt_bp            get senseOfRotation for n points on backplane bp
UT3D_sr_el                get sense_of_rotation of a ellipse
UT3D_sr_ci                get sense_of_rotation of a circ                    INLINE
UT3D_sr_rev_obj           get rotSense from rotSense and reverse-flag

-------------- side -------------------------------------
UT3D_sid_2vc              check vc's for perp, same or opposit direction
UT3D_sid_3pt              check if pt is in, before or behind perpendic.Plane
UT3D_sid_pt_pt2vc         check if point is above or below plane from pt0,vx,vy
UT3D_sid_ptpl             check if pt is in, above or below plane
UT3D_sid_ptptvc           compare if pt is in, above or below plane

-------------- angles -------------------------------------
UT3D_acos_2vc             cos of opening angle of 2 vecs (dot=scalarprod)    INLINE
UT3D_acos_vc2pt           cos of opening angle of vc-ptpt (dot=scalarprod)   INLINE
UT3D_angr_2vc__           angle between two vec's (always 0 <= PI)
UT3D_angr_2vc_n           angle between two normalized vec's (0 <= PI)
UT3D_angr_3pt             angle between 3 points
UT3D_angr_3vc__           angle between 2 vectors; vz gives direction (pos|neg)
UT3D_angr_3vcn_CCW        angle between 2 vectors CCW around vz; all normalized
UT3D_angr_4pt             angle between 2 lines
UT3D_angr_ci_par1         get opening-angle from parameter 0-1
UT3D_angr_ci_p1_pt        opening angle of point on Circ
UT3D_angr_vc2pt           angle between 2 points in plane (plane from pt-vc)
UT3D_angr_vc2vc           compute angle of vec in Refsys (to X-axis)
UT3D_angr_vcpl_z          compute ang. of vec in Refsys (to X-axis, around Z-axis)
UT3D_angr_vcpl_tilt       compute angle of vec in Refsys (to XY-plane)
UT3D_angr_2pl             opening angle of 2 planes
UT3D_angrR_vc             get rotation-angle for vector
UT3D_angrT_vc             get tilt-angle for vector
UT3D_2angr_2vc            get transf.angles for a refSys from its X-and Y-axis
UT3D_2angr_vc__           2 opening angles of vec (Y-ang = titlt, Z-ang = rot)
// UT3D_2angr_vc_pl  UNFINISHED 2 angles of vec relativ in coordinate-system (vx,vz)
UT3D_3angr_2vc            3 opening angles (3D-position) from X and Z-vec
UT3D_atan_vcpl            Anstieg (tan) der Geraden vci bezueglich Ebene pli
UT_RADIANS                radians from degrees
UT_DEGREES                degrees from radians

-------------- length, parameter -------------------------------------
UT3D_len_vc               Vectorlength                    INLINE
UT3D_lenq_vc              Quadr.Vectorlength              INLINE
UT3D_bplen_vc             returns longest vectorpart and its backPlane
UT3D_lenB_vc              returns longest vectorpart
UT3D_lenB_2pt             longest dx/dy/dz-distance point-point  (fast!)
UT3D_len_2pt              distance point - point
UT3D_lenq_PtPt            square-distance point - point
UT3D_lenq_PtLn            minimum (quadr) distance Point-Segment
UT3D_minLen_3pt           query min dist of 3 points
UT3D_minLen_4pt           query min dist of 4 points
UT3D_minLen_npt           query min dist of n points
UT3D_len_ln               length of line
UT3D_len_ci               length of circ
UT3D_slen_3pt             signed distance point - point
UT3D_slen_2ptvc           signed Laenge pt-pt in Richtung vec (m. Vorzeichen !)
UT3D_slen_projvcvc        Proj.laenge eines Vektors auf einen anderen Vektor
UT3D_slenq_2ptvc          Quadr. Laenge pt-pt in Richtung vec (m. Vorzeichen !)
UT3D_slenq_ptptvc         signed quadr.Distance pt-pt in Richtung vec
UT3D_slenq_projvcvc       Quadr.laenge eines Vektors auf einen anderen Vektor
UT3D_slen_ptpl            signed distance point - plane
UT3D_nlen_2ptvc           Laenge der Normalen auf Vektor
UT3D_nlen_projvcvc        Normalabstand eines Vektors auf einen anderen Vektor
UT3D_nlenq_3pt            give quadr.Distance from point to line
UT3D_nlenq_2ptvc          give quadr.Distance from point to line
UT3D_nlenq_2vc            give quadr.Distance from point to line
UT3D_nlen_3pt             Laenge der Normalen von p2 auf Line p1-p3

UT3D_par_ck_inObj__       check if parameter is inside range us-ue & check points
UT3D_par_ck_inObj_del     delete all points & parameters not on obj
UT3D_parvc_2vc            parameter of distance of vec projected on vec
UT3D_parvc_2vcbp          parameter of distance of vec projected on vec
UT3D_2par_vc_vcx_vcy      get parameter of vector projected onto vx,vy
UT3D_2parvc_3vcbp         project end of vec1 along vec2 on vec3
UT3D_parpt_ln__           parameter of distance of point ptx on line
UT3D_par_pt_2pt           get parameter of point on line
UT3D_parpt_ptvc           parameter of distance of point ptx on line pt1-vc1
UT3D_parpt_lnbp           parameter of point on line (via 2D,BackPlane)
UT3D_par1_ci_angr         get parameter 0-1 for circ from opening-angle
UT3D_par1_parplg          ref
UT3D_par1_parbsp          ref
UT3D_par1_par_rbsp        ref
UT3D_park__par1_clot      ref
UT3D_par_pt__pt_prj_ln    parameter and point of point projected onto line
UT3D_par_pt__pt_prj_ci    get parameter and point of point projected on circ
UT3D_par_pt__pt_prj_plg   get parameter / point of nearest point on polygon
UT3D_par_pt__pt_prj_cv    get parameter and point of point projected onto curve

-------------- points --------------------------------------
UT3D_swap2pt              swap 2 points                               INLINE
UT3D_comp2pt              compare 2 points with tol.                  INLINE
UT3D_ck2D_equ_2pt         2D-compare of 2 3D-points with tolerance    INLINE
UT3D_comp4pt              compare 4 points
UT3D_pt_ck_npt            compare point / points
UT3D_pt_ck_ptvc           check if point is on unlimited line (point-vector)
UT3D_pt_ck_2pt            check if point is on unlimited line (2 points)
UT3D_pt_ck_inLine         check point on line segment
UT3D_pt_ck_onLine         check point on line segment
UT3D_pt_ck_onLnSeg        check point on line segment (for polygons)
UT3D_pt_ck_perp_2pt       check if 3 points are normal (perpendic.)
UT3D_pt_ck_mid_2pt        check if point is on perp.plane between 2 points
UT3D_pt_ck_on_pta         check if point lies ON polygon
UT3D_pt_ck_in2pt          check if point p3 is inside box of points p1-p2
UT3D_pt_ck_inCirc         check if point is on circ segment
UT3D_pt_ck_inSph          check if point is inside sphere
UT3D_pt_ck_linear         check if points are linear
UT3D_ipt_cknear_3pt       return index of nearest Point from 3 points
UT3D_ipt_cknear_npt       return index of nearest Point from n points
UT3D_ipt_ckfar_npt        return index of farest Point from n points
UT3D_ipt_cknearn_npt      return index of n-nearest Point from n points
UT3D_ipt2_nptvc           ind. of outermost points in direction vc of n pt's
..                        point inside polygon: see UT2D_pt_ck_inCv3
UT3D_pt_isFree            check if pt is empty (free - UT3D_pt_init)   INLINE
UT3D_ipt2_npt             find the indices of the most extreme points
UT3D_isConvx_ptTab        check if is contour convex or concave
UT3D_pt_ln_lim_del        remove points outside limited line
UT3D_pt_ci_lim_del        remove points outside limited circle

UT3D_pt_NEW               create empty point (UT3D_pt_isFree)          INLINE
UT3D_pt_pt2               3D-Point = 2D-Point; z = 0.
UT3D_pt_pt2_0             3D-Point = 2D-Point; z = 0.                  INLINE
UT3D_pt_pt2_z             3D-Point = 2D-Point + z-coord                INLINE
UT3D_pt_3db               Point = 3 doubles (x, y, z)
UT3D_pt_vc                copy
UT3D_pt_txt               Point from text
UT3D_pt_tra_pt2_bp             3D-point from 2D-point & backplane
UT3D_pt_addpt             Add two points:      po += p1                INLINE
UT3D_pt_add_pt2           add 2D-point         po += p1                INLINE
UT3D_pt_add2pt            Add two points:      po = p1 + p2
UT3D_pt_add_vc__          add vector:          pt += vc                INLINE
UT3D_pt_add_vc_rev        add vector revers    pt -= vc                INLINE
UT3D_pt_add_vc_par        add (vector * lpar)  po += (vc * lpar)       INLINE
UT3D_pt_sub_pt2           subtract 2D-point                            INLINE
UT3D_pt_multpt            multiply; po = pi * d
UT3D_pt_LinComb2Pts       Linear combination of 2 points: po = a1 * p1 + a2 * p2
UT3D_pt_mid2pt            midpoint between 2 points
UT3D_pt_mid_pta           arithm. Mittelwert der Punktetabelle pTab
UT3D_pt_mid_ci            midpoint of a circ
UT3D_pt_std_ci            90/180/270-deg-point of circ
UT3D_pt_opp2pt            opposite point (p1 = center)
UT3D_pt_oppptptvc         point opposite line (= 180 deg rot.)
UT3D_2pt_oppptvclen       2 opposite points (center, vector, dist)
UT3D_pt_tra_pt_dx         Point = Point + dx                  (INLINE)
UT3D_pt_tra_pt_dy         Point = Point + dy                  (INLINE)
UT3D_pt_traptvc           Point = Point + Vector              (INLINE)
UT3D_pt_traptivc          Point = Point - Vector              (INLINE)
UT3D_pt_traptmultvc       point = point + (vector * factor)   (INLINE)
UT3D_pt_tra_pt_pt_par    point = segment p1-p2 * fakt
UT3D_pt_traptvclen        transl. point into dir vc dist. lenv
UT3D_pt_tra_pt_vc_par     transl. point into dir vc dist. lenv (vcLen=1)
UT3D_pt_trapt2vc          transl. point into 2 directions
UT3D_pt_tra_pt_2vc_par    transl. point into 2 directions (multiply vec)
UT3D_pt_tra_pt_2vc_2par   transl. point into 2 directions
UT3D_pt_trapt2vc2len      transl. point into 2 directions (2 len's)
UT3D_pt_trapt3vc3len      transl. point into 3 directions (3 len's)
UT3D_pt_traptptlen        transl. point into dir pDir dist. lenv
UT3D_pt_trapt2pt          transl. point distance p1 -> p2
UT3D_pt_tracirlen         transl. point circular length
UT3D_pt_tra_pt2_rsys      transf. 2D-Point => 3D-Point
UT3D_pt_tra_pt_bp         transf. 3D-Point => 2D-Point with Z-value from backplane
UT3D_pt_tra_pt_rsys       transf. 3D-Point => 2D-Point with Z-value from Refsys
UT3D_pt_tra_pt_m3         apply transformation to point (from 4x3-matrix)
UT3D_pt_tra_pt_m4         apply transformation to point (from 4x4-matrix)
UT3D_pt_tra_wcs2ucs_pl    point-worldCoords -> userCoords from plane
UT3D_pt_tra_ucs2wcs_pl    point-userCoords from plane -> worldCoords
UT3D_pt_rotptptangr       rotate pti around ptc/Z-Axis
UT3D_pt_rotptptvcangr     rotate pt around Line (pt/vc), angle
UT3D_pt_rot2ptpl2angr     rotate and tilt point around centerpoint
UT3D_pt_rotciangr         rotate pt around Circ, angle
UT3D_pt_rotptm3           rotate pt (prepare with UT3D_m3_inirot_angr)
UT3D_pt_projpt2pt         point = project point pt to line pt+pt
UT3D_pt_projptptvc        point = project point pt to line pl+vl
UT3D_pt_projptln          point = project point to line
UT3D_pt_proj_pt_pt_vcn    project point pt onto line pl+vcl (len=1)
UT3D_pt_projptci          point = project point to circ
UT3D_pt_projptptnvc       project point pt to plane from Origin, Z-vec
UT3D_pt_projptpl          point = project point to plane
UT3D_pt_int2pt2pt_lim     intersect 2 limited lines
UT3D_pt_int2pt2vcn_lim    intersect 2 limited NORMAL lines
UT3D_pt_intptvcplx        intersect point/vector with Y-Z-plane
UT3D_pt_intptvcply        intersect point/vector with X-Z-plane
UT3D_pt_intptvcplz        intersect point/vector with X-Y-plane
UT3D_pt_intptvcxpln       intersect line(x-parallel) with plane (pt-vc)
UT3D_pt_intptvcypln       intersect line(y-parallel) with plane (pt-vc)
UT3D_pt_intptvczpln       intersect line(z-parallel) with plane (pt-vc)
UT3D_2par_int2pt2vc       intersect 2 unlimitedLines; gives 2 parameters
UT3D_pt1_int2pt2pt        intersect 2 unlimitedLines; point on pa-pb out
UT3D_pt_int2pt2vc         intersect 2 rays
UT3D_pt_intptvcln         intersect ray - lineSegment
UT3D_pt_int2ln            intersect 2 unlimitedLines; get 2 points & dist
UT3D_pt_intlnln           intersect 2 Lines both limited or unlimited
UT3D_pt_intptvcsph        intersect unlimited line / sphere
UT3D_pt_intperp2ln        inters ln2 with perp.plane to ln1
UT3D_ptCen_2Ln            give common centerPoint of 2 Lines (from 2 CircSegs)
UT3D_pt_intlnci_l         intersect line - circle (limited|unlimited)
UT3D_pt_intlnci__         intersect line - circle (both unlimited)
UT3D_pt_intlnci_lp        intersection LN-CIR (limited|unlimited, same plane)
UT3D_pt_intlnci_p         intersect LN-CIR (in same plane; both unlimited)
UT3D_pt_intlnsph          intersect limited line / sphere
UT3D_pt_intcici           intersect CIR CIR (gives 0/1/2 points)
UT3D_pt_intlnpl           point = intersection of line - plane
UT3D_pt_intplnln          intersect LineSegment X Plane
UT3D_pt_intlnpl1          intersect line - plane / besonders schnell
UT3D_ptDi_intptvcptvc     intersect line-plane (pt-vc  pt-vc)
UT3D_ptDi_intptvcpln      intersect line-plane (pt-vc  plane)
UT3D_pt_intptvcpln        intersect line-plane (pt-vc  pt-vc)
UT3D_pt_intplci           intersect PLN CIR (gives 0/1/2 points)
UT3D_pt_intcidpln         intersect circ / perpend.offsetPlane
UT3D_pt_intptvcpl_         point = intersection of pt / vc  - plane
UT3D_pt_ipl_2ptpt2        interpolate PointPosition for 2D-point
UT3D_pt_mirptpl           mirror point about plane
UT3D_pt_mirptln           mirror point about line
UT3D_pt_tangptci          tangent from point to circ
UT3D_pt_tng_ci_vc         tangent with fixed vector to circ
UT3D_ptvc_eval_ci_angr    get point/tangent to circ from opening-angle
UT3D_pt_evpar2pt          evaluate param.point on line
UT3D_pt__par_pt_vc        evaluate param.point on line from point+vector
UT3D_pt_evparln           evaluate param.point on line
UT3D_pt_vc__par_ci           evaluate param.point on circ
UT3D_pt_m3                copy origin out of 4x3-matrix
UT3D_pt_seg_par_nln       point <-- segNr & parameter on lines[] (Typ_CVLNA)

UT3D_ptvcpar_std_dbo      get typical points & tangent-vector for DB-obj
UT3D_ptvcpar_std_obj      get typical points & tangent-vector for obj
UT3D_ptvcpar_ipt_obj      get control-points (tangent-vector) for polygon,spln

-------------- point - vector ---------------------------------
UT3D_ptvc_int2pl          point/vector = intersection of 2 planes
UT3D_ptvc_int2pln         point/vector = intersection of 2 planes (pln=pt,z-vec)
UT3D_ptvc_ox              get axis (PT+VC) from PT|LN|CI|PLN
UT3D_ptvc_eval_ci_angr    get point/tangent to circ from opening-angle
UT3D_pt_vc__par_ci        get point/tangent-vector from parameter on circle
UT3D_pt_vc__par_cv        get point/tangentVector from parameter on curve
UT3D_ptvc_eval_ci_angr    get point/tangent to circ from opening-angle

-------------- float[]-vectors -------------------------------------
UT3D_vc3f_perp_3pt           float-vector = perpendic. to 3 points (crossprod)
UT3D_vc3f_setLength          change vectorlength
UT3D_vc3f_vc               float-vector from vector

-------------- vectors -------------------------------------
UT3D_compvcNull           compare vector for (exact) 0,0,0
UT3D_compvc0              compare vector for 0,0,0 with tol
UT3D_comp2vc_d            compare 2 vectors for parallel and antiparallel
UT3D_comp2vc_p            compare 2 vectors for parallel
UT3D_vc_ck_parl2_vc       check for parallel/antiparallel (normalized only)
UT3D_vc_ck_parl_vc        check for parallel (normalized only)
UT3D_vc_ck_aparl_vc       check for antiparallel (normalized only)
UT3D_vc_ck_parpl          check if vec is parallel to plane
UT3D_vc_ck_perpvc         check if 2 vectors are normal (perpendic.)
UT3D_vc_ckperp_2vc1       check if 2 normalized-vectors are normal (perp.)
UT3D_vc_ck_std            check for defaultVector (DX DY DZ DIX DIY DIZ)
see also UT3D_sid_2vc       check vc's for perp, same or opposit direction
UT3D_vc_merge2vc          merge 2 vectors   vNew = (v1 + v2) / 2    (INLINE)
UT3D_vc_vc3f              vector from float-vector
UT3D_vc_3db               Vector = 3 doubles (x, y, z)              (INLINE)
UT3D_vc_pt                Vector = 0,0,0 -> Point                   (INLINE)
UT3D_vc_pt3db             Vector = Point -> point from 3 doubles    (INLINE)
UT3D_vc_vc2               3D-vector = 2D-vector                     (INLINE)
UT3D_vc_2pt               Vector = Point -> Point                   (INLINE)
UT3D_vc_2ptlen            Vector = Point -> Point, set Length
UT3D_vc_angr              Vector = Angle_X_axis
UT3D_vc_2angr             Vector = Angle_X_axis, Angle_Z_axis
UT3D_vc_2vc               intermediate Vector
UT3D_vc_ln                Vector = LineStartPoint -> LineEndPoint
UT3D_vcn_len__ln          get normalized vector and length of line
UT3D_vc_txt               Vector from text
UT3D_vc_m3                = UT3D_m3_get
UT3D_vcz_bp               get z-vector of backplane
UT3D_2vc_pta              get 2 vecs of max extent of a polgon (for perp.Vec)
UT3D_vc_invert            Vector invert (change direction)   INLINE
UT3D_vc_Zup               invert Vector if its Z-coord is negative
UT3D_vc_add2vc            v3 = v1 + v2       (INLINE!)
UT3D_vc_addvc2pt          add Vector + Vector from 2 points         (INLINE)
UT3D_vc_add3vc            vo = v1 + v2 + v3                         (INLINE)
UT3D_vc_sub2vc            v3 = v1 - v2                              (INLINE)
UT3D_vc_div_d             divide vector                             (INLINE)
UT3D_vc_multvc            vo = vi * fakt     Vec-multiplication     (INLINE)
UT3D_vc_perp1vc           normal vector to 3D-vector (in X-Y-plane)
UT3D_vc_perpvcvc          get normal-vector from vector onto other vector
UT3D_vc_perp2vc           vector = perpendic. to 2 vectors (crossprod)
UT3D_vc_perpvc2pt         vector = perpendic. to vec & 2 points (crossprod)
UT3D_vc_perp3pt           vector = perpendic. to 3 points (crossprod)
UT3D_vc_perp4pt           vector = perpendic. to 2 lines (crossprod)
UT3D_vc_perp_npt          vector = perpendic. to n points
UT3D_vc_perpvcplXY        Normalvektor auf vc1, der in der X-Y-Plane liegt
UT3D_vc_perpcv            Normalvektor auf planares Polygon / alt-langsam
UT3D_vc_perppta           get normalvector for polygon
UT3D_vc_perp_npt_bp_ix    normalvector for polygon from backPlane & extreme-points
UT3D_vc_perp_bp           get normalvector for backplane
UT3D_vc_normalize         change to length 1
UT3D_vc_setLength         change vectorlength
UT3D_vc_setLenLen         change vectorlength; actLen known

UT3D_vc_tra_vc2_rsys      transf. 2D-Vector => 3D-Vector
UT3D_vc_tra_vc2_bp        transfer 2D-vector onto 3D-vector from bp and bpd
UT3D_vc_tra_vc_m3         apply transformation to vector (from 4x3-matrix)
UT3D_vc_travcm4           apply transformation to vector (from 4x4-matrix)
UT3D_vc_tng_crv_pt        vector tangent to curve
UT3D_vc_tng_ci_pt         tangent-vector to circ (0. - 1.) from point on circ
UT3D_vc_rot_90_sr         rotate vector 90 deg CCW or CW around axis
UT3D_vc_rotangr           rotate a vector around Z-axis
UT3D_vc_rot3angr          rotate a vector around 3 angles
UT3D_vc_rotvcvcangr       rotate vec vi around vec va
UT3D_vc_rotvcangr         rotate a vector around axis
UT3D_vc_projvcvc          vector v3 = project vector v1 to vector v2
UT3D_vc_projvc2vc         vector = project vector to plane from 2 vec's
UT3D_vc_projvcnvc         project vec onto plane from Z-vec
UT3D_vc_projvcpl          vector = project vector onto plane
UT3D_vc_mirvcpl           mirror vector about plane
UT3D_vc_mirvcln           mirror vector about line

UT3D_vc_mod_obj           get typical (normal-)vectors for obj (MOD)

-------------- lines (see also _ptvc_) --------------------
UT3D_comp2ln              compare 2 lines if identical
UT3D_ln_ck_on_ln          check if 2 lines collinear
UT3D_ln_ck_parpl          check if line is parallel to plane
UT3D_ln_ck_degen          check if line is degenerated (length < UT_TOL_cv)
UT3D_ln_inv               invert (change p1, p2)          INLINE
UT3D_ln_unlim             set  length for construction-line
UT3D_ln_6db               line from 2 * 3 doubles
UT3D_ln_ln2               3D-Line from 2D-Line (Z=0)
UT3D_ln_2pt2              3D-Line from 2D-points (Z=0)
UT3D_ln_ptvc              Line = PT + VC                  INLINE
UT3D_ln_ptpt              Line = Point, Point             INLINE
UT3D_ln_parl2ln           ln <== gemeinsame Strecke of 2 lines
UT3D_ln_pts_dmax          line <-- points with max. distance of pointTab
UT3D_ln_setLen            change LineLength; center around pc

UT3D_ln_tra_parl          offset line
UT3D_ln_tra_vc            translate line
UT3D_ln_tra_ln2_rsys      transf. 2D-Line => 3D-Line
UT3D_ln_tra_ln2_bp        transfer 2D-line onto 3D-plane from bp and bpd
UT3D_ln_rot_90_sr         rotate line 90 deg CCW or CW around axis at startpoint
UT3D_ln_tra_ln_m3         apply transformation to line (4x3-matrix)
UT3D_ln_tng_ci_pt         tangent to circ tru point
UT3D_ln_tng_ci_vc         tangent with fixed vector to circ
UT3D_ln_tng_ci_ci         Tangente an CIR - CIR
UT3D_ln_projlnci          line = project endpoints of line --> circPlane
UT3D_ln_int2pl            LN = Intersection PLN / PLN

UT3D_cvLn_obj             get line (Typ_CVLN3) from PT|LN|CI|PLN (eg axis)
UT3D_cvLn_obj             get line (Typ_CVLN3) from 2 points

-------------- circs --------------------------------------
UT3D_rdc_3pt              Radius eines Kreises aus 3 Punkten errechnen
UT3D_ck_ci180             check if circ is 180-degree-circ
UT3D_ck_ci360             check if circ is 360-degree-circ
UT3D_cksid_civc           ck if KreisStart/Endpunkt ist in Richtg VC od opposit
UT3D_ptNr_ci              nr of points for polygon from circle (see UT2D_ptNr_ci)
UT3D_ci_inv1              invert (change p1, p2, dir (optisch gleich))
UT3D_ci_inv2              invert (dir - KomplementaerKreis)
UT3D_ci_inv3              invert (optisch gleich, Z-Axis invers)
UT3D_ci_cip1              change startpoint of circ
UT3D_ci_cip2              change endpoint of circ
UT3D_ci_ciangr            change endpoint of circ from angle
UT3D_ci_setangr           fix angle from p1-p2
UT3D_ci_cip3              change start- and endpoint of circ
UT3D_ci_cirad             change radius of circ
UT3D_ci_cipt180           change circ -> 180 deg-circ; keep dir
UT3D_ci_cipt360           change circ -> 360 deg-circ; keep dir
UT3D_ci_ptvcr             Circ from center, axis, radius
UT3D_ci_pt2vcr            Circ from center, axis, x-vec and radius
UT3D_ci_ptptvcangr        Circ from center, startpoint, axis, opening angle
UT3D_ci_2ptvcrd           Circ from  2 Points-on-Circ  & Radius
UT3D_ci_ptptvcrd          Circ from startpoint endpoint radius Z-vector
UT3D_ci_ptvcpt2angr       circ from center, point, axis, startAngle, endAngle
UT3D_ci_ptrd2vc2angr      circ from cen, Rd, Z-Axis, X-Axis, startAng, endAng
UT3D_ci_ptvcrd            Circ from startpoint, startvector, radius ..
UT3D_ci_3pt               Circ from 3 points
UT3D_ci_3pt_vc_rd_ango    Circ from startPt endPt centerPt Z-vec rad ango
UT3D_ci_pcvzpt180         180 deg-circ (CCW) from center, axis, startpoint
UT3D_ci_ci2               3D-Circ = 2D-Circ
UT3D_ci_obj2              3D-Circ = 2D-ObjG
UT3D_ci_obj               3D-Circ = 3D-ObjG
UT3D_civz_ci              den vz eines Circ (aus p1-pc-p2) errechnen
UT3D_ci_2lnrd             Circ = corner for 2 Lines
UT3D_ci_ptptrd            Circ = tangent to PT + PT, radius
UT3D_ci_lnptrd            Circ = tangent to LN + PT, radius
UT3D_ci_lncird            Circ = tangent to LN + CIR, radius
UT3D_ci_ciptrd            Circ = tangent to CI + PT, radius
UT3D_ci_cicird            Circ = tangent to CIR + CIR, radius
UT3D_ci_intplsph          Circ = intersect Plane Sphere
UT3D_ci_tracivc           move circle

UT3D_ci_tra_ci2_rsys      transf. 2D-Circ => 3D-Circ
UT3D_ci_tra_ci2_bp        transfer 2D-Circ onto 3D-plane from bp and bpd
UT3D_ci_tra_ci_m3         apply transformation to circle (4x3-matrix)

-------------- polygon -----------------------------------------
// TODO: cv, pta -> ../ut/ut3d_npt.c
UT3D_pta_ck_planar        test if all points are in plane from pt-normalVec
UT3D_rMin_pta             den kleinsten Radius errechnen eines Polygon suchen
UT3D_cksq_4pt             check 4 points if its a square
UT3D_pta_dbo              get polygon from DB-object(typ,ind)
UT3D_cv_obj2              Polygon < 2D-Obj
UT3D_cv_ln                Linearstueck -> Polygon / Anzahl
UT3D_cv_ci                Polygon < 3D-Kreis
UT3D_npt_ci               circular polygon
UT3D_pta_rot__            rotate curve CCW um axis ..
UT3D_cv_inv               Punktetabelle umdrehen
UT3D_cv_realg             Punktetabelle umschichten (realign)
UT3D_cv_delia             delete points out of pTab
see also: ../ut/ut_npt.c
UT3D_npt_ox__
UT3D_pta_sus

-------------- GrafObj - (see also UTO_) ---------------
UT3D_obj_obj2             change 2D-Obj > 3D-Obj (add Z=0; vz=0,0,1)
UT3D_obj_pt    DO NOT USE change Point > 3D-Obj
UT3D_obj_ln    DO NOT USE change Line > 3D-Obj
UT3D_obj_ci2   DO NOT USE change 2D-Circ > 3D-Obj
UT3D_obj_ci    DO NOT USE change Circ > 3D-Obj
UT3D_obj_vctra DO NOT USE translate Obj

-------------- plane ---------------------------------------
UT3D_pl_XYZ               Defaultplane auf absolut 0,0,0
UT3D_pl_bp                plane from backplane and offset
UT3D_pl_3pt               plane from 3 points (vx parallel XY-mainPlane)
UT3D_pl_pta               plane from n-points
UT3D_pl_ptvzpl            plane from z-axis and origin and base-plane
UT3D_pl_lnz_ptx           plane from Z-axis (Typ_CVLN3), point on X-axis
UT3D_pl_pto_vcxn_pt       plane from Origin, X-vec(normalized), point in plane
UT3D_pl_ptvxpl            plane from x-axis and origin and base-plane
UT3D_pl_pto_vcz_vcx       plane from Origin, Z-vec, X-Vec. Z-vec is fixed.
UT3D_pl_pto_vcx_vcz       plane from Origin, X-vec, Z-Vec. X-vec is fixed.
UT3D_pl_pto_vcx_vcy       plane from origin, X-vec, Y-Vec. X-vec is fixed.
UT3D_pl_ptvc              plane from origin, Z-vector
UT3D_pl_pto_vcz_ptx       plane from origin, Z-vec, point on X-axis
UT3D_pl_obj               get basic-plane for obj
UT3D_pl_perp_ln           make Plane from line (line = Plane-Z-vec)
UT3D_pl_ln                get plane for line in plane
UT3D_pl_2ln               plane from 2 lines in plane
UT3D_pl_ci                make Plane from circ
UT3D_pl_ell               make Plane from ellipse
UT3D_pl_nobj              plane from ObjGX-table
UT3D_pl_invert            Plane invert (change direction of Z-vec)
UT3D_pl_rotpt             rot. Refsys around Z; point gives new X-direction
UT3D_pl_rotZangr          rot. Refsys around Z
UT3D_pl_rotYangr          rot. Refsys around Z (tilt-angle)
UT3D_pl_ptpl              change origin of plane
UT3D_pl_plcoe             plane <-- plane equation
UT3D_plcoe_pl             plane equation <-- plane
UT3D_pl_p                 plane - update p after a change of po or vz
UT3D_pl_mirln             mirror plane about line
UT3D_pl_mirpl             mirror plane about plane
UT3D_pl_tra_m3            apply transformation to refSys

-------------- backPlane ---------------------------------------
UT3D_bp_perp_2pt          returns main-BackPlaneNr (0-2) normal to 2 points
UT3D_bp_perp_vc           returns main-BackPlaneNr (0-2) normal to vec
UT3D_bp_ln                backplane from line
UT3D_bp_pta               backplane from n points
UT3D_bp_pl__              get backplane+offset for plane
UT3D_bpd_bp_pt            get disptance of point from backplane

-------------- Refsys ------------------------------
UT3D_rsys_pl              get backplane/transformation-matrix for plane
UT3D_rsys_bp              get Refsys and plane for backplane

-------------- transformation ------------------------------
UT3D_m3_copy              memcpy
UT3D_m3_inirot_angr       define rotation (axis, origin angle)
UT3D_m3_iniZrot_angr      define rotation around z-axis; (center, angle)
UT3D_m3_inirot_2vc        define rotation-transformation between 2 vectors
UT3D_m3_load              neues Achsensystem mit vx,vy,vz definieren
UT3D_m3_load_povxvy       neues Achsensystem mit Origin,vx,vy definieren
UT3D_m3_load_povxvz       neues Achsensystem mit Origin,vx,vz definieren
UT3D_m3_load_o            Nullpunkt into 4x3 Transform.Matrix
UT3D_m3_loadpl            load Plane(=Achsensystem) into 4x3 Transform.Matrix
UT3D_m3_invm3             4x3-Matrix invertieren
UT3D_m3_get               get part of matrix;  0=VX, 1=VY, 2=VZ, 3=PO
UT3D_m3_set               set part of matrix;  0=VX, 1=VY, 2=VZ, 3=PO
UT3D_m3_vc                = UT3D_m3_set
UT3D_m3_traVc             translate mat (move progin)              
UT3D_m3_tram3m3           apply transformation to Mat_4x3
UT3D_m3_scl               scale mat
UT3D_m3_multm3            4x3-Matrix mit 4x3 Matrix multiplizieren
UT3D_m3_tra_m3            ev ident mit UT3D_m3_multm3 ?
UT3D_m3_rot_m3            rotate matrix
UT3D_m3_m4v               copy a vertical-Mat_4x4  -->  Mat_4x3

UT3D_m4_init              Initialize a 4x4 - matrix
UT3D_m4_init_ori          Initialize a 4x4 - matrix with translation
UT3D_m4_init_rot          Initialize a 4x4 - matrix with rotation
UT3D_m4_load_o            set origin
UT3D_m4_loadpl            load Plane(=Achsensystem) into 4x4 Transform.Matrix
UT3D_m4_addrot            Concatenate 2 4x4 - matrixes.
UT3D_m4_addtra            add a translation into 4x4-matrix
UT3D_m4_copy              memcpy
UT3D_m4v_m3               copy a Mat_4x3  -->  vertical-Mat_4x4

List_functions_end:
=====================================================

\endcode *//*----------------------------------------



=====================================================
Datenstrukturen:
Lineare Kurven sollen durch Projektion nicht zerstoert werden
  (Z-Vektor speichern!)


Planare Kurven kann man mit RefSys und 2D-Beschreibung speichern!
Kreis:   {3D-RefSys, 2D-Origin, AngleStart, AngleEnd, Radius}
Ellipse: {3D-RefSys, 2D-Origin, AngleStart, AngleEnd, la, lb}
Line     {3D-RefSys, 2D-pt, 2d-pt}
Planare_3D-Curve {3D-RefSys, Planare_2D-Curve}

*/

#ifdef _MSC_VER
#include "../xa/MS_Def0.h"
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#include <stdarg.h>


// #include "../ut/ut_umem.h"            // UME_reserve
#include "../ut/ut_geo.h"
#include "../ut/ut_ox_base.h"             // OGX_SET_INDEX
#include "../ut/ut_math.h"
#include "../ut/ut_bspl.h"                // UT3D_parbsp_par1
#include "../ut/ut_rbspl.h"               // UT3D_par_par1_rbsp
#include "../ut/ut_plg.h"                 // UT3D_par_par1plg
#include "../ut/ut_elli.h"                // UT3D_angr_par1_ell
#include "../ut/func_types.h"             // SYM_SQUARE ..
#include "../ut/ut_TX.h"                  // TX_Error
#include "../ut/ut_memTab.h"              // MemTab_..

#include "../xa/xa_msg.h"                 // MSG_*

#include "../gr/ut_gr.h"                  // GR_tDyn_*



//----------------------------------------------------------------
// protos
Point*    DB_get_PT        (long Ind);




//===========================================================================
typedef_MemTab(Point);






//======================================================================
  double UT3D_angr_vc2pt (Point *pz, Vector *vz, Point *p1, Point *p2) {
//======================================================================
/// \code
/// UT3D_angr_vc2pt           angle between 2 points in plane (plane from pt-vc)
/// get angle between lines pz-p1 and pz-p2, on a plane normal to vz.
/// angle is CCW, 0 - PI*2
///
/// see also UT3D_angr_vcpl_z UT3D_angr_ci_p1_pt UT3D_angr_ci__ UT3D_acos_2vc
/// \endcode

  double  ao, dx, dy;
  Point   pc;                  // der Origin der Plane
  Vector  vx, vy, vpt;
  Vector2 vc2;

  // DEB_dump_obj__ (Typ_VC, vz, "UT3D_angr_vc2pt vz:");

  // ein Achsensystem erzeugen;
  // project p1 auf Achse pz-vz
  UT3D_pt_projptptvc (&pc, &dx, NULL, p1, pz, vz);

  // pc-p1 ist nun der X-Vektor; dazu einen Y-Vektor erzeugen
  UT3D_vc_2pt (&vx, &pc, p1);
  UT3D_vc_perp2vc (&vy, vz, &vx);
    // DEB_dump_obj__ (Typ_VC, &vx, " vx:");
    // DEB_dump_obj__ (Typ_VC, &vy, " vy:");


  // ab hier wie UT3D_angr_vcpl_z ..

  // vc auf die X- und Y-Achse projizieren
  UT3D_vc_2pt (&vpt, &pc, p2);
  dx = UT3D_slen_projvcvc (&vpt, &vx);
  dy = UT3D_slen_projvcvc (&vpt, &vy);
    // printf(" dx=%f dy=%f\n",dx,dy);

  // dx, dy in eine normierten 2D-Vector vc2 umwandeln
  vc2.dx = dx;
  vc2.dy = dy;
  UT2D_vc_setLength (&vc2, &vc2, 1.);
    // DEB_dump_obj__ (Typ_VC2, &vc2, "  vc2=");

  // Winkel des 2D-vec vc2
  ao = UT2D_angr_vc (&vc2);

  // printf("ex UT3D_angr_vcpl_z %f\n",ao);

  return ao;

}


//================================================================
  double UT3D_angr_vc2vc (Vector *vc1, Vector *vcx, Vector *vcy) {
//================================================================
/// \code
/// UT3D_angr_vc2vc        compute angle of vec in Refsys (to X-axis)
/// Compute the angle between vector vc1 in a refSys = plane
///   from its X- and Y-vector around the Z-axis of this plane.
/// vcx,vcy must be normalized !
/// the angle between vc1 - vcx is computed.
/// Input:
///   vcx, vcy  defining the plane
///   vc1       compute the angle between vcx-vc1 in the plane.
/// Output:
///   angr     angle in radians; min 0 max 2Pi (0-360 deg).
///
/// see also UT3D_angr_vcpl_z UT3D_angr_vc2pt
/// \endcode


  double    ao, dx, dy;
  Vector2   vc2;


  // vc auf die X- und Y-Achse projizieren
  dx = UT3D_slen_projvcvc (vc1, vcx);
  dy = UT3D_slen_projvcvc (vc1, vcy);
    // printf(" dx=%f dy=%f\n",dx,dy);


  // dx, dy in eine normierten 2D-Vector vc2 umwandeln
  vc2.dx = dx;
  vc2.dy = dy;
  UT2D_vc_setLength (&vc2, &vc2, 1.);
    // printf(" vc2: dx=%f dy=%f\n",vc2.dx,vc2.dy);


  // Winkel des 2D-vec vc2
  ao = UT2D_angr_vc (&vc2);


  // L_fertig:
  printf("ex UT3D_angr_vc2vc %f\n",ao);


  return ao;

}


//================================================================
  double UT3D_angr_vcpl_tilt (Plane *pl1, Vector *vc1) {
//================================================================
/// \code
/// UT3D_angr_vcpl_tilt       compute angle of vec in Refsys (to XY-plane)
/// Compute the angle between vc1 and the XY-plane of plane pl1
///   around a vector normal to vc1 in the XY-plane of pl1
/// Input:
///   pl1       Plane / Refsys
///   vc1       Vector
/// Output:
///   angr     angle in radians; min 0 max 2Pi (0-360 deg).
/// \endcode

  // return UT3D_angr_vc2vc (vc1, &pl1->vx, &pl1->vz);

  Vector2  vc2;

  vc2.dx = UT3D_nlen_projvcvc (&pl1->vz, vc1);
  vc2.dy = UT3D_skp_2vc (&pl1->vz, vc1);
  // dy = UT3D_acos_2vc (&pl1->vz, vc1) - pl->p;
    printf(" vc2=%lf %lf\n",vc2.dx,vc2.dy);


  return UT2D_angr_vc (&vc2);

}


//================================================================
  double UT3D_angr_vcpl_z (Plane *pl1, Vector *vc1) {
//================================================================
/// \code
/// UT3D_angr_vcpl_z          compute angle of vec in Refsys (to X-axis)
/// Compute the angle between vc1 and the X-vector of plane pl1
///   around the Z-axis of pl1.
/// Input:
///   pl1       Plane / Refsys
///   vc1       Vector
/// Output:
///   angr     angle in radians; min 0 max 2Pi (0-360 deg).
/// \endcode



  // DEB_dump_obj__ (Typ_VC, vc1, "UT3D_angr_vcpl_z");
  // DEB_dump_obj__ (Typ_PLN, pl1, "");

                        // v1,  vx,  vy
  return UT3D_angr_vc2vc (vc1, &pl1->vx, &pl1->vy);


/*    das macht alles auch die UT2D_angr_vc ...
  if(fabs(dy) < UT_TOL_min1) {      // parallel X
    if(dx < 0.) ao = RAD_180;
    else        ao = 0.;           // auch wenn dx=0, dy=0, dz=1.
    goto L_fertig;
  }


  if(fabs(dx) < UT_TOL_min1) {      // parallel Y
    if(dy > 0.) ao = RAD_90;
    else        ao = RAD_270;
    goto L_fertig;
  }
*/


}


//================================================================
  double UT3D_angr_2pl (Plane* pl1, Plane* pl2) {
//================================================================
/// Oeffnungswinkel zwischen 2 Planes

  return UT3D_angr_2vc__ (&pl1->vz, &pl2->vz);

}


//================================================================
  int UT3D_angrR_vc (double *ar, Vector *vc1) {
//================================================================
// UT3D_angrR_vc     get rotation-angle for vector
// vc1 must be normalized
// see UT3D_angrT_vc UT3D_2angr_vc__

  // angle from top-view
  *ar = ACOS(vc1->dx);

  if(vc1->dy < 0.) *ar *= -1.;

  return 0;

}


//================================================================
  int UT3D_angrT_vc (double *at, Vector *vc1) {
//================================================================
// UT3D_angrT_vc     get tilt-angle for vector
// vc1 must be normalized
// angle-values -90-deg to +90-deg;  not up to 180 !
// see UT3D_angrR_vc UT3D_2angr_vc__

  Vector2   vcxz;


  // DEB_dump_obj__ (Typ_VC, vc1, "UT3D_angrT_vc ");

  
  // Seitenansicht X-Z aus Richtung -Y
  vcxz.dx = sqrt((vc1->dx*vc1->dx)+(vc1->dy*vc1->dy));
  vcxz.dy = vc1->dz;


  // normalize
  UT2D_vc_setLength (&vcxz, &vcxz, 1.);

  // get angle 
  *at = ACOS(vcxz.dx);   // always positiv;

  if(vc1->dz < 0.) *at *= -1.;

    // printf(" ex-angrT_vc %f\n",*at);

  return 0;

}


//================================================================
  int UT3D_2angr_2vc (double *angX, double *angZ,
                      Vector *vcX, Vector *vcY) {
//================================================================
/// \code
/// UT3D_2angr_2vc   get transf.angles for a refSys from its X-and Y-axis
/// angX = tilt.Angle (rotate around X-axis), angZ = rotate around Z-axis
/// using: rotate first GL_angX then GL_angZ  (see GLBT_view_set)
/// see also UQT_vcar_qt
/// \endcode


  // DEB_dump_obj__ (Typ_VC, vcX, "UT3D_2angr_2vc - vcX");
  // DEB_dump_obj__ (Typ_VC, vcY, "                 vcY");


  // angZ = rot.Angle
  UT3D_angrR_vc (angZ, vcX);  // rot.Angle from X-axis
  *angZ = UT_DEGREES(*angZ) * -1.;

  // angX = tilt.Angle
  UT3D_angrT_vc (angX, vcY);  // tilt.Angle from Y-axis
  *angX = UT_DEGREES(*angX) * -1.;


  if((*angZ < 0.) && (vcY->dx > 0.)) *angX = 180. - *angX;
  if((*angZ > 0.) && (vcY->dx < 0.)) *angX = -180. - *angX;


    // printf("ex UT3D_2angr_2vc angX=%f angZ=%f\n",*angX,*angZ);

  return 0;

}


//=======================================================================
  int UT3D_2angr_vc__ (double *az, double *ay, Vector *vc1) {
//=======================================================================
// \code
// UT3D_2angr_vc__           2 opening angles of vec (angle to Z-axis, tilt-angle)
// Output:
//   ay   tilt-angle = angle between vc1 and the absolute-XY-plane
//   az   angle between vc1 and the absolute XZ-plane
// \endcode
//
//  az: get vcx = project vc1 onto absolute-XY-plane;
//      az = angle between vcx - absolute-X-axis)
//
// restore vector from angles:
//   vcx = rotate X-axis az radians around Z-axis;
//   vc1 = rotate vcx ay radians around the (new) Y-axis
//         (the new Y-axis yet is in the absolute-XY-plane)
//
// see UT3D_angrR_vc UT3D_angrT_vc


  Vector2 vcxy, vcxz;


  // 2D-Vector aus Draufsicht auf X-Y
  vcxy.dx = vc1->dx;
  vcxy.dy = vc1->dy;

  // 2D-Vector normieren
  UT2D_vc_setLength (&vcxy, &vcxy, 1.);
  // printf("vcxy=%f,%f\n",vcxy.dx,vcxy.dy);

  *az = ACOS(vcxy.dx);          // um Z-Achse verdrehen


  // Seitenansicht X-Z aus Richtung -Y
  vcxz.dx = sqrt((vc1->dx*vc1->dx)+(vc1->dy*vc1->dy));
  vcxz.dy = vc1->dz;


  // normieren
  UT2D_vc_setLength (&vcxz, &vcxz, 1.);
    // printf("vcxz=%f,%f\n",vcxz.dx,vcxz.dy);

  *ay = ACOS(vcxz.dx);          // um (die neue) Y-Achse verdrehen


  if(vc1->dy < 0.) *az *= -1.;
  if(vc1->dz < 0.) *ay *= -1.;


    // printf("ex-UT3D_2angr_vc__ az=%f ay=%f vc=%f,%f,%f\n",
            // UT_DEGREES(*az),UT_DEGREES(*ay),
            // vc1->dx,vc1->dy,vc1->dz);


  return 0;

}


/*
//================================================================
  int UT3D_2angr_vc_pl (double *az, double *ay, Vector *vc1,
                         Vector *vx,  Vector *vz, double *dpn) {
//================================================================
// \code
// UT3D_2angr_vc_pl          2 angles of vec relativ in coordinate-system (vx,vz)
//   compute the angles between vc1 and the coordinate-system (vx,vz) 
// Input:
//   vx,vz,dpn define a coordinate-system;
//   dpn       normal-distance of plane ((vx,vz) from 0,0,0
//   vc1       a vector in the coordinate-system (vx,vz)
// Output:
//   ay        tilt-angle = angle between vc1 and the XY-plane of (vx,vz)
//   az        angle between vc1 and the XZ-plane of (vx,vz)
//
// restore vector from angles:
//   vcx = rotate X-axis az radians around Z-axis vz;
//   vc1 = rotate vcx ay radians around the (new) Y-axis
//         (the new Y-axis is in the XY-plane of coordinate-system (vx,vz))
// 
// \endcode
// see UT3D_2angr_vc__ UT3D_3angr_2vc

  double   k1, k2, dist, dp, pr;
  Point    pt1, pt2, pth;
  Vector   vczz, vcxy, vln;


  printf("UT3D_2angr_vc_pl dpn=%f\n",*dpn);
  DEB_dump_obj__ (Typ_VC, vc1, "  vc1");
  DEB_dump_obj__ (Typ_VC, vx, "  vx");
  DEB_dump_obj__ (Typ_VC, vz, "  vz");


  UT3D_pt_vc (&pt1, vc1);
  // UT3D_pt_3db (&pt1, 0., 0., 0.);
    DEB_dump_obj__ (Typ_PT, &pt1, "  pt1");
    // if(WC_sur_ind) UT3D_pt_tra_pt_m3 (&pth, WC_sur_mat, &pt1);
    // DEB_dump_obj__ (Typ_PT, &pth, "  pt1-ucs");
    GR_tDyn_symB__ (&pt1, SYM_STAR_S, ATT_COL_RED);

  vln = *vz;
  dp = 0.; // *dpn;

  //----------------------------------------------------------------
  // get vcxy = project vc1 onto XY-plane of coordinate-system (vx,vz)
  // see UT3D_ptDi_intptvcpln
  // intersect line (pt1,vz) with XY-plane of (vx,vz)
  k1 = vz->dx * vln.dx + vz->dy * vln.dy + vz->dz * vln.dz;
    printf(" k1=%f\n",k1);

  if (fabs(k1) < 1E-12 ) {
  // if ( fabs(k1) == 0.) {
return MSG_ERR__ (ERR_TODO_I, "BAUSTELLE-1");
    // printf("UT3D_pt_intptvcpl: Vector parallel to plane\n");
    // printf(" vc   %f %f %f\n",vln->dx,vln->dy,vln->dz);
    // printf(" pl-Z %f %f %f\n",vz->dx,vz->dy,vz->dz);

    return 0;
  }

  k2 = dp - vz->dx * pt1.x -
            vz->dy * pt1.y -
            vz->dz * pt1.z;
    printf(" k2=%f\n",k2);

  dist = k2/k1;
    printf("  intptvcpl k1=%f k2=%f dist=%f\n",k1,k2,dist);

  // vector from pt1 to XY-plane along vz
  vczz.dx = vln.dx * dist;
  vczz.dy = vln.dy * dist;
  vczz.dz = vln.dz * dist;
    DEB_dump_obj__ (Typ_VC, &vczz, "  vczz");

  UT3D_vc_add2vc (&vcxy, vc1, &vczz);
    DEB_dump_obj__ (Typ_VC, &vcxy, "  vcxy");
// TODO: vectorPosition 0,0,0 but UCS !
// TODO: vector -> UCS !
    // if(WC_sur_ind) UT3D_pt_tra_pt_m3 (&pth, WC_sur_mat, &pt1);
    APT_disp_vc (&vcxy, (Point*)&UT3D_PT_NUL, Typ_Att_top2, 1);


  //----------------------------------------------------------------
  // get az = angle between X-axis vx and vcxy
  pr = UT3D_len_vc (&vcxy);
  if (fabs(pr) < UT_TOL_min2) {
    *az = 0.0;
  } else {
    *az = ACOS(UT3D_acos_2vc (vx, &vcxy) / pr);
  }
    printf(" az=%f pr=%f\n",UT_DEGREES(*az),pr);


  //----------------------------------------------------------------
  // get vcxz = project vc1 onto XZ-plane of coordinate-system (vx,vz)


  //----------------------------------------------------------------
  // get ay = angle between X-axis vx and vcxz


  return 0;

}
*/


//=======================================================================
  int UT3D_3angr_2vc (double *az1, double *ay, double *az2,
                      Vector *vx, Vector *vz) {
//=======================================================================
// \code
// UT3D_3angr_2vc            3 opening angles (3D-position) from X and Z-vec.
//   compute the angles between absolute and the coordinate-system (vx,vz) 
// Input:
//   vx,vz   define a new coordinate-system;
//
// change the orientation from absolute to to coordinate-system (vx,vz):
// - vcx = rotate X-axis az1 radians around Z-axis;
// - vcy = rotate vcx ay radians around the (new) Y-axis
//         (the new Y-axis yet is in the absolute-XY-plane)
// - vc1 = rotate vcy az2 radians around the (new) Z-axis
// 
// \endcode
//
// see also quaternions

/*
  Vector  vx, vz; double az1, ay, az2;
  UT3D_vc_3db (&vz,  0.1, 0.,  1. ); UT3D_vc_3db (&vx,  1.,  0., -0.1);
  UT3D_vc_3db (&vz,  1.,  0.,  1. ); UT3D_vc_3db (&vx,  1.,  0., -1.);
  UT3D_vc_3db (&vz,  1.,  0.,  0.1); UT3D_vc_3db (&vx,  0.1, 0., -1.);
  UT3D_vc_3db (&vz,  1.,  0., -0.1); UT3D_vc_3db (&vx, -0.1, 0., -1.);
  UT3D_vc_3db (&vz,  1.,  0., -1. ); UT3D_vc_3db (&vx, -1.,  0., -1.);
  UT3D_vc_3db (&vz, -1.,  0., -1. ); UT3D_vc_3db (&vx,  1.,  0., -1.);
  UT3D_vc_3db (&vz,  1.,  1.,  1. ); UT3D_vc_3db (&vx,  1.,  1., -1.);
  UT3D_vc_3db (&vz,  1.,  1., -1. ); UT3D_vc_3db (&vx,  1.,  1.,  1.);
  UT3D_vc_3db (&vz,  1.,  1.,  1. ); UT3D_vc_3db (&vx, -1.,  1., -1.);
  UT3D_vc_3db (&vz,  1.,  1., -1. ); UT3D_vc_3db (&vx,  1.,  1.,  1.);
  UT3D_3angr_2vc (&az1, &ay, &az2, &vx, &vz);
*/


  double  axz, d1;
  Vector  vcxy, vcx, vcz;
  Vector2 v2x, v2z;


  // DEB_dump_obj__(Typ_VC, vx, "UT3D_3angr_2vc vx:");
  // DEB_dump_obj__(Typ_VC, vz, "               vz:");


  // normieren ..   
  UT3D_vc_setLength (&vcz, vz, 1.);
  UT3D_vc_setLength (&vcx, vx, 1.);


  //--------------------------------------------------------------------
  // axz Winkel zwischen dem Z-Vec und dem X-Vec in Draufsicht errechnen
  if(fabs(vcz.dx) > UT_TOL_min1) goto L_normal;
  if(fabs(vcz.dy) > UT_TOL_min1) goto L_normal;

    // Draufsicht auf vz ist Punkt. vx.dx/dy muss linear und normiert sein.
    // winkel zwischen abs. X-Achse und neuer X-Achse errechnen.
    // d1 = UT2D_acos_2vc (&v2x, &UT2D_VECTOR_X);
    *az1 = acos(vcx.dx);

    if(vcx.dy < 0.) *az1 = -*az1;

    if(vcz.dz > 0.) {
      *ay  = 0.;
      *az2 = 0.;

    } else {
      *az1 += RAD_180;
      UT2D_angr_set (az1);
      *ay  = RAD_180;
      *az2 = 0.;
    }
    goto L_fertig;





  //--------------------------------------------------------------------
  // Draufsicht auf vz ist Linie;
  L_normal:


    // Draufsicht auf vz -> v2z; normieren
    v2z.dx = vcz.dx;
    v2z.dy = vcz.dy;
    UT2D_vc_setLength (&v2z, &v2z, 1.);

    // Z-Winkel 1 (az1) ist der Winkel zwischen der Z-Achse mit der
    // absoluten X-Achse in Draufsicht
    *az1 = acos(v2z.dx);

    if(vcz.dy < 0.) *az1= -*az1;


    //------------------------------------------------------------------------
    // ay ist der Winkel zwischen der Z-Achse vcz und der absoluten Z-Achse.
    // dazu die Laenge des Z-Vec in Draufsicht errechnen
    // d1 = sqrt((vcz.dx*vcz.dx)+(vcz.dy*vcz.dy));
    *ay = acos(vcz.dz);

    // if(vcz.dz < 0.) *ay =  RAD_180 - *ay;


    //------------------------------------------------------------------------
    // az2 ist der Raumwinkel zwischen dem neuen Y-Vector und dem X-Vektor vcz.
    // Der neue Y-Vector liegt in der absoluten XY-Ebene und ist ein
    // Normalvektor auf vcz.
    vcxy.dx = -vcz.dy;
    vcxy.dy = vcz.dx;
    vcxy.dz = 0.;
    // DEB_dump_obj__(Typ_VC, &vcxy, "  vcxy:");
    *az2 = UT3D_angr_2vc__ (&vcxy, &vcx);

    if(vcx.dz < 0.) *az2 = -*az2;
    *az2 += RAD_90;


  L_fertig:
  // printf("ex UT3D_3angr_2vc az1=%f ay=%f az2=%f\n",
          // UT_DEGREES(*az1),UT_DEGREES(*ay),UT_DEGREES(*az2));


  return 0;

}


//================================================================
  int UT3D_atan_vcpl (double *kvc, Vector *vci, Plane *pli) {
//================================================================
/// \code
/// den Anstieg (tan) der Geraden vci bezueglich Ebene pli
/// vci muss nicht normiert sein
/// irc:  0  OK
/// irc: -1  vci ist normal auf die Plane pli
/// \endcode

// Vector auf PlaneOrigin setzen, auf Plane projizieren, dann k = dy / dx.


  double dx, dy;
  Point  pt1, pt2;

  // ptl = PlaneOrigin + Vector vci
  UT3D_pt_traptvc (&pt1, &pli->po, vci);


  // pt1 auf Plane pli projiz -> pt2
  UT3D_pt_intptvcpl_ (&pt2, pli, &pt1, &pli->vz);


  // dx = Laenge PlaneOrigin - pt2;
  dx = UT3D_len_2pt (&pli->po, &pt2);
  if(dx < UT_TOL_min2) {
    // printf("ex UT3D_atan_vcpl NORMAL\n");
    return -1;
  }

  // dy = Laenge pt1 - pt2
  dy = UT3D_len_2pt (&pt1, &pt2);
  *kvc = dy / dx;

  // printf("ex UT3D_atan_vcpl k=%f\n",*kvc);

  return 0;

}


//===========================================================================
  int UT3D_pt_intptvcln (Point *ip1, Point *ip2, double *dist,
                         Point *pt1, Vector *vc1, Line *ln2) {
//===========================================================================
/// \code
/// UT3D_pt_intptvcln     intersect ray - lineSegment
/// 
/// ip1 liegt auf ray (pt1/vc1)
/// ip2 liegt auf Line ln2
/// 
/// Returncodes:
///  0 = Lines parallel or zero-length
///  1 = OK; dist gives the minimum distance.
///  2 = OK; lines are crossing, dist=0, ip1=ip2, but outside lineSegment.
///  3 = OK; lines are crossing, dist=0, ip1=ip2 and inside lineSegment.
///
/// see UT2D_ptvc_ck_int2pt
/// \endcode


  int    irc;
  Vector vc2;
  Point  pb1, pb2;


  // vector vc2 from ln2
  UT3D_vc_ln (&vc2, ln2);

  // intersect
  irc = UT3D_pt_int2pt2vc (ip1,ip2,dist,  pt1,vc1, &ln2->p1,&vc2);
  if(irc < 2) goto L_fertig;

  // make box of ln2
  UT3D_box_2pt__ (&pb1, &pb2, &ln2->p1, &ln2->p2);

  // test if ip2 lies inside Box ln2
  if(UT3D_pt_ck_inBoxTol (&pb1, &pb2, ip2, UT_TOL_pt) == 0) irc = 3; // inside


  L_fertig:
  // printf("ex UT3D_pt_intptvcln %d d1=%f\n",irc,*dist);
  // GR_tDyn_symB__ (ip1, SYM_STAR_S, 0);
  // GR_tDyn_symB__ (ip2, SYM_STAR_S, 0);

  return irc;

}


/* BUGGY
//=====================================================================
  int UT3D_pt_int2pt2pt_lim (Point *px, double *ux_ab, double *ux_cd,
                             Point *pta, Point *ptb,
                             Point *ptc, Point *ptd, double tol) {
//=====================================================================
/// \code
/// UT3D_pt_int2pt2pt_lim            intersect 2 limited lines
/// get intersectionpoint and parameters
///
/// IN:
///   pta, ptb         line 1
///   ptc, ptd         line 2
/// OUT:
///   Point *px        intersectionpoint
///   double *ux_ab     parameter of px on line a-b  (none if NULL)
///   double *ux_cd     parameter of px on line c-d  (none if NULL)
/// Returncodes:
///    -1 = no intersection inside the segments
///     0 = OK; dist gives the minimum distance.
///     1 = Line a-b parallel and covering c-d (px set to midpoint)
///
/// a-b = ln1    c-d = ln2   e = u_c_ab   f = u_d_ab
///                       d
///                      /.
///                     / .
///                 ln2/  .
///                   /   .
///                  /    .
///                 /     .
///       a-----e--x------f-------------b
///             | /            ln1
///             |/
///             c
///
///
/// \endcode

// intersect 2 limited lines
// see UT3D_pt_int2ln UT3D_pt_intlnln UT3D_pt1_int2pt2pt


   // double d1, d2;  int i1;
   // Point p1 = {    0.0,      0.0,      0.0};
   // Point p2 = { 1000.0,      0.0,      0.0};
   // Point p3 = {   50.0,     10.0,      0.0};
   // Point p4 = {   60.0,     20.0,      0.0};
   // i1 = UT3D_pt_int2pt2pt_lim (&pi, &d1, &d2, &p1, &p2, &p3, &p4, UT_TOL_cv);



  int      irc=0, iover, icdrev=0;
  double   par1, par2, lqab, lqcd, lqec, lqfd, qtol;
  double   u_c_ab, u_d_ab, u_x_ab, u_x_cd, q1, q0, s_ab_cd, s_ac_cd;
  Vector   vab, vcd, vac, vad, vae, vaf, vax, vcx, vec, vfd;
  Point    pb1, pb2, *pp1, *pp2;


  printf("UT3D_pt_int2pt2pt_lim %lf\n",tol);

  qtol = tol * tol;

  printf("UT3D_pt_int2pt2pt_lim tol=%f qtol=%lf\n",tol,qtol);
  DEB_dump_obj__ (Typ_PT, pta, " pta =");
  DEB_dump_obj__ (Typ_PT, ptb, " ptb =");
  DEB_dump_obj__ (Typ_PT, ptc, " ptc =");
  DEB_dump_obj__ (Typ_PT, ptd, " ptd =");


  // vectors from lines
  UT3D_vc_2pt (&vab, pta, ptb);
  UT3D_vc_2pt (&vcd, ptc, ptd);
    // DEB_dump_obj__ (Typ_VC, &vab, " vab =");
    // DEB_dump_obj__ (Typ_VC, &vcd, " vcd =");


  // get the quadratic length of vab, vcd
  lqab = UT3D_skp_2vc (&vab, &vab);  // q-length of vab (length*length)
  lqcd = UT3D_skp_2vc (&vcd, &vcd);  // q-length of vcd
  // Test for 0 ?
    printf(" lqab=%lf lqcd=%lf\n",lqab,lqcd);


  s_ab_cd = UT3D_skp_2vc (&vab, &vcd);   // Laenge e-f relativ zu Laenge a-b
    printf(" s_ab_cd=%f\n",s_ab_cd);
  if(fabs(s_ab_cd) < UT_TOL_min0)
    return  UT3D_pt_int2pt2vcn_lim (px, ux_ab, ux_cd, pta, &vab, ptc, &vcd, tol);



  // swap ln1-ln2 if ln2 longer ln1; then ln1 is longer than ln2.
  if(lqcd > lqab) {
      // printf(" swap ln1 - ln2\n");
    MEM_swap_2vp (&pta, &ptc);
    MEM_swap_2vp (&ptb, &ptd);
    MEM_swap_2vp (&ux_ab, &ux_cd);
    MEM_swap_2db (&lqab, &lqcd);
    MEM_swap__ (&vab, &vcd, sizeof(Vector));
  }


  UT3D_vc_2pt (&vac, pta, ptc);
  UT3D_vc_2pt (&vad, pta, ptd);
    // DEB_dump_obj__ (Typ_VC, &vac, " vac =");
    // DEB_dump_obj__ (Typ_VC, &vad, " vad =");


  // get u_c_ab = parameter of c projected onto vab.
  u_c_ab = UT3D_skp_2vc (&vab, &vac);
  // get u_d_ab = parameter of d projected onto vab.
  u_d_ab = UT3D_skp_2vc (&vab, &vad);
    printf(" u_c_ab=%lf u_d_ab=%lf\n",u_c_ab,u_d_ab);



  // make ptc-ptd parallel to pta-ptb (not antiparallel)
  if(u_c_ab > u_d_ab) {
    // antiparallel
    MEM_swap_2db (&u_c_ab, &u_d_ab);
    MEM_swap_2vp (&ptc, &ptd);
    MEM_swap__ (&vac, &vad, sizeof(Vector));
    UT3D_vc_invert (&vcd, &vcd);
    icdrev = 1; // c-d reverted
  }


  // test if projection of vcd onto vab overlaps;
  iover = UTP_2db_ck_in2db (0., lqab, u_c_ab, u_d_ab);
    printf(" iover=%d\n",iover);


  // outside -
  if (iover == -2) {
    // ln2 is left of ln1
    if(!UT3D_comp2pt(ptd, pta, tol)) { irc = -1; goto L_exit; }
    // x == d == a
    *px = *pta;
    if(ux_ab) *ux_ab = 0.;
    if(ux_cd) *ux_cd = 1.;
    goto L_exit;

  } else if (iover == 2) {
    // ln2 is right of ln1
    if(!UT3D_comp2pt(ptb, ptc, tol)) { irc = -1; goto L_exit; }
    // x == b == c
    *px = *ptb;
    if(ux_ab) *ux_ab = 1.;
    if(ux_cd) *ux_cd = 0.;
    goto L_exit;
  }


  // get vector a-e
  UT3D_vc_multvc (&vae, &vab, u_c_ab / lqab);
    DEB_dump_obj__ (Typ_VC, &vae, " vae =");
    pb1 = *pta; UT3D_pt_add_vc__ (&pb1, &vae);
    GR_tDyn_symB__ (&pb1, SYM_TRI_S, ATT_COL_RED);

  // get vector e-c = Subtraktion (a-c - a-e)
  UT3D_vc_sub2vc (&vec, &vac, &vae);
    // DEB_dump_obj__ (Typ_VC, &vec, " vec =");

  // lqec = quadrat.length e-c
  lqec = UT3D_skp_2vc (&vec, &vec);
    // printf(" lqec=%lf\n",lqec);



  // get vector a-f
  UT3D_vc_multvc (&vaf, &vab, u_d_ab / lqab);
    DEB_dump_obj__ (Typ_VC, &vaf, " vaf =");
    pb1 = *pta; UT3D_pt_add_vc__ (&pb1, &vaf);
    GR_tDyn_symB__ (&pb1, SYM_TRI_S, ATT_COL_GREEN);

  // get vector f-d = Subtraktion (a-d - a-f)
  UT3D_vc_sub2vc (&vfd, &vad, &vaf);
    // DEB_dump_obj__ (Typ_VC, &vfd, " vfd =");

  // lqfd = quadrat.length f-d
  lqfd = UT3D_skp_2vc (&vfd, &vfd);
    // printf(" lqec=%lf lqfd=%lf\n",lqec,lqfd);


  // test if lines are parallel, but have distance > tol
  if(UTP_comp2db(lqec,lqfd,qtol)) {
    // yes - parallel; test if normal-distance > tol
    if(lqec > qtol) return -1;         // 2016-03-21
  }


  // test if c is on a-b AND d is on a-b
  if((lqec < qtol)&&(lqfd < qtol)) {
    // c-d covers a-b; find midpoint of overlapping range.
      // printf(" ln1 covers ln2 !\n");
    if(iover == -1) {
      // ln2 is left of ln1 (c-a-d-b); set px = mid a-d
      UT3D_pt_mid2pt (px, pta, ptd);
    } else if(iover == 0) {
      // ln2 inside ln1 (a-c-d-b); ln1 is longer.
      UT3D_pt_mid2pt (px, ptc, ptd);
      // if(ux_cd) *ux_cd = 0.5;
    } else if(iover == 1) {
      // ln2 is right of ln1 (a-c-b-d)
      UT3D_pt_mid2pt (px, ptc, ptb);
    }
    irc = 1;
    goto L_ux;
  }


  // test if ptc is on line pta-ptb
  if(lqec < qtol) {
    // ptc is on pta-ptb
      // printf(" c is on a-b\n");
    if (iover == -1) {
      // ln2 is left of ln1 (c-a-d-b)
      if(!UT3D_comp2pt(ptc, pta, tol)) { irc = -1; goto L_exit; }
    }
    *px = *ptc;
    goto L_ux;
  }


  // test if ptd is on line pta-ptb
  if(lqfd < qtol) {
    // ptd is on pta-ptb
      // printf(" d is on a-b\n");
    if(iover == 1) {
      // ln2 is right of ln1 (a-c-b-d)
      if(!UT3D_comp2pt(ptb, ptd, tol)) { irc = -1; goto L_exit; }
    }
    *px = *ptd;
    goto L_ux;
  }


  // intersect lines a-b  c-d
  s_ac_cd = UT3D_skp_2vc (&vac, &vcd);
    // printf(" s_ac_cd=%lf\n",s_ac_cd);

  q0 = lqab * lqcd - s_ab_cd * s_ab_cd;

  if(fabs(q0) < UT_TOL_min2) {
    // internal error (should be handled before)
    irc = -1;
    printf("UT3D_pt_int2pt2pt_lim - internal error \n");
    goto L_exit;
  }

  q1 = (u_c_ab * lqcd) - (s_ac_cd * s_ab_cd);

  // u_x_ab = parameter of intersect-point on a-b
  u_x_ab = q1 / q0;
  if(ux_ab) *ux_ab = u_x_ab;
    // printf(" u_x_ab=%lf q1=%lf q0=%lf\n",u_x_ab,q1,q0);

  // u_x_cd = parameter of intersect-point on c-d
  u_x_cd = ((s_ab_cd * u_x_ab) - s_ac_cd) / lqcd;
  if(ux_cd) *ux_cd = u_x_cd;
    // printf(" u_x_cd=%lf q1=%lf q0=%lf\n",u_x_cd,q1,q0);

  // get vector a-x
  UT3D_vc_multvc (&vax, &vab, u_x_ab);
    // DEB_dump_obj__ (Typ_VC, &vax, " vax =");

  // px from vax
  UT3D_pt_traptvc (px, pta, &vax);


  // test intersectpoint left | right of line a-b
  if(u_x_ab < 0.) {
    // intersectpoint is left of a
    if(UT3D_comp2pt(px, pta, tol)) goto L_exit;
    irc = -1;
    goto L_exit;

  } else if(u_x_ab > 1.) {
    // intersectpoint is right of b
    if(UT3D_comp2pt(px, ptb, tol)) goto L_exit;
    irc = -1;
    goto L_exit;
  }


  // test intersectpoint left | right of line c-d
  if(u_x_cd < 0.) { 
    // intersectpoint is left of c
    if(UT3D_comp2pt(px, ptc, tol)) goto L_exit;
    irc = -1;
    goto L_exit;
  
  } else if(u_x_cd > 1.) {
    // intersectpoint is right of d
    if(UT3D_comp2pt(px, ptd, tol)) goto L_exit;
    irc = -1;
    goto L_exit;
  }


  //----------------------------------------------------------------
  L_exit:
    if(icdrev) {
      // c-d reverted
      if(ux_cd) *ux_cd = 1. - *ux_cd;
    }


    printf("ex _int2pt2pt irc=%d\n",irc);
    if(ux_ab) printf(" ux_ab=%lf\n",*ux_ab);
    if(ux_cd) printf(" ux_cd=%lf\n",*ux_cd);
    if(irc != -1) DEB_dump_obj__ (Typ_PT, px, " px =");
    // exit(0);

  return irc;


  //----------------------------------------------------------------
  L_ux:
      printf(" L_ux: irc=%d\n",irc);

    // get u_x_ab = qadr.length of vax from x
    if(ux_ab) {
      // u_x_ab = parameter of x projected onto vab.
      UT3D_vc_2pt (&vax, pta, px);
      u_x_ab = UT3D_skp_2vc (&vab, &vax);
      *ux_ab = u_x_ab / lqab;
    }
    
    // get u_x_cd = qadr.length of vax from x
    if(ux_cd) {
      // u_x_cd = parameter of x projected onto vcd.
      UT3D_vc_2pt (&vcx, ptc, px);
      u_x_cd = UT3D_skp_2vc (&vcd, &vcx);
      *ux_cd = u_x_cd / lqcd;
    }
    goto L_exit;

}
*/

//=====================================================================
  int UT3D_pt_int2pt2vcn_lim (Point *px, double *ux_ab, double *ux_cd,
                              Point *pta, Vector *vab,
                              Point *ptc, Vector *vcd, double tol) {
//=====================================================================
/// \code
/// UT3D_pt_int2pt2vcn_lim            intersect 2 limited NORMAL lines
/// get intersectionpoint and parameters
///
/// IN:
///   pta, vab         line 1
///   ptc, vcd         line 2
/// OUT:
///   Point *px        intersectionpoint
///   double *ux_ab     parameter of px on line a-b  (none if NULL)
///   double *ux_cd     parameter of px on line c-d  (none if NULL)
/// Returncodes:
///    -1 = no intersection inside the segments
///     0 = OK; 
///
/// a-b = ln1    c-d = ln2
///              
///             d
///             |
///             |
///             |
///             |
///             |
///       a-----x-------------------b
///             |          ln1
///             | 
///             c
///
///
/// \endcode


  double     qtol, par1, par2, lqab, lqcd, s_ab_ac, s_cd_ac;
  Point      ptb, ptd;
  Vector     vac, vax;


  qtol = tol * tol;


  // printf("UT3D_pt_int2pt2vcn_lim tol=%f qtol=%lf\n",tol,qtol);
  // DEB_dump_obj__ (Typ_PT, pta, " pta =");
  // DEB_dump_obj__ (Typ_VC, vab, " vab=");
  // DEB_dump_obj__ (Typ_PT, ptc, " ptc =");
  // DEB_dump_obj__ (Typ_VC, vcd, " vcd=");


  UT3D_vc_2pt (&vac, pta, ptc);

  lqab = UT3D_skp_2vc (vab, vab);  // q-length of vab (length*length)
    // printf(" lqab=%f\n",lqab);


  s_ab_ac = UT3D_skp_2vc (&vac, vab);
    // printf(" s_ab_ac=%f\n",s_ab_ac);


  par1 = s_ab_ac / lqab;
    // printf(" par-ab = %f\n",par1);



  lqcd = UT3D_skp_2vc (vcd, vcd);  // q-length of vcd (length*length)

  s_cd_ac = UT3D_skp_2vc (&vac, vcd);
  s_cd_ac *= -1.;

  par2 = s_cd_ac / lqcd;
    // printf(" par-cd = %f\n",par2);


  // get vector ax
  UT3D_vc_multvc (&vax, vab, par1);
    // DEB_dump_obj__ (Typ_VC, &vax, " vax =");


  // px from vax
  UT3D_pt_traptvc (px, pta, &vax);
    // DEB_dump_obj__ (Typ_PT, px, " px =");


  // test px left of vab
  if(UT2D_comp2pt(px, pta, tol)) goto L_ab_ok;
  if(par1 < 0.) return -1;

  // test px left of vab
  ptb = *pta;
  UT3D_pt_add_vc__(&ptb, vab);
  if(UT2D_comp2pt(px, &ptb, tol)) goto L_ab_ok;
  if(par1 > 1.) return -2;


  L_ab_ok:

  // test px left of vcd
  if(UT2D_comp2pt(px, ptc, tol)) goto L_cd_ok;
  if(par2 < 0.) return -3;

  // test px left of vcd
  ptd = *ptc;
  UT3D_pt_add_vc__(&ptd, vcd);
  if(UT2D_comp2pt(px, &ptd, tol)) goto L_cd_ok;
  if(par2 > 1.) return -4;


  L_cd_ok:

  if(ux_ab) *ux_ab = par1;
  if(ux_cd) *ux_cd = par2;

    // printf("ex UT3D_pt_int2pt2vcn_lim irc=0 par-ab=%f ipar-cd=%f\n",par1,par2);
    // DEB_dump_obj__ (Typ_PT, px, " px =");


  return 0;

}


//=======================================================================
  int UT3D_pt_intptvcplx (Point *px, Point *pl, Vector *vl, double plx) {
//=======================================================================
/// \code
/// UT3D_pt_intptvcplx              intersect point/vector with Y-Z-plane
/// irc    0  OK px = Schnittpunkt
/// irc   -1  kein Schnittpunkt (vl parallel pln)
/// \endcode


  double  dx, f;
  Vector  v1;

  // printf("UT3D_pt_intptvcplx plx=%f\n",plx);
  // DEB_dump_obj__ (Typ_PT, pl, " pl");
  // DEB_dump_obj__ (Typ_VC, vl, " vl");

  if(fabs(vl->dx) < UT_TOL_min2) return -1;

  dx = plx - pl->x;
  f = dx / vl->dx;
  // printf(" dx=%f f=%f\n",dx,f);

  v1.dx = f * vl->dx;
  v1.dy = f * vl->dy;
  v1.dz = f * vl->dz;

  px->x = pl->x + v1.dx;
  px->y = pl->y + v1.dy;
  px->z = pl->z + v1.dz;

  // GR_tDyn_symB__ (px, SYM_STAR_S, 0);
  // DEB_dump_obj__ (Typ_PT, px, "ex UT3D_pt_intptvcplx");

  return 0;

}


//=======================================================================
  int UT3D_pt_intptvcply (Point *px, Point *pl, Vector *vl, double ply) {
//=======================================================================
/// \code
/// UT3D_pt_intptvcply              intersect point/vector with X-Z-plane
/// irc    0  OK px = Schnittpunkt
/// irc   -1  kein Schnittpunkt (vl parallel pln)
/// \endcode


  double  dy, f;
  Vector  v1;

  // printf("UT3D_pt_intptvcply ply=%f\n",ply);
  // DEB_dump_obj__ (Typ_PT, pl, " pl");
  // DEB_dump_obj__ (Typ_VC, vl, " vl");

  if(fabs(vl->dy) < UT_TOL_min2) return -1;

  dy = ply - pl->y;
  f = dy / vl->dy;
  // printf(" dy=%f f=%f\n",dy,f);

  v1.dx = f * vl->dx;
  v1.dy = f * vl->dy;
  v1.dz = f * vl->dz;

  px->x = pl->x + v1.dx;
  px->y = pl->y + v1.dy;
  px->z = pl->z + v1.dz;

  // GR_tDyn_symB__ (px, SYM_STAR_S, 0);
  // DEB_dump_obj__ (Typ_PT, px, "ex UT3D_pt_intptvcply");

  return 0;

}


//=======================================================================
  int UT3D_pt_intptvcplz (Point *px, Point *pl, Vector *vl, double plz) {
//=======================================================================
/// \code
/// UT3D_pt_intptvcplz              intersect point/vector with X-Y-plane
/// irc    0  OK px = Schnittpunkt
/// irc   -1  kein Schnittpunkt (vl parallel pln)
/// \endcode


  double  dz, f;
  Vector  v1;

  // printf("UT3D_pt_intptvcplz plz=%f\n",plz);
  // DEB_dump_obj__ (Typ_PT, pl, " pl");
  // DEB_dump_obj__ (Typ_VC, vl, " vl");

  if(fabs(vl->dz) < UT_TOL_min2) return -1;

  dz = plz - pl->z;
  f = dz / vl->dz;
  // printf(" dz=%f f=%f\n",dz,f);

  v1.dx = f * vl->dx;
  v1.dy = f * vl->dy;
  v1.dz = f * vl->dz;

  px->x = pl->x + v1.dx;
  px->y = pl->y + v1.dy;
  px->z = pl->z + v1.dz;

  // GR_tDyn_symB__ (px, SYM_STAR_S, 0);
  // DEB_dump_obj__ (Typ_PT, px, "ex UT3D_pt_intptvcplz");

  return 0;

}


//================================================================
  int UT3D_pt_intptvczpln (Point *pti, Point *ptl,
                           Point *ptpl,  Vector *vcpl) {
//================================================================
/// \code
/// UT3D_pt_intptvczpln      intersect line(z-parallel) X plane (pt-vc)
/// Input:
///   ptl    Point on line (direction of line = 0,0,1)
///   ptpl   Point in plane
///   vcpl   perpendicular Vector to plane (muss nicht normiert sein);
/// Returncodes:
///   0 = OK
///   1 = no intersection (parallel)
///
/// See also UT3D_ln_int2pl UT3D_ptvc_int2pl UT3D_pt_intptvcpl
/// \endcode


  double  k1, k2, dist, p;
  Vector  v1;

  // GR_tDyn_vc__ (vcl, ptl, 3, 0);
  // GR_tDyn_vc__ (vcpl, ptpl, 3, 0);


  k1 = vcpl->dz;
  // printf(" k1=%f\n",k1);

  if(fabs(k1) < UT_TOL_min2) return 0;

  p =    vcpl->dx * ptpl->x +
         vcpl->dy * ptpl->y +
         vcpl->dz * ptpl->z;
  // printf(" p=%f\n",p);

  k2 = p - vcpl->dx * ptl->x -
           vcpl->dy * ptl->y -
           vcpl->dz * ptl->z;
  // printf(" k2=%f\n",k2);

  dist = k2/k1;
  // printf("  intptvcpl %f %f %f\n",k1,k2,dist);

  *pti = *ptl;
  pti->z += dist;

  // printf("ex UT3D_pt_intptvcpl_ %f %f %f\n",ip->x,ip->y,ip->z);
  // GR_tDyn_symB__ (pti, SYM_TRI_S, 2);

  return 0;

}


//================================================================
  int UT3D_pt_intptvcxpln (Point *pti, Point *ptl,
                           Point *ptpl,  Vector *vcpl) {
//================================================================
/// \code
/// UT3D_pt_intptvcxpln      intersect line(x-parallel) X plane (pt-vc)
/// Input:
///   ptl    Point on line (direction of line = 1,0,0)
///   ptpl   Point in plane
///   vcpl   perpendicular Vector to plane (muss nicht normiert sein);
/// Returncodes:
///   0 = OK
///   1 = no intersection (parallel)
///
/// See also UT3D_ln_int2pl UT3D_ptvc_int2pl UT3D_pt_intptvcpl
/// \endcode


  double  k1, k2, dist, p;
  Vector  v1;

  // GR_tDyn_vc__ (vcl, ptl, 3, 0);
  // GR_tDyn_vc__ (vcpl, ptpl, 3, 0);


  k1 = vcpl->dx;
  // printf(" k1=%f\n",k1);

  if(fabs(k1) < UT_TOL_min2) return 0;

  p =    vcpl->dx * ptpl->x +
         vcpl->dy * ptpl->y +
         vcpl->dz * ptpl->z;
  // printf(" p=%f\n",p);

  k2 = p - vcpl->dx * ptl->x -
           vcpl->dy * ptl->y -
           vcpl->dz * ptl->z;
  // printf(" k2=%f\n",k2);

  dist = k2/k1;
  // printf("  intptvcpl %f %f %f\n",k1,k2,dist);

  *pti = *ptl;
  pti->x += dist;

  // printf("ex UT3D_pt_intptvcpl_ %f %f %f\n",ip->x,ip->y,ip->z);
  // GR_tDyn_symB__ (pti, SYM_TRI_S, 2);

  return 0;

}


//================================================================
  int UT3D_pt_intptvcypln (Point *pti, Point *ptl,
                           Point *ptpl,  Vector *vcpl) {
//================================================================
/// \code
/// UT3D_pt_intptvcypln      intersect line(y-parallel) X plane (pt-vc)
/// Input:
///   ptl    Point on line (direction of line = 0,1,0)
///   ptpl   Point in plane
///   vcpl   perpendicular Vector to plane (muss nicht normiert sein);
/// Returncodes:
///   0 = OK
///   1 = no intersection (parallel)
///
/// See also UT3D_ln_int2pl UT3D_ptvc_int2pl UT3D_pt_intptvcpl
/// \endcode


  double  k1, k2, dist, p;
  Vector  v1;

  // GR_tDyn_vc__ (vcl, ptl, 3, 0);
  // GR_tDyn_vc__ (vcpl, ptpl, 3, 0);


  k1 = vcpl->dy;
  // printf(" k1=%f\n",k1);

  if(fabs(k1) < UT_TOL_min2) return 0;

  p =    vcpl->dx * ptpl->x +
         vcpl->dy * ptpl->y +
         vcpl->dz * ptpl->z;
  // printf(" p=%f\n",p);

  k2 = p - vcpl->dx * ptl->x -
           vcpl->dy * ptl->y -
           vcpl->dz * ptl->z;
  // printf(" k2=%f\n",k2);

  dist = k2/k1;
  // printf("  intptvcpl %f %f %f\n",k1,k2,dist);

  *pti = *ptl;
  pti->y += dist;

  // printf("ex UT3D_pt_intptvcpl_ %f %f %f\n",ip->x,ip->y,ip->z);
  // GR_tDyn_symB__ (pti, SYM_TRI_S, 2);

  return 0;

}


//=====================================================================
  int UT3D_pt_int2pt2pt_lim (Point *px, double *ux_ab, double *ux_cd,
                             Point *pta, Point *ptb,
                             Point *ptc, Point *ptd, double tol) {
//=====================================================================
/// \code
/// UT3D_pt_int2pt2pt_lim            intersect 2 limited lines
/// get intersectionpoint and parameters
///
/// IN:
///   pta, ptb         line 1
///   ptc, ptd         line 2
/// OUT:
///   Point *px        intersectionpoint
///   double *ux_ab     parameter of px on line a-b  (none if NULL)
///   double *ux_cd     parameter of px on line c-d  (none if NULL)
/// Returncodes:
///    -1 = no intersection inside the segments
///     0 = OK; dist gives the minimum distance.
///     1 = Line a-b parallel and covering c-d (px set to midpoint)
///
///                       d
///                      /.
///                     / .
///                 ln2/  .
///                   /   .
///                  /    .
///                 /     .
///       a-----e--x------f-------------b
///             | /            ln1
///             |/
///             c
///
///
/// \endcode


  int       irc, iover;
  double    qtol, u_ab, u_cd, dec;
  double    s_ab_ab, s_ab_ac, s_ab_cd, s_ac_cd, s_cd_cd, s_ab_ad, q0, q1, q2;
  Vector    vab, vcd, vac, vad, vax, vcx, vae, vec;
  Point     px1, px2;

  qtol = tol * tol;

  // printf("UT3D_pt_int2pt2pt_lim tol=%f qtol=%lf\n",tol,qtol);
  // DEB_dump_obj__ (Typ_PT, pta, " pta =");
  // DEB_dump_obj__ (Typ_PT, ptb, " ptb =");
  // DEB_dump_obj__ (Typ_PT, ptc, " ptc =");
  // DEB_dump_obj__ (Typ_PT, ptd, " ptd =");


  UT3D_vc_2pt (&vab, pta, ptb);
  UT3D_vc_2pt (&vcd, ptc, ptd);
  UT3D_vc_2pt (&vac, pta, ptc);


  //----------------------------------------------------------------
  // check box     UT3D_box_2pt_tol UT3D_ckBoxinBox1


  //----------------------------------------------------------------
  s_ab_ab = UT3D_skp_2vc (&vab, &vab);  // length ab ^2
  s_ab_ac = UT3D_skp_2vc (&vab, &vac);  // ie = parameter c on ab
  s_ab_cd = UT3D_skp_2vc (&vab, &vcd);  // Laenge von e-f relativ zu s_ab_ab
  s_ac_cd = UT3D_skp_2vc (&vac, &vcd);  // 
  s_cd_cd = UT3D_skp_2vc (&vcd, &vcd);  // length cd ^2
    // printf(" s_ab_ab=%f\n",s_ab_ab);
    // printf(" s_ab_ac=%f\n",s_ab_ac);
    // printf(" s_ab_cd=%f\n",s_ab_cd);
    // printf(" s_ac_cd=%f\n",s_ac_cd);
    // printf(" s_cd_cd=%f\n",s_cd_cd);


  q0 = s_ab_ab * s_cd_cd - s_ab_cd * s_ab_cd;

  if(fabs(q0) < UT_TOL_min2) goto L_parall;

  q1 = s_ab_ac * s_cd_cd - s_ac_cd * s_ab_cd;
    // printf(" q0=%f q1=%f\n",q0,q1);


  u_ab = q1 / q0;

  u_cd = ((s_ab_cd * (u_ab)) - s_ac_cd) / s_cd_cd;

  UT3D_vc_multvc (&vax, &vab, u_ab);
  UT3D_pt_traptvc (&px1, pta, &vax);
    // DEB_dump_obj__ (Typ_PT, &px1, " px1");

  UT3D_vc_multvc (&vcx, &vcd, u_cd);
  UT3D_pt_traptvc (&px2, ptc, &vcx);
    // DEB_dump_obj__ (Typ_PT, &px2, " px2");


  // test distance px1 - px2
  if(!UT3D_comp2pt(&px1, &px2, tol)) {irc = -1; goto L_exit;}

  UT3D_pt_mid2pt (px, &px1, &px2);


  // test px left of vab
  if(UT3D_comp2pt(px, pta, tol)) goto L_ab_ok;
  if(u_ab < 0.) return -1;

  // test px right of vab
  if(UT3D_comp2pt(px, ptb, tol)) goto L_ab_ok;
  if(u_ab > 1.) return -2;


  L_ab_ok:

  // test px left of vcd
  if(UT3D_comp2pt(px, ptc, tol)) goto L_cd_ok;
  if(u_cd < 0.) return -3;

  // test px right of vcd
  if(UT3D_comp2pt(px, ptd, tol)) goto L_cd_ok;
  if(u_cd > 1.) return -4;


  L_cd_ok:
  irc = 0;
  if(ux_ab) *ux_ab = u_ab;
  if(ux_cd) *ux_cd = u_cd;



    // TESTBLOCK
    // printf(" q0=%f q1=%f u_ab=%f u_cd=%f\n",q0,q1,u_ab,u_cd);
    // GR_tDyn_symB__ (&px1, SYM_STAR_S, ATT_COL_RED);
    // END TESTBLOCK



  //----------------------------------------------------------------
  L_exit:

    // printf("ex _int2pt2pt irc=%d\n",irc);
    // if(ux_ab) printf(" ux_ab=%lf\n",*ux_ab);
    // if(ux_cd) printf(" ux_cd=%lf\n",*ux_cd);
    // if(irc != -1) DEB_dump_obj__ (Typ_PT, px, " px =");
    // exit(0);

  return irc;


  //----------------------------------------------------------------
  L_parall:
    // printf(" L_parall\n");
  // get normal dist of c on ab
  // Parameter of e on ab
  u_ab = s_ab_ac / s_ab_ab;
  // Multip. des Vektors a-b mit Parameterwert von e ergibt den Vektor a-e
  UT3D_vc_multvc (&vae, &vab, u_ab);
  // Subtraktion (Vec-a-c - Vec-a-e) = Vec-e-c
  UT3D_vc_sub2vc (&vec, &vac, &vae);
  dec = UT3D_len_vc (&vec);

  if(dec > tol) return -1;


  // test overlappimg
  UT3D_vc_2pt (&vad, pta, ptd);
  s_ab_ad = UT3D_skp_2vc (&vab, &vad);
  iover = UTP_2db_ck_in2db (0., s_ab_ab, s_ab_ac, s_ab_ad);
    // printf(" s_ab_ad=%f iover=%d\n", s_ab_ad, iover);

  if(iover < -2) return -1;

  if(iover == -2)      UT3D_pt_mid2pt (px, pta, ptd);
  else if(iover == -1) UT3D_pt_mid2pt (px, ptc, ptb);
  else if(iover == 1)  UT3D_pt_mid2pt (px, ptc, ptd);
  else if(iover == 2)  UT3D_pt_mid2pt (px, pta, ptb);

  irc = 1;
  goto L_exit;


  //----------------------------------------------------------------
  L_err1:
  return -1;
}


//================================================================
  int UT3D_2par_int2pt2vc (double *par1, double *par2,
                          Point *pa, Vector *vab,
                          Point *pc, Vector *vcd) {
//================================================================
/// \code
/// UT3D_2par_int2pt2vc     intersect 2 unlimitedLines; gives 2 parameters
/// Input:
///   pa-vab   Line1 (pa-pb)
///   pc-vcb   Line2 (pc-pd)
/// Output:
///   par1     Parameter of IntersectPt on pa-pb
///   par2     Parameter of IntersectPt on pc-pd
///   retCod   -1 = parallel
///             0 = OK;
///
///                         d
///                        /.
///                       / .
///                      /  .
///                     /   .
///                    c    .
///                   ..    .
///                  . .    .
///                 .  .    .
///          a-----+---e----f-----------------------b
///               ip
/// \endcode

  int       irc;
  double    s_ab_ab, s_ab_ac, s_ab_cd, s_ac_cd, s_cd_cd, q0, q1, q2;
  Vector    vac;  //, vad;
  Point     pm1, pm2;


  // printf("UT3D_2par_int2pt2vc \n");
  // DEB_dump_obj__ (Typ_PT, pa,  " pa =");
  // DEB_dump_obj__ (Typ_VC, vab, " vab =");
  // DEB_dump_obj__ (Typ_PT, pc,  " pc =");
  // DEB_dump_obj__ (Typ_VC, vcd, " vcd =");


/*
  // check for lines parallel or antiparallel
  if (UT3D_comp2vc_p (vab, vcd, UT_TOL_min1)) {
    //printf("UT3D_pt_int2ln parallel error\n");
    irc = -1;
    // UT3D_pt_projptptvc (&ip2, dist, NULL, p1, p3, &v);
    goto Done;
  }
*/

  UT3D_vc_2pt (&vac, pa, pc);           // vac = Vector a-c
    // DEB_dump_obj__ (Typ_VC, &vac, " vac =");

  s_ab_ab = UT3D_skp_2vc (vab, vab);  // skp mit sich selbst = Laenge^2
  s_ab_ac = UT3D_skp_2vc (vab, &vac);  // gibt Wert fuer e relativ zu s_ab_ab
  s_ab_cd = UT3D_skp_2vc (vab, vcd);   // Laenge von e-f relativ zu s_ab_ab
  s_ac_cd = UT3D_skp_2vc (&vac, vcd);
  s_cd_cd = UT3D_skp_2vc (vcd, vcd);
    // printf(" s_ab_ab=%f\n",s_ab_ab);
    // printf(" s_ab_ac=%f\n",s_ab_ac);
    // printf(" s_ab_cd=%f\n",s_ab_cd);
    // printf(" s_ac_cd=%f\n",s_ac_cd);
    // printf(" s_cd_cd=%f\n",s_cd_cd);

  // q1 = param vom pi1 auf ab  (zw 0 und 1)
  // q2 = param vom pi2 auf cd  (zw 0 und 1)
  // q0 = (s_ab_ab * s_cd_cd) - (s_ab_cd * s_ab_cd);
  // if(fabs(q0) < UT_TOL_min1) return 1;
  // *par1 = ((s_ab_ac * s_cd_cd) - (s_ab_cd * s_ac_cd)) / q0;
  // *par2 = ((s_ab_ac * s_ab_cd) - (s_ab_ab * s_ac_cd))  / q0;


  q0 = s_ab_ab * s_cd_cd - s_ab_cd * s_ab_cd;

  if(fabs(q0) < UT_TOL_min2) {irc = -1; goto Done;}
  // make comparable
  // if((fabs(q0) * s_ab_ab / s_ab_cd) < UT_TOL_min1) {irc = -1; goto Done;}

  q1 = (s_ab_ac * s_cd_cd) - (s_ac_cd * s_ab_cd);

  *par1 = q1 / q0;

  *par2 = ((s_ab_cd * (*par1)) - s_ac_cd) / s_cd_cd;

  irc = 0;


    // // TESTBLOCK:
    // {Point ip1, ip2;
      // ip1.x = pa->x + *par1 * vab->dx;
      // ip1.y = pa->y + *par1 * vab->dy;
      // ip1.z = pa->z + *par1 * vab->dz;
      // DEB_dump_obj__ (Typ_PT, &ip1, " ip1 =");
 //  
      // ip2.x = pc->x + *par2 * vcd->dx;
      // ip2.y = pc->y + *par2 * vcd->dy;
      // ip2.z = pc->z + *par2 * vcd->dz;
      // DEB_dump_obj__ (Typ_PT, &ip2, " ip2 =");
    // }
    // // END TESTBLOCK:


  Done:

    // printf("ex UT3D_2par_int2pt2vc irc=%d par1=%f par2=%f\n",irc,*par1,*par2);

  return irc;

}


//================================================================
  int UT3D_pt1_int2pt2pt (Point *ip,
                          Point *pa, Point *pb, Point *pc, Point *pd) {
//================================================================
/// \code
/// UT3D_pt1_int2pt2pt        intersect 2 unlimitedLines; point on pa-pb out.
/// Input:
///   pa-pb    Line1
///   pc-pd    Line2
/// Output:
///   ip = intersectionpoint on pa-pb
///   irc = -1 = parallel
///   irc =  0 = OK;
/// \endcode


  int       irc;
  double    q1, q2;
  Point     pm1, pm2;
  Vector    vab, vcd;


  UT3D_vc_2pt (&vab, pa, pb);           // vab = Vector a-b
  UT3D_vc_2pt (&vcd, pc, pd);           // vcd = Vector c-d

  irc = UT3D_2par_int2pt2vc (&q1, &q2, pa, &vab, pc, &vcd);
  if(irc != 0) {     //- parallel !
    UT3D_pt_mid2pt (&pm1, pa, pb);
    UT3D_pt_mid2pt (&pm2, pc, pd);
    UT3D_pt_mid2pt (ip, &pm1, &pm2);

  } else {
    // ip = intersectionPoint on pa-pb.
    ip->x = pa->x + vab.dx * q1;
    ip->y = pa->y + vab.dy * q1;
    ip->z = pa->z + vab.dz * q1;
  }


    // GR_tDyn_symB__ (ip, SYM_STAR_S, 0);
    // DEB_dump_obj__ (Typ_PT, ip, "ex UT3D_pt1_int2pt2pt %d ip =",irc);

  return irc;

}


//===========================================================================
  int UT3D_pt_int2pt2vc (Point *ip1, Point *ip2, double *dist,
                         Point *ptu, Vector *vcu, Point *ptv, Vector *vcv) {
//===========================================================================
/// \code
/// UT3D_pt_int2pt2vc     intersect 2 rays
/// 
/// ip1 liegt auf ray1 (ptu/vcu)
/// ip2 liegt auf ray2 (ptv/vcv)
/// 
/// Returncodes:
/// 0 = Lines parallel or zero-length
/// 1 = OK; dist gives the minimum distance.
/// 2 = OK; lines are crossing, ip1 = ip2, dist = 0.
/// \endcode



  int     rc;
  double  q0, q1, q2, uu, vv, uv, wu, wv;
  Vector  vi, w;


  // line1Start -> line2Start
  UT3D_vc_2pt (&w, ptu, ptv);


  // printf("UT3D_pt_int2pt2vc u=%f,%f,%f\n",vcu->dx,vcu->dy,vcu->dz);
  // printf("                  v=%f,%f,%f\n",vcv->dx,vcv->dy,vcv->dz);
  // printf("                  w=%f,%f,%f\n",w.dx,w.dy,w.dz);


/*
  // check for lines parallel ?
  if (UT3D_comp2vc_p (vcu, vcv, UT_TOL_min1)) {
    //printf("UT3D_pt_int2pt2vc parallel error\n");
    rc = 0;
    UT3D_pt_projptptvc (ip2, dist, NULL, ptu, ptv, vcv);
    goto Done;
  }
  UT3D_vc_invert (&vi, vcv);
  if (UT3D_comp2vc_p (vcu, &vi, UT_TOL_min1)) {
    //printf("UT3D_pt_int2pt2vc parallel error\n");
    rc = 0;
    UT3D_pt_projptptvc (ip2, dist, NULL, ptu, ptv, vcv);
    goto Done;
  }
*/

  // some scalar products
  uu = vcu->dx * vcu->dx + vcu->dy * vcu->dy + vcu->dz * vcu->dz;
  vv = vcv->dx * vcv->dx + vcv->dy * vcv->dy + vcv->dz * vcv->dz;
  uv = vcu->dx * vcv->dx + vcu->dy * vcv->dy + vcu->dz * vcv->dz;
  wu = w.dx * vcu->dx + w.dy * vcu->dy + w.dz * vcu->dz;
  wv = w.dx * vcv->dx + w.dy * vcv->dy + w.dz * vcv->dz;

  q0 = (uu * vv) - (uv * uv);
  if(fabs(q0) < UT_TOL_min1) {             // - parallel !
    //printf("UT3D_pt_int2pt2vc parallel error\n");
    rc = 0;
    UT3D_pt_projptptvc (ip2, dist, NULL, ptu, ptv, vcv);
    goto Done;
  }



  q1 = ((vv * wu) - (uv * wv)) / q0;
  // q1 = ((vv * wu) - (uv * wv)) / ((uu * vv) - (uv * uv));
  // printf("           q1 = %f\n",q1);

  ip1->x = ptu->x + vcu->dx * q1;
  ip1->y = ptu->y + vcu->dy * q1;
  ip1->z = ptu->z + vcu->dz * q1;



  q2 = ((wu * uv) - (uu * wv))  / q0;

  ip2->x = ptv->x + vcv->dx * q2;
  ip2->y = ptv->y + vcv->dy * q2;
  ip2->z = ptv->z + vcv->dz * q2;

  *dist = UT3D_len_2pt(ip1, ip2);

  // UT3D_pt_projptptvc (ip2, dist, NULL, ip1, ptv, vcv);

  if (*dist < UT_TOL_min1) {
    rc = 2;
  } else {
    rc = 1;
  }

  Done:

  // printf("ex UT3D_pt_int2pt2vc %d %f\n",rc,*dist);
  // printf("      %f %f %f\n",ip1->x,ip1->y,ip1->z);
  // printf("      %f %f %f\n",ip2->x,ip2->y,ip2->z);


  return rc;

}



//==================================================================
  int UT3D_ci_cicird (Circ ca[],
                      Circ *ci1, Circ *ci2, double rad1, int sNr) {
//==================================================================
/// \code
/// UT3D_ci_lncird            Circ = tangent to CIR + CIR, radius.
/// 
///   sNr       solutionNr,  0-7   (0=first, 1=second, 7=last)
///   RC = 1,2 = rn circles
///        -1  = Error - no solution
/// \endcode


  int      irc, i1, i2, i3, is34, is56, is78, ySid;
  double   dx, dy, lc, la, lb, d1, r1, r2, rd, dd, dx1, dx2, dx3, dGap;
  Point    ptx;
  Vector   vx, vy;
  Circ     c1, c2;


  // printf("UT3D_ci_cicird r=%lf sNr=%d\n",rad1,sNr);
  // DEB_dump_obj__ (Typ_CI, ci1, "  ci1");
  // DEB_dump_obj__ (Typ_CI, ci2, "  ci2");


  irc  = 0;

  // make sNr 1-8 from 0-7
  if(sNr < 0) sNr = 0;
  ++sNr;
  // make side above=1, below=0.
  // ySid = 1;
  ySid = sNr % 2;
    // printf(" sNr=%d ySid=%d\n",sNr,ySid);


  r1 = fabs(ci1->rad);
  r2 = fabs(ci2->rad);
  rd = fabs(rad1);
  dd = rd * 2.;
    // printf(" r1=%f r2=%f dd=%f\n",r1,r2,dd);

  ca[0].rad  = rad1;
  ca[0].vz   = ci1->vz;
  ca[0].ango = RAD_360;

  // lc = Abstand pc1 - pc2
  lc = UT3D_len_2pt (&ci1->pc, &ci2->pc);
    // printf(" rd=%f lc=%f\n",rd,lc);

  // vx = ci1->pc --> ci2->pc
  UT3D_vc_2pt (&vx, &ci1->pc, &ci2->pc);
  // vy = Normalvec to vx
  UT3D_vc_perp2vc (&vy, &ci1->vz, &vx);
    // DEB_dump_obj__ (Typ_VC, &vx, "  vx:");
    // DEB_dump_obj__ (Typ_VC, &vy, "  vy:");


  // wenn gap > (2 * rd):  gar keine Loesung.
  dGap = lc - r1 - r2;
  if(dGap > dd) return -1;


  // test if circ touches circ inside ..
  if(UTP_comp2db(dGap, dd, UT_TOL_pt)) goto L_1sol;
  

  // wenn ein kreis den zweiten innen beruehrt -
  if(r1 > r2) {
    if(UTP_comp2db((lc+r2), r1, UT_TOL_pt)) goto L_C_AT_C1;
  } else {
    if(UTP_comp2db((lc+r1), r2, UT_TOL_pt)) goto L_C_AT_C2;
  }


  // test ob sich die InputKreis von aussen beruehren
  if(UTP_comp2db(dGap, 0., UT_TOL_pt)) goto L_touchOut;  // 4 Loesungen
    // printf("  gapsize dGap=%f\n",dGap);


  // test if circs overlap
  if(dGap < 0.) goto L_over;


  //----------------------------------------------------------------
  // circs have a gap;

  // sNr 1, 2 = around c1 and around c2: always possible
  //          1
  //    c1         c2
  //          2


  // sNr 5, 6: test for around c1 touching c2
  //          1
  //    c1  3.1    c2
  //          2
  d1 = lc + r1 - r2;
  if(UTP_comp2db(dd, d1, UT_TOL_pt)) is56 = 1;      // exact; 1 solution
  else if(dd < d1) is56 = 0;                        // keine solution
  else if(dd > d1) is56 = 2;                        // 2 solutions
    // printf("is56=%d d1=%f\n",is56,d1);


  // sNr 3, 4: test for touching c1 around c2
  //          1
  //    c1    3.2  c2
  //          2
  d1 = lc - r1 + r2;
  if(UTP_comp2db(dd, d1, UT_TOL_pt)) is34 = 1;      // exact; 1 solution
  else if(dd < d1) is34 = 0;                        // keine solution
  else if(dd > d1) is34 = 2;                        // 2 solutions
    // printf("is34=%d d1=%f\n",is34,d1);


  // sNr 7, 8: test for around c1 and around c2
  //         3.3
  //          1
  //    c1    3.2  c2
  //          2
  //         3.4
  d1 = lc + r1 + r2;
  if(UTP_comp2db(dd, d1, UT_TOL_pt)) is78 = 1;      // exact; 1 solution
  else if(dd < d1) is78 = 0;                        // keine solution
  else if(dd > d1) is78 = 2;                        // 2 solutions
    // printf("is78=%d d1=%f\n",is78,d1);

  goto L_maxCirc_;
  





  //----------------------------------------------------------------
  // Die kreise liegen ineinander.
  L_over:

  // Liegt ein Kreis komplett im anderen ?
  if(r1 > r2) {
    if((lc + r2) <= r1) goto L_C_IN_C;
  } else {
    if((lc + r1) <= r2) goto L_C_IN_C;
  }


  // Errechnen der Innenabstaende;
  //   /c1    / \   c2\
  //  /      /   \     \
  //  | dx1  |dx2| dx3 |
  //  \      \   /     /
  dx1 = r1 + lc - r2;
  dx3 = lc + r2 - r1;
  dx2 = r1 + r2 + lc - dx1 - dx3;
    // printf(" dx1=%f dx2=%f dx3=%f\n",dx1,dx2,dx3);

  irc = 2; // die beiden Aussenkreise

  // den rechten Innenkreis (dx3) behandeln.
  // Gibts 1 oder 2 Loesungen -
  i3 = 0;
  if(UTP_comp2db(dx3, dd, UT_TOL_pt)) {
    // es gibt 1 Loesung
    i3 = 1;
  } else if(dx3 > dd) {
    // es gibt 2 Loesungen
    i3 = 2;
  }
  irc += i3; // Anz punkte rechts; 0-2
    // printf(" i3=%d\n",i3);
 

  // den linken Innenkreis (dx1) behandeln.
 // Gibts 0 od 1 od 2 Loesungen -
  i1 = 0;
  if(UTP_comp2db(dx1, dd, UT_TOL_pt)) {
    // es gibt 1 Loesung
    i1 = 1;
  } else if(dx1 > dd) {
    // es gibt 2 Loesungen
    i1 = 2;
  }
  irc += i1;
    // printf(" i1=%d\n",i1);
 

  // den mittleren Innenkreis (dx2) behandeln.
  // Gibts 0 od 1 od 2 Loesungen -
  i2 = 0;
  if(UTP_comp2db(dx2, dd, UT_TOL_pt)) {
    // es gibt 1 Loesung
    i2 = 1;
  } else if(dx2 > dd) {
    // es gibt 2 Loesungen
    i2 = 2;
  }
  irc += i2; // Anz punkte mitte; 0-2
    // printf(" irc=%d i1=%d i2=%d i3=%d\n",irc,i1,i2,i3);

  if(sNr < 2) goto L_2sol;
  sNr -= 2;
  if(sNr < i3) goto L_rechts;
  sNr -= i3;
  if(sNr < i1) goto L_links;
  sNr -= i1;
  goto L_mitte;




  //================================================================
  L_1sol:
    // printf("L_1sol \n");
    // c1 touches c2 inside; only 1 sol. possible.
    irc = 1;
    la = ((lc -r1 -r2) / 2.) + r1;
    lb = rd;
    goto L_cre_1;


  //================================================================
  L_2sol:
    // printf("L_2sol %d\n",sNr);
    la = r2 + rd;
    lb = r1 + rd;
    goto L_create;


  //================================================================
  L_rechts:
    // sNr=welcher punkt erzeugt werden soll; i3=wieviele Punkt moeglich waeren.
    // printf("L_rechts %d %d\n",sNr,i3);
    if(i3 == 1) {  // 1 Kreis mittig
      UT3D_pt_traptvclen (&ca[0].p1, &ci2->pc, &vx, r2);
      UT3D_pt_traptvclen (&ca[0].pc, &ca[0].p1, &vx, -rd);
      ca[0].p2 = ca[0].p1;
      goto L_done;
    }
    // 2 Kreise;
    la = r2 - rd;
    lb = r1 + rd;
    // if(sNr == 1) ySid = -1;
    goto L_create;


  //================================================================
  L_links:
    // sNr=welcher punkt erzeugt werden soll; i1=wieviele Punkt moeglich waeren.
    // printf("L_links %d %d\n",sNr,i1);
    if(i3 == 1) {  // 1 Kreis mittig
      UT3D_pt_traptvclen (&ca[0].p1, &ci1->pc, &vx, -r1);
      UT3D_pt_traptvclen (&ca[0].pc, &ca[0].p1, &vx, rd);
      ca[0].p2 = ca[0].p1;
      goto L_done;
    }
    // 2 Kreise;
    la = r2 + rd;
    lb = r1 - rd;
    // if(sNr == 1) ySid = -1;
    goto L_create;


  //================================================================
  L_mitte:
    // sNr=welcher punkt erzeugt werden soll; i2=wieviele Punkt moeglich waeren.
    // printf("L_mitte %d %d\n",sNr,i2);
    if(i3 == 1) {  // 1 Kreis mittig
      UT3D_pt_traptvclen (&ca[0].p1, &ci1->pc, &vx, r1);
      UT3D_pt_traptvclen (&ca[0].pc, &ca[0].p1, &vx, -rd);
      ca[0].p2 = ca[0].p1;
      goto L_done;
    }
    // 2 Kreise;
    la = r2 - rd;
    lb = r1 - rd;
    // if(sNr == 1) ySid = -1;
    goto L_create;


  //================================================================
  L_create:
// set new pc from la, lb
//
//              x
//        lb  /   \ la      dy = h
//          /       \
//     pc1 x----|----x pc2
//             lc
//           dx=q
//
      // printf("L_create: la=%f lb=%f lc=%f ySid=%d\n",la,lb,lc,ySid);

    UT2D_solvtri_abc (&dx, &dy, la, lb, lc);
    // if(sNr == 1) dy = -dy;  // 2 = unten
    // if(ySid < 0) dy = -dy;  // 2 = unten
    if(ySid < 1) dy *= -1.;        // 1=above; 0=below
      // printf("    dx=%f dy=%f\n",dx,dy);

    // dx along vx
    UT3D_pt_traptvclen (&ptx, &ci1->pc, &vx, dx);
      // GR_tDyn_symB__ (&ptx, SYM_STAR_S, 3);

    // dy along vy
    UT3D_pt_traptvclen (&ca[0].pc, &ptx, &vy, dy);
      // GR_tDyn_symB__ (&ca[0].pc, SYM_STAR_S, 4);

    // p1 of new circ = from pc1 length r1 -> new pc
    UT3D_pt_traptptlen (&ca[0].p1, &ci1->pc, &ca[0].pc, r1);
    ca[0].p2 = ca[0].p1;
    goto L_done;



  //================================================================
  L_cre_1:
    //              vx
    //      |------------->------------>
    //   ci1->pc  la    ca.pc   lb    ca.p1
    UT3D_pt_traptvclen (&ca[0].pc, &ci1->pc, &vx, la);
    UT3D_pt_traptvclen (&ca[0].p1, &ca[0].pc, &vx, lb);
    ca[0].p2 = ca[0].p1;
    goto L_done;





  //================================================================
  //================================================================
  L_C_AT_C1:
    // printf("L_C_AT_C1 %d\n",sNr);
  // ein Kreis liegt komplett im anderen drin und beruehrt.
  // 0 u 1 sind am Beruehrungspunkt;
  irc = 2;
  if(sNr > 1) {sNr -= 2; goto L_C_IN_C;}
    dx = rd;
    if(sNr > 0) dx = -rd;
    UT3D_pt_traptvclen (&ca[0].p1, &ci1->pc, &vx, r1);
    UT3D_pt_traptvclen (&ca[0].pc, &ca[0].p1, &vx, dx);
    ca[0].p2 = ca[0].p1;
    goto L_done;


  //================================================================
  L_C_AT_C2:
    // printf("L_C_AT_C2 %d\n",sNr);
  // ein Kreis liegt komplett im anderen drin und beruehrt.
  // 0 u 1 sind am Beruehrungspunkt;
  irc = 2;
  if(sNr > 1) {sNr -= 2; goto L_C_IN_C;}
    dx = -rd;
    if(sNr > 0) dx = rd;
    UT3D_pt_traptvclen (&ca[0].p1, &ci2->pc, &vx, -r2);
    UT3D_pt_traptvclen (&ca[0].pc, &ca[0].p1, &vx, dx);
    ca[0].p2 = ca[0].p1;
    goto L_done;


  //================================================================
  //================================================================
  L_C_IN_C:
    // printf("L_C_IN_C:\n");
  // die Kreise liegen ineinander.
  // die Kreise kopieren; so, dass c1 der groesser Kreis ist
  if(r1 > r2) {
    c1 = *ci1;
    c2 = *ci2;
  } else {
    c1 = *ci2;
    c2 = *ci1;
    MEM_swap_2db (&r1, &r2);
    UT3D_vc_invert (&vx, &vx);
    UT3D_vc_invert (&vy, &vy);
  }


  // ein Kreis liegt komplett im anderen drin.
  // RandAbstaende dx1 u dx2 errnechen;
  //   /c1    / \   c1\
  //  /      /c2 \     \
  //  | dx1  |   | dx3 |
  //  \      \   /     /
  dx1 = r1 + lc - r2;
  dx3 = r1 - lc - r2;
    // printf(" dx1=%f dx3=%f\n",dx1,dx3);


 // dx1: Gibts 0 od 1 od 2 Loesungen -
  i1 = 0;
  i3 = 0;
  if(UTP_comp2db(dx1, dd, UT_TOL_pt)) {i1 = 1; goto L_C_IN_1;}
  if(UTP_comp2db(dx3, dd, UT_TOL_pt)) {i3 = 1; goto L_C_IN_1;}
  if(dx3 > dd) goto L_C_IN_1;
  if(dx1 > dd) i1 = 2;


  L_C_IN_1:
  irc += i1 + i3;
    // printf(" i1=%d i3=%d sNr=%d\n",i1,i3,sNr);
 

  //================================================================
  // handle links
  if(i1 == 1) {
    // ein circ mittig ...
    if(sNr == 0) {
      UT3D_pt_traptvclen (&ca[0].p1, &c1.pc, &vx, -r1);
      UT3D_pt_traptvclen (&ca[0].pc, &ca[0].p1, &vx, rd);
      ca[0].p2 = ca[0].p1;
      goto L_done;
    }
  } else if(i1 == 2) {
    // 2 kreise ..
    if(sNr < 2) {
      la = r2 + rd;
      lb = r1 - rd;
      goto L_IN_create;
    }
  }
  sNr -= i1;


  //================================================================
  // handle rechts
  if(i3 == 1) {
    // ein circ mittig ...
    if(sNr == 0) {
      UT3D_pt_traptvclen (&ca[0].p1, &c2.pc, &vx, r2+dx3);
      UT3D_pt_traptvclen (&ca[0].pc, &ca[0].p1, &vx, -rd);
      ca[0].p2 = ca[0].p1;
      goto L_done;
    }
/*
  } else if(i3 == 2) {
    // 2 kreise ..
    if(sNr < 2) {
      la = r2 + rd;
      lb = r1 - rd;
      goto L_IN_create;
    }
*/
  }
  // sNr -= i3;
  irc = -1;
  goto L_done;



  //================================================================
  L_touchOut:
    // printf("L_touchOut:\n");
    // c1 und c2 beruehren sich aussen. 4 solutions.
    //          1
    //    c1  3   4  c2
    //          2
    irc = 4;
    if(sNr == 2) {          // 3
      la = r1 - rd;
      lb = rd;
      goto L_cre_1;
    }
    if(sNr == 3) {          // 4
      la = r1 + rd;
      lb = rd;
      goto L_cre_1;
    }
    goto L_2sol;





  //================================================================
  L_maxCirc_:
      // printf("L_maxCirc_: is34=%d is56=%d is78=%d\n",is34,is56,is78);

    // irc = max nr of solutions
    irc = 2 + is34 + is56 + is78;
    if(sNr < 3) goto L_2sol;          // 1,2


    // handle is34   (touch c1, around c2)
    if(sNr < 5) {
      if(is34 == 1) {
        //    c1 X    c2
        la = -r1 + rd;
        lb = rd;
        goto L_cre_1;
  
      } else if(is34 > 1) {
        la = rd - r2;
        lb = rd + r1;
        goto L_create;
      }
    }


    // handle is56
    if(sNr < 7) {
      if(is56 == 1) {
        //    c1    X c2
        la = lc + r2 - rd;
        lb = rd;
        goto L_cre_1;
  
      } else if(is56 > 1) {
        la = rd + r2;
        lb = rd - r1;
        r1 = -r1;
        goto L_create;
      }
    }



    // handle is78
    if(is78 == 1) {
      //    c1 X c2
      la = ((lc + r1 + r2) / 2.) - r1;
      lb = rd;
      goto L_cre_1;

    } else if(is78 > 1) {
      la = r2 - rd;
      lb = r1 - rd;
      r1 = -r1;
      // ySid = ICHG01(ySid);
      goto L_create;
    }

    return -1;



  //================================================================
  L_IN_create:
      // printf(" la=%f lb=%f lc=%f\n",la,lb,lc);
    UT2D_solvtri_abc (&dx, &dy, la, lb, lc);
      // printf("    dx=%f dy=%f\n",dx,dy);
    if(sNr == 1) dy = -dy;  // 2 = unten
    UT3D_pt_traptvclen (&ptx, &c1.pc, &vx, dx);
      // GR_tDyn_symB__ (&ptx, SYM_STAR_S, 3);
    UT3D_pt_traptvclen (&ca[0].pc, &ptx, &vy, dy);
      // GR_tDyn_symB__ (&ca[0].pc, SYM_STAR_S, 4);
    UT3D_pt_traptptlen (&ca[0].p1, &c1.pc, &ca[0].pc, r1);
    ca[0].p2 = ca[0].p1;






  //================================================================
  //================================================================
  L_done:
    // DEB_dump_obj__(Typ_CI, ca, "ex UT3D_ci_cicird irc=%d\n",irc);

  return irc;

}


//=====================================================================
  int UT3D_ci_ptptrd (Circ ca[], Point *pt1, Point *pt2, double rad1) {
//=====================================================================
/// \code
/// UT3D_ci_ptptrd            Circ = tangent to PT + PT, radius.
/// DO NOT USE; use UT3D_ci_2ptvcrd
/// \endcode
// 
// RC = 1,2 = Anzahl Kreise.
// es fehlt der Z-vektor im Interface !!!


  double   rd, la, lb;
	Point    ptm;
	Vector   vcl, vcn;


  rd = fabs(rad1);


	UT3D_pt_mid2pt (&ptm, pt1, pt2);
	lb = UT3D_len_2pt (&ptm, pt1);
  // printf("UT3D_ci_ptptrd rd=%f lb=%f\n",rd,lb);

	if(lb < UT_TOL_pt) return -1;


	ca[0].rad = rad1;
	ca[0].vz  = UT3D_VECTOR_Z;
	ca[0].p1  = *pt1;
	ca[0].p2  = *pt1;


  if(UTP_comp2db(lb, rd, UT_TOL_pt)) {   // beruehrt
		ca[0].pc = ptm;
    return 1;
	}

  if(lb > rd) return -1;

  ca[1] = ca[0];

	la = sqrt(rd*rd - lb*lb);

	UT3D_vc_2pt (&vcl, pt1, pt2);
	UT3D_vc_perp2vc (&vcn, &vcl, (Vector*)&UT3D_VECTOR_Z);
  UT3D_pt_traptvclen (&ca[0].pc, &ptm, &vcn, la);
  UT3D_pt_traptvclen (&ca[1].pc, &ptm, &vcn, -la);
  return 2;


}


//==============================================================================
  int UT3D_ci_lnptrd (Circ *cia, Line *ln1, Point *pt1, Vector *vc, double rdc) {
//==============================================================================
/// \code
/// UT3D_ci_lnptrd            Circ = tangent to LN + PT, radius.
/// 
/// Input:
///   pt1
///   ln1
///   vz       axis of Circ; only used if pt1 is on line ln1
///   rdc      radius
/// RetCod:
///  0-2       Nr of resulting Circles
/// \endcode



  double   rd, la, lb, ln;
  Point    ptn;
  Vector   vl, vz, vn;


  // printf("UT3D_ci_lnptrd %fd\n",rdc);
  // GR_tDyn_symB__ (pt1, SYM_STAR_S, 2);
  // GR_Disp_ln (ln1, 4);

  rd = fabs(rdc);


  // ptn = Normalpunkt des pt1 auf die Line
  // ln = Abstand pt1-ptn
  UT3D_vc_2pt (&vl, &ln1->p1, &ln1->p2);
  UT3D_pt_projptptvc (&ptn, &ln, NULL, pt1, &ln1->p1, &vl);
    // printf(" ln=%f ptn=%f,%f,%f\n",ln,ptn.x,ptn.y,ptn.z);


  // NormalVektor vz = crossprod ln X ln.p1-pt1
  if(ln < UT_TOL_pt) {
    vz = *vc;
  } else {
    UT3D_vc_perpvc2pt (&vz, &vl, &ln1->p1, pt1);
  }

  cia[0].ango = RAD_360;
  cia[0].rad  = rdc;
  cia[0].vz   = vz;
  cia[0].p1   = *pt1;
  cia[0].p2   = *pt1;

  cia[1] = cia[0];   // copy Circ


  // Punkt liegt genau auf Linie: 2 Loesungen
  if(ln < UT_TOL_pt) {
    UT3D_vc_perp2vc (&vn, &vz, &vl);
    UT3D_pt_traptvclen (&cia[0].pc, &ptn, &vn, rd);
    UT3D_pt_opp2pt (&cia[1].pc, &ptn, &cia[0].pc);
    return 2;
  }


  // Kreis genau zwischen Punkt und Linie
  if(fabs(ln - (rd * 2.)) < UT_TOL_pt) {
    UT3D_pt_mid2pt (&cia[0].pc, pt1, &ptn);
    return 1;
  }


  // kein Loesung ..
  if(ln > (rd * 2.)) return 0;



  la = rd - ln;
  lb = sqrt((rd*rd) - (la*la));

  UT3D_pt_traptptlen (&ptn, &ptn, pt1, rd);
  UT3D_pt_traptvclen (&cia[0].pc, &ptn, &vl, lb);
  UT3D_pt_opp2pt (&cia[1].pc, &ptn, &cia[0].pc);
    // UT3D_pt_traptvclen (&ca[0].pc, &ptn, &vl, -lb);

  return 2;

}


//=====================================================================
  int UT3D_ci_ciptrd (Circ ca[], Circ *ci1, Point *pt1, double rad1) {
//=====================================================================
/// \code
/// UT3D_ci_ciptrd            Circ = tangent to CI + PT, radius.
/// 
/// RetCod:
///  -1        Error
///  0-2       Nr of resulting Circles
/// \endcode


  int      i1;
  double   rc1, rd, dx, dy, la, lb, lc, d1, d2;
  Point    ptx;
  Vector   vx, vn;


  // printf("UT3D_ci_ciptrd %f\n",rad1);
  // DEB_dump_obj__(Typ_CI, ci1, "ci1");
  // DEB_dump_obj__(Typ_PT, pt1, "pt1");


  rd = fabs(rad1);
  rc1 = fabs(ci1->rad);

  // lc = disp centerCi1 - pt1
  lc = UT3D_len_2pt (&ci1->pc, pt1);
    // printf("lc=%f\n",lc);



  ca[0].ango = RAD_360;
  ca[0].rad  = rad1;
  ca[0].vz   = ci1->vz;
  ca[0].p1   = *pt1;
  ca[0].p2   = *pt1;
  i1 = 2;
  
  ca[1] = ca[0];   // copy Circ


  // dist = radius: tangent tru this point ...
  if(UTP_comp2db(lc, rc1, UT_TOL_pt) == 1) {   // beruehrt am ci1
    // printf(" touch1..\n");
    UT3D_pt_traptptlen (&ca[1].pc, pt1, &ci1->pc, rd);
    UT3D_pt_opp2pt (&ca[0].pc, pt1, &ca[1].pc);
    goto L_exit;
  }


  // outside or inside
  if(lc < rc1) goto L_inside;


  // maxDist = (ci1.rad + rdc) > lc: Error.
  d1 = rc1 + (rd * 2.);
  if(UTP_comp2db(lc, d1, UT_TOL_pt) == 1) {   // beruehrt ganz aussen
    // printf(" touch2..\n");
    UT3D_pt_traptptlen (&ca[0].pc, pt1, &ci1->pc, rd);
    i1 = 1;
    goto L_exit;
  }


  // error - no solution
  if(lc > d1) return -1;

  la = rc1 + rd;

  goto L_allg;



  //================================================================
  L_inside:


  // check if dist-pt1-circ == 2*rd
  d1 = rd * 2.;
  d2 = lc + d1;
  if(UTP_comp2db(rc1, d2, UT_TOL_pt) == 1) {   // beruehrt - kleiner Kreis
    // printf(" touchi1..\n");
    if(lc < UT_TOL_pt) {
      UT3D_pt_mid2pt (&ca[0].pc, &ci1->pc, &ci1->p1);
    } else {
      UT3D_pt_traptptlen (&ca[0].pc, pt1, &ci1->pc, -rd);
    }
    i1 = 1;
    goto L_exit;
  }

  // error - no solution
    // printf(" rc1=%f d2=%f\n",rc1,d2);
  if(rc1 > d2) return -1;


  // andere seite -
  // check if dist-pt1-circ == 2*rd
  d2 = lc + rc1;
  if(UTP_comp2db(d2, d1, UT_TOL_pt) == 1) {   // beruehrt - grosser Kreis
    // printf(" touchi2..\n");
    UT3D_pt_traptptlen (&ca[0].pc, pt1, &ci1->pc, rd);
    i1 = 1;
    goto L_exit;
  }

  // error - no solution
    // printf(" d2=%f d1=%f\n",d2,d1);
  if(d1 > d2) return -1;


  //-----------------------
  la = rc1 - rd;


  L_allg:

  lb = rd;
  UT2D_solvtri_abc (&dx, &dy, la, lb, lc);


  // Normalvec
  UT3D_vc_2pt (&vx, pt1, &ci1->pc);
  UT3D_vc_perp2vc (&vn, &vx, &ci1->vz);

  UT3D_pt_traptvclen (&ptx, pt1, &vx, dx);
  UT3D_pt_traptvclen (&ca[0].pc, &ptx, &vn, dy);
  UT3D_pt_opp2pt (&ca[1].pc, &ptx, &ca[0].pc);



  //================================================================
  L_exit:
    // if(i1>0) DEB_dump_obj__(Typ_CI, &ca[0], "ex UT3D_ci_ciptrd-1\n");
    // if(i1>1) DEB_dump_obj__(Typ_CI, &ca[1], "ex UT3D_ci_ciptrd-2\n");

    return i1;

}



//=====================================================================
  int UT3D_ci_lncird (Circ ca[],
                      Line *ln1, Circ *ci1, double rad1, int sNr) {
//=====================================================================
/// \code
/// UT3D_ci_lncird            Circ = tangent to LN + CIR, radius.
/// 
/// Input:
///  sNr  solutionNr, -1 == all Solutions, else requested solution
/// 
/// RC = 1,2,3,4,5,6,7,8 = Max. Anzahl Kreise.
/// \endcode


  int      irc, iSid;
  double   d1, lcl, la, lb, lc, rc1, rd, dr1, dr2, dd;
  Point    pcl, ptx;
  Vector   vl, vn;



	// printf("UT3D_ci_lncird rd=%f %d\n",rad1,sNr);
  // printf("  pc = %f,%f,%f r=%f\n",ci1->pc.x,ci1->pc.y,ci1->pc.z,ci1->rad);
  // printf("  ln1.p1=%f %f %f\n",ln1->p1.x,ln1->p1.y,ln1->p1.z);
  // printf("  ln1.p2=%f %f %f\n",ln1->p2.x,ln1->p2.y,ln1->p2.z);



  rd   = fabs(rad1);                      // radius des gesuchten kreises
  dd = rd * 2.;

  ca[0].ango = RAD_360;
  ca[0].rad  = rd;
  ca[0].vz   = ci1->vz;


  UT3D_vc_2pt (&vl, &ln1->p1, &ln1->p2);  // vl = Richtung ln1
  rc1 = fabs(ci1->rad);                   // radius Kreis ci1


  // pcl = Normalpunkt des PC auf die Line
  // lcl = Abstand pcl - ci1->pc
  UT3D_pt_projptptvc (&pcl, &lcl, NULL, &ci1->pc, &ln1->p1, &vl);
    // printf("  abst=%f pcl=%f,%f,%f\n",lcl,pcl.x,pcl.y,pcl.z);
    // GR_tDyn_symB__ (&pcl, SYM_STAR_S, 2);



  // ganz aussen beruehrend ?
  d1 = rc1 + dd;
  if(UTP_comp2db(d1, lcl, UT_TOL_pt)) goto L_1sol;


  // gar keine Loesung ?
  if(lcl > d1) return -1;


  // den Vektor vn errechen; ist normal auf vl=ln1
  UT3D_vc_perp2vc (&vn, &vl, &ci1->vz);
  // UT3D_vc_perp2vc (&vn, &ci1->vz, &vl);


  // liegt pc1Center ueber oder unter ln1 -
  iSid = UT3D_sid_ptptvc (&ci1->pc, &pcl, &vn);
    // printf("iSid=%d\n",iSid);
  // if(iSid < 0) UT3D_vc_invert (&vn, &vn);


  // does line touch circ ?
  if(UTP_comp2db(rc1, lcl, UT_TOL_pt)) goto L_3sol;


	// geht Linie genau durch den Kreis-Mittelpunkt ?
  if(lcl < UT_TOL_pt) {
    // wenn (2 * rd == rc1): Sonderfall 2 Innenkreise
    if(UTP_comp2db(rc1, dd, UT_TOL_pt)) {irc = 6; goto L_6sol1;}
  }


  // zeigt vn nun von pcl in Richtung ci1.pc ? Wenn ja: umdrehen !
  // vn zeigt nun von pcl -> ci1.pc
  // if(UT3D_acos_vc2pt(&vn,&pcl,&ci1->pc) < 0.) UT3D_vc_invert (&vn,&vn);
  // if(iSid > 0) UT3D_vc_invert (&vn,&vn);


  // is line inside or outside circ ?
  if(rc1 < lcl) {irc = 2; goto L_2sol;}


  // line ist inside circ
  // die Abstande vom kreis mal feststellen.
  dr1 = rc1 - lcl;   // der kuerzere Abst
  dr2 = rc1 + lcl;   // der laengere Abst
    // printf(" dr1=%f dr2=%f\n",dr1,dr2);


  if(UTP_comp2db(dr1, dd, UT_TOL_pt)) {irc = 7; goto L_7sol;}
  if(UTP_comp2db(dr2, dd, UT_TOL_pt)) {irc = 5; goto L_5sol;}

  if(dd < dr1) {irc = 8; goto L_8sol;}
  if(dd < dr2) {irc = 6; goto L_8sol;}

  irc = 4;
  goto L_4sol;



  //================================================================
  L_1sol:     // ganz aussen beruehrend ...
  // printf("L_1sol \n");
    // UT3D_pt_traptptlen (&ca[0].pc, &pcl, &ci1->pc, rd * 2.);
    UT3D_pt_traptptlen (&ca[0].pc, &ci1->pc, &pcl, rc1);
    ca[0].p1 = pcl;
    ca[0].p2 = pcl;
    ca[0].vz = ci1->vz;
    irc = 1;
    goto L_done;
    return 1;



  //================================================================
  L_2sol:                     // sol-1 & 2
  // printf("L_2sol \n");
  // line ist outside circ.
  //
  //
  //
  //       ci1.pc
  //         x
  //         | \
  //  la =   |    \  lc = rd + rc1
  //  lcl-rd |        \
  //         |          \
  //         |-------------x   ca.pc
  //     ptx      lb
  //         |
  //     pcl x-------------x ln1

  // Dreick mit c = rd + rc1; a = lcl-rd;
  lc = rc1 + rd;
  la = lcl - rd;
  lb = sqrt((lc * lc) - (la * la));
    // printf("  la=%f lb=%f lc=%f\n",la,lb,lc);

  if(sNr == 1) lb = -lb;  // 2 =0 links

  goto L_create;



  //================================================================
  L_3sol:
  // line touches circ.
  // printf("L_3sol \n");
  //    2    4    1
  // ---------------
  //         3

  irc = 4;
  if(sNr < 2) goto L_2sol;

  if(sNr == 2) d1 = -rd;
  else d1 = rd;

  ca[0].p1 = pcl;
  ca[0].p2 = pcl;
  UT3D_pt_traptptlen (&ca[0].pc, &pcl, &ci1->pc, d1);
  goto L_done;



  //================================================================
  L_4sol:
  // printf("L_4sol %d\n",sNr);
  //    2        1
  // ---------------
  //  4     x     3

  lc = rc1 + rd;

  if(sNr < 2) la = lcl + rd;
  else        la = lcl - rd;

  lb = sqrt((lc * lc) - (la * la));

  if((sNr == 1)||(sNr == 3)) lb = -lb;
  // printf("  la=%f lb=%f lc=%f\n",la,lb,lc);

  goto L_create;


  //================================================================
  L_5sol:
  // innen gibts nur einen circ; genau an der groessten stelle.
  // printf("L_5sol \n");
  if(sNr != 4) goto L_4sol;

  // beruehrungspunkt am ci1 aussen:
  UT3D_pt_traptvclen (&ca[0].p1, &ci1->pc, &vn, rc1);
    // GR_tDyn_symB__ (&ca[0].p1, SYM_STAR_S, 2);
  ca[0].p2 = ca[0].p1;
  // pc=mittelpunkt pcl-p1
  UT3D_pt_mid2pt (&ca[0].pc, &pcl, &ca[0].p1);
    // GR_tDyn_symB__ (&ca[0].p1, SYM_STAR_S, 3);

  goto L_done;


  //================================================================
  L_6sol1:
  // ln1 geht genau durch ci1.pc
  //  2  6  1
  // ----------
  //  4  5  3
  // printf("L_6sol1 %d\n",sNr);
  if(sNr < 4) goto L_4sol;

  if(sNr == 4) la = -rc1;   // 5
  else         la = rc1;    // 6

  UT3D_pt_traptvclen (&ca[0].p1, &ci1->pc, &vn, la);
  UT3D_pt_mid2pt (&ca[0].pc, &ci1->pc, &ca[0].p1);
  ca[0].p2 = ca[0].p1;

  goto L_done;


  //================================================================
  L_7sol:
  // printf("L_7sol %d\n",sNr);
  //    2    7     1
  // ---------------
  //  4  6   x   5  3
  if(sNr < 6) goto L_8sol;

  if(iSid > 0) rc1 = -rc1;

  UT3D_pt_traptvclen (&ca[0].p1, &ci1->pc, &vn, rc1);
  UT3D_pt_mid2pt (&ca[0].pc, &pcl, &ca[0].p1);
  ca[0].p2 = ca[0].p1;

  goto L_done;


  //================================================================
  L_8sol:
  // printf("L_8sol \n");
  //    2  8    7  1
  // ---------------
  //  4  6   x   5  3
  if(sNr < 4) goto L_4sol;

  lc = rc1 - rd;

  if(sNr < 6) la = lcl - rd; // sol 5 & 6
  else        la = lcl + rd; // sol 7 & 8

  lb = sqrt((lc * lc) - (la * la));
    // printf("  la=%f lb=%f lc=%f\n",la,lb,lc);

  if((sNr == 5)||(sNr == 7)) lb = -lb;  // 6 und 8 ist links

  goto L_create;


  //================================================================
  L_create:
  //       ci1.pc
  //         x
  //         | \
  //         |    \
  //      la |        \
  //         |          \
  //         |-------------x   ca.pc
  //     ptx      lb

  if(iSid > 0) la = -la;

  UT3D_pt_traptvclen (&ptx, &ci1->pc, &vn, la);
    // GR_tDyn_symB__ (&ptx, SYM_STAR_S, 3);

  UT3D_pt_traptvclen (&ca[0].pc, &ptx, &vl, lb);

  UT3D_pt_traptptlen (&ca[0].p1, &ci1->pc, &ca[0].pc, rc1);
    // GR_tDyn_symB__ (&ca[0].p1, SYM_STAR_S, 4);

  ca[0].p2 = ca[0].p1;
  goto L_done;






  //================================================================
  L_done:
    // DEB_dump_obj__(Typ_CI, ca, "ex UT3D_ci_lncird %d\n",irc);

  return irc;

}


//==========================================================================
  int UT3D_ci_2lnrd (Circ ca[], Line *ln1, Line *ln2, double rad1, int sNr) {
//==========================================================================
/// \code
/// UT3D_ci_2lnrd             Circ = corner for 2 Lines
/// 
/// Input:
///  sNr       solutionNr,  -1 = all Solutions, else requested solution
///              0|1|2|3|4
/// RC = -1: Lines parallel 
///      -2  lines do not intersect
///       4: OK, max nr of Circs, one circ back.
/// \endcode


  int       irc;
  double    aa, la, lc, r1, d1;
  Point     ps, px1, pm1, pm2, fp1, fp2, pmv;
  Vector    v1, v2, vm;


  // printf("============================================= \n");
  // printf("UT3D_ci_2lnrd rad1=%f sNr=%d\n",rad1,sNr);
  // DEB_dump_obj__(Typ_LN, ln1, " ln1");
  // DEB_dump_obj__(Typ_LN, ln2, " ln2");


  r1 = fabs(rad1);

  ca[0].ango = RAD_360;
  ca[0].rad = rad1;


  // ps = intersection-point ln1 -ln2
  // irc = UT3D_pt_int2ln (&ps, &px1, &d1, ln1, ln2);
  irc = UT3D_pt_intlnln (&ps, &px1, &d1, ln1, 1, ln2, 1);
  if(irc < 0) return -1;
  if(d1 > UT_TOL_cv) return -2; //UT3D_pt_mid2pt (&ps, &ps, &px1);
    // DEB_dump_obj__ (Typ_PT, &ps, "ps");

//----------------------------------------------------------------
// NEW:
  // get fp1 = farer point from ps on ln1
  irc = UT3D_minLen_3pt (&aa, &ps, &ln1->p1, &ln1->p2);
  if(irc == 1) fp1 = ln1->p2;
  else         fp1 = ln1->p1;


  // get fp2 = farer point from ps on ln2
  irc = UT3D_minLen_3pt (&aa, &ps, &ln2->p1, &ln2->p2);
  if(irc == 1) fp2 = ln2->p2;
  else         fp2 = ln2->p1;


  UT3D_vc_2pt (&v1, &ps, &fp1);
  UT3D_vc_2pt (&v2, &ps, &fp2);

  UT3D_vc_setLength (&v1, &v1, 1.);
  UT3D_vc_setLength (&v2, &v2, 1.);

  UT3D_vc_perp2vc (&ca[0].vz, &v1, &v2);
  UT3D_vc_setLength (&ca[0].vz, &ca[0].vz, 1.);     // 2016-09-05
    // DEB_dump_obj__ (Typ_VC, &v1, "v1");
    // DEB_dump_obj__ (Typ_VC, &v2, "v2");
    // DEB_dump_obj__ (Typ_VC, &ca[0].vz, "vz");

  // vm  = midvector ps - ln1 - ln2
  UT3D_vc_2vc (&vm, &v1, &v2);
  UT3D_vc_setLength (&vm, &vm, 1.);
    // DEB_dump_obj__ (Typ_VC, &vm, "vm");


  //-------------------------------------
  //      v2
  //       \                /vm
  //        \              /
  //         \     r   _--x pc
  //          \    _--'  /|
  //           x--'     / |
  //            \    lc/  |
  //             \    /   |r=lb
  //              \  /    |
  //               \/a la |
  //      ---------x------x----- v1
  //              ps\     p1
  //                 \

  if(sNr > 1) goto L_sol34;

  // half opening-angle aa
  aa = ACOS(UT3D_acos_2vc (&v1, &vm));

  // Dreieck la=v1, lb=r, lc auf Mittellinie
  la = r1 / tan(aa);
  lc = r1 / sin(aa);
    // printf(" aa=%f la=%f lc=%f\n",aa,la,lc);


  //-------------------------------------
  if(sNr == 0) {
    UT3D_pt_traptvclen (&ca[0].pc, &ps, &vm, lc);
    UT3D_pt_traptvclen (&ca[0].p1, &ps, &v1, la);
    // UT3D_pt_traptvclen (&ca[0].p2, &ps, &v2, la);
    ca[0].p2  = ca[0].p1;
    goto L_done;


  //-------------------------------------
  } else if(sNr == 1) {
    UT3D_pt_traptvclen (&ca[0].pc, &ps, &vm, -lc);
    UT3D_pt_traptvclen (&ca[0].p1, &ps, &v1, -la);
    // UT3D_pt_traptvclen (&ca[0].p2, &ps, &v2, la);
    ca[0].p2  = ca[0].p1;
    goto L_done;
  }



  //-------------------------------------
  L_sol34:
  // Mittelvektor vm;
  UT3D_vc_perp2vc (&vm, &ca[0].vz, &vm);
  UT3D_vc_setLength (&vm, &vm, 1.);
  // printf(" vm=%f,%f,%f\n",vm.dx,vm.dy,vm.dz);


  // Oeffnungswinkel aa
  aa = acos(UT3D_acos_2vc (&v1, &vm));

  // Dreieck la, lb=r, lc auf Mittellinie
  la = r1 / tan(aa);
  lc = r1 / sin(aa);
  // printf(" aa=%f la=%f lc=%f\n",aa,la,lc);


  if(sNr == 2) {
    UT3D_pt_traptvclen (&ca[0].pc, &ps, &vm, lc);
    UT3D_pt_traptvclen (&ca[0].p1, &ps, &v1, la);
    // UT3D_pt_traptvclen (&ca[0].p2, &ps, &v2, la);
    ca[0].p2  = ca[0].p1;
    goto L_done;


  } else if(sNr == 3) {
    UT3D_pt_traptvclen (&ca[0].pc, &ps, &vm, -lc);
    UT3D_pt_traptvclen (&ca[0].p1, &ps, &v1, -la);
    // UT3D_pt_traptvclen (&ca[0].p2, &ps, &v2, la);
    ca[0].p2  = ca[0].p1;
  }



  //-------------------------------
  L_done:

    // DEB_dump_obj__(Typ_CI, ca, "ex UT3D_ci_2lnrd s-%d\n",sNr);

  return 4;

}


//================================================================
  int UT3D_ci_intplsph (Circ *cio, Plane *pli, Sphere *spi) {
//================================================================
/// \code
/// Circ = intersect Plane Sphere
/// 
/// irc  0   OK
/// irc -1   Plane touches Sphere
/// irc -2   Plane outside Sphere
/// \endcode

  double  d1, d2, d3;
  Point   pt1;


  // DEB_dump_obj__ (Typ_PLN, pli, "UT3D_ci_intplsph:\n");
  // DEB_dump_obj__ (Typ_SPH, spi, "");


  // d1 = Abstand von sph-cen von der plane
  d1 = UT3D_slen_2ptvc (&pli->po, &spi->pc, &pli->vz);
  d2 = fabs(d1);
  d3 = fabs(spi->rad - d2);
    // printf(" d1=%f d2=%f d3=%f\n",d1,d2,d3);

  if(d3 < -UT_TOL_pt)  return -2;
  if(d3 <  UT_TOL_pt)  return -1;

  UT3D_pt_projptpl (&pt1, pli, &spi->pc);


  cio->vz   = pli->vz;
  UT2D_solvtriri_a (&d3, d2, spi->rad); // Radius Circ

  // make 360-Grad Circ from pc, vz, vx and radius
  UT3D_ci_pt2vcr (cio, &pt1, &pli->vz, &pli->vx, d3);

  // UT3D_pt_traptvclen (&cio->p1, &cio->pc, &pli->vx, cio->rad);
  // cio->p2 = cio->p1;
  // cio->ango = RAD_360;


  // DEB_dump_obj__ (Typ_CI, cio, "ex UT3D_ci_intplsph:\n");

  return 0;

}


//================================================================
  int UT3D_ci_tracivc (Circ *cio, Circ *cii, Vector *vc1) {
//================================================================
// move circle

  *cio = *cii;   // copy vz, rad, ango

  UT3D_pt_traptvc(&cio->pc, &cii->pc, vc1);
  UT3D_pt_traptvc(&cio->p1, &cii->p1, vc1);
  UT3D_pt_traptvc(&cio->p2, &cii->p2, vc1);

    // DEB_dump_obj__ (Typ_CI, cio, "ex UT3D_ci_tracivc:");

  return 0;

}



/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/



//===============================================================
  void UT3D_obj_vctra (ObjG *o2, ObjG *o1, Vector *vc1) {
//===============================================================
/// UT3D_obj_vctra            translate Obj



  *o2 = *o1; // copy typ, att, rad, ID ...


/*
  if(o2->typ == Typ_CVPOL) {
    UTCV_cv_vctra (o2->ID, vc1);
    return;
  }
*/




  o2->p1.x = o1->p1.x + vc1->dx;
  o2->p1.y = o1->p1.y + vc1->dy;
  o2->p1.z = o1->p1.z + vc1->dz;

  if(o2->typ == Typ_PT) return;


  o2->p2.x = o1->p2.x + vc1->dx;
  o2->p2.y = o1->p2.y + vc1->dy;
  o2->p2.z = o1->p2.z + vc1->dz;

  if(o2->typ == Typ_LN) return;


  o2->pc.x = o1->pc.x + vc1->dx;
  o2->pc.y = o1->pc.y + vc1->dy;
  o2->pc.z = o1->pc.z + vc1->dz;

  return;

}


/*   inline ..
//===============================================================
  void UT3D_swap2pt (Point *p1, Point *p2) {
//===============================================================
// swap 2 2D-points

  Point pth;

  pth = *p1;
  *p1 = *p2;
  *p2 = pth;

}
*/


/*
//======================================================================
  ObjG UT3D_obj_obj2 (ObjG2 *o2) {
//======================================================================
//  UT3D_obj_obj2            change 2D-Obj > 3D-Obj 


  ObjG o3;


  o3.typ  = o2->typ;
  o3.p1   = UT3D_pt_pt2(&o2->p1);
  o3.p2   = UT3D_pt_pt2(&o2->p2);
  o3.pc   = UT3D_pt_pt2(&o2->pc);
  o3.rad  = o2->rad;

  // if(o2->rad > 0.) {
    o3.vz   = UT3D_VECTOR_Z;
  // } else {
    // o3.vz   = UT3D_VECTOR_IZ;
  // }

  return o3;

}

*/


//================================================================
  int UT3D_obj_obj2 (ObjGX *oo, ObjGX *oi, Memspc *memSeg) {
//================================================================
/// \code
/// UT3D_obj_obj2             change 2D-Obj > 3D-Obj (add Z=0; vz=0,0,1)
/// 2D-Objekte in 3D-Objekte umwandeln (add Z=0)
/// \endcode

  void    *vp;

  // DEB_dump_obj__ (Typ_ObjGX, oi, "UT3D_obj_obj2:");
  // DEB_dump_ox_0 (oi, "UT3D_obj_obj2:");

  if(oi->form == Typ_PT2) {
    vp = UME_reserve (memSeg, sizeof(Point));
    *((Point*)vp) = UT3D_pt_pt2 ((Point2*)oi->data);
    OGX_SET_OBJ (oo, Typ_PT, Typ_PT, 1, vp);

  } else if(oi->form == Typ_LN2) {
    vp = UME_reserve (memSeg, sizeof(Line));
    UT3D_ln_ln2 ((Line*)vp, ((Line2*)oi->data));
    OGX_SET_OBJ (oo, Typ_LN, Typ_LN, 1, vp);

  } else if(oi->form == Typ_CI2) {
    vp = UME_reserve (memSeg, sizeof(Circ));
    UT3D_ci_ci2 ((Circ*)vp, (Circ2*)oi->data);
    OGX_SET_OBJ (oo, Typ_CI, Typ_CI, 1, vp);

  } else {
    TX_Print("UT3D_obj_obj2 E001 %d",oi->form);
    return -1;
  }

  // DEB_dump_obj__ (Typ_ObjGX, oo, "ex UT3D_obj_obj2:");
  // DEB_dump_ox_0 (oo, "ex UT3D_obj_obj2:");


  return 0;

}


/*
//=====================================================================
  ObjG UT3D_obj_pt (Point *pt1) {
//=====================================================================
//  UT3D_obj_pt              change Point > 3D-Obj

  ObjG o1;
  o1.typ  = Typ_PT;
  o1.p1   = *pt1;
  return o1;

}
*/

//=====================================================================
  ObjG UT3D_obj_ln (Line *ln1) {
//=====================================================================
// UT3D_obj_ln               change Line > 3D-Obj
/// DO NOT USE

  ObjG o1;


  o1.typ  = Typ_LN;

  o1.p1   = ln1->p1;
  o1.p2   = ln1->p2;


  return o1;

}

/*
//=====================================================================
  ObjG UT3D_obj_ci2 (Circ2 *ci1, double zVal) {
//=====================================================================
// UT3D_obj_ci2              change 2D-Circ > 3D-Obj

  ObjG o1;


  o1.typ  = Typ_CI;

  o1.p1.x = ci1->p1.x;
  o1.p1.y = ci1->p1.y;
  o1.p1.z = zVal;

  o1.p2.x = ci1->p2.x;
  o1.p2.y = ci1->p2.y;
  o1.p2.z = zVal;

  o1.pc.x = ci1->pc.x;
  o1.pc.y = ci1->pc.y;
  o1.pc.z = zVal;

  o1.rad  = ci1-> rad;

  return o1;

}
*/


//=====================================================================
  ObjG UT3D_obj_ci (Circ *ci1) {
//=====================================================================
// UT3D_obj_ci               change Circ > 3D-Obj
/// DO NOT USE


  ObjG o1;


  o1.typ  = Typ_CI;

  o1.p1 = ci1->p1;
  o1.p2 = ci1->p2;
  o1.pc = ci1->pc;

  o1.vz = ci1->vz;

  o1.rad  = ci1-> rad;

  return o1;

}







/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/




//================================================================
  int UT3D_cv3_linear (int *pNr, Point *pTab, double tol) {
//================================================================
/// \code
/// UT3D_cv3_linear              delete unnecesary straight points
/// alle Punkte die innerhalb tol liegen aus pTab entfernen.
/// \endcode


  int     ii, i1, i3, ie;
  double  dl;
  char    ptStat[10000];   // max Anz Points pro Kontur
  // statt ptStat koennte man pt2a[x].x auf VAL_MAX setzen und dann danach suchen

  ie = *pNr - 1;  // index auf last point

  // printf("UT3D_cv3_linear %d\n",*pNr);

  if(ie > 10000) {TX_Error("UT2D_cv3_linear E001"); return -1;}

  ptStat[0]  = 0;  // keep point
  ptStat[ie] = 0;  // keep point

  i1 = 0;
  i3 = 2;


  //----------------------------------------------------------------
  L_test:
  // printf("----- L_test: i2=%d i3=%d\n",i1,i3);

  for(ii=i1+1; ii<i3; ++ii) {
    dl = UT3D_nlen_3pt (&pTab[i1], &pTab[ii], &pTab[i3]);
    // printf("   dl[%d]=%f\n",ii,dl);
    if(dl > tol) goto L_accept;
  }

  // alle Punkte zwischen i1 und i3 sind unnutz.
  ptStat[i3-1] = 1;   // delete point;
  i3 += 1;
  if(i3 <= ie) goto L_test;
  goto L_remove;



  //----------------------------------------------------------------
  L_accept:
  // der Punkt i3-1 muss bleiben.
  // Alle Punkte zwischen i1 und i3-1 entfernen.
  // i3-1 wird dann zu i1.
    // GR_tDyn_symB__ (&pTab[i3-1], SYM_STAR_S, 2);
  ptStat[i3-1] = 0;  // keep point
  i1 = i3-1;
  i3 += 1;
  if(i3 <= ie) goto L_test;



  //----------------------------------------------------------------
  L_remove:
  UT3D_cv_delia (pNr, pTab, ptStat);

  // printf("ex UT2D_cv3_linear %d\n",*pNr);

  return 0;

}


//================================================================
  int UT3D_cv_delia (int *pNr, Point *pTab, char *ptStat) {
//================================================================
/// \code
/// UT3D_cv_delia             delete points out of pTab
/// alle Punkte die (ptStat = 1) haben aus Tabelle loeschen
/// pNr richtigstellen
/// ptStat = 0: Punkt wird gebraucht
/// ptStat = 1: Punkt loeschen
/// \endcode


  int   iNr, ivon, inach;


  iNr   = *pNr;
  ivon  = 1;
  inach = 1;


  L_cpNxt:
  if(ivon >= iNr) goto L_fertig;
  if(ptStat[ivon] == 0)  {   // Punkt wird gebraucht
    // printf(" cp %d = %d\n",inach,ivon);
    if(inach != ivon) pTab[inach] = pTab[ivon];
    ++ivon;
    ++inach;
    goto L_cpNxt;

  } else {                 // Punkt nicht mehr used
    ++ivon;
    goto L_cpNxt;
  }




  L_fertig:
  *pNr = inach;

  // printf("ex UT3D_cv_delia %d\n",inach);
  // for(i1=0; i1<inach; ++i1) DEB_dump_obj__(Typ_PT2,&pt2a[i1],"P2[%d]=",i1);
  // for(iNr=0; iNr<inach; ++iNr) GR_tDyn_symB__ (&pTab[iNr], SYM_STAR_S, 2);

  return 0;

}


//================================================================
  int UT3D_sr_rev_obj (int sr1, int sr2) {
//================================================================
/// \code
/// UT3D_sr_rev_obj     get rotSense from rotSense and reverse-flag
/// Input:
///   sr1     direction; 0=fwd, 1=bwd
///   sr2     direction; 0=fwd, 1=bwd
/// Output:
///   retCod  combined sr
/// \endcode


  // 0,0 -> 0
  // 0,1 -> 1
  // 1,0 -> 1
  // 1,1 -> 0

  return I_XOR_2I (sr1, sr2);
  // if((sr1 + sr2) == 1) return 1;
  // return 0;

}


/* INLINE
//================================================================
  int UT3D_sr_ci (Circ *ac1) {
//================================================================
// return sense of direction; 0=CCW; 1=CW.
  if(ac1->rad >= 0.) return 0;
  // CW
  return 1;
}
*/

//================================================================
  int UT3D_sr_el (CurvElli *el1) {
//================================================================
// return sense of direction; 0=CCW; 1=CW.

  return el1->srot;

  // if(el1->dir >= 0) return 0;
  // return 1;

}


//================================================================
  int UT3D_sr_npt_bp (int pNr, Point *pa, int bp) {
//================================================================
/// \code
/// UT3D_sr_npt_bp            get senseOfRotation for n points on backplane bp
/// Compute sr=sense of rotation of a closed 3D-polygon.
/// 
/// Input:
///   pNr      nr points (without last = first point)
///   pa       points.
///   bp       BackPlane (get it from UT3D_bp_perp_vc)
/// Output:
///   RetCod   0 = CCW
///            1 = CW
/// 
/// \endcode

// see UT3D_vc_perppta

  int     i1, i2, i3, ie, sr;
  double  d1;


  // printf("\nUT3D_sr_npt_bp bp=%d ptNr=%d\n",bp,pNr);
  // for(i1=0;i1<pNr;++i1) DEB_dump_obj__(Typ_PT, &pa[i1], "p[%d]",i1);
  // GR_tDyn_npt__ (pNr, pa, SYM_STAR_S, 9);

  d1 = 0.;   // f. Flaeche ..
  ie   = pNr-1;

  for(i2=0; i2<pNr; ++i2) {

    if(i2 > 0) i1 = i2-1;
    else       i1 = ie;

    if(i2 < ie) i3 = i2+1;
    else        i3 = 0;


    // nun ist p1,p2,p3 ein Dreieck aus dem polygon pa von 3
    // aufeinanderfolgenden Punkten; p2 ist der mittlere.
    if(bp == 0)      d1 += pa[i2].y * (pa[i3].z - pa[i1].z); // skip X
    else if(bp == 1) d1 += pa[i2].z * (pa[i3].x - pa[i1].x); // skip Y
    else             d1 += pa[i2].x * (pa[i3].y - pa[i1].y); // skip Z



    // NUR TESTAUSG:
    // printf(" %d %d %d - %f\n",i1,i2,i3,d1);
    // if(mode == 0) printf("   %f,%f,%f\n",pa[i2].y,pa[i3].z, pa[i1].z);
    // if(mode == 1) printf("   %f,%f,%f\n",pa[i2].z,pa[i3].x, pa[i1].x);
    // if(mode == 2) printf("   %f,%f,%f\n",pa[i2].x,pa[i3].y, pa[i1].y);
  }

  if(d1 < 0.) {  // CW
    sr =  1;
    // *aro = d1 / -2.;
    // die Flaeche muesste um einen bestimmten Faktor vergroessert werden ..
  } else {       // CCW
    sr = 0;
    // *aro = d1 / 2.0;
  }

    // printf("ex UT3D_sr_npt_bp sr=%d\n",sr);

  return sr;

}


//=================================================================
  int UT3D_sr_polc (int ptNr, Point *pa, Vector *vcn, int mode)  {
//=================================================================
/// \code
/// UT3D_sr_polc                           nach Karl Sauer 2004-04-07
/// Compute sr=sense of rotation of a closed 3D-polygon.
/// 
/// Input:
///   ptNr     nr points (without last = first point)
///   pa       points.
///   vcn      normalvector (get it from UT3D_vc_perppta)
///   mode     BackPlane (get it from UT3D_bp_perp_vc)
/// Output:
///   RetCod   1 = CCW
///           -1 = CW
/// 
/// \endcode

  int    sr;


  // printf("UT3D_sr_polc ptNr=%d mode=%d\n",ptNr,mode);
  // GR_tDyn_npt__ (ptNr, pa, SYM_STAR_S, 2);

  // get senseOfRotation for pa on backplane mode
  sr = UT3D_sr_npt_bp (ptNr, pa, mode);
    // printf(" sr=%d\n",sr); // 0=CCW;1=CW
  if(!sr) sr = 1;   // 0 -> 1 (CCW)
  else    sr = -1;  // 1 -> -1; 2017-05-15 

  

  // nochmal invertieren wenn Ebene verkehrt ..
  if(mode == 0)        {   // skip X
    if(vcn->dx < 0.) sr = -sr;
  } else if(mode == 1) {   // skip Y
    if(vcn->dy < 0.) sr = -sr;
  } else               {   // skip Z
    if(vcn->dz < 0.) sr = -sr;
  }

  // if((mode == 0)&&(vcn->dx < 0.)) printf(" turn X\n");
  // else if((mode == 1)&&(vcn->dy < 0.)) printf(" turn Y\n");
  // else if(vcn->dz < 0.) printf(" turn Z\n");
  // printf("ex UT3D_sr_polc %d\n",sr);

  return sr;

}


//==========================================================================
  int UT3D_pta_ck_planar (int pNr, Point *pTab, Point *pPln, Vector *vc) {
//==========================================================================
/// \code
/// UT3D_pta_ck_planar        test if all points are in plane from pt-normalVec
/// testen ob alle Punkte planar (Abst. von Plane pPln-vc < UT_TOL_pt).
/// Input:
///   pPln       point on plane
///   vc         normalvector of plane
/// RetCod = 0   OK, ist planar
///         -1   no, 1-n points of pTab are not in plane pPln-vc
/// \endcode

  int    i1;
  double lvc, skp;
  Vector vc2;


  // DEB_dump_obj__(Typ_VC, vc, "UT3D_pta_ck_planar");
  // DEB_dump_obj__(Typ_PT, pPln, "pPln");


  // lvc = Laenge Normalvec d. plane
  lvc = sqrt(vc->dx*vc->dx + vc->dy*vc->dy + vc->dz*vc->dz);

  if(lvc < UT_TOL_min1) {
    printf("UT3D_slen_2ptvc E001\n");
    return 0.;
  }



  for(i1=0; i1<pNr; ++i1) {

    // vc2 = Vec. p1 --> p2
    vc2.dx = pTab[i1].x - pPln->x;
    vc2.dy = pTab[i1].y - pPln->y;
    vc2.dz = pTab[i1].z - pPln->z;

    skp = (vc->dx*vc2.dx + vc->dy*vc2.dy + vc->dz*vc2.dz) / lvc;
      // printf(" i1=%d skp=%lf\n",i1,skp);

    if(fabs(skp) > UT_TOL_pt) return -1;

  }

    // printf("ex UT3D_pta_ck_planar: lvc=%f skp=%f\n",lvc,skp);

  return 0;

}


//================================================================
  int UT3D_rMin_pta (double *rMin, int pNr, Point *pTab) {
//================================================================
/// \code
/// vom gesamten Polgon den kleinsten Radius errechnen
/// Nur Abschaetzung; es wird von jeweils 3 Punkten der Kreisradius errechnet.
/// ev in eine neue source ../ut/ut_pta.c ?
/// \endcode

  int    irc, i1;
  double d1;

  if(pNr < 3) {TX_Error("UT3D_rMin_pta E001"); return -1;}


  // GR_tDyn_npt__ (pNr, pTab, SYM_STAR_S, 9);


  *rMin = UT_VAL_MAX;
  for(i1=2; i1<pNr; ++i1) {
    // DEB_dump_obj__ (Typ_PT, &pTab[i1], "p[%d]",i1);
    // printf(" dist=%f\n",UT3D_len_2pt(&pTab[i1-2],&pTab[i1]));

    // radius zw 3 punkten
    irc = UT3D_rdc_3pt (&d1, &pTab[i1-2], &pTab[i1-1], &pTab[i1]);
    if(irc < 0) continue;
    if(d1 < *rMin) *rMin = d1;

  }


    // printf("ex UT3D_rMin_pta %f\n",*rMin);

  return 0;

}


//=====================================================================
  int UT3D_cksq_4pt (Point *pp1, Point *pp2, Point *pp3, Point *pp4) {
//=====================================================================
/// \code
/// UT3D_cksq_4pt          check 4 points if its a square
/// Achtung: die 4 Punkte muessen korrekt geordnet sein !
///
///    4    3
///
///    1    2
///
/// die 4 vektoren muessen gleich / bzw invers sein
/// RetCod  0   yes, its a square
///        -1   no
/// \endcode

  Vector  vc1, vc2;
  Point   p1, p2;

  printf("UT3D_cksq_4pt\n");

  UT3D_vc_2pt (&vc1, pp1, pp2);
  UT3D_vc_2pt (&vc2, pp4, pp3);

  if(UT3D_vc_ck_parl_vc (&vc1, &vc2, UT_TOL_pt) != 1) return -1;

  UT3D_vc_2pt (&vc1, pp1, pp4);
  UT3D_vc_2pt (&vc2, pp2, pp3);

  if(UT3D_vc_ck_parl_vc (&vc1, &vc2, UT_TOL_pt) != 1) return -1;

  // mittelpunkt1 1-3 und 2-4 muessen gleich sein !
  UT3D_pt_mid2pt (&p1, pp1, pp3);
  UT3D_pt_mid2pt (&p2, pp2, pp4);
  if(UT3D_comp2pt (&p1, &p2, UT_TOL_pt) != 1) return -1;


  printf("ex UT3D_cksq_4pt 0\n");

  return 0;

}



/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/


//================================================================
  int UT3D_bplen_2pt (double *lnv, Point *p1, Point *p2) {
//================================================================
/// \code
/// UT3D_bplen_2pt     returns longest vectorpart and its backPlane
/// RC = 0 = BCKPLN_YZ = X-part-len
///      1 = BCKPLN_XZ = Y-part-len
///      2 = BCKPLN_XY = Z-part-len
/// \endcode


  int      bp;
  double   d1, d2, d3;
  
  // DEB_dump_obj__(Typ_VC, vc1, "UT3D_bp_perp_vc:");

  
  // die Koordinate mit dem hoechsten Wert stillegen ..
  d1 = fabs(p2->x - p1->x);
  d2 = fabs(p2->y - p1->y);
  d3 = fabs(p2->z - p1->z);
  
  if(d2 > d1) {             // Y > X
    if(d3 > d2) {bp = BCKPLN_XY; *lnv = d3;}  // skip Z-Coord
    else        {bp = BCKPLN_XZ; *lnv = d2;}  // skip Y-Coord (y am kleinsten)
  } else {                  // X > Y
    if(d3 > d1) {bp = BCKPLN_XY; *lnv = d3;}  // skip Z-Coord
    else        {bp = BCKPLN_YZ; *lnv = d1;}  // skip X-Coord (x am kleinsten)
  }

  // printf("ex UT3D_bplen_vc  %d %f\n",bp, *lnv);

  return bp;

}


//================================================================
  double UT3D_lenB_vc (Vector *vc1) {
//================================================================
/// \code
/// UT3D_lenB_vc     returns longest vectorpart
/// \endcode


  double   dx, dy, dz;

  // DEB_dump_obj__(Typ_VC, vc1, "UT3D_bp_perp_vc:");

  dx = fabs(vc1->dx);
  dy = fabs(vc1->dy);
  dz = fabs(vc1->dz);

  if(dx > dy) {
    if(dz > dx) return dz;
    else return dx;
  } else {
    if(dz > dy) return dz;
    else return dy;
  }
}


//================================================================
  int UT3D_bplen_vc (double *lnv, Vector *vc1) {
//================================================================
/// \code
/// UT3D_len1_vc     returns longest vectorpart and its backPlane
/// RC = 0 = BCKPLN_YZ = X-part-len
///      1 = BCKPLN_XZ = Y-part-len
///      2 = BCKPLN_XY = Z-part-len
/// \endcode


  int      bp;
  double   d1, d2, d3;

  // DEB_dump_obj__(Typ_VC, vc1, "UT3D_bp_perp_vc:");


  // die Koordinate mit dem hoechsten Wert stillegen ..
  d1 = fabs(vc1->dx);
  d2 = fabs(vc1->dy);
  d3 = fabs(vc1->dz);

  if(d2 > d1) {             // Y > X
    if(d3 > d2) {bp = BCKPLN_XY; *lnv = d3;}  // skip Z-Coord
    else        {bp = BCKPLN_XZ; *lnv = d2;}  // skip Y-Coord (y am kleinsten)
  } else {                  // X > Y
    if(d3 > d1) {bp = BCKPLN_XY; *lnv = d3;}  // skip Z-Coord
    else        {bp = BCKPLN_YZ; *lnv = d1;}  // skip X-Coord (x am kleinsten)
  }

  // printf("ex UT3D_bplen_vc  %d %f\n",bp, *lnv);

  return bp;

}


/*
//====================================================================
  double UT3D_len_vc (Vector *vc) {
//===============
// UT3D_len_vc              Vectorlength


  double lenv;
  // double sqval;


  // sqval = vc->dx*vc->dx + vc->dy*vc->dy + vc->dz*vc->dz;
  // printf("UT3D_len_vc val=%f\n",sqval);
  // lenv = sqrt(sqval);

  lenv = sqrt(vc->dx*vc->dx + vc->dy*vc->dy + vc->dz*vc->dz);
  // printf("UT3D_len_vc %f\n",lenv);

  return lenv;

}
*/

//=====================================================================
  double UT3D_len_ci (Circ *ci1) {
//=====================================================================
/// \code
/// UT3D_len_ci               length of circ
/// immer pos.
/// \endcode

  double   ao, la;


  // DEB_dump_obj__ (Typ_CI, ci1, "UT3D_len_ci:\n");


  // ao = UT3D_angr_ci__ (ci1);


  // Umfangswinkel * radius.
  // la = fabs(ao * ci1->rad);
  la = fabs(ci1->ango * ci1->rad);

  // printf("ex UT3D_len_ci la=%f ao=%f\n",la,ao);

  return la;

}


//====================================================================
  double UT3D_len_ln (Line *ln) {
//====================================================================
/// UT3D_len_ln              length of line


  Vector   vl;

  UT3D_vc_ln (&vl, ln);

  return (UT3D_len_vc(&vl));

}



//================================================================
  double UT3D_slen_ptpl (Point *pt, Plane *pl) {
//================================================================
/// \code
/// UT3D_slen_ptpl     signed distance point - plane
/// 
/// Value is positive  point ON same side as plane normal vector
/// Value is negativ   point on OPPOSITE side of plane normal vector
/// \endcode
// pl->vz must be normalized ?

  // skalarprod.
  return UT3D_acos_2vc (&pl->vz, (Vector*)pt) - pl->p;

}


/*
//================================================================
  double UT3D_slen_ptpl (Point *pt, Plane *pl) {
//================================================================
// alte Version:

// UT3D_slen_ptpl     signed distance point - plane

// UT3D_slen_ptpl     Author: Thomas Backmeister       4.7.2003

// Signed distance between a point and a plane.
// Value is positive <=> point ON side of plane normal vector
// Value is negativ  <=> point on OPPOSITE side of plane normal vector

  Vector vc, vn;

  // vector plane origin -> point
  UT3D_vc_2pt (&vc, &(pl->po), pt);

  // normalize plane normal vector
  UT3D_vc_normalize (&vn, &(pl->vz));

  return UT3D_acos_2vc (&vc, &vn);
}
*/


//================================================================
  double UT3D_lenB_2pt (Point *p1, Point *p2) {
//================================================================
/// UT3D_lenB_2pt          longest dx/dy/dz-distance point-point  (fast!)

  double dx, dy, dz;


  dx = fabs(p2->x - p1->x);
  dy = fabs(p2->y - p1->y);
  dz = fabs(p2->z - p1->z);

  if(dx > dy) {
    if(dz > dx) return dz;
    else return dx;
  } else {
    if(dz > dy) return dz;
    else return dy;
  }
}

 
//====================================================================
  double UT3D_len_2pt (Point *p1, Point *p2) {
//====================================================================
/// UT3D_len_2pt             distance point - point


  // Vector   vl;
  // UT3D_vc_2pt (&vl, p1, p2);
  // return (UT3D_len_vc (&vl));

  double dx, dy, dz;


  dx = p2->x - p1->x;
  dy = p2->y - p1->y;
  dz = p2->z - p1->z;

  return sqrt(dx*dx + dy*dy + dz*dz);

}


//========================================================================
  double UT3D_lenq_PtPt (Point *p1, Point *p2) {
//========================================================================
/// \code
/// UT3D_lenq_PtPt            square-distance point - point
/// alias UT3D_lenq_2pt
/// nur Abstand ohne sqrt ausrechen (nur f Vergleichswert)
/// \endcode

  double dx, dy, dz;

  dx = p2->x - p1->x;
  dy = p2->y - p1->y;
  dz = p2->z - p1->z;


  return dx*dx + dy*dy + dz*dz;
}


//================================================================
  int UT3D_lenq_PtLn (double *lenq, Point *p,
                      Point *p1, Point *p2) {
//================================================================
/// \code
/// UT3D_lenq_PtLn          minimum (quadr) distance Point-Segment
/// give min. distance from Point p to Line p1-p2
/// 
/// see also UT3D_nlenq_3pt
/// \endcode


  int      irc=0;
  double   t, ns2, x, y, z;
  Point    *px;


  // printf("UT3D_lenq_PtLn\n");
  // DEB_dump_obj__ (Typ_PT, p, "  pd:");
  // DEB_dump_obj__ (Typ_PT, p1, "  pa:");
  // DEB_dump_obj__ (Typ_PT, p2, "  pb:");


  // quadr. Laenge der Line
  ns2 = UT3D_lenq_PtPt (p1, p2);                 // UT3D_lenq_PtPt

  if (ns2 == 0.0) {px = p1; goto L_pt_pt;}

  // parameter von p auf der Line
  t = ((p2->x - p1->x)*(p->x - p1->x) +
       (p2->y - p1->y)*(p->y - p1->y) +
       (p2->z - p1->z)*(p->z - p1->z)) / ns2;


  if (t > 1.0) {
    px = p2;
    goto L_pt_pt;

  } else if (t < 0.0) {
    px = p1;
    goto L_pt_pt;


  } else {
    // Abstand von Projektion auf Line
    x = (1. - t) * p1->x + t * p2->x - p->x;
    y = (1. - t) * p1->y + t * p2->y - p->y;
    z = (1. - t) * p1->z + t * p2->z - p->z;
      // printf(" x=%f y=%f z=%f\n",x,y,z);

    *lenq = x * x + y * y + z * z;
    goto L_done;
  }

  L_pt_pt:
  *lenq = UT3D_lenq_PtPt (p, px);


  L_done:
  // printf("ex UT3D_lenq_PtLn %d %f\n",irc,*lenq);


  return irc;
}


//==============================================================================
  int UT3D_minLen_3pt (double *dp,Point *p0,Point *p1,Point *p2) {
//==============================================================================
/// \code
/// dp ist Streckensumme entlang der Hauptachsen - nicht genau Laenge !
/// rc = 1  p0 - p1 hat geringeren Abstand
/// rc = 2  p0 - p2 hat geringeren Abstand
/// dx,dy,dz addieren; den geringsten Wert auswaehlen.
/// \endcode


  int       ii;
  double    da[2];


  da[0] = fabs(p1->x - p0->x) + fabs(p1->y - p0->y) + fabs(p1->z - p0->z);
  da[1] = fabs(p2->x - p0->x) + fabs(p2->y - p0->y) + fabs(p2->z - p0->z);

  if(da[0] < da[1]) {ii = 1; *dp = da[0];}
  else              {ii = 2; *dp = da[1];}

  // printf("ex UT3D_minLen_3pt %d %f %f %f %f\n",ii,da[0],da[1],da[2],da[3]);

  return ii;

}

//================================================================
  int UT3D_minLen_npt (Point *p0, Point *pa, int pNr) {
//================================================================
// UT3D_minLen_npt           query min dist of n points

// see also AP_cont_nearPt

  int     i1, iind;
  double  d1, lmax;

  if(pNr < 2) return 0;

  lmax = UT_VAL_MAX;

  for(i1=0; i1<pNr; ++i1) {
    // printf(" nearPt[%d] %f %f %f\n",i1,ptTab[i1].x,ptTab[i1].y,ptTab[i1].z);
    d1 = UT3D_lenq_PtPt (p0, &pa[i1]);

    if(d1 < lmax) {
      iind = i1;
      lmax = d1;
    }
  }

  // DEB_dump_obj__ (Typ_PT, &ptTab[iind], "ex AP_cont_nearPt %d ",iind);

  return iind;

}


//==============================================================================
  int UT3D_minLen_4pt (double *dp,Point *p1a,Point *p1e,Point *p2a,Point *p2e) {
//==============================================================================
/// \code
/// dp ist Streckensumme entlang der Hauptachsen - nicht genau Laenge !
/// rc = 1  p1e - p2a hat geringsten Abstand
/// rc = 2  p1a - p2a hat geringsten Abstand
/// rc = 3  p1e - p2e hat geringsten Abstand
/// rc = 4  p1a - p2e hat geringsten Abstand
/// dx,dy,dz addieren; den geringsten Wert auswaehlen.
/// \endcode


  int       ii;
  double    da[4];

  // DEB_dump_obj__(Typ_PT, p1a, "p1a:");
  // DEB_dump_obj__(Typ_PT, p1e, "p1e:");
  // DEB_dump_obj__(Typ_PT, p2a, "p2a:");
  // DEB_dump_obj__(Typ_PT, p2e, "p2e:");


  da[0] = fabs(p2a->x - p1e->x) + fabs(p2a->y - p1e->y) + fabs(p2a->z - p1e->z);
  da[1] = fabs(p2a->x - p1a->x) + fabs(p2a->y - p1a->y) + fabs(p2a->z - p1a->z);
  da[2] = fabs(p2e->x - p1e->x) + fabs(p2e->y - p1e->y) + fabs(p2e->z - p1e->z);
  da[3] = fabs(p2e->x - p1a->x) + fabs(p2e->y - p1a->y) + fabs(p2e->z - p1a->z);

  ii = UTP_min (4, da);

  *dp = da[ii];

  ++ii;

  // printf("ex UT3D_minLen_4pt %d %f %f %f %f %f\n",ii,*dp,
                             // da[0],da[1],da[2],da[3]);

  return ii;

}


//=======================================================================
  double UT3D_slen_3pt (Point *p1, Point *p2, Point *p3) {
//=======================================================================
/// \code
/// UT3D_slen_3pt             signed distance point - point
/// 
/// p1-p2 gives the direction,
/// OUT:  signed distance p1-p3
///
///    ........ p3
///    .        .
///    .        .
///    .        .
///    .        .
///   p1--------+------>p2
///    <---rc--->
/// \endcode


  Vector   vl;

  UT3D_vc_2pt (&vl, p1, p2);

  return UT3D_slen_2ptvc (p1, p3, &vl);

}


//=======================================================================
  double UT3D_nlen_2ptvc (Point *p1, Point *pv, Vector *vc) {
//=======================================================================
/// \code
/// UT3D_nlen_2ptvc     Laenge der Normalen auf Vektor
/// 2003-11-07: p1-p2 vertauscht! RF.
/// 
/// p1 auf pv-vc projizieren (ergibt ps); die Laenge p1-ps liefern.
/// (p1 -ps ist normal auf vc)
/// 
///            X p1
///            |
///         rc |
///            |
///    pv------+-------->  vc
/// 
/// see also UT3D_pt_ck_ptvc UT3D_pt_ck_onLine UT3D_nlen_2ptvc UT3D_pt_projptptvc
/// \endcode


  Point  ps;
  double dl;


  UT3D_pt_projptptvc (&ps, &dl, NULL, p1, pv, vc);
  // printf("ex UT3D_nlen_2ptvc l=%f\n",dl);

  return dl;

}


//====================================================================
  int UT3D_nlenq_3pt (double *qlen,
                      Point *pa, Point *pb, Point *pc) {
//====================================================================
/// \code
/// UT3D_nlenq_3pt                 give quadr.Distance from point to line
/// pa - pb gives a line, pc is projected onto this line.
/// 
///            c
///            |
///         len|
///            |
///     a------+--------b
///            e
/// \endcode



  double    s_ab_ab, s_ab_ac, pe_ab;
  Vector    vab, vac, vae, vec;


  UT3D_vc_2pt (&vab, pa, pb);           // vab = Vector a-b

  return UT3D_nlenq_2ptvc (qlen, pc, pa, &vab);

}


//====================================================================
  int UT3D_nlenq_2ptvc (double *qlen,
                        Point *pc, Point *pa, Vector *vab) {
//====================================================================
/// \code
/// UT3D_nlenq_2ptvc               give quadr.Distance from point to line
/// pa - pb gives a line, pc is projected onto this line.
/// 
///           pc
///            |
///            |nlenq
///            |
///    pa------+--vab---b
///            e
/// \endcode


  Vector    vac;

  UT3D_vc_2pt (&vac, pa, pc);           // vac = Vector a-c

  return UT3D_nlenq_2vc (qlen, &vac, vab);

}


//====================================================================
  int UT3D_nlenq_2vc (double *qlen,
                      Vector *vac, Vector *vab) {
//====================================================================
/// \code
/// UT3D_nlenq_2ptvc               give quadr.Distance from point to line
/// pa - pb gives a line, pc is projected onto this line.
/// 
///            c
///          / |
///     vac/   |nlenq
///      /     |
///     a------+--vab---b
///            e
/// \endcode



  double    s_ab_ab, s_ab_ac, pe_ab;
  Vector    vae, vec;


  s_ab_ab = UT3D_skp_2vc (vab, vab);    // skp mit sich selbst = Laenge^2
  s_ab_ac = UT3D_skp_2vc (vab, vac);    // gibt Wert fuer e relativ zu s_ab_ab

  // Parameterwert von e zwischen a-b
  pe_ab = s_ab_ac / s_ab_ab;

  // Multip. des Vektors a-b mit Parameterwert von e ergibt den Vektor a-e
  UT3D_vc_multvc (&vae, vab, pe_ab);

  // Subtraktion (Vec-a-c - Vec-a-e) = Vec-e-c
  UT3D_vc_sub2vc (&vec, vac, &vae);

  *qlen = UT3D_skp_2vc (&vec, &vec);

    // printf("ex UT3D_nlenq_3pt %f\n",*qlen);

  return 0;

}


//=======================================================================
  double UT3D_nlen_3pt (Point *p1, Point *p2, Point *p3) {
//=======================================================================
/// \code
/// UT3D_nlen_3pt       Laenge der Normalen von p2 auf Line p1-p3
/// p2 liegt in der Mitte; p1-p3 ist die Linie.
/// p2 wird auf die Linie projiziert; der Abstand ist der Rueckgabewert.
/// 
///            X p2
///            |
///         rc |
///            |
///    p1------+--------p3
/// \endcode


  Point  ps;
  Vector vl;
  double dl;


  // DEB_dump_obj__ (Typ_PT, p1, "UT3D_nlen_3pt p1 ");
  // DEB_dump_obj__ (Typ_PT, p2, " p2 ");
  // DEB_dump_obj__ (Typ_PT, p3, " p3 ");


  UT3D_vc_2pt (&vl, p1, p3);

  UT3D_pt_projptptvc (&ps, &dl, NULL, p2, p1, &vl);

    // DEB_dump_obj__ (Typ_PT, &ps, " nlen_3pt ");
    // printf("ex UT3D_nlen_3pt l=%f\n",dl);

  return dl;

}


//================================================================
  double UT3D_slen_projvcvc (Vector *vc1, Vector *vc2) {
//================================================================
/// \code
/// UT3D_slen_projvcvc       signed length of vector projected on vector
/// length of vc1 does not influence result.
/// 3D-Projektionslaenge eines Vektors auf einen anderen Vektor:
/// vc2 wird auf vc1 projiziert.
/// negativ if Oeffnungswinkel > 90 Grad
/// if (length of vc1 = 1): use UT3D_skp_2vc
/// 
///            /|
///        V2/  |
///        /    |
///      /      |
///    x--------+------>  V1
///    <---rc--->
/// \endcode

// see also UT3D_slen_2ptvc UT3D_skp_2vc


  double skp, lv1;

  lv1 = UT3D_len_vc (vc1);

  if(lv1 < UT_TOL_min2) return 0.;

  skp = vc1->dx * vc2->dx + vc1->dy * vc2->dy + vc1->dz * vc2->dz;

  skp /= lv1;

  // printf("ex UT3D_slen_projvcvc %f\n",skp);

  return skp;

}


//================================================================
  double UT3D_slenq_projvcvc (Vector *vc1, Vector *vc2) {
//================================================================
/// \code
/// Quadr.3D-Projektionslaenge eines Vektors auf einen anderen Vektor:
/// vc2 wird auf vc1 projiziert.
/// negativ if Oeffnungswinkel > 90 Grad !
/// ACHTUNG: sqrt von einem negativen Wert geht ned !!!!
/// 
/// see also UT3D_ptDi_intptvcpln
/// 
/// 
///            /|
///        V2/  |
///        /    |
///      /      |
///    x--------+------>  V1
///    <---rc--->
/// \endcode

  double       s11, s12, lq;


  // DEB_dump_obj__ (Typ_VC, vc1, "UT3D_slenq_projvcvc  vc1:");
  // DEB_dump_obj__ (Typ_VC, vc2, "  vc2:");


  // s11 = quadr laenge von vc1
  s11 = UT3D_skp_2vc (vc1, vc1);
  if(s11 == 0.) {lq = 0; goto L_done;}

  // s12 = laenge rc in Verhaeltnis s11
  s12 = UT3D_skp_2vc (vc1, vc2);

  // quadr laenge rc
  lq = s12 * s12 / s11;
  if(s12 < 0.) lq = -lq;


  L_done:
  // printf("ex UT3D_slenq_projvcvc %f %f %f %f\n",lq,sqrt(lq),s11,s12);

  return lq;

}


//=======================================================================
  double UT3D_slenq_2ptvc (Point *p1, Point *p2, Vector *vc) {
//=======================================================================
/// \code
/// UT3D_slenq_2ptvc     Quadr. Laenge pt-pt in Richtung vec (m. Vorzeichen !)
/// die wahre Laenge ist sqrt(fabs(lq))*DSIGN(lq)
/// 
/// see also UT3D_slenq_projvcvc
///
///            p2
///            /|
///          /  |
///        /    |
///      /      |
///   p1--------+------>  VC
///    <---rc--->
/// \endcode


  double       s11, s12, lq;



  // s11 = quadr laenge von vc
  s11 = UT3D_skp_2vc (vc, vc);
  if(s11 == 0.) {lq = 0; goto L_done;}

  // s12 = laenge rc in Verhaeltnis s11
  s12 = UT3D_skp_2ptvc (p1, p2, vc);

  // quadr laenge rc
  lq = s12 * s12 / s11;
  if(s12 < 0.) lq = -lq;


  L_done:
  // printf("ex UT3D_slenq_2ptvc %f %f %f\n",lq,s11,s12);
  // printf("   len=%f\n",sqrt(fabs(lq))*DSIGN(lq));

  return lq;

}


//================================================================
  double UT3D_nlen_projvcvc (Vector *vc1, Vector *vc2) {
//================================================================
/// \code
/// 3D-Normalabstand eines Vektors auf einen anderen Vektor:
/// vc2 wird auf vc1 projiziert.
/// 
///                  _
///            /|    |
///        V2/  |    |
///         /  vc3  l3=rc
///        /    |    |
///      /      |    |
///    x--------+------------> V1
///    |---v1x--|
/// \endcode

  double skp, lv1, l3;
  Vector v1x, vc3;




  // DEB_dump_obj__(Typ_VC, vc1, "UT3D_nlen_projvcvc vc1:");
  // DEB_dump_obj__(Typ_VC, vc2, "  vc2:");


  lv1 = UT3D_len_vc (vc1);
  // wenn lenl==0 dann pp=pt
  if(lv1 < UT_TOL_min1) {
    return 0.;
  }

  // laenge von v2 auf v1 projiziert
  skp = vc1->dx * vc2->dx + vc1->dy * vc2->dy + vc1->dz * vc2->dz;
  skp /= lv1;
  skp /= lv1;
    // printf(" skp=%f %f\n",skp,lv1);

  // make vec v1x from laenge
  v1x.dx = vc1->dx * skp;
  v1x.dy = vc1->dy * skp;
  v1x.dz = vc1->dz * skp;
    // DEB_dump_obj__(Typ_VC, &v1x, "  v1x:");

  // vc3 = vc2 - v1x
  vc3.dx = vc2->dx - v1x.dx;
  vc3.dy = vc2->dy - v1x.dy;
  vc3.dz = vc2->dz - v1x.dz;
    // DEB_dump_obj__(Typ_VC, &vc3, "  vc3:");

  l3 = UT3D_len_vc (&vc3);

  // printf("ex UT3D_nlen_projvcvc %f\n",l3);

  return l3;

}


//====================================================================
  int UT3D_slenq_ptptvc (double *qlen,
                         Point *pa, Point *pb, Point *pc) {
//====================================================================
/// \code
/// UT3D_slenq_ptptvc         signed quadr.Distance pt-pt in Richtung vec
/// pa - pb gives a line, pc is projected onto this line.
/// 
///              c
///              |
///              |
///        len   |
///     a--------+------b
///              e
/// \endcode



  double    s_ab_ab, s_ab_ac, pe_ab;
  Vector    vab, vac, vae;


  UT3D_vc_2pt (&vab, pa, pb);           // vab = Vector a-b
  UT3D_vc_2pt (&vac, pa, pc);           // vac = Vector a-c

  s_ab_ab = UT3D_skp_2vc (&vab, &vab);  // skp mit sich selbst = Laenge^2
  s_ab_ac = UT3D_skp_2vc (&vab, &vac);  // gibt Wert fuer e relativ zu s_ab_ab

  // Parameterwert von e zwischen a-b
  pe_ab = s_ab_ac / s_ab_ab;

  // Multip. des Vektors a-b mit Parameterwert von e ergibt den Vektor a-e
  UT3D_vc_multvc (&vae, &vab, pe_ab);

  // Quadr. Abstand des Punktes e von a:
  // skp a-e mit sich selbst ergibt das Quadrat der Laenge;
  // wahre Laenge = sqrt(*qlen)
  *qlen = UT3D_skp_2vc (&vae, &vae) * DSIGN(pe_ab);

    // printf("ex UT3D_slenq_ptptvc %f (%f)\n",*qlen,sqrt(*qlen));

  return 0;

}


//=======================================================================
  double UT3D_slen_2ptvc (Point *p1, Point *p2, Vector *vc) {
//=======================================================================
/// \code
/// UT3D_slen_2ptvc     signed length of (p1-(p2 projected on (p1-vc))
/// Laenge pt-pt in Richtung vec (m. Vorzeichen !)
/// 
/// Wenn vc normiert ist (Laenge 1) dann genuegt Skalarprodunkt (UT3D_acos_2vc)
/// 
/// p1-vc definieren eine Plane;  p1=Origin, vc=Normalvektor.
/// p2 auf die Plane p1-vc projizieren (ergibt ps); die Laenge p2-ps liefern.
///
///    ........ p2
///    .        .
///    .        .
///    .        .
///    .        .
///   p1--------+------>  vc
///    <---rc--->
/// \endcode


  // double d1; Vector vc1={0.,10.,0.};
  // Point  p1={0.,0.,0.}, p2={-3.,-5.,0.};
  // d1 = UT3D_slen_2ptvc (&p1, &p2, &vc1);

// see UT3D_acos_2vc UT3D_parvc_2vc UT3D_slen_2ptvc UT2D_slen_nor3pt


  double lvc, skp;
  Vector vc2;


  // DEB_dump_obj__(Typ_VC, vc, "UT3D_slen_2ptvc");
  // DEB_dump_obj__(Typ_PT, p1, "p1");
  // DEB_dump_obj__(Typ_PT, p2, "p2");

  // lvc = Laenge Normalvec d. plane
  lvc = sqrt(vc->dx*vc->dx + vc->dy*vc->dy + vc->dz*vc->dz);

  // if(lvc < UT_TOL_min1) {
  // if(fabs(lvc) == 0.) {
  if(lvc < UT_TOL_min2) {
    printf("UT3D_slen_2ptvc E001\n");
    return 0.;
  }

  // vc2 = Vec. p1 --> p2
  vc2.dx = p2->x - p1->x;
  vc2.dy = p2->y - p1->y;
  vc2.dz = p2->z - p1->z;


  skp = (vc->dx*vc2.dx + vc->dy*vc2.dy + vc->dz*vc2.dz) / lvc;

  // printf("ex UT3D_slen_2ptvc: lvc=%f skp=%f\n",lvc,skp);

  return skp;

}






/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/


/*
//======================================================================
  int UT3D_comp2pt (Point *p1, Point *p2, double tol) {
//======================================================================
// UT3D_comp2pt              compare 2 points
// if(UT3D_comp2pt(&p1,&p1,tol)) printf("Punkt sind gleich!\n");
// RC = 0: Punkte nicht gleich; Abstand > tol.
// RC = 1: Punkte sind gleich; Abstand < tol.


  // printf("UT3D_comp2pt %f,%f,%f\n",p1->x,p1->y,p1->z);
  // printf("             %f,%f,%f\n",p2->x,p2->y,p2->z);



  return ((UTP_comp2db (p1->x, p2->x, tol)) &&
          (UTP_comp2db (p1->y, p2->y, tol)) &&
          (UTP_comp2db (p1->z, p2->z, tol)));
}
*/


//==============================================================================
  int UT3D_comp4pt (Point *p1a,Point *p1e,Point *p2a,Point *p2e,double tol) {
//==============================================================================
/// \code
/// UT3D_comp4pt              compare 4 points
/// Ob 2 der 4 Punkte zusammenpassen (ob 2 Lines einen gemeinsamen Punkt haben).
/// Es wird nur der Abstand entlang der Hauptachsen getestet !
/// 
///    rc=0: keine Verbindung
///    rc=1: bei El. liegen richtig.
///    rc=2: Obj 1 verkehrt
///    rc=3: Obj 2 verkehrt
///    rc=4: Obj 1 und Obj 2 verkehrt
/// 
/// Retcode 0 = Abstand aller Punkte > tol
///         1 = Abstand (p1e - p2a) < tol  (beide Obj. liegen korrekt)
///         2 = Abstand (p1a - p2a) < tol  (Obj 1 verkehrt)
///         3 = Abstand (p1e - p2e) < tol  (Obj 2 verkehrt)
///         4 = Abstand (p1a - p2e) < tol  (Obj 1 und 2 verkehrt)
/// \endcode

  // printf("UT3D_comp4pt: tol=%f\n",tol);
  // DEB_dump_obj__(Typ_PT, p1a, "p1a");
  // DEB_dump_obj__(Typ_PT, p1e, "p1e");
  // DEB_dump_obj__(Typ_PT, p2a, "p2a");
  // DEB_dump_obj__(Typ_PT, p2e, "p2e");


  if(UT3D_comp2pt(p1e, p2a, tol) == 1) return 1;
  if(UT3D_comp2pt(p1a, p2a, tol) == 1) return 2;
  if(UT3D_comp2pt(p1e, p2e, tol) == 1) return 3;
  if(UT3D_comp2pt(p1a, p2e, tol) == 1) return 4;


  return 0;

}


//================================================================
  int UT3D_compn4pt (Point *p1, Point *p2, Point *p3, Point *p4) {
//================================================================
/// \code
/// UT3D_compn4pt              compare 4 points - find nearest points
/// rc wie UT3D_comp4pt
/// 
/// rc=1: bei El. liegen richtig.    (p2-p3 nearest)
/// rc=2: Obj 1 verkehrt             (p1-p3 nearest)
/// rc=3: Obj 2 verkehrt             (p2-p4 nearest)
/// rc=4: Obj 1 und Obj 2 verkehrt   (p1-p4 nearest)
/// \endcode


  double   dTab[4];


  // DEB_dump_obj__(Typ_PT, p1, "  p1:");
  // DEB_dump_obj__(Typ_PT, p2, "  p2:");
  // DEB_dump_obj__(Typ_PT, p3, "  p3:");
  // DEB_dump_obj__(Typ_PT, p4, "  p4:");


  // den quadrat abstand testen;
  dTab[0] = UT3D_lenq_PtPt (p2, p3);
  dTab[1] = UT3D_lenq_PtPt (p1, p3);
  dTab[2] = UT3D_lenq_PtPt (p2, p4);
  dTab[3] = UT3D_lenq_PtPt (p1, p4);

  // printf("UT3D_compn4pt %f %f %f %f\n",dTab[0],dTab[1],dTab[2],dTab[3]);

  return UTP_min (4, dTab) + 1;

}


//================================================================
  int UT3D_sid_ptptvc (Point *ptx, Point *pto, Vector *vz) {
//================================================================
/// \code
/// UT3D_sid_ptptvc          compare if pt is in, above or below plane
///              ........ ptx
///              .        .
///              .        .
///              .        .
///              .        .
///             pto-------+------>  vz
///      below---|---above
///             in
///
/// Plane is defind by origin pto and z-vector vz
/// retcode:
///   0   pt is in plane pl
///   1   pt is above plane pl (same side as the z-vector)
///  -1   pt is below plane pl
/// see also UT3D_slen_2ptvc
/// \endcode


  int       rc;
  double    d1;
  Vector    vp;
  /* ------------------------------------------------------------- */

  UT3D_vc_2pt (&vp, pto, ptx);

  d1 = UT3D_acos_2vc (vz, &vp);



  // if (UTP_comp_0 (d1)) rc =  0;
  if (fabs(d1) == 0.)        rc =  0;

  else if (d1 > 0.)    rc =  1;

  else                 rc = -1;


  // printf("ex UT3D_sid_ptptvc %d\n",rc);
  return rc;
}



 
//===================================================================
  int UT3D_sid_ptpl (Point *pt, Plane *pl) {
//===================================================================
/// \code
/// UT3D_sid_ptpl         compare if pt is in, above or below plane
/// 
/// retcode:
///   0   pt is in plane pl
///   1   pt is above plane pl (same side as the z-vector)
///  -1   pt is below plane pl
/// \endcode


  int       rc;
  double    d1;
  Vector    vp;
  /* ------------------------------------------------------------- */

  UT3D_vc_2pt (&vp, &pl->po, pt);

  d1 = UT3D_acos_2vc (&pl->vz, &vp);



  if (UTP_comp_0 (d1)) rc =  0;

  else if (d1 > 0.)    rc =  1;

  else                 rc = -1;


  /*printf("UT3D_sid_ptpl %d\n",rc); */
  return rc;
}


//=====================================================================
  int UT3D_sid_pt_pt2vc (Point *pt0, Vector *vx, Vector *vy, Point *pt1) {
//=====================================================================
/// \code
// check if point pt1 is above or below plane from pt0,vx,vy
/// RC  -1 point is below plane vx-vy
/// RC   0 point is in plane vx-vy
/// RC   1 point is above plane vx-vy
/// \endcode

  Vector   vz, vp;

  UT3D_vc_perp2vc (&vz, vx, vy);  // vz = crossprod

  UT3D_vc_2pt (&vp, pt0, pt1);

  return UT3D_sid_2vc (&vp, &vz);

}


//================================================================
  int UT3D_sid_3pt (Point *p1, Point *p2, Point *p3) {
//================================================================
/// \code
/// UT3D_sid_3pt        check if pt is in, before or behind perpendic.Plane
/// Ist p3 vor oder hinter der Ebene, die durch p1 geht u.normal auf p1-p2 steht
/// RC  -1 p3 liegt vor Ebene p1-p2 (vis-a-vis)
/// RC   0 p3 liegt genau in der Ebene
/// RC   1 liegt hinter Ebene p1-p2 (in der Naehe von p2)
/// 
/// see also UT3D_acos_2vc
/// \endcode

  int    irc;
  double sk;
  Vector v1, v2;


  UT3D_vc_2pt (&v1, p1, p2);
  UT3D_vc_2pt (&v2, p1, p3);

  sk = v1.dx * v2.dx + v1.dy * v2.dy + v1.dz * v2.dz;

  // irc = UTP_COMP_0(sk);

  if(sk < -UT_TOL_min2)     irc = -1;
  else if(sk > UT_TOL_min2) irc = 1;
  else                      irc = 0;

  // printf("ex UT3D_sid_3pt %d %f\n",irc,sk);
  return irc;

}


//================================================================
  int UT3D_sid_2vc (Vector *v1, Vector *v2) {
//================================================================
/// \code
/// UT3D_sid_2vc        check vc's for perp, same or opposit direction
/// test if vectors are parallel or antipiparallel or normal ..
/// RC  -1 vc's point into opposit direction
/// RC   0 vc's are perpendicular
/// RC   1 vc's point into same direction
/// see also UT3D_acos_2vc
/// \endcode

// Skp = Laenge von Vec2 projiz. auf Vec1 * Laenge von Vec1.
// Wenn state=normal(90Grad) uninteressant, use UT3D_acos_2vc.

  int    irc;
  double sk;


  // DEB_dump_obj__ (Typ_VC, v1, "UT3D_sid_2vc");
  // DEB_dump_obj__ (Typ_VC, v2, "              ");


  sk = v1->dx * v2->dx + v1->dy * v2->dy + v1->dz * v2->dz;

  // irc = UTP_COMP_0(sk);

  if(sk < -UT_TOL_min1)     irc = -1;
  else if(sk > UT_TOL_min1) irc = 1;
  else                      irc = 0;

  // printf("ex UT3D_sid_2vc %d %f\n",irc,sk);

  return irc;

}


//================================================================
  int UT3D_pt_ck_linear (int np, Point *ptab, double tol) {
//================================================================
// UT3D_pt_ck_linear               Author: Karl Sauer           2005-08-22
/// \code
/// UT3D_pt_ck_linear       check straight position of points
/// Check if points form a straight line
/// 
/// IN:
///   int np       ... number of points
///   Point *ptab  ... points
///   double tol   ... tolerance for straightness (>0)
///                    (as smaller tol as straighter the points)
/// irc:
///   0 = position of points is not straight
///   1 = position of points is straight
/// \endcode



  int    rc, i1;
  double d1, dist2, rl, tol2;
  double a[3], r[3];       // Vec
  Point  p1;


  // printf("UT3D_pt_ck_linear %d\n",np);

  // check input
  if (np < 3) return 1;

  // wenn ptab ein geschlossene Kontur ist: remove last pt, sonst gehts ned !
  if(UT3D_comp2pt(&ptab[0],&ptab[np-1],UT_TOL_pt)) --np;

  // init
  tol2 = tol*tol;
  rc = 1;
    // printf(" tol2=%f\n",tol2);


  // compute square of distance (d1)
  // and unit vector (a): first point -> last point
  // d1 > 0.0 !!!!!
  a[0] = ptab[np-1].x - ptab[0].x;
  a[1] = ptab[np-1].y - ptab[0].y;
  a[2] = ptab[np-1].z - ptab[0].z;

  d1 = sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
  a[0] /= d1;
  a[1] /= d1;
  a[2] /= d1;
  // a ist nun Einheitsvec vom ersten in Richtung letzter Punkt
    // printf(" vc a=%f %f %f\n",a[0],a[1],a[2]);

  // check the distance(s) of the inner points
  for (i1=1; i1<np-1; ++i1) {
      // printf(" %d p=%f %f %f\n",i1,ptab[i1].x,ptab[i1].y,ptab[i1].z);

    // project current point i1 --> Line (first point - last point)
    // vector r: first point -> current point
    r[0] = ptab[i1].x - ptab[0].x;
    r[1] = ptab[i1].y - ptab[0].y;
    r[2] = ptab[i1].z - ptab[0].z;
      // printf(" %d vc r=%f %f %f\n",i1,r[0],r[1],r[2]);

    // point p1: on line next to current point
    rl = a[0]*r[0] + a[1]*r[1] + a[2]*r[2];
    p1.x = ptab[0].x + rl*a[0];
    p1.y = ptab[0].y + rl*a[1];
    p1.z = ptab[0].z + rl*a[2];
      // printf(" %d p1=%f %f %f\n",i1,p1.x,p1.y,p1.z);

    // check square of distance (dist2) of current point
    // (p1 - ptab[i1]) ist der Normalabstand; Strecke berechnen
    dist2 = UT3D_lenq_PtPt (&ptab[i1], &p1);
    // dist2 ist nun (dist * dist) !
      // printf(" %d dist2=%f\n",i1,dist2);

    if (dist2 > tol2) {
      rc = 0;
      break;
    }
  }

  // printf("ex UT3D_pt_ck_linear %d np=%d tol=%f\n",rc,np,tol);
  return rc;
}


/*
funktioniert mit folgenden daten nicht:
P[0] PT      -50.000,  -65.553,    0.000
P[1] PT      -50.000,  -58.837,    0.000
P[2] PT      -50.000,  -36.644,    0.000
P[3] PT      -50.000,  -16.154,    0.000

//=======================================================================
  int UT3D_pt_ck_linear (int np, Point *ptab, double tol) {
//==================
UT3D_pt_ck_linear       check straight position of points

UT3D_pt_ck_linear       Author: Thomas Backmeister       21.5.2003

Check if the position of points is straight.

IN:
  int np       ... number of points
  Point *ptab  ... points
  double tol   ... tolerance for straightness (>0)
                   (as smaller tol as straighter the points)
OUT:
Returncodes:
  0 = position of points is not straight
  1 = position of points is straight
//

  int rc, i1, i2, np1;
  double pn, pnmax;
  Point *ptab1=NULL;

  //printf("UT3D_pt_ck_linear: check straight position of points\n");

  // check input
  if (np < 3) return 1;

  // allocate memory
  ptab1 = (Point*) malloc (np * sizeof(Point));

  // copy points
  for (i1=0; i1<np; ++i1) {
    ptab1[i1] = ptab[i1];
  }

  // second forward differences of points
  np1 = np;
  for (i1=0; i1<2; ++i1) {
    --np1;
    for (i2=0; i2<np1; ++i2) {
      ptab1[i2].x = ptab1[i2+1].x - ptab1[i2].x;
      ptab1[i2].y = ptab1[i2+1].y - ptab1[i2].y;
      ptab1[i2].z = ptab1[i2+1].z - ptab1[i2].z;
    }
  }

  // measure the straightness
  pnmax = 0.0;
  for (i1=0; i1<np1; ++i1) {
    pn = sqrt (ptab1[i1].x * ptab1[i1].x + ptab1[i1].y * ptab1[i1].y +
               ptab1[i1].z * ptab1[i1].z);
    printf(" %d pn=%f\n",i1,pn);
    if (pn > pnmax) pnmax = pn;
  }
  if (np1 * (np1+1) * pnmax > tol) rc = 0;
  else rc = 1;

  // free memory
  if (ptab1 != NULL) free (ptab1);

  return rc;
}
*/



//================================================================
  int UT3D_pt_ck_npt (Point *p0, Point *pTab, int pNr, double tol) {
//================================================================
/// \code
/// UT3D_pt_ck_npt            compare point / points
/// check if p0 ist identical with one of the points of pTab
/// Retcode:
/// irc = -1    all points have a distance > tol
/// irc >= 0    index of (first) identical point;
/// \endcode


  int  i1, irc = -1;

  for(i1=0; i1<pNr; ++i1) {

    if(UT3D_comp2pt (p0, &pTab[i1], tol) == 1) {  // dist < tol = 1
      irc = i1;
      break;
    }
  }

  // DEB_dump_obj__ (Typ_PT, p0, "ex UT3D_pt_ck_npt %d ",irc);

  return -1;

}



//===================================================================
  int UT3D_pt_ck_ptvc (Point *pt, Point *pl, Vector *vl, double tol) {
//===================================================================
/// \code
/// UT3D_pt_ck_ptvc     check if point is on unlimited line (point-vector)
/// RC = -1   NO; Point pt is outside Line pl1-pl2
/// RC =  0   Yes; pt is nearer than tol to line pl1-pl2
/// 
/// see also UT3D_pt_ck_ptvc UT3D_pt_ck_onLine UT3D_nlen_2ptvc UT3D_pt_projptptvc
///   UT3D_pt_projptln 
/// \endcode

  int     irc;
  double  d1;
  Vector  vp, vx;


  // printf("UT3D_pt_ck_ptvc %f\n",tol);

  // d1 = Faktor Laenge Lotpunkt von p3 auf p1-p2
  UT3D_vc_2pt (&vp, pl, pt);
  UT3D_parvc_2vc (&d1, &vp, vl);
    // printf(" d1=%f\n",d1);

  // den Vektor vom pt zum Lotpunkt generieren (vecMult)
  UT3D_vc_multvc (&vx, vl, d1);
    // DEB_dump_obj__ (Typ_VC, &vx, "    vx ");

  // den vektor vom vp abziehen
  UT3D_vc_sub2vc (&vp, &vp, &vx);
    // DEB_dump_obj__ (Typ_VC, &vp, "    vp ");

  // Normalabstand von p3 auf p1-p2
  d1 = UT3D_len_vc (&vp);
    // printf(" d1=%f\n",d1);

  if(d1 > tol) irc = -1;
  else         irc = 0;

    // printf("ex UT3D_pt_ck_ptvc %d\n",irc);

  return irc;

}


//===================================================================
  int UT3D_pt_ck_2pt (Point *pt, Point *pl1, Point *pl2, double tol) {
//===================================================================
/// \code
/// UT3D_pt_ck_2pt     check if point is on unlimited line (2 points)
/// RC = -1   NO; Point pt is outside Line pl1-pl2
/// RC =  0   Yes; pt is nearer than tol to line pl1-pl2
/// 
/// see UT3D_pt_projptptvc UT3D_pt_projptln UT3D_pt_ck_onLine
/// \endcode


  // int     irc;
  // double  d1;
  Vector  vl;
  // Vector  vl, vp, vx;


  // printf("UT3D_pt_ck_2pt %f\n",tol);

  UT3D_vc_2pt (&vl, pl1, pl2);

  return UT3D_pt_ck_ptvc (pt, pl1, &vl, tol);

}


//======================================================================
  int UT3D_pt_ck_inLine (Point *p1, Point *p2, Point *p3, double tol) {
//======================================================================
// UT3D_pt_ck_inLine      Author: Thomas Backmeister       21.5.2003
/// \code
/// UT3D_pt_ck_inLine      check point on line segment
/// 
/// Check if a 3. point lies inside the line segment defined by two points.
/// It is assumed that the point is ON the line defined by the two points.
///
/// IN:
///   Point p1   ... point 1 defining the line segment
///   Point p2   ... point 2 defining the line segment
///   Point p3   ... point to check
///   double tol ... tolerance for point to ly inside line segment (>0)
/// OUT:
/// Returncodes:
///   0 = the point lies outside the segment
///   1 = the point lies inside the segment
/// \endcode

  int rc;
  double l1, l2, l3, lt;
  Vector v1, v2, v3;


  // printf("UT3D_pt_ck_inLine: check point on line segment\n");
  // DEB_dump_obj__ (Typ_PT, p1, " p1 =");
  // DEB_dump_obj__ (Typ_PT, p2, " p2 =");
  // DEB_dump_obj__ (Typ_PT, p3, " p3 =");

  // init
  rc = 0;


  // test identical points 2011-08-22 RF
  if(UT3D_comp2pt (p1, p3, tol)) return 1;
  if(UT3D_comp2pt (p2, p3, tol)) return 1;


  // vectors from point to point
  UT3D_vc_2pt (&v1, p1, p2);
  UT3D_vc_2pt (&v2, p1, p3);
  UT3D_vc_2pt (&v3, p2, p3);

  // square of length of vectors
  l1 = v1.dx * v1.dx + v1.dy * v1.dy + v1.dz * v1.dz;
  l2 = v2.dx * v2.dx + v2.dy * v2.dy + v2.dz * v2.dz;
  l3 = v3.dx * v3.dx + v3.dy * v3.dy + v3.dz * v3.dz;

  // check point position
  lt = l1 + tol;
  if (l2 <= lt && l3 <= lt) rc = 1;

    // printf("ex UT3D_pt_ck_inLine: %d %f %f %f\n",rc, lt, l2, l3);

  return rc;
}


//======================================================================
  int UT3D_pt_ck_onLine (Point *p1,Point *p2,Point *p3, double tol) {
//======================================================================
/// \code
/// liegt p3 auf der Linie p1-p2 ?      (Tests mit UT_TOL_pt)
/// RC = -1   NO; Point outside Line p1-p2
/// RC =  0   Yes; p3=p1; po=p1.
/// RC =  1   Yes; po between p1-p2
/// RC =  2   Yes; p3=p2; po=p2.
/// 
/// see also UT3D_pt_ck_onLnSeg
/// see also UT3D_pt_ck_ptvc UT3D_nlen_2ptvc UT3D_pt_projptptvc
///   UT3D_pt_projptln 
/// \endcode

  int     irc;
  double  d1;
  Point   pb1, pb2, px;
  Vector  vl, vp, vx;


  // printf("------------------------------------ \n");
  // DEB_dump_obj__ (Typ_PT, p3, "UT3D_pt_ck_onLine tol=%f",tol);
  // DEB_dump_obj__ (Typ_PT, p1, "    p1 ");
  // DEB_dump_obj__ (Typ_PT, p2, "    p2 ");


  // check if point is in box oder voellig ausserhalb ..
  UT3D_box_2pt__ (&pb1, &pb2, p1, p2);

  // test if p3 lies inside Box with tol
  if(UT3D_pt_ck_inBoxTol (&pb1, &pb2, p3, tol) == 1) return -1; // outside



  // check if point is near lineStartPoint
  // p3 = p1 ??
  if(UT3D_comp2pt (p1, p3, tol) == 1) {
    // *po = *p1;
    return 0;
  }


  // check if point is near lineEndPoint
  // p3 = p2 ??
  if(UT3D_comp2pt (p2, p3, tol) == 1) {
    // *po = *p2;
    return 2;
  }


  // d1 = Faktor Laenge Lotpunkt von p3 auf p1-p2
  UT3D_vc_2pt (&vl, p1, p2);
  UT3D_vc_2pt (&vp, p1, p3);
  UT3D_parvc_2vc (&d1, &vp, &vl);
  // printf(" d1=%f\n",d1);

  // den Vektor vom p1 zum Lotpunkt generieren (vecMult)
  UT3D_vc_multvc (&vx, &vl, d1);
  // DEB_dump_obj__ (Typ_VC, &vx, "    vx ");

  // den vektor vom vp abziehen
  UT3D_vc_sub2vc (&vp, &vp, &vx);
  // DEB_dump_obj__ (Typ_VC, &vp, "    vx ");

  // Normalabstand von p3 auf p1-p2
  d1 = UT3D_len_vc (&vp);


  if(d1 > tol) irc = -1;
  else         irc = 1;


  // printf("ex UT3D_pt_ck_onLine irc=%d\n",irc);
  return irc;

}


//======================================================================
  int UT3D_pt_ck_onLnSeg (Point *p1,Point *p2,Point *p3, double tol) {
//======================================================================
/// \code
/// liegt p3 auf der Linie p1-p2 ?      (Tests mit UT_TOL_pt)
/// Speziell fuer Polygone;
/// RC = -1   NO; Point outside Line p1-p2
/// RC =  0   Yes; p3=p1; po=p1.
/// RC =  1   Yes; po between p1-p2
/// RC =  2   Yes; p3=p2; po=p2.
/// \endcode

  int     irc;
  double  d1;
  Point   pb1, pb2, px;
  Vector  vl, vp, vx;


  // DEB_dump_obj__ (Typ_PT, p3, "UT3D_pt_ck_onLine tol=%f",tol);
  // DEB_dump_obj__ (Typ_PT, p1, "    p1 ");
  // DEB_dump_obj__ (Typ_PT, p2, "    p2 ");



  // check if point is in box oder voellig ausserhalb ..
  UT3D_box_2pt__ (&pb1, &pb2, p1, p2);


  // test if p3 lies inside Box with tol
  if(UT3D_pt_ck_inBoxTol (&pb1, &pb2, p3, tol) == 1) {
    irc = -1;
    goto L_done;
  }



  // check if point is near lineStartPoint
  // p3 = p1 ??
  if(UT3D_comp2pt (p1, p3, tol) == 1) {
    // *po = *p1;
    irc = 0;
    goto L_done;
  }


  // check if point is near lineEndPoint
  // p3 = p2 ??
  if(UT3D_comp2pt (p2, p3, tol) == 1) {
    // *po = *p2;
    return 2;
  }


  // d1 = Faktor Laenge Lotpunkt von p3 auf p1-p2
  UT3D_vc_2pt (&vl, p1, p2);
  UT3D_vc_2pt (&vp, p1, p3);
  UT3D_parvc_2vc (&d1, &vp, &vl);
  // printf(" d1=%f\n",d1);

  // den Vektor vom p1 zum Lotpunkt generieren (vecMult)
  UT3D_vc_multvc (&vx, &vl, d1);
  // DEB_dump_obj__ (Typ_VC, &vx, "    vx ");


  // den Lotpunkt erzeugen
  UT3D_pt_traptvc (&px, p1, &vx);


  // test m box
  if(UT3D_comp2pt (&px, p3, tol) == 1) {
    // *po = *p1;
    irc = 1;
  }

  L_done:
  // printf("ex UT3D_pt_ck_onLnSeg irc=%d\n",irc);
  return irc;

}


//================================================================
  int UT3D_pt_ck_inSph (Point *pt, Point *ps, double rs) {
//================================================================
/// \code
/// UT3D_pt_ck_inSph              check if point is inside sphere
///
/// irc:
///  1 = YES point lies inside
///  0 = --- point touches
/// -1 = NO  point lies outside
/// \endcode

  int    irc;
  double d1;


  d1 = UT3D_len_2pt (pt, ps);

  if(fabs(d1-rs) < UT_TOL_pt) irc = 0;      // touch
  else if(d1 < rs)            irc = 1;      // inside
  else                        irc = -1;     // outside


  // printf("ex UT3D_pt_ck_inSph %d\n",irc);
  return irc;

}


//====================================================================
  Point UT3D_pt_pt2 (Point2 *pt20) {
//====================================================================
/// UT3D_pt_pt2               3D-Point = 2D-Point    (add z=0.)
/// see UT3D_pt_pt2_0  INLINE

  Point  pt;

  pt.x = pt20->x;
  pt.y = pt20->y;
  pt.z = 0.0;

  return pt;

}


//====================================================================
  void UT3D_pt_3db (Point *pt, double x, double y, double z) {
//====================================================================
/// UT3D_pt_3db             Point < 3 doubles (x, y, z)


  pt->x = x;
  pt->y = y;
  pt->z = z;

  return;

}


//====================================================================
  void UT3D_pt_mid2pt (Point *po, Point *p1, Point *p2) {
//====================================================================
/// \code
/// UT3D_pt_mid2pt                  midpoint between 2 points
/// po may be p1
/// \endcode


  /* ----------------------------- */

  po->x = (p1->x + p2->x) / 2.0;
  po->y = (p1->y + p2->y) / 2.0;
  po->z = (p1->z + p2->z) / 2.0;

  /*printf("UT3D_midPoint %f %f %f\n",po->x,po->y,po->z); */


  return;
}


//=====================================================================
  int UT3D_pt_mid_pta (Point *pto, Point *pTab, int ptNr, int iExact) {
//=====================================================================
/// \code
/// Input:
///   iExact    0=arithmetic, 1=midIndex
/// arithm. Mittelwert der Punktetabelle pTab
///   (alle Punktcoord addieren dann durch PunkteAnzahl divid.)
/// \endcode

  int     i1;
  Point   p1;


  if(ptNr < 2) {
    *pto = *pTab;
    goto L_done;
  }
    

  p1.x = 0.;
  p1.y = 0.;
  p1.z = 0.;

  if((ptNr > 2)&&(iExact)) {
    i1 = ptNr / 2;
    p1 = pTab[i1];
      // printf(" pt_mid_pta-ptNr=%d i1=%d\n",ptNr,i1);
    goto L_exit;
  }

  for(i1=0; i1<ptNr; ++i1) {
     p1.x += pTab[i1].x;
     p1.y += pTab[i1].y;
     p1.z += pTab[i1].z;
  }

  p1.x /= ptNr;
  p1.y /= ptNr;
  p1.z /= ptNr;


  L_exit:
    *pto = p1;

  L_done:
    // GR_tDyn_symB__ (pto, SYM_SQU_B, 2);

  return 0;

}


//=======================================================================
  void UT3D_pt_mid_ci (Point *po, Circ *ci1) {
//=======================================================================
///  UT3D_pt_mid_ci             midpoint of a circ

  int       sr, sri = -1;
  double    ao, a1;
  Vector    vc1, vc2, vm;

  // DEB_dump_obj__ (Typ_CI, ci1, "UT3D_pt_mid_ci");
  // GR_Disp_ac (ci1, Typ_Att_blue);


  sr = DLIM01(ci1->rad);  // 0=CCW, 1=CW

  ao = fabs(ci1->ango);

  // test 360-deg-circ
  if(fabs(RAD_360 - ao) < UT_TOL_min1)  {
    // opposite point for 360-deg-circ
    UT3D_pt_opp2pt (po, &ci1->pc, &ci1->p1);
    goto L_exit;
  }

  // vc1 = circCen -> circStartPt
  UT3D_vc_2pt (&vc1, &ci1->pc, &ci1->p1);

  // 180-deg-circ: 
  a1 = fabs(RAD_180 - ao);
  if(a1 < UT_TOL_min1) {
    if(sr) {
      // rot 90 deg CW
      UT3D_vc_perp2vc (&vm, &vc1, &ci1->vz);
    } else {
      // rot 90 deg CCW
      UT3D_vc_perp2vc (&vm, &ci1->vz, &vc1);
    }
    UT3D_pt_traptvc (po, &ci1->pc, &vm);
    goto L_exit;
  }

  // vc2 = circCen -> circEndPt
  UT3D_vc_2pt (&vc2, &ci1->pc, &ci1->p2);

  // test 170-190
  if(a1 < RAD_10) {
    // rot vc1 90 deg CCW
    UT3D_vc_rot_90_sr (&vc1, &ci1->vz, &vc1, sr);
    // rot vc2 90 deg CW
    sri = ICHG01(sr);
    UT3D_vc_rot_90_sr (&vc2, &ci1->vz, &vc2, sri);
  }

  // vm = midVec
  vm.dx = vc1.dx + vc2.dx;
  vm.dy = vc1.dy + vc2.dy;
  vm.dz = vc1.dz + vc2.dz;

  // change side of vm; not for 170-190
  if((sri < 0)&&(ao > RAD_180)) UT3D_vc_invert (&vm, &vm);

  // der Punkt = radius > vm
  UT3D_pt_traptvclen (po, &ci1->pc, &vm, fabs(ci1->rad));


  L_exit:

      // DEB_dump_obj__ (Typ_PT, po, "ex-UT3D_pt_mid_ci");
      // GR_tDyn_symB__ (po, SYM_SQU_B, 2);

    return;

}

    
//=============================================================================
  int UT3D_vc_rot_90_sr (Vector *vcOut, Vector *vcAxis, Vector *vcIn, int sr) {
//=============================================================================
// UT3D_vc_rot_90_sr           rotate vector 90 deg CCW or CW around axis
// Input:
//   sr      0=CCW, 1=CW

  if(sr)
    // rotate vector 90 deg CW around axis
    UT3D_vc_perp2vc (vcOut,vcIn,vcAxis);
  else
    // rotate vector 90 deg CCW around axis
    UT3D_vc_perp2vc (vcOut,vcAxis,vcIn);

  return 0;

}


//================================================================
  int UT3D_pt_std_ci (Point *po, int imod, Circ *ci1) {
//================================================================
/// \code
/// UT3D_pt_std_ci         get 90/180/270-deg-point of circ
/// imod -1: rc returns nr of points; 4 for 360-deg-circs, else 2.
/// if (circ == 360-deg-circ):
///   imod: 0=0, 1=90, 2=180, 3=270 Grad !
/// else if (circ < 360-deg):
///   imod: 0=p1, 1=p2
/// For nr of points use UTO_ptnr_std_obj
/// \endcode

  Vector   vc1, vc2;


  // printf("UT3D_pt_std_ci %d\n",imod);


/*
  if(UT3D_ck_ci360(ci1) == 0) goto L_360;

  if(imod < 0)  return 2;
  else if(imod == 0) *po = ci1->p1;
  else               *po = ci1->p2;
  goto L_done;
*/



  //================================================================
  L_360:
  if(imod < 0)  return 5;

  // 360-deg-circ: 0=0, 1=90, 2=180, 3=270 Grad
  if(imod == 0) {            // Startpt
    *po = ci1->p1;

  } else if(imod == 1) {     // 90
    UT3D_vc_2pt (&vc1, &ci1->p1, &ci1->pc);
    UT3D_vc_perp2vc (&vc2, &vc1, &ci1->vz);
      // DEB_dump_obj__(Typ_VC, &vc2, "-pt_ci__-1");
    UT3D_pt_traptvc (po, &ci1->pc, &vc2);

  } else if(imod == 2) {     // 180
    UT3D_pt_opp2pt (po, &ci1->pc, &ci1->p1);

  } else if(imod == 3) {     // 270
    UT3D_vc_2pt (&vc1, &ci1->pc, &ci1->p1);
    UT3D_vc_perp2vc (&vc2, &vc1, &ci1->vz);
      // DEB_dump_obj__(Typ_VC, &vc2, "-pt_ci__-3");
    UT3D_pt_traptvc (po, &ci1->pc, &vc2);

  } else if(imod == 4) {     // Endpt
    *po = ci1->p2;

  } else {
    TX_Error("UT3D_pt_std_ci E001");
    return -1;
  }


  L_done:
  // GR_tDyn_symB__ (po, SYM_STAR_S, 1);

  return 0;

}


//=======================================================================
  int UT3D_ptvcpar_std_dbo (Point *pto, Vector *vco, double *par,
                            int pType, int dbtyp, long dbi) {
//=======================================================================
/// \code
/// UT3D_ptvcpar_std_obj   get typical-points,tangent-vector,parameter for DB-obj
/// see UT3D_ptvcpar_std_obj
/// \endcode

  ObjGX   oxi;


  OGX_SET_INDEX (&oxi, dbtyp, dbi);

  return UT3D_ptvcpar_std_obj (pto, vco, par,
                               0, pType, Typ_ObjGX, &oxi);

}


//=======================================================================
  int UT3D_ptvcpar_ipt_obj (Point *pto, Vector *vco, double *par,
                            int ipt, int cvTyp, void *cvDat) {
//=======================================================================
/// \code
/// UT3D_ptvcpar_ipt_obj     get control-points/tangent-vector/param. for curve
///
/// Input:
///   pto        NULL for no output
///   vco        NULL for no output 
///   par        NULL for no output  - else par 0-1
///   ipt        point index; 1=first; eg controlpoint of polygon|spline
///   cvTyp      type of cvDat; eg Typ_LN
///   cvDat      line/curve, eg struct Line
/// Output: 
///   pto        point out
///   vco        vector out      normalized ??
///   par        parameter out (vTyp=0; see INF_struct_par)
///   retcod     0=ok, -1=Error
/// 
/// nr of std-points: UTO_ptnr_std_obj
/// boxpoints: see UT3D_box_obja
/// parameter: see UT3D_pt_vc__par_cv UT3D_pt_evparcrv
/// \endcode
      
// TODO: get start/endPoints out of PRCV
      
      
  int     irc, i1, i2, ptNr, ifRes;
  char    *cp1;
  double  db1; 
  Point   pt1, *pTab;
  void    *vp1;


  // printf("UT3D_ptvcpar_ipt_obj ipt=%d cvTyp=%d\n", ipt, cvTyp);
  // DEB_dump_obj__ (cvTyp, cvDat, "  _ipt_obj-obj");

  irc = 0;

  switch (cvTyp) {

    //----------------------------------------------------------------
    case Typ_PT2:
      if(pto) *pto = UT3D_pt_pt2 ((Point2*)cvDat);
      irc = 1;
      break;

    //----------------------------------------------------------------
    case Typ_PT:
      if(pto) *pto = *((Point*)cvDat);
      irc = 1;
      break;

    //----------------------------------------------------------------
    case Typ_LN:
      if(vco) UT3D_vc_ln (vco, (Line*)cvDat);
      irc = 3;

      if(ipt <= 1) { // startpoint
        if(pto) *pto = ((Line*)cvDat)->p1;
        if(par) *par = 0.;
        break;

      } else if(ipt == 2) { // endpoint
        if(pto) *pto = ((Line*)cvDat)->p2;
        if(par) *par = 1.;
        break;

      } else if(ipt == 3) { // midpoint
        if(pto) UT3D_pt_mid2pt (pto, &((Line*)cvDat)->p1, &((Line*)cvDat)->p2);
        if(par) *par = 0.5;
        break;

      }
      goto L_err_FNI;

    //----------------------------------------------------------------
    case Typ_CI:
      irc = 3;
      if(ipt == 1) { // startpoint
        pt1 = ((Circ*)cvDat)->p1;
        if(par) *par = 0.;
        goto L_CI2;

      } else if(ipt == 2) { // endpoint
        pt1 = ((Circ*)cvDat)->p2;
        if(par) *par = 1.;
        goto L_CI2;

      } else if((ipt == 3)||(ipt == 0)) { // midpoint / 180-deg-point
        if(pto) *pto = ((Circ*)cvDat)->pc;
        break;

      } else goto L_err_FNI;

      L_CI2:
        if(pto) *pto = pt1;
        if(vco) UT3D_vc_tng_ci_pt (vco, &pt1, (Circ*)cvDat);

        break;

    //----------------------------------------------------------------
    // Typ_Note
    case Typ_GTXT:
      // DEB_dump_obj__ (Typ_GTXT, obj, "GText:");
      if(pto) *pto = (((GText*)cvDat)->pt);
      irc = 1;
      break;

    //----------------------------------------------------------------
    case Typ_ATXT:
      // DEB_dump_obj__ (Typ_GTXT, obj, "GText:");
      if(pto) *pto = (((AText*)cvDat)->p1);
      irc = 1;
      break;

    //----------------------------------------------------------------
    // Typ_Model
    case Typ_Model:
      if(pto) *pto = (((ModelRef*)cvDat)->po);
      irc = 1;
      break;

    //----------------------------------------------------------------
    // Curves:
    case Typ_CVPOL:
      ptNr = ((CurvPoly*)cvDat)->ptNr;
      if(ipt > ptNr) goto L_err_par;
      --ipt;
      if(pto) *pto = ((CurvPoly*)cvDat)->cpTab[ipt];
      if(vco) goto L_err_FNI; // UT3D_vc_evalplg (vco, (CurvPoly*)cvDat, db1);
      if(par) goto L_err_FNI; // *par = UT3D_par1_parplg (&db1, (CurvPoly*)cvDat);
      irc = ptNr;
      break;

    //----------------------------------------------------------------
    case Typ_CVELL:
      irc = 4;
      if(ipt == 1) { // startpoint
        pt1 = ((CurvElli*)cvDat)->p1;
        if(par) *par = 0.;
        goto L_ELL2;

      } else if(ipt == 2) { // endpoint
        pt1 = ((CurvElli*)cvDat)->p2;
        if(par) *par = 1.;
        goto L_ELL2;


      } else if(ipt == 3) { // focus 1
        UT3D_pt_elfoc (pto, NULL, (CurvElli*)cvDat);
        break;

      } else if(ipt == 4) { // focus 2
        UT3D_pt_elfoc (NULL, pto, (CurvElli*)cvDat);
        break;

      } else goto L_err_FNI;

      L_ELL2:
        if(pto) *pto = pt1;
        if(vco) {
          if(!UT3D_ck_el360((CurvElli*)cvDat)) {
            *vco = ((CurvElli*)cvDat)->vb;
          } else {
            UT3D_vc__pt_el (vco, &pt1, (CurvElli*)cvDat);
          }
        }
      break;

    //----------------------------------------------------------------
    case Typ_CVCLOT:
      if(ipt <= 1)  {
        if(pto) *pto = ((CurvClot*)cvDat)->stp;
        if(vco) *vco = ((CurvClot*)cvDat)->stv;
        if(par) *par = 0.;

      } else if(ipt == 2) { // endpoint
        if((pto) || (vco))
          UT3D_ptvc_evparclot (pto, vco, 1, (CurvClot*)cvDat, 1.);
        if(par) *par = 1.;

      } else goto L_err_FNI;

      irc = 2;
      break;

    //----------------------------------------------------------------
    case Typ_CVBSP:
      irc = ((CurvBSpl*)cvDat)->ptNr;
      if(ipt > irc) goto L_err_par;
      ipt -= 1;
      if(pto) *pto = ((CurvBSpl*)cvDat)->cpTab[ipt];
      // db1 = ((CurvBSpl*)cvDat)->kvTab[i1];
      // if(vco) UT3D_vc_evalparCv (vco, (CurvBSpl*)cvDat, db1);
      if(vco) goto L_err_FNI;
      // if(par) *par = db1;
      if(par) goto L_err_FNI;
      break;

    //----------------------------------------------------------------
    case Typ_CVRBSP:
      irc = ((CurvRBSpl*)cvDat)->ptNr;
      if(ipt > irc) goto L_err_par;
      ipt -= 1;
      if(pto) *pto = ((CurvRBSpl*)cvDat)->cpTab[ipt];
      //db1 = ((CurvRBSpl*)cvDat)->kvTab[i1];
      if(vco) goto L_err_FNI;
      //if(par) *par = db1;
      if(par) goto L_err_FNI;
      break;

    //----------------------------------------------------------------
    case Typ_ObjGX:
      i1 = UTO_objDat_ox (&vp1, &i2, cvDat);
      if(i1 < 0) return -1;
      if(i2 > 1) TX_Print("**** UT3D_ptvcpar_ipt_obj I001");

      // recursion
      irc = UT3D_ptvcpar_ipt_obj (pto, vco, par, ipt, i1, vp1);
      break;


    //----------------------------------------------------------------
    case Typ_CVTRM:
      // Typ_CVTRM can have form=Typ_ObjGX (contour) 
      //   or form=Typ_CVTRM(single trimmed curve)
      cp1 = (char*) MEM_alloc_tmp (OBJ_SIZ_MAX);
      UTO_cv_cvtrm (&i1, cp1, NULL, (CurvCCV*)cvDat);
// TODO: spline, clot has v0-point, v1-point in CurvCCV-struct (io0, ip1) !?
      // recursion
      irc = UT3D_ptvcpar_ipt_obj (pto, vco, par, ipt, i1, cp1);
      break;


    //----------------------------------------------------------------
    default:
      goto L_err_FNI;
  }


    // TESTBLOCK
    // printf(" ex-UT3D_ptvcpar_ipt_obj-irc =%d\n",irc);
    // if(irc >= 0) {
      // if(par) printf("ex UT3D_ptvcpar_ipt_obj par=%lf\n",*par);
      // if(pto) DEB_dump_obj__ (Typ_PT, pto, "ex UT3D_ptvcpar_ipt_obj");
      // if(pto) GR_tDyn_symB__ (pto, SYM_STAR_S, ATT_COL_RED);
      // if(vco) GR_tDyn_vc__ (vco, pto, 9, 1);
    // }
    // END TESTBLOCK


  return irc;


  //----------------------------------------------------------------
  L_err_FNI:  // function not implemented
    DEB_dump_obj__ (cvTyp, cvDat, " *** ERR UT3D_ptvcpar_ipt_obj");
    return MSG_ERR__ (ERR_func_not_impl, "/ pTyp %d cvTyp %d", ipt, cvTyp);

  L_err_par:
    DEB_dump_obj__ (cvTyp, cvDat, " *** ERR UT3D_ptvcpar_ipt_obj");
    TX_Print("**** UT3D_ptvcpar_ipt_obj error parameter");

  return -1;

}


//=====================================================================
  int UT3D_vc_mod_obj (Vector *vco, int iMod, int cvTyp, void *cvDat) {
//=====================================================================
// UT3D_vc_mod_obj              get typical (normal-)vectors for obj (MOD)
//   for tangents see UT3D_ptvcpar_std_obj
//
// Main/sub-axis of Circ, ellipse, Plane:
//
// iMod   plane   ellipse       circle
//  0    Z-axis  normal-axis   normal-axis
//  1    X-axis  major-axis    center-startpoint
//  2    Y-axis  minor-axis    center-endpoint
//
//  retcod    2 (nr of solutions)
//           -1 Error

  int    irc = 2;


  // printf("UT3D_vc_mod_obj iMod=%d cvTyp=%d\n", iMod, cvTyp);


  switch (cvTyp) {

    //----------------------------------------------------------------
    case Typ_CI: 
      if(iMod == 1) {
        UT3D_vc_2pt(vco, &((Circ*)cvDat)->pc, &((Circ*)cvDat)->p1);
      } else if(iMod == 2) {
        UT3D_vc_2pt(vco, &((Circ*)cvDat)->pc, &((Circ*)cvDat)->p2);
      } else {
        *vco = ((Circ*)cvDat)->vz;
      }
      break;

    //----------------------------------------------------------------
    case Typ_CVELL:
      if(iMod == 1) *vco = ((CurvElli*)cvDat)->va;
      else if(iMod == 2) *vco = ((CurvElli*)cvDat)->vb;
      else *vco = ((CurvElli*)cvDat)->vz;
      break;

    //----------------------------------------------------------------
    case Typ_PLN:
      if(iMod == 1) *vco = ((Plane*)cvDat)->vx;
      else if(iMod == 2) *vco = ((Plane*)cvDat)->vy;
      else *vco = ((Plane*)cvDat)->vz;
      break;

    //----------------------------------------------------------------
    default:
      goto L_err_FNI;
  }


  return irc;


  //----------------------------------------------------------------
  L_err_FNI:  // function not implemented
    DEB_dump_obj__ (cvTyp, cvDat, " *** ERR UT3D_vc_mod_obj");
    return MSG_ERR__ (ERR_func_not_impl, "/ MOD %d cvTyp %d", iMod, cvTyp);

}

 
//========================================================================
  int UT3D_ptvcpar_std_obj (Point *pto, Vector *vco, double *par,
                            int vTyp, int pType, int cvTyp, void *cvDat) {
//========================================================================
/// \code
// UT3D_ptvcpar_std_obj     get typical-points/parameter/tangent-vector for obj
//                       (start, end, mid, ..)
//  for normal-vectors: see UT3D_vc_mod_obj
//   (parametric-points, typical-points, standardpoints, characteristic-points)
//
// Input:
//   pto        NULL for no output
//   vco        NULL for no output
//   par        NULL for no output
//   vTyp       0 get knotvalues for spline/polygon, else 0-1. See INF_struct_par.
//              1 get values from 0-1 for all types of curves. See INF_struct_par.
//   pType      which point to compute (eg Ptyp_start|Ptyp_end ../ut/AP_types.h)
//   cvTyp      type of cvDat; eg Typ_LN
//   cvDat      line/curve, eg struct Line
//
// Output:
//   pto        point out
//   vco        vector out      normalized ??
//   par        parametervalue 0-1 or knotvalue for splines,polygon;
//              (vTyp=0 - see INF_struct_par)
//   retcod     -1=Error, >=0 = nr-of-typical-points
//
// nr of std-points: UTO_ptnr_std_obj
// boxpoints: see UT3D_box_obja
// parameter: see UT3D_pt_vc__par_cv UT3D_pt_evparcrv
// control-points: UT3D_ptvcpar_ipt_obj
/// \endcode

// TODO: extract surfaces into new function (sur needs upar,vpar, dType (normal|along-u|along-v)
//       (this function only for linear objects (curves))
// TODO: get start/endPoints out of PRCV

// see also UTO_parLim_get_cv

  int     irc, i1, i2, oTyp, imod;
  char    cv1[OBJ_SIZ_MAX];
  double  db1, d1;
  Point   pt1;
  Vector  vc1;
  Point2  pt21;
  void    *vp1;


  // printf("UT3D_ptvcpar_std_obj vTyp=%d pType=%d cvTyp=%d\n",vTyp,pType,cvTyp);
  // DEB_dump_obj__  (cvTyp, cvDat, " ptvcpar_std_obj");



  irc = 0;


/*
    if(cvTyp == Typ_ObjGX)
      cvTyp = UTO_objDat_ox (&cvDat, &i1, cvDat);
*/

  switch (cvTyp) {


    //----------------------------------------------------------------
    case Typ_PT2:
      if(pto) *pto = UT3D_pt_pt2 ((Point2*)cvDat);
      irc = 1;
      break;


    //----------------------------------------------------------------
    case Typ_PT:
      if(pto) *pto = *((Point*)cvDat);
      irc = 1;
      break;


    //----------------------------------------------------------------
    case Typ_LN:
      if(vco) UT3D_vc_ln (vco, (Line*)cvDat);
      irc = 3;

      if(pType <= Ptyp_start) { // startpoint
        if(pto) *pto = ((Line*)cvDat)->p1;
        if(par) *par = 0.;
        break;

      } else if(pType == Ptyp_end) { // endpoint
        if(pto) *pto = ((Line*)cvDat)->p2;
        if(par) *par = 1.;
        break;

      } else if(pType == Ptyp_mid) { // midpoint
        if(pto) UT3D_pt_mid2pt (pto, &((Line*)cvDat)->p1, &((Line*)cvDat)->p2);
        if(par) *par = 0.5;
        break;

      }
      goto L_err_FNI;




    //----------------------------------------------------------------
    case Typ_CI:           // set points and tangents
      irc = 6;
      if(pType == Ptyp_start) { // startpoint
        pt1 = ((Circ*)cvDat)->p1;
        if(par) *par = 0.;
        goto L_CI2;

      } else if(pType == Ptyp_end) { // endpoint
        pt1 = ((Circ*)cvDat)->p2;
        if(par) *par = 1.;
        goto L_CI2;

      } else if(pType == Ptyp_mid) { // midpoint / 180-deg-point
        UT3D_pt_mid_ci (&pt1, (Circ*)cvDat);
        if(par) *par = 0.5;
        goto L_CI2;

      } else if((pType == Ptyp_def) ||
                (pType == Ptyp_cen)) { // centerpoint
        if(pto) *pto = ((Circ*)cvDat)->pc;
        break;

      } else if(pType == Ptyp_90_deg) { // 90-deg-point
        imod = 1;
        if(par) *par = 0.25;
        goto L_CI1;

      } else if(pType == Ptyp_270_deg) { // 270-deg-point
        // imod: 1=90;2=180;3=270;
        imod = 3;
        if(par) *par = 0.75;
        goto L_CI1;

      } else goto L_err_FNI;

      L_CI1:
        UT3D_pt_std_ci (&pt1, imod, (Circ*)cvDat);

      L_CI2:
        if(pto) *pto = pt1;
        if(vco) UT3D_vc_tng_ci_pt (vco, &pt1, (Circ*)cvDat);

        break;


    //----------------------------------------------------------------
    // Typ_Note
    case Typ_GTXT:
      // DEB_dump_obj__ (Typ_GTXT, obj, "GText:");
      if(pto) *pto = (((GText*)cvDat)->pt);
      irc = 1;
      break;


    //----------------------------------------------------------------
    case Typ_ATXT:
      // DEB_dump_obj__ (Typ_GTXT, obj, "GText:");
      if(pto) *pto = (((AText*)cvDat)->p1);
      irc = 1;
      break;


    //----------------------------------------------------------------
    // Typ_Model
    case Typ_Model:
      if(pto) *pto = (((ModelRef*)cvDat)->po);
      irc = 1;
      break;


    //----------------------------------------------------------------
    // Curves:
    case Typ_CVPOL:

      if((pType == Ptyp_def) ||
         (pType == Ptyp_start)) { // startpoint
        db1 = ((CurvPoly*)cvDat)->v0;

      } else if(pType == Ptyp_end) { // endpoint
        db1 = ((CurvPoly*)cvDat)->v1;

      } else if(pType == Ptyp_mid) { // midpoint
        db1 = (((CurvPoly*)cvDat)->v0 + ((CurvPoly*)cvDat)->v1) / 2.;

      } else if(pType < Ptyp_start) { 
        i1 = pType * -1 - 1;  // make index from MOD; first point = -1
          // printf(" pType=%d i1=%d\n",pType,i1);
        if(pto) *pto = ((CurvPoly*)cvDat)->cpTab[i1];
        db1 = ((CurvPoly*)cvDat)->lvTab[i1];
        goto L_POL1;
 
      } else goto L_err_FNI;


      if(pto) UT3D_pt_evalplg (pto, (CurvPoly*)cvDat, db1);

      L_POL1:
      if(vco) UT3D_vc_evalplg (vco, (CurvPoly*)cvDat, db1);
      // if(par) *par = db1;
      if(par) {
        if(vTyp) {
          // change len-offset -> parameter_0-1
           *par = UT3D_par1_parplg (&db1, (CurvPoly*)cvDat);
        } else {
           *par = db1;
        }
      }
      irc = 3;
      break;
  

    //----------------------------------------------------------------
    case Typ_CVELL:
      irc = 8;
      if(pType == Ptyp_start) { // startpoint
        pt1 = ((CurvElli*)cvDat)->p1;
        if(par) *par = 0.;
        goto L_ELL2;

      } else if(pType == Ptyp_end) { // endpoint
        pt1 = ((CurvElli*)cvDat)->p2;
        if(par) *par = 1.;
        goto L_ELL2;

      } else if(pType == Ptyp_mid) { // midpoint
        // if 360-deg: pt = pc + (rev)va; vc = (rev)vb;
        if(!UT3D_ck_el360((CurvElli*)cvDat)) {
          if(pto) {
            UT3D_vc_invert (&vc1, &((CurvElli*)cvDat)->va);
            UT3D_pt_traptvc (pto, &((CurvElli*)cvDat)->pc, &vc1);
          }
          if(vco) UT3D_vc_invert (vco, &((CurvElli*)cvDat)->vb);
        } else {
          UT3D_ptvc_eval_ell_par (pto, vco, (CurvElli*)cvDat, 0.5);
        }
        if(par) *par = 0.5;
        break;

      } else if((pType == Ptyp_def) ||
                (pType == Ptyp_cen)) { // centerpoint
        if(pto) *pto = ((CurvElli*)cvDat)->pc;
        break;

      } else if(pType == Ptyp_90_deg) { // 90-deg-point
        // pc + vb
        if(pto) UT3D_pt_traptvc (pto, &((CurvElli*)cvDat)->pc,
                                      &((CurvElli*)cvDat)->vb);
        if(vco) UT3D_vc_invert (vco, &((CurvElli*)cvDat)->va);
        // if(par) *par = 0.?;
        break;

      } else if(pType == Ptyp_270_deg) { // 270-deg-point
        // pc + (rev)vb
        if(pto) {
          UT3D_vc_invert (&vc1, &((CurvElli*)cvDat)->vb);
          UT3D_pt_traptvc (pto, &((CurvElli*)cvDat)->pc, &vc1);
        }
        if(vco) *vco = ((CurvElli*)cvDat)->va;
        // if(par) *par = 0.?;
        break;

      } else if(pType == Ptyp_focus1) { // focus 1
        UT3D_pt_elfoc (pto, NULL, (CurvElli*)cvDat);
        break;

      } else if(pType == Ptyp_focus2) { // focus 2
        UT3D_pt_elfoc (NULL, pto, (CurvElli*)cvDat);
        break;


      } else goto L_err_FNI;

      L_ELL1:
        // UT3D_pt_std_ci (&pt1, imod, (Circ*)cvDat);
      L_ELL2:
        if(pto) *pto = pt1;
        if(vco) {
          if(!UT3D_ck_el360((CurvElli*)cvDat)) {
            *vco = ((CurvElli*)cvDat)->vb;
          } else {
            UT3D_vc__pt_el (vco, &pt1, (CurvElli*)cvDat);
          }
        }
      break;


    //----------------------------------------------------------------
    case Typ_CVCLOT:
      if((pType == Ptyp_def) ||
         (pType == Ptyp_start)) { // startpoint
        if(pto) *pto = ((CurvClot*)cvDat)->stp;
        if(vco) *vco = ((CurvClot*)cvDat)->stv;
        if(par) *par = 0.;

      } else if(pType == Ptyp_end) { // endpoint
        if((pto) || (vco))
          UT3D_ptvc_evparclot (pto, vco, 1, (CurvClot*)cvDat, 1.);
        if(par) *par = 1.;

      } else if(pType == Ptyp_mid) { // midpoint
        if((pto) || (vco))
          UT3D_ptvc_evparclot (pto, vco, 1, (CurvClot*)cvDat, 0.5);
        if(par) *par = 0.5;
// vTyp - UT3D_park__par1_clot ??

      } else goto L_err_FNI;

      irc = 3;
      break;


    //----------------------------------------------------------------
    case Typ_CVBSP:
      if((pType == Ptyp_def) ||
         (pType == Ptyp_start)) { // startpoint
        db1 = ((CurvBSpl*)cvDat)->v0;

// Ptyp_start_abs
//         i1 = pType * -1 - 1;  // make index from MOD; first point = -1
//         // db1 = ((CurvBSpl*)cvDat)->kvTab[i1];
//           // printf(" pType=%d i1=%d\n",pType,i1);
//         if(pto) *pto = ((CurvBSpl*)cvDat)->cpTab[i1];

      } else if(pType == Ptyp_end) { // endpoint
        db1 = ((CurvBSpl*)cvDat)->v1;

      } else if(pType == Ptyp_mid) { // midpoint
        db1 = (((CurvBSpl*)cvDat)->v0 + ((CurvBSpl*)cvDat)->v1) / 2.;

      } else goto L_err_FNI;
        // printf(" _std_obj-L_BSP0: db1=%f\n",db1);

      if(pto) UT3D_pt_evalparCv (pto, (CurvBSpl*)cvDat, db1);
      if(vco) UT3D_vc_evalparCv (vco, (CurvBSpl*)cvDat, db1);
      if(par) {
        if(vTyp) {
          // change knotval -> parameter_0-1
           *par = UT3D_par1_parbsp (&db1, (CurvBSpl*)cvDat);
        } else {
           *par = db1;
        }
      }
      irc = 3;
      break;


    //----------------------------------------------------------------
    case Typ_CVRBSP:
      if(pType == Ptyp_start) { // startpoint
        db1 = ((CurvRBSpl*)cvDat)->v0;

      } else if(pType == Ptyp_end) { // endpoint
        db1 = ((CurvRBSpl*)cvDat)->v1;

      } else if(pType == Ptyp_mid) { // midpoint
        db1 = (((CurvRBSpl*)cvDat)->v0 + ((CurvRBSpl*)cvDat)->v1) / 2.;

      } else goto L_err_FNI;

      if(pto) UT3D_pt_evparCrvRBSpl (pto, &d1, (CurvRBSpl*)cvDat, db1);
      if(vco) goto L_err_FNI;
        // make a new UT3D_vc_eval_par_rbspl from UT3D_rbspl_tst_tg
      if(par) {
        if(vTyp) {
          // change len-offset -> parameter_0-1
           *par = UT3D_par1_par_rbsp (&db1, (CurvRBSpl*)cvDat);
        } else {
           *par = db1;
        }
      }
      irc = 3;
      break;


    //----------------------------------------------------------------
    case Typ_ObjGX:
      i1 = UTO_objDat_ox (&vp1, &i2, cvDat);
      if(i1 < 0) return -1;
      if(i2 > 1) TX_Print("**** UT3D_ptvcpar_std_obj I001");

      // recursion
      irc = UT3D_ptvcpar_std_obj (pto, vco, par, vTyp, pType, i1, vp1);
      break;


    //----------------------------------------------------------------
    case Typ_CVTRM:
      // Typ_CVTRM can have form=Typ_ObjGX (contour) 
      //   or form=Typ_CVTRM(single trimmed curve)
      // get standardCurve for trimmed-curve
      UTO_cv_cvtrm (&i1, (void*)cv1, NULL, (CurvCCV*)cvDat);
// TODO: spline, clot has v0-point, v1-point in CurvCCV-struct (io0, ip1) !?
      // recursion
      irc = UT3D_ptvcpar_std_obj (pto, vco, par, vTyp, pType, i1, cv1);
      break;


    //----------------------------------------------------------------
    case Typ_PLN:
      if(pto) *pto = ((Plane*)cvDat)->po;
      break;



    //----------------------------------------------------------------
    default:
      goto L_err_FNI;
  }


    // TESTBLOCK
    // printf(" ex-UT3D_ptvcpar_std_obj-irc =%d\n",irc);
    // if(irc >= 0) {
      // if(par) printf("ex UT3D_ptvcpar_std_obj par=%lf\n",*par);
      // if(pto) DEB_dump_obj__ (Typ_PT, pto, "ex ptvcpar_std_obj");
      // if(pto) GR_tDyn_symB__ (pto, SYM_STAR_S, ATT_COL_RED);
      // if(vco) GR_tDyn_vc__ (vco, pto, 9, 1);
    // }
    // END TESTBLOCK


  return irc;


  //----------------------------------------------------------------
  L_err_FNI:  // function not implemented
    DEB_dump_obj__ (cvTyp, cvDat, " *** ERR UT3D_ptvcpar_std_obj");
    return MSG_ERR__ (ERR_func_not_impl, "/ pTyp %d cvTyp %d", pType, cvTyp);

}



//========================================================================
  int UT3D_2pt_oppptvclen (Point *p1, Point *p2,
                           Point *pc, Vector *vc1, double dist) {
//========================================================================
/// \code
/// UT3D_2pt_oppptvclen       2 opposite points (center, vector, dist)
/// pc is center, p1 is direction vc1 from pc, p2 in reverse direction.
/// \endcode


  Vector    vc2;


  UT3D_vc_setLength (&vc2, vc1, dist);


  p1->x = pc->x + vc2.dx;
  p1->y = pc->y + vc2.dy;
  p1->z = pc->z + vc2.dz;

  p2->x = pc->x - vc2.dx;
  p2->y = pc->y - vc2.dy;
  p2->z = pc->z - vc2.dz;


  return 0;

}


//========================================================================
  int UT3D_ipt_ckfar_npt (Point *p0, Point *ptTab, int ptNr) {
//========================================================================
/// UT3D_ipt_ckfar_npt         return index of farest Point from n points

// OFFEN: kan man hier mit UT3D_lenq_PtPt beschleunigen ?

// use: pt1 = ptTab[UT3D_ipt_ckfar_npt (&ptStart, ptTab, ptNr)];



  int     i1, iind;
  double  d1, lmax;


  // printf("UT3D_ipt_ckfar_npt %f %f %f\n",p0->x,p0->y,p0->z);

  if(ptNr < 2) return 0;

  lmax = UT_VAL_MIN;

  for(i1=0; i1<ptNr; ++i1) {
    // printf(" near[%d] %f %f %f\n",i1,ptTab[i1].x,ptTab[i1].y,ptTab[i1].z);
    d1 = UT3D_len_2pt (p0, &ptTab[i1]);
    if(d1 > lmax) {
      iind = i1;
      lmax = d1;
    }
  }

  // GR_tDyn_symB__ (&ptTab[iind], SYM_TRI_S, 2);
  return iind;

}


//========================================================================
  int UT3D_ipt_cknear_3pt (Point *p0, Point *p1, Point *p2) {
//========================================================================
/// \code
/// UT3D_ipt_cknear_3pt        return index of nearest Point from 3 points
/// RetCod 0:  p0-p1 is nearest
///        1:  p0-p2 is nearest
/// \endcode

  int     i1, iind;
  double  d1, d2;


  // printf("UT3D_ipt_cknear_3pt %d %f %f %f\n",ptNr,p0->x,p0->y,p0->z);

  d1 = UT3D_lenq_PtPt (p0, p1);
  d2 = UT3D_lenq_PtPt (p0, p2);

  if(d1 < d2) i1 = 0;
  else        i1 = 1;

  return i1;

}


//========================================================================
  int UT3D_ipt_cknear_npt (Point *p0, Point *ptTab, int ptNr) {
//========================================================================
/// UT3D_ipt_cknear_npt        return index of nearest Point from n points

// use: pt1 = ptTab[UT3D_ipt_cknear_npt (&ptStart, ptTab, ptNr)];



  int     i1, iind;
	double  d1, lmax;


  // printf("UT3D_ipt_cknear_npt %d %f %f %f\n",ptNr,p0->x,p0->y,p0->z);
  // for(i1=0;i1<ptNr;++i1) DEB_dump_obj__ (Typ_PT,&ptTab[i1],"%d  ",i1);



  iind = 0;                    // wenn p0-Entfernung > UT_VAL_MAX erforderl
  if(ptNr < 2) return iind;

  lmax = UT_VAL_MAX;

  for(i1=0; i1<ptNr; ++i1) {
    // printf(" near[%d] %f %f %f\n",i1,ptTab[i1].x,ptTab[i1].y,ptTab[i1].z);
    // d1 = UT3D_len_2pt (p0, &ptTab[i1]);
    d1 = UT3D_lenq_PtPt (p0, &ptTab[i1]);
    if(d1 < lmax) {
      iind = i1;
      lmax = d1;
    }
  }

    // printf("ex UT3D_ipt_cknear_npt %d\n",iind);

  return iind;

}


//========================================================================
  int UT3D_ipt_cknearn_npt (Point *p0, Point *ptTab, int ptNr, int distNr) {
//========================================================================
/// \code
/// UT3D_ipt_cknearn_npt        return index of n-nearest Point from n points
/// distNr=0: return index of nearest point;
/// distNr=1: return index of 2.nearest point ...
/// \endcode

// use: pt1 = ptTab[UT3D_ipt_cknearn_npt (&ptStart, ptTab, ptNr, imod)];



  int     i1, i2, iind;
  double  lmax, dTab[100];


  // printf("UT3D_ipt_cknearn_npt %f %f %f %d %d\n",p0->x,p0->y,p0->z,ptNr,distNr);



  if(ptNr < 2) return 0;

  if(ptNr > 100) { TX_Error("UT3D_ipt_cknearn_npt EOM"); return 0; }

  if(distNr >= ptNr) distNr = ptNr - 1;


  for(i1=0; i1<ptNr; ++i1) {
    // dTab[i1] = UT3D_len_2pt (p0, &ptTab[i1]);
    dTab[i1] = UT3D_lenq_PtPt (p0, &ptTab[i1]);
    // printf(" dist[%d]=%f\n",i1,dTab[i1]);
  }


  ++distNr;
  iind = 0;
  for(i2=0; i2<distNr; ++i2) {
    lmax = UT_VAL_MAX;
    for(i1=0; i1<ptNr; ++i1) {
      // printf(" i1=%d d=%f\n",i1,dTab[i1]);
      if(dTab[i1] < lmax) {
        iind = i1;
        lmax = dTab[i1];
      }
    }
    dTab[iind] = UT_VAL_MAX + 1.;
    // printf(" i2=%d iind=%d d=%f\n",i2,iind,dTab[iind]);
  }

  // printf("ex UT3D_ipt_cknearn_npt %d\n",iind);
  return iind;

}


//================================================================
  int UT3D_ipt2_npt (int *ip1, int *ip2, Point *pta, int pNr) {
//================================================================
/// find the indices of the most extreme points ..
// ganz doof jeden mit jedem ..

  int     i1, i2;
  double  d1, d2;


  printf("UT3D_ipt2_npt %d\n",pNr);

  d1 = 0.;
  for(i1=0; i1<pNr; ++i1) {
    for(i2=0; i2<pNr; ++i2) {
      d2 = UT3D_lenq_PtPt (&pta[i1], &pta[i2]);
      if(d2 > d1) {
        d1 = d2;
        *ip1 = i1;
        *ip2 = i2;
      }
    }
  }

    printf("ex UT3D_ipt2_npt %d %d\n",*ip1,*ip2);
    // GR_tDyn_symB__ (&pta[*ip1], SYM_TRI_S, 5);
    // GR_tDyn_symB__ (&pta[*ip2], SYM_TRI_S, 5);

  return 0;

}


//===========================================================================
  int UT3D_ipt2_nptvc (Point *pt1, Point *pt2, int mode,
                       Point *pta, int iNr, Vector *vc) {
//===========================================================================
/// \code
/// ind. of outermost points in direction vc of n pt's
/// mode   0 = start, >0 = resume
/// 
/// see UT3D_slen_2ptvc UT3D_slen_ptpl
/// \endcode


  int    i1, iMin, iMax;
  double lvc, skp, dMin, dMax;


  iMin = -1;
  iMax = -1;

  if(mode == 0) {
    dMin = UT_VAL_MAX;
    dMax = UT_VAL_MIN;
  } else {
    dMin = vc->dx*pt1->x + vc->dy*pt1->y + vc->dz*pt1->z;
    dMax = vc->dx*pt2->x + vc->dy*pt2->y + vc->dz*pt2->z;
  }
    // printf(" dMin=%f dMax=%f\n",dMin,dMax);

  for(i1=0; i1<iNr; ++i1) {

    // vc2 = vektor vom absoluten Nullpunkt bis pta[i1]
    // skp = (vc->dx*vc2.dx + vc->dy*vc2.dy + vc->dz*vc2.dz) / lvc;
    skp = vc->dx*pta[i1].x + vc->dy*pta[i1].y + vc->dz*pta[i1].z;

    if(skp > dMax) {
      dMax = skp;
      iMax = i1;
    }
    if(skp < dMin) {
      dMin = skp;
      iMin = i1;
    }
      // printf(" _ipt2_nptvc skp=%f iMin=%d iMax=%d\n",skp,iMin,iMax);
  }

  if(iMin >= 0) *pt1 = pta[iMin];
  if(iMax >= 0) *pt2 = pta[iMax];

    // nur Test:
    // lvc = Laenge Normalvec d. plane
    // lvc = sqrt(vc->dx*vc->dx + vc->dy*vc->dy + vc->dz*vc->dz);
    // printf(" ex _ipt2_nptvc %d (%f) %d (%f) mode=%d iNr=%d\n",iMin,
            // dMin/lvc,iMax,dMax/lvc,mode,iNr);

  return 0;

}


//=======================================================================
  int UT3D_pt_ck_perp_2pt (Point *p1, Point *p2, Point *p3, double tol) {
//=======================================================================
/// \code
/// UT3D_pt_ck_perp_2pt              check if 3 points are normal (perpendic.)
///
///                p3
///                 X
///                /       
///               / |
///              /        
///             /   |
///            /          
///           X-----|------------>X
///         p1                   p2
/// RetCodes:
///  0 = p1-p3 is NOT normal to p1-p2
///  1 = p1-p3 is normal to p1-p2
///
/// see also UT3D_vc_ck_perpvc
/// \endcode


  Vector    v12, v13;

  UT3D_vc_2pt (&v12, p1, p2);
  UT3D_vc_2pt (&v13, p1, p3);

  return UT3D_vc_ck_perpvc (&v12, &v13, tol);

}


//=======================================================================
  int UT3D_pt_ck_mid_2pt (Point *p1, Point *p2, Point *p3, double *tol) {
//=======================================================================
/// \code
/// check if point is on perp.plane between 2 points
///
///               x p3
///               |
///    p1 x-------|--------x p2
///
/// RetCod
///   0       not OK; p3 has different distances from p1 and p2
///   1       OK; p3 has the same distance from p1 and p2
/// \endcode

// - compute parameter of the distance of p3 on p1-p2
// - create px (p3 projected onto p1-p2)
// - test distance px - midpoint p1-p2
//

  double   d1;
  Vector   v1, v2;
  Point    pm, px;

  // printf("UT3D_pt_ck_mid_2pt %f\n",*tol);
    // DEB_dump_obj__ (Typ_PT, p1, "  p1:");
    // DEB_dump_obj__ (Typ_PT, p2, "  p2:");
    // DEB_dump_obj__ (Typ_PT, p3, "  p3:");


  UT3D_vc_2pt (&v1, p1, p3);         // v1 = p1-p3
  UT3D_vc_2pt (&v2, p1, p2);         // v2 = p1-p2

  UT3D_parvc_2vc (&d1, &v1, &v2);    // d1=parametric dist of p3 on p1-p2

  UT3D_pt_mid2pt (&pm, p1, p2);      // pm = midpoint p1-p2

  UT3D_pt_traptmultvc (&px, p1, &v2, d1); // px = p3 projected onto p1-p2

  return UT3D_comp2pt (&pm, &px, *tol);

}


//========================================================================
  int UT3D_pt_ck_on_pta (Point *pTab, int ptNr, Point *pt1, double tol) {
//========================================================================
/// \code
/// testen, ob pt1 AUF Polygon liegt
/// RetCod  0 pt1 is on polygon ptab
/// RetCod -1 pt1 is not on polygon ptab
/// \endcode

  int   irc, i1;

  // DEB_dump_obj__ (Typ_PT, pt1, "UT3D_pt_ck_on_pta %d ",ptNr);


  for(i1=1; i1<ptNr; ++i1) {
    irc = UT3D_pt_ck_onLine (&pTab[i1-1], &pTab[i1], pt1, tol);
    if(irc >= 0) {
      // DEB_dump_obj__ (Typ_PT, &pTab[i1-1], "ex _ck_on_pta %d ",irc);
      // DEB_dump_obj__ (Typ_PT, &pTab[i1], "                 ");
      return 0;
    }
  }


  // printf("ex _ck_on_pta -1\n");

  return -1;

}


//================================================================
   int UT3D_pt_ck_in2pt (Point *p1,Point *p2,Point *p3, double tol) {
//================================================================
/// \code
/// check if point p3 is inside box of points p1-p2
/// Returncodes:
///  -1 = the point lies outside the box
///   0 = the point lies inside the segment
/// \endcode



  Point   pb1, pb2;


  // check if point is in box oder voellig ausserhalb ..
  UT3D_box_2pt__ (&pb1, &pb2, p1, p2);

  // test if p3 lies inside Box
  if(UT3D_pt_ck_inBoxTol (&pb1, &pb2, p3, tol) == 1) return -1; // outside


  return 0;

}


//======================================================================
  int UT3D_pt_ck_inCirc (Circ *ci1, Point *pt1, double tol) {
//======================================================================
/// \code
/// check if point is on circSegment
/// ACHTUNG: Radius und ob in gleicher Plane wird nicht mehr getestet !
///          Es wird nur geprueft ob zwischen Start- und Endpunkt.
/// RC = -1   NO; Point outside CircSeg p1-p2
/// RC =  0   Yes; p3=p1; po=p1.
/// RC =  1   Yes; po between p1-p2
/// RC =  2   Yes; p3=p2; po=p2.
/// \endcode

/*
neue variante testen: testen ob pt1 ueber oder unter der Linie ci.p1-ci.p2
liegt. ohne acos.
  // vector from p1 -> p2; test if px is above or below (between).
  if(dir = CCW) {
    vc12 = vector(p1 -> p2)
  } else {   // dir =CW
    vc12 = vector(p2 -> p1)
  }
  if(angC < 180) {
    if(px = right_of_vc12) then onCirc = True
    if(ps = on-vc12) then
      if(px = between-p1-p2) then  onCirc = True
      else onCirc = False
    endif
  } else {  // angC > 180
    if(px = left_of_vc12) then onCirc = True
    if(ps = on-vc12) then
      if(px = between-p1-p2) then onCirc = False
      else onCirc = True
    endif
  }
*/



  int      irc;
  double   ap;


  // DEB_dump_obj__ (Typ_PT, pt1, "UT3D_pt_ck_inCirc ");



  // check if point is near CircStartPoint
  if(UT3D_comp2pt (pt1, &ci1->p1, tol) == 1) { irc = 0; goto L_fertig; }


  // check if point is near CircEndPoint
  if(UT3D_comp2pt (pt1, &ci1->p2, tol) == 1) { irc = 2; goto L_fertig; }


  // check point on CircSegment
  irc = 1;

  ap = UT3D_angr_ci_p1_pt (ci1, pt1);
  // printf(" ap=%f\n",ap);

  if(ci1->ango > 0.) {  // CCW-Kreis
    if(ap > ci1->ango) irc = -1;

  } else {              // CW-Kreis
    if(ap < ci1->ango) irc = -1;
  }


  L_fertig:
  // printf("ex UT3D_pt_ck_inCirc irc=%d\n",irc);
  return irc;

}


//====================================================================
  void UT3D_pt_opp2pt (Point *po, Point *p1, Point *p2) {
//====================================================================
/// \code
/// UT3D_pt_opp2pt                  opposite point (p1 = center)
/// p1 may be == po
/// p2 may be == po
/// \endcode


  Vector vc1;

  UT3D_vc_2pt (&vc1, p1, p2);

  po->x = p1->x - vc1.dx;
  po->y = p1->y - vc1.dy;
  po->z = p1->z - vc1.dz;

  /*
  printf("UT3D_opposPoint %f %f %f\n",po->x,po->y,po->z);
  printf("               %f %f %f\n",p1->x,p1->y,p1->z);
  printf("               %f %f %f\n",p2->x,p2->y,p2->z);
  */

  return;
}


//=====================================================================
  int UT3D_pt_oppptptvc (Point *po, Point *pi, Point *pl, Vector *vl) {
//=====================================================================
/// UT3D_pt_oppptptvc            point opposite line (= 180 deg rot.)


  double  d1;
  Point   px;


  // px = project point pi -> line pl/vl  (UT3D_pt_projptptvc)
  UT3D_pt_projptptvc (&px, &d1, NULL, pi, pl, vl);
    // GR_Disp_pt(&px, SYM_SQU_B, 2);

  // po = pi opposite px
  po->x = px.x - (pi->x - px.x);
  po->y = px.y - (pi->y - px.y);
  po->z = px.z - (pi->z - px.z);



  return 0;

}


/*
//================================================================
  void UT3D_pt_traptvc (Point *po, Point *pi, Vector *vc) {
//================================================================
// UT3D_pt_traptvc            Point = Point + Vector
// pi und po darf ident sein.


  po->x = pi->x + vc->dx;
  po->y = pi->y + vc->dy;
  po->z = pi->z + vc->dz;

  printf("UT3D_pt_vc %f %f %f\n",po->x,po->y,po->z);
  // printf("           %f %f %f\n",pi->x,pi->y,pi->z);
  // printf("          %f %f %f\n",vc->dx,vc->dy,vc->dz);
  // printf("          %f %f %f\n",po->x,po->y,po->z);


  return;
}
*/


//=======================================================================
  void UT3D_pt_tra_pt_vc_par (Point *po,Point *pi,Vector *vc,double dist) {
///====.==================================================================
/// \code
/// UT3D_pt_tra_pt_vc_par          transl. point into dir vc dist.lenv (vcLen=1)
/// Vector has Length 1.; Point = basePoint + (vec * len)
/// \endcode


  po->x = pi->x + (vc->dx * dist);
  po->y = pi->y + (vc->dy * dist);
  po->z = pi->z + (vc->dz * dist);


}


//=========================================================================
  void UT3D_pt_traptvclen (Point *po, Point *pi, Vector *vc, double lenv) {
//=========================================================================
/// \code
/// UT3D_pt_ptvclen          transl. point into dir vc dist. lenv
/// Move point into direction vc with distance dx.
/// po and pi may be identical.
/// \endcode


  double    l1;


  l1 = lenv / UT3D_len_vc (vc);
  po->x = pi->x + vc->dx * l1;
  po->y = pi->y + vc->dy * l1;
  po->z = pi->z + vc->dz * l1;

  /*
  printf("UT3D_pt_traptvclen %f %f %f\n",po->x,po->y,po->z);
  printf("                %f %f %f\n",pi->x,pi->y,pi->z);
  printf("                %f %f %f %f\n",vc->dx,vc->dy,vc->dz,lenv);
  */

}



//========================================================================
  void UT3D_pt_trapt2vc (Point *po,Point *pi,Vector *vc1, Vector *vc2) {
//========================================================================
/// \code
/// UT3D_pt_trapt2vc        transl. point into 2 directions
/// po kann mit pi ident sein.
/// \endcode


  po->x = pi->x + vc1->dx + vc2->dx;
  po->y = pi->y + vc1->dy + vc2->dy;
  po->z = pi->z + vc1->dz + vc2->dz;

}


//============================================================================
  void UT3D_pt_tra_pt_2vc_par (Point *po, Point *pi,
                               Vector *vc1, double f1, Vector *vc2, double f2) {
//============================================================================
/// \code
/// UT3D_pt_trapt2vc2len        transl. point into 2 directions (multiply vec)
/// \endcode


  // printf("UT3D_pt_tra_pt_2vc_par f1=%f f2=%f\n",f1,f2);


  po->x = pi->x + vc1->dx * f1 + vc2->dx * f2;
  po->y = pi->y + vc1->dy * f1 + vc2->dy * f2;
  po->z = pi->z + vc1->dz * f1 + vc2->dz * f2;


}


//========================================================================
  void UT3D_pt_tra_pt_2vc_2par (Point *po, Point *pi,
                                Vector *vc1, double l1,
                                Vector *vc2, double l2) {
//========================================================================
/// \code
/// UT3D_pt_tra_pt_2vc_2par      transl. point into 2 directions
/// distance = vc. * l. (real dist if vc has length=1)
/// po, pi can have same address.
/// retour: UT3D_2par_vc_vcx_vcy
/// \endcode


  // DEB_dump_obj__ (Typ_PT, pi, "UT3D_pt_tra_pt_2vc_2par: ");
  // DEB_dump_obj__ (Typ_VC, vc1, "    vc1:");
  // DEB_dump_obj__ (Typ_VC, vc2, "    vc2:");


  po->x = pi->x + (vc1->dx * l1) + (vc2->dx * l2);
  po->y = pi->y + (vc1->dy * l1) + (vc2->dy * l2);
  po->z = pi->z + (vc1->dz * l1) + (vc2->dz * l2);


  // printf("ex UT3D_pt_tra_pt_2vc_2par %lf %lf %lf\n",po->x,po->y,po->z);
  // printf("                %f %f %f\n",pi->x,pi->y,pi->z);
  // printf("                %f %f %f %f\n",vc->dx,vc->dy,vc->dz,lenv);


}


//========================================================================
  void UT3D_pt_trapt2vc2len (Point *po,Point *pi,Vector *vc1,double len1,
                                                 Vector *vc2,double len2) {
//========================================================================
/// \code
/// UT3D_pt_trapt2vc2len        transl. point into 2 directions (2 len's)
/// vc1, vc2 must not be normalized.
/// Move point into direction vc with distance dx.
/// po kann mit pi ident sein.
/// \endcode


  double    l1, l2;


  l1 = len1 / UT3D_len_vc (vc1);
  l2 = len2 / UT3D_len_vc (vc2);

  po->x = pi->x + vc1->dx * l1 + vc2->dx * l2;
  po->y = pi->y + vc1->dy * l1 + vc2->dy * l2;
  po->z = pi->z + vc1->dz * l1 + vc2->dz * l2;


  // printf("ex UT3D_pt_trapt2vc2len %f %f %f\n",po->x,po->y,po->z);
  // printf("                %f %f %f\n",pi->x,pi->y,pi->z);
  // printf("                %f %f %f %f\n",vc->dx,vc->dy,vc->dz,lenv);


}


//========================================================================
  void UT3D_pt_trapt3vc3len (Point *po,Point *pi,Vector *vx,double dx,
                                                 Vector *vy,double dy,
                                                 Vector *vz,double dz) {
//========================================================================
/// \code
/// UT3D_pt_trapt3vc3len        transl. point into 3 directions (3 len's)
/// Move point into direction vx,vy,vz with distance dx, dy dz.
/// vx, vy, vz must not be normalized.
/// po kann mit pi ident sein.
/// \endcode


  double    l1, l2, l3;


  l1 = dx / UT3D_len_vc (vx);
  l2 = dy / UT3D_len_vc (vy);
  l3 = dz / UT3D_len_vc (vz);

  po->x = pi->x + vx->dx * l1 + vy->dx * l2 + vz->dx * l3;
  po->y = pi->y + vx->dy * l1 + vy->dy * l2 + vz->dy * l3;
  po->z = pi->z + vx->dz * l1 + vy->dz * l2 + vz->dz * l3;


  // printf("ex UT3D_pt_trapt2vc2len %f %f %f\n",po->x,po->y,po->z);
  // printf("                %f %f %f\n",pi->x,pi->y,pi->z);
  // printf("                %f %f %f %f\n",vc->dx,vc->dy,vc->dz,lenv);


}


//=========================================================================
  void UT3D_pt_trapt2pt (Point *po, Point *pi, Point *p1, Point *p2) {
//=========================================================================
/// UT3D_pt_trapt2pt           transl. point distance p1 -> p2
  
  po->x = pi->x + p2->x - p1->x;
  po->y = pi->y + p2->y - p1->y;
  po->z = pi->z + p2->z - p1->z;


}


//========================================================================
  void UT3D_pt_traptptlen (Point *po,Point *pi,Point *pDir,double lenv) {
//========================================================================
/// \code
/// UT3D_pt_traptptlen        transl. point into dir pDir dist. lenv
/// Move point into direction pi->pDir with distance lenv.
/// po kann mit pi ident sein.
/// \endcode


  double    l1;
  Vector    vc;



  // printf("UT3D_pt_traptptlen %f,%f,%f l=%f\n",pi->x,pi->y,pi->z,lenv);


  if(fabs(lenv) < UT_TOL_min1) {
    *po = *pi;
    return;
  }


  vc.dx = pDir->x - pi->x;
  vc.dy = pDir->y - pi->y;
  vc.dz = pDir->z - pi->z;

  l1 = lenv / UT3D_len_vc (&vc);

  po->x = pi->x + vc.dx * l1;
  po->y = pi->y + vc.dy * l1;
  po->z = pi->z + vc.dz * l1;


  // printf("UT3D_pt_traptptlen %f %f %f\n",po->x,po->y,po->z);
  // printf("                %f %f %f\n",pi->x,pi->y,pi->z);
  // printf("                %f %f %f %f\n",vc.dx,vc.dy,vc.dz,lenv);
    // GR_tDyn_vc__ (&vc, pi, 8, 0);


}


//==============================================================================
  void UT3D_pt_tra_pt_pt_par (Point *pt3, Point *pt1, Point *pt2, double fakt) {
//==============================================================================
// UT3D_pt_tra_pt_pt_par     p3 = segment p1-p2 * fakt
// see UT2D_pt_tra_pt_pt_par


  pt3->x = pt1->x + (pt2->x - pt1->x) * fakt;
  pt3->y = pt1->y + (pt2->y - pt1->y) * fakt;
  pt3->z = pt1->z + (pt2->z - pt1->z) * fakt;

}
 

//======================================================================
  int UT3D_pt_tracirlen (Point *pto, Point *pti, Circ *cii, double clen) {
//======================================================================
/// \code
/// UT3D_pt_tracirlen         transl. point circular length
/// 
///   CCW = rad > 0 ;
///   CW  = rad < 0 ;
/// \endcode

  double   ao;


  // printf("UT3D_pt_tracirlen %f\n",clen);

  if(fabs(cii->rad) < UT_TOL_pt) {
    *pto = *pti;
    return -1;
  }


  //  Oeffnungswinkel
  ao = clen / cii->rad;
  // printf("  ao=%f\n",ao);


  UT3D_pt_rotptptvcangr(pto,pti,&cii->pc,&cii->vz,ao);


  return 0;

}


//===================================================================
  int UT3D_pt_rotciangr (Point *pto, double angr, Circ *ci1) {
//===================================================================
/// \code
/// UT3D_pt_rotciangr           get point on circle from opening-angle
/// Rotate startpoint of circle around its axis
/// Input:
///   angr         opening-angle to vector (ci1.pc - ci1.p1)
///
/// If angr=0.0     - the resulting point = ci1.p1;
/// If angr=pc.ango - the resulting point = ci1.p2;
/// \endcode

// see UT3D_ptvc_eval_ci_angr


  double dx, dy;
  Vector vcx, vcy, vcz;


  // printf("UT3D_pt_rotciangr %f\n",angr);


  // GR_tDyn_symB__ (&ci1->pc, SYM_TRI_S, 2); // TestDisp Kreiscenter
  // GR_tDyn_vc__ (&ci1->vz, &ci1->pc, 2, 0);  // TestDisp Kreis-Z-Vec
  // angr = UT_RADIANS(-270.);  // nur TEST


  dx = cos (angr);
  dy = sin (angr);

  // vcx = vektor pc - p1.
  UT3D_vc_2pt (&vcx, &ci1->pc, &ci1->p1);
  // UT3D_vc_setLength (&vcx, &vcx, 1.);
    // GR_tDyn_vc__ (&vcx, &ci1->pc, 2, 0);  // TestDisp Kreis-Z-Vec

  // normieren, sonst ist vcy nicht gleichlang wie vcx !
  UT3D_vc_setLength (&vcz, &ci1->vz, 1.);
  // es sollte genuegen, beide gleichlang zu machen ?
  
  // vcy = Normalvektor auf vcx/vcz errechnen (vcy = zeigt auf 90-Grad-Punkt).
  UT3D_vc_perp2vc (&vcy, &vcz, &vcx);
    // GR_tDyn_vc__ (&vcy, &ci1->pc, 2, 0);  // TestDisp Kreis-Z-Vec

  // nun vcx und vcy entspr dem Winkel veraendern = neuer Punkt
  UT3D_vc_multvc (&vcx, &vcx, dx);
  UT3D_vc_multvc (&vcy, &vcy, dy);

  // UT3D_pt_traptvc (pto, &ci1->pc, &vcy);
  UT3D_pt_trapt2vc (pto, &ci1->pc, &vcx, &vcy);

  // TestDisp pto
  // GR_tDyn_symB__ (pto, SYM_STAR_S, 2);

  return 0;

}


//================================================================
  int UT3D_pt_rotptm3 (Point *p2, Point *p1, Mat_4x3 ma) {
//================================================================
/// Rotate Point (prepare with UT3D_m3_inirot_angr)


  double x, y, z;

  x = p1->x - ma[0][3];
  y = p1->y - ma[1][3];
  z = p1->z - ma[2][3];


  /* printf("UT3D_pt_rotptm3    %f,%f,%f",x,y,z); */


  p2->x = ma[0][0]*x + ma[0][1]*y + ma[0][2]*z + ma[0][3];
  p2->y = ma[1][0]*x + ma[1][1]*y + ma[1][2]*z + ma[1][3];
  p2->z = ma[2][0]*x + ma[2][1]*y + ma[2][2]*z + ma[2][3];

  return 0;
}


//============================================================================
  int UT3D_pt_rotptptangr (Point *pto, Point *ptc, Point *pti, double *ar) {
//============================================================================
/// UT3D_pt_rotptptangr              rotate pti around ptc/Z-Axis


  double cos_a, sin_a, dx, dy;


  // printf("UT3D_pt_rotptptangr ar=%f\n",*ar);
  // DEB_dump_obj__ (Typ_PT, pti, "   pti ");
  // DEB_dump_obj__ (Typ_PT, ptc, "   ptc ");


  dx = pti->x - ptc->x;
  dy = pti->y - ptc->y;

  sin_a = sin(*ar);
  cos_a = cos(*ar);

  pto->x = ptc->x + (dx * cos_a - dy * sin_a);
  pto->y = ptc->y + (dx * sin_a + dy * cos_a);
  pto->z = pti->z;

    // DEB_dump_obj__ (Typ_PT, pto, "ex UT3D_pt_rotptptangr");

  return 0;

}


//=====================================================================
  int UT3D_pt_rot2ptpl2angr (Point *pto, Point *ptc, Point *ptx,
                             Plane *pl1, double *angZ, double *angY) {
//=====================================================================
/// \code
/// UT3D_pt_rot2ptpl2angr         rotate and tilt point around centerpoint
/// Input:
///   ptc         centerpoint
///   ptx         point to rotate
///   pl1         plane, rotate angZ around ptc/pl1->vz, angY = tilt-angle
///   angZ        rotate ptx <angZ> rad around ptc/pl1->vz
///   angY        then rotate new-ptx around ptc in plane (ptc/pl1->vz/new-ptx)
/// \endcode


  Vector    vcx1, vcz1;


  // pto = rot ptx around ptc-vc1 angle=*angZ
  if(fabs(*angZ) > UT_TOL_pt) {
    UT3D_pt_rotptptvcangr (pto, ptx, ptc, &pl1->vz, *angZ);
  } else {
    *pto = *ptx;
  }

  // tilt in plane vz - ptc-ptx
  if(fabs(*angY) > UT_TOL_pt) {
    // get vcx1 = vector ptc-pto
    UT3D_vc_2pt (&vcx1, ptc, pto);
    UT3D_vc_normalize (&vcx1, &vcx1);  // new x-axis
    // get vcz1 = new-z-axis from new-x-axis,old-z-axis
    UT3D_vc_perp2vc (&vcz1, &vcx1, &pl1->vz);
    UT3D_vc_normalize (&vcz1, &vcz1);
    UT3D_pt_rotptptvcangr (pto, pto, ptc, &vcz1, *angY);
  }

  return 0;

}


//===================================================================
  void UT3D_pt_rotptptvcangr (Point *pto,
                         Point *pti, Point *pta, Vector *vca, double angr) {
//===================================================================
/// \code
/// Punkt pti um die Achse pta/vca um angr weiterdrehen
/// angr = -2Pi bis +2Pi.
/// ACHTUNG: vca muss normiert sein !!
/// \endcode



  double dx, dy, rdc;
  Point  ptc;
  Vector vcx, vcy;


  // printf("UT3D_pt_rotptptvcangr %f\n",angr);
  // DEB_dump_obj__ (Typ_PT, pti, "  pti=");
  // DEB_dump_obj__ (Typ_PT, pta, "  pta=");
  // DEB_dump_obj__ (Typ_VC, vca, "  vca=");


  // GR_tDyn_symB__ (&ci1->pc, SYM_TRI_S, 2); // TestDisp Kreiscenter
  // GR_tDyn_vc__ (&ci1->vz, &ci1->pc, 2, 0);  // TestDisp Kreis-Z-Vec
  // angr = UT_RADIANS(-270.);  // nur TEST


  dx = cos (angr);
  dy = sin (angr);
  // printf(" dx=%f dy=%f\n",dx,dy);


  // ptc = project pti auf pta/vca
  UT3D_pt_projptptvc (&ptc, &rdc, NULL, pti, pta, vca);

  // vcx = vektor pc - p1.
  UT3D_vc_2pt (&vcx, &ptc, pti);
  // UT3D_vc_2pt (&vcx, pta, pti);

  // vcy = Normalvektor auf vcx/vcz errechnen (vcy = zeigt auf 90-Grad-Punkt).
  UT3D_vc_perp2vc (&vcy, vca, &vcx);
  // DEB_dump_obj__ (Typ_VC, &vcx, "vcx=");
  // DEB_dump_obj__ (Typ_VC, &vcy, "vcy=");

  // // vcx korrected
  // UT3D_vc_perp2vc (&vcx, &vcy, vca);

  // nun vcx und vcy entspr dem Winkel veraendern = neuer Punkt
  UT3D_vc_multvc (&vcx, &vcx, dx);
  UT3D_vc_multvc (&vcy, &vcy, dy);
  // DEB_dump_obj__ (Typ_VC, &vcx, "vcx=");
  // DEB_dump_obj__ (Typ_VC, &vcy, "vcy=");

  // UT3D_pt_trapt2vc (pto, pta, &vcx, &vcy);
  UT3D_pt_trapt2vc (pto, &ptc, &vcx, &vcy);

  // TestDisp pto
  // GR_tDyn_symB__ (pto, SYM_STAR_S, 2);

  // return 0;

}


/*
//==========================================================================
  void UT3D_pt_rotptptvcangr (Point *pto,
                              Point *pti,Point *ptc,Vector *vz,double angr) {
//==========================================================================
// Punkt pti um die Achse vz um angr weiterdrehen
// ACHTUNG: alte Version, besser m UT3D_pt_rotciangr
// pto und pti duerfen ident sein.


  double  rdc;
  Point   pt0, pt1, ptb;
  Vector  vx;
  Mat_4x3 m1;


  ptb = *pti;


  // printf("UT3D_pt_rotptptvcangr pti=%f,%f,%f\n",pti->x,pti->y,pti->z);
  // printf("                      ptc=%f,%f,%f\n",ptc->x,ptc->y,ptc->z);
  // printf("                      ang=%f\n",angr);



  if(fabs(angr) < UT_TOL_min1) {
    *pto = ptb;
    goto L_fertig;
  }


  // den Nullpt des Refsys durch Proj. von pti auf ptc/vz.
  UT3D_pt_projptptvc (&pt0, &rdc, NULL, &ptb, ptc, vz);
  // printf("  pt0=%f,%f,%f\n",pt0.x,pt0.y,pt0.z);
  // Radius rdc
  // rdc = UT3D_len_2pt (pti, &pt0);

  // printf("  rdc=%f\n",rdc);
  if(fabs(rdc) < UT_TOL_pt) {
    *pto = ptb;
    goto L_fertig;
  }


  // in 2D drehen, dann transformieren.
  pt1.x = rdc * cos(angr);
  pt1.y = rdc * sin(angr);
  pt1.z = 0.0;
  // printf(" 2D-pt1=%f,%f\n",pt1.x,pt1.y);


  // ein Refsys auf Origin=pt0, Z-Vec=vz, X-Vec=pt0-pti
  UT3D_vc_2pt (&vx, &pt0, &ptb);
  UT3D_m3_load_povxvz (m1, &pt0, &vx, vz);


  // pt1 transformieren
  UT3D_pt_tra_pt_m3 (pto, m1, &pt1);


  L_fertig:
  // printf("ex UT3D_pt_rotptptvcangr %f,%f,%f\n",pto->x,pto->y,pto->z);
  return;

}
*/


//======================================================================
  int UT3D_pt_projptln (Point *pp, double *ndist, double *par1,
                        Point *pt, Line *ln) {
//======================================================================
/// \code
/// UT3D_pt_projptln                        point = project point to line
/// Output:
///   ndist   normal distance pt - ln  or NULL 
///   par1    parameter for pp on ln or NULL
///   retcod  0  OK, but point outside limited line
///           1  pt is on line
///           2  pp == ln.p1
///           3  pp == ln.p2
///           -1 Inputerror (line too short)
/// \endcode

   int      irc;
   double   nlen, ppp;
   Vector   vl;


  // DEB_dump_obj__ (Typ_LN, ln, "UT3D_pt_projptln");
  // DEB_dump_obj__ (Typ_PT, pt, "              pt");


  /* change line into vector vl */
  UT3D_vc_ln (&vl, ln);


  irc = UT3D_pt_projptptvc (pp, &nlen, &ppp, pt, &ln->p1, &vl);
  if(irc < 0) goto L_exit;
    // printf(" nlen=%lf ppp=%lf\n",nlen,ppp);


  // compare pp-ln.p1
  if(UT3D_comp2pt(pp, &ln->p1, UT_TOL_pt)) {irc = 2; goto L_exit;}

  // compare pp-ln.p2
  if(UT3D_comp2pt(pp, &ln->p2, UT_TOL_pt)) {irc = 3; goto L_exit;}

  // test if pp is inside line
  if((ppp < 0.)||(ppp > 1.)) {irc = 0; goto L_exit;}
  irc = 1;


  L_exit:
  if(ndist) *ndist = nlen;
  if(par1) *par1 = ppp;

    // DEB_dump_obj__ (Typ_PT, pp, "ex UT3D_pt_projptln irc=%d",irc);
    // if(ndist) printf(" ndist=%lf\n",*ndist);
    // if(par1) printf(" par1=%lf\n",*par1);
  

  return irc;

}


//===========================================================================
  int UT3D_pt_projptci (Point *ptn, Point *ptf, Point *pt1, Circ *ci1) {
//===========================================================================
/// \code
/// UT3D_pt_projptci          point = project point to unlim. circ
/// Output:
///   ptn     point near on (unlim) circ
///   ptf     point far  on (unlim) circ; can be NULL
///   Retcod: 0=OK
///          -1=Inputerror (pt1 == CircCenter)
/// \endcode



  double  d1;
  Vector  vc1;
  Point   pt0;


  // DEB_dump_obj__ (Typ_PT, pt1, "UT3D_pt_projptci");
  // DEB_dump_obj__ (Typ_CI, ci1, "");


  // proj. pt -> cir-vz
  UT3D_pt_projptptvc (&pt0, &d1, NULL, pt1, &ci1->pc, &ci1->vz);
  // printf(" d1=%f\n",d1);
  // d1 is the distance on the axis to the CircCenter


  if(d1 < UT_TOL_pt) {
    // printf("Line Nr %d: Punkt ist im Kreismittelpunkt",APT_lNr);
    // TX_Print("Punkt ist im Kreismittelpunkt");
    *ptn = ci1->p1;
    return -1;
  }


  // go from circCen outside
  UT3D_vc_2pt (&vc1, &pt0, pt1);
  d1 = fabs(ci1->rad);
  UT3D_pt_traptvclen (ptn, &ci1->pc, &vc1, d1);

  if(ptf) UT3D_pt_opp2pt (ptf, &ci1->pc, ptn);


    // DEB_dump_obj__ (Typ_PT, ptn, "ex UT3D_pt_projptci ptn");
    // if(ptf) DEB_dump_obj__ (Typ_PT, ptf, "ex UT3D_pt_projptci ptf");

  return 0;
}


//===========================================================================
  int UT3D_pt_projpt2pt (Point *pp,double *len,Point *pt,Point *p1,Point *p2){
//===========================================================================
/// \code
/// UT3D_pt_projpt2pt         point = project point pt to line p1+p2
/// 
/// retcod 0=OK, 1=pt is on line, 2=Inputerror (vl too short)
/// 
/// see also UT3D_pt_ck_ptvc UT3D_pt_ck_onLine UT3D_nlen_2ptvc UT3D_pt_projptptvc
/// \endcode

  Vector vp;


  // change linstartpoint > ext.point into vector vp
  UT3D_vc_2pt (&vp, p1, p2);


  return UT3D_pt_projptptvc (pp, len, NULL, pt, p1, &vp);

}


//================================================================
  int UT3D_pt_projptptvc (Point *pp, double *len, double *par,
                          Point *pt, Point *pl, Vector *vl) {
//================================================================
/// \code
/// UT3D_pt_projptptvc                 point = project point pt onto line pl+vl
/// Output:
///    pp       point pt projected onto line pl+vl
///    len      length pp - pt
///    par      parameter of pp on vl; NULL for none.
///    retcod   0=OK,
///            -1=Inputerror (vl too short)
/// 
/// pl - vl gives a line, pt is projected onto this line.
/// 
///            X pt
///            |
///         len|
///            |
///      x-----x------->vl
///     pl     pp
/// 
/// 
/// E001 vl too short
/// 
/// see also UT3D_pt_ck_ptvc UT3D_pt_ck_onLine UT3D_nlen_2ptvc UT3D_pt_projptptvc
///   UT3D_pt_projptln 
/// \endcode


   double  sprod,lenl,dx,dy,dz;
   Vector  vp;


  // printf("UT3D_pt_projptptvc %f,%f,%f\n",pt->x,pt->y,pt->z);
  // printf("                pl=%f,%f,%f\n",pl->x,pl->y,pl->z);
  // printf("                vl=%f,%f,%f\n",vl->dx,vl->dy,vl->dz);


  // change linstartpoint > ext.point into vector vp
  UT3D_vc_2pt (&vp, pl, pt);

  // length of Line
  lenl = UT3D_len_vc (vl);

  // wenn lenl==0 dann pp=pt
  if(lenl < UT_TOL_min2) {
    // printf("UT3D_pt_projptptvc I001\n");
    *pp = *pt;
    *len = 0.;
    return -1;
  }

  // Project vp on vl. (dot-product)
  sprod = (vl->dx*vp.dx + vl->dy*vp.dy + vl->dz*vp.dz) / lenl;

  // printf(" %f %f\n",lenl,sprod);


  // compute prjPt on vector
  sprod /= lenl;
    // printf(" projptptvc %lf\n",sprod);
  if(par) *par = sprod;


  pp->x = pl->x + vl->dx * sprod;
  pp->y = pl->y + vl->dy * sprod;
  pp->z = pl->z + vl->dz * sprod;

  dx = pt->x - pp->x;
  dy = pt->y - pp->y;
  dz = pt->z - pp->z;
  *len = sqrt((dx*dx) + (dy*dy) + (dz*dz));

  // printf("ex UT3D_pt_projptptvc p=%f,%f,%f l=%f\n",pp->x,pp->y,pp->z,*len);

  return 0;
}


//========================================================================
  int UT3D_pt_projptptnvc (Point *po, Point *pi, Point *plo, Vector *plz) {
//========================================================================
/// UT3D_pt_projptptnvc  project point pt to plane from Origin, Z-vec

  double d1;
  Point  ps;
  Vector vci;


  // ips = Projection pi auf die Z-Achse der Plane plo/plz
  UT3D_pt_projptptvc (&ps, &d1, NULL, pi, plo, plz);


  // vci = Vektor vom ProjectionPosit ps --> pi
  UT3D_vc_2pt (&vci, &ps, pi);


  // ptOut = den PlaneOrigin plo um vci verschieben
  UT3D_pt_traptvc (po, plo, &vci);

    // DEB_dump_obj__(Typ_PT, po, "ex UT3D_pt_projptptnvc");


  return 0;

}


//======================================================================
  void UT3D_pt_projptpl (Point *pp, Plane *pl, Point *pt) {
//======================================================================
/// UT3D_pt_projptpl          point = project point to plane
 
// Man koennte auch den Punkt auf die Z-Achse projizieren und diese Strecke
// dann vom Nullpunkt wieder weg !

/// see UPRJ_app_pt

/*
  Vector   vln;

  // invert the plan-Z-vector
  UT3D_vc_invert (&vln, &pl->vz);
  UT3D_pt_intptvcpl_ (pp, pl, pt, &vln);
*/

  // intersect the plane and the point + the vector
  UT3D_pt_intptvcpl_ (pp, pl, pt, &pl->vz);


  /* printf("UT3D_pt_projptpl %f %f %f\n",pp->x,pp->y,pp->z); */

  return;

}


//=====================================================================
  int UT3D_pt_intplnln (Point *ip, double *dist, Plane *pl, Line *ln) {
//=====================================================================
/// \code
/// UT3D_pt_intplnln         intersect LineSegment X Plane
/// Von pl wird nur p und vz benutzt.
/// 
/// Returncodes:
///  -1 = LineSegment outside Plane
///   0 = vc parallel to plane
///   1 = OK; intersecting ...
///   2 = ip = ln.p1
///   3 = ip = ln.p2
///
/// intersect unlimited Line X Plane: UT3D_pt_intptvcpln
/// see also UT3D_pt_intlnpl1 UT3D_ptDi_intptvcpln
/// \endcode


  int    irc;
  Vector vln;

  UT3D_vc_ln (&vln, ln);
  irc = UT3D_ptDi_intptvcpln (ip, dist, pl, &ln->p1, &vln);
  if(irc < 1) return irc;


  if(*dist < 0.5) {           // test ln.p1
    // check if (ln.p1 = ip)
    if(UT3D_comp2pt(&ln->p1, ip, UT_TOL_pt) == 1) return 2;
    if(*dist < 0.) return -1;

  } else {                    // test ln.p2
    // check if (ln.p2 = ip)
    if(UT3D_comp2pt(&ln->p2, ip, UT_TOL_pt) == 1) return 2;
    if(*dist > 1.) return -1;
  }

  return 1;

}


//======================================================================
  int UT3D_pt_intlnpl (Point *ip, Plane *pl, Line *ln) {
//======================================================================
/// \code
/// UT3D_pt_intlnpl           point = intersection of line - plane
///   ln = unlimited line
/// 
/// Returncodes:
///   0 = line parallel to plane
///   1 = OK;
/// \endcode


  Vector   vln;

  UT3D_vc_ln (&vln, ln);

  return (UT3D_pt_intptvcpl_ (ip, pl, &ln->p1, &vln));

 }



//======================================================================
  int UT3D_pt_intptvcpl_ (Point *ip, Plane *pl, Point *pt, Vector *vln) {
//======================================================================
/// \code
/// UT3D_pt_intptvcpl_         point = intersection of pt / vc  - plane
/// Von pl wird nur p und vz benutzt.
/// 
/// Returncodes:
///   0 = vc parallel to plane
///   1 = OK;
/// \endcode


  double   dist;


  // OFFEN:
  // ckeck for type of Plane: X/Y/Z-parallel
  // X/Y/Z-parallel: use UT3D_pt_intptvcplx/y/z



  return UT3D_ptDi_intptvcpln (ip, &dist, pl, pt, vln);

}

//================================================================
  int UT3D_ptDi_intptvcptvc (Point *ip,  double *dist,
                             Point *ptl, Vector *vcl,
                             Point *ptp, Vector *vcp) {
//================================================================
/// \code
/// intersect line-plane (pt-vc  pt-vc)
/// ip = project pt1 along vcl onto plane ptp-vcp
///
/// Output:
///   dist = der Faktor fuer den Abstand vom pt zum ip in Richtung vln.
///          vln * dist ab pt ergibt ip.
/// Returncodes:
///   0 = vc parallel to plane
///   1 = OK;
/// \endcode

// see UT3D_ptDi_intptvcpln

  double   k1, k2, p;
  Vector   v1;


  k1 = vcp->dx * vcl->dx + vcp->dy * vcl->dy + vcp->dz * vcl->dz;
  // printf(" k1=%f\n",k1);

  if (fabs(k1) < 1E-12) {
    return 0;
  }

  p = vcp->dx * ptp->x +
      vcp->dy * ptp->y +
      vcp->dz * ptp->z;

  k2 = p - vcp->dx * ptl->x -
           vcp->dy * ptl->y -
           vcp->dz * ptl->z;

  *dist = k2/k1;

  // printf("  intptvcpl k1=%f k2=%f dist=%f\n",k1,k2,*dist);

  v1.dx = vcl->dx * *dist;
  v1.dy = vcl->dy * *dist;
  v1.dz = vcl->dz * *dist;

  // printf("  intptvcpl %f %f %f\n",v1.dx,v1.dy,v1.dz);

  UT3D_pt_traptvc (ip, ptl, &v1);

  // printf("ex UT3D_ptDi_intptvcptvc %f,%f,%f\n",ip->x,ip->y,ip->z);
  // printf("   dist = %f\n",*dist);

  return 1;

}


//================================================================
  int UT3D_ptDi_intptvcpln (Point *ip, double *dist,
                            Plane *pl, Point *pt, Vector *vln) {
//================================================================
/// \code
/// UT3D_ptDi_intptvcpln      intersect line-plane (pt-vc  pt-vc)
/// provide also parameter of intersection-point on line.
/// Von pl wird nur p und vz benutzt.
/// 
/// Input:
///   pt,vln     line to intersect with plane pl
/// Output:
///   ip         intersectionpoint; 
///   dist       der Faktor fuer den Abstand vom pt zum ip in Richtung vln.
///              vln * dist ab pt ergibt ip.
/// Returncodes:
///   0 = vc parallel to plane
///   1 = OK;
///
/// see also UT3D_pt_intplnln UT3D_pt_intlnpl1 UT3D_pt_intptvcpln
/// see also UPRJ_app_pt UT3D_pt_intptvcplx/y/z
/// \endcode

  double   k1, k2;
  Vector   v1;





  // printf("UT3D_pt_intptvcpl_ %f %f %f\n",pt->x,pt->y,pt->z);
  // printf("             vc = %f %f %f\n",vln->dx,vln->dy,vln->dz);
  // printf("          pl-po = %f %f %f\n",pl->po.x,pl->po.y,pl->po.z);
  // printf("          pl-vz = %f %f %f\n",pl->vz.dx,pl->vz.dy,pl->vz.dz);
  // printf("          pl-p  = %f\n",pl->p);



  k1 = pl->vz.dx * vln->dx + pl->vz.dy * vln->dy + pl->vz.dz * vln->dz;
  // printf(" k1=%f\n",k1);

  if (fabs(k1) < 1E-12 ) {
  // if ( fabs(k1) == 0.) {

    // printf("UT3D_pt_intptvcpl: Vector parallel to plane\n");
    // printf(" vc   %f %f %f\n",vln->dx,vln->dy,vln->dz);
    // printf(" pl-Z %f %f %f\n",pl->vz.dx,pl->vz.dy,pl->vz.dz);

    return 0;
  }


/* MUSS bei Plane gesetzt sein !!
  pl->p = pl->vz.dx * pl->po.x +
          pl->vz.dy * pl->po.y +
          pl->vz.dz * pl->po.z;
  printf(" pl->p=%f\n",pl->p);
*/


  k2 = pl->p - pl->vz.dx * pt->x -
               pl->vz.dy * pt->y -
               pl->vz.dz * pt->z;

  *dist = k2/k1;

  // printf("  intptvcpl k1=%f k2=%f dist=%f\n",k1,k2,*dist);

  v1.dx = vln->dx * *dist;
  v1.dy = vln->dy * *dist;
  v1.dz = vln->dz * *dist;

  // printf("  intptvcpl %f %f %f\n",v1.dx,v1.dy,v1.dz);

  UT3D_pt_traptvc (ip, pt, &v1);

  // printf("ex UT3D_pt_intptvcpl_ %f %f %f\n",ip->x,ip->y,ip->z);

  return 1;

}


//================================================================
  int UT3D_pt_intlnpl1 (Point *pi,  double *dl,
                        Point *p1,  Point *p2,
                        double dn1, double dn2) {
//================================================================
/// \code
/// intersect line - plane
/// OUT
///   pi      Intersectionpoint
///   dl      Faktor Abstand p1 - pi - p2; 0.5 ist genau in der Mitte.
/// IN
///   p1, p2  die Linie, die geschnitten wird
///   dn1 dn2 der Normalabstand der Punkte p1 p2 von der Schnittebene.
///           (get dn1 from UT3D_slen_ptpl)
///
/// pi liegt auf der Geraden p1;p2.
/// Die Plane wird ebenfalls als (hor-) Linie betrachtet.
/// pi ist intersection dieser beiden Lines.
/// dn. sind die NormalAbstaende von p1,p2 auf die Plane (=hor.Linie).
///
/// see also UT3D_pt_intplnln UT3D_ptDi_intptvcpln UT3D_pt_intptvcpln
/// \endcode

  Vector   dvc;


  // printf("UT3D_pt_intlnpl1 %f %f\n",dn1,dn2);


  // Faktor Abstand des Schnittpunkts zwischen p1 - p2;
  // 0.5 ist genau in der Mitte.
  *dl = dn1 / (dn1 - dn2);
    // printf(" *dl=%f\n",*dl);

  UT3D_vc_2pt (&dvc, p1, p2);         // dvc = p1 --> p2

  UT3D_vc_multvc (&dvc, &dvc, *dl);   // dvc = dvc * dl

  UT3D_pt_traptvc (pi, p1, &dvc);     // pi = p1 + dvc
    // GR_tDyn_symB__ (pi, SYM_STAR_S, 2);

  // dx1 dx2 eine 2D-Dimension von p1,p2; die mit dem hoechsten Wert.
  // *pp = dx1 + (dx2-dx1) * tmp;         // Abstand
  // der Abstand entspricht jenem Teil des pi, der fuer dx benutzt wurde !

  return 0;

}


//================================================================
  int UT3D_pt_intptvcpln (Point *pti,
                          Point *ptl,   Vector *vcl,
                          Point *ptpl,  Vector *vcpl) {
//================================================================
/// \code
/// UT3D_pt_intptvcpln       intersect line-plane (pt-vc  pt-vc)
/// Line durch Point + Vektor
/// Plane durch Nullpunkt + Normalvektor.
/// pl2vz muss nicht normiert sein;
///   See also UT3D_ln_int2pl UT3D_ptvc_int2pl UT3D_pt_intptvcpl
/// Returncodes:
///   0 = OK
///   1 = no intersection (parallel)
///
/// see also UT3D_pt_intplnln UT3D_pt_intlnpl1 UT3D_ptDi_intptvcpln
/// \endcode



  double  k1,k2,dist,p;
  Vector  v1;


  // GR_tDyn_vc__ (vcl, ptl, 3, 0);
  // GR_tDyn_vc__ (vcpl, ptpl, 3, 0);



  k1 = vcpl->dx * vcl->dx + vcpl->dy * vcl->dy + vcpl->dz * vcl->dz;
  // printf(" k1=%f\n",k1);

  if ( fabs(k1) < 1E-12 ) {

    // printf("UT3D_pt_intptvcpl: Vector parallel to plane\n");
    // printf(" vc   %f %f %f\n",vcl->dx,vcl->dy,vcl->dz);
    // printf(" pl-Z %f %f %f\n",vcpl->dx,vcpl->dy,vcpl->dz);

    return 0;
  }


  p =    vcpl->dx * ptpl->x +
         vcpl->dy * ptpl->y +
         vcpl->dz * ptpl->z;
  // printf(" p=%f\n",p);


  k2 = p - vcpl->dx * ptl->x -
           vcpl->dy * ptl->y -
           vcpl->dz * ptl->z;
  // printf(" k2=%f\n",k2);

  dist = k2/k1;


  // printf("  intptvcpl %f %f %f\n",k1,k2,dist);

  v1.dx = vcl->dx * dist;
  v1.dy = vcl->dy * dist;
  v1.dz = vcl->dz * dist;

  // printf("  intptvcpl %f %f %f\n",v1.dx,v1.dy,v1.dz);

  UT3D_pt_traptvc (pti, ptl, &v1);

  // printf("ex UT3D_pt_intptvcpl_ %f %f %f\n",ip->x,ip->y,ip->z);



  // GR_tDyn_symB__ (pti, SYM_TRI_S, 2);



  return 0;

}


//==============================================================================
  int UT3D_pt_intperp2ln (Point *po,Point *p1,Vector *v1,Point *p2,Vector *v2) {
//==============================================================================
/// \code
/// UT3D_pt_intperp2ln            inters ln2 with perp.plane to ln1
/// Plane geht durch p1 normal auf v1; Schnittpunkt liegt auf p2/v2.
/// po-p1 hat rechten Winkel zu p1-v1.
/// see UT3D_pt_intptvcpl; pl.po=p1; pl.vz=v1; pt=p2, vln=v2.
/// Returncodes:
///   0 = vc parallel to plane; Error
///   1 = OK;
///  -1 = p1 und p2 ident; po = p1.
/// \endcode


  double   k1, k2, dist, pp;
  Vector   v3;


  // DEB_dump_obj__ (Typ_PT, p1, "UT3D_pt_intperp2ln");
  // DEB_dump_obj__ (Typ_PT, p2, "    p2");
  // DEB_dump_obj__ (Typ_VC, v1, "    v1");
  // DEB_dump_obj__ (Typ_VC, v2, "    v2");

  if(UT3D_comp2pt(p1, p2, UT_TOL_pt) == 1) {       // 1=gleich
    *po = *p1;
    return -1;
  }

  k1 = v1->dx * v2->dx + v1->dy * v2->dy + v1->dz * v2->dz;

  // printf(" k1=%f\n",k1);

  if ( fabs(k1) < 1E-12 ) return 0;


  pp = v1->dx * p1->x +
       v1->dy * p1->y +
       v1->dz * p1->z;
  // printf(" pp=%f\n",pp);


  k2 = pp - v1->dx * p2->x -
            v1->dy * p2->y -
            v1->dz * p2->z;

  dist = k2/k1;

  // printf("  intperp2ln %f %f %f %f\n",k1,k2,dist,pp);

  v3.dx = v2->dx * dist;
  v3.dy = v2->dy * dist;
  v3.dz = v2->dz * dist;

  UT3D_pt_traptvc (po, p2, &v3);

  // DEB_dump_obj__ (Typ_PT, po, "intperp2ln v3");
  // DEB_dump_obj__ (Typ_PT, po, "ex UT3D_pt_intperp2ln");
  // GR_tDyn_symB__ (po, SYM_TRI_S, 2);

  return 1;

}


//========================================================================
  int UT3D_pt_ipl_2ptpt2 (Point *pi3, Point *p1, Point *p2, Point2 *pi2) {
//========================================================================
/// \code
/// UT3D_pt_ipl_2ptpt2        interpolate PointPosition for 2D-point
/// Input:   2 3D-points and 1 2D-point (between the 3D-pt's; z-coord missing).
/// Output:  3D-Point with interpolated Z-coord.
///
/// see also UTP_par1_vMin_vMax_vx UTP_vx_vMin_vMax_par1
/// \endcode

  double     d1, d2, par, dz, dp, dd;


  d1 = p2->x - p1->x;
  d2 = p2->y - p1->y;

  if(fabs(d1) > fabs(d2)) {
    d1 = p1->x;
    d2 = p2->x;
    dp = pi2->x;
  } else {
    d1 = p1->y;
    d2 = p2->y;
    dp = pi2->y;
  }

  pi3->x = pi2->x;
  pi3->y = pi2->y;

  dz = p2->z - p1->z;

  // compute z-value for intersectionPoint ip.
  dd = d2 - d1;
  par =  (dp - d1) / dd;
  pi3->z = p1->z + (dz * par);
    printf(" d1=%f d2=%f par=%f\n",d1,d2,par);


  return 0;

}


//================================================================
  int UT3D_pt_mirptln (Point *pto, Point *pti, Line *ln1) {
//================================================================
/// \code
/// mirror point about line
/// \endcode


  UT3D_pt_projptln (pto, NULL, NULL, pti, ln1);
  UT3D_pt_opp2pt (pto, pto, pti);

  return 0;

}


//================================================================
  int UT3D_pt_mirptpl (Point *pto, Point *pti, Plane *pln) {
//================================================================
/// \code
/// mirror point about plane
/// \endcode

// pt auf die Pln projiz; opposite Punkt = gespiegelter pt.

  UT3D_pt_projptpl (pto, pln, pti);
  UT3D_pt_opp2pt (pto, pto, pti);

  return 0;

}

/*
//================================================================
  int UT3D_pt_mirptpl (Point *pto, Point *pti, ObjBin *objMir) {
//================================================================
/// \code
/// mirror point in plane
/// pt auf die Pln projiz; opposite Punkt = gespiegelter pt.
/// \endcode



  if(objMir->typ == Typ_PLN) {
    UT3D_pt_projptpl (pto, (Plane*)objMir->obj, pti);

  } else { // Ln
    UT3D_pt_projptln (pto, &d1, pti, (Line*)objMir->obj);

  }

  UT3D_pt_opp2pt (pto, pto, pti);

  return 0;

}
*/

//================================================================
  int UT3D_isConvx_ptTab (int pNr, Point *pa) {
//================================================================
/// \code
/// check if is contour convex or concave
/// UT3D_isConvx_ptTab                               nach Karl Sauer 2005-08-12
/// pa ist eine geschlossene Kontur; der erste und der letzte Punkt sind ident !
/// irc  1  Konvex: zB ein Kreis, kann via GL_set_fan dargestellt werden
/// irc -1  Konkav: tesselieren
/// \endcode
#define CONVEX  1
#define CONCAV -1

  int     bp, i1, i2, i3, ie, s1, s2;
  double  d1;
  Vector2 v1, v2;

  // GR_tDyn_npt__ (pNr, pa, SYM_TRI_S, 2);

  if(pNr < 4) return CONVEX;


  // Contour nach 2D; dazu Backplane suchen
  bp = UT3D_bp_pta (pNr, pa);
    // printf(" bp=%d\n",bp);

  // Startvec
  UT2D_vc_2pt3_bp (&v2, &pa[0], &pa[1], bp);

  s2 = 0; i1 = 0; i2 = 1; i3 = 2; ie = pNr -1;


  L_test:
  v1 = v2;

  // V2 = 2D-vec von P[i2] --> P[i3]
  UT2D_vc_2pt3_bp (&v2, &pa[i2], &pa[i3], bp);

  // nur vorzeichen des Winkels bestimmen mit -y1.x2 + x1.y2
  d1 = -v1.dy * v2.dx + v1.dx * v2.dy;

  if     (d1 >  UT_TOL_min2) s1 =  1;
  else if(d1 < -UT_TOL_min2) s1 = -1;
  else {                               // skip; 3 collinear points
    // printf(" skip P%d P%d P%d\n",i1,i2,i3);
    goto L_weiter;
  }


  // printf(" test P%d P%d P%d d1=%f s1=%d s2=%d\n",i1,i2,i3,d1,s1,s2);

  if(s2 == 0) {s2 = s1; goto L_weiter;}      // erstmalig setzen

  if(s1 != s2)  {
      // printf("ex UT3D_isConvx_ptTab CONCAV  pNr=%d\n",pNr);
    return CONCAV;
  }

  // prepare next set of points ..
  L_weiter:
  ++i1; ++i2; ++i3;
  if(i2 < ie) goto L_test;
  i3 = 1;  // i2 ist nun der letze Punkt == ident mit dem ersten.
  if(i1 < ie) goto L_test;

    // s1 ist eigentlich der Umdrehungssinn bezueglich des BackplaneVektors !
    // printf("ex UT3D_isConvx_ptTab CONVEX %d pNr=%d\n",s1,pNr);
  return CONVEX;
}


//================================================================
  int UT3D_parpt_ln__ (double *parP, Point *ptx, Line *ln1) {
//================================================================
/// UT3D_parpt_ln__           parameter of distance of point ptx on line
/// retcode    0=OK; -1=degenerated-line


  Vector   v1, v2;

  // DEB_dump_obj__ (Typ_LN, ln1, "UT3D_parpt_ln__");
  // DEB_dump_obj__ (Typ_PT, ptx, "   ptx");


  UT3D_vc_2pt (&v1, &ln1->p1, ptx);
  UT3D_vc_2pt (&v2, &ln1->p1, &ln1->p2);

  return UT3D_parvc_2vc (parP, &v1, &v2);

}


//======================================================================
  int UT3D_parpt_ptvc (double *pl, Point *ptx, Point *pt1, Vector *vc1) {
//======================================================================
/// \code
/// UT3D_parpt_ptvc    parameter of distance of point ptx on line pt1-vc1
///
/// pl ist zwischen 0 bis 1; wenn pl=0.5 liegt ptx
/// genau in der Mitte von pl1-pl2
///
///
///                ptx
///                 X
///                /|
///               / |
///              /  |
///             /   |
///            /   .|
///           X-----|------------>X
///         pt1     pl=0.4       vc1
/// RetCodes:
///    0  OK
///   -1  v1-v2 aufeinander Normal; pl = 0.
/// \endcode


// see also UT3D_parvc_2vc

  Vector   vpt;

  UT3D_vc_2pt (&vpt, pt1, ptx);
  UT3D_parvc_2vc (pl, &vpt, vc1);

    // printf("ex UT3D_parpt_ptvc pl=%f\n",*pl);

  return 0;

}


//===================================================================
  double UT3D_parpt_lnbp (Point *pti, Line *ln1, int bp) {
//===================================================================
/// \code
/// UT3D_parpt_lnbp          parameter of point on line (via 2D,BackPlane)
/// bp = Berechnungsebene BackPlane ex UT3D_bp_perp_vc
/// \endcode

  double  dxl, dyl, dxp, dyp, dl, dp, parP;


  // printf("UT3D_parpt_lnbp bp=%d\n",bp);
  // DEB_dump_obj__ (Typ_LN, ln1, "");
  // DEB_dump_obj__ (Typ_PT, pti, "");


  if(bp == BCKPLN_YZ) {            // Y-Z-Plane, skip X   (x=y, y=z)     0
    dxl = ln1->p2.y - ln1->p1.y;
    dyl = ln1->p2.z - ln1->p1.z;

    dxp = pti->y    - ln1->p1.y;
    dyp = pti->z    - ln1->p1.z;


  } else if(bp == BCKPLN_XZ) {     // X-Z-Plane, skip Y   (x=x, y=z)     1
    dxl = ln1->p2.x - ln1->p1.x;
    dyl = ln1->p2.z - ln1->p1.z;

    dxp = pti->x    - ln1->p1.x;
    dyp = pti->z    - ln1->p1.z;


  } else if(bp == BCKPLN_XY) {     // X-Y-Plane, skip Z   (x=x, y=y)     2
    dxl = ln1->p2.x - ln1->p1.x;
    dyl = ln1->p2.y - ln1->p1.y;

    dxp = pti->x    - ln1->p1.x;
    dyp = pti->y    - ln1->p1.y;

  }


  dl  = dxl*dxl + dyl*dyl;
  dp  = dxl*dxp + dyl*dyp;

  if (dl != 0.) {
    parP = dp / dl;
  } else {
    printf("UT3D_parpt_lnbp - E001\n");
    parP = 0.;
  }

  // printf("  parP=%f dl=%f dp=%f\n",parP,dl,dp);

  return parP;
}


//======================================================================
  int UT3D_pt_tangptci (Point *po1, Point *po2, Point *pt1, Circ *ci1) {
//======================================================================
/// \code
/// UT3D_pt_tangptci          tangent from point to circ
/// RetCod: Nr of solutions; -1,1,2.
/// see UT3D_vc_tng_ci_pt UT3D_vc_tng_ci_pt
/// \endcode

  int     irc;
  double  d1, d2, d3, d4;
  Point   pt2;
  Vector  vc1, vc2;


  // DEB_dump_obj__ (Typ_PT, pt1, "UT3D_pt_tangptci: ");
  // DEB_dump_obj__ (Typ_CI, ci1, "");


    // pt2 = den Punkt pt1 auf die Z-Achse des Kreises projizieren
    UT3D_pt_projptptvc (&pt2, &d1, NULL, pt1, &ci1->pc, &ci1->vz);

    d2 = fabs(ci1->rad);

    UT3D_vc_2pt (&vc1, pt1, &pt2);

    // vc2 = Crossprod vc1 - Z-Achse des Kreises.
    UT3D_vc_perp2vc (&vc2, &vc1, &ci1->vz);


    // liegt Punkt am Kreis: Tangente an den Kreis;
    if(UTP_comp2db (d1, d2, UT_TOL_pt) == 1) {     // liegt am Kreis.
      // create line w len = APT_ModSiz/2
      d1 = UT_LEN_TNG;
      UT3D_pt_traptvclen (po1, pt1, &vc2, d1);
      UT3D_pt_opp2pt (po2, pt1, po1);
      irc = 1;
      goto Fertig;
    }

    // liegt Punkt innerhalb Kreis: nix.
    if(d1 < d2) {irc = -1; goto Fertig;}


    // pt1 liegt ausserhalb Kreis; 2 tangenten durch pt1 an den Kreis.
    UT2D_solvtriri_bc   (&d4, &d3, d2, d1);
    // printf("P = P C d4=%f d3=%f d2=%f d1=%f\n",d4,d3,d2,d1);

    //printf(" d1-d4=%f %f %f %f\n",d1,d2,d3,d4);

    // vom pc d4 in Richtg vc1 abtragen
    UT3D_pt_traptvclen (&pt2, &ci1->pc, &vc1, -d4);

    // hc = d3 in Richtg vc2
    UT3D_pt_traptvclen (po1, &pt2, &vc2, d3);
    UT3D_pt_traptvclen (po2, &pt2, &vc2, -d3);

    irc = 2;

  Fertig:
  // printf("ex UT3D_pt_tangptci %d\n",irc);
  // if(irc > 0) GR_tDyn_symB__ (po1, SYM_TRI_S, 2);
  // if(irc > 1) GR_tDyn_symB__ (po2, SYM_TRI_S, 2);

  return irc;

}


//======================================================================
  int UT3D_pt_tng_ci_vc (Point *pto, Circ *ci1, Vector *vc1, int imod) {
//======================================================================
/// \code
/// UT3D_pt_tng_ci_vc          tangent with fixed vector to circ
/// imod      0 = point at y-axis (vc1=x-axis; ci1.vz=z-axis)
///           1 = point at negative y-axis
/// \endcode

  double   d1;
  Vector   vcn;


  printf("UT3D_pt_tng_ci_vc %d\n",imod);
  // DEB_dump_obj__ (Typ_CI, ci1,  "  circ:");
  // DEB_dump_obj__ (Typ_VC, vc1, "  vc1:");


  // get normal to vc1 (normal=y from vc1=x and ci.vz
  UT3D_vc_perp2vc (&vcn, &ci1->vz, vc1);
    // DEB_dump_obj__ (Typ_VC, &vcn, "  vcn:");


  // translate circ.pc length=radius along vcn
  d1 = fabs(ci1->rad);
  if((imod % 2) > 0) d1 *= -1.;
  UT3D_pt_traptvclen (pto, &ci1->pc, &vcn, d1);
    // GR_tDyn_symB__ (pto, SYM_TRI_S, 2);
    // GR_tDyn_vc__ (vc1, pto, 2, 0);

  return 0;

}


//=====================================================================
  int UT3D_pt_evpar2pt (Point *po, double lpar, Point *p1, Point *p2) {
//=====================================================================
/// \code
/// UT3D_pt_evpar2pt           evaluate param.point on line
/// einen Punkt auf Line ln1 errechnen;
/// lpar = Abstandswert 0 - 1.
/// lpar=0: pto = ln1.p1;
/// lpar=1: pto = ln1.p2;
/// \endcode


  double   dx, dy, dz;

  dx = p2->x - p1->x;
  dy = p2->y - p1->y;
  dz = p2->z - p1->z;
  // printf(" dx=%f dy=%f dz=%f\n",dx,dy,dz);

  po->x = p1->x + dx * lpar;
  po->y = p1->y + dy * lpar;
  po->z = p1->z + dz * lpar;

  return 0;

}

/*
//========================================================================
  int UT3D_pt__par_pt_vc (Point *po, double lpar, Point *pt1, Vector *vc1) {
//========================================================================
/// UT3D_pt__par_pt_vc          evaluate param.point on line from point+vector
/// lpar = Abstandswert


  po->x = pt1->x + vc1->dx * lpar;
  po->y = pt1->y + vc1->dy * lpar;
  po->z = pt1->z + vc1->dz * lpar;

  return 0;

}
*/


//===================================================================
  int UT3D_pt_evparln (Point *pto, double lpar, Line *ln1) {
//===================================================================
/// \code
/// UT3D_pt_evparln           evaluate param.point on line
/// einen Punkt auf Line ln1 errechnen;
/// lpar = Abstandswert 0 - 1.
/// lpar=0: pto = ln1.p1;
/// lpar=1: pto = ln1.p2;
/// \endcode


  return UT3D_pt_evpar2pt (pto, lpar, &ln1->p1, &ln1->p2);

}


//====================================================================
  int UT3D_pt_seg_par_nln (Point *p1, int is, double ps, ObjGX *cv1) {
//====================================================================
/// UT3D_pt_seg_par_nln      point <-- segNr & parameter on lines[] (Typ_CVLNA)
/// TODO: change Typ_CVLNA -> Typ_LN; 

// see UTO_pt_par1_dbo
// see UT3D_obj_segccv

  int      irc, typ;
  void     *data;
  Point    *pa, *pe, pta;
  Line     *ln1;
  ObjGX    ox1;


  // printf("UT3D_pt_seg_par_nln %d %f\n",is,ps);
  // DEB_dump_obj__ (Typ_ObjGX, cv1, " _pt_segparlna: ");
  // DEB_dump_ox_0 (cv1, " _pt_segparlna: "); 


  // get line <is> out of curve
  if(is < 0) return -1;
  if(is >= cv1->siz) return -1;
  ln1 = &((Line*)cv1->data)[is];
    // DEB_dump_obj__ (Typ_LN, ln1, " _pt_segparlna ln1:");


  // create paraemetric point on ln1
  return UT3D_pt_evpar2pt (p1, ps, &ln1->p1, &ln1->p2);

}


/* replaced by UT3D_pt_vc__par_cv
//======================================================================
  int UT3D_pt_evparcrv (Point *pto, double lpar, int typ, void *data) {
//======================================================================
/// \code
/// get point on line/curve from parameter
/// Aufruf nur mit der struct (statt einem ObjX):
/// ObjGX ox1; UT3D_pt_evparcrv (pto, 0.5, ox1.form, &ox1.data);
/// Input:
///   typ,data   curve
///   lpar       parameter on curve;  0-1
/// 
/// see also UT3D_pt_vc__par_cv UTO_stru_int
/// \endcode


  int     typ1;
  char    obj1[OBJ_SIZ_MAX];
  double  d1;


  // printf("UT3D_pt_evparcrv %f %d\n",lpar,typ);


  if(typ == Typ_PT) { // kommt bei SRU mit Point !!!!!
    *pto = *((Point*)data);


  //-------------------------------
  } else if(typ == Typ_LN) {

    UT3D_pt_evparln (pto, lpar, data);


  //-------------------------------
  } else if(typ == Typ_CI) {

    UT3D_pt_vc__par_ci (pto, lpar, data);


  //-------------------------------
  } else if(typ == Typ_CVPOL) {

    // change par 0-1  -->  dist
    lpar = UT3D_par_par1plg (lpar, data);
      // printf(" lpar=%f\n",lpar);

    // get point on plg
    UT3D_pt_evalplg (pto, data, lpar);


  //-------------------------------
  } else if(typ == Typ_CVELL) {

    // // change par 0-1  -->  angle
    // lpar = UT3D_angr_par1_ell (lpar, data);
      // printf(" lpar=%f\n",lpar);

    // get point on plg
    UT3D_ptvc_eval_ell_par (pto, NULL, data, lpar);


  //-------------------------------
  } else if(typ == Typ_CVCLOT) {

    // // change par 0-1  -->  curvature
    // lpar = UT3D_par_par1clot (lpar, data);
      // // printf(" lpar=%f\n",lpar);

    // get point on curv
    UT3D_ptvc_evparclot (pto, NULL, 1, data, lpar);


  //-------------------------------
  } else if(typ == Typ_CVBSP) {

    // change par 0-1  -->  knotVal
    lpar = UT3D_parbsp_par1 (lpar, data);

    UT3D_pt_evparCrvBSpl (pto, data, lpar);      // see UT3D_pt_evalparCv


  //-------------------------------
  } else if(typ == Typ_CVRBSP) {

    // change par 0-1  -->  knotVal
    lpar = UT3D_par_par1_rbsp (lpar, data);

    // get point from knotValue
    UT3D_pt_evparCrvRBSpl (pto, &d1, data, lpar);      // see UT3D_pt_evalparCv


  //-------------------------------
  } else if(typ == Typ_CVTRM) {
    // get standard-curve from trimmed-curev
    UTO_cv_cvtrm (&typ1, obj1, NULL, data);
    // recurse
    UT3D_pt_evparcrv (pto, lpar, typ1, obj1);


  //-------------------------------
  } else {
    TX_Error("UT3D_pt_evparcrv E001 %d",typ);
    return -1;
  }


  // TestDisp
  // GR_tDyn_symB__ (pto, SYM_TRI_S, 2);
  // DEB_dump_obj__ (Typ_PT, pto, "ex UT3D_pt_evparcrv %f",lpar);


  return 0;

}
*/

//================================================================
  int UT3D_pt_m3 (Point *pto, Mat_4x3 ma) {
//================================================================
/// UT3D_pt_m3                copy origin out of 4x3-matrix

  pto->x = ma[0][3];
  pto->y = ma[1][3];
  pto->z = ma[2][3];

  // DEB_dump_obj__ (Typ_PT, pto, "ex UT3D_pt_m3");
  
  return 0;

}

 
//=====================================================================
  void UT3D_pt_tra_pt_m3 (Point *p2, Mat_4x3 mata, Point* p1) {
//=====================================================================
/// \code
/// UT3D_pt_tra_pt_m3           apply transformation to point (from 4x3-matrix)
/// 
/// p2 and p1 may be identical
///
///        VX             VY            VZ        Nullpunkt
///
///  [0][0]=vx.dx   [0][1]=vy.dx  [0][2]=vz.dx   [0][3]=o.x
///  [1][0]=vx.dy   [1][1]=vy.dy  [1][2]=vz.dy   [1][3]=o.y
///  [2][0]=vx.dz   [2][1]=vy.dz  [2][2]=vz.dz   [2][3]=o.z
///
/// printf("    vz=%f,%f,%f\n",(*mata)[0][2],(*mata)[1][2],(*mata)[2][2]);
/// \endcode

/*
  printf(" matVX=%f,%f,%f",mata[0][0],mata[1][0],mata[2][0]);
  printf(" matVY=%f,%f,%f",mata[0][1],mata[1][1],mata[2][1]);
  printf(" matVZ=%f,%f,%f",mata[0][2],mata[1][2],mata[2][2]);
  printf(" matPO=%f,%f,%f",mata[0][3],mata[1][3],mata[2][3]);
*/

  double x, y, z;


  x = p1->x;
  y = p1->y;
  z = p1->z;


  /* printf("UT3D_pt_tra_pt_m3    %f,%f,%f",x,y,z); */


  p2->x = mata[0][0]*x + mata[0][1]*y + mata[0][2]*z + mata[0][3];
  p2->y = mata[1][0]*x + mata[1][1]*y + mata[1][2]*z + mata[1][3];
  p2->z = mata[2][0]*x + mata[2][1]*y + mata[2][2]*z + mata[2][3];


  /* printf("ex UT3D_pt_tra_pt_m3 %f,%f,%f",p2->x,p2->y,p2->z); */

}



//====================================================================
  void UT3D_pt_tra_pt_m4 (Point *p2, Mat_4x4 ma, Point *p1) {
//====================================================================
/// \code
/// UT3D_pt_tra_pt_m4           apply transformation to point (from 4x4-matrix)
///  p1 in p2 out.
/// \endcode


  double x, y, z, w;

  x = p1->x;
  y = p1->y;
  z = p1->z;

  w = (ma[0][3]*x + ma[1][3]*y + ma[2][3]*z + ma[3][3]);

  if(w != 1.0) {
    w = 1.0 / w;
    p2->x = w * (x*ma[0][0] + y*ma[1][0] + z*ma[2][0] + ma[3][0]);
    p2->y = w * (x*ma[0][1] + y*ma[1][1] + z*ma[2][1] + ma[3][1]);
    p2->z = w * (x*ma[0][2] + y*ma[1][2] + z*ma[2][2] + ma[3][2]);

  } else {
    p2->x = x*ma[0][0] + y*ma[1][0] + z*ma[2][0] + ma[3][0];
    p2->y = x*ma[0][1] + y*ma[1][1] + z*ma[2][1] + ma[3][1];
    p2->z = x*ma[0][2] + y*ma[1][2] + z*ma[2][2] + ma[3][2];
  }

  /* printf ("UT3D_pt_tra_pt_m4 %f %f %f \n",p2->x,p2->y,p2->z); */
}


//===================================================================
  int UT3D_pt_tra_wcs2ucs_pl (Point *pto, Plane *pln, Point *pti) {
//===================================================================
/// \code
/// UT3D_pt_tra_wcs2ucs_pl    point-worldCoords -> userCoords from plane
/// 
///
/// retour: UT3D_pt_tra_pt_2vc_2par
/// \endcode

  // DEB_dump_obj__(Typ_PT, pti, "UT3D_pt_tra_wcs2ucs_pl");


  Vector   vc1, *vcx, *vcy, *vcz;
    
  // vector in WCS
  UT3D_vc_2pt (&vc1, &pln->po, pti);

  vcx = &pln->vx;
  vcy = &pln->vy;
  vcz = &pln->vz;
    
  pto->x = (vcx->dx * vc1.dx + vcx->dy * vc1.dy + vcx->dz * vc1.dz);
  pto->y = (vcy->dx * vc1.dx + vcy->dy * vc1.dy + vcy->dz * vc1.dz);
  pto->z = (vcz->dx * vc1.dx + vcz->dy * vc1.dy + vcz->dz * vc1.dz);
    
    // DEB_dump_obj__ (Typ_PT, pto, " ex-UT3D_pt_tra_wcs2ucs_m3");

  return 0;

}

  
//===================================================================
  int UT3D_pt_tra_ucs2wcs_pl (Point *pto, Plane *pln, Point *pti) {
//===================================================================
/// \code
/// UT3D_pt_tra_ucs2wcs_pl    point-userCoords from plane -> worldCoords
/// 
/// wcs2ucs: UT3D_pt_tra_wcs2ucs_pl
/// \endcode


  Vector   vc1, *vcx, *vcy, *vcz;

  // DEB_dump_obj__(Typ_PT, pti, "UT3D_pt_tra_ucs2wcs_pl");

  vcx = &pln->vx;
  vcy = &pln->vy;
  vcz = &pln->vz;

  pto->x = pln->po.x + (vcx->dx * pti->x) + (vcy->dx * pti->y) + (vcz->dx * pti->z);
  pto->y = pln->po.y + (vcx->dy * pti->x) + (vcy->dy * pti->y) + (vcz->dy * pti->z);
  pto->z = pln->po.z + (vcx->dz * pti->x) + (vcy->dz * pti->y) + (vcz->dz * pti->z);

    // DEB_dump_obj__ (Typ_PT, pto, " ex-UT3D_pt_tra_ucs2wcs_pl");

  return 0;

}









/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/


//=======================================================================
  int UT3D_compvcNull (Vector *v1) {
//=======================================================================
/// \code
/// UT3D_compvcNull           compare vector for (exact) 0,0,0.
/// 
/// Retcode 0:   v1 <> 0,0,0
///         1:   v1 = 0,0,0
///
/// if(UT3D_compvcNull (&vz) == 1) printf("vec is null !\n");
/// \endcode


  // DEB_dump_obj__ (Typ_VC,  v1, "UT3D_compvcNull: ");

  if(fabs(v1->dx) > UT_TOL_min2) return 0;
  if(fabs(v1->dy) > UT_TOL_min2) return 0;
  if(fabs(v1->dz) > UT_TOL_min2) return 0;

  // if((v1->dx == 0.)&&(v1->dy == 0.)&&(v1->dz == 0.)) return 1;

  return 1;
}



//=======================================================================
  int UT3D_compvc0 (Vector *v1, double tol) {
//=======================================================================
/// \code
/// UT3D_compvc0              compare vectors for 0,0,0.
/// 
/// Retcode 0:   v1 <> 0.0 (gr. oder kl. tol)
///         1:   v1 = 0.0 (innerhalb tol)
///
/// if(UT3D_compvc0 (&vz, UT_TOL_min0) == 1) printf("vec is 0,0,0 !\n");
/// \endcode



  return ((UTP_compdb0 (v1->dx, tol)) &&
          (UTP_compdb0 (v1->dy, tol)) &&
          (UTP_compdb0 (v1->dz, tol)));

}


//================================================================
  int UT3D_comp2vc_p (Vector *v1, Vector *v2, double tol) {
//================================================================
/// \code
/// UT3D_comp2vc_p              compare 2 vectors for parallel
/// tolerances: see func. UT3D_vc_ck_parpl
/// RC=1:   die Vektoren sind gleich.
/// RC=0:   die Vektoren sind unterschiedlich.
/// 
/// if(UT3D_comp2vc_p(&ciO.vz, &UT3D_VECTOR_IZ, UT_TOL_min1) != 0) gleich
/// \endcode
// 
// hoechsten Zahlenwert suchen (x/y/od Z-Komponente)
// mit dieser Komponente (ibp) den Multiplikationsfaktor errechnen
// den ganzen Vektor mit diesem Faktor multiplizieren -
// nun Vektoren vergleichen; tol ev ebenfalls multiplizieren ?
//   (Komponente ibp nicht benutzen - ergibt Differenz 0.0)

  // int      ibp;
  double   d1, dd, dx, dy, dz;


  // printf("UT3D_comp2vc_p %f\n",tol);
  // DEB_dump_obj__ (Typ_VC, v1, " V1:");
  // DEB_dump_obj__ (Typ_VC, v2, " V2:");

  dx = fabs(v1->dx);
  dy = fabs(v1->dy);
  dz = fabs(v1->dz);


  if(dx > dy) {
    if(dz > dx) goto L_z_max;
    else goto L_x_max;

  } else {
    if(dz > dy) goto L_z_max;
    else goto L_y_max;
  }


  L_x_max:
    if(dx < UT_TOL_min2) return 0;
    d1 = fabs(v2->dx / dx);
    dd = fabs(tol * d1 * dx);
    // ibp = 0;
    goto L_1;

  L_y_max:
    if(dy < UT_TOL_min2) return 0;
    d1 = fabs(v2->dy / dy);
    dd = fabs(tol * d1 * dy);
    // ibp = 1;
    goto L_1;

  L_z_max:
    if(dz < UT_TOL_min2) return 0;
    d1 = fabs(v2->dz / dz);
    dd = fabs(tol * d1 * dz);
    // ibp = 2;


  L_1:
    // printf(" ibp=%d d1=%f dd=%f\n",ibp,d1,dd);
  if(d1 < UT_TOL_min1) return 0;

      // printf(" dx=%f\n",(v1->dx * d1) - v2->dx);
    if(fabs((v1->dx * d1) - v2->dx) > dd) return 0;

      // printf(" dy=%f\n",(v1->dy * d1) - v2->dy);
    if(fabs((v1->dy * d1) - v2->dy) > dd) return 0;

      // printf(" dz=%f\n",(v1->dz * d1) - v2->dz);
    if(fabs((v1->dz * d1) - v2->dz) > dd) return 0;

  return 1;

/*   alte Version:
  short  i1;
  Vector vc1, vc2;

  // same length necessary
  UT3D_vc_setLength (&vc1, v1, 1.);
  UT3D_vc_setLength (&vc2, v2, 1.);

  i1 =  (UTP_comp2db (vc1.dx, vc2.dx, tol)) &&
        (UTP_comp2db (vc1.dy, vc2.dy, tol)) &&
        (UTP_comp2db (vc1.dz, vc2.dz, tol));

  if (i1) return i1;

  UT3D_vc_invert (&vc2, &vc2);

  return ((UTP_comp2db (vc1.dx, vc2.dx, tol)) &&
          (UTP_comp2db (vc1.dy, vc2.dy, tol)) &&
          (UTP_comp2db (vc1.dz, vc2.dz, tol)));
*/

}


//================================================================
  int UT3D_comp2vc_d (Vector *v1, Vector *v2, double tol) {
//================================================================
/// \code
/// UT3D_comp2vc              compare 2 vectors for parallel and antiparallel
/// tolerances: see func. UT3D_vc_ck_parpl
/// RC=1:   die Vektoren sind gleich.
/// RC=0:   die Vektoren sind unterschiedlich.
/// \endcode

// hoechsten Zahlenwert suchen (x/y/od Z-Komponente)
// mit dieser Komponente (ibp) den Multiplikationsfaktor errechnen
// den ganzen Vektor mit diesem Faktor multiplizieren -
// nun Vektoren vergleichen; tol ev ebenfalls multiplizieren ?
//   (Komponente ibp nicht benutzen - ergibt Differenz 0.0)

  int      ibp;
  double   d1, dd, dx, dy, dz;


  // printf("==================== UT3D_comp2vc_d %f\n",tol);
  // DEB_dump_obj__ (Typ_VC, v1, " V1:");
  // DEB_dump_obj__ (Typ_VC, v2, " V2:");

  dx = fabs(v1->dx);
  dy = fabs(v1->dy);
  dz = fabs(v1->dz);
    // printf(" dx=%f dy=%f dz=%f\n",dx,dy,dz);


  
  if(dx > dy) {    
    if(dz > dx) goto L_z_max;
    else goto L_x_max;

  } else {                
    if(dz > dy) goto L_z_max;
    else goto L_y_max;
  }


  L_x_max:
    if(dx < UT_TOL_min2) goto L_exit_no;
    d1 = fabs(v2->dx / dx);
    dd = fabs(tol * d1 * dx);
    ibp = 0;
    goto L_1;

  L_y_max:
    if(dy < UT_TOL_min2) goto L_exit_no;
    d1 = fabs(v2->dy / dy);
    dd = fabs(tol * d1 * dy);
    ibp = 1;
    goto L_1;

  L_z_max:
    if(dz < UT_TOL_min2) goto L_exit_no;
    d1 = fabs(v2->dz / dz);
    dd = fabs(tol * d1 * dz);
    ibp = 2;


  L_1:
    // printf(" ibp=%d d1=%f dd=%f\n",ibp,d1,dd);
  if(d1 < UT_TOL_min1) goto L_exit_no;


  if(ibp != 0) {
      // printf(" dx=%f\n",(dx * d1) - fabs(v2->dx));
    if(fabs((dx * d1) - fabs(v2->dx)) > dd) goto L_exit_no;
  }

  if(ibp != 1) {
      // printf(" dy=%f\n",(dy * d1) - fabs(v2->dy));
    if(fabs((dy * d1) - fabs(v2->dy)) > dd) goto L_exit_no;
  }

  if(ibp != 2) {
      // printf(" dz=%f\n",(dz * d1) - fabs(v2->dz));
    if(fabs((dz * d1) - fabs(v2->dz)) > dd) goto L_exit_no;
  }


    // printf("ex UT3D_comp2vc_d 1\n");
  return 1;


  L_exit_no:  // vectors totally different
      // printf("ex UT3D_comp2vc_d 0\n");
  return 0;


/*   alte Version:
  short  i1;
  Vector vc1, vc2;

  // same length necessary
  UT3D_vc_setLength (&vc1, v1, 1.);
  UT3D_vc_setLength (&vc2, v2, 1.);

  i1 =  (UTP_comp2db (vc1.dx, vc2.dx, tol)) &&
        (UTP_comp2db (vc1.dy, vc2.dy, tol)) &&
        (UTP_comp2db (vc1.dz, vc2.dz, tol));

  if (i1) return i1;

  UT3D_vc_invert (&vc2, &vc2);

  return ((UTP_comp2db (vc1.dx, vc2.dx, tol)) &&
          (UTP_comp2db (vc1.dy, vc2.dy, tol)) &&
          (UTP_comp2db (vc1.dz, vc2.dz, tol)));
*/

}


//================================================================
  int UT3D_vc_ck_parl2_vc (Vector *v1, Vector *v2, double tol) {
//================================================================
/// \code
/// UT3D_vc_ck_parl2_vc            check for parallel/antiparallel (normalized only)
/// 
/// useable also for circles and planes (vz is normalized)
/// tol: RAD_1 RAD_01 ..
///
/// Output:
///   retCod     0   vectors are not parallel or antiparallel
///              1   vectors are parallel or antiparallel
/// see also
///   UT3D_vc_ck_aparl_vc  (check for antiparallel)
///   UT3D_comp2vc_p, UT3D_comp2vc_d     - if not normalized
/// \endcode


  double   dx1, dy1, dz1;
  double   dx2, dy2, dz2;

  dx1 = fabs(v1->dx);
  dx2 = fabs(v2->dx);
  if(!UTP_comp2db (dx1, dx2, tol)) return 0;

  dy1 = fabs(v1->dy);
  dy2 = fabs(v2->dy);
  if(!UTP_comp2db (dy1, dy2, tol)) return 0;

  dz1 = fabs(v1->dz);
  dz2 = fabs(v2->dz);
  if(!UTP_comp2db (dz1, dz2, tol)) return 0;

  return 1;
}



//================================================================
  int UT3D_vc_ck_parl_vc (Vector *v1, Vector *v2, double tol) {
//================================================================
/// \code
/// UT3D_vc_ck_parl_vc            check for parallel (normalized only)
/// 
/// useable also for circles and planes (vz is normalized)
/// tol: RAD_1 RAD_01 ..
///
/// Output:
///   retCod     0   vectors are not parallel
///              1   vectors are parallel
/// see also
///   UT3D_vc_ck_aparl_vc  (check for antiparallel)
///   UT3D_comp2vc_p, UT3D_comp2vc_d     - if not normalized
/// \endcode



  return ((UTP_comp2db (v1->dx, v2->dx, tol)) &&
          (UTP_comp2db (v1->dy, v2->dy, tol)) &&
          (UTP_comp2db (v1->dz, v2->dz, tol)));
}


//================================================================
  int UT3D_vc_ck_aparl_vc (Vector *v1, Vector *v2, double tol) {
//================================================================
/// \code
/// UT3D_vc_ck_aparl_vc       check for antiparallel (normalized only)
///
/// tol: RAD_1 RAD_01 ..
///
/// Output:
///   retCod     0   vectors are not antiparallel
///              1   vectors are parallel with different direction
/// see also UT3D_comp2vc__
/// \endcode

  double  dd;


  dd = -v1->dx;
  if(!UTP_comp2db (dd, v2->dx, tol)) return 0;

  dd = -v1->dy;
  if(!UTP_comp2db (dd, v2->dy, tol)) return 0;

  dd = -v1->dz;
  if(!UTP_comp2db (dd, v2->dz, tol)) return 0;

  return 1;

}


/* ersetzt durch UT3D_sid_2vc
//================================================================
  int UT3D_ckDir_2vc (Vector *vc1, Vector *vc2) {
//================================================================
// check if 2 vectors have same or opposite direction
// Retcod:   1 = same direction
//           0 = vc1 is normal to vc2
//          -1 = opposite direction

  double  skp;

  skp = vc1->dx * vc2->dx + vc1->dy * vc2->dy + vc1->dz * vc2->dz;

  // printf("UT3D_ckDir_2vc %f %d\n",skp,DSIGN(skp));

  if(fabs(skp) < UT_TOL_min2) return 0;

  return DSIGN(skp);

}
*/

//================================================================
  int UT3D_vc_ck_parpl (Vector *vci, Plane *pli, double tol) {
//================================================================
/// \code
/// UT3D_vc_ck_parpl           check if vec is parallel to plane
/// tol ca 0.1  (zB UT_TOL_cv
/// tol 0.02  ist 1.15 Grad (bei Vec-Laenge 1 !)
/// tol 0.01  ist 0.60 Grad (bei Vec-Laenge 1 !)
/// tol 0.004 ist 0.23 Grad (bei Vec-Laenge 1 !)
/// RetCode:
///  0 = vec is NOT parallel to plane
///  1 = vec is parallel to plane
/// \endcode


// see UT3D_ln_ck_parpl

  double scp;
  Vector vl;


  // printf("UT3D_ln_ck_parpl tol=%f\n",tol);
  // DEB_dump_obj__ (Typ_VC,  vci, "");
  // DEB_dump_obj__ (Typ_PLN, pli, "");


  // vector of line
  UT3D_vc_setLength (&vl, vci, 1.);
  // DEB_dump_obj__ (Typ_VC,  &vl, "vl:");

  // scalar product <line vector, plane normal vector>
  scp = UT3D_acos_2vc (&vl, &(pli->vz));
  // printf(" UT3D_vc_ck_parpl: scp=%f\n",scp);
  // printf(" openAng=%f\n",90. - UT_DEGREES(ACOS(scp)));

  if (fabs(scp) < tol) {

    // Yes; is parallel.
    return 1;

  } else {
    // No; is NOT parallel.
    return 0;
  }
}


//================================================================
  int UT3D_vc_ck_perpvc (Vector *vc1, Vector *vc2, double tol) {
//================================================================
/// \code
/// UT3D_vc_ck_perpvc           check if 2 vectors are normal (perpendic.)
/// For normalized vectors use UT3D_vc_ckperp_2vc1.
/// tol ca 0.1  (auch 1. genuegt!)
/// RetCode:
///  0 = vc1 is NOT normal to vc2
///  1 = vc1 is normal to vc2
/// \endcode


// see UT3D_ln_ck_parpl

  double scp;
  Vector vl1, vl2;


  // printf("UT3D_vc_ck_perpvc tol=%f\n",tol);
  // DEB_dump_obj__ (Typ_VC,  vc1, "");


  // vector of line
  UT3D_vc_setLength (&vl1, vc1, 1.);
  UT3D_vc_setLength (&vl2, vc2, 1.);

  // scalar product <line vector, plane normal vector>
  // scp = UT3D_acos_2vc (&vl1, &vl2);
  scp = vl1.dx * vl2.dx + vl1.dy * vl2.dy + vl1.dz * vl2.dz;
  // printf(" UT3D_vc_ck_parpl: scp=%f\n",scp);

  if (fabs(scp) < tol) {

    // Yes; is normal.
    return 1;

  } else {
    // No; is NOT normal.
    return 0;
  }
}


//================================================================
  int UT3D_vc_ckperp_2vc1 (Vector *vc1, Vector *vc2, double tol) {
//================================================================
/// \code
/// UT3D_vc_ckperp_2vc1         check if 2 normalized-vectors are normal (perp.)
/// For not-normalized vectors use UT3D_vc_ck_perpvc.
/// tol entspricht etwa der Abweichung; 1 / 0.1 = Steigung 10 : 1;
///     0.1 ~ 5.7 deg, 0.01 ~ 0.5 deg ..
/// RetCode:
///  0 = vc1 is NOT normal to vc2
///  1 = vc1 is normal to vc2
/// \endcode

// see UT3D_ln_ck_parpl

  double scp;


  // printf("UT3D_vc_ck_perpvc tol=%f\n",tol);
  // DEB_dump_obj__ (Typ_VC,  vc1, "");

  // scalar product <line vector, plane normal vector>
  scp = vc1->dx * vc2->dx + vc1->dy * vc2->dy + vc1->dz * vc2->dz;
  // printf(" UT3D_vc_ck_parpl: scp=%f\n",scp);

  if (fabs(scp) < tol) {

    // Yes; is normal.
    return 1;

  } else {
    // No; is NOT normal.
    return 0;
  }
}


//================================================================
  int UT3D_vc_ck_std (Vector *vc1) {
//================================================================
/// \code
/// UT3D_vc_ck_std            check for standardVector (DX DY DZ DIX DIY DIZ)
/// RetCod:
///   0         not parallel to x, y, or z-vector
///   1         DX
///   2         DY
///   3         DZ
///  -1         DIX
///  -2         DIY
///  -3         DIZ
///
///  index:   i = v; if(i < 0) i = 3 - i;  // gives 1-6
///  DBindex: dbi = -i;
/// \endcode



  int       i1;
  double    dx, dy, dz;


  // DEB_dump_obj__ (Typ_VC, vc1, "UT3D_vc_ck_std");

  dx = fabs(vc1->dx);
  dy = fabs(vc1->dy);
  dz = fabs(vc1->dz);

  if((dx > dy)&&(dx > dz)) {     // X ist am groessten
    if(!UTP_comp2db(dx, 1.0, UT_TOL_pt)) return 0;
    i1 = UT3D_comp2vc_d (vc1, (Vector*)&UT3D_VECTOR_X, UT_TOL_min1);
    if(i1 == 0) return 0;
    if(vc1->dx > 0.)  return 1;
    else              return -1;



  } else if(dy > dz) {           // Y ist am groessten
    if(!UTP_comp2db(dy, 1.0, UT_TOL_pt)) return 0;
    i1 = UT3D_comp2vc_d (vc1, (Vector*)&UT3D_VECTOR_Y, UT_TOL_min1);
    if(i1 == 0) return 0;
    if(vc1->dy > 0.)  return 2;
    else              return -2;


  } else {                       // Z ist am groessten
    if(!UTP_comp2db(dz, 1.0, UT_TOL_pt)) return 0;
    i1 = UT3D_comp2vc_d (vc1, (Vector*)&UT3D_VECTOR_Z, UT_TOL_min1);
    if(i1 == 0) return 0;
    if(vc1->dz > 0.)  return 3;
    else              return -3;

  }


  return 0;

}


/*
//================================================================
  void UT3D_vc_3db (Vector *vc, double x, double y, double z) {
//================================================================
// UT3D_vc_3db              Vector < 3 doubles (x, y, z)

  vc->dx = x; vc->dy = y; vc->dz = z;

  return;

}


//================================================================
  void UT3D_vc_pt (Vector *vc, Point *pt) {
//================================================================
// UT3D_vc_pt                Vector = 0,0,0 -> Point

  vc->dx = pt->x; vc->dy = pt->y; vc->dz = pt->z;
  // memcpy(vc, pt, sizeof(Point));

  return;

}
*/


//=====================================================================
  int UT3D_vc_tra_vc2_bp (Vector *v3, Vector2 *v2, int bpi, double *bpd) {
//=====================================================================
/// UT3D_vc_tra_vc2_bp           transfer 2D-vector onto 3D-vector from bp and bpd

  if       (bpi == BCKPLN_XY) {
    v3->dx = v2->dx;
    v3->dy = v2->dy;
    v3->dz = 0.;  //*bpd;   

  } else if(bpi == BCKPLN_XZ) {
    v3->dx = v2->dx;
    v3->dy = 0.;  //*bpd;
    v3->dz = v2->dy;

  } else if(bpi == BCKPLN_YZ) {
    v3->dx = 0.;  //*bpd;
    v3->dy = v2->dx;
    v3->dz = v2->dy;
  }

  // DEB_dump_obj__(Typ_PT, p3, "ex UT3D_pt_tra_pt2_bp p3=");

  return 0;

}


//=====================================================================
  int UT3D_pt_tra_pt2_bp (Point *p3, Point2 *p2, int bpi, double *bpd) {
//=====================================================================
/// UT3D_pt_tra_pt2_bp           transfer 2D-point onto 3D-plane from bp and bpd

  if       (bpi == BCKPLN_XY) {
    p3->x = p2->x;
    p3->y = p2->y;
    p3->z = *bpd;

  } else if(bpi == BCKPLN_XZ) {
    p3->x = p2->x;
    p3->y = *bpd;
    p3->z = p2->y;

  } else if(bpi == BCKPLN_YZ) {
    p3->x = *bpd;
    p3->y = p2->x;
    p3->z = p2->y;
  }

  // DEB_dump_obj__(Typ_PT, p3, "ex UT3D_pt_tra_pt2_bp p3=");

  return 0;

}


//=======================================================================
  void UT3D_pt_txt (Point *pt, char *txt) {
//=======================================================================
/// \code
/// UT3D_pt_txt               Point from text
/// delimiters: blank, comma, comma and blank .. 
/// eg: "100 23 45"
/// \endcode


  int  i1;
  char *p1, *p2;


  i1 = strlen(txt);

  if(i1 < 1) {
    pt->x = 0.;
    pt->y = 0.;
    pt->z = 0.;

  } else {
    pt->x = strtod (txt, &p2); ++p2;
    pt->y = strtod (p2,&p1); p2 = p1; ++p2;
    if(p1) {
      pt->z = strtod (p2, &p1);
    } else {
      pt->z = 0.;
    }
  }

  return;

}


//=======================================================================
  void UT3D_vc_txt (Vector *vc, char *txt) {
//=======================================================================
/// UT3D_vc_txt               Vector from text
/// txt eg. "1 0 [0]"


  int  i1;
  char *p1, *p2;


  i1 = strlen(txt);

  if(i1 < 1) {
    vc->dx = 0.;
    vc->dy = 0.;
    vc->dz = 0.;

  } else {
    vc->dx = strtod (txt, &p2); ++p2;
    vc->dy = strtod (p2,&p1); p2 = p1; ++p2;
    if(p1) {
      vc->dz = strtod (p2, &p1);
    } else {
      vc->dz = 0.;
    }
  }

  return;

}



//================================================================
  int UT3D_vcz_bp (Vector *vn, int bp) {
//================================================================
/// UT3D_vcz_bp               returns normalVector from BackPlaneNr (0-2)


  switch (bp) {

    case BCKPLN_XY:
      *vn = UT3D_VECTOR_Z;
      break;

    case BCKPLN_XZ:
      *vn = UT3D_VECTOR_Y;
      break;

    case BCKPLN_YZ:
      *vn = UT3D_VECTOR_X;
      break;
  }

  return 0;

}


//================================================================
  int UT3D_2vc_pta (Vector *vc1, Vector *vc2, int pNr, Point *pta) {
//================================================================
/// \code
/// get 2 vecs of max extent of a polgon (for perp.Vec)
/// 2 normal aufeinander-stehende Vektoren eines Polygon suchen
/// (fuer Normalvektor auf Polygon)
/// \endcode

  int      i1, ii1, ii2;
  double   d1, vl;
  Vector   vx1, vx2, vxd;


  // zuerst den laengsten Vektor suchen;
  d1 = 0.;
  for (i1=1; i1<pNr; ++i1) {
    vl = fabs(pta[0].x - pta[i1].x +
              pta[0].y - pta[i1].y +
              pta[0].z - pta[i1].z);
    // printf(" P[%d] vl=%f\n",i1,vl);
    if(vl > d1) {d1 = vl; ii1 = i1;}
  }
  // printf(" ii1=%d\n",ii1);

  // longest vec vc1 = p[0] --> p[ii1]
  UT3D_vc_2pt (&vx1, &pta[0], &pta[ii1]);

  // vec mit max. Normalabst zu vx1 suchen
  d1 = 0.;
  for (i1=1; i1<pNr; ++i1) {
    if(i1 == ii1) continue;
    UT3D_vc_2pt (&vx2, &pta[0], &pta[i1]);
    // size = Normalabstand !
    UT3D_vc_perp2vc (&vxd, &vx1, &vx2);

    // Differenzvektor vcd = vom Punkt ii zum Punkt i1
    vl = fabs(vxd.dx + vxd.dy + vxd.dz);
    // printf(" dn[P0-P%d] vl=%f\n",i1,vl);
    if(vl > d1) {d1 = vl; ii2 = i1;}
  }
  printf(" ii1=%d ii2=%d\n",ii1,ii2);

  // damit Drehrichtung nicht umgedreht wird !!!!
  if(ii2 > ii1) {
    UT3D_vc_2pt (vc1, &pta[0], &pta[ii1]);
    UT3D_vc_2pt (vc2, &pta[0], &pta[ii2]);
  } else {
    UT3D_vc_2pt (vc1, &pta[0], &pta[ii2]);
    UT3D_vc_2pt (vc2, &pta[0], &pta[ii1]);
  }

  return 0;

}


/*
//====================================================================
  void UT3D_vc_ln (Vector *vc, Line *ln) {
//====================================================================
/// UT3D_vc_ln                Vector = LineStartPoint -> LineEndPoint


  // DEB_dump_obj__ (Typ_LN, ln, "UT3D_vc_ln");


  // --------------------------------------------------
  vc->dx = ln->p2.x - ln->p1.x;
  vc->dy = ln->p2.y - ln->p1.y;
  vc->dz = ln->p2.z - ln->p1.z;

  // printf("UT3D_vc_ln = %f, %f, %f\n", vc->dx, vc->dy, vc->dz);

  return;

}


//================================================================
  void UT3D_vc_2pt (Vector *vc, Point *p1, Point *p2) {
//================================================================
// UT3D_vc_2pt               Vector = Point -> Point

  // --------------------------------------------------
  vc->dx = p2->x - p1->x;
  vc->dy = p2->y - p1->y;
  vc->dz = p2->z - p1->z;

  // printf("UT3D_vc_pt_pt = %f, %f, %f\n", vc->dx, vc->dy, vc->dz);

  return;

}
*/

//=======================================================================
  void UT3D_vc_2ptlen (Vector *vc, Point *p1, Point *p2, double vlen) {
//=======================================================================
/// UT3D_vc_2ptlen            Vector = Point -> Point, set Length


  Vector vc1;

	UT3D_vc_2pt (&vc1, p1, p2);
  UT3D_vc_setLength (vc, &vc1, vlen);

}


//=======================================================================
  void UT3D_vc_angr (Vector *vc, double angr) {
//=======================================================================
/// UT3D_vc_angr              Vector = Angle_X_axis


  vc->dx = cos (angr);
  vc->dy = sin (angr);
  vc->dz = 0.;

	return;

}


//=======================================================================
  void UT3D_vc_2angr (Vector *vc, double angX, double angZ) {
//=======================================================================
/// \code
/// UT3D_vc_2angr             Vector = Angle_X_axis, Angle_Z_axis.
/// 
/// angX = Drehwinkel, angZ = Kippwinkel.
/// \endcode


  double  lXY;


  vc->dz = sin(angZ);
  lXY    = cos(angZ);


  // printf(" dz=%f lXY=%f\n",vc->dz,lXY);

  if(fabs(lXY) < UT_TOL_min1) {

    vc->dx = 0.;
    vc->dy = 0.;

  } else {
    vc->dx = cos(angX) * lXY;
    vc->dy = sin(angX) * lXY;
  }

  // printf("UT3D_vc_2angr %f,%f,%f;%f,%f\n",vc->dx,vc->dy,vc->dz,angX,angZ);

}


//=========================================================================
  void UT3D_vc_2vc (Vector *vco, Vector *vc1, Vector *vc2) {
//=========================================================================
/// \code
/// UT3D_vc_2vc            intermediate Vector
/// 
/// Vektoren muessen normiert sein !!!
/// Vektoren duerfen nicht antiparallel sein !!
/// AusgabeVektor nicht normiert !
/// \endcode

  

  vco->dx = (vc1->dx + vc2->dx) / 2.;
  vco->dy = (vc1->dy + vc2->dy) / 2.;
  vco->dz = (vc1->dz + vc2->dz) / 2.;


  // printf("ex UT3D_vc_2vc %f,%f,%f\n",vco->dx,vco->dy,vco->dz);

}


/*
//================================================================
 void UT3D_vc_invert (Vector *vco, Vector *vci ) {
//================================================================
// UT3D_vc_invert          Vector invert (change direction)

  vco->dx = -vci->dx;
  vco->dy = -vci->dy;
  vco->dz = -vci->dz;

  // printf("UT3D_vc_invert %f %f %f\n",vco->dx,vco->dx,vco->dx);

  return;

}

//================================================================
  int UT3D_vc_add2vc (Vector *v3, Vector *v1, Vector *v2) {
//================================================================
// UT3D_vc_add2vc        v3 = v1 + v2

  v3->dx = v1->dx + v2->dx;
  v3->dy = v1->dy + v2->dy;
  v3->dz = v1->dz + v2->dz;

  return 0;

}

//========================================================================
  int UT3D_vc_addvc2pt (Vector *vco, Vector *v1, Point *pv1, Point *pv2) {
//========================================================================
// UT3D_vc_addvc2pt         add Vector + Vector from 2 points

  vco->dx = v1->dx + pv2->x - pv1->x;
  vco->dy = v1->dy + pv2->y - pv1->y;
  vco->dz = v1->dz + pv2->z - pv1->z;

  return 0;

}

//================================================================
  int UT3D_vc_sub2vc (Vector *v3, Vector *v1, Vector *v2) {
//================================================================
// UT3D_vc_sub2vc        v3 = v1 - v2

  v3->dx = v1->dx - v2->dx;
  v3->dy = v1->dy - v2->dy;
  v3->dz = v1->dz - v2->dz;

  return 0;

}
*/

/* REPLACED by UT3D_ptvc_eval_ci_angr
//========================================================================
  int UT3D_vc_tng_ci_par1 (Vector *vs, Circ *ci1, double par1) {
//========================================================================
/// \code
/// UT3D_vc_tng_ci_par1        get tangent-vector to circ from parameter (0-1)
/// ACHTUNG: dzt nur 0. und 1. !!!
/// \endcode


  Vector vc1;


  // printf("UT3D_vc_tng_ci_par1 %f\n",u);
  // DEB_dump_obj__ (Typ_CI, ci1, "");


  // Vektor von Mitte nach aussen
  if(par1 <= 0.) {   // Startpunkt CI
    UT3D_vc_2pt (&vc1, &ci1->pc, &ci1->p1);
    // UT3D_vc_2pt (&vc1, &ci1->p1, &ci1->pc);

  } else if(par1 >= 1.) {   // Endpunkt CI
    // UT3D_vc_2pt (&vc1, &ci1->p2, &ci1->pc);
    UT3D_vc_2pt (&vc1, &ci1->pc, &ci1->p2);

  } else {
    TX_Error("UT3D_vc_tng_ci_par1 E001");
    return -1;
  }
  // DEB_dump_obj__ (Typ_VC, &vc1, " Vektor von Mitte:\n");


  // Startvektor = Kreuzprod. aus vc1 und ci1.vz
  UT3D_vc_perp2vc (vs, &vc1, &ci1->vz);
  // DEB_dump_obj__ (Typ_VC, vs, " Kreuzprod\n");


  // wenn der Rad. neg, umdrehen
  if(ci1->rad > 0.) UT3D_vc_invert (vs, vs);

  // DEB_dump_obj__ (Typ_VC, vs, "ex UT3D_vc_tng_ci_par1:\n");

  return 0;

}
*/
/* REPLACED with UTO_vc_tng_obj_par   2014-03-03
//=======================================================================
  int UT3D_vc_tangcvpar (Vector *vct, double lpar, int typ, void *data) {
//=======================================================================
/// \code
/// UT3D_vc_tangcvpar                tangent to point on circ
/// Input:
///   lpar       native parameter; can be < 0 > 1
/// \endcode

  double d1;
  Vector vp;
  Point  pt1;

  // printf("UT3D_vc_tangcvpar %d %f\n",typ,lpar);


  //-------------------------------
  if(typ == Typ_CVPOL) {
    UT3D_vc_evalplg (vct, data, lpar);


  //-------------------------------
  } else if(typ == Typ_CVELL) {
    d1 = UT3D_angr_par1_ell (lpar, data);
    UT3D_pt_eval_ell_par1 (&pt1, data, d1);
    UT3D_vc__pt_el (vct, &pt1, data);


  //-------------------------------
  } else if(typ == Typ_CVCLOT) {
    UT3D_ptvc_evparclot (&pt1, vct, 1, data, lpar);


  //-------------------------------
  } else if(typ == Typ_CVBSP) {
    UT3D_vc_evalparCv (vct, data, lpar);


  //-------------------------------
  } else {
    TX_Error("UT3D_vc_tangcvpar E001 %d",typ);
    return -1;
  }

  return 0;

}
*/

//=========================================================================
  int UT3D_pt_vc__par_ci (Point *pto, Vector *vco, Circ *ci1, double par) {
//=========================================================================
// UT3D_pt_vc__par_ci        get point/tangent-vector from parameter on circle
// Input:
//   pto        set NULL for no output
//   vct        set NULL for no output
//   par        parametervalue accord. pTyp
//
// Output:
//   pto        point; set NULL for no output
//   vct        tangent-vector; NULL for no output
//
// was UT3D_pt_vc__par_ci UT3D_ptvc_eval_ci_angr

  // double  ao, angr;
  double    dx, dy, rada;
  Vector    vc1, vx, vy;
  Point     pt1;



  // printf("UT3D_pt_vc__par_ci pTyp=%d par=%f\n",pTyp,par);


  // angle(rad) = par1 * opening-angle;
//   if(pTyp) {
    par *= ci1->ango;
//   }


//   if(!pTyp) {
//     // AP_debug__ ("UT3D_pt_vc__par_ci I001");
//     printf("*********** UT3D_pt_vc__par_ci I001  **************\n");
// // typ of parameter for circle always value 0-1;
// // make new function for get point/tangent from angle if necessary ..
//   }
  

  // test p1
  // (par==0): pto=ci1.p1;
  if(UTP_comp_0 (par)) {
    if(pto) *pto = ci1->p1;
    if(vco) {UT3D_vc_2pt (&vc1, &ci1->pc, &ci1->p1); goto L_vct;}
    goto L_ex;
  }

  // test p2
  // (par==ci1.ango): pto=ci1.p2;
  if(UTP_comp2db (par, ci1->ango, UT_TOL_min1)) {
    if(pto) *pto = ci1->p2;
    if(vco) {UT3D_vc_2pt (&vc1, &ci1->pc, &ci1->p2); goto L_vct;}
    goto L_ex;
  }

  // radius
  rada = fabs(ci1->rad);

  // get dx,dy for vector with angle par
  dx = cos(par);
  dy = sin(par);
    // printf(" dx=%lf dy=%lf\n",dx,dy);

  // get x-axis = ci.pc-ci.p1 / radius;
  UT3D_vc_2pt (&vx, &ci1->pc, &ci1->p1);
  UT3D_vc_div_d (&vx, &vx, rada);
    // DEB_dump_obj__ (Typ_VC, &vx, "  vx:");

  // get y-axis
  UT3D_vc_perp2vc (&vy, &ci1->vz, &vx);
    // DEB_dump_obj__ (Typ_VC, &vy, "  vy:");

  // creat point on circ: // multiply dx * radius
  if((pto)||(vco)) {
    dx *= rada;
    dy *= rada;
    // pt1 = add dx,dy (vector from par) to x-axis,y-axis of circ
    UT3D_pt_tra_pt_2vc_2par (&pt1, &ci1->pc, &vx, dx, &vy, dy);
  }

  if(pto) *pto = pt1;

  // get vc1 as vector ci.pc - pt1
  if(vco) UT3D_vc_2pt (&vc1, &ci1->pc, &pt1);

  // create normal-vector to vc1 according to sense-of-rotation of ci1
  L_vct:
  if(vco) {
    if(ci1->rad > 0.) {
      // rotate vector 90 deg CCW around axis
      UT3D_vc_perp2vc (vco, &ci1->vz, &vc1);

    } else {
      // rotate vector 90 deg CW around axis
      UT3D_vc_perp2vc (vco, &vc1, &ci1->vz);
    }
  }


  L_ex:

    // TESTBLOCK
    // if(pto) {
      // DEB_dump_obj__ (Typ_PT, pto, "ex UT3D_pt_vc__par_ci:");
      // GR_tDyn_symB__ (pto, SYM_STAR_S, ATT_COL_RED);
    // }
    // if(vco) {
      // DEB_dump_obj__ (Typ_VC, vco, "ex UT3D_pt_vc__par_ci:");
      // GR_tDyn_vc__ (vco, NULL, 9, 0);
    // }
    // END TESTBLOCK

  return 0;

}


/* replaced by UT3D_pt_vc__par_ci
//=========================================================================
  int UT3D_ptvc_eval_ci_angr (Point *pto, Vector *vct, Circ *ci1, double angr) {
//=========================================================================
/// \code
/// UT3D_ptvc_eval_ci_angr           get point/tangent to circ from opening-angle
/// Input:
///   angr         opening-angle to vector (ci1.pc - ci1.p1)
/// Output:
///   pto          point on circ (if *pto != NULL)
///   vco          tangent to circ (if *vct != NULL)
///
/// If angr=0.0  - the resulting point = ci1.p1; vector is normal to ci1.p1;
/// see also UT3D_pt_rotciangr UT3D_pt_vc__par_ci UT3D_vc_tng_ci_pt
/// \endcode


  double    dx, dy, rada;
  Vector    vc1, vx, vy;
  Point     pt1;


  // DEB_dump_obj__ (Typ_CI, ci1, "UT3D_ptvc_eval_ci_angr %lf",angr);

  // (angr==0): pto=ci1.p1;
  if(UTP_comp_0 (angr)) {
    if(pto) *pto = ci1->p1;
    UT3D_vc_2pt (&vc1, &ci1->pc, &ci1->p1);
    goto L_vct;
  }


  // (angr==ci1.ango): pto=ci1.p2;
  if(UTP_comp2db (angr, ci1->ango, UT_TOL_min1)) {
    if(pto) *pto = ci1->p2;
    UT3D_vc_2pt (&vc1, &ci1->pc, &ci1->p2);
    goto L_vct;
  }


  rada = fabs(ci1->rad);

  // get dx,dy for vector with angle angr
  dx = cos(angr);
  dy = sin(angr);
    // printf(" dx=%lf dy=%lf\n",dx,dy);


  // make x-axis=ci.pc-ci.p1; z-axis=ci.vz; get y-axis.
  UT3D_vc_2pt (&vx, &ci1->pc, &ci1->p1);
  UT3D_vc_div_d (&vx, &vx, rada);
  UT3D_vc_perp2vc (&vy, &ci1->vz, &vx);
    // DEB_dump_obj__ (Typ_VC, &vx, "  vx:");
    // DEB_dump_obj__ (Typ_VC, &vy, "  vy:");

  // pt1 = add dx,dy (vector from angr) to x-axis,y-axis of circ
  UT3D_pt_tra_pt_2vc_2par (&pt1, &ci1->pc, &vx, dx, &vy, dy);

  // get vc1 as vector ci.pc - pt1
  UT3D_vc_2pt (&vc1, &ci1->pc, &pt1);

  // creat point on circ: // multiply dx * radius
  if(pto) {
    dx *= rada;
    dy *= rada;
    // pt1 = add dx,dy (vector from angr) to x-axis,y-axis of circ
    UT3D_pt_tra_pt_2vc_2par (pto, &ci1->pc, &vx, dx, &vy, dy);
  }

  // create normal-vector to vc1 according to sense-of-rotation of ci1
  L_vct:
  if(vct) {
    UT3D_vc_perp2vc (vct, &ci1->vz, &vc1);
    if(ci1->ango < 0.)  UT3D_vc_invert (vct, vct);
  }


    // TESTS ONLY:
    // if(pto) {
      // DEB_dump_obj__ (Typ_PT, pto, "ex UT3D_ptvc_eval_ci_angr:");
      // GR_tDyn_symB__ (pto, SYM_STAR_S, ATT_COL_RED);
    // }
    // if(vct) {
      // DEB_dump_obj__ (Typ_VC, vct, "ex UT3D_ptvc_eval_ci_angr:");
      // GR_tDyn_vc__ (vct, NULL, 9, 0);
    // }

  return 0;

}
*/

//====================================================================
   int UT3D_vc_tng_crv_pt (Vector *vco,
                          Point *pti, int typ, void *data) {
//====================================================================
/// \code
/// UT3D_vc_tng_crv_pt           vector tangent to curve
/// Input:
///   typ      type of curve 
///   data     curve-data
///   retCode  0   OK
///           -1   Error  
///
/// see UT3D_pt_vc__par_cv
/// \endcode

/*
UT3D_vc_evalparCv
UCV_EvPtTgCltCrv
UT3D_vc_evalplg
USBS_TgVecIsoBspSur
*/

  int        irc, cTyp;
  double     d1;
  char       *cp1;
  Point      pt1;


  // printf("UT3D_vc_tng_crv_pt typ=%d\n",typ);
    // DEB_dump_obj__ (Typ_PT, pti, "pti=");


  //----------------------------------------------------------------
  if(typ == Typ_LN) {
    UT3D_vc_ln (vco, (Line*)data);
    return 0;


  //----------------------------------------------------------------
  } else if(typ == Typ_CI) {
    return UT3D_vc_tng_ci_pt (vco, pti, data);


  //----------------------------------------------------------------
  } else if(typ == Typ_CVELL) {
    return UT3D_vc__pt_el (vco, pti, data);


  //----------------------------------------------------------------
  } else if(typ == Typ_CVBSP) {
    // get par from point on curve
    irc = UT3D_par_pt__pt_cvbsp (&d1, NULL, NULL, data, pti);
    if(irc < 0) return irc;
    // get tangent from par
    return UT3D_vc_evalparCv  (vco, data, d1);


  //----------------------------------------------------------------
  } else if(typ == Typ_CVPOL) {
    // get knotVal from point
    irc = UT3D_par_pt__plg_pt (&d1, NULL, NULL, pti, data, UT_TOL_pt);
    if(irc < 0) return irc;
    return UT3D_vc_evalplg (vco, data, d1);


  //----------------------------------------------------------------
  } else if(typ == Typ_CVCLOT) {
    irc = UT3D_par_clotpt (&d1, pti, data, UT_DISP_cv);
    if(irc < 0) return irc;
    return UT3D_ptvc_evparclot (&pt1, vco, 1, data, d1);


  //----------------------------------------------------------------
  } else if(typ == Typ_CVTRM) {
    // get standardCurve from trimmedCurve
    cp1 = (char*) MEM_alloc_tmp (OBJ_SIZ_MAX);
    UTO_cv_cvtrm (&cTyp, cp1, NULL, (CurvCCV*)data);
    // recursion
    return UT3D_vc_tng_crv_pt (vco, pti, cTyp, cp1);


  //----------------------------------------------------------------
  } else goto L_NYI;




  return 0;




  //----------------------------------------------------------------
  L_NYI:
    TX_Error("not yet implemented - UT3D_vc_tng_crv_pt E001 %d",typ);
    return -1;

}


//================================================================
  int UT3D_vc_tng_ci_pt (Vector *vct, Point *p1, Circ *ci) {
//================================================================
/// UT3D_vc_tng_ci_pt         tangent-vector to circ (0. - 1.) from point on circ

  Vector vy;


  // DEB_dump_obj__ (Typ_PT, p1, "UT3D_vc_tng_ci_pt");
  // DEB_dump_obj__ (Typ_CI, ci, "");

  // get vy = p1 -> pc
  UT3D_vc_2pt (&vy, p1, &ci->pc);
  // UT3D_vc_setLength (&vy, &vy, 1.);  nicht notwendig ..
    // GR_tDyn_vc__ (&vy, p1, 2, 0);  // TestDisp

  // vektor = Kreuzprod. aus vc und ci1.vz
  UT3D_vc_perp2vc (vct, &vy, &ci->vz);
    // GR_tDyn_vc__ (vct, p1, 5, 0);  // TestDisp
    // DEB_dump_obj__ (Typ_VC, vct, " vt:");


  // wenn der Rad. neg, umdrehen
  // if(ci->rad > 0.) UT3D_vc_invert (vct, &vt);
  if(ci->rad < 0.) {
    UT3D_vc_invert (vct, vct);
  }


    // DEB_dump_obj__ (Typ_VC, vct, "ex UT3D_vc_tng_ci_pt:");
    // GR_tDyn_vc__ (vct, p1, 2, 0);  // TestDisp

  return 0;

}


//================================================================
  int UT3D_vc_Zup (Vector *v2, Vector *v1) {
//================================================================
/// invert Vector if its Z-coord is negative
// wenn v1 einen negativen Z-Anteil hat: invertieren.


  if(v1->dz < 0.) {
    UT3D_vc_invert (v2, v1);
  } else {
    if(v2 != v1) *v2 = *v1;
  }

  return 0;

}


//======================================================================
  void UT3D_vc_perp1vc (Vector *vp, Vector *vi) {
//======================================================================
/// UT3D_vc_perp1vc          normal vector to 3D-vector in X-Y-plane
/// 
// UT3D_vc_perp1vc          Author: Thomas Backmeister         3.3.2003
// 
/// Computation of a normal vector (not necessarily of length 1) to a 3D-vector.
/// Liefert den Y-Vektor !!!
/// 
/// IN:
///   Vector *vi ... 3D-vector
/// OUT:
///   Vector *vp ... vector normal to the input vector


  // DEB_dump_obj__ (Typ_VC, vi, "UT3D_vc_perp1vc");


  // irgendein Vektor: Normalvektor dazu in der X-Y-Ebene machen
  // if ((vi->dx != 0.) || (vi->dy != 0.)) {   // FEHLER bei -0.0; korr RF
  if ((fabs(vi->dx) > UT_TOL_min1) ||
      (fabs(vi->dy) > UT_TOL_min1)) {
    vp->dx = -vi->dy;
    vp->dy = vi->dx;
    vp->dz = 0.;


  // reiner Z-Vektor: einen Vektor in Y-Richtung in der X-Y-Ebene
  // } else if (vi->dz != 0.) {             // FEHLER bei -0.0; korr RF
  } else if (fabs(vi->dz) > UT_TOL_min1) {
    vp->dx = 0.;
    vp->dy = vi->dz;
    vp->dz = -vi->dy;


  // 0,0,0  -> 0,0,1
  } else {                 
    *vp = UT3D_VECTOR_X;
  }


  // DEB_dump_obj__ (Typ_VC, vp, "ex UT3D_vc_perp1vc");

  return;
}


//========================================================================
  void UT3D_vc_perp2vc (Vector *vp, Vector *v1, Vector *v2) {
//========================================================================
/// \code
/// UT3D_vc_perp2vc            vector perpendic. to 2 vectors (angle < PI only)
///   NO OUTPUT if v1-v2 is parallel or antiparallel (can BE 0,0,0)
///
/// Output:
///   vp       vector normal to v1 and v2;
///            length = 1 only if v1 and v2 are normalized AND have angle = 90 deg
/// 
/// 
/// Get VZ from VX, VY:    UT3D_vc_perp2vc (&vz, &vx, &vy);
/// Get VY from VZ, VX:    UT3D_vc_perp2vc (&vy, &vz, &vx);
/// Get VX from VY, VZ:    UT3D_vc_perp2vc (&vx, &vy, &vz);
///
/// Get -VZ from VX, VY:    UT3D_vc_perp2vc (&vz, &vy, &vx);
/// Get -VY from VZ, VX:    UT3D_vc_perp2vc (&vy, &vx, &vz);
/// Get -VX from VY, VZ:    UT3D_vc_perp2vc (&vx, &vz, &vy);
///
/// Rotation 90 deg CCW:
///   get vp = v2 rotated 90 deg CCW around axis v1
///
/// Rotation 90 deg CW:
///   get vp = v1 rotated 90 deg CW around axis v2
///
///
///    vp (z)
///     |    /v2 (y)
///     |   / 
///     |  /                  
///     | / 
///     x----------v1 (x)      angle v1 v2 < PI
///      
///           
///        /v1 (x)
///       / 
///      / 
///     x----------v2 (y)       angle v1 v2 > PI
///     |
///     |
///     |
///    vp (z)
///     
/// \endcode


  Vector vn;

    // DEB_dump_obj__ (Typ_VC, v1, " _perp2vc v1");
    // DEB_dump_obj__ (Typ_VC, v2, " _perp2vc v2");

  // Vergleich Input- Outputadressen
  // if ((v1 == vp) || (v2 == vp)) {
    vn.dx = v1->dy * v2->dz - v1->dz * v2->dy;
    vn.dy = v1->dz * v2->dx - v1->dx * v2->dz;
    vn.dz = v1->dx * v2->dy - v1->dy * v2->dx;
    *vp = vn;
  // } else {
    // vp->dx = v1->dy * v2->dz - v1->dz * v2->dy;
    // vp->dy = v1->dz * v2->dx - v1->dx * v2->dz;
    // vp->dz = v1->dx * v2->dy - v1->dy * v2->dx;
  // }

    // DEB_dump_obj__ (Typ_VC, &vn, "ex UT3D_vc_perp2vc vn");
    // DEB_dump_obj__ (Typ_VC, vp, "ex UT3D_vc_perp2vc vp");

}


//========================================================================
  int UT3D_vc_perpvc2pt (Vector *vp, Vector *vx, Point *pc, Point *py) {
//========================================================================
/// \code
/// UT3D_vc_perpvc2pt         vector = perpendic. to vec & 2 points (crossprod)
/// 
/// Liefert den Z-Vektor von X-Vektor, Y-AchsStartpunkt und Y-AchsEndpunkt
/// Get VZ from VX, PY1, PY2:    UT3D_vc_perp3pt (&vz, &vx, &pya, &pye);
/// Get VY from VZ, PX1, PX2:    UT3D_vc_perp3pt (&vy, &vz, &pxa, &pxe);
/// \endcode

  double  dyx,dyy,dyz;


  // DEB_dump_obj__ (Typ_VC, vx, "UT3D_vc_perpvc2pt-vx:");
  // DEB_dump_obj__ (Typ_PT, pc, "  pc:");
  // DEB_dump_obj__ (Typ_PT, py, "  py:");


  dyx = py->x - pc->x;
  dyy = py->y - pc->y;
  dyz = py->z - pc->z;
    // printf("  vy=%f %f %f\n",dyx,dyy,dyz);

  vp->dx = vx->dy * dyz - vx->dz * dyy;
  vp->dy = vx->dz * dyx - vx->dx * dyz;
  vp->dz = vx->dx * dyy - vx->dy * dyx;

  // DEB_dump_obj__ (Typ_VC, vp, "ex UT3D_vc_perpvc2pt:");

  return 0;

}


//================================================================
  int UT3D_vc_perp_npt (Vector *vn, Point *pa, int pNr) {
//================================================================
// UT3D_vc_perp_npt         vector = perpendic. to n points
// NEWELL'S METHOD FOR COMPUTING THE PLANE EQUATION OF A POLYGON
// see also UT3D_vc_perp3pt


  int     ii, i1;
  Point   *u, *v;


  vn->dx = vn->dy = vn->dz = 0.;

  for(ii = 0; ii < pNr; ++ii) {
    u = &pa[ii];
    i1 = (ii + 1) % pNr;
    v = &pa[i1];
      printf(" ii=%d i1=%d\n",ii,i1);
    vn->dx += (u->y - v->y) * (u->z + v->z);
    vn->dy += (u->z - v->z) * (u->x + v->x);
    vn->dz += (u->x - v->x) * (u->y + v->y);
  }

    printf(" vn %lf %lf %lf\n",vn->dx,vn->dy,vn->dz);


  return 0;

}


//==========================================================================
  int UT3D_vc_perp3pt (Vector *vp, Point *ptc, Point *ptx, Point *pty) {
//==========================================================================
/// \code
/// UT3D_vc_perp3pt           vector = perpendic. to 3 points (crossprod)
/// 
/// Liefert den Z-Vektor von Origin, X-AchsEndpunkt und Y-AchsEndpunkt
/// Get VZ from PO, PX, PY:    UT3D_vc_perp3pt (&vz, &po, &px, &py);
/// Get VY from PO, PX, PZ:    UT3D_vc_perp3pt (&vy, &po, &pz, &px);
/// \endcode


  double dxx,dxy,dxz,dyx,dyy,dyz;


  dxx = ptx->x - ptc->x;
  dxy = ptx->y - ptc->y;
  dxz = ptx->z - ptc->z;

  dyx = pty->x - ptc->x;
  dyy = pty->y - ptc->y;
  dyz = pty->z - ptc->z;

  vp->dx = dxy * dyz - dxz * dyy;
  vp->dy = dxz * dyx - dxx * dyz;
  vp->dz = dxx * dyy - dxy * dyx;

  return 0;

}


//=============================================================================
  int UT3D_vc_perp4pt (Vector *vp, Point *p1, Point *p2, Point *p3, Point *p4) {
//=============================================================================
/// \code
/// UT3D_vc_perp4pt         vector = perpendic. to 2 lines (crossprod)
/// 
/// Get VY from VZ, PX1, PX2:    UT3D_vc_perp3pt (&vy, &vz, &pxa, &pxe);
/// \endcode

  Vector   vc1, vc2;


  // DEB_dump_obj__ (Typ_PT, pc, "  pc:");

  UT3D_vc_2pt (&vc1, p1, p2);
  UT3D_vc_2pt (&vc2, p3, p4);
  UT3D_vc_perp2vc (vp, &vc1, &vc2);

  // DEB_dump_obj__ (Typ_VC, vp, "ex UT3D_vc_perp4pt:");

  return 0;

}


//======================================================================
  void UT3D_vc_perpvcplXY (Vector *vx, Vector *vc1) {
//======================================================================
///  UT3D_vc_perpvcplXY       Normalvektor auf vc1, der in der X-Y-Plane liegt.
///  returns vx from vz; vz,vy gives vx. vx gives vy.
/*

  // Tests:
  UT3D_vc_3db (&vc1, 0.,0.,1.);
  UT3D_vc_perpvcplXY (&vx, &vc1);

  UT3D_vc_3db (&vc1, 0.,1.,0.);
  UT3D_vc_perpvcplXY (&vx, &vc1);

  UT3D_vc_3db (&vc1, 1.,0.,0.);
  UT3D_vc_perpvcplXY (&vx, &vc1);

  UT3D_vc_3db (&vc1, 1.,1.,0.);
  UT3D_vc_perpvcplXY (&vx, &vc1);

  UT3D_vc_3db (&vc1, 1.,1.,1.);
  UT3D_vc_perpvcplXY (&vx, &vc1);

  UT3D_vc_3db (&vc1, -1.,1.,1.);
  UT3D_vc_perpvcplXY (&vx, &vc1);

  UT3D_vc_3db (&vc1, -1.,-1.,1.);
  UT3D_vc_perpvcplXY (&vx, &vc1);

  UT3D_vc_3db (&vc1, -1.,-1.,-1.);
  UT3D_vc_perpvcplXY (&vx, &vc1);
*/

  /*
  printf("");
  printf("UT3D_vc_perpvcplXY     %f,%f,%f",vc1->dx,vc1->dy,vc1->dz);
  */


  /* x == 0; Vektor liegt in d. Ebene YZ. */
  if (UTP_comp_0 (vc1->dx)) {
    vx->dx = 1.;
    vx->dy = 0.;
    vx->dz = 0.;


  /* y == 0; Vektor liegt in d. Ebene XZ. */
  } else if (UTP_comp_0 (vc1->dy)) {
    vx->dx = 0.;
    vx->dy = 1.;
    vx->dz = 0.;


/*
  // z == 0; Vektor liegt in d. Ebene XY.
  } else if (UTP_comp_0 (vc1->dz)) {
    vx->dx = 0.;
    vx->dy = 0.;
    vx->dz = 1.;
*/

  } else {
    vx->dx = -vc1->dy;
    vx->dy = vc1->dx;
    vx->dz = 0.0;
    UT3D_vc_setLength (vx, vx, 1.);
  }

  /* printf(" ex UT3D_vc_perpvcplXY %f,%f,%f",vx->dx,vx->dy,vx->dz); */

}


//================================================================
  int UT3D_vc_perpcv (Vector *vcz, int pNr, Point *pTab) {
//================================================================
/// \code
/// USE UT3D_vc_perppta
/// UT3D_vc_perpcv         Normalvektor auf planares Polygon
/// ACHTUNG: liefert NICHT immer im gleichen Umlaufsinn ! Korrigieren !!!!
/// \endcode

  Plane   pl1;

  // *vcz = UT3D_VECTOR_Z;

  // Plane durch Polygon
  UT3D_pl_pta (&pl1, pNr, pTab);

  // *pto = pl1.po;
  *vcz = pl1.vz;

  // DEB_dump_obj__(Typ_VC, vcz, "ex UT3D_vc_perpcv");

  return 0;

}


//======================================================================
  int UT3D_vc_perp_npt_bp_ix (Vector *vcn, Point *pa, int *ia, int bp) {
//======================================================================
/// \code
/// get normalvector plb->vz for polygon from backPlane and extreme-points
/// Input:
///   ia[6] index to extreme-points in pa; get it from UT3D_box_ix_npt
///   bp    backPlane (eg from UT3D_bp_perp_2pt)
/// Output
///   vcn
/// \endcode


  double d1, d2;
  Vector vx1, vx2;

  if(bp == 0) {                 // BCKPLN_YZ
    // X-vector must be 0
    d1 = pa[ia[0]].x;
    d2 = pa[ia[3]].x;
    if(fabs(d1-d2) < UT_DISP_cv) { *vcn = UT3D_VECTOR_X; goto L_exit;}
    // get 2 vectors from extreme points ..
    UT3D_vc_2pt (&vx1, &pa[ia[1]], &pa[ia[4]]);
    UT3D_vc_2pt (&vx2, &pa[ia[2]], &pa[ia[5]]);

  } else if(bp == 1) {          // BCKPLN_XZ
    // Y-vector must be 0
    d1 = pa[ia[1]].y;
    d2 = pa[ia[4]].y;
    if(fabs(d1-d2) < UT_DISP_cv) { *vcn = UT3D_VECTOR_Y; goto L_exit;}
    // get 2 vectors from extreme points ..
    UT3D_vc_2pt (&vx1, &pa[ia[0]], &pa[ia[3]]);
    UT3D_vc_2pt (&vx2, &pa[ia[2]], &pa[ia[5]]);

  } else if(bp == 2) {          // BCKPLN_XY
    // Z-vector must be 0
    d1 = pa[ia[2]].z;
    d2 = pa[ia[5]].z;
    if(fabs(d1-d2) < UT_DISP_cv) { *vcn = UT3D_VECTOR_Z; goto L_exit;}
    // get 2 vectors from extreme points ..
    UT3D_vc_2pt (&vx1, &pa[ia[0]], &pa[ia[3]]);
    UT3D_vc_2pt (&vx2, &pa[ia[1]], &pa[ia[4]]);
  }

    // DEB_dump_obj__ (Typ_VC, &vx1, "MaxVec1=");
    // DEB_dump_obj__ (Typ_VC, &vx2, "MaxVec2=");

  // den Normalvektor erzeugen - nicht normiert;
  // ohne Beruecksichtigung des Umdrehungssinnes.
  UT3D_vc_perp2vc (vcn, &vx1, &vx2);

  L_exit:
    // DEB_dump_obj__ (Typ_VC, vcn, "ex UT3D_vc_perp_npt_bp_ix");
  return 0;

}


//================================================================
  int UT3D_vc_perppta (Vector *vcn, int pNr, Point *pta) {
//================================================================
/// \code
/// UT3D_vc_perppta           get normalvector for polygon for rot.sense CCW
/// Normalvector vcn always for sense-of-rotation CCW !
/// Input
///   pNr         nr of points in pta, including first = last point.
///   pta[pNr]    polygon; first and last point must be ident.
/// Output
///   vcn
/// see also UT3D_vcn_cvo UT3D_vcn_cvtrm UT3D_ptvc_sur UT3D_ptvc_sus
/// see also UT3D_ChkPlanPts
/// \endcode

// see also UT3D_pl_pta


  int      i1, ii1, ii2, ibp;
  double   d1, vl;
  Vector   vx1, vx2, vxd;



  if(pNr < 4) {
    if(pNr < 3) goto L_err;
    // eigentlich sollte 1. / letzter Punkt ident sein ..
    if(UT3D_comp2pt(&pta[0],&pta[2],UT_TOL_pt) == 1) goto L_err;
    // den Normalvektor erzeugen - nicht normiert;
    // Umdrehungssinn passt immer; kein konvex moeglich !
    UT3D_vc_perp3pt (vcn, &pta[0], &pta[1], &pta[2]);
    goto L_fertig;
  }


  //----------------------------------------------------------------
  // get 2 vecs of max extent of a polgon (for perp.Vec)
  // 2 normal aufeinander-stehende Vektoren eines Polygon suchen
  // (fuer Normalvektor auf Polygon)

  // zuerst den laengsten Vektor suchen;
  ii1 = -1;
  d1 = 0.;
  for (i1=1; i1<pNr; ++i1) {
    vl = fabs(pta[0].x - pta[i1].x +
              pta[0].y - pta[i1].y +
              pta[0].z - pta[i1].z);
    // printf(" P[%d] vl=%f\n",i1,vl);
    if(vl > d1) {d1 = vl; ii1 = i1;}
  }
  // printf(" ii1=%d\n",ii1);
  if(ii1 < 0) return -1;  // alle Punkte sind ident !

  // longest vec vc1 = p[0] --> p[ii1]
  UT3D_vc_2pt (&vx1, &pta[0], &pta[ii1]);

  // vec mit max. Normalabst zu vx1 suchen
  d1 = 0.;
  ii2 = 0;
  for (i1=1; i1<pNr; ++i1) {
    if(i1 == ii1) continue;
    UT3D_vc_2pt (&vx2, &pta[0], &pta[i1]);
    // size = Normalabstand !
    UT3D_vc_perp2vc (&vxd, &vx1, &vx2);

    // Differenzvektor vcd = vom Punkt ii zum Punkt i1
    vl = fabs(vxd.dx) + fabs(vxd.dy) + fabs(vxd.dz);
    // printf(" dn[P0-P%d] vl=%f\n",i1,vl);
    if(vl > d1) {d1 = vl; ii2 = i1;}
  }
  // printf(" ii1=%d ii2=%d\n",ii1,ii2);
  if(ii2 < 1) {
    // printf("*** UT3D_vc_perppta I001\n");
    // for(i1=0;i1<pNr;++i1) DEB_dump_obj__ (Typ_PT, &pta[i1], "   P[%d] ",i1);
    return -1;
  }

  UT3D_vc_2pt (&vx1, &pta[0], &pta[ii1]);
  UT3D_vc_2pt (&vx2, &pta[0], &pta[ii2]);
  // DEB_dump_obj__ (Typ_VC, &vx1, "MaxVec1=");
  // DEB_dump_obj__ (Typ_VC, &vx2, "MaxVec2=");

  // den Normalvektor erzeugen - nicht normiert;
  // noch ohne Beruecksichtigung des Umdrehungssinnes.
  UT3D_vc_perp2vc (vcn, &vx1, &vx2);


  //----------------------------------------------------------------
  // fuer den Umdrehungssinn braucht man eine lueckenlose Punktekette - alle !
  ibp = UT3D_bp_perp_vc (NULL, vcn);
  // rotSense;
  i1 = UT3D_sr_polc (pNr, pta, vcn, ibp);
  // printf(" sr=%d\n",i1);

  if(i1 < 0) UT3D_vc_invert (vcn, vcn);


  L_fertig:
  UT3D_vc_setLength (vcn, vcn, 1.);
  // DEB_dump_obj__ (Typ_VC, vcn, "nVC=");

  return 0;


  L_err:
  // printf("UT3D_vc_perppta I001\n");  // recht viele !!
  // for(i1=0;i1<pNr; ++i1) DEB_dump_obj__ (Typ_PT, &pta[i1],"  P[%d]=",i1);
  return -1;

}


/*
Version 2 - auch Mist
//================================================================
  int UT3D_vc_perppta (Vector *vcn, int pNr, Point *pa) {
//================================================================
// Normalvektor auf Polygon
// die indexe von 3 opposite Points; ueber diese dann 2 Vektoren spannen.
// see UT3D_box_pts ..

  int     i1, ii0, ii1, ii2, ixmin, ixmax, iymin, iymax, izmin, izmax;
  double  d1, dmax;
  Vector  vc1, vc2;
  Point   p1;


  // printf("UT3D_vc_perppta %d\n",pNr);
  // GR_tDyn_npt__ (pNr, pa, SYM_STAR_S, 0);
  // GR_tDyn_pcv (pa, pNr, 9);


  // indices zu den opposite points suchen ..
  ixmin = 0;
  iymin = 0;
  izmin = 0;
  ixmax = 0;
  iymax = 0;
  izmax = 0;

  // box ueber alle punkte ..
  for (i1=1; i1<pNr; ++i1) {          // see UT3D_box_pts
    if (pa[i1].x < pa[ixmin].x) ixmin = i1;
    if (pa[i1].y < pa[iymin].y) iymin = i1;
    if (pa[i1].z < pa[izmin].z) izmin = i1;
    if (pa[i1].x > pa[ixmax].x) ixmax = i1;
    if (pa[i1].y > pa[iymax].y) iymax = i1;
    if (pa[i1].z > pa[izmax].z) izmax = i1;
  }
    // printf(" xyz %d %d %d %d %d %d\n",ixmin,ixmax,iymin,iymax,izmin,izmax);


  // vec from boxpoints (Vec von links unten nach rechts oben ..)
  vc1.dx = pa[ixmax].x - pa[ixmin].x;
  vc1.dy = pa[iymax].y - pa[iymin].y;
  vc1.dz = pa[izmax].z - pa[izmin].z;
    // dieser vec NUR fuer Bildung der backplane !!


  // make backplane-vector
  if(vc1.dx > vc1.dy) {   // dx | dz
    if(vc1.dx > vc1.dz) {   // dx
      ii0 = ixmin;
      ii1 = ixmax;
    } else {                // dz
      ii0 = izmin;
      ii1 = izmax;
    }

  } else {                // dy | dz
    if(vc1.dy > vc1.dz) {   // dy
      ii0 = iymin;
      ii1 = iymax;
    } else {                // dz
      ii0 = izmin;
      ii1 = izmax;
    }
  }
  // der laengste Vec geht von pa[ii0] --> pa[ii1]
    // GR_tDyn_symB__ (&pa[ii0], SYM_TRI_S, 3);
    // GR_tDyn_symB__ (&pa[ii1], SYM_TRI_S, 3);


  // den Punkt mit dem hoechsten Normalabst von pa[ii0] --> pa[ii1] ermitteln
  // vc1 = pa[ii0] --> pa[ii1]; normieren !
  UT3D_vc_2pt (&vc1, &pa[ii0], &pa[ii1]);
  UT3D_vc_setLength (&vc1, &vc1, 1.);
  dmax = 0.;
  ii2 = 0;
  for(i1=0; i1<pNr; ++i1) {
    if(i1 == ii0) continue;
    if(i1 == ii1) continue;
    // den hoechsten sqareAbst errechnen (UT3D_nlen_projvcvc UT3D_pt_ck_linear)
    // poject point i1 -> vector ii0 --> ii1
    // vc2 = pa[ii0] --> pa[i1]
    UT3D_vc_2pt (&vc2, &pa[ii0], &pa[i1]);
    d1 = vc1.dx*vc2.dx + vc1.dy*vc2.dy + vc1.dz*vc2.dz;
    p1.x = pa[ii0].x + d1*vc1.dx;
    p1.y = pa[ii0].y + d1*vc1.dy;
    p1.z = pa[ii0].z + d1*vc1.dz;
      // GR_tDyn_symB__ (&p1, SYM_TRI_S, 4);
      // printf(" %d p1=%f %f %f\n",i1,p1.x,p1.y,p1.z);

    d1 = UT3D_lenq_PtPt (&pa[i1], &p1);
    // printf(" %d d1=%f\n",i1,d1);
    if(d1 > dmax) { dmax = d1; ii2 = i1; }

  }
    // printf(" ii2=%d\n",ii2);
    // GR_tDyn_symB__ (&pa[ii2], SYM_TRI_S, 4);


  // nun gibts 2 Vektoren; Normalvektor drauf.
  UT3D_vc_2pt (&vc2, &pa[ii0], &pa[ii2]);
  UT3D_vc_perp2vc (vcn, &vc1, &vc2);

  // Vektor umdrehen, wenn i2 < i1
  if(ii2 < ii1) UT3D_vc_invert (vcn, vcn);

  UT3D_vc_setLength (vcn, vcn, 1.);
    // GR_tDyn_vc__ (vcn, &p1, 2, 0);



  // test ob Punktekette planar:
  // test die Normalabstaende der Punkte von der Plane


  return 0;

}
*/

/* alte - fehlerhafte Loesung:

//================================================================
  int UT3D_vc_perppta (Vector *vcn, int pNr, Point *pa) {
//================================================================
// Normalvektor auf Polygon
// die indexe von 4 opposite Points; ueber diese dann 2 Vektoren spannen.
// see UT3D_box_pts ..

  int     bp, i1, ii0, ii1, ii2, ixmin, ixmax, iymin, iymax, izmin, izmax;
  double  d1, dmax;
  Vector  vcb, vc1, vc2;
  Vector2 v21;
  Point   p1, p2;
  Point2  p21, p22;


  // GR_tDyn_npt__ (pNr, pa, SYM_STAR_S, 0);


  // indices zu den opposite points suchen ..
  ixmin = 0;
  iymin = 0;
  izmin = 0;
  ixmax = 0;
  iymax = 0;
  izmax = 0;

  for (i1=1; i1<pNr; ++i1) {          // see UT3D_box_pts
    if (pa[i1].x < pa[ixmin].x) ixmin = i1;
    if (pa[i1].y < pa[iymin].y) iymin = i1;
    if (pa[i1].z < pa[izmin].z) izmin = i1;
    if (pa[i1].x > pa[ixmax].x) ixmax = i1;
    if (pa[i1].y > pa[iymax].y) iymax = i1;
    if (pa[i1].z > pa[izmax].z) izmax = i1;
  }
  // printf(" xyz %d %d %d %d %d %d\n",ixmin,ixmax,iymin,iymax,izmin,izmax);


  // vec from boxpoints (Vec von links unten nach rechts oben ..)
  vcb.dx = pa[ixmax].x - pa[ixmin].x;
  vcb.dy = pa[iymax].y - pa[iymin].y;
  vcb.dz = pa[ixmax].z - pa[ixmin].z;


  // backplane from vector
  bp = UT3D_bp_perp_vc (&vcb);


  // nun 2 opposite 2D-vecs bilden
  if(bp == BCKPLN_XY) {

    // wenn (vcb.dx > vcb.dy) dann flach
    if(vcb.dx > vcb.dy) {
      ii0 = ixmin; 
      ii1 = ixmax; 
    } else {
      ii0 = iymin; 
      ii1 = iymax; 
    }


  } else if(bp == BCKPLN_XZ) {

    if(vcb.dx > vcb.dz) {
      ii0 = ixmin; 
      ii1 = ixmax; 
    } else {
      ii0 = izmin; 
      ii1 = izmax; 
    }


  } else if(bp == BCKPLN_YZ) {

    if(vcb.dy > vcb.dz) {
      ii0 = iymin; 
      ii1 = iymax; 
    } else {
      ii0 = izmin; 
      ii1 = izmax; 
    }


  } else {
    TX_Error("UT3D_vc_perppta E001 %d",bp);
    return -1;
  }


  // Punkte des Startvektor
  p1 = pa[ii0];
  p2 = pa[ii1];

  // vc1 ist nun der wahrscheinl laengster vec in der Kontur.
  UT3D_vc_2pt (&vc1, &p1, &p2);
    // GR_tDyn_vc__ (&vc1, &p1, 2, 1);

  // den Punkt mit dem hoechsten Normalabst dazu ermitteln
  UT2D_PT_PT3BP (&p21, &p1, bp);   // startpt
  UT2D_vc_vc3_bp (&v21, &vc1, bp);  // startvec vec in 2D
    // GR_Disp_vc2 (&v21, &p21, 2, 1);

  dmax = 0;

  for (i1=0; i1<pNr; ++i1) {
    UT2D_PT_PT3BP (&p22, &pa[i1], bp);
    // Normalabstand
    d1 = fabs(UT2D_slen_norvc2pt (&v21, &p21, &p22));
    if(d1 > dmax) { dmax = d1; ii2 = i1; }
    // DEB_dump_obj__ (Typ_PT2, &pa[i1], " d1=%f P[%d]",d1,i1);
  }


  // ii2 ist der Punkt mit dem groessten Normalabstand
  printf(" ii1=%d ii2=%d\n",ii1,ii2);
  UT3D_vc_2pt (&vc2, &p1, &pa[ii2]);
    // GR_tDyn_vc__ (&vc2, &p1, 2, 1);

  // nun gibts 2 Vektoren; Normalvektor drauf.
  UT3D_vc_perp2vc (vcn, &vc1, &vc2);

  if(ii1 < ii2) UT3D_vc_invert (vcn, vcn);
  // if(ii1 < ii2) UT3D_vc_invert (vcn, vcn);

  UT3D_vc_setLength (vcn, vcn, 1.);
  // GR_tDyn_vc__ (vcn, &p1, 2, 0);

  return 0;

}
*/

//==================================================================
  void UT3D_vc_normalize (Vector *vn, Vector *vc) {
//==================================================================
/// \code
/// UT3D_vc_normalize         change to length 1
/// 
/// vn = vc = OK.
/// \endcode


  UT3D_vc_setLength (vn, vc, 1.);

  return;
}


//====================================================================
  void UT3D_vc_setLength (Vector *vco, Vector *vci, double new_len) {
//====================================================================
/// \code
/// UT3D_vc_setLength               change vectorlength
/// 
/// vco und vci duerfen ident sein
/// 
/// use UT3D_vc_multvc with vectors of length=1
/// \endcode


  double len;

  /* printf("UT2D_vc_setLength %f %f %f %f\n", */
                   /* vci->dx,vci->dy,vci->dz,new_len); */

  len = UT3D_len_vc (vci);

  if (len != 0.0)
    {
      len = new_len / len;
      vco->dx = vci->dx*len;
      vco->dy = vci->dy*len;
      vco->dz = vci->dz*len;
      /* printf("UT3D_vc_setLength %f %f %f\n",vco->dx,vco->dy,vco->dz); */
    }
  else
    {
      // printf("**** UT3D_vc_setLength error\n");
      *vco = UT3D_VECTOR_X;
    }

}


//=====================================================================
  int UT3D_vc_setLenLen (Vector *vco,
                         Vector *vci, double newLen, double actLen) {
//=====================================================================
/// UT3D_vc_setLenLen               change vectorlength; actLen known.

  double len;

  if(actLen != 0.0) {
      len = newLen / actLen;
      vco->dx = vci->dx*len;
      vco->dy = vci->dy*len;
      vco->dz = vci->dz*len;
      // printf("UT3D_vc_setLenLen %f %f %f\n",vco->dx,vco->dy,vco->dz);
      return 0;

  } else {
      *vco = UT3D_VECTOR_X;
      return -1;
  }

}


//================================================================
  int UT3D_vc_rotangr (Vector *vco, Vector *vci, double *ar) {
//================================================================
/// \code
/// UT3D_vc_rotangr                 rotate a vector around Z-axis
/// 
/// vco and vci can be the same adress
/// \endcode

  Vector  vs;
  double  sin_a, cos_a;

  sin_a = sin(*ar);
  cos_a = cos(*ar);

  vs.dx = cos_a * vci->dx + sin_a * vci->dy;
  vs.dy = cos_a * vci->dy - sin_a * vci->dx;
  vs.dz = vci->dz;

  *vco = vs;

  return 0;

}


//====================================================================
  void UT3D_vc_rot3angr (Vector *vco,Vector *vci,
                       double alpha,double beta,double gamma) {
//====================================================================
/// \code
/// UT3D_vc_rot3angr                  rotate a vector
/// 
/// (vco and vci can have the same adress)
/// \endcode


  Vector vs, vt;


  /* First we rotate it around the Z axis (XY plane).. */
  /* ================================================= */

  vs.dx = cos(alpha) * vci->dx + sin(alpha) * vci->dy;
  vs.dy = cos(alpha) * vci->dy - sin(alpha) * vci->dx;

  /* ..then around the Y axis (XZ plane).. ?? about new or old axis ?? */
  /* ===================================== */

  vt = vs;

  vco->dx = cos(beta) * vt.dx+sin(beta)    * vci->dz;
  vs.dz    = cos(beta) * vci->dz-sin(beta) * vt.dx;

  /* ..and at last around the X axis (YZ plane) ?? about new or old axis ?? */
  /* ========================================== */

  vco->dy = cos(gamma) * vt.dy+sin(gamma) * vs.dz;
  vco->dz = cos(gamma) * vs.dz-sin(gamma) * vt.dy;

}


//===========================================================================
  int UT3D_vc_rotvcvcangr (Vector *vo, Vector *va, Vector *vi, double angr) {
//===========================================================================
/// \code
/// UT3D_vc_rotvcvcangr            rotate vec vi around vec va
/// VA MUSS NORMIERT SEIN !!!
/// see UT3D_pt_rotptptvcangr
/// \endcode

  double  dx, dy;
  Vector  vp, vx, vy, vz;


  // proj vi auf va
  UT3D_vc_projvcvc (&vp, vi, va);
    // GR_tDyn_vc__ (&vp, &UT3D_PT_NUL, 2, 0);

  // subract
  UT3D_vc_sub2vc (&vx, vi, &vp);
    // GR_tDyn_vc__ (&vx, &UT3D_PT_NUL, 2, 0);

  // vn = ein Normalvec auf vi mit Z-Achse va (90-Grad-Punkt)
  UT3D_vc_perp2vc (&vy, va, &vx);
    // GR_tDyn_vc__ (&vy, &UT3D_PT_NUL, 2, 0);


  dx = cos (angr);
  dy = sin (angr);
    // printf(" dx=%f dy=%f\n",dx,dy);


  // nun vcx und vcy entspr dem Winkel veraendern = neuer Punkt
  UT3D_vc_multvc (&vx, &vx, dx);
  UT3D_vc_multvc (&vy, &vy, dy);
    // DEB_dump_obj__ (Typ_VC, &vx, "vx=");
    // DEB_dump_obj__ (Typ_VC, &vy, "vy=");


  // add vx und vy und vp
  UT3D_vc_add3vc (vo, &vp, &vx, &vy);
    // GR_tDyn_vc__ (vo, &UT3D_PT_NUL, 2, 1);

  return 0;

}


//=========================================================================
  void UT3D_vc_rotvcangr (Vector *vco, double a1, Vector *Vi, Vector *Va) {
//=========================================================================
/// \code
/// rotate vector Vi around axis Va angr=a1
///  vco Vi may be ident
/// \endcode


  double  x, y, z, sinA, cosA, versA;
  Vector  vci;
  Mat_4x3 ma;



  // UT3D_m3_inirot_angr ohne Punkt
  UT3D_m3_inirot_angr (ma, NULL, Va, a1);

  // rot vec
  UT3D_vc_tra_vc_m3 (vco, ma, Vi);

  // printf ("ex UT3D_vc_rotvcangr  %f %f %f \n",vco->dx,vco->dy,vco->dz);

  return;

}


//================================================================
  int UT3D_vc_perpvcvc (Vector *v3, Vector *v1, Vector *v2) {
//================================================================
/// \code
/// UT3D_vc_perpvcvc          get normal-vector from vector onto other vector
/// output-vector v3 is normal onto v2
/// RetCodes:
///    0  OK
///   -1  v1-v2 aufeinander Normal; v3 == v1
/// 
///            /|
///        V1/  |
///         /   V3
///        /    |
///      /     \|/   V2
///    x---vcl-->------------>
/// \endcode

// 9 Mult + 2 Div

  int    irc;
  Vector vcl;


  irc = UT3D_vc_projvcvc (&vcl, v1, v2);
  if(irc) {*v3 = *v1; return irc;}       // v1 normal auf v2

  // v3 = v1 - vcl
  UT3D_vc_sub2vc (v3, v1, &vcl);

  return 0;

}


//================================================================
  int UT3D_vc_projvcvc (Vector *v3, Vector *v1, Vector *v2) {
//================================================================
/// \code
/// UT3D_vc_projvcvc             vector v3 = project vector v1 to vector v2
/// v3 will have same direction as v2 (parallel or antiparallel)
/// the length of v3 is the length of v1 projected onto v2.
/// Opening-angle v1-v2 > 180: v3 follows v1.
/// see UT3D_parvc_2vc
/// RetCodes:
///    0  OK
///    1  v1-v2 are perpendicular; v3 = 0,0,0
///   -1  v2=0,0,0;                v3 = 0,0,0
/// 
///            /|
///        V1/  |
///         /   |
///        /    |
///      /      |    V2
///    x----V3-->------------>
///                              
///                              /|
///                            /  |
///                         V1/   |
///                          /    |
///                        /      |
///    <-------V2--------x----V3-->
///
/// \endcode

// 9 Mult + 2 Div


  double  Det, dd, pl;


  // dot-prod v2
  Det = v2->dx * v2->dx + v2->dy * v2->dy + v2->dz * v2->dz;
  if(Det < UT_TOL_min2) {
    // v2=0,0,0; return 0,0,0
    *v3 = UT3D_VECTOR_NUL;
    return 1;
  }


  // Skalarprodukt
  dd  = v1->dx * v2->dx + v1->dy * v2->dy + v1->dz * v2->dz;
  if(fabs(dd) < UT_TOL_min2) {
    // v1-v2 perpendic.; return 0,0,0
    *v3 = UT3D_VECTOR_NUL;
    return 1;
  }


  pl = dd/Det;
  // pl = prozentuelle Laenge 0-1

  v3->dx = pl * v2->dx;
  v3->dy = pl * v2->dy;
  v3->dz = pl * v2->dz;

  // printf("ex UT3D_vc_projvcvc pl=%f Det=%f dd=%f\n",pl,Det,dd);

  return 0;

}


//=========================================================================
  int UT3D_vc_projvc2vc (Vector *vo, Vector *vi, Vector *v1, Vector *v2) {
//=========================================================================
/// UT3D_vc_projvc2vc         vector = project vector to plane from 2 vec's

  Vector vn, vp;


  // normalvektor vn aus v1 und v2 berechnen (Kreuzprod, UT3D_vc_perp2vc)
  vn.dx = v1->dy * v2->dz - v1->dz * v2->dy;
  vn.dy = v1->dz * v2->dx - v1->dx * v2->dz;
  vn.dz = v1->dx * v2->dy - v1->dy * v2->dx;
    // DEB_dump_obj__(Typ_VC, &vn, "vn");


  // vi auf vn projizieren
  UT3D_vc_projvcvc (&vp, vi, &vn);
    // DEB_dump_obj__(Typ_VC, &vp, "vp");


  // vp von vi abziehen
  UT3D_vc_sub2vc (vo, vi, &vp);
    // DEB_dump_obj__(Typ_VC, vo, "vo");


  return 0;

}


//================================================================
  int UT3D_vc_projvcnvc (Vector *vo, Vector *vi, Vector *vz) {
//================================================================
/// \code
/// UT3D_vc_projvcnvc              project vec onto plane from Z-vec
/// vo = vi projected onto a plane defined by its normal-vector vz.
/// vo is parallel to the plane from vz
/// the length of vo is the length of vi projected onto the plane from vz.
/// Opening-angle vi-vz > 180: vo follows vi.
/// RetCodes:
///    0  OK
///    1  vi-vz are perpendicular; vo = vi
///   -1  vz=0,0,0; vo = vi
/// 
///  pln                     vo = vi - vz1
///    ^-------/^
///    |      / |
///   vo    vi  vo
///    |   /    |
///    | /      |
///    x--------+------------> vz (normal-vector of plane)
///    |---vz1-->
/// 
/// \endcode


  double skp, lvz, l3;
  Vector vz1, vc3;


  // DEB_dump_obj__(Typ_VC, vi, "UT3D_vc_projvcnvc\n  vi:");
  // DEB_dump_obj__(Typ_VC, vz, "  vz:");


  lvz = UT3D_len_vc (vz);
  // wenn lenl==0 dann pp=pt
  if(lvz < UT_TOL_min1) {
    printf("***** UT3D_vc_projvcnvc E001\n");
    *vo = *vi;
    return -1;
  }


  skp = vz->dx * vi->dx + vz->dy * vi->dy + vz->dz * vi->dz;
    printf(" skp-1=%lf\n",skp);
  if(fabs(skp) < UT_TOL_min2) {
    // vi-vz perpendicular; copy vector
    *vo = *vi;
    return 1;
  }


  skp /= lvz;
  skp /= lvz;
    // printf(" skp=%f %f\n",skp,lv1);

  vz1.dx = vz->dx * skp;
  vz1.dy = vz->dy * skp;
  vz1.dz = vz->dz * skp;
    // DEB_dump_obj__(Typ_VC, &vz1, "  vz1:");

  vo->dx = vi->dx - vz1.dx;
  vo->dy = vi->dy - vz1.dy;
  vo->dz = vi->dz - vz1.dz;

    // DEB_dump_obj__(Typ_VC, vo, "ex UT3D_nlen_projvcvc");

  return 0;

}


//======================================================================
  void UT3D_vc_projvcpl (Vector *vco, Plane *pl, Vector *vci) {
//======================================================================
/// \code
/// UT3D_vc_projvcpl          vector = project vector onto plane
/// SEE UT3D_vc_projvcnvc
/// test for vci=normal to pl with:
/// if(UT3D_compvc0 (vco, UT_TOL_min1)) - IS NORMAL !
/// \endcode


  Point p1, p2;

  /* create a point; distance is vci, starting at the plane-origin */
  UT3D_pt_traptvc (&p1, &pl->po, vci);

  /* project this point onto pl */
  UT3D_pt_projptpl (&p2, pl, &p1);

  /* now the vector is plane-origin > p2 */
  UT3D_vc_2pt (vco, &pl->po, &p2);

  /* printf("UT3D_vc_projvcpl %f %f %f\n",vco->dx,vco->dy,vco->dz); */

  return;

}


//================================================================
  int UT3D_vc_mirvcpl (Vector *vco, Vector *vci, Plane *pln) {
//================================================================
/// \code
/// mirror vector about plane
/// \endcode

// dem Planeorigin den vektor aufaddieren;
// diesen punkt dann auf die Pln projiz; opposite Punkt = gespiegelter Vektor.


  Point pte, ptp, ptm;

  // dem Planeorigin den vektor aufaddieren;
  // UT3D_PT_TRAPTVC (&pte, &pln->po, vci);
  UT3D_pt_traptvc (&pte, &pln->po, vci);
    // GR_tDyn_symB__ (&pte, SYM_STAR_S, 2);

  // Kopie von UT3D_pt_mirptpl
  // pte auf die plane projiz. -> ptp
  UT3D_pt_projptpl (&ptp, pln, &pte);
  UT3D_pt_opp2pt (&ptm, &ptp, &pte);

  UT3D_vc_2pt (vco, &pln->po, &ptm);

  return 0;

}


//================================================================
  int UT3D_vc_mirvcln (Vector *vco, Vector *vci, Line *ln1) {
//================================================================
/// \code
/// mirror vector about line
/// \endcode


  Vector  vcl;

  // DEB_dump_obj__ (Typ_LN, ln1, "UT3D_vc_mirvcln - ln1:");
  // DEB_dump_obj__ (Typ_VC, vci, "  vci:");

  // ln -> vector vcl
  UT3D_vc_ln (&vcl, ln1);
  UT3D_vc_perpvcvc (vco, vci, &vcl);    // prj vc on ln; get diff-vc
  UT3D_vc_multvc (vco, vco, -2.);
  UT3D_vc_add2vc (vco, vci, vco);

    // DEB_dump_obj__ (Typ_VC, vco, "  vco:");

  return 0;

}


//====================================================================
  void UT3D_vc_tra_vc_m3 (Vector *b, Mat_4x3 mata, Vector *a) {
//====================================================================
/// \code
/// UT3D_vc_tra_vc_m3           apply transformation to vector (from 4x3-matrix)
/// rotate Vector: prepare with UT3D_m3_inirot_angr
/// b a may be ident
/// \endcode

//  wird das ein normalisierter vektor ?
//  eigentlich eine vectorTransform ?

  double x, y, z;


  /* printf ("UT3D_vc_tra_vc_m3 in %f %f %f \n",a->dx,a->dy,a->dz); */

  x = a->dx;
  y = a->dy;
  z = a->dz;


  b->dx = mata[0][0]*x + mata[0][1]*y + mata[0][2]*z;
  b->dy = mata[1][0]*x + mata[1][1]*y + mata[1][2]*z;
  b->dz = mata[2][0]*x + mata[2][1]*y + mata[2][2]*z;


  /* printf ("UT3D_vc_tra_vc_m3 ou %f %f %f \n",b->dx,b->dy,b->dz); */


  // UT3D_vc_normalize (b, b);   // raus 2003-04-12


}






//====================================================================
  void UT3D_vc_travcm4 (Vector *b, Mat_4x4 ma, Vector *a) {
//====================================================================
/// \code
/// UT3D_vc_travcm4           apply transformation to vector (from 4x4-matrix)
///  wird das ein normalisierter vektor ?
///  eigentlich eine vectorTransform ?
/// dann sollte a ein vc sein !
/// \endcode


  /* UT3D_pt_tra_pt_m4 ((void*)b, ma, (void*)a); */
  UT3D_pt_tra_pt_m4 ((Point*)b, ma, (Point*)a);

  b->dx -= ma[3][0];
  b->dy -= ma[3][1];
  b->dz -= ma[3][2];

  UT3D_vc_normalize (b, b);

  /* printf ("UT3D_vc_travcm4 %f %f %f \n",b->dx,b->dy,b->dz); */

}








/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/



//==============================================================================
  int UT3D_comp2ln (Line *pa1, Line *pa2, double tol) {
//==============================================================================
/// \code
/// UT3D_comp2ln              compare 2 lines if identical
/// 
/// RC = 1 = Lines are identical
/// RC = 0 = one of the points has a distance > tol
/// \endcode


  // printf("UT3D_comp4pt: tol=%f\n",tol);


  if(UT3D_comp2pt(&pa1->p1, &pa2->p1, tol) == 1) {     // Start/Start sind gleich
    if(UT3D_comp2pt(&pa1->p2, &pa2->p2, tol) == 1) return 1;


  } else {
    if(UT3D_comp2pt(&pa1->p1, &pa2->p2, tol) == 1) {   // Start/End gleich
      if(UT3D_comp2pt(&pa1->p2, &pa2->p1, tol) == 1) return 1;
    }
  }

  return 0;

}


//================================================================
  int UT3D_ln_ck_on_ln (Point *lo1,Point *lo2,
                        Point *l11,Point *l12,
                        Point *l21,Point *l22, double tol) {
//================================================================
/// \code
/// check if 2 lines collinear
/// Input:
///   l11, l12     Line 1
///   l21, l22     Line 2
/// Output:
///   lo1, lo2     the collinear segment if RC==1
/// RetCod:
///  -1 no
///   0 yes, no coverage
///   1 coverage: coverline out
/// \endcode

  int      irc;
  double   d1, p1, p2, p3;
  Vector   vl1, vl2, vx;
  Point    *pp2, *pp3;


  // printf("UT3D_ln_ck_on_ln %f\n",tol);


  // check if 2 lines are parallel / antiparallel
  //     Vektorvergleich   see UT3D_comp2vc UT3D_comp2vc_p UT2D_pt_int4pt
  UT3D_vc_2pt (&vl1, l11,l12);
  UT3D_vc_2pt (&vl2, l21,l22);

  irc = UT3D_comp2vc_d (&vl1, &vl2, 0.001);  // 1=parl, 0=not
  if(irc != 1) {
    // printf(" ck_on_ln not parl\n");
    return -1;
  }


  // check if 2 lines are collinear
  // Normalabstand zueinander
  d1 = UT3D_nlen_2ptvc (l11, l21, &vl2);
  if(d1 > tol) {
    // printf(" ck_on_ln not colli\n");
    return -1;
  }


  // alle 4 Punkte sind collinear.
  // parameter aller 4 Punkte generieren.
  // p0 ist l11, p1 ist l12
  // wenn ueberschneidung: die beiden inneren Punkte sind die common line.

  p1 = UT3D_len_vc (&vl1);  // Laenge l11-l12

  // Laenge l11-l21
  vx.dx = l21->x - l11->x;
  vx.dy = l21->y - l11->y;
  vx.dz = l21->z - l11->z;
  p2 = (vl1.dx*vx.dx + vl1.dy*vx.dy + vl1.dz*vx.dz) / p1;


  // Laenge l11-l22
  vx.dx = l22->x - l11->x;
  vx.dy = l22->y - l11->y;
  vx.dz = l22->z - l11->z;
  p3 = (vl1.dx*vx.dx + vl1.dy*vx.dy + vl1.dz*vx.dz) / p1;
    // printf(" p1=%f p2=%f p3=%f\n",p1,p2,p3);

  // L1 geht nun von 0 bis p1,  L2 von p2 nach p3.
  // p1 ist immer positiv.

  // p2-p3 aufsteigend (wie 0-p1) orientieren.
  if(p3 < p2) {
    MEM_swap_2db (&p2, &p3);
    pp2 = l22;
    pp3 = l21;
  } else {
    pp2 = l21;
    pp3 = l22;
  }
    // printf(" p1=%f p2=%f p3=%f\n",p1,p2,p3);

  if(p2 > p1) goto L_out;
  if(p3 < 0.) goto L_out;


  if(p2 < 0.) *lo1 = *l11;
  else        *lo1 = *pp2;

  if(p3 < p1) *lo2 = *pp3;
  else        *lo2 = *l12;



    // printf("ex ck_on_ln 1\n");
    // DEB_dump_obj__(Typ_PT, lo1, "  lo1:");
    // DEB_dump_obj__(Typ_PT, lo2, "  lo2:");
  return 1;


  L_out:
    // printf("ex ck_on_ln 0\n");
    return 0;

}


//================================================================
  int UT3D_ln_ck_degen (Line *ln) {
//================================================================
/// \code
/// UT3D_ln_ck_degen          check if line is degenerated (length < UT_TOL_cv)
/// retCode:  1     no, not degenerated
///          -2     yes, degenerated; length is .lt. UT_TOL_cv but .gt. UT_TOL_pt
///          -3     yes, degenerated; length is .lt. UT_TOL_pt
/// \endcode

  double   d1;

  d1 = UT3D_lenB_2pt (&ln->p1, &ln->p2);

  if(d1 < UT_TOL_cv) {
    if(d1 < UT_TOL_pt) return -3;
    return -2;
  }

  return 1;

}


//======================================================================
  int UT3D_ln_ck_parpl (double *dist, Line *ln, Plane *pl, double tol) {
//======================================================================
/// \code
/// UT3D_ln_ck_parpl          check if line is parallel to plane
/// 
/// UT3D_ln_ck_parpl     Author: Thomas Backmeister       4.7.2003
/// 
/// Check, if a line is parallel to a plane.
/// 
/// IN:
///   Line *ln    ... 3D-line
///   Plane *pl   ... plane
///   double tol  ... tolerance for parallelism
/// OUT:
///   double *dist ... signed distance line - plane if line || plane
///                    >0.0 <=> line ON side of plane normal vector
///                    <0.0 <=> line on OPPOSITE side of plane normal vector
/// Returncodes:
///   0 = line is NOT parallel to plane
///   1 = line is parallel to plane
/// \endcode


  double scp;
  Vector vl;

  // printf("UT3D_ln_ck_parpl tol=%f\n",tol);
  // DEB_dump_obj__ (Typ_LN,  ln, "");
  // DEB_dump_obj__ (Typ_PLN, pl, "");


  // init
  *dist = 0.0;

  // vector of line
  UT3D_vc_ln (&vl, ln);

  // scalar product <line vector, plane normal vector>
  scp = UT3D_acos_2vc (&vl, &(pl->vz));

  // --- line || plane ---
  if (fabs(scp) < tol) {

    // distance line - plane
    *dist = UT3D_slen_ptpl (&(ln->p1), pl);
    // printf("ex UT3D_ln_ck_parpl 1 %f\n",*dist);
    return 1;

  } else {
    // printf("ex UT3D_ln_ck_parpl 0\n");
    return 0;
  }
}


/*
//=======================================================================
  int UT3D_ln_inv (Line *ln1) {
//=======================================================================
// UT3D_ln_inv               invert (change p1, p2)

  MEM_swap__(&ln1->p1,&ln1->p2,sizeof(Point));

  return 0;

}
*/


//================================================================
  int UT3D_ln_unlim (Line *lno, int lTyp) {
//================================================================
// UT3D_ln_unlim           set  length for construction-line

  double  d1, d2;
  Point   *pta, *pte;
  Vector  vc1;


  // printf("UT3D_ln_unlim %d\n",lTyp);
  // DEB_dump_obj__ (Typ_LN,  lno, "UT3D_ln_unlim ");


  lno->typ = lTyp;

  if(!lTyp) return 0;

  pta = &lno->p1;
  pte = &lno->p2;

  UT3D_vc_2pt (&vc1, pta, pte); // ln -> vc

  d1 = UT3D_len_vc (&vc1);
  d2 = UT_DISP_ln / d1;   // UT_DISP_ln=length of construction-line
  UT3D_vc_multvc (&vc1, &vc1, d2);



  if(lTyp == 1) {  // p1 is startpoint, p2 unlimited) UNL1
    UT3D_pt_traptvc (pte, pta, &vc1);

  } else if(lTyp == 2) {  // p1 is startpoint, p1 unlimited) UNL2
    UT3D_pt_traptivc (pte, pta, &vc1);

  } else if(lTyp == 3) {  // both sides unlimited   UNL
    UT3D_pt_traptivc (pta, pta, &vc1);
    UT3D_pt_traptvc (pte, pte, &vc1);
  }

    // DEB_dump_obj__ (Typ_LN,  lno, "ex-UT3D_ln_unlim ");

  return 0;

}


//================================================================
  void UT3D_ln_6db (Line *ln, double x1, double y1, double z1,
                              double x2, double y2, double z2) {
//================================================================
/// line from 2 * 3 doubles

  ln->p1.x = x1;
  ln->p1.y = y1;
  ln->p1.z = z1;

  ln->p2.x = x2;
  ln->p2.y = y2;
  ln->p2.z = z2;

  return;

}


/* replaced by UT3D_ln_ptpt
//====================================================================
  void  UT3D_ln_2pt (Line *ln1, Point *pta, Point *pte) {
//====================================================================
/// UT3D_ln_2pt2              3D-Line aus 2 2D-Punkten


  ln1->p1 = *pta;
  ln1->p2 = *pte;

  return;

}
*/


//====================================================================
  void  UT3D_ln_2pt2 (Line *ln1, Point2 *pta, Point2 *pte) {
//====================================================================
/// UT3D_ln_2pt2              3D-Line aus 2 2D-Punkten


  ln1->p1.x = pta->x;
  ln1->p1.y = pta->y;
  ln1->p1.z = 0.0;

  ln1->p2.x = pte->x;
  ln1->p2.y = pte->y;
  ln1->p2.z = 0.0;

  return;

}




/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/




//========================================================================
  int UT3D_ptNr_ci (Circ *ci1, double tol) {
//========================================================================
/// UT3D_ptNr_ci              nr of points for polygon from circle

// Sehnentoleranz: siehe UT2D_angr_ciSec
// see also UT2D_ptNr_ci


  // int    ianz;
  // double aCir, aTol, rd1, a1, dCos;


  // DEB_dump_obj__ (Typ_CI, ci1, "UT3D_ptNr_ci tol=%f:\n",tol);


  //  die Anzahl Punkte ianz feststellen
  return UT2D_ptNr_ci (fabs(ci1->rad), fabs(ci1->ango), tol);

/*
  //  den Umfangswinkel aCir errechnen
  // aCir = UT3D_angr_ci__ (ci1);
  aCir = fabs(ci1->ango);
  rd1  = fabs(ci1->rad);
  // printf(" AC-Oeffnungswinkel=%f\n",aCir);

  a1 = rd1 - tol;
  // printf("   tol=%f rd1=%f a1=%f\n",tol,rd1,a1);


  if(a1 < 0.) {   //  if Tol. > Radius
    aTol = RAD_45;
  } else {
    dCos = (a1/rd1);  //  der Oeffnungswinkel
    aTol = ACOS(dCos) * 2.;
    if(aTol > RAD_45)  aTol = RAD_45;
  }

  ianz = (aCir / aTol) + 1;
  if(ianz < 2) ianz = 2;

  // printf("ex UT3D_ptNr_ci ianz=%d  (tol=%lf ao=%lf)\n",ianz,tol,aCir);
  return ianz;
*/

}


//======================================================================
  int UT3D_ck_ci180 (Circ *ci1) {
//======================================================================
/// \code
/// UT3D_ck_ci180             check if circ is 180-degree-circ
/// RC = 0:   YES, is 180-deg-circ
/// RC = 1:   NO
/// if(UT3D_ck_ci180 == YES) ...
/// \endcode

  int    irc;
  double tol;

  // tol = UT_TOL_pt * fabs(ci1->rad);
  tol = UT_TOL_pt;

  if(UTP_comp2db (fabs(ci1->ango), RAD_180, tol) == 1) irc = YES; // 1=diff < tol
  else irc = NO;

  // DEB_dump_obj__ (Typ_CI, ci1, "ex UT3D_ck_ci180 %d\n",irc);

  return irc;

}


//======================================================================
  int UT3D_ck_ci360 (Circ *ci1) {
//======================================================================
/// \code
/// UT3D_ck_ci360             check if circ is 360-degree-circ
/// RC =  1:   NO
/// RC =  0:   YES, is 360DegCirc
/// RC = -2:   degenerated (rad < UT_TOL_cv)
/// if(UT3D_ck_ci360 == YES) ...
///
/// see UT2D_2angr_ck_360
/// \endcode

  double d1;


  if(fabs(ci1->rad) < UT_TOL_cv) return -2;

  d1 = UT3D_lenB_2pt (&ci1->p1, &ci1->p2);
  if(d1 < UT_TOL_cv) {
    if(UTP_comp2db (fabs(ci1->ango), RAD_360, UT_TOL_Ang1)) {
      // 360-deg-circ
      return 0; // 1=diff < tol
    }
    if(fabs(ci1->ango) < RAD_180) return -2;  // degenerated
  }

  // DEB_dump_obj__ (Typ_CI, ci1, "ex UT3D_ck_ci360 %d\n",irc);

  return 1;

}


//================================================================
  int UT3D_cksid_civc (Circ *ci1, Vector *vc1) {
//================================================================
/// \code
/// UT3D_cksid_civc           ck if KreisStart/Endpunkt ist in Richtg VC od verk.
/// Ein Vektor und ein Kreisbogen sind in der gleichen Ebene;
/// Test, ob KreisStart/Endpunkt eher in Richtung des Vektors liegen
/// oder eher verkehrt liegen.
/// RetCod=0:  Start/Endpunkt liegen in Richtung vc1
/// RetCod=1:  Start/Endpunkt liegen verkehrt zu vc1
/// \endcode

  double d1, d2;


  d1 = UT3D_slen_2ptvc (&ci1->pc, &ci1->p1, vc1);
  d2 = UT3D_slen_2ptvc (&ci1->pc, &ci1->p2, vc1);

  if(d1 > d2) {
    // printf(" _cksid_civc inv!!!!!!!!!!\n");
    return 1;
  } else {
    return 0;
  }
}


//================================================================
  int UT3D_ci_cirad (Circ *cio, Circ *cii, double rad) {
//================================================================
// UT3D_ci_cirad             change radius of circ; keep sense of rotation
// 
// Input:
//   rad        new radius
//



  double d1;

    // DEB_dump_obj__ (Typ_CI, cii, "UT3D_ci_cirad");

  *cio = *cii;
  cio->rad = rad * DSIGN(cii->ango);
  d1 = fabs(rad / cii->rad);

  // set p1,p2
  // transl point from p2 along p1-p2 * fakt
  UT3D_pt_tra_pt_pt_par (&cio->p1, &cii->pc, &cii->p1, d1);
  UT3D_pt_tra_pt_pt_par (&cio->p2, &cii->pc, &cii->p2, d1);

    // DEB_dump_obj__ (Typ_CI, cio, " ex-UT3D_ci_cirad");

  return 0;

}


//======================================================================
  int UT3D_ci_cipt180 (Circ *ci1, Point *p1) {
//======================================================================
/// \code
/// UT3D_ci_cipt180           change circ -> 180 deg-circ; keep dir
/// make p2 opposit to p1
/// \endcode

  int   idir;

  // DEB_dump_obj__ (Typ_PT, p1, "UT3D_ci_cipt180 ");

  idir = DSIGN (ci1->rad);

  ci1->p1 = *p1;
  UT3D_pt_opp2pt (&ci1->p2, &ci1->pc, &ci1->p1);

  ci1->rad  = UT3D_len_2pt(&ci1->pc, p1);
  ci1->ango = RAD_180;

  if(idir < 0) {
    ci1->rad  = -ci1->rad;
    ci1->ango = -ci1->ango;
  }

  return 0;

}


//======================================================================
  int UT3D_ci_cipt360 (Circ *ci1, Point *p1) {
//======================================================================
/// UT3D_ci_cipt360           change circ -> 360 deg-circ; keep dir

  int   idir;

  // DEB_dump_obj__ (Typ_PT, p1, "UT3D_ci_cipt360 ");

  idir = DSIGN (ci1->rad);

  ci1->p1 = *p1;
  ci1->p2 = *p1;

  ci1->rad  = UT3D_len_2pt(&ci1->pc, p1);
  ci1->ango = RAD_360;

  if(idir < 0) {
    ci1->rad  = -ci1->rad;
    ci1->ango = -ci1->ango;
  }

  return 0;

}


//======================================================================
  int UT3D_ci_cip1 (Circ *ci1, Point *p1) {
//======================================================================
/// UT3D_ci_cip1              change startpoint of circ


  // DEB_dump_obj__ (Typ_PT, p1, "UT3D_ci_cip1 ");
  // DEB_dump_obj__ (Typ_CI, ci1, "");

  ci1->p1 = *p1;

  // openingAngle errechen
  ci1->ango = UT3D_angr_ci_p1_pt (ci1, &ci1->p2);

  // DEB_dump_obj__ (Typ_CI, ci1, "ex UT3D_ci_cip1\n");

  return 0;

}


//======================================================================
  int UT3D_ci_cip2 (Circ *ci1, Point *p2) {
//======================================================================
/// UT3D_ci_cip2              change endpoint of circ

  // DEB_dump_obj__ (Typ_PT, p2, "UT3D_ci_cip2 ");

  ci1->p2 = *p2;

  ci1->ango = UT3D_angr_ci_p1_pt (ci1, &ci1->p2);

  // DEB_dump_obj__ (Typ_CI, ci1, "ex UT3D_ci_cip2\n");

  return 0;

}


//======================================================================
  int UT3D_ci_cip3 (Circ *ci1, Point *p1, Point *p2) {
//======================================================================
/// \code
/// UT3D_ci_cip3              change start- and endpoint of circ
/// ATTENTION: DO NOT USE FOR 360-DEG-CIRCs: returns angle = 0.;
///   if (UT3D_comp2pt (&p1, &p2, UT_TOL_min0)) {
///     UT3D_ci_cipt360 (cvo, &p1);    // set 360-deg-circ
///   } else {
///     UT3D_ci_cip3 (cvo, &p1, &p2);
///   }
/// \endcode


  // printf("UT3D_ci_cip3 \n");

  ci1->p1 = *p1;
  ci1->p2 = *p2;

  // get opening angle
  ci1->ango = UT3D_angr_ci_p1_pt (ci1, &ci1->p2);
    // DEB_dump_obj__ (Typ_CI, ci1, "ex UT3D_ci_cip2\n");

  return 0;

}


//======================================================================
  int UT3D_ci_ciangr (Circ *ci, double ao2) {
//======================================================================
/// UT3D_ci_ciangr              change endpoint of circ from angle
/// Input:
///   ao2   new angle in rad; >0. = CCW; <0 = CW; 
///         sign does also change change the sense-of-rotation;


  ci->ango = ao2;
  ci->rad  = DSIGN(ao2) * fabs(ci->rad);

  UT3D_pt_rotptptvcangr (&ci->p2,&ci->p1,&ci->pc,&ci->vz,ao2);
  // DEB_dump_obj__ (Typ_CI, ci1, "ex UT3D_ci_cip2\n");

  return 0;

}


//================================================================
  void UT3D_ci_setangr (Circ *ci1) {
//================================================================
/// UT3D_ci_setangr           fix angle from p1-p2

// see also UT3D_angr_ci__


  // DEB_dump_obj__ (Typ_CI, ci1, "UT3D_ci_setangr\n");

  ci1->ango = UT3D_angr_ci_p1_pt (ci1, &ci1->p2);

  // DEB_dump_obj__ (Typ_CI, ci1, "ex UT3D_ci_setangr\n");

}


//============================================================================
  int UT3D_ci_ptptvcangr (Circ *ci,Point *pc,Point *p1,Vector *vz,double ao) {
//============================================================================
/// \code
/// UT3D_ci_ptptvcangr        Circ from center, startpoint, axis, opening angle
/// neg. Winkel: CCW !
/// \endcode

  printf("UT3D_ci_ptptvcangr ao=%f\n",ao);


  ci->pc = *pc;
  ci->p1 = *p1;
  ci->vz = *vz;

  ci->rad  = UT3D_len_2pt (pc, p1);
  if(ao < 0.) ci->rad = -ci->rad;

  UT3D_ci_ciangr (ci, ao);

  return 0;

}


//================================================================
  int UT3D_rdc_3pt (double *rdc, Point *pp1, Point *pp2, Point *pp3) {
//================================================================
/// \code
/// Den Radius eines Kreises errechnen, von dem 3 Punkte bekannt sind.
/// p1-p3 sind 3 punkte am Kreis.
/// rc = a * b / 2 * hc             - is halt so ..
/// 
///               pp2
///               / | \
///             /   |   \
///           b    hc     a
///         /       |       \
///     pp3 -------c--------pp1
///             q      p
/// \endcode


  int        irc;
  double     a, b, c, q, hc;



  a = UT3D_len_2pt (pp1, pp2);
  b = UT3D_len_2pt (pp2, pp3);
  c = UT3D_len_2pt (pp3, pp1);

  irc = UT2D_solvtri_abc (&q, &hc, a, b, c);
  if(irc < 0) return -1;
  if(hc < UT_TOL_min1) return -2;

  *rdc = (a * b) / (hc * 2.);


  L_exit:
  // printf("ex UT3D_rdc_3pt %f\n",*rdc);

  return 0;

}


//================================================================
  int UT3D_ci_ptrd2vc2angr (Circ *ci, Point *pc, double rd,
                            Vector *vz, Vector *vx,
                            double a1, double a2) {
//================================================================
/// \code
/// circ from center, Radius, Z-Axis, X-Axis, startAngle, endAngle
/// ACHTUNG: Z-Axis u X-Axis muessen normiert sein !!!!
/// rd hat DrehRichtg.
/// ACHTUNG: die Winkel werden hier umgedreht, die Kreisdrehachse
/// ist genau verkehrt zum Plane-vz !
/// \endcode


  int     dreh;
  Vector  vc1, vc2;
  Mat_4x3 ma;


  // printf("UT3D_ci_ptrd2vc2angr rd=%f a1=%f a2=%f\n",rd,a1,a2);

  // dreh = DSIGN(rd);
  dreh = DLIM01(rd);                            // 2017-02-02
    // printf(" dreh=%d\n",dreh);

  ci->pc   = *pc;
  ci->vz   = *vz;
  ci->rad  = rd;
  ci->ango = UT2D_angr_2angr (a1, a2, dreh);

  UT3D_vc_multvc (&vc1, vx, fabs(rd));

  // UT3D_m3_inirot_angr ohne Punkt
  UT3D_m3_inirot_angr (ma, NULL, vz, -a1);

  // rot vec
  UT3D_vc_tra_vc_m3 (&vc2, ma, &vc1);
  UT3D_pt_traptvc (&ci->p1, pc, &vc2);

  // UT3D_m3_inirot_angr ohne Punkt
  UT3D_m3_inirot_angr (ma, NULL, vz, -a2);

  // rot vec
  UT3D_vc_tra_vc_m3 (&vc2, ma, &vc1);
  UT3D_pt_traptvc (&ci->p2, pc, &vc2);

  // DEB_dump_obj__ (Typ_CI, ci, "ex UT3D_ci_ptrd2vc2angr:\n");
  // GR_Disp_ac (ci, 9);

  return 0;

}


//================================================================
  int UT3D_ci_ptvcpt2angr (Circ *ci,
                           Point *pc, Point *p1, Vector *vz,
                           double a1, double a2) {
//================================================================
/// circ from center, point, axis, startAngle, endAngle

  ci->pc   = *pc;
  ci->vz   = *vz;
  ci->rad  = UT3D_len_2pt (pc, p1);
  ci->ango = UT2D_angr_2angr (a1, a2, 0);

  UT3D_pt_rotptptvcangr (&ci->p1,p1,pc,vz,a1);
  UT3D_pt_rotptptvcangr (&ci->p2,p1,pc,vz,a2);


  // DEB_dump_obj__ (Typ_CI, ci, "ex UT3D_ci_ptvcpt2angr:\n");
  // GR_Disp_ac (ci, 9);

  return 0;

}


//================================================================
  int UT3D_ci_ptvcrd (Circ *ci,    Point *ps, Vector *vs, double rd,
                      Vector *vz, double a1) {
//================================================================
/// \code
/// UT3D_ci_ptvcrd  Circ from startpoint, startvector, radius ..
/// ACHTUNG: vs und vz muessen normiert sein !
/// Input:
///   ps    Startpoint of circ
///   vs    tangent in ps
///   rd    Radius; positiv:circ=ccw; negativ: circ=cw.
///   vz    Axis
///   a1    Angle in rad; 0=full circle.
/// \endcode


  Vector      vcc;


  // printf("UT3D_ci_ptvcrd rd=%f a1=%f\n",rd,a1);
    // GR_tDyn_symB__ (ps, SYM_TRI_S, 3);
    // GR_tDyn_vc__ (vs, ps, 9, 0);
    // GR_tDyn_vc__ (vz, ps, 2, 0);


  ci->p1  = *ps;
  ci->vz  = *vz;
  ci->rad = rd;

  // // wenn rd=negativ muss auch angle=negativ sein !
  // if(rd < 0.) if(a1 > 0.) a1 = -a1;

  // get center: vec vcc
  // vc ps -> cen = Kreuzprodukt
  UT3D_vc_perp2vc (&vcc, vz, vs);
    // GR_tDyn_vc__ (&vcc, ps, 3, 0);

  // Abstand = fabs(rd)
  UT3D_vc_setLength (&vcc, &vcc, 1.);    // 2011-08-09
  UT3D_vc_multvc (&vcc, &vcc, rd);
  UT3D_pt_traptvc (&ci->pc, ps, &vcc);
    // GR_tDyn_symB__ (&ci->pc, SYM_TRI_S, 4);


  // openingAngle: 0=Vollkreis; else Arc.
  if(a1 == 0.) {
    ci->ango = RAD_360;
    ci->p2 = ci->p1;

  } else {
    UT3D_ci_ciangr (ci, a1);
  }


    // DEB_dump_obj__ (Typ_CI, ci, "ex UT3D_ci_ptvcrd:\n");
    // GR_Disp_ac (ci, 9);

  return 0;

}


//================================================================
  int UT3D_ci_3pt (Circ *cpo, Point *pp1, Point *pp2, Point *pp3) {
//================================================================
/// \code
/// UT3D_ci_3pt          Circ from 3 points
/// p1-p3 sind 3 punkte am Kreis.
/// rc = a * b / 2 * hc             - is halt so ..
/// hx wird genau in Mitte von c angesetzt (Streckensymmetrale, Pt. pMc)
/// hx = sqrt (r * r - (c * c / 4))   // gleichseitiges Dreieck p1,p2,Cen.
/// 
///               pp2
///               / | \
///             /   |   \
///           b    hc     a
///         /       |       \
///       pp3-------c--------pp1
///             q    | 
///                  hx,vx
///                  |
///                  Cen
/// \endcode

// Wenn der Kreismittelpunkt im Dreieck p1-p2-p3 liegt, geht immer. Sonst:
// Abstand pMc - pp2 messen; wenn < rad muss man Richtung vx umdrehen.



  int        irc;
  double     a, b, c, q, hc, hx, dMc2;
  Vector     vc, va, vx;
  Point      pMc;   // Mittelpunkt von a (p1-p3)


  // printf("UT3D_ci_3pt \n");
  // GR_tDyn_symB__ (pp1, SYM_TRI_S, 3);
  // GR_tDyn_symB__ (pp2, SYM_TRI_S, 3);
  // GR_tDyn_symB__ (pp3, SYM_TRI_S, 3);
  // GR_tDyn_txiA (pp1, 1, 0);
  // GR_tDyn_txiA (pp2, 2, 0);
  // GR_tDyn_txiA (pp3, 3, 0);


  // den Radius errechnen  // ident UT3D_rdc_3pt
  a = UT3D_len_2pt (pp1, pp2);
  b = UT3D_len_2pt (pp2, pp3);
  c = UT3D_len_2pt (pp3, pp1);
    // printf(" a=%f b=%f c=%f\n",a,b,c);


  irc = UT2D_solvtri_abc (&q, &hc, a, b, c);
  if(irc < 0) return -1;
  if(hc < UT_TOL_min1) return -2;

  cpo->rad = (a * b) / (hc * 2.);
    // printf(" rad=%f q=%f hc=%f\n",cpo->rad,q,hc);


  // normalvec to 3 pt's
  UT3D_vc_2pt (&va, pp1, pp2);
  UT3D_vc_2pt (&vc, pp1, pp3);
  // UT3D_vc_perp2vc (&cpo->vz, &vc, &va);
  UT3D_vc_perp2vc (&cpo->vz, &va, &vc);
  UT3D_vc_setLength (&cpo->vz, &cpo->vz, 1.);


  //  Punkte in 0, 90, 180 Grad ?  liefert sonst r=nan !!!
  if(UTP_comp2db(q,hc,UT_TOL_min1) == 1) {
    // Mittelpunkt c
    UT3D_pt_mid2pt (&cpo->pc, pp1, pp3);
    // UT3D_ci_cipt180
    cpo->rad = UT3D_len_2pt (pp1, pp3) / 2.;
    goto L_done;
  }


  // Mittelpunkt c
  UT3D_pt_mid2pt (&pMc, pp1, pp3);
    // GR_tDyn_vc__ (&cpo->vz, &pMc, 1, 0);

  // gleichseitiges Dreieck p1,p2,Cen.
  hx = sqrt (cpo->rad * cpo->rad - (c * c / 4));
   // printf(" q=%f hc=%f hx=%f\n",q,hc,hx);


  // vx = Normalvec auf vz, vc soll in Richtg CenCir zeigen.

  // wenn p2 ausserhalb p1-p3 liegt:
  if((q < 0.)||(q > c)) {
    // UT3D_vc_perp2vc (&vx, &cpo->vz, &vc);
    UT3D_vc_perp2vc (&vx, &vc, &cpo->vz);

  } else {

    // Abstand pMc - pp2 messen; wenn < rad muss man Richtung vx umdrehen.
    dMc2 = UT3D_len_2pt (&pMc, pp2);
      // printf(" dMc2=%f\n",dMc2);
    // if(dMc2 > cpo->rad) UT3D_vc_perp2vc (&vx, &cpo->vz, &vc);
    // else                UT3D_vc_perp2vc (&vx, &vc, &cpo->vz);
    if(dMc2 > cpo->rad) UT3D_vc_perp2vc (&vx, &vc, &cpo->vz);
    else                UT3D_vc_perp2vc (&vx, &cpo->vz, &vc);
  }

  // UT3D_vc_invert (&vx, &vx);
    // GR_tDyn_vc__ (&vc, &pMc, 9, 0);
    // GR_tDyn_vc__ (&vx, &pMc, 9, 0);


  // cenCirc = den Radius von pp3 weg in Richtung pM (vx) abschlagen
  UT3D_pt_traptvclen (&cpo->pc, &pMc, &vx, hx);


  L_done:
  // make 360-deg-circ from p1
  UT3D_ci_cipt360 (cpo, pp1);

  // GR_Disp_ac (cpo, 2);
  // if(isnan(cpo->rad)) exit(0);

  return 0;

}


//======================================================================
  int UT3D_ci_2ptvcrd (Circ *cia,
                       Point *pp1, Point *pp2, Vector *vz, double rdc) {
//======================================================================
/// \code
/// UT3D_ci_2ptvcrd                  Circ from 2 Points-on-Circ & Radius
///   returns unlimited circle, starting at pp1.
///
/// RetCod: nr of circles in cia
///
/// see UT3D_ci_ptptvcrd UT3D_ci_ptvcrd
///
///             d1     |    vc1
///        x-----------x-----------x
///      pp1         pt1          pp2
///                    |
///                    |
///                    |vc2
///                    |
///                    x
///                  circ.pc
/// \endcode


  int     i1;
  double  d1, d3;
  Point   pt1;
  Vector  vc1, vc2;
  Circ    *c1;


  c1 = &cia[0];

  c1->p1   = *pp1;
  c1->p2   = *pp1;
  c1->vz   = *vz;
  c1->rad  = rdc;
  c1->ango = RAD_360;

  cia[1] = *c1;   // copy Circ


  // pt1 = mittelpunkt pp1 - pt1
  UT3D_pt_mid2pt (&pt1, pp1, pp2);

  // d1 = strecke pp1 - pt1
  d1 =  UT3D_len_2pt (pp1, &pt1);


  // d1 < UT_TOL_min0: InputError
  if(d1 < UT_TOL_min0) return -1;



  // wenn d1 = radius: pt1=pc; nur 1 Loesung.
  if(UTP_comp2db (d1,rdc,UT_TOL_pt) == 1) {
    cia[0].pc = pt1;
    return 1;
  }


  // wenn d1 > radius: keine Loesung.
  if(d1 > rdc) return 0;


  // d3 = fehlende Seitenlaenge;
  UT2D_solvtriri_a (&d3, d1, rdc);

  // vc1 = pp1 -> pp2
  UT3D_vc_2pt (&vc1, pp1, pp2);

  // vc2 = der vcx des neuen Kreises: kreuzVec aus vz - vc1.
  UT3D_vc_perp2vc (&vc2, &vc1, vz);
    // GR_tDyn_vc__ (&vc2, &pt1, 2, 1);
  // cen1/2 = vom Mittelpunkt pt1 um d3 Richtg vc2 verschieben

  // modifier (aus_tab[3]) 0 od 1.
  UT3D_pt_traptvclen (&c1->pc, &pt1, &vc2, d3);
  UT3D_pt_opp2pt (&cia[1].pc, &pt1, &c1->pc);

    // GR_tDyn_symB__ (&cia[0].pc, SYM_STAR_S, 2);
    // GR_tDyn_symB__ (&cia[1].pc, SYM_STAR_S, 2);
    // GR_Disp_ac (&cia[0], 9);
    // GR_Disp_ac (&cia[1], 9);
    // DEB_dump_obj__(Typ_CI, &cia[0], "ex UT3D_ci_2ptvcrd-0\n");
    // DEB_dump_obj__(Typ_CI, &cia[1], "ex UT3D_ci_2ptvcrd-1\n");

  return 2;

}


//============================================================================
  int UT3D_ci_ptptvcrd (Circ *ci,
                        Point *p1, Point *p2, double rd, Vector *vz,
                        int sr, int iMod) {
//============================================================================
/// \code
/// UT3D_ci_ptptvcrd         Circ from startpoint endpoint radius Z-vector
///   limited circle.
/// 
/// rd      Radius; r>0 = CCW, r<0 = sr inverted.
/// sr      sense of rotation; 1=CCW; -1=CW
/// iMod 0  0-180 deg
/// iMod 1  180-360 deg
/// 
/// Retcod:
///     -1  p1 - p2 ident
///     -2  Radius too small       (p1-p2) > (rd * 2)
/// \endcode


  double dx, dy, rda;
  Point  pm;
  Vector vx, vy;


  // printf("UT3D_ci_ptptvcrd rd=%f\n",rd);
    // GR_tDyn_symB__ (p1, SYM_STAR_S, 0);
    // GR_tDyn_symB__ (p2, SYM_STAR_S, 0);

  rda = fabs(rd);

  if(rd < 0.) sr *= -1;

  ci->p1   = *p1;
  ci->p2   = *p2;
  ci->vz   = *vz;
  ci->rad  = rda * sr;
    // printf(" sr=%d rad=%lf iMod=%d\n",sr,ci->rad,iMod);


  // pm=Mittelpunkt p1-p2
  UT3D_pt_mid2pt (&pm, p1, p2);
    // GR_tDyn_symB__ (&pm, SYM_STAR_S, 1);

  // vy = p1-p2;
  UT3D_vc_2pt (&vy, p1, p2);

  // vx = durch pm, normal auf vy und vz.
  UT3D_vc_perp2vc (&vx, vz, &vy);

  // dy = (p1 - p2) / 2
  dy = UT3D_len_2pt (p1, p2) / 2.;
    // printf(" dy=%f\n",dy);
  if(dy < UT_TOL_pt) return -1;

  if((dy - rda) > UT_TOL_pt) return -2;

  // Halbkreis -
  if(fabs(dy - rda) < UT_TOL_pt) {
    ci->pc = pm;
    ci->ango = sr * RAD_180;
    goto L_exit;
  }

  // dx aus:   rd =  sqrt (dy **2 + dx ** 2)
  UT2D_solvtriri_a (&dx, dy, rda);

  // enstprechen siz dx in Richtg vx od invers dazu
  if(sr > 0) {  // CCW
    if(iMod == 0) { // CCW < 180
      ;
    } else {        // CCW > 180
      dx = -dx;
    }

  } else {      // CW
    if(iMod == 0) { // CW < 180 
      dx = -dx;
    } else {        // CW > 180
      ;
    }

  }


  UT3D_pt_traptvclen (&ci->pc, &pm, &vx, dx);

  L_go:
  // GR_tDyn_symB__ (&ci->pc, SYM_STAR_S, 2);

  ci->ango = UT3D_angr_ci_p1_pt (ci, &ci->p2);

  L_exit:
    // GR_Disp_ac (ci, 2);
  return 0;

}


//======================================================================
  int UT3D_ci_pcvzpt180 (Circ *ci1,Point *pc,Vector *vz,Point *p1,int dreh) {
//======================================================================
/// UT3D_ci_pcvzpt180         180 deg-circ (CCW) from center, axis, startpoint


  // DEB_dump_obj__ (Typ_PT, p1, "UT3D_ci_pcvzpt180 ");

  ci1->pc = *pc;
  ci1->p1 = *p1;
  ci1->vz = *vz;

  UT3D_pt_opp2pt (&ci1->p2, &ci1->pc, &ci1->p1);
  ci1->ango = RAD_180;
  ci1->rad  = UT3D_len_2pt(pc, p1);

  if(dreh < 0) { ci1->ango = -ci1->ango; ci1->rad = -ci1->rad; }

  // DEB_dump_obj__ (Typ_CI, ci1, "ex UT3D_ci_pcvzpt180:\n");

  return 0;

}


//=======================================================================
  int UT3D_ci_inv1 (Circ *ci1) {
//=======================================================================
/// \code
/// UT3D_ci_inv1              invert (change p1, p2, dir)
/// Kreis bleibt optisch ganz gleich !!
/// \endcode


  // printf("UT3D_ci_inv1\n");
  // DEB_dump_obj__ (Typ_CI, ci1, "UT3D_ci_inv1");


  MEM_swap__ (&ci1->p1, &ci1->p2, sizeof(Point));

  ci1->rad  = -ci1->rad;
  ci1->ango = -ci1->ango;

  return 0;

}

//=======================================================================
  int UT3D_ci_inv2 (Circ *ci1) {
//=======================================================================
/// \code
/// UT3D_ci_inv2              reverse Circ (open circle: complementary circ);
///   closed circle: reverse direction.
/// \endcode


  // printf("UT3D_ci_inv2\n");


  // MEM_swap__ (&ci1->p1, &ci1->p2, sizeof(Point));

  ci1->rad  = -ci1->rad;

  // closed circle: reverse direction
  if (UT3D_comp2pt (&ci1->p1, &ci1->p2, UT_TOL_min0)) {
    // reverse
    ci1->ango = RAD_360 * DSIGN(ci1->rad);
    return 0;
  }

  if(ci1->rad > 0.) {             // 2014-05-13
    ci1->ango += RAD_360;
  } else {
    ci1->ango = (ci1->ango - RAD_360) * -1.;
  }

  return 0;

}


//=======================================================================
  int UT3D_ci_inv3 (Circ *ci1) {
//=======================================================================
/// UT3D_ci_inv3              invert (optisch gleich, Z-Axis invers)

  // printf("UT3D_ci_inv3\n");

  MEM_swap__ (&ci1->p1, &ci1->p2, sizeof(Point));

  UT3D_vc_invert (&ci1->vz, &ci1->vz);

  return 0;

}


//=======================================================================
  int UT3D_ci_ptvcr (Circ *ci1, Point *pc, Vector *vcz, double rdc) {
//=======================================================================
/// UT3D_ci_ptvcr             Circ from center, axis, radius

  Vector vc1;


  // printf("UT3D_ci_ptvcr r=%f\n",rdc);
  // DEB_dump_obj__(Typ_PT, pc, " ");
  // DEB_dump_obj__(Typ_VC, vcz, " ");

  ci1->pc   = *pc;
  ci1->vz   = *vcz;
  ci1->rad  = rdc;
  ci1->ango = RAD_360;


  // create the X-vector = Normalvektor auf vc, der in der X-Y-Plane liegt.
  UT3D_vc_perpvcplXY (&vc1, vcz);
  UT3D_pt_traptvclen (&ci1->p1, pc, &vc1, rdc);
  ci1->p2 = ci1->p1;



  return 0;

}


//=======================================================================
  int UT3D_ci_pt2vcr (Circ *ci1, Point *pc, Vector *vz, Vector *vx, double rc) {
//=======================================================================
/// UT3D_ci_pt2vcr             Circ from center, axis, x-vec and radius

  Vector vc1;


  // printf("UT3D_ci_pt2vcr r=%f\n",rc);
  // DEB_dump_obj__(Typ_PT, pc, " ");
  // DEB_dump_obj__(Typ_VC, vcz, " ");
  // DEB_dump_obj__(Typ_VC, vcx, " ");


  ci1->pc   = *pc;
  ci1->vz   = *vz;
  ci1->rad  = rc;
  ci1->ango = RAD_360;


  // create p1 from pc, X-vector and vc-Len
  UT3D_pt_traptvclen (&ci1->p1, pc, vx, rc);
  ci1->p2 = ci1->p1;


  return 0;

}


//====================================================================
  void UT3D_ci_ci2 (Circ *cio, Circ2 *ci20) {
//====================================================================
/// UT3D_ci_ci2               3D-Circ = 2D-Circ; (add z-coord 0, Z-vec=0,0,1)


  cio->rad  = ci20->rad;
  cio->ango = ci20->ango;

  cio->p1.x = ci20->p1.x;
  cio->p1.y = ci20->p1.y;
  cio->p1.z = 0.0;

  cio->p2.x = ci20->p2.x;
  cio->p2.y = ci20->p2.y;
  cio->p2.z = 0.0;

  cio->pc.x = ci20->pc.x;
  cio->pc.y = ci20->pc.y;
  cio->pc.z = 0.0;

  cio->vz = UT3D_VECTOR_Z;

}



//====================================================================
  Circ   UT3D_ci_obj2 (ObjG2 *ci20) {
//====================================================================
// UT3D_ci_obj2              3D-Circ = 2D-ObjG
/// DO NOT USE
// Eigentlich sollte als ci1.vz noch ein z-Vektor zu !!


  Circ    ci1;


  ci1.rad   = ci20->rad;

  ci1.p1.x = ci20->p1.x;
  ci1.p1.y = ci20->p1.y;
  ci1.p1.z = 0.0;

  ci1.p2.x = ci20->p2.x;
  ci1.p2.y = ci20->p2.y;
  ci1.p2.z = 0.0;

  ci1.pc.x = ci20->pc.x;
  ci1.pc.y = ci20->pc.y;
  ci1.pc.z = 0.0;

  ci1.vz   = UT3D_VECTOR_Z;

  ci1.ango = UT3D_angr_ci_p1_pt (&ci1, &ci1.p2);

  /* printf("UT3D_ci_obj2 %f,%f bis %f,%f\n",ci1.p1.x,ci1.p1.y,
                                             ci1.p2.x,ci1.p2.y); */
  /* printf("          um %f,%f rad=%f\n",ci1.pc.x,ci1.pc.y,ci1.rad); */

  return ci1;

}



/* UNUSED
//================================================================
  Circ   UT3D_ci_obj (ObjG *ci_in) {
//================================================================
// UT3D_ci_obj2              3D-Circ = 2D-ObjG
// Eigentlich sollte als ci1.vz noch ein z-Vektor zu !!
/// DO NOT USE

  Circ    ci1;


  ci1.rad  = ci_in->rad;

  ci1.p1 = ci_in->p1;
  ci1.p2 = ci_in->p2;
  ci1.pc = ci_in->pc;

  ci1.vz = ci_in->vz;

  ci1.ango = UT3D_angr_ci__ (&ci1);

  // printf("UT3D_ci_obj2 %f,%f bis %f,%f\n",ci1.p1.x,ci1.p1.y,
  //                                         ci1.p2.x,ci1.p2.y);
  // printf("          um %f,%f rad=%f\n",ci1.pc.x,ci1.pc.y,ci1.rad);

  return ci1;

}
*/


/* UNUSED
//=========================================================================
  int UT3D_civz_ci (Circ *ci1) {
//=========================================================================
/// UT3D_civz_ci              den vz eines Circ (aus p1-pc-p2) errechnen


  Vector    vc1, vc2;



  UT3D_vc_2pt (&vc1, &ci1->pc, &ci1->p1);
  UT3D_vc_2pt (&vc2, &ci1->pc, &ci1->p2);



  //  Ist es ein Vollkreis ? //
  // if(UT3D_comp2pt(&ci1->p1, &ci1->p2, UT_TOL_pt)) { //
  if((UTP_compdb0(vc1.dx-vc2.dx,UT_TOL_pt))    &&
     (UTP_compdb0(vc1.dy-vc2.dy,UT_TOL_pt))    &&
     (UTP_compdb0(vc1.dz-vc2.dz,UT_TOL_pt))) {
    // TX_Print("Vollkreis"); //
    goto Error_1;
  }

  //  Ist es ein Halbkreis - antiparallel ? //
  if((UTP_compdb0(vc1.dx+vc2.dx,UT_TOL_pt))    &&
     (UTP_compdb0(vc1.dy+vc2.dy,UT_TOL_pt))    &&
     (UTP_compdb0(vc1.dz+vc2.dz,UT_TOL_pt))) {
    // TX_Print("Halbkreis"); //
    goto Error_1;
  }

  // TX_Print("     vc1=%f,%f",vc1.dx,vc1.dy); //
  // TX_Print("     vc2=%f,%f",vc2.dx,vc2.dy); //

  UT3D_vc_perp2vc (&ci1->vz, &vc1, &vc2);

  Fertig:

//
  TX_Print("ex UT3D_civz_ci r=%f",ci1->rad);
  TX_Print("   p1 = %f,%f,%f",ci1->p1.x,ci1->p1.y,ci1->p1.z);
  TX_Print("   p2 = %f,%f,%f",ci1->p2.x,ci1->p2.y,ci1->p2.z);
  TX_Print("   pc = %f,%f,%f",ci1->pc.x,ci1->pc.y,ci1->pc.z);
  TX_Print("   vz = %f,%f,%f",ci1->vz.dx,ci1->vz.dy,ci1->vz.dz);
//

  return 1;

  Error_1:
  //  wenn alle 3 Z-Werte gleich, dann vz = UT3D_VECTOR_Z; sonst Error. //
  if((UTP_comp2db(ci1->pc.z,ci1->p1.z,UT_TOL_pt))  &&
     (UTP_comp2db(ci1->pc.z,ci1->p2.z,UT_TOL_pt)))     {
    ci1->vz = UT3D_VECTOR_Z;
    goto Fertig;
  } else {
    TX_Error("Kreisbogen mit 180 od. 360 Grad braucht Z-Vektor");
    return 0;
  }

}
*/




/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/



//============================================================================
  int UT3D_pta_dbo (Point **pTab, int *pNr, int typ, long ind, double tol) {
//============================================================================
/// \code
/// UT3D_pta_dbo              get polygon from DB-object(typ,ind)
/// returns points in memspc201 ! (do not free)
/// \endcode

  int        i1, plgSiz;
  Point      *pPlg;
  ObjGX      ox1;

// extern:
  ObjGX     DB_GetObjGX ();


  // get ObjGX from typ, DB-index
  ox1 = DB_GetObjGX (typ, ind);
  if(ox1.typ == Typ_Error) return -1;


  pPlg = *pTab;


  // decode; get polygon from DB-object.
  i1 = UT3D_npt_ox__ (pNr, pPlg, &ox1, tol);
  if(i1 < 0) return -1;

    // printf("---------- polygon from %d points -\n",*pNr);
    // for(i1=0;i1< *pNr;++i1) DEB_dump_obj__ (Typ_PT,&pPlg[i1],"p[%d]=",i1);

  *pTab = pPlg;

  return 0;

}


/* UU
//====================================================================
  Curv UT3D_cv_obj2 (ObjG2 *o1) {
//====================================================================
/// UT3D_cv_obj2              Polygon < 2D-Obj


  Curv cv1;


  cv1.typ = Typ_CVPOL2;

  cv1.p1.x = o1->p1.x;
  cv1.p1.y = o1->p1.y;

  cv1.p2.x = o1->p2.x;
  cv1.p2.y = o1->p2.y;

  cv1.cvnr = o1->rad;

  cv1.ind1 = o1->pc.x;
  cv1.ind2 = o1->pc.y;

  // printf("exit UT2D_obj_ln3 %f,%f bis %f,%f\n",o1.p1.x,o1.p1.y,
                                                  o1.p2.x,o1.p2.y);

  return cv1;

}
*/



//===============================================================
  void UT3D_cv_ln (Point *cv, int *ptAnz, Point *p1, Point *p2) {
//===============================================================
/// \code
/// Linearstueck -> Polygon.
/// 
/// In/Out: *ptAnz wird nur in 2 geandert, wenn < 2.
/// \endcode



  int    ind, iend, ianz;
  double dx, dy, dz;
  Vector vc;


/*
  printf("UT3D_cv_ln %d\n",*ptAnz);
  printf("           %f,%f,%f\n",p1->x,p1->y,p1->z);
  printf("           %f,%f,%f\n",p2->x,p2->y,p2->z);
*/


  // erster Pt
  cv[0] = *p1;


  if(*ptAnz < 2) {
    *ptAnz = 2;
    ind = 0;
    goto L_fertig;
  }


  UT3D_vc_2pt (&vc, p1, p2);

  ianz = *ptAnz - 1;
  dx = vc.dx / ianz;
  dy = vc.dy / ianz;
  dz = vc.dz / ianz;


  ind  = 0;
  iend = *ptAnz-2;


  L_nxt_pt:
  ++ind;

  cv[ind].x = p1->x + (dx * ind);
  cv[ind].y = p1->y + (dy * ind);
  cv[ind].z = p1->z + (dz * ind);


  if(ind < iend) goto L_nxt_pt;



  L_fertig:
  ++ind;
  cv[ind] = *p2;


/*
  // nur Testausg:
  for(ind=0; ind<*ptAnz; ++ind) {
    printf("        %d %f,%f,%f\n",ind,cv[ind].x,cv[ind].y,cv[ind].z);
  }
*/

  return;

}


//==========================================================================
  void UT3D_cv_ci (Point cv[], int *ptanz, Circ *ci1, int ptmax, double tol) {
//==========================================================================
/// \code
/// UT3D_cv_ci                Polygon < 3D-Kreis
/// IN:
///   ptmax   max. nr of outputpoints
///   tol     maximal deviation of secant;
///           if(tol <0) then return fixed nr of points (ptmax)
///   ptanz   number of outputpoints (only if stol<0)
/// OUT:
///   Point cv[]    ...  array of polygonpoints;
///   ptanz   number of outputpoints
///
/// \endcode

  int    pNr, i1;


  // printf("UT3D_cv_ci tol=%f ptmax=%d ptanz=%d\n",tol,ptmax,*ptanz);
  // DEB_dump_obj__ (Typ_CI, ci1, "UT3D_cv_ci\n");


  // fix nr of points according to modelsize
  if (tol < 0.0) {
    pNr = ptmax;
  } else {
    pNr = UT2D_ptNr_ci (fabs(ci1->rad), fabs(ci1->ango), tol);
    if(pNr > ptmax)  pNr = ptmax;
  }
     // printf(" pNr=%d\n",pNr);

  // circ -> polygon
  UT3D_npt_ci (cv, pNr, ci1);

  *ptanz = pNr;

/*
  int      i1, i2, ianz;
  double   wi, wu, a1, w_tol, rd1;
  Mat_4x4  m1;

  //  den Umfangswinkel wu errechnen
  // wu = UT3D_angr_ci__ (ci1);
  wu = fabs(ci1->ango);
    // printf(" AC-Oeffnungswinkel wu=%f\n",wu);


  //  die Anzahl Punkte ianz feststellen
  // die Anzahl von Segmenten sowie der hoechste Punktindex ist 1 niedriger
  if (tol < 0.0) {
    ianz = ptmax;

  } else {
    ianz = UT3D_ptNr_ci (ci1, tol);
    if(ianz > ptmax) ianz=ptmax;
      // printf(" ianz=%d wu=%f w_tol=%f\n",ianz,wu,w_tol);
  }


  //  den Incrementalwinkel wi errechnen
  wi = wu / (ianz - 1);
  if(ci1->rad < 0.) wi = -wi;
    // printf(" wu=%f ianz=%d wi=%f w_tol=%f\n",wu,ianz,wi,w_tol);


  //  rund um eine Achse im Nullpunkt drehen
  cv[0].x = ci1->p1.x - ci1->pc.x;
  cv[0].y = ci1->p1.y - ci1->pc.y;
  cv[0].z = ci1->p1.z - ci1->pc.z;


  if(ianz < 2) { ianz = 2; goto Fertig;}


  // UT3D_m4_init_ori (m1, ci1->pc.x, ci1->pc.y, ci1->pc.z);
  // UT3D_m4_init_rot (m2, wi, &ci1->vz);
  // //UT3D_m4_addtra (m1, ci1->pc.x, ci1->pc.y, ci1->pc.z);
  // UT3D_m4_addrot (m1, m1, m2);


  UT3D_m4_init_rot (m1, wi, &ci1->vz);




  //  die Punkte rumdrehen
  i2=ianz-1;
  for(i1=1; i1<i2; ++i1) {
    UT3D_pt_tra_pt_m4 (&cv[i1], m1, &cv[i1-1]);
  }



  Fertig:

  //  den Mittelpunkt noch aufaddieren
  i2=ianz-1;
  for(i1=0; i1<i2; ++i1) {
    cv[i1].x += ci1->pc.x;
    cv[i1].y += ci1->pc.y;
    cv[i1].z += ci1->pc.z;
    // TX_Print(" %d %f,%f,%f",i1,cv[i1].x,cv[i1].y,cv[i1].z);
  }


  // Endpunkt
  i1=ianz-1;
  cv[i1] = ci1->p2;


  *ptanz=ianz;
*/


  // Testausgaben:
  // printf("ex UT3D_cv_ci %d\n",*ptanz);
  // for(i1=0; i1 < *ptanz; ++i1) {
    // GR_tDyn_symB__ (&cv[i1], SYM_TRI_S, 2);
//     // GR_tDyn_txiA (&cv[i1], i1, 0);
    // DEB_dump_obj__ (Typ_PT, &cv[i1], " P[%d]=",i1);
  // }
  // END TESTBLOCK

  return;

}


//================================================================
  int UT3D_npt_ci (Point *pa, int pNr, Circ *ci1) {
//================================================================
/// \code
/// UT3D_npt_ci               circular polygon
///
/// see also UT3D_cv_ci
/// \endcode


  int        i1, segNr;
  double     ai;
  Mat_4x4    m1;


  // printf("UT3D_npt_ci %d\n",pNr);
  // DEB_dump_obj__ (Typ_CI, ci1, "\n",i1);

  segNr = pNr - 1;

  if(pNr < 3) {
    pa[0] = ci1->p1;
    goto Fertig;
  }


  // den Incrementalwinkel ai errechnen
  ai = fabs(ci1->ango) / segNr;
  if(ci1->rad < 0.) ai = -ai;
    // printf(" wu=%f ianz=%d wi=%f w_tol=%f\n",wu,ianz,wi,w_tol);


  // init rotation around ?
  UT3D_m4_init_rot (m1, ai, &ci1->vz);

  // geht ned, weil um Achse durch 0-Punkt gedreht wird ..
  // m1[3][0] = ci1->pc.x;
  // m1[3][1] = ci1->pc.y;
  // m1[3][2] = ci1->pc.z;


  // startpt
  pa[0] = ci1->p1;


  //  rund um eine Achse im Nullpunkt drehen
  pa[0].x = ci1->p1.x - ci1->pc.x;
  pa[0].y = ci1->p1.y - ci1->pc.y;
  pa[0].z = ci1->p1.z - ci1->pc.z;


  //  die Punkte rumdrehen
  for(i1=1; i1<segNr; ++i1) {
    UT3D_pt_tra_pt_m4 (&pa[i1], m1, &pa[i1-1]);
  }



  // Mittelpunkte aufaddieren
  for(i1=0; i1<segNr; ++i1) {
    pa[i1].x += ci1->pc.x;
    pa[i1].y += ci1->pc.y;
    pa[i1].z += ci1->pc.z;
    // TX_Print(" %d %f,%f,%f",i1,cv[i1].x,cv[i1].y,cv[i1].z);
  }


  // Endpunkt
  Fertig:
  pa[segNr] = ci1->p2;


  // Testausgaben:
  // printf("ex UT3D_npt_ci %d\n",pNr);
  // for(i1=0; i1<pNr; ++i1) {
    // GR_tDyn_symB__ (&pa[i1], SYM_TRI_S, 2);
    // GR_tDyn_txiA (&pa[i1], i1, 0);
    // GR_tDyn_pcv (pa, pNr, 9);
    // DEB_dump_obj__ (Typ_PT, &cv[i1], " P[%d]=",i1);
  // }


  return 0;

}

//================================================================
  int UT3D_pta_rot__ (Point *p2Tab, Point *p1Tab, int ptNr,
                      Point *pc, Vector *vz, double ar) {
//================================================================
/// rotate curve CCW um axis ..

  int       i1;
  Mat_4x3   ma;

  // printf("UT3D_pta_rot__ %f\n",ar);

  if(fabs(ar) < UT_TOL_Ang1) {
    memcpy(p2Tab, p1Tab, sizeof(Point)*ptNr);
    return 0;
  }



  // den angle verkehrt, da die UT3D_pt_rotptm3 scheinbar nicht CCW
  // sondern CW dreht !
  UT3D_m3_inirot_angr (ma, pc, vz, -ar);

  for(i1=0; i1<ptNr; ++i1)
    UT3D_pt_rotptm3 (&p2Tab[i1], &p1Tab[i1], ma);

  // GR_tDyn_pcv (p2Tab, ptNr, 9);

  return 0;

}


//======================================================================
  int UT3D_cv_inv (int ptNr, Point *pta) {
//======================================================================
/// UT3D_cv_inv               Punktetabelle umdrehen

  int    i1;
  int    inach, ivon, iSiz;


  // printf("UT3D_cv_inv %d\n",ptNr);
  // for(i1=0;i1<ptNr;++i1) DEB_dump_obj__(Typ_PT,&pta[i1],"pta[%d]",i1);



  iSiz = sizeof(Point);
  inach = 0;
  ivon = ptNr - 1;


  L_umd_next:
  // printf("   vert. %d -> %d\n",ivon,inach);
  MEM_swap__ (&pta[inach], &pta[ivon], iSiz);
  ++inach;
  --ivon;
  if(inach < ivon) goto L_umd_next;


  // printf("ex UT3D_cv_inv %d\n",ptNr);
  // for(i1=0;i1<ptNr;++i1) DEB_dump_obj__(Typ_PT,&pta[i1],"pta[%d]",i1);


  return 0;

}



//======================================================================
  int UT3D_cv_realg (int ptNr, Point *pta, int ista) {
//======================================================================
/// \code
/// UT3D_cv_realg             Punktetabelle umschichten (realign)
/// pta[0] soll pta[ista] sein und dann alle anschliessenden
/// pta[ista] ist also der erste Punkt, dann folgen alle weiteren.
/// \endcode


  int    i1, isiz;
  Point  paAux[1000];


  // printf("UT3D_cv_realg %d %d\n",ptNr,ista);

  if(ista == 0) return 0;

  if(ista >= 1000) {
    TX_Error("UT3D_cv_realg E001");
    return-1;
  }


  isiz = sizeof(Point)*(ista);
  memcpy (paAux, pta, isiz);
  memcpy (pta, &pta[ista], sizeof(Point)*(ptNr-ista));
  memcpy (&pta[ptNr-ista], paAux, isiz);


  
  // nur TEST
  // printf("ex UT3D_cv_realg %d %d\n",ptNr,ista);
  // for(i1=0; i1<ptNr; ++i1) {
    // DEB_dump_obj__ (Typ_PT, &pta[i1], "P[%d]=",i1);
  // }


  return 0;

}


//===================================================================
  int UT3D_ptvc_int2pl (Point *pt, Vector *vc, Plane *pl1, Plane *pl2) {
//===================================================================
/// \code
/// UT3D_ptvc_int2pl          point/vector = intersection of 2 planes
/// 
/// Retcodes:
///   1 - OK
///   0 - no intersection; planes parallel
/// Intersectionpoint is nearest point to both origins.
/// \endcode


  int       irc;
  double    l1, dl, d1, d2;
  Point     ptl;
  Vector    vcl, v1, v2;


  //----------------------------------------------------------------
  // printf("UT3D_ptvc_int2pl\n");


  /* direction of intersecline */
  UT3D_vc_perp2vc (vc, &pl1->vz, &pl2->vz);
  l1 = vc->dx * vc->dx + vc->dy * vc->dy + vc->dz * vc->dz;
  // printf("vc=%f,%f,%f, l=%f\n",vc->dx,vc->dy,vc->dz,l1);


  // check for planes parallel 
  if (UTP_comp_0 (l1)) {
    irc = 0;
    goto Done;
  }


  dl = sqrt(l1);
  vc->dx /= dl;
  vc->dy /= dl;
  vc->dz /= dl;


  d1 = (pl1->po.x * pl1->vz.dx +
        pl1->po.y * pl1->vz.dy +
        pl1->po.z * pl1->vz.dz) / dl;

  d2 = (pl2->po.x * pl2->vz.dx +
        pl2->po.y * pl2->vz.dy +
        pl2->po.z * pl2->vz.dz) / dl;


  v1.dx = pl2->vz.dy * vc->dz - pl2->vz.dz * vc->dy;
  v1.dy = pl2->vz.dz * vc->dx - pl2->vz.dx * vc->dz;
  v1.dz = pl2->vz.dx * vc->dy - pl2->vz.dy * vc->dx;

  v2.dx = vc->dy * pl1->vz.dz - vc->dz * pl1->vz.dy;
  v2.dy = vc->dz * pl1->vz.dx - vc->dx * pl1->vz.dz;
  v2.dz = vc->dx * pl1->vz.dy - vc->dy * pl1->vz.dx;

  pt->x = d1 * v1.dx + d2 * v2.dx;
  pt->y = d1 * v1.dy + d2 * v2.dy;
  pt->z = d1 * v1.dz + d2 * v2.dz;


/*
  // ptl = project origin of pl1 > pl2
  UT3D_pt_projptpl (&ptl, pl2, &pl1->po);
    // printf(" ptl=%f,%f,%f\n",ptl.x,ptl.y,ptl.z);
    // printf(" pl2.po=%f,%f,%f\n",pl2->po.x,pl2->po.y,pl2->po.z);

  // if ptl - pl2.origin identical: that point is OK
  if (UT3D_comp2pt (&ptl, &pl2->po, UT_TOL_min1)) {
    // pl2->po == ptl.

    UT3D_pt_projptpl (&ptl, pl1, &pl2->po);
FEHLER: pos's koennen gleich sein !
    // intersect Line (pt - pl1.origin) with pl2
    UT3D_vc_2pt (&vcl, &pl1->po, &ptl);
    UT3D_pt_intptvcpl_ (pt, pl2, &ptl, &vcl);

  } else {

    // intersect Line (pt - pl2.origin) with pl1
    UT3D_vc_2pt (&vcl, &pl2->po, &ptl);

    // intersect ptl-vcl mit plane1
    UT3D_pt_intptvcpl_ (pt, pl1, &ptl, &vcl);
  }
  // printf(" vcl=%f,%f,%f\n",vcl.dx,vcl.dy,vcl.dz);
*/

  irc = 1;

  Done:

  // printf("ex UT3D_ptvc_int2pl irc=%d\n",irc);
  // printf("      pt=%f %f %f\n",pt->x,pt->y,pt->z);
  // printf("      vc=%f %f %f\n",vc->dx,vc->dy,vc->dz);


  return irc;
}


//================================================================
  int UT3D_ptvc_int2pln (Point *pti, Vector *vci,
                         Point *pl1pt, Vector *pl1vz,
                         Point *pl2pt, Vector *pl2vz) {
//================================================================
/// \code
/// UT3D_ptvc_int2pln       point/vector = intersection of 2 planes
/// Planes durch Nullpunkt und Normalvektoren.
/// pl2vz muss nicht normiert sein;
/// vci (out) ist normiert.
///   See also UT3D_ln_int2pl UT3D_ptvc_int2pl UT3D_pt_intptvcpl
/// Returncodes:
///   0 = OK
///   1 = no intersection (parallel planes)
/// \endcode



  double  lvl, d1, d2;
  Vector  v1, v2;


  // GR_tDyn_vc__ (pl1vz, pl1pt, 3, 0);
  // GR_tDyn_vc__ (pl2vz, pl2pt, 3, 0);


  // Richtg der intLn = crossprod
  UT3D_vc_perp2vc (vci, pl1vz, pl2vz); // x-vec plane 1


  // check if planes are parallel
  lvl = vci->dx * vci->dx + vci->dy * vci->dy + vci->dz * vci->dz;
  if(lvl < UT_TOL_min1) {
    // printf("UT3D_ln_int2pl - parallel\n");
    return 1;
  }


  // // normalize line vector
  lvl = sqrt (lvl);
  d1 = 1. / lvl;
  vci->dx *= d1;
  vci->dy *= d1;
  vci->dz *= d1;
    // DEB_dump_obj__(Typ_VC, vci, "  vci");


  UT3D_vc_perp2vc (&v1, pl2vz, vci); // x-vec plane 2
  UT3D_vc_perp2vc (&v2, vci, pl1vz); // x-vec plane 1
    // GR_tDyn_vc__ (&v1, pl2pt, 1, 0);
    // GR_tDyn_vc__ (&v2, pl1pt, 1, 0);


  d1 = pl1pt->x*pl1vz->dx + pl1pt->y*pl1vz->dy + pl1pt->z*pl1vz->dz;
  d1 /= lvl;
  d2 = pl2pt->x*pl2vz->dx + pl2pt->y*pl2vz->dy + pl2pt->z*pl2vz->dz;
  d2 /= lvl;
  // printf(" d1=%f d2=%f\n",d1,d2);


  // intersection line
  pti->x = d1 * v1.dx + d2 * v2.dx;
  pti->y = d1 * v1.dy + d2 * v2.dy;
  pti->z = d1 * v1.dz + d2 * v2.dz;


  // GR_tDyn_symB__ (pti, SYM_TRI_S, 2);
  // GR_tDyn_vc__ (vci, pti, 2, 0);

  return 0;

}


//================================================================
  int UT3D_ptvc_ox (Point *pta, Vector *vca, ObjGX *oxi) {
//================================================================
/// \code
/// UT3D_ptvc_ox              get axis (PT+VC) from PT|LN|CI|PLN
/// retCod     0=OK-pta-vca; 1=vca-only; -1=error
/// replaced by UT3D_cvLn_obj
/// \endcode
// TODO: use UT3D_cvLn_obj

  int    irc = 0;
  CVLn3  cvl;

/*
  // get pt/vector from 
  irc = UT3D_cvLn_obj (&cvl, oxi->form, oxi->data);
  if(irc < 0) return irc;

  *pta = cvl->pt0;
  *vca = cvl->vcl;
*/

    // rotAxis setzen; Punkt TSU_prx + Vector *vca
    if(oxi->form == Typ_PT) {
     *pta = *((Point*)oxi->data);
     *vca = UT3D_VECTOR_Z;

    } else if(oxi->form == Typ_LN) {
     *pta = ((Line*)oxi->data)->p1;
     UT3D_vc_ln (vca, (Line*)oxi->data);
     UT3D_vc_setLength (vca, vca, 1.);

    } else if(oxi->form == Typ_CI) {
     *pta = ((Circ*)oxi->data)->pc;
     *vca = ((Circ*)oxi->data)->vz;

    } else if(oxi->form == Typ_VC) {
     *vca = *((Vector*)oxi->data);
     irc = 1;

    } else if(oxi->form == Typ_PLN) {
     // DEB_dump_obj__ (Typ_PLN, TSU_obj1.vp, "Axis");
     *pta = ((Plane*)oxi->data)->po;
     *vca = ((Plane*)oxi->data)->vz;

    } else {
      TX_Error("UT3D_ptvc_ox E001");
      return -1;
    }


  // GR_tDyn_vc__ (vca, pta, 1, 0);

  return irc;

}

//=======================================================================
  int UT3D_pt_vc__par_cv (Point *pto, Vector *vct,
                             int oTyp, void *obj, int pTyp, double par) {
//=======================================================================
// UT3D_pt_vc__par_cv        get point/tangentVector from parameter on curve
//
// Input:
//   pto        set NULL for no output
//   vct        set NULL for no output
//   oTyp,obj   curve
//   pTyp       type of parameter;
//                0=knotvalue for splines,polygon; else parameter 0-1
//                1=normalized parameter (0-1); see INF_struct_par
//   par        parametervalue accord. pTyp
//
// Output:
//   pto        point; NULL for no output
//   vct        tangent-vector; NULL for no output
//
// was UTO_pt_evpar1ost
// see also UT3D_pt_evparcrv UT3D_vc_tng_crv_pt
// see also UTO_pt_eval_par1_dbo UT3D_ptvc_ox UT3D_ptvc_sur UT3D_ptvc_sus


  int     irc=0, ii, sTyp;
  long    sDbi;
  double  d1;
  void    *sDat;
  Vector  vc1;
  char    cv1[OBJ_SIZ_MAX];


  // printf("UT3D_pt_vc__par_cv par=%lf oTyp=%d pTyp=%d\n",par,oTyp,pTyp);
  // DEB_dump_obj__ (oTyp, obj, " ptvc_tng_crv_par-obj");


  switch (oTyp) {

    //----------------------------------------------------------------
    case Typ_PT:
      if(pto) *pto = *(Point*)obj;  // SRU_pt_evparcrv < SRU_init_basPts
      break;

    //----------------------------------------------------------------
    // case Typ_LN2:
      // UT2D_pt_tra_pt_pt_par (pto, &((Line2*)obj)->p1, &((Line2*)obj)->p2, du);

    // case Typ_CI2:
      // return UT2D_pt_par_ci  (pto, du, obj);

    //----------------------------------------------------------------
    case Typ_LN:
//TODO: native-parameter=lenght ?
      // UT2D_pt_parvc_pt_vc UT2D_pt_tra_pt_pt_par UT3D_pt_evpar2pt UT3D_pt__par_pt_vc
      // UT3D_pt_evparln 
      if(pto) UT3D_pt_evpar2pt (pto, par, &((Line*)obj)->p1, &((Line*)obj)->p2);
      if(vct) UT3D_vc_ln (vct, (Line*)obj);
      break;

    //----------------------------------------------------------------
    case Typ_CI:       // circle
      irc = UT3D_pt_vc__par_ci (pto, vct, obj, par);
      break;

    //----------------------------------------------------------------
    case Typ_CVELL:    // CurvElli
      irc = UT3D_ptvc_eval_ell_par (pto, vct, obj, par);
      break;

    //----------------------------------------------------------------
    case Typ_CVPOL:    // CurvPoly
      // UT3D_pt_evalplg UT3D_pt_evparcrv
      // change par 0-1  -->  dist
      if(pTyp) par = UT3D_par_par1plg (par, obj);
        // printf(" lpar=%f\n",lpar);
      if(pto) irc = UT3D_pt_evalplg (pto, obj, par);
      if(irc < 0) goto L_errEx;
      if(vct) irc = UT3D_vc_evalplg (vct, obj, par);
      break;

    //----------------------------------------------------------------
    case Typ_CVCLOT:
      irc = UT3D_ptvc_evparclot (pto, vct, pTyp, obj, par);
      break;

    //----------------------------------------------------------------
    case Typ_CVBSP:
      // fuer curves den parameter in einem Bereich 0-1 bringen ..
      // get knotValue from parameter 0-1
      if(pTyp) par = UT3D_parbsp_par1 (par, obj);
      // UT2D_pt_evpar_cbsp UT3D_pt_evparCrvBSpl UT3D_pt_evalparCv
      // bspl_pt_cbspDeg1
      if(pto) irc = UT3D_pt_evparCrvBSpl (pto, obj, par);
      // see also UT3D_pt_evalparCv
      if(irc < 0) goto L_errEx;
      // bspl_eval_Tg UT3D_vc_evalparCv
      if(vct) irc = UT3D_vc_evalparCv (vct, obj, par);
      break;

    //-------------------------------
    case Typ_CVRBSP:
      irc = UT3D_pt_vc__par_rbsp (pto, vct, par, pTyp, (CurvRBSpl*)obj, UT_TOL_cv);
      if(irc < 0) goto L_errEx;
      goto L_done;

    //----------------------------------------------------------------
    case Typ_CVTRM:
      // get basic-curve of vp2
      irc = CVTRM_basCv_trmCv (&sTyp, &sDbi, &sDat, obj);
      if(irc < 0) {TX_Error ("UT3D_par_pt__pt_prj_cv E2"); return -1; }
      if(((CurvCCV*)obj)->dir) {
        // reverse: change parameter
        if(pTyp) par = 1. - par;
        else     TX_Error("UT3D_pt_vc__par_cv E2%d",irc);
      }
      // recursion
      irc = UT3D_pt_vc__par_cv (pto, vct, sTyp, sDat, pTyp, par);
      goto L_done;

    //----------------------------------------------------------------
    default:
      TX_Error("not yet implemented - UT3D_pt_vc__par_cv E001 %d",oTyp);
      return -1;
      goto L_done;
  }


  //----------------------------------------------------------------
  L_done:

    // TESTBLOCK
    // if(pto) {
      // DEB_dump_obj__ (Typ_PT, pto, "ex-UT3D_pt_vc__par_cv:");
      // GR_tDyn_symB__ (pto, SYM_STAR_S, ATT_COL_BLUE);
    // }
    // if(vct) {
      // DEB_dump_obj__ (Typ_VC, vct, "ex-UT3D_pt_vc__par_cv:");
      // GR_tDyn_vc__ (vct, NULL, 9, 0);
    // }
    // END TESTBLOCK

  return irc;


  //----------------------------------------------------------------
  L_errEx:
    TX_Error("UT3D_pt_vc__par_cv E0%d",irc);
    return -1;

  L_errNYI:
    TX_Error("UT3D_pt_vc__par_cv - E1 - NOT YET IMPLEMENTED - %d",oTyp);
    return -1;



}


/*
//================================================================
  double UT3D_acos_vc2pt (Vector *v1, Point *p1, Point *p2) {
//================================================================
// Doku see UT3D_acos_2vc


//================================================================
  double UT3D_acos_2vc (Vector *v1, Vector *v2) {
//================================================================
UT3D_acos_2vc             cos of opening angle of 2 vecs (scalarproduct)

Das Skalarprodukt von 2 Vektoren liefert bei 90 Grad 0,
  bei 0-90 Grad einen Wert > 0, bei einem Winkel > 90 Grad einen Wert < 0.
  wenn normiert: bei 0 Grad (parallel) 1.0, bei -180 Grad (antiparallel) -1.0
  Wenn vc2 vom vc1 mehr als 90 Grad wegzeigt, ist SKP negativ.
  Wenn vc1 u. vc2 gleiche Laenge haben, entspr. SKP genau dem cos des
  Oeffnungswinkels.

Oeffnungswinkel ist ACOS(UT3D_acos_2vc(..));
  MUESSEN DIE WINKEL DAZU NORMIERT SEIN ?

3D-Projektionslaenge eines Vektors auf einen anderen Vektor.
 v2 wird auf v1 projiziert; out Laengenparameter (mit Vorzeichen!)
 Wenn v1 == X-Achse, waere Laenge der DX-Wert des v2 in der X-Y-Plane.
 
 Wahre_Laenge = sk / len_v1
   oder v1 normieren.



  double sk;

  sk = v1->dx * v2->dx + v1->dy * v2->dy + v1->dz * v2->dz;

  // printf("ex UT3D_acos_2vc %f\n",sk);

  return sk;



  //return (v1->dx * v2->dx + v1->dy * v2->dy + v1->dz * v2->dz);
}
*/


//================================================================
  double UT3D_angr_3pt (Point *p1, Point *pc, Point *p2) {
//================================================================
/// UT3D_angr_3pt                 angle between 3 points


  Vector    v1, v2;


    // GR_tDyn_symB__  (p1, SYM_STAR_S, 4);
    // GR_tDyn_symB__  (pc, SYM_STAR_S, 4);
    // GR_tDyn_symB__  (p2, SYM_STAR_S, 4);



  UT3D_vc_2pt (&v1, pc, p1);
  UT3D_vc_2pt (&v2, pc, p2);

  return UT3D_angr_2vc__ (&v1, &v2);

}


//================================================================
  double UT3D_angr_2vc__ (Vector *v1, Vector *v2) {
//================================================================
/// \code
/// UT3D_angr_2vc__d            angle between two vec's (always 0 <= PI)
///   v1, v2 must not be normalized.
/// see UT3D_angr_2vc_n
/// \endcode


  double   ang, pr;


  pr = UT3D_len_vc (v1) * UT3D_len_vc (v2);


  // check for zero - length
  if (fabs(pr) < UT_TOL_min2) {
    ang = 0.0;

  } else {
    ang = ACOS(UT3D_acos_2vc (v1, v2) / pr);

  }

  // printf("ex UT3D_angr_2vc__ %f %f\n",ang,UT_DEGREES(ang));

  return ang;

}


//================================================================
  double UT3D_angr_2vc_n (Vector *v1, Vector *v2) {
//================================================================
/// \code
/// UT3D_angr_2vc_n           angle between two normalized vec's
///   angle = always 0 <= PI;    direction is CCW OR CW.
/// for CCW-direction / angles (0 < 2PI)  use UT3D_angr_3vcn
/// 0.866,    0.500,    30 deg
/// 0.707,    0.707,    45 deg
/// 0.500,    0.866,    60 deg
/// \endcode


  // DEB_dump_obj__ (Typ_VC, v1, "v1");
  // DEB_dump_obj__ (Typ_VC, v2, "v2");

  return ACOS(UT3D_skp_2vc (v1, v2));

}


//=============================================================================
  double UT3D_angr_3vc__ (Vector *vz, Vector *v1, Vector *v2) {
//=============================================================================
/// \code
/// UT3D_angr_3vc__     angle between 2 vectors; vz gives direction (pos|neg)
///   RetCod: angle between v1 and v2;
/// range from pi to -pi
/// see UT3D_angr_2vc__
/// see UT3D_angr_ci_p1_pt
/// \endcode


  double    d1, ao, dCos;
  Vector    vc1, vc2, vcn;


  //  Länge 1 notwendig, sonst geht zB acos(3.5) nicht!
  UT3D_vc_normalize (&vc1, v1);
  UT3D_vc_normalize (&vc2, v2);

    // DEB_dump_obj__ (Typ_VC, &vc1, "vc1:");
    // DEB_dump_obj__ (Typ_VC, &vc2, "vc2:");


  //  ao = Oeffnungswinkel
  dCos = UT3D_acos_2vc (&vc1, &vc2);
  ao = ACOS(dCos);
  // printf("ao=%f\n",ao);


  // Normalvektor auf vc1 - vz: zeigt auf einen 90-Grad-Endpunkt
  UT3D_vc_perp2vc (&vcn, vz, &vc1);
    // DEB_dump_obj__ (Typ_VC, &vcn, "vcn:");


  // wenn Endpunkt weniger als 90 Grad Abweichung zum vcn hat, ist d1 > 0.
  // wenn vc2 oberhalb vc1 / vz liegt, ist d1 positiv; unterhalb negativ.
  d1 = UT3D_acos_2vc (&vcn, &vc2);
    // printf(" d1=%f, ao=%f\n",d1,ao);

  // if(d1 < 0.) ao = RAD_360 - ao;
  if(d1 < 0.) ao = -ao;

    // printf("ex UT3D_angr_3vc__ %f\n",ao);

  return ao;

}


//========================================================================
  double UT3D_angr_3vcn_CCW (Vector *vz, Vector *v1, Vector *v2) {
//=============================================================================
/// \code
/// UT3D_angr_3vcn_CCW    angle between 2 vectors CCW around vz; all normalized
///   Rotation CCW around vz
///   RetCod: angle CCW between v1 and v2;
/// Input:
///   v1, v2    compute angle between these vectors
///   vz        up-vector (necessary if angle > PI)
/// Output:
///   retCod    angle 0 < 2*PI
/// see UT3D_angr_2vc__ UT3D_angr_3vc__
/// see UT3D_angr_ci_p1_pt
/// \endcode


  int       svz;
  double    ao;
  Vector    vcn;


  // DEB_dump_obj__ (Typ_VC, v1, "v1:");
  // DEB_dump_obj__ (Typ_VC, v2, "v2:");
  // DEB_dump_obj__ (Typ_VC, vz, "vz:");

  //  ao = Oeffnungswinkel
  ao = ACOS(UT3D_acos_2vc (v1, v2));
    // printf("ao=%f\n",ao);


  // get the normalvector
  UT3D_vc_perp2vc (&vcn, v1, v2);
    // DEB_dump_obj__ (Typ_VC, &vcn, "vcn:");

  // side; 1=parl, -1=antiparl
  svz = UT3D_sid_2vc (&vcn, vz);
  if(svz < 0) ao = RAD_360 - ao;
    // printf(" svz=%d\n",svz);

    // printf("ex UT3D_angr_3vcn_CCW %f %f\n",ao,UT_DEGREES(ao));

  return ao;

}


//=======================================================================
  double UT3D_angr_4pt (Point *p11, Point *p12, Point *p21, Point *p22) {
//=======================================================================
/// UT3D_angr_4pt                 angle between 2 lines


  Vector    v1, v2;


    // GR_tDyn_symB__  (p1, SYM_STAR_S, 4);
    // GR_tDyn_symB__  (pc, SYM_STAR_S, 4);
    // GR_tDyn_symB__  (p2, SYM_STAR_S, 4);



  UT3D_vc_2pt (&v1, p11, p12);
  UT3D_vc_2pt (&v2, p21, p22);

  return UT3D_angr_2vc__ (&v1, &v2);

}


//================================================================
  double UT3D_angr_ci_p1_pt (Circ *ci1, Point *pti) {
//================================================================
/// \code
/// UT3D_angr_ci_p1_pt            opening angle of point on Circ
/// Returns the angle from ci1->p1 to pti.
///   angle 0 means that pti == ci1->p1;
///   angle 3.14 means that pti is opposite ci1->p1.
///
/// ATTENTION: DO NOT USE FOR 360-DEG-CIRCs: returns angle = 0.;
///  if(!memcpm(&ci1->p1, &pti, sizeof(Point))) printf(" 0-deg or 360-deg ??\n");
/// \endcode

// see also UT3D_ci_setangr UT3D_angr_ci__

  int       ii;
  double    ao;
  Vector    vc1, vc2;
  Point     pt1;


  // DEB_dump_obj__(Typ_PT, pti, "UT3D_angr_ci_p1_pt ");
  // DEB_dump_obj__(Typ_CI, ci1, "");


  // ident mit dem Startpunkt?
  // if(UT3D_comp2pt(&ci1->p1, pti, UT_TOL_pt) == 1) {
  if(UT3D_comp2pt(&ci1->p1, pti, UT_TOL_min0) == 1) {     // 360-deg-circ ?
      // printf(" pti=startpt\n");
    ao = 0.;
    goto Fertig;
  }

  // vc1=pc-p1;
  UT3D_vc_2pt (&vc1, &ci1->pc, &ci1->p1);
  // vc1=pc-pti;
  UT3D_vc_2pt (&vc2, &ci1->pc, pti);
    // DEB_dump_obj__(Typ_VC, &vc1, " vc1");
    // DEB_dump_obj__(Typ_VC, &vc2, " vc2");
    // printf(" tol=%f\n",UT_TOL_pt);


  // check 180 deg
  pt1 = ci1->pc;
  UT3D_pt_add_vc_rev (&pt1, &vc1);
  if(UT3D_comp2pt(&pt1, pti, UT_TOL_pt) == 1) { 
    if(ci1->rad > 0.) ao   = RAD_180;
    else              ao   = -RAD_180;
    goto Fertig;
  }


  //  Länge 1 notwendig, sonst geht zB acos(3.5) nicht!
  UT3D_vc_setLength (&vc1, &vc1, 1.);
  UT3D_vc_setLength (&vc2, &vc2, 1.);
    // printf("     vc1=%f,%f,%f\n",vc1.dx,vc1.dy,vc1.dz);
    // printf("     vc2=%f,%f,%f\n",vc2.dx,vc2.dy,vc2.dz);


  // dCos = scalarprod = signed length of &vc2 along vc1
  ao = ACOS(UT3D_acos_2vc (&vc1, &vc2));
    // printf("ao=%f\n",ao);


  // test if point pti is above or below the plane vc1.dx - vc1.dz; 1=yes,-1=no
  ii = UT3D_sid_pt_pt2vc (&ci1->pc, &ci1->vz, &vc1, pti);
    // printf(" ii=%d\n",ii);


  //  wenn der Bogen groesser als ein Halbkreis, muss Halbkreisumfang dazu !
  if(ci1->rad > 0.) {
    // CCW
    if(ii < 0) ao = RAD_360 - ao;
    
  } else {
    // CW
    if(ii > 0) ao = RAD_360 - ao;
    ao = -ao;

  }

 
  //----------------------------------------------------------------
  Fertig:

    // printf("ex UT3D_angr_ci_p1_pt %f\n",ao);

  return ao;

}


//================================================================
  double UT3D_angr_ci_par1 (Circ *ci1, double par1) {
//================================================================
/// \code
/// UT3D_angr_ci_par1              get opening-angle from parameter 0-1
/// Input:
///   par1   0. = startpoint of circ, 1. = endpoint of circ
/// RetCod:
///   angle            opening-angle to vector (ci.pc - ci.p1) 
///
/// \endcode

// see also UT3D_angr_ci_p1_pt UT3D_ci_setangr UT3D_angr_ci__
  double    aa;


  // DEB_dump_obj__ (Typ_CI, ci1, "UT3D_angr_ci_par1 %lf",par1);
  // printf("ex DEB_dump_obj__ %lf\n",par1 * ci1->ango);


  // angr = (par1 * openingAngle)
  return (par1 * ci1->ango);

}


//======================================================================
  int UT3D_pl_pta (Plane *pl, int ptNr, Point *pta) {
//======================================================================
/// \code
/// UT3D_pl_pta               plane from n-points
/// ACHTUNG: Z-Vektor entspricht nicht dem Umlaufsinn !
/// RetCod: -1  weniger als 3 Punkte
/// see also UT3D_vc_perppta
/// \endcode


  int      i1, ix, iy;
  double   d1, dMax;
  Vector   vc1;


  // printf("UT3D_pl_pta %d\n",ptNr);
  // for(i1=0; i1<ptNr; ++i1) DEB_dump_obj__ (Typ_PT,&pta[i1],"pta[%d]",i1);


  if(ptNr < 3) {
    // TX_Error("UT3D_pl_pta E001");
    return -1;
  }


  // den ersten Punkt mit Abst > UT_TOL_pt suchen ..
  ix = -1;
  for(i1=0; i1<ptNr; ++i1) {
    if(UT3D_len_2pt(&pta[0], &pta[i1]) < UT_TOL_pt) continue;
    ix = i1;
    break;
  }
  if(ix < 0) return -1;
    // printf(" ix=%d\n",ix);


  // den vom Startvektor am weitesten entfernten punkt suchen ..
  dMax = UT_VAL_MIN;
  UT3D_vc_2pt (&vc1, &pta[0], &pta[ix]);

  for(i1=1; i1<ptNr-1; ++i1) {
    d1 = UT3D_nlen_2ptvc (&pta[i1], &pta[0], &vc1);
    // printf("  #### p[%d] d1=%f\n",i1,d1);
    if(d1 > dMax) { dMax = d1; iy = i1; }
  }



  // printf("UT3D_pl_pta %d ix=%d iy=%d\n",ptNr,ix,iy);

  // pln from 3 pt's
  UT3D_pl_3pt (pl, &pta[0], &pta[ix], &pta[iy]);

/*
  // verkehrt: umdrehen
  // zerstoert aber aussen/innen-Info !
  if((UTP_comp2db(pl->vx.dx,  1., UT_TOL_PAR)) &&
     (UTP_comp2db(pl->vz.dz, -1., UT_TOL_PAR))) {
    printf(" _pl_pta revert\n");
    pl->vx = UT3D_VECTOR_X;
    pl->vy = UT3D_VECTOR_Y;
    pl->vz = UT3D_VECTOR_Z;
  }
*/

  // DEB_dump_obj__ (Typ_PLN, pl, "ex UT3D_pl_pta:\n");

  return 0;
}


//================================================================
  int UT3D_pl_bp (Plane *plo, int bpi, double bpd) {
//================================================================
/// \code
/// UT3D_pl_bp                plane from backplane and offset
///   backplanes: BCKPLN_XY | BCKPLN_XZ | BCKPLN_YZ
/// 
/// Input:
///   bpi = BCKVEC_Z = BCKPLN_XY: bpd = Z-Coord.Value
///   bpi = BCKVEC_Y = BCKPLN_XZ: bpd = Y-Coord.Value
///   bpi = BCKVEC_X = BCKPLN_YZ: bpd = X-Coord.Value
///   bpd   offset from backplane along z-axis
/// \endcode

  Point     pt1;


  pt1 = UT3D_PT_NUL;

  switch (bpi) {

    case BCKPLN_XY:
      pt1.z = bpd;
      plo->vx = UT3D_VECTOR_X;
      plo->vy = UT3D_VECTOR_Y;
      plo->vz = UT3D_VECTOR_Z;
      break;

    case BCKPLN_XZ:
      pt1.y = bpd;
      plo->vx = UT3D_VECTOR_X;
      plo->vy = UT3D_VECTOR_IZ;
      plo->vz = UT3D_VECTOR_Y;
      break;

    case BCKPLN_YZ:
      pt1.x = bpd;
      plo->vx = UT3D_VECTOR_Y;
      plo->vy = UT3D_VECTOR_Z;
      plo->vz = UT3D_VECTOR_X;
      break;
  }

  plo->po = pt1;
  plo->p  = bpd;

  // UT3D_pl_ptpl (plo, &pt1);


  return 0;

}


//======================================================================
  int UT3D_pl_3pt (Plane *pl, Point *p1, Point *p2, Point *p3) {
//======================================================================
/// \code
/// UT3D_pl_3pt               plane from 3 points (vx parallel XY-mainPlane)
/// Output:
///   pl         origin=p1, vx = parallel to XY-mainPlane
///
/// see also  UT3D_pl_pto_vcx_vcy
/// \endcode


  Vector v1, v2;

  UT3D_vc_2pt (&v1, p1, p2);
  UT3D_vc_2pt (&v2, p1, p3);

  /* the z-vector = normal to p1-p2, p1-p3 */
  UT3D_vc_perp2vc (&v1, &v1, &v2);

  /* p1 = origin, v1 = Z-vector */
  UT3D_pl_ptvc (pl, p1, &v1);

  // DEB_dump_obj__ (Typ_PLN, pl, "ex UT3D_pl_3pt:\n");

  return 1;

}


//========================================================================
  int UT3D_pl_perp_ln (Plane *pln, Line *lna) {
//========================================================================
/// \code
/// UT3D_pl_perp_ln                make Plane normal to line (line = Plane-Z-vec)
/// Origin is line-center; Z-vec is line.
/// \endcode


  // DEB_dump_obj__ (Typ_LN, lna, "UT3D_pl_perp_ln:");


  // UT3D_pt_mid2pt (&pln->po, &lna->p1, &lna->p2);
  pln->po = lna->p1;

  UT3D_vc_2pt (&pln->vz, &lna->p1, &lna->p2);       // Z-vec
  UT3D_vc_setLength (&pln->vz, &pln->vz, 1.);

  UT3D_vc_perp1vc (&pln->vx, &pln->vz);             // X-vec
  UT3D_vc_setLength (&pln->vx, &pln->vx, 1.);

  UT3D_vc_perp2vc (&pln->vy, &pln->vz, &pln->vx);   // Y-vec
  // UT3D_vc_setLength  // normalize

  UT3D_pl_p (pln);   // set p, bpi

  // DEB_dump_obj__ (Typ_PLN, pln, "ex UT3D_pl_perp_ln:");
  // GR_Disp_pln (pln, 2);

  return 0;

}


//================================================================
  int UT3D_pl_ln (Plane *oPln, Line *oLn) {
//================================================================
// UT3D_pl_ln                get plane for line in plane

  int    bpi;
  double bpd;
  Vector vx, vy, vz;


   // DEB_dump_obj__ (Typ_LN, oLn, "UT3D_pl_perp_ln");

  // get backplane for line
  bpi = UT3D_bp_ln (&bpd, oLn);

  if(bpi >= 0) {
    UT3D_pl_bp (oPln, bpi, bpd);

  } else {
    // free plane; get vx from line normalized
    UT3D_vc_ln (&vx, oLn);
    UT3D_vc_setLength (&vx, &vx, 1.);
    // get vy normal to vx in plane XY
    UT3D_vc_perpvcplXY (&vy, &vx);
    // get plane from vx, vy, p1
    UT3D_pl_pto_vcx_vcy (oPln, &oLn->p1, &vx, &vy);
  }

    // DEB_dump_obj__ (Typ_PLN, oPln, "Typ_LN bpi = %d",bpi);
    // GR_Disp_pln (oPln, Typ_Att_def);

  return 0;

}


//====================================================================
  void UT3D_pl_2ln (Plane *pl1, Line *ln1, Line *ln2) {
//====================================================================
// UT3D_pl_2ln               plane from 2 lines in plane

  Vector   vc1;

  // printf("UT3D_pl_2ln\n");
  // DEB_dump_obj__ (Typ_LN, ln1, "");
  // DEB_dump_obj__ (Typ_LN, ln2, "");


  // den Z-Vektor errechnen
  UT3D_vc_perp4pt (&vc1, &ln1->p1, &ln1->p2, &ln2->p1, &ln2->p2);

  // plane from startpt of ln1 & normalvektor
  UT3D_pl_ptvc (pl1, &ln1->p1, &vc1);

  // DEB_dump_obj__ (Typ_PLN, pl1, "ex UT3D_pl_2ln\n");

}

//================================================================
  int UT3D_pl_ell (Plane *pl1, CurvElli *el1) {
//================================================================
/// UT3D_pl_ell         make Plane from ellipse


  pl1->po = el1->pc;
  pl1->vz = el1->vz;
  UT3D_vc_setLength (&pl1->vx, &el1->va, 1.);
  UT3D_vc_setLength (&pl1->vy, &el1->vb, 1.);

  UT3D_pl_p (pl1);

  return 0;

}


//================================================================
  int UT3D_pl_obj (Plane *pl1, int typ, void *obj, int oNr) {
//================================================================
// UT3D_pl_obj              get basic-plane for obj
// TODO: problem plane from point+vector; use ATO or OTB or OGX ?

  int      irc, ptNr, paSiz;
  Point    *pta;
  // Memspc   tbuf1;
  MemTab(Point) mtpa = _MEMTAB_NUL;


  //----------------------------------------------------------------
  if(typ == Typ_PT) {
    return  UT3D_pl_pta (pl1, oNr, (Point*)obj);

  //----------------------------------------------------------------
  } else if(typ == Typ_LN) {
    return UT3D_pl_ln (pl1, obj);          // TODO: oNr (create pTab of n Lines)

  //----------------------------------------------------------------
  } else if(typ == Typ_CI) {
    return UT3D_pl_ci (pl1, obj);

  //----------------------------------------------------------------
  } else if(typ == Typ_CVPOL) {
    return UT3D_pl_pta (pl1, ((CurvPoly*)obj)->ptNr,
                             ((CurvPoly*)obj)->cpTab);

  //----------------------------------------------------------------
  } else if(typ == Typ_CVELL) {
    return UT3D_pl_ell (pl1, obj);

  //----------------------------------------------------------------
  } else if(typ == Typ_CVBSP) {
    return UT3D_pl_pta (pl1, ((CurvBSpl*)obj)->ptNr,
                             ((CurvBSpl*)obj)->cpTab);

  //----------------------------------------------------------------
  } else if(typ == Typ_CVTRM) {
    // get tempSpace
    // UME_init (&tbuf1, MEM_alloc_tmp (SPC_MAX_STK), SPC_MAX_STK);
    // // get whole space for points
    // paSiz = UME_nStru_get (&pta, 0, sizeof(Point), &tbuf1);
    // // get polygon
    // ptNr = 0;
    // irc = UT3D_npt_trmCv (&ptNr, pta, paSiz, (CurvCCV*)obj, oNr, UT_DISP_cv, 0);
    MemTab_ini_fixed (&mtpa, MEM_alloc_tmp (SPC_MAX_STK), SPC_MAX_STK,
                    sizeof(Point), Typ_PT); 
    irc = UT3D_mtpt_trmCv (&mtpa, NULL, (CurvCCV*)obj, oNr, UT_DISP_cv, 0);
    if(irc < 0) { TX_Error("UT3D_pl_obj E1"); return -1;}
    // get plane from polygon
    // irc = UT3D_pl_pta (pl1, ptNr, pta);
    irc = UT3D_pl_pta (pl1, mtpa.rNr, (Point*)mtpa.data);
    if(irc < 0) { TX_Error("UT3D_pl_obj E2"); return -1;}
    MemTab_free (&mtpa);

  //----------------------------------------------------------------
  } else if(typ == Typ_PLN) {
    *pl1 = *((Plane*)obj);

  //----------------------------------------------------------------
  } else {
    TX_Error ("UT3D_pl_obj objtyp %d not supported",typ);
    return -1;
  }

  return 0;

}


//================================================================
  int UT3D_pl_ci (Plane *pl1, Circ *ci1) {
//================================================================
/// UT3D_pl_ci         make Plane from circ


  pl1->po = ci1->pc;

  UT3D_vc_2pt (&pl1->vx, &ci1->pc, &ci1->p1);
  UT3D_vc_setLength (&pl1->vx, &pl1->vx, 1.);

  pl1->vz = ci1->vz;

  UT3D_vc_perp2vc (&pl1->vy, &pl1->vz, &pl1->vx);

  UT3D_pl_p (pl1);

  // pl1->p  = pl1->vz.dx * pl1->po.x +
            // pl1->vz.dy * pl1->po.y +
            // pl1->vz.dz * pl1->po.z;


  return 0;

}


//================================================================
  int UT3D_pl_nobj (Plane *pl1, int oNr, ObjGX *oTab) {
//================================================================
/// \code
/// UT3D_pl_nobj              plane fron ObjGX-table
/// die Transformationsmatrix zu n Objekten suchen ..
/// oTab enthaelt PT|LN|AC
/// 1) Circ in den Inputdaten: vz auslesen, ptCen laden, Plane davon.
/// 2) mind 2 Lines: vz=CrossProd. dazu; Origin = 1.Point, Plane davon.
/// 3) von den InputObjekten eine Punktetabelle beladen; Plane davon.
/// \endcode

     // UT3D_pl_3pt UT3D_pl_pta

  int    i1, i2;
  Circ   *ci1;
  Point  pTab[4];


  //----------------------------------------------------------------
  // Circ in den Inputdaten: vz auslesen, ptCen laden, Plane davon.
  if(oTab[0].form == Typ_CI) {ci1 = oTab[0].data; goto L_C__;}
  if(oTab[1].form == Typ_CI) {ci1 = oTab[1].data; goto L_C__;}
  if(oTab[2].form == Typ_CI) {ci1 = oTab[2].data; goto L_C__;}
  goto L_L_0;

  L_C__:
  UT3D_pl_ptvc (pl1, &ci1->pc, &ci1->vz);
  goto L_done;



  //----------------------------------------------------------------
  // mind 2 Lines: vz=CrossProd. dazu; Origin = 1.Point, Plane davon.
  L_L_0:
  i1 = -1;
  if(oTab[0].form == Typ_LN) i1 = 0;
  if(oTab[1].form == Typ_LN) {
    if(i1 < 0) i1 = 1;
    else {i2 = 1; goto L_L__;}
  }
  if(oTab[2].form == Typ_LN) {
    if(i1 < 0) {i1 = 2; goto L_P_0;}
    else {i2 = 2; goto L_L__;}
  }
  goto L_P_0;

  L_L__:
    // printf("L_L__: %d %d\n",i1,i2);
  UT3D_pl_2ln (pl1, (Line*)oTab[i1].data, (Line*)oTab[i2].data);
  goto L_done;


  //----------------------------------------------------------------
  // von den InputObjekten eine Punktetabelle beladen; Plane davon.
  L_P_0:
    // printf("_pl_nobj %d %d\n",i1,i2);
  // 2 pt's & 1 line or 3 points
  if(i1 >= 0) {
    pTab[0] = ((Line*)oTab[i1].data)->p1;
    pTab[1] = ((Line*)oTab[i1].data)->p2;
    i1 = 2;
  } else {
    i1 = 0;
  }

  if(oTab[0].form == Typ_PT) {pTab[i1] = *((Point*)oTab[0].data); ++i1;}
  if(oTab[1].form == Typ_PT) {pTab[i1] = *((Point*)oTab[1].data); ++i1;}
  if(oTab[2].form == Typ_PT) {pTab[i1] = *((Point*)oTab[2].data); ++i1;}

  UT3D_pl_pta (pl1, i1, pTab);


  L_done:
    // GR_Disp_pln (pl1, 9);

  return 0;

}


//=====================================================================
  void UT3D_pl_XYZ (Plane *pl1) {
//=====================================================================
/// UT3D_pl_XYZ               Defaultplane


  pl1->po = UT3D_PT_NUL;

  pl1->vx = UT3D_VECTOR_X;
  pl1->vy = UT3D_VECTOR_Y;
  pl1->vz = UT3D_VECTOR_Z;

  pl1->p  = 0.;

}


//======================================================================
  int UT3D_pl_ptvzpl (Plane *pl, Point *pt, Vector *vcz, Plane *basPln) {
//======================================================================
/// UT3D_pl_ptvzpl            plane from z-axis and origin and base-plane

  Vector  vx1;

  // DEB_dump_obj__ (Typ_PT, pt, "UT3D_pl_ptvzpl pt");
  // DEB_dump_obj__ (Typ_VC, vcz, "    _pl_ptvzpl vcx");
  // DEB_dump_obj__ (Typ_PT, basPln, "    _pl_ptvzpl basPln");


  // project vector onto base-plane;
  UT3D_vc_projvcpl (&vx1, basPln, vcz);

  pl->po = *pt;

  // if vcx is normal to basPln:
  if(UT3D_compvc0 (&vx1, UT_TOL_min1)) {
    // vz = vz: copy plane
    *pl = *basPln;
    pl->po = *pt;
    UT3D_pl_p (pl);

  } else {


    // make vcz=length 1
    UT3D_vc_setLength (&pl->vz, vcz, 1.);

    // make normal-vector = Y-vector (cross-prod vx1 vz-basPln)
    UT3D_vc_perp2vc (&pl->vy, &(basPln->vz), &vx1);

    // make vcy=length 1
    UT3D_vc_setLength (&pl->vy, &pl->vy, 1.);

    // make normal-vector = X-vector (cross-prod vcy vcz)
    UT3D_vc_perp2vc (&pl->vx, &pl->vy, &pl->vz);

    UT3D_pl_p (pl);

  }

    // DEB_dump_obj__ (Typ_PLN, pl, "ex UT3D_pl_ptvzpl");

  return 0;

}


//======================================================================
  int UT3D_pl_ptvxpl (Plane *pl, Point *pt, Vector *vcx, Plane *basPln) {
//======================================================================
/// UT3D_pl_ptvxpl            plane from x-axis and origin and base-plane

  Vector  vx1, vcy;


  // project vector onto base-plane;
  UT3D_vc_projvcpl (&vx1, basPln, vcx);


  // if vcx is normal to basPln:
  if(UT3D_compvc0 (&vx1, UT_TOL_min1)) {
    // new vx = bas-vz; new vy = bas-vy; new vz = bas-ix
    pl->vx = basPln->vz;
    pl->vy = basPln->vy;
    UT3D_vc_invert (&(pl->vz), &(basPln->vz));
    pl->po = *pt;
    UT3D_pl_p (pl);


  } else {

    // make normal-vector = Y-vector (cross-prod vx1 vz-basPln)
    UT3D_vc_perp2vc (&vcy, &(basPln->vz), &vx1);

    // create plane from pt, x-vec, y-vec
    UT3D_pl_pto_vcx_vcy (pl, pt, vcx, &vcy);

  }

    // DEB_dump_obj__ (Typ_PLN, pl, "ex UT3D_pl_ptvxpl");


  return 0;

}


//==========================================================================
  int UT3D_pl_pto_vcz_vcx (Plane *pl1, Point *po, Vector *vz, Vector *vx) {
//==========================================================================
/// \code
/// UT3D_pl_pto_vcz_vcx      plane from Origin, Z-vec, X-Vec. Z-vec is fixed.
/// vz must have length=1.
/// Create vy from vx and vz; create new vx from vy, vz.
/// \endcode


  pl1->po = *po;


  // DEB_dump_obj__ (Typ_VC, vz, "UT3D_pl_pto_vcz_vcx vz");
  // DEB_dump_obj__ (Typ_VC, vx, "                    vx");


  UT3D_vc_setLength (&pl1->vz, vz, 1.);
  UT3D_vc_setLength (&pl1->vx, vx, 1.);
  

  // Create vy from vx and vz
  // 
  UT3D_vc_perp2vc (&pl1->vy, &pl1->vz, &pl1->vx);
  UT3D_vc_setLength (&pl1->vy, &pl1->vy, 1.);
    // DEB_dump_obj__ (Typ_VC, &pl1->vy, "               vy=");

  // create new vx from vy, vz.
  UT3D_vc_perp2vc (&pl1->vx, &pl1->vy, &pl1->vz);
    // DEB_dump_obj__ (Typ_VC, &pl1->vx, "               vx=");


  UT3D_pl_p (pl1);

    // DEB_dump_obj__ (Typ_PLN, pl1, "ex UT3D_pl_pto_vcz_vcx\n");

  return 0;

}


//====================================================================
  void UT3D_pl_pto_vcx_vcz (Plane *pl1, Point *po, Vector *vx, Vector *vz) {
//====================================================================
/// UT3D_pl_pto_vcx_vcz     plane from Origin, X-vec, Z-Vec.    X-vec is fixed.

  // DEB_dump_obj__ (Typ_PT, po, "UT3D_pl_pto_vcx_vcz po=");
  // DEB_dump_obj__ (Typ_VC, vx, "               vx=");
  // DEB_dump_obj__ (Typ_VC, vz, "               vz=");


  pl1->po = *po;

  UT3D_vc_setLength (&pl1->vx, vx, 1.);
  UT3D_vc_setLength (&pl1->vz, vz, 1.);

  // Create vy from vx and vz
  UT3D_vc_perp2vc (&pl1->vy, &pl1->vz, &pl1->vx);
    // DEB_dump_obj__ (Typ_VC, &pl1->vy, "               vy=");

  // create new vz from vx, vy.
  UT3D_vc_perp2vc (&pl1->vz, &pl1->vx, &pl1->vy);
    // DEB_dump_obj__ (Typ_VC, &pl1->vz, "               vz=");


  UT3D_pl_p (pl1);

  // pl1->p = pl1->vz.dx * pl1->po.x +
           // pl1->vz.dy * pl1->po.y +
           // pl1->vz.dz * pl1->po.z;


  // DEB_dump_obj__ (Typ_PLN, pl1, "ex UT3D_pl_pto_vcx_vcz\n");

}
 

//=====================================================================
  void UT3D_pl_pto_vcx_vcy (Plane *pl1, Point *po, Vector *vx, Vector *vy) {
//=====================================================================
/// \code
/// UT3D_pl_pto_vcx_vcy         plane from origin, X-vec, Y-Vec. X-vec is fixed.
/// 
/// Both vecs are in the Plane !
/// 
/// \endcode
// Der X-Vektor bleibt fix, der Y-Vektor wird zurechtgerückt,
//   der Z-Vektor errechnet.


  
  pl1->po = *po;

  // pl1->vx = *vx;
  UT3D_vc_setLength (&pl1->vx, vx, 1.);
  pl1->vy = *vy;
  // UT3D_vc_setLength (&pl1->vy, vy, 1.);


  // den Z-Vektor errechnen (normal auf vx, vy)
  UT3D_vc_perp2vc (&pl1->vz, &pl1->vx, &pl1->vy);
  UT3D_vc_setLength (&pl1->vz, &pl1->vz, 1.);


  // den Y-Vektor errechnen (normal auf vz, vx)
  UT3D_vc_perp2vc (&pl1->vy, &pl1->vz, &pl1->vx);
  UT3D_vc_setLength (&pl1->vy, &pl1->vy, 1.);


  UT3D_pl_p (pl1);

  // pl1->p = pl1->vz.dx * pl1->po.x +
           // pl1->vz.dy * pl1->po.y +
           // pl1->vz.dz * pl1->po.z;


/*
  printf("ex UT3D_pl_pto_vcx_vcy po=%f,%f,%f\n",pl1->po.x,pl1->po.y,pl1->po.z);
  printf("vx=%f,%f,%f\n",pl1->vx.dx,pl1->vx.dy,pl1->vx.dz);
  printf("vy=%f,%f,%f\n",pl1->vy.dx,pl1->vy.dy,pl1->vy.dz);
  printf("vz=%f,%f,%f\n",pl1->vz.dx,pl1->vz.dy,pl1->vz.dz);
*/

}


//======================================================================
  int UT3D_pl_ptvc (Plane *pl, Point *pt, Vector *vc) {
//======================================================================
/// UT3D_pl_ptvc              plane from point + z-vector



  // DEB_dump_obj__ (Typ_VC, vc, "UT3D_pl_ptvc:");


  // origin of plane
  pl->po = *pt;

  // the z-vector
  UT3D_vc_setLength (&pl->vz, vc, 1.);
    // DEB_dump_obj__ (Typ_VC, &pl->vz, "   vz:");


  // create the X-vector = Normalvektor auf vc, der in der X-Y-Plane liegt.
  UT3D_vc_perpvcplXY (&pl->vx, &pl->vz);
    // DEB_dump_obj__ (Typ_VC, &pl->vx, "   vx:");

  // den Y-Vektor errechnen
  UT3D_vc_perp2vc (&pl->vy, &pl->vz, &pl->vx);

  // das D aus der Normalform via dotproduct
  // pl->p = pl->vz.dx * pl->po.x +
          // pl->vz.dy * pl->po.y +
          // pl->vz.dz * pl->po.z;
  UT3D_pl_p (pl);


/*
  printf("ex UT3D_pl_ptvc %f %f %f\n",pl->po.x,pl->po.y,pl->po.z);
  printf("        vx = %f %f %f\n",pl->vx.dx,pl->vx.dy,pl->vx.dz);
  printf("        vy = %f %f %f\n",pl->vy.dx,pl->vy.dy,pl->vy.dz);
  printf("        vz = %f %f %f\n",pl->vz.dx,pl->vz.dy,pl->vz.dz);
  printf("          p= %f\n",pl->p);
  printf(" lx=%f\n",UT3D_len_vc(&pl->vx));
  printf(" ly=%f\n",UT3D_len_vc(&pl->vy));
  printf(" lz=%f\n",UT3D_len_vc(&pl->vz));
*/


  return 1;

}




//============================================================================
  void UT3D_pl_pto_vcz_ptx (Plane *pl1, Point *po, Vector *vz, Point *ptx) {
//============================================================================
/// UT3D_pl_pto_vcz_ptx    Plane aus Nullpunkt, Z-Achse und Punkt auf X-Achse

  double len;
  Point  pts;
  Vector vx;



  pl1->po = *po;

  UT3D_vc_normalize (&pl1->vz, vz);

  // pts = den ptx auf den po/vz projizieren
  UT3D_pt_projptptvc (&pts, &len, NULL, ptx, po, vz);

  // pts -> ptx ist nun der neue X-Vektor
  UT3D_vc_2pt (&vx, &pts, ptx);
  UT3D_vc_setLength (&pl1->vx, &vx, 1.);

  // vy ist nun das Kreuzprod von vx u vz
  UT3D_vc_perp2vc (&pl1->vy, &pl1->vz, &pl1->vx);

  UT3D_pl_p (pl1);

  // pl1->p = pl1->vz.dx * pl1->po.x +
           // pl1->vz.dy * pl1->po.y +
           // pl1->vz.dz * pl1->po.z;

  return;

}


//===================================================================
  void UT3D_pl_invert (Plane *pl) {
//===================================================================
/// \code
/// UT3D_pl_invert            Plane invert (change direction of Z-vec)
/// Invert Z-Achse, swap X-Y-Achse.
/// \endcode
// 
// Eine 2. Variante waere: 180 Grad um die X-Achse drehen
//   (invert Z- und Y-Achse).
// 
// NICHT geht: alle 3 Vec. invertieren !! (korr 2005-06-30 RF).


  // swap the x-vec and the y-vec
  MEM_swap__ (&pl->vx, &pl->vy, sizeof(Vector));
  UT3D_vc_invert (&pl->vz, &pl->vz);

  // UT3D_vc_invert (&pl->vx, &pl->vx);
  // UT3D_vc_invert (&pl->vy, &pl->vy);

  UT3D_pl_p (pl);

  // pl->p = pl->vz.dx * pl->po.x +
          // pl->vz.dy * pl->po.y +
          // pl->vz.dz * pl->po.z;

  return;
}


//================================================================
  int UT3D_pl_rotZangr (Plane *pln1, double *ar) {
//================================================================
/// UT3D_pl_rotZangr          rot. Refsys around Z


  // rotate vx around vz
  UT3D_vc_rotvcvcangr (&pln1->vx, &pln1->vz, &pln1->vx, *ar);

  // recompute vy
  UT3D_vc_perp2vc (&pln1->vy, &pln1->vz, &pln1->vx);
      
  return 0;

}


//================================================================
  int UT3D_pl_rotYangr (Plane *pln1, double *ar) {
//================================================================
/// UT3D_pl_rotYangr          rot. Refsys around Z (tilt-angle)


  // printf("UT3D_pl_rotYangr %lf\n",*ar);
  // DEB_dump_obj__ (Typ_PLN, pln1, " pl-in:");


  // rotate vx around vy
  UT3D_vc_rotvcvcangr (&pln1->vx, &pln1->vy, &pln1->vx, *ar);

  // recompute vz
  UT3D_vc_perp2vc (&pln1->vz, &pln1->vx, &pln1->vy);

  UT3D_pl_p (pln1);  // update p

    // DEB_dump_obj__ (Typ_PLN, pln1, " pl-out:");
      
  return 0;

}


//================================================================
  int UT3D_pl_rotpt (Plane *plo, Plane *pli, Point *ptx) {
//================================================================
/// \code
/// UT3D_pl_rotpt             rot. Refsys around Z; point gives new X-direction
/// plo und pli duerfen gleich sein.
/// \endcode

  double  d1;
  Point   pta;
  Vector  vx1;
  Plane   pl1;

  pl1 = *pli;

  // project ptx --> axis; this makes new x-vec
  UT3D_pt_projptptvc (&pta, &d1, NULL, ptx, &pli->po, &pli->vz);
  UT3D_vc_2pt (&vx1, &pta, ptx);
  UT3D_vc_setLenLen (&pl1.vx, &vx1, 1., d1);

  // make new y-vec from new x-vec and old z-vec
  UT3D_vc_perp2vc (&pl1.vy, &pl1.vz, &pl1.vx);

  // GR_tDyn_vc__ (&pli->vx, &pli->po, 8, 0);
  // GR_tDyn_vc__ (&plo->vx, &pli->po, 9, 0);

  *plo = pl1;

  return 0;

}


//======================================================================
  void UT3D_pl_ptpl (Plane *pl, Point *pt1) {
//======================================================================
/// UT3D_pl_ptpl              change origin of plane
// 
// p muss neu errechnet werden !!!


	// proj pt1 -> Plane = Origin
  pl->po = *pt1;
  // UT3D_pt_intptvcpl_ (&pl->po, pl, pt1, &pl->vz);

  UT3D_pl_p (pl);

	// pl->p = pl->vz.dx * pl->po.x +
          // pl->vz.dy * pl->po.y +
          // pl->vz.dz * pl->po.z;

  // printf(" ex UT3D_pl_ptpl p=%f\n",pl->p);

  return;
}


//======================================================================
  void UT3D_pl_p (Plane *pl) {
//======================================================================
/// UT3D_pl_p                 plane - update p after a change of po or vz
 

  pl->p = pl->vz.dx * pl->po.x +
          pl->vz.dy * pl->po.y +
          pl->vz.dz * pl->po.z;

  // printf(" ex UT3D_pl_ptpl p=%f\n",pl->p);

  return;
}


//================================================================
  int UT3D_pl_mirln (Plane *plno, Plane *plni, Line *ln1) {
//================================================================
 
  Point    pto;
  Vector   vch, vx, vz;


  // printf("UT3D_pl_mirln \n");
  // DEB_dump_obj__ (Typ_LN, ln1, "ln1");
  // DEB_dump_obj__ (Typ_PLN, plni, "plni");


  // mirror originPoint
  UT3D_pt_projptln (&pto, NULL, NULL, &plni->po, ln1);
  UT3D_pt_opp2pt (&pto, &pto, &plni->po);
    // DEB_dump_obj__ (Typ_PT, &pto, "  pto:");

  UT3D_vc_ln (&vch, ln1);

  // mirror x-vector   // UT3D_vc_mirvcln
  UT3D_vc_mirvcln (&vx, &plni->vx, ln1);
    // DEB_dump_obj__ (Typ_VC, &vx, "  vx:");

  // mirror z-vector
  UT3D_vc_mirvcln (&vz, &plni->vz, ln1);
    // DEB_dump_obj__ (Typ_VC, &vz, "  vz:");

  // plane from x,z,origin
  UT3D_pl_pto_vcx_vcz (plno, &pto, &vx, &vz);

    // DEB_dump_obj__ (Typ_PLN, plno, "plno");

  return 0;

}


//================================================================
  int UT3D_pl_mirpl (Plane *plno, Plane *plni, Plane *plnm) {
//================================================================

  Point    pto;
  Vector   vch, vx, vz;


  // printf("UT3D_pl_mirpl:\n");



  // mirror originPoint
  UT3D_pt_projptpl (&pto, plnm, &plni->po);
  UT3D_pt_opp2pt (&pto, &pto, &plni->po);
 
  // mirror x-vector, z-vec
  UT3D_vc_mirvcpl (&vx, &plni->vx, plnm);
  UT3D_vc_mirvcpl (&vz, &plni->vz, plnm);

  // plane from x,z,origin
  UT3D_pl_pto_vcx_vcz (plno, &pto, &vx, &vz);

  return 0;

}


//================================================================
  int UT3D_pl_tra_m3 (Plane *pl2, Plane *pl1, Mat_4x3 ma) {
//================================================================
/// \code
/// apply transformation to refSys
/// Keep origin, rotate axes.
/// get matrix eg with UT3D_m3_inirot_2vc
/// \endcode

  UT3D_vc_tra_vc_m3 (&pl2->vx, ma, &pl1->vx);
  UT3D_vc_tra_vc_m3 (&pl2->vy, ma, &pl1->vy);
  UT3D_vc_tra_vc_m3 (&pl2->vz, ma, &pl1->vz);

  UT3D_pt_tra_pt_m3 (&pl2->po, ma, &pl1->po);    // 2020-10-13

  // copy origin
//   pl2->po.x = ma[0][3];
//   pl2->po.y = ma[1][3];
//   pl2->po.z = ma[2][3];

  return 0;

}




/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/
/*=====================================================================*/



//=======================================================================
  int UT3D_m3_get (void *out, int mode, Mat_4x3 ma) {
//=======================================================================
/// UT3D_m3_get            get part of matrix;  0=VX, 1=VY, 2=VZ, 3=PO
// #define UT3D_vc_m3

  memcpy(&((double*)out)[0], &ma[0][mode], sizeof(double));
  memcpy(&((double*)out)[1], &ma[1][mode], sizeof(double));
  memcpy(&((double*)out)[2], &ma[2][mode], sizeof(double));

  // if(mode<3) DEB_dump_obj__ (Typ_VC, out, "ex UT3D_m3_get V%d: n",mode);
  // else       DEB_dump_obj__ (Typ_PT, out, "ex UT3D_m3_get PO: \n");

  return 0;

}


//=======================================================================
  int UT3D_m3_set (Mat_4x3 ma, int mode, void *da) {
//=======================================================================
/// UT3D_m3_set            set part of matrix;  0=VX, 1=VY, 2=VZ, 3=PO
// #define UT3D_m3_vc


  // if(mode<3) DEB_dump_obj__ (Typ_VC, da, "UT3D_m3_set V%d: n",mode);
  // else       DEB_dump_obj__ (Typ_PT, da, "UT3D_m3_set PO: \n");

  memcpy(&ma[0][mode], &((double*)da)[0], sizeof(double));
  memcpy(&ma[1][mode], &((double*)da)[1], sizeof(double));
  memcpy(&ma[2][mode], &((double*)da)[2], sizeof(double));

  // DEB_dump_obj__ (Typ_M4x3, ma, "UT3D_m3_set %d\n",mode);

  return 0;

}




//================================================================
  int UT3D_m3_copy (Mat_4x3 mo, Mat_4x3 mi) {
//================================================================
/// UT3D_m3_copy              memcpy

///                // VX   VY   VZ   PO
///  Mat_4x3  mi= { {1.0, 0.0, 0.0, 0.0},
///                 {0.0, 1.0, 0.0, 0.0},
///                 {0.0, 0.0, 1.0, 0.0} };
///
///  UT3D_m3_copy (mo, mi);   // mi=UT3D_MAT_4x3


  memcpy (mo, mi, sizeof(Mat_4x3));

  return 0;

}


//=====================================================================
  int UT3D_m3_iniZrot_angr (Mat_4x3 ma, Point *ptOri, double ar) {
//=====================================================================
/// UT3D_m3_iniZrot_angr           define rotation around z-axis; (center, angle)
/// Drehung um die Z-Achse definieren
/// ptOri ist ein Punkt, um den gedreht werden soll.
/// Drehung um absolute Z-Achse oder obj schon in NullLage: ptOri=NULL.
/// \endcode

//   c  s  0 -
//  -s  c  0 -
//   0  0  1 -

// see UTRA_m4_init_rot
// UTRA_m3_inirot

  double  cos_a, sin_a;

  // DEB_dump_obj__ (Typ_PT, ptOri, "UTRA_m3_iniZrot ");

  sin_a = sin(ar);
  cos_a = cos(ar);
  // printf(" s=%f c=%f\n",sin_a,cos_a);

  // load Einheitsmatrix
  memcpy (ma, UT3D_MAT_4x4, sizeof(Mat_4x3));

  // add rotation
    ma[0][0] =  cos_a;     ma[0][1] =  sin_a;   // ma[0][2] =  0.;
    ma[1][0] = -sin_a;     ma[1][1] =  cos_a;   // ma[1][2] =  0.;
 // ma[2][0] =  0.;        ma[2][1] =  0.;         ma[2][2] =  1.;


  // add origin
  if(ptOri != NULL) {
    ma[0][3] = ptOri->x;
    ma[1][3] = ptOri->y;
    ma[2][3] = ptOri->z;
  }

  return 0;

}


//==========================================================================
  int UT3D_m3_inirot_angr (Mat_4x3 ma, Point *pa, Vector *va, double angr) {
//==========================================================================
/// define rotation (axis, origin angle)

// Rotationsmatrix definieren; ex struct TraRot:
// TraRot   trr;
// trr.angr = UT_RADIANS(<RotationAngle>);
// trr.vz   = <Vector_RotationAxis>;
// UT3D_m3_inirot_angr (trr.ma, &<RotationOrigin>, &trr.vz, trr.angr);

// Der RotationOrigin steckt in der ma in ma[0][3], ma[1][3], ma[2][3].
// Extrahieren Via Funktion UT3D_m3_get (&ptOri, 3, trr.ma);

  Vector   Vu;
  double   sinA, cosA, versA;


  sinA = sin(angr);
  cosA = cos(angr);


  versA = 1 - cosA;


  UT3D_vc_setLength (&Vu, va, 1.);


  ma[0][0] = Vu.dx*Vu.dx*versA + cosA;
  ma[1][0] = Vu.dx*Vu.dy*versA - Vu.dz*sinA;
  ma[2][0] = Vu.dx*Vu.dz*versA + Vu.dy*sinA;

  ma[0][1] = Vu.dy*Vu.dx*versA + Vu.dz*sinA;
  ma[1][1] = Vu.dy*Vu.dy*versA + cosA;
  ma[2][1] = Vu.dy*Vu.dz*versA - Vu.dx*sinA;

  ma[0][2] = Vu.dx*Vu.dz*versA - Vu.dy*sinA;
  ma[1][2] = Vu.dy*Vu.dz*versA + Vu.dx*sinA;
  ma[2][2] = Vu.dz*Vu.dz*versA + cosA;

  // RotAxisCenter; wird nach Rotation aufaddiert.
  if(pa) {      // nur vec-rot: pa = NULL
    ma[0][3] = pa->x;
    ma[1][3] = pa->y;
    ma[2][3] = pa->z;

  } else {
    ma[0][3] = 0.;
    ma[1][3] = 0.;
    ma[2][3] = 0.;
  }

  return 0;

}


//===========================================================================
  int UT3D_m3_inirot_2vc (Mat_4x3 ma1, Point *pto, Vector *vc1, Vector *vc2) {
//===========================================================================
/// \code
/// UT3D_m3_inirot_2vc        define rotation-transformation between 2 vectors
/// RetCod:  0     OK, transformation in ma1 defined;
///          1     vc1-vc2 are parallel (same direction); ma1 not defined.
///         -1     vc1-vc2 are antiparallel; ma1 not defined.
/// \endcode

  double ao;
  Vector rx;

  // DEB_dump_obj__ (Typ_VC, vc1, "UT3D_m3_inirot_2vc vc1");
  // DEB_dump_obj__ (Typ_VC, vc2, " vc2");

  // rotation-axis = crossprod vc1-vc2
  UT3D_vc_perp2vc (&rx, vc1, vc2);

  // check for parallel
  if(UT3D_compvcNull (&rx)) {
      // printf(" vecs parall ...\n");
    return UT3D_sid_2vc (vc1, vc2);
  }

  // ao = opening-angle vc1 to vc2
  ao = UT3D_angr_2vc__ (vc1, vc2) * -1.;
    // printf(" ao=%lf\n",ao);

  UT3D_vc_setLength (&rx, &rx, 1.);
  UT3D_m3_inirot_angr (ma1, pto, &rx, ao);

  return 0;

}


//=====================================================================
  void UT3D_m3_load (Mat_4x3 ma,  Vector *vx, Vector *vy, Vector *vz) {
//=====================================================================
/// \code
///  UT3D_m3_load              define refSys from 3 vectors (4x3 Matrix).
/// Die Inp.Vec. muessen nicht normiert sein.
///   use (translate points) with UT3D_pt_tra_pt_m3
/// \endcode
// 
// Mat_4x3
//   Der erste Wert ist der Index ins double-Array.
// 
//       VX              VY              VZ           Nullpunkt
// 
//   0=[0][0]=vx.x   1=[0][1]=vy.x   2=[0][2]=vz.x    3=[0][3]=o.x
//   4=[1][0]=vx.y   5=[1][1]=vy.y   6=[1][2]=vz.y    7=[1][3]=o.y
//   8=[2][0]=vx.z   9=[2][1]=vy.z  10=[2][2]=vz.z   11=[2][3]=o.z
// 
//   Die Daten liegen im eindimensinalen Array vertikal;
//     der X-Vektor liegt auf 0,4,8; der Y-Vektor auf 1,5,9;
//     der Z-vektor auf 3,7,11.

/*
  Vector   vx, vy, vz;
  Point    p1, p2, p3;
  Mat_3x3  m1;

  // 30 GradDrehung um Y. OK.
  //double da[]={-0.86,  0.,   -0.5,
  //              0.,   -1.,    0.,
  //             -0.5,   0.,    0.86 };
  UT3D_vc_3db (&vx, -0.866, 0.,   -0.5);
  UT3D_vc_3db (&vy,  0.,   -1.,    0.);
  UT3D_vc_3db (&vz, -0.5,   0.,    0.866);

  UT3D_pt_3db (&p1,  0.,  0., 0.);
  UT3D_pt_3db (&p2,-25.,  0., 0.);
  UT3D_pt_3db (&p3,  0.,-25., 0.);

  UT3D_pt_3db (&p1,-100.,   0.,  0.);
  UT3D_pt_3db (&p2,-125.,   0.,  0.);
  UT3D_pt_3db (&p3,-100., -25.,  0.);

  UT3D_pt_3db (&p1, -96.6,  0.,  -32.68);
  UT3D_pt_3db (&p2,-121.6,  0.,  -32.68);
  UT3D_pt_3db (&p3, -96.6, -25., -32.68);


  // 30 GradDrehung um X. OK.
  //double da[]={ 0.,    1.,    0.,      0.,
  //             -0.866, 0.,   -0.5,     0.,
  //             -0.5,   0.,    0.866,   0.  };
  UT3D_vc_3db (&vx,  0.,   -0.86, -0.5);
  UT3D_vc_3db (&vy,  1.,    0.,    0.);
  UT3D_vc_3db (&vz,  0.,   -0.5,   0.86);

  UT3D_pt_3db (&p1,  0.,  0., 0.);
  UT3D_pt_3db (&p2,  0., 25., 0.);
  UT3D_pt_3db (&p3,-25.,  0., 0.);

  UT3D_pt_3db (&p1,   0., 100.,  0.);
  UT3D_pt_3db (&p2,   0., 125.,  0.);
  UT3D_pt_3db (&p3, -25., 100.,  0.);


  UT3D_m3_load (m1, &vx, &vy, &vz);

  UT3D_pt_tra_pt_m3 (&p1, m1, &p1);
  UT3D_pt_tra_pt_m3 (&p1, m1, &p2);
  UT3D_pt_tra_pt_m3 (&p1, m1, &p3);

*/

  Vector   vcx,vcy,vcz;


/*
  printf("UT3D_m3_load vx=%f,%f,%f",vx->dx,vx->dy,vx->dz);
  printf("             vy=%f,%f,%f",vy->dx,vy->dy,vy->dz);
  printf("             vz=%f,%f,%f",vz->dx,vz->dy,vz->dz);
*/


  UT3D_vc_setLength (&vcx, vx, 1.);
  UT3D_vc_setLength (&vcy, vy, 1.);
  UT3D_vc_setLength (&vcz, vz, 1.);

  // UT3D_vc_normalize (&vcx, vx);
  // UT3D_vc_normalize (&vcy, vy);
  // UT3D_vc_normalize (&vcz, vz);


/*
  printf("UT3D_m3_load vx=%f,%f,%f",vcx.dx,vcx.dy,vcx.dz);
  printf("             vy=%f,%f,%f",vcy.dx,vcy.dy,vcy.dz);
  printf("             vz=%f,%f,%f",vcz.dx,vcz.dy,vcz.dz);
*/

  ma[0][0] = vcx.dx;
  ma[1][0] = vcx.dy;
  ma[2][0] = vcx.dz;

  ma[0][1] = vcy.dx;
  ma[1][1] = vcy.dy;
  ma[2][1] = vcy.dz;

  ma[0][2] = vcz.dx;
  ma[1][2] = vcz.dy;
  ma[2][2] = vcz.dz;

  ma[0][3] = 0.;
  ma[1][3] = 0.;
  ma[2][3] = 0.;

}



//==================================================================
  void UT3D_m3_load_o (Mat_4x3 ma,  Point *ori) {
//==================================================================
/// \code
///  UT3D_m3_load_o            Nullpunkt into 4x3 Transform.Matrix.
///  Nach UT3D_m3_load.
/// \endcode


  /* printf("UT3D_m3_load_o %f,%f,%f",ori->x,ori->y,ori->z); */

  ma[0][3] = ori->x;
  ma[1][3] = ori->y;
  ma[2][3] = ori->z;

}


//==========================================================================
  void UT3D_m3_load_povxvy (Mat_4x3 ma, Point *ori, Vector *vx, Vector *vy) {
//==========================================================================
/// UT3D_m3_load_povxvy       neues Achsensystem mit Origin,vx,vy definieren


  Vector   vcx,vcy,vcz;


  // den Z-Vektor errechnen (normal auf vx, vy)
  UT3D_vc_perp2vc (&vcz, vx, vy);


  UT3D_vc_setLength (&vcx, vx,   1.);
  UT3D_vc_setLength (&vcy, vy,   1.);
  UT3D_vc_setLength (&vcz, &vcz, 1.);


  ma[0][0] = vcx.dx;
  ma[1][0] = vcx.dy;
  ma[2][0] = vcx.dz;

  ma[0][1] = vcy.dx;
  ma[1][1] = vcy.dy;
  ma[2][1] = vcy.dz;

  ma[0][2] = vcz.dx;
  ma[1][2] = vcz.dy;
  ma[2][2] = vcz.dz;

  ma[0][3] = ori->x;
  ma[1][3] = ori->y;
  ma[2][3] = ori->z;


  // printf("ex UT3D_m3_load_povxvy vy=%f,%f,%f\n",vcy.dx,vcy.dy,vcy.dz);

  return;

}


//==========================================================================
  void UT3D_m3_load_povxvz (Mat_4x3 ma, Point *ori, Vector *vx, Vector *vz) {
//==========================================================================
/// \code
/// UT3D_m3_load_povxvz       neues Achsensystem mit Origin,vx,vz definieren
/// Doku see UT3D_m3_load
/// \endcode


  Vector   vcx,vcy,vcz;


  // den Y-Vektor errechnen (normal auf vx, vz)
  UT3D_vc_perp2vc (&vcy, vz, vx);


  UT3D_vc_setLength (&vcx, vx,   1.);
  UT3D_vc_setLength (&vcy, &vcy, 1.);
  UT3D_vc_setLength (&vcz, vz,   1.);


  ma[0][0] = vcx.dx;
  ma[1][0] = vcx.dy;
  ma[2][0] = vcx.dz;

  ma[0][1] = vcy.dx;
  ma[1][1] = vcy.dy;
  ma[2][1] = vcy.dz;

  ma[0][2] = vcz.dx;
  ma[1][2] = vcz.dy;
  ma[2][2] = vcz.dz;

  ma[0][3] = ori->x;
  ma[1][3] = ori->y;
  ma[2][3] = ori->z;


  // printf("ex UT3D_m3_load_povxvz vy=%f,%f,%f\n",vcy.dx,vcy.dy,vcy.dz);

  return;

}


//===================================================================
  void UT3D_m3_loadpl (Mat_4x3 m1, Plane *pl1) {
//===================================================================
///  UT3D_m3_loadpl          load Plane(=Achsensystem) into 4x3 Transform.Matrix

/*
  // po=Origin, p1=Punkt_auf_X_Achse, p2=Punkt_in_der_Ebene
  Point     po, p1, p2, p3;
  UT3D_pt_3db (&po, 100.,100.,100.);
  UT3D_pt_3db (&p1, 110.,100.,100.);
  UT3D_pt_3db (&p2, 110.,110.,100.);


  // den X-Vektor errechnen
  Vector    vx, vc1;
  UT3D_vc_2pt (&vx, &po, &p1);

  // weiteren Vektor in Plane errechnen
  UT3D_vc_2pt (&vc1, &po, &p2);

  Plane pl1;  // Plane aus Nullpunkt, X_Vektor u. Vektor in Plane
  UT3D_pl_pto_vcx_vcy (&pl1, &po, &vx, &vc1);


  Mat_4x3   m1; // Matrix mit Plane beladen
  UT3D_m3_loadpl (m1, &pl1);


  UT3D_pt_3db (&p1, 10.,  0.,  0.);
  UT3D_pt_3db (&p2,  0., 10.,  0.);
  UT3D_pt_3db (&p3, 10., 10.,  0.);

  UT3D_pt_tra_pt_m3 (&p1, m1, &p1);
  UT3D_pt_tra_pt_m3 (&p1, m1, &p2);
  UT3D_pt_tra_pt_m3 (&p1, m1, &p3);

*/


  UT3D_m3_load (m1, &pl1->vx, &pl1->vy, &pl1->vz);
  UT3D_m3_load_o (m1, &pl1->po);


}


//======================================================================
  int UT3D_m3_invm3 (Mat_4x3 im1, Mat_4x3 m1) {
//======================================================================
/// \code
///  UT3D_m3_invm3              4x3-Matrix invertieren
/// ACHTUNG: Adresse darf NICHT gleich sein !!
/// \endcode


  double   det, tol = 0.0000001;
  Point    p1, p2;




  /* determinant of A */
  det = (m1[0][0] * m1[1][1] * m1[2][2]) + (m1[0][1] * m1[1][2] * m1[2][0]) +
        (m1[0][2] * m1[1][0] * m1[2][1]) - (m1[0][2] * m1[1][1] * m1[2][0]) -
        (m1[0][0] * m1[1][2] * m1[2][1]) - (m1[0][1] * m1[1][0] * m1[2][2]);
  if (fabs(det) < tol) {
    printf("UT3D_m3_invm3: matrix has no inverse\n");
    return 1;
  }
  det = 1.0 / det;

  im1[0][0] = det * (m1[1][1] * m1[2][2] - m1[2][1] * m1[1][2]);
  im1[1][0] = det * (m1[2][0] * m1[1][2] - m1[1][0] * m1[2][2]);
  im1[2][0] = det * (m1[1][0] * m1[2][1] - m1[2][0] * m1[1][1]);
  im1[0][1] = det * (m1[2][1] * m1[0][2] - m1[0][1] * m1[2][2]);
  im1[1][1] = det * (m1[0][0] * m1[2][2] - m1[2][0] * m1[0][2]);
  im1[2][1] = det * (m1[2][0] * m1[0][1] - m1[0][0] * m1[2][1]);
  im1[0][2] = det * (m1[0][1] * m1[1][2] - m1[1][1] * m1[0][2]);
  im1[1][2] = det * (m1[1][0] * m1[0][2] - m1[0][0] * m1[1][2]);
  im1[2][2] = det * (m1[0][0] * m1[1][1] - m1[1][0] * m1[0][1]);

  im1[0][3] = 0.;
  im1[1][3] = 0.;
  im1[2][3] = 0.;



  /* Den alten Nullpunkt zurückrechnen und laden */
  p1.x = m1[0][3];
  p1.y = m1[1][3];
  p1.z = m1[2][3];
  UT3D_pt_tra_pt_m3 (&p2, im1, &p1);
  im1[0][3] = -p2.x;
  im1[1][3] = -p2.y;
  im1[2][3] = -p2.z;



  return 0;
}


//================================================================
  int UT3D_m3_traVc (Mat_4x3 *ma, Vector *trVc) {
//================================================================
/// UT3D_m3_traVc             translate mat (move origin)              
 
  // dann die Matrix Transformieren (um trVc verschieben)
  (*ma)[0][3] += trVc->dx;
  (*ma)[1][3] += trVc->dy;
  (*ma)[2][3] += trVc->dz;

  return 0;
}


//================================================================
  int UT3D_m3_tram3m3 (Mat_4x3 m3, Mat_4x3 m1, Mat_4x3 m2) {
//================================================================
/// \code
/// UT3D_m3_tram3m3         apply transformation to Mat_4x3
///
/// see also UTRA_app__
/// \endcode

  Point   po;
  Vector  vx, vy, vz;


  UT3D_m3_get (&po, 3, m2);
  UT3D_pt_tra_pt_m3 (&po, m1, &po);
  UT3D_m3_set (m3, 3, &po);

  UT3D_m3_get (&vx, 0, m2);
  UT3D_vc_tra_vc_m3 (&vx, m1, &vx);
  UT3D_m3_set (m3, 0, &vx);

  UT3D_m3_get (&vy, 1, m2);
  UT3D_vc_tra_vc_m3 (&vy, m1, &vy);
  UT3D_m3_set (m3, 1, &vy);

  // UT3D_m3_get (&vz, 2, m2);
  // UT3D_vc_tra_vc_m3 (&vc, m1, &vz);
  // UT3D_m3_set (m3, 2, &vz);

  UT3D_vc_perp2vc (&vz, &vx, &vy);
  UT3D_m3_set (m3, 2, &vz);

  return 0;

}


//===================================================================
  int UT3D_m3_scl (Mat_4x3 *ma, double *sx, double *sy, double *sz) {
//===================================================================
/// UT3D_m3_scl               scale mat

  (*ma)[0][0] *= *sx;  // X-vec * X-scale
  (*ma)[1][0] *= *sx;
  (*ma)[2][0] *= *sx;

  (*ma)[0][1] *= *sy;  // Y-vec * Y-scale
  (*ma)[1][1] *= *sy;
  (*ma)[2][1] *= *sy;

  (*ma)[0][2] *= *sz;  // Z-vec * z-scale
  (*ma)[1][2] *= *sz;
  (*ma)[2][2] *= *sz;


  return 0;

}


//=====================================================================
  void UT3D_m3_multm3 (Mat_4x3 m3, Mat_4x3 m1, Mat_4x3 m2) {
//=====================================================================
/// \code
/// UT3D_m3_multm3             4x3-Matrix mit 4x3 Matrix multiplizieren
///   (nur den 3x3-Anteil)
/// \endcode

//  Eine Matrix mal ihrer inversen Matrix ergibt Einheitsmatrix



    int             zeile, spalte, i1;


    for (zeile = 0; zeile < 3; zeile++)   {

        for (spalte = 0; spalte < 3; spalte++)  {

            m3[zeile][spalte] = 0.0;

            for (i1 = 0; i1 < 3; i1++) {
                m3[zeile][spalte] += (m1[zeile][i1] * m2[i1][spalte]);
            }
        }
    }


  // memcpy (m3, m1, sizeof(Mat_4x3));  // NUR TEST

  // m3[0][3] = m1[0][3] + m2[0][3];
  // m3[1][3] = m1[1][3] + m2[1][3];
  // m3[2][3] = m1[2][3] + m2[2][3];

}


//================================================================
  int UT3D_m3_tra_m3 (Mat_4x3 m3, Mat_4x3 m1, Mat_4x3 m2) {
//================================================================
/// \code
/// m1 ist das aktive Refsys; es soll um m2 transformiert werden.
/// Das resultierende Refsys ist m3.
/// Die Punkte 0,0,0 1,0,0 und 0,0,1 mit m2 transformieren;
/// die Ergebnispunkte dann mit m1 transformieren;
/// daraus das Refsys m3 generieren.
/// \endcode

  Point   po1={0,0,0}, px1={1,0,0}, py1={0,1,0}, pz1={0,0,1};
  Point   po2, px2, py2, pz2;
  Point   po3, px3, py3, pz3;
  Vector  vx2, vy2, vz2;
  Vector  vx3, vy3, vz3;

  // DEB_dump_obj__ (Typ_M4x3, m1, "UT3D_m3_tra_m3 m1\n");
  // DEB_dump_obj__ (Typ_M4x3, m2, "UT3D_m3_tra_m3 m2\n");

  UT3D_pt_tra_pt_m3 (&po2, m1, &po1);
  UT3D_pt_tra_pt_m3 (&px2, m1, &px1);
  UT3D_pt_tra_pt_m3 (&py2, m1, &py1);
  UT3D_pt_tra_pt_m3 (&pz2, m1, &pz1);

  // UT3D_vc_m3 (&vx2, 0, m1);   // copy vec out of 4x3-matrix
  // UT3D_vc_m3 (&vy2, 1, m1);   // copy vec out of 4x3-matrix
  // UT3D_vc_m3 (&vz2, 2, m1);   // copy vec out of 4x3-matrix

  UT3D_pt_tra_pt_m3 (&po3, m2, &po2);
  UT3D_pt_tra_pt_m3 (&px3, m2, &px2);
  UT3D_pt_tra_pt_m3 (&py3, m2, &py2);
  UT3D_pt_tra_pt_m3 (&pz3, m2, &pz2);

  // UT3D_vc_tra_vc_m3 (&vx3, m2, &vx2);
  // UT3D_vc_tra_vc_m3 (&vy3, m2, &vy2);
  // UT3D_vc_tra_vc_m3 (&vz3, m2, &vz2);

  // UT3D_vc_perp3pt (&vy, &po3, &pz3, &px3);
  // UT3D_m3_load_povxvz (m3, &po3, &vx3, &vz3);

  UT3D_vc_2pt (&vx3, &po3, &px3);
  UT3D_vc_2pt (&vy3, &po3, &py3);
  UT3D_vc_2pt (&vz3, &po3, &pz3);

  UT3D_m3_set (m3, 0, &vx3);
  UT3D_m3_set (m3, 1, &vy3);
  UT3D_m3_set (m3, 2, &vz3);
  UT3D_m3_set (m3, 3, &po3);

  // DEB_dump_obj__ (Typ_M4x3, m3, "ex UT3D_m3_tra_m3\n");

/* das folgende liefert scheinbar identisches ergebnis !!!!!!!!!!!!!!!!!!
  Mat_4x4   newMat, oldMat;
  UT3D_m4v_m3 (newMat, trMat);
  UT3D_m4v_m3 (oldMat, maTab[level].ma);
  glPushMatrix ();
  glMatrixMode (GL_MODELVIEW);
  glLoadMatrixd (newMat);
  glMultMatrixd (oldMat);
  glGetDoublev (GL_MODELVIEW_MATRIX, newMat);
  glPopMatrix ();
  UT3D_m3_m4v (maTab[level].ma, newMat);
*/

  return 0;

}


//======================================================================
  int UT3D_m3_rot_m3 (Mat_4x3 m2, Mat_4x3 m1, Vector *vc1, double *a1) {
//======================================================================
// rotate matrix
// m2 and m1 may be identical
  
  Mat_4x3   mr;
  Vector    vx, vy, vz;
  
    
  // printf("UT3D_m3_rot_m3 %f\n",*a1);
    // printf(" vc %f %f %f\n",vc1->dx,vc1->dy,vc1->dz);
  
  if((void*)m1 != (void*)m2)
  UT3D_m3_copy (m2, m1);
  
  
  // get vecs out of mat
  UT3D_m3_get (&vx, 0, m1);
  UT3D_m3_get (&vy, 1, m1);
    // printf(" vx %f %f %f\n",vx.dx,vx.dy,vx.dz);
    // printf(" vy %f %f %f\n",vy.dx,vy.dy,vy.dz);
  
  // rot mat (ang1 um vz drehen)
  UT3D_m3_inirot_angr (mr, NULL, vc1, *a1);
  
  UT3D_vc_tra_vc_m3 (&vx, mr, &vx);
  UT3D_vc_tra_vc_m3 (&vy, mr, &vy);
  UT3D_vc_perp2vc (&vz, &vx, &vy);
    // printf(" vx %f %f %f\n",vx.dx,vx.dy,vx.dz);
    // printf(" vy %f %f %f\n",vy.dx,vy.dy,vy.dz);
    // printf(" vz %f %f %f\n",vz.dx,vz.dy,vz.dz);
  
  // put vecs back into mat
  UT3D_m3_set (m2, 0, &vx);
  UT3D_m3_set (m2, 1, &vy);
  UT3D_m3_set (m2, 2, &vz);

  return 0;

}


//====================================================================
  void UT3D_m4_init (Mat_4x4 ma) {
//====================================================================
/// \code
/// UT3D_m4_init              Initialize a 4x4 - matrix
/// 
/// Fuer horiz u vert matrix. !
///
///       ( 1 0 0 0 )
///       ( 0 1 0 0 )
/// [a] = ( 0 0 1 0 )
///       ( 0 0 0 1 )
///
/// \endcode

  memcpy (ma, UT3D_MAT_4x4, sizeof(Mat_4x4));

}


//====================================================================
  void UT3D_m4_init_ori (Mat_4x4 ma, double px, double py, double pz) {
//====================================================================
/// \code
/// UT3D_m4_init_ori          Initialize a 4x4 - matrix with translation
/// 
/// hor. Matrix !
/// 
/// Mat_4x4-horizontal:
///   Der erste Wert ist der Index ins double-Array.
/// 
/// VX     0=[0][0]=vx.x   1=[0][1]=vx.y   2=[0][2]=vx.z    3=[0][3]=0
/// VY     4=[1][0]=vy.x   5=[1][1]=vy.y   6=[1][2]=vy.z    7=[1][3]=0
/// VZ     8=[2][0]=vz.x   9=[2][1]=vz.y  10=[2][2]=vz.z   11=[2][3]=0
/// PO    12=[3][0]=o.x   13=[3][1]=o.y   14=[3][2]=o.z    15=[3][3]=1
/// \endcode

/*
Define the origin.


Add a rotation with:

  UT3D_m4_init_ori (m1, x, y, z);                // def. origin
  UT3D_m4_init_rot (m2, UT_RADIANS(90.), &v);    // rot around v
  UT3D_m4_addrot (m1, m1, m2);
  UT3D_pt_tra_pt_m4 (&p2, m1, &p1);                // apply

 
       ( 1  0  0  0 )
       ( 0  1  0  0 )
 [a] = ( 0  0  1  0 )
       ( px py pz 1 )
*/

  UT3D_m4_init (ma);

  ma[3][0] = px / 1.;
  ma[3][1] = py / 1.;
  ma[3][2] = pz / 1.;
}



//====================================================================
  void UT3D_m4_init_rot (Mat_4x4 ma, double angle, Vector *axis ) {
//====================================================================
/// UT3D_m4_init_rot          Initialize a 4x4 - matrix with rotation
// 
// hor od. vert Matrix ?
// 
// Give back the matrix for a rotation about an axis.
/* 
  Mat_4x4  m1;
  UT3D_m4_init_rot (m1, UT_RADIANS (90.), &UT3D_VECTOR_X); // 90 deg around x
  UT3D_pt_tra_pt_m4 (&p2, m1, &p1);                          // apply

*/
    int     i;
    Vector  Vu;
    double  sinA, cosA, versA, t;


  UT3D_m4_init (ma);

  sinA = sin(angle);
  cosA = cos(angle);


  /* ---------------- rotate around x ------------------ 
  * Pre-multiply a matrix by a rotate about the x axis.
  *
  *       (  1  0  0  0 )      
  *       (  0  c  s  0 )      
  * [a] = (  0 -s  c  0 )  * [a]
  *       (  0  0  0  1 )      
  */

  if      (axis == &UT3D_VECTOR_X) {
    for (i=0; i<4; ++i) {
      t = ma[1][i] * cosA + ma[2][i] * sinA;
      ma[2][i] = ma[2][i] * cosA - ma[1][i] * sinA;
      ma[1][i] = t;
    }
  }


  /* ---------------- rotate around y ------------------
  * Pre-multiply a matrix by a rotate about the y axis.
  *
  *       (  c  0  s  0 )      
  *       (  0  1  0  0 )      
  * [a] = ( -s  0  c  0 ) * [a]
  *       (  0  0  0  1 )      
  */
  else if (axis == &UT3D_VECTOR_Y) {
    for (i=0; i<4; ++i) {
      t = ma[2][i] * cosA - ma[0][i] * sinA;
      ma[0][i] = ma[0][i] * cosA + ma[2][i] * sinA;
      ma[2][i] = t;
    }
  }


  /* ---------------- rotate around z ------------------
  * Pre-multiply a matrix by a rotate about the z axis.
  *
  *       (  c  s  0  0 )      
  *       ( -s  c  0  0 )      
  * [a] = (  0  0  1  0 ) * [a]
  *       (  0  0  0  1 )      
  */
  else if (axis == &UT3D_VECTOR_Z) {
    for (i=0; i<4; ++i) {
      t = ma[0][i] * cosA + ma[1][i] * sinA;
      ma[1][i] = ma[1][i] * cosA - ma[0][i] * sinA;
      ma[0][i] = t;
    }
  }


  /* ---------------- rotate free ------------------ */
  else {

    Vu = *axis;

    UT3D_vc_normalize (&Vu, &Vu);

    versA = 1 - cosA;

    UT3D_m4_init ( ma );
    ma[0][0] = Vu.dx*Vu.dx*versA + cosA;
    ma[1][0] = Vu.dx*Vu.dy*versA - Vu.dz*sinA;
    ma[2][0] = Vu.dx*Vu.dz*versA + Vu.dy*sinA;
    /* ma[3][0] = 0.; */

    ma[0][1] = Vu.dy*Vu.dx*versA + Vu.dz*sinA;
    ma[1][1] = Vu.dy*Vu.dy*versA + cosA;
    ma[2][1] = Vu.dy*Vu.dz*versA - Vu.dx*sinA;
    /* ma[3][1] = 0.; */

    ma[0][2] = Vu.dx*Vu.dz*versA - Vu.dy*sinA;
    ma[1][2] = Vu.dy*Vu.dz*versA + Vu.dx*sinA;
    ma[2][2] = Vu.dz*Vu.dz*versA + cosA;
    /* ma[3][2] = 0.; */
  }

}


//================================================================
  void UT3D_m4_loadpl (Mat_4x4 m1, Plane *pl1) {
//================================================================
/// \code
/// UT3D_m4_loadpl          load Plane(=Achsensystem) into 4x4 Transform.Matrix
/// kann fuer glMultMatrixd verwendet werden.
/// \endcode


  m1[0][0] = pl1->vx.dx;
  m1[0][1] = pl1->vx.dy;
  m1[0][2] = pl1->vx.dz;
  m1[0][3] = 0.;

  m1[1][0] = pl1->vy.dx;
  m1[1][1] = pl1->vy.dy;
  m1[1][2] = pl1->vy.dz;
  m1[1][3] = 0.;

  m1[2][0] = pl1->vz.dx;
  m1[2][1] = pl1->vz.dy;
  m1[2][2] = pl1->vz.dz;
  m1[2][3] = 0.;

  m1[3][0] = pl1->po.x;
  m1[3][1] = pl1->po.y;
  m1[3][2] = pl1->po.z;
  m1[3][3] = 1.;

}


//==================================================================
  void UT3D_m4_load_o (Mat_4x4 ma,  Point *ori) {
//==================================================================
/// \code
///  UT3D_m3_load_o            set origin into 4x4 Transform.Matrix.
/// \endcode


  /* printf("UT3D_m4_load_o %f,%f,%f",ori->x,ori->y,ori->z); */

  ma[3][0] = ori->x;
  ma[3][1] = ori->y;
  ma[3][2] = ori->z;

}


//====================================================================
  void UT3D_m4_addrot (Mat_4x4 mo, Mat_4x4 ma, Mat_4x4 mb) {
//====================================================================
/// UT3D_m4_addrot            Concatenate 2 4x4 - matrixes.
/*
hor od. vert Matrix ?

Using the same matrix for either input and output is allowed.

Examples:
  UT3D_m4_init_ori (m1, 10., 0., 0.);
  UT3D_m4_init_rot (m2, UT_RADIANS(90.), &v1);
  UT3D_m4_addrot (m1, m1, m2);
  UT3D_pt_tra_pt_m4 (&p2, m1, &p1);
*/

  int     i;
  Mat_4x4 m3;

  for( i=0; i<4; i++ ) {
      m3[i][0] = mb[i][0] * ma[0][0] +
                 mb[i][1] * ma[1][0] +
                 mb[i][2] * ma[2][0] +
                 mb[i][3] * ma[3][0];
      m3[i][1] = mb[i][0] * ma[0][1] +
                 mb[i][1] * ma[1][1] +
                 mb[i][2] * ma[2][1] +
                 mb[i][3] * ma[3][1];
      m3[i][2] = mb[i][0] * ma[0][2] +
                 mb[i][1] * ma[1][2] +
                 mb[i][2] * ma[2][2] +
                 mb[i][3] * ma[3][2];
      m3[i][3] = mb[i][0] * ma[0][3] +
                 mb[i][1] * ma[1][3] +
                 mb[i][2] * ma[2][3] +
                 mb[i][3] * ma[3][3];
  }

  memcpy (mo, m3, sizeof(Mat_4x4));

}





//====================================================================
  void UT3D_m4_addtra ( Mat_4x4 ma, double px, double py, double pz ) {
//====================================================================
/// UT3D_m4_addtra            add a translation into 4x4-matrix
/*
hor od. vert Matrix ?

Example:
  UT3D_m4_init_rot (m1, UT_RADIANS (90.), &v1);
  UT3D_m4_addtra (m1, 10., 0., 0.);
  UT3D_pt_tra_pt_m4 (&p2, m1, &p1);


  multiply a matrix (translation)
 
       ( 1  0  0  0 )
       ( 0  1  0  0 )
 [a] = ( 0  0  1  0 ) * [a] 
       ( px py pz 1 )
*/

  int    i1;
  double *mp;

  mp = ma[0];

  for (i1 = 0; i1<4; i1++) {
    mp[12] += mp[0]*px + mp[4]*py + mp[8]*pz;
    ++mp;
  }


}


//================================================================
  int UT3D_m4_copy (Mat_4x4 mo, Mat_4x4 mi) {
//================================================================
/// UT3D_m4_copy              memcpy


  memcpy (mo, mi, sizeof(Mat_4x4));

  return 0;

}


//================================================================
  int UT3D_m4v_m3 (Mat_4x4 mo, Mat_4x3 mi) {
//================================================================
/// UT3D_m4v_m3      copy a Mat_4x3  -->  vertical-Mat_4x4
/*

Mat_4x4-vertical   (used by OpenGL !)
  Der erste Wert ist der Index ins double-Array.

      VX              VY              VZ           Nullpunkt

  0=[0][0]=vx.x   1=[0][1]=vy.x   2=[0][2]=vz.x    3=[0][3]=o.x
  4=[1][0]=vx.y   5=[1][1]=vy.y   6=[1][2]=vz.y    7=[1][3]=o.y
  8=[2][0]=vx.z   9=[2][1]=vy.z  10=[2][2]=vz.z   11=[2][3]=o.z
 12=[3][0]=0     13=[3][1]=0     14=[3][2]=0      15=[3][3]=1

*/


  memcpy (mo, mi, sizeof(Mat_4x3));

  mo[3][0] = 0.;
  mo[3][1] = 0.;
  mo[3][2] = 0.;
  mo[3][3] = 1.;

  return 0;

}


//================================================================
  int UT3D_m3_m4v (Mat_4x3 mo, Mat_4x4 mi) {
//================================================================
/// UT3D_m3_m4v     copy a vertical-Mat_4x4  -->  Mat_4x3

  memcpy (mo, mi, sizeof(Mat_4x3));

  return 0;

}


/*
//======================================================================
  int UT3D_vc_multvc (Vector *vo, Vector *vi, double d) {
//==================
// UT3D_vc_multvc            Vec-multiplication

// UT3D_vc_multvc     	Author: Thomas Backmeister         22.2.2003

// Multiplication of a vector with a factor.
// vi and vo may be identical.

// IN:
//   Vector *vi ...  vector
//   double d   ...  multiplication factor
// OUT:
//   Vector *vo ...  multiplied vector
// Returncode:
//   0 = OK


  vo->dx = vi->dx * d;
  vo->dy = vi->dy * d;
  vo->dz = vi->dz * d;

  return 0;
}
*/


//================================================================
  int UT3D_pt_intlnsph (Line *ln1, Point *ps, double rs) {
//================================================================
/// \code
/// UT3D_pt_intlnsph          intersect limited line / sphere
/// irc  -1  Line ganz ausserhalb
/// \endcode

  int    i1, i2, irc;
  Vector vl;
  Point  pa[2];


  // befindet sich p1 innerhalb Kugel ?
  i1 = UT3D_pt_ck_inSph (&ln1->p1, ps, rs);

  // befindet sich p2 innerhalb Kugel ?
  i2 = UT3D_pt_ck_inSph (&ln1->p2, ps, rs);

  // beide innerhalb; nix tun.
  if((i1 > 0)&&(i2 > 0)) {irc = 0; goto L_fertig;}

  // die Schnittpunkte erechnen
  UT3D_vc_2pt (&vl, &ln1->p1, &ln1->p2);
  irc = UT3D_pt_intptvcsph (pa, &ln1->p1, &vl, ps, rs);
  if(irc < 2) {irc = -1; goto L_fertig;}
  // if(irc < 2) pa[1] = pa[0];

  // p1 ersetzen
  if(i1 < 0) ln1->p1 = pa[0];

  // p2 ersetzen
  if(i2 < 0) ln1->p2 = pa[1];


  L_fertig:
  // printf("ex UT3D_pt_intlnsph %d\n",irc);
  // GR_Disp_ln (ln1, 2);
  return irc;

}


//================================================================
  int UT3D_ptCen_2Ln (Point *ptCen,
                      Point *pa, Point *pb, Point *pc, Point *pd) {
//================================================================
/// \code
/// UT3D_ptCen_2Ln        give common centerPoint of 2 Lines
/// gemeinsamer Mittelpunkt: Intersect Normalen an den LineMittelpunkten
/// irc =  0 = OK;
/// irc = -1 = parallel; ip = midPoint.
/// \endcode

  int      irc;
  Point    pm1,  pm2, pc1, pc2;
  Vector   vl1, vl2, vn12, vn1, vn2;


  UT3D_vc_2pt (&vl1, pa, pb);
  UT3D_vc_2pt (&vl2, pc, pd);


  // LineMittelpunkte
  UT3D_pt_mid2pt (&pm1, pa, pb);
  UT3D_pt_mid2pt (&pm2, pc, pd);
    // GR_tDyn_symB__ (&pm1, SYM_STAR_S, 2);
    // GR_tDyn_symB__ (&pm2, SYM_STAR_S, 2);


  // Normalvektoren an die Lines
  UT3D_vc_perp2vc (&vn12, &vl1, &vl2);
  UT3D_vc_perp2vc (&vn1, &vl1, &vn12);
  UT3D_vc_perp2vc (&vn2, &vl2, &vn12);
    // GR_tDyn_vc__ (&vn1, &pm1, 9, 0);
    // GR_tDyn_vc__ (&vn2, &pm2, 9, 0);


  // IntersectionPoint pm1-vn1 X pm2-vn2
  UT3D_pt_traptvc (&pc1, &pm1, &vn1);
  UT3D_pt_traptvc (&pc2, &pm2, &vn2);
  irc = UT3D_pt1_int2pt2pt (ptCen, &pm1, &pc1, &pm2, &pc2);
    // GR_tDyn_symB__ (ptCen, SYM_STAR_S, 0);


  return irc;

}

/*
//======================================================================
  int UT3D_pt_intlnci__ (int *np, Point xp[], Line *ln, Circ *ci) {
//======================================================================
// UT3D_pt_intlnci__     Author: Thomas Backmeister       2.7.2003
/// \code
/// UT3D_pt_intlnci__     intersect unlimited line and unlimited circle
/// 
/// Intersection of line and circle.
/// The line need NOT lie in the plane of the circle.
/// 
/// IN:
///   Line *ln    ... 3D-line
///   Circ *ci    ... 3D-circle
/// OUT:
///   int *np     ... number of intersection points (0,1,2)
///   Point xp[]  ... array of intersection points
/// Returncodes:
///   0 = OK
///   1 = no intersection
/// \endcode


  int    rc;
  double dist;
  Point  ip;
  Plane  pl;

  //printf("UT3D_pt_intlnci__: intersect line and circle\n");

  // init
  *np = 0;


  // plane of circle
  UT3D_pl_ptvc (&pl, &(ci->pc), &(ci->vz));

  // check line || plane of circle
  rc = UT3D_ln_ck_parpl (&dist, ln, &pl, UT_TOL_PAR);


  // --- line NOT || plane of circle ---
  if (rc == 0) {

    // intersect line with plane of circle
    rc = UT3D_pt_intlnpl (&ip, &pl, ln);
    // printf(" n.UT3D_pt_intlnpl %d\n",rc);
    if (rc == 0) return 1;

    // distance between intersection point and circle centerpoint
    dist = UT3D_len_2pt (&ip, &(ci->pc));
    // printf(" dist=%f\n",*dist);

    // save point, if distance is equal radius of circle
    if (UTP_comp2db (dist, fabs(ci->rad), UT_TOL_pt)) {
      xp[*np] = ip;
      ++(*np);
    }
    else return 1;
  }


  // --- line || plane of circle ---
  else {

    // intersect line with circle
    if(fabs(dist) < UT_TOL_pt) return (UT3D_pt_intlnci_p (np, xp, ln, ci));

    else return 1;
  }

  return 0;
}


//======================================================================
  int UT3D_pt_intlnci_p (int *np, Point xp[], Line *ln, Circ *ci1) {
//======================================================================
// UT3D_pt_intlnci_p       Author: Thomas Backmeister         22.2.2003
/// \code
/// UT3D_pt_intlnci_p         intersection LN-CIR (gives 0/1/2 points)
/// 
/// Intersection of 3D-line with 3D-circle.
/// The line must lie in the plane of the circle. !!
/// 
/// IN:
///   Line *ln   ...  3D-line
///   Circ *ci1   ...  3D-circle
/// OUT:
///   int *np    ...  number of intersection points (0,1,2)
///   Point xp[] ...  array of intersection points
/// Returncode:
///   0 = OK
///   1 = no intersection; xp[0]=pc projected -> ln
/// \endcode


  int    irc;
  double len, rd, d1, tol1;
  Point  pd;
  Vector vl;


  // printf("UT3D_pt_intlnci_p\n");
  // DEB_dump_obj__ (Typ_LN, ln, "");
  // DEB_dump_obj__ (Typ_CI, ci1, "");


  // init
  *np = 0;
  // tol1 = UT_TOL_cv * 5.;  // beim Tesselate Polygone aus Kreisen .. !
  // tol1 = UT_DISP_cv;
  // tol1 = UT_TOL_pt;  // Vergroessern verringert die Rechengenauigkeit !!!
  tol1 = UT_TOL_min1;  // Vergroessern verringert die Rechengenauigkeit  !!!


  // vl = vector of line
  UT3D_vc_ln (&vl, ln);


  // pd = proj. CIR-Cen -> LN
  UT3D_pt_projptptvc (&pd, &len, NULL, &(ci1->pc), &(ln->p1), &vl);
  // printf(" len=%f pd=%f,%f,%f\n",len,pd.x,pd.y,pd.z);

  rd = fabs(ci1->rad); // rad can be neg !


  // sind nicht im Grenzbereich !
  // if ((len-tol1) > rd) {
  if(len > rd + UT_TOL_pt ) {
    // printf(" len=%f rd=%f\n",len,rd);
    xp[0] = pd; 
    *np = 0;
    irc = 1;
    goto L_fertig;
  }


  // do they touch
  // if (UTP_comp2db (len, rd, tol1) != 0) {
  if(len > rd - tol1) {
    xp[0] = pd; 
    *np = 1;
    irc = 0;
    goto L_fertig;
  }


  // Phytagoras ..
  d1 = sqrt((rd * rd) - (len * len));
  UT3D_pt_traptvclen (&xp[0], &pd, &vl,  d1);
  UT3D_pt_traptvclen (&xp[1], &pd, &vl, -d1);

  *np = 2;
  irc = 0;


  L_fertig:



  // nur Testausg:
  // printf("ex UT3D_pt_intlnci_p irc=%d np=%d\n",irc,*np);
  // if(*np > 0) printf("     %f %f %f\n",xp[0].x,xp[0].y,xp[0].z);
  // if(*np > 1) printf("     %f %f %f\n",xp[1].x,xp[1].y,xp[1].z);



  return irc;
}
*/


//============================================================================
  int UT3D_pt_intlnci_l (Point pa[], Line *ln, int lnLim, Circ *ci, int ciLim) {
//============================================================================
/// \code
/// UT3D_pt_intlnci_l       intersect line and circle (limited|unlimited)
/// 
/// Intersection of line and circle.
/// The line need NOT lie in the plane of the circle.
/// 
/// IN:
///   Line *ln        3D-line
///   lnLim           0=limited Line, 1=unlimited Line
///   Circ *ci        3D-circle
///   ciLim           0=limited Circ, 1=unlimited Circ
/// OUT:
///   Point pa[]      array of 0-2 intersection points
/// Returncode:
///  -1               no intersection
///   1               1 intersectionpoint (pa[0])
///   2               2 intersectionpoints (pa[0] and pa[1])
/// \endcode


  int    irc;
  double dist;
  Point  ip;
  Plane  pl;


  // printf("UT3D_pt_intlnci_l %d %d\n",lnLim,ciLim);
  // DEB_dump_obj__ (Typ_LN, ln, "lim=%d",lnLim);
  // DEB_dump_obj__ (Typ_CI, ci, "lim=%d",ciLim);


  // plane of circle
  UT3D_pl_ptvc (&pl, &(ci->pc), &(ci->vz));

  // check if line is parallel to plane of circle
  irc = UT3D_ln_ck_parpl (&dist, ln, &pl, UT_TOL_PAR);
    // printf(" ck_parpl irc=%d dist=%f\n",irc,dist);

  //----------------------------------------------------------------
  // line NOT parallel to plane of circle; only 1 solution
  if (irc == 0) {


    // intersect limited line with plane of circle
    if(lnLim < 1) {
      // line is limited
      // intersect LineSegment X Plane
      irc = UT3D_pt_intplnln (pa, &dist, &pl, ln);
      if(irc < 0) return -1;


    } else {
      // intersect unlimited line with plane of circle
      irc = UT3D_pt_intlnpl (pa, &pl, ln);
      // printf(" n.UT3D_pt_intlnpl %d\n",irc);
      // if (irc == 0) return 1;  // parallel is not possible ..
    }


    // distance between intersection point and circle centerpoint
    dist = UT3D_len_2pt (&ip, &(ci->pc));
    // printf(" dist=%f\n",*dist);


    // if distance is not equal to radius of circle
    if (UTP_comp2db (dist, fabs(ci->rad), UT_TOL_pt) == 0) return -1;


    // test if pa[0] is inside circle-segment
    if(ciLim > 0) return 1;
    // circ is limited
    irc = UT3D_pt_ck_inCirc (ci, pa, UT_TOL_pt);
    if (irc < 0) return -1;

    return 1;

  }


  //----------------------------------------------------------------
  // line is parallel to plane of circle, 2 solutions
  else {

    // dist line-plane  must be 0
    if(fabs(dist) > UT_TOL_pt) return -1;


    // intersect line with circle
    return UT3D_pt_intlnci_lp (pa, ln, lnLim, ci, ciLim);

  }

}


//====================================================================
  int UT3D_pt_intlnci_lp (Point pa[],
                          Line *ln1, int lnLim,
                          Circ *ci1, int ciLim) {
//====================================================================
/// \code
/// UT3D_pt_intlnci_p         intersection LN-CIR (limited|unlimited)
/// The line must lie in the plane of the circle. !!
/// 
/// IN:
///   Line *ln        3D-line
///   lnLim           0=limited Line, 1=unlimited Line
///   Circ *ci        3D-circle
///   ciLim           0=limited Circ=LIMITED, 1=unlimited-Circ=UNLIMITED
/// OUT:
///   Point pa[]    array of intersection points; size 2
/// Returncode:
///  -1               no intersection
///   1               1 intersectionpoint (pa[0])
///   2               2 intersectionpoints (pa[0] and pa[1])
/// \endcode


  int    irc, i1;
  double len, rd, d1;
  Point  plc;
  Vector vl;


  // printf("UT3D_pt_intlnci_lp\n");
  // DEB_dump_obj__ (Typ_LN, ln1, "lim=%d",lnLim);
  // DEB_dump_obj__ (Typ_CI, ci1, "lim=%d",ciLim);


  // init
  // tol1 = UT_TOL_min1;  // Vergroessern verringert die Rechengenauigkeit  !!!
  irc = -1;


  // vl = vector of line
  UT3D_vc_ln (&vl, ln1);


  // plc = proj. CIR-Cen -> LN
  UT3D_pt_projptptvc (&plc, &len, NULL, &(ci1->pc), &(ln1->p1), &vl);


  rd = fabs(ci1->rad); // rad can be neg !
    // printf(" len=%f rd=%f plc=%f,%f,%f\n",len,rd,plc.x,plc.y,plc.z);


  // they touch: 1 point ..
  if (UTP_comp2db (len, rd, UT_TOL_pt) != 0) {
    pa[0] = plc;
    irc = 1;
    goto L_fertig;
  }


  if(len > rd) goto L_exit;  // no intersection ..



  // Phytagoras ..
  d1 = sqrt((rd * rd) - (len * len));
  UT3D_pt_traptvclen (&pa[0], &plc, &vl,  d1);
  UT3D_pt_traptvclen (&pa[1], &plc, &vl, -d1);

  irc = 2;


  //----------------------------------------------------------------
  // check if results are in segments
  L_fertig:
    // DEB_dump_obj__ (Typ_PT, &pa[0], " pa[0]");
    // DEB_dump_obj__ (Typ_PT, &pa[1], " pa[1]");

  if(lnLim < 1) {
    // line is limited
    // check if points are in lineSeg ln1
    irc = UT3D_ck_npt_inLine (pa, irc, ln1);
      // printf(" npt_inLine=%d\n",irc);
  }

  if(ciLim < 1) {
    // circ is limited
    // check if points are in circSeg ci1
    irc = UT3D_ck_npt_inCirc (pa, irc, ci1);
      // printf(" npt_inCirc=%d\n",irc);
  }


  L_exit:

  // nur Testausg:
  // printf("ex UT3D_pt_intlnci_p irc=%d np=%d\n",irc,irc);
  // if(irc > 0) printf("     %f %f %f\n",pa[0].x,pa[0].y,pa[0].z);
  // if(irc > 1) printf("     %f %f %f\n",pa[1].x,pa[1].y,pa[1].z);


  return irc;
}


//======================================================================
  int UT3D_pt_intlnci_p (int *np, Point xp[], Line *ln, Circ *ci1) {
//======================================================================
// UT3D_pt_intlnci_p       Author: Thomas Backmeister         22.2.2003
/// \code
/// UT3D_pt_intlnci_p         intersection LN-CIR (both unlimited; 0/1/2 points)
/// 
/// Intersection of 3D-line with 3D-circle.
/// The line must lie in the plane of the circle. !!
/// 
/// IN:
///   Line *ln   ...  3D-line
///   Circ *ci1   ...  3D-circle
/// OUT:
///   int *np    ...  number of intersection points (0,1,2)
///   Point xp[] ...  array of intersection points
/// Returncode:
///   0 = OK
///   1 = no intersection; xp[0]=pc projected -> ln
/// \endcode

  *np = UT3D_pt_intlnci_lp (xp, ln, 1, ci1, 1);
  if(*np < 0) return 1;
  return 0;

}


//======================================================================
  int UT3D_pt_intlnci__ (int *np, Point xp[], Line *ln, Circ *ci) {
//======================================================================
// UT3D_pt_intlnci__     Author: Thomas Backmeister       2.7.2003
/// \code
/// UT3D_pt_intlnci__     intersect line - circle (both unlimited)
/// 
/// Intersection of line and circle.
/// The line need NOT lie in the plane of the circle.
/// 
/// IN:
///   Line *ln    ... 3D-line
///   Circ *ci    ... 3D-circle
/// OUT:
///   int *np     ... number of intersection points (0,1,2)
///   Point xp[]  ... array of intersection points
/// Returncodes:
///   0 = OK
///   1 = no intersection
/// \endcode

  *np = UT3D_pt_intlnci_l (xp, ln, 1, ci, 1);
  if(*np < 0) return 1;
  return 0;

}


//======================================================================
  int UT3D_ln_tra_parl (Line *lno, Line *lni, double dist, Vector *vz) {
//======================================================================
// UT3D_ln_tra_parl              offset line
// Input:
//   vz    must be normalized


  Vector    vx, vy;


  // DEB_dump_obj__ (Typ_LN, lni, "UT3D_ln_tra_parl");


  // make vec vx along line, normalize
  UT3D_vc_ln (&vx, lni);
  UT3D_vc_setLength (&vx, &vx, 1.);

  // get vec vy from vx and vz
  UT3D_vc_perp2vc (&vy, vz, &vx);

  // multiply vy * dist
  UT3D_vc_multvc (&vy, &vy, dist);

  // translate line
  UT3D_ln_tra_vc (lno, lni, &vy);

    // DEB_dump_obj__ (Typ_LN, lno, "ex-UT3D_ln_tra_parl");
    // GR_tDyn_obj (Typ_LN, lno, Typ_Att_hili, 1);

  return 0;

}



//================================================================
  int UT3D_ln_tra_vc (Line *lno, Line *lni, Vector *vco) {
//================================================================
// translate line

  *lno = *lni;
  UT3D_pt_traptvc (&lno->p1, &lni->p1, vco);
  UT3D_pt_traptvc (&lno->p2, &lni->p2, vco);

    // DEB_dump_obj__ (Typ_LN, lno, "ex UT3D_ln_tra_vc ");

  return 0;

}


//=====================================================================
  int UT3D_ln_tng_ci_vc (Line *lno, Circ *ci1, Vector *vc1, int imod) {
//=====================================================================
/// \code
/// UT3D_ln_tng_ci_vc          tangent with fixed vector to circ
/// imod      0 = point at y-axis (vc1=x-axis; ci1.vz=z-axis)
///           1 = point at negative y-axis
/// \endcode


  Point   pt1;

  UT3D_pt_tng_ci_vc (&pt1, ci1, vc1, imod); // tangent-point on circ
    // DEB_dump_obj__(Typ_PT, &pt1, "pt1");

  UT3D_ln_ptvc (lno, &pt1, vc1);
    // GR_Disp_ln (lno, 9);


  return 0;

}


//=======================================================================
  int UT3D_ln_tng_ci_ci (Line *ln1, Circ *ci1, Circ *ci2, int sNr) {
//=======================================================================
/// \code
/// UT3D_ln_tng_ci_ci          Tangente (4 Lines) an CIR - CIR
///   sNr     solution-number 0-3    
/// Output:
///   RC = nr of possible solutions; -1 (no solution), 1|2|3|4
///
///   ------------1--------
///     \ \         _/ /
///      \ \__3   4/  /
///      |   _\ __/   |
///      /__/  \___   \
///     //         \__ \
///   -------2-------------
///
/// \endcode

  int      irc, i1;
  double   lb, lc, r1, r2, dx, dy, f1, f2;
  Vector   vcc, vcn, vc1, vc2;
  Point    pt0, pt1, pt2;


  // printf("UT3D_ln_tng_ci_ci %d\n",sNr);
  // DEB_dump_obj__ (Typ_CI, ci1, " ci1");
  // DEB_dump_obj__ (Typ_CI, ci2, " ci2");


  // lc = dist centerpoints
  lc = UT3D_len_2pt (&ci1->pc, &ci2->pc);
  if(UTP_compdb0(lc, UT_TOL_pt)) goto AC_AC_err; // concentric
  r1 = fabs(ci1->rad);
  r2 = fabs(ci2->rad);
    // printf(" r1=%f r2=%f lc=%f\n",r1,r2,lc);

  // vcc = von ci1.pc --> ci2.pc
  UT3D_vc_2pt (&vcc, &ci1->pc, &ci2->pc);

  // normalize vcc
  UT3D_vc_div_d  (&vcc, &vcc, lc);
    // DEB_dump_obj__ (Typ_VC, &vcc, " vcc");

  // vcn = Crossvec von vcc | ci.vz   (y-vector from vcc=x-vector, axis=z-vector)
  UT3D_vc_perp2vc (&vcn, &ci1->vz, &vcc);
    // DEB_dump_obj__ (Typ_VC, &vcn, " vcn");


  // test touch outside
  dx = r1 + r2;
    // printf(" lc=%f dx=%f\n",lc,dx);


  // test touch outside
  if(UTP_comp2db(dx, lc, UT_TOL_pt)) {
    // yes, touching outside; 3 solutions possible
    irc = 3;
    if(sNr > 2) goto AC_AC_err;
    if(sNr < 2) goto L_ac_01;
    f1 = r1;
    goto L_ac_perp;


  // test touch inside
  } else if(UTP_comp2db(fabs(r1 - r2), lc, UT_TOL_pt)) {
    // yes, touching inside; 1 solution possible
    irc = 1;
    if(sNr > 0) goto AC_AC_err;
    f1 = r1;
    if(r2 > r1) f1 *= -1.;
    goto L_ac_perp;


  // test inside - 0 solutions
  } else if(lc < fabs(r1 - r2)) {
    irc = 0;
    goto AC_AC_err;


  // test ci1 intersects ci2
  } else if(dx > lc) {
    // yes, ci1 intersects ci2; max 2 solutions
    irc = 2;
    if(sNr > 1) goto AC_AC_err;
    goto L_ac_01;


  } else {
    // max 4 solutions
    irc = 4;
    if(sNr > 3) goto AC_AC_err;
    if(sNr < 2) goto L_ac_01;
    goto L_ac_23;
  }



  //----------------------------------------------------------------
  // ln1 = line perp to ci1-ci2;
  L_ac_perp:
      // printf(" L_ac_perp:\n");
    if(fabs(lc) < UT_TOL_pt) goto AC_AC_err;
    // pt0 = point on ci1 on line ci1->pc - ci2->pc
    UT3D_pt_traptmultvc (&ln1->p1, &ci1->pc, &vcc, f1);
    UT3D_pt_traptmultvc (&ln1->p2, &ln1->p1, &vcn, UT_DISP_cv*1000.);
    goto L_done;


  //----------------------------------------------------------------
  L_ac_01:
  // solutions 0 and 1 (outer tangents)
    // printf(" L_ac_01:\n");

  // test for parallel (same radius)
  if(UTP_comp2db(r1, r2, UT_TOL_pt)) {
    dx = 0.;
    dy = 1.;
    if(sNr < 1) dy = -1.;   // sol.0 = below

    f1 = -r1;
    f2 = -r1;
    goto L_move;
  }

  lb = r1 - r2;
  UT2D_solvtriri_bc (&dx, &dy, lb, lc);
  if((sNr == 1)&&(r1 > r2)) dy *= -1.;   // sol.0 = below
  if((sNr == 0)&&(r1 < r2)) dy *= -1.;   // sol.0 = below
    // printf(" r1=%f r2=%f lc=%f lb=%f dx=%f dy=%f\n",r1,r2,lc,lb,dx,dy);

  f1 = r1 / lb;
  f2 = r2 / lb;
  goto L_move;
  

  //----------------------------------------------------------------
  L_ac_23:
  // solutions 2 and 3 (inner tangents)

  // test for touch

  // test for no solution


  lb = r1 + r2;
  UT2D_solvtriri_bc (&dx, &dy, lb, lc);
  if(sNr == 3) dy *= -1.;
    // printf("L_ac_23:r1=%f r2=%f lc=%f lb=%f dx=%f dy=%f\n",r1,r2,lc,lb,dx,dy);

  f1 = r1 / lb;
  f2 = r2 / lb * -1.;
  goto L_move;
  

  //----------------------------------------------------------------
  L_move:
  // vc1 = normalvector to new line; length lb
    // printf(" L_move: f1=%f f2=%f\n",f1,f2);

  // f1,f2 = multipl.factors to tangent-points
  UT3D_pt_tra_pt_2vc_2par (&ln1->p1, &ci1->pc, &vcc, dx*f1, &vcn, dy*f1);
  UT3D_pt_tra_pt_2vc_2par (&ln1->p2, &ci2->pc, &vcc, dx*f2, &vcn, dy*f2);


  goto L_done;



/*
  //----------------------------------------------------------------
  if(UTP_comp2db(r1, r2, UT_TOL_pt)) {
    // circles have same radius
    // nur die Mittellinie parallelverschieben
    if(sNr < 3) {
      dx = 0.;
      dy = r1;
      goto L_ac_std;
    }
  }


  //----------------------------------------------------------------
  if(sNr >= 2)   lb = -(r1 + r2);      // inner sol.
  else           lb = r2 - r1;         // outer sol.

  UT2D_solvtriri_bc   (&dx, &dy, lb, lc);
  // printf(" r1=%f r2=%f lc=%f lb=%f dx=%f dy=%f\n",r1,r2,lc,lb,dx,dy);

  if(lb < 0.) {
    dx = -dx;
  }
  goto L_ac_std;

  //----------------------------------------------------------------
  L_ac_std:
  // pt0 = von ci2.pc in Richt vcc Abst=dx
  // pt1 = von pt0 in Richt vcn Abst=-dy
  // pt2 = von pt0 in Richt vcn Abst=dy
  UT3D_pt_traptvclen (&pt0, &ci2->pc, &vcc, dx);

  // vc1 = ci2.pc --> pt1
  // vc2 = ci2.pc --> pt2
  // p1 = von ci1.pc in Richt vc1 Abst=r1
  // p2 = von ci2.pc in Richt vc2 Abst=r2

  i1 = sNr % 2; // module; 0 od 1
  if(i1 < 1) {
    UT3D_pt_traptvclen (&pt1, &pt0, &vcn, -dy);
    UT3D_vc_2pt (&vc1, &ci2->pc, &pt1);
    UT3D_pt_traptvclen (&ln1->p1, &ci1->pc, &vc1, r1);
  } else {
    UT3D_pt_traptvclen (&pt2, &pt0, &vcn, dy);
    UT3D_vc_2pt (&vc2, &ci2->pc, &pt2);
    UT3D_pt_traptvclen (&ln1->p1, &ci1->pc, &vc2, r1);
  }

  // solution 2,3:
  if(sNr >= 2)  r2 = -r2;
  if(i1 < 1)
    UT3D_pt_traptvclen (&ln1->p2, &ci2->pc, &vc1, r2);
  else
    UT3D_pt_traptvclen (&ln1->p2, &ci2->pc, &vc2, r2);

  goto L_done;
*/


  //----------------------------------------------------------------
  L_done:
      // printf("ex UT3D_ln_tng_ci_ci %d\n",irc);
      // DEB_dump_obj__ (Typ_LN, ln1, " ex-n_tng_ci_ci-ln1");
      // GR_Disp_ln (ln1, 9);
    return irc;


  AC_AC_err:
    printf("UT3D_ln_tng_ci_ci no solution ..\n");
    return -1;

}


//================================================================
  int UT3D_ln_projlnci (Line *lno, Line *lni, Circ *cii) {
//================================================================
/// line = project endpoints of line --> circPlane

  UT3D_pt_projptptnvc (&lno->p1, &lni->p1, &cii->pc, &cii->vz);
  UT3D_pt_projptptnvc (&lno->p2, &lni->p2, &cii->pc, &cii->vz);

    // GR_tDyn_symB__ (&lno->p1, SYM_STAR_S, 2);
    // GR_tDyn_symB__ (&lno->p2, SYM_STAR_S, 2);


  return 0;

}


//================================================================
  int UT3D_ln_parl2ln (Point *po1,Point *po2,
                       Point *p11,Point *p12, Point *p21,Point *p22) {
//================================================================
/// \code
/// UT3D_ln_parl2ln           ln <== gemeinsame Strecke of 2 lines
/// rc = -1:  keine gemeinsame Strecke
/// rc =  0:  po1-po2 ist die gemeinsame Strecke
/// \endcode


  int    irc, bp, i1, i2;
  double dt[4];
  Point  pb1, pb2, pb3, pb4;
  Vector vc1;
  Line   ln1;


  // GR_tDyn_symB__ (p11, SYM_STAR_S, 2);
  // GR_tDyn_symB__ (p12, SYM_STAR_S, 2);
  // GR_tDyn_symB__ (p21, SYM_STAR_S, 2);
  // GR_tDyn_symB__ (p22, SYM_STAR_S, 2);


  // mit BBoxen testen ob 2 Lines gemeinsamen Part haben ..
  UT3D_box_2pt_tol (&pb1, &pb2, p11, p12, UT_TOL_pt);
  UT3D_box_2pt_tol (&pb3, &pb4, p21, p22, UT_TOL_pt);
    // GR_tDyn_box__(&pb1, &pb2, 2);
    // GR_tDyn_box__(&pb3, &pb4, 2);


  irc = UT3D_ckBoxinBox1 (&pb1, &pb2, &pb3, &pb4);
  if(irc < 0) return -1;

  // vc1 = p11,p12 (ln1)
  UT3D_vc_2pt (&vc1, p11, p12);

  // test ob die Punkte der ln2 auf ln1 liegen
  irc = UT3D_pt_ck_ptvc (p21, p11, &vc1, UT_TOL_pt);
  if(irc < 0) return -1;

  irc = UT3D_pt_ck_ptvc (p22, p11, &vc1, UT_TOL_pt);
  if(irc < 0) return -1;

  // ja, alle 4 Punkte befinden sich in einer Linie ...
  // ln1 = p11,p12
  UT3D_ln_ptpt (&ln1, p11, p21);

  // find Backplane
  UT3D_vc_2pt (&vc1, p11,p12);
  bp = UT3D_bp_perp_vc (NULL, &vc1);
  // printf(" bp=%d\n",bp);


  // 4 Zahlen = 3 Vektorlaengen + Zahl 0
  dt[0] = 0.; // p11
  dt[1] = 1.; // p11-p12

  dt[2] = UT3D_parpt_lnbp (p21, &ln1, bp); // p11-p21
  // printf(" p21=%f\n",dt[2]);

  dt[3] = UT3D_parpt_lnbp (p22, &ln1, bp); // p11-p22
  // printf(" p22=%f\n",dt[3]);


  // innere Zahlen von 4 Zahlen
  UTP_2db_ck_in4db (&i1, &i2, dt);

  if(i1 == 0) *po1 = *p11;
  else if(i1 == 1) *po1 = *p12;
  else if(i1 == 2) *po1 = *p21;
  else if(i1 == 3) *po1 = *p22;

  if(i2 == 0) *po2 = *p11;
  else if(i2 == 1) *po2 = *p12;
  else if(i2 == 2) *po2 = *p21;
  else if(i2 == 3) *po2 = *p22;


  // TEST ONLY
  // GR_tDyn_symB__ (po1, SYM_SQU_B, 2);
  // GR_tDyn_symB__ (po2, SYM_SQU_B, 2);
  // UT3D_ln_ptpt(&ln1,po1,po2);GR_Disp_ln(&ln1,2);


  return 0;

}


//======================================================================
  int UT3D_ln_int2pl (Line *ln, Plane *pl1, Plane *pl2) {
//======================================================================
// UT3D_ln_int2pl     Author: Thomas Backmeister         22.2.2003
/// \code
/// UT3D_ln_int2pl            LN = Intersection PLN / PLN
/// 
/// 
/// Intersection of 2 planes.
/// 
/// The 1. point of the intersection line is the closest to the
/// coordinate origin.
/// The 2. point of the intersection line has distance 1 to the 1. point.
/// 
/// IN:
///   Plane *pl1 ...  1. plane
///   Plane *pl2 ...  2. plane
/// OUT:
///   Line *ln   ...  intersection line of the 2 planes
/// Returncodes:
///   0 = OK
///   1 = no intersection (parallel planes)
/// \endcode


  double lvl, d1, d2;
  Vector vl, v1, v2;


  // printf("UT3D_ln_int2pl: intersection of 2 planes\n");
  // DEB_dump_obj__ (Typ_PLN, pl1, "");
  // DEB_dump_obj__ (Typ_PLN, pl2, "");
  // GR_Disp_pln (pl1, 2);
  // GR_Disp_pln (pl2, 2);


  // vector of intersection line
  UT3D_vc_perp2vc (&vl, &(pl1->vz), &(pl2->vz));


  // check if planes are parallel
  lvl = UT3D_acos_2vc (&vl, &vl);
  if(UTP_comp_0 (lvl)) {
    // printf("UT3D_ln_int2pl - parallel\n");
    return 1;
  }


  // normalize line vector
  lvl = sqrt (lvl);
  UT3D_vc_multvc (&vl, &vl, 1.0/lvl);


  // some cross products
  UT3D_vc_perp2vc (&v1, &(pl2->vz), &vl);
  UT3D_vc_perp2vc (&v2, &vl, &(pl1->vz));


  // some parameters
  d1 = pl1->po.x*pl1->vz.dx + pl1->po.y*pl1->vz.dy + pl1->po.z*pl1->vz.dz;
  d1 /= lvl;
  d2 = pl2->po.x*pl2->vz.dx + pl2->po.y*pl2->vz.dy + pl2->po.z*pl2->vz.dz;
  d2 /= lvl;


  // 1. point of intersection line
  ln->p1.x = d1 * v1.dx + d2 * v2.dx;
  ln->p1.y = d1 * v1.dy + d2 * v2.dy;
  ln->p1.z = d1 * v1.dz + d2 * v2.dz;


  // 2. point of intersection line
  UT3D_pt_traptvc (&(ln->p2), &(ln->p1), &vl);



  // printf("ex UT3D_ln_int2pl %f,%f,%f\n",ln->p1.x,ln->p1.y,ln->p1.z);
  // printf("                  %f,%f,%f\n",ln->p2.x,ln->p2.y,ln->p2.z);
  // GR_tDyn_pcv (ln, 2, 2);


  return 0;
}

//======================================================================
  int UT3D_ln_pts_dmax (Line *ln, int np, Point *ptab, double tol) {
//======================================================================
// UT3D_ln_pts_dmax         Author: Thomas Backmeister       5.6.2003
/// \code
/// UT3D_ln_pts_dmax          line <-- points with max. distance of pointTab.
/// 
/// 
/// Computation of a line from a set of points.
/// The line points will be such that their distance is maximal!
/// Die am weitesten auseinanderliegenden Punkte suchen, Linie davon machen.
/// 
/// IN:
///   int np      ... number of points
///   Point *ptab ... points
///   double tol  ... tolerance: minimal desired distance between
///                              line points
/// OUT:
///   Line *ln    ... line with maximal point distance
/// Returncodes:
///   1: line exists
///   0: no line exists
/// \endcode


  int lnex, i1, i2, ind1, ind2;
  double dist, distmax;

  //printf("UT3D_ln_pts_dmax: line <-- set of points\n");

  // init
  lnex = 0;

  // check number of points
  if (np <= 1) {
    printf("UT3D_ln_pts_dmax: no line exists\n");
    return lnex;
  }

  // find the most distant points
  distmax = 0.0;
  ind1 = -1;
  ind2 = -1;
  for (i1=0; i1<np; ++i1) {
    for (i2=i1+1; i2<np; ++i2) {
      dist = UT3D_len_2pt (&ptab[i1], &ptab[i2]);
      if (dist > distmax) {
        distmax = dist;
        ind1 = i1;
        ind2 = i2;
      }
    }
  }

  // check point distance
  if (ind1 >= 0) {
    if (dist >= tol) {
      lnex = 1;
      ln->p1 = ptab[ind1];
      ln->p2 = ptab[ind2];
    }
  }

  return lnex;
}


//================================================================
  int UT3D_ln_setLen (Line *lno, Point *pc, double lnlg, Line *lni) {
//================================================================
/// change LineLength; center around pc

  Vector   vc1;


  // make vec from pa-pe
  UT3D_vc_2pt (&vc1, &lni->p1, &lni->p2);

  // give length
  UT3D_vc_setLength (&vc1, &vc1, lnlg / 2.);

  // add to pc
  UT3D_pt_traptvc (&lno->p2, pc, &vc1);
  UT3D_pt_traptivc (&lno->p1, pc, &vc1);

  return 0;

}


/*
//======================================================================
  int UT3D_pt_intplci (int *np, Point xp[], Plane *pl, Circ *ci) {
//======================================================================
// UT3D_pt_intplci     Author: Thomas Backmeister         22.2.2003
/// \code
/// UT3D_pt_intplci           intersect PLN CIR (gives 0/1/2 points)
///                           unlimited circle only !
/// 
///
/// Intersection of plane and 3D-circle.
/// 
/// IN:
///   Plane *pl  ...  a plane
///   Circ  *ci  ...  a circle
/// OUT:
///   int *np    ...  number of intersection points (0,1,2)
///   Point xp[] ...  array of intersection points
/// Returncode:
///   0 = OK
///   1 = no intersection (parallel planes)
///   2 = no intersection (plane passes circle) xp[0]=cp.pc proj -> pl
/// \endcode


  int   rc;
  Plane plci;
  Line  ln;


  // DEB_dump_obj__ (Typ_CI, ci, "UT3D_pt_intplci\n");
  // DEB_dump_obj__ (Typ_PLN, pl, "");
  //printf("UT3D_pt_intplci: intersection Plane with Circle\n");


  // Determine plane of circle
  rc = UT3D_pl_ptvc (&plci, &(ci->pc), &(ci->vz));


  // Intersect plane with plane of circle
  rc = UT3D_ln_int2pl (&ln, pl, &plci);


  // Intersect line with circle
  if (rc == 0) {
    rc = UT3D_pt_intlnci_p (np, xp, &ln, ci);
    if (rc == 1) rc = 2;
  } else {
    *np = 0;
    rc = 1;
  }



  // nur Testausg:
  // printf("ex UT3D_pt_intplci %d\n",rc);
  // if(*np > 0) printf("     %f %f %f\n",xp[0].x,xp[0].y,xp[0].z);
  // if(*np > 1) printf("     %f %f %f\n",xp[1].x,xp[1].y,xp[1].z);



  return rc;
}
*/


//======================================================================
  int UT3D_pt_intplnci (Point pa[],
                        Plane *pl1,
                        Circ *ci1, int ciLim) {
//======================================================================
// UT3D_pt_intplnci     Author: Thomas Backmeister         22.2.2003
/// \code
/// UT3D_pt_intplnci        intersect PLN 360-deg-Circ
/// 
/// IN:
///   Plane *pl1      Plane
///   Circ *ci        3D-circle
///   ciLim           0=limited Circ, 1=unlimited Circ
/// OUT:
///   Point pa[]      array of intersection points; size 2
/// Returncode:
///  -2 = no intersection (parallel planes)
///  -1 = no intersection (plane passes circle) pa[0]=cp.pc proj -> pl
///   1 = 1 intersectionpoint (circ touch plane)
///   2 = 2 intersectionpoints
///
/// was UT3D_pt_intplci
/// see also UT3D_pt_intcidpln
/// \endcode


  int   irc;
  Plane plci;
  Line  ln1;


  // DEB_dump_obj__ (Typ_PLN, pl1, "UT3D_pt_intplci");
  // DEB_dump_obj__ (Typ_CI, ci1, " ciLim=%d\n",ciLim);


  // Determine plane of circle
  irc = UT3D_pl_ptvc (&plci, &(ci1->pc), &(ci1->vz));

  // Intersect plane with plane of circle
  irc = UT3D_ln_int2pl (&ln1, pl1, &plci);
    // DEB_dump_obj__ (Typ_LN, &ln1, " LnPlnCi ");


  // Intersect line with circle
  if (irc == 0) {
    irc = UT3D_pt_intlnci_lp (pa, &ln1, UNLIMITED, ci1, ciLim);
  } else {
    irc = -2;
  }



  // nur Testausg:
  // printf("ex UT3D_pt_intplci %d\n",irc);
  // if(irc > 0) printf("     %f %f %f\n",pa[0].x,pa[0].y,pa[0].z);
  // if(irc > 1) printf("     %f %f %f\n",pa[1].x,pa[1].y,pa[1].z);



  return irc;

}


//=====================================================================
  int UT3D_pt_intcidpln (int *np, Point xp[],
                         Point *pc, Point *p1, Vector *vz, double dx) {
//=====================================================================
/// \code
/// UT3D_pt_intcidpln            intersect circ / perpend.offsetPlane
///                              unlimited circle only !
/// Ein Kreis (pc=Mittelpunkt, p1=Punkt am Kreis, vz=Achse)
/// dx ist der Abstand der Plane vom pc.
/// Eine Plane normal zur Kreisflaeche im Abstand von dx schneidet den Kreis.
///   Z-Vektor der Plane ist pc-p1; der vz ist der X-vec der Plane.
///   Origin der Plane ist ppo (Vom pc in Richtg p1 Abstand dx)
/// Output: die beiden Schnittpunkte mit der Plane.
///   np   Anzahl Schnittpunkte (0/1/2)
/// \endcode


  double  rdc, dy;
  Vector  vcx, vcy;

  rdc = UT3D_len_2pt (pc, p1);  // radius

  if(fabs(rdc -dx) < UT_TOL_pt) {
    *np = 1;
    xp[0] = *p1;
    goto L_fertig;
  }

  if(fabs(dx) > rdc) {
    *np = 0;
    goto L_fertig;
  }


  if(dx < UT_TOL_min1)  dy = rdc;
  else dy = sqrt (rdc * rdc - dx * dx);

  UT3D_vc_2pt (&vcx, pc, p1);
  UT3D_vc_perp2vc (&vcy, &vcx, vz);

  UT3D_pt_trapt2vc2len (&xp[0], pc, &vcx, dx, &vcy, dy);
  UT3D_pt_trapt2vc2len (&xp[1], pc, &vcx, dx, &vcy, -dy);

  *np = 2;

  L_fertig:
    // if(*np > 0) GR_tDyn_symB__ (&xp[0], SYM_TRI_S, 2);
    // if(*np > 1) GR_tDyn_symB__ (&xp[1], SYM_TRI_S, 2);
    // printf("ex UT3D_pt_intcidpln %d\n",*np);
  return 0;

}


//================================================================
  int UT3D_pt_intptvcsph (Point *pa, Point *pl, Vector *vl,
                          Point *ps, double rs) {
//================================================================
/// \code
/// UT3D_pt_intlnsph          intersect unlimited line / sphere
/// irc   -1   Error (vl too short)
/// irc    0   pl-vl outside sphere
/// irc    1   touches - OK - 1 point out
/// irc    2   OK, 2 points out. Richtung entspricht vl.
/// \endcode

  int    i1, irc;
  double d1, d2;
  Point  px;


  // proj ps -> pl-vl
  irc = UT3D_pt_projptptvc (&px, &d1, NULL, ps, pl, vl);
    // GR_tDyn_symB__ (&px, SYM_STAR_S, 2);
  if(irc < 0) goto L_fertig;


  // touch ?
  if(UTP_comp2db(d1,rs,UT_TOL_pt) == 1) {irc = 1; pa[0] = px; goto L_fertig;}


  // outside
  if(d1 > rs) {irc = 0; pa[0] = px; goto L_fertig;}


  UT2D_solvtriri_a (&d2, d1, rs);

  UT3D_pt_traptvclen (&pa[0], &px, vl, -d2);
  UT3D_pt_traptvclen (&pa[1], &px, vl, d2);

  irc = 2;


  L_fertig:
  // printf("ex UT3D_pt_intptvcsph %d\n",irc);
  // if(irc > 0) GR_tDyn_symB__ (&pa[0], SYM_STAR_S, 1);
  // if(irc > 1) GR_tDyn_symB__ (&pa[1], SYM_STAR_S, 1);
  return irc;

}


//======================================================================
  int UT3D_plcoe_pl (polcoeff_d3 *co, Plane *pl) {
//======================================================================
// UT3D_plcoe_pl              Author: Thomas Backmeister       10.4.2003
/// \code
/// UT3D_plcoe_pl              plane equation from plane
/// 
/// Computation of the coefficients of the plane equation to a plane,
/// i.e. a, b, c, d such that a*X + b*Y + c*Z = d;
/// 
/// IN:
///   Plane *pl      ... plane
/// OUT:
///   polcoeff_d3 *co ... coefficients of plane equation
/// Returncodes:
///   0 = OK
/// \endcode


  Vector vo;

  //printf("UT3D_plcoe_pl: plane equation <-- plane\n");

  // normal vector of plane
  co->a = pl->vz.dx;
  co->b = pl->vz.dy;
  co->c = pl->vz.dz;
  
  // vector to a point on the plane
  vo.dx = pl->po.x;
  vo.dy = pl->po.y;
  vo.dz = pl->po.z;

  // scalar product of normal vector and vo
  co->d = UT3D_acos_2vc (&(pl->vz), &vo);

  return 0;
}




//======================================================================
  int UT3D_pl_plcoe (Plane *pl, polcoeff_d3 *co) {
//======================================================================
// UT3D_pl_plcoe              Author: Thomas Backmeister        10.4.2003
/// \code
/// UT3D_pl_plcoe              plane <-- plane equation
/// 
///
/// Computation of the plane from the coefficients of the plane equation.
/// 
/// IN:
///   polcoeff_d3 *co ... coefficients of plane equation
/// OUT:
///   Plane *pl      ... plane
/// Returncodes:
///   0 = OK
///  -1 = input error
/// \endcode


  //printf("UT3D_pl_plcoe: plane <-- plane equation\n");

  // point on the plane
  if (!UTP_compdb0 (co->a, UT_TOL_min1)) {
    pl->po.x = co->d / co->a;
    pl->po.y = 0.0;
    pl->po.z = 0.0;
  }
  else if (!UTP_compdb0 (co->b, UT_TOL_min1)) {
    pl->po.x = 0.0;
    pl->po.y = co->d / co->b;
    pl->po.z = 0.0;
  }
  else if (!UTP_compdb0 (co->c, UT_TOL_min1)) {
    pl->po.x = 0.0;
    pl->po.y = 0.0;
    pl->po.z = co->d / co->c;
  }
  else {
    printf("Error UT3D_pl_plcoe: coefficients describe NO plane !\n");
    return -1;
  }

    // DEB_dump_obj__ (Typ_PT, &pl->po, "po ex plcoe:");


  // normal vector of plane
  pl->vz.dx = co->a;
  pl->vz.dy = co->b;
  pl->vz.dz = co->c;
    // DEB_dump_obj__ (Typ_VC, &pl->vz, "vz ex plcoe:");


  // x-vector of plane
  UT3D_vc_perp1vc (&(pl->vx), &(pl->vz));


  // y-vector of plane
  UT3D_vc_perp2vc (&(pl->vy), &(pl->vz), &(pl->vx));


  // p-value
  pl->p = pl->vz.dx * pl->po.x +
          pl->vz.dy * pl->po.y +
          pl->vz.dz * pl->po.z;

  // DEB_dump_obj__ (Typ_PLN, pl, "pln ex coe:\n");

  return 0;
}


//================================================================
  int UT3D_bp_perp_2pt (Point *pt1, Point *pt2) {
//================================================================
/// \code
/// UT3D_bp_perp_2pt         returns main-BackPlaneNr (0-2) from 2 points
/// Input:
///   pt1,pt2     a vector normal to the plane
/// Output:
///   reCode      the backplane for the plane normal to pt1,pt2; 
///
/// eg all points in XY-plane return 2 = BCKPLN_XY
/// see also UT3D_bp_pta UT3D_bp_perp_vc
/// \endcode

// Hauptebene mit dem hoechsten Wert waehlen; zb fuer UT3D_parpt_lnbp

  Vector    vc1;

  UT3D_vc_2pt (&vc1, pt1, pt2);
    // GR_tDyn_vc__ (&vc1, &p1, 2, 1);

  // backplane from vector
  return UT3D_bp_perp_vc (NULL, &vc1);

}
 

//================================================================
  int UT3D_bp_perp_vc (int *isParl, Vector *vc1) {
//================================================================
/// \code
/// UT3D_bp_vc_ck     get backplane from normal-vector
/// 
/// Input:
///   vc1      vector normal to backplane (eg z-vector of plane)
///   isParl   NULL  get backplane from longest vector; must not be parallel
///            else test if vector is absolute normal to backplane;
/// Output:
///   retcod:  2   vc1 = Z-vector (BCKPLN_XY)
///            1   vc1 = Y-vector (BCKPLN_XZ)
///            0   vc1 = X-vector (BCKPLN_YZ)
///   isParl   0   yes; vc1 is absolute normal to backplane;
///            1   no, vc1 is not exact normal to backplane;
///                retCod gives longest vector
///                  
/// \endcode

  int      bpi;
  double   dx, dy, dz;
  
  // DEB_dump_obj__(Typ_VC, vc1, "UT3D_bp_perp_vc:");

  
  // die Koordinate mit dem hoechsten Wert stillegen ..
  dx = fabs(vc1->dx);
  dy = fabs(vc1->dy);
  dz = fabs(vc1->dz);

  if(dy > dx) {             // Y > X
    if(dz > dy) bpi = BCKPLN_XY;     // skip Z-Coord
    else        bpi = BCKPLN_XZ;     // skip Y-Coord 
  } else {                  // X > Y
    if(dz > dx) bpi = BCKPLN_XY;     // skip Z-Coord
    else        bpi = BCKPLN_YZ;     // skip X-Coord
  }


  if(!isParl) goto L_exit;


  if(bpi == BCKPLN_XY) {
    // Z
    if((dx * 1024.) > dz) goto L_not;
    if((dy * 1024.) > dz) goto L_not;
  } else if(bpi == BCKPLN_XZ) {
    // Y
    if((dx * 1024.) > dy) goto L_not;
    if((dz * 1024.) > dy) goto L_not;
  } else {
    // X
    if((dy * 1024.) > dx) goto L_not;
    if((dz * 1024.) > dx) goto L_not;
  }

  *isParl = 0;

  L_exit:
    // printf("ex UT3D_bp_perp_vc bpi=%d isParl=%d %f %f %f\n",bpi,*isParl,dx,dy,dz);
  return bpi;


  L_not:
  *isParl = 1;
  goto L_exit;

}


//================================================================
  int UT3D_bp_ln (double *bpd, Line *oLn) {
//================================================================
// UT3D_bp_ln                backplane from line
// Output:
//   bpd         offset from backplane
//   retCode     2=BCKPLN_XY; 1=BCKPLN_XZ; 0=BCKPLN_YZ; -1=free-plane 
//
// see also UT3D_bp_perp_vc

  int   irc;

  if(UTP_comp2db(oLn->p1.z, oLn->p2.z, UT_TOL_pt)) {
    irc = BCKPLN_XY;  // parall. X-Y   2
    *bpd = oLn->p1.z;

  } else if (UTP_comp2db(oLn->p1.y, oLn->p2.y, UT_TOL_pt)) {
    irc = BCKPLN_XZ;  // parall. X-Z   1
    *bpd = oLn->p1.y;

  } else if (UTP_comp2db(oLn->p1.x, oLn->p2.x, UT_TOL_pt)) {
    irc = BCKPLN_YZ;  // parall. Y-Z   0
    *bpd = oLn->p1.x;

  } else irc = BCKPLN_FREE;    // -1

    // printf("ex-UT3D_bp_ln %d %f \n",irc,*bpd);

  return irc;

}


//================================================================
  int UT3D_bp_pta (int pNr, Point *pa) {
//================================================================
/// \code
/// backplane from n points
/// see UT3D_bp_perp_vc
/// \endcode
// TODO: use UT3D_bp_box

  int     bp;
  Point   p1, p2;
  double  dx,dy,dz;


  // box from n points
  UT3D_box_pts (&p1, &p2, pNr, pa);

    dx = fabs(p2.x - p1.x);
    dy = fabs(p2.y - p1.y);
    dz = fabs(p2.z - p1.z);
      // printf(" _bp_pta-1 %f %f %f\n",dx,dy,dz);

   if(dx < dy) {
     if(dz < dx) bp = BCKPLN_XY;
     else        bp = BCKPLN_YZ;
   } else {
     if(dz < dy) bp = BCKPLN_XY;
     else        bp = BCKPLN_XZ;
   } 

    // printf("ex-UT3D_bp_pta %d\n",bp);

  return bp;
 
}


//=========================================================================
  int UT3D_par_ck_inObj__ (double ux, Point *px,
                           Point *ps, Point *pe,
                           double us, double ue) {
//=========================================================================
/// \code
/// check if parameter is inside range us-ue & check limitpoints
/// ux       parameter of point to check
/// px       point to check
/// ps, pe   limiting points
/// us,ue    parametric range of unlimited obj;
/// RetCod:  0  NO, par1 is outside parameter-range us-ue
///          1  YES, par1 is inside parameter-range us-ue
/// \endcode

  // test if points coincide; then test if par1 inside 0-1


  // printf("UT3D_par_ck_inObj__ ux=%lf us=%lf ue=%lf\n",ux,us,ue);


  if(us < ue) {
    // curve fwd
    if(ux < us) {
      // test ps px
      if(UT3D_comp2pt (px, ps, UT_TOL_pt)) return 1;
  
    } else if(ux > ue) {
      // test pe px
      if(UT3D_comp2pt (px, pe, UT_TOL_pt)) return 1;
  
    } else {
      // inside, ok
      return 1;
    }

  } else {
    // curve bwd
    if(ux > us) {
      // test ps px
      if(UT3D_comp2pt (px, ps, UT_TOL_pt)) return 1;

    } else if(ux < ue) {
      // test pe px
      if(UT3D_comp2pt (px, pe, UT_TOL_pt)) return 1;

    } else {
      // inside, ok
      return 1;
    }
  }



  return 0;  // va (and its point) is outside range

}


//=========================================================================
  int UT3D_par_ck_inObj_del (int *np, Point *pa, double *va,
                             Point *ps, Point *pe,
                             double us, double ue) {
//=========================================================================
/// \code
/// delete all points & parameters not on obj  (between limits us-ue)
/// Input:
///   np      nr of parameters, points
///   pa      np points
///   va      np parameters
///   ps,pe   startpoint, endpoint of obj
///   us,ue   startparameter, endparameter of curve
/// Output:
///   np      reduced nr of parameters, points
///   pa      delete all points outside range us-ue
///   va      delete all parameters outside range us-ue
///
/// test if points coincide; then test if par1 inside 0-1
/// \endcode


  int    i1;

  // printf("UT3D_par_ck_inObj_del %d us=%lf ue=%lf\n",*np,us,ue);
  // for(i1=0; i1 < *np; ++i1) printf("  _ck_inObj_del %d %lf\n",i1,va[i1]);


  L__1:

  for(i1=0; i1 < *np; ++i1) {

    // ck if par is inside obj
    // if(UT3D_par1_ck_inObj__ (&va[i1], &pa[i1], ps, pe)) continue;
    if(UT3D_par_ck_inObj__(va[i1], &pa[i1], ps, pe, us, ue)) continue;
      // printf(" foll.par_ck_inObj__ - delete i1=%d\n",i1);


    // not on limited curve; delete this point
    MEM_del_nrec (np, pa, i1, 1, sizeof(Point));
    *np += 1;
    MEM_del_nrec (np, va, i1, 1,sizeof(double));
      // printf(" _par_ck_inObj_del delete %d ****\n",i1);
    goto L__1;
  }


  // TESTBLOCK
  // for(i1=0; i1 < *np; ++i1) printf("ex _ck_inObj_del %d %lf\n",i1,va[i1]);
  // printf("ex UT3D_par_ck_inObj_del %d\n",*np);
  // END TESTBLOCK


  return 0;

}


//=========================================================================
  int UT3D_parvc_2vc (double *pl, Vector *v1, Vector *v2) {
//=========================================================================
/// \code
/// UT3D_parvc_2vc    parameter of distance of vec1 projected on vec2
/// retCod:   0=Ok  -1=v1-v2 normal, pl=0.
///
/// v1={0.5, 0.5, 0.}, v2={1., 0., 0.} gives pl = 0.5
/// v1={-0.5, 0.5, 0.}, v2={1., 0., 0.} gives pl = -0.5
/// v1={1.5, 0.5, 0.}, v2={1., 0., 0.} gives pl = 1.5
///
///
///                             V1,V2 have same startpoint s.
///                 x
///                /|
///               / |
///             V1  |
///             /   |
///            /   .|
///           s-----x---V2------->
///     < 0   0     pl=0.4       1         > 1
///
/// \endcode


  double  Det, dd;


  Det = v2->dx * v2->dx + v2->dy * v2->dy + v2->dz * v2->dz;

  if(Det == 0.0) {
    // printf("UT3D_parvc_2vc - No solution!\n");
    *pl = 0.;
    return -1;
  }


  // Skalarprodukt
  dd  = v1->dx * v2->dx + v1->dy * v2->dy + v1->dz * v2->dz;

  *pl = dd/Det;

    // printf("ex UT3D_parvc_2vc pl=%f Det=%f dd=%f\n",*pl,Det,dd);

  return 0;

}


//===========================================================================
  int UT3D_parvc_2vcbp (double *dl, int mainPln, Vector *v1, Vector *v2) {
//===========================================================================
/// \code
/// UT3D_parvc_2vcbp    parameter of distance of vec1 projected on vec2
///
/// mainPln: Berechnungsebene BackPlane ex UT3D_bp_perp_vc.
///
/// dl ist zwischen 0 bis 1; wenn dl=0.5 liegt der Endpunkt von V2
/// genau in der Mitte des Vektors V2.
///
///    
///                             V1,V2 have same startpoint s.
///                 x
///                /| 
///               / |
///             V1  |
///             /   |
///            /    |
///           s---------V2-------x
///                 dl 
/// \endcode

  // double  vl;
  double  Det, dd;
  // Vector2 v3;


  // printf("v1= %f %f %f\n", v1->dx, v1->dy, v1->dz);
  // printf("v2= %f %f %f\n", v2->dx, v2->dy, v2->dz);


  // als v3 eine Normale auf v2 annehmen, dann wie UT3D_2parvc_3vcbp


  if(mainPln == 0) {            // Y-Z-Plane, skip X   (x=y, y=z)
    Det = v2->dy * v2->dy + v2->dz * v2->dz;
    dd  = v1->dy * v2->dy + v2->dz * v1->dz;

  } else if(mainPln == 1) {     // X-Z-Plane, skip Y   (x=x, y=z)
    Det = v2->dx * v2->dx + v2->dz * v2->dz;
    dd  = v1->dx * v2->dx + v2->dz * v1->dz;

  } else if(mainPln == 2) {     // X-Y-Plane, skip Z   (x=x, y=y)
    // v3.dx =  -(v2->dy);
    // v3.dy =    v2->dx;
    // Det = v2->dx * v3.dy  - v3.dx * v2->dy;
    // dd  = v1->dx * v3.dy  - v3.dx * v1->dy;
    Det = v2->dx * v2->dx + v2->dy * v2->dy;
    dd  = v1->dx * v2->dx + v2->dy * v1->dy;
  }


  if (Det != 0.0) {
    *dl = dd/Det;
    // printf("ex UT3D_parvc_2vcbp dl=%f pl=%d Det=%f dd=%f\n",*dl,mainPln,Det,dd);
  }

  else {
   printf("UT3D_parvc_2vcbp - No solution!\n");
   // exit(0);
   return -1;
  }




  return 0;

}


//===================================================================
  int UT3D_2par_vc_vcx_vcy (double *dx, double *dy,
                            Vector *vc1, Vector *vcx, Vector *vcy) {
//===================================================================
/// \code
/// UT3D_2par_vc_vcx_vcy    get parameters of vector projected onto vx,vy
/// vcx and vcy must have length=1
/// 
///   vcy
///    ^             
///    |
///    |---dx---|
///    |        
///    |        x   ---
///    |       /     |
///    |   vc1/      |
///    |    /        dy
///    |   /         |
///    | /           |
///    x---------------------> vcx
///
/// retour: UT3D_pt_tra_pt_2vc_2par
/// \endcode

// see also UT3D_nlen_projvcvc UT3D_nlen_projvcvc


  // DEB_dump_obj__(Typ_VC, vc1, "UT3D_2par_vc_vcx_vcy vc1:");
  // DEB_dump_obj__(Typ_VC, vcx, "  vcx:");
  // DEB_dump_obj__(Typ_VC, vcy, "  vcy:");


    // TEST ONLY:
    // angle between vcx vcy
    // printf(" angr=%lf\n",UT3D_angr_2vc__ (vcx,vcy));




  *dx = (vcx->dx * vc1->dx + vcx->dy * vc1->dy + vcx->dz * vc1->dz);
  *dy = (vcy->dx * vc1->dx + vcy->dy * vc1->dy + vcy->dz * vc1->dz);

    // printf("ex UT3D_2par_vc_vcx_vcy %lf %lf\n",*dx,*dy);

  return 0;

}


//===========================================================================
  int UT3D_2parvc_3vcbp (double *dx, double *dy,
                       int mainPln, Vector *v1, Vector *v2, Vector *v3) {
//===========================================================================
/// \code
/// UT2D_2parvc_3vc project end of vec1 along vec2 on vec3
///
/// mainPln: Berechnungsebene BackPlane ex UT3D_bp_perp_vc.
///
/// Zu gegebenen Vektoren v1, v2, v3 finden wir Zahlen x und y so,
/// dass dx * v1 + dy * v3 = v2.
///
/// dx/dy sind zwischen 0 bis 1; wenn dx=0.5 liegt der Endpunkt von V2
/// genau in der Mitte des Parallelogramss V1-V3.
///
///   y
///    \                        V1,V2,V3 have same startpoint s.
///  dy \-----------x
///      \         / \
///       \       /   \
///     V3 \    V2     \
///         \   /       \
///          \ /         \
///           s----------------------------x
///                      dx          V1
/// \endcode


  double Det, Dx, Dy;


  // printf("v1= %f %f %f\n", v1->dx, v1->dy, v1->dz);
  // printf("v2= %f %f %f\n", v2->dx, v2->dy, v2->dz);
  // printf("v3= %f %f %f\n", v3->dx, v3->dy, v3->dz);


  if(mainPln == 0) {            // Y-Z-Plane, skip X   (x=y, y=z)
    Det = v1->dy * v3->dz - v3->dy * v1->dz;
    Dx  = v2->dy * v3->dz - v3->dy * v2->dz;
    Dy  = v1->dy * v2->dz - v2->dy * v1->dz;

  } else if(mainPln == 1) {     // X-Z-Plane, skip Y   (x=x, y=z)
    Det = v1->dx * v3->dz - v3->dx * v1->dz;
    Dx  = v2->dx * v3->dz - v3->dx * v2->dz;
    Dy  = v1->dx * v2->dz - v2->dx * v1->dz;

  } else if(mainPln == 2) {     // X-Y-Plane, skip Z   (x=x, y=y)
    Det = v1->dx * v3->dy - v3->dx * v1->dy;
    Dx  = v2->dx * v3->dy - v3->dx * v2->dy;
    Dy  = v1->dx * v2->dy - v2->dx * v1->dy;
  }


  if (Det != 0.0) {
    *dx = Dx/Det;
    *dy = Dy/Det;
     // printf("ex UT3D_2parvc_3vcbp x=%f y=%f\n", *dx, *dy);
  }

  else {
    // wenn v1=0,0,0: dx=0,     dy=v3/v2.
    // wenn v2=0,0,0: dx=0,     dy=0.
    // wenn v2=0,0,0: dx=v1/v2, dy=0.
    // printf("UT3D_2parvc_3vcbp - No solution! bp=%d\n",mainPln);
      *dx = 0.5;
      *dy = 0.5;
    if(UT3D_compvc0 (v1, UT_TOL_pt) == 1)  {
      *dx = UT3D_len_vc(v3) / UT3D_len_vc(v2);  //verkehrt; f.UT3D_parsbsp_pt!

    } else if(UT3D_compvc0 (v3, UT_TOL_pt) == 1)  {
      *dy = UT3D_len_vc(v1) / UT3D_len_vc(v2);

    }
    // TX_Error("UT3D_2parvc_3vcbp E001");
    // printf("  v1= %f %f %f\n", v1->dx, v1->dy, v1->dz);
    // printf("  v2= %f %f %f\n", v2->dx, v2->dy, v2->dz);
    // printf("  v3= %f %f %f\n", v3->dx, v3->dy, v3->dz);
    // printf("  dx=%f dy=%f\n",*dx,*dy);
    // exit(0);
    return -1;
  }

  return 0;

}


//==============================================================================
  int UT3D_par_pt__pt_prj_ln (double *parP, Point *ptl, Point *ptx, Line *ln1) {
//==============================================================================
/// UT3D_par_pt__pt_prj_ln    parameter and point of point projected onto line
/// retcode    0=OK; -1=degenerated-line


  int      irc;
  Vector   v1, v2;

  // DEB_dump_obj__ (Typ_LN, ln1, "UT3D_parpt_ln__");
  // DEB_dump_obj__ (Typ_PT, ptx, "   ptx");


  UT3D_vc_2pt (&v1, &ln1->p1, ptx);
  UT3D_vc_2pt (&v2, &ln1->p1, &ln1->p2);

  irc = UT3D_parvc_2vc (parP, &v1, &v2);

  if(ptl) {
    UT3D_pt_traptmultvc (ptl, &ln1->p1, &v2, *parP);
  }

    // printf("ex-UT3D_par_pt__pt_prj_ln par=%f",*parP);
    // if(ptl) printf(" ptl %f %f %f",ptl->x,ptl->y,ptl->z);
    // printf("\n");

  return irc;

}


//=========================================================================
  int UT3D_par_pt__pt_prj_plg (double *pao, Point *pto, int vTyp, int mode,
                               Point *ptx, CurvPoly *plg, double tol) {
//=========================================================================
// UT3D_par_pt__pt_prj_plg    get parameter / point of nearest point on polygon
// Input:
//   ptx        point
//   plg        polygon, Typ_CVPOL
//   vTyp       0 get knotvalues for curve, else 0-1. See INF_struct_par.
//              1 get values from 0-1 for curve. See INF_struct_par.
//   mode       0   check if dist. ptx-curve > tol; if yes - return -1
//              1   do not test distance - project onto curve (test only endpoints)
//              2   point is on curve; get only parameter du
//   tol        max dist for pti from curve
// Output:
//   pao        parameter of point pti on curve plg; can be NULL on input
//   pto        point projected onto curve; can be NULL on input
//   retCod     0  ptx is on curve between start/endpoint
//              1  ptx = startpoint curve
//              2  ptx = endpoint curve
//              3  ptx = cornerpoint
//             -1  ptx = outside curve; distance > tol
//
// see also UT3D_par_pt__pt_prj_cv UT3D_pt_ck_onLine

// TODO: set retcode 3 if point is near definitionPoint

  int      irc, i1, ie;
  Point    pb1, pb2, *p1, *p2; //, px;
  double   dx, dd, dMax;
  Vector   vl, vp, vx;



  // printf("---------------------------------- \n");
  // printf("UT3D_par_pt__pt_prj_plg tol=%f vTyp=%d mode=%d\n",tol,vTyp,mode);
  // DEB_dump_obj__ (Typ_PT, ptx, "  ptx");
  // DEB_dump_obj__ (Typ_CVPOL, plg, "  plg");


  if(mode) dMax = UT_VAL_MAX;
  else     dMax = tol;

  ie = plg->ptNr - 1;
  irc = -1;


  // check if point is near lineStartPoint
  p1 = &plg->cpTab[0];
  if(UT3D_comp2pt (p1, ptx, tol) == 1) {
    if(pto) *pto = *p1;
    if(pao) *pao = plg->lvTab[0];
    irc = 1;
    goto L_exit;
  }

  // check if point is near lineEndPoint
  p2 = &plg->cpTab[ie];
  if(UT3D_comp2pt (p2, ptx, tol) == 1) {
    if(pto) *pto = *p2;
    if(pao) *pao = plg->lvTab[ie];
    irc = 2;
    goto L_exit;
  }


  // loop tru points
  for (i1=0; i1<ie; ++i1) {                    // see UT3D_pt_ck_onLine
    // get linesegment p1-p2
    p1 = &plg->cpTab[i1];
    p2 = &plg->cpTab[i1 + 1];
      // printf("----- nxt p1=%d p2=%d\n",i1, i1+1);


    if(!mode) {
      // mode=0: check also distance
      // check if point is in box oder outside box with tol ..
      UT3D_box_2pt__ (&pb1, &pb2, p1, p2);
      if(UT3D_pt_ck_inBoxTol (&pb1, &pb2, ptx, tol) == 1) continue;  // outside
    }

    // check if point is near lineEndPoint (startpoint already tested)
    if(UT3D_comp2pt (p2, ptx, tol) == 1) {
      if(pto) *pto = *p2;
      if(pao) *pao = plg->lvTab[i1 + 1];
      irc = 3;
      goto L_exit;
    }

    // dx = Faktor length orth.proj.point ptx onto p1-p2
    UT3D_vc_2pt (&vl, p1, p2);
    UT3D_vc_2pt (&vp, p1, ptx);
    UT3D_parvc_2vc (&dx, &vp, &vl);
      // printf(" dx=%f\n",dx);

    // skip outside
    if(dx < 0.) continue;
    if(dx > 1.) continue;

    // get vx = vector from p1 to orth.proj.point 
    UT3D_vc_multvc (&vx, &vl, dx);
      // DEB_dump_obj__ (Typ_VC, &vx, "    vx ");

    // vector orth.proj.point -> ptx = subtract vx from vp
    UT3D_vc_sub2vc (&vp, &vp, &vx);
      // DEB_dump_obj__ (Typ_VC, &vp, "    vp ");

    // distance orth.proj.point - ptx
    dd = UT3D_len_vc (&vp);
      // printf(" dd=%f\n",dd);

    if(!mode) {
      // mode=0: check also distance
      if(dd > tol) continue;
    }

    if(dd < dMax) {
        // printf(" pt %d is nearer ..\n",i1);
      // point is nearer ..
      if(pto) UT3D_pt_traptvc (pto, p1, &vx);
      // if(pao) *pao = plg->lvTab[i1] + UT3D_len_vc (&vx);
      if(pao) *pao = plg->lvTab[i1] + ((plg->lvTab[i1+1] - plg->lvTab[i1]) * dx);
      dMax = dd;
      irc = 0;
    }

  }


  //----------------------------------------------------------------
  L_exit:

  if(pao) {
    if(vTyp)
      // get normalized-parameter from knot.value
      *pao = UT3D_par1_parplg (pao, plg);
      // printf("   irc=%d par1=%f park=%f\n",irc,par1,park);
  }

    // TESTBLOCK
    // printf("ex-UT3D_par_pt__pt_prj_plg irc=%d\n",irc);
    // if(pto) DEB_dump_obj__ (Typ_PT, pto, "    pto ");
    // if(pto) GR_tDyn_pt (pto, ATT_PT_YELLOW);
    // if(pao) printf("    pao=%f\n",*pao);
    // END TESTBLOCK

  return irc;

}


//=============================================================================
  int UT3D_par_pt__pt_prj_cv (double *pao, Point *pto, int vTyp, int mode,
                                Point *pti, int oTyp, void *oDat, double tol) {
//=============================================================================
// UT3D_par_pt__pt_prj_cv    get parameter and point of point projected onto curve
// Input:
//   oTyp,oDat  bin.obj of curve
//   pti        point
//   vTyp       0 get knotvalues for spline/polygon, else 0-1. See INF_struct_par.
//              1 get values from 0-1 for all types of curves. See INF_struct_par.
//   mode       0   check if dist. ptx-curve > tol; if yes - return -1
//              1   do not test distance - project onto curve (test only endpoints)
//              2   point is on curve; get only parameter du
//   tol        max dist for pti from curve
// Output:
//   pao        parameter of point pti on curve oTyp,oDat; can be NULL
//   pto        point projected onto curve; can be NULL
//   retCod     0  pti is on curve between start/endpoint
//              1  pti = startpoint curve
//              2  pti = endpoint curve
//             -1  pti = outside curve; distance > tol
//
// see also UTO_par__pt_obj


  int    irc=0, i1, ibp, ipt, sTyp;
  long   sDbi;
  double par1, park, dst, d1, d2;
  void   *sDat;
  Point  ptx;
  CurvCCV  *cvtrm;


  // printf("---------------------------------- \n");
  // printf("UT3D_par_pt__pt_prj_cv oTyp=%d tol=%f vTyp=%d mode=%d\n",
         // oTyp,tol,vTyp,mode);
  // DEB_dump_obj__ (Typ_PT, pti, "  _par_pt__pt_prj_cv-pti");
  // DEB_dump_obj__ (oTyp, oDat, "  _par_pt__pt_prj_cv-oDat");


  // if(oTyp == Typ_Val) {
    // *po = *((double*)oDat);
  // } else

  L_start:

  //----------------------------------------------------------------
  if(oTyp == Typ_LN) {
    // get parameter and point on curve
    UT3D_par_pt__pt_prj_ln (&par1, &ptx, pti, oDat);
    // get dist point - point on curve
    dst = UT3D_len_2pt (&ptx, pti);
    // set retcode
    if(!mode) {
      if(dst > tol) irc = -1;
    } else {
      if(par1 < 0.5) {
        if(UT3D_comp2pt(&ptx, &((Line*)oDat)->p1, tol)) irc = 1;
      } else {
        if(UT3D_comp2pt(&ptx, &((Line*)oDat)->p2, tol)) irc = 2;
      }
    }
    park = par1;


  //----------------------------------------------------------------
  } else if(oTyp == Typ_CI) {
    // get parameter, point
    irc = UT3D_par_pt__pt_prj_ci (&par1, &ptx, NULL, (Circ*)oDat, pti, mode, tol);
    park = par1;


  //----------------------------------------------------------------
  } else if(oTyp == Typ_CVELL) {
    // get parameter and point
    irc = UT3D_par_pt__pt_el (&par1, &ptx, (CurvElli*)oDat, pti, mode, tol);
    park = par1;


  //----------------------------------------------------------------
  } else if(oTyp == Typ_CVCLOT) {
    // irc = UT3D_par_clotpt (&par, pti, (CurvClot*)oDat, UT_DISP_cv);
    // get parameter and point
    i1 = 1;
    irc = UT3D_pt_prjclotpt (&i1, &pti, &par1, pti, (CurvClot*)oDat, tol);
    park = par1;


  //----------------------------------------------------------------
  } else if(oTyp == Typ_CVBSP) {
    // UME_init (&wrkSpc, tmpSpc, 50000);
    // get par, point, dist
    irc = UT3D_par_pt__pt_cvbsp (&park, &ptx, &dst, (CurvBSpl*)oDat, pti);
      // printf(" f-pt_cvbsp irc=%d par=%f dst=%f\n",irc,park,dst);
    if(irc != 0) return -1;
    // set retcode
    if(!mode) {
      if(dst > tol) {
        irc = -1;
      }
    } else {
      if(UT3D_comp2pt(&ptx, &((Line*)oDat)->p1, tol)) irc = 1;
      if(UT3D_comp2pt(&ptx, &((Line*)oDat)->p2, tol)) irc = 2;
    }
    // get par1 from park
    if(vTyp) {
      // get normalized-parameter from knot.value
      par1 = UT3D_par1_parbsp (&park, oDat);
        // printf("  f-par1_parbsp par1=%f park=%f\n",par1,park);
    }


  //----------------------------------------------------------------
  } else if(oTyp == Typ_CVRBSP) {
    // get only parameter
    irc = UT3D_pt_vc_par__pt_rbsp (pto, NULL, pao,
                                   (CurvRBSpl*)oDat, pti, 1, vTyp, tol);
    if(irc != 0) return -1;
    goto L_done;


  //----------------------------------------------------------------
  } else if(oTyp == Typ_CVPOL) {
    // get nearest parameter / point on polygon
    irc = UT3D_par_pt__pt_prj_plg (pao, pto, vTyp, mode, pti, oDat, tol);
    if(irc > 2) irc = 0;   // 3 = cornerpoint
    goto L_done;


  //----------------------------------------------------------------
  } else if(oTyp == Typ_CVTRM) {

    // get basic-curve of vp2
    irc = CVTRM_basCv_trmCv (&sTyp, &sDbi, &sDat, oDat);
    if(irc < 0) {TX_Error ("UT3D_par_pt__pt_prj_cv E2"); return -1; }
      // DEB_dump_obj__ (sTyp, sDat, " ck_is_on_crv-sTyp");
    // recursion
    irc = UT3D_par_pt__pt_prj_cv (pao, pto, vTyp, mode, pti, sTyp, sDat, tol);
// TODO: set parameter for trimmed-curve
    goto L_done;


  //----------------------------------------------------------------
  } else {
    TX_Error("UT3D_par_pt__pt_prj_cv E001 %d",oTyp);
    return -1;
  }


  //----------------------------------------------------------------
  L_exit:
  if(pto) *pto = ptx;
  if(pao) {
    if(vTyp) *pao = par1;
    else     *pao = park;
  }


  //----------------------------------------------------------------
  L_done:

    // TESTBLOCK
    // printf("ex-UT3D_par_pt__pt_prj_cv irc=%d typ=%d dst=%f\n",irc,oTyp,dst);
    // if(pao) printf("  par %f",*pao);
    // if(pto) printf("  pto %f %f %f",pto->x,pto->y,pto->z);
    // printf("\n");
    // if(pto) GR_tDyn_symB__ (pto, SYM_TRI_S, ATT_COL_RED);
    // END TESTBLOCK


  return irc;


  //----------------------------------------------------------------
  L_out:
    irc = -1;
    goto L_exit;

  L_NYI:
    return MSG_ERR__ (ERR_func_not_impl, "typ=%d",oTyp);

}


//=============================================================================
  int UT3D_par_pt__pt_prj_ci (double *par, Point *pto, double *dpc,
                              Circ *ci1, Point *pt1, int mode, double tol) {
//=============================================================================
// \code
// UT3D_par_pt__pt_prj_ci   get parameter and point of point projected on circ
// Input:
//   mode     0   check if dist. ptx-curve > tol; if yes - return -1
//            1   do not test distance - project onto curve (test only endpoints)
//            2   point is on curve; get only parameter du
//   tol      max dist from curve
// Output:
//   par      parameter 0-1 of pt1 projected onto ci1; can be NULL on input;
//   pto      pt1 projected onto ci1; can be NULL on input
//   dpc      distance pt1-pto; can be NULL on input
//   retCod   0  OK, par is inside ci1
//            1  parameter = startpoint (ci1->p2)
//            2  parameter = endpoint (ci1->p2)
//           -1  pt1 is outside ci1
//
// par = total_length / length_p1_pti
// length = radius * angle
//
// see UT3D_angr_ci_p1_pt UT3D_par1_ci_angr
// \endcode

  int      irc, i1, i360;
  double   a1, d1, par1, oaTot, dst;
  Vector   vc1;


  // printf("........................................ \n");
  // printf("UT3D_par_pt__pt_prj_ci  tol=%f \n",tol);
  // DEB_dump_obj__ (Typ_CI, ci1, "  ci1");
  // DEB_dump_obj__ (Typ_PT, pt1, "  pt1");
  // GR_tDyn_symB__ (pt1, SYM_STAR_S, ATT_COL_CYAN);

  // total opening angle
  oaTot = fabs(ci1->ango);

  // weiter hinten division / oaTot !
  if(oaTot < UT_TOL_min2) {TX_Error("UT3D_par_pt__pt_prj_ci E01");return 0.;}

  // get opening-angle of p1 on circ o1
  a1 = UT3D_angr_ci_p1_pt (ci1, pt1);
    // printf(" _par1_ci_pt-a1=%lf\n",a1);

  // 360-deg-circ ?  UT3D_ck_ci360
  i360 = UTP_comp2db (fabs(ci1->ango), RAD_360, UT_TOL_Ang1); // 0=no, 1=yes
    // printf(" _par1_ci_pt-i360=%d\n",i360);

  // test if a1 is ouside curve
  if(!i360) {
    // trimmed-circle;
    // test if angle a1 is between / near values 0-oaTot
    i1 = UTP_db_ck_in2db_ang1 (a1, 0., oaTot, DLIM01(ci1->rad));
      // printf(" f-ck_in2db_ang1=%d\n",i1);
    if(i1) {
      if(abs(i1) == 1) {
        // inside, near p1
        if(UT3D_comp2pt(pt1, &ci1->p1, tol)) {
          if(par) *par = 0.;
          if(dpc) *dpc = 0.;
          if(pto) *pto = ci1->p1;
          irc = 1;
          goto L_exit;
        }
      } else if(abs(i1) == 2) {
        // inside, near p2
        if(UT3D_comp2pt(pt1, &ci1->p2, tol)) {
          if(par) *par = 1.;
          if(dpc) *dpc = 0.;
          if(pto) *pto = ci1->p2;
          irc = 2;
          goto L_exit;
        }
      }
      if(i1 < 0) return -1;
    }

  } else {
    // 360-deg-circ
    if(UT3D_comp2pt(pt1, &ci1->p2, tol)) {
      if(par) *par = 1.;
      if(dpc) *dpc = 0.;
      if(pto) *pto = ci1->p2;
      irc = 2;
      goto L_exit;
    }
  }

  // get distance pt1 - curve
  d1 = UT3D_len_2pt (pt1, &ci1->pc);
  dst = d1 - fabs(ci1->rad);
    // printf(" pt_prj_ci-d1=%f dst=%f\n",d1,dst);
  if(!mode) {
    // 0 = test distance
    if(fabs(dst) > tol) return -1;
  }

  // get par1 of a1 (UT3D_par1_ci_angr)
  if(par) *par = fabs(a1 / oaTot);
  if(dpc) *dpc = dst;
  if(pto) {
    // pto = project onto circle
    UT3D_vc_2pt (&vc1, &ci1->pc, pt1);
    UT3D_vc_multvc (&vc1, &vc1, fabs(ci1->rad) / d1);
    UT3D_pt_traptvc (pto, &ci1->pc, &vc1);
  }

  irc = 0;


  //----------------------------------------------------------------
  L_exit:
    // TESTBLOCK
    // printf("ex-UT3D_par_pt__pt_prj_ci ");
    // if(par) printf(" par=%f",*par);
    // if(dpc) printf(" dpc=%f",*dpc);
    // printf(" \n");
    // END TESTBLOCK

  return irc;

}


/* replaced by UT3D_par_pt__pt_prj_ci
//================================================================
  double UT3D_par1_ci_pt (Circ *ci1, double *dpc, Point *pt1) {
//================================================================
/// \code
/// UT3D_par1_ci_pt          get parameter (0-1) for point on circ
///  RetCod=0 gives the startpoint of the circ (ci1->p1);
///         1 gives the endpoint (ci1->p2)
///
/// par = total_length / length_p1_pti
/// length = radius * angle
///
/// see UT3D_angr_ci_p1_pt UT3D_par1_ci_angr
/// \endcode

  double   a1, par1, oaTot;


  // DEB_dump_obj__ (Typ_CI, ci1, "UT3D_par1_ci_pt");
  // DEB_dump_obj__ (Typ_PT, pt1, "UT3D_par1_ci_pt");
  // GR_tDyn_symB__ (pt1, SYM_STAR_S, 2);

  oaTot = fabs(ci1->ango);

  // weiter hinten division / oaTot !
  if(oaTot < UT_TOL_min2) {TX_Error("UT3D_par1_ci_pt E001");return 0.;}

  // get opening-angle of p1 on circ o1
  a1 = UT3D_angr_ci_p1_pt (ci1, pt1);
    // printf(" _par1_ci_pt-a1=%lf\n",a1);

  // get par1 of a1 (UT3D_par1_ci_angr)
  par1 = fabs(a1 / oaTot); 


  if(dpc) {
    // get distance pt1 - curve
    *dpc = UT3D_len_2pt (pt1, &ci1->pc) - fabs(ci1->rad);
       printf(" _par1_ci_pt-dpc = %f\n",*dpc);
  }

    // TEST ONLY:
    // printf("ex par1_ci_pt %lf\n",par1);
    // UT3D_pt_vc__par_ci (&pt1, par1, ci1); // get point of par1
    // GR_tDyn_symB__ (&pt1, SYM_TRI_S, 3);

/
//   // Gesamter Oeffnungswinkel oaTot
//   // oaTot = UT3D_angr_ci_p1_pt (ci1, &ci1->p2);
//   oaTot = ci1->ango;
//   // TeilOeffnungswinkel oap
//   oap = UT3D_angr_ci_p1_pt (ci1, pti);
//   // an den naeheren Circ-Endpunkt positionieren (Toleranzproblem)
//   // deltaAngle zu CircStart:
//   d1 = RAD_360 - oap;
//   // deltaAngle zu CircEnd:
//   d2 = oap - oaTot;
//   if(d1 < d2) oap -= RAD_360;
//   parP = oap / oaTot;
/

  return (par1);

}
*/

//======================================================================
  int UT3D_pt_ln_lim_del (int *nrp, Point *pa, double *va1,
                          Point *pl1, Point *pl2) {
//======================================================================
/// \code
/// UT3D_pt_ln_lim_del       remove points outside limited line
/// Input:
///   nrp    nr of points on unlimited line
///   pa     points
///   va1    size nrp, can be empty or NULL
/// Output:
///   nrp    nr of points between pl1-pl2
///   va1    parameter on points;
/// 
/// see UT3D_ck_npt_inCirc UT3D_pt_ell_lim_del
/// \endcode


  int     i1;

  if(*nrp < 1) return 0;

  if(!va1) {
    // get local space
    va1 = MEM_alloc_tmp ((int)(sizeof(double) * *nrp));
  }

  for(i1=0; i1 < *nrp; ++i1) {
    // get parameter of point on line
    UT3D_par_pt_2pt (&va1[i1], &pa[i1], pl1, pl2);
  }

  // delete all points & parameters not on obj
  UT3D_par_ck_inObj_del (nrp, pa, va1, pl1, pl2, 0., 1.);

  return 0;

}


//================================================================
  int UT3D_pt_ci_lim_del (int *nrp, Point *pa, double *va,
                          Circ *ci1) {
//================================================================
/// \code
/// UT3D_pt_ci_lim_del        remove points outside limited circle
/// Input:
///   nrp    nr of points on unlimited circle ci1
///   pa     points
///   va1    size nrp, can be empty or NULL
/// Output:
///   nrp    nr of points inside trimmed circle ci1
///   va1    parameter on points;
/// 
/// see UT3D_pt_ln_lim_del UT3D_ck_npt_inCirc UT3D_pt_ell_lim_del
/// \endcode


  int     irc, i1;


  // printf("UT3D_pt_ci_lim_del %d\n",*nrp);
  // for(i1=0;i1< *nrp;++i1)DEB_dump_obj__(Typ_PT,&pa[i1],"[%d] %lf",i1,va1[i1]);

  if(*nrp < 1) return 0;

//   if(!va) {
//     // get local space
//     va = MEM_alloc_tmp ((int)(sizeof(double) * *nrp));
//   }

  
  for(i1=0; i1 < *nrp; ++i1) {
    // get parameter of point on elli; positive for CCW and CW
    irc = UT3D_par_pt__pt_prj_ci (NULL, NULL, NULL, ci1, &pa[i1], 1, UT_TOL_cv);
    if(irc < 0) {
      // not on limited curve; delete this point
      MEM_del_nrec (nrp, pa, i1, 1, sizeof(Point));
      if(va) {
        *nrp += 1;
        MEM_del_nrec (nrp, va, i1, 1,sizeof(double));
      }
        // printf(" _par_ck_inObj_del delete %d ****\n",i1);
    }
//     va1[i1] = UT3D_par1_ci_pt (ci1, NULL, &pa[i1]);
      // printf(" [%d] va=%lf da=%lf\n",i1,va1[i1],da[i1]);
  }


//   // delete all points & parameters not on obj
//   UT3D_par_ck_inObj_del (nrp, pa, va1, &ci1->p1, &ci1->p2, 0., 1.);

    // printf("ex _ci_lim_del %d\n",*nrp);


  return 0;

}

 
//========================================================================
  int UT3D_par_pt_2pt (double *par1, Point *px, Point *pl1, Point *pl2) {
//========================================================================
/// \code
/// UT3D_par_pt_2pt            get parameter of point px on line pl1 - pl2
///
/// pl ist zwischen 0 bis 1; wenn pl=0.5 liegt ptx
/// genau in der Mitte von pl1-pl2
///
///
///                ptx
///                 X
///                /|
///               / |
///              /  |
///             /   |
///            /   .| 
///           X-----|------------X
///         pl1     pl=0.4       pl2
/// RetCodes:
///    0  OK
///   -1  v1-v2 aufeinander Normal; pl = 0.
/// \endcode


  Vector  vl, vp;


  // DEB_dump_obj__ (Typ_PT, px, "UT3D_par_pt_2pt ");


  UT3D_vc_2pt (&vl, pl1, pl2);
    // DEB_dump_obj__ (Typ_VC, &vl, "  vl ");
  UT3D_vc_2pt (&vp, pl1, px);
    // DEB_dump_obj__ (Typ_VC, &vp, "  vp ");

  UT3D_parvc_2vc (par1, &vp, &vl);
    // printf(" par1=%f\n",*par1);
          
  return 0; 
          
}         

        
//================================================================
  double UT3D_par1_ci_angr (Circ *ci1, double angr) {
//================================================================
/// \code
/// UT3D_par1_ci_angr              get parameter 0-1 for circ from opening-angle
///   angr            opening-angle to vector (ci.pc - ci.p1)
/// RetCod:   parameter 0-1;
///           0. = startpoint of circ, 1. = endpoint of circ.
///
/// see also UT3D_angr_ci_par1
/// \endcode

  return (fabs(angr / ci1->ango));

}


//============================================================================
  int UT3D_pt_int2ln (Point *ip1, Point *ip2, double *dist,
                      Line *ln1, Line *ln2) {
//============================================================================
/// \code
/// UT3D_pt_int2ln            intersect 2 unlimitedLines; get 2 points & dist.
///
/// IN:
///   Line *ln1        line 1
///   Line *ln2        line 2
///   Point *ip1       NULL: do not compute ip1
///   double *dist     NULL = do not compute dist
/// OUT:
///   Point *ip1       point on line 1;
///   Point *ip2       point on line 2
///   double *dist     minimal distance between ip1 and ip2
///   Returncodes:
///    -1 = Lines parallel or zero-length
///     0 = OK; dist gives the minimum distance.
///     1 = OK; distance (ip1-ip2) is less than UT_TOL_pt
///
/// intersect limited lines: UT3D_pt_int2pt2pt_lim
/// \endcode

  return UT3D_pt_intlnln (ip1, ip2, dist, ln1, 1, ln2, 1);

}


//============================================================================
  int UT3D_pt_intlnln (Point *ip1, Point *ip2, double *dist,
                       Line *ln1, int ln1Mode,
                       Line *ln2, int ln2Mode) {
//============================================================================
/// \code
/// UT3D_pt_int2ln            intersect 2 Lines both limited or unlimited
/// get 2 points & dist.
///
/// IN:
///   Line *ln1        line 1
///   ln1Mode          0=limited Line, 1=unlimited Line.
///   Line *ln2        line 2
///   ln2Mode          0=limited Line, 1=unlimited Line.
///   Point *ip1       NULL: do not compute ip1
///   double *dist     NULL = do not compute dist
/// OUT:
///   Point *ip1       point on line 1;
///   Point *ip2       point on line 2
///   double *dist     minimal distance between ip1 and ip2
///   Returncodes:
///    -2 = no intersection inside the segments
///    -1 = Lines parallel or zero-length
///     0 = OK; dist gives the minimum distance.
///     1 = OK; distance (ip1-ip2) is less than UT_TOL_pt
/// \endcode

// see UT2D_pt_intlnln

  int      irc;
  double   par1, par2, tol;
  Vector   vc1, vc2;
  Point    pb1, pb2;


  tol = UT_TOL_cv; //cv|pt;


  // printf("UT3D_pt_intlnln %d %d tol=%f\n",ln1Mode,ln2Mode,tol);
  // DEB_dump_obj__ (Typ_LN, ln1, " ln1 =");
  // DEB_dump_obj__ (Typ_LN, ln2, " ln2 =");


  // vector vc2 from ln2
  UT3D_vc_ln (&vc1, ln1);
  UT3D_vc_ln (&vc2, ln2);



  // get intersection-parameters of intersectionPt on ln1, ln2
  irc = UT3D_2par_int2pt2vc (&par1, &par2, &ln1->p1, &vc1, &ln2->p1, &vc2);
    // printf(" _2par irc=%d par1=%f par2=%f\n",irc,par1,par2);
  if(irc < 0) goto L_fertig;



  //================================================================
  // // TEST
  // UT3D_pt_projptptvc (ip1, &par1, NULL, &ln1->p1, &ln2->p1, &vc2);
    // printf(" dist=%lf\n",par1);
  // if(par1 < tol) {
    // exit(0);
    // return 0;
  // }

  //================================================================




  // compute intersectionPoint ip1 on unlimitedLine pt1/vc1
  if((ip1 != NULL)||(dist != NULL)||(ln1Mode < 1)) {
    UT3D_pt__par_pt_vc (ip1, par1, &ln1->p1, &vc1);
      // DEB_dump_obj__ (Typ_PT, ip1,  " ip1 =");
      // GR_tDyn_symB__ (ip1, SYM_STAR_S, 9);
  }


  // compute intersectionPoint ip2 on Line ln2
  UT3D_pt__par_pt_vc (ip2, par2, &ln2->p1, &vc2);
    // DEB_dump_obj__ (Typ_PT, ip2,  " ip2 =");
    // GR_tDyn_symB__ (ip2, SYM_STAR_S, 9);


  // compute dist - distance ip1-ip2
  irc = 0;
  if(dist != NULL) {
    *dist = UT3D_len_2pt (ip1, ip2);
      // printf(" dist=%f\n",*dist);
    if (*dist < UT_TOL_pt) {
      irc = 1;
    }
  }

  
  // test outside & endpoints if limited
  if(ln1Mode < 1) {
    // l1 is limited
    if(par1 < 0.) if(UT2D_comp2pt(ip1, &ln1->p1, tol) == 0) goto L_out;
    if(par1 > 1.) if(UT2D_comp2pt(ip1, &ln1->p2, tol) == 0) goto L_out;
  }   

  if(ln2Mode < 1) {         
    // l2 is limited
    if(par2 < 0.) if(UT2D_comp2pt(ip2, &ln2->p1, tol) == 0) goto L_out;
    if(par2 > 1.) if(UT2D_comp2pt(ip2, &ln2->p2, tol) == 0) goto L_out;
  }

  L_fertig:
    // printf("ex UT3D_pt_intptvcln %d %f\n",irc,*dist);

  return irc;


  L_out:  // outside segment
   irc = -2;
   goto L_fertig;

}


//=======================================================================
  int UT3D_pt_intcici (Point pa[], Circ *ci1, int ci1Lim,
                                   Circ *ci2, int ci2Lim) {
//=======================================================================
/// \code
/// UT3D_pt_intcici            intersect CIR CIR (gives 0/1/2 points)
/// 
/// IN:
///   Line *ln        3D-line
///   Circ *ci1       3D-circle
///   ci1Lim1         0=limited Circ, 1=unlimited Circ
///   Circ *ci2       3D-circle
///   ci1Lim2         0=limited Circ, 1=unlimited Circ
/// OUT:
///   Point pa[]      array of intersection points; size 2
///   Returncode:
///     -2            concentric (same center, radius, z.vec)
///     -1            no intersection
///     1             1 intersectionpoint (pa[0])
///     2             2 intersectionpoints (pa[0] and pa[1])
/// \endcode


  int       irc, i1, zParl;
  double    dx, dy, rd1, rd2, lc;
  Point     pt1;
  Vector    vcc, vcn;
  Plane     pl1;



  // DEB_dump_obj__ (Typ_CI, ci1, "UT3D_pt_intcici ci1: Lim=%d\n",ci1Lim);
  // DEB_dump_obj__ (Typ_CI, ci2, "                ci2: Lim=%d\n",ci2Lim);
  // printf(" tolpt=%f tolcv=%f\n",UT_TOL_pt,UT_TOL_cv);



  rd1 = fabs(ci1->rad);
  rd2 = fabs(ci2->rad);
  irc = -1;


  // distance pc-pc
  lc  = UT3D_len_2pt (&ci1->pc, &ci2->pc);
    // printf(" lc=%f\n",lc);


  // lc > r1 + r2: cannot intersect / touch
  if(lc > (rd1 + rd2 + UT_TOL_pt)) goto L_exit;


  // check if z-axes parallel; 1=yes, 0=not
  zParl = UT3D_comp2vc_d (&ci1->vz, &ci2->vz, UT_TOL_pt);
    // printf(" zParl=%d\n",zParl);


  // zwei Kreise mit gleichem center und gleichem radius aber anderem Z-vektor
  if(lc < UT_TOL_min1) {
      // printf("  circles have same center\n");

    if(fabs(rd1-rd2) > UT_TOL_pt) {
        // printf(" same center, different radius\n");
      irc = -2;
      goto L_exit;
    }

    // same center, same radius
    // z-vector must be different
    if(zParl) {
        // printf(" same center, same radius, same z-vector\n");
      goto L_exit;
    }
    // same center, different z-vectors,
    // Kreuzvec 
    UT3D_vc_perp2vc (&vcn, &ci1->vz, &ci2->vz);
    UT3D_pt_traptvclen (&pa[0], &ci1->pc, &vcn, rd1);
    UT3D_pt_traptvclen (&pa[1], &ci1->pc, &vcn, -rd1);
    irc = 2;
    goto L_ck_c21;
  }



  //================================================================
  // check if z-axes parallel
  
  // 2 circles - axis are not parallel
  if(zParl) goto L_parl_ci;
    // printf(" axes_not_parl_ci:\n");


  // make Plane of circ1
  UT3D_pl_ci (&pl1, ci1);
    // DEB_dump_obj__ (Typ_PLN, &pl1, " pln from ci1:\n");


  // // Schnitt Plane mit circ2
  i1 = UT3D_pt_intplnci (pa, &pl1, ci2, ci2Lim);
  if(i1 < 0) goto L_exit;
    // if(i1 > 0) DEB_dump_obj__ (Typ_PT, &pa[0], " pa0:\n");
    // if(i1 > 1) DEB_dump_obj__ (Typ_PT, &pa[1], " pa1:\n");

    // TESTDISP:
    // if(i1 > 0) GR_tDyn_symB__ (&pa[0], SYM_STAR_S, 2);
    // if(i1 > 1) GR_tDyn_symB__ (&pa[1], SYM_STAR_S, 2);


  // Abstand der Schnittpunkte auf c2 vom c1-center testen
  // hier kann es 2 Punkte geben.
    // printf(" L_ck_np: i1=%d\n",i1);
  if(i1 > 1) {
    lc = UT3D_len_2pt (&pa[1], &ci1->pc);
    lc -= rd1;
      // printf(" A:i1=%d lc=%f\n",i1,lc);
    if(fabs(lc) > UT_TOL_cv) --i1;                   // ! UT_TOL_pt
  }

  if(i1 > 0) {
    lc = UT3D_len_2pt (&pa[0], &ci1->pc);
    lc -= rd1;
      // printf(" B:i1=%d lc=%f\n",i1,lc);
    if(fabs(lc) > UT_TOL_cv) {                   // ! UT_TOL_pt
        // printf(" pt0 = pt1 \n");
      if(i1 > 1) pa[0] = pa[1];
      --i1;
    }
  }
  irc = i1;
    // printf(" L_ck_np: irc=%d\n",irc);
  // check only if point is on ci1Seg; ci2 already tested.
  goto L_ck_c1;





  //================================================================
  // 2 circles - axis are parallel
  L_parl_ci:
    // printf("L_parl_ci:\n");

  // vcc  = Vec pc-c1 -> pc-c2
  UT3D_vc_2pt (&vcc, &ci1->pc, &ci2->pc);


  // check for single solution = touching circles.
  // printf(" %f %f %f\n",lc+rd1-rd2,lc+rd2-rd1,lc-rd1-rd2);
  if(fabs(lc-rd1-rd2) < UT_TOL_pt) {
      // printf(" touch out ..\n");
    // Kreise beruehren von aussen.
    UT3D_pt_traptvclen (&pa[0], &ci1->pc, &vcc, rd1);
    irc = 1;
    goto L_ck_c21;
  }
  if(fabs(lc+rd1-rd2) < UT_TOL_pt) {
      // printf(" touch - c1 in c2 ..\n");
    // Kreise beruehren von innen; c1 liegt in c2 
    UT3D_pt_traptvclen (&pa[0], &ci1->pc, &vcc, -rd1);
    irc = 1;
    goto L_ck_c21;
  }
  if(fabs(lc+rd2-rd1) < UT_TOL_pt) {
      // printf(" touch - c2 in c1 ..\n");
    // Kreise beruehren von innen; c2 liegt in c1.
    UT3D_pt_traptvclen (&pa[0], &ci1->pc, &vcc, lc+rd2);
    irc = 1;
    goto L_ck_c21;
  }


  // normal intersection; solve 3angle 
  i1 = UT2D_solvtri_abc (&dx, &dy, rd1, rd2, lc);
    // printf(" UT2D_solvtri_abc i1=%d\n",i1);
  if(i1 < 0) return -1;
    // printf(" rd1/2=%f,%f dx=%f dy=%f lc=%f\n",rd1,rd2,dx,dy,lc);


  UT3D_pt_traptvclen (&pt1, &ci2->pc, &vcc, -dx);


  // // dy = 0: Kreise beruehren.
  // if(UTP_compdb0(dy, UT_TOL_pt)) {
    // irc = 1;
    // pa[0] = pt1;
    // goto L_ck_c21;
  // }


  // vcn = normal auf vcc / ci1-vz
  UT3D_vc_perp2vc (&vcn, &vcc, &ci1->vz);
  UT3D_pt_traptvclen (&pa[0], &pt1, &vcn, dy);
  UT3D_pt_traptvclen (&pa[1], &pt1, &vcn, -dy);

  irc = 2;



  L_ck_c21:
  // check if intersection-points are on ci2
    // printf(" L_ck_c21: %d\n",irc);
  if(ci2Lim < 1)
  irc = UT3D_ck_npt_inCirc (pa, irc, ci2);

  L_ck_c1:
  // check if intersection-points are on ci1Seg; ci2 already tested.
    // printf(" L_ck_c1: %d\n",irc);
  if(ci1Lim < 1)
  irc = UT3D_ck_npt_inCirc (pa, irc, ci1);


  L_exit:
    // printf("ex UT3D_pt_intcici irc=%d\n",irc);
    // if(irc>0)DEB_dump_obj__ (Typ_PT, &pa[0], "    p0:\n");
    // if(irc>1)DEB_dump_obj__ (Typ_PT, &pa[1], "    p1:\n");

  return irc;

}


//================================================================
  int UT3D_bpd_bp_pt (double *hbp, int bp, Point *pt) {
//================================================================
// UT3D_bpd_bp_pt         get disptance of point from backplane
// Hoehenwert der Backplane aus Punkt uebernehmen


  if(bp == 0) {        // BCKPLN_YZ
    *hbp = pt->x;

  } else if(bp == 1) { // BCKPLN_XZ
    *hbp = pt->y;

  } else if(bp == 2) { // BCKPLN_XY
    *hbp = pt->z;
  }

    // printf("ex UT3D_hbp_bppt %f\n",*hbp);

  return 0;

}


//================================================================
  int UT3D_bp_pl__ (double *bpd, Plane *plni) {
//================================================================
// UT3D_bp_pl__              get backplane+offset for plane
// Output:
//   bpd         offset from backplane
//   retCode     2=BCKPLN_XY; 1=BCKPLN_XZ; 0=BCKPLN_YZ; -1=free-plane 
//
// see also UT3D_bp_perp_vc

  int    bpi, isParl;


  DEB_dump_obj__ (Typ_PLN, plni, "UT3D_bp_pl__");

  bpi = UT3D_bp_perp_vc (&isParl, &plni->vz);

  if(isParl) {
    // backplane bpi is not exact normal to backplane;
    bpi = BCKPLN_FREE;
  }

  *bpd = plni->p;

    printf("ex-UT3D_bp_pl__  bpi = %d bpd = %f\n",bpi,*bpd);

  return bpi;

}


//======================================================================
  int UT3D_ln_tra_ln2_bp (Line *ln3, Line2 *ln2, int bpi, double *bpd) {
//======================================================================
// UT3D_ln_tra_ln2_bp   transfer 2D-line onto 3D-plane from bp and bpd

  UT3D_pt_tra_pt2_bp (&ln3->p1, &ln2->p1, bpi, bpd);
  UT3D_pt_tra_pt2_bp (&ln3->p2, &ln2->p2, bpi, bpd);

  ln3->typ = ln2->typ;

  return 0;

}


//======================================================================
  int UT3D_ci_tra_ci2_bp (Circ *ci3, Circ2 *ci2, int bpi, double *bpd) {
//======================================================================
// UT3D_ci_tra_ci2_bp   transfer 2D-Circ onto 3D-plane from bp and bpd

  UT3D_pt_tra_pt2_bp (&ci3->p1, &ci2->p1, bpi, bpd);
  UT3D_pt_tra_pt2_bp (&ci3->p2, &ci2->p2, bpi, bpd);
  UT3D_pt_tra_pt2_bp (&ci3->pc, &ci2->pc, bpi, bpd);

  UT3D_vcz_bp (&ci3->vz, bpi);

  ci3->rad  = ci2->rad;
  ci3->ango = ci2->ango;

  return 0;

}


//====================================================================
  int UT3D_vc_tra_vc2_rsys (Vector *vc3, Vector2 *vc2, Refsys *rSys) {
//====================================================================
// UT3D_vc_tra_vc2_rsys            transf. 2D-Vector => 3D-Vector

  Vector  vcx;

  if(rSys->bpi >= 0) {
    // transf. from Backplane
    return UT3D_vc_tra_vc2_bp (vc3, vc2, rSys->bpi, &rSys->bpd);

  } else {
    // transf. from free plane (4x3-matrix)
    UT3D_vc_vc2 (&vcx, vc2);
    UT3D_vc_tra_vc_m3 (vc3, rSys->mat1, &vcx);
    return 0;
  }

}


//====================================================================
  int UT3D_pt_tra_pt2_rsys (Point *pt3, Point2 *pt2, Refsys *rSys) {
//====================================================================
// UT3D_pt_tra_pt2_rsys            transf. 2D-Point => 3D-Point

  Point  ptx;

  if(rSys->bpi >= 0) {
    // transf. from Backplane
    return UT3D_pt_tra_pt2_bp (pt3, pt2, rSys->bpi, &rSys->bpd);

  } else {
    // transf. from free plane (4x3-matrix)
    UT3D_pt_pt2_0 (&ptx, pt2);
    UT3D_pt_tra_pt_m3 (pt3, rSys->mat1, &ptx);
    return 0;
  }

}


//====================================================================
  int UT3D_ln_tra_ln2_rsys (Line *ln3, Line2 *ln2, Refsys *rSys) {
//====================================================================
// UT3D_ln_tra_ln2_rsys            transf. 2D-Line => 3D-Line

  Line  lnx;

  if(rSys->bpi >= 0) {
    // transf. from Backplane
    return UT3D_ln_tra_ln2_bp (ln3, ln2, rSys->bpi, &rSys->bpd);

  } else {
    // transf. from free plane (4x3-matrix)
    UT3D_ln_ln2 (&lnx, ln2);
    UT3D_ln_tra_ln_m3 (ln3, rSys->mat1, &lnx);
    return 0;
  }

}


//====================================================================
  int UT3D_ci_tra_ci2_rsys (Circ *ci3, Circ2 *ci2, Refsys *rSys) {
//====================================================================
// UT3D_ci_tra_ci2_rsys            transf. 2D-Circ => 3D-Circ

  Circ  cix;

  if(rSys->bpi >= 0) {
    // transf. from Backplane
    return UT3D_ci_tra_ci2_bp (ci3, ci2, rSys->bpi, &rSys->bpd);

  } else {
    // transf. from free plane (4x3-matrix)
    UT3D_ci_ci2 (&cix, ci2);
    UT3D_ci_tra_ci_m3 (ci3, rSys->mat1, &cix);
    return 0;
  }

}


//================================================================
  void UT3D_ln_tra_ln_m3 (Line *lno, Mat_4x3 trmat, Line *lni) {
//================================================================
// UT3D_ln_tra_ln_m3              apply transformation to line (4x3-matrix)
// see also UT2D_pt_tra_pt3_m3

    UT3D_pt_tra_pt_m3 (&lno->p1, trmat, &lni->p1);
    UT3D_pt_tra_pt_m3 (&lno->p2, trmat, &lni->p2);

}


//================================================================
  void UT3D_ci_tra_ci_m3 (Circ *cio, Mat_4x3 trmat, Circ *cii) {
//================================================================
// UT3D_ci_tra_ci_m3              apply transformation to circle (4x3-matrix)
// see also UT2D_pt_tra_pt3_m3


    UT3D_pt_tra_pt_m3 (&cio->p1, trmat, &cii->p1);
    UT3D_pt_tra_pt_m3 (&cio->p2, trmat, &cii->p2);
    UT3D_pt_tra_pt_m3 (&cio->pc, trmat, &cii->pc);
    UT3D_vc_tra_vc_m3 (&cio->vz, trmat, &cii->vz);
    UT3D_vc_normalize (&cio->vz, &cio->vz);
    cio->rad = cii->rad;
    cio->ango = cii->ango;

}


//================================================================
  int UT3D_vc_perp_bp (Vector *vcz, int bpi) {
//================================================================
// UT3D_vc_perp_bp                   get normalvector for backplane bpi


  if       (bpi == BCKPLN_XY) *vcz = UT3D_VECTOR_Z;
    else if(bpi == BCKPLN_XZ) *vcz = UT3D_VECTOR_Y;
    else if(bpi == BCKPLN_YZ) *vcz = UT3D_VECTOR_X;

    // DEB_dump_obj__(Typ_VC, vcz, "ex-UT3D_vc_perp_bp");

  return 0;

}


//================================================================
  int UT3D_rsys_pl (Refsys *rsys, Plane *oPln) {
//================================================================
// UT3D_rsys_pl             get backplane/transformation-matrix for plane

  int    ii, typ1, bpi, isParl;
  double d1 = 1.;
  // Plane  *pln1;


  // DEB_dump_obj__ (Typ_PLN, oPln, "UT3D_rsys_pl");
  // DEB_dump_ox__ (oxpl);


  // get plane-data
  // OGX_GET_OBJ (&typ1, &pln1, &ii, oxpl);
  // if(typ1 != Typ_PLN) {return MSG_ERR__ (ERR_func_not_impl, "UT3D_rsys_pl-E1");}
    // DEB_dump_obj__ (Typ_PLN, pln1, "UT3D_rsys_pl-pln1");

  rsys->pln = oPln;

  // test if pln is backplane ..
  // get longest vector of plane
  rsys->bpv = UT3D_bp_perp_vc (&isParl, &oPln->vz);


  // test if plane is backplane; (bpi < 0)=no, else yes.
  // update plni
  if(!isParl) {
    rsys->bpi = rsys->bpv;
    // invert bpd for inverted normal-vector
    if(rsys->bpi == BCKPLN_XY) {
      if(oPln->vz.dz < 0.) d1 = -1.;
    } else if(rsys->bpi == BCKPLN_XZ) {
      if(oPln->vz.dy < 0.) d1 = -1.;
    } else if(rsys->bpi == BCKPLN_YZ) {
      if(oPln->vz.dx < 0.) d1 = -1.;
    }
    rsys->bpd = oPln->p * d1;
    goto L_exit;            // COMMENT OUT FOR FREE-PLANE-TEST
  }

  //----------------------------------------------------------------
  // free-plane backplane bpi is not exact normal to backplane;
  rsys->bpi = BCKPLN_FREE;

  // get mat1 = 2D->3D-matrix from plane
  UT3D_m3_loadpl (rsys->mat1, oPln);
    // DEB_dump_obj__ (Typ_M4x3, rsys->mat1, "mat1");

  // get mat2 = 3D->2D-matrix
  UT3D_m3_invm3 (rsys->mat2, rsys->mat1);
    // DEB_dump_obj__ (Typ_M4x3, rsys->mat2, "mat2");



  //----------------------------------------------------------------
  L_exit:

    // DEB_dump_obj__ (Typ_Refsys, rsys, "ex-UT3D_rsys_pl");

  return 0;

}


//==================================================================
  int UT3D_rsys_bp (Refsys *rsys, Plane *pln, int bpi, double bpd) {
//==================================================================
// UT3D_rsys_bp           get Refsys and plane for backplane
// Input:
//   bpi      backplane; eg BCKPLN_XY for X-Y-plane (2D)
//   bpd      offset from origin


  // printf("UT3D_rsys_bp bpi=%d bpd=%f\n",bpi,bpd);

  // get plane
  UT3D_pl_bp (pln, bpi, bpd);

  rsys->pln = pln;
  rsys->bpi = bpi;
  rsys->bpd = bpd;

  // get mat1 = 2D->3D-matrix from plane
  UT3D_m3_loadpl (rsys->mat1, pln);
    // DEB_dump_obj__ (Typ_M4x3, rsys->mat1, "mat1");

  // get mat2 = 3D->2D-matrix
  UT3D_m3_invm3 (rsys->mat2, rsys->mat1);
    // DEB_dump_obj__ (Typ_M4x3, rsys->mat2, "mat2");

    // DEB_dump_obj__ (Typ_Refsys, rsys, "ex-UT3D_rsys_bp");

  return 0;

}

 
//====================================================================
  int UT3D_pt_tra_pt_rsys (Point *pt2, Point *pt3, Refsys *rSys) {
//====================================================================
// UT3D_pt_tra_pt_rsys             transf. 3D-Point => 2D-Point with Z-value

  if(rSys->bpi >= 0) {
    // transf. onto Backplane
    UT3D_pt_tra_pt_bp (pt2, pt3, rSys->bpi);
    // UT2D_pt_tra_pt3_bp (pt2, pt3, rSys->bpi);
    return 0;

  } else {
    // transf. onto free plane (4x3-matrix)
    UT3D_pt_tra_pt_m3 (pt2, rSys->mat2, pt3);
    return 0;
  }

}


//========================================================================
  int UT3D_pl_lnz_ptx (Plane *pl1, double *dx, CVLn3 *lnz, Point *ptx) {
//========================================================================
// UT3D_pl_lnz_ptx           plane from Z-axis (Typ_CVLN3), point on X-axis
// Input:
//   lnz        Z-point + vector, MUST BE NORMALIZED
//   ptx        point on the X-axis
// Output:
//   dx         distance ptx - Z-axis; NULL for none


  // int    irc;
  double d1;
  Vector vx;


  // printf("UT3D_pl_lnz_ptx ----------\n");
  // DEB_dump_obj__ (Typ_CVLN3, lnz, " lnz");
  // DEB_dump_obj__ (Typ_PT, ptx, " ptx");


  pl1->vz = lnz->vcl;

  // get origin = project ptx onto vz
  UT3D_pt_proj_pt_pt_vcn (&pl1->po, &d1, ptx, &lnz->pt0, &lnz->vcl);

  // get x-vec
  UT3D_vc_2pt (&vx, &pl1->po, ptx);
  UT3D_vc_div_d (&pl1->vx, &vx, d1);


  // get y-vec
  UT3D_vc_perp2vc (&pl1->vy, &pl1->vz, &pl1->vx);

  UT3D_pl_p (pl1);

  if(dx) *dx = d1;

    // DEB_dump_obj__ (Typ_PLN, pl1, " ex-UT3D_pl_lnz_ptx");

  return 0;

}


//================================================================
  int UT3D_cvLn_2pt (CVLn3 *cvLn, Point *pt1, Point *pt2) {
//================================================================
// UT3D_cvLn_obj             get line (Typ_CVLN3) from 2 points
//
// see UT3D_cvLn_obj

  cvLn->pt0 = *pt1;

  UT3D_vc_2pt (&cvLn->vcl, pt1, pt2);

  cvLn->lnl = UT3D_len_vc (&cvLn->vcl);

  UT3D_vc_div_d (&cvLn->vcl, &cvLn->vcl, cvLn->lnl);

    // DEB_dump_obj__ (Typ_CVLN3, cvLn, " ex-UT3D_cvLn_2pt");

  return 0;

}

 
//================================================================
  int UT3D_cvLn_obj (CVLn3 *cvLn, int oTyp, void *oDat) {
//================================================================
// UT3D_cvLn_obj             get line (Typ_CVLN3) from PT|LN|CI|PLN (eg axis)
// retCod     0=OK-pta-vca; 1=vca-only; -1=error
// see UT3D_ptvc_ox

  int  irc = 0;

  // DEB_dump_obj__ (oTyp, oDat, " UT3D_cvLn_obj:");

  if(oTyp == Typ_PT) {
   cvLn->pt0 = *((Point*)oDat);
   cvLn->vcl = UT3D_VECTOR_Z;

  } else if(oTyp == Typ_LN) {
   cvLn->pt0 = ((Line*)oDat)->p1;
   UT3D_vc_ln (&cvLn->vcl, (Line*)oDat);
   cvLn->lnl = UT3D_len_vc (&cvLn->vcl);
   UT3D_vc_div_d (&cvLn->vcl, &cvLn->vcl, cvLn->lnl);

  } else if(oTyp == Typ_CI) {
   cvLn->pt0 = ((Circ*)oDat)->pc;
   cvLn->vcl = ((Circ*)oDat)->vz;
   cvLn->lnl = 1.;

  } else if(oTyp == Typ_VC) {
   cvLn->vcl = *((Vector*)oDat);
   cvLn->lnl = UT3D_len_vc (&cvLn->vcl);
   UT3D_vc_div_d (&cvLn->vcl, &cvLn->vcl, cvLn->lnl);
   irc = 1;

  } else if(oTyp == Typ_PLN) {
   cvLn->pt0 = ((Plane*)oDat)->po;
   cvLn->vcl = ((Plane*)oDat)->vz;
   cvLn->lnl = 1.;

  } else {
    TX_Error("UT3D_cvLn_obj E001");
    return -1;
  }

    // DEB_dump_obj__ (Typ_CVLN3, cvLn, " ex-UT3D_cvLn_obj");

  return irc;

}


//=========================================================================
  int UT3D_pl_pto_vcxn_pt (Plane *pl1, Point *po, Vector *vx, Point *pt1) {
//=========================================================================
// UT3D_pl_pto_vcxn_pt       plane from Origin, X-vec(normalized), point in plane
// vx of plane  = vx


  pl1->po = *po;
  pl1->vx = *vx;
  
  // get y-vec 
  UT3D_vc_2pt (&pl1->vy, &pl1->po, pt1);
  
  // get z-vec  from vx and vy
  UT3D_vc_perp2vc (&pl1->vz, &pl1->vx, &pl1->vy);
  UT3D_vc_setLength (&pl1->vz, &pl1->vz, 1.);

  // recompute y-vec from vx and vz
  UT3D_vc_perp2vc (&pl1->vy, &pl1->vz, &pl1->vx);

  UT3D_pl_p (pl1);

    // DEB_dump_obj__ (Typ_PLN, pl1, " ex-UT3D_pl_pto_vcxn_pt");

  return 0;

}


//================================================================
  int UT3D_ci_3pt_vc_rd_ango (Circ *ci1,
                              Point *p1, Point *p2, Point *pc,
                              Vector *vz, double *rd, double *ango) {
//================================================================
/// \code
/// UT3D_ci_3pt_vc_rd_ango    Circ from startPt endPt centerPt Z-vec rad ango
/// 
/// Input:
///   p1, p2, pc      startpoint, endpoint, centerpoint
///   vz              normal-vector; must be normalized
///   rd              radius; positiv for CCW, negativ for CW
///   ango            opening-angle in rad, negativ for CW
/// 
/// \endcode


  ci1->p1 = *p1;
  ci1->p2 = *p2;
  ci1->pc = *pc;

  ci1->vz = *vz;
  ci1->rad = *rd;
  ci1->ango = *ango;

  return 0;

}


//================================================================
  int UT3D_pt_proj_pt_pt_vcn (Point *pp, double *len,
                              Point *pt, Point *pl, Vector *vln) {
//================================================================
/// \code
/// UT3D_pt_proj_pt_pt_vcn    project point pt onto line pl+vcl (len=1)
/// Input:
///   pt        point to project
//    pl,vl     line; vector MUST BE NORMALIZED
/// Output:
///   pp       point pt projected onto line pl+vl
///   len      length pp - pt
///   retcod   0=OK,
///           -1=Inputerror (vl too short)
/// 
/// pl - vl gives a line, pt is projected onto this line.
/// 
///            X pt
///            |
///         len|
///            |
///      x-----x------->vl
///     pl     pp
/// 
/// 
/// E001 vl too short
/// 
/// see also UT3D_pt_ck_ptvc UT3D_pt_ck_onLine UT3D_nlen_2ptvc UT3D_pt_projptptvc
///   UT3D_pt_projptln 
/// \endcode


   double  sprod, dx, dy, dz;
   Vector  vp;


  // DEB_dump_obj__ (Typ_PT, pt, "UT3D_pt_proj_pt_pt_vcn pt");
  // DEB_dump_obj__ (Typ_PT, pl, "  pl");
  // DEB_dump_obj__ (Typ_VC, vln, "  vln");


  // change linstartpoint > ext.point into vector vp
  UT3D_vc_2pt (&vp, pl, pt);

  // Project vp on vl. (dot-product)
  sprod = vln->dx*vp.dx + vln->dy*vp.dy + vln->dz*vp.dz;
    // printf("  sprod = %f\n",sprod);


  pp->x = pl->x + vln->dx * sprod;
  pp->y = pl->y + vln->dy * sprod;
  pp->z = pl->z + vln->dz * sprod;

  dx = pt->x - pp->x;
  dy = pt->y - pp->y;
  dz = pt->z - pp->z;

  *len = sqrt((dx*dx) + (dy*dy) + (dz*dz));

    // DEB_dump_obj__ (Typ_PT, pp, "  ex-UT3D_pt_proj_pt_pt_vcn pt");
    // printf(" len = %f\n",*len);

  return 0;

}


//================================================================
  int UT3D_vcn_len__ln (Vector *vco, double *lnl, Line *lni) {
//================================================================
// UT3D_vcn_len__ln              get normalized vector and length of line
//   Output: 
//     vco          vector - normalized (length = 1.)
//     lnl          length of line (NULL: no output)

  double  dl;
  Vector  vc1;

  // DEB_dump_obj__ (Typ_LN, lni, "UT3D_vcn_len__ln  ln1");

  // line -> vec;
  UT3D_vc_ln (&vc1, lni);

  // length
  dl = UT3D_len_vc (&vc1);

  // normalize
  UT3D_vc_div_d (vco, &vc1, dl);

  if(lnl) *lnl = dl;

  return 0;

}


//======================================================================
  int UT3D_ln_rot_90_sr (Line *lno, Line *lni, Vector *vcAxis, int sr) {
//======================================================================
// UT3D_ln_rot_90_sr         rotate line 90 deg CCW or CW around axis at startpoint
// Input:
//   sr      0=CCW, 1=CW

  Vector    vci, vco;
  double    lnl;

  // printf("UT3D_ln_rot_90_sr sr=%d\n",sr);
  // DEB_dump_obj__ (Typ_LN, lni, "  ln1");

  // vci from lni; get normalized vci and length
  UT3D_vcn_len__ln (&vci, &lnl, lni);

  // rotate
  UT3D_vc_rot_90_sr (&vco, vcAxis, &vci, sr);


  // lno from vco; multiply with length
  UT3D_vc_multvc (&vco, &vco, lnl);
  UT3D_ln_ptvc (lno, &lni->p1, &vco);

  lno->typ = lni->typ;

    // DEB_dump_obj__ (Typ_LN, lno, "  ex-UT3D_ln_rot_90_sr");

  return 0;

}


//==========================================================================
  int UT3D_vc3f_perp_3pt (Vec3f *vp, Point *ptc, Point *ptx, Point *pty) {
//==========================================================================
/// \code
/// UT3D_vc_perp_3pt           float-vector = perpendic. to 3 points (crossprod)
/// 
/// Liefert den Z-Vektor von Origin, X-AchsEndpunkt und Y-AchsEndpunkt
/// Get VZ from PO, PX, PY:    UT3D_vc_perp3pt (&vz, &po, &px, &py);
/// Get VY from PO, PX, PZ:    UT3D_vc_perp3pt (&vy, &po, &pz, &px);
/// \endcode


  float dxx,dxy,dxz,dyx,dyy,dyz;


  dxx = ptx->x - ptc->x;
  dxy = ptx->y - ptc->y;
  dxz = ptx->z - ptc->z;

  dyx = pty->x - ptc->x;
  dyy = pty->y - ptc->y;
  dyz = pty->z - ptc->z;

  vp->dx = dxy * dyz - dxz * dyy;
  vp->dy = dxz * dyx - dxx * dyz;
  vp->dz = dxx * dyy - dxy * dyx;

  return 0;

}


//================================================================
  void UT3D_vc3f_vc (Vec3f *vf, Vector *vc) {
//================================================================
// UT3D_vc3f_vc            float-vector from vector

  vf->dx = vc->dx;
  vf->dy = vc->dy;
  vf->dz = vc->dz;

}


//================================================================
  void UT3D_vc_vc3f (Vector *vc, Vec3f *vf) {
//================================================================
// UT3D_vc_vc3f            vector from float-vector

  vc->dx = vf->dx;
  vc->dy = vf->dy;
  vc->dz = vf->dz;

}


//====================================================================
  void UT3D_vc3f_setLength (Vec3f *vco, Vec3f *vci, float new_len) {
//====================================================================
/// \code
/// UT3D_vc_setLength               change vectorlength
/// 
/// vco und vci duerfen ident sein
/// 
/// use UT3D_vc_multvc with vectors of length=1
/// \endcode


  float len;

  /* printf("UT2D_vc_setLength %f %f %f %f\n", */
                   /* vci->dx,vci->dy,vci->dz,new_len); */

  len = UT3F_len_vc (vci);
    // printf(" UT3D_vc3f_setLength-len=%f\n",len);

  if (len != 0.f)
    {
      len = new_len / len;
      vco->dx = vci->dx*len;
      vco->dy = vci->dy*len;
      vco->dz = vci->dz*len;
      /* printf("UT3D_vc_setLength %f %f %f\n",vco->dx,vco->dy,vco->dz); */
    }
  else
    {
      printf("**** UT3D_vc3f_setLength error\n");
      // *vco = UT3D_VECTOR_X;
    }
}


//====================== EOF ===========================
