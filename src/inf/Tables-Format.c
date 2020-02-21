/* ../inf/Tables-Format.c


INF_DynamicDataArea
INF_permanent_attributes
INF_DisplayList



================================================================= */
void INF_DynamicDataArea (){        /*! code
================================================================= 

In every model the first block is the DynamicDataArea;
  it ends with the line ":DYNAMIC_DATA".
The DynamicDataArea can have the following commands:


MODSIZ ..
DEFTX ..
DEFCOL ..
VIEW ..
CONST_PL ..
GATAB
..
ENDGATAB
:DYNAMIC_DATA




GATAB starts the grafic-attributes-table;

Grafic-Attributes-Records for surfaces:

<ObjectID> [C<color> [H] [T#] [S#] [X"filename"<,parameters>]
 H  hidden
 C  R-G-B-Colors as 3 x 2 hex-chars; eg. green is C00ff00
 T  transparent; 0=not, 1=50% transparent, 2=full transparent
 S  symbolic; 0=not, 1=symbolic display 
 X  textured surface; Format:
    X"<Image>",X-scale,Y-Scale,X-Offset,Y-Offset,RotAng[,X-vec,Z-Vec]
      <Image>  consists of "<symbolPath>/<imagefilename>"
      Offsets are Values from 0-1
      RotAng in degree


Grafic-Attributes-Records for Lines, Curves:
 <ObjectID> [T#]
   H  hide
   T  index Linetyp; 2=dash, 3=dashDot.

# end of grafic-attributes-table
ENDGATAB



Example DynamicDataArea:

MODSIZ 500 0.005 0.05
MODBOX P(-583.3792117 -285.1265177 -6) P(9.262386051 240.5683093 50)
DEFTX 3.5 2.5 4 2
# DefaultColor
DEFCOL 224 224 242
GATAB
# Linetype; 2=dash, 3=dashDot.
L20 T2
L21 T3
# color green, semiTransparent
A20 C00ff00 T1
# textures
A21 X"Data/sample_Stein1.bmp",5,5,0,0,15
A22 X"Data/sample_Ziegel1.bmp",5,5,0,0,0
ENDGATAB
:DYNAMIC_DATA
# here starts geometric-object-space
L20=P(0 0 0) P(50 0 0)
L21=P(0 0 0) P(50 50 0)
S20=REC P(50 0 0) D(50 0 0) D(0 50 0)
S21=REC P(50 0 0) D(0 50 0) D(0 0 20)
S22=REC P(50 50 0) D(50 0 0) D(0 50 0)
A20=S20
A21=S21
A22=S22




================================================================= \endcode */}
void INF_permanent_attributes (){        /*! code
================================================================= 

PermanentAttributes in the Modelfile (see INF_DynamicDataArea):
# example: surface A20 has color e2dc27; surf. A21 is symbolic (not shaded)
GATAB
A20 Ce2dc27
A21 S1
ENDGATAB

Primary load of GATAB into PermanentAttributList GA_ObjTab:
ED_work_dyn    // after processing removes DYNAMIC_DATA-BLOCK from mainModel
  GA_load__
ED_Read_Line
  // work DYNAMIC_DATA-BLOCK in subModels, skip this block in primary-model


//----------------------------------------------------------------
// PermanentAttributRecord
typedef struct {long ind;
                unsigned long iatt;
                unsigned lay:16, typ:8,
                         unused:7, disp:1;}                         ObjAtt;
// typ    DB-Typ
// ind    DB-Index
// iatt   for typ=LN/AC/Curve: LineTypNr.
//        for typ=TAG/IMG: sx/sy
//        for typ=Surf/Model: ColRGB
// disp   0=normal; 1=hidden
// oNam   objectname; not yet impl.
// size = 12


../xa/xa_ga.c:
ObjAtt  *GA_ObjTab=NULL;         // die PermanentAttributeTable
int     GA_recNr=0;              // die aktuelle Anzahl von Records


 
Die DispList enthaelt alle fuers Display erforderlichen Infos
  (Color, Texture, symbolic-yes/no usw).
Die DispList wird bei RUN neu generiert, die GA_ObjTab ist permanent.
 Die GA_ObjTab ist eine Kopie, die erforderlich ist um den DL-Record
  wieder zu erzeugen.
In der DispList gibt es keine Info ob ein zugehoeriger GA_ObjTab-Record
  existiert.


Nur fuer typ=Surf/Model:    GA_ObjTab[i].iatt   ist eine ColRGB;

// Typ_Color
typedef struct {unsigned cr:8, cg:8, cb:8,
                  unused:3, vtra:2, vsym:1, vtex:1, color:1;}       ColRGB;
// vtra   view transparent; 0=not, 1=25% transparent, 2=50%, 3=75%
// vtex   has texture; 0=not, 1=yes; (cr+cg+cb) = textureRefNr
// vsym   view normal (0,shaded) or symbolic (1,wireframeboundary)
// color  0=color not active; 1=color from cr/cg/cb
// size = 4


GA_ObjTab ist the permanent-attributes-table (../xa/xa_ga.c).

Functions:
Color:
APcol_actColTra // set active col.
GA_Col__ // modify/reset/set-new color





================================================================= \endcode */}
void INF_DisplayList (){        /*! code
================================================================= 

see INF_DisplayList-record
    INF_Temporary-DisplayList-record


// typedef struct {long ind; int lNr; short refInd, attInd;
typedef struct {long ind, lNr;
                unsigned long iatt;
                unsigned modInd:16, typ:8,
                         disp:1,  pick:1,  hili:1,  dim:1,
                         grp_1:1, temp:1,  unvis:1, sPar:1;}        DL_Att;
// lNr    APTlineNr.
// typ    DB-Typ
// ind    DB-Index
// refInd 2d_to_3d_Ind
// modInd ModelNr
// iatt   for typ=LN/AC/Curve: LineTypNr.
//        for typ=TAG/IMG: sx/sy
//        for typ=Surf/Model: ColRGB
//        in Parent-state DL-index of child
// disp, hili:
//        normal = ((disp == 0)&&(hili == 1))
//        hilite = ((disp == 1)&&(hili == 0))          DL_OBJ_IS_HILITE
//        hidden = ((disp == 1)&&(hili == 1))
// pick   0=unpickable, 1=pickable
// unvis  0=visible; 1=obj does not have graph. representation
// sPar   0=independent; 1=Parent-state is active.
// grp_1  0=belongs to active Group, 1=not
// size = 16




../gr/ut_DL.c:
DL_Att     *GR_ObjTab;      // die DisplayList
long       GR_TAB_IND;      // naechster freier Index


Nur fuer typ=Surf/Model:    GR_ObjTab[i].iatt   ist eine ColRGB;
  genauso wie bei PermanentAttributeList. For iatt see PermanentAttributeList.



================================================================= \endcode */}
// eof
