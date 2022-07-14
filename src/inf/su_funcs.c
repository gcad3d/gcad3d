/*     ../inf/su_funcs.c

================================================================== */
void INF_surf_funcs___(){                   /*! \code

Files:
../gr/ut_gr.c         GR_tDyn_nifac
../gr/grtsu.c         GRTSU_nifac_sru
../gr/tess_su.c       TSU_DrawSurTRV
../gr/ut_GL.c         GL_set_nifac_V2
../gr/gr_sSym.c       GR_sSym_spl


INF-records:
SupportSurfaces:
  INF_FMTB_Surface_RV            OK
  INF_FMTB_Surface_RU
  INF_FMTB_Surface_SWP
  INF_FMTB_Surface_BSP
  INF_FMTB_Surface_RBSP

TrimmedPerforatedSurfaces:
  INF_FMTB_Surface_PLN           OK
  INF_FMTB_Surface_TPS           OK



//================================================================
COMPONENTS OF SURFACE:
Face    struct Fac3     triangle with 3 indices; needs pointarray
fnb     (struct Fac3)   neighbourface
nifac   group of indexed Fac3-faces with vectors;
patch   group of triangles, organized as fan,stripe or triangles.
ipatch  group of indexed triangles, organized as fan,stripe or triangles.


patch:
  int gTyp: 4=GL_TRIANGLES 5=GL_TRIANGLE_STRIP 6=GL_TRIANGLE_FAN
  int ptNr, Point *pa
Do not use glEnable(GL_NORMALIZE); // normalize nVecs (glNormal*)

  GL_set_patch
   GL_set_bMsh GR_Disp_patch 





//================================================================
FUNCTIONS FOR SURFACES:
- DB-surfaces|binary-surface: tesselate or create-symbolic-curves;
  - tesselate: create GL-patch (fan|stripe|triangles)
- create symbolic-surfaces (add additional lines to boundary) GR_sSym_*
  - display curves, not shaded faces; change GL-type / create line-loops ?
  - disp boundaries plus additional curves; (if WC_stat_bound==ON)
- store all faces into GLT_stor_rec (if TSU_mode==1)
  - with color, normalvectors, texture - for export to wrl, obj, ..
- store all faces into GLT_stor_rec (if TSU_mode==1)
   - without color, texture, only for intersections; 
   - ev auch via ifac; spart viele Punkte ..


VARIABLES:
- TSU_mode - set to 1 by intersect-surfaces; 0 = display normal
     1: no color, transparency, texture needed - only simple triangles, with fnb
- WC_stat_bound
- GL_actCol    default-flags for shade|symbolic, color,transparency,texture


//================================================================
FUNCTION-NAMES FOR SURFACES:
- GR_temp_*|GR_tDyn_*|GR_temp_* 
  - set temp, dynamic, permanent ..
  - open/close GL-list, set color,texture,transparency ..
- GR_sSym_* = create symbolic boundaries (add additional lines to boundary)
- GL_set_* = out into open GL-liste


//================================================================
 surface-display-functions und testfunctions dazu -> ../APP/DemoPlugin_Create.c

//----------------------------------------------------------------
FUNCS WITHOUT Open/close GL-list (add into open GL-list)
//----------------------------------------------------------------

GL_set_ntri           display n triangles from points
  - input n triangles in pointArray
  - add into open GL
 
GL_set_fan            display TriangFan from points
  - add into open GL
  - disp fan of points; planar or not; from points;
  - can add to GLT_stor_rec (if TSU_mode==1)
  - can disp boundary (if WC_stat_bound==ON)

GL_set_strip1         stripe from startpt and 2 pointArrays
  GR_Disp_fan / GR_Disp_su3p2 GR_Disp_su3pt GR_Disp_tris 

GL_set_strip_v         stripe from 2 pointArrays
  - disp stripe of 2 pointArrays
  - can add to GLT_stor_rec (if TSU_mode==1)
  - can disp boundary (if WC_stat_bound==ON)

GL_set_patch          display Opengl-patch
  - disp GL_TRIANGLES|GL_TRIANGLE_STRIP|GL_TRIANGLE_FAN from type and pointArray

GL_set_ipatch         display indexed-Opengl-patch (type,indexTable,points)
  - type: GL_TRIANGLE_FAN|GL_TRIANGLES|GL_TRIANGLE_STRIP

GL_set_nfac           display triangles from Fac3[], Point[], color
  - can add to GLT_stor_rec (if TSU_mode==1)
  - checks color.vsym - ??  should use (GL_FRONT_AND_BACK, GL_LINE) ??

GL_set_nifac          display indexed-Opengl-patch
  - input Fac3[], index[], Point[], nVec[], 

GR_Disp_nfac         display triangles from Fac3[], Point[], color
  - adds into open GL
  - can add to GLT_stor_rec (if TSU_mode==1)
  - use GL_set_nfac

GL_set_sur_tess       GL_Tex_set1 + GL_set_bMsh
  - activates texture !?
  - use GL_set_bMsh

GL_disp_sph
  - adds into open GL
  - can create symbolic-boundary|faces for sph
  - use GL_set_fan|GL_set_strip_v

GL_disp_cone          
  - adds into open GL
  - can disp parts of cone (bottom-face,top-face,mantel)
  - can create symbolic-boundary|faces for cone (bottom-face,top-face,mantel)
  - use GL_set_fan|GL_set_strip_v

GL_disp_tor
  - adds into open GL
  - can create symbolic-boundary|faces for torus
  - use GL_set_strip_v

GR_DrawFan           disp fan - planar or not planar
  - adds into open GL
  - can add to GLT_stor_rec (if TSU_mode==1)
  - use GL_set_fan

GR_DrawTriaFan
  - input att, point, pointArray
  - adds into open GL, sets color
  - use GL_set_fan

GR_DrawTriaStrip
  - input dli, att, 2 pointArrays (in single pointarray)
  - adds into open GL
  - can add to GLT_stor_rec (if TSU_mode==1)
  - use GL_set_strip_v

GR_DrawStrip
  - input: 2 pointArrays, flag open-new-surface
  - adds into open GL, sets color GL_actCol
  - can add to GLT_stor_rec (if TSU_mode==1)
  - use GL_set_strip_v

GL_disp_prism
  - input: 2 pointArrays, flag symbolic;
  - adds into open GL
  - can disp symbolic|shaded
  - use GR_DrawFan|GR_DrawStrip|GR_DrawSup

TSU_DrawSurBsp        B-spline support-surface
  - input dbi, att, SurBSpl
  - adds into open GL
  - can create symbolic-boundary|faces for SurBSpl
  - use GR_DrawTriaFan|GR_DrawTriaStrip                                  

TSU_DrawSurRBsp       rational-B-spline support-surface
  - input dbi, att, SurBSpl
  - adds into open GL
  - can create symbolic-boundary|faces for SurRBSpl
  - use GR_DrawTriaFan|GR_DrawTriaStrip

GL_set_bMsh           Draw 1-n Planar Patches from ObjGX (binary mesh)
  - adds into open GL; sets color, nVec; separate patches;
  - use GL_set_patch|GL_disp_sph|GL_disp_cone
  - can also set color, normalVec;

GR_DrawSup           disp planar surface, covex or concave
  - adds into open GL
  - convex - use GR_DrawFan|GLT_spp_*
  - concave - store as bMesh with GLT_spp_*, disp with GL_set_bMsh

TSU_DrawRCIR       tess/disp 3|4-point-face
  - input ogx, att
  - adds into open GL
  - can create symbolic-boundary|face
  - use GR_DrawFan

TSU_DrawRSTRIP
  - input ogx, att
  - adds into open GL
  - use GR_DrawStrip

TSU_DrawSurTRV     support-surface - RevolvedSurface
  - input ogx, att
  - adds into open GL
  - can create symbolic-boundary|face
  - can apply texture !
  - use GR_DrawStrip

TSU_DrawSurTRU     tess/disp support-surface RuledSurface
  - input ogx, att
  - adds into open GL
  - can create symbolic-boundary|face
  - can apply texture !
  - use GR_DrawStrip,GR_DrawFan

Tess_sur__         tess/disp support-surface - sweep-surface.
  - input ogx, att
  - adds into open GL
  - can create symbolic-boundary|face
  - can add to GLT_stor_rec (if TSU_mode==1)  (use Tess_bMsh_iMsh)
  - use GR_Draw_nipatch

TSU_DrawSurTC      tess/disp trimmed-perforated surface
  - input ogx, att
  - adds into open GL
  - can create symbolic-boundary|face (GR_sSym_spl)
  - can add to GLT_stor_rec (if TSU_mode==1)
  - use GL_set_sur_tess

TSU_DrawSurPMsh     surface-from-points
  - input ogx, att / pointArray
  - adds into open GL
  - can apply texture !
  - use GR_Disp_nfac

GR_disp_cv_pMesh   create/disp boundary of surface-from-points
  - input dbi, att
  - adds into open GL
  - use GL_set_ipcv (disp single GL_LINE_STRIP)

TSU_DrawHAT        hatched-surface
  - input ogx, att
  - adds into open GL
  - use GL_set_ln_2pt    TODO: use compound-struct of GL_LINE_STRIP


//----------------------------------------------------------------
FUNCS WITH Open/close GL-list, set color:
//----------------------------------------------------------------
TSU_DrawSurTess        display bMesh
  - Open/close GL-list, set color;
  - use GL_set_bMsh

GR_DrawTorSeg          disp torus (2 (limited) circs)
  - input 2 (limited) circs
  - Open/close GL-list, set color;
  - use GL_set_strip_v

GR_DrawDisk            (Kreisring)
  - Open/close GL-list, set color;
  - use GL_set_fan|GL_set_strip_v

GR_DrawPrism                                                             
  - input dli att 2 pointArrays
  - Open/close GL-list, set color;
  - can add to GLT_stor_rec (if TSU_mode==1)
  - use GL_set_strip_v

GR_CreSolCon
  - input: Conus
  - Open/close GL-list, set color;
  - use GL_disp_cone

GR_CreSolTor
  - input: Torus
  - Open/close GL-list, set color;
  - use GL_disp_tor

GR_CreSolSph
  - input: Sphere
  - Open/close GL-list, set color;
  - use GL_disp_sph

GR_CreSol__          tess/disp SPH|CON|TOR|BREP (toplevel body)
  - input dbi, att
  - Open/close GL-list, set color;
  - use all body-funcs

GR_Disp_patch         
  - Open/close GL-list, set color;
  - use GL_set_patch 

GL_DrawRSur           RuledSurf                   UU
  - input  dli, att, 2 pointArrays
  - Open/close GL-list, set color;
  - use GL_set_strip_v

GR_draw_mesh         display triangles from Fac3[], Point[], color
  - Open/close GL-list, set color;
  - use GL_set_nfac

UFA_view_nifac     DO NOT USE; replaced by GR_Draw_i2fac
  - Open/close GL-list, set color;
  - use GL_set_nifac


//----------------------------------------------------------------
FUNCS WITH create DL-RECORD + display
//----------------------------------------------------------------

GR_tDyn_nifac   display MshFac
  - input: MshFac (Fac3, ..)
  - GL_set_nifac_V2

GR_Disp_fan          GL_set_strip1 temp
  - create DL-record
  - use GL_set_strip1

GR_CreTorSeg          disp torus (2 (limited) circs)
  - create DL-record
  - use GR_DrawTorSeg

GR_CreDisk             Kreisring
  - create DL-record
  - use GR_DrawDisk            (Kreisring)

GR_CrePrism           disp stripe
  - input: 2 pointArrays
  - create DL-record
  - use GR_DrawPrism

GR_CreFan
  - create DL-record
  - can add to GLT_stor_rec (if TSU_mode==1)
  - use GR_DrawFan

GR_CreTriaFan
  - create DL-record
  - use GR_DrawTriaFan

GR_DrawSur           toplevel-func tess/disp/store surf
  - UU - now GR_perm_sur - GR_set_sur
  - input ObjGX (DB-obj ..) TSU_mode ..
  - create DL-record nur teilweise, PROBLEM !
  - Open/close GL-list nur teilweise, PROBLEM !
  - use all

GR_set_sur                NEW toplevel-func tess/disp/store surf
  - input opers dbi att  TSU_mode ..
  - create DL-record, Open/close GL-list;
  - tesselate, display|store surface from DB
  - use all


//----------------------------------------------------------------
FUNCS FOR SYMBOLIC_SURFACES:
//----------------------------------------------------------------
GR_sSym_srus         RuledSurfaces / supported (nicht getrimmt)
GR_sSym_spl          trimmed/punched surfs
GR_sSym_srv          RevolvedSurface
GR_sSym_sbsp
GR_sSym_srbsp
  - symbolic rational-B-spline


//----------------------------------------------------------------
TESTFUNKTIONEN:
//----------------------------------------------------------------

GR_Disp_tris         disp struct Triang         disp struct Triang
GR_Disp_su3pt        disp triangle from 3 points
GR_Disp_su3p2        disp triangle from 3 2D-points
  - all use GR_Disp_fan

UPAT_ipatch_disp_opts    display indexed-Opengl-patch
  - input: gTyp, Point[], index[];
  - can display faces, bounds, normalvectors,

UFA_disp__            display indexed faces (Fac3[], Point[])
  - input: Fac3[] Point[] 
  - can display faces, bounds, face-numbers, point-numbers, normalvectors,

UFA_disp_fb1          display boundary of single indexed-triangle
  - input Fac3
  - use GR_tDyn_pcv

UFA_Disp_fac2
  - disp single Fac3 with 2D-points; UFA_Disp_fac2 MSH2D_dump_brkls
  - Fac3 = struct with 3 indices to points
  - use GL_set_ntri

UFA_disp_nEdg2
  - input: Fac3[] Point2[] edges (2 intArrays)
  - disp faces, bounds

MSH2D_dump_brkls   dump crossing-edges
  - input: struct brklStru = list faces crossed by line
  - disp faces|boundaries

//----------------------------------------------------------------
TSU_test_tess__
  - old version; TessDat__ + TessStru
TSU_test_tess_PLN__
TSU_test_tess_TPS
TSU_test_tess_SS_BSP_s
TSU_tess_add_iSTRIP
TSU_test_sru
TSU_test_srv
TSU_test_tor
TSU_test_con
TSU_test_sbsp
UT3D_rbspl_tst_cc
UT3D_srbsp_tst_load             load Rat.B-Spline Surf.
UT3D_tria_tst_load              load testTriangs
UT3D_testsurf                   test SurBSpl-struct intern
OGX_tst_load_ox
test_SUR_bsp
test_SUR_pln
MSH_test_disp_*



//================================================================
Functions to store  (if TSU_mode == 1):
GLT_stor_rec


GL_set_bMsh
TSU_store
TSU_ntria_bMsh__
tess_triaNr_bMsh






================================================================== \endcode */}
// eof
